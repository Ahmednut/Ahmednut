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
-- File        : $Id: DataOutOfRange.vhd,v 1.1 2015/04/10 19:48:36 sqa Exp $
--------------------------------------------------------------------------------
-- Description : This module monitors the data and
--               asserts an out of range if the limit values are reached.
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/09/30
-- $Log: DataOutOfRange.vhd,v $
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.2  2013/01/18 19:03:45  julien.roy
-- Merge changes from ZedBoard reference design to Perseus
--
-- Revision 1.1  2013/01/10 14:26:46  julien.roy
-- Change adc_out_of_range to DataOutOfRange
--
-- Revision 1.1  2012/09/28 19:31:26  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.1  2011/08/30 20:00:17  patrick.gilbert
-- add calibration core inside FPGA : beta
--
-- Revision 1.2  2010/07/29 14:22:33  francois.blackburn
-- add another dds
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.1  2010/01/14 22:48:37  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/09/30 15:35:58  karim.ouadid
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity DataOutOfRange is
  generic (
    FilterLength : positive);
  port(
    i_clk_p                 : in std_logic;
    i_rst_p                 : in std_logic;
    iv12_DataCh1_p          : in std_logic_vector (11 downto 0);
    iv12_DataCh2_p          : in std_logic_vector (11 downto 0);
    i_DataType_p            : in std_logic; -- 0 = offset binary output format / 1= 2s complement output format
    o_Ch1_OvrNotFiltred_p   : out std_logic;
    o_Ch1_OvrFiltred_p      : out std_logic;
    o_Ch2_OvrNotFiltred_p   : out std_logic;
    o_Ch2_OvrFiltred_p      : out std_logic
  );
end DataOutOfRange;

architecture arch of DataOutOfRange is

  ----------------------------------------
  -- Component declaration
  ----------------------------------------

  component ChannelOutOfRange is
    generic (
      PatternOffsetBin   : std_logic_vector(11 downto 0);
      Pattern2s          : std_logic_vector(11 downto 0)
    );
    port(
      i_clk_p         : in std_logic;
      i_rst_p         : in std_logic;
      iv12_DataCh_p   : in std_logic_vector (11 downto 0);
      i_DataType_p    : in std_logic;
      o_Ch_oor_p      : out std_logic
    );
  end component;

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------
  
  signal ovr_match_foundCh1_s   : std_logic;
  signal udr_match_foundCh1_s   : std_logic;
  signal ovr_match_foundCh2_s   : std_logic;
  signal udr_match_foundCh2_s   : std_logic;
  signal SetResetLatchOutCh2_s  : std_logic;
  signal SetResetLatchOutCh1_s  : std_logic;
  signal ResetLatchOutCh1_s     : std_logic;
  signal ResetLatchOutCh2_s     : std_logic;
  signal FilterCountCh1_s       : std_logic_vector (FilterLength-1 downto 0);
  signal FilterCountCh2_s       : std_logic_vector (FilterLength-1 downto 0);
  signal ResetCountCh1_s        : std_logic;
  signal ResetCountCh2_s        : std_logic;

