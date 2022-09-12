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
-- File : lyt_rtdex_test.vhd
--------------------------------------------------------------------------------
-- Description : RTDEx test core
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_rtdex_test.vhd,v $
-- Revision 1.3  2015/10/14 20:50:40  christian.dufour-bou
-- Added sample counter
--
-- Revision 1.2  2015/07/27 19:02:49  christian.dufour-bou
-- added temp error count so that core doesn't count errors after rtdex has terminated transfer
--
-- Revision 1.1  2015/04/10 19:49:01  sqa
-- bas first commit
--
-- Revision 1.8  2014/11/19 14:49:57  julien.roy
-- Add tig constraint on enable flow control.
-- Set default mode to resync automatically.
--
-- Revision 1.7  2014/10/20 17:58:11  david.quinn
-- Add generic for sync recover or not after error. Change flow control command behaviour.
--
-- Revision 1.6  2014/05/02 15:42:10  julien.roy
-- Add 1 more delay to v8_DataValidExpected_s to match read operation
--
-- Revision 1.5  2014/04/28 19:54:06  julien.roy
-- Add possibility to disable flow control to monitor RTDEx overflow and underflow
--
-- Revision 1.4  2013/09/23 17:34:50  julien.roy
-- Add data width generic parameter
--
-- Revision 1.3  2013/04/12 13:13:39  julien.roy
-- Disable "keep_hierarchy"
--
-- Revision 1.2  2013/04/08 14:33:37  julien.roy
-- Remove RxReady signals for Read signals to ease timing.
-- Suppose to make no difference since Read will be ignore if the FIFO is not Ready
--
-- Revision 1.1  2013/04/03 14:03:55  julien.roy
-- Commit new RTDEx and RecPlay test pcore. These new pcore does not have an AXI interface and they use Custom Registers for configuration.
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_misc.all;

