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
use ieee.std_logic_unsigned.all;

library lyt_axi_pcie_rtdex_v1_00_a;
use lyt_axi_pcie_rtdex_v1_00_a.all;

entity fifo_to_axi_stream is
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
end fifo_to_axi_stream;

architecture fifo_to_axi_stream_behav of fifo_to_axi_stream is

  attribute keep_hierarchy : string;
  attribute keep_hierarchy of fifo_to_axi_stream_behav: architecture is "true";

  -- IRQ pulse logic
  --
  signal intReq_s               : std_logic;
  signal v_irqCnt_s             : std_logic_vector(2 downto 0);
  
  -- AXI Streaming logic
  --
  signal StartNewTransfer_s     : std_logic;
  signal StartNewTransferD1_s   : std_logic;
  signal StartNewTransferPulse_s: std_logic;

  signal strRst_s               : std_logic;

  type AXI_STR_DATA_FSM_t is
  (
    idle_c,
    txData_c
  );
  
  signal strDataFsm_s : AXI_STR_DATA_FSM_t; 

  type AXI_STR_CTRL_FSM_t is
  (
    strCtrlIdleState_c,
    strCtrlSendCmdState_c,
    strCtrlWaitCmdCompletionState_c
  );

  signal strCtrlFsm_s : AXI_STR_CTRL_FSM_t; 
  
  signal v24_currentTransferCnt_s: std_logic_vector(23 downto 0);
   
  -- FIFO signals
  --
  signal txFifoWrEn_s           : std_logic;
  signal txFifoWrEnR1_1_s       : std_logic;
  signal txFifoWrEnR1_2_s       : std_logic;
  signal fifoRst_s              : std_logic;
  signal fifoRstR1_s            : std_logic;
  signal fifoOverflow_s         : std_logic;
  signal fifoUnderflow_s        : std_logic;
  signal fifoEmpty_s            : std_logic;
  signal fifoRdEn_s             : std_logic;
  signal fifoProgFull_s         : std_logic;
  signal v64_fifoDout_s         : std_logic_vector(63 downto 0);
  
  signal v_fifoDin_s            : std_logic_vector(C_TX_WIDTH-1 downto 0);
  signal v_fifoDinR1_s          : std_logic_vector(C_TX_WIDTH-1 downto 0);
  signal v_fifoDinR2_s          : std_logic_vector(C_TX_WIDTH-1 downto 0);
  signal v_fifoDinReordered_s   : std_logic_vector(C_TX_WIDTH-1 downto 0);
  
  signal fifoWen_s              : std_logic;
  signal fifoWenR1_s            : std_logic;
  signal fifoWenR2_s            : std_logic;
  signal TxReady_s              : std_logic;
  
  signal v64_fifoDistDout_s     : std_logic_vector(63 downto 0);
  signal fifoDistEmpty_s        : std_logic;
  signal fifoDistRdEn_s         : std_logic;
  signal fifoDistAlmostFull_s   : std_logic;
  
  component fifo_d16_w64_fwft_dist
    port (
        clk : in std_logic;
        rst : in std_logic;
        din : in std_logic_vector(63 downto 0);
        wr_en : in std_logic;
        rd_en : in std_logic;
        dout : out std_logic_vector(63 downto 0);
        full : out std_logic;
        almost_full : out std_logic;
        empty : out std_logic;
        underflow : out std_logic
    );
  end component;
  
  attribute keep : string;
  attribute keep of v_fifoDin_s : signal is "true";
  attribute keep of v_fifoDinR1_s : signal is "true";
  attribute keep of txFifoWrEn_s : signal is "true";
  attribute keep of txFifoWrEnR1_1_s : signal is "true";
  attribute keep of txFifoWrEnR1_2_s : signal is "true";
  attribute keep of TxReady_s : signal is "true";
  attribute keep of fifoRst_s : signal is "true";
  
 begin


  -----------------------------------------------------------------------
  -- RTDEx TX FIFO
  -----------------------------------------------------------------------

  -- Resync
  --
  process(i_TxUserClk_p)
  begin
    if rising_edge(i_TxUserClk_p) then
      txFifoWrEn_s <= i_TxFifoWrEn_p;
      txFifoWrEnR1_1_s <= txFifoWrEn_s;
      txFifoWrEnR1_2_s <= txFifoWrEn_s;
      
      fifoRst_s <= i_TxReset_p or i_TxFifoReset_p;
      fifoRstR1_s <= fifoRst_s;
    end if;
  end process;
  
  -- Add two clock delays on the writes and the o_TxReady_p signals for timing issues.
  -- We use the fifoProgFull_s fifo status instead of full FIFO status to balance 
  -- this clock latency and more !
  --  
  process(i_TxUserClk_p)
  begin
    if rising_edge(i_TxUserClk_p) then
        
        v_fifoDin_s   <= iv_TxData_p;
        v_fifoDinR1_s <= v_fifoDin_s;
        v_fifoDinR2_s <= v_fifoDinR1_s;
        
        fifoWen_s       <= i_TxWe_p and txFifoWrEnR1_1_s;
        fifoWenR1_s     <= fifoWen_s;
        fifoWenR2_s     <= fifoWenR1_s;
        
        TxReady_s       <= (not fifoProgFull_s) and txFifoWrEnR1_2_s;
        o_TxReady_p     <= TxReady_s;
        
        if fifoRstR1_s = '1' then
            fifoWen_s       <= '0';
            fifoWenR1_s     <= '0';
            fifoWenR2_s     <= '0';
            TxReady_s       <= '0';
            o_TxReady_p     <= '0';
        end if;
    end if;
  end process;
  
  WordPackingGen: for k in 0 to ((C_TX_WIDTH/32)-1) generate
    v_fifoDinReordered_s((32*(k+1)-1) downto (32*k)) <= v_fifoDinR2_s((C_TX_WIDTH-32*k-1) downto (C_TX_WIDTH-32*(k+1)));
  end generate WordPackingGen;
  
  U0_genFifo: entity lyt_axi_pcie_rtdex_v1_00_a.generic_fifo
    generic map (
      WRITE_WIDTH_g                 => C_TX_WIDTH,
      READ_WIDTH_g                  => 64,
      WRITE_DEPTH_g                 => (WRITE_DEPTH_g*32/C_TX_WIDTH),
      READ_DEPTH_g                  => (WRITE_DEPTH_g*32/64),
      FIRST_WORD_FALL_THROUGH_g     => true,
      FULL_FLAGS_RST_VAL            => 1,
      USE_EMBEDDED_REG              => 0
    )
    port map (
      i_rst_p                       => fifoRstR1_s,
      i_wr_clk_p                    => i_TxUserClk_p,
      i_rd_clk_p                    => i_strClk_p,
      iv_din_p                      => v_fifoDinReordered_s,
      i_wr_en_p                     => fifoWenR2_s,
      i_rd_en_p                     => fifoRdEn_s,
      ov_dout_p                     => v64_fifoDout_s,
      o_full_p                      => open,
      o_prog_full_p                 => fifoProgFull_s,
      o_almost_full_p               => open,
      o_overflow_p                  => fifoOverflow_s,
      o_empty_p                     => fifoEmpty_s,
      o_valid_p                     => open,
      o_underflow_p                 => open,
      o_prog_empty_p                => open,
      iv_prog_empty_thresh_p        => X"000000",
      iv_prog_full_thresh_assert_p  => std_logic_vector(to_unsigned((WRITE_DEPTH_g*32/C_TX_WIDTH) - 10, 24)),
      iv_prog_full_thresh_negate_p  => std_logic_vector(to_unsigned((WRITE_DEPTH_g*32/C_TX_WIDTH) - 11, 24)),
      ov_rd_data_count_p            => open,
      ov_wr_data_count_p            => open
    );
    
  fifoRdEn_s <= (not fifoDistAlmostFull_s) and (not fifoEmpty_s);
    
  -- Add a distributed logic FIFO to ease timing
  fifo_d16_w64_fwft_dist_i : fifo_d16_w64_fwft_dist
    port map (
      clk => i_strClk_p,
      rst => fifoRstR1_s,
      din => v64_fifoDout_s,
      wr_en => fifoRdEn_s,
      rd_en => fifoDistRdEn_s,
      dout => v64_fifoDistDout_s,
      full => open,
      almost_full => fifoDistAlmostFull_s,
      empty => fifoDistEmpty_s,
      underflow => fifoUnderflow_s
    );

  AXI_STR_TDATA <= v64_fifoDistDout_s(31 downto 0) & v64_fifoDistDout_s(63 downto 32);
  

  -----------------------------------------------------------------------
  -- FIFO status held to '1' until reset
  -----------------------------------------------------------------------

  process(Bus2IP_Clk)
  begin
    if rising_edge(Bus2IP_Clk) then
      if (fifoRstR1_s = '1') then
        o_overflow_p <= '0';
      elsif (fifoOverflow_s = '1') then
        o_overflow_p <= '1';
      end if;
    end if;
  end process;

  process(Bus2IP_Clk)
  begin
    if rising_edge(Bus2IP_Clk) then
      if (fifoRstR1_s = '1') then
        o_underflow_p <= '0';
      elsif (fifoUnderflow_s = '1') then
        o_underflow_p <= '1';
      end if;
    end if;
  end process;
  

  -----------------------------------------------------------------------
  -- AXI DataMover: Data streaming interface
  -----------------------------------------------------------------------

  -- Detect RX Start/enable cmd
  --
