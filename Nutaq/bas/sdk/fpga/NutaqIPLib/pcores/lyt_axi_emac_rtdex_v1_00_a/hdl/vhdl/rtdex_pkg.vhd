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
--
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

package rtdex_pkg is

component rtdex_mux_rx   
   port(
   		 -- config signals from/to axi registers --
         iv32_RemoteEndMacAddrH_p     : in  std_logic_vector(31 downto 0); 
         iv16_RemoteEndMacAddrL_p     : in  std_logic_vector(15 downto 0); 		   
         iv32_LocalEndMacAddrH_p     : in  std_logic_vector(31 downto 0); 
         iv16_LocalEndMacAddrL_p     : in  std_logic_vector(15 downto 0);
                           
         -- AXI input from TEMAC
         AXI_STR_TXD_ACLK         : in  std_logic;                           --  AXI-Stream Transmit Data Clk      
         AXI_STR_TXD_ARESETN      : in  std_logic;                           --  AXI-Stream Transmit Data Reset
         AXI_STR_TXD_TVALID       : in  std_logic;                           --  AXI-Stream Transmit Data Valid
         AXI_STR_TXD_TREADY       : out std_logic;                           --  AXI-Stream Transmit Data Ready
         AXI_STR_TXD_TLAST        : in  std_logic;                           --  AXI-Stream Transmit Data Last
         AXI_STR_TXD_TKEEP        : in  std_logic_vector(3 downto 0);        --  AXI-Stream Transmit Data Keep
         AXI_STR_TXD_TDATA        : in  std_logic_vector(31 downto 0);       --  AXI-Stream Transmit Data Data
         
         AXI_STR_TXC_ACLK         : in  std_logic;                           --  AXI-Stream Transmit Control Clk
         AXI_STR_TXC_ARESETN      : in  std_logic;                           --  AXI-Stream Transmit Control Reset
         AXI_STR_TXC_TVALID       : in  std_logic;                           --  AXI-Stream Transmit Control Valid
         AXI_STR_TXC_TREADY       : out std_logic;                           --  AXI-Stream Transmit Control Ready
         AXI_STR_TXC_TLAST        : in  std_logic;                           --  AXI-Stream Transmit Control Last
         AXI_STR_TXC_TKEEP        : in  std_logic_vector(3 downto 0);        --  AXI-Stream Transmit Control Keep
         AXI_STR_TXC_TDATA        : in  std_logic_vector(31 downto 0);       --  AXI-Stream Transmit Control Data
         
         -- AXI output to EDMA         
         AXI_STR_RXD_ACLK         : in  std_logic;                           --  AXI-Stream Receive Data Clk
         AXI_STR_RXD_ARESETN      : in  std_logic;                           --  AXI-Stream Receive Data Reset
         AXI_STR_RXD_TVALID       : out std_logic;                           --  AXI-Stream Receive Data Valid
         AXI_STR_RXD_TREADY       : in  std_logic;                           --  AXI-Stream Receive Data Ready
         AXI_STR_RXD_TLAST        : out std_logic;                           --  AXI-Stream Receive Data Last
         AXI_STR_RXD_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Data Keep
         AXI_STR_RXD_TDATA        : out std_logic_vector(31 downto 0);       --  AXI-Stream Receive Data Data
         
         AXI_STR_RXS_ACLK         : in  std_logic;                           --  AXI-Stream Receive Status Clk 
         AXI_STR_RXS_ARESETN      : in  std_logic;                           --  AXI-Stream Receive Status Reset
         AXI_STR_RXS_TVALID       : out std_logic;                           --  AXI-Stream Receive Status Valid
         AXI_STR_RXS_TREADY       : in  std_logic;                           --  AXI-Stream Receive Status Ready
         AXI_STR_RXS_TLAST        : out std_logic;                           --  AXI-Stream Receive Status Last
         AXI_STR_RXS_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Status Keep
         AXI_STR_RXS_TDATA        : out std_logic_vector(31 downto 0);       --  AXI-Stream Receive Status Data         
                  
         -- AXI output to RTDEx         
         RTDEx_STR_RXD_ACLK         : out  std_logic;                        --  AXI-Stream Receive Data Clk
         RTDEx_STR_RXD_ARESETN      : out  std_logic;                        --  AXI-Stream Receive Data Reset
         RTDEx_STR_RXD_TVALID       : out std_logic;                         --  AXI-Stream Receive Data Valid
         RTDEx_STR_RXD_TREADY       : in  std_logic;                         --  AXI-Stream Receive Data Ready
         RTDEx_STR_RXD_TLAST        : out std_logic;                         --  AXI-Stream Receive Data Last
         RTDEx_STR_RXD_TKEEP        : out std_logic_vector(3 downto 0);      --  AXI-Stream Receive Data Keep
         RTDEx_STR_RXD_TDATA        : out std_logic_vector(31 downto 0);     --  AXI-Stream Receive Data Data
         
         RTDEx_STR_RXS_ACLK         : out  std_logic;                           --  AXI-Stream Receive Status Clk 
         RTDEx_STR_RXS_ARESETN      : out  std_logic;                           --  AXI-Stream Receive Status Reset
         RTDEx_STR_RXS_TVALID       : out std_logic;                           --  AXI-Stream Receive Status Valid
         RTDEx_STR_RXS_TREADY       : in  std_logic;                           --  AXI-Stream Receive Status Ready
         RTDEx_STR_RXS_TLAST        : out std_logic;                           --  AXI-Stream Receive Status Last
         RTDEx_STR_RXS_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Status Keep
         RTDEx_STR_RXS_TDATA        : out std_logic_vector(31 downto 0)       --  AXI-Stream Receive Status Data         
         );
