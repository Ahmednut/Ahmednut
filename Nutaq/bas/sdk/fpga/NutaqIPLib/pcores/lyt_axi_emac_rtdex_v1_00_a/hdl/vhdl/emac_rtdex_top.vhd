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

library lyt_axi_emac_rtdex_v1_00_a;
use lyt_axi_emac_rtdex_v1_00_a.rtdex_pkg.all;

-------------------------------------------------------------------------------------
--
--
-- Definition of Ports

-------------------------------------------------------------------------------

------------------------------------------------------------------------------
-- Entity Section
------------------------------------------------------------------------------

entity emac_rtdex_top is
   generic (
     C_RTDEX_RX_NUMER_OF_CHANNELS 	: integer range 1 to 8:=1;
     C_RTDEX_TX_NUMER_OF_CHANNELS 	: integer range 1 to 8:=1;
     C_TX_CH_FIFO_DEPTH            	: integer := 4096;
     C_RX_CH_FIFO_DEPTH            	: integer := 4096;
     C_RX_WIDTH_CH0                 : integer := 32;
     C_RX_WIDTH_CH1                 : integer := 32;
     C_RX_WIDTH_CH2                 : integer := 32;
     C_RX_WIDTH_CH3                 : integer := 32;
     C_RX_WIDTH_CH4                 : integer := 32;
     C_RX_WIDTH_CH5                 : integer := 32;
     C_RX_WIDTH_CH6                 : integer := 32;
     C_RX_WIDTH_CH7                 : integer := 32;
     C_TX_WIDTH_CH0                 : integer := 32;
     C_TX_WIDTH_CH1                 : integer := 32;
     C_TX_WIDTH_CH2                 : integer := 32;
     C_TX_WIDTH_CH3                 : integer := 32;
     C_TX_WIDTH_CH4                 : integer := 32;
     C_TX_WIDTH_CH5                 : integer := 32;
     C_TX_WIDTH_CH6                 : integer := 32;
     C_TX_WIDTH_CH7                 : integer := 32;
     C_SUPPORT_JUMBO_FRM		   	: integer range 0 to 1 := 1;
     C_RX_STATS_COUNTERS  		   	: integer range 0 to 1 := 1;
     C_TX_STATS_COUNTERS  		   	: integer range 0 to 1 := 1;
     C_ENABLE_FLOW_CTRL		   		: integer range 0 to 1 := 0;
     big_Endian_g					: integer range 0 to 1 := 0
  );
	port
	(
	-- config signals from/to axi registers --
	iv32_RemoteEndMacAddrH_p    : in  std_logic_vector(31 downto 0);
	iv16_RemoteEndMacAddrL_p    : in  std_logic_vector(15 downto 0);
	iv32_LocalEndMacAddrH_p     : in  std_logic_vector(31 downto 0);
	iv16_LocalEndMacAddrL_p     : in  std_logic_vector(15 downto 0);
	-- RX
	i_RxMode_p					: in std_logic;
	iv8_RxStartNewTransfer_p    : in  std_logic_vector(7 downto 0);
	ov32_RxBadFrameCnt_p        : out std_logic_vector(31 downto 0);
	ov32_RxFrameLostCntCh0_p    : out std_logic_vector(31 downto 0);
    ov32_RxFrameLostCntCh1_p    : out std_logic_vector(31 downto 0);
    ov32_RxFrameLostCntCh2_p    : out std_logic_vector(31 downto 0);
    ov32_RxFrameLostCntCh3_p    : out std_logic_vector(31 downto 0);
    ov32_RxFrameLostCntCh4_p    : out std_logic_vector(31 downto 0);
    ov32_RxFrameLostCntCh5_p    : out std_logic_vector(31 downto 0);
    ov32_RxFrameLostCntCh6_p    : out std_logic_vector(31 downto 0);
    ov32_RxFrameLostCntCh7_p    : out std_logic_vector(31 downto 0);
    iv8_RxFifoReset_p           : in  std_logic_vector(7 downto 0);
    i_RxReset_p                 : in  std_logic;
    ov32_RxConfigInfo_p         : out std_logic_vector(31 downto 0);
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
    -- TX
    iv32_FrameGap_p             : in std_logic_vector(31 downto 0);
    i_Mode_p					: in std_logic;
    iv8_TxChFrsBurst_p			: in std_logic_vector(7 downto 0);

    iv15_FrameSizeCh0_p         : in std_logic_vector(14 downto 0);
    iv32_TransferSizeCh0_p      : in std_logic_vector(31 downto 0);

    iv15_FrameSizeCh1_p         : in std_logic_vector(14 downto 0);
    iv32_TransferSizeCh1_p      : in std_logic_vector(31 downto 0);

    iv15_FrameSizeCh2_p         : in std_logic_vector(14 downto 0);
    iv32_TransferSizeCh2_p      : in std_logic_vector(31 downto 0);

    iv15_FrameSizeCh3_p         : in std_logic_vector(14 downto 0);
    iv32_TransferSizeCh3_p      : in std_logic_vector(31 downto 0);

    iv15_FrameSizeCh4_p         : in std_logic_vector(14 downto 0);
    iv32_TransferSizeCh4_p      : in std_logic_vector(31 downto 0);

    iv15_FrameSizeCh5_p         : in std_logic_vector(14 downto 0);
    iv32_TransferSizeCh5_p      : in std_logic_vector(31 downto 0);

    iv15_FrameSizeCh6_p         : in std_logic_vector(14 downto 0);
    iv32_TransferSizeCh6_p      : in std_logic_vector(31 downto 0);

    iv15_FrameSizeCh7_p         : in std_logic_vector(14 downto 0);
    iv32_TransferSizeCh7_p      : in std_logic_vector(31 downto 0);

    i_TxReset_p                 : in  std_logic;
    iv8_TxFifoReset_p           : in  std_logic_vector(7 downto 0);
    iv8_TxStartNewTransfer_p    : in  std_logic_vector(7 downto 0);
    ov32_TxConfigInfo_p         : out std_logic_vector(31 downto 0);
    ov8_TxFifoOverrun_p			: out  std_logic_vector(7 downto 0);
    --------------------------------------------
    ----    RX side: From EMAC to EDMA     -----
    --------------------------------------------
	-- AXI streaming RX To EDMA --
	AXI_STR_EDMA_RXD_ACLK         : in  std_logic;                           --  AXI-Stream Receive Data Clk
    AXI_STR_EDMA_RXD_ARESETN      : in  std_logic;                           --  AXI-Stream Receive Data Reset
    AXI_STR_EDMA_RXD_TVALID       : out std_logic;                           --  AXI-Stream Receive Data Valid
    AXI_STR_EDMA_RXD_TREADY       : in  std_logic;                           --  AXI-Stream Receive Data Ready
    AXI_STR_EDMA_RXD_TLAST        : out std_logic;                           --  AXI-Stream Receive Data Last
    AXI_STR_EDMA_RXD_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Data Keep
    AXI_STR_EDMA_RXD_TDATA        : out std_logic_vector(31 downto 0);       --  AXI-Stream Receive Data Data

    AXI_STR_EDMA_RXS_ACLK         : in  std_logic;                           --  AXI-Stream Receive Status Clk
    AXI_STR_EDMA_RXS_ARESETN      : in  std_logic;                           --  AXI-Stream Receive Status Reset
    AXI_STR_EDMA_RXS_TVALID       : out std_logic;                           --  AXI-Stream Receive Status Valid
    AXI_STR_EDMA_RXS_TREADY       : in  std_logic;                           --  AXI-Stream Receive Status Ready
    AXI_STR_EDMA_RXS_TLAST        : out std_logic;                           --  AXI-Stream Receive Status Last
    AXI_STR_EDMA_RXS_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Status Keep
    AXI_STR_EDMA_RXS_TDATA        : out std_logic_vector(31 downto 0);       --  AXI-Stream Receive Status Data

    -- AXI streaming RX From EMAC --
    AXI_STR_EMAC_TXD_ACLK         : in  std_logic;                           --  AXI-Stream Transmit Data Clk
    AXI_STR_EMAC_TXD_ARESETN      : in  std_logic;                           --  AXI-Stream Transmit Data Reset
    AXI_STR_EMAC_TXD_TVALID       : in  std_logic;                           --  AXI-Stream Transmit Data Valid
    AXI_STR_EMAC_TXD_TREADY       : out std_logic;                           --  AXI-Stream Transmit Data Ready
    AXI_STR_EMAC_TXD_TLAST        : in  std_logic;                           --  AXI-Stream Transmit Data Last
    AXI_STR_EMAC_TXD_TKEEP        : in  std_logic_vector(3 downto 0);        --  AXI-Stream Transmit Data Keep
    AXI_STR_EMAC_TXD_TDATA        : in  std_logic_vector(31 downto 0);       --  AXI-Stream Transmit Data Data

    AXI_STR_EMAC_TXC_ACLK         : in  std_logic;                           --  AXI-Stream Transmit Control Clk
    AXI_STR_EMAC_TXC_ARESETN      : in  std_logic;                           --  AXI-Stream Transmit Control Reset
    AXI_STR_EMAC_TXC_TVALID       : in  std_logic;                           --  AXI-Stream Transmit Control Valid
    AXI_STR_EMAC_TXC_TREADY       : out std_logic;                           --  AXI-Stream Transmit Control Ready
    AXI_STR_EMAC_TXC_TLAST        : in  std_logic;                           --  AXI-Stream Transmit Control Last
    AXI_STR_EMAC_TXC_TKEEP        : in  std_logic_vector(3 downto 0);        --  AXI-Stream Transmit Control Keep
    AXI_STR_EMAC_TXC_TDATA        : in  std_logic_vector(31 downto 0);

    --------------------------------------------
    ----     TX side: From EDMA to EMAC     -----
    --------------------------------------------
    -- AXI streaming TX to EMAC --
	AXI_STR_EMAC_RXD_ACLK         : in  std_logic;                           --  AXI-Stream Receive Data Clk
    AXI_STR_EMAC_RXD_ARESETN      : in  std_logic;                           --  AXI-Stream Receive Data Reset
    AXI_STR_EMAC_RXD_TVALID       : out std_logic;                           --  AXI-Stream Receive Data Valid
    AXI_STR_EMAC_RXD_TREADY       : in  std_logic;                           --  AXI-Stream Receive Data Ready
    AXI_STR_EMAC_RXD_TLAST        : out std_logic;                           --  AXI-Stream Receive Data Last
    AXI_STR_EMAC_RXD_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Data Keep
    AXI_STR_EMAC_RXD_TDATA        : out std_logic_vector(31 downto 0);       --  AXI-Stream Receive Data Data

    AXI_STR_EMAC_RXS_ACLK         : in  std_logic;                           --  AXI-Stream Receive Status Clk
    AXI_STR_EMAC_RXS_ARESETN      : in  std_logic;                           --  AXI-Stream Receive Status Reset
    AXI_STR_EMAC_RXS_TVALID       : out std_logic;                           --  AXI-Stream Receive Status Valid
    AXI_STR_EMAC_RXS_TREADY       : in  std_logic;                           --  AXI-Stream Receive Status Ready
    AXI_STR_EMAC_RXS_TLAST        : out std_logic;                           --  AXI-Stream Receive Status Last
    AXI_STR_EMAC_RXS_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Status Keep
    AXI_STR_EMAC_RXS_TDATA        : out std_logic_vector(31 downto 0);       --  AXI-Stream Receive Status Data

    -- AXI streaming TX From EDMA --
    AXI_STR_EDMA_TXD_ACLK         : in  std_logic;                           --  AXI-Stream Transmit Data Clk
    AXI_STR_EDMA_TXD_ARESETN      : in  std_logic;                           --  AXI-Stream Transmit Data Reset
    AXI_STR_EDMA_TXD_TVALID       : in  std_logic;                           --  AXI-Stream Transmit Data Valid
    AXI_STR_EDMA_TXD_TREADY       : out std_logic;                           --  AXI-Stream Transmit Data Ready
    AXI_STR_EDMA_TXD_TLAST        : in  std_logic;                           --  AXI-Stream Transmit Data Last
    AXI_STR_EDMA_TXD_TKEEP        : in  std_logic_vector(3 downto 0);        --  AXI-Stream Transmit Data Keep
    AXI_STR_EDMA_TXD_TDATA        : in  std_logic_vector(31 downto 0);       --  AXI-Stream Transmit Data Data

    AXI_STR_EDMA_TXC_ACLK         : in  std_logic;                           --  AXI-Stream Transmit Control Clk
    AXI_STR_EDMA_TXC_ARESETN      : in  std_logic;                           --  AXI-Stream Transmit Control Reset
    AXI_STR_EDMA_TXC_TVALID       : in  std_logic;                           --  AXI-Stream Transmit Control Valid
    AXI_STR_EDMA_TXC_TREADY       : out std_logic;                           --  AXI-Stream Transmit Control Ready
    AXI_STR_EDMA_TXC_TLAST        : in  std_logic;                           --  AXI-Stream Transmit Control Last
    AXI_STR_EDMA_TXC_TKEEP        : in  std_logic_vector(3 downto 0);        --  AXI-Stream Transmit Control Keep
    AXI_STR_EDMA_TXC_TDATA        : in  std_logic_vector(31 downto 0);

      -- RX User side
      i_RxUserClk_p                 : in std_logic;
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

      -- TX User side
      i_TxUserClk_p                 : in std_logic;

      i_TxWeCh0_p                   : in std_logic;
      o_TxReadyCh0_p                : out std_logic;
      iv_TxDataCh0_p                : in std_logic_vector(C_TX_WIDTH_CH0-1 downto 0);
      ov32_TxFifoCountCh0_p         : out std_logic_vector(31 downto 0);
      ov32_NbrSentFramesCh0_p       : out std_logic_vector(31 downto 0);

      i_TxWeCh1_p             : in std_logic;
      o_TxReadyCh1_p          : out std_logic;
      iv_TxDataCh1_p          : in std_logic_vector(C_TX_WIDTH_CH1-1 downto 0);
      ov32_TxFifoCountCh1_p   : out std_logic_vector(31 downto 0);
      ov32_NbrSentFramesCh1_p : out std_logic_vector(31 downto 0);

      i_TxWeCh2_p             : in std_logic;
      o_TxReadyCh2_p          : out std_logic;
      iv_TxDataCh2_p          : in std_logic_vector(C_TX_WIDTH_CH2-1 downto 0);
      ov32_TxFifoCountCh2_p   : out std_logic_vector(31 downto 0);
      ov32_NbrSentFramesCh2_p : out std_logic_vector(31 downto 0);

      i_TxWeCh3_p             : in std_logic;
      o_TxReadyCh3_p          : out std_logic;
      iv_TxDataCh3_p          : in std_logic_vector(C_TX_WIDTH_CH3-1 downto 0);
      ov32_TxFifoCountCh3_p   : out std_logic_vector(31 downto 0);
      ov32_NbrSentFramesCh3_p : out std_logic_vector(31 downto 0);

      i_TxWeCh4_p             : in std_logic;
      o_TxReadyCh4_p          : out std_logic;
      iv_TxDataCh4_p          : in std_logic_vector(C_TX_WIDTH_CH4-1 downto 0);
      ov32_TxFifoCountCh4_p   : out std_logic_vector(31 downto 0);
      ov32_NbrSentFramesCh4_p : out std_logic_vector(31 downto 0);

      i_TxWeCh5_p             : in std_logic;
      o_TxReadyCh5_p          : out std_logic;
      iv_TxDataCh5_p          : in std_logic_vector(C_TX_WIDTH_CH5-1 downto 0);
      ov32_TxFifoCountCh5_p   : out std_logic_vector(31 downto 0);
      ov32_NbrSentFramesCh5_p : out std_logic_vector(31 downto 0);

      i_TxWeCh6_p             : in std_logic;
      o_TxReadyCh6_p          : out std_logic;
      iv_TxDataCh6_p          : in std_logic_vector(C_TX_WIDTH_CH6-1 downto 0);
      ov32_TxFifoCountCh6_p   : out std_logic_vector(31 downto 0);
      ov32_NbrSentFramesCh6_p : out std_logic_vector(31 downto 0);

      i_TxWeCh7_p             : in std_logic;
      o_TxReadyCh7_p          : out std_logic;
      iv_TxDataCh7_p          : in std_logic_vector(C_TX_WIDTH_CH7-1 downto 0);
      ov32_TxFifoCountCh7_p   : out std_logic_vector(31 downto 0);
      ov32_NbrSentFramesCh7_p : out std_logic_vector(31 downto 0)
	);


