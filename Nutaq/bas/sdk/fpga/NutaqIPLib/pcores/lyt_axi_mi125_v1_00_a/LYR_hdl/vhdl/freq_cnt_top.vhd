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
-- File        : $Id: freq_cnt_top.vhd,v
--------------------------------------------------------------------------------
-- Description : Clock Frequency counter
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: freq_cnt_top.vhd,v $
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.3  2012/12/11 15:28:12  julien.roy
-- Add max fanout constraint for frequency counter reset
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
-- Revision 1.4  2012/02/01 14:36:32  khalid.bensadek
-- Update du projet
--

--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

entity freq_cnt_top is
  generic (
    C_REFCLK_FREQ_MHZ   : integer := 100;
    C_NUM_OF_TEST_CLK   : integer := 35
  );
  port (
    i_Rst_p          : in std_logic;
    i_RefClk_p       : in std_logic;
    iv_TestClks_p    : in std_logic_vector(C_NUM_OF_TEST_CLK-1 downto 0);
    iv6_TestClkSel_p : in std_logic_vector(5 downto 0);
    ov16_Freq_p      : out std_logic_vector(15 downto 0)
  );
end entity;

architecture behavior of freq_cnt_top is

  component freq_cnt is
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

  signal v16_FreqClk200Mhz_s, v16_FreqClkA_s, v16_FreqClkB_s, v16_FreqClkC_s,
         v16_FreqClkD_s, v16_FreqClk2fpga_s, sip_freqOut_s : std_logic_vector(15 downto 0);

  type FreqCntArray_t is array(natural range <>) of std_logic_vector(15 downto 0);
  signal FreqCntArray_s : FreqCntArray_t(iv_TestClks_p'range);
  
  attribute max_fanout: string;
  attribute max_fanout of i_Rst_p: signal is "50";

begin

  Clk_counters_gen: for i in 0 to iv_TestClks_p'high generate

     freq_cnt_inst: freq_cnt
       generic map(
         C_REFCLK_FREQ_MHZ   => C_REFCLK_FREQ_MHZ
       )
       port map(
         i_RefClk_p    => i_RefClk_p,
         i_TestClk_p   => iv_TestClks_p(i),
         i_Rst_p       => i_Rst_p,
         ov16_Freq_p   => FreqCntArray_s(i)
       );

  end generate Clk_counters_gen;

  --- Output mux ---
  process(iv6_TestClkSel_p, FreqCntArray_s)
  begin
     ov16_Freq_p <= FreqCntArray_s(to_integer(unsigned(iv6_TestClkSel_p)));
  end process;


end architecture;