
 library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;
 use ieee.std_logic_misc.all;

 library work;
 use work.recplay_type_p.all;

-------------------------------------------------------------------------------
-- Entity Section
-------------------------------------------------------------------------------
entity mem_controller is
generic
(
  ADDR_WIDTH  : integer := 30
);
port
(
     -- Control and registers
     i_Reset_p 				    : in std_logic;
     i_MemClk_p 			    : in std_logic;
     iv_StartAddress_p 	  : in std_logic_vector(ADDR_WIDTH-1 downto 0);
     iv_TransferSize_p 	  : in std_logic_vector(ADDR_WIDTH-1 downto 0);
     o_TransferOver_p 		: out std_logic;
     
     iv288_BufferDin_p	  : in std_logic_vector(287 downto 0);
     i_BufferWen_p        : in std_logic;

     --  Record FIFO interface
     o_Record_ResetFifo_p         : out std_logic;
     -- iv32_Record_TriggerIn_p 		  : in std_logic_vector(31 downto 0);
     -- iv256_Record_Data_p 			    : in std_logic_vector(255 downto 0);
     -- i_Record_DValid_p				    : in std_logic;
     -- o_Record_ReadData_p 			    : out std_logic;
     -- i_Record_FifoEmpty_p 			  : in std_logic;

     --  RtdexWr FIFO interface
     o_RtdexWr_ResetFifo_p        : out std_logic;
     -- iv256_RtdexWr_Data_p 			  : in std_logic_vector(255 downto 0);
     -- i_RtdexWr_DValid_p				    : in std_logic;
     -- o_RtdexWr_ReadData_p 			  : out std_logic;
     -- i_RtdexWr_FifoEmpty_p 			  : in std_logic;

     --  Playback FIFO interface
     ov256_Playback_FifoWrData_p  : out std_logic_vector(255 downto 0);
     o_Playback_FifoWrite_p       : out std_logic;
     o_Playback_ResetFifo_p       : out std_logic;
     i_Playback_FifoProgFull_p    : in std_logic;

     --  RtdexRd FIFO interface
     ov256_RtdexRd_FifoWrData_p  : out std_logic_vector(255 downto 0);
     o_RtdexRd_FifoWrite_p       : out std_logic;
     o_RtdexRd_ResetFifo_p       : out std_logic;
     i_RtdexRd_FifoProgFull_p    : in std_logic;

     -- Control and status
     o_BufferProgFull_p         : out std_logic;
     o_Record_On_p              : out std_logic;
     o_RtdexWr_On_p             : out std_logic;
     i_Record_En_p 			        : in std_logic;     -- This is a pulse signal
     i_RtdexWr_En_p 			      : in std_logic;     -- This is a pulse signal
     i_Playback_En_p 			      : in std_logic;     -- This is a pulse signal
     i_RtdexRd_En_p 			      : in std_logic;     -- This is a pulse signal
     i_ModeIsContinuous_p       : in std_logic;
     ov32_Record_Cnt_p          : out std_logic_vector(31 downto 0);
     ov32_RtdexWr_Cnt_p         : out std_logic_vector(31 downto 0);
     ov32_Playback_Cnt_p        : out std_logic_vector(31 downto 0);
     ov32_RtdexRd_Cnt_p         : out std_logic_vector(31 downto 0);

     -- Trigger control and status for record mode
     iv_Record_TrigDly_p 		      : in std_logic_vector(ADDR_WIDTH downto 0);
     ov_Record_TrigAddr_p 		    : out std_logic_vector(ADDR_WIDTH-1 downto 0);
     ov32_Record_TrigAddrIndex_p 	: out std_logic_vector(31 downto 0);
     o_Record_ParityAddrReg_p 		: out std_logic;

     i_PlaybackTriggerIn_p        : in std_logic;


     -- memory interface
     o_AppWdfWren_p     	: out std_logic;
     ov256_AppWdfData_p 	: out std_logic_vector(255 downto 0);
     o_AppWdfEnd_p      	: out std_logic;
     ov3_AppCmd_p       	: out std_logic_vector(2 downto 0);
     o_AppEn_p          	: out std_logic;
     ov_TgAddr_p      	  : out std_logic_vector(ADDR_WIDTH-1 downto 0);
     iv256_AppRdData_p    : in std_logic_vector(255 downto 0);
     i_AppRdDataValid_p   : in std_logic;

     i_AppRdy_p 			    : in std_logic;
     i_AppWdfRdy_p 			  : in std_logic;
     i_PhyInitDone_p 		  : in std_logic
);
end entity mem_controller;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture rtl of mem_controller is

 COMPONENT fifo_d16_w288_fwft_dist
  PORT (
    clk : IN STD_LOGIC;
    rst : IN STD_LOGIC;
    din : IN STD_LOGIC_VECTOR(287 DOWNTO 0);
    wr_en : IN STD_LOGIC;
    rd_en : IN STD_LOGIC;
    dout : OUT STD_LOGIC_VECTOR(287 DOWNTO 0);
    full : OUT STD_LOGIC;
    empty : OUT STD_LOGIC;
    almost_empty : OUT STD_LOGIC;
    prog_full : OUT STD_LOGIC
  );
 END COMPONENT;

  type MemoryState_t is ( MemoryIdle_c, FirstWrd_c, SecondWrd_c);
  signal MemoryState_s : MemoryState_t;

  type AcquCtrlFsm_t is (Idle_c, WaitForTrig_c, WaitForAcquOver_c);
  signal AcquCtrlFsm_s : AcquCtrlFsm_t;

  signal startMemoryTranfert_s    : std_logic;

  signal new_transaction_s        : std_logic;

  signal Record_En_latch_s        : std_logic;
  signal RtdexWr_En_latch_s       : std_logic;
  signal Playback_En_latch_s      : std_logic;
  signal RtdexRd_En_latch_s       : std_logic;
  signal ModeIsContinuous_latch_s : std_logic;


  -- Tell which mode is enabled
  signal writeEnabled_s           : std_logic;
  signal recordEnabled_s          : std_logic;
  signal rtdexWrEnabled_s         : std_logic;
  signal playbackEnabled_s        : std_logic;
  signal rtdexRdEnabled_s         : std_logic;
  -- Tell which mode is enabled or was the last one enabled
  signal recordLastEnabled_s      : std_logic;
  signal rtdexWrLastEnabled_s     : std_logic;
  signal playbackLastEnabled_s    : std_logic;
  signal rtdexRdLastEnabled_s     : std_logic;

  signal FifoRdProgFull_s         : std_logic;

  signal writeFifoReset_s         : std_logic;

  signal enable_mem_transaction_counter_s : std_logic;

  signal v3_AppCmd_s              : std_logic_vector(2 downto 0);

  signal v_TrigDly_comb_s : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_TrigDly_s      : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_LatchTrigDly_s      : std_logic_vector(ADDR_WIDTH-1 downto 0);

  signal v_CurrAddress_s : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_PrevAddress_s : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_LatchTriggerAddress_s : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal ParityAddress_s : std_logic;
  signal Record_TriggerParity_s : std_logic := '0';
  signal TrigHappened_s : std_logic;
  signal v32_TriggerIndex_s : std_logic_vector(31 downto 0) := (others => '0');

  signal AcquOver_s : std_logic;
  -- signal NegativeTrigAcqOver_s : std_logic;
  signal StorageEnd_s : std_logic;

  signal AppWdfWren_s : std_logic;
  signal AppWdfEnd_s  : std_logic;
  signal AppEn_s      : std_logic;

  signal v_Record_TrigAddr_s  : std_logic_vector(ADDR_WIDTH-1 downto 0) := (others => '0');
  signal IncAddress_s : std_logic;
  signal TransferOver_s : std_logic := '1';
  signal DlyIsNeg_s  : std_logic;
  signal v_RealAcquSize_s : unsigned(ADDR_WIDTH downto 0);
  signal v_TransferCnt_s : unsigned(ADDR_WIDTH downto 0);

  signal SetAcquCntInitVal_s : std_logic;

  signal AcquisitionIsOn_s : std_logic;

  signal v256_Data_s : std_logic_vector(255 downto 0);
  signal BufferFifoRen_s	: std_logic;
  -- signal BufferFifoWen_s	: std_logic;
  signal BufferProgFull_s		  : std_logic;
  signal BufferAlmostEmpty_s	: std_logic;

  signal v32_TriggerIn_s    : std_logic_vector(31 downto 0);
  signal v32_TriggerIn_R1_s : std_logic_vector(31 downto 0);

  signal v288_BufferDout_s	: std_logic_vector(287 downto 0);

  -- signal v8_delayRecordEnabled_s  : std_logic_vector(7 downto 0);
  -- signal v8_delayRtdexWrEnabled_s : std_logic_vector(7 downto 0);
  
  signal PhyInitDone_s       : std_logic;

  ----------------------------------------------------------
  -- Memory Read Section
  ----------------------------------------------------------

  signal ReadReady_s : std_logic;
  signal ModeContinuous_s : std_logic;

  signal TrigInRead_s : std_logic;


  -- *************** Module Constants definition **********--
  constant iMinimalInc_c : integer := 8;
  constant u29_MinimalInc_c : unsigned(ADDR_WIDTH downto 0) := to_unsigned( iMinimalInc_c, ADDR_WIDTH+1 );
  constant v29_Zero_c : std_logic_vector(ADDR_WIDTH downto 0) := ( others => '0' );
  -- *****************************************************--

  attribute bel : string;
  attribute bel of BufferFifoRen_process  : label is "A6LUT";

