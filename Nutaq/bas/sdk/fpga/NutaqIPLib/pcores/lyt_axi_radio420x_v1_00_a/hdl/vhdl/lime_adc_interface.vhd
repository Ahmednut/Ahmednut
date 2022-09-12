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
-- File        : $Id: lime_adc_interface.vhd,v 1.1 2015/04/10 19:48:36 sqa Exp $
--------------------------------------------------------------------------------
-- Description : ADC Interface for LimeMicro LMS6002D
--------------------------------------------------------------------------------
-- Notes / Assumptions : This core use source synchronous to remove any timing
-- problem due to unknow clock frequency and phase. By sampling the forwarded
-- clock of the LMS6002D on falling edge we remove any probleme related to timing.
-- IOB Delay, Hardware(PCB) delay. The PLL clock pass by a GC is used to read the
-- FIFO DATA.
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lime_adc_interface.vhd,v $
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.4  2013/11/08 16:52:08  julien.roy
-- Change Radio420 clock architecture.
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
-- Revision 1.2  2011/08/30 19:58:58  patrick.gilbert
-- remove IO standard to reduce warning... set anyway in UCF
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

entity lime_adc_interface is
port (
  i_DesignClk_p             : in std_logic;
  i_RefClk200MHz_p          : in std_logic;
  i_SystemClk_p             : in std_logic;
  i_Rst_p                   : in std_logic;
  i_RstFifo_p               : in std_logic;

  i_AdcDataClk_p            : in std_logic;
  i_IQSel_p                 : in std_logic;
  iv12_ADCData_p            : in std_logic_vector(11 downto 0);

  o_IDelayRdy_p             : out std_logic;
  iv5_AdcIdelayValue_p      : in std_logic_vector(4 downto 0);
  iv5_AdcClkIdelayValue_p   : in std_logic_vector(4 downto 0);

  ov12_userAdcData_p        : out std_logic_vector (11 downto 0);
  o_userAdcIQSel_p          : out std_logic;
  
  o_AdcClkBufr_p            : out std_logic
  );
end entity lime_adc_interface;

architecture arch of lime_adc_interface is

  ----------------------------------------
  -- Component declaration
  ----------------------------------------

  component radio420x_fifo1k_18b_async
    port (
      rst: IN std_logic;
      wr_clk: IN std_logic;
      rd_clk: IN std_logic;
      din: IN std_logic_VECTOR(17 downto 0);
      wr_en: IN std_logic;
      rd_en: IN std_logic;
      dout: OUT std_logic_VECTOR(17 downto 0);
      full: OUT std_logic;
      empty: OUT std_logic
    );
  end component;

  ----------------------------------------
  -- Signals declaration
  ----------------------------------------

  signal idelayCtrlRdy_s   : std_logic;

  signal AdcDataInput_s    : std_logic_vector(11 downto 0);
  signal AdcDataDelayO_s   : std_logic_vector(11 downto 0);
  signal AdcDataBufR_s     : std_logic_vector(11 downto 0);
  signal AdcDataBufIO_s    : std_logic_vector(11 downto 0);

  signal v18_fifoDout_s    : std_logic_vector(17 downto 0);
  signal v18_fifoDin_s     : std_logic_vector(17 downto 0);
  signal fifoEmpty_s       : std_logic;
  signal nfifoEmpty_s      : std_logic;

  signal adcIQSel_s        : std_logic;
  signal adcIQSelDelayO_s  : std_logic;
  signal adcIQSelBufIO_s   : std_logic;
  signal AdcIQSelBufR_s    : std_logic;

  signal adcClkInput_s     : std_logic;
  signal adcClkDelayO_s    : std_logic;
  signal adcClkBufIO_s     : std_logic;
  signal adcClkBufR_s      : std_logic;
  
  signal v16_FreqCnt_s     : std_logic_vector(15 downto 0);

