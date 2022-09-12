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
-- File        : $Id: adac250_wrapper.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
--------------------------------------------------------------------------------
-- Description : ADAC250Wrapper
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
-- $Log: adac250_wrapper.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.5  2015/02/05 16:41:56  julien.roy
-- Remove PPS port since it is the same signal than trigger
--
-- Revision 1.4  2014/04/14 14:29:06  julien.roy
-- Add register stage to ease timing
--
-- Revision 1.3  2013/08/29 19:53:37  khalid.bensadek
-- Added pps out port
--
-- Revision 1.2  2013/02/01 16:07:58  julien.roy
-- Small modifications
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
--
-- Revision 1.10  2011/01/11 16:18:39  patrick.gilbert
-- fix OVR delay z-1  behavior...
--
-- Revision 1.9  2010/12/17 14:49:44  jeffrey.johnson
-- Modified trigger delay to compensate for recent mods
--
-- Revision 1.8  2010/11/30 20:51:37  patrick.gilbert
-- fix FMC interface to make linux build + remove adc_synchronous parameter
--
-- Revision 1.7  2010/10/29 15:28:56  jeffrey.johnson
-- Added DAC design clock input and DAC reference clock output.
-- Moved DAC reference clock buffer from dac_interface to adac250_wrapper.
-- Changed DAC clk IDELAY value from 0 to 10.
--
-- Revision 1.6  2010/10/27 13:07:26  patrick.gilbert
-- fix timing error... on pll status + adc flag + DAC interface
--
-- Revision 1.5  2010/09/17 15:30:54  patrick.gilbert
-- fix DAC_sync pin + pass new automatic MMCM config.
--
-- Revision 1.4  2010/08/26 21:51:31  francois.blackburn
-- remove mmcm for the adc_interface to allow parameters modifications
--
-- Revision 1.3  2010/08/03 19:22:59  francois.blackburn
-- add mix divider
--
-- Revision 1.2  2010/07/29 14:22:33  francois.blackburn
-- add another dds
--
-- Revision 1.1.2.1  2010/07/13 17:14:00  francois.blackburn
-- fix mmcm bug
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.2  2010/02/10 15:01:06  patrick.gilbert
-- working ADC DAC PLL
--
-- Revision 1.1  2010/01/14 22:50:02  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/09/30 15:35:58  karim.ouadid
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library unisim;
  use unisim.vcomponents.all;

entity adac250_wrapper is
port
(
  ----***FMC Ports mapping***----
  i_SystemClk_p               : in std_logic;
  i_RefClk200MHz_p            : in std_logic;
  i_rst_p                     : in std_logic;

  i_AdcDataClk_p              : in std_logic;
  i_AdcClkLock_p              : in std_logic;
  o_AdcClkBufr_p              : out std_logic;
  o_MmcmReset_p               : out std_logic;

  --FMC mapping
  idp_fmcClk0_p          : in std_logic;
  idn_fmcClk0_p          : in std_logic;
  iv15dp_AdcExtBus_p       : in std_logic_vector(14 downto 0);
  iv15dn_AdcExtBus_p       : in std_logic_vector(14 downto 0);
  ov18dp_DacExtBus_p       : out std_logic_vector(17 downto 0);
  ov18dn_DacExtBus_p       : out std_logic_vector(17 downto 0);
  iv3_CtrlExtBus_p         : in std_logic_vector(2 downto 0);
  ov23_CtrlExtBus_p        : out std_logic_vector(22 downto 0);

  ----***ADC ports***----
  --To user logic
  ov14_AdcDataChA_p           : out std_logic_vector (13 downto 0);
  ov14_AdcDataChB_p           : out std_logic_vector (13 downto 0);
  -- From control
  iv2_AdcRun_p                : in std_logic_vector (1 downto 0);
  iv2_AdcStart_p              : in std_logic_vector (1 downto 0);
  --***DAC ports***----
  iv16_DacChA_p                : in std_logic_vector(15 downto 0);
  iv16_DacChB_p                : in std_logic_vector(15 downto 0);
  i_DacDesignClk_p             : in std_logic;
  i_DacDataSync_p              : in std_logic;
  o_DacRefDataClk_p            : out std_logic;
  o_DacDataClkLocked_p         : out std_logic;

  iv3_DacAMuxSelect_p           : in std_logic_vector(2 downto 0);
  iv3_DacBMuxSelect_p           : in std_logic_vector(2 downto 0);
  ----***ADCOutOfRange***----
  i_DataType_p                 : in std_logic;                         -- 0 = offset binary output format / 1= 2s complement output format
  o_ChA_OvrFiltred_p           : out std_logic;
  o_ChA_OvrNotFiltred_p        : out std_logic;
  o_ChB_OvrFiltred_p           : out std_logic;
  o_ChB_OvrNotFiltred_p        : out std_logic;

  ----***Trigger***----
  iv5_TriggerDelay_p           : in std_logic_vector(4 downto 0);
  o_Trigger_p                  : out std_logic;

  o_SpiBusy_p                  : out std_logic;
  iv9_SpiWriteaddr_p           : in  std_logic_vector (8 downto 0);
  iv9_SpiReadaddr_p            : in  std_logic_vector (8 downto 0);
  i_SpiReq_p                   : in  std_logic;
  iv32_SpiDin_p                : in  std_logic_vector (31 downto 0);
  o_SpiGnt_p                   : out std_logic;
  ov32_SpiDout_p                 : out std_logic_vector (31 downto 0);
  o_SpiAck_p                   : out std_logic;

  -- Simple IO mapping
  --From/To custom logic
  i_AdcSpiReset_p              : in std_logic;
  i_DacReset_p                 : in std_logic;
  o_PllStatus_p                : out  std_logic;
  i_PllFunction_p              : in std_logic;
  i_ClkMuxConfig_p             : in std_logic;
  i_ClkMuxLoad_p               : in std_logic;
  iv2_ClkMuxSin_p              : in std_logic_vector (1 downto 0);
  iv2_ClkMuxSout_p             : in std_logic_vector (1 downto 0);

  iv5_AdcIdelayValue_p         : in std_logic_vector(4 downto 0);
  iv5_AdcClkIdelayValue_p      : in std_logic_vector(4 downto 0);
  o_AdcPatternError_p          : out std_logic;
  iv5_DacIdelayValue_p         : in std_logic_vector(4 downto 0);
  iv5_DacClkIdelayValue_p      : in std_logic_vector(4 downto 0);

  iv2_FreqCntClkSel_p          : in std_logic_vector(1 downto 0);
  ov16_FreqCntClkCnt_p         : out std_logic_vector(15 downto 0));
