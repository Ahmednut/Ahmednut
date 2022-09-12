--------------------------------------------------------------------------------
--
--
--          **  **     **  ******  ********  ********  ********  **    **
--         **    **   **  **   ** ********  ********  ********  **    **
--        **     *****   **   **    **     **        **        **    **
--       **       **    ******     **     ****      **        ********
--      **       **    **  **     **     **        **        **    **
--     *******  **    **   **    **     ********  ********  **    **
--    *******  **    **    **   **     ********  ********  **    **
--
--                       L Y R T E C H   R D   I N C
--
--------------------------------------------------------------------------------
-- File        : $Id: lyt_freq_counter.vhd,v
--------------------------------------------------------------------------------
-- Description : Frequency counter
--               Calculates the frequency of the each input clock
--               and outputs a 16 bit value giving frequency in MHz.
--
--------------------------------------------------------------------------------
-- Notes / Assumptions : Reference clock is 100MHz.
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: lyt_freq_counter.vhd,v $
-- Revision 1.1  2015/04/10 19:48:37  sqa
-- bas first commit
--
-- Revision 1.1  2014/01/29 16:32:14  julien.roy
-- Add freq counter core used in the radio420 BSDK exemple in the prod test
--
-- Revision 1.1  2011/08/08 19:53:59  jeffrey.johnson
-- First commit.
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;

entity lyt_freq_counter is
  generic (
    C_REFCLK_FREQ_MHZ    : integer := 100
  );
  port (
    -- Reset
    i_Rst_p              : in std_logic; 
    -- Reference clock input
    i_RefClk_p           : in std_logic;
    -- Clock inputs
    i_ClkIn0_p           : in std_logic;
    i_ClkIn1_p           : in std_logic;
    i_ClkIn2_p           : in std_logic;
    i_ClkIn3_p           : in std_logic;
    -- Frequency counter outputs
    ov16_ClkFreq0_p      : out std_logic_vector(15 downto 0);
    ov16_ClkFreq1_p      : out std_logic_vector(15 downto 0);
    ov16_ClkFreq2_p      : out std_logic_vector(15 downto 0);
    ov16_ClkFreq3_p      : out std_logic_vector(15 downto 0)
  );
end entity lyt_freq_counter;

architecture rtl of lyt_freq_counter is


begin

  Clk0_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_ClkIn0_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_ClkFreq0_p
  );
    
  Clk1_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_ClkIn1_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_ClkFreq1_p
  );
    
  Clk2_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_ClkIn2_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_ClkFreq2_p
  );
    
  Clk3_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_ClkIn3_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_ClkFreq3_p
  );
    

end architecture rtl;