end emac_rtdex_top;

------------------------------------------------------------------------------
-- Architecture Section
------------------------------------------------------------------------------


architecture emac_rtdex_top_behav of emac_rtdex_top is

-- Use to avoid unrelated LUT packing with other components
attribute keep_hierarchy : string;
attribute keep_hierarchy of emac_rtdex_top_behav : architecture is "true";

signal Rtdex_rxd_aclk_s       : std_logic;
signal Rtdex_rxd_aresetn_s     : std_logic;
signal Rtdex_rxd_tvalid_s     : std_logic;
signal Rtdex_rxd_tready_s     : std_logic;
signal Rtdex_rxd_tlast_s      : std_logic;
signal Rtdex_rxd_tkeep_s      : std_logic_vector(3 downto 0);
signal v32_Rtdex_rxd_tdata_s  : std_logic_vector(31 downto 0);

signal Rtdex_rxs_aclk_s       : std_logic;
signal Rtdex_rxs_aresetn_s     : std_logic;
signal Rtdex_rxs_tvalid_s     : std_logic;
signal Rtdex_rxs_tready_s     : std_logic;
signal Rtdex_rxs_tlast_s      : std_logic;
signal Rtdex_rxs_tkeep_s      : std_logic_vector(3 downto 0);
signal v32_Rtdex_rxs_tdata_s  : std_logic_vector(31 downto 0);


