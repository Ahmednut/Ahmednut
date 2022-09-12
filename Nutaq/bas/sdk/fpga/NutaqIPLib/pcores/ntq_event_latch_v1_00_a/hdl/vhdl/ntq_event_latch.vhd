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
-- Description : Core to latch a value at a trigger event
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: ntq_event_latch.vhd,v $
-- Revision 1.1  2015/04/10 19:49:02  sqa
-- bas first commit
--
-- Revision 1.1  2015/02/02 14:40:15  julien.roy
-- Add first version of the event latch core
--
-- Revision 1.1  2013/05/09 12:37:59  julien.roy
-- Add uart switch pcore to support both UART pins
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

Library UNISIM;
use UNISIM.vcomponents.all;

entity ntq_event_latch is

    generic(
        C_DATA_WIDTH    : integer range 1 to 256 := 32);
    port (
        i_Rst_p             : in std_logic;
        i_Clk_p             : in std_logic;
        i_ClkEn_p           : in std_logic;

        i_ArmLatch_p        : in std_logic;
        i_Trigger_p         : in std_logic;
        iv_Data_p           : in std_logic_vector(C_DATA_WIDTH-1 downto 0);
        i_DataWen_p         : in std_logic;

        ov_LatchData_p      : out std_logic_vector(C_DATA_WIDTH-1 downto 0);
        o_TriggerHappened_p : out std_logic);

end entity ntq_event_latch;


architecture rtl of ntq_event_latch is

    constant FSM_IDLE                   : std_logic_vector(1 downto 0)  := "00";
    constant FSM_WAIT_FOR_TRIGGER_RE    : std_logic_vector(1 downto 0)  := "01";
    constant FSM_WAIT_FOR_DATA_WE       : std_logic_vector(1 downto 0)  := "10";

    signal v2_Fsm_s         : std_logic_vector(1 downto 0) := FSM_IDLE;
    
    signal ArmLatchR1_s     : std_logic := '0';
    signal ArmLatchR2_s     : std_logic := '0';
    signal ArmLatchRe_s     : std_logic := '0';
    
    signal ResetR1_s        : std_logic := '0';
    signal ResetR2_s        : std_logic := '0';
    
    signal TriggerR1_s      : std_logic := '0';
    signal TriggerRe_s      : std_logic := '0';

begin

    -- Clock domain crossing
    --
    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then

            ArmLatchR1_s    <= i_ArmLatch_p;
            ArmLatchR2_s    <= ArmLatchR1_s;
            ArmLatchRe_s    <= ArmLatchR1_s and (not ArmLatchR2_s);
            
            ResetR1_s       <= i_Rst_p;
            ResetR2_s       <= ResetR1_s;
            
            TriggerR1_s     <= i_Trigger_p;

        end if;
    end process;
    
    TriggerRe_s <= i_Trigger_p and (not TriggerR1_s);

    -- Latch logic
    --
    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then

            case v2_Fsm_s is

                when FSM_IDLE =>
           
                    -- On Arm signal rising edge, change state to monitor the trigger event
                    if ArmLatchRe_s = '1' then
                        v2_Fsm_s            <= "01";
                        o_TriggerHappened_p <= '0';
                    end if;
                
                when FSM_WAIT_FOR_TRIGGER_RE =>
                
                    -- If armed and the trigger event happened,
                    -- latch the input data and disarm the latch.
                    -- The trigger is high-level sensitive.
                    if TriggerRe_s = '1' and i_ClkEn_p = '1' then
                    
                        if i_DataWen_p = '1' then
                            v2_Fsm_s            <= FSM_IDLE;
                            ov_LatchData_p      <= iv_Data_p;
                            o_TriggerHappened_p <= '1';
                            
                        else
                            v2_Fsm_s    <= FSM_WAIT_FOR_DATA_WE;
                        end if;
                        
                    end if;
                
                when FSM_WAIT_FOR_DATA_WE =>
                
                    if i_DataWen_p = '1' and i_ClkEn_p = '1' then
                        v2_Fsm_s            <= FSM_IDLE;
                        ov_LatchData_p      <= iv_Data_p;
                        o_TriggerHappened_p <= '1';
                    end if;

                when others =>
                    v2_Fsm_s            <= FSM_IDLE;
                
            end case;

            -- Reset
            if ResetR2_s = '1' then
                v2_Fsm_s            <= FSM_IDLE;
                o_TriggerHappened_p <= '0';
                ov_LatchData_p      <= (others => '0');
            end if;
            
        end if;
    end process;

end rtl;


