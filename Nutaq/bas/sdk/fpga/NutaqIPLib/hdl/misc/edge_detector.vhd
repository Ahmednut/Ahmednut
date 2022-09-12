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
--  Module name    : Edge_detector.vhd                        
--  Project name  : SMC67X-based SignalMaster          
--  Project number  : S9-FPGA                
--  Description    : Edge detector module 
--                - Rising => 1 : Rising edge detector    
--                - Rising => 0 : Falling edge detector
--                                    
--  Author(s)    : Frederic Vachon                    
--                                    
--            Copyright (c) LYRtech inc. 2002          
--                                    
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
--                                    
--                    R e v i s i o n s              
--                                    
-----------------------------------------------------------------------------
--  Date       Author  Version  Description                  
-----------------------------------------------------------------------------
--  15/02/02    FVAC    1.00    - first release            
--                                    
-----------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;


entity edge_detector is

  Port ( 
    D       : in std_logic;
    Rising   : in std_logic;
    Clk     : in std_logic;
    Q       : out std_logic
  );

end edge_detector;



architecture Behavioral of edge_detector is

  signal Q1  : std_logic;
  signal Q2   : std_logic;

begin

  process (Clk)

  begin

    if rising_edge(Clk) then
      Q1 <= D;
      Q2 <= Q1;
    end if;

  end process;

  Q <= (Q1 and not(Q2)) when (Rising = '1') else (not(Q1) and Q2);
      

end Behavioral;
