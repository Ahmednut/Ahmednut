--------------------------------------------------------------------------------
--
--    ****                              *
--   ******                            ***
--   *******                           ****
--   ********    ****  ****     **** *********    ******* ****    ***********
--   *********   ****  ****     **** *********  **************  *************
--   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
--   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
--  ****    *********  ****     ****   ****   ****       ****  ****      ****
--  ****     ********  ****    *****  ****    *****     *****  ****      ****
--  ****      ******   ***** ******   *****    ****** *******  ****** *******
--  ****        ****   ************    ******   *************   *************
--  ****         ***     ****  ****     ****      *****  ****     *****  ****
--                                                                       ****
--          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
--                                                                        ***
--
--------------------------------------------------------------------------------
-- File : generic_fifo.vhd
--------------------------------------------------------------------------------
-- Description : Generic Fifos to be generated
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log:
--
--
--------------------------------------------------------------------------------

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

use IEEE.math_real.all;

ENTITY generic_fifo IS
  generic (
    WRITE_WIDTH_g             : integer := 128;
    READ_WIDTH_g              : integer := 32;
    WRITE_DEPTH_g             : integer := 1024;
    READ_DEPTH_g              : integer := 4096;
    FIRST_WORD_FALL_THROUGH_g : boolean := false;
    FULL_FLAGS_RST_VAL        : integer := 1;
    USE_EMBEDDED_REG          : integer := 0
  );
  PORT (
    i_rst_p                   : in std_logic;
    i_wr_clk_p                : in std_logic;
    i_rd_clk_p                : in std_logic;
    iv_din_p                  : in std_logic_vector(WRITE_WIDTH_g-1 downto 0);
    i_wr_en_p                 : in std_logic;
    i_rd_en_p                 : in std_logic;
    iv_prog_empty_thresh_p          : in std_logic_vector(23 downto 0);
    iv_prog_full_thresh_assert_p    : in std_logic_vector(23 downto 0);
    iv_prog_full_thresh_negate_p    : in std_logic_vector(23 downto 0);
    ov_dout_p                 : out std_logic_vector(READ_WIDTH_g-1 downto 0);
    o_full_p                  : out std_logic;
    o_overflow_p              : out std_logic;
    o_empty_p                 : out std_logic;
    o_valid_p                 : out std_logic;
    o_underflow_p             : out std_logic;
    ov_rd_data_count_p        : out std_logic_vector(23 downto 0);
    ov_wr_data_count_p        : out std_logic_vector(23 downto 0);
    o_prog_full_p             : out std_logic;
    o_prog_empty_p            : out std_logic;
    o_almost_full_p           : out std_logic
  );
END generic_fifo;

ARCHITECTURE arch OF generic_fifo IS

  function get_data_count_width(DEPTH_WIDTH : integer; FIRST_WORD_FALL_THROUGH : boolean) return integer is
    begin
      if (FIRST_WORD_FALL_THROUGH = true) then
        return (DEPTH_WIDTH+1);
      else
        return DEPTH_WIDTH;
      end if;
    end function get_data_count_width;

  constant WRITE_DEPTH_WIDTH : integer := integer(ceil(log2(real(WRITE_DEPTH_g))));
  constant READ_DEPTH_WIDTH : integer := integer(ceil(log2(real(READ_DEPTH_g))));
  
  constant WRITE_DATA_COUNT_WIDTH : integer := get_data_count_width(WRITE_DEPTH_WIDTH, FIRST_WORD_FALL_THROUGH_g);
  constant READ_DATA_COUNT_WIDTH : integer := get_data_count_width(READ_DEPTH_WIDTH, FIRST_WORD_FALL_THROUGH_g);

  ------------------------------------------------------------------
  -- Functions
  ------------------------------------------------------------------
  
  -- Find minimum number of bits required to represent N as an unsigned
  -- binary number.
  -- simple recursive implementation...
  --
  function get_prim_fifo_type(N: natural) return string is
    begin
      if    (N = 8192) then
        return "8kx4";
      elsif (N = 4096) then
        return "4kx9";
      elsif (N = 2048) then
        return "2kx18";
      elsif (N = 1024) then
        return "1kx36";
      else
        return "4kx9";
      end if;
    end function get_prim_fifo_type;

--------------------------------------------------------
-- Declare general attributes used in this file
-- for defining each component being used with
-- the generatecore utility

attribute box_type: string;
attribute GENERATOR_DEFAULT: string;

-------------------------------------------------------

