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
-- File        : $Id: clkmon_e.vhd,v
--------------------------------------------------------------------------------
-- Description : Clock monitoring entity.
--               This modules samples the monitored clock to detect transitions.
--
--------------------------------------------------------------------------------
-- Notes / Assumptions : Monitoring clock is 125MHz
--                       Monitored clocks range from 10MHz to 125MHz
--
--------------------------------------------------------------------------------
-- Copyright (c) 2006 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: clkmon_e.vhd,v $
-- Revision 1.3  2015/07/21 21:06:01  julien.roy
-- Readd clkmon. It was used in the ADAC250 pcore.
--
-- Revision 1.1  2015/04/10 19:48:37  sqa
-- bas first commit
--
-- Revision 1.1  2013/01/24 17:25:27  julien.roy
-- Add pcore clkmon
--
-- Revision 1.1  2009/10/20 19:01:45  francois.blackburn
-- first working version
--
-- Revision 1.1  2009/08/24 15:35:06  francois.blackburn
-- first version
--
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;

entity clkmon is
  port (
    -- Reference clock
    i_clkRef_p                 : in std_logic;
    -- Monitored clock
    i_clkIn_p                  : in std_logic;
    o_ClkInStop_p              : out std_logic
  );
end entity clkmon;

