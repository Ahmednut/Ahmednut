--------------------------------------------------------------------------------
--
--
--          **  **     **  ******  ********  ********  ********  **    **
--         **    **   **  **   ** ********  ********  ********  **    **
--        **     *****   **   **    **     **        **        **    **
--       **       **    ******     **     ****      **        ********
--      **       **    **  **     **     **        **        **    **
--     *******  **    **   **    **     ********  ********  **    **
--    *******  **    **    **   **     ********  ********  **    **
--
--                       L Y R T E C H   R D   I N C
--
--------------------------------------------------------------------------------
-- File        : $Id: spi.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
--------------------------------------------------------------------------------
-- Description : spi ports for ADAC250
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2009 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/09/30
-- $Log: spi.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.1  2010/01/14 22:48:20  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/09/30 15:35:58  karim.ouadid
--------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;

-- pragma translate_off
library unisim;
    use unisim.vcomponents.all;
-- pragma translate_on

entity spi is
port (
    i_clk_p         : in  std_logic;                                          -- design clock
    i_reset_p       : in  std_logic;                                          -- global reset
    --SPI interface
    i_spi_in_p      : in  std_logic;
    o_spi_out_p     : out std_logic;
    o_spi_od_p      : out std_logic;
    o_spi_clk_p     : out std_logic;
    ov8_spi_sel_p   : out std_logic_vector(7 downto 0);
    ov8_spi_sel2_p  : out std_logic_vector(7 downto 0);
    -- Busy signal
    o_busy_p        : out std_logic;
    --External module 1 interface
    iv9_writeaddr1_p: in  std_logic_vector (8 downto 0);
    iv9_readaddr1_p : in  std_logic_vector (8 downto 0);
    i_req1_p        : in  std_logic;
    iv32_din1_p     : in  std_logic_vector (31 downto 0);
    o_gnt1_p        : out std_logic;
    ov32_dout1_p    : out std_logic_vector (31 downto 0);
    o_ack1_p        : out std_logic;
    --External module 2 interface
    iv9_writeaddr2_p: in  std_logic_vector (8 downto 0);
    iv9_readaddr2_p : in  std_logic_vector (8 downto 0);
    i_req2_p        : in  std_logic;
    iv32_din2_p     : in  std_logic_vector (31 downto 0);
    o_gnt2_p        : out std_logic;
    ov32_dout2_p    : out std_logic_vector (31 downto 0);
    o_ack2_p        : out std_logic;
    --External module 3 interface
    iv9_writeaddr3_p: in  std_logic_vector (8 downto 0);
    iv9_readaddr3_p : in  std_logic_vector (8 downto 0);
    i_req3_p        : in  std_logic;
    iv32_din3_p     : in  std_logic_vector (31 downto 0);
    o_gnt3_p        : out std_logic;
    ov32_dout3_p    : out std_logic_vector (31 downto 0);
    o_ack3_p        : out std_logic;
    --External module 4 interface
    iv9_writeaddr4_p: in  std_logic_vector (8 downto 0);
    iv9_readaddr4_p : in  std_logic_vector (8 downto 0);
    i_req4_p        : in  std_logic;
    iv32_din4_p     : in  std_logic_vector (31 downto 0);
    o_gnt4_p        : out std_logic;
    ov32_dout4_p    : out std_logic_vector (31 downto 0);
    o_ack4_p        : out std_logic
);
end entity spi;

architecture arch of spi is

component spi_mux is
port (
    i_clk_p         : in  std_logic;
    i_reset_p       : in  std_logic;
    --From/to SPI interface
    ov9_writeaddr_p : out std_logic_vector (8 downto 0);
    ov9_readaddr_p  : out std_logic_vector (8 downto 0);
    ov32_din_p      : out std_logic_vector (31 downto 0);
    iv32_dout_p     : in  std_logic_vector (31 downto 0);
    i_ack_p         : in  std_logic;
    --From/to modules accessing the SPI
    iv4_req_p       : in  std_logic_vector (3 downto 0);
    iv36_writeaddr_p: in  std_logic_vector (35 downto 0);
    iv36_readaddr_p : in  std_logic_vector (35 downto 0);
    iv128_din_p     : in  std_logic_vector (127 downto 0);
    ov4_gnt_p       : out std_logic_vector (3 downto 0);
    ov128_din_p     : out std_logic_vector (127 downto 0);
    ov4_ack_p       : out std_logic_vector (3 downto 0)
);
end component spi_mux;

