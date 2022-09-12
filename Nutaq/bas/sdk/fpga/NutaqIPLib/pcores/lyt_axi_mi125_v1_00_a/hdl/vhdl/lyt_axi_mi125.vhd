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
-- File        : $Id: lyt_axi_mi125.vhd,v 1.3 2015/06/03 18:13:39 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : MI 125 AXI Peripheral Top Module
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: lyt_axi_mi125.vhd,v $
-- Revision 1.3  2015/06/03 18:13:39  julien.roy
-- Add input clock delay control
--
-- Revision 1.2  2015/05/20 13:58:39  julien.roy
-- Merge modifications from Perseus611X branch
--
-- Revision 1.6  2014/04/10 15:24:29  julien.roy
-- Set valid signal to 0 when reset
--
-- Revision 1.5  2013/08/08 15:33:26  julien.roy
-- Change calibration from FPGA to software
--
-- Revision 1.4  2013/04/11 13:30:47  julien.roy
-- Disable "keep_hierarchy"
--
-- Revision 1.3  2012/12/10 14:30:38  julien.roy
-- Modify ucf to support the 4 FPGA types
-- Add chip enable status ports
-- Add variable delay trigger
-- Move frequency counter status into core registers
--
-- Revision 1.2  2012/10/30 18:18:42  david.quinn
-- New alignement FSM to take into account one lane decision for the remaining lanes.
--
-- Revision 1.1  2012/10/16 13:17:50  khalid.bensadek
-- First commit of a working version of this project
--
--------------------------------------------------------------------------------

library unisim;
use unisim.vcomponents.all;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

use work.mi125_pkg.all;

library lyt_axi_mi125_v1_00_a;
use lyt_axi_mi125_v1_00_a.all;

