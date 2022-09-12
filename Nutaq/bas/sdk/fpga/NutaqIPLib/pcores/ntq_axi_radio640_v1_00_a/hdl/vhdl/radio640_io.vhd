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
-- File        : $Id: radio640_io.vhd,v 1.6 2015/12/07 21:10:33 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Instantiate all the logic around the Radio640 IO.
--------------------------------------------------------------------------------
-- Copyright (c) 2015 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: radio640_io.vhd,v $
-- Revision 1.6  2015/12/07 21:10:33  julien.roy
-- Add Trigger, InputSync and OutputSync value access.
-- Add Trigger, InputSync and OutputSync low and high value access.
--
-- Revision 1.5  2015/11/12 16:09:51  christian.dufour-bou
-- Added missing declaration of parameter DDR_REGISTER
--
-- Revision 1.4  2015/11/12 14:51:13  julien.roy
-- Modify SPI core to have dynamicaly configurable SPI CLK speed
--
-- Revision 1.3  2015/11/12 13:42:07  christian.dufour-bou
-- Preliminary first commit for support of ZC706 carrier and Vivado
--
-- Revision 1.2  2015/10/30 14:09:29  julien.roy
-- Add trigger idelay logic
--
-- Revision 1.1  2015/10/20 19:14:44  julien.roy
-- Add first revision of Radio640 fpga pcore.
--
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library ntq_axi_radio640_v1_00_a;
use ntq_axi_radio640_v1_00_a.all;

entity radio640_io is
  generic  (
    PCORE_DEVICE_TYPE           : integer := 0
    );
  port  (
    i_Rst_p                     : in     std_logic;
    i_DelayClk_p                : in     std_logic;
  
    -- Pin
    o_Ad9361TxnrxPin_p          : out    std_logic;
    o_Ad9361EnablePin_p         : out    std_logic;
    o_Ad9361EnAgcPin_p          : out    std_logic;
    o_Ad9361ResetnPin_p         : out    std_logic;
    ov4_Ad9361CtrlInPin_p       : out    std_logic_vector(3 downto 0);
    iv8_Ad9361CtrlOutPin_p      : in     std_logic_vector(7 downto 0);
    o_RfSwitch1Pin_p            : out    std_logic;
    o_RfSwitch2Pin_p            : out    std_logic;
    ov2_ClkSwitch1Pin_p         : out    std_logic_vector(1 downto 0);
    o_ClkSwitch2Pin_p           : out    std_logic;
    i_TriggerPin_p              : in     std_logic;

    -- User Logic
    i_DesignClk_p               : in     std_logic;
    i_Ad9361Txnrx_p             : in     std_logic;
    i_Ad9361Enable_p            : in     std_logic;
    i_Ad9361EnAgc_p             : in     std_logic;
    iv4_Ad9361CtrlIn_p          : in     std_logic_vector(3 downto 0);
    ov8_Ad9361CtrlOut_p         : out    std_logic_vector(7 downto 0);
    i_Trx1InRxDir_p             : in     std_logic;
    i_Trx2InRxDir_p             : in     std_logic;
    o_Trigger_p                 : out    std_logic;

    -- Processor
    i_SystemClk_p               : in     std_logic;
    i_TxnrxForce_p              : in     std_logic;
    i_TxnrxValue_p              : in     std_logic;
    i_EnableForce_p             : in     std_logic;
    i_EnableValue_p             : in     std_logic;
    i_EnAgcForce_p              : in     std_logic;
    i_EnAgcValue_p              : in     std_logic;
    iv4_ForceCtrlIn_p           : in     std_logic_vector(3 downto 0);
    iv4_ValueCtrlIn_p           : in     std_logic_vector(3 downto 0);
    i_UpdateCtrlOutLatch_p      : in     std_logic;
    ov8_CtrlOutCurrent_p        : out    std_logic_vector(7 downto 0);
    ov8_CtrlOutLow_p            : out    std_logic_vector(7 downto 0);
    ov8_CtrlOutHigh_p           : out    std_logic_vector(7 downto 0);
    iv2_ClkSwitch1_p            : in     std_logic_vector(1 downto 0);
    i_ClkSwitch2_p              : in     std_logic;
    i_Trx1InRxDirValue_p        : in     std_logic;
    i_Trx1InRxDirForce_p        : in     std_logic;
    i_Trx2InRxDirValue_p        : in     std_logic;
    i_Trx2InRxDirForce_p        : in     std_logic;
    i_TriggerBitSlip            : in     std_logic;
    iv5_TriggerDelay_p          : in     std_logic_vector(4 downto 0);
    i_UpdateTriggerLatch_p      : in     std_logic;
    o_TriggerLow_p              : out    std_logic;
    o_TriggerHigh_p             : out    std_logic
  );
