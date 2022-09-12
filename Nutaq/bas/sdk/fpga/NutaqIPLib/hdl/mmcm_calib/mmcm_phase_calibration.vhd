--------------------------------------------------------------------------------
-- Company: Xilinx
-- Engineer: Xilinx Appplications Team
--
-- Create Date: Jan 12, 2010
-- Design Name: mmcm_phase_calibration
-- Component Name: mmcm_phase_calibration
-- Target Device: Virtex-6
-- Tool versions: 11.4
-- Description:
--    MMCM Phase Calibration Macro
-- Dependencies:
--    Must be used in conjunction with an MMCM
-- Revision:
--    1.0
-- Additional Comments:
--    None
--------------------------------------------------------------------------------
library IEEE;
use IEEE.std_logic_1164.all;

Library UNISIM;
use UNISIM.vcomponents.all;

entity mmcm_phase_calibration is
    Port ( mmcm_lock : in std_logic;
           user_reset, user_pwrdwn : in std_logic;
           mmcm_reset, user_lock, mmcm_pwrdwn : out std_logic);
end mmcm_phase_calibration;

architecture BEHAVE of mmcm_phase_calibration is

  signal clrflops, la_pwrup, clkflops : std_logic;
  signal F1, F2, F3 : std_logic;
  signal mmcm_pwrdwn_internal, mmcm_reset_internal : std_logic;

begin

   process (clkflops, mmcm_pwrdwn_internal)
   begin  
      if mmcm_pwrdwn_internal = '1' then
         F1 <= '0';
         F3 <= '0';
      elsif (clkflops'event and clkflops = '1') then
         F1 <= '1';
         F3 <= F2;
      end if;
   end process;

   process (mmcm_reset_internal, mmcm_pwrdwn_internal)
   begin  
      if mmcm_pwrdwn_internal = '1' then
         F2 <= '0';
      elsif (mmcm_reset_internal'event and mmcm_reset_internal = '0') then
         F2 <= F1;
      end if;
   end process;


   -- LDCE: Transparent latch with Asynchronous Reset and
   --        Gate Enable.
   --        Virtex-6
   -- Xilinx HDL Language Template, version 11.4

   ld_pwrup : LDCE
   generic map (
      INIT => '0') -- Initial value of latch ('0' or '1')  
   port map (
      Q => la_pwrup,  -- Data output
      CLR => '0',     -- Asynchronous clear/reset input
      D => '1',       -- Data input
      G => mmcm_lock, -- Gate input
      GE => '1'       -- Gate enable input
   );

   -- End of ld_pwrup instantiation

  clkflops <= mmcm_lock AND la_pwrup;
  mmcm_reset_internal <= mmcm_lock AND F1 AND (NOT F2);
  mmcm_reset <= mmcm_reset_internal;

  user_lock <= F3 AND clkflops;
  mmcm_pwrdwn_internal <= user_reset OR user_pwrdwn;
  mmcm_pwrdwn <= mmcm_pwrdwn_internal;
   
end BEHAVE;
