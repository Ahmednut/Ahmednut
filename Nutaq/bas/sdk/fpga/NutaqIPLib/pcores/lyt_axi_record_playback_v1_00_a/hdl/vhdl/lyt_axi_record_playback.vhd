
------------------------------------------------------------------------------
-- Filename:          lyt_axi_record_playback.vhd
-- Version:           v1_00_a
-- Description:       Top level design, instantiates library components and
--                    user logic.
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Copyright (c) 2001-2012 LYRtech RD Inc.  All rights reserved.
------------------------------------------------------------------------------

 library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;
 use ieee.std_logic_misc.all;

 library lyt_axi_record_playback_v1_00_a;
 use lyt_axi_record_playback_v1_00_a.v6Ddr3Controler64b_p.all;
 use lyt_axi_record_playback_v1_00_a.recplay_type_p.all;

-------------------------------------------------------------------------------
-- Entity Section
-------------------------------------------------------------------------------
entity lyt_axi_record_playback is
  generic
  (
    RecordPortWidth_g              : integer := 8;
    NumberOfRecordPorts_g          : integer := 1;

    PlayBackPortWidth_g            : integer := 8;
    NumberOfPlayBackPorts_g        : integer := 1;

    C_S_AXI_DATA_WIDTH             : integer              := 32;
    C_S_AXI_ADDR_WIDTH             : integer              := 32;
    C_S_AXI_MIN_SIZE               : std_logic_vector     := X"000001FF";
    C_USE_WSTRB                    : integer              := 0;
    C_DPHASE_TIMEOUT               : integer              := 8;
    C_BASEADDR                     : std_logic_vector     := X"FFFFFFFF";
    C_HIGHADDR                     : std_logic_vector     := X"00000000";
    C_FAMILY                       : string               := "virtex6"
  );
  port
  (
    -- User Interface
    i_RecDataClk_p                : in std_logic;

    i_RecTrigger_p                : in std_logic;
    iv_RecDataPort0_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort1_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort2_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort3_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort4_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort5_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort6_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort7_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort8_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort9_p             : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort10_p            : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort11_p            : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort12_p            : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort13_p            : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort14_p            : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    iv_RecDataPort15_p            : in std_logic_vector(RecordPortWidth_g - 1 downto 0);
    i_RecWriteEn_p                : in std_logic;
    o_RecFifoFull_p               : out std_logic;
    o_RecFifoEmpty_p              : out std_logic;


    i_PlayDataClk_p                : in std_logic;

    i_PlayTriggerIn_p              : in std_logic;
    ov_PlayDataPort0_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort1_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort2_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort3_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort4_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort5_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort6_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort7_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort8_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort9_p             : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort10_p            : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort11_p            : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort12_p            : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort13_p            : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort14_p            : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    ov_PlayDataPort15_p            : out std_logic_vector(PlayBackPortWidth_g - 1 downto 0);
    o_PlayValid_p                  : out std_logic;
    i_PlayReadEn_p                 : in std_logic;
    o_PlayEmpty_p                  : out std_logic;

    -- RTDEx interface
    i_RTDExRxClock_p   : in std_logic;
    i_RxDataValid_p : in std_logic;

    o_RxRe_p         : out std_logic;
    i_RxReady_p      : in std_logic;

    iv32_RxDataCh0_p : in std_logic_vector(31 downto 0);

    i_RTDExTxClock_p   : in std_logic;
    o_TxWe_p          : out std_logic;
    i_TxReady_p       : in std_logic;
    ov32_TxDataCh0_p  : out std_logic_vector(31 downto 0);

    -- Memory Interface
    o_MemClk_p :          out std_logic;
    sys_clk :             in std_logic;
    clk_ref :             in std_logic;
    ddr3_dq :             inout std_logic_vector(63 downto 0);
    ddr3_addr :           out std_logic_vector(15 downto 0);
    ddr3_ba :             out std_logic_vector(2 downto 0);
    ddr3_ras_n :          out std_logic;
    ddr3_cas_n :          out std_logic;
    ddr3_we_n :           out std_logic;
    ddr3_reset_n :        out std_logic;
    ddr3_cs_n :           out std_logic_vector(0 downto 0);
    ddr3_odt :            out std_logic_vector(0 downto 0);
    ddr3_cke :            out std_logic_vector(0 downto 0);
    ddr3_dm :             out std_logic_vector(7 downto 0);
    ddr3_dqs_p :          inout std_logic_vector(7 downto 0);
    ddr3_dqs_n :          inout std_logic_vector(7 downto 0);
    ddr3_ck_p :           out std_logic_vector(1 downto 0);
    ddr3_ck_n :           out std_logic_vector(1 downto 0);
    iodelay_ctrl_rdy :    in std_logic;

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
        S_AXI_ACLK                     : in  std_logic;
        S_AXI_ARESETN                  : in  std_logic;
        S_AXI_AWADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
        S_AXI_AWVALID                  : in  std_logic;
        S_AXI_WDATA                    : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
        S_AXI_WSTRB                    : in  std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
        S_AXI_WVALID                   : in  std_logic;
        S_AXI_BREADY                   : in  std_logic;
        S_AXI_ARADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
        S_AXI_ARVALID                  : in  std_logic;
        S_AXI_RREADY                   : in  std_logic;
        S_AXI_ARREADY                  : out std_logic;
        S_AXI_RDATA                    : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
        S_AXI_RRESP                    : out std_logic_vector(1 downto 0);
        S_AXI_RVALID                   : out std_logic;
        S_AXI_WREADY                   : out std_logic;
        S_AXI_BRESP                    : out std_logic_vector(1 downto 0);
        S_AXI_BVALID                   : out std_logic;
        S_AXI_AWREADY                  : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );

  attribute MAX_FANOUT   : string;
  attribute SIGIS                          : string;
  attribute MAX_FANOUT of S_AXI_ACLK       : signal is "10000";
  attribute MAX_FANOUT of S_AXI_ARESETN    : signal is "10000";
  attribute SIGIS of S_AXI_ACLK            : signal is "Clk";
  attribute SIGIS of clk_ref            : signal is "Clk";
  attribute SIGIS of S_AXI_ARESETN         : signal is "Rst";

