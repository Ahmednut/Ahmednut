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
-- File : ntq_event_latch.vhd
--------------------------------------------------------------------------------
-- Description : Time-based trigger generator
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2016 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: ntq_timegate.vhd,v $
-- Revision 1.2  2016/07/11 17:21:19  christian.dufour-bou
-- Corrected 1 clock cycle difference between timestamp and timegate
--
-- Revision 1.1  2016/06/20 14:09:03  christian.dufour-bou
-- Added ppssync, timestamp and timegate to radio640 examples. Timegate is used to start a record at a given time using the timestamp core. This uses a new pcore (ntq_timegate_v1_00_a).
--
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use IEEE.NUMERIC_STD.all;

Library UNISIM;
use UNISIM.vcomponents.all;

library ntq_timegate_v1_00_a;
    use ntq_timegate_v1_00_a.all;

entity ntq_timegate is

    port (
        i_Rst_p             : in std_logic;
        i_CurrentTimeClk_p  : in std_logic;
        i_TimeGateClk_p     : in std_logic;

        i_CurrentTime_p     : in std_logic_vector(63 downto 0);
        i_TimeGate_p        : in std_logic_vector(63 downto 0);
        
        i_TriggerEn_p       : in std_logic;
        o_Trigger_p         : out std_logic
        );

end entity ntq_timegate;


architecture rtl of ntq_timegate is
    signal ResetR1_s                      : std_logic := '0';
    signal ResetR2_s                      : std_logic := '0'; 
    signal TriggerEnR1_s                  : std_logic := '0';
    signal TriggerEnR2_s                  : std_logic := '0';    
    
    signal TimeGate_CurrentTimeClk_s      : std_logic_vector(63 downto 0) := (others => '0');
    signal TriggerRe_s                    : std_logic := '0';

begin

    -- Latch logic
    process(i_CurrentTimeClk_p)
    begin
        if rising_edge(i_CurrentTimeClk_p) then
            ResetR1_s       <= i_Rst_p;
            ResetR2_s       <= ResetR1_s;
            
            TriggerEnR1_s       <= i_TriggerEn_p;
            TriggerEnR2_s       <= TriggerEnR1_s;
        end if;
    end process;
    
    process(i_CurrentTimeClk_p)
    begin
      if rising_edge(i_CurrentTimeClk_p) then
        if (TriggerEnR2_s = '1') then 
          if (unsigned(i_CurrentTime_p) + 1) = unsigned(TimeGate_CurrentTimeClk_s) then
            o_Trigger_p <= '1'; 
          else
            o_Trigger_p <= '0'; 
          end if;
        else
          o_Trigger_p <= '0';
        end if;
        if ResetR2_s = '1' then
          o_Trigger_p <= '0'; 
        end if;
      end if;
    end process;
    
    BusSyncer_i0: entity ntq_timegate_v1_00_a.bus_syncer
    generic map(
        C_WIDTH         => 64
    )
    port map(
      i_InClk_p 	    => i_TimeGateClk_p,
      iv_InSig_p 	    => i_TimeGate_p,
      i_OutClk_p 	    => i_CurrentTimeClk_p,
      ov_OutSig_p 	  => TimeGate_CurrentTimeClk_s
    );
    

end rtl;


