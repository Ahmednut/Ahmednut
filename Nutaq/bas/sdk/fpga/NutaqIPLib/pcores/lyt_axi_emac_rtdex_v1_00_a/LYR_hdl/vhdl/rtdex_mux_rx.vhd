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

entity rtdex_mux_rx is   
   port(
   		 -- config signals from/to axi registers --
		 iv32_RemoteEndMacAddrH_p    : in  std_logic_vector(31 downto 0); 
		 iv16_RemoteEndMacAddrL_p    : in  std_logic_vector(15 downto 0); 
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
         
         RTDEx_STR_RXS_ACLK         : out  std_logic;                         --  AXI-Stream Receive Status Clk 
         RTDEx_STR_RXS_ARESETN      : out  std_logic;                         --  AXI-Stream Receive Status Reset
         RTDEx_STR_RXS_TVALID       : out std_logic;                          --  AXI-Stream Receive Status Valid
         RTDEx_STR_RXS_TREADY       : in  std_logic;                          --  AXI-Stream Receive Status Ready
         RTDEx_STR_RXS_TLAST        : out std_logic;                          --  AXI-Stream Receive Status Last
         RTDEx_STR_RXS_TKEEP        : out std_logic_vector(3 downto 0);       --  AXI-Stream Receive Status Keep
         RTDEx_STR_RXS_TDATA        : out std_logic_vector(31 downto 0)      --  AXI-Stream Receive Status Data
         );
end rtdex_mux_rx;
 
architecture behav of rtdex_mux_rx is

-- attribute keep hierarchy is just for CS dbg. Can be removed
-- attribute keep_hierarchy : string;                         
-- attribute keep_hierarchy of behav : architecture is "true";

----------------------

 type Rx_fsm_t is
   (
	idle_c,
	WaitForTxcLastWord_c,
	destMacAdrsHigh_c,
	destLSrcHMacAdr_c,
	srcMacAdrsLow_c,
	CheckSnapId_c,
	CheckOrgId_c,
	CheckUserId_c,
	RegularPackets_c,
	RtdexPackets_c,
	WaitForTxdLastWord_c,
	WaitForRxdLastWord_c
   );
 signal Rx_fsm_s : Rx_fsm_t;

  type v2_Trafic_mux_t is
	(
	Closed_c,
	RegularTrafic_c,
	RtdexTrafic_c
	);
 signal v2_Trafic_mux_s : v2_Trafic_mux_t;	

 signal AXI_STR_TXD_TDATA_LittleEndian_s : std_logic_vector(31 downto 0);
 signal reset_s       : std_logic;
 signal Txd_wr_en_s       : std_logic;
 signal Rxd_rd_en_s       : std_logic;
 signal v37_Txd_din_s     : std_logic_vector(36 downto 0);
 signal v37_Rxd_dout_s    : std_logic_vector(36 downto 0);
 signal Txd_overflow_s    : std_logic;
 signal Rxd_underflow_s   : std_logic;
 signal Txd_almost_full_s : std_logic;
 signal Rxd_empty_s       : std_logic;
 signal Rxd_Dvalid_s  : std_logic;
 signal RXD_BufferFifoErr_s : std_logic;
 
 signal reset1_s       : std_logic;
 signal Txc_wr_en_s       : std_logic;
 signal Rxs_rd_en_s       : std_logic;
 signal v37_Txc_din_s     : std_logic_vector(36 downto 0);
 signal v37_Rxs_dout_s    : std_logic_vector(36 downto 0);
 signal Txc_overflow_s    : std_logic;
 signal Rxs_underflow_s   : std_logic;
 signal Txc_almost_full_s : std_logic;
 signal Rxs_empty_s       : std_logic;
 signal Rxs_dvalid_s  : std_logic;
 signal RXS_BufferFifoErr_s : std_logic;
 
 signal Rxc_Ready_s  : std_logic;
 signal Rxd_Ready_s  : std_logic;
 
 signal RxdFifoDvalid_dly1_s : std_logic;
 signal Txd_ready_s          : std_logic;
 signal Txc_ready_s          : std_logic;

 signal v32_DestMacAddrsH_s	 	: std_logic_vector(31 downto 0);
 signal v32_DestMacL_SrcMacH_s  : std_logic_vector(31 downto 0);
 signal v32_SrcMacAddrsL_s		: std_logic_vector(31 downto 0);
 