end entity radio640_io;


architecture rtl of radio640_io is

    component ad_lvds_in
    generic (
        BUFTYPE      : integer := 0;
        IODELAY_CTRL : integer := 0;
        LVDS_INPUT   : integer := 1;
        DDR_REGISTER : integer := 1
    );
    port (
        rx_clk       : in std_logic;
        rx_data_in_p : in std_logic;
        rx_data_in_n : in std_logic;
        rx_data_p    : out std_logic;
        rx_data_n    : out std_logic;
        i_BitSlip_p  : in std_logic;
        delay_clk    : in std_logic;
        delay_rst    : in std_logic;
        delay_ld     : in std_logic;
        delay_wdata  : in std_logic_vector(4 downto 0);
        delay_rdata  : out std_logic_vector(4 downto 0);
        delay_locked : out std_logic
    );
    end component;
    
    ----------------------------------------------------------
    -- Signal declaration
    ----------------------------------------------------------
    signal RfSwitch1Pin_s   : std_logic;
    signal v4_ValueCtrlIn_s : std_logic_vector(3 downto 0);
    
    signal Trigger_s        : std_logic;
    signal v1_Trigger_s     : std_logic_vector(0 downto 0);
    signal v1_TriggerLow_s  : std_logic_vector(0 downto 0);
    signal v1_TriggerHigh_s : std_logic_vector(0 downto 0);

