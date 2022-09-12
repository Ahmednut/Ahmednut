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
-- File        : $Id: radio420x_top.vhd,v 1.4 2015/07/21 13:50:16 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : This component is the top of the adac250 core
-- It includes ADC and DAC interfaces, over range verification,
-- calibration modules and SPI interface
--
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/09/30
-- $Log: radio420x_top.vhd,v $
-- Revision 1.4  2015/07/21 13:50:16  julien.roy
-- Force data valid to toggle very 2 clock cycles.
--
-- Revision 1.3  2015/07/09 20:08:15  christian.dufour-bou
-- corrected some tabs
--
-- Revision 1.2  2015/06/17 13:25:55  christian.dufour-bou
-- Core now features two separate IQ ports
--
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.7  2014/05/21 13:56:30  julien.roy
-- Reset ADC and DAC fifo when enabling the design clock
--
-- Revision 1.6  2013/11/08 16:52:08  julien.roy
-- Change Radio420 clock architecture.
--
-- Revision 1.5  2013/04/19 15:39:16  julien.roy
-- Add latch for DAC data going to the IO pin to ease timing
-- Add latches for ClkEn
--
-- Revision 1.4  2013/03/05 14:38:35  julien.roy
-- Change fifo rst counter from refClk_s to designClk_s
--
-- Revision 1.3  2013/02/28 20:16:34  jd.gagnon
-- Add ADC fifo reset
--
-- Revision 1.2  2013/01/18 19:03:45  julien.roy
-- Merge changes from ZedBoard reference design to Perseus
--
-- Revision 1.1  2012/09/28 19:31:26  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.10  2012/06/27 20:10:02  david.quinn
-- Support the new calibration method (DFT)
--
-- Revision 1.9  2011/12/21 18:54:06  jm.fortin
-- Mantis no.2296 (Radio420X gated clock issue)
--
-- Revision 1.8  2011/09/28 20:28:42  jeffrey.johnson
-- Added ports for monitoring the outputs to the DAC.
--
-- Revision 1.7  2011/09/21 17:43:35  jeffrey.johnson
-- Added reset for overrange outputs.
--
-- Revision 1.6  2011/09/19 20:42:35  jeffrey.johnson
-- Connected compensation block MUX output.
--
-- Revision 1.5  2011/09/19 13:27:22  jeffrey.johnson
-- Clocking flops with system clock.
--
-- Revision 1.4  2011/09/08 20:16:40  patrick.gilbert
-- change netlist name to follow naming standard.
--
-- Revision 1.3  2011/09/07 14:37:18  jeffrey.johnson
-- Added two-stage flip flops for clock domain crossing the external SPI control interface.
--
-- Revision 1.2  2011/08/30 20:00:17  patrick.gilbert
-- add calibration core inside FPGA : beta
--
-- Revision 1.1  2011/06/15 21:12:51  jeffrey.johnson
-- Changed core name.
--
-- Revision 1.2  2011/06/14 22:22:12  patrick.gilbert
-- add mimo modification...add generic to instantiate FMCclk0 BUFG. + add dac idelayctrl
--
-- Revision 1.1  2011/05/27 13:37:56  patrick.gilbert
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

