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
-- File        : $Id: spi_mux.vhd,v 1.1 2015/04/10 19:47:33 sqa Exp $
--------------------------------------------------------------------------------
-- Description : MUX to SPI port for MI250
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: spi_mux.vhd,v $
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2012/11/08 15:42:11  khalid.bensadek
-- Updated the ipcore to AXI bus version. Working version as is with 4 MMCMs that will conflect if adding Record-Playback ipcore.
--
-- Revision 1.3  2011/05/19 18:27:52  jeffrey.johnson
-- Added ADC interface.
--
-- Revision 1.2  2011/05/11 13:34:45  jeffrey.johnson
-- SPI fixed.
--
-- Revision 1.1  2011/05/09 16:21:44  jeffrey.johnson
-- First commit.
--------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;

-- pragma translate_off
library unisim;
    use unisim.vcomponents.all;
-- pragma translate_on


entity spi_mux is
port (
    i_clk_p         : in  std_logic;                       -- design clock
    i_reset_p       : in  std_logic;                       -- global reset
    --From/to SPI interface
    ov9_writeaddr_p : out std_logic_vector (8 downto 0);   -- address mapped on the SPI interface
    ov9_readaddr_p  : out std_logic_vector (8 downto 0);
    ov32_din_p      : out std_logic_vector (31 downto 0);  -- Data write to the SPI interface
    iv32_dout_p     : in  std_logic_vector (31 downto 0);  -- Data read from the SPI interface
    i_ack_p         : in  std_logic;                       -- read or write command acknowledge from the SPI interface
    --From/to modules accessing the SPI
    iv4_req_p       : in  std_logic_vector (3 downto 0);   -- SPI interface access request
    iv36_writeaddr_p: in  std_logic_vector (35 downto 0);
    iv36_readaddr_p : in  std_logic_vector (35 downto 0);   -- address for the mapping on the SPI interface
    iv128_din_p     : in  std_logic_vector (127 downto 0); -- Data to the SPI interface datain from 4 external modules
    ov4_gnt_p       : out std_logic_vector (3 downto 0);   -- Arbiter give grant SPI interface access to one of the 4 external modules
    ov128_din_p     : out std_logic_vector (127 downto 0); -- Data from the SPI interface dataout to one of the 4 external modules
    ov4_ack_p       : out std_logic_vector (3 downto 0)    -- command acknowledg from the SPI interface to one of the 4 external modules
);
end entity spi_mux;

architecture arch of spi_mux is

component arbiter is
port (
    clk,  rst       : in  std_logic;
    req0, req1      : in  std_logic;
    req2, req3      : in  std_logic;
    gnt0, gnt1      : out std_logic;
    gnt2, gnt3      : out std_logic
);
end component;

signal v4_ack_s          : std_logic_vector (3 downto 0);
signal v128_din_s        : std_logic_vector (127 downto 0);
signal v4_gnt_s          : std_logic_vector (3 downto 0);
signal v32_din_s         : std_logic_vector (31 downto 0);
signal v9_writeaddr_s    : std_logic_vector (8 downto 0);
signal v9_readaddr_s     : std_logic_vector (8 downto 0);
signal v9_writeaddrPLB_s : std_logic_vector (0 to 8);
signal v9_readaddrPLB_s  : std_logic_vector (0 to 8);
signal v32_din_PLB_s     : std_logic_vector (0 to 31);
signal v32_doutPLB_s     : std_logic_vector (0 to 31);


begin
-- arbiter instanciation
u_arbiter_l : entity work.arbiter
    port map(
        clk  => i_clk_p,
        rst  => i_reset_p,
        req0 => iv4_req_p(0),
        req1 => iv4_req_p(1),
        req2 => iv4_req_p(2),
        req3 => iv4_req_p(3),
        gnt0 => v4_gnt_s(0),
        gnt1 => v4_gnt_s(1),
        gnt2 => v4_gnt_s(2),
        gnt3 => v4_gnt_s(3)
    );

-- Endianess swapping for the plb
v9_writeaddrPLB_s (0 to 8) <= iv36_writeaddr_p (8 downto 0);
v9_readaddrPLB_s  (0 to 8) <= iv36_readaddr_p (8 downto 0);
v32_din_PLB_s     (0 to 31)<= iv128_din_p(31 downto 0);
v32_doutPLB_s     (0 to 31)<= iv32_dout_p(31 downto 0);


-- Output port mapping
ov4_gnt_p       <= v4_gnt_s;
ov128_din_p     <= v128_din_s;
ov4_ack_p       <= v4_ack_s;
ov9_writeaddr_p <= v9_writeaddr_s;
ov9_readaddr_p  <= v9_readaddr_s ;
ov32_din_p      <= v32_din_s;

-- Muxing ports
mux_process : process (v4_gnt_s, v9_writeaddrPLB_s, v9_readaddrPLB_s, v32_din_PLB_s, i_ack_p, iv36_writeaddr_p, iv36_readaddr_p, iv128_din_p, iv32_dout_p, v32_doutPLB_s)
begin
 v9_writeaddr_s  <= (others => '0');
 v9_readaddr_s   <= (others => '0');
 v4_ack_s   <= (others => '0');
 v128_din_s <= (others => '0');
 case v4_gnt_s is
    when "0001" =>
          v9_writeaddr_s   <= v9_writeaddrPLB_s;
          v9_readaddr_s    <= v9_readaddrPLB_s;
          v32_din_s        <= v32_din_PLB_s;
          v4_ack_s         <= "000"&i_ack_p;
          v128_din_s       <= x"0000_0000_0000_0000_0000_0000" & v32_doutPLB_s;

    when "0010" =>
          v9_writeaddr_s   <= iv36_writeaddr_p (17 downto 9);
          v9_readaddr_s    <= iv36_readaddr_p (17 downto 9);
          v32_din_s        <= iv128_din_p(63 downto 32);
          v4_ack_s         <= "00"&i_ack_p&'0';
          v128_din_s       <= x"0000_0000_0000_0000" & iv32_dout_p & x"0000_0000";

    when "0100" =>
          v9_writeaddr_s   <= iv36_writeaddr_p (26 downto 18);
          v9_readaddr_s    <= iv36_readaddr_p (26 downto 18);
          v32_din_s        <= iv128_din_p(95 downto 64);
          v4_ack_s         <= '0'&i_ack_p&"00";
          v128_din_s       <= x"0000_0000" & iv32_dout_p & x"0000_0000_0000_0000";

    when "1000" =>
          v9_writeaddr_s   <= iv36_writeaddr_p (35 downto 27);
          v9_readaddr_s    <= iv36_readaddr_p (26 downto 18);
          v32_din_s        <= iv128_din_p(127 downto 96);
          v4_ack_s         <= i_ack_p&"000";
          v128_din_s       <= iv32_dout_p & x"0000_0000_0000_0000_0000_0000";

    when others =>
          v9_writeaddr_s   <= (others => '0');
          v9_readaddr_s    <= (others => '0');
          v32_din_s        <= (others => '0');
          v4_ack_s         <= (others => '0');
          v128_din_s       <= (others => '0');
   end case;
end process;

end architecture arch;