-------------------------------------------------------------------------------
-- Entity Section
-------------------------------------------------------------------------------
entity lyt_axi_mi125 is
    generic
    (
        -- ADD USER GENERICS BELOW THIS LINE ---------------
        C_BUILD_REVISION              : std_logic_vector      := X"0000";
        C_PRIMARY_MODULE              : boolean               := true;
        C_FMC_POSITION                : integer               := 0;
        -- ADD USER GENERICS ABOVE THIS LINE ---------------
        -- DO NOT EDIT BELOW THIS LINE ---------------------
        -- Bus protocol parameters, do not add to or delete
        C_S_AXI_DATA_WIDTH             : integer              := 32;
        C_S_AXI_ADDR_WIDTH             : integer              := 32;
        C_S_AXI_MIN_SIZE               : std_logic_vector     := X"000001FF";
        C_USE_WSTRB                    : integer              := 0;
        C_DPHASE_TIMEOUT               : integer              := 8;
        C_BASEADDR                     : std_logic_vector     := X"FFFFFFFF";
        C_HIGHADDR                     : std_logic_vector     := X"00000000";
        C_FAMILY                       : string               := "virtex6"
        -- DO NOT EDIT ABOVE THIS LINE ---------------------
    );
    port
    (

        i_logicRst_p                   : in std_logic;
        i_RefClk200MHz_p               : in std_logic;

        idp_DataFromADC_p              : in std_logic_vector(31 DOWNTO 0);
        idn_DataFromADC_p              : in std_logic_vector(31 DOWNTO 0);
        idp_ClockFromADC_p             : in std_logic;
        idn_ClockFromADC_p             : in std_logic;
        idp_FrameFromADC_p             : in std_logic;
        idn_FrameFromADC_p             : in std_logic;
        i_TriggerInFromIO_p            : in std_logic;
        o_TriggerOutToIO_p             : out std_logic;

        ---- Inter MI125 connections ------------------------------
        i_IPSerialClockIn_p            : IN std_logic;
        i_IPSerialClockInDiv_p         : IN std_logic;
        o_IPSerialClockOut_p           : OUT std_logic;
        o_IPSerialClockOutDiv_p        : OUT std_logic;

        ---- Data ports -------------------------------------------
        o_AdcDataClk_p                 : out std_logic;
        ov14_AdcDataCh1_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh1Valid_p                : out std_logic;
        ov14_AdcDataCh2_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh2Valid_p                : out std_logic;
        ov14_AdcDataCh3_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh3Valid_p                : out std_logic;
        ov14_AdcDataCh4_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh4Valid_p                : out std_logic;
        ov14_AdcDataCh5_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh5Valid_p                : out std_logic;
        ov14_AdcDataCh6_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh6Valid_p                : out std_logic;
        ov14_AdcDataCh7_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh7Valid_p                : out std_logic;
        ov14_AdcDataCh8_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh8Valid_p                : out std_logic;
        ov14_AdcDataCh9_p              : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh9Valid_p                : out std_logic;
        ov14_AdcDataCh10_p             : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh10Valid_p               : out std_logic;
        ov14_AdcDataCh11_p             : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh11Valid_p               : out std_logic;
        ov14_AdcDataCh12_p             : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh12Valid_p               : out std_logic;
        ov14_AdcDataCh13_p             : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh13Valid_p               : out std_logic;
        ov14_AdcDataCh14_p             : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh14Valid_p               : out std_logic;
        ov14_AdcDataCh15_p             : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh15Valid_p               : out std_logic;
        ov14_AdcDataCh16_p             : out std_logic_vector (13 DOWNTO 0);
        o_AdcCh16Valid_p               : out std_logic;

        o_AdcCh1to4Enabled_p           : out std_logic;
        o_AdcCh5to8Enabled_p           : out std_logic;
        o_AdcCh9to12Enabled_p          : out std_logic;
        o_AdcCh13to16Enabled_p         : out std_logic;

        i_TriggerFromFPGA_p            : in std_logic;
        o_TriggerToFPGA_p              : out std_logic;
        o_DataFormat_p                 : out std_logic;

        -- DO NOT EDIT BELOW THIS LINE ---------------------
        -- Bus protocol ports, do not add to or delete
        S_AXI_ACLK                     : in  std_logic;
        S_AXI_ARESETN                  : in  std_logic;
        S_AXI_AWADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
        S_AXI_AWVALID                  : in  std_logic;
        S_AXI_WDATA                    : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
        S_AXI_WSTRB                    : in  std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
        S_AXI_WVALID                   : in  std_logic;
        S_AXI_BREADY                   : in  std_logic;
        S_AXI_ARADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
        S_AXI_ARVALID                  : in  std_logic;
        S_AXI_RREADY                   : in  std_logic;
        S_AXI_ARREADY                  : out std_logic;
        S_AXI_RDATA                    : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
        S_AXI_RRESP                    : out std_logic_vector(1 downto 0);
        S_AXI_RVALID                   : out std_logic;
        S_AXI_WREADY                   : out std_logic;
        S_AXI_BRESP                    : out std_logic_vector(1 downto 0);
        S_AXI_BVALID                   : out std_logic;
        S_AXI_AWREADY                  : out std_logic
        -- DO NOT EDIT ABOVE THIS LINE ---------------------
    );

    attribute MAX_FANOUT                     : string;
    attribute SIGIS                          : string;
    attribute MAX_FANOUT of S_AXI_ACLK       : signal is "10000";
    attribute MAX_FANOUT of S_AXI_ARESETN    : signal is "10000";
    attribute SIGIS of S_AXI_ACLK            : signal is "Clk";
    attribute SIGIS of S_AXI_ARESETN         : signal is "Rst";

