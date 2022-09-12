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
-- File        : $Id: lime_dac_interface.vhd,v 1.1 2015/04/10 19:48:36 sqa Exp $
--------------------------------------------------------------------------------
-- Description : DAC Interface for LimeMicro LMS6002D
--------------------------------------------------------------------------------
-- Notes / Assumptions : This core use falling edge to add more setup to
-- the DAC interface of the LMS6002D
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lime_dac_interface.vhd,v $
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.3  2013/11/08 16:52:08  julien.roy
-- Change Radio420 clock architecture.
--
-- Revision 1.2  2013/01/18 19:03:45  julien.roy
-- Merge changes from ZedBoard reference design to Perseus
--
-- Revision 1.1  2012/09/28 19:31:26  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.3  2011/09/08 14:01:41  patrick.gilbert
-- add save attribute to be able to let dac data IO and IQ IO to ground if unused
--
-- Revision 1.2  2011/08/30 19:58:58  patrick.gilbert
-- remove IO standard to reduce warning... set anyway in UCF
--
-- Revision 1.1  2011/06/15 21:12:51  jeffrey.johnson
-- Changed core name.
--
-- Revision 1.2  2011/06/14 22:22:12  patrick.gilbert
-- add mimo modification...add generic to instantiate FMCclk0 BUFG. + add dac idelayctrl
--
-- Revision 1.1  2011/05/27 13:37:57  patrick.gilbert
-- first commit: revA
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library lyt_axi_radio420x_v1_00_a;
 use lyt_axi_radio420x_v1_00_a.all;

library unisim;
  use unisim.vcomponents.all;

entity lime_dac_interface is
  port (
    i_DesignClk_p          : in std_logic;
    i_AdcClk_p             : in std_logic;
    i_RefClk200MHz_p       : in std_logic;
    i_Rst_p                : in std_logic;
    i_RstFifo_p        : in std_logic;

    i_UserDacIQSel_p       : in std_logic;
    iv12_UserDacData_p     : in std_logic_vector(11 downto 0);

    ov12_DacData_p         : out std_logic_vector (11 downto 0);
    o_DacIQSel_p           : out std_logic
  );
end entity lime_dac_interface;

architecture arch of lime_dac_interface is

  ----------------------------------------
  -- Component declaration
  ----------------------------------------

  component radio420x_fifo1k_18b_async
    port (
      rst: IN std_logic;
      wr_clk: IN std_logic;
      rd_clk: IN std_logic;
      din: IN std_logic_VECTOR(17 downto 0);
      wr_en: IN std_logic;
      rd_en: IN std_logic;
      dout: OUT std_logic_VECTOR(17 downto 0);
      full: OUT std_logic;
      empty: OUT std_logic
    );
  end component;

  ----------------------------------------
  -- Signals declaration
  ----------------------------------------

  signal v12_DacData_s          : std_logic_vector(11 downto 0);
  signal v12_DacDataDff_s       : std_logic_vector(11 downto 0);

  signal DacIQSel_s             : std_logic;
  signal DacIQSelDff_s          : std_logic;

  signal v18_fifoDout_s    : std_logic_vector(17 downto 0);
  signal v18_fifoDin_s     : std_logic_vector(17 downto 0);
  signal fifoEmpty_s       : std_logic;
  signal nfifoEmpty_s      : std_logic;

  ----------------------------------------
  -- Attribute declaration
  ----------------------------------------

  attribute S : string;
  attribute S of DacIQSelDff_s : signal is "TRUE";
  attribute S of v12_DacDataDff_s : signal is "TRUE";

begin

  -----------------------------------------
  -- Clock domain crossing
  -----------------------------------------
  v18_fifoDin_s <= "00000" & iv12_UserDacData_p & i_UserDacIQSel_p;

  adcFifo_l : radio420x_fifo1k_18b_async
    port map (
      rst => i_RstFifo_p,
      wr_clk => i_DesignClk_p,
      rd_clk => i_AdcClk_p,
      din => v18_fifoDin_s,
      wr_en => '1',
      rd_en => nfifoEmpty_s,
      dout => v18_fifoDout_s,
      full => open,
      empty => fifoEmpty_s
  );

  nfifoEmpty_s  <= not fifoEmpty_s;

  v12_DacData_s    <= v18_fifoDout_s(12 downto 1);
  DacIQSel_s       <= v18_fifoDout_s(0);

  -----------------------------------------
  -- DAC data IO pins
  -----------------------------------------
  iob_DacData_gen_l : for i in 11 downto 0 generate
  begin
    DacDataInst_l : OBUF
      port map (
        O => ov12_DacData_p(i),
        I => v12_DacDataDff_s(i)
      );

    DacDataFF_l : entity lyt_axi_radio420x_v1_00_a.io_dff
      generic map (
        io_dff_Falling_edge => false
      )
      port map(
        clk     => i_AdcClk_p,
        din     => v12_DacData_s(i),
        dout    => v12_DacDataDff_s(i)
      );

  end generate iob_DacData_gen_l;


  -----------------------------------------
  -- DAC IQ select IO pin
  -----------------------------------------
  DacIQSelInst_l : OBUF
    port map (
      O => o_DacIQSel_p,
      I => DacIQSelDff_s
    );

  DacDataFF_l : entity lyt_axi_radio420x_v1_00_a.io_dff
    generic map (
      io_dff_Falling_edge => false
    )
    port map(
      clk     => i_AdcClk_p,
      din     => DacIQSel_s,
      dout    => DacIQSelDff_s
    );

end architecture;
