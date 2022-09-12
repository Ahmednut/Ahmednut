`uselib lib=unisims_ver
`uselib lib=proc_common_v3_00_a

module user_logic
(
  input                               Bus2IP_Clk,     // Bus to IP clock
  input                               Bus2IP_Resetn,  // Bus to IP reset
  input      [C_SLV_DWIDTH-1 : 0]     Bus2IP_Data,    // Bus to IP data bus
  input      [C_SLV_DWIDTH/8-1 : 0]   Bus2IP_BE,      // Bus to IP byte enables
  input      [C_NUM_REG-1 : 0]        Bus2IP_RdCE,    // Bus to IP read chip enable
  input      [C_NUM_REG-1 : 0]        Bus2IP_WrCE,    // Bus to IP write chip enable
  output     [C_SLV_DWIDTH-1 : 0]     IP2Bus_Data,    // IP to Bus data bus
  output                              IP2Bus_RdAck,   // IP to Bus read transfer acknowledgement
  output                              IP2Bus_WrAck,   // IP to Bus write transfer acknowledgement
  output                              IP2Bus_Error,   // IP to Bus error response
  //-----------------------------------------------------------------
  input   wire        i_TrigIn_p,
  input   wire        i_TrigInClk_p,
  input   wire        i_DesignClk_p,
  output  reg         o_En_p = 1'b0
);

parameter C_NUM_REG                      = 8;
parameter C_SLV_DWIDTH                   = 32;

reg        [C_SLV_DWIDTH-1 : 0]           slv_reg0;
reg        [C_SLV_DWIDTH-1 : 0]           slv_reg1;
reg        [C_SLV_DWIDTH-1 : 0]           slv_reg2;
reg        [C_SLV_DWIDTH-1 : 0]           slv_reg3;
reg        [C_SLV_DWIDTH-1 : 0]           slv_reg4;
reg        [C_SLV_DWIDTH-1 : 0]           slv_reg5;
reg        [C_SLV_DWIDTH-1 : 0]           slv_reg6;
reg        [C_SLV_DWIDTH-1 : 0]           slv_reg7;
wire       [7 : 0]                        slv_reg_write_sel;
wire       [7 : 0]                        slv_reg_read_sel;
reg        [C_SLV_DWIDTH-1 : 0]           slv_ip2bus_data;
wire                                      slv_read_ack;
wire                                      slv_write_ack;
integer                                   byte_index, bit_index;

assign
  slv_reg_write_sel = Bus2IP_WrCE[7:0],
  slv_reg_read_sel  = Bus2IP_RdCE[7:0],
  slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1] || Bus2IP_WrCE[2] || Bus2IP_WrCE[3] || Bus2IP_WrCE[4] || Bus2IP_WrCE[5] || Bus2IP_WrCE[6] || Bus2IP_WrCE[7],
  slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1] || Bus2IP_RdCE[2] || Bus2IP_RdCE[3] || Bus2IP_RdCE[4] || Bus2IP_RdCE[5] || Bus2IP_RdCE[6] || Bus2IP_RdCE[7];

