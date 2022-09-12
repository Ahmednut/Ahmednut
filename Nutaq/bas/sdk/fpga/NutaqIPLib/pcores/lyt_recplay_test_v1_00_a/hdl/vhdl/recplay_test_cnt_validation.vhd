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
-- File        : $Id: recplay_test_cnt_validation.vhd,v 1.1 2015/04/10 19:49:00 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Record Playback test module
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- François Blackburn - Initial revision 2011/02/15
-- $Log: recplay_test_cnt_validation.vhd,v $
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.3  2013/04/12 13:13:39  julien.roy
-- Disable "keep_hierarchy"
--
-- Revision 1.2  2013/04/08 14:32:01  julien.roy
-- Add 1 more register stage on Read signal to ease timing
--
-- Revision 1.1  2013/04/03 14:03:55  julien.roy
-- Commit new RTDEx and RecPlay test pcore. These new pcore does not have an AXI interface and they use Custom Registers for configuration.
--
-- Revision 1.4  2013/03/28 19:34:56  julien.roy
-- Fix error with clock divider
--
-- Revision 1.3  2013/03/28 16:20:40  julien.roy
-- Add register to easy timing
--
-- Revision 1.2  2012/11/30 21:30:23  khalid.bensadek
-- Added Registers reset. Added a data valid counter plugged on already existing reg 0x54.
--
-- Revision 1.1  2012/10/02 16:51:55  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.6  2011/03/08 20:29:46  jeffrey.johnson
-- Added pipelining.
--
-- Revision 1.5  2011/03/08 16:13:03  jeffrey.johnson
-- Added register MaxData for testing continuous playback.
-- MaxData specifies maximum value of the expected data.
--
-- Revision 1.4  2011/02/25 16:52:01  jeffrey.johnson
-- Fixed timing problems.
--
-- Revision 1.3  2011/02/21 20:37:31  jeffrey.johnson
-- Added downsampling feature.
--
--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RecPlayTestCntValidation is
generic
(
  AddressWidth_g             : integer := 28;
  PortWidth_g                : integer := 8;
  NumberOfPorts_g            : integer := 1
);
port
(

  i_RdClk_p                  : in std_logic;
  i_Reset_p                  : in std_logic;

  -- Programmable interface
  i_Start_p                  : in std_logic;
  ov16_NbErrorsPort0_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort1_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort2_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort3_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort4_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort5_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort6_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort7_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort8_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort9_p       : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort10_p      : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort11_p      : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort12_p      : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort13_p      : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort14_p      : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort15_p      : out std_logic_vector(15 downto 0);
  
  -- User interface
  iv_DataPort0_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort1_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort2_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort3_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort4_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort5_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort6_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort7_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort8_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort9_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort10_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort11_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort12_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort13_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort14_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort15_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  i_Valid_p                  : in std_logic;
  i_Empty_p                  : in std_logic;
  o_RdEn_p                   : out std_logic;
  iv32_Divnt_p               : in std_logic_vector(31 downto 0);
  iv32_MaxData_p             : in std_logic_vector(31 downto 0);
  ov32_RampCnt_p             : out std_logic_vector(31 downto 0)
);

end entity RecPlayTestCntValidation;

architecture rtl of RecPlayTestCntValidation is

  signal StartD1_s : std_logic;
  signal StartD2_s : std_logic;
  signal StartD3_s : std_logic;
  signal ResetD1_s : std_logic;
  signal ResetD2_s : std_logic;
  signal ResetD3_s : std_logic;
  signal v32_NbDataCh0_s : std_logic_vector(31 downto 0);
  signal ValidD1_s : std_logic;

  type DataPort_t is array(15 downto 0 ) of std_logic_vector(PortWidth_g - 1 downto 0);

  type ErrorPort_t is array(15 downto 0 ) of std_logic_vector(15 downto 0);

  signal v_DataPorts_s    : DataPort_t;
  signal v_DataPortsD1_s  : DataPort_t;
  signal v_NbErrorsPort_s : ErrorPort_t;
  signal v_ExpectedData_s : std_logic_vector(PortWidth_g - 1 downto 0);

  signal v_MaxData_s    : std_logic_vector(PortWidth_g - 1 downto 0);
  
  -- attribute keep_hierarchy : string;
  -- attribute keep_hierarchy of rtl : architecture is "true";

  signal v32_DivCounter_s : std_logic_vector(31 downto 0);
  signal RdEn_s           : std_logic;
  signal RdEnR1_s         : std_logic;
  signal v32_TiedToGnd_s  : std_logic_vector(31 downto 0);
  
  signal DivCounter_equal_1 : std_logic;

