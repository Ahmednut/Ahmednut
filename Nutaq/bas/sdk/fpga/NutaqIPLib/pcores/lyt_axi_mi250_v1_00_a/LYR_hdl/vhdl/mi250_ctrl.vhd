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
-- File        : $Id: mi250_ctrl.vhd,v 1.3 2016/01/26 13:58:43 christian.dufour-bou Exp $
--------------------------------------------------------------------------------
-- Description : MI 250 ADC interface
--               Code derived from fmc108_ctrl.vhd (4DSP BSP)
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: mi250_ctrl.vhd,v $
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
-- Revision 1.9  2013/05/10 13:22:56  julien.roy
-- Remove useless reset in process sensivity list
--
-- Revision 1.8  2013/05/10 13:20:50  julien.roy
-- Modify core reset to ease timing
--
-- Revision 1.7  2013/02/15 17:31:24  julien.roy
-- Modification for dynamic calibration
--
-- Revision 1.6  2012/11/20 13:58:58  khalid.bensadek
-- Commented-out the pattern error signals for now, it is unused for now and just causing timing errors.
--
-- Revision 1.5  2012/11/15 21:44:15  julien.roy
-- Modification to the mi250 core
-- - remove fifos
-- - add pattern verification
--
-- Revision 1.3  2012/11/14 19:59:45  khalid.bensadek
-- Added reset for trigger output to user logic.
--
-- Revision 1.2  2012/11/12 19:34:15  khalid.bensadek
-- Optimized design to use one MMCM only instead of 4. That way, we can use Record-playback ip core with it.
--
-- Revision 1.1  2012/11/08 15:42:11  khalid.bensadek
-- Updated the ipcore to AXI bus version. Working version as is with 4 MMCMs that will conflect if adding Record-Playback ipcore.
--
-- Revision 1.5  2011/06/17 21:16:10  jeffrey.johnson
-- Changed to AB global clock.
-- Added IDELAY for ADC clocks.
--
-- Revision 1.4  2011/05/26 20:00:33  jeffrey.johnson
-- Added ADC data clock output.
-- Changed data output to 14 bits.
-- Removed over range registers.
--
-- Revision 1.3  2011/05/25 20:36:54  jeffrey.johnson
-- Removed 16 to 64 bit FIFO.
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

entity mi250_ctrl is
port (
  i_Rst_p              : in std_logic;
  i_RefClk200MHz_p     : in std_logic;
  i_SystemClk_p        : in  std_logic;
  
  -- Control registers
  i_ChArmed_p          : in std_logic;
  
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
  
  ov2_Adc12PatternError_p      : out std_logic_vector(1 downto 0);
  ov2_Adc34PatternError_p      : out std_logic_vector(1 downto 0);
  ov2_Adc56PatternError_p      : out std_logic_vector(1 downto 0);
  ov2_Adc78PatternError_p      : out std_logic_vector(1 downto 0);
  
  -- Data output bus
  -- ADC Data outputs
  ov14_Adc1DataOut_p   : out std_logic_vector(13 downto 0);
  ov14_Adc2DataOut_p   : out std_logic_vector(13 downto 0);
  ov14_Adc3DataOut_p   : out std_logic_vector(13 downto 0);
  ov14_Adc4DataOut_p   : out std_logic_vector(13 downto 0);
  ov14_Adc5DataOut_p   : out std_logic_vector(13 downto 0);
  ov14_Adc6DataOut_p   : out std_logic_vector(13 downto 0);
  ov14_Adc7DataOut_p   : out std_logic_vector(13 downto 0);
  ov14_Adc8DataOut_p   : out std_logic_vector(13 downto 0);
  -- ADC Data valid outputs
  o_Adc1DataValid_p    : out std_logic;
  o_Adc2DataValid_p    : out std_logic;
  o_Adc3DataValid_p    : out std_logic;
  o_Adc4DataValid_p    : out std_logic;
  o_Adc5DataValid_p    : out std_logic;
  o_Adc6DataValid_p    : out std_logic;
  o_Adc7DataValid_p    : out std_logic;
  o_Adc8DataValid_p    : out std_logic;
  -- Channel A & B input
  clk_12_p             : in  std_logic;
  clk_12_n             : in  std_logic;
  ch1_p                : in  std_logic_vector(6 downto 0);
  ch1_n                : in  std_logic_vector(6 downto 0);
  ch2_p                : in  std_logic_vector(6 downto 0);
  ch2_n                : in  std_logic_vector(6 downto 0);
  -- Channel C & D input
  clk_34_p             : in  std_logic;
  clk_34_n             : in  std_logic;
  ch3_p                : in  std_logic_vector(6 downto 0);
  ch3_n                : in  std_logic_vector(6 downto 0);
  ch4_p                : in  std_logic_vector(6 downto 0);
  ch4_n                : in  std_logic_vector(6 downto 0);
  -- Channel E & F input
  clk_56_p             : in  std_logic;
  clk_56_n             : in  std_logic;
  ch5_p                : in  std_logic_vector(6 downto 0);
  ch5_n                : in  std_logic_vector(6 downto 0);
  ch6_p                : in  std_logic_vector(6 downto 0);
  ch6_n                : in  std_logic_vector(6 downto 0);
  -- Channel H & G input
  clk_78_p             : in  std_logic;
  clk_78_n             : in  std_logic;
  ch7_p                : in  std_logic_vector(6 downto 0);
  ch7_n                : in  std_logic_vector(6 downto 0);
  ch8_p                : in  std_logic_vector(6 downto 0);
  ch8_n                : in  std_logic_vector(6 downto 0);
  -- ADC Clock outputs (from BUFR)
  o_Adc12ClkBufr_p     : out std_logic;
  o_Adc34ClkBufr_p     : out std_logic;
  o_Adc56ClkBufr_p     : out std_logic;
  o_Adc78ClkBufr_p     : out std_logic;
  -- ADC Global clocks (from BUFG)
  i_AdcClkBufg_p        : in std_logic
);
end mi250_ctrl;

