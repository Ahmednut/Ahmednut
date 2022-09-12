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
-- File : lyt_ind_ch_reg.vhd
--------------------------------------------------------------------------------
-- Description : indirect channel register
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_misc.all;

library lyt_rtdexsync_test_v1_00_a;
use lyt_rtdexsync_test_v1_00_a.all;
use lyt_rtdexsync_test_v1_00_a.lyt_type_p.all;

entity lyt_ind_ch_reg is
  port
  (

    -- User ports
    i_Clk_p         : in std_logic;
    i_Rst_p         : in std_logic;

    -- Indirect addressing interface
    --
    i_IndWrEn_p      : in  std_logic;                        -- Indirect addressing write enable
    iv3_IndChanIdx_s : in std_logic_vector(2 downto 0);      -- Indirect addressing channel index
    iv5_IndRegIdx_s  : in std_logic_vector(4 downto 0);      -- Indirect addressing reg index
    iv32_IndWrReg_p  : in std_logic_vector(31 downto 0);     -- Indirect addressing write register
    ov32_IndRdReg_p  : out std_logic_vector(31 downto 0);    -- Indirect addressing read register

    ia8v32_Reg0_p        : in  array8_v32_t;
    ia8v32_Reg1_p        : in  array8_v32_t;
    ia8v32_Reg2_p        : in  array8_v32_t;
    ia8v32_Reg3_p        : in  array8_v32_t;
    ia8v32_Reg4_p        : in  array8_v32_t;
    ia8v32_Reg5_p        : in  array8_v32_t;
    ia8v32_Reg6_p        : in  array8_v32_t;
    ia8v32_Reg7_p        : in  array8_v32_t;
    ia8v32_Reg8_p        : in  array8_v32_t;
    ia8v32_Reg9_p        : in  array8_v32_t;
    ia8v32_Reg10_p       : in  array8_v32_t;
    ia8v32_Reg11_p       : in  array8_v32_t;
    ia8v32_Reg12_p       : in  array8_v32_t;
    ia8v32_Reg13_p       : in  array8_v32_t;
    ia8v32_Reg14_p       : in  array8_v32_t;
    ia8v32_Reg15_p       : in  array8_v32_t;
    ia8v32_Reg16_p       : in  array8_v32_t;
    ia8v32_Reg17_p       : in  array8_v32_t;
    ia8v32_Reg18_p       : in  array8_v32_t;
    ia8v32_Reg19_p       : in  array8_v32_t;
    ia8v32_Reg20_p       : in  array8_v32_t;
    ia8v32_Reg21_p       : in  array8_v32_t;
    ia8v32_Reg22_p       : in  array8_v32_t;
    ia8v32_Reg23_p       : in  array8_v32_t;
    ia8v32_Reg24_p       : in  array8_v32_t;
    ia8v32_Reg25_p       : in  array8_v32_t;
    ia8v32_Reg26_p       : in  array8_v32_t;
    ia8v32_Reg27_p       : in  array8_v32_t;
    ia8v32_Reg28_p       : in  array8_v32_t;
    ia8v32_Reg29_p       : in  array8_v32_t;
    ia8v32_Reg30_p       : in  array8_v32_t;
    ia8v32_Reg31_p       : in  array8_v32_t;

    oa8v32_Reg0_p        : out array8_v32_t;
    oa8v32_Reg1_p        : out array8_v32_t;
    oa8v32_Reg2_p        : out array8_v32_t;
    oa8v32_Reg3_p        : out array8_v32_t;
    oa8v32_Reg4_p        : out array8_v32_t;
    oa8v32_Reg5_p        : out array8_v32_t;
    oa8v32_Reg6_p        : out array8_v32_t;
    oa8v32_Reg7_p        : out array8_v32_t;
    oa8v32_Reg8_p        : out array8_v32_t;
    oa8v32_Reg9_p        : out array8_v32_t;
    oa8v32_Reg10_p       : out array8_v32_t;
    oa8v32_Reg11_p       : out array8_v32_t;
    oa8v32_Reg12_p       : out array8_v32_t;
    oa8v32_Reg13_p       : out array8_v32_t;
    oa8v32_Reg14_p       : out array8_v32_t;
    oa8v32_Reg15_p       : out array8_v32_t;
    oa8v32_Reg16_p       : out array8_v32_t;
    oa8v32_Reg17_p       : out array8_v32_t;
    oa8v32_Reg18_p       : out array8_v32_t;
    oa8v32_Reg19_p       : out array8_v32_t;
    oa8v32_Reg20_p       : out array8_v32_t;
    oa8v32_Reg21_p       : out array8_v32_t;
    oa8v32_Reg22_p       : out array8_v32_t;
    oa8v32_Reg23_p       : out array8_v32_t;
    oa8v32_Reg24_p       : out array8_v32_t;
    oa8v32_Reg25_p       : out array8_v32_t;
    oa8v32_Reg26_p       : out array8_v32_t;
    oa8v32_Reg27_p       : out array8_v32_t;
    oa8v32_Reg28_p       : out array8_v32_t;
    oa8v32_Reg29_p       : out array8_v32_t;
    oa8v32_Reg30_p       : out array8_v32_t;
    oa8v32_Reg31_p       : out array8_v32_t

  );
