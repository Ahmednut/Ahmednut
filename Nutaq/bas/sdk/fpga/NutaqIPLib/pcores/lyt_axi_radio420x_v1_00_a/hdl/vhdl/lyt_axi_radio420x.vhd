--------------------------------------------------------------------------------
--
--    ****                              *
--   ******                            ***
--   *******                           ****
--   ********    ****  ****     **** *********    ******* ****    ***********
--   *********   ****  ****     **** *********  **************  *************
--   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
--   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
--  ****    *********  ****     ****   ****   ****       ****  ****      ****
--  ****     ********  ****    *****  ****    *****     *****  ****      ****
--  ****      ******   ***** ******   *****    ****** *******  ****** *******
--  ****        ****   ************    ******   *************   *************
--  ****         ***     ****  ****     ****      *****  ****     *****  ****
--                                                                       ****
--          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
--                                                                        ***
--
--------------------------------------------------------------------------------
-- Description : This component includes Radio420x component and
--              its AXI interface
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------

library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;
 use ieee.std_logic_misc.all;

library lyt_axi_radio420x_v1_00_a;
 use lyt_axi_radio420x_v1_00_a.all;

entity lyt_axi_radio420x is
  generic
  (
    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_S_AXI_DATA_WIDTH             : integer              := 32;
    C_S_AXI_ADDR_WIDTH             : integer              := 32;
    C_S_AXI_MIN_SIZE               : std_logic_vector     := X"000001FF";
    C_USE_WSTRB                    : integer              := 0;
    C_DPHASE_TIMEOUT               : integer              := 8;
    C_BASEADDR                     : std_logic_vector     := X"FFFFFFFF";
    C_HIGHADDR                     : std_logic_vector     := X"00000000";
    C_FAMILY                       : string               := "virtex6";
    C_FMCRADIO_CH2                 : integer range 0 to 1:=0
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    --FMC mapping
    iv12_fmcLA01LA06_AdcData_p     : in std_logic_vector(11 downto 0);
    i_fmcLA00N_AdcIQSel_p          : in std_logic;
    i_fmcLA00P_AdcClk_p            : in std_logic;
    ov12_fmcLA18LA23_DacData_p     : out std_logic_vector(11 downto 0);
    o_fmcLA17N_DacIQSel_p          : out std_logic;
    o_fmcLA07N_LimeSpiCs_p         : out std_logic;
    o_fmcLA07P_LimeSpiClk_p        : out std_logic;
    o_fmcLA08N_LimeSpiMosi_p       : out std_logic;
    i_fmcLA08P_LimeSpiMiso_p       : in  std_logic;

    o_fmcLA28P_RefDacSpiCs_p       : out std_logic;
    o_fmcLA27N_RefDacSpiClk_p      : out std_logic;
    o_fmcLA27P_RefDacSpiMosi_p     : out std_logic;

    ov2_fmcLA16P_PllCtrlSpiCs_p    : out std_logic_vector(1 downto 0);
    o_fmcLA16N_PllCtrlSpiClk_p     : out std_logic;
    o_fmcLA15N_PllCtrlSpiMosi_p    : out std_logic;
    i_fmcLA15P_PllCtrlSpiMiso_p    : in  std_logic;

    o_fmcLA31N_RxGainSpiCs_p       : out std_logic;
    o_fmcLA30P_RxGainSpiClk_p      : out std_logic;
    o_fmcLA30N_RxGainSpiMosi_p     : out std_logic;

    o_fmcLA31P_TxGainSpiCs_p       : out std_logic;
    o_fmcLA32N_TxGainSpiClk_p      : out std_logic;
    o_fmcLA32P_TxGainSpiMosi_p     : out std_logic;

    i_fmcLA25N_PllLock_p           : in std_logic;
    o_fmcLA10P_LimeReset_p         : out std_logic;
    o_fmcLA09N_LimeTxEn_p          : out std_logic;
    o_fmcLA09P_LimeRxEn_p          : out std_logic;
    ov2_fmcLA12_ClkMuxSin_p        : out std_logic_vector(1 downto 0);
    ov2_fmcLA11_ClkMuxSout_p       : out std_logic_vector(1 downto 0);
    o_fmcLA13N_ClkMuxLoad_p        : out std_logic;
    o_fmcLA13P_ClkMuxConfig_p      : out std_logic;

    i_fmcLA14P_Pps_p               : in std_logic;

    --User Signal mapping
    i_PeriphReset_p                : in std_logic;
    i_refClk200MHz_p               : in std_logic;

    o_designClk_p                  : out std_logic;
    i_designClk_p                  : in  std_logic;
    ov12_adcIData_p                : out std_logic_vector (11 downto 0);
    ov12_adcQData_p                : out std_logic_vector (11 downto 0);
    o_adcDataValid_p               : out std_logic;
	i_dacDataValid_p               : in  std_logic;
    iv12_dacIData_p                : in  std_logic_vector (11 downto 0);
    iv12_dacQData_p                : in  std_logic_vector (11 downto 0);
    o_dacReadEnable_p              : out std_logic;
    ov12_dacDataComp_p             : out std_logic_vector (11 downto 0);
    o_dacIQSelComp_p               : out std_logic;

    -- From/to user external control
    iv16_refDacSpiData_p           : in  std_logic_vector(15 downto 0);
    i_refDacSpiStart_p             : in  std_logic;
    o_refDacSpiBusy_p              : out std_logic;
    o_refDacSpiExtCtrl_p           : out std_logic;

    iv16_limeSpiData_p             : in  std_logic_vector(15 downto 0);
    i_limeSpiStart_p               : in std_logic;
    o_limeSpiBusy_p                : out std_logic;
    o_limeSpiExtCtrl_p             : out std_logic;

    iv6_rxGainSpiData_p            : in std_logic_vector(5 downto 0);
    i_rxGainSpiStart_p             : in std_logic;
    o_rxGainSpiBusy_p              : out std_logic;
    o_rxGainSpiExtCtrl_p           : out std_logic;

    iv6_txGainSpiData_p            : in std_logic_vector(5 downto 0);
    i_txGainSpiStart_p             : in std_logic;
    o_txGainSpiBusy_p              : out std_logic;
    o_txGainSpiExtCtrl_p           : out std_logic;

    iv32_pllCtrlSpiData_p          : in  std_logic_vector(31 downto 0);
    iv2_pllCtrlSpiStart_p          : in  std_logic_vector(1 downto 0);
    o_pllCtrlSpiBusy_p             : out std_logic;
    o_pllSpiExtCtrl_p              : out std_logic;
    
    o_PPsOut_p					   : out std_logic;

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
    -- DO NOT EDIT ABOVE THIS LINE --
  );

 attribute MAX_FANOUT                     : string;
 attribute SIGIS                          : string;
 attribute MAX_FANOUT of S_AXI_ACLK       : signal is "10000";
 attribute MAX_FANOUT of S_AXI_ARESETN    : signal is "10000";
 attribute SIGIS of S_AXI_ACLK            : signal is "Clk";
 attribute SIGIS of S_AXI_ARESETN         : signal is "Rst";

