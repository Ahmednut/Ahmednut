--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:27:07 08/01/2013
-- Design Name:   
-- Module Name:   C:/projets/adp/fpga/Nutaq_ip_cores/pcores/lyt_axi_mi125_v1_00_a/Lyr_prj/mi125wrapper/calibration_detection_tb.vhd
-- Project Name:  mi125wrapper
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: calibration_detection
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY calibration_detection_tb IS
END calibration_detection_tb;
 
ARCHITECTURE behavior OF calibration_detection_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT calibration_detection
    GENERIC(
        DATA_LENGTH : integer := 8
        );
    PORT(
         i_clk_p : IN  std_logic;
         i_reset_p : IN  std_logic;
         iv_pattern_p : IN  std_logic_vector(DATA_LENGTH-1 downto 0);
         iv_data_p : IN  std_logic_vector(DATA_LENGTH-1 downto 0);
         o_error_p : OUT  std_logic;
         o_pattern_error_p : OUT  std_logic;
         o_detection_done_p : OUT  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal i_clk_p : std_logic := '0';
   signal i_reset_p : std_logic := '0';
   signal iv_pattern_p : std_logic_vector(7 downto 0) := (others => '0');
   signal iv_data_p : std_logic_vector(7 downto 0) := (others => '0');

 	--Outputs
   signal o_error_p : std_logic;
   signal o_pattern_error_p : std_logic;
   signal o_detection_done_p : std_logic;
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
   constant clock_period : time := 10 ns;
 
BEGIN

    iv_pattern_p <= X"2C";
    
    i_reset_p <= '1', '0' after 40 ns;
 
	-- Instantiate the Unit Under Test (UUT)
   uut: calibration_detection 
   GENERIC MAP (
        DATA_LENGTH => 8
        )
   PORT MAP (
          i_clk_p => i_clk_p,
          i_reset_p => i_reset_p,
          iv_pattern_p => iv_pattern_p,
          iv_data_p => iv_data_p,
          o_error_p => o_error_p,
          o_pattern_error_p => o_pattern_error_p,
          o_detection_done_p => o_detection_done_p
        );

   -- Clock process definitions
   clock_process :process
   begin
		i_clk_p <= '0';
		wait for clock_period/2;
		i_clk_p <= '1';
		wait for clock_period/2;
   end process;
 

   -- Stimulus process
   process(i_clk_p)
   begin	
        if i_reset_p = '1' then
        
        else
            iv_data_p <= X"3C";
        
        end if;
   end process;

END;
