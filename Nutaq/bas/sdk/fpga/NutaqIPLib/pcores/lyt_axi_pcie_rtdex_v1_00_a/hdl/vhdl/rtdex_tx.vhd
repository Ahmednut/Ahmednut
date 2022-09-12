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
-- Description: Go from FIFO interfaces to AXI streams
-- David Quinn
-- 2012/11
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

entity rtdex_tx is
  generic(         
    NumberChannels_g 		      : integer range 0 to 8  := 1;
    
    -- The DEPTH parameter specify the FIFO depth in number of 4-byte samples.
    WRITE_DEPTH_g                 : integer               := 2048;
    
    C_TX_WIDTH_CH0                : integer := 32;
    C_TX_WIDTH_CH1                : integer := 32;
    C_TX_WIDTH_CH2                : integer := 32;
    C_TX_WIDTH_CH3                : integer := 32;
    C_TX_WIDTH_CH4                : integer := 32;
    C_TX_WIDTH_CH5                : integer := 32;
    C_TX_WIDTH_CH6                : integer := 32;
    C_TX_WIDTH_CH7                : integer := 32
  );
  port(
    ov8_intReq_p                  : out std_logic_vector(7 downto 0);
    
    -- config & status signals from/to axi registers --
    Bus2IP_Clk                    : in  std_logic;
    
    i_TxReset_p             	    : in  std_logic;
    iv8_TxFifoReset_p       	    : in  std_logic_vector(7 downto 0);
    iv8_TxFifoWrEn_p              : in  std_logic_vector(7 downto 0);
    ov8_overflow_p                : out std_logic_vector(7 downto 0);
    ov8_underflow_p               : out std_logic_vector(7 downto 0);
    iv8_streamingTransfer_p	      : in  std_logic_vector(7 downto 0);
    iv8_StartNewTransfer_p  	    : in  std_logic_vector(7 downto 0);
    ov8_transferDone_p  	        : out std_logic_vector(7 downto 0);
    iv8_irqLastTransferEn_p	      : in  std_logic_vector(7 downto 0);
    ov8_TxDataMoverHaltCmplt_p    : out std_logic_vector(7 downto 0);
    ov8_TxDataMoverErr_p          : out std_logic_vector(7 downto 0);
    iv8_TxDataMoverRst_p          : in  std_logic_vector(7 downto 0);
    iv8_TxDataMoverHaltReq_p      : in  std_logic_vector(7 downto 0);
    ov32_TxConfiginfo_p     	    : out std_logic_vector(31 downto 0);
    
    iv32_dataMoverCtrlCh0_p  	    : in std_logic_vector(31 downto 0);
    iv32_dataMoverAddrCh0_p       : in std_logic_vector(31 downto 0);
    iv4_dataMoverTagCh0_p         : in std_logic_vector(3 downto 0);
    ov8_dataMoverStatusCh0_p      : out std_logic_vector(7 downto 0);
    iv24_transferCntCh0_p   	    : in std_logic_vector(23 downto 0);
    ov24_currentTransferCntCh0_p  : out std_logic_vector(23 downto 0);
        
    iv32_dataMoverCtrlCh1_p  	    : in std_logic_vector(31 downto 0);
    iv32_dataMoverAddrCh1_p       : in std_logic_vector(31 downto 0);
    iv4_dataMoverTagCh1_p         : in std_logic_vector(3 downto 0);
    ov8_dataMoverStatusCh1_p      : out std_logic_vector(7 downto 0);
    iv24_transferCntCh1_p   	    : in std_logic_vector(23 downto 0);
    ov24_currentTransferCntCh1_p  : out std_logic_vector(23 downto 0);
    
    iv32_dataMoverCtrlCh2_p  	    : in std_logic_vector(31 downto 0);
    iv32_dataMoverAddrCh2_p       : in std_logic_vector(31 downto 0);
    iv4_dataMoverTagCh2_p         : in std_logic_vector(3 downto 0);
    ov8_dataMoverStatusCh2_p      : out std_logic_vector(7 downto 0);
    iv24_transferCntCh2_p   	    : in std_logic_vector(23 downto 0);
    ov24_currentTransferCntCh2_p  : out std_logic_vector(23 downto 0);
    
    iv32_dataMoverCtrlCh3_p  	    : in std_logic_vector(31 downto 0);
    iv32_dataMoverAddrCh3_p       : in std_logic_vector(31 downto 0);
    iv4_dataMoverTagCh3_p         : in std_logic_vector(3 downto 0);
    ov8_dataMoverStatusCh3_p      : out std_logic_vector(7 downto 0);
    iv24_transferCntCh3_p   	    : in std_logic_vector(23 downto 0);
    ov24_currentTransferCntCh3_p  : out std_logic_vector(23 downto 0);
    
    iv32_dataMoverCtrlCh4_p  	    : in std_logic_vector(31 downto 0);
    iv32_dataMoverAddrCh4_p       : in std_logic_vector(31 downto 0);
    iv4_dataMoverTagCh4_p         : in std_logic_vector(3 downto 0);
    ov8_dataMoverStatusCh4_p      : out std_logic_vector(7 downto 0);
    iv24_transferCntCh4_p   	    : in std_logic_vector(23 downto 0);
    ov24_currentTransferCntCh4_p  : out std_logic_vector(23 downto 0);
    
    iv32_dataMoverCtrlCh5_p  	    : in std_logic_vector(31 downto 0);
    iv32_dataMoverAddrCh5_p       : in std_logic_vector(31 downto 0);
    iv4_dataMoverTagCh5_p         : in std_logic_vector(3 downto 0);
    ov8_dataMoverStatusCh5_p      : out std_logic_vector(7 downto 0);
    iv24_transferCntCh5_p   	    : in std_logic_vector(23 downto 0);
    ov24_currentTransferCntCh5_p  : out std_logic_vector(23 downto 0);
    
    iv32_dataMoverCtrlCh6_p  	    : in std_logic_vector(31 downto 0);
    iv32_dataMoverAddrCh6_p       : in std_logic_vector(31 downto 0);
    iv4_dataMoverTagCh6_p         : in std_logic_vector(3 downto 0);
    ov8_dataMoverStatusCh6_p      : out std_logic_vector(7 downto 0);
    iv24_transferCntCh6_p   	    : in std_logic_vector(23 downto 0);
    ov24_currentTransferCntCh6_p  : out std_logic_vector(23 downto 0);
    
    iv32_dataMoverCtrlCh7_p  	    : in std_logic_vector(31 downto 0);
    iv32_dataMoverAddrCh7_p       : in std_logic_vector(31 downto 0);
    iv4_dataMoverTagCh7_p         : in std_logic_vector(3 downto 0);
    ov8_dataMoverStatusCh7_p      : out std_logic_vector(7 downto 0);
    iv24_transferCntCh7_p   	    : in std_logic_vector(23 downto 0);
    ov24_currentTransferCntCh7_p  : out std_logic_vector(23 downto 0);  
      
    --- User side ---
    i_TxUserClk_p                 : in std_logic;
                                  
    i_TxWeCh0_p                   : in std_logic;
    o_TxReadyCh0_p                : out std_logic;
    iv_TxDataCh0_p                : in std_logic_vector(C_TX_WIDTH_CH0-1 downto 0);
                                        
    i_TxWeCh1_p                   : in std_logic;
    o_TxReadyCh1_p                : out std_logic;
    iv_TxDataCh1_p                : in std_logic_vector(C_TX_WIDTH_CH1-1 downto 0);
                                        
    i_TxWeCh2_p                   : in std_logic;
    o_TxReadyCh2_p                : out std_logic;
    iv_TxDataCh2_p                : in std_logic_vector(C_TX_WIDTH_CH2-1 downto 0);
                                        
    i_TxWeCh3_p                   : in std_logic;
    o_TxReadyCh3_p                : out std_logic;
    iv_TxDataCh3_p                : in std_logic_vector(C_TX_WIDTH_CH3-1 downto 0);
                                        
    i_TxWeCh4_p                   : in std_logic;
    o_TxReadyCh4_p                : out std_logic;
    iv_TxDataCh4_p                : in std_logic_vector(C_TX_WIDTH_CH4-1 downto 0);
                                        
    i_TxWeCh5_p                   : in std_logic;
    o_TxReadyCh5_p                : out std_logic;
    iv_TxDataCh5_p                : in std_logic_vector(C_TX_WIDTH_CH5-1 downto 0);
                                        
    i_TxWeCh6_p                   : in std_logic;
    o_TxReadyCh6_p                : out std_logic;
    iv_TxDataCh6_p                : in std_logic_vector(C_TX_WIDTH_CH6-1 downto 0);
                                        
    i_TxWeCh7_p                   : in std_logic;
    o_TxReadyCh7_p                : out std_logic;
    iv_TxDataCh7_p                : in std_logic_vector(C_TX_WIDTH_CH7-1 downto 0);

    -- AXI Memory interface (Channel 0)
    m_axi_s2mm_Ch0_aclk           : in std_logic;
    m_axi_s2mm_Ch0_aresetn        : in std_logic;
    m_axi_s2mm_Ch0_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch0_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch0_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch0_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch0_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch0_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch0_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch0_awvalid        : out std_logic;
    m_axi_s2mm_Ch0_awready        : in std_logic;
    m_axi_s2mm_Ch0_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch0_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch0_wlast          : out std_logic;
    m_axi_s2mm_Ch0_wvalid         : out std_logic;
    m_axi_s2mm_Ch0_wready         : in std_logic;
    m_axi_s2mm_Ch0_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch0_bvalid         : in std_logic;
    m_axi_s2mm_Ch0_bready         : out std_logic;
    
    -- AXI Memory interface (Channel 1)
    m_axi_s2mm_Ch1_aclk           : in std_logic;
    m_axi_s2mm_Ch1_aresetn        : in std_logic;
    m_axi_s2mm_Ch1_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch1_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch1_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch1_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch1_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch1_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch1_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch1_awvalid        : out std_logic;
    m_axi_s2mm_Ch1_awready        : in std_logic;
    m_axi_s2mm_Ch1_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch1_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch1_wlast          : out std_logic;
    m_axi_s2mm_Ch1_wvalid         : out std_logic;
    m_axi_s2mm_Ch1_wready         : in std_logic;
    m_axi_s2mm_Ch1_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch1_bvalid         : in std_logic;
    m_axi_s2mm_Ch1_bready         : out std_logic;
    
    -- AXI Memory interface (Channel 2)
    m_axi_s2mm_Ch2_aclk           : in std_logic;
    m_axi_s2mm_Ch2_aresetn        : in std_logic;
    m_axi_s2mm_Ch2_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch2_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch2_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch2_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch2_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch2_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch2_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch2_awvalid        : out std_logic;
    m_axi_s2mm_Ch2_awready        : in std_logic;
    m_axi_s2mm_Ch2_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch2_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch2_wlast          : out std_logic;
    m_axi_s2mm_Ch2_wvalid         : out std_logic;
    m_axi_s2mm_Ch2_wready         : in std_logic;
    m_axi_s2mm_Ch2_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch2_bvalid         : in std_logic;
    m_axi_s2mm_Ch2_bready         : out std_logic;
    
    -- AXI Memory interface (Channel 3)
    m_axi_s2mm_Ch3_aclk           : in std_logic;
    m_axi_s2mm_Ch3_aresetn        : in std_logic;
    m_axi_s2mm_Ch3_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch3_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch3_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch3_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch3_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch3_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch3_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch3_awvalid        : out std_logic;
    m_axi_s2mm_Ch3_awready        : in std_logic;
    m_axi_s2mm_Ch3_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch3_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch3_wlast          : out std_logic;
    m_axi_s2mm_Ch3_wvalid         : out std_logic;
    m_axi_s2mm_Ch3_wready         : in std_logic;
    m_axi_s2mm_Ch3_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch3_bvalid         : in std_logic;
    m_axi_s2mm_Ch3_bready         : out std_logic;
    
    -- AXI Memory interface (Channel 4)
    m_axi_s2mm_Ch4_aclk           : in std_logic;
    m_axi_s2mm_Ch4_aresetn        : in std_logic;
    m_axi_s2mm_Ch4_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch4_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch4_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch4_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch4_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch4_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch4_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch4_awvalid        : out std_logic;
    m_axi_s2mm_Ch4_awready        : in std_logic;
    m_axi_s2mm_Ch4_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch4_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch4_wlast          : out std_logic;
    m_axi_s2mm_Ch4_wvalid         : out std_logic;
    m_axi_s2mm_Ch4_wready         : in std_logic;
    m_axi_s2mm_Ch4_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch4_bvalid         : in std_logic;
    m_axi_s2mm_Ch4_bready         : out std_logic;
    
    -- AXI Memory interface (Channel 5)
    m_axi_s2mm_Ch5_aclk           : in std_logic;
    m_axi_s2mm_Ch5_aresetn        : in std_logic;
    m_axi_s2mm_Ch5_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch5_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch5_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch5_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch5_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch5_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch5_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch5_awvalid        : out std_logic;
    m_axi_s2mm_Ch5_awready        : in std_logic;
    m_axi_s2mm_Ch5_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch5_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch5_wlast          : out std_logic;
    m_axi_s2mm_Ch5_wvalid         : out std_logic;
    m_axi_s2mm_Ch5_wready         : in std_logic;
    m_axi_s2mm_Ch5_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch5_bvalid         : in std_logic;
    m_axi_s2mm_Ch5_bready         : out std_logic;
    
    -- AXI Memory interface (Channel 6)
    m_axi_s2mm_Ch6_aclk           : in std_logic;
    m_axi_s2mm_Ch6_aresetn        : in std_logic;
    m_axi_s2mm_Ch6_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch6_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch6_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch6_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch6_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch6_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch6_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch6_awvalid        : out std_logic;
    m_axi_s2mm_Ch6_awready        : in std_logic;
    m_axi_s2mm_Ch6_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch6_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch6_wlast          : out std_logic;
    m_axi_s2mm_Ch6_wvalid         : out std_logic;
    m_axi_s2mm_Ch6_wready         : in std_logic;
    m_axi_s2mm_Ch6_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch6_bvalid         : in std_logic;
    m_axi_s2mm_Ch6_bready         : out std_logic;
    
    -- AXI Memory interface (Channel 7)
    m_axi_s2mm_Ch7_aclk           : in std_logic;
    m_axi_s2mm_Ch7_aresetn        : in std_logic;
    m_axi_s2mm_Ch7_awid           : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch7_awaddr         : out std_logic_vector(31 downto 0);
    m_axi_s2mm_Ch7_awlen          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch7_awsize         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch7_awburst        : out std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch7_awprot         : out std_logic_vector(2 downto 0);
    m_axi_s2mm_Ch7_awcache        : out std_logic_vector(3 downto 0);
    m_axi_s2mm_Ch7_awvalid        : out std_logic;
    m_axi_s2mm_Ch7_awready        : in std_logic;
    m_axi_s2mm_Ch7_wdata          : out std_logic_vector(63 downto 0);
    m_axi_s2mm_Ch7_wstrb          : out std_logic_vector(7 downto 0);
    m_axi_s2mm_Ch7_wlast          : out std_logic;
    m_axi_s2mm_Ch7_wvalid         : out std_logic;
    m_axi_s2mm_Ch7_wready         : in std_logic;
    m_axi_s2mm_Ch7_bresp          : in std_logic_vector(1 downto 0);
    m_axi_s2mm_Ch7_bvalid         : in std_logic;
    m_axi_s2mm_Ch7_bready         : out std_logic
  );