-- attribute keep : string;
-- attribute keep of RXD_BufferFifoErr_s : signal is "true";
-- attribute keep of RXS_BufferFifoErr_s : signal is "true";


COMPONENT fifo_w37_d16
  PORT (
    rst : IN STD_LOGIC;
    wr_clk : IN STD_LOGIC;
    rd_clk : IN STD_LOGIC;
    din : IN STD_LOGIC_VECTOR(36 DOWNTO 0);
    wr_en : IN STD_LOGIC;
    rd_en : IN STD_LOGIC;
    dout : OUT STD_LOGIC_VECTOR(36 DOWNTO 0);
    full : OUT STD_LOGIC;
    almost_full : OUT STD_LOGIC;
    overflow : OUT STD_LOGIC;
    empty : OUT STD_LOGIC;
    valid : OUT STD_LOGIC;
    underflow : OUT STD_LOGIC
  );
END COMPONENT;


COMPONENT fifo_w37_d32
  PORT (
    rst : IN STD_LOGIC;
    wr_clk : IN STD_LOGIC;
    rd_clk : IN STD_LOGIC;
    din : IN STD_LOGIC_VECTOR(36 DOWNTO 0);
    wr_en : IN STD_LOGIC;
    rd_en : IN STD_LOGIC;
    dout : OUT STD_LOGIC_VECTOR(36 DOWNTO 0);
    full : OUT STD_LOGIC;
    almost_full : OUT STD_LOGIC;
    overflow : OUT STD_LOGIC;
    empty : OUT STD_LOGIC;
    valid : OUT STD_LOGIC;
    underflow : OUT STD_LOGIC;
    prog_full : OUT STD_LOGIC
  );
END COMPONENT;

begin

   