end entity adac250_wrapper;

architecture arch of adac250_wrapper is

  ---------------------------------
  -- Signal declaration
  ---------------------------------
  signal AdcALedRed_s                     : std_logic;
  signal AdcALedGreen_s                   : std_logic;
  signal AdcBLedRed_s                     : std_logic;
  signal AdcBLedGreen_s                   : std_logic;
  signal DacLedRed_s                      : std_logic;
  signal DacLedGreen_s                    : std_logic;
  signal PllLedRed_s                      : std_logic;
  signal PllLedGreen_s                    : std_logic;
  signal DacDataClkP_s                    : std_logic;
  signal DacDataClkN_s                    : std_logic;
  
  -- DAC External Reference clock
  signal DacRefDataClk_s                  : std_logic;
  signal DacRefDataClkB_s                 : std_logic;

  signal AdcClkBufr_s                     : std_logic;
  signal MmcmReset_s                      : std_logic;
  signal v14_AdcDataChA_s                 : std_logic_vector(13 downto 0);
  signal v14_AdcDataChB_s                 : std_logic_vector(13 downto 0);
  signal v14_OvrDataChA_s                 : std_logic_vector(13 downto 0);
  signal v14_OvrDataChB_s                 : std_logic_vector(13 downto 0);
  signal v16_DacDataP_s                   : std_logic_vector(15 downto 0);
  signal v16_DacDataN_s                   : std_logic_vector(15 downto 0);
  signal DacSyncP_s                       : std_logic;
  signal DacSyncN_s                       : std_logic;
  signal DacDataClkLocked_s               : std_logic;
  signal Trigger_s                        : std_logic;
  signal ChA_ovr_s                        : std_logic;
  signal ChA_udr_s                        : std_logic;
  signal ChB_Ovr_s                        : std_logic;
  signal ChB_udr_s                        : std_logic;
  signal ChA_OvrFiltred_s                 : std_logic;
  signal ChA_OvrNotFiltred_s              : std_logic;
  signal ChB_OvrFiltred_s                 : std_logic;
  signal ChB_OvrNotFiltred_s              : std_logic;
  signal v2_AdcOvr_s                      : std_logic_vector(1 downto 0);
  signal v2_Adcudr_s                      : std_logic_vector(1 downto 0);
  signal busy_s                           : std_logic;
  signal v9_ADCDACGainUpdaterWriteAddr_s  : std_logic_vector(8 downto 0);
  signal ADCDACGainUpdaterReq_s           : std_logic;
  signal v32_ADCDACGainUpdaterDout_s      : std_logic_vector(31 downto 0);
  signal ADCDACGainUpdaterGnt_s           : std_logic;
  signal ADCDACGainUpdaterAck_s           : std_logic;
  signal AckUpdateADCChAFineGain_s        : std_logic;
  signal AckUpdateADCChBFineGain_s        : std_logic;
  signal AckUpdateADCChACoarseGain_s      : std_logic;
  signal AckUpdateADCChBCoarseGain_s      : std_logic;
  signal AckUpdateDACChAGain_s            : std_logic;
  signal AckUpdateDACChBGain_s            : std_logic;
  signal UpdaterBusy_s                    : std_logic;

  --adc port map signal
  signal AdcDataClkP_s          : std_logic;
  signal AdcDataClkN_s          : std_logic;
  signal v14_ADCDataN_s         : std_logic_vector (13 downto 0);
  signal v14_ADCDataP_s         : std_logic_vector (13 downto 0);

  signal AdcSpiResetOut_s       : std_logic;
  signal clkMuxConfOut_s        : std_logic;
  signal clkMuxLoadOut_s        : std_logic;
  signal v2_ClkMuxSinOut_s      : std_logic_vector(1 downto 0);
  signal v2_ClkMuxSoutOut_s     : std_logic_vector(1 downto 0);
  signal PLLStatusIO_s          : std_logic;
  signal PLLStatusIn_s          : std_logic;
  signal PLLFunctionOut_s       : std_logic;
  signal PLLFunctionIO_s        : std_logic;
  signal nDacReset_s            : std_logic;
  signal nDacResetOut_s         : std_logic;
  signal SpiClk_s               : std_logic;
  signal SpiClkOut_s            : std_logic;
  signal SpiMosi_s              : std_logic;
  signal SpiMosiOut_s           : std_logic;
  signal SpiMisoIn_s            : std_logic;
  signal SpiCs_s                : std_logic_vector(7 downto 0);
  signal SpiCsOut_s             : std_logic_vector(7 downto 0);

  signal DacClkP_s              : std_logic;
  signal DacClkN_s              : std_logic;

  signal dacSrcDataA_s          : std_logic_vector(15 downto 0);
  signal dacSrcDataB_s          : std_logic_vector(15 downto 0);
  signal dacSrcSync_s           : std_logic;

  signal v5_TriggerDelay_s      : std_logic_vector(4 downto 0);

  signal triggerIn_s            : std_logic;

  signal v4_TestClks_s          : std_logic_vector(3 downto 0);
  
  
  signal v3_DacAMuxSelect_R1_s : std_logic_vector(2 downto 0);
  signal v3_DacAMuxSelect_R2_s : std_logic_vector(2 downto 0);
  signal v3_DacBMuxSelect_R1_s : std_logic_vector(2 downto 0);
  signal v3_DacBMuxSelect_R2_s : std_logic_vector(2 downto 0);
  

  attribute keep : string;
  
  attribute keep of v3_DacAMuxSelect_R1_s : signal is "true";
  attribute keep of v3_DacBMuxSelect_R1_s : signal is "true";

