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
-- File        : $Id: mi250_io_buf_adc.vhd,v 1.3 2016/01/26 13:58:43 christian.dufour-bou Exp $
--------------------------------------------------------------------------------
-- Description : MI 250 ADC interface
--               Code derived from fmc108_io_buf_adc.vhd (4DSP BSP)
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: mi250_io_buf_adc.vhd,v $
-- Revision 1.3  2016/01/26 13:58:43  christian.dufour-bou
-- MI250 channels names changed to 1 - 8 instead of A - H and trigger no longer managed by the MI250 core
--
-- Revision 1.2  2015/06/10 12:48:36  julien.roy
-- Add bitslip functionnality to have more possibility to find a valid calibration.
-- Add clock master/slave logic.
-- Move constraints directly inside the .tcl instead of external .ucf files.
--
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.8  2013/05/14 18:49:06  julien.roy
-- Add 1 more register stage in the calibration error detection to ease timing
--
-- Revision 1.7  2013/02/20 22:05:28  julien.roy
-- Set error detection to detect a ramp
--
-- Revision 1.6  2013/02/15 17:31:24  julien.roy
-- Modification for dynamic calibration
--
-- Revision 1.5  2012/11/26 20:46:45  julien.roy
-- Add GENERIC port for MMCM control.
-- Modify pattern error verification to ease timing.
--
-- Revision 1.4  2012/11/20 13:58:58  khalid.bensadek
-- Commented-out the pattern error signals for now, it is unused for now and just causing timing errors.
--
-- Revision 1.3  2012/11/15 21:44:15  julien.roy
-- Modification to the mi250 core
-- - remove fifos
-- - add pattern verification
--
-- Revision 1.1  2012/11/08 15:42:11  khalid.bensadek
-- Updated the ipcore to AXI bus version. Working version as is with 4 MMCMs that will conflect if adding Record-Playback ipcore.
--
-- Revision 1.4  2011/06/17 21:16:10  jeffrey.johnson
-- Changed to AB global clock.
-- Added IDELAY for ADC clocks.
--
-- Revision 1.3  2011/05/26 20:00:33  jeffrey.johnson
-- Added ADC data clock output.
-- Changed data output to 14 bits.
-- Removed over range registers.
--
-- Revision 1.2  2011/05/25 16:12:30  jeffrey.johnson
-- Moved MMCM from wrapper to clk_module.
-- Added freq counters for all clocks.
--
-- Revision 1.1  2011/05/19 18:27:52  jeffrey.johnson
-- Added ADC interface.
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_misc.all;
  use ieee.std_logic_unsigned.all;
library unisim;
  use unisim.vcomponents.all;

