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

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

library unisim;
use unisim.vcomponents.all;

entity MMCMExtended_V6 is
port (
  i_SystemClk_p         : in std_logic;
  iv5_clkIdelayValue_p  : in std_logic_vector(4 downto 0);
  -- Clock in ports from external diff pins
  CLK_IN1_P         : in     std_logic;
  CLK_IN1_N         : in     std_logic;
  -- Clock out ports
  CLK_OUT1          : out    std_logic;   --Serial high speed clock out
  CLK_OUT2          : out    std_logic;   --Low speed div by 4 clock out 
  LOCKED            : out    std_logic
);
end MMCMExtended_V6;

architecture xilinx of MMCMExtended_V6 is  
  signal clkout0          : std_logic;
  signal clkin1_s : std_logic;
  signal clkin1_delay_s : std_logic;
  
begin

    --------------------------------------
    -- Input buffering
    -------------------------------------- 
    SOURCE_SYNC_CLOCK_IN : IBUFDS
    GENERIC MAP(              
        DIFF_TERM => TRUE,      
        IOSTANDARD => "LVDS_25")
    port map
        (O  => clkin1_s,
        I  => CLK_IN1_P,
        IB => CLK_IN1_N);
    
    -- data idelay
    IODELAY_RX_DATA : IODELAYE1
    generic map (
        IDELAY_TYPE  => "VAR_LOADABLE",
        IDELAY_VALUE => 0,
        DELAY_SRC    => "I",
        HIGH_PERFORMANCE_MODE => TRUE
    )
    port map (
        dataout => clkin1_delay_s,
        idatain => clkin1_s,
        c => i_SystemClk_p,
        ce => '1',
        inc => '1',
        datain => '0',
        odatain => '0',
        clkin => '0',
        rst => '1',
        cntvaluein => iv5_clkIdelayValue_p(4 downto 0),
        cinvctrl => '0',
        t => '1'
    );
    
    -- Design clock
    RX_CLK_BUFR : BUFR              
    GENERIC MAP(                   
        BUFR_DIVIDE => "4" )        
    PORT MAP (                     
        O => clkout0,        
        CE => '1',                  
        CLR => '0',                 
        I => clkin1_delay_s); 
    
    clkout2_buf : BUFG
    port map
        (O   => CLK_OUT2,
        I   => clkout0);
        
    -- Serial Clock
    clkout1_buf : BUFG
    port map
        (O   => CLK_OUT1,
        I   => clkin1_delay_s);

    LOCKED <= '1';
                                         
end xilinx;
                                                 