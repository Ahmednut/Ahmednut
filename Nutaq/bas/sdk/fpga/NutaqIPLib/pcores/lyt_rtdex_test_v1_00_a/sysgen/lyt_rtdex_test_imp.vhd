
library ieee;
  use ieee.std_logic_1164.all;
  use ieee.numeric_std.all;

entity lyt_rtdex_test_imp is
  port
  (
    clk                        : in std_logic;
    ce                         : in std_logic;

    -- Control ports
    iv8_TxReset_p      : in std_logic_vector(7 downto 0);
    iv8_RxStart_p      : in std_logic_vector(7 downto 0);
    iv8_TxStart_p      : in std_logic_vector(7 downto 0);
    iv8_RxReset_p      : in std_logic_vector(7 downto 0);

    ov32_ErrorCntCh0_p : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh1_p : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh2_p : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh3_p : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh4_p : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh5_p : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh6_p : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh7_p : out std_logic_vector(31 downto 0);
    
    ov32_RxReceivedBytesCh0_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh1_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh2_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh3_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh4_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh5_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh6_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh7_p : out std_logic_vector(31 downto 0);

    iv32_DivntCh0_p      : in std_logic_vector(31 downto 0);
    iv32_DivntCh1_p      : in std_logic_vector(31 downto 0);
    iv32_DivntCh2_p      : in std_logic_vector(31 downto 0);
    iv32_DivntCh3_p      : in std_logic_vector(31 downto 0);
    iv32_DivntCh4_p      : in std_logic_vector(31 downto 0);
    iv32_DivntCh5_p      : in std_logic_vector(31 downto 0);
    iv32_DivntCh6_p      : in std_logic_vector(31 downto 0);
    iv32_DivntCh7_p      : in std_logic_vector(31 downto 0);

    iv32_TxInitDataCh0_p : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh1_p : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh2_p : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh3_p : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh4_p : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh5_p : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh6_p : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh7_p : in std_logic_vector(31 downto 0);

    iv8_RxTxLoopBackEn_p : in std_logic_vector(7 downto 0);

    iv32_RxEnDivCnt_p  : in std_logic_vector(31 downto 0);

    -- User ports
    i_RxReadyCh0_p     : in std_logic;
    o_RxReCh0_p        : out std_logic;
    iv_RxDataCh0_p   : in std_logic_vector(31 downto 0);
    i_RxDataValidCh0_p : in std_logic;

    i_RxReadyCh1_p     : in std_logic;
    o_RxReCh1_p        : out std_logic;
    iv_RxDataCh1_p   : in std_logic_vector(31 downto 0);
    i_RxDataValidCh1_p : in std_logic;

    i_RxReadyCh2_p     : in std_logic;
    o_RxReCh2_p        : out std_logic;
    iv_RxDataCh2_p   : in std_logic_vector(31 downto 0);
    i_RxDataValidCh2_p : in std_logic;

    i_RxReadyCh3_p     : in std_logic;
    o_RxReCh3_p        : out std_logic;
    iv_RxDataCh3_p   : in std_logic_vector(31 downto 0);
    i_RxDataValidCh3_p : in std_logic;

    i_RxReadyCh4_p     : in std_logic;
    o_RxReCh4_p        : out std_logic;
    iv_RxDataCh4_p   : in std_logic_vector(31 downto 0);
    i_RxDataValidCh4_p : in std_logic;

    i_RxReadyCh5_p     : in std_logic;
    o_RxReCh5_p        : out std_logic;
    iv_RxDataCh5_p   : in std_logic_vector(31 downto 0);
    i_RxDataValidCh5_p : in std_logic;

    i_RxReadyCh6_p     : in std_logic;
    o_RxReCh6_p        : out std_logic;
    iv_RxDataCh6_p   : in std_logic_vector(31 downto 0);
    i_RxDataValidCh6_p : in std_logic;

    i_RxReadyCh7_p     : in std_logic;
    o_RxReCh7_p        : out std_logic;
    iv_RxDataCh7_p   : in std_logic_vector(31 downto 0);
    i_RxDataValidCh7_p : in std_logic;

    i_TxReadyCh0_p    : in std_logic;
    o_TxWeCh0_p       : out std_logic;
    ov_TxDataCh0_p  : out std_logic_vector(31 downto 0);

    i_TxReadyCh1_p    : in std_logic;
    o_TxWeCh1_p       : out std_logic;
    ov_TxDataCh1_p  : out std_logic_vector(31 downto 0);

    i_TxReadyCh2_p    : in std_logic;
    o_TxWeCh2_p       : out std_logic;
    ov_TxDataCh2_p  : out std_logic_vector(31 downto 0);

    i_TxReadyCh3_p    : in std_logic;
    o_TxWeCh3_p       : out std_logic;
    ov_TxDataCh3_p  : out std_logic_vector(31 downto 0);

    i_TxReadyCh4_p    : in std_logic;
    o_TxWeCh4_p       : out std_logic;
    ov_TxDataCh4_p  : out std_logic_vector(31 downto 0);

    i_TxReadyCh5_p    : in std_logic;
    o_TxWeCh5_p       : out std_logic;
    ov_TxDataCh5_p  : out std_logic_vector(31 downto 0);

    i_TxReadyCh6_p    : in std_logic;
    o_TxWeCh6_p       : out std_logic;
    ov_TxDataCh6_p  : out std_logic_vector(31 downto 0);

    i_TxReadyCh7_p    : in std_logic;
    o_TxWeCh7_p       : out std_logic;
    ov_TxDataCh7_p  : out std_logic_vector(31 downto 0);

    i_DisableFlowControl_p  : in std_logic;
    
    ov32_TxOverflowCh0_p    : out std_logic_vector(31 downto 0);
    ov32_TxOverflowCh1_p    : out std_logic_vector(31 downto 0);
    ov32_TxOverflowCh2_p    : out std_logic_vector(31 downto 0);
    ov32_TxOverflowCh3_p    : out std_logic_vector(31 downto 0);
    ov32_TxOverflowCh4_p    : out std_logic_vector(31 downto 0);
    ov32_TxOverflowCh5_p    : out std_logic_vector(31 downto 0);
    ov32_TxOverflowCh6_p    : out std_logic_vector(31 downto 0);
    ov32_TxOverflowCh7_p    : out std_logic_vector(31 downto 0);
    
    ov32_RxUnderflowCh0_p    : out std_logic_vector(31 downto 0);
    ov32_RxUnderflowCh1_p    : out std_logic_vector(31 downto 0);
    ov32_RxUnderflowCh2_p    : out std_logic_vector(31 downto 0);
    ov32_RxUnderflowCh3_p    : out std_logic_vector(31 downto 0);
    ov32_RxUnderflowCh4_p    : out std_logic_vector(31 downto 0);
    ov32_RxUnderflowCh5_p    : out std_logic_vector(31 downto 0);
    ov32_RxUnderflowCh6_p    : out std_logic_vector(31 downto 0);
    ov32_RxUnderflowCh7_p    : out std_logic_vector(31 downto 0)

  );
