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
-- File        : $Id: dac_interface.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
--------------------------------------------------------------------------------
-- Description : DACInterface for ADAC250
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2009 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/09/30
-- $Log: dac_interface.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.2  2014/04/14 14:29:06  julien.roy
-- Add register stage to ease timing
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
--
-- Revision 1.8  2010/11/30 20:50:53  patrick.gilbert
-- remove adc synchronous parameter
--
-- Revision 1.7  2010/10/29 15:28:56  jeffrey.johnson
-- Added DAC design clock input and DAC reference clock output.
-- Moved DAC reference clock buffer from dac_interface to adac250_wrapper.
-- Changed DAC clk IDELAY value from 0 to 10.
--
-- Revision 1.6  2010/10/27 13:07:26  patrick.gilbert
-- fix timing error... on pll status + adc flag + DAC interface
--
-- Revision 1.5  2010/09/17 15:31:08  patrick.gilbert
-- fix DAC_sync pin + pass new automatic MMCM config.
--
-- Revision 1.4  2010/09/17 15:28:28  francois.blackburn
-- fix dac inversion bug and add clkmon
--
-- Revision 1.3  2010/08/26 21:51:31  francois.blackburn
-- remove mmcm for the adc_interface to allow parameters modifications
--
-- Revision 1.2  2010/07/29 14:22:33  francois.blackburn
-- add another dds
--
-- Revision 1.1.2.2  2010/07/13 17:14:00  francois.blackburn
-- fix mmcm bug
--
-- Revision 1.1.2.1  2010/06/17 21:28:26  francois.blackburn
-- change the dcms for mmcms
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.2  2010/02/10 15:02:03  patrick.gilbert
-- working ADC DAC PLL
--
-- Revision 1.1  2010/01/14 22:49:15  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/09/30 15:35:58  karim.ouadid
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;


library unisim;
  use unisim.vcomponents.all;


entity DACInterface is

  port (
    -- Direct to IO
    ov16_DacDataP_p             : out std_logic_vector(15 downto 0);
    ov16_DacDataN_p             : out std_logic_vector(15 downto 0);
    o_DacDataClkP_p             : out std_logic;
    o_DacDataClkN_p             : out std_logic;
    o_DacSyncP_p                : out std_logic;
    o_DacSyncN_p                : out std_logic;
    -- Global Signals
    i_SystemClk_p               : in std_logic;
    i_rst_p                     : in std_logic;
    -- User Interface
    iv16_DacChA_p               : in std_logic_vector(15 downto 0);
    iv16_DacChB_p               : in std_logic_vector(15 downto 0);
    i_DacDesignClk_p            : in std_logic;
    i_DacDataSync_p             : in std_logic;
    o_DacDataClkLocked_p        : out std_logic;
    i_RefClk200MHz_p            : in std_logic;
    
    iv5_DacIdelayValue_p        : in std_logic_vector(4 downto 0);
    iv5_DacClkIdelayValue_p     : in std_logic_vector(4 downto 0)
  );
end DACInterface;

architecture arch of DACInterface is

  signal DacDataClkP_s            : std_logic;
  signal DacDataClkN_s            : std_logic;
  signal DacSyncP_s               : std_logic;
  signal DacSyncN_s               : std_logic;
  signal DacDataClkToBufg_s       : std_logic;
  signal DacDataClkFbinTommcm     : std_logic;
  signal DacDataClkFboutToBufg_s  : std_logic;
  signal DacChAChBDataSync_s     : std_logic;
  signal DacChAChBDataSyncD_s     : std_logic;
  signal DacDataClkOut_s          : std_logic;
  signal DacDataClkOutD_s         : std_logic;
  signal v16_DacChA_s             : std_logic_vector(15 downto 0);
  signal v16_DacChB_s             : std_logic_vector(15 downto 0);
  signal v16_DAcCh1Ch2_s          : std_logic_vector(15 downto 0);
  signal v16_DacDataP_s           : std_logic_vector(15 downto 0);
  signal v16_DacDataN_s           : std_logic_vector(15 downto 0);
  signal v16_DAcChAChB_s          : std_logic_vector(15 downto 0);
  signal v16_DAcChAChBDelayed_s   : std_logic_vector(15 downto 0);
  signal DacDataClkLocked_s       : std_logic;
  signal ClkStop_s                : std_logic;

  signal dcmRst_s           : std_logic;
  signal dlyRdy15_s           : std_logic;
  signal dlyRdy16_s           : std_logic;

  signal MmcmFbOutBufg_s : std_logic;
  signal MmcmFbOut_s     : std_logic;

  signal clkout0b_unused  : std_logic;
  signal clkout1_unused   : std_logic;
  signal clkout1b_unused  : std_logic;
  signal clkout2_unused   : std_logic;
  signal clkout2b_unused  : std_logic;
  signal clkout3_unused   : std_logic;
  signal clkout3b_unused  : std_logic;
  signal clkout4_unused   : std_logic;
  signal clkout5_unused   : std_logic;
  signal clkout6_unused   : std_logic;
  -- Dynamic programming unused signals
  signal do_unused        : std_logic_vector(15 downto 0);
  signal drdy_unused      : std_logic;
  -- Dynamic phase shift unused signals
  signal psdone_unused    : std_logic;
  -- Unused status signals
  signal clkfbstopped_unused : std_logic;
  signal clkinstopped_unused : std_logic;
  
  signal v16_DacChA_R1_s : std_logic_vector(15 downto 0);
  signal v16_DacChB_R1_s : std_logic_vector(15 downto 0);

