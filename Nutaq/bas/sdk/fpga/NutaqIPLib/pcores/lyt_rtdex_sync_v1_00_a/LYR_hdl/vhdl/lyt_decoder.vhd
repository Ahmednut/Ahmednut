--------------------------------------------------------------------------------
--
--    ****                              *
--   ******                            ***
--   *******                           ****
--   ********    ****  ****     **** *********    ******* ****    ***********
--   *********   ****  ****     **** *********  **************  *************
--   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
--   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
--  ****    *********  ****     ****   ****   ****       ****  ****      ****
--  ****     ********  ****    *****  ****    *****     *****  ****      ****
--  ****      ******   ***** ******   *****    ****** *******  ****** *******
--  ****        ****   ************    ******   *************   *************
--  ****         ***     ****  ****     ****      *****  ****     *****  ****
--                                                                       ****
--          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
--                                                                        ***
--
--------------------------------------------------------------------------------
-- File : lyt_decoder.vhd
--------------------------------------------------------------------------------
-- Description : VITA 49 data packet decoder (uplink and downlink)
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use ieee.std_logic_unsigned.all;
    
use work.lyt_rtdex_sync_p.all; 

entity lyt_decoder is
  port (
    i_SysClk_p                    : in std_logic;                       -- Perseus system clock: 200 Mhz      
    i_CoreReset_p                 : in std_logic;
    i_CoreEnable_p                : in std_logic;                
    iv20_RxFrameSize_p            : in std_logic_vector(19 downto 0);

    -- Decoder Status
    --
    ov32_decCurrentStreamID_p     : out std_logic_vector(31 downto 0);  -- Decoder current StreamID
    ov32_decSampleCnt_p           : out std_logic_vector(31 downto 0);  -- Sample count
    o_newVrlFrame_p               : out std_logic;                      -- New VRL frame received
    ov2_vrtSyncMode_p             : out std_logic_vector(1 downto 0);   -- Sync mode
    ov2_vrtTrigSel_p              : out std_logic_vector(1 downto 0);   -- Trigger select
    ov64_decTimeStamp_p           : out std_logic_vector(63 downto 0);  -- Decoder Timestamp
      
    -- RTDEx Sync interface
    --
    o_RTDExSyncStart_p            : out std_logic;                      -- Start RTDEx sync module
    o_RTDExSyncStop_p             : out std_logic;                      -- Stop RTDEx sync module
    iv2_RTDExSyncState_p          : in  std_logic_vector(1 downto 0);   -- RTDEx Sync state

    -- Decoder data interface
    --
    o_decFifoWen_p                : out std_logic;                      -- Decoder data fifo write enable
    ov32_decFifoWrData_p          : out std_logic_vector(31 downto 0);  -- Decoder data fifo data
    i_FsmReady_p                  : in std_logic;                       -- Finite-state machine is ready to accept new data
    
    -- Decoder error report interface
    --
    o_decVrlSyncLost_p            : out std_logic;                      -- Decoder sync lost
    o_decVrlFrameErr_p            : out std_logic;                      -- Decoder frame error
    o_decVrtPktErr_p              : out std_logic;                      -- Decoder packet error
    o_decInvalidCmd_p             : out std_logic;                      -- Decoder invalid command
    o_decInvalidVrlFrameCnt_p     : out std_logic;                      -- Decoder invalid VRL frame count
    o_decInvalidVrtPktCnt_p       : out std_logic;                      -- Decoder invalid VRT packet count

    -- RTDEx Rx interface
    --
    i_RTDExRxReady_p              : in std_logic;                       -- RTDEx receiver has valid data to read
    o_RTDExRxReadReq_p            : out std_logic;                      -- RTDEx receiver reads FIFO request
    iv32_RTDExRxData_p            : in std_logic_vector(31 downto 0);   -- RTDEx receiver output data bus
    i_RTDExRxDataValid_p          : in std_logic                        -- Data at RTDEx receiver data bus is valid
  );
