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
-- File        : $Id: radio640_regs.vhd,v 1.4 2015/12/07 21:10:33 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Generic SPI core
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: radio640_regs.vhd,v $
-- Revision 1.4  2015/12/07 21:10:33  julien.roy
-- Add Trigger, InputSync and OutputSync value access.
-- Add Trigger, InputSync and OutputSync low and high value access.
--
-- Revision 1.3  2015/11/12 14:51:13  julien.roy
-- Modify SPI core to have dynamicaly configurable SPI CLK speed
--
-- Revision 1.2  2015/10/30 14:09:00  julien.roy
-- Add trigger and test clock registers
--
-- Revision 1.1  2015/10/20 19:14:44  julien.roy
-- Add first revision of Radio640 fpga pcore.
--
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.2  2013/01/18 19:03:46  julien.roy
-- Merge changes from ZedBoard reference design to Perseus
--
-- Revision 1.1  2012/09/28 19:31:26  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.2  2011/06/16 18:54:55  jeffrey.johnson
-- Added CVS tags.
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

entity radio640_regs is
    port  (
    -- SPI Interface
    i_Rst_p                 : in  std_logic;
    i_Clk_p                 : in  std_logic;

    i_up_sel_p              : in  std_logic;
    i_up_wr_p               : in  std_logic;
    iv14_up_addr_p          : in  std_logic_vector(13 downto 0);
    iv32_up_wdata_p         : in  std_logic_vector(31 downto 0);
    iv4_up_wstrb_p          : in  std_logic_vector(3 downto 0);
    ov32_up_rdata_p         : out std_logic_vector(31 downto 0);
    o_up_ack_p              : out std_logic;

    ov8_TrxSpiClkDownsampling_p     : out std_logic_vector(7 downto 0);
    o_TrxSpiClkPol_p                : out std_logic;
    o_TrxClkSpiCtrl_p               : out std_logic;
    ov24_TrxClkSpiData_p            : out std_logic_vector(23 downto 0);
    iv24_TrxClkSpiData_p            : in  std_logic_vector(23 downto 0);
    o_TrxSpiStart_p                 : out std_logic;
    o_ClkSpiStart_p                 : out std_logic;
    i_TrxClkSpiBusy_p               : in  std_logic;

    o_DacOscSpiCtrl_p       : out std_logic;
    ov16_DacOscClkSpiData_p : out std_logic_vector(15 downto 0);
    o_DacOscSpiStart_p      : out std_logic;
    i_DacOscClkSpiBusy_p    : in  std_logic;

    o_RfAttSpiCtrl_p        : out std_logic;
    ov16_RfAttClkSpiData_p  : out std_logic_vector(15 downto 0);
    o_RfAttSpiStart_p       : out std_logic;
    i_RfAttClkSpiBusy_p     : in  std_logic;

    iv4_FmcConnector_p      : in  std_logic_vector(3 downto 0);
    iv4_BoardPosition_p     : in  std_logic_vector(3 downto 0);
    iv4_DesignClkRate_p     : in  std_logic_vector(3 downto 0);
    i_ClockMaster_p         : in  std_logic;

    o_TxnrxForce_p          : out std_logic;
    o_TxnrxValue_p          : out std_logic;
    o_EnableForce_p         : out std_logic;
    o_EnableValue_p         : out std_logic;
    o_EnAgcForce_p          : out std_logic;
    o_EnAgcValue_p          : out std_logic;
    ov4_ForceCtrlIn_p       : out std_logic_vector(3 downto 0);
    ov4_ValueCtrlIn_p       : out std_logic_vector(3 downto 0);
    o_UpdateCtrlOutLatch_p  : out std_logic;
    iv8_CtrlOutCurrent_p    : in  std_logic_vector(7 downto 0);
    iv8_CtrlOutLow_p        : in  std_logic_vector(7 downto 0);
    iv8_CtrlOutHigh_p       : in  std_logic_vector(7 downto 0);
    ov2_ClkSwitch1_p        : out std_logic_vector(1 downto 0);
    o_ClkSwitch2_p          : out std_logic;
    o_Trx1InRxDirValue_p    : out std_logic;
    o_Trx1InRxDirForce_p    : out std_logic;
    o_Trx2InRxDirValue_p    : out std_logic;
    o_Trx2InRxDirForce_p    : out std_logic;
    o_Ad9361ResetnPin_p     : out std_logic;
    o_OscEnPin_p            : out std_logic;
    o_ClkResetPin_p         : out std_logic;
    o_TriggerBitSlip_p      : out std_logic;
    ov5_TriggerDelay_p      : out std_logic_vector(4 downto 0);
    ov32_TestClkSel_p       : out std_logic_vector(31 downto 0);
    iv32_TestClkFreq_p      : in  std_logic_vector(31 downto 0);

    i_Trigger_p                 : in  std_logic;
    o_UpdateTriggerLatch_p      : out std_logic;
    i_TriggerLow_p              : in  std_logic;
    i_TriggerHigh_p             : in  std_logic;
    o_UpdateInputSyncLatch_p    : out std_logic;
    o_UpdateOutputSyncLatch_p   : out std_logic;
    i_InputSync_p               : in  std_logic;
    i_InputSyncLow_p            : in  std_logic;
    i_InputSyncHigh_p           : in  std_logic;
    i_OutputSyncLow_p           : in  std_logic;
    i_OutputSyncHigh_p          : in  std_logic
    );
