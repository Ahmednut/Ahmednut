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
-- File        : $Id: adc_interface.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
--------------------------------------------------------------------------------
-- Description : ADCInterface for ADAC250
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2009 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/09/30
-- $Log: adc_interface.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.2  2013/02/01 16:07:58  julien.roy
-- Small modifications
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
--
-- Revision 1.4  2010/10/27 13:07:26  patrick.gilbert
-- fix timing error... on pll status + adc flag + DAC interface
--
-- Revision 1.3  2010/08/26 21:51:31  francois.blackburn
-- remove mmcm for the adc_interface to allow parameters modifications
--
-- Revision 1.2  2010/07/29 14:22:33  francois.blackburn
-- add another dds
--
-- Revision 1.1.2.2  2010/07/13 17:14:00  francois.blackburn
-- fix mmcm bug
--
-- Revision 1.1.2.1  2010/06/17 21:28:26  francois.blackburn
-- change the dcms for mmcms
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.2  2010/02/10 15:01:24  patrick.gilbert
-- working ADC DAC PLL
--
-- Revision 1.1  2010/01/14 22:49:47  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/09/30 15:35:58  karim.ouadid
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;


library unisim;
  use unisim.vcomponents.all;


entity ADCInterface is
  port (
    i_AdcDataClkP_p       : in std_logic;
    i_AdcDataClkN_p       : in std_logic;
    i_RefClk200MHz_p      : in std_logic;
    i_SystemClk_p         : in std_logic;
    i_rst_p               : in std_logic;

    iv14_ADCDataN_p       : in std_logic_vector (13 downto 0);
    iv14_ADCDataP_p       : in std_logic_vector (13 downto 0);
    
    o_AdcClkBufr_p        : out std_logic;
    o_MmcmReset_p         : out std_logic;
    i_AdcDataClk_p        : in std_logic;
    
    iv5_AdcIdelayValue_p    : in std_logic_vector(4 downto 0);
    iv5_AdcClkIdelayValue_p : in std_logic_vector(4 downto 0);
    o_AdcPatternError_p     : out std_logic;
    
    ov14_AdcDataChA_p   : out std_logic_vector (13 downto 0);
    ov14_AdcDataChB_p   : out std_logic_vector (13 downto 0)
  );
end entity ADCInterface;

architecture arch of ADCInterface is

  signal adcDataInput_s : std_logic_vector(13 downto 0);
  signal adcDataDelayO_s : std_logic_vector(13 downto 0);
  signal DataR_s : std_logic_vector(13 downto 0);
  signal DataF_s : std_logic_vector(13 downto 0);
  signal AdcADataBufR_s : std_logic_vector(13 downto 0);
  signal AdcBDataBufR_s : std_logic_vector(13 downto 0);
  signal AdcADataBufIO_s : std_logic_vector(13 downto 0);
  signal AdcBDataBUFIO_s : std_logic_vector(13 downto 0);
  signal AdcADataOut_s : std_logic_vector(13 downto 0);
  signal AdcBDataOut_s : std_logic_vector(13 downto 0);

  signal adcClkInput_s : std_logic;
  signal adcClkDelayO_s : std_logic;
  signal adcClkBufR_s : std_logic;

  signal adcClkDcmO_s : std_logic;
  signal adcClkBufgO_s : std_logic;
  signal dcmLock_s: std_logic;

  signal idelayCtrlRdy_s : std_logic;
  signal MmcmRst_s : std_logic;
  signal MmcmFbOutBufg_s : std_logic;
  signal MmcmFbOut_s     : std_logic;

  signal clkout0b_unused  : std_logic;
  signal clkout1_unused   : std_logic;
  signal clkout1b_unused  : std_logic;
  signal clkout2_unused   : std_logic;
  signal clkout2b_unused  : std_logic;
  signal clkout3_unused   : std_logic;
  signal clkout3b_unused  : std_logic;
  signal clkout4_unused   : std_logic;
  signal clkout5_unused   : std_logic;
  signal clkout6_unused   : std_logic;
  -- Dynamic programming unused signals
  signal do_unused        : std_logic_vector(15 downto 0);
  signal drdy_unused      : std_logic;
  -- Dynamic phase shift unused signals
  signal psdone_unused    : std_logic;
  -- Unused status signals
  signal clkfbstopped_unused : std_logic;
  signal clkinstopped_unused : std_logic;

