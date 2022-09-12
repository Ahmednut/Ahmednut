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
-- File : calibration_detection.vhd
--------------------------------------------------------------------------------
-- Description : Detect if there are calibration error.
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: calibration_detection.vhd,v $
-- Revision 1.2  2015/05/20 13:58:39  julien.roy
-- Merge modifications from Perseus611X branch
--
-- Revision 1.2  2013/09/11 14:08:24  julien.roy
-- Change detection length from 2^12 to 2^20
--
-- Revision 1.1  2013/08/23 15:10:41  julien.roy
-- Change calibration from FPGA to software
--
-- Revision 1.1  2013/06/03 16:22:59  julien.roy
-- Add ramp detection for the iodelay calibration process
--
--
--------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;
    use ieee.std_logic_misc.all;

entity calibration_detection is
    generic(
        DATA_LENGTH : integer
        );
    port(
        i_clk_p             : in  std_logic;
        i_reset_p           : in  std_logic;
        iv_pattern_p        : in  std_logic_vector(DATA_LENGTH-1 downto 0);
        iv_data_p           : in  std_logic_vector(DATA_LENGTH-1 downto 0);
        o_error_p           : out std_logic;
        o_pattern_error_p   : out std_logic;
        o_detection_done_p  : out std_logic
    );
end calibration_detection;

architecture arch of calibration_detection is
  
  signal v_data_r1_s            : std_logic_vector(DATA_LENGTH-1 downto 0) := (others => '0');
  signal v_data_r2_s            : std_logic_vector(DATA_LENGTH-1 downto 0) := (others => '0');
  
  signal resetR1_s              : std_logic;
  signal resetR2_s              : std_logic;

  signal error_s                : std_logic;
  signal pattern_error_s        : std_logic;

  signal error_cum_s            : std_logic;
  signal pattern_error_cum_s    : std_logic;

  signal v_counter_s            : unsigned(19 downto 0) := (others => '0');

begin

    process(i_clk_p)
    begin
        if rising_edge(i_clk_p) then
        
            -- Latch input data to ease timing
            v_data_r1_s <= iv_data_p;
            v_data_r2_s <= v_data_r1_s;

            -- Verify that the data is always the same
            -- This verify the data iodelay calibration
            if v_data_r1_s /= v_data_r2_s then
                error_s <= '1';
            else
                error_s <= '0';
            end if;
            
            -- Verify that the data is the same than expected
            -- This verify the iserdes calibration
            if v_data_r2_s /= iv_pattern_p then
                pattern_error_s <= '1';
            else
                pattern_error_s <= '0';
            end if;
            
        end if;
    end process;

    -- Integrate the error during N clock cycles
    -- N := the length of the counter
    process(i_clk_p)
    begin
        if rising_edge(i_clk_p) then
        
            v_counter_s <= v_counter_s + 1;

            if v_counter_s = to_unsigned(0,v_counter_s'length) then
                o_pattern_error_p   <= pattern_error_cum_s;
                o_error_p           <= error_cum_s;
                pattern_error_cum_s <= '0';
                error_cum_s         <= '0';
                o_detection_done_p  <= '1';

            else
                error_cum_s         <= error_cum_s or error_s;
                pattern_error_cum_s <= pattern_error_cum_s or pattern_error_s;

            end if;
            
            -- Synchronous reset
            resetR1_s <= i_reset_p;
            resetR2_s <= resetR1_s;
            
            if resetR2_s = '1' then
                o_pattern_error_p   <= '1';
                o_error_p           <= '1';
                v_counter_s         <= to_unsigned(1,v_counter_s'length);
                pattern_error_cum_s <= '0';
                error_cum_s         <= '0';
                o_detection_done_p  <= '0';
            end if;

        end if;
    end process;


end arch;