--- Byte swap ---
 AXI_STR_TXD_TDATA_LittleEndian_s(7 downto 0)   <= AXI_STR_TXD_TDATA(31 downto 24);
 AXI_STR_TXD_TDATA_LittleEndian_s(15 downto 8)  <= AXI_STR_TXD_TDATA(23 downto 16);
 AXI_STR_TXD_TDATA_LittleEndian_s(23 downto 16) <= AXI_STR_TXD_TDATA(15 downto 8);
 AXI_STR_TXD_TDATA_LittleEndian_s(31 downto 24) <= AXI_STR_TXD_TDATA(7 downto 0);
 
 -- Mac address bus concatination:
 v32_DestMacAddrsH_s <= iv32_LocalEndMacAddrH_p;
 v32_DestMacL_SrcMacH_s <= iv16_LocalEndMacAddrL_p & iv32_RemoteEndMacAddrH_p(31 downto 16);  
 v32_SrcMacAddrsL_s <= iv32_RemoteEndMacAddrH_p(15 downto 0) & iv16_RemoteEndMacAddrL_p;

 ------------------------------------------------------------------------------
 -- Ethernet RX Muxer main FSM.
 -- Here, we verify the header against what's expected to be RTDEx header. If
 -- the header isn't RTDEx, then it'll be just forwarded to uBlaze MCU.
 ------------------------------------------------------------------------------
 process(AXI_STR_TXD_ACLK)
 begin
	if rising_edge(AXI_STR_TXD_ACLK)then
		if AXI_STR_TXD_ARESETN = '0' then          
			Rx_fsm_s <= idle_c;
			Rxc_Ready_s <= '0';
			Rxd_Ready_s <= '0';
			v2_Trafic_mux_s <= Closed_c;
		else  
			case Rx_fsm_s is
		
				when idle_c =>
				   Rxd_Ready_s <= '0';
			      v2_Trafic_mux_s <= Closed_c;
					Rxc_Ready_s <= '1';
					
					if AXI_STR_TXC_TVALID = '1' then
						Rx_fsm_s <= WaitForTxcLastWord_c;					
					end if;
					
				when WaitForTxcLastWord_c =>
				
					if (AXI_STR_TXC_TVALID = '1' and AXI_STR_TXC_TLAST = '1') then
						Rxc_Ready_s <= '0';
						Rxd_Ready_s <= '1';
						Rx_fsm_s <= destMacAdrsHigh_c;
					elsif AXI_STR_TXC_TVALID = '0' then
					   Rx_fsm_s <= idle_c;	
					end if;
					  
				when destMacAdrsHigh_c => 
					if AXI_STR_TXD_TVALID = '1' then
						if AXI_STR_TXD_TDATA_LittleEndian_s = v32_DestMacAddrsH_s then
							Rx_fsm_s <= destLSrcHMacAdr_c;
						else
							Rx_fsm_s <= RegularPackets_c;
						end if;
					end if;
							   
				when destLSrcHMacAdr_c =>
					if AXI_STR_TXD_TVALID = '1' then
						if AXI_STR_TXD_TDATA_LittleEndian_s = v32_DestMacL_SrcMacH_s then
							Rx_fsm_s <= srcMacAdrsLow_c;
						else
							Rx_fsm_s <= RegularPackets_c;
						end if;		
					else
					   Rx_fsm_s <= idle_c;		
					end if;				            
											  
				when srcMacAdrsLow_c => 
					if AXI_STR_TXD_TVALID = '1' then
						if AXI_STR_TXD_TDATA_LittleEndian_s = v32_SrcMacAddrsL_s then
							Rx_fsm_s <= CheckSnapId_c;
						else
							Rx_fsm_s <= RegularPackets_c;
						end if;
					else
					   Rx_fsm_s <= idle_c;		
				end if;	
			
				when CheckSnapId_c => -- EtherType is not verified here to give more flexibility to use any.
					if AXI_STR_TXD_TVALID = '1' then
						if AXI_STR_TXD_TDATA_LittleEndian_s(15 downto 0) = v16_SnapId_c then
							Rx_fsm_s <= CheckOrgId_c;
						else
							Rx_fsm_s <= RegularPackets_c;                     
						end if;
					else
					   Rx_fsm_s <= idle_c;		
					end if;
													   
				when CheckOrgId_c =>
					if AXI_STR_TXD_TVALID = '1' then
						if AXI_STR_TXD_TDATA_LittleEndian_s = (v4_Ctrl_c & v24_OrgId_c) then
							Rx_fsm_s <= CheckUserId_c;
						else
							Rx_fsm_s <= RegularPackets_c;                     
						end if;
					else
					   Rx_fsm_s <= idle_c;		
					end if;
	 
				when CheckUserId_c => -- Check in RX module to count for a bad/droped frame
					if AXI_STR_TXD_TVALID = '1' then						
						Rx_fsm_s <= RtdexPackets_c;						
					else
					    Rx_fsm_s <= idle_c;		
					end if;
			   
				when RegularPackets_c =>
				   if AXI_STR_TXD_TVALID = '1' then
					   v2_Trafic_mux_s <= RegularTrafic_c;
					   Rx_fsm_s <= WaitForTxdLastWord_c;
					else
					   Rx_fsm_s <= idle_c;	
					end if;   
				
				when RtdexPackets_c =>
				   if AXI_STR_TXD_TVALID = '1' then
					   v2_Trafic_mux_s <= RtdexTrafic_c;
					   Rx_fsm_s <= WaitForTxdLastWord_c;
					else
					   Rx_fsm_s <= idle_c;	
					end if;   
				
				when WaitForTxdLastWord_c	=>
					-- wait for end of the current packet
					if (AXI_STR_TXD_TVALID = '1' and AXI_STR_TXD_TLAST = '1') then
						Rxd_Ready_s <= '0';
						Rx_fsm_s <= WaitForRxdLastWord_c;
					elsif AXI_STR_TXD_TVALID = '0' then
					   Rx_fsm_s <= idle_c;		
					end if;   
					
				when WaitForRxdLastWord_c =>	
					if RxdFifoDvalid_dly1_s = '0' then
						v2_Trafic_mux_s <= Closed_c;
						Rx_fsm_s <= idle_c;
					end if;
					
				when others =>
					Rx_fsm_s <= idle_c;
				   
			end case;   
		end if;   
	end if;
 end process;

--- Rxd_Dvalid_s: Clock domain crossing. 
 process(AXI_STR_RXD_ACLK)
 begin
	if rising_edge(AXI_STR_RXD_ACLK)then
		RxdFifoDvalid_dly1_s <= Rxd_Dvalid_s;		  
   end if;
 end process;
  
