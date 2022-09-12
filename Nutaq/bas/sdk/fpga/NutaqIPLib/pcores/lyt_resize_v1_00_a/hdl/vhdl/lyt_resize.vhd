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
-- File : lyt_resize.vhd
--------------------------------------------------------------------------------
-- Description : Resize std_logic_vector
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_resize.vhd,v $
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.2  2013/01/16 18:48:31  julien.roy
-- Add SLL_EN
-- Modify pcore group to Utility
--
-- Revision 1.1  2013/01/09 15:21:03  julien.roy
-- Add resize pcore
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lyt_resize is
  generic (
    INPUT_LENGTH    : integer range 1 to 512 := 14;
    OUTPUT_LENGTH   : integer range 1 to 512 := 16;
    SLL_EN          : boolean := false);
  port (
    iv_Data_p   		  : in std_logic_vector(INPUT_LENGTH-1 downto 0);
    i_DataFormat_p   	: in std_logic; -- 0:Unsigned, 1:Signed
    ov_Data_p   		  : out std_logic_vector(OUTPUT_LENGTH-1 downto 0));
end entity lyt_resize;


architecture rtl of lyt_resize is

  constant sll_size : integer := OUTPUT_LENGTH-INPUT_LENGTH;

  signal data_unsigned  : std_logic_vector(OUTPUT_LENGTH-1 downto 0);
  signal data_signed    : std_logic_vector(OUTPUT_LENGTH-1 downto 0);

begin

  sll_gen : if (SLL_EN and OUTPUT_LENGTH>INPUT_LENGTH) generate
    data_unsigned <= std_logic_vector(resize(unsigned(iv_Data_p),OUTPUT_LENGTH) sll sll_size);
    data_signed   <= std_logic_vector(resize(signed(iv_Data_p),OUTPUT_LENGTH) sll sll_size);
  end generate;
  
  not_sll_gen : if not (SLL_EN and OUTPUT_LENGTH>INPUT_LENGTH) generate
    data_unsigned <= std_logic_vector(resize(unsigned(iv_Data_p),OUTPUT_LENGTH));
    data_signed   <= std_logic_vector(resize(signed(iv_Data_p),OUTPUT_LENGTH));
  end generate;

  ov_Data_p <= data_signed when i_DataFormat_p = '1' else
    data_unsigned;

end rtl;
