------------------------------------------------------------------------------
-- Filename:       axi_perseus6010_regs.vhd
-- Version:        v1_00_a
-- Description:    Top level design, instantiates library components and user logic.
-- Generated by:   julien.roy
-- Date:           2015-05-19 16:41:12
-- Generated:      using LyrtechRD REGGENUTIL based on Xilinx IPIF Wizard.
-- VHDL Standard:  VHDL'93
------------------------------------------------------------------------------
-- Copyright (c) 2001-2012 LYRtech RD Inc.  All rights reserved.
------------------------------------------------------------------------------

 library ieee;
 use ieee.std_logic_1164.all;
 use ieee.std_logic_arith.all;
 use ieee.std_logic_unsigned.all;

 library proc_common_v3_00_a;
 use proc_common_v3_00_a.proc_common_pkg.all;
 use proc_common_v3_00_a.ipif_pkg.all;

 library axi_lite_ipif_v1_01_a;
 use axi_lite_ipif_v1_01_a.axi_lite_ipif;

 library lyt_axi_perseus6010_regs_v1_00_a;
 use lyt_axi_perseus6010_regs_v1_00_a.user_logic;


-------------------------------------------------------------------------------
-- Entity section
-------------------------------------------------------------------------------
-- Definition of Generics:
--   C_S_AXI_DATA_WIDTH           -- AXI4LITE slave: Data width
--   C_S_AXI_ADDR_WIDTH           -- AXI4LITE slave: Address Width
--   C_S_AXI_MIN_SIZE             -- AXI4LITE slave: Min Size
--   C_USE_WSTRB                  -- AXI4LITE slave: Write Strobe
--   C_DPHASE_TIMEOUT             -- AXI4LITE slave: Data Phase Timeout
--   C_BASEADDR                   -- AXI4LITE slave: base address
--   C_HIGHADDR                   -- AXI4LITE slave: high address
--   C_FAMILY                     -- FPGA Family
--
-- Definition of Ports:
--   S_AXI_ACLK                   -- AXI4LITE slave: Clock
--   S_AXI_ARESETN                -- AXI4LITE slave: Reset
--   S_AXI_AWADDR                 -- AXI4LITE slave: Write address
--   S_AXI_AWVALID                -- AXI4LITE slave: Write address valid
--   S_AXI_WDATA                  -- AXI4LITE slave: Write data
--   S_AXI_WSTRB                  -- AXI4LITE slave: Write strobe
--   S_AXI_WVALID                 -- AXI4LITE slave: Write data valid
--   S_AXI_BREADY                 -- AXI4LITE slave: Response ready
--   S_AXI_ARADDR                 -- AXI4LITE slave: Read address
--   S_AXI_ARVALID                -- AXI4LITE slave: Read address valid
--   S_AXI_RREADY                 -- AXI4LITE slave: Read data ready
--   S_AXI_ARREADY                -- AXI4LITE slave: read addres ready
--   S_AXI_RDATA                  -- AXI4LITE slave: Read data
--   S_AXI_RRESP                  -- AXI4LITE slave: Read data response
--   S_AXI_RVALID                 -- AXI4LITE slave: Read data valid
--   S_AXI_WREADY                 -- AXI4LITE slave: Write data ready
--   S_AXI_BRESP                  -- AXI4LITE slave: Response
--   S_AXI_BVALID                 -- AXI4LITE slave: Resonse valid
--   S_AXI_AWREADY                -- AXI4LITE slave: Wrte address ready
------------------------------------------------------------------------------
entity lyt_axi_perseus6010_regs is
generic
(
  -- ADD USER GENERICS BELOW THIS LINE ---------------
  --USER generics added here
  -- ADD USER GENERICS ABOVE THIS LINE ---------------

  -- DO NOT EDIT BELOW THIS LINE ---------------------
  -- Bus protocol parameters, do not add to or delete
      C_S_AXI_DATA_WIDTH             : integer              := 32;
      C_S_AXI_ADDR_WIDTH             : integer              := 32;
      C_S_AXI_MIN_SIZE               : std_logic_vector     := X"000001FF";
      C_USE_WSTRB                    : integer              := 0;
      C_DPHASE_TIMEOUT               : integer              := 8;
      C_BASEADDR                     : std_logic_vector     := X"FFFFFFFF";
      C_HIGHADDR                     : std_logic_vector     := X"00000000";
      C_FAMILY                       : string               := "virtex6"
  -- DO NOT EDIT ABOVE THIS LINE ---------------------
);
port
(
  -- User ports
  i_daughter_absent_p                       : in std_logic;
  i_fmc1_absent_p                       : in std_logic;
  o_pcie_msi_p                       : out std_logic;
  ov2_uart_mode_p                       : out std_logic_vector(1 downto 0);
  i_fmc1_stack_absent_p                       : in std_logic;
  i_fmc1_clkdir_p                       : in std_logic;
  i_ddr3_init_done_p                       : in std_logic;
  i_pcie_present_p                       : in std_logic;
  i_pcie_clock_lock_p                       : in std_logic;
  o_CtrlTclkdTxEn_p                       : out std_logic;
  o_CtrlTclkdRxDis_p                       : out std_logic;
  o_CtrlAmctclkc2Fmcclk3En_p                       : out std_logic;
  o_CtrlAmctclka2Fmcclk2En_p                       : out std_logic;
  o_CtrlFclkaHighz_p                       : out std_logic;
  o_Ctrl100mhzOutEn_p                       : out std_logic;
  o_CtrlGatedClkEn_p                       : out std_logic;
  o_CtrlTclkcTxEn_p                       : out std_logic;
  o_CtrlTclkcRxDis_p                       : out std_logic;
  o_CtrlTclkbTxEn_p                       : out std_logic;
  o_CtrlTclkbRxDis_p                       : out std_logic;
  o_CtrlTclkaTxEn_p                       : out std_logic;
  o_CtrlTclkaRxDis_p                       : out std_logic;
  o_CtrlFmcclk12AmctclkdEn_p                       : out std_logic;
  o_CtrlFmcclk02AmctclkbEn_p                       : out std_logic;
  ov2_CtrlVadjSel_p                       : out std_logic_vector(1 downto 0);
  o_CtrlVadjEn_p                       : out std_logic;
  o_nFpgaProg_p                       : out std_logic;
  ov8_nCtrlLedGrn_p                       : out std_logic_vector(7 downto 0);
  o_CtrlLedBufOd_p                       : out std_logic;
  ov8_nCtrlLedRed_p                       : out std_logic_vector(7 downto 0);
  o_MmcI2cReleaseReq_p                       : out std_logic;
  i_MmcI2cReleaseAck_p                       : in std_logic;
  iv32_CustomReg0_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg0_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg1_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg1_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg2_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg2_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg3_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg3_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg4_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg4_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg5_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg5_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg6_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg6_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg7_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg7_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg8_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg8_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg9_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg9_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg10_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg10_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg11_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg11_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg12_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg12_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg13_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg13_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg14_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg14_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg15_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg15_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg16_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg16_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg17_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg17_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg18_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg18_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg19_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg19_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg20_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg20_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg21_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg21_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg22_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg22_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg23_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg23_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg24_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg24_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg25_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg25_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg26_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg26_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg27_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg27_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg28_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg28_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg29_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg29_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg30_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg30_p                       : out std_logic_vector(31 downto 0);
  iv32_CustomReg31_p                       : in std_logic_vector(31 downto 0);
  ov32_CustomReg31_p                       : out std_logic_vector(31 downto 0);
  ov16_Aurora0_Idx_p                       : out std_logic_vector(15 downto 0);
  o_Aurora0_WrEn_p                       : out std_logic;
  iv32_Aurora0_Data_p                       : in std_logic_vector(31 downto 0);
  ov32_Aurora0_Data_p                       : out std_logic_vector(31 downto 0);
  ov16_Aurora1_Idx_p                       : out std_logic_vector(15 downto 0);
  o_Aurora1_WrEn_p                       : out std_logic;
  iv32_Aurora1_Data_p                       : in std_logic_vector(31 downto 0);
  ov32_Aurora1_Data_p                       : out std_logic_vector(31 downto 0);
  ov16_Aurora2_Idx_p                       : out std_logic_vector(15 downto 0);
  o_Aurora2_WrEn_p                       : out std_logic;
  iv32_Aurora2_Data_p                       : in std_logic_vector(31 downto 0);
  ov32_Aurora2_Data_p                       : out std_logic_vector(31 downto 0);
  iv32_PPSSYNC_CoreIdVers_p                       : in std_logic_vector(31 downto 0);
  ov32_PPSSYNC_ClkTarget_p                       : out std_logic_vector(31 downto 0);
  o_PPSSYNC_CoreReset_p                       : out std_logic;
  o_PPSSYNC_RstUponRead_p                       : out std_logic;
  o_PPSSYNC_RstAcc_p                       : out std_logic;
  o_PPSSYNC_CoreEnable_p                       : out std_logic;
  o_PPSSYNC_MovSumEn_p                       : out std_logic;
  o_PPSSYNC_MovSumRst_p                       : out std_logic;
  iv32_PPSSYNC_PpsCnt_p                       : in std_logic_vector(31 downto 0);
  iv32_PPSSYNC_DiffAcc_p                       : in std_logic_vector(31 downto 0);
  o_PPSSYNC_DiffAccRead_p                       : out std_logic;
  ov32_PPSSYNC_ProgDelay_p                       : out std_logic_vector(31 downto 0);
  ov32_PPSSYNC_PpsFreq_p                       : out std_logic_vector(31 downto 0);
  iv32_lvds0_coreid_p                       : in std_logic_vector(31 downto 0);
  ov32_lvds0_ctrl_p                       : out std_logic_vector(31 downto 0);
  iv32_lvds0_value_p                       : in std_logic_vector(31 downto 0);
  ov32_lvds0_value_p                       : out std_logic_vector(31 downto 0);
  ov32_lvds0_oe_p                       : out std_logic_vector(31 downto 0);
  iv32_lvds1_coreid_p                       : in std_logic_vector(31 downto 0);
  ov32_lvds1_ctrl_p                       : out std_logic_vector(31 downto 0);
  iv32_lvds1_value_p                       : in std_logic_vector(31 downto 0);
  ov32_lvds1_value_p                       : out std_logic_vector(31 downto 0);
  ov32_lvds1_oe_p                       : out std_logic_vector(31 downto 0);
  iv32_lvds2_coreid_p                       : in std_logic_vector(31 downto 0);
  ov32_lvds2_ctrl_p                       : out std_logic_vector(31 downto 0);
  iv32_lvds2_value_p                       : in std_logic_vector(31 downto 0);
  ov32_lvds2_value_p                       : out std_logic_vector(31 downto 0);
  ov32_lvds2_oe_p                       : out std_logic_vector(31 downto 0);
  iv32_lvds3_coreid_p                       : in std_logic_vector(31 downto 0);
  ov32_lvds3_ctrl_p                       : out std_logic_vector(31 downto 0);
  iv32_lvds3_value_p                       : in std_logic_vector(31 downto 0);
  ov32_lvds3_value_p                       : out std_logic_vector(31 downto 0);
  ov32_lvds3_oe_p                       : out std_logic_vector(31 downto 0);
  iv32_RTDExSyncCoreId_p                       : in std_logic_vector(31 downto 0);
  o_RTDExSyncCoreReset_p                       : out std_logic;
  ov8_RtdexSyncAddr_p                       : out std_logic_vector(7 downto 0);
  o_RTDExSyncConfigWrEn_p                       : out std_logic;
  ov32_RtdexSyncWrReg_p                       : out std_logic_vector(31 downto 0);
  iv32_RtdexSyncRdReg_p                       : in std_logic_vector(31 downto 0);
  iv32_Timestamp_CoreIdVers_p                       : in std_logic_vector(31 downto 0);
  o_Timestamp_CoreReset_p                       : out std_logic;
  o_Timestamp_SetTime_p                       : out std_logic;
  o_Timestamp_LatchTime_p                       : out std_logic;
  iv64_Timestamp_p                       : in std_logic_vector(63 downto 0);
  ov64_Timestamp_p                       : out std_logic_vector(63 downto 0);
  -- DO NOT EDIT BELOW THIS LINE ---------------------
  -- Bus protocol ports, do not add to or delete
      S_AXI_ACLK                     : in  std_logic;
      S_AXI_ARESETN                  : in  std_logic;
      S_AXI_AWADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
      S_AXI_AWVALID                  : in  std_logic;
      S_AXI_WDATA                    : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
      S_AXI_WSTRB                    : in  std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
      S_AXI_WVALID                   : in  std_logic;
      S_AXI_BREADY                   : in  std_logic;
      S_AXI_ARADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
      S_AXI_ARVALID                  : in  std_logic;
      S_AXI_RREADY                   : in  std_logic;
      S_AXI_ARREADY                  : out std_logic;
      S_AXI_RDATA                    : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
      S_AXI_RRESP                    : out std_logic_vector(1 downto 0);
      S_AXI_RVALID                   : out std_logic;
      S_AXI_WREADY                   : out std_logic;
      S_AXI_BRESP                    : out std_logic_vector(1 downto 0);
      S_AXI_BVALID                   : out std_logic;
      S_AXI_AWREADY                  : out std_logic
  -- DO NOT EDIT ABOVE THIS LINE ---------------------
);

  attribute MAX_FANOUT                     : string;
  attribute SIGIS                          : string;
  attribute MAX_FANOUT of S_AXI_ACLK       : signal is "10000";
  attribute MAX_FANOUT of S_AXI_ARESETN    : signal is "10000";
  attribute SIGIS of S_AXI_ACLK            : signal is "Clk";
  attribute SIGIS of S_AXI_ARESETN         : signal is "Rst";