-------------------------------------------------------------------------------------
-- Start FIFO Generator Component for fifo_generator_v9_1
-- The Component declaration for fifo_generator_v9_1 pulled from the
-- Coregen version of
-- file: fifo_generator_v9_1_comp.vhd.
--
-- This component is used for both dual clock (async) and synchronous fifos
-- implemented with BRAM or distributed RAM. Hard FIFO simulation support may not
-- be provided in FIFO Generator V8.2 so not supported here.
--
-- Note: AXI ports and parameters added for this version of FIFO Generator.
--
-------------------------------------------------------------------------------------
 COMPONENT fifo_generator_v8_4
  GENERIC (
    --------------------------------------------------------------------------------
    -- Generic Declarations (verilog model ordering)
    --------------------------------------------------------------------------------
    C_COMMON_CLOCK                          : integer := 0;
    C_COUNT_TYPE                            : integer := 0;
    C_DATA_COUNT_WIDTH                      : integer := 2;
    C_DEFAULT_VALUE                         : string  := "";
    C_DIN_WIDTH                             : integer := 8;
    C_DOUT_RST_VAL                          : string  := "";
    C_DOUT_WIDTH                            : integer := 8;
    C_ENABLE_RLOCS                          : integer := 0;
    C_FAMILY                                : string  := "";
    C_FULL_FLAGS_RST_VAL                    : integer := 1;
    C_HAS_ALMOST_EMPTY                      : integer := 0;
    C_HAS_ALMOST_FULL                       : integer := 0;
    C_HAS_BACKUP                            : integer := 0;
    C_HAS_DATA_COUNT                        : integer := 0;
    C_HAS_INT_CLK                           : integer := 0;
    C_HAS_MEMINIT_FILE                      : integer := 0;
    C_HAS_OVERFLOW                          : integer := 0;
    C_HAS_RD_DATA_COUNT                     : integer := 0;
    C_HAS_RD_RST                            : integer := 0;
    C_HAS_RST                               : integer := 1;
    C_HAS_SRST                              : integer := 0;
    C_HAS_UNDERFLOW                         : integer := 0;
    C_HAS_VALID                             : integer := 0;
    C_HAS_WR_ACK                            : integer := 0;
    C_HAS_WR_DATA_COUNT                     : integer := 0;
    C_HAS_WR_RST                            : integer := 0;
    C_IMPLEMENTATION_TYPE                   : integer := 0;
    C_INIT_WR_PNTR_VAL                      : integer := 0;
    C_MEMORY_TYPE                           : integer := 1;
    C_MIF_FILE_NAME                         : string  := "";
    C_OPTIMIZATION_MODE                     : integer := 0;
    C_OVERFLOW_LOW                          : integer := 0;
    C_PRELOAD_LATENCY                       : integer := 1;
    C_PRELOAD_REGS                          : integer := 0;
    C_PRIM_FIFO_TYPE                        : string  := "4kx4";
    C_PROG_EMPTY_THRESH_ASSERT_VAL          : integer := 0;
    C_PROG_EMPTY_THRESH_NEGATE_VAL          : integer := 0;
    C_PROG_EMPTY_TYPE                       : integer := 0;
    C_PROG_FULL_THRESH_ASSERT_VAL           : integer := 0;
    C_PROG_FULL_THRESH_NEGATE_VAL           : integer := 0;
    C_PROG_FULL_TYPE                        : integer := 4;
    C_RD_DATA_COUNT_WIDTH                   : integer := 2;
    C_RD_DEPTH                              : integer := 256;
    C_RD_FREQ                               : integer := 1;
    C_RD_PNTR_WIDTH                         : integer := 8;
    C_UNDERFLOW_LOW                         : integer := 0;
    C_USE_DOUT_RST                          : integer := 0;
    C_USE_ECC                               : integer := 0;
    C_USE_EMBEDDED_REG                      : integer := 0;
    C_USE_FIFO16_FLAGS                      : integer := 0;
    C_USE_FWFT_DATA_COUNT                   : integer := 0;
    C_VALID_LOW                             : integer := 0;
    C_WR_ACK_LOW                            : integer := 0;
    C_WR_DATA_COUNT_WIDTH                   : integer := 2;
    C_WR_DEPTH                              : integer := 256;
    C_WR_FREQ                               : integer := 1;
    C_WR_PNTR_WIDTH                         : integer := 8;
    C_WR_RESPONSE_LATENCY                   : integer := 1;
    C_MSGON_VAL                             : integer := 1;
    C_ENABLE_RST_SYNC                       : integer := 1;
    C_ERROR_INJECTION_TYPE                  : integer := 0;
    C_SYNCHRONIZER_STAGE                    : integer := 2;

    -- AXI Interface related parameters start here
    C_INTERFACE_TYPE                        : integer := 0; -- 0: Native Interface; 1: AXI Interface
    C_AXI_TYPE                              : integer := 0; -- 0: AXI Stream; 1: AXI Full; 2: AXI Lite
    C_HAS_AXI_WR_CHANNEL                    : integer := 0;
    C_HAS_AXI_RD_CHANNEL                    : integer := 0;
    C_HAS_SLAVE_CE                          : integer := 0;
    C_HAS_MASTER_CE                         : integer := 0;
    C_ADD_NGC_CONSTRAINT                    : integer := 0;
    C_USE_COMMON_OVERFLOW                   : integer := 0;
    C_USE_COMMON_UNDERFLOW                  : integer := 0;
    C_USE_DEFAULT_SETTINGS                  : integer := 0;

    -- AXI Full/Lite
    C_AXI_ID_WIDTH                          : integer := 0;
    C_AXI_ADDR_WIDTH                        : integer := 0;
    C_AXI_DATA_WIDTH                        : integer := 0;
    C_HAS_AXI_AWUSER                        : integer := 0;
    C_HAS_AXI_WUSER                         : integer := 0;
    C_HAS_AXI_BUSER                         : integer := 0;
    C_HAS_AXI_ARUSER                        : integer := 0;
    C_HAS_AXI_RUSER                         : integer := 0;
    C_AXI_ARUSER_WIDTH                      : integer := 0;
    C_AXI_AWUSER_WIDTH                      : integer := 0;
    C_AXI_WUSER_WIDTH                       : integer := 0;
    C_AXI_BUSER_WIDTH                       : integer := 0;
    C_AXI_RUSER_WIDTH                       : integer := 0;

    -- AXI Streaming
    C_HAS_AXIS_TDATA                        : integer := 0;
    C_HAS_AXIS_TID                          : integer := 0;
    C_HAS_AXIS_TDEST                        : integer := 0;
    C_HAS_AXIS_TUSER                        : integer := 0;
    C_HAS_AXIS_TREADY                       : integer := 0;
    C_HAS_AXIS_TLAST                        : integer := 0;
    C_HAS_AXIS_TSTRB                        : integer := 0;
    C_HAS_AXIS_TKEEP                        : integer := 0;
    C_AXIS_TDATA_WIDTH                      : integer := 1;
    C_AXIS_TID_WIDTH                        : integer := 1;
    C_AXIS_TDEST_WIDTH                      : integer := 1;
    C_AXIS_TUSER_WIDTH                      : integer := 1;
    C_AXIS_TSTRB_WIDTH                      : integer := 1;
    C_AXIS_TKEEP_WIDTH                      : integer := 1;

    -- AXI Channel Type
    -- WACH --> Write Address Channel
    -- WDCH --> Write Data Channel
    -- WRCH --> Write Response Channel
    -- RACH --> Read Address Channel
    -- RDCH --> Read Data Channel
    -- AXIS --> AXI Streaming
    C_WACH_TYPE                             : integer := 0; -- 0 = FIFO; 1 = Register Slice; 2 = Pass Through Logic
    C_WDCH_TYPE                             : integer := 0; -- 0 = FIFO; 1 = Register Slice; 2 = Pass Through Logie
    C_WRCH_TYPE                             : integer := 0; -- 0 = FIFO; 1 = Register Slice; 2 = Pass Through Logie
    C_RACH_TYPE                             : integer := 0; -- 0 = FIFO; 1 = Register Slice; 2 = Pass Through Logie
    C_RDCH_TYPE                             : integer := 0; -- 0 = FIFO; 1 = Register Slice; 2 = Pass Through Logie
    C_AXIS_TYPE                             : integer := 0; -- 0 = FIFO; 1 = Register Slice; 2 = Pass Through Logie

    -- AXI Implementation Type
    -- 1 = Common Clock Block RAM FIFO
    -- 2 = Common Clock Distributed RAM FIFO
    -- 11 = Independent Clock Block RAM FIFO
    -- 12 = Independent Clock Distributed RAM FIFO
    C_IMPLEMENTATION_TYPE_WACH              : integer := 0;
    C_IMPLEMENTATION_TYPE_WDCH              : integer := 0;
    C_IMPLEMENTATION_TYPE_WRCH              : integer := 0;
    C_IMPLEMENTATION_TYPE_RACH              : integer := 0;
    C_IMPLEMENTATION_TYPE_RDCH              : integer := 0;
    C_IMPLEMENTATION_TYPE_AXIS              : integer := 0;

    -- AXI FIFO Type
    -- 0 = Data FIFO
    -- 1 = Packet FIFO
    -- 2 = Low Latency Data FIFO
    C_APPLICATION_TYPE_WACH                 : integer := 0;
    C_APPLICATION_TYPE_WDCH                 : integer := 0;
    C_APPLICATION_TYPE_WRCH                 : integer := 0;
    C_APPLICATION_TYPE_RACH                 : integer := 0;
    C_APPLICATION_TYPE_RDCH                 : integer := 0;
    C_APPLICATION_TYPE_AXIS                 : integer := 0;

    -- Enable ECC
    -- 0 = ECC disabled
    -- 1 = ECC enabled
    C_USE_ECC_WACH                          : integer := 0;
    C_USE_ECC_WDCH                          : integer := 0;
    C_USE_ECC_WRCH                          : integer := 0;
    C_USE_ECC_RACH                          : integer := 0;
    C_USE_ECC_RDCH                          : integer := 0;
    C_USE_ECC_AXIS                          : integer := 0;

    -- ECC Error Injection Type
    -- 0 = No Error Injection
    -- 1 = Single Bit Error Injection
    -- 2 = Double Bit Error Injection
    -- 3 = Single Bit and Double Bit Error Injection
    C_ERROR_INJECTION_TYPE_WACH             : integer := 0;
    C_ERROR_INJECTION_TYPE_WDCH             : integer := 0;
    C_ERROR_INJECTION_TYPE_WRCH             : integer := 0;
    C_ERROR_INJECTION_TYPE_RACH             : integer := 0;
    C_ERROR_INJECTION_TYPE_RDCH             : integer := 0;
    C_ERROR_INJECTION_TYPE_AXIS             : integer := 0;

    -- Input Data Width
    -- Accumulation of all AXI input signal's width
    C_DIN_WIDTH_WACH                        : integer := 1;
    C_DIN_WIDTH_WDCH                        : integer := 1;
    C_DIN_WIDTH_WRCH                        : integer := 1;
    C_DIN_WIDTH_RACH                        : integer := 1;
    C_DIN_WIDTH_RDCH                        : integer := 1;
    C_DIN_WIDTH_AXIS                        : integer := 1;

    C_WR_DEPTH_WACH                         : integer := 16;
    C_WR_DEPTH_WDCH                         : integer := 16;
    C_WR_DEPTH_WRCH                         : integer := 16;
    C_WR_DEPTH_RACH                         : integer := 16;
    C_WR_DEPTH_RDCH                         : integer := 16;
    C_WR_DEPTH_AXIS                         : integer := 16;

    C_WR_PNTR_WIDTH_WACH                    : integer := 4;
    C_WR_PNTR_WIDTH_WDCH                    : integer := 4;
    C_WR_PNTR_WIDTH_WRCH                    : integer := 4;
    C_WR_PNTR_WIDTH_RACH                    : integer := 4;
    C_WR_PNTR_WIDTH_RDCH                    : integer := 4;
    C_WR_PNTR_WIDTH_AXIS                    : integer := 4;

    C_HAS_DATA_COUNTS_WACH                  : integer := 0;
    C_HAS_DATA_COUNTS_WDCH                  : integer := 0;
    C_HAS_DATA_COUNTS_WRCH                  : integer := 0;
    C_HAS_DATA_COUNTS_RACH                  : integer := 0;
    C_HAS_DATA_COUNTS_RDCH                  : integer := 0;
    C_HAS_DATA_COUNTS_AXIS                  : integer := 0;

    C_HAS_PROG_FLAGS_WACH                   : integer := 0;
    C_HAS_PROG_FLAGS_WDCH                   : integer := 0;
    C_HAS_PROG_FLAGS_WRCH                   : integer := 0;
    C_HAS_PROG_FLAGS_RACH                   : integer := 0;
    C_HAS_PROG_FLAGS_RDCH                   : integer := 0;
    C_HAS_PROG_FLAGS_AXIS                   : integer := 0;

    C_PROG_FULL_TYPE_WACH                   : integer := 0;
    C_PROG_FULL_TYPE_WDCH                   : integer := 0;
    C_PROG_FULL_TYPE_WRCH                   : integer := 0;
    C_PROG_FULL_TYPE_RACH                   : integer := 0;
    C_PROG_FULL_TYPE_RDCH                   : integer := 0;
    C_PROG_FULL_TYPE_AXIS                   : integer := 0;
    C_PROG_FULL_THRESH_ASSERT_VAL_WACH      : integer := 0;
    C_PROG_FULL_THRESH_ASSERT_VAL_WDCH      : integer := 0;
    C_PROG_FULL_THRESH_ASSERT_VAL_WRCH      : integer := 0;
    C_PROG_FULL_THRESH_ASSERT_VAL_RACH      : integer := 0;
    C_PROG_FULL_THRESH_ASSERT_VAL_RDCH      : integer := 0;
    C_PROG_FULL_THRESH_ASSERT_VAL_AXIS      : integer := 0;

    C_PROG_EMPTY_TYPE_WACH                  : integer := 0;
    C_PROG_EMPTY_TYPE_WDCH                  : integer := 0;
    C_PROG_EMPTY_TYPE_WRCH                  : integer := 0;
    C_PROG_EMPTY_TYPE_RACH                  : integer := 0;
    C_PROG_EMPTY_TYPE_RDCH                  : integer := 0;
    C_PROG_EMPTY_TYPE_AXIS                  : integer := 0;
    C_PROG_EMPTY_THRESH_ASSERT_VAL_WACH     : integer := 0;
    C_PROG_EMPTY_THRESH_ASSERT_VAL_WDCH     : integer := 0;
    C_PROG_EMPTY_THRESH_ASSERT_VAL_WRCH     : integer := 0;
    C_PROG_EMPTY_THRESH_ASSERT_VAL_RACH     : integer := 0;
    C_PROG_EMPTY_THRESH_ASSERT_VAL_RDCH     : integer := 0;
    C_PROG_EMPTY_THRESH_ASSERT_VAL_AXIS     : integer := 0;

    C_REG_SLICE_MODE_WACH                   : integer := 0;
    C_REG_SLICE_MODE_WDCH                   : integer := 0;
    C_REG_SLICE_MODE_WRCH                   : integer := 0;
    C_REG_SLICE_MODE_RACH                   : integer := 0;
    C_REG_SLICE_MODE_RDCH                   : integer := 0;
    C_REG_SLICE_MODE_AXIS                   : integer := 0

    );


  PORT(
    rst : IN STD_LOGIC;
    wr_clk : IN STD_LOGIC;
    rd_clk : IN STD_LOGIC;
    din : IN STD_LOGIC_VECTOR(WRITE_WIDTH_g-1 DOWNTO 0);
    wr_en : IN STD_LOGIC;
    rd_en : IN STD_LOGIC;
    prog_empty_thresh : IN STD_LOGIC_VECTOR(READ_DEPTH_WIDTH-1 DOWNTO 0);
    prog_full_thresh_assert : IN STD_LOGIC_VECTOR(WRITE_DEPTH_WIDTH-1 DOWNTO 0);
    prog_full_thresh_negate : IN STD_LOGIC_VECTOR(WRITE_DEPTH_WIDTH-1 DOWNTO 0);
    dout : OUT STD_LOGIC_VECTOR(READ_WIDTH_g-1 DOWNTO 0);
    full : OUT STD_LOGIC;
    overflow : OUT STD_LOGIC;
    empty : OUT STD_LOGIC;
    valid : OUT STD_LOGIC;
    underflow : OUT STD_LOGIC;
    rd_data_count : OUT STD_LOGIC_VECTOR(READ_DATA_COUNT_WIDTH-1 DOWNTO 0);
    wr_data_count : OUT STD_LOGIC_VECTOR(WRITE_DATA_COUNT_WIDTH-1 DOWNTO 0);
    prog_full : OUT STD_LOGIC;
    prog_empty : OUT STD_LOGIC;
    almost_full: OUT STD_LOGIC
    );
 END COMPONENT;


