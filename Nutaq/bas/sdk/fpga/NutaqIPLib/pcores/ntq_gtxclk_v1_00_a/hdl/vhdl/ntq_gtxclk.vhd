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
-- File : ntq_gtxclk.vhd
--------------------------------------------------------------------------------
-- Description : GTX clock buffer
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: ntq_gtxclk.vhd,v $
-- Revision 1.1  2015/05/20 14:02:49  julien.roy
-- Merge modifications from Perseus611X branch
--
-- Revision 1.1  2014/06/25 14:37:21  julien.roy
-- Add ntq_gtxclk core
--
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

library ntq_gtxclk_v1_00_a;
use ntq_gtxclk_v1_00_a.all;

library unisim;
use unisim.vcomponents.all;

entity ntq_gtxclk is
  port (
    idp_GtxClk_p    : in std_logic;
    idn_GtxClk_p    : in std_logic;
    o_ClkBufg_p     : out std_logic
  );
end entity ntq_gtxclk;


architecture rtl of ntq_gtxclk is
    
    signal internal_clock_s : std_logic;

begin

    -- GTX clock buffer
    IBUFDS_i :  IBUFDS_GTXE1
    port map (
        I     => idp_GtxClk_p,
        IB    => idn_GtxClk_p,
        CEB   => '0',
        O     => internal_clock_s,
        ODIV2 => OPEN);

    bufg_u0: bufg
    port map(
        O => o_ClkBufg_p,      
        I => internal_clock_s
    );

end rtl;