begin

  --IOB generation;
  iob_ADCData_gen_l : for i in 13 downto 0 generate
  begin
    ADCDataInst_l : IBUFDS
      generic map (
        DIFF_TERM => TRUE, -- Differential Termination (Virtex-4/5, Spartan-3E/3A)
        IOSTANDARD => "LVDS_25")
      port map (
        O => adcDataInput_s(i), -- Clock buffer output
        I => iv14_ADCDataP_p(i),
        IB => iv14_ADCDataN_p(i)
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

    ADCDataIDDR_l : IDDR
      generic map (
        DDR_CLK_EDGE => "SAME_EDGE_PIPELINED",
        INIT_Q1 => '0', -- Initial value of Q1: ’0’ or ’1’
        INIT_Q2 => '0', -- Initial value of Q2: ’0’ or ’1’
        SRTYPE => "SYNC") -- Set/Reset type: "SYNC" or "ASYNC"
      port map (
        Q1 => DataR_s(i), -- 1-bit output for positive edge of clock
        Q2 => DataF_s(i), -- 1-bit output for negative edge of clock
        C => i_AdcDataClk_p, -- 1-bit clock input
        --C => adcClkBufR_s,
        CE => '1', -- 1-bit clock enable input
        D => adcDataDelayO_s(i), -- 1-bit DDR data input
        R => '0', -- 1-bit reset
        S => '0' -- 1-bit set
      );


  end generate iob_ADCData_gen_l;

  ADCClkInst_l : IBUFDS
    generic map (
      DIFF_TERM => TRUE, -- Differential Termination (Virtex-4/5, Spartan-3E/3A)
      IOSTANDARD => "LVDS_25")
    port map (
      O => adcClkInput_s, -- Clock buffer output
      I => i_AdcDataClkP_p,
      IB => i_AdcDataClkN_p
    );
    
  -- Input delay
  ADCDataIdelay_lL : iodelaye1
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

  IDELAYCTRL_inst : IDELAYCTRL
    port map (
      RDY => idelayCtrlRdy_s,
      REFCLK => i_RefClk200MHz_p,
      RST => i_rst_p
    );

  MmcmRst_s <= not idelayCtrlRdy_s;

  o_AdcClkBufr_p <= AdcClkBufr_s;
  o_MmcmReset_p <= MmcmRst_s;

  --add pipeline on bufR stage after bufio


  --output assignement;
  AdcADataBufIO_s <=   DataF_s(6) & DataR_s(6) &
                       DataF_s(5) & DataR_s(5) & DataF_s(4) & DataR_s(4) &
                       DataF_s(3) & DataR_s(3) & DataF_s(2) & DataR_s(2) &
                       DataF_s(1) & DataR_s(1) & DataF_s(0) & DataR_s(0);

  AdcBDataBufIO_s <=   DataF_s(13) & DataR_s(13) &
                       DataF_s(12) & DataR_s(12) & DataF_s(11) & DataR_s(11) &
                       DataF_s(10) & DataR_s(10) & DataF_s(9)  & DataR_s(9)  &
                       DataF_s(8)  & DataR_s(8)  & DataF_s(7)  & DataR_s(7);

  --add pipeline on bufR stage after bufio
  process(i_AdcDataClk_p)
    begin
      if (rising_edge(i_AdcDataClk_p)) then
        AdcADataBufR_s <= AdcADataBufIO_s;
        AdcBDataBufR_s <= AdcBDataBufIO_s;
        
        AdcADataOut_s <= AdcADataBufR_s;
        AdcBDataOut_s <= AdcBDataBufR_s;
      end if;
  end process;
  
  ----------------------------------------------------------------------------------------------------
  -- Pattern verification
  ----------------------------------------------------------------------------------------------------
  process (i_rst_p, i_AdcDataClk_p)
    variable counter                : unsigned(9 downto 0);
    variable adcPatternError_s      : std_logic;
    variable v2_adcAPatternVerify_s : std_logic_vector(1 downto 0);
    variable v2_adcBPatternVerify_s : std_logic_vector(1 downto 0);
    
  begin
    if (i_rst_p = '1') then
      counter                     := (others => '0');      
      adcPatternError_s           := '0';      
      o_AdcPatternError_p         <= '0';
      
    elsif (rising_edge(i_AdcDataClk_p)) then
      
      if counter = "0000000000" then
        o_AdcPatternError_p  <= adcPatternError_s;
        
        adcPatternError_s := '0';
      end if;
      
      if v2_adcAPatternVerify_s = "11" then
        adcPatternError_s := '1';
      end if;
      
      v2_adcAPatternVerify_s(1 downto 0) := "00";
      if (AdcADataBufR_s /= "10101010101010") then
        v2_adcAPatternVerify_s(0) := '1';
      end if;
      if (AdcADataBufR_s /= "01010101010101") then
        v2_adcAPatternVerify_s(1) := '1';
      end if;
      
      v2_adcBPatternVerify_s(1 downto 0) := "00";
      if (AdcBDataBufR_s /= "10101010101010") then
        v2_adcBPatternVerify_s(0) := '1';
      end if;
      if (AdcBDataBufR_s /= "01010101010101") then
        v2_adcBPatternVerify_s(1) := '1';
      end if;
    
      counter := counter + 1;
        
    end if;
  end process;

  
  ov14_AdcDataChA_p <= AdcADataOut_s;
  ov14_AdcDataChB_p <= AdcBDataOut_s;
  
end architecture;