entity lyt_rtdex_test is
  generic
  (
    C_RTDEX_RX_NUMER_OF_CHANNELS : integer := 1;
    C_RTDEX_TX_NUMER_OF_CHANNELS : integer := 1;
    C_RTDEX_DATA_WIDTH           : integer := 32;
    C_RECOVER_SYNC_AFTER_ERROR   : integer range 0 to 1 := 1
  );
  port
  (
    -- Control ports
    iv8_TxReset_p             : in std_logic_vector(7 downto 0);
    iv8_RxStart_p             : in std_logic_vector(7 downto 0);
    iv8_TxStart_p             : in std_logic_vector(7 downto 0);
    iv8_RxReset_p             : in std_logic_vector(7 downto 0);
    
    ov32_ErrorCntCh0_p        : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh1_p        : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh2_p        : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh3_p        : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh4_p        : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh5_p        : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh6_p        : out std_logic_vector(31 downto 0);
    ov32_ErrorCntCh7_p        : out std_logic_vector(31 downto 0);
    
    ov32_RxReceivedBytesCh0_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh1_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh2_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh3_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh4_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh5_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh6_p : out std_logic_vector(31 downto 0);
    ov32_RxReceivedBytesCh7_p : out std_logic_vector(31 downto 0);

    iv32_DivntCh0_p           : in std_logic_vector(31 downto 0);
    iv32_DivntCh1_p           : in std_logic_vector(31 downto 0);
    iv32_DivntCh2_p           : in std_logic_vector(31 downto 0);
    iv32_DivntCh3_p           : in std_logic_vector(31 downto 0);
    iv32_DivntCh4_p           : in std_logic_vector(31 downto 0);
    iv32_DivntCh5_p           : in std_logic_vector(31 downto 0);
    iv32_DivntCh6_p           : in std_logic_vector(31 downto 0);
    iv32_DivntCh7_p           : in std_logic_vector(31 downto 0);
        
    iv32_TxInitDataCh0_p      : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh1_p      : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh2_p      : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh3_p      : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh4_p      : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh5_p      : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh6_p      : in std_logic_vector(31 downto 0);
    iv32_TxInitDataCh7_p      : in std_logic_vector(31 downto 0);
        
    iv8_RxTxLoopBackEn_p      : in std_logic_vector(7 downto 0);
    
    iv32_RxEnDivCnt_p         : in std_logic_vector(31 downto 0);
        
    -- User ports   
    i_RxUserClk_p             : in std_logic;
    ov8_RxFifoRst_p           : out std_logic_vector(7 downto 0);
    
    i_RxReadyCh0_p            : in std_logic;
    o_RxReCh0_p               : out std_logic;  
    iv_RxDataCh0_p            : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    i_RxDataValidCh0_p        : in std_logic;
        
    i_RxReadyCh1_p            : in std_logic;
    o_RxReCh1_p               : out std_logic;
    iv_RxDataCh1_p            : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    i_RxDataValidCh1_p        : in std_logic;
        
    i_RxReadyCh2_p            : in std_logic;
    o_RxReCh2_p               : out std_logic;
    iv_RxDataCh2_p            : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    i_RxDataValidCh2_p        : in std_logic;
        
    i_RxReadyCh3_p            : in std_logic;
    o_RxReCh3_p               : out std_logic;
    iv_RxDataCh3_p            : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    i_RxDataValidCh3_p        : in std_logic;
        
    i_RxReadyCh4_p            : in std_logic;
    o_RxReCh4_p               : out std_logic;
    iv_RxDataCh4_p            : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    i_RxDataValidCh4_p        : in std_logic;
        
    i_RxReadyCh5_p            : in std_logic;
    o_RxReCh5_p               : out std_logic;
    iv_RxDataCh5_p            : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    i_RxDataValidCh5_p        : in std_logic;
        
    i_RxReadyCh6_p            : in std_logic;
    o_RxReCh6_p               : out std_logic;
    iv_RxDataCh6_p            : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    i_RxDataValidCh6_p        : in std_logic;
        
    i_RxReadyCh7_p            : in std_logic;
    o_RxReCh7_p               : out std_logic;
    iv_RxDataCh7_p            : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    i_RxDataValidCh7_p        : in std_logic;
        
    i_TxUserClk_p             : in std_logic;
    ov8_TxFifoRst_p           : out std_logic_vector(7 downto 0);
    
    i_TxReadyCh0_p            : in std_logic;
    o_TxWeCh0_p               : out std_logic;
    ov_TxDataCh0_p            : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
    i_TxReadyCh1_p            : in std_logic;
    o_TxWeCh1_p               : out std_logic;
    ov_TxDataCh1_p            : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
    i_TxReadyCh2_p            : in std_logic;
    o_TxWeCh2_p               : out std_logic;
    ov_TxDataCh2_p            : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
    i_TxReadyCh3_p            : in std_logic;
    o_TxWeCh3_p               : out std_logic;
    ov_TxDataCh3_p            : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
    i_TxReadyCh4_p            : in std_logic;
    o_TxWeCh4_p               : out std_logic;
    ov_TxDataCh4_p            : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
    i_TxReadyCh5_p            : in std_logic;
    o_TxWeCh5_p               : out std_logic;
    ov_TxDataCh5_p            : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
    i_TxReadyCh6_p            : in std_logic;
    o_TxWeCh6_p               : out std_logic;
    ov_TxDataCh6_p            : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
    i_TxReadyCh7_p            : in std_logic;
    o_TxWeCh7_p               : out std_logic;
    ov_TxDataCh7_p            : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
    
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
end entity lyt_rtdex_test;

architecture rtl of lyt_rtdex_test is  
  
  signal v8_TxReset_s         : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_RxReset_s         : std_logic_vector(7 downto 0) := (others => '0');
  
  signal v8_TxWe_s            : std_logic_vector(7 downto 0) := (others => '0');

  signal v8_RxStart_s         : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_RxStartDly_s      : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_TxStart_s         : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_TxStartDly_s      : std_logic_vector(7 downto 0) := (others => '0');

  type array8_u_t is array(7 downto 0) of unsigned(C_RTDEX_DATA_WIDTH-1 downto 0);
  type array8_v16_t is array(7 downto 0) of std_logic_vector(15 downto 0);
  type array8_v9_t is array(7 downto 0) of std_logic_vector(8 downto 0);
  type array8_v32_t is array(7 downto 0) of std_logic_vector(31 downto 0);

  signal a8u_DataGen_s      : array8_u_t;
  signal a8u_ExpectedData_s : array8_u_t;
  signal a8u_RecvData_s     : array8_u_t;
  signal a8u_RecvDataDly_s  : array8_u_t;
  
  -- 8-bit counters + 1 bit for Overflow detection
  signal a8v32_RxErrorCnt_s       : array8_v32_t;
  signal a8v32_RxErrorCntTemp_s   : array8_v32_t;
  signal v8_RxErrorDet_s          : std_logic_vector(7 downto 0);
  signal a8v32_RxReceivedBytes_s  : array8_v32_t;
  
  signal a8v16_DivCntVal_s    : array8_v16_t;
  signal a8v16_DivCounter_s   : array8_v16_t;
  signal a8u_TxInitData_s  : array8_u_t;
  signal v8_TxReady_s         : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_TxReadyR1_s       : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_TxReadyStarted_s  : std_logic_vector(7 downto 0) := (others => '0');
  
  signal v8_RxDataValid_s     : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_RxDataValidDly_s  : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_RxReady_s         : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_RxReadyR1_s         : std_logic_vector(7 downto 0) := (others => '0');

  signal v8_rxStartTestRedge_s: std_logic_vector(7 downto 0) := (others => '0');
  signal v8_txStartTestRedge_s: std_logic_vector(7 downto 0) := (others => '0');
  
  signal a8v32_RxEnDivCnt_s         : array8_v32_t;
  signal v8_RxReDiv_s               : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_RxReDiv_r1              : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_RxReDiv_r2              : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_DataValidExpected_s     : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_DataValidExpectedDly_s  : std_logic_vector(7 downto 0) := (others => '0');
  
  signal a8v32_TxOverflow_s             : array8_v32_t;
  signal a8v32_TxOverflowTemp_s         : array8_v32_t;
  signal a8v32_RxUnderflow_s            : array8_v32_t;
  signal a8v32_RxUnderflowTemp_s        : array8_v32_t;
  signal v8_DisableFlowControlTX_r1_s   : std_logic_vector(7 downto 0) := (others => '0');
  signal v8_DisableFlowControlTX_r2_s   : std_logic_vector(7 downto 0) := (others => '0');
  
  signal disableFlowControl_s           : std_logic;
  signal v8_RxReadyStarted_s            : std_logic_vector(7 downto 0) := (others => '0');
  

  attribute keep : string;  
  attribute keep of disableFlowControl_s     : signal is "true";  

begin

  --- Input ports ---
  a8u_TxInitData_s(0)<= unsigned(iv32_TxInitDataCh0_p(a8u_TxInitData_s(0)'range));
  a8u_TxInitData_s(1)<= unsigned(iv32_TxInitDataCh1_p(a8u_TxInitData_s(1)'range));
  a8u_TxInitData_s(2)<= unsigned(iv32_TxInitDataCh2_p(a8u_TxInitData_s(2)'range));
  a8u_TxInitData_s(3)<= unsigned(iv32_TxInitDataCh3_p(a8u_TxInitData_s(3)'range));
  a8u_TxInitData_s(4)<= unsigned(iv32_TxInitDataCh4_p(a8u_TxInitData_s(4)'range));
  a8u_TxInitData_s(5)<= unsigned(iv32_TxInitDataCh5_p(a8u_TxInitData_s(5)'range));
  a8u_TxInitData_s(6)<= unsigned(iv32_TxInitDataCh6_p(a8u_TxInitData_s(6)'range));
  a8u_TxInitData_s(7)<= unsigned(iv32_TxInitDataCh7_p(a8u_TxInitData_s(7)'range));
 
  a8v16_DivCntVal_s(0) <= iv32_DivntCh0_p(15 downto 0);
  a8v16_DivCntVal_s(1) <= iv32_DivntCh1_p(15 downto 0);
  a8v16_DivCntVal_s(2) <= iv32_DivntCh2_p(15 downto 0);
  a8v16_DivCntVal_s(3) <= iv32_DivntCh3_p(15 downto 0);
  a8v16_DivCntVal_s(4) <= iv32_DivntCh4_p(15 downto 0);
  a8v16_DivCntVal_s(5) <= iv32_DivntCh5_p(15 downto 0);
  a8v16_DivCntVal_s(6) <= iv32_DivntCh6_p(15 downto 0);
  a8v16_DivCntVal_s(7) <= iv32_DivntCh7_p(15 downto 0);

  -- Output ports
  
  -- NO FIFO protection in the case of loopback
  -- This is to avoid extra logic that might bring some timming issues.
  --
  o_TxWeCh0_p <= v8_TxWe_s(0) and v8_TxReady_s(0) when iv8_RxTxLoopBackEn_p(0) = '0' else v8_RxDataValidDly_s(0);
  o_TxWeCh1_p <= v8_TxWe_s(1) and v8_TxReady_s(1) when iv8_RxTxLoopBackEn_p(1) = '0' else v8_RxDataValidDly_s(1);
  o_TxWeCh2_p <= v8_TxWe_s(2) and v8_TxReady_s(2) when iv8_RxTxLoopBackEn_p(2) = '0' else v8_RxDataValidDly_s(2);
  o_TxWeCh3_p <= v8_TxWe_s(3) and v8_TxReady_s(3) when iv8_RxTxLoopBackEn_p(3) = '0' else v8_RxDataValidDly_s(3);
  o_TxWeCh4_p <= v8_TxWe_s(4) and v8_TxReady_s(4) when iv8_RxTxLoopBackEn_p(4) = '0' else v8_RxDataValidDly_s(4);
  o_TxWeCh5_p <= v8_TxWe_s(5) and v8_TxReady_s(5) when iv8_RxTxLoopBackEn_p(5) = '0' else v8_RxDataValidDly_s(5);
  o_TxWeCh6_p <= v8_TxWe_s(6) and v8_TxReady_s(6) when iv8_RxTxLoopBackEn_p(6) = '0' else v8_RxDataValidDly_s(6);
  o_TxWeCh7_p <= v8_TxWe_s(7) and v8_TxReady_s(7) when iv8_RxTxLoopBackEn_p(7) = '0' else v8_RxDataValidDly_s(7);
    
  o_RxReCh0_p <= v8_RxReDiv_r1(0) and (v8_RxReady_s(0) or disableFlowControl_s);
  o_RxReCh1_p <= v8_RxReDiv_r1(1) and (v8_RxReady_s(1) or disableFlowControl_s);
  o_RxReCh2_p <= v8_RxReDiv_r1(2) and (v8_RxReady_s(2) or disableFlowControl_s);
  o_RxReCh3_p <= v8_RxReDiv_r1(3) and (v8_RxReady_s(3) or disableFlowControl_s);
  o_RxReCh4_p <= v8_RxReDiv_r1(4) and (v8_RxReady_s(4) or disableFlowControl_s);
  o_RxReCh5_p <= v8_RxReDiv_r1(5) and (v8_RxReady_s(5) or disableFlowControl_s);
  o_RxReCh6_p <= v8_RxReDiv_r1(6) and (v8_RxReady_s(6) or disableFlowControl_s);
  o_RxReCh7_p <= v8_RxReDiv_r1(7) and (v8_RxReady_s(7) or disableFlowControl_s);
  
  ov_TxDataCh0_p <= std_logic_vector(a8u_DataGen_s(0));
  ov_TxDataCh1_p <= std_logic_vector(a8u_DataGen_s(1));
  ov_TxDataCh2_p <= std_logic_vector(a8u_DataGen_s(2));
  ov_TxDataCh3_p <= std_logic_vector(a8u_DataGen_s(3));
  ov_TxDataCh4_p <= std_logic_vector(a8u_DataGen_s(4));
  ov_TxDataCh5_p <= std_logic_vector(a8u_DataGen_s(5));
  ov_TxDataCh6_p <= std_logic_vector(a8u_DataGen_s(6));
  ov_TxDataCh7_p <= std_logic_vector(a8u_DataGen_s(7));

  ov32_ErrorCntCh0_p <=a8v32_RxErrorCnt_s(0);
  ov32_ErrorCntCh1_p <=a8v32_RxErrorCnt_s(1);
  ov32_ErrorCntCh2_p <=a8v32_RxErrorCnt_s(2);
  ov32_ErrorCntCh3_p <=a8v32_RxErrorCnt_s(3);
  ov32_ErrorCntCh4_p <=a8v32_RxErrorCnt_s(4);
  ov32_ErrorCntCh5_p <=a8v32_RxErrorCnt_s(5);
  ov32_ErrorCntCh6_p <=a8v32_RxErrorCnt_s(6);
  ov32_ErrorCntCh7_p <=a8v32_RxErrorCnt_s(7);
  
  ov32_RxReceivedBytesCh0_p <= a8v32_RxReceivedBytes_s(0);
  ov32_RxReceivedBytesCh1_p <= a8v32_RxReceivedBytes_s(1);
  ov32_RxReceivedBytesCh2_p <= a8v32_RxReceivedBytes_s(2);
  ov32_RxReceivedBytesCh3_p <= a8v32_RxReceivedBytes_s(3);
  ov32_RxReceivedBytesCh4_p <= a8v32_RxReceivedBytes_s(4);
  ov32_RxReceivedBytesCh5_p <= a8v32_RxReceivedBytes_s(5);
  ov32_RxReceivedBytesCh6_p <= a8v32_RxReceivedBytes_s(6);
  ov32_RxReceivedBytesCh7_p <= a8v32_RxReceivedBytes_s(7);
  
  ov8_RxFifoRst_p <= v8_RxReset_s;
  ov8_TxFifoRst_p <= v8_TxReset_s;
  
  -- input ports

  a8u_RecvData_s(0) <= unsigned(iv_RxDataCh0_p);
  a8u_RecvData_s(1) <= unsigned(iv_RxDataCh1_p);
  a8u_RecvData_s(2) <= unsigned(iv_RxDataCh2_p);
  a8u_RecvData_s(3) <= unsigned(iv_RxDataCh3_p);
  a8u_RecvData_s(4) <= unsigned(iv_RxDataCh4_p);
  a8u_RecvData_s(5) <= unsigned(iv_RxDataCh5_p);
  a8u_RecvData_s(6) <= unsigned(iv_RxDataCh6_p);
  a8u_RecvData_s(7) <= unsigned(iv_RxDataCh7_p);

  v8_TxReady_s(0) <= i_TxReadyCh0_p;
  v8_TxReady_s(1) <= i_TxReadyCh1_p;
  v8_TxReady_s(2) <= i_TxReadyCh2_p;
  v8_TxReady_s(3) <= i_TxReadyCh3_p;
  v8_TxReady_s(4) <= i_TxReadyCh4_p;
  v8_TxReady_s(5) <= i_TxReadyCh5_p;
  v8_TxReady_s(6) <= i_TxReadyCh6_p;
  v8_TxReady_s(7) <= i_TxReadyCh7_p;
  
  v8_RxDataValid_s(0) <= i_RxDataValidCh0_p;
  v8_RxDataValid_s(1) <= i_RxDataValidCh1_p;
  v8_RxDataValid_s(2) <= i_RxDataValidCh2_p;
  v8_RxDataValid_s(3) <= i_RxDataValidCh3_p;
  v8_RxDataValid_s(4) <= i_RxDataValidCh4_p;
  v8_RxDataValid_s(5) <= i_RxDataValidCh5_p;
  v8_RxDataValid_s(6) <= i_RxDataValidCh6_p;
  v8_RxDataValid_s(7) <= i_RxDataValidCh7_p;
  
  v8_RxReady_s(0) <= i_RxReadyCh0_p;
  v8_RxReady_s(1) <= i_RxReadyCh1_p;
  v8_RxReady_s(2) <= i_RxReadyCh2_p;
  v8_RxReady_s(3) <= i_RxReadyCh3_p;
  v8_RxReady_s(4) <= i_RxReadyCh4_p;
  v8_RxReady_s(5) <= i_RxReadyCh5_p;
  v8_RxReady_s(6) <= i_RxReadyCh6_p;
  v8_RxReady_s(7) <= i_RxReadyCh7_p;
  
  ov32_TxOverflowCh0_p <= a8v32_TxOverflow_s(0);
  ov32_TxOverflowCh1_p <= a8v32_TxOverflow_s(1);
  ov32_TxOverflowCh2_p <= a8v32_TxOverflow_s(2);
  ov32_TxOverflowCh3_p <= a8v32_TxOverflow_s(3);
  ov32_TxOverflowCh4_p <= a8v32_TxOverflow_s(4);
  ov32_TxOverflowCh5_p <= a8v32_TxOverflow_s(5);
  ov32_TxOverflowCh6_p <= a8v32_TxOverflow_s(6);
  ov32_TxOverflowCh7_p <= a8v32_TxOverflow_s(7);
  
  ov32_RxUnderflowCh0_p <= a8v32_RxUnderflow_s(0);
  ov32_RxUnderflowCh1_p <= a8v32_RxUnderflow_s(1);
  ov32_RxUnderflowCh2_p <= a8v32_RxUnderflow_s(2);
  ov32_RxUnderflowCh3_p <= a8v32_RxUnderflow_s(3);
  ov32_RxUnderflowCh4_p <= a8v32_RxUnderflow_s(4);
  ov32_RxUnderflowCh5_p <= a8v32_RxUnderflow_s(5);
  ov32_RxUnderflowCh6_p <= a8v32_RxUnderflow_s(6);
  ov32_RxUnderflowCh7_p <= a8v32_RxUnderflow_s(7);
    
  -------------------------------------------------------
  -- Sample Reset signals with the appropriate clock
  -- Helps timings too.
  -------------------------------------------------------
  
  process(i_TxUserClk_p)
  begin
    if rising_edge(i_TxUserClk_p) then
      v8_TxReset_s <= iv8_TxReset_p;
    end if;
  end process;
  
  process(i_RxUserClk_p)
  begin
    if rising_edge(i_RxUserClk_p) then
      v8_RxReset_s <= iv8_RxReset_p;
    end if;
  end process;


  -------------------------------------------------------
  -- Sample Start signals with the appropriate clock
  -- Helps timings too.
  -------------------------------------------------------

  process(i_TxUserClk_p)
  begin
    if rising_edge(i_TxUserClk_p) then
      v8_TxStart_s <= iv8_TxStart_p;
      v8_TxStartDly_s <= v8_TxStart_s;
    end if;
  end process;
  
  process(i_RxUserClk_p)
  begin
    if rising_edge(i_RxUserClk_p) then
      v8_RxStart_s <= iv8_RxStart_p;
      v8_RxStartDly_s <= v8_RxStart_s;
    end if;
  end process;


  -------------------------------------------------------
  -- Sample i_DisableFlowControl_p with the appropriate clock
  -- A TIG constraint can be added since this is a constant during
  -- normal operation
  -------------------------------------------------------

  -- Synchronize the 
  Process(i_RxUserClk_p)
  begin
    if rising_edge(i_RxUserClk_p) then
      disableFlowControl_s <= i_DisableFlowControl_p;
    end if;
  end process;
  

  -------------------------------------------------------
  -- One cycle delay of the data valid and rxData signals
  -- from RX FIFO to meet timing.
  ------------------------------------------------------- 
  
  process(i_RxUserClk_p)
  begin
    if rising_edge(i_RxUserClk_p) then
      v8_RxDataValidDly_s <= v8_RxDataValid_s;
      a8u_RecvDataDly_s <= a8u_RecvData_s;
    end if;
  end process;

  -------------------------------------------------------
  -- Additional configurable div counters for each TX channels
  -------------------------------------------------------
  
  ChDivCnt_gen : for i in 0 to (C_RTDEX_TX_NUMER_OF_CHANNELS - 1) generate
    ChDivCnt_Proc : process(i_TxUserClk_p)
    begin
      if rising_edge(i_TxUserClk_p) then
      
        v8_DisableFlowControlTX_r1_s(i) <= i_DisableFlowControl_p;
        v8_DisableFlowControlTX_r2_s(i) <= v8_DisableFlowControlTX_r1_s(i);
      
        v8_TxReadyR1_s <= v8_TxReady_s;

        if v8_TxReset_s(i) = '1' then
          a8v16_DivCounter_s(i) <= (others => '0');
          v8_TxWe_s(i) <= '0';
          v8_TxReadyStarted_s(i) <= '0';
          a8v32_TxOverflow_s(i) <= (others => '0');
          a8v32_TxOverflowTemp_s(i) <= (others => '0');
        else
          if (v8_TxReady_s(i) = '1') then
            v8_TxReadyStarted_s(i) <= '1';
          end if;
         -- if (v8_TxStartDly_s(i) = '1') then
         -- Wait until Rtdex core has risen its txready flag at least once
          if (v8_TxStartDly_s(i) = '1') and v8_TxReadyStarted_s(i) = '1' then
            if(a8v16_DivCounter_s(i) < a8v16_DivCntVal_s(i)) then
              a8v16_DivCounter_s(i) <= a8v16_DivCounter_s(i) + '1';
              v8_TxWe_s(i) <= '0';
            else
              a8v16_DivCounter_s(i) <= (others => '0');
              v8_TxWe_s(i) <= '1';          
            end if;  
          else
            v8_TxWe_s(i) <= '0';  
          end if;
          
          -- Count overflow events
          if v8_TxWe_s(i) = '1' and (v8_TxReady_s(i) = '0' and v8_DisableFlowControlTX_r2_s(i) = '1' and v8_TxReadyStarted_s(i) = '1') then
            a8v32_TxOverflowTemp_s(i) <= a8v32_TxOverflowTemp_s(i) + 1;
          end if;
          
          -- Latch overflow number only when TxReady goes down.
          -- This is to make sure this core doesn't count overflow events
          -- after the Rtdex core finalized the transmission for good
          -- (after which TxReady will never go up again)
          if v8_TxReady_s(i) = '0' and v8_TxReadyR1_s(i) = '1' then
            a8v32_TxOverflow_s(i) <= a8v32_TxOverflowTemp_s(i);
          end if;
        end if;
      end if;
    end process ChDivCnt_Proc;
  end generate ChDivCnt_gen;


  -------------------------------------------------------
  -- Rising edge detector for the start tx
  -------------------------------------------------------

--  txStartTestRedge_gen : for i in 0 to (C_RTDEX_TX_NUMER_OF_CHANNELS - 1) generate
    v8_txStartTestRedge_s <= not(v8_TxStartDly_s) and v8_TxStart_s;
--  end generate txStartTestRedge_gen;

  
  -------------------------------------------------------
  -- Rising edge detector for the start rx
  -------------------------------------------------------

--  rxStartTestRedge_gen : for i in 0 to (C_RTDEX_TX_NUMER_OF_CHANNELS - 1) generate
    v8_rxStartTestRedge_s <= not(v8_RxStartDly_s) and v8_RxStart_s;
--  end generate rxStartTestRedge_gen;

  
  -------------------------------------------------------
  -- Data gen for tx channel
  -- Ramp or RX FIFO if LoopBackEn is '1'
  --
  -------------------------------------------------------

  TxData_gen : for i in 0 to (C_RTDEX_TX_NUMER_OF_CHANNELS - 1) generate
    TxData_Proc : process(i_TxUserClk_p)
    begin
      if rising_edge(i_TxUserClk_p) then
        if v8_TxReset_s(i) = '1' then
          a8u_DataGen_s(i) <= ( others => '0' );
        else
          -- Load counters
          if (iv8_RxTxLoopBackEn_p(i) = '0') then
            if v8_txStartTestRedge_s(i) = '1' then
              a8u_DataGen_s(i) <= a8u_TxInitData_s(i);
            elsif v8_TxWe_s(i) = '1' and (v8_TxReady_s(i) = '1' or v8_DisableFlowControlTX_r2_s(i) = '1') then
              a8u_DataGen_s(i) <= a8u_DataGen_s(i) + 1;
            end if;
          else
            -- Data Loop back
            a8u_DataGen_s(i) <= a8u_RecvData_s(i);
          end if;
        end if;
      end if;
    end process TxData_Proc;
  end generate TxData_gen;
  
  
  --*************************************************************--  
  --******************** RX Ramp check side ********************--
  --*************************************************************--
  
  -----------------------------------------------------------
  -- Div counter to slow down Rx data consumption rate on CH0
  -----------------------------------------------------------

  RxRe_gen : for i in 0 to (C_RTDEX_RX_NUMER_OF_CHANNELS - 1) generate
  Process(i_RxUserClk_p)
  begin
    if rising_edge(i_RxUserClk_p) then
    
--      v8_DisableFlowControlRX_r1_s(i) <= i_DisableFlowControl_p;
--      v8_DisableFlowControlRX_r2_s(i) <= v8_DisableFlowControlRX_r1_s(i);
      
--      v8_DisableFlowControlRX_r3_s(i) <= v8_DisableFlowControlRX_r2_s(i) and v8_first_sample_received_s(i);
    
      v8_RxReadyR1_s <= v8_RxReady_s;
    
      if v8_RxReset_s(i) = '1' then
        a8v32_RxEnDivCnt_s(i) <= (others=>'0');
        v8_RxReDiv_s(i) <= '0';
        v8_RxReDiv_r1(i) <= '0';
        v8_RxReDiv_r2(i) <= '0';
        v8_DataValidExpected_s(i) <= '0';
        v8_DataValidExpectedDly_s(i) <= '0';
        a8v32_RxUnderflow_s(i) <= (others=>'0');
        a8v32_RxUnderflowTemp_s(i) <= (others=>'0');
--        v8_first_sample_received_s(i) <= '0';
        v8_RxReadyStarted_s(i) <= '0';
      else

        if v8_RxReady_s(i) = '1' then
          v8_RxReadyStarted_s(i) <= '1';
        end if;
        
--        if v8_RxDataValid_s(i) = '1' then
--          v8_first_sample_received_s(i) <= '1';
--        end if;
      
        if a8v32_RxEnDivCnt_s(i) < iv32_RxEnDivCnt_p then
          a8v32_RxEnDivCnt_s(i) <= a8v32_RxEnDivCnt_s(i) + '1';
          v8_RxReDiv_s(i) <= '0';
        else
          a8v32_RxEnDivCnt_s(i) <= (others=>'0');
          v8_RxReDiv_s(i) <= '1';
        end if;
        
        -- Start to read when the FIFO is ready, then keep the pace.
        -- This is to avoid initial errors.
        --
        v8_RxReDiv_r1(i) <= v8_RxReDiv_s(i) and v8_RxStartDly_s(i) and v8_RxReadyStarted_s(i);
        
        v8_RxReDiv_r2(i) <= v8_RxReDiv_r1(i);
        v8_DataValidExpected_s(i) <= v8_RxReDiv_r2(i);
        v8_DataValidExpectedDly_s(i) <= v8_DataValidExpected_s(i);
        
--        if v8_RxReDiv_r1(i) = '1' and (v8_RxReady_s(i) = '0' and v8_DisableFlowControlRX_r3_s(i) = '1') then
        if (v8_RxReDiv_r1(i) = '1' and v8_RxReady_s(i) = '0') then
          a8v32_RxUnderflowTemp_s(i) <= a8v32_RxUnderflowTemp_s(i) + 1;
        end if;
        
        if v8_RxReady_s(i) = '0' and v8_RxReadyR1_s(i) = '1' then
          a8v32_RxUnderflow_s(i) <= a8v32_RxUnderflowTemp_s(i);
        end if;
        
      end if;
    end if;
  end process;  
  end generate RxRe_gen;
  
  -------------------------------------------------------
  -- Rx Ramp validation and error counters
  -------------------------------------------------------
  
  RxDataCheck_gen : for i in 0 to (C_RTDEX_RX_NUMER_OF_CHANNELS - 1) generate
    RxDataCheck_proc : process(i_RxUserClk_p)
    begin
      if rising_edge(i_RxUserClk_p) then
        if v8_RxReset_s(i) = '1' then
          v8_RxErrorDet_s(i) <= '0';
          a8v32_RxReceivedBytes_s(i) <= ( others => '0' );
        else
          -- Load counters
          if v8_rxStartTestRedge_s(i) = '1' then
            a8u_ExpectedData_s(i) <= a8u_TxInitData_s(i);
          end if;
          
          if (v8_RxDataValidDly_s(i) = '1') or (v8_DataValidExpectedDly_s(i) = '1' and disableFlowControl_s = '1') then
            -- Sample integrity verification
            if( a8u_ExpectedData_s(i) /= a8u_RecvDataDly_s(i) ) then
              v8_RxErrorDet_s(i) <= '1';
              if (C_RECOVER_SYNC_AFTER_ERROR = 0)  then
                -- One missing sample will make the receive counter and the internal counter misaligned
                -- and can result to errors for the following received samples.
                --
                a8u_ExpectedData_s(i) <= a8u_ExpectedData_s(i) + 1;
              else
                -- After an error is detected, the expected value is set to the next value from the received value.
                -- So if many consecutive values are missing, only one error will be counted.
                --
                a8u_ExpectedData_s(i) <= a8u_RecvDataDly_s(i) + 1;
              end if;
            else
              v8_RxErrorDet_s(i) <= '0';
              a8u_ExpectedData_s(i) <= a8u_ExpectedData_s(i) + 1;
            end if;
            -- Sample counter
            if (v8_RxDataValidDly_s(i) = '1') then
              a8v32_RxReceivedBytes_s(i) <= a8v32_RxReceivedBytes_s(i) + 1;
            end if;
          else
            v8_RxErrorDet_s(i) <= '0';
          end if;
        end if;
      end if;
    end process RxDataCheck_proc;
  end generate RxDataCheck_gen;

  RxErrCnt_gen : for i in 0 to (C_RTDEX_RX_NUMER_OF_CHANNELS - 1) generate
    RxErrCnt_proc : process(i_RxUserClk_p)
    begin
      if rising_edge(i_RxUserClk_p) then
        if v8_RxReset_s(i) = '1' then
          a8v32_RxErrorCnt_s(i) <= ( others => '0' );
          a8v32_RxErrorCntTemp_s(i) <= ( others => '0' );
        else
          if ( v8_RxErrorDet_s(i) = '1' ) then
           -- a8v32_RxErrorCnt_s(i) <= a8v32_RxErrorCnt_s(i) + '1';
            a8v32_RxErrorCntTemp_s(i) <= a8v32_RxErrorCntTemp_s(i) + '1';
          end if;
          if v8_RxReady_s(i) = '0' and v8_RxReadyR1_s(i) = '1' then
            a8v32_RxErrorCnt_s(i) <= a8v32_RxErrorCntTemp_s(i);         
          end if;
        end if;
      end if;
    end process RxErrCnt_proc;
  end generate RxErrCnt_gen;

  
    
end rtl;