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
-- File : lyt_recplay_test.vhd
--------------------------------------------------------------------------------
-- Description : Record/Playback test core
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_recplay_test.vhd,v $
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.1  2013/04/03 14:03:55  julien.roy
-- Commit new RTDEx and RecPlay test pcore. These new pcore does not have an AXI interface and they use Custom Registers for configuration.
--
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.std_logic_misc.all;

library lyt_recplay_test_v1_00_a;
  use lyt_recplay_test_v1_00_a.all;

-------------------------------------------------------------------------------
-- Entity Section
-------------------------------------------------------------------------------
entity lyt_recplay_test is
  generic
  (
    AddressWidth_g             : integer := 30;
    PortWidth_g                : integer := 8;
    NumberOfPorts_g            : integer := 1
  );
  port
  (
    -- Control ports
    i_Reset_p                     : in std_logic;
    i_Start_p                     : in std_logic;
    i_SetRecTrig_p                : in std_logic;
    i_SetPlayTrig_p               : in std_logic;

    iv32_TrigAddress_p            : in std_logic_vector(31 downto 0);

    ov32_NbErrorsPort0_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort1_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort2_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort3_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort4_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort5_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort6_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort7_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort8_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort9_p          : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort10_p         : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort11_p         : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort12_p         : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort13_p         : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort14_p         : out std_logic_vector(31 downto 0);
    ov32_NbErrorsPort15_p         : out std_logic_vector(31 downto 0);

    ov32_NbDataCh0_p              : out std_logic_vector(31 downto 0);

    iv32_Divnt_p                  : in std_logic_vector(31 downto 0);

    iv32_MaxData_p                : in std_logic_vector(31 downto 0);

    -- User ports
    i_WrClk_p                     : in std_logic;
    i_RdClk_p                     : in std_logic;

    o_RecTrigger_p                : out std_logic;
    ov_RecDataPort0_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort1_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort2_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort3_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort4_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort5_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort6_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort7_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort8_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort9_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort10_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort11_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort12_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort13_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort14_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
    ov_RecDataPort15_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
    o_RecWriteEn_p                : out std_logic;
    i_RecFifoFull_p               : in std_logic;

    o_PlayTrigger_p               : out std_logic;
    iv_PlayDataPort0_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort1_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort2_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort3_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort4_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort5_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort6_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort7_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort8_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort9_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort10_p           : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort11_p           : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort12_p           : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort13_p           : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort14_p           : in std_logic_vector(PortWidth_g - 1 downto 0);
    iv_PlayDataPort15_p           : in std_logic_vector(PortWidth_g - 1 downto 0);
    i_PlayValid_p                 : in std_logic;
    i_PlayEmpty_p                 : in std_logic;
    o_PlayRdEn_p                  : out std_logic
  );
end entity lyt_recplay_test;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of lyt_recplay_test is

