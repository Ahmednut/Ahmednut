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
-- File : ntq_ind_reg.vhd
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

entity ntq_ind_reg is
  generic
  (
    C_REG0_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG1_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG2_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG3_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG4_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG5_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG6_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG7_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG8_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG9_DEFAULT_VALUE    : std_logic_vector(31 downto 0) := (others => '0');
    C_REG10_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG11_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG12_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG13_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG14_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG15_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG16_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG17_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG18_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG19_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG20_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG21_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG22_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG23_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG24_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG25_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG26_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG27_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG28_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG29_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG30_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0');
    C_REG31_DEFAULT_VALUE   : std_logic_vector(31 downto 0) := (others => '0')
  );
  port
  (

    -- User ports
    i_Clk_p         : in std_logic;
    i_Rst_p         : in std_logic;

    -- Indirect addressing interface
    --
    i_IndWrEn_p      : in  std_logic;                        -- Indirect addressing write enable
    iv5_IndRegIdx_p  : in std_logic_vector(4 downto 0);      -- Indirect addressing reg index
    iv32_IndWrReg_p  : in std_logic_vector(31 downto 0);     -- Indirect addressing write register
    ov32_IndRdReg_p  : out std_logic_vector(31 downto 0);    -- Indirect addressing read register

    iv32_Reg0_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg1_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg2_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg3_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg4_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg5_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg6_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg7_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg8_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg9_p        : in  std_logic_vector(31 downto 0);
    iv32_Reg10_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg11_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg12_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg13_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg14_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg15_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg16_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg17_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg18_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg19_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg20_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg21_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg22_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg23_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg24_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg25_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg26_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg27_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg28_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg29_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg30_p       : in  std_logic_vector(31 downto 0);
    iv32_Reg31_p       : in  std_logic_vector(31 downto 0);

    ov32_Reg0_p        : out std_logic_vector(31 downto 0);
    ov32_Reg1_p        : out std_logic_vector(31 downto 0);
    ov32_Reg2_p        : out std_logic_vector(31 downto 0);
    ov32_Reg3_p        : out std_logic_vector(31 downto 0);
    ov32_Reg4_p        : out std_logic_vector(31 downto 0);
    ov32_Reg5_p        : out std_logic_vector(31 downto 0);
    ov32_Reg6_p        : out std_logic_vector(31 downto 0);
    ov32_Reg7_p        : out std_logic_vector(31 downto 0);
    ov32_Reg8_p        : out std_logic_vector(31 downto 0);
    ov32_Reg9_p        : out std_logic_vector(31 downto 0);
    ov32_Reg10_p       : out std_logic_vector(31 downto 0);
    ov32_Reg11_p       : out std_logic_vector(31 downto 0);
    ov32_Reg12_p       : out std_logic_vector(31 downto 0);
    ov32_Reg13_p       : out std_logic_vector(31 downto 0);
    ov32_Reg14_p       : out std_logic_vector(31 downto 0);
    ov32_Reg15_p       : out std_logic_vector(31 downto 0);
    ov32_Reg16_p       : out std_logic_vector(31 downto 0);
    ov32_Reg17_p       : out std_logic_vector(31 downto 0);
    ov32_Reg18_p       : out std_logic_vector(31 downto 0);
    ov32_Reg19_p       : out std_logic_vector(31 downto 0);
    ov32_Reg20_p       : out std_logic_vector(31 downto 0);
    ov32_Reg21_p       : out std_logic_vector(31 downto 0);
    ov32_Reg22_p       : out std_logic_vector(31 downto 0);
    ov32_Reg23_p       : out std_logic_vector(31 downto 0);
    ov32_Reg24_p       : out std_logic_vector(31 downto 0);
    ov32_Reg25_p       : out std_logic_vector(31 downto 0);
    ov32_Reg26_p       : out std_logic_vector(31 downto 0);
    ov32_Reg27_p       : out std_logic_vector(31 downto 0);
    ov32_Reg28_p       : out std_logic_vector(31 downto 0);
    ov32_Reg29_p       : out std_logic_vector(31 downto 0);
    ov32_Reg30_p       : out std_logic_vector(31 downto 0);
    ov32_Reg31_p       : out std_logic_vector(31 downto 0)

  );
end entity ntq_ind_reg;

architecture rtl of ntq_ind_reg is

  -- Regiters
  --

  -- Indirect addressing interface
  --
  signal IndWrEn_s                  : std_logic                     := '0';
  signal v5_IndRegIdx_s             : std_logic_vector(4 downto 0)  := (others => '0');
  signal v32_IndWrReg_s             : std_logic_vector(31 downto 0) := (others => '0');
  signal v32_IndRdReg_s             : std_logic_vector(31 downto 0) := (others => '0');


  -- Keep constraints (timing constraints outside this code on these signals)
  --
  attribute keep                        : boolean;
  attribute keep of i_IndWrEn_p         : signal is true;
  attribute keep of iv5_IndRegIdx_p     : signal is true;
  attribute keep of iv32_IndWrReg_p     : signal is true;

