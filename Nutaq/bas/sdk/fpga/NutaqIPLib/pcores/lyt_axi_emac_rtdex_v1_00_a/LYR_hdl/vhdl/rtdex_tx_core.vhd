--------------------------------------------------------------------------------
--
--                    NUTAQ   R D   I N C                           
--
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- Notes / Assumptions :
-- Description: 
-- Khalid Bensadek 
-- 2012/010
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: 
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

library work;
use work.rtdex_defines.all;

 entity rtdex_tx_core is
 	port(
 	RTDEX_STR_TXC_ACLK			: in std_logic; 
 	RTDEX_STR_TXC_TREADY		: in std_logic;
 	RTDEX_STR_TXD_ACLK			: in std_logic; 
 	RTDEX_STR_TXD_TREADY 		: in std_logic; 
 	i_TxcRstn_p					: in std_logic;
 	i_SysRstn_p					: in std_logic;
 	--
 	iv32_RemoteEndMacAddrH_p 	: in  std_logic_vector(31 downto 0); 
    iv16_RemoteEndMacAddrL_p 	: in  std_logic_vector(15 downto 0); 		   
    iv32_LocalEndMacAddrH_p 	: in  std_logic_vector(31 downto 0); 
    iv16_LocalEndMacAddrL_p 	: in  std_logic_vector(15 downto 0);    
 	ia8v15_FrameSize_p			: in array8_v15_t;
 	ia8v32_TransferSize_p		: in array8_v32_t;
 	iv8_TxChFrsBurst_p			: in std_logic_vector(7 downto 0);
 	iv32_FrameGap_p				: in std_logic_vector(31 downto 0);  
 	i_Mode_p					: in  std_logic; 	
 	iv16_EtherType_p			: in std_logic_vector(15 downto 0);  
 	iv8_ActiveChannel_p			: in std_logic_vector(7 downto 0);
 	iv8_gotEnoughDataToSend_p	: in std_logic_vector(7 downto 0);
 	--
 	ov32_RtdexTxc_tdata_p		: out std_logic_vector(31 downto 0);
 	o_RtdexTxc_tvalid_p			: out std_logic; 	
 	o_RtdexTxc_tlast_p			: out std_logic;
 	ov4_RtdexTxc_tkeep_p		: out std_logic_vector(3 downto 0); 	
 	ov32_RtdexTxd_tdata_p		: out std_logic_vector(31 downto 0);
 	o_RtdexTxd_tvalid_p  		: out std_logic;
 	o_RtdexTxd_tlast_p   		: out std_logic;
 	ov4_RtdexTxd_tkeep_p 		: out std_logic_vector(3 downto 0);
 	--
 	iv8_StartNewTransfer_p		: in  std_logic_vector(7 downto 0);
 	o_CurrentCh_p				: out integer range 0 to 7;
 	o_GoToNextChan_p			: out std_logic;
 	ov8_Txdata_Rd_p				: out std_logic_vector(7 downto 0);
 	ia8v32_RtdexChData_p		: in array8_v32_t;
 	oa8u32_XferSizeCnt_p		: out array8_u32_t;
 	ov8_CurntChXferDone_p		: out std_logic_vector(7 downto 0);
 	ov8_LoadTxSentFrmCnt_p		: out std_logic_vector(7 downto 0)
 	
 	);
 end rtdex_tx_core;
 
 architecture rtdex_TxCore_rtl of rtdex_tx_core is
 
 -- ################## Constants definition ################################# 
 constant FrameCntExpired_c : unsigned(15 downto 0):=x"0001";
 -- ################## Constants definition ################################
 
 -------------------------------------------------------------------------
 -- Function that returns the position (index) of the one hot bit position.
 -- Used to figure-out the channel number. Note that if no bit is on, 0 is
 -- returned. This ensures channel 0 is always default selected.
 -------------------------------------------------------------------------
  function OneHotBit2ChNumb( InVect : std_logic_vector;                              
                InVectWidth : integer)                                             
                return integer
  is                                                                         
    variable Result                   : integer;

  begin                        
    Result := 0; 
    for i in 0 to InVectWidth-1 loop
      if InVect(i)= '1' then
         Result:=i;
         exit;
      end if;   
    end loop;    
    return Result;             
  end OneHotBit2ChNumb;            
 -----------------------------------------------------------------------
 type Rtdex_TXC_fsm_t is
 	(
 	idle_c      ,
 	CheckDataAvail_c ,
 	SendFlagWord_c ,
 	SendAppWords0_c ,
 	SendAppWords1_c ,
 	SendAppWords2_c ,
 	SendAppWords3_c ,
 	SendAppWords4_c ,
 	WaitForTxdDone_c,
 	EvalNextCh_c	,
 	ChangeChReq_c	,
 	InterFrameGap_c
 	);
 
 signal Rtdex_TXC_fsm_s : Rtdex_TXC_fsm_t; 
 
 type RtdexTx_fsm_t is
   (   
   idle_c,         
   destHMacAdr_c,
   destLSrcHMacAdr_c,
   srcLMacAdr_c,
   SendSnapId_c,
   SendOrgId_c,
   SendUserId_c,
   SendFrameNumber_c,
   SendFrameSize_c,
   SendRtdexData_c   
   );
 signal RtdexTx_fsm_s : RtdexTx_fsm_t;
 
 signal a8u32_XferSizeCnt_s    : array8_u32_t;
 signal a8v32_TransferSize_s   : array8_v32_t;
 signal a8v15_FrameSize_s      : array8_v15_t;
  
 signal TXD_OneFrmaeEna_s 		: std_logic;
 signal GoToNextChan_s 			: std_logic;
 
 signal v32_RtdexTxc_tdata_s : std_logic_vector(31 downto 0);
 signal v4_RtdexTxc_tkeep_s  	: std_logic_vector(3 downto 0);
 signal RtdexTxc_tvalid_s    : std_logic;  
 signal RtdexTxc_tlast_s     : std_logic; 
 
 signal RtdexTxd_tvalid_s    : std_logic;  
 signal RtdexTxd_tlast_s     : std_logic; 
 signal v4_RtdexTxd_tkeep_s  : std_logic_vector(3 downto 0);
 signal v32_RtdexTxd_tdata_s : std_logic_vector(31 downto 0);
 
 
  
 signal v32_DestMacH_s         : std_logic_vector(31 downto 0);
 signal v32_DestMacLSrcMacH_s  : std_logic_vector(31 downto 0);
 signal v32_SrcMacL_s          : std_logic_vector(31 downto 0);
 
 signal v8_Txdata_Rd_s   : std_logic_vector(7 downto 0);
 signal InterFrameGapCnt_s : unsigned(31 downto 0);
 signal u8_ChFramesBurstCnt_s : unsigned(7 downto 0);
 signal InterFrameGap_s : unsigned(31 downto 0);
 signal v8_ActiveChannel_s : std_logic_vector(7 downto 0);
 signal TxStartStop_s              : std_logic;
 signal CurrentCh_s            : integer range 0 to 7;
 signal v8_CurntChXferDone_s   : std_logic_vector(7 downto 0);
 signal v8_CurntChFrameDone_s  : std_logic_vector(7 downto 0);
 signal v8_LoadTxSentFrmCnt_s : std_logic_vector(7 downto 0);
 signal v8_StartNewTransferDly1_s : std_logic_vector(7 downto 0);
 signal v16_FrameCnt_s         : unsigned(15 downto 0); 
 signal a8v32_RtdexChData_s       : array8_v32_t;
 

 
 begin
  
 a8v15_FrameSize_s		<= ia8v15_FrameSize_p;
 a8v32_TransferSize_s	<= ia8v32_TransferSize_p;
 oa8u32_XferSizeCnt_p	<= a8u32_XferSizeCnt_s;
 --
 ov32_RtdexTxc_tdata_p	<= v32_RtdexTxc_tdata_s;
 o_RtdexTxc_tvalid_p	<= RtdexTxc_tvalid_s;
 ov4_RtdexTxc_tkeep_p	<= v4_RtdexTxc_tkeep_s;
 o_RtdexTxc_tlast_p		<= RtdexTxc_tlast_s;
 
 o_RtdexTxd_tvalid_p     <= RtdexTxd_tvalid_s;     
 o_RtdexTxd_tlast_p      <= RtdexTxd_tlast_s;     
 ov4_RtdexTxd_tkeep_p   <= v4_RtdexTxd_tkeep_s; 
 ov32_RtdexTxd_tdata_p  <= v32_RtdexTxd_tdata_s;
 
 ov8_Txdata_Rd_p		<= v8_Txdata_Rd_s;
 a8v32_RtdexChData_s	<= ia8v32_RtdexChData_p;
 o_GoToNextChan_p		<= GoToNextChan_s;
 o_CurrentCh_p			<= CurrentCh_s;
 ov8_CurntChXferDone_p	<= v8_CurntChXferDone_s;
 ov8_LoadTxSentFrmCnt_p	<= v8_LoadTxSentFrmCnt_s;
 v8_StartNewTransferDly1_s <= iv8_StartNewTransfer_p;
 
 v32_DestMacH_s <= iv32_RemoteEndMacAddrH_p;
 v32_DestMacLSrcMacH_s <= iv16_RemoteEndMacAddrL_p & iv32_LocalEndMacAddrH_p(31 downto 16);
 v32_SrcMacL_s <= iv32_LocalEndMacAddrH_p(15 downto 0) & iv16_LocalEndMacAddrL_p;
 
 InterFrameGap_s <= unsigned(iv32_FrameGap_p);
 
 
 -----------------------------------------------------------------------
  -- Detect any active channel Tx request from the arbiter: 
  -- v8_ActiveChannel_s /= 0.
 -----------------------------------------------------------------------
 TxStartStop_s <= or_reduce(iv8_ActiveChannel_p);
   
 -----------------------------------------------------------------------
 -- RTDEx TXC FSM: Tx Control FSM
 -----------------------------------------------------------------------
 process(RTDEX_STR_TXC_ACLK)
 begin
   if rising_edge(RTDEX_STR_TXC_ACLK) then
      if i_TxcRstn_p = '0' then  
      	  RtdexTxc_tvalid_s <= '0';
          RtdexTxc_tlast_s <= '0';          
          v4_RtdexTxc_tkeep_s <= (others=>'0');          
          InterFrameGapCnt_s <= (others=>'0');
          u8_ChFramesBurstCnt_s <= (others=>'0');          
          CurrentCh_s <= 0;
          a8u32_XferSizeCnt_s <= (others=>(others=>'0'));          
          TXD_OneFrmaeEna_s <= '0';
		  v32_RtdexTxc_tdata_s <= (others=>'0'); 
      else
      
        -- Default values
        v4_RtdexTxc_tkeep_s <= x"F";        
        GoToNextChan_s <= '0';
        v8_CurntChXferDone_s <= (others=>'0');
        v8_LoadTxSentFrmCnt_s <= (others=>'0');
              	
      	  case Rtdex_TXC_fsm_s is
      	  
      	  	  when idle_c =>
      	  	  	 -- Wait for request from the host
                 if TxStartStop_s = '1' then 
                 	CurrentCh_s <= OneHotBit2ChNumb(iv8_ActiveChannel_p, 8);                  
                 	Rtdex_TXC_fsm_s <= CheckDataAvail_c;                 
                 end if;
              
              when CheckDataAvail_c =>
              	 if iv8_gotEnoughDataToSend_p(CurrentCh_s) = '1' then
              	 	-- Send Flag word (constant fixed by Xilinx) --
                   	v32_RtdexTxc_tdata_s <= v32_TxcFlagWord_c;     
                   	RtdexTxc_tvalid_s <= '1';
              	 	Rtdex_TXC_fsm_s <= SendFlagWord_c;
              	 else
              	 	-- Go to next channel in line
                 	GoToNextChan_s <= '1';
                 	Rtdex_TXC_fsm_s <= idle_c;
              	 end if;	
                  
              when SendFlagWord_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                   	v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 0                   	
                   	Rtdex_TXC_fsm_s <= SendAppWords0_c;
               	 end if;
               	
              when SendAppWords0_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 1                                                                                
                    Rtdex_TXC_fsm_s <= SendAppWords1_c;                      
                 end if;
                 
              when SendAppWords1_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 2                                                                                
                    Rtdex_TXC_fsm_s <= SendAppWords2_c;                      
                 end if;   
               
              when SendAppWords2_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 3                                                                                
                    Rtdex_TXC_fsm_s <= SendAppWords3_c;                      
                 end if;
                 
              when SendAppWords3_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c; -- app Word 3                                                                                
                    RtdexTxc_tlast_s <= '1';                    
                    TXD_OneFrmaeEna_s <= '1';	
                    Rtdex_TXC_fsm_s <= SendAppWords4_c;                         
                 end if;        
                 
              when SendAppWords4_c =>
                 if RTDEX_STR_TXC_TREADY = '1' then
                    v32_RtdexTxc_tdata_s <= v32_TxEmptyAppWord_c;  -- app Word 4                       
                    -- Turn of TXC channel
                 	RtdexTxc_tlast_s    <= '0';
                 	TXD_OneFrmaeEna_s <= '0';
                 	RtdexTxc_tvalid_s   <= '0';	
                    Rtdex_TXC_fsm_s <= WaitForTxdDone_c;                      
                 end if;
                 
                    
                 
              when WaitForTxdDone_c =>              	 
                 if v8_CurntChFrameDone_s(CurrentCh_s) = '1'then
                 	a8u32_XferSizeCnt_s(CurrentCh_s) <= a8u32_XferSizeCnt_s(CurrentCh_s) + 1;
                 	v8_LoadTxSentFrmCnt_s(CurrentCh_s) <= '1';
                 	u8_ChFramesBurstCnt_s <= u8_ChFramesBurstCnt_s +1;
                 	Rtdex_TXC_fsm_s <= EvalNextCh_c;
                 end if;
                 
              when EvalNextCh_c =>
                 -- What we do here depends on the xfer mode: Single or Continious.              
                 case i_Mode_p is
                 
                 	when SingleMode_c =>
                 		-- Transfer size is reached, we're done here.
              	    	if a8u32_XferSizeCnt_s(CurrentCh_s) >= unsigned(a8v32_TransferSize_s(CurrentCh_s)) then 
              	    		a8u32_XferSizeCnt_s(CurrentCh_s) <= (others=>'0');
              	 			u8_ChFramesBurstCnt_s <= (others=>'0');
              	 			v8_CurntChXferDone_s(CurrentCh_s)  <= '1';	              	 	
              	 			Rtdex_TXC_fsm_s <= ChangeChReq_c;
              	 		-- Otherwise, is number of burst frames reached for the current channel, let somebody else use the link too. 
              	 		elsif u8_ChFramesBurstCnt_s >= unsigned(iv8_TxChFrsBurst_p) then
              	 			u8_ChFramesBurstCnt_s <= (others=>'0');
              	 			Rtdex_TXC_fsm_s <= ChangeChReq_c;
              	 		else
              	 			Rtdex_TXC_fsm_s <= InterFrameGap_c;
              	 		end if;
              	 		
              	 	when ContinuousMode_c =>
              	 		-- Channel stoped by the user OR number of burst frames per CH is reached for the current CH, let somebody else use the link too.
              	 		if v8_StartNewTransferDly1_s(CurrentCh_s) = '0' or u8_ChFramesBurstCnt_s >= unsigned(iv8_TxChFrsBurst_p) then
              	 			u8_ChFramesBurstCnt_s <= (others=>'0');              	 			
              	 			Rtdex_TXC_fsm_s <= ChangeChReq_c;
              	 		else
              	 			Rtdex_TXC_fsm_s <= InterFrameGap_c;
              	 		end if;
              	 		
              	 	when others => null; -- Impossible to fall here, one bit case.
              	 end case;	
                 
              when ChangeChReq_c =>
              	 GoToNextChan_s <= '1';
              	 Rtdex_TXC_fsm_s <= InterFrameGap_c;
                 
              when InterFrameGap_c =>                                            	
               	 InterFrameGapCnt_s <= InterFrameGapCnt_s + 1;
                 if (InterFrameGapCnt_s >= InterFrameGap_s) then
                    InterFrameGapCnt_s <= (others=>'0');
                    Rtdex_TXC_fsm_s <= idle_c;
                 end if;   

              when others => Rtdex_TXC_fsm_s <= Idle_c; 
                           
           end case;   
              
              
      end if;
   end if;
 end process;
    	
 -----------------------------------------------------------------------
 -- RTDEx TXD FSM: TX Data FSM
 -----------------------------------------------------------------------
 process(RTDEX_STR_TXD_ACLK)
 begin
   if rising_edge(RTDEX_STR_TXD_ACLK) then
      if i_SysRstn_p = '0' then  
         RtdexTx_fsm_s <= idle_c;         
         v4_RtdexTxd_tkeep_s <= (others=>'0');
         RtdexTxd_tvalid_s   <= '0';         
         v16_FrameCnt_s <=  (others=>'0');         
         v8_Txdata_Rd_s <= (others=>'0');
                           
      else
      
         -- Default value --         
         v4_RtdexTxd_tkeep_s <= x"F";         
         v8_CurntChFrameDone_s <= (others=>'0');
         RtdexTxd_tlast_s    <= '0';
         
         
         case RtdexTx_fsm_s is
         
            when idle_c =>
               RtdexTxd_tlast_s    <= '0';
               RtdexTxd_tvalid_s   <= '0';               
               v16_FrameCnt_s <=  (others=>'0');
                              
               -- Wait for enable from TXC FSM
               if TXD_OneFrmaeEna_s = '1' then
               	  RtdexTx_fsm_s <= destHMacAdr_c;               
                end if;
             
            when destHMacAdr_c =>               
               if RTDEX_STR_TXD_TREADY = '1' then
                  v32_RtdexTxd_tdata_s <= v32_DestMacH_s;                  
                  RtdexTxd_tvalid_s <= '1';
                  RtdexTx_fsm_s <= destLSrcHMacAdr_c;  
               end if;
            
            when destLSrcHMacAdr_c =>
               if RTDEX_STR_TXD_TREADY = '1' then
                  v32_RtdexTxd_tdata_s <= v32_DestMacLSrcMacH_s;                  
                  RtdexTx_fsm_s <= srcLMacAdr_c;                                    
               end if;   
               
            when srcLMacAdr_c =>
               if RTDEX_STR_TXD_TREADY = '1' then
                  v32_RtdexTxd_tdata_s <= v32_SrcMacL_s;                  
                  RtdexTx_fsm_s <= SendSnapId_c;                               
               end if;   
            
            when SendSnapId_c =>
               if RTDEX_STR_TXD_TREADY = '1' then
                  v32_RtdexTxd_tdata_s <= (iv16_EtherType_p & v16_SnapId_c);                  
                  RtdexTx_fsm_s <= SendOrgId_c;
               end if;   
            
            
            when SendOrgId_c =>               
               if RTDEX_STR_TXD_TREADY = '1' then
                  v32_RtdexTxd_tdata_s <= (v4_Ctrl_c & v24_OrgId_c);                  
                  RtdexTx_fsm_s <= SendUserId_c;
               end if;   
               
            when SendUserId_c =>               
               if RTDEX_STR_TXD_TREADY = '1' then
                  v32_RtdexTxd_tdata_s <= (v16_UserId_c & v8_ProtocolId_c & std_logic_vector(to_unsigned(CurrentCh_s,8)));                  
                  RtdexTx_fsm_s <= SendFrameNumber_c;
               end if;   
               
           when SendFrameNumber_c =>               
               if RTDEX_STR_TXD_TREADY = '1' then
                  v32_RtdexTxd_tdata_s <= std_logic_vector(a8u32_XferSizeCnt_s(CurrentCh_s));                                    
                  RtdexTx_fsm_s <= SendFrameSize_c;
               end if;       
            
            when SendFrameSize_c =>               
               if RTDEX_STR_TXD_TREADY = '1' then
                  v32_RtdexTxd_tdata_s <= x"0000" & '0' & a8v15_FrameSize_s(CurrentCh_s);
                  -- Frame size is in byte & we send 4 bytes at a time, so divide by 4
                  v16_FrameCnt_s      <= "000" & unsigned(a8v15_FrameSize_s(CurrentCh_s)(14 downto 2)); -- Load the counter
                  v8_Txdata_Rd_s(CurrentCh_s) <= '1';
                  RtdexTx_fsm_s <= SendRtdexData_c;
               end if;       
               
           when SendRtdexData_c =>                                             
               if RTDEX_STR_TXD_TREADY = '1' then                  
                  v32_RtdexTxd_tdata_s(31 downto 24) <= a8v32_RtdexChData_s(CurrentCh_s)(7 downto 0);   
                  v32_RtdexTxd_tdata_s(23 downto 16) <= a8v32_RtdexChData_s(CurrentCh_s)(15 downto 8);   
                  v32_RtdexTxd_tdata_s(15 downto 8)  <= a8v32_RtdexChData_s(CurrentCh_s)(23 downto 16);   
                  v32_RtdexTxd_tdata_s(7 downto 0)   <= a8v32_RtdexChData_s(CurrentCh_s)(31 downto 24);   
                  v16_FrameCnt_s <= v16_FrameCnt_s - 1; -- Data counter (data in courrent frame)
                  
                  -- Wait for the Frame count to be reached then go idle
                  if (v16_FrameCnt_s = FrameCntExpired_c ) then 
                     v8_Txdata_Rd_s(CurrentCh_s) <= '0';  
                     RtdexTxd_tlast_s <= '1';                     
                     v8_CurntChFrameDone_s(CurrentCh_s) <= '1';                     
                   	 RtdexTx_fsm_s <= idle_c;                   	                 	 
                  end if;   
               end if;
            
            when others =>
               RtdexTx_fsm_s <= idle_c;
               
            end case;            
      end if;
   end if;
 end process;
 
 
 end rtdex_TxCore_rtl;