component spi_interface is
port(
  --clk & rst
  i_rst_p : in std_logic;
  i_clk_p : in std_logic;
  --CPU mem map signals
  i_writeaddr_p : in std_logic_vector(8 downto 0);
  i_readaddr_p  : in std_logic_vector(8 downto 0);
  i_din_p : in std_logic_vector(31 downto 0);
  o_dout_p : out std_logic_vector(31 downto 0);
  o_ack_p : out std_logic;
  o_busy_p: out std_logic;
  --SPI external signals
  o_spi_out_p : out std_logic;--MOSI
  o_spi_od_p  : out std_logic;--Output disable
  o_spi_clk_p : out std_logic;
  i_spi_in_p : in std_logic ;--MISO
  o_spi_sel_p : out std_logic_vector(7 downto 0); --Up to 8 devices can be connected
  o_spi_sel2_p : out std_logic_vector(7 downto 0)--Up to 8 others devices can be connected
);
end component spi_interface;

signal v4_gnt_s         : std_logic_vector ( 3   downto 0);
signal v128_din_s       : std_logic_vector ( 127 downto 0);
signal v4_ack_s         : std_logic_vector ( 3   downto 0);
signal busy_s           : std_logic;
signal v9_writeaddr_s   : std_logic_vector ( 8 downto 0);
signal v9_readaddr_s    : std_logic_vector ( 8 downto 0);
signal v32_din_s        : std_logic_vector ( 31  downto 0);
signal v32_dout_s       : std_logic_vector ( 31  downto 0);
signal ack_s            : std_logic;
signal v4_req_s         : std_logic_vector ( 3   downto 0);
signal v36_writeaddr_s  : std_logic_vector ( 35   downto 0);
signal v36_readaddr_s   : std_logic_vector ( 35   downto 0);
signal v128_dinI_s      : std_logic_vector ( 127   downto 0);
signal spi_out_s        : std_logic;
signal spi_od_s         : std_logic;
signal spi_clk_s        : std_logic;
signal spi_sel_s        : std_logic_vector ( 7   downto 0);
signal spi_sel2_s       : std_logic_vector ( 7   downto 0);

begin

U_spi_mux_l : entity work.spi_mux
port map(
    i_clk_p          => i_clk_p,
    i_reset_p        => i_reset_p,
    --From/to SPI interface
    ov9_writeaddr_p  => v9_writeaddr_s,
    ov9_readaddr_p   => v9_readaddr_s,
    ov32_din_p       => v32_din_s,
    iv32_dout_p      => v32_dout_s,
    i_ack_p          => ack_s,
    --From/to modules accessing the SPI
    iv4_req_p        => v4_req_s,
    iv36_writeaddr_p => v36_writeaddr_s,
    iv36_readaddr_p  => v36_readaddr_s,
    iv128_din_p      => v128_dinI_s,
    ov4_gnt_p        => v4_gnt_s,
    ov128_din_p      => v128_din_s,
    ov4_ack_p        => v4_ack_s
);

v4_req_s    <= i_req4_p & i_req3_p & i_req2_p & i_req1_p;
v36_writeaddr_s  <= iv9_writeaddr4_p & iv9_writeaddr3_p & iv9_writeaddr2_p & iv9_writeaddr1_p;
v36_readaddr_s  <= iv9_readaddr4_p & iv9_readaddr3_p & iv9_readaddr2_p & iv9_readaddr1_p;
v128_dinI_s   <= iv32_din4_p & iv32_din3_p & iv32_din2_p & iv32_din1_p;

U_spi_interface_l : entity work.spi_interface
port map(
  --clk & rst
  i_rst_p       => i_reset_p,
  i_clk_p       => i_clk_p,
  --CPU mem map signals
  i_writeaddr_p => v9_writeaddr_s,
  i_readaddr_p  => v9_readaddr_s,
  i_din_p       => v32_din_s,
  o_dout_p      => v32_dout_s,
  o_ack_p       => ack_s,
  o_busy_p      => busy_s,
  --SPI external signals
  o_spi_out_p   => spi_out_s,
  o_spi_od_p    => spi_od_s,
  o_spi_clk_p   => spi_clk_s,
  i_spi_in_p    => i_spi_in_p,
  o_spi_sel_p   => spi_sel_s,
  o_spi_sel2_p  => spi_sel2_s
);

-- output port assignement
o_busy_p       <= busy_s;
o_spi_out_p    <= spi_out_s;
o_spi_od_p     <= spi_od_s;
o_spi_clk_p    <= spi_clk_s;
ov8_spi_sel_p  <= spi_sel_s;
ov8_spi_sel2_p <= spi_sel2_s;
o_gnt4_p       <= v4_gnt_s(3);
o_gnt3_p       <= v4_gnt_s(2);
o_gnt2_p       <= v4_gnt_s(1);
o_gnt1_p       <= v4_gnt_s(0);
o_ack4_p       <= v4_ack_s(3);
o_ack3_p       <= v4_ack_s(2);
o_ack2_p       <= v4_ack_s(1);
o_ack1_p       <= v4_ack_s(0);
ov32_dout4_p   <= v128_din_s(127 downto 96);
ov32_dout3_p   <= v128_din_s(95  downto 64);
ov32_dout2_p   <= v128_din_s(63  downto 32);
ov32_dout1_p   <= v128_din_s(31  downto  0);

end architecture arch;