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
-- Description: Go from AXI data format to EMAC data format.
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

library work;
use work.rtdex_defines.all;

entity rtdex_mux_tx is   
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
         AXI_STR_RXS_TDATA        : out std_logic_vector(31 downto 0)        --  AXI-Stream Receive Status Data         
         );
end rtdex_mux_tx;
 
architecture behav of rtdex_mux_tx is

 -- attribute keep hierarchy is just for CS dbg. Can be removed
-- attribute keep_hierarchy : string;                         
-- attribute keep_hierarchy of behav : architecture is "true";
 -- End for dbg --


type Rx_fsm_t is
   (
   idle_c         ,
   PauseReqTxcGrand_c ,
   PauseReqTxdGrand_c ,
   EdmaTxcGrand_c ,
   EdmaTxdGrand_c ,
   RtdexTxcGrand_c,
   RtdexTxdGrand_c   
   );
signal Tx_fsm_s : Rx_fsm_t;

type TxTraficMux_t is
   (
   Closed_c,
   PauseReqTrafic_c, 
   GrantReglTrafic_c,
   GrantRtdexTrafic_c
   );

signal TxTraficMux_s : TxTraficMux_t;
signal v8_cnt_s : unsigned(7 downto 0);

begin
 
 
  process(AXI_STR_TXC_ACLK)
  begin
 	if rising_edge(AXI_STR_TXC_ACLK)then
 		if AXI_STR_TXC_ARESETN = '0' then           		    		   
 			Tx_fsm_s <= idle_c;
 			TxTraficMux_s <= Closed_c;
 			v8_cnt_s <= (others => '0');
 		else  
 			case Tx_fsm_s is
 		
 				when idle_c => 			
               
               TxTraficMux_s <= Closed_c;
               
               -- priority given to pause frames over normal eth trafic and over RTDEx
               if PFG_STR_TXC_TVALID = '1' and (PFG_STR_TXC_TDATA = v32_TxcFlagWord_c) then 
                	  TxTraficMux_s <= PauseReqTrafic_c;	                  
                   Tx_fsm_s <= PauseReqTxcGrand_c;
                elsif AXI_STR_TXC_TVALID = '1' and (AXI_STR_TXC_TDATA = v32_TxcFlagWord_c) then 
                	  TxTraficMux_s <= GrantReglTrafic_c;	                  
                   Tx_fsm_s <= EdmaTxcGrand_c;
                elsif RTDEx_STR_TXC_TVALID = '1' and (RTDEx_STR_TXC_TDATA = v32_TxcFlagWord_c) then                                                     
                   TxTraficMux_s <= GrantRtdexTrafic_c;
                   Tx_fsm_s <= RtdexTxcGrand_c;   
                end if;
               
            when PauseReqTxcGrand_c =>
               -- Just wait for TXC last word --   
               if (AXI_STR_RXS_TREADY = '1' and PFG_STR_TXC_TVALID = '1' and PFG_STR_TXC_TLAST = '1') then                                    
                  Tx_fsm_s <= PauseReqTxdGrand_c;
               end if;
                  
            when EdmaTxcGrand_c =>
               -- Just wait for TXC last word --   
               if (AXI_STR_RXS_TREADY = '1' and AXI_STR_TXC_TVALID = '1' and AXI_STR_TXC_TLAST = '1') then                                    
                  Tx_fsm_s <= EdmaTxdGrand_c;
               end if;
            
            when RtdexTxcGrand_c =>
               -- Just wait for TXC last word --   
               if (AXI_STR_RXS_TREADY = '1' and RTDEx_STR_TXC_TVALID = '1' and RTDEx_STR_TXC_TLAST = '1') then                                    
                  Tx_fsm_s <= RtdexTxdGrand_c;
               end if;
           
           when PauseReqTxdGrand_c =>   
               -- Wait for TXD last word --   
               if (AXI_STR_RXD_TREADY = '1' and PFG_STR_TXD_TVALID = '1' and PFG_STR_TXD_TLAST = '1') then                  
                  Tx_fsm_s <= idle_c;
               end if;   
                   
           when EdmaTxdGrand_c =>   
               -- Wait for TXD last word --   
               if (AXI_STR_RXD_TREADY = '1' and AXI_STR_TXD_TVALID = '1' and AXI_STR_TXD_TLAST = '1') then                  
                  Tx_fsm_s <= idle_c;
               end if;   
                   
           when RtdexTxdGrand_c =>    
               -- Wait for TXD last word --
               if (AXI_STR_RXD_TREADY = '1' and RTDEx_STR_TXD_TVALID = '1' and RTDEx_STR_TXD_TLAST = '1') then                  
                  Tx_fsm_s <= idle_c;
               end if;   

 				when others =>
 					Tx_fsm_s <= idle_c;
 				   
 			end case;   
 		end if;   
 	end if;
  end process;
  
 -----------------------------------------------------------------------
 --- RXS output Mux  --- 
 -----------------------------------------------------------------------
 process(TxTraficMux_s,
 		 AXI_STR_TXC_TDATA, AXI_STR_TXC_TKEEP, AXI_STR_TXC_TLAST, AXI_STR_TXC_TVALID,
 		 RTDEx_STR_TXC_TDATA, RTDEx_STR_TXC_TKEEP, RTDEx_STR_TXC_TLAST, RTDEx_STR_TXC_TVALID,
 		 PFG_STR_TXC_TDATA, PFG_STR_TXC_TKEEP, PFG_STR_TXC_TLAST, PFG_STR_TXC_TVALID)
 begin
 	case TxTraficMux_s is
 	
 		when Closed_c =>
 		
 			AXI_STR_RXS_TDATA    <= (others=>'0');
 		    AXI_STR_RXS_TKEEP    <= (others=>'0');
 		    AXI_STR_RXS_TLAST    <= '0';
 		    AXI_STR_RXS_TVALID   <= '0';
 		    -- Ready signals
 		    PFG_STR_TXC_TREADY   <= '0';
 		    AXI_STR_TXC_TREADY   <= '0';
 		    RTDEX_STR_TXC_TREADY <= '0';
 		    
 		    
 		when PauseReqTrafic_c =>
 			AXI_STR_RXS_TDATA    <= PFG_STR_TXC_TDATA; 
 		    AXI_STR_RXS_TKEEP    <= PFG_STR_TXC_TKEEP; 
 		    AXI_STR_RXS_TLAST    <= PFG_STR_TXC_TLAST; 
 		    AXI_STR_RXS_TVALID   <= PFG_STR_TXC_TVALID;
 		    -- Ready for Pause Frame req gen trafic signal
 		    PFG_STR_TXC_TREADY   <= AXI_STR_RXS_TREADY;
 		    -- Disable uBlaze trafic
 		    AXI_STR_TXC_TREADY   <= '0';
 		    -- Disable RTDEx trafic
 		    RTDEX_STR_TXC_TREADY <= '0';
 		
 		when GrantReglTrafic_c =>
 		
 			AXI_STR_RXS_TDATA    <= AXI_STR_TXC_TDATA; 
 		    AXI_STR_RXS_TKEEP    <= AXI_STR_TXC_TKEEP; 
 		    AXI_STR_RXS_TLAST    <= AXI_STR_TXC_TLAST; 
 		    AXI_STR_RXS_TVALID   <= AXI_STR_TXC_TVALID;
 		    -- Ready for uBalze trafic signal
 		    AXI_STR_TXC_TREADY   <= AXI_STR_RXS_TREADY;
 		    -- Disable PFG trafic
 		    PFG_STR_TXC_TREADY   <= '0';
 		    -- Disable RTDEx trafic
 		    RTDEX_STR_TXC_TREADY <= '0';
 		
 		when GrantRtdexTrafic_c =>
 		
 			AXI_STR_RXS_TDATA    <= RTDEx_STR_TXC_TDATA;  
 		    AXI_STR_RXS_TKEEP    <= RTDEx_STR_TXC_TKEEP;  
 		    AXI_STR_RXS_TLAST    <= RTDEx_STR_TXC_TLAST;  
 		    AXI_STR_RXS_TVALID   <= RTDEx_STR_TXC_TVALID; 
 		    -- Ready for RTDEx trafic signal
 		    RTDEX_STR_TXC_TREADY   <= AXI_STR_RXS_TREADY;
 		    -- Disable PFG trafic
 		    PFG_STR_TXC_TREADY   <= '0';
 		    -- Disable uBlaze trafic
 		    AXI_STR_TXC_TREADY   <= '0';
 		
 		when others =>
 		
 			AXI_STR_RXS_TDATA    <= (others=>'0');
 		    AXI_STR_RXS_TKEEP    <= (others=>'0');
 		    AXI_STR_RXS_TLAST    <= '0';
 		    AXI_STR_RXS_TVALID   <= '0';
 		    -- Ready signals
 		    PFG_STR_TXC_TREADY   <= '0';
 		    AXI_STR_TXC_TREADY   <= '0';
 		    RTDEX_STR_TXC_TREADY <= '0';
 		
 	end case;
 	
 end process;
 
 -----------------------------------------------------------------------
 --- RXD output Mux  --- 
 -----------------------------------------------------------------------
 process(TxTraficMux_s,AXI_STR_RXD_TREADY,
 		 AXI_STR_TXD_TDATA, AXI_STR_TXD_TKEEP, AXI_STR_TXD_TLAST, AXI_STR_TXD_TVALID,
 		 RTDEx_STR_TXD_TDATA, RTDEx_STR_TXD_TKEEP, RTDEx_STR_TXD_TLAST, RTDEx_STR_TXD_TVALID,
 		 PFG_STR_TXD_TDATA, PFG_STR_TXD_TKEEP, PFG_STR_TXD_TLAST, PFG_STR_TXD_TVALID)
 begin
 	case TxTraficMux_s is
 	
 		when Closed_c =>
 		
 			AXI_STR_RXD_TDATA    <= (others=>'0');
 		    AXI_STR_RXD_TKEEP    <= (others=>'0');
 		    AXI_STR_RXD_TLAST    <= '0';
 		    AXI_STR_RXD_TVALID   <= '0';
 		    -- Ready signals
 		    PFG_STR_TXD_TREADY   <= '0';
 		    AXI_STR_TXD_TREADY   <= '0';
 		    RTDEX_STR_TXD_TREADY <= '0';
 		    
 		when PauseReqTrafic_c =>
 		
 			AXI_STR_RXD_TDATA    <= PFG_STR_TXD_TDATA; 
 		    AXI_STR_RXD_TKEEP    <= PFG_STR_TXD_TKEEP; 
 		    AXI_STR_RXD_TLAST    <= PFG_STR_TXD_TLAST; 
 		    AXI_STR_RXD_TVALID   <= PFG_STR_TXD_TVALID;
 		    -- Ready for Pause Frame req gen trafic signal
 		    PFG_STR_TXD_TREADY   <= AXI_STR_RXD_TREADY;
 		    -- Disable uBlaze trafic
 		    AXI_STR_TXD_TREADY   <= '0';
 		    -- Disable RTDEx trafic
 		    RTDEX_STR_TXD_TREADY <= '0';
 		
 		when GrantReglTrafic_c =>
 		
 			AXI_STR_RXD_TDATA    <= AXI_STR_TXD_TDATA; 
 		    AXI_STR_RXD_TKEEP    <= AXI_STR_TXD_TKEEP; 
 		    AXI_STR_RXD_TLAST    <= AXI_STR_TXD_TLAST; 
 		    AXI_STR_RXD_TVALID   <= AXI_STR_TXD_TVALID;
 		    -- Ready for uBalze trafic signal
 		    AXI_STR_TXD_TREADY   <= AXI_STR_RXD_TREADY;
 		    -- Disable PFG trafic
 		    PFG_STR_TXD_TREADY   <= '0';
 		    -- Disable RTDEx trafic
 		    RTDEX_STR_TXD_TREADY <= '0';
 		
 		when GrantRtdexTrafic_c =>
 		
 			AXI_STR_RXD_TDATA    <= RTDEx_STR_TXD_TDATA;  
 		    AXI_STR_RXD_TKEEP    <= RTDEx_STR_TXD_TKEEP;  
 		    AXI_STR_RXD_TLAST    <= RTDEx_STR_TXD_TLAST;  
 		    AXI_STR_RXD_TVALID   <= RTDEx_STR_TXD_TVALID;
 		    -- Ready for RTDEx trafic signal 
 		    RTDEX_STR_TXD_TREADY   <= AXI_STR_RXD_TREADY;
 		    -- Disable PFG trafic
 		    PFG_STR_TXD_TREADY   <= '0';
 		    -- Disable uBlaze trafic
 		    AXI_STR_TXD_TREADY   <= '0';
 		
 		when others =>
 		
 			AXI_STR_RXD_TDATA    <= (others=>'0');
 		    AXI_STR_RXD_TKEEP    <= (others=>'0');
 		    AXI_STR_RXD_TLAST    <= '0';
 		    AXI_STR_RXD_TVALID   <= '0';
 		    -- Ready signals
 		    PFG_STR_TXD_TREADY   <= '0';
 		    AXI_STR_TXD_TREADY   <= '0';
 		    RTDEX_STR_TXD_TREADY <= '0';
 		
 	end case;
 	
 end process;
 
 -- Output ports --
 -- RTDEX
 RTDEX_STR_TXD_ACLK     <= AXI_STR_TXD_ACLK;
 RTDEX_STR_TXD_ARESETN  <= AXI_STR_TXD_ARESETN;
 RTDEX_STR_TXC_ACLK     <= AXI_STR_TXC_ACLK;
 RTDEX_STR_TXC_ARESETN  <= AXI_STR_TXC_ARESETN;
 -- PFG
 PFG_STR_TXD_ACLK     <= AXI_STR_TXD_ACLK;
 PFG_STR_TXD_ARESETN  <= AXI_STR_TXD_ARESETN;
 PFG_STR_TXC_ACLK     <= AXI_STR_TXC_ACLK;
 PFG_STR_TXC_ARESETN  <= AXI_STR_TXC_ARESETN;
 
end behav;         