end entity lyt_axi_radio420x;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture arch of lyt_axi_radio420x is

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal TxEnable_s                  : std_logic;
  signal RxEnable_s                  : std_logic;
  signal v2_ClkMuxSout_s             : std_logic_vector(1 downto 0);
  signal ClkMuxLoad_s                : std_logic;
  signal ClkMuxConfig_s              : std_logic;
  signal PLLLock_s                   : std_logic;
  signal ovrAdcI_s                   : std_logic;
  signal ovrAdcQ_s                   : std_logic;
  signal ovrDacI_s                   : std_logic;
  signal ovrDacQ_s                   : std_logic;
  signal fmcPps_s                    : std_logic;
  signal LimeReset_s                 : std_logic;
  signal ResetOvr_s                  : std_logic;
  signal DesignClkEn_s               : std_logic;
  signal CoreResetPulse_s            : std_logic;
  signal RstFifo_s                   : std_logic;
  signal v5_FpgaControl_s            : std_logic_vector(4 downto 0);
  signal v2_ClkMuxSin_s              : std_logic_vector(1 downto 0);
  signal v24_ddsfreq_s               : std_logic_vector(23 downto 0);
  signal v3_dacOutSel_s              : std_logic_vector(2 downto 0);
  signal v16_limeSpiDataIn_s         : std_logic_vector(15 downto 0);
  signal v16_limeSpiDataOut_s        : std_logic_vector(15 downto 0);
  signal v6_txGainSpiData_s          : std_logic_vector(5 downto 0);
  signal v6_rxGainSpiData_s          : std_logic_vector(5 downto 0);
  signal v32_pllCtrlSpiDataIn_s      : std_logic_vector(31 downto 0);
  signal v32_pllCtrlSpiDataOut_s     : std_logic_vector(31 downto 0);
  signal v16_refDacSpiData_s         : std_logic_vector(15 downto 0);
  signal limeSpiStart_s              : std_logic;
  signal limeSpiBusy_s               : std_logic;
  signal pllCtrlSpiBusy_s            : std_logic;
  signal refDacSpiStart_s            : std_logic;
  signal refDacSpiBusy_s             : std_logic;
  signal txGainSpiStart_s            : std_logic;
  signal txGainSpiBusy_s             : std_logic;
  signal v3_RxGainSpiStart_s         : std_logic_vector(2 downto 0);
  signal rxGainSpiBusy_s             : std_logic;
  signal v2_pllCtrlSpiStart_s        : std_logic_vector(1 downto 0);
  signal v13_IQGainImb_s             : std_logic_vector(12 downto 0);
  signal v13_IQPhaseImbCos_s         : std_logic_vector(12 downto 0);
  signal v13_IQPhaseImbSin_s         : std_logic_vector(12 downto 0);
  signal v13_IQImbBackOff_s          : std_logic_vector(12 downto 0);
  signal v12_RxIDcVal_s              : std_logic_vector(11 downto 0);
  signal v12_RxQDcVal_s              : std_logic_vector(11 downto 0);
  signal v23_ERF_s                   : std_logic_vector(22 downto 0);
  signal RXERRFCT_START_s            : std_logic;
  signal RXERRFCT_DONE_s             : std_logic;
  signal RXERRFCT_FREQ_WEN_s         : std_logic;
  signal v14_RXERRFCT_NB_POINT_s     : std_logic_vector(13 downto 0);
  signal v24_RXERRFCT_FREQ_s         : std_logic_vector(23 downto 0);
  signal v32_RXERRFCT_ERR_I_s        : std_logic_vector(31 downto 0);
  signal v32_RXERRFCT_ERR_Q_s        : std_logic_vector(31 downto 0);
  signal v5_AdcIdelayValue_s         : std_logic_vector(4 downto 0);
  signal v5_AdcClkIdelayValue_s      : std_logic_vector(4 downto 0);
  
  -- Reset signals
  signal v8_SignalStretch_s		     : std_logic_vector(7 downto 0);
  signal CoreReset_s				 : std_logic;

  signal designClk_s                 : std_logic;
  
  signal PPsOutR1_s                 : std_logic;
  signal fmcPpsR1_s                 : std_logic;

