------------------------------------------------------------------------------
-- Filename:       axi_freq_counter.vhd
-- Version:        v1_00_a
-- Description:    Top level design, instantiates library components and user logic.
-- Generated by:   julien.roy
-- Date:           2014-02-11 11:46:03
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

 library ntq_axi_freq_counter_v1_00_a;
 use ntq_axi_freq_counter_v1_00_a.user_logic;


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
entity axi_freq_counter is
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
    i_CoreReset_p                       : in std_logic;

  iv32_freq_p                       : in std_logic_vector(31 downto 0);
  ov32_index_p                       : out std_logic_vector(31 downto 0);
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

end entity axi_freq_counter;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of axi_freq_counter is

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

  constant USER_SLV_NUM_REG               : integer              := 3;
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
 -- instantiate User Logic
 ------------------------------------------
 USER_LOGIC_I : entity ntq_axi_freq_counter_v1_00_a.user_logic
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
    i_CoreReset_p                     => i_CoreReset_p,

     iv32_freq_p                     => iv32_freq_p,
     ov32_index_p                     => ov32_index_p,

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

