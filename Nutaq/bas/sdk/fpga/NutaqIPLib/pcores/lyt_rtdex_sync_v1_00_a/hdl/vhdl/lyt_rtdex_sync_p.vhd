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
-- File        : lyt_rtdex_sync_p.vhd
--------------------------------------------------------------------------------
-- Description : Package to RTDExSync encoder / decoder
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;

package lyt_rtdex_sync_p is

  -- VITA49 constant definition
  --
  constant C_VRL_HEADER             : std_logic_vector(31 downto 0) := X"56524C50";
  constant C_VRL_CRC32              : std_logic_vector(31 downto 0) := X"56454E44";

  constant C_UPLINK                 : std_logic                     := '0';
  constant C_DOWNLINK               : std_logic                     := '1';

  -- RTDEx Sync channel states
  --
  constant RTDEX_SYNC_IDLE          : std_logic_vector(1 downto 0)  := "00";
  constant RTDEX_SYNC_START_PENDING : std_logic_vector(1 downto 0)  := "01";
  constant RTDEX_SYNC_RUNNING       : std_logic_vector(1 downto 0)  := "10";
  constant RTDEX_SYNC_STOP_PENDING  : std_logic_vector(1 downto 0)  := "11";

  -- RTDEx SyncMode
  --
  constant RTDEX_SYNCMODE_IMMEDIATE : std_logic_vector(1 downto 0)  := "00";
  constant RTDEX_SYNCMODE_ON_TRIGGER: std_logic_vector(1 downto 0)  := "01";
  constant RTDEX_SYNCMODE_AT_TIME   : std_logic_vector(1 downto 0)  := "10";
  constant RTDEX_SYNCMODE_RESERVED  : std_logic_vector(1 downto 0)  := "11";
  
  -- RTDEx Sync time correction
  --
  constant RTDEX_SYNC_DL_START_DELAY        : integer := 3;
  constant RTDEX_SYNC_DL_STOP_DELAY_IMM     : integer := 2;
  constant RTDEX_SYNC_DL_STOP_DELAY_TRIG    : integer := 3;
  
  constant RTDEX_SYNC_UL_START_DELAY        : integer := 1;
  constant RTDEX_SYNC_UL_STOP_DELAY         : integer := 1;
  
  constant TIME_TRIGGER_DELAY               : integer := 1;

  -- Functions
  --
  function log2_ceil(N: natural) return positive;
  
  -- Component
  --
  component lyt_decoder is
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
  end component;
  
  component lyt_encoder is
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
  end component;
  
  component lyt_downlink_fsm is
  port (
    i_SysClk_p                    : in std_logic;                       -- RTDEXSync system clock: 200 Mhz
    i_userClk_p                   : in std_logic;
    i_CoreReset_p                 : in std_logic;
    i_CoreEnable_p                : in std_logic;

    -- FPGA time stamping
    --
    iv64_FpgaTime_p               : in std_logic_vector(63 downto 0);   -- Free-running timestamp

    -- Decoder Status
    --
    iv32_decSampleCnt_p           : in std_logic_vector(31 downto 0);   -- Sample count
    i_newVrlFrame_p               : in std_logic;                       -- New VRL frame received
    iv2_vrtSyncMode_p             : in std_logic_vector(1 downto 0);    -- Sync mode
    iv2_vrtTrigSel_p              : in std_logic_vector(1 downto 0);    -- Trigger select
    iv64_decTimeStamp_p           : in std_logic_vector(63 downto 0);   -- Decoder timestamp

    -- Decoder data interface
    --
    i_decFifoWen_p                : in std_logic;                       -- Decoder data fifo write enable
    iv32_decFifoWrData_p          : in std_logic_vector(31 downto 0);   -- Decoder data fifo data
    o_FsmReady_p                  : out std_logic;                      -- Stop RTDEx sync module

    -- RTDEx Sync interface
    --
    i_startSync_p                 : in std_logic;                       -- Start RTDEx sync module
    i_stopSync_p                  : in std_logic;                       -- Stop RTDEx sync module

    -- External triggers
    --
    iv4_ExtTrigger_p              : in std_logic_vector(3 downto 0);

    -- Decoder error report interface
    --
    o_InvalidTimeStamp_p          : out std_logic;                      -- Invalid timestamp
    ov64_InvalidTimeStampValue_p  : out std_logic_vector(63 downto 0);  -- Invalid timestamp value
    ov64_decErrTimeStamp_p        : out std_logic_vector(63 downto 0);  -- Decoder requested time when invalid timestamp value
    o_RTDExUnderrun_p             : out std_logic;                      -- RTDEx Underrun

    -- Current state
    --
    ov2_DLState_p                 : out std_logic_vector(1 downto 0);   -- Downlink state

    -- Change of state report interface
    --
    o_ChangeOfStateEvent_s        : out std_logic;                        -- Downlink change of state event
    o_ChangeOfStateTsmEvent_p     : out std_logic;                        -- Downlink change of state event TSM
    ov2_StateEvent                : out std_logic_vector(1 downto 0);     -- Downlink state at change of state event
    ov64_ChangeOfStateTimeEvent_p : out std_logic_vector(63 downto 0);    -- Downlink FPGA time at the change of state

    -- Decoder output FIFO interface
    --
    i_decFifoProgFull_p           : in std_logic;                       -- VITA49 decoder output FIFO programmable full
    i_decFifoEmpty_p              : in std_logic;                       -- VITA49 decoder output FIFO empty
    i_decFifoProgEmpty_p          : in std_logic;                       -- VITA49 decoder output FIFO programmable empty
    o_decFifoRst_p                : out std_logic;                      -- VITA49 decoder output FIFO reset
    o_decFifoEnReading_p          : out std_logic;                      -- VITA49 decoder output FIFO enable reading (user side)
    o_decFifoWeEn_p               : out std_logic;                      -- VITA49 decoder output FIFO write enable
    ov32_decFifoData_p            : out std_logic_vector(31 downto 0)   -- VITA49 decoder output FIFO write data
  );
  end component;
  
  component lyt_uplink_fsm is
  port (
    i_SysClk_p                    : in std_logic;
    i_userClk_p                   : in std_logic;
    i_CoreReset_p                 : in std_logic;
    i_CoreEnable_p                : in std_logic;

    -- FPGA time stamping
    --
    iv64_FpgaTime_p               : in std_logic_vector(63 downto 0);   -- Free-running timestamp

    -- Decoder Status
    --
    iv2_vrtSyncMode_p             : in std_logic_vector(1 downto 0);    -- Sync mode
    iv2_vrtTrigSel_p              : in std_logic_vector(1 downto 0);    -- Trigger select
    iv64_decTimeStamp_p           : in std_logic_vector(63 downto 0);   -- Decoder timestamp

    -- Encoder data interface
    --
    i_encFifoRen_p                : in std_logic;                       -- Encoder data fifo read enable
    o_encFifoRdValid_p            : out std_logic;                      -- Encoder data fifo read enable
    ov32_encFifoRdData_p          : out std_logic_vector(31 downto 0);  -- Encoder data fifo read data
    ov32_encSampleCount_p         : out std_logic_vector(31 downto 0);  -- Encoder data sample count

    -- RTDEx Sync interface
    --
    i_startSync_p                 : in std_logic;                       -- Start RTDEx sync module
    i_stopSync_p                  : in std_logic;                       -- Stop RTDEx sync module

    -- External triggers
    --
    iv4_ExtTrigger_p              : in std_logic_vector(3 downto 0);

    -- Encoder error report interface
    --
    o_InvalidTimeStamp_p          : out std_logic;                      -- Invalid timestamp
    ov64_InvalidTimeStampValue_p  : out std_logic_vector(63 downto 0);  -- Invalid timestamp value
    ov64_decErrTimeStamp_p        : out std_logic_vector(63 downto 0);  -- Decoder requested time when invalid timestamp value
    o_RTDExUnderrun_p             : out std_logic;                      -- RTDEx Underrun

    -- Current state
    --
    ov2_ULState_p                 : out std_logic_vector(1 downto 0);   -- Downlink state

    -- Change of state report interface
    --
    o_ChangeOfStateEvent_s        : out std_logic;                        -- Downlink change of state event
    o_ChangeOfStateTsmEvent_p     : out std_logic;                        -- Downlink change of state event TSM
    ov2_StateEvent                : out std_logic_vector(1 downto 0);     -- Downlink state at change of state event
    ov64_ChangeOfStateTimeEvent_p : out std_logic_vector(63 downto 0);    -- Downlink FPGA time at the change of state

    -- Encoder input FIFO interface
    --
    i_encFifoProgFull_p           : in std_logic;                       -- VITA49 encoder input FIFO programmable full
    i_encFifoEmpty_p              : in std_logic;                       -- VITA49 encoder input FIFO empty
    o_encFifoRst_p                : out std_logic;                      -- VITA49 encoder input FIFO reset
    o_encFifoEnWriting_p          : out std_logic;                      -- VITA49 encoder input FIFO enable writing (user side)
    o_encFifoRdEn_p               : out std_logic;                      -- VITA49 encoder input FIFO read enable
    i_encFifoRdValid_p            : in std_logic;                       -- VITA49 encoder input FIFO read enable
    iv32_encFifoRdData_p          : in std_logic_vector(31 downto 0)    -- VITA49 encoder input FIFO read data
  );
  end component;

end lyt_rtdex_sync_p;


package body lyt_rtdex_sync_p is

  ------------------------------------------------------------------
  -- Functions
  ------------------------------------------------------------------

  -- Find minimum number of bits required to represent N as an unsigned 
  -- binary number.
  -- Simple recursive implementation...
  --
  function log2_ceil(N: natural) return positive is
  begin
    if (N < 2) then
      return 1;
    else
      return 1 + log2_ceil(N/2);
    end if;
  end function log2_ceil;

end lyt_rtdex_sync_p;