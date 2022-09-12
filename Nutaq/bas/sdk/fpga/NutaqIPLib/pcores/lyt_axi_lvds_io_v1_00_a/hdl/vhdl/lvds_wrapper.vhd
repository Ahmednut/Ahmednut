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
-- File        : $Id: lvds_wrapper.vhd,v 1.1 2015/04/10 19:47:33 sqa Exp $
--------------------------------------------------------------------------------
-- Description :
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lvds_wrapper.vhd,v $
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.6  2013/02/08 14:04:36  julien.roy
-- Add IDELAY to ease timing in sync mode
--
-- Revision 1.5  2013/01/30 19:52:04  julien.roy
-- Modify LVDS example for Synchronous examples
--
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

library lyt_axi_lvds_io_v1_00_a;
use lyt_axi_lvds_io_v1_00_a.all;

library UNISIM;
use UNISIM.VComponents.all;

entity lvds_wrapper is
  generic(
    USE_SYNCHRONOUS_GPIO_GROUP0   : integer := 1;
    USE_SYNCHRONOUS_GPIO_GROUP1   : integer := 1;
    DATA_IDELAY_VALUE_GROUP0      : integer := 0;
    CLK_IDELAY_VALUE_GROUP0       : integer := 0;
    DATA_IDELAY_VALUE_GROUP1      : integer := 0;
    CLK_IDELAY_VALUE_GROUP1       : integer := 0
  );
  port (
    i_rst_p                     : in std_logic;              -- main async reset
    i_SystemClk_p               : in std_logic;
    i_RefClk200MHz_p            : in std_logic;
    i_UserClk_p                 : in std_logic;

    i_directionGroup0_p         : in std_logic;   -- IO direction
    i_directionGroup1_p         : in std_logic;   -- IO direction

    -- internal data interface
    iv_UserLvdsGroup0_p       : in  std_logic_vector(15 - USE_SYNCHRONOUS_GPIO_GROUP0*2  downto 0);
    iv_UserLvdsGroup1_p       : in  std_logic_vector(15 - USE_SYNCHRONOUS_GPIO_GROUP1*2  downto 0);
    ov_UserLvdsGroup0_p       : out std_logic_vector(15 - USE_SYNCHRONOUS_GPIO_GROUP0*2 downto 0);
    ov_UserLvdsGroup1_p       : out std_logic_vector(15 - USE_SYNCHRONOUS_GPIO_GROUP1*2 downto 0);

    -- fifo flag and control ( NOT ACTIVE in GPIO mode )
    -- TX FIFO 0
    i_TxClkGroup0_p             : in  std_logic;
    i_inWrEnGroup0_p            : in  std_logic;
    o_inWrAckGroup0_p           : out std_logic;
    o_fullGroup0_p              : out std_logic;
    -- RX FIFO 0
    i_outRdEnGroup0_p           : in  std_logic;
    o_outValidGroup0_p          : out std_logic;
    o_emptyGroup0_p             : out std_logic;
    o_RxClkGroup0_p             : out std_logic;
    -- TX FIFO 1
    i_TxClkGroup1_p             : in  std_logic;
    i_inWrEnGroup1_p            : in  std_logic;
    o_inWrAckGroup1_p           : out std_logic;
    o_fullGroup1_p              : out std_logic;
    -- RX FIFO 1
    i_outRdEnGroup1_p           : in  std_logic;
    o_outValidGroup1_p          : out std_logic;
    o_emptyGroup1_p             : out std_logic;
    o_RxClkGroup1_p             : out std_logic;

    -- external interface
    iov16_Group0_padIOp_p       : inout std_logic_vector(15 downto 0);
    iov16_Group1_padIOp_p       : inout std_logic_vector(15 downto 0);
    iov16_Group0_padIOn_p       : inout std_logic_vector(15 downto 0);
    iov16_Group1_padIOn_p       : inout std_logic_vector(15 downto 0)
  );
end lvds_wrapper;

architecture Behavioral of lvds_wrapper is

