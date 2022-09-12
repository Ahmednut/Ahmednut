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
-- $Log: lyt_clkmon_a.vhd,v $
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
-- Revision 1.1  2009/10/20 19:01:44  francois.blackburn
-- first working version
--
-- Revision 1.1  2009/08/24 15:35:06  francois.blackburn
-- first version
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.numeric_std.all;


architecture rtl of lyt_clkmon is


  component clkmon is
    port (
      i_clkRef_p : in std_logic;
      i_clkIn_p : in std_logic;
      o_ClkInStop_p : out std_logic
    );
  end component;

begin

  clkmon_l : clkmon
    port map (
      i_clkRef_p => i_clkRef_p,
      i_clkIn_p => i_clkIn_p,
      o_ClkInStop_p => o_ClkInStop_p
    );




end architecture rtl;

