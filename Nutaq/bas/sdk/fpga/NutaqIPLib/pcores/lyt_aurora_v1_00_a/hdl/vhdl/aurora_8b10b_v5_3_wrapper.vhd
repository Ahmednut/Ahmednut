-- (c) Copyright 2008 Xilinx, Inc. All rights reserved.
--
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
--
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
--
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
--
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
--
--
---------------------------------------------------------------------------------------------
--  AURORA_EXAMPLE
--
--  Aurora Generator
--
--
--
--  Description: Sample Instantiation of a 4 4-byte lane module.
--               Only tests initialization in hardware.
--
--

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.STD_LOGIC_MISC.all;
use ieee.numeric_std.all;
use WORK.AURORA_PKG.all;

-- synthesis translate_off
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
-- synthesis translate_on

entity aurora_8b10b_v5_3_wrapper is
generic
(
    AURORA_SPEED           : integer              := 0;
    DATA_WIDTH             : integer              := 16
);
port (

    -- User ports
    RXEQMIX               : in  std_logic_vector(2 downto 0);
    TXDIFFCTRL            : in  std_logic_vector(3 downto 0);
    TXPOSTEMPHASIS        : in  std_logic_vector(4 downto 0);
    TXPREEMPHASIS         : in  std_logic_vector(3 downto 0);
    DFETAP1               : in  std_logic_vector(4 downto 0);
    DFEEYEDACMON_LANE0    : out std_logic_vector(4 downto 0);
    DFEEYEDACMON_LANE1    : out std_logic_vector(4 downto 0);
    DFEEYEDACMON_LANE2    : out std_logic_vector(4 downto 0);
    DFEEYEDACMON_LANE3    : out std_logic_vector(4 downto 0);

    i_User_Clock_p                                    : IN STD_LOGIC;                                      --User clock that will clock the fifos data
    o_GTX_fifo_clk_p                                  : OUT STD_LOGIC;
    ov_RX_Data_p                                      : OUT STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);       --Data received by the Aurora link
    iv_TX_Data_p                                      : IN STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);        --Data to be sent by the Aurora link
    i_TX_Src_Rdy_n                                    : IN STD_LOGIC;
    o_TX_Dst_Rdy_n                                    : OUT STD_LOGIC;
    o_RX_Src_Rdy_n                                    : OUT STD_LOGIC;

    --AURORA GTX
    iv4_Aurora_GTX_RX_p                               : IN STD_LOGIC_VECTOR(3 downto 0);                   --Aurora GTX RX Lanes
    iv4_Aurora_GTX_RX_n                               : IN STD_LOGIC_VECTOR(3 downto 0);                   --
    ov4_Aurora_GTX_TX_p                               : OUT STD_LOGIC_VECTOR(3 downto 0);                  --Aurora GTX TX Lanes
    ov4_Aurora_GTX_TX_n                               : OUT STD_LOGIC_VECTOR(3 downto 0);                  --
    --AURORA GTX CLOCK
    i_Aurora_GTX_CLK_p                                : IN STD_LOGIC;                                      --Aurora GTX Clock Input
    --AURORA_STATUS
    o_Aurora_Hard_Error_p                             : OUT STD_LOGIC;                                     --Aurora Hardware Error Flag
    o_Aurora_Soft_Error_p                             : OUT STD_LOGIC;                                     --Aurora Soft Error Flag
    o_Aurora_Channel_Up_p                             : OUT STD_LOGIC;                                     --Aurora 4x Channel Up flag
    ov4_Aurora_Lanes_Up_p                             : OUT STD_LOGIC_VECTOR(3 downto 0);                  --Aurora Lanes Up Flag inside the Channel
    --AURORA_CTRL
    i_Aurora_Reset_p                                  : IN STD_LOGIC;                                      --Aurora Reset
    i_GT_Reset_p                                      : IN STD_LOGIC                                       --Gigabit transceiver Reset
);

end aurora_8b10b_v5_3_wrapper;

