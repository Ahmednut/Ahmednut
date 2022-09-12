
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RecPlayTestCntValidationImp is
port
(

  clk                  : in std_logic;
  ce                  : in std_logic;  
  i_Reset_p                  : in std_logic;

  -- Programmable interface
  i_Start_p : in std_logic;
  ov16_NbErrorsPort0_p  : out std_logic_vector(15 downto 0);
  ov16_NbErrorsPort1_p  : out std_logic_vector(15 downto 0);
  ov32_RampCnt_p        : out std_logic_vector(31 downto 0);
  

  -- User interface
  iv_DataPort0_p             : in std_logic_vector(15 downto 0);
  iv_DataPort1_p             : in std_logic_vector(15 downto 0);
  i_Valid_p                  : in std_logic;
  iv32_Divnt_p               : in std_logic_vector(31 downto 0);
  iv32_MaxData_p             : in std_logic_vector(31 downto 0)
);

end entity RecPlayTestCntValidationImp;


architecture rtl of RecPlayTestCntValidationImp is

  component RecPlayTestCntValidation is
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
    i_Start_p : in std_logic;
    ov16_NbErrorsPort0_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort1_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort2_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort3_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort4_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort5_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort6_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort7_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort8_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort9_p  : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort10_p : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort11_p : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort12_p : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort13_p : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort14_p : out std_logic_vector(15 downto 0);
    ov16_NbErrorsPort15_p : out std_logic_vector(15 downto 0);
    
  
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
  
  end component RecPlayTestCntValidation;

begin

U0 : RecPlayTestCntValidation 
  generic map
  (
    AddressWidth_g  => 28, 
    PortWidth_g     => 16, 
    NumberOfPorts_g => 2
  )
  port map
  (
  
    i_RdClk_p                  => clk,
    i_Reset_p                  => i_Reset_p,
  
    -- Programmable interface
    i_Start_p             => i_Start_p,            
    ov16_NbErrorsPort0_p  => ov16_NbErrorsPort0_p, 
    ov16_NbErrorsPort1_p  => ov16_NbErrorsPort1_p, 
                                                  
                                                  
    -- User interface        -- User interface    
    iv_DataPort0_p        => iv_DataPort0_p,       
    iv_DataPort1_p        => iv_DataPort1_p,       
                                         
    iv_DataPort2_p        => x"0000",
    iv_DataPort3_p        => x"0000",
    iv_DataPort4_p        => x"0000",
    iv_DataPort5_p        => x"0000",
    iv_DataPort6_p        => x"0000",
    iv_DataPort7_p        => x"0000",
    iv_DataPort8_p        => x"0000",
    iv_DataPort9_p        => x"0000",
    iv_DataPort10_p       => x"0000",
    iv_DataPort11_p       => x"0000",
    iv_DataPort12_p       => x"0000",
    iv_DataPort13_p       => x"0000",
    iv_DataPort14_p       => x"0000",       
    iv_DataPort15_p       => x"0000",
    i_Valid_p             => i_Valid_p,     
    i_Empty_p             => '0',       
    iv32_Divnt_p          => iv32_Divnt_p,  
    iv32_MaxData_p        => iv32_MaxData_p,
    ov32_RampCnt_p        => ov32_RampCnt_p
  );                      

END rtl;

