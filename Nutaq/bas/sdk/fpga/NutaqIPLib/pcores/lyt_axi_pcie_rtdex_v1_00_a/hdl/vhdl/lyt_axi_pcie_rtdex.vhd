------------------------------------------------------------------------------
-- lyt_axi_pcie_rtdex.vhd - entity/architecture pair
------------------------------------------------------------------------------
-- IMPORTANT:
-- DO NOT MODIFY THIS FILE EXCEPT IN THE DESIGNATED SECTIONS.
--
-- SEARCH FOR --USER TO DETERMINE WHERE CHANGES ARE ALLOWED.
--
-- TYPICALLY, THE ONLY ACCEPTABLE CHANGES INVOLVE ADDING NEW
-- PORTS AND GENERICS THAT GET PASSED THROUGH TO THE INSTANTIATION
-- OF THE USER_LOGIC ENTITY.
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          lyt_axi_pcie_rtdex.vhd
-- Version:           1.00.a
-- Description:       Top level design, instantiates library components and user logic.
-- Date:              Fri Aug 10 12:59:23 2012 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;
use proc_common_v3_00_a.ipif_pkg.all;

library axi_slave_burst_v1_00_a;
use axi_slave_burst_v1_00_a.axi_slave_burst;

library axi_lite_ipif_v1_01_a;
use axi_lite_ipif_v1_01_a.axi_lite_ipif;
 
library lyt_axi_pcie_rtdex_v1_00_a;
use lyt_axi_pcie_rtdex_v1_00_a.user_logic_fifo;

library lyt_axi_pcie_rtdex_v1_00_a;
use lyt_axi_pcie_rtdex_v1_00_a.user_logic_reg;

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_S_AXI_DATA_WIDTH           -- AXI4 slave: Data Width
--   C_S_AXI_ADDR_WIDTH           -- AXI4 slave: Address Width
--   C_S_AXI_ID_WIDTH             -- AXI4 slave: ID Width
--   C_RDATA_FIFO_DEPTH           -- AXI4 slave: FIFO Depth
--   C_INCLUDE_TIMEOUT_CNT        -- AXI4 slave: Data Timeout Count
--   C_TIMEOUT_CNTR_VAL           -- AXI4 slave: Timeout Counter Value
--   C_ALIGN_BE_RDADDR            -- AXI4 slave: Align Byte Enable read Data Address
--   C_S_AXI_SUPPORTS_WRITE       -- AXI4 slave: Support Write
--   C_S_AXI_SUPPORTS_READ        -- AXI4 slave: Support Read
--   C_FAMILY                     -- FPGA Family
--   C_S_AXI_RX_FIFO_BASEADDR     -- User memory space base address
--   C_S_AXI_RX_FIFO_HIGHADDR     -- User memory space high address
--   C_S_AXI_REG_DATA_WIDTH       -- AXI4LITE slave: Data width
--   C_S_AXI_REG_ADDR_WIDTH       -- AXI4LITE slave: Address Width
--   C_S_AXI_REG_MIN_SIZE         -- AXI4LITE slave: Min Size
--   C_REG_USE_WSTRB              -- AXI4LITE slave: Write Strobe
--   C_REG_DPHASE_TIMEOUT         -- AXI4LITE slave: Data Phase Timeout
--   C_REG_BASEADDR               -- AXI4LITE slave: base address
--   C_REG_HIGHADDR               -- AXI4LITE slave: high address
--   C_REG_NUM_REG                -- Number of software accessible registers
--   C_REG_NUM_MEM                -- Number of address-ranges
--   C_REG_SLV_AWIDTH             -- Slave interface address bus width
--   C_REG_SLV_DWIDTH             -- Slave interface data bus width
--
-- Definition of Ports:
--   S_AXI_ACLK                   -- AXI4 slave: Clock
--   S_AXI_ARESETN                -- AXI4 slave: Reset
--   S_AXI_AWADDR                 -- AXI4 slave: Write address
--   S_AXI_AWVALID                -- AXI4 slave: Write address valid
--   S_AXI_WDATA                  -- AXI4 slave: Write data
--   S_AXI_WSTRB                  -- AXI4 slave: Write strobe
--   S_AXI_WVALID                 -- AXI4 slave: Write data valid
--   S_AXI_BREADY                 -- AXI4 slave: read response ready
--   S_AXI_ARADDR                 -- AXI4 slave: read address
--   S_AXI_ARVALID                -- AXI4 slave: read address valid
--   S_AXI_RREADY                 -- AXI4 slave: read data ready
--   S_AXI_ARREADY                -- AXI4 slave: read address ready
--   S_AXI_RDATA                  -- AXI4 slave: read data
--   S_AXI_RRESP                  -- AXI4 slave: read data response
--   S_AXI_RVALID                 -- AXI4 slave: read data valid
--   S_AXI_WREADY                 -- AXI4 slave: write data ready
--   S_AXI_BRESP                  -- AXI4 slave: read response
--   S_AXI_BVALID                 -- AXI4 slave: read response valid
--   S_AXI_AWREADY                -- AXI4 slave: write address ready
--   S_AXI_AWID                   -- AXI4 slave: write address ID
--   S_AXI_AWLEN                  -- AXI4 slave: write address Length
--   S_AXI_AWSIZE                 -- AXI4 slave: write address size
--   S_AXI_AWBURST                -- AXI4 slave: write address burst
--   S_AXI_AWLOCK                 -- AXI4 slave: write address lock
--   S_AXI_AWCACHE                -- AXI4 slave: write address cache
--   S_AXI_AWPROT                 -- AXI4 slave: write address protection
--   S_AXI_WLAST                  -- AXI4 slave: write data last
--   S_AXI_BID                    -- AXI4 slave: read response ID
--   S_AXI_ARID                   -- AXI4 slave: read address ID
--   S_AXI_ARLEN                  -- AXI4 slave: read address Length
--   S_AXI_ARSIZE                 -- AXI4 slave: read address size
--   S_AXI_ARBURST                -- AXI4 slave: read address burst
--   S_AXI_ARLOCK                 -- AXI4 slave: read address lock
--   S_AXI_ARCACHE                -- AXI4 slave: read address cache
--   S_AXI_ARPROT                 -- AXI4 slave: read address protection
--   S_AXI_RID                    -- AXI4 slave: read data ID
--   S_AXI_RLAST                  -- AXI4 slave: read data last
------------------------------------------------------------------------------

