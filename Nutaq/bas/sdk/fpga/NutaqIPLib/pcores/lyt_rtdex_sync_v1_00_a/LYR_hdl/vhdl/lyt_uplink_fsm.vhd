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
-- File : lyt_uplink_fsm.vhd
--------------------------------------------------------------------------------
-- Description : VITA 49 uplink FSM
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

entity lyt_uplink_fsm is
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
end lyt_uplink_fsm;

architecture Behavioral of lyt_uplink_fsm is

  -- Uplink FSM related signals
  --

  signal ulFsm_s                  : std_logic_vector(1 downto 0) := "00";
  signal ChangeOfState_s          : std_logic := '0';


  signal stopCondMet_s            : std_logic := '0';
  signal encFifoEmptyForAWhile_s  : std_logic := '0';
  signal v3_encFifoEmptyCnt_s     : std_logic_vector(2 downto 0) := (others => '0');

  -- VRT SYNC Modes and command
  --
  signal v2_vrtCurrentSyncMode_s  : std_logic_vector(1 downto 0) := (others => '0');
  signal v2_vrtStopSyncMode_s     : std_logic_vector(1 downto 0) := (others => '0');
  signal v2_vrtStopTrigSel_s      : std_logic_vector(1 downto 0) := (others => '0');
  signal v2_vrtTrigSel_s          : std_logic_vector(1 downto 0) := (others => '0');
  signal stopSyncPending_s        : std_logic := '0';
  signal stopSyncPendingRst_s     : std_logic := '0';

  -- Trigger logic
  --
  signal trigger_s                : std_logic := '0';
  signal timeTrigger_s            : std_logic := '0';

  signal trigUclkD1_s             : std_logic := '0';
  signal trigUclkD2_s             : std_logic := '0';
  signal trigUclkD3_s             : std_logic := '0';
  signal trigUclkD4_s             : std_logic := '0';
  signal timeTrigUclkD1_s         : std_logic := '0';

  signal trigUclk_s               : std_logic := '0';
  signal trigUclkDelay_s          : std_logic := '0';

  signal trigSclkD1_s             : std_logic := '0';
  signal trigSclkD2_s             : std_logic := '0';

  signal v64_PendingTime_s        : std_logic_vector(63 downto 0) := (others => '0');

  -- Ttrigger FSM related signals
  --
  type trigger_fsm_t is (
    WRITING_DISABLED,
    WRITING_ENABLED
  );

  signal trigFsm_s                : trigger_fsm_t := WRITING_DISABLED;

  -- Decoder output FIFO logic
  --
  signal v32_FifoSampleCnt_s      : std_logic_vector(31 downto 0) := (others => '0');
  signal encFifoEnWriting_s       : std_logic := '0';
  signal encFifoRdEn_s            : std_logic := '0';
  signal encFifoRst_s             : std_logic := '0';

  -- Overrun logic
  --
  signal RTDExOverrun_s           : std_logic := '0';

  -- Write Enable and Write Valid logic
  --
  signal v4_encFifoRdValidMask_s  : std_logic_vector(3 downto 0) := (others => '0');
  signal v2_encFifoRdEnPtr_s      : std_logic_vector(1 downto 0) := (others => '0');
  signal v2_encFifoRdValidPtr_s   : std_logic_vector(1 downto 0) := (others => '0');

  -- User / RTDExSync crossing clock domain signals
  --
  signal coreResetUclk_s          : std_logic := '0';
  signal coreEnableUclk_s         : std_logic := '0';

  signal enWrite_s                : std_logic := '0';
  signal enWriteLatch_s           : std_logic := '0';
  signal enWriteUclk_s            : std_logic := '0';
  signal enWriteUclkD1_s          : std_logic := '0';

  signal disWrite_s               : std_logic := '0';
  signal disWriteLatch_s          : std_logic := '0';
  signal disWriteUclk_s           : std_logic := '0';
  signal disWriteUclkD1_s         : std_logic := '0';

  signal enWriteOnTrig_s          : std_logic := '0';
  signal enWriteOnTrigUclk_s      : std_logic := '0';
  signal enWriteOnTrigUclkD1_s    : std_logic := '0';

  signal disWriteOnTrig_s         : std_logic := '0';
  signal disWriteOnTrigUclk_s     : std_logic := '0';
  signal disWriteOnTrigUclkD1_s   : std_logic := '0';

  signal decFifoEmpty_s           : std_logic := '0';

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

  signal FsmPendingLatch_s                  : std_logic_vector(ulFsm_s'range) := (others => '0');
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
  attribute keep of encFifoRst_s : signal is true;
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
  trigUclk_l : process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then
      trigUclkD1_s          <= trigger_s;
      trigUclkD2_s          <= trigUclkD1_s;
      trigUclkD3_s          <= trigUclkD2_s;
      trigUclkD4_s          <= trigUclkD3_s;
      timeTrigUclkD1_s      <= timeTrigger_s;
    end if;
  end process trigUclk_l;

  -- Trigger Rising edge detection on trigger mode
  -- Time trigger high level at time mode
  --
  trigUclk_s <= '1' when (trigUclkD3_s = '1' and trigUclkD4_s = '0' and v2_vrtCurrentSyncMode_s = RTDEX_SYNCMODE_ON_TRIGGER)
           else '1' when (timeTrigUclkD1_s = '1' and v2_vrtCurrentSyncMode_s = RTDEX_SYNCMODE_AT_TIME)
           else '0';


  ----------------------------------------------------------------------------
  --  Trigger FSM (user clock domain)
  --
  --    The logic related to the trigger must be on the user clock domain
  --    in order to predict the delay between a trigger event (timer or
  --    external trigger) and the FIFO writings. The delay between a start
  --    immediate command and the FIFO writings does not need to be cycle
  --    accurate.
  ----------------------------------------------------------------------------

  -- Reset and enables synchronization
  --

  userClkSync_l : process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then
      coreResetUclk_s               <= i_CoreReset_p;
      coreEnableUclk_s              <= i_CoreEnable_p;
    end if;
  end process userClkSync_l;

  -- Pulses synchronization.
  -- The enRead and disRead signals are pulses.
  --
  Syncer_enWrite: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => enWrite_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => enWriteUclk_s
    );

  Syncer_disWrite: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => disWrite_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => disWriteUclk_s
    );

  Syncer_enWriteOnTrig: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => enWriteOnTrig_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => enWriteOnTrigUclk_s
    );

  Syncer_disWriteOnTrig: entity work.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => disWriteOnTrig_s,
        i_OutClk_p  => i_userClk_p,
        o_OutSig_p  => disWriteOnTrigUclk_s
    );


  trigger_fsm_proc : process(i_userClk_p)
  begin
    if rising_edge(i_userClk_p) then
      if (coreResetUclk_s = '1') then
        -- Reset
        --
        trigFsm_s                   <= WRITING_DISABLED;
        encFifoEnWriting_s          <= '0';
        StartTimeWe_s               <= '0';
        StopTimeWe_s                <= '0';
        trigUclkDelay_s             <= '0';
        enWriteUclkD1_s             <= '0';
        enWriteOnTrigUclkD1_s       <= '0';
        disWriteUclkD1_s            <= '0';
        disWriteOnTrigUclkD1_s      <= '0';

      elsif coreEnableUclk_s = '1' then

        -- default value
        StartTimeWe_s           <= '0';
        StopTimeWe_s            <= '0';

        trigUclkDelay_s         <= trigUclk_s;
        enWriteUclkD1_s         <= enWriteUclk_s;
        enWriteOnTrigUclkD1_s   <= enWriteOnTrigUclk_s;
        disWriteUclkD1_s        <= disWriteUclk_s;
        disWriteOnTrigUclkD1_s  <= disWriteOnTrigUclk_s;

        case trigFsm_s is

          when WRITING_DISABLED =>

            -- For the start with a trigger condition,
            -- we start on the rising edge
            if ((enWriteOnTrigUclkD1_s = '1' and trigUclk_s = '1' and trigUclkDelay_s = '0') or enWriteUclkD1_s = '1') then
                -- Enable the FIFO writings.
                --
                encFifoEnWriting_s    <= '1';
                trigFsm_s             <= WRITING_ENABLED;
                v64_StartTime_s       <= iv64_FpgaTime_p + RTDEX_SYNC_UL_START_DELAY;
                StartTimeWe_s         <= '1';

            -- If the time is past, go to the IDLE state
            elsif (enWriteOnTrigUclkD1_s = '1' and trigUclk_s = '1' and trigUclkDelay_s = '1') then
                v64_StopTime_s                <= iv64_FpgaTime_p + RTDEX_SYNC_UL_STOP_DELAY;
                StopTimeWe_s                  <= '1';

            end if;


          when WRITING_ENABLED =>

            -- For the stop with a trigger condition,
            -- we stop on the high level
            -- If the time is past, we stop now but
            -- an error message will still be generated
            if ((disWriteOnTrigUclkD1_s = '1' and trigUclk_s = '1') or disWriteUclkD1_s = '1') then
              -- Disable the FIFO writings.
              --
              encFifoEnWriting_s    <= '0';
              trigFsm_s             <= WRITING_DISABLED;
              v64_StopTime_s        <= iv64_FpgaTime_p + RTDEX_SYNC_UL_STOP_DELAY;
              StopTimeWe_s          <= '1';

            end if;

           when others =>

            trigFsm_s               <= WRITING_DISABLED;

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
  -- Uplink state machine
  ----------------------------------------------------------------------------

  -- Hold VRT sync modes
  --
  vrtSyncModes_l : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if (i_CoreReset_p = '1') then
        v2_vrtStopSyncMode_s        <= (others => '0');
        v2_vrtStopTrigSel_s         <= (others => '0');
        v64_vrtStopTimeStamp_s      <= (others => '0');

      else

        -- On stop sync event, latch current sync mode,
        -- trigger select and stop time
        if (i_stopSync_p = '1') then
          v2_vrtStopSyncMode_s      <= iv2_vrtSyncMode_p;
          v2_vrtStopTrigSel_s       <= iv2_vrtTrigSel_p;

          -- iv64_decTimeStamp_p is already available when i_stopSync_p = '1'
          v64_vrtStopTimeStamp_s    <= std_logic_vector(
            unsigned(iv64_decTimeStamp_p) -
            (TIME_TRIGGER_DELAY + RTDEX_SYNC_UL_STOP_DELAY));

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


  uplink_fsm_proc : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if i_CoreReset_p = '1' then
        -- Reset
        --
        ChangeOfState_s             <= '0';
        ulFsm_s                     <= RTDEX_SYNC_IDLE;
        encFifoRst_s              <= '1';
        enWrite_s                   <= '0';
        disWrite_s                  <= '1';
        enWriteOnTrig_s             <= '0';
        disWriteOnTrig_s            <= '0';
        stopSyncPendingRst_s        <= '1';
        stopCondMet_s               <= '0';

        v2_vrtTrigSel_s             <= (others => '0');
        v2_vrtCurrentSyncMode_s     <= (others => '0');
        v64_vrtTimeStamp_s          <= (others => '0');
        decStartTimeStampWe_s       <= '0';
        decStopTimeStampWe_s        <= '0';

      elsif i_CoreEnable_p = '1' then

        -- Default values
        --
        ChangeOfState_s             <= '0';
        enWrite_s                   <= '0';
        disWrite_s                  <= '0';
        enWriteOnTrig_s             <= '0';
        disWriteOnTrig_s            <= '0';
        stopSyncPendingRst_s        <= '0';
        decStartTimeStampWe_s       <= '0';
        decStopTimeStampWe_s        <= '0';
        encFifoRst_s              <= '0';

        case ulFsm_s is

          when RTDEX_SYNC_IDLE =>

            v2_vrtCurrentSyncMode_s <= (others => '0');
            disWrite_s              <= '1';

            if (i_startSync_p = '1') then
              -- Start command: jump to pending state to wait on conditions
              --
              disWrite_s            <= '0';
              encFifoRst_s        <= '1';
              v2_vrtTrigSel_s       <= iv2_vrtTrigSel_p;
              ChangeOfState_s       <= '1';
              ulFsm_s               <= RTDEX_SYNC_START_PENDING;

              -- Save the start sync mode to current sync mode
              v2_vrtCurrentSyncMode_s       <= iv2_vrtSyncMode_p;

              -- iv64_decTimeStamp_p is already available when i_startSync_p = '1'
              v64_vrtTimeStamp_s            <= std_logic_vector(
                unsigned(iv64_decTimeStamp_p) -
                (TIME_TRIGGER_DELAY + RTDEX_SYNC_UL_START_DELAY));

              -- If start condition is "at time",
              -- set a bit to '1' to verify if the start time
              -- is valid or not
              --
              if iv2_vrtSyncMode_p = RTDEX_SYNCMODE_AT_TIME then
                decStartTimeStampWe_s           <= '1';
              end if;
            end if;


          when RTDEX_SYNC_START_PENDING =>

            if (v2_vrtCurrentSyncMode_s = RTDEX_SYNCMODE_IMMEDIATE) then
              -- Activate the enable now logic.
              --
              enWrite_s             <= '1';

            elsif (v2_vrtCurrentSyncMode_s /= RTDEX_SYNCMODE_IMMEDIATE) then
              -- Activate the trigger enable logic.
              --
              enWriteOnTrig_s       <= '1';

            end if;

            if (StartTimeWeSclk_s = '1') then
              -- Trigger happens
              --
              ChangeOfState_s           <= '1';
              ulFsm_s                   <= RTDEX_SYNC_RUNNING;
              v2_vrtCurrentSyncMode_s   <= "00";
            end if;

            if (StopTimeWeSclk_s = '1') then
              -- Miss-time packet happened
              --
              ChangeOfState_s           <= '1';
              ulFsm_s                   <= RTDEX_SYNC_IDLE;
              v2_vrtCurrentSyncMode_s   <= "00";
            end if;

          when RTDEX_SYNC_RUNNING =>

            if (stopSyncPending_s = '1') then
              -- Stop command received: jump to pending state to wait on conditions
              -- This is usefull since a change of state event is sent and can
              -- served as acknoledge message.
              --
              stopSyncPendingRst_s  <= '1';

              -- It is ok since the pulse has already been detected in the user clock domain
              --
              ChangeOfState_s       <= '1';
              ulFsm_s               <= RTDEX_SYNC_STOP_PENDING;

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
              --    Disable the writing
              --
              disWrite_s             <= '1';

            elsif (v2_vrtStopSyncMode_s /= RTDEX_SYNCMODE_IMMEDIATE) then
              -- Stop on trig or on time command:
              --    Activate the trigger logic
              --    Wait for the trigger to happen
              --    Disable the writing
              --
              disWriteOnTrig_s       <= '1';
            end if;

            if (StopTimeWeSclk_s = '1') then
              -- The stop condition is met
              -- Empty the FIFO (reset it)
              --
              stopCondMet_s             <= '1';
            end if;

            -- We stop when the stop conditions have been met and the FIFO empty.
            --
            if (stopCondMet_s = '1' and encFifoEmptyForAWhile_s = '1') then
              -- The empty for a while signal insure that no samples are in transition
              -- between the two clock domains (it takes few cycles for the write enable
              -- to affect the empty signal).
              --
              ChangeOfState_s           <= '1';
              ulFsm_s                   <= RTDEX_SYNC_IDLE;
              v2_vrtCurrentSyncMode_s   <= "00";
              stopCondMet_s             <= '0';
            end if;

          when others =>

            ulFsm_s                 <= RTDEX_SYNC_IDLE;

        end case;
      end if;
    end if;
  end process uplink_fsm_proc;

  -- Logic to insure that the FIFO is empty for a while (no samples in transition).
  --
    encFifoEmptyCnt_l : process(i_SysClk_p)
    begin
        if rising_edge(i_SysClk_p) then
            if (i_encFifoEmpty_p = '1' and stopCondMet_s = '1' and i_CoreReset_p = '0') then
                -- Increment counter (with saturation)
                --
                if (v3_encFifoEmptyCnt_s /= "111") then
                    v3_encFifoEmptyCnt_s      <= v3_encFifoEmptyCnt_s + '1';
                end if;

            else
                -- reset counter
                --
                v3_encFifoEmptyCnt_s        <= (others => '0');
            end if;
        end if;
    end process encFifoEmptyCnt_l;

  encFifoEmptyForAWhile_s           <= '1' when (v3_encFifoEmptyCnt_s = "111") else '0';


  ----------------------------------------------------------------------------
  -- Encoder input FIFO
  --
  --   This logic read samples in the encoder FIFO in case it is almost full
  --   to avoid uncounted samples on the user side (overflow).
  --   The packet encoder can also read from this FIFO.
  --
  ----------------------------------------------------------------------------

  -- The FIFO can be read by the packet encoder or because samples must be discarded
  -- since the FIFO is almost full.
  -- A bit mask is used to know which one has read the FIFO and then properly handle the
  -- valid signal.
  --
  -- NOTE_1: The FIFO empty flag verification is performed outside
  --
  encFifoRdEn_s   <= '1' when (i_encFifoRen_p = '1') or (i_encFifoProgFull_p = '1') else '0';
  RTDExOverrun_s  <= '1' when (i_encFifoProgFull_p = '1') and (i_encFifoRen_p = '0') else '0';

  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if (i_CoreReset_p = '1') then
        v4_encFifoRdValidMask_s     <= (others => '0');
        v2_encFifoRdEnPtr_s         <= (others => '0');
      elsif (encFifoRdEn_s = '1') then
        v4_encFifoRdValidMask_s(to_integer(unsigned(v2_encFifoRdEnPtr_s))) <= not RTDExOverrun_s;
        v2_encFifoRdEnPtr_s         <= v2_encFifoRdEnPtr_s + '1';
      end if;
    end if;
  end process;

  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if (i_CoreReset_p = '1') then
        o_encFifoRdValid_p          <= '0';
        v2_encFifoRdValidPtr_s      <= (others => '0');
      elsif (i_encFifoRdValid_p = '1') then
        -- Look if it corresponds to a read performs by the encoder or not
        --
        o_encFifoRdValid_p          <= v4_encFifoRdValidMask_s(to_integer(unsigned(v2_encFifoRdValidPtr_s)));
        v2_encFifoRdValidPtr_s      <= v2_encFifoRdValidPtr_s + '1';
      else
        o_encFifoRdValid_p          <= '0';
      end if;
    end if;
  end process;

  -- One cycle delay on the FIFO data to match de read valid signal delay
  --
  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      ov32_encFifoRdData_p          <= iv32_encFifoRdData_p;
    end if;
  end process;

  -- Sample count
  --
  process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if (i_CoreReset_p = '1') then
        v32_FifoSampleCnt_s         <= (others => '0');
      elsif (ulFsm_s = RTDEX_SYNC_IDLE and i_startSync_p = '1') then
        -- Reset the sample counter every new command
        --
        v32_FifoSampleCnt_s         <= (others => '0');
      elsif (i_encFifoRdValid_p = '1') then
        v32_FifoSampleCnt_s         <= v32_FifoSampleCnt_s + '1';
      end if;
    end if;
  end process;

  ov32_encSampleCount_p             <= v32_FifoSampleCnt_s;


  ----------------------------------------------------------------------------
  -- Change of state event
  ----------------------------------------------------------------------------

  -- Only latch FPGA time in v64_PendingTime_s when in start pending
  -- or stop pending state
  --
  ChangeOfStatePending_s <= ChangeOfState_s
    when (ulFsm_s = RTDEX_SYNC_START_PENDING or ulFsm_s = RTDEX_SYNC_STOP_PENDING)
    else '0';

  -- Change of state clock domain crossing
  -- System clock -> User clock
  --
  Syncer_ChangeOfStatePending: entity work.syncer
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
                if (ulFsm_s = RTDEX_SYNC_START_PENDING or ulFsm_s = RTDEX_SYNC_STOP_PENDING) then
                    ChangeOfStatePendingLatch_s   <= '1';
                    FsmPendingLatch_s             <= ulFsm_s;

                -- When start and stop (idle) states,
                -- the time is already available.
                -- Create the event now
                --
                elsif (ulFsm_s = RTDEX_SYNC_RUNNING) then
                    ov64_ChangeOfStateTimeEvent_p <= v64_StartTime_s;
                    o_ChangeOfStateEvent_s        <= '1';
                    o_ChangeOfStateTsmEvent_p     <= '0'; -- Start events have precise timing
                    ov2_StateEvent                <= ulFsm_s;

                else --(dlFsmStartStopLatch_s = RTDEX_SYNC_IDLE)
                    ov64_ChangeOfStateTimeEvent_p <= v64_StopTime_s;
                    o_ChangeOfStateEvent_s        <= '1';
                    o_ChangeOfStateTsmEvent_p     <= '0'; -- Stop events have precise timing
                    ov2_StateEvent                <= ulFsm_s;

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
                    + (1 + TIME_TRIGGER_DELAY + RTDEX_SYNC_UL_START_DELAY);
                v64_InvalidCommandTimeStampValue_s  <= v64_vrtTimeStamp_s 
                    + (TIME_TRIGGER_DELAY + RTDEX_SYNC_UL_START_DELAY);
            end if;
        end if;

        -- If rising edge on stop at time,
        -- verify if the current time if already higher than desired time
        --
        if decStopTimeStampWeUclk_s = '1' and decStopTimeStampWeUclkD1_s = '0' then
            if iv64_FpgaTime_p >= v64_vrtTimeStamp_s then
                decInvalidTimeStamp_s               <= '1';
                v64_InvalidTimeStampValue_s         <= iv64_FpgaTime_p 
                    + (1 + TIME_TRIGGER_DELAY + RTDEX_SYNC_UL_STOP_DELAY);
                v64_InvalidCommandTimeStampValue_s  <= v64_vrtTimeStamp_s
                    + (TIME_TRIGGER_DELAY + RTDEX_SYNC_UL_STOP_DELAY);  
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
  -- Overrun output
  ----------------------------------------------------------------------------

  o_RTDExUnderrun_p               <= RTDExOverrun_s;


  ----------------------------------------------------------------------------
  -- Output port assignements
  ----------------------------------------------------------------------------

  o_encFifoEnWriting_p              <= encFifoEnWriting_s;
  o_encFifoRdEn_p                   <= encFifoRdEn_s;
  o_encFifoRst_p                    <= encFifoRst_s;

  ov2_ULState_p                     <= ulFsm_s;


end Behavioral;