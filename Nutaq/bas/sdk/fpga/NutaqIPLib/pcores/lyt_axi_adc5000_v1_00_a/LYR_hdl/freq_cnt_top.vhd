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
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2012/09/28 19:08:53  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
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
    C_NUM_OF_TEST_CLK   : integer := 6
  );
  port (                                                                  
    i_Rst_p          : in std_logic;
    i_RefClk_p       : in std_logic;    
    iv_TestClks_p    : in std_logic_vector(C_NUM_OF_TEST_CLK-1 downto 0);     
    iv4_TestClkSel_p : in std_logic_vector(3 downto 0);  
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
process(iv4_TestClkSel_p, FreqCntArray_s)
begin
   ov16_Freq_p <= FreqCntArray_s(to_integer(unsigned(iv4_TestClkSel_p)));  
end process;         
      
  
end architecture;                                                           
                                                                            
                                                                            