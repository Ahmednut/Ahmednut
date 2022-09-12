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
--	Module name	: Edge_detector_1t.vhd												
--	Project name	: SMC67X-based SignalMaster					
--	Project number	: S9-FPGA								
--	Description	: Edge detector module 
--					- Rising => 1 : Rising edge detector		
--					- Rising => 0 : Falling edge detector
--																		
--	Author(s)		: Frederic Vachon										
--																		
--						Copyright (c) LYRtech inc. 2002					
--																		
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
--																		
--										R e v i s i o n s							
--																		
-----------------------------------------------------------------------------
--	Date   		Author	Version	Description									
-----------------------------------------------------------------------------
--	15/02/02		FVAC		1.00		- first release						
--																		
-----------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity Edge_detector_1t is

	Port ( 
		D 			: in std_logic;
		Rising 	: in std_logic;
      Clk 		: in std_logic;
      Q 			: out std_logic
	);

end Edge_detector_1t;



architecture rtl of Edge_detector_1t is

	signal Q1	: std_logic;

begin

	process (Clk)

	begin

		if rising_edge(Clk) then
			Q1 <= D;
		end if;

	end process;

	Q <= (D and not(Q1)) when (Rising = '1') else (not(D) and Q1);
			

end rtl;
