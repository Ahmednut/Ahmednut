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
-- File        : $Id: ch_out_of_range.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
--------------------------------------------------------------------------------
-- Description : ChOutOfRange module for ADAC250
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
-- $Log: ch_out_of_range.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
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
    use ieee.numeric_std.all;
    
entity ChOutOfRange is
  generic (
             PatternOffsetBin   : std_logic_vector(13 downto 0);
             Pattern2s          : std_logic_vector(13 downto 0)
            );
  port(
    i_clk_p : in std_logic;
    i_rst_p : in std_logic;
    iv14_DataCh_p   : in std_logic_vector (13 downto 0);
    i_DataType_p : in std_logic;
    ov14_DataCh_p   : out std_logic_vector (13 downto 0);
    o_Ch_oor_p   : out std_logic
      );
end ChOutOfRange;

architecture arch of ChOutOfRange is

signal Ch_oor_s : std_logic;
signal v14_DataCh_s : std_logic_vector (13 downto 0);

begin
U_OutOfRange_l: process (i_clk_p)
begin
    if rising_edge(i_clk_p) then
        if i_rst_p ='1' then
          Ch_oor_s <= '0';
        else
            if (i_DataType_p ='0') then
                if (iv14_DataCh_p = PatternOffsetBin) then
                    Ch_oor_s <= '1'; 
                else
                    Ch_oor_s <= '0';
                end if;
            else
                if (iv14_DataCh_p = Pattern2s) then
                    Ch_oor_s <= '1'; 
                else
                    Ch_oor_s <= '0';
                end if;
            end if; 
         end if;            
    end if;
end process  U_OutOfRange_l;

U_latchData_l: process (i_clk_p)                         
begin                                                     
    if rising_edge(i_clk_p) then                          
        if i_rst_p ='1' then                              
            v14_DataCh_s <= (others=>'0');                                
        else      
            v14_DataCh_s <= iv14_DataCh_p;                                          
         end if;                                          
    end if;                                               
end process  U_latchData_l;                              

ov14_DataCh_p <= v14_DataCh_s;
o_Ch_oor_p <= Ch_oor_s;

end arch;