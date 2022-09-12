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
-- File : calibration_ramp_detection.vhd
--------------------------------------------------------------------------------
-- Description : Detect if there are calibration error when the ADC5000 is
--               configure to output a ramp.
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: calibration_ramp_detection.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2013/06/03 16:22:59  julien.roy
-- Add ramp detection for the iodelay calibration process
--
--
--------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.std_logic_unsigned.all;
    use ieee.std_logic_misc.all;

entity calibration_ramp_detection is
  port(
    i_clk_p                 : in  std_logic;
    iv80_dataChA_p          : in  std_logic_vector(79 downto 0);
    iv80_dataChB_p          : in  std_logic_vector(79 downto 0);
    iv80_dataChC_p          : in  std_logic_vector(79 downto 0);
    iv80_dataChD_p          : in  std_logic_vector(79 downto 0);
    ov10_errorChA_p         : out std_logic_vector(9 downto 0);
    ov10_errorChB_p         : out std_logic_vector(9 downto 0);
    ov10_errorChC_p         : out std_logic_vector(9 downto 0);
    ov10_errorChD_p         : out std_logic_vector(9 downto 0)
  );
end calibration_ramp_detection;

architecture arch of calibration_ramp_detection is

  signal v80_dataChA_R1_s     : std_logic_vector(79 downto 0) := (others => '0');
  signal v80_dataChB_R1_s     : std_logic_vector(79 downto 0) := (others => '0');
  signal v80_dataChC_R1_s     : std_logic_vector(79 downto 0) := (others => '0');
  signal v80_dataChD_R1_s     : std_logic_vector(79 downto 0) := (others => '0');

  signal v80_compChA_s        : std_logic_vector(79 downto 0) := (others => '0');
  signal v80_compChB_s        : std_logic_vector(79 downto 0) := (others => '0');
  signal v80_compChC_s        : std_logic_vector(79 downto 0) := (others => '0');
  signal v80_compChD_s        : std_logic_vector(79 downto 0) := (others => '0');

  signal v10_errorChA_s       : std_logic_vector(9 downto 0)  := (others => '0');
  signal v10_errorChB_s       : std_logic_vector(9 downto 0)  := (others => '0');
  signal v10_errorChC_s       : std_logic_vector(9 downto 0)  := (others => '0');
  signal v10_errorChD_s       : std_logic_vector(9 downto 0)  := (others => '0');

  signal v10_errorChACum_s    : std_logic_vector(9 downto 0)  := (others => '0');
  signal v10_errorChBCum_s    : std_logic_vector(9 downto 0)  := (others => '0');
  signal v10_errorChCCum_s    : std_logic_vector(9 downto 0)  := (others => '0');
  signal v10_errorChDCum_s    : std_logic_vector(9 downto 0)  := (others => '0');

  signal v12_counter_s        : std_logic_vector(11 downto 0) := (others => '0');
  
