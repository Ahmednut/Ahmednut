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
-- File : lyt_mux16.vhd
--------------------------------------------------------------------------------
-- Description : Multiplexer with 2 inputs
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_mux4.vhd,v $
-- Revision 1.1  2015/06/26 13:46:50  christian.dufour-bou
-- Add mux4
--
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.1  2014/10/14 16:11:52  julien.roy
-- Add mux16 core
--
-- Revision 1.1  2013/01/09 15:20:38  julien.roy
-- Add mux2 pcore
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lyt_mux4 is
  generic (
    DATA_LENGTH    : integer range 1 to 512 := 16
  );
  port (
    iv_Data1_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data2_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data3_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data4_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv2_Sel_p       : in std_logic_vector(1 downto 0);
    ov_Data_p       : out std_logic_vector(DATA_LENGTH-1 downto 0));
end entity lyt_mux4;


architecture rtl of lyt_mux4 is
begin

  ov_Data_p <=  iv_Data1_p  when iv2_Sel_p = "00" else
                iv_Data2_p  when iv2_Sel_p = "01" else
                iv_Data3_p  when iv2_Sel_p = "10" else
                iv_Data4_p;

end rtl;