architecture mi250_ctrl_syn of mi250_ctrl is

----------------------------------------------------------------------------------------------------
-- Component declaration
----------------------------------------------------------------------------------------------------

component pulse2pulse
port (
   in_clk      :in std_logic;
   out_clk     :in std_logic;
   rst         :in std_logic;
   pulsein     :in std_logic;
   inbusy      :out std_logic;
   pulseout    :out std_logic
   );
end component;
----------------------------------------------------------------------------------------------------
-- Constant declaration
----------------------------------------------------------------------------------------------------

constant nb_channel :integer := 8;

----------------------------------------------------------------------------------------------------
-- Signal declaration
----------------------------------------------------------------------------------------------------

signal ch_armed                  : std_logic;
signal ch_armed_pulse            :std_logic;
signal ch_disarmed_pulse         :std_logic;
signal ch_armed_pulse_clkadc     :std_logic;
signal ch_disarmed_pulse_clkadc  :std_logic;

signal clk_adc_i              :std_logic;
signal clk_34_i               :std_logic;
signal clk_56_i               :std_logic;
signal clk_78_i               :std_logic;

signal Adc12IdelayTrig_s         : std_logic;
signal Adc34IdelayTrig_s         : std_logic;
signal Adc56IdelayTrig_s         : std_logic;
signal Adc78IdelayTrig_s         : std_logic;

signal v14_Adc1DataOut_s         : std_logic_vector(13 downto 0);
signal v14_Adc2DataOut_s         : std_logic_vector(13 downto 0);
signal v14_Adc3DataOut_s         : std_logic_vector(13 downto 0);
signal v14_Adc4DataOut_s         : std_logic_vector(13 downto 0);
signal v14_Adc5DataOut_s         : std_logic_vector(13 downto 0);
signal v14_Adc6DataOut_s         : std_logic_vector(13 downto 0);
signal v14_Adc7DataOut_s         : std_logic_vector(13 downto 0);
signal v14_Adc8DataOut_s         : std_logic_vector(13 downto 0);
signal AdcDataValid_s            : std_logic;

signal v14_Adc1DataOut0_s        : std_logic_vector(13 downto 0);
signal v14_Adc2DataOut0_s        : std_logic_vector(13 downto 0);
signal v14_Adc3DataOut0_s        : std_logic_vector(13 downto 0);
signal v14_Adc4DataOut0_s        : std_logic_vector(13 downto 0);
signal v14_Adc5DataOut0_s        : std_logic_vector(13 downto 0);
signal v14_Adc6DataOut0_s        : std_logic_vector(13 downto 0);
signal v14_Adc7DataOut0_s        : std_logic_vector(13 downto 0);
signal v14_Adc8DataOut0_s        : std_logic_vector(13 downto 0);
signal AdcDataValid0_s           : std_logic;