begin

  o_AdcClkBufr_p <= AdcClkBufr_s;
  o_MmcmReset_p  <= MmcmReset_s;

-- FMC port Mapping

  -- ADC map
  AdcDataClkP_s              <= iv15dp_AdcExtBus_p(14);
  AdcDataClkN_s              <= iv15dn_AdcExtBus_p(14);
  v14_ADCDataP_s(13 downto 0)<= iv15dp_AdcExtBus_p(13 downto 0);
  v14_ADCDataN_s(13 downto 0)<= iv15dn_AdcExtBus_p(13 downto 0);


-- -- led map
  ov23_CtrlExtBus_p(0) <=  AdcBLedRed_s;
  ov23_CtrlExtBus_p(1) <=  AdcBLedGreen_s;
  ov23_CtrlExtBus_p(2) <=  DacLedRed_s;
  ov23_CtrlExtBus_p(3) <=  DacLedGreen_s;
  ov23_CtrlExtBus_p(4) <=  PllLedRed_s;
  ov23_CtrlExtBus_p(5) <=  PllLedGreen_s;
  ov23_CtrlExtBus_p(6) <=  AdcALedRed_s;
  ov23_CtrlExtBus_p(7) <=  AdcALedGreen_s;


  ov23_CtrlExtBus_p(8)     <=  AdcSpiResetOut_s;
  ov23_CtrlExtBus_p(9)     <=  clkMuxConfOut_s;
  ov23_CtrlExtBus_p(10)    <=  clkMuxLoadOut_s;
  ov23_CtrlExtBus_p(11)    <=  v2_ClkMuxSinOut_s(0);
  ov23_CtrlExtBus_p(12)    <=  v2_ClkMuxSinOut_s(1);
  ov23_CtrlExtBus_p(13)    <=  v2_ClkMuxSoutOut_s(0);
  ov23_CtrlExtBus_p(14)    <=  v2_ClkMuxSoutOut_s(1);

  --SPI mapping
  ov23_CtrlExtBus_p(15)    <=  SpiMosiOut_s;   --MOSI
  ov23_CtrlExtBus_p(16)    <=  SpiClkOut_s;   --SPI clk
  ov23_CtrlExtBus_p(17)    <=  SpiCsOut_s(0);  --PLL CS
  ov23_CtrlExtBus_p(18)    <=  SpiCsOut_s(2);  --DAC CS
  ov23_CtrlExtBus_p(19)    <=  SpiCsOut_s(1);  --ADC CS
  ov23_CtrlExtBus_p(20)    <=  SpiCsOut_s(3);  --CLKDAC CS

   ov23_CtrlExtBus_p(21)   <=  nDacResetOut_s;
   ov23_CtrlExtBus_p(22)   <=  PLLFunctionIO_s;

  --DAC mapping

  DacClkP_s <= idp_fmcClk0_p;
  DacClkN_s <= idn_fmcClk0_p;
  ov18dp_DacExtBus_p(16)   <= DacSyncP_s;
  ov18dn_DacExtBus_p(16)   <= DacSyncN_s;

  ov18dp_DacExtBus_p(0)  <= v16_DacDataP_s(0);
  ov18dp_DacExtBus_p(1)  <= v16_DacDataP_s(1);
  ov18dp_DacExtBus_p(2)  <= v16_DacDataP_s(2);
  ov18dp_DacExtBus_p(3)  <= v16_DacDataP_s(3);
  ov18dp_DacExtBus_p(4)  <= v16_DacDataP_s(4);
  ov18dp_DacExtBus_p(5)  <= v16_DacDataP_s(5);
  ov18dp_DacExtBus_p(6)  <= v16_DacDataP_s(6);
  ov18dp_DacExtBus_p(7)  <= v16_DacDataP_s(7);
  ov18dp_DacExtBus_p(8)  <= v16_DacDataP_s(8);
  ov18dp_DacExtBus_p(9)  <= v16_DacDataP_s(9);
  ov18dp_DacExtBus_p(10) <= v16_DacDataP_s(10);
  ov18dp_DacExtBus_p(11) <= v16_DacDataP_s(11);
  ov18dp_DacExtBus_p(12) <= v16_DacDataP_s(12);
  ov18dp_DacExtBus_p(13) <= v16_DacDataP_s(13);
  ov18dp_DacExtBus_p(14) <= v16_DacDataP_s(14);
  ov18dp_DacExtBus_p(15) <= v16_DacDataP_s(15);

  ov18dn_DacExtBus_p(0)  <= v16_DacDataN_s(0);
  ov18dn_DacExtBus_p(1)  <= v16_DacDataN_s(1);
  ov18dn_DacExtBus_p(2)  <= v16_DacDataN_s(2);
  ov18dn_DacExtBus_p(3)  <= v16_DacDataN_s(3);
  ov18dn_DacExtBus_p(4)  <= v16_DacDataN_s(4);
  ov18dn_DacExtBus_p(5)  <= v16_DacDataN_s(5);
  ov18dn_DacExtBus_p(6)  <= v16_DacDataN_s(6);
  ov18dn_DacExtBus_p(7)  <= v16_DacDataN_s(7);
  ov18dn_DacExtBus_p(8)  <= v16_DacDataN_s(8);
  ov18dn_DacExtBus_p(9)  <= v16_DacDataN_s(9);
  ov18dn_DacExtBus_p(10) <= v16_DacDataN_s(10);
  ov18dn_DacExtBus_p(11) <= v16_DacDataN_s(11);
  ov18dn_DacExtBus_p(12) <= v16_DacDataN_s(12);
  ov18dn_DacExtBus_p(13) <= v16_DacDataN_s(13);
  ov18dn_DacExtBus_p(14) <= v16_DacDataN_s(14);
  ov18dn_DacExtBus_p(15) <= v16_DacDataN_s(15);

  ov18dp_DacExtBus_p(17) <= DacDataClkP_s;
  ov18dn_DacExtBus_p(17) <= DacDataClkN_s;

  -- ADC interface
  ADCInterface_l : entity work.ADCInterface
    port map(
    i_AdcDataClkP_p       => AdcDataClkP_s,
    i_AdcDataClkN_p       => AdcDataClkN_s,
    i_RefClk200MHz_p      => i_RefClk200MHz_p,
    i_SystemClk_p         => i_SystemClk_p,
    i_rst_p               => i_rst_p,

    iv14_ADCDataN_p       => v14_ADCDataN_s,
    iv14_ADCDataP_p       => v14_ADCDataP_s,

    o_AdcClkBufr_p        => AdcClkBufr_s,
    o_MmcmReset_p         => MmcmReset_s,
    i_AdcDataClk_p        => i_AdcDataClk_p,


    iv5_AdcIdelayValue_p     => iv5_AdcIdelayValue_p,
    iv5_AdcClkIdelayValue_p  => iv5_AdcClkIdelayValue_p,

    o_AdcPatternError_p   => o_AdcPatternError_p,
    ov14_AdcDataChA_p     => v14_AdcDataChA_s,
    ov14_AdcDataChB_p     => v14_AdcDataChB_s);

  -- DAC Interface
  DACInterface_l : entity work.DACInterface
    port map(
    -- Direct to IO
    ov16_DacDataP_p             => v16_DacDataP_s,
    ov16_DacDataN_p             => v16_DacDataN_s,
    o_DacDataClkP_p             => DacDataClkP_s,
    o_DacDataClkN_p             => DacDataClkN_s,
    o_DacSyncP_p                => DacSyncP_s,
    o_DacSyncN_p                => DacSyncN_s,
    -- Global Signals
    i_SystemClk_p               => i_SystemClk_p,
    i_rst_p                     => i_rst_p,
    -- User Interface
    iv16_DacChA_p               => dacSrcDataA_s,
    iv16_DacChB_p               => dacSrcDataB_s,
    i_DacDesignClk_p            => i_DacDesignClk_p,
    i_DacDataSync_p             => dacSrcSync_s,


    iv5_DacIdelayValue_p        => iv5_DacIdelayValue_p,
    iv5_DacClkIdelayValue_p     => iv5_DacClkIdelayValue_p,

    o_DacDataClkLocked_p        => DacDataClkLocked_s,
    i_RefClk200MHz_p            => i_RefClk200MHz_p);



  AMuxProc_l : process(i_DacDesignClk_p)
  begin
    if (rising_edge(i_DacDesignClk_p)) then
      
      v3_DacAMuxSelect_R1_s <= iv3_DacAMuxSelect_p;
      v3_DacAMuxSelect_R2_s <= v3_DacAMuxSelect_R1_s;
      
      case  v3_DacAMuxSelect_R2_s is
        when "000" => --User data
          dacSrcSync_s <= i_DacDataSync_p ;
          dacSrcDataA_s <= iv16_DacChA_p ;
        when "100" => -- Pattern
          dacSrcSync_s <= '1';
          dacSrcDataA_s <= X"5555";
        when others =>
          dacSrcDataA_s <= (others=>'0');
      end case;
    end if;
  end process;

  BMuxProc_l : process(i_DacDesignClk_p)
  begin
    if (rising_edge(i_DacDesignClk_p)) then
      
      v3_DacBMuxSelect_R1_s <= iv3_DacBMuxSelect_p;
      v3_DacBMuxSelect_R2_s <= v3_DacBMuxSelect_R1_s;
      
      case  v3_DacBMuxSelect_R2_s is
        when "000" => --User data
          dacSrcDataB_s <= iv16_DacChB_p;
        when "100" => -- Pattern
          dacSrcDataB_s <= X"AAAA";
        when others =>
          dacSrcDataB_s <= (others=>'0');
      end case;
    end if;
  end process;

  ADCOutOfRange_l : entity work.ADCOutOfRange
  generic map(
    FilterLength => 26 --give near .5sec flag.
  )
  port map(
    i_clk_p                 => i_AdcDataClk_p,
    i_rst_p                 => i_rst_p,
    iv14_DataCh1_p          => v14_AdcDataChA_s,
    iv14_DataCh2_p          => v14_AdcDataChB_s,
    ov14_DataCh1_p          => v14_OvrDataChA_s,
    ov14_DataCh2_p          => v14_OvrDataChB_s,
    i_DataType_p            => i_DataType_p,        -- 0 = offset binary output format / 1= 2s complement output format
    o_Ch1_OvrNotFiltred_p   => ChA_OvrNotFiltred_s,
    o_Ch1_OvrFiltred_p      => ChA_OvrFiltred_s,
    o_Ch2_OvrNotFiltred_p   => ChB_OvrNotFiltred_s,
    o_Ch2_OvrFiltred_p      => ChB_OvrFiltred_s
  );

  --SPI interface

  --SPI Cs Mapping
  -- 0:PLL  1:ADC  2:DAC  3:CLKDAC
  spi_l : entity work.spi
    port map(
    i_clk_p         => i_SystemClk_p,
    i_reset_p       => i_rst_p,
    --SPI interface
    i_spi_in_p      => SpiMisoIn_s,
    o_spi_out_p     => SpiMosi_s,
    --o_spi_od_p      => o_spi_od_p,
    o_spi_clk_p     => SpiClk_s,
    ov8_spi_sel_p   => SpiCs_s,
    --ov8_spi_sel2_p  => ov8_spi_sel2_p,
    -- Busy signal
    o_busy_p        => busy_s,
    --External module 1 interface
--     iv9_writeaddr1_p=> v9_ADCDACGainUpdaterWriteAddr_s,
--     iv9_readaddr1_p => "000000000",
--     i_req1_p        => ADCDACGainUpdaterReq_s,
--     iv32_din1_p     => v32_ADCDACGainUpdaterDout_s,
--     o_gnt1_p        => ADCDACGainUpdaterGnt_s,
--     ov32_dout1_p    => open,
--     o_ack1_p        => ADCDACGainUpdaterAck_s,
    --temp
    iv9_writeaddr1_p=> "000000000",
    iv9_readaddr1_p => "000000000",
    i_req1_p        => '0',
    iv32_din1_p     => X"00000000",
    o_gnt1_p        => open,
    ov32_dout1_p    => open,
    o_ack1_p        => open,
    --External module 2 interface
    iv9_writeaddr2_p => iv9_SpiWriteaddr_p,
    iv9_readaddr2_p => iv9_SpiReadaddr_p,
    i_req2_p        => i_SpiReq_p,
    iv32_din2_p     => iv32_SpiDin_p,
    o_gnt2_p        => o_SpiGnt_p,
    ov32_dout2_p    => ov32_SpiDout_p,
    o_ack2_p        => o_SpiAck_p,
--     --External module 3 interface
--     iv9_writeaddr3_p=> iv9_writeaddr3_p,
--     iv9_readaddr3_p => iv9_readaddr3_p,
--     i_req3_p        => i_req3_p,
--     iv32_din3_p     => iv32_din3_p,
--     o_gnt3_p        => o_gnt3_p,
--     ov32_dout3_p    => ov32_dout3_p,
--     o_ack3_p        => o_ack3_p,
--     --External module 4 interface
--     iv9_writeaddr4_p=> iv9_writeaddr4_p,
--     iv9_readaddr4_p => iv9_readaddr4_p,
--     i_req4_p        => i_req4_p,
--     iv32_din4_p     => iv32_din4_p,
--     o_gnt4_p        => o_gnt4_p,
--     ov32_dout4_p    => ov32_dout4_p,
--     o_ack4_p        => o_ack4_p

    --temp
    --External module 3 interface
    iv9_writeaddr3_p=> "000000000",
    iv9_readaddr3_p => "000000000",
    i_req3_p        => '0',
    iv32_din3_p     => X"00000000",
    o_gnt3_p        => open,
    ov32_dout3_p    => open,
    o_ack3_p        => open,
    --External module 4 interface
    iv9_writeaddr4_p=> "000000000",
    iv9_readaddr4_p => "000000000",
    i_req4_p        => '0',
    iv32_din4_p     => X"00000000",
    o_gnt4_p        => open,
    ov32_dout4_p    => open,
    o_ack4_p        => open
  );

