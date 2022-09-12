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

module axi_ad9361_tx (

  // dac interface

  dac_clk,
  dac_valid,
  dac_data,
  dac_r1_mode,
  adc_data,
  adc_valid,
  
  // delay interface

  delay_clk,
  delay_rst,
  delay_sel,
  delay_rwn,
  delay_addr,
  delay_wdata,
  //delay_rdata,
  //delay_ack_t,

  // dma interface

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
  
  o_RealCoefficient0_p,
  o_ImgCoefficient0_p,
  o_RealCoefficient1_p,
  o_ImgCoefficient1_p,
  o_DataDelay0_p,
  o_DataDelay1_p,

  // gpio

  up_dac_gpio_in,
  up_dac_gpio_out,

  // processor interface

  up_rstn,
  up_clk,
  up_sel,
  up_wr,
  up_addr,
  up_wdata,
  up_rdata,
  up_ack);

  // parameters

  parameter   DP_DISABLE = 0;
  parameter   PCORE_ID = 0;

  // dac interface

  input           dac_clk;
  output          dac_valid;
  output  [47:0]  dac_data;
  output          dac_r1_mode;
  input   [47:0]  adc_data;
  input           adc_valid;
  
    // delay interface

  input           delay_clk;
  output          delay_rst;
  output          delay_sel;
  output          delay_rwn;
  output  [ 7:0]  delay_addr;
  output  [ 4:0]  delay_wdata;
  //input   [ 4:0]  delay_rdata;
  //input           delay_ack_t;

  // dma interface

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

  // gpio

  input   [31:0]  up_dac_gpio_in;
  output  [31:0]  up_dac_gpio_out;
  
  // complex coefficients
  output  [15:0]  o_RealCoefficient0_p;
  output  [15:0]  o_ImgCoefficient0_p;
  output  [15:0]  o_RealCoefficient1_p;
  output  [15:0]  o_ImgCoefficient1_p;
  output  [31:0]  o_DataDelay0_p;
  output  [31:0]  o_DataDelay1_p;

  // processor interface

  input           up_rstn;
  input           up_clk;
  input           up_sel;
  input           up_wr;
  input   [13:0]  up_addr;
  input   [31:0]  up_wdata;
  output  [31:0]  up_rdata;
  output          up_ack;

  // internal registers

  reg             dac_data_sync = 'd0;
  reg     [ 3:0]  dac_rate_cnt = 'd0;
  reg             dac_valid = 'd0;
  reg             dac_valid_i0 = 'd0;
  reg             dac_valid_q0 = 'd0;
  reg             dac_valid_i1 = 'd0;
  reg             dac_valid_q1 = 'd0;
  reg     [31:0]  up_rdata = 'd0;
  reg             up_ack = 'd0;
  
  
  //synthesis attribute max_fanout of dac_rate_cnt_equal_0 is 1;
  //synthesis attribute register_duplication of dac_rate_cnt_equal_0 is true; 
  //synthesis attribute equivalent_register_removal of dac_rate_cnt_equal_0 false;
  reg             dac_rate_cnt_equal_0 = 'd0;

  // internal clock and resets

  wire            dac_rst;

  // internal signals

  wire            dac_data_sync_s;
  wire    [ 7:0]  dac_datarate_s;
  wire    [47:0]  dac_data_int_s;
  wire    [31:0]  up_rdata_s[0:4];
  wire            up_ack_s[0:4];

  // There is no need that all boards have the same dac sync 
  // since the adc_valid a aligned with the digital calibration
  // By using the adc_valid, we are sure that all board dac_valid
  // are synchronized.
  //assign dac_data_sync_s = (PCORE_ID == 0) ? dac_sync_out : dac_sync_in;

  always @(posedge dac_clk) begin
    // Only sync synchronous to adc valid to 
    // keep constant round trip path between RX and TX
    if (adc_valid == 1'b1) begin
        dac_data_sync <= dac_data_sync_s;
    end
  end

  // rate counters and data sync signals

  always @(posedge dac_clk) begin
    if ((dac_data_sync == 1'b1) || (dac_rate_cnt_equal_0 == 1'd1)) begin
      dac_rate_cnt <= dac_datarate_s[3:0];
    end else begin
      dac_rate_cnt <= dac_rate_cnt - 1'b1;
    end
    
    if (dac_data_sync == 1'b1) begin
        dac_rate_cnt_equal_0 <= 1'b0;
    end else begin
        // Next clock cycle data_rate_cnt will be 0
        dac_rate_cnt_equal_0  <= (dac_rate_cnt == 4'd1) ? 1'b1 : 1'b0;
    end
  end

  // dma interface

  always @(posedge dac_clk) begin
    dac_valid <= dac_rate_cnt_equal_0;
    dac_valid_i0 <= dac_valid & dac_enable_i0;
    dac_valid_q0 <= dac_valid & dac_enable_q0;
    dac_valid_i1 <= dac_valid & dac_enable_i1 & ~dac_r1_mode;
    dac_valid_q1 <= dac_valid & dac_enable_q1 & ~dac_r1_mode;
  end

  // processor read interface

  always @(negedge up_rstn or posedge up_clk) begin
    if (up_rstn == 0) begin
      up_rdata <= 'd0;
      up_ack <= 'd0;
    end else begin
      up_rdata <= up_rdata_s[0] | up_rdata_s[1] | up_rdata_s[2] | up_rdata_s[3] | up_rdata_s[4];
      up_ack <= up_ack_s[0] | up_ack_s[1] | up_ack_s[2] | up_ack_s[3] | up_ack_s[4];
    end
  end

  // dac channel
  
  axi_ad9361_tx_channel #(
    .CHID (0),
    .IQSEL (0),
    .DP_DISABLE (DP_DISABLE))
  i_tx_channel_0 (
    .dac_clk (dac_clk),
    .dac_rst (dac_rst),
    .dac_valid (dac_valid),
    .dma_data (dac_data_i0),
    .adc_data (adc_data[11:0]),
    .dac_data (dac_data[11:0]),
    .dac_data_out (dac_data_int_s[11:0]),
    .dac_data_in (dac_data_int_s[23:12]),
    .dac_enable (dac_enable_i0),
    .dac_data_sync (dac_data_sync),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_sel (up_sel),
    .up_wr (up_wr),
    .up_addr (up_addr),
    .up_wdata (up_wdata),
    .up_rdata (up_rdata_s[0]),
    .up_ack (up_ack_s[0]));

  // dac channel
  
  axi_ad9361_tx_channel #(
    .CHID (1),
    .IQSEL (1),
    .DP_DISABLE (DP_DISABLE))
  i_tx_channel_1 (
    .dac_clk (dac_clk),
    .dac_rst (dac_rst),
    .dac_valid (dac_valid),
    .dma_data (dac_data_q0),
    .adc_data (adc_data[23:12]),
    .dac_data (dac_data[23:12]),
    .dac_data_out (dac_data_int_s[23:12]),
    .dac_data_in (dac_data_int_s[11:0]),
    .dac_enable (dac_enable_q0),
    .dac_data_sync (dac_data_sync),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_sel (up_sel),
    .up_wr (up_wr),
    .up_addr (up_addr),
    .up_wdata (up_wdata),
    .up_rdata (up_rdata_s[1]),
    .up_ack (up_ack_s[1]));

  // dac channel
  
  axi_ad9361_tx_channel #(
    .CHID (2),
    .IQSEL (0),
    .DP_DISABLE (DP_DISABLE))
  i_tx_channel_2 (
    .dac_clk (dac_clk),
    .dac_rst (dac_rst),
    .dac_valid (dac_valid),
    .dma_data (dac_data_i1),
    .adc_data (adc_data[35:24]),
    .dac_data (dac_data[35:24]),
    .dac_data_out (dac_data_int_s[35:24]),
    .dac_data_in (dac_data_int_s[47:36]),
    .dac_enable (dac_enable_i1),
    .dac_data_sync (dac_data_sync),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_sel (up_sel),
    .up_wr (up_wr),
    .up_addr (up_addr),
    .up_wdata (up_wdata),
    .up_rdata (up_rdata_s[2]),
    .up_ack (up_ack_s[2]));

  // dac channel
  
  axi_ad9361_tx_channel #(
    .CHID (3),
    .IQSEL (1),
    .DP_DISABLE (DP_DISABLE))
  i_tx_channel_3 (
    .dac_clk (dac_clk),
    .dac_rst (dac_rst),
    .dac_valid (dac_valid),
    .dma_data (dac_data_q1),
    .adc_data (adc_data[47:36]),
    .dac_data (dac_data[47:36]),
    .dac_data_out (dac_data_int_s[47:36]),
    .dac_data_in (dac_data_int_s[35:24]),
    .dac_enable (dac_enable_q1),
    .dac_data_sync (dac_data_sync),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_sel (up_sel),
    .up_wr (up_wr),
    .up_addr (up_addr),
    .up_wdata (up_wdata),
    .up_rdata (up_rdata_s[3]),
    .up_ack (up_ack_s[3]));

  // dac common processor interface

  up_dac_common #(.PCORE_ID (PCORE_ID)) i_up_dac_common (
    .mmcm_rst (),
    .dac_clk (dac_clk),
    .dac_rst (dac_rst),
    .dac_sync (dac_data_sync_s),
    .dac_frame (),
    .dac_par_type (),
    .dac_par_enb (),
    .dac_r1_mode (dac_r1_mode),
    .dac_datafmt (),
    .dac_datarate (dac_datarate_s),
    .dac_status (1'b1),
    .dac_status_ovf (dac_dovf),
    .dac_status_unf (dac_dunf),
    .dac_clk_ratio (32'd1),
    .drp_clk (1'b0),
    .drp_rst (),
    .drp_sel (),
    .drp_wr (),
    .drp_addr (),
    .drp_wdata (),
    .drp_rdata (16'd0),
    .drp_ready (1'd0),
    .drp_locked (1'd1),
    .delay_clk (delay_clk),
    .delay_rst (delay_rst),
    .delay_sel (delay_sel),
    .delay_rwn (delay_rwn),
    .delay_addr (delay_addr),
    .delay_wdata (delay_wdata),
    //.delay_rdata (delay_rdata),
    //.delay_ack_t (delay_ack_t),
    .up_usr_chanmax (),
    .dac_usr_chanmax (8'd3),
    .o_RealCoefficient0_p (o_RealCoefficient0_p),
    .o_ImgCoefficient0_p (o_ImgCoefficient0_p),
    .o_RealCoefficient1_p (o_RealCoefficient1_p),
    .o_ImgCoefficient1_p (o_ImgCoefficient1_p),
    .o_DataDelay0_p (o_DataDelay0_p),
    .o_DataDelay1_p (o_DataDelay1_p),
    .up_dac_gpio_in (up_dac_gpio_in),
    .up_dac_gpio_out (up_dac_gpio_out),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_sel (up_sel),
    .up_wr (up_wr),
    .up_addr (up_addr),
    .up_wdata (up_wdata),
    .up_rdata (up_rdata_s[4]),
    .up_ack (up_ack_s[4]));
  
endmodule

// ***************************************************************************
// ***************************************************************************
