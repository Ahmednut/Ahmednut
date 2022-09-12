
-------------------------------------------------------------------------------------
-- FILE NAME : adc5000_top.vhd
--
-- AUTHOR    : Peter Kortekaas
--
-- COMPANY   : 4DSP
--
-- ITEM      : 1
--
-- UNITS     : Entity       - adc5000_top
--             architecture - adc5000_top_syn
--
-- LANGUAGE  : VHDL
--
-------------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------------
-- DESCRIPTION
-- ===========
--
-- adc5000_top
-- Notes: adc5000_top
-------------------------------------------------------------------------------------
--  Disclaimer: LIMITED WARRANTY AND DISCLAIMER. These designs are
--              provided to you as is.  4DSP specifically disclaims any
--              implied warranties of merchantability, non-infringement, or
--              fitness for a particular purpose. 4DSP does not warrant that
--              the functions contained in these designs will meet your
--              requirements, or that the operation of these designs will be
--              uninterrupted or error free, or that defects in the Designs
--              will be corrected. Furthermore, 4DSP does not warrant or
--              make any representations regarding use or the results of the
--              use of the designs in terms of correctness, accuracy,
--              reliability, or otherwise.
--
--              LIMITATION OF LIABILITY. In no event will 4DSP or its
--              licensors be liable for any loss of data, lost profits, cost
--              or procurement of substitute goods or services, or for any
--              special, incidental, consequential, or indirect damages
--              arising from the use or operation of the designs or
--              accompanying documentation, however caused and on any theory
--              of liability. This limitation will apply even if 4DSP
--              has been advised of the possibility of such damage. This
--              limitation shall apply not-withstanding the failure of the
--              essential purpose of any limited remedies herein.
--
----------------------------------------------

-- Library declarations
library ieee;
  use ieee.std_logic_unsigned.all;
  use ieee.std_logic_misc.all;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_1164.all;
library unisim;
  use unisim.vcomponents.all;
library work;

entity adc5000_top is
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
end adc5000_top;

architecture adc5000_top_syn of adc5000_top is

	----------------------------------------------------------------------------------------------------
	-- Constant declaration
	----------------------------------------------------------------------------------------------------
	--

	----------------------------------------------------------------------------------------------------
	--Signal declaration
	----------------------------------------------------------------------------------------------------
	signal phy_clk_a        : std_logic;
	signal phy_clk_b        : std_logic;
	signal phy_clk_c        : std_logic;
	signal phy_clk_d        : std_logic;
	signal v7_TestClks_s    : std_logic_vector(6 downto 0);
	signal ext_trigger_buf  : std_logic;
	signal clk_to_fpga_buf  : std_logic;
	signal phy_clk_s        : std_logic;
	signal FreCnt_rst_s     : std_logic;
	signal v5_TriggerDelay_s	: std_logic_vector(4 downto 0);
	signal ext_trigger_s		: std_logic;
	signal ext_trigger_delay_s		: std_logic;

	----------------------------------------------------------------------------------------------------
	--Component declaration
	----------------------------------------------------------------------------------------------------

