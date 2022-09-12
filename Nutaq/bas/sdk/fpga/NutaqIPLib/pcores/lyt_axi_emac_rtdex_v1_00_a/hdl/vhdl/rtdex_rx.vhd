--------------------------------------------------------------------------------
--
--
--          **  **     **  ******  ********  ********  ********  **    **
--         **    **   **  **   ** ********  ********  ********  **    **
--        **     *****   **   **    **     **        **        **    **
--       **       **    ******     **     ****      **        ********
--      **       **    **  **     **     **        **        **    **
--     *******  **    **   **    **     ********  ********  **    **
--    *******  **    **    **   **     ********  ********  **    **
--
--                       L Y R T E C H   R D   I N C
--
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- Notes / Assumptions :
-- Description: RDTEx Rx inferface
-- Khalid Bensadek
-- 2012/05
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log:
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

library lyt_axi_emac_rtdex_v1_00_a;
use lyt_axi_emac_rtdex_v1_00_a.rtdex_defines.all;
use lyt_axi_emac_rtdex_v1_00_a.rtdex_pkg.all;

entity rtdex_rx is
   generic(
          NumberChannels_g 		: integer range 1 to 8 := 1;
          
          -- The DEPTH parameters specify the FIFO depth in number of 4-byte samples.
          C_RXMEM               : integer := 4096;
          
          C_RX_WIDTH_CH0        : integer := 32;
          C_RX_WIDTH_CH1        : integer := 32;
          C_RX_WIDTH_CH2        : integer := 32;
          C_RX_WIDTH_CH3        : integer := 32;
          C_RX_WIDTH_CH4        : integer := 32;
          C_RX_WIDTH_CH5        : integer := 32;
          C_RX_WIDTH_CH6        : integer := 32;
          C_RX_WIDTH_CH7        : integer := 32;
          StatsCntrs_g		  	: integer range 0 to 1 := 1;
          FlowCtrlEn_g			: integer range 0 to 1 := 0;
          big_Endian_g			: integer range 0 to 1 := 0
          );
   port(
         -- Config & status signals from/to axi registers --
		 iv8_RxStartNewTransfer_p   : in  std_logic_vector(7 downto 0);
		 ov32_RxBadFrameCnt_p       : out std_logic_vector(31 downto 0);
		 ov32_RxFrameLostCntCh0_p   : out std_logic_vector(31 downto 0);
         ov32_RxFrameLostCntCh1_p   : out std_logic_vector(31 downto 0);
         ov32_RxFrameLostCntCh2_p   : out std_logic_vector(31 downto 0);
         ov32_RxFrameLostCntCh3_p   : out std_logic_vector(31 downto 0);
         ov32_RxFrameLostCntCh4_p   : out std_logic_vector(31 downto 0);
         ov32_RxFrameLostCntCh5_p   : out std_logic_vector(31 downto 0);
         ov32_RxFrameLostCntCh6_p   : out std_logic_vector(31 downto 0);
         ov32_RxFrameLostCntCh7_p   : out std_logic_vector(31 downto 0);
         iv8_RxFifoReset_p          : in  std_logic_vector(7 downto 0);  -- SW Reset/Flush for a given RTDEx RX FIFO
         i_RxReset_p                : in  std_logic;                     -- SW Reset for the whole RTDEx RX component
         ov32_RxConfigInfo_p        : out std_logic_vector(31 downto 0);
         iv32_RxTimeout2DropFrm_p	: in std_logic_vector(31 downto 0);
         ov32_RcvdFrameCntCh0_p		: out std_logic_vector(31 downto 0);
		 ov32_RcvdFrameCntCh1_p		: out std_logic_vector(31 downto 0);
		 ov32_RcvdFrameCntCh2_p		: out std_logic_vector(31 downto 0);
		 ov32_RcvdFrameCntCh3_p		: out std_logic_vector(31 downto 0);
		 ov32_RcvdFrameCntCh4_p		: out std_logic_vector(31 downto 0);
		 ov32_RcvdFrameCntCh5_p		: out std_logic_vector(31 downto 0);
		 ov32_RcvdFrameCntCh6_p		: out std_logic_vector(31 downto 0);
		 ov32_RcvdFrameCntCh7_p		: out std_logic_vector(31 downto 0);
		 ov32_RxDropdFrmsCh0_p 		: out std_logic_vector(31 downto 0);
		 ov32_RxDropdFrmsCh1_p 		: out std_logic_vector(31 downto 0);
		 ov32_RxDropdFrmsCh2_p 		: out std_logic_vector(31 downto 0);
		 ov32_RxDropdFrmsCh3_p 		: out std_logic_vector(31 downto 0);
		 ov32_RxDropdFrmsCh4_p 		: out std_logic_vector(31 downto 0);
		 ov32_RxDropdFrmsCh5_p 		: out std_logic_vector(31 downto 0);
		 ov32_RxDropdFrmsCh6_p 		: out std_logic_vector(31 downto 0);
		 ov32_RxDropdFrmsCh7_p 		: out std_logic_vector(31 downto 0);
		 ov3_RxErrStatus_p			: out std_logic_vector(2 downto 0);
		 iv32_RxFifoFullAssertThrCh0_p	: in std_logic_vector(31 downto 0);
		 iv32_RxFifoFullNegateThrCh0_p	: in std_logic_vector(31 downto 0);
		 ov8_RxFifoUnderrun_p		: out std_logic_vector(7 downto 0);


         --- AXI STR side ---
         RTDEX_RXD_ACLK       : in  std_logic;                          --  AXI-Stream Receive Data Clk
         RTDEX_RXD_ARESET     : in  std_logic;                          --  AXI-Stream Receive Data Reset
         RTDEX_RXD_TVALID     : in std_logic;                           --  AXI-Stream Receive Data Valid
         RTDEX_RXD_TREADY     : out  std_logic;                         --  AXI-Stream Receive Data Ready
         RTDEX_RXD_TLAST      : in std_logic;                           --  AXI-Stream Receive Data Last
         RTDEX_RXD_TKEEP      : in std_logic_vector(3 downto 0);        --  AXI-Stream Receive Data Keep
         V32_RTDEX_RXD_TDATA  : in std_logic_vector(31 downto 0);       --  AXI-Stream Receive Data Data

         RTDEX_RXS_ACLK       : in  std_logic;                          --  AXI-Stream Receive Status Clk
         RTDEX_RXS_ARESET     : in  std_logic;                          --  AXI-Stream Receive Status Reset
         RTDEX_RXS_TVALID     : in std_logic;                           --  AXI-Stream Receive Status Valid
         RTDEX_RXS_TREADY     : out  std_logic;                         --  AXI-Stream Receive Status Ready
         RTDEX_RXS_TLAST      : in std_logic;                           --  AXI-Stream Receive Status Last
         RTDEX_RXS_TKEEP      : in std_logic_vector(3 downto 0);        --  AXI-Stream Receive Status Keep
         V32_RTDEX_RXS_TDATA  : in std_logic_vector(31 downto 0);       --  AXI-Stream Receive Status Data

         --
         o_SendPause_p		  : out std_logic;
		 ov16_PauseVal_p	  : out std_logic_vector(15 downto 0);

         --- User side ---
         i_RxUserClk_p        : in std_logic;
         o_RxReadyCh0_p       : out std_logic;
         i_RxReCh0_p          : in std_logic;
         ov_RxDataCh0_p       : out std_logic_vector(C_RX_WIDTH_CH0-1 downto 0);
         o_RxDataValidCh0_p   : out std_logic;
         o_RxReadyCh1_p       : out std_logic;
         i_RxReCh1_p          : in std_logic;
         ov_RxDataCh1_p       : out std_logic_vector(C_RX_WIDTH_CH1-1 downto 0);
         o_RxDataValidCh1_p   : out std_logic;
         o_RxReadyCh2_p       : out std_logic;
         i_RxReCh2_p          : in std_logic;
         ov_RxDataCh2_p       : out std_logic_vector(C_RX_WIDTH_CH2-1 downto 0);
         o_RxDataValidCh2_p   : out std_logic;
         o_RxReadyCh3_p       : out std_logic;
         i_RxReCh3_p          : in std_logic;
         ov_RxDataCh3_p       : out std_logic_vector(C_RX_WIDTH_CH3-1 downto 0);
         o_RxDataValidCh3_p   : out std_logic;
         o_RxReadyCh4_p       : out std_logic;
         i_RxReCh4_p          : in std_logic;
         ov_RxDataCh4_p       : out std_logic_vector(C_RX_WIDTH_CH4-1 downto 0);
         o_RxDataValidCh4_p   : out std_logic;
         o_RxReadyCh5_p       : out std_logic;
         i_RxReCh5_p          : in std_logic;
         ov_RxDataCh5_p       : out std_logic_vector(C_RX_WIDTH_CH5-1 downto 0);
         o_RxDataValidCh5_p   : out std_logic;
         o_RxReadyCh6_p       : out std_logic;
         i_RxReCh6_p          : in std_logic;
         ov_RxDataCh6_p       : out std_logic_vector(C_RX_WIDTH_CH6-1 downto 0);
         o_RxDataValidCh6_p   : out std_logic;
         o_RxReadyCh7_p       : out std_logic;
         i_RxReCh7_p          : in std_logic;
         ov_RxDataCh7_p       : out std_logic_vector(C_RX_WIDTH_CH7-1 downto 0);
         o_RxDataValidCh7_p   : out std_logic
        );