begin

  RecPlayTestCntGen_l : entity lyt_recplay_test_v1_00_a.RecPlayTestCntGen
    generic map
    (  
      AddressWidth_g   => AddressWidth_g,
      PortWidth_g      => PortWidth_g,
      NumberOfPorts_g  => NumberOfPorts_g
    )
    port  map
    (
      i_WrClk_p  => i_WrClk_p,
      i_Reset_p  => i_Reset_p,

      -- Programmable interface
      i_Start_p        =>    i_Start_p,
      i_Trig_p         =>    i_SetRecTrig_p,
      iv_TriggerAddr_p =>    iv32_TrigAddress_p(AddressWidth_g-1 downto 0),


      -- User interface
      o_Trigger_p         =>  o_RecTrigger_p,
      ov_DataPort0_p      =>  ov_RecDataPort0_p,
      ov_DataPort1_p      =>  ov_RecDataPort1_p,
      ov_DataPort2_p      =>  ov_RecDataPort2_p,
      ov_DataPort3_p      =>  ov_RecDataPort3_p,
      ov_DataPort4_p      =>  ov_RecDataPort4_p,
      ov_DataPort5_p      =>  ov_RecDataPort5_p,
      ov_DataPort6_p      =>  ov_RecDataPort6_p,
      ov_DataPort7_p      =>  ov_RecDataPort7_p,
      ov_DataPort8_p      =>  ov_RecDataPort8_p,
      ov_DataPort9_p      =>  ov_RecDataPort9_p,
      ov_DataPort10_p     =>  ov_RecDataPort10_p,
      ov_DataPort11_p     =>  ov_RecDataPort11_p,
      ov_DataPort12_p     =>  ov_RecDataPort12_p,
      ov_DataPort13_p     =>  ov_RecDataPort13_p,
      ov_DataPort14_p     =>  ov_RecDataPort14_p,
      ov_DataPort15_p     =>  ov_RecDataPort15_p,
      o_WriteEn_p         =>  o_RecWriteEn_p,
      i_FifoFull_p        =>  i_RecFifoFull_p,
      iv32_Divnt_p        =>  iv32_Divnt_p
    );

    
  o_PlayTrigger_p <= i_SetPlayTrig_p;

  RecPlayTestCntValidation_l : entity lyt_recplay_test_v1_00_a.RecPlayTestCntValidation
    generic map
    (
      AddressWidth_g        => AddressWidth_g,
      PortWidth_g           => PortWidth_g,
      NumberOfPorts_g       => NumberOfPorts_g
    )
    port map
    (
      i_RdClk_p             => i_RdClk_p,
      i_Reset_p             => i_Reset_p,

      -- Programmable interface
      i_Start_p             => i_Start_p,

      ov16_NbErrorsPort0_p  => ov32_NbErrorsPort0_p(15 downto 0),
      ov16_NbErrorsPort1_p  => ov32_NbErrorsPort1_p(15 downto 0),
      ov16_NbErrorsPort2_p  => ov32_NbErrorsPort2_p(15 downto 0),
      ov16_NbErrorsPort3_p  => ov32_NbErrorsPort3_p(15 downto 0),
      ov16_NbErrorsPort4_p  => ov32_NbErrorsPort4_p(15 downto 0),
      ov16_NbErrorsPort5_p  => ov32_NbErrorsPort5_p(15 downto 0),
      ov16_NbErrorsPort6_p  => ov32_NbErrorsPort6_p(15 downto 0),
      ov16_NbErrorsPort7_p  => ov32_NbErrorsPort7_p(15 downto 0),
      ov16_NbErrorsPort8_p  => ov32_NbErrorsPort8_p(15 downto 0),
      ov16_NbErrorsPort9_p  => ov32_NbErrorsPort9_p(15 downto 0),
      ov16_NbErrorsPort10_p => ov32_NbErrorsPort10_p(15 downto 0),
      ov16_NbErrorsPort11_p => ov32_NbErrorsPort11_p(15 downto 0),
      ov16_NbErrorsPort12_p => ov32_NbErrorsPort12_p(15 downto 0),
      ov16_NbErrorsPort13_p => ov32_NbErrorsPort13_p(15 downto 0),
      ov16_NbErrorsPort14_p => ov32_NbErrorsPort14_p(15 downto 0),
      ov16_NbErrorsPort15_p => ov32_NbErrorsPort15_p(15 downto 0),


      -- User interface
      iv_DataPort0_p        => iv_PlayDataPort0_p,
      iv_DataPort1_p        => iv_PlayDataPort1_p,
      iv_DataPort2_p        => iv_PlayDataPort2_p,
      iv_DataPort3_p        => iv_PlayDataPort3_p,
      iv_DataPort4_p        => iv_PlayDataPort4_p,
      iv_DataPort5_p        => iv_PlayDataPort5_p,
      iv_DataPort6_p        => iv_PlayDataPort6_p,
      iv_DataPort7_p        => iv_PlayDataPort7_p,
      iv_DataPort8_p        => iv_PlayDataPort8_p,
      iv_DataPort9_p        => iv_PlayDataPort9_p,
      iv_DataPort10_p       => iv_PlayDataPort10_p,
      iv_DataPort11_p       => iv_PlayDataPort11_p,
      iv_DataPort12_p       => iv_PlayDataPort12_p,
      iv_DataPort13_p       => iv_PlayDataPort13_p,
      iv_DataPort14_p       => iv_PlayDataPort14_p,
      iv_DataPort15_p       => iv_PlayDataPort15_p,
      i_Valid_p             => i_PlayValid_p,
      i_Empty_p             => i_PlayEmpty_p,
      o_RdEn_p              => o_PlayRdEn_p,
      iv32_Divnt_p          => iv32_Divnt_p,
      iv32_MaxData_p        => iv32_MaxData_p,
      ov32_RampCnt_p        => ov32_NbDataCh0_p
    );
    
  ov32_NbErrorsPort0_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort1_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort2_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort3_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort4_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort5_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort6_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort7_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort8_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort9_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort10_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort11_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort12_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort13_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort14_p(31 downto 16) <= (others => '0');
  ov32_NbErrorsPort15_p(31 downto 16) <= (others => '0');

end IMP;