end entity lyt_axi_perseus6010_regs;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of lyt_axi_perseus6010_regs is

  constant USER_SLV_DWIDTH                : integer              := C_S_AXI_DATA_WIDTH;

  constant IPIF_SLV_DWIDTH                : integer              := C_S_AXI_DATA_WIDTH;

  constant ZERO_ADDR_PAD                  : std_logic_vector(0 to 31) := (others => '0');
  constant USER_SLV_BASEADDR              : std_logic_vector     := C_BASEADDR;
  constant USER_SLV_HIGHADDR              : std_logic_vector     := C_HIGHADDR;

  constant IPIF_ARD_ADDR_RANGE_ARRAY      : SLV64_ARRAY_TYPE     := 
    (
      ZERO_ADDR_PAD & USER_SLV_BASEADDR,  -- user logic slave space base address
      ZERO_ADDR_PAD & USER_SLV_HIGHADDR   -- user logic slave space high address
    );

  constant USER_SLV_NUM_REG               : integer              := 75;
  constant USER_NUM_REG                   : integer              := USER_SLV_NUM_REG;
  constant TOTAL_IPIF_CE                  : integer              := USER_NUM_REG;

  constant IPIF_ARD_NUM_CE_ARRAY          : INTEGER_ARRAY_TYPE   :=
    (
      0  => (USER_SLV_NUM_REG)  -- number of ce for user logic slave space
    );

  ------------------------------------------
  -- Index for CS/CE
  ------------------------------------------
  constant USER_SLV_CS_INDEX              : integer              := 0;
  constant USER_SLV_CE_INDEX              : integer              := calc_start_ce_index(IPIF_ARD_NUM_CE_ARRAY, USER_SLV_CS_INDEX);

  constant USER_CE_INDEX                  : integer              := USER_SLV_CE_INDEX;

  ------------------------------------------
  -- IP Interconnect (IPIC) signal declarations
  ------------------------------------------
  signal ipif_Bus2IP_Clk                : std_logic;
  signal ipif_Bus2IP_Resetn             : std_logic;
  signal ipif_Bus2IP_Addr               : std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
  signal ipif_Bus2IP_RNW                : std_logic;
  signal ipif_Bus2IP_BE                 : std_logic_vector(IPIF_SLV_DWIDTH/8-1 downto 0);
  signal ipif_Bus2IP_CS                 : std_logic_vector((IPIF_ARD_ADDR_RANGE_ARRAY'LENGTH)/2-1 downto 0);
  signal ipif_Bus2IP_RdCE               : std_logic_vector(calc_num_ce(IPIF_ARD_NUM_CE_ARRAY)-1 downto 0);
  signal ipif_Bus2IP_WrCE               : std_logic_vector(calc_num_ce(IPIF_ARD_NUM_CE_ARRAY)-1 downto 0);
  signal ipif_Bus2IP_Data               : std_logic_vector(IPIF_SLV_DWIDTH-1 downto 0);
  signal ipif_IP2Bus_WrAck              : std_logic;
  signal ipif_IP2Bus_RdAck              : std_logic;
  signal ipif_IP2Bus_Error              : std_logic;
  signal ipif_IP2Bus_Data               : std_logic_vector(IPIF_SLV_DWIDTH-1 downto 0);
  signal user_Bus2IP_RdCE               : std_logic_vector(USER_NUM_REG-1 downto 0);
  signal user_Bus2IP_WrCE               : std_logic_vector(USER_NUM_REG-1 downto 0);
  signal user_IP2Bus_Data               : std_logic_vector(USER_SLV_DWIDTH-1 downto 0);
  signal user_IP2Bus_RdAck              : std_logic;
  signal user_IP2Bus_WrAck              : std_logic;
  signal user_IP2Bus_Error              : std_logic;
  
  ------------------------------------------
  -- FlipFlop to ease timing
  ------------------------------------------
  signal Ddr3CceInitDone_s      : std_logic;
  
begin

  ------------------------------------------
  -- instantiate axi_lite_ipif
  ------------------------------------------
  AXI_LITE_IPIF_I : entity axi_lite_ipif_v1_01_a.axi_lite_ipif
    generic map
    (
      C_S_AXI_DATA_WIDTH             => IPIF_SLV_DWIDTH,
      C_S_AXI_ADDR_WIDTH             => C_S_AXI_ADDR_WIDTH,
      C_S_AXI_MIN_SIZE               => C_S_AXI_MIN_SIZE,
      C_USE_WSTRB                    => C_USE_WSTRB,
      C_DPHASE_TIMEOUT               => C_DPHASE_TIMEOUT,
      C_ARD_ADDR_RANGE_ARRAY         => IPIF_ARD_ADDR_RANGE_ARRAY,
      C_ARD_NUM_CE_ARRAY             => IPIF_ARD_NUM_CE_ARRAY,
      C_FAMILY                       => C_FAMILY
    )
    port map
    (
      S_AXI_ACLK                     => S_AXI_ACLK,
      S_AXI_ARESETN                  => S_AXI_ARESETN,
      S_AXI_AWADDR                   => S_AXI_AWADDR,
      S_AXI_AWVALID                  => S_AXI_AWVALID,
      S_AXI_WDATA                    => S_AXI_WDATA,
      S_AXI_WSTRB                    => S_AXI_WSTRB,
      S_AXI_WVALID                   => S_AXI_WVALID,
      S_AXI_BREADY                   => S_AXI_BREADY,
      S_AXI_ARADDR                   => S_AXI_ARADDR,
      S_AXI_ARVALID                  => S_AXI_ARVALID,
      S_AXI_RREADY                   => S_AXI_RREADY,
      S_AXI_ARREADY                  => S_AXI_ARREADY,
      S_AXI_RDATA                    => S_AXI_RDATA,
      S_AXI_RRESP                    => S_AXI_RRESP,
      S_AXI_RVALID                   => S_AXI_RVALID,
      S_AXI_WREADY                   => S_AXI_WREADY,
      S_AXI_BRESP                    => S_AXI_BRESP,
      S_AXI_BVALID                   => S_AXI_BVALID,
      S_AXI_AWREADY                  => S_AXI_AWREADY,
      Bus2IP_Clk                     => ipif_Bus2IP_Clk,
      Bus2IP_Resetn                  => ipif_Bus2IP_Resetn,
      Bus2IP_Addr                    => ipif_Bus2IP_Addr,
      Bus2IP_RNW                     => ipif_Bus2IP_RNW,
      Bus2IP_BE                      => ipif_Bus2IP_BE,
      Bus2IP_CS                      => ipif_Bus2IP_CS,
      Bus2IP_RdCE                    => ipif_Bus2IP_RdCE,
      Bus2IP_WrCE                    => ipif_Bus2IP_WrCE,
      Bus2IP_Data                    => ipif_Bus2IP_Data,
      IP2Bus_WrAck                   => ipif_IP2Bus_WrAck,
      IP2Bus_RdAck                   => ipif_IP2Bus_RdAck,
      IP2Bus_Error                   => ipif_IP2Bus_Error,
      IP2Bus_Data                    => ipif_IP2Bus_Data
    );
    
  ------------------------------------------
  -- FlipFlop to ease timing
  ------------------------------------------
  process(S_AXI_ACLK)
  begin
    if rising_edge(S_AXI_ACLK) then
      Ddr3CceInitDone_s     <= i_ddr3_init_done_p;
    end if;
  end process;

 ------------------------------------------
 -- instantiate User Logic
 ------------------------------------------
 USER_LOGIC_I : entity lyt_axi_perseus6010_regs_v1_00_a.user_logic
   generic map
   (
     -- MAP USER GENERICS BELOW THIS LINE ---------------
     --USER generics mapped here
     -- MAP USER GENERICS ABOVE THIS LINE ---------------

     C_NUM_REG                      => USER_NUM_REG,
     C_SLV_DWIDTH                   => USER_SLV_DWIDTH
   )
   port map
   (
     -- user_logic entity ports mapping  ---------------
     i_daughter_absent_p                     => i_daughter_absent_p,
     i_fmc1_absent_p                     => i_fmc1_absent_p,
     o_pcie_msi_p                     => o_pcie_msi_p,
     ov2_uart_mode_p                     => ov2_uart_mode_p,
     i_fmc1_stack_absent_p                     => i_fmc1_stack_absent_p,
	 i_fmc1_clkdir_p                     => i_fmc1_clkdir_p,
     i_ddr3_init_done_p                     => Ddr3CceInitDone_s,
     i_pcie_present_p                     => i_pcie_present_p,
     i_pcie_clock_lock_p                     => i_pcie_clock_lock_p,
     o_CtrlTclkdTxEn_p                     => o_CtrlTclkdTxEn_p,
     o_CtrlTclkdRxDis_p                     => o_CtrlTclkdRxDis_p,
     o_CtrlAmctclkc2Fmcclk3En_p                     => o_CtrlAmctclkc2Fmcclk3En_p,
     o_CtrlAmctclka2Fmcclk2En_p                     => o_CtrlAmctclka2Fmcclk2En_p,
     o_CtrlFclkaHighz_p                     => o_CtrlFclkaHighz_p,
     o_Ctrl100mhzOutEn_p                     => o_Ctrl100mhzOutEn_p,
     o_CtrlGatedClkEn_p                     => o_CtrlGatedClkEn_p,
     o_CtrlTclkcTxEn_p                     => o_CtrlTclkcTxEn_p,
     o_CtrlTclkcRxDis_p                     => o_CtrlTclkcRxDis_p,
     o_CtrlTclkbTxEn_p                     => o_CtrlTclkbTxEn_p,
     o_CtrlTclkbRxDis_p                     => o_CtrlTclkbRxDis_p,
     o_CtrlTclkaTxEn_p                     => o_CtrlTclkaTxEn_p,
     o_CtrlTclkaRxDis_p                     => o_CtrlTclkaRxDis_p,
     o_CtrlFmcclk12AmctclkdEn_p                     => o_CtrlFmcclk12AmctclkdEn_p,
     o_CtrlFmcclk02AmctclkbEn_p                     => o_CtrlFmcclk02AmctclkbEn_p,
     ov2_CtrlVadjSel_p                     => ov2_CtrlVadjSel_p,
     o_CtrlVadjEn_p                     => o_CtrlVadjEn_p,
     o_nFpgaProg_p                     => o_nFpgaProg_p,
     ov8_nCtrlLedGrn_p                     => ov8_nCtrlLedGrn_p,
     o_CtrlLedBufOd_p                     => o_CtrlLedBufOd_p,
     ov8_nCtrlLedRed_p                     => ov8_nCtrlLedRed_p,
     o_MmcI2cReleaseReq_p                     => o_MmcI2cReleaseReq_p,
     i_MmcI2cReleaseAck_p                     => i_MmcI2cReleaseAck_p,
     iv32_CustomReg0_p                     => iv32_CustomReg0_p,
     ov32_CustomReg0_p                     => ov32_CustomReg0_p,
     iv32_CustomReg1_p                     => iv32_CustomReg1_p,
     ov32_CustomReg1_p                     => ov32_CustomReg1_p,
     iv32_CustomReg2_p                     => iv32_CustomReg2_p,
     ov32_CustomReg2_p                     => ov32_CustomReg2_p,
     iv32_CustomReg3_p                     => iv32_CustomReg3_p,
     ov32_CustomReg3_p                     => ov32_CustomReg3_p,
     iv32_CustomReg4_p                     => iv32_CustomReg4_p,
     ov32_CustomReg4_p                     => ov32_CustomReg4_p,
     iv32_CustomReg5_p                     => iv32_CustomReg5_p,
     ov32_CustomReg5_p                     => ov32_CustomReg5_p,
     iv32_CustomReg6_p                     => iv32_CustomReg6_p,
     ov32_CustomReg6_p                     => ov32_CustomReg6_p,
     iv32_CustomReg7_p                     => iv32_CustomReg7_p,
     ov32_CustomReg7_p                     => ov32_CustomReg7_p,
     iv32_CustomReg8_p                     => iv32_CustomReg8_p,
     ov32_CustomReg8_p                     => ov32_CustomReg8_p,
     iv32_CustomReg9_p                     => iv32_CustomReg9_p,
     ov32_CustomReg9_p                     => ov32_CustomReg9_p,
     iv32_CustomReg10_p                     => iv32_CustomReg10_p,
     ov32_CustomReg10_p                     => ov32_CustomReg10_p,
     iv32_CustomReg11_p                     => iv32_CustomReg11_p,
     ov32_CustomReg11_p                     => ov32_CustomReg11_p,
     iv32_CustomReg12_p                     => iv32_CustomReg12_p,
     ov32_CustomReg12_p                     => ov32_CustomReg12_p,
     iv32_CustomReg13_p                     => iv32_CustomReg13_p,
     ov32_CustomReg13_p                     => ov32_CustomReg13_p,
     iv32_CustomReg14_p                     => iv32_CustomReg14_p,
     ov32_CustomReg14_p                     => ov32_CustomReg14_p,
     iv32_CustomReg15_p                     => iv32_CustomReg15_p,
     ov32_CustomReg15_p                     => ov32_CustomReg15_p,
     iv32_CustomReg16_p                     => iv32_CustomReg16_p,
     ov32_CustomReg16_p                     => ov32_CustomReg16_p,
     iv32_CustomReg17_p                     => iv32_CustomReg17_p,
     ov32_CustomReg17_p                     => ov32_CustomReg17_p,
     iv32_CustomReg18_p                     => iv32_CustomReg18_p,
     ov32_CustomReg18_p                     => ov32_CustomReg18_p,
     iv32_CustomReg19_p                     => iv32_CustomReg19_p,
     ov32_CustomReg19_p                     => ov32_CustomReg19_p,
     iv32_CustomReg20_p                     => iv32_CustomReg20_p,
     ov32_CustomReg20_p                     => ov32_CustomReg20_p,
     iv32_CustomReg21_p                     => iv32_CustomReg21_p,
     ov32_CustomReg21_p                     => ov32_CustomReg21_p,
     iv32_CustomReg22_p                     => iv32_CustomReg22_p,
     ov32_CustomReg22_p                     => ov32_CustomReg22_p,
     iv32_CustomReg23_p                     => iv32_CustomReg23_p,
     ov32_CustomReg23_p                     => ov32_CustomReg23_p,
     iv32_CustomReg24_p                     => iv32_CustomReg24_p,
     ov32_CustomReg24_p                     => ov32_CustomReg24_p,
     iv32_CustomReg25_p                     => iv32_CustomReg25_p,
     ov32_CustomReg25_p                     => ov32_CustomReg25_p,
     iv32_CustomReg26_p                     => iv32_CustomReg26_p,
     ov32_CustomReg26_p                     => ov32_CustomReg26_p,
     iv32_CustomReg27_p                     => iv32_CustomReg27_p,
     ov32_CustomReg27_p                     => ov32_CustomReg27_p,
     iv32_CustomReg28_p                     => iv32_CustomReg28_p,
     ov32_CustomReg28_p                     => ov32_CustomReg28_p,
     iv32_CustomReg29_p                     => iv32_CustomReg29_p,
     ov32_CustomReg29_p                     => ov32_CustomReg29_p,
     iv32_CustomReg30_p                     => iv32_CustomReg30_p,
     ov32_CustomReg30_p                     => ov32_CustomReg30_p,
     iv32_CustomReg31_p                     => iv32_CustomReg31_p,
     ov32_CustomReg31_p                     => ov32_CustomReg31_p,
     ov16_Aurora0_Idx_p                     => ov16_Aurora0_Idx_p,
     o_Aurora0_WrEn_p                     => o_Aurora0_WrEn_p,
     iv32_Aurora0_Data_p                     => iv32_Aurora0_Data_p,
     ov32_Aurora0_Data_p                     => ov32_Aurora0_Data_p,
     ov16_Aurora1_Idx_p                     => ov16_Aurora1_Idx_p,
     o_Aurora1_WrEn_p                     => o_Aurora1_WrEn_p,
     iv32_Aurora1_Data_p                     => iv32_Aurora1_Data_p,
     ov32_Aurora1_Data_p                     => ov32_Aurora1_Data_p,
     ov16_Aurora2_Idx_p                     => ov16_Aurora2_Idx_p,
     o_Aurora2_WrEn_p                     => o_Aurora2_WrEn_p,
     iv32_Aurora2_Data_p                     => iv32_Aurora2_Data_p,
     ov32_Aurora2_Data_p                     => ov32_Aurora2_Data_p,
     iv32_PPSSYNC_CoreIdVers_p                     => iv32_PPSSYNC_CoreIdVers_p,
     ov32_PPSSYNC_ClkTarget_p                     => ov32_PPSSYNC_ClkTarget_p,
     o_PPSSYNC_CoreReset_p                     => o_PPSSYNC_CoreReset_p,
     o_PPSSYNC_RstUponRead_p                     => o_PPSSYNC_RstUponRead_p,
     o_PPSSYNC_RstAcc_p                     => o_PPSSYNC_RstAcc_p,
     o_PPSSYNC_CoreEnable_p                     => o_PPSSYNC_CoreEnable_p,
     o_PPSSYNC_MovSumEn_p                     => o_PPSSYNC_MovSumEn_p,
     o_PPSSYNC_MovSumRst_p                     => o_PPSSYNC_MovSumRst_p,
     iv32_PPSSYNC_PpsCnt_p                     => iv32_PPSSYNC_PpsCnt_p,
     iv32_PPSSYNC_DiffAcc_p                     => iv32_PPSSYNC_DiffAcc_p,
     o_PPSSYNC_DiffAccRead_p                     => o_PPSSYNC_DiffAccRead_p,
     ov32_PPSSYNC_ProgDelay_p                     => ov32_PPSSYNC_ProgDelay_p,
     ov32_PPSSYNC_PpsFreq_p                     => ov32_PPSSYNC_PpsFreq_p,
     iv32_lvds0_coreid_p                     => iv32_lvds0_coreid_p,
     ov32_lvds0_ctrl_p                     => ov32_lvds0_ctrl_p,
     iv32_lvds0_value_p                     => iv32_lvds0_value_p,
     ov32_lvds0_value_p                     => ov32_lvds0_value_p,
     ov32_lvds0_oe_p                     => ov32_lvds0_oe_p,
     iv32_lvds1_coreid_p                     => iv32_lvds1_coreid_p,
     ov32_lvds1_ctrl_p                     => ov32_lvds1_ctrl_p,
     iv32_lvds1_value_p                     => iv32_lvds1_value_p,
     ov32_lvds1_value_p                     => ov32_lvds1_value_p,
     ov32_lvds1_oe_p                     => ov32_lvds1_oe_p,
     iv32_lvds2_coreid_p                     => iv32_lvds2_coreid_p,
     ov32_lvds2_ctrl_p                     => ov32_lvds2_ctrl_p,
     iv32_lvds2_value_p                     => iv32_lvds2_value_p,
     ov32_lvds2_value_p                     => ov32_lvds2_value_p,
     ov32_lvds2_oe_p                     => ov32_lvds2_oe_p,
     iv32_lvds3_coreid_p                     => iv32_lvds3_coreid_p,
     ov32_lvds3_ctrl_p                     => ov32_lvds3_ctrl_p,
     iv32_lvds3_value_p                     => iv32_lvds3_value_p,
     ov32_lvds3_value_p                     => ov32_lvds3_value_p,
     ov32_lvds3_oe_p                     => ov32_lvds3_oe_p,
     iv32_RTDExSyncCoreId_p                     => iv32_RTDExSyncCoreId_p,
     o_RTDExSyncCoreReset_p                     => o_RTDExSyncCoreReset_p,
     ov8_RtdexSyncAddr_p                     => ov8_RtdexSyncAddr_p,
     o_RTDExSyncConfigWrEn_p                     => o_RTDExSyncConfigWrEn_p,
     ov32_RtdexSyncWrReg_p                     => ov32_RtdexSyncWrReg_p,
     iv32_RtdexSyncRdReg_p                     => iv32_RtdexSyncRdReg_p,
     iv32_Timestamp_CoreIdVers_p                     => iv32_Timestamp_CoreIdVers_p,
     o_Timestamp_CoreReset_p                     => o_Timestamp_CoreReset_p,
     o_Timestamp_SetTime_p                     => o_Timestamp_SetTime_p,
     o_Timestamp_LatchTime_p                     => o_Timestamp_LatchTime_p,
     iv32_Timestamp_MSW_p                     => iv64_Timestamp_p(63 downto 32),
     ov32_Timestamp_MSW_p                     => ov64_Timestamp_p(63 downto 32),
     iv32_Timestamp_LSW_p                     => iv64_Timestamp_p(31 downto 0),
     ov32_Timestamp_LSW_p                     => ov64_Timestamp_p(31 downto 0),

     Bus2IP_Clk                     => ipif_Bus2IP_Clk,
     Bus2IP_Resetn                  => ipif_Bus2IP_Resetn,
     Bus2IP_Data                    => ipif_Bus2IP_Data,
     Bus2IP_BE                      => ipif_Bus2IP_BE,
     Bus2IP_RdCE                    => user_Bus2IP_RdCE,
     Bus2IP_WrCE                    => user_Bus2IP_WrCE,
     IP2Bus_Data                    => user_IP2Bus_Data,
     IP2Bus_RdAck                   => user_IP2Bus_RdAck,
     IP2Bus_WrAck                   => user_IP2Bus_WrAck,
     IP2Bus_Error                   => user_IP2Bus_Error
   );
 
 ------------------------------------------
 -- connect internal signals
 ------------------------------------------
 ipif_IP2Bus_Data <= user_IP2Bus_Data;
 ipif_IP2Bus_WrAck <= user_IP2Bus_WrAck;
 ipif_IP2Bus_RdAck <= user_IP2Bus_RdAck;
 ipif_IP2Bus_Error <= user_IP2Bus_Error;

 user_Bus2IP_RdCE <= ipif_Bus2IP_RdCE(USER_NUM_REG-1 downto 0);
 user_Bus2IP_WrCE <= ipif_Bus2IP_WrCE(USER_NUM_REG-1 downto 0);

 end IMP;
