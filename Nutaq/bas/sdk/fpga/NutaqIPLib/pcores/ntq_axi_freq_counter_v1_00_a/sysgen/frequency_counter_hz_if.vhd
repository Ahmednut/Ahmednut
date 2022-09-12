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

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity frequency_counter_hz_if is
 	port(
        clk                     : in std_logic;
        ce                      : in std_logic;
 		
		-- Freq Counter IF --
		i_TestClk_p    			: in std_logic; 
		ov32_Freq_p    			: out std_logic_vector(31 downto 0)
		
  );
end frequency_counter_hz_if;

architecture arch of frequency_counter_hz_if is

    component frequency_counter_hz is
    port(
 		i_RefClk_p     			: in std_logic;
		i_TestClk_p    			: in std_logic;
		i_Rst_p        			: in std_logic; 
		ov32_Freq_p    			: out std_logic_vector(31 downto 0)
    );
    end component;

begin

    frequency_counter_hz_inst : frequency_counter_hz
    port map(
 		i_Rst_p			    => '0',
 		i_RefClk_p			=> clk,
		-- Freq Counter IF --
 		i_TestClk_p	        => i_TestClk_p,
 		ov32_Freq_p	        => ov32_Freq_p);
		
 end arch;