-----------------------------------------------------------------------
--- RXD Buffer FIFO --- 
-- Depth = 32 but set to signle prog full @ 28 for security.
-----------------------------------------------------------------------
 v37_Txd_din_s <= AXI_STR_TXD_TLAST & AXI_STR_TXD_TKEEP & AXI_STR_TXD_TDATA;
 Txd_ready_s <= Rxd_Ready_s and not(Txd_almost_full_s);
 Txd_wr_en_s <= AXI_STR_TXD_TVALID and Txd_ready_s;
 
 reset_s <= not(AXI_STR_TXD_ARESETN);
 --RXD_BufferFifoErr_s <= Txd_overflow_s or Rxd_underflow_s;
 
 BufferFifo_RXD : fifo_w37_d32
  PORT MAP (
    rst => reset_s,
    wr_clk => AXI_STR_TXD_ACLK,
    rd_clk => AXI_STR_RXD_ACLK,
    din => v37_Txd_din_s,
    wr_en => Txd_wr_en_s,
    rd_en => Rxd_rd_en_s,
    dout => v37_Rxd_dout_s,
    full => open,
    almost_full => open,
    overflow => Txd_overflow_s,
    empty => Rxd_empty_s,
    valid => Rxd_Dvalid_s,
    underflow => Rxd_underflow_s,
    prog_full => Txd_almost_full_s
  );
  