begin

--------------------------------------------------------------
-- DATA output and user Clock Setup
--------------------------------------------------------------


-- dcmRst_s <= (not dlyRdy16_s) ; --(not dlyRdy15_s) or

--   mmcm_adv_inst : entity work.mmcm_calib
--   generic map
--   (
--     BANDWIDTH            => "OPTIMIZED",
--     CLKOUT4_CASCADE      => FALSE,
--     CLOCK_HOLD           => FALSE,
--     COMPENSATION         => "ZHOLD",
--     STARTUP_WAIT         => FALSE,
--     DIVCLK_DIVIDE        => 2,
--     CLKFBOUT_MULT_F      => 8.000,
--     CLKFBOUT_PHASE       => 0.000,
--     CLKFBOUT_USE_FINE_PS => FALSE,
--     CLKOUT0_DIVIDE_F     => 4.000,
--     CLKOUT0_PHASE        => 0.000,
--     CLKOUT0_DUTY_CYCLE   => 0.500,
--     CLKOUT0_USE_FINE_PS  => FALSE,
--     CLKOUT1_DIVIDE       => 20,
--     CLKOUT1_PHASE        => 0.000,
--     CLKOUT1_DUTY_CYCLE   => 0.500,
--     CLKOUT1_USE_FINE_PS  => FALSE,
--     CLKIN1_PERIOD        => 4.0,
--     REF_JITTER1          => 0.010
--   )
--   port map
--     -- Output clocks
--   (
--     CLKFBOUT          => MmcmFbOut_s,
--     CLKFBOUTB         => open,
--     CLKOUT0           => DacDataClkToBufg_s,
--     CLKOUT0B          => clkout0b_unused,
--     CLKOUT1           => DacDataClkToBufgDiv5_s,
--     CLKOUT1B          => clkout1b_unused,
--     CLKOUT2           => clkout2_unused,
--     CLKOUT2B          => clkout2b_unused,
--     CLKOUT3           => clkout3_unused,
--     CLKOUT3B          => clkout3b_unused,
--     CLKOUT4           => clkout4_unused,
--     CLKOUT5           => clkout5_unused,
--     CLKOUT6           => clkout6_unused,
--     -- Input clock control
--     CLKFBIN             => MmcmFbOutBufg_s,
--     CLKIN1              => DacDataClk_s,
--     CLKIN2              => '0',
--     -- Tied to always select the primary input clock
--     CLKINSEL            => '1',
--     -- Ports for dynamic reconfiguration
--     DADDR               => (others => '0'),
--     DCLK                => '0',
--     DEN                 => '0',
--     DI                  => (others => '0'),
--     DO                  => do_unused,
--     DRDY                => drdy_unused,
--     DWE                 => '0',
--     -- Ports for dynamic phase shift
--     PSCLK               => '0',
--     PSEN                => '0',
--     PSINCDEC            => '0',
--     PSDONE              => psdone_unused,
--     -- Other control and status signals
--     LOCKED              => DacDataClkLocked_s,
--     CLKINSTOPPED        => clkinstopped_unused,
--     CLKFBSTOPPED        => clkfbstopped_unused,
--     PWRDWN              => '0',
--     RST                 => dcmRst_s
--   );
--
--   ClkfBufg_l : BUFG
--   port map
--   (
--     O => MmcmFbOutBufg_s,
--     I => MmcmFbOut_s
--   );