begin

  --------------------------------------------------------------------------
  -- Prog_Full_Threshold = 12
  --------------------------------------------------------------------------
  -- process(i_MemClk_p)
  -- begin
    -- if rising_edge(i_MemClk_p) then    
      -- if recordEnabled_s = '1' then
        -- v288_BufferDin_s <= iv32_Record_TriggerIn_p & iv256_Record_Data_p;
        -- BufferFifoWen_s <= i_Record_DValid_p;
      -- elsif rtdexWrEnabled_s = '1' then
        -- v288_BufferDin_s <= X"00000001" & iv256_RtdexWr_Data_p;
        -- BufferFifoWen_s <= i_RtdexWr_DValid_p;
      -- end if;
    -- end if;
  -- end process;

  Buffer_FWFT : fifo_d16_w288_fwft_dist
    PORT MAP (
      clk => i_MemClk_p,
      rst => writeFifoReset_s,
      din => iv288_BufferDin_p,
      wr_en => i_BufferWen_p,
      rd_en => BufferFifoRen_s,
      dout => v288_BufferDout_s,
      full => open,
      empty => open,
      almost_empty => BufferAlmostEmpty_s,
      prog_full => BufferProgFull_s
    );

  v32_TriggerIn_s <= v288_BufferDout_s(287 downto 256);
  v256_Data_s <= v288_BufferDout_s(255 downto 0);
 --------------------------------------------------------------------------
 -- FSM to read data from packer FIFO and write it to the DDR3 MIG
 -- interface in burst of 2x256 bits (Mem config = BL8).
 --------------------------------------------------------------------------

  -- Write to destination (DDR MIG) when PackFifo is Read (FWFT FIFO type)
  AppWdfWren_s   <= BufferFifoRen_s and writeEnabled_s;

  ReadStateProc_l : process(i_MemClk_p)
  begin
    if rising_edge(i_MemClk_p) then
    	if( i_Reset_p = '1' ) then
        MemoryState_s <= MemoryIdle_c;

      else
        case MemoryState_s is

          when MemoryIdle_c =>
            if AcquOver_s = '0' then
              MemoryState_s 	<= FirstWrd_c;
            end if;

          when FirstWrd_c =>
            -- Once the first write operation is done, switch to second word (or 1 cycle pause for Read)
            if AcquOver_s = '1' then
              MemoryState_s   <= MemoryIdle_c;
            elsif AppEn_s = '1' then
              MemoryState_s 	<= SecondWrd_c;
            end if;

          when SecondWrd_c =>
            if (writeEnabled_s = '1') then
              -- if Write verify if the secode word has been written
              if i_AppWdfRdy_p = '1' then
                if AcquOver_s = '1' then
                  MemoryState_s   <= MemoryIdle_c;
                else
                  MemoryState_s 	<= FirstWrd_c;
                end if;
              end if;
            else
              -- if Read nothing to do in this state
              if AcquOver_s = '1' then
                MemoryState_s   <= MemoryIdle_c;
              else
                MemoryState_s 	<= FirstWrd_c;
              end if;
            end if;

        end case;
	  	end if;
    end if;
  end process;
  
  BufferFifoRen_process : process(MemoryState_s, BufferAlmostEmpty_s, i_AppRdy_p, i_AppWdfRdy_p, AcquOver_s)
  begin
    BufferFifoRen_s 	<= '0';
    
    case MemoryState_s is
    
      when FirstWrd_c =>
        if AcquOver_s = '0' then
          if BufferAlmostEmpty_s = '0' and i_AppRdy_p = '1' and i_AppWdfRdy_p = '1' then
            BufferFifoRen_s 	<= '1'; -- Read the packer fifo and write DDR3 1st 256-bit word
          end if;
        end if;
        
      when SecondWrd_c =>
        if i_AppWdfRdy_p = '1' then
          BufferFifoRen_s 	<= '1'; -- Read the packer fifo and write DDR3 2nd 256-bit word
        end if;
      
      when others => null;
    end case;
  end process;

  ReadStateFifoCtrlProc_l : process(MemoryState_s, BufferAlmostEmpty_s, i_AppRdy_p, i_AppWdfRdy_p, ReadReady_s, writeEnabled_s, AcquOver_s)
  begin
    -- default values
    AppEn_s 		      <= '0';
    AppWdfEnd_s       <= '0';

    case MemoryState_s is

      when FirstWrd_c =>
        if AcquOver_s = '0' then
          if (writeEnabled_s = '1') then
            if BufferAlmostEmpty_s = '0' and i_AppRdy_p = '1' and i_AppWdfRdy_p = '1' then
              AppEn_s 		      <= '1'; -- Enable the DDR CTRLer CMD
            end if;
          else
            if ReadReady_s = '1' then
              AppEn_s 		      <= '1'; -- Enable the DDR CTRLer CMD
            end if;
          end if;
        end if;

      when SecondWrd_c =>
        if i_AppWdfRdy_p = '1' then
          AppWdfEnd_s    	  <= writeEnabled_s;		-- Burst of 2x256-bit end
        end if;

      when others => null;

    end case;
  end process;

 --------------------------------------------------------------------------
 -- Trigger Parity info: Shows in which halfe of the 2x256-bit write
 -- burst the trigger event took place:
 --			0 if trigger happened inside the 1st 256-bit
 --			1 if trigger happened inside the 2nd 256-bit
 --------------------------------------------------------------------------
  process(i_MemClk_p)
  begin
   	if rising_edge(i_MemClk_p) then
      if AppWdfEnd_s = '1' then
        ParityAddress_s <= '1';
      elsif AppWdfWren_s = '1' then
        ParityAddress_s <= '0';
      end if;
    end if;
  end process;

 --------------------------------------------------------------------------
 -- This signal is used as start signal from reading from the DDR3 memory
 --------------------------------------------------------------------------
 process(i_MemClk_p)
 begin
   if rising_edge(i_MemClk_p) then
   
     -- Generate a single pulse signal on rising edge.
     if playbackEnabled_s = '1' then
      TrigInRead_s <= i_PlaybackTriggerIn_p;
     elsif rtdexRdEnabled_s = '1' then
      TrigInRead_s <= '1';
     else
      TrigInRead_s <= '0';
     end if;

   end if;
 end process;

 --------------------------------------------------------------------------
 -- Increment the write address at each AppEn_s (BL8: addrs = addrs + 8)
 --------------------------------------------------------------------------
  process( i_MemClk_p )
  begin
    if( rising_edge( i_MemClk_p ) ) then

      -- Increment address when a command is sent to the MIG
      IncAddress_s <= AppEn_s;
  
      if( startMemoryTranfert_s = '1') then
        -- Latch the start address
        v_CurrAddress_s <= iv_StartAddress_p;
        v_PrevAddress_s <= (others => '0');
      elsif( IncAddress_s = '1' ) then

        v_CurrAddress_s <= std_logic_vector( unsigned( v_CurrAddress_s ) + iMinimalInc_c );
        v_PrevAddress_s <= v_CurrAddress_s;

        if (v_TransferCnt_s = iMinimalInc_c) then
          if ModeContinuous_s = '1' then
            v_CurrAddress_s <= iv_StartAddress_p;
            v_PrevAddress_s <= (others => '0');
          end if;
        end if;
      end if;
    end if;
  end process;


  --------------------------------------------------------------------------
  -- Detect is a read operation can be done
  --------------------------------------------------------------------------
  ReadReady_s <= i_AppRdy_p and not(FifoRdProgFull_s);

  process(i_MemClk_p)
  begin
    if rising_edge(i_MemClk_p) then
      -- It's fine to delay this signal by 1 cycle because ProgFull is asserted many
      -- cycles before the real fifo full
      if playbackEnabled_s = '1' then
        FifoRdProgFull_s <= i_Playback_FifoProgFull_p;
      elsif rtdexRdEnabled_s = '1' then
        FifoRdProgFull_s <= i_RtdexRd_FifoProgFull_p;
      else
        FifoRdProgFull_s <= '1';
      end if;
    end if;
  end process;

 --------------------------------------------------------------------------
 -- FSM to control the acquisition stop and start. It also calculats the
 -- trigger infos to make it available for the host: Trigger Address,
 -- trigger Index and trigger Parity
 --------------------------------------------------------------------------
 process(i_MemClk_p)
 begin
 	if rising_edge(i_MemClk_p) then
 		if i_Reset_p = '1' then
      v3_AppCmd_s <= CMD_WRITE_c;
 			AcquCtrlFsm_s <= Idle_c;
 			TransferOver_s  <= '1';
 			SetAcquCntInitVal_s <= '0';
