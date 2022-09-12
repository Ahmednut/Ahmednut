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
-- File        : $Id: spi_register.vhd,v 1.4 2015/11/24 14:24:58 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Generic SPI core
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: spi_register.vhd,v $
-- Revision 1.4  2015/11/24 14:24:58  julien.roy
-- Update logic to force SPI IO register to IOB
--
-- Revision 1.3  2015/11/23 20:28:47  julien.roy
-- MOdify SPI register component to register output and input SPI signals
--
-- Revision 1.2  2015/11/12 14:51:13  julien.roy
-- Modify SPI core to have dynamicaly configurable SPI CLK speed
--
-- Revision 1.1  2015/10/20 19:14:44  julien.roy
-- Add first revision of Radio640 fpga pcore.
--
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.2  2013/01/18 19:03:46  julien.roy
-- Merge changes from ZedBoard reference design to Perseus
--
-- Revision 1.1  2012/09/28 19:31:26  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
--
-- Revision 1.2  2011/06/16 18:54:55  jeffrey.johnson
-- Added CVS tags.
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

Library UNISIM;
use UNISIM.vcomponents.all;

entity spi_register is
  generic(
    SPI_DATA_WIDTH  : integer := 16;
    SPI_NUMBER_CS : integer := 1
  );
  port  (
    -- SPI Interface
    i_clk_p                     : in     std_logic;
    i_rst_p                     : in     std_logic;

    iv8_spiClkDownsampling_p    : in     std_logic_vector(7 downto 0);

    iv_spiStart_p               : in     std_logic_vector(SPI_NUMBER_CS-1 downto 0);
    i_spiClkPol_p               : in     std_logic;
    i_spiMsbf_p                 : in     std_logic;
    iv_spiDataIn_p              : in     std_logic_vector((SPI_DATA_WIDTH - 1) downto 0);
    ov_spiDataOut_p             : out    std_logic_vector((SPI_DATA_WIDTH - 1) downto 0);
    o_spiBusy_p                 : out    std_logic := '1';
    o_spiEnd_p                  : out    std_logic;

    o_spiClkOutPin_p            : out    std_logic;
    ov_spiCsPin_p               : out    std_logic_vector(SPI_NUMBER_CS-1 downto 0);
    o_spiMosiPin_p              : out    std_logic;
    i_spiMisoPin_p              : in     std_logic
  );
end entity spi_register;


architecture rtl of spi_register is

  ----------------------------------------
  -- Function declaration
  ----------------------------------------

  function OR_REDUCE(ARG: std_logic_vector) return std_logic is
    variable result: std_logic;
    begin
      result := '0';
      for i in ARG'range loop
        result := result or ARG(i);
      end loop;
      return result;
    end;

  ----------------------------------------
  -- Type declaration
  ----------------------------------------

  --Declaration for control state machine
  type state_type is (st1_idle, st2_load_tx, st3_transmit, st4_receive1, st5_receive2);

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------

  signal state_sig, next_state_sig, last_state : state_type;

  signal spi_tx_enable_s    : std_logic;
  signal spi_rx_enable_s    : std_logic;
  signal spi_cs_enable_s    : std_logic;
  signal spiEnable_s        : std_logic;

  signal csLatch_s          : std_logic_vector(SPI_NUMBER_CS-1 downto 0);
  signal countTxBit_s       : integer range 0 to (SPI_DATA_WIDTH-1);
  signal orStart_s          : std_logic;
  signal orStart_R1_s       : std_logic;
  signal startEdge_s        : std_logic;
  signal startEdgeDetect_s  : std_logic;
  signal v_spi_lfsr_s       : std_logic_vector(SPI_DATA_WIDTH-1  downto 0);
  signal v_dataLatch_s      : std_logic_vector(SPI_DATA_WIDTH-1  downto 0);

  signal spiCe_s            : std_logic;
  signal spiCeN_s           : std_logic;
  signal spiClkCount_s      : unsigned(7 downto 0);

  signal spiMiso_s          : std_logic;
  signal spiMosi_s          : std_logic;
  signal v_spiCsPin_s       : std_logic_vector(SPI_NUMBER_CS-1 downto 0);
  signal spiClkOut_s        : std_logic;
  signal spiClkOutCe_s      : std_logic;
  signal spiClkOutEnable_s  : std_logic;

