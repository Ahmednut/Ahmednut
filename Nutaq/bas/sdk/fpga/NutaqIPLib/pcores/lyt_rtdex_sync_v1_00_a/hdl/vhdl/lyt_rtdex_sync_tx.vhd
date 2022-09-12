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
-- File : lyt_rtdex_sync_tx.vhd
--------------------------------------------------------------------------------
-- Description : VITA 49 transport protocol uplink decoder/encoder
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------    

library ieee;
  use ieee.std_logic_1164.all;
	use ieee.std_logic_arith.all;
	use ieee.numeric_std.all; 
	use ieee.std_logic_unsigned.all;    
    
library lyt_rtdex_sync_v1_00_a;
	use lyt_rtdex_sync_v1_00_a.all; 
	use lyt_rtdex_sync_v1_00_a.lyt_rtdex_sync_p.all; 

entity lyt_rtdex_sync_tx is
	generic (
    C_UPLINK_CRC32_CHECK        : boolean := TRUE;
    C_UPLINK_FIFO_DEPTH    	    : integer := 8192;
    C_UPLINK_FIFO_FULL_THRES    : integer := 8192 - 16;
    C_UPLINK_FIFO_EMPTY_THRES	  : integer := 4
	);
	port (
    i_RTDExSyncCoreClk_p        : in std_logic;     -- 
    i_userClk_p                 : in std_logic;     -- user clock
    i_CoreReset_p               : in std_logic;
    i_CoreEnable_p              : in std_logic;
    
    -- External triggers
    -- 
    iv4_ExtTrigger_p            : in std_logic_vector(3 downto 0);
    
    -- FMC Radio status
    --
    i_PllLocked_p               : in std_logic;
  
    -- PPS sync lock status
    --
    i_GpsLocked_p               : in std_logic;      
    
    -- FPGA time stamping
    --
    iv64_FpgaTime_p             : in std_logic_vector(63 downto 0);     -- Free-running timestamp
    
    -- Encoder Regs CTRL & Status via AXI
    --
    ov32_encStatus_p            : out std_logic_vector(31 downto 0);    -- Encoder status
    iv20_RTDExSyncRxFrameSize_p : in std_logic_vector(19 downto 0);
    iv20_RTDExSyncTxFrameSize_p : in std_logic_vector(19 downto 0);
    iv20_RTDExSyncTxCntxFrameSize_p : in std_logic_vector(19 downto 0);
        
    -- RTDEx Rx interface
    --
    i_RTDExRxReady_p            : in std_logic;                         -- RTDEx receiver has valid data to read
    o_RTDExRxReadReq_p          : out std_logic;                        -- RTDEx receiver reads FIFO request
    iv32_RTDExRxData_p          : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
    i_RTDExRxDataValid_p        : in std_logic;                         -- Data at RTDEx receiver data bus is valid
  
    -- RTDEx Tx interface
    --
    i_RTDExTxReady_p            : in std_logic;                         -- RTDEx transmitter ready
    o_RTDExTxWriteReq_p         : out std_logic;                        -- RTDEx transmitter writes FIFO request
    ov32_RTDExTxData_p          : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    
    -- Interface compatible with RTDEx Tx user interface
    --
    o_encReady_p                : out std_logic;                        -- Encoder ready signal
    i_encWriteReq_p             : in std_logic;                         -- Encoder write request
    iv32_encData_p              : in std_logic_vector(31 downto 0)      -- Encoder write data
  );

end lyt_rtdex_sync_tx;