end entity lyt_ind_ch_reg;

architecture rtl of lyt_ind_ch_reg is

  -- Regiters
  --

  -- Indirect addressing interface
  --
  signal IndWrEn_s                  : std_logic                     := '0';
  signal v3_IndChanIdx_s            : std_logic_vector(2 downto 0)  := (others => '0');
  signal v5_IndRegIdx_s             : std_logic_vector(4 downto 0)  := (others => '0');
  signal v32_IndWrReg_s             : std_logic_vector(31 downto 0) := (others => '0');
  signal a8v32_IndRdReg_s           : array8_v32_t                  := (others => (others => '0'));


  -- Keep constraints (timing constraints outside this code on these signals)
  --
  attribute keep                        : boolean;
  attribute keep of i_IndWrEn_p         : signal is true;
  attribute keep of iv3_IndChanIdx_s    : signal is true;
  attribute keep of iv5_IndRegIdx_s     : signal is true;
  attribute keep of iv32_IndWrReg_p     : signal is true;

begin

  pipeline_input:process(i_Clk_p)
  begin
    if rising_edge(i_Clk_p) then
      IndWrEn_s                         <= i_IndWrEn_p;
      v3_IndChanIdx_s                   <= iv3_IndChanIdx_s;
      v5_IndRegIdx_s                    <= iv5_IndRegIdx_s;
      v32_IndWrReg_s                    <= iv32_IndWrReg_p;
    end if;
  end process;


  ------------------------------------------------------------
  -- Indirect addressing
  --
  --   The address is composed of the channel index (3 LSB)
  --   and the register index (2 LSB)
  --
  --   The i_IndWrEn_p signal must be set at least one cycle
  --   after the other registers
  --
  ------------------------------------------------------------

  -- Write interface
  --
  gen_IndessingWrite_l: for i in 0 to 7 generate
    IndessingWrite_l : process(i_Rst_p, i_Clk_p)
    begin

      if i_Rst_p = '1' then
          oa8v32_Reg0_p(i)  <= (others => '0');
          oa8v32_Reg1_p(i)  <= (others => '0');
          oa8v32_Reg2_p(i)  <= (others => '0');
          oa8v32_Reg3_p(i)  <= (others => '0');
          oa8v32_Reg4_p(i)  <= (others => '0');
          oa8v32_Reg5_p(i)  <= (others => '0');
          oa8v32_Reg6_p(i)  <= (others => '0');
          oa8v32_Reg7_p(i)  <= (others => '0');
          oa8v32_Reg8_p(i)  <= (others => '0');
          oa8v32_Reg9_p(i)  <= (others => '0');
          oa8v32_Reg10_p(i) <= (others => '0');
          oa8v32_Reg11_p(i) <= (others => '0');
          oa8v32_Reg12_p(i) <= (others => '0');
          oa8v32_Reg13_p(i) <= (others => '0');
          oa8v32_Reg14_p(i) <= (others => '0');
          oa8v32_Reg15_p(i) <= (others => '0');
          oa8v32_Reg16_p(i) <= (others => '0');
          oa8v32_Reg17_p(i) <= (others => '0');
          oa8v32_Reg18_p(i) <= (others => '0');
          oa8v32_Reg19_p(i) <= (others => '0');
          oa8v32_Reg20_p(i) <= (others => '0');
          oa8v32_Reg21_p(i) <= (others => '0');
          oa8v32_Reg22_p(i) <= (others => '0');
          oa8v32_Reg23_p(i) <= (others => '0');
          oa8v32_Reg24_p(i) <= (others => '0');
          oa8v32_Reg25_p(i) <= (others => '0');
          oa8v32_Reg26_p(i) <= (others => '0');
          oa8v32_Reg27_p(i) <= (others => '0');
          oa8v32_Reg28_p(i) <= (others => '0');
          oa8v32_Reg29_p(i) <= (others => '0');
          oa8v32_Reg30_p(i) <= (others => '0');
          oa8v32_Reg31_p(i) <= (others => '0');

      elsif rising_edge(i_Clk_p) then

        if (IndWrEn_s = '1' and v3_IndChanIdx_s = std_logic_vector(to_unsigned(i, 3))) then

          case v5_IndRegIdx_s is

            when "00000" => oa8v32_Reg0_p(i)      <= v32_IndWrReg_s;
            when "00001" => oa8v32_Reg1_p(i)      <= v32_IndWrReg_s;
            when "00010" => oa8v32_Reg2_p(i)      <= v32_IndWrReg_s;
            when "00011" => oa8v32_Reg3_p(i)      <= v32_IndWrReg_s;
            when "00100" => oa8v32_Reg4_p(i)      <= v32_IndWrReg_s;
            when "00101" => oa8v32_Reg5_p(i)      <= v32_IndWrReg_s;
            when "00110" => oa8v32_Reg6_p(i)      <= v32_IndWrReg_s;
            when "00111" => oa8v32_Reg7_p(i)      <= v32_IndWrReg_s;
            when "01000" => oa8v32_Reg8_p(i)      <= v32_IndWrReg_s;
            when "01001" => oa8v32_Reg9_p(i)      <= v32_IndWrReg_s;
            when "01010" => oa8v32_Reg10_p(i)     <= v32_IndWrReg_s;
            when "01011" => oa8v32_Reg11_p(i)     <= v32_IndWrReg_s;
            when "01100" => oa8v32_Reg12_p(i)     <= v32_IndWrReg_s;
            when "01101" => oa8v32_Reg13_p(i)     <= v32_IndWrReg_s;
            when "01110" => oa8v32_Reg14_p(i)     <= v32_IndWrReg_s;
            when "01111" => oa8v32_Reg15_p(i)     <= v32_IndWrReg_s;
            when "10000" => oa8v32_Reg16_p(i)     <= v32_IndWrReg_s;
            when "10001" => oa8v32_Reg17_p(i)     <= v32_IndWrReg_s;
            when "10010" => oa8v32_Reg18_p(i)     <= v32_IndWrReg_s;
            when "10011" => oa8v32_Reg19_p(i)     <= v32_IndWrReg_s;
            when "10100" => oa8v32_Reg20_p(i)     <= v32_IndWrReg_s;
            when "10101" => oa8v32_Reg21_p(i)     <= v32_IndWrReg_s;
            when "10110" => oa8v32_Reg22_p(i)     <= v32_IndWrReg_s;
            when "10111" => oa8v32_Reg23_p(i)     <= v32_IndWrReg_s;
            when "11000" => oa8v32_Reg24_p(i)     <= v32_IndWrReg_s;
            when "11001" => oa8v32_Reg25_p(i)     <= v32_IndWrReg_s;
            when "11010" => oa8v32_Reg26_p(i)     <= v32_IndWrReg_s;
            when "11011" => oa8v32_Reg27_p(i)     <= v32_IndWrReg_s;
            when "11100" => oa8v32_Reg28_p(i)     <= v32_IndWrReg_s;
            when "11101" => oa8v32_Reg29_p(i)     <= v32_IndWrReg_s;
            when "11110" => oa8v32_Reg30_p(i)     <= v32_IndWrReg_s;
            when "11111" => oa8v32_Reg31_p(i)     <= v32_IndWrReg_s;

            when others =>

              -- Read only or reserved registers
              --
              null;

          end case;
        end if;

      end if;
    end process IndessingWrite_l;
  end generate gen_IndessingWrite_l;

  -- Read interface : Select register
  --
  gen_IndessingRead_l: for i in 0 to 7 generate
    IndessingRead_l : process(v5_IndRegIdx_s, ia8v32_Reg0_p, ia8v32_Reg1_p, ia8v32_Reg2_p, ia8v32_Reg3_p,
        ia8v32_Reg4_p, ia8v32_Reg5_p, ia8v32_Reg6_p, ia8v32_Reg7_p, ia8v32_Reg8_p, ia8v32_Reg9_p, ia8v32_Reg10_p,
        ia8v32_Reg11_p, ia8v32_Reg12_p, ia8v32_Reg13_p, ia8v32_Reg14_p, ia8v32_Reg15_p, ia8v32_Reg16_p, ia8v32_Reg17_p, 
        ia8v32_Reg18_p, ia8v32_Reg19_p, ia8v32_Reg20_p, ia8v32_Reg21_p, ia8v32_Reg22_p, ia8v32_Reg23_p, ia8v32_Reg24_p, 
        ia8v32_Reg25_p, ia8v32_Reg26_p, ia8v32_Reg27_p, ia8v32_Reg28_p, ia8v32_Reg29_p, ia8v32_Reg30_p, ia8v32_Reg31_p)
    begin

      case v5_IndRegIdx_s is

        when "00000" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg0_p(i);
        when "00001" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg1_p(i);
        when "00010" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg2_p(i);
        when "00011" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg3_p(i);
        when "00100" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg4_p(i);
        when "00101" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg5_p(i);
        when "00110" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg6_p(i);
        when "00111" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg7_p(i);
        when "01000" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg8_p(i);
        when "01001" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg9_p(i);
        when "01010" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg10_p(i);
        when "01011" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg11_p(i);
        when "01100" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg12_p(i);
        when "01101" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg13_p(i);
        when "01110" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg14_p(i);
        when "01111" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg15_p(i);
        when "10000" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg16_p(i);
        when "10001" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg17_p(i);
        when "10010" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg18_p(i);
        when "10011" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg19_p(i);
        when "10100" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg20_p(i);
        when "10101" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg21_p(i);
        when "10110" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg22_p(i);
        when "10111" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg23_p(i);
        when "11000" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg24_p(i);
        when "11001" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg25_p(i);
        when "11010" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg26_p(i);
        when "11011" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg27_p(i);
        when "11100" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg28_p(i);
        when "11101" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg29_p(i);
        when "11110" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg30_p(i);
        when "11111" =>  a8v32_IndRdReg_s(i)     <= ia8v32_Reg31_p(i);

        when others =>

          a8v32_IndRdReg_s(i)   <= (others => '0');

      end case;

    end process IndessingRead_l;

  end generate gen_IndessingRead_l;


  -- Read interface : Select channel
  --
  --   (add one cycle latency for possible timing issues)
  --
  process(i_Clk_p)
  begin
    if rising_edge(i_Clk_p) then
      case v3_IndChanIdx_s is
        when "000" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(0);
        when "001" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(1);
        when "010" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(2);
        when "011" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(3);
        when "100" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(4);
        when "101" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(5);
        when "110" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(6);
        when "111" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(7);
        when others =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(0);
      end case;
    end if;
  end process;



end rtl;