--  process(Bus2IP_Clk)
--  begin
--   if rising_edge(Bus2IP_Clk) then
--      v8_RtdexRxEna_s <= iv8_RxStartNewTransfer_p;
--   end if;
--  end process;

  -- 
  --
  process(i_strClk_p)
    begin
    if rising_edge(i_strClk_p) then
      strRst_s <= fifoRstR1_s;
    end if;
  end process;

  -- Rising edge detection to start a transfer
  --
  process(i_strClk_p)
    begin
    if rising_edge(i_strClk_p) then
      StartNewTransfer_s <= i_StartNewTransfer_p;
      StartNewTransferD1_s <= StartNewTransfer_s;
    end if;
  end process;
  
  StartNewTransferPulse_s <= '1' when (StartNewTransfer_s = '1' and StartNewTransferD1_s = '0') else '0';

  process(i_strClk_p)
  begin
    if rising_edge(i_strClk_p) then
      if (strRst_s = '1') then
        strDataFsm_s <= idle_c;
      else
      
        -- Default values
                
        case strDataFsm_s is
        
          when idle_c =>
            -- Wait for request from the host
            if (StartNewTransferPulse_s = '1') then
              strDataFsm_s <= txData_c;
            end if;
          
          when txData_c =>
            -- transmit until iv32_TransferSize_p has been reached
            -- if () then
              strDataFsm_s <= idle_c;
            -- end if;
              
          when others => strDataFsm_s <= Idle_c; 
                         
        end case;   
              
      end if;
    end if;
  end process;                                                                       

 
  AXI_STR_TKEEP <= (others => '1');
  
  fifoDistRdEn_s  <= (not fifoDistEmpty_s) and AXI_STR_TREADY;
  AXI_STR_TVALID  <= fifoDistRdEn_s;
  AXI_STR_TLAST   <= '0';


  -----------------------------------------------------------------------
  -- AXI DataMover: Control streaming interface
  -----------------------------------------------------------------------

  AXI_STR_CTRL_STAT_ACLK      <= i_strClk_p;
  AXI_STR_CTRL_STAT_ARESETN   <= not strRst_s;
  AXI_STR_CTRL_TDATA          <= "0000" & iv4_dataMoverTag_p & iv32_dataMoverAddr_p & iv32_dataMoverCtrl_p;

  process(i_strClk_p)
  begin
    if rising_edge(i_strClk_p) then
      if (strRst_s = '1') then
        o_transferDone_p <= '0';
        v24_currentTransferCnt_s <= (others => '0');
        AXI_STR_CTRL_TVALID <= '0';
        intReq_s <= '0';
        strCtrlFsm_s <= strCtrlIdleState_c;
      else
      
        -- Default values
        AXI_STR_CTRL_TVALID <= '0';
        intReq_s <= '0';
        
        case strCtrlFsm_s is
        
          when strCtrlIdleState_c =>
            -- Reset Done status from previous transfer by setting StartNewTransfer to 0
            if (StartNewTransfer_s = '0') then
              o_transferDone_p <= '0';
            end if;
            -- Wait for request from the host            
            if (StartNewTransferPulse_s = '1') then
              v24_currentTransferCnt_s <= (others => '0');
              strCtrlFsm_s <= strCtrlSendCmdState_c;
            end if;
          
          when strCtrlSendCmdState_c =>
            -- Issue a command if dataMover is ready
            if (AXI_STR_CTRL_TREADY = '1') then
              v24_currentTransferCnt_s <= v24_currentTransferCnt_s + '1';
              AXI_STR_CTRL_TVALID <= '1';
              strCtrlFsm_s <= strCtrlWaitCmdCompletionState_c;
            end if;	
              
          when strCtrlWaitCmdCompletionState_c =>
            -- Verify if the transfer has completed
            --
            if (AXI_STR_STAT_TVALID = '1') then
              if (v24_currentTransferCnt_s >= iv24_transferCnt_p and i_streamingTransfer_p = '0') then
                -- Transmission completed, generate an interrupt if required
                --
                if (i_irqLastTransferEn_p = '1') then
                  intReq_s <= '1';
                end if;
                o_transferDone_p <= '1';
                strCtrlFsm_s <= strCtrlIdleState_c;
              else
                -- Issue a new command
                --
                strCtrlFsm_s <= strCtrlSendCmdState_c;
              end if;
            end if;
            
          when others => strCtrlFsm_s <= strCtrlIdleState_c; 
                         
        end case;   
              
      end if;
    end if;
  end process;

  ov24_currentTransferCnt_p <= v24_currentTransferCnt_s;
  
  
  -----------------------------------------------------------------------
  -- IRQ pulse logic
  -- The pulse is held 7 clock cycles
  -----------------------------------------------------------------------
  
  process(i_strClk_p)
    begin
    if rising_edge(i_strClk_p) then
      if (strRst_s = '1') then
        -- Reset the counter
        --
        v_irqCnt_s <= (others => '0');
      elsif (intReq_s = '1') then
        -- Load the counter
        --
        v_irqCnt_s <= (others => '1');
      elsif (v_irqCnt_s /= "000") then
        -- Counter -1 until it reaches 0
        --
        v_irqCnt_s <= v_irqCnt_s - '1';
      end if;
    end if;
  end process;

  process(i_strClk_p)
    begin
    if rising_edge(i_strClk_p) then
      if (v_irqCnt_s /= "000") then
        o_intReq_p <= '1';
      else
        o_intReq_p <= '0';
      end if;
    end if;
  end process;

  
  -----------------------------------------------------------------------
  -- AXI DataMover: Status streaming interface
  -----------------------------------------------------------------------

  -- Only latch the current status value in registers
  -- So always ready !
  -- AXI_STR_STAT_TKEEP and AXI_STR_STAT_TLAST are don't care
  
  AXI_STR_STAT_TREADY <= '1';

  process(i_strClk_p)
    begin
    if rising_edge(i_strClk_p) then
      if (strRst_s = '1') then
        ov8_dataMoverStatus_p <= (others => '0');
      elsif (AXI_STR_STAT_TVALID = '1') then
        ov8_dataMoverStatus_p <= AXI_STR_STAT_TDATA;
      end if;
    end if;
  end process;


  -----------------------------------------------------------------------
  -- Address posting
  -----------------------------------------------------------------------

    s2mm_allow_addr_req  <= '1';
--    s2mm_addr_req_posted          : in  std_logic;
--    s2mm_wr_xfer_cmplt            : in  std_logic;
--    s2mm_ld_nxt_len               : in  std_logic;
--    s2mm_wr_len                   : in  std_logic_vector(7 downto 0);
    
  -----------------------------------------------------------------------
  -- Output ports
  -----------------------------------------------------------------------
  

end fifo_to_axi_stream_behav;