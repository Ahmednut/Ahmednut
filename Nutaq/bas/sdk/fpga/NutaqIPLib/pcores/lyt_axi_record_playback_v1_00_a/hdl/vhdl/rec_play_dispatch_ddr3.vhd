

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use work.recplay_type_p.all;

library lyt_axi_record_playback_v1_00_a;
  use lyt_axi_record_playback_v1_00_a.recplay_fifos_p.all;

-------------------------------------------------------------------------------
-- Entity Section
-------------------------------------------------------------------------------
entity rec_play_dispatch_ddr3 is
  generic
  (
    ADDR_WIDTH  : integer := 30
  );
  port
  (
    -- Reset And Clocks
    i_Reset_p             : in std_logic;
    i_SystemClk_p         : in std_logic;

    -- Ctrl/Status for the host Registers
    i_SetMode_p           : in std_logic;
    iv3_Mode_p            : in std_logic_vector(2 downto 0);
    iv_StartAddress_p     : in std_logic_vector(ADDR_WIDTH-1 downto 0);
    iv_TrigDly_p          : in std_logic_vector(ADDR_WIDTH downto 0);
    iv_TransferSize_p     : in std_logic_vector(ADDR_WIDTH-1 downto 0);
    ov_TrigAddr_p         : out std_logic_vector(ADDR_WIDTH-1 downto 0);
    ov32_TrigAddrIndex_p  : out std_logic_vector(31 downto 0);
    o_TransferOver_p      : out std_logic;
    o_ParityAddrReg_p     : out std_logic;
    o_RecordStarted_p     : out std_logic;
    ov32_RecordStorageCnt_p   : out std_logic_vector(31 downto 0);
    ov32_RtdexStorageCnt_p    : out std_logic_vector(31 downto 0);
    ov32_PlaybackReadCnt_p    : out std_logic_vector(31 downto 0);
    ov32_RtdexReadCnt_p       : out std_logic_vector(31 downto 0);

    -- Record Fifo Interface
    o_RecordResetFifo_p   : out std_logic;
    iv32_RecTriggerIn_p   : in std_logic_vector(31 downto 0);
    iv256_RecData_p       : in std_logic_vector(255 downto 0);
    i_RecDValid_p         : in std_logic;
    o_RecReadData_p       : out std_logic;
    i_RecFifoEmpty_p      : in std_logic;

    -- RTDEx HOST 2 FPGA Fifo Interface
    o_RtdexRxResetFifo_p  : out std_logic;
    o_RtdexRxStarted_p    : out std_logic;
    iv256_RtdexRxData_p   : in std_logic_vector(255 downto 0);
    i_RtdexDValid_p	      : in std_logic;
    o_RtdexRxReadData_p   : out std_logic;
    i_RtdexFifoEmpty_p    : in std_logic;

    -- Play back Fifo Interface
    i_PlayTriggerIn_p       : in std_logic;
    ov256_PlayFifoWrData_p  : out std_logic_vector(255 downto 0);
    o_PlayFifoWrite_p       : out std_logic;
    o_PlayResetFifo_p       : out std_logic;
    i_PlayFifoProgFull_p    : in std_logic;

    -- RTDEx FPGA 2 HOST Fifo Interface
    ov256_RtdexTxFifoWrData_p : out std_logic_vector(255 downto 0);
    o_RtdexTxFifoWrite_p      : out std_logic;
    o_RtdexTxResetFifo_p      : out std_logic;
    i_RtdexTxFifoProgFull_p   : in std_logic;

    -- Memory Interface
    i_MemClk_p          : in std_logic;
    i_PhyInitDone_p     : in std_logic;

    i_AppRdy_p          : in std_logic;
    o_AppEn_p           : out std_logic;
    ov3_AppCmd_p        : out std_logic_vector(2 downto 0);
    ov_TgAddr_p         : out std_logic_vector(ADDR_WIDTH-1 downto 0);

    iv256_AppRdData_p   : in std_logic_vector(255 downto 0);
    i_AppRdDataValid_p  : in std_logic;

    i_AppWdfRdy_p       : in std_logic;
    o_AppWdfWren_p      : out std_logic;
    ov256_AppWdfData_p  : out std_logic_vector(255 downto 0);
    o_AppWdfEnd_p       : out std_logic
  );
end entity rec_play_dispatch_ddr3;

 ------------------------------------------------------------------------------
 -- Architecture section
 ------------------------------------------------------------------------------