-- ADCDACGainUpdater_l : ADCDACGainUpdater
-- generic map(
--         ADCChAOFFSetCorrectionTime  => ADCChAOFFSetCorrectionTime,
--         ADCChBOFFSetCorrectionTime  => ADCChBOFFSetCorrectionTime
--         )
-- port map(
--     --Global signals
--     i_clk_p                         =>  i_SystemClk_p,
--     i_rst_p                         =>  i_rst_p,
--     i_WriteCmd_p                    =>  i_WriteCmd_p,
--     --User ports
--     iv6_ADCChAFineGain_p            =>  iv6_ADCChAFineGain_p,
--     iv6_ADCChBFineGain_p            =>  iv6_ADCChBFineGain_p,
--     iv4_ADCChACoarseGain_p          =>  iv4_ADCChACoarseGain_p,
--     iv4_ADCChBCoarseGain_p          =>  iv4_ADCChBCoarseGain_p,
--     iv4_DACChAGain_p                =>  iv4_DACChAGain_p,
--     iv4_DACChBGain_p                =>  iv4_DACChBGain_p,
--     i_UpdateADCChAFineGain_p        =>  i_UpdateADCChAFineGain_p,
--     i_UpdateADCChBFineGain_p        =>  i_UpdateADCChBFineGain_p,
--     i_UpdateADCChACoarseGain_p      =>  i_UpdateADCChACoarseGain_p,
--     i_UpdateADCChBCoarseGain_p      =>  i_UpdateADCChBCoarseGain_p,
--     i_UpdateDACChAGain_p            =>  i_UpdateDACChAGain_p,
--     i_UpdateDACChBGain_p            =>  i_UpdateDACChBGain_p,
--     o_AckUpdateADCChAFineGain_p     =>  AckUpdateADCChAFineGain_s,
--     o_AckUpdateADCChBFineGain_p     =>  AckUpdateADCChBFineGain_s,
--     o_AckUpdateADCChACoarseGain_p   =>  AckUpdateADCChACoarseGain_s,
--     o_AckUpdateADCChBCoarseGain_p   =>  AckUpdateADCChBCoarseGain_s,
--     o_AckUpdateDACChAGain_p         =>  AckUpdateDACChAGain_s,
--     o_AckUpdateDACChBGain_p         =>  AckUpdateDACChBGain_s,
--     o_UpdaterBusy_p                 =>  UpdaterBusy_s,
--     --To SPI module ports
--       i_SPIBusy_p                     =>  busy_s,
--     ov9_writeaddr_p                 =>  v9_ADCDACGainUpdaterWriteAddr_s,
--     o_req_p                         =>  ADCDACGainUpdaterReq_s,
--     i_gnt_p                         =>  ADCDACGainUpdaterGnt_s,
--     ov32_dout_p                     =>  v32_ADCDACGainUpdaterDout_s,
--     i_ack_p                         =>  ADCDACGainUpdaterAck_s
--  );

  v2_AdcOvr_s <= ChB_OvrFiltred_s & ChA_OvrFiltred_s;
  v2_Adcudr_s <= "00";

  LedManager_l : entity work.LedManager
    port map(
      i_clk_p           => i_SystemClk_p,
      i_rst_p           => i_rst_p,
      i_AdcClkLock_p    => i_AdcClkLock_p,
      iv2_AdcRun_p      => iv2_AdcRun_p,
      iv2_AdcStart_p    => iv2_AdcStart_p,
      iv2_AdcOvr_p      => v2_AdcOvr_s,
      iv2_AdcUdr_p      => v2_Adcudr_s,
      i_DacClkLock_p    => DacDataClkLocked_s,
      i_PLLStatus_p     => PLLStatusIn_s,
      o_AdcALedRed_p    => AdcALedRed_s,
      o_AdcALedGreen_p  => AdcALedGreen_s,
      o_AdcBLedRed_p    => AdcBLedRed_s,
      o_AdcBLedGreen_p  => AdcBLedGreen_s,
      o_DacLedRed_p     => DacLedRed_s,
      o_DacLedGreen_p   => DacLedGreen_s,
      o_PllLedRed_p     => PllLedRed_s,
      o_PllLedGreen_p   => PllLedGreen_s
    );

  -- Output port assignement
  o_SpiBusy_p                       <= busy_s;
  o_ChA_ovrFiltred_p                <= ChA_ovrFiltred_s;
  o_ChA_ovrNotFiltred_p             <= ChA_ovrNotFiltred_s;
  o_ChB_ovrFiltred_p                <= ChB_ovrFiltred_s;
  o_ChB_ovrNotFiltred_p             <= ChB_ovrNotFiltred_s;
  o_DacRefDataClk_p                 <= DacRefDataClkB_s;
  o_DacDataClkLocked_p              <= DacDataClkLocked_s;
  o_Trigger_p                       <= Trigger_s;

  process(i_AdcDataClk_p)
  begin
    if rising_edge(i_AdcDataClk_p) then
      ov14_AdcDataChA_p                 <= v14_OvrDataChA_s;
      ov14_AdcDataChB_p                 <= v14_OvrDataChB_s;
    end if;
  end process;

  --mapping
  nDacReset_s <= not i_DacReset_p;

  --output mapping.
  o_PllStatus_p <= PLLStatusIn_s;

  --output IOBFF...
  process(i_SystemClk_p)
   begin
     if rising_edge(i_SystemClk_p) then
          PLLFunctionOut_s <= i_PllFunction_p;
          PLLStatusIn_s <= PLLStatusIO_s;
      end if;
  end process;

  --IOBUF Integration...
  OBUFADCSPIRST_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => AdcSpiResetOut_s,     -- Buffer output (connect directly to top-level port)
      I => i_AdcSpiReset_p      -- Buffer input
    );

  OBUFDACRESETN_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => nDacResetOut_s,     -- Buffer output (connect directly to top-level port)
      I => nDacReset_s      -- Buffer input
    );

  OBUFCLKMUXConf_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => clkMuxConfOut_s,     -- Buffer output (connect directly to top-level port)
      I => i_ClkMuxConfig_p      -- Buffer input
    );

  OBUFCLKMUXLOAD_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => clkMuxLoadOut_s,     -- Buffer output (connect directly to top-level port)
      I => i_ClkMuxLoad_p      -- Buffer input
    );

  OBUFCLKMUXSIN0_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => v2_ClkMuxSinOut_s(0),     -- Buffer output (connect directly to top-level port)
      I => iv2_ClkMuxSin_p(0)      -- Buffer input
    );

  OBUFCLKMUXSIN1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => v2_ClkMuxSinOut_s(1),     -- Buffer output (connect directly to top-level port)
      I => iv2_ClkMuxSin_p(1)      -- Buffer input
    );

  OBUFCLKMUXSOUT0_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => v2_ClkMuxSoutOut_s(0),     -- Buffer output (connect directly to top-level port)
      I => iv2_ClkMuxSout_p(0)      -- Buffer input
    );

  OBUFCLKMUXSout1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => v2_ClkMuxSoutOut_s(1),     -- Buffer output (connect directly to top-level port)
      I => iv2_ClkMuxSout_p(1)      -- Buffer input
    );

  OBUFPLLStatusOut1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => PLLFunctionIO_s,     -- Buffer output (connect directly to top-level port)
      I => PLLFunctionOut_s      -- Buffer input
    );

  OBUFSPIMOSIOut1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiMosiOut_s,     -- Buffer output (connect directly to top-level port)
      I => SpiMosi_s      -- Buffer input
    );

  OBUFSPICLKOut1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiClkOut_s,     -- Buffer output (connect directly to top-level port)
      I => SpiClk_s      -- Buffer input
    );

  OBUFSPICS0Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(0),     -- Buffer output (connect directly to top-level port)
      I => SpiCs_s(0)      -- Buffer input
    );

  OBUFSPICS1Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(1),     -- Buffer output (connect directly to top-level port)
      I => SpiCs_s(1)      -- Buffer input
    );

  OBUFSPICS2Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(2),     -- Buffer output (connect directly to top-level port)
      I => SpiCs_s(2)      -- Buffer input
    );

  OBUFSPICS3Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(3),     -- Buffer output (connect directly to top-level port)
      I => SpiCs_s(3)      -- Buffer input
    );

  IBUFPLLFunctin1_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => PLLStatusIO_s,
      I => iv3_CtrlExtBus_p(0)
    );

  IBUFMISOin1_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => SpiMisoIn_s,
      I => iv3_CtrlExtBus_p(1)
    );

  IBUFTrigin1_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => triggerIn_s,
      I => iv3_CtrlExtBus_p(2)
    );

  --------------------------------------------------------------
  -- DAC External Reference Clock buffering
  --------------------------------------------------------------
  IBUFGDSInst_l : IBUFGDS
    generic map (
      DIFF_TERM => TRUE,
        IOSTANDARD => "LVDS_25")
    port map (
        O  => DacRefDataClk_s,    -- Clock buffer output
        I  => DacClkP_s,          -- Diff_p clock buffer input
        IB => DacClkN_s           -- Diff_n clock buffer input
        );

  BUFG_inst : BUFG
    port map
    (
      O => DacRefDataClkB_s, -- 1-bit Clock buffer output
      I => DacRefDataClk_s -- 1-bit Clock buffer input
    );

  -------------------------------------------------------------------------------
  -- Trigger section
  -------------------------------------------------------------------------------
  Synchronizer_2Tv_Inst1 : entity work.Synchronizer_2Tv
    generic map (
      size => iv5_TriggerDelay_p'length
    )
    Port map (
      Clk           => i_AdcDataClk_p,
      Reset         => i_rst_p,
      Reset_value   => (others=>'0'),
      Async         => iv5_TriggerDelay_p,
      Sync          => v5_TriggerDelay_s
    );

  VariableDelayTrigger_INST: entity work.VariableDelayTrigger
    port map (
      i_clk_p     => i_AdcDataClk_p,
      iv5_Delay_p => v5_TriggerDelay_s,
      i_Trigger_p => triggerIn_s,
      o_Trigger_p => Trigger_s);

  -------------------------------------------------------------------------------
  -- Frequency counter section
  -------------------------------------------------------------------------------
  v4_TestClks_s(0) <= AdcClkBufr_s;
  v4_TestClks_s(1) <= i_AdcDataClk_p;
  v4_TestClks_s(2) <= DacRefDataClkB_s;
  v4_TestClks_s(3) <= i_DacDesignClk_p;

  FrequencyCounterMux_inst : entity work.FrequencyCounterMux
    generic map(
      C_REFCLK_FREQ_MHZ  => 100,                  -- AXI 100MHz
      C_NUM_OF_TEST_CLK  => 4,
      C_SEL_WIDTH        => 2
    )
    port map(
      i_Rst_p          => i_rst_p,
      i_RefClk_p       => i_SystemClk_p,
      iv_TestClks_p    => v4_TestClks_s,          -- Freqs to be measured
      iv_TestClkSel_p => iv2_FreqCntClkSel_p,     -- select the clock to measure
      ov16_Freq_p      => ov16_FreqCntClkCnt_p    -- measured freq value in MHz
    );



end arch;