begin

  -- IDELAYCTRL instance
  idelayctrl_inst : IDELAYCTRL
    port map (
      RDY 	 => open,
      REFCLK => i_RefClk200MHz_p,
      RST 	 => i_rst_p
    );

  -- Group 0
  -- generate strait GPIO
  GEN_USE_SYNCHRONOUS_GPIO_GROUP0n : if USE_SYNCHRONOUS_GPIO_GROUP0 = 0 generate

    lvds16Gpio_group0 : entity lyt_axi_lvds_io_v1_00_a.lvds16gpio
      generic map(
        DATA_IDELAY_VALUE     => DATA_IDELAY_VALUE_GROUP0
      )
      port map(
        i_SystemClk_p         => i_SystemClk_p,
        i_clock_p             => i_UserClk_p,
        i_direction_p         => i_directionGroup0_p,
        iv16_input_p          => iv_UserLvdsGroup0_p,
        ov16_output_p         => ov_UserLvdsGroup0_p,
        lvdsIO_p              => iov16_Group0_padIOp_p,
        lvdsIO_n              => iov16_Group0_padIOn_p
      );

    o_inWrAckGroup0_p  <= '1';
    o_outValidGroup0_p <= '1';
    o_emptyGroup0_p    <= '1';
    o_fullGroup0_p     <= '1';

  end generate GEN_USE_SYNCHRONOUS_GPIO_GROUP0n;

  -- generate link with clock and data valid
  GEN_USE_SYNCHRONOUS_GPIO_GROUP0 : if USE_SYNCHRONOUS_GPIO_GROUP0 = 1 generate

    lvds14sync_group0 : entity lyt_axi_lvds_io_v1_00_a.lvds14sync
      generic map(
        DATA_IDELAY_VALUE     => DATA_IDELAY_VALUE_GROUP0,
        CLK_IDELAY_VALUE      => CLK_IDELAY_VALUE_GROUP0
      )
      port map(
        i_rst_p               => i_rst_p,
        i_SystemClk_p         => i_SystemClk_p,
        i_clock_p             => i_UserClk_p,
        i_txClock_p           => i_TxClkGroup0_p,
        o_rxClock_p           => o_RxClkGroup0_p,

        i_direction_p         => i_directionGroup0_p,

        iv14_input_p          => iv_UserLvdsGroup0_p,
        i_inWrEn_p            => i_inWrEnGroup0_p,
        o_inWrAck_p           => o_inWrAckGroup0_p,
        o_full_p              => o_fullGroup0_p,

        ov14_output_p         => ov_UserLvdsGroup0_p,
        i_outRdEn_p           => i_outRdEnGroup0_p,
        o_outValid_p          => o_outValidGroup0_p,
        o_empty_p             => o_emptyGroup0_p,

        lvdsClk_p             => iov16_Group0_padIOp_p(0),
        lvdsClk_n             => iov16_Group0_padIOn_p(0),
        lvdsValid_p           => iov16_Group0_padIOp_p(1),
        lvdsValid_n           => iov16_Group0_padIOn_p(1),
        lvdsDataIO_p          => iov16_Group0_padIOp_p(15 downto 2),
        lvdsDataIO_n          => iov16_Group0_padIOn_p(15 downto 2)
      );

  end generate GEN_USE_SYNCHRONOUS_GPIO_GROUP0;

  -- Group 1
  -- generate strait GPIO
  GEN_USE_SYNCHRONOUS_GPIO_GROUP1n : if USE_SYNCHRONOUS_GPIO_GROUP1 = 0 generate

    lvds16Gpio_group1 : entity lyt_axi_lvds_io_v1_00_a.lvds16Gpio
      generic map(
        DATA_IDELAY_VALUE     => DATA_IDELAY_VALUE_GROUP1
      )
      port map(
        i_SystemClk_p         => i_SystemClk_p,
        i_clock_p             => i_UserClk_p,
        i_direction_p         => i_directionGroup1_p,
        iv16_input_p          => iv_UserLvdsGroup1_p,
        ov16_output_p         => ov_UserLvdsGroup1_p,
        lvdsIO_p              => iov16_Group1_padIOp_p,
        lvdsIO_n              => iov16_Group1_padIOn_p
      );

    o_inWrAckGroup1_p  <= '1';
    o_outValidGroup1_p <= '1';
    o_emptyGroup1_p    <= '1';
    o_fullGroup1_p     <= '1';

  end generate GEN_USE_SYNCHRONOUS_GPIO_GROUP1n;

  -- generate link with clock and data valid
  GEN_USE_SYNCHRONOUS_GPIO_GROUP1 : if USE_SYNCHRONOUS_GPIO_GROUP1 = 1 generate

    lvds14sync_group1 : entity lyt_axi_lvds_io_v1_00_a.lvds14sync
      generic map(
        DATA_IDELAY_VALUE     => DATA_IDELAY_VALUE_GROUP1,
        CLK_IDELAY_VALUE      => CLK_IDELAY_VALUE_GROUP1
      )
      port map(
        i_rst_p               => i_rst_p,
        i_SystemClk_p         => i_SystemClk_p,
        i_clock_p             => i_UserClk_p,
        i_txClock_p           => i_TxClkGroup1_p,
        o_rxClock_p           => o_RxClkGroup1_p,

        i_direction_p         => i_directionGroup1_p,

        iv14_input_p          => iv_UserLvdsGroup1_p,
        i_inWrEn_p            => i_inWrEnGroup1_p,
        o_inWrAck_p           => o_inWrAckGroup1_p,
        o_full_p              => o_fullGroup1_p,

        ov14_output_p         => ov_UserLvdsGroup1_p,
        i_outRdEn_p           => i_outRdEnGroup1_p,
        o_outValid_p          => o_outValidGroup1_p,
        o_empty_p             => o_emptyGroup1_p,
        
        lvdsClk_p             => iov16_Group1_padIOp_p(0),
        lvdsClk_n             => iov16_Group1_padIOn_p(0),
        lvdsValid_p           => iov16_Group1_padIOp_p(1),
        lvdsValid_n           => iov16_Group1_padIOn_p(1),
        lvdsDataIO_p          => iov16_Group1_padIOp_p(15 downto 2),
        lvdsDataIO_n          => iov16_Group1_padIOn_p(15 downto 2)
      );

  end generate GEN_USE_SYNCHRONOUS_GPIO_GROUP1;

end Behavioral;