architecture rtl of rec_play_dispatch_ddr3 is

  signal SetModeD1_s : std_logic;
  signal SetModeD2_s : std_logic;
  signal SetModeRise_s : std_logic;

  signal ParityAddrReg_s : std_logic;
  signal ParityAddrRegD1_s : std_logic;

  signal ResetD1_s : std_logic;

  signal SetModeRec_s : std_logic;
  signal SetModePlayContinuous_s : std_logic;
  signal SetModePlay_s : std_logic;
  signal SetModeRTDExHost2Fpga_s : std_logic;
  signal SetModeRTDExFpga2Host_s : std_logic;

  signal v_TrigAddr_s     : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_TrigAddrD1_s     : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v32_TrigAddrIndex_s : std_logic_vector(31 downto 0);
  signal v32_TrigAddrIndexD1_s : std_logic_vector(31 downto 0);
  signal TransferOver_s     : std_logic;
  signal TransferOverD1_s : std_logic;

  signal TransferOverRecord_s   : std_logic;
  signal TransferOverPlayback_s : std_logic;
  signal TransferOverRtdexRx_s  : std_logic;
  signal TransferOverRtdexTx_s  : std_logic;

  signal BufferProgFull_s       : std_logic;
  signal RtdexBufferProgFull_s  : std_logic;
  signal RecBufferProgFull_s    : std_logic;
  signal RtdexRxReadData_s      : std_logic;
  signal RecReadData_s          : std_logic;

  signal delayRecordEnabled_R1_s : std_logic;
  signal delayRecordEnabled_R2_s : std_logic;
  signal delayRecordEnabled_R3_s : std_logic;
  signal delayRecordEnabled_R4_s : std_logic;
  signal delayRecordEnabled_R5_s : std_logic;
  signal delayRecordEnabled_R6_s : std_logic;
  signal delayRecordEnabled_R7_s : std_logic;
  signal delayRecordEnabled_R8_s : std_logic;

  signal delayRtdexWrEnabled_R1_s : std_logic;
  signal delayRtdexWrEnabled_R2_s : std_logic;
  signal delayRtdexWrEnabled_R3_s : std_logic;
  signal delayRtdexWrEnabled_R4_s : std_logic;
  signal delayRtdexWrEnabled_R5_s : std_logic;
  signal delayRtdexWrEnabled_R6_s : std_logic;
  signal delayRtdexWrEnabled_R7_s : std_logic;
  signal delayRtdexWrEnabled_R8_s : std_logic;

  signal v256_PlayFifoWrData_s : std_logic_vector(255 downto 0);
  signal PlayFifoWrite_s       :  std_logic;

  signal v256_RtdexTxFifoWrData_s : std_logic_vector(255 downto 0);
  signal RtdexTxFifoWrite_s       : std_logic;

  signal RecordStarted_s          : std_logic;
  signal RtdexRxStarted_s         : std_logic;

  signal v_StartAddress_s         : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_TransferSize_s         : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_TrigDly_s	            : std_logic_vector(ADDR_WIDTH downto 0);
  signal v3_ModeD1_s		          : std_logic_vector(2 downto 0);
  
  signal v288_BufferDin_s         : std_logic_vector(287 downto 0);
  signal BufferWen_s              : std_logic;
  signal v288_BufferDin_D1_s      : std_logic_vector(287 downto 0);
  signal BufferWen_D1_s           : std_logic;
  
  signal RecordResetFifo_s          : std_logic;
  signal RtdexRxResetFifo_s         : std_logic;
  
  -- attribute keep_hierarchy : string;
  -- attribute keep_hierarchy of rtl : architecture is "true";
  
  attribute keep : string;
  
  attribute keep of v288_BufferDin_s : signal is "true";
  attribute keep of BufferWen_s : signal is "true";
  
  attribute keep of delayRecordEnabled_R1_s : signal is "true";
  attribute keep of delayRecordEnabled_R2_s : signal is "true";
  attribute keep of delayRecordEnabled_R3_s : signal is "true";
  attribute keep of delayRecordEnabled_R4_s : signal is "true";
  attribute keep of delayRecordEnabled_R5_s : signal is "true";
  attribute keep of delayRecordEnabled_R6_s : signal is "true";
  attribute keep of delayRecordEnabled_R7_s : signal is "true";
  attribute keep of delayRtdexWrEnabled_R1_s : signal is "true";
  attribute keep of delayRtdexWrEnabled_R2_s : signal is "true";
  attribute keep of delayRtdexWrEnabled_R3_s : signal is "true";
  attribute keep of delayRtdexWrEnabled_R4_s : signal is "true";
  attribute keep of delayRtdexWrEnabled_R5_s : signal is "true";
  attribute keep of delayRtdexWrEnabled_R6_s : signal is "true";
  attribute keep of delayRtdexWrEnabled_R7_s : signal is "true";
  attribute keep of RtdexBufferProgFull_s   : signal is "true";
  attribute keep of RecBufferProgFull_s     : signal is "true";
  attribute keep of RecordResetFifo_s       : signal is "true";
  attribute keep of RtdexRxResetFifo_s      : signal is "true";

