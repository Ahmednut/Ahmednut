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
-- File        : $Id: a_latch_sclear_ea.vhd,v 1.1 2015/04/10 20:11:55 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Asynchronous Status bit that needs to be latched.
--               With Synchronous Clear.
--   
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--  
--------------------------------------------------------------------------------
-- Copyright (c) 2006 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: a_latch_sclear_ea.vhd,v $
-- Revision 1.1  2015/04/10 20:11:55  sqa
-- bas first commit
--
-- Revision 1.2  2012/12/07 20:50:47  julien.roy
-- Change Lyrtech logo to Nutaq logo
--
-- Revision 1.1  2010/06/17 15:39:29  francois.blackburn
-- first commit (move from lsp)
--
-- Revision 1.1  2006/10/02 13:06:47  guy.menard
-- Commit in new branch
--
-- Revision 1.1  2006/06/01 20:03:28  guy.menard
-- Initial release
--
--------------------------------------------------------------------------------

library IEEE;
  use IEEE.std_logic_1164.all;

entity ALatchSClear is
  port (
    i_Clk_p                    : in std_logic;
    i_Status_p                 : in std_logic;
    i_StatusClear_p            : in std_logic;
    o_StatusLatched_p          : out std_logic
  );
end entity ALatchSClear;
  
architecture Rtl of ALatchSClear is
begin
  
  Latch_process : process (i_Clk_p, i_Status_p)
  begin
    if (i_Status_p = '1') then
      -- Asynchronous latching of status
      o_StatusLatched_p   <= '1';
    elsif rising_edge(i_Clk_p) then
      -- Synchronous clearing of latched status
      if (i_StatusClear_p = '1') then
        o_StatusLatched_p   <= '0';
      end if;
    end if;
  end process Latch_process;

end architecture Rtl;
  
