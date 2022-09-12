//*****************************************************************************
// (c) Copyright 2010 Xilinx, Inc. All rights reserved.
//
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
//
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
//
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
//
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
//
//*****************************************************************************
// Device           : Virtex-6
// Design Name      : DDR2/3 SDRAM
// Reference        :
// Revision History :
//*****************************************************************************

`timescale 1ps/1ps





module axi_v6_ddrx #
  (
   parameter C_REFCLK_FREQ             = 200,
                                       // # = 200 when design frequency <= 533 MHz,
                                       //   = 300 when design frequency > 533 MHz.
   parameter C_BYPASS_INIT_CAL         = "OFF",
                                       // # = "OFF" -  Complete memory init &
                                       //              calibration sequence
                                       // # = "SKIP" - Skip memory init &
                                       //              calibration sequence
                                       // # = "FAST" - Skip memory init & use
                                       //              abbreviated calib sequence
   parameter C_IODELAY_GRP             = "IODELAY_MIG",
                                       // It is associated to a set of IODELAYs with
                                       // an IDELAYCTRL that have same IODELAY CONTROLLER
                                       // clock frequency.
   parameter C_USE_EXTERNAL_IODELAY_CTRL = 0,
                                       // # = 0, if not chaining IODELAY CTRLs
                                       //   = 1, Chaining IODELAY CTRLs and
                                       //        iodelay_ctrl_rdy_i is connected
   parameter C_DRAM_TYPE               = "DDR3",
                                       // SDRAM type. # = "DDR3", "DDR2".
   parameter C_NCS_PER_RANK            = 1,
                                       // # of unique CS outputs per Rank for
                                       // phy.
   parameter C_BANK_WIDTH              = 3,
                                       // # of memory Bank Address bits.
   parameter C_CK_WIDTH                = 1,
                                       // # of CK/CK# outputs to memory.
   parameter C_CKE_WIDTH               = 1,
                                       // # of CKE outputs to memory.
   parameter C_COL_WIDTH               = 10,
                                       // # of memory Column Address bits.
   parameter C_CS_WIDTH                = 1,
                                       // # of unique CS outputs to memory.
   parameter C_DM_WIDTH                = 8,
                                       // # of Data Mask bits.
   parameter C_USE_DM_PORT             = 1,
                                       // # = 1, When Data Mask option is enabled
                                       //   = 0, When Data Mask option is disbaled
                                       // When Data Mask option is disbaled in
                                       // MIG Controller Options page, the logic
                                       // related to Data Mask should not get
                                       // synthesized
   parameter C_DRAM_WIDTH              = 8,
                                       // # of DQ bits per DQS.
   parameter C_DQS_WIDTH               = 8,
                                       // # of DQS/DQS# bits.
   parameter C_ROW_WIDTH               = 13,
                                       // # of memory Row Address bits.
   parameter C_DQ_WIDTH                = 64,
                                       // # of Data (DQ) bits.
   parameter C_BURST_MODE              = "8",
                                       // DDR3 SDRAM:
                                       // Burst Length (Mode Register 0).
                                       // # = "8", "4", "OTF".
                                       // DDR2 SDRAM:
                                       // Burst Length (Mode Register).
                                       // # = "8", "4".
   parameter C_BURST_TYPE              = "SEQ",
                                       // DDR3 SDRAM: Burst Type (Mode Register 0).
                                       // DDR2 SDRAM: Burst Type (Mode Register).
                                       // # = "SEQ" - (Sequential),
                                       //   = "INT" - (Interleaved).
   parameter C_IODELAY_HP_MODE         = "ON",
                                       // to phy_top
   parameter C_CL                      = 6,
                                       // DDR3 SDRAM: CAS Latency (Mode Register 0).
                                       // DDR2 SDRAM: CAS Latency (Mode Register).
   parameter C_CWL                     = 5,
                                       // DDR3 SDRAM: CAS Write Latency (Mode Register 2).
                                       // DDR2 SDRAM: Can be ignored
   parameter C_ADDR_CMD_MODE           = "UNBUF",
                                       // # = "UNBUF", "REG".
   parameter C_NBANK_MACHS              = 4,
                                       // # = 2,3,4,5,6,7,8.
   parameter C_ORDERING                = "STRICT",
                                       // # = "NORM", "STRICT", "RELAXED".
   parameter C_RANKS                   = 1,
                                       // # of Ranks.
   parameter C_CAL_WIDTH               = "HALF",
                                       // # = "HALF", "FULL".
   parameter C_RTT_NOM                 = "60",
                                       // DDR3 SDRAM:
                                       // RTT_NOM (ODT) (Mode Register 1).
                                       // # = "DISABLED" - RTT_NOM disabled,
                                       //   = "120" - RZQ/2,
                                       //   = "60"  - RZQ/4,
                                       //   = "40"  - RZQ/6.
                                       // DDR2 SDRAM:
                                       // RTT (Nominal) (Extended Mode Register).
                                       // # = "DISABLED" - RTT disabled,
                                       //   = "150" - 150 Ohms,
                                       //   = "75" - 75 Ohms,
                                       //   = "50" - 50 Ohms.
   parameter C_RTT_WR                  = "OFF",
                                       // DDR3 SDRAM:
                                       // RTT_WR (ODT) (Mode Register 2).
                                       // # = "OFF" - Dynamic ODT off,
                                       //   = "120" - RZQ/2,
                                       //   = "60"  - RZQ/4,
                                       // DDR2 SDRAM:
                                       // Can be ignored. Always set to "OFF".
   parameter C_OUTPUT_DRV              = "HIGH",
                                       // DDR3 SDRAM:
                                       // Output Drive Strength (Mode Register 1).
                                       // # = "HIGH" - RZQ/7,
                                       //   = "LOW" - RZQ/6.
                                       // DDR2 SDRAM:
                                       // Output Drive Strength (Extended Mode Register).
                                       // # = "HIGH" - FULL,
                                       //   = "LOW" - REDUCED.
   parameter C_REG_CTRL                = "OFF",
                                       // # = "ON" - RDIMMs,
                                       //   = "OFF" - Components, SODIMMs, UDIMMs.
   parameter C_NDQS_COL0               = 3,
                                       // Number of DQS groups in I/O column #1.
   parameter C_NDQS_COL1               = 5,
                                       // Number of DQS groups in I/O column #2.
   parameter C_NDQS_COL2               = 0,
                                       // Number of DQS groups in I/O column #3.
   parameter C_NDQS_COL3               = 0,
                                       // Number of DQS groups in I/O column #4.
   parameter C_DQS_LOC_COL0            = 24'h020100,
                                       // DQS groups in column #1.
   parameter C_DQS_LOC_COL1            = 40'h0706050403,
                                       // DQS groups in column #2.
   parameter C_DQS_LOC_COL2            = 0,
                                       // DQS groups in column #3.
   parameter C_DQS_LOC_COL3            = 0,
                                       // DQS groups in column #4.
   parameter C_TCK                     = 2500,
                                       // memory tCK paramter.
                                       // # = Clock Period.
   parameter C_TFAW                    = 45000,
                                       // memory tRAW paramter.
   parameter C_TPRDI                   = 1_000_000,
                                       // memory tPRDI paramter.
   parameter C_TRRD                    = 7500,
                                       // memory tRRD paramter.
   parameter C_TRAS                    = 37500,
                                       // memory tRAS paramter.
   parameter C_TRCD                    = 13130,
                                       // memory tRCD paramter.
   parameter C_TREFI                   = 7800000,
                                       // memory tREFI paramter.
   parameter C_TRFC                    = 110000,
                                       // memory tRFC paramter.
   parameter C_TRP                     = 13130,
                                       // memory tRP paramter.
   parameter C_TRTP                    = 7500,
                                       // memory tRTP paramter.
   parameter C_TWTR                    = 7500,
                                       // memory tWTR paramter.
   parameter C_TZQI                    = 128_000_000,
                                       // memory tZQI paramter.
   parameter C_TZQCS                   = 64,
                                       // memory tZQCS paramter.
   parameter C_SLOT_0_CONFIG           = 8'b0000_0001,
                                       // Mapping of Ranks.
   parameter C_SLOT_1_CONFIG           = 8'b0000_0000,
                                       // Mapping of Ranks.
   parameter C_MEM_ADDR_ORDER          = "BANK_ROW_COLUMN",
                                       // Address Ordering.
   parameter C_ECC                     = "OFF",
   parameter C_ECC_TEST                = "OFF",

                                                           
                                                            
   parameter C_S_AXI_ID_WIDTH         = 4, 
                                       // Width of all master and slave ID signals.
                                        // # = >= 1.
   parameter C_S_AXI_ADDR_WIDTH       = 32, 
                                        // Width of S_AXI_AWADDR, S_AXI_ARADDR, M_AXI_AWADDR and 
                                        // M_AXI_ARADDR for all SI/MI slots.
                                        // # = 32.
   parameter C_S_AXI_DATA_WIDTH       = 32,
                                        // Width of WDATA and RDATA on SI slot.
                                        // Must be <= P_DATA_WIDTH*4
                                        // # = 32, 64, 128, 256.
                                        // Indicates whether to instatiate upsizer
                                        // Range: 0, 1
   parameter C_S_AXI_SUPPORTS_NARROW_BURST  = 1, 
                                        // Instatiates register slices before upsizer.
                                        // The type of register is specified for each channel
                                        // in a vector. 4 bits per channel are used.  
                                        // C_S_AXI_REG_EN0[03:00] = AW CHANNEL REGISTER SLICE
                                        // C_S_AXI_REG_EN0[07:04] =  W CHANNEL REGISTER SLICE
                                        // C_S_AXI_REG_EN0[11:08] =  B CHANNEL REGISTER SLICE
                                        // C_S_AXI_REG_EN0[15:12] = AR CHANNEL REGISTER SLICE
                                        // C_S_AXI_REG_EN0[20:16] =  R CHANNEL REGISTER SLICE
                                        // Possible values for each channel are:
                                        //
                                        //   0 => BYPASS    = The channel is just wired through the
                                        //                    module.
                                        //   1 => FWD       = The master VALID and payload signals
                                        //                    are registrated. 
                                        //   2 => REV       = The slave ready signal is registrated
                                        //   3 => FWD_REV   = Both FWD and REV
                                        //   4 => SLAVE_FWD = All slave side signals and master 
                                        //                    VALID and payload are registrated.
                                        //   5 => SLAVE_RDY = All slave side signals and master 
                                        //                    READY are registrated.
                                        //   6 => INPUTS    = Slave and Master side inputs are 
                                        //                    registrated.
                                        //
                                        //                  A  A
                                        //                 RRBWW 
   parameter         C_S_AXI_REG_EN0  = 20'h00000, 
                                        // Same as C_S_AXI_REG_EN0, but this register is after
                                        // the upsizer
   parameter         C_S_AXI_REG_EN1  = 20'h00000, 
                                        // Indicates the Arbitration
                                        // Allowed values - "TDM", "ROUND_ROBIN",
                                        // "RD_PRI_REG", "RD_PRI_REG_STARVE_LIMIT"
   parameter         C_RD_WR_ARB_ALGORITHM = "RD_PRI_REG",
                                        // AXI base address to memory
   parameter         C_S_AXI_BASEADDR = 32'h0000_0000,
                                        // AXI high address to memory
   parameter         C_S_AXI_HIGHADDR = 32'hFFFF_FFFF,
                                         // Width of AXI-4-Lite address bus
   parameter integer C_S_AXI_CTRL_ADDR_WIDTH = 32, 
                                       // Width of AXI-4-Lite data buses
   parameter integer C_S_AXI_CTRL_DATA_WIDTH = 32, 
                                       // Controls ECC on/off value at startup/reset
   parameter integer C_ECC_ONOFF_RESET_VALUE = 1
   )
  (

   input  wire                                 clk,
   input  wire                                 clk_mem,
   input  wire                                 clk_rd_base,
   input  wire                                 aresetn,
   input  wire                                 clk_ref,
   input  wire                                 iodelay_ctrl_rdy_i,
   output wire                                 iodelay_ctrl_rdy_o,

   output wire [C_ROW_WIDTH-1:0]               ddr_addr,
   output wire [C_BANK_WIDTH-1:0]              ddr_ba,
   output wire                                 ddr_cas_n,
   output wire [C_CK_WIDTH-1:0]                ddr_ck_n,
   output wire [C_CK_WIDTH-1:0]                ddr_ck_p,
   output wire [C_CKE_WIDTH-1:0]               ddr_cke,
   output wire [C_CS_WIDTH*C_NCS_PER_RANK-1:0] ddr_cs_n,
   output wire [C_DM_WIDTH-1:0]                ddr_dm,
   output wire [C_CS_WIDTH*C_NCS_PER_RANK-1:0] ddr_odt,
   output wire                                 ddr_ras_n,
   output wire                                 ddr_reset_n,
   output wire                                 ddr_parity,
   output wire                                 ddr_we_n,
   inout  wire [C_DQ_WIDTH-1:0]                ddr_dq,
   inout  wire [C_DQS_WIDTH-1:0]               ddr_dqs_n,
   inout  wire [C_DQS_WIDTH-1:0]               ddr_dqs_p,
   output wire                                 pd_PSEN,
   output wire                                 pd_PSINCDEC,
   input  wire                                 pd_PSDONE,
   output wire                                 phy_init_done,
   // Slave Interface Write Address Ports
   input  wire [C_S_AXI_ID_WIDTH-1:0]          s_axi_awid    , 
   input  wire [C_S_AXI_ADDR_WIDTH-1:0]        s_axi_awaddr  , 
   input  wire [7:0]                           s_axi_awlen   , 
   input  wire [2:0]                           s_axi_awsize  , 
   input  wire [1:0]                           s_axi_awburst , 
   input  wire [0:0]                           s_axi_awlock  , 
   input  wire [3:0]                           s_axi_awcache , 
   input  wire [2:0]                           s_axi_awprot  , 
   input  wire [3:0]                           s_axi_awqos   , 
   input  wire                                 s_axi_awvalid , 
   output wire                                 s_axi_awready , 
   // Slave Interface Write Data Ports         
   input  wire [C_S_AXI_DATA_WIDTH-1:0]        s_axi_wdata   , 
   input  wire [C_S_AXI_DATA_WIDTH/8-1:0]      s_axi_wstrb   , 
   input  wire                                 s_axi_wlast   , 
   input  wire                                 s_axi_wvalid  , 
   output wire                                 s_axi_wready  , 
   // Slave Interface Write Response Ports     
   output wire [C_S_AXI_ID_WIDTH-1:0]          s_axi_bid     , 
   output wire [1:0]                           s_axi_bresp   , 
   output wire                                 s_axi_bvalid  , 
   input  wire                                 s_axi_bready  , 
   // Slave Interface Read Address Ports       
   input  wire [C_S_AXI_ID_WIDTH-1:0]          s_axi_arid    , 
   input  wire [C_S_AXI_ADDR_WIDTH-1:0]        s_axi_araddr  , 
   input  wire [7:0]                           s_axi_arlen   , 
   input  wire [2:0]                           s_axi_arsize  , 
   input  wire [1:0]                           s_axi_arburst , 
   input  wire [0:0]                           s_axi_arlock  , 
   input  wire [3:0]                           s_axi_arcache , 
   input  wire [2:0]                           s_axi_arprot  , 
   input  wire [3:0]                           s_axi_arqos   , 
   input  wire                                 s_axi_arvalid , 
   output wire                                 s_axi_arready , 
   // Slave Interface Read Data Ports          
   output wire [C_S_AXI_ID_WIDTH-1:0]          s_axi_rid     , 
   output wire [C_S_AXI_DATA_WIDTH-1:0]        s_axi_rdata   , 
   output wire [1:0]                           s_axi_rresp   , 
   output wire                                 s_axi_rlast   , 
   output wire                                 s_axi_rvalid  , 
   input  wire                                 s_axi_rready  ,
   // AXI CTRL port
   input  wire                                 s_axi_ctrl_awvalid , 
   output wire                                 s_axi_ctrl_awready , 
   input  wire [C_S_AXI_CTRL_ADDR_WIDTH-1:0]   s_axi_ctrl_awaddr  , 
   // Slave Interface Write Data Ports         
   input  wire                                 s_axi_ctrl_wvalid  , 
   output wire                                 s_axi_ctrl_wready  , 
   input  wire [C_S_AXI_CTRL_DATA_WIDTH-1:0]   s_axi_ctrl_wdata   , 
   // Slave Interface Write Response Ports     
   output wire                                 s_axi_ctrl_bvalid  , 
   input  wire                                 s_axi_ctrl_bready  , 
   output wire [1:0]                           s_axi_ctrl_bresp   , 
   // Slave Interface Read Address Ports       
   input  wire                                 s_axi_ctrl_arvalid , 
   output wire                                 s_axi_ctrl_arready , 
   input  wire [C_S_AXI_CTRL_ADDR_WIDTH-1:0]   s_axi_ctrl_araddr  , 
   // Slave Interface Read Data Ports          
   output wire                                 s_axi_ctrl_rvalid  , 
   input  wire                                 s_axi_ctrl_rready  , 
   output wire [C_S_AXI_CTRL_DATA_WIDTH-1:0]   s_axi_ctrl_rdata   , 
   output wire [1:0]                           s_axi_ctrl_rresp   , 
   // Interrupt output                         
   output wire                                 interrupt          

   );

  /////////////////////////////////////////////////////////////////////////////
  // Functions
  /////////////////////////////////////////////////////////////////////////////
  
  // clog2.
  function integer clog2;
    input integer value;
  begin
    for (clog2=0; value>0; clog2=clog2+1) begin
      value = value >> 1;
    end
    if (clog2 < 1) begin
      clog2 = 1;
    end
  end
  endfunction

  /////////////////////////////////////////////////////////////////////////////
  // Local params
  /////////////////////////////////////////////////////////////////////////////
  localparam P_S_AXI_ADDRMASK   = C_S_AXI_BASEADDR ^ C_S_AXI_HIGHADDR;

  // Mig auto-calculated parameters and tie-offs
  localparam P_NCK_PER_CLK              = 2;
  localparam P_DQ_CNT_WIDTH             = clog2(C_DQ_WIDTH);
  localparam P_DQS_CNT_WIDTH            = clog2(C_DQS_WIDTH);
  localparam P_RANK_WIDTH               = clog2(C_RANKS);
  localparam P_AL                       = "0";
  localparam P_IBUF_LPWR_MODE           = "OFF";
  localparam P_NAL                      = 0;
  localparam P_DATA_BUF_ADDR_WIDTH      = 4;
  localparam P_DATA_BUF_OFFSET_WIDTH    = 1;
  localparam P_BM_CNT_WIDTH             = clog2(C_NBANK_MACHS);
  localparam P_WRLVL                    = C_DRAM_TYPE == "DDR3" ? "ON" : "OFF";
  localparam P_PHASE_DETECT             = "ON"; 
  // calibration Address. The address given below will be used for calibration
  // read and write operations. 
  localparam P_CALIB_ROW_ADD            = 16'hFFFF;// Calibration row address
  localparam P_CALIB_COL_ADD            = 12'hFFF; // Calibration column address
  localparam P_CALIB_BA_ADD             = 3'b111;  // Calibration bank address 

  localparam P_DEBUG_PORT               = "OFF";
  localparam P_ADDR_WIDTH               = P_RANK_WIDTH + C_BANK_WIDTH 
                                          + C_ROW_WIDTH + C_COL_WIDTH;
  localparam P_STARVE_LIMIT             = 2;
  localparam P_TCQ                      = 0;
  localparam P_DATA_WIDTH               = (C_DQ_WIDTH == 72) ? 64 : 
                                          (C_DQ_WIDTH == 144) ? 128 :
                                          C_DQ_WIDTH;
  localparam P_PAYLOAD_WIDTH            = P_DATA_WIDTH;
  localparam P_APP_DATA_WIDTH           = P_PAYLOAD_WIDTH * 4;
  localparam P_APP_MASK_WIDTH           = P_APP_DATA_WIDTH / 8;

  localparam P_ECC_CE_COUNTER_WIDTH     = 8;
  
  /////////////////////////////////////////////////////////////////////////////
  // Internal signals
  /////////////////////////////////////////////////////////////////////////////
  reg                                 aresetn_d1;
  reg                                 areset_d1;
  wire [5*C_DQS_WIDTH-1:0]            dbg_wr_dqs_tap_set;
  wire [5*C_DQS_WIDTH-1:0]            dbg_wr_dq_tap_set;
  wire                                dbg_wr_tap_set_en;
  wire                                dbg_pd_off;
  wire                                dbg_pd_maintain_off;
  wire                                dbg_pd_maintain_0_only;
  wire                                dbg_inc_cpt;
  wire                                dbg_dec_cpt;
  wire                                dbg_inc_rd_dqs;
  wire                                dbg_dec_rd_dqs;
  wire [P_DQS_CNT_WIDTH-1:0]          dbg_inc_dec_sel;
  wire                                dbg_inc_rd_fps;
  wire                                dbg_dec_rd_fps;

  generate 
  if (C_USE_EXTERNAL_IODELAY_CTRL == 1) begin : NO_IODELAY_CTRL
    assign iodelay_ctrl_rdy_o = iodelay_ctrl_rdy_i;
  end 
  else begin : IODELAY_CTRL
    wire            aresetn_synch;

    // Synchronize the input reset with the clk_ref clock domain
    axi_v6_ddrx_synch #
    (
      .C_SYNCH_WIDTH ( 1 )
    )
    u_synch_to_clk_ref
    (
      .clk          ( clk_ref ),
      .synch_in     ( aresetn ),
      .synch_out    ( aresetn_synch   )
    );

    axi_v6_ddrx_iodelay_ctrl #
    (
      .TCQ         ( P_TCQ         ) ,
      .IODELAY_GRP ( C_IODELAY_GRP ) ,
      .RST_ACT_LOW ( 1             ) 
    )
    u_axi_v6_ddrx_iodelay_ctrl
    (
      .clk_ref          ( clk_ref            ) ,
      .sys_rst          ( aresetn_synch      ) ,
      .iodelay_ctrl_rdy ( iodelay_ctrl_rdy_o ) 
    );
  end
  endgenerate

  // Synchronize the iodelay_ctrl_rdy_o to clk domain
  axi_v6_ddrx_synch #
  (
    .C_SYNCH_WIDTH ( 1 )
  )
  u_synch_to_clk
  (
    .clk          ( clk ),
    .synch_in     ( iodelay_ctrl_rdy_o),
    .synch_out    ( iodelay_ctrl_rdy_o_synch )
  );

  always @(posedge clk) begin
    areset_d1  <= ~(aresetn & iodelay_ctrl_rdy_o_synch);
  end

  memc_ui_top #
  (
   .REFCLK_FREQ             ( C_REFCLK_FREQ           ) ,
   .SIM_BYPASS_INIT_CAL     ( C_BYPASS_INIT_CAL       ) ,
   .IODELAY_GRP             ( C_IODELAY_GRP           ) ,
   .nCK_PER_CLK             ( P_NCK_PER_CLK           ) ,
   .DRAM_TYPE               ( C_DRAM_TYPE             ) ,
   .nCS_PER_RANK            ( C_NCS_PER_RANK          ) ,
   .DQ_CNT_WIDTH            ( P_DQ_CNT_WIDTH          ) ,
   .DQS_CNT_WIDTH           ( P_DQS_CNT_WIDTH         ) ,
   .RANK_WIDTH              ( P_RANK_WIDTH            ) ,
   .BANK_WIDTH              ( C_BANK_WIDTH            ) ,
   .CK_WIDTH                ( C_CK_WIDTH              ) ,
   .CKE_WIDTH               ( C_CKE_WIDTH             ) ,
   .COL_WIDTH               ( C_COL_WIDTH             ) ,
   .CS_WIDTH                ( C_CS_WIDTH              ) ,
   .DM_WIDTH                ( C_DM_WIDTH              ) ,
   .USE_DM_PORT             ( C_USE_DM_PORT           ) ,
   .DQ_WIDTH                ( C_DQ_WIDTH              ) ,
   .DRAM_WIDTH              ( C_DRAM_WIDTH            ) ,
   .DQS_WIDTH               ( C_DQS_WIDTH             ) ,
   .ROW_WIDTH               ( C_ROW_WIDTH             ) ,
   .AL                      ( P_AL                    ) ,
   .BURST_MODE              ( C_BURST_MODE            ) ,
   .BURST_TYPE              ( C_BURST_TYPE            ) ,
   .IBUF_LPWR_MODE          ( P_IBUF_LPWR_MODE        ) ,
   .IODELAY_HP_MODE         ( C_IODELAY_HP_MODE       ) ,
   .nAL                     ( P_NAL                   ) ,
   .CL                      ( C_CL                    ) ,
   .CWL                     ( C_CWL                   ) ,
   .DATA_BUF_ADDR_WIDTH     ( P_DATA_BUF_ADDR_WIDTH   ) ,
   .DATA_BUF_OFFSET_WIDTH   ( P_DATA_BUF_OFFSET_WIDTH ) ,
   .BM_CNT_WIDTH            ( P_BM_CNT_WIDTH          ) ,
   .ADDR_CMD_MODE           ( C_ADDR_CMD_MODE         ) ,
   .nBANK_MACHS             ( C_NBANK_MACHS           ) ,
   .ORDERING                ( C_ORDERING              ) ,
   .RANKS                   ( C_RANKS                 ) ,
   .WRLVL                   ( P_WRLVL                 ) ,
   .PHASE_DETECT            ( P_PHASE_DETECT          ) ,
   .CAL_WIDTH               ( C_CAL_WIDTH             ) ,
   .CALIB_ROW_ADD           ( P_CALIB_ROW_ADD         ) , 
   .CALIB_COL_ADD           ( P_CALIB_COL_ADD         ) , 
   .CALIB_BA_ADD            ( P_CALIB_BA_ADD          ) , 
   .RTT_NOM                 ( C_RTT_NOM               ) ,
   .RTT_WR                  ( C_RTT_WR                ) ,
   .OUTPUT_DRV              ( C_OUTPUT_DRV            ) ,
   .REG_CTRL                ( C_REG_CTRL              ) ,
   .nDQS_COL0               ( C_NDQS_COL0             ) ,
   .nDQS_COL1               ( C_NDQS_COL1             ) ,
   .nDQS_COL2               ( C_NDQS_COL2             ) ,
   .nDQS_COL3               ( C_NDQS_COL3             ) ,
   .DQS_LOC_COL0            ( C_DQS_LOC_COL0          ) ,
   .DQS_LOC_COL1            ( C_DQS_LOC_COL1          ) ,
   .DQS_LOC_COL2            ( C_DQS_LOC_COL2          ) ,
   .DQS_LOC_COL3            ( C_DQS_LOC_COL3          ) ,
   .tCK                     ( C_TCK                   ) ,
   .tFAW                    ( C_TFAW                  ) ,
   .tPRDI                   ( C_TPRDI                 ) ,
   .tRRD                    ( C_TRRD                  ) ,
   .tRAS                    ( C_TRAS                  ) ,
   .tRCD                    ( C_TRCD                  ) ,
   .tREFI                   ( C_TREFI                 ) ,
   .tRFC                    ( C_TRFC                  ) ,
   .tRP                     ( C_TRP                   ) ,
   .tRTP                    ( C_TRTP                  ) ,
   .tWTR                    ( C_TWTR                  ) ,
   .tZQI                    ( C_TZQI                  ) ,
   .tZQCS                   ( C_TZQCS                 ) ,
   .SLOT_0_CONFIG           ( C_SLOT_0_CONFIG         ) ,
   .SLOT_1_CONFIG           ( C_SLOT_1_CONFIG         ) ,
   .DEBUG_PORT              ( P_DEBUG_PORT            ) ,
   .ADDR_WIDTH              ( P_ADDR_WIDTH            ) ,
   .MEM_ADDR_ORDER          ( C_MEM_ADDR_ORDER        ) ,
   .STARVE_LIMIT            ( P_STARVE_LIMIT          ) ,
   .TCQ                     ( P_TCQ                   ) ,
   .ECC                     ( C_ECC                   ) ,
   .DATA_WIDTH              ( P_DATA_WIDTH            ) ,
   .ECC_TEST                ( C_ECC_TEST              ) ,
   .PAYLOAD_WIDTH           ( P_PAYLOAD_WIDTH         ) ,
   .APP_DATA_WIDTH          ( P_APP_DATA_WIDTH        ) ,
   .APP_MASK_WIDTH          ( P_APP_MASK_WIDTH        ) ,
   .C_S_AXI_ID_WIDTH        ( C_S_AXI_ID_WIDTH        ) ,
   .C_S_AXI_ADDR_WIDTH      ( C_S_AXI_ADDR_WIDTH      ) ,
   .C_S_AXI_DATA_WIDTH      ( C_S_AXI_DATA_WIDTH      ) ,
   .C_S_AXI_SUPPORTS_NARROW_BURST ( C_S_AXI_SUPPORTS_NARROW_BURST ) ,
   .C_S_AXI_REG_EN0         ( C_S_AXI_REG_EN0         ) ,
   .C_S_AXI_REG_EN1         ( C_S_AXI_REG_EN1         ) ,
   .C_RD_WR_ARB_ALGORITHM   ( C_RD_WR_ARB_ALGORITHM   ) ,
   .C_S_AXI_CTRL_ADDR_WIDTH ( C_S_AXI_CTRL_ADDR_WIDTH ) ,
   .C_S_AXI_CTRL_DATA_WIDTH ( C_S_AXI_CTRL_DATA_WIDTH ) ,
   .C_S_AXI_BASEADDR        ( C_S_AXI_BASEADDR        ) ,
   .C_ECC_ONOFF_RESET_VALUE ( C_ECC_ONOFF_RESET_VALUE ) ,
   .C_ECC_CE_COUNTER_WIDTH  ( P_ECC_CE_COUNTER_WIDTH  )

   )
  u_memc_ui_top
  (
   .clk                       ( clk                    ) ,
   .clk_mem                   ( clk_mem                ) ,
   .clk_rd_base               ( clk_rd_base            ) ,
   .rst                       ( areset_d1              ) ,
   .ddr_addr                  ( ddr_addr               ) ,
   .ddr_ba                    ( ddr_ba                 ) ,
   .ddr_cas_n                 ( ddr_cas_n              ) ,
   .ddr_ck_n                  ( ddr_ck_n               ) ,
   .ddr_ck                    ( ddr_ck_p               ) ,
   .ddr_cke                   ( ddr_cke                ) ,
   .ddr_cs_n                  ( ddr_cs_n               ) ,
   .ddr_dm                    ( ddr_dm                 ) ,
   .ddr_odt                   ( ddr_odt                ) ,
   .ddr_ras_n                 ( ddr_ras_n              ) ,
   .ddr_reset_n               ( ddr_reset_n            ) ,
   .ddr_parity                ( ddr_parity             ) ,
   .ddr_we_n                  ( ddr_we_n               ) ,
   .ddr_dq                    ( ddr_dq                 ) ,
   .ddr_dqs_n                 ( ddr_dqs_n              ) ,
   .ddr_dqs                   ( ddr_dqs_p              ) ,
   .pd_PSEN                   ( pd_PSEN                ) ,
   .pd_PSINCDEC               ( pd_PSINCDEC            ) ,
   .pd_PSDONE                 ( pd_PSDONE              ) ,
   .phy_init_done             ( phy_init_done          ) ,
   .bank_mach_next            (                        ) ,
   .app_ecc_multiple_err      (                        ) ,
   .dbg_wr_dqs_tap_set        ( dbg_wr_dqs_tap_set     ) ,
   .dbg_wr_dq_tap_set         ( dbg_wr_dq_tap_set      ) ,
   .dbg_wr_tap_set_en         ( dbg_wr_tap_set_en      ) ,
   .dbg_wrlvl_start           (                        ) ,
   .dbg_wrlvl_done            (                        ) ,
   .dbg_wrlvl_err             (                        ) ,
   .dbg_wl_dqs_inverted       (                        ) ,
   .dbg_wr_calib_clk_delay    (                        ) ,
   .dbg_wl_odelay_dqs_tap_cnt (                        ) ,
   .dbg_wl_odelay_dq_tap_cnt  (                        ) ,
   .dbg_rdlvl_start           (                        ) ,
   .dbg_rdlvl_done            (                        ) ,
   .dbg_rdlvl_err             (                        ) ,
   .dbg_cpt_tap_cnt           (                        ) ,
   .dbg_cpt_first_edge_cnt    (                        ) ,
   .dbg_cpt_second_edge_cnt   (                        ) ,
   .dbg_rd_bitslip_cnt        (                        ) ,
   .dbg_rd_clkdly_cnt         (                        ) ,
   .dbg_rd_active_dly         (                        ) ,
   .dbg_pd_off                ( dbg_pd_off             ) ,
   .dbg_pd_maintain_off       ( dbg_pd_maintain_off    ) ,
   .dbg_pd_maintain_0_only    ( dbg_pd_maintain_0_only ) ,
   .dbg_inc_cpt               ( dbg_inc_cpt            ) ,
   .dbg_dec_cpt               ( dbg_dec_cpt            ) ,
   .dbg_inc_rd_dqs            ( dbg_inc_rd_dqs         ) ,
   .dbg_dec_rd_dqs            ( dbg_dec_rd_dqs         ) ,
   .dbg_inc_dec_sel           ( dbg_inc_dec_sel        ) ,
   .dbg_inc_rd_fps            ( dbg_inc_rd_fps         ) ,
   .dbg_dec_rd_fps            ( dbg_dec_rd_fps         ) ,
   .dbg_dqs_tap_cnt           (                        ) ,
   .dbg_dq_tap_cnt            (                        ) ,
   .dbg_rddata                (                        ) ,
   .aresetn                   ( aresetn                ) ,
   .s_axi_awid                ( s_axi_awid             ) ,
   .s_axi_awaddr              ( s_axi_awaddr & P_S_AXI_ADDRMASK ),
   .s_axi_awlen               ( s_axi_awlen            ) ,
   .s_axi_awsize              ( s_axi_awsize           ) ,
   .s_axi_awburst             ( s_axi_awburst          ) ,
   .s_axi_awlock              ( s_axi_awlock           ) ,
   .s_axi_awcache             ( s_axi_awcache          ) ,
   .s_axi_awprot              ( s_axi_awprot           ) ,
   .s_axi_awqos               ( s_axi_awqos            ) ,
   .s_axi_awvalid             ( s_axi_awvalid          ) ,
   .s_axi_awready             ( s_axi_awready          ) ,
   .s_axi_wdata               ( s_axi_wdata            ) ,
   .s_axi_wstrb               ( s_axi_wstrb            ) ,
   .s_axi_wlast               ( s_axi_wlast            ) ,
   .s_axi_wvalid              ( s_axi_wvalid           ) ,
   .s_axi_wready              ( s_axi_wready           ) ,
   .s_axi_bid                 ( s_axi_bid              ) ,
   .s_axi_bresp               ( s_axi_bresp            ) ,
   .s_axi_bvalid              ( s_axi_bvalid           ) ,
   .s_axi_bready              ( s_axi_bready           ) ,
   .s_axi_arid                ( s_axi_arid             ) ,
   .s_axi_araddr              ( s_axi_araddr & P_S_AXI_ADDRMASK ) ,
   .s_axi_arlen               ( s_axi_arlen            ) ,
   .s_axi_arsize              ( s_axi_arsize           ) ,
   .s_axi_arburst             ( s_axi_arburst          ) ,
   .s_axi_arlock              ( s_axi_arlock           ) ,
   .s_axi_arcache             ( s_axi_arcache          ) ,
   .s_axi_arprot              ( s_axi_arprot           ) ,
   .s_axi_arqos               ( s_axi_arqos            ) ,
   .s_axi_arvalid             ( s_axi_arvalid          ) ,
   .s_axi_arready             ( s_axi_arready          ) ,
   .s_axi_rid                 ( s_axi_rid              ) ,
   .s_axi_rdata               ( s_axi_rdata            ) ,
   .s_axi_rresp               ( s_axi_rresp            ) ,
   .s_axi_rlast               ( s_axi_rlast            ) ,
   .s_axi_rvalid              ( s_axi_rvalid           ) ,
   .s_axi_rready              ( s_axi_rready           ) ,
   .s_axi_ctrl_awvalid        ( s_axi_ctrl_awvalid     ) ,
   .s_axi_ctrl_awready        ( s_axi_ctrl_awready     ) ,
   .s_axi_ctrl_awaddr         ( s_axi_ctrl_awaddr      ) ,
   .s_axi_ctrl_wvalid         ( s_axi_ctrl_wvalid      ) ,
   .s_axi_ctrl_wready         ( s_axi_ctrl_wready      ) ,
   .s_axi_ctrl_wdata          ( s_axi_ctrl_wdata       ) ,
   .s_axi_ctrl_bvalid         ( s_axi_ctrl_bvalid      ) ,
   .s_axi_ctrl_bready         ( s_axi_ctrl_bready      ) ,
   .s_axi_ctrl_bresp          ( s_axi_ctrl_bresp       ) ,
   .s_axi_ctrl_arvalid        ( s_axi_ctrl_arvalid     ) ,
   .s_axi_ctrl_arready        ( s_axi_ctrl_arready     ) ,
   .s_axi_ctrl_araddr         ( s_axi_ctrl_araddr      ) ,
   .s_axi_ctrl_rvalid         ( s_axi_ctrl_rvalid      ) ,
   .s_axi_ctrl_rready         ( s_axi_ctrl_rready      ) ,
   .s_axi_ctrl_rdata          ( s_axi_ctrl_rdata       ) ,
   .s_axi_ctrl_rresp          ( s_axi_ctrl_rresp       ) ,
   .interrupt                 ( interrupt              )

   );

  // If debug port is not enabled, then make certain control input
  // to Debug Port are disabled
  generate
    if (P_DEBUG_PORT == "OFF") begin: gen_dbg_tie_off
      assign dbg_wr_dqs_tap_set     = 'b0;
      assign dbg_wr_dq_tap_set      = 'b0;
      assign dbg_wr_tap_set_en      = 1'b0;
      assign dbg_pd_off             = 1'b0;
      assign dbg_pd_maintain_off    = 1'b0;
      assign dbg_pd_maintain_0_only = 1'b0;
      assign dbg_inc_cpt            = 1'b0;
      assign dbg_dec_cpt            = 1'b0;
      assign dbg_inc_rd_dqs         = 1'b0;
      assign dbg_dec_rd_dqs         = 1'b0;
      assign dbg_inc_dec_sel        = 'b0;
      assign dbg_inc_rd_fps         = 1'b0;
      assign dbg_dec_rd_fps         = 1'b0;
    end
  endgenerate

endmodule