end entity lyt_axi_mi125;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of lyt_axi_mi125 is

    signal TriggerInFromIO_s          : std_logic;

    signal SerialClock_s              : std_logic;
    signal DataClock_s                : std_logic;

    signal IPSerialClockIn_s          : std_logic;
    signal IPSerialClockInDiv_s       : std_logic;

    signal UpdateADCStatus_s          : std_logic;
    signal IdelayRst_s                : std_logic;
    signal v5_TriggerDelay_s          : std_logic_vector(4 downto 0);
    signal reset_calib_detection_s    : std_logic;
    signal IdelayCtrlRst_s            : std_logic;
    signal IserdesRst_s               : std_logic;
    signal v16_AdcValid_s             : std_logic_vector(15 downto 0);
    signal DigOutRandEn_s             : std_logic;
    signal IPSoftRst_s                : std_logic;
    signal v2_ChannelConfig_s         : std_logic_vector(1 downto 0);
    signal DataFormat_s               : std_logic;
    signal ADCClockMMCMRst_s          : std_logic;
    signal idelay_ready_s             : std_logic;
    signal calib_detection_done_s     : std_logic;
    signal ADCClockMMCMLocked_s       : std_logic;
    signal ADCClockMMCMPresent_s      : std_logic;
    signal v5_adcIdelay_value_s       : std_logic_vector(4 downto 0);
    signal v5_clkIdelayValue_s        : std_logic_vector(4 downto 0);
    signal v32_adcIdelay_we_s         : std_logic_vector(31 downto 0);
    signal v32_bitslip_s              : std_logic_vector(31 downto 0);
    signal v32_calib_error_s          : std_logic_vector(31 downto 0);
    signal v32_calib_pattern_error_s  : std_logic_vector(31 downto 0);
    signal v16_FreqCntClkCnt_s        : std_logic_vector(15 downto 0);
    signal v6_FreqCntClkSel_s         : std_logic_vector(5 downto 0);
    signal FreqCntRst_s               : std_logic;
    signal v4_FmcPosition_s           : std_logic_vector(3 downto 0);

    signal pUpdateADCStatus_s         : std_logic;

    signal AdcDataClk_s               : std_logic;
    signal AdcDataCh1_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh2_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh3_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh4_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh5_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh6_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh7_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh8_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh9_s               : std_logic_vector(15 downto 0);
    signal AdcDataCh10_s              : std_logic_vector(15 downto 0);
    signal AdcDataCh11_s              : std_logic_vector(15 downto 0);
    signal AdcDataCh12_s              : std_logic_vector(15 downto 0);
    signal AdcDataCh13_s              : std_logic_vector(15 downto 0);
    signal AdcDataCh14_s              : std_logic_vector(15 downto 0);
    signal AdcDataCh15_s              : std_logic_vector(15 downto 0);
    signal AdcDataCh16_s              : std_logic_vector(15 downto 0);
    signal AdcCh1Valid_s              : std_logic;
    signal AdcCh2Valid_s              : std_logic;
    signal AdcCh3Valid_s              : std_logic;
    signal AdcCh4Valid_s              : std_logic;
    signal AdcCh5Valid_s              : std_logic;
    signal AdcCh6Valid_s              : std_logic;
    signal AdcCh7Valid_s              : std_logic;
    signal AdcCh8Valid_s              : std_logic;
    signal AdcCh9Valid_s              : std_logic;
    signal AdcCh10Valid_s             : std_logic;
    signal AdcCh11Valid_s             : std_logic;
    signal AdcCh12Valid_s             : std_logic;
    signal AdcCh13Valid_s             : std_logic;
    signal AdcCh14Valid_s             : std_logic;
    signal AdcCh15Valid_s             : std_logic;
    signal AdcCh16Valid_s             : std_logic;

    -- Keep net names to maintain constraints in UCF
    attribute KEEP : string;
    attribute KEEP of IPSoftRst_s : signal is "TRUE";