end entity radio640_regs;


architecture rtl of radio640_regs is

    ----------------------------------------------------------
    -- Constant declaration
    ----------------------------------------------------------
    constant RADIO640_REGS_BASE_ADDR    : std_logic_vector(5 downto 0) := "100000";
    constant RADIO640_MAX_REGS          : integer := 16;

    constant REG_TRXCLK_SPI_IDX         : integer := 0;
    constant REG_TRXCLK_READ_SPI_IDX    : integer := 1;
    constant REG_DACOSC_SPI_IDX         : integer := 2;
    constant REG_RFATT_SPI_IDX          : integer := 3;
    constant REG_PARAMETER_IDX          : integer := 4;
    constant REG_IO_IDX                 : integer := 5;
    constant REG_CTRLOUT_IDX            : integer := 6;
    constant REG_TESTCLK_IDX            : integer := 7;
    constant REG_TRX_SPI_CLK_IDX        : integer := 8;
    constant REG_IO_LOW_HIGH_IDX        : integer := 9;

    ----------------------------------------------------------
    -- Type declaration
    ----------------------------------------------------------
    type RegArray_t is array(natural range <>) of std_logic_vector(31 downto 0);

    ----------------------------------------------------------
    -- Signal declaration
    ----------------------------------------------------------
    signal av32_ReadReg_s  : RegArray_t((RADIO640_MAX_REGS-1) downto 0) := (others => (others => '0'));
    signal av32_WriteReg_s : RegArray_t((RADIO640_MAX_REGS-1) downto 0) := (others => (others => '0'));

    signal RegIndex_s       : integer range 0 to 255;

    signal Valid_s          : std_logic;
    signal ReadValid_s      : std_logic;
    signal WriteValid_s     : std_logic;
    signal WriteValidB1_s   : std_logic;
    signal WriteValidB2_s   : std_logic;
    signal WriteValidB3_s   : std_logic;
    signal WriteValidB4_s   : std_logic;


