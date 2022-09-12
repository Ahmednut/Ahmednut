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
-- File        : $Id: ChannelOutOfRange.vhd,v 1.1 2015/04/10 19:48:36 sqa Exp $
--------------------------------------------------------------------------------
-- Description : This module monitors one 12-bit data and
--               asserts an out of range if the limit value is reached.
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/09/30
-- $Log: ChannelOutOfRange.vhd,v $
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.1  2013/01/10 14:41:02  julien.roy
-- Rename ch_out_of_range to ChannelOutOfRange
--
-- Revision 1.1  2012/09/28 19:31:26  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.1  2011/08/30 20:00:17  patrick.gilbert
-- add calibration core inside FPGA : beta
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.1  2010/01/14 22:48:37  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/09/30 15:35:58  karim.ouadid 
--------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;
    
entity ChannelOutOfRange is
  generic (
    PatternOffsetBin   : std_logic_vector(11 downto 0);
    Pattern2s          : std_logic_vector(11 downto 0)
  );
  port(
    i_clk_p         : in std_logic;
    i_rst_p         : in std_logic;
    iv12_DataCh_p   : in std_logic_vector (11 downto 0);
    i_DataType_p    : in std_logic;
    o_Ch_oor_p      : out std_logic
  );
end ChannelOutOfRange;

architecture arch of ChannelOutOfRange is

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal Ch_oor_s : std_logic;

begin

  U_OutOfRange_l: process (i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if i_rst_p ='1' then
        Ch_oor_s <= '0';
      else
        if (i_DataType_p ='0') then
          if (iv12_DataCh_p = PatternOffsetBin) then
            Ch_oor_s <= '1'; 
          else
            Ch_oor_s <= '0';
          end if;
        else
          if (iv12_DataCh_p = Pattern2s) then
            Ch_oor_s <= '1'; 
          else
            Ch_oor_s <= '0';
          end if;
        end if; 
      end if;            
    end if;
  end process U_OutOfRange_l;                           

  o_Ch_oor_p <= Ch_oor_s;

end arch;