begin

  -----------------------------------------
  -- IDELAYCTRL instance
  -----------------------------------------
  IDELAYCTRL_inst : IDELAYCTRL
    port map (
      RDY     => idelayCtrlRdy_s,
      REFCLK  => i_RefClk200MHz_p,
      RST     => i_rst_p
    );

  o_IDelayRdy_p <= idelayCtrlRdy_s;

  -----------------------------------------
  -- ADC data IO pins
  -----------------------------------------
  iob_ADCData_gen_l : for i in 11 downto 0 generate
  begin
    ADCDataInst_l : IBUF
      port map (
        O => AdcDataInput_s(i),
        I => iv12_ADCData_p(i)
      );

    -- Input delay
    ADCDataIDelay_l : iodelaye1
      generic map (
        IDELAY_TYPE  => "VAR_LOADABLE",
        IDELAY_VALUE => 0,
        DELAY_SRC    => "I"
      )
      port map (
        DATAOUT => adcDataDelayO_s(i),
        IDATAIN => adcDataInput_s(i),

        c => i_SystemClk_p,
        ce => '1',
        inc => '1',
        datain => '0',
        odatain => '0',
        clkin => '0',
        rst => '1',
        cntvaluein => iv5_AdcIdelayValue_p,
        cinvctrl => '0',
        t => '1'
      );

    ADCDataFF_l : entity lyt_axi_radio420x_v1_00_a.io_dff
      generic map (
        io_dff_Falling_edge => true
      )
      port map(
        clk     => adcClkBufIO_s,
        din     => AdcDataDelayO_s(i),
        dout    => AdcDataBufIO_s(i)
      );

  end generate iob_ADCData_gen_l;

  -----------------------------------------
  -- ADC IQ select IO pin
  -----------------------------------------
  ADCIOSelInst_l : IBUF
    port map (
      O => adcIQSel_s,
      I => i_IQSel_p
    );
    
  -- Input delay
  ADCIQSelIDelay_l : iodelaye1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I"
    )
    port map (
      DATAOUT => adcIQSelDelayO_s,
      IDATAIN => adcIQSel_s,

      c => i_SystemClk_p,
      ce => '1',
      inc => '1',
      datain => '0',
      odatain => '0',
      clkin => '0',
      rst => '1',
      cntvaluein => iv5_AdcIdelayValue_p,
      cinvctrl => '0',
      t => '1'
    );

  ADCIOSelFF_l : entity lyt_axi_radio420x_v1_00_a.io_dff
    generic map (
      io_dff_Falling_edge => true
    )
    port map(
      clk     => adcClkBufIO_s,
      din     => AdcIQSelDelayO_s,
      dout    => AdcIQSelBufIO_s
    );

  -----------------------------------------
  -- ADC data clock IO pin
  -----------------------------------------
  ADCClkInst_l : IBUF
    port map (
      O => adcClkInput_s,
      I => i_AdcDataClk_p
    );
    
  -- Input delay
  ADCClockIdelay_l : iodelaye1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I",
      SIGNAL_PATTERN => "CLOCK"
    )
    port map (
      DATAOUT => adcClkDelayO_s,
      IDATAIN => adcClkInput_s,

      c => i_SystemClk_p,
      ce => '1',
      inc => '1',
      datain => '0',
      odatain => '0',
      clkin => '0',
      rst => '1',
      cntvaluein => iv5_AdcClkIdelayValue_p,
      cinvctrl => '0',
      t => '1'
    );

  BUFIO_inst : BUFIO
    port map (
      O => adcClkBufIO_s,
      I => adcClkDelayO_s
    );

  BUFR_inst : BUFR
    generic map (
      BUFR_DIVIDE => "BYPASS",
      SIM_DEVICE => "VIRTEX6"
    )
    port map (
      O => adcClkBufR_s,
      CE => '1',
      CLR => '0',
      I => adcClkDelayO_s
    );
    
  o_AdcClkBufr_p <= adcClkBufR_s;

  -----------------------------------------
  -- Clock domain crossing
  -----------------------------------------
  process(adcClkBufR_s)
  begin
    if rising_edge(adcClkBufR_s) then
      AdcDataBufR_s <= AdcDataBufIO_s;
      AdcIQSelBufR_s <= AdcIQSelBufIO_s;
    end if;
  end process;

  v18_fifoDin_s <= "00000" & AdcDataBufR_s & AdcIQSelBufR_s;

  adcFifo_l : radio420x_fifo1k_18b_async
    port map (
      rst => i_RstFifo_p,
      wr_clk => adcClkBufR_s,
      rd_clk => i_DesignClk_p,
      din => v18_fifoDin_s,
      wr_en => '1',
      rd_en => nfifoEmpty_s,
      dout => v18_fifoDout_s,
      full => open,
      empty => fifoEmpty_s
  );

  nfifoEmpty_s <= not fifoEmpty_s;

  ov12_userAdcData_p <= v18_fifoDout_s(12 downto 1);
  o_userAdcIQSel_p   <= v18_fifoDout_s(0);

end architecture;