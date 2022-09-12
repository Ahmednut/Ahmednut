`uselib lib=unisims_ver
`uselib lib=proc_common_v3_00_a

module user_logic
(
  input  wire                        Bus2IP_Clk,    
  input  wire                        Bus2IP_Resetn, 
  input  wire [C_SLV_DWIDTH-1 : 0]   Bus2IP_Data,   
  input  wire [C_SLV_DWIDTH/8-1 : 0] Bus2IP_BE,     
  input  wire [C_NUM_REG-1 : 0]      Bus2IP_RdCE,   
  input  wire [C_NUM_REG-1 : 0]      Bus2IP_WrCE,   
  output wire [C_SLV_DWIDTH-1 : 0]   IP2Bus_Data,   
  output wire                        IP2Bus_RdAck,  
  output wire                        IP2Bus_WrAck,  
  output wire                        IP2Bus_Error,  
  // -----------------------------------------------------------------------------------------------------------
  output wire                        O_Sync_Pulse_0_p,
  output wire                        O_Sync_Pulse_1_p,
  output wire                        O_Sync_Pulse_2_p,
  output wire                        O_Sync_Pulse_3_p,
  input  wire                        I_Sync_Pulse_From_Radio640_p,
  output wire                        O_Sync_Out_To_Radio640_0_p,
  output wire                        O_Sync_Out_To_Radio640_1_p,
  output wire                        O_Sync_Out_To_Radio640_2_p,
  output wire                        O_Sync_Out_To_Radio640_3_p,
  input  wire                        I_REF_OUT_From_Radio640_p,
  output reg                         O_ReceivedSyncPulse_p
);

parameter PCORE_IODELAY_GROUP = "delay_group_1";
parameter C_NUM_REG                      = 24;
parameter C_SLV_DWIDTH                   = 32;

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  // Nets for user logic slave model s/w accessible register example
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg0;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg1;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg2;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg3;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg4;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg5;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg6;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg7;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg8;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg9;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg10;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg11;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg12;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg13;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg14;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg15;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg16;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg17;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg18;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg19;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg20;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg21;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg22;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg23;
  wire       [23 : 0]                       slv_reg_write_sel;
  wire       [23 : 0]                       slv_reg_read_sel;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_ip2bus_data;
  wire                                      slv_read_ack;
  wire                                      slv_write_ack;
  integer                                   byte_index, bit_index;
