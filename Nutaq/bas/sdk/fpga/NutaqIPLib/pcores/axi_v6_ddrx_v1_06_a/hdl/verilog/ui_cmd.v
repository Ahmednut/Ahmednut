//*****************************************************************************
// (c) Copyright 2008-2009 Xilinx, Inc. All rights reserved.
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
//   ____  ____
//  /   /\/   /
// /___/  \  /    Vendor                : Xilinx
// \   \   \/     Version               : %version
//  \   \         Application           : MIG
//  /   /         Filename              : ui_cmd.v
// /___/   /\     Date Last Modified    : $date$
// \   \  /  \    Date Created          : Tue Jun 30 2009
//  \___\/\___\
//
//Device            : Virtex-6
//Design Name       : DDR3 SDRAM
//Purpose           :
//Reference         :
//Revision History  :
//*****************************************************************************

`timescale 1 ps / 1 ps

// User interface command port.

module ui_cmd #
  (
   parameter TCQ = 100,
   parameter ADDR_WIDTH         = 33,
   parameter BANK_WIDTH         = 3,
   parameter COL_WIDTH          = 12,
   parameter RANK_WIDTH         = 2,
   parameter ROW_WIDTH          = 16,
   parameter RANKS              = 4,
   parameter MEM_ADDR_ORDER     = "BANK_ROW_COLUMN"
  )
  (/*AUTOARG*/
  // Outputs
  app_rdy, use_addr, rank, bank, row, col, size, cmd, hi_priority,
  rd_accepted, wr_accepted, data_buf_addr, app_rdy_user,
  // Inputs
  rst, clk, accept_ns, rd_buf_full, wr_req_16, app_addr, app_cmd,
  app_sz, app_hi_pri, app_en, wr_data_buf_addr, rd_data_buf_addr_r
  );
  
  // synthesis attribute register_duplication of ui_cmd is true; 
  // synthesis attribute equivalent_register_removal of ui_cmd false;
  // synthesis attribute MAX_FANOUT of app_rdy is 3;

  input rst;
  input clk;

  input accept_ns;
  input rd_buf_full;
  input wr_req_16;
  wire app_rdy_ns = accept_ns && ~rd_buf_full && ~wr_req_16;
  reg app_rdy_r;

  reg app_rdy_r1;    
  reg app_rdy_r2;
  reg app_rdy_r3;
  reg app_rdy_r4;
  reg app_rdy_r5;
  reg app_rdy_r6;
            
  reg app_rdy_r7;    
  reg app_rdy_r8;
  reg app_rdy_r9;
  reg app_rdy_r10;
  reg app_rdy_r11;
  reg app_rdy_r12;  
  
  reg app_rdy_r13;
  reg app_rdy_r14;    
  reg app_rdy_r15;
  reg app_rdy_r16;
            
  reg app_rdy_r17;    
  reg app_rdy_r18;
  reg app_rdy_r19;
  reg app_rdy_r20;
  
  //synthesis attribute max_fanout of app_rdy_r21 is 3;
  //synthesis attribute register_duplication of app_rdy_r21 is true; 
  //synthesis attribute equivalent_register_removal of app_rdy_r21 false;
  reg app_rdy_r21; /* synthesis syn_maxfan = 3 */;
  
  reg app_rdy_user_r;
  
  always @(posedge clk) app_rdy_r <= #TCQ app_rdy_ns;
  always @(posedge clk) app_rdy_user_r <= #TCQ app_rdy_ns;  
  
  always @(posedge clk) app_rdy_r1 <= #TCQ app_rdy_ns;   
  always @(posedge clk) app_rdy_r2 <= #TCQ app_rdy_ns;  
  always @(posedge clk) app_rdy_r3 <= #TCQ app_rdy_ns;  
  always @(posedge clk) app_rdy_r4 <= #TCQ app_rdy_ns;  
  always @(posedge clk) app_rdy_r5 <= #TCQ app_rdy_ns;        
  always @(posedge clk) app_rdy_r6 <= #TCQ app_rdy_ns;   
  always @(posedge clk) app_rdy_r7 <= #TCQ app_rdy_ns;  
  always @(posedge clk) app_rdy_r8 <= #TCQ app_rdy_ns;  
  always @(posedge clk) app_rdy_r9 <= #TCQ app_rdy_ns;  
  always @(posedge clk) app_rdy_r10 <= #TCQ app_rdy_ns;        
  always @(posedge clk) app_rdy_r11 <= #TCQ app_rdy_ns;   
  always @(posedge clk) app_rdy_r12 <= #TCQ app_rdy_ns;  
  always @(posedge clk) app_rdy_r13 <= #TCQ app_rdy_ns;  
  always @(posedge clk) app_rdy_r14 <= #TCQ app_rdy_ns;       
  always @(posedge clk) app_rdy_r15 <= #TCQ app_rdy_ns;    
  always @(posedge clk) app_rdy_r16 <= #TCQ app_rdy_ns;    
  always @(posedge clk) app_rdy_r17 <= #TCQ app_rdy_ns;    
  always @(posedge clk) app_rdy_r18 <= #TCQ app_rdy_ns;    
  always @(posedge clk) app_rdy_r19 <= #TCQ app_rdy_ns;    
  always @(posedge clk) app_rdy_r20 <= #TCQ app_rdy_ns;    
  always @(posedge clk) app_rdy_r21 <= #TCQ app_rdy_ns;    
  
  
  output wire app_rdy;
  output wire app_rdy_user;
  
  assign app_rdy = app_rdy_r;
  assign app_rdy_user = app_rdy_user_r;

  input [ADDR_WIDTH-1:0] app_addr;
  input [2:0] app_cmd;
  input app_sz;
  input app_hi_pri;
  input app_en;

  reg [ADDR_WIDTH-1:0] app_addr_r1 = {ADDR_WIDTH{1'b0}};
  reg [ADDR_WIDTH-1:0] app_addr_r2 = {ADDR_WIDTH{1'b0}};
  reg [2:0] app_cmd_r1;
  reg [2:0] app_cmd_r2;
  reg app_sz_r1;
  reg app_sz_r2;
  reg app_hi_pri_r1;
  reg app_hi_pri_r2;
  
  //synthesis attribute max_fanout of app_en_r1 is 10;
  //synthesis attribute register_duplication of app_en_r1 is true; 
  //synthesis attribute equivalent_register_removal of app_en_r1 false;
  reg app_en_r1 /* synthesis syn_maxfan = 10 */;
  
  //synthesis attribute max_fanout of app_en_r2 is 10;
  //synthesis attribute register_duplication of app_en_r2 is true; 
  //synthesis attribute equivalent_register_removal of app_en_r2 false;
  reg app_en_r2 /* synthesis syn_maxfan = 10 */;
  

  wire [ADDR_WIDTH-1:0] app_addr_ns1 = app_rdy_r1 && app_en ? app_addr : app_addr_r1;
  wire [ADDR_WIDTH-1:0] app_addr_ns2 = app_rdy_r2 ? app_addr_r1 : app_addr_r2;
  wire [2:0] app_cmd_ns1 = app_rdy_r3 ? app_cmd : app_cmd_r1;
  wire [2:0] app_cmd_ns2 = app_rdy_r4 ? app_cmd_r1 : app_cmd_r2;
  wire app_sz_ns1 = app_rdy_r5 ? app_sz : app_sz_r1;
  wire app_sz_ns2 = app_rdy_r6 ? app_sz_r1 : app_sz_r2;
  wire app_hi_pri_ns1 = app_rdy_r7 ? app_hi_pri : app_hi_pri_r1;
  wire app_hi_pri_ns2 = app_rdy_r8 ? app_hi_pri_r1 : app_hi_pri_r2;
  wire app_en_ns1 = ~rst && (app_rdy_r9 ? app_en : app_en_r1);
  wire app_en_ns2 = ~rst && (app_rdy_r10 ? app_en_r1 : app_en_r2);

  always @(posedge clk) begin
    if (rst) begin
      app_addr_r1 <= #TCQ {ADDR_WIDTH{1'b0}};
      app_addr_r2 <= #TCQ {ADDR_WIDTH{1'b0}};
    end else begin
      app_addr_r1 <= #TCQ app_addr_ns1;
      app_addr_r2 <= #TCQ app_addr_ns2;
    end 
    app_cmd_r1 <= #TCQ app_cmd_ns1;
    app_cmd_r2 <= #TCQ app_cmd_ns2;
    app_sz_r1 <= #TCQ app_sz_ns1;
    app_sz_r2 <= #TCQ app_sz_ns2;
    app_hi_pri_r1 <= #TCQ app_hi_pri_ns1;
    app_hi_pri_r2 <= #TCQ app_hi_pri_ns2;
    app_en_r1 <= #TCQ app_en_ns1;
    app_en_r2 <= #TCQ app_en_ns2;
  end // always @ (posedge clk)

  wire use_addr_lcl = app_en_r2 && app_rdy_r11;
  output wire use_addr;
  assign use_addr = use_addr_lcl;

  output wire [RANK_WIDTH-1:0] rank;
  output wire [BANK_WIDTH-1:0] bank;
  output wire [ROW_WIDTH-1:0] row;
  output wire [COL_WIDTH-1:0] col;
  output wire size;
  output wire [2:0] cmd;
  output wire hi_priority;

  assign col = app_rdy_r12
                 ? app_addr_r1[0+:COL_WIDTH]
                 : app_addr_r2[0+:COL_WIDTH];
  generate
    begin
      if (MEM_ADDR_ORDER == "ROW_BANK_COLUMN")
      begin
        assign row = app_rdy_r13
                       ? app_addr_r1[COL_WIDTH+BANK_WIDTH+:ROW_WIDTH]
                       : app_addr_r2[COL_WIDTH+BANK_WIDTH+:ROW_WIDTH];
        assign bank = app_rdy_r14
                        ? app_addr_r1[COL_WIDTH+:BANK_WIDTH]
                        : app_addr_r2[COL_WIDTH+:BANK_WIDTH];
      end
      else
      begin
        assign row = app_rdy_r15
                       ? app_addr_r1[COL_WIDTH+:ROW_WIDTH]
                       : app_addr_r2[COL_WIDTH+:ROW_WIDTH];
        assign bank = app_rdy_r16
                        ? app_addr_r1[COL_WIDTH+ROW_WIDTH+:BANK_WIDTH]
                        : app_addr_r2[COL_WIDTH+ROW_WIDTH+:BANK_WIDTH];
      end
    end
  endgenerate

  assign rank = (RANKS == 1)
                  ? 1'b0
                  : app_rdy_r17
                    ? app_addr_r1[COL_WIDTH+ROW_WIDTH+BANK_WIDTH+:RANK_WIDTH]
                    : app_addr_r2[COL_WIDTH+ROW_WIDTH+BANK_WIDTH+:RANK_WIDTH];
  assign size = app_rdy_r18
                  ? app_sz_r1
                  : app_sz_r2;
  assign cmd = app_rdy_r19
                 ? app_cmd_r1
                 : app_cmd_r2;
  assign hi_priority = app_rdy_r20
                         ? app_hi_pri_r1
                         : app_hi_pri_r2;

  wire request_accepted = use_addr_lcl && app_rdy_r21;
  wire rd = app_cmd_r2[1:0] == 2'b01;
  wire wr = app_cmd_r2[1:0] == 2'b00;
  wire wr_bytes = app_cmd_r2[1:0] == 2'b11;
  wire write = wr || wr_bytes;
  output wire rd_accepted;
  assign rd_accepted = request_accepted && rd;
  output wire wr_accepted;
  assign wr_accepted = request_accepted && write;

  input [3:0] wr_data_buf_addr;
  input [3:0] rd_data_buf_addr_r;
  output wire [3:0] data_buf_addr;

  assign data_buf_addr = ~write ? rd_data_buf_addr_r : wr_data_buf_addr;

endmodule // ui_cmd

// Local Variables:
// verilog-library-directories:(".")
// End:
