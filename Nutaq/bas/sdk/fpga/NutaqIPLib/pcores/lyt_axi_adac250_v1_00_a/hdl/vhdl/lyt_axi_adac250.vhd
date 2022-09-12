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
-- File : lyt_axi_adac250.vhd
--------------------------------------------------------------------------------
-- Description : ADAC250 core with AXI interface
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_axi_adac250.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.3  2015/02/05 16:41:57  julien.roy
-- Remove PPS port since it is the same signal than trigger
--
-- Revision 1.2  2013/08/29 19:53:37  khalid.bensadek
-- Added pps out port
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;
  use ieee.std_logic_misc.all;

library unisim;
  use unisim.vcomponents.all;

library lyt_axi_adac250_v1_00_a;
  use lyt_axi_adac250_v1_00_a.adac250_wrapper_p.all;

entity lyt_axi_adac250 is
generic
(
  -- ADD USER GENERICS BELOW THIS LINE ---------------
  ADC_CLKIN_FREQ                 : real := 250000000.0;
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
  C_FAMILY                       : string               := "virtex6"
  -- DO NOT EDIT ABOVE THIS LINE ---------------------
);
port
(
  -- User ports --
  i_RefClk200MHz_p              : in std_logic;

  idp_fmcClk0_p                 : in std_logic;
  idn_fmcClk0_p                 : in std_logic;

  iv15dp_AdcExtBus_p            : in std_logic_vector(14 downto 0);
  iv15dn_AdcExtBus_p            : in std_logic_vector(14 downto 0);
  ov18dp_DacExtBus_p            : out std_logic_vector(17 downto 0);
  ov18dn_DacExtBus_p            : out std_logic_vector(17 downto 0);
  iv3_CtrlExtBus_p              : in std_logic_vector(2 downto 0);
  ov23_CtrlExtBus_p             : out std_logic_vector(22 downto 0);

  --To user logic
  o_AdcDataClk_p                : out std_logic;
  ov14_AdcDataChA_p             : out std_logic_vector (13 downto 0);
  ov14_AdcDataChB_p             : out std_logic_vector (13 downto 0);
  o_DataFormat_p                : out std_logic;

  iv16_DacChA_p                 : in std_logic_vector(15 downto 0);
  iv16_DacChB_p                 : in std_logic_vector(15 downto 0);
  i_DacDesignClk_p              : in std_logic;
  i_DacDataSync_p               : in std_logic;
  o_DacRefDataClk_p             : out std_logic;
  o_DacDataClkLocked_p          : out std_logic;
  ----***ADCOutOfRange***----
  o_ChA_OvrFiltred_p            : out std_logic;
  o_ChA_OvrNotFiltred_p         : out std_logic;
  o_ChB_OvrFiltred_p            : out std_logic;
  o_ChB_OvrNotFiltred_p         : out std_logic;
  ----***Trigger***----
  o_Trigger_p                   : out std_logic;
  
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

end entity lyt_axi_adac250;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture arch of lyt_axi_adac250 is

  ----------------------------------------
  -- Functions declaration
  ----------------------------------------

  function CalculateFbOut( ADC_CLKIN_PERIOD : real ) return real
  is
    variable result : real;
  begin
    if( ADC_CLKIN_PERIOD >= 10000000.0 and ADC_CLKIN_PERIOD < 20000000.0 ) then
      result := 60.0;
    elsif( ADC_CLKIN_PERIOD >= 20000000.0 and ADC_CLKIN_PERIOD < 40000000.0 ) then
      result := 30.0;
    elsif ( ADC_CLKIN_PERIOD >= 40000000.0 and ADC_CLKIN_PERIOD < 60000000.0 ) then
      result := 20.0;
    elsif ( ADC_CLKIN_PERIOD >= 60000000.0 and ADC_CLKIN_PERIOD < 100000000.0 ) then
      result := 12.0;
    elsif ( ADC_CLKIN_PERIOD >= 100000000.0 and ADC_CLKIN_PERIOD < 120000000.0 ) then
      result := 10.0;
    elsif ( ADC_CLKIN_PERIOD >= 150000000.0 and ADC_CLKIN_PERIOD < 200000000.0 ) then
      result := 6.0;
    else
      result := 8.0;
    end if;
    return result;
  end CalculateFbOut;

  function CalculateDivClkDiv( ADC_CLKIN_PERIOD : real ) return integer
  is
    variable result : integer;
  begin
    if( ADC_CLKIN_PERIOD >= 200000000.0 and ADC_CLKIN_PERIOD < 300000000.0 ) then
      result := 2;
    else
      result := 1;
    end if;
    return result;
  end CalculateDivClkDiv;

  function CalculateClockOutDiv( ADC_CLKIN_PERIOD : real;
                                 ADC_CLKFBOUT_MULT_F : real ) return real
  is
    variable result : real;
  begin
    if( ADC_CLKIN_PERIOD >= 200000000.0 and ADC_CLKIN_PERIOD < 300000000.0 ) then
      result := 4.0;
    else
      result := ADC_CLKFBOUT_MULT_F;
    end if;
    return result;
  end CalculateClockOutDiv;
  
  function CalculateMMCMBandwidth( ADC_CLKIN_FREQ : real;
                                 ADC_DIVCLK_DIVIDE :integer ) return string
  is
    constant MMCM_LOW : string := "LOW";
    constant MMCM_OPT : string := "OPTIMIZED";
  begin
    if( (ADC_CLKIN_FREQ / real(ADC_DIVCLK_DIVIDE)) <= 135000000.0 ) then
      return MMCM_LOW;
    else
      return MMCM_OPT;
    end if;
  end CalculateMMCMBandwidth;
 
  ----------------------------------------
  -- Constants declaration
  ----------------------------------------
 
  constant ADC_CLKIN_PERIOD : real := 1000000000.0 / ADC_CLKIN_FREQ;
  constant ADC_CLKFBOUT_MULT_F : real := CalculateFbOut( ADC_CLKIN_FREQ );
  constant ADC_DIVCLK_DIVIDE : integer := CalculateDivClkDiv( ADC_CLKIN_FREQ);
  constant ADC_CLKOUT0_DIVIDE_F : real := CalculateClockOutDiv( ADC_CLKIN_FREQ, ADC_CLKFBOUT_MULT_F);

  ----------------------------------------
  -- Signals declaration
  ----------------------------------------
  signal CoreResetPulse_s             : std_logic;
  signal v2_AdcRun_s                  : std_logic_vector(1 downto 0);
  signal v2_AdcStart_s                : std_logic_vector(1 downto 0);
  signal AdcDataFormat_s              : std_logic;
  signal AdcSpiReset_s                : std_logic;
  signal ADCAOvr_s                    : std_logic;
  signal ADCBOvr_s                    : std_logic;
  signal AdcResetMmcm_s               : std_logic;
  signal v5_TriggerDelay_s            : std_logic_vector(4 downto 0);
  signal DacReset_s                   : std_logic;
  signal DacDataClkLocked_s           : std_logic;
  signal v3_DacAMuxSelect_s           : std_logic_vector(2 downto 0);
  signal v3_DacBMuxSelect_s           : std_logic_vector(2 downto 0);
  signal PllStatus_s                  : std_logic;
  signal PllFunction_s                : std_logic;
  signal UpdaterBusy_s                : std_logic;
  signal ClkMuxConfig_s               : std_logic;
  signal ClkMuxLoad_s                 : std_logic;
  signal v2_ClkMuxSin_s               : std_logic_vector(1 downto 0);
  signal v2_ClkMuxSout_s              : std_logic_vector(1 downto 0);
  signal v5_AdcIdelayValue_s          : std_logic_vector(4 downto 0);
  signal v5_AdcClkIdelayValue_s       : std_logic_vector(4 downto 0);
  signal AdcPatternError_s            : std_logic;
  signal v5_DacIdelayValue_s          : std_logic_vector(4 downto 0);
  signal v5_DacClkIdelayValue_s       : std_logic_vector(4 downto 0);
  signal SpiBusy_s                    : std_logic;
  signal SpiReq_s                     : std_logic;
  signal SpiGnt_s                     : std_logic;
  signal SpiAck_s                     : std_logic;
  signal v9_SpiWriteaddr              : std_logic_vector(8 downto 0);
  signal v9_SpiReadaddr               : std_logic_vector(8 downto 0);
  signal v32_SpiDin_s                 : std_logic_vector(31 downto 0);
  signal v32_SpiDout_s                : std_logic_vector(31 downto 0);
  signal v2_FreqCntClkSel_s           : std_logic_vector(1 downto 0);
  signal v16_FreqCntClkCnt_s          : std_logic_vector(15 downto 0);
  
  -- MMCM signalssignal 
  signal adcClkDcmO_s     : std_logic;
  signal adcClkBufgO_s    : std_logic;
  signal dcmLock_s        : std_logic;
  signal MmcmFbOutBufg_s  : std_logic;
  signal MmcmFbOut_s      : std_logic;
  signal MmcmReset_s      : std_logic;
  signal adcClkBufR_s     : std_logic;
  signal WrapMmcmReset_s  : std_logic;
  
  -- Overflow signals
  signal ChA_OvrFiltred_s     : std_logic;
  signal ChA_OvrNotFiltred_s  : std_logic;
  signal ChB_OvrFiltred_s     : std_logic;
  signal ChB_OvrNotFiltred_s  : std_logic;
  
  signal AdcPatternError_r1_s   : std_logic;
  signal AdcPatternError_r2_s   : std_logic;
  signal AdcDataFormat_r1_s     : std_logic;
  signal AdcDataFormat_r2_s     : std_logic;
  
  -- Reset signals
  signal v8_SignalStretch_s		  : std_logic_vector(7 downto 0);
  signal CoreReset_s				    : std_logic;
  
begin

  MmcmReset_s <= AdcResetMmcm_s or WrapMmcmReset_s;

  adac250_mmcm_adv_inst : entity lyt_axi_adac250_v1_00_a.mmcm_calib
  generic map
  (
    BANDWIDTH            => CalculateMMCMBandwidth( ADC_CLKIN_FREQ, ADC_DIVCLK_DIVIDE),    --Was "OPTIMIZED" with AR#38132 must be set to low if clkin/d <= 135Mhz.
    CLKOUT4_CASCADE      => FALSE,
    CLOCK_HOLD           => FALSE,
    COMPENSATION         => "ZHOLD",
    STARTUP_WAIT         => FALSE,
    DIVCLK_DIVIDE        => ADC_DIVCLK_DIVIDE,
    CLKFBOUT_MULT_F      => ADC_CLKFBOUT_MULT_F,
    CLKFBOUT_PHASE       => 0.000,
    CLKFBOUT_USE_FINE_PS => FALSE,
    CLKOUT0_DIVIDE_F     => ADC_CLKOUT0_DIVIDE_F,
    CLKOUT0_PHASE        => 0.000,
    CLKOUT0_DUTY_CYCLE   => 0.500,
    CLKOUT0_USE_FINE_PS  => FALSE,
    CLKIN1_PERIOD        => ADC_CLKIN_PERIOD,
    REF_JITTER1          => 0.010
  )
  port map
    -- Output clocks
  (
    CLKFBOUT          => MmcmFbOut_s,
    CLKFBOUTB         => open,
    CLKOUT0           => adcClkDcmO_s,
    CLKOUT0B          => open,
    CLKOUT1           => open,
    CLKOUT1B          => open,
    CLKOUT2           => open,
    CLKOUT2B          => open,
    CLKOUT3           => open,
    CLKOUT3B          => open,
    CLKOUT4           => open,
    CLKOUT5           => open,
    CLKOUT6           => open,
    -- Input clock control
    CLKFBIN           => MmcmFbOutBufg_s,
    CLKIN1            => adcClkBufR_s,
    CLKIN2            => '0',
    -- Tied to always select the primary input clock
    CLKINSEL          => '1',
    -- Ports for dynamic reconfiguration
    DADDR             => (others => '0'),
    DCLK              => '0',
    DEN               => '0',
    DI                => (others => '0'),
    DO                => open,
    DRDY              => open,
    DWE               => '0',
    -- Ports for dynamic phase shift
    PSCLK             => '0',
    PSEN              => '0',
    PSINCDEC          => '0',
    PSDONE            => open,
    -- Other control and status signals
    LOCKED            => dcmLock_s,
    CLKINSTOPPED      => open,
    CLKFBSTOPPED      => open,
    PWRDWN            => '0',
    RST               => MmcmReset_s
  );

  ClkfBufg_l : BUFG
  port map
  (
    O => MmcmFbOutBufg_s,
    I => MmcmFbOut_s
  );

  BUFG_inst : BUFG
  port map
  (
    O => adcClkBufgO_s, -- 1-bit Clock buffer output
    I => adcClkDcmO_s -- 1-bit Clock buffer input
  );

  ------------------------------------------
  -- instantiate your component here!
  ------------------------------------------
  ADAC250Wrapper_l : adac250_wrapper
    port map(
      ----***Global ports***----
      i_SystemClk_p               => S_AXI_ACLK,
      i_RefClk200MHz_p            => i_RefClk200MHz_p,
      i_rst_p                     => CoreReset_s,

      idp_fmcClk0_p               => idp_fmcClk0_p,
      idn_fmcClk0_p               => idn_fmcClk0_p,

      iv15dp_AdcExtBus_p          => iv15dp_AdcExtBus_p,
      iv15dn_AdcExtBus_p          => iv15dn_AdcExtBus_p,
      ov18dp_DacExtBus_p          => ov18dp_DacExtBus_p,
      ov18dn_DacExtBus_p          => ov18dn_DacExtBus_p,
      iv3_CtrlExtBus_p            => iv3_CtrlExtBus_p,
      ov23_CtrlExtBus_p           => ov23_CtrlExtBus_p,

      --To user logic
      i_AdcDataClk_p              => adcClkBufgO_s,
      i_AdcClkLock_p              => dcmLock_s,
      o_AdcClkBufr_p              => adcClkBufR_s,
      o_MmcmReset_p               => WrapMmcmReset_s,

      ov14_AdcDataChA_p           => ov14_AdcDataChA_p,
      ov14_AdcDataChB_p           => ov14_AdcDataChB_p,
      -- From control
      iv2_AdcRun_p                => v2_AdcRun_s,
      iv2_AdcStart_p              => v2_AdcStart_s,
      ----***DAC ports***----
      --To/From user logic
      iv16_DacChA_p               => iv16_DacChA_p,
      iv16_DacChB_p               => iv16_DacChB_p,
      i_DacDesignClk_p            => i_DacDesignClk_p,
      i_DacDataSync_p             => i_DacDataSync_p,

      o_DacRefDataClk_p           => o_DacRefDataClk_p,
      o_DacDataClkLocked_p        => DacDataClkLocked_s,

      iv3_DacAMuxSelect_p         => v3_DacAMuxSelect_s,
      iv3_DacBMuxSelect_p         => v3_DacBMuxSelect_s,
      ----***ADCOutOfRange***----
      i_DataType_p                => AdcDataFormat_r2_s,     -- 0 = offset binary output format / 1= 2s complement output format
      o_ChA_OvrFiltred_p          => ChA_OvrFiltred_s,
      o_ChA_OvrNotFiltred_p       => ChA_OvrNotFiltred_s,
      o_ChB_OvrFiltred_p          => ChB_OvrFiltred_s,
      o_ChB_OvrNotFiltred_p       => ChB_OvrNotFiltred_s,
      ----***Trigger***----
      iv5_TriggerDelay_p          => v5_TriggerDelay_s,
      o_Trigger_p                 => o_Trigger_p,
      ----***SPI interface ***----
      o_SpiBusy_p                 => SpiBusy_s,
      iv9_SpiWriteaddr_p          => v9_SpiWriteaddr,
      iv9_SpiReadaddr_p           => v9_SpiReadaddr,
      i_SpiReq_p                  => SpiReq_s,
      iv32_SpiDin_p               => v32_SpiDin_s,
      o_SpiGnt_p                  => SpiGnt_s,
      ov32_SpiDout_p              => v32_SpiDout_s,
      o_SpiAck_p                  => SpiAck_s,
      --From/To custom logic
      i_AdcSpiReset_p             => AdcSpiReset_s,
      i_DacReset_p                => DacReset_s,

      o_PllStatus_p               => PllStatus_s,
      i_PllFunction_p             => PllFunction_s,
      i_ClkMuxConfig_p            => ClkMuxConfig_s,
      i_ClkMuxLoad_p              => ClkMuxLoad_s,
      iv2_ClkMuxSin_p             => v2_ClkMuxSin_s,
      iv2_ClkMuxSout_p            => v2_ClkMuxSout_s,
      
      iv5_AdcIdelayValue_p        => v5_AdcIdelayValue_s,
      iv5_AdcClkIdelayValue_p     => v5_AdcClkIdelayValue_s,
      o_AdcPatternError_p         => AdcPatternError_s,
      iv5_DacIdelayValue_p        => v5_DacIdelayValue_s,
      iv5_DacClkIdelayValue_p     => v5_DacClkIdelayValue_s,
      
      iv2_FreqCntClkSel_p         => v2_FreqCntClkSel_s,
      ov16_FreqCntClkCnt_p        => v16_FreqCntClkCnt_s);
  
  --------------------------------------------
  -- Output assignments
  --------------------------------------------  
  o_AdcDataClk_p  <= adcClkBufgO_s;
  
  o_DacDataClkLocked_p <= DacDataClkLocked_s;
  
  o_ChA_OvrFiltred_p    <= ChA_OvrFiltred_s;
  o_ChA_OvrNotFiltred_p <= ChA_OvrNotFiltred_s;
  o_ChB_OvrFiltred_p    <= ChB_OvrFiltred_s;
  o_ChB_OvrNotFiltred_p <= ChB_OvrNotFiltred_s;
  o_DataFormat_p        <= AdcDataFormat_r2_s;
  
  --------------------------------------------
  -- Clock synchronization (for metastability)
  --------------------------------------------    
  -- AXI clock
  process(S_AXI_ACLK)
    variable ChA_OvrFiltred_r1  : std_logic := '0';
    variable ChB_OvrFiltred_r1  : std_logic := '0';
  begin
    if rising_edge(S_AXI_ACLK) then
    
      ADCAOvr_s <= ChA_OvrFiltred_r1;
      ADCBOvr_s <= ChB_OvrFiltred_r1;
      ChA_OvrFiltred_r1 := ChA_OvrFiltred_s;
      ChB_OvrFiltred_r1 := ChB_OvrFiltred_s;
      
      AdcPatternError_r1_s  <= AdcPatternError_s;
      AdcPatternError_r2_s  <= AdcPatternError_r1_s;
      
    end if;
  end process;
  
  -- ADC clock
  process(adcClkBufgO_s)
  begin
    if rising_edge(adcClkBufgO_s) then
    
      AdcDataFormat_r1_s <= AdcDataFormat_s;
      AdcDataFormat_r2_s <= AdcDataFormat_r1_s;
      
    end if;
  end process;

  --------------------------------------------
  -- instantiate AXI Memory Mapped User Logic
  --------------------------------------------
  USER_LOGIC_I : entity lyt_axi_adac250_v1_00_a.axi_adac250
    generic map(
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
      C_FAMILY                  => C_FAMILY          )
    port map(
      -- user_logic entity ports mapping  ---------------
      i_CoreReset_p                   => CoreReset_s,
      o_CoreResetPulse_p              => CoreResetPulse_s,
      
      ov2_AdcRun_p                    => v2_AdcRun_s,
      ov2_AdcStart_p                  => v2_AdcStart_s,
      o_AdcDataFormat_p               => AdcDataFormat_s,
      o_AdcSpiReset_p                 => AdcSpiReset_s,
      i_ADCAOvr_p                     => ADCAOvr_s,
      i_ADCBOvr_p                     => ADCBOvr_s,
      o_AdcResetMmcm_p                => AdcResetMmcm_s,
      ov5_TriggerDelay_p              => v5_TriggerDelay_s,
      
      o_DacReset_p                    => DacReset_s,
      i_DacDataClkLocked_p            => DacDataClkLocked_s,
      ov3_DacAMuxSelect_p             => v3_DacAMuxSelect_s,
      ov3_DacBMuxSelect_p             => v3_DacBMuxSelect_s,
      i_PllStatus_p                   => PllStatus_s,
      o_PllFunction_p                 => PllFunction_s,
      i_UpdaterBusy_p                 => '0',
      o_ClkMuxConfig_p                => ClkMuxConfig_s,
      o_ClkMuxLoad_p                  => ClkMuxLoad_s,
      ov2_ClkMuxSin_p                 => v2_ClkMuxSin_s,
      ov2_ClkMuxSout_p                => v2_ClkMuxSout_s,
      
      i_SpiBusy_p                     => SpiBusy_s,
      o_SpiReq_p                      => SpiReq_s,
      i_SpiGnt_p                      => SpiGnt_s,
      i_SpiAck_p                      => SpiAck_s,
      ov9_SpiWriteaddr_p              => v9_SpiWriteaddr,
      ov9_SpiReadaddr_p               => v9_SpiReadaddr,
      ov32_SpiDin_p                   => v32_SpiDin_s,
      iv32_SpiDout_p                  => v32_SpiDout_s,

      ov5_AdcIdelayValue_p            => v5_AdcIdelayValue_s,
      ov5_AdcClkIdelayValue_p         => v5_AdcClkIdelayValue_s,
      i_AdcPatternError_p             => AdcPatternError_r2_s,
      ov5_DacIdelayValue_p            => v5_DacIdelayValue_s,
      ov5_DacClkIdelayValue_p         => v5_DacClkIdelayValue_s,
      
      ov2_FreqCntClkSel_p             => v2_FreqCntClkSel_s,
      iv16_FreqCntClkCnt_p            => v16_FreqCntClkCnt_s,
      
      -- Bus Protocol Ports mapping --
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
      S_AXI_AWREADY             => S_AXI_AWREADY);
      
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
