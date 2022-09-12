-------------------------------------------------------------------------------------
-- FILE NAME : ev10aq190_quad_phy.vhd
--
-- AUTHOR    : Peter Kortekaas
--
-- COMPANY   : 4DSP
--
-- ITEM      : 1
--
-- UNITS     : Entity       - ev10aq190_quad_phy
--             architecture - ev10aq190_quad_phy_syn
--
-- LANGUAGE  : VHDL
--
-------------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------------
-- DESCRIPTION
-- ===========
--
-- ev10aq190_quad_phy
-- Notes: ev10aq190_quad_phy
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
  use ieee.std_logic_1164.all;
  use ieee.std_logic_unsigned.all;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_misc.all;
  use ieee.numeric_std.all;
library unisim;
  use unisim.vcomponents.all;

entity ev10aq190_quad_phy is
  port (
    -- Global signals
    rst          : in  std_logic;
    REF_CLOCK    : in    std_logic;                           -- Ref clk for IDELAYCTRL

    --- Registers Interface ---
    clk_regs      : in  std_logic;                       -- system/Microblaze clock that registers are synced to it
    cmd_in        : in std_logic_vector(3 downto 0);
    i_RstFifo_p   : in std_logic;                          -- reset signal for fifo

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

    -- Sync signal to ADC
    sync_p       : out std_logic;
    sync_n       : out std_logic;

    -- Channel A: DDR LVDS Interface
    adr_p        : in  std_logic;
    adr_n        : in  std_logic;
    ad_p         : in  std_logic_vector(9 downto 0);
    ad_n         : in  std_logic_vector(9 downto 0);
    aor_p        : in  std_logic;
    aor_n        : in  std_logic;

    -- Channel B: DDR LVDS Interface
    bdr_p        : in  std_logic;
    bdr_n        : in  std_logic;
    bd_p         : in  std_logic_vector(9 downto 0);
    bd_n         : in  std_logic_vector(9 downto 0);
    bor_p        : in  std_logic;
    bor_n        : in  std_logic;

    -- Channel C: DDR LVDS Interface
    cdr_p        : in  std_logic;
    cdr_n        : in  std_logic;
    cd_p         : in  std_logic_vector(9 downto 0);
    cd_n         : in  std_logic_vector(9 downto 0);
    cor_p        : in  std_logic;
    cor_n        : in  std_logic;

    -- Channel D: DDR LVDS Interface
    ddr_p        : in  std_logic;
    ddr_n        : in  std_logic;
    dd_p         : in  std_logic_vector(9 downto 0);
    dd_n         : in  std_logic_vector(9 downto 0);
    dor_p        : in  std_logic;
    dor_n        : in  std_logic;

    -- Output data ports synchronous to one clock
    phy_clk      : out std_logic;                      -- clock 1/8 of the sample frequecy
    phy_data_a   : out std_logic_vector(127 downto 0); -- 8 samples in parallel in 16-bit format
    phy_data_b   : out std_logic_vector(127 downto 0); -- 8 samples in parallel in 16-bit format
    phy_data_c   : out std_logic_vector(127 downto 0); -- 8 samples in parallel in 16-bit format
    phy_data_d   : out std_logic_vector(127 downto 0); -- 8 samples in parallel in 16-bit format
    ov4_Otr_p : out std_logic_vector(3 downto 0);

    -- Output clocks, for frequency monitoring
    phy_clk_a    : out std_logic;                      -- clock 1/8 of the sample frequecy
    phy_clk_b    : out std_logic;                      -- clock 1/8 of the sample frequecy
    phy_clk_c    : out std_logic;                      -- clock 1/8 of the sample frequecy
    phy_clk_d    : out std_logic                       -- clock 1/8 of the sample frequecy

  );
end ev10aq190_quad_phy;

architecture ev10aq190_quad_phy_syn of ev10aq190_quad_phy is

----------------------------------------------------------------------------------------------------
-- Components
----------------------------------------------------------------------------------------------------
component adc5000_fifo_d16_w80 IS
	port (
	rst: IN std_logic;
	wr_clk: IN std_logic;
	rd_clk: IN std_logic;
	din: IN std_logic_VECTOR(79 downto 0);
	wr_en: IN std_logic;
	rd_en: IN std_logic;
	dout: OUT std_logic_VECTOR(79 downto 0);
	full: OUT std_logic;
	empty: OUT std_logic);
end component;

----------------------------------------------------------------------------------------------------
-- Constants
----------------------------------------------------------------------------------------------------
--

