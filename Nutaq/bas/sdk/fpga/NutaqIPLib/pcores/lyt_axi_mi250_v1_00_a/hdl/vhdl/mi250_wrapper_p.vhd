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
-- File        : $Id: mi250_wrapper_p.vhd,v 1.3 2016/01/26 13:58:43 christian.dufour-bou Exp $
--------------------------------------------------------------------------------
-- Description : MI 250 Package
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: mi250_wrapper_p.vhd,v $
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
-- Revision 1.10  2013/02/18 13:21:38  julien.roy
-- New mi250 wrapper for v2.1
--
-- Revision 1.9  2012/11/20 14:16:35  khalid.bensadek
-- Cleaned-up unused signals.
--
-- Revision 1.8  2012/11/15 21:44:15  julien.roy
-- Modification to the mi250 core
-- - remove fifos
-- - add pattern verification
--
-- Revision 1.7  2012/11/15 16:48:06  julien.roy
-- New wrapper for the mi250_top without fifos
--
-- Revision 1.6  2012/11/13 19:21:23  khalid.bensadek
-- Updated the ipcore to AXI bus version. Working version as is with 4 MMCMs that will conflect if adding Record-Playback ipcore.
--
-- Revision 1.5  2011/06/13 16:13:15  jeffrey.johnson
-- ADC data outputs to 14 bit wide.
--
-- Revision 1.4  2011/05/25 16:21:20  jeffrey.johnson
-- Moved MMCM from wrapper to clk_module.
--
-- Revision 1.3  2011/05/19 19:20:22  jeffrey.johnson
-- Added ADC interface.
--
-- Revision 1.2  2011/05/12 15:16:45  jeffrey.johnson
-- Reference enable removed.
--
-- Revision 1.1  2011/05/10 13:16:39  jeffrey.johnson
-- First commit.
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

Package mi250_wrapper_p is