end component;

component rtdex_rx
   generic(         
          NumberChannels_g : integer range 1 to 8 := 3;
          C_RXMEM          : integer := 4096;
          C_RX_WIDTH_CH0        : integer := 32;
          C_RX_WIDTH_CH1        : integer := 32;
          C_RX_WIDTH_CH2        : integer := 32;
          C_RX_WIDTH_CH3        : integer := 32;
          C_RX_WIDTH_CH4        : integer := 32;
          C_RX_WIDTH_CH5        : integer := 32;
          C_RX_WIDTH_CH6        : integer := 32;
          C_RX_WIDTH_CH7        : integer := 32;
          StatsCntrs_g	   : integer range 0 to 1 := 1;
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
end component;         

component rtdex_mux_tx   
   port(
         -- AXI input from EDMA
         AXI_STR_TXD_ACLK         : in  std_logic;                           --  AXI-Stream Transmit Data Clk      
         AXI_STR_TXD_ARESETN      : in  std_logic;                           --  AXI-Stream Transmit Data Reset
         AXI_STR_TXD_TVALID       : in  std_logic;                           --  AXI-Stream Transmit Data Valid
         AXI_STR_TXD_TREADY       : out std_logic;                           --  AXI-Stream Transmit Data Ready
         AXI_STR_TXD_TLAST        : in  std_logic;                           --  AXI-Stream Transmit Data Last
         AXI_STR_TXD_TKEEP        : in  std_logic_vector(3 downto 0);        --  AXI-Stream Transmit Data Keep
         AXI_STR_TXD_TDATA        : in  std_logic_vector(31 downto 0);       --  AXI-Stream Transmit Data Data
         
         AXI_STR_TXC_ACLK         : in  std_logic;                           --  AXI-Stream Transmit Control Clk
         AXI_STR_TXC_ARESETN      : in  std_logic;                           --  AXI-Stream Transmit Control Reset
         AXI_STR_TXC_TVALID       : in  std_logic;                           --  AXI-Stream Transmit Control Valid
         AXI_STR_TXC_TREADY       : out std_logic;                           --  AXI-Stream Transmit Control Ready
         AXI_STR_TXC_TLAST        : in  std_logic;                           --  AXI-Stream Transmit Control Last
         AXI_STR_TXC_TKEEP        : in  std_logic_vector(3 downto 0);        --  AXI-Stream Transmit Control Keep
         AXI_STR_TXC_TDATA        : in  std_logic_vector(31 downto 0);       --  AXI-Stream Transmit Control Data
         
         -- AXI input from RTDEx
         RTDEX_STR_TXD_ACLK       : out  std_logic;                         --  AXI-Stream Transmit Data Clk      
         RTDEX_STR_TXD_ARESETN    : out  std_logic;                         --  AXI-Stream Transmit Data Reset
         RTDEX_STR_TXD_TVALID     : in  std_logic;                          --  AXI-Stream Transmit Data Valid
         RTDEX_STR_TXD_TREADY     : out std_logic;                          --  AXI-Stream Transmit Data Ready
         RTDEX_STR_TXD_TLAST      : in  std_logic;                          --  AXI-Stream Transmit Data Last
         RTDEX_STR_TXD_TKEEP      : in  std_logic_vector(3 downto 0);       --  AXI-Stream Transmit Data Keep
         RTDEX_STR_TXD_TDATA      : in  std_logic_vector(31 downto 0);      --  AXI-Stream Transmit Data Data
                  
         RTDEX_STR_TXC_ACLK       : out  std_logic;                         --  AXI-Stream Transmit Control Clk
         RTDEX_STR_TXC_ARESETN    : out  std_logic;                         --  AXI-Stream Transmit Control Reset
         RTDEX_STR_TXC_TVALID     : in  std_logic;                          --  AXI-Stream Transmit Control Valid
         RTDEX_STR_TXC_TREADY     : out std_logic;                          --  AXI-Stream Transmit Control Ready
         RTDEX_STR_TXC_TLAST      : in  std_logic;                          --  AXI-Stream Transmit Control Last
         RTDEX_STR_TXC_TKEEP      : in  std_logic_vector(3 downto 0);       --  AXI-Stream Transmit Control Keep
         RTDEX_STR_TXC_TDATA      : in  std_logic_vector(31 downto 0);      --  AXI-Stream Transmit Control Data
         
         -- AXI input from Pause Frames request generator
         PFG_STR_TXD_ACLK       : out  std_logic;                         --  AXI-Stream Transmit Data Clk      
         PFG_STR_TXD_ARESETN    : out  std_logic;                         --  AXI-Stream Transmit Data Reset
         PFG_STR_TXD_TVALID     : in  std_logic;                          --  AXI-Stream Transmit Data Valid
         PFG_STR_TXD_TREADY     : out std_logic;                          --  AXI-Stream Transmit Data Ready
         PFG_STR_TXD_TLAST      : in  std_logic;                          --  AXI-Stream Transmit Data Last
         PFG_STR_TXD_TKEEP      : in  std_logic_vector(3 downto 0);       --  AXI-Stream Transmit Data Keep
         PFG_STR_TXD_TDATA      : in  std_logic_vector(31 downto 0);      --  AXI-Stream Transmit Data Data
                  
         PFG_STR_TXC_ACLK       : out  std_logic;                         --  AXI-Stream Transmit Control Clk
         PFG_STR_TXC_ARESETN    : out  std_logic;                         --  AXI-Stream Transmit Control Reset
         PFG_STR_TXC_TVALID     : in  std_logic;                          --  AXI-Stream Transmit Control Valid
         PFG_STR_TXC_TREADY     : out std_logic;                          --  AXI-Stream Transmit Control Ready
         PFG_STR_TXC_TLAST      : in  std_logic;                          --  AXI-Stream Transmit Control Last
         PFG_STR_TXC_TKEEP      : in  std_logic_vector(3 downto 0);       --  AXI-Stream Transmit Control Keep
         PFG_STR_TXC_TDATA      : in  std_logic_vector(31 downto 0);      --  AXI-Stream Transmit Control Data
                  
         -- AXI output to EMAC         
         AXI_STR_RXD_ACLK         : in  std_logic;                           --  AXI-Stream Receive Data Clk
         AXI_STR_RXD_ARESETN      : in  std_logic;                           --  AXI-Stream Receive Data Reset
         AXI_STR_RXD_TVALID       : out std_logic;                           --  AXI-Stream Receive Data Valid
         AXI_STR_RXD_TREADY       : in  std_logic;                           --  AXI-Stream Receive Data Ready
         AXI_STR_RXD_TLAST        : out std_logic;                           --  AXI-Stream Receive Data Last
         AXI_STR_RXD_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Data Keep
         AXI_STR_RXD_TDATA        : out std_logic_vector(31 downto 0);       --  AXI-Stream Receive Data Data
         
         AXI_STR_RXS_ACLK         : in  std_logic;                           --  AXI-Stream Receive Status Clk 
         AXI_STR_RXS_ARESETN      : in  std_logic;                           --  AXI-Stream Receive Status Reset
         AXI_STR_RXS_TVALID       : out std_logic;                           --  AXI-Stream Receive Status Valid
         AXI_STR_RXS_TREADY       : in  std_logic;                           --  AXI-Stream Receive Status Ready
         AXI_STR_RXS_TLAST        : out std_logic;                           --  AXI-Stream Receive Status Last
         AXI_STR_RXS_TKEEP        : out std_logic_vector(3 downto 0);        --  AXI-Stream Receive Status Keep
         AXI_STR_RXS_TDATA        : out std_logic_vector(31 downto 0)       --  AXI-Stream Receive Status Data
         );
end component;

component rtdex_tx
   generic(         
          NumberChannels_g 		: integer range 1 to 8 := 1;
          C_TXMEM           	: integer := 4096;
          C_TX_WIDTH_CH0        : integer := 32;
          C_TX_WIDTH_CH1        : integer := 32;
          C_TX_WIDTH_CH2        : integer := 32;
          C_TX_WIDTH_CH3        : integer := 32;
          C_TX_WIDTH_CH4        : integer := 32;
          C_TX_WIDTH_CH5        : integer := 32;
          C_TX_WIDTH_CH6        : integer := 32;
          C_TX_WIDTH_CH7        : integer := 32;
          SuportJumboFrames_g   : integer range 0 to 1 := 1;
          StatsCntrs_g		  	: integer range 0 to 1 := 1;
          big_Endian_g			: integer range 0 to 1 := 0
          );
   port(
         -- config & status signals from/to axi registers --
         iv32_RemoteEndMacAddrH_p   : in  std_logic_vector(31 downto 0); 
         iv16_RemoteEndMacAddrL_p   : in  std_logic_vector(15 downto 0); 		   
         iv32_LocalEndMacAddrH_p    : in  std_logic_vector(31 downto 0); 
         iv16_LocalEndMacAddrL_p    : in  std_logic_vector(15 downto 0);          
         ov32_TxConfigInfo_p     	: out std_logic_vector(31 downto 0);         
         i_Mode_p					: in std_logic;
         iv8_TxChFrsBurst_p			: in std_logic_vector(7 downto 0);
                                    
         iv15_FrameSizeCh0_p      : in std_logic_vector(14 downto 0);
         iv32_TransferSizeCh0_p   : in std_logic_vector(31 downto 0);
         
         iv15_FrameSizeCh1_p      : in std_logic_vector(14 downto 0);
         iv32_TransferSizeCh1_p   : in std_logic_vector(31 downto 0);
         
         iv15_FrameSizeCh2_p      : in std_logic_vector(14 downto 0);
         iv32_TransferSizeCh2_p   : in std_logic_vector(31 downto 0);
         
         iv15_FrameSizeCh3_p      : in std_logic_vector(14 downto 0);
         iv32_TransferSizeCh3_p   : in std_logic_vector(31 downto 0);
         
         iv15_FrameSizeCh4_p      : in std_logic_vector(14 downto 0);
         iv32_TransferSizeCh4_p   : in std_logic_vector(31 downto 0);
         
         iv15_FrameSizeCh5_p      : in std_logic_vector(14 downto 0);
         iv32_TransferSizeCh5_p   : in std_logic_vector(31 downto 0);
         
         iv15_FrameSizeCh6_p      : in std_logic_vector(14 downto 0);
         iv32_TransferSizeCh6_p   : in std_logic_vector(31 downto 0);
         
         iv15_FrameSizeCh7_p      : in std_logic_vector(14 downto 0);
         iv32_TransferSizeCh7_p   : in std_logic_vector(31 downto 0);
         
         i_TxReset_p             : in  std_logic;
         iv8_TxFifoReset_p       : in  std_logic_vector(7 downto 0);      
         iv32_FrameGap_p         	: in std_logic_vector(31 downto 0);         
         iv8_StartNewTransfer_p  : in  std_logic_vector(7 downto 0);
         ov8_TxFifoOverrun_p		: out  std_logic_vector(7 downto 0);
		
         --- AXI STR side ---         
         RTDEX_STR_TXD_ACLK      : in  std_logic;                               
         RTDEX_STR_TXD_ARESETN   : in  std_logic;                           
         RTDEX_STR_TXD_TVALID    : out  std_logic;                          
         RTDEX_STR_TXD_TREADY    : in std_logic;                            
         RTDEX_STR_TXD_TLAST     : out  std_logic;                          
         RTDEX_STR_TXD_TKEEP     : out  std_logic_vector(3 downto 0);       
         RTDEX_STR_TXD_TDATA     : out  std_logic_vector(31 downto 0);      
         
         RTDEX_STR_TXC_ACLK      : in  std_logic;                           
         RTDEX_STR_TXC_ARESETN   : in  std_logic;                           
         RTDEX_STR_TXC_TVALID    : out  std_logic;                          
         RTDEX_STR_TXC_TREADY    : in std_logic;                            
         RTDEX_STR_TXC_TLAST     : out  std_logic;                          
         RTDEX_STR_TXC_TKEEP     : out  std_logic_vector(3 downto 0);       
         RTDEX_STR_TXC_TDATA     : out  std_logic_vector(31 downto 0);      
         
         --- User side ---
         i_TxUserClk_p           : in std_logic;
         
         i_TxWeCh0_p             : in std_logic;
         o_TxReadyCh0_p          : out std_logic;
         iv_TxDataCh0_p        : in std_logic_vector(C_TX_WIDTH_CH0-1 downto 0);
         ov32_TxFifoCountCh0_p   : out std_logic_vector(31 downto 0);
         ov32_NbrSentFramesCh0_p : out std_logic_vector(31 downto 0);
         
         i_TxWeCh1_p             : in std_logic;
         o_TxReadyCh1_p          : out std_logic;
         iv_TxDataCh1_p        : in std_logic_vector(C_TX_WIDTH_CH1-1 downto 0);
         ov32_TxFifoCountCh1_p   : out std_logic_vector(31 downto 0);
         ov32_NbrSentFramesCh1_p : out std_logic_vector(31 downto 0);
         
         i_TxWeCh2_p             : in std_logic;
         o_TxReadyCh2_p          : out std_logic;
         iv_TxDataCh2_p        : in std_logic_vector(C_TX_WIDTH_CH2-1 downto 0);
         ov32_TxFifoCountCh2_p   : out std_logic_vector(31 downto 0);
         ov32_NbrSentFramesCh2_p : out std_logic_vector(31 downto 0);
         
         i_TxWeCh3_p             : in std_logic;
         o_TxReadyCh3_p          : out std_logic;
         iv_TxDataCh3_p        : in std_logic_vector(C_TX_WIDTH_CH3-1 downto 0);
         ov32_TxFifoCountCh3_p   : out std_logic_vector(31 downto 0);
         ov32_NbrSentFramesCh3_p : out std_logic_vector(31 downto 0);
         
         i_TxWeCh4_p             : in std_logic;
         o_TxReadyCh4_p          : out std_logic;
         iv_TxDataCh4_p        : in std_logic_vector(C_TX_WIDTH_CH4-1 downto 0);
         ov32_TxFifoCountCh4_p   : out std_logic_vector(31 downto 0);
         ov32_NbrSentFramesCh4_p : out std_logic_vector(31 downto 0);
         
         i_TxWeCh5_p             : in std_logic;
         o_TxReadyCh5_p          : out std_logic;
         iv_TxDataCh5_p        : in std_logic_vector(C_TX_WIDTH_CH5-1 downto 0);
         ov32_TxFifoCountCh5_p   : out std_logic_vector(31 downto 0);
         ov32_NbrSentFramesCh5_p : out std_logic_vector(31 downto 0);
         
         i_TxWeCh6_p             : in std_logic;
         o_TxReadyCh6_p          : out std_logic;
         iv_TxDataCh6_p        : in std_logic_vector(C_TX_WIDTH_CH6-1 downto 0);
         ov32_TxFifoCountCh6_p   : out std_logic_vector(31 downto 0);
         ov32_NbrSentFramesCh6_p : out std_logic_vector(31 downto 0);
         
         i_TxWeCh7_p             : in std_logic;
         o_TxReadyCh7_p          : out std_logic;
         iv_TxDataCh7_p        : in std_logic_vector(C_TX_WIDTH_CH7-1 downto 0);
         ov32_TxFifoCountCh7_p   : out std_logic_vector(31 downto 0);
         ov32_NbrSentFramesCh7_p : out std_logic_vector(31 downto 0)
        );
end component;

component rtdex_pause_req_gen
 	port(
 		-- Config side
 		iv32_LocalEndMacAddrH_p 	: in  std_logic_vector(31 downto 0); 
        iv16_LocalEndMacAddrL_p 	: in  std_logic_vector(15 downto 0); 
 		-- Control side
 		i_SendPause_p			 : in std_logic;
 		iv16_PauseVal_p			 : in std_logic_vector(15 downto 0);
 		--- AXI STR side ---         
         RTDEX_STR_TXD_ACLK      : in  std_logic;                               
         RTDEX_STR_TXD_ARESETN   : in  std_logic;                           
         RTDEX_STR_TXD_TVALID    : out  std_logic;                          
         RTDEX_STR_TXD_TREADY    : in std_logic;                            
         RTDEX_STR_TXD_TLAST     : out  std_logic;                          
         RTDEX_STR_TXD_TKEEP     : out  std_logic_vector(3 downto 0);       
         RTDEX_STR_TXD_TDATA     : out  std_logic_vector(31 downto 0);      
         
         RTDEX_STR_TXC_ACLK      : in  std_logic;                           
         RTDEX_STR_TXC_ARESETN   : in  std_logic;                           
         RTDEX_STR_TXC_TVALID    : out  std_logic;                          
         RTDEX_STR_TXC_TREADY    : in std_logic;                            
         RTDEX_STR_TXC_TLAST     : out  std_logic;                          
         RTDEX_STR_TXC_TKEEP     : out  std_logic_vector(3 downto 0);       
         RTDEX_STR_TXC_TDATA     : out  std_logic_vector(31 downto 0)
 		);
 end component;

         
end rtdex_pkg;