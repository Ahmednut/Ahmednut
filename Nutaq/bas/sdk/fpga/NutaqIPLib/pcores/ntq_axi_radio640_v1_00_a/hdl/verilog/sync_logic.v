
`timescale 1ns/100ps

module sync_logic (

  // external interface
  idp_RefClk_p,
  idn_RefClk_p,
  i_SyncPin_p,
  o_Sync_p,
  
  // Master/Slave interface
  o_RefClkToSlave_p,
  o_SyncToSlave_p,
  i_RefClkFromMaster_p,
  i_SyncFromMaster_p,

  // register interface
  i_DelayClk_p,
  i_SystemClk_p,
  
  i_SoftSync_p,
  iv5_RefClkIODelay_p,
  iv5_InputSyncDelay_p,
  iv5_OutputSyncDelay_p,
  iv4_SyncSel_p,
  i_SyncBitSlip_p,
  
  i_UpdateInputSyncLatch_p,
  i_UpdateOutputSyncLatch_p,
  o_InputSync_p,
  o_InputSyncLow_p,
  o_InputSyncHigh_p,
  o_OutputSyncLow_p,
  o_OutputSyncHigh_p);

  // parameters
  parameter   C_CLOCK_MASTER = 1;
  parameter   PCORE_DEVICE_TYPE = 0;

  localparam  SERIES7 = 0;
  localparam  VIRTEX6 = 1;
  // external interface

  input               idp_RefClk_p;
  input               idn_RefClk_p;
  input               i_SyncPin_p;
  output              o_Sync_p;

  // Master/Slave interface

  input               i_RefClkFromMaster_p;
  input               i_SyncFromMaster_p;
  output              o_RefClkToSlave_p;
  output              o_SyncToSlave_p;
  
  // register interface
  
  input               i_DelayClk_p;
  input               i_SystemClk_p;
  input               i_SoftSync_p;
  input               i_SyncBitSlip_p;
  input      [ 3:0]   iv4_SyncSel_p;
  input      [ 4:0]   iv5_RefClkIODelay_p;
  input      [ 4:0]   iv5_OutputSyncDelay_p;
  input      [ 4:0]   iv5_InputSyncDelay_p;
  
  input               i_UpdateInputSyncLatch_p;
  input               i_UpdateOutputSyncLatch_p;
  output              o_InputSync_p;
  output              o_InputSyncLow_p;
  output              o_InputSyncHigh_p;
  output              o_OutputSyncLow_p;
  output              o_OutputSyncHigh_p;

  // internal registers

  reg                 rx_data_n;
  reg                 InternalSync_s;
  reg                 InternalSyncR1_s;;
  reg                 InternalSync2CC_s;
  reg                 SoftSyncR1_s;
  reg                 SoftSyncRe_s;
  reg                 SyncR1_s;
  reg                 SyncRp_s;
  reg                 SyncRn_s;
  reg        [ 1:0]   Counter_s;
  reg                 SyncPulseTrain_s;
  

  // internal signals

  wire                RefClk_idelay_s;
  wire                RefClk_bufmr_s;
  wire                RefClk_bufr_s;
  wire                RefClk_bufg_s;
  wire                RefClk_s;
  wire                RefClkIn_s;
  wire      [ 4:0]    v4_CtrlInRe_s;
  wire      [ 4:0]    v4_CtrlInFe_s;
  
  // synthesis attribute keep of SyncR1_s is true;
  wire                Sync_s;
  
  genvar              l_inst;
  
  /////////////////////////////////////////////////////
  // Reference clock input
  /////////////////////////////////////////////////////
  
  generate
  if(C_CLOCK_MASTER == 1)
      assign RefClk_s = RefClk_bufg_s;
  else
      assign RefClk_s = i_RefClkFromMaster_p;
  endgenerate

  assign o_RefClkToSlave_p = RefClk_bufg_s;

  // Ref clock path on Virtex 6 devices
  // Ref clock pin -> ibufds -> idelay -> bufg
  
  // Ref clock path on 7-series devices
  // Ref clock pin -> ibufds -> idelay -> bufr -> bufg

  IBUFDS i_ref_clk_ibuf (
    .I (idp_RefClk_p),
    .IB (idn_RefClk_p),
    .O (RefClkIn_s));

  generate
  if (PCORE_DEVICE_TYPE == VIRTEX6) begin
  IODELAYE1 #(
      .CINVCTRL_SEL ("FALSE"),
      .DELAY_SRC ("I"),
      .HIGH_PERFORMANCE_MODE ("TRUE"),
      .IDELAY_TYPE ("VAR_LOADABLE"),
      .IDELAY_VALUE (0),
      .ODELAY_TYPE ("FIXED"),
      .ODELAY_VALUE (0),
      .REFCLK_FREQUENCY (200.0),
      .SIGNAL_PATTERN ("DATA"))
  i_refclk_idelay (
      .T (1'b1),
      .CE (1'b0),
      .INC (1'b0),
      .CLKIN (1'b0),
      .DATAIN (1'b0),
      .ODATAIN (1'b0),
      .CINVCTRL (1'b0),
      .C (i_DelayClk_p),
      .IDATAIN (RefClkIn_s),
      .DATAOUT (RefClk_idelay_s),
      .RST (1'b1),
      .CNTVALUEIN (iv5_RefClkIODelay_p),
      .CNTVALUEOUT ());
  end else begin
  IDELAYE2 #(
    .CINVCTRL_SEL ("FALSE"),
    .DELAY_SRC ("IDATAIN"),
    .HIGH_PERFORMANCE_MODE ("TRUE"),
    .IDELAY_TYPE ("VAR_LOAD"),
    .IDELAY_VALUE (0),
    .REFCLK_FREQUENCY (200.0),
    .PIPE_SEL ("FALSE"),
    .SIGNAL_PATTERN ("DATA"))
  i_rx_data_idelay (
    .CE (1'b0),
    .INC (1'b0),
    .DATAIN (1'b0),
    .LDPIPEEN (1'b0),
    .CINVCTRL (1'b0),
    .REGRST (1'b0),
    .C (i_DelayClk_p),
    .IDATAIN (RefClkIn_s),
    .DATAOUT (RefClk_idelay_s),
    .LD (1'b1),
    .CNTVALUEIN (iv5_RefClkIODelay_p),
    .CNTVALUEOUT ());
  end
  endgenerate
    
    generate
    if(PCORE_DEVICE_TYPE == SERIES7) begin
      BUFR i_refclk_rbuf (
        .I (RefClk_idelay_s),
        .O (RefClk_bufr_s)); 
    end else begin
      assign RefClk_bufr_s = RefClk_idelay_s;
    end
    endgenerate
    
    BUFG i_refclk_gbuf (
        .I (RefClk_bufr_s),
        // .I (i_RefClk_p),
        .O (RefClk_bufg_s));

        
    wire SyncPinRe_s;
    wire SyncPinFe_s;
  
  /////////////////////////////////////////////////////
  // External Sync input
  /////////////////////////////////////////////////////
    
    ad_lvds_in #(
        .LVDS_INPUT (0),
        .BUFTYPE (PCORE_DEVICE_TYPE),
        .IODELAY_CTRL (0))
    i_sync_in (
        .rx_clk (RefClk_s),
        .rx_data_in_p (i_SyncPin_p),
        .rx_data_in_n (1'b0),
        .rx_data_p (SyncPinRe_s),
        .rx_data_n (SyncPinFe_s),
        .delay_clk (i_DelayClk_p),
        .delay_rst (1'b0),
        .delay_ld (1'b1),
        .delay_wdata (iv5_InputSyncDelay_p),
        .delay_rdata (),
        .delay_locked ());
        
    
    assign o_InputSync_p = SyncPinRe_s;
        
    low_high_latch #(
        .C_WIDTH             (1))
    input_sync_latch_i (
        .i_Rst_p             (1'b0),
        .i_UpdateClk_p       (i_SystemClk_p),
        .i_UpdatePulse_p     (i_UpdateInputSyncLatch_p),
        .i_Clk_p             (RefClk_s),
        .iv_Data_p           (SyncPinRe_s),
        .ov_LowData_p        (o_InputSyncLow_p),
        .ov_HighData_p       (o_InputSyncHigh_p));
  
  /////////////////////////////////////////////////////
  // AD9361 Sync logic and output
  /////////////////////////////////////////////////////
    
    always @(posedge RefClk_s) begin
    
        SoftSyncRe_s <= i_SoftSync_p & (~SoftSyncR1_s);
        SoftSyncR1_s <= i_SoftSync_p;
        
        InternalSyncR1_s <= InternalSync_s;
  
        case (iv4_SyncSel_p)
            4'h0: InternalSync_s <= SoftSyncRe_s;
            4'h1: InternalSync_s <= SyncPinRe_s;
            4'h2: InternalSync_s <= SyncPinFe_s;
            4'h3: InternalSync_s <= SyncPulseTrain_s;

            default: InternalSync_s <= SoftSyncRe_s;
        endcase
        
        // At least 2 clock cycle width.
        InternalSync2CC_s <= InternalSync_s | InternalSyncR1_s;
    end
   
   //Provide sync pulse train for prod testing. The pulse is asserted on 1 out of 4 ref clock cycles 
   always @(posedge RefClk_s) begin
        Counter_s <= Counter_s + 2'h1;
        if(Counter_s >= 2'h3) begin
            Counter_s <= 2'h0;
        end
        SyncPulseTrain_s <= 1'b0;
        if(Counter_s == 2'h0) begin
            SyncPulseTrain_s <= 1'b1;
        end
    end

    generate
    if(C_CLOCK_MASTER == 1)
        assign Sync_s = InternalSync2CC_s;
    else
        assign Sync_s = i_SyncFromMaster_p;
    endgenerate
  
    assign o_SyncToSlave_p = Sync_s;
  
    always @(posedge RefClk_s) begin
        SyncR1_s <= Sync_s;
        
        if (i_SyncBitSlip_p == 1'b1) begin
            SyncRp_s <= SyncR1_s;
            SyncRn_s <= Sync_s;
        end else begin
            SyncRp_s <= Sync_s;
            SyncRn_s <= Sync_s;
        end
    end
        
    low_high_latch #(
        .C_WIDTH             (1))
    output_sync_latch_i (
        .i_Rst_p             (1'b0),
        .i_UpdateClk_p       (i_SystemClk_p),
        .i_UpdatePulse_p     (i_UpdateOutputSyncLatch_p),
        .i_Clk_p             (RefClk_s),
        .iv_Data_p           (Sync_s & (~SyncR1_s)),
        .ov_LowData_p        (o_OutputSyncLow_p),
        .ov_HighData_p       (o_OutputSyncHigh_p));
    
    ad_lvds_out #(
        .LVDS_OUTPUT (0),
        .IODELAY_OUTPUT (1),
        // .IODELAY_OUTPUT (0),
        .BUFTYPE (PCORE_DEVICE_TYPE))
    i_sync_out (
        .i_DelayClk_p(i_DelayClk_p),
        .iv5_Delay_p(iv5_OutputSyncDelay_p),
        .tx_clk (RefClk_s),
        .tx_data_p (SyncRp_s),
        .tx_data_n (SyncRn_s),
        .tx_data_out_p (o_Sync_p),
        .tx_data_out_n ());

endmodule

// ***************************************************************************
// ***************************************************************************
