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
-- File        : $Id: recplay_test_cnt_gen.vhd,v 1.1 2015/04/10 19:49:00 sqa Exp $
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
-- $Log: recplay_test_cnt_gen.vhd,v $
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.2  2013/04/12 13:13:38  julien.roy
-- Disable "keep_hierarchy"
--
-- Revision 1.1  2013/04/03 14:03:55  julien.roy
-- Commit new RTDEx and RecPlay test pcore. These new pcore does not have an AXI interface and they use Custom Registers for configuration.
--
-- Revision 1.3  2013/03/28 16:20:39  julien.roy
-- Add register to easy timing
--
-- Revision 1.2  2012/11/30 21:30:23  khalid.bensadek
-- Added Registers reset. Added a data valid counter plugged on already existing reg 0x54.
--
-- Revision 1.1  2012/10/02 16:51:55  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.3  2011/02/25 16:52:01  jeffrey.johnson
-- Fixed timing problems.
--
-- Revision 1.2  2011/02/21 20:37:31  jeffrey.johnson
-- Added downsampling feature.
--
--------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RecPlayTestCntGen is
generic
(
  AddressWidth_g             : integer := 28;
  PortWidth_g                : integer := 8;
  NumberOfPorts_g            : integer := 1
);
port
(

  i_WrClk_p                  : in std_logic;
  i_Reset_p                  : in std_logic;

  -- Programmable interface
  i_Start_p : in std_logic;
  i_Trig_p : in std_logic;
  iv_TriggerAddr_p : in std_logic_vector( AddressWidth_g - 1 downto 0 );


  -- User interface
  o_Trigger_p                : out std_logic;
  ov_DataPort0_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort1_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort2_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort3_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort4_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort5_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort6_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort7_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort8_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort9_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort10_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort11_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort12_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort13_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort14_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort15_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  o_WriteEn_p                : out std_logic;
  i_FifoFull_p               : in std_logic;
  iv32_Divnt_p               : in std_logic_vector(31 downto 0)
);

end entity RecPlayTestCntGen;

architecture rtl of RecPlayTestCntGen is

  signal StartD1_s : std_logic;
  signal StartD2_s : std_logic;
  signal ResetD1_s : std_logic;
  signal ResetD2_s : std_logic;


  signal TrigD1_s : std_logic;
  signal TrigD2_s : std_logic;
  signal TrigD3_s : std_logic;
  signal TrigD4_s : std_logic;
  signal TrigD5_s : std_logic;
  signal TrigRise_s : std_logic;

  signal Trigger_s : std_logic;
  signal Trigger_r1 : std_logic;
  signal WriteEn_s : std_logic;
  signal WriteEnD1_s : std_logic;
  signal WriteEnD2_s : std_logic;
  signal StartCapted_s : std_logic;
  signal StartCaptedD1_s : std_logic;

  type DataPort_t is array(15 downto 0 ) of std_logic_vector(PortWidth_g - 1 downto 0);

  signal v_DataPorts_s : DataPort_t;
  signal v_DataPorts_r1 : DataPort_t;

  signal v_TriggerAddr_s : std_logic_vector( AddressWidth_g - 1 downto 0 );
  signal v_Addr_s : std_logic_vector( AddressWidth_g - 1 downto 0 );

  type TrigState_t is ( Idle_c, TrigCapted_c, StartCapted_c, SendData_c );

  signal TrigState_s : TrigState_t;

  -- attribute keep_hierarchy : string;
  -- attribute keep_hierarchy of rtl : architecture is "true";

  attribute fsm_encoding: string;
  attribute fsm_encoding of TrigState_s: signal is "sequential";
  attribute safe_implementation: string;
  attribute safe_implementation of TrigState_s: signal is "true";
  
  
  attribute keep: string;
  attribute keep of v_Addr_s : signal is "true";  
  
  signal v32_DivCounter_s : std_logic_vector(31 downto 0);
  signal DivCntReached_s  : std_logic;
  signal v32_TiedToGnd_s  : std_logic_vector(31 downto 0);
  

