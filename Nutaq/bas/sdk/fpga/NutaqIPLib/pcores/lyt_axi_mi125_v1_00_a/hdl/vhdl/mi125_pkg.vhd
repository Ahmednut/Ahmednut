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
-- File        : $Id: mi125_pkg.vhd
--------------------------------------------------------------------------------
-- Description : Package to FMC MI125 Wrapper
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: 
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  
package MI125_pkg is  

    component MI125_Wrapper is
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
    end component;
  
END MI125_pkg;