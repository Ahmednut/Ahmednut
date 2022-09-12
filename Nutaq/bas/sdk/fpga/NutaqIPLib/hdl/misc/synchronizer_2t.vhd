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
--
--   Module name      : Synchronizer_2T.vhd
--   Project name   : SMC67X-based SignalMaster
--   Project number   : S9-FPGA
--   Description      : Synchronizer (protects against meta-stability)
--
--   Author(s)      : Mario Bergeron
--
--                  Copyright (c) LYRtech inc. 2002
--
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
--
--                              R e v i s i o n s
--
-----------------------------------------------------------------------------
--   Date         Author   Version   Description
-----------------------------------------------------------------------------
--   25/03/02      MBER      1.00      - first release
-- 30/05/02      FVAC      1.01      - Remove Generic declaration and replace by
--                                   reset_value input port.
-- 02/07/02      FVAC      1.02      - Rename component
--
-----------------------------------------------------------------------------

library ieee;
   use ieee.std_logic_1164.all;


entity synchronizer_2t is

   Port
   (
      Clk          : in std_logic;
      Reset         : in std_logic;
      Reset_value : in std_logic;
      Async         : in std_logic;
      Sync         : out std_logic
   );

end synchronizer_2t;


architecture rtl of synchronizer_2t is

   signal tmp   : std_logic;

   begin

      synchronizer_process : process ( Clk, Reset, Reset_value )

         begin
            if ( Reset = '1' ) then
               tmp <= Reset_value;
               Sync <= Reset_value;
            elsif rising_edge(Clk) then
               tmp <= Async;
               Sync <= tmp;
            end if;
      end process;

end rtl;
