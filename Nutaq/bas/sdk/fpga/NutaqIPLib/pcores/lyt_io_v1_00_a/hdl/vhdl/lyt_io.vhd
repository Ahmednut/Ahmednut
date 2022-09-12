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
-- File : lyt_io.vhd
--------------------------------------------------------------------------------
-- Description : Core that instanciate a flipflop in the IOB if USE_FF is true
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_io.vhd,v $
-- Revision 1.1  2015/04/10 19:48:37  sqa
-- bas first commit
--
-- Revision 1.1  2013/06/04 13:24:09  julien.roy
-- Add IO core to instanciate flipflop in the IO pin
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

library lyt_io_v1_00_a;
use lyt_io_v1_00_a.all;

entity lyt_io is
  generic(
    USE_FF      : boolean := false;
    FALING_EDGE : boolean := false
  );
  port (
    i_Clk_p     : in std_logic;
    i_Input_p   : in std_logic;
    o_Output_p  : out std_logic
  );
end entity lyt_io;


architecture rtl of lyt_io is
begin

  generate_FF : if USE_FF generate
  begin
    io_dff_U0 : entity lyt_io_v1_00_a.io_dff
      generic map (
        io_dff_Falling_edge => FALING_EDGE
      )
      port map(
        clk   => i_Clk_p,
        din   => i_Input_p,
        dout  => o_Output_p
      );
  end generate;
  
  notgenerate_FF : if (not USE_FF) generate
  begin
    o_Output_p <= i_Input_p;
  end generate;
    

end rtl;