architecture Behavioral of lyt_rtdex_sync_tx is

  component fifo64k_async_w32_r32
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(15 downto 0);
      prog_empty_thresh : in std_logic_vector(15 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      empty : out std_logic;
      prog_empty : out std_logic;
      valid : out std_logic;
      overflow : out std_logic;
      underflow : out std_logic;
      prog_full : out std_logic
    );
  end component;

  component fifo32k_async_w32_r32
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(14 downto 0);
      prog_empty_thresh : in std_logic_vector(14 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      empty : out std_logic;
      prog_empty : out std_logic;
      valid : out std_logic;
      overflow : out std_logic;
      underflow : out std_logic;
      prog_full : out std_logic
    );
  end component;

  component fifo16k_async_w32_r32
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(13 downto 0);
      prog_empty_thresh : in std_logic_vector(13 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      empty : out std_logic;
      prog_empty : out std_logic;
      valid : out std_logic;
      overflow : out std_logic;
      underflow : out std_logic;
      prog_full : out std_logic
    );
  end component;

  component fifo8k_async_w32_r32
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(12 downto 0);
      prog_empty_thresh : in std_logic_vector(12 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      empty : out std_logic;
      prog_empty : out std_logic;
      valid : out std_logic;
      overflow : out std_logic;
      underflow : out std_logic;
      prog_full : out std_logic
    );
  end component;
  
  -- Encoder input FIFO
  --
  signal encReady_s                   : std_logic;
  
  signal encFifoWen_s                 : std_logic;
  signal v32_encFifoWrData_s          : std_logic_vector(31 downto 0);
  signal encFifoRen_s                 : std_logic;
  signal v32_encFifoRdData_s          : std_logic_vector(31 downto 0);
  signal encFifoEmpty_s               : std_logic;

  signal encFifoProgFull_s            : std_logic;
  signal encFifoProgFullCoreClk_s     : std_logic;
  signal encFifoProgEmpty_s           : std_logic;
  signal encFifoRst_s                 : std_logic;
  signal encFifoFull_s                : std_logic;
  signal encFifoOverflow_s            : std_logic;
  signal encFifoUnderflow_s           : std_logic;
  signal encFifoRdValid_s             : std_logic_vector(0 downto 0);

  signal encFifoEnWriting_s           : std_logic;

  signal encFifoRenTmp_s              : std_logic;
  signal encFifoRdValidTmp_s          : std_logic;
  signal v32_encFifoRdDataTmp_s       : std_logic_vector(31 downto 0);
  signal v32_encSampleCount_s         : std_logic_vector(31 downto 0);

  -- Encoder error report interface
  --
  signal encRTDExOverrun_s            : std_logic;
  
  -- Decoder error report interface
  --
  signal decVrlSyncLost_s             : std_logic;
  signal decVrlFrameErr_s             : std_logic;
  signal decVrtPktErr_s               : std_logic;
  signal InvalidTimeStamp_s        : std_logic;
  signal decInvalidCmd_s              : std_logic;
  signal decInvalidVrlFrameCnt_s      : std_logic;
  signal decInvalidVrtPktCnt_s        : std_logic;
  signal RTDExUnderrun_s              : std_logic;
  signal v64_decTimeStamp_s           : std_logic_vector(63 downto 0);
  signal v64_InvalidTimeStampValue_s  : std_logic_vector(63 downto 0);
  signal v64_decErrTimeStamp_s        : std_logic_vector(63 downto 0);
  
  signal v64_ImpreciseFpgaTime_s      : std_logic_vector(63 downto 0);
      
  -- Event generator: Output to context packet encoder
  --
  signal weEvent_s                    : std_logic;
  signal EventTsm_s                   : std_logic;
  signal v32_EventIndicator_s         : std_logic_vector(31 downto 0);
  signal v32_EventStreamID_s          : std_logic_vector(31 downto 0);
  signal v64_EventTimeStamp_s         : std_logic_vector(63 downto 0);

  -- Event generator: Output to econtext packet encoder
  --
  signal weErr_s                      : std_logic;
  signal ErrTsm_s                     : std_logic;
  signal v8_errIndicator_s            : std_logic_vector(7 downto 0);
  signal v32_ErrStreamID_s            : std_logic_vector(31 downto 0);
  signal v64_ErrTimeStamp_s           : std_logic_vector(63 downto 0);
  signal v64_ErrMissTime_s            : std_logic_vector(63 downto 0);

  -- Decoder Status
  --
  signal v32_decCurrentStreamID_s     : std_logic_vector(31 downto 0);
  signal v32_decSampleCnt_s           : std_logic_vector(31 downto 0);
  signal newVrlFrame_s                : std_logic;
  signal v2_vrtSyncMode_s             : std_logic_vector(1 downto 0);
  signal v2_vrtTrigSel_s              : std_logic_vector(1 downto 0);

  -- RTDEx Sync interface
  --
  signal RTDExSyncStart_s                   : std_logic;
  signal RTDExSyncStop_s                    : std_logic;
  signal v2_RTDExSyncState_s                : std_logic_vector(1 downto 0);
  signal v2_RTDExSyncStateEvent_s           : std_logic_vector(1 downto 0);
  signal RTDExSyncChangeOfStateEvent_s      : std_logic;
  signal ChangeOfStateTsmEvent_s            : std_logic;
  signal v64_ChangeOfStateTimeEvent_s       : std_logic_vector(63 downto 0);
  signal FsmReady_s                         : std_logic;
  

