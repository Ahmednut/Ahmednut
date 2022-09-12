

 library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;
 use work.recplay_fifos_p.all;

-------------------------------------------------------------------------------
-- Entity Section
-------------------------------------------------------------------------------
entity rtdex_rx_fifo_interface is
	generic
	(
  	PlayBackPortWidth_g               : integer := 8;
  	NumberOfPlayBackPorts_g           : integer := 1
	);
	port
	(
     -- Mem writer controler interface
     i_ResetFifo_p : in std_logic;
     i_MemClk_p : in std_logic;
     ov256_Data_p : out std_logic_vector(255 downto 0);
     o_DValid_p	  : out std_logic;
     i_ReadData_p : in std_logic;
     o_FifoEmpty_p : out std_logic;
     
     -- Ctrl Interface        
     i_RtdexRxStarted_p     : in std_logic;  

     -- RTDEx interface
     i_RTDExClock_p : in std_logic;

     i_RxDataValid_p       : in std_logic;
     o_RxRe_p              : out std_logic;
     i_RxReady_p           : in std_logic;

     iv32_RxDataCh0_p      : in std_logic_vector(31 downto 0)
	);
end entity rtdex_rx_fifo_interface;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture rtl of rtdex_rx_fifo_interface is

    --signal FifoWrEn_s : std_logic;
    signal FifoWrEnD1_s : std_logic;
    signal FifoWrEnD2_s : std_logic;
    signal FifoWrEnD3_s : std_logic;
    signal FifoWrEnD4_s : std_logic;
    signal FifoWrEnD5_s : std_logic;
    signal FifoWrEnAndVld_s : std_logic;
    signal FifoRd_s : std_logic;
    --signal v256_Data_s : std_logic_vector(255 downto 0);
    signal AlmostFull_s : std_logic;
    signal AlmostFullD1_s : std_logic;
    signal AlmostFullD2_s : std_logic;
    signal AlmostFullD3_s : std_logic;
    signal ReadDataD1_s : std_logic;

    signal RtdexRxStartedD1_s : std_logic;
    signal RtdexRxStartedD2_s : std_logic;

    signal RxReadyD1_s : std_logic;

    signal v32_RxDataCh0D1_s :  std_logic_vector(31 downto 0);
    signal v32_RxDataCh0D2_s :  std_logic_vector(31 downto 0);
    signal v32_RxDataCh0D3_s :  std_logic_vector(31 downto 0);
    signal v32_RxDataCh0D4_s :  std_logic_vector(31 downto 0);
    signal v32_RxDataCh0D5_s :  std_logic_vector(31 downto 0);
    signal v32_FifoDin_s :  std_logic_vector(31 downto 0);
    
    signal v256_fifoData_s  : std_logic_vector(255 downto 0);
    signal fifoValid_s      : std_logic;

    -- Use to avoid unrelated LUT packing with other components
    attribute keep_hierarchy : string;
    attribute keep_hierarchy of rtl : architecture is "true";

    attribute keep : string;
    attribute keep of v32_RxDataCh0D1_s : signal is "TRUE";
    attribute keep of v32_RxDataCh0D2_s : signal is "TRUE";
    attribute keep of v32_RxDataCh0D3_s : signal is "TRUE";
    attribute keep of v32_RxDataCh0D4_s : signal is "TRUE";
    attribute keep of FifoWrEnD1_s : signal is "TRUE";
    attribute keep of FifoWrEnD2_s : signal is "TRUE";
    attribute keep of FifoWrEnD3_s : signal is "TRUE";
    attribute keep of FifoWrEnD4_s : signal is "TRUE";
    attribute keep of AlmostFullD1_s : signal is "TRUE";
    attribute keep of AlmostFullD2_s : signal is "TRUE";
    attribute keep of AlmostFullD3_s : signal is "TRUE";
    attribute keep of RxReadyD1_s : signal is "TRUE";
    attribute keep of RtdexRxStartedD1_s : signal is "TRUE";

