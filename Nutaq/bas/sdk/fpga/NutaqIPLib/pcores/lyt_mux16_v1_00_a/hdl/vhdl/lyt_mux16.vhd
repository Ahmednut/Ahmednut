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
-- $Log: lyt_mux16.vhd,v $
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

entity lyt_mux16 is
  generic (
    DATA_LENGTH    : integer range 1 to 512 := 16
  );
  port (
    iv_Data1_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data2_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data3_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data4_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data5_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data6_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data7_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data8_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data9_p      : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data10_p     : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data11_p     : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data12_p     : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data13_p     : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data14_p     : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data15_p     : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv_Data16_p     : in std_logic_vector(DATA_LENGTH-1 downto 0);
    iv4_Sel_p       : in std_logic_vector(3 downto 0);
    ov_Data_p       : out std_logic_vector(DATA_LENGTH-1 downto 0));
end entity lyt_mux16;


architecture rtl of lyt_mux16 is
begin

  ov_Data_p <=  iv_Data1_p  when iv4_Sel_p = "0000" else
                iv_Data2_p  when iv4_Sel_p = "0001" else
                iv_Data3_p  when iv4_Sel_p = "0010" else
                iv_Data4_p  when iv4_Sel_p = "0011" else
                iv_Data5_p  when iv4_Sel_p = "0100" else
                iv_Data6_p  when iv4_Sel_p = "0101" else
                iv_Data7_p  when iv4_Sel_p = "0110" else
                iv_Data8_p  when iv4_Sel_p = "0111" else
                iv_Data9_p  when iv4_Sel_p = "1000" else
                iv_Data10_p when iv4_Sel_p = "1001" else
                iv_Data11_p when iv4_Sel_p = "1010" else
                iv_Data12_p when iv4_Sel_p = "1011" else
                iv_Data13_p when iv4_Sel_p = "1100" else
                iv_Data14_p when iv4_Sel_p = "1101" else
                iv_Data15_p when iv4_Sel_p = "1110" else
                iv_Data16_p;

end rtl;



