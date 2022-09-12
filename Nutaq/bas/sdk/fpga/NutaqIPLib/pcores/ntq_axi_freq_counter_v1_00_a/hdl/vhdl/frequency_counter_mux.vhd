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
-- File        : $Id: frequency_counter_mux.vhd,v 1.1 2015/10/30 14:13:23 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Instantiate multiple frequency counters and provide mux
--               selector the read the wanted clock frequency.
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Julien Roy - Initial revision 2014/02/11
-- $Log: frequency_counter_mux.vhd,v $
-- Revision 1.1  2015/10/30 14:13:23  julien.roy
-- Add clock frequency monitoring core.
--
-- Revision 1.1  2014/02/19 16:33:58  julien.roy
-- Add the first version of Nutaq ip core used in the BIST project for the Integrated Transceiver and Processor boards (DSOD1301)
--
--
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library ntq_axi_freq_counter_v1_00_a;
use ntq_axi_freq_counter_v1_00_a.all;

entity frequency_counter_mux is
  generic (
    C_REFCLK_FREQ_HZ    : integer := 100000000;
    C_NUM_OF_TEST_CLK   : integer := 8
  );
  port (
    i_Rst_p             : in std_logic;
    i_RefClk_p          : in std_logic;
    iv_TestClks_p       : in std_logic_vector(C_NUM_OF_TEST_CLK-1 downto 0);
    iv32_TestClkSel_p   : in std_logic_vector(31 downto 0);
    ov32_Freq_p         : out std_logic_vector(31 downto 0)
  );
end entity;

architecture rtl of frequency_counter_mux is

  ----------------------------------------------------------
  -- Type declaration
  ----------------------------------------------------------  
  type FreqCntArray_t is array(natural range <>) of std_logic_vector(31 downto 0);
  
  ----------------------------------------------------------
  -- Signal declaration
  ----------------------------------------------------------  
  signal FreqCntArray_s : FreqCntArray_t(iv_TestClks_p'range);
  
  ----------------------------------------------------------
  -- Attribute declaration
  ----------------------------------------------------------  
  attribute max_fanout: string;
  attribute max_fanout of i_Rst_p: signal is "50";

begin

  ---- Generate all FrequencyCounter instances ----
  frquency_counter_hz_gen: for i in 0 to iv_TestClks_p'high generate

     frequency_counter_hz_inst:  entity ntq_axi_freq_counter_v1_00_a.frequency_counter_hz
       generic map(
         C_REFCLK_FREQ_HZ   => C_REFCLK_FREQ_HZ
       )
       port map(
         i_RefClk_p    => i_RefClk_p,
         i_TestClk_p   => iv_TestClks_p(i),
         i_Rst_p       => i_Rst_p,
         ov32_Freq_p   => FreqCntArray_s(i)
       );

  end generate frquency_counter_hz_gen;

  --- Output mux ----
  -- If clock selector is valid, return the clock frequency
  -- If it is not a valid value, return 0
  process(i_RefClk_p)
  begin
    if rising_edge(i_RefClk_p) then
      if to_integer(unsigned(iv32_TestClkSel_p)) < C_NUM_OF_TEST_CLK then
        ov32_Freq_p <= FreqCntArray_s(to_integer(unsigned(iv32_TestClkSel_p)));
      else
        ov32_Freq_p <= (others => '0');
      end if;
    end if;
  end process;

end architecture;