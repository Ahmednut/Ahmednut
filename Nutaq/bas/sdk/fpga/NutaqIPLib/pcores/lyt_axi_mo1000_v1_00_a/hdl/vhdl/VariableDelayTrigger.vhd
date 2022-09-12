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
-- File        : $Id: VariableDelayTrigger.vhd,v 1.1 2015/04/10 19:48:35 sqa Exp $
--------------------------------------------------------------------------------
-- Description : VariableDelayTrigger port
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Julien Roy - Initial revision 2012/12/07
-- $Log: VariableDelayTrigger.vhd,v $
-- Revision 1.1  2015/04/10 19:48:35  sqa
-- bas first commit
--
-- Revision 1.1  2014/06/18 14:43:05  julien.roy
-- Add first version of the mo1000 core
--
-- Revision 1.2  2012/12/10 16:46:40  julien.roy
-- Modify trigger delay from 0-31 to 1-32. The previous configuration produced timing failure.
--
-- Revision 1.1  2012/12/07 20:49:42  julien.roy
-- First commit of a working version of VariableDelayTrigger.
-- Delay the trigger from 0 (async) to 31 clock cycles.
--
--------------------------------------------------------------------------------


library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library unisim;
  use unisim.vcomponents.all;

entity VariableDelayTrigger is
port (
    i_clk_p         : in std_logic;
    iv5_Delay_p     : in std_logic_vector(4 downto 0);
    i_Trigger_p     : in std_logic;
    o_Trigger_p     : out std_logic
    );
end entity VariableDelayTrigger;


architecture arch of VariableDelayTrigger is

  signal shift_D_s      : std_logic;
  signal shift_Q_s      : std_logic;

begin

  shift_D_s <= i_Trigger_p;
  
  SRLC32E_inst : SRLC32E
  generic map (
    INIT => X"00000000")
  port map (
    A => iv5_Delay_p,  -- 5-bit shift depth select input
    CLK => i_clk_p,   -- Clock input
    CE => '1',        -- Clock enable input
    D => shift_D_s,   -- SRL data input
    Q => shift_Q_s    -- SRL data output
  );
  
  o_Trigger_p <= shift_Q_s;

end arch;