// ---------------------------------------------------  
    assign 
    slv_reg_write_sel = Bus2IP_WrCE[23:0],
    slv_reg_read_sel  = Bus2IP_RdCE[23:0],
    slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1] || Bus2IP_WrCE[2] || Bus2IP_WrCE[3] || Bus2IP_WrCE[4] || Bus2IP_WrCE[5] || Bus2IP_WrCE[6] || Bus2IP_WrCE[7] || Bus2IP_WrCE[8] || Bus2IP_WrCE[9] || Bus2IP_WrCE[10] || Bus2IP_WrCE[11] || Bus2IP_WrCE[12] || Bus2IP_WrCE[13] || Bus2IP_WrCE[14] || Bus2IP_WrCE[15] || Bus2IP_WrCE[16] || Bus2IP_WrCE[17] || Bus2IP_WrCE[18] || Bus2IP_WrCE[19] || Bus2IP_WrCE[20] || Bus2IP_WrCE[21] || Bus2IP_WrCE[22] || Bus2IP_WrCE[23],
    slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1] || Bus2IP_RdCE[2] || Bus2IP_RdCE[3] || Bus2IP_RdCE[4] || Bus2IP_RdCE[5] || Bus2IP_RdCE[6] || Bus2IP_RdCE[7] || Bus2IP_RdCE[8] || Bus2IP_RdCE[9] || Bus2IP_RdCE[10] || Bus2IP_RdCE[11] || Bus2IP_RdCE[12] || Bus2IP_RdCE[13] || Bus2IP_RdCE[14] || Bus2IP_RdCE[15] || Bus2IP_RdCE[16] || Bus2IP_RdCE[17] || Bus2IP_RdCE[18] || Bus2IP_RdCE[19] || Bus2IP_RdCE[20] || Bus2IP_RdCE[21] || Bus2IP_RdCE[22] || Bus2IP_RdCE[23];

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
          slv_reg8 <= 0;
          slv_reg9 <= 0;
          slv_reg10 <= 0;
          slv_reg11 <= 0;
          slv_reg12 <= 0;
          slv_reg13 <= 0;
          slv_reg14 <= 0;
          slv_reg15 <= 0;
          slv_reg16 <= 0;
          slv_reg17 <= 0;
          slv_reg18 <= 0;
          slv_reg19 <= 0;
          slv_reg20 <= 0;
          slv_reg21 <= 0;
          slv_reg22 <= 0;
          slv_reg23 <= 0;
        end
      else
        case ( slv_reg_write_sel )
          24'b100000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg0[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b010000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg1[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b001000000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg2[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000100000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg3[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000010000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg4[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000001000000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg5[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000100000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg6[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000010000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg7[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000001000000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg8[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000100000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg9[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000010000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg10[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000001000000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg11[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000100000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg12[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000010000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg13[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000001000000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg14[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000100000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg15[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000010000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg16[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000001000000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg17[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000000100000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg18[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000000010000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg19[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000000001000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg20[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000000000100 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg21[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000000000010 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg22[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          24'b000000000000000000000001 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg23[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          default : begin
            slv_reg0 <= slv_reg0;
            slv_reg1 <= slv_reg1;
            slv_reg2 <= slv_reg2;
            slv_reg3 <= slv_reg3;
            slv_reg4 <= slv_reg4;
            slv_reg5 <= slv_reg5;
            slv_reg6 <= slv_reg6;
            slv_reg7 <= slv_reg7;
            slv_reg8 <= slv_reg8;
            slv_reg9 <= slv_reg9;
            slv_reg10 <= slv_reg10;
            slv_reg11 <= slv_reg11;
            slv_reg12 <= slv_reg12;
            slv_reg13 <= slv_reg13;
            slv_reg14 <= slv_reg14;
            slv_reg15 <= slv_reg15;
            slv_reg16 <= slv_reg16;
            slv_reg17 <= slv_reg17;
            slv_reg18 <= slv_reg18;
            slv_reg19 <= slv_reg19;
            slv_reg20 <= slv_reg20;
            slv_reg21 <= slv_reg21;
            slv_reg22 <= slv_reg22;
            slv_reg23 <= slv_reg23;
                    end
        endcase

    end // SLAVE_REG_WRITE_PROC

  // implement slave model register read mux
  always @( slv_reg_read_sel or slv_reg0 or slv_reg1 or slv_reg2 or slv_reg3 or slv_reg4 or slv_reg5 or slv_reg6 or slv_reg7 or slv_reg8 or slv_reg9 or slv_reg10 or slv_reg11 or slv_reg12 or slv_reg13 or slv_reg14 or slv_reg15 or slv_reg16 or slv_reg17 or slv_reg18 or slv_reg19 or slv_reg20 or slv_reg21 or slv_reg22 or slv_reg23 )
    begin 
      case ( slv_reg_read_sel )
        24'b100000000000000000000000 : slv_ip2bus_data <= slv_reg0;
        24'b010000000000000000000000 : slv_ip2bus_data <= slv_reg1;
        24'b001000000000000000000000 : slv_ip2bus_data <= slv_reg2;
        24'b000100000000000000000000 : slv_ip2bus_data <= slv_reg3;
        //24'b000010000000000000000000 : slv_ip2bus_data <= slv_reg4;
        24'b000010000000000000000000 : slv_ip2bus_data <= {31'h00000000,Received_Sync_Pulse_for_uB};
        24'b000001000000000000000000 : slv_ip2bus_data <= slv_reg5;
        24'b000000100000000000000000 : slv_ip2bus_data <= slv_reg6;
        24'b000000010000000000000000 : slv_ip2bus_data <= slv_reg7;
        24'b000000001000000000000000 : slv_ip2bus_data <= slv_reg8;
        24'b000000000100000000000000 : slv_ip2bus_data <= slv_reg9;
        24'b000000000010000000000000 : slv_ip2bus_data <= slv_reg10;
        24'b000000000001000000000000 : slv_ip2bus_data <= slv_reg11;
        24'b000000000000100000000000 : slv_ip2bus_data <= slv_reg12;
        24'b000000000000010000000000 : slv_ip2bus_data <= slv_reg13;
        24'b000000000000001000000000 : slv_ip2bus_data <= slv_reg14;
        24'b000000000000000100000000 : slv_ip2bus_data <= slv_reg15;
        24'b000000000000000010000000 : slv_ip2bus_data <= slv_reg16;
        24'b000000000000000001000000 : slv_ip2bus_data <= slv_reg17;
        24'b000000000000000000100000 : slv_ip2bus_data <= slv_reg18;
        24'b000000000000000000010000 : slv_ip2bus_data <= slv_reg19;
        24'b000000000000000000001000 : slv_ip2bus_data <= slv_reg20;
        24'b000000000000000000000100 : slv_ip2bus_data <= slv_reg21;
        24'b000000000000000000000010 : slv_ip2bus_data <= slv_reg22;
        24'b000000000000000000000001 : slv_ip2bus_data <= slv_reg23;
        default : slv_ip2bus_data <= 0;
      endcase

    end // SLAVE_REG_READ_PROC

   