entity lyt_axi_pcie_rtdex is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    C_RTDEX_RX_NUMBER_OF_CHANNELS : integer range 0 to 8  :=1;
    C_RTDEX_TX_NUMBER_OF_CHANNELS : integer range 0 to 8  :=1;
    
    -- The DEPTH parameters specify the FIFO depth in number of 4-byte samples.
    C_RX_CH_FIFO_DEPTH            : integer               := 4096;
    C_TX_CH_FIFO_DEPTH            : integer               := 4096;
    
    C_RX_WIDTH_CH0                : integer := 32;
    C_RX_WIDTH_CH1                : integer := 32;
    C_RX_WIDTH_CH2                : integer := 32;
    C_RX_WIDTH_CH3                : integer := 32;
    C_RX_WIDTH_CH4                : integer := 32;
    C_RX_WIDTH_CH5                : integer := 32;
    C_RX_WIDTH_CH6                : integer := 32;
    C_RX_WIDTH_CH7                : integer := 32;
    C_TX_WIDTH_CH0                : integer := 32;
    C_TX_WIDTH_CH1                : integer := 32;
    C_TX_WIDTH_CH2                : integer := 32;
    C_TX_WIDTH_CH3                : integer := 32;
    C_TX_WIDTH_CH4                : integer := 32;
    C_TX_WIDTH_CH5                : integer := 32;
    C_TX_WIDTH_CH6                : integer := 32;
    C_TX_WIDTH_CH7                : integer := 32;
    C_CDMA_PRESENT                : integer range 0 to 1  := 1;
    
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters (FIFO interface)
    C_S_AXI_DATA_WIDTH             : integer              := 64;
    C_S_AXI_ADDR_WIDTH             : integer              := 32;
    C_S_AXI_ID_WIDTH               : integer              := 4;
    C_RDATA_FIFO_DEPTH             : integer              := 0;
    C_INCLUDE_TIMEOUT_CNT          : integer              := 1;
    C_TIMEOUT_CNTR_VAL             : integer              := 8;
    C_ALIGN_BE_RDADDR              : integer              := 0;
    C_S_AXI_SUPPORTS_WRITE         : integer              := 1;
    C_S_AXI_SUPPORTS_READ          : integer              := 1;
    C_FAMILY                       : string               := "virtex6";
    C_S_AXI_RX_FIFO_BASEADDR       : std_logic_vector     := X"FFFFFFFF";
    C_S_AXI_RX_FIFO_HIGHADDR       : std_logic_vector     := X"00000000";

    -- Bus protocol parameters (register interface)
    C_S_AXI_REG_DATA_WIDTH         : integer              := 32;
    C_S_AXI_REG_ADDR_WIDTH         : integer              := 32;
    C_S_AXI_REG_MIN_SIZE           : std_logic_vector     := X"000001FF";
    C_REG_USE_WSTRB                : integer              := 0;
    C_REG_DPHASE_TIMEOUT           : integer              := 8;
    C_REG_BASEADDR                 : std_logic_vector     := X"FFFFFFFF";
    C_REG_HIGHADDR                 : std_logic_vector     := X"00000000";
    C_REG_NUM_REG                  : integer              := 1;
    C_REG_NUM_MEM                  : integer              := 1;
    C_REG_SLV_AWIDTH               : integer              := 32;
    C_REG_SLV_DWIDTH               : integer              := 32

    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    --USER ports added here
    o_mbIrq_p                     : out std_logic;
    o_PcieMsi_p                   : out std_logic;

    --- RX User ports ---           
    i_RxUserClk_p                 : in std_logic;
    
    o_RxReadyCh0_p                : out std_logic;
    i_RxReCh0_p                   : in std_logic;
    ov_RxDataCh0_p                : out std_logic_vector(C_RX_WIDTH_CH0-1 downto 0);
    o_RxDataValidCh0_p            : out std_logic;      
    o_RxReadyCh1_p                : out std_logic;
    i_RxReCh1_p                   : in std_logic;
    ov_RxDataCh1_p                : out std_logic_vector(C_RX_WIDTH_CH1-1 downto 0);
    o_RxDataValidCh1_p            : out std_logic;      
    o_RxReadyCh2_p                : out std_logic;
    i_RxReCh2_p                   : in std_logic;
    ov_RxDataCh2_p                : out std_logic_vector(C_RX_WIDTH_CH2-1 downto 0);
    o_RxDataValidCh2_p            : out std_logic;      
    o_RxReadyCh3_p                : out std_logic;
    i_RxReCh3_p                   : in std_logic;
    ov_RxDataCh3_p                : out std_logic_vector(C_RX_WIDTH_CH3-1 downto 0);
    o_RxDataValidCh3_p            : out std_logic;      
    o_RxReadyCh4_p                : out std_logic;
    i_RxReCh4_p                   : in std_logic;
    ov_RxDataCh4_p                : out std_logic_vector(C_RX_WIDTH_CH4-1 downto 0);
    o_RxDataValidCh4_p            : out std_logic;      
    o_RxReadyCh5_p                : out std_logic;
    i_RxReCh5_p                   : in std_logic;
    ov_RxDataCh5_p                : out std_logic_vector(C_RX_WIDTH_CH5-1 downto 0);
    o_RxDataValidCh5_p            : out std_logic;
    o_RxReadyCh6_p                : out std_logic;
    i_RxReCh6_p                   : in std_logic;
    ov_RxDataCh6_p                : out std_logic_vector(C_RX_WIDTH_CH6-1 downto 0);
    o_RxDataValidCh6_p            : out std_logic;      
    o_RxReadyCh7_p                : out std_logic;
    i_RxReCh7_p                   : in std_logic;
    ov_RxDataCh7_p                : out std_logic_vector(C_RX_WIDTH_CH7-1 downto 0);
    o_RxDataValidCh7_p            : out std_logic;

    --- TX User ports ---                     
    i_TxUserClk_p                 : in std_logic;
                                  
    i_TxWeCh0_p                   : in std_logic;
    o_TxReadyCh0_p                : out std_logic;
    iv_TxDataCh0_p                : in std_logic_vector(C_TX_WIDTH_CH0-1 downto 0);
                                        
    i_TxWeCh1_p                   : in std_logic;
    o_TxReadyCh1_p                : out std_logic;
    iv_TxDataCh1_p                : in std_logic_vector(C_TX_WIDTH_CH1-1 downto 0);
                                        
    i_TxWeCh2_p                   : in std_logic;
    o_TxReadyCh2_p                : out std_logic;
    iv_TxDataCh2_p                : in std_logic_vector(C_TX_WIDTH_CH2-1 downto 0);
                                        
    i_TxWeCh3_p                   : in std_logic;
    o_TxReadyCh3_p                : out std_logic;
    iv_TxDataCh3_p                : in std_logic_vector(C_TX_WIDTH_CH3-1 downto 0);
                                        
    i_TxWeCh4_p                   : in std_logic;
    o_TxReadyCh4_p                : out std_logic;
    iv_TxDataCh4_p                : in std_logic_vector(C_TX_WIDTH_CH4-1 downto 0);
                                        
    i_TxWeCh5_p                   : in std_logic;
    o_TxReadyCh5_p                : out std_logic;
    iv_TxDataCh5_p                : in std_logic_vector(C_TX_WIDTH_CH5-1 downto 0);
                                        
    i_TxWeCh6_p                   : in std_logic;
    o_TxReadyCh6_p                : out std_logic;
    iv_TxDataCh6_p                : in std_logic_vector(C_TX_WIDTH_CH6-1 downto 0);
                                        
    i_TxWeCh7_p                   : in std_logic;
    o_TxReadyCh7_p                : out std_logic;
    iv_TxDataCh7_p                : in std_logic_vector(C_TX_WIDTH_CH7-1 downto 0);
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports (fifo interface)
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
    S_AXI_AWREADY                  : out std_logic;
    S_AXI_AWID                     : in  std_logic_vector(C_S_AXI_ID_WIDTH-1 downto 0);
    S_AXI_AWLEN                    : in  std_logic_vector(7 downto 0);
    S_AXI_AWSIZE                   : in  std_logic_vector(2 downto 0);
    S_AXI_AWBURST                  : in  std_logic_vector(1 downto 0);
    S_AXI_AWLOCK                   : in  std_logic;
    S_AXI_AWCACHE                  : in  std_logic_vector(3 downto 0);
    S_AXI_AWPROT                   : in  std_logic_vector(2 downto 0);
    S_AXI_WLAST                    : in  std_logic;
    S_AXI_BID                      : out std_logic_vector(C_S_AXI_ID_WIDTH-1 downto 0);
    S_AXI_ARID                     : in  std_logic_vector(C_S_AXI_ID_WIDTH-1 downto 0);
    S_AXI_ARLEN                    : in  std_logic_vector(7 downto 0);
    S_AXI_ARSIZE                   : in  std_logic_vector(2 downto 0);
    S_AXI_ARBURST                  : in  std_logic_vector(1 downto 0);
    S_AXI_ARLOCK                   : in  std_logic;
    S_AXI_ARCACHE                  : in  std_logic_vector(3 downto 0);
    S_AXI_ARPROT                   : in  std_logic_vector(2 downto 0);
    S_AXI_RID                      : out std_logic_vector(C_S_AXI_ID_WIDTH-1 downto 0);
    S_AXI_RLAST                    : out std_logic;

    -- Bus protocol ports (register interface)
    S_AXI_REG_ACLK                 : in  std_logic;
    S_AXI_REG_ARESETN              : in  std_logic;
    S_AXI_REG_AWADDR               : in  std_logic_vector(C_S_AXI_REG_ADDR_WIDTH-1 downto 0);
    S_AXI_REG_AWVALID              : in  std_logic;
    S_AXI_REG_WDATA                : in  std_logic_vector(C_S_AXI_REG_DATA_WIDTH-1 downto 0);
    S_AXI_REG_WSTRB                : in  std_logic_vector((C_S_AXI_REG_DATA_WIDTH/8)-1 downto 0);
    S_AXI_REG_WVALID               : in  std_logic;
    S_AXI_REG_BREADY               : in  std_logic;
    S_AXI_REG_ARADDR               : in  std_logic_vector(C_S_AXI_REG_ADDR_WIDTH-1 downto 0);
    S_AXI_REG_ARVALID              : in  std_logic;
    S_AXI_REG_RREADY               : in  std_logic;
    S_AXI_REG_ARREADY              : out std_logic;
    S_AXI_REG_RDATA                : out std_logic_vector(C_S_AXI_REG_DATA_WIDTH-1 downto 0);
    S_AXI_REG_RRESP                : out std_logic_vector(1 downto 0);
    S_AXI_REG_RVALID               : out std_logic;
    S_AXI_REG_WREADY               : out std_logic;
    S_AXI_REG_BRESP                : out std_logic_vector(1 downto 0);
    S_AXI_REG_BVALID               : out std_logic;
    S_AXI_REG_AWREADY              : out std_logic;

    -- AXI Memory interface (Channel 0)
    m_axi_s2mm_Ch0_aclk           : in std_logic;
    m_axi_s2mm_Ch0_aresetn        : in std_logic;
    m_axi_s2mm_Ch0_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch0_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch0_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch0_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch0_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch0_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch0_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch0_awvalid        : out std_logic;
    m_axi_s2mm_Ch0_awready        : in std_logic;
    m_axi_s2mm_Ch0_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch0_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch0_wlast          : out std_logic;
    m_axi_s2mm_Ch0_wvalid         : out std_logic;
    m_axi_s2mm_Ch0_wready         : in std_logic;
    m_axi_s2mm_Ch0_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch0_bvalid         : in std_logic;
    m_axi_s2mm_Ch0_bready         : out std_logic;

    -- AXI Memory interface (Channel 1)
    m_axi_s2mm_Ch1_aclk           : in std_logic;
    m_axi_s2mm_Ch1_aresetn        : in std_logic;
    m_axi_s2mm_Ch1_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch1_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch1_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch1_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch1_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch1_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch1_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch1_awvalid        : out std_logic;
    m_axi_s2mm_Ch1_awready        : in std_logic;
    m_axi_s2mm_Ch1_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch1_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch1_wlast          : out std_logic;
    m_axi_s2mm_Ch1_wvalid         : out std_logic;
    m_axi_s2mm_Ch1_wready         : in std_logic;
    m_axi_s2mm_Ch1_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch1_bvalid         : in std_logic;
    m_axi_s2mm_Ch1_bready         : out std_logic;

    -- AXI Memory interface (Channel 2)
    m_axi_s2mm_Ch2_aclk           : in std_logic;
    m_axi_s2mm_Ch2_aresetn        : in std_logic;
    m_axi_s2mm_Ch2_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch2_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch2_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch2_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch2_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch2_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch2_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch2_awvalid        : out std_logic;
    m_axi_s2mm_Ch2_awready        : in std_logic;
    m_axi_s2mm_Ch2_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch2_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch2_wlast          : out std_logic;
    m_axi_s2mm_Ch2_wvalid         : out std_logic;
    m_axi_s2mm_Ch2_wready         : in std_logic;
    m_axi_s2mm_Ch2_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch2_bvalid         : in std_logic;
    m_axi_s2mm_Ch2_bready         : out std_logic;

    -- AXI Memory interface (Channel 3)
    m_axi_s2mm_Ch3_aclk           : in std_logic;
    m_axi_s2mm_Ch3_aresetn        : in std_logic;
    m_axi_s2mm_Ch3_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch3_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch3_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch3_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch3_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch3_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch3_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch3_awvalid        : out std_logic;
    m_axi_s2mm_Ch3_awready        : in std_logic;
    m_axi_s2mm_Ch3_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch3_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch3_wlast          : out std_logic;
    m_axi_s2mm_Ch3_wvalid         : out std_logic;
    m_axi_s2mm_Ch3_wready         : in std_logic;
    m_axi_s2mm_Ch3_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch3_bvalid         : in std_logic;
    m_axi_s2mm_Ch3_bready         : out std_logic;

    -- AXI Memory interface (Channel 4)
    m_axi_s2mm_Ch4_aclk           : in std_logic;
    m_axi_s2mm_Ch4_aresetn        : in std_logic;
    m_axi_s2mm_Ch4_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch4_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch4_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch4_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch4_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch4_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch4_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch4_awvalid        : out std_logic;
    m_axi_s2mm_Ch4_awready        : in std_logic;
    m_axi_s2mm_Ch4_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch4_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch4_wlast          : out std_logic;
    m_axi_s2mm_Ch4_wvalid         : out std_logic;
    m_axi_s2mm_Ch4_wready         : in std_logic;
    m_axi_s2mm_Ch4_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch4_bvalid         : in std_logic;
    m_axi_s2mm_Ch4_bready         : out std_logic;

    -- AXI Memory interface (Channel 5)
    m_axi_s2mm_Ch5_aclk           : in std_logic;
    m_axi_s2mm_Ch5_aresetn        : in std_logic;
    m_axi_s2mm_Ch5_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch5_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch5_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch5_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch5_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch5_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch5_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch5_awvalid        : out std_logic;
    m_axi_s2mm_Ch5_awready        : in std_logic;
    m_axi_s2mm_Ch5_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch5_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch5_wlast          : out std_logic;
    m_axi_s2mm_Ch5_wvalid         : out std_logic;
    m_axi_s2mm_Ch5_wready         : in std_logic;
    m_axi_s2mm_Ch5_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch5_bvalid         : in std_logic;
    m_axi_s2mm_Ch5_bready         : out std_logic;

    -- AXI Memory interface (Channel 6)
    m_axi_s2mm_Ch6_aclk           : in std_logic;
    m_axi_s2mm_Ch6_aresetn        : in std_logic;
    m_axi_s2mm_Ch6_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch6_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch6_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch6_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch6_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch6_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch6_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch6_awvalid        : out std_logic;
    m_axi_s2mm_Ch6_awready        : in std_logic;
    m_axi_s2mm_Ch6_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch6_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch6_wlast          : out std_logic;
    m_axi_s2mm_Ch6_wvalid         : out std_logic;
    m_axi_s2mm_Ch6_wready         : in std_logic;
    m_axi_s2mm_Ch6_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch6_bvalid         : in std_logic;
    m_axi_s2mm_Ch6_bready         : out std_logic;

    -- AXI Memory interface (Channel 7)
    m_axi_s2mm_Ch7_aclk           : in std_logic;
    m_axi_s2mm_Ch7_aresetn        : in std_logic;
    m_axi_s2mm_Ch7_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch7_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch7_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch7_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch7_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch7_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch7_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch7_awvalid        : out std_logic;
    m_axi_s2mm_Ch7_awready        : in std_logic;
    m_axi_s2mm_Ch7_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch7_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch7_wlast          : out std_logic;
    m_axi_s2mm_Ch7_wvalid         : out std_logic;
    m_axi_s2mm_Ch7_wready         : in std_logic;
    m_axi_s2mm_Ch7_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch7_bvalid         : in std_logic;
    m_axi_s2mm_Ch7_bready         : out std_logic    
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT              : string;
  attribute SIGIS                   : string;
  attribute MAX_FANOUT of S_AXI_ACLK : signal is "10000";
  attribute MAX_FANOUT of S_AXI_ARESETN : signal is "10000";
  attribute SIGIS of S_AXI_ACLK     : signal is "Clk";
  attribute SIGIS of S_AXI_ARESETN  : signal is "Rst";
  
