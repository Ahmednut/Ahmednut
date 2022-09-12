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
-- File : lyt_event_generator.vhd
--------------------------------------------------------------------------------
-- Description :
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

entity lyt_event_generator is
  port (
    i_SysClk_p                  : in std_logic;                       -- Perseus system clock: 200 Mhz
    i_CoreReset_p               : in std_logic;
    i_CoreEnable_p              : in std_logic;

    -- System Time
    --
    iv64_ImpreciseFpgaTime_p    : in std_logic_vector(63 downto 0);   -- Free-running timestamp

    -- Status monitoring
    --
    i_PllLocked_p               : in std_logic;                       -- FMC Radio PLL lock status
    i_PpsLocked_p               : in std_logic;                       -- PPS Lock status
    i_RTDExRxFifoUnderrun_p     : in std_logic;                       -- RTDEx RX FIFO underrun

    -- Decoder current stream ID
    --
    iv32_decCurrentStreamID_p       : in std_logic_vector(31 downto 0);   -- Decoder error stream ID

    -- Decoder error report interface
    --
    i_decVrlSyncLost_p          : in std_logic;                       -- Decoder sync lost
    i_decVrlFrameErr_p          : in std_logic;                       -- Decoder frame error
    i_decVrtPktErr_p            : in std_logic;                       -- Decoder packet error
    i_decInvalidCmd_p           : in std_logic;                       -- Decoder invalid command
    i_decInvalidVrlFrameCnt_p   : in std_logic;                       -- Decoder invalid VRL frame count
    i_decInvalidVrtPktCnt_p     : in std_logic;                       -- Decoder invalid VRT packet count

    i_decInvalidTimeStamp_p     : in std_logic;                           -- Decoder invalid timestamp
    iv64_decErrTimeStamp_p          : in std_logic_vector(63 downto 0);   -- Decoder error Timestamp
    iv64_InvalidTimeStampValue_p    : in std_logic_vector(63 downto 0);   -- Time value for the timestamp error

    -- Decoder change of state report interface
    --
    iv32_DecStateStreamID_p      : in std_logic_vector(31 downto 0);   -- Decoder new state associated StreamID
    i_ChangeOfState_p            : in std_logic;                       -- Decoder change of state En
    i_ChangeOfStateTsm_p         : in std_logic;                       -- Decoder change of state TSM
    iv2_State_p                  : in std_logic_vector(1 downto 0);    -- Decoder new state
    iv64_ChangeOfStateTimeStamp_p: in std_logic_vector(63 downto 0);   -- Free-running timestamp

    -- Output to context packet encoder
    --
    o_weEvent_p                 : out std_logic;                      -- Write an event
    o_EventTsm_p                : out std_logic;                      -- Event TSM
    ov32_EventIndicator_p       : out std_logic_vector(31 downto 0);  -- State and event indicators
    ov32_EventStreamID_p        : out std_logic_vector(31 downto 0);  -- Event StreamID
    ov64_EventTimeStamp_p       : out std_logic_vector(63 downto 0);  -- Event timestamp

    -- Output to econtext packet encoder
    --
    o_weErr_p                   : out std_logic;                      -- Write an error
    o_ErrTsm_p                  : out std_logic;                      -- Error TSM
    ov8_errIndicator_p          : out std_logic_vector(7 downto 0);   -- Error indicator
    ov32_ErrStreamID_p          : out std_logic_vector(31 downto 0);  -- Error StreamID
    ov64_ErrTimeStamp_p         : out std_logic_vector(63 downto 0);  -- Error timestamp
    ov64_ErrMissTime_p          : out std_logic_vector(63 downto 0)   -- Time of miss-timed packet
  );

end lyt_event_generator;

architecture Behavioral of lyt_event_generator is

  -- Edge detector with resets
  --
  signal pllLockedD1_s              : std_logic;
  signal pllLockedD2_s              : std_logic;
  signal pllLockedEn_s              : std_logic;
  signal pllLockedStat_s            : std_logic;

  signal ppsLockedD1_s              : std_logic;
  signal ppsLockedD2_s              : std_logic;
  signal ppsLockedEn_s              : std_logic;
  signal ppsLockedStat_s            : std_logic;

  signal RTDExRxFifoUnderrunD1_s    : std_logic;
  signal RTDExRxFifoUnderrunD2_s    : std_logic;
  signal RTDExRxFifoUnderrunEn_s    : std_logic;
  signal RTDExRxFifoUnderrunStat_s  : std_logic;

  -- Output to context packet encoder
  --
  signal weEvent_s                  : std_logic;
  signal v32_eventIndicator_s       : std_logic_vector(31 downto 0);

  -- Output to econtext packet encoder
  --
  signal weErr_s                    : std_logic;
  signal v8_errIndicator_s          : std_logic_vector(7 downto 0);


