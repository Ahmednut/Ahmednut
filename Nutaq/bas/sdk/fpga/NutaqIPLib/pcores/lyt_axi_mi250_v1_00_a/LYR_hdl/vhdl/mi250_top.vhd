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
-- File        : $Id: mi250_top.vhd,v 1.3 2016/01/26 13:58:43 christian.dufour-bou Exp $
--------------------------------------------------------------------------------
-- Description : MI 250 Wrapper
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: mi250_top.vhd,v $
-- Revision 1.3  2016/01/26 13:58:43  christian.dufour-bou
-- MI250 channels names changed to 1 - 8 instead of A - H and trigger no longer managed by the MI250 core
--
-- Revision 1.2  2015/06/10 12:48:36  julien.roy
-- Add bitslip functionnality to have more possibility to find a valid calibration.
-- Add clock master/slave logic.
-- Move constraints directly inside the .tcl instead of external .ucf files.
--
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.8  2013/04/12 14:44:38  julien.roy
-- Disable "keep_hierarchy"
--
-- Revision 1.7  2013/02/15 17:31:25  julien.roy
-- Modification for dynamic calibration
--
-- Revision 1.6  2013/02/01 16:07:58  julien.roy
-- Small modifications
--
-- Revision 1.5  2012/11/20 13:58:58  khalid.bensadek
-- Commented-out the pattern error signals for now, it is unused for now and just causing timing errors.
--
-- Revision 1.4  2012/11/15 21:44:15  julien.roy
-- Modification to the mi250 core
-- - remove fifos
-- - add pattern verification
--
-- Revision 1.2  2012/11/12 19:34:16  khalid.bensadek
-- Optimized design to use one MMCM only instead of 4. That way, we can use Record-playback ip core with it.
--
-- Revision 1.1  2012/11/08 15:42:11  khalid.bensadek
-- Updated the ipcore to AXI bus version. Working version as is with 4 MMCMs that will conflect if adding Record-Playback ipcore.
--
-- Revision 1.7  2011/05/26 20:00:33  jeffrey.johnson
-- Added ADC data clock output.
-- Changed data output to 14 bits.
-- Removed over range registers.
--
-- Revision 1.6  2011/05/25 20:36:54  jeffrey.johnson
-- Removed 16 to 64 bit FIFO.
--
-- Revision 1.5  2011/05/25 16:12:30  jeffrey.johnson
-- Moved MMCM from wrapper to clk_module.
-- Added freq counters for all clocks.
--
-- Revision 1.4  2011/05/19 18:27:52  jeffrey.johnson
-- Added ADC interface.
--
-- Revision 1.3  2011/05/12 15:15:23  jeffrey.johnson
-- Separated mi250_regs from top level design.
--
-- Revision 1.2  2011/05/11 13:34:45  jeffrey.johnson
-- SPI fixed.
--
-- Revision 1.1  2011/05/09 16:21:44  jeffrey.johnson
-- First commit.
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;


library unisim;
  use unisim.vcomponents.all;


entity mi250_top is
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
end entity mi250_top;

architecture arch of mi250_top is

  -- attribute keep_hierarchy : string;                         
  -- attribute keep_hierarchy of arch : architecture is "true";

  -- Signal declaration

  -- ADC signals
  signal AdcSpiResetOut_s          : std_logic;
  
  -- PLL signals
  signal PLLStatusIO_s             : std_logic;
  signal PLLStatusIn_s             : std_logic;
  signal PLLFunctionOut_s          : std_logic;
  signal PLLFunctionIO_s           : std_logic;
  
  -- SPI signals
  signal SpiBusy_s                 : std_logic;
  signal SpiClk_s                  : std_logic;
  signal SpiMosi_s                 : std_logic;
  signal SpiMiso_s                 : std_logic;
  signal SpiCs_s                   : std_logic_vector(7 downto 0);
  signal SpiCsOut_s                : std_logic_vector(7 downto 0);
  signal Mi250AdcScl_s             : std_logic;
  signal Mi250AdcMosi_s            : std_logic;
  signal Mi250PllScl_s             : std_logic;
  signal Mi250PllMosi_s            : std_logic;

  -- MISO signals
  signal Mi250AdcMiso12_s          : std_logic;
  signal Mi250AdcMiso34_s          : std_logic;
  signal Mi250AdcMiso56_s          : std_logic;
  signal Mi250AdcMiso78_s          : std_logic;
  signal Mi250PllMiso_s            : std_logic;

  