-- The following attributes tells XST that the fifo_generator_v8_3 is a black box
-- which should be generated using the command given by the value
-- of this attribute
attribute box_type of fifo_generator_v8_4: component is "black_box";
attribute GENERATOR_DEFAULT of fifo_generator_v8_4: component is
     "generatecore com.xilinx.ip.fifo_generator_v8_4.fifo_generator_v8_4 -a map_qvirtex_to=virtex map_qrvirtex_to=virtex map_virtexe_to=virtex map_qvirtex2_to=virtex2 map_qrvirtex2_to=virtex2 map_spartan2_to=virtex map_spartan2e_to=virtex map_virtex5_to=virtex4 map_spartan3a_to=spartan3e spartan3an_to=spartan3e spartan3adsp_to=spartan3e ";

-- End FIFO Generator Component ---------------------------------------

signal v_rd_data_count_s                          : std_logic_vector(READ_DATA_COUNT_WIDTH-1 downto 0) :=(others => '0');
signal v_wr_data_count_s                          : std_logic_vector(WRITE_DATA_COUNT_WIDTH-1 downto 0) :=(others => '0');

attribute keep: boolean;
attribute keep of ov_rd_data_count_p: signal is true;
attribute keep of ov_wr_data_count_p: signal is true;
  
BEGIN

  --------------------------------------------------------------------------------
  -- Generic parameters validation
  --------------------------------------------------------------------------------

  -- FIFO depht validation
  --

  assert (WRITE_DEPTH_g = 512) or
         (WRITE_DEPTH_g = 1024) or
         (WRITE_DEPTH_g = 2048) or
         (WRITE_DEPTH_g = 4096) or
         (WRITE_DEPTH_g = 8192) or
         (WRITE_DEPTH_g = 16384) or
         (WRITE_DEPTH_g = 32768) or
         (WRITE_DEPTH_g = 65536) or
         (WRITE_DEPTH_g = 131072) or
         (WRITE_DEPTH_g = 262144) or
         (WRITE_DEPTH_g = 524288) or
         (WRITE_DEPTH_g = 1048576)
  report "invalid FIFO size (write depth)"
  severity failure;

  assert (READ_DEPTH_g = 512) or
         (READ_DEPTH_g = 1024) or
         (READ_DEPTH_g = 2048) or
         (READ_DEPTH_g = 4096) or
         (READ_DEPTH_g = 8192) or
         (READ_DEPTH_g = 16384) or
         (READ_DEPTH_g = 32768) or
         (READ_DEPTH_g = 65536) or
         (READ_DEPTH_g = 131072) or
         (READ_DEPTH_g = 262144) or
         (READ_DEPTH_g = 524288) or
         (READ_DEPTH_g = 1048576)
  report "invalid FIFO size (read depth)"
  severity failure;

  -- FIFO width validation
  --

  assert  ((WRITE_WIDTH_g =  32) and (READ_WIDTH_g =  32)) or
          ((WRITE_WIDTH_g =  64) and (READ_WIDTH_g =  32)) or
          ((WRITE_WIDTH_g =  32) and (READ_WIDTH_g =  64)) or
          ((WRITE_WIDTH_g =  32) and (READ_WIDTH_g = 128)) or
          ((WRITE_WIDTH_g =  64) and (READ_WIDTH_g = 128)) or
          ((WRITE_WIDTH_g = 128) and (READ_WIDTH_g = 128)) or
          ((WRITE_WIDTH_g = 128) and (READ_WIDTH_g =  32)) or
          ((WRITE_WIDTH_g = 128) and (READ_WIDTH_g =  64)) or
          ((WRITE_WIDTH_g =  32) and (READ_WIDTH_g = 256)) or
          ((WRITE_WIDTH_g =  64) and (READ_WIDTH_g = 256)) or
          ((WRITE_WIDTH_g = 128) and (READ_WIDTH_g = 256)) or
          ((WRITE_WIDTH_g = 256) and (READ_WIDTH_g = 256)) or
          ((WRITE_WIDTH_g = 256) and (READ_WIDTH_g =  32)) or
          ((WRITE_WIDTH_g = 256) and (READ_WIDTH_g =  64)) or
          ((WRITE_WIDTH_g = 256) and (READ_WIDTH_g = 128))
  report "invalid FIFO write/read port widths"
  severity failure;

  ov_rd_data_count_p(23 downto READ_DATA_COUNT_WIDTH)    <= (others => '0');
  ov_wr_data_count_p(23 downto WRITE_DATA_COUNT_WIDTH)   <= (others => '0');
  ov_rd_data_count_p(READ_DATA_COUNT_WIDTH-1 downto 0)   <= v_rd_data_count_s;
  ov_wr_data_count_p(WRITE_DATA_COUNT_WIDTH-1 downto 0)  <= v_wr_data_count_s;

