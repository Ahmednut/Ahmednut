-- file: serdes_clocks.vhd
------------------------------------------------------------------------------
-- User entered comments
------------------------------------------------------------------------------
-- None
--
------------------------------------------------------------------------------
-- Output     Output      Phase    Duty Cycle   
-- Clock     Freq (MHz)  (degrees)    (%)     
------------------------------------------------------------------------------
-- CLK_OUT1   625.000      0.000      50.0      
-- CLK_OUT2   156.250      0.000      50.0      
--
------------------------------------------------------------------------------
-- Input Clock   Input Freq (MHz)   Input Jitter (UI)
------------------------------------------------------------------------------
-- primary             625            0.010

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

library unisim;
use unisim.vcomponents.all;

entity serdes_clocks is
port
 (-- Clock in ports
  CLK_IN1_P         : in     std_logic;
  CLK_IN1_N         : in     std_logic;
  -- Clock out ports
  CLK_OUT1          : out    std_logic;
  CLK_OUT2          : out    std_logic;
  -- Status and control signals
  RESET             : in     std_logic;
  LOCKED            : out    std_logic
 );
end serdes_clocks;

architecture serdes_clk_behav of serdes_clocks is
  
 signal clkin1, clkin1_s, clkin1Delqyed_s      : std_logic;
 --signal clk_div_s : std_logic; 
 --attribute KEEP : "string";
 --attribute KEEP of clk_div_s : signal is "true";
  
begin
  -------------------------------------- 
  -- Input buffering
  --------------------------------------
  clkin1_buf : IBUFDS
  port map
   (O  => clkin1_s,
    I  => CLK_IN1_P,
    IB => CLK_IN1_N);
    
  --------------------------------------
  -- IDELAY to match clk with data  
  --------------------------------------
    ADCDataIDelay_l : IDELAY
     generic map (
       IOBDELAY_TYPE => "FIXED",
       IOBDELAY_VALUE => 0)
     port map (
       O => clkin1Delqyed_s,
       I => clkin1_s,
       C => '0',
       CE => '0',
       INC => '0',
       RST => RESET
   );
  
--   clkin1Delqyed_s <= clkin1_s;
  --------------------------------------
  -- BUFIO for 1x serdes clk
  -------------------------------------- 
  BUFIO_inst : BUFIO
   port map (
   O => CLK_OUT1,
   I => clkin1Delqyed_s  
   );  
   
  --------------------------------------
  -- BUFR for div_clk = serdes_clk/4
  --------------------------------------
  BUFR_inst : BUFR
  generic map (
   BUFR_DIVIDE => "4", -- "BYPASS", "1", "2", "3", "4", "5", "6", "7", "8"
   SIM_DEVICE => "VIRTEX6") -- Specify target device, "VIRTEX4", "VIRTEX5", "VIRTEX6"
  port map
   (O => CLK_OUT2,    
    CE => '1',
    CLR => RESET,
    I => clkin1Delqyed_s);

    --CLK_OUT2 <= clk_div_s;

  
end serdes_clk_behav;
