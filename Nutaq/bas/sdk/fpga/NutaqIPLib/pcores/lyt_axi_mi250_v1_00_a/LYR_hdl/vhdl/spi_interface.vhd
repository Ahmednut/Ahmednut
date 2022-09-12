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
-- File        : $Id: spi_interface.vhd,v 1.1 2015/04/10 19:47:33 sqa Exp $
--------------------------------------------------------------------------------
-- Description : SPI interface.  
--               Not directly compatible with the Motorola specification.
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Christian Simard - Initial revision 23/02/2006
-- $Log: spi_interface.vhd,v $
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2012/11/08 15:42:11  khalid.bensadek
-- Updated the ipcore to AXI bus version. Working version as is with 4 MMCMs that will conflect if adding Record-Playback ipcore.
--
-- Revision 1.2  2011/05/11 13:34:45  jeffrey.johnson
-- SPI fixed.
--
-- Revision 1.1  2011/05/09 16:21:44  jeffrey.johnson
-- First commit.
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.ALL;

entity spi_interface is
--generic(
----Divider ratio of SPI CLK.  It is generated from i_clk_p.
--  Clk_div_g   : integer := 10 --Minimum is 2
--);
port(
  --clk & rst
  i_rst_p : in std_logic;
  i_clk_p : in std_logic;
  --CPU mem map signals
  i_writeaddr_p : in std_logic_vector(8 downto 0);
  i_readaddr_p  : in std_logic_vector(8 downto 0);
  i_din_p : in std_logic_vector(31 downto 0);
  o_dout_p : out std_logic_vector(31 downto 0);
  o_ack_p : out std_logic;
  o_busy_p: out std_logic;
  --SPI external signals
  o_spi_out_p : out std_logic;--MOSI
  o_spi_od_p  : out std_logic;--Output disable
  o_spi_clk_p : out std_logic;
  i_spi_in_p : in std_logic ;--MISO
  o_spi_sel_p : out std_logic_vector(7 downto 0); --Up to 8 devices can be connected
  o_spi_sel2_p : out std_logic_vector(7 downto 0)--Up to 8 others devices can be connected
);
end spi_interface;

architecture Behavioral of spi_interface is

-- Output signals
signal v32_dout_s                : std_logic_vector(31 downto 0);
signal ack_s                     : std_logic;
signal spi_clk_s                 : std_logic;
signal spi_out_s                 : std_logic;
signal spi_od_s                  : std_logic;
signal spi_sel_s                 : std_logic_vector(15 downto 0);
signal busy_s                    : std_logic;
--
--Memory map registers and signals
--0 : Control words (clk_pol|nbits|rw|start)
--1 to n : Data words
--Control word :
signal v32_cntrl_s               : std_logic_vector(31 downto 0);
--Control bit: Number of bits to transfer or read
signal v6_cntrl_nbits_s          : std_logic_vector(5 downto 0);
signal v6_cntrl_nbits_save_s          : std_logic_vector(5 downto 0);
--Control bit: Start(1) or Idle(0), this bits is cleared when current transaction is completed
signal cntrl_start_s             : std_logic;
--Word 1 to be transfer or read
signal v8_word1_s                : std_logic_vector(7 downto 0);
--Word 2 to be transfer or read
signal v8_word2_s                : std_logic_vector(7 downto 0);
--Word 3 to be transfer or read
signal v8_word3_s                : std_logic_vector(7 downto 0);
--Word 4 to be transfer or read
signal v8_word4_s                : std_logic_vector(7 downto 0);
--Word 5 to be transfer or read
signal v8_word5_s                : std_logic_vector(7 downto 0);
--Word 6 to be transfer or read
signal v8_word6_s                : std_logic_vector(7 downto 0);
--Chip select register
signal v16_cs_s                  : std_logic_vector(15 downto 0);
--clock divider register
signal Clk_div_s                 : std_logic_vector(7 downto 0);


--
--SPI state machine signals
--
type spi_enum is (
  SPI_IDLE,
  SPI_SHIFT_OUT,
  SPI_WRITE,
  SPI_SETUP_SLAVE,
  SPI_READ,
  SPI_SHIFT_IN,
  UNUSED7,
  UNUSED8
);
signal spi_state_s               :spi_enum;
--SPI shift register
signal v48_spi_sr_s              : std_logic_vector(47 downto 0);