Generate_Normal : if FIRST_WORD_FALL_THROUGH_g = FALSE generate
  u_Normal : fifo_generator_v8_4
    GENERIC MAP (
      c_add_ngc_constraint => 0,
      c_application_type_axis => 0,
      c_application_type_rach => 0,
      c_application_type_rdch => 0,
      c_application_type_wach => 0,
      c_application_type_wdch => 0,
      c_application_type_wrch => 0,
      c_axi_addr_width => 32,
      c_axi_aruser_width => 1,
      c_axi_awuser_width => 1,
      c_axi_buser_width => 1,
      c_axi_data_width => 64,
      c_axi_id_width => 4,
      c_axi_ruser_width => 1,
      c_axi_type => 0,
      c_axi_wuser_width => 1,
      c_axis_tdata_width => 64,
      c_axis_tdest_width => 4,
      c_axis_tid_width => 8,
      c_axis_tkeep_width => 4,
      c_axis_tstrb_width => 4,
      c_axis_tuser_width => 4,
      c_axis_type => 0,
      c_common_clock => 0,
      c_count_type => 0,
      c_data_count_width => WRITE_DEPTH_WIDTH,
      c_default_value => "BlankString",
      c_din_width => WRITE_WIDTH_g,
      c_din_width_axis => 1,
      c_din_width_rach => 32,
      c_din_width_rdch => 64,
      c_din_width_wach => 32,
      c_din_width_wdch => 64,
      c_din_width_wrch => 2,
      c_dout_rst_val => "0",
      c_dout_width => READ_WIDTH_g,
      c_enable_rlocs => 0,
      c_enable_rst_sync => 1,
      c_error_injection_type => 0,
      c_error_injection_type_axis => 0,
      c_error_injection_type_rach => 0,
      c_error_injection_type_rdch => 0,
      c_error_injection_type_wach => 0,
      c_error_injection_type_wdch => 0,
      c_error_injection_type_wrch => 0,
      c_family => "virtex6",
      c_full_flags_rst_val => FULL_FLAGS_RST_VAL,
      c_has_almost_empty => 0,
      c_has_almost_full => 1,
      c_has_axi_aruser => 0,
      c_has_axi_awuser => 0,
      c_has_axi_buser => 0,
      c_has_axi_rd_channel => 0,
      c_has_axi_ruser => 0,
      c_has_axi_wr_channel => 0,
      c_has_axi_wuser => 0,
      c_has_axis_tdata => 0,
      c_has_axis_tdest => 0,
      c_has_axis_tid => 0,
      c_has_axis_tkeep => 0,
      c_has_axis_tlast => 0,
      c_has_axis_tready => 1,
      c_has_axis_tstrb => 0,
      c_has_axis_tuser => 0,
      c_has_backup => 0,
      c_has_data_count => 0,
      c_has_data_counts_axis => 0,
      c_has_data_counts_rach => 0,
      c_has_data_counts_rdch => 0,
      c_has_data_counts_wach => 0,
      c_has_data_counts_wdch => 0,
      c_has_data_counts_wrch => 0,
      c_has_int_clk => 0,
      c_has_master_ce => 0,
      c_has_meminit_file => 0,
      c_has_overflow => 1,
      c_has_prog_flags_axis => 0,
      c_has_prog_flags_rach => 0,
      c_has_prog_flags_rdch => 0,
      c_has_prog_flags_wach => 0,
      c_has_prog_flags_wdch => 0,
      c_has_prog_flags_wrch => 0,
      c_has_rd_data_count => 1,
      c_has_rd_rst => 0,
      c_has_rst => 1,
      c_has_slave_ce => 0,
      c_has_srst => 0,
      c_has_underflow => 1,
      c_has_valid => 1,
      c_has_wr_ack => 0,
      c_has_wr_data_count => 1,
      c_has_wr_rst => 0,
      c_implementation_type => 2,
      c_implementation_type_axis => 1,
      c_implementation_type_rach => 1,
      c_implementation_type_rdch => 1,
      c_implementation_type_wach => 1,
      c_implementation_type_wdch => 1,
      c_implementation_type_wrch => 1,
      c_init_wr_pntr_val => 0,
      c_interface_type => 0,
      c_memory_type => 1,
      c_mif_file_name => "BlankString",
      c_msgon_val => 1,
      c_optimization_mode => 0,
      c_overflow_low => 0,
      c_preload_latency => (USE_EMBEDDED_REG + 1),
      c_preload_regs => USE_EMBEDDED_REG,
      c_prim_fifo_type => get_prim_fifo_type(WRITE_DEPTH_g),
      c_prog_empty_thresh_assert_val => 2,
      c_prog_empty_thresh_assert_val_axis => 1022,
      c_prog_empty_thresh_assert_val_rach => 1022,
      c_prog_empty_thresh_assert_val_rdch => 1022,
      c_prog_empty_thresh_assert_val_wach => 1022,
      c_prog_empty_thresh_assert_val_wdch => 1022,
      c_prog_empty_thresh_assert_val_wrch => 1022,
      c_prog_empty_thresh_negate_val => 3,
      c_prog_empty_type => 3,
      c_prog_empty_type_axis => 5,
      c_prog_empty_type_rach => 5,
      c_prog_empty_type_rdch => 5,
      c_prog_empty_type_wach => 5,
      c_prog_empty_type_wdch => 5,
      c_prog_empty_type_wrch => 5,
      c_prog_full_thresh_assert_val => WRITE_DEPTH_g-3,
      c_prog_full_thresh_assert_val_axis => 1023,
      c_prog_full_thresh_assert_val_rach => 1023,
      c_prog_full_thresh_assert_val_rdch => 1023,
      c_prog_full_thresh_assert_val_wach => 1023,
      c_prog_full_thresh_assert_val_wdch => 1023,
      c_prog_full_thresh_assert_val_wrch => 1023,
      c_prog_full_thresh_negate_val => WRITE_DEPTH_g-4,
      c_prog_full_type => 4,
      c_prog_full_type_axis => 5,
      c_prog_full_type_rach => 5,
      c_prog_full_type_rdch => 5,
      c_prog_full_type_wach => 5,
      c_prog_full_type_wdch => 5,
      c_prog_full_type_wrch => 5,
      c_rach_type => 0,
      c_rd_data_count_width => READ_DATA_COUNT_WIDTH,
      c_rd_depth => READ_DEPTH_g,
      c_rd_freq => 1,
      c_rd_pntr_width => READ_DEPTH_WIDTH,
      c_rdch_type => 0,
      c_reg_slice_mode_axis => 0,
      c_reg_slice_mode_rach => 0,
      c_reg_slice_mode_rdch => 0,
      c_reg_slice_mode_wach => 0,
      c_reg_slice_mode_wdch => 0,
      c_reg_slice_mode_wrch => 0,
      c_synchronizer_stage => 2,
      c_underflow_low => 0,
      c_use_common_overflow => 0,
      c_use_common_underflow => 0,
      c_use_default_settings => 0,
      c_use_dout_rst => 1,
      c_use_ecc => 0,
      c_use_ecc_axis => 0,
      c_use_ecc_rach => 0,
      c_use_ecc_rdch => 0,
      c_use_ecc_wach => 0,
      c_use_ecc_wdch => 0,
      c_use_ecc_wrch => 0,
      c_use_embedded_reg => USE_EMBEDDED_REG,
      c_use_fifo16_flags => 0,
      c_use_fwft_data_count => 0,
      c_valid_low => 0,
      c_wach_type => 0,
      c_wdch_type => 0,
      c_wr_ack_low => 0,
      c_wr_data_count_width => WRITE_DATA_COUNT_WIDTH,
      c_wr_depth => WRITE_DEPTH_g,
      c_wr_depth_axis => 1024,
      c_wr_depth_rach => 16,
      c_wr_depth_rdch => 1024,
      c_wr_depth_wach => 16,
      c_wr_depth_wdch => 1024,
      c_wr_depth_wrch => 16,
      c_wr_freq => 1,
      c_wr_pntr_width => WRITE_DEPTH_WIDTH,
      c_wr_pntr_width_axis => 10,
      c_wr_pntr_width_rach => 4,
      c_wr_pntr_width_rdch => 10,
      c_wr_pntr_width_wach => 4,
      c_wr_pntr_width_wdch => 10,
      c_wr_pntr_width_wrch => 4,
      c_wr_response_latency => 1,
      c_wrch_type => 0
    )
    PORT MAP (
    rst               => i_rst_p               ,
    wr_clk            => i_wr_clk_p            ,
    rd_clk            => i_rd_clk_p            ,
    din               => iv_din_p              ,
    wr_en             => i_wr_en_p             ,
    rd_en             => i_rd_en_p             ,
    prog_empty_thresh => iv_prog_empty_thresh_p(READ_DEPTH_WIDTH-1 DOWNTO 0),
    prog_full_thresh_assert => iv_prog_full_thresh_assert_p(WRITE_DEPTH_WIDTH-1 DOWNTO 0),
    prog_full_thresh_negate => iv_prog_full_thresh_negate_p(WRITE_DEPTH_WIDTH-1 DOWNTO 0),
    dout              => ov_dout_p             ,
    full              => o_full_p              ,
    overflow          => o_overflow_p          ,
    empty             => o_empty_p             ,
    valid             => o_valid_p             ,
    underflow         => o_underflow_p         ,
    rd_data_count     => v_rd_data_count_s     ,
    wr_data_count     => v_wr_data_count_s     ,
    prog_full         => o_prog_full_p         ,
    prog_empty        => o_prog_empty_p        ,
    almost_full       => o_almost_full_p
    );
