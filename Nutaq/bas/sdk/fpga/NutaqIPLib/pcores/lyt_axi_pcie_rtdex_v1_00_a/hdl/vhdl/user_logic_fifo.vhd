------------------------------------------------------------------------------
-- user_logic_fifo.vhd - entity/architecture pair
------------------------------------------------------------------------------
--
-- ***************************************************************************
-- ** Copyright (c) 1995-2011 Xilinx, Inc.  All rights reserved.            **
-- **                                                                       **
-- ** Xilinx, Inc.                                                          **
-- ** XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS"         **
-- ** AS A COURTESY TO YOU, SOLELY FOR USE IN DEVELOPING PROGRAMS AND       **
-- ** SOLUTIONS FOR XILINX DEVICES.  BY PROVIDING THIS DESIGN, CODE,        **
-- ** OR INFORMATION AS ONE POSSIBLE IMPLEMENTATION OF THIS FEATURE,        **
-- ** APPLICATION OR STANDARD, XILINX IS MAKING NO REPRESENTATION           **
-- ** THAT THIS IMPLEMENTATION IS FREE FROM ANY CLAIMS OF INFRINGEMENT,     **
-- ** AND YOU ARE RESPONSIBLE FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE      **
-- ** FOR YOUR IMPLEMENTATION.  XILINX EXPRESSLY DISCLAIMS ANY              **
-- ** WARRANTY WHATSOEVER WITH RESPECT TO THE ADEQUACY OF THE               **
-- ** IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO ANY WARRANTIES OR        **
-- ** REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE FROM CLAIMS OF       **
-- ** INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS       **
-- ** FOR A PARTICULAR PURPOSE.                                             **
-- **                                                                       **
-- ***************************************************************************
--
------------------------------------------------------------------------------
-- Filename:          user_logic_fifo.vhd
-- Version:           1.00.a
-- Description:       User logic.
-- Date:              Fri Aug 10 12:59:23 2012 (by Create and Import Peripheral Wizard)
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Naming Conventions:
--   active low signals:                    "*_n"
--   clock signals:                         "clk", "clk_div#", "clk_#x"
--   reset signals:                         "rst", "rst_n"
--   generics:                              "C_*"
--   user defined types:                    "*_TYPE"
--   state machine next state:              "*_ns"
--   state machine current state:           "*_cs"
--   combinatorial signals:                 "*_com"
--   pipelined or register delay signals:   "*_d#"
--   counter signals:                       "*cnt*"
--   clock enable signals:                  "*_ce"
--   internal version of output port:       "*_i"
--   device pins:                           "*_pin"
--   ports:                                 "- Names begin with Uppercase"
--   processes:                             "*_PROCESS"
--   component instantiations:              "<ENTITY_>I_<#|FUNC>"
------------------------------------------------------------------------------

-- DO NOT EDIT BELOW THIS LINE --------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

library lyt_axi_pcie_rtdex_v1_00_a;
use lyt_axi_pcie_rtdex_v1_00_a.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_SLV_AWIDTH                 -- Slave interface address bus width
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--   C_NUM_MEM                    -- Number of memory spaces
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Resetn                -- Bus to IP reset
--   Bus2IP_Addr                  -- Bus to IP address bus
--   Bus2IP_CS                    -- Bus to IP chip select for user logic memory selection
--   Bus2IP_RNW                   -- Bus to IP read/not write
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   Bus2IP_Burst                 -- Bus to IP burst-mode qualifier
--   Bus2IP_BurstLength           -- Bus to IP burst length
--   Bus2IP_RdReq                 -- Bus to IP read request
--   Bus2IP_WrReq                 -- Bus to IP write request
--   IP2Bus_AddrAck               -- IP to Bus address acknowledgement
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
--   Type_of_xfer                 -- Transfer Type
------------------------------------------------------------------------------