// implement slave model register(s)
always @( posedge Bus2IP_Clk )
  begin

    if ( Bus2IP_Resetn == 1'b0 )
      begin
        slv_reg0 <= 0;
        slv_reg1 <= 0;
        slv_reg2 <= 0;
        slv_reg3 <= 0;
        slv_reg4 <= 0;
        slv_reg5 <= 0;
        slv_reg6 <= 0;
        slv_reg7 <= 0;
      end
    else
      case ( slv_reg_write_sel )
        8'b10000000 :
          for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
            if ( Bus2IP_BE[byte_index] == 1 )
              slv_reg0[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
        8'b01000000 :
          for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
            if ( Bus2IP_BE[byte_index] == 1 )
              slv_reg1[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
        8'b00100000 :
          for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
            if ( Bus2IP_BE[byte_index] == 1 )
              slv_reg2[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
        8'b00010000 :
          for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
            if ( Bus2IP_BE[byte_index] == 1 )
              slv_reg3[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
        8'b00001000 :
          for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
            if ( Bus2IP_BE[byte_index] == 1 )
              slv_reg4[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
        8'b00000100 :
          for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
            if ( Bus2IP_BE[byte_index] == 1 )
              slv_reg5[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
        8'b00000010 :
          for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
            if ( Bus2IP_BE[byte_index] == 1 )
              slv_reg6[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
        8'b00000001 :
          for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
            if ( Bus2IP_BE[byte_index] == 1 )
              slv_reg7[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
        default : begin
          slv_reg0 <= slv_reg0;
          slv_reg1 <= slv_reg1;
          slv_reg2 <= slv_reg2;
          slv_reg3 <= slv_reg3;
          slv_reg4 <= slv_reg4;
          slv_reg5 <= slv_reg5;
          slv_reg6 <= slv_reg6;
          slv_reg7 <= slv_reg7;
                  end
      endcase

  end // SLAVE_REG_WRITE_PROC

// implement slave model register read mux
always @( slv_reg_read_sel or slv_reg0 or slv_reg1 or slv_reg2 or slv_reg3 or slv_reg4 or slv_reg5 or slv_reg6 or slv_reg7 )
  begin 

    case ( slv_reg_read_sel )
      // slv_reg0 <-- TriggerReset
      8'b10000000 : slv_ip2bus_data <= slv_reg0;
      // slv_reg1 <-- CounterValue
      8'b01000000 : slv_ip2bus_data <= CounterValue;
      // slv_reg2 --> CounterReset
      8'b00100000 : slv_ip2bus_data <= slv_reg2;
      // slv_reg3 --> CounterEnable
      8'b00010000 : slv_ip2bus_data <= slv_reg3;
      // slv_reg4 <-- TrigPulseReceived
      8'b00001000 : slv_ip2bus_data <= {31'h00000000, TrigPulseReceived};
      // slv_reg5 --> TriggerEnable
      8'b00000100 : slv_ip2bus_data <= slv_reg5;   
      // slv_reg6 --> CheckPoint1
      8'b00000010 : slv_ip2bus_data <= slv_reg6;
      // slv_reg7 --> CheckPoint2
      8'b00000001 : slv_ip2bus_data <= slv_reg7;
      default : slv_ip2bus_data <= 0;
    endcase

  end // SLAVE_REG_READ_PROC

// ------------------------------------------------------------
// Example code to drive IP to Bus signals
// ------------------------------------------------------------

assign IP2Bus_Data = (slv_read_ack == 1'b1) ? slv_ip2bus_data :  0 ;
assign IP2Bus_WrAck = slv_write_ack;
assign IP2Bus_RdAck = slv_read_ack;
assign IP2Bus_Error = 0;


// ------------------------------------------------------------
//                            Register Map
// ------------------------------------------------------------
// slv_reg0[0]    : TriggerReset
// slv_reg1[31:0] : Current Counter Value
// slv_reg2[0]    : Counter Reset
// slv_reg3[0]    : Counter Enable
// slv_reg4[0]    : Trig Pulse Received
// slv_reg5[0]    : Trigger Enable
// slv_reg6[31:0] : Check Point Value 1
// slv_reg7[31:0] : Check Point Value 2

reg  [31:0] CounterValue = 32'h00000000;
wire [31:0] CheckPoint1;
wire [31:0] CheckPoint2;

wire CounterReset;
wire CounterEnable;
wire TriggerEnable;
wire TriggerReset;

assign TriggerReset  = slv_reg0[0];
assign CounterReset  = slv_reg2[0];
assign CounterEnable = slv_reg3[0];
assign TriggerEnable = slv_reg5[0];
assign CheckPoint1   = slv_reg6[31:0];
assign CheckPoint2   = slv_reg7[31:0];

//----------------------------------------------------------
// Trig Pulse Clock Domain Crossing
//----------------------------------------------------------
wire TrigInAfterCDC;
reg TrigPulseReceived = 1'b0;

syncer TrigCDC
(
  .i_InClk_p(i_TrigInClk_p),
  .i_InSig_p(i_TrigIn_p),
  .i_OutClk_p(i_DesignClk_p),
  .o_OutSig_p(TrigInAfterCDC)
);

always @(posedge i_DesignClk_p) begin
  if(CounterReset == 1'b1)
    TrigPulseReceived <= 1'b0;
  else
    if(TrigInAfterCDC == 1'b1)
      TrigPulseReceived <= 1'b1;
end

//----------------------------------------------------------
// Counter Implementation
//----------------------------------------------------------

always @(posedge i_DesignClk_p) begin
  if(CounterReset == 1'b1)
    CounterValue <= 32'h00000000;
  else
    if(TrigPulseReceived == 1'b1 && CounterEnable == 1'b1)
      CounterValue <= CounterValue + 1'b1;
end


always @(posedge i_DesignClk_p) begin
  if(TriggerReset | CounterReset == 1'b1) begin
    o_En_p <= 0;
  end else begin
    if(TriggerEnable == 1'b1) begin
      if(CheckPoint1 == CheckPoint2) begin
        if(CounterValue >= CheckPoint1) 
          o_En_p <= 1'b1;
      end else begin
        if((CounterValue == CheckPoint1) || (CounterValue == CheckPoint2))
          o_En_p <= ~o_En_p; 
      end
    end
  end
end

endmodule