begin

  v32_TiedToGnd_s <= (others => '0');

  TrigRise_s <= TrigD4_s and ( not TrigD5_s );
  o_Trigger_p <= Trigger_r1;
  o_WriteEn_p <= WriteEnD2_s;

  ov_DataPort0_p   <=  v_DataPorts_r1(0);
  ov_DataPort1_p   <=  v_DataPorts_r1(1);
  ov_DataPort2_p   <=  v_DataPorts_r1(2);
  ov_DataPort3_p   <=  v_DataPorts_r1(3);
  ov_DataPort4_p   <=  v_DataPorts_r1(4);
  ov_DataPort5_p   <=  v_DataPorts_r1(5);
  ov_DataPort6_p   <=  v_DataPorts_r1(6);
  ov_DataPort7_p   <=  v_DataPorts_r1(7);
  ov_DataPort8_p   <=  v_DataPorts_r1(8);
  ov_DataPort9_p   <=  v_DataPorts_r1(9);
  ov_DataPort10_p  <=  v_DataPorts_r1(10);
  ov_DataPort11_p  <=  v_DataPorts_r1(11);
  ov_DataPort12_p  <=  v_DataPorts_r1(12);
  ov_DataPort13_p  <=  v_DataPorts_r1(13);
  ov_DataPort14_p  <=  v_DataPorts_r1(14);
  ov_DataPort15_p  <=  v_DataPorts_r1(15);

  FfProc_l : process( i_WrClk_p )
  begin
    if( rising_edge( i_WrClk_p ) ) then
    
      ResetD1_s <= i_Reset_p;
      ResetD2_s <= ResetD1_s;
      StartD1_s <= i_Start_p;
      StartD2_s <= StartD1_s;
      StartCaptedD1_s <= StartCapted_s;
      
      if( TrigRise_s = '1' ) then
        v_TriggerAddr_s <= iv_TriggerAddr_p;
      end if;
      
      if( ResetD2_s = '1' ) then
        v_DataPorts_r1(0) <= (others=>'0');
        v_DataPorts_r1(1) <= (others=>'0');
        v_DataPorts_r1(2) <= (others=>'0');
        v_DataPorts_r1(3) <= (others=>'0');
        v_DataPorts_r1(4) <= (others=>'0');
        v_DataPorts_r1(5) <= (others=>'0');
        v_DataPorts_r1(6) <= (others=>'0');
        v_DataPorts_r1(7) <= (others=>'0');
        v_DataPorts_r1(8) <= (others=>'0');
        v_DataPorts_r1(9) <= (others=>'0');
        v_DataPorts_r1(10) <= (others=>'0');
        v_DataPorts_r1(11) <= (others=>'0');
        v_DataPorts_r1(12) <= (others=>'0');
        v_DataPorts_r1(13) <= (others=>'0');
        v_DataPorts_r1(14) <= (others=>'0');
        v_DataPorts_r1(15) <= (others=>'0');
        
        WriteEnD1_s <= '0';
        WriteEnD2_s <= '0';
        
        TrigD1_s <= '0';
        TrigD2_s <= '0';
        TrigD3_s <= '0';
        TrigD4_s <= '0';
        TrigD5_s <= '0';
        
        Trigger_r1  <= '0';
        
      else
        v_DataPorts_r1 <= v_DataPorts_s;
        WriteEnD1_s <= WriteEn_s;
        WriteEnD2_s <= WriteEnD1_s;
        TrigD1_s <= i_Trig_p;
        TrigD2_s <= TrigD1_s;
        TrigD3_s <= TrigD2_s;
        TrigD4_s <= TrigD3_s;
        TrigD5_s <= TrigD4_s;
        Trigger_r1 <= Trigger_s;
        
      end if;
    end if;

  end process;

  TrigStateProc_l : process( i_WrClk_p )
  begin
    if( rising_edge( i_WrClk_p )) then
      if( ResetD2_s = '1' ) then
        TrigState_s <= Idle_c;
      else
        case TrigState_s is
          when Idle_c =>
            if( TrigRise_s = '1' ) then
              TrigState_s <= TrigCapted_c;
            end if;

          when TrigCapted_c =>
            if( StartD2_s = '1' ) then
              TrigState_s <= StartCapted_c;
            end if;

          when StartCapted_c =>
            if( v_Addr_s = v_TriggerAddr_s ) then
              TrigState_s <= SendData_c;
            end if;

          when SendData_c =>
             if( StartD2_s = '0' ) then
               TrigState_s <= Idle_c;
             end if;

          when others =>
            TrigState_s <= Idle_c;
        end case;
      end if;
    end if;
  end process;


  TrigStateDecode_l : process( TrigState_s, v_Addr_s)
                               
  begin
    StartCapted_s <= '0';

    case TrigState_s is
      when Idle_c =>
        null;

      when TrigCapted_c =>
        null;

      when StartCapted_c =>
        StartCapted_s <= '1';

      when SendData_c =>
         StartCapted_s <= '1';

      when others =>

    end case;

  end process;

  WriteEn_s <= '1' when ( DivCntReached_s = '1' and StartCaptedD1_s = '1' and i_FifoFull_p = '0' ) else '0';
  
  CounterProc_l : process( i_WrClk_p )
  begin
    if( rising_edge( i_WrClk_p ) ) then
    
      if( ResetD2_s = '1' ) then
        Trigger_s <= '0';
      elsif( WriteEnD1_s = '1' and v_Addr_s = std_logic_vector(unsigned(v_TriggerAddr_s) - 1 )) then --Trig address should never be 0.
        Trigger_s <= '1';
      elsif( WriteEnD1_s = '1' ) then
        Trigger_s <= '0';
      end if;
      
      if( ResetD2_s = '1' ) then
        v_Addr_s <= ( others => '0' );
      elsif( WriteEnD1_s = '1' ) then
        v_Addr_s <= std_logic_vector( unsigned( v_Addr_s ) + 1 );
      end if;

      for i in 0 to NumberOfPorts_g - 1 loop
        if( ResetD2_s = '1' ) then
          v_DataPorts_s(i) <= ( others => '0' );
        elsif( WriteEnD1_s = '1' ) then
          v_DataPorts_s(i)  <= std_logic_vector( unsigned( v_DataPorts_s(i)  ) + 1 );
        end if;
      end loop;

      if( ResetD2_s = '1' ) then
        v32_DivCounter_s <= (others => '0');
        DivCntReached_s <= '0';
      elsif( StartCapted_s = '1' and i_FifoFull_p = '0' ) then

        if( v32_DivCounter_s = v32_TiedToGnd_s  ) then
          v32_DivCounter_s <= iv32_Divnt_p;
          DivCntReached_s <= '1';
        else
          v32_DivCounter_s <= std_logic_vector( unsigned( v32_DivCounter_s ) - 1 );
          DivCntReached_s <= '0';
        end if;

      else
        DivCntReached_s <= '0';
      end if;
      
    end if;

  end process;






end rtl;