begin

  ------------------------------------------
  -- Output ports
  ------------------------------------------
  o_txGainSpiBusy_p <= txGainSpiBusy_s;

  o_pllCtrlSpiBusy_p <= pllCtrlSpiBusy_s;

  o_rxGainSpiBusy_p <= rxGainSpiBusy_s;

  o_refDacSpiBusy_p <= refDacSpiBusy_s;
  
  o_limeSpiBusy_p <= limeSpiBusy_s;

  o_refDacSpiExtCtrl_p  <= v5_FpgaControl_s(0);
  o_limeSpiExtCtrl_p    <= v5_FpgaControl_s(1);
  o_rxGainSpiExtCtrl_p  <= v5_FpgaControl_s(2);
  o_txGainSpiExtCtrl_p  <= v5_FpgaControl_s(3);
  o_pllSpiExtCtrl_p     <= v5_FpgaControl_s(4);
  
  
  
  
  o_designClk_p <= designClk_s;

  ------------------------------------------
  -- Input ports
  ------------------------------------------
      
  -- Double latches to avoid metastability
  process(designClk_s)
  begin
    if rising_edge(designClk_s) then
      PPsOutR1_s <= i_fmcLA14P_Pps_p;
      o_PPsOut_p <= PPsOutR1_s;
      
      fmcPpsR1_s <= i_fmcLA14P_Pps_p;
      fmcPps_s 	 <= fmcPpsR1_s;
      
    end if;
  end process;

  ------------------------------------------
  -- radio420x_top instance
  ------------------------------------------
  radio420x_top_u0 : entity lyt_axi_radio420x_v1_00_a.radio420x_top
    generic map (
      C_FMCRADIO_CH2 => C_FMCRADIO_CH2
    )
    port map(
      -- Global signals
      i_systemClk_p     		=> S_AXI_ACLK,
      i_RefClk200MHz_p  		=> i_RefClk200MHz_p,
      i_rst_p           		=> CoreReset_s,
      i_RstFifo_p               => RstFifo_s,

      -- FMC signals
      iv12_fmcAdcData_p   	=> iv12_fmcLA01LA06_AdcData_p,
      i_fmcAdcIQSel_p     	=> i_fmcLA00N_AdcIQSel_p,
      i_fmcAdcClk_p       	=> i_fmcLA00P_AdcClk_p,
      ov12_fmcDacData_p   	=> ov12_fmcLA18LA23_DacData_p,
      o_fmcDacIQSel_p     	=> o_fmcLA17N_DacIQSel_p,
      -- SPI signals
      o_fmcLimeSpiCs_p    	=> o_fmcLA07N_LimeSpiCs_p,
      o_fmcLimeSpiClk_p   	=> o_fmcLA07P_LimeSpiClk_p,
      o_fmcLimeSpiMosi_p  	=> o_fmcLA08N_LimeSpiMosi_p,
      i_fmcLimeSpiMiso_p  	=> i_fmcLA08P_LimeSpiMiso_p,

      o_fmcRefDacSpiCs_p     	=> o_fmcLA28P_RefDacSpiCs_p,
      o_fmcRefDacSpiClk_p    	=> o_fmcLA27N_RefDacSpiClk_p,
      o_fmcRefDacSpiMosi_p   	=> o_fmcLA27P_RefDacSpiMosi_p,

      ov2_fmcPllCtrlSpiCs_p  	=> ov2_fmcLA16P_PllCtrlSpiCs_p,
      o_fmcPllCtrlSpiClk_p   	=> o_fmcLA16N_PllCtrlSpiClk_p,
      o_fmcPllCtrlSpiMosi_p  	=> o_fmcLA15N_PllCtrlSpiMosi_p,
      i_fmcPllCtrlSpiMiso_p  	=> i_fmcLA15P_PllCtrlSpiMiso_p,

      ov3_fmcRxGainSpiCs_p(0)	=> o_fmcLA31N_RxGainSpiCs_p,
      ov3_fmcRxGainSpiCs_p(1)	=> open,
      ov3_fmcRxGainSpiCs_p(2)	=> open,
      o_fmcRxGainSpiClk_p    	=> o_fmcLA30P_RxGainSpiClk_p,
      o_fmcRxGainSpiMosi_p   	=> o_fmcLA30N_RxGainSpiMosi_p,

      o_fmcTxGainSpiCs_p     	=> o_fmcLA31P_TxGainSpiCs_p,
      o_fmcTxGainSpiClk_p    	=> o_fmcLA32N_TxGainSpiClk_p,
      o_fmcTxGainSpiMosi_p   	=> o_fmcLA32P_TxGainSpiMosi_p,

      i_fmcPllLock_p         	=> i_fmcLA25N_PllLock_p,
      o_fmcLimeReset_p       	=> o_fmcLA10P_LimeReset_p,
      o_fmcTxEnable_p        	=> o_fmcLA09N_LimeTxEn_p,
      o_fmcRxEnable_p        	=> o_fmcLA09P_LimeRxEn_p,
      ov2_fmcClkMuxSin_p     	=> ov2_fmcLA12_ClkMuxSin_p,
      ov2_fmcClkMuxSout_p    	=> ov2_fmcLA11_ClkMuxSout_p,
      o_fmcClkMuxLoad_p      	=> o_fmcLA13N_ClkMuxLoad_p,
      o_fmcClkMuxConfig_p    	=> o_fmcLA13P_ClkMuxConfig_p,

      --To user logic
      i_designClk_p             => i_designClk_p,
      o_designClk_p       		=> designClk_s,    
      ov12_adcIData_p           => ov12_adcIData_p,    
      ov12_adcQData_p           => ov12_adcQData_p,    
      o_adcDataValid_p          => o_adcDataValid_p,   
	  i_dacDataValid_p          => i_dacDataValid_p,
      iv12_dacIData_p           => iv12_dacIData_p,    
      iv12_dacQData_p           => iv12_dacQData_p,    
      o_dacReadEnable_p         => o_dacReadEnable_p,  
      ov12_dacDataComp_p        => ov12_dacDataComp_p, 
      o_dacIQSelComp_p          => o_dacIQSelComp_p,   

      -- From/to control
      i_DesignClkEn_p           => DesignClkEn_s,
      iv5_AdcIdelayValue_p      => v5_AdcIdelayValue_s,
      iv5_AdcClkIdelayValue_p   => v5_AdcClkIdelayValue_s,
      iv5_fpgaCtrlEn_p          => v5_FpgaControl_s,
      iv16_refDacSpiData_p      => iv16_refDacSpiData_p,
      i_refDacSpiStart_p        => i_refDacSpiStart_p,
      iv16_HostRefDacSpiData_p  => v16_refDacSpiData_s,
      i_HostRefDacSpiStart_p    => refDacSpiStart_s,
      o_refDacSpiBusy_p         => refDacSpiBusy_s,

      iv16_limeSpiData_p        => iv16_limeSpiData_p,
      i_limeSpiStart_p          => i_limeSpiStart_p,
      iv16_HostLimeSpiData_p    => v16_limeSpiDataIn_s,
      ov16_HostLimeSpiData_p    => v16_limeSpiDataOut_s,
      i_HostLimeSpiStart_p      => limeSpiStart_s,
      o_limeSpiBusy_p           => limeSpiBusy_s,

      iv6_rxGainSpiData_p       => iv6_rxGainSpiData_p,
      iv3_rxGainSpiStart_p      => ("00" & i_rxGainSpiStart_p),
      iv6_HostRxGainSpiData_p   => v6_rxGainSpiData_s,
      iv3_HostRxGainSpiStart_p  => v3_RxGainSpiStart_s,
      o_rxGainSpiBusy_p         => rxGainSpiBusy_s,

      iv6_txGainSpiData_p       => iv6_txGainSpiData_p,
      i_txGainSpiStart_p        => i_txGainSpiStart_p,
      iv6_HostTxGainSpiData_p   => v6_txGainSpiData_s,
      i_HostTxGainSpiStart_p    => txGainSpiStart_s,
      o_txGainSpiBusy_p         => txGainSpiBusy_s,

      iv32_pllCtrlSpiData_p     => iv32_pllCtrlSpiData_p,
      iv2_pllCtrlSpiStart_p     => iv2_pllCtrlSpiStart_p,
      iv32_HostPllCtrlSpiData_p => v32_pllCtrlSpiDataIn_s,
      ov32_HostPllCtrlSpiData_p => v32_pllCtrlSpiDataOut_s,
      iv2_HostPllCtrlSpiStart_p => v2_pllCtrlSpiStart_s,
      o_pllCtrlSpiBusy_p        => pllCtrlSpiBusy_s,

      o_fmcPllLock_p            => pllLock_s,
      i_fmcLimeReset_p          => LimeReset_s,
      i_fmcTxEnable_p           => TxEnable_s,
      i_fmcRxEnable_p           => RxEnable_s,
      iv2_fmcClkMuxSin_p        => v2_ClkMuxSin_s,
      iv2_fmcClkMuxSout_p       => v2_ClkMuxSout_s,
      i_fmcClkMuxLoad_p         => ClkMuxLoad_s,
      i_fmcClkMuxConfig_p       => ClkMuxConfig_s,

      o_ovrAdcI_p               => ovrAdcI_s,
      o_ovrAdcQ_p               => ovrAdcQ_s,
      o_ovrDacI_p               => ovrDacI_s,
      o_ovrDacQ_p               => ovrDacQ_s,
      i_ResetOvr_p              => ResetOvr_s,

      ov12_rxIDcLevel_p         => v12_RxIDcVal_s,
      ov12_rxQDcLevel_p         => v12_RxQDcVal_s,
      ov23_rxErf_p              => v23_ERF_s,
      i_rxErrFctStart_p         => RXERRFCT_START_s,
      i_rxErrFctFreqWen_p       => RXERRFCT_FREQ_WEN_s,
      o_rxErrFctDone_p          => RXERRFCT_DONE_s,
      iv14_rxErrFctNbPt_p       => v14_RXERRFCT_NB_POINT_s,
      iv24_rxErrFctFreq_p       => v24_RXERRFCT_FREQ_s,
      ov32_rxErrFctErrI_p       => v32_RXERRFCT_ERR_I_s,
      ov32_rxErrFctErrQ_p       => v32_RXERRFCT_ERR_Q_s,

      iv13_phaseoffsetcos_p     => v13_IQPhaseImbCos_s,
      iv13_phaseoffsetsin_p     => v13_IQPhaseImbSin_s,
      iv13_backoff_p            => v13_IQImbBackOff_s,
      iv13_gainoffset_p         => v13_IQGainImb_s,
      iv24_dacOutFreq_p         => v24_ddsfreq_s,
      iv3_dacOutSel_p           => v3_dacOutSel_s
 		);

  --------------------------------------------
  -- AXI Memory Mapped User Logic
  --------------------------------------------
  USER_LOGIC_I : entity lyt_axi_radio420x_v1_00_a.axi_radio420x
    generic map
    (
      -- MAP USER GENERICS BELOW THIS LINE ---------------
      --USER generics mapped here
      -- MAP USER GENERICS ABOVE THIS LINE ---------------
      C_S_AXI_DATA_WIDTH        => C_S_AXI_DATA_WIDTH,
      C_S_AXI_ADDR_WIDTH        => C_S_AXI_ADDR_WIDTH,
      C_S_AXI_MIN_SIZE          => C_S_AXI_MIN_SIZE  ,
      C_USE_WSTRB               => C_USE_WSTRB       ,
      C_DPHASE_TIMEOUT          => C_DPHASE_TIMEOUT  ,
      C_BASEADDR                => C_BASEADDR        ,
      C_HIGHADDR                => C_HIGHADDR        ,
      C_FAMILY                  => C_FAMILY
    )
    port map
    (
      -- user_logic entity ports mapping  ---------------
      i_CoreReset_p                  => CoreReset_s,
      o_TxEnable_p                   => TxEnable_s,
      o_RxEnable_p                   => RxEnable_s,
      ov2_ClkMuxSout_p               => v2_ClkMuxSout_s,
      o_ClkMuxLoad_p                 => ClkMuxLoad_s,
      o_ClkMuxConfig_p               => ClkMuxConfig_s,
      i_PLLLock_p                    => PLLLock_s,
      i_ovrAdcI_p                    => ovrAdcI_s,
      i_ovrAdcQ_p                    => ovrAdcQ_s,
      i_ovrDacI_p                    => ovrDacI_s,
      i_ovrDacQ_p                    => ovrDacQ_s,
      i_fmcPps_p                     => fmcPps_s,
      o_LimeReset_p                  => LimeReset_s,
      o_ResetOvr_p                   => ResetOvr_s,
      o_DesignClkEn_p                => DesignClkEn_s,
      o_CoreResetPulse_p             => CoreResetPulse_s,
      o_RstFifo_p                    => RstFifo_s,
      ov5_FpgaControl_p              => v5_FpgaControl_s,
      ov2_ClkMuxSin_p                => v2_ClkMuxSin_s,
      ov24_ddsfreq_p                 => v24_ddsfreq_s,
      ov3_dacOutSel_p                => v3_dacOutSel_s,
      ov16_limeSpiDataIn_p           => v16_limeSpiDataIn_s,
      iv16_limeSpiDataOut_p			 => v16_limeSpiDataOut_s,
      ov6_txGainSpiData_p            => v6_txGainSpiData_s,
      ov6_rxGainSpiData_p            => v6_rxGainSpiData_s,
      ov32_pllCtrlSpiDataIn_p        => v32_pllCtrlSpiDataIn_s,
      iv32_pllCtrlSpiDataOut_p       => v32_pllCtrlSpiDataOut_s,
      ov16_refDacSpiData_p           => v16_refDacSpiData_s,
      iv16_refDacSpiData_p           => iv16_refDacSpiData_p,
      o_limeSpiStart_p               => limeSpiStart_s,
      i_limeSpiBusy_p                => limeSpiBusy_s,
      i_pllCtrlSpiBusy_p             => pllCtrlSpiBusy_s,
      o_refDacSpiStart_p             => refDacSpiStart_s,
      i_refDacSpiBusy_p              => refDacSpiBusy_s,
      o_txGainSpiStart_p             => txGainSpiStart_s,
      i_txGainSpiBusy_p              => txGainSpiBusy_s,
      ov3_RxGainSpiStart_p           => v3_RxGainSpiStart_s,
      i_rxGainSpiBusy_p              => rxGainSpiBusy_s,
      ov2_pllCtrlSpiStart_p          => v2_pllCtrlSpiStart_s,
      ov13_IQGainImb_p               => v13_IQGainImb_s,
      ov13_IQPhaseImbCos_p           => v13_IQPhaseImbCos_s,
      ov13_IQPhaseImbSin_p           => v13_IQPhaseImbSin_s,
      ov13_IQImbBackOff_p            => v13_IQImbBackOff_s,
      iv12_RxIDcVal_p                => v12_RxIDcVal_s,
      iv12_RxQDcVal_p                => v12_RxQDcVal_s,
      iv23_ERF_p                     => v23_ERF_s,
      o_RXERRFCT_START_p             => RXERRFCT_START_s,
      i_RXERRFCT_DONE_p              => RXERRFCT_DONE_s,
      o_RXERRFCT_FREQ_WEN_p          => RXERRFCT_FREQ_WEN_s,
      ov14_RXERRFCT_NB_POINT_p       => v14_RXERRFCT_NB_POINT_s,
      ov24_RXERRFCT_FREQ_p           => v24_RXERRFCT_FREQ_s,
      iv32_RXERRFCT_ERR_I_p          => v32_RXERRFCT_ERR_I_s,
      iv32_RXERRFCT_ERR_Q_p          => v32_RXERRFCT_ERR_Q_s,
      ov5_AdcIdelayValue_p           => v5_AdcIdelayValue_s,
      ov5_AdcClkIdelayValue_p        => v5_AdcClkIdelayValue_s,
      S_AXI_ACLK                => S_AXI_ACLK    ,
      S_AXI_ARESETN             => S_AXI_ARESETN ,
      S_AXI_AWADDR              => S_AXI_AWADDR  ,
      S_AXI_AWVALID             => S_AXI_AWVALID ,
      S_AXI_WDATA               => S_AXI_WDATA   ,
      S_AXI_WSTRB               => S_AXI_WSTRB   ,
      S_AXI_WVALID              => S_AXI_WVALID  ,
      S_AXI_BREADY              => S_AXI_BREADY  ,
      S_AXI_ARADDR              => S_AXI_ARADDR  ,
      S_AXI_ARVALID             => S_AXI_ARVALID ,
      S_AXI_RREADY              => S_AXI_RREADY  ,
      S_AXI_ARREADY             => S_AXI_ARREADY ,
      S_AXI_RDATA               => S_AXI_RDATA   ,
      S_AXI_RRESP               => S_AXI_RRESP   ,
      S_AXI_RVALID              => S_AXI_RVALID  ,
      S_AXI_WREADY              => S_AXI_WREADY  ,
      S_AXI_BRESP               => S_AXI_BRESP   ,
      S_AXI_BVALID              => S_AXI_BVALID  ,
      S_AXI_AWREADY             => S_AXI_AWREADY
    );
    
  --------------------------------------------
  -- SW reset pulse stretcher.
  --------------------------------------------
  process(S_AXI_ACLK)
  begin
    if rising_edge(S_AXI_ACLK) then
      v8_SignalStretch_s <= v8_SignalStretch_s(6 downto 0) & CoreResetPulse_s; 			 		
    end if;
  end process;

  CoreReset_s <= or_reduce(v8_SignalStretch_s);

end arch;
