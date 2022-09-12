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
-- File        : $Id: FrequencyCounterMux.vhd,v 1.1 2015/04/10 20:11:55 sqa Exp $
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
-- Julien Roy - Initial revision 2012/12/18
-- $Log: FrequencyCounterMux.vhd,v $
-- Revision 1.1  2015/04/10 20:11:55  sqa
-- bas first commit
--
-- Revision 1.1  2013/01/24 17:26:08  julien.roy
-- Add FrequencyCounter source code
--
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

entity FrequencyCounterMux is
  generic (
    C_REFCLK_FREQ_MHZ   : integer := 100;
    C_NUM_OF_TEST_CLK   : integer := 35;
    C_SEL_WIDTH         : integer := 6
  );
  port (
    i_Rst_p          : in std_logic;
    i_RefClk_p       : in std_logic;
    iv_TestClks_p    : in std_logic_vector(C_NUM_OF_TEST_CLK-1 downto 0);
    iv_TestClkSel_p  : in std_logic_vector(C_SEL_WIDTH-1 downto 0);
    ov16_Freq_p      : out std_logic_vector(15 downto 0)
  );
end entity;

architecture rtl of FrequencyCounterMux is

  ----------------------------------------------------------
  -- Component declaration
  ----------------------------------------------------------
  component FrequencyCounter is
    generic (
      C_REFCLK_FREQ_MHZ   : integer := 100
    );
    port (
      i_RefClk_p     : in std_logic;
      i_TestClk_p    : in std_logic;
      i_Rst_p        : in std_logic;
      ov16_Freq_p    : out std_logic_vector(15 downto 0)
    );
  end component;

  ----------------------------------------------------------
  -- Type declaration
  ----------------------------------------------------------  
  type FreqCntArray_t is array(natural range <>) of std_logic_vector(15 downto 0);
  
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
  FrequencyCounter_gen: for i in 0 to iv_TestClks_p'high generate

     FrequencyCounter_inst: FrequencyCounter
       generic map(
         C_REFCLK_FREQ_MHZ   => C_REFCLK_FREQ_MHZ
       )
       port map(
         i_RefClk_p    => i_RefClk_p,
         i_TestClk_p   => iv_TestClks_p(i),
         i_Rst_p       => i_Rst_p,
         ov16_Freq_p   => FreqCntArray_s(i)
       );

  end generate FrequencyCounter_gen;

  --- Output mux ----
  -- If clock selector is valid, return the clock frequency
  -- If it is not a valid value, return 0
  process(i_RefClk_p)
  begin
    if rising_edge(i_RefClk_p) then
      if to_integer(unsigned(iv_TestClkSel_p)) < C_NUM_OF_TEST_CLK then
        ov16_Freq_p <= FreqCntArray_s(to_integer(unsigned(iv_TestClkSel_p)));
      else
        ov16_Freq_p <= (others => '0');
      end if;
    end if;
  end process;

end architecture;