begin

  ----------------------------------------------------------------------------
  -- Edge detector with resets for PLL and PPS
  -- Registers that hold the event for errors and change of state
  --
  ----------------------------------------------------------------------------

  -- PLL
  --
  pllLockedDelayed_l : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      pllLockedD1_s         <= i_PllLocked_p;
      pllLockedD2_s         <= pllLockedD1_s;
    end if;
  end process pllLockedDelayed_l;

  pllLockedEn_s   <= pllLockedD1_s xor pllLockedD2_s;
  pllLockedStat_s <= pllLockedD1_s;

  -- PPS
  --
  ppsLockedDelayed_l : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      ppsLockedD1_s         <= i_ppsLocked_p;
      ppsLockedD2_s         <= ppsLockedD1_s;
    end if;
  end process ppsLockedDelayed_l;

  ppsLockedEn_s   <= ppsLockedD1_s xor ppsLockedD2_s;
  ppsLockedStat_s <= ppsLockedD1_s;

  -- Underrun
  --
  RTDExRxFifoUnderrun_l : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      RTDExRxFifoUnderrunD1_s <= i_RTDExRxFifoUnderrun_p;
      RTDExRxFifoUnderrunD2_s <= RTDExRxFifoUnderrunD1_s;
    end if;
  end process RTDExRxFifoUnderrun_l;

  RTDExRxFifoUnderrunEn_s   <= RTDExRxFifoUnderrunD1_s xor RTDExRxFifoUnderrunD2_s;
  RTDExRxFifoUnderrunStat_s <= RTDExRxFifoUnderrunD1_s;


  -- Change of state
  --
--  DecState_l : process(i_SysClk_p)
--  begin
--    if rising_edge(i_SysClk_p) then
--      if (DecChangeOfStateRst_s = '1') then
--        DecChangeOfState_s        <= '0';
--        v2_DecState_s             <= (others => '0');
--        v32_DecStateStreamID_s    <= (others => '0');
--        v64_DecChangeFpgaTime_s   <= (others => '0');
--      elsif (i_ChangeOfState_p = '1') then
--        DecChangeOfState_s        <= '1';
--        v2_DecState_s             <= iv2_State_p;
--        v32_DecStateStreamID_s    <= iv32_DecStateStreamID_p;
--        v64_DecChangeFpgaTime_s   <= iv64_ChangeOfStateTimeStamp_p;
--      end if;
--    end if;
--  end process DecState_l;

-- DQ_DEBUG : complete this section..
--
--  DecoderErr_l : process(i_SysClk_p)
--  begin
--    if rising_edge(i_SysClk_p) then
--      if (DecoderErrRst_s = '1') then
--        DecoderErr_s                <= '0';
--        v32_DecoderErrStreamID_s    <= (others => '0');
--        v64_DecoderErrTimeStamp_s   <= (others => '0');
--      elsif (i_ChangeOfState_p = '1') then
--        DecoderErr_s                <= '1';
--        v32_DecoderErrStreamID_s    <= iv32_DecoderErrStreamID_p;
--        v64_DecoderErrTimeStamp_s   <= iv64_DecoderErrTimeStamp_p;
--      end if;
--    end if;
--  end process DecoderErr_l;


  ----------------------------------------------------------------------------
  -- Priority encoder for event generation
  ----------------------------------------------------------------------------