end entity lyt_rtdex_test_imp;

architecture rtl of lyt_rtdex_test_imp is

  component lyt_rtdex_test is
    generic
    (
      C_RTDEX_RX_NUMER_OF_CHANNELS : integer := 1;
      C_RTDEX_TX_NUMER_OF_CHANNELS : integer := 1
    );
    port
    (
      -- Control ports
      iv8_TxReset_p      : in std_logic_vector(7 downto 0);
      iv8_RxStart_p      : in std_logic_vector(7 downto 0);
      iv8_TxStart_p      : in std_logic_vector(7 downto 0);
      iv8_RxReset_p      : in std_logic_vector(7 downto 0);

      ov32_ErrorCntCh0_p : out std_logic_vector(31 downto 0);
      ov32_ErrorCntCh1_p : out std_logic_vector(31 downto 0);
      ov32_ErrorCntCh2_p : out std_logic_vector(31 downto 0);
      ov32_ErrorCntCh3_p : out std_logic_vector(31 downto 0);
      ov32_ErrorCntCh4_p : out std_logic_vector(31 downto 0);
      ov32_ErrorCntCh5_p : out std_logic_vector(31 downto 0);
      ov32_ErrorCntCh6_p : out std_logic_vector(31 downto 0);
      ov32_ErrorCntCh7_p : out std_logic_vector(31 downto 0);
      
      ov32_RxReceivedBytesCh0_p   : out std_logic_vector(31 downto 0);
      ov32_RxReceivedBytesCh1_p   : out std_logic_vector(31 downto 0);
      ov32_RxReceivedBytesCh2_p   : out std_logic_vector(31 downto 0);
      ov32_RxReceivedBytesCh3_p   : out std_logic_vector(31 downto 0);
      ov32_RxReceivedBytesCh4_p   : out std_logic_vector(31 downto 0);
      ov32_RxReceivedBytesCh5_p   : out std_logic_vector(31 downto 0);
      ov32_RxReceivedBytesCh6_p   : out std_logic_vector(31 downto 0);
      ov32_RxReceivedBytesCh7_p   : out std_logic_vector(31 downto 0);

      iv32_DivntCh0_p      : in std_logic_vector(31 downto 0);
      iv32_DivntCh1_p      : in std_logic_vector(31 downto 0);
      iv32_DivntCh2_p      : in std_logic_vector(31 downto 0);
      iv32_DivntCh3_p      : in std_logic_vector(31 downto 0);
      iv32_DivntCh4_p      : in std_logic_vector(31 downto 0);
      iv32_DivntCh5_p      : in std_logic_vector(31 downto 0);
      iv32_DivntCh6_p      : in std_logic_vector(31 downto 0);
      iv32_DivntCh7_p      : in std_logic_vector(31 downto 0);

      iv32_TxInitDataCh0_p : in std_logic_vector(31 downto 0);
      iv32_TxInitDataCh1_p : in std_logic_vector(31 downto 0);
      iv32_TxInitDataCh2_p : in std_logic_vector(31 downto 0);
      iv32_TxInitDataCh3_p : in std_logic_vector(31 downto 0);
      iv32_TxInitDataCh4_p : in std_logic_vector(31 downto 0);
      iv32_TxInitDataCh5_p : in std_logic_vector(31 downto 0);
      iv32_TxInitDataCh6_p : in std_logic_vector(31 downto 0);
      iv32_TxInitDataCh7_p : in std_logic_vector(31 downto 0);

      iv8_RxTxLoopBackEn_p : in std_logic_vector(7 downto 0);

      iv32_RxEnDivCnt_p  : in std_logic_vector(31 downto 0);

      -- User ports
      i_RxUserClk_p      : in std_logic;
      ov8_RxFifoRst_p    : out std_logic_vector(7 downto 0);

      i_RxReadyCh0_p     : in std_logic;
      o_RxReCh0_p        : out std_logic;
      iv_RxDataCh0_p   : in std_logic_vector(31 downto 0);
      i_RxDataValidCh0_p : in std_logic;

      i_RxReadyCh1_p     : in std_logic;
      o_RxReCh1_p        : out std_logic;
      iv_RxDataCh1_p   : in std_logic_vector(31 downto 0);
      i_RxDataValidCh1_p : in std_logic;

      i_RxReadyCh2_p     : in std_logic;
      o_RxReCh2_p        : out std_logic;
      iv_RxDataCh2_p   : in std_logic_vector(31 downto 0);
      i_RxDataValidCh2_p : in std_logic;

      i_RxReadyCh3_p     : in std_logic;
      o_RxReCh3_p        : out std_logic;
      iv_RxDataCh3_p   : in std_logic_vector(31 downto 0);
      i_RxDataValidCh3_p : in std_logic;

      i_RxReadyCh4_p     : in std_logic;
      o_RxReCh4_p        : out std_logic;
      iv_RxDataCh4_p   : in std_logic_vector(31 downto 0);
      i_RxDataValidCh4_p : in std_logic;

      i_RxReadyCh5_p     : in std_logic;
      o_RxReCh5_p        : out std_logic;
      iv_RxDataCh5_p   : in std_logic_vector(31 downto 0);
      i_RxDataValidCh5_p : in std_logic;

      i_RxReadyCh6_p     : in std_logic;
      o_RxReCh6_p        : out std_logic;
      iv_RxDataCh6_p   : in std_logic_vector(31 downto 0);
      i_RxDataValidCh6_p : in std_logic;

      i_RxReadyCh7_p     : in std_logic;
      o_RxReCh7_p        : out std_logic;
      iv_RxDataCh7_p   : in std_logic_vector(31 downto 0);
      i_RxDataValidCh7_p : in std_logic;

      i_TxUserClk_p        : in std_logic;
      ov8_TxFifoRst_p      : out std_logic_vector(7 downto 0);

      i_TxReadyCh0_p    : in std_logic;
      o_TxWeCh0_p       : out std_logic;
      ov_TxDataCh0_p  : out std_logic_vector(31 downto 0);

      i_TxReadyCh1_p    : in std_logic;
      o_TxWeCh1_p       : out std_logic;
      ov_TxDataCh1_p  : out std_logic_vector(31 downto 0);

      i_TxReadyCh2_p    : in std_logic;
      o_TxWeCh2_p       : out std_logic;
      ov_TxDataCh2_p  : out std_logic_vector(31 downto 0);

      i_TxReadyCh3_p    : in std_logic;
      o_TxWeCh3_p       : out std_logic;
      ov_TxDataCh3_p  : out std_logic_vector(31 downto 0);

      i_TxReadyCh4_p    : in std_logic;
      o_TxWeCh4_p       : out std_logic;
      ov_TxDataCh4_p  : out std_logic_vector(31 downto 0);

      i_TxReadyCh5_p    : in std_logic;
      o_TxWeCh5_p       : out std_logic;
      ov_TxDataCh5_p  : out std_logic_vector(31 downto 0);

      i_TxReadyCh6_p    : in std_logic;
      o_TxWeCh6_p       : out std_logic;
      ov_TxDataCh6_p  : out std_logic_vector(31 downto 0);

      i_TxReadyCh7_p    : in std_logic;
      o_TxWeCh7_p       : out std_logic;
      ov_TxDataCh7_p : out std_logic_vector(31 downto 0);

      i_DisableFlowControl_p  : in std_logic;

      ov32_TxOverflowCh0_p    : out std_logic_vector(31 downto 0);
      ov32_TxOverflowCh1_p    : out std_logic_vector(31 downto 0);
      ov32_TxOverflowCh2_p    : out std_logic_vector(31 downto 0);
      ov32_TxOverflowCh3_p    : out std_logic_vector(31 downto 0);
      ov32_TxOverflowCh4_p    : out std_logic_vector(31 downto 0);
      ov32_TxOverflowCh5_p    : out std_logic_vector(31 downto 0);
      ov32_TxOverflowCh6_p    : out std_logic_vector(31 downto 0);
      ov32_TxOverflowCh7_p    : out std_logic_vector(31 downto 0);

      ov32_RxUnderflowCh0_p    : out std_logic_vector(31 downto 0);
      ov32_RxUnderflowCh1_p    : out std_logic_vector(31 downto 0);
      ov32_RxUnderflowCh2_p    : out std_logic_vector(31 downto 0);
      ov32_RxUnderflowCh3_p    : out std_logic_vector(31 downto 0);
      ov32_RxUnderflowCh4_p    : out std_logic_vector(31 downto 0);
      ov32_RxUnderflowCh5_p    : out std_logic_vector(31 downto 0);
      ov32_RxUnderflowCh6_p    : out std_logic_vector(31 downto 0);
      ov32_RxUnderflowCh7_p    : out std_logic_vector(31 downto 0)
    );
  end component;