begin

    -------------------------------------------------------------------------------
    --  Instantiate MMCM module to generate ADC serial and data clocks
    -------------------------------------------------------------------------------
    U_MMCM_V6_Inst : entity lyt_axi_mi125_v1_00_a.MMCMExtended_V6
    port map (
        i_SystemClk_p         => S_AXI_ACLK,
        iv5_clkIdelayValue_p  => v5_clkIdelayValue_s,
        -- Clock in ports from external diff pins
        CLK_IN1_P         => idp_ClockFromADC_p,
        CLK_IN1_N         => idn_ClockFromADC_p,
        -- Clock out ports
        CLK_OUT1          => SerialClock_s,
        CLK_OUT2          => DataClock_s,
        LOCKED            => ADCClockMMCMLocked_s
    );

    o_IPSerialClockOut_p    <= SerialClock_s;
    o_IPSerialClockOutDiv_p <= DataClock_s;

    -------------------------------------------------------------------------------
    --  If primary module, use MMCM generated clock
    -------------------------------------------------------------------------------
    primaryModuleGen: if C_PRIMARY_MODULE generate
    begin
        ADCClockMMCMPresent_s <= '1';
        IPSerialClockIn_s     <= SerialClock_s;
        IPSerialClockInDiv_s  <= DataClock_s;
    end generate primaryModuleGen;

    -------------------------------------------------------------------------------
    --  If not primary module, use clocks from an other FMC core
    -------------------------------------------------------------------------------
    secondaryModuleGen: if not C_PRIMARY_MODULE generate
    begin
        ADCClockMMCMPresent_s <= '0';
        IPSerialClockIn_s     <= i_IPSerialClockIn_p;
        IPSerialClockInDiv_s  <= i_IPSerialClockInDiv_p;
    end generate secondaryModuleGen;

    -------------------------------------------------------------------------------
    --  Only the bottom MI125 FMC card has its trigger IO routed to the FPGA
    -------------------------------------------------------------------------------
    triggerGen : if (C_FMC_POSITION = 0 or C_FMC_POSITION = 2) generate
        IBUF_inst : IBUF
        generic map (
            IOSTANDARD => "LVCMOS25")
        port map (
            O  => TriggerInFromIO_s,
            I  => i_TriggerInFromIO_p);

        OBUFTRIG_INST : OBUF
        generic map (
            IOSTANDARD => "LVCMOS25")
        port map (
            O => o_TriggerOutToIO_p,
            I => i_TriggerFromFPGA_p);
    end generate;

    triggerNoGen : if (not (C_FMC_POSITION = 0 or C_FMC_POSITION = 2)) generate
        TriggerInFromIO_s <= '0';
        o_TriggerOutToIO_p <= '0';
    end generate;

    -------------------------------------------------------------------------------
    --  Instantiate MI125 FPGA core
    -------------------------------------------------------------------------------
    MI125_Wrapper_INST: MI125_Wrapper
    port map (
        ---- Clock ports ------------------------------------------
        i_SystemClock_p           => S_AXI_ACLK,
        i_RefClk200MHz_p          => i_RefClk200MHz_p,
        i_RxClk_p                 => IPSerialClockIn_s,
        i_RxClkDiv_p              => IPSerialClockInDiv_s,
        -----------------------------------------------------------
        -----------------------------------------------------------

        ---- Pins -------------------------------------------------
        idp_DataFromADC_p         => idp_DataFromADC_p,
        idn_DataFromADC_p         => idn_DataFromADC_p,
        idp_FrameFromADC_p        => idp_FrameFromADC_p,
        idn_FrameFromADC_p        => idn_FrameFromADC_p,
        i_TriggerInFromIO_p       => TriggerInFromIO_s,
        -----------------------------------------------------------

        ---- Parameters & Status ----------------------------------
        -- MI250 configuration port
        i_reset_p                 => IPSoftRst_s,
        i_DigOutRandEn_p          => DigOutRandEn_s,

        -- Calibration ports
        i_IdelayCtrlRst_p         => IdelayCtrlRst_s,
        i_adcIdelay_rst_p         => IdelayRst_s,
        iv5_adcIdelay_value_p     => v5_adcIdelay_value_s,
        iv32_adcIdelay_we_p       => v32_adcIdelay_we_s,
        i_serdesRst_p             => IserdesRst_s,
        iv32_bitslip_p            => v32_bitslip_s,
        o_idelay_ready_p          => idelay_ready_s,

        -- Frequency counter
        i_FreqCntRst_p            => FreqCntRst_s,
        iv6_FreqCntClkSel_p       => v6_FreqCntClkSel_s,
        ov16_FreqCntClkCnt_p      => v16_FreqCntClkCnt_s,

        -- Config i_TriggerInFromIO_p to o_TriggerToFPGA_p delay
        iv5_TriggerDelay_p        => v5_TriggerDelay_s,

        i_reset_calib_detection_p   => reset_calib_detection_s,
        ov_calib_error_p            => v32_calib_error_s,
        ov_calib_pattern_error_p    => v32_calib_pattern_error_s,
        o_calib_detection_done_p    => calib_detection_done_s,
        -----------------------------------------------------------

        ---- Data outputs -----------------------------------------
        o_adc_data_clk_p        => AdcDataClk_s,
        ov16_adc_data_ch1_p     => AdcDataCh1_s,
        ov16_adc_data_ch2_p     => AdcDataCh2_s,
        ov16_adc_data_ch3_p     => AdcDataCh3_s,
        ov16_adc_data_ch4_p     => AdcDataCh4_s,
        ov16_adc_data_ch5_p     => AdcDataCh5_s,
        ov16_adc_data_ch6_p     => AdcDataCh6_s,
        ov16_adc_data_ch7_p     => AdcDataCh7_s,
        ov16_adc_data_ch8_p     => AdcDataCh8_s,
        ov16_adc_data_ch9_p     => AdcDataCh9_s,
        ov16_adc_data_ch10_p    => AdcDataCh10_s,
        ov16_adc_data_ch11_p    => AdcDataCh11_s,
        ov16_adc_data_ch12_p    => AdcDataCh12_s,
        ov16_adc_data_ch13_p    => AdcDataCh13_s,
        ov16_adc_data_ch14_p    => AdcDataCh14_s,
        ov16_adc_data_ch15_p    => AdcDataCh15_s,
        ov16_adc_data_ch16_p    => AdcDataCh16_s,
        -----------------------------------------------------------

        ---- Trigger ports ----------------------------------------
        o_TriggerToFPGA_p       => o_TriggerToFPGA_p
        -----------------------------------------------------------
    );

    -------------------------------------------------------------------------------
    --  Instantiate MI125 AXI registers
    -------------------------------------------------------------------------------
    v4_FmcPosition_s <= std_logic_vector(to_unsigned(C_FMC_POSITION,4));
    
    AxiMi125Regs_l : entity lyt_axi_mi125_v1_00_a.axi_MI125
    generic map(
        C_BUILD_REVISION               => C_BUILD_REVISION,
        C_S_AXI_DATA_WIDTH             => C_S_AXI_DATA_WIDTH,
        C_S_AXI_ADDR_WIDTH             => C_S_AXI_ADDR_WIDTH,
        C_S_AXI_MIN_SIZE               => C_S_AXI_MIN_SIZE,
        C_USE_WSTRB                    => C_USE_WSTRB,
        C_DPHASE_TIMEOUT               => C_DPHASE_TIMEOUT,
        C_BASEADDR                     => C_BASEADDR,
        C_HIGHADDR                     => C_HIGHADDR,
        C_FAMILY                       => C_FAMILY
    )
    port map(
        -- User ports        
        i_logicRst_p                    => i_logicRst_p,
        o_UpdateADCStatus_p             => UpdateADCStatus_s,
        o_IdelayRst_p                   => IdelayRst_s,
        ov5_TriggerDelay_p              => v5_TriggerDelay_s,
        o_reset_calib_detection_p       => reset_calib_detection_s,
        o_IdelayCtrlRst_p               => IdelayCtrlRst_s,
        o_IserdesRst_p                  => IserdesRst_s,
        ov16_AdcValid_p                 => v16_AdcValid_s,
        o_DigOutRandEn_p                => DigOutRandEn_s,
        o_IPSoftRst_p                   => IPSoftRst_s,
        ov2_ChannelConfig_p             => v2_ChannelConfig_s,
        o_DataFormat_p                  => o_DataFormat_p,
        o_ADCClockMMCMRst_p             => ADCClockMMCMRst_s,
        i_idelay_ready_p                => idelay_ready_s,
        i_calib_detection_done_p        => calib_detection_done_s,
        i_ADCClockMMCMLocked_p          => ADCClockMMCMLocked_s,
        i_ADCClockMMCMPresent_p         => ADCClockMMCMPresent_s,
        iv4_FmcPosition_p               => v4_FmcPosition_s,
        ov5_adcIdelay_value_p           => v5_adcIdelay_value_s,
        ov5_clkIdelay_value_p           => v5_clkIdelayValue_s,
        ov32_adcIdelay_we_p             => v32_adcIdelay_we_s,
        ov32_bitslip_p                  => v32_bitslip_s,
        iv32_calib_error_p              => v32_calib_error_s,
        iv32_calib_pattern_error_p      => v32_calib_pattern_error_s,
        iv16_FreqCntClkCnt_p            => v16_FreqCntClkCnt_s,
        ov6_FreqCntClkSel_p             => v6_FreqCntClkSel_s,
        o_FreqCntRst_p                  => FreqCntRst_s,

        -- DO NOT EDIT BELOW THIS LINE ---------------------
        -- Bus protocol ports, do not add to or delete
        S_AXI_ACLK                      => S_AXI_ACLK,
        S_AXI_ARESETN                   => S_AXI_ARESETN,
        S_AXI_AWADDR                    => S_AXI_AWADDR,
        S_AXI_AWVALID                   => S_AXI_AWVALID,
        S_AXI_WDATA                     => S_AXI_WDATA,
        S_AXI_WSTRB                     => S_AXI_WSTRB,
        S_AXI_WVALID                    => S_AXI_WVALID,
        S_AXI_BREADY                    => S_AXI_BREADY,
        S_AXI_ARADDR                    => S_AXI_ARADDR,
        S_AXI_ARVALID                   => S_AXI_ARVALID,
        S_AXI_RREADY                    => S_AXI_RREADY,
        S_AXI_ARREADY                   => S_AXI_ARREADY,
        S_AXI_RDATA                     => S_AXI_RDATA,
        S_AXI_RRESP                     => S_AXI_RRESP,
        S_AXI_RVALID                    => S_AXI_RVALID,
        S_AXI_WREADY                    => S_AXI_WREADY,
        S_AXI_BRESP                     => S_AXI_BRESP,
        S_AXI_BVALID                    => S_AXI_BVALID,
        S_AXI_AWREADY                   => S_AXI_AWREADY
        -- DO NOT EDIT ABOVE THIS LINE --------S_AXI_AWREADY     -------------
    );

    -------------------------------------------------------------------------------
    --  Output assignment
    -------------------------------------------------------------------------------

    o_AdcDataClk_p <= AdcDataClk_s;

    --// Bus reduction from 16 bits received at the ISEDES interface
    --// to  the 14 effective bits from the ADC.
    --// Remaping to fit with the external connector naming.
    ov14_AdcDataCh3_p  <= AdcDataCh1_s(15 downto 2);
    ov14_AdcDataCh4_p  <= AdcDataCh2_s(15 downto 2);
    ov14_AdcDataCh1_p  <= AdcDataCh3_s(15 downto 2);
    ov14_AdcDataCh2_p  <= AdcDataCh4_s(15 downto 2);
    ov14_AdcDataCh7_p  <= AdcDataCh5_s(15 downto 2);
    ov14_AdcDataCh8_p  <= AdcDataCh6_s(15 downto 2);
    ov14_AdcDataCh5_p  <= AdcDataCh7_s(15 downto 2);
    ov14_AdcDataCh6_p  <= AdcDataCh8_s(15 downto 2);
    ov14_AdcDataCh11_p <= AdcDataCh9_s(15 downto 2);
    ov14_AdcDataCh12_p <= AdcDataCh10_s(15 downto 2);
    ov14_AdcDataCh9_p  <= AdcDataCh11_s(15 downto 2);
    ov14_AdcDataCh10_p <= AdcDataCh12_s(15 downto 2);
    ov14_AdcDataCh15_p <= AdcDataCh13_s(15 downto 2);
    ov14_AdcDataCh16_p <= AdcDataCh14_s(15 downto 2);
    ov14_AdcDataCh13_p <= AdcDataCh15_s(15 downto 2);
    ov14_AdcDataCh14_p <= AdcDataCh16_s(15 downto 2);
    
    pulse2pulse_io_reset : entity lyt_axi_mi125_v1_00_a.pulse2pulse
    port map (
        in_clk   => S_AXI_ACLK,
        out_clk  => AdcDataClk_s,
        rst      => IPSoftRst_s,
        pulsein  => UpdateADCStatus_s,
        inbusy   => open,
        pulseout => pUpdateADCStatus_s);

    process(IPSoftRst_s, AdcDataClk_s)
    begin
        if (IPSoftRst_s = '1') then
            o_AdcCh1to4Enabled_p    <= '0';
            o_AdcCh5to8Enabled_p    <= '0';
            o_AdcCh9to12Enabled_p   <= '0';
            o_AdcCh13to16Enabled_p  <= '0';
            
            o_AdcCh3Valid_p   <= '0';
            o_AdcCh4Valid_p   <= '0';
            o_AdcCh1Valid_p   <= '0';
            o_AdcCh2Valid_p   <= '0';
            o_AdcCh7Valid_p   <= '0';
            o_AdcCh8Valid_p   <= '0';
            o_AdcCh5Valid_p   <= '0';
            o_AdcCh6Valid_p   <= '0';
            o_AdcCh11Valid_p  <= '0';
            o_AdcCh12Valid_p  <= '0';
            o_AdcCh9Valid_p   <= '0';
            o_AdcCh10Valid_p  <= '0';
            o_AdcCh15Valid_p  <= '0';
            o_AdcCh16Valid_p  <= '0';
            o_AdcCh13Valid_p  <= '0';
            o_AdcCh14Valid_p  <= '0';
            
        elsif rising_edge(AdcDataClk_s) then
        
            -- Update o_AdcChXtoXEnabled_p and o_AdcChXValid_p
            -- only when pUpdateADCStatus_s = '1'
            if pUpdateADCStatus_s = '1' then
            
                if v2_ChannelConfig_s = "00" then
                    o_AdcCh1to4Enabled_p    <= '1';
                    o_AdcCh5to8Enabled_p    <= '0';
                    o_AdcCh9to12Enabled_p   <= '0';
                    o_AdcCh13to16Enabled_p  <= '0';
                elsif v2_ChannelConfig_s = "01" then
                    o_AdcCh1to4Enabled_p    <= '1';
                    o_AdcCh5to8Enabled_p    <= '1';
                    o_AdcCh9to12Enabled_p   <= '0';
                    o_AdcCh13to16Enabled_p  <= '0';
                elsif v2_ChannelConfig_s = "10" then
                    o_AdcCh1to4Enabled_p    <= '1';
                    o_AdcCh5to8Enabled_p    <= '1';
                    o_AdcCh9to12Enabled_p   <= '1';
                    o_AdcCh13to16Enabled_p  <= '0';
                else -- v2_ChannelConfig_s = "11"
                    o_AdcCh1to4Enabled_p    <= '1';
                    o_AdcCh5to8Enabled_p    <= '1';
                    o_AdcCh9to12Enabled_p   <= '1';
                    o_AdcCh13to16Enabled_p  <= '1';
                end if;
                
                o_AdcCh3Valid_p   <= v16_AdcValid_s(0);
                o_AdcCh4Valid_p   <= v16_AdcValid_s(1);
                o_AdcCh1Valid_p   <= v16_AdcValid_s(2);
                o_AdcCh2Valid_p   <= v16_AdcValid_s(3);
                o_AdcCh7Valid_p   <= v16_AdcValid_s(4);
                o_AdcCh8Valid_p   <= v16_AdcValid_s(5);
                o_AdcCh5Valid_p   <= v16_AdcValid_s(6);
                o_AdcCh6Valid_p   <= v16_AdcValid_s(7);
                o_AdcCh11Valid_p  <= v16_AdcValid_s(8);
                o_AdcCh12Valid_p  <= v16_AdcValid_s(9);
                o_AdcCh9Valid_p   <= v16_AdcValid_s(10);
                o_AdcCh10Valid_p  <= v16_AdcValid_s(11);
                o_AdcCh15Valid_p  <= v16_AdcValid_s(12);
                o_AdcCh16Valid_p  <= v16_AdcValid_s(13);
                o_AdcCh13Valid_p  <= v16_AdcValid_s(14);
                o_AdcCh14Valid_p  <= v16_AdcValid_s(15);
            end if;
        end if;
    end process;

end IMP;

