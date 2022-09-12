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
-- File : lyt_axi_adc5000.vhd
--------------------------------------------------------------------------------
-- Description : ADC5000 core with AXI interface
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_axi_adc5000.vhd,v $
-- Revision 1.1  2015/04/10 19:47:30  sqa
-- bas first commit (CBOU)
--
-- Revision 1.3  2013/05/02 18:21:47  julien.roy
-- Major modifications of ADC5000 core
--
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_unsigned.all;
  use ieee.std_logic_misc.all;

library lyt_axi_adc5000_v1_00_a;
  use lyt_axi_adc5000_v1_00_a.axi_adc5000;
  use lyt_axi_adc5000_v1_00_a.adc5000_pkg.all;

entity lyt_axi_adc5000 is
generic
(
  -- ADD USER GENERICS BELOW THIS LINE ---------------
  --USER generics added here
  -- ADD USER GENERICS ABOVE THIS LINE ---------------

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
  C_SLV_AWIDTH                   : integer              := 32;
  C_SLV_DWIDTH                   : integer              := 32
  -- DO NOT EDIT ABOVE THIS LINE ---------------------
);
port
(
  -- User ports
  i_RefClk200MHz_p					: in std_logic;
  o_DataClk_p            : out std_logic;
  ov80_DataChA_p         	  : out std_logic_vector( 79 downto 0 );
  ov80_DataChB_p         	  : out std_logic_vector( 79 downto 0 );
  ov80_DataChC_p         	  : out std_logic_vector( 79 downto 0 );
  ov80_DataChD_p         	  : out std_logic_vector( 79 downto 0 );
  o_OverRangeChA_p          : out std_logic;
  o_OverRangeChB_p          : out std_logic;
  o_OverRangeChC_p          : out std_logic;
  o_OverRangeChD_p          : out std_logic;
  o_ReadyChA_p              : out std_logic;
  o_ReadyChB_p              : out std_logic;
  o_ReadyChC_p              : out std_logic;
  o_ReadyChD_p              : out std_logic;
  o_Trigger_p               : out std_logic;
  -- FMC ports
  idp_Fmc_Clk2Fpga_p        : in    std_logic;
  idn_Fmc_Clk2Fpga_p        : in    std_logic;
  idp_Fmc_ext_trigger_p     : in    std_logic;
  idn_fmc_ext_trigger_p     : in    std_logic;
  odp_Fmc_Sync_p            : out   std_logic;
  odn_Fmc_Sync_p            : out   std_logic;
  idp_Fmc_adr_p             : in    std_logic;
  idn_Fmc_adr_p             : in    std_logic;
  iv10dp_Fmc_ad_p           : in    std_logic_vector(9 downto 0);
  iv10dn_Fmc_ad_p           : in    std_logic_vector(9 downto 0);
  idp_Fmc_aor_p             : in    std_logic;
  idn_Fmc_aor_p             : in    std_logic;
  idp_Fmc_bdr_p             : in    std_logic;
  idn_Fmc_bdr_p             : in    std_logic;
  iv10dp_Fmc_bd_p           : in    std_logic_vector(9 downto 0);
  iv10dn_Fmc_bd_p           : in    std_logic_vector(9 downto 0);
  idp_Fmc_bor_p             : in    std_logic;
  idn_Fmc_bor_p             : in    std_logic;
  idp_Fmc_cdr_p             : in    std_logic;
  idn_Fmc_cdr_p             : in    std_logic;
  iv10dp_Fmc_cd_p           : in    std_logic_vector(9 downto 0);
  iv10dn_Fmc_cd_p           : in    std_logic_vector(9 downto 0);
  idp_Fmc_cor_p             : in    std_logic;
  idn_Fmc_cor_p             : in    std_logic;
  idp_Fmc_ddr_p             : in    std_logic;
  idn_Fmc_ddr_p             : in    std_logic;
  iv10dp_Fmc_dd_p           : in    std_logic_vector(9 downto 0);
  iv10dn_Fmc_dd_p           : in    std_logic_vector(9 downto 0);
  idp_Fmc_dor_p             : in    std_logic;
  idn_Fmc_dor_p             : in    std_logic;
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

  attribute MAX_FANOUT                     : string;
  attribute SIGIS                          : string;
  attribute MAX_FANOUT of S_AXI_ACLK       : signal is "10000";
  attribute MAX_FANOUT of S_AXI_ARESETN    : signal is "10000";
  attribute SIGIS of S_AXI_ACLK            : signal is "Clk";
  attribute SIGIS of i_RefClk200MHz_p      : signal is "Clk";
  attribute SIGIS of S_AXI_ARESETN         : signal is "Rst";

end entity lyt_axi_adc5000;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of lyt_axi_adc5000 is

  -- Core reset signals
  signal CoreResetPulse_s			  : std_logic;
  signal v8_SignalStretch_s		  : std_logic_vector(7 downto 0);
  signal CoreReset_s				    : std_logic;

  signal delay_reset_s		: std_logic;
  signal clk_reset_s			: std_logic;
  signal v4_data_aligned_s	  : std_logic_vector(3 downto 0);
  signal v4_data_aligned_R1_s	: std_logic_vector(3 downto 0);
  signal v4_Otr_s	      : std_logic_vector(3 downto 0);
  signal io_reset_s			: std_logic;
  signal rstFifo_s        : std_logic;
  signal start_align_s		: std_logic;
  signal sync_cmd_s			: std_logic;
  
  signal dataClk_s      : std_logic;

  signal v11_PhyDlyIncA_s	: std_logic_vector(10 downto 0);
  signal v11_PhyDlyDecA_s	: std_logic_vector(10 downto 0);
  signal v11_PhyDlyIncB_s	: std_logic_vector(10 downto 0);
  signal v11_PhyDlyDecB_s	: std_logic_vector(10 downto 0);
  signal v11_PhyDlyIncC_s	: std_logic_vector(10 downto 0);
  signal v11_PhyDlyDecC_s	: std_logic_vector(10 downto 0);
  signal v11_PhyDlyIncD_s	: std_logic_vector(10 downto 0);
  signal v11_PhyDlyDecD_s	: std_logic_vector(10 downto 0);

  signal v11_PhyBitslipA_s	: std_logic_vector(10 downto 0);
  signal v11_PhyBitslipB_s	: std_logic_vector(10 downto 0);
  signal v11_PhyBitslipC_s	: std_logic_vector(10 downto 0);
  signal v11_PhyBitslipD_s	: std_logic_vector(10 downto 0);

  signal v2_trigger_sel_reg_s 	: std_logic_vector(1 downto 0);
  signal FreqCntRst_s			: std_logic;
  signal v16_FeqCntClkCnt_s		: std_logic_vector(15 downto 0);
  signal v4_FeqCntClkSel_s		: std_logic_vector(3 downto 0);
  signal v4_Phy_cmd_in_s			: std_logic_vector(3 downto 0);
  signal v5_TriggerDelay_s 			: std_logic_vector(4 downto 0);
  signal v4_PhyDlyValid_s		: std_logic_vector(3 downto 0);

  signal ov128_phyDataA_s		: std_logic_vector( 127 downto 0);
  signal ov128_phyDataB_s		: std_logic_vector( 127 downto 0);
  signal ov128_phyDataC_s		: std_logic_vector( 127 downto 0);
  signal ov128_phyDataD_s		: std_logic_vector( 127 downto 0);
  
  signal v5_adcIdelayValueA_s     : std_logic_vector(4 downto 0);
  signal v5_adcIdelayValueB_s     : std_logic_vector(4 downto 0);
  signal v5_adcIdelayValueC_s     : std_logic_vector(4 downto 0);
  signal v5_adcIdelayValueD_s     : std_logic_vector(4 downto 0);
  signal v10_adcIdelayValueA_we_s : std_logic_vector(9 downto 0);
  signal v10_adcIdelayValueB_we_s : std_logic_vector(9 downto 0);
  signal v10_adcIdelayValueC_we_s : std_logic_vector(9 downto 0);
  signal v10_adcIdelayValueD_we_s : std_logic_vector(9 downto 0);
  signal v10_bitslipA_s           : std_logic_vector(9 downto 0);
  signal v10_bitslipB_s           : std_logic_vector(9 downto 0);
  signal v10_bitslipC_s           : std_logic_vector(9 downto 0);
  signal v10_bitslipD_s           : std_logic_vector(9 downto 0);
  
  signal v10_calibErrorChA_s    : std_logic_vector(9 downto 0);
  signal v10_calibErrorChB_s    : std_logic_vector(9 downto 0);
  signal v10_calibErrorChC_s    : std_logic_vector(9 downto 0);
  signal v10_calibErrorChD_s    : std_logic_vector(9 downto 0);
  signal v10_calibErrorChA_R1_s : std_logic_vector(9 downto 0);
  signal v10_calibErrorChB_R1_s : std_logic_vector(9 downto 0);
  signal v10_calibErrorChC_R1_s : std_logic_vector(9 downto 0);
  signal v10_calibErrorChD_R1_s : std_logic_vector(9 downto 0);
  signal v10_calibErrorChA_R2_s : std_logic_vector(9 downto 0);
  signal v10_calibErrorChB_R2_s : std_logic_vector(9 downto 0);
  signal v10_calibErrorChC_R2_s : std_logic_vector(9 downto 0);
  signal v10_calibErrorChD_R2_s : std_logic_vector(9 downto 0);

begin

  -----------------------------------------------------------------
  -- Trim the 6 MSB zeros bits to get back the ADC 10-bits/sample.
  -----------------------------------------------------------------
  samples: for s in 0 to 7 generate
    Trim_bits: for b in 0 to 9 generate
      ov80_DataChA_p(s*10+b) 	<= 	ov128_phyDataA_s(s*16+b);
      ov80_DataChB_p(s*10+b) 	<= 	ov128_phyDataB_s(s*16+b);
      ov80_DataChC_p(s*10+b) 	<= 	ov128_phyDataC_s(s*16+b);
      ov80_DataChD_p(s*10+b) 	<= 	ov128_phyDataD_s(s*16+b);
    end generate Trim_bits;
  end generate samples;

  process(dataClk_s)
  begin
    if rising_edge(dataClk_s) then
      v4_data_aligned_R1_s  <= v4_data_aligned_s;
      o_ReadyChA_p          <= v4_data_aligned_R1_s(0);
      o_ReadyChB_p          <= v4_data_aligned_R1_s(1);
      o_ReadyChC_p          <= v4_data_aligned_R1_s(2);
      o_ReadyChD_p          <= v4_data_aligned_R1_s(3);
    end if;
  end process;
  
  o_OverRangeChA_p <= v4_Otr_s(0);
  o_OverRangeChB_p <= v4_Otr_s(1);
  o_OverRangeChC_p <= v4_Otr_s(2);
  o_OverRangeChD_p <= v4_Otr_s(3);
  
  ----------------------------------------------------
  -- Latch signals to AXI register for metastability
  ----------------------------------------------------
  process(S_AXI_ACLK)
  begin
    if rising_edge(S_AXI_ACLK) then
      v10_calibErrorChA_R1_s <= v10_calibErrorChA_s;
      v10_calibErrorChB_R1_s <= v10_calibErrorChB_s;
      v10_calibErrorChC_R1_s <= v10_calibErrorChC_s;
      v10_calibErrorChD_R1_s <= v10_calibErrorChD_s;
      
      v10_calibErrorChA_R2_s <= v10_calibErrorChA_R1_s;
      v10_calibErrorChB_R2_s <= v10_calibErrorChB_R1_s;
      v10_calibErrorChC_R2_s <= v10_calibErrorChC_R1_s;
      v10_calibErrorChD_R2_s <= v10_calibErrorChD_R1_s;
    end if;
  end process;

  ------------------------------------------
  -- instantiate User Logic
  ------------------------------------------
  USER_LOGIC_I : entity lyt_axi_adc5000_v1_00_a.axi_adc5000
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
    i_CoreReset_p             => CoreReset_s,
    
    o_delay_reset_p           => delay_reset_s,
    o_clk_reset_p             => clk_reset_s,
    ov4_data_aligned_p        => v4_data_aligned_s,
    o_io_reset_p              => io_reset_s,
    o_RstFifo_p               => rstFifo_s,
    o_sync_cmd_p              => sync_cmd_s,
    o_CoreResetPulse_p			  => CoreResetPulse_s,
    ov5_adcIdelayValueA_p     => v5_adcIdelayValueA_s,
    ov5_adcIdelayValueB_p     => v5_adcIdelayValueB_s,
    ov5_adcIdelayValueC_p     => v5_adcIdelayValueC_s,
    ov5_adcIdelayValueD_p     => v5_adcIdelayValueD_s,
    ov10_adcIdelayValueA_we_p => v10_adcIdelayValueA_we_s,
    ov10_adcIdelayValueB_we_p => v10_adcIdelayValueB_we_s,
    ov10_adcIdelayValueC_we_p => v10_adcIdelayValueC_we_s,
    ov10_adcIdelayValueD_we_p => v10_adcIdelayValueD_we_s,
    ov10_bitslipA_p           => v10_bitslipA_s,
    ov10_bitslipB_p           => v10_bitslipB_s,
    ov10_bitslipC_p           => v10_bitslipC_s,
    ov10_bitslipD_p           => v10_bitslipD_s,
    iv10_calibErrorChA_p      => v10_calibErrorChA_R2_s,
    iv10_calibErrorChB_p      => v10_calibErrorChB_R2_s,
    iv10_calibErrorChC_p      => v10_calibErrorChC_R2_s,
    iv10_calibErrorChD_p      => v10_calibErrorChD_R2_s,
    ov5_TriggerDelay_p        => v5_TriggerDelay_s,
    o_FreqCntRst_p			      => FreqCntRst_s,
    iv16_FeqCntClkCnt_p       => v16_FeqCntClkCnt_s,
    ov4_FeqCntClkSel_p        => v4_FeqCntClkSel_s,
    -- Bus ports mapping
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
      CoreReset_s <= or_reduce(v8_SignalStretch_s);	 		
    end if;
  end process;

  ------------------------------------------
  -- instantiate ADC5000 top
  ------------------------------------------
  v4_Phy_cmd_in_s <= (sync_cmd_s & io_reset_s & clk_reset_s & delay_reset_s);


  Adc5000_inst: adc5000_top
  port map(
    -- Global signals
    rst              => CoreReset_s,
    REF_CLOCK        => i_RefClk200MHz_p,
    --- Registers Interface ---
    -- SERDES control PHY registers
    clk_regs         => S_AXI_ACLK,
    Phy_cmd_in       => v4_Phy_cmd_in_s,
    i_RstFifo_p      => rstFifo_s,
    
    iv5_adcIdelayValueA_p     => v5_adcIdelayValueA_s,
    iv5_adcIdelayValueB_p     => v5_adcIdelayValueB_s,
    iv5_adcIdelayValueC_p     => v5_adcIdelayValueC_s,
    iv5_adcIdelayValueD_p     => v5_adcIdelayValueD_s,
    iv10_adcIdelayValueA_we_p => v10_adcIdelayValueA_we_s,
    iv10_adcIdelayValueB_we_p => v10_adcIdelayValueB_we_s,
    iv10_adcIdelayValueC_we_p => v10_adcIdelayValueC_we_s,
    iv10_adcIdelayValueD_we_p => v10_adcIdelayValueD_we_s,
    iv10_bitslipA_p           => v10_bitslipA_s,
    iv10_bitslipB_p           => v10_bitslipB_s,
    iv10_bitslipC_p           => v10_bitslipC_s,
    iv10_bitslipD_p           => v10_bitslipD_s,
    ov10_calibErrorChA_p      => v10_calibErrorChA_s,
    ov10_calibErrorChB_p      => v10_calibErrorChB_s,
    ov10_calibErrorChC_p      => v10_calibErrorChC_s,
    ov10_calibErrorChD_p      => v10_calibErrorChD_s,
    -- Trigger control Register
    iv5_TriggerDelay_p    => v5_TriggerDelay_s,
    -- Frequancy counter registers
    FeqCnt_Cnt_rst   => FreqCntRst_s,
    FeqCnt_clk_sel   => v4_FeqCntClkSel_s,
    FeqCnt_clk_cnt   => v16_FeqCntClkCnt_s,
    --- ADC output user data
    phy_clk          => dataClk_s,
    phy_data_a       => ov128_phyDataA_s,
    phy_data_b       => ov128_phyDataB_s,
    phy_data_c       => ov128_phyDataC_s,
    phy_data_d       => ov128_phyDataD_s,
    ov4_Otr_p     	 => v4_Otr_s,
    triggerOut       => o_Trigger_p,
    --- Serdes external ports
    clk_to_fpga_p    => idp_Fmc_Clk2Fpga_p,
    clk_to_fpga_n    => idn_Fmc_Clk2Fpga_p,
    ext_trigger_p    => idp_Fmc_ext_trigger_p,
    ext_trigger_n    => idn_fmc_ext_trigger_p,
    sync_from_fpga_p => odp_Fmc_Sync_p,
    sync_from_fpga_n => odn_Fmc_Sync_p,
    -- Channel A
    adr_p            => idp_Fmc_adr_p,
    adr_n            => idn_Fmc_adr_p,
    ad_p             => iv10dp_Fmc_ad_p,
    ad_n             => iv10dn_Fmc_ad_p,
    aor_p            => idp_Fmc_aor_p,
    aor_n            => idn_Fmc_aor_p,
    -- Channel B
    bdr_p            => idp_Fmc_bdr_p,
    bdr_n            => idn_Fmc_bdr_p,
    bd_p             => iv10dp_Fmc_bd_p,
    bd_n             => iv10dn_Fmc_bd_p,
    bor_p            => idp_Fmc_bor_p,
    bor_n            => idn_Fmc_bor_p,
    -- Channel C
    cdr_p            => idp_Fmc_cdr_p,
    cdr_n            => idn_Fmc_cdr_p,
    cd_p             => iv10dp_Fmc_cd_p,
    cd_n             => iv10dn_Fmc_cd_p,
    cor_p            => idp_Fmc_cor_p,
    cor_n            => idn_Fmc_cor_p,
    -- Channel D
    ddr_p            => idp_Fmc_ddr_p,
    ddr_n            => idn_Fmc_ddr_p,
    dd_p             => iv10dp_Fmc_dd_p,
    dd_n             => iv10dn_Fmc_dd_p,
    dor_p            => idp_Fmc_dor_p,
    dor_n            => idn_Fmc_dor_p
  );
  
  o_DataClk_p <= dataClk_s;

end IMP;