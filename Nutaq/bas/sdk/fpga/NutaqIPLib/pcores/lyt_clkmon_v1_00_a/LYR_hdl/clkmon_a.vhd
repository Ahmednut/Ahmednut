--------------------------------------------------------------------------------
--
--
--          **  **     **  ******  ********  ********  ********  **    **
--         **    **   **  **   ** ********  ********  ********  **    **
--        **     *****   **   **    **     **        **        **    **
--       **       **    ******     **     ****      **        ********
--      **       **    **  **     **     **        **        **    **
--     *******  **    **   **    **     ********  ********  **    **
--    *******  **    **    **   **     ********  ********  **    **
--
--                       L Y R T E C H   R D   I N C
--
--------------------------------------------------------------------------------
-- File        : $Id: lyt_clkmon_a.vhd,v 1.1
--------------------------------------------------------------------------------
-- Description : Clock monitoring architecture.
--               This modules samples the monitored clock to detect transitions.
--
--------------------------------------------------------------------------------
-- Notes / Assumptions : Monitoring reference clock is 125MHz
--                       Monitored clocks range from 10MHz to 125MHz
--
--------------------------------------------------------------------------------
-- Copyright (c) 2006 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: clkmon_a.vhd,v $
-- Revision 1.3  2015/07/21 21:06:01  julien.roy
-- Readd clkmon. It was used in the ADAC250 pcore.
--
-- Revision 1.1  2015/04/10 19:48:37  sqa
-- bas first commit
--
-- Revision 1.1  2013/01/24 17:25:27  julien.roy
-- Add pcore clkmon
--
-- Revision 1.2  2010/06/18 14:19:43  francois.blackburn
-- change LYR for lyt
--
-- Revision 1.1  2009/10/20 19:01:45  francois.blackburn
-- first working version
--
-- Revision 1.1  2009/08/24 15:35:06  francois.blackburn
-- first version
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.numeric_std.all;


architecture rtl of clkmon is
  -- declarations
  constant v10_All1_c               : std_logic_vector(9 downto 0) := (others => '1');
  constant v10_All0_c               : std_logic_vector(9 downto 0) := (others => '0');
  constant v16_All1_c               : std_logic_vector(15 downto 0) := (others => '1');
  constant v16_All0_c               : std_logic_vector(15 downto 0) := (others => '0');


  signal v16_DirectSample_s         : std_logic_vector(15 downto 0);
  signal v10_Div4Sample_s           : std_logic_vector(9 downto 0);
  signal Div4ClkCnt_s               : unsigned(1 downto 0);


begin
  --------------------------------------------
  -- Process DivideClockIn
  --------------------------------------------
  DivideClockIn_process : process (i_clkIn_p)
  begin
    if rising_edge(i_clkIn_p) then
      Div4ClkCnt_s   <= Div4ClkCnt_s + 1;
    end if;
  end process DivideClockIn_process;

  --------------------------------------------
  -- Process MonitorDividedClock: For rapid clocks, the clock is divided before sampling
  -- to see the toggling of the clock in the samples.
  --------------------------------------------
  MonitorDividedClock_process : process (i_clkRef_p)
  begin
    if rising_edge(i_clkRef_p) then
      v10_Div4Sample_s   <= v10_Div4Sample_s(8 downto 0) & Div4ClkCnt_s(1);
    end if;
  end process MonitorDividedClock_process;

  --------------------------------------------
  -- Process MonitorDirectClock: For slow clocks, the clock is sampled directly
  -- to see the toggling of the clock in the samples and to avoid taking too many samples.
  --------------------------------------------
  MonitorDirectClock_process : process (i_clkRef_p)
  begin
    if rising_edge(i_clkRef_p) then
      v16_DirectSample_s   <= v16_DirectSample_s(14 downto 0) & i_clkIn_p;
    end if;
  end process MonitorDirectClock_process;

  --------------------------------------------
  -- Process ClockInStopped: if clock is stopped than the samples are all 0 or 1.
  --------------------------------------------
  ClockInStopped_process : process (i_clkRef_p)
  begin
    if rising_edge(i_clkRef_p) then
      if ((v10_Div4Sample_s = v10_All1_c or v10_Div4Sample_s = v10_All0_c)  and
          (v16_DirectSample_s = v16_All1_c or v16_DirectSample_s = v16_All0_c)) then
        o_ClkInStop_p <= '1';
      else
        o_ClkInStop_p <= '0';
      end if;
    end if;
  end process ClockInStopped_process;



end architecture rtl;