end lyt_axi_record_playback;

architecture rtl of lyt_axi_record_playback is

  -------------------------------------------------------------------------------
  -- Constant declarations
  -------------------------------------------------------------------------------

  constant ROW_WIDTH  : integer := 16;
  constant ADDR_WIDTH : integer := 30;

  -------------------------------------------------------------------------------
  -- Signal declarations
  -------------------------------------------------------------------------------

  signal MemClk_s                     : std_logic;
  signal CoreReset_s                  : std_logic;
  signal MemReset_s                   : std_logic;
  signal PhyInitDone_s                : std_logic;
  signal Reset_s                      : std_logic;

  signal AppEn_s                      : std_logic;
  signal v_TgAddr_s                   : std_logic_vector(ADDR_WIDTH-1 downto 0);
  --signal AppFull_s                    : std_logic;
  signal v256_AppRdData_s             : std_logic_vector(255 downto 0);
  signal AppRdDataValid_s             : std_logic;
  signal AppWdfWren_s                 : std_logic;
  signal v256_AppWdfData_s            : std_logic_vector(255 downto 0);
  signal AppWdfEnd_s                  : std_logic;
  signal v3_AppCmd_s                  : std_logic_vector(2 downto 0);
  --signal AppWdfFull_s                 : std_logic;
  --signal AppWdfAFull_s                : std_logic;

  signal v32_Trigger_s                : std_logic_vector(31 downto 0);
  signal v256_RecData_s               : std_logic_vector(255 downto 0);
  signal RecReadData_s                : std_logic;
  signal RecDValid_s                        : std_logic;

  signal v256_PlayFifoWrData_s        : std_logic_vector(255 downto 0);
  signal PlayFifoWrite_s              : std_logic;
  signal PlayResetFifo_s              : std_logic;
  signal RecordResetFifo_s            : std_logic;
  signal RtdexRxResetFifo_s           : std_logic;
  signal PlayFifoProgFull_s           : std_logic;

  signal v32_StartAddress_s           : std_logic_vector(31 downto 0);
  signal v32_TrigDly_s                : std_logic_vector(31 downto 0);
  signal v32_TransferSize_s           : std_logic_vector(31 downto 0);
  signal v32_TrigAddr_s               : std_logic_vector(31 downto 0);
  signal v_StartAddress_s             : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_TrigDly_s                  : std_logic_vector(ADDR_WIDTH downto 0);
  signal v_TransferSize_s             : std_logic_vector(ADDR_WIDTH-1 downto 0);
  signal v_TrigAddr_s                 : std_logic_vector(ADDR_WIDTH-1 downto 0);

  signal SetMode_s                    : std_logic;
  signal v3_ModeValue_s               : std_logic_vector(2 downto 0);
  signal v32_TrigAddrIndex_s          : std_logic_vector(31 downto 0);
  signal TransferOver_s               : std_logic;
  signal ParityAddrReg_s              : std_logic;

  signal v256_RtdexRxData_s           : std_logic_vector(255 downto 0);
  signal RtdexRxReadData_s            : std_logic;
  signal RtdexDValid_s                : std_logic;

  signal v256_RtdexTxFifoWrData_s     : std_logic_vector(255 downto 0);
  signal RtdexTxFifoWrite_s           : std_logic;
  signal RtdexTxResetFifo_s           : std_logic;
  signal RtdexTxFifoProgFull_s        : std_logic;
  
  signal RecTriggerReLatch_s          : std_logic;  
  signal RecSoftTrigLatch_s           : std_logic;  
  signal RecTriggerR1_s               : std_logic;
  signal RecTriggerRe_s               : std_logic;
  signal v2_RecTrigMux_s              : std_logic_vector(1 downto 0);
  signal v2_RecTrigMuxD1_s            : std_logic_vector(1 downto 0);
  signal v2_RecTrigMuxD2_s            : std_logic_vector(1 downto 0);
  signal v2_RecTrigMuxD3_s            : std_logic_vector(1 downto 0);
  signal RecSoftTrig_s                : std_logic;
  signal RecSoftTrigD1_s              : std_logic;
  signal RecSoftTrigD2_s              : std_logic;
  signal RecSoftTrigD3_s              : std_logic;
  signal RecTrigger_s                 : std_logic;
  signal RecordStarted_s              : std_logic;
  signal RecordStartedR1_s            : std_logic;
  signal RecordStartedR2_s            : std_logic;
  signal RtdexRxStarted_s             : std_logic;

  signal v2_PlayTrigMux_s             : std_logic_vector(1 downto 0);
  signal v2_PlayTrigMuxD1_s           : std_logic_vector(1 downto 0);
  signal v2_PlayTrigMuxD2_s           : std_logic_vector(1 downto 0);
  signal v2_PlayTrigMuxD3_s           : std_logic_vector(1 downto 0);
  
  signal PlaySoftTrig_s               : std_logic;
  signal PlaySoftTrigD1_s             : std_logic;
  signal PlaySoftTrigD2_s             : std_logic;
  signal PlaySoftTrigD3_s             : std_logic;
  signal PlaySoftTrigD4_s             : std_logic;
  signal PlaySoftTrigD5_s             : std_logic;
  signal PlaySoftTrigD6_s             : std_logic;
  signal PlayTrigger_s                : std_logic;
  signal PlayTriggerInRe_s            : std_logic;
  signal PlayTriggerInR1_s            : std_logic;

  constant v32_NbRecordPorts_c        : std_logic_vector(31 downto 0) := std_logic_vector( to_unsigned ( NumberOfRecordPorts_g, 32 ) );
  constant v32_NbPlaybackPorts_c      : std_logic_vector(31 downto 0) := std_logic_vector( to_unsigned ( NumberOfPlayBackPorts_g, 32 ) );
  constant v32_RecordPortsWidth_c     : std_logic_vector(31 downto 0) := std_logic_vector( to_unsigned ( RecordPortWidth_g, 32 ) );
  constant v32_PlaybackPortsWidth_c   : std_logic_vector(31 downto 0) := std_logic_vector( to_unsigned ( PlayBackPortWidth_g, 32 ) );

  signal v9_ProgFullThresAssert_s     : std_logic_vector(8 downto 0);
  signal v9_ProgFullThresNegate_s     : std_logic_vector(8 downto 0);

  signal RecFifoFull_s                : std_logic;

  signal app_rdy_s, app_wdf_rdy_s     : std_logic;
  signal axi_areset_s                 : std_logic;


  signal ov_PlayDataPort0_s           : std_logic_vector(PlayBackPortWidth_g-1 downto 0);
  signal ov_PlayDataPort1_s           : std_logic_vector(PlayBackPortWidth_g-1 downto 0);
  signal o_PlayValid_s                : std_logic;
  signal o_PlayEmpty_s                : std_logic;

  signal RecFifoEmpty_s               : std_logic;
  signal RtdexFifoEmpty_s             : std_logic;

  signal v8_SignalStretch_s                : std_logic_vector(7 downto 0);
  signal CoreResetPulse_s             : std_logic;

  signal v32_RecordStorageCnt_s       : std_logic_vector(31 downto 0);
  signal v32_RecordStorageCntR1_s       : std_logic_vector(31 downto 0);
  signal v32_RecordStorageCntR2_s       : std_logic_vector(31 downto 0);
  signal v32_RtdexStorageCnt_s        : std_logic_vector(31 downto 0);
  signal v32_RtdexStorageCntR1_s        : std_logic_vector(31 downto 0);
  signal v32_RtdexStorageCntR2_s        : std_logic_vector(31 downto 0);
  signal v32_PlaybackReadCnt_s        : std_logic_vector(31 downto 0);
  signal v32_PlaybackReadCntR1_s        : std_logic_vector(31 downto 0);
  signal v32_PlaybackReadCntR2_s        : std_logic_vector(31 downto 0);
  signal v32_RtdexReadCnt_s           : std_logic_vector(31 downto 0);
  signal v32_RtdexReadCntR1_s           : std_logic_vector(31 downto 0);
  signal v32_RtdexReadCntR2_s           : std_logic_vector(31 downto 0);

  -------------------------------------------------------------------------------
  -- Attribute declarations
  -------------------------------------------------------------------------------

  attribute BUFFER_TYPE  : string;
  attribute BUFFER_TYPE of MemClk_s   : signal is "none";
  attribute MAX_FANOUT  of MemClk_s   : signal is "10000";
  
  attribute keep : string;
  attribute keep of MemClk_s : signal is "TRUE";
  attribute keep of CoreReset_s : signal is "TRUE";
  attribute keep of MemReset_s : signal is "TRUE";
  
  -- Keep these signals to prevent inferred SRL
  attribute keep of v32_RecordStorageCntR1_s : signal is "TRUE";
  attribute keep of v32_RtdexStorageCntR1_s : signal is "TRUE";
  attribute keep of v32_PlaybackReadCntR1_s : signal is "TRUE";
  attribute keep of v32_RtdexReadCntR1_s : signal is "TRUE";
  
  attribute keep of RecSoftTrigD1_s : signal is "TRUE";
  attribute keep of RecSoftTrigD2_s : signal is "TRUE";
  
  attribute keep of v2_RecTrigMuxD1_s : signal is "TRUE";
  attribute keep of v2_RecTrigMuxD2_s : signal is "TRUE";
  
  attribute keep of v2_PlayTrigMuxD1_s : signal is "TRUE";
  attribute keep of v2_PlayTrigMuxD2_s : signal is "TRUE";
  
  attribute keep of PlaySoftTrigD1_s : signal is "TRUE";
  attribute keep of PlaySoftTrigD2_s : signal is "TRUE";
  attribute keep of PlaySoftTrigD3_s : signal is "TRUE";
  attribute keep of PlaySoftTrigD4_s : signal is "TRUE";
  attribute keep of PlaySoftTrigD5_s : signal is "TRUE";

  attribute keep_hierarchy : string;
  attribute keep_hierarchy of rtl : architecture is "true";

