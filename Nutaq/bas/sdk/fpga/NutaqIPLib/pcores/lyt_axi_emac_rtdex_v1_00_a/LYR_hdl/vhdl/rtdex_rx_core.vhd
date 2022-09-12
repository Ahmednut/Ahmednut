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

entity rtdex_rx_core is
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
		
end rtdex_rx_core;

architecture RtdexRxCore_rtl of rtdex_rx_core is

 -------------------------------------------------------------------------
 -- Function that transformes channel number integer input to its 
 -- corespending std_logic_vector: its a simple 2 pow of ChanNumber:
 -- Ch = 0 ==> return x"01" 
 -- Ch = 1 ==> return x"02" 
 -- ...
 -- Ch = 7 ==> return x"80" 
 -------------------------------------------------------------------------
  function ChNumb2OneHotBit(ChNumb : integer; InVectWidth : integer) 
  			return std_logic_vector
  is      
  begin    
    return std_logic_vector(to_unsigned(2**ChNumb, InVectWidth));
  end ChNumb2OneHotBit;            
 -----------------------------------------------------------------------

 type RtdexRx_fsm_t is
   (   
   destMacAdr_c,
   destSrcMacAdr_c,
   srcMacAdr_c,
   CheckSnapId_c,
   CheckOrgId_c,
   CheckUserId_c,
   GetFrameNumber_c,
   GetFrameSize_c,
   GetRtdexData_c,
   ChTimeoout_c,
   FlushRtdexData_c
   );
 signal RtdexRx_fsm_s : RtdexRx_fsm_t;
 
 type PauseReqFsm_t is
 	(
 	SinglePause_c,
 	MultiplePause_c
 	);
 signal PauseReqFsm_s : PauseReqFsm_t;
 
 signal ChWatchDog_s 			: std_logic;
 signal v8_ChJammed_s 			: std_logic_vector(7 downto 0);
 signal v32_ChWatchDogTimer_s  : unsigned(31 downto 0);
 signal Rxd_readyForHeader_s   : std_logic;
 signal AcceptData_s           : std_logic;
 signal FlushData_s            : std_logic;
 signal ChannelId_s            : integer range 0 to 7;
 signal v8_activeCh_s          : std_logic_vector(7 downto 0);
 signal a8u32_FrameNumber_s	   : array8_u32_t;
 signal v8_ProtocolId_s        : std_logic_vector(7 downto 0);
 signal v16_SubId_s            : std_logic_vector(15 downto 0);
 signal v14_FrameSize_s        : unsigned(13 downto 0); -- In terme of 4 bytes at a time.
 signal RtdexBadFrame_s        : std_logic;
 signal v8_RxFrameCompleted_s  : std_logic_vector(7 downto 0);
 signal v8_RtdexDropedFrame_s  : std_logic_vector(7 downto 0);
 signal v8_RtdexRxEna_s : std_logic_vector(7 downto 0);
  
 signal SendPause_s			: std_logic; 		 
  
 --attribute keep : string;                         
 --attribute keep of ProgFull_s : signal is "true";
  
