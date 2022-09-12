`uselib lib=unisims_ver
`uselib lib=proc_common_v3_00_a

module user_logic
(
 input                             Bus2IP_Clk,                     // Bus to IP clock
 input                             Bus2IP_Resetn,                  // Bus to IP reset
 input      [C_SLV_DWIDTH-1 : 0]   Bus2IP_Data,                    // Bus to IP data bus
 input      [C_SLV_DWIDTH/8-1 : 0] Bus2IP_BE,                      // Bus to IP byte enables
 input      [C_NUM_REG-1 : 0]      Bus2IP_RdCE,                    // Bus to IP read chip enable
 input      [C_NUM_REG-1 : 0]      Bus2IP_WrCE,                    // Bus to IP write chip enable
 output     [C_SLV_DWIDTH-1 : 0]   IP2Bus_Data,                    // IP to Bus data bus
 output                            IP2Bus_RdAck,                   // IP to Bus read transfer acknowledgement
 output                            IP2Bus_WrAck,                   // IP to Bus write transfer acknowledgement
 output                            IP2Bus_Error,                   // IP to Bus error response
  // -- DO NOT EDIT ABOVE THIS LINE ------------------
 output wire [7:0] GPIO_PAD_O,
 output wire [7:0] GPIO_PAD_T,
 input  wire [7:0] GPIO_PAD_I,
 input  wire [7:0] GPIO_VALUE_OUT,
 output wire [7:0] GPIO_VALUE_IN
);

parameter C_NUM_REG                      = 4;
parameter C_SLV_DWIDTH                   = 32;

// User parameters
parameter C_GPIO_GROUP_0_IS_OUTPUT       = 0;
parameter C_GPIO_GROUP_1_IS_OUTPUT       = 0;
parameter C_GPIO_GROUP_2_IS_OUTPUT       = 0;
parameter C_GPIO_GROUP_3_IS_OUTPUT       = 0;