entity user_logic_fifo is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    --USER generics added here
    RTDEX_RX_NUMBER_OF_CHANNELS_g : integer range 0 to 8  := 1;
    
    -- The DEPTH parameter specify the FIFO depth in number of 4-byte samples.
    READ_DEPTH_g                  : integer               := 2048;
    
    C_RX_WIDTH_CH0                : integer := 32;
    C_RX_WIDTH_CH1                : integer := 32;
    C_RX_WIDTH_CH2                : integer := 32;
    C_RX_WIDTH_CH3                : integer := 32;
    C_RX_WIDTH_CH4                : integer := 32;
    C_RX_WIDTH_CH5                : integer := 32;
    C_RX_WIDTH_CH6                : integer := 32;
    C_RX_WIDTH_CH7                : integer := 32;
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_SLV_AWIDTH                   : integer              := 32;
    C_SLV_DWIDTH                   : integer              := 32;
    C_NUM_MEM                      : integer              := 8
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    --USER ports added here

    -- Config & status signals from/to axi registers
    --
    iv8_RxFifoReset_p               : in  std_logic_vector(7 downto 0);  -- SW Reset/Flush for a given RTDEx RX FIFO
    i_RxReset_p                     : in  std_logic;                     -- SW Reset for the whole RTDEx RX component
    iv8_RxWordCntReset_p            : in std_logic_vector(7 downto 0);
    iv8_RxFifoRdEn_p                : in std_logic_vector(7 downto 0);
    iv8_RxFifoWrEn_p                : in std_logic_vector(7 downto 0);
    ov8_overflow_p                  : out std_logic_vector(7 downto 0);
    ov8_underflow_p                 : out std_logic_vector(7 downto 0);
    ov32_RxConfigInfo_p             : out std_logic_vector(31 downto 0);
    ov32_RcvdWordCntCh0_p           : out std_logic_vector(31 downto 0);
    ov32_RcvdWordCntCh1_p           : out std_logic_vector(31 downto 0);
    ov32_RcvdWordCntCh2_p           : out std_logic_vector(31 downto 0);
    ov32_RcvdWordCntCh3_p           : out std_logic_vector(31 downto 0);
    ov32_RcvdWordCntCh4_p           : out std_logic_vector(31 downto 0);
    ov32_RcvdWordCntCh5_p           : out std_logic_vector(31 downto 0);
    ov32_RcvdWordCntCh6_p           : out std_logic_vector(31 downto 0);
    ov32_RcvdWordCntCh7_p           : out std_logic_vector(31 downto 0);

    -- FIFOs side
    --
    i_RxUserClk_p                   : in std_logic;

    o_RxReadyCh0_p                : out std_logic;
    i_RxReCh0_p                   : in std_logic;
    ov_RxDataCh0_p                : out std_logic_vector(C_RX_WIDTH_CH0-1 downto 0);
    o_RxDataValidCh0_p            : out std_logic;
    o_RxReadyCh1_p                : out std_logic;
    i_RxReCh1_p                   : in std_logic;
    ov_RxDataCh1_p                : out std_logic_vector(C_RX_WIDTH_CH1-1 downto 0);
    o_RxDataValidCh1_p            : out std_logic;
    o_RxReadyCh2_p                : out std_logic;
    i_RxReCh2_p                   : in std_logic;
    ov_RxDataCh2_p                : out std_logic_vector(C_RX_WIDTH_CH2-1 downto 0);
    o_RxDataValidCh2_p            : out std_logic;
    o_RxReadyCh3_p                : out std_logic;
    i_RxReCh3_p                   : in std_logic;
    ov_RxDataCh3_p                : out std_logic_vector(C_RX_WIDTH_CH3-1 downto 0);
    o_RxDataValidCh3_p            : out std_logic;
    o_RxReadyCh4_p                : out std_logic;
    i_RxReCh4_p                   : in std_logic;
    ov_RxDataCh4_p                : out std_logic_vector(C_RX_WIDTH_CH4-1 downto 0);
    o_RxDataValidCh4_p            : out std_logic;
    o_RxReadyCh5_p                : out std_logic;
    i_RxReCh5_p                   : in std_logic;
    ov_RxDataCh5_p                : out std_logic_vector(C_RX_WIDTH_CH5-1 downto 0);
    o_RxDataValidCh5_p            : out std_logic;
    o_RxReadyCh6_p                : out std_logic;
    i_RxReCh6_p                   : in std_logic;
    ov_RxDataCh6_p                : out std_logic_vector(C_RX_WIDTH_CH6-1 downto 0);
    o_RxDataValidCh6_p            : out std_logic;
    o_RxReadyCh7_p                : out std_logic;
    i_RxReCh7_p                   : in std_logic;
    ov_RxDataCh7_p                : out std_logic_vector(C_RX_WIDTH_CH7-1 downto 0);
    o_RxDataValidCh7_p            : out std_logic;
    -- ADD USER PORTS ABOVE THIS LINE ------------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                      : in  std_logic;
    Bus2IP_Resetn                   : in  std_logic;
    Bus2IP_Addr                     : in  std_logic_vector(C_SLV_AWIDTH-1 downto 0);
    Bus2IP_CS                       : in  std_logic_vector(C_NUM_MEM-1 downto 0);
    Bus2IP_RNW                      : in  std_logic;
    Bus2IP_Data                     : in  std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    Bus2IP_BE                       : in  std_logic_vector(C_SLV_DWIDTH/8-1 downto 0);
    Bus2IP_RdCE                     : in  std_logic_vector(C_NUM_MEM-1 downto 0);
    Bus2IP_WrCE                     : in  std_logic_vector(C_NUM_MEM-1 downto 0);
    Bus2IP_Burst                    : in  std_logic;
    Bus2IP_BurstLength              : in  std_logic_vector(7 downto 0);
    Bus2IP_RdReq                    : in  std_logic;
    Bus2IP_WrReq                    : in  std_logic;
    IP2Bus_AddrAck                  : out std_logic;
    IP2Bus_Data                     : out std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    IP2Bus_RdAck                    : out std_logic;
    IP2Bus_WrAck                    : out std_logic;
    IP2Bus_Error                    : out std_logic;
    Type_of_xfer                    : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT : string;
  attribute SIGIS : string;

  attribute SIGIS of Bus2IP_Clk     : signal is "CLK";
  attribute SIGIS of Bus2IP_Resetn  : signal is "RST";