end generate Generate_Normal;

Generate_FWFT : if FIRST_WORD_FALL_THROUGH_g = TRUE generate
    u_FWFT : fifo_generator_v8_4
    GENERIC MAP (
      c_add_ngc_constraint => 0,
      c_application_type_axis => 0,
      c_application_type_rach => 0,
      c_application_type_rdch => 0,
      c_application_type_wach => 0,
      c_application_type_wdch => 0,
      c_application_type_wrch => 0,
      c_axi_addr_width => 32,
      c_axi_aruser_width => 1,
      c_axi_awuser_width => 1,
      c_axi_buser_width => 1,
      c_axi_data_width => 64,
      c_axi_id_width => 4,
      c_axi_ruser_width => 1,
      c_axi_type => 0,
      c_axi_wuser_width => 1,
      c_axis_tdata_width => 64,
      c_axis_tdest_width => 4,
      c_axis_tid_width => 8,
      c_axis_tkeep_width => 4,
      c_axis_tstrb_width => 4,
      c_axis_tuser_width => 4,
      c_axis_type => 0,
      c_common_clock => 0,
      c_count_type => 0,
      c_data_count_width => WRITE_DEPTH_WIDTH,
      c_default_value => "BlankString",
      c_din_width => WRITE_WIDTH_g,
      c_din_width_axis => 1,
      c_din_width_rach => 32,
      c_din_width_rdch => 64,
      c_din_width_wach => 32,
      c_din_width_wdch => 64,
      c_din_width_wrch => 2,
      c_dout_rst_val => "0",
      c_dout_width => READ_WIDTH_g,
      c_enable_rlocs => 0,
      c_enable_rst_sync => 1,
      c_error_injection_type => 0,
      c_error_injection_type_axis => 0,
      c_error_injection_type_rach => 0,
      c_error_injection_type_rdch => 0,
      c_error_injection_type_wach => 0,
      c_error_injection_type_wdch => 0,
      c_error_injection_type_wrch => 0,
      c_family => "virtex6",
      c_full_flags_rst_val => FULL_FLAGS_RST_VAL,
      c_has_almost_empty => 0,
      c_has_almost_full => 1,
      c_has_axi_aruser => 0,
      c_has_axi_awuser => 0,
      c_has_axi_buser => 0,
      c_has_axi_rd_channel => 0,
      c_has_axi_ruser => 0,
      c_has_axi_wr_channel => 0,
      c_has_axi_wuser => 0,
      c_has_axis_tdata => 0,
      c_has_axis_tdest => 0,
      c_has_axis_tid => 0,
      c_has_axis_tkeep => 0,
      c_has_axis_tlast => 0,
      c_has_axis_tready => 1,
      c_has_axis_tstrb => 0,
      c_has_axis_tuser => 0,
      c_has_backup => 0,
      c_has_data_count => 0,
      c_has_data_counts_axis => 0,
      c_has_data_counts_rach => 0,
      c_has_data_counts_rdch => 0,
      c_has_data_counts_wach => 0,
      c_has_data_counts_wdch => 0,
      c_has_data_counts_wrch => 0,
      c_has_int_clk => 0,
      c_has_master_ce => 0,
      c_has_meminit_file => 0,
      c_has_overflow => 1,
      c_has_prog_flags_axis => 0,
      c_has_prog_flags_rach => 0,
      c_has_prog_flags_rdch => 0,
      c_has_prog_flags_wach => 0,
      c_has_prog_flags_wdch => 0,
      c_has_prog_flags_wrch => 0,
      c_has_rd_data_count => 1,
      c_has_rd_rst => 0,
      c_has_rst => 1,
      c_has_slave_ce => 0,
      c_has_srst => 0,
      c_has_underflow => 1,
      c_has_valid => 1,
      c_has_wr_ack => 0,
      c_has_wr_data_count => 1,
      c_has_wr_rst => 0,
      c_implementation_type => 2,
      c_implementation_type_axis => 1,
      c_implementation_type_rach => 1,
      c_implementation_type_rdch => 1,
      c_implementation_type_wach => 1,
      c_implementation_type_wdch => 1,
      c_implementation_type_wrch => 1,
      c_init_wr_pntr_val => 0,
      c_interface_type => 0,
      c_memory_type => 1,
      c_mif_file_name => "BlankString",
      c_msgon_val => 1,
      c_optimization_mode => 0,
      c_overflow_low => 0,
      c_preload_latency => 0,
      c_preload_regs => 1,
      c_prim_fifo_type => get_prim_fifo_type(WRITE_DEPTH_g),
      c_prog_empty_thresh_assert_val => 4,
      c_prog_empty_thresh_assert_val_axis => 1022,
      c_prog_empty_thresh_assert_val_rach => 1022,
      c_prog_empty_thresh_assert_val_rdch => 1022,
      c_prog_empty_thresh_assert_val_wach => 1022,
      c_prog_empty_thresh_assert_val_wdch => 1022,
      c_prog_empty_thresh_assert_val_wrch => 1022,
      c_prog_empty_thresh_negate_val => 5,
      c_prog_empty_type => 3,
      c_prog_empty_type_axis => 5,
      c_prog_empty_type_rach => 5,
      c_prog_empty_type_rdch => 5,
      c_prog_empty_type_wach => 5,
      c_prog_empty_type_wdch => 5,
      c_prog_empty_type_wrch => 5,
      c_prog_full_thresh_assert_val => WRITE_DEPTH_g-1,
      c_prog_full_thresh_assert_val_axis => 1023,
      c_prog_full_thresh_assert_val_rach => 1023,
      c_prog_full_thresh_assert_val_rdch => 1023,
      c_prog_full_thresh_assert_val_wach => 1023,
      c_prog_full_thresh_assert_val_wdch => 1023,
      c_prog_full_thresh_assert_val_wrch => 1023,
      c_prog_full_thresh_negate_val => WRITE_DEPTH_g-2,
      c_prog_full_type => 4,
      c_prog_full_type_axis => 5,
      c_prog_full_type_rach => 5,
      c_prog_full_type_rdch => 5,
      c_prog_full_type_wach => 5,
      c_prog_full_type_wdch => 5,
      c_prog_full_type_wrch => 5,
      c_rach_type => 0,
      c_rd_data_count_width => READ_DATA_COUNT_WIDTH,
      c_rd_depth => READ_DEPTH_g,
      c_rd_freq => 1,
      c_rd_pntr_width => READ_DEPTH_WIDTH,
      c_rdch_type => 0,
      c_reg_slice_mode_axis => 0,
      c_reg_slice_mode_rach => 0,
      c_reg_slice_mode_rdch => 0,
      c_reg_slice_mode_wach => 0,
      c_reg_slice_mode_wdch => 0,
      c_reg_slice_mode_wrch => 0,
      c_synchronizer_stage => 2,
      c_underflow_low => 0,
      c_use_common_overflow => 0,
      c_use_common_underflow => 0,
      c_use_default_settings => 0,
      c_use_dout_rst => 1,
      c_use_ecc => 0,
      c_use_ecc_axis => 0,
      c_use_ecc_rach => 0,
      c_use_ecc_rdch => 0,
      c_use_ecc_wach => 0,
      c_use_ecc_wdch => 0,
      c_use_ecc_wrch => 0,
      c_use_embedded_reg => USE_EMBEDDED_REG,
      c_use_fifo16_flags => 0,
      c_use_fwft_data_count => 1,
      c_valid_low => 0,
      c_wach_type => 0,
      c_wdch_type => 0,
      c_wr_ack_low => 0,
      c_wr_data_count_width => WRITE_DATA_COUNT_WIDTH,
      c_wr_depth => WRITE_DEPTH_g,
      c_wr_depth_axis => 1024,
      c_wr_depth_rach => 16,
      c_wr_depth_rdch => 1024,
      c_wr_depth_wach => 16,
      c_wr_depth_wdch => 1024,
      c_wr_depth_wrch => 16,
      c_wr_freq => 1,
      c_wr_pntr_width => WRITE_DEPTH_WIDTH,
      c_wr_pntr_width_axis => 10,
      c_wr_pntr_width_rach => 4,
      c_wr_pntr_width_rdch => 10,
      c_wr_pntr_width_wach => 4,
      c_wr_pntr_width_wdch => 10,
      c_wr_pntr_width_wrch => 4,
      c_wr_response_latency => 1,
      c_wrch_type => 0
    )
    PORT MAP (
    rst               => i_rst_p               ,
    wr_clk            => i_wr_clk_p            ,
    rd_clk            => i_rd_clk_p            ,
    din               => iv_din_p              ,
    wr_en             => i_wr_en_p             ,
    rd_en             => i_rd_en_p             ,
    prog_empty_thresh => iv_prog_empty_thresh_p(READ_DEPTH_WIDTH-1 DOWNTO 0),
    prog_full_thresh_assert => iv_prog_full_thresh_assert_p(WRITE_DEPTH_WIDTH-1 DOWNTO 0),
    prog_full_thresh_negate => iv_prog_full_thresh_negate_p(WRITE_DEPTH_WIDTH-1 DOWNTO 0),
    dout              => ov_dout_p             ,
    full              => o_full_p              ,
    overflow          => o_overflow_p          ,
    empty             => o_empty_p             ,
    valid             => o_valid_p             ,
    underflow         => o_underflow_p         ,
    rd_data_count     => v_rd_data_count_s     ,
    wr_data_count     => v_wr_data_count_s     ,
    prog_full         => o_prog_full_p         ,
    prog_empty        => o_prog_empty_p        ,
    almost_full       => o_almost_full_p
    );
end generate Generate_FWFT;


END arch;