signal v14_Adc1DataOut1_s        : std_logic_vector(13 downto 0);
signal v14_Adc2DataOut1_s        : std_logic_vector(13 downto 0);
signal v14_Adc3DataOut1_s        : std_logic_vector(13 downto 0);
signal v14_Adc4DataOut1_s        : std_logic_vector(13 downto 0);
signal v14_Adc5DataOut1_s        : std_logic_vector(13 downto 0);
signal v14_Adc6DataOut1_s        : std_logic_vector(13 downto 0);
signal v14_Adc7DataOut1_s        : std_logic_vector(13 downto 0);
signal v14_Adc8DataOut1_s        : std_logic_vector(13 downto 0);
signal AdcDataValid1_s           : std_logic;

signal v14_Adc1DataOut2_s        : std_logic_vector(13 downto 0);
signal v14_Adc2DataOut2_s        : std_logic_vector(13 downto 0);
signal v14_Adc3DataOut2_s        : std_logic_vector(13 downto 0);
signal v14_Adc4DataOut2_s        : std_logic_vector(13 downto 0);
signal v14_Adc5DataOut2_s        : std_logic_vector(13 downto 0);
signal v14_Adc6DataOut2_s        : std_logic_vector(13 downto 0);
signal v14_Adc7DataOut2_s        : std_logic_vector(13 downto 0);
signal v14_Adc8DataOut2_s        : std_logic_vector(13 downto 0);
signal AdcDataValid2_s           : std_logic;

signal v2_Adc12PatternError_s    : std_logic_vector(1 downto 0);
signal v2_Adc34PatternError_s    : std_logic_vector(1 downto 0);
signal v2_Adc56PatternError_s    : std_logic_vector(1 downto 0);
signal v2_Adc78PatternError_s    : std_logic_vector(1 downto 0);
signal v2_Adc12PatternError_r1   : std_logic_vector(1 downto 0);
signal v2_Adc34PatternError_r1   : std_logic_vector(1 downto 0);
signal v2_Adc56PatternError_r1   : std_logic_vector(1 downto 0);
signal v2_Adc78PatternError_r1   : std_logic_vector(1 downto 0);


  attribute keep : string;
  attribute keep of v14_Adc1DataOut0_s : signal is "true";
  attribute keep of v14_Adc2DataOut0_s : signal is "true";
  attribute keep of v14_Adc3DataOut0_s : signal is "true";
  attribute keep of v14_Adc4DataOut0_s : signal is "true";
  attribute keep of v14_Adc5DataOut0_s : signal is "true";
  attribute keep of v14_Adc6DataOut0_s : signal is "true";
  attribute keep of v14_Adc7DataOut0_s : signal is "true";
  attribute keep of v14_Adc8DataOut0_s : signal is "true";
  attribute keep of AdcDataValid0_s : signal is "true";
  attribute keep of v14_Adc1DataOut1_s : signal is "true";
  attribute keep of v14_Adc2DataOut1_s : signal is "true";
  attribute keep of v14_Adc3DataOut1_s : signal is "true";
  attribute keep of v14_Adc4DataOut1_s : signal is "true";
  attribute keep of v14_Adc5DataOut1_s : signal is "true";
  attribute keep of v14_Adc6DataOut1_s : signal is "true";
  attribute keep of v14_Adc7DataOut1_s : signal is "true";
  attribute keep of v14_Adc8DataOut1_s : signal is "true";
  attribute keep of AdcDataValid1_s : signal is "true";

begin

----------------------------------------------------------------------------------------------------
-- Stellar Command Interface
----------------------------------------------------------------------------------------------------
i0_p2p:pulse2pulse
port map (
   in_clk      =>i_SystemClk_p,
   out_clk     =>i_AdcClkBufg_p,
   rst         =>i_Rst_p,
   pulsein     =>ch_armed_pulse,
   inbusy      =>open,
   pulseout    =>ch_armed_pulse_clkadc
   );
i1_p2p:pulse2pulse
port map (
   in_clk      =>i_SystemClk_p,
   out_clk     =>i_AdcClkBufg_p,
   rst         =>i_Rst_p,
   pulsein     =>ch_disarmed_pulse,
   inbusy      =>open,
   pulseout    =>ch_disarmed_pulse_clkadc
   );

----------------------------------------------------------------------------------------------------
-- ADC Channels
----------------------------------------------------------------------------------------------------