end rtdex_rx;

architecture rtdex_rx_behav of rtdex_rx is

-- Use to avoid unrelated LUT packing with other components
attribute keep_hierarchy : string;
attribute keep_hierarchy of rtdex_rx_behav : architecture is "true";

 component rtdex_rx_core
	port(
		RTDEX_RXD_ACLK				: in std_logic;
		V32_RTDEX_RXD_TDATA			: in std_logic_vector(31 downto 0);
		RTDEX_RXD_TKEEP				: in std_logic_vector(3 downto 0);
		RTDEX_RXD_TVALID			: in std_logic;
		RTDEX_RXD_TLAST				: in std_logic;
		RTDEX_RXD_TREADY			: out std_logic;
		i_sysRst_p					: in std_logic;
		--
		iv32_rxtimeout2dropfrm_p	: in std_logic_vector(31 downto 0);
		iv8_RxStartNewTransfer_p	: in std_logic_vector(7 downto 0);
		oa8u32_FrameNumber_p		: out array8_u32_t;
		ov8_RxFrameCompleted_p		: out std_logic_vector(7 downto 0);
		o_RtdexBadFrame_p			: out std_logic;
		ov8_RtdexDropedFrame_p		: out std_logic_vector(7 downto 0);
		--
		iv8_ChFifoNotFull_p			: in std_logic_vector(7 downto 0);
		ia8u24_ChFifoFreeSpace_p	: in array8_u24_t;
		ov8_ChWr_en_p				: out std_logic_vector(7 downto 0);
		--
		iv8_ProgFull_p				: in std_logic_vector(7 downto 0);
		o_SendPause_p				: out std_logic;
		ov16_PauseVal_p				: out std_logic_vector(15 downto 0)
		);

 end component;

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