signal Rtdex_txd_aclk_s       : std_logic;
signal rtdex_txd_aresetn_s     : std_logic;
signal Rtdex_txd_tvalid_s     : std_logic;
signal Rtdex_txd_tready_s     : std_logic;
signal Rtdex_txd_tlast_s      : std_logic;
signal Rtdex_txd_tkeep_s      : std_logic_vector(3 downto 0);
signal Rtdex_txd_tdata_s  : std_logic_vector(31 downto 0);

signal Rtdex_txc_aclk_s       : std_logic;
signal rtdex_txc_aresetn_s    : std_logic;
signal Rtdex_txc_tvalid_s     : std_logic;
signal Rtdex_txc_tready_s     : std_logic;
signal Rtdex_txc_tlast_s      : std_logic;
signal Rtdex_txc_tkeep_s      : std_logic_vector(3 downto 0);
signal Rtdex_txc_tdata_s      : std_logic_vector(31 downto 0);

signal pfg_txd_aclk_s   	 : std_logic;
signal pfg_txd_aresetn_s	 : std_logic;
signal pfg_txd_tvalid_s 	 : std_logic;
signal pfg_txd_tready_s 	 : std_logic;
signal pfg_txd_tlast_s  	 : std_logic;
signal pfg_txd_tkeep_s  	 : std_logic_vector(3 downto 0);
signal pfg_txd_tdata_s  	 : std_logic_vector(31 downto 0);