begin

  ---------------------------------------------------------------------------
  -- Encoder data input
  ---------------------------------------------------------------------------

  -- Add one cycle delay to help timing.
  --
  process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then
      -- Only allow write request on ready state
      encFifoWen_s          <= i_encWriteReq_p and encReady_s;
      v32_encFifoWrData_s   <= iv32_encData_p;
    end if;
  end process;

  -- Gate the ready signal with the enable writing signal.
  -- (Already in the good clock domain)
  -- The programmable full protection should not be necessary since the FIFO is emptied
  -- when almost full.
  --
  encReady_s <= encFifoEnWriting_s; --  and (not encFifoProgFull_s);    
  o_encReady_p <= encReady_s;
  
  enc_fifo8k: if (C_UPLINK_FIFO_DEPTH = 8192) generate
    encoder_input_fifo: fifo8k_async_w32_r32
      port map(
        rst        	    => encFifoRst_s,
        wr_clk          => i_userClk_p,
        rd_clk          => i_RTDExSyncCoreClk_p,            
        din        	    => v32_encFifoWrData_s,
        wr_en           => encFifoWen_s,
        rd_en           => encFifoRen_s,
        dout            => v32_encFifoRdData_s,    
        prog_full_thresh  => std_logic_vector(to_unsigned(C_UPLINK_FIFO_FULL_THRES, log2_ceil(C_UPLINK_FIFO_DEPTH) - 1)),
        prog_empty_thresh	=> Std_logic_vector(to_unsigned(C_UPLINK_FIFO_EMPTY_THRES, log2_ceil(C_UPLINK_FIFO_DEPTH) - 1)),
        full            => encFifoFull_s,           -- sync to FmcCLK        
        overflow     	  => encFifoOverflow_s,    	  -- sync to FmcCLK
        empty           => encFifoEmpty_s,     	    -- sync to SysCLK
        prog_empty    	=> encFifoProgEmpty_s,      -- sync to SysCLK
        valid           => encFifoRdValid_s(0),     -- sync to SysCLK    
        underflow     	=> encFifoUnderflow_s,      -- sync to SysCLK
        prog_full       => encFifoProgFull_s        -- sync to FmcCLK
      );
  end generate enc_fifo8k;

  enc_fifo16k: if (C_UPLINK_FIFO_DEPTH = 16384) generate
    encoder_input_fifo: fifo16k_async_w32_r32
      port map(
        rst        	    => encFifoRst_s,
        wr_clk          => i_userClk_p,
        rd_clk          => i_RTDExSyncCoreClk_p,            
        din        	    => v32_encFifoWrData_s,
        wr_en           => encFifoWen_s,
        rd_en           => encFifoRen_s,
        dout            => v32_encFifoRdData_s,    
        prog_full_thresh  => std_logic_vector(to_unsigned(C_UPLINK_FIFO_FULL_THRES, log2_ceil(C_UPLINK_FIFO_DEPTH) - 1)),
        prog_empty_thresh	=> Std_logic_vector(to_unsigned(C_UPLINK_FIFO_EMPTY_THRES, log2_ceil(C_UPLINK_FIFO_DEPTH) - 1)),
        full            => encFifoFull_s,           -- sync to FmcCLK        
        overflow     	  => encFifoOverflow_s,    	  -- sync to FmcCLK
        empty           => encFifoEmpty_s,     	    -- sync to SysCLK
        prog_empty    	=> encFifoProgEmpty_s,      -- sync to SysCLK
        valid           => encFifoRdValid_s(0),     -- sync to SysCLK    
        underflow     	=> encFifoUnderflow_s,      -- sync to SysCLK
        prog_full       => encFifoProgFull_s        -- sync to FmcCLK
      );
  end generate enc_fifo16k;
  
  enc_fifo32k: if (C_UPLINK_FIFO_DEPTH = 32768) generate
    encoder_input_fifo: fifo32k_async_w32_r32
      port map(
        rst        	    => encFifoRst_s,
        wr_clk          => i_userClk_p,
        rd_clk          => i_RTDExSyncCoreClk_p,            
        din        	    => v32_encFifoWrData_s,
        wr_en           => encFifoWen_s,
        rd_en           => encFifoRen_s,
        dout            => v32_encFifoRdData_s,    
        prog_full_thresh  => std_logic_vector(to_unsigned(C_UPLINK_FIFO_FULL_THRES, log2_ceil(C_UPLINK_FIFO_DEPTH) - 1)),
        prog_empty_thresh	=> Std_logic_vector(to_unsigned(C_UPLINK_FIFO_EMPTY_THRES, log2_ceil(C_UPLINK_FIFO_DEPTH) - 1)),
        full            => encFifoFull_s,           -- sync to FmcCLK        
        overflow     	  => encFifoOverflow_s,    	  -- sync to FmcCLK
        empty           => encFifoEmpty_s,     	    -- sync to SysCLK
        prog_empty    	=> encFifoProgEmpty_s,      -- sync to SysCLK
        valid           => encFifoRdValid_s(0),     -- sync to SysCLK    
        underflow     	=> encFifoUnderflow_s,      -- sync to SysCLK
        prog_full       => encFifoProgFull_s        -- sync to FmcCLK
      );
  end generate enc_fifo32k;
  
  enc_fifo64k: if (C_UPLINK_FIFO_DEPTH = 65536) generate
    encoder_input_fifo: fifo64k_async_w32_r32
      port map(
        rst        	    => encFifoRst_s,
        wr_clk          => i_userClk_p,
        rd_clk          => i_RTDExSyncCoreClk_p,            
        din        	    => v32_encFifoWrData_s,
        wr_en           => encFifoWen_s,
        rd_en           => encFifoRen_s,
        dout            => v32_encFifoRdData_s,    
        prog_full_thresh  => std_logic_vector(to_unsigned(C_UPLINK_FIFO_FULL_THRES, log2_ceil(C_UPLINK_FIFO_DEPTH) - 1)),
        prog_empty_thresh	=> Std_logic_vector(to_unsigned(C_UPLINK_FIFO_EMPTY_THRES, log2_ceil(C_UPLINK_FIFO_DEPTH) - 1)),
        full            => encFifoFull_s,           -- sync to FmcCLK
        overflow     	  => encFifoOverflow_s,    	  -- sync to FmcCLK
        empty           => encFifoEmpty_s,     	    -- sync to SysCLK
        prog_empty    	=> encFifoProgEmpty_s,      -- sync to SysCLK
        valid           => encFifoRdValid_s(0),     -- sync to SysCLK
        underflow     	=> encFifoUnderflow_s,      -- sync to SysCLK
        prog_full       => encFifoProgFull_s        -- sync to FmcCLK
      );
  end generate enc_fifo64k;
  
  -- Resync programmable FIFO full
  --
  process(i_RTDExSyncCoreClk_p)
  begin
    if rising_edge(i_RTDExSyncCoreClk_p) then
      encFifoProgFullCoreClk_s       <= encFifoProgFull_s;
    end if;
  end process;
  
  ---------------------------------------------------------------------------
  -- Give access to the FPGA time in the System clock domain
  -- The output is imprecise since it has a delay and it is updated
  -- only every several clock cycles.
  ---------------------------------------------------------------------------
  bus_syncer_i0: entity lyt_rtdex_sync_v1_00_a.bus_syncer
    generic map(
        C_WIDTH         => 64        
    )
	port map(
		i_InClk_p 	    => i_userClk_p,
		iv_InSig_p 	    => iv64_FpgaTime_p,
		i_OutClk_p 	    => i_RTDExSyncCoreClk_p,
		ov_OutSig_p 	=> v64_ImpreciseFpgaTime_s
	);
    
  ---------------------------------------------------------------------------
  -- Event manager
  ---------------------------------------------------------------------------
  
  event_generator:entity lyt_rtdex_sync_v1_00_a.lyt_event_generator
    port map (
      i_SysClk_p                => i_RTDExSyncCoreClk_p,
      i_CoreReset_p             => i_CoreReset_p,
      i_CoreEnable_p            => i_CoreEnable_p,

      -- System Time
      --    
      iv64_ImpreciseFpgaTime_p  => v64_ImpreciseFpgaTime_s,
      
      -- Status monitoring
      --
      i_PllLocked_p             => i_PllLocked_p,
      i_PpsLocked_p             => i_GpsLocked_p,
      i_RTDExRxFifoUnderrun_p   => '0',               -- Never happens in UPLINK

      -- Decoder current stream ID
      --
      iv32_decCurrentStreamID_p => v32_decCurrentStreamID_s,
      
      -- Decoder error report interface
      --
      i_decVrlSyncLost_p        => decVrlSyncLost_s,
      i_decVrlFrameErr_p        => decVrlFrameErr_s,
      i_decVrtPktErr_p          => decVrtPktErr_s,
      i_decInvalidCmd_p         => decInvalidCmd_s,
      i_decInvalidVrlFrameCnt_p => decInvalidVrlFrameCnt_s,
      i_decInvalidVrtPktCnt_p   => decInvalidVrtPktCnt_s,
      
      iv64_decErrTimeStamp_p         => v64_decErrTimeStamp_s,
      i_decInvalidTimeStamp_p        => InvalidTimeStamp_s,
      iv64_InvalidTimeStampValue_p   => v64_InvalidTimeStampValue_s,

      -- Decoder change of state report interface
      --   
      iv32_DecStateStreamID_p       => v32_decCurrentStreamID_s,
      i_ChangeOfState_p             => RTDExSyncChangeOfStateEvent_s,
      i_ChangeOfStateTsm_p          => ChangeOfStateTsmEvent_s,
      iv2_State_p                   => v2_RTDExSyncStateEvent_s,
      iv64_ChangeOfStateTimeStamp_p => v64_ChangeOfStateTimeEvent_s,
      
      -- Event generator: Output to context packet encoder
      --
      o_weEvent_p               => weEvent_s,
      o_EventTsm_p              => EventTsm_s,
      ov32_EventIndicator_p     => v32_EventIndicator_s,
      ov32_EventStreamID_p      => v32_EventStreamID_s,
      ov64_EventTimeStamp_p     => v64_EventTimeStamp_s,
      
      -- Event generator: Output to econtext packet encoder
      --
      o_weErr_p                 => weErr_s,
      o_ErrTsm_p                => ErrTsm_s,
      ov8_errIndicator_p        => v8_errIndicator_s,
      ov32_ErrStreamID_p        => v32_ErrStreamID_s,
      ov64_ErrTimeStamp_p       => v64_ErrTimeStamp_s,
      ov64_ErrMissTime_p        => v64_ErrMissTime_s
    );


  ---------------------------------------------------------------------------
  -- Encoder
  ---------------------------------------------------------------------------
  
  uplink_encoder: lyt_encoder
    port map (
      i_SysClk_p                => i_RTDExSyncCoreClk_p,
      i_CoreReset_p             => i_CoreReset_p,
      i_CoreEnable_p            => i_CoreEnable_p,
      iv64_ImpreciseFpgaTime_p  => v64_ImpreciseFpgaTime_s,
      iv20_TxFrameSize_p        => iv20_RTDExSyncTxFrameSize_p,
      iv20_TxCntxFrameSize_p    => iv20_RTDExSyncTxCntxFrameSize_p,

      -- Encoder Status
      --
      o_newVrlFrame_p           => newVrlFrame_s,

      -- System Status
      --
      i_encRTDExOverrun_p       => encRTDExOverrun_s,
      i_PllLocked_p             => i_PllLocked_p,
      i_PpsLocked_p             => i_GpsLocked_p,
      
      -- Sample count and decoder stream ID
      --
      iv32_SampleCnt_p          => v32_encSampleCount_s,
      iv32_curStreamID_p        => v32_decCurrentStreamID_s,
    
      -- Event interface
      --
      i_weEvent_p               => weEvent_s,
      i_EventTsm_p              => EventTsm_s,
      iv32_EventIndicator_p     => v32_EventIndicator_s,
      iv32_EventStreamID_p      => v32_EventStreamID_s,
      iv64_EventTimeStamp_p     => v64_EventTimeStamp_s,

      -- Error interface
      --
      i_weErr_p                 => weErr_s,
      i_ErrTsm_p                => ErrTsm_s,
      iv8_errIndicator_p        => v8_errIndicator_s,
      iv32_ErrStreamID_p        => v32_ErrStreamID_s,
      iv64_ErrTimeStamp_p       => v64_ErrTimeStamp_s,
      iv64_ErrMissTime_p        => v64_ErrMissTime_s,
      
      -- RTDEx Tx interface
      --
      i_RTDExTxReady_p          => i_RTDExTxReady_p,
      o_EncoderFifoWeEn_p       => o_RTDExTxWriteReq_p,
      ov32_EncoderFifoData_p    => ov32_RTDExTxData_p,

      -- Uplink FSM interface
      --
      iv2_ULState_p             => v2_RTDExSyncState_s,
    
      -- Data fifo interface
      --
      i_encFifoEmpty_p          => encFifoEmpty_s,
      iv32_encFifoRdData_p      => v32_encFifoRdDataTmp_s,
      i_encFifoRdValid_p        => encFifoRdValidTmp_s,
      o_encFifoRen_p            => encFifoRenTmp_s
    );


  ---------------------------------------------------------------------------
  --   Statistics measurements
  ---------------------------------------------------------------------------
  