----------------------------------------------------------------------------------------------------
-- Signals
----------------------------------------------------------------------------------------------------
signal out_reg_val    : std_logic;
signal out_reg_addr   : std_logic_vector(27 downto 0);
signal out_reg        : std_logic_vector(31 downto 0);

signal in_reg_req     : std_logic;
signal in_reg_addr    : std_logic_vector(27 downto 0);
signal in_reg_val     : std_logic;
signal in_reg         : std_logic_vector(31 downto 0);

signal sync_cmd       : std_logic;
signal sync_pls       : std_logic;

signal delay_reset    : std_logic;
signal clk_reset      : std_logic;
signal io_reset       : std_logic;
signal rstFifoR1_s    : std_logic;
signal rstFifoR2_s    : std_logic;

signal in0_p          : std_logic_vector(9 downto 0);
signal in0_n          : std_logic_vector(9 downto 0);
signal out0           : std_logic_vector(79 downto 0);

signal in1_p          : std_logic_vector(9 downto 0);
signal in1_n          : std_logic_vector(9 downto 0);
signal out1           : std_logic_vector(79 downto 0);

signal in2_p          : std_logic_vector(9 downto 0);
signal in2_n          : std_logic_vector(9 downto 0);
signal out2           : std_logic_vector(79 downto 0);

signal in3_p          : std_logic_vector(9 downto 0);
signal in3_n          : std_logic_vector(9 downto 0);
signal out3           : std_logic_vector(79 downto 0);

signal adr            : std_logic;
signal ad             : std_logic_vector(79 downto 0);
signal aor            : std_logic_vector(7 downto 0);

signal bdr            : std_logic;
signal bd             : std_logic_vector(79 downto 0);
signal bor            : std_logic_vector(7 downto 0);

signal cdr            : std_logic;
signal cd             : std_logic_vector(79 downto 0);
signal cor            : std_logic_vector(7 downto 0);

signal ddr            : std_logic;
signal dd             : std_logic_vector(79 downto 0);
signal dor            : std_logic_vector(7 downto 0);

signal ad_8x11        : std_logic_vector(79 downto 0);
signal bd_8x11        : std_logic_vector(79 downto 0);
signal cd_8x11        : std_logic_vector(79 downto 0);
signal dd_8x11        : std_logic_vector(79 downto 0);

signal ad_8x10_o      : std_logic_vector(79 downto 0);
signal bd_8x10_o      : std_logic_vector(79 downto 0);
signal cd_8x10_o      : std_logic_vector(79 downto 0);
signal dd_8x10_o      : std_logic_vector(79 downto 0);

signal v80_Ch0_SerdesDout_s,
       v80_Ch1_SerdesDout_s,
       v80_Ch2_SerdesDout_s,
       v80_Ch3_SerdesDout_s : std_logic_vector(79 downto 0);

signal v80_Ch0_FifoDout_s,
       v80_Ch1_FifoDout_s,
       v80_Ch2_FifoDout_s,
       v80_Ch3_FifoDout_s : std_logic_vector(79 downto 0);

signal v80_Ch0_data_s,
       v80_Ch1_data_s,
       v80_Ch2_data_s,
       v80_Ch3_data_s : std_logic_vector(79 downto 0);

signal Ch1_OvrNotFiltred_s,
       Ch2_OvrNotFiltred_s,
       Ch3_OvrNotFiltred_s,
       Ch4_OvrNotFiltred_s : std_logic;


signal data_clk_s : std_logic;

signal a_fifo_wen,
       b_fifo_wen,
       c_fifo_wen,
       d_fifo_wen : std_logic;

signal a_fifo_ren,
       b_fifo_ren,
       c_fifo_ren,
       d_fifo_ren : std_logic;

signal a_fifo_full,
       b_fifo_full,
       c_fifo_full,
       d_fifo_full : std_logic;

signal a_fifo_empty,
       b_fifo_empty,
       c_fifo_empty,
       d_fifo_empty : std_logic;

  signal rstFifoA_s : std_logic;
  signal rstFifoB_s : std_logic;
  signal rstFifoC_s : std_logic;
  signal rstFifoD_s : std_logic;

  attribute keep : string;
  attribute keep of rstFifoA_s : signal is "true";
  attribute keep of rstFifoB_s : signal is "true";
  attribute keep of rstFifoC_s : signal is "true";
  attribute keep of rstFifoD_s : signal is "true";

begin