// ---------------------------------------------------
  assign IP2Bus_Data  = (slv_read_ack == 1'b1) ? slv_ip2bus_data :  0 ;
  assign IP2Bus_WrAck = slv_write_ack;
  assign IP2Bus_RdAck = slv_read_ack;
  assign IP2Bus_Error = 0;

  // slv_reg0: Sync Pulse
  // slv_reg1: Sync_Pulse_In delay adjust
  // slv_reg2: O_Sync_Out_To_Radio640_0_p delay adjust
  // slv_reg3: O_Sync_Out_To_Radio640_1_p delay adjust
  // slv_reg4: Sync Pulse Received
  // slv_reg5: Sync Pulse Clear
  // slv_reg6: Pulse train frequency
  // slv_reg7: Pulse/Pulse Train mux
  // slv_reg8: O_Sync_Out_To_Radio640_2_p delay adjust
  // slv_reg9: O_Sync_Out_To_Radio640_3_p delay adjust

  //-------------------------------------------------------------------------------------------------------------------
  wire REF_IN_CLK;
   BUFG BUFG_inst (
      .O(REF_IN_CLK), // 1-bit output: Clock buffer output
      .I(I_REF_OUT_From_Radio640_p)  // 1-bit input: Clock buffer input
   );
  
  //-------------------------------------------------------------------------------------------------------------------
  // (* IODELAY_GROUP = "delay_group_MCS" *) // Specifies group name for associated IODELAYs and IDELAYCTRL
    // IDELAYCTRL IDELAYCTRL_inst (
    // .RDY(),                               // 1-bit Ready output
    // .REFCLK(Bus2IP_Clk),                  // 1-bit Reference clock input
    // .RST(1'b0)                            // 1-bit Reset input
  // );
  //-------------------------------------------------------------------------------------------------------------------
  // Sync Pulse Reception
  wire Sync_Pulse_In;
  reg Sync_Pulse_Rising_Edge_Detected;
  reg Received_Sync_Pulse_delayed = 1'b0;
  reg Received_Sync_Pulse_for_uB = 1'b0;
  (* IOB = "true" *) reg Sync_Out_0 = 1'b0;
  (* IOB = "true" *) reg Sync_Out_1 = 1'b0;
  (* IOB = "true" *) reg Sync_Out_2 = 1'b0;
  (* IOB = "true" *) reg Sync_Out_3 = 1'b0;

  always @(posedge REF_IN_CLK) begin
    Sync_Out_0 <= O_ReceivedSyncPulse_p;
    Sync_Out_1 <= O_ReceivedSyncPulse_p;
    Sync_Out_2 <= O_ReceivedSyncPulse_p;
    Sync_Out_3 <= O_ReceivedSyncPulse_p;
  end

  always @(posedge REF_IN_CLK) begin
    O_ReceivedSyncPulse_p <= Sync_Pulse_In;
    Received_Sync_Pulse_delayed <= O_ReceivedSyncPulse_p;
    if(slv_reg5[0] == 1'b1)
      Sync_Pulse_Rising_Edge_Detected <= 1'b0;
    else
      if((Received_Sync_Pulse_delayed == 1'b0) && (O_ReceivedSyncPulse_p == 1'b1))
        Sync_Pulse_Rising_Edge_Detected <= 1'b1;
  end

  always @( posedge Bus2IP_Clk ) begin
    if(slv_reg5[0] == 1'b1)
      Received_Sync_Pulse_for_uB <= 1'b0;
    else
      Received_Sync_Pulse_for_uB <= Sync_Pulse_Rising_Edge_Detected;
  end

  // (* IODELAY_GROUP = "FMC_IODELAY_GRP" *)     // Specifies group name for associated IODELAYs and IDELAYCTRL
  // IODELAYE1 #(
    // .CINVCTRL_SEL("FALSE"),                   // Enable dynamic clock inversion ("TRUE"/"FALSE") 
    // .DELAY_SRC("I"),                          // Delay input ("I", "CLKIN", "DATAIN", "IO", "O")
    // .HIGH_PERFORMANCE_MODE("TRUE"),           // Reduced jitter ("TRUE"), Reduced power ("FALSE")
    // .IDELAY_TYPE("VAR_LOADABLE"),             // "DEFAULT", "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    // .IDELAY_VALUE(0),                         // Input delay tap setting (0-32)
    // .ODELAY_TYPE("FIXED"),                    // "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    // .ODELAY_VALUE(0),                         // Output delay tap setting (0-32)
    // .REFCLK_FREQUENCY(200.0),                 // IDELAYCTRL clock input frequency in MHz
    // .SIGNAL_PATTERN("DATA")                   // "DATA" or "CLOCK" input signal
  // )
  // IODELAYE1_Sync_In(
    // .CNTVALUEOUT(),                           // 5-bit output - Counter value for monitoring purpose
    // .DATAOUT(Sync_Pulse_In),                  // 1-bit output - Delayed data output
    // .C(Bus2IP_Clk),                           // 1-bit input - Clock input
    // .CE(1'b0),                                // 1-bit input - Active high enable increment/decrement function
    // .CINVCTRL(1'b0),                          // 1-bit input - Dynamically inverts the Clock (C) polarity
    // .CLKIN(1'b0),                             // 1-bit input - Clock Access into the IODELAY
    // .CNTVALUEIN(slv_reg1[4:0]),               // 5-bit input - Counter value for loadable counter application
    // .DATAIN(1'b0),                            // 1-bit input - Internal delay data
    // .IDATAIN(I_Sync_Pulse_From_Radio640_p),   // 1-bit input - Delay data input
    // .INC(1'b0),                               // 1-bit input - Increment / Decrement tap delay
    // .ODATAIN(1'b0),                           // 1-bit input - Data input for the output datapath from the device
    // .RST(1'b1),                               // 1-bit input - Active high, synchronous reset, resets delay chain to IDELAY_VALUE/
    //                                          ODELAY_VALUE tap. If no value is specified, the default is 0.
    // .T(1'b1)                                  // 1-bit input - 3-state input control. Tie high for input-only or internal delay or
    //                                          tie low for output only.
  // );
 assign Sync_Pulse_In = I_Sync_Pulse_From_Radio640_p;
  //-------------------------------------------------------------------------------------------------------------------
  // slv_reg0's bit 0 is used to generated Sync pulses.
  // Sync pulse strobe

  reg [2:0] Soft_Sync_CDC;
  reg Soft_Sync_delayed = 1'b0;
  wire Soft_Sync_Pulse;

  wire Soft_Sync_Pulse_Train;
  reg [15:0] Soft_Sync_Pulse_Train_CNT = 16'h0000;

  // Soft sync pulse
  always @(posedge REF_IN_CLK) begin
    Soft_Sync_CDC[0] <= slv_reg0[0];
    Soft_Sync_CDC[1] <= Soft_Sync_CDC[0];
    Soft_Sync_CDC[2] <= Soft_Sync_CDC[1];
    
    Soft_Sync_delayed <= Soft_Sync_CDC[2];
  end
  assign Soft_Sync_Pulse = (Soft_Sync_delayed == 1'b0) && (Soft_Sync_CDC[2] == 1'b1);

  // Soft sync pulse train
  always @(posedge REF_IN_CLK) begin
    if(Soft_Sync_Pulse_Train == 1'b1)
      Soft_Sync_Pulse_Train_CNT <= 16'h0000;
    else
      Soft_Sync_Pulse_Train_CNT <= Soft_Sync_Pulse_Train_CNT + 1'b1;
  end
  assign Soft_Sync_Pulse_Train = (Soft_Sync_Pulse_Train_CNT == slv_reg6[15:0])? 1'b1 : 1'b0;

  // Soft sync mux
  (* IOB = "true" *) reg PULSE_0;
  (* IOB = "true" *) reg PULSE_1;
  (* IOB = "true" *) reg PULSE_2;
  (* IOB = "true" *) reg PULSE_3;
  always @(posedge REF_IN_CLK) begin
    if(slv_reg7[0] == 1'b1) begin
      PULSE_0 <= Soft_Sync_Pulse_Train;
      PULSE_1 <= Soft_Sync_Pulse_Train;
      PULSE_2 <= Soft_Sync_Pulse_Train;
      PULSE_3 <= Soft_Sync_Pulse_Train;
    end
    else begin
      PULSE_0 <= Soft_Sync_Pulse;
      PULSE_1 <= Soft_Sync_Pulse;
      PULSE_2 <= Soft_Sync_Pulse;
      PULSE_3 <= Soft_Sync_Pulse;
    end
  end
  assign O_Sync_Pulse_0_p = PULSE_0;
  assign O_Sync_Pulse_1_p = PULSE_1;
  assign O_Sync_Pulse_2_p = PULSE_2;
  assign O_Sync_Pulse_3_p = PULSE_3;


  // ODelays
  (* IODELAY_GROUP = "FMC_IODELAY_GRP" *)      // Specifies group name for associated IODELAYs and IDELAYCTRL
  IODELAYE1 #(
    .CINVCTRL_SEL("FALSE"),                  // Enable dynamic clock inversion ("TRUE"/"FALSE") 
    .DELAY_SRC("O"),                         // Delay input ("I", "CLKIN", "DATAIN", "IO", "O")
    .HIGH_PERFORMANCE_MODE("TRUE"),          // Reduced jitter ("TRUE"), Reduced power ("FALSE")
    .IDELAY_TYPE("VAR_LOADABLE"),            // "DEFAULT", "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    .IDELAY_VALUE(0),                        // Input delay tap setting (0-32)
    .ODELAY_TYPE("VAR_LOADABLE"),            // "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    .ODELAY_VALUE(0),                        // Output delay tap setting (0-32)
    .REFCLK_FREQUENCY(200.0),                // IDELAYCTRL clock input frequency in MHz
    .SIGNAL_PATTERN("DATA")                  // "DATA" or "CLOCK" input signal
  )
  IODELAYE1_Sync_Out_0(
    .CNTVALUEOUT(),                          // 5-bit output - Counter value for monitoring purpose
    .DATAOUT(O_Sync_Out_To_Radio640_0_p),    // 1-bit output - Delayed data output
    .C(Bus2IP_Clk),                          // 1-bit input - Clock input
    .CE(1'b1),                               // 1-bit input - Active high enable increment/decrement function
    .CINVCTRL(1'b0),                         // 1-bit input - Dynamically inverts the Clock (C) polarity
    .CLKIN(1'b0),                            // 1-bit input - Clock Access into the IODELAY
    .CNTVALUEIN(slv_reg2[4:0]),              // 5-bit input - Counter value for loadable counter application
    .DATAIN(1'b0),                           // 1-bit input - Internal delay data
    .IDATAIN(1'b0),                          // 1-bit input - Delay data input
    .INC(1'b1),                              // 1-bit input - Increment / Decrement tap delay
    .ODATAIN(Sync_Out_0),                    // 1-bit input - Data input for the output datapath from the device
    .RST(1'b1),                              // 1-bit input - Active high, synchronous reset, resets delay chain to IDELAY_VALUE/
                                             // ODELAY_VALUE tap. If no value is specified, the default is 0.
    .T(1'b0)                                 // 1-bit input - 3-state input control. Tie high for input-only or internal delay or
                                             // tie low for output only.
  );

  (* IODELAY_GROUP = "FMC_IODELAY_GRP" *)      // Specifies group name for associated IODELAYs and IDELAYCTRL
  IODELAYE1 #(          
    .CINVCTRL_SEL("FALSE"),                  // Enable dynamic clock inversion ("TRUE"/"FALSE") 
    .DELAY_SRC("O"),                         // Delay input ("I", "CLKIN", "DATAIN", "IO", "O")
    .HIGH_PERFORMANCE_MODE("TRUE"),          // Reduced jitter ("TRUE"), Reduced power ("FALSE")
    .IDELAY_TYPE("VAR_LOADABLE"),            // "DEFAULT", "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    .IDELAY_VALUE(0),                        // Input delay tap setting (0-32)
    .ODELAY_TYPE("VAR_LOADABLE"),            // "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    .ODELAY_VALUE(0),                        // Output delay tap setting (0-32)
    .REFCLK_FREQUENCY(200.0),                // IDELAYCTRL clock input frequency in MHz
    .SIGNAL_PATTERN("DATA")                  // "DATA" or "CLOCK" input signal
  )
  IODELAYE1_Sync_Out_1(
    .CNTVALUEOUT(),                          // 5-bit output - Counter value for monitoring purpose
    .DATAOUT(O_Sync_Out_To_Radio640_1_p),    // 1-bit output - Delayed data output
    .C(Bus2IP_Clk),                          // 1-bit input - Clock input
    .CE(1'b1),                               // 1-bit input - Active high enable increment/decrement function
    .CINVCTRL(1'b0),                         // 1-bit input - Dynamically inverts the Clock (C) polarity
    .CLKIN(1'b0),                            // 1-bit input - Clock Access into the IODELAY
    .CNTVALUEIN(slv_reg3[4:0]),              // 5-bit input - Counter value for loadable counter application
    .DATAIN(1'b0),                           // 1-bit input - Internal delay data
    .IDATAIN(1'b0),                          // 1-bit input - Delay data input
    .INC(1'b1),                              // 1-bit input - Increment / Decrement tap delay
    .ODATAIN(Sync_Out_1),                    // 1-bit input - Data input for the output datapath from the device
    .RST(1'b1),                              // 1-bit input - Active high, synchronous reset, resets delay chain to IDELAY_VALUE/
                                             // ODELAY_VALUE tap. If no value is specified, the default is 0.
    .T(1'b0)                                 // 1-bit input - 3-state input control. Tie high for input-only or internal delay or
                                             // tie low for output only.
  );
  
  (* IODELAY_GROUP = "FMC_IODELAY_GRP" *)      // Specifies group name for associated IODELAYs and IDELAYCTRL
  IODELAYE1 #(
    .CINVCTRL_SEL("FALSE"),                  // Enable dynamic clock inversion ("TRUE"/"FALSE") 
    .DELAY_SRC("O"),                         // Delay input ("I", "CLKIN", "DATAIN", "IO", "O")
    .HIGH_PERFORMANCE_MODE("TRUE"),          // Reduced jitter ("TRUE"), Reduced power ("FALSE")
    .IDELAY_TYPE("VAR_LOADABLE"),            // "DEFAULT", "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    .IDELAY_VALUE(0),                        // Input delay tap setting (0-32)
    .ODELAY_TYPE("VAR_LOADABLE"),            // "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    .ODELAY_VALUE(0),                        // Output delay tap setting (0-32)
    .REFCLK_FREQUENCY(200.0),                // IDELAYCTRL clock input frequency in MHz
    .SIGNAL_PATTERN("DATA")                  // "DATA" or "CLOCK" input signal
  )
  IODELAYE1_Sync_Out_2(
    .CNTVALUEOUT(),                          // 5-bit output - Counter value for monitoring purpose
    .DATAOUT(O_Sync_Out_To_Radio640_2_p),    // 1-bit output - Delayed data output
    .C(Bus2IP_Clk),                          // 1-bit input - Clock input
    .CE(1'b1),                               // 1-bit input - Active high enable increment/decrement function
    .CINVCTRL(1'b0),                         // 1-bit input - Dynamically inverts the Clock (C) polarity
    .CLKIN(1'b0),                            // 1-bit input - Clock Access into the IODELAY
    .CNTVALUEIN(slv_reg8[4:0]),              // 5-bit input - Counter value for loadable counter application
    .DATAIN(1'b0),                           // 1-bit input - Internal delay data
    .IDATAIN(1'b0),                          // 1-bit input - Delay data input
    .INC(1'b1),                              // 1-bit input - Increment / Decrement tap delay
    .ODATAIN(Sync_Out_2),                    // 1-bit input - Data input for the output datapath from the device
    .RST(1'b1),                              // 1-bit input - Active high, synchronous reset, resets delay chain to IDELAY_VALUE/
                                             // ODELAY_VALUE tap. If no value is specified, the default is 0.
    .T(1'b0)                                 // 1-bit input - 3-state input control. Tie high for input-only or internal delay or
                                             // tie low for output only.
  );
  
  (* IODELAY_GROUP = "FMC_IODELAY_GRP" *)    // Specifies group name for associated IODELAYs and IDELAYCTRL
  IODELAYE1 #(
    .CINVCTRL_SEL("FALSE"),                  // Enable dynamic clock inversion ("TRUE"/"FALSE") 
    .DELAY_SRC("O"),                         // Delay input ("I", "CLKIN", "DATAIN", "IO", "O")
    .HIGH_PERFORMANCE_MODE("TRUE"),          // Reduced jitter ("TRUE"), Reduced power ("FALSE")
    .IDELAY_TYPE("VAR_LOADABLE"),            // "DEFAULT", "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    .IDELAY_VALUE(0),                        // Input delay tap setting (0-32)
    .ODELAY_TYPE("VAR_LOADABLE"),            // "FIXED", "VARIABLE", or "VAR_LOADABLE" 
    .ODELAY_VALUE(0),                        // Output delay tap setting (0-32)
    .REFCLK_FREQUENCY(200.0),                // IDELAYCTRL clock input frequency in MHz
    .SIGNAL_PATTERN("DATA")                  // "DATA" or "CLOCK" input signal
  )
  IODELAYE1_Sync_Out_3(
    .CNTVALUEOUT(),                          // 5-bit output - Counter value for monitoring purpose
    .DATAOUT(O_Sync_Out_To_Radio640_3_p),    // 1-bit output - Delayed data output
    .C(Bus2IP_Clk),                          // 1-bit input - Clock input
    .CE(1'b1),                               // 1-bit input - Active high enable increment/decrement function
    .CINVCTRL(1'b0),                         // 1-bit input - Dynamically inverts the Clock (C) polarity
    .CLKIN(1'b0),                            // 1-bit input - Clock Access into the IODELAY
    .CNTVALUEIN(slv_reg9[4:0]),              // 5-bit input - Counter value for loadable counter application
    .DATAIN(1'b0),                           // 1-bit input - Internal delay data
    .IDATAIN(1'b0),                          // 1-bit input - Delay data input
    .INC(1'b1),                              // 1-bit input - Increment / Decrement tap delay
    .ODATAIN(Sync_Out_3),                    // 1-bit input - Data input for the output datapath from the device
    .RST(1'b1),                              // 1-bit input - Active high, synchronous reset, resets delay chain to IDELAY_VALUE/
                                             // ODELAY_VALUE tap. If no value is specified, the default is 0.
    .T(1'b0)                                 // 1-bit input - 3-state input control. Tie high for input-only or internal delay or
                                             // tie low for output only.
  );
  //-------------------------------------------------------------------------------------------------------------------

endmodule



