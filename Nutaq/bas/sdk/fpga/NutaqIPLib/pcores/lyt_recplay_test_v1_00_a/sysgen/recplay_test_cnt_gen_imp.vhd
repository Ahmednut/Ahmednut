
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity RecPlayTestCntGenImp is
port
(

  clk                        : in std_logic;
  ce                         : in std_logic;  
  i_Reset_p                  : in std_logic;

  -- Programmable interface
  i_Start_p : in std_logic;
  i_Trig_p : in std_logic;
  
  iv32_TriggerAddr_p : in std_logic_vector(31 downto 0);
  iv32_Divnt_p : in std_logic_vector(31 downto 0);


  -- User interface
  o_Trigger_p                : out std_logic;
  ov_DataPort0_p             : out std_logic_vector(15 downto 0);
  ov_DataPort1_p             : out std_logic_vector(15 downto 0);
  o_WriteEn_p                : out std_logic;
  i_FifoFull_p               : in std_logic
);

end entity RecPlayTestCntGenImp;

architecture rtl of RecPlayTestCntGenImp is


  component RecPlayTestCntGen is
  generic
  (
    AddressWidth_g             : integer := 30;
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

  end component;


begin

  U0 : RecPlayTestCntGen
  generic map
  (
    AddressWidth_g   => 30,
    PortWidth_g      => 16 ,
    NumberOfPorts_g  => 2
  )
  port map
  (
  
    i_WrClk_p                  => clk,
    i_Reset_p                  => i_Reset_p,
  
    -- Programmable interface
    i_Start_p                 => i_Start_p,  
    i_Trig_p                  => i_Trig_p,   
    iv_TriggerAddr_p          => iv32_TriggerAddr_p(29 downto 0),
    iv32_Divnt_p              => iv32_Divnt_p, 
  
    -- User interface
    o_Trigger_p       => o_Trigger_p,        
    ov_DataPort0_p    => ov_DataPort0_p,     
    ov_DataPort1_p    => ov_DataPort1_p ,    
    o_WriteEn_p       => o_WriteEn_p,        
    i_FifoFull_p      => i_FifoFull_p       
  );



end rtl;