begin

  ----------------------------------------------------------------------------------------------------
  -- MI250 Channel control
  ----------------------------------------------------------------------------------------------------
  Mi250Ctrl_inst : entity work.mi250_ctrl
  port map (
    i_Rst_p           => i_Rst_p,    
    i_RefClk200MHz_p  => i_RefClk200MHz_p,
    i_SystemClk_p     => i_SystemClk_p,
    
    -- Control registers
    i_ChArmed_p          => i_ChArmed_p,    
  
    iv5_Adc12IdelayValue_p      => iv5_Adc12IdelayValue_p,
    iv5_Adc34IdelayValue_p      => iv5_Adc34IdelayValue_p,
    iv5_Adc56IdelayValue_p      => iv5_Adc56IdelayValue_p,
    iv5_Adc78IdelayValue_p      => iv5_Adc78IdelayValue_p,
    iv5_Adc12ClkIdelayValue_p   => iv5_Adc12ClkIdelayValue_p,
    iv5_Adc34ClkIdelayValue_p   => iv5_Adc34ClkIdelayValue_p,
    iv5_Adc56ClkIdelayValue_p   => iv5_Adc56ClkIdelayValue_p,
    iv5_Adc78ClkIdelayValue_p   => iv5_Adc78ClkIdelayValue_p,
  
    i_Adc12BitSlip_p            => i_Adc12BitSlip_p,
    i_Adc34BitSlip_p            => i_Adc34BitSlip_p,
    i_Adc56BitSlip_p            => i_Adc56BitSlip_p,
    i_Adc78BitSlip_p            => i_Adc78BitSlip_p,
  
    ov2_Adc12PatternError_p => ov2_Adc12PatternError_p,
    ov2_Adc34PatternError_p => ov2_Adc34PatternError_p,
    ov2_Adc56PatternError_p => ov2_Adc56PatternError_p,
    ov2_Adc78PatternError_p => ov2_Adc78PatternError_p,
  
    -- ADC Data outputs
    ov14_Adc1DataOut_p  => ov14_Mi250Adc1DataOut_p,
    ov14_Adc2DataOut_p  => ov14_Mi250Adc2DataOut_p,
    ov14_Adc3DataOut_p  => ov14_Mi250Adc3DataOut_p,
    ov14_Adc4DataOut_p  => ov14_Mi250Adc4DataOut_p,
    ov14_Adc5DataOut_p  => ov14_Mi250Adc5DataOut_p,
    ov14_Adc6DataOut_p  => ov14_Mi250Adc6DataOut_p,
    ov14_Adc7DataOut_p  => ov14_Mi250Adc7DataOut_p,
    ov14_Adc8DataOut_p  => ov14_Mi250Adc8DataOut_p,
    
    -- ADC Data valid outputs
    o_Adc1DataValid_p   => o_Mi250Adc1DataValid_p,
    o_Adc2DataValid_p   => o_Mi250Adc2DataValid_p,
    o_Adc3DataValid_p   => o_Mi250Adc3DataValid_p,
    o_Adc4DataValid_p   => o_Mi250Adc4DataValid_p,
    o_Adc5DataValid_p   => o_Mi250Adc5DataValid_p,
    o_Adc6DataValid_p   => o_Mi250Adc6DataValid_p,
    o_Adc7DataValid_p   => o_Mi250Adc7DataValid_p,
    o_Adc8DataValid_p   => o_Mi250Adc8DataValid_p,
  
    clk_12_p          => idp_Mi250AdcClk12_p,
    clk_12_n          => idn_Mi250AdcClk12_p,
    ch1_p             => iv7dp_Mi250Adc1Data_p,
    ch1_n             => iv7dn_Mi250Adc1Data_p,
    ch2_p             => iv7dp_Mi250Adc2Data_p,
    ch2_n             => iv7dn_Mi250Adc2Data_p,
    clk_34_p          => idp_Mi250AdcClk34_p,
    clk_34_n          => idn_Mi250AdcClk34_p,
    ch3_p             => iv7dp_Mi250Adc3Data_p,
    ch3_n             => iv7dn_Mi250Adc3Data_p,
    ch4_p             => iv7dp_Mi250Adc4Data_p,
    ch4_n             => iv7dn_Mi250Adc4Data_p,
    clk_56_p          => idp_Mi250AdcClk56_p,
    clk_56_n          => idn_Mi250AdcClk56_p,
    ch5_p             => iv7dp_Mi250Adc5Data_p,
    ch5_n             => iv7dn_Mi250Adc5Data_p,
    ch6_p             => iv7dp_Mi250Adc6Data_p,
    ch6_n             => iv7dn_Mi250Adc6Data_p,
    clk_78_p          => idp_Mi250AdcClk78_p,
    clk_78_n          => idn_Mi250AdcClk78_p,
    ch7_p             => iv7dp_Mi250Adc7Data_p,
    ch7_n             => iv7dn_Mi250Adc7Data_p,
    ch8_p             => iv7dp_Mi250Adc8Data_p,
    ch8_n             => iv7dn_Mi250Adc8Data_p,
    -- ADC Clock outputs (from BUFR)
    o_Adc12ClkBufr_p  => o_Adc12ClkBufr_p,
    o_Adc34ClkBufr_p  => o_Adc34ClkBufr_p,
    o_Adc56ClkBufr_p  => o_Adc56ClkBufr_p,
    o_Adc78ClkBufr_p  => o_Adc78ClkBufr_p,
    -- ADC Global clocks (from BUFG)
    i_AdcClkBufg_p    => i_AdcClkBufg_p
  );

  -- SPI shared ADC
  o_Mi250AdcScl_p          <= Mi250AdcScl_s;
  o_Mi250AdcMosi_p         <= Mi250AdcMosi_s;
  o_nMi250AdcCs12_p        <= SpiCsOut_s(0);    -- ADC channels 12 CS 
  o_nMi250AdcCs34_p        <= SpiCsOut_s(1);    -- ADC channels 34 CS   
  o_nMi250AdcCs56_p        <= SpiCsOut_s(2);    -- ADC channels 56 CS  
  o_nMi250AdcCs78_p        <= SpiCsOut_s(3);    -- ADC channels 78 CS

  -- SPI PLL
  o_Mi250PllScl_p          <= Mi250PllScl_s;
  o_Mi250PllMosi_p         <= Mi250PllMosi_s;
  o_nMi250PllCs_p          <= SpiCsOut_s(4);    -- PLL CS
  
  -- ADC shared reset
  o_Mi250AdcRst_p          <= AdcSpiResetOut_s;

  -- MISO MUX
  MisoMux_l: process (SpiCs_s,Mi250AdcMiso12_s,Mi250AdcMiso34_s,Mi250AdcMiso56_s,Mi250AdcMiso78_s,Mi250PllMiso_s) 
  begin 
    case SpiCs_s is  
      when "11111110"  => SpiMiso_s <= Mi250AdcMiso12_s;  
      when "11111101"  => SpiMiso_s <= Mi250AdcMiso34_s;  
      when "11111011"  => SpiMiso_s <= Mi250AdcMiso56_s;  
      when "11110111"  => SpiMiso_s <= Mi250AdcMiso78_s;  
      when "11101111"  => SpiMiso_s <= Mi250PllMiso_s;
      when others      => SpiMiso_s <= '0';  
    end case;
  end process MisoMux_l; 
  
  -- SPI Chip select Mapping
  -- 0:ADC_12  1:ADC_34  2:ADC_56  3:ADC_78  4:PLL
  spi_l : entity work.spi
  port map(
      i_clk_p         => i_SystemClk_p,
      i_reset_p       => i_Rst_p,
      --SPI interface
      i_spi_in_p      => SpiMiso_s,
      o_spi_out_p     => SpiMosi_s,
      o_spi_clk_p     => SpiClk_s,
      ov8_spi_sel_p   => SpiCs_s,
      -- Busy signal
      o_busy_p        => SpiBusy_s,
      --External module 1 interface
      iv9_writeaddr1_p=> "000000000",
      iv9_readaddr1_p => "000000000",
      i_req1_p        => '0',
      iv32_din1_p     => X"00000000",
      o_gnt1_p        => open,
      ov32_dout1_p    => open,
      o_ack1_p        => open,
      --External module 2 interface
      iv9_writeaddr2_p=> iv9_writeaddr2_p,
      iv9_readaddr2_p => iv9_readaddr2_p,
      i_req2_p        => i_req2_p,
      iv32_din2_p     => iv32_din2_p,
      o_gnt2_p        => o_gnt2_p,
      ov32_dout2_p    => ov32_dout2_p,
      o_ack2_p        => o_ack2_p,
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

  --IOBUF Integration...
  OBUFADCSPIRST_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => AdcSpiResetOut_s,
      I => i_AdcSpiReset_p  
    );
  
  ------------------------------------------------------
  -- OBUFs for SPI MOSI
  ------------------------------------------------------
  OBUFAdcSpiMosi_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => Mi250AdcMosi_s,
      I => SpiMosi_s    
    );
  
  OBUFPllSpiMosiOut1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => Mi250PllMosi_s,
      I => SpiMosi_s
    );
  
  ------------------------------------------------------
  -- OBUFs for SPI i_SystemClk_p out
  ------------------------------------------------------
  OBUFAdcSpiClkOut_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => Mi250AdcScl_s,
      I => SpiClk_s    
    );
  
  OBUFPllSpiClkOut_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => Mi250PllScl_s,
      I => SpiClk_s
    );
  
  ------------------------------------------------------
  -- OBUFs for SPI chip selects
  ------------------------------------------------------
  OBUFSPICS0Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(0),
      I => SpiCs_s(0)    
    );
  
  OBUFSPICS1Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(1),
      I => SpiCs_s(1)    
    );

  OBUFSPICS2Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(2),
      I => SpiCs_s(2)    
    );

  OBUFSPICS3Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(3),
      I => SpiCs_s(3)    
    );

  OBUFSPICS4Out1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => SpiCsOut_s(4),
      I => SpiCs_s(4)    
    );

  ------------------------------------------------------
  -- IOBUFs for PLL
  ------------------------------------------------------
  IBUFPllFunction_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => PLLStatusIO_s,
      I => i_Mi250PllStatus_p
    );
  
  OBUFPLLStatusOut1_inst : OBUF
    generic map (
      IOSTANDARD => "LVCMOS25",
      SLEW => "SLOW")
    port map (
      O => PLLFunctionIO_s,
      I => PLLFunctionOut_s
    );
  
  process(i_SystemClk_p)
  begin
    if rising_edge(i_SystemClk_p) then
      PLLFunctionOut_s <= i_PllFunction_p;
      PLLStatusIn_s <= PLLStatusIO_s;
    end if;
  end process;
  
  ------------------------------------------------------
  -- IBUFs for MISO
  ------------------------------------------------------
  IBUFMi250AdcMiso12_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => Mi250AdcMiso12_s,
      I => i_Mi250AdcMiso12_p
    );

  IBUFMi250AdcMiso34_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => Mi250AdcMiso34_s,
      I => i_Mi250AdcMiso34_p
    );

  IBUFMi250AdcMiso56_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => Mi250AdcMiso56_s,
      I => i_Mi250AdcMiso56_p
    );

  IBUFMi250AdcMiso78_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => Mi250AdcMiso78_s,
      I => i_Mi250AdcMiso78_p
    );

  IBUFMi250PllMiso_inst : IBUF
    generic map (
      IOSTANDARD => "LVCMOS25")
    port map (
      O => Mi250PllMiso_s,
      I => i_Mi250PllMiso_p
    );

    
  -- Output mapping
  o_Mi250PllFunction_p <= PLLFunctionIO_s;
  o_SpiBusy_p          <= SpiBusy_s;
  o_PllStatus_p        <= PLLStatusIn_s;
  
    
end arch;
