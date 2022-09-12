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
-- File        : $Id: lvds14sync.vhd,v 1.1 2015/04/10 19:47:32 sqa Exp $
--------------------------------------------------------------------------------
-- Description :
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lvds14sync.vhd,v $
-- Revision 1.1  2015/04/10 19:47:32  sqa
-- bas first commit (CBOU)
--
-- Revision 1.6  2013/02/08 14:04:36  julien.roy
-- Add IDELAY to ease timing in sync mode
--
-- Revision 1.5  2013/01/30 19:52:04  julien.roy
-- Modify LVDS example for Synchronous examples
--
--------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity lvds14sync is
  generic(
    DATA_IDELAY_VALUE : integer := 0;
    CLK_IDELAY_VALUE  : integer := 0
  );
  port(
    i_rst_p           : in  std_logic;
    i_SystemClk_p     : in  std_logic;
    i_clock_p         : in  std_logic;
    i_txClock_p       : in  std_logic;
    o_rxClock_p       : out std_logic;
    
    i_direction_p     : in  std_logic; -- 3-state enable input, high=input, low=output

    -- TX FIFO
    iv14_input_p      : in  std_logic_vector( 13 downto 0 ); -- data input TX
    i_inWrEn_p        : in  std_logic;
    o_inWrAck_p       : out std_logic;
    o_full_p          : out std_logic;
    
    -- RX FIFO
    ov14_output_p     : out std_logic_vector( 13 downto 0 ); -- data output RX
    i_outRdEn_p       : in  std_logic;
    o_outValid_p      : out std_logic;
    o_empty_p         : out std_logic;

    -- external
    lvdsClk_p         : inout std_logic;
    lvdsClk_n         : inout std_logic;
    lvdsValid_p       : inout std_logic;
    lvdsValid_n       : inout std_logic;
    lvdsDataIO_p      : inout  std_logic_vector( 13 downto 0 );
    lvdsDataIO_n      : inout  std_logic_vector( 13 downto 0 )
  );
end lvds14sync;

architecture Behavioral of lvds14sync is

  ----------------------------------------
  -- Component declaration
  ----------------------------------------

  component fifo14w16d
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(13 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      dout : out std_logic_vector(13 downto 0);
      full : out std_logic;
      almost_full : out std_logic;
      wr_ack : out std_logic;
      empty : out std_logic;
      almost_empty : out std_logic;
      valid : out std_logic
    );
  end component;

  component io_dff
    generic (
      io_dff_Falling_edge : boolean := false
    );
    port (
      clk     : in std_logic;
      din     : in std_logic;
      dout    : out std_logic
    );
  end component;

  ----------------------------------------
  -- Signal declaration
  ----------------------------------------
  
  signal RxClockBuffIdelay_s                : std_logic;
  signal lvdsrxvalidIdelay_s                : std_logic;
  signal lvdsrxdataIdelay_s                 : std_logic_vector(13 downto 0);

  signal RxClockBuff_s, TxClockBuff_s       : std_logic;

  signal BuffIOclk_s, RxValid_s             : std_logic;
  signal RxData_s,lvdsrxdata_s, TxIOBUFDS_s : std_logic_vector(13 downto 0);

  signal lvdsrxvalid_s, TxValidIOBUFDS_s    : std_logic;

  signal fifo_tx_valid                      : std_logic;
  signal fifo_tx_dout                       : std_logic_vector(13 downto 0);

