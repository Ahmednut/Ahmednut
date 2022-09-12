// ***************************************************************************
// ***************************************************************************
// Copyright 2014(c) Analog Devices, Inc.
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

module ad_lvds_out (

  i_DelayClk_p,
  iv5_Delay_p,
  delay_ld,
  // data interface

  tx_clk,
  tx_data_p,
  tx_data_n,
  tx_data_out_p,
  tx_data_out_n);

  // parameters

  parameter   LVDS_OUTPUT = 1;
  parameter   IODELAY_OUTPUT = 0;
  parameter   BUFTYPE = 0;
  localparam  SERIES7 = 0;
  localparam  VIRTEX6 = 1;
  
  input               delay_ld;
  input               i_DelayClk_p;
  input   [4:0]       iv5_Delay_p;

  // data interface
  input               tx_clk;
  input               tx_data_p;
  input               tx_data_n;
  output              tx_data_out_p;
  output              tx_data_out_n;

  // internal signals

  wire                tx_data_oddr_s;
  wire                tx_data_oddr_delay_s;

  // transmit data interface, oddr -> obuf

  ODDR #(
    .DDR_CLK_EDGE ("SAME_EDGE"),
    .INIT (1'b0),
    .SRTYPE ("ASYNC"))
  i_tx_data_oddr (
    .CE (1'b1),
    .R (1'b0),
    .S (1'b0),
    .C (tx_clk),
    .D1 (tx_data_p),
    .D2 (tx_data_n),
    .Q (tx_data_oddr_s));
    

  generate
  if (IODELAY_OUTPUT == 1 && BUFTYPE == VIRTEX6) begin //ODELAY not supported on 7-series HR banks
  IODELAYE1 #(
        .CINVCTRL_SEL ("FALSE"),
        .DELAY_SRC ("O"),
        .HIGH_PERFORMANCE_MODE ("TRUE"),
        .IDELAY_TYPE ("VAR_LOADABLE"),
        .IDELAY_VALUE (0),
        .ODELAY_TYPE ("VAR_LOADABLE"),
        .ODELAY_VALUE (0),
        .REFCLK_FREQUENCY (200.0),
        .SIGNAL_PATTERN ("DATA"))
  i_data_delay (
        .CNTVALUEOUT (),
        .DATAOUT (tx_data_oddr_delay_s),
        .C (i_DelayClk_p),
        .CE (1'b0),
        .INC (1'b0),
        .CINVCTRL (1'b0),
        .CLKIN (1'b0),
        .CNTVALUEIN (iv5_Delay_p),
        .DATAIN (1'b0),
        .IDATAIN (1'b0),
        .ODATAIN (tx_data_oddr_s),
        .RST (delay_ld),
        .T (1'b0));

  end else begin
  assign tx_data_oddr_delay_s = tx_data_oddr_s;
  
  end
  endgenerate
    
  generate 
  if (LVDS_OUTPUT == 1) begin
      OBUFDS i_tx_data_obuf (
        .I (tx_data_oddr_delay_s),
        .O (tx_data_out_p),
        .OB (tx_data_out_n));
  end else begin
      assign tx_data_out_p = tx_data_oddr_delay_s;
      assign tx_data_out_n = 1'b0;
  end
  endgenerate

endmodule

// ***************************************************************************
// ***************************************************************************