--
-- SPI clock generation signals
--
signal cntclk_s                  : integer range 0 to 128;

signal cntrl_start_rise_s   : std_logic;

--
--  SPI clock divider value latched (avoid latching a 0).
--
signal Clk_divD_s                 : std_logic_vector(7 downto 0);
--signal writeaddr_s                : std_logic_vector(8 downto 0);
--signal readaddr_s                 : std_logic_vector(8 downto 0);

signal v32_din_s                  : std_logic_vector(31 downto 0);


begin
--writeaddr_s (8 downto 0)  <= i_writeaddr_p(0 to 8);
--readaddr_s  (8 downto 0)  <= i_readaddr_p (0 to 8);
--v32_din_s   (31 downto 0) <= i_din_p (0 to 31);

SLAVE_REG_WRITE_PROC : process( i_clk_p ) is
  begin
    if i_clk_p'event and i_clk_p = '1' then
      if i_rst_p = '1' then
        v32_cntrl_s <= (others=>'0');
        v8_word1_s  <= (others=>'0');
        v8_word2_s  <= (others=>'0');
        v8_word3_s  <= (others=>'0');
        v8_word4_s  <= (others=>'0');
        v8_word5_s  <= (others=>'0');
        v8_word6_s  <= (others=>'0');

        v16_cs_s    <= (others=>'1');
      else
           case i_writeaddr_p is
                when "000000001" =>
                    v32_cntrl_s <= i_din_p;
                when "000000010" =>
                    v8_word1_s <= i_din_p(7 downto 0);
                when "000000100" =>
                    v8_word2_s <= i_din_p(7 downto 0);
                when "000001000" =>
                    v8_word3_s <= i_din_p(7 downto 0);
                when "000010000"=>
                    v8_word4_s <= i_din_p(7 downto 0);
                when "000100000" =>
                    v8_word5_s <= i_din_p(7 downto 0);
                when "001000000" =>
                    v8_word6_s <= i_din_p(7 downto 0);
                when "010000000" =>
                    v16_cs_s   <= i_din_p(15 downto 0);
                when "100000000" =>
                    Clk_div_s   <= i_din_p(7 downto 0);
               when others => null;
        end case;
      end if;
    end if;
  end process SLAVE_REG_WRITE_PROC;


  SLAVE_REG_READ_PROC : process( cntrl_start_s, i_readaddr_p, Clk_div_s, spi_sel_s, v48_spi_sr_s, v32_cntrl_s, busy_s ) is
  begin
    v32_dout_s              <= (others => '0');
    case i_readaddr_p is
     when "000000001" => v32_dout_s              <= X"0000" & "000000" & busy_s & cntrl_start_s & v32_cntrl_s(7) & v32_cntrl_s(6 downto 1) & v32_cntrl_s(0);
     when "000000010" => v32_dout_s(7 downto 0)  <= v48_spi_sr_s(47 downto 40);
     when "000000100" => v32_dout_s(7 downto 0)  <= v48_spi_sr_s(39 downto 32);
     when "000001000" => v32_dout_s(7 downto 0)  <= v48_spi_sr_s(31 downto 24);
     when "000010000" => v32_dout_s(7 downto 0)  <= v48_spi_sr_s(23 downto 16);
     when "000100000" => v32_dout_s(7 downto 0)  <= v48_spi_sr_s(15 downto 8);
     when "001000000" => v32_dout_s(7 downto 0)  <= v48_spi_sr_s(7 downto 0);
     when "010000000" => v32_dout_s(15 downto 0) <= spi_sel_s;
     when "100000000" => v32_dout_s(7 downto 0)  <= Clk_div_s;
     when others      => v32_dout_s              <= (others => '0');
    end case;
  end process SLAVE_REG_READ_PROC;



Edge_detector_inst : entity work.Edge_detector_2T

  Port map(
    D     => v32_cntrl_s(8),
    Rising  => '1',
      Clk     => i_clk_p,
      Q     => cntrl_start_rise_s
  );