begin


  o_RecFifoFull_p  <= RecFifoFull_s;
  o_MemClk_p       <= MemClk_s;

  -- Bring Fifo emppty flag to the Wr_clk (i_RecDataClk_p) clock domain
  process(i_RecDataClk_p)
  begin
    if rising_edge(i_RecDataClk_p) then
      o_RecFifoEmpty_p <= RecFifoEmpty_s;
    end if;
  end process;


  RecTrigMuxProc_l : process( i_RecDataClk_p )
  begin
    if( rising_edge( i_RecDataClk_p ) ) then
        v2_RecTrigMuxD1_s    <= v2_RecTrigMux_s;
        v2_RecTrigMuxD2_s    <= v2_RecTrigMuxD1_s;
        v2_RecTrigMuxD3_s    <= v2_RecTrigMuxD2_s;
        
        RecSoftTrigD1_s   <= RecSoftTrig_s;
        RecSoftTrigD2_s   <= RecSoftTrigD1_s;
        RecSoftTrigD3_s   <= RecSoftTrigD2_s;

        RecTriggerR1_s    <= i_RecTrigger_p;
        
        -- Delay RecordStarted by 2 clock cycles,
        -- the same delay than in acquisition_packer component
        RecordStartedR2_s   <= RecordStartedR1_s;
        RecordStartedR2_s   <= RecordStarted_s;

        -- Keep the trigger high until the next record
        -- in order to catch the rising edge even if 
        -- no writing was done during this clock cycle.
        if RecordStartedR2_s = '1' then
            if RecTriggerRe_s = '1' then
                RecTriggerReLatch_s <= '1';
            end if;
            if RecSoftTrigD3_s = '1' then
                RecSoftTrigLatch_s <= '1';
            end if;
        else
            RecTriggerReLatch_s <= '0';
            RecSoftTrigLatch_s <= '0';
        end if;
        
    end if;
  end process;
  
  RecTriggerRe_s <= i_RecTrigger_p and (not RecTriggerR1_s);

  RecTrigger_s <= (RecSoftTrigD3_s or RecSoftTrigLatch_s) when v2_RecTrigMuxD3_s = "01" 
    else (RecTriggerRe_s or RecTriggerReLatch_s)  when v2_RecTrigMuxD3_s = "00"
    else '1';

  PlayTrigMuxProc_l : process( i_PlayDataClk_p )
  begin
    if( rising_edge( i_PlayDataClk_p ) ) then
      v2_PlayTrigMuxD1_s  <= v2_PlayTrigMux_s;
      v2_PlayTrigMuxD2_s  <= v2_PlayTrigMuxD1_s;
      v2_PlayTrigMuxD3_s  <= v2_PlayTrigMuxD2_s;
      
      PlayTriggerInR1_s <= i_PlayTriggerIn_p;
      
      PlaySoftTrigD1_s <= PlaySoftTrig_s;
      PlaySoftTrigD2_s <= PlaySoftTrigD1_s;
      PlaySoftTrigD3_s <= PlaySoftTrigD2_s;
      PlaySoftTrigD4_s <= PlaySoftTrigD3_s;
      PlaySoftTrigD5_s <= PlaySoftTrigD4_s;
      PlaySoftTrigD6_s <= PlaySoftTrigD5_s;
    end if;
  end process;
  
  PlayTriggerInRe_s <= i_PlayTriggerIn_p and (not PlayTriggerInR1_s);

  PlayTrigger_s <= PlaySoftTrigD6_s when v2_PlayTrigMuxD3_s = "01" 
    else PlayTriggerInRe_s  when v2_PlayTrigMuxD3_s = "00"
    else '1';


  -------------------------------------------------------------------------------
  -- DDR3 controller instance
  -------------------------------------------------------------------------------
    v6Ddr3Controler64b_l : v6Ddr3Controler64b
      port map
      (
        sys_clk             => sys_clk,
        clk_ref             => clk_ref,

        ddr3_dq             => ddr3_dq,
        ddr3_addr           => ddr3_addr,
        ddr3_ba             => ddr3_ba,
        ddr3_ras_n          => ddr3_ras_n,
        ddr3_cas_n          => ddr3_cas_n,
        ddr3_we_n           => ddr3_we_n,
        ddr3_reset_n        => ddr3_reset_n,
        ddr3_cs_n           => ddr3_cs_n,
        ddr3_odt            => ddr3_odt,
        ddr3_cke            => ddr3_cke,
        ddr3_dm             => ddr3_dm,
        ddr3_dqs_p          => ddr3_dqs_p,
        ddr3_dqs_n          => ddr3_dqs_n,
        ddr3_ck_p           => ddr3_ck_p,
        ddr3_ck_n           => ddr3_ck_n,

        app_wdf_wren        => AppWdfWren_s,
        app_wdf_data        => v256_AppWdfData_s,
        app_wdf_end         => AppWdfEnd_s,
        app_wdf_mask        => x"00000000",
        app_addr            => v_TgAddr_s,
        app_cmd             => v3_AppCmd_s,
        app_en              => AppEn_s,
        app_rdy                    => app_rdy_s,
        app_wdf_rdy         => app_wdf_rdy_s,
        app_rd_data         => v256_AppRdData_s,
        app_rd_data_end        => open,
        app_rd_data_valid   => AppRdDataValid_s,

        iodelay_ctrl_rdy    => iodelay_ctrl_rdy,
        ui_clk_sync_rst     => Reset_s,
        ui_clk              => MemClk_s,
        phy_init_done       => PhyInitDone_s,
        sys_rst             => MemReset_s,
        sda                 => open,
        scl                 => open
      );

  -------------------------------------------------------------------------------
  -- Data packer and unpacker for Record/Playback ports
  -------------------------------------------------------------------------------
  AcqPack_l : entity lyt_axi_record_playback_v1_00_a.aquisition_packer
    generic map
    (
      PortWidth_g                 =>  RecordPortWidth_g,
      NumberOfPorts_g             =>  NumberOfRecordPorts_g
    )
    port map
    (
      i_WrClk_p                   => i_RecDataClk_p,
      i_RdClk_p                   => MemClk_s,
      i_ResetFifo_p               => RecordResetFifo_s,

      -- User interface
      i_Trigger_p                 => RecTrigger_s,
      iv_DataPort0_p              => iv_RecDataPort0_p,
      iv_DataPort1_p              => iv_RecDataPort1_p,
      iv_DataPort2_p              => iv_RecDataPort2_p,
      iv_DataPort3_p              => iv_RecDataPort3_p,
      iv_DataPort4_p              => iv_RecDataPort4_p,
      iv_DataPort5_p              => iv_RecDataPort5_p,
      iv_DataPort6_p              => iv_RecDataPort6_p,
      iv_DataPort7_p              => iv_RecDataPort7_p,
      iv_DataPort8_p              => iv_RecDataPort8_p,
      iv_DataPort9_p              => iv_RecDataPort9_p,
      iv_DataPort10_p             => iv_RecDataPort10_p,
      iv_DataPort11_p             => iv_RecDataPort11_p,
      iv_DataPort12_p             => iv_RecDataPort12_p,
      iv_DataPort13_p             => iv_RecDataPort13_p,
      iv_DataPort14_p             => iv_RecDataPort14_p,
      iv_DataPort15_p             => iv_RecDataPort15_p,
      i_WriteEn_p                 => i_RecWriteEn_p,
      o_FifoFull_p                => RecFifoFull_s,

      -- Ctrl
      i_RecordStarted_p           => RecordStarted_s,

      -- Interface to memory controller
      ov32_Trigger_p              => v32_Trigger_s,
      ov256_Data_p                => v256_RecData_s,
      o_DValid_p                  => RecDValid_s,
      i_ReadEn_p                  => RecReadData_s,
      o_FifoEmpty_p               => RecFifoEmpty_s
    );

  PlayUnpack_l : entity lyt_axi_record_playback_v1_00_a.playback_unpacker
    generic map
    (
      PortWidth_g                 => PlayBackPortWidth_g,
      NumberOfPorts_g             => NumberOfPlayBackPorts_g
    )
    port map
    (
      -- Interface to memory controller
      i_WrClk_p                   => MemClk_s,
      iv256_FifoWrData_p          => v256_PlayFifoWrData_s,
      i_WriteEn_p                 => PlayFifoWrite_s,
      i_ResetFifo_p               => PlayResetFifo_s,
      o_FifoProgFull_p            => PlayFifoProgFull_s,

      iv9_ProgFullThresAssert_p   => v9_ProgFullThresAssert_s,
      iv9_ProgFullThresNegate_p   => v9_ProgFullThresNegate_s,

      -- User interface
      i_RdClk_p                   =>  i_PlayDataClk_p,
      ov_DataPort0_p              =>  ov_PlayDataPort0_s,
      ov_DataPort1_p              =>  ov_PlayDataPort1_s,
      ov_DataPort2_p              =>  ov_PlayDataPort2_p,
      ov_DataPort3_p              =>  ov_PlayDataPort3_p,
      ov_DataPort4_p              =>  ov_PlayDataPort4_p,
      ov_DataPort5_p              =>  ov_PlayDataPort5_p,
      ov_DataPort6_p              =>  ov_PlayDataPort6_p,
      ov_DataPort7_p              =>  ov_PlayDataPort7_p,
      ov_DataPort8_p              =>  ov_PlayDataPort8_p,
      ov_DataPort9_p              =>  ov_PlayDataPort9_p,
      ov_DataPort10_p             =>  ov_PlayDataPort10_p,
      ov_DataPort11_p             =>  ov_PlayDataPort11_p,
      ov_DataPort12_p             =>  ov_PlayDataPort12_p,
      ov_DataPort13_p             =>  ov_PlayDataPort13_p,
      ov_DataPort14_p             =>  ov_PlayDataPort14_p,
      ov_DataPort15_p             =>  ov_PlayDataPort15_p,
      o_Valid_p                   =>  o_PlayValid_s,
      o_Empty_p                   =>  o_PlayEmpty_s,
      i_ReadEn_p                  =>  i_PlayReadEn_p
    );

  -------------------------------------------------------------------------------
  -- DDR3 access manager
  -------------------------------------------------------------------------------
  v_StartAddress_s  <= v32_StartAddress_s(v_StartAddress_s'high downto 0);
  v_TrigDly_s       <= v32_TrigDly_s(v_TrigDly_s'high downto 0);
  v_TransferSize_s  <= v32_TransferSize_s(v_TransferSize_s'high downto 0);

  v32_TrigAddr_s    <= std_logic_vector(resize(unsigned(v_TrigAddr_s),32));

  RecPlayDispatcher_l : entity lyt_axi_record_playback_v1_00_a.rec_play_dispatch_ddr3
  generic map
  (
    ADDR_WIDTH                    => ADDR_WIDTH
  )
  port map
  (
    -- Reset And Clocks
    i_Reset_p  => CoreReset_s,
    i_SystemClk_p => S_AXI_ACLK,

    -- Ctrl Registers
    i_SetMode_p                   => SetMode_s,
    iv3_Mode_p                    => v3_ModeValue_s,
    iv_StartAddress_p             => v_StartAddress_s,
    iv_TrigDly_p                  => v_TrigDly_s,
    iv_TransferSize_p             => v_TransferSize_s,
    ov_TrigAddr_p                 => v_TrigAddr_s,
    ov32_TrigAddrIndex_p          => v32_TrigAddrIndex_s,
    o_TransferOver_p              => TransferOver_s,
    o_ParityAddrReg_p             => ParityAddrReg_s,
    o_RecordStarted_p             => RecordStarted_s,
    ov32_RecordStorageCnt_p       => v32_RecordStorageCnt_s,
    ov32_RtdexStorageCnt_p        => v32_RtdexStorageCnt_s,
    ov32_PlaybackReadCnt_p        => v32_PlaybackReadCnt_s,
    ov32_RtdexReadCnt_p           => v32_RtdexReadCnt_s,

    -- Record Fifo Interface
    o_RecordResetFifo_p           => RecordResetFifo_s,
    o_RtdexRxStarted_p            => RtdexRxStarted_s,
    iv32_RecTriggerIn_p           => v32_Trigger_s,
    iv256_RecData_p               => v256_RecData_s,
    o_RecReadData_p               => RecReadData_s,
    i_RecDValid_p                          => RecDValid_s,
    i_RecFifoEmpty_p                    => RecFifoEmpty_s,

    -- RTDEx HOST 2 FPGA Fifo Interface
    o_RtdexRxResetFifo_p          => RtdexRxResetFifo_s,
    iv256_RtdexRxData_p           => v256_RtdexRxData_s,
    i_RtdexDValid_p                         => RtdexDValid_s,
    o_RtdexRxReadData_p           => RtdexRxReadData_s,
    i_RtdexFifoEmpty_p                   => RtdexFifoEmpty_s,

    -- Play back Fifo Interface
    i_PlayTriggerIn_p             => PlayTrigger_s,
    ov256_PlayFifoWrData_p        => v256_PlayFifoWrData_s,
    o_PlayFifoWrite_p             => PlayFifoWrite_s,
    o_PlayResetFifo_p             => PlayResetFifo_s,
    i_PlayFifoProgFull_p          => PlayFifoProgFull_s,

    -- RTDEx FPGA 2 HOST Fifo Interface
    ov256_RtdexTxFifoWrData_p     => v256_RtdexTxFifoWrData_s,
    o_RtdexTxFifoWrite_p          => RtdexTxFifoWrite_s,
    o_RtdexTxResetFifo_p          => RtdexTxResetFifo_s,
    i_RtdexTxFifoProgFull_p       => RtdexTxFifoProgFull_s,

    -- Memory Interface
    i_MemClk_p                    => MemClk_s,
    i_PhyInitDone_p               => PhyInitDone_s,
    i_AppRdy_p                    => app_rdy_s,
    o_AppEn_p                     => AppEn_s,
    ov3_AppCmd_p                  => v3_AppCmd_s,
    ov_TgAddr_p                   => v_TgAddr_s,
    iv256_AppRdData_p             => v256_AppRdData_s,
    i_AppRdDataValid_p            => AppRdDataValid_s,
    i_AppWdfRdy_p                 => app_wdf_rdy_s,
    o_AppWdfWren_p                => AppWdfWren_s,
    ov256_AppWdfData_p            => v256_AppWdfData_s,
    o_AppWdfEnd_p                 => AppWdfEnd_s
  );


  -------------------------------------------------------------------------------
  -- DDR3-RTDEX interface
  -------------------------------------------------------------------------------
  RTDExRx_l : entity lyt_axi_record_playback_v1_00_a.rtdex_rx_fifo_interface
    generic map
    (
      PlayBackPortWidth_g     => PlayBackPortWidth_g,
      NumberOfPlayBackPorts_g => NumberOfPlayBackPorts_g
    )
    port map
    (
      -- Mem writer controler interface
      i_ResetFifo_p           => RtdexRxResetFifo_s,
      i_MemClk_p              => MemClk_s,
      ov256_Data_p            => v256_RtdexRxData_s,
      o_DValid_p              => RtdexDValid_s,
      i_ReadData_p            => RtdexRxReadData_s,
      o_FifoEmpty_p           => RtdexFifoEmpty_s,
      
      i_RtdexRxStarted_p      => RtdexRxStarted_s,

      -- RTDEx interface
      i_RTDExClock_p          => i_RTDExRxClock_p,
      i_RxDataValid_p         => i_RxDataValid_p,
      o_RxRe_p                => o_RxRe_p,
      i_RxReady_p             => i_RxReady_p,
      iv32_RxDataCh0_p        => iv32_RxDataCh0_p
    );


  RTDExTx_l : entity lyt_axi_record_playback_v1_00_a.rtdex_tx_fifo_interface
    generic map
    (
      RecordPortWidth_g          => RecordPortWidth_g,
      NumberOfRecordPorts_g   => NumberOfRecordPorts_g
    )
    port map
    (
      -- Mem reader controler interface
      i_MemClk_p              => MemClk_s,
      iv256_FifoWrData_p      => v256_RtdexTxFifoWrData_s,
      i_FifoWrite_p           => RtdexTxFifoWrite_s,
      i_ResetFifo_p           => RtdexTxResetFifo_s,
      o_FifoProgFull_p        => RtdexTxFifoProgFull_s,

      -- RTDEx interface
      i_RTDExClock_p          => i_RTDExTxClock_p,
      o_TxWe_p                => o_TxWe_p,
      i_TxReady_p             => i_TxReady_p,
      ov32_TxDataCh0_p        => ov32_TxDataCh0_p
    );
    
  -------------------------------------------------------------------------------
  -- Latch input AXI registers for metastability and to ease timing
  -------------------------------------------------------------------------------
  process(S_AXI_ACLK)
  begin
    if rising_edge(S_AXI_ACLK) then
      v32_RecordStorageCntR1_s  <= v32_RecordStorageCnt_s;
      v32_RtdexStorageCntR1_s   <= v32_RtdexStorageCnt_s;
      v32_PlaybackReadCntR1_s   <= v32_PlaybackReadCnt_s;
      v32_RtdexReadCntR1_s      <= v32_RtdexReadCnt_s;
      
      v32_RecordStorageCntR2_s  <= v32_RecordStorageCntR1_s;
      v32_RtdexStorageCntR2_s   <= v32_RtdexStorageCntR1_s;
      v32_PlaybackReadCntR2_s   <= v32_PlaybackReadCntR1_s;
      v32_RtdexReadCntR2_s      <= v32_RtdexReadCntR1_s;
    end if;
  end process;

  -------------------------------------------------------------------------------
  -- AXI decoder and user registers
  -------------------------------------------------------------------------------
  AXILiteReg_l : entity lyt_axi_record_playback_v1_00_a.axi_record_playback
    generic map
    (
      -- ADD USER GENERICS BELOW THIS LINE ---------------
      --USER generics added here
      -- ADD USER GENERICS ABOVE THIS LINE ---------------

      -- DO NOT EDIT BELOW THIS LINE ---------------------
      -- Bus protocol parameters, do not add to or delete
      C_S_AXI_DATA_WIDTH   => C_S_AXI_DATA_WIDTH ,
      C_S_AXI_ADDR_WIDTH   => C_S_AXI_ADDR_WIDTH ,
      C_S_AXI_MIN_SIZE     => C_S_AXI_MIN_SIZE   ,
      C_USE_WSTRB          => C_USE_WSTRB        ,
      C_DPHASE_TIMEOUT     => C_DPHASE_TIMEOUT   ,
      C_BASEADDR           => C_BASEADDR         ,
      C_HIGHADDR           => C_HIGHADDR         ,
      C_FAMILY             => C_FAMILY
      -- DO NOT EDIT ABOVE THIS LINE ---------------------
    )
    port map
    (
      -- User ports
      i_logicRst_p              => CoreReset_s,
      o_CoreReset_p         => CoreResetPulse_s,
      o_MemReset_p          => MemReset_s,
      i_PhyInitDone_p       => PhyInitDone_s,

      o_SetMode_p           => SetMode_s,
      ov3_ModeValue_p       => v3_ModeValue_s,
      o_SetStartAddress_p   => open,
      ov32_StartAddress_p   => v32_StartAddress_s,
      o_SetTransferSize_p   => open,
      ov32_TrigDly_p        => v32_TrigDly_s,
      ov32_TransferSize_p   => v32_TransferSize_s,
      iv32_TrigAddr_p       => v32_TrigAddr_s,
      iv32_TrigAddrIndex_p  => v32_TrigAddrIndex_s,
      i_TransferOver_p      => TransferOver_s,

      i_UnderRunFlag_p          => '0',
      i_OverRunFlag_p           => RecFifoFull_s,
      ov9_ProgFullThresAssert_p => v9_ProgFullThresAssert_s,
      ov9_ProgFullThresNegate_p => v9_ProgFullThresNegate_s,
      i_ParityAddrReg_p         => ParityAddrReg_s,

      iv32_NbRecordPorts_p       => v32_NbRecordPorts_c     ,
      iv32_NbPlaybackPorts_p     => v32_NbPlaybackPorts_c   ,
      iv32_RecordPortsWidth_p    => v32_RecordPortsWidth_c  ,
      iv32_PlaybackPortsWidth_p  => v32_PlaybackPortsWidth_c,

      ov2_RecTrigMux_p  => v2_RecTrigMux_s,
      o_RecSoftTrig_p => RecSoftTrig_s,

      ov2_PlayTrigMux_p  => v2_PlayTrigMux_s,
      o_PlaySoftTrig_p => PlaySoftTrig_s,
      
      iv32_RecordStorageCnt_p       => v32_RecordStorageCntR2_s,
      iv32_RtdexStorageCnt_p        => v32_RtdexStorageCntR2_s,
      iv32_PlaybackReadCnt_p        => v32_PlaybackReadCntR2_s,
      iv32_RtdexReadCnt_p           => v32_RtdexReadCntR2_s,

      -- DO NOT EDIT BELOW THIS LINE ---------------------
      -- Bus protocol ports, do not add to or delete
      S_AXI_ACLK                   =>  S_AXI_ACLK    ,
      S_AXI_ARESETN                =>  S_AXI_ARESETN ,
      S_AXI_AWADDR                 =>  S_AXI_AWADDR  ,
      S_AXI_AWVALID                =>  S_AXI_AWVALID ,
      S_AXI_WDATA                  =>  S_AXI_WDATA   ,
      S_AXI_WSTRB                  =>  S_AXI_WSTRB   ,
      S_AXI_WVALID                 =>  S_AXI_WVALID  ,
      S_AXI_BREADY                 =>  S_AXI_BREADY  ,
      S_AXI_ARADDR                 =>  S_AXI_ARADDR  ,
      S_AXI_ARVALID                =>  S_AXI_ARVALID ,
      S_AXI_RREADY                 =>  S_AXI_RREADY  ,
      S_AXI_ARREADY                =>  S_AXI_ARREADY ,
      S_AXI_RDATA                  =>  S_AXI_RDATA   ,
      S_AXI_RRESP                  =>  S_AXI_RRESP   ,
      S_AXI_RVALID                 =>  S_AXI_RVALID  ,
      S_AXI_WREADY                 =>  S_AXI_WREADY  ,
      S_AXI_BRESP                  =>  S_AXI_BRESP   ,
      S_AXI_BVALID                 =>  S_AXI_BVALID  ,
      S_AXI_AWREADY                =>  S_AXI_AWREADY
    );


  --axi_areset_s <= not S_AXI_ARESETN;

  --------------------------------------------
  -- SW reset pulse stretcher.
  --------------------------------------------
   Process(S_AXI_ACLK)
   begin
       if rising_edge(S_AXI_ACLK) then
      CoreReset_s <= or_reduce(v8_SignalStretch_s);
           v8_SignalStretch_s <= v8_SignalStretch_s(6 downto 0) & CoreResetPulse_s;
       end if;
   end process;
  

  ov_PlayDataPort0_p  <= ov_PlayDataPort0_s;
  ov_PlayDataPort1_p  <= ov_PlayDataPort1_s;
  o_PlayValid_p       <= o_PlayValid_s;
  o_PlayEmpty_p       <= o_PlayEmpty_s;

end rtl;