begin

  v32_TiedToGnd_s <= (others => '0');
  
  MaxDataGen_l : if( PortWidth_g /= 64 ) generate 
    v_MaxData_s <= iv32_MaxData_p(PortWidth_g - 1 downto 0);
  end generate;

  MaxData64Gen_l : if( PortWidth_g = 64 ) generate 
    v_MaxData_s(63 downto 32) <= (others => '0');
    v_MaxData_s(31 downto 0) <= iv32_MaxData_p(31 downto 0);
  end generate;

  v_DataPorts_s(0)  <= iv_DataPort0_p;
  v_DataPorts_s(1)  <= iv_DataPort1_p;
  v_DataPorts_s(2)  <= iv_DataPort2_p;
  v_DataPorts_s(3)  <= iv_DataPort3_p;
  v_DataPorts_s(4)  <= iv_DataPort4_p;
  v_DataPorts_s(5)  <= iv_DataPort5_p;
  v_DataPorts_s(6)  <= iv_DataPort6_p;
  v_DataPorts_s(7)  <= iv_DataPort7_p;
  v_DataPorts_s(8)  <= iv_DataPort8_p;
  v_DataPorts_s(9)  <= iv_DataPort9_p;
  v_DataPorts_s(10) <= iv_DataPort10_p;
  v_DataPorts_s(11) <= iv_DataPort11_p;
  v_DataPorts_s(12) <= iv_DataPort12_p;
  v_DataPorts_s(13) <= iv_DataPort13_p;
  v_DataPorts_s(14) <= iv_DataPort14_p;
  v_DataPorts_s(15) <= iv_DataPort15_p;

  ov16_NbErrorsPort0_p  <= v_NbErrorsPort_s(0);
  ov16_NbErrorsPort1_p  <= v_NbErrorsPort_s(1);
  ov16_NbErrorsPort2_p  <= v_NbErrorsPort_s(2);
  ov16_NbErrorsPort3_p  <= v_NbErrorsPort_s(3);
  ov16_NbErrorsPort4_p  <= v_NbErrorsPort_s(4);
  ov16_NbErrorsPort5_p  <= v_NbErrorsPort_s(5);
  ov16_NbErrorsPort6_p  <= v_NbErrorsPort_s(6);
  ov16_NbErrorsPort7_p  <= v_NbErrorsPort_s(7);
  ov16_NbErrorsPort8_p  <= v_NbErrorsPort_s(8);
  ov16_NbErrorsPort9_p  <= v_NbErrorsPort_s(9);
  ov16_NbErrorsPort10_p <= v_NbErrorsPort_s(10);
  ov16_NbErrorsPort11_p <= v_NbErrorsPort_s(11);
  ov16_NbErrorsPort12_p <= v_NbErrorsPort_s(12);
  ov16_NbErrorsPort13_p <= v_NbErrorsPort_s(13);
  ov16_NbErrorsPort14_p <= v_NbErrorsPort_s(14);
  ov16_NbErrorsPort15_p <= v_NbErrorsPort_s(15);
  
  ov32_RampCnt_p <= v32_NbDataCh0_s;

  FfProc_l : process( i_RdClk_p )
  begin
    if( rising_edge( i_RdClk_p ) ) then
      StartD1_s <= i_Start_p;
      StartD2_s <= StartD1_s;
      StartD3_s <= StartD2_s;
      ResetD1_s <= i_Reset_p;
      ResetD2_s <= ResetD1_s;
      ResetD3_s <= ResetD2_s;
      ValidD1_s <= i_Valid_p;
      
      for i in 0 to NumberOfPorts_g - 1 loop
        v_DataPortsD1_s(i) <= v_DataPorts_s(i);
      end loop;
      
    end if;

  end process;


  ReadEnableProc_l : process( i_RdClk_p )
  begin
    if( rising_edge( i_RdClk_p ) ) then
    
      if( ResetD3_s = '1' ) then
        v32_DivCounter_s <= (others => '0');
        RdEn_s <= '0';
        DivCounter_equal_1 <= '1';
      elsif( DivCounter_equal_1 = '1'  ) then
        v32_DivCounter_s <= iv32_Divnt_p;
        RdEn_s <= not i_Empty_p;
        
        if (iv32_Divnt_p = X"00000000") then
          DivCounter_equal_1 <= '1';
        else
          DivCounter_equal_1 <= '0';
        end if;
      else
        v32_DivCounter_s <= std_logic_vector( unsigned( v32_DivCounter_s ) - 1 );
        RdEn_s <= '0';
        if (v32_DivCounter_s = X"00000001") then
          DivCounter_equal_1 <= '1';
        else
          DivCounter_equal_1 <= '0';
        end if;
      end if;
    end if;
  end process;
  
  -- Latch RdEn_s to ease timing with the playback controller
  process( i_RdClk_p )
  begin
    if( rising_edge( i_RdClk_p ) ) then
      if( ResetD3_s = '1' ) then
        RdEnR1_s <= '0';
      else
        RdEnR1_s <= RdEn_s;
      end if;
    end if;
  end process; 
      