end entity user_logic_fifo;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic_fifo is

  attribute keep_hierarchy : string;
  attribute keep_hierarchy of IMP: architecture is "true";

  ------------------------------------------------------------------
  -- Functions
  ------------------------------------------------------------------

  -- Find minimum number of bits required to represent N as an unsigned
  -- binary number.
  -- simple recursive implementation...
  --
  function log2_ceil(N: natural) return positive is
    begin
      if (N < 2) then
        return 1;
      else
        return 1 + log2_ceil(N/2);
      end if;
    end function log2_ceil;

  type array8_int_t is array(0 to 7) of integer;
  constant A8I_RX_WIDTH : array8_int_t :=
    (C_RX_WIDTH_CH0,
     C_RX_WIDTH_CH1,
     C_RX_WIDTH_CH2,
     C_RX_WIDTH_CH3,
     C_RX_WIDTH_CH4,
     C_RX_WIDTH_CH5,
     C_RX_WIDTH_CH6,
     C_RX_WIDTH_CH7);

  -- USER signal declarations added here, as needed for user logic
  --
  signal v8_chFifoRst_s         : std_logic_vector(7 downto 0);
  signal v8_almostFull_s        : std_logic_vector(7 downto 0);
  signal v8_ChEmpty_s           : std_logic_vector(7 downto 0);
  signal v8_RxChDataValid_s     : std_logic_vector(7 downto 0);
  signal v8_RxChRe_s            : std_logic_vector(7 downto 0);
  signal v8_overflow_s          : std_logic_vector(7 downto 0);
  signal v8_underflow_s         : std_logic_vector(7 downto 0);

  type array8_v256_t is array(7 downto 0) of std_logic_vector(255 downto 0);
  signal a8v256_RxChData_s      : array8_v256_t;
  signal a8v256_ChFifoDout_s    : array8_v256_t;

  signal v8_RtdexRxEna_s        : std_logic_vector(7 downto 0);
  signal v8_RxFifoRdEn_s        : std_logic_vector(7 downto 0);
  signal v8_fifoWrEn_s          : std_logic_vector(7 downto 0);

  -- 32-bit received data counters (for debug)
  --
  type FIFO_COUNTERS_TYPE is array(7 downto 0) of std_logic_vector(31 downto 0);
  signal v_fifoRxDataCounters_s : FIFO_COUNTERS_TYPE;
  signal v8_RxWordCntReset_s    : std_logic_vector(7 downto 0);

  signal v6_FifoDepthInfo_s 		: std_logic_vector(5 downto 0);

  -----------------------------------------------------------------------
  -- Signals for user logic memory interface
  -----------------------------------------------------------------------

  signal mem_read_enable        : std_logic;
  signal mem_read_ack_dly1      : std_logic;
  signal mem_read_ack_dly2      : std_logic;
  signal mem_read_ack           : std_logic;
  signal mem_write_ack          : std_logic;

  -- attribute MAX_FANOUT of v8_RxChRe_s   : signal is "5";