--

--   BUFG_inst : BUFG
--   port map
--   (
--     O => DacDesignClk_s, -- 1-bit Clock buffer output
--     I => DacDataClkToBufg_s -- 1-bit Clock buffer input
--   );



  --BANK 16 LA
  DACDLYCTRL_B16 : IDELAYCTRL
    port map (
      REFCLK      => i_RefClk200MHz_p,
      RDY         => dlyRdy16_s,
      RST         => i_rst_p
    );

  -------------------------------------------------------------
  --DAC DATA
  -------------------------------------------------------------
  
  DAC_DATA : process(i_DacDesignClk_p)
  begin
    if (rising_edge(i_DacDesignClk_p)) then
      
      v16_DacChA_R1_s <= iv16_DacChA_p;
      v16_DacChB_R1_s <= iv16_DacChB_p;
      
    end if;
  end process;


  GenerateDacChAChB_l : for n in 0 to 15 generate
    ODDR_inst_l : ODDR
      generic map(
          DDR_CLK_EDGE => "SAME_EDGE",    -- "OPPOSITE_EDGE" or "SAME_EDGE"
          INIT         => '0',            -- Initial value for Q port (’1’ or ’0’)
          SRTYPE       => "SYNC")         -- Reset Type ("ASYNC" or "SYNC")
      port map (
          Q  => v16_DAcChAChB_s(n),       -- 1-bit DDR output
          C  => i_DacDesignClk_p,         -- 1-bit clock input
          CE => '1',                      -- 1-bit clock enable input
          D1 => v16_DacChA_R1_s(n),         -- 1-bit data input (positive edge)
          D2 => v16_DacChB_R1_s(n),         -- 1-bit data input (negative edge)
          R  => i_rst_p,                  -- 1-bit reset input
          S  => '0'                       -- 1-bit set input
      );


    IODELAYEInstDacData : IODELAYE1
      generic map (
        CINVCTRL_SEL => FALSE,
        DELAY_SRC => "O",
        HIGH_PERFORMANCE_MODE => TRUE,
        IDELAY_TYPE => "VAR_LOADABLE",
        IDELAY_VALUE => 0,
        ODELAY_TYPE => "VAR_LOADABLE",
        ODELAY_VALUE => 0,
        REFCLK_FREQUENCY => 200.0,
        SIGNAL_PATTERN => "DATA"
      )
      port map (
        CNTVALUEOUT => open,
        DATAOUT => v16_DAcChAChBDelayed_s(n),
        C => i_SystemClk_p,
        CE => '1',
        INC => '1',
        CINVCTRL => '0',
        CLKIN => '0',
        CNTVALUEIN => iv5_DacIdelayValue_p,
        DATAIN => '0',
        IDATAIN => '0',
        ODATAIN => v16_DAcChAChB_s(n),
        RST => '1',
        T => '0'
      );
      
    OBUFDS_inst_l : OBUFDS
      generic map (
        IOSTANDARD  => "LVDS_25")
      port map (
        O   => v16_DacDataP_s(n),
        OB  => v16_DacDataN_s(n),
        I   => v16_DAcChAChBDelayed_s(n)
      );
      
  end generate GenerateDacChAChB_l;