--  event_generator_l : process(i_SysClk_p)
--  begin
--    if rising_edge(i_SysClk_p) then
--      if i_CoreReset_p = '1' then
--        pllLockedEnRst_s        <= '0';
--        ppsLockedEnRst_s        <= '0';
--        DecChangeOfStateRst_s   <= '0';
--        DecoderErrRst_s         <= '0';
--
--      elsif i_CoreEnable_p = '1' then
--
--        -- Default values
--        --
--        pllLockedEnRst_s        <= '0';
--        ppsLockedEnRst_s        <= '0';
--        DecChangeOfStateRst_s   <= '0';
--        DecoderErrRst_s         <= '0';
--
--        if (DecoderErr_s = '1') then
--          -- Priority to errors context packet
--          --
--          DecoderErrRst_s <= '1';
--
--        elsif (ppsLockedEn_s = '1') then
--          -- Genearte PPS event
--          --
--          ppsLockedEnRst_s <= '1';
--
--        elsif (pllLockedEn_s = '1') then
--          -- Genearte PLL event
--          --
--          pllLockedEnRst_s <= '1';
--
--        elsif (DecChangeOfState_s = '1') then
--          -- Generate Decoder change of state event
--          --
--          DecChangeOfStateRst_s <= '1';
--        end if;
--
--      end if;
--
--    end if;
--  end process event_generator_l;


  -- Enables
  --
  v32_eventIndicator_s(31)      <= ppsLockedEn_s;           -- Calibrated Time Indicator
  v32_eventIndicator_s(30)      <= '0';                     -- Valid Data Indicator
  v32_eventIndicator_s(29)      <= pllLockedEn_s;           -- Reference Lock Indicator
  v32_eventIndicator_s(28)      <= '0';                     -- unused
  v32_eventIndicator_s(27)      <= '0';                     -- unused
  v32_eventIndicator_s(26)      <= '0';                     -- unused
  v32_eventIndicator_s(25)      <= '0';                     -- unused
  v32_eventIndicator_s(24)      <= RTDExRxFifoUnderrunEn_s; -- Sample Loss indicator

  -- Unused
  --
  v32_eventIndicator_s(23)      <= '0';                     -- unused
  v32_eventIndicator_s(22)      <= '0';                     -- unused
  v32_eventIndicator_s(21)      <= '0';                     -- unused
  v32_eventIndicator_s(20)      <= '0';                     -- unused

  -- Indicators
  --
  v32_eventIndicator_s(19)      <= ppsLockedStat_s;         -- Calibrated Time Indicator
  v32_eventIndicator_s(18)      <= '0';                     -- unused
  v32_eventIndicator_s(17)      <= pllLockedStat_s;         -- Reference Lock Indicator
  v32_eventIndicator_s(16)      <= '0';                     -- unused
  v32_eventIndicator_s(15)      <= '0';                     -- unused
  v32_eventIndicator_s(14)      <= '0';                     -- unused
  v32_eventIndicator_s(13)      <= '0';                     -- unused
  v32_eventIndicator_s(12)      <= RTDExRxFifoUnderrunStat_s;-- Sample Lost Indicator

  -- Unused
  --
  v32_eventIndicator_s(11)      <= '0';                     -- unused
  v32_eventIndicator_s(10)      <= '0';                     -- unused
  v32_eventIndicator_s(9)       <= '0';                     -- unused
  v32_eventIndicator_s(8)       <= '0';                     -- unused

  -- User-defined Enables
  --
  v32_eventIndicator_s(7)       <= '0';                     -- unused
  v32_eventIndicator_s(6)       <= '0';                     -- unused
  v32_eventIndicator_s(5)       <= '0';                     -- unused
  v32_eventIndicator_s(4)       <= i_ChangeOfState_p;     -- State Indicator

  -- User-defined Indicators
  --
  v32_eventIndicator_s(3)       <= '0';                     -- unused
  v32_eventIndicator_s(2)       <= '0';                     -- unused
  v32_eventIndicator_s(1 downto 0)  <= iv2_State_p;       -- State Indicator

  weEvent_s <= '1' when (v32_eventIndicator_s(31 downto 24) /= X"00" or v32_eventIndicator_s(7 downto 4) /= "0000") else '0';

  eventIndicatorLatch_l : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      o_weEvent_p               <= weEvent_s;
      ov32_EventIndicator_p     <= v32_eventIndicator_s;
      ov32_EventStreamID_p      <= iv32_DecStateStreamID_p;

      if (v32_eventIndicator_s(4) = '1') then
        ov64_EventTimeStamp_p   <= iv64_ChangeOfStateTimeStamp_p;
        o_EventTsm_p            <= i_ChangeOfStateTsm_p;
      else
        ov64_EventTimeStamp_p   <= iv64_ImpreciseFpgaTime_p;
        o_EventTsm_p            <= '1';
      end if;
    end if;
  end process eventIndicatorLatch_l;


  ----------------------------------------------------------------------------
  -- Output to econtext packet encoder
  ----------------------------------------------------------------------------

  v8_errIndicator_s(7)          <= i_decInvalidCmd_p;           -- Decoder invalid command
  v8_errIndicator_s(6)          <= i_decInvalidTimeStamp_p;     -- Decoder invalid timestamp
  v8_errIndicator_s(5)          <= '0';                         -- Reserved
  v8_errIndicator_s(4)          <= '0';                         -- Reserved
  v8_errIndicator_s(3)          <= '0';                         -- Reserved
  v8_errIndicator_s(2)          <= i_decVrtPktErr_p;            -- Decoder packet error
  v8_errIndicator_s(1)          <= i_decVrlFrameErr_p;          -- Decoder frame error
  v8_errIndicator_s(0)          <= i_decVrlSyncLost_p;          -- Decoder sync lost

  weErr_s <= '1' when v8_errIndicator_s /= X"00" else '0';

  errIndicatorLatch_l : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      o_weErr_p                 <= weErr_s;
      ov8_errIndicator_p        <= v8_errIndicator_s;
      ov32_ErrStreamID_p        <= iv32_decCurrentStreamID_p;

      if i_decInvalidTimeStamp_p = '1' then
        ov64_ErrTimeStamp_p       <= iv64_InvalidTimeStampValue_p;
        o_ErrTsm_p                <= '0';
      else
        ov64_ErrTimeStamp_p       <= iv64_ImpreciseFpgaTime_p;
        o_ErrTsm_p                <= '1';
      end if;

      ov64_ErrMissTime_p        <= iv64_decErrTimeStamp_p;
    end if;
  end process errIndicatorLatch_l;


end Behavioral;