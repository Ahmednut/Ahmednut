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
-- File : lyt_round.vhd
--------------------------------------------------------------------------------
-- Description : Round std_logic_vector
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_round.vhd,v $
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.1  2013/06/04 13:39:41  julien.roy
-- Add round core
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lyt_round is
  generic (
    INPUT_LENGTH    : integer range 1 to 512 := 10;
    OUTPUT_LENGTH   : integer range 1 to 512 := 8);
  port (
    iv_Data_p   		  : in std_logic_vector(INPUT_LENGTH-1 downto 0);
    i_DataFormat_p   	: in std_logic; -- 0:Unsigned, 1:Signed
    ov_Data_p   		  : out std_logic_vector(OUTPUT_LENGTH-1 downto 0));
end entity lyt_round;


architecture rtl of lyt_round is

  constant HALF_UNIT          : std_logic_vector(iv_Data_p'range) := ((INPUT_LENGTH-OUTPUT_LENGTH-1) => '1', others => '0');
  constant HALF_UNIT_MINUS_1  : std_logic_vector(iv_Data_p'range) := std_logic_vector(unsigned(HALF_UNIT) - to_unsigned(1,HALF_UNIT'length));

  signal data_plus_half_unit          : std_logic_vector(iv_Data_p'range);
  signal data_plus_half_unit_minus_1  : std_logic_vector(iv_Data_p'range);
  
  signal a : std_logic_vector(iv_Data_p'range) :=HALF_UNIT;
  signal a2 : std_logic_vector(iv_Data_p'range) :=HALF_UNIT_MINUS_1;

begin

  process(iv_Data_p, i_DataFormat_p, data_plus_half_unit, data_plus_half_unit_minus_1)
  begin
    data_plus_half_unit <= std_logic_vector(signed(iv_Data_p) + signed(HALF_UNIT));
    data_plus_half_unit_minus_1 <= std_logic_vector(signed(iv_Data_p) + signed(HALF_UNIT_MINUS_1));
    if to_integer(signed(iv_Data_p)) > 0 or i_DataFormat_p = '0' then
      ov_Data_p <= data_plus_half_unit(data_plus_half_unit'length-1 downto data_plus_half_unit'length-OUTPUT_LENGTH);
    else
      ov_Data_p <= data_plus_half_unit_minus_1(data_plus_half_unit_minus_1'length-1 downto data_plus_half_unit_minus_1'length-OUTPUT_LENGTH);
    end if;
  end process;

end rtl;




