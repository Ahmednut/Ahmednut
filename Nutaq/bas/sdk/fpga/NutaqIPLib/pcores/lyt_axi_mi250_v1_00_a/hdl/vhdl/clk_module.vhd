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
-- File        : $Id: clk_module.vhd,v 1.3 2016/01/26 13:58:43 christian.dufour-bou Exp $
--------------------------------------------------------------------------------
-- Description : Clock Module
--               Instantiates MMCMs, BUFGs and freq counters for the clocks
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: clk_module.vhd,v $
-- Revision 1.3  2016/01/26 13:58:43  christian.dufour-bou
-- MI250 channels names changed to 1 - 8 instead of A - H and trigger no longer managed by the MI250 core
--
-- Revision 1.2  2015/06/10 12:48:36  julien.roy
-- Add bitslip functionnality to have more possibility to find a valid calibration.
-- Add clock master/slave logic.
-- Move constraints directly inside the .tcl instead of external .ucf files.
--
-- Revision 1.1  2015/04/10 19:47:34  sqa
-- bas first commit (CBOU)
--
-- Revision 1.4  2012/11/26 20:46:45  julien.roy
-- Add GENERIC port for MMCM control.
-- Modify pattern error verification to ease timing.
--
-- Revision 1.3  2012/11/20 14:02:02  khalid.bensadek
-- Cleaned-up unused signals.
--
-- Revision 1.2  2012/11/12 19:34:16  khalid.bensadek
-- Optimized design to use one MMCM only instead of 4. That way, we can use Record-playback ip core with it.
--
-- Revision 1.2  2011/05/25 20:36:54  jeffrey.johnson
-- Removed 16 to 64 bit FIFO.
--
-- Revision 1.1  2011/05/25 16:12:30  jeffrey.johnson
-- Moved MMCM from wrapper to clk_module.
-- Added freq counters for all clocks.
--
--
--------------------------------------------------------------------------------

library ieee;                                                               
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
library unisim;
  use unisim.vcomponents.all;

entity clk_module is
  generic (
    C_REFCLK_FREQ_MHZ    : integer := 100
  );
  port (                                                                  
    i_Rst_p              : in std_logic; 
    i_RefClk_p           : in std_logic;
    -- Clock inputs (from BUFR)
    i_ClkToFpgaBufr_p    : in std_logic;
    i_ExternClkBufr_p    : in std_logic;
    i_Adc12ClkBufr_p     : in std_logic;
    i_Adc34ClkBufr_p     : in std_logic;
    i_Adc56ClkBufr_p     : in std_logic;
    i_Adc78ClkBufr_p     : in std_logic;
    -- Global clock outputs (from BUFG)
    o_Adc12ClkBufg_p        : out std_logic;
    o_Adc12ClkBufgLocked_p  : out std_logic;
    -- Frequency counter outputs
    ov16_ClkToFpgaFreq_p : out std_logic_vector(15 downto 0);
    ov16_ExternClkFreq_p : out std_logic_vector(15 downto 0);
    ov16_Adc12ClkFreq_p  : out std_logic_vector(15 downto 0);
    ov16_Adc34ClkFreq_p  : out std_logic_vector(15 downto 0);
    ov16_Adc56ClkFreq_p  : out std_logic_vector(15 downto 0);
    ov16_Adc78ClkFreq_p  : out std_logic_vector(15 downto 0)
  );                                                                      
end entity;
                                                
architecture behavior of clk_module is                                         
  
  signal Clk12Bufg_s       : std_logic;

begin                                                                       

  ----------------------------------------------------------------------------------------------------
  -- Clock-to-FPGA Frequency counter
  ----------------------------------------------------------------------------------------------------
  
  ClkToFpga_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_ClkToFpgaBufr_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_ClkToFpgaFreq_p
  );
    
  ----------------------------------------------------------------------------------------------------
  -- External Clock Frequency counter
  ----------------------------------------------------------------------------------------------------
  
  ExternClk_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_ExternClkBufr_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_ExternClkFreq_p
  );
    
  ----------------------------------------------------------------------------------------------------
  -- MMCM for channel 1 and 2
  ----------------------------------------------------------------------------------------------------   

  -- Make sure the clock is routed on a global net
  bufg_12_inst : bufg
  port map (
    i  => i_Adc12ClkBufr_p,
    o  => Clk12Bufg_s
  );


  ----------------------------------------------------------------------------------------------------
  -- Frequency counter for ADC channel 1 and 2
  ----------------------------------------------------------------------------------------------------
  
  Adc12_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => Clk12Bufg_s,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_Adc12ClkFreq_p
  );
    
  ----------------------------------------------------------------------------------------------------
  -- Frequency counter for ADC channel 3 and 4
  ----------------------------------------------------------------------------------------------------
  
  Adc34_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_Adc34ClkBufr_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_Adc34ClkFreq_p
  );
    
  ----------------------------------------------------------------------------------------------------
  -- Frequency counter for ADC channel 5 and 6
  ----------------------------------------------------------------------------------------------------
  
  Adc56_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_Adc56ClkBufr_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_Adc56ClkFreq_p
  );
    
  ----------------------------------------------------------------------------------------------------
  -- Frequency counter for ADC channel 7 and 8
  ----------------------------------------------------------------------------------------------------
  
  Adc78_FreqCnt  : entity work.freq_cnt
  generic map
  (
    C_REFCLK_FREQ_MHZ  =>  C_REFCLK_FREQ_MHZ
  )
  port map
  (
    i_RefClk_p         => i_RefClk_p,
    i_TestClk_p        => i_Adc78ClkBufr_p,
    i_Rst_p            => i_Rst_p,
    ov16_Freq_p        => ov16_Adc78ClkFreq_p
  );
    
  ----------------------------------------------------------------------------------------------------
  -- Output mapping
  ----------------------------------------------------------------------------------------------------
  o_Adc12ClkBufg_p          <= Clk12Bufg_s;
  o_Adc12ClkBufgLocked_p    <= '1';

end architecture;                                                           
                                                                            
                                                                            