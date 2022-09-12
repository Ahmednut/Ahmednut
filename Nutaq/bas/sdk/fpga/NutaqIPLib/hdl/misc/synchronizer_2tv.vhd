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
--    Module name        : Synchronizer_2T.vhd                                                
--    Project name       : SMC67X-based SignalMaster                    
--    Project number     : S9-FPGA                                
--    Description        : Synchronizer (protects against meta-stability)
--                         Support std_logic_vector 
--                                                                        
--    Author(s)          : François Blackburn 
--                                                                        
--                        Copyright (c) LYRtech inc. 2002                    
--                                                                        
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
--                                                                        
--                                        R e v i s i o n s                            
--                                                                        
-----------------------------------------------------------------------------
--    Date           Author    Version    Description                                    
-----------------------------------------------------------------------------
--    25/03/02        MBER        1.00        - first release
--    30/05/02        FVAC        1.01        - Remove Generic declaration and replace by
--                                              reset_value input port.
--    02/07/02        FVAC        1.02        - Rename component
--                                                                        
-----------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;


entity synchronizer_2tv is
    generic( size : integer := 8 );
    Port ( 
        Clk           : in std_logic;
        Reset         : in std_logic;
        Reset_value   : in std_logic_vector( size - 1 downto 0 );
        Async         : in std_logic_vector( size - 1 downto 0 );
        Sync          : out std_logic_vector( size - 1 downto 0 )
    );

end synchronizer_2tv;


architecture RTL of synchronizer_2tv is

    signal tmp    : std_logic_vector( size - 1 downto 0 );

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

end RTL;