signal a8u32_LostFrameCnt_s     : array8_u32_t;
signal a8u32_RcvdFramesCnt_s    : array8_u32_t;
signal a8u32_FrameNumber_s	  	: array8_u32_t;
signal a8u32_FrmNbrExpected_s   : array8_u32_t;

signal v8_RxFrameCompleted_s  : std_logic_vector(7 downto 0);
signal v8_ChAlmostFull_s      : std_logic_vector(7 downto 0);
signal v8_underflow_s         : std_logic_vector(7 downto 0);

attribute keep : string;
 --attribute keep of v8_progFull_s : signal is "true";
 --attribute keep of v8_progEmpty_s : signal is "true";

signal v32_FifoDin_s    : std_logic_vector(31 downto 0);

-- Defines array with the maximal data width 
signal a8v256_RxChData_s    : array8_v256_t;
signal a8v256_ChFifoDout_s  : array8_v256_t;

signal v8_reset_s             : std_logic_vector(7 downto 0);
signal v8_RxChFifoRst_s       : std_logic_vector(7 downto 0);
signal v8_ChWr_en_s           : std_logic_vector(7 downto 0);
signal v8_RxChRe_s            : std_logic_vector(7 downto 0);
signal v8_ChEmpty_s           : std_logic_vector(7 downto 0);
signal v8_RxChDataValid_s     : std_logic_vector(7 downto 0);
signal v8_ChFifoNotFull_s     : std_logic_vector(7 downto 0);
signal v8_RxChReady_s         : std_logic_vector(7 downto 0);
signal RtdexBadFrame_s        : std_logic;
signal v8_RtdexDropedFrame_s  : std_logic_vector(7 downto 0);

signal v32_RxBadFrameCnt_s    : unsigned(31 downto 0);

signal RxResetDly1_s : std_logic;
signal sysRst_s      : std_logic;

signal a8u32_DropdFramesCnt_s  : array8_u32_t;

signal RxBadFrmErr_s	: std_logic;
signal RxLostFrmErr_s	: std_logic;
signal RxDropdFrmErr_s	: std_logic;
signal v8_FrmLostErr_s : std_logic_vector(7 downto 0);
signal v8_RxDropdFrmErr_s : std_logic_vector(7 downto 0);

signal a8v24_ChFifoWrCnt_s      : array8_v24_t;
signal a8u24_ChFifoFreeSpace_s  : array8_u24_t;