begin

  lyt_rtdex_test_inst : lyt_rtdex_test
    generic map
    (
      C_RTDEX_RX_NUMER_OF_CHANNELS => 8,
      C_RTDEX_TX_NUMER_OF_CHANNELS => 8
    )
    port map
    (
      -- Control ports
      iv8_TxReset_p      => iv8_TxReset_p,
      iv8_RxStart_p      => iv8_RxStart_p,
      iv8_TxStart_p      => iv8_TxStart_p,
      iv8_RxReset_p      => iv8_RxReset_p,

      ov32_ErrorCntCh0_p  => ov32_ErrorCntCh0_p,
      ov32_ErrorCntCh1_p  => ov32_ErrorCntCh1_p,
      ov32_ErrorCntCh2_p  => ov32_ErrorCntCh2_p,
      ov32_ErrorCntCh3_p  => ov32_ErrorCntCh3_p,
      ov32_ErrorCntCh4_p  => ov32_ErrorCntCh4_p,
      ov32_ErrorCntCh5_p  => ov32_ErrorCntCh5_p,
      ov32_ErrorCntCh6_p  => ov32_ErrorCntCh6_p,
      ov32_ErrorCntCh7_p  => ov32_ErrorCntCh7_p,
      
      ov32_RxReceivedBytesCh0_p => ov32_RxReceivedBytesCh0_p,
      ov32_RxReceivedBytesCh1_p => ov32_RxReceivedBytesCh1_p,
      ov32_RxReceivedBytesCh2_p => ov32_RxReceivedBytesCh2_p,
      ov32_RxReceivedBytesCh3_p => ov32_RxReceivedBytesCh3_p,
      ov32_RxReceivedBytesCh4_p => ov32_RxReceivedBytesCh4_p,
      ov32_RxReceivedBytesCh5_p => ov32_RxReceivedBytesCh5_p,
      ov32_RxReceivedBytesCh6_p => ov32_RxReceivedBytesCh6_p,
      ov32_RxReceivedBytesCh7_p => ov32_RxReceivedBytesCh7_p,
      

      iv32_DivntCh0_p     => iv32_DivntCh0_p,
      iv32_DivntCh1_p     => iv32_DivntCh1_p,
      iv32_DivntCh2_p     => iv32_DivntCh2_p,
      iv32_DivntCh3_p     => iv32_DivntCh3_p,
      iv32_DivntCh4_p     => iv32_DivntCh4_p,
      iv32_DivntCh5_p     => iv32_DivntCh5_p,
      iv32_DivntCh6_p     => iv32_DivntCh6_p,
      iv32_DivntCh7_p     => iv32_DivntCh7_p,

      iv32_TxInitDataCh0_p => iv32_TxInitDataCh0_p,
      iv32_TxInitDataCh1_p => iv32_TxInitDataCh1_p,
      iv32_TxInitDataCh2_p => iv32_TxInitDataCh2_p,
      iv32_TxInitDataCh3_p => iv32_TxInitDataCh3_p,
      iv32_TxInitDataCh4_p => iv32_TxInitDataCh4_p,
      iv32_TxInitDataCh5_p => iv32_TxInitDataCh5_p,
      iv32_TxInitDataCh6_p => iv32_TxInitDataCh6_p,
      iv32_TxInitDataCh7_p => iv32_TxInitDataCh7_p,

      iv8_RxTxLoopBackEn_p => iv8_RxTxLoopBackEn_p,

      iv32_RxEnDivCnt_p => iv32_RxEnDivCnt_p,

      -- User ports
      i_RxUserClk_p      => clk,
      ov8_RxFifoRst_p    => open,

      i_RxReadyCh0_p     => i_RxReadyCh0_p,
      o_RxReCh0_p        => o_RxReCh0_p,
      iv_RxDataCh0_p     => iv_RxDataCh0_p,
      i_RxDataValidCh0_p => i_RxDataValidCh0_p,

      i_RxReadyCh1_p     => i_RxReadyCh1_p,
      o_RxReCh1_p        => o_RxReCh1_p,
      iv_RxDataCh1_p     => iv_RxDataCh1_p,
      i_RxDataValidCh1_p => i_RxDataValidCh1_p,

      i_RxReadyCh2_p     => i_RxReadyCh2_p,
      o_RxReCh2_p        => o_RxReCh2_p,
      iv_RxDataCh2_p     => iv_RxDataCh2_p,
      i_RxDataValidCh2_p => i_RxDataValidCh2_p,

      i_RxReadyCh3_p     => i_RxReadyCh3_p,
      o_RxReCh3_p        => o_RxReCh3_p,
      iv_RxDataCh3_p     => iv_RxDataCh3_p,
      i_RxDataValidCh3_p => i_RxDataValidCh3_p,

      i_RxReadyCh4_p     => i_RxReadyCh4_p,
      o_RxReCh4_p        => o_RxReCh4_p,
      iv_RxDataCh4_p     => iv_RxDataCh4_p,
      i_RxDataValidCh4_p => i_RxDataValidCh4_p,

      i_RxReadyCh5_p     => i_RxReadyCh5_p,
      o_RxReCh5_p        => o_RxReCh5_p,
      iv_RxDataCh5_p     => iv_RxDataCh5_p,
      i_RxDataValidCh5_p => i_RxDataValidCh5_p,

      i_RxReadyCh6_p     => i_RxReadyCh6_p,
      o_RxReCh6_p        => o_RxReCh6_p,
      iv_RxDataCh6_p     => iv_RxDataCh6_p,
      i_RxDataValidCh6_p => i_RxDataValidCh6_p,

      i_RxReadyCh7_p     => i_RxReadyCh7_p,
      o_RxReCh7_p        => o_RxReCh7_p,
      iv_RxDataCh7_p     => iv_RxDataCh7_p,
      i_RxDataValidCh7_p => i_RxDataValidCh7_p,

      i_TxUserClk_p      => clk,
      ov8_TxFifoRst_p    => open,

      i_TxReadyCh0_p    => i_TxReadyCh0_p,
      o_TxWeCh0_p       => o_TxWeCh0_p,
      ov_TxDataCh0_p     => ov_TxDataCh0_p,

      i_TxReadyCh1_p    => i_TxReadyCh1_p,
      o_TxWeCh1_p       => o_TxWeCh1_p,
      ov_TxDataCh1_p    => ov_TxDataCh1_p,

      i_TxReadyCh2_p    => i_TxReadyCh2_p,
      o_TxWeCh2_p       => o_TxWeCh2_p,
      ov_TxDataCh2_p    => ov_TxDataCh2_p,

      i_TxReadyCh3_p    => i_TxReadyCh3_p,
      o_TxWeCh3_p       => o_TxWeCh3_p,
      ov_TxDataCh3_p    => ov_TxDataCh3_p,

      i_TxReadyCh4_p    => i_TxReadyCh4_p,
      o_TxWeCh4_p       => o_TxWeCh4_p,
      ov_TxDataCh4_p    => ov_TxDataCh4_p,

      i_TxReadyCh5_p    => i_TxReadyCh5_p,
      o_TxWeCh5_p       => o_TxWeCh5_p,
      ov_TxDataCh5_p    => ov_TxDataCh5_p,

      i_TxReadyCh6_p    => i_TxReadyCh6_p,
      o_TxWeCh6_p       => o_TxWeCh6_p,
      ov_TxDataCh6_p    => ov_TxDataCh6_p,

      i_TxReadyCh7_p    => i_TxReadyCh7_p,
      o_TxWeCh7_p       => o_TxWeCh7_p,
      ov_TxDataCh7_p    => ov_TxDataCh7_p,

      i_DisableFlowControl_p  => i_DisableFlowControl_p,

      ov32_TxOverflowCh0_p    => ov32_TxOverflowCh0_p ,
      ov32_TxOverflowCh1_p    => ov32_TxOverflowCh1_p ,
      ov32_TxOverflowCh2_p    => ov32_TxOverflowCh2_p ,
      ov32_TxOverflowCh3_p    => ov32_TxOverflowCh3_p ,
      ov32_TxOverflowCh4_p    => ov32_TxOverflowCh4_p ,
      ov32_TxOverflowCh5_p    => ov32_TxOverflowCh5_p ,
      ov32_TxOverflowCh6_p    => ov32_TxOverflowCh6_p ,
      ov32_TxOverflowCh7_p    => ov32_TxOverflowCh7_p ,
                                 
      ov32_RxUnderflowCh0_p   => ov32_RxUnderflowCh0_p,
      ov32_RxUnderflowCh1_p   => ov32_RxUnderflowCh1_p,
      ov32_RxUnderflowCh2_p   => ov32_RxUnderflowCh2_p,
      ov32_RxUnderflowCh3_p   => ov32_RxUnderflowCh3_p,
      ov32_RxUnderflowCh4_p   => ov32_RxUnderflowCh4_p,
      ov32_RxUnderflowCh5_p   => ov32_RxUnderflowCh5_p,
      ov32_RxUnderflowCh6_p   => ov32_RxUnderflowCh6_p,
      ov32_RxUnderflowCh7_p   => ov32_RxUnderflowCh7_p

    );

end rtl;