-- 			enable_mem_transaction_counter_s	<= '0';

      new_transaction_s   <= '0';

      Record_En_latch_s   <= '0';
      RtdexWr_En_latch_s  <= '0';
      Playback_En_latch_s <= '0';
      RtdexRd_En_latch_s  <= '0';
      ModeIsContinuous_latch_s <= '0';

      writeEnabled_s <= '0';
 			recordEnabled_s <= '0';
 			rtdexWrEnabled_s <= '0';
 			playbackEnabled_s <= '0';
 			rtdexRdEnabled_s <= '0';

      recordLastEnabled_s <= '0';
      rtdexWrLastEnabled_s <= '0';
      playbackLastEnabled_s <= '0';
      rtdexRdLastEnabled_s <= '0';

      ModeContinuous_s <= '0';

      o_Playback_ResetFifo_p <= '1';
      o_RtdexRd_ResetFifo_p <= '1';
      writeFifoReset_s <= '1';
      
      v_LatchTriggerAddress_s <= (others => '0');
      v_LatchTrigDly_s        <= (others => '0');
      
      PhyInitDone_s <= '0';
      
 		else
    
      PhyInitDone_s <= i_PhyInitDone_p;
    
      v_Record_TrigAddr_s <= std_logic_vector( unsigned(v_LatchTriggerAddress_s) + unsigned(v_LatchTrigDly_s) );

 			-- Default value: --
 			SetAcquCntInitVal_s <= '0';
      o_Record_ResetFifo_p <= '0';
      o_RtdexWr_ResetFifo_p <= '0';
      o_Playback_ResetFifo_p <= '0';
      o_RtdexRd_ResetFifo_p <= '0';
      writeFifoReset_s <= '0';
      

      --------------------------------------------------------------------------
      -- Found-out if trigger event occurs: check if any bit of iv32_Record_TriggerIn_p
      -- went high
      --------------------------------------------------------------------------
      TrigHappened_s      <= or_reduce(v32_TriggerIn_s) and BufferFifoRen_s;
      v32_TriggerIn_R1_s  <= v32_TriggerIn_s;

 			case AcquCtrlFsm_s is

 				when Idle_c =>

          if new_transaction_s = '1' then
            TransferOver_s <= '0';
            
            -- Only clear the previous record trigger result
            -- when launching a new record
            if (Record_En_latch_s = '1') then
                v_Record_TrigAddr_s <= (others=>'0');
                v32_TriggerIndex_s <= (others=>'0');
                Record_TriggerParity_s <= '0';
            end if;
            