entity radio420x_top is
  generic
  (
    C_FMCRADIO_CH2                : integer range 0 to 1:=0
  );
  port
  (
    i_systemClk_p              : in std_logic;
    i_RefClk200MHz_p           : in std_logic;
    i_rst_p                    : in std_logic;
    i_RstFifo_p             : in std_logic;

    --FMC mapping
    iv12_fmcAdcData_p          : in std_logic_vector (11 downto 0);
    i_fmcAdcIQSel_p            : in std_logic;
    i_fmcAdcClk_p              : in std_logic;
    ov12_fmcDacData_p          : out std_logic_vector (11 downto 0);
    o_fmcDacIQSel_p            : out std_logic;
    -- SPI pin mapping
    o_fmcLimeSpiCs_p              : out std_logic;
    o_fmcLimeSpiClk_p             : out std_logic;
    o_fmcLimeSpiMosi_p            : out std_logic;
    i_fmcLimeSpiMiso_p            : in  std_logic;

    o_fmcRefDacSpiCs_p            : out std_logic;
    o_fmcRefDacSpiClk_p           : out std_logic;
    o_fmcRefDacSpiMosi_p          : out std_logic;

    ov2_fmcPllCtrlSpiCs_p         : out std_logic_vector(1 downto 0);
    o_fmcPllCtrlSpiClk_p          : out std_logic;
    o_fmcPllCtrlSpiMosi_p         : out std_logic;
    i_fmcPllCtrlSpiMiso_p         : in  std_logic;

    ov3_fmcRxGainSpiCs_p          : out std_logic_vector(2 downto 0);
    o_fmcRxGainSpiClk_p           : out std_logic;
    o_fmcRxGainSpiMosi_p          : out std_logic;

    o_fmcTxGainSpiCs_p            : out std_logic;
    o_fmcTxGainSpiClk_p           : out std_logic;
    o_fmcTxGainSpiMosi_p          : out std_logic;

    i_fmcPllLock_p             : in std_logic;
    o_fmcLimeReset_p           : out std_logic;
    o_fmcTxEnable_p            : out std_logic;
    o_fmcRxEnable_p            : out std_logic;
    ov2_FmcClkMuxSin_p         : out std_logic_vector(1 downto 0);
    ov2_FmcClkMuxSout_p        : out std_logic_vector(1 downto 0);
    o_FmcClkMuxLoad_p          : out std_logic;
    o_FmcClkMuxConfig_p        : out std_logic;

    --To user logic
    o_designClk_p              : out std_logic;
    i_designClk_p              : in  std_logic;
    ov12_adcIData_p            : out std_logic_vector (11 downto 0);
    ov12_adcQData_p            : out std_logic_vector (11 downto 0);
    o_adcDataValid_p           : out std_logic;
    i_dacDataValid_p           : in  std_logic;
    iv12_dacIData_p            : in  std_logic_vector (11 downto 0);
    iv12_dacQData_p            : in  std_logic_vector (11 downto 0);
    o_dacReadEnable_p          : out std_logic;
    ov12_dacDataComp_p         : out std_logic_vector (11 downto 0);
    o_dacIQSelComp_p           : out std_logic;

    -- From/to control
    i_DesignClkEn_p            : in  std_logic;
    iv5_AdcIdelayValue_p       : in std_logic_vector(4 downto 0);
    iv5_AdcClkIdelayValue_p    : in std_logic_vector(4 downto 0);
    iv5_fpgaCtrlEn_p           : in  std_logic_vector(4 downto 0);
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

    o_fmcPllLock_p             : out std_logic;
    i_fmcLimeReset_p           : in std_logic;
    i_fmcTxEnable_p            : in std_logic;
    i_fmcRxEnable_p            : in std_logic;
    iv2_fmcClkMuxSin_p         : in std_logic_vector(1 downto 0);
    iv2_fmcClkMuxSout_p        : in std_logic_vector(1 downto 0);
    i_fmcClkMuxLoad_p          : in std_logic;
    i_fmcClkMuxConfig_p        : in std_logic;

    o_ovrAdcI_p                : out std_logic;
    o_ovrAdcQ_p                : out std_logic;
    o_ovrDacI_p                : out std_logic;
    o_ovrDacQ_p                : out std_logic;
    i_ResetOvr_p               : in std_logic;

    ov12_rxIDcLevel_p          : out std_logic_vector(11 downto 0);
    ov12_rxQDcLevel_p          : out std_logic_vector(11 downto 0);
    ov23_rxErf_p               : out std_logic_vector(22 downto 0);
    i_rxErrFctStart_p          : in std_logic;
    i_rxErrFctFreqWen_p        : in std_logic;
    o_rxErrFctDone_p           : out std_logic;
    iv14_rxErrFctNbPt_p        : in std_logic_vector(13 downto 0);
    iv24_rxErrFctFreq_p        : in std_logic_vector(23 downto 0);
    ov32_rxErrFctErrI_p        : out std_logic_vector(31 downto 0);
    ov32_rxErrFctErrQ_p        : out std_logic_vector(31 downto 0);

    iv13_phaseoffsetcos_p   : in std_logic_vector(12 downto 0);
    iv13_phaseoffsetsin_p   : in std_logic_vector(12 downto 0);
    iv13_backoff_p          : in std_logic_vector(12 downto 0);
    iv13_gainoffset_p       : in std_logic_vector(12 downto 0);
    iv24_dacOutFreq_p       : in std_logic_vector(23 downto 0);
    iv3_dacOutSel_p         : in std_logic_vector(2 downto 0)
  );
