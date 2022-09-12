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
-- File : lyt_encoder.vhd
--------------------------------------------------------------------------------
-- Description : VITA 49 context and data packet encoder
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

entity lyt_encoder is
  port (
    i_SysClk_p                  : in  std_logic;                      -- Perseus system clock: 200 Mhz      
    i_CoreReset_p               : in  std_logic;
    i_CoreEnable_p              : in  std_logic;            
    iv64_ImpreciseFpgaTime_p    : in std_logic_vector(63 downto 0);   -- Free-running timestamp
    iv20_TxFrameSize_p          : in std_logic_vector(19 downto 0);   -- VRL packet size includes VRL header + VRT packet + VRL trailer
    iv20_TxCntxFrameSize_p      : in std_logic_vector(19 downto 0);  -- VRL packet size for context and extended context

    -- Encoder Status
    --
    o_newVrlFrame_p             : out std_logic;                      -- New VRL frame transmitted

    -- System Status
    --
    i_encRTDExOverrun_p         : in std_logic;                       -- Sample Lost Indicator
    i_PllLocked_p               : in std_logic;                       -- PLL Lock Status Indicator
    i_PpsLocked_p               : in std_logic;                       -- PPS Lock Status Indicator
    
    -- Decoder sample count and stream ID
    --
    iv32_SampleCnt_p            : in std_logic_vector(31 downto 0);   -- Sample count
    iv32_curStreamID_p          : in std_logic_vector(31 downto 0);   -- current StreamID
    
    -- Event interface
    --
    i_weEvent_p                 : in std_logic;                       -- Write an event
    i_EventTsm_p                : in std_logic;                       -- Event TSM
    iv32_EventIndicator_p       : in std_logic_vector(31 downto 0);   -- State and Event indicators
    iv32_EventStreamID_p        : in std_logic_vector(31 downto 0);   -- Event StreamID
    iv64_EventTimeStamp_p       : in std_logic_vector(63 downto 0);   -- Event timestamp

    -- Error interface
    --
    i_weErr_p                   : in std_logic;                       -- Write an error
    i_ErrTsm_p                  : in std_logic;                       -- Error TSM
    iv8_errIndicator_p          : in std_logic_vector(7 downto 0);    -- Error indicator
    iv32_ErrStreamID_p          : in std_logic_vector(31 downto 0);   -- Error StreamID
    iv64_ErrTimeStamp_p         : in std_logic_vector(63 downto 0);   -- Error timestamp
    iv64_ErrMissTime_p          : in std_logic_vector(63 downto 0);   -- Time in the miss-timed packet
    
    -- RTDEx Tx interface
    --
    i_RTDExTxReady_p            : in std_logic;                       -- RTDEx transmitter ready
    o_EncoderFifoWeEn_p         : out std_logic;                      -- VITA49 context encoder FIFO write enable
    ov32_EncoderFifoData_p      : out std_logic_vector(31 downto 0);  -- VITA49 context encoder FIFO write data

    -- Uplink FSM interface
    --
    iv2_ULState_p               : in std_logic_vector(1 downto 0);    -- Uplink state
    
    -- Data interface
    --
    i_encFifoEmpty_p            : in std_logic;                       -- Encoder Data Fifo empty
    iv32_encFifoRdData_p        : in std_logic_vector(31 downto 0);   -- Encoder Data Fifo Read Data
    i_encFifoRdValid_p          : in std_logic;                       -- Encoder Data Fifo Read Valid
    o_encFifoRen_p              : out std_logic                       -- Encoder Data Fifo Read Enable
  );

end lyt_encoder;