architecture MAPPED of aurora_8b10b_v5_3_wrapper is
  attribute core_generation_info           : string;
  attribute core_generation_info of MAPPED : architecture is "aurora_8b10b_v5_3,aurora_8b10b_v5_3,{user_interface=Legacy_LL, backchannel_mode=Sidebands, c_aurora_lanes=1, c_column_used=left, c_gt_clock_1=GTXQ3, c_gt_clock_2=None, c_gt_loc_1=X, c_gt_loc_10=X, c_gt_loc_11=X, c_gt_loc_12=X, c_gt_loc_13=X, c_gt_loc_14=X, c_gt_loc_15=1, c_gt_loc_16=X, c_gt_loc_17=X, c_gt_loc_18=X, c_gt_loc_19=X, c_gt_loc_2=X, c_gt_loc_20=X, c_gt_loc_21=X, c_gt_loc_22=X, c_gt_loc_23=X, c_gt_loc_24=X, c_gt_loc_25=X, c_gt_loc_26=X, c_gt_loc_27=X, c_gt_loc_28=X, c_gt_loc_29=X, c_gt_loc_3=X, c_gt_loc_30=X, c_gt_loc_31=X, c_gt_loc_32=X, c_gt_loc_33=X, c_gt_loc_34=X, c_gt_loc_35=X, c_gt_loc_36=X, c_gt_loc_37=X, c_gt_loc_38=X, c_gt_loc_39=X, c_gt_loc_4=X, c_gt_loc_40=X, c_gt_loc_41=X, c_gt_loc_42=X, c_gt_loc_43=X, c_gt_loc_44=X, c_gt_loc_45=X, c_gt_loc_46=X, c_gt_loc_47=X, c_gt_loc_48=X, c_gt_loc_5=X, c_gt_loc_6=X, c_gt_loc_7=X, c_gt_loc_8=X, c_gt_loc_9=X, c_lane_width=4, c_line_rate=3.125, c_nfc=false, c_nfc_mode=IMM, c_refclk_frequency=125.0, c_simplex=false, c_simplex_mode=TX, c_stream=true, c_ufc=false, flow_mode=None, interface_mode=Streaming, dataflow_config=Duplex}";

-- Parameter Declarations --

    constant DLY : time := 1 ns;

-- External Register Declarations --

    signal HARD_ERR_Buffer    : std_logic;
    signal SOFT_ERR_Buffer    : std_logic;
    signal LANE_UP_Buffer     : std_logic_vector(0 to 3);
    signal CHANNEL_UP_Buffer  : std_logic;

-- Wire Declarations --

-- Internal Register Declarations --

    signal gt_reset_i         : std_logic;
    signal system_reset_i     : std_logic;

    -- Error Detection Interface

    signal hard_err_i       : std_logic;
    signal soft_err_i       : std_logic;

    -- Status

    signal channel_up_i       : std_logic;
    signal lane_up_i          : std_logic_vector(0 to 3);

    -- Clock Compensation Control Interface

    signal warn_cc_i          : std_logic;
    signal do_cc_i            : std_logic;

    -- System Interface

    signal pll_not_locked_i   : std_logic;
    signal user_clk_i         : std_logic;
    signal sync_clk_i         : std_logic;
    signal reset_i            : std_logic;
    signal tx_lock_i          : std_logic;
    signal tx_out_clk_i       : std_logic;

   signal lane_up_i_i  	      : std_logic;
   signal tx_lock_i_i  	      : std_logic;
   signal lane_up_reduce_i    : std_logic;
   signal rst_cc_module_i     : std_logic;
   -- SLACK registers
   signal lane_up_r           : std_logic_vector(0 to 3);
   signal lane_up_r2          : std_logic_vector(0 to 3);