begin

  mem_read_enable <= ( Bus2IP_RdCE(0) or Bus2IP_RdCE(1) or Bus2IP_RdCE(2) or Bus2IP_RdCE(3) or Bus2IP_RdCE(4) or Bus2IP_RdCE(5) or Bus2IP_RdCE(6) or Bus2IP_RdCE(7) );
  mem_read_ack    <= mem_read_ack_dly1 and (not mem_read_ack_dly2);
  mem_write_ack   <= ( Bus2IP_WrCE(0) and not v8_almostFull_s(0) ) or
                     ( Bus2IP_WrCE(1) and not v8_almostFull_s(1) ) or
                     ( Bus2IP_WrCE(2) and not v8_almostFull_s(2) ) or
                     ( Bus2IP_WrCE(3) and not v8_almostFull_s(3) ) or
                     ( Bus2IP_WrCE(4) and not v8_almostFull_s(4) ) or
                     ( Bus2IP_WrCE(5) and not v8_almostFull_s(5) ) or
                     ( Bus2IP_WrCE(6) and not v8_almostFull_s(6) ) or
                     ( Bus2IP_WrCE(7) and not v8_almostFull_s(7) );

  -- This process generates the read acknowledge 1 clock after read enable
  -- is presented. Note that no reads are supported, but the acknoledge logic
  -- is there just in case.
  --
  BRAM_RD_ACK_PROC : process( Bus2IP_Clk ) is
  begin

    if ( Bus2IP_Clk'event and Bus2IP_Clk = '1' ) then
      if ( Bus2IP_Resetn = '0' ) then
        mem_read_ack_dly1 <= '0';
        mem_read_ack_dly2 <= '0';
      else
        mem_read_ack_dly1 <= mem_read_enable;
        mem_read_ack_dly2 <= mem_read_ack_dly1;
      end if;
    end if;

  end process BRAM_RD_ACK_PROC;


  -----------------------------------------------------------------------
  -- No read supported, always returns 0
  -----------------------------------------------------------------------

  IP2Bus_Data  <= (others => '0');

  IP2Bus_AddrAck <= mem_write_ack or (mem_read_enable and mem_read_ack);
  IP2Bus_WrAck <= mem_write_ack;
  IP2Bus_RdAck <= mem_read_ack;
  IP2Bus_Error <= '0';
  
  
  -----------------------------------------------------------------------
  -- RTDEx channels FIFO (up to 8)
  -----------------------------------------------------------------------

  -- Resync the write enable signal
  --
  process(Bus2IP_Clk)
  begin
   if rising_edge(Bus2IP_Clk) then
      v8_RtdexRxEna_s <= iv8_RxFifoWrEn_p;
   end if;
  end process;

  ChGen: for i in 0 to (RTDEX_RX_NUMBER_OF_CHANNELS_g - 1) generate

    v8_chFifoRst_s(i) <= i_RxReset_p or iv8_RxFifoReset_p(i);

    -- The RX channels must be enabled in order to receive data
    --
    v8_fifoWrEn_s(i) <= v8_RtdexRxEna_s(i) and Bus2IP_WrCE(i) and not v8_almostFull_s(i);

    U0_genFifo: entity lyt_axi_pcie_rtdex_v1_00_a.generic_fifo
      generic map (
        -- Always 64 bits in the AXI side
        WRITE_WIDTH_g             => 64,
        WRITE_DEPTH_g             => READ_DEPTH_g*32/64,
        -- User side match the specified parameters
        READ_WIDTH_g              => A8I_RX_WIDTH(i),
        READ_DEPTH_g              => (READ_DEPTH_g*32/A8I_RX_WIDTH(i)),
        -- Fix configuration for RTDEx RX FIFO
        FIRST_WORD_FALL_THROUGH_g => false,
        FULL_FLAGS_RST_VAL        => 0,
        USE_EMBEDDED_REG          => 1
      )
      port map (
        i_rst_p          => v8_chFifoRst_s(i),
        i_wr_clk_p       => Bus2IP_Clk,
        i_rd_clk_p       => i_RxUserClk_p,
        -- 32 bit swap required
        iv_din_p         => (Bus2IP_Data(31 downto 0) & Bus2IP_Data(63 downto 32)),
        i_wr_en_p        => v8_fifoWrEn_s(i),
        i_rd_en_p        => v8_RxChRe_s(i),
        ov_dout_p        => a8v256_ChFifoDout_s(i)(A8I_RX_WIDTH(i)-1 downto 0),
        o_full_p         => open,
        o_prog_full_p    => open,
        o_almost_full_p  => v8_almostFull_s(i),
        o_overflow_p     => v8_overflow_s(i),
        o_empty_p        => v8_ChEmpty_s(i),
        o_valid_p        => v8_RxChDataValid_s(i),
        o_underflow_p    => v8_underflow_s(i),
        o_prog_empty_p                => open,
        iv_prog_empty_thresh_p        => X"000000",
        iv_prog_full_thresh_assert_p  => std_logic_vector(to_unsigned((READ_DEPTH_g*32/64) - 4, 24)),
        iv_prog_full_thresh_negate_p  => std_logic_vector(to_unsigned((READ_DEPTH_g*32/64) - 5, 24)),
        ov_rd_data_count_p            => open,
        ov_wr_data_count_p            => open
      );
      
     WordUnpackingGen: for k in 0 to ((A8I_RX_WIDTH(i)/32)-1) generate
         a8v256_RxChData_s(i)((32*(k+1)-1) downto (32*k)) <= a8v256_ChFifoDout_s(i)((A8I_RX_WIDTH(I)-32*k-1) downto (A8I_RX_WIDTH(i)-32*(k+1)));
     end generate WordUnpackingGen;

  end generate ChGen;

  -- Default values for unconnected signals.
  --
  noChGen_l: for i in RTDEX_RX_NUMBER_OF_CHANNELS_g to 7 generate

    a8v256_RxChData_s(i)  <= (others => '0');
    v8_almostFull_s(i)    <= '0';
    v8_overflow_s(i)      <= '0';
    v8_ChEmpty_s(i)       <= '0';
    v8_RxChDataValid_s(i) <= '0';
    v8_underflow_s(i)     <= '0';

  end generate noChGen_l;


  -----------------------------------------------------------------------
  -- FIFO status held to '1' until reset
  -----------------------------------------------------------------------

  fifoStatusGen: for i in 0 to (RTDEX_RX_NUMBER_OF_CHANNELS_g - 1) generate

    process(Bus2IP_Clk)
    begin
      if rising_edge(Bus2IP_Clk) then
        if (v8_chFifoRst_s(i) = '1') then
          ov8_overflow_p(i) <= '0';
        elsif (v8_overflow_s(i) = '1') then
          ov8_overflow_p(i) <= '1';
        end if;
      end if;
    end process;

    process(Bus2IP_Clk)
    begin
      if rising_edge(Bus2IP_Clk) then
        if (v8_chFifoRst_s(i) = '1') then
          ov8_underflow_p(i) <= '0';
        elsif (v8_underflow_s(i) = '1') then
          ov8_underflow_p(i) <= '1';
        end if;
      end if;
    end process;

  end generate fifoStatusGen;

  -- Default values for unconnected signals.
  --
  noFifoStatusGen: for i in RTDEX_RX_NUMBER_OF_CHANNELS_g to 7 generate

    ov8_overflow_p(i)     <= '0';
    ov8_underflow_p(i)    <= '0';

  end generate noFifoStatusGen;


  -----------------------------------------------------------------------
  -- Channels receive data counters
  -----------------------------------------------------------------------

  -- Resync the reset signal
  --
  process(Bus2IP_Clk)
  begin
    if rising_edge(Bus2IP_Clk) then
      v8_RxWordCntReset_s <= iv8_RxWordCntReset_p;
    end if;
  end process;


  ChCnt: for i in 0 to (RTDEX_RX_NUMBER_OF_CHANNELS_g - 1) generate

    process(Bus2IP_Clk)
    begin
      if rising_edge(Bus2IP_Clk) then
        if (v8_chFifoRst_s(i) = '1' or v8_RxWordCntReset_s(i) = '1') then
          v_fifoRxDataCounters_s(i) <= (others => '0');
        elsif (v8_fifoWrEn_s(i) = '1') then
          v_fifoRxDataCounters_s(i) <= v_fifoRxDataCounters_s(i) + '1';
        end if;
      end if;
    end process;

  end generate ChCnt;

  -- Default values for unconnected fifos.
  --
  noChCnt_l: for i in RTDEX_RX_NUMBER_OF_CHANNELS_g to 7 generate

    v_fifoRxDataCounters_s(i)     <= (others => '0');

  end generate noChCnt_l;


  -----------------------------------------------------------------------
  -- FIFO data info
  -----------------------------------------------------------------------

  -- The FIFO size is WRITE_WIDTH_g * WRITE_DEPTH_g bytes,
  -- with WRITE_WIDTH_g = 8;
  -- The FIFO Depth Info is n, where 2^n = FIFO size
  --
  v6_FifoDepthInfo_s <= std_logic_vector(to_unsigned(log2_ceil(4*READ_DEPTH_g - 1),6));

  -----------------------------------------------------------------------
  -- FIFO Inputs / Output ports
  -----------------------------------------------------------------------

  -- Resync RX FIFO Read Enable
  --
  process(i_RxUserClk_p)
  begin
    if rising_edge(i_RxUserClk_p) then
      v8_RxFifoRdEn_s <= iv8_RxFifoRdEn_p;
    end if;
  end process;

  o_RxReadyCh0_p <= not(v8_ChEmpty_s(0)) and v8_RxFifoRdEn_s(0);
  o_RxReadyCh1_p <= not(v8_ChEmpty_s(1)) and v8_RxFifoRdEn_s(1);
  o_RxReadyCh2_p <= not(v8_ChEmpty_s(2)) and v8_RxFifoRdEn_s(2);
  o_RxReadyCh3_p <= not(v8_ChEmpty_s(3)) and v8_RxFifoRdEn_s(3);
  o_RxReadyCh4_p <= not(v8_ChEmpty_s(4)) and v8_RxFifoRdEn_s(4);
  o_RxReadyCh5_p <= not(v8_ChEmpty_s(5)) and v8_RxFifoRdEn_s(5);
  o_RxReadyCh6_p <= not(v8_ChEmpty_s(6)) and v8_RxFifoRdEn_s(6);
  o_RxReadyCh7_p <= not(v8_ChEmpty_s(7)) and v8_RxFifoRdEn_s(7);

  -- Removed protection to help timing.
  -- Any ways, user should not read FIFO if empty, and the FIFO appears empty when v8_RxFifoRdEn_s equals '0'.
  --
  v8_RxChRe_s(0) <= i_RxReCh0_p; -- and v8_RxFifoRdEn_s(0);
  v8_RxChRe_s(1) <= i_RxReCh1_p; -- and v8_RxFifoRdEn_s(1);
  v8_RxChRe_s(2) <= i_RxReCh2_p; -- and v8_RxFifoRdEn_s(2);
  v8_RxChRe_s(3) <= i_RxReCh3_p; -- and v8_RxFifoRdEn_s(3);
  v8_RxChRe_s(4) <= i_RxReCh4_p; -- and v8_RxFifoRdEn_s(4);
  v8_RxChRe_s(5) <= i_RxReCh5_p; -- and v8_RxFifoRdEn_s(5);
  v8_RxChRe_s(6) <= i_RxReCh6_p; -- and v8_RxFifoRdEn_s(6);
  v8_RxChRe_s(7) <= i_RxReCh7_p; -- and v8_RxFifoRdEn_s(7);

  ov_RxDataCh0_p <= a8v256_RxChData_s(0)(A8I_RX_WIDTH(0)-1 downto 0);
  ov_RxDataCh1_p <= a8v256_RxChData_s(1)(A8I_RX_WIDTH(1)-1 downto 0);
  ov_RxDataCh2_p <= a8v256_RxChData_s(2)(A8I_RX_WIDTH(2)-1 downto 0);
  ov_RxDataCh3_p <= a8v256_RxChData_s(3)(A8I_RX_WIDTH(3)-1 downto 0);
  ov_RxDataCh4_p <= a8v256_RxChData_s(4)(A8I_RX_WIDTH(4)-1 downto 0);
  ov_RxDataCh5_p <= a8v256_RxChData_s(5)(A8I_RX_WIDTH(5)-1 downto 0);
  ov_RxDataCh6_p <= a8v256_RxChData_s(6)(A8I_RX_WIDTH(6)-1 downto 0);
  ov_RxDataCh7_p <= a8v256_RxChData_s(7)(A8I_RX_WIDTH(7)-1 downto 0);

  o_RxDataValidCh0_p <= v8_RxChDataValid_s(0);
  o_RxDataValidCh1_p <= v8_RxChDataValid_s(1);
  o_RxDataValidCh2_p <= v8_RxChDataValid_s(2);
  o_RxDataValidCh3_p <= v8_RxChDataValid_s(3);
  o_RxDataValidCh4_p <= v8_RxChDataValid_s(4);
  o_RxDataValidCh5_p <= v8_RxChDataValid_s(5);
  o_RxDataValidCh6_p <= v8_RxChDataValid_s(6);
  o_RxDataValidCh7_p <= v8_RxChDataValid_s(7);

  ov32_RcvdWordCntCh0_p <= v_fifoRxDataCounters_s(0);
  ov32_RcvdWordCntCh1_p <= v_fifoRxDataCounters_s(1);
  ov32_RcvdWordCntCh2_p <= v_fifoRxDataCounters_s(2);
  ov32_RcvdWordCntCh3_p <= v_fifoRxDataCounters_s(3);
  ov32_RcvdWordCntCh4_p <= v_fifoRxDataCounters_s(4);
  ov32_RcvdWordCntCh5_p <= v_fifoRxDataCounters_s(5);
  ov32_RcvdWordCntCh6_p <= v_fifoRxDataCounters_s(6);
  ov32_RcvdWordCntCh7_p <= v_fifoRxDataCounters_s(7);

  ov32_RxConfigInfo_p <= x"00000" & "00" & v6_FifoDepthInfo_s & std_logic_vector(to_unsigned(RTDEX_RX_NUMBER_OF_CHANNELS_g, 4));

 end IMP;
