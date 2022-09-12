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
-- File        : $Id: datarate_cnt.vhd,v 1.1 2015/05/20 13:56:58 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Clock Frequency counter
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: datarate_cnt.vhd,v $
-- Revision 1.1  2015/05/20 13:56:58  julien.roy
-- Update MSI pin address to match the Perseus611X branch merge.
--
-- Revision 1.3  2015/01/13 14:16:31  julien.roy
-- Latch the ready signal high.
--
-- Revision 1.2  2014/09/16 13:46:08  julien.roy
-- Change clock architecture to solve DAC FIFO warning errors.
-- Add new frame mode.
-- Add more register stage to ease timing.
--
-- Revision 1.1  2014/06/18 14:43:05  julien.roy
-- Add first version of the mo1000 core
--
-- Revision 1.3  2013/08/27 20:44:44  julien.roy
-- Add one flipflop for TestTrigger_s to ease timing
--
-- Revision 1.2  2012/12/10 14:30:37  julien.roy
-- Modify ucf to support the 4 FPGA types
-- Add chip enable status ports
-- Add variable delay trigger
-- Move frequency counter status into core registers
--
-- Revision 1.1  2012/10/16 13:17:50  khalid.bensadek
-- First commit of a working version of this project
--
-- Revision 1.4  2012/02/01 14:36:32  khalid.bensadek
-- Update du projet
--
-- Revision 1.2  2011/05/25 16:12:30  jeffrey.johnson
-- Moved MMCM from wrapper to clk_module.
-- Added freq counters for all clocks.
--
-- Revision 1.1  2011/05/19 18:27:53  jeffrey.johnson
-- Added ADC interface.
--
--------------------------------------------------------------------------------

library ieee;                                                               
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
                                                                            
entity datarate_cnt is
  generic (
    C_REFCLK_FREQ_MHZ   : integer := 100;
    C_BYTE_PER_EN       : integer := 16
  );
  port (                                                                  
    i_RefClk_p          : in std_logic;
    i_Rst_p             : in std_logic; 
    i_Clk_p             : in std_logic;
    i_DataEn_p          : in std_logic;
    
    ov32_DataRate_p     : out std_logic_vector(31 downto 0);
    o_Rdy_p             : out std_logic 
  );                                                                      
end entity;
                                                
architecture behavior of datarate_cnt is                                         

  signal Zero_s            : std_logic_vector(31 downto 0);
  signal TriggerCnt_s      : std_logic_vector(31 downto 0);
  
  signal Trigger_s         : std_logic;
  signal TriggerR1_s       : std_logic;
  signal TriggerR2_s       : std_logic;
  signal TestTrigger_s     : std_logic;
  signal TestTrigger_r1_s  : std_logic;
  
  signal TestFreqCnt_s     : std_logic_vector(31 downto 0);
  signal TestFreq_s        : std_logic_vector(31 downto 0);
  signal TestFreqOut_s     : std_logic_vector(31 downto 0);
  
  signal v_rdy_s           : std_logic_vector(9 downto 0);
  
  signal rst_r1_s          : std_logic;
                                                                            
begin                                                                       

  Zero_s <= (others => '0');
  
  -- Reference clock divider
  process (i_RefClk_p) begin                                                     
    if (rising_edge(i_RefClk_p)) then    
    
      rst_r1_s <= i_Rst_p;
    
      if (i_Rst_p = '1') then
        TriggerCnt_s <= (others => '0');
        Trigger_s <= '0';
      elsif TriggerCnt_s = Zero_s then
        TriggerCnt_s <= std_logic_vector(to_unsigned(C_REFCLK_FREQ_MHZ * 1000 * 1000 - 1, 32));
        if rst_r1_s = '0' then
            Trigger_s <= not Trigger_s;
        end if;
      else
        TriggerCnt_s <= std_logic_vector( unsigned(TriggerCnt_s) - 1 );
      end if;
    end if;
  end process;

  -- Trigger buffering and clock domain crossing
  process (i_Clk_p, i_Rst_p) begin
    if (i_Rst_p = '1') then
        TriggerR1_s <= '0';
        TriggerR2_s <= '0';
    elsif (rising_edge(i_Clk_p)) then
        TriggerR1_s <= Trigger_s;
        TriggerR2_s <= TriggerR1_s;
    end if;
  end process;

  -- Trigger pulse generation
  process (i_Clk_p, i_Rst_p) begin
    if (i_Rst_p = '1') then
        TestTrigger_s <= '0';
        TestTrigger_r1_s <= '0';
    elsif (rising_edge(i_Clk_p)) then
      if TriggerR1_s /= TriggerR2_s then
        TestTrigger_s <= '1';
      else
        TestTrigger_s <= '0';
      end if;
      
      TestTrigger_r1_s  <= TestTrigger_s;
    end if;
  end process;

  -- Test clock counter
  process (i_Clk_p, i_Rst_p) begin
    if (i_Rst_p = '1') then
        TestFreqCnt_s <= (others => '0');
        TestFreq_s <= (others => '0');
    elsif (rising_edge(i_Clk_p)) then
      if TestTrigger_r1_s = '1' then
      
        if (i_DataEn_p = '1') then
            TestFreqCnt_s <= std_logic_vector(to_unsigned(C_BYTE_PER_EN,TestFreqCnt_s'length));
        else
            TestFreqCnt_s <= (others => '0');
        end if;
        
        TestFreq_s <= TestFreqCnt_s;
      elsif (i_DataEn_p = '1') then
        TestFreqCnt_s <= std_logic_vector( unsigned(TestFreqCnt_s) + C_BYTE_PER_EN );
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
        ov32_DataRate_p <= (others => '0');
        o_Rdy_p <= '0';
        
      else
        
        -- delay line for ready signal
        v_rdy_s(0) <= Trigger_s;
        v_rdy_s(v_rdy_s'high downto 1) <= v_rdy_s(v_rdy_s'high-1 downto 0);
      
        
        if v_rdy_s(v_rdy_s'high) /= v_rdy_s(v_rdy_s'high-1) then
            o_Rdy_p <= '1';
            ov32_DataRate_p <= TestFreqOut_s;
        end if;
        
      end if;
    end if;
  end process;

end architecture;                                                           
                                                                            
                                                                            