end lyt_decoder;

architecture Behavioral of lyt_decoder is

  constant C_ENABLE_CRC32_CHECK          : boolean := FALSE;

  type decoder_fsm_t is (
    DEC_VRL_HDR,
    DEC_VRL_FRM_SIZE,
    DEC_VRT_HDR,
    DEC_VRT_STREAM_ID,
    DEC_VRT_TIMESTAMP_1,
    DEC_VRT_TIMESTAMP_2,
    DEC_VRT_TIMESTAMP_3,
    DEC_VRT_CONTROL,
    DEC_VRT_RESERVED,
    DEC_PUSH_FIFO,
    DEC_VRL_NEXT,
    DEC_VRL_CRC32,
    DEC_VRL_PAD_FOR_RTDEX,
    DEC_RETRIEVE_SYNC
  );
  
  signal decoder_fsm_s            : decoder_fsm_t;

  attribute fsm_encoding: string;
  attribute fsm_encoding of decoder_fsm_s: signal is "auto";
  
  -- Decoder packet type signals definition
  --
  signal v64_fracsec_timestamp_s  : std_logic_vector(63 downto 0);
  
  -- VRL
  --
  signal v20_vrlFrameSize_s       : std_logic_vector(19 downto 0);
  signal v12_vrlFrameCnt_s        : std_logic_vector(11 downto 0);
  
  -- VRT
  --
  signal v3_vrtPktType_s          : std_logic_vector(2 downto 0);
  signal v2_vrtTsf_s              : std_logic_vector(1 downto 0);
  signal v4_vrtPktCnt_s           : std_logic_vector(3 downto 0);
  signal v32_vrtCurStreamID_s     : std_logic_vector(31 downto 0);
  signal v32_vrtThisStreamID_s    : std_logic_vector(31 downto 0);
  signal v2_vrtSyncMode_s         : std_logic_vector(1 downto 0);
  signal v2_vrtTrigSel_s          : std_logic_vector(1 downto 0);

  signal v20_decWordCnt_s         : integer range 0 to 2**20-1;
  
  -- Statistics
  --
  signal newVrlFrame_s            : std_logic;

  -- CRC32 generator
  --
  signal crc32_reset_s            : std_logic;
  signal v32_vrl_crc32_s          : std_logic_vector(31 downto 0);

  -- RTDEx Rx interface
  --
  signal v32_RTDExRxData_s        : std_logic_vector(31 downto 0);
  signal RTDExRxDataValid_s       : std_logic;

  -- Deocder FIFO sample count
  --
  signal v32_decSampleCnt_s       : std_logic_vector(31 downto 0);


