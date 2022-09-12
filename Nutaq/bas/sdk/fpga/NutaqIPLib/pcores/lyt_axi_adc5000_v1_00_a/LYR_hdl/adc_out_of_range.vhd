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
-- Revision 1.1  2012/09/28 19:08:53  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.5  2012/02/01 14:36:32  khalid.bensadek
-- Update du projet
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
     FilterLength : positive range 2 to 64);
  port(    
    i_rst_p                 : in std_logic;
    i_clkCh1_p              : in std_logic;
    i_clkCh2_p              : in std_logic;
    i_clkCh3_p              : in std_logic;
    i_clkCh4_p              : in std_logic;
    iv80_dataCh1_p          : in std_logic_vector(79 downto 0);
    iv80_dataCh2_p          : in std_logic_vector(79 downto 0);
    iv80_dataCh3_p          : in std_logic_vector(79 downto 0);
    iv80_dataCh4_p          : in std_logic_vector(79 downto 0);    
    ov80_dataCh1_p          : out std_logic_vector(79 downto 0);
    ov80_dataCh2_p          : out std_logic_vector(79 downto 0);
    ov80_dataCh3_p          : out std_logic_vector(79 downto 0);
    ov80_dataCh4_p          : out std_logic_vector(79 downto 0);
    o_Ch1_OvrNotFiltred_p : out std_logic;
    o_Ch2_OvrNotFiltred_p : out std_logic;
    o_Ch3_OvrNotFiltred_p : out std_logic;
    o_Ch4_OvrNotFiltred_p : out std_logic;
    o_Ch1_OvrFiltred_p    : out std_logic;
    o_Ch2_OvrFiltred_p    : out std_logic;
    o_Ch3_OvrFiltred_p    : out std_logic;
    o_Ch4_OvrFiltred_p    : out std_logic    
  );
end ADCOutOfRange;

architecture arch of ADCOutOfRange is
 
signal OvrMatchFoundCh1_s,
       OvrMatchFoundCh2_s,
       OvrMatchFoundCh3_s,
       OvrMatchFoundCh4_s   : std_logic;
       
signal UdrMatchFoundCh1_s,
       UdrMatchFoundCh2_s,
       UdrMatchFoundCh3_s,
       UdrMatchFoundCh4_s   : std_logic;

signal SetResetLatchOutCh1_s,
       SetResetLatchOutCh2_s,
       SetResetLatchOutCh3_s,
       SetResetLatchOutCh4_s  : std_logic;

signal ResetLatchOutCh1_s,
       ResetLatchOutCh2_s,
       ResetLatchOutCh3_s,
       ResetLatchOutCh4_s     : std_logic;

signal FilterCountCh1_s,
       FilterCountCh2_s,
       FilterCountCh3_s,
       FilterCountCh4_s       : std_logic_vector(FilterLength-1 downto 0);

signal ResetCountCh1_s,
       ResetCountCh2_s,
       ResetCountCh3_s,
       ResetCountCh4_s        : std_logic;

component ChOutOfRange is
  generic(
           PATTERN          : std_logic_vector(9 downto 0)             
         );
  port(
    i_clk_p          : in std_logic;
    i_rst_p          : in std_logic;    
    iv80_DataCh_p    : in std_logic_vector(79 downto 0);    
    ov80_DataCh_p    : out std_logic_vector(79 downto 0);
    o_Ch_oor_p     : out std_logic
      );
end component;

--------------------------------------------------------------------------
-- Contants definition
--------------------------------------------------------------------------
constant PATTERN_2COMPLEMENT_MAX : STD_LOGIC_VECTOR(9 DOWNTO 0) :=b"01_1111_1111";
constant PATTERN_2COMPLEMENT_MIN : STD_LOGIC_VECTOR(9 DOWNTO 0) :=b"10_0000_0000";

constant PATTERN_OFFSETBINARY_MAX : STD_LOGIC_VECTOR(9 DOWNTO 0) :=b"11_1111_1111";
constant PATTERN_OFFSETBINARY_MIN : STD_LOGIC_VECTOR(9 DOWNTO 0) :=b"00_0000_0000";
                                  

begin

