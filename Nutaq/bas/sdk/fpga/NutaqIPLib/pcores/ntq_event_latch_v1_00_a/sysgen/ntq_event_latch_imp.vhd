
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ntq_event_latch_imp is
port
(
  clk                 : in std_logic;
  ce                  : in std_logic;  

  i_Rst_p             : in std_logic;
  i_ClkEn_p           : in std_logic;

  i_ArmLatch_p        : in std_logic;
  i_Trigger_p         : in std_logic;
  iv_Data_p           : in std_logic_vector(63 downto 0);
  i_DataWen_p         : in std_logic;

  ov_LatchData_p      : out std_logic_vector(63 downto 0);
  o_TriggerHappened_p : out std_logic
);

end entity ntq_event_latch_imp;

architecture rtl of ntq_event_latch_imp is


  component ntq_event_latch is
  generic
  (
    C_DATA_WIDTH               : integer range 1 to 256 := 32
  );
  port
  (
    i_Rst_p             : in std_logic;
    i_Clk_p             : in std_logic;
    i_ClkEn_p           : in std_logic;

    i_ArmLatch_p        : in std_logic;
    i_Trigger_p         : in std_logic;
    iv_Data_p           : in std_logic_vector(C_DATA_WIDTH-1 downto 0);
    i_DataWen_p         : in std_logic;

    ov_LatchData_p      : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
    o_TriggerHappened_p : out std_logic
  );

  end component;


begin

  U0 : ntq_event_latch
  generic map
  (
    C_DATA_WIDTH => 64
  )
  port map
  (
    i_Rst_p             => i_Rst_p,
    i_Clk_p             => clk,
    i_ClkEn_p           => i_ClkEn_p,
                        
    i_ArmLatch_p        => i_ArmLatch_p,
    i_Trigger_p         => i_Trigger_p,
    iv_Data_p           => iv_Data_p,
    i_DataWen_p         => i_DataWen_p,
                        
    ov_LatchData_p      => ov_LatchData_p,
    o_TriggerHappened_p => o_TriggerHappened_p
  );

end rtl;