--  decoder_stats:entity lyt_rtdex_sync_v1_00_a.lyt_downlink_stat 
--    port map (
--      i_SysClk_p                => i_RTDExSyncCoreClk_p,
--      i_CoreReset_p             => i_CoreReset_p,
--      i_CoreEnable_p            => i_CoreEnable_p,            
--      i_DecoderNewFrame_p       => newVrlFrame_s,
--      i_DecoderError_p          => '0',
--      iv16_DecoderErrCode_p     => v16_DecoderErrCode_s,
--       -- Output interface   
--      ov32_DecoderRcvFrame_p    => v32_rcv_frame_s,
--      ov32_DecoderBadFrame_p    => v32_bad_frame_s,
--      ov32_DecoderDropFrame_p   => v32_drop_frame_s,
--      ov32_DecoderLostFrame_p   => v32_lost_frame_s
--    );


  ---------------------------------------------------------------------------
  --   Decoder
  ---------------------------------------------------------------------------
  
  packet_decoder: lyt_decoder
    port map (
      i_SysClk_p                => i_RTDExSyncCoreClk_p,
      i_CoreReset_p             => i_CoreReset_p,
      i_CoreEnable_p            => i_CoreEnable_p,
      iv20_RxFrameSize_p        => iv20_RTDExSyncRxFrameSize_p,

      -- Decoder Status
      --
      ov32_decCurrentStreamID_p => v32_decCurrentStreamID_s,
      ov32_decSampleCnt_p       => v32_decSampleCnt_s,
      o_newVrlFrame_p           => open,
      ov2_vrtSyncMode_p         => v2_vrtSyncMode_s,
      ov2_vrtTrigSel_p          => v2_vrtTrigSel_s,
      ov64_decTimeStamp_p       => v64_decTimeStamp_s,

      -- RTDEx Sync interface
      --
      o_RTDExSyncStart_p        => RTDExSyncStart_s,
      o_RTDExSyncStop_p         => RTDExSyncStop_s,
      iv2_RTDExSyncState_p      => v2_RTDExSyncState_s,
      
      -- Decoder data interface
      --
      o_decFifoWen_p            => open,      -- No data in uplink
      ov32_decFifoWrData_p      => open,      -- No data in uplink
      i_FsmReady_p              => '1',
      
      -- Decoder error report interface
      --
      o_decVrlSyncLost_p        => decVrlSyncLost_s,
      o_decVrlFrameErr_p        => decVrlFrameErr_s,
      o_decVrtPktErr_p          => decVrtPktErr_s,
      o_decInvalidCmd_p         => decInvalidCmd_s,
      o_decInvalidVrlFrameCnt_p => decInvalidVrlFrameCnt_s,
      o_decInvalidVrtPktCnt_p   => decInvalidVrtPktCnt_s,

      -- RTDEx Rx interface
      --
      i_RTDExRxReady_p          => i_RTDExRxReady_p,
      o_RTDExRxReadReq_p        => o_RTDExRxReadReq_p,
      iv32_RTDExRxData_p        => iv32_RTDExRxData_p,
      i_RTDExRxDataValid_p      => i_RTDExRxDataValid_p
    );
    
    
  ---------------------------------------------------------------------------
  --   RTDEx Uplink Sync logic
  ---------------------------------------------------------------------------

  u_lyt_uplink_fsm: lyt_uplink_fsm
    port map (
      i_SysClk_p                => i_RTDExSyncCoreClk_p,
      i_userClk_p               => i_userClk_p,
      i_CoreReset_p             => i_CoreReset_p,
      i_CoreEnable_p            => i_CoreEnable_p,            

      -- FPGA time stamping
      --
      iv64_FpgaTime_p           => iv64_FpgaTime_p,

      -- Decoder Status
      --
      iv2_vrtSyncMode_p         => v2_vrtSyncMode_s,
      iv2_vrtTrigSel_p          => v2_vrtTrigSel_s,
      iv64_decTimeStamp_p       => v64_decTimeStamp_s,

      -- Encoder data interface
      --
      i_encFifoRen_p            => encFifoRenTmp_s,
      o_encFifoRdValid_p        => encFifoRdValidTmp_s,
      ov32_encFifoRdData_p      => v32_encFifoRdDataTmp_s,
      ov32_encSampleCount_p     => v32_encSampleCount_s,

      -- RTDEx Sync interface
      --    
      i_startSync_p             => RTDExSyncStart_s,
      i_stopSync_p              => RTDExSyncStop_s,

      -- External triggers
      -- 
      iv4_ExtTrigger_p          => iv4_ExtTrigger_p,

      -- Encoder error report interface
      --
      o_InvalidTimeStamp_p          => InvalidTimeStamp_s,
      ov64_InvalidTimeStampValue_p  => v64_InvalidTimeStampValue_s,
      ov64_decErrTimeStamp_p        => v64_decErrTimeStamp_s,
      o_RTDExUnderrun_p             => RTDExUnderrun_s,
      
      -- Current state
      --
      ov2_ULState_p             => v2_RTDExSyncState_s,
      
      -- Change of state report interface
      --
      o_ChangeOfStateEvent_s          => RTDExSyncChangeOfStateEvent_s,
      o_ChangeOfStateTsmEvent_p       => ChangeOfStateTsmEvent_s,
      ov2_StateEvent                  => v2_RTDExSyncStateEvent_s,
      ov64_ChangeOfStateTimeEvent_p   => v64_ChangeOfStateTimeEvent_s,
      
      -- Decoder output FIFO interface
      --
      i_encFifoProgFull_p       => encFifoProgFullCoreClk_s,
      i_encFifoEmpty_p          => encFifoEmpty_s,
      o_encFifoRst_p            => encFifoRst_s,
      o_encFifoEnWriting_p      => encFifoEnWriting_s,
      o_encFifoRdEn_p           => encFifoRen_s,
      i_encFifoRdValid_p        => encFifoRdValid_s(0),
      iv32_encFifoRdData_p      => v32_encFifoRdData_s
    );


  ---------------------------------------------------------------------------
  -- Output ports
  ---------------------------------------------------------------------------

  -- Encoder status accessible via AXI bus
  -- (Reserved)
  --
  ov32_encStatus_p          <= (others => '0');


end Behavioral;
