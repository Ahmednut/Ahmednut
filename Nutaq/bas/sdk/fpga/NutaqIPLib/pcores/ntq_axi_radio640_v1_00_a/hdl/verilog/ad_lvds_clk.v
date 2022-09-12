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

module ad_lvds_clk (

  clk_in_p,
  clk_in_n,
  i_DesignClkEn_p,
  i_DivClkPulse_p,
  clk,
  o_DivClk_p);

  parameter   BUFTYPE       = 0;
  parameter   BUFRENABLE    = 1;
  localparam  SERIES7       = 0;
  localparam  VIRTEX6       = 1;

  input     clk_in_p;
  input     clk_in_n;
  input     i_DesignClkEn_p;
  input     i_DivClkPulse_p;
  output    clk;
  output    o_DivClk_p;

  // internal registers
  reg             designClkEn_r1 = 'd0;
  reg             designClkEn_r2 = 'd0;
  reg             divClkEn_r1 = 'd0;
  reg             divClkEn_r2 = 'd0;
  
  //synthesis attribute keep of DivClkPulseBufg_s is true; 
  //synthesis attribute keep of DivClkPulseBufg_R1_s is true; 
  //synthesis attribute keep of DivClkPulseBufg_R2_s is true; 
  reg             DivClkPulseBufg_s     = 'd0;
  reg             DivClkPulseBufg_R1_s  = 'd0;
  reg             DivClkPulseBufg_R2_s  = 'd0;
  
  //synthesis attribute keep of DivClkPulse_s is true; 
  //synthesis attribute keep of DivClkPulse_Pos_R1_s is true; 
  //synthesis attribute keep of DivClkPulse_Pos_R2_s is true; 
  reg             DivClkPulse_s         = 'd0;
  reg             DivClkPulse_Pos_R1_s  = 'd0;
  reg             DivClkPulse_Pos_R2_s  = 'd0;
  
  reg             DivClkPulse_R1_s = 'd0;
  reg             DivClkPulse_R2_s = 'd0;
  reg             DivClkPulse_R3_s = 'd0;
  reg             DivClkPulse_R4_s = 'd0;
  
  //synthesis attribute keep of divclk_clr_R2_s is true; 
  reg             divclk_clr_s = 'd0;
  reg             divclk_clr_R1_s = 'd0;
  reg             divclk_clr_R2_s = 'd0;

  // wires

  wire      clk_ibuf_s;
  wire      clk_bufr_s;
  wire      divclk_bufr_s;

  // instantiations

  IBUFGDS i_rx_clk_ibuf (
    .I (clk_in_p),
    .IB (clk_in_n),
    .O (clk_ibuf_s));

  always @(posedge clk) begin
      DivClkPulseBufg_s    <= i_DivClkPulse_p;
      DivClkPulseBufg_R1_s <= DivClkPulseBufg_s;
      DivClkPulseBufg_R2_s <= DivClkPulseBufg_R1_s;
  end

  always @(posedge clk_bufr_s) begin
      DivClkPulse_s        <= DivClkPulseBufg_R2_s;
      DivClkPulse_Pos_R1_s <= DivClkPulse_s;
      DivClkPulse_Pos_R2_s <= DivClkPulse_Pos_R1_s;
  end

  // Latch i_DesignClkEn_p 2 times on negative edge
  // to avoid clk glitch
  always @(negedge clk_bufr_s) begin
    designClkEn_r1 <= i_DesignClkEn_p;
    designClkEn_r2 <= designClkEn_r1;
    
    // Detect DAC Sync. At least 4 clock cycles low.
    DivClkPulse_R1_s <= DivClkPulse_Pos_R2_s;
    DivClkPulse_R2_s <= DivClkPulse_R1_s;
    DivClkPulse_R3_s <= DivClkPulse_R2_s;
    DivClkPulse_R4_s <= DivClkPulse_R3_s;
    if ((DivClkPulse_R1_s | DivClkPulse_R2_s | DivClkPulse_R3_s | DivClkPulse_R4_s) == 1'b0) begin
      divclk_clr_s <= 1'b1;
    end else if (DivClkPulse_R1_s == 1'b1) begin
      // On next DAC valid, clear the BUFR clear signal.
      divclk_clr_s <= 1'b0;
    end
    
    divclk_clr_R1_s <= divclk_clr_s;
    divclk_clr_R2_s <= divclk_clr_R1_s;
    
    if ((designClkEn_r1 == 1'b0) || (divclk_clr_s == 1'b1)) begin
      divClkEn_r1 <= 1'b0;
    end else begin
      divClkEn_r1 <= 1'b1;
    end
    divClkEn_r2 <= divClkEn_r1;
  end
    

  BUFR #(.BUFR_DIVIDE("1")) i_clk_rbuf (
    .CLR (1'b0),
    .CE (1'b1),
    .I (clk_ibuf_s),
    .O (clk_bufr_s));
  
  BUFR #(.BUFR_DIVIDE("4")) i_divclk_rbuf (
    .CLR (divclk_clr_R2_s),
    .CE (1'b1),
    .I (clk_ibuf_s),
    .O (divclk_bufr_s));      
      
  generate
  if(BUFRENABLE == 1) begin
    BUFGCE i_clk_gbuf (
      .CE (designClkEn_r2),
      .I (clk_bufr_s),
      .O (clk));
      
    BUFGCE i_divclk_gbuf (
      .CE (divClkEn_r2),
      .I (divclk_bufr_s),
      .O (o_DivClk_p));       
      
  end else begin
    BUFGCE i_clk_gbuf (
      .CE (1'b1),
      .I (clk_bufr_s),
      .O (clk));  
    
    BUFGCE i_divclk_gbuf (
      .CE (1'b1),
      .I (divclk_bufr_s),
      .O (o_DivClk_p));
            
  end
  endgenerate




endmodule

// ***************************************************************************
// ***************************************************************************