io_buf_adc_12 : entity work.mi250_io_buf_adc
port map (
  rst          => i_Rst_p,
  i_RefClk200MHz_p => i_RefClk200MHz_p,
  i_SystemClk_p    => i_SystemClk_p,
  
  iv5_Adc12IdelayValue_p      => iv5_Adc12IdelayValue_p,
  iv5_Adc34IdelayValue_p      => iv5_Adc34IdelayValue_p,
  iv5_Adc56IdelayValue_p      => iv5_Adc56IdelayValue_p,
  iv5_Adc78IdelayValue_p      => iv5_Adc78IdelayValue_p,
  iv5_Adc12ClkIdelayValue_p   => iv5_Adc12ClkIdelayValue_p,
  iv5_Adc34ClkIdelayValue_p   => iv5_Adc34ClkIdelayValue_p,
  iv5_Adc56ClkIdelayValue_p   => iv5_Adc56ClkIdelayValue_p,
  iv5_Adc78ClkIdelayValue_p   => iv5_Adc78ClkIdelayValue_p,
  
  i_Adc12BitSlip_p            => i_Adc12BitSlip_p,
  i_Adc34BitSlip_p            => i_Adc34BitSlip_p,
  i_Adc56BitSlip_p            => i_Adc56BitSlip_p,
  i_Adc78BitSlip_p            => i_Adc78BitSlip_p,
  
  ov2_Adc12PatternError_p => v2_Adc12PatternError_s,
  ov2_Adc34PatternError_p => v2_Adc34PatternError_s,
  ov2_Adc56PatternError_p => v2_Adc56PatternError_s,
  ov2_Adc78PatternError_p => v2_Adc78PatternError_s,
  
  clk12_p      => clk_12_p,
  clk12_n      => clk_12_n,
  ch1_p        => ch1_p,
  ch1_n        => ch1_n,
  ch2_p        => ch2_p,
  ch2_n        => ch2_n,
  clk34_p      => clk_34_p,
  clk34_n      => clk_34_n,
  ch3_p        => ch3_p,
  ch3_n        => ch3_n,
  ch4_p        => ch4_p,
  ch4_n        => ch4_n,
  clk56_p      => clk_56_p,
  clk56_n      => clk_56_n,
  ch5_p        => ch5_p,
  ch5_n        => ch5_n,
  ch6_p        => ch6_p,
  ch6_n        => ch6_n,
  clk78_p      => clk_78_p,
  clk78_n      => clk_78_n,
  ch7_p        => ch7_p,
  ch7_n        => ch7_n,
  ch8_p        => ch8_p,
  ch8_n        => ch8_n,

  -- ADC Clock outputs (from BUFR)
  o_Adc12ClkBufr_p  => o_Adc12ClkBufr_p,
  o_Adc34ClkBufr_p  => o_Adc34ClkBufr_p,
  o_Adc56ClkBufr_p  => o_Adc56ClkBufr_p,
  o_Adc78ClkBufr_p  => o_Adc78ClkBufr_p,
  -- ADC Global clocks (from BUFG)
  i_AdcClkBufg_p    => i_AdcClkBufg_p,

  ch1_data     => v14_Adc1DataOut_s,
  ch2_data     => v14_Adc2DataOut_s,
  ch3_data     => v14_Adc3DataOut_s,
  ch4_data     => v14_Adc4DataOut_s,
  ch5_data     => v14_Adc5DataOut_s,
  ch6_data     => v14_Adc6DataOut_s,
  ch7_data     => v14_Adc7DataOut_s,
  ch8_data     => v14_Adc8DataOut_s
);


data_buffer0_l : process (i_AdcClkBufg_p)
begin
  if (rising_edge(i_AdcClkBufg_p)) then
    v14_Adc1DataOut0_s <= v14_Adc1DataOut_s;
    v14_Adc2DataOut0_s <= v14_Adc2DataOut_s;
    v14_Adc3DataOut0_s <= v14_Adc3DataOut_s;
    v14_Adc4DataOut0_s <= v14_Adc4DataOut_s;
    v14_Adc5DataOut0_s <= v14_Adc5DataOut_s;
    v14_Adc6DataOut0_s <= v14_Adc6DataOut_s;
    v14_Adc7DataOut0_s <= v14_Adc7DataOut_s;
    v14_Adc8DataOut0_s <= v14_Adc8DataOut_s;
    AdcDataValid0_s <= AdcDataValid_s;
  end if;
end process;

data_buffer1_l : process (i_AdcClkBufg_p)
begin
  if (rising_edge(i_AdcClkBufg_p)) then
    v14_Adc1DataOut1_s <= v14_Adc1DataOut0_s;
    v14_Adc2DataOut1_s <= v14_Adc2DataOut0_s;
    v14_Adc3DataOut1_s <= v14_Adc3DataOut0_s;
    v14_Adc4DataOut1_s <= v14_Adc4DataOut0_s;
    v14_Adc5DataOut1_s <= v14_Adc5DataOut0_s;
    v14_Adc6DataOut1_s <= v14_Adc6DataOut0_s;
    v14_Adc7DataOut1_s <= v14_Adc7DataOut0_s;
    v14_Adc8DataOut1_s <= v14_Adc8DataOut0_s;
    AdcDataValid1_s <= AdcDataValid0_s;
  end if;
