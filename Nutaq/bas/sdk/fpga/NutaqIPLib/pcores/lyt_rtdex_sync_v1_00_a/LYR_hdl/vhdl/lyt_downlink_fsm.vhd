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
-- File : lyt_downlink_fsm.vhd
--------------------------------------------------------------------------------
-- Description : VITA 49 downlink FSM
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

entity lyt_downlink_fsm is
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
end lyt_downlink_fsm;

architecture Behavioral of lyt_downlink_fsm is

  -- Downlink FSM related signals
  --

  signal dlFsm_s                  : std_logic_vector(1 downto 0) := "00";

  signal ChangeOfState_s        : std_logic := '0';

  -- VRT SYNC Modes and command
  --
  signal v2_vrtCurrentSyncMode_s  : std_logic_vector(1 downto 0) := (others => '0');

  signal v2_vrtStartSyncMode_s    : std_logic_vector(1 downto 0) := (others => '0');
  signal v2_vrtStopSyncMode_s     : std_logic_vector(1 downto 0) := (others => '0');
  signal v2_vrtStopTrigSel_s      : std_logic_vector(1 downto 0) := (others => '0');
  signal v2_vrtTrigSel_s          : std_logic_vector(1 downto 0) := (others => '0');
  signal stopSyncPending_s        : std_logic := '0';
  signal stopSyncPendingRst_s     : std_logic := '0';

  -- Trigger logic
  --
  signal trigger_s                : std_logic := '0';
  signal timeTrigger_s            : std_logic := '0';

  signal trigUClkD1_s             : std_logic := '0';
  signal trigUClkD2_s             : std_logic := '0';
  signal timeTrigUClkD1_s         : std_logic := '0';

  signal trigUClk_s               : std_logic := '0';
  signal trigUClkDelay_s          : std_logic := '0';

  signal trigSClkD1_s             : std_logic := '0';
  signal trigSClkD2_s             : std_logic := '0';

  signal v64_PendingTime_s        : std_logic_vector(63 downto 0) := (others => '0');

  -- Decoder output FIFO logic
  --
  signal v32_FifoSampleCnt_s      : std_logic_vector(31 downto 0) := (others => '0');
  signal decFifoProgEmpty_s       : std_logic := '0';
  signal decFifoEnReading_s       : std_logic := '0';

  -- Underrun logic
  --
  signal RTDExUnderrun_s          : std_logic := '0';

  -- Ttrigger FSM related signals
  --
  type trigger_fsm_t is (
    READING_DISABLED,
    READING_ENABLED
  );

  signal trigFsm_s                : trigger_fsm_t := READING_DISABLED;

  -- User / RTDExSync crossing clock domain signals
  --
  signal coreResetUClk_s          : std_logic := '0';
  signal coreEnableUClk_s         : std_logic := '0';

  signal enRead_s                 : std_logic := '0';
  signal enReadLatch_s            : std_logic := '0';
  signal enReadUclk_s             : std_logic := '0';
  signal enReadUclkD1_s           : std_logic := '0';

  signal disRead_s                : std_logic := '0';
  signal disReadLatch_s           : std_logic := '0';
  signal disReadUclk_s            : std_logic := '0';
  signal disReadUclkD1_s          : std_logic := '0';

  signal enReadOnTrig_s           : std_logic := '0';
  signal enReadOnTrigUClk_s       : std_logic := '0';
  signal enReadOnTrigUclkD1_s     : std_logic := '0';

  signal disReadOnTrig_s          : std_logic := '0';
  signal disReadOnTrigUClk_s      : std_logic := '0';
  signal disReadOnTrigUClkD1_s    : std_logic := '0';

  signal FutureSampleCnt_s        : std_logic := '0';

  signal decFifoRst_s             : std_logic := '1';
  signal decFifoRstD1_s           : std_logic := '1';
  signal decFifoRstD2_s           : std_logic := '1';

  -- Timestamp
  --
  signal v64_vrtTimeStamp_s                 : std_logic_vector(63 downto 0) := (others => '0');
  signal v64_vrtStopTimeStamp_s             : std_logic_vector(63 downto 0) := (others => '0');

  signal v64_StartTime_s                    : std_logic_vector(63 downto 0) := (others => '0');
  signal v64_StopTime_s                     : std_logic_vector(63 downto 0) := (others => '0');
  signal StartTimeWe_s                      : std_logic := '0';
  signal StopTimeWe_s                       : std_logic := '0';
  signal StartTimeWeSclk_s                  : std_logic := '0';
  signal StopTimeWeSclk_s                   : std_logic := '0';

  signal ChangeOfStatePending_s             : std_logic := '0';
  signal ChangeOfStatePendingUclk_s         : std_logic := '0';
  signal ChangeOfStatePendingUclkD1_s       : std_logic := '0';

  signal PendingTimeWe_s                    : std_logic := '0';
  signal PendingTimeWeSclk_s                : std_logic := '0';

  signal FsmPendingLatch_s                  : std_logic_vector(dlFsm_s'range) := (others => '0');
  signal ChangeOfStatePendingLatch_s        : std_logic := '0';

  -- Invalid timestamp error
  --
  signal decStartTimeStampWe_s                  : std_logic := '0';
  signal decStartTimeStampWeUclk_s              : std_logic := '0';
  signal decStartTimeStampWeUclkD1_s            : std_logic := '0';
  signal decStopTimeStampWe_s                   : std_logic := '0';
  signal decStopTimeStampWeUclk_s               : std_logic := '0';
  signal decStopTimeStampWeUclkD1_s             : std_logic := '0';
        
  signal decInvalidTimeStamp_s                  : std_logic := '0';
  signal decInvalidTimeStampSclk_s              : std_logic := '0';
  signal decInvalidTimeStampSclkD1_s            : std_logic := '0';

  signal v64_InvalidTimeStampValue_s            : std_logic_vector(63 downto 0) := (others => '0');
  signal v64_InvalidCommandTimeStampValue_s     : std_logic_vector(63 downto 0) := (others => '0');

  -- Keep constraints (timing constraints outside this code on these signals)
  --
  attribute keep : boolean;
  attribute keep of coreEnableUClk_s : signal is true;
  attribute keep of v64_StartTime_s : signal is true;
  attribute keep of v64_StopTime_s : signal is true;
  attribute keep of v64_PendingTime_s : signal is true;
  attribute keep of v64_vrtTimeStamp_s : signal is true;
  attribute keep of v64_InvalidTimeStampValue_s : signal is true;
  attribute keep of v64_InvalidCommandTimeStampValue_s : signal is true;
  attribute keep of v2_vrtCurrentSyncMode_s : signal is true;
  attribute keep of v2_vrtTrigSel_s : signal is true;

begin

  ----------------------------------------------------------------------------
  -- Trigger logic (user and RTEDEx clock domains)
  ----------------------------------------------------------------------------

  -- Trigger selection (v2_vrtTrigSel_s is stable prior to the trigger)
  --
  trigger_s <= iv4_ExtTrigger_p(to_integer(unsigned(v2_vrtTrigSel_s)));

  -- Time trigger verification (v64_vrtTimeStamp_s is stable prior to the trigger)
  --
  timeTrigger_s <= '1' when (iv64_FpgaTime_p >= v64_vrtTimeStamp_s) else '0';

  -- Trigger latching
  --
  trigUClk_l : process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then
      trigUClkD1_s          <= trigger_s;
      trigUClkD2_s          <= trigUClkD1_s;
      timeTrigUClkD1_s      <= timeTrigger_s;
    end if;
  end process trigUClk_l;

  -- Trigger Rising edge detection on trigger mode
  -- Time trigger high level at time mode
  --
  trigUClk_s <= '1' when (trigUClkD1_s = '1' and trigUClkD2_s = '0' and v2_vrtCurrentSyncMode_s = RTDEX_SYNCMODE_ON_TRIGGER)
           else '1' when (timeTrigUClkD1_s = '1' and v2_vrtCurrentSyncMode_s = RTDEX_SYNCMODE_AT_TIME)
           else '0';


  ----------------------------------------------------------------------------
  --  Trigger FSM (user clock domain)
  --
  --    The logic related to the trigger must be on the user clock domain
  --    in order to predict with precision the delay between a trigger event
  --    (timer or external trigger) and the FIFO readings. The delay between a start
  --    immediate command and the FIFO readings does not need to be cycle
  --    accurate.
  ----------------------------------------------------------------------------

  -- Reset and enables synchronization
  --

  userClkSync_l : process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then
      coreResetUClk_s               <= i_CoreReset_p;
      coreEnableUClk_s              <= i_CoreEnable_p;
    end if;
  end process userClkSync_l;

  -- Pulses synchronization.
  -- The enRead and disRead signals are pulses.
  --
  Syncer_enRead: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => enRead_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => enReadUclk_s
    );

  Syncer_disRead: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => disRead_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => disReadUclk_s
    );

  Syncer_enReadOnTrig: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => enReadOnTrig_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => enReadOnTrigUclk_s
    );

  Syncer_disReadOnTrig: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => disReadOnTrig_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => disReadOnTrigUclk_s
    );


  trigger_fsm_proc : process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then
      if (coreResetUClk_s = '1') then
        -- Reset
        --
        trigFsm_s               <= READING_DISABLED;
        decFifoEnReading_s      <= '0';
        StartTimeWe_s           <= '0';
        StopTimeWe_s            <= '0';
        trigUClkDelay_s         <= '0';
        enReadUclkD1_s          <= '0';
        enReadOnTrigUclkD1_s    <= '0';
        disReadUclkD1_s         <= '0';
        disReadOnTrigUclkD1_s   <= '0';

        decFifoRst_s            <= '1';
        decFifoRstD1_s          <= '1';
        decFifoRstD2_s          <= '1';
        o_decFifoRst_p          <= '1';

      elsif coreEnableUClk_s = '1' then

        -- default value
        StartTimeWe_s           <= '0';
        StopTimeWe_s            <= '0';
        decFifoRst_s            <= '0';

        -- 3 clock delay between decFifoRst_s and o_decFifoRst_p to let time
        -- to get the sample requested (the FIFO has a latency of 2 clock cycles)
        decFifoRstD1_s          <= decFifoRst_s;
        decFifoRstD2_s          <= decFifoRstD1_s;
        o_decFifoRst_p          <= decFifoRstD2_s;
        
        trigUClkDelay_s         <= trigUClk_s;
        enReadUclkD1_s          <= enReadUclk_s;
        enReadOnTrigUclkD1_s    <= enReadOnTrigUclk_s;
        disReadUclkD1_s         <= disReadUclk_s;
        disReadOnTrigUclkD1_s   <= disReadOnTrigUclk_s;

        case trigFsm_s is

          when READING_DISABLED =>

            -- For the start with a trigger condition,
            -- we start on the rising edge
            if ((enReadOnTrigUclkD1_s = '1' and trigUClk_s = '1' and trigUClkDelay_s = '0') or enReadUclkD1_s = '1') then
              -- Enable the FIFO readings.
              --
              decFifoEnReading_s            <= '1';
              trigFsm_s                     <= READING_ENABLED;
              v64_StartTime_s               <= iv64_FpgaTime_p + RTDEX_SYNC_DL_START_DELAY;
              StartTimeWe_s                 <= '1';

            -- If the time is past, go to the IDLE state
            elsif (enReadOnTrigUclkD1_s = '1' and trigUClk_s = '1' and trigUClkDelay_s = '1') then
                v64_StopTime_s                <= iv64_FpgaTime_p + RTDEX_SYNC_DL_STOP_DELAY_TRIG;
                StopTimeWe_s                  <= '1';
                decFifoRst_s                  <= '1';
              
            end if;

          when READING_ENABLED =>

            -- For the stop with a trigger condition,
            -- we stop on the high level
            -- If the time is past, we stop now but
            -- an error message will still be generated
            if ((disReadOnTrigUclkD1_s = '1' and trigUClk_s = '1') or (disReadUclkD1_s = '1' and i_decFifoEmpty_p = '1')) then
              -- Disable the FIFO readings.
              --
              decFifoEnReading_s            <= '0';
              trigFsm_s                     <= READING_DISABLED;
              if disReadUclkD1_s = '1' then
                v64_StopTime_s                <= iv64_FpgaTime_p + RTDEX_SYNC_DL_STOP_DELAY_IMM;
              else -- disReadOnTrigUclk_s = '1'
                v64_StopTime_s                <= iv64_FpgaTime_p + RTDEX_SYNC_DL_STOP_DELAY_TRIG;
              end if;
              StopTimeWe_s                  <= '1';
              decFifoRst_s                  <= '1';
            end if;

           when others =>

            trigFsm_s               <= READING_DISABLED;

        end case;
      end if;
    end if;
  end process trigger_fsm_proc;

  Syncer_StartTimeWe: entity work.syncer
    port map(
        i_InClk_p   => i_userClk_p,
        i_InSig_p   => StartTimeWe_s,
        i_OutClk_p  => i_SysClk_p,
        o_OutSig_p  => StartTimeWeSclk_s
    );

  Syncer_StopTimeWe: entity work.syncer
    port map(
        i_InClk_p   => i_userClk_p,
        i_InSig_p   => StopTimeWe_s,
        i_OutClk_p  => i_SysClk_p,
        o_OutSig_p  => StopTimeWeSclk_s
    );

  ----------------------------------------------------------------------------
  -- Downlink state machine
  ----------------------------------------------------------------------------

  -- Hold VRT sync mode
  --
  vrtSyncModes_l : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if (i_CoreReset_p = '1') then
        v2_vrtStartSyncMode_s       <= (others => '0');
        v2_vrtStopSyncMode_s        <= (others => '0');
        v2_vrtStopTrigSel_s         <= (others => '0');
        v64_vrtStopTimeStamp_s      <= (others => '0');

      else

        -- On start sync event, latch current sync mode
        if (i_startSync_p = '1') then
          v2_vrtStartSyncMode_s     <= iv2_vrtSyncMode_p;
        end if;

        -- On stop sync event, latch current sync mode,
        -- trigger select and stop time
        if (i_stopSync_p = '1') then
          v2_vrtStopSyncMode_s      <= iv2_vrtSyncMode_p;
          v2_vrtStopTrigSel_s       <= iv2_vrtTrigSel_p;

          -- iv64_decTimeStamp_p is already available when i_stopSync_p = '1'
          v64_vrtStopTimeStamp_s    <= std_logic_vector(
            unsigned(iv64_decTimeStamp_p) -
            (TIME_TRIGGER_DELAY + RTDEX_SYNC_DL_STOP_DELAY_TRIG));

        end if;
      end if;
    end if;
  end process vrtSyncModes_l;

  -- Hold stop sync command
  --
  vrtStopSyncComd_l : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if (i_CoreReset_p = '1') then
        stopSyncPending_s           <= '0';
      elsif (stopSyncPendingRst_s = '1') then
        stopSyncPending_s           <= '0';
      elsif (i_stopSync_p = '1') then
        stopSyncPending_s           <= '1';
      end if;
    end if;
  end process vrtStopSyncComd_l;

  
  downlink_fsm_proc : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if i_CoreReset_p = '1' then
        -- Reset
        --
        ChangeOfState_s             <= '0';
        dlFsm_s                     <= RTDEX_SYNC_IDLE;
        enReadOnTrig_s              <= '0';
        enRead_s                    <= '0';
        disReadOnTrig_s             <= '1';
        disRead_s                   <= '1';
        stopSyncPendingRst_s        <= '1';
        
        v2_vrtTrigSel_s             <= (others => '0');
        v2_vrtCurrentSyncMode_s     <= (others => '0');
        v64_vrtTimeStamp_s          <= (others => '0');
        decStartTimeStampWe_s       <= '0';
        decStopTimeStampWe_s        <= '0';

      elsif (i_CoreEnable_p = '1') then

        -- Default values
        --
        ChangeOfState_s             <= '0';
        enRead_s                    <= '0';
        disRead_s                   <= '0';
        enReadOnTrig_s              <= '0';
        disReadOnTrig_s             <= '0';
        stopSyncPendingRst_s        <= '0';
        decStartTimeStampWe_s       <= '0';
        decStopTimeStampWe_s        <= '0';

        case dlFsm_s is

          when RTDEX_SYNC_IDLE =>

            v2_vrtCurrentSyncMode_s <= (others => '0');

            if (i_startSync_p = '1') then
              -- Start command: jump to pending state to wait on conditions.
              -- Stop any pending reads.
              --
              disRead_s                     <= '1';
              v2_vrtTrigSel_s               <= iv2_vrtTrigSel_p;
              ChangeOfState_s               <= '1';
              dlFsm_s                       <= RTDEX_SYNC_START_PENDING;

              -- Save the start sync mode to current sync mode
              v2_vrtCurrentSyncMode_s       <= iv2_vrtSyncMode_p;

              -- iv64_decTimeStamp_p is already available when i_startSync_p = '1'
              v64_vrtTimeStamp_s            <= std_logic_vector(
                unsigned(iv64_decTimeStamp_p) -
                (TIME_TRIGGER_DELAY + RTDEX_SYNC_DL_START_DELAY));

              -- If start condition is "at time",
              -- set a bit to '1' to verify if the start time
              -- is valid or not
              --
              if iv2_vrtSyncMode_p = RTDEX_SYNCMODE_AT_TIME then
                decStartTimeStampWe_s           <= '1';
              end if;
            end if;


          when RTDEX_SYNC_START_PENDING =>

            if (v2_vrtStartSyncMode_s = RTDEX_SYNCMODE_IMMEDIATE and (i_decFifoProgFull_p = '1' or i_newVrlFrame_p = '1')) then
              -- For a start immediate command, the running state starts when the FIFO has been fill (almost full) with samples
              -- or when a complete VRL frame has been received.
              --
              enRead_s              <= '1';

            elsif (v2_vrtStartSyncMode_s /= RTDEX_SYNCMODE_IMMEDIATE) then
              -- Activate the trigger enable logic.
              --
              enReadOnTrig_s        <= '1';

            end if;

            if (StartTimeWeSclk_s = '1') then
              -- Trigger happens
              --
              ChangeOfState_s           <= '1';
              dlFsm_s                   <= RTDEX_SYNC_RUNNING;
              v2_vrtCurrentSyncMode_s   <= "00";
            end if;

            if (StopTimeWeSclk_s = '1') then
              -- Miss-time packet happened
              --
              ChangeOfState_s           <= '1';
              dlFsm_s                   <= RTDEX_SYNC_IDLE;
              v2_vrtCurrentSyncMode_s   <= "00";
            end if;


          when RTDEX_SYNC_RUNNING =>

            if (stopSyncPending_s = '1') then
              -- Stop command received: jump to pending state to wait on conditions
              -- This is useful since a change of state event is sent and can
              -- served as acknowledge message.
              --
              stopSyncPendingRst_s          <= '1';

              -- It is OK since the pulse has already been detected in the user clock domain
              --
              ChangeOfState_s               <= '1';
              dlFsm_s                       <= RTDEX_SYNC_STOP_PENDING;

              -- Save the stop sync mode to current sync mode
              v2_vrtCurrentSyncMode_s       <= v2_vrtStopSyncMode_s;
              v2_vrtTrigSel_s               <= v2_vrtStopTrigSel_s;
              v64_vrtTimeStamp_s            <= v64_vrtStopTimeStamp_s;

              -- If stop condition is "at time",
              -- set a bit to '1' to verify if the StopTime
              -- is valid or not
              --
              if v2_vrtStopSyncMode_s = RTDEX_SYNCMODE_AT_TIME then
                decStopTimeStampWe_s <= '1';
              end if;
            end if;


          when RTDEX_SYNC_STOP_PENDING =>

            if (v2_vrtStopSyncMode_s = RTDEX_SYNCMODE_IMMEDIATE) then
              -- Stop immediate command:
              --    Stop when the FIFO has been completely empty
              --    Disable the reading
              --
              disRead_s             <= '1';

            elsif (v2_vrtStopSyncMode_s /= RTDEX_SYNCMODE_IMMEDIATE) then
              -- Stop on trig or on time command:
              --    Disable the reading
              --    Activate the trigger logic
              --    Wait for the trigger to happen
              --
              disReadOnTrig_s       <= '1';
            end if;

            if (StopTimeWeSclk_s = '1') then
              -- The stop condition is met
              -- Empty the FIFO (reset it)
              --
              ChangeOfState_s   <= '1';
              dlFsm_s             <= RTDEX_SYNC_IDLE;
              v2_vrtCurrentSyncMode_s <= "00";
            end if;

          when others =>

            dlFsm_s                 <= RTDEX_SYNC_IDLE;

        end case;
      end if;
    end if;
  end process downlink_fsm_proc;


  ----------------------------------------------------------------------------
  -- Decoder output FIFO feeder
  --
  --   This logic feeds the decoder FIFO with zeros in case no data are
  --   available from RTDEx.
  --
  ----------------------------------------------------------------------------

  -- Resync programmable FIFO output
  --
  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      decFifoProgEmpty_s         <= i_decFifoProgEmpty_p;
    end if;
  end process;

  -- NOTE : The FIFO full flag verification is performed outside (by the decoder)
  --
  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if (i_CoreReset_p = '1') then
        -- reset
        --
        o_decFifoWeEn_p                 <= '0';
        RTDExUnderrun_s                 <= '0';
        ov32_decFifoData_p              <= (others => '0');
        v32_FifoSampleCnt_s             <= (others => '0');
        FutureSampleCnt_s               <= '0';

      else

        -- default values
        --
        o_decFifoWeEn_p                 <= '0';
        FutureSampleCnt_s               <= '0';

        case dlFsm_s is

          when RTDEX_SYNC_IDLE =>

            v32_FifoSampleCnt_s         <= (others => '0');
            RTDExUnderrun_s             <= '0';


          when RTDEX_SYNC_START_PENDING =>
            -- Prefill the FIFO
            --
            ov32_decFifoData_p          <= iv32_decFifoWrData_p;

            if (i_decFifoWen_p = '1') then
              v32_FifoSampleCnt_s       <= v32_FifoSampleCnt_s + '1';
              o_decFifoWeEn_p           <= '1';
            end if;


          when RTDEX_SYNC_RUNNING =>
            -- During transmission, the decoder output FIFO must never be empty.
            -- If no data are available, zeros must be transmitted instead.
            -- The number of inserted zeros must be counted.
            --
            if(decFifoProgEmpty_s = '1') then
              -- The FIFO is almost empty.
              -- Verify if data are available from the decoder
              -- else transmit zeros.
              --
              if (i_decFifoWen_p = '1') then
                -- Data from decoder
                -- Verify if aligned (write it) or not (discard it)
                --
                if (iv32_decSampleCnt_p = v32_FifoSampleCnt_s) then
                  o_decFifoWeEn_p       <= '1';
                  ov32_decFifoData_p    <= iv32_decFifoWrData_p;
                  v32_FifoSampleCnt_s   <= v32_FifoSampleCnt_s + '1';
                  RTDExUnderrun_s       <= '0';
                else
                  -- Discard data and transmit zeros
                  --
                  o_decFifoWeEn_p       <= '1';
                  v32_FifoSampleCnt_s   <= v32_FifoSampleCnt_s + '1';
                  ov32_decFifoData_p    <= (others => '0');
                  RTDExUnderrun_s       <= '1';
                end if;
              else
                -- No data from decoder, transmit zeros
                --
                o_decFifoWeEn_p         <= '1';
                v32_FifoSampleCnt_s     <= v32_FifoSampleCnt_s + '1';
                ov32_decFifoData_p      <= (others => '0');
                RTDExUnderrun_s         <= '1';
              end if;
            elsif (i_decFifoWen_p = '1') then
              -- Data from decoder
              -- Verify if aligned (write it) or not (discard it)
              --
              if (iv32_decSampleCnt_p = v32_FifoSampleCnt_s) then
                o_decFifoWeEn_p         <= '1';
                ov32_decFifoData_p      <= iv32_decFifoWrData_p;
                v32_FifoSampleCnt_s     <= v32_FifoSampleCnt_s + '1';
                RTDExUnderrun_s         <= '0';
              end if;
            end if;

            -- If the received sample count is higher than the current sample count,
            -- FutureSampleCnt_s flag is set to stop the vita decoder from reading
            -- new samples.
            -- This is done in order to avoid discarding future sample.
            if (signed(iv32_decSampleCnt_p) - signed(v32_FifoSampleCnt_s)) > 0 then
                FutureSampleCnt_s <= '1';
            end if;


          when RTDEX_SYNC_STOP_PENDING =>
            -- During transmission, the decoder output FIFO must never be empty,
            -- except on a stop immediate command.
            -- If no data are available, zeros must be transmitted instead.
            -- The number of inserted zeros must be counted.
            --
            if(decFifoProgEmpty_s = '1' and v2_vrtStopSyncMode_s /= RTDEX_SYNCMODE_IMMEDIATE) then
              -- The FIFO is almost empty.
              -- Verify if data are available from the decoder
              -- else transmit zeros.
              --
              if (i_decFifoWen_p = '1') then
                -- Data from decoder
                -- Verify if aligned (write it) or not (discard it)
                --
                if (iv32_decSampleCnt_p = v32_FifoSampleCnt_s) then
                  o_decFifoWeEn_p       <= '1';
                  ov32_decFifoData_p    <= iv32_decFifoWrData_p;
                  v32_FifoSampleCnt_s   <= v32_FifoSampleCnt_s + '1';
                  RTDExUnderrun_s       <= '0';
                else
                  -- Discard data and transmit zeros
                  --
                  o_decFifoWeEn_p       <= '1';
                  v32_FifoSampleCnt_s   <= v32_FifoSampleCnt_s + '1';
                  ov32_decFifoData_p    <= (others => '0');
                  RTDExUnderrun_s       <= '1';
                end if;
              else
                -- No data from decoder, transmit zeros
                --
                o_decFifoWeEn_p         <= '1';
                v32_FifoSampleCnt_s     <= v32_FifoSampleCnt_s + '1';
                ov32_decFifoData_p      <= (others => '0');
                RTDExUnderrun_s         <= '1';
              end if;
            elsif (i_decFifoWen_p = '1') then
              -- Data from decoder
              -- Verify if aligned (write it) or not (discard it)
              --
              if (iv32_decSampleCnt_p = v32_FifoSampleCnt_s) then
                o_decFifoWeEn_p         <= '1';
                ov32_decFifoData_p      <= iv32_decFifoWrData_p;
                v32_FifoSampleCnt_s     <= v32_FifoSampleCnt_s + '1';
                RTDExUnderrun_s         <= '0';
              end if;
            end if;

            -- If the received sample count is higher than the current sample count,
            -- FutureSampleCnt_s flag is set to stop the vita decoder from reading
            -- new samples.
            -- This is done in order to avoid discarding future sample.
            if (signed(iv32_decSampleCnt_p) - signed(v32_FifoSampleCnt_s)) > 0 then
                FutureSampleCnt_s <= '1';
            end if;


          when others =>
            RTDExUnderrun_s             <= '0';
            v32_FifoSampleCnt_s         <= (others => '0');
            o_decFifoWeEn_p             <= '0';

        end case;

      end if;
    end if;
  end process;


  ----------------------------------------------------------------------------
  -- Change of state event
  ----------------------------------------------------------------------------

  -- Only latch FPGA time in v64_PendingTime_s when in start pending
  -- or stop pending state
  --
  ChangeOfStatePending_s <= ChangeOfState_s
    when (dlFsm_s = RTDEX_SYNC_START_PENDING or dlFsm_s = RTDEX_SYNC_STOP_PENDING)
    else '0';

  -- Change of state clock domain crossing
  -- System clock -> User clock
  --
  Syncer_DLChangeOfStatePending: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => ChangeOfStatePending_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => ChangeOfStatePendingUclk_s
    );

  process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then

        -- Default value
        --
        PendingTimeWe_s <= '0';

        -- Save last state for rising edge detection
        --
        ChangeOfStatePendingUclkD1_s <= ChangeOfStatePendingUclk_s;

        -- Latch time on the rising edge of a state change to a pending state
        --
        if ChangeOfStatePendingUclk_s = '1' and ChangeOfStatePendingUclkD1_s = '0' then
            v64_PendingTime_s   <= iv64_FpgaTime_p;
            PendingTimeWe_s     <= '1';
        end if;
    end if;
  end process;

  -- Change of state to a pending state clock domain crossing
  -- User clock -> System clock
  --
  Syncer_PendingTimeWe: entity work.syncer
    port map(
        i_InClk_p   => i_userClk_p,
        i_InSig_p   => PendingTimeWe_s,
        i_OutClk_p  => i_SysClk_p,
        o_OutSig_p  => PendingTimeWeSclk_s
    );


  -- Process that generate the change of state events
  --
  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then

        if (i_CoreReset_p = '1') then
            -- reset
            --
            ChangeOfStatePendingLatch_s   <= '0';
            o_ChangeOfStateEvent_s        <= '0';

        else
            -- Default value
            --
            o_ChangeOfStateEvent_s    <= '0';

            -- Detect a new states
            --
            if ChangeOfState_s = '1' then

                -- Latch the change to a pending state
                -- to wait for the time to be available before
                -- creating the event
                --
                if (dlFsm_s = RTDEX_SYNC_START_PENDING or dlFsm_s = RTDEX_SYNC_STOP_PENDING) then
                    ChangeOfStatePendingLatch_s   <= '1';
                    FsmPendingLatch_s             <= dlFsm_s;

                -- When start and stop (idle) states,
                -- the time is already available.
                -- Create the event now
                --
                elsif (dlFsm_s = RTDEX_SYNC_RUNNING) then
                    ov64_ChangeOfStateTimeEvent_p <= v64_StartTime_s;
                    o_ChangeOfStateEvent_s        <= '1';
                    o_ChangeOfStateTsmEvent_p     <= '0'; -- Start events have precise timing
                    ov2_StateEvent                <= dlFsm_s;

                else --(dlFsmStartStopLatch_s = RTDEX_SYNC_IDLE)
                    ov64_ChangeOfStateTimeEvent_p <= v64_StopTime_s;
                    o_ChangeOfStateEvent_s        <= '1';
                    o_ChangeOfStateTsmEvent_p     <= '0'; -- Stop events have precise timing
                    ov2_StateEvent                <= dlFsm_s;

                end if;

            end if;

            -- If there is a pending change of state
            -- and the time is available,
            -- create the event
            --
            if ChangeOfStatePendingLatch_s = '1' and PendingTimeWeSclk_s = '1' then
                ov64_ChangeOfStateTimeEvent_p <= v64_PendingTime_s;
                o_ChangeOfStateEvent_s        <= '1';
                o_ChangeOfStateTsmEvent_p     <= '1'; -- Pending events have imprecise timing
                ov2_StateEvent                <= FsmPendingLatch_s;
                ChangeOfStatePendingLatch_s   <= '0';
            end if;

        end if;
    end if;
  end process;

  ----------------------------------------------------------------------------
  -- Verify if the received timestamp from the decoder
  -- is valid or not
  ----------------------------------------------------------------------------

  -- Clock domain crossing of decStartTimeStampWe_s and decStopTimeStampWe_s
  -- System clock -> User clock
  --
  Syncer_decStartTimeStampWe: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => decStartTimeStampWe_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => decStartTimeStampWeUclk_s
    );
  Syncer_decStopTimeStampWe: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => decStopTimeStampWe_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => decStopTimeStampWeUclk_s
    );

  -- Verify received timestamp with the current time
  --
  process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then

        -- Default value
        --
        decInvalidTimeStamp_s <= '0';

        -- Delay register to detect rising edge
        --
        decStartTimeStampWeUclkD1_s <= decStartTimeStampWeUclk_s;
        decStopTimeStampWeUclkD1_s <= decStopTimeStampWeUclk_s;

        -- If rising edge on start at time,
        -- verify if the current time if already higher than desired time
        --
        if decStartTimeStampWeUclk_s = '1' and decStartTimeStampWeUclkD1_s = '0' then
            if iv64_FpgaTime_p >= v64_vrtTimeStamp_s then
                decInvalidTimeStamp_s               <= '1';
                v64_InvalidTimeStampValue_s         <= iv64_FpgaTime_p
                    + (1 + TIME_TRIGGER_DELAY + RTDEX_SYNC_DL_START_DELAY);
                v64_InvalidCommandTimeStampValue_s  <= v64_vrtTimeStamp_s 
                    + (TIME_TRIGGER_DELAY + RTDEX_SYNC_DL_START_DELAY);
            end if;
        end if;

        -- If rising edge on stop at time,
        -- verify if the current time if already higher than desired time
        --
        if decStopTimeStampWeUclk_s = '1' and decStopTimeStampWeUclkD1_s = '0' then
            if iv64_FpgaTime_p >= v64_vrtTimeStamp_s then
                decInvalidTimeStamp_s               <= '1';
                v64_InvalidTimeStampValue_s         <= iv64_FpgaTime_p
                    + (1 + TIME_TRIGGER_DELAY + RTDEX_SYNC_DL_STOP_DELAY_TRIG);
                v64_InvalidCommandTimeStampValue_s  <= v64_vrtTimeStamp_s
                    + (TIME_TRIGGER_DELAY + RTDEX_SYNC_DL_STOP_DELAY_TRIG);                
            end if;
        end if;

    end if;
  end process;

  -- Clock domain crossing of decInvalidTimeStamp_s
  -- User clock -> System clock
  --
  Syncer_decInvalidTimeStamp: entity work.syncer
    port map(
        i_InClk_p   => i_userClk_p,
        i_InSig_p   => decInvalidTimeStamp_s,
        i_OutClk_p  => i_SysClk_p,
        o_OutSig_p  => decInvalidTimeStampSclk_s
    );

  -- Generate Invalid TimeStamp error on rising edge
  --
  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
        decInvalidTimeStampSclkD1_s <= decInvalidTimeStampSclk_s;
    end if;
  end process;

  o_InvalidTimeStamp_p          <= decInvalidTimeStampSclk_s and (not decInvalidTimeStampSclkD1_s);
  ov64_InvalidTimeStampValue_p  <= v64_InvalidTimeStampValue_s;
  ov64_decErrTimeStamp_p        <= v64_InvalidCommandTimeStampValue_s;

  ----------------------------------------------------------------------------
  -- Underrun output
  ----------------------------------------------------------------------------

  o_RTDExUnderrun_p              <= RTDExUnderrun_s;


  ----------------------------------------------------------------------------
  -- Output port assignements
  ----------------------------------------------------------------------------

  o_decFifoEnReading_p              <= decFifoEnReading_s;

  ov2_DLState_p                     <= dlFsm_s;

  o_FsmReady_p                      <= (not i_decFifoProgFull_p) and (not FutureSampleCnt_s);


end Behavioral;