-------------------------------------------------------------
--DAC SYNC
-------------------------------------------------------------
  ODDR_instsync_l : ODDR
    generic map(
      DDR_CLK_EDGE => "SAME_EDGE",    -- "OPPOSITE_EDGE" or "SAME_EDGE"
      INIT         => '0',            -- Initial value for Q port (’1’ or ’0’)
      SRTYPE       => "SYNC")         -- Reset Type ("ASYNC" or "SYNC")
    port map (
      Q  => DacChAChBDataSync_s,     -- 1-bit DDR output
      C  => i_DacDesignClk_p,             -- 1-bit clock input
      CE => '1',                      -- 1-bit clock enable input
      D1 => i_DacDataSync_p,      -- 1-bit data input (positive edge)
      D2 => i_DacDataSync_p,      -- 1-bit data input (negative edge)
      R  => i_rst_p,                  -- 1-bit reset input
      S  => '0'                       -- 1-bit set input
    );

  IODELAYEInstDacSync : IODELAYE1
    generic map (
      CINVCTRL_SEL => FALSE,
      DELAY_SRC => "O",
      HIGH_PERFORMANCE_MODE => TRUE,
      IDELAY_TYPE => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      ODELAY_TYPE => "VAR_LOADABLE",
      ODELAY_VALUE => 0,
      REFCLK_FREQUENCY => 200.0,
      SIGNAL_PATTERN => "DATA"
    )
    port map (
      CNTVALUEOUT => open,
      DATAOUT => DacChAChBDataSyncD_s,
      C => i_SystemClk_p,
      CE => '1',
      INC => '1',
      CINVCTRL => '0',
      CLKIN => '0',
      CNTVALUEIN => iv5_DacIdelayValue_p,
      DATAIN => '0',
      IDATAIN => '0',
      ODATAIN => DacChAChBDataSync_s,
      RST => '1',
      T => '0'
    );


  OBUFInstSync_l : OBUFDS
    generic map (
      IOSTANDARD => "LVDS_25")
    port map (
      O   => DacSyncP_s,
      OB  => DacSyncN_s,
      I   => DacChAChBDataSyncD_s
    );
-------------------------------------------------------------
--DAC CLK
-------------------------------------------------------------

  ODDR_instclk_l : ODDR
    generic map(
      DDR_CLK_EDGE => "SAME_EDGE",    -- "OPPOSITE_EDGE" or "SAME_EDGE"
      INIT         => '0',            -- Initial value for Q port (’1’ or ’0’)
      SRTYPE       => "SYNC")         -- Reset Type ("ASYNC" or "SYNC")
    port map (
      Q  => DacDataClkOut_s,          -- 1-bit DDR output
      C  => i_DacDesignClk_p,             -- 1-bit clock input
      CE => '1',                      -- 1-bit clock enable input
      D1 => '1',                      -- 1-bit data input (positive edge)
      D2 => '0',                      -- 1-bit data input (negative edge)
      R  => i_rst_p,                               -- 1-bit reset input
      S  => '0'                       -- 1-bit set input
    );

  IODELAYEInstDacClk : IODELAYE1
    generic map (
      CINVCTRL_SEL => FALSE,
      DELAY_SRC => "O",
      HIGH_PERFORMANCE_MODE => TRUE,
      IDELAY_TYPE => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      ODELAY_TYPE => "VAR_LOADABLE",
      ODELAY_VALUE => 0,
      REFCLK_FREQUENCY => 200.0,
      SIGNAL_PATTERN => "CLOCK"
    )
    port map (
      CNTVALUEOUT => open,
      DATAOUT => DacDataClkOutD_s,
      C => i_SystemClk_p,
      CE => '1',
      INC => '1',
      CINVCTRL => '0',
      CLKIN => '0',
      CNTVALUEIN => iv5_DacClkIdelayValue_p,
      DATAIN => '0',
      IDATAIN => '0',
      ODATAIN => DacDataClkOut_s,
      RST => '1',
      T => '0'
    );

  OBUFInstDacDataClk_l : OBUFDS
    generic map (
      IOSTANDARD  => "LVDS_25")
    port map (
      O   => DacDataClkP_s,
      OB  => DacDataClkN_s,
      I   => DacDataClkOutD_s
    );


  DacClockMon_l: entity work.clkmon
    port map (
      i_clkRef_p => i_RefClk200MHz_p,
      i_clkIn_p  => i_DacDesignClk_p,
      o_ClkInStop_p => ClkStop_s
    );

  DacDataClkLocked_s <= not ClkStop_s;


  --Output Mapping

  o_DacDataClkP_p  <= DacDataClkP_s;
  o_DacDataClkN_p  <= DacDataClkN_s;

  o_DacSyncP_p     <= DacSyncP_s;
  o_DacSyncN_p     <= DacSyncN_s;

  ov16_DacDataP_p  <= v16_DacDataP_s;
  ov16_DacDataN_p  <= v16_DacDataN_s;

  o_DacDataClkLocked_p <= DacDataClkLocked_s;

end architecture arch;