end entity radio420x_top;

architecture arch of radio420x_top is

  ----------------------------------------
  -- Component declaration
  ----------------------------------------

  -- Custom component build in matlab netlist to evaluate the
  -- error function and rx dc offset level.
  component radio420x_rxerrorfunc12b_netlist_cw  port (
      ce                     : in std_logic := '1';
      clk                    : in std_logic;
      i_freqvalid_p          : in std_logic;
      i_start_p              : in std_logic;
      iv12_datai_p           : in std_logic_vector(11 downto 0);
      iv12_dataq_p           : in std_logic_vector(11 downto 0);
      iv14_n_p               : in std_logic_vector(13 downto 0);
      iv_freq_p              : in std_logic_vector(23 downto 0);
      o_done_p               : out std_logic;
      ov12_dcleveli_p        : out std_logic_vector(11 downto 0);
      ov12_dclevelq_p        : out std_logic_vector(11 downto 0);
      ov32_erri_p            : out std_logic_vector(31 downto 0);
      ov32_errq_p            : out std_logic_vector(31 downto 0)
    );
  end component;

  -- Custom component build in matlab netlist to calibrate the
  -- TX IQ imbalance.
  component radio420x_iqcomp12b_netlist
  port (
      ce_1: in std_logic;
      clk_1: in std_logic;
      i_iqsel_p: in std_logic;
      iv12_iqdata_p: in std_logic_vector(11 downto 0);
      iv13p11_backoff_p: in std_logic_vector(12 downto 0);
      iv13p11_gainoffset_p: in std_logic_vector(12 downto 0);
      iv13p11_phaseoffsetcos_p: in std_logic_vector(12 downto 0);
      iv13p11_phaseoffsetsin_p: in std_logic_vector(12 downto 0);
      iv24_ddsfreq_p: in std_logic_vector(23 downto 0);
      iv3_dacoutsel_p: in std_logic_vector(2 downto 0);
      o_iqsel_p: out std_logic;
      ov12_iqdata_p: out std_logic_vector(11 downto 0)
    );
  end component;

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal refClk_s                : std_logic;
  signal designClk_s             : std_logic;

  signal adcIdata_s              : std_logic_vector(11 downto 0);
  signal adcQdata_s              : std_logic_vector(11 downto 0);
  signal iqSel_s                 : std_logic;
  signal adcDataValid_s          : std_logic;
  signal adc_data_s              : std_logic_vector(11 downto 0);
  signal adc_data_r1_s           : std_logic_vector(11 downto 0);

  signal dacIdata_s              : std_logic_vector(11 downto 0);
  signal dacQdata_s              : std_logic_vector(11 downto 0);
  signal v12_dacIData_latch_s    : std_logic_vector(11 downto 0);
  signal v12_dacQData_latch_s    : std_logic_vector(11 downto 0);
  signal v12_dacQData_latch_r1_s : std_logic_vector(11 downto 0);
  signal dacIQdata_s             : std_logic_vector(11 downto 0);
  signal dacIQsel_s              : std_logic;
  signal dacDataValidInput_1r_s  : std_logic;

  signal dacIQdataComp_s         : std_logic_vector(11 downto 0);
  signal dacIQselComp_s          : std_logic;
  signal v12_UserDacData_s       : std_logic_vector(11 downto 0);
  signal userDacIQSel_s          : std_logic;

  signal ResetOvr_s              : std_logic;

  -- SPI external interface busy signals
  signal refDacSpiBusy_s         : std_logic;
  signal limeSpiBusy_s           : std_logic;
  signal rxGainSpiBusy_s         : std_logic;
  signal txGainSpiBusy_s         : std_logic;
  signal pllCtrlSpiBusy_s        : std_logic;

  -- SPI external interface flops (first level)
  signal v16_refDacSpiData0_s    : std_logic_vector(15 downto 0);
  signal refDacSpiStart0_s       : std_logic;
  signal refDacSpiBusy0_s        : std_logic;
  signal v16_limeSpiData0_s      : std_logic_vector(15 downto 0);
  signal limeSpiStart0_s         : std_logic;
  signal limeSpiBusy0_s          : std_logic;
  signal v6_rxGainSpiData0_s     : std_logic_vector(5 downto 0);
  signal v3_rxGainSpiStart0_s    : std_logic_vector(2 downto 0);
  signal rxGainSpiBusy0_s        : std_logic;
  signal v6_txGainSpiData0_s     : std_logic_vector(5 downto 0);
  signal txGainSpiStart0_s       : std_logic;
  signal txGainSpiBusy0_s        : std_logic;
  signal v32_pllCtrlSpiData0_s   : std_logic_vector(31 downto 0);
  signal v2_pllCtrlSpiStart0_s   : std_logic_vector(1 downto 0);
  signal pllCtrlSpiBusy0_s       : std_logic;

  -- SPI external interface flops (second level)
  signal v16_refDacSpiData1_s    : std_logic_vector(15 downto 0);
  signal refDacSpiStart1_s       : std_logic;
  signal refDacSpiBusy1_s        : std_logic;
  signal v16_limeSpiData1_s      : std_logic_vector(15 downto 0);
  signal limeSpiStart1_s         : std_logic;
  signal limeSpiBusy1_s          : std_logic;
  signal v6_rxGainSpiData1_s     : std_logic_vector(5 downto 0);
  signal v3_rxGainSpiStart1_s    : std_logic_vector(2 downto 0);
  signal rxGainSpiBusy1_s        : std_logic;
  signal v6_txGainSpiData1_s     : std_logic_vector(5 downto 0);
  signal txGainSpiStart1_s       : std_logic;
  signal txGainSpiBusy1_s        : std_logic;
  signal v32_pllCtrlSpiData1_s   : std_logic_vector(31 downto 0);
  signal v2_pllCtrlSpiStart1_s   : std_logic_vector(1 downto 0);
  signal pllCtrlSpiBusy1_s       : std_logic;

  signal RstFifo_s              : std_logic := '1';

  signal designClkEn_r1          : std_logic;
  signal designClkEn_r2          : std_logic;

  signal adcClkBufr_s            : std_logic;
  signal adcClkBufg_s            : std_logic;

