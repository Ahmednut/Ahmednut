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
-- File        : $Id: mi250_io_buf_data.vhd,v 1.3 2016/01/26 13:58:43 christian.dufour-bou Exp $
--------------------------------------------------------------------------------
-- Description : MI 250 ADC interface
--               Code derived from fmc108_io_buf_data.vhd (4DSP BSP)
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: mi250_io_buf_data.vhd,v $
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
-- Revision 1.4  2013/02/15 17:31:25  julien.roy
-- Modification for dynamic calibration
--
-- Revision 1.3  2012/11/15 21:44:15  julien.roy
-- Modification to the mi250 core
-- - remove fifos
-- - add pattern verification
--
-- Revision 1.1  2012/11/08 15:42:11  khalid.bensadek
-- Updated the ipcore to AXI bus version. Working version as is with 4 MMCMs that will conflect if adding Record-Playback ipcore.
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

entity mi250_io_buf_data is
generic (
  CH1_IDELAY    : integer := 0;
  CH2_IDELAY    : integer := 0
);
port (
  -- IDELAY Control
  i_RefClk200MHz_p      : in std_logic;
  i_SystemClk_p         : in  std_logic;
  iv5_AdcIdelayValue_p  : in  std_logic_vector(4 downto 0);
  i_BitSlip_p           : in  std_logic;

  -- Input pins
  ch1_p        : in  std_logic_vector(6 downto 0);
  ch1_n        : in  std_logic_vector(6 downto 0);
  ch2_p        : in  std_logic_vector(6 downto 0);
  ch2_n        : in  std_logic_vector(6 downto 0);

  -- Clock to latch data from pins
  clk_adc       : in std_logic;
  
  -- Data outputs, latched on clk_adc
  ch1_data     : out std_logic_vector(13 downto 0);
  ch2_data     : out std_logic_vector(13 downto 0)
);
end mi250_io_buf_data;

architecture mi250_io_buf_data_syn of mi250_io_buf_data is

signal ch1_ddr      : std_logic_vector(6 downto 0);  -- Double Data Rate
signal ch1_ddr_dly  : std_logic_vector(6 downto 0);  -- Double Data Rate, delayed
signal ch1_sdr      : std_logic_vector(13 downto 0); -- Single Data Rate

signal ch2_ddr      : std_logic_vector(6 downto 0);  -- Double Data Rate
signal ch2_ddr_dly  : std_logic_vector(6 downto 0);  -- Double Data Rate, delayed
signal ch2_sdr      : std_logic_vector(13 downto 0); -- Single Data Rate

attribute S: string;
attribute S of ch1_ddr_dly: signal is "TRUE";
attribute S of ch2_ddr_dly: signal is "TRUE";

begin

----------------------------------------------------------------------------------------------------
-- Channel 1 data
----------------------------------------------------------------------------------------------------


data_1: for i in 0 to 6 generate

  -- Differantial input buffer
  ibufds_inst : ibufds
  port map (
    i  => ch1_p(i),
    ib => ch1_n(i),
    o  => ch1_ddr(i)
  );
  
  data1_iodelaye1_inst : iodelaye1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I"
    )
    port map (
      DATAOUT => ch1_ddr_dly(i),
      IDATAIN => ch1_ddr(i),

      c => i_SystemClk_p,
      ce => '1',
      inc => '1',
      datain => '0',
      odatain => '0',
      clkin => '0',
      rst => '1',
      cntvaluein => iv5_AdcIdelayValue_p,
      cinvctrl => '0',
      t => '1'
    );

  -- DDR to SDR
  iddr_inst : iddr
  generic map (
    DDR_CLK_EDGE => "SAME_EDGE_PIPELINED"
  )
  port map (
    q1 => ch1_sdr(2*i),
    q2 => ch1_sdr(2*i+1),
    c  => clk_adc,
    ce => '1',
    d  => ch1_ddr_dly(i),
    r  => '0',
    s  => '0'
  );

end generate;

----------------------------------------------------------------------------------------------------
-- Channel 2 data
----------------------------------------------------------------------------------------------------

data_2: for i in 0 to 6 generate

  -- Differantial input buffer
  ibufds_inst : ibufds
  port map (
    i  => ch2_p(i),
    ib => ch2_n(i),
    o  => ch2_ddr(i)
  );
  
  data2_iodelaye1_inst : iodelaye1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I"
    )
    port map (
      DATAOUT => ch2_ddr_dly(i),
      IDATAIN => ch2_ddr(i),

      c => i_SystemClk_p,
      ce => '1',
      inc => '1',
      datain => '0',
      odatain => '0',
      clkin => '0',
      rst => '1',
      cntvaluein => iv5_AdcIdelayValue_p,
      cinvctrl => '0',
      t => '1'
    );

  -- DDR to SDR
  iddr_inst : iddr
  generic map (
    DDR_CLK_EDGE => "SAME_EDGE_PIPELINED"
  )
  port map (
    q1 => ch2_sdr(2*i),
    q2 => ch2_sdr(2*i+1),
    c  => clk_adc,
    ce => '1',
    d  => ch2_ddr_dly(i),
    r  => '0',
    s  => '0'
  );

end generate;


----------------------------------------------------------------------------------------------------
-- Bitslip process
----------------------------------------------------------------------------------------------------
process (clk_adc)
    variable v14_ch1_data_R1_v : std_logic_vector(13 downto 0);
    variable v14_ch2_data_R1_v : std_logic_vector(13 downto 0);
begin
    if rising_edge(clk_adc) then
        if i_BitSlip_p = '1' then
            for i in 0 to 6 loop
                ch1_data(2*i)   <= v14_ch1_data_R1_v(2*i+1);
                ch1_data(2*i+1) <= ch1_sdr(2*i);
                ch2_data(2*i)   <= v14_ch2_data_R1_v(2*i+1);
                ch2_data(2*i+1) <= ch2_sdr(2*i);
            end loop;
        else
            ch1_data <= ch1_sdr;
            ch2_data <= ch2_sdr;
        end if;
        
        v14_ch1_data_R1_v := ch1_sdr;
        v14_ch2_data_R1_v := ch2_sdr;
    end if;
end process;

end mi250_io_buf_data_syn;
