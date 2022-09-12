

 library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;
 use work.recplay_fifos_p.all;

-------------------------------------------------------------------------------
-- Entity Section
-------------------------------------------------------------------------------
entity rtdex_tx_fifo_interface is
	generic
	(
  	RecordPortWidth_g               : integer := 8;
  	NumberOfRecordPorts_g           : integer := 1
  	);
	port
	(
     -- Mem reader controler interface
     i_MemClk_p : in std_logic;
     iv256_FifoWrData_p : in std_logic_vector(255 downto 0);
     i_FifoWrite_p : in std_logic;
     i_ResetFifo_p : in std_logic;
     o_FifoProgFull_p : out std_logic;

     -- RTDEx interface
     i_RTDExClock_p    : in std_logic;
     o_TxWe_p          : out std_logic;
     i_TxReady_p       : in std_logic;
     ov32_TxDataCh0_p  : out std_logic_vector(31 downto 0)
	);
end entity rtdex_tx_fifo_interface;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture rtl of rtdex_tx_fifo_interface is

    signal v256_FifoWrDataD1_s  : std_logic_vector(255 downto 0);
    signal FifoWriteD1_s        : std_logic;
    signal v256_FifoWrDataD2_s  : std_logic_vector(255 downto 0);
    signal FifoWriteD2_s        : std_logic;

    signal ProgFull_s : std_logic;
    signal FifoRdEn_s : std_logic;
    signal v8_ReadFifoDataCnt_s : std_logic_vector(7 downto 0);
    signal v32_TxDataCh0_s : std_logic_vector(31 downto 0);
    signal Empty_s : std_logic;
    signal Valid_s : std_logic;

    -- Use to avoid unrelated LUT packing with other components
    attribute keep_hierarchy : string;
    attribute keep_hierarchy of rtl : architecture is "true";
    
    attribute keep : string;
    attribute keep of v256_FifoWrDataD1_s : signal is "true";
    attribute keep of FifoWriteD1_s : signal is "true";

begin

  --------------------------------------------------
  -- RTDEx interface asymetric FIFO
  -- Constante programmable full @ 20 (Depth=64)
  -- This is because of the DDR3 memory big latency
  --
  -- Currently prog full at 64 and depth 128
  --------------------------------------------------
  process( i_MemClk_p )
  begin
    if( rising_edge( i_MemClk_p ) ) then
      v256_FifoWrDataD1_s <= iv256_FifoWrData_p;
      FifoWriteD1_s       <= i_FifoWrite_p;
      v256_FifoWrDataD2_s <= v256_FifoWrDataD1_s;
      FifoWriteD2_s       <= FifoWriteD1_s;
    end if;
  end process;

  Fifo_l : fifo64_w256_r32_std_ff
    port map
    (
      rst => i_ResetFifo_p,
      wr_clk => i_MemClk_p,
      rd_clk => i_RTDExClock_p,
      din => v256_FifoWrDataD2_s,
      wr_en => FifoWriteD2_s,
      rd_en => FifoRdEn_s,
      dout => v32_TxDataCh0_s,
      full => open,
      empty => Empty_s,
      valid => Valid_s,
      prog_full => ProgFull_s
    );

 FifoRdEn_s <= i_TxReady_p and ( not Empty_s );

 -----------------------------------------------
 -- Output port mapping
 -----------------------------------------------
 o_FifoProgFull_p <= ProgFull_s;
 o_TxWe_p <= Valid_s;

 -----------------------------------------------
 -- Swap bytes ...
 -----------------------------------------------
 ByteSwap_gen: if (RecordPortWidth_g = 8 and NumberOfRecordPorts_g = 1) generate
 	ov32_TxDataCh0_p(31 downto 24) <= v32_TxDataCh0_s(7 downto 0);
 	ov32_TxDataCh0_p(23 downto 16) <= v32_TxDataCh0_s(15 downto 8);
 	ov32_TxDataCh0_p(15 downto 8)  <= v32_TxDataCh0_s(23 downto 16);
 	ov32_TxDataCh0_p(7 downto 0)   <= v32_TxDataCh0_s(31 downto 24);
 end generate ByteSwap_gen;

 HalfWordSwap_gen: if ((RecordPortWidth_g = 16 and NumberOfRecordPorts_g = 1) or (RecordPortWidth_g = 8 and NumberOfRecordPorts_g = 2)) generate
 	ov32_TxDataCh0_p(31 downto 16) <= v32_TxDataCh0_s(15 downto 0);
 	ov32_TxDataCh0_p(15 downto 0) <= v32_TxDataCh0_s(31 downto 16);
 end generate HalfWordSwap_gen;

 AsIs_gen: if ((RecordPortWidth_g = 8 and NumberOfRecordPorts_g > 2) or (RecordPortWidth_g = 16 and NumberOfRecordPorts_g > 1) or (RecordPortWidth_g > 16)) generate
 	ov32_TxDataCh0_p <= v32_TxDataCh0_s;
 end generate AsIs_gen;



end rtl;