entity mi250_io_buf_adc is
port (
   -- Global reset
  rst          : in  std_logic;
  -- IDELAY Control
  i_RefClk200MHz_p : in std_logic;
  i_SystemClk_p    : in std_logic;
  
  iv5_Adc12IdelayValue_p        : in  std_logic_vector(4 downto 0);
  iv5_Adc34IdelayValue_p        : in  std_logic_vector(4 downto 0);
  iv5_Adc56IdelayValue_p        : in  std_logic_vector(4 downto 0);
  iv5_Adc78IdelayValue_p        : in  std_logic_vector(4 downto 0);
  iv5_Adc12ClkIdelayValue_p     : in  std_logic_vector(4 downto 0);
  iv5_Adc34ClkIdelayValue_p     : in  std_logic_vector(4 downto 0);
  iv5_Adc56ClkIdelayValue_p     : in  std_logic_vector(4 downto 0);
  iv5_Adc78ClkIdelayValue_p     : in  std_logic_vector(4 downto 0);
  
  i_Adc12BitSlip_p              : in  std_logic;
  i_Adc34BitSlip_p              : in  std_logic;
  i_Adc56BitSlip_p              : in  std_logic;
  i_Adc78BitSlip_p              : in  std_logic;
    
  ov2_Adc12PatternError_p       : out std_logic_vector(1 downto 0);
  ov2_Adc34PatternError_p       : out std_logic_vector(1 downto 0);
  ov2_Adc56PatternError_p       : out std_logic_vector(1 downto 0);
  ov2_Adc78PatternError_p       : out std_logic_vector(1 downto 0);

  -- Input pins
  clk12_p     : in  std_logic;
  clk12_n     : in  std_logic;
  ch1_p        : in  std_logic_vector(6 downto 0);
  ch1_n        : in  std_logic_vector(6 downto 0);
  ch2_p        : in  std_logic_vector(6 downto 0);
  ch2_n        : in  std_logic_vector(6 downto 0);

  clk34_p     : in  std_logic;
  clk34_n     : in  std_logic;
  ch3_p        : in  std_logic_vector(6 downto 0);
  ch3_n        : in  std_logic_vector(6 downto 0);
  ch4_p        : in  std_logic_vector(6 downto 0);
  ch4_n        : in  std_logic_vector(6 downto 0);

  clk56_p     : in  std_logic;
  clk56_n     : in  std_logic;
  ch5_p        : in  std_logic_vector(6 downto 0);
  ch5_n        : in  std_logic_vector(6 downto 0);
  ch6_p        : in  std_logic_vector(6 downto 0);
  ch6_n        : in  std_logic_vector(6 downto 0);

  clk78_p     : in  std_logic;
  clk78_n     : in  std_logic;
  ch7_p        : in  std_logic_vector(6 downto 0);
  ch7_n        : in  std_logic_vector(6 downto 0);
  ch8_p        : in  std_logic_vector(6 downto 0);
  ch8_n        : in  std_logic_vector(6 downto 0);

  -- ADC Clock outputs (from BUFR)
  o_Adc12ClkBufr_p  : out std_logic;
  o_Adc34ClkBufr_p  : out std_logic;
  o_Adc56ClkBufr_p  : out std_logic;
  o_Adc78ClkBufr_p  : out std_logic;
  
  -- ADC Global clocks (from BUFG)
  i_AdcClkBufg_p    : in std_logic;
  
  -- Output ports
  ch1_data     : out std_logic_vector(13 downto 0);
  ch2_data     : out std_logic_vector(13 downto 0);
  ch3_data     : out std_logic_vector(13 downto 0);
  ch4_data     : out std_logic_vector(13 downto 0);
  ch5_data     : out std_logic_vector(13 downto 0);
  ch6_data     : out std_logic_vector(13 downto 0);
  ch7_data     : out std_logic_vector(13 downto 0);
  ch8_data     : out std_logic_vector(13 downto 0)
);
end mi250_io_buf_adc;

architecture mi250_io_buf_adc_syn of mi250_io_buf_adc is

----------------------------------------------------------------------------------------------------
-- Signal declaration
----------------------------------------------------------------------------------------------------

-- clocks after differential input buffer
signal clk_12_i   :std_logic;
signal clk_34_i   :std_logic;
signal clk_56_i   :std_logic;
signal clk_78_i   :std_logic;

-- clk_xx_i delayed (or not)
signal clk_12_dly :std_logic;
signal clk_34_dly :std_logic;
signal clk_56_dly :std_logic;
signal clk_78_dly :std_logic;

-- clk_xx_dly through a bufr
signal clk_12_bufr  :std_logic;
signal clk_34_bufr  :std_logic;
signal clk_56_bufr  :std_logic;
signal clk_78_bufr  :std_logic;

-- channel data internal signals
signal ch1_data_i : std_logic_vector(13 downto 0);
signal ch2_data_i : std_logic_vector(13 downto 0);
signal ch3_data_i : std_logic_vector(13 downto 0);
signal ch4_data_i : std_logic_vector(13 downto 0);
signal ch5_data_i : std_logic_vector(13 downto 0);
signal ch6_data_i : std_logic_vector(13 downto 0);
signal ch7_data_i : std_logic_vector(13 downto 0);
signal ch8_data_i : std_logic_vector(13 downto 0);