component mi250_top is
port
(
  -- Global ports
  i_SystemClk_p                : in std_logic;
  i_RefClk200MHz_p             : in std_logic;
  i_Rst_p                      : in std_logic;
  
  -- SPI PLL
  o_Mi250PllScl_p              : out std_logic;
  o_Mi250PllMosi_p             : out std_logic;
  i_Mi250PllMiso_p             : in  std_logic;
  o_nMi250PllCs_p              : out std_logic;

  -- SPI shared ADC                               
  o_Mi250AdcScl_p              : out std_logic;
  o_Mi250AdcMosi_p             : out std_logic;
  i_Mi250AdcMiso12_p           : in  std_logic;
  i_Mi250AdcMiso34_p           : in  std_logic;
  i_Mi250AdcMiso56_p           : in  std_logic;
  i_Mi250AdcMiso78_p           : in  std_logic;
  o_nMi250AdcCs12_p            : out std_logic;
  o_nMi250AdcCs34_p            : out std_logic;
  o_nMi250AdcCs56_p            : out std_logic;
  o_nMi250AdcCs78_p            : out std_logic;
  o_Mi250AdcRst_p              : out std_logic;

  -- PLL control                               
  i_Mi250PllStatus_p           : in std_logic;
  o_Mi250PllFunction_p         : out std_logic;

  -- Monitor interrupt                               
  i_nMi250MonInt_p             : in  std_logic;
  
  -- ADC interface clocks
  idp_Mi250AdcClk12_p          : in std_logic;
  idn_Mi250AdcClk12_p          : in std_logic;
  idp_Mi250AdcClk34_p          : in std_logic;
  idn_Mi250AdcClk34_p          : in std_logic;
  idp_Mi250AdcClk56_p          : in std_logic;
  idn_Mi250AdcClk56_p          : in std_logic;
  idp_Mi250AdcClk78_p          : in std_logic;
  idn_Mi250AdcClk78_p          : in std_logic;
  
  -- ADC interface data
  iv7dp_Mi250Adc1Data_p        : in std_logic_vector(6 downto 0);
  iv7dn_Mi250Adc1Data_p        : in std_logic_vector(6 downto 0);
  iv7dp_Mi250Adc2Data_p        : in std_logic_vector(6 downto 0);
  iv7dn_Mi250Adc2Data_p        : in std_logic_vector(6 downto 0);
  iv7dp_Mi250Adc3Data_p        : in std_logic_vector(6 downto 0);
  iv7dn_Mi250Adc3Data_p        : in std_logic_vector(6 downto 0);
  iv7dp_Mi250Adc4Data_p        : in std_logic_vector(6 downto 0);
  iv7dn_Mi250Adc4Data_p        : in std_logic_vector(6 downto 0);
  iv7dp_Mi250Adc5Data_p        : in std_logic_vector(6 downto 0);
  iv7dn_Mi250Adc5Data_p        : in std_logic_vector(6 downto 0);
  iv7dp_Mi250Adc6Data_p        : in std_logic_vector(6 downto 0);
  iv7dn_Mi250Adc6Data_p        : in std_logic_vector(6 downto 0);
  iv7dp_Mi250Adc7Data_p        : in std_logic_vector(6 downto 0);
  iv7dn_Mi250Adc7Data_p        : in std_logic_vector(6 downto 0);
  iv7dp_Mi250Adc8Data_p        : in std_logic_vector(6 downto 0);
  iv7dn_Mi250Adc8Data_p        : in std_logic_vector(6 downto 0);
  
  iv5_Adc12IdelayValue_p        : in  std_logic_vector(4 downto 0);
  iv5_Adc34IdelayValue_p        : in  std_logic_vector(4 downto 0);
  iv5_Adc56IdelayValue_p        : in  std_logic_vector(4 downto 0);
  iv5_Adc78IdelayValue_p        : in  std_logic_vector(4 downto 0);
  iv5_Adc12ClkIdelayValue_p     : in  std_logic_vector(4 downto 0);
  iv5_Adc34ClkIdelayValue_p     : in  std_logic_vector(4 downto 0);
  iv5_Adc56ClkIdelayValue_p     : in  std_logic_vector(4 downto 0);
  iv5_Adc78ClkIdelayValue_p     : in  std_logic_vector(4 downto 0);
  
  i_Adc12BitSlip_p              : in  std_logic;
  i_Adc34BitSlip_p              : in  std_logic;
  i_Adc56BitSlip_p              : in  std_logic;
  i_Adc78BitSlip_p              : in  std_logic;
    
  ov2_Adc12PatternError_p       : out std_logic_vector(1 downto 0);
  ov2_Adc34PatternError_p       : out std_logic_vector(1 downto 0);
  ov2_Adc56PatternError_p       : out std_logic_vector(1 downto 0);
  ov2_Adc78PatternError_p       : out std_logic_vector(1 downto 0);
    
  -- ADC interface control
  i_ChArmed_p                  : in std_logic;
  iv2_TestMode_p               : in std_logic_vector(1 downto 0);
  
  -- ADC data outputs
  ov14_Mi250Adc1DataOut_p      : out std_logic_vector(13 downto 0);
  ov14_Mi250Adc2DataOut_p      : out std_logic_vector(13 downto 0);
  ov14_Mi250Adc3DataOut_p      : out std_logic_vector(13 downto 0);
  ov14_Mi250Adc4DataOut_p      : out std_logic_vector(13 downto 0);
  ov14_Mi250Adc5DataOut_p      : out std_logic_vector(13 downto 0);
  ov14_Mi250Adc6DataOut_p      : out std_logic_vector(13 downto 0);
  ov14_Mi250Adc7DataOut_p      : out std_logic_vector(13 downto 0);
  ov14_Mi250Adc8DataOut_p      : out std_logic_vector(13 downto 0);
  
  -- ADC data valid outputs
  o_Mi250Adc1DataValid_p       : out std_logic;
  o_Mi250Adc2DataValid_p       : out std_logic;
  o_Mi250Adc3DataValid_p       : out std_logic;
  o_Mi250Adc4DataValid_p       : out std_logic;
  o_Mi250Adc5DataValid_p       : out std_logic;
  o_Mi250Adc6DataValid_p       : out std_logic;
  o_Mi250Adc7DataValid_p       : out std_logic;
  o_Mi250Adc8DataValid_p       : out std_logic;
  
  -- ADC Clock outputs (from BUFR)
  o_Adc12ClkBufr_p             : out std_logic;
  o_Adc34ClkBufr_p             : out std_logic;
  o_Adc56ClkBufr_p             : out std_logic;
  o_Adc78ClkBufr_p             : out std_logic;
  
  -- ADC Global clocks (from BUFG)
  i_AdcClkBufg_p                : in std_logic;
    
  o_SpiBusy_p                  : out std_logic;
  
  -- External module 2 interface
  iv9_writeaddr2_p             : in  std_logic_vector (8 downto 0);
  iv9_readaddr2_p              : in  std_logic_vector (8 downto 0);
  i_req2_p                     : in  std_logic;
  iv32_din2_p                  : in  std_logic_vector (31 downto 0);
  o_gnt2_p                     : out std_logic;
  ov32_dout2_p                 : out std_logic_vector (31 downto 0);
  o_ack2_p                     : out std_logic;
  
  -- From/To custom logic
  i_AdcSpiReset_p              : in std_logic;
  o_PllStatus_p                : out std_logic;
  i_PllFunction_p              : in std_logic
);
end component;

end mi250_wrapper_p;