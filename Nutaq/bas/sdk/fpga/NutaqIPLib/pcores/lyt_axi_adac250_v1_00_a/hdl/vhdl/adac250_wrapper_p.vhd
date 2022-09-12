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
-- File        : $Id: adac250_wrapper_p.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
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
-- $Log: adac250_wrapper_p.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.11  2015/02/23 15:43:57  julien.roy
-- Remove PPS port since it is the same signal than trigger
--
-- Revision 1.10  2013/09/06 17:14:32  khalid.bensadek
-- Added pps out port
--
-- Revision 1.9  2013/01/24 17:38:19  julien.roy
-- Commit new wrapper for adac250 axi core
--
-- Revision 1.8  2010/12/07 19:17:52  patrick.gilbert
-- fix new fmc port definition
--
-- Revision 1.7  2010/10/29 15:53:31  jeffrey.johnson
-- Added DAC design clock input.
--
-- Revision 1.6  2010/09/20 17:43:04  patrick.gilbert
-- fix dacdatasnc
--
-- Revision 1.5  2010/09/17 16:51:39  patrick.gilbert
-- fix new wrapper port - new mmcm + new dac mux
--
-- Revision 1.4  2010/08/27 17:19:37  francois.blackburn
-- remove mmcm for the adc_interface to allow parameters modifications
--
-- Revision 1.3  2010/08/03 19:23:26  francois.blackburn
-- add mix divider
--
-- Revision 1.2  2010/07/29 14:26:52  francois.blackburn
-- add another dds
--
-- Revision 1.1  2010/06/17 15:40:15  francois.blackburn
-- first commit
--
-- Revision 1.2  2010/02/10 15:00:49  patrick.gilbert
-- working ADC DAC PLL
--
-- Revision 1.1  2010/01/14 22:46:10  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/09/30 15:35:58  karim.ouadid
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

Package adac250_wrapper_p is

component adac250_wrapper is
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
end component;

end adac250_wrapper_p;