begin

  --------------------------------------------------
  -- IDELAYCTRL instance
  --------------------------------------------------
  IDELAYCTRL_inst : IDELAYCTRL
  port map (
    RDY => open,
    REFCLK => i_RefClk200MHz_p,
    RST => rst
  );
  
  --------------------------------------------------
  -- mi250_io_buf_data instances
  --------------------------------------------------
  i0_iobuff : entity work.mi250_io_buf_data
    port map (
      i_RefClk200MHz_p      => i_RefClk200MHz_p,
      i_SystemClk_p         => i_SystemClk_p,
      iv5_AdcIdelayValue_p  => iv5_Adc12IdelayValue_p,
      i_BitSlip_p           => i_Adc12BitSlip_p,
      ch1_p                 => ch1_p,
      ch1_n                 => ch1_n,
      ch2_p                 => ch2_p,
      ch2_n                 => ch2_n,
      clk_adc               => i_AdcClkBufg_p,
      ch1_data              => ch1_data_i,
      ch2_data              => ch2_data_i);
      
  i1_iobuff : entity work.mi250_io_buf_data
    port map (
      i_RefClk200MHz_p      => i_RefClk200MHz_p,
      i_SystemClk_p         => i_SystemClk_p,
      iv5_AdcIdelayValue_p  => iv5_Adc34IdelayValue_p,
      i_BitSlip_p           => i_Adc34BitSlip_p,
      ch1_p                 => ch3_p,
      ch1_n                 => ch3_n,
      ch2_p                 => ch4_p,
      ch2_n                 => ch4_n,
      clk_adc               => i_AdcClkBufg_p,
      ch1_data              => ch3_data_i,
      ch2_data              => ch4_data_i);
      
  i2_iobuff : entity work.mi250_io_buf_data
    port map (
      i_RefClk200MHz_p      => i_RefClk200MHz_p,
      i_SystemClk_p         => i_SystemClk_p,
      iv5_AdcIdelayValue_p  => iv5_Adc56IdelayValue_p,
      i_BitSlip_p           => i_Adc56BitSlip_p,
      ch1_p                 => ch5_p,
      ch1_n                 => ch5_n,
      ch2_p                 => ch6_p,
      ch2_n                 => ch6_n,
      clk_adc               => i_AdcClkBufg_p,
      ch1_data              => ch5_data_i,
      ch2_data              => ch6_data_i);
      
  i3_iobuff : entity work.mi250_io_buf_data
    port map (
      i_RefClk200MHz_p      => i_RefClk200MHz_p,
      i_SystemClk_p         => i_SystemClk_p,
      iv5_AdcIdelayValue_p  => iv5_Adc78IdelayValue_p,
      i_BitSlip_p           => i_Adc78BitSlip_p,
      ch1_p                 => ch7_p,
      ch1_n                 => ch7_n,
      ch2_p                 => ch8_p,
      ch2_n                 => ch8_n,
      clk_adc               => i_AdcClkBufg_p,
      ch1_data              => ch7_data_i,
      ch2_data              => ch8_data_i);
      
      
  ----------------------------------------------------------------------------------------------------
  -- Pattern verification
  ----------------------------------------------------------------------------------------------------
  process (rst, i_AdcClkBufg_p)
    variable counter : std_logic_vector(11 downto 0);
    
    variable adc1PatternError_s : std_logic;
    variable adc2PatternError_s : std_logic;
    variable adc3PatternError_s : std_logic;
    variable adc4PatternError_s : std_logic;
    variable adc5PatternError_s : std_logic;
    variable adc6PatternError_s : std_logic;
    variable adc7PatternError_s : std_logic;
    variable adc8PatternError_s : std_logic;
    
    variable v2_adc1PatternVerify_s : std_logic_vector(1 downto 0);
    variable v2_adc2PatternVerify_s : std_logic_vector(1 downto 0);
    variable v2_adc3PatternVerify_s : std_logic_vector(1 downto 0);
    variable v2_adc4PatternVerify_s : std_logic_vector(1 downto 0);
    variable v2_adc5PatternVerify_s : std_logic_vector(1 downto 0);
    variable v2_adc6PatternVerify_s : std_logic_vector(1 downto 0);
    variable v2_adc7PatternVerify_s : std_logic_vector(1 downto 0);
    variable v2_adc8PatternVerify_s : std_logic_vector(1 downto 0);
    
    variable ch1_data_r1 : std_logic_vector(13 downto 0);
    variable ch2_data_r1 : std_logic_vector(13 downto 0);
    variable ch3_data_r1 : std_logic_vector(13 downto 0);
    variable ch4_data_r1 : std_logic_vector(13 downto 0);
    variable ch5_data_r1 : std_logic_vector(13 downto 0);
    variable ch6_data_r1 : std_logic_vector(13 downto 0);
    variable ch7_data_r1 : std_logic_vector(13 downto 0);
    variable ch8_data_r1 : std_logic_vector(13 downto 0);
    
    variable ch1_data_r2 : std_logic_vector(13 downto 0);
    variable ch2_data_r2 : std_logic_vector(13 downto 0);
    variable ch3_data_r2 : std_logic_vector(13 downto 0);
    variable ch4_data_r2 : std_logic_vector(13 downto 0);
    variable ch5_data_r2 : std_logic_vector(13 downto 0);
    variable ch6_data_r2 : std_logic_vector(13 downto 0);
    variable ch7_data_r2 : std_logic_vector(13 downto 0);
    variable ch8_data_r2 : std_logic_vector(13 downto 0);
    
  begin
    if (rst = '1') then
      counter   := (others => '0');
      
      adc1PatternError_s := '0';
      adc2PatternError_s := '0';
      adc3PatternError_s := '0';
      adc4PatternError_s := '0';
      adc5PatternError_s := '0';
      adc6PatternError_s := '0';
      adc7PatternError_s := '0';
      adc8PatternError_s := '0';
      
      ov2_Adc12PatternError_p(0)  <= '0';
      ov2_Adc12PatternError_p(1)  <= '0';
      ov2_Adc34PatternError_p(0)  <= '0';
      ov2_Adc34PatternError_p(1)  <= '0';
      ov2_Adc56PatternError_p(0)  <= '0';
      ov2_Adc56PatternError_p(1)  <= '0';
      ov2_Adc78PatternError_p(0)  <= '0';
      ov2_Adc78PatternError_p(1)  <= '0';
      
    elsif (rising_edge(i_AdcClkBufg_p)) then
      
      if counter = "000000000000" then
        ov2_Adc12PatternError_p(0)  <= adc1PatternError_s;
        ov2_Adc12PatternError_p(1)  <= adc2PatternError_s;
        ov2_Adc34PatternError_p(0)  <= adc3PatternError_s;
        ov2_Adc34PatternError_p(1)  <= adc4PatternError_s;
        ov2_Adc56PatternError_p(0)  <= adc5PatternError_s;
        ov2_Adc56PatternError_p(1)  <= adc6PatternError_s;
        ov2_Adc78PatternError_p(0)  <= adc7PatternError_s;
        ov2_Adc78PatternError_p(1)  <= adc8PatternError_s;
        
        adc1PatternError_s := '0';
        adc2PatternError_s := '0';
        adc3PatternError_s := '0';
        adc4PatternError_s := '0';
        adc5PatternError_s := '0';
        adc6PatternError_s := '0';
        adc7PatternError_s := '0';
        adc8PatternError_s := '0';
      end if;
      
      if v2_adc1PatternVerify_s = "11" then
        adc1PatternError_s := '1';
      end if;
      if v2_adc2PatternVerify_s = "11" then
        adc2PatternError_s := '1';
      end if;
      if v2_adc3PatternVerify_s = "11" then
        adc3PatternError_s := '1';
      end if;
      if v2_adc4PatternVerify_s = "11" then
        adc4PatternError_s := '1';
      end if;
      if v2_adc5PatternVerify_s = "11" then
        adc5PatternError_s := '1';
      end if;
      if v2_adc6PatternVerify_s = "11" then
        adc6PatternError_s := '1';
      end if;
      if v2_adc7PatternVerify_s = "11" then
        adc7PatternError_s := '1';
      end if;
      if v2_adc8PatternVerify_s = "11" then
        adc8PatternError_s := '1';
      end if;
      
      v2_adc1PatternVerify_s(1 downto 0) := "00";
      if (ch1_data_r1 /= ch1_data_r2) then
        v2_adc1PatternVerify_s := "11";
      end if;
      
      v2_adc2PatternVerify_s(1 downto 0) := "00";
      if (ch2_data_r1 /= ch2_data_r2) then
        v2_adc2PatternVerify_s := "11";
      end if;
      
      v2_adc3PatternVerify_s(1 downto 0) := "00";
      if (ch3_data_r1 /= ch3_data_r2) then
        v2_adc3PatternVerify_s := "11";
      end if;
      
      v2_adc4PatternVerify_s(1 downto 0) := "00";
      if (ch4_data_r1 /= ch4_data_r2) then
        v2_adc4PatternVerify_s := "11";
      end if;
      
      v2_adc5PatternVerify_s(1 downto 0) := "00";
      if (ch5_data_r1 /= ch5_data_r2) then
        v2_adc5PatternVerify_s := "11";
      end if;
      
      v2_adc6PatternVerify_s(1 downto 0) := "00";
      if (ch6_data_r1 /= ch6_data_r2) then
        v2_adc6PatternVerify_s := "11";
      end if;
      
      v2_adc7PatternVerify_s(1 downto 0) := "00";
      if (ch7_data_r1 /= ch7_data_r2) then
        v2_adc7PatternVerify_s := "11";
      end if;
      
      v2_adc8PatternVerify_s(1 downto 0) := "00";
      if (ch8_data_r1 /= ch8_data_r2) then
        v2_adc8PatternVerify_s := "11";
      end if;
    
      counter := counter + 1;
      
      ch1_data_r2 := ch1_data_r1 + 1;
      ch2_data_r2 := ch2_data_r1 + 1;
      ch3_data_r2 := ch3_data_r1 + 1;
      ch4_data_r2 := ch4_data_r1 + 1;
      ch5_data_r2 := ch5_data_r1 + 1;
      ch6_data_r2 := ch6_data_r1 + 1;
      ch7_data_r2 := ch7_data_r1 + 1;
      ch8_data_r2 := ch8_data_r1 + 1;
      
      ch1_data_r1 := ch1_data_i;
      ch2_data_r1 := ch2_data_i;
      ch3_data_r1 := ch3_data_i;
      ch4_data_r1 := ch4_data_i;
      ch5_data_r1 := ch5_data_i;
      ch6_data_r1 := ch6_data_i;
      ch7_data_r1 := ch7_data_i;
      ch8_data_r1 := ch8_data_i;
        
    end if;
  end process;
      
  -----------------------------------------------------------------------------
  -- Clock for channel 1 and 2
  -----------------------------------------------------------------------------
  -- Differential input buffer
  ibufds_inst_12 : ibufds
  port map (
    i  => clk12_p,
    ib => clk12_n,
    o  => clk_12_i
  );
  
  -- clk_12_dly <= clk_12_i;
  
  clk12_iodelaye1_inst : iodelaye1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I",
      SIGNAL_PATTERN => "CLOCK"
    )
    port map (
      DATAOUT => clk_12_dly,
      IDATAIN => clk_12_i,

      c => i_SystemClk_p,
      ce => '1',
      inc => '1',
      datain => '0',
      odatain => '0',
      clkin => '0',
      rst => '1',
      cntvaluein => iv5_Adc12ClkIdelayValue_p,
      cinvctrl => '0',
      t => '1'
    );
   
  -- BUFR for clocking logic
  bufr_inst_12 : bufr
  generic map (
    BUFR_DIVIDE => "BYPASS",
    SIM_DEVICE => "VIRTEX6"
  )
  port map (
    O => clk_12_bufr,
    CE => '1',
    CLR => '0',
    I => clk_12_dly
  );

  -----------------------------------------------------------------------------
  -- Clock for channel 3 and 4
  -----------------------------------------------------------------------------
  -- Differential input buffer
  ibufds_inst_34 : ibufds
  port map (
    i  => clk34_p,
    ib => clk34_n,
    o  => clk_34_i
  );
  
  -- clk_34_dly <= clk_34_i;
  
  clk34_iodelaye1_inst : iodelaye1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I",
      SIGNAL_PATTERN => "CLOCK"
    )
    port map (
      DATAOUT => clk_34_dly,
      IDATAIN => clk_34_i,

      c => i_SystemClk_p,
      ce => '1',
      inc => '1',
      datain => '0',
      odatain => '0',
      clkin => '0',
      rst => '1',
      cntvaluein => iv5_Adc34ClkIdelayValue_p,
      cinvctrl => '0',
      t => '1'
    );
   
  -- BUFR for clocking logic
  bufr_inst_34 : bufr
  generic map (
    BUFR_DIVIDE => "BYPASS",
    SIM_DEVICE => "VIRTEX6"
  )
  port map (
    O => clk_34_bufr,
    CE => '1',
    CLR => '0',
    I => clk_34_dly
  );

  ----------------------------------------------------------------------------------------------------
  -- Clock for channel 5 and 6
  ----------------------------------------------------------------------------------------------------
  -- Differential input buffer
  ibufds_inst_56 : ibufds
  port map (
    i  => clk56_p,
    ib => clk56_n,
    o  => clk_56_i
  );
  
  -- clk_56_dly <= clk_56_i;
  
  clk56_iodelaye1_inst : iodelaye1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I",
      SIGNAL_PATTERN => "CLOCK"
    )
    port map (
      DATAOUT => clk_56_dly,
      IDATAIN => clk_56_i,

      c => i_SystemClk_p,
      ce => '1',
      inc => '1',
      datain => '0',
      odatain => '0',
      clkin => '0',
      rst => '1',
      cntvaluein => iv5_Adc56ClkIdelayValue_p,
      cinvctrl => '0',
      t => '1'
    );
   
  -- BUFR for clocking logic
  bufr_inst_56 : bufr
  generic map (
    BUFR_DIVIDE => "BYPASS",
    SIM_DEVICE => "VIRTEX6"
  )
  port map (
    O => clk_56_bufr,
    CE => '1',
    CLR => '0',
    I => clk_56_dly
  );

  ----------------------------------------------------------------------------------------------------
  -- Clock for channel 7 and 8
  ----------------------------------------------------------------------------------------------------
  -- Differential input buffer
  ibufds_inst_78 : ibufds
  port map (
    i  => clk78_p,
    ib => clk78_n,
    o  => clk_78_i
  );
  
  
  -- clk_78_dly <= clk_78_i;
  
  clk78_iodelaye1_inst : iodelaye1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I",
      SIGNAL_PATTERN => "CLOCK"
    )
    port map (
      DATAOUT => clk_78_dly,
      IDATAIN => clk_78_i,

      c => i_SystemClk_p,
      ce => '1',
      inc => '1',
      datain => '0',
      odatain => '0',
      clkin => '0',
      rst => '1',
      cntvaluein => iv5_Adc78ClkIdelayValue_p,
      cinvctrl => '0',
      t => '1'
    );
   
  -- BUFR for clocking logic
  bufr_inst_78 : bufr
  generic map (
    BUFR_DIVIDE => "BYPASS",
    SIM_DEVICE => "VIRTEX6"
  )
  port map (
    O => clk_78_bufr,
    CE => '1',
    CLR => '0',
    I => clk_78_dly
  );

  ----------------------------------------------------------------------------------------------------
  -- Connect entity
  ----------------------------------------------------------------------------------------------------
  -- Clocks for the MMCMs
  o_Adc12ClkBufr_p <= clk_12_bufr;
  o_Adc34ClkBufr_p <= clk_34_bufr;
  o_Adc56ClkBufr_p <= clk_56_bufr;
  o_Adc78ClkBufr_p <= clk_78_bufr;
  
  -- ADC data
  ch1_data  <= ch1_data_i;
  ch2_data  <= ch2_data_i;
  ch3_data  <= ch3_data_i;
  ch4_data  <= ch4_data_i;
  ch5_data  <= ch5_data_i;
  ch6_data  <= ch6_data_i;
  ch7_data  <= ch7_data_i;
  ch8_data  <= ch8_data_i;
  
  ----------------------------------------------------------------------------------------------------
  -- End
  ----------------------------------------------------------------------------------------------------

end mi250_io_buf_adc_syn;