--   ExpectedDataProc_l : process( i_RdClk_p )
--   begin
--     if( rising_edge( i_RdClk_p ) ) then
--     
--       for i in 0 to NumberOfPorts_g - 1 loop
--         if( ResetD3_s = '1' ) then
--           v_ExpectedData_s(i) <= ( others => '0' );
--         elsif( ValidD1_s = '1' and StartD3_s = '1' ) then
--           v_ExpectedData_s(i) <= ( others => '0' );
--           if( v_ExpectedData_s(i) /= v_MaxData_s ) then
--             v_ExpectedData_s(i) <= std_logic_vector( unsigned( v_ExpectedData_s(i)  ) + 1 );
--           end if;
--         end if;
--       end loop;
-- 
--     end if;
-- 
--   end process;
 -------------------------------------------------------------------
 -- Generate a local ramp
 -------------------------------------------------------------------
 process(i_RdClk_p)
 begin
    if rising_edge(i_RdClk_p) then          
        if(ResetD3_s = '1') then
          v_ExpectedData_s <= (others => '0');
        elsif( ValidD1_s = '1' and StartD3_s = '1' ) then          
          if(unsigned(v_ExpectedData_s) >= unsigned(v_MaxData_s) ) then
          	v_ExpectedData_s <= (others => '0'); -- Wrap-back for continuous mode test
          else	
            v_ExpectedData_s <= std_logic_vector(unsigned(v_ExpectedData_s) + 1);
          end if;
        end if;      
    end if;
  end process;  
  --------------------------

  CounterVerifProc_l : process( i_RdClk_p )
  begin
    if( rising_edge( i_RdClk_p ) ) then
    
      for i in 0 to NumberOfPorts_g - 1 loop
        if( ResetD3_s = '1' ) then
          v_NbErrorsPort_s(i) <= ( others => '0' );
        elsif( ValidD1_s = '1' and StartD3_s = '1' ) then
          if( v_ExpectedData_s /= v_DataPortsD1_s(i) ) then
            v_NbErrorsPort_s(i) <= std_logic_vector( unsigned( v_NbErrorsPort_s(i)  ) + 1 );
          end if;
        end if;
      end loop;

    end if;

  end process;

  CounterNbDataProc_l : process( i_RdClk_p )
  begin
    if( rising_edge( i_RdClk_p ) ) then
      if( ResetD3_s = '1' ) then
        v32_NbDataCh0_s <= ( others => '0' );
      elsif( ValidD1_s = '1' and StartD3_s = '1' ) then
        v32_NbDataCh0_s <= std_logic_vector( unsigned( v32_NbDataCh0_s  ) + 1 );
      end if; 
    end if;

  end process;

  -- Output mappings
  -- o_RdEn_p <= RdEnR1_s and (not i_Empty_p);
  o_RdEn_p <= RdEnR1_s;


end rtl;

