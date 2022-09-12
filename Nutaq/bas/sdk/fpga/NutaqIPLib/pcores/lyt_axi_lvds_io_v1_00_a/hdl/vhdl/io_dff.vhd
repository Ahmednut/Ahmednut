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
-- File        : $Id: io_dff.vhd,v 1.1 2015/04/10 19:47:32 sqa Exp $
--------------------------------------------------------------------------------
-- Description : D Flip-Flop - flip-flop located in IOB
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: io_dff.vhd,v $
-- Revision 1.1  2015/04/10 19:47:32  sqa
-- bas first commit (CBOU)
--
-- Revision 1.6  2013/01/30 19:52:04  julien.roy
-- Modify LVDS example for Synchronous examples
--
-- Revision 1.5  2012/12/21 16:30:25  eric.dube
-- split ngc
--
-- Revision 1.1.2.1  2012/12/21 03:16:39  eric.dube
-- add netlist builder
--
-- Revision 1.4  2012/12/19 16:29:41  eric.dube
-- fix branch error
--
-- Revision 1.1.2.1  2012/12/07 15:36:14  eric.dube
-- initial commit
--
-- Revision 1.1  2011/06/15 21:12:51  jeffrey.johnson
-- Changed core name.
--
-- Revision 1.1  2011/05/27 13:37:57  patrick.gilbert
-- first commit: revA
--
--------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;

-- pragma translate_off
library unisim;
    use unisim.vcomponents.all;
-- pragma translate_on

entity io_dff is
  generic (
    io_dff_Falling_edge : boolean := FALSE
  );
  port (
    clk     : in std_logic;
    din     : in std_logic;
    dout    : out std_logic
  );
end entity io_dff;


architecture rtl of io_dff is

    --
    -- FD
    --

  component FD_1
  port (
    C : in std_logic;
    D : in std_logic;
    Q : out std_logic
  );
  end component;

  component FD
  port (
    C : in std_logic;
    D : in std_logic;
    Q : out std_logic
  );
  end component;

    attribute fpga_dont_touch : string;
    attribute iob : string;

    attribute fpga_dont_touch of u_fd : label is "TRUE";
    attribute iob of u_fd : label is "TRUE";

    attribute fpga_dont_touch of u_fd_1 : label is "TRUE";
    attribute iob of u_fd_1 : label is "TRUE";

begin

    --
    -- IFD Instantiation
    --

    u_fd : if ( io_dff_Falling_edge = FALSE ) generate
    begin
     u_fd : FD
        port map (
            C   => clk,
            D   => din,
            Q   => dout
        );
    end generate;
    u_fd_1  : if ( io_dff_Falling_edge = TRUE ) generate
     begin
    u_fd_1 : FD_1
        port map (
            C   => clk,
            D   => din,
            Q   => dout
        );
    end generate;

end architecture rtl;

