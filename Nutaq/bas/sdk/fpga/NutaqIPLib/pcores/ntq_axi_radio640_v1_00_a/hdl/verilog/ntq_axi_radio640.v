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

module ntq_axi_radio640 (

    idp_Ad9361RxClkPin_p,
    idn_Ad9361RxClkPin_p,
    idp_Ad9361RxFramePin_p,
    idn_Ad9361RxFramePin_p,
    iv6dp_Ad9361RxDataPin_p,
    iv6dn_Ad9361RxDataPin_p,
    odp_Ad9361TxClkPin_p,
    odn_Ad9361TxClkPin_p,
    odp_Ad9361TxFramePin_p,
    odn_Ad9361TxFramePin_p,
    ov6dp_Ad9361TxDataPin_p,
    ov6dn_Ad9361TxDataPin_p,

    idp_Ad9361RefClkPin_p,
    idn_Ad9361RefClkPin_p,
    i_Ad9361ClkOutPin_p,
    i_SyncPin_p,
    i_TriggerPin_p,
    o_Ad9361SyncPin_p,
    o_Ad9361TxnrxPin_p,
    o_Ad9361EnablePin_p,
    o_Ad9361EnAgcPin_p,
    o_Ad9361ResetnPin_p,
    o_OscEnPin_p,
    o_ClkResetPin_p,
    ov4_Ad9361CtrlInPin_p,
    iv8_Ad9361CtrlOutPin_p,
    o_RfSwitch1Pin_p,
    o_RfSwitch2Pin_p,
    ov2_ClkSwitch1Pin_p,
    o_ClkSwitch2Pin_p,

    o_TrxSpiCsnPin_p,
    o_ClkSpiCsnPin_p,
    o_TrxClkSpiClkPin_p,
    o_TrxClkSpiMosiPin_p,
    i_TrxClkSpiMisoPin_p,

    o_OscDacSpiCsnPin_p,
    o_OscDacSpiClkPin_p,
    o_OscDacSpiMosiPin_p,

    o_RfAttSpiLePin_p,
    o_RfAttSpiClkPin_p,
    o_RfAttSpiMosiPin_p,

    o_AdcEnableCh1_p,
    ov16_AdcDataICh1_p,
    ov16_AdcDataQCh1_p,
    o_AdcValidCh1_p,

    o_AdcEnableCh2_p,
    ov16_AdcDataICh2_p,
    ov16_AdcDataQCh2_p,
    o_AdcValidCh2_p,

    o_DacEnableCh1_p,
    o_DacReadEnCh1_p,
    iv16_DacDataICh1_p,
    iv16_DacDataQCh1_p,
    i_DacValidCh1_p,

    o_DacEnableCh2_p,
    o_DacReadEnCh2_p,
    iv16_DacDataICh2_p,
    iv16_DacDataQCh2_p,
    i_DacValidCh2_p,

    i_Ad9361Txnrx_p,
    i_Ad9361Enable_p,
    i_Ad9361EnAgc_p,
    iv4_Ad9361CtrlIn_p,
    ov8_Ad9361CtrlOut_p,
    i_Trx1InRxDir_p,
    i_Trx2InRxDir_p,
    o_Trigger_p,

    iv10_TrxSpiAddr_p,
    iv8_TrxSpiData_p,
    ov8_TrxSpiData_p,
    i_TrxSpiWrnrd_p,
    i_TrxSpiStart_p,
    o_TrxSpiBusy_p,

    iv16_OscDacSpiData_p,
    i_OscDacSpiStart_p,
    o_OscDacSpiBusy_p,

    iv7_RfAttSpiData_p,
    i_RfAtt1SpiStart_p,
    i_RfAtt2SpiStart_p,
    o_RfAttSpiBusy_p,

    i_DelayClk_p,
    i_DelayRst_p,
    o_RefClk_p,
    o_Sync_p,
    o_SerialClk_p,
    o_DesignClk_p,
    i_SerialClkFromMaster_p,
    i_DesignClkFromMaster_p,
    i_RefClkFromMaster_p,
    i_SyncFromMaster_p,

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
    s_axi_rready);

    // parameters

    parameter   C_CARRIER = "unknown";
    parameter   C_FMC_CONNECTOR = 1;
    parameter   C_BOARD_POSITION = 1;
    parameter   C_CLOCK_MASTER = 1;
    parameter   C_DESIGN_CLK_RATE = 1;

    parameter   PCORE_DEVICE_TYPE = 0;
    parameter   PCORE_DAC_DP_DISABLE = 0;
    parameter   PCORE_ADC_DP_DISABLE = 0;
    parameter   C_S_AXI_MIN_SIZE = 32'hffff;
    parameter   C_BASEADDR = 32'hffffffff;
    parameter   C_HIGHADDR = 32'h00000000;
    parameter   COMPLEX_MULT = 0;

    localparam  PCORE_ID = (C_CLOCK_MASTER == 1) ? 0 : 1;

    input           idp_Ad9361RxClkPin_p;
    input           idn_Ad9361RxClkPin_p;
    input           idp_Ad9361RxFramePin_p;
    input           idn_Ad9361RxFramePin_p;
    input   [ 5:0]  iv6dp_Ad9361RxDataPin_p;
    input   [ 5:0]  iv6dn_Ad9361RxDataPin_p;
    output          odp_Ad9361TxClkPin_p;
    output          odn_Ad9361TxClkPin_p;
    output          odp_Ad9361TxFramePin_p;
    output          odn_Ad9361TxFramePin_p;
    output  [ 5:0]  ov6dp_Ad9361TxDataPin_p;
    output  [ 5:0]  ov6dn_Ad9361TxDataPin_p;

    input           idp_Ad9361RefClkPin_p;
    input           idn_Ad9361RefClkPin_p;
    input           i_Ad9361ClkOutPin_p;
    input           i_SyncPin_p;
    input           i_TriggerPin_p;
    output          o_Ad9361SyncPin_p;
    output          o_Ad9361TxnrxPin_p;
    output          o_Ad9361EnablePin_p;
    output          o_Ad9361EnAgcPin_p;
    output          o_Ad9361ResetnPin_p;
    output          o_OscEnPin_p;
    output          o_ClkResetPin_p;

    output  [ 3:0]  ov4_Ad9361CtrlInPin_p;
    input   [ 7:0]  iv8_Ad9361CtrlOutPin_p;
    output          o_RfSwitch1Pin_p;
    output          o_RfSwitch2Pin_p;
    output  [ 1:0]  ov2_ClkSwitch1Pin_p;
    output          o_ClkSwitch2Pin_p;

    output          o_TrxSpiCsnPin_p;
    output          o_ClkSpiCsnPin_p;
    output          o_TrxClkSpiClkPin_p;
    output          o_TrxClkSpiMosiPin_p;
    input           i_TrxClkSpiMisoPin_p;

    output          o_OscDacSpiCsnPin_p;
    output          o_OscDacSpiClkPin_p;
    output          o_OscDacSpiMosiPin_p;

    output          o_RfAttSpiLePin_p;
    output          o_RfAttSpiClkPin_p;
    output          o_RfAttSpiMosiPin_p;

    output          o_AdcEnableCh1_p;
    output  [15:0]  ov16_AdcDataICh1_p;
    output  [15:0]  ov16_AdcDataQCh1_p;
    output          o_AdcValidCh1_p;

    output          o_AdcEnableCh2_p;
    output  [15:0]  ov16_AdcDataICh2_p;
    output  [15:0]  ov16_AdcDataQCh2_p;
    output          o_AdcValidCh2_p;

    output          o_DacEnableCh1_p;
    output          o_DacReadEnCh1_p;
    input   [15:0]  iv16_DacDataICh1_p;
    input   [15:0]  iv16_DacDataQCh1_p;
    input           i_DacValidCh1_p;

    output          o_DacEnableCh2_p;
    output          o_DacReadEnCh2_p;
    input   [15:0]  iv16_DacDataICh2_p;
    input   [15:0]  iv16_DacDataQCh2_p;
    input           i_DacValidCh2_p;

    input           i_Ad9361Txnrx_p;
    input           i_Ad9361Enable_p;
    input           i_Ad9361EnAgc_p;
    input   [ 3:0]  iv4_Ad9361CtrlIn_p;
    output  [ 7:0]  ov8_Ad9361CtrlOut_p;
    input           i_Trx1InRxDir_p;
    input           i_Trx2InRxDir_p;
    output          o_Trigger_p;

    input   [ 9:0]  iv10_TrxSpiAddr_p;
    input   [ 7:0]  iv8_TrxSpiData_p;
    output  [ 7:0]  ov8_TrxSpiData_p;
    input           i_TrxSpiWrnrd_p;
    input           i_TrxSpiStart_p;
    output          o_TrxSpiBusy_p;

    input   [15:0]  iv16_OscDacSpiData_p;
    input           i_OscDacSpiStart_p;
    output          o_OscDacSpiBusy_p;

    input   [ 6:0]  iv7_RfAttSpiData_p;
    input           i_RfAtt1SpiStart_p;
    input           i_RfAtt2SpiStart_p;
    output          o_RfAttSpiBusy_p;

    input           i_DelayClk_p;
    input           i_DelayRst_p;
    output          o_RefClk_p;
    output          o_Sync_p;
    output          o_SerialClk_p;
    output          o_DesignClk_p;
    input           i_SerialClkFromMaster_p;
    input           i_DesignClkFromMaster_p;
    input           i_RefClkFromMaster_p;
    input           i_SyncFromMaster_p;

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

    // Internal registers

    reg           DacDataCh1TwoRdEn_s         = 'd0;
    reg           DacDataCh2TwoRdEn_s         = 'd0;
    reg  [11:0]   v12_ad9361_DacDataICh1_s    = 'd0;
    reg  [11:0]   v12_ad9361_DacDataQCh1_s    = 'd0;
    reg  [11:0]   v12_ad9361_DacDataICh2_s    = 'd0;
    reg  [11:0]   v12_ad9361_DacDataQCh2_s    = 'd0;


    reg           AdcValidCh1_R1_s = 'd0;
    reg           AdcValidCh1_R2_s = 'd0;
    reg           AdcValidCh1_R3_s = 'd0;
    reg           AdcValidCh2_R1_s = 'd0;
    reg           AdcValidCh2_R2_s = 'd0;
    reg           AdcValidCh2_R3_s = 'd0;
    reg           AdcValidCh1_to_DesignClk_s = 'd0;
    reg           AdcValidCh2_to_DesignClk_s = 'd0;

    reg  [11:0]   v12_AdcDataICh1_to_DesignClk_s = 'd0;
    reg  [11:0]   v12_AdcDataQCh1_to_DesignClk_s = 'd0;
    reg           AdcEnableCh1_to_DesignClk_s    = 'd0;
    reg  [11:0]   v12_AdcDataICh2_to_DesignClk_s = 'd0;
    reg  [11:0]   v12_AdcDataQCh2_to_DesignClk_s = 'd0;
    reg           AdcEnableCh2_to_DesignClk_s    = 'd0;
    
    reg  [15:0]   v16_AdcDataICh1_AdjDelay_s [3:0];
    reg  [15:0]   v16_AdcDataQCh1_AdjDelay_s [3:0];
    reg  [3:0]    AdcValidCh1_AdjDelay_s;
    
    reg  [5:0]    AdcValidCh1_DesignClk_R_s  = 'd0;

    
    reg  [15:0]   v16_AdcDataICh2_AdjDelay_s [3:0];
    reg  [15:0]   v16_AdcDataQCh2_AdjDelay_s [3:0];
    reg  [3:0]    AdcValidCh2_AdjDelay_s;
    
    reg  [5:0]    AdcValidCh2_DesignClk_R_s  = 'd0;
    
    reg  [15:0]   v16_DacDataICh1_AdjDelay_s [3:0];
    reg  [15:0]   v16_DacDataQCh1_AdjDelay_s [3:0];
    reg  [3:0]    DacValidCh1_AdjDelay_s;
    
    reg  [15:0]   v16_DacDataICh1_Delayed_s = 'd0;
    reg  [15:0]   v16_DacDataQCh1_Delayed_s = 'd0;
    reg           DacValidCh1_Delayed_s = 'd0;

    reg  [5:0]    DacValidCh1_R_s          = 'd0;
  
    reg  [15:0]   v16_DacDataICh2_AdjDelay_s [3:0];
    reg  [15:0]   v16_DacDataQCh2_AdjDelay_s [3:0];
    reg  [3:0]    DacValidCh2_AdjDelay_s;
    
    reg  [15:0]   v16_DacDataICh2_Delayed_s = 'd0;
    reg  [15:0]   v16_DacDataQCh2_Delayed_s = 'd0;
    reg           DacValidCh2_Delayed_s = 'd0;

    reg  [5:0]    DacValidCh2_R_s          = 'd0;
  
    reg           AdcValidCh1_DesignClk_s = 'd0;
    reg           AdcValidCh2_DesignClk_s = 'd0;  
    reg  [11:0]   AdcDataICh1_DesignClk_s = 'd0;
    reg  [11:0]   AdcDataQCh1_DesignClk_s = 'd0;
    reg  [11:0]   AdcDataICh2_DesignClk_s = 'd0;
    reg  [11:0]   AdcDataQCh2_DesignClk_s = 'd0;
    
    //reg           o_AdcValidCh1_p     = 'd0;
    //reg           o_AdcValidCh2_p     = 'd0;
    //reg  [15:0]   ov16_AdcDataICh1_p  = 'd0;
    //reg  [15:0]   ov16_AdcDataQCh1_p  = 'd0;
    reg           o_AdcEnableCh1_p    = 'd0;
    //reg  [15:0]   ov16_AdcDataICh2_p  = 'd0;
    //reg  [15:0]   ov16_AdcDataQCh2_p  = 'd0;
    reg           o_AdcEnableCh2_p    = 'd0;

    reg           DacReadEnCh1_R1_s = 'd0;
    reg           DacReadEnCh1_R2_s = 'd0;
    reg           DacReadEnCh1_R3_s = 'd0;
    reg           DacReadEnCh2_R1_s = 'd0;
    reg           DacReadEnCh2_R2_s = 'd0;
    reg           DacReadEnCh2_R3_s = 'd0;
    reg           DacReadEnCh1_to_DesignClk_s = 'd0;
    reg           DacReadEnCh2_to_DesignClk_s = 'd0;
    reg           DacEnableCh1_to_DesignClk_s = 'd0;
    reg           DacEnableCh2_to_DesignClk_s = 'd0;

    reg           o_DacReadEnCh1_p = 'd0;
    reg           o_DacReadEnCh2_p = 'd0;
    reg           o_DacEnableCh1_p = 'd0;
    reg           o_DacEnableCh2_p = 'd0;

    reg           DacValidCh1_s      = 'd0;
    reg  [11:0]   v12_DacDataICh1_s  = 'd0;
    reg  [11:0]   v12_DacDataQCh1_s  = 'd0;
    reg           DacValidCh2_s      = 'd0;
    reg  [11:0]   v12_DacDataICh2_s  = 'd0;
    reg  [11:0]   v12_DacDataQCh2_s  = 'd0;

    reg           DacValidCh1_to_SerialClk_s      = 'd0;
    reg  [11:0]   v12_DacDataICh1_to_SerialClk_s  = 'd0;
    reg  [11:0]   v12_DacDataQCh1_to_SerialClk_s  = 'd0;
    reg           DacValidCh2_to_SerialClk_s      = 'd0;
    reg  [11:0]   v12_DacDataICh2_to_SerialClk_s  = 'd0;
    reg  [11:0]   v12_DacDataQCh2_to_SerialClk_s  = 'd0;


    // Internal signals
    wire  [15:0]   v16_AdcDataICh1_Multed_s;
    wire  [15:0]   v16_AdcDataQCh1_Multed_s;
    wire  [15:0]   v16_AdcDataICh2_Multed_s;
    wire  [15:0]   v16_AdcDataQCh2_Multed_s;
    wire           AdcValidCh1_DesignClk_R6_s;
    wire           AdcValidCh2_DesignClk_R6_s;
    wire  [11:0]  v12_DacDataICh1_Multed_s;
    wire  [11:0]  v12_DacDataQCh1_Multed_s; 
    wire          DacValidCh1_Multed_s;  
    wire  [11:0]  v12_DacDataICh2_Multed_s;
    wire  [11:0]  v12_DacDataQCh2_Multed_s;
    wire          DacValidCh2_Multed_s; 
    wire          LocalClock_s;
    wire          DivLocalClk_s;
    wire          SerialClk_s;
    wire          DesignClk_s;

    wire          AdcValidCh1_s;
    wire          AdcValidCh2_s;
    wire  [11:0]  v12_AdcDataICh1_s;
    wire  [11:0]  v12_AdcDataQCh1_s;
    wire          AdcEnableCh1_s;
    wire  [11:0]  v12_AdcDataICh2_s;
    wire  [11:0]  v12_AdcDataQCh2_s;
    wire          AdcEnableCh2_s;
    
    wire  [15:0]  RealCoefficient1_Rx_s;
    wire  [15:0]  ImgCoefficient1_Rx_s;
    wire  [15:0]  RealCoefficient2_Rx_s;
    wire  [15:0]  ImgCoefficient2_Rx_s;
    
    wire  [15:0]  RealCoefficient1_Tx_s;
    wire  [15:0]  ImgCoefficient1_Tx_s;
    wire  [15:0]  RealCoefficient2_Tx_s;
    wire  [15:0]  ImgCoefficient2_Tx_s;
    
    wire  [31:0]  DataDelay1_Rx_s;
    wire  [31:0]  DataDelay2_Rx_s;
    
    wire  [31:0]  DataDelay1_Tx_s;
    wire  [31:0]  DataDelay2_Tx_s;

    wire          DacReadEnCh1_s;
    wire          DacReadEnCh2_s;
    wire          DacEnableCh1_s;
    wire          DacEnableCh2_s;

    // AXI register
    wire          up_sel_s;
    wire          up_wr_s;
    wire  [13:0]  v14_up_addr_s;
    wire  [31:0]  v32_up_wdata_s;
    wire  [ 3:0]  v4_up_wstrb_s;
    wire  [31:0]  v32_up_rdata_s;
    wire          up_ack_s;

    wire          s_axi_areset;
    assign s_axi_areset = ~s_axi_aresetn;


    // Parameter signals
    wire  [ 3:0]  v4_FmcConnector_s;
    wire  [ 3:0]  v4_BoardPosition_s;
    wire  [ 3:0]  v4_DesignClkRate_s;
    wire          ClockMaster_s;

    assign v4_FmcConnector_s    = C_FMC_CONNECTOR;
    assign v4_BoardPosition_s   = C_BOARD_POSITION;
    assign v4_DesignClkRate_s   = C_DESIGN_CLK_RATE;
    assign ClockMaster_s        = C_CLOCK_MASTER;


  // io delay controller for the whole core
  IDELAYCTRL idelayctrl_inst (
    .RST (i_DelayRst_p),
    .REFCLK (i_DelayClk_p),
    .RDY ());

  // If clock master, use the local clock,
  // if not, use the clock from the clock master core
  generate
    if(C_CLOCK_MASTER == 1) begin
        assign SerialClk_s = LocalClock_s;
        if(C_DESIGN_CLK_RATE == 1) begin
            assign DesignClk_s = DivLocalClk_s;
        end else begin
            assign DesignClk_s = LocalClock_s;
        end
    end else begin
        assign SerialClk_s = i_SerialClkFromMaster_p;
        assign DesignClk_s = i_DesignClkFromMaster_p;
    end
  endgenerate

  assign o_SerialClk_p = SerialClk_s;
  assign o_DesignClk_p = DesignClk_s;

  // ADC clock domain crossing
  always @(posedge SerialClk_s) begin
    // Stretch Signal if design clock rate is 1
    AdcValidCh1_R1_s <= AdcValidCh1_s;
    AdcValidCh1_R2_s <= AdcValidCh1_R1_s;
    AdcValidCh1_R3_s <= AdcValidCh1_R2_s;
    AdcValidCh2_R1_s <= AdcValidCh2_s;
    AdcValidCh2_R2_s <= AdcValidCh2_R1_s;
    AdcValidCh2_R3_s <= AdcValidCh2_R2_s;

    if(C_DESIGN_CLK_RATE == 1) begin
        AdcValidCh1_to_DesignClk_s  <= AdcValidCh1_s | AdcValidCh1_R1_s | AdcValidCh1_R2_s | AdcValidCh1_R3_s;
        AdcValidCh2_to_DesignClk_s  <= AdcValidCh2_s | AdcValidCh2_R1_s | AdcValidCh2_R2_s | AdcValidCh2_R3_s;
    end else begin
        AdcValidCh1_to_DesignClk_s  <= AdcValidCh1_s;
        AdcValidCh2_to_DesignClk_s  <= AdcValidCh2_s;
    end

    v12_AdcDataICh1_to_DesignClk_s  <= v12_AdcDataICh1_s;
    v12_AdcDataQCh1_to_DesignClk_s  <= v12_AdcDataQCh1_s;
    AdcEnableCh1_to_DesignClk_s     <= AdcEnableCh1_s;
    v12_AdcDataICh2_to_DesignClk_s  <= v12_AdcDataICh2_s;
    v12_AdcDataQCh2_to_DesignClk_s  <= v12_AdcDataQCh2_s;
    AdcEnableCh2_to_DesignClk_s     <= AdcEnableCh2_s;
  end

  always @(posedge DesignClk_s) begin
    AdcValidCh1_DesignClk_s  <= AdcValidCh1_to_DesignClk_s;
    AdcValidCh2_DesignClk_s  <= AdcValidCh2_to_DesignClk_s;
    AdcDataICh1_DesignClk_s  <= v12_AdcDataICh1_to_DesignClk_s;
    AdcDataQCh1_DesignClk_s  <= v12_AdcDataQCh1_to_DesignClk_s;
    o_AdcEnableCh1_p    <= AdcEnableCh1_to_DesignClk_s;
    AdcDataICh2_DesignClk_s  <= v12_AdcDataICh2_to_DesignClk_s;
    AdcDataQCh2_DesignClk_s  <= v12_AdcDataQCh2_to_DesignClk_s;
    o_AdcEnableCh2_p    <= AdcEnableCh2_to_DesignClk_s;
  end

  // DAC clock domain crossing
  always @(posedge SerialClk_s) begin
    // Stretch Signal if design clock rate is 1
    DacReadEnCh1_R1_s <= DacReadEnCh1_s;
    DacReadEnCh1_R2_s <= DacReadEnCh1_R1_s;
    DacReadEnCh1_R3_s <= DacReadEnCh1_R2_s;
    DacReadEnCh2_R1_s <= DacReadEnCh2_s;
    DacReadEnCh2_R2_s <= DacReadEnCh2_R1_s;
    DacReadEnCh2_R3_s <= DacReadEnCh2_R2_s;

    if(C_DESIGN_CLK_RATE == 1) begin
        DacReadEnCh1_to_DesignClk_s <= DacReadEnCh1_s | DacReadEnCh1_R1_s | DacReadEnCh1_R2_s | DacReadEnCh1_R3_s;
        DacReadEnCh2_to_DesignClk_s <= DacReadEnCh2_s | DacReadEnCh2_R1_s | DacReadEnCh2_R2_s | DacReadEnCh2_R3_s;
    end else begin
        DacReadEnCh1_to_DesignClk_s <= DacReadEnCh1_s;
        DacReadEnCh2_to_DesignClk_s <= DacReadEnCh2_s;
    end
    DacEnableCh1_to_DesignClk_s     <= DacEnableCh1_s;
    DacEnableCh2_to_DesignClk_s     <= DacEnableCh2_s;

    if(C_DESIGN_CLK_RATE == 1) begin
        if (DacReadEnCh1_s == 1'b1) begin
            DacValidCh1_s       <= DacValidCh1_to_SerialClk_s;
            v12_DacDataICh1_s   <= v12_DacDataICh1_to_SerialClk_s;
            v12_DacDataQCh1_s   <= v12_DacDataQCh1_to_SerialClk_s;
        end else begin
            DacValidCh1_s       <= 1'b0;
        end

        if (DacReadEnCh2_s == 1'b1) begin
            DacValidCh2_s       <= DacValidCh2_to_SerialClk_s;
            v12_DacDataICh2_s   <= v12_DacDataICh2_to_SerialClk_s;
            v12_DacDataQCh2_s   <= v12_DacDataQCh2_to_SerialClk_s;
        end else begin
            DacValidCh2_s       <= 1'b0;
        end

    end else begin
        DacValidCh1_s           <= DacValidCh1_to_SerialClk_s;
        v12_DacDataICh1_s       <= v12_DacDataICh1_to_SerialClk_s;
        v12_DacDataQCh1_s       <= v12_DacDataQCh1_to_SerialClk_s;
        DacValidCh2_s           <= DacValidCh2_to_SerialClk_s;
        v12_DacDataICh2_s       <= v12_DacDataICh2_to_SerialClk_s;
        v12_DacDataQCh2_s       <= v12_DacDataQCh2_to_SerialClk_s;
    end
  end

  always @(posedge DesignClk_s) begin
    o_DacReadEnCh1_p                <= DacReadEnCh1_to_DesignClk_s;
    o_DacReadEnCh2_p                <= DacReadEnCh2_to_DesignClk_s;
    o_DacEnableCh1_p                <= DacEnableCh1_to_DesignClk_s;
    o_DacEnableCh2_p                <= DacEnableCh2_to_DesignClk_s;

    DacValidCh1_to_SerialClk_s      <= DacValidCh1_Multed_s;
    v12_DacDataICh1_to_SerialClk_s  <= v12_DacDataICh1_Multed_s;
    v12_DacDataQCh1_to_SerialClk_s  <= v12_DacDataQCh1_Multed_s;
    DacValidCh2_to_SerialClk_s      <= DacValidCh2_Multed_s;
    v12_DacDataICh2_to_SerialClk_s  <= v12_DacDataICh2_Multed_s;
    v12_DacDataQCh2_to_SerialClk_s  <= v12_DacDataQCh2_Multed_s;
  end



  always @(posedge SerialClk_s) begin

    if(DacValidCh1_s==1'b1) begin
        // Latch Data on Data Valid
        v12_ad9361_DacDataICh1_s <= v12_DacDataICh1_s;
        v12_ad9361_DacDataQCh1_s <= v12_DacDataQCh1_s;
        DacDataCh1TwoRdEn_s <= 1'b0;
    end
    else if(DacReadEnCh1_s==1'b1) begin
        // Force value 0 if two data request in a row
        // without any new data
        if(DacDataCh1TwoRdEn_s==1'b1) begin
            v12_ad9361_DacDataICh1_s <= 12'd0;
            v12_ad9361_DacDataQCh1_s <= 12'd0;
        end
        DacDataCh1TwoRdEn_s <= 1'b1;
    end

    if(DacValidCh2_s==1'b1) begin
        // Latch Data on Data Valid
        v12_ad9361_DacDataICh2_s <= v12_DacDataICh2_s;
        v12_ad9361_DacDataQCh2_s <= v12_DacDataQCh2_s;
        DacDataCh2TwoRdEn_s <= 1'b0;
    end
    else if(DacReadEnCh2_s==1'b1) begin
        // Force value 0 if two data request in a row
        // without any new data
        if(DacDataCh2TwoRdEn_s==1'b1) begin
            v12_ad9361_DacDataICh2_s <= 12'd0;
            v12_ad9361_DacDataQCh2_s <= 12'd0;
        end
        DacDataCh2TwoRdEn_s <= 1'b1;
    end

  end


  wire      UpdateInputSyncLatch_s;
  wire      UpdateOutputSyncLatch_s;
  wire      InputSync_s;
  wire      InputSyncLow_s;
  wire      InputSyncHigh_s;
  wire      OutputSyncLow_s;
  wire      OutputSyncHigh_s;

  axi_ad9361 #(
    .C_CLOCK_MASTER(C_CLOCK_MASTER),
    .PCORE_ID(PCORE_ID),
    .PCORE_DEVICE_TYPE(PCORE_DEVICE_TYPE),
    .PCORE_DAC_DP_DISABLE(PCORE_DAC_DP_DISABLE),
    .PCORE_ADC_DP_DISABLE(PCORE_ADC_DP_DISABLE),
    .C_S_AXI_MIN_SIZE(C_S_AXI_MIN_SIZE),
    .C_BASEADDR(C_BASEADDR),
    .C_HIGHADDR(C_HIGHADDR)
  ) i_axi_ad9361 (
    .rx_clk_in_p(idp_Ad9361RxClkPin_p),
    .rx_clk_in_n(idn_Ad9361RxClkPin_p),
    .rx_frame_in_p(idp_Ad9361RxFramePin_p),
    .rx_frame_in_n(idn_Ad9361RxFramePin_p),
    .rx_data_in_p(iv6dp_Ad9361RxDataPin_p),
    .rx_data_in_n(iv6dn_Ad9361RxDataPin_p),
    .tx_clk_out_p(odp_Ad9361TxClkPin_p),
    .tx_clk_out_n(odn_Ad9361TxClkPin_p),
    .tx_frame_out_p(odp_Ad9361TxFramePin_p),
    .tx_frame_out_n(odn_Ad9361TxFramePin_p),
    .tx_data_out_p(ov6dp_Ad9361TxDataPin_p),
    .tx_data_out_n(ov6dn_Ad9361TxDataPin_p),
    .delay_clk(i_DelayClk_p),
    .i_SystemClk_p(s_axi_aclk),
    .l_clk(LocalClock_s),
    .o_DivClk_p(DivLocalClk_s),
    .clk(SerialClk_s),
    .adc_enable_i0(AdcEnableCh1_s),
    .adc_valid_i0(AdcValidCh1_s),
    .adc_data_i0(v12_AdcDataICh1_s),
    .adc_enable_q0(),
    .adc_valid_q0(),
    .adc_data_q0(v12_AdcDataQCh1_s),
    .adc_enable_i1(AdcEnableCh2_s),
    .adc_valid_i1(AdcValidCh2_s),
    .adc_data_i1(v12_AdcDataICh2_s),
    .adc_enable_q1(),
    .adc_valid_q1(),
    .adc_data_q1(v12_AdcDataQCh2_s),
    .adc_dovf(1'b0),
    .adc_dunf(1'b0),
    .dac_enable_i0(DacEnableCh1_s),
    .dac_valid_i0(DacReadEnCh1_s),
    .dac_data_i0(v12_ad9361_DacDataICh1_s),
    .dac_enable_q0(),
    .dac_valid_q0(),
    .dac_data_q0(v12_ad9361_DacDataQCh1_s),
    .dac_enable_i1(DacEnableCh2_s),
    .dac_valid_i1(DacReadEnCh2_s),
    .dac_data_i1(v12_ad9361_DacDataICh2_s),
    .dac_enable_q1(),
    .dac_valid_q1(),
    .dac_data_q1(v12_ad9361_DacDataQCh2_s),
    .dac_dovf(1'b0),
    .dac_dunf(1'b0),
    .s_axi_aclk(s_axi_aclk),
    .s_axi_aresetn(s_axi_aresetn),
    .s_axi_awvalid(s_axi_awvalid),
    .s_axi_awaddr(s_axi_awaddr),
    .s_axi_awready(s_axi_awready),
    .s_axi_wvalid(s_axi_wvalid),
    .s_axi_wdata(s_axi_wdata),
    .s_axi_wstrb(s_axi_wstrb),
    .s_axi_wready(s_axi_wready),
    .s_axi_bvalid(s_axi_bvalid),
    .s_axi_bresp(s_axi_bresp),
    .s_axi_bready(s_axi_bready),
    .s_axi_arvalid(s_axi_arvalid),
    .s_axi_araddr(s_axi_araddr),
    .s_axi_arready(s_axi_arready),
    .s_axi_rvalid(s_axi_rvalid),
    .s_axi_rdata(s_axi_rdata),
    .s_axi_rresp(s_axi_rresp),
    .s_axi_rready(s_axi_rready),
    .idp_RefClk_p(idp_Ad9361RefClkPin_p),
    .idn_RefClk_p(idn_Ad9361RefClkPin_p),
    .i_SyncPin_p            (i_SyncPin_p),
    .o_Sync_p               (o_Ad9361SyncPin_p),
    .o_RefClkToSlave_p      (o_RefClk_p),
    .o_SyncToSlave_p        (o_Sync_p),
    .i_RefClkFromMaster_p   (i_RefClkFromMaster_p),
    .i_SyncFromMaster_p     (i_SyncFromMaster_p),
    .o_RealCoefficient0_Rx_p   (RealCoefficient1_Rx_s),
    .o_ImgCoefficient0_Rx_p    (ImgCoefficient1_Rx_s),
    .o_RealCoefficient1_Rx_p   (RealCoefficient2_Rx_s),
    .o_ImgCoefficient1_Rx_p    (ImgCoefficient2_Rx_s),
    .o_DataDelay0_Rx_p         (DataDelay1_Rx_s),
    .o_DataDelay1_Rx_p         (DataDelay2_Rx_s),
    .o_RealCoefficient0_Tx_p   (RealCoefficient1_Tx_s),
    .o_ImgCoefficient0_Tx_p    (ImgCoefficient1_Tx_s),
    .o_RealCoefficient1_Tx_p   (RealCoefficient2_Tx_s),
    .o_ImgCoefficient1_Tx_p    (ImgCoefficient2_Tx_s),
    .o_DataDelay0_Tx_p         (DataDelay1_Tx_s),
    .o_DataDelay1_Tx_p         (DataDelay2_Tx_s),
    .o_up_sel_p(up_sel_s),
    .o_up_wr_p(up_wr_s),
    .ov14_up_addr_p(v14_up_addr_s),
    .ov32_up_wdata_p(v32_up_wdata_s),
    .ov4_up_wstrb_p(v4_up_wstrb_s),
    .iv32_up_rdata_p(v32_up_rdata_s),
    .i_up_ack_p(up_ack_s),

    .i_UpdateInputSyncLatch_p    (UpdateInputSyncLatch_s),
    .i_UpdateOutputSyncLatch_p   (UpdateOutputSyncLatch_s),
    .o_InputSync_p               (InputSync_s),
    .o_InputSyncLow_p            (InputSyncLow_s),
    .o_InputSyncHigh_p           (InputSyncHigh_s),
    .o_OutputSyncLow_p           (OutputSyncLow_s),
    .o_OutputSyncHigh_p          (OutputSyncHigh_s),

    .dev_dbg_data(),
    .dev_l_dbg_data(),
    .up_adc_gpio_in (up_adc_gpio_in),
    .up_adc_gpio_out (up_adc_gpio_out),
    .up_dac_gpio_in (up_dac_gpio_in),
    .up_dac_gpio_out (up_dac_gpio_out));


    // TRX/CLK SPI control
    wire  [ 7:0]  v8_TrxSpiClkDownsampling_s;
    wire          TrxSpiClkPol_s;
    wire          TrxClkSpiCtrl_s;
    wire  [23:0]  v24_TrxClkSpiDataTx_s;
    wire  [23:0]  v24_TrxClkSpiDataRx_s;
    wire          TrxSpiStart_s;
    wire          ClkSpiStart_s;
    wire          TrxClkSpiBusy_s;

    trxclk_spi i_trxclk_spi (
        .i_Rst_p                    (s_axi_areset),
        .i_SystemClk_p              (s_axi_aclk),

        .iv8_spiClkDownsampling_p   (v8_TrxSpiClkDownsampling_s),
        .i_regTrxSpiClkPol_p        (TrxSpiClkPol_s),
        .i_regTrxClkSpiCtrl_p       (TrxClkSpiCtrl_s),
        .iv24_regTrxClkSpiData_p    (v24_TrxClkSpiDataTx_s),
        .ov24_regTrxClkSpiData_p    (v24_TrxClkSpiDataRx_s),
        .i_regTrxSpiStart_p         (TrxSpiStart_s),
        .i_regClkSpiStart_p         (ClkSpiStart_s),
        .o_regTrxClkSpiBusy_p       (TrxClkSpiBusy_s),

        .i_DesignClk_p              (DesignClk_s),

        .iv10_ulTrxSpiAddr_p        (iv10_TrxSpiAddr_p),
        .iv8_ulTrxSpiData_p         (iv8_TrxSpiData_p),
        .ov8_ulTrxSpiData_p         (ov8_TrxSpiData_p),
        .i_ulTrxSpiWrnrd_p          (i_TrxSpiWrnrd_p),
        .i_ulTrxSpiStart_p          (i_TrxSpiStart_p),
        .o_ulTrxSpiBusy_p           (o_TrxSpiBusy_p),
        .o_ulTrxSpiDone_p           (),

        .o_TrxSpiCsnPin_p           (o_TrxSpiCsnPin_p),
        .o_ClkSpiCsnPin_p           (o_ClkSpiCsnPin_p),
        .o_TrxClkSpiClkPin_p        (o_TrxClkSpiClkPin_p),
        .o_TrxClkSpiMosiPin_p       (o_TrxClkSpiMosiPin_p),
        .i_TrxClkSpiMisoPin_p       (i_TrxClkSpiMisoPin_p));

    // DAC OSC SPI control
    wire          DacOscSpiCtrl_s;
    wire  [15:0]  v16_DacOscSpiDataTx_s;
    wire          DacOscSpiStart_s;
    wire          DacOscSpiBusy_s;

    dacosc_spi i_dacosc_spi (
        .i_Rst_p                    (s_axi_areset),
        .i_SystemClk_p              (s_axi_aclk),

        .i_regDacOscSpiCtrl_p       (DacOscSpiCtrl_s),
        .iv16_regDacOscClkSpiData_p (v16_DacOscSpiDataTx_s),
        .i_regDacOscSpiStart_p      (DacOscSpiStart_s),
        .o_regDacOscClkSpiBusy_p    (DacOscSpiBusy_s),

        .i_DesignClk_p              (DesignClk_s),

        .iv16_ulDacOscSpiData_p     (iv16_OscDacSpiData_p),
        .i_ulDacOscSpiStart_p       (i_OscDacSpiStart_p),
        .o_ulDacOscSpiBusy_p        (o_OscDacSpiBusy_p),
        .o_ulDacOscSpiDone_p        (),

        .o_OscDacSpiCsnPin_p        (o_OscDacSpiCsnPin_p),
        .o_OscDacSpiClkPin_p        (o_OscDacSpiClkPin_p),
        .o_OscDacSpiMosiPin_p       (o_OscDacSpiMosiPin_p));

    // DAC OSC SPI control
    wire          RfAttSpiCtrl_s;
    wire  [15:0]  v16_RfAttSpiDataTx_s;
    wire          RfAttSpiStart_s;
    wire          RfAttSpiBusy_s;

    rfatt_spi i_rfatt_spi (
        .i_Rst_p                    (s_axi_areset),
        .i_SystemClk_p              (s_axi_aclk),

        .i_regRfAttSpiCtrl_p        (RfAttSpiCtrl_s),
        .iv16_regRfAttClkSpiData_p  (v16_RfAttSpiDataTx_s),
        .i_regRfAttSpiStart_p       (RfAttSpiStart_s),
        .o_regRfAttClkSpiBusy_p     (RfAttSpiBusy_s),

        .i_DesignClk_p              (DesignClk_s),

        .iv7_ulRfAttSpiData_p       (iv7_RfAttSpiData_p),
        .i_ulRfAtt1SpiStart_p       (i_RfAtt1SpiStart_p),
        .i_ulRfAtt2SpiStart_p       (i_RfAtt2SpiStart_p),
        .o_ulRfAttSpiBusy_p         (o_RfAttSpiBusy_p),
        .o_ulRfAttSpiDone_p         (),

        .o_RfAttSpiLePin_p          (o_RfAttSpiLePin_p),
        .o_RfAttSpiClkPin_p         (o_RfAttSpiClkPin_p),
        .o_RfAttSpiMosiPin_p        (o_RfAttSpiMosiPin_p));


    wire        TxnrxForce_s;
    wire        TxnrxValue_s;
    wire        EnableForce_s;
    wire        EnableValue_s;
    wire        EnAgcForce_s;
    wire        EnAgcValue_s;
    wire [ 3:0] v4_ForceCtrlIn_s;
    wire [ 3:0] v4_ValueCtrlIn_s;
    wire        UpdateCtrlOutLatch_s;
    wire [ 7:0] v8_CtrlOutCurrent_s;
    wire [ 7:0] v8_CtrlOutLow_s;
    wire [ 7:0] v8_CtrlOutHigh_s;
    wire [ 1:0] v2_ClkSwitch1_s;
    wire        ClkSwitch2_s;
    wire        Trx1InRxDirValue_s;
    wire        Trx1InRxDirForce_s;
    wire        Trx2InRxDirValue_s;
    wire        Trx2InRxDirForce_s;
    wire        TriggerBitSlip_s;
    wire [ 4:0] v5_TriggerDelay_s;
    wire        UpdateTriggerLatch_s;
    wire        TriggerLow_s;
    wire        TriggerHigh_s;

    radio640_io #(
        .PCORE_DEVICE_TYPE          (PCORE_DEVICE_TYPE)
    ) i_radio640_io(
        .i_Rst_p                    (s_axi_areset),
        .i_DelayClk_p               (i_DelayClk_p),

        // Pin
        .o_Ad9361TxnrxPin_p         (o_Ad9361TxnrxPin_p),
        .o_Ad9361EnablePin_p        (o_Ad9361EnablePin_p),
        .o_Ad9361EnAgcPin_p         (o_Ad9361EnAgcPin_p),
        .o_Ad9361ResetnPin_p        (o_Ad9361ResetnPin_p),
        .ov4_Ad9361CtrlInPin_p      (ov4_Ad9361CtrlInPin_p),
        .iv8_Ad9361CtrlOutPin_p     (iv8_Ad9361CtrlOutPin_p),
        .o_RfSwitch1Pin_p           (o_RfSwitch1Pin_p),
        .o_RfSwitch2Pin_p           (o_RfSwitch2Pin_p),
        .ov2_ClkSwitch1Pin_p        (ov2_ClkSwitch1Pin_p),
        .o_ClkSwitch2Pin_p          (o_ClkSwitch2Pin_p),
        .i_TriggerPin_p             (i_TriggerPin_p),

        // User Logic
        .i_DesignClk_p              (DesignClk_s),
        .i_Ad9361Txnrx_p            (i_Ad9361Txnrx_p),
        .i_Ad9361Enable_p           (i_Ad9361Enable_p),
        .i_Ad9361EnAgc_p            (i_Ad9361EnAgc_p),
        .iv4_Ad9361CtrlIn_p         (iv4_Ad9361CtrlIn_p),
        .ov8_Ad9361CtrlOut_p        (ov8_Ad9361CtrlOut_p),
        .i_Trx1InRxDir_p            (i_Trx1InRxDir_p),
        .i_Trx2InRxDir_p            (i_Trx2InRxDir_p),
        .o_Trigger_p                (o_Trigger_p),

        // Processor
        .i_SystemClk_p              (s_axi_aclk),
        .i_TxnrxForce_p             (TxnrxForce_s),
        .i_TxnrxValue_p             (TxnrxValue_s),
        .i_EnableForce_p            (EnableForce_s),
        .i_EnableValue_p            (EnableValue_s),
        .i_EnAgcForce_p             (EnAgcForce_s),
        .i_EnAgcValue_p             (EnAgcValue_s),
        .iv4_ForceCtrlIn_p          (v4_ForceCtrlIn_s),
        .iv4_ValueCtrlIn_p          (v4_ValueCtrlIn_s),
        .i_UpdateCtrlOutLatch_p     (UpdateCtrlOutLatch_s),
        .ov8_CtrlOutCurrent_p       (v8_CtrlOutCurrent_s),
        .ov8_CtrlOutLow_p           (v8_CtrlOutLow_s),
        .ov8_CtrlOutHigh_p          (v8_CtrlOutHigh_s),
        .iv2_ClkSwitch1_p           (v2_ClkSwitch1_s),
        .i_ClkSwitch2_p             (ClkSwitch2_s),
        .i_Trx1InRxDirValue_p       (Trx1InRxDirValue_s),
        .i_Trx1InRxDirForce_p       (Trx1InRxDirForce_s),
        .i_Trx2InRxDirValue_p       (Trx2InRxDirValue_s),
        .i_Trx2InRxDirForce_p       (Trx2InRxDirForce_s),
        .i_TriggerBitSlip           (TriggerBitSlip_s),
        .iv5_TriggerDelay_p         (v5_TriggerDelay_s),
        .i_UpdateTriggerLatch_p     (UpdateTriggerLatch_s),
        .o_TriggerLow_p             (TriggerLow_s),
        .o_TriggerHigh_p            (TriggerHigh_s)
        );

    wire [31:0] v32_TestClkSel_s;
    wire [31:0] v32_TestClkFreq_s;

    frequency_counter_mux #(
        .C_REFCLK_FREQ_HZ           (100000000),
        .C_NUM_OF_TEST_CLK          (4)
    ) i_frequency_counter_mux (
        .i_Rst_p                    (s_axi_areset),
        .i_RefClk_p                 (s_axi_aclk),
        .iv_TestClks_p              ({i_Ad9361ClkOutPin_p, o_RefClk_p, DesignClk_s, SerialClk_s}),
        .iv32_TestClkSel_p          (v32_TestClkSel_s),
        .ov32_Freq_p                (v32_TestClkFreq_s)
    );

    radio640_regs i_radio640_regs (
        .i_Rst_p(s_axi_areset),
        .i_Clk_p(s_axi_aclk),
        .i_up_sel_p(up_sel_s),
        .i_up_wr_p(up_wr_s),
        .iv14_up_addr_p(v14_up_addr_s),
        .iv32_up_wdata_p(v32_up_wdata_s),
        .iv4_up_wstrb_p(v4_up_wstrb_s),
        .ov32_up_rdata_p(v32_up_rdata_s),
        .o_up_ack_p(up_ack_s),

        .ov8_TrxSpiClkDownsampling_p    (v8_TrxSpiClkDownsampling_s),
        .o_TrxSpiClkPol_p               (TrxSpiClkPol_s),
        .o_TrxClkSpiCtrl_p              (TrxClkSpiCtrl_s),
        .ov24_TrxClkSpiData_p           (v24_TrxClkSpiDataTx_s),
        .iv24_TrxClkSpiData_p           (v24_TrxClkSpiDataRx_s),
        .o_TrxSpiStart_p                (TrxSpiStart_s),
        .o_ClkSpiStart_p                (ClkSpiStart_s),
        .i_TrxClkSpiBusy_p              (TrxClkSpiBusy_s),

        .o_DacOscSpiCtrl_p       (DacOscSpiCtrl_s),
        .ov16_DacOscClkSpiData_p (v16_DacOscSpiDataTx_s),
        .o_DacOscSpiStart_p      (DacOscSpiStart_s),
        .i_DacOscClkSpiBusy_p    (DacOscSpiBusy_s),

        .o_RfAttSpiCtrl_p        (RfAttSpiCtrl_s),
        .ov16_RfAttClkSpiData_p  (v16_RfAttSpiDataTx_s),
        .o_RfAttSpiStart_p       (RfAttSpiStart_s),
        .i_RfAttClkSpiBusy_p     (RfAttSpiBusy_s),

        .iv4_FmcConnector_p      (v4_FmcConnector_s),
        .iv4_BoardPosition_p     (v4_BoardPosition_s),
        .iv4_DesignClkRate_p     (v4_DesignClkRate_s),
        .i_ClockMaster_p         (ClockMaster_s),

        .o_TxnrxForce_p          (TxnrxForce_s),
        .o_TxnrxValue_p          (TxnrxValue_s),
        .o_EnableForce_p         (EnableForce_s),
        .o_EnableValue_p         (EnableValue_s),
        .o_EnAgcForce_p          (EnAgcForce_s),
        .o_EnAgcValue_p          (EnAgcValue_s),
        .ov4_ForceCtrlIn_p       (v4_ForceCtrlIn_s),
        .ov4_ValueCtrlIn_p       (v4_ValueCtrlIn_s),
        .o_UpdateCtrlOutLatch_p  (UpdateCtrlOutLatch_s),
        .iv8_CtrlOutCurrent_p    (v8_CtrlOutCurrent_s),
        .iv8_CtrlOutLow_p        (v8_CtrlOutLow_s),
        .iv8_CtrlOutHigh_p       (v8_CtrlOutHigh_s),
        .ov2_ClkSwitch1_p        (v2_ClkSwitch1_s),
        .o_ClkSwitch2_p          (ClkSwitch2_s),
        .o_Trx1InRxDirValue_p    (Trx1InRxDirValue_s),
        .o_Trx1InRxDirForce_p    (Trx1InRxDirForce_s),
        .o_Trx2InRxDirValue_p    (Trx2InRxDirValue_s),
        .o_Trx2InRxDirForce_p    (Trx2InRxDirForce_s),
        .o_Ad9361ResetnPin_p     (o_Ad9361ResetnPin_p),
        .o_OscEnPin_p            (o_OscEnPin_p),
        .o_ClkResetPin_p         (o_ClkResetPin_p),
        .o_TriggerBitSlip_p      (TriggerBitSlip_s),
        .ov5_TriggerDelay_p      (v5_TriggerDelay_s),
        
        .ov32_TestClkSel_p       (v32_TestClkSel_s),
        .iv32_TestClkFreq_p      (v32_TestClkFreq_s),
        
        .i_Trigger_p                (o_Trigger_p),
        .o_UpdateTriggerLatch_p     (UpdateTriggerLatch_s),
        .i_TriggerLow_p             (TriggerLow_s),
        .i_TriggerHigh_p            (TriggerHigh_s),
        .o_UpdateInputSyncLatch_p   (UpdateInputSyncLatch_s),
        .o_UpdateOutputSyncLatch_p  (UpdateOutputSyncLatch_s),
        .i_InputSync_p              (InputSync_s),
        .i_InputSyncLow_p           (InputSyncLow_s),
        .i_InputSyncHigh_p          (InputSyncHigh_s),
        .i_OutputSyncLow_p          (OutputSyncLow_s),
        .i_OutputSyncHigh_p         (OutputSyncHigh_s)
        );
        
    generate 
    if (COMPLEX_MULT == 1) begin
      //Channel 1
      //Complex multiplier
      ComplexMult_RX_Calibre M1
      (
        .ar(AdcDataICh1_DesignClk_s),
        .ai(AdcDataQCh1_DesignClk_s),
        .br(RealCoefficient1_Rx_s),
        .bi(ImgCoefficient1_Rx_s),
        .clk(DesignClk_s),
        .round_cy(AdcDataICh1_DesignClk_s[0]),
        .pr(v16_AdcDataICh1_Multed_s),
        .pi(v16_AdcDataQCh1_Multed_s)
      );
      
      // Complex multiplier has a 6 clock cycles delay
      // Delay the AdcValid signal
      integer j;
      always @(posedge DesignClk_s) begin
        AdcValidCh1_DesignClk_R_s[0] <= AdcValidCh1_DesignClk_s;
        for (j = 0; j < 5; j = j + 1) begin
            AdcValidCh1_DesignClk_R_s[j+1] <= AdcValidCh1_DesignClk_R_s[j];
        end
      end
      assign AdcValidCh1_DesignClk_R6_s = AdcValidCh1_DesignClk_R_s[5];    
      
      // Additional adjustable delays for data and AdcValid (4 taps)
      always @(posedge DesignClk_s) begin
        v16_AdcDataICh1_AdjDelay_s[0] <= v16_AdcDataICh1_Multed_s;
        v16_AdcDataQCh1_AdjDelay_s[0] <= v16_AdcDataQCh1_Multed_s;
        AdcValidCh1_AdjDelay_s[0]    <= AdcValidCh1_DesignClk_R6_s;
        for (j = 0; j < 3; j = j + 1) begin
            v16_AdcDataICh1_AdjDelay_s[j+1] <= v16_AdcDataICh1_AdjDelay_s[j];
            v16_AdcDataQCh1_AdjDelay_s[j+1] <= v16_AdcDataQCh1_AdjDelay_s[j];
            AdcValidCh1_AdjDelay_s[j+1] <= AdcValidCh1_AdjDelay_s[j];
        end
      end
      
      assign ov16_AdcDataICh1_p = v16_AdcDataICh1_AdjDelay_s[DataDelay1_Rx_s[1:0]];
      assign ov16_AdcDataQCh1_p = v16_AdcDataQCh1_AdjDelay_s[DataDelay1_Rx_s[1:0]];
      assign o_AdcValidCh1_p = AdcValidCh1_AdjDelay_s[DataDelay1_Rx_s[1:0]];
      

      //Channel 2
      ComplexMult_RX_Calibre M2
      (
        .ar(AdcDataICh2_DesignClk_s),
        .ai(AdcDataQCh2_DesignClk_s),
        .br(RealCoefficient2_Rx_s),
        .bi(ImgCoefficient2_Rx_s),
        .clk(DesignClk_s),
        .round_cy(AdcDataICh2_DesignClk_s[0]),
        .pr(v16_AdcDataICh2_Multed_s),
        .pi(v16_AdcDataQCh2_Multed_s)
      );
      
      // Complex multiplier has a 6 clock cycles delay
      // Delay the AdcValid signal
      always @(posedge DesignClk_s) begin
        AdcValidCh2_DesignClk_R_s[0] <= AdcValidCh2_DesignClk_s;
        for (j = 0; j < 5; j = j + 1) begin
            AdcValidCh2_DesignClk_R_s[j+1] <= AdcValidCh2_DesignClk_R_s[j];
        end
      end
      assign AdcValidCh2_DesignClk_R6_s = AdcValidCh2_DesignClk_R_s[5];    
      
      // Additional adjustable delays for data and AdcValid
      always @(posedge DesignClk_s) begin
        v16_AdcDataICh2_AdjDelay_s[0] <= v16_AdcDataICh2_Multed_s;
        v16_AdcDataQCh2_AdjDelay_s[0] <= v16_AdcDataQCh2_Multed_s;
        AdcValidCh2_AdjDelay_s[0]    <= AdcValidCh2_DesignClk_R6_s;
        for (j = 0; j < 3; j = j + 1) begin
            v16_AdcDataICh2_AdjDelay_s[j+1] <= v16_AdcDataICh2_AdjDelay_s[j];
            v16_AdcDataQCh2_AdjDelay_s[j+1] <= v16_AdcDataQCh2_AdjDelay_s[j];
            AdcValidCh2_AdjDelay_s[j+1] <= AdcValidCh2_AdjDelay_s[j];
        end
      end
      
      assign ov16_AdcDataICh2_p = v16_AdcDataICh2_AdjDelay_s[DataDelay2_Rx_s[1:0]];
      assign ov16_AdcDataQCh2_p = v16_AdcDataQCh2_AdjDelay_s[DataDelay2_Rx_s[1:0]];
      assign o_AdcValidCh2_p = AdcValidCh2_AdjDelay_s[DataDelay2_Rx_s[1:0]];

      //Dac
      //Channel 1
      
      // Additional adjustable delays for data and AdcValid (4 taps)
      always @(posedge DesignClk_s) begin
        v16_DacDataICh1_AdjDelay_s[0] <= iv16_DacDataICh1_p;
        v16_DacDataQCh1_AdjDelay_s[0] <= iv16_DacDataQCh1_p;
        DacValidCh1_AdjDelay_s[0]    <= i_DacValidCh1_p;
        for (j = 0; j < 3; j = j + 1) begin
            v16_DacDataICh1_AdjDelay_s[j+1] <= v16_DacDataICh1_AdjDelay_s[j];
            v16_DacDataQCh1_AdjDelay_s[j+1] <= v16_DacDataQCh1_AdjDelay_s[j];
            DacValidCh1_AdjDelay_s[j+1] <= DacValidCh1_AdjDelay_s[j];
        end
        
        v16_DacDataICh1_Delayed_s <= v16_DacDataICh1_AdjDelay_s[DataDelay1_Tx_s[1:0]];
        v16_DacDataQCh1_Delayed_s <= v16_DacDataQCh1_AdjDelay_s[DataDelay1_Tx_s[1:0]];
        DacValidCh1_Delayed_s <= DacValidCh1_AdjDelay_s[DataDelay1_Tx_s[1:0]];
      end
      
      
      ComplexMult_TX_Calibre M3
      (
        .ar(v16_DacDataICh1_Delayed_s),
        .ai(v16_DacDataQCh1_Delayed_s),
        .br(RealCoefficient1_Tx_s),
        .bi(ImgCoefficient1_Tx_s),
        .clk(DesignClk_s),
        .round_cy(v16_DacDataICh1_Delayed_s[0]),
        .pr(v12_DacDataICh1_Multed_s),
        .pi(v12_DacDataQCh1_Multed_s)
      );
      
      // Complex multiplier has a 6 clock cycles delay
      // Delay the DacValid signal
      always @(posedge DesignClk_s) begin
        DacValidCh1_R_s[0] <= DacValidCh1_Delayed_s;
        for (j = 0; j < 5; j = j + 1) begin
            DacValidCh1_R_s[j+1] <= DacValidCh1_R_s[j];
        end
      end
      assign DacValidCh1_Multed_s = DacValidCh1_R_s[5];
      
      //Channel 2
      // Additional adjustable delays for data and AdcValid (4 taps)
      always @(posedge DesignClk_s) begin
        v16_DacDataICh2_AdjDelay_s[0] <= iv16_DacDataICh2_p;
        v16_DacDataQCh2_AdjDelay_s[0] <= iv16_DacDataQCh2_p;
        DacValidCh2_AdjDelay_s[0]    <= i_DacValidCh2_p;
        for (j = 0; j < 3; j = j + 1) begin
            v16_DacDataICh2_AdjDelay_s[j+1] <= v16_DacDataICh2_AdjDelay_s[j];
            v16_DacDataQCh2_AdjDelay_s[j+1] <= v16_DacDataQCh2_AdjDelay_s[j];
            DacValidCh2_AdjDelay_s[j+1] <= DacValidCh2_AdjDelay_s[j];
        end
        
        v16_DacDataICh2_Delayed_s <= v16_DacDataICh2_AdjDelay_s[DataDelay2_Tx_s[1:0]];
        v16_DacDataQCh2_Delayed_s <= v16_DacDataQCh2_AdjDelay_s[DataDelay2_Tx_s[1:0]];
        DacValidCh2_Delayed_s <= DacValidCh2_AdjDelay_s[DataDelay2_Tx_s[1:0]];
      end
      
      
      ComplexMult_TX_Calibre M4
      (
        .ar(v16_DacDataICh2_Delayed_s),
        .ai(v16_DacDataQCh2_Delayed_s),
        .br(RealCoefficient2_Tx_s),
        .bi(ImgCoefficient2_Tx_s),
        .clk(DesignClk_s),
        .round_cy(v16_DacDataICh2_Delayed_s[0]),
        .pr(v12_DacDataICh2_Multed_s),
        .pi(v12_DacDataQCh2_Multed_s)
      );
      
      // Complex multiplier has a 6 clock cycles delay
      // Delay the DacValid signal
      always @(posedge DesignClk_s) begin
        DacValidCh2_R_s[0] <= DacValidCh2_Delayed_s;
        for (j = 0; j < 5; j = j + 1) begin
            DacValidCh2_R_s[j+1] <= DacValidCh2_R_s[j];
        end
      end   
      assign DacValidCh2_Multed_s = DacValidCh2_R_s[5];    
      
    end else begin
      //No delay chain and no multiplier. Simply bypass them
    
      //Adc
      //Channel 1
      assign ov16_AdcDataICh1_p = {AdcDataICh1_DesignClk_s, 4'b0};
      assign ov16_AdcDataQCh1_p = {AdcDataQCh1_DesignClk_s, 4'b0};
      assign o_AdcValidCh1_p = AdcValidCh1_DesignClk_s;
      
      //Channel 2
      assign ov16_AdcDataICh2_p = {AdcDataICh2_DesignClk_s, 4'b0};
      assign ov16_AdcDataQCh2_p = {AdcDataQCh2_DesignClk_s, 4'b0};
      assign o_AdcValidCh2_p = AdcValidCh2_DesignClk_s;      
        
      //Dac
      //Channel 1
      assign v12_DacDataICh1_Multed_s = iv16_DacDataICh1_p[15:4];
      assign v12_DacDataQCh1_Multed_s = iv16_DacDataQCh1_p[15:4];
      
      //Channel 2
      assign v12_DacDataICh2_Multed_s = iv16_DacDataICh2_p[15:4];
      assign v12_DacDataQCh2_Multed_s = iv16_DacDataQCh2_p[15:4];
      
      assign DacValidCh1_Multed_s = i_DacValidCh1_p;
      assign DacValidCh2_Multed_s = i_DacValidCh2_p;


    end
    endgenerate

endmodule

// ***************************************************************************
// ***************************************************************************