begin

  -- Reset for the overrange detectors
  ResetOvr_s <= i_rst_p or i_ResetOvr_p;

  -- ADC interface
  limeADC_l: entity lyt_axi_radio420x_v1_00_a.lime_adc_interface
    port map(
      i_RefClk200MHz_p   => i_RefClk200MHz_p,
      i_SystemClk_p      => i_SystemClk_p,
      i_Rst_p            => i_rst_p,
      i_RstFifo_p        => RstFifo_s,
      i_AdcDataClk_p     => i_fmcAdcClk_p,
      i_IQSel_p          => i_fmcAdcIQSel_p,
      iv12_ADCData_p     => iv12_fmcAdcData_p,
      o_IDelayRdy_p      => open,
      iv5_AdcIdelayValue_p      => iv5_AdcIdelayValue_p,
      iv5_AdcClkIdelayValue_p   => iv5_AdcClkIdelayValue_p,
      i_DesignClk_p      => designClk_s,
      ov12_userAdcData_p => adc_data_s,
      o_userAdcIQSel_p   => iqSel_s,
      o_AdcClkBufr_p     => adcClkBufr_s
    );

  o_adcDataValid_p <= adcDataValid_s;
  o_dacReadEnable_p <= adcDataValid_s;
  ov12_adcIData_p <= adcIData_s;
  ov12_adcQData_p <= adcQdata_s;

  -- Demux IQ ADC data
  ADCDemux_prc : process(designClk_s)
    variable iqSel_r1_s : std_logic;
  begin
    if rising_edge(designClk_s) then
      if i_rst_p ='1' then
        adcDataValid_s <= '0';
      else
        if ( iqSel_s = '1' and iqSel_r1_s = '0' ) then
		  adc_data_r1_s <= adc_data_s;
          adcDataValid_s <= '0';
        elsif  ( iqSel_s = '0' and iqSel_r1_s = '1' ) then
		  adcIData_s <= adc_data_r1_s;
          adcQdata_s <= adc_data_s;
		  adcDataValid_s <= '1';
        else
          -- Force adcDataValid_s to toggle every clock cycle
		  adcIData_s <= (others => '0');
          adcQdata_s <= (others => '0');
		  adcDataValid_s <= not adcDataValid_s;
        end if;
      end if;
      
      iqSel_r1_s := iqSel_s;
      
    end if;
  end process ADCDemux_prc;

  -- Mux IQ Dac data
  DACmux_prc : process(designClk_s)
  begin
    if rising_edge(designClk_s) then
      dacDataValidInput_1r_s <= i_dacDataValid_p;
    if i_dacDataValid_p = '1' then
      v12_dacIData_latch_s <= iv12_dacIData_p;
      v12_dacQData_latch_s <= iv12_dacQData_p;
    elsif (dacDataValidInput_1r_s = '0') then
      v12_dacIData_latch_s <= (others => '0');
      v12_dacQData_latch_s <= (others => '0');
    end if;

    if(adcDataValid_s = '1') then
      dacIQdata_s <= v12_dacIData_latch_s;
      dacIQsel_s <= '1';

      v12_dacQData_latch_r1_s <= v12_dacQData_latch_s;
    else
      dacIQdata_s <= v12_dacQData_latch_r1_s;
      dacIQsel_s <= '0';
    end if;

    end if;
  end process DACmux_prc;

  -- ADC over range verification
  ADCOutOfRange_inst : entity lyt_axi_radio420x_v1_00_a.DataOutOfRange
    generic map (
      FilterLength => 26 -- give near .5sec flag.
    )
    port map(
      i_clk_p                 => designClk_s,
      i_rst_p                 => ResetOvr_s,
      iv12_DataCh1_p          => adcIdata_s,
      iv12_DataCh2_p          => adcQdata_s,
      i_DataType_p            => '1',              -- 0 = offset binary output format / 1= 2s complement output format
      o_Ch1_OvrNotFiltred_p   => open,
      o_Ch1_OvrFiltred_p      => o_ovrAdcI_p,
      o_Ch2_OvrNotFiltred_p   => open,
      o_Ch2_OvrFiltred_p      => o_ovrAdcQ_p
    );

  -- Custom component to evaluate the error function and rx dc offset level
  ERFcalculator_l : radio420x_rxerrorfunc12b_netlist_cw
    port map (
      ce               => '1',
      clk              => designClk_s,
      i_freqvalid_p    => i_rxErrFctFreqWen_p,
      i_start_p        => i_rxErrFctStart_p,
      iv12_datai_p     => adcIdata_s,
      iv12_dataq_p     => adcQdata_s,
      iv14_n_p         => iv14_rxErrFctNbPt_p,
      iv_freq_p        => iv24_rxErrFctFreq_p,
      o_done_p         => o_rxErrFctDone_p,
      ov12_dcleveli_p  => ov12_rxIDcLevel_p,
      ov12_dclevelq_p  => ov12_rxQDcLevel_p,
      ov32_erri_p      => ov32_rxErrFctErrI_p,
      ov32_errq_p      => ov32_rxErrFctErrQ_p
    );

  ov23_rxErf_p <= (others => '0');

  -- DAC interface
  process (designClk_s)
  begin
    if rising_edge(designClk_s) then
      v12_UserDacData_s <= dacIQdataComp_s;
      userDacIQSel_s   <= dacIQselComp_s;
    end if;
  end process;

  limeDAC_l: entity lyt_axi_radio420x_v1_00_a.lime_dac_interface
    port map(
      i_DesignClk_p           => designClk_s,
      i_AdcClk_p              => adcClkBufg_s,
      i_RefClk200MHz_p        => i_RefClk200MHz_p,
      i_Rst_p                 => i_rst_p,
      i_RstFifo_p             => RstFifo_s,

      --User Data port
      iv12_UserDacData_p      => v12_UserDacData_s,
      i_UserDacIQSel_p        => userDacIQSel_s,

      -- Interface interface.
      ov12_DacData_p          => ov12_fmcDacData_p,
      o_DacIQSel_p            => o_fmcDacIQSel_p
    );

  -- Demux IQ DAC data
  process (designClk_s)
  begin
    if rising_edge(designClk_s) then
      if i_rst_p ='1' then
        dacIdata_s <= (others=>'0');
        dacQdata_s <= (others=>'0');
      else
        if( dacIQselComp_s ='1') then
          dacIdata_s <= dacIQdataComp_s;
        else
          dacQdata_s <= dacIQdataComp_s;
        end if;
       end if;
    end if;
  end process;

  -- DAC over range verification
  DACOutOfRange_inst : entity lyt_axi_radio420x_v1_00_a.DataOutOfRange
    generic map (
      FilterLength => 26)   --give .5 sec flag
    port map(
      i_clk_p                 => designClk_s,
      i_rst_p                 => ResetOvr_s,
      iv12_DataCh1_p          => dacIdata_s,
      iv12_DataCh2_p          => dacQdata_s,
      i_DataType_p            => '1',              -- 0 = offset binary output format / 1= 2s complement output format
      o_Ch1_OvrNotFiltred_p   => open,
      o_Ch1_OvrFiltred_p      => o_ovrDacI_p,
      o_Ch2_OvrNotFiltred_p   => open,
      o_Ch2_OvrFiltred_p      => o_ovrDacQ_p
    );

  -- Custom component to calibrate the TX IQ imbalance.
  imbalance1_l : radio420x_iqcomp12b_netlist
    port map (
      ce_1                        => '1',
      clk_1                       => designClk_s,
      i_iqsel_p                   => dacIQsel_s,
      iv12_iqdata_p               => dacIQdata_s,
      iv13p11_phaseoffsetcos_p    => iv13_phaseoffsetcos_p,
      iv13p11_phaseoffsetsin_p    => iv13_phaseoffsetsin_p,
      iv13p11_backoff_p           => iv13_backoff_p,
      iv13p11_gainoffset_p        => iv13_gainoffset_p,
      iv24_ddsfreq_p              => iv24_dacOutFreq_p,
      iv3_dacoutsel_p             => iv3_dacOutSel_p,
      o_iqsel_p                   => dacIQselComp_s,
      ov12_iqdata_p               => dacIQdataComp_s
    );

  process (designClk_s)
  begin
    if rising_edge(designClk_s) then
      ov12_dacDataComp_p <= dacIQdataComp_s;
      o_dacIQSelComp_p   <= dacIQselComp_s;
    end if;
  end process;

  --------------------------------------------------------------
  -- Reference Clock buffering
  --------------------------------------------------------------
  -- "if generate" to instantiate the IBUFGS of the FMC clock if the
  -- module is used for the channel #1 of the FMCRADIO
  -- else use direct clocking as the clock is coming from the other module.

  -- Latch i_DesignClkEn_p 2 times to avoid designClk_s glitch
  process(adcClkBufr_s)
  begin
    if falling_edge(adcClkBufr_s) then
      designClkEn_r1 <= i_DesignClkEn_p;
      designClkEn_r2 <= designClkEn_r1;
    end if;
  end process;

  BUFG_inst : BUFGCE
    port map (
      O   => adcClkBufg_s,
      CE  => designClkEn_r2,
      I   => adcClkBufr_s
    );

  genIBUFGDS : if (C_FMCRADIO_CH2 = 0) generate
  begin
    designClk_s <= adcClkBufg_s;
  end generate;

  genBypassIBUFGDS : if (C_FMCRADIO_CH2 = 1) generate
  begin
    designClk_s <= i_designClk_p;
  end generate;

  process(designClk_s)
    variable RstFifo_v : std_logic := '1';
  begin
    if rising_edge(designClk_s) then
        RstFifo_s  <= RstFifo_v;
        RstFifo_v  := i_RstFifo_p;
    end if;
  end process;

  o_designClk_p <= designClk_s;

  --SPI mapping
  spi_port_l: entity lyt_axi_radio420x_v1_00_a.spi_radio420x
    port map (
      i_clk_p                     => i_systemClk_p,
      i_rst_p                     => i_rst_p,
      iv5_fpgaCtrlEn_p            => iv5_fpgaCtrlEn_p,

      iv16_refDacSpiData_p        => v16_refDacSpiData1_s,
      i_refDacSpiStart_p          => refDacSpiStart1_s,
      iv16_HostRefDacSpiData_p    => iv16_HostRefDacSpiData_p,
      i_HostRefDacSpiStart_p      => i_HostRefDacSpiStart_p,
      o_refDacSpiBusy_p           => refDacSpiBusy_s,

      iv16_limeSpiData_p          => v16_limeSpiData1_s,
      i_limeSpiStart_p            => limeSpiStart1_s,
      iv16_HostLimeSpiData_p      => iv16_HostLimeSpiData_p,
      ov16_HostLimeSpiData_p      => ov16_HostLimeSpiData_p,
      i_HostLimeSpiStart_p        => i_HostLimeSpiStart_p,
      o_limeSpiBusy_p             => limeSpiBusy_s,


      iv6_rxGainSpiData_p         => v6_rxGainSpiData1_s,
      iv3_rxGainSpiStart_p        => v3_rxGainSpiStart1_s,
      iv6_HostRxGainSpiData_p     => iv6_HostRxGainSpiData_p,
      iv3_HostRxGainSpiStart_p    => iv3_HostRxGainSpiStart_p,
      o_rxGainSpiBusy_p           => rxGainSpiBusy_s,

      iv6_txGainSpiData_p         => v6_txGainSpiData1_s,
      i_txGainSpiStart_p          => txGainSpiStart1_s,
      iv6_HostTxGainSpiData_p     => iv6_HostTxGainSpiData_p,
      i_HostTxGainSpiStart_p      => i_HostTxGainSpiStart_p,
      o_txGainSpiBusy_p           => txGainSpiBusy_s,

      iv32_pllCtrlSpiData_p       => v32_pllCtrlSpiData1_s,
      iv2_pllCtrlSpiStart_p       => v2_pllCtrlSpiStart1_s,
      iv32_HostPllCtrlSpiData_p   => iv32_HostPllCtrlSpiData_p,
      ov32_HostPllCtrlSpiData_p   => ov32_HostPllCtrlSpiData_p,
      iv2_HostPllCtrlSpiStart_p   => iv2_HostPllCtrlSpiStart_p,
      o_pllCtrlSpiBusy_p          => pllCtrlSpiBusy_s,

      o_limeSpiCs_p               => o_fmcLimeSpiCs_p,
      o_limeSpiClk_p              => o_fmcLimeSpiClk_p,
      o_limeSpiMosi_p             => o_fmcLimeSpiMosi_p,
      i_limeSpiMiso_p             => i_fmcLimeSpiMiso_p,

      o_refDacSpiCs_p             => o_fmcRefDacSpiCs_p,
      o_refDacSpiClk_p            => o_fmcRefDacSpiClk_p,
      o_refDacSpiMosi_p           => o_fmcRefDacSpiMosi_p,

      ov2_pllCtrlSpiCs_p          => ov2_fmcPllCtrlSpiCs_p,
      o_pllCtrlSpiClk_p           => o_fmcPllCtrlSpiClk_p ,
      o_pllCtrlSpiMosi_p          => o_fmcPllCtrlSpiMosi_p,
      i_pllCtrlSpiMiso_p          => i_fmcPllCtrlSpiMiso_p,

      ov3_rxGainSpiCs_p           => ov3_fmcRxGainSpiCs_p,
      o_rxGainSpiClk_p            => o_fmcRxGainSpiClk_p,
      o_rxGainSpiMosi_p           => o_fmcRxGainSpiMosi_p,

      o_txGainSpiCs_p             => o_fmcTxGainSpiCs_p,
      o_txGainSpiClk_p            => o_fmcTxGainSpiClk_p,
      o_txGainSpiMosi_p           => o_fmcTxGainSpiMosi_p
    );

  -- SPI external interface flops (start and data)
  spi_extern_l: process (i_systemClk_p)
  begin
    if rising_edge(i_systemClk_p) then
      if i_rst_p ='1' then
        v16_refDacSpiData0_s  <= (others => '0');
        refDacSpiStart0_s     <= '0';
        v16_limeSpiData0_s    <= (others => '0');
        limeSpiStart0_s       <= '0';
        v6_rxGainSpiData0_s   <= (others => '0');
        v3_rxGainSpiStart0_s  <= (others => '0');
        v6_txGainSpiData0_s   <= (others => '0');
        txGainSpiStart0_s     <= '0';
        v32_pllCtrlSpiData0_s <= (others => '0');
        v2_pllCtrlSpiStart0_s <= (others => '0');

        v16_refDacSpiData1_s  <= (others => '0');
        refDacSpiStart1_s     <= '0';
        v16_limeSpiData1_s    <= (others => '0');
        limeSpiStart1_s       <= '0';
        v6_rxGainSpiData1_s   <= (others => '0');
        v3_rxGainSpiStart1_s  <= (others => '0');
        v6_txGainSpiData1_s   <= (others => '0');
        txGainSpiStart1_s     <= '0';
        v32_pllCtrlSpiData1_s <= (others => '0');
        v2_pllCtrlSpiStart1_s <= (others => '0');

      else
        v16_refDacSpiData0_s  <= iv16_refDacSpiData_p;
        refDacSpiStart0_s     <= i_refDacSpiStart_p;
        v16_limeSpiData0_s    <= iv16_limeSpiData_p;
        limeSpiStart0_s       <= i_limeSpiStart_p;
        v6_rxGainSpiData0_s   <= iv6_rxGainSpiData_p;
        v3_rxGainSpiStart0_s  <= iv3_rxGainSpiStart_p;
        v6_txGainSpiData0_s   <= iv6_txGainSpiData_p;
        txGainSpiStart0_s     <= i_txGainSpiStart_p;
        v32_pllCtrlSpiData0_s <= iv32_pllCtrlSpiData_p;
        v2_pllCtrlSpiStart0_s <= iv2_pllCtrlSpiStart_p;

        v16_refDacSpiData1_s  <= v16_refDacSpiData0_s;
        refDacSpiStart1_s     <= refDacSpiStart0_s;
        v16_limeSpiData1_s    <= v16_limeSpiData0_s;
        limeSpiStart1_s       <= limeSpiStart0_s;
        v6_rxGainSpiData1_s   <= v6_rxGainSpiData0_s;
        v3_rxGainSpiStart1_s  <= v3_rxGainSpiStart0_s;
        v6_txGainSpiData1_s   <= v6_txGainSpiData0_s;
        txGainSpiStart1_s     <= txGainSpiStart0_s;
        v32_pllCtrlSpiData1_s <= v32_pllCtrlSpiData0_s;
        v2_pllCtrlSpiStart1_s <= v2_pllCtrlSpiStart0_s;

      end if;
    end if;
  end process;


  -- SPI external interface flops (busy)
  spi_extern_busy_l: process (i_systemClk_p)
  begin
    if rising_edge(i_systemClk_p) then
      if i_rst_p = '1' then
        refDacSpiBusy0_s      <= '0';
        limeSpiBusy0_s        <= '0';
        rxGainSpiBusy0_s      <= '0';
        txGainSpiBusy0_s      <= '0';
        pllCtrlSpiBusy0_s     <= '0';

        refDacSpiBusy1_s      <= '0';
        limeSpiBusy1_s        <= '0';
        rxGainSpiBusy1_s      <= '0';
        txGainSpiBusy1_s      <= '0';
        pllCtrlSpiBusy1_s     <= '0';
      else
        refDacSpiBusy0_s      <= refDacSpiBusy_s;
        limeSpiBusy0_s        <= limeSpiBusy_s;
        rxGainSpiBusy0_s      <= rxGainSpiBusy_s;
        txGainSpiBusy0_s      <= txGainSpiBusy_s;
        pllCtrlSpiBusy0_s     <= pllCtrlSpiBusy_s;

        refDacSpiBusy1_s      <= refDacSpiBusy0_s;
        limeSpiBusy1_s        <= limeSpiBusy0_s;
        rxGainSpiBusy1_s      <= rxGainSpiBusy0_s;
        txGainSpiBusy1_s      <= txGainSpiBusy0_s;
        pllCtrlSpiBusy1_s     <= pllCtrlSpiBusy0_s;
      end if;
    end if;
  end process;

  o_refDacSpiBusy_p   <= refDacSpiBusy1_s;
  o_limeSpiBusy_p     <= limeSpiBusy1_s;
  o_rxGainSpiBusy_p   <= rxGainSpiBusy1_s;
  o_txGainSpiBusy_p   <= txGainSpiBusy1_s;
  o_pllCtrlSpiBusy_p  <= pllCtrlSpiBusy1_s;

  o_fmcPllLock_p      <= i_fmcPllLock_p;
  o_fmcLimeReset_p    <= i_fmcLimeReset_p;
  o_fmcTxEnable_p     <= i_fmcTxEnable_p;
  o_fmcRxEnable_p     <= i_fmcRxEnable_p;

  ov2_FmcClkMuxSin_p  <= iv2_fmcClkMuxSin_p;
  ov2_FmcClkMuxSout_p <= iv2_fmcClkMuxSout_p;
  o_FmcClkMuxLoad_p   <= i_fmcClkMuxLoad_p;
  o_FmcClkMuxConfig_p <= i_fmcClkMuxConfig_p;

end arch;