--
--  Process used to latch the clock divider ratio and to avoid set at 0
--

U_Check_ClkDivValue_l : process (i_clk_p)
begin
if rising_edge(i_clk_p) then
  if (i_rst_p = '1') then
     Clk_divD_s <= x"0A";
  elsif  (unsigned(Clk_div_s) >= 10)then
      Clk_divD_s <=  Clk_div_s;
    else
      Clk_divD_s <= x"0A";
  end if;
end if;
end process U_Check_ClkDivValue_l;

--
-- SPI State machine
--
U_fsm_spi : process(i_clk_p)
begin
if rising_edge(i_clk_p) then

  if (i_rst_p = '1') then
    spi_state_s     <=  SPI_IDLE ;
    cntrl_start_s   <= '0';
    v6_cntrl_nbits_s<= (others => '0');
    v6_cntrl_nbits_save_s<= (others => '0');
    cntclk_s        <= 1;
    spi_clk_s       <= '0';
    spi_out_s       <= '0';
    spi_od_s        <= '0';
    v48_spi_sr_s    <= (others => '0');
    busy_s          <= '0';
    ack_s           <= '0';
  else
    ack_s       <= '0';
    case spi_state_s is
      when SPI_IDLE =>
        --busy_s      <= '0';
        spi_od_s        <= '1';--Disable output by default
        if (cntrl_start_rise_s = '1') then
          --If Start, then we latch the control register into our internal state
          cntrl_start_s   <= cntrl_start_rise_s;
          v6_cntrl_nbits_s <= (others=>'0');
          v6_cntrl_nbits_save_s <= v32_cntrl_s(6 downto 1);
          if (v32_cntrl_s(7) = '1') then
            busy_s          <= '1';
            spi_state_s     <=  SPI_WRITE ;
            spi_od_s        <= '0';--Enable output
          else
            busy_s          <= '1';
            spi_state_s     <=  SPI_SETUP_SLAVE ;
            spi_out_s       <= '0';--Put output in tri-state
            spi_od_s        <= '1';--Disable output
            cntclk_s <= 1;
          end if;
        else
            --Clock polarity must be set prior to asserting start to ensure proper operation.
            --In fact, it should be set prior to asserting the SPI SEL signal
            spi_clk_s <= v32_cntrl_s(0);
        end if;
      when SPI_WRITE =>
        --Load the data words into the shift register   (MSB first)
        v48_spi_sr_s(7 downto 0)   <= v8_word6_s;
        v48_spi_sr_s(15 downto 8)  <= v8_word5_s;
        v48_spi_sr_s(23 downto 16) <= v8_word4_s;
        v48_spi_sr_s(31 downto 24) <= v8_word3_s;
        v48_spi_sr_s(39 downto 32) <= v8_word2_s;
        v48_spi_sr_s(47 DOWNTO 40) <= v8_word1_s;
        spi_state_s     <=  SPI_SHIFT_OUT ;
        --initialize counter
        cntclk_s <= 1;
      when SPI_SETUP_SLAVE =>
        --This state start toggling clock for one cycle.  The Read only start
        --in the second cycle since the slave have to sense a first edge to latch
        --its data
        if (TO_SIGNED(cntclk_s,7) = (signed(Clk_divD_s)/2))  then
          spi_clk_s <= not spi_clk_s;
          cntclk_s <= cntclk_s+1;
        elsif (TO_SIGNED(cntclk_s,8) = signed(Clk_divD_s)) then
          spi_clk_s <= not spi_clk_s;
          spi_state_s    <=  SPI_READ ;
          cntclk_s <= 1;
        else
          cntclk_s <= cntclk_s+1;
        end if;
      when SPI_READ =>
        --In this state, data from the slave is latched into the shift register
        if (TO_SIGNED(cntclk_s,7) = (signed(Clk_divD_s)/2))  then
          spi_clk_s <= not spi_clk_s;
          v48_spi_sr_s <= v48_spi_sr_s(46 downto 0)& i_spi_in_p ;
          cntclk_s <= cntclk_s+1;
        elsif (TO_SIGNED(cntclk_s,8) = signed(Clk_divD_s)) then
          spi_clk_s <= not spi_clk_s;
          cntclk_s <= 1;
          if (v6_cntrl_nbits_s = v6_cntrl_nbits_save_s) then
            spi_state_s     <=  SPI_SHIFT_IN ;
          else
            v6_cntrl_nbits_s <= STD_LOGIC_VECTOR(UNSIGNED(v6_cntrl_nbits_s)+1);
          end if;
        else
          cntclk_s <= cntclk_s+1;
        end if;
      when SPI_SHIFT_IN =>
        --If the number of bit to shift is smaller to the length
        --of the shift register, we need to shift left by the difference
        --in order to have data at the rigth place
        if (v6_cntrl_nbits_s = "101111") then
          spi_state_s     <=  SPI_IDLE ;
          cntrl_start_s   <= '0';
          ack_s           <= '1';
          busy_s          <= '0';
        else
          v48_spi_sr_s <= v48_spi_sr_s(46 downto 0)& '0' ;
          v6_cntrl_nbits_s <= STD_LOGIC_VECTOR(UNSIGNED(v6_cntrl_nbits_s)+1);
        end if;

      when SPI_SHIFT_OUT =>
        if (TO_SIGNED(cntclk_s,7) = (signed(Clk_divD_s)/2)) then
          spi_clk_s <= not spi_clk_s;
          spi_out_s <= v48_spi_sr_s(47);
          v48_spi_sr_s <= v48_spi_sr_s(46 downto 0) & '0' ;
          cntclk_s <= cntclk_s+1;
        elsif (TO_SIGNED(cntclk_s,8) = signed(Clk_divD_s)) then
          spi_clk_s <= not spi_clk_s;
          cntclk_s <= 1;
          if (v6_cntrl_nbits_s = v6_cntrl_nbits_save_s) then
            spi_state_s     <=  SPI_IDLE ;
            cntrl_start_s   <= '0';
            ack_s          <= '1';
            busy_s          <= '0';
          else
            v6_cntrl_nbits_s <= STD_LOGIC_VECTOR(UNSIGNED(v6_cntrl_nbits_s)+1);
          end if;
        else
          cntclk_s <= cntclk_s+1;
        end if;
      when others =>
     end case;
    end if;
