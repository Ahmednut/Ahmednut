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
-- Revision 1.1  2012/09/28 19:08:53  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.5  2012/02/01 14:36:32  khalid.bensadek
-- Update du projet
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
    use ieee.std_logic_misc.all;
    
    
    
entity ChOutOfRange is
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
end ChOutOfRange;

architecture arch of ChOutOfRange is

type SampleArray_t is array(natural range <>) of std_logic_vector(9 downto 0);
signal v8_Ch_oor_s : std_logic_vector(7 downto 0);
signal v80_DataCh_s : std_logic_vector (79 downto 0);
signal DataInSamplesArray_s : SampleArray_t (7 downto 0);

begin

SampleArrayGen: for j in 0 to 7 generate
   DataInSamplesArray_s(j) <= iv80_DataCh_p(10*j+9 downto 10*j); 
end generate SampleArrayGen;

samples_gen : for i in 0 to 7 generate
   U_OutOfRange_l: process(i_clk_p)
   begin
       if rising_edge(i_clk_p) then
           if i_rst_p ='1' then
              v8_Ch_oor_s(i) <= '0';
           else
              if (DataInSamplesArray_s(i) = PATTERN) then
                v8_Ch_oor_s(i) <= '1'; 
              else
                v8_Ch_oor_s(i) <= '0';
              end if;         
           end if;            
       end if;
   end process  U_OutOfRange_l;                 
end generate samples_gen;

-- Delay data samples to match otr
U_latchData_l: process(i_clk_p)                         
begin                                                     
    if rising_edge(i_clk_p) then                          
        if i_rst_p ='1' then                              
            v80_DataCh_s <= (others=>'0');                                
        else      
            v80_DataCh_s <= iv80_DataCh_p;                                          
         end if;                                          
    end if;                                               
end process  U_latchData_l;                              

ov80_DataCh_p <= v80_DataCh_s;
o_Ch_oor_p <= or_reduce(v8_Ch_oor_s);

end arch;