----------------------------------------------------------------------------------------------------
-- Map commands
----------------------------------------------------------------------------------------------------
delay_reset     <= cmd_in(0) or rst;
clk_reset       <= cmd_in(1) or rst;
io_reset        <= cmd_in(2) or rst;
sync_cmd        <= cmd_in(3);

----------------------------------------------------------------------------------------------------
-- Sync signal
----------------------------------------------------------------------------------------------------
sync_pls <= sync_cmd;

sync_out : obufds_lvdsext_25
port map (
  i  => sync_pls,
  o  => sync_p,
  ob => sync_n
);

----------------------------------------------------------------------------------------------------
-- Serdes input mapping (4 clock domains/regions)
----------------------------------------------------------------------------------------------------
in0_p <= ad_p;
in0_n <= ad_n;

in1_p <= bd_p;
in1_n <= bd_n;

in2_p <= cd_p;
in2_n <= cd_n;

in3_p <= dd_p;
in3_n <= dd_n;

----------------------------------------------------------------------------------------------------
-- Channel A
----------------------------------------------------------------------------------------------------
serdes_v6_inst0 : entity work.serdes_v6
generic map (
--   SYS_W               => 11,
--   DEV_W               => 88
  -- ORs not supported for the moment, not on the same FPGA bank as data and clk. 10 data bits per channel
  SYS_W               => 10,
  DEV_W               => 80
)
port map (
  data_in_from_pins_p => in0_p,
  data_in_from_pins_n => in0_n,
  data_in_to_device   => v80_Ch0_SerdesDout_s,

  i_rst_cmd_p               => rst,
  i_clk_cmd_p               => clk_regs,
  i_clk_reset_p             => clk_reset,
  i_io_reset_p              => io_reset,
  i_adcIdelayValue_rst_p    => delay_reset,
  iv5_adcIdelayValue_p      => iv5_adcIdelayValueA_p,
  iv_adcIdelayValue_we_p    => iv10_adcIdelayValueA_we_p,
  iv_bitslip_p              => iv10_bitslipA_p,
  clk_in_p                  => adr_p,
  clk_in_n                  => adr_n,
  clk_div_out               => adr,
  REF_CLOCK                 => REF_CLOCK
);

----------------------------------------------------------------------------------------------------
-- Channel B
----------------------------------------------------------------------------------------------------
serdes_v6_inst1 : entity work.serdes_v6
generic map (
--   SYS_W               => 11,
--   DEV_W               => 88
  -- ORs not supported for the moment, not on the same FPGA bank as data and clk. 10 data bits per channel
  SYS_W               => 10,
  DEV_W               => 80
)
port map (
  data_in_from_pins_p => in1_p,
  data_in_from_pins_n => in1_n,
  data_in_to_device   => v80_Ch1_SerdesDout_s,

  i_rst_cmd_p               => rst,
  i_clk_cmd_p               => clk_regs,
  i_clk_reset_p             => clk_reset,
  i_io_reset_p              => io_reset,
  i_adcIdelayValue_rst_p    => delay_reset,
  iv5_adcIdelayValue_p      => iv5_adcIdelayValueB_p,
  iv_adcIdelayValue_we_p    => iv10_adcIdelayValueB_we_p,
  iv_bitslip_p              => iv10_bitslipB_p,
  clk_in_p                  => bdr_p,
  clk_in_n                  => bdr_n,
  clk_div_out               => bdr,
  REF_CLOCK                 => REF_CLOCK
);

----------------------------------------------------------------------------------------------------
-- Channel C
----------------------------------------------------------------------------------------------------
serdes_v6_inst2 : entity work.serdes_v6
generic map (
--   SYS_W               => 11,
--   DEV_W               => 88
  -- ORs not supported for the moment, not on the same FPGA bank as data and clk. 10 data bits per channel
  SYS_W               => 10,
  DEV_W               => 80
)
port map (
  data_in_from_pins_p => in2_p,
  data_in_from_pins_n => in2_n,
  data_in_to_device   => v80_Ch2_SerdesDout_s,

  i_rst_cmd_p               => rst,
  i_clk_cmd_p               => clk_regs,
  i_clk_reset_p             => clk_reset,
  i_io_reset_p              => io_reset,
  i_adcIdelayValue_rst_p    => delay_reset,
  iv5_adcIdelayValue_p      => iv5_adcIdelayValueC_p,
  iv_adcIdelayValue_we_p    => iv10_adcIdelayValueC_we_p,
  iv_bitslip_p              => iv10_bitslipC_p,
  clk_in_p                  => cdr_p,
  clk_in_n                  => cdr_n,
  clk_div_out               => cdr,
  REF_CLOCK                 => REF_CLOCK
);

