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
generic (
  CLKIN_FREQ       : real    := 250.0;
  CLKFBOUT_MULT_F  : real    := 8.0;
  DIVCLK_DIVIDE    : integer := 2;
  CLKOUT0_DIVIDE_F : real    := 4.0
);
port (
  -- Clock in ports from external diff pins
  CLK_IN1_P         : in     std_logic;
  CLK_IN1_N         : in     std_logic;
  -- Clock out ports
  CLK_OUT1          : out    std_logic;   --Serial high speed clock out
  CLK_OUT2          : out    std_logic;   --Low speed div
  -- Clock in port from other MI125 IP core instance 
  --CLK_IN1           : in     std_logic;   --Serial high speed clock in 
  --CLK_IN2           : in     std_logic;   --Low speed div
  -- Status and control signals
  --CLK_SEL           : in     std_logic;   --Clock select
  RESET             : in     std_logic;
  LOCKED            : out    std_logic
);
end MMCMExtended_V6;

architecture xilinx of MMCMExtended_V6 is
  attribute CORE_GENERATION_INFO : string;
  attribute CORE_GENERATION_INFO of xilinx : architecture is "MMCM_V6,clk_wiz_v3_3,{component_name=MMCM_V6,use_phase_alignment=true,use_min_o_jitter=false,use_max_i_jitter=false,use_dyn_phase_shift=false,use_inclk_switchover=false,use_dyn_reconfig=false,feedback_source=FDBK_AUTO,primtype_sel=MMCM_ADV,num_out_clk=2,clkin1_period=2.000,clkin2_period=2.000,use_power_down=false,use_reset=true,use_locked=true,use_inclk_stopped=false,use_status=false,use_freeze=false,use_clk_valid=false,feedback_type=SINGLE,clock_mgr_type=MANUAL,manual_override=false}";
  
  constant SERIAL_CLOCK_PERIOD : real := 1000.0/2.0/CLKIN_FREQ;
  
  -- Input clock buffering / unused connectors
  signal clkin1             : std_logic;
  
  -- Output clock buffering / unused connectors
  signal clkfbout           : std_logic;
  signal clkfbout_buf       : std_logic;
  signal clkfboutb_unused   : std_logic;
  signal clkout0            : std_logic;
  signal clkout0b_unused    : std_logic;
  signal clkout1_unused     : std_logic;
  signal clkout1b_unused    : std_logic;
  signal clkout2_unused     : std_logic;
  signal clkout2b_unused    : std_logic;
  signal clkout3_unused     : std_logic;
  signal clkout3b_unused    : std_logic;
  signal clkout4_unused     : std_logic;
  signal clkout5_unused     : std_logic;
  signal clkout6_unused     : std_logic;
  -- Dynamic programming unused signals
  signal do_unused          : std_logic_vector(15 downto 0);
  signal drdy_unused        : std_logic;
  -- Dynamic phase shift unused signals
  signal psdone_unused      : std_logic;
  -- Unused status signals
  signal clkfbstopped_unused : std_logic;
  signal clkinstopped_unused : std_logic;
  signal clkin1_s : std_logic;
  
begin

    --------------------------------------
    -- Input buffering
    --------------------------------------
    SOURCE_SYNC_CLOCK_IN : IBUFDS
    port map
        (O  => clkin1_s,
        I  => CLK_IN1_P,
        IB => CLK_IN1_N);
    
    RX_CLK_BUFR : BUFR              
    GENERIC MAP(                   
        BUFR_DIVIDE => "2" )        
    PORT MAP (                     
        O => clkout0,        
        CE => '1',                  
        CLR => '0',                 
        I => clkin1_s); 


    --------------------------------------
    -- Clocking primitive
    --------------------------------------
    -- Instantiation of the MMCM primitive
    --    * Unused inputs are tied off
    --    * Unused outputs are labeled unused
  -- mmcm_adv_inst : MMCM_ADV
  -- generic map
   -- (BANDWIDTH            => "OPTIMIZED",
    -- CLKOUT4_CASCADE      => FALSE,
    -- CLOCK_HOLD           => FALSE,
    -- COMPENSATION         => "ZHOLD",
    -- STARTUP_WAIT         => FALSE,
    -- DIVCLK_DIVIDE        => DIVCLK_DIVIDE,
    -- CLKFBOUT_MULT_F      => CLKFBOUT_MULT_F,
    -- CLKFBOUT_PHASE       => 0.000,
    -- CLKFBOUT_USE_FINE_PS => FALSE,
    -- CLKOUT0_DIVIDE_F     => CLKOUT0_DIVIDE_F,
    -- CLKOUT0_PHASE        => 0.000,
    -- CLKOUT0_DUTY_CYCLE   => 0.500,
    -- CLKOUT0_USE_FINE_PS  => FALSE,
    -- CLKIN1_PERIOD        => SERIAL_CLOCK_PERIOD,
    -- REF_JITTER1          => 0.050)
  -- port map
    -- -- Output clocks
   -- (CLKFBOUT            => clkfbout,
    -- CLKFBOUTB           => clkfboutb_unused,
    -- CLKOUT0             => clkout0,
    -- CLKOUT0B            => clkout0b_unused,
    -- CLKOUT1             => clkout1_unused,
    -- CLKOUT1B            => clkout1b_unused,
    -- CLKOUT2             => clkout2_unused,
    -- CLKOUT2B            => clkout2b_unused,
    -- CLKOUT3             => clkout3_unused,
    -- CLKOUT3B            => clkout3b_unused,
    -- CLKOUT4             => clkout4_unused,
    -- CLKOUT5             => clkout5_unused,
    -- CLKOUT6             => clkout6_unused,
    -- -- Input clock control
    -- CLKFBIN             => clkfbout_buf,
    -- CLKIN1              => clkin1,
    -- CLKIN2              => '0',
    -- -- Tied to always select the primary input clock
    -- CLKINSEL            => '1',
    -- -- Ports for dynamic reconfiguration
    -- DADDR               => (others => '0'),
    -- DCLK                => '0',
    -- DEN                 => '0',
    -- DI                  => (others => '0'),
    -- DO                  => do_unused,
    -- DRDY                => drdy_unused,
    -- DWE                 => '0',
    -- -- Ports for dynamic phase shift
    -- PSCLK               => '0',
    -- PSEN                => '0',
    -- PSINCDEC            => '0',
    -- PSDONE              => psdone_unused,
    -- -- Other control and status signals
    -- LOCKED              => LOCKED,
    -- CLKINSTOPPED        => clkinstopped_unused,
    -- CLKFBSTOPPED        => clkfbstopped_unused,
    -- PWRDWN              => '0',
    -- RST                 => RESET);
    
    LOCKED <= '1';

    -------------------------------------
    -- Output buffering
    -------------------------------------
    -- clkf_buf : BUFG
    -- port map
        -- (O => clkfbout_buf,
        -- I => clkfbout);
        
    clkout1_buf : BUFG
    port map
        (O   => clkin1,
        I   => clkin1_s);
        
    CLK_OUT1 <= clkin1;
    
    clkout2_buf : BUFG
    port map
        (O   => CLK_OUT2,
        I   => clkout0);
                                         
end xilinx;
                                                 