--------------------------------------------------------------------------
-- Out of range of up dircetion
--------------------------------------------------------------------------
   ChOutOfRange_Ch1UpInst : ChOutOfRange
      generic map(                 
                  PATTERN => PATTERN_OFFSETBINARY_MAX
                 )
      port map(
        i_clk_p         => i_clkCh1_p,
        i_rst_p         => i_rst_p,
        iv80_DataCh_p   =>  iv80_dataCh1_p,        
        ov80_DataCh_p   =>  ov80_dataCh1_p,
        o_Ch_oor_p      => OvrMatchFoundCh1_s
            );


ChOutOfRange_Ch2UpInst : ChOutOfRange
   generic map(                 
              PATTERN  => PATTERN_OFFSETBINARY_MAX
             )
   port map(
     i_clk_p         => i_clkCh2_p,
     i_rst_p         => i_rst_p,
     iv80_DataCh_p   =>  iv80_dataCh2_p,        
     ov80_DataCh_p   =>  ov80_dataCh2_p,
     o_Ch_oor_p      => OvrMatchFoundCh2_s
         );

ChOutOfRange_Ch3UpInst : ChOutOfRange
   generic map(                 
              PATTERN => PATTERN_OFFSETBINARY_MAX
             )
   port map(
     i_clk_p         => i_clkCh3_p,
     i_rst_p         => i_rst_p,
     iv80_DataCh_p   =>  iv80_dataCh3_p,        
     ov80_DataCh_p   =>  ov80_dataCh3_p,
     o_Ch_oor_p      => OvrMatchFoundCh3_s
         );
         
ChOutOfRange_Ch4UpInst : ChOutOfRange
   generic map(                 
              PATTERN => PATTERN_OFFSETBINARY_MAX
             )
   port map(
     i_clk_p         => i_clkCh4_p,
     i_rst_p         => i_rst_p,
     iv80_DataCh_p   =>  iv80_dataCh4_p,        
     ov80_DataCh_p   =>  ov80_dataCh4_p,
     o_Ch_oor_p      => OvrMatchFoundCh4_s
         );                           
         
--------------------------------------------------------------------------
-- Out of range of down dircetion
--------------------------------------------------------------------------
ChOutOfRange_Ch1DownInst : ChOutOfRange
   generic map(                 
              PATTERN => PATTERN_OFFSETBINARY_MIN
             )
   port map(
     i_clk_p         => i_clkCh1_p,
     i_rst_p         => i_rst_p,
     iv80_DataCh_p   => iv80_dataCh1_p,        
     ov80_DataCh_p   => open,
     o_Ch_oor_p      => UdrMatchFoundCh1_s
         );

ChOutOfRange_Ch2DownInst : ChOutOfRange
   generic map(                 
              PATTERN => PATTERN_OFFSETBINARY_MIN
             )
   port map(
     i_clk_p         => i_clkCh2_p,
     i_rst_p         => i_rst_p,
     iv80_DataCh_p   => iv80_dataCh2_p,        
     ov80_DataCh_p   => open,
     o_Ch_oor_p      => UdrMatchFoundCh2_s
         );

ChOutOfRange_Ch3DownInst : ChOutOfRange
   generic map(                 
              PATTERN => PATTERN_OFFSETBINARY_MIN
             )
   port map(
     i_clk_p         => i_clkCh3_p,
     i_rst_p         => i_rst_p,
     iv80_DataCh_p   => iv80_dataCh3_p,        
     ov80_DataCh_p   => open,
     o_Ch_oor_p      => UdrMatchFoundCh3_s
         );
         
ChOutOfRange_Ch4DownInst : ChOutOfRange
   generic map(                 
              PATTERN => PATTERN_OFFSETBINARY_MIN
             )
   port map(
     i_clk_p         => i_clkCh4_p,
     i_rst_p         => i_rst_p,
     iv80_DataCh_p   => iv80_dataCh4_p,        
     ov80_DataCh_p   => open,
     o_Ch_oor_p      => UdrMatchFoundCh4_s
         );
--------------------------------------------------------------------------
-- OTR filtering
--------------------------------------------------------------------------
process (i_clkCh1_p)
begin
   if rising_edge(i_clkCh1_p) then
      if (i_rst_p = '1' or  ResetLatchOutCh1_s ='1') then
         SetResetLatchOutCh1_s <= '0';
      elsif (OvrMatchFoundCh1_s ='1' or UdrMatchFoundCh1_s='1') then
         SetResetLatchOutCh1_s <= '1'  ;
      end if;
   end if;
end process;