-- Component Declarations --

    component aurora_8b10b_v5_3_CLOCK_MODULE
          generic
      (
         constant MULT        : real    := 8.0;
         constant DIVIDE      : integer := 2;
         constant CLK_PERIOD  : real    := 6.4;
         constant OUT0_DIVIDE : real    := 8.0;
         constant OUT1_DIVIDE : integer := 4;
         constant OUT2_DIVIDE : integer := 8;
         constant OUT3_DIVIDE : integer := 4
      );
        port (
                GT_CLK                  : in std_logic;
                GT_CLK_LOCKED           : in std_logic;
                USER_CLK                : out std_logic;
                SYNC_CLK                : out std_logic;
                PLL_NOT_LOCKED          : out std_logic
             );
    end component;

    component aurora_8b10b_v5_3_RESET_LOGIC
        port (
                RESET                  : in std_logic;
                USER_CLK               : in std_logic;
                INIT_CLK_P             : in std_logic;
                INIT_CLK_N             : in std_logic;
                GT_RESET_IN            : in std_logic;
                TX_LOCK_IN             : in std_logic;
                PLL_NOT_LOCKED         : in std_logic;
                SYSTEM_RESET           : out std_logic;
                GT_RESET_OUT           : out std_logic
             );
    end component;

    component aurora_8b10b_v5_3
        generic(
                 --AURORA_SPEED           : string               := "3G125_125MHz";
                 AURORA_SPEED           : integer := 0;
                 SIM_GTXRESET_SPEEDUP   : integer := 1
               );
        port   (

                -- User ports
                RXEQMIX               : in  std_logic_vector(2 downto 0);
                TXDIFFCTRL            : in  std_logic_vector(3 downto 0);
                TXPOSTEMPHASIS        : in  std_logic_vector(4 downto 0);
                TXPREEMPHASIS         : in  std_logic_vector(3 downto 0);
                DFETAP1               : in  std_logic_vector(4 downto 0);
                DFEEYEDACMON_LANE0    : out std_logic_vector(4 downto 0);
                DFEEYEDACMON_LANE1    : out std_logic_vector(4 downto 0);
                DFEEYEDACMON_LANE2    : out std_logic_vector(4 downto 0);
                DFEEYEDACMON_LANE3    : out std_logic_vector(4 downto 0);

        -- LocalLink TX Interface

                TX_D             : in std_logic_vector(0 to DATA_WIDTH-1);
                TX_SRC_RDY_N     : in std_logic;

                TX_DST_RDY_N     : out std_logic;

        -- LocalLink RX Interface

                RX_D             : out std_logic_vector(0 to DATA_WIDTH-1);
                RX_SRC_RDY_N     : out std_logic;
        -- GT Serial I/O

                RXP              : in std_logic_vector(0 to 3);
                RXN              : in std_logic_vector(0 to 3);

                TXP              : out std_logic_vector(0 to 3);
                TXN              : out std_logic_vector(0 to 3);

        -- GT Reference Clock Interface

                GTXQ3    : in std_logic;
        -- Error Detection Interface

                HARD_ERR       : out std_logic;
                SOFT_ERR       : out std_logic;

        -- Status

                CHANNEL_UP       : out std_logic;
                LANE_UP          : out std_logic_vector(0 to 3);

        -- Clock Compensation Control Interface

                WARN_CC          : in std_logic;
                DO_CC            : in std_logic;

        -- System Interface

                USER_CLK         : in std_logic;
                SYNC_CLK         : in std_logic;
                GT_RESET         : in std_logic;
                RESET            : in std_logic;
                POWER_DOWN       : in std_logic;
                LOOPBACK         : in std_logic_vector(2 downto 0);
                TX_OUT_CLK       : out std_logic;
                RXEQMIX_IN           : in    std_logic_vector(2 downto 0);
                RXEQMIX_IN_LANE1           : in    std_logic_vector(2 downto 0);
                RXEQMIX_IN_LANE2           : in    std_logic_vector(2 downto 0);
                RXEQMIX_IN_LANE3           : in    std_logic_vector(2 downto 0);
    DADDR_IN       : in   std_logic_vector(7 downto 0);
    DCLK_IN        : in   std_logic;
    DEN_IN         : in   std_logic;
    DI_IN          : in   std_logic_vector(15 downto 0);
    DRDY_OUT       : out  std_logic;
    DRPDO_OUT      : out  std_logic_vector(15 downto 0);
    DWE_IN         : in   std_logic;
    DADDR_IN_LANE1       : in   std_logic_vector(7 downto 0);
    DCLK_IN_LANE1        : in   std_logic;
    DEN_IN_LANE1         : in   std_logic;
    DI_IN_LANE1          : in   std_logic_vector(15 downto 0);
    DRDY_OUT_LANE1       : out  std_logic;
    DRPDO_OUT_LANE1      : out  std_logic_vector(15 downto 0);
    DWE_IN_LANE1         : in   std_logic;
    DADDR_IN_LANE2       : in   std_logic_vector(7 downto 0);
    DCLK_IN_LANE2        : in   std_logic;
    DEN_IN_LANE2         : in   std_logic;
    DI_IN_LANE2          : in   std_logic_vector(15 downto 0);
    DRDY_OUT_LANE2       : out  std_logic;
    DRPDO_OUT_LANE2      : out  std_logic_vector(15 downto 0);
    DWE_IN_LANE2         : in   std_logic;
    DADDR_IN_LANE3       : in   std_logic_vector(7 downto 0);
    DCLK_IN_LANE3        : in   std_logic;
    DEN_IN_LANE3         : in   std_logic;
    DI_IN_LANE3          : in   std_logic_vector(15 downto 0);
    DRDY_OUT_LANE3       : out  std_logic;
    DRPDO_OUT_LANE3      : out  std_logic_vector(15 downto 0);
    DWE_IN_LANE3         : in   std_logic;
                INIT_CLK_IN      : in  std_logic;
                TX_LOCK          : out std_logic
            );

    end component;


    component aurora_8b10b_v5_3_STANDARD_CC_MODULE

        port (

        -- Clock Compensation Control Interface

                WARN_CC        : out std_logic;
                DO_CC          : out std_logic;

        -- System Interface

                PLL_NOT_LOCKED : in std_logic;
                USER_CLK       : in std_logic;
                RESET          : in std_logic

             );

    end component;