begin

	phy_clk <= phy_clk_s;

	----------------------------------------------------------------------------------------------------
	-- LVDS Trigger Input
	----------------------------------------------------------------------------------------------------
	ibufds_trig : ibufds
	generic map (
		IOSTANDARD => "LVDS_25",
		DIFF_TERM => TRUE
	)
	port map (
		i => ext_trigger_p,
		ib => ext_trigger_n,
		o => ext_trigger_s
	);

	------------------------------------------------------------
	-- Trigger control
	------------------------------------------------------------
  Synchronizer_2Tv_Inst1 : entity work.Synchronizer_2Tv
    generic map (
      size => iv5_TriggerDelay_p'length
    )
    Port map (
      Clk           => phy_clk_s,
      Reset         => rst,
      Reset_value   => (others=>'0'),
      Async         => iv5_TriggerDelay_p,
      Sync          => v5_TriggerDelay_s
    );

  VariableDelayTrigger_INST: entity work.VariableDelayTrigger
    port map (
      i_clk_p     => phy_clk_s,
      iv5_Delay_p => v5_TriggerDelay_s,
      i_Trigger_p => ext_trigger_s,
      o_Trigger_p => ext_trigger_delay_s);

  triggerOut <= ext_trigger_delay_s;

  ----------------------------------------------------------------------------------------------------
  -- Physical Data Interface for ADC0
  ----------------------------------------------------------------------------------------------------
  ev10aq190_quad_phy_inst : entity work.ev10aq190_quad_phy
  port map (
    rst          => rst,
    REF_CLOCK    => REF_CLOCK,

    clk_regs      => clk_regs,
    cmd_in        => Phy_cmd_in,
    i_RstFifo_p   => i_RstFifo_p,
    
    iv5_adcIdelayValueA_p     => iv5_adcIdelayValueA_p,
    iv5_adcIdelayValueB_p     => iv5_adcIdelayValueB_p,
    iv5_adcIdelayValueC_p     => iv5_adcIdelayValueC_p,
    iv5_adcIdelayValueD_p     => iv5_adcIdelayValueD_p,
    iv10_adcIdelayValueA_we_p => iv10_adcIdelayValueA_we_p,
    iv10_adcIdelayValueB_we_p => iv10_adcIdelayValueB_we_p,
    iv10_adcIdelayValueC_we_p => iv10_adcIdelayValueC_we_p,
    iv10_adcIdelayValueD_we_p => iv10_adcIdelayValueD_we_p,
    iv10_bitslipA_p           => iv10_bitslipA_p,
    iv10_bitslipB_p           => iv10_bitslipB_p,
    iv10_bitslipC_p           => iv10_bitslipC_p,
    iv10_bitslipD_p           => iv10_bitslipD_p,
    
    ov10_calibErrorChA_p => ov10_calibErrorChA_p,
    ov10_calibErrorChB_p => ov10_calibErrorChB_p,
    ov10_calibErrorChC_p => ov10_calibErrorChC_p,
    ov10_calibErrorChD_p => ov10_calibErrorChD_p,

    sync_p       => sync_from_fpga_p,
    sync_n       => sync_from_fpga_n,

    adr_p        => adr_p,
    adr_n        => adr_n,
    ad_p         => ad_p,
    ad_n         => ad_n,
    aor_p        => aor_p,
    aor_n        => aor_n,

    bdr_p        => bdr_p,
    bdr_n        => bdr_n,
    bd_p         => bd_p,
    bd_n         => bd_n,
    bor_p        => bor_p,
    bor_n        => bor_n,

    cdr_p        => cdr_p,
    cdr_n        => cdr_n,
    cd_p         => cd_p,
    cd_n         => cd_n,
    cor_p        => cor_p,
    cor_n        => cor_n,

    ddr_p        => ddr_p,
    ddr_n        => ddr_n,
    dd_p         => dd_p,
    dd_n         => dd_n,
    dor_p        => dor_p,
    dor_n        => dor_n,

    phy_clk      => phy_clk_s,
    phy_data_a   => phy_data_a,
    phy_data_b   => phy_data_b,
    phy_data_c   => phy_data_c,
    phy_data_d   => phy_data_d,
    ov4_Otr_p => ov4_Otr_p,

    phy_clk_a    => phy_clk_a,
    phy_clk_b    => phy_clk_b,
    phy_clk_c    => phy_clk_c,
    phy_clk_d    => phy_clk_d

  );

  ----------------------------------------------------------------------------------------------------
  -- Frequency counter
  ----------------------------------------------------------------------------------------------------
  FreCnt_rst_s <= rst or FeqCnt_Cnt_rst;

  v7_TestClks_s(0) <= REF_CLOCK; -- 200MHz ref clk
  v7_TestClks_s(1) <= phy_clk_a;
  v7_TestClks_s(2) <= phy_clk_b;
  v7_TestClks_s(3) <= phy_clk_c;
  v7_TestClks_s(4) <= phy_clk_d;
  v7_TestClks_s(5) <= '0';
  v7_TestClks_s(6) <= clk_to_fpga_buf;

  FrequencyCounterMux_inst : entity work.FrequencyCounterMux
    generic map(
      C_REFCLK_FREQ_MHZ  => 100,                  -- AXI 100MHz
      C_NUM_OF_TEST_CLK  => 7,
      C_SEL_WIDTH        => 3
    )
    port map(
      i_Rst_p          => FreCnt_rst_s,
      i_RefClk_p       => clk_regs,
      iv_TestClks_p    => v7_TestClks_s,                -- Freqs to be measured
      iv_TestClkSel_p  => FeqCnt_clk_sel(2 downto 0),   -- select the clock to measure
      ov16_Freq_p      => FeqCnt_clk_cnt                -- measured freq value in MHz
    );

  ----------------------------------------------------------------------------------------------------
  -- Clock input
  ----------------------------------------------------------------------------------------------------
  ibufgds_ref_clk : ibufgds
  generic map (
    IOSTANDARD => "LVDS_25",
    DIFF_TERM => TRUE
  )
  port map (
    i  => clk_to_fpga_p,
    ib => clk_to_fpga_n,
    o  => clk_to_fpga_buf
  );

  ----------------------------------------------------------------------------------------------------
  -- Front IO
  ----------------------------------------------------------------------------------------------------
  --front_io_fmc <= (others => 'Z');

  ----------------------------------------------------------------------------------------------------
  -- CPLD Bus
  ----------------------------------------------------------------------------------------------------
  -- The CPLD bus is mapped to the CPLD on the FMC12x and is reserved for future use. These signals
  -- are not used by the CPLD on the FMC12x.
  ----------------------------------------------------------------------------------------------------
  --fmc_to_cpld <= (others => 'Z');

----------------------------------------------------------------------------------------------------
-- End
----------------------------------------------------------------------------------------------------
end adc5000_top_syn;