end entity lyt_axi_pcie_rtdex;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of lyt_axi_pcie_rtdex is
  
  attribute keep_hierarchy : string;
  attribute keep_hierarchy of IMP: architecture is "true";
  
  
  constant USER_SLV_DWIDTH                : integer              := C_S_AXI_DATA_WIDTH;

  constant IPIF_SLV_DWIDTH                : integer              := C_S_AXI_DATA_WIDTH;

  constant ZERO_ADDR_PAD                  : std_logic_vector(0 to 31) := (others => '0');

  constant IPIF_ARD_ADDR_RANGE_ARRAY      : SLV64_ARRAY_TYPE     := 
    (
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR,                    -- user logic memory space 0 base address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"1FFFF",-- user logic memory space 0 high address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"20000",-- user logic memory space 1 base address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"3FFFF",-- user logic memory space 1 high address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"40000",-- user logic memory space 2 base address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"5FFFF",-- user logic memory space 2 high address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"60000",-- user logic memory space 3 base address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"7FFFF",-- user logic memory space 3 high address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"80000",-- user logic memory space 4 base address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"9FFFF",-- user logic memory space 4 high address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"A0000",-- user logic memory space 5 base address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"BFFFF",-- user logic memory space 5 high address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"C0000",-- user logic memory space 6 base address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"DFFFF",-- user logic memory space 6 high address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"E0000",-- user logic memory space 7 base address
      ZERO_ADDR_PAD & C_S_AXI_RX_FIFO_BASEADDR(0 to 11) & X"FFFFF" -- user logic memory space 7 high address
    );

  constant USER_NUM_MEM                   : integer              := 8;

  constant IPIF_ARD_NUM_CE_ARRAY          : INTEGER_ARRAY_TYPE   := 
    (
      0  => 1,                            -- number of ce for user logic memory space 0 (always 1 chip enable)
      1  => 1,                            -- number of ce for user logic memory space 1 (always 1 chip enable)
      2  => 1,                            -- number of ce for user logic memory space 2 (always 1 chip enable)
      3  => 1,                            -- number of ce for user logic memory space 3 (always 1 chip enable)
      4  => 1,                            -- number of ce for user logic memory space 4 (always 1 chip enable)
      5  => 1,                            -- number of ce for user logic memory space 5 (always 1 chip enable)
      6  => 1,                            -- number of ce for user logic memory space 6 (always 1 chip enable)
      7  => 1                             -- number of ce for user logic memory space 7 (always 1 chip enable)
    );

  ------------------------------------------
  -- Width of the slave address bus (32 only)
  ------------------------------------------
  constant USER_SLV_AWIDTH                : integer              := C_S_AXI_ADDR_WIDTH;

  ------------------------------------------
  -- Index for CS/CE
  ------------------------------------------
  constant USER_MEM0_CS_INDEX             : integer              := 0;

  constant USER_CS_INDEX                  : integer              := USER_MEM0_CS_INDEX;

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
  signal ipif_Bus2IP_Burst              : std_logic;
  signal ipif_Bus2IP_BurstLength        : std_logic_vector(7 downto 0);
  signal ipif_Bus2IP_WrReq              : std_logic;
  signal ipif_Bus2IP_RdReq              : std_logic;
  signal ipif_IP2Bus_AddrAck            : std_logic;
  signal ipif_IP2Bus_RdAck              : std_logic;
  signal ipif_IP2Bus_WrAck              : std_logic;
  signal ipif_IP2Bus_Error              : std_logic;
  signal ipif_IP2Bus_Data               : std_logic_vector(IPIF_SLV_DWIDTH-1 downto 0);
  signal ipif_Type_of_xfer              : std_logic;
  signal user_Bus2IP_BurstLength        : std_logic_vector(7 downto 0)   := (others => '0');
  signal user_IP2Bus_AddrAck            : std_logic;
  signal user_IP2Bus_Data               : std_logic_vector(USER_SLV_DWIDTH-1 downto 0);
  signal user_IP2Bus_RdAck              : std_logic;
  signal user_IP2Bus_WrAck              : std_logic;
  signal user_IP2Bus_Error              : std_logic;
  signal user_Type_of_xfer              : std_logic;

  ------------------------------------------------------------------------------
  -- Signal for AXI register interface
  ------------------------------------------------------------------------------

  constant USER_SLV_REG_DWIDTH            : integer              := C_S_AXI_REG_DATA_WIDTH;

  constant IPIF_SLV_REG_DWIDTH            : integer              := C_S_AXI_REG_DATA_WIDTH;

  constant USER_SLV_REG_BASEADDR          : std_logic_vector     := C_REG_BASEADDR;
  constant USER_SLV_REG_HIGHADDR          : std_logic_vector     := C_REG_HIGHADDR;

  constant IPIF_REG_ARD_ADDR_RANGE_ARRAY  : SLV64_ARRAY_TYPE     := 
    (
      ZERO_ADDR_PAD & USER_SLV_REG_BASEADDR,  -- user logic slave space base address
      ZERO_ADDR_PAD & USER_SLV_REG_HIGHADDR   -- user logic slave space high address
    );

  constant USER_SLV_REG_NUM_REG           : integer              := 85;
  constant USER_REG_NUM_REG               : integer              := USER_SLV_REG_NUM_REG;
  constant REG_TOTAL_IPIF_CE              : integer              := USER_REG_NUM_REG;

  constant IPIF_REG_ARD_NUM_CE_ARRAY      : INTEGER_ARRAY_TYPE   :=
    (
      0  => (USER_SLV_REG_NUM_REG)  -- number of ce for user logic slave space
    );

  ------------------------------------------
  -- Index for CS/CE
  ------------------------------------------
  constant USER_SLV_REG_CS_INDEX          : integer              := 0;
  constant USER_SLV_REG_CE_INDEX          : integer              := calc_start_ce_index(IPIF_REG_ARD_NUM_CE_ARRAY, USER_SLV_REG_CS_INDEX);

  constant USER_REG_CE_INDEX              : integer              := USER_SLV_REG_CE_INDEX;

  ------------------------------------------
  -- IP Interconnect (IPIC) signal declarations
  ------------------------------------------
  signal ipif_reg_Bus2IP_Clk              : std_logic;
  signal ipif_reg_Bus2IP_Resetn           : std_logic;
  signal ipif_reg_Bus2IP_Addr             : std_logic_vector(C_S_AXI_REG_ADDR_WIDTH-1 downto 0);
  signal ipif_reg_Bus2IP_RNW              : std_logic;
  signal ipif_reg_Bus2IP_BE               : std_logic_vector(IPIF_SLV_REG_DWIDTH/8-1 downto 0);
  signal ipif_reg_Bus2IP_CS               : std_logic_vector((IPIF_REG_ARD_ADDR_RANGE_ARRAY'LENGTH)/2-1 downto 0);
  signal ipif_reg_Bus2IP_RdCE             : std_logic_vector(calc_num_ce(IPIF_REG_ARD_NUM_CE_ARRAY)-1 downto 0);
  signal ipif_reg_Bus2IP_WrCE             : std_logic_vector(calc_num_ce(IPIF_REG_ARD_NUM_CE_ARRAY)-1 downto 0);
  signal ipif_reg_Bus2IP_Data             : std_logic_vector(IPIF_SLV_REG_DWIDTH-1 downto 0);
  signal ipif_reg_IP2Bus_WrAck            : std_logic;
  signal ipif_reg_IP2Bus_RdAck            : std_logic;
  signal ipif_reg_IP2Bus_Error            : std_logic;
  signal ipif_reg_IP2Bus_Data             : std_logic_vector(IPIF_SLV_REG_DWIDTH-1 downto 0);
  signal user_reg_Bus2IP_RdCE             : std_logic_vector(USER_REG_NUM_REG-1 downto 0);
  signal user_reg_Bus2IP_WrCE             : std_logic_vector(USER_REG_NUM_REG-1 downto 0);
  signal user_reg_IP2Bus_Data             : std_logic_vector(USER_SLV_REG_DWIDTH-1 downto 0);
  signal user_reg_IP2Bus_RdAck            : std_logic;
  signal user_reg_IP2Bus_WrAck            : std_logic;
  signal user_reg_IP2Bus_Error            : std_logic;

  signal cdmaPresent_s                    : std_logic;
  signal logicRst_s                       : std_logic;
  signal CoreResetPulse_s                 : std_logic;
  signal v8_RxWordCntReset_s              : std_logic_vector(7 downto 0);
  signal v8_TxFifoReset_s                 : std_logic_vector(7 downto 0);
  signal v8_RxFifoReset_s                 : std_logic_vector(7 downto 0);
  signal v32_RxConfigInfo_s               : std_logic_vector(31 downto 0);
  signal v8_RxFifoOverflow_s              : std_logic_vector(7 downto 0);
  signal v8_RxFifoUnderflow_s             : std_logic_vector(7 downto 0);
  signal v8_RxFifoRdEn_s                  : std_logic_vector(7 downto 0);
  signal v8_TxFifoWrEn_s                  : std_logic_vector(7 downto 0);
  signal v8_RxFifoWrEn_s                  : std_logic_vector(7 downto 0);
  signal v32_RcvdWordCntCh0_s             : std_logic_vector(31 downto 0);
  signal v32_RcvdWordCntCh1_s             : std_logic_vector(31 downto 0);
  signal v32_RcvdWordCntCh2_s             : std_logic_vector(31 downto 0);
  signal v32_RcvdWordCntCh3_s             : std_logic_vector(31 downto 0);
  signal v32_RcvdWordCntCh4_s             : std_logic_vector(31 downto 0);
  signal v32_RcvdWordCntCh5_s             : std_logic_vector(31 downto 0);
  signal v32_RcvdWordCntCh6_s             : std_logic_vector(31 downto 0);
  signal v32_RcvdWordCntCh7_s             : std_logic_vector(31 downto 0);
  signal v32_TxConfigInfo_s               : std_logic_vector(31 downto 0);
  signal v8_TxFifoOverflow_s              : std_logic_vector(7 downto 0);
  signal v8_TxFifoUnderflow_s             : std_logic_vector(7 downto 0);
  signal v8_TxStartNewTransfer_s          : std_logic_vector(7 downto 0);
  signal v8_TxTransferDone_s              : std_logic_vector(7 downto 0);
  signal v8_streamingTransfer_s           : std_logic_vector(7 downto 0);

  signal v8_irqLastTransferEn_s	          : std_logic_vector(7 downto 0);

  signal v8_TxDataMoverHaltReq_s          : std_logic_vector(7 downto 0);
  signal v8_TxDataMoverHaltCmplt_s        : std_logic_vector(7 downto 0);
  signal v8_TxDataMoverErr_s              : std_logic_vector(7 downto 0);
  signal v8_TxDataMoverRst_s              : std_logic_vector(7 downto 0);

  signal v32_dataMoverCtrlCh0_s  	        : std_logic_vector(31 downto 0);
  signal v32_dataMoverAddrCh0_s           : std_logic_vector(31 downto 0);
  signal v4_dataMoverTagCh0_s             : std_logic_vector(3 downto 0);
  signal v8_dataMoverStatusCh0_s          : std_logic_vector(7 downto 0);
  signal v24_transferCntCh0_s             : std_logic_vector(23 downto 0);
  signal v24_currentTransferCntCh0_s      : std_logic_vector(23 downto 0);
      
  signal v32_dataMoverCtrlCh1_s  	        : std_logic_vector(31 downto 0);
  signal v32_dataMoverAddrCh1_s           : std_logic_vector(31 downto 0);
  signal v4_dataMoverTagCh1_s             : std_logic_vector(3 downto 0);
  signal v8_dataMoverStatusCh1_s          : std_logic_vector(7 downto 0);
  signal v24_transferCntCh1_s             : std_logic_vector(23 downto 0);
  signal v24_currentTransferCntCh1_s      : std_logic_vector(23 downto 0);

  signal v32_dataMoverCtrlCh2_s  	        : std_logic_vector(31 downto 0);
  signal v32_dataMoverAddrCh2_s           : std_logic_vector(31 downto 0);
  signal v4_dataMoverTagCh2_s             : std_logic_vector(3 downto 0);
  signal v8_dataMoverStatusCh2_s          : std_logic_vector(7 downto 0);
  signal v24_transferCntCh2_s             : std_logic_vector(23 downto 0);
  signal v24_currentTransferCntCh2_s      : std_logic_vector(23 downto 0);

  signal v32_dataMoverCtrlCh3_s  	        : std_logic_vector(31 downto 0);
  signal v32_dataMoverAddrCh3_s           : std_logic_vector(31 downto 0);
  signal v4_dataMoverTagCh3_s             : std_logic_vector(3 downto 0);
  signal v8_dataMoverStatusCh3_s          : std_logic_vector(7 downto 0);
  signal v24_transferCntCh3_s             : std_logic_vector(23 downto 0);
  signal v24_currentTransferCntCh3_s      : std_logic_vector(23 downto 0);

  signal v32_dataMoverCtrlCh4_s  	        : std_logic_vector(31 downto 0);
  signal v32_dataMoverAddrCh4_s           : std_logic_vector(31 downto 0);
  signal v4_dataMoverTagCh4_s             : std_logic_vector(3 downto 0);
  signal v8_dataMoverStatusCh4_s          : std_logic_vector(7 downto 0);
  signal v24_transferCntCh4_s             : std_logic_vector(23 downto 0);
  signal v24_currentTransferCntCh4_s      : std_logic_vector(23 downto 0);

  signal v32_dataMoverCtrlCh5_s  	        : std_logic_vector(31 downto 0);
  signal v32_dataMoverAddrCh5_s           : std_logic_vector(31 downto 0);
  signal v4_dataMoverTagCh5_s             : std_logic_vector(3 downto 0);
  signal v8_dataMoverStatusCh5_s          : std_logic_vector(7 downto 0);
  signal v24_transferCntCh5_s             : std_logic_vector(23 downto 0);
  signal v24_currentTransferCntCh5_s      : std_logic_vector(23 downto 0);

  signal v32_dataMoverCtrlCh6_s  	        : std_logic_vector(31 downto 0);
  signal v32_dataMoverAddrCh6_s           : std_logic_vector(31 downto 0);
  signal v4_dataMoverTagCh6_s             : std_logic_vector(3 downto 0);
  signal v8_dataMoverStatusCh6_s          : std_logic_vector(7 downto 0);
  signal v24_transferCntCh6_s             : std_logic_vector(23 downto 0);
  signal v24_currentTransferCntCh6_s      : std_logic_vector(23 downto 0);

  signal v32_dataMoverCtrlCh7_s  	        : std_logic_vector(31 downto 0);
  signal v32_dataMoverAddrCh7_s           : std_logic_vector(31 downto 0);
  signal v4_dataMoverTagCh7_s             : std_logic_vector(3 downto 0);
  signal v8_dataMoverStatusCh7_s          : std_logic_vector(7 downto 0);
  signal v24_transferCntCh7_s             : std_logic_vector(23 downto 0);
  signal v24_currentTransferCntCh7_s      : std_logic_vector(23 downto 0);

  signal v8_intReq_s                      : std_logic_vector(7 downto 0);

begin

  ------------------------------------------
  -- instantiate axi_slave_burst for fifo interface
  ------------------------------------------
  AXI_SLAVE_BURST_I : entity axi_slave_burst_v1_00_a.axi_slave_burst
    generic map
    (
      C_S_AXI_DATA_WIDTH             => IPIF_SLV_DWIDTH,
      C_S_AXI_ADDR_WIDTH             => C_S_AXI_ADDR_WIDTH,
      C_S_AXI_ID_WIDTH               => C_S_AXI_ID_WIDTH,
      C_RDATA_FIFO_DEPTH             => C_RDATA_FIFO_DEPTH,
      C_INCLUDE_TIMEOUT_CNT          => C_INCLUDE_TIMEOUT_CNT,
      C_TIMEOUT_CNTR_VAL             => C_TIMEOUT_CNTR_VAL,
      C_ALIGN_BE_RDADDR              => C_ALIGN_BE_RDADDR,
      C_S_AXI_SUPPORTS_WRITE         => C_S_AXI_SUPPORTS_WRITE,
      C_S_AXI_SUPPORTS_READ          => C_S_AXI_SUPPORTS_READ,
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
      S_AXI_AWID                     => S_AXI_AWID,
      S_AXI_AWLEN                    => S_AXI_AWLEN,
      S_AXI_AWSIZE                   => S_AXI_AWSIZE,
      S_AXI_AWBURST                  => S_AXI_AWBURST,
      S_AXI_AWLOCK                   => S_AXI_AWLOCK,
      S_AXI_AWCACHE                  => S_AXI_AWCACHE,
      S_AXI_AWPROT                   => S_AXI_AWPROT,
      S_AXI_WLAST                    => S_AXI_WLAST,
      S_AXI_BID                      => S_AXI_BID,
      S_AXI_ARID                     => S_AXI_ARID,
      S_AXI_ARLEN                    => S_AXI_ARLEN,
      S_AXI_ARSIZE                   => S_AXI_ARSIZE,
      S_AXI_ARBURST                  => S_AXI_ARBURST,
      S_AXI_ARLOCK                   => S_AXI_ARLOCK,
      S_AXI_ARCACHE                  => S_AXI_ARCACHE,
      S_AXI_ARPROT                   => S_AXI_ARPROT,
      S_AXI_RID                      => S_AXI_RID,
      S_AXI_RLAST                    => S_AXI_RLAST,
      Bus2IP_Clk                     => ipif_Bus2IP_Clk,
      Bus2IP_Resetn                  => ipif_Bus2IP_Resetn,
      Bus2IP_Addr                    => ipif_Bus2IP_Addr,
      Bus2IP_RNW                     => ipif_Bus2IP_RNW,
      Bus2IP_BE                      => ipif_Bus2IP_BE,
      Bus2IP_CS                      => ipif_Bus2IP_CS,
      Bus2IP_RdCE                    => ipif_Bus2IP_RdCE,
      Bus2IP_WrCE                    => ipif_Bus2IP_WrCE,
      Bus2IP_Data                    => ipif_Bus2IP_Data,
      Bus2IP_Burst                   => ipif_Bus2IP_Burst,
      Bus2IP_BurstLength             => ipif_Bus2IP_BurstLength,
      Bus2IP_WrReq                   => ipif_Bus2IP_WrReq,
      Bus2IP_RdReq                   => ipif_Bus2IP_RdReq,
      IP2Bus_AddrAck                 => ipif_IP2Bus_AddrAck,
      IP2Bus_RdAck                   => ipif_IP2Bus_RdAck,
      IP2Bus_WrAck                   => ipif_IP2Bus_WrAck,
      IP2Bus_Error                   => ipif_IP2Bus_Error,
      IP2Bus_Data                    => ipif_IP2Bus_Data,
      Type_of_xfer                   => ipif_Type_of_xfer
    );

  ------------------------------------------
  -- instantiate User Logic for fifo interface
  ------------------------------------------
  USER_LOGIC_FIFO_I : entity lyt_axi_pcie_rtdex_v1_00_a.user_logic_fifo
    generic map
    (
      -- MAP USER GENERICS BELOW THIS LINE ---------------
      --USER generics mapped here
      RTDEX_RX_NUMBER_OF_CHANNELS_g => C_RTDEX_RX_NUMBER_OF_CHANNELS,
      READ_DEPTH_g                  => C_RX_CH_FIFO_DEPTH,
      C_RX_WIDTH_CH0                => C_RX_WIDTH_CH0,
      C_RX_WIDTH_CH1                => C_RX_WIDTH_CH1,
      C_RX_WIDTH_CH2                => C_RX_WIDTH_CH2,
      C_RX_WIDTH_CH3                => C_RX_WIDTH_CH3,
      C_RX_WIDTH_CH4                => C_RX_WIDTH_CH4,
      C_RX_WIDTH_CH5                => C_RX_WIDTH_CH5,
      C_RX_WIDTH_CH6                => C_RX_WIDTH_CH6,
      C_RX_WIDTH_CH7                => C_RX_WIDTH_CH7,
      -- MAP USER GENERICS ABOVE THIS LINE ---------------

      C_SLV_AWIDTH                   => USER_SLV_AWIDTH,
      C_SLV_DWIDTH                   => USER_SLV_DWIDTH,
      C_NUM_MEM                      => USER_NUM_MEM
    )
    port map
    (
      -- MAP USER PORTS BELOW THIS LINE ------------------
      --USER ports mapped here
      
      -- Config & status signals from/to axi registers
      --
      iv8_RxFifoReset_p              => v8_RxFifoReset_s,
      i_RxReset_p                    => '0',
      iv8_RxWordCntReset_p           => v8_RxWordCntReset_s,
      iv8_RxFifoRdEn_p               => v8_RxFifoRdEn_s,
      iv8_RxFifoWrEn_p               => v8_RxFifoWrEn_s,
      ov8_overflow_p                 => v8_RxFifoOverflow_s,
      ov8_underflow_p                => v8_RxFifoUnderflow_s,
      ov32_RxConfigInfo_p            => v32_RxConfigInfo_s,
      ov32_RcvdWordCntCh0_p          => v32_RcvdWordCntCh0_s,
      ov32_RcvdWordCntCh1_p          => v32_RcvdWordCntCh1_s,
      ov32_RcvdWordCntCh2_p          => v32_RcvdWordCntCh2_s,
      ov32_RcvdWordCntCh3_p          => v32_RcvdWordCntCh3_s,
      ov32_RcvdWordCntCh4_p          => v32_RcvdWordCntCh4_s,
      ov32_RcvdWordCntCh5_p          => v32_RcvdWordCntCh5_s,
      ov32_RcvdWordCntCh6_p          => v32_RcvdWordCntCh6_s,
      ov32_RcvdWordCntCh7_p          => v32_RcvdWordCntCh7_s,
      
      -- RX User ports 
      --           
      i_RxUserClk_p                  => i_RxUserClk_p,
      o_RxReadyCh0_p                 => o_RxReadyCh0_p,
      i_RxReCh0_p                    => i_RxReCh0_p,
      ov_RxDataCh0_p                 => ov_RxDataCh0_p,
      o_RxDataValidCh0_p             => o_RxDataValidCh0_p,    
      o_RxReadyCh1_p                 => o_RxReadyCh1_p,
      i_RxReCh1_p                    => i_RxReCh1_p,
      ov_RxDataCh1_p                 => ov_RxDataCh1_p,
      o_RxDataValidCh1_p             => o_RxDataValidCh1_p,
      o_RxReadyCh2_p                 => o_RxReadyCh2_p,
      i_RxReCh2_p                    => i_RxReCh2_p,
      ov_RxDataCh2_p                 => ov_RxDataCh2_p,
      o_RxDataValidCh2_p             => o_RxDataValidCh2_p,
      o_RxReadyCh3_p                 => o_RxReadyCh3_p,
      i_RxReCh3_p                    => i_RxReCh3_p,
      ov_RxDataCh3_p                 => ov_RxDataCh3_p,
      o_RxDataValidCh3_p             => o_RxDataValidCh3_p,
      o_RxReadyCh4_p                 => o_RxReadyCh4_p,
      i_RxReCh4_p                    => i_RxReCh4_p,
      ov_RxDataCh4_p                 => ov_RxDataCh4_p,
      o_RxDataValidCh4_p             => o_RxDataValidCh4_p,
      o_RxReadyCh5_p                 => o_RxReadyCh5_p,
      i_RxReCh5_p                    => i_RxReCh5_p,
      ov_RxDataCh5_p                 => ov_RxDataCh5_p,
      o_RxDataValidCh5_p             => o_RxDataValidCh5_p,
      o_RxReadyCh6_p                 => o_RxReadyCh6_p,
      i_RxReCh6_p                    => i_RxReCh6_p,
      ov_RxDataCh6_p                 => ov_RxDataCh6_p,
      o_RxDataValidCh6_p             => o_RxDataValidCh6_p,
      o_RxReadyCh7_p                 => o_RxReadyCh7_p,
      i_RxReCh7_p                    => i_RxReCh7_p,
      ov_RxDataCh7_p                 => ov_RxDataCh7_p,
      o_RxDataValidCh7_p             => o_RxDataValidCh7_p,
    
      -- MAP USER PORTS ABOVE THIS LINE ------------------

      Bus2IP_Clk                     => ipif_Bus2IP_Clk,
      Bus2IP_Resetn                  => ipif_Bus2IP_Resetn,
      Bus2IP_Addr                    => ipif_Bus2IP_Addr,
      Bus2IP_CS                      => ipif_Bus2IP_CS(USER_NUM_MEM-1 downto 0),
      Bus2IP_RNW                     => ipif_Bus2IP_RNW,
      Bus2IP_Data                    => ipif_Bus2IP_Data,
      Bus2IP_BE                      => ipif_Bus2IP_BE,
      Bus2IP_RdCE                    => ipif_Bus2IP_RdCE,
      Bus2IP_WrCE                    => ipif_Bus2IP_WrCE,
      Bus2IP_Burst                   => ipif_Bus2IP_Burst,
      Bus2IP_BurstLength             => user_Bus2IP_BurstLength,
      Bus2IP_RdReq                   => ipif_Bus2IP_RdReq,
      Bus2IP_WrReq                   => ipif_Bus2IP_WrReq,
      IP2Bus_AddrAck                 => user_IP2Bus_AddrAck,
      IP2Bus_Data                    => user_IP2Bus_Data,
      IP2Bus_RdAck                   => user_IP2Bus_RdAck,
      IP2Bus_WrAck                   => user_IP2Bus_WrAck,
      IP2Bus_Error                   => user_IP2Bus_Error,
      Type_of_xfer                   => user_Type_of_xfer
    );

  ------------------------------------------
  -- connect internal signals for fifo interface
  ------------------------------------------
  IP2BUS_DATA_MUX_PROC : process( ipif_Bus2IP_CS, user_IP2Bus_Data ) is
  begin

    case ipif_Bus2IP_CS is
      when "10000000" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when "01000000" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when "00100000" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when "00010000" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when "00001000" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when "00000100" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when "00000010" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when "00000001" => ipif_IP2Bus_Data <= user_IP2Bus_Data;
      when others => ipif_IP2Bus_Data <= (others => '0');
    end case;

  end process IP2BUS_DATA_MUX_PROC;

  ipif_IP2Bus_AddrAck <= user_IP2Bus_AddrAck;
  ipif_IP2Bus_WrAck <= user_IP2Bus_WrAck;
  ipif_IP2Bus_RdAck <= user_IP2Bus_RdAck;
  ipif_IP2Bus_Error <= user_IP2Bus_Error;

  user_Bus2IP_BurstLength(7 downto 0)<= ipif_Bus2IP_BurstLength(7 downto 0);


  ------------------------------------------
  -- instantiate axi_lite_ipif for register interface
  ------------------------------------------
  AXI_LITE_IPIF_I : entity axi_lite_ipif_v1_01_a.axi_lite_ipif
    generic map
    (
      C_S_AXI_DATA_WIDTH             => IPIF_SLV_REG_DWIDTH,
      C_S_AXI_ADDR_WIDTH             => C_S_AXI_REG_ADDR_WIDTH,
      C_S_AXI_MIN_SIZE               => C_S_AXI_REG_MIN_SIZE,
      C_USE_WSTRB                    => C_REG_USE_WSTRB,
      C_DPHASE_TIMEOUT               => C_REG_DPHASE_TIMEOUT,
      C_ARD_ADDR_RANGE_ARRAY         => IPIF_REG_ARD_ADDR_RANGE_ARRAY,
      C_ARD_NUM_CE_ARRAY             => IPIF_REG_ARD_NUM_CE_ARRAY,
      C_FAMILY                       => C_FAMILY
    )
    port map
    (
      S_AXI_ACLK                     => S_AXI_REG_ACLK,
      S_AXI_ARESETN                  => S_AXI_REG_ARESETN,
      S_AXI_AWADDR                   => S_AXI_REG_AWADDR,
      S_AXI_AWVALID                  => S_AXI_REG_AWVALID,
      S_AXI_WDATA                    => S_AXI_REG_WDATA,
      S_AXI_WSTRB                    => S_AXI_REG_WSTRB,
      S_AXI_WVALID                   => S_AXI_REG_WVALID,
      S_AXI_BREADY                   => S_AXI_REG_BREADY,
      S_AXI_ARADDR                   => S_AXI_REG_ARADDR,
      S_AXI_ARVALID                  => S_AXI_REG_ARVALID,
      S_AXI_RREADY                   => S_AXI_REG_RREADY,
      S_AXI_ARREADY                  => S_AXI_REG_ARREADY,
      S_AXI_RDATA                    => S_AXI_REG_RDATA,
      S_AXI_RRESP                    => S_AXI_REG_RRESP,
      S_AXI_RVALID                   => S_AXI_REG_RVALID,
      S_AXI_WREADY                   => S_AXI_REG_WREADY,
      S_AXI_BRESP                    => S_AXI_REG_BRESP,
      S_AXI_BVALID                   => S_AXI_REG_BVALID,
      S_AXI_AWREADY                  => S_AXI_REG_AWREADY,
      Bus2IP_Clk                     => ipif_reg_Bus2IP_Clk,
      Bus2IP_Resetn                  => ipif_reg_Bus2IP_Resetn,
      Bus2IP_Addr                    => ipif_reg_Bus2IP_Addr,
      Bus2IP_RNW                     => ipif_reg_Bus2IP_RNW,
      Bus2IP_BE                      => ipif_reg_Bus2IP_BE,
      Bus2IP_CS                      => ipif_reg_Bus2IP_CS,
      Bus2IP_RdCE                    => ipif_reg_Bus2IP_RdCE,
      Bus2IP_WrCE                    => ipif_reg_Bus2IP_WrCE,
      Bus2IP_Data                    => ipif_reg_Bus2IP_Data,
      IP2Bus_WrAck                   => ipif_reg_IP2Bus_WrAck,
      IP2Bus_RdAck                   => ipif_reg_IP2Bus_RdAck,
      IP2Bus_Error                   => ipif_reg_IP2Bus_Error,
      IP2Bus_Data                    => ipif_reg_IP2Bus_Data
    );

  -- CDMA Present logic
  --
  cdmaPresent_s <= '1' when C_CDMA_PRESENT = 1 else '0';

  ------------------------------------------
  -- instantiate User Logic for register interface
  ------------------------------------------
  USER_LOGIC_REG_I : entity lyt_axi_pcie_rtdex_v1_00_a.user_logic_reg
  generic map (
     -- MAP USER GENERICS BELOW THIS LINE ---------------
     --USER generics mapped here
     -- MAP USER GENERICS ABOVE THIS LINE ---------------

     C_NUM_REG                      => USER_REG_NUM_REG,
     C_SLV_DWIDTH                   => USER_SLV_REG_DWIDTH
  )
  port map (
     -- user_logic entity ports mapping  ---------------
    i_logicRst_p                    => logicRst_s,

    o_CoreResetPulse_p              => CoreResetPulse_s,
    o_mbIrq_p                       => o_mbIrq_p,
    i_cdmaPresent_p                 => cdmaPresent_s,

    ov8_RxWordCntReset_p            => v8_RxWordCntReset_s,
    ov8_TxFifoReset_p               => v8_TxFifoReset_s,
    ov8_RxFifoReset_p               => v8_RxFifoReset_s,
    iv32_RxConfigInfo_p             => v32_RxConfigInfo_s,
    iv8_RxFifoOverflow_p            => v8_RxFifoOverflow_s,
    iv8_RxFifoUnderflow_p           => v8_RxFifoUnderflow_s,
    ov8_TxStreamingTransfer_p       => v8_streamingTransfer_s,
    ov8_RxFifoRdEn_p                => v8_RxFifoRdEn_s,
    ov8_TxFifoWrEn_p                => v8_TxFifoWrEn_s,
    ov8_RxFifoWrEn_p                => v8_RxFifoWrEn_s,
    iv32_RcvdWordCntCh0_p           => v32_RcvdWordCntCh0_s,
    iv32_RcvdWordCntCh1_p           => v32_RcvdWordCntCh1_s,
    iv32_RcvdWordCntCh2_p           => v32_RcvdWordCntCh2_s,
    iv32_RcvdWordCntCh3_p           => v32_RcvdWordCntCh3_s,
    iv32_RcvdWordCntCh4_p           => v32_RcvdWordCntCh4_s,
    iv32_RcvdWordCntCh5_p           => v32_RcvdWordCntCh5_s,
    iv32_RcvdWordCntCh6_p           => v32_RcvdWordCntCh6_s,
    iv32_RcvdWordCntCh7_p           => v32_RcvdWordCntCh7_s,
    iv32_TxConfigInfo_p             => v32_TxConfigInfo_s,
    iv8_TxFifoOverflow_p            => v8_TxFifoOverflow_s,
    iv8_TxFifoUnderflow_p           => v8_TxFifoUnderflow_s,
    ov8_TxStartNewTransfer_p        => v8_TxStartNewTransfer_s,
    iv8_TxTransferDone_p  	        => v8_TxTransferDone_s, 
    ov8_TxIrqLastTransferEn_p       => v8_irqLastTransferEn_s,
    ov8_TxDataMoverHaltReq_p        => v8_TxDataMoverHaltReq_s,
    iv8_TxDataMoverHaltCmplt_p      => v8_TxDataMoverHaltCmplt_s,
    iv8_TxDataMoverErr_p            => v8_TxDataMoverErr_s,
    ov8_TxDataMoverRst_p            => v8_TxDataMoverRst_s,
    
    ov24_TxTransferCntCh0_p         => v24_transferCntCh0_s,
    iv24_TxCurrentTransferCntCh0_p  => v24_currentTransferCntCh0_s,
    ov32_TxDataMoverCtrlCh0_p       => v32_dataMoverCtrlCh0_s,
    ov32_TxDataMoverAddrCh0_p       => v32_dataMoverAddrCh0_s,
    ov4_TxDataMoverTagCh0_p         => v4_dataMoverTagCh0_s,
    iv8_TxDataMoverStatusCh0_p      => v8_dataMoverStatusCh0_s,
    
    ov24_TxTransferCntCh1_p         => v24_transferCntCh1_s,
    iv24_TxCurrentTransferCntCh1_p  => v24_currentTransferCntCh1_s,
    ov32_TxDataMoverCtrlCh1_p       => v32_dataMoverCtrlCh1_s,
    ov32_TxDataMoverAddrCh1_p       => v32_dataMoverAddrCh1_s,
    ov4_TxDataMoverTagCh1_p         => v4_dataMoverTagCh1_s,
    iv8_TxDataMoverStatusCh1_p      => v8_dataMoverStatusCh1_s,
    
    ov24_TxTransferCntCh2_p         => v24_transferCntCh2_s,
    iv24_TxCurrentTransferCntCh2_p  => v24_currentTransferCntCh2_s,
    ov32_TxDataMoverCtrlCh2_p       => v32_dataMoverCtrlCh2_s,
    ov32_TxDataMoverAddrCh2_p       => v32_dataMoverAddrCh2_s,
    ov4_TxDataMoverTagCh2_p         => v4_dataMoverTagCh2_s,
    iv8_TxDataMoverStatusCh2_p      => v8_dataMoverStatusCh2_s,
    
    ov24_TxTransferCntCh3_p         => v24_transferCntCh3_s,
    iv24_TxCurrentTransferCntCh3_p  => v24_currentTransferCntCh3_s,
    ov32_TxDataMoverCtrlCh3_p       => v32_dataMoverCtrlCh3_s,
    ov32_TxDataMoverAddrCh3_p       => v32_dataMoverAddrCh3_s,
    ov4_TxDataMoverTagCh3_p         => v4_dataMoverTagCh3_s,
    iv8_TxDataMoverStatusCh3_p      => v8_dataMoverStatusCh3_s,
    
    ov24_TxTransferCntCh4_p         => v24_transferCntCh4_s,
    iv24_TxCurrentTransferCntCh4_p  => v24_currentTransferCntCh4_s,
    ov32_TxDataMoverCtrlCh4_p       => v32_dataMoverCtrlCh4_s,
    ov32_TxDataMoverAddrCh4_p       => v32_dataMoverAddrCh4_s,
    ov4_TxDataMoverTagCh4_p         => v4_dataMoverTagCh4_s,
    iv8_TxDataMoverStatusCh4_p      => v8_dataMoverStatusCh4_s,
    
    ov24_TxTransferCntCh5_p         => v24_transferCntCh5_s,
    iv24_TxCurrentTransferCntCh5_p  => v24_currentTransferCntCh5_s,
    ov32_TxDataMoverCtrlCh5_p       => v32_dataMoverCtrlCh5_s,
    ov32_TxDataMoverAddrCh5_p       => v32_dataMoverAddrCh5_s,
    ov4_TxDataMoverTagCh5_p         => v4_dataMoverTagCh5_s,
    iv8_TxDataMoverStatusCh5_p      => v8_dataMoverStatusCh5_s,
    
    ov24_TxTransferCntCh6_p         => v24_transferCntCh6_s,
    iv24_TxCurrentTransferCntCh6_p  => v24_currentTransferCntCh6_s,
    ov32_TxDataMoverCtrlCh6_p       => v32_dataMoverCtrlCh6_s,
    ov32_TxDataMoverAddrCh6_p       => v32_dataMoverAddrCh6_s,
    ov4_TxDataMoverTagCh6_p         => v4_dataMoverTagCh6_s,
    iv8_TxDataMoverStatusCh6_p      => v8_dataMoverStatusCh6_s,
    
    ov24_TxTransferCntCh7_p         => v24_transferCntCh7_s,
    iv24_TxCurrentTransferCntCh7_p  => v24_currentTransferCntCh7_s,
    ov32_TxDataMoverCtrlCh7_p       => v32_dataMoverCtrlCh7_s,
    ov32_TxDataMoverAddrCh7_p       => v32_dataMoverAddrCh7_s,
    ov4_TxDataMoverTagCh7_p         => v4_dataMoverTagCh7_s,
    iv8_TxDataMoverStatusCh7_p      => v8_dataMoverStatusCh7_s,
    
    Bus2IP_Clk                      => ipif_reg_Bus2IP_Clk,
    Bus2IP_Resetn                   => ipif_reg_Bus2IP_Resetn,
    Bus2IP_Data                     => ipif_reg_Bus2IP_Data,
    Bus2IP_BE                       => ipif_reg_Bus2IP_BE,
    Bus2IP_RdCE                     => user_reg_Bus2IP_RdCE,
    Bus2IP_WrCE                     => user_reg_Bus2IP_WrCE,
    IP2Bus_Data                     => user_reg_IP2Bus_Data,
    IP2Bus_RdAck                    => user_reg_IP2Bus_RdAck,
    IP2Bus_WrAck                    => user_reg_IP2Bus_WrAck,
    IP2Bus_Error                    => user_reg_IP2Bus_Error
  );
 
  ------------------------------------------
  -- connect internal signals for register interface
  ------------------------------------------
  ipif_reg_IP2Bus_Data <= user_reg_IP2Bus_Data;
  ipif_reg_IP2Bus_WrAck <= user_reg_IP2Bus_WrAck;
  ipif_reg_IP2Bus_RdAck <= user_reg_IP2Bus_RdAck;
  ipif_reg_IP2Bus_Error <= user_reg_IP2Bus_Error;

  user_reg_Bus2IP_RdCE <= ipif_reg_Bus2IP_RdCE(USER_REG_NUM_REG-1 downto 0);
  user_reg_Bus2IP_WrCE <= ipif_reg_Bus2IP_WrCE(USER_REG_NUM_REG-1 downto 0);
   
   
  ------------------------------------------
  -- instantiate User Logic for register interface
  ------------------------------------------
  
  rtdex_tx_I : entity lyt_axi_pcie_rtdex_v1_00_a.rtdex_tx
    generic map (         
      NumberChannels_g 		        => C_RTDEX_TX_NUMBER_OF_CHANNELS,
      WRITE_DEPTH_g                 => C_TX_CH_FIFO_DEPTH,
      C_TX_WIDTH_CH0                => C_TX_WIDTH_CH0,
      C_TX_WIDTH_CH1                => C_TX_WIDTH_CH1,
      C_TX_WIDTH_CH2                => C_TX_WIDTH_CH2,
      C_TX_WIDTH_CH3                => C_TX_WIDTH_CH3,
      C_TX_WIDTH_CH4                => C_TX_WIDTH_CH4,
      C_TX_WIDTH_CH5                => C_TX_WIDTH_CH5,
      C_TX_WIDTH_CH6                => C_TX_WIDTH_CH6,
      C_TX_WIDTH_CH7                => C_TX_WIDTH_CH7
    )
    port map (
      ov8_intReq_p                  => v8_intReq_s,
    
      -- config & status signals from/to axi registers --
      Bus2IP_Clk                    => ipif_reg_Bus2IP_Clk,

      i_TxReset_p             	    => '0',
      iv8_TxFifoReset_p       	    => v8_TxFifoReset_s,
      iv8_TxFifoWrEn_p              => v8_TxFifoWrEn_s,
      ov8_overflow_p                => v8_TxFifoOverflow_s,
      ov8_underflow_p               => v8_TxFifoUnderflow_s,
      iv8_streamingTransfer_p	      => v8_streamingTransfer_s,
      iv8_StartNewTransfer_p  	    => v8_TxStartNewTransfer_s,
      ov8_transferDone_p  	        => v8_TxTransferDone_s,
      iv8_irqLastTransferEn_p	      => v8_irqLastTransferEn_s,
      ov8_TxDataMoverHaltCmplt_p    => v8_TxDataMoverHaltCmplt_s,
      ov8_TxDataMoverErr_p          => v8_TxDataMoverErr_s,
      iv8_TxDataMoverRst_p          => v8_TxDataMoverRst_s,
      iv8_TxDataMoverHaltReq_p      => v8_TxDataMoverHaltReq_s,
      ov32_TxConfigInfo_p     	    => v32_TxConfigInfo_s,

      iv32_dataMoverCtrlCh0_p  	    => v32_dataMoverCtrlCh0_s,
      iv32_dataMoverAddrCh0_p       => v32_dataMoverAddrCh0_s,
      iv4_dataMoverTagCh0_p         => v4_dataMoverTagCh0_s ,
      ov8_dataMoverStatusCh0_p      => v8_dataMoverStatusCh0_s,
      iv24_transferCntCh0_p   	    => v24_transferCntCh0_s,
      ov24_currentTransferCntCh0_p  => v24_currentTransferCntCh0_s,

      iv32_dataMoverCtrlCh1_p  	    => v32_dataMoverCtrlCh1_s,
      iv32_dataMoverAddrCh1_p       => v32_dataMoverAddrCh1_s,
      iv4_dataMoverTagCh1_p         => v4_dataMoverTagCh1_s ,
      ov8_dataMoverStatusCh1_p      => v8_dataMoverStatusCh1_s,
      iv24_transferCntCh1_p   	    => v24_transferCntCh1_s,
      ov24_currentTransferCntCh1_p  => v24_currentTransferCntCh1_s,

      iv32_dataMoverCtrlCh2_p  	    => v32_dataMoverCtrlCh2_s,
      iv32_dataMoverAddrCh2_p       => v32_dataMoverAddrCh2_s,
      iv4_dataMoverTagCh2_p         => v4_dataMoverTagCh2_s ,
      ov8_dataMoverStatusCh2_p      => v8_dataMoverStatusCh2_s,
      iv24_transferCntCh2_p   	    => v24_transferCntCh2_s,
      ov24_currentTransferCntCh2_p  => v24_currentTransferCntCh2_s,

      iv32_dataMoverCtrlCh3_p  	    => v32_dataMoverCtrlCh3_s,
      iv32_dataMoverAddrCh3_p       => v32_dataMoverAddrCh3_s,
      iv4_dataMoverTagCh3_p         => v4_dataMoverTagCh3_s ,
      ov8_dataMoverStatusCh3_p      => v8_dataMoverStatusCh3_s,
      iv24_transferCntCh3_p   	    => v24_transferCntCh3_s,
      ov24_currentTransferCntCh3_p  => v24_currentTransferCntCh3_s,

      iv32_dataMoverCtrlCh4_p  	    => v32_dataMoverCtrlCh4_s,
      iv32_dataMoverAddrCh4_p       => v32_dataMoverAddrCh4_s,
      iv4_dataMoverTagCh4_p         => v4_dataMoverTagCh4_s ,
      ov8_dataMoverStatusCh4_p      => v8_dataMoverStatusCh4_s,
      iv24_transferCntCh4_p   	    => v24_transferCntCh4_s,
      ov24_currentTransferCntCh4_p  => v24_currentTransferCntCh4_s,

      iv32_dataMoverCtrlCh5_p  	    => v32_dataMoverCtrlCh5_s,
      iv32_dataMoverAddrCh5_p       => v32_dataMoverAddrCh5_s,
      iv4_dataMoverTagCh5_p         => v4_dataMoverTagCh5_s ,
      ov8_dataMoverStatusCh5_p      => v8_dataMoverStatusCh5_s,
      iv24_transferCntCh5_p   	    => v24_transferCntCh5_s,
      ov24_currentTransferCntCh5_p  => v24_currentTransferCntCh5_s,

      iv32_dataMoverCtrlCh6_p  	    => v32_dataMoverCtrlCh6_s,
      iv32_dataMoverAddrCh6_p       => v32_dataMoverAddrCh6_s,
      iv4_dataMoverTagCh6_p         => v4_dataMoverTagCh6_s ,
      ov8_dataMoverStatusCh6_p      => v8_dataMoverStatusCh6_s,
      iv24_transferCntCh6_p   	    => v24_transferCntCh6_s,
      ov24_currentTransferCntCh6_p  => v24_currentTransferCntCh6_s,

      iv32_dataMoverCtrlCh7_p  	    => v32_dataMoverCtrlCh7_s,
      iv32_dataMoverAddrCh7_p       => v32_dataMoverAddrCh7_s,
      iv4_dataMoverTagCh7_p         => v4_dataMoverTagCh7_s ,
      ov8_dataMoverStatusCh7_p      => v8_dataMoverStatusCh7_s,
      iv24_transferCntCh7_p   	    => v24_transferCntCh7_s,
      ov24_currentTransferCntCh7_p  => v24_currentTransferCntCh7_s,
      
      --- User side ---
      i_TxUserClk_p                 => i_TxUserClk_p,

      i_TxWeCh0_p                   => i_TxWeCh0_p,
      o_TxReadyCh0_p                => o_TxReadyCh0_p,
      iv_TxDataCh0_p                => iv_TxDataCh0_p,

      i_TxWeCh1_p                   => i_TxWeCh1_p,
      o_TxReadyCh1_p                => o_TxReadyCh1_p,
      iv_TxDataCh1_p                => iv_TxDataCh1_p,

      i_TxWeCh2_p                   => i_TxWeCh2_p,
      o_TxReadyCh2_p                => o_TxReadyCh2_p,
      iv_TxDataCh2_p                => iv_TxDataCh2_p,

      i_TxWeCh3_p                   => i_TxWeCh3_p,
      o_TxReadyCh3_p                => o_TxReadyCh3_p,
      iv_TxDataCh3_p                => iv_TxDataCh3_p,

      i_TxWeCh4_p                   => i_TxWeCh4_p,
      o_TxReadyCh4_p                => o_TxReadyCh4_p,
      iv_TxDataCh4_p                => iv_TxDataCh4_p,

      i_TxWeCh5_p                   => i_TxWeCh5_p,
      o_TxReadyCh5_p                => o_TxReadyCh5_p,
      iv_TxDataCh5_p                => iv_TxDataCh5_p,

      i_TxWeCh6_p                   => i_TxWeCh6_p,
      o_TxReadyCh6_p                => o_TxReadyCh6_p,
      iv_TxDataCh6_p                => iv_TxDataCh6_p,

      i_TxWeCh7_p                   => i_TxWeCh7_p,
      o_TxReadyCh7_p                => o_TxReadyCh7_p,
      iv_TxDataCh7_p                => iv_TxDataCh7_p,

      -- AXI Memory interface (Channel 0)
      m_axi_s2mm_Ch0_aclk           => m_axi_s2mm_Ch0_aclk,
      m_axi_s2mm_Ch0_aresetn        => m_axi_s2mm_Ch0_aresetn,
      m_axi_s2mm_Ch0_awid           => m_axi_s2mm_Ch0_awid,
      m_axi_s2mm_Ch0_awaddr         => m_axi_s2mm_Ch0_awaddr,
      m_axi_s2mm_Ch0_awlen          => m_axi_s2mm_Ch0_awlen,
      m_axi_s2mm_Ch0_awsize         => m_axi_s2mm_Ch0_awsize,
      m_axi_s2mm_Ch0_awburst        => m_axi_s2mm_Ch0_awburst,
      m_axi_s2mm_Ch0_awprot         => m_axi_s2mm_Ch0_awprot,
      m_axi_s2mm_Ch0_awcache        => m_axi_s2mm_Ch0_awcache,
      m_axi_s2mm_Ch0_awvalid        => m_axi_s2mm_Ch0_awvalid,
      m_axi_s2mm_Ch0_awready        => m_axi_s2mm_Ch0_awready,
      m_axi_s2mm_Ch0_wdata          => m_axi_s2mm_Ch0_wdata,
      m_axi_s2mm_Ch0_wstrb          => m_axi_s2mm_Ch0_wstrb,
      m_axi_s2mm_Ch0_wlast          => m_axi_s2mm_Ch0_wlast,
      m_axi_s2mm_Ch0_wvalid         => m_axi_s2mm_Ch0_wvalid,
      m_axi_s2mm_Ch0_wready         => m_axi_s2mm_Ch0_wready,
      m_axi_s2mm_Ch0_bresp          => m_axi_s2mm_Ch0_bresp,
      m_axi_s2mm_Ch0_bvalid         => m_axi_s2mm_Ch0_bvalid,
      m_axi_s2mm_Ch0_bready         => m_axi_s2mm_Ch0_bready,

      -- AXI Memory interface (Channel 1)
      m_axi_s2mm_Ch1_aclk           => m_axi_s2mm_Ch1_aclk,
      m_axi_s2mm_Ch1_aresetn        => m_axi_s2mm_Ch1_aresetn,
      m_axi_s2mm_Ch1_awid           => m_axi_s2mm_Ch1_awid,
      m_axi_s2mm_Ch1_awaddr         => m_axi_s2mm_Ch1_awaddr,
      m_axi_s2mm_Ch1_awlen          => m_axi_s2mm_Ch1_awlen,
      m_axi_s2mm_Ch1_awsize         => m_axi_s2mm_Ch1_awsize,
      m_axi_s2mm_Ch1_awburst        => m_axi_s2mm_Ch1_awburst,
      m_axi_s2mm_Ch1_awprot         => m_axi_s2mm_Ch1_awprot,
      m_axi_s2mm_Ch1_awcache        => m_axi_s2mm_Ch1_awcache,
      m_axi_s2mm_Ch1_awvalid        => m_axi_s2mm_Ch1_awvalid,
      m_axi_s2mm_Ch1_awready        => m_axi_s2mm_Ch1_awready,
      m_axi_s2mm_Ch1_wdata          => m_axi_s2mm_Ch1_wdata,
      m_axi_s2mm_Ch1_wstrb          => m_axi_s2mm_Ch1_wstrb,
      m_axi_s2mm_Ch1_wlast          => m_axi_s2mm_Ch1_wlast,
      m_axi_s2mm_Ch1_wvalid         => m_axi_s2mm_Ch1_wvalid,
      m_axi_s2mm_Ch1_wready         => m_axi_s2mm_Ch1_wready,
      m_axi_s2mm_Ch1_bresp          => m_axi_s2mm_Ch1_bresp,
      m_axi_s2mm_Ch1_bvalid         => m_axi_s2mm_Ch1_bvalid,
      m_axi_s2mm_Ch1_bready         => m_axi_s2mm_Ch1_bready,

      -- AXI Memory interface (Channel 2)
      m_axi_s2mm_Ch2_aclk           => m_axi_s2mm_Ch2_aclk,
      m_axi_s2mm_Ch2_aresetn        => m_axi_s2mm_Ch2_aresetn,
      m_axi_s2mm_Ch2_awid           => m_axi_s2mm_Ch2_awid,
      m_axi_s2mm_Ch2_awaddr         => m_axi_s2mm_Ch2_awaddr,
      m_axi_s2mm_Ch2_awlen          => m_axi_s2mm_Ch2_awlen,
      m_axi_s2mm_Ch2_awsize         => m_axi_s2mm_Ch2_awsize,
      m_axi_s2mm_Ch2_awburst        => m_axi_s2mm_Ch2_awburst,
      m_axi_s2mm_Ch2_awprot         => m_axi_s2mm_Ch2_awprot,
      m_axi_s2mm_Ch2_awcache        => m_axi_s2mm_Ch2_awcache,
      m_axi_s2mm_Ch2_awvalid        => m_axi_s2mm_Ch2_awvalid,
      m_axi_s2mm_Ch2_awready        => m_axi_s2mm_Ch2_awready,
      m_axi_s2mm_Ch2_wdata          => m_axi_s2mm_Ch2_wdata,
      m_axi_s2mm_Ch2_wstrb          => m_axi_s2mm_Ch2_wstrb,
      m_axi_s2mm_Ch2_wlast          => m_axi_s2mm_Ch2_wlast,
      m_axi_s2mm_Ch2_wvalid         => m_axi_s2mm_Ch2_wvalid,
      m_axi_s2mm_Ch2_wready         => m_axi_s2mm_Ch2_wready,
      m_axi_s2mm_Ch2_bresp          => m_axi_s2mm_Ch2_bresp,
      m_axi_s2mm_Ch2_bvalid         => m_axi_s2mm_Ch2_bvalid,
      m_axi_s2mm_Ch2_bready         => m_axi_s2mm_Ch2_bready,

      -- AXI Memory interface (Channel 3)
      m_axi_s2mm_Ch3_aclk           => m_axi_s2mm_Ch3_aclk,
      m_axi_s2mm_Ch3_aresetn        => m_axi_s2mm_Ch3_aresetn,
      m_axi_s2mm_Ch3_awid           => m_axi_s2mm_Ch3_awid,
      m_axi_s2mm_Ch3_awaddr         => m_axi_s2mm_Ch3_awaddr,
      m_axi_s2mm_Ch3_awlen          => m_axi_s2mm_Ch3_awlen,
      m_axi_s2mm_Ch3_awsize         => m_axi_s2mm_Ch3_awsize,
      m_axi_s2mm_Ch3_awburst        => m_axi_s2mm_Ch3_awburst,
      m_axi_s2mm_Ch3_awprot         => m_axi_s2mm_Ch3_awprot,
      m_axi_s2mm_Ch3_awcache        => m_axi_s2mm_Ch3_awcache,
      m_axi_s2mm_Ch3_awvalid        => m_axi_s2mm_Ch3_awvalid,
      m_axi_s2mm_Ch3_awready        => m_axi_s2mm_Ch3_awready,
      m_axi_s2mm_Ch3_wdata          => m_axi_s2mm_Ch3_wdata,
      m_axi_s2mm_Ch3_wstrb          => m_axi_s2mm_Ch3_wstrb,
      m_axi_s2mm_Ch3_wlast          => m_axi_s2mm_Ch3_wlast,
      m_axi_s2mm_Ch3_wvalid         => m_axi_s2mm_Ch3_wvalid,
      m_axi_s2mm_Ch3_wready         => m_axi_s2mm_Ch3_wready,
      m_axi_s2mm_Ch3_bresp          => m_axi_s2mm_Ch3_bresp,
      m_axi_s2mm_Ch3_bvalid         => m_axi_s2mm_Ch3_bvalid,
      m_axi_s2mm_Ch3_bready         => m_axi_s2mm_Ch3_bready,

      -- AXI Memory interface (Channel 4)
      m_axi_s2mm_Ch4_aclk           => m_axi_s2mm_Ch4_aclk,
      m_axi_s2mm_Ch4_aresetn        => m_axi_s2mm_Ch4_aresetn,
      m_axi_s2mm_Ch4_awid           => m_axi_s2mm_Ch4_awid,
      m_axi_s2mm_Ch4_awaddr         => m_axi_s2mm_Ch4_awaddr,
      m_axi_s2mm_Ch4_awlen          => m_axi_s2mm_Ch4_awlen,
      m_axi_s2mm_Ch4_awsize         => m_axi_s2mm_Ch4_awsize,
      m_axi_s2mm_Ch4_awburst        => m_axi_s2mm_Ch4_awburst,
      m_axi_s2mm_Ch4_awprot         => m_axi_s2mm_Ch4_awprot,
      m_axi_s2mm_Ch4_awcache        => m_axi_s2mm_Ch4_awcache,
      m_axi_s2mm_Ch4_awvalid        => m_axi_s2mm_Ch4_awvalid,
      m_axi_s2mm_Ch4_awready        => m_axi_s2mm_Ch4_awready,
      m_axi_s2mm_Ch4_wdata          => m_axi_s2mm_Ch4_wdata,
      m_axi_s2mm_Ch4_wstrb          => m_axi_s2mm_Ch4_wstrb,
      m_axi_s2mm_Ch4_wlast          => m_axi_s2mm_Ch4_wlast,
      m_axi_s2mm_Ch4_wvalid         => m_axi_s2mm_Ch4_wvalid,
      m_axi_s2mm_Ch4_wready         => m_axi_s2mm_Ch4_wready,
      m_axi_s2mm_Ch4_bresp          => m_axi_s2mm_Ch4_bresp,
      m_axi_s2mm_Ch4_bvalid         => m_axi_s2mm_Ch4_bvalid,
      m_axi_s2mm_Ch4_bready         => m_axi_s2mm_Ch4_bready,

      -- AXI Memory interface (Channel 5)
      m_axi_s2mm_Ch5_aclk           => m_axi_s2mm_Ch5_aclk,
      m_axi_s2mm_Ch5_aresetn        => m_axi_s2mm_Ch5_aresetn,
      m_axi_s2mm_Ch5_awid           => m_axi_s2mm_Ch5_awid,
      m_axi_s2mm_Ch5_awaddr         => m_axi_s2mm_Ch5_awaddr,
      m_axi_s2mm_Ch5_awlen          => m_axi_s2mm_Ch5_awlen,
      m_axi_s2mm_Ch5_awsize         => m_axi_s2mm_Ch5_awsize,
      m_axi_s2mm_Ch5_awburst        => m_axi_s2mm_Ch5_awburst,
      m_axi_s2mm_Ch5_awprot         => m_axi_s2mm_Ch5_awprot,
      m_axi_s2mm_Ch5_awcache        => m_axi_s2mm_Ch5_awcache,
      m_axi_s2mm_Ch5_awvalid        => m_axi_s2mm_Ch5_awvalid,
      m_axi_s2mm_Ch5_awready        => m_axi_s2mm_Ch5_awready,
      m_axi_s2mm_Ch5_wdata          => m_axi_s2mm_Ch5_wdata,
      m_axi_s2mm_Ch5_wstrb          => m_axi_s2mm_Ch5_wstrb,
      m_axi_s2mm_Ch5_wlast          => m_axi_s2mm_Ch5_wlast,
      m_axi_s2mm_Ch5_wvalid         => m_axi_s2mm_Ch5_wvalid,
      m_axi_s2mm_Ch5_wready         => m_axi_s2mm_Ch5_wready,
      m_axi_s2mm_Ch5_bresp          => m_axi_s2mm_Ch5_bresp,
      m_axi_s2mm_Ch5_bvalid         => m_axi_s2mm_Ch5_bvalid,
      m_axi_s2mm_Ch5_bready         => m_axi_s2mm_Ch5_bready,

      -- AXI Memory interface (Channel 6)
      m_axi_s2mm_Ch6_aclk           => m_axi_s2mm_Ch6_aclk,
      m_axi_s2mm_Ch6_aresetn        => m_axi_s2mm_Ch6_aresetn,
      m_axi_s2mm_Ch6_awid           => m_axi_s2mm_Ch6_awid,
      m_axi_s2mm_Ch6_awaddr         => m_axi_s2mm_Ch6_awaddr,
      m_axi_s2mm_Ch6_awlen          => m_axi_s2mm_Ch6_awlen,
      m_axi_s2mm_Ch6_awsize         => m_axi_s2mm_Ch6_awsize,
      m_axi_s2mm_Ch6_awburst        => m_axi_s2mm_Ch6_awburst,
      m_axi_s2mm_Ch6_awprot         => m_axi_s2mm_Ch6_awprot,
      m_axi_s2mm_Ch6_awcache        => m_axi_s2mm_Ch6_awcache,
      m_axi_s2mm_Ch6_awvalid        => m_axi_s2mm_Ch6_awvalid,
      m_axi_s2mm_Ch6_awready        => m_axi_s2mm_Ch6_awready,
      m_axi_s2mm_Ch6_wdata          => m_axi_s2mm_Ch6_wdata,
      m_axi_s2mm_Ch6_wstrb          => m_axi_s2mm_Ch6_wstrb,
      m_axi_s2mm_Ch6_wlast          => m_axi_s2mm_Ch6_wlast,
      m_axi_s2mm_Ch6_wvalid         => m_axi_s2mm_Ch6_wvalid,
      m_axi_s2mm_Ch6_wready         => m_axi_s2mm_Ch6_wready,
      m_axi_s2mm_Ch6_bresp          => m_axi_s2mm_Ch6_bresp,
      m_axi_s2mm_Ch6_bvalid         => m_axi_s2mm_Ch6_bvalid,
      m_axi_s2mm_Ch6_bready         => m_axi_s2mm_Ch6_bready,

      -- AXI Memory interface (Channel 7)
      m_axi_s2mm_Ch7_aclk           => m_axi_s2mm_Ch7_aclk,
      m_axi_s2mm_Ch7_aresetn        => m_axi_s2mm_Ch7_aresetn,
      m_axi_s2mm_Ch7_awid           => m_axi_s2mm_Ch7_awid,
      m_axi_s2mm_Ch7_awaddr         => m_axi_s2mm_Ch7_awaddr,
      m_axi_s2mm_Ch7_awlen          => m_axi_s2mm_Ch7_awlen,
      m_axi_s2mm_Ch7_awsize         => m_axi_s2mm_Ch7_awsize,
      m_axi_s2mm_Ch7_awburst        => m_axi_s2mm_Ch7_awburst,
      m_axi_s2mm_Ch7_awprot         => m_axi_s2mm_Ch7_awprot,
      m_axi_s2mm_Ch7_awcache        => m_axi_s2mm_Ch7_awcache,
      m_axi_s2mm_Ch7_awvalid        => m_axi_s2mm_Ch7_awvalid,
      m_axi_s2mm_Ch7_awready        => m_axi_s2mm_Ch7_awready,
      m_axi_s2mm_Ch7_wdata          => m_axi_s2mm_Ch7_wdata,
      m_axi_s2mm_Ch7_wstrb          => m_axi_s2mm_Ch7_wstrb,
      m_axi_s2mm_Ch7_wlast          => m_axi_s2mm_Ch7_wlast,
      m_axi_s2mm_Ch7_wvalid         => m_axi_s2mm_Ch7_wvalid,
      m_axi_s2mm_Ch7_wready         => m_axi_s2mm_Ch7_wready,
      m_axi_s2mm_Ch7_bresp          => m_axi_s2mm_Ch7_bresp,
      m_axi_s2mm_Ch7_bvalid         => m_axi_s2mm_Ch7_bvalid,
      m_axi_s2mm_Ch7_bready         => m_axi_s2mm_Ch7_bready      
    );
   
  o_PcieMsi_p <= '1' when (v8_intReq_s /= X"00") else '0';
   
end IMP;
