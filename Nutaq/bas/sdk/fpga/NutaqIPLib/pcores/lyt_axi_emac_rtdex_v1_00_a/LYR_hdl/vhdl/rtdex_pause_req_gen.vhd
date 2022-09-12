--------------------------------------------------------------------------------
--
--                       NUTAQ   R D   I N C
--
--------------------------------------------------------------------------------
-- File        : rtdex_pause_req_gen.vhd
--------------------------------------------------------------------------------
--
-- Description : Generates pause frame request to the transmitting end. It work
--				The same way a RTDEx Tx works with a streaming interface except
--				thta it only send a pause frames with max value or with a zero
--				value to cancel a pause.
--
-- Designedr: Khalid Bensadek
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq RD inc.
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
--use ieee.std_logic_misc.all;

library work;
use work.rtdex_defines.all;


 entity rtdex_pause_req_gen is
 	port(
 		-- Config side
 		iv32_LocalEndMacAddrH_p  : in  std_logic_vector(31 downto 0); 
        iv16_LocalEndMacAddrL_p  : in  std_logic_vector(15 downto 0); 
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
 end rtdex_pause_req_gen;
 
 architecture rtl of rtdex_pause_req_gen is
 
 -- CTRL FSM
 type FlowCtrl_txc_fsm_t is
 	(
 	Idle_c         , 	
 	SendFlagWord_c ,
 	SendAppWords0_c ,
 	SendAppWords1_c ,
 	SendAppWords2_c ,
 	SendAppWords3_c ,
 	SendAppWords4_c ,
 	WaitForTxdDone_c 	
 	);
 
 signal FlowCtrl_txc_fsm_s : FlowCtrl_txc_fsm_t; 
 
 -- Data FSM
 type FlowCtrl_txd_fsm_t is
 	(
 	Idle_c,
 	DestHMacAdr_c,
    DestLSrcHMacAdr_c,
    SrcLMacAdr_c,
    EtherType_c,    
    PauseVal_c,
    WaitForDone_c
 	);
 	
 signal FlowCtrl_txd_fsm_s : FlowCtrl_txd_fsm_t;
   
 signal v32_RtdexTxc_tdata_s : std_logic_vector(31 downto 0);
 signal v4_RtdexTxc_tkeep_s  	: std_logic_vector(3 downto 0);
 signal RtdexTxc_tvalid_s    : std_logic;  
 signal RtdexTxc_tlast_s     : std_logic; 
 
 signal RtdexTxd_tvalid_s    : std_logic;  
 signal RtdexTxd_tlast_s     : std_logic; 
 signal v4_RtdexTxd_tkeep_s  : std_logic_vector(3 downto 0);
 signal v32_RtdexTxd_tdata_s : std_logic_vector(31 downto 0); 
 signal TXD_OneFrmaeEna_s 	 : std_logic;
 signal Txd_done_s			 : std_logic;
 
 signal v32_DestMacH_s         : std_logic_vector(31 downto 0);
 signal v32_DestMacLSrcMacH_s  : std_logic_vector(31 downto 0);
 signal v32_SrcMacL_s          : std_logic_vector(31 downto 0);
 
   
 begin
   
 -----------------------------------------------------------------------
 -- RTDEx TXC FSM: Tx Control FSM
 -----------------------------------------------------------------------
 process(RTDEX_STR_TXC_ACLK)
 begin
   if rising_edge(RTDEX_STR_TXC_ACLK) then
   	  if RTDEX_STR_TXC_ARESETN = '0' then
   	  	FlowCtrl_txc_fsm_s <= Idle_c;
   	  	v4_RtdexTxc_tkeep_s <= (others=>'0');
   	  	v32_RtdexTxc_tdata_s <= (others=>'0');
   	  	RtdexTxc_tvalid_s <= '0';
        RtdexTxc_tlast_s <= '0'; 
        TXD_OneFrmaeEna_s <= '0';         
   	  else
   	  
   	  	-- Default values
        v4_RtdexTxc_tkeep_s <= x"F";        
        TXD_OneFrmaeEna_s <= '0';
   	  
   	  	case FlowCtrl_txc_fsm_s is
   	  	
   	  		when Idle_c =>
   	  			if i_SendPause_p = '1' then										
   	  				-- Send Flag word (constant fixed by Xilinx) --
                   	v32_RtdexTxc_tdata_s <= v32_TxcFlagWord_c;     
                   	RtdexTxc_tvalid_s <= '1';
              	 	FlowCtrl_txc_fsm_s <= SendFlagWord_c;
              	 end if;	
   	  		
   	  		when SendFlagWord_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                   	v32_RtdexTxc_tdata_s <= v32_TxFCSAppWord_c; -- app Word 0                   	
                   	FlowCtrl_txc_fsm_s <= SendAppWords0_c;
               	 end if;
               	
              when SendAppWords0_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 1                                                                                
                    FlowCtrl_txc_fsm_s <= SendAppWords1_c;                      
                 end if;
                 
              when SendAppWords1_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 2                                                                                
                    FlowCtrl_txc_fsm_s <= SendAppWords2_c;                      
                 end if;   
               
              when SendAppWords2_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 3                                                                                
                    FlowCtrl_txc_fsm_s <= SendAppWords3_c;                      
                 end if;
                 
              when SendAppWords3_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 3                                                                                
                    RtdexTxc_tlast_s <= '1';                                        
                    FlowCtrl_txc_fsm_s <= SendAppWords4_c;                         
                 end if;        
                 
              when SendAppWords4_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c;  -- app Word 4                       
                    -- Turn of TXC channel
                 	RtdexTxc_tlast_s    <= '0';
                 	TXD_OneFrmaeEna_s <= '1';  -- Enable TXD FSM
                 	RtdexTxc_tvalid_s   <= '0';	
                    FlowCtrl_txc_fsm_s <= WaitForTxdDone_c;                      
                 end if;
   	  		
   	  		when WaitForTxdDone_c =>
   	  			if Txd_done_s = '1' then   	  				
   	  				FlowCtrl_txc_fsm_s <= Idle_c;
   	  			end if;
   	  		
   	  		
   	  		when others => FlowCtrl_txc_fsm_s <= Idle_c;
   	  	end case;
   	  end if;
   end if;
 end process;
 
 -- Destination and source mac addresses concatenation.
 v32_DestMacH_s <= iv48_MulticastMacAddr_c(47 downto 16);
 v32_DestMacLSrcMacH_s <= iv48_MulticastMacAddr_c(15 downto 0) & iv32_LocalEndMacAddrH_p(31 downto 16);
 v32_SrcMacL_s <= iv32_LocalEndMacAddrH_p(15 downto 0) & iv16_LocalEndMacAddrL_p;
    	
 -----------------------------------------------------------------------
 -- RTDEx TXC FSM: Tx data FSM
 -----------------------------------------------------------------------
 process(RTDEX_STR_TXD_ACLK)
 begin
   if rising_edge(RTDEX_STR_TXD_ACLK) then
      if RTDEX_STR_TXD_ARESETN = '0' then 
      	FlowCtrl_txd_fsm_s <= Idle_c;
      	Txd_done_s <= '0';
      	v32_RtdexTxd_tdata_s <= (others=>'0');
      	v4_RtdexTxd_tkeep_s <= (others=>'0');
         RtdexTxd_tvalid_s   <= '0';
         RtdexTxd_tlast_s    <= '0';         
      else
      
      	-- DEfault value:
      	Txd_done_s <= '0';
      	        
      	      	
      	case FlowCtrl_txd_fsm_s is
      	
      		when Idle_c => 
      			-- Wait for enable from TXC FSM
               if TXD_OneFrmaeEna_s = '1' then
               	  v32_RtdexTxd_tdata_s <= v32_DestMacH_s;
               	  v4_RtdexTxd_tkeep_s <= x"F";
                  RtdexTxd_tvalid_s <= '1';
                  FlowCtrl_txd_fsm_s <= DestLSrcHMacAdr_c;	               	  
                end if;
            
            when DestLSrcHMacAdr_c =>
               if RTDEX_STR_TXD_TREADY = '1' then  -- Dest MAC High word accepted
                  v32_RtdexTxd_tdata_s <= v32_DestMacLSrcMacH_s;                  
                  FlowCtrl_txd_fsm_s <= SrcLMacAdr_c;                                    
               end if;   
               
            when SrcLMacAdr_c =>
               if RTDEX_STR_TXD_TREADY = '1' then -- Dest MAC Low 1/2 word & Src MAH H 1/2 word accepted
                  v32_RtdexTxd_tdata_s <= v32_SrcMacL_s;                  
                  FlowCtrl_txd_fsm_s <= EtherType_c;                               
               end if;   
      		               
      		
      		when EtherType_c =>
      			if RTDEX_STR_TXD_TREADY = '1' then -- Src MAH L word accepted      				
      				v32_RtdexTxd_tdata_s <= (v16_MacCtrlType_c & v16_CtrlOpcode_c);                  	
                  	FlowCtrl_txd_fsm_s <= PauseVal_c;
                end if;  	
      		      		      		
      		when PauseVal_c =>      			
      			if RTDEX_STR_TXD_TREADY = '1' then      				
      				v32_RtdexTxd_tdata_s <= (iv16_PauseVal_p & x"0000");      				
      				v4_RtdexTxd_tkeep_s <= x"3";
      				RtdexTxd_tlast_s   <= '1';
                  	FlowCtrl_txd_fsm_s <= WaitForDone_c;
                end if;  	
      		
      		when WaitForDone_c =>
      			if RTDEX_STR_TXD_TREADY = '1' then
      				v32_RtdexTxd_tdata_s <= (others=>'0');
      				v4_RtdexTxd_tkeep_s <= x"0";
      				RtdexTxd_tvalid_s   <= '0';
      				RtdexTxd_tlast_s    <= '0';
      				-- We do not need to do padding our selfes, the axi_ethernet core will do it for us
      				-- because FCS is calculated by the axi_ethernet core. 
      				--If configured differenty, then we need to pad here with exact 40 bytes to reache
      				-- the required min frame length of 64 bytes.
      				Txd_done_s <= '1';
      				FlowCtrl_txd_fsm_s <= Idle_c; 
      			end if;
      		
      		when others => FlowCtrl_txd_fsm_s <= Idle_c;
      	end case;	
      end if;
   end if;
 end process;  
 
 -------------------------
 -- Outputs
 -------------------------
 --- TXC ---
 RTDEX_STR_TXC_TVALID <= RtdexTxc_tvalid_s;
 RTDEX_STR_TXC_TLAST  <= RtdexTxc_tlast_s;
 RTDEX_STR_TXC_TKEEP  <= v4_RtdexTxc_tkeep_s;
 RTDEX_STR_TXC_TDATA  <= v32_RtdexTxc_tdata_s;
 
 --- TXD ---
 -- Data is byte swapped --
 RTDEX_STR_TXD_TDATA(31 downto 24)  <= v32_RtdexTxd_tdata_s(7 downto 0);
 RTDEX_STR_TXD_TDATA(23 downto 16)  <= v32_RtdexTxd_tdata_s(15 downto 8);
 RTDEX_STR_TXD_TDATA(15 downto 8)   <= v32_RtdexTxd_tdata_s(23 downto 16);
 RTDEX_STR_TXD_TDATA(7 downto 0)    <= v32_RtdexTxd_tdata_s(31 downto 24);
 RTDEX_STR_TXD_TVALID <= RtdexTxd_tvalid_s;
 RTDEX_STR_TXD_TLAST  <= RtdexTxd_tlast_s;
 RTDEX_STR_TXD_TKEEP  <= v4_RtdexTxd_tkeep_s;
 
 end rtl;		