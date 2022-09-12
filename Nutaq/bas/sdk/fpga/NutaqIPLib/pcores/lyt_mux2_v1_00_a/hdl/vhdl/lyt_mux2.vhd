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
-- File : lyt_mux2.vhd
--------------------------------------------------------------------------------
-- Description : Multiplexer with 2 inputs
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_mux2.vhd,v $
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.1  2013/01/09 15:20:38  julien.roy
-- Add mux2 pcore
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lyt_mux2 is    
  generic (
    DATA_LENGTH    : integer range 1 to 512 := 16
  );
  port (
    iv_Data1_p    : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data2_p    : in std_logic_vector(DATA_LENGTH-1 downto 0);
    i_Sel_p       : in std_logic; -- 0:iv_Data1_p, 1:iv_Data2_p
    ov_Data_p     : out std_logic_vector(DATA_LENGTH-1 downto 0));
end entity lyt_mux2;


architecture rtl of lyt_mux2 is
begin
  
  ov_Data_p <= iv_Data2_p when i_Sel_p = '1' else
    iv_Data1_p;
  
end rtl;


