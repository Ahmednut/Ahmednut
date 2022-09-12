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
-- File        : $Id: MI125_Wrapper.vhd,v 1.1 2015/04/10 19:47:33 sqa Exp $
--------------------------------------------------------------------------------
-- Description : MI125 data acquisition logic wrapper
--------------------------------------------------------------------------------
-- Notes / Assumptions : Simple wrapper to the MI125 LVDS data alignement and
--                       acquisition process.
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: MI125_Wrapper.vhd,v $
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.5  2013/08/08 15:33:26  julien.roy
-- Change calibration from FPGA to software
--
-- Revision 1.4  2012/12/11 15:28:12  julien.roy
-- Add max fanout constraint for frequency counter reset
--
-- Revision 1.3  2012/12/10 14:30:37  julien.roy
-- Modify ucf to support the 4 FPGA types
-- Add chip enable status ports
-- Add variable delay trigger
-- Move frequency counter status into core registers
--
-- Revision 1.2  2012/10/30 18:17:39  david.quinn
-- New alignement FSM to take into account one lane decision for the remaining lanes.
--
-- Revision 1.1  2012/10/16 13:17:50  khalid.bensadek
-- First commit of a working version of this project
--
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

library unisim;
use unisim.vcomponents.all;

entity MI125_Wrapper is
	generic (
        NBRCHANNELS             : integer := 32;
        C_SYSCLK_FREQ_MHZ       : integer := 100
    );
    port (
        ---- Clock ports ------------------------------------------
        i_SystemClock_p           : in std_logic;
        i_RefClk200MHz_p          : in std_logic;
        i_RxClk_p                 : in std_logic;
        i_RxClkDiv_p              : in std_logic;
        -----------------------------------------------------------
        -----------------------------------------------------------

        ---- Pins -------------------------------------------------
        idp_DataFromADC_p         : in std_logic_vector(NBRCHANNELS-1 downto 0);
        idn_DataFromADC_p         : in std_logic_vector(NBRCHANNELS-1 downto 0);
        idp_FrameFromADC_p        : in std_logic;
        idn_FrameFromADC_p        : in std_logic;
        i_TriggerInFromIO_p       : in std_logic;
        -----------------------------------------------------------

        ---- Parameters & Status ----------------------------------
        -- MI250 configuration port
        i_reset_p                 : in std_logic;
        i_DigOutRandEn_p          : in std_logic;

        -- Calibration ports
        i_IdelayCtrlRst_p         : in std_logic;
        i_adcIdelay_rst_p         : in std_logic;
        iv5_adcIdelay_value_p     : in std_logic_vector(4 downto 0);
        iv32_adcIdelay_we_p       : in std_logic_vector(31 downto 0);
        i_serdesRst_p             : in std_logic;
        iv32_bitslip_p            : in std_logic_vector(31 downto 0);
        o_idelay_ready_p          : out std_logic;

        -- Frequency counter
        i_FreqCntRst_p            : in std_logic;
        iv6_FreqCntClkSel_p       : in std_logic_vector(5 downto 0);
        ov16_FreqCntClkCnt_p      : out std_logic_vector(15 downto 0);

        -- Config i_TriggerInFromIO_p to o_TriggerToFPGA_p delay
        iv5_TriggerDelay_p        : in std_logic_vector(4 downto 0);
        
        -- Calibration detection signals
        i_reset_calib_detection_p   : in std_logic;
        ov_calib_error_p            : out std_logic_vector(31 downto 0);
        ov_calib_pattern_error_p    : out std_logic_vector(31 downto 0);
        o_calib_detection_done_p    : out std_logic;
        -----------------------------------------------------------

        ---- Data outputs -----------------------------------------
        o_adc_data_clk_p        : out std_logic;
        ov16_adc_data_ch1_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch2_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch3_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch4_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch5_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch6_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch7_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch8_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch9_p     : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch10_p    : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch11_p    : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch12_p    : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch13_p    : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch14_p    : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch15_p    : out std_logic_vector (15 downto 0);
        ov16_adc_data_ch16_p    : out std_logic_vector (15 downto 0);
        -----------------------------------------------------------

        ---- Trigger ports ----------------------------------------
        o_TriggerToFPGA_p       : OUT std_logic
        -----------------------------------------------------------
    );
end MI125_Wrapper;


