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
-- File        : $Id: led_manager.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Led Manager for ADAC250
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2009 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/10/06
-- $Log: led_manager.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
--
-- Revision 1.2  2010/07/29 14:22:33  francois.blackburn
-- add another dds
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.2  2010/02/10 15:02:03  patrick.gilbert
-- working ADC DAC PLL
--
-- Revision 1.1  2010/01/14 22:48:56  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/11/06 15:35:58  karim.ouadid
--------------------------------------------------------------------------------


library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

library unisim;
  use unisim.vcomponents.all;



entity LedManager is
port (
    i_clk_p           : in std_logic;
    i_rst_p           : in std_logic;
    i_AdcClkLock_p    : in std_logic;
    iv2_AdcRun_p      : in std_logic_vector(1 downto 0);
    iv2_AdcStart_p    : in std_logic_vector(1 downto 0);
    iv2_AdcOvr_p      : in std_logic_vector(1 downto 0);
    iv2_AdcUdr_p      : in std_logic_vector(1 downto 0);
    i_DacClkLock_p    : in std_logic;
    i_PLLStatus_p     : in std_logic;
    o_AdcALedRed_p    : out std_logic;
    o_AdcALedGreen_p  : out std_logic;
    o_AdcBLedRed_p    : out std_logic;
    o_AdcBLedGreen_p  : out std_logic;
    o_DacLedRed_p     : out std_logic;
    o_DacLedGreen_p   : out std_logic;
    o_PllLedRed_p     : out std_logic;
    o_PllLedGreen_p   : out std_logic
     );
end entity LedManager;

architecture arch of LedManager is

signal AdcALedRed_s         : std_logic;
signal AdcALedGreen_s       : std_logic;
signal AdcBLedRed_s         : std_logic;
signal AdcBLedGreen_s       : std_logic;
signal DacLedRed_s          : std_logic;
signal DacLedGreen_s        : std_logic;

signal PllRedStatus_s       : std_logic;
begin


  --  ADC LED : Green if ADC RUN
  --            RED if ADC clock Unlock
  --            Orange on Over/Under RUN 
  AdcALed_l : process(i_clk_p)
  begin
    if rising_edge( i_clk_p) then
      if (i_rst_p ='1') then
        AdcALedRed_s     <= '0';
        AdcALedGreen_s   <= '0';
      elsif (iv2_AdcStart_p(0) ='1') then
        if (iv2_AdcRun_p(0)='1') then
          AdcALedGreen_s   <= '1';
          if (i_AdcClkLock_p = '1') then
            AdcALedRed_s  <= '0';
            if (iv2_AdcOvr_p(0) = '0' AND iv2_AdcUdr_p(0) = '0') then
              AdcALedRed_s  <= '0';
            else
              AdcALedRed_s  <= '1';
            end if;
          else
            AdcALedRed_s     <= '1';
            AdcALedGreen_s   <= '0';
          end if;
        end if;
      else
        AdcALedGreen_s   <= '0';
        AdcALedRed_s     <= '0';
      end if;
    end if;
  end process AdcALed_l;

  --  ADC LED : Green if ADC RUN
  --            RED if ADC clock Unlock
  --            Orange on Over/Under RUN 
  AdcBLed_l : process(i_clk_p)
  begin
    if rising_edge( i_clk_p) then
      if (i_rst_p ='1') then
         AdcBLedRed_s     <= '0';
         AdcBLedGreen_s   <= '0';
      elsif (iv2_AdcStart_p(1) ='1') then
        if (iv2_AdcRun_p(1)='1') then
          AdcBLedGreen_s   <= '1';
          if (i_AdcClkLock_p = '1') then
            AdcBLedRed_s  <= '0';
            if (iv2_AdcOvr_p(1) = '0' AND iv2_AdcUdr_p(1) = '0') then
              AdcBLedRed_s  <= '0';
            else
              AdcBLedRed_s  <= '1';
            end if;
          else
            AdcBLedRed_s     <= '1';
            AdcBLedGreen_s   <= '0';
          end if;
        end if;
      else
        AdcBLedGreen_s   <= '0';
        AdcBLedRed_s     <= '0';
      end if;
    end if;
  end process AdcBLed_l;

