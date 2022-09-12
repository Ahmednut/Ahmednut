// ***************************************************************************
// ***************************************************************************
// Copyright 2011(c) Analog Devices, Inc.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//     - Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     - Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in
//       the documentation and/or other materials provided with the
//       distribution.
//     - Neither the name of Analog Devices, Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//     - The use of this software may or may not infringe the patent rights
//       of one or more patent holders.  This license does not release you
//       from the requirement that you obtain separate licenses from these
//       patent holders to use this software.
//     - Use of the software either in source or binary form, must be run
//       on or directly connected to an Analog Devices Inc. component.
//
// THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
// INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED.
//
// IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, INTELLECTUAL PROPERTY
// RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
// BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// ***************************************************************************
// ***************************************************************************
// ***************************************************************************
// ***************************************************************************

`timescale 1ns/100ps

module axi_ad9361 (

  // physical interface (receive)

  rx_clk_in_p,
  rx_clk_in_n,
  rx_frame_in_p,
  rx_frame_in_n,
  rx_data_in_p,
  rx_data_in_n,

  // physical interface (transmit)

  tx_clk_out_p,
  tx_clk_out_n,
  tx_frame_out_p,
  tx_frame_out_n,
  tx_data_out_p,
  tx_data_out_n,

  // delay clock

  delay_clk,
  i_SystemClk_p,

  // master interface

  l_clk,
  o_DivClk_p,
  clk,

  // dma interface

  adc_enable_i0,
  adc_valid_i0,
  adc_data_i0,
  adc_enable_q0,
  adc_valid_q0,
  adc_data_q0,
  adc_enable_i1,
  adc_valid_i1,
  adc_data_i1,
  adc_enable_q1,
  adc_valid_q1,
  adc_data_q1,
  adc_dovf,
  adc_dunf,

  dac_enable_i0,
  dac_valid_i0,
  dac_data_i0,
  dac_enable_q0,
  dac_valid_q0,
  dac_data_q0,
  dac_enable_i1,
  dac_valid_i1,
  dac_data_i1,
  dac_enable_q1,
  dac_valid_q1,
  dac_data_q1,
  dac_dovf,
  dac_dunf,

  // axi interface

  s_axi_aclk,
  s_axi_aresetn,
  s_axi_awvalid,
  s_axi_awaddr,
  s_axi_awready,
  s_axi_wvalid,
  s_axi_wdata,
  s_axi_wstrb,
  s_axi_wready,
  s_axi_bvalid,
  s_axi_bresp,
  s_axi_bready,
  s_axi_arvalid,
  s_axi_araddr,
  s_axi_arready,
  s_axi_rvalid,
  s_axi_rdata,
  s_axi_rresp,
  s_axi_rready,

  // gpio

  up_dac_gpio_in,
  up_dac_gpio_out,
  up_adc_gpio_in,
  up_adc_gpio_out,
  
   // Reference and Sync

  idp_RefClk_p,
  idn_RefClk_p,
  i_SyncPin_p,
  o_Sync_p,
  
  // Master/Slave interface
  o_RefClkToSlave_p,
  o_SyncToSlave_p,
  i_RefClkFromMaster_p,
  i_SyncFromMaster_p,
  
  // AXI register
  o_up_sel_p,
  o_up_wr_p,
  ov14_up_addr_p,
  ov32_up_wdata_p,
  ov4_up_wstrb_p,
  iv32_up_rdata_p,
  i_up_ack_p,
  
  // complex coefficients
  o_RealCoefficient0_Rx_p,
  o_ImgCoefficient0_Rx_p,
  o_RealCoefficient1_Rx_p,
  o_ImgCoefficient1_Rx_p,
  
  o_DataDelay0_Rx_p,
  o_DataDelay1_Rx_p,
  
  // complex coefficients
  o_RealCoefficient0_Tx_p,
  o_ImgCoefficient0_Tx_p,
  o_RealCoefficient1_Tx_p,
  o_ImgCoefficient1_Tx_p,
  
  o_DataDelay0_Tx_p,
  o_DataDelay1_Tx_p,
  
  // Register access
  i_UpdateInputSyncLatch_p,
  i_UpdateOutputSyncLatch_p,
  o_InputSync_p,
  o_InputSyncLow_p,
  o_InputSyncHigh_p,
  o_OutputSyncLow_p,
  o_OutputSyncHigh_p,

  // chipscope signals

  dev_dbg_data,
  dev_l_dbg_data);

  // parameters

  parameter   C_CLOCK_MASTER = 1;
  parameter   PCORE_ID = 0;
  parameter   PCORE_DEVICE_TYPE = 0;
  parameter   PCORE_DAC_DP_DISABLE = 0;
  parameter   PCORE_ADC_DP_DISABLE = 0;
  parameter   C_S_AXI_MIN_SIZE = 32'hffff;
  parameter   C_BASEADDR = 32'hffffffff;
  parameter   C_HIGHADDR = 32'h00000000;

  // physical interface (receive)

  input           rx_clk_in_p;
  input           rx_clk_in_n;
  input           rx_frame_in_p;
  input           rx_frame_in_n;
  input   [ 5:0]  rx_data_in_p;
  input   [ 5:0]  rx_data_in_n;

  // physical interface (transmit)

  output          tx_clk_out_p;
  output          tx_clk_out_n;
  output          tx_frame_out_p;
  output          tx_frame_out_n;
  output  [ 5:0]  tx_data_out_p;
  output  [ 5:0]  tx_data_out_n;

  // delay clock

  input           delay_clk;
  input           i_SystemClk_p;

  // master interface

  output          l_clk;
  output          o_DivClk_p;
  input           clk;
  
  // complex coefficients
  
  output  [15:0]  o_RealCoefficient0_Rx_p;
  output  [15:0]  o_ImgCoefficient0_Rx_p;
  output  [15:0]  o_RealCoefficient1_Rx_p;
  output  [15:0]  o_ImgCoefficient1_Rx_p;

  output  [31:0]  o_DataDelay0_Rx_p;
  output  [31:0]  o_DataDelay1_Rx_p;
  
  // complex coefficients
  
  output  [15:0]  o_RealCoefficient0_Tx_p;
  output  [15:0]  o_ImgCoefficient0_Tx_p;
  output  [15:0]  o_RealCoefficient1_Tx_p;
  output  [15:0]  o_ImgCoefficient1_Tx_p;

  output  [31:0]  o_DataDelay0_Tx_p;
  output  [31:0]  o_DataDelay1_Tx_p;
  
  // dma interface

  output          adc_enable_i0;
  output          adc_valid_i0;
  output  [11:0]  adc_data_i0;
  output          adc_enable_q0;
  output          adc_valid_q0;
  output  [11:0]  adc_data_q0;
  output          adc_enable_i1;
  output          adc_valid_i1;
  output  [11:0]  adc_data_i1;
  output          adc_enable_q1;
  output          adc_valid_q1;
  output  [11:0]  adc_data_q1;
  input           adc_dovf;
  input           adc_dunf;

  output          dac_enable_i0;
  output          dac_valid_i0;
  input   [11:0]  dac_data_i0;
  output          dac_enable_q0;
  output          dac_valid_q0;
  input   [11:0]  dac_data_q0;
  output          dac_enable_i1;
  output          dac_valid_i1;
  input   [11:0]  dac_data_i1;
  output          dac_enable_q1;
  output          dac_valid_q1;
  input   [11:0]  dac_data_q1;
  input           dac_dovf;
  input           dac_dunf;

  // axi interface

  input           s_axi_aclk;
  input           s_axi_aresetn;
  input           s_axi_awvalid;
  input   [31:0]  s_axi_awaddr;
  output          s_axi_awready;
  input           s_axi_wvalid;
  input   [31:0]  s_axi_wdata;
  input   [ 3:0]  s_axi_wstrb;
  output          s_axi_wready;
  output          s_axi_bvalid;
  output  [ 1:0]  s_axi_bresp;
  input           s_axi_bready;
  input           s_axi_arvalid;
  input   [31:0]  s_axi_araddr;
  output          s_axi_arready;
  output          s_axi_rvalid;
  output  [31:0]  s_axi_rdata;
  output  [ 1:0]  s_axi_rresp;
  input           s_axi_rready;

  // gpio

  input   [31:0]  up_dac_gpio_in;
  output  [31:0]  up_dac_gpio_out;
  input   [31:0]  up_adc_gpio_in;
  output  [31:0]  up_adc_gpio_out;
  
  // Reference and Sync

  input               idp_RefClk_p;
  input               idn_RefClk_p;
  input               i_SyncPin_p;
  output              o_Sync_p;

  // Master/Slave interface

  input               i_RefClkFromMaster_p;
  input               i_SyncFromMaster_p;
  output              o_RefClkToSlave_p;
  output              o_SyncToSlave_p;
  
  // AXI register
  output              o_up_sel_p;
  output              o_up_wr_p;
  output    [13:0]    ov14_up_addr_p;
  output    [31:0]    ov32_up_wdata_p;
  output    [ 3:0]    ov4_up_wstrb_p;
  input     [31:0]    iv32_up_rdata_p;
  input               i_up_ack_p;
  
  input               i_UpdateInputSyncLatch_p;
  input               i_UpdateOutputSyncLatch_p;
  output              o_InputSync_p;
  output              o_InputSyncLow_p;
  output              o_InputSyncHigh_p;
  output              o_OutputSyncLow_p;
  output              o_OutputSyncHigh_p;

  // chipscope signals

  output [111:0]  dev_dbg_data;
  output [ 61:0]  dev_l_dbg_data;

  // internal registers

  reg     [31:0]  up_rdata = 'd0;
  reg             up_ack = 'd0;

  // internal clocks and resets

  wire            rst;
  wire            up_clk;
  wire            up_rstn;
  wire            delay_rst;

  // internal signals

  wire            adc_valid_s;
  wire    [47:0]  adc_data_s;
  wire            adc_status_s;
  wire            adc_r1_mode_s;
  wire            dac_valid_s;
  wire    [47:0]  dac_data_s;
  wire            dac_r1_mode_s;
  wire            delay_sel_s;
  wire            delay_rwn_s;
  wire    [ 7:0]  delay_addr_s;
  wire    [ 4:0]  delay_wdata_s;
  wire    [ 4:0]  delay_rdata_s;
  wire            delay_ack_t_s;
  wire            delay_locked_s;
  wire            dac_delay_rst_s;
  wire            dac_delay_sel_s;
  wire            dac_delay_rwn_s;
  wire    [ 7:0]  dac_delay_addr_s;
  wire    [ 4:0]  dac_delay_wdata_s;
  //wire    [ 4:0]  dac_delay_rdata_s;
  //wire            dac_delay_ack_t_s;
  wire            up_sel_s;
  wire            up_wr_s;
  wire    [13:0]  up_addr_s;
  wire    [31:0]  up_wdata_s;
  wire    [ 3:0]  up_wstrb_s;
  wire    [31:0]  up_rdata_rx_s;
  wire            up_ack_rx_s;
  wire    [31:0]  up_rdata_tx_s;
  wire            up_ack_tx_s;

  wire            DesignClkEn_s;
  wire            SoftSync_s;
  wire    [ 4:0]  v5_RefClkIODelay_s;
  wire    [ 4:0]  v5_SyncIDelay_s;
  wire    [ 4:0]  v5_SyncODelay_s;
  wire    [ 3:0]  v4_SyncSel_s;
  wire            SyncBitSlip_s;
  wire            RxBitSlip_s;
  wire            TxBitSlip_s;

  // signal name changes

  assign up_clk = s_axi_aclk;
  assign up_rstn = s_axi_aresetn;

  // processor read interface

  always @(negedge up_rstn or posedge up_clk) begin
    if (up_rstn == 0) begin
      up_rdata <= 'd0;
      up_ack <= 'd0;
    end else begin
      up_rdata <= up_rdata_rx_s | up_rdata_tx_s | iv32_up_rdata_p;
      up_ack <= up_ack_rx_s | up_ack_tx_s | i_up_ack_p;
    end
  end

  // device interface

  axi_ad9361_dev_if #(
    .PCORE_DEVICE_TYPE (PCORE_DEVICE_TYPE),
    .C_CLOCK_MASTER(C_CLOCK_MASTER))
  i_dev_if (
    .rx_clk_in_p (rx_clk_in_p),
    .rx_clk_in_n (rx_clk_in_n),
    .rx_frame_in_p (rx_frame_in_p),
    .rx_frame_in_n (rx_frame_in_n),
    .rx_data_in_p (rx_data_in_p),
    .rx_data_in_n (rx_data_in_n),
    .tx_clk_out_p (tx_clk_out_p),
    .tx_clk_out_n (tx_clk_out_n),
    .tx_frame_out_p (tx_frame_out_p),
    .tx_frame_out_n (tx_frame_out_n),
    .tx_data_out_p (tx_data_out_p),
    .tx_data_out_n (tx_data_out_n),
    .rst (rst),
    .i_DesignClkEn_p (DesignClkEn_s),
    .l_clk (l_clk),
    .o_DivClk_p (o_DivClk_p),
    .clk (clk),
    .adc_valid (adc_valid_s),
    .adc_data (adc_data_s),
    .adc_status (adc_status_s),
    .adc_r1_mode (adc_r1_mode_s),
    .dac_valid (dac_valid_s),
    .dac_data (dac_data_s),
    .dac_r1_mode (dac_r1_mode_s),
    .i_RxBitSlip_p (RxBitSlip_s),
    .i_TxBitSlip_p (TxBitSlip_s),
    .delay_clk (delay_clk),
    .delay_rst (delay_rst),
    .delay_sel (delay_sel_s),
    .delay_rwn (delay_rwn_s),
    .delay_addr (delay_addr_s),
    .delay_wdata (delay_wdata_s),
    .delay_rdata (delay_rdata_s),
    .delay_ack_t (delay_ack_t_s),
    .delay_locked (delay_locked_s),
    .dac_delay_rst (dac_delay_rst_s),
    .dac_delay_sel (dac_delay_sel_s),
    .dac_delay_rwn (dac_delay_rwn_s),
    .dac_delay_addr (dac_delay_addr_s),
    .dac_delay_wdata (dac_delay_wdata_s),
    //.dac_delay_rdata (dac_delay_rdata_s),
    //.dac_delay_ack_t (dac_delay_ack_t_s),
    .dev_dbg_data (dev_dbg_data),
    .dev_l_dbg_data (dev_l_dbg_data));

  // receive

  axi_ad9361_rx #(
    .PCORE_ID (PCORE_ID),
    .DP_DISABLE (PCORE_ADC_DP_DISABLE))
  i_rx (
    .adc_rst (rst),
    .adc_clk (clk),
    .adc_valid (adc_valid_s),
    .adc_data (adc_data_s),
    .adc_status (adc_status_s),
    .adc_r1_mode (adc_r1_mode_s),
    .dac_data (dac_data_s),
    .delay_clk (delay_clk),
    .delay_rst (delay_rst),
    .delay_sel (delay_sel_s),
    .delay_rwn (delay_rwn_s),
    .delay_addr (delay_addr_s),
    .delay_wdata (delay_wdata_s),
    .delay_rdata (delay_rdata_s),
    .delay_ack_t (delay_ack_t_s),
    .delay_locked (delay_locked_s),
    .adc_enable_i0 (adc_enable_i0),
    .adc_valid_i0 (adc_valid_i0),
    .adc_data_i0 (adc_data_i0),
    .adc_enable_q0 (adc_enable_q0),
    .adc_valid_q0 (adc_valid_q0),
    .adc_data_q0 (adc_data_q0),
    .adc_enable_i1 (adc_enable_i1),
    .adc_valid_i1 (adc_valid_i1),
    .adc_data_i1 (adc_data_i1),
    .adc_enable_q1 (adc_enable_q1),
    .adc_valid_q1 (adc_valid_q1),
    .adc_data_q1 (adc_data_q1),
    .adc_dovf (adc_dovf),
    .adc_dunf (adc_dunf),
    .up_adc_gpio_in (up_adc_gpio_in),
    .up_adc_gpio_out (up_adc_gpio_out),
    .o_RealCoefficient0_p (o_RealCoefficient0_Rx_p),
    .o_ImgCoefficient0_p (o_ImgCoefficient0_Rx_p),
    .o_RealCoefficient1_p (o_RealCoefficient1_Rx_p),
    .o_ImgCoefficient1_p (o_ImgCoefficient1_Rx_p),
    .o_DataDelay0_p (o_DataDelay0_Rx_p),
    .o_DataDelay1_p (o_DataDelay1_Rx_p),  
    .o_DesignClkEn_p (DesignClkEn_s),
    .o_Sync_p (SoftSync_s),
    .ov5_RefClkIODelay_p (v5_RefClkIODelay_s),
    .ov5_SyncIDelay_p (v5_SyncIDelay_s),
    .ov5_SyncODelay_p (v5_SyncODelay_s),
    .ov4_SyncSel_p (v4_SyncSel_s),
    .o_SyncBitSlip_p (SyncBitSlip_s),
    .o_RxBitSlip_p (RxBitSlip_s),
    .o_TxBitSlip_p (TxBitSlip_s),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_sel (up_sel_s),
    .up_wr (up_wr_s),
    .up_addr (up_addr_s),
    .up_wdata (up_wdata_s),
    .up_rdata (up_rdata_rx_s),
    .up_ack (up_ack_rx_s));


    sync_logic #(
        .C_CLOCK_MASTER (C_CLOCK_MASTER),
        .PCORE_DEVICE_TYPE (PCORE_DEVICE_TYPE))
    i_sync_logic (
        .idp_RefClk_p (idp_RefClk_p),
        .idn_RefClk_p (idn_RefClk_p),
        .i_SyncPin_p (i_SyncPin_p),
        .o_Sync_p (o_Sync_p),
        .o_RefClkToSlave_p (o_RefClkToSlave_p),
        .o_SyncToSlave_p (o_SyncToSlave_p),
        .i_RefClkFromMaster_p (i_RefClkFromMaster_p),
        .i_SyncFromMaster_p (i_SyncFromMaster_p),
        .i_DelayClk_p (delay_clk),
        .i_SystemClk_p (i_SystemClk_p),
        .i_SoftSync_p (SoftSync_s),
        .iv5_RefClkIODelay_p (v5_RefClkIODelay_s),
        .iv5_InputSyncDelay_p (v5_SyncIDelay_s),
        .iv5_OutputSyncDelay_p (v5_SyncODelay_s),
        .iv4_SyncSel_p (v4_SyncSel_s),
        .i_SyncBitSlip_p (SyncBitSlip_s),
        .i_UpdateInputSyncLatch_p      (i_UpdateInputSyncLatch_p),
        .i_UpdateOutputSyncLatch_p     (i_UpdateOutputSyncLatch_p),
        .o_InputSync_p                 (o_InputSync_p),
        .o_InputSyncLow_p              (o_InputSyncLow_p),
        .o_InputSyncHigh_p             (o_InputSyncHigh_p),
        .o_OutputSyncLow_p             (o_OutputSyncLow_p),
        .o_OutputSyncHigh_p            (o_OutputSyncHigh_p)
        );

  // transmit

  axi_ad9361_tx #(
    .PCORE_ID (PCORE_ID),
    .DP_DISABLE (PCORE_DAC_DP_DISABLE))
  i_tx (
    .delay_clk (delay_clk),
    .delay_rst (dac_delay_rst_s),
    .delay_sel (dac_delay_sel_s),
    .delay_rwn (dac_delay_rwn_s),
    .delay_addr (dac_delay_addr_s),
    .delay_wdata (dac_delay_wdata_s),
    //.delay_rdata (dac_delay_rdata_s),
    //.delay_ack_t (dac_delay_ack_t_s),
    .dac_clk (clk),
    .dac_valid (dac_valid_s),
    .dac_data (dac_data_s),
    .dac_r1_mode (dac_r1_mode_s),
    .adc_data (adc_data_s),
    .adc_valid (adc_valid_s),
    .dac_enable_i0 (dac_enable_i0),
    .dac_valid_i0 (dac_valid_i0),
    .dac_data_i0 (dac_data_i0),
    .dac_enable_q0 (dac_enable_q0),
    .dac_valid_q0 (dac_valid_q0),
    .dac_data_q0 (dac_data_q0),
    .dac_enable_i1 (dac_enable_i1),
    .dac_valid_i1 (dac_valid_i1),
    .dac_data_i1 (dac_data_i1),
    .dac_enable_q1 (dac_enable_q1),
    .dac_valid_q1 (dac_valid_q1),
    .dac_data_q1 (dac_data_q1),
    .dac_dovf(dac_dovf),
    .dac_dunf(dac_dunf),
    .o_RealCoefficient0_p (o_RealCoefficient0_Tx_p),
    .o_ImgCoefficient0_p (o_ImgCoefficient0_Tx_p),
    .o_RealCoefficient1_p (o_RealCoefficient1_Tx_p),
    .o_ImgCoefficient1_p (o_ImgCoefficient1_Tx_p),
    .o_DataDelay0_p (o_DataDelay0_Tx_p),
    .o_DataDelay1_p (o_DataDelay1_Tx_p),  
    .up_dac_gpio_in (up_dac_gpio_in),
    .up_dac_gpio_out (up_dac_gpio_out),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_sel (up_sel_s),
    .up_wr (up_wr_s),
    .up_addr (up_addr_s),
    .up_wdata (up_wdata_s),
    .up_rdata (up_rdata_tx_s),
    .up_ack (up_ack_tx_s));

  // axi interface

  up_axi i_up_axi (
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_axi_awvalid (s_axi_awvalid),
    .up_axi_awaddr (s_axi_awaddr),
    .up_axi_awready (s_axi_awready),
    .up_axi_wvalid (s_axi_wvalid),
    .up_axi_wdata (s_axi_wdata),
    .up_axi_wstrb (s_axi_wstrb),
    .up_axi_wready (s_axi_wready),
    .up_axi_bvalid (s_axi_bvalid),
    .up_axi_bresp (s_axi_bresp),
    .up_axi_bready (s_axi_bready),
    .up_axi_arvalid (s_axi_arvalid),
    .up_axi_araddr (s_axi_araddr),
    .up_axi_arready (s_axi_arready),
    .up_axi_rvalid (s_axi_rvalid),
    .up_axi_rresp (s_axi_rresp),
    .up_axi_rdata (s_axi_rdata),
    .up_axi_rready (s_axi_rready),
    .up_sel (up_sel_s),
    .up_wr (up_wr_s),
    .up_addr (up_addr_s),
    .up_wdata (up_wdata_s),
    .up_wstrb (up_wstrb_s),
    .up_rdata (up_rdata),
    .up_ack (up_ack));
    
    
  assign o_up_sel_p = up_sel_s;
  assign o_up_wr_p = up_wr_s;
  assign ov14_up_addr_p = up_addr_s;
  assign ov32_up_wdata_p = up_wdata_s;
  assign ov4_up_wstrb_p = up_wstrb_s;

endmodule

// ***************************************************************************
// ***************************************************************************