begin

  process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      if iv80_dataChA_p(9 downto 3) = "0000000" then
        v80_dataChA_R1_s(9 downto 0)    <= iv80_dataChA_p(9 downto 0) + 8;
        v80_dataChA_R1_s(19 downto 10)  <= iv80_dataChA_p(9 downto 0) + 9;
        v80_dataChA_R1_s(29 downto 20)  <= iv80_dataChA_p(9 downto 0) + 10;
        v80_dataChA_R1_s(39 downto 30)  <= iv80_dataChA_p(9 downto 0) + 11;
        v80_dataChA_R1_s(49 downto 40)  <= iv80_dataChA_p(9 downto 0) + 12;
        v80_dataChA_R1_s(59 downto 50)  <= iv80_dataChA_p(9 downto 0) + 13;
        v80_dataChA_R1_s(69 downto 60)  <= iv80_dataChA_p(9 downto 0) + 14;
        v80_dataChA_R1_s(79 downto 70)  <= iv80_dataChA_p(9 downto 0) + 15;
      else
        v80_dataChA_R1_s(9 downto 0)    <= v80_dataChA_R1_s(9 downto 0) + 8;
        v80_dataChA_R1_s(19 downto 10)  <= v80_dataChA_R1_s(19 downto 10) + 8;
        v80_dataChA_R1_s(29 downto 20)  <= v80_dataChA_R1_s(29 downto 20) + 8;
        v80_dataChA_R1_s(39 downto 30)  <= v80_dataChA_R1_s(39 downto 30) + 8;
        v80_dataChA_R1_s(49 downto 40)  <= v80_dataChA_R1_s(49 downto 40) + 8;
        v80_dataChA_R1_s(59 downto 50)  <= v80_dataChA_R1_s(59 downto 50) + 8;
        v80_dataChA_R1_s(69 downto 60)  <= v80_dataChA_R1_s(69 downto 60) + 8;
        v80_dataChA_R1_s(79 downto 70)  <= v80_dataChA_R1_s(79 downto 70) + 8;
      end if;

      if iv80_dataChB_p(9 downto 3) = "0000000" then
        v80_dataChB_R1_s(9 downto 0)    <= iv80_dataChB_p(9 downto 0) + 8;
        v80_dataChB_R1_s(19 downto 10)  <= iv80_dataChB_p(9 downto 0) + 9;
        v80_dataChB_R1_s(29 downto 20)  <= iv80_dataChB_p(9 downto 0) + 10;
        v80_dataChB_R1_s(39 downto 30)  <= iv80_dataChB_p(9 downto 0) + 11;
        v80_dataChB_R1_s(49 downto 40)  <= iv80_dataChB_p(9 downto 0) + 12;
        v80_dataChB_R1_s(59 downto 50)  <= iv80_dataChB_p(9 downto 0) + 13;
        v80_dataChB_R1_s(69 downto 60)  <= iv80_dataChB_p(9 downto 0) + 14;
        v80_dataChB_R1_s(79 downto 70)  <= iv80_dataChB_p(9 downto 0) + 15;
      else
        v80_dataChB_R1_s(9 downto 0)    <= v80_dataChB_R1_s(9 downto 0) + 8;
        v80_dataChB_R1_s(19 downto 10)  <= v80_dataChB_R1_s(19 downto 10) + 8;
        v80_dataChB_R1_s(29 downto 20)  <= v80_dataChB_R1_s(29 downto 20) + 8;
        v80_dataChB_R1_s(39 downto 30)  <= v80_dataChB_R1_s(39 downto 30) + 8;
        v80_dataChB_R1_s(49 downto 40)  <= v80_dataChB_R1_s(49 downto 40) + 8;
        v80_dataChB_R1_s(59 downto 50)  <= v80_dataChB_R1_s(59 downto 50) + 8;
        v80_dataChB_R1_s(69 downto 60)  <= v80_dataChB_R1_s(69 downto 60) + 8;
        v80_dataChB_R1_s(79 downto 70)  <= v80_dataChB_R1_s(79 downto 70) + 8;
      end if;

      if iv80_dataChC_p(9 downto 3) = "0000000" then
        v80_dataChC_R1_s(9 downto 0)    <= iv80_dataChC_p(9 downto 0) + 8;
        v80_dataChC_R1_s(19 downto 10)  <= iv80_dataChC_p(9 downto 0) + 9;
        v80_dataChC_R1_s(29 downto 20)  <= iv80_dataChC_p(9 downto 0) + 10;
        v80_dataChC_R1_s(39 downto 30)  <= iv80_dataChC_p(9 downto 0) + 11;
        v80_dataChC_R1_s(49 downto 40)  <= iv80_dataChC_p(9 downto 0) + 12;
        v80_dataChC_R1_s(59 downto 50)  <= iv80_dataChC_p(9 downto 0) + 13;
        v80_dataChC_R1_s(69 downto 60)  <= iv80_dataChC_p(9 downto 0) + 14;
        v80_dataChC_R1_s(79 downto 70)  <= iv80_dataChC_p(9 downto 0) + 15;
      else
        v80_dataChC_R1_s(9 downto 0)    <= v80_dataChC_R1_s(9 downto 0) + 8;
        v80_dataChC_R1_s(19 downto 10)  <= v80_dataChC_R1_s(19 downto 10) + 8;
        v80_dataChC_R1_s(29 downto 20)  <= v80_dataChC_R1_s(29 downto 20) + 8;
        v80_dataChC_R1_s(39 downto 30)  <= v80_dataChC_R1_s(39 downto 30) + 8;
        v80_dataChC_R1_s(49 downto 40)  <= v80_dataChC_R1_s(49 downto 40) + 8;
        v80_dataChC_R1_s(59 downto 50)  <= v80_dataChC_R1_s(59 downto 50) + 8;
        v80_dataChC_R1_s(69 downto 60)  <= v80_dataChC_R1_s(69 downto 60) + 8;
        v80_dataChC_R1_s(79 downto 70)  <= v80_dataChC_R1_s(79 downto 70) + 8;
      end if;
      
      if iv80_dataChD_p(9 downto 3) = "0000000" then
        v80_dataChD_R1_s(9 downto 0)    <= iv80_dataChD_p(9 downto 0) + 8;
        v80_dataChD_R1_s(19 downto 10)  <= iv80_dataChD_p(9 downto 0) + 9;
        v80_dataChD_R1_s(29 downto 20)  <= iv80_dataChD_p(9 downto 0) + 10;
        v80_dataChD_R1_s(39 downto 30)  <= iv80_dataChD_p(9 downto 0) + 11;
        v80_dataChD_R1_s(49 downto 40)  <= iv80_dataChD_p(9 downto 0) + 12;
        v80_dataChD_R1_s(59 downto 50)  <= iv80_dataChD_p(9 downto 0) + 13;
        v80_dataChD_R1_s(69 downto 60)  <= iv80_dataChD_p(9 downto 0) + 14;
        v80_dataChD_R1_s(79 downto 70)  <= iv80_dataChD_p(9 downto 0) + 15;
      else
        v80_dataChD_R1_s(9 downto 0)    <= v80_dataChD_R1_s(9 downto 0) + 8;
        v80_dataChD_R1_s(19 downto 10)  <= v80_dataChD_R1_s(19 downto 10) + 8;
        v80_dataChD_R1_s(29 downto 20)  <= v80_dataChD_R1_s(29 downto 20) + 8;
        v80_dataChD_R1_s(39 downto 30)  <= v80_dataChD_R1_s(39 downto 30) + 8;
        v80_dataChD_R1_s(49 downto 40)  <= v80_dataChD_R1_s(49 downto 40) + 8;
        v80_dataChD_R1_s(59 downto 50)  <= v80_dataChD_R1_s(59 downto 50) + 8;
        v80_dataChD_R1_s(69 downto 60)  <= v80_dataChD_R1_s(69 downto 60) + 8;
        v80_dataChD_R1_s(79 downto 70)  <= v80_dataChD_R1_s(79 downto 70) + 8;
      end if;
      
    end if;
  end process;

  process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      v80_compChA_s <= v80_dataChA_R1_s xor iv80_dataChA_p;
      v80_compChB_s <= v80_dataChB_R1_s xor iv80_dataChB_p;
      v80_compChC_s <= v80_dataChC_R1_s xor iv80_dataChC_p;
      v80_compChD_s <= v80_dataChD_R1_s xor iv80_dataChD_p;
    end if;
  end process;
  
  Sample_bits: for b in 0 to 9 generate
    v10_errorChA_s(b) <= v80_compChA_s(b) or v80_compChA_s(10+b)
      or v80_compChA_s(20+b) or v80_compChA_s(30+b)
      or v80_compChA_s(40+b) or v80_compChA_s(50+b)
      or v80_compChA_s(60+b) or v80_compChA_s(70+b);
    v10_errorChB_s(b) <= v80_compChB_s(b) or v80_compChB_s(10+b)
      or v80_compChB_s(20+b) or v80_compChB_s(30+b)
      or v80_compChB_s(40+b) or v80_compChB_s(50+b)
      or v80_compChB_s(60+b) or v80_compChB_s(70+b);
    v10_errorChC_s(b) <= v80_compChC_s(b) or v80_compChC_s(10+b)
      or v80_compChC_s(20+b) or v80_compChC_s(30+b)
      or v80_compChC_s(40+b) or v80_compChC_s(50+b)
      or v80_compChC_s(60+b) or v80_compChC_s(70+b);
    v10_errorChD_s(b) <= v80_compChD_s(b) or v80_compChD_s(10+b)
      or v80_compChD_s(20+b) or v80_compChD_s(30+b)
      or v80_compChD_s(40+b) or v80_compChD_s(50+b)
      or v80_compChD_s(60+b) or v80_compChD_s(70+b);
  end generate Sample_bits;

  process(i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      v12_counter_s <= v12_counter_s + 1;

      if v12_counter_s = "000000000000" then
      
        ov10_errorChA_p <= v10_errorChACum_s;
        ov10_errorChB_p <= v10_errorChBCum_s;
        ov10_errorChC_p <= v10_errorChCCum_s;
        ov10_errorChD_p <= v10_errorChDCum_s;

        v10_errorChACum_s <= (others => '0');
        v10_errorChBCum_s <= (others => '0');
        v10_errorChCCum_s <= (others => '0');
        v10_errorChDCum_s <= (others => '0');

      else
      
        v10_errorChACum_s <= v10_errorChACum_s or v10_errorChA_s;
        v10_errorChBCum_s <= v10_errorChBCum_s or v10_errorChB_s;
        v10_errorChCCum_s <= v10_errorChCCum_s or v10_errorChC_s;
        v10_errorChDCum_s <= v10_errorChDCum_s or v10_errorChD_s;

      end if;

    end if;
  end process;


end arch;