begin


 -----------------------------------------------
 -- Register stage to improve timings
 -- Required for PCIe and big RTDEx FIFO
 -----------------------------------------------
  WriteDataFf_l : process( i_RTDExClock_p )
  begin
    if( rising_edge( i_RTDExClock_p ) ) then
      v32_RxDataCh0D1_s <= iv32_RxDataCh0_p;
      v32_RxDataCh0D2_s <= v32_RxDataCh0D1_s;
      v32_RxDataCh0D3_s <= v32_RxDataCh0D2_s;
      v32_RxDataCh0D4_s <= v32_RxDataCh0D3_s;
      v32_RxDataCh0D5_s <= v32_RxDataCh0D4_s;

      FifoWrEnD1_s <= i_RxDataValid_p;
      FifoWrEnD2_s <= FifoWrEnD1_s;
      FifoWrEnD3_s <= FifoWrEnD2_s;
      FifoWrEnD4_s <= FifoWrEnD3_s;
      FifoWrEnD5_s <= FifoWrEnD4_s;

      RtdexRxStartedD1_s <= i_RtdexRxStarted_p;
      RtdexRxStartedD2_s <= RtdexRxStartedD1_s;

      FifoRd_s <= ( not AlmostFullD3_s ) and RxReadyD1_s and RtdexRxStartedD2_s;

      AlmostFullD1_s <= AlmostFull_s;
      AlmostFullD2_s <= AlmostFullD1_s;
      AlmostFullD3_s <= AlmostFullD2_s;
      RxReadyD1_s <= i_RxReady_p;
    end if;
  end process;

  o_RxRe_p <= FifoRd_s;


 -----------------------------------------------
 -- Swap bytes ...
 -----------------------------------------------
 ByteSwap_gen: if (PlayBackPortWidth_g = 8 and NumberOfPlayBackPorts_g = 1) generate
 	v32_FifoDin_s(31 downto 24) <= v32_RxDataCh0D5_s(7 downto 0);
 	v32_FifoDin_s(23 downto 16) <= v32_RxDataCh0D5_s(15 downto 8);
 	v32_FifoDin_s(15 downto 8)  <= v32_RxDataCh0D5_s(23 downto 16);
 	v32_FifoDin_s(7 downto 0)   <= v32_RxDataCh0D5_s(31 downto 24);
 end generate ByteSwap_gen;

 HalfWordSwap_gen: if ((PlayBackPortWidth_g = 16 and NumberOfPlayBackPorts_g = 1) or (PlayBackPortWidth_g = 8 and NumberOfPlayBackPorts_g = 2)) generate
 	v32_FifoDin_s(31 downto 16) <= v32_RxDataCh0D5_s(15 downto 0);
 	v32_FifoDin_s(15 downto 0) <= v32_RxDataCh0D5_s(31 downto 16);
 end generate HalfWordSwap_gen;

 AsIs_gen: if ((PlayBackPortWidth_g = 8 and NumberOfPlayBackPorts_g > 2) or (PlayBackPortWidth_g = 16 and NumberOfPlayBackPorts_g > 1) or (PlayBackPortWidth_g > 16)) generate
 	v32_FifoDin_s <= v32_RxDataCh0D5_s;
 end generate AsIs_gen;


 -----------------------------------------------
 -- RTDEx interface asymetric FIFO
 -- Fifo Depth = 1024.
 -----------------------------------------------
 Fifo_l : fifo128_w32_r256_std_ff
 port map
 (
    rst => i_ResetFifo_p,
    wr_clk => i_RTDExClock_p,
    rd_clk => i_MemClk_p,
    din => v32_FifoDin_s,
    wr_en => FifoWrEnD5_s,
    rd_en => i_ReadData_p,
    dout => v256_fifoData_s,
    full => open,
    empty => o_FifoEmpty_p,
    valid => fifoValid_s,
    prog_full_thresh => "1000000000", -- 512
    prog_full => AlmostFull_s
 );
 
  process(i_MemClk_p)
  begin
    if rising_edge(i_MemClk_p) then
      ov256_Data_p  <= v256_fifoData_s;
      o_DValid_p    <= fifoValid_s;
    end if;
  end process;

end rtl;

