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
-- File : aurora_top.vhd
--------------------------------------------------------------------------------
-- Description : Top Level Aurora Core with Fifos
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log:
--
--
--------------------------------------------------------------------------------

-- Library declarations
library ieee;
  use ieee.std_logic_unsigned.all;
  use ieee.std_logic_misc.all;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_1164.all;
library unisim;
  use unisim.vcomponents.all;
library work;

entity lyt_aurora is
generic
(
  AURORA_SPEED                          : integer   := 0;
  USER_DATA_WIDTH                       : integer   := 16;
  REFCLK_FREQ_MHZ                       : integer   := 100;
  MGT_USED                              : string    := "unknown"
);
port (

  -- System signal
  i_Rst_p                               : in  std_logic;
  i_RefClk_p                            : in  std_logic;

  -- User interface
  i_UserClk_p                           : in  std_logic;                                        --User clock that will clock the fifos data

  o_TxReady_p                           : out std_logic;                                        --Data ready to be sent
  iv_TxData_p                           : in  std_logic_vector(user_data_width-1 downto 0);     --Data to be sent by the Aurora link
  i_TxWriteReq_p                        : in  std_logic;                                        --Write enable

  o_RxReady_p                           : out std_logic;                                        --Data ready to be read
  i_RxReadReq_p                         : in  std_logic;                                        --Read request
  ov_RxData_p                           : out std_logic_vector(user_data_width-1 downto 0);     --Data received by the Aurora link
  o_RxDataValid_p                       : out std_logic;                                        --Data read is valid

  -- Registers
  i_RegWrEn_p                           : in  std_logic;
  iv16_RegIdx_p                         : in  std_logic_vector(15 downto 0);
  iv32_RegData_p                       : in  std_logic_vector(31 downto 0);
  ov32_RegData_p                       : out std_logic_vector(31 downto 0);

  -- Hardware pins
  iv4_Aurora_GTX_RX_p                   : IN  STD_LOGIC_VECTOR(3 downto 0);                     --Aurora GTX RX Lanes
  iv4_Aurora_GTX_RX_n                   : IN  STD_LOGIC_VECTOR(3 downto 0);
  ov4_Aurora_GTX_TX_p                   : OUT STD_LOGIC_VECTOR(3 downto 0);                     --Aurora GTX TX Lanes
  ov4_Aurora_GTX_TX_n                   : OUT STD_LOGIC_VECTOR(3 downto 0);
  i_Aurora_GTX_CLK_p                    : IN  STD_LOGIC;                                        --Aurora GTX Clock Input
  i_Aurora_GTX_CLK_n                    : IN  STD_LOGIC

);
end lyt_aurora;

