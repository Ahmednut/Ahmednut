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
-- File : ntq_trig_delay.vhd
--------------------------------------------------------------------------------
-- Description : Logic to delay a trigger
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2014 Nutaq inc.
--------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;
    use ieee.std_logic_unsigned.all;

entity ntq_trig_delay is
  port (
    -- Control signals
    --
    i_clk_p                   : in std_logic;
    i_rst_p                   : in std_logic;
    iv32_delayCnt_p           : in std_logic_vector(31 downto 0);
 
    -- Trigger
    --
    i_trigInSel_p             : in std_logic;
    i_trigIn0_p               : in std_logic;
    i_trigIn1_p               : in std_logic;
    o_trigOut_p               : out std_logic;
    o_trigOutDelayed_p        : out std_logic
  );
end entity ntq_trig_delay;


architecture rtl of ntq_trig_delay is

  -- synchronous reset
  --
  signal reset_s              : std_logic;
  
  -- Input trigger detection
  --
  signal trigger_s            : std_logic;
  signal triggerInD1_s        : std_logic;
  signal triggerInD2_s        : std_logic;
  signal triggerInPulse_s     : std_logic;
  
  -- Delay counter
  --  
  signal delayCntEn_s         : std_logic;
  signal v32_delayCnt_s       : std_logic_vector(31 downto 0);
  
begin

  -- Trigger input selection
  --
  trigger_s    <= i_trigIn0_p when (i_trigInSel_p = '0') else i_trigIn1_p;
  o_trigOut_p  <= trigger_s when (reset_s = '0') else '0';
    
  -- Synchronize the reset
  --
  process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      reset_s <= i_rst_p;
    end if;
  end process;
  
  -- Input trigger detection (pulse)
  --
  process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      triggerInD1_s <= trigger_s;
      triggerInD2_s <= triggerInD1_s;
    end if;
  end process;

  triggerInPulse_s <= '1' when (triggerInD1_s = '1' and triggerInD2_s = '0') else '0';

  -- Delay logic (counter)
  --
  process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if (reset_s = '1') then
         delayCntEn_s <= '0';
      elsif (triggerInPulse_s = '1') then
        -- Enable the counter
        --
        delayCntEn_s <= '1';
      elsif (v32_delayCnt_s = iv32_delayCnt_p) then
        -- The counter has reached the final value.
        --
        delayCntEn_s <= '0';
      end if;
    end if;
  end process;

  process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if (reset_s = '1') then
        v32_delayCnt_s <= (others => '0');
      elsif (v32_delayCnt_s = iv32_delayCnt_p) then
        -- The counter has reached the final value, reset it until next input trig.
        --
        v32_delayCnt_s <= (others => '0');
      elsif (delayCntEn_s = '1') then
        -- Count
        --
        v32_delayCnt_s <= v32_delayCnt_s + '1';
      end if;
    end if;
  end process;
  
  -- Output trigger
  --
  process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if (reset_s = '1') then
        o_trigOutDelayed_p <= '0';
      elsif (v32_delayCnt_s = iv32_delayCnt_p) then
        o_trigOutDelayed_p <= '1';
      else
        o_trigOutDelayed_p <= '0';
      end if;
    end if;
  end process;
  
end rtl;