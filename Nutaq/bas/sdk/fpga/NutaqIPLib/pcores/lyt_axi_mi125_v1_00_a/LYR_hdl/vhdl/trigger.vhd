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
-- File        : $Id: trigger.vhd,v 1.1 2015/04/10 19:47:33 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Trigger port for ADAC250
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2009 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/10/06
-- $Log: trigger.vhd,v $
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.2  2012/12/10 14:30:37  julien.roy
-- Modify ucf to support the 4 FPGA types
-- Add chip enable status ports
-- Add variable delay trigger
-- Move frequency counter status into core registers
--
-- Revision 1.1  2012/10/16 13:17:50  khalid.bensadek
-- First commit of a working version of this project
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.2  2010/02/10 15:02:03  patrick.gilbert
-- working ADC DAC PLL
--
-- Revision 1.1  2010/01/14 22:47:56  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/10/06 15:35:58  karim.ouadid
--------------------------------------------------------------------------------


library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

-- pragma translate_off
library unisim;
  use unisim.vcomponents.all;
-- pragma translate_on


entity Trigger is
generic(
            SRL16_WIDTH : natural range 0 to 32 := 20
        );
port (
    i_clk_p         : in std_logic;
    i_rst_p         : in std_logic;
    i_Trigger_p     : in std_logic;
    o_Trigger_p     : out std_logic;
	 --i_TriggerTap_p  : out std_logic_vector(4 downto 0);
    o_SyncTrigger_p : out std_logic
    );
end entity Trigger;


architecture arch of Trigger is

signal SyncTrigger_s             : std_logic_vector(SRL16_WIDTH-1 downto 0);
signal RisingEdgeTrigger_s       : std_logic;
signal SyncTriggerOut_s          : std_logic;
signal MaintainedRst_s           : std_logic;
signal MaintainedRstCount_s      : std_logic_vector (SRL16_WIDTH+3-1 downto 0);
signal TriggerD1_s               : std_logic;
signal TriggerD2_s               : std_logic;
signal RisingTrigger_s           : std_logic;

begin

--MaintainRst_l : process(i_clk_p)
--
--begin
--if rising_edge( i_clk_p) then
--    if (i_rst_p ='1') then
--       MaintainedRst_s       <= '1';
--       MaintainedRstCount_s  <= (others => '1');
--    elsif (MaintainedRstCount_s(SRL16_WIDTH+3-1)='1' and MaintainedRst_s ='1') then
--       MaintainedRstCount_s  <= MaintainedRstCount_s(SRL16_WIDTH+3-1 downto 1) & '0';
--       MaintainedRst_s       <= '1';
--    else
--       MaintainedRst_s       <= '0';
--    end if;
--end if;
--end process;

MaintainedRst_s <= i_rst_p;

DoubleFlipFlop_l : process(i_clk_p)
begin
if rising_edge( i_clk_p) then
    if (MaintainedRst_s ='1') then
       TriggerD1_s  <= '0';
       TriggerD2_s  <= '0';
    else
       TriggerD1_s  <=  i_Trigger_p;
       TriggerD2_s  <=  TriggerD1_s;
    end if;
end if;
end process;

Edge_detector_inst_l : entity work.Edge_detector_1T
  Port map(
    D         => TriggerD2_s,
    Rising    => '1',
    Clk       => i_clk_p,
    Q         => RisingEdgeTrigger_s
  );

SyncTrigg_l : process(i_clk_p)
begin
  if rising_edge( i_clk_p) then
    --SyncTrigger_s       <= SyncTrigger_s(SRL16_WIDTH-2 downto 0) & RisingEdgeTrigger_s;
      SyncTrigger_s       <= SyncTrigger_s(SRL16_WIDTH-2 downto 0) & TriggerD2_s; --Karim Ouadid: This change is done to make sure the trig last enough to get to the Record module
  end if;
end process;

SyncTriggerOut_s    <= SyncTrigger_s(SRL16_WIDTH-1);
o_Trigger_p         <= TriggerD2_s;
o_SyncTrigger_p     <= SyncTriggerOut_s;

end arch;