----------------------------------------------------------------------------------------------------
-- Channel D
----------------------------------------------------------------------------------------------------
serdes_v6_inst3 : entity work.serdes_v6
generic map (
--   SYS_W               => 11,
--   DEV_W               => 88
  -- ORs not supported for the moment, not on the same FPGA bank as data and clk. 10 data bits per channel
  SYS_W               => 10,
  DEV_W               => 80
)
port map (
  data_in_from_pins_p => in3_p,
  data_in_from_pins_n => in3_n,
  data_in_to_device   => v80_Ch3_SerdesDout_s,

  i_rst_cmd_p               => rst,
  i_clk_cmd_p               => clk_regs,
  i_clk_reset_p             => clk_reset,
  i_io_reset_p              => io_reset,
  i_adcIdelayValue_rst_p    => delay_reset,
  iv5_adcIdelayValue_p      => iv5_adcIdelayValueD_p,
  iv_adcIdelayValue_we_p    => iv10_adcIdelayValueD_we_p,
  iv_bitslip_p              => iv10_bitslipD_p,
  clk_in_p                  => ddr_p,
  clk_in_n                  => ddr_n,
  clk_div_out               => ddr,
  REF_CLOCK                 => REF_CLOCK
);

----------------------------------------------------------------------------------------------------
-- Out of Range implementation
-- This is because we couldn't use the OTR that come from the ADC chip because the reside
-- on another FPGA bank than the channel data and clock. That causes timig errors.
----------------------------------------------------------------------------------------------------
ADCOutOfRange_int0 : entity work.ADCOutOfRange
  generic map (
     FilterLength => 2)
  port map(
    i_rst_p                 => io_reset,
    i_clkCh1_p              => data_clk_s,
    i_clkCh2_p              => data_clk_s,
    i_clkCh3_p              => data_clk_s,
    i_clkCh4_p              => data_clk_s,
    iv80_dataCh1_p          => v80_Ch0_FifoDout_s,
    iv80_dataCh2_p          => v80_Ch1_FifoDout_s,
    iv80_dataCh3_p          => v80_Ch2_FifoDout_s,
    iv80_dataCh4_p          => v80_Ch3_FifoDout_s,
    ov80_dataCh1_p          => v80_Ch0_data_s,
    ov80_dataCh2_p          => v80_Ch1_data_s,
    ov80_dataCh3_p          => v80_Ch2_data_s,
    ov80_dataCh4_p          => v80_Ch3_data_s,
    o_Ch1_OvrNotFiltred_p => Ch1_OvrNotFiltred_s,
    o_Ch2_OvrNotFiltred_p => Ch2_OvrNotFiltred_s,
    o_Ch3_OvrNotFiltred_p => Ch3_OvrNotFiltred_s,
    o_Ch4_OvrNotFiltred_p => Ch4_OvrNotFiltred_s,
    o_Ch1_OvrFiltred_p    => open,
    o_Ch2_OvrFiltred_p    => open,
    o_Ch3_OvrFiltred_p    => open,
    o_Ch4_OvrFiltred_p    => open
  );

-- Output to drive LED
 ov4_Otr_p(0) <= Ch1_OvrNotFiltred_s;
 ov4_Otr_p(1) <= Ch2_OvrNotFiltred_s;
 ov4_Otr_p(2) <= Ch3_OvrNotFiltred_s;
 ov4_Otr_p(3) <= Ch4_OvrNotFiltred_s;

----------------------------------------------------------------------------------------------------
-- Calibration error detection
----------------------------------------------------------------------------------------------------
calibration_ramp_detection_inst : entity work.calibration_ramp_detection
  port map(
    i_clk_p                 => data_clk_s,
    iv80_dataChA_p          => v80_Ch0_FifoDout_s,
    iv80_dataChB_p          => v80_Ch1_FifoDout_s,
    iv80_dataChC_p          => v80_Ch2_FifoDout_s,
    iv80_dataChD_p          => v80_Ch3_FifoDout_s,
    ov10_errorChA_p         => ov10_calibErrorChA_p,
    ov10_errorChB_p         => ov10_calibErrorChB_p,
    ov10_errorChC_p         => ov10_calibErrorChC_p,
    ov10_errorChD_p         => ov10_calibErrorChD_p
  );

----------------------------------------------------------------------------------------------------
-- MMCM for user fifo clock
----------------------------------------------------------------------------------------------------