begin

    -- Direct connection
    ov2_ClkSwitch1Pin_p <= iv2_ClkSwitch1_p;
    o_ClkSwitch2Pin_p   <= i_ClkSwitch2_p;

    -- User logic / Processor multiplexer
    o_Ad9361TxnrxPin_p <= i_TxnrxValue_p when i_TxnrxForce_p = '1'
        else i_Ad9361Txnrx_p;

    o_Ad9361EnablePin_p <= i_EnableValue_p when i_EnableForce_p = '1'
        else i_Ad9361Enable_p;

    o_Ad9361EnAgcPin_p <= i_EnAgcValue_p when i_EnAgcForce_p = '1'
        else i_Ad9361EnAgc_p;

    ---- RfSwitch1Pin : 0, TRX1 connector to RX1 channel
    RfSwitch1Pin_s <= i_Trx1InRxDirValue_p when i_Trx1InRxDirForce_p = '1'
        else i_Trx1InRxDir_p;
    o_RfSwitch1Pin_p  <= not RfSwitch1Pin_s;

    ---- RfSwitch2Pin : 1, TRX2 connector to RX2 channel
    o_RfSwitch2Pin_p <= i_Trx2InRxDirValue_p when i_Trx2InRxDirForce_p = '1'
        else i_Trx2InRxDir_p;


    ----------------------------------------------------------
    -- CtrlIn
    ----------------------------------------------------------
    -- Clock domain crossing of CtrlIn from processor
    ctrlin_syncer_i : entity ntq_axi_radio640_v1_00_a.bus_syncer
    generic map (
        C_WIDTH         => 4)
	port map (
		i_InClk_p 	    => i_SystemClk_p,
		iv_InSig_p 	    => iv4_ValueCtrlIn_p,
		i_OutClk_p 	    => i_DesignClk_p,
		ov_OutSig_p 	=> v4_ValueCtrlIn_s);

    -- User logic / Processor multiplexer
    process(v4_ValueCtrlIn_s, iv4_ForceCtrlIn_p, iv4_Ad9361CtrlIn_p)
    begin
        for I in 0 to iv4_ForceCtrlIn_p'high loop
            if iv4_ForceCtrlIn_p(I) = '1' then
                ov4_Ad9361CtrlInPin_p(I) <= v4_ValueCtrlIn_s(I);
            else
                ov4_Ad9361CtrlInPin_p(I) <= iv4_Ad9361CtrlIn_p(I);
            end if;
        end loop;
    end process;


    ----------------------------------------------------------
    -- CtrlOut
    ----------------------------------------------------------
    -- Directly output to user logic the Ad9361 CtrlOut bits
    ov8_Ad9361CtrlOut_p <= iv8_Ad9361CtrlOutPin_p;

    -- Output to processor the current CtrlOut bits latched on system clock
    ctrlout_syncer_i : entity ntq_axi_radio640_v1_00_a.bus_syncer
    generic map (
        C_WIDTH         => 8)
	port map (
		i_InClk_p 	    => i_DesignClk_p,
		iv_InSig_p 	    => iv8_Ad9361CtrlOutPin_p,
		i_OutClk_p 	    => i_SystemClk_p,
		ov_OutSig_p 	=> ov8_CtrlOutCurrent_p);

    -- Output to processor the low and high CtrlOut bits since last update
    ctrlout_latch_i: entity ntq_axi_radio640_v1_00_a.low_high_latch
    generic map (
        C_WIDTH             => 8)
    port map (
        i_Rst_p             => i_Rst_p,
        i_UpdateClk_p       => i_SystemClk_p,
        i_UpdatePulse_p     => i_UpdateCtrlOutLatch_p,
        i_Clk_p             => i_DesignClk_p,
        iv_Data_p           => iv8_Ad9361CtrlOutPin_p,
        ov_LowData_p        => ov8_CtrlOutLow_p,
        ov_HighData_p       => ov8_CtrlOutHigh_p);

        
    ----------------------------------------------------------
    -- Trigger
    ----------------------------------------------------------  
    trigger_i: ad_lvds_in
    generic map (
        BUFTYPE      => PCORE_DEVICE_TYPE,
        IODELAY_CTRL => 0,
        LVDS_INPUT   => 0,
        DDR_REGISTER => 1)
    port map (
        rx_clk       => i_DesignClk_p,
        rx_data_in_p => i_TriggerPin_p,
        rx_data_in_n => std_logic'('0'),
        rx_data_p    => Trigger_s,
        rx_data_n    => open,
        i_BitSlip_p  => i_TriggerBitSlip,
        delay_clk    => i_DelayClk_p,
        delay_rst    => std_logic'('0'),
        delay_ld     => std_logic'('1'),
        delay_wdata  => iv5_TriggerDelay_p,
        delay_rdata  => open,
        delay_locked => open);
    
    o_Trigger_p <= Trigger_s;

    -- Output to processor the low and high Trigger bit since last update
    v1_Trigger_s(0) <= Trigger_s;
    
    trigger_latch_i: entity ntq_axi_radio640_v1_00_a.low_high_latch
    generic map (
        C_WIDTH             => 1)
    port map (
        i_Rst_p             => i_Rst_p,
        i_UpdateClk_p       => i_SystemClk_p,
        i_UpdatePulse_p     => i_UpdateTriggerLatch_p,
        i_Clk_p             => i_DesignClk_p,
        iv_Data_p           => v1_Trigger_s,
        ov_LowData_p        => v1_TriggerLow_s,
        ov_HighData_p       => v1_TriggerHigh_s);
        
    o_TriggerLow_p  <= v1_TriggerLow_s(0);
    o_TriggerHigh_p <= v1_TriggerHigh_s(0);

end architecture rtl;
