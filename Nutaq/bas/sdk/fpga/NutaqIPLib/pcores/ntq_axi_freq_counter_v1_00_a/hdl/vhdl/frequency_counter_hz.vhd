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
-- File        : $Id: frequency_counter_hz.vhd,v 1.2 2015/11/20 19:41:43 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Frequency counter 
--               Return the rounded frequency (MHz) of TestClk
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Julien Roy - Initial revision 2014/02/11
-- $Log: frequency_counter_hz.vhd,v $
-- Revision 1.2  2015/11/20 19:41:43  julien.roy
-- Modify logic to force the frequency counter to 0 after a clock update.
-- This is to make sure the frequency is 0 when no clock is present (and not the old value)
--
-- Revision 1.1  2015/10/30 14:13:23  julien.roy
-- Add clock frequency monitoring core.
--
-- Revision 1.1  2014/02/19 16:33:58  julien.roy
-- Add the first version of Nutaq ip core used in the BIST project for the Integrated Transceiver and Processor boards (DSOD1301)
--
--
--
--------------------------------------------------------------------------------

library ieee;                                                               
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
                                                                            
entity frequency_counter_hz is
  generic (
    C_REFCLK_FREQ_HZ   : integer := 100000000
  );
  port (                                                                  
    i_RefClk_p     : in std_logic;
    i_TestClk_p    : in std_logic;
    i_Rst_p        : in std_logic; 
    ov32_Freq_p    : out std_logic_vector(31 downto 0)
  );                                                                      
end entity;
                                                
architecture rtl of frequency_counter_hz is                                         

  ----------------------------------------------------------
  -- Signal declaration
  ----------------------------------------------------------  
  signal RstTestFreqCnt_s   : std_logic := '0';
  signal RstTestFreqCntR1_s : std_logic := '0';
  signal StartCounting_s    : std_logic := '0';
  
  signal Trigger_s         : std_logic := '0';
  signal Trigger_pulse_s   : std_logic := '0';
  signal TriggerR1_s       : std_logic := '0';
  signal TriggerR2_s       : std_logic := '0';
  signal TriggerCnt_s      : std_logic_vector(31 downto 0) := (others => '0');
  signal TestTrigger_s     : std_logic := '0';
  signal TestFreqCnt_s     : std_logic_vector(31 downto 0) := (others => '0');
  signal TestFreq_s        : std_logic_vector(31 downto 0) := (others => '0');
  signal TestFreqOut_s     : std_logic_vector(31 downto 0) := (others => '0');
                                                                            
begin                                                                       
  
  -- Reference clock divider
  process (i_RefClk_p) begin                                                     
    if (rising_edge(i_RefClk_p)) then                                          
      if (i_Rst_p = '1') then
        TriggerCnt_s    <= std_logic_vector(to_unsigned(C_REFCLK_FREQ_HZ - 1, TriggerCnt_s'length));
        Trigger_s       <= '0';
        Trigger_pulse_s <= '0';
        
      elsif RstTestFreqCntR1_s = '1' then
        TriggerCnt_s    <= std_logic_vector(to_unsigned(C_REFCLK_FREQ_HZ - 1, TriggerCnt_s'length));
        Trigger_s       <= not Trigger_s;
        
      elsif TriggerCnt_s = std_logic_vector(to_unsigned(0, TriggerCnt_s'length)) then
        TriggerCnt_s    <= std_logic_vector(to_unsigned(C_REFCLK_FREQ_HZ - 1, TriggerCnt_s'length));
        Trigger_s       <= not Trigger_s;
        Trigger_pulse_s <= '1';
        
      else
        TriggerCnt_s    <= std_logic_vector( unsigned(TriggerCnt_s) - 1 );
        Trigger_pulse_s <= '0';
        
      end if;
    end if;
  end process;

  -- Trigger buffering and clock domain crossing
  process (i_TestClk_p) begin
    if (rising_edge(i_TestClk_p)) then
      TriggerR1_s <= Trigger_s;
      TriggerR2_s <= TriggerR1_s;
    end if;
  end process;

  -- Trigger pulse generation
  process (i_TestClk_p) begin
    if (rising_edge(i_TestClk_p)) then
      if TriggerR1_s /= TriggerR2_s then
        TestTrigger_s <= '1';
      else
        TestTrigger_s <= '0';
      end if;
    end if;
  end process;

  -- Test clock counter
  process (i_TestClk_p, RstTestFreqCntR1_s, i_Rst_p) begin
    if (i_Rst_p = '1' or RstTestFreqCntR1_s = '1') then
        TestFreq_s      <= (others => '0');
        TestFreqCnt_s   <= (others => '0');
        StartCounting_s <= '0';
    
    elsif (rising_edge(i_TestClk_p)) then
      if TestTrigger_s = '1' then
        StartCounting_s <= '1';
        TestFreq_s      <= TestFreqCnt_s;
        TestFreqCnt_s   <= std_logic_vector(to_unsigned(1, TestFreqCnt_s'length));
        
      elsif StartCounting_s = '1' then
        TestFreqCnt_s <= std_logic_vector( unsigned(TestFreqCnt_s) + 1 );
        
      end if;
    end if;
  end process;

  -- Test Clock Frequency clock domain crossing
  -- Delay the trigger pulse to let TestFreq_s becoming stable before latching it
  process (i_RefClk_p) 
    variable u10_DelayCnt_s  : unsigned(9 downto 0) := (others => '0');
    constant u10_MaxCnt_c    : unsigned(9 downto 0) := (others => '1');
    variable UpdateFreqOut_s : std_logic := '0';
  begin
    if (rising_edge(i_RefClk_p)) then
      RstTestFreqCntR1_s    <= RstTestFreqCnt_s;
      RstTestFreqCnt_s      <= '0';
        
      if (i_Rst_p = '1') then
        TestFreqOut_s <= (others => '0');
      elsif UpdateFreqOut_s = '1' then
        TestFreqOut_s <= TestFreq_s;
        RstTestFreqCnt_s <= '1';
      end if;
      
      -- Default value
      UpdateFreqOut_s := '0';
      if Trigger_pulse_s = '1' then
        u10_DelayCnt_s := (others => '0');
        
      elsif u10_DelayCnt_s /= u10_MaxCnt_c then
        if(u10_DelayCnt_s = (u10_MaxCnt_c - 1)) then
          UpdateFreqOut_s := '1';
        end if;
        
        u10_DelayCnt_s := u10_DelayCnt_s + 1;
        
      end if;
    end if;
  end process;
  
  ov32_Freq_p <= TestFreqOut_s;

end architecture;                                                           
                                                                            
                                                                            