architecture Behavioral of lyt_encoder is

  -- Constant definitions
  --
  constant VRT_CONTEXT_PKT_SIZE               : integer := 7;
  constant VRT_ECONTEXT_PKT_SIZE              : integer := 13;
  constant VRL_FRAME_SIZE_WITH_CNTX_PKT       : integer := VRT_CONTEXT_PKT_SIZE + 3;
  constant VRL_FRAME_SIZE_WITH_ECNTX_PKT      : integer := VRT_ECONTEXT_PKT_SIZE + 3;

  constant v20_VRL_FRAME_SIZE_WITH_CNTX_PKT_C : std_logic_vector(19 downto 0) := std_logic_vector(to_unsigned(VRL_FRAME_SIZE_WITH_CNTX_PKT, 20));
  constant v20_VRL_FRAME_SIZE_WITH_ECNTX_PKT_C: std_logic_vector(19 downto 0) := std_logic_vector(to_unsigned(VRL_FRAME_SIZE_WITH_ECNTX_PKT, 20));
  
  constant v16_VRT_CONTEXT_PKT_SIZE_C         : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(VRT_CONTEXT_PKT_SIZE,  16));
  constant v16_VRT_ECONTEXT_PKT_SIZE_C        : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(VRT_ECONTEXT_PKT_SIZE, 16));


  component enc_fifo_err
    port (
      clk                 : in std_logic;
      rst                 : in std_logic;
      din                 : in std_logic_vector(168 downto 0);
      wr_en               : in std_logic;
      rd_en               : in std_logic;
      dout                : out std_logic_vector(168 downto 0);
      full                : out std_logic;
      overflow            : out std_logic;
      empty               : out std_logic
    );
  end component;

  component enc_fifo_event
    port (
      clk                 : in std_logic;
      rst                 : in std_logic;
      din                 : in std_logic_vector(128 downto 0);
      wr_en               : in std_logic;
      rd_en               : in std_logic;
      dout                : out std_logic_vector(128 downto 0);
      full                : out std_logic;
      overflow            : out std_logic;
      empty               : out std_logic
    );
  end component;
  
  type encoder_fsm_t is (
    ENC_IDLE,
    ENC_VRL_HDR,
    ENC_VRL_FRM_SIZE,
    ENC_VRT_HDR,
    ENC_VRT_STREAM_ID,
    ENC_VRT_TIMESTAMP_1,
    ENC_VRT_TIMESTAMP_2,
    ENC_VRT_TIMESTAMP_3,
    ENC_CNTX_VRT_CNTX_IND,
    ENC_CNTX_VRT_EVENT_IND,
    ENC_ECNTX_VRT_PL_1,
    ENC_ECNTX_VRT_PL_2,
    ENC_ECNTX_VRT_PL_3,
    ENC_ECNTX_VRT_PL_4,
    ENC_ECNTX_VRT_PL_5,
    ENC_ECNTX_VRT_PL_6,
    ENC_ECNTX_VRT_PL_7,
    ENC_ECNTX_VRT_PL_8,
    ENC_DATA_PKT_PUSH_FIFO,
    ENC_DATA_PKT_PUSH_FIFO_WAIT_VALID,
    ENC_DATA_PKT_PUSH_FIFO_ZEROS_PAD,
    ENC_DATA_PKT_SAMPLE_CNT,
    ENC_DATA_PKT_T,
    ENC_VRL_CRC32,
    ENC_RTDEX_PAD
  );

  signal encoder_fsm_s            : encoder_fsm_t;
  attribute fsm_encoding: string;
  attribute fsm_encoding of encoder_fsm_s: signal is "auto";
  
  type vrt_type is (
    DATA_PKT,
    CONTEXT_PKT,
    EXTENDED_CONTEXT_PKT
  );

  signal vrtPktType_s             : vrt_type;

  -- Encoder Status
  --
  signal newVrlFrame_s            : std_logic;
    
  -- Encoder control signals definition
  --
  signal v16_encWordCnt_s         : std_logic_vector(15 downto 0);
  signal v12_encVrlFrameCnt_s     : std_logic_vector(11 downto 0);
  signal v20_encVrlFrameSize_s    : std_logic_vector(19 downto 0);
  
  signal v32_encFifoDin_s         : std_logic_vector(31 downto 0);
  signal encFifoWen_s             : std_logic;
  
  signal v4_encVrtPktCnt_s        : std_logic_vector(3 downto 0);

  -- Encoder error FIFO
  --
  signal v169_encFifoErrDin_s     : std_logic_vector(168 downto 0);
  signal encFifoErrRdEn_s         : std_logic;
  signal v169_encFifoErrDout_s    : std_logic_vector(168 downto 0);
  signal encFifoErrOverFlow_s     : std_logic;
  signal encFifoErrEmpty_s        : std_logic;

  signal ErrTsm_s                 : std_logic;
  signal v8_errIndicator_s        : std_logic_vector(7 downto 0);
  signal v32_ErrStreamID_s        : std_logic_vector(31 downto 0);
  signal v64_ErrTimeStamp_s       : std_logic_vector(63 downto 0);
  signal v64_ErrMissTime_s        : std_logic_vector(63 downto 0);
  
  -- Encoder event FIFO
  --
  signal v129_encFifoEventDin_s   : std_logic_vector(128 downto 0);
  signal encFifoEventRdEn_s       : std_logic;
  signal v129_encFifoEventDout_s  : std_logic_vector(128 downto 0);
  signal encFifoEventOverFlow_s   : std_logic;
  signal encFifoEventEmpty_s      : std_logic;

  signal EventTsm_s               : std_logic;
  signal v32_EventIndicator_s     : std_logic_vector(31 downto 0);
  signal v32_EventStreamID_s      : std_logic_vector(31 downto 0);
  signal v64_EventTimeStamp_s     : std_logic_vector(63 downto 0);

  -- Wait counter
  --
  signal v2_waitRdValid_s         : std_logic_vector(1 downto 0);

  -- Data interface
  --
  signal encFifoRen_s             : std_logic;

  -- Few signals and types to clarify some parts of the code
  --
  subtype slv is std_logic_vector; -- abbreviation used to convert
  signal v16_TxFrameSize_s        :  unsigned(15 downto 0);

  -- Indicator fields for Uplink data packet Trailer
  --
  signal encRTDExOverrun_s        : std_logic;
  signal encRTDExOverrunRst_s     : std_logic;
  
  signal PllLocked_s              : std_logic;
  signal PllLockedRst_s           : std_logic;
  
  signal PpsLocked_s              : std_logic;
  signal PpsLockedRst_s           : std_logic;

  signal validDAtaInd_s           : std_logic;

  -- Logic to count coninuous number of samples (before overrun appends)
  --
  signal v32_consSampleCnt_s      : std_logic_vector(31 downto 0);
  signal consSampleCntEn_s        : std_logic;
  signal holdConsSampleCnt_s      : std_logic;
  
  -- Keep constraints (timing constraints outside this code on these signals)
  --
  attribute keep                   : boolean;
  attribute keep of v169_encFifoErrDout_s : signal is true;
  attribute keep of v129_encFifoEventDout_s : signal is true;