end rtdex_tx;

architecture rtdex_tx_behav of rtdex_tx is
  
  attribute keep_hierarchy : string;
  attribute keep_hierarchy of rtdex_tx_behav: architecture is "true";

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
  constant A8I_TX_WIDTH : array8_int_t :=
    (C_TX_WIDTH_CH0,
     C_TX_WIDTH_CH1,
     C_TX_WIDTH_CH2,
     C_TX_WIDTH_CH3,
     C_TX_WIDTH_CH4,
     C_TX_WIDTH_CH5,
     C_TX_WIDTH_CH6,
     C_TX_WIDTH_CH7);
    
  type array8_v1_t  is array(7 downto 0) of std_logic_vector(0 downto 0);
  type array8_v2_t  is array(7 downto 0) of std_logic_vector(1 downto 0);
  type array8_v3_t  is array(7 downto 0) of std_logic_vector(2 downto 0);
  type array8_v4_t  is array(7 downto 0) of std_logic_vector(3 downto 0);
  type array8_v8_t  is array(7 downto 0) of std_logic_vector(7 downto 0);
  type array8_v15_t is array(7 downto 0) of std_logic_vector(14 downto 0);
  type array8_v24_t is array(7 downto 0) of std_logic_vector(23 downto 0);
  type array8_v32_t is array(7 downto 0) of std_logic_vector(31 downto 0);
  type array8_v64_t is array(7 downto 0) of std_logic_vector(63 downto 0);
  type array8_v72_t is array(7 downto 0) of std_logic_vector(71 downto 0);
  type array8_v256_t is array(7 downto 0) of std_logic_vector(255 downto 0);

  signal a8v32_dataMoverCtrl_s  	: array8_v32_t;
  signal a8v32_dataMoverAddr_s    : array8_v32_t;
  signal a8v4_dataMoverTag_s      : array8_v4_t;
  signal a8v8_dataMoverStatus_s   : array8_v8_t;
  
  signal a8v24_transferCnt_s      : array8_v24_t;
  signal a8v24_currentTransferCnt_s : array8_v24_t;
  
  signal a8v256_TxDataCh_s         : array8_v256_t;
  signal a8v256_FifoDin_s          : array8_v256_t;
  
  
  signal v8_TxReady_s             : std_logic_vector(7 downto 0);
  signal v8_TxWe_s                : std_logic_vector(7 downto 0);
    
  signal v6_FifoDepthinfo_s 		  : std_logic_vector(5 downto 0);

  -- AXI control and status streaming channel
  signal v8_AXI_STR_CTRL_STAT_ACLK_s : std_logic_vector(7 downto 0);   
  signal v8_AXI_STR_CTRL_STAT_ARESETN_s : std_logic_vector(7 downto 0);   
  signal v8_AXI_STR_CTRL_TVALID_s : std_logic_vector(7 downto 0);   
  signal v8_AXI_STR_CTRL_TREADY_s : std_logic_vector(7 downto 0);   
  signal a8v72_AXI_STR_CTRL_TDATA_s : array8_v72_t;
  signal v8_AXI_STR_STAT_TVALID_s : std_logic_vector(7 downto 0);
  signal v8_AXI_STR_STAT_TREADY_s : std_logic_vector(7 downto 0);
  signal a8v8_AXI_STR_STAT_TDATA_s : array8_v8_t;
  signal a8v1_AXI_STR_STAT_TKEEP_s : array8_v1_t;
  signal v8_AXI_STR_STAT_TLAST_s  : std_logic_vector(7 downto 0);
      
  -- AXI streaming (Channels 0 to 7)
  signal v8_AXI_STR_TVALID_s      : std_logic_vector(7 downto 0);
  signal v8_AXI_STR_TREADY_s      : std_logic_vector(7 downto 0);
  signal v8_AXI_STR_TLAST_s       : std_logic_vector(7 downto 0);
  signal a8v8_AXI_STR_TKEEP_s     : array8_v8_t;
  signal a8v64_AXI_STR_TDATA_s    : array8_v64_t;

  -- Address posting
  signal v8_s2mm_allow_addr_req_s    : std_logic_vector(7 downto 0);
  signal v8_s2mm_addr_req_posted_s   : std_logic_vector(7 downto 0);
  signal v8_s2mm_wr_xfer_cmplt_s     : std_logic_vector(7 downto 0);
  signal v8_s2mm_ld_nxt_len_s        : std_logic_vector(7 downto 0);
  signal a8v8_s2mm_wr_len_s          : array8_v8_t;
    
  component fifo_to_axi_stream is
    generic(         
      WRITE_DEPTH_g                 : integer               := 2048;
      C_TX_WIDTH                    : integer := 32
    );
    port(
      i_strClk_p                    : in  std_logic;
      o_intReq_p                    : out std_logic;
      
      -- config & status signals from/to axi registers --
      Bus2IP_Clk                    : in  std_logic;
      
      i_TxReset_p             	    : in  std_logic;
      i_TxFifoReset_p       	      : in  std_logic;
      i_TxFifoWrEn_p                : in  std_logic;
      o_overflow_p                  : out std_logic;
      o_underflow_p                 : out std_logic;
      i_streamingTransfer_p	        : in  std_logic;
      i_StartNewTransfer_p  	      : in  std_logic;
      o_transferDone_p  	          : out std_logic;
      
      i_irqLastTransferEn_p	        : in  std_logic;

      iv32_dataMoverCtrl_p  	      : in std_logic_vector(31 downto 0);
      iv32_dataMoverAddr_p          : in std_logic_vector(31 downto 0);
      iv4_dataMoverTag_p            : in std_logic_vector(3 downto 0);
      ov8_dataMoverStatus_p         : out std_logic_vector(7 downto 0);
      
      iv24_transferCnt_p   	        : in std_logic_vector(23 downto 0);
      ov24_currentTransferCnt_p     : out std_logic_vector(23 downto 0);
      
      -- AXI Data streaming channel
      AXI_STR_TVALID                : out std_logic;
      AXI_STR_TREADY                : in  std_logic;
      AXI_STR_TLAST                 : out std_logic;
      AXI_STR_TKEEP                 : out std_logic_vector(7 downto 0);
      AXI_STR_TDATA                 : out std_logic_vector(63 downto 0);

      -- AXI control and status streaming channel
      AXI_STR_CTRL_STAT_ACLK        : out  std_logic;
      AXI_STR_CTRL_STAT_ARESETN     : out  std_logic;
      AXI_STR_CTRL_TVALID           : out std_logic;
      AXI_STR_CTRL_TREADY           : in  std_logic;
      AXI_STR_CTRL_TDATA            : out std_logic_vector(71 downto 0);
      AXI_STR_STAT_TVALID           : in  std_logic;
      AXI_STR_STAT_TREADY           : out std_logic;
      AXI_STR_STAT_TDATA            : in  std_logic_vector(7 downto 0);   
      AXI_STR_STAT_TKEEP            : in  std_logic_vector(0 downto 0);
      AXI_STR_STAT_TLAST            : in  std_logic;

      -- Address posting
      s2mm_allow_addr_req           : out std_logic;
      s2mm_addr_req_posted          : in  std_logic;
      s2mm_wr_xfer_cmplt            : in  std_logic;
      s2mm_ld_nxt_len               : in  std_logic;
      s2mm_wr_len                   : in  std_logic_vector(7 downto 0);
    
       -- User side (TX FIFO)
      i_TxUserClk_p                 : in std_logic;
      i_TxWe_p                      : in std_logic;
      o_TxReady_p                   : out std_logic;
      iv_TxData_p                   : in std_logic_vector(C_TX_WIDTH-1 downto 0)
    );
  end component fifo_to_axi_stream;

  component axi_datamover_v3_00_a
    port (
      -- AXI Memory to stream interface (unused)
      --
      m_axi_mm2s_aclk               : in std_logic;
      m_axi_mm2s_aresetn            : in std_logic;
      mm2s_halt                     : in std_logic;
      mm2s_halt_cmplt               : out std_logic;
      mm2s_err                      : out std_logic;
      m_axis_mm2s_cmdsts_aclk       : in std_logic;
      m_axis_mm2s_cmdsts_aresetn    : in std_logic;
      s_axis_mm2s_cmd_tvalid        : in std_logic;
      s_axis_mm2s_cmd_tready        : out std_logic;
      s_axis_mm2s_cmd_tdata         : in std_logic_vector(71 downto 0);
      m_axis_mm2s_sts_tvalid        : out std_logic;
      m_axis_mm2s_sts_tready        : in std_logic;
      m_axis_mm2s_sts_tdata         : out std_logic_vector(7 downto 0);
      m_axis_mm2s_sts_tkeep         : out std_logic_vector(0 downto 0);
      m_axis_mm2s_sts_tlast         : out std_logic;
      mm2s_allow_addr_req           : in std_logic;
      mm2s_addr_req_posted          : out std_logic;
      mm2s_rd_xfer_cmplt            : out std_logic;
      m_axi_mm2s_arid               : out std_logic_vector(3 downto 0);
      m_axi_mm2s_araddr             : out std_logic_vector(31 downto 0);
      m_axi_mm2s_arlen              : out std_logic_vector(7 downto 0);
      m_axi_mm2s_arsize             : out std_logic_vector(2 downto 0);
      m_axi_mm2s_arburst            : out std_logic_vector(1 downto 0);
      m_axi_mm2s_arprot             : out std_logic_vector(2 downto 0);
      m_axi_mm2s_arcache            : out std_logic_vector(3 downto 0);
      m_axi_mm2s_arvalid            : out std_logic;
      m_axi_mm2s_arready            : in std_logic;
      m_axi_mm2s_rdata              : in std_logic_vector(31 downto 0);
      m_axi_mm2s_rresp              : in std_logic_vector(1 downto 0);
      m_axi_mm2s_rlast              : in std_logic;
      m_axi_mm2s_rvalid             : in std_logic;
      m_axi_mm2s_rready             : out std_logic;
      m_axis_mm2s_tdata             : out std_logic_vector(31 downto 0);
      m_axis_mm2s_tkeep             : out std_logic_vector(3 downto 0);
      m_axis_mm2s_tlast             : out std_logic;
      m_axis_mm2s_tvalid            : out std_logic;
      m_axis_mm2s_tready            : in std_logic;
      mm2s_dbg_sel                  : in std_logic_vector(3 downto 0);
      mm2s_dbg_data                 : out std_logic_vector(31 downto 0);
      
      -- AXI Stream to memory interface
      --
      
      -- General Control/Status signals
      s2mm_halt                     : in std_logic;
      s2mm_halt_cmplt               : out std_logic;
      s2mm_err                      : out std_logic;
      
      -- AXI control and status streaming
      m_axis_s2mm_cmdsts_awclk      : in std_logic;
      m_axis_s2mm_cmdsts_aresetn    : in std_logic;
      s_axis_s2mm_cmd_tvalid        : in std_logic;
      s_axis_s2mm_cmd_tready        : out std_logic;
      s_axis_s2mm_cmd_tdata         : in std_logic_vector(71 downto 0);
      m_axis_s2mm_sts_tvalid        : out std_logic;
      m_axis_s2mm_sts_tready        : in std_logic;
      m_axis_s2mm_sts_tdata         : out std_logic_vector(7 downto 0);
      m_axis_s2mm_sts_tkeep         : out std_logic_vector(0 downto 0);
      m_axis_s2mm_sts_tlast         : out std_logic;
      
      -- Address posting
      s2mm_allow_addr_req           : in std_logic;
      s2mm_addr_req_posted          : out std_logic;
      s2mm_wr_xfer_cmplt            : out std_logic;
      s2mm_ld_nxt_len               : out std_logic;
      s2mm_wr_len                   : out std_logic_vector(7 downto 0);
      
      -- AXI Memory interface
      m_axi_s2mm_aclk               : in std_logic;
      m_axi_s2mm_aresetn            : in std_logic;
      m_axi_s2mm_awid               : out std_logic_vector(3 downto 0);
      m_axi_s2mm_awaddr             : out std_logic_vector(31 downto 0);
      m_axi_s2mm_awlen              : out std_logic_vector(7 downto 0);
      m_axi_s2mm_awsize             : out std_logic_vector(2 downto 0);
      m_axi_s2mm_awburst            : out std_logic_vector(1 downto 0);
      m_axi_s2mm_awprot             : out std_logic_vector(2 downto 0);
      m_axi_s2mm_awcache            : out std_logic_vector(3 downto 0);
      m_axi_s2mm_awvalid            : out std_logic;
      m_axi_s2mm_awready            : in std_logic;
      m_axi_s2mm_wdata              : out std_logic_vector(63 downto 0);
      m_axi_s2mm_wstrb              : out std_logic_vector(7 downto 0);
      m_axi_s2mm_wlast              : out std_logic;
      m_axi_s2mm_wvalid             : out std_logic;
      m_axi_s2mm_wready             : in std_logic;
      m_axi_s2mm_bresp              : in std_logic_vector(1 downto 0);
      m_axi_s2mm_bvalid             : in std_logic;
      m_axi_s2mm_bready             : out std_logic;
      
      -- AXI Data streaming
      s_axis_s2mm_tdata             : in std_logic_vector(63 downto 0);
      s_axis_s2mm_tkeep             : in std_logic_vector(7 downto 0);
      s_axis_s2mm_tlast             : in std_logic;
      s_axis_s2mm_tvalid            : in std_logic;
      s_axis_s2mm_tready            : out std_logic;
      
      -- Debug interface
      s2mm_dbg_sel                  : in std_logic_vector(3 downto 0);
      s2mm_dbg_data                 : out std_logic_vector(31 downto 0)
    );
  end component;

  -- AXI Memory interface
  --
  signal v8_m_axi_s2mm_aclk_s       : std_logic_vector(7 downto 0);
  signal v8_m_axi_s2mm_aresetn_s    : std_logic_vector(7 downto 0);
  signal a8v4_m_axi_s2mm_awid_s     : array8_v4_t;
  signal a8v32_m_axi_s2mm_awaddr_s  : array8_v32_t;
  signal a8v8_m_axi_s2mm_awlen_s    : array8_v8_t;
  signal a8v3_m_axi_s2mm_awsize_s   : array8_v3_t;
  signal a8v2_m_axi_s2mm_awburst_s  : array8_v2_t;
  signal a8v3_m_axi_s2mm_awprot_s   : array8_v3_t;
  signal a8v4_m_axi_s2mm_awcache_s  : array8_v4_t;
  signal v8_m_axi_s2mm_awvalid_s    : std_logic_vector(7 downto 0);
  signal v8_m_axi_s2mm_awready_s    : std_logic_vector(7 downto 0);
  signal a8v64_m_axi_s2mm_wdata_s   : array8_v64_t;
  signal a8v8_m_axi_s2mm_wstrb_s    : array8_v8_t;
  signal v8_m_axi_s2mm_wlast_s      : std_logic_vector(7 downto 0);
  signal v8_m_axi_s2mm_wvalid_s     : std_logic_vector(7 downto 0);
  signal v8_m_axi_s2mm_wready_s     : std_logic_vector(7 downto 0);
  signal a8v2_m_axi_s2mm_bresp_s    : array8_v2_t;
  signal v8_m_axi_s2mm_bvalid_s     : std_logic_vector(7 downto 0);
  signal v8_m_axi_s2mm_bready_s     : std_logic_vector(7 downto 0);

  signal v8_TxDataMoverRstN_s       : std_logic_vector(7 downto 0);


 begin

  -----------------------------------------------------------------------
  -- input ports mapping
  ----------------------------------------------------------------------- 

  a8v32_dataMoverCtrl_s(0)  <= iv32_dataMoverCtrlCh0_p;
  a8v32_dataMoverCtrl_s(1)  <= iv32_dataMoverCtrlCh1_p;
  a8v32_dataMoverCtrl_s(2)  <= iv32_dataMoverCtrlCh2_p;
  a8v32_dataMoverCtrl_s(3)  <= iv32_dataMoverCtrlCh3_p;
  a8v32_dataMoverCtrl_s(4)  <= iv32_dataMoverCtrlCh4_p;
  a8v32_dataMoverCtrl_s(5)  <= iv32_dataMoverCtrlCh5_p;
  a8v32_dataMoverCtrl_s(6)  <= iv32_dataMoverCtrlCh6_p;
  a8v32_dataMoverCtrl_s(7)  <= iv32_dataMoverCtrlCh7_p;
  
  a8v32_dataMoverAddr_s(0)  <= iv32_dataMoverAddrCh0_p;
  a8v32_dataMoverAddr_s(1)  <= iv32_dataMoverAddrCh1_p;
  a8v32_dataMoverAddr_s(2)  <= iv32_dataMoverAddrCh2_p;
  a8v32_dataMoverAddr_s(3)  <= iv32_dataMoverAddrCh3_p;
  a8v32_dataMoverAddr_s(4)  <= iv32_dataMoverAddrCh4_p;
  a8v32_dataMoverAddr_s(5)  <= iv32_dataMoverAddrCh5_p;
  a8v32_dataMoverAddr_s(6)  <= iv32_dataMoverAddrCh6_p;
  a8v32_dataMoverAddr_s(7)  <= iv32_dataMoverAddrCh7_p;
  
  a8v4_dataMoverTag_s(0)    <= iv4_dataMoverTagCh0_p;
  a8v4_dataMoverTag_s(1)    <= iv4_dataMoverTagCh1_p;
  a8v4_dataMoverTag_s(2)    <= iv4_dataMoverTagCh2_p;
  a8v4_dataMoverTag_s(3)    <= iv4_dataMoverTagCh3_p;
  a8v4_dataMoverTag_s(4)    <= iv4_dataMoverTagCh4_p;
  a8v4_dataMoverTag_s(5)    <= iv4_dataMoverTagCh5_p;
  a8v4_dataMoverTag_s(6)    <= iv4_dataMoverTagCh6_p;
  a8v4_dataMoverTag_s(7)    <= iv4_dataMoverTagCh7_p;
 
  a8v24_transferCnt_s(0)    <= iv24_transferCntCh0_p;   
  a8v24_transferCnt_s(1)    <= iv24_transferCntCh1_p;   
  a8v24_transferCnt_s(2)    <= iv24_transferCntCh2_p;   
  a8v24_transferCnt_s(3)    <= iv24_transferCntCh3_p;   
  a8v24_transferCnt_s(4)    <= iv24_transferCntCh4_p;   
  a8v24_transferCnt_s(5)    <= iv24_transferCntCh5_p;   
  a8v24_transferCnt_s(6)    <= iv24_transferCntCh6_p;   
  a8v24_transferCnt_s(7)    <= iv24_transferCntCh7_p;   
                                                             
  v8_TxWe_s(0)              <= i_TxWeCh0_p;
  v8_TxWe_s(1)              <= i_TxWeCh1_p;
  v8_TxWe_s(2)              <= i_TxWeCh2_p;
  v8_TxWe_s(3)              <= i_TxWeCh3_p;
  v8_TxWe_s(4)              <= i_TxWeCh4_p;
  v8_TxWe_s(5)              <= i_TxWeCh5_p;
  v8_TxWe_s(6)              <= i_TxWeCh6_p;
  v8_TxWe_s(7)              <= i_TxWeCh7_p;

  a8v256_TxDataCh_s(0)((A8I_TX_WIDTH(0)-1) downto 0)    <= iv_TxDataCh0_p;
  a8v256_TxDataCh_s(1)((A8I_TX_WIDTH(1)-1) downto 0)    <= iv_TxDataCh1_p;
  a8v256_TxDataCh_s(2)((A8I_TX_WIDTH(2)-1) downto 0)    <= iv_TxDataCh2_p;
  a8v256_TxDataCh_s(3)((A8I_TX_WIDTH(3)-1) downto 0)    <= iv_TxDataCh3_p;
  a8v256_TxDataCh_s(4)((A8I_TX_WIDTH(4)-1) downto 0)    <= iv_TxDataCh4_p;
  a8v256_TxDataCh_s(5)((A8I_TX_WIDTH(5)-1) downto 0)    <= iv_TxDataCh5_p;
  a8v256_TxDataCh_s(6)((A8I_TX_WIDTH(6)-1) downto 0)    <= iv_TxDataCh6_p;
  a8v256_TxDataCh_s(7)((A8I_TX_WIDTH(7)-1) downto 0)    <= iv_TxDataCh7_p;

  -- AXI Memory interface
  --
  v8_m_axi_s2mm_aclk_s(0)   <= m_axi_s2mm_Ch0_aclk;
  v8_m_axi_s2mm_aresetn_s(0)<= m_axi_s2mm_Ch0_aresetn and v8_TxDataMoverRstN_s(0);
  v8_m_axi_s2mm_awready_s(0)<= m_axi_s2mm_Ch0_awready;
  v8_m_axi_s2mm_wready_s(0) <= m_axi_s2mm_Ch0_wready;
  a8v2_m_axi_s2mm_bresp_s(0)<= m_axi_s2mm_Ch0_bresp;
  v8_m_axi_s2mm_bvalid_s(0) <= m_axi_s2mm_Ch0_bvalid;

  v8_m_axi_s2mm_aclk_s(1)   <= m_axi_s2mm_Ch1_aclk;
  v8_m_axi_s2mm_aresetn_s(1)<= m_axi_s2mm_Ch1_aresetn and v8_TxDataMoverRstN_s(1);
  v8_m_axi_s2mm_awready_s(1)<= m_axi_s2mm_Ch1_awready;
  v8_m_axi_s2mm_wready_s(1) <= m_axi_s2mm_Ch1_wready;
  a8v2_m_axi_s2mm_bresp_s(1)<= m_axi_s2mm_Ch1_bresp;
  v8_m_axi_s2mm_bvalid_s(1) <= m_axi_s2mm_Ch1_bvalid;
  
  v8_m_axi_s2mm_aclk_s(2)   <= m_axi_s2mm_Ch2_aclk;
  v8_m_axi_s2mm_aresetn_s(2)<= m_axi_s2mm_Ch2_aresetn and v8_TxDataMoverRstN_s(2);
  v8_m_axi_s2mm_awready_s(2)<= m_axi_s2mm_Ch2_awready;
  v8_m_axi_s2mm_wready_s(2) <= m_axi_s2mm_Ch2_wready;
  a8v2_m_axi_s2mm_bresp_s(2)<= m_axi_s2mm_Ch2_bresp;
  v8_m_axi_s2mm_bvalid_s(2) <= m_axi_s2mm_Ch2_bvalid;
  
  v8_m_axi_s2mm_aclk_s(3)   <= m_axi_s2mm_Ch3_aclk;
  v8_m_axi_s2mm_aresetn_s(3)<= m_axi_s2mm_Ch3_aresetn and v8_TxDataMoverRstN_s(3);
  v8_m_axi_s2mm_awready_s(3)<= m_axi_s2mm_Ch3_awready;
  v8_m_axi_s2mm_wready_s(3) <= m_axi_s2mm_Ch3_wready;
  a8v2_m_axi_s2mm_bresp_s(3)<= m_axi_s2mm_Ch3_bresp;
  v8_m_axi_s2mm_bvalid_s(3) <= m_axi_s2mm_Ch3_bvalid;
  
  v8_m_axi_s2mm_aclk_s(4)   <= m_axi_s2mm_Ch4_aclk;
  v8_m_axi_s2mm_aresetn_s(4)<= m_axi_s2mm_Ch4_aresetn and v8_TxDataMoverRstN_s(4);
  v8_m_axi_s2mm_awready_s(4)<= m_axi_s2mm_Ch4_awready;
  v8_m_axi_s2mm_wready_s(4) <= m_axi_s2mm_Ch4_wready;
  a8v2_m_axi_s2mm_bresp_s(4)<= m_axi_s2mm_Ch4_bresp;
  v8_m_axi_s2mm_bvalid_s(4) <= m_axi_s2mm_Ch4_bvalid;
  
  v8_m_axi_s2mm_aclk_s(5)   <= m_axi_s2mm_Ch5_aclk;
  v8_m_axi_s2mm_aresetn_s(5)<= m_axi_s2mm_Ch5_aresetn and v8_TxDataMoverRstN_s(5);
  v8_m_axi_s2mm_awready_s(5)<= m_axi_s2mm_Ch5_awready;
  v8_m_axi_s2mm_wready_s(5) <= m_axi_s2mm_Ch5_wready;
  a8v2_m_axi_s2mm_bresp_s(5)<= m_axi_s2mm_Ch5_bresp;
  v8_m_axi_s2mm_bvalid_s(5) <= m_axi_s2mm_Ch5_bvalid;
  
  v8_m_axi_s2mm_aclk_s(6)   <= m_axi_s2mm_Ch6_aclk;
  v8_m_axi_s2mm_aresetn_s(6)<= m_axi_s2mm_Ch6_aresetn and v8_TxDataMoverRstN_s(6);
  v8_m_axi_s2mm_awready_s(6)<= m_axi_s2mm_Ch6_awready;
  v8_m_axi_s2mm_wready_s(6) <= m_axi_s2mm_Ch6_wready;
  a8v2_m_axi_s2mm_bresp_s(6)<= m_axi_s2mm_Ch6_bresp;
  v8_m_axi_s2mm_bvalid_s(6) <= m_axi_s2mm_Ch6_bvalid;
  
  v8_m_axi_s2mm_aclk_s(7)   <= m_axi_s2mm_Ch7_aclk;
  v8_m_axi_s2mm_aresetn_s(7)<= m_axi_s2mm_Ch7_aresetn and v8_TxDataMoverRstN_s(7);
  v8_m_axi_s2mm_awready_s(7)<= m_axi_s2mm_Ch7_awready;
  v8_m_axi_s2mm_wready_s(7) <= m_axi_s2mm_Ch7_wready;
  a8v2_m_axi_s2mm_bresp_s(7)<= m_axi_s2mm_Ch7_bresp;
  v8_m_axi_s2mm_bvalid_s(7) <= m_axi_s2mm_Ch7_bvalid;
  

  -----------------------------------------------------------------------
  -- DataMover reset synch
  -----------------------------------------------------------------------

  DataMoverRstGen: for i in 0 to (NumberChannels_g - 1) generate

    process(v8_AXI_STR_CTRL_STAT_ACLK_s(i))
    begin
      if rising_edge(v8_AXI_STR_CTRL_STAT_ACLK_s(i)) then
        -- Active high to low conversion
        --
        v8_TxDataMoverRstN_s(i) <= not iv8_TxDataMoverRst_p(i);
      end if;
    end process;

  end generate DataMoverRstGen;


  NoDataMoverRstGen: for i in NumberChannels_g to 7 generate

    v8_TxDataMoverRstN_s(i) <= '1';

  end generate NoDataMoverRstGen;


  -----------------------------------------------------------------------
  -- RTDEx channels FIFO (up to 8)
  -----------------------------------------------------------------------

  ChGen: for i in 0 to (NumberChannels_g - 1) generate

    u_fifo_to_axi_stream : fifo_to_axi_stream
      generic map (
        WRITE_DEPTH_g             => WRITE_DEPTH_g,
        C_TX_WIDTH                => A8I_TX_WIDTH(i)
      )
      port map (
        i_strClk_p               => m_axi_s2mm_Ch0_aclk,
        o_intReq_p               => ov8_intReq_p(i),

        -- config & status signals from/to axi registers --
        Bus2IP_Clk                => Bus2IP_Clk,
        
        i_TxReset_p             	=> i_TxReset_p,
        i_TxFifoReset_p       	  => iv8_TxFifoReset_p(i),
        i_TxFifoWrEn_p            => iv8_TxFifoWrEn_p(i),
        o_overflow_p              => ov8_overflow_p(i),
        o_underflow_p             => ov8_underflow_p(i),
        i_streamingTransfer_p	    => iv8_streamingTransfer_p(i),
        i_StartNewTransfer_p  	  => iv8_StartNewTransfer_p(i),
        o_transferDone_p  	      => ov8_transferDone_p(i),

        i_irqLastTransferEn_p	    => iv8_irqLastTransferEn_p(i),

        iv32_dataMoverCtrl_p  	  => a8v32_dataMoverCtrl_s(i),
        iv32_dataMoverAddr_p      => a8v32_dataMoverAddr_s(i),
        iv4_dataMoverTag_p        => a8v4_dataMoverTag_s(i),
        ov8_dataMoverStatus_p     => a8v8_dataMoverStatus_s(i),
        
        iv24_transferCnt_p   	    => a8v24_transferCnt_s(i),
        ov24_currentTransferCnt_p => a8v24_currentTransferCnt_s(i),

        -- AXI data streaming channel
        AXI_STR_TVALID            => v8_AXI_STR_TVALID_s(i),
        AXI_STR_TREADY            => v8_AXI_STR_TREADY_s(i),
        AXI_STR_TLAST             => v8_AXI_STR_TLAST_s(i),
        AXI_STR_TKEEP             => a8v8_AXI_STR_TKEEP_s(i),
        AXI_STR_TDATA             => a8v64_AXI_STR_TDATA_s(i),
        
        -- AXI control and status streaming channel
        AXI_STR_CTRL_STAT_ACLK    => v8_AXI_STR_CTRL_STAT_ACLK_s(i),
        AXI_STR_CTRL_STAT_ARESETN => v8_AXI_STR_CTRL_STAT_ARESETN_s(i),
        AXI_STR_CTRL_TVALID       => v8_AXI_STR_CTRL_TVALID_s(i),
        AXI_STR_CTRL_TREADY       => v8_AXI_STR_CTRL_TREADY_s(i),
        AXI_STR_CTRL_TDATA        => a8v72_AXI_STR_CTRL_TDATA_s(i),
        AXI_STR_STAT_TVALID       => v8_AXI_STR_STAT_TVALID_s(i),
        AXI_STR_STAT_TREADY       => v8_AXI_STR_STAT_TREADY_s(i),
        AXI_STR_STAT_TDATA        => a8v8_AXI_STR_STAT_TDATA_s(i),
        AXI_STR_STAT_TKEEP        => a8v1_AXI_STR_STAT_TKEEP_s(i),
        AXI_STR_STAT_TLAST        => v8_AXI_STR_STAT_TLAST_s(i),

        -- Address posting
        s2mm_allow_addr_req       => v8_s2mm_allow_addr_req_s(i),
        s2mm_addr_req_posted      => v8_s2mm_addr_req_posted_s(i),
        s2mm_wr_xfer_cmplt        => v8_s2mm_wr_xfer_cmplt_s(i),
        s2mm_ld_nxt_len           => v8_s2mm_ld_nxt_len_s(i),
        s2mm_wr_len               => a8v8_s2mm_wr_len_s(i),
        
        -- User side (TX FIFO)
        i_TxUserClk_p             => i_TxUserClk_p,
        i_TxWe_p                  => v8_TxWe_s(i),
        o_TxReady_p               => v8_TxReady_s(i),
        iv_TxData_p               => a8v256_TxDataCh_s(i)((A8I_TX_WIDTH(i)-1) downto 0)
      );

    u_axi_datamover_v3_00_a : axi_datamover_v3_00_a
      port map (
        -- AXI Memory to stream interface (unused)
        --
        m_axi_mm2s_aclk           => '0',
        m_axi_mm2s_aresetn        => '0',
        mm2s_halt                 => '0',
        mm2s_halt_cmplt           => open,
        mm2s_err                  => open,
        m_axis_mm2s_cmdsts_aclk   => '0',
        m_axis_mm2s_cmdsts_aresetn=> '0',
        s_axis_mm2s_cmd_tvalid    => '0',
        s_axis_mm2s_cmd_tready    => open,
        s_axis_mm2s_cmd_tdata     => (others => '0'),
        m_axis_mm2s_sts_tvalid    => open,
        m_axis_mm2s_sts_tready    => '0',
        m_axis_mm2s_sts_tdata     => open,
        m_axis_mm2s_sts_tkeep     => open,
        m_axis_mm2s_sts_tlast     => open,
        mm2s_allow_addr_req       => '0',
        mm2s_addr_req_posted      => open,
        mm2s_rd_xfer_cmplt        => open,
        m_axi_mm2s_arid           => open,
        m_axi_mm2s_araddr         => open,
        m_axi_mm2s_arlen          => open,
        m_axi_mm2s_arsize         => open,
        m_axi_mm2s_arburst        => open,
        m_axi_mm2s_arprot         => open,
        m_axi_mm2s_arcache        => open,
        m_axi_mm2s_arvalid        => open,
        m_axi_mm2s_arready        => '0',
        m_axi_mm2s_rdata          => (others => '0'),
        m_axi_mm2s_rresp          => (others => '0'),
        m_axi_mm2s_rlast          => '0',
        m_axi_mm2s_rvalid         => '0',
        m_axi_mm2s_rready         => open,
        m_axis_mm2s_tdata         => open,
        m_axis_mm2s_tkeep         => open,
        m_axis_mm2s_tlast         => open,
        m_axis_mm2s_tvalid        => open,
        m_axis_mm2s_tready        => '0',
        mm2s_dbg_sel              => (others => '0'),
        mm2s_dbg_data             => open,
        
        -- AXI Stream to memory interface
        --

        -- General Control/Status signals
        s2mm_halt                 => iv8_TxDataMoverHaltReq_p(i),
        s2mm_halt_cmplt           => ov8_TxDataMoverHaltCmplt_p(i),
        s2mm_err                  => ov8_TxDataMoverErr_p(i),

        -- AXI control and status streaming
        m_axis_s2mm_cmdsts_awclk  => v8_AXI_STR_CTRL_STAT_ACLK_s(i),
        m_axis_s2mm_cmdsts_aresetn=> v8_AXI_STR_CTRL_STAT_ARESETN_s(i),
        s_axis_s2mm_cmd_tvalid    => v8_AXI_STR_CTRL_TVALID_s(i),
        s_axis_s2mm_cmd_tready    => v8_AXI_STR_CTRL_TREADY_s(i),
        s_axis_s2mm_cmd_tdata     => a8v72_AXI_STR_CTRL_TDATA_s(i),
        m_axis_s2mm_sts_tvalid    => v8_AXI_STR_STAT_TVALID_s(i),
        m_axis_s2mm_sts_tready    => v8_AXI_STR_STAT_TREADY_s(i),
        m_axis_s2mm_sts_tdata     => a8v8_AXI_STR_STAT_TDATA_s(i),
        m_axis_s2mm_sts_tkeep     => a8v1_AXI_STR_STAT_TKEEP_s(i),
        m_axis_s2mm_sts_tlast     => v8_AXI_STR_STAT_TLAST_s(i),
        
        -- Address posting
        s2mm_allow_addr_req       => v8_s2mm_allow_addr_req_s(i),
        s2mm_addr_req_posted      => v8_s2mm_addr_req_posted_s(i),
        s2mm_wr_xfer_cmplt        => v8_s2mm_wr_xfer_cmplt_s(i),
        s2mm_ld_nxt_len           => v8_s2mm_ld_nxt_len_s(i),
        s2mm_wr_len               => a8v8_s2mm_wr_len_s(i),
  
        -- AXI Memory interface
        m_axi_s2mm_aclk           => v8_m_axi_s2mm_aclk_s(i),
        m_axi_s2mm_aresetn        => v8_m_axi_s2mm_aresetn_s(i),
        m_axi_s2mm_awid           => a8v4_m_axi_s2mm_awid_s(i),
        m_axi_s2mm_awaddr         => a8v32_m_axi_s2mm_awaddr_s(i),
        m_axi_s2mm_awlen          => a8v8_m_axi_s2mm_awlen_s(i),
        m_axi_s2mm_awsize         => a8v3_m_axi_s2mm_awsize_s(i),
        m_axi_s2mm_awburst        => a8v2_m_axi_s2mm_awburst_s(i),
        m_axi_s2mm_awprot         => a8v3_m_axi_s2mm_awprot_s(i),
        m_axi_s2mm_awcache        => a8v4_m_axi_s2mm_awcache_s(i),
        m_axi_s2mm_awvalid        => v8_m_axi_s2mm_awvalid_s(i),
        m_axi_s2mm_awready        => v8_m_axi_s2mm_awready_s(i),
        m_axi_s2mm_wdata          => a8v64_m_axi_s2mm_wdata_s(i),
        m_axi_s2mm_wstrb          => a8v8_m_axi_s2mm_wstrb_s(i),
        m_axi_s2mm_wlast          => v8_m_axi_s2mm_wlast_s(i),
        m_axi_s2mm_wvalid         => v8_m_axi_s2mm_wvalid_s(i),
        m_axi_s2mm_wready         => v8_m_axi_s2mm_wready_s(i),
        m_axi_s2mm_bresp          => a8v2_m_axi_s2mm_bresp_s(i),
        m_axi_s2mm_bvalid         => v8_m_axi_s2mm_bvalid_s(i),
        m_axi_s2mm_bready         => v8_m_axi_s2mm_bready_s(i),

        -- AXI Data streaming
        s_axis_s2mm_tdata         => a8v64_AXI_STR_TDATA_s(i),
        s_axis_s2mm_tkeep         => a8v8_AXI_STR_TKEEP_s(i),
        s_axis_s2mm_tlast         => v8_AXI_STR_TLAST_s(i),
        s_axis_s2mm_tvalid        => v8_AXI_STR_TVALID_s(i),
        s_axis_s2mm_tready        => v8_AXI_STR_TREADY_s(i),
        
        -- Debug interface
        s2mm_dbg_sel              => (others => '0'),
        s2mm_dbg_data             => open
      );
    
  end generate ChGen;

  -- Default values for unconnected signals.
  --
  noChGen_l: for i in NumberChannels_g to 7 generate

    ov8_overflow_p(i)         <= '0';
    ov8_underflow_p(i)        <= '0';
    ov8_transferDone_p(i)     <= '0';
    a8v8_dataMoverStatus_s(i) <= (others => '0');
    a8v24_currentTransferCnt_s(i)<= (others => '0');

    -- AXI data streaming channel
    v8_AXI_STR_TVALID_s(i)    <= '0';
    v8_AXI_STR_TLAST_s(i)     <= '0';
    a8v8_AXI_STR_TKEEP_s(i)   <= (others => '0');
    a8v64_AXI_STR_TDATA_s(i)  <= (others => '0');

    -- AXI control and status streaming channel
    v8_AXI_STR_CTRL_STAT_ACLK_s(i)<= '0';
    v8_AXI_STR_CTRL_STAT_ARESETN_s(i) <= '0';
    v8_AXI_STR_CTRL_TVALID_s(i)   <= '0';
    a8v72_AXI_STR_CTRL_TDATA_s(i) <= (others => '0');
    v8_AXI_STR_STAT_TREADY_s(i)   <= '0';

    -- User side (TX FIFO)
    v8_TxReady_s(i)             <= '0';

    -- AXI Memory interface
    a8v4_m_axi_s2mm_awid_s(i)   <= (others => '0');
    a8v32_m_axi_s2mm_awaddr_s(i)<= (others => '0');
    a8v8_m_axi_s2mm_awlen_s(i)  <= (others => '0');
    a8v3_m_axi_s2mm_awsize_s(i) <= (others => '0');
    a8v2_m_axi_s2mm_awburst_s(i)<= (others => '0');
    a8v3_m_axi_s2mm_awprot_s(i) <= (others => '0');
    a8v4_m_axi_s2mm_awcache_s(i)<= (others => '0');
    v8_m_axi_s2mm_awvalid_s(i)  <= '0';
    a8v64_m_axi_s2mm_wdata_s(i) <= (others => '0');
    a8v8_m_axi_s2mm_wstrb_s(i)  <= (others => '0');
    v8_m_axi_s2mm_wlast_s(i)    <= '0';
    v8_m_axi_s2mm_wvalid_s(i)   <= '0';
    v8_m_axi_s2mm_bready_s(i)   <= '0';
  
  end generate noChGen_l;

 
  -----------------------------------------------------------------------
  -- output ports
  -----------------------------------------------------------------------
  
  ov32_TxConfiginfo_p       <= x"00000" & "00" & v6_FifoDepthInfo_s & std_logic_vector(to_unsigned(NumberChannels_g, 4));

  ov8_dataMoverStatusCh0_p      <= a8v8_dataMoverStatus_s(0);
  ov8_dataMoverStatusCh1_p      <= a8v8_dataMoverStatus_s(1);
  ov8_dataMoverStatusCh2_p      <= a8v8_dataMoverStatus_s(2);
  ov8_dataMoverStatusCh3_p      <= a8v8_dataMoverStatus_s(3);
  ov8_dataMoverStatusCh4_p      <= a8v8_dataMoverStatus_s(4);
  ov8_dataMoverStatusCh5_p      <= a8v8_dataMoverStatus_s(5);
  ov8_dataMoverStatusCh6_p      <= a8v8_dataMoverStatus_s(6);
  ov8_dataMoverStatusCh7_p      <= a8v8_dataMoverStatus_s(7);

  ov24_currentTransferCntCh0_p  <= a8v24_currentTransferCnt_s(0);
  ov24_currentTransferCntCh1_p  <= a8v24_currentTransferCnt_s(1);
  ov24_currentTransferCntCh2_p  <= a8v24_currentTransferCnt_s(2);
  ov24_currentTransferCntCh3_p  <= a8v24_currentTransferCnt_s(3);
  ov24_currentTransferCntCh4_p  <= a8v24_currentTransferCnt_s(4);
  ov24_currentTransferCntCh5_p  <= a8v24_currentTransferCnt_s(5);
  ov24_currentTransferCntCh6_p  <= a8v24_currentTransferCnt_s(6);
  ov24_currentTransferCntCh7_p  <= a8v24_currentTransferCnt_s(7);
  
  process(i_TxUserClk_p)
  begin
    if rising_edge(i_TxUserClk_p) then
      if i_TxReset_p = '1' then  
        o_TxReadyCh0_p          <= '0';
        o_TxReadyCh1_p          <= '0';
        o_TxReadyCh2_p          <= '0';
        o_TxReadyCh3_p          <= '0';
        o_TxReadyCh4_p          <= '0';
        o_TxReadyCh5_p          <= '0';
        o_TxReadyCh6_p          <= '0';
        o_TxReadyCh7_p          <= '0';
      else
        o_TxReadyCh0_p          <= v8_TxReady_s(0);
        o_TxReadyCh1_p          <= v8_TxReady_s(1);
        o_TxReadyCh2_p          <= v8_TxReady_s(2);
        o_TxReadyCh3_p          <= v8_TxReady_s(3);
        o_TxReadyCh4_p          <= v8_TxReady_s(4);
        o_TxReadyCh5_p          <= v8_TxReady_s(5);
        o_TxReadyCh6_p          <= v8_TxReady_s(6);
        o_TxReadyCh7_p          <= v8_TxReady_s(7);
      end if;
    end if;
  end process;

  m_axi_s2mm_Ch0_awid     <= a8v4_m_axi_s2mm_awid_s(0);
  m_axi_s2mm_Ch0_awaddr   <= a8v32_m_axi_s2mm_awaddr_s(0);
  m_axi_s2mm_Ch0_awlen    <= a8v8_m_axi_s2mm_awlen_s(0);
  m_axi_s2mm_Ch0_awsize   <= a8v3_m_axi_s2mm_awsize_s(0);
  m_axi_s2mm_Ch0_awburst  <= a8v2_m_axi_s2mm_awburst_s(0);
  m_axi_s2mm_Ch0_awprot   <= a8v3_m_axi_s2mm_awprot_s(0);
  m_axi_s2mm_Ch0_awcache  <= a8v4_m_axi_s2mm_awcache_s(0);
  m_axi_s2mm_Ch0_awvalid  <= v8_m_axi_s2mm_awvalid_s(0);
  m_axi_s2mm_Ch0_wdata    <= a8v64_m_axi_s2mm_wdata_s(0);
  m_axi_s2mm_Ch0_wstrb    <= a8v8_m_axi_s2mm_wstrb_s(0);
  m_axi_s2mm_Ch0_wlast    <= v8_m_axi_s2mm_wlast_s(0);
  m_axi_s2mm_Ch0_wvalid   <= v8_m_axi_s2mm_wvalid_s(0);
  m_axi_s2mm_Ch0_bready   <= v8_m_axi_s2mm_bready_s(0);

  m_axi_s2mm_Ch1_awid     <= a8v4_m_axi_s2mm_awid_s(1);
  m_axi_s2mm_Ch1_awaddr   <= a8v32_m_axi_s2mm_awaddr_s(1);
  m_axi_s2mm_Ch1_awlen    <= a8v8_m_axi_s2mm_awlen_s(1);
  m_axi_s2mm_Ch1_awsize   <= a8v3_m_axi_s2mm_awsize_s(1);
  m_axi_s2mm_Ch1_awburst  <= a8v2_m_axi_s2mm_awburst_s(1);
  m_axi_s2mm_Ch1_awprot   <= a8v3_m_axi_s2mm_awprot_s(1);
  m_axi_s2mm_Ch1_awcache  <= a8v4_m_axi_s2mm_awcache_s(1);
  m_axi_s2mm_Ch1_awvalid  <= v8_m_axi_s2mm_awvalid_s(1);
  m_axi_s2mm_Ch1_wdata    <= a8v64_m_axi_s2mm_wdata_s(1);
  m_axi_s2mm_Ch1_wstrb    <= a8v8_m_axi_s2mm_wstrb_s(1);
  m_axi_s2mm_Ch1_wlast    <= v8_m_axi_s2mm_wlast_s(1);
  m_axi_s2mm_Ch1_wvalid   <= v8_m_axi_s2mm_wvalid_s(1);
  m_axi_s2mm_Ch1_bready   <= v8_m_axi_s2mm_bready_s(1);
  
  m_axi_s2mm_Ch2_awid     <= a8v4_m_axi_s2mm_awid_s(2);
  m_axi_s2mm_Ch2_awaddr   <= a8v32_m_axi_s2mm_awaddr_s(2);
  m_axi_s2mm_Ch2_awlen    <= a8v8_m_axi_s2mm_awlen_s(2);
  m_axi_s2mm_Ch2_awsize   <= a8v3_m_axi_s2mm_awsize_s(2);
  m_axi_s2mm_Ch2_awburst  <= a8v2_m_axi_s2mm_awburst_s(2);
  m_axi_s2mm_Ch2_awprot   <= a8v3_m_axi_s2mm_awprot_s(2);
  m_axi_s2mm_Ch2_awcache  <= a8v4_m_axi_s2mm_awcache_s(2);
  m_axi_s2mm_Ch2_awvalid  <= v8_m_axi_s2mm_awvalid_s(2);
  m_axi_s2mm_Ch2_wdata    <= a8v64_m_axi_s2mm_wdata_s(2);
  m_axi_s2mm_Ch2_wstrb    <= a8v8_m_axi_s2mm_wstrb_s(2);
  m_axi_s2mm_Ch2_wlast    <= v8_m_axi_s2mm_wlast_s(2);
  m_axi_s2mm_Ch2_wvalid   <= v8_m_axi_s2mm_wvalid_s(2);
  m_axi_s2mm_Ch2_bready   <= v8_m_axi_s2mm_bready_s(2);
  
  m_axi_s2mm_Ch3_awid     <= a8v4_m_axi_s2mm_awid_s(3);
  m_axi_s2mm_Ch3_awaddr   <= a8v32_m_axi_s2mm_awaddr_s(3);
  m_axi_s2mm_Ch3_awlen    <= a8v8_m_axi_s2mm_awlen_s(3);
  m_axi_s2mm_Ch3_awsize   <= a8v3_m_axi_s2mm_awsize_s(3);
  m_axi_s2mm_Ch3_awburst  <= a8v2_m_axi_s2mm_awburst_s(3);
  m_axi_s2mm_Ch3_awprot   <= a8v3_m_axi_s2mm_awprot_s(3);
  m_axi_s2mm_Ch3_awcache  <= a8v4_m_axi_s2mm_awcache_s(3);
  m_axi_s2mm_Ch3_awvalid  <= v8_m_axi_s2mm_awvalid_s(3);
  m_axi_s2mm_Ch3_wdata    <= a8v64_m_axi_s2mm_wdata_s(3);
  m_axi_s2mm_Ch3_wstrb    <= a8v8_m_axi_s2mm_wstrb_s(3);
  m_axi_s2mm_Ch3_wlast    <= v8_m_axi_s2mm_wlast_s(3);
  m_axi_s2mm_Ch3_wvalid   <= v8_m_axi_s2mm_wvalid_s(3);
  m_axi_s2mm_Ch3_bready   <= v8_m_axi_s2mm_bready_s(3);
  
  m_axi_s2mm_Ch4_awid     <= a8v4_m_axi_s2mm_awid_s(4);
  m_axi_s2mm_Ch4_awaddr   <= a8v32_m_axi_s2mm_awaddr_s(4);
  m_axi_s2mm_Ch4_awlen    <= a8v8_m_axi_s2mm_awlen_s(4);
  m_axi_s2mm_Ch4_awsize   <= a8v3_m_axi_s2mm_awsize_s(4);
  m_axi_s2mm_Ch4_awburst  <= a8v2_m_axi_s2mm_awburst_s(4);
  m_axi_s2mm_Ch4_awprot   <= a8v3_m_axi_s2mm_awprot_s(4);
  m_axi_s2mm_Ch4_awcache  <= a8v4_m_axi_s2mm_awcache_s(4);
  m_axi_s2mm_Ch4_awvalid  <= v8_m_axi_s2mm_awvalid_s(4);
  m_axi_s2mm_Ch4_wdata    <= a8v64_m_axi_s2mm_wdata_s(4);
  m_axi_s2mm_Ch4_wstrb    <= a8v8_m_axi_s2mm_wstrb_s(4);
  m_axi_s2mm_Ch4_wlast    <= v8_m_axi_s2mm_wlast_s(4);
  m_axi_s2mm_Ch4_wvalid   <= v8_m_axi_s2mm_wvalid_s(4);
  m_axi_s2mm_Ch4_bready   <= v8_m_axi_s2mm_bready_s(4);
  
  m_axi_s2mm_Ch5_awid     <= a8v4_m_axi_s2mm_awid_s(5);
  m_axi_s2mm_Ch5_awaddr   <= a8v32_m_axi_s2mm_awaddr_s(5);
  m_axi_s2mm_Ch5_awlen    <= a8v8_m_axi_s2mm_awlen_s(5);
  m_axi_s2mm_Ch5_awsize   <= a8v3_m_axi_s2mm_awsize_s(5);
  m_axi_s2mm_Ch5_awburst  <= a8v2_m_axi_s2mm_awburst_s(5);
  m_axi_s2mm_Ch5_awprot   <= a8v3_m_axi_s2mm_awprot_s(5);
  m_axi_s2mm_Ch5_awcache  <= a8v4_m_axi_s2mm_awcache_s(5);
  m_axi_s2mm_Ch5_awvalid  <= v8_m_axi_s2mm_awvalid_s(5);
  m_axi_s2mm_Ch5_wdata    <= a8v64_m_axi_s2mm_wdata_s(5);
  m_axi_s2mm_Ch5_wstrb    <= a8v8_m_axi_s2mm_wstrb_s(5);
  m_axi_s2mm_Ch5_wlast    <= v8_m_axi_s2mm_wlast_s(5);
  m_axi_s2mm_Ch5_wvalid   <= v8_m_axi_s2mm_wvalid_s(5);
  m_axi_s2mm_Ch5_bready   <= v8_m_axi_s2mm_bready_s(5);
  
  m_axi_s2mm_Ch6_awid     <= a8v4_m_axi_s2mm_awid_s(6);
  m_axi_s2mm_Ch6_awaddr   <= a8v32_m_axi_s2mm_awaddr_s(6);
  m_axi_s2mm_Ch6_awlen    <= a8v8_m_axi_s2mm_awlen_s(6);
  m_axi_s2mm_Ch6_awsize   <= a8v3_m_axi_s2mm_awsize_s(6);
  m_axi_s2mm_Ch6_awburst  <= a8v2_m_axi_s2mm_awburst_s(6);
  m_axi_s2mm_Ch6_awprot   <= a8v3_m_axi_s2mm_awprot_s(6);
  m_axi_s2mm_Ch6_awcache  <= a8v4_m_axi_s2mm_awcache_s(6);
  m_axi_s2mm_Ch6_awvalid  <= v8_m_axi_s2mm_awvalid_s(6);
  m_axi_s2mm_Ch6_wdata    <= a8v64_m_axi_s2mm_wdata_s(6);
  m_axi_s2mm_Ch6_wstrb    <= a8v8_m_axi_s2mm_wstrb_s(6);
  m_axi_s2mm_Ch6_wlast    <= v8_m_axi_s2mm_wlast_s(6);
  m_axi_s2mm_Ch6_wvalid   <= v8_m_axi_s2mm_wvalid_s(6);
  m_axi_s2mm_Ch6_bready   <= v8_m_axi_s2mm_bready_s(6);
  
  m_axi_s2mm_Ch7_awid     <= a8v4_m_axi_s2mm_awid_s(7);
  m_axi_s2mm_Ch7_awaddr   <= a8v32_m_axi_s2mm_awaddr_s(7);
  m_axi_s2mm_Ch7_awlen    <= a8v8_m_axi_s2mm_awlen_s(7);
  m_axi_s2mm_Ch7_awsize   <= a8v3_m_axi_s2mm_awsize_s(7);
  m_axi_s2mm_Ch7_awburst  <= a8v2_m_axi_s2mm_awburst_s(7);
  m_axi_s2mm_Ch7_awprot   <= a8v3_m_axi_s2mm_awprot_s(7);
  m_axi_s2mm_Ch7_awcache  <= a8v4_m_axi_s2mm_awcache_s(7);
  m_axi_s2mm_Ch7_awvalid  <= v8_m_axi_s2mm_awvalid_s(7);
  m_axi_s2mm_Ch7_wdata    <= a8v64_m_axi_s2mm_wdata_s(7);
  m_axi_s2mm_Ch7_wstrb    <= a8v8_m_axi_s2mm_wstrb_s(7);
  m_axi_s2mm_Ch7_wlast    <= v8_m_axi_s2mm_wlast_s(7);
  m_axi_s2mm_Ch7_wvalid   <= v8_m_axi_s2mm_wvalid_s(7);
  m_axi_s2mm_Ch7_bready   <= v8_m_axi_s2mm_bready_s(7);

  
  -----------------------------------------------------------------------
  -- FIFO data info
  -----------------------------------------------------------------------

  -- The FIFO size is WRITE_WIDTH_g * WRITE_DEPTH_g bytes,
  -- with WRITE_WIDTH_g = 4;
  -- The FIFO Depth Info is n, where 2^n = FIFO size
  --
  v6_FifoDepthInfo_s <= std_logic_vector(to_unsigned(log2_ceil(4*WRITE_DEPTH_g - 1),6)); 
  
    
end rtdex_tx_behav;