//----------------------------------------------------------------------------
// Implementation
//----------------------------------------------------------------------------

  // --USER nets declarations added here, as needed for user logic

  // Nets for user logic slave model s/w accessible register example
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg0;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg1;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg2;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_reg3;
  wire       [3 : 0]                        slv_reg_write_sel;
  wire       [3 : 0]                        slv_reg_read_sel;
  reg        [C_SLV_DWIDTH-1 : 0]           slv_ip2bus_data;
  wire                                      slv_read_ack;
  wire                                      slv_write_ack;
  integer                                   byte_index, bit_index;
  wire       [7:0]                          Gpio_Direction;

  assign
    slv_reg_write_sel = Bus2IP_WrCE[3:0],
    slv_reg_read_sel  = Bus2IP_RdCE[3:0],
    slv_write_ack     = Bus2IP_WrCE[0] || Bus2IP_WrCE[1] || Bus2IP_WrCE[2] || Bus2IP_WrCE[3],
    slv_read_ack      = Bus2IP_RdCE[0] || Bus2IP_RdCE[1] || Bus2IP_RdCE[2] || Bus2IP_RdCE[3];

  // implement slave model register(s)
  always @( posedge Bus2IP_Clk )
    begin

      if ( Bus2IP_Resetn == 1'b0 )
        begin
          slv_reg0 <= 0;
          slv_reg1 <= 0;
          slv_reg2 <= 0;
          slv_reg3 <= 0;
        end
      else
        case ( slv_reg_write_sel )
          4'b1000 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg0[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          4'b0100 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg1[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          4'b0010 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg2[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          4'b0001 :
            for ( byte_index = 0; byte_index <= (C_SLV_DWIDTH/8)-1; byte_index = byte_index+1 )
              if ( Bus2IP_BE[byte_index] == 1 )
                slv_reg3[(byte_index*8) +: 8] <= Bus2IP_Data[(byte_index*8) +: 8];
          default : begin
            slv_reg0 <= slv_reg0;
            slv_reg1 <= slv_reg1;
            slv_reg2 <= slv_reg2;
            slv_reg3 <= slv_reg3;
                    end
        endcase

    end // SLAVE_REG_WRITE_PROC

  // implement slave model register read mux
  always @( slv_reg_read_sel or slv_reg0 or slv_reg1 or slv_reg2 or slv_reg3 or pipeline[1] or Gpio_Direction)
    begin 

      case ( slv_reg_read_sel )
        // slv_reg0: Value present at GPIO_PADs (input value)
        4'b1000 : slv_ip2bus_data <= {24'h000000, pipeline[1]};
        
        // slv_reg1: Value written to GPIO output register (written by host or FPGA interface).
        //           This register is effective only if the control register (slv_reg3) allows the host to write values to the GPIO and the direction is set to output.
        4'b0100 : slv_ip2bus_data <= slv_reg1;
        
        // slv_reg2: GPIO direction mask (written by host): For each bit --> Input = 0, Output = 1
        //4'b0010 : slv_ip2bus_data <= slv_reg2;
		4'b0010 : slv_ip2bus_data <= {24'h000000, Gpio_Direction};
        
        // slv_reg3: GPIO owner mask: For each bit --> Host (AXI bus) is owner = 0 , FPGA interface is owner = 1
        4'b0001 : slv_ip2bus_data <= slv_reg3;
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
  // User logic
  // ------------------------------------------------------------
  
  // Reg Map: 
  // slv_reg0: Value present at GPIO_PADs (input value)
  // slv_reg1: Value written to GPIO output register (written by host or FPGA interface).
  //           This register is effective only if the control register (slv_reg3) allows the host to write values to the GPIO and the direction is set to output.
  // slv_reg2: GPIO direction mask (written by host): For each bit --> Input = 0, Output = 1
  // slv_reg3: GPIO owner mask: For each bit --> Host (AXI bus) is owner = 0 , FPGA interface is owner = 1
  
  //assign GPIO_PAD_T = ~slv_reg2[7:0]; // GPIO direction: 0 = input, 1 = output
  // ------------------------------------------------------------
  // GPIO direction is defined by generics on build time.
  // ------------------------------------------------------------
  
  generate
  if(C_GPIO_GROUP_0_IS_OUTPUT == 1) 	
	assign Gpio_Direction[1:0] = 2'b11; // Output
  else	
	assign Gpio_Direction[1:0] = 2'b00; // Input by default
  endgenerate
  
  generate
  if(C_GPIO_GROUP_1_IS_OUTPUT == 1) 	
	assign Gpio_Direction[3:2] = 2'b11;
  else	
	assign Gpio_Direction[3:2] = 2'b00;
  endgenerate
  
  generate
  if(C_GPIO_GROUP_2_IS_OUTPUT == 1)	
	assign Gpio_Direction[5:4] = 2'b11;
  else	
	assign Gpio_Direction[5:4] = 2'b00;
  endgenerate
  
  generate
  if(C_GPIO_GROUP_3_IS_OUTPUT == 1)	
	assign Gpio_Direction[7:6] = 2'b11;
  else	
	assign Gpio_Direction[7:6] = 2'b00;
  endgenerate
    
  assign GPIO_PAD_T = ~Gpio_Direction; // GPIO direction: 0 = input, 1 = output
      
  // -------------------------------------------------------------------
  // GPIO Output Ownership (AXI-Lite config reg or FPGA user logic) mux
  // -------------------------------------------------------------------
  genvar index;
  generate
  for (index=0; index < 8; index=index+1)
    begin: mux_label
      assign GPIO_PAD_O[index] = slv_reg3[index] == 1'b1 ? GPIO_VALUE_OUT[index] : slv_reg1[index];
    end
  endgenerate

  
  
  reg [7:0] pipeline [1:0];

  assign GPIO_VALUE_IN = pipeline[1];
 
  always @(posedge Bus2IP_Clk) begin
    pipeline[0] <= GPIO_PAD_I;
    pipeline[1] <= pipeline[0];
  end
  
endmodule