end if;
end process;

--
--  SPI address decoding : Only one select can be activated at a time by using a priority encoder.
--  The chip select work in manual mode, the CPU as full control on when it activates a selection
--
U_spi_sel : process(i_clk_p)
begin
if rising_edge(i_clk_p) then
  if (i_rst_p = '1') then
    spi_sel_s <= (others=>'1');
  else
    if (v16_cs_s(0) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(0) <= '0';
    elsif (v16_cs_s(1) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(1) <= '0';
    elsif (v16_cs_s(2) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(2) <= '0';
    elsif (v16_cs_s(3) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(3) <= '0';
    elsif (v16_cs_s(4) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(4) <= '0';
    elsif (v16_cs_s(5) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(5) <= '0';
    elsif (v16_cs_s(6) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(6) <= '0';
    elsif (v16_cs_s(7) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(7) <= '0';
    elsif (v16_cs_s(8) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(8) <= '0';
    elsif (v16_cs_s(9) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(9) <= '0';
    elsif (v16_cs_s(10) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(10) <= '0';
    elsif (v16_cs_s(11) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(11) <= '0';
    elsif (v16_cs_s(12) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(12) <= '0';
    elsif (v16_cs_s(13) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(13) <= '0';
    elsif (v16_cs_s(14) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(14) <= '0';
    elsif (v16_cs_s(15) = '0') then
      spi_sel_s <= (others=>'1');
      spi_sel_s(15) <= '0';
    else
      spi_sel_s <= (others=>'1');
    end if;
  end if;
end if;
end process;

-- Output ports assignments
o_dout_p <= v32_dout_s;
o_ack_p <= ack_s;
o_spi_clk_p <= spi_clk_s;
o_spi_out_p <= spi_out_s;
o_spi_sel_p <= spi_sel_s(7 downto 0);
o_spi_sel2_p <= spi_sel_s(15 downto 8);
o_spi_od_p <= spi_od_s;
o_busy_p <= busy_s;
end Behavioral;

