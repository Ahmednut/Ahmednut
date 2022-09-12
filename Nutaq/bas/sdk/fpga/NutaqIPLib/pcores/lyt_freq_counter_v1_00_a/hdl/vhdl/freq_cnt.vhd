--------------------------------------------------------------------------------
--
--
--          **  **     **  ******  ********  ********  ********  **    **
--         **    **   **  **   ** ********  ********  ********  **    **
--        **     *****   **   **    **     **        **        **    **
--       **       **    ******     **     ****      **        ********
--      **       **    **  **     **     **        **        **    **
--     *******  **    **   **    **     ********  ********  **    **
--    *******  **    **    **   **     ********  ********  **    **
--
--                       L Y R T E C H   R D   I N C
--
--------------------------------------------------------------------------------
-- File        : $Id: freq_cnt.vhd,v 1.1 2015/04/10 19:48:37 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Clock Frequency counter.
--               Produces 16-bit value representing the frequency of the
--               input clock in MHz. Applies correct rounding.
--
--------------------------------------------------------------------------------
-- Notes / Assumptions : Reference clock must be 100MHz.
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: freq_cnt.vhd,v $
-- Revision 1.1  2015/04/10 19:48:37  sqa
-- bas first commit
--
-- Revision 1.1  2014/01/29 16:32:14  julien.roy
-- Add freq counter core used in the radio420 BSDK exemple in the prod test
--
-- Revision 1.1  2011/08/08 19:53:59  jeffrey.johnson
-- First commit.
--
--
--------------------------------------------------------------------------------

library ieee;                                                               
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
                                                                            
entity freq_cnt is
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
                                                
architecture behavior of freq_cnt is                                         

  signal Zero_s            : std_logic_vector(31 downto 0);
  signal Trigger_s         : std_logic;
  signal TriggerR1_s       : std_logic;
  signal TriggerR2_s       : std_logic;
  signal TriggerCnt_s      : std_logic_vector(31 downto 0);
  signal TestTrigger_s     : std_logic;
  signal TestFreqCnt_s     : std_logic_vector(31 downto 0);
  signal TestFreq_s        : std_logic_vector(31 downto 0);
  signal TestFreqOut_s     : std_logic_vector(31 downto 0);
                                                                            
begin                                                                       

  Zero_s <= (others => '0');
  
  -- Reference clock divider
  process (i_RefClk_p) begin                                                     
    if (rising_edge(i_RefClk_p)) then                                          
      if (i_Rst_p = '1') then
        TriggerCnt_s <= (others => '0');
        Trigger_s <= '0';
      elsif TriggerCnt_s = Zero_s then
        TriggerCnt_s <= std_logic_vector(to_unsigned(C_REFCLK_FREQ_MHZ * 1024,32));
        Trigger_s <= not Trigger_s;
      else
        TriggerCnt_s <= std_logic_vector( unsigned(TriggerCnt_s) - 1 );
      end if;
    end if;
  end process;

  -- Trigger buffering and clock domain crossing
  process (i_TestClk_p) begin
    if (rising_edge(i_TestClk_p)) then
      if (i_Rst_p = '1') then
        TriggerR1_s <= '0';
        TriggerR2_s <= '0';
      else
        TriggerR1_s <= Trigger_s;
        TriggerR2_s <= TriggerR1_s;
      end if;
    end if;
  end process;

  -- Trigger pulse generation
  process (i_TestClk_p) begin
    if (rising_edge(i_TestClk_p)) then
      if (i_Rst_p = '1') then
        TestTrigger_s <= '0';
      elsif TriggerR1_s /= TriggerR2_s then
        TestTrigger_s <= '1';
      else
        TestTrigger_s <= '0';
      end if;
    end if;
  end process;

  -- Test clock counter
  process (i_TestClk_p) begin
    if (rising_edge(i_TestClk_p)) then
      if (i_Rst_p = '1') then
        TestFreqCnt_s <= (others => '0');
        TestFreq_s <= (others => '0');
      elsif TestTrigger_s = '1' then
        TestFreqCnt_s <= (others => '0');
        TestFreq_s <= TestFreqCnt_s;
      else
        TestFreqCnt_s <= std_logic_vector( unsigned(TestFreqCnt_s) + 1 );
      end if;
    end if;
  end process;

  -- Test Clock Frequency clock domain crossing
  process (i_RefClk_p) begin
    if (rising_edge(i_RefClk_p)) then
      if (i_Rst_p = '1') then
        TestFreqOut_s <= (others => '0');
      else
        TestFreqOut_s <= TestFreq_s;
      end if;
    end if;
  end process;

  -- Output mapping
  
  -- Shift the result by 10 bits (division by 1024) and round properly
  process (i_RefClk_p) begin
    if (rising_edge(i_RefClk_p)) then
      if (i_Rst_p = '1') then
        ov16_Freq_p <= (others => '0');
      -- Round the number UP
      elsif TestFreqOut_s(9) = '1' then
        ov16_Freq_p <= std_logic_vector( unsigned(TestFreqOut_s(25 downto 10)) + 1 );
      -- Round the number DOWN
      else
        ov16_Freq_p <= TestFreqOut_s(25 downto 10);
      end if;
    end if;
  end process;

end architecture;                                                           
                                                                            
                                                                            