begin

  pipeline_input:process(i_Clk_p)
  begin
    if rising_edge(i_Clk_p) then
      IndWrEn_s                         <= i_IndWrEn_p;
      v5_IndRegIdx_s                    <= iv5_IndRegIdx_p;
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
  IndessingWrite_l : process(i_Rst_p, i_Clk_p)
  begin

    if i_Rst_p = '1' then
        ov32_Reg0_p  <= C_REG0_DEFAULT_VALUE;
        ov32_Reg1_p  <= C_REG1_DEFAULT_VALUE;
        ov32_Reg2_p  <= C_REG2_DEFAULT_VALUE;
        ov32_Reg3_p  <= C_REG3_DEFAULT_VALUE;
        ov32_Reg4_p  <= C_REG4_DEFAULT_VALUE;
        ov32_Reg5_p  <= C_REG5_DEFAULT_VALUE;
        ov32_Reg6_p  <= C_REG6_DEFAULT_VALUE;
        ov32_Reg7_p  <= C_REG7_DEFAULT_VALUE;
        ov32_Reg8_p  <= C_REG8_DEFAULT_VALUE;
        ov32_Reg9_p  <= C_REG9_DEFAULT_VALUE;
        ov32_Reg10_p <= C_REG10_DEFAULT_VALUE;
        ov32_Reg11_p <= C_REG11_DEFAULT_VALUE;
        ov32_Reg12_p <= C_REG12_DEFAULT_VALUE;
        ov32_Reg13_p <= C_REG13_DEFAULT_VALUE;
        ov32_Reg14_p <= C_REG14_DEFAULT_VALUE;
        ov32_Reg15_p <= C_REG15_DEFAULT_VALUE;
        ov32_Reg16_p <= C_REG16_DEFAULT_VALUE;
        ov32_Reg17_p <= C_REG17_DEFAULT_VALUE;
        ov32_Reg18_p <= C_REG18_DEFAULT_VALUE;
        ov32_Reg19_p <= C_REG19_DEFAULT_VALUE;
        ov32_Reg20_p <= C_REG20_DEFAULT_VALUE;
        ov32_Reg21_p <= C_REG21_DEFAULT_VALUE;
        ov32_Reg22_p <= C_REG22_DEFAULT_VALUE;
        ov32_Reg23_p <= C_REG23_DEFAULT_VALUE;
        ov32_Reg24_p <= C_REG24_DEFAULT_VALUE;
        ov32_Reg25_p <= C_REG25_DEFAULT_VALUE;
        ov32_Reg26_p <= C_REG26_DEFAULT_VALUE;
        ov32_Reg27_p <= C_REG27_DEFAULT_VALUE;
        ov32_Reg28_p <= C_REG28_DEFAULT_VALUE;
        ov32_Reg29_p <= C_REG29_DEFAULT_VALUE;
        ov32_Reg30_p <= C_REG30_DEFAULT_VALUE;
        ov32_Reg31_p <= C_REG31_DEFAULT_VALUE;

    elsif rising_edge(i_Clk_p) then

      if (IndWrEn_s = '1') then

        case v5_IndRegIdx_s is

          when "00000" => ov32_Reg0_p      <= v32_IndWrReg_s;
          when "00001" => ov32_Reg1_p      <= v32_IndWrReg_s;
          when "00010" => ov32_Reg2_p      <= v32_IndWrReg_s;
          when "00011" => ov32_Reg3_p      <= v32_IndWrReg_s;
          when "00100" => ov32_Reg4_p      <= v32_IndWrReg_s;
          when "00101" => ov32_Reg5_p      <= v32_IndWrReg_s;
          when "00110" => ov32_Reg6_p      <= v32_IndWrReg_s;
          when "00111" => ov32_Reg7_p      <= v32_IndWrReg_s;
          when "01000" => ov32_Reg8_p      <= v32_IndWrReg_s;
          when "01001" => ov32_Reg9_p      <= v32_IndWrReg_s;
          when "01010" => ov32_Reg10_p     <= v32_IndWrReg_s;
          when "01011" => ov32_Reg11_p     <= v32_IndWrReg_s;
          when "01100" => ov32_Reg12_p     <= v32_IndWrReg_s;
          when "01101" => ov32_Reg13_p     <= v32_IndWrReg_s;
          when "01110" => ov32_Reg14_p     <= v32_IndWrReg_s;
          when "01111" => ov32_Reg15_p     <= v32_IndWrReg_s;
          when "10000" => ov32_Reg16_p     <= v32_IndWrReg_s;
          when "10001" => ov32_Reg17_p     <= v32_IndWrReg_s;
          when "10010" => ov32_Reg18_p     <= v32_IndWrReg_s;
          when "10011" => ov32_Reg19_p     <= v32_IndWrReg_s;
          when "10100" => ov32_Reg20_p     <= v32_IndWrReg_s;
          when "10101" => ov32_Reg21_p     <= v32_IndWrReg_s;
          when "10110" => ov32_Reg22_p     <= v32_IndWrReg_s;
          when "10111" => ov32_Reg23_p     <= v32_IndWrReg_s;
          when "11000" => ov32_Reg24_p     <= v32_IndWrReg_s;
          when "11001" => ov32_Reg25_p     <= v32_IndWrReg_s;
          when "11010" => ov32_Reg26_p     <= v32_IndWrReg_s;
          when "11011" => ov32_Reg27_p     <= v32_IndWrReg_s;
          when "11100" => ov32_Reg28_p     <= v32_IndWrReg_s;
          when "11101" => ov32_Reg29_p     <= v32_IndWrReg_s;
          when "11110" => ov32_Reg30_p     <= v32_IndWrReg_s;
          when "11111" => ov32_Reg31_p     <= v32_IndWrReg_s;

          when others =>

            -- Read only or reserved registers
            --
            null;

        end case;
      end if;

    end if;
  end process IndessingWrite_l;

  -- Read interface : Select register
  --
  IndessingRead_l : process(v5_IndRegIdx_s, iv32_Reg0_p, iv32_Reg1_p, iv32_Reg2_p, iv32_Reg3_p,
      iv32_Reg4_p, iv32_Reg5_p, iv32_Reg6_p, iv32_Reg7_p, iv32_Reg8_p, iv32_Reg9_p, iv32_Reg10_p,
      iv32_Reg11_p, iv32_Reg12_p, iv32_Reg13_p, iv32_Reg14_p, iv32_Reg15_p, iv32_Reg16_p, iv32_Reg17_p,
      iv32_Reg18_p, iv32_Reg19_p, iv32_Reg20_p, iv32_Reg21_p, iv32_Reg22_p, iv32_Reg23_p, iv32_Reg24_p,
      iv32_Reg25_p, iv32_Reg26_p, iv32_Reg27_p, iv32_Reg28_p, iv32_Reg29_p, iv32_Reg30_p, iv32_Reg31_p)
  begin

    case v5_IndRegIdx_s is

      when "00000" =>  v32_IndRdReg_s     <= iv32_Reg0_p;
      when "00001" =>  v32_IndRdReg_s     <= iv32_Reg1_p;
      when "00010" =>  v32_IndRdReg_s     <= iv32_Reg2_p;
      when "00011" =>  v32_IndRdReg_s     <= iv32_Reg3_p;
      when "00100" =>  v32_IndRdReg_s     <= iv32_Reg4_p;
      when "00101" =>  v32_IndRdReg_s     <= iv32_Reg5_p;
      when "00110" =>  v32_IndRdReg_s     <= iv32_Reg6_p;
      when "00111" =>  v32_IndRdReg_s     <= iv32_Reg7_p;
      when "01000" =>  v32_IndRdReg_s     <= iv32_Reg8_p;
      when "01001" =>  v32_IndRdReg_s     <= iv32_Reg9_p;
      when "01010" =>  v32_IndRdReg_s     <= iv32_Reg10_p;
      when "01011" =>  v32_IndRdReg_s     <= iv32_Reg11_p;
      when "01100" =>  v32_IndRdReg_s     <= iv32_Reg12_p;
      when "01101" =>  v32_IndRdReg_s     <= iv32_Reg13_p;
      when "01110" =>  v32_IndRdReg_s     <= iv32_Reg14_p;
      when "01111" =>  v32_IndRdReg_s     <= iv32_Reg15_p;
      when "10000" =>  v32_IndRdReg_s     <= iv32_Reg16_p;
      when "10001" =>  v32_IndRdReg_s     <= iv32_Reg17_p;
      when "10010" =>  v32_IndRdReg_s     <= iv32_Reg18_p;
      when "10011" =>  v32_IndRdReg_s     <= iv32_Reg19_p;
      when "10100" =>  v32_IndRdReg_s     <= iv32_Reg20_p;
      when "10101" =>  v32_IndRdReg_s     <= iv32_Reg21_p;
      when "10110" =>  v32_IndRdReg_s     <= iv32_Reg22_p;
      when "10111" =>  v32_IndRdReg_s     <= iv32_Reg23_p;
      when "11000" =>  v32_IndRdReg_s     <= iv32_Reg24_p;
      when "11001" =>  v32_IndRdReg_s     <= iv32_Reg25_p;
      when "11010" =>  v32_IndRdReg_s     <= iv32_Reg26_p;
      when "11011" =>  v32_IndRdReg_s     <= iv32_Reg27_p;
      when "11100" =>  v32_IndRdReg_s     <= iv32_Reg28_p;
      when "11101" =>  v32_IndRdReg_s     <= iv32_Reg29_p;
      when "11110" =>  v32_IndRdReg_s     <= iv32_Reg30_p;
      when "11111" =>  v32_IndRdReg_s     <= iv32_Reg31_p;

      when others =>

        v32_IndRdReg_s   <= (others => '0');

    end case;

  end process IndessingRead_l;

  ov32_IndRdReg_p   <= v32_IndRdReg_s;


end rtl;