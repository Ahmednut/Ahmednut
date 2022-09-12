LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

package recplay_type_p is


  constant v3_ModeNone_c : std_logic_vector( 2 downto 0 ) := "000";
  constant v3_ModeRecord_c : std_logic_vector( 2 downto 0 ) :=  "001";
  constant v3_ModePlayBackSingle_c :  std_logic_vector( 2 downto 0 ) :=  "010";
  constant v3_ModePlayBackContinuous_c :  std_logic_vector( 2 downto 0 ) :=  "011";
  constant v3_ModeRTDExMem2Host_c : std_logic_vector( 2 downto 0 ) :=  "100";
  constant v3_ModeRTDExHost2Mem_c : std_logic_vector( 2 downto 0 ) :=  "101";
  constant v3_ModeMemValidation_c : std_logic_vector( 2 downto 0 ) :=  "110";
  
  constant CMD_WRITE_c	: std_logic_vector(2 downto 0) := "000";
  constant CMD_READ_c	: std_logic_vector(2 downto 0) := "001";


end package recplay_type_p;