begin

  ---------------------------------------------------------------------------
  --  RTDEx Rx interface
  --  Add one cycle delay on the read interface to help timing
  --
  ----------------------------------------------------------------------------

  -- This assumes the reset has been applied before the core is enabled
  --
  o_RTDExRxReadReq_p <= '1' when (i_CoreEnable_p = '1' and i_RTDExRxReady_p = '1' and i_FsmReady_p = '1') else '0';

  -- Add one cycle latency to help timing
  --
  rtdex_fifo_rd_delay_l: process(i_SysClk_p)
  begin
     if rising_edge(i_SysClk_p) then
      if i_CoreReset_p = '1' then
        v32_RTDExRxData_s     <= (others => '0');
        RTDExRxDataValid_s    <= '0';
      else
        v32_RTDExRxData_s     <= iv32_rtdexrxdata_p;
        RTDExRxDataValid_s    <= i_rtdexrxdatavalid_p;
      end if;
    end if;
  end process rtdex_fifo_rd_delay_l; 
  
  
  ----------------------------------------------------------------------------
  -- CRC32 generator
  ----------------------------------------------------------------------------

  gen_crc32_check: if C_ENABLE_CRC32_CHECK = TRUE generate
    crc32_gen: entity work.lyt_crc32
    port map(
      data_in     => v32_RTDExRxData_s,
      crc_en      => RTDExRxDataValid_s,
      rst         => crc32_reset_s,
      clk         => i_SysClk_p,
      crc_out     => v32_vrl_crc32_s
    );
    
    crc32_reset_s <= i_CoreReset_p or newVrlFrame_s;
    
  end generate;

  gen_vrl_trailer_check: if C_ENABLE_CRC32_CHECK = FALSE generate
    v32_vrl_crc32_s <= C_VRL_CRC32;
  end generate;


  ----------------------------------------------------------------------------
  -- Process decoder_fsm_proc
  -- Synchronous update of next state of decoder_fsm_s
  ----------------------------------------------------------------------------
  
  decoder_fsm_proc : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if i_CoreReset_p = '1' then
        -- Downlink state control signals
        --
        o_RTDExSyncStart_p            <= '0';
        o_RTDExSyncStop_p             <= '0';

        decoder_fsm_s                 <= DEC_VRL_HDR;
        v20_decWordCnt_s              <= 0;

        -- VRL
        --
        v20_vrlFrameSize_s            <= (others=>'0');
        v12_vrlFrameCnt_s             <= (others=>'0');

        -- VRT
        --
        v3_vrtPktType_s               <= (others=>'0');
        v2_vrtTsf_s                   <= (others=>'0');
        v4_vrtPktCnt_s                <= (others=>'0');
        v64_fracsec_timestamp_s       <= (others=>'0');
        v2_vrtSyncMode_s              <= (others=>'0');
        v2_vrtTrigSel_s               <= (others=>'0');
        v32_vrtCurStreamID_s          <= (others=>'0');
        v32_vrtThisStreamID_s         <= (others=>'0');
  
        o_decVrlSyncLost_p            <= '0';
        o_decVrlFrameErr_p            <= '0';
        o_decVrtPktErr_p              <= '0';
        o_decInvalidCmd_p             <= '0';
        o_decInvalidVrlFrameCnt_p     <= '0';
        o_decInvalidVrtPktCnt_p       <= '0';

        -- FIFO interface
        --
        o_decFifoWen_p                <= '0';
        v32_decSampleCnt_s            <= (others=>'0');
  
      elsif i_CoreEnable_p = '1' then
      
        -- Default values
        --
        o_decVrlSyncLost_p            <= '0';
        o_decVrlFrameErr_p            <= '0';
        o_decVrtPktErr_p              <= '0';
        o_decInvalidCmd_p             <= '0';
        o_decInvalidVrlFrameCnt_p     <= '0';
        o_decInvalidVrtPktCnt_p       <= '0';
        
        o_decFifoWen_p                <= '0';
        
        o_RTDExSyncStart_p            <= '0';
        o_RTDExSyncStop_p             <= '0';

       if RTDExRxDataValid_s = '1' then
        
          case decoder_fsm_s is

            when DEC_VRL_HDR =>
          
              v20_decWordCnt_s        <= 1;
            
              if v32_RTDExRxData_s = C_VRL_HEADER then
                decoder_fsm_s         <= DEC_VRL_FRM_SIZE;
              else
                -- set VRL sync lost flag and jump to the sync state
                --
                o_decVrlSyncLost_p    <= '1';
                decoder_fsm_s         <= DEC_RETRIEVE_SYNC;
              end if;


            when DEC_VRL_FRM_SIZE =>
          
              v20_decWordCnt_s        <= v20_decWordCnt_s + 1;

              v20_vrlFrameSize_s      <= v32_RTDExRxData_s(19 downto 0);
              
              -- Validate the VRL frame count
              --
              if v32_RTDExRxData_s(31 downto 20) /= v12_vrlFrameCnt_s then
                o_decInvalidVrlFrameCnt_p  <= '1';
              end if;
              
              -- Increase internal valid VRL frame counter
              --              
              v12_vrlFrameCnt_s       <= v32_RTDExRxData_s(31 downto 20) + '1';

              -- Validate the VRL frame size
              -- VRL frame size bigger than RTDEx is not supported for now
              --
              if (v32_RTDExRxData_s(19 downto 0) <= iv20_RxFrameSize_p) then              
                decoder_fsm_s         <= DEC_VRT_HDR;  
              else
                -- Bad frame size
                --
                decoder_fsm_s         <= DEC_VRL_NEXT;
                o_decVrlFrameErr_p    <= '1';
              end if;


            when DEC_VRT_HDR =>
          
              v20_decWordCnt_s        <= v20_decWordCnt_s + 1;
            
              -- VRT header decoding
              --
              v3_vrtPktType_s         <= v32_RTDExRxData_s(30 downto 28);   -- Packet type decoder
              v2_vrtTsf_s             <= v32_RTDExRxData_s(21 downto 20);   -- TSF
              if v32_RTDExRxData_s(19 downto 16) /= v4_vrtPktCnt_s then
                -- Probably some missing packets...
                --
                o_decInvalidVrtPktCnt_p  <= '1';
              end if;
              
              -- Increase internal valid VRT packet count
              --              
              v4_vrtPktCnt_s          <= v32_RTDExRxData_s(19 downto 16) + '1';
              
              if v32_RTDExRxData_s(31 downto 28)/= "1001" then
                -- Only support downlink data packet type for now
                --
                decoder_fsm_s         <= DEC_VRL_NEXT;
                o_decVrtPktErr_p      <= '1';

              elsif v32_RTDExRxData_s(27) /= '0' then
                -- C field must be set to '0', not supported
                --
                decoder_fsm_s         <= DEC_VRL_NEXT;
                o_decVrtPktErr_p      <= '1';
              
              elsif v32_RTDExRxData_s(26) /= '0' then
                -- T field must be set to '0', not supported
                --
                decoder_fsm_s         <= DEC_VRL_NEXT;
                o_decVrtPktErr_p      <= '1';

              --  v32_RTDExRxData_s(25) : Reserved, not checked
              --  v32_RTDExRxData_s(24) : PE, payload field (must be completed)
              --
              elsif v32_RTDExRxData_s(23 downto 22) /= "11" then
                -- Validate TSI field (must be equal 0x3)
                --
                decoder_fsm_s         <= DEC_VRL_NEXT;
                o_decVrtPktErr_p      <= '1';

              elsif v32_RTDExRxData_s(21 downto 20) /= "00" and v32_RTDExRxData_s(21 downto 20) /= "11" then
                -- Validate TSF field (must be 0x0 or qual 0x3)
                --
                decoder_fsm_s         <= DEC_VRL_NEXT;
                o_decVrtPktErr_p      <= '1';

              elsif std_logic_vector(resize(unsigned(v32_RTDExRxData_s(15 downto 0)),iv20_RxFrameSize_p'length)) /= v20_vrlFrameSize_s - 3  then
                -- set VRT packet size error flag and jump to next burst
                --
                decoder_fsm_s         <= DEC_VRL_NEXT;
                o_decVrtPktErr_p      <= '1';

              else
                decoder_fsm_s         <= DEC_VRT_STREAM_ID;
                
              end if;
              

            when DEC_VRT_STREAM_ID =>
          
              v20_decWordCnt_s        <= v20_decWordCnt_s + 1;
            
              -- The streamID verification is performed later
              --
              v32_vrtThisStreamID_s   <= v32_RTDExRxData_s;
              decoder_fsm_s           <= DEC_VRT_TIMESTAMP_1;


            when DEC_VRT_TIMESTAMP_1 =>

              v20_decWordCnt_s        <= v20_decWordCnt_s + 1;

              -- Sample count from the VRT packet
              --
              v32_decSampleCnt_s      <= v32_RTDExRxData_s - 1;

              -- If TSF /= "00", it means the fractional-seconds timestamp is present
              --
              if (v2_vrtTsf_s = "00") then
                decoder_fsm_s         <= DEC_VRT_CONTROL;
              else
                decoder_fsm_s         <= DEC_VRT_TIMESTAMP_2;
              end if;


            when DEC_VRT_TIMESTAMP_2 =>

              v20_decWordCnt_s        <= v20_decWordCnt_s + 1;
              v64_fracsec_timestamp_s(63 downto 32) <= v32_RTDExRxData_s;
              decoder_fsm_s           <= DEC_VRT_TIMESTAMP_3;
 
            
            when DEC_VRT_TIMESTAMP_3 =>

              v20_decWordCnt_s        <= v20_decWordCnt_s + 1;
              v64_fracsec_timestamp_s(31 downto 0) <= v32_RTDExRxData_s;
              decoder_fsm_s           <= DEC_VRT_CONTROL;


            when DEC_VRT_CONTROL =>
            
              v20_decWordCnt_s        <= v20_decWordCnt_s + 1;
              
              -- Change the RTDEx Sync state based on the command
              -- and verify the StreamID
              --
              case v32_RTDExRxData_s(5 downto 4) is

                when "00" =>
                  -- No change in state
                  -- Validate the streamID
                  --
                  if (v32_vrtCurStreamID_s /= v32_vrtThisStreamID_s) then
                    -- Invalid streamID
                    --
                    decoder_fsm_s         <= DEC_VRL_NEXT;
                    o_decVrtPktErr_p      <= '1';
                  else
                    decoder_fsm_s         <= DEC_VRT_RESERVED;
                  end if;
                  
                when "01" =>
                  -- START command
                  -- Verify if the current state is valid
                  --
                  if (iv2_RTDExSyncState_p = RTDEX_SYNC_IDLE) then
                    -- The command is valid
                    --
                    v2_vrtSyncMode_s      <= v32_RTDExRxData_s(3 downto 2);
                    v2_vrtTrigSel_s       <= v32_RTDExRxData_s(1 downto 0);
                    decoder_fsm_s         <= DEC_VRT_RESERVED;
                    v32_vrtCurStreamID_s  <= v32_vrtThisStreamID_s;
                    o_RTDExSyncStart_p    <= '1';
                  else
                    -- cannot start when not in idle state
                    --
                    decoder_fsm_s         <= DEC_VRL_NEXT;
                    o_decInvalidCmd_p     <= '1';
                  end if;

                when "10" =>
                  -- STOP command
                  -- Verify the streamID matches
                  -- Verify if the current state is valid
                  --
                  if (v32_vrtThisStreamID_s /= v32_vrtCurStreamID_s) then
                    -- bad StreamID, don't process this packet
                    --
                    decoder_fsm_s         <= DEC_VRL_NEXT;
                    o_decVrtPktErr_p      <= '1';
                  elsif (iv2_RTDExSyncState_p = RTDEX_SYNC_IDLE) then
                    -- bad command, stoping during idle, do not process the packet
                    --
                    decoder_fsm_s         <= DEC_VRL_NEXT;
                    o_decInvalidCmd_p     <= '1';
                  else
                    -- The command is valid
                    --
                    v2_vrtSyncMode_s      <= v32_RTDExRxData_s(3 downto 2);
                    v2_vrtTrigSel_s       <= v32_RTDExRxData_s(1 downto 0);
                    decoder_fsm_s         <= DEC_VRT_RESERVED;
                    o_RTDExSyncStop_p     <= '1';
                  end if;
                
                when others =>
                  -- bad command
                  --
                  decoder_fsm_s           <= DEC_VRL_NEXT;
                  o_decVrtPktErr_p        <= '1';
              end case;


            when DEC_VRT_RESERVED =>
          
              -- 1 word reserved
              --
              v20_decWordCnt_s            <= v20_decWordCnt_s + 1;
              
              if v20_decWordCnt_s = v20_vrlFrameSize_s - 2 then
                -- No data is contained in the VRT packet
                --
                decoder_fsm_s             <= DEC_VRL_CRC32;
              else
                decoder_fsm_s             <= DEC_PUSH_FIFO;
              end if;


            when DEC_PUSH_FIFO =>
          
              v20_decWordCnt_s            <= v20_decWordCnt_s + 1;
              
              -- Transfert data when data packet
              --
              if (v3_vrtPktType_s = "001")  then
                o_decFifoWen_p            <= '1';
                v32_decSampleCnt_s        <= v32_decSampleCnt_s + '1';
              end if;

              -- Look if end of the VRT packet (1 word left for CRC)
              -- NOTE : add support for trailer if required
              --
              if v20_decWordCnt_s = v20_vrlFrameSize_s - 2 then
                decoder_fsm_s           <= DEC_VRL_CRC32;
              end if;
              
  
            when DEC_VRL_NEXT =>
            
              -- This state allows skipping a VRL Frame / VRT Packet
              --
              
              v20_decWordCnt_s          <= v20_decWordCnt_s + 1;

              if v20_decWordCnt_s = v20_vrlFrameSize_s - 2 then
                -- Next word is the CRC field...
                --
                decoder_fsm_s           <= DEC_VRL_CRC32;
              end if;


            when DEC_VRL_CRC32 =>
          
              v20_decWordCnt_s          <= v20_decWordCnt_s + 1;
         
              if v32_RTDExRxData_s /= v32_vrl_crc32_s then
                o_decVrlFrameErr_p  <= '1';
              end if;
              if (v20_decWordCnt_s = iv20_RxFrameSize_p - 1) then
                decoder_fsm_s           <= DEC_VRL_HDR;
              else 
                -- Must pad to iv20_RxFrameSize_p
                --
                decoder_fsm_s           <= DEC_VRL_PAD_FOR_RTDEX;
              end if;


            when DEC_VRL_PAD_FOR_RTDEX =>
 
              v20_decWordCnt_s          <= v20_decWordCnt_s + 1;

              -- Search for VRL header to try to sync with
              --
              if (v20_decWordCnt_s = iv20_RxFrameSize_p - 1) then
                decoder_fsm_s           <= DEC_VRL_HDR;
              end if;


            when DEC_RETRIEVE_SYNC =>

              -- Search for VRL header to try to sync with
              --
              if v32_RTDExRxData_s = C_VRL_HEADER then
                decoder_fsm_s           <= DEC_VRL_FRM_SIZE;
              end if;


            when others => 
          
              decoder_fsm_s             <= DEC_VRL_HDR;
          
          end case;
        end if;
      end if;
    end if;
  end process;

  -- Combinatorial signal since we can not afford an extra cycle (used for CRC reset)
  --
  newVrlFrame_s <= '1' when (decoder_fsm_s = DEC_VRL_CRC32 and RTDExRxDataValid_s = '1') else '0';


  ----------------------------------------------------------------------------
  -- Output port assignements
  ----------------------------------------------------------------------------

  o_newVrlFrame_p                 <= newVrlFrame_s;
  ov2_vrtSyncMode_p               <= v2_vrtSyncMode_s;
  ov2_vrtTrigSel_p                <= v2_vrtTrigSel_s;

  ov64_decTimeStamp_p             <= v64_fracsec_timestamp_s;

-- DQ_DEBUG: to verify
  ov32_decCurrentStreamID_p       <= v32_vrtThisStreamID_s;

  -- Decoder sample count
  --
  ov32_decSampleCnt_p             <= v32_decSampleCnt_s;


  -- One cycle delay to match the FIFO write enable signal
  --
  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      ov32_decFifoWrData_p         <= v32_RTDExRxData_s;
    end if;
  end process;
  
end Behavioral;