--DacLed_l : process(i_clk_p)
--begin
--if rising_edge( i_clk_p) then
--    if (i_rst_p ='1') then
--       DacLedRed_s     <= '0';
--       DacLedGreen_s   <= '0';
--    elsif (i_DacClkLock_p ='1') then
--            if (i_AdcRun_p(1)='1') then
--                AdcBLedGreen_s   <= '1';
--                if (i_AdcClkLock_p = '1') then
--                  AdcBLedRed_s  <= '0';
--                    if (i_AdcOvr_p(1) = '0' AND i_AdcUdr_p(1) = '0') then
--                       AdcBLedRed_s  <= '0';
--                    else
--                       AdcBLedRed_s  <= '1';
--                    end if;
--                else
--                    AdcBLedRed_s     <= '1';
--                    AdcBLedGreen_s   <= '0';
--                end if;
--            end if;
--   else
--            AdcBLedGreen_s   <= '0';
--            AdcBLedRed_s     <= '0';
--   end if;
--end if;
--end process DacLed_l;



DacLedRed_s    <=  not i_DacClkLock_p;
DacLedGreen_s  <=  i_DacClkLock_p;

OBUFDACRED_inst : OBUF
  generic map (
    DRIVE => 12,
    IOSTANDARD => "LVCMOS25",
    SLEW => "SLOW")
  port map (
    O => o_DacLedRed_p,     -- Buffer output (connect directly to top-level port)
    I => DacLedRed_s      -- Buffer input
  );

OBUFDACGREEN_inst : OBUF
  generic map (
    DRIVE => 12,
    IOSTANDARD => "LVCMOS25",
    SLEW => "SLOW")
  port map (
    O => o_DacLedGreen_p,     -- Buffer output (connect directly to top-level port)
    I => DacLedGreen_s      -- Buffer input
  );

--o_DacLedRed_p     <=  DacLedRed_s;
--o_DacLedGreen_p   <=  DacLedGreen_s;

OBUFPLLRED_inst : OBUF
  generic map (
    DRIVE => 12,
    IOSTANDARD => "LVCMOS25",
    SLEW => "SLOW")
  port map (
    O => o_PllLedRed_p,     -- Buffer output (connect directly to top-level port)
    I => PllRedStatus_s      -- Buffer input
  );

OBUFPLLGREEN_inst : OBUF
  generic map (
    DRIVE => 12,
    IOSTANDARD => "LVCMOS25",
    SLEW => "SLOW")
  port map (
    O => o_PllLedGreen_p,     -- Buffer output (connect directly to top-level port)
    I => i_PLLStatus_p      -- Buffer input
  );
PllRedStatus_s <=  not i_PLLStatus_p;
--o_PllLedRed_p     <= PllRedStatus_s;
--o_PllLedGreen_p   <= i_PLLStatus_p;

OBUFADCARED_inst : OBUF
  generic map (
    DRIVE => 12,
    IOSTANDARD => "LVCMOS25",
    SLEW => "SLOW")
  port map (
    O => o_AdcALedRed_p,     -- Buffer output (connect directly to top-level port)
    I => AdcALedRed_s      -- Buffer input
  );

OBUFADCAGREEN_inst : OBUF
  generic map (
    DRIVE => 12,
    IOSTANDARD => "LVCMOS25",
    SLEW => "SLOW")
  port map (
    O => o_AdcALedGreen_p,     -- Buffer output (connect directly to top-level port)
    I => AdcALedGreen_s      -- Buffer input
  );
--o_AdcALedRed_p    <= AdcALedRed_s;
--o_AdcALedGreen_p  <= AdcALedGreen_s;


OBUFADCBRED_inst : OBUF
  generic map (
    DRIVE => 12,
    IOSTANDARD => "LVCMOS25",
    SLEW => "SLOW")
  port map (
    O => o_AdcBLedRed_p,     -- Buffer output (connect directly to top-level port)
    I => AdcBLedRed_s      -- Buffer input
  );

OBUFADCBGREEN_inst : OBUF
  generic map (
    DRIVE => 12,
    IOSTANDARD => "LVCMOS25",
    SLEW => "SLOW")
  port map (
    O => o_AdcBLedGreen_p,     -- Buffer output (connect directly to top-level port)
    I => AdcBLedGreen_s      -- Buffer input
  );
--o_AdcBLedRed_p    <= AdcBLedRed_s;
--o_AdcBLedGreen_p  <= AdcBLedGreen_s;

end arch;