architecture aurora_top_syn of lyt_aurora is

	----------------------------------------------------------------------------------------------------
	-- Constant declaration
	----------------------------------------------------------------------------------------------------

    constant AURORA_WIDTH               : integer   := 128;
    
    constant C_TXPOSTEMPHASIS_DEFAULT_VALUE     : std_logic_vector(4 downto 0)  := "00000";
    constant C_TXPREEMPHASIS_DEFAULT_VALUE      : std_logic_vector(3 downto 0)  := "0000";
    constant C_TXDIFFCTRL_DEFAULT_VALUE         : std_logic_vector(3 downto 0)  := "1010";
    constant C_DFETAP1_DEFAULT_VALUE            : std_logic_vector(4 downto 0)  := "00000";
    constant C_RXEQMIX_DEFAULT_VALUE            : std_logic_vector(2 downto 0)  := "110";
    
    constant C_REG8_DEFAULT_VALUE               : std_logic_vector(31 downto 0) := 
        "00000000000" &
        C_RXEQMIX_DEFAULT_VALUE & 
        C_DFETAP1_DEFAULT_VALUE & 
        C_TXDIFFCTRL_DEFAULT_VALUE &
        C_TXPREEMPHASIS_DEFAULT_VALUE & 
        C_TXPOSTEMPHASIS_DEFAULT_VALUE;
    
	----------------------------------------------------------------------------------------------------
	--Signal declaration
	----------------------------------------------------------------------------------------------------
    
    signal v32_Reg0_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg1_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg1_Read_s              : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg2_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg3_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg4_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg5_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg6_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg7_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg8_s                   : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_Reg9_s                   : std_logic_vector(31 downto 0) := (others => '0');

    signal v32_AuroraControlR1_s        : std_logic_vector(31 downto 0);
    signal v32_AuroraControlR2_s        : std_logic_vector(31 downto 0);

    signal Aurora_GTX_CLK_s             : std_logic;
    signal GTX_fifo_clk_s               : std_logic;
    signal TX_Dst_Rdy_n_s               : std_logic;
    signal RX_Src_Rdy_n_s               : std_logic;
    signal TX_Src_Rdy_n_s               : std_logic;

    signal v_Aurora_RX_Data_s           : std_logic_vector(AURORA_WIDTH-1 downto 0);
    signal v_Aurora_TX_Data_s           : std_logic_vector(AURORA_WIDTH-1 downto 0);
    signal TX_Fifo_Read_Enable_s        : std_logic;
    signal RX_Fifo_Write_Enable_s       : std_logic;

    signal RX_Fifo_out_valid_s          : std_logic := '0';
    signal TX_Fifo_out_valid_s          : std_logic := '0';

    signal tx_ready_s                   : std_logic;

    signal v64_transmit_count_s         : std_logic_vector(63 downto 0);
    signal v64_receive_count_s          : std_logic_vector(63 downto 0);

    signal Aurora_Enable_Tx_s           : std_logic;
    signal Aurora_Enable_Rx_s           : std_logic;
    signal Aurora_Enable_Tx_R1_s        : std_logic;
    signal Aurora_Enable_Rx_R1_s        : std_logic;
    signal Aurora_Reset_s               : std_logic;
    signal Aurora_Reset_Delay_s         : std_logic;
    signal GT_Reset_s                   : std_logic;
    signal GT_Reset_change_s            : std_logic;
    signal v_GT_Reset_counter_s         : std_logic_vector(31 downto 0);


    signal Aurora_Hard_Error_s          : std_logic;
    signal Aurora_Soft_Error_s          : std_logic;
    signal Aurora_Channel_Up_s          : std_logic;
    signal Aurora_Channel_Up_R1_s       : std_logic;
    signal Aurora_Channel_Up_R2_s       : std_logic;
    signal Aurora_Channel_Up_R6_s       : std_logic;
    signal v4_Aurora_Lanes_Up_s         : std_logic_vector(3 downto 0);

    signal TX_Fifo_Reset_s              : std_logic;
    signal TX_Fifo_Empty_s              : std_logic;
    signal TX_Fifo_AFull_s              : std_logic;
    signal TX_Fifo_Full_s               : std_logic;
    signal RX_Fifo_Reset_s              : std_logic;
    signal RX_Fifo_Empty_s              : std_logic;

    signal latch_counter_s              : std_logic;
    signal latch_counter_r1_s           : std_logic;
    signal latch_counter_r2_s           : std_logic;
    
    
    signal RXEQMIX                     : std_logic_vector(2 downto 0);
    signal TXDIFFCTRL                  : std_logic_vector(3 downto 0);
    signal TXPOSTEMPHASIS              : std_logic_vector(4 downto 0);
    signal TXPREEMPHASIS               : std_logic_vector(3 downto 0);
    signal DFETAP1                     : std_logic_vector(4 downto 0);
    signal DFEEYEDACMON_LANE0          : std_logic_vector(4 downto 0);
    signal DFEEYEDACMON_LANE1          : std_logic_vector(4 downto 0);
    signal DFEEYEDACMON_LANE2          : std_logic_vector(4 downto 0);
    signal DFEEYEDACMON_LANE3          : std_logic_vector(4 downto 0);

	constant RW_USER_SIDE_FIFO_DEPTH	: integer :=1024;
	
	----------------------------------------------------------------------------------------------------
	--Component declaration
	----------------------------------------------------------------------------------------------------

  attribute keep : string;
  attribute keep of GT_Reset_change_s       : signal is "true";
  attribute keep of v_GT_Reset_counter_s    : signal is "true";
  attribute keep of TX_Fifo_Reset_s         : signal is "true";
  attribute keep of RX_Fifo_Reset_s         : signal is "true";

  attribute maxdelay : string;
  attribute maxdelay of TX_Fifo_Reset_s : signal is "10 ns";
  attribute maxdelay of RX_Fifo_Reset_s : signal is "10 ns";


