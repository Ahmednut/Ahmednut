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
-- File        : $Id: spi_radio420x.vhd,v 1.1 2015/04/10 19:48:36 sqa Exp $
--------------------------------------------------------------------------------
-- Description : SPI cores for the Radio420x
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: spi_radio420x.vhd,v $
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.3  2013/11/08 16:52:08  julien.roy
-- Change Radio420 clock architecture.
--
-- Revision 1.2  2013/01/18 19:03:46  julien.roy
-- Merge changes from ZedBoard reference design to Perseus
--
-- Revision 1.1  2012/09/28 19:31:26  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.3  2011/10/12 20:44:22  lp.lessard
-- slow the LMS6002 SPI clock down. Same speed that was working before
--
-- Revision 1.2  2011/10/11 20:27:31  patrick.gilbert
-- update speed of the lime spi chain at 50MHz
--
-- Revision 1.1  2011/06/15 21:12:51  jeffrey.johnson
-- Changed core name.
--
-- Revision 1.1  2011/05/27 13:37:57  patrick.gilbert
-- first commit: revA
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library lyt_axi_radio420x_v1_00_a;
 use lyt_axi_radio420x_v1_00_a.all;

library unisim;
  use unisim.vcomponents.all;

entity spi_radio420x is
  port (
    i_clk_p           : in std_logic;
    i_rst_p           : in std_logic;
    iv5_FpgaCtrlEn_p    : in std_logic_vector(4 downto 0);

    iv16_refDacSpiData_p       : in  std_logic_vector(15 downto 0);
    i_refDacSpiStart_p         : in  std_logic;
    iv16_HostRefDacSpiData_p   : in  std_logic_vector(15 downto 0);
    i_HostRefDacSpiStart_p     : in  std_logic;
    o_refDacSpiBusy_p          : out std_logic;

    iv16_limeSpiData_p         : in  std_logic_vector(15 downto 0);
    i_limeSpiStart_p           : in std_logic;
    iv16_HostLimeSpiData_p     : in  std_logic_vector(15 downto 0);
    ov16_HostLimeSpiData_p     : out std_logic_vector(15 downto 0);
    i_HostLimeSpiStart_p       : in std_logic;
    o_limeSpiBusy_p            : out std_logic;

    iv6_rxGainSpiData_p        : in std_logic_vector(5 downto 0);
    iv3_rxGainSpiStart_p       : in std_logic_vector(2 downto 0);
    iv6_HostRxGainSpiData_p    : in std_logic_vector(5 downto 0);
    iv3_HostRxGainSpiStart_p   : in std_logic_vector(2 downto 0);
    o_rxGainSpiBusy_p          : out std_logic;

    iv6_txGainSpiData_p        : in std_logic_vector(5 downto 0);
    i_txGainSpiStart_p         : in std_logic;
    iv6_HostTxGainSpiData_p    : in std_logic_vector(5 downto 0);
    i_HostTxGainSpiStart_p     : in std_logic;
    o_txGainSpiBusy_p          : out std_logic;

    iv32_pllCtrlSpiData_p      : in  std_logic_vector(31 downto 0);
    iv2_pllCtrlSpiStart_p      : in  std_logic_vector(1 downto 0);
    iv32_HostPllCtrlSpiData_p  : in  std_logic_vector(31 downto 0);
    ov32_HostPllCtrlSpiData_p  : out std_logic_vector(31 downto 0);
    iv2_HostPllCtrlSpiStart_p  : in  std_logic_vector(1 downto 0);
    o_pllCtrlSpiBusy_p         : out std_logic;

    o_limeSpiCs_p          : out std_logic;
    o_limeSpiClk_p         : out std_logic;
    o_limeSpiMosi_p        : out std_logic;
    i_limeSpiMiso_p        : in  std_logic;

    o_refDacSpiCs_p        : out std_logic;
    o_refDacSpiClk_p       : out std_logic;
    o_refDacSpiMosi_p      : out std_logic;

    ov2_pllCtrlSpiCs_p     : out std_logic_vector(1 downto 0);
    o_pllCtrlSpiClk_p      : out std_logic;
    o_pllCtrlSpiMosi_p     : out std_logic;
    i_pllCtrlSpiMiso_p     : in  std_logic;

    ov3_rxGainSpiCs_p      : out std_logic_vector(2 downto 0);
    o_rxGainSpiClk_p       : out std_logic;
    o_rxGainSpiMosi_p      : out std_logic;

    o_txGainSpiCs_p        : out std_logic;
    o_txGainSpiClk_p       : out std_logic;
    o_txGainSpiMosi_p      : out std_logic
  );
end entity spi_radio420x;

