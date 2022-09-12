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
-- File        : $Id: lyt_lvds_gpio.vhd,v 1.2 2015/11/27 19:32:52 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description :
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_lvds_gpio.vhd,v $
-- Revision 1.2  2015/11/27 19:32:52  julien.roy
-- Add Save attribute to avoid optimization that will remove the T connection.
-- The T connection is required when the IODELAY configured in IO direction.
--
-- Revision 1.1  2015/04/10 19:48:59  sqa
-- bas first commit
--
-- Revision 1.1  2013/09/24 14:46:44  julien.roy
-- Add lyt_lvds_gpio_v1_00_a core
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

library unisim;
  use unisim.vcomponents.all;

entity lyt_lvds_gpio is
    generic
    (
        DATA_WIDTH              : integer range 1 to 32 := 16;
        DATA_INPUT_DELAY        : integer range 0 to 31 := 0;
        DATA_OUTPUT_DELAY       : integer range 0 to 31 := 0
    );
    port
    (
        -- Register ports
        i_SystemClk_p           : in  std_logic;
        ov32_InfoRegister_p     : out std_logic_vector(31 downto 0);
        iv32_CtrlRegister_p     : in  std_logic_vector(31 downto 0);
        iv32_ValueRegister_p    : in  std_logic_vector(31 downto 0);
        ov32_ValueRegister_p    : out std_logic_vector(31 downto 0);
        iv32_OERegister_p       : in  std_logic_vector(31 downto 0);
        -- User ports
        iv_Data_p               : in  std_logic_vector(DATA_WIDTH-1 downto 0) := (others => '0');
        ov_Data_p               : out std_logic_vector(DATA_WIDTH-1 downto 0);
        iv_OutputEnable_p       : in  std_logic_vector(DATA_WIDTH-1 downto 0);
        -- LVDS ports
        iovdp_lvds_p            : inout std_logic_vector(DATA_WIDTH-1 downto 0);
        iovdn_lvds_p            : inout std_logic_vector(DATA_WIDTH-1 downto 0)
    );

end entity lyt_lvds_gpio;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture arch of lyt_lvds_gpio is

    signal input_reset_s        : std_logic := '0';
    signal v8_SignalStretch_s   : std_logic_vector(7 downto 0) := (others => '0');
    signal CoreReset_s          : std_logic := '0';

    signal v_OEn_s              : std_logic_vector(DATA_WIDTH-1 downto 0) := (others => '1');
    signal v_Data_s             : std_logic_vector(DATA_WIDTH-1 downto 0) := (others => '0');

    signal v_iobufds_O_s        : std_logic_vector(DATA_WIDTH-1 downto 0) := (others => '0');
    signal v_iodelay_DATAOUT_s  : std_logic_vector(DATA_WIDTH-1 downto 0) := (others => '0');
    
    attribute S : string;
    attribute S of v_OEn_s      : signal is "TRUE";

begin

    ov32_InfoRegister_p <= X"1000" & X"0100"; -- Core ID 1000, Version 1.0

    --------------------------------------------
    -- SW reset pulse stretcher.
    --------------------------------------------
    input_reset_s <= iv32_CtrlRegister_p(0);

    process(i_SystemClk_p)
    begin
        if rising_edge(i_SystemClk_p) then
            v8_SignalStretch_s <= v8_SignalStretch_s(6 downto 0) & input_reset_s;
            CoreReset_s <= or_reduce(v8_SignalStretch_s);
        end if;
    end process;

    --------------------------------------------
    -- Instantiate a buffer for each LVDS pair
    --------------------------------------------
    lvds_buffer_generate : for i in 0 to DATA_WIDTH-1 generate

        -- Enable output if OE register or OE port is high
        v_OEn_s(i) <= not (iv32_OERegister_p(i) or iv_OutputEnable_p(i));
        v_Data_s(i) <= iv32_ValueRegister_p(i) or iv_Data_p(i);

        -- IO differential buffer
        iobufds_inst : IOBUFDS
        generic map (
            IOSTANDARD => "BLVDS_25"
        )
        port map (
            O     => v_iobufds_O_s(i),
            IO    => iovdp_lvds_p(i),
            IOB   => iovdn_lvds_p(i),
            I     => v_iodelay_DATAOUT_s(i),
            T     => v_OEn_s(i)
        );

        -- Delay input and output data
        iodelay_inst : IODELAYE1
        generic map (
            CINVCTRL_SEL => FALSE,
            DELAY_SRC => "IO",
            HIGH_PERFORMANCE_MODE => TRUE,
            IDELAY_TYPE => "FIXED",
            IDELAY_VALUE => DATA_INPUT_DELAY,
            ODELAY_TYPE => "FIXED",
            ODELAY_VALUE => DATA_OUTPUT_DELAY,
            REFCLK_FREQUENCY => 200.0,
            SIGNAL_PATTERN => "DATA"
        )
        port map (
            CNTVALUEOUT => open,
            DATAOUT => v_iodelay_DATAOUT_s(i),
            C => i_SystemClk_p,
            CE => '0',
            INC => '0',
            CINVCTRL => '0',
            CLKIN => 'Z',
            CNTVALUEIN => "00000",
            DATAIN => '0',
            IDATAIN => v_iobufds_O_s(i),
            ODATAIN => v_Data_s(i),
            RST => '0',
            T => v_OEn_s(i)
        );

        ov_Data_p(i) <= v_iodelay_DATAOUT_s(i);

    end generate;
    
    ov32_ValueRegister_p <= std_logic_vector(resize(unsigned(v_iodelay_DATAOUT_s),32));

end arch;