signal pfg_txc_aclk_s   	 : std_logic;
signal pfg_txc_aresetn_s	 : std_logic;
signal pfg_txc_tvalid_s 	 : std_logic;
signal pfg_txc_tready_s 	 : std_logic;
signal pfg_txc_tlast_s  	 : std_logic;
signal pfg_txc_tkeep_s  	 : std_logic_vector(3 downto 0);
signal pfg_txc_tdata_s  	 : std_logic_vector(31 downto 0);


signal SendPause_s 		: std_logic;
signal v16_PauseVal_s : std_logic_vector(15 downto 0);

begin


 U0_RtdexMux : rtdex_mux_rx
  port map(
  		 -- config & status signals from/to axi registers --
         iv32_RemoteEndMacAddrH_p      => iv32_RemoteEndMacAddrH_p,
         iv16_RemoteEndMacAddrL_p      => iv16_RemoteEndMacAddrL_p,
         iv32_LocalEndMacAddrH_p      => iv32_LocalEndMacAddrH_p,
         iv16_LocalEndMacAddrL_p      => iv16_LocalEndMacAddrL_p,

         -- AXI streaming RX From EMAC --
         AXI_STR_TXD_ACLK         => AXI_STR_EMAC_TXD_ACLK,
         AXI_STR_TXD_ARESETN      => AXI_STR_EMAC_TXD_ARESETN,
         AXI_STR_TXD_TVALID       => AXI_STR_EMAC_TXD_TVALID,
         AXI_STR_TXD_TREADY       => AXI_STR_EMAC_TXD_TREADY,
         AXI_STR_TXD_TLAST        => AXI_STR_EMAC_TXD_TLAST,
         AXI_STR_TXD_TKEEP        => AXI_STR_EMAC_TXD_TKEEP,
         AXI_STR_TXD_TDATA        => AXI_STR_EMAC_TXD_TDATA,

         AXI_STR_TXC_ACLK         => AXI_STR_EMAC_TXC_ACLK,
         AXI_STR_TXC_ARESETN      => AXI_STR_EMAC_TXC_ARESETN,
         AXI_STR_TXC_TVALID       => AXI_STR_EMAC_TXC_TVALID,
         AXI_STR_TXC_TREADY       => AXI_STR_EMAC_TXC_TREADY,
         AXI_STR_TXC_TLAST        => AXI_STR_EMAC_TXC_TLAST,
         AXI_STR_TXC_TKEEP        => AXI_STR_EMAC_TXC_TKEEP,
         AXI_STR_TXC_TDATA        => AXI_STR_EMAC_TXC_TDATA,

         -- AXI streaming RX To EDMA --
         AXI_STR_RXD_ACLK         => AXI_STR_EDMA_RXD_ACLK,
         AXI_STR_RXD_ARESETN      => AXI_STR_EDMA_RXD_ARESETN,
         AXI_STR_RXD_TVALID       => AXI_STR_EDMA_RXD_TVALID,
         AXI_STR_RXD_TREADY       => AXI_STR_EDMA_RXD_TREADY,
         AXI_STR_RXD_TLAST        => AXI_STR_EDMA_RXD_TLAST,
         AXI_STR_RXD_TKEEP        => AXI_STR_EDMA_RXD_TKEEP,
         AXI_STR_RXD_TDATA        => AXI_STR_EDMA_RXD_TDATA,

         AXI_STR_RXS_ACLK         => AXI_STR_EDMA_RXS_ACLK,
         AXI_STR_RXS_ARESETN      => AXI_STR_EDMA_RXS_ARESETN,
         AXI_STR_RXS_TVALID       => AXI_STR_EDMA_RXS_TVALID,
         AXI_STR_RXS_TREADY       => AXI_STR_EDMA_RXS_TREADY,
         AXI_STR_RXS_TLAST        => AXI_STR_EDMA_RXS_TLAST,
         AXI_STR_RXS_TKEEP        => AXI_STR_EDMA_RXS_TKEEP,
         AXI_STR_RXS_TDATA        => AXI_STR_EDMA_RXS_TDATA,

         -- AXI output to RTDEx
         RTDEx_STR_RXD_ACLK       => Rtdex_rxd_aclk_s,
         RTDEx_STR_RXD_ARESETN    => Rtdex_rxd_aresetn_s,
         RTDEx_STR_RXD_TVALID     => Rtdex_rxd_tvalid_s,
         RTDEx_STR_RXD_TREADY     => Rtdex_rxd_tready_s,
         RTDEx_STR_RXD_TLAST      => Rtdex_rxd_tlast_s,
         RTDEx_STR_RXD_TKEEP      => Rtdex_rxd_tkeep_s,
         RTDEx_STR_RXD_TDATA      => v32_Rtdex_rxd_tdata_s,

         RTDEx_STR_RXS_ACLK       => Rtdex_rxs_aclk_s,
         RTDEx_STR_RXS_ARESETN    => Rtdex_rxs_aresetn_s,
         RTDEx_STR_RXS_TVALID     => Rtdex_rxs_tvalid_s,
         RTDEx_STR_RXS_TREADY     => Rtdex_rxs_tready_s,
         RTDEx_STR_RXS_TLAST      => Rtdex_rxs_tlast_s,
         RTDEx_STR_RXS_TKEEP      => Rtdex_rxs_tkeep_s,
         RTDEx_STR_RXS_TDATA      => v32_Rtdex_rxs_tdata_s
         );

 U0_RtdexRx: rtdex_rx
   generic map(
          NumberChannels_g 	=> C_RTDEX_RX_NUMER_OF_CHANNELS,
          C_RXMEM          	=> C_RX_CH_FIFO_DEPTH,
          C_RX_WIDTH_CH0    => C_RX_WIDTH_CH0,
          C_RX_WIDTH_CH1    => C_RX_WIDTH_CH1,
          C_RX_WIDTH_CH2    => C_RX_WIDTH_CH2,
          C_RX_WIDTH_CH3    => C_RX_WIDTH_CH3,
          C_RX_WIDTH_CH4    => C_RX_WIDTH_CH4,
          C_RX_WIDTH_CH5    => C_RX_WIDTH_CH5,
          C_RX_WIDTH_CH6    => C_RX_WIDTH_CH6,
          C_RX_WIDTH_CH7    => C_RX_WIDTH_CH7,
          StatsCntrs_g		=> C_RX_STATS_COUNTERS,
          FlowCtrlEn_g 		=> C_ENABLE_FLOW_CTRL,
          big_Endian_g		=> big_Endian_g
          )
   port map(
         -- config signals from/to
         iv8_RxStartNewTransfer_p   => iv8_RxStartNewTransfer_p,
         ov32_RxBadFrameCnt_p       => ov32_RxBadFrameCnt_p,
         ov32_RxFrameLostCntCh0_p   => ov32_RxFrameLostCntCh0_p,
         ov32_RxFrameLostCntCh1_p   => ov32_RxFrameLostCntCh1_p,
         ov32_RxFrameLostCntCh2_p   => ov32_RxFrameLostCntCh2_p,
         ov32_RxFrameLostCntCh3_p   => ov32_RxFrameLostCntCh3_p,
         ov32_RxFrameLostCntCh4_p   => ov32_RxFrameLostCntCh4_p,
         ov32_RxFrameLostCntCh5_p   => ov32_RxFrameLostCntCh5_p,
         ov32_RxFrameLostCntCh6_p   => ov32_RxFrameLostCntCh6_p,
         ov32_RxFrameLostCntCh7_p   => ov32_RxFrameLostCntCh7_p,
         iv8_RxFifoReset_p          => iv8_RxFifoReset_p,
         i_RxReset_p                => i_RxReset_p,
         ov32_RxConfigInfo_p        => ov32_RxConfigInfo_p,
         iv32_RxTimeout2DropFrm_p	=> iv32_RxTimeout2DropFrm_p,
         ov32_RcvdFrameCntCh0_p		=> ov32_RcvdFrameCntCh0_p,
		 ov32_RcvdFrameCntCh1_p		=> ov32_RcvdFrameCntCh1_p,
		 ov32_RcvdFrameCntCh2_p		=> ov32_RcvdFrameCntCh2_p,
		 ov32_RcvdFrameCntCh3_p		=> ov32_RcvdFrameCntCh3_p,
		 ov32_RcvdFrameCntCh4_p		=> ov32_RcvdFrameCntCh4_p,
		 ov32_RcvdFrameCntCh5_p		=> ov32_RcvdFrameCntCh5_p,
		 ov32_RcvdFrameCntCh6_p		=> ov32_RcvdFrameCntCh6_p,
		 ov32_RcvdFrameCntCh7_p		=> ov32_RcvdFrameCntCh7_p,
		 ov32_RxDropdFrmsCh0_p 		=> ov32_RxDropdFrmsCh0_p,
		 ov32_RxDropdFrmsCh1_p 		=> ov32_RxDropdFrmsCh1_p,
		 ov32_RxDropdFrmsCh2_p 		=> ov32_RxDropdFrmsCh2_p,
		 ov32_RxDropdFrmsCh3_p 		=> ov32_RxDropdFrmsCh3_p,
		 ov32_RxDropdFrmsCh4_p 		=> ov32_RxDropdFrmsCh4_p,
		 ov32_RxDropdFrmsCh5_p 		=> ov32_RxDropdFrmsCh5_p,
		 ov32_RxDropdFrmsCh6_p 		=> ov32_RxDropdFrmsCh6_p,
		 ov32_RxDropdFrmsCh7_p 		=> ov32_RxDropdFrmsCh7_p,
		 ov3_RxErrStatus_p			=> ov3_RxErrStatus_p,
		 iv32_RxFifoFullAssertThrCh0_p 	=> iv32_RxFifoFullAssertThrCh0_p,
		 iv32_RxFifoFullNegateThrCh0_p	=> iv32_RxFifoFullNegateThrCh0_p,
		 ov8_RxFifoUnderrun_p		=> ov8_RxFifoUnderrun_p,

         --- AXI STR side ---
         RTDEX_RXD_ACLK             => Rtdex_rxd_aclk_s,
         RTDEX_RXD_ARESET           => Rtdex_rxd_aresetn_s,
         RTDEX_RXD_TVALID           => Rtdex_rxd_tvalid_s,
         RTDEX_RXD_TREADY           => Rtdex_rxd_tready_s,
         RTDEX_RXD_TLAST            => Rtdex_rxd_tlast_s,
         RTDEX_RXD_TKEEP            => Rtdex_rxd_tkeep_s,
         V32_RTDEX_RXD_TDATA        => v32_Rtdex_rxd_tdata_s,

         RTDEX_RXS_ACLK             => Rtdex_rxs_aclk_s,
         RTDEX_RXS_ARESET           => Rtdex_rxs_aresetn_s,
         RTDEX_RXS_TVALID           => Rtdex_rxs_tvalid_s,
         RTDEX_RXS_TREADY           => Rtdex_rxs_tready_s,
         RTDEX_RXS_TLAST            => Rtdex_rxs_tlast_s,
         RTDEX_RXS_TKEEP            => Rtdex_rxs_tkeep_s,
         V32_RTDEX_RXS_TDATA        => v32_Rtdex_rxs_tdata_s,

         --
         o_SendPause_p				=> SendPause_s,
		 ov16_PauseVal_p			=> v16_PauseVal_s,

         --- User side ---
         i_RxUserClk_p              => i_RxUserClk_p,
         o_RxReadyCh0_p             => o_RxReadyCh0_p,
         i_RxReCh0_p                => i_RxReCh0_p,
         ov_RxDataCh0_p             => ov_RxDataCh0_p,
         o_RxDataValidCh0_p         => o_RxDataValidCh0_p,
         o_RxReadyCh1_p             => o_RxReadyCh1_p,
         i_RxReCh1_p                => i_RxReCh1_p,
         ov_RxDataCh1_p             => ov_RxDataCh1_p,
         o_RxDataValidCh1_p         => o_RxDataValidCh1_p,
         o_RxReadyCh2_p             => o_RxReadyCh2_p,
         i_RxReCh2_p                => i_RxReCh2_p,
         ov_RxDataCh2_p             => ov_RxDataCh2_p,
         o_RxDataValidCh2_p         => o_RxDataValidCh2_p,
         o_RxReadyCh3_p             => o_RxReadyCh3_p,
         i_RxReCh3_p                => i_RxReCh3_p,
         ov_RxDataCh3_p             => ov_RxDataCh3_p,
         o_RxDataValidCh3_p         => o_RxDataValidCh3_p,
         o_RxReadyCh4_p             => o_RxReadyCh4_p,
         i_RxReCh4_p                => i_RxReCh4_p,
         ov_RxDataCh4_p             => ov_RxDataCh4_p,
         o_RxDataValidCh4_p         => o_RxDataValidCh4_p,
         o_RxReadyCh5_p             => o_RxReadyCh5_p,
         i_RxReCh5_p                => i_RxReCh5_p,
         ov_RxDataCh5_p             => ov_RxDataCh5_p,
         o_RxDataValidCh5_p         => o_RxDataValidCh5_p,
         o_RxReadyCh6_p             => o_RxReadyCh6_p,
         i_RxReCh6_p                => i_RxReCh6_p,
         ov_RxDataCh6_p             => ov_RxDataCh6_p,
         o_RxDataValidCh6_p         => o_RxDataValidCh6_p,
         o_RxReadyCh7_p             => o_RxReadyCh7_p,
         i_RxReCh7_p                => i_RxReCh7_p,
         ov_RxDataCh7_p             => ov_RxDataCh7_p,
         o_RxDataValidCh7_p         => o_RxDataValidCh7_p
         );

  U0_rtdex_mux_tx: rtdex_mux_tx
   port map(
         -- AXI input from EDMA
         AXI_STR_TXD_ACLK         => AXI_STR_EDMA_TXD_ACLK,
         AXI_STR_TXD_ARESETN      => AXI_STR_EDMA_TXD_ARESETN,
         AXI_STR_TXD_TVALID       => AXI_STR_EDMA_TXD_TVALID,
         AXI_STR_TXD_TREADY       => AXI_STR_EDMA_TXD_TREADY,
         AXI_STR_TXD_TLAST        => AXI_STR_EDMA_TXD_TLAST,
         AXI_STR_TXD_TKEEP        => AXI_STR_EDMA_TXD_TKEEP,
         AXI_STR_TXD_TDATA        => AXI_STR_EDMA_TXD_TDATA,

         AXI_STR_TXC_ACLK         => AXI_STR_EDMA_TXC_ACLK,
         AXI_STR_TXC_ARESETN      => AXI_STR_EDMA_TXC_ARESETN,
         AXI_STR_TXC_TVALID       => AXI_STR_EDMA_TXC_TVALID,
         AXI_STR_TXC_TREADY       => AXI_STR_EDMA_TXC_TREADY,
         AXI_STR_TXC_TLAST        => AXI_STR_EDMA_TXC_TLAST,
         AXI_STR_TXC_TKEEP        => AXI_STR_EDMA_TXC_TKEEP,
         AXI_STR_TXC_TDATA        => AXI_STR_EDMA_TXC_TDATA,

         -- AXI input from RTDEx
         RTDEX_STR_TXD_ACLK       => rtdex_txd_aclk_s,
         RTDEX_STR_TXD_ARESETN    => rtdex_txd_aresetn_s,
         RTDEX_STR_TXD_TVALID     => rtdex_txd_tvalid_s,
         RTDEX_STR_TXD_TREADY     => rtdex_txd_tready_s,
         RTDEX_STR_TXD_TLAST      => rtdex_txd_tlast_s,
         RTDEX_STR_TXD_TKEEP      => rtdex_txd_tkeep_s,
         RTDEX_STR_TXD_TDATA      => rtdex_txd_tdata_s,

         RTDEX_STR_TXC_ACLK       => rtdex_txc_aclk_s,
         RTDEX_STR_TXC_ARESETN    => rtdex_txc_aresetn_s,
         RTDEX_STR_TXC_TVALID     => rtdex_txc_tvalid_s,
         RTDEX_STR_TXC_TREADY     => rtdex_txc_tready_s,
         RTDEX_STR_TXC_TLAST      => rtdex_txc_tlast_s,
         RTDEX_STR_TXC_TKEEP      => rtdex_txc_tkeep_s,
         RTDEX_STR_TXC_TDATA      => rtdex_txc_tdata_s,

         -- AXI input from Pause Frames request generator
         PFG_STR_TXD_ACLK         => pfg_txd_aclk_s   ,
         PFG_STR_TXD_ARESETN      => pfg_txd_aresetn_s,
         PFG_STR_TXD_TVALID       => pfg_txd_tvalid_s ,
         PFG_STR_TXD_TREADY       => pfg_txd_tready_s ,
         PFG_STR_TXD_TLAST        => pfg_txd_tlast_s  ,
         PFG_STR_TXD_TKEEP        => pfg_txd_tkeep_s  ,
         PFG_STR_TXD_TDATA        => pfg_txd_tdata_s  ,

         PFG_STR_TXC_ACLK         => pfg_txc_aclk_s   ,
         PFG_STR_TXC_ARESETN      => pfg_txc_aresetn_s,
         PFG_STR_TXC_TVALID       => pfg_txc_tvalid_s ,
         PFG_STR_TXC_TREADY       => pfg_txc_tready_s ,
         PFG_STR_TXC_TLAST        => pfg_txc_tlast_s  ,
         PFG_STR_TXC_TKEEP        => pfg_txc_tkeep_s  ,
         PFG_STR_TXC_TDATA        => pfg_txc_tdata_s  ,

         -- AXI output to EMAC
         AXI_STR_RXD_ACLK         => AXI_STR_EMAC_RXD_ACLK,
         AXI_STR_RXD_ARESETN      => AXI_STR_EMAC_RXD_ARESETN,
         AXI_STR_RXD_TVALID       => AXI_STR_EMAC_RXD_TVALID,
         AXI_STR_RXD_TREADY       => AXI_STR_EMAC_RXD_TREADY,
         AXI_STR_RXD_TLAST        => AXI_STR_EMAC_RXD_TLAST,
         AXI_STR_RXD_TKEEP        => AXI_STR_EMAC_RXD_TKEEP,
         AXI_STR_RXD_TDATA        => AXI_STR_EMAC_RXD_TDATA,

         AXI_STR_RXS_ACLK         => AXI_STR_EMAC_RXS_ACLK,
         AXI_STR_RXS_ARESETN      => AXI_STR_EMAC_RXS_ARESETN,
         AXI_STR_RXS_TVALID       => AXI_STR_EMAC_RXS_TVALID,
         AXI_STR_RXS_TREADY       => AXI_STR_EMAC_RXS_TREADY,
         AXI_STR_RXS_TLAST        => AXI_STR_EMAC_RXS_TLAST,
         AXI_STR_RXS_TKEEP        => AXI_STR_EMAC_RXS_TKEEP,
         AXI_STR_RXS_TDATA        => AXI_STR_EMAC_RXS_TDATA
         );

 FlowCTRL_gen: if C_ENABLE_FLOW_CTRL = 1 generate
	 U0_PauseReqGen : rtdex_pause_req_gen
	 	port map(
	 		-- Config side
	 		iv32_LocalEndMacAddrH_p  => iv32_LocalEndMacAddrH_p,
	        iv16_LocalEndMacAddrL_p  => iv16_LocalEndMacAddrL_p,
	 		-- Control side
	 		i_SendPause_p			 => SendPause_s,
	 		iv16_PauseVal_p			 => v16_PauseVal_s,
	 		--- AXI STR side ---
	         RTDEX_STR_TXD_ACLK       => pfg_txd_aclk_s   ,
	         RTDEX_STR_TXD_ARESETN    => pfg_txd_aresetn_s,
	         RTDEX_STR_TXD_TVALID     => pfg_txd_tvalid_s ,
	         RTDEX_STR_TXD_TREADY     => pfg_txd_tready_s ,
	         RTDEX_STR_TXD_TLAST      => pfg_txd_tlast_s  ,
	         RTDEX_STR_TXD_TKEEP      => pfg_txd_tkeep_s  ,
	         RTDEX_STR_TXD_TDATA      => pfg_txd_tdata_s  ,

	         RTDEX_STR_TXC_ACLK       => pfg_txc_aclk_s   ,
	         RTDEX_STR_TXC_ARESETN    => pfg_txc_aresetn_s,
	         RTDEX_STR_TXC_TVALID     => pfg_txc_tvalid_s ,
	         RTDEX_STR_TXC_TREADY     => pfg_txc_tready_s ,
	         RTDEX_STR_TXC_TLAST      => pfg_txc_tlast_s  ,
	         RTDEX_STR_TXC_TKEEP      => pfg_txc_tkeep_s  ,
	         RTDEX_STR_TXC_TDATA      => pfg_txc_tdata_s
	 		);
 end generate FlowCTRL_gen;

 NoFlowCTRL_gen: if C_ENABLE_FLOW_CTRL = 0 generate
 	pfg_txd_tvalid_s <= '0';
 	pfg_txd_tlast_s	 <= '0';
 	pfg_txd_tkeep_s	 <= (others=>'0');
 	pfg_txd_tdata_s	 <= (others=>'0');
 	pfg_txc_tvalid_s <= '0';
 	pfg_txc_tlast_s	 <= '0';
 	pfg_txc_tkeep_s	 <= (others=>'0');
 	pfg_txc_tdata_s	 <= (others=>'0');
 end generate NoFlowCTRL_gen;

 U0_rtdex_tx: rtdex_tx
   generic map(
          NumberChannels_g       => C_RTDEX_TX_NUMER_OF_CHANNELS,
          C_TXMEM                => C_TX_CH_FIFO_DEPTH,
          C_TX_WIDTH_CH0         => C_TX_WIDTH_CH0,
          C_TX_WIDTH_CH1         => C_TX_WIDTH_CH1,
          C_TX_WIDTH_CH2         => C_TX_WIDTH_CH2,
          C_TX_WIDTH_CH3         => C_TX_WIDTH_CH3,
          C_TX_WIDTH_CH4         => C_TX_WIDTH_CH4,
          C_TX_WIDTH_CH5         => C_TX_WIDTH_CH5,
          C_TX_WIDTH_CH6         => C_TX_WIDTH_CH6,
          C_TX_WIDTH_CH7         => C_TX_WIDTH_CH7,
          SuportJumboFrames_g	 => C_SUPPORT_JUMBO_FRM,
          StatsCntrs_g		  	 => C_TX_STATS_COUNTERS,
          big_Endian_g			 => big_Endian_g
          )
   port map(
         -- config & status signals from/to axi registers --
         iv32_RemoteEndMacAddrH_p => iv32_RemoteEndMacAddrH_p,
         iv16_RemoteEndMacAddrL_p => iv16_RemoteEndMacAddrL_p,
         iv32_LocalEndMacAddrH_p  => iv32_LocalEndMacAddrH_p,
         iv16_LocalEndMacAddrL_p  => iv16_LocalEndMacAddrL_p,
         ov32_TxConfigInfo_p      => ov32_TxConfigInfo_p,
         iv32_FrameGap_p          => iv32_FrameGap_p,
         i_Mode_p				  => i_Mode_p,
         iv8_TxChFrsBurst_p		  => iv8_TxChFrsBurst_p,

         iv15_FrameSizeCh0_p      => iv15_FrameSizeCh0_p,
         iv32_TransferSizeCh0_p   => iv32_TransferSizeCh0_p,

         iv15_FrameSizeCh1_p      => iv15_FrameSizeCh1_p,
         iv32_TransferSizeCh1_p   => iv32_TransferSizeCh1_p,

         iv15_FrameSizeCh2_p      => iv15_FrameSizeCh2_p,
         iv32_TransferSizeCh2_p   => iv32_TransferSizeCh2_p,

         iv15_FrameSizeCh3_p      => iv15_FrameSizeCh3_p,
         iv32_TransferSizeCh3_p   => iv32_TransferSizeCh3_p,

         iv15_FrameSizeCh4_p      => iv15_FrameSizeCh4_p,
         iv32_TransferSizeCh4_p   => iv32_TransferSizeCh4_p,

         iv15_FrameSizeCh5_p      => iv15_FrameSizeCh5_p,
         iv32_TransferSizeCh5_p   => iv32_TransferSizeCh5_p,

         iv15_FrameSizeCh6_p      => iv15_FrameSizeCh6_p,
         iv32_TransferSizeCh6_p   => iv32_TransferSizeCh6_p,

         iv15_FrameSizeCh7_p      => iv15_FrameSizeCh7_p,
         iv32_TransferSizeCh7_p   => iv32_TransferSizeCh7_p,

         i_TxReset_p              => i_TxReset_p,
         iv8_TxFifoReset_p        => iv8_TxFifoReset_p,
         iv8_StartNewTransfer_p   => iv8_TxStartNewTransfer_p,
         ov8_TxFifoOverrun_p	  => ov8_TxFifoOverrun_p,

         --- AXI STR side ---
         RTDEX_STR_TXD_ACLK       => rtdex_txd_aclk_s,
         RTDEX_STR_TXD_ARESETN    => rtdex_txd_aresetn_s,
         RTDEX_STR_TXD_TVALID     => rtdex_txd_tvalid_s,
         RTDEX_STR_TXD_TREADY     => rtdex_txd_tready_s,
         RTDEX_STR_TXD_TLAST      => rtdex_txd_tlast_s,
         RTDEX_STR_TXD_TKEEP      => rtdex_txd_tkeep_s,
         RTDEX_STR_TXD_TDATA      => rtdex_txd_tdata_s,

         RTDEX_STR_TXC_ACLK       => rtdex_txc_aclk_s,
         RTDEX_STR_TXC_ARESETN    => rtdex_txc_aresetn_s,
         RTDEX_STR_TXC_TVALID     => rtdex_txc_tvalid_s,
         RTDEX_STR_TXC_TREADY     => rtdex_txc_tready_s,
         RTDEX_STR_TXC_TLAST      => rtdex_txc_tlast_s,
         RTDEX_STR_TXC_TKEEP      => rtdex_txc_tkeep_s,
         RTDEX_STR_TXC_TDATA      => rtdex_txc_tdata_s,

         --- User side ---
         i_TxUserClk_p            => i_TxUserClk_p,

         i_TxWeCh0_p              => i_TxWeCh0_p,
         o_TxReadyCh0_p           => o_TxReadyCh0_p,
         iv_TxDataCh0_p           => iv_TxDataCh0_p,
         ov32_TxFifoCountCh0_p    => ov32_TxFifoCountCh0_p,
         ov32_NbrSentFramesCh0_p  => ov32_NbrSentFramesCh0_p,

         i_TxWeCh1_p              => i_TxWeCh1_p,
         o_TxReadyCh1_p           => o_TxReadyCh1_p,
         iv_TxDataCh1_p           => iv_TxDataCh1_p,
         ov32_TxFifoCountCh1_p    => ov32_TxFifoCountCh1_p,
         ov32_NbrSentFramesCh1_p  => ov32_NbrSentFramesCh1_p,

         i_TxWeCh2_p              => i_TxWeCh2_p,
         o_TxReadyCh2_p           => o_TxReadyCh2_p,
         iv_TxDataCh2_p           => iv_TxDataCh2_p,
         ov32_TxFifoCountCh2_p    => ov32_TxFifoCountCh2_p,
         ov32_NbrSentFramesCh2_p  => ov32_NbrSentFramesCh2_p,

         i_TxWeCh3_p              => i_TxWeCh3_p,
         o_TxReadyCh3_p           => o_TxReadyCh3_p,
         iv_TxDataCh3_p           => iv_TxDataCh3_p,
         ov32_TxFifoCountCh3_p    => ov32_TxFifoCountCh3_p,
         ov32_NbrSentFramesCh3_p  => ov32_NbrSentFramesCh3_p,

         i_TxWeCh4_p              => i_TxWeCh4_p,
         o_TxReadyCh4_p           => o_TxReadyCh4_p,
         iv_TxDataCh4_p           => iv_TxDataCh4_p,
         ov32_TxFifoCountCh4_p    => ov32_TxFifoCountCh4_p,
         ov32_NbrSentFramesCh4_p  => ov32_NbrSentFramesCh4_p,

         i_TxWeCh5_p              => i_TxWeCh5_p,
         o_TxReadyCh5_p           => o_TxReadyCh5_p,
         iv_TxDataCh5_p           => iv_TxDataCh5_p,
         ov32_TxFifoCountCh5_p    => ov32_TxFifoCountCh5_p,
         ov32_NbrSentFramesCh5_p  => ov32_NbrSentFramesCh5_p,

         i_TxWeCh6_p              => i_TxWeCh6_p,
         o_TxReadyCh6_p           => o_TxReadyCh6_p,
         iv_TxDataCh6_p           => iv_TxDataCh6_p,
         ov32_TxFifoCountCh6_p    => ov32_TxFifoCountCh6_p,
         ov32_NbrSentFramesCh6_p  => ov32_NbrSentFramesCh6_p,

         i_TxWeCh7_p              => i_TxWeCh7_p,
         o_TxReadyCh7_p           => o_TxReadyCh7_p,
         iv_TxDataCh7_p           => iv_TxDataCh7_p,
         ov32_TxFifoCountCh7_p    => ov32_TxFifoCountCh7_p,
         ov32_NbrSentFramesCh7_p  => ov32_NbrSentFramesCh7_p
        );

end architecture emac_rtdex_top_behav;