architecture arch of spi_radio420x is

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal v16_refDacSpiData_s       : std_logic_vector(15 downto 0);
  signal refDacSpiStart_s          : std_logic_vector(0 downto 0);
  signal v16_limeSpiData_s         : std_logic_vector(15 downto 0);
  signal limeSpiStart_s            : std_logic_vector(0 downto 0);
  signal v6_rxGainSpiData_s        : std_logic_vector(5 downto 0);
  signal v3_rxGainSpiStart_s       : std_logic_vector(2 downto 0);
  signal v6_txGainSpiData_s        : std_logic_vector(5 downto 0);
  signal txGainSpiStart_s          : std_logic_vector(0 downto 0);
  signal v32_pllCtrlSpiData_s      : std_logic_vector(31 downto 0);
  signal v2_pllCtrlSpiStart_s      : std_logic_vector(1 downto 0);

  signal refDacSpiStartD1_s        : std_logic;
  signal refDacSpiStartD2_s        : std_logic;
  signal limeSpiStartD1_s          : std_logic;
  signal limeSpiStartD2_s          : std_logic;
  signal v3_rxGainSpiStartD1_s     : std_logic_vector(2 downto 0);
  signal v3_rxGainSpiStartD2_s     : std_logic_vector(2 downto 0);
  signal txGainSpiStartD1_s        : std_logic;
  signal txGainSpiStartD2_s        : std_logic;
  signal v2_pllCtrlSpiStartD1_s    : std_logic_vector(1 downto 0);
  signal v2_pllCtrlSpiStartD2_s    : std_logic_vector(1 downto 0);