begin

  o_GTX_fifo_clk_p <= user_clk_i;

    process (user_clk_i)
    begin
      if (user_clk_i 'event and user_clk_i = '1') then
        lane_up_r    <=  lane_up_i;
        lane_up_r2   <=  lane_up_r;
      end if;
    end process;


    lane_up_reduce_i    <=  AND_REDUCE(lane_up_r2);
  rst_cc_module_i     <=  not lane_up_reduce_i;

    -- Instantiate a clock module for clock division
--Gen_AuroraSpeed3G125 : if AURORA_SPEED = "3G125_125MHz" or AURORA_SPEED = "3G125_156.25MHz" generate
Gen_AuroraSpeed3G125 : if AURORA_SPEED = 0 or AURORA_SPEED = 1 generate
    clock_module_i : aurora_8b10b_v5_3_CLOCK_MODULE
        generic map(
                        MULT        => 8.0,
                        DIVIDE      => 2,
                        CLK_PERIOD  => 6.4,
                        OUT0_DIVIDE => 8.0,
                        OUT1_DIVIDE => 4,
                        OUT2_DIVIDE => 8,
                        OUT3_DIVIDE => 4
                   )
        port map (

                    GT_CLK              => tx_out_clk_i,
                    GT_CLK_LOCKED       => tx_lock_i,
                    USER_CLK            => user_clk_i,
                    SYNC_CLK            => sync_clk_i,
                    PLL_NOT_LOCKED      => pll_not_locked_i
                 );
end generate Gen_AuroraSpeed3G125;

--Gen_AuroraSpeed5G : if AURORA_SPEED = "5G_125MHz" or AURORA_SPEED = "5G_156.25MHz" generate
Gen_AuroraSpeed5G : if AURORA_SPEED = 2 or AURORA_SPEED = 3 generate
    clock_module_i : aurora_8b10b_v5_3_CLOCK_MODULE
        generic map(
                        MULT        => 8.0,
                        DIVIDE      => 2,
                        CLK_PERIOD  => 4.0,
                        OUT0_DIVIDE => 8.0,
                        OUT1_DIVIDE => 4,
                        OUT2_DIVIDE => 8,
                        OUT3_DIVIDE => 4
                   )
        port map (

                    GT_CLK              => tx_out_clk_i,
                    GT_CLK_LOCKED       => tx_lock_i,
                    USER_CLK            => user_clk_i,
                    SYNC_CLK            => sync_clk_i,
                    PLL_NOT_LOCKED      => pll_not_locked_i
                 );
end generate Gen_AuroraSpeed5G;

    -- Register User I/O --

    -- Register User Outputs from core.

    process (user_clk_i)

    begin

        if (user_clk_i 'event and user_clk_i = '1') then

            o_Aurora_Hard_Error_p  <= hard_err_i;
            o_Aurora_Soft_Error_p  <= soft_err_i;
            ov4_Aurora_Lanes_Up_p     <= lane_up_i;
            o_Aurora_Channel_Up_p  <= channel_up_i;

        end if;

    end process;


    aurora_module_i : aurora_8b10b_v5_3
        generic map(
                    AURORA_SPEED         => AURORA_SPEED,
                    SIM_GTXRESET_SPEEDUP => 1
                   )
        port map   (

                    -- User ports
                    RXEQMIX               => RXEQMIX,
                    TXDIFFCTRL            => TXDIFFCTRL,
                    TXPOSTEMPHASIS        => TXPOSTEMPHASIS,
                    TXPREEMPHASIS         => TXPREEMPHASIS,
                    DFETAP1               => DFETAP1,
                    DFEEYEDACMON_LANE0    => DFEEYEDACMON_LANE0,
                    DFEEYEDACMON_LANE1    => DFEEYEDACMON_LANE1,
                    DFEEYEDACMON_LANE2    => DFEEYEDACMON_LANE2,
                    DFEEYEDACMON_LANE3    => DFEEYEDACMON_LANE3,

        -- LocalLink TX Interface

                    TX_D             => iv_TX_Data_p,
                    TX_SRC_RDY_N     => i_TX_Src_Rdy_n,

                    TX_DST_RDY_N     => o_TX_Dst_Rdy_n,

        -- LocalLink RX Interface

                    RX_D             => ov_RX_Data_p,
                    RX_SRC_RDY_N     => o_RX_Src_Rdy_n,

        -- GT Serial I/O

                    RXP              => iv4_Aurora_GTX_RX_p,
                    RXN              => iv4_Aurora_GTX_RX_n,
                    TXP              => ov4_Aurora_GTX_TX_p,
                    TXN              => ov4_Aurora_GTX_TX_n,

        -- GT Reference Clock Interface
                   GTXQ3             => i_Aurora_GTX_CLK_p,

        -- Error Detection Interface

                    HARD_ERR       => hard_err_i,
                    SOFT_ERR       => soft_err_i,

        -- Status

                    CHANNEL_UP       => channel_up_i,
                    LANE_UP          => lane_up_i,

        -- Clock Compensation Control Interface

                    WARN_CC          => warn_cc_i,
                    DO_CC            => do_cc_i,

        -- System Interface

                    USER_CLK         => user_clk_i,
                    SYNC_CLK         => sync_clk_i,
                    RESET            => system_reset_i,
                    POWER_DOWN       => '0',
                    LOOPBACK         => "000",
                    GT_RESET         => gt_reset_i,
                    TX_OUT_CLK       => tx_out_clk_i,
                    INIT_CLK_IN      => i_User_Clock_p,
                    RXEQMIX_IN => "111",
                    DADDR_IN   => (others=>'0'),
                    DCLK_IN    => '0',
                    DEN_IN     => '0',
                    DI_IN      => (others=>'0'),
                    DRDY_OUT   => open,
                    DRPDO_OUT  => open,
                    DWE_IN     => '0',
                    RXEQMIX_IN_LANE1 => "111",
                    DADDR_IN_LANE1   => (others=>'0'),
                    DCLK_IN_LANE1    => '0',
                    DEN_IN_LANE1     => '0',
                    DI_IN_LANE1      => (others=>'0'),
                    DRDY_OUT_LANE1   => open,
                    DRPDO_OUT_LANE1  => open,
                    DWE_IN_LANE1     => '0',
                    RXEQMIX_IN_LANE2 => "111",
                    DADDR_IN_LANE2   => (others=>'0'),
                    DCLK_IN_LANE2    => '0',
                    DEN_IN_LANE2     => '0',
                    DI_IN_LANE2      => (others=>'0'),
                    DRDY_OUT_LANE2   => open,
                    DRPDO_OUT_LANE2  => open,
                    DWE_IN_LANE2     => '0',
                    RXEQMIX_IN_LANE3 => "111",
                    DADDR_IN_LANE3   => (others=>'0'),
                    DCLK_IN_LANE3    => '0',
                    DEN_IN_LANE3     => '0',
                    DI_IN_LANE3      => (others=>'0'),
                    DRDY_OUT_LANE3   => open,
                    DRPDO_OUT_LANE3  => open,
                    DWE_IN_LANE3     => '0',
                    TX_LOCK          => tx_lock_i
                 );

    standard_cc_module_i : aurora_8b10b_v5_3_STANDARD_CC_MODULE

        port map (

        -- Clock Compensation Control Interface

                    WARN_CC        => warn_cc_i,
                    DO_CC          => do_cc_i,

        -- System Interface

                    PLL_NOT_LOCKED => pll_not_locked_i,
                    USER_CLK       => user_clk_i,
                    RESET          => rst_cc_module_i

                 );

    reset_logic_i : aurora_8b10b_v5_3_RESET_LOGIC
        port map (
                    RESET              => i_Aurora_Reset_p,
                    USER_CLK           => user_clk_i,
                    INIT_CLK_P         => i_User_Clock_p,
                    INIT_CLK_N         => '0',
                    GT_RESET_IN        => i_GT_Reset_p,
                    TX_LOCK_IN         => tx_lock_i,
                    PLL_NOT_LOCKED     => pll_not_locked_i,
                    SYSTEM_RESET       => system_reset_i,
                    GT_RESET_OUT       => gt_reset_i
                 );

end MAPPED;