-----------------------------------------------------------------------  
--- RXS Buffer FIFO ---
 -----------------------------------------------------------------------
 v37_Txc_din_s <= AXI_STR_TXC_TLAST & AXI_STR_TXC_TKEEP & AXI_STR_TXC_TDATA;
 Txc_ready_s   <= Rxc_Ready_s and not(Txc_almost_full_s);
 Txc_wr_en_s <= AXI_STR_TXC_TVALID and Txc_ready_s;
 
 reset1_s <= not(AXI_STR_TXC_ARESETN);
 --RXS_BufferFifoErr_s <= Txc_overflow_s or Rxs_underflow_s;
 
 BufferFifo_RXS : fifo_w37_d16
  PORT MAP (
    rst => reset1_s,
    wr_clk => AXI_STR_TXC_ACLK,
    rd_clk => AXI_STR_RXS_ACLK,
    din => v37_Txc_din_s,
    wr_en => Txc_wr_en_s,
    rd_en => Rxs_rd_en_s,
    dout => v37_Rxs_dout_s,
    full => open,
    almost_full => Txc_almost_full_s,
    overflow => Txc_overflow_s,
    empty => Rxs_empty_s,
    valid => Rxs_dvalid_s,
    underflow => Rxs_underflow_s
  );

 ------------------------------------------------------------------------ 
 --- Output RXD Mux ---
 ------------------------------------------------------------------------
 Process(v2_Trafic_mux_s, v37_Rxd_dout_s, Rxd_empty_s, Rxd_Dvalid_s, AXI_STR_RXD_TREADY, rtdex_str_rxd_tready)
 begin
	case v2_Trafic_mux_s is
		when RegularTrafic_c => 						
			AXI_STR_RXD_TDATA <= v37_Rxd_dout_s(31 downto 0);
			AXI_STR_RXD_TKEEP <= v37_Rxd_dout_s(35 downto 32);
			AXI_STR_RXD_TLAST <= v37_Rxd_dout_s(36);
			AXI_STR_RXD_TVALID <= Rxd_Dvalid_s;      
			Rxd_rd_en_s <= AXI_STR_RXD_TREADY and not(Rxd_empty_s);
			--      
			RTDEx_STR_RXD_TDATA <= (others=>'0');
			RTDEx_STR_RXD_TKEEP <= (others=>'0');
			RTDEx_STR_RXD_TLAST <= '0';
			RTDEx_STR_RXD_TVALID <='0';			
						
		when RtdexTrafic_c =>  						      
			RTDEx_STR_RXD_TDATA <= v37_Rxd_dout_s(31 downto 0);
			RTDEx_STR_RXD_TKEEP <= v37_Rxd_dout_s(35 downto 32);
			RTDEx_STR_RXD_TLAST <= v37_Rxd_dout_s(36);
			RTDEx_STR_RXD_TVALID <= Rxd_Dvalid_s;      
			Rxd_rd_en_s <= RTDEx_STR_RXD_TREADY and not(Rxd_empty_s);
			--
			AXI_STR_RXD_TDATA  <= (others=>'0');
			AXI_STR_RXD_TKEEP  <= (others=>'0');
			AXI_STR_RXD_TLAST  <= '0';
			AXI_STR_RXD_TVALID <='0';			
			
		when others =>
			AXI_STR_RXD_TDATA  <= (others=>'0');
		    AXI_STR_RXD_TKEEP  <= (others=>'0');
			AXI_STR_RXD_TLAST <= '0';
			AXI_STR_RXD_TVALID <='0';			
			--
			RTDEx_STR_RXD_TDATA <= (others=>'0');
			RTDEx_STR_RXD_TKEEP <= (others=>'0');
			RTDEx_STR_RXD_TLAST <= '0';
			RTDEx_STR_RXD_TVALID <='0';	
			Rxd_rd_en_s <= '0';			
	end case;		
 end process;  
 
 ------------------------------------------------------------------------ 
 --- Output RXS Mux ---
 ------------------------------------------------------------------------
 Process(v2_Trafic_mux_s, v37_Rxs_dout_s, Rxs_empty_s, Rxs_Dvalid_s, AXI_STR_RXS_TREADY, rtdex_str_rxs_tready)
 begin
	case v2_Trafic_mux_s is
		when RegularTrafic_c =>
			AXI_STR_RXS_TDATA <= v37_Rxs_dout_s(31 downto 0);
			AXI_STR_RXS_TKEEP <= v37_Rxs_dout_s(35 downto 32);
			AXI_STR_RXS_TLAST <= v37_Rxs_dout_s(36);
			AXI_STR_RXS_TVALID <= Rxs_Dvalid_s;      
			Rxs_rd_en_s <= AXI_STR_RXS_TREADY and not(Rxs_empty_s);
			--      
			RTDEx_STR_RXS_TDATA <= (others=>'0');  
			RTDEx_STR_RXS_TKEEP <= (others=>'0'); 
			RTDEx_STR_RXS_TLAST <= '0';
			RTDEx_STR_RXS_TVALID <='0';	          
			
		when RtdexTrafic_c  =>						      
			RTDEx_STR_RXS_TDATA <= v37_Rxs_dout_s(31 downto 0);
			RTDEx_STR_RXS_TKEEP <= v37_Rxs_dout_s(35 downto 32);
			RTDEx_STR_RXS_TLAST <= v37_Rxs_dout_s(36);
			RTDEx_STR_RXS_TVALID <= Rxs_Dvalid_s;      
			Rxs_rd_en_s <= RTDEx_STR_RXS_TREADY and not(Rxs_empty_s);
			--      
			AXI_STR_RXS_TDATA <= (others=>'0'); 
			AXI_STR_RXS_TKEEP <= (others=>'0'); 
			AXI_STR_RXS_TLAST <= '0';
			AXI_STR_RXS_TVALID <='0';			
			
		when others =>
			AXI_STR_RXS_TDATA <= (others=>'0'); 
		    AXI_STR_RXS_TKEEP <= (others=>'0'); 
			AXI_STR_RXS_TLAST <= '0';
			AXI_STR_RXS_TVALID <='0';
			--
			RTDEx_STR_RXS_TDATA <= (others=>'0');  		
			RTDEx_STR_RXS_TKEEP <= (others=>'0');  
			RTDEx_STR_RXS_TLAST <= '0';
			RTDEx_STR_RXS_TVALID <='0';	
			Rxs_rd_en_s <= '0';
			
	end case;		
 end process;
 
 -----------------------------------------------    
 -- Outputs mappings ---
 -----------------------------------------------
 AXI_STR_TXD_TREADY <= Txd_ready_s;
 AXI_STR_TXC_TREADY <= Txc_ready_s;
 
 RTDEx_STR_RXD_ACLK       <= AXI_STR_RXD_ACLK;
 RTDEx_STR_RXD_ARESETN    <= AXI_STR_RXD_ARESETN;
 
 RTDEx_STR_RXS_ACLK       <= AXI_STR_RXS_ACLK;
 RTDEx_STR_RXS_ARESETN    <= AXI_STR_RXS_ARESETN;
   
end behav;         