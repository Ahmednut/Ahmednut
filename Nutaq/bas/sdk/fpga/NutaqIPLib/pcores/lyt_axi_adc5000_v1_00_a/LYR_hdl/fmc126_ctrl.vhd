-------------------------------------------------------------------------------------
-- FILE NAME : fmc126_ctrl.vhd
--
-- AUTHOR    : Peter Kortekaas
--
-- COMPANY   : 4DSP
--
-- ITEM      : 1
--
-- UNITS     : Entity       - fmc126_ctrl
--             architecture - fmc126_ctrl_syn
--
-- LANGUAGE  : VHDL
--
-------------------------------------------------------------------------------------
--
-------------------------------------------------------------------------------------
-- DESCRIPTION
-- ===========
--
-- fmc126_ctrl
-- Notes: fmc126_ctrl
-------------------------------------------------------------------------------------
--  Disclaimer: LIMITED WARRANTY AND DISCLAIMER. These designs are
--              provided to you as is.  4DSP specifically disclaims any
--              implied warranties of merchantability, non-infringement, or
--              fitness for a particular purpose. 4DSP does not warrant that
--              the functions contained in these designs will meet your
--              requirements, or that the operation of these designs will be
--              uninterrupted or error free, or that defects in the Designs
--              will be corrected. Furthermore, 4DSP does not warrant or
--              make any representations regarding use or the results of the
--              use of the designs in terms of correctness, accuracy,
--              reliability, or otherwise.
--
--              LIMITATION OF LIABILITY. In no event will 4DSP or its
--              licensors be liable for any loss of data, lost profits, cost
--              or procurement of substitute goods or services, or for any
--              special, incidental, consequential, or indirect damages
--              arising from the use or operation of the designs or
--              accompanying documentation, however caused and on any theory
--              of liability. This limitation will apply even if 4DSP
--              has been advised of the possibility of such damage. This
--              limitation shall apply not-withstanding the failure of the
--              essential purpose of any limited remedies herein.
--
----------------------------------------------

-- Library declarations
library ieee;
  use ieee.std_logic_1164.all;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_unsigned.all;
library unisim;
  use unisim.vcomponents.all;

entity fmc126_ctrl is  
  port (
    rst                    : in  std_logic;

    -- Registers Interface
    clk_regs               : in  std_logic;                   -- system/Microblaze clock that registers are synced to it
    v3_TrigCtrl_p          : in std_logic_vector(2 downto 0); -- from Trigger control reg
                
    --External trigger
    ext_trigger_p          : in  std_logic;
    ext_trigger_n          : in  std_logic;
    ext_trigger_buf        : out std_logic;
    triggerOut                : out std_logic;

    -- Clock
    phy_clk                 : in  std_logic
        
  );
end fmc126_ctrl;

architecture fmc126_ctrl_syn of fmc126_ctrl is

----------------------------------------------------------------------------------------------------
-- Components
----------------------------------------------------------------------------------------------------
component pulse2pulse is
port (
  in_clk   : in  std_logic;
  out_clk  : in  std_logic;
  rst      : in  std_logic;
  pulsein  : in  std_logic;
  inbusy   : out std_logic;
  pulseout : out std_logic
);
end component pulse2pulse;
----------------------------------------------------------------------------------------------------
-- Constants
----------------------------------------------------------------------------------------------------
constant EXT_TRIGGER_RISE     : std_logic_vector(1 downto 0) := "01";
constant EXT_TRIGGER_FALL     : std_logic_vector(1 downto 0) := "10";
constant EXT_TRIGGER_BOTH     : std_logic_vector(1 downto 0) := "11";

----------------------------------------------------------------------------------------------------
-- Signals
----------------------------------------------------------------------------------------------------
signal trigger_sel_reg   : std_logic_vector(1 downto 0);
signal sw_trigger        : std_logic;
signal trigger           : std_logic;

signal ext_trigger       : std_logic;
signal ext_trigger_prev0 : std_logic;
signal ext_trigger_prev1 : std_logic;
signal ext_trigger_re    : std_logic;
signal ext_trigger_fe    : std_logic;

signal v3_TrigCtrl_tmp   : std_logic_vector(2 downto 0);

begin

----------------------------------------------------------------------------------------------------
-- Transfer command pulses to other ADC0 clock domain
----------------------------------------------------------------------------------------------------
Trig_Ctrl_pls: for i in 0 to 2 generate

  pulse2pulse_inst : pulse2pulse
  port map (
    in_clk   => clk_regs,
    out_clk  => phy_clk,
    rst      => rst,
    pulsein  => v3_TrigCtrl_p(i),
    inbusy   => open,
    pulseout => v3_TrigCtrl_tmp(i)
  );

end generate;

----------------------------------------------------------------------------------------------------
-- Map pulses
----------------------------------------------------------------------------------------------------
trigger_sel_reg  <= v3_TrigCtrl_tmp(1 downto 0);
sw_trigger <= v3_TrigCtrl_tmp(2);

----------------------------------------------------------------------------------------------------
-- LVDS Trigger Input
----------------------------------------------------------------------------------------------------
ibufds_trig : ibufds
generic map (
  IOSTANDARD => "LVDS_25",
  DIFF_TERM => TRUE
)
port map (
  i => ext_trigger_p,
  ib => ext_trigger_n,
  o => ext_trigger
);

-----------------------------------------------------------------------------------
-- ADC triggering and burst control
-----------------------------------------------------------------------------------
process (rst, phy_clk)
begin
  if (rst = '1') then
    ext_trigger_prev0 <= '0';
    ext_trigger_prev1 <= '0';
    ext_trigger_re    <= '0';
    ext_trigger_fe    <= '0';
    trigger           <= '0';
    
  elsif (rising_edge(phy_clk)) then

    ext_trigger_prev0 <= ext_trigger;
    ext_trigger_prev1 <= ext_trigger_prev0;

    -- Generate pulse on rising edge external trigger
    if (ext_trigger_prev0 = '1' and ext_trigger_prev1 = '0') then
      ext_trigger_re <= '1';
    else
      ext_trigger_re <= '0';
    end if;

    -- Generate pulse on falling edge external trigger
    if (ext_trigger_prev0 = '0' and ext_trigger_prev1 = '1') then
      ext_trigger_fe <= '1';
    else
      ext_trigger_fe <= '0';
    end if;

    -- Select the trigger source
    if (sw_trigger = '1') then
      trigger <= '1';
    elsif (ext_trigger_re = '1' and (trigger_sel_reg = EXT_TRIGGER_RISE or trigger_sel_reg = EXT_TRIGGER_BOTH) ) then
      trigger <= '1';
    elsif (ext_trigger_fe = '1' and (trigger_sel_reg = EXT_TRIGGER_FALL or trigger_sel_reg = EXT_TRIGGER_BOTH) ) then
      trigger <= '1';
    else
      trigger <= '0';
    end if;

  end if;
end process;

----------------------------------------------------------------------------------------------------
-- Connect entity
----------------------------------------------------------------------------------------------------
ext_trigger_buf <= ext_trigger_prev1;
triggerOut <= trigger;

----------------------------------------------------------------------------------------------------
-- End
----------------------------------------------------------------------------------------------------
end fmc126_ctrl_syn;