mmcm_inst: entity work.mmcm_v6
port map(
  CLK_IN1           => adr,
  CLK_OUT1          => data_clk_s,
  RESET             => clk_reset,
  LOCKED            => open
 );

----------------------------------------------------------------------------------------------------
-- FIFO's
----------------------------------------------------------------------------------------------------
process(data_clk_s)
begin
  if falling_edge(data_clk_s) then
    rstFifoR1_s <= i_RstFifo_p;
    rstFifoA_s <= rstFifoR1_s;
    rstFifoB_s <= rstFifoR1_s;
    rstFifoC_s <= rstFifoR1_s;
    rstFifoD_s <= rstFifoR1_s;
  end if;
end process;

ev10aq190_phy_fifo_a : adc5000_fifo_d16_w80
port map (
  rst           => rstFifoA_s,
  wr_clk        => adr,
  rd_clk        => data_clk_s,
  din           => v80_Ch0_SerdesDout_s,
  wr_en         => a_fifo_wen,
  rd_en         => a_fifo_ren,
  dout          => v80_Ch0_FifoDout_s,
  full          => a_fifo_full,
  empty         => a_fifo_empty
);
-- FIFO takes 3 wr_clk to exit reset state after reset goes back low, during this time
-- we shouln't wen/ren the fifo to preven glitches. The full flag goes high during the firo reset state,
-- we use it to peven that.
a_fifo_wen <= not(a_fifo_full);
a_fifo_ren  <= not(a_fifo_empty);

ev10aq190_phy_fifo_b : adc5000_fifo_d16_w80
port map (
  rst           => rstFifoB_s,
  wr_clk        => bdr,
  rd_clk        => data_clk_s,
  din           => v80_Ch1_SerdesDout_s,
  wr_en         => b_fifo_wen,
  rd_en         => b_fifo_ren,
  dout          => v80_Ch1_FifoDout_s,
  full          => b_fifo_full,
  empty         => b_fifo_empty
);

b_fifo_wen <= not(b_fifo_full);
b_fifo_ren  <= not(b_fifo_empty);

ev10aq190_phy_fifo_c : adc5000_fifo_d16_w80
port map (
  rst           => rstFifoC_s,
  wr_clk        => cdr,
  rd_clk        => data_clk_s,
  din           => v80_Ch2_SerdesDout_s,
  wr_en         => c_fifo_wen,
  rd_en         => c_fifo_ren,
  dout          => v80_Ch2_FifoDout_s,
  full          => c_fifo_full,
  empty         => c_fifo_empty
);

c_fifo_wen <= not(c_fifo_full);
c_fifo_ren  <= not(c_fifo_empty);

ev10aq190_phy_fifo_d : adc5000_fifo_d16_w80
port map (
  rst           => rstFifoD_s,
  wr_clk        => ddr,
  rd_clk        => data_clk_s,
  din           => v80_Ch3_SerdesDout_s,
  wr_en         => d_fifo_wen,
  rd_en         => d_fifo_ren,
  dout          => v80_Ch3_FifoDout_s,
  full          => d_fifo_full,
  empty         => d_fifo_empty
);

d_fifo_wen <= not(d_fifo_full);
d_fifo_ren  <= not(d_fifo_empty);

----------------------------------------------------------------------------------------------------
-- Connect entity
----------------------------------------------------------------------------------------------------
phy_clk    <= data_clk_s;
phy_clk_a  <= data_clk_s;
phy_clk_b  <= bdr;
phy_clk_c  <= cdr;
phy_clk_d  <= ddr;

--- Make 16-bit data sample out of 10-bit data by extending sign bit.
samples: for s in 0 to 7 generate

  or_bits: for b in 0 to 9 generate
    phy_data_a(s*16+b) <= v80_Ch0_data_s(s*10+b);
    phy_data_b(s*16+b) <= v80_Ch1_data_s(s*10+b);
    phy_data_c(s*16+b) <= v80_Ch2_data_s(s*10+b);
    phy_data_d(s*16+b) <= v80_Ch3_data_s(s*10+b);
  end generate;

 -- ADC data format = binary, so we zero fill-up for bits 10 to 15
 data_bits: for b in 10 to 15 generate
   phy_data_a(s*16+b) <= '0';
   phy_data_b(s*16+b) <= '0';
   phy_data_c(s*16+b) <= '0';
   phy_data_d(s*16+b) <= '0';
 end generate;

end generate;

----------------------------------------------------------------------------------------------------
-- end
----------------------------------------------------------------------------------------------------
end ev10aq190_quad_phy_syn;
