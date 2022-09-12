
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
-- File        : $Id: trxclk_spi.vhd,v 1.5 2016/03/17 19:07:12 christian.dufour-bou Exp $
--------------------------------------------------------------------------------
-- Description : SPI cores for the Radio640
--------------------------------------------------------------------------------
-- Copyright (c) 2015 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: trxclk_spi.vhd,v $
-- Revision 1.5  2016/03/17 19:07:12  christian.dufour-bou
-- typos
--
-- Revision 1.4  2016/03/17 19:02:17  christian.dufour-bou
-- Corrected clock domain crossing between design clock and system clock
--
-- Revision 1.3  2015/11/12 14:51:13  julien.roy
-- Modify SPI core to have dynamicaly configurable SPI CLK speed
--
-- Revision 1.2  2015/10/30 14:10:23  julien.roy
-- Set the SPI speed for the AD9361 to 25 MHz since it is unstable at 50 mHz on the second FMC connector on the Perseus611X carrier board.
--
-- Revision 1.1  2015/10/20 19:14:44  julien.roy
-- Add first revision of Radio640 fpga pcore.
--
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library ntq_axi_radio640_v1_00_a;
 use ntq_axi_radio640_v1_00_a.all;

library unisim;
  use unisim.vcomponents.all;

entity trxclk_spi is
    port (
    -- AXI register
    i_Rst_p                    : in  std_logic;
    i_SystemClk_p              : in  std_logic;

    iv8_spiClkDownsampling_p   : in  std_logic_vector(7 downto 0);
    i_regTrxSpiClkPol_p        : in  std_logic;
    i_regTrxClkSpiCtrl_p       : in  std_logic;
    iv24_regTrxClkSpiData_p    : in  std_logic_vector(23 downto 0);
    ov24_regTrxClkSpiData_p    : out std_logic_vector(23 downto 0);
    i_regTrxSpiStart_p         : in  std_logic;
    i_regClkSpiStart_p         : in  std_logic;
    o_regTrxClkSpiBusy_p       : out std_logic;

    -- User Logic
    i_DesignClk_p              : in std_logic;

    iv10_ulTrxSpiAddr_p        : in  std_logic_vector(9 downto 0);
    iv8_ulTrxSpiData_p         : in  std_logic_vector(7 downto 0);
    ov8_ulTrxSpiData_p         : out std_logic_vector(7 downto 0) := (others => '0');
    i_ulTrxSpiWrnrd_p          : in  std_logic;
    i_ulTrxSpiStart_p          : in  std_logic;
    o_ulTrxSpiBusy_p           : out std_logic;
    o_ulTrxSpiDone_p           : out std_logic := '0';

    -- External interface
    o_TrxSpiCsnPin_p           : out std_logic;
    o_ClkSpiCsnPin_p           : out std_logic;
    o_TrxClkSpiClkPin_p        : out std_logic;
    o_TrxClkSpiMosiPin_p       : out std_logic;
    i_TrxClkSpiMisoPin_p       : in  std_logic
    );
end entity trxclk_spi;

architecture arch of trxclk_spi is

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal v24_ultrxclk_spiDataIn_DesignClk_s     : std_logic_vector(23 downto 0) := (others => '0');
  signal v24_ultrxclk_spiDataIn_SystemClk_s     : std_logic_vector(23 downto 0) := (others => '0');
  signal v24_trxclk_spiDataIn_s                 : std_logic_vector(23 downto 0) := (others => '0');
  signal v24_trxclk_spiDataOut_s                : std_logic_vector(23 downto 0) := (others => '0');
          
  signal v2_trxclk_SpiStart_s                   : std_logic_vector(1 downto 0) := (others => '0');
  signal v2_TrxClkSpiCsnPin_s                   : std_logic_vector(1 downto 0) := (others => '0');
          
  signal regTrxClkSpiCtrl_DesignClk_s           : std_logic := '0';
  signal trxclk_spiBusy_DesignClk_s             : std_logic := '0';
  signal trxclk_spiBusy_s                       : std_logic := '1';
  signal trxclk_spiEnd_s                        : std_logic := '0';
  signal ulTrxSpiStart_Pending_DesignClk_s      : std_logic := '0';
  signal ulTrxSpiStart_Pending_SystemClk_s      : std_logic := '0';
  signal ulTrxSpiResult_Pending_s               : std_logic := '0';
  signal ulTrxSpiStart_s                        : std_logic := '0';
  signal ulTrxSpiBusy_s                         : std_logic := '1';
  signal regTrxSpiClkPol_s                      : std_logic := '1';