process (i_clkCh1_p)
begin
   if rising_edge(i_clkCh1_p) then
      if (i_rst_p = '1' or ResetCountCh1_s = '1')then
         FilterCountCh1_s <= (others =>'0');
      elsif SetResetLatchOutCh1_s ='1' then
         FilterCountCh1_s <= FilterCountCh1_s +"1";
      end if;
   end if;
end process;

process (i_clkCh2_p)
begin
   if rising_edge(i_clkCh2_p) then
      if (i_rst_p = '1' or  ResetLatchOutCh2_s ='1') then
         SetResetLatchOutCh2_s <= '0';
      elsif (OvrMatchFoundCh2_s ='1' or UdrMatchFoundCh2_s ='1') then
         SetResetLatchOutCh2_s <= '1'  ;
      end if;
   end if;
end process;

process (i_clkCh2_p)
begin
   if rising_edge(i_clkCh2_p) then
      if (i_rst_p = '1' or ResetCountCh2_s = '1')then
         FilterCountCh2_s <= (others =>'0');
      elsif SetResetLatchOutCh2_s ='1' then
         FilterCountCh2_s <= FilterCountCh2_s +"1";
      end if;
   end if;
end process;

process (i_clkCh3_p)
begin
   if rising_edge(i_clkCh3_p) then
      if (i_rst_p = '1' or  ResetLatchOutCh3_s ='1') then
         SetResetLatchOutCh3_s <= '0';
      elsif (OvrMatchFoundCh3_s ='1' or UdrMatchFoundCh3_s ='1') then
         SetResetLatchOutCh3_s <= '1'  ;
      end if;
   end if;
end process;

process (i_clkCh3_p)
begin
   if rising_edge(i_clkCh3_p) then
      if (i_rst_p = '1' or ResetCountCh3_s = '1')then
         FilterCountCh3_s <= (others =>'0');
      elsif SetResetLatchOutCh3_s ='1' then
         FilterCountCh3_s <= FilterCountCh3_s +"1";
      end if;
   end if;
end process;

process (i_clkCh4_p)
begin
   if rising_edge(i_clkCh4_p) then
      if (i_rst_p = '1' or  ResetLatchOutCh4_s ='1') then
         SetResetLatchOutCh4_s <= '0';
      elsif (OvrMatchFoundCh4_s ='1' or UdrMatchFoundCh4_s ='1') then
         SetResetLatchOutCh4_s <= '1'  ;
      end if;
   end if;
end process;

process (i_clkCh4_p)
begin
   if rising_edge(i_clkCh4_p) then
      if (i_rst_p = '1' or ResetCountCh4_s = '1')then
         FilterCountCh4_s <= (others =>'0');
      elsif SetResetLatchOutCh4_s ='1' then
         FilterCountCh4_s <= FilterCountCh4_s +"1";
      end if;
   end if;
end process;

ResetLatchOutCh1_s <= FilterCountCh1_s(FilterLength-1);
ResetLatchOutCh2_s <= FilterCountCh2_s(FilterLength-1);
ResetLatchOutCh3_s <= FilterCountCh3_s(FilterLength-1);
ResetLatchOutCh4_s <= FilterCountCh4_s(FilterLength-1);

ResetCountCh1_s    <= OvrMatchFoundCh1_s or UdrMatchFoundCh1_s;
ResetCountCh2_s    <= OvrMatchFoundCh2_s or UdrMatchFoundCh2_s;
ResetCountCh3_s    <= OvrMatchFoundCh3_s or UdrMatchFoundCh3_s;
ResetCountCh4_s    <= OvrMatchFoundCh4_s or UdrMatchFoundCh4_s;

o_Ch1_OvrNotFiltred_p <= OvrMatchFoundCh1_s or UdrMatchFoundCh1_s;
o_Ch2_OvrNotFiltred_p <= OvrMatchFoundCh2_s or UdrMatchFoundCh2_s;
o_Ch3_OvrNotFiltred_p <= OvrMatchFoundCh3_s or UdrMatchFoundCh3_s;
o_Ch4_OvrNotFiltred_p <= OvrMatchFoundCh4_s or UdrMatchFoundCh4_s;
                                              
o_Ch1_OvrFiltred_p <=  SetResetLatchOutCh1_s ;
o_Ch2_OvrFiltred_p <=  SetResetLatchOutCh2_s ;
o_Ch3_OvrFiltred_p <=  SetResetLatchOutCh3_s ;
o_Ch4_OvrFiltred_p <=  SetResetLatchOutCh4_s ;

end arch;