end process;

data_buffer2_l : process (i_AdcClkBufg_p)
begin
  if (rising_edge(i_AdcClkBufg_p)) then
    v14_Adc1DataOut2_s <= v14_Adc1DataOut1_s;
    v14_Adc2DataOut2_s <= v14_Adc2DataOut1_s;
    v14_Adc3DataOut2_s <= v14_Adc3DataOut1_s;
    v14_Adc4DataOut2_s <= v14_Adc4DataOut1_s;
    v14_Adc5DataOut2_s <= v14_Adc5DataOut1_s;
    v14_Adc6DataOut2_s <= v14_Adc6DataOut1_s;
    v14_Adc7DataOut2_s <= v14_Adc7DataOut1_s;
    v14_Adc8DataOut2_s <= v14_Adc8DataOut1_s;
    AdcDataValid2_s <= AdcDataValid1_s;
  end if;
end process;

AdcDataValid_process:process (i_Rst_p, i_AdcClkBufg_p)
begin
  if (i_Rst_p = '1') then
    AdcDataValid_s <= '0';
  elsif (rising_edge(i_AdcClkBufg_p)) then
    if (ch_armed_pulse_clkadc = '1') then
      AdcDataValid_s <= '1';
    elsif (ch_disarmed_pulse_clkadc = '1') then
      AdcDataValid_s <= '0';
    end if;
  end if;
end process;

----------------------------------------------------------------------------------------------------
-- registers control
----------------------------------------------------------------------------------------------------

process (i_Rst_p, i_SystemClk_p)
begin
  if (i_Rst_p = '1') then

    ch_armed         <= '0';

    ch_armed_pulse   <= '0';

  elsif (rising_edge(i_SystemClk_p)) then
    ch_armed <= i_ChArmed_p;

    if (i_ChArmed_p = '1' and ch_armed = '0') then
        ch_armed_pulse <= '1';
    else
         ch_armed_pulse <= '0';
    end if;

    if (i_ChArmed_p = '0' and ch_armed = '1') then -- disarm
        ch_disarmed_pulse <= '1';
    else
         ch_disarmed_pulse <= '0';
    end if;
    
    -- Pattern errors
    -- dual buffering for clock domain crossing
    v2_Adc12PatternError_r1 <= v2_Adc12PatternError_s;
    v2_Adc34PatternError_r1 <= v2_Adc34PatternError_s;
    v2_Adc56PatternError_r1 <= v2_Adc56PatternError_s;
    v2_Adc78PatternError_r1 <= v2_Adc78PatternError_s;
    
    ov2_Adc12PatternError_p <= v2_Adc12PatternError_r1;
    ov2_Adc34PatternError_p <= v2_Adc34PatternError_r1;
    ov2_Adc56PatternError_p <= v2_Adc56PatternError_r1;
    ov2_Adc78PatternError_p <= v2_Adc78PatternError_r1;

  end if;
end process;


----------------------------------------------------------------------------------------------------
-- Connect to entity
----------------------------------------------------------------------------------------------------

ov14_Adc1DataOut_p <= v14_Adc1DataOut2_s;
ov14_Adc2DataOut_p <= v14_Adc2DataOut2_s;
ov14_Adc3DataOut_p <= v14_Adc3DataOut2_s;
ov14_Adc4DataOut_p <= v14_Adc4DataOut2_s;
ov14_Adc5DataOut_p <= v14_Adc5DataOut2_s;
ov14_Adc6DataOut_p <= v14_Adc6DataOut2_s;
ov14_Adc7DataOut_p <= v14_Adc7DataOut2_s;
ov14_Adc8DataOut_p <= v14_Adc8DataOut2_s;
o_Adc1DataValid_p  <= AdcDataValid2_s;
o_Adc2DataValid_p  <= AdcDataValid2_s;
o_Adc3DataValid_p  <= AdcDataValid2_s;
o_Adc4DataValid_p  <= AdcDataValid2_s;
o_Adc5DataValid_p  <= AdcDataValid2_s;
o_Adc6DataValid_p  <= AdcDataValid2_s;
o_Adc7DataValid_p  <= AdcDataValid2_s;
o_Adc8DataValid_p  <= AdcDataValid2_s;

----------------------------------------------------------------------------------------------------
-- End
----------------------------------------------------------------------------------------------------

end mi250_ctrl_syn;
