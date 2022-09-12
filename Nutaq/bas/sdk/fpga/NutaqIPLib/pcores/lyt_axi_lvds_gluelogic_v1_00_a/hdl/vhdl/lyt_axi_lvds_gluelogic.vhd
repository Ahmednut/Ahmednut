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
-- File        : $Id: lyt_axi_lvds_gluelogic.vhd,v 1.1 2015/04/10 19:47:31 sqa Exp $
--------------------------------------------------------------------------------
-- Description :
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_axi_lvds_gluelogic.vhd,v $
-- Revision 1.1  2015/04/10 19:47:31  sqa
-- bas first commit (CBOU)
--
-- Revision 1.3  2013/02/04 18:59:44  julien.roy
-- Modify file header
--
-- Revision 1.6  2013/01/30 19:52:04  julien.roy
-- Modify LVDS example for Synchronous examples
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lyt_axi_lvds_gluelogic is
  port
  (
    -- rtdex channel 0
    -- from host
    iv32_rtdex_RxDataCh0      : in     std_logic_vector( 31 downto 0 );
    o_rtdex_RxReCh0        : out     std_logic;
    i_rtdex_RxDataValidCh0    : in     std_logic;
    -- to host
    ov32_rtdex_TxDataCh0      : out    std_logic_vector( 31 downto 0 );
    i_rtdex_TxReadyCh0      : in     std_logic;
    o_rtdex_TxWeCh0        : out   std_logic;
    -- 
    ov14_UserLvdsGroup0_p    : out   std_logic_vector( 13 downto 0 );
    ov14_UserLvdsGroup1_p    : out   std_logic_vector( 13 downto 0 );  
    iv14_UserLvdsGroup0_p    : in     std_logic_vector( 13 downto 0 );
    iv14_UserLvdsGroup1_p    : in     std_logic_vector( 13 downto 0 );
    --
    ov2_inWrEn_p           : out    std_logic_vector( 1  downto 0 ); 
    iv2_inWrAck_p           : in     std_logic_vector( 1  downto 0 );
    ov2_outRdEn_p           : out   std_logic_vector( 1  downto 0 );
    iv2_outValid_p         : in     std_logic_vector( 1  downto 0 );
    iv2_empty_p           : in     std_logic_vector( 1  downto 0 );
    iv2_full_p             : in    std_logic_vector( 1  downto 0 );
    
    clk                : in     std_logic
  );

end entity lyt_axi_lvds_gluelogic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of lyt_axi_lvds_gluelogic is

  signal sv16_rx1DataOut    : std_logic_vector(15 downto 0);
  signal sv16_rx2DataOut    : std_logic_vector(15 downto 0);

  signal sv16_tx1DataIn    : std_logic_vector(15 downto 0);
  signal sv16_tx2DataIn    : std_logic_vector(15 downto 0);

  signal rxFifoFull_1, rxFifoFull_2       : std_logic;

  signal rx1RdEn, rx2RdEn    : std_logic;

  signal tx1Full, tx2Full    : std_logic;

  signal debugCounter      : unsigned(31 downto 0) := (others => '0');


begin

  -- rtdex -> LVDS on Group 0
  ov14_UserLvdsGroup0_p    <= iv32_rtdex_RxDataCh0( 13 downto 0 );
  ov14_UserLvdsGroup1_p    <= ( others => '0' );
  
  o_rtdex_RxReCh0        <= not iv2_full_p(0);
  ov2_inWrEn_p(0)        <= i_rtdex_RxDataValidCh0;
  ov2_inWrEn_p(1)        <= '0';
  
  -- LVDS -> rtdex on group 1
  ov32_rtdex_TxDataCh0      <= "00" & x"0000" & iv14_UserLvdsGroup1_p;    
  ov2_outRdEn_p(0)        <= '0';
  ov2_outRdEn_p(1)        <= i_rtdex_TxReadyCh0;
  o_rtdex_TxWeCh0        <= iv2_outValid_p(1);

end IMP;