begin

    ind_reg_i : entity work.ntq_ind_reg
    generic map
    (
        C_REG1_DEFAULT_VALUE => x"00000000",
        C_REG8_DEFAULT_VALUE => C_REG8_DEFAULT_VALUE
    )
    port map
    (

        -- User ports
        i_Clk_p             => i_UserClk_p,
        i_Rst_p             => i_Rst_p,

        -- Indirect addressing interface
        --
        i_IndWrEn_p         => i_RegWrEn_p,
        iv5_IndRegIdx_p     => iv16_RegIdx_p(4 downto 0),
        iv32_IndWrReg_p     => iv32_RegData_p,
        ov32_IndRdReg_p     => ov32_RegData_p,

        iv32_Reg0_p         => v32_Reg0_s,
        iv32_Reg1_p         => v32_Reg1_Read_s,
        iv32_Reg2_p         => v32_Reg2_s,
        iv32_Reg3_p         => v32_Reg3_s,
        iv32_Reg4_p         => v32_Reg4_s,
        iv32_Reg5_p         => v32_Reg5_s,
        iv32_Reg6_p         => v32_Reg6_s,
        iv32_Reg7_p         => v32_Reg7_s,
        iv32_Reg8_p         => v32_Reg8_s,
        iv32_Reg9_p         => v32_Reg9_s,
        iv32_reg10_p        => x"00000000",
        iv32_reg11_p        => x"00000000",
        iv32_reg12_p        => x"00000000",
        iv32_reg13_p        => x"00000000",
        iv32_reg14_p        => x"00000000",
        iv32_reg15_p        => x"00000000",
        iv32_reg16_p        => x"00000000",
        iv32_reg17_p        => x"00000000",
        iv32_reg18_p        => x"00000000",
        iv32_reg19_p        => x"00000000",
        iv32_reg20_p        => x"00000000",
        iv32_reg21_p        => x"00000000",
        iv32_reg22_p        => x"00000000",
        iv32_reg23_p        => x"00000000",
        iv32_reg24_p        => x"00000000",
        iv32_reg25_p        => x"00000000",
        iv32_reg26_p        => x"00000000",
        iv32_reg27_p        => x"00000000",
        iv32_reg28_p        => x"00000000",
        iv32_reg29_p        => x"00000000",
        iv32_reg30_p        => x"00000000",
        iv32_reg31_p        => x"00000000",

        ov32_Reg1_p         => v32_Reg1_s,
        ov32_Reg8_p         => v32_Reg8_s
    );



                                   -- Core      Version
                                   --  ID      Maj    Min
  v32_Reg0_s                        <= X"AAAA" & X"02" & X"00";

  v32_Reg1_Read_s(25)               <= v32_Reg1_s(25);
  v32_Reg1_Read_s(23)               <= v32_Reg1_s(23);
  v32_Reg1_Read_s(22)               <= Aurora_Hard_Error_s;
  v32_Reg1_Read_s(21)               <= Aurora_Soft_Error_s;
  v32_Reg1_Read_s(20)               <= Aurora_Channel_Up_s;
  v32_Reg1_Read_s(19 downto 16)     <= v4_Aurora_Lanes_Up_s;
  v32_Reg1_Read_s(15)               <= v32_Reg1_s(15);
  v32_Reg1_Read_s(11)               <= TX_Fifo_Full_s;
  v32_Reg1_Read_s( 7)               <= v32_Reg1_s(7);
  v32_Reg1_Read_s(6)                <= v32_Reg1_s(6);
  v32_Reg1_Read_s(5)                <= v32_Reg1_s(5);
  v32_Reg1_Read_s( 0)               <= RX_Fifo_Empty_s;

  latch_counter_s                   <= v32_Reg1_s(25);

  process(i_UserClk_p)
  begin
    if rising_edge(i_UserClk_p) then

        Aurora_Channel_Up_R1_s  <= Aurora_Channel_Up_s;
        Aurora_Channel_Up_R2_s  <= Aurora_Channel_Up_R1_s;

        TX_Fifo_Reset_s         <= v32_Reg1_s(15) or (not Aurora_Channel_Up_R2_s);
        RX_Fifo_Reset_s         <= v32_Reg1_s(7)  or (not Aurora_Channel_Up_R2_s);
        Aurora_Enable_Tx_s      <= v32_Reg1_s(6);
        Aurora_Enable_Rx_s      <= v32_Reg1_s(5);
        Aurora_Enable_Tx_R1_s   <= Aurora_Enable_Tx_s;
        Aurora_Enable_Rx_R1_s   <= Aurora_Enable_Rx_s;

    end if;
  end process;


  
  -- CBOU: Previously aurora channels would always be running. It
  -- would be impossible to test either tx or rx alone.
  --Aurora_Enable_Tx_s                     <= '1';  -- Always running
  --Aurora_Enable_Rx_s                     <= '1';  -- Always running


  -----------------------------------------------------------------------------------
  -- Aurora core
  -----------------------------------------------------------------------------------

  -- GTX clock buffer
  IBUFDS_i :  IBUFDS_GTXE1
  port map (
       I     => i_Aurora_GTX_CLK_p,
       IB    => i_Aurora_GTX_CLK_n,
       CEB   => '0',
       O     => Aurora_GTX_CLK_s,
       ODIV2 => OPEN);

  TX_Fifo_Read_Enable_s  <= not TX_Dst_Rdy_n_s and not TX_Fifo_Empty_s and Aurora_Enable_Tx_R1_s;
  RX_Fifo_Write_Enable_s <= not RX_Src_Rdy_n_s and Aurora_Enable_Rx_R1_s;

  TX_Src_Rdy_n_s <= TX_Fifo_out_valid_s nand Aurora_Enable_Tx_R1_s;

  -- Reset the GT at the core reset.
  process(i_UserClk_p)
    variable v_gtreset_counter_s  : std_logic_vector(9 downto 0);
  begin
    if rising_edge(i_UserClk_p) then
        GT_Reset_change_s <= '0';

        if v_gtreset_counter_s = "0000000000" then
            GT_Reset_s <= '0';

            if GT_Reset_s = '1' then
                GT_Reset_change_s <= '1';
            end if;

        else
            v_gtreset_counter_s := v_gtreset_counter_s - 1;
        end if;

        -- Reset at system start-up, when reset by the user register
        -- or Auto reset when not up after 250M clock cycles.
        if (i_Rst_p = '1') or (v_GT_Reset_counter_s > 25000000 and Aurora_Channel_Up_R2_s = '0') then
            GT_Reset_s <= '1';
            v_gtreset_counter_s := (others => '1');
            v_GT_Reset_counter_s <= (others => '0');
            GT_Reset_change_s <= '1';
        end if;

        if v_GT_Reset_counter_s <= 25000000 then
            v_GT_Reset_counter_s <= v_GT_Reset_counter_s + 1;
        end if;

    end if;
  end process;

  -- Aurora_Reset_s    <= i_Rst_p or v32_Reg1_s(23);
  Aurora_Reset_s    <= i_Rst_p or v32_Reg1_s(23);

  -- Aurora core instance
  u_aurora_8b10b_v5_3_wrapper: entity work.aurora_8b10b_v5_3_wrapper
  generic map
  (
    AURORA_SPEED              => AURORA_SPEED,
    DATA_WIDTH                => AURORA_WIDTH
  )
  PORT MAP
  (
		i_User_Clock_p            => i_UserClk_p,
		o_GTX_fifo_clk_p          => GTX_fifo_clk_s,
		ov_RX_Data_p              => v_Aurora_RX_Data_s,
		iv_TX_Data_p              => v_Aurora_TX_Data_s,
		i_TX_Src_Rdy_n            => TX_Src_Rdy_n_s,
		o_TX_Dst_Rdy_n            => TX_Dst_Rdy_n_s,
    
		o_RX_Src_Rdy_n            => RX_Src_Rdy_n_s,

		iv4_Aurora_GTX_RX_p       => iv4_Aurora_GTX_RX_p,
		iv4_Aurora_GTX_RX_n       => iv4_Aurora_GTX_RX_n,
		ov4_Aurora_GTX_TX_p       => ov4_Aurora_GTX_TX_p,
		ov4_Aurora_GTX_TX_n       => ov4_Aurora_GTX_TX_n,
		i_Aurora_GTX_CLK_p        => Aurora_GTX_CLK_s,
		o_Aurora_Hard_Error_p     => Aurora_Hard_Error_s,
		o_Aurora_Soft_Error_p     => Aurora_Soft_Error_s,
		o_Aurora_Channel_Up_p     => Aurora_Channel_Up_s,
		ov4_Aurora_Lanes_Up_p     => v4_Aurora_Lanes_Up_s,
		i_Aurora_Reset_p          => Aurora_Reset_s,
        i_GT_Reset_p              => GT_Reset_s,
        
        RXEQMIX                   => RXEQMIX,
        TXDIFFCTRL                => TXDIFFCTRL,
        TXPOSTEMPHASIS            => TXPOSTEMPHASIS,
        TXPREEMPHASIS             => TXPREEMPHASIS,
        DFETAP1                   => DFETAP1,
        DFEEYEDACMON_LANE0        => DFEEYEDACMON_LANE0,
        DFEEYEDACMON_LANE1        => DFEEYEDACMON_LANE1,
        DFEEYEDACMON_LANE2        => DFEEYEDACMON_LANE2,
        DFEEYEDACMON_LANE3        => DFEEYEDACMON_LANE3
	);
    
    TXPOSTEMPHASIS              <= v32_Reg8_s(4 downto 0);
    TXPREEMPHASIS               <= v32_Reg8_s(8 downto 5);
    TXDIFFCTRL                  <= v32_Reg8_s(12 downto 9);
    DFETAP1                     <= v32_Reg8_s(17 downto 13);
    RXEQMIX                     <= v32_Reg8_s(20 downto 18);
    
    v32_Reg9_s(4 downto 0)      <= DFEEYEDACMON_LANE0;
    v32_Reg9_s(9 downto 5)      <= DFEEYEDACMON_LANE1;
    v32_Reg9_s(14 downto 10)    <= DFEEYEDACMON_LANE2;
    v32_Reg9_s(19 downto 15)    <= DFEEYEDACMON_LANE3;
    
  -----------------------------------------------------------------------------------
  -- User FIFOs
  -----------------------------------------------------------------------------------
  -- Calculate Read/Write depth for user side so the aurora side stays always = 1024
  gen_16_to_128: if (USER_DATA_WIDTH = 16 or USER_DATA_WIDTH = 32 or USER_DATA_WIDTH = 64 or USER_DATA_WIDTH = 128) generate	
     u_fifo_RX : entity work.generic_fifo
     generic map
     (
       WRITE_WIDTH_g             => AURORA_WIDTH,
       READ_WIDTH_g              => USER_DATA_WIDTH,
       WRITE_DEPTH_g             => 1024,
       READ_DEPTH_g              => (AURORA_WIDTH/USER_DATA_WIDTH)*1024,
       FIRST_WORD_FALL_THROUGH_g => FALSE
     )
     port map
     (
       i_rst_p                   => RX_Fifo_Reset_s,
       i_wr_clk_p                => GTX_fifo_clk_s,
       i_rd_clk_p                => i_UserClk_p,
       iv_din_p                  => v_Aurora_RX_Data_s,
       i_wr_en_p                 => RX_Fifo_Write_Enable_s,
       i_rd_en_p                 => i_RxReadReq_p,
       iv_prog_empty_thresh_p    => x"000F",
       iv_prog_full_thresh_p     => x"00FF",
       ov_dout_p                 => ov_RxData_p,
       o_full_p                  => open,
       o_overflow_p              => open,
       o_empty_p                 => RX_Fifo_Empty_s,
       o_valid_p                 => RX_Fifo_out_valid_s,
       o_underflow_p             => open,
       ov_rd_data_count_p        => open,
       ov_wr_data_count_p        => open,
       o_prog_full_p             => open,
       o_prog_empty_p            => open,
       o_almost_full_p           => open
     );
   
     u_fifo_TX : entity work.generic_fifo
     generic map
     (
       WRITE_WIDTH_g             => USER_DATA_WIDTH,
       READ_WIDTH_g              => AURORA_WIDTH,
       WRITE_DEPTH_g             => (AURORA_WIDTH/USER_DATA_WIDTH)*1024,
       READ_DEPTH_g              => 1024,
       FIRST_WORD_FALL_THROUGH_g => TRUE
     )
     port map
     (
       i_rst_p                   => TX_Fifo_Reset_s,
       i_wr_clk_p                => i_UserClk_p,
       i_rd_clk_p                => GTX_fifo_clk_s,
       iv_din_p                  => iv_TxData_p,
       i_wr_en_p                 => (i_TxWriteReq_p and tx_ready_s),
       i_rd_en_p                 => TX_Fifo_Read_Enable_s,
       iv_prog_empty_thresh_p    => x"000F",
       iv_prog_full_thresh_p     => x"00FF",
       ov_dout_p                 => v_Aurora_TX_Data_s,
       o_full_p                  => open,
       o_overflow_p              => open,
       o_empty_p                 => TX_Fifo_Empty_s,
       o_valid_p                 => TX_Fifo_out_valid_s,
       o_underflow_p             => open,
       ov_rd_data_count_p        => open,
       ov_wr_data_count_p        => open,
       o_prog_full_p             => open,
       o_prog_empty_p            => open,
       o_almost_full_p           => TX_Fifo_AFull_s
     );
 end generate;

  gen_256: if USER_DATA_WIDTH = 256 generate	
     u_fifo_RX : entity work.generic_fifo
     generic map
     (
       WRITE_WIDTH_g             => AURORA_WIDTH,
       READ_WIDTH_g              => USER_DATA_WIDTH,
       WRITE_DEPTH_g             => 1024,
       READ_DEPTH_g              => 512,
       FIRST_WORD_FALL_THROUGH_g => FALSE
     )
     port map
     (
       i_rst_p                   => RX_Fifo_Reset_s,
       i_wr_clk_p                => GTX_fifo_clk_s,
       i_rd_clk_p                => i_UserClk_p,
       iv_din_p                  => v_Aurora_RX_Data_s,
       i_wr_en_p                 => RX_Fifo_Write_Enable_s,
       i_rd_en_p                 => i_RxReadReq_p,
       iv_prog_empty_thresh_p    => x"000F",
       iv_prog_full_thresh_p     => x"00FF",
       ov_dout_p                 => ov_RxData_p,
       o_full_p                  => open,
       o_overflow_p              => open,
       o_empty_p                 => RX_Fifo_Empty_s,
       o_valid_p                 => RX_Fifo_out_valid_s,
       o_underflow_p             => open,
       ov_rd_data_count_p        => open,
       ov_wr_data_count_p        => open,
       o_prog_full_p             => open,
       o_prog_empty_p            => open,
       o_almost_full_p           => open
     );
   
     u_fifo_TX : entity work.generic_fifo
     generic map
     (
       WRITE_WIDTH_g             => USER_DATA_WIDTH,
       READ_WIDTH_g              => AURORA_WIDTH,
       WRITE_DEPTH_g             => 512,
       READ_DEPTH_g              => 1024,
       FIRST_WORD_FALL_THROUGH_g => TRUE
     )
     port map
     (
       i_rst_p                   => TX_Fifo_Reset_s,
       i_wr_clk_p                => i_UserClk_p,
       i_rd_clk_p                => GTX_fifo_clk_s,
       iv_din_p                  => iv_TxData_p,
       i_wr_en_p                 => (i_TxWriteReq_p and tx_ready_s),
       i_rd_en_p                 => TX_Fifo_Read_Enable_s,
       iv_prog_empty_thresh_p    => x"000F",
       iv_prog_full_thresh_p     => x"00FF",
       ov_dout_p                 => v_Aurora_TX_Data_s,
       o_full_p                  => open,
       o_overflow_p              => open,
       o_empty_p                 => TX_Fifo_Empty_s,
       o_valid_p                 => TX_Fifo_out_valid_s,
       o_underflow_p             => open,
       ov_rd_data_count_p        => open,
       ov_wr_data_count_p        => open,
       o_prog_full_p             => open,
       o_prog_empty_p            => open,
       o_almost_full_p           => TX_Fifo_AFull_s
     );
  end generate;
  
  process(i_UserClk_p)
  begin
    if rising_edge(i_UserClk_p) then
        -- Latch almost full signal to ease timing
        TX_Fifo_Full_s <= TX_Fifo_AFull_s;
    end if;
  end process;


  o_RxDataValid_p   <= RX_Fifo_out_valid_s;
  tx_ready_s        <= (not TX_Fifo_Full_s)  and (not TX_Fifo_Reset_s) and Aurora_Enable_Tx_R1_s;
  --o_TxReady_p       <= tx_ready_s and (not TX_Dst_Rdy_n_s);
  o_TxReady_p       <= tx_ready_s;
  o_RxReady_p       <= (not RX_Fifo_Empty_s) and (not RX_Fifo_Reset_s);

  -----------------------------------------------------------------------------------
  -- ReceiveCountPrc : Count the amout of data received by the Aurora core
  -----------------------------------------------------------------------------------
  ReceiveCountPrc : process(GTX_fifo_clk_s)
    begin
      if rising_edge(GTX_fifo_clk_s) then
        if(RX_Fifo_Reset_s = '1') then
          v64_receive_count_s <= (others => '0');
        elsif(RX_Src_Rdy_n_s = '0') then                     --Need to be Ready to receive data
          v64_receive_count_s <= v64_receive_count_s + (AURORA_WIDTH/8);
        end if;
      end if;
  end process ReceiveCountPrc;

  -----------------------------------------------------------------------------------
  -- TransmitCountPrc : Count the amout of data transferred by the Aurora core
  -----------------------------------------------------------------------------------
  TransmitCountPrc : process(GTX_fifo_clk_s)
    begin
      if rising_edge(GTX_fifo_clk_s) then
        if(TX_Fifo_Reset_s = '1') then
          v64_transmit_count_s <= (others => '0');
        elsif((TX_Src_Rdy_n_s nor TX_Dst_Rdy_n_s) = '1') then  --Both need to be Ready to transmit data
          v64_transmit_count_s <= v64_transmit_count_s + (AURORA_WIDTH/8);
        end if;
      end if;
  end process TransmitCountPrc;

  -----------------------------------------------------------------------------------
  -- Latch counters of latch signal rising edge
  -----------------------------------------------------------------------------------
  process(GTX_fifo_clk_s)
  begin
    if rising_edge(GTX_fifo_clk_s) then
        latch_counter_r1_s <= latch_counter_s;
        latch_counter_r2_s <= latch_counter_r1_s;

        if (latch_counter_r1_s = '1' and latch_counter_r2_s = '0') then
            v32_Reg2_s  <= v64_receive_count_s(63 downto 32);
            v32_Reg3_s  <= v64_receive_count_s(31 downto 0);
            v32_Reg4_s <= v64_transmit_count_s(63 downto 32);
            v32_Reg5_s <= v64_transmit_count_s(31 downto 0);
        end if;

    end if;
  end process;

  -----------------------------------------------------------------------------------
  -- Data rate counters
  -----------------------------------------------------------------------------------
  rx_datarate_cnt : entity work.datarate_cnt
  generic map (
    C_REFCLK_FREQ_MHZ   => REFCLK_FREQ_MHZ,
    C_BYTE_PER_EN       => (AURORA_WIDTH/8)
  )
  port map (
    i_RefClk_p          => i_RefClk_p,
    i_Rst_p             => RX_Fifo_Reset_s,
    i_Clk_p             => GTX_fifo_clk_s,
    i_DataEn_p          => RX_Fifo_Write_Enable_s,

    ov32_DataRate_p     => v32_Reg6_s,
    o_Rdy_p             => open
  );

  tx_datarate_cnt : entity work.datarate_cnt
  generic map (
    C_REFCLK_FREQ_MHZ   => REFCLK_FREQ_MHZ,
    C_BYTE_PER_EN       => (AURORA_WIDTH/8)
  )
  port map (
    i_RefClk_p          => i_RefClk_p,
    i_Rst_p             => TX_Fifo_Reset_s,
    i_Clk_p             => GTX_fifo_clk_s,
    i_DataEn_p          => TX_Fifo_out_valid_s,

    ov32_DataRate_p     => v32_Reg7_s,
    o_Rdy_p             => open
  );

end aurora_top_syn;
