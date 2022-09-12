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
-- File        : $Id: rfatt_spi.vhd,v 1.2 2015/11/12 14:51:13 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : SPI cores for the Radio640
--------------------------------------------------------------------------------
-- Copyright (c) 2015 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: rfatt_spi.vhd,v $
-- Revision 1.2  2015/11/12 14:51:13  julien.roy
-- Modify SPI core to have dynamicaly configurable SPI CLK speed
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

entity rfatt_spi is
    port (
    -- AXI register
    i_Rst_p                     : in  std_logic;
    i_SystemClk_p               : in  std_logic;

    i_regRfAttSpiCtrl_p         : in  std_logic;
    iv16_regRfAttClkSpiData_p   : in  std_logic_vector(15 downto 0);
    i_regRfAttSpiStart_p        : in  std_logic;
    o_regRfAttClkSpiBusy_p      : out std_logic;

    -- User Logic
    i_DesignClk_p               : in std_logic;

    iv7_ulRfAttSpiData_p        : in  std_logic_vector(6 downto 0);
    i_ulRfAtt1SpiStart_p        : in  std_logic;
    i_ulRfAtt2SpiStart_p        : in  std_logic;
    o_ulRfAttSpiBusy_p          : out std_logic;
    o_ulRfAttSpiDone_p          : out std_logic := '0';

    -- External interface
    o_RfAttSpiLePin_p           : out std_logic;
    o_RfAttSpiClkPin_p          : out std_logic;
    o_RfAttSpiMosiPin_p         : out std_logic
    );
end entity rfatt_spi;

architecture arch of rfatt_spi is

  constant RF_ATT_1_ADDR                    : std_logic_vector(2 downto 0) := "000";
  constant RF_ATT_2_ADDR                    : std_logic_vector(2 downto 0) := "001";

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal v16_ulrfatt_spiDataIn_s            : std_logic_vector(15 downto 0) := (others => '0');
  signal v16_rfatt_spiDataIn_s              : std_logic_vector(15 downto 0) := (others => '0');

  signal rfatt_SpiStart_s                   : std_logic := '0';
  signal regRfAttSpiCtrl_DesignClk_s        : std_logic := '0';
  signal rfatt_spiBusy_DesignClk_s          : std_logic := '0';
  signal ulRfAttSpiStart_Pending_s          : std_logic := '0';
  signal ulRfAttSpiResult_Pending_s         : std_logic := '0';
  signal ulRfAttSpiBusy_s                   : std_logic := '1';
  signal rfatt_spiBusy_s                    : std_logic := '1';

begin

    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then

            -- Default value
            o_ulRfAttSpiDone_p <= '0';

            regRfAttSpiCtrl_DesignClk_s <= i_regRfAttSpiCtrl_p;
            rfatt_spiBusy_DesignClk_s   <= rfatt_spiBusy_s;

            -- Clear busy signal and update read value
            if (ulRfAttSpiStart_Pending_s = '0') and (rfatt_spiBusy_DesignClk_s = '0') then

                -- Generate a pulse to tell the user logic the transaction is over
                -- THe data has been written or the read data is available at the port
                if ulRfAttSpiResult_Pending_s = '1' then
                    o_ulRfAttSpiDone_p        <= '1';
                end if;

                ulRfAttSpiResult_Pending_s    <= '0';
                ulRfAttSpiBusy_s              <= '0';
            end if;

            -- If FPGA has control and not busy, be sensitive to SPI Start
            if (ulRfAttSpiBusy_s = '0') and (regRfAttSpiCtrl_DesignClk_s = '0')
                and ((i_ulRfAtt1SpiStart_p = '1') or (i_ulRfAtt2SpiStart_p = '1')) then

                if (i_ulRfAtt1SpiStart_p = '1') then
                    v16_ulrfatt_spiDataIn_s(10 downto 8)    <= RF_ATT_1_ADDR;
                else
                    v16_ulrfatt_spiDataIn_s(10 downto 8)    <= RF_ATT_2_ADDR;
                end if;

                v16_ulrfatt_spiDataIn_s(15 downto 11)   <= (others => '0');
                v16_ulrfatt_spiDataIn_s(7)              <= '0';
                v16_ulrfatt_spiDataIn_s(6 downto 0)     <= iv7_ulRfAttSpiData_p;

                ulRfAttSpiBusy_s <= '1';
                ulRfAttSpiStart_Pending_s <= '1';
                ulRfAttSpiResult_Pending_s <= '1';
            end if;

            -- If the spi core is busy, clear the start pending
            if (rfatt_spiBusy_DesignClk_s = '1') then
                ulRfAttSpiStart_Pending_s <= '0';
            end if;

        end if;
    end process;

    o_ulRfAttSpiBusy_p <= ulRfAttSpiBusy_s or regRfAttSpiCtrl_DesignClk_s;

    v16_rfatt_spiDataIn_s <= v16_ulrfatt_spiDataIn_s when ulRfAttSpiStart_Pending_s = '1'
        else iv16_regRfAttClkSpiData_p;

    rfatt_SpiStart_s <= ulRfAttSpiStart_Pending_s or i_regRfAttSpiStart_p;

    -------------------------------
    -- Instantiate SPI registers
    -------------------------------

    rfatt_spi_i: entity ntq_axi_radio640_v1_00_a.spi_register    --10MHz Max
    generic map(
        SPI_NUMBER_CS => 1,
        SPI_DATA_WIDTH  => 16
    )
    port map (
        -- SPI Interface
        i_Clk_p            => i_SystemClk_p,
        i_Rst_p            => i_Rst_p,
        
        iv8_spiClkDownsampling_p => X"0A",
        
        iv_spiStart_p(0)   => rfatt_SpiStart_s,
        i_spiClkPol_p      => '1',                     -- Update data on clock falling edge
        i_spiMsbf_p        => '0',                     -- LSB first
        iv_spiDataIn_p     => v16_rfatt_spiDataIn_s,
        ov_spiDataOut_p    => open,
        o_spiBusy_p        => rfatt_spiBusy_s,
        o_spiEnd_p         => o_RfAttSpiLePin_p,

        o_spiClkOutPin_p   => o_RfAttSpiClkPin_p,
        ov_spiCsPin_p      => open,
        o_spiMosiPin_p     => o_RfAttSpiMosiPin_p,
        i_spiMisoPin_p     => '0'
    );

    o_regRfAttClkSpiBusy_p    <= rfatt_spiBusy_s;

end architecture;
