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
-- File        : $Id: low_high_latch.vhd,v 1.1 2015/10/20 19:14:44 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Latch to hold the lowest and highest value of each bit
--               since the last update pulse.
--------------------------------------------------------------------------------
-- Copyright (c) 2015 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: low_high_latch.vhd,v $
-- Revision 1.1  2015/10/20 19:14:44  julien.roy
-- Add first revision of Radio640 fpga pcore.
--
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library ntq_axi_radio640_v1_00_a;
use ntq_axi_radio640_v1_00_a.all;

entity low_high_latch is
    generic (
        C_WIDTH             : integer := 8
    );
    port  (
        i_Rst_p             : in     std_logic;
    
        i_UpdateClk_p       : in     std_logic;
        i_UpdatePulse_p     : in     std_logic;
        
        i_Clk_p             : in     std_logic;
        iv_Data_p           : in     std_logic_vector(C_WIDTH-1 downto 0);
        ov_LowData_p        : out    std_logic_vector(C_WIDTH-1 downto 0);
        ov_HighData_p       : out    std_logic_vector(C_WIDTH-1 downto 0)
    );
end entity low_high_latch;


architecture rtl of low_high_latch is
    
    signal v_LowData_s      : std_logic_vector(C_WIDTH-1 downto 0) := (others => '1');
    signal v_HighData_s     : std_logic_vector(C_WIDTH-1 downto 0) := (others => '0');
    
    signal updatePulse_s    : std_logic := '0';

begin

    -- Pulse clock domain crossing
    syncer_i: entity ntq_axi_radio640_v1_00_a.syncer
    port map(
        i_InClk_p 	=> i_UpdateClk_p,
        i_InSig_p 	=> i_UpdatePulse_p,
        i_OutClk_p 	=> i_Clk_p,
        o_OutSig_p 	=> updatePulse_s
    );

    process (i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
        
            -- Keep lowest and highest values
            v_LowData_s     <= v_LowData_s and iv_Data_p;
            v_HighData_s    <= v_HighData_s or iv_Data_p;
            
            if updatePulse_s = '1' then
                -- Update output values
                ov_LowData_p    <= v_LowData_s;
                ov_HighData_p   <= v_HighData_s;
                
                -- Reset internal latch
                v_LowData_s     <= iv_Data_p;
                v_HighData_s    <= iv_Data_p;
            end if;
            
            -- Synchronous reset
            if i_Rst_p = '1' then
            
                -- Set default value
                v_LowData_s     <= (others => '1');
                v_HighData_s    <= (others => '0');
                ov_LowData_p    <= (others => '1');
                ov_HighData_p   <= (others => '0');
                
            end if;
        end if;
    end process;

end architecture rtl;
