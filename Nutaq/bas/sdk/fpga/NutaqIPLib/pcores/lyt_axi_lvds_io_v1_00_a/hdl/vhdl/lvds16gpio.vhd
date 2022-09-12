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
-- File        : $Id: lvds16gpio.vhd,v 1.1 2015/04/10 19:47:33 sqa Exp $
--------------------------------------------------------------------------------
-- Description :
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lvds16gpio.vhd,v $
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

library UNISIM;
use UNISIM.VComponents.all;

entity lvds16Gpio is
  generic(
    DATA_IDELAY_VALUE : integer := 0
  );
  port (
    i_SystemClk_p         : in  std_logic;
    i_clock_p             : in  std_logic;

    i_direction_p         : in  std_logic; -- 3-state enable input, high=input, low=output

    iv16_input_p          : in  std_logic_vector( 15 downto 0 );  -- data input TX
    ov16_output_p         : out std_logic_vector( 15 downto 0 );  -- data output RX

    -- external
    lvdsIO_p              : inout  std_logic_vector( 15 downto 0 );
    lvdsIO_n              : inout  std_logic_vector( 15 downto 0 )

  );
end lvds16Gpio;

architecture Behavioral of lvds16Gpio is

  ----------------------------------------
  -- Component declaration
  ----------------------------------------

  component io_dff
    generic (
      io_dff_Falling_edge : boolean := FALSE
    );
    port (
      clk     : in std_logic;
      din     : in std_logic;
      dout    : out std_logic
    );
  end component;

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal v16_inputBuffer_s      : std_logic_vector(15 downto 0);
  signal v16_outputBuffer_s     : std_logic_vector(15 downto 0);

  signal v16_lvdsRxData_s       : std_logic_vector(15 downto 0);
  signal v16_lvdsRxDataIdelay_s : std_logic_vector(15 downto 0);

begin

  inputBufferProcess : process(i_clock_p)
  begin
    if rising_edge(i_clock_p) then
      v16_inputBuffer_s   <= iv16_input_p;
    end if;
  end process;

  outputBufferProcess : process(i_clock_p)
  begin
    if rising_edge(i_clock_p) then
      ov16_output_p       <= v16_outputBuffer_s;
    end if;
  end process;


  INPUT_RX_TX_BUFFER_GENERATE : for i in 0 to 15 generate

    IOBUFDS_RxTxData : IOBUFDS
      generic map (
        IOSTANDARD => "BLVDS_25")
      port map (
        O     => v16_lvdsRxData_s(i),     -- Buffer output
        IO    => lvdsIO_p(i),             -- Diff_p inout (connect directly to top-level port)
        IOB   => lvdsIO_n(i),             -- Diff_n inout (connect directly to top-level port)
        I     => v16_inputBuffer_s(i),    -- Buffer input
        T     => i_direction_p            -- 3-state enable input, high=input, low=output
      );
      
    data_idelay_inst : IDELAY
      generic map (
        IOBDELAY_TYPE => "FIXED",
        IOBDELAY_VALUE => DATA_IDELAY_VALUE)
      port map (
        O 		=> v16_lvdsRxDataIdelay_s(i),
        I 		=> v16_lvdsRxData_s(i),
        C 		=> '0',
        CE 	=> '0',
        INC 	=> '0',
        RST 	=> '0'
      );

    io_dff_U0 : io_dff
      generic map (
        io_dff_Falling_edge => false
      )
      port map(
        clk   => i_clock_p,
        din   => v16_lvdsRxDataIdelay_s(i),
        dout  => v16_outputBuffer_s(i)
      );

  end generate  INPUT_RX_TX_BUFFER_GENERATE;

end Behavioral;