begin

  -------------------------------
  -- Double latch start signal from FPGA logic
  -------------------------------
  -- This doubling latching is used to avoid metastability because the phase relation between
  -- the FPGA fabric clock and the system clock is unknown
  doubleLatc_l: process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      refDacSpiStartD1_s     <= i_refDacSpiStart_p;
      refDacSpiStartD2_s     <= refDacSpiStartD1_s;
      limeSpiStartD1_s       <= i_limeSpiStart_p;
      limeSpiStartD2_s       <= limeSpiStartD1_s;
      v3_rxGainSpiStartD1_s  <= iv3_rxGainSpiStart_p;
      v3_rxGainSpiStartD2_s  <= v3_rxGainSpiStartD1_s;
      txGainSpiStartD1_s     <= i_txGainSpiStart_p;
      txGainSpiStartD2_s     <= txGainSpiStartD1_s;
      v2_pllCtrlSpiStartD1_s <= iv2_pllCtrlSpiStart_p;
      v2_pllCtrlSpiStartD2_s <= v2_pllCtrlSpiStartD1_s;
    end if;
  end process;

  -------------------------------
  -- Select between host or FPGA for SPI master
  -------------------------------
  v16_refDacSpiData_s   <= iv16_refDacSpiData_p    when iv5_fpgaCtrlEn_p(0) = '1' else  iv16_hostrefDacSpiData_p;
  refDacSpiStart_s(0)   <= refDacSpiStartD2_s      when iv5_fpgaCtrlEn_p(0) = '1' else  i_hostrefDacSpiStart_p;
  v16_limeSpiData_s     <= iv16_limeSpiData_p      when iv5_fpgaCtrlEn_p(1) = '1' else  iv16_hostlimeSpiData_p;
  limeSpiStart_s(0)     <= limeSpiStartD2_s        when iv5_fpgaCtrlEn_p(1) = '1' else  i_hostlimeSpiStart_p;
  v6_rxGainSpiData_s    <= iv6_rxGainSpiData_p     when iv5_fpgaCtrlEn_p(2) = '1' else  iv6_hostrxGainSpiData_p;
  v3_rxGainSpiStart_s   <= v3_rxGainSpiStartD2_s   when iv5_fpgaCtrlEn_p(2) = '1' else  iv3_hostrxGainSpiStart_p;
  v6_txGainSpiData_s    <= iv6_txGainSpiData_p     when iv5_fpgaCtrlEn_p(3) = '1' else  iv6_hosttxGainSpiData_p;
  txGainSpiStart_s(0)   <= txGainSpiStartD2_s      when iv5_fpgaCtrlEn_p(3) = '1' else  i_hosttxGainSpiStart_p;
  v32_pllCtrlSpiData_s  <= iv32_pllCtrlSpiData_p   when iv5_fpgaCtrlEn_p(4) = '1' else  iv32_hostpllCtrlSpiData_p;
  v2_pllCtrlSpiStart_s  <= v2_pllCtrlSpiStartD2_s  when iv5_fpgaCtrlEn_p(4) = '1' else  iv2_hostpllCtrlSpiStart_p;

  -------------------------------
  -- Instanciate SPI registers
  -------------------------------
  
  pllCtrlReg_l: entity lyt_axi_radio420x_v1_00_a.spi_register --20MHz Max
    generic map(
      SPI_NUMBER_CS => 2,
      SPI_DATA_WIDTH  => 32,
      SPI_CLK_DOWNSAMPLING  => 20
    )
    port map (
      -- SPI Interface
      i_clk_p            => i_clk_p,
      i_rst_p            => i_rst_p,
      iv_spiStart_p      => v2_pllCtrlSpiStart_s,
      i_spiClkPol_p      => '1',
      i_spiMsbf_p        => '0',
      iv_spiDataIn_p     => v32_PllCtrlSpiData_s,
      ov_spiDataOut_p    => ov32_hostPllCtrlSpiData_p,
      o_spiBusy_p        => o_pllCtrlSpiBusy_p,

      o_spiClkOutPin_p   => o_pllCtrlSpiClk_p,
      ov_spiCsPin_p      => ov2_pllCtrlSpiCs_p,
      o_spiMosiPin_p     => o_pllCtrlSpiMosi_p,
      i_spiMisoPin_p     => i_pllCtrlSpiMiso_p
    );

  txGainReg_l: entity lyt_axi_radio420x_v1_00_a.spi_register   --25MHz Max
    generic map(
      SPI_NUMBER_CS => 1,
      SPI_DATA_WIDTH  => 6,
      SPI_CLK_DOWNSAMPLING  => 10
    )
    port map (
      -- SPI Interface
      i_clk_p            => i_clk_p,
      i_rst_p            => i_rst_p,
      iv_spiStart_p      => txGainSpiStart_s,
      i_spiClkPol_p      => '1',
      i_spiMsbf_p        => '1',
      iv_spiDataIn_p     => v6_txGainSpiData_s,
      ov_spiDataOut_p    => open,
      o_spiBusy_p        => o_txGainSpiBusy_p,

      o_spiClkOutPin_p   => o_txGainSpiClk_p,
      ov_spiCsPin_p(0)   => o_txGainSpiCs_p,
      o_spiMosiPin_p     => o_txGainSpiMosi_p,
      i_spiMisoPin_p     => '0'
    );

  rxGainReg_l: entity lyt_axi_radio420x_v1_00_a.spi_register   --25MHz Max
    generic map(
      SPI_NUMBER_CS => 3,
      SPI_DATA_WIDTH  => 6,
      SPI_CLK_DOWNSAMPLING  => 10
    )
    port map (
      -- SPI Interface
      i_clk_p            => i_clk_p,
      i_rst_p            => i_rst_p,
      iv_spiStart_p       => v3_rxGainSpiStart_s,
      i_spiClkPol_p      => '1',
      i_spiMsbf_p        => '1',
      iv_spiDataIn_p     => v6_rxGainSpiData_s,
      ov_spiDataOut_p    => open,
      o_spiBusy_p        => o_rxGainSpiBusy_p,

      o_spiClkOutPin_p   => o_rxGainSpiClk_p,
      ov_spiCsPin_p      => ov3_rxGainSpiCs_p,
      o_spiMosiPin_p     => o_rxGainSpiMosi_p,
      i_spiMisoPin_p     => '0'
    );

  LimeReg_l: entity lyt_axi_radio420x_v1_00_a.spi_register    --50MHz Max
    generic map(
      SPI_NUMBER_CS => 1,
      SPI_DATA_WIDTH  => 16,
      SPI_CLK_DOWNSAMPLING  => 10
    )
    port map (
      -- SPI Interface
      i_clk_p            => i_clk_p,
      i_rst_p            => i_rst_p,
      iv_spiStart_p      => limeSpiStart_s,
      i_spiClkPol_p      => '1',
      i_spiMsbf_p        => '1',
      iv_spiDataIn_p     => v16_limeSpiData_s,
      ov_spiDataOut_p    => ov16_HostlimeSpiData_p,
      o_spiBusy_p        => o_limeSpiBusy_p,

      o_spiClkOutPin_p   => o_limeSpiClk_p,
      ov_spiCsPin_p(0)   => o_limeSpiCs_p,
      o_spiMosiPin_p     => o_limeSpiMosi_p,
      i_spiMisoPin_p     => i_limeSpiMiso_p
    );

  refDacReg_l: entity lyt_axi_radio420x_v1_00_a.spi_register   --50MHz Max
    generic map(
      SPI_NUMBER_CS => 1,
      SPI_DATA_WIDTH  => 16,
      SPI_CLK_DOWNSAMPLING  => 10
    )
    port map (
      -- SPI Interface
      i_clk_p            => i_clk_p,
      i_rst_p            => i_rst_p,
      iv_spiStart_p      => refDacSpiStart_s,
      i_spiClkPol_p      => '1',
      i_spiMsbf_p        => '1',
      iv_spiDataIn_p     => v16_refDacSpiData_s,
      ov_spiDataOut_p    => open,
      o_spiBusy_p        => o_refDacSpiBusy_p,

      o_spiClkOutPin_p   => o_refDacSpiClk_p,
      ov_spiCsPin_p(0)   => o_refDacSpiCs_p,
      o_spiMosiPin_p     => o_refDacSpiMosi_p,
      i_spiMisoPin_p     => '0'
    );

end architecture;