--            enable_mem_transaction_counter_s	<= '0';
            AcquCtrlFsm_s <= WaitForTrig_c;

            if (Record_En_latch_s = '1' or RtdexWr_En_latch_s = '1') then
              writeFifoReset_s <= '1';
              writeEnabled_s <= '1';
              v3_AppCmd_s <= CMD_WRITE_c;
              SetAcquCntInitVal_s <= '1';
            else
              writeEnabled_s <= '0';
              v3_AppCmd_s <= CMD_READ_c;
              SetAcquCntInitVal_s <= '0';
            end if;

            o_Record_ResetFifo_p    <= Record_En_latch_s;
            o_RtdexWr_ResetFifo_p   <= RtdexWr_En_latch_s;
            o_Playback_ResetFifo_p  <= Playback_En_latch_s;
            o_RtdexRd_ResetFifo_p   <= RtdexRd_En_latch_s;

            if Playback_En_latch_s = '1' then
              ModeContinuous_s <= ModeIsContinuous_latch_s;
            else
              ModeContinuous_s <= '0';
            end if;

            recordEnabled_s   <= Record_En_latch_s;
            rtdexWrEnabled_s  <= RtdexWr_En_latch_s;
            playbackEnabled_s <= Playback_En_latch_s;
            rtdexRdEnabled_s  <= RtdexRd_En_latch_s;

            recordLastEnabled_s   <= Record_En_latch_s;
            rtdexWrLastEnabled_s  <= RtdexWr_En_latch_s;
            playbackLastEnabled_s <= Playback_En_latch_s;
            rtdexRdLastEnabled_s  <= RtdexRd_En_latch_s;
          end if;

          new_transaction_s <= '0';

 				when WaitForTrig_c =>

 					if (writeEnabled_s = '1') and TrigHappened_s = '1' then

            -- If in record mode, latch trigger address and parity to send it to the microblaze
            if recordEnabled_s = '1' then
              if ParityAddress_s = '1' then
                -- This means that the 256-bit word was already writen
                -- then the address was incremented. So trigger's addrs = Previous (+ trigger delay, if any)
                v_LatchTriggerAddress_s <= v_PrevAddress_s;
                v_LatchTrigDly_s        <= v_TrigDly_s;
                Record_TriggerParity_s  <= '1';
              else
                -- This means that the 256-bit word was not yet writen
                -- So trigger's addrs = Current (+ trigger delay, if any)
                v_LatchTriggerAddress_s <= v_CurrAddress_s;
                v_LatchTrigDly_s        <= v_TrigDly_s;
                Record_TriggerParity_s  <= '0';
              end if;
              
              v32_TriggerIndex_s <= v32_TriggerIn_R1_s;
            end if;

            AcquCtrlFsm_s <= WaitForAcquOver_c;
