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
-- File : adc5000_pkg.vhd
--------------------------------------------------------------------------------
-- Description : Package to ADC5000 internal use
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: adc5000_pkg.vhd,v $
-- Revision 1.1  2015/04/10 19:47:30  sqa
-- bas first commit (CBOU)
--
-- Revision 1.5  2013/06/03 16:25:55  julien.roy
-- Major modification of the adc5000 netlist (calibration, ...)
--
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;


package adc5000_pkg is

component adc5000_top is
port (
  -- Global signals
  rst              : in    std_logic;
  REF_CLOCK        : in    std_logic;                           -- Ref clk for IDELAYCTRL

  --- Registers Interface ---
  -- SERDES control PHY registers
  clk_regs         : in  std_logic;                   -- system/PLB Microblaze clock that registers are synced to it
  Phy_cmd_in       : in std_logic_vector(3 downto 0); -- commands pulses registers: shold be a pulses
  i_RstFifo_p      : in std_logic;                          -- reset signal for fifo
  
  iv5_adcIdelayValueA_p     : in std_logic_vector(4 downto 0);
  iv5_adcIdelayValueB_p     : in std_logic_vector(4 downto 0);
  iv5_adcIdelayValueC_p     : in std_logic_vector(4 downto 0);
  iv5_adcIdelayValueD_p     : in std_logic_vector(4 downto 0);
  iv10_adcIdelayValueA_we_p : in std_logic_vector(9 downto 0);
  iv10_adcIdelayValueB_we_p : in std_logic_vector(9 downto 0);
  iv10_adcIdelayValueC_we_p : in std_logic_vector(9 downto 0);
  iv10_adcIdelayValueD_we_p : in std_logic_vector(9 downto 0);
  iv10_bitslipA_p           : in std_logic_vector(9 downto 0);
  iv10_bitslipB_p           : in std_logic_vector(9 downto 0);
  iv10_bitslipC_p           : in std_logic_vector(9 downto 0);
  iv10_bitslipD_p           : in std_logic_vector(9 downto 0);
  
  ov10_calibErrorChA_p   : out std_logic_vector(9 downto 0);
  ov10_calibErrorChB_p   : out std_logic_vector(9 downto 0);
  ov10_calibErrorChC_p   : out std_logic_vector(9 downto 0);
  ov10_calibErrorChD_p   : out std_logic_vector(9 downto 0);

  -- Trigger control Register
  iv5_TriggerDelay_p    : in std_logic_vector(4 downto 0);
  -- Frequancy counter registers
  FeqCnt_Cnt_rst   : in std_logic;                     -- reset pulse to clear the counters each time we request a new freq count
  FeqCnt_clk_sel   : in  std_logic_vector(3 downto 0); -- Select the clock that needs to be counted
  FeqCnt_clk_cnt   : out std_logic_vector(15 downto 0);-- Selected clock count output

  --- ADC output user data
  phy_clk          : out std_logic;
  phy_data_a       : out std_logic_vector(127 downto 0);
  phy_data_b       : out std_logic_vector(127 downto 0);
  phy_data_c       : out std_logic_vector(127 downto 0);
  phy_data_d       : out std_logic_vector(127 downto 0);
  ov4_Otr_p        : out std_logic_vector(3 downto 0);

  -- Triggers output
  triggerOut          : out std_logic;

  --External signals
--  fmc_to_cpld      : inout std_logic_vector(3 downto 0);
--  front_io_fmc     : inout std_logic_vector(3 downto 0);

  --- Serdes external ports
  clk_to_fpga_p    : in    std_logic;
  clk_to_fpga_n    : in    std_logic;
  ext_trigger_p    : in    std_logic;
  ext_trigger_n    : in    std_logic;
  sync_from_fpga_p : out   std_logic;
  sync_from_fpga_n : out   std_logic;

  adr_p            : in    std_logic;
  adr_n            : in    std_logic;
  ad_p             : in    std_logic_vector(9 downto 0);
  ad_n             : in    std_logic_vector(9 downto 0);
  aor_p            : in    std_logic;
  aor_n            : in    std_logic;

  bdr_p            : in    std_logic;
  bdr_n            : in    std_logic;
  bd_p             : in    std_logic_vector(9 downto 0);
  bd_n             : in    std_logic_vector(9 downto 0);
  bor_p            : in    std_logic;
  bor_n            : in    std_logic;

  cdr_p            : in    std_logic;
  cdr_n            : in    std_logic;
  cd_p             : in    std_logic_vector(9 downto 0);
  cd_n             : in    std_logic_vector(9 downto 0);
  cor_p            : in    std_logic;
  cor_n            : in    std_logic;

  ddr_p            : in    std_logic;
  ddr_n            : in    std_logic;
  dd_p             : in    std_logic_vector(9 downto 0);
  dd_n             : in    std_logic_vector(9 downto 0);
  dor_p            : in    std_logic;
  dor_n            : in    std_logic
);
end component adc5000_top;

end adc5000_pkg;