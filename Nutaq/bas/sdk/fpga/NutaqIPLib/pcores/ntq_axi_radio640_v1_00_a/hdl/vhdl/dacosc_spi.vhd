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
-- File        : $Id: dacosc_spi.vhd,v 1.2 2015/11/12 14:51:13 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : SPI cores for the Radio640
--------------------------------------------------------------------------------
-- Copyright (c) 2015 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: dacosc_spi.vhd,v $
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

entity dacosc_spi is
    port (
    -- AXI register
    i_Rst_p                    : in  std_logic;
    i_SystemClk_p              : in  std_logic;

    i_regDacOscSpiCtrl_p       : in  std_logic;
    iv16_regDacOscClkSpiData_p : in  std_logic_vector(15 downto 0);
    i_regDacOscSpiStart_p      : in  std_logic;
    o_regDacOscClkSpiBusy_p    : out std_logic;

    -- User Logic
    i_DesignClk_p              : in std_logic;

    iv16_ulDacOscSpiData_p     : in  std_logic_vector(15 downto 0);
    i_ulDacOscSpiStart_p       : in  std_logic;
    o_ulDacOscSpiBusy_p        : out std_logic;
    o_ulDacOscSpiDone_p        : out std_logic := '0';

    -- External interface
    o_OscDacSpiCsnPin_p        : out std_logic;
    o_OscDacSpiClkPin_p        : out std_logic;
    o_OscDacSpiMosiPin_p       : out std_logic
    );
end entity dacosc_spi;

architecture arch of dacosc_spi is

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal v16_uldacosc_spiDataIn_s           : std_logic_vector(15 downto 0) := (others => '0');
  signal v16_dacosc_spiDataIn_s             : std_logic_vector(15 downto 0) := (others => '0');
    
  signal dacosc_SpiStart_s                  : std_logic := '0';
  signal regDacOscSpiCtrl_DesignClk_s       : std_logic := '0';
  signal dacosc_spiBusy_DesignClk_s         : std_logic := '0';
  signal ulDacOscSpiStart_Pending_s         : std_logic := '0';
  signal ulDacOscSpiResult_Pending_s        : std_logic := '0';
  signal ulDacOscSpiBusy_s                  : std_logic := '1';
  signal dacosc_spiBusy_s                   : std_logic := '1';

begin

    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then
        
            -- Default value
            o_ulDacOscSpiDone_p <= '0';
            
            regDacOscSpiCtrl_DesignClk_s <= i_regDacOscSpiCtrl_p;
            dacosc_spiBusy_DesignClk_s   <= dacosc_spiBusy_s;
            
            -- Clear busy signal and update read value
            if (ulDacOscSpiStart_Pending_s = '0') and (dacosc_spiBusy_DesignClk_s = '0') then
                
                -- Generate a pulse to tell the user logic the transaction is over
                -- The data has been written or the read data is available at the port
                if ulDacOscSpiResult_Pending_s = '1' then
                    o_ulDacOscSpiDone_p        <= '1';
                end if;
                
                ulDacOscSpiResult_Pending_s    <= '0';
                ulDacOscSpiBusy_s              <= '0';
            end if;
            
            -- If FPGA has control and not busy, be sensitive to SPI Start
            if (ulDacOscSpiBusy_s = '0') and (regDacOscSpiCtrl_DesignClk_s = '0') and (i_ulDacOscSpiStart_p = '1') then
                v16_uldacosc_spiDataIn_s <= iv16_ulDacOscSpiData_p;
                
                ulDacOscSpiBusy_s <= '1';
                ulDacOscSpiStart_Pending_s <= '1';
                ulDacOscSpiResult_Pending_s <= '1';
            end if;
            
            -- If the spi core is busy, clear the start pending
            if (dacosc_spiBusy_DesignClk_s = '1') then
                ulDacOscSpiStart_Pending_s <= '0';
            end if;
            
        end if;
    end process;
  
    o_ulDacOscSpiBusy_p <= ulDacOscSpiBusy_s or regDacOscSpiCtrl_DesignClk_s;
    
    v16_dacosc_spiDataIn_s <= v16_uldacosc_spiDataIn_s when ulDacOscSpiStart_Pending_s = '1' 
        else iv16_regDacOscClkSpiData_p;
        
    dacosc_SpiStart_s <= ulDacOscSpiStart_Pending_s or i_regDacOscSpiStart_p;

    -------------------------------
    -- Instantiate SPI registers
    -------------------------------

    dacosc_spi_i: entity ntq_axi_radio640_v1_00_a.spi_register    --50MHz Max
    generic map(
        SPI_NUMBER_CS => 1,
        SPI_DATA_WIDTH  => 16
    )
    port map (
        -- SPI Interface
        i_Clk_p            => i_SystemClk_p,
        i_Rst_p            => i_Rst_p,
        
        iv8_spiClkDownsampling_p => X"02",
        
        iv_spiStart_p(0)   => dacosc_SpiStart_s,
        i_spiClkPol_p      => '1',                         -- Update data on clock falling edge
        i_spiMsbf_p        => '1',                         -- MSB first
        iv_spiDataIn_p     => v16_dacosc_spiDataIn_s,
        ov_spiDataOut_p    => open,
        o_spiBusy_p        => dacosc_spiBusy_s,
        o_spiEnd_p         => open,

        o_spiClkOutPin_p   => o_OscDacSpiClkPin_p,
        ov_spiCsPin_p(0)   => o_OscDacSpiCsnPin_p,
        o_spiMosiPin_p     => o_OscDacSpiMosiPin_p,
        i_spiMisoPin_p     => '0'
    );
    
    o_regDacOscClkSpiBusy_p    <= dacosc_spiBusy_s;

end architecture;