--            enable_mem_transaction_counter_s	<= '1'; -- Enable the acquisition down-counter

          elsif (writeEnabled_s = '0') and TrigInRead_s = '1' then
            AcquCtrlFsm_s <= WaitForAcquOver_c;
--            enable_mem_transaction_counter_s	<= '1'; -- Enable the acquisition down-counter
            SetAcquCntInitVal_s <= '1';
 					end if;

 				when WaitForAcquOver_c =>
 					-- Wait for the acquisition end then inhibit further Writes to DDR3
 					if(AcquOver_s = '1' and MemoryState_s = MemoryIdle_c and SetAcquCntInitVal_s = '0') then
 						TransferOver_s <= '1'; -- Signals acquisition done to the host
-- 						enable_mem_transaction_counter_s	<= '0';
 						AcquCtrlFsm_s <= Idle_c;

            recordEnabled_s   <= '0';
            rtdexWrEnabled_s  <= '0';
            playbackEnabled_s <= '0';
            rtdexRdEnabled_s  <= '0';
 					end if;

 			end case;

      -- If new memory transfert requested, overwrite the state
      if ((i_Record_En_p or i_RtdexWr_En_p or i_Playback_En_p or i_RtdexRd_En_p) and PhyInitDone_s) = '1' then

        new_transaction_s         <= '1';
        Record_En_latch_s         <= i_Record_En_p;
        RtdexWr_En_latch_s        <= i_RtdexWr_En_p;
        Playback_En_latch_s       <= i_Playback_En_p;
        RtdexRd_En_latch_s        <= i_RtdexRd_En_p;
        ModeIsContinuous_latch_s  <= i_ModeIsContinuous_p;
      end if;

 		end if;
 	end if;
 end process;

  -- Set to 1 when a new transaction is started
  -- (Idle mode and a new transaction is requested)
  startMemoryTranfert_s <= '1' when (new_transaction_s = '1' and AcquCtrlFsm_s = Idle_c) else '0';

  --------------------------------------------------------------------------
  -- Capture transfere size and trigger delay
  --------------------------------------------------------------------------

  v_TrigDly_comb_s(2 downto 0) <= (others =>'0'); -- Multiply by 8 (Expressed in term of 64 bytes block value)
  v_TrigDly_comb_s(v_TrigDly_s'high downto 3) <= iv_Record_TrigDly_p(v_TrigDly_s'high-3 downto 0); -- for 1 GB memory capacity limit

  process(i_MemClk_p)
  begin
    if rising_edge(i_MemClk_p) then
      if startMemoryTranfert_s = '1' then

        if Record_En_latch_s = '1' then
          v_TrigDly_s <= v_TrigDly_comb_s;
          DlyIsNeg_s <= iv_Record_TrigDly_p(iv_Record_TrigDly_p'high); -- bit 28 is used as a signe bit (0=Positive & 1=negative trig)

          -- The real acquisition size = requisted record size + trigger delay
          -- We added one bit to the sum to couver the wrap-up past 1 GB of SODIMM memory
          v_RealAcquSize_s <= resize(unsigned(iv_TransferSize_p),ADDR_WIDTH + 1) + resize(unsigned(v_TrigDly_comb_s),ADDR_WIDTH + 1);
          
        else
          v_TrigDly_s <= (others => '0');
          DlyIsNeg_s  <= '0';
          
          v_RealAcquSize_s <= resize(unsigned(iv_TransferSize_p),ADDR_WIDTH + 1);
        end if;
      end if;
    end if;
  end process;
  
  
  enable_mem_transaction_counter_s <= '1' when (AcquCtrlFsm_s = WaitForAcquOver_c or ((writeEnabled_s = '1') and TrigHappened_s = '1')) else '0';

 --------------------------------------------------------------------------
 -- Acquisition size down-counter to check when we're done capturing required
 -- amount of data.
 --------------------------------------------------------------------------
  process(i_MemClk_p)
    begin
    if rising_edge(i_MemClk_p) then
      if (SetAcquCntInitVal_s = '1') then  -- Load the couter with the initial value
        v_TransferCnt_s <= v_RealAcquSize_s;
        AcquOver_s <= '0';

      elsif (IncAddress_s = '1' and enable_mem_transaction_counter_s = '1') then

        v_TransferCnt_s <= v_TransferCnt_s - iMinimalInc_c;

        if (v_TransferCnt_s = iMinimalInc_c) then
          AcquOver_s <= '1';
          if ModeContinuous_s = '1' then
            v_TransferCnt_s <= v_RealAcquSize_s;
            AcquOver_s <= '0';
          end if;
        end if;

      end if;

      if (IncAddress_s and new_transaction_s) = '1' then
        v_TransferCnt_s <= (others => '0');
        AcquOver_s <= '1';
      end if;

      if i_Reset_p = '1' then
        v_TransferCnt_s <= (others => '0');
        AcquOver_s <= '1';
      end if;
    end if;
  end process;
  
--  AcquOver_s <= '1' when v_TransferCnt_s = to_unsigned(0,v_TransferCnt_s'length) else '0';
  
  

  -- Output the v_TransferCnt_s to the corresponding output depending of the last mode enabled
  process(i_MemClk_p, i_Reset_p)
  begin
    if rising_edge(i_MemClk_p) then
      if recordLastEnabled_s = '1' then
        ov32_Record_Cnt_p <= std_logic_vector(resize(v_TransferCnt_s,32));
      end if;

      if rtdexWrLastEnabled_s = '1' then
        ov32_RtdexWr_Cnt_p <= std_logic_vector(resize(v_TransferCnt_s,32));
      end if;

      if playbackLastEnabled_s = '1' then
        ov32_Playback_Cnt_p <= std_logic_vector(resize(v_TransferCnt_s,32));
      end if;

      if rtdexRdLastEnabled_s = '1' then
        ov32_RtdexRd_Cnt_p <= std_logic_vector(resize(v_TransferCnt_s,32));
      end if;

      -- Synchronous reset
      if i_Reset_p = '1' then
        ov32_Record_Cnt_p   <= (others => '0');
        ov32_RtdexWr_Cnt_p  <= (others => '0');
        ov32_Playback_Cnt_p <= (others => '0');
        ov32_RtdexRd_Cnt_p  <= (others => '0');
      end if;
    end if;
  end process;

  --------------------------------------------------------------------------
  --               Negative trig delay (pre-trig)
  -- In the case of nagative trig delay, if xferSize = TrigDly we stop
  -- immediatly upon reception of a trig. The same thing happens if TrigDly
  -- is > xferSize (theire sum is negative)
  --------------------------------------------------------------------------
  ---- NOT SUPPORTED
  -- NegCondProc_l : process( DlyIsNeg_s, v_RealAcquSize_s )
  -- begin

    -- NegativeTrigAcqOver_s <= '0';

    -- if( DlyIsNeg_s = '1' ) then

      ----XferSize = abs(TrigDelay)
      -- if v_RealAcquSize_s = to_unsigned(0,v_RealAcquSize_s'length) then
        -- NegativeTrigAcqOver_s <= '1';
      -- end if;

      ----XferSize < abs(TrigDelay)
      -- if (v_RealAcquSize_s(v_RealAcquSize_s'high) = '1' )then
        -- NegativeTrigAcqOver_s <= '1';
      -- end if;

    -- end if;
  -- end process;

  -----------------------------------------
  -- Delay enable signal to let the write FIFO clear its reset
  -----------------------------------------
  -- process(i_MemClk_p)
  -- begin
    -- if rising_edge(i_MemClk_p) then
      -- if recordEnabled_s = '1' then
        -- v8_delayRecordEnabled_s <= v8_delayRecordEnabled_s(6 downto 0) & '1';
      -- else
        -- v8_delayRecordEnabled_s <= (others => '0');
      -- end if;
      
      -- if rtdexWrEnabled_s = '1' then
        -- v8_delayRtdexWrEnabled_s <= v8_delayRtdexWrEnabled_s(6 downto 0) & '1';
      -- else
        -- v8_delayRtdexWrEnabled_s <= (others => '0');
      -- end if;
    -- end if;
  -- end process;

  -----------------------------------------
  -- Output ports
  -----------------------------------------
  o_BufferProgFull_p   <= BufferProgFull_s;
  -- o_Record_ReadData_p  <= (not BufferProgFullR1_s) and (not i_Record_FifoEmpty_p) and v8_delayRecordEnabled_s(v8_delayRecordEnabled_s'high);
  -- o_RtdexWr_ReadData_p <= (not BufferProgFullR1_s) and (not i_RtdexWr_FifoEmpty_p) and v8_delayRtdexWrEnabled_s(v8_delayRtdexWrEnabled_s'high);

  -- MIG ports
  ov_TgAddr_p          <= v_CurrAddress_s;
  o_AppWdfWren_p       <= AppWdfWren_s;
  ov256_AppWdfData_p   <= v256_Data_s;
  o_AppWdfEnd_p        <= AppWdfEnd_s;
  ov3_AppCmd_p         <= v3_AppCmd_s;
  o_AppEn_p            <= AppEn_s;

  -- Write CMD is the oly one issued here
  ov_Record_TrigAddr_p         <= v_Record_TrigAddr_s;
  ov32_Record_TrigAddrIndex_p  <= v32_TriggerIndex_s;
  o_Record_ParityAddrReg_p     <= Record_TriggerParity_s;

  -- Output the status of the transfer
  o_TransferOver_p      <= TransferOver_s;

  -- Allow write operation in the record packer FIFO
  o_Record_On_p                <= recordEnabled_s;
  o_RtdexWr_On_p               <= rtdexWrEnabled_s;

  -- Send data to FIFOs
  -- Write only in the active FIFO
  ov256_Playback_FifoWrData_p <= iv256_AppRdData_p;
  ov256_RtdexRd_FifoWrData_p <= iv256_AppRdData_p;
  -- Fifo reset is edge not level sensitive, so we added wen signal to prevent
  -- writes to it when the other port is active and not this one.
  o_Playback_FifoWrite_p <= i_AppRdDataValid_p and playbackLastEnabled_s;
  o_RtdexRd_FifoWrite_p <= i_AppRdDataValid_p and rtdexRdLastEnabled_s;


end rtl;