begin

  ----------------------------------------
  -- Out of range detection
  ----------------------------------------
  
  U_ChOutOfRange_Ch1Up_l : ChannelOutOfRange
    generic map(
      PatternOffsetBin   =>x"FFF",
      Pattern2s          =>x"7FF"
    )
    port map(
      i_clk_p         => i_clk_p,
      i_rst_p         => i_rst_p,
      iv12_DataCh_p   =>  iv12_DataCh1_p,
      i_DataType_p    => i_DataType_p,
      o_Ch_oor_p      => ovr_match_foundCh1_s
    );

  U_ChOutOfRange_Ch2Up_l : ChannelOutOfRange
    generic map(
      PatternOffsetBin   =>x"FFF",
      Pattern2s          =>x"7FF"
    )
    port map(
      i_clk_p  => i_clk_p,
      i_rst_p  => i_rst_p,
      iv12_DataCh_p  =>  iv12_DataCh2_p,
      i_DataType_p => i_DataType_p,
      o_Ch_oor_p => ovr_match_foundCh2_s
    );


  U_ChOutOfRange_Ch1Down_l : ChannelOutOfRange
    generic map(
      PatternOffsetBin   =>x"000",
      Pattern2s          =>x"800"
    )
    port map(
      i_clk_p  => i_clk_p,
      i_rst_p  => i_rst_p,
      iv12_DataCh_p  =>  iv12_DataCh1_p,
      i_DataType_p => i_DataType_p,
      o_Ch_oor_p => udr_match_foundCh1_s
    );


  U_ChOutOfRange_Ch2Down_l : ChannelOutOfRange
    generic map(
      PatternOffsetBin   =>x"000",
      Pattern2s          =>x"800"
    )
    port map(
      i_clk_p  => i_clk_p,
      i_rst_p  => i_rst_p,
      iv12_DataCh_p  =>  iv12_DataCh2_p,
      i_DataType_p => i_DataType_p,
      o_Ch_oor_p => udr_match_foundCh2_s
    );

  ----------------------------------------
  -- Filter over range status
  ----------------------------------------
  
  -- Reset filtered status when filter counter is half full
  ResetLatchOutCh1_s <= FilterCountCh1_s(FilterLength-1);
  ResetLatchOutCh2_s <= FilterCountCh2_s(FilterLength-1);
  
  -- Assert filtered status when over range or under range
  SetResetLatchCh1_l : process (i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if (i_rst_p = '1' or  ResetLatchOutCh1_s ='1') then
        SetResetLatchOutCh1_s <= '0';
      elsif (ovr_match_foundCh1_s ='1' or udr_match_foundCh1_s='1') then
        SetResetLatchOutCh1_s <= '1'  ;
      end if;
    end if;
  end process SetResetLatchCh1_l;
  
  SetResetLatchCh2_l : process (i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if (i_rst_p = '1' or  ResetLatchOutCh2_s ='1') then
        SetResetLatchOutCh2_s <= '0';
      elsif (ovr_match_foundCh2_s='1' or udr_match_foundCh2_s='1') then
        SetResetLatchOutCh2_s <= '1';
      end if;
    end if;
  end process SetResetLatchCh2_l;
  
  -- Reset filter counter when a new over range happens
  ResetCountCh1_s    <= ovr_match_foundCh1_s or udr_match_foundCh1_s;
  ResetCountCh2_s    <= ovr_match_foundCh2_s or udr_match_foundCh2_s;

  -- Increase filter counter when filtered status is high
  FilterCountCh1_l : process (i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if (i_rst_p = '1' or ResetCountCh1_s = '1')then
        FilterCountCh1_s <= (others =>'0');
      elsif SetResetLatchOutCh1_s ='1' then
        FilterCountCh1_s <= FilterCountCh1_s +"1";
      end if;
    end if;
  end process FilterCountCh1_l;
  
  FilterCountCh2_l : process (i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if (i_rst_p = '1' or  ResetCountCh2_s ='1')then
        FilterCountCh2_s <= (others =>'0');
      elsif SetResetLatchOutCh2_s ='1' then
        FilterCountCh2_s <= FilterCountCh2_s +"1";
      end if;
    end if;
  end process FilterCountCh2_l;
  
  ----------------------------------------
  -- Output assignments
  ----------------------------------------  

  o_Ch1_OvrFiltred_p <=  SetResetLatchOutCh1_s ;
  o_Ch2_OvrFiltred_p <=  SetResetLatchOutCh2_s ;

  o_Ch1_OvrNotFiltred_p <= ovr_match_foundCh1_s or udr_match_foundCh1_s;
  o_Ch2_OvrNotFiltred_p <= ovr_match_foundCh2_s or udr_match_foundCh2_s;


end arch;