begin


  ----------------------------------------
  -- RX FIFO
  ----------------------------------------
  fifo_rx : fifo14w16d
    port map (
     rst            => i_rst_p,
     wr_clk         => BuffIOclk_s,
     rd_clk         => i_clock_p,
     din            => RxData_s,
     wr_en          => RxValid_s,
     rd_en          => i_outRdEn_p,
     dout           => ov14_output_p,
     full           => open,
     almost_full    => open,
     wr_ack         => open,
     empty          => o_empty_p,
     almost_empty   => open,
     valid          => o_outValid_p
    );

  ----------------------------------------
  -- TX FIFO
  ----------------------------------------    
  fifo_tx : fifo14w16d
    PORT MAP (
     rst            => i_rst_p,
     wr_clk         => i_clock_p,
     rd_clk         => i_txClock_p,
     din            => iv14_input_p,
     wr_en          => i_inWrEn_p,
     rd_en          => '1',
     dout           => fifo_tx_dout,
     full           => o_full_p,
     almost_full    => open,
     wr_ack         => o_inWrAck_p,
     empty          => open,
     almost_empty   => open,
     valid          => fifo_tx_valid
    );

  ----------------------------------------
  -- Clock buffer
  ----------------------------------------
   IOBUFDS_rxClk : IOBUFDS
   generic map (
      IOSTANDARD => "BLVDS_25")
   port map (
      O     => RxClockBuff_s, -- Buffer output
      IO    => lvdsClk_p,     -- Diff_p inout (connect directly to top-level port)
      IOB   => lvdsClk_n,     -- Diff_n inout (connect directly to top-level port)
      I     => TxClockBuff_s, -- Buffer input
      T     => i_direction_p  -- 3-state enable input, high=input, low=output
   );
   
  clk_idelay_inst : IDELAY
    generic map (
      IOBDELAY_TYPE => "FIXED",
      IOBDELAY_VALUE => CLK_IDELAY_VALUE)
    port map (
      O 		=> RxClockBuffIdelay_s,
      I 		=> RxClockBuff_s,
      C 		=> '0',
      CE 	=> '0',
      INC 	=> '0',
      RST 	=> '0'
    );

  -- RX BUFG clock
  BUFG_inst : BUFG
    port map (
      O => BuffIOclk_s,     -- 1-bit output: Clock buffer output
      I => RxClockBuffIdelay_s  -- 1-bit input: Clock buffer input
    );

  -- Generate TX clock output from i_txClock_p
  ODDR_TX_CLOCK : ODDR
    generic map(
      DDR_CLK_EDGE   => "OPPOSITE_EDGE",
      INIT           => '0',
      SRTYPE         => "ASYNC")
    port map(
      Q           => TxClockBuff_s,
      C           => i_txClock_p,
      CE          => '1',
      D1          => '1',
      D2          => '0',
      R           => '0',
      S           => '0'
    );


  ----------------------------------------
  -- Data valid buffer
  ----------------------------------------
  IOBUFDS_txData : IOBUFDS
    generic map (
      IOSTANDARD => "BLVDS_25")
    port map (
      O     => lvdsrxvalid_s,  -- Buffer output
      IO    => lvdsValid_p,    -- Diff_p inout (connect directly to top-level port)
      IOB   => lvdsValid_n,    -- Diff_n inout (connect directly to top-level port)
      I     => TxValidIOBUFDS_s,  -- Buffer input
      T     => i_direction_p   -- 3-state enable input, high=input, low=output
    );
    
  valid_idelay_inst : IDELAY
    generic map (
      IOBDELAY_TYPE => "FIXED",
      IOBDELAY_VALUE => DATA_IDELAY_VALUE)
    port map (
      O 		=> lvdsrxvalidIdelay_s,
      I 		=> lvdsrxvalid_s,
      C 		=> '0',
      CE 	=> '0',
      INC 	=> '0',
      RST 	=> '0'
    );

  -- Input data valid flipflop
  io_dff_rxValid : io_dff
    generic map (
      io_dff_Falling_edge => false
    )
    port map(
      clk   => BuffIOclk_s,
      din   => lvdsrxvalidIdelay_s,
      dout  => RxValid_s 
    );
    
  -- Output data valid flipflop
  io_dff_U2 : io_dff
    generic map (
      io_dff_Falling_edge => false
    )
    port map(
      clk   => i_txClock_p,
      din   => fifo_tx_valid,
      dout  => TxValidIOBUFDS_s
    );

  ----------------------------------------
  -- Data buffers
  ----------------------------------------
  INPUT_RX_TX_BUFFER_GENERATE : for i in 0 to 13 generate

    -- data IOBUFDS
    IOBUFDS_RxTxData : IOBUFDS
      generic map (
        IOSTANDARD => "BLVDS_25")
      port map (
        O     => lvdsrxdata_s(i),   -- Buffer output
        IO    => lvdsDataIO_p(i),   -- Diff_p inout (connect directly to top-level port)
        IOB   => lvdsDataIO_n(i),   -- Diff_n inout (connect directly to top-level port)
        I     => TxIOBUFDS_s(i),    -- Buffer input
        T     => i_direction_p      -- 3-state enable input, high=input, low=output
      );
      
    data_idelay_inst : IDELAY
      generic map (
        IOBDELAY_TYPE => "FIXED",
        IOBDELAY_VALUE => DATA_IDELAY_VALUE)
      port map (
        O 		=> lvdsrxdataIdelay_s(i),
        I 		=> lvdsrxdata_s(i),
        C 		=> '0',
        CE 	=> '0',
        INC 	=> '0',
        RST 	=> '0'
      );

    -- Input data flipflop
    io_dff_U0 : io_dff
      generic map (
        io_dff_Falling_edge => false
      )
      port map(
        clk   => BuffIOclk_s,
        din   => lvdsrxdataIdelay_s(i),
        dout  => RxData_s(i)
      );

    -- Output data flipflop
    io_dff_U1 : io_dff
      generic map (
        io_dff_Falling_edge => false
      )
      port map(
        clk   => i_txClock_p,
        din   => fifo_tx_dout(i),
        dout  => TxIOBUFDS_s(i)
      );

  end generate  INPUT_RX_TX_BUFFER_GENERATE;

  ----------------------------------------
  -- Output assignments
  ----------------------------------------
  
  o_rxClock_p <= BuffIOclk_s;

end Behavioral;