architecture MI125_Wrapper_Arch of MI125_Wrapper is

    -------------------------------------------------------------------------------
    --         ************** Signals declaration *******************
    -------------------------------------------------------------------------------

    signal v256_data_s 	        : std_logic_vector (255 downto 0);
    signal v256_data_r1_s       : std_logic_vector (255 downto 0);

    signal v5_TriggerDelay_s    : std_logic_vector(4 downto 0);

    -------------------------------------------------------------------------------
    --     ************** Function declaration *******************
    -------------------------------------------------------------------------------
    
    -- Return a std_logic_vector with all bit xored with LSB if Enable is 1. No xor if Enable=0
    -- This function remove the randomizer effect if the ADC is randomizing the date

    function Derandomize(val: std_logic_vector(15 downto 0);Enable:std_logic) return std_logic_vector is
        variable tmp: std_logic_vector(15 downto 0);
    begin
        for i in val'high downto val'low+3 loop
                if Enable='0' then
                tmp(i):=val(i);
            else
                tmp(i):=val(i) xor val(2);
            end if;
        end loop;
        tmp(0):=val(val'low);
        tmp(1):=val(val'low+1);
        tmp(2):=val(val'low+2);
        return tmp;
    end Derandomize;

    -------------------------------------------------------------------------------
    --         ************** Attribute declaration *******************
    -------------------------------------------------------------------------------
    
    attribute keep_hierarchy : string;
    attribute keep_hierarchy of MI125_Wrapper_Arch : architecture is "true";

begin

    DDR_8TO1_32CHAN_RX_INST00 : entity work.DDR_8TO1_32CHAN_RX
    generic map(
        NBRCHANNELS             => NBRCHANNELS,
        C_SYSCLK_FREQ_MHZ       => C_SYSCLK_FREQ_MHZ
    )
    port map(
        i_RefClk200MHz_p        => i_RefClk200MHz_p,
        i_SystemClock_p         => i_SystemClock_p,
        i_reset_p               => i_reset_p,

        idp_DataFromADC_p       => idp_DataFromADC_p,
        idn_DataFromADC_p       => idn_DataFromADC_p,
        idp_FrameFromADC_p      => idp_FrameFromADC_p,
        idn_FrameFromADC_p      => idn_FrameFromADC_p,

        i_RxClk_p               => i_RxClk_p,
        i_RxClkDiv_p            => i_RxClkDiv_p,

        o_idelay_ready_p        => o_idelay_ready_p,
        ov_Data_p               => v256_data_s,

        i_FreqCntRst_p          => i_FreqCntRst_p,
        iv6_FreqCntClkSel_p     => iv6_FreqCntClkSel_p,
        ov16_FreqCntClkCnt_p    => ov16_FreqCntClkCnt_p,

        i_IdelayCtrlRst_p       => i_IdelayCtrlRst_p,
        i_adcIdelay_rst_p       => i_adcIdelay_rst_p,
        iv5_adcIdelay_value_p   => iv5_adcIdelay_value_p,
        iv32_adcIdelay_we_p     => iv32_adcIdelay_we_p,

        i_serdesRst_p           => i_serdesRst_p,
        iv32_bitslip_p          => iv32_bitslip_p,
        
        i_reset_calib_detection_p   => i_reset_calib_detection_p,
        ov_calib_error_p            => ov_calib_error_p,
        ov_calib_pattern_error_p    => ov_calib_pattern_error_p,
        o_calib_detection_done_p    => o_calib_detection_done_p
    );

    FlipFlop_l : process (i_RxClkDiv_p)
    begin
        if rising_edge(i_RxClkDiv_p) then
            v256_data_r1_s          <= v256_data_s;
            
            ov16_adc_data_ch1_p     <= Derandomize(v256_data_r1_s(255 downto 240),i_DigOutRandEn_p);
            ov16_adc_data_ch2_p     <= Derandomize(v256_data_r1_s(239 downto 224),i_DigOutRandEn_p);
            ov16_adc_data_ch3_p     <= Derandomize(v256_data_r1_s(223 downto 208),i_DigOutRandEn_p);
            ov16_adc_data_ch4_p     <= Derandomize(v256_data_r1_s(207 downto 192),i_DigOutRandEn_p);
            ov16_adc_data_ch5_p     <= Derandomize(v256_data_r1_s(191 downto 176),i_DigOutRandEn_p);
            ov16_adc_data_ch6_p     <= Derandomize(v256_data_r1_s(175 downto 160),i_DigOutRandEn_p);
            ov16_adc_data_ch7_p     <= Derandomize(v256_data_r1_s(159 downto 144),i_DigOutRandEn_p);
            ov16_adc_data_ch8_p     <= Derandomize(v256_data_r1_s(143 downto 128),i_DigOutRandEn_p);
            ov16_adc_data_ch9_p     <= Derandomize(v256_data_r1_s(127 downto 112),i_DigOutRandEn_p);
            ov16_adc_data_ch10_p    <= Derandomize(v256_data_r1_s(111 downto  96),i_DigOutRandEn_p);
            ov16_adc_data_ch11_p    <= Derandomize(v256_data_r1_s(95  downto  80),i_DigOutRandEn_p);
            ov16_adc_data_ch12_p    <= Derandomize(v256_data_r1_s(79  downto  64),i_DigOutRandEn_p);
            ov16_adc_data_ch13_p    <= Derandomize(v256_data_r1_s(63  downto  48),i_DigOutRandEn_p);
            ov16_adc_data_ch14_p    <= Derandomize(v256_data_r1_s(47  downto  32),i_DigOutRandEn_p);
            ov16_adc_data_ch15_p    <= Derandomize(v256_data_r1_s(31  downto  16),i_DigOutRandEn_p);
            ov16_adc_data_ch16_p    <= Derandomize(v256_data_r1_s(15  downto  0 ),i_DigOutRandEn_p);
        end if;
    end process FlipFlop_l;

    o_adc_data_clk_p   <= i_RxClkDiv_p;

    -------------------------------------------------------------------------------
    -- Trigger section
    -------------------------------------------------------------------------------
    Synchronizer_2Tv_Inst1 : entity work.synchronizer_2tv
    generic map (
        size            => iv5_TriggerDelay_p'length
    )
    port map (
        Clk             => i_RxClkDiv_p,
        Reset           => i_reset_p,
        Reset_value     => (others=>'0'),
        Async           => iv5_TriggerDelay_p,
        Sync            => v5_TriggerDelay_s
    );

    VariableDelayTrigger_INST: entity work.VariableDelayTrigger
    port map (
        i_clk_p         => i_RxClkDiv_p,
        iv5_Delay_p     => v5_TriggerDelay_s,
        i_Trigger_p     => i_TriggerInFromIO_p,
        o_Trigger_p     => o_TriggerToFPGA_p
    );


END MI125_Wrapper_Arch;