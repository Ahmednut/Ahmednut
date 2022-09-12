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
-- File        : $Id: io_dff.vhd,v 1.1 2015/04/10 19:48:36 sqa Exp $
--------------------------------------------------------------------------------
-- Description : D Flip-Flop - flip-flop located in IOB
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: io_dff.vhd,v $
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.2  2013/01/18 19:03:45  julien.roy
-- Merge changes from ZedBoard reference design to Perseus
--
-- Revision 1.1  2012/09/28 19:31:26  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
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

  ----------------------------------------
  -- Component declaration
  ----------------------------------------

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

  ----------------------------------------
  -- Attribute declaration
  ----------------------------------------
  
  attribute fpga_dont_touch : string;
  attribute iob : string;

  attribute fpga_dont_touch of u_fd : label is "TRUE";
  attribute iob of u_fd : label is "TRUE";

  attribute fpga_dont_touch of u_fd_1 : label is "TRUE";
  attribute iob of u_fd_1 : label is "TRUE";

begin

  ----------------------------
  -- IFD Instantiation
  ----------------------------

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

