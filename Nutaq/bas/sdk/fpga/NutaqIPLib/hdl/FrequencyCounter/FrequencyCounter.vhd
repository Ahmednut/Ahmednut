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
-- File        : $Id: FrequencyCounter.vhd,v 1.1 2015/04/10 20:11:55 sqa Exp $
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
-- Julien Roy - Initial revision 2012/12/18
-- $Log: FrequencyCounter.vhd,v $
-- Revision 1.1  2015/04/10 20:11:55  sqa
-- bas first commit
--
-- Revision 1.1  2013/01/24 17:26:08  julien.roy
-- Add FrequencyCounter source code
--
--
--------------------------------------------------------------------------------

library ieee;                                                               
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
                                                                            
entity FrequencyCounter is
  generic (
    C_REFCLK_FREQ_MHZ   : integer := 100
  );
  port (                                                                  
    i_RefClk_p     : in std_logic;
    i_TestClk_p    : in std_logic;
    i_Rst_p        : in std_logic; 
    ov16_Freq_p    : out std_logic_vector(15 downto 0)
  );                                                                      
end entity;
                                                
architecture rtl of FrequencyCounter is                                         

  ----------------------------------------------------------
  -- Signal declaration
  ----------------------------------------------------------  
  signal Trigger_s         : std_logic;
  signal Trigger_pulse_s   : std_logic;
  signal TriggerR1_s       : std_logic;
  signal TriggerR2_s       : std_logic;
  signal TriggerCnt_s      : std_logic_vector(19 downto 0);
  signal TestTrigger_s     : std_logic;
  signal TestFreqCnt_s     : std_logic_vector(25 downto 0);
  signal TestFreq_s        : std_logic_vector(25 downto 0);
  signal TestFreqOut_s     : std_logic_vector(25 downto 0);
                                                                            
begin                                                                       
  
  -- Reference clock divider
  process (i_RefClk_p) begin                                                     
    if (rising_edge(i_RefClk_p)) then                                          
      if (i_Rst_p = '1') then
        TriggerCnt_s    <= (others => '0');
        Trigger_s       <= '0';
        Trigger_pulse_s <= '0';
        
      elsif TriggerCnt_s = std_logic_vector(to_unsigned(0, TriggerCnt_s'length)) then
        TriggerCnt_s    <= std_logic_vector(to_unsigned(C_REFCLK_FREQ_MHZ * 1024, TriggerCnt_s'length));
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
  process (i_TestClk_p) begin
    if (rising_edge(i_TestClk_p)) then
      if TestTrigger_s = '1' then
        TestFreq_s    <= TestFreqCnt_s;
        TestFreqCnt_s <= (others => '0');
      else
        TestFreqCnt_s <= std_logic_vector( unsigned(TestFreqCnt_s) + 1 );
      end if;
    end if;
  end process;

  -- Test Clock Frequency clock domain crossing
  -- Delay the trigger pulse to let TestFreq_s becoming stable before latching it
  process (i_RefClk_p) 
    variable DelayLine : std_logic_vector(19 downto 0);
  begin
    if (rising_edge(i_RefClk_p)) then
      if (i_Rst_p = '1') then
        TestFreqOut_s <= (others => '0');
      elsif DelayLine(DelayLine'length-1) = '1' then
        TestFreqOut_s <= TestFreq_s;
      end if;
      
      DelayLine := DelayLine(DelayLine'length-2 downto 0) & Trigger_pulse_s;
    end if;
  end process;
  
  -- Shift the result by 10 bits (division by 1024) and round properly
  process (i_RefClk_p) begin
    if (rising_edge(i_RefClk_p)) then
      -- Round the number UP
      if TestFreqOut_s(9) = '1' then
        ov16_Freq_p <= std_logic_vector( unsigned(TestFreqOut_s(25 downto 10)) + 1 );
      -- Round the number DOWN
      else
        ov16_Freq_p <= TestFreqOut_s(25 downto 10);
      end if;
    end if;
  end process;

end architecture;                                                           
                                                                            
                                                                            