begin


  ----------------------------------------------------------------------------
  -- Encoder error and event FIFOs
  ----------------------------------------------------------------------------

  v169_encFifoErrDin_s        <= i_ErrTsm_p & 
                                 iv8_errIndicator_p &
                                 iv32_ErrStreamID_p & 
                                 iv64_ErrMissTime_p & 
                                 iv64_ErrTimeStamp_p;
                                 
  v129_encFifoEventDin_s      <= i_EventTsm_p & 
                                 iv32_EventIndicator_p & 
                                 iv32_EventStreamID_p & 
                                 iv64_EventTimeStamp_p;
    
  enc_fifo_err_u : enc_fifo_err
    port map (
      clk                     => i_SysClk_p,
      rst                     => i_CoreReset_p,
      din                     => v169_encFifoErrDin_s,
      wr_en                   => i_weErr_p,
      rd_en                   => encFifoErrRdEn_s,
      dout                    => v169_encFifoErrDout_s,
      full                    => open,
      overflow                => encFifoErrOverFlow_s,
      empty                   => encFifoErrEmpty_s
    );


  enc_fifo_event_u : enc_fifo_event
    port map (
      clk                     => i_SysClk_p,
      rst                     => i_CoreReset_p,
      din                     => v129_encFifoEventDin_s,
      wr_en                   => i_weEvent_p,
      rd_en                   => encFifoEventRdEn_s,
      dout                    => v129_encFifoEventDout_s,
      full                    => open,
      overflow                => encFifoEventOverFlow_s,
      empty                   => encFifoEventEmpty_s
    );

  -- FIFO Dout decoding
  --
  ErrTsm_s                    <= v169_encFifoErrDout_s(168);
  v8_errIndicator_s           <= v169_encFifoErrDout_s(167 downto 160);
  v32_ErrStreamID_s           <= v169_encFifoErrDout_s(159 downto 128);
  v64_ErrMissTime_s           <= v169_encFifoErrDout_s(127 downto 64);
  v64_ErrTimeStamp_s          <= v169_encFifoErrDout_s(63 downto 0);

  EventTsm_s                  <= v129_encFifoEventDout_s(128);
  v32_EventIndicator_s        <= v129_encFifoEventDout_s(127 downto 96);
  v32_EventStreamID_s         <= v129_encFifoEventDout_s(95 downto 64);
  v64_EventTimeStamp_s        <= v129_encFifoEventDout_s(63 downto 0);


  -- Indicator field for Uplink data packet Trailer
  -- Hold the errors until transmitted
  --
  process (i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      -- Set it to one even if a reset occurs.
      --
      if (i_encRTDExOverrun_p = '1') then
        encRTDExOverrun_s     <= '1';
      elsif (encRTDExOverrunRst_s = '1') then
        encRTDExOverrun_s     <= '0';
      end if;
    end if;
  end process;

  process (i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      -- Set it to zero even if a reset occurs.
      --
      if (i_PllLocked_p = '0') then
        PllLocked_s           <= '0';
      elsif (PllLockedRst_s = '1') then
        PllLocked_s           <= '1';
      end if;
    end if;
  end process;

  process (i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      -- Set it to zero even if a reset occurs.
      --
      if (i_PpsLocked_p = '0') then
        PpsLocked_s           <= '0';
      elsif (PpsLockedRst_s = '1') then
        PpsLocked_s           <= '1';
      end if;
    end if;
  end process;

  -- Valid Data Indicator
  -- Set to '0' if one condition is not met.
  --
  validDAtaInd_s              <= '1' when ( PpsLocked_s = '1' and PllLocked_s = '1' and encRTDExOverrun_s = '0') else '0';

  process (i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if (consSampleCntEn_s = '0') then 
        -- Kind of reset
        --
        v32_consSampleCnt_s   <= (others => '0');
        holdConsSampleCnt_s   <= '0';
      else 
        -- Enable to count consecutive samples (before an overrun)
        --
        if (i_encRTDExOverrun_p = '1') then
          -- Stop counting...
          --
          holdConsSampleCnt_s <= '1';
        elsif (holdConsSampleCnt_s = '0' and i_encFifoRdValid_p = '1')  then
          -- Count samples from the FIFO
          --
          v32_consSampleCnt_s <= v32_consSampleCnt_s + '1';
        end if;
      end if;
    end if;
  end process;
  
  
  ----------------------------------------------------------------------------
  -- Process extension context packet encoder_fsm_proc
  -- Synchronous update of next state of encoder_fsm_s
  ----------------------------------------------------------------------------

  context_encoder_fsm_proc : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if i_CoreReset_p = '1' then
        encoder_fsm_s           <= ENC_IDLE;
        v16_encWordCnt_s        <= (others => '0');

        v20_encVrlFrameSize_s   <= (others=>'0');
        
        v12_encVrlFrameCnt_s    <= (others=>'0');
        v4_encVrtPktCnt_s       <= (others=>'0');

        vrtPktType_s            <= CONTEXT_PKT;
        v2_waitRdValid_s        <= (others => '0');

        -- Error and Event FIFOs
        --
        encFifoErrRdEn_s        <= '0';
        encFifoEventRdEn_s      <= '0';
        encRTDExOverrunRst_s    <= '1';
        PllLockedRst_s          <= '1';
        PpsLockedRst_s          <= '1';
        
        -- RTDEx TX Interface
        --
        encFifoWen_s            <= '0';
        v32_encFifoDin_s        <= (others=>'0');

        -- Enable the count of continuous samples (before overrun)        
        --
        consSampleCntEn_s       <= '0';
        
      else
      
        -- default values
        --
        encFifoErrRdEn_s        <= '0';
        encFifoEventRdEn_s      <= '0';
        encRTDExOverrunRst_s    <= '0';
        PllLockedRst_s          <= '0';
        PpsLockedRst_s          <= '0';
        encFifoWen_s            <= '0';
        
        if (i_CoreEnable_p = '1') then

          case encoder_fsm_s is
            
            when ENC_IDLE =>
              
              if (i_RTDExTxReady_p = '1') then

                -- NOTE : Constraints can be added to relax the timing of the outputs of the
                --        event and error FIFOs since their outputs are only accesed 2 cycles 
                --        after the read enable.
                --
                
                consSampleCntEn_s               <= '0';
        
                -- Wait until there is an error, event or data to transmit
                --
                if (encFifoErrEmpty_s = '0') then
                  -- Error to transmit -> transmit extended context packet
                  --
                  vrtPktType_s                  <= EXTENDED_CONTEXT_PKT;
                  v16_encWordCnt_s              <= X"0001";
                  encoder_fsm_s                 <= ENC_VRL_HDR;
                  encFifoErrRdEn_s              <= '1';
                  v20_encVrlFrameSize_s         <= v20_VRL_FRAME_SIZE_WITH_ECNTX_PKT_C;

                  -- Only the 16 LSB are used for the packet size.
                  --
                  v16_TxFrameSize_s           <= unsigned(iv20_TxCntxFrameSize_p(15 downto 0));

                elsif (encFifoEventEmpty_s = '0') then
                  -- Event to transmit -> transmit context packet
                  --
                  vrtPktType_s                  <= CONTEXT_PKT;
                  v16_encWordCnt_s              <= X"0001";
                  encoder_fsm_s                 <= ENC_VRL_HDR;
                  encFifoEventRdEn_s            <= '1';
                  v20_encVrlFrameSize_s         <= v20_VRL_FRAME_SIZE_WITH_CNTX_PKT_C;

                  -- Only the 16 LSB are used for the packet size.
                  --
                  v16_TxFrameSize_s           <= unsigned(iv20_TxCntxFrameSize_p(15 downto 0));
                  
                elsif (i_encFifoEmpty_p = '0') then
                  -- Transmit a data packet
                  --
                  vrtPktType_s                  <= DATA_PKT;
                  v16_encWordCnt_s              <= X"0001";
                  encoder_fsm_s                 <= ENC_VRL_HDR;              
                  v20_encVrlFrameSize_s         <= iv20_TxFrameSize_p;

                  -- Only the 16 LSB are used for the packet size.
                  --
                  v16_TxFrameSize_s           <= unsigned(iv20_TxFrameSize_p(15 downto 0));
                  
                end if;
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_VRL_HDR =>

              if (i_RTDExTxReady_p = '1') then
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s                <= C_VRL_HEADER;
                encoder_fsm_s                   <= ENC_VRL_FRM_SIZE;
              end if; -- i_RTDExTxReady_p = '1'

              
            when ENC_VRL_FRM_SIZE =>

              if (i_RTDExTxReady_p = '1') then
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s(31 downto 20)  <= v12_encVrlFrameCnt_s;
                v32_encFifoDin_s(19 downto 0)   <= v20_encVrlFrameSize_s;
                encoder_fsm_s                   <= ENC_VRT_HDR;
              end if; -- i_RTDExTxReady_p = '1'
              

            when ENC_VRT_HDR =>

              if (i_RTDExTxReady_p = '1') then

                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                
                case vrtPktType_s is
                
                  when CONTEXT_PKT =>

                    -- This is a context packet
                    --
                    v32_encFifoDin_s(31 downto 28)    <= X"4";              -- Packet type
                    v32_encFifoDin_s(27)              <= '0';               -- Indicates class identifier present
                    v32_encFifoDin_s(26)              <= '0';               -- Reserved
                    v32_encFifoDin_s(25)              <= '0';               -- Payload endianness
                    v32_encFifoDin_s(24)              <= EventTsm_s;               -- TSM
                    v32_encFifoDin_s(23 downto 22)    <= "11";              -- TSI 
                    v32_encFifoDin_s(21 downto 20)    <= "11";              -- TSF
                    v32_encFifoDin_s(19 downto 16)    <= v4_encVrtPktCnt_s; -- Packet count
                    v32_encFifoDin_s(15 downto 0)     <= v16_VRT_CONTEXT_PKT_SIZE_C;  -- Packet size
                  
                  when EXTENDED_CONTEXT_PKT =>

                    -- This is an extension context packet
                    --

                    v32_encFifoDin_s(31 downto 28)    <= X"5";              -- Packet type
                    v32_encFifoDin_s(27)              <= '0';               -- Indicates class identifier present
                    v32_encFifoDin_s(26)              <= '0';               -- Reserved
                    v32_encFifoDin_s(25)              <= '0';               -- Payload endianness
                    v32_encFifoDin_s(24)              <= ErrTsm_s;               -- TSM
                    v32_encFifoDin_s(23 downto 22)    <= "11";              -- TSI 
                    v32_encFifoDin_s(21 downto 20)    <= "11";              -- TSF
                    v32_encFifoDin_s(19 downto 16)    <= v4_encVrtPktCnt_s; -- Packet count
                    v32_encFifoDin_s(15 downto 0)     <= v16_VRT_ECONTEXT_PKT_SIZE_C;  -- Packet size

                  when others  =>

                    -- Data packet
                    --
                    v32_encFifoDin_s(31 downto 28)    <= X"1";              -- Packet type
                    v32_encFifoDin_s(27)              <= '0';               -- Indicates class identifier present
                    v32_encFifoDin_s(26)              <= '1';               -- Indicates trailer word present
                    v32_encFifoDin_s(25)              <= '0';               -- PReserved
                    v32_encFifoDin_s(24)              <= '0';               -- Payload endianness
                    v32_encFifoDin_s(23 downto 22)    <= "11";              -- TSI 
                    v32_encFifoDin_s(21 downto 20)    <= "11";              -- TSF
                    v32_encFifoDin_s(19 downto 16)    <= v4_encVrtPktCnt_s; -- Packet count
                    v32_encFifoDin_s(15 downto 0)     <= slv(v16_TxFrameSize_s - 3);  -- Packet size
                    
                end case;

                v4_encVrtPktCnt_s               <= v4_encVrtPktCnt_s + '1';
                encoder_fsm_s                   <= ENC_VRT_STREAM_ID;

              end if; -- i_RTDExTxReady_p = '1'


            when ENC_VRT_STREAM_ID =>

              if (i_RTDExTxReady_p = '1') then

                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';

                case vrtPktType_s is
                
                  when CONTEXT_PKT =>

                    -- This is a context packet
                    --
                    v32_encFifoDin_s            <= v32_EventStreamID_s;
                    
                  when EXTENDED_CONTEXT_PKT =>

                    -- This is an extension context packet, use the error Stream ID
                    --
                    v32_encFifoDin_s            <= v32_ErrStreamID_s;
                  
                  when others =>
                  
                    -- Data packet
                    --
                    v32_encFifoDin_s            <= iv32_curStreamID_p;
                    
                end case;

                encoder_fsm_s                   <= ENC_VRT_TIMESTAMP_1;

              end if; -- i_RTDExTxReady_p = '1'
              
            
            when ENC_VRT_TIMESTAMP_1 =>

              if (i_RTDExTxReady_p = '1') then
              
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                
                v32_encFifoDin_s                <= iv32_SampleCnt_p;

                -- The Overrun (continuity) status only takes into account the samples within the packet.
                -- Missing samples between packets are not seen as discontinuity within the packets/
                --
                consSampleCntEn_s               <= '1';
                
                encoder_fsm_s                   <= ENC_VRT_TIMESTAMP_2;
                
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_VRT_TIMESTAMP_2 =>

              if (i_RTDExTxReady_p = '1') then

                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';

                case vrtPktType_s is
                
                  when CONTEXT_PKT =>

                    -- This is a context packet
                    --
                    v32_encFifoDin_s            <= v64_EventTimeStamp_s(63 downto 32);
                    
                  when EXTENDED_CONTEXT_PKT =>

                    -- This is an extension context packet
                    --
                    v32_encFifoDin_s            <= v64_ErrTimeStamp_s(63 downto 32);
                  
                  when others =>
                  
                    -- Data packet
                    --
                    v32_encFifoDin_s            <= iv64_ImpreciseFpgaTime_p(63 downto 32);

                end case;

                encoder_fsm_s                   <= ENC_VRT_TIMESTAMP_3;
              
              end if; -- i_RTDExTxReady_p = '1'
              

            when ENC_VRT_TIMESTAMP_3 =>
            
              if (i_RTDExTxReady_p = '1') then
              
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';

                case vrtPktType_s is
                
                  when CONTEXT_PKT =>

                    -- This is a context packet
                    --
                    v32_encFifoDin_s            <= v64_EventTimeStamp_s(31 downto 0);
                    encoder_fsm_s               <= ENC_CNTX_VRT_CNTX_IND;
                    
                  when EXTENDED_CONTEXT_PKT =>

                    -- This is an extension context packet
                    --
                    v32_encFifoDin_s            <= v64_ErrTimeStamp_s(31 downto 0);
                    encoder_fsm_s               <= ENC_ECNTX_VRT_PL_1;
                  
                  when others =>
                  
                    -- Data packet
                    --
                    v32_encFifoDin_s            <= iv64_ImpreciseFpgaTime_p(31 downto 0);
                    encoder_fsm_s               <= ENC_DATA_PKT_PUSH_FIFO;
                    
                end case;
              
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_CNTX_VRT_CNTX_IND =>

              if (i_RTDExTxReady_p = '1') then
              
                -- Context packet
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';

                v32_encFifoDin_s(31)            <= '1';               -- I: indicates changes in system. We only transmit context packets when it's the case !!
                v32_encFifoDin_s(30 downto 17)  <= (others => '0');   -- Unused
                v32_encFifoDin_s(16)            <= '1';	              -- E: Indicates state and event indicator
                v32_encFifoDin_s(15 downto 9)   <= (others => '0');   -- Unused
                v32_encFifoDin_s(8)             <= '0';               -- CA: indicates context association lists
                v32_encFifoDin_s(7 downto 0)    <= (others => '0');   -- Unused

                encoder_fsm_s                   <= ENC_CNTX_VRT_EVENT_IND;
                
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_CNTX_VRT_EVENT_IND =>
            
              if (i_RTDExTxReady_p = '1') then
                -- Context packet
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s                <= v32_EventIndicator_s;
                encoder_fsm_s                   <= ENC_VRL_CRC32;
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_ECNTX_VRT_PL_1 =>
            
              if (i_RTDExTxReady_p = '1') then
                -- Extended context packet
                -- Payload word #1, error indicator
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s(31 downto 8)   <= (others => '0');       -- Unused
                v32_encFifoDin_s(7 downto 0)    <= v8_errIndicator_s;
                encoder_fsm_s                   <= ENC_ECNTX_VRT_PL_2;
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_ECNTX_VRT_PL_2 =>
            
              if (i_RTDExTxReady_p = '1') then
                -- Extended context packet
                -- Payload word #2, Reserved
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s                <= (others => '0');       -- Reserved 
                encoder_fsm_s                   <= ENC_ECNTX_VRT_PL_3;
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_ECNTX_VRT_PL_3 =>
              
              if (i_RTDExTxReady_p = '1') then
                -- Extended context packet
                -- Payload word #3, Reserved
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s                <= (others => '0');       -- Reserved
                encoder_fsm_s                   <= ENC_ECNTX_VRT_PL_4;
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_ECNTX_VRT_PL_4 =>
            
              if (i_RTDExTxReady_p = '1') then
                -- Extended context packet
                -- Payload word #4, Reserved
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s                <= (others => '0');       -- Reserved
                encoder_fsm_s                   <= ENC_ECNTX_VRT_PL_5;
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_ECNTX_VRT_PL_5 =>
            
              if (i_RTDExTxReady_p = '1') then
                -- Extended context packet
                -- Payload word #5, Reserved
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s                <= (others => '0');       -- Reserved
                encoder_fsm_s                   <= ENC_ECNTX_VRT_PL_6;
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_ECNTX_VRT_PL_6 =>
            
              if (i_RTDExTxReady_p = '1') then
                -- Extended context packet
                -- Payload word #6, 
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
-- DQ_DEBUG : validate what should be transmitted here
                v32_encFifoDin_s                <= (others => '0');
                encoder_fsm_s                   <= ENC_ECNTX_VRT_PL_7;
              end if; -- i_RTDExTxReady_p = '1'
              

            when ENC_ECNTX_VRT_PL_7 =>
            
              if (i_RTDExTxReady_p = '1') then
                -- Extended context packet
                -- Payload word #7, Miss-timed packet
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s                <= v64_ErrMissTime_s(63 downto 32);
                encoder_fsm_s                   <= ENC_ECNTX_VRT_PL_8;
              end if; -- i_RTDExTxReady_p = '1'
              
              
            when ENC_ECNTX_VRT_PL_8 =>
            
              if (i_RTDExTxReady_p = '1') then
                -- Extended context packet
                -- Payload word #8, Miss-timed packet
                --
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                v32_encFifoDin_s                <= v64_ErrMissTime_s(31 downto 0);
                encoder_fsm_s                   <= ENC_VRL_CRC32;
              end if; -- i_RTDExTxReady_p = '1'
              
    
            when ENC_DATA_PKT_PUSH_FIFO =>

              -- Data packet
              --

              -- Must perform the writes even if i_RTDExTxReady_p = '0'.
              -- This will be ok since i_RTDExTxReady_p is a 'programmable full' signal.
              --
              encFifoWen_s                      <= i_encFifoRdValid_p;
              v32_encFifoDin_s                  <= iv32_encFifoRdData_p;
              v2_waitRdValid_s                  <= (others => '0');

              if (encFifoRen_s = '1') then
                -- Count the read enables. The data will come later...
                --
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
              end if;

              if (v16_encWordCnt_s = slv(v16_TxFrameSize_s - 3) and encFifoRen_s = '1') or
                 (iv2_ULState_p = RTDEX_SYNC_IDLE) then
                -- Enough data for this packet,
                -- or will not receive data anymore, must complete with zeros
                --
                encoder_fsm_s                   <= ENC_DATA_PKT_PUSH_FIFO_WAIT_VALID;
              end if;


            when ENC_DATA_PKT_PUSH_FIFO_WAIT_VALID =>

              -- Data packet, wait for valid read signals...
              --
              encFifoWen_s                      <= i_encFifoRdValid_p;
              
              v32_encFifoDin_s                  <= iv32_encFifoRdData_p;
              
              v2_waitRdValid_s <= v2_waitRdValid_s + '1';

              if (v2_waitRdValid_s = "11") then
                -- The waiting value  must be bigger or equal to all delays between the FIFO read and the valid signals
                -- to insure that all samples associated with a read request have been received.
                --
                
                if (v16_encWordCnt_s = slv(v16_TxFrameSize_s - 2)) then
                  -- Enough data, go to Trailer Fields
                  --
                  encoder_fsm_s                 <= ENC_DATA_PKT_SAMPLE_CNT;
                else
                  -- Complete with zeros
                  --
                  encoder_fsm_s                 <= ENC_DATA_PKT_PUSH_FIFO_ZEROS_PAD;
                end if;
              end if;
                

            when ENC_DATA_PKT_PUSH_FIFO_ZEROS_PAD =>
            
              if (i_RTDExTxReady_p = '1') then
              
                -- Pad data packet with zeros
                --
                encFifoWen_s                    <= '1';
                v32_encFifoDin_s                <= (others => '0');
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';

                if (v16_encWordCnt_s = slv(v16_TxFrameSize_s - 3)) then
                  -- Enough data
                  --
                  encoder_fsm_s                 <= ENC_DATA_PKT_SAMPLE_CNT;
                end if;
                
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_DATA_PKT_SAMPLE_CNT =>
            
              if (i_RTDExTxReady_p = '1') then
              
                encFifoWen_s                    <= '1';
                v32_encFifoDin_s                <= v32_consSampleCnt_s;
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                encoder_fsm_s                   <= ENC_DATA_PKT_T;
                
              end if; -- i_RTDExTxReady_p = '1'


            when ENC_DATA_PKT_T =>
            
              if (i_RTDExTxReady_p = '1') then
              
                -- Trailer Field (Uplink data packet)
                --
                
                -- Indicator Enables
                --
                v32_encFifoDin_s(31)            <= '1';               -- Calibrated Time Indicator(field always present)
                v32_encFifoDin_s(30)            <= '1';               -- Valid Data Indicator (field always present)
                v32_encFifoDin_s(29)            <= '1';               -- Reference Lock Indicator(field always present)
                v32_encFifoDin_s(28 downto 25)  <= (others => '0');   -- Unused
                v32_encFifoDin_s(24)            <= '1';	              -- Sample Lost Indicator (field always present)
                v32_encFifoDin_s(23 downto 21)  <= (others => '0');   -- Unused
                v32_encFifoDin_s(20)            <= '1';               -- Channel State Indicator (field always present)

                -- Indicators
                --
                v32_encFifoDin_s(19)            <= PpsLocked_s;       -- Calibrated Time Indicator
                v32_encFifoDin_s(18)            <= validDAtaInd_s;    -- Valid Data Indicator
                v32_encFifoDin_s(17)            <= PllLocked_s;       -- Reference Lock Indicator
                v32_encFifoDin_s(16 downto 13)  <= (others => '0');   -- Unused
                v32_encFifoDin_s(12)            <= encRTDExOverrun_s;	-- Sample Lost Indicator
                v32_encFifoDin_s(11 downto 10)  <= (others => '0');   -- Unused
                v32_encFifoDin_s(9 downto 8)    <= iv2_ULState_p;     -- Channel State Indicator
                
                -- Associated Context Packet (E flag and count)
                --
                v32_encFifoDin_s(7)             <= encFifoEventEmpty_s;-- Look if an extension context packet must be sent
                v32_encFifoDin_s(6 downto 1)    <= (others => '0');   -- Number of associated context packet. Only one is supported
                v32_encFifoDin_s(7)             <= encFifoEventEmpty_s;
                
                -- Reset the indicators
                --
                encRTDExOverrunRst_s            <= '1';
                PllLockedRst_s                  <= '1';
                PpsLockedRst_s                  <= '1';

                -- Reset the consecutive sample count
                --
                consSampleCntEn_s               <= '0';

                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                encoder_fsm_s                   <= ENC_VRL_CRC32;

              end if; -- i_RTDExTxReady_p = '1'
              
              
            when ENC_VRL_CRC32 =>
            
              if (i_RTDExTxReady_p = '1') then
              
                -- Increase VRL frame count
                --
                v12_encVrlFrameCnt_s            <= v12_encVrlFrameCnt_s + '1';
                v32_encFifoDin_s                <= C_VRL_CRC32;
                encFifoWen_s                    <= '1';
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';
                
                if (v16_encWordCnt_s = slv(v16_TxFrameSize_s)) then 
                  -- No need to pad to RTDEx frame size
                  --
                  encoder_fsm_s                   <= ENC_IDLE;
                else
                  encoder_fsm_s                   <= ENC_RTDEX_PAD;
                end if;
                
              end if; -- i_RTDExTxReady_p = '1'
              

            when ENC_RTDEX_PAD =>

              if (i_RTDExTxReady_p = '1') then
              
                encFifoWen_s                    <= '1';
                v32_encFifoDin_s                <= v16_encWordCnt_s & v16_encWordCnt_s;
                v16_encWordCnt_s                <= v16_encWordCnt_s + '1';

                if (v16_encWordCnt_s = slv(v16_TxFrameSize_s)) then 
                  -- No need to pad anymore
                  --
                  encoder_fsm_s                   <= ENC_IDLE;
                end if;
              end if; -- i_RTDExTxReady_p = '1'
              
              
            when others =>

          end case;
        end if;
      end if;
    end if;
  end process;

  -- Combinatorial logic for the encoder data FIFO read enable
  --
  encFifoRen_s            <= '1' when (i_RTDExTxReady_p = '1') and (encoder_fsm_s = ENC_DATA_PKT_PUSH_FIFO) and (i_encFifoEmpty_p = '0') else '0';

  o_encFifoRen_p          <= encFifoRen_s;


  -- Combinatorial signal since we can not afford an extra cycle (used for CRC reset)
  --
  newVrlFrame_s           <= '1' when (encoder_fsm_s = ENC_VRL_CRC32 and i_RTDExTxReady_p = '1') else '0';


  ----------------------------------------------------------------------------
  -- Output port assignements
  ----------------------------------------------------------------------------

  o_newVrlFrame_p         <= newVrlFrame_s;
  
  -- output ports to Tx FIFO interface
  --
  o_EncoderFifoWeEn_p     <= encFifoWen_s;
  ov32_EncoderFifoData_p  <= v32_encFifoDin_s;

end Behavioral;