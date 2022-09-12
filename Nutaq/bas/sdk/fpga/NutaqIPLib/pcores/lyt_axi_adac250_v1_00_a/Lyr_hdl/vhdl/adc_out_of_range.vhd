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
-- File        : $Id: adc_out_of_range.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
--------------------------------------------------------------------------------
-- Description : ADCOutOfRange module for ADAC250
--               This module monitors the Data captured from ADC channels and
--               assertes an out of range when it gets N Overflow samples as
--               described on the ADS62P49 datasheet page 66.
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2009 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/09/30
-- $Log: adc_out_of_range.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
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

entity ADCOutOfRange is
  generic (
    FilterLength : positive);

  port(
    i_clk_p                 : in std_logic;
    i_rst_p                 : in std_logic;
    iv14_DataCh1_p          : in std_logic_vector (13 downto 0);
    iv14_DataCh2_p          : in std_logic_vector (13 downto 0);
    ov14_DataCh1_p          : out std_logic_vector (13 downto 0);
    ov14_DataCh2_p          : out std_logic_vector (13 downto 0);
    i_DataType_p            : in std_logic;                                   -- 0 = offset binary output format / 1= 2s complement output format
    o_Ch1_OvrNotFiltred_p   : out std_logic;
    o_Ch1_OvrFiltred_p      : out std_logic;
    o_Ch2_OvrNotFiltred_p   : out std_logic;
    o_Ch2_OvrFiltred_p      : out std_logic

  );
end ADCOutOfRange;

architecture arch of ADCOutOfRange is

signal ovr_match_foundCh1_s   : std_logic;
signal udr_match_foundCh1_s   : std_logic;
signal ovr_match_foundCh2_s   : std_logic;
signal udr_match_foundCh2_s   : std_logic;
signal v14_DataCh1_s          : std_logic_vector (13 downto 0);
signal v14_DataCh2_s          : std_logic_vector (13 downto 0);
signal SetResetLatchOutCh2_s  : std_logic;
signal SetResetLatchOutCh1_s  : std_logic;
signal ResetLatchOutCh1_s     : std_logic;
signal ResetLatchOutCh2_s     : std_logic;
signal FilterCountCh1_s       : std_logic_vector (FilterLength-1 downto 0);
signal FilterCountCh2_s       : std_logic_vector (FilterLength-1 downto 0);
signal ResetCountCh1_s        : std_logic;
signal ResetCountCh2_s        : std_logic;


component ChOutOfRange is
  generic (
             PatternOffsetBin   : std_logic_vector(13 downto 0);
             Pattern2s          : std_logic_vector(13 downto 0)
            );
  port(
    i_clk_p : in std_logic;
    i_rst_p : in std_logic;
    iv14_DataCh_p   : in std_logic_vector (13 downto 0);
    ov14_DataCh_p   : out std_logic_vector (13 downto 0);
    i_DataType_p : in std_logic;
    o_Ch_oor_p   : out std_logic
      );
end component;


begin

    U_ChOutOfRange_Ch1Up_l : entity work.ChOutOfRange
      generic map(
                 PatternOffsetBin   =>"11"&x"FFF",
                 Pattern2s          =>"01"&x"FFF"
                )
      port map(
        i_clk_p         => i_clk_p,
        i_rst_p         => i_rst_p,
        iv14_DataCh_p   =>  iv14_DataCh1_p,
        i_DataType_p    => i_DataType_p,
        ov14_DataCh_p   => v14_DataCh1_s ,
        o_Ch_oor_p      => ovr_match_foundCh1_s
          );

    U_ChOutOfRange_Ch2Up_l : entity work.ChOutOfRange
      generic map(
                 PatternOffsetBin   =>"11"&x"FFF",
                 Pattern2s          =>"01"&x"FFF"
                )
      port map(
        i_clk_p  => i_clk_p,
        i_rst_p  => i_rst_p,
        iv14_DataCh_p  =>  iv14_DataCh2_p,
        i_DataType_p => i_DataType_p,
        ov14_DataCh_p  => v14_DataCh2_s ,
        o_Ch_oor_p => ovr_match_foundCh2_s
          );


    U_ChOutOfRange_Ch1Down_l : entity work.ChOutOfRange
      generic map(
                 PatternOffsetBin   =>"00"&x"000",
                 Pattern2s          =>"10"&x"000"
                )
      port map(
        i_clk_p  => i_clk_p,
        i_rst_p  => i_rst_p,
        iv14_DataCh_p  =>  iv14_DataCh1_p,
        i_DataType_p => i_DataType_p,
        ov14_DataCh_p  => open,
        o_Ch_oor_p => udr_match_foundCh1_s
          );


    U_ChOutOfRange_Ch2Down_l : entity work.ChOutOfRange
      generic map(
                 PatternOffsetBin   =>"00"&x"000",
                 Pattern2s          =>"10"&x"000"
                )
      port map(
        i_clk_p  => i_clk_p,
        i_rst_p  => i_rst_p,
        iv14_DataCh_p  =>  iv14_DataCh2_p,
        i_DataType_p => i_DataType_p,
        ov14_DataCh_p  => open,
        o_Ch_oor_p => udr_match_foundCh2_s
          );

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

ResetLatchOutCh1_s <= FilterCountCh1_s(FilterLength-1);--or ovr_match_foundCh1_s or udr_match_foundCh1_s;
ResetLatchOutCh2_s <= FilterCountCh2_s(FilterLength-1);--or ovr_match_foundCh2_s or udr_match_foundCh2_s;
ResetCountCh1_s    <= ovr_match_foundCh1_s or udr_match_foundCh1_s;
ResetCountCh2_s    <= ovr_match_foundCh2_s or udr_match_foundCh2_s;

o_Ch1_OvrNotFiltred_p <= ovr_match_foundCh1_s or udr_match_foundCh1_s;
o_Ch2_OvrNotFiltred_p <= ovr_match_foundCh2_s or udr_match_foundCh2_s;

o_Ch2_OvrFiltred_p <=  SetResetLatchOutCh2_s ;
o_Ch1_OvrFiltred_p <=  SetResetLatchOutCh1_s ;

ov14_DataCh1_p<= v14_DataCh1_s;
ov14_DataCh2_p<= v14_DataCh2_s;

end arch;