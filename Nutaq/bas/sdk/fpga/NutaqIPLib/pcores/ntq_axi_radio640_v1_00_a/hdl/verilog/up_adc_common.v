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

module up_adc_common (

  // clock reset

  mmcm_rst,

  // adc interface

  adc_clk,
  adc_rst,
  adc_r1_mode,
  adc_ddr_edgesel,
  adc_pin_mode,
  adc_status,
  adc_status_ovf,
  adc_status_unf,
  adc_clk_ratio,

  // channel interface

  up_status_pn_err,
  up_status_pn_oos,
  up_status_or,

  // delay interface

  delay_clk,
  delay_rst,
  delay_sel,
  delay_rwn,
  delay_addr,
  delay_wdata,
  delay_rdata,
  delay_ack_t,
  delay_locked,

  // drp interface

  drp_clk,
  drp_rst,
  drp_sel,
  drp_wr,
  drp_addr,
  drp_wdata,
  drp_rdata,
  drp_ready,
  drp_locked,

  // user channel control

  up_usr_chanmax,
  adc_usr_chanmax,
  up_adc_gpio_in,
  up_adc_gpio_out,
  o_RealCoefficient0_p,
  o_ImgCoefficient0_p,
  o_RealCoefficient1_p,
  o_ImgCoefficient1_p,
  o_DataDelay0_p,
  o_DataDelay1_p,
  o_DesignClkEn_p,
  o_Sync_p,
  ov5_RefClkIODelay_p,
  ov5_SyncIDelay_p,
  ov5_SyncODelay_p,
  ov4_SyncSel_p,
  o_SyncBitSlip_p,
  o_RxBitSlip_p,
  o_TxBitSlip_p,

  // bus interface

  up_rstn,
  up_clk,
  up_sel,
  up_wr,
  up_addr,
  up_wdata,
  up_rdata,
  up_ack);

  // parameters

  localparam  PCORE_NTQ_ID = 16'hF640;
  localparam  PCORE_VERSION = 16'h0861;
  parameter   PCORE_ID = 0;

  // clock reset

  output          mmcm_rst;

  // adc interface

  input           adc_clk;
  output          adc_rst;
  output          adc_r1_mode;
  output          adc_ddr_edgesel;
  output          adc_pin_mode;
  input           adc_status;
  input           adc_status_ovf;
  input           adc_status_unf;
  input   [31:0]  adc_clk_ratio;

  // channel interface

  input           up_status_pn_err;
  input           up_status_pn_oos;
  input           up_status_or;

  // delay interface

  input           delay_clk;
  output          delay_rst;
  output          delay_sel;
  output          delay_rwn;
  output  [ 7:0]  delay_addr;
  output  [ 4:0]  delay_wdata;
  input   [ 4:0]  delay_rdata;
  input           delay_ack_t;
  input           delay_locked;

  // drp interface

  input           drp_clk;
  output          drp_rst;
  output          drp_sel;
  output          drp_wr;
  output  [11:0]  drp_addr;
  output  [15:0]  drp_wdata;
  input   [15:0]  drp_rdata;
  input           drp_ready;
  input           drp_locked;

  // user channel control

  output  [ 7:0]  up_usr_chanmax;
  input   [ 7:0]  adc_usr_chanmax;
  input   [31:0]  up_adc_gpio_in;
  output  [31:0]  up_adc_gpio_out;
  output  [15:0]  o_RealCoefficient0_p;
  output  [15:0]  o_ImgCoefficient0_p;
  output  [15:0]  o_RealCoefficient1_p;
  output  [15:0]  o_ImgCoefficient1_p;
  output  [31:0]  o_DataDelay0_p;
  output  [31:0]  o_DataDelay1_p; 
  
  output          o_DesignClkEn_p;
  output          o_Sync_p;
  output  [ 4:0]  ov5_RefClkIODelay_p;
  output  [ 4:0]  ov5_SyncIDelay_p;
  output  [ 4:0]  ov5_SyncODelay_p;
  output  [ 3:0]  ov4_SyncSel_p;
  output          o_SyncBitSlip_p;
  output          o_RxBitSlip_p;
  output          o_TxBitSlip_p;

  // bus interface

  input           up_rstn;
  input           up_clk;
  input           up_sel;
  input           up_wr;
  input   [13:0]  up_addr;
  input   [31:0]  up_wdata;
  output  [31:0]  up_rdata;
  output          up_ack;

  // internal registers

  reg     [31:0]  up_scratch = 'd0;
  reg             up_mmcm_resetn = 'd0;
  reg             up_resetn = 'd0;
  reg             up_adc_r1_mode = 'd0;
  reg             up_adc_ddr_edgesel = 'd0;
  reg             up_adc_pin_mode = 'd0;
  reg             up_delay_sel = 'd0;
  reg             up_delay_rwn = 'd0;
  reg     [ 7:0]  up_delay_addr = 'd0;
  reg     [ 4:0]  up_delay_wdata = 'd0;
  reg             up_drp_sel_t = 'd0;
  reg             up_drp_rwn = 'd0;
  reg     [11:0]  up_drp_addr = 'd0;
  reg     [15:0]  up_drp_wdata = 'd0;
  reg             up_status_ovf = 'd0;
  reg             up_status_unf = 'd0;
  reg     [ 7:0]  up_usr_chanmax = 'd0;
  reg     [31:0]  up_adc_gpio_out = 'd0;
  reg     [15:0]  o_RealCoefficient0_p = 'h7fff;
  reg     [15:0]  o_ImgCoefficient0_p = 'd0;
  reg     [15:0]  o_RealCoefficient1_p = 'h7fff;
  reg     [15:0]  o_ImgCoefficient1_p = 'd0;
  reg     [31:0]  o_DataDelay0_p = 'd0;
  reg     [31:0]  o_DataDelay1_p = 'd0;
  
  reg             o_DesignClkEn_p = 'd0;
  reg             o_Sync_p = 'd0;
  
  reg     [ 4:0]  ov5_RefClkIODelay_p = 'd0;
  reg     [ 4:0]  ov5_SyncIDelay_p = 'd0;
  reg     [ 4:0]  ov5_SyncODelay_p = 'd0;
  reg     [ 3:0]  ov4_SyncSel_p = 'd0;
  reg             o_SyncBitSlip_p = 'd0;
  reg             o_RxBitSlip_p = 'd0;
  reg             o_TxBitSlip_p = 'd0;
  
  reg             up_ack = 'd0;
  reg     [31:0]  up_rdata = 'd0;
  reg             up_preset = 'd0;
  reg             up_preset_r1 = 'd0;
  reg             drp_rst = 'd0;
  reg             delay_rst = 'd0;
  reg             adc_rst = 'd0;
  //reg             drp_rst_r1 = 'd0;
  //reg             delay_rst_r1 = 'd0;
  //reg             adc_rst_r1 = 'd0;

  // internal signals

  wire            up_sel_s;
  wire            up_wr_s;
  wire            up_mmcm_preset_s;
  wire            up_status_s;
  wire            up_status_ovf_s;
  wire            up_status_unf_s;
  wire    [31:0]  up_adc_clk_count_s;
  wire    [ 4:0]  up_delay_rdata_s;
  wire            up_delay_status_s;
  wire            up_delay_locked_s;
  wire    [15:0]  up_drp_rdata_s;
  wire            up_drp_status_s;
  wire            up_drp_locked_s;
  wire            drp_rst_r0;
  wire            delay_rst_r0;
  wire            adc_rst_r0;

  // decode block select

  assign up_sel_s = (up_addr[13:8] == 6'h00) ? up_sel : 1'b0;
  assign up_wr_s = up_sel_s & up_wr;
  assign up_mmcm_preset_s = ~up_mmcm_resetn;

  //Add double delay to ease timing on this high
  //fanout non-timing sensitive signal
  always @(posedge up_clk) begin
      up_preset    <= ~up_resetn;
      up_preset_r1 <= up_preset;
  end
  
  // processor write interface
  always @(negedge up_rstn or posedge up_clk) begin
    if (up_rstn == 0) begin
      up_scratch <= 'd0;
      up_mmcm_resetn <= 'd0;
      up_resetn <= 'd0;
      up_adc_r1_mode <= 'd0;
      up_adc_ddr_edgesel <= 'd0;
      up_adc_pin_mode <= 'd0;
      up_delay_sel <= 'd0;
      up_delay_rwn <= 'd0;
      up_delay_addr <= 'd0;
      up_delay_wdata <= 'd0;
      up_drp_sel_t <= 'd0;
      up_drp_rwn <= 'd0;
      up_drp_addr <= 'd0;
      up_drp_wdata <= 'd0;
      up_status_ovf <= 'd0;
      up_status_unf <= 'd0;
      up_usr_chanmax <= 'd0;
      up_adc_gpio_out <= 'd0;
      o_RealCoefficient0_p <= 'h7fff;
      o_ImgCoefficient0_p <= 'd0;
      o_RealCoefficient1_p <= 'h7fff;
      o_ImgCoefficient1_p <= 'd0;    
      o_DataDelay0_p <= 'd0;
      o_DataDelay1_p <= 'd0;
      o_DesignClkEn_p <= 'd0;
      o_Sync_p <= 'd0;
      ov5_RefClkIODelay_p <= 'd0;
      ov5_SyncIDelay_p <= 'd0;
      ov5_SyncODelay_p <= 'd0;
      ov4_SyncSel_p <= 'd0;
      o_SyncBitSlip_p <= 'd0;
      o_RxBitSlip_p <= 'd0;
      o_TxBitSlip_p <= 'd0;
      
    end else begin
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h02)) begin
        up_scratch <= up_wdata;
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h10)) begin
        up_mmcm_resetn <= up_wdata[1];
        up_resetn <= up_wdata[0];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h11)) begin
        up_adc_r1_mode <= up_wdata[2];
        up_adc_ddr_edgesel <= up_wdata[1];
        up_adc_pin_mode <= up_wdata[0];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h18)) begin
        up_delay_sel <= up_wdata[17];
        up_delay_rwn <= up_wdata[16];
        up_delay_addr <= up_wdata[15:8];
        up_delay_wdata <= up_wdata[4:0];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h1c)) begin
        up_drp_sel_t <= ~up_drp_sel_t;
        up_drp_rwn <= up_wdata[28];
        up_drp_addr <= up_wdata[27:16];
        up_drp_wdata <= up_wdata[15:0];
      end
      if (up_status_ovf_s == 1'b1) begin
        up_status_ovf <= 1'b1;
      end else if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h22)) begin
        up_status_ovf <= up_status_ovf & ~up_wdata[2];
      end
      if (up_status_unf_s == 1'b1) begin
        up_status_unf <= 1'b1;
      end else if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h22)) begin
        up_status_unf <= up_status_unf & ~up_wdata[1];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h28)) begin
        up_usr_chanmax <= up_wdata[7:0];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h2f)) begin
        up_adc_gpio_out <= up_wdata;
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h30)) begin
        o_DesignClkEn_p <= up_wdata[0];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h31)) begin
        ov5_RefClkIODelay_p <= up_wdata[4:0];
        ov5_SyncODelay_p   <= up_wdata[9:5];
        ov5_SyncIDelay_p   <= up_wdata[14:10];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h32)) begin
        o_Sync_p <= up_wdata[0];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h33)) begin
        ov4_SyncSel_p <= up_wdata[3:0];
        o_SyncBitSlip_p <= up_wdata[4];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h34)) begin
        o_RxBitSlip_p <= up_wdata[0];
        o_TxBitSlip_p <= up_wdata[1];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h35)) begin
        o_RealCoefficient0_p <= up_wdata[15:0];
        o_ImgCoefficient0_p <= up_wdata[31:16];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h36)) begin
        o_RealCoefficient1_p <= up_wdata[15:0];
        o_ImgCoefficient1_p <= up_wdata[31:16];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h37)) begin
        o_DataDelay0_p <= up_wdata[31:0];
      end
      if ((up_wr_s == 1'b1) && (up_addr[7:0] == 8'h38)) begin
        o_DataDelay1_p <= up_wdata[31:0];
      end    
    end
  end

  // processor read interface

  always @(negedge up_rstn or posedge up_clk) begin
    if (up_rstn == 0) begin
      up_ack <= 'd0;
      up_rdata <= 'd0;
    end else begin
      up_ack <= up_sel_s;
      if (up_sel_s == 1'b1) begin
        case (up_addr[7:0])
          8'h00: up_rdata <= {PCORE_NTQ_ID, PCORE_VERSION};
          8'h01: up_rdata <= PCORE_ID;
          8'h02: up_rdata <= up_scratch;
          8'h10: up_rdata <= {30'd0, up_mmcm_resetn, up_resetn};
          8'h11: up_rdata <= {29'd0, up_adc_r1_mode, up_adc_ddr_edgesel, up_adc_pin_mode};
          8'h15: up_rdata <= up_adc_clk_count_s;
          8'h16: up_rdata <= adc_clk_ratio;
          8'h17: up_rdata <= {28'd0, up_status_pn_err, up_status_pn_oos, up_status_or, up_status_s};
          8'h18: up_rdata <= {14'd0, up_delay_sel, up_delay_rwn, up_delay_addr, 3'd0, up_delay_wdata};
          8'h19: up_rdata <= {22'd0, up_delay_locked_s, up_delay_status_s, 3'd0, up_delay_rdata_s};
          8'h1c: up_rdata <= {3'd0, up_drp_rwn, up_drp_addr, up_drp_wdata};
          8'h1d: up_rdata <= {14'd0, up_drp_locked_s, up_drp_status_s, up_drp_rdata_s};
          8'h22: up_rdata <= {29'd0, up_status_ovf, up_status_unf, 1'b0};
          8'h23: up_rdata <= 32'd8;
          8'h28: up_rdata <= {24'd0, adc_usr_chanmax};
          8'h2e: up_rdata <= up_adc_gpio_in;
          8'h2f: up_rdata <= up_adc_gpio_out;
          8'h30: up_rdata <= {31'd0, o_DesignClkEn_p};
          8'h31: up_rdata <= {17'd0, ov5_SyncIDelay_p, ov5_SyncODelay_p, ov5_RefClkIODelay_p};
          8'h32: up_rdata <= {31'd0, o_Sync_p};
          8'h33: up_rdata <= {27'd0, o_SyncBitSlip_p, ov4_SyncSel_p};
          8'h34: up_rdata <= {30'd0, o_TxBitSlip_p, o_RxBitSlip_p};
          8'h35: up_rdata <= {o_ImgCoefficient0_p, o_RealCoefficient0_p};
          8'h36: up_rdata <= {o_ImgCoefficient1_p, o_RealCoefficient1_p};       
          8'h37: up_rdata <= {o_DataDelay0_p};
          8'h38: up_rdata <= {o_DataDelay1_p};             
          default: up_rdata <= 0;
        endcase
      end else begin
        up_rdata <= 32'd0;
      end
    end
  end

  // resets

  ad_rst i_mmcm_rst_reg   (.preset(up_mmcm_preset_s), .clk(drp_clk),    .rst(mmcm_rst));
  ad_rst i_adc_rst_reg    (.preset(up_preset_r1),      .clk(adc_clk),    .rst(adc_rst_r0));
  ad_rst i_delay_rst_reg  (.preset(up_preset_r1),      .clk(delay_clk),  .rst(delay_rst_r0));
  ad_rst i_drp_rst_reg    (.preset(up_preset_r1),      .clk(drp_clk),    .rst(drp_rst_r0));

  //Add register stages to ease timing
  always @(posedge adc_clk) begin
      adc_rst    <= adc_rst_r0;
  end
  always @(posedge delay_clk) begin
      delay_rst    <= delay_rst_r0;
  end
  always @(posedge drp_clk) begin
      drp_rst    <= drp_rst_r0;
  end
  
  // adc control & status


  up_xfer_cntrl #(.DATA_WIDTH(3)) i_adc_xfer_cntrl (
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_data_cntrl ({ up_adc_r1_mode,
                      up_adc_ddr_edgesel,
                      up_adc_pin_mode}),
    .up_xfer_done (),
    .d_rst (adc_rst),
    .d_clk (adc_clk),
    .d_data_cntrl ({  adc_r1_mode,
                      adc_ddr_edgesel,
                      adc_pin_mode}));

  up_xfer_status #(.DATA_WIDTH(3)) i_adc_xfer_status (
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_data_status ({up_status_s,
                      up_status_ovf_s,
                      up_status_unf_s}),
    .d_rst (adc_rst),
    .d_clk (adc_clk),
    .d_data_status ({ adc_status,
                      adc_status_ovf,
                      adc_status_unf}));

  // adc clock monitor

  up_clock_mon i_adc_clock_mon (
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_d_count (up_adc_clk_count_s),
    .d_rst (adc_rst),
    .d_clk (adc_clk));

  // delay control & status

  up_delay_cntrl i_delay_cntrl (
    .delay_clk (delay_clk),
    .delay_rst (delay_rst),
    .delay_sel (delay_sel),
    .delay_rwn (delay_rwn),
    .delay_addr (delay_addr),
    .delay_wdata (delay_wdata),
    .delay_rdata (delay_rdata),
    .delay_ack_t (delay_ack_t),
    .delay_locked (delay_locked),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_delay_sel (up_delay_sel),
    .up_delay_rwn (up_delay_rwn),
    .up_delay_addr (up_delay_addr),
    .up_delay_wdata (up_delay_wdata),
    .up_delay_rdata (up_delay_rdata_s),
    .up_delay_status (up_delay_status_s),
    .up_delay_locked (up_delay_locked_s));

  // drp control & status

  up_drp_cntrl i_drp_cntrl (
    .drp_clk (drp_clk),
    .drp_rst (drp_rst),
    .drp_sel (drp_sel),
    .drp_wr (drp_wr),
    .drp_addr (drp_addr),
    .drp_wdata (drp_wdata),
    .drp_rdata (drp_rdata),
    .drp_ready (drp_ready),
    .drp_locked (drp_locked),
    .up_rstn (up_rstn),
    .up_clk (up_clk),
    .up_drp_sel_t (up_drp_sel_t),
    .up_drp_rwn (up_drp_rwn),
    .up_drp_addr (up_drp_addr),
    .up_drp_wdata (up_drp_wdata),
    .up_drp_rdata (up_drp_rdata_s),
    .up_drp_status (up_drp_status_s),
    .up_drp_locked (up_drp_locked_s));

endmodule

// ***************************************************************************
// ***************************************************************************