signal StatisticCntEnabled_s	: std_logic;
signal PauseReqSendCapab_s		: std_logic;

signal v24_progFullThreshNegate_s,
	   v24_progFullThreshAssert_s		: std_logic_vector(23 downto 0);

signal v8_progFull_s	: std_logic_vector(7 downto 0);

--**************************** Constants definition ******************************************
constant SecurityBuffer_c   : integer := 4;
constant ChFifoSize_c       : unsigned(23 downto 0):= to_unsigned((C_RXMEM - SecurityBuffer_c),24);
--********************************************************************************************

  -- attribute MAX_FANOUT                  : string;
  -- attribute MAX_FANOUT of v8_RxChRe_s   : signal is "5";

begin

	RTDEx_RxCoreMachine : rtdex_rx_core
	port map(
		RTDEX_RXD_ACLK				=> RTDEX_RXD_ACLK		,
		V32_RTDEX_RXD_TDATA			=> V32_RTDEX_RXD_TDATA	,
		RTDEX_RXD_TKEEP				=> RTDEX_RXD_TKEEP		,
		RTDEX_RXD_TVALID			=> RTDEX_RXD_TVALID	    ,
		RTDEX_RXD_TLAST				=> RTDEX_RXD_TLAST		,
		RTDEX_RXD_TREADY			=> RTDEX_RXD_TREADY	   ,
		i_sysRst_p					=> sysRst_s,
		--
		iv32_rxtimeout2dropfrm_p	=> iv32_rxtimeout2dropfrm_p,
		iv8_RxStartNewTransfer_p	=> iv8_RxStartNewTransfer_p,
		oa8u32_FrameNumber_p		=> a8u32_FrameNumber_s     ,
		ov8_RxFrameCompleted_p		=> v8_RxFrameCompleted_s   ,
		o_RtdexBadFrame_p			=> RtdexBadFrame_s         ,
		ov8_RtdexDropedFrame_p		=> v8_RtdexDropedFrame_s   ,
		--
		iv8_ChFifoNotFull_p			=> v8_ChFifoNotFull_s   ,
		ia8u24_ChFifoFreeSpace_p    => a8u24_ChFifoFreeSpace_s,
		ov8_ChWr_en_p				=> v8_ChWr_en_s,
		iv8_ProgFull_p				=> v8_progFull_s,
		o_SendPause_p				=> o_SendPause_p,
		ov16_PauseVal_p				=> ov16_PauseVal_p
		);

 -----------------------------------------------------------------------
 -- Reset handling
 -----------------------------------------------------------------------
 process(RTDEX_RXD_ACLK)
 begin
   if rising_edge(RTDEX_RXD_ACLK) then
      RxResetDly1_s <= i_RxReset_p;
   end if;
 end process;

 -- system reset
 sysRst_s <= RTDEX_RXD_ARESET and not(RxResetDly1_s);

 FifoFreeSpace_gen: for i in 0 to MaxChannelNb_c - 1 generate
 	-- FIFO free space (in 4 bytes word) = FIFO size - FIFO Write counte:
    a8u24_ChFifoFreeSpace_s(i) <= X"000000" when (unsigned(a8v24_ChFifoWrCnt_s(i)) >= ChFifoSize_c) else (ChFifoSize_c - unsigned(a8v24_ChFifoWrCnt_s(i)));
 end generate FifoFreeSpace_gen;

 -----------------------------------------------------------------------
 --- RTDEx channels FIFO ---
 -----------------------------------------------------------------------
         
 -- Little indian: convenient for pc architecture
 LittleEndianGen: if big_Endian_g = 0 generate 
    v32_FifoDin_s <= V32_RTDEX_RXD_TDATA;
 end generate LittleEndianGen;
  
 -- Big Endian: swape bytes
 BigEndianGen: if big_Endian_g = 1 generate 
    v32_FifoDin_s(31 downto 24) <= V32_RTDEX_RXD_TDATA(7 downto 0);  
    v32_FifoDin_s(23 downto 16) <= V32_RTDEX_RXD_TDATA(15 downto 8); 
    v32_FifoDin_s(15 downto 8)  <= V32_RTDEX_RXD_TDATA(23 downto 16);
    v32_FifoDin_s(7 downto 0)   <= V32_RTDEX_RXD_TDATA(31 downto 24); 	
 end generate BigEndianGen;
     
 ChGen: for i in 0 to (NumberChannels_g - 1) generate

   v8_ChFifoNotFull_s(i)   <= not(v8_ChAlmostFull_s(i));

   v8_RxChReady_s(i) <=  not(v8_ChEmpty_s(i));

    U0_ChanFifo: entity lyt_axi_emac_rtdex_v1_00_a.generic_fifo
    generic map (
        -- User side match the specified parameters
        READ_WIDTH_g              => A8I_RX_WIDTH(i),
        READ_DEPTH_g              => (C_RXMEM*32/A8I_RX_WIDTH(i)),
        -- Always 32 bits in the AXI side
        WRITE_WIDTH_g             => 32,
        WRITE_DEPTH_g             => C_RXMEM,
        -- Fix configuration for RTDEx RX FIFO
        FIRST_WORD_FALL_THROUGH_g => false,
        FULL_FLAGS_RST_VAL        => 0,
        USE_EMBEDDED_REG          => 1
    )
    port map (
        i_rst_p                         => v8_RxChFifoRst_s(i),
        i_wr_clk_p                      => RTDEX_RXD_ACLK,
        i_rd_clk_p                      => i_RxUserClk_p,
        iv_din_p                        => v32_FifoDin_s,
        i_wr_en_p                       => v8_ChWr_en_s(i),
        i_rd_en_p                       => v8_RxChRe_s(i),
        iv_prog_empty_thresh_p          => X"000000",
        iv_prog_full_thresh_assert_p    => v24_progFullThreshAssert_s,
        iv_prog_full_thresh_negate_p    => v24_progFullThreshNegate_s,
        ov_dout_p                       => a8v256_ChFifoDout_s(i)(A8I_RX_WIDTH(i)-1 downto 0),
        o_full_p                        => open,
        o_overflow_p                    => open,
        o_empty_p                       => v8_ChEmpty_s(i),
        o_valid_p                       => v8_RxChDataValid_s(i),
        o_underflow_p                   => v8_underflow_s(i),
        ov_rd_data_count_p              => open,
        ov_wr_data_count_p              => a8v24_ChFifoWrCnt_s(i),
        o_prog_full_p                   => v8_progFull_s(i),
        o_prog_empty_p                  => open,
        o_almost_full_p                 => v8_ChAlmostFull_s(i)
    );
    
     WordUnpackingGen: for k in 0 to ((A8I_RX_WIDTH(i)/32)-1) generate
         a8v256_RxChData_s(i)((32*(k+1)-1) downto (32*k)) <= a8v256_ChFifoDout_s(i)((A8I_RX_WIDTH(i)-32*k-1) downto (A8I_RX_WIDTH(i)-32*(k+1)));
     end generate WordUnpackingGen;

 end generate ChGen;

 -----------------------------------------------------------------------
 -- Regs to aid timings
 -----------------------------------------------------------------------
 process(RTDEX_RXD_ACLK)
 begin
 	if rising_edge(RTDEX_RXD_ACLK) then
 		v24_progFullThreshAssert_s <= iv32_RxFifoFullAssertThrCh0_p(23 downto 0);
 		v24_progFullThreshNegate_s <= iv32_RxFifoFullNegateThrCh0_p(23 downto 0);
 	end if;
 end process;

 --*************** Statistic counter gen *******************************--
 IncludeStatCnts_gen :if StatsCntrs_g = 1 generate
 -----------------------------------------------------------------------
 -- Received Frames counter: counts completed Rxed frames for the host
 -- to know. Reset to 0 when starting new transfer. In case of continous
 -- xfer, the counter will simply wrapp-around. In this case, the host can
 -- still at least check that the counter is moving.
 -----------------------------------------------------------------------
 RcvdFramesCnt_gen: for i in 0 to (NumberChannels_g - 1) generate
 	process(RTDEX_RXD_ACLK)
	 begin
	 	if rising_edge(RTDEX_RXD_ACLK) then
			if sysRst_s = '0' then
				a8u32_RcvdFramesCnt_s(i) <= (others=>'0');
			else
				if v8_RxFrameCompleted_s(i) = '1' then
					a8u32_RcvdFramesCnt_s(i) <= a8u32_RcvdFramesCnt_s(i) + 1;
				end if;
			end if;
		end if;
	 end process;
 end generate RcvdFramesCnt_gen;

 -- Tie unused channel's counters to gnd
 UnusedRcvdFramesCnt_CondGen: if NumberChannels_g /= MaxChannelNb_c generate
 	UnusedRcvdFramesCnt_gen: for i in NumberChannels_g to (MaxChannelNb_c-1) generate
 		a8u32_RcvdFramesCnt_s(i) <= (others=>'0');
 	end generate UnusedRcvdFramesCnt_gen;
 end generate UnusedRcvdFramesCnt_CondGen;

 -----------------------------------------------------------------------
 -- Check if a frame was lost: compare the counters, actual with
 -- expected one.
 -----------------------------------------------------------------------
 LostFrmCnt_gen: for i in 0 to (NumberChannels_g - 1) generate
 process(RTDEX_RXD_ACLK)
 begin
   if rising_edge(RTDEX_RXD_ACLK) then
		if sysRst_s = '0' then
		   a8u32_FrmNbrExpected_s(i) <= (others=>'0');
		   a8u32_LostFrameCnt_s(i) 	 <= (others=>'0');
		   v8_FrmLostErr_s(i)  <= '0';
		else

		   if v8_RxFrameCompleted_s(i) = '1' then
		      if (a8u32_FrameNumber_s(i) /= a8u32_FrmNbrExpected_s(i)) then
		         -- Frame lost for the current channel, increment the counter.
		         a8u32_LostFrameCnt_s(i) <= a8u32_LostFrameCnt_s(i) + 1;
		         -- Load Expected counter with the actual received frame number + 1.
		         a8u32_FrmNbrExpected_s(i) <= a8u32_FrameNumber_s(i) + 1;
		         v8_FrmLostErr_s(i)  <= '1';
		      else
		      	   -- Increment for next frame to come
		      	   a8u32_FrmNbrExpected_s(i) <= a8u32_FrmNbrExpected_s(i) + 1;
		      end if;
		   end if;

		end if;
	end if;
 end process;

 end generate LostFrmCnt_gen;

 -- Latched error status bit if Frame Lost in ANY channel
 RxLostFrmErr_s <= or_reduce(v8_FrmLostErr_s);

 -- Tie unused channel's counters to gnd
 UnusedFrameNbCompare_gen_CondGen: if NumberChannels_g /= MaxChannelNb_c generate
 	UnusedFrameNbCompare_gen: for i in NumberChannels_g to (MaxChannelNb_c - 1) generate
 		a8u32_FrmNbrExpected_s(i) <= (others=>'0');
 		a8u32_LostFrameCnt_s(i) <= (others=>'0');
 		v8_FrmLostErr_s(i)  <= '0';
 	end generate UnusedFrameNbCompare_gen;
 end generate UnusedFrameNbCompare_gen_CondGen;

 -----------------------------------------------------------------------
 --- Bad Frames counter
 -----------------------------------------------------------------------
 process(RTDEX_RXD_ACLK)
 begin
   if rising_edge(RTDEX_RXD_ACLK) then
		if sysRst_s = '0' then
		   v32_RxBadFrameCnt_s <= (others=>'0');
		   RxBadFrmErr_s <= '0';
		else

		   if (RtdexBadFrame_s = '1') then
		      v32_RxBadFrameCnt_s <= v32_RxBadFrameCnt_s + 1;
		      RxBadFrmErr_s <= '1';
		   end if;

		end if;
	end if;
 end process;

 -----------------------------------------------------------------------
 -- Droped Frames counter per channel
 -----------------------------------------------------------------------
 DropedFramesCnt_gen: for i in 0 to (NumberChannels_g - 1) generate
 	process(RTDEX_RXD_ACLK)
	 begin
	 	if rising_edge(RTDEX_RXD_ACLK) then
			if sysRst_s = '0' then
				a8u32_DropdFramesCnt_s(i) <= (others=>'0');
				v8_RxDropdFrmErr_s(i) <= '0';
			else
				if v8_RtdexDropedFrame_s(i) = '1' then
					a8u32_DropdFramesCnt_s(i) <= a8u32_DropdFramesCnt_s(i) + 1;
					v8_RxDropdFrmErr_s(i) <= '1';
				end if;
			end if;
		end if;
	 end process;
 end generate DropedFramesCnt_gen;

 RxDropdFrmErr_s <= or_reduce(v8_RxDropdFrmErr_s);

 -- Tie unused channel's counters to gnd
 UnusedDropedFramesCnt_CondGen: if NumberChannels_g /= MaxChannelNb_c generate
 	UnusedDropedFramesCnt_gen: for i in NumberChannels_g to (MaxChannelNb_c-1) generate
 		a8u32_DropdFramesCnt_s(i) <= (others=>'0');
 		v8_RxDropdFrmErr_s(i) <= '0';
 	end generate UnusedDropedFramesCnt_gen;
 end generate UnusedDropedFramesCnt_CondGen;

 end generate IncludeStatCnts_gen;

  -- If Stats counters are not included:
  NoStatCnts_gen :if StatsCntrs_g = 0 generate

  	Loop_gen : for i in 0 to (MaxChannelNb_c - 1) generate
  		a8u32_RcvdFramesCnt_s(i) 	<= x"DEADBEEF";
  		a8u32_LostFrameCnt_s(i)  	<= x"DEADBEEF";
  		a8u32_DropdFramesCnt_s(i)	<= x"DEADBEEF";
  	end generate Loop_gen;

	v32_RxBadFrameCnt_s <= x"DEADBEEF";
	RxLostFrmErr_s 		<= '0';
	RxBadFrmErr_s 		<= '0';
	RxDropdFrmErr_s 	<= '0';

 end generate NoStatCnts_gen;

 --*************** End Statistic counter gen ******************************--

 -----------------------------------------------------------------------
 -- FIFO data info
 -----------------------------------------------------------------------

 ---
 NoStatisticCnt_gen: if StatsCntrs_g = 0 generate
 	StatisticCntEnabled_s <= '0';
 end generate NoStatisticCnt_gen;

 StatisticCnt_gen: if StatsCntrs_g = 1 generate
 	StatisticCntEnabled_s <= '1';
 end generate StatisticCnt_gen;

 NoPauseReq_gen: if FlowCtrlEn_g = 0 generate
 	PauseReqSendCapab_s <= '0';
 end generate NoPauseReq_gen;

 PauseReq_gen: if FlowCtrlEn_g = 1 generate
 	PauseReqSendCapab_s <= '1';
 end generate PauseReq_gen;
 -----------------------------------------------------------------------
 --- Output ports ---
 -----------------------------------------------------------------------

 -- Don't care for RXS (Status data), so we set TREADY to 1 & ignore RXS inputs
 RTDEX_RXS_TREADY <= '1';

 ov32_RxBadFrameCnt_p         <= std_logic_vector(v32_RxBadFrameCnt_s);

 ov32_RxFrameLostCntCh0_p     <= std_logic_vector(a8u32_LostFrameCnt_s(0));
 ov32_RxFrameLostCntCh1_p     <= std_logic_vector(a8u32_LostFrameCnt_s(1));
 ov32_RxFrameLostCntCh2_p     <= std_logic_vector(a8u32_LostFrameCnt_s(2));
 ov32_RxFrameLostCntCh3_p     <= std_logic_vector(a8u32_LostFrameCnt_s(3));
 ov32_RxFrameLostCntCh4_p     <= std_logic_vector(a8u32_LostFrameCnt_s(4));
 ov32_RxFrameLostCntCh5_p     <= std_logic_vector(a8u32_LostFrameCnt_s(5));
 ov32_RxFrameLostCntCh6_p     <= std_logic_vector(a8u32_LostFrameCnt_s(6));
 ov32_RxFrameLostCntCh7_p     <= std_logic_vector(a8u32_LostFrameCnt_s(7));

 ov32_RxConfigInfo_p          <= std_logic_vector(to_unsigned(C_RXMEM/1024,20)) & PauseReqSendCapab_s & StatisticCntEnabled_s & "000000" & std_logic_vector(to_unsigned(NumberChannels_g,4));

 --- user IOs ---
 v8_RxChFifoRst_s(0) <= i_RxReset_p or iv8_RxFifoReset_p(0);
 v8_RxChFifoRst_s(1) <= i_RxReset_p or iv8_RxFifoReset_p(1);
 v8_RxChFifoRst_s(2) <= i_RxReset_p or iv8_RxFifoReset_p(2);
 v8_RxChFifoRst_s(3) <= i_RxReset_p or iv8_RxFifoReset_p(3);
 v8_RxChFifoRst_s(4) <= i_RxReset_p or iv8_RxFifoReset_p(4);
 v8_RxChFifoRst_s(5) <= i_RxReset_p or iv8_RxFifoReset_p(5);
 v8_RxChFifoRst_s(6) <= i_RxReset_p or iv8_RxFifoReset_p(6);
 v8_RxChFifoRst_s(7) <= i_RxReset_p or iv8_RxFifoReset_p(7);


 o_RxReadyCh0_p <= v8_RxChReady_s(0);
 o_RxReadyCh1_p <= v8_RxChReady_s(1);
 o_RxReadyCh2_p <= v8_RxChReady_s(2);
 o_RxReadyCh3_p <= v8_RxChReady_s(3);
 o_RxReadyCh4_p <= v8_RxChReady_s(4);
 o_RxReadyCh5_p <= v8_RxChReady_s(5);
 o_RxReadyCh6_p <= v8_RxChReady_s(6);
 o_RxReadyCh7_p <= v8_RxChReady_s(7);


 v8_RxChRe_s(0) <= i_RxReCh0_p;
 v8_RxChRe_s(1) <= i_RxReCh1_p;
 v8_RxChRe_s(2) <= i_RxReCh2_p;
 v8_RxChRe_s(3) <= i_RxReCh3_p;
 v8_RxChRe_s(4) <= i_RxReCh4_p;
 v8_RxChRe_s(5) <= i_RxReCh5_p;
 v8_RxChRe_s(6) <= i_RxReCh6_p;
 v8_RxChRe_s(7) <= i_RxReCh7_p;

 ov_RxDataCh0_p <= a8v256_RxChData_s(0)((A8I_RX_WIDTH(0)-1) downto 0);
 ov_RxDataCh1_p <= a8v256_RxChData_s(1)((A8I_RX_WIDTH(1)-1) downto 0);
 ov_RxDataCh2_p <= a8v256_RxChData_s(2)((A8I_RX_WIDTH(2)-1) downto 0);
 ov_RxDataCh3_p <= a8v256_RxChData_s(3)((A8I_RX_WIDTH(3)-1) downto 0);
 ov_RxDataCh4_p <= a8v256_RxChData_s(4)((A8I_RX_WIDTH(4)-1) downto 0);
 ov_RxDataCh5_p <= a8v256_RxChData_s(5)((A8I_RX_WIDTH(5)-1) downto 0);
 ov_RxDataCh6_p <= a8v256_RxChData_s(6)((A8I_RX_WIDTH(6)-1) downto 0);
 ov_RxDataCh7_p <= a8v256_RxChData_s(7)((A8I_RX_WIDTH(7)-1) downto 0);

 o_RxDataValidCh0_p <= v8_RxChDataValid_s(0);
 o_RxDataValidCh1_p <= v8_RxChDataValid_s(1);
 o_RxDataValidCh2_p <= v8_RxChDataValid_s(2);
 o_RxDataValidCh3_p <= v8_RxChDataValid_s(3);
 o_RxDataValidCh4_p <= v8_RxChDataValid_s(4);
 o_RxDataValidCh5_p <= v8_RxChDataValid_s(5);
 o_RxDataValidCh6_p <= v8_RxChDataValid_s(6);
 o_RxDataValidCh7_p <= v8_RxChDataValid_s(7);

 ov32_RcvdFrameCntCh0_p <= std_logic_vector(a8u32_RcvdFramesCnt_s(0));
 ov32_RcvdFrameCntCh1_p <= std_logic_vector(a8u32_RcvdFramesCnt_s(1));
 ov32_RcvdFrameCntCh2_p <= std_logic_vector(a8u32_RcvdFramesCnt_s(2));
 ov32_RcvdFrameCntCh3_p <= std_logic_vector(a8u32_RcvdFramesCnt_s(3));
 ov32_RcvdFrameCntCh4_p <= std_logic_vector(a8u32_RcvdFramesCnt_s(4));
 ov32_RcvdFrameCntCh5_p <= std_logic_vector(a8u32_RcvdFramesCnt_s(5));
 ov32_RcvdFrameCntCh6_p <= std_logic_vector(a8u32_RcvdFramesCnt_s(6));
 ov32_RcvdFrameCntCh7_p <= std_logic_vector(a8u32_RcvdFramesCnt_s(7));

 ov32_RxDropdFrmsCh0_p <= std_logic_vector(a8u32_DropdFramesCnt_s(0));
 ov32_RxDropdFrmsCh1_p <= std_logic_vector(a8u32_DropdFramesCnt_s(1));
 ov32_RxDropdFrmsCh2_p <= std_logic_vector(a8u32_DropdFramesCnt_s(2));
 ov32_RxDropdFrmsCh3_p <= std_logic_vector(a8u32_DropdFramesCnt_s(3));
 ov32_RxDropdFrmsCh4_p <= std_logic_vector(a8u32_DropdFramesCnt_s(4));
 ov32_RxDropdFrmsCh5_p <= std_logic_vector(a8u32_DropdFramesCnt_s(5));
 ov32_RxDropdFrmsCh6_p <= std_logic_vector(a8u32_DropdFramesCnt_s(6));
 ov32_RxDropdFrmsCh7_p <= std_logic_vector(a8u32_DropdFramesCnt_s(7));

 ov3_RxErrStatus_p(0) <= RxBadFrmErr_s;
 ov3_RxErrStatus_p(1) <= RxLostFrmErr_s;
 ov3_RxErrStatus_p(2) <= RxDropdFrmErr_s;

 ov8_RxFifoUnderrun_p <= v8_underflow_s;

end rtdex_rx_behav;