begin

  ----------------------------------------
  -- Clock decimation process
  ----------------------------------------
  p_ClkDecim_l: process(i_clk_p)
  begin
    if (rising_edge(i_clk_p)) then
        if (i_rst_p = '1') then
          spiClkCount_s <= (others => '0');
          spiCe_s  <= '0';
          spiCeN_s <= '0';
        else
          spiCe_s  <= '0';
          spiCeN_s <= '0';
          spiClkCount_s <= spiClkCount_s + 1;
          if ( spiClkCount_s = (("0" & unsigned(iv8_spiClkDownsampling_p(7 downto 1)))-1) ) then
            spiCeN_s    <= '1';
          end if;
          if ( spiClkCount_s >= (unsigned(iv8_spiClkDownsampling_p)-1) ) then
            spiClkCount_s <= (others => '0');
            spiCe_s <= '1';
          end if;
        end if;
    end if;
  end process;

  ----------------------------------------
  --Detect register rising edge
  ----------------------------------------
  orStart_s <= OR_REDUCE(iv_spiStart_p);

  start_re_process_l : process (i_clk_p)
  begin
    if rising_edge(i_clk_p) then
      orStart_R1_s <= orStart_s;
    end if;
  end process;

  startEdgeDetect_s <= orStart_s and (not orStart_R1_s);

  ----------------------------------------
  -- Wait detection due to clock decimation
  ----------------------------------------
  Latch_process_l : process (i_clk_p)
  begin
    if rising_edge(i_clk_p) then

      -- Not busy when in IDLE state and no start pending
      if (startEdge_s = '0' and state_sig = st1_idle) then
        o_spiBusy_p <= '0';
      end if;

      -- When in IDLE state and no start pending,
      -- sensitive to start rising edge
      if (startEdgeDetect_s = '1' and startEdge_s = '0' and state_sig = st1_idle) then
        csLatch_s       <= iv_spiStart_p;
        v_dataLatch_s   <= iv_spiDataIn_p;
        startEdge_s     <= '1';
        o_spiBusy_p     <= '1';
      end if;

      -- Clear start pending status when started
      if (spiEnable_s = '1') then
        startEdge_s   <= '0';
      end if;

      -- SYnchronous reset
      if (i_rst_p = '1') then
        startEdge_s   <= '0';
        o_spiBusy_p   <= '1';
      end if;
    end if;
  end process;

  ----------------------------------------
  -- Serialize/Deserialize SPI data
  ----------------------------------------
  ps_UserSPIO: process(i_clk_p)
   begin
      if rising_edge(i_clk_p) then
        if (i_rst_p = '1') then
          v_spi_lfsr_s <= (others=>'0');
        elsif (spiCe_s ='1' and spiEnable_s ='1') then
          if ( spi_tx_enable_s = '1' or spi_rx_enable_s = '1' ) then
            if (i_spiMsbf_p = '1') then
              for i in (SPI_DATA_WIDTH - 1) downto 1 loop
                v_spi_lfsr_s(i) <= v_spi_lfsr_s(i-1);
              end loop;
              v_spi_lfsr_s(0) <= spiMiso_s;

            else
              for i in 0 to (SPI_DATA_WIDTH - 2) loop
                v_spi_lfsr_s(i) <= v_spi_lfsr_s(i+1);
              end loop;
              v_spi_lfsr_s(SPI_DATA_WIDTH - 1) <= spiMiso_s;

            end if;

          else

            v_spi_lfsr_s <= v_dataLatch_s;

          end if;
        end if;
     end if;
  end process ;

  ----------------------------------------
  -- Create a TX bit counter
  ----------------------------------------
  ps_countbit: process (i_clk_p)
    begin
      if rising_edge(i_clk_p) then
        if (i_rst_p = '1') then
          countTxBit_s <= 0;
        elsif (spiCe_s ='1' and spi_tx_enable_s = '1') then
          if (countTxBit_s = SPI_DATA_WIDTH -1) then
            countTxBit_s <= 0;
          else
            countTxBit_s <= countTxBit_s + 1 ;
          end if;
        end if;
     end if;
  end process;


  ----------------------------------------
  -- State machine
  ----------------------------------------
  SYNC_PROC: process (i_clk_p, i_rst_p)
  begin
    if rising_edge(i_clk_p) then
      if (i_rst_p='1') then
        state_sig <= st1_idle;
      elsif (spiCe_s ='1') then
        last_state <= state_sig;
        state_sig <= next_state_sig;
      end if;
    end if;
  end process;

  -- Moore State Machine - Outputs based on state only
  OUTPUT_DECODE: process (state_sig,spiEnable_s,spi_tx_enable_s,spi_rx_enable_s,spi_cs_enable_s)
  begin
    case (state_sig) is
        when st1_idle =>
            spi_tx_enable_s <= '0';
            spi_rx_enable_s <= '0';
            spi_cs_enable_s <= '0';
            spiEnable_s <= '0';
        when st2_load_tx =>
            spi_tx_enable_s <= '0';
            spi_rx_enable_s <= '0';
            spi_cs_enable_s <= '1';
            spiEnable_s <= '1';
        when st3_transmit =>
            spi_tx_enable_s <= '1';
            spi_rx_enable_s <= '1';
            spi_cs_enable_s <= '1';
            spiEnable_s <= '1';
        when st4_receive1 =>
            spi_tx_enable_s <= '0';
            spi_rx_enable_s <= '1';
            spi_cs_enable_s <= '0';
            spiEnable_s <= '1';
        when st5_receive2 =>
            spi_tx_enable_s <= '0';
            spi_rx_enable_s <= '1';
            spi_cs_enable_s <= '0';
            spiEnable_s <= '1';
        when others =>
            spi_tx_enable_s <= '0';
            spi_rx_enable_s <= '0';
            spi_cs_enable_s <= '0';
            spiEnable_s <= '0';
    end case;
  end process;

  -- Next state decoder
  NEXT_STATE_DECODE: process (state_sig, startEdge_s,countTxBit_s)
  begin

    -- Declare default state for next_state to avoid latches
    next_state_sig <= state_sig;  -- Default is to stay in current state

    case (state_sig) is
      when st1_idle =>
        if startEdge_s = '1' then
          next_state_sig <= st2_load_tx;
        end if;
     when st2_load_tx =>
        next_state_sig <= st3_transmit;
     when st3_transmit =>
        if (countTxBit_s = SPI_DATA_WIDTH -1) then
            next_state_sig <= st4_receive1;
        end if;
     when st4_receive1 =>
        next_state_sig <= st5_receive2;
     when st5_receive2 =>
        next_state_sig <= st1_idle;
     when others =>
        next_state_sig <= st1_idle;
    end case;
  end process;

    ----------------------------------------
    -- Output assignment
    ----------------------------------------

    spiMosi_s <= 
        (spi_tx_enable_s and v_spi_lfsr_s(SPI_DATA_WIDTH-1)) when (i_spiMsbf_p = '1') 
        else (spi_tx_enable_s and v_spi_lfsr_s(0));

    spiMosiPin_inst : FDRE
       generic map (
          INIT => '0')          -- Initial value of register ('0' or '1')
       port map (
          Q => o_spiMosiPin_p,  -- Data output
          C => i_clk_p,         -- Clock input
          CE => spiCe_s,        -- Clock enable input
          R => '0',             -- Synchronous reset input
          D => spiMosi_s     -- Data input
       );

    spiCsPin_gen: for i in 0 to csLatch_s'high generate

        v_spiCsPin_s(i) <= not (csLatch_s(i) and spi_cs_enable_s);

        spiCsPin_inst : FDRE
           generic map (
              INIT => '1')              -- Initial value of register ('0' or '1')
           port map (
              Q => ov_spiCsPin_p(i),    -- Data output
              C => i_clk_p,             -- Clock input
              CE => spiCe_s,            -- Clock enable input
              R => '0',                 -- Synchronous reset input
              D => v_spiCsPin_s(i)      -- Data input
           );
    end generate;


    process (i_clk_p, i_rst_p)
    begin
        if rising_edge(i_clk_p) then
            if spiCeN_s = '1' then
                spiClkOutEnable_s <= spi_tx_enable_s;
            end if;
        end if;
    end process;

    spiClkOutCe_s <= (spiCeN_s or spiCe_s);
    spiClkOut_s <=
        spiClkOutEnable_s and
        (((not i_spiClkPol_p) and spiCe_s) or
        (i_spiClkPol_p and spiCeN_s));

    spiClkOutPin_inst : FDRE
       generic map (
          INIT => '0')              -- Initial value of register ('0' or '1')
       port map (
          Q => o_spiClkOutPin_p,    -- Data output
          C => i_clk_p,             -- Clock input
          CE => spiClkOutCe_s,      -- Clock enable input
          R => '0',                 -- Synchronous reset input
          D => spiClkOut_s          -- Data input
       );

    spiMisoPin_inst : FDRE
       generic map (
          INIT => '0')          -- Initial value of register ('0' or '1')
       port map (
          Q => spiMiso_s,       -- Data output
          C => i_clk_p,         -- Clock input
          CE => spiCe_s,        -- Clock enable input
          R => '0',             -- Synchronous reset input
          D => i_spiMisoPin_p   -- Data input
       );

    ov_spiDataOut_p <= v_spi_lfsr_s;

    o_spiEnd_p <= '1' when ((last_state = st5_receive2) and (state_sig = st1_idle)) else '0';

end architecture rtl;