begin

    -- ulTrxSpiBusy_s is one when start from user logic
    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then
        
            -- Default value
            o_ulTrxSpiDone_p <= '0';
            
            regTrxClkSpiCtrl_DesignClk_s <= i_regTrxClkSpiCtrl_p;
            
            -- Latching SPI busy from SPI register with the design clock
            trxclk_spiBusy_DesignClk_s   <= trxclk_spiBusy_s;
            
            -- Clear busy signal and update read value when no start asked from user logic
            -- and SPI register not busy
            if (ulTrxSpiStart_Pending_DesignClk_s = '0') and (trxclk_spiBusy_DesignClk_s = '0') then
                
                -- Generate a pulse to tell the user logic the transaction is over
                -- The data has been written or the read data is available at the port
                if ulTrxSpiResult_Pending_s = '1' then
                    o_ulTrxSpiDone_p        <= '1';
                    ov8_ulTrxSpiData_p          <= v24_trxclk_spiDataOut_s(7 downto 0);
                end if;
                
                ulTrxSpiResult_Pending_s    <= '0';
                ulTrxSpiBusy_s              <= '0';
            end if;
            
            
            -- If SPI not busy and FPGA has control, be sensitive to SPI start
            
            -- Then, SPI becomes busy,
            -- Start pending,
            -- Result pending
            if (ulTrxSpiBusy_s = '0') and (regTrxClkSpiCtrl_DesignClk_s = '0') and (i_ulTrxSpiStart_p = '1') then
                v24_ultrxclk_spiDataIn_DesignClk_s(23)            <= i_ulTrxSpiWrnrd_p;
                v24_ultrxclk_spiDataIn_DesignClk_s(22 downto 18)  <= (others => '0');
                v24_ultrxclk_spiDataIn_DesignClk_s(17 downto 8)   <= iv10_ulTrxSpiAddr_p;
                v24_ultrxclk_spiDataIn_DesignClk_s(7 downto 0)    <= iv8_ulTrxSpiData_p;
                
                ulTrxSpiBusy_s <= '1';
                ulTrxSpiStart_Pending_DesignClk_s <= '1';
                ulTrxSpiResult_Pending_s <= '1';
            end if;
            
            -- If the spi core is busy (design clock-latched busy signal coming from the SPI register), 
            -- clear the start pending
            if (trxclk_spiBusy_DesignClk_s = '1') then
                ulTrxSpiStart_Pending_DesignClk_s <= '0';
            end if;
        end if;
    end process;
    
    -- Clock domain crossing 
    process(i_SystemClk_p)
    begin
        if rising_edge(i_SystemClk_p) then
            ulTrxSpiStart_Pending_SystemClk_s <= ulTrxSpiStart_Pending_DesignClk_s;
            ulTrxSpiStart_s <= '0';
            -- Latch SPI Data only when Start pending from the design clock domain
            -- has been stable for one system clock period
            if ulTrxSpiStart_Pending_SystemClk_s = '1' then
                v24_ultrxclk_spiDataIn_SystemClk_s  <= v24_ultrxclk_spiDataIn_DesignClk_s;
                ulTrxSpiStart_s <= '1';
            end if;   
            
            if ulTrxSpiStart_s = '1' then
                ulTrxSpiStart_s <= '0';
            end if;
        end if;
    end process;

    o_ulTrxSpiBusy_p <= ulTrxSpiBusy_s or regTrxClkSpiCtrl_DesignClk_s;
    
    -- SPI Data in multiplexer. 
    v24_trxclk_spiDataIn_s <= v24_ultrxclk_spiDataIn_SystemClk_s when ulTrxSpiResult_Pending_s = '1' 
        else iv24_regTrxClkSpiData_p;       
        
    regTrxSpiClkPol_s <= '0' when ulTrxSpiResult_Pending_s = '1' 
        else i_regTrxSpiClkPol_p;
        
    --v2_trxclk_SpiStart_s(0) <= ulTrxSpiStart_Pending_s or i_regTrxSpiStart_p;
    v2_trxclk_SpiStart_s(0) <= ulTrxSpiStart_s or i_regTrxSpiStart_p;
    v2_trxclk_SpiStart_s(1) <= i_regClkSpiStart_p;

    -------------------------------
    -- Instantiate SPI registers
    -------------------------------

    trxclk_spi_i: entity ntq_axi_radio640_v1_00_a.spi_register    --50MHz Max
    generic map(
        SPI_NUMBER_CS => 2,
        SPI_DATA_WIDTH  => 24
    )
    port map (
        -- SPI Interface
        i_Clk_p            => i_SystemClk_p,
        i_Rst_p            => i_Rst_p,
        
        iv8_spiClkDownsampling_p => iv8_spiClkDownsampling_p,
        
        iv_spiStart_p      => v2_trxclk_SpiStart_s,
        i_spiClkPol_p      => regTrxSpiClkPol_s,             -- Update data on clock falling edge ('1') for HMC7043 and rising edge ('0') for AD9361
        i_spiMsbf_p        => '1',                             -- MSB first
        iv_spiDataIn_p     => v24_trxclk_spiDataIn_s,
        ov_spiDataOut_p    => v24_trxclk_spiDataOut_s,
        o_spiBusy_p        => trxclk_spiBusy_s,
        o_spiEnd_p         => trxclk_spiEnd_s,

        o_spiClkOutPin_p   => o_TrxClkSpiClkPin_p,
        ov_spiCsPin_p      => v2_TrxClkSpiCsnPin_s,
        o_spiMosiPin_p     => o_TrxClkSpiMosiPin_p,
        i_spiMisoPin_p     => i_TrxClkSpiMisoPin_p
    );
    
    o_TrxSpiCsnPin_p <= v2_TrxClkSpiCsnPin_s(0);
    o_ClkSpiCsnPin_p <= v2_TrxClkSpiCsnPin_s(1);
    
    o_regTrxClkSpiBusy_p    <= trxclk_spiBusy_s;
    ov24_regTrxClkSpiData_p <= v24_trxclk_spiDataOut_s;

end architecture;