begin

  --------------------------------------------------------------------------
  -- Output port
  --------------------------------------------------------------------------
  -- Status signals for the host
  o_TransferOver_p      <= TransferOverD1_s;
  o_ParityAddrReg_p     <= ParityAddrRegD1_s;
  ov_TrigAddr_p         <= v_TrigAddrD1_s;
  ov32_TrigAddrIndex_p  <= v32_TrigAddrIndexD1_s;

  -- For Acquisition Unpacker
  ov256_PlayFifoWrData_p <= v256_PlayFifoWrData_s;
  o_PlayFifoWrite_p      <= PlayFifoWrite_s;

  -- For RTDEx
  ov256_RtdexTxFifoWrData_p <= v256_RtdexTxFifoWrData_s;
  o_RtdexTxFifoWrite_p      <= RtdexTxFifoWrite_s;

  o_RtdexRxStarted_p        <= RtdexRxStarted_s;
  o_RtdexRxReadData_p       <= RtdexRxReadData_s;

  -- For Aquisition Packer
  o_RecordStarted_p  <= RecordStarted_s;
  o_RecReadData_p  <= RecReadData_s;



  -----------------------------------------
  -- Delay enable signal to let the write FIFO clear its reset
  -----------------------------------------
  process(i_MemClk_p)
  begin
    if rising_edge(i_MemClk_p) then

      delayRecordEnabled_R1_s <= RecordStarted_s;
      delayRecordEnabled_R2_s <= delayRecordEnabled_R1_s;
      delayRecordEnabled_R3_s <= delayRecordEnabled_R2_s;
      delayRecordEnabled_R4_s <= delayRecordEnabled_R3_s;
      delayRecordEnabled_R5_s <= delayRecordEnabled_R4_s;
      delayRecordEnabled_R6_s <= delayRecordEnabled_R5_s;
      delayRecordEnabled_R7_s <= delayRecordEnabled_R6_s;
      delayRecordEnabled_R8_s <= delayRecordEnabled_R7_s;

      delayRtdexWrEnabled_R1_s <= RtdexRxStarted_s;
      delayRtdexWrEnabled_R2_s <= delayRtdexWrEnabled_R1_s;
      delayRtdexWrEnabled_R3_s <= delayRtdexWrEnabled_R2_s;
      delayRtdexWrEnabled_R4_s <= delayRtdexWrEnabled_R3_s;
      delayRtdexWrEnabled_R5_s <= delayRtdexWrEnabled_R4_s;
      delayRtdexWrEnabled_R6_s <= delayRtdexWrEnabled_R5_s;
      delayRtdexWrEnabled_R7_s <= delayRtdexWrEnabled_R6_s;
      delayRtdexWrEnabled_R8_s <= delayRtdexWrEnabled_R7_s;

      -- Latch BufferProgFull_s and ReadData to ease timing
      -- the prog full value will compensate for this extra clock cycle
      -- the Empty status of the FIFO is not used to ease timing
      -- Since the Valid flag will be monitor, we can read even if the FIFO is empty
      -- without causing any harm
      RtdexBufferProgFull_s <= BufferProgFull_s;
      RecBufferProgFull_s   <= BufferProgFull_s;
      
      RtdexRxReadData_s <= (not RtdexBufferProgFull_s) and delayRtdexWrEnabled_R8_s;
      RecReadData_s <= (not RecBufferProgFull_s) and delayRecordEnabled_R8_s;

    end if;
  end process;
  
  process(i_MemClk_p)
  begin
    if rising_edge(i_MemClk_p) then    
      if delayRecordEnabled_R4_s = '1' then
        v288_BufferDin_s  <= iv32_RecTriggerIn_p & iv256_RecData_p;
        BufferWen_s       <= i_RecDValid_p;
      elsif delayRtdexWrEnabled_R4_s = '1' then
        v288_BufferDin_s  <= X"00000001" & iv256_RtdexRxData_p;
        BufferWen_s       <= i_RtdexDValid_p;
      else
        v288_BufferDin_s  <= (others => '0');
        BufferWen_s       <= '0';
      end if;
      
      v288_BufferDin_D1_s <= v288_BufferDin_s;
      BufferWen_D1_s      <= BufferWen_s;
    end if;
  end process;

 --------------------------------------------------------------------------
 -- SetMode rising edge detect: This signal is used as start signal.
 --------------------------------------------------------------------------
 process(i_MemClk_p)
 begin
 	if rising_edge(i_MemClk_p) then
    	SetModeD1_s <= i_SetMode_p;
    	SetModeD2_s <= SetModeD1_s;
  end if;
 end process;
 -- Generate a single pulse signal on rising edge.
 SetModeRise_s <= SetModeD1_s and ( not SetModeD2_s );

 --------------------------------------------------------------------------
 -- Capture configuration parametre with the memory clock.
 --------------------------------------------------------------------------
 process(i_MemClk_p)
 begin
 	if rising_edge(i_MemClk_p) then
    	v_StartAddress_s  <= iv_StartAddress_p;
    	v_TrigDly_s		    <= iv_TrigDly_p;
    	v_TransferSize_s	<= iv_TransferSize_p;
    	ResetD1_s 			  <= i_Reset_p;
    	v3_ModeD1_s			  <= iv3_Mode_p;
  end if;
 end process;

 --------------------------------------------------------------------------
 -- FSM to handel different modes: Record, Playback, Host2Mem, Mem2Host,...
 -- It implements a 4 port mux to access the DDR3 memory: 2 WR ports & 2 RD
 -- ports. It also enables/disables the corresponding port.
 --------------------------------------------------------------------------
 ModeDecoding_l : process( i_MemClk_p )
 begin
   if( rising_edge( i_MemClk_p ) ) then

     if( ResetD1_s = '1' ) then
       SetModeRec_s            <= '0';
       SetModePlayContinuous_s <= '0';
       SetModePlay_s           <= '0';
       SetModeRTDExHost2Fpga_s <= '0';
       SetModeRTDExFpga2Host_s <= '0';
     else

       SetModeRec_s            <= '0';
       SetModePlay_s           <= '0';
       SetModeRTDExHost2Fpga_s <= '0';
       SetModeRTDExFpga2Host_s <= '0';

       if( SetModeRise_s = '1' ) then
         case v3_ModeD1_s is

           when v3_ModeNone_c =>
			 null;

           when v3_ModeRecord_c =>
             SetModeRec_s <= '1';

           when v3_ModePlayBackSingle_c =>
             SetModePlayContinuous_s <= '0';
             SetModePlay_s <= '1';

           when v3_ModePlayBackContinuous_c =>
             SetModePlayContinuous_s <= '1';
             SetModePlay_s <= '1';

           when v3_ModeRTDExHost2Mem_c =>
             SetModeRTDExHost2Fpga_s <= '1';

           when v3_ModeRTDExMem2Host_c =>
             SetModeRTDExFpga2Host_s <= '1';

           when others =>
             null;
         end case;

       end if;
     end if;
   end if;
 end process;

 --------------------------------------------------------------------------
 -- Capture status signal with teh system clock
 --------------------------------------------------------------------------
  process(i_SystemClk_p)
  begin
    if rising_edge(i_SystemClk_p) then
      TransferOverD1_s 		  <= TransferOver_s;
      ParityAddrRegD1_s 		<= ParityAddrReg_s;
      v_TrigAddrD1_s		    <= v_TrigAddr_s;
      v32_TrigAddrIndexD1_s	<= v32_TrigAddrIndex_s;
    end if;
  end process;

  mem_controller_inst : mem_controller
    port map
    (
      -- Control and registers
      i_Reset_p 				            => ResetD1_s,
      i_MemClk_p 			              => i_MemClk_p,
      iv_StartAddress_p 	          => v_StartAddress_s,
      iv_TransferSize_p 	          => v_TransferSize_s,
      o_TransferOver_p 		          => TransferOver_s,
      
      iv288_BufferDin_p	            => v288_BufferDin_D1_s,
      i_BufferWen_p                 => BufferWen_D1_s,

      --  Record FIFO interface
      o_Record_ResetFifo_p          => RecordResetFifo_s,
      -- iv32_Record_TriggerIn_p 		  => iv32_RecTriggerIn_p,
      -- iv256_Record_Data_p 			    => iv256_RecData_p,
      -- i_Record_DValid_p				      => i_RecDValid_p,
      -- o_Record_ReadData_p 			    => RecReadData_s,
      -- i_Record_FifoEmpty_p 			    => i_RecFifoEmpty_p,

      --  RtdexWr FIFO interface
      o_RtdexWr_ResetFifo_p         => RtdexRxResetFifo_s,
      -- iv256_RtdexWr_Data_p 			    => iv256_RtdexRxData_p,
      -- i_RtdexWr_DValid_p				    => i_RtdexDValid_p,
      -- o_RtdexWr_ReadData_p 			    => RtdexRxDReadData_s,
      -- i_RtdexWr_FifoEmpty_p 			  => i_RtdexFifoEmpty_p,

      --  Playback FIFO interface
      ov256_Playback_FifoWrData_p   => v256_PlayFifoWrData_s,
      o_Playback_FifoWrite_p        => PlayFifoWrite_s,
      o_Playback_ResetFifo_p        => o_PlayResetFifo_p,
      i_Playback_FifoProgFull_p     => i_PlayFifoProgFull_p,

      --  RtdexRd FIFO interface
      ov256_RtdexRd_FifoWrData_p    => v256_RtdexTxFifoWrData_s,
      o_RtdexRd_FifoWrite_p         => RtdexTxFifoWrite_s,
      o_RtdexRd_ResetFifo_p         => o_RtdexTxResetFifo_p,
      i_RtdexRd_FifoProgFull_p      => i_RtdexTxFifoProgFull_p,

      -- Control and status
      o_BufferProgFull_p            => BufferProgFull_s,
      o_Record_On_p                 => RecordStarted_s,
      o_RtdexWr_On_p                => RtdexRxStarted_s,
      i_Record_En_p 			          => SetModeRec_s,
      i_RtdexWr_En_p 			          => SetModeRTDExHost2Fpga_s,
      i_Playback_En_p 			        => SetModePlay_s,
      i_RtdexRd_En_p 			          => SetModeRTDExFpga2Host_s,
      i_ModeIsContinuous_p          => SetModePlayContinuous_s,
      ov32_Record_Cnt_p             => ov32_RecordStorageCnt_p,
      ov32_RtdexWr_Cnt_p            => ov32_RtdexStorageCnt_p,
      ov32_Playback_Cnt_p           => ov32_PlaybackReadCnt_p,
      ov32_RtdexRd_Cnt_p            => ov32_RtdexReadCnt_p,

      -- Trigger control and status for record mode
      iv_Record_TrigDly_p 		      => v_TrigDly_s,
      ov_Record_TrigAddr_p 		      => v_TrigAddr_s,
      ov32_Record_TrigAddrIndex_p 	=> v32_TrigAddrIndex_s,
      o_Record_ParityAddrReg_p 		  => ParityAddrReg_s,

      i_PlaybackTriggerIn_p         => i_PlayTriggerIn_p,


      -- memory interface
      o_AppWdfWren_p     	          => o_AppWdfWren_p,
      ov256_AppWdfData_p 	          => ov256_AppWdfData_p,
      o_AppWdfEnd_p      	          => o_AppWdfEnd_p,
      ov3_AppCmd_p       	          => ov3_AppCmd_p,
      o_AppEn_p          	          => o_AppEn_p,
      ov_TgAddr_p      	            => ov_TgAddr_p,
      iv256_AppRdData_p             => iv256_AppRdData_p,
      i_AppRdDataValid_p            => i_AppRdDataValid_p,

      i_AppRdy_p 			              => i_AppRdy_p,
      i_AppWdfRdy_p 			          => i_AppWdfRdy_p,
      i_PhyInitDone_p 		          => i_PhyInitDone_p
    );
    
    
  process(i_MemClk_p)
  begin
    if rising_edge(i_MemClk_p) then
        o_RecordResetFifo_p  <= RecordResetFifo_s;
        o_RtdexRxResetFifo_p <= RtdexRxResetFifo_s;
    end if;
  end process;

end rtl;