begin

 -----------------------------------------------------------------------
 -- Capture RX Start/enable cmd with local clock
 -----------------------------------------------------------------------
 process(RTDEX_RXD_ACLK)
 begin
   if rising_edge(RTDEX_RXD_ACLK) then
   	  v8_RtdexRxEna_s <= iv8_RxStartNewTransfer_p;
   end if;
 end process;
  
 -----------------------------------------------------------------------
 -- Figue-out when to send pause request to the transmitting end
 -----------------------------------------------------------------------
 process(RTDEX_RXD_ACLK)
 begin
   if rising_edge(RTDEX_RXD_ACLK) then
		if i_sysRst_p = '0' then  
			SendPause_s <= '0';						
			ov16_PauseVal_p <= (others=>'0');
			PauseReqFsm_s <= SinglePause_c;
		else
			-- Default 
			SendPause_s <= '0';
			
			case PauseReqFsm_s is
			
				when SinglePause_c =>
					if or_reduce(iv8_ProgFull_p) = '1' then -- prog full asserted
						SendPause_s <= '1';
						ov16_PauseVal_p <= v16_PauseMaxVal_c; -- send a pause						
						PauseReqFsm_s <= MultiplePause_c;
					end if;	
					
				when MultiplePause_c =>	 
					if or_reduce(iv8_ProgFull_p) = '0' then -- prog full negated
						SendPause_s <= '1';
						ov16_PauseVal_p <= v16_PauseCancelVal_c; -- send cancel a pause
						PauseReqFsm_s <= SinglePause_c;
					end if;
					
					if RtdexRx_fsm_s = GetFrameNumber_c then			
				    	if iv8_ProgFull_p(ChannelId_s) = '1' then
							SendPause_s <= '1';
							ov16_PauseVal_p <= v16_PauseMaxVal_c;								
						end if;						
					end if;
				
					
				when others => null;
			end case;										
			
		end if;
	end if;
 end process;		 

 o_SendPause_p <= SendPause_s; 
  
 -----------------------------------------------------------------------
 -- Header decoding and stripping FSM
 -- The RTDEx header is'nt verified here. It's already done before in
 -- the rtdex_mux_rx module. Any non RTDEx frames are forwarded to uBlaze
 -----------------------------------------------------------------------  
 process(RTDEX_RXD_ACLK)
 begin
   if rising_edge(RTDEX_RXD_ACLK) then
		if i_sysRst_p = '0' then  
		   Rxd_readyForHeader_s <= '0'; 
		   ChannelId_s <= 0;
		   v8_ProtocolId_s <= (others=>'0');
		   v16_SubId_s <= (others=>'0');
		   a8u32_FrameNumber_s <= (others=>(others=>'0'));      
		   v14_FrameSize_s <= (others=>'0');		   
		   AcceptData_s <= '0';
		   FlushData_s  <= '0';
		   ChWatchDog_s <= '0';		   
		   v8_ChJammed_s <= (others=>'0');		    
		   RtdexRx_fsm_s <= destMacAdr_c;			
		else  
		   
		   -- Default value, creates a pulse signal
		   RtdexBadFrame_s <= '0';
		   v8_RxFrameCompleted_s <= (others=>'0');
		   v8_RtdexDropedFrame_s <= (others=>'0');
		   
			case RtdexRx_fsm_s is
						   
				when destMacAdr_c =>
				
				   Rxd_readyForHeader_s <= '1';
					if RTDEX_RXD_TVALID = '1' then
						RtdexRx_fsm_s <= destSrcMacAdr_c;					
					end if;				                
			   
				when destSrcMacAdr_c =>
				
					if RTDEX_RXD_TVALID = '1' then
						RtdexRx_fsm_s <= srcMacAdr_c;
					else
					   RtdexBadFrame_s <= '1';
					   RtdexRx_fsm_s <= destMacAdr_c;	
					end if;				            
											  
				when srcMacAdr_c =>
				
				   if RTDEX_RXD_TVALID = '1' then
					   RtdexRx_fsm_s <= CheckSnapId_c;
					else
					   RtdexBadFrame_s <= '1';
					   RtdexRx_fsm_s <= destMacAdr_c;                 
				   end if;	
			
				when CheckSnapId_c =>
				
					if RTDEX_RXD_TVALID = '1' then						
					   RtdexRx_fsm_s <= CheckOrgId_c;
					else
					   RtdexBadFrame_s <= '1';
					   RtdexRx_fsm_s <= destMacAdr_c;   						
					end if;
													   
				when CheckOrgId_c =>
				
					if RTDEX_RXD_TVALID = '1' then					
						RtdexRx_fsm_s <= CheckUserId_c;
					else
					   RtdexBadFrame_s <= '1';
					   RtdexRx_fsm_s <= destMacAdr_c;						
					end if;
	 
				when CheckUserId_c =>
				
					if RTDEX_RXD_TVALID = '1' then
					   ChannelId_s	            	<= to_integer(unsigned(V32_RTDEX_RXD_TDATA(31 downto 24)));
					   v8_ProtocolId_s          	<= V32_RTDEX_RXD_TDATA(23 downto 16); -- Unused for now!!!
		            	v16_SubId_s(7 downto 0)     <= V32_RTDEX_RXD_TDATA(15 downto 8);
		            	v16_SubId_s(15 downto 8)	<= V32_RTDEX_RXD_TDATA(7 downto 0);
						RtdexRx_fsm_s 			<= GetFrameNumber_c;
					else
					   RtdexBadFrame_s <= '1';
					   RtdexRx_fsm_s <= destMacAdr_c;							
					end if;
					
			   when GetFrameNumber_c =>
			   
					if RTDEX_RXD_TVALID = '1' then
					   a8u32_FrameNumber_s(ChannelId_s)(7 downto 0)	  <= unsigned(V32_RTDEX_RXD_TDATA(31 downto 24));
					   a8u32_FrameNumber_s(ChannelId_s)(15 downto 8)  <= unsigned(V32_RTDEX_RXD_TDATA(23 downto 16));
					   a8u32_FrameNumber_s(ChannelId_s)(23 downto 16) <= unsigned(V32_RTDEX_RXD_TDATA(15 downto 8));
					   a8u32_FrameNumber_s(ChannelId_s)(31 downto 24) <= unsigned(V32_RTDEX_RXD_TDATA(7 downto 0));
					   
					   -- Check if current channel is enabled and the User Sub-ID matche
					   if (v8_RtdexRxEna_s(ChannelId_s) = '1' and v16_SubId_s = v16_UserId_c) then 					   		
					   	  RtdexRx_fsm_s <= GetFrameSize_c;						   		
					   else -- Channel not enabled or unvalide User sub-ID	  
					      FlushData_s  <= '1'; -- flush pipes
					      Rxd_readyForHeader_s <= '0'; 
					      RtdexBadFrame_s <= '1';
					      RtdexRx_fsm_s <= FlushRtdexData_c;
					   end if;   
						
					else
					   RtdexBadFrame_s <= '1';
					   RtdexRx_fsm_s <= destMacAdr_c;							
					end if;
					
				when GetFrameSize_c =>
				
					if RTDEX_RXD_TVALID = '1' then
						Rxd_readyForHeader_s <= '0';	
						
						--Frame size is in bytes and we process 4 bytes at a time, so /4 or 2 bits left shift 
						v14_FrameSize_s(5 downto 0)  <= unsigned(V32_RTDEX_RXD_TDATA(31 downto 26));				   
					    v14_FrameSize_s(13 downto 6) <= unsigned(V32_RTDEX_RXD_TDATA(23 downto 16));
					   
						-- Do we have enough space in channel's fifo?.
						if ia8u24_ChFifoFreeSpace_p(ChannelId_s) >= ("0000000000" & unsigned(V32_RTDEX_RXD_TDATA(23 downto 16)) & unsigned(V32_RTDEX_RXD_TDATA(31 downto 26))) then
							ChWatchDog_s <= '0'; -- Reset the watchdog timer
							v8_ChJammed_s(ChannelId_s) <= '0'; -- Clear was jammed tag
					   		AcceptData_s <= '1';
							RtdexRx_fsm_s <= GetRtdexData_c;
						else
							ChWatchDog_s <= '1';
							RtdexRx_fsm_s <= ChTimeoout_c;
						end if;	
					else
					   RtdexBadFrame_s <= '1';
					   RtdexRx_fsm_s <= destMacAdr_c;							
					end if;	
					
				when GetRtdexData_c =>					
					if RTDEX_RXD_TVALID = '1' and RTDEX_RXD_TLAST = '1' then					   					   
					   	AcceptData_s <= '0';
					   	v8_RxFrameCompleted_s(ChannelId_s) <= '1';
						RtdexRx_fsm_s <= destMacAdr_c;						
					elsif RTDEX_RXD_TVALID = '0' then					   
					   AcceptData_s <= '0';							   
					   RtdexBadFrame_s <= '1';
					   RtdexRx_fsm_s <= destMacAdr_c;					
					end if;
																
				when ChTimeoout_c =>	
					-- If watchdog timer expires or channel was previously jammed, drop the frame so ethernet link does not jamme
			   		if v32_ChWatchDogTimer_s >= unsigned(iv32_RxTimeout2DropFrm_p) or v8_ChJammed_s(ChannelId_s) = '1' then
			   			ChWatchDog_s <= '0'; -- Reset the watchdog timer			   			
			   			FlushData_s  <= '1'; -- flush pipes			      		
			      		v8_RtdexDropedFrame_s(ChannelId_s) <= '1';
			      		v8_ChJammed_s(ChannelId_s) <= '1'; -- Tag it for next time to know 
			      		RtdexRx_fsm_s <= FlushRtdexData_c;
			      	elsif ia8u24_ChFifoFreeSpace_p(ChannelId_s) >= ("0000000000" & v14_FrameSize_s) then -- Suddenly, there is a space for current frame data, w'll take it
			      		ChWatchDog_s <= '0'; -- Reset the watchdog timer
			      		AcceptData_s <= '1'; 						   			
			      		RtdexRx_fsm_s <= GetRtdexData_c;
			   		end if;
				
				when FlushRtdexData_c =>	
					if RTDEX_RXD_TVALID = '1' and RTDEX_RXD_TLAST = '1' then					   					   						
						FlushData_s  <= '0';					   							
						RtdexRx_fsm_s <= destMacAdr_c;						
					elsif RTDEX_RXD_TVALID = '0' then					   					   
					   FlushData_s  <= '0';					   
					   RtdexRx_fsm_s <= destMacAdr_c;					
					end if;
												
				when others =>
					RtdexRx_fsm_s <= destMacAdr_c;
				   
			end case;   
		end if;      
   end if;
 end process;
 
 
 -----------------------------------------------------------------------
 --- Watch dog timer
 -----------------------------------------------------------------------
 process(RTDEX_RXD_ACLK)
 begin
   if rising_edge(RTDEX_RXD_ACLK) then
		if i_sysRst_p = '0' then
		   v32_ChWatchDogTimer_s <= (others=>'0');
		else		
		   case ChWatchDog_s is
		   
		   		when '1' =>
		   			if v32_ChWatchDogTimer_s >= unsigned(iv32_RxTimeout2DropFrm_p) then
		   	  			-- Count is reached, freez here.
		      			v32_ChWatchDogTimer_s <= v32_ChWatchDogTimer_s;	   
		   			else
		      			v32_ChWatchDogTimer_s <= v32_ChWatchDogTimer_s + 1;		   	   
		      		end if;	
		      		
		   		when '0' =>
		   			v32_ChWatchDogTimer_s <=  (others=>'0');   
		   		
		   			when others =>
		   				v32_ChWatchDogTimer_s <=  (others=>'0');   
		   				
		   		end case;		
		   		   
		end if;
	end if;
 end process;
 
 --- Outputs ---
 -- Ready for Data when Chan FIFO not full, Parsing frame's Header or Flushing current Frame:
 RTDEX_RXD_TREADY <= (iv8_ChFifoNotFull_p(ChannelId_s) and AcceptData_s) or FlushData_s or Rxd_readyForHeader_s;
 
 -- Wr_en when RXD Valid, chan_Fifo not full and current frame's data is accepted:
 v8_activeCh_s <= ChNumb2OneHotBit(ChannelId_s, 8);
 
 Wren_gen: for i in 0 to MaxChannelNb_c-1 generate
 	ov8_ChWr_en_p(i)  <= RTDEX_RXD_TVALID and iv8_ChFifoNotFull_p(i) and AcceptData_s and v8_activeCh_s(i);
 end generate Wren_gen;
 
   
 oa8u32_FrameNumber_p <= a8u32_FrameNumber_s;
 o_RtdexBadFrame_p	<= RtdexBadFrame_s;
 ov8_RxFrameCompleted_p	<= v8_RxFrameCompleted_s;
 ov8_RtdexDropedFrame_p	<= v8_RtdexDropedFrame_s;
  

end RtdexRxCore_rtl;
				