begin

    ----------------------------------------------------------
    -- Register mapping
    ----------------------------------------------------------

    ---- Reg REG_TRXCLK_SPI_IDX ----
    -- Write
    ov24_TrxClkSpiData_p                                <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(23 downto 0);
    o_TrxClkSpiCtrl_p                                   <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(24);
    o_TrxSpiStart_p                                     <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(25);
    o_ClkSpiStart_p                                     <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(26);
    o_TrxSpiClkPol_p                                    <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(30);
    -- Read
    av32_ReadReg_s(REG_TRXCLK_SPI_IDX)(23 downto 0)     <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(23 downto 0);
    av32_ReadReg_s(REG_TRXCLK_SPI_IDX)(24)              <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(24);
    av32_ReadReg_s(REG_TRXCLK_SPI_IDX)(25)              <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(25);
    av32_ReadReg_s(REG_TRXCLK_SPI_IDX)(26)              <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(26);
    av32_ReadReg_s(REG_TRXCLK_SPI_IDX)(30)              <= av32_WriteReg_s(REG_TRXCLK_SPI_IDX)(30);
    av32_ReadReg_s(REG_TRXCLK_SPI_IDX)(31)              <= i_TrxClkSpiBusy_p;


    ---- Reg REG_TRXCLK_READ_SPI_IDX ----
    -- Read
    av32_ReadReg_s(REG_TRXCLK_READ_SPI_IDX)(23 downto 0)    <= iv24_TrxClkSpiData_p;


    ---- Reg REG_DACOSC_SPI_IDX ----
    -- Write
    ov16_DacOscClkSpiData_p                             <= av32_WriteReg_s(REG_DACOSC_SPI_IDX)(15 downto 0);
    o_DacOscSpiCtrl_p                                   <= av32_WriteReg_s(REG_DACOSC_SPI_IDX)(24);
    o_DacOscSpiStart_p                                  <= av32_WriteReg_s(REG_DACOSC_SPI_IDX)(25);
    -- Read
    av32_ReadReg_s(REG_DACOSC_SPI_IDX)(15 downto 0)     <= av32_WriteReg_s(REG_DACOSC_SPI_IDX)(15 downto 0);
    av32_ReadReg_s(REG_DACOSC_SPI_IDX)(24)              <= av32_WriteReg_s(REG_DACOSC_SPI_IDX)(24);
    av32_ReadReg_s(REG_DACOSC_SPI_IDX)(25)              <= av32_WriteReg_s(REG_DACOSC_SPI_IDX)(25);
    av32_ReadReg_s(REG_DACOSC_SPI_IDX)(31)              <= i_DacOscClkSpiBusy_p;


    ---- Reg REG_RFATT_SPI_IDX ----
    -- Write
    ov16_RfAttClkSpiData_p                              <= av32_WriteReg_s(REG_RFATT_SPI_IDX)(15 downto 0);
    o_RfAttSpiCtrl_p                                    <= av32_WriteReg_s(REG_RFATT_SPI_IDX)(24);
    o_RfAttSpiStart_p                                   <= av32_WriteReg_s(REG_RFATT_SPI_IDX)(25);
    -- Read
    av32_ReadReg_s(REG_RFATT_SPI_IDX)(15 downto 0)      <= av32_WriteReg_s(REG_RFATT_SPI_IDX)(15 downto 0);
    av32_ReadReg_s(REG_RFATT_SPI_IDX)(24)               <= av32_WriteReg_s(REG_RFATT_SPI_IDX)(24);
    av32_ReadReg_s(REG_RFATT_SPI_IDX)(25)               <= av32_WriteReg_s(REG_RFATT_SPI_IDX)(25);
    av32_ReadReg_s(REG_RFATT_SPI_IDX)(31)               <= i_RfAttClkSpiBusy_p;


    ---- Reg REG_PARAMETER_IDX ----
    -- Read
    av32_ReadReg_s(REG_PARAMETER_IDX)(3 downto 0)       <= iv4_FmcConnector_p;
    av32_ReadReg_s(REG_PARAMETER_IDX)(7 downto 4)       <= iv4_BoardPosition_p;
    av32_ReadReg_s(REG_PARAMETER_IDX)(11 downto 8)      <= iv4_DesignClkRate_p;
    av32_ReadReg_s(REG_PARAMETER_IDX)(12)               <= i_ClockMaster_p;


    ---- Reg REG_IO_IDX ----
    -- Write
    o_TxnrxForce_p                                      <= av32_WriteReg_s(REG_IO_IDX)(0);
    o_TxnrxValue_p                                      <= av32_WriteReg_s(REG_IO_IDX)(1);
    o_EnableForce_p                                     <= av32_WriteReg_s(REG_IO_IDX)(2);
    o_EnableValue_p                                     <= av32_WriteReg_s(REG_IO_IDX)(3);
    o_EnAgcForce_p                                      <= av32_WriteReg_s(REG_IO_IDX)(4);
    o_EnAgcValue_p                                      <= av32_WriteReg_s(REG_IO_IDX)(5);
    o_Trx1InRxDirValue_p                                <= av32_WriteReg_s(REG_IO_IDX)(6);
    o_Trx1InRxDirForce_p                                <= av32_WriteReg_s(REG_IO_IDX)(7);
    o_Trx2InRxDirValue_p                                <= av32_WriteReg_s(REG_IO_IDX)(8);
    o_Trx2InRxDirForce_p                                <= av32_WriteReg_s(REG_IO_IDX)(9);
    ov2_ClkSwitch1_p                                    <= av32_WriteReg_s(REG_IO_IDX)(11 downto 10);
    o_ClkSwitch2_p                                      <= av32_WriteReg_s(REG_IO_IDX)(12);
    o_Ad9361ResetnPin_p                                 <= av32_WriteReg_s(REG_IO_IDX)(13);
    o_OscEnPin_p                                        <= av32_WriteReg_s(REG_IO_IDX)(14);
    o_ClkResetPin_p                                     <= av32_WriteReg_s(REG_IO_IDX)(15);
    ov4_ForceCtrlIn_p                                   <= av32_WriteReg_s(REG_IO_IDX)(19 downto 16);
    ov4_ValueCtrlIn_p                                   <= av32_WriteReg_s(REG_IO_IDX)(23 downto 20);
    o_TriggerBitSlip_p                                  <= av32_WriteReg_s(REG_IO_IDX)(24);
    ov5_TriggerDelay_p                                  <= av32_WriteReg_s(REG_IO_IDX)(29 downto 25);

    -- Read
    av32_ReadReg_s(REG_IO_IDX)(29 downto 0)             <= av32_WriteReg_s(REG_IO_IDX)(29 downto 0);


    ---- Reg REG_CTRLOUT_IDX ----
    -- Write
    o_UpdateCtrlOutLatch_p                              <= av32_WriteReg_s(REG_CTRLOUT_IDX)(0);
    -- Read
    av32_ReadReg_s(REG_CTRLOUT_IDX)(0)                  <= av32_WriteReg_s(REG_CTRLOUT_IDX)(0);
    av32_ReadReg_s(REG_CTRLOUT_IDX)(15 downto 8)        <= iv8_CtrlOutCurrent_p;
    av32_ReadReg_s(REG_CTRLOUT_IDX)(23 downto 16)       <= iv8_CtrlOutLow_p;
    av32_ReadReg_s(REG_CTRLOUT_IDX)(31 downto 24)       <= iv8_CtrlOutHigh_p;


    ---- Reg REG_TESTCLK_IDX ----
    -- Write
    ov32_TestClkSel_p                                   <= av32_WriteReg_s(REG_TESTCLK_IDX)(31 downto 0);
    -- Read
    av32_ReadReg_s(REG_TESTCLK_IDX)(31 downto 0)        <= iv32_TestClkFreq_p;

    ---- Reg REG_TRX_SPI_CLK_IDX ----
    -- Write
    ov8_TrxSpiClkDownsampling_p                         <= av32_WriteReg_s(REG_TRX_SPI_CLK_IDX)(7 downto 0);
    -- Read
    av32_ReadReg_s(REG_TRX_SPI_CLK_IDX)(7 downto 0)     <= av32_WriteReg_s(REG_TRX_SPI_CLK_IDX)(7 downto 0);


    ---- Reg REG_IO_LOW_HIGH_IDX ----
    -- Write
    o_UpdateTriggerLatch_p                              <= av32_WriteReg_s(REG_IO_LOW_HIGH_IDX)(0);
    o_UpdateInputSyncLatch_p                            <= av32_WriteReg_s(REG_IO_LOW_HIGH_IDX)(3);
    o_UpdateOutputSyncLatch_p                           <= av32_WriteReg_s(REG_IO_LOW_HIGH_IDX)(4);
    -- Read
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(0)              <= av32_WriteReg_s(REG_IO_LOW_HIGH_IDX)(0);
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(1)              <= i_TriggerLow_p;
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(2)              <= i_TriggerHigh_p;
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(3)              <= av32_WriteReg_s(REG_IO_LOW_HIGH_IDX)(3);
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(4)              <= av32_WriteReg_s(REG_IO_LOW_HIGH_IDX)(4);
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(5)              <= i_InputSync_p;
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(6)              <= i_InputSyncLow_p;
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(7)              <= i_InputSyncHigh_p;
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(8)              <= i_OutputSyncLow_p;
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(9)              <= i_OutputSyncHigh_p;
    av32_ReadReg_s(REG_IO_LOW_HIGH_IDX)(10)             <= i_Trigger_p;

    
    ----------------------------------------------------------
    -- Register access
    ----------------------------------------------------------

    -- Address parsing
    Valid_s         <= i_up_sel_p when (iv14_up_addr_p(13 downto 8) = RADIO640_REGS_BASE_ADDR) else '0';
    RegIndex_s      <= to_integer(unsigned(iv14_up_addr_p(7 downto 0)));

    -- Read or Write transaction
    ReadValid_s     <= Valid_s when (RegIndex_s < RADIO640_MAX_REGS) else '0';
    WriteValid_s    <= (Valid_s and i_up_wr_p) when (RegIndex_s < RADIO640_MAX_REGS) else '0';

    -- Write with Byte mask
    WriteValidB1_s  <= WriteValid_s and iv4_up_wstrb_p(0);
    WriteValidB2_s  <= WriteValid_s and iv4_up_wstrb_p(1);
    WriteValidB3_s  <= WriteValid_s and iv4_up_wstrb_p(2);
    WriteValidB4_s  <= WriteValid_s and iv4_up_wstrb_p(3);

    process (i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then

            -- Default value to 0 since all slaves are ORed together
            ov32_up_rdata_p <= (others => '0');

            o_up_ack_p      <= Valid_s;

            if ReadValid_s = '1' then
                ov32_up_rdata_p <= av32_ReadReg_s(RegIndex_s);
            end if;

            if WriteValidB1_s = '1' then
                av32_WriteReg_s(RegIndex_s)(7 downto 0) <= iv32_up_wdata_p(7 downto 0);
            end if;
            if WriteValidB2_s = '1' then
                av32_WriteReg_s(RegIndex_s)(15 downto 8) <= iv32_up_wdata_p(15 downto 8);
            end if;
            if WriteValidB3_s = '1' then
                av32_WriteReg_s(RegIndex_s)(23 downto 16) <= iv32_up_wdata_p(23 downto 16);
            end if;
            if WriteValidB4_s = '1' then
                av32_WriteReg_s(RegIndex_s)(31 downto 24) <= iv32_up_wdata_p(31 downto 24);
            end if;

            -- Synchronous reset
            if i_Rst_p = '1' then

                ov32_up_rdata_p <= (others => '0');
                o_up_ack_p      <= '0';

                -- Default register values
                for I in 0 to (RADIO640_MAX_REGS-1) loop
                    av32_WriteReg_s(I) <= (others => '0');
                end loop;
                
                -- Set o_ClkResetPin_p to reset by default
                av32_WriteReg_s(REG_IO_IDX)(15) <= '1';

            end if;
        end if;
    end process;

end architecture rtl;
