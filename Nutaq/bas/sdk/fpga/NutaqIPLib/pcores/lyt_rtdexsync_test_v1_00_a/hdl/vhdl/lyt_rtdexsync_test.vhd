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
-- File : lyt_rtdexsync_test.vhd
--------------------------------------------------------------------------------
-- Description : RTDExSync test core
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_rtdexsync_test.vhd,v $
-- Revision 1.1  2015/04/10 19:49:02  sqa
-- bas first commit
--
-- Revision 1.2  2015/01/30 18:28:20  julien.roy
-- Set the time back to 0 on reset
--
-- Revision 1.1  2014/11/19 14:48:27  julien.roy
-- Add rtdexsync test core
--
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_misc.all;

library lyt_rtdexsync_test_v1_00_a;
use lyt_rtdexsync_test_v1_00_a.all;
use lyt_rtdexsync_test_v1_00_a.lyt_type_p.all;

entity lyt_rtdexsync_test is
    generic
    (
        C_RTDEX_RX_NUMER_OF_CHANNELS : integer := 8;
        C_RTDEX_TX_NUMER_OF_CHANNELS : integer := 8;
        C_RTDEX_DATA_WIDTH           : integer := 32
    );
    port
    (
        -- User ports
        i_Clk_p                 : in std_logic;
        i_Rst_p                 : in std_logic;
            
        iv64_Time_p             : in std_logic_vector(63 downto 0);

        -- Indirect addressing interface
        --
        i_IndWrEn_p             : in  std_logic;                        -- Indirect addressing write enable
        iv8_IndAddr_p           : in std_logic_vector(7 downto 0);      -- Indirect addressing address
        iv32_IndWrReg_p         : in std_logic_vector(31 downto 0);     -- Indirect addressing write register
        ov32_IndRdReg_p         : out std_logic_vector(31 downto 0);    -- Indirect addressing read register
        
        i_RxReadyCh0_p          : in std_logic;
        o_RxReCh0_p             : out std_logic;
        iv_RxDataCh0_p          : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        i_RxDataValidCh0_p      : in std_logic;
        
        i_RxReadyCh1_p          : in std_logic;
        o_RxReCh1_p             : out std_logic;
        iv_RxDataCh1_p          : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        i_RxDataValidCh1_p      : in std_logic;
        
        i_RxReadyCh2_p          : in std_logic;
        o_RxReCh2_p             : out std_logic;
        iv_RxDataCh2_p          : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        i_RxDataValidCh2_p      : in std_logic;
        
        i_RxReadyCh3_p          : in std_logic;
        o_RxReCh3_p             : out std_logic;
        iv_RxDataCh3_p          : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        i_RxDataValidCh3_p      : in std_logic;
        
        i_RxReadyCh4_p          : in std_logic;
        o_RxReCh4_p             : out std_logic;
        iv_RxDataCh4_p          : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        i_RxDataValidCh4_p      : in std_logic;
        
        i_RxReadyCh5_p          : in std_logic;
        o_RxReCh5_p             : out std_logic;
        iv_RxDataCh5_p          : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        i_RxDataValidCh5_p      : in std_logic;
        
        i_RxReadyCh6_p          : in std_logic;
        o_RxReCh6_p             : out std_logic;
        iv_RxDataCh6_p          : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        i_RxDataValidCh6_p      : in std_logic;
        
        i_RxReadyCh7_p          : in std_logic;
        o_RxReCh7_p             : out std_logic;
        iv_RxDataCh7_p          : in std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        i_RxDataValidCh7_p      : in std_logic;
        
        i_TxReadyCh0_p          : in std_logic;
        o_TxWeCh0_p             : out std_logic;
        ov_TxDataCh0_p          : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
        i_TxReadyCh1_p          : in std_logic;
        o_TxWeCh1_p             : out std_logic;
        ov_TxDataCh1_p          : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
        i_TxReadyCh2_p          : in std_logic;
        o_TxWeCh2_p             : out std_logic;
        ov_TxDataCh2_p          : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
        i_TxReadyCh3_p          : in std_logic;
        o_TxWeCh3_p             : out std_logic;
        ov_TxDataCh3_p          : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
        i_TxReadyCh4_p          : in std_logic;
        o_TxWeCh4_p             : out std_logic;
        ov_TxDataCh4_p          : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
        i_TxReadyCh5_p          : in std_logic;
        o_TxWeCh5_p             : out std_logic;
        ov_TxDataCh5_p          : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
        i_TxReadyCh6_p          : in std_logic;
        o_TxWeCh6_p             : out std_logic;
        ov_TxDataCh6_p          : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0);
        
        i_TxReadyCh7_p          : in std_logic;
        o_TxWeCh7_p             : out std_logic;
        ov_TxDataCh7_p          : out std_logic_vector(C_RTDEX_DATA_WIDTH-1 downto 0)
    );
end entity lyt_rtdexsync_test;

architecture rtl of lyt_rtdexsync_test is

    type array8_u_t is array(7 downto 0) of unsigned(C_RTDEX_DATA_WIDTH-1 downto 0);

    -- Regiters
    --

    signal v8_TxReset_s                 : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxStart_s                 : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_TxStart_s                 : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxReset_s                 : std_logic_vector(7 downto 0)  := (others => '0');

    signal v8_RxStartR1_s               : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxStartR2_s               : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_TxStartR1_s               : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_TxStartR2_s               : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_TxStartRedge_s            : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxStartRedge_s            : std_logic_vector(7 downto 0)  := (others => '0');

    signal v8_TxWe_s                    : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxRe_s                    : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_TxWeReq_s                 : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxReReq_s                 : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_TxWeFsm_s                 : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxReFsm_s                 : std_logic_vector(7 downto 0)  := (others => '0');

    signal a8u_DataGen_s                : array8_u_t                    := (others => (others => '0'));
    signal a8u_ExpectedData_s           : array8_u_t                    := (others => (others => '0'));

    signal a8u_RecvData_s               : array8_u_t                    := (others => (others => '0'));
    signal a8u_RecvDataR1_s             : array8_u_t                    := (others => (others => '0'));
    signal v8_RxDataValid_s             : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxDataValidR1_s           : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxDataValidR2_s           : std_logic_vector(7 downto 0)  := (others => '0');
    signal a8v32_RxErrorCnt_s           : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_RxErrorCntTmp_s        : array8_v32_t                  := (others => (others => '0'));

    signal a8u_TxInitData_s             : array8_u_t                    := (others => (others => '0'));
    signal a8v16_TxDivCntVal_s          : array8_v16_t                  := (others => (others => '0'));
    signal a8v16_TxDivCounter_s         : array8_v16_t                  := (others => (others => '0'));
    signal a8u_RxInitData_s             : array8_u_t                    := (others => (others => '0'));
    signal a8v16_RxDivCntVal_s          : array8_v16_t                  := (others => (others => '0'));
    signal a8v16_RxDivCounter_s         : array8_v16_t                  := (others => (others => '0'));

    signal v8_TxReady_s                 : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_TxReadyR1_s               : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_TxStarted_s               : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxReady_s                 : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxReadyR1_s               : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxStarted_s               : std_logic_vector(7 downto 0)  := (others => '0');

    signal v8_RxReR1_s                  : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_RxReR2_s                  : std_logic_vector(7 downto 0)  := (others => '0');
    signal v8_ExpectedRxDataValidR1_s   : std_logic_vector(7 downto 0)  := (others => '0');

    signal a8v32_TxOverflow_s           : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_TxOverflowTmp_s        : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_RxUnderrun_s           : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_RxUnderrunTmp_s        : array8_v32_t                  := (others => (others => '0'));

    signal a8v32_TxSampNum_s            : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_RxSampNum_s            : array8_v32_t                  := (others => (others => '0'));

    signal a8v64_TxStartTime_s          : array8_v64_t                  := (others => (others => '0'));
    signal a8v64_TxStopTime_s           : array8_v64_t                  := (others => (others => '0'));
    signal a8v64_RxStartTime_s          : array8_v64_t                  := (others => (others => '0'));
    signal a8v64_RxStopTime_s           : array8_v64_t                  := (others => (others => '0'));


    -- Indirect registers
    --
    signal a8v32_Reg0_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg1_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg2_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg3_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg4_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg5_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg6_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg7_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg8_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg9_s                 : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg10_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg11_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg12_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg13_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg14_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg15_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg16_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg17_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg18_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg19_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg20_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg21_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg22_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg23_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg24_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg25_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg26_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg27_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg28_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg29_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg30_s                : array8_v32_t                  := (others => (others => '0'));
    signal a8v32_Reg31_s                : array8_v32_t                  := (others => (others => '0'));


begin


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

    lyt_ind_ch_reg_i : entity lyt_rtdexsync_test_v1_00_a.lyt_ind_ch_reg
    port map(

        -- User ports
        i_Clk_p              => i_Clk_p,
        i_Rst_p              => i_Rst_p,

        -- Indirect addressing interface
        --
        i_IndWrEn_p          => i_IndWrEn_p,
        iv3_IndChanIdx_s     => iv8_IndAddr_p(7 downto 5),
        iv5_IndRegIdx_s      => iv8_IndAddr_p(4 downto 0),
        iv32_IndWrReg_p      => iv32_IndWrReg_p,
        ov32_IndRdReg_p      => ov32_IndRdReg_p,

        -- Output registers
        oa8v32_Reg0_p        => a8v32_Reg0_s,
        oa8v32_Reg1_p        => a8v32_Reg1_s,
        oa8v32_Reg2_p        => a8v32_Reg2_s,
        oa8v32_Reg3_p        => a8v32_Reg3_s,
        oa8v32_Reg4_p        => a8v32_Reg4_s,

        ia8v32_Reg0_p        => a8v32_Reg0_s,
        ia8v32_Reg1_p        => a8v32_Reg1_s,
        ia8v32_Reg2_p        => a8v32_Reg2_s,
        ia8v32_Reg3_p        => a8v32_Reg3_s,
        ia8v32_Reg4_p        => a8v32_Reg4_s,

        -- Input only registers
        oa8v32_Reg5_p        => open,
        oa8v32_Reg6_p        => open,
        oa8v32_Reg7_p        => open,
        oa8v32_Reg8_p        => open,
        oa8v32_Reg9_p        => open,
        oa8v32_Reg10_p       => open,
        oa8v32_Reg11_p       => open,
        oa8v32_Reg12_p       => open,
        oa8v32_Reg13_p       => open,
        oa8v32_Reg14_p       => open,
        oa8v32_Reg15_p       => open,
        oa8v32_Reg16_p       => open,
        oa8v32_Reg17_p       => open,
        oa8v32_Reg18_p       => open,
        oa8v32_Reg19_p       => open,
        oa8v32_Reg20_p       => open,
        oa8v32_Reg21_p       => open,
        oa8v32_Reg22_p       => open,
        oa8v32_Reg23_p       => open,
        oa8v32_Reg24_p       => open,
        oa8v32_Reg25_p       => open,
        oa8v32_Reg26_p       => open,
        oa8v32_Reg27_p       => open,
        oa8v32_Reg28_p       => open,
        oa8v32_Reg29_p       => open,
        oa8v32_Reg30_p       => open,
        oa8v32_Reg31_p       => open,
        
        ia8v32_Reg5_p        => a8v32_Reg5_s,
        ia8v32_Reg6_p        => a8v32_Reg6_s,
        ia8v32_Reg7_p        => a8v32_Reg7_s,
        ia8v32_Reg8_p        => a8v32_Reg8_s,
        ia8v32_Reg9_p        => a8v32_Reg9_s,
        ia8v32_Reg10_p       => a8v32_Reg10_s,
        ia8v32_Reg11_p       => a8v32_Reg11_s,
        ia8v32_Reg12_p       => a8v32_Reg12_s,
        ia8v32_Reg13_p       => a8v32_Reg13_s,
        ia8v32_Reg14_p       => a8v32_Reg14_s,
        ia8v32_Reg15_p       => a8v32_Reg15_s,
        ia8v32_Reg16_p       => a8v32_Reg16_s,
        ia8v32_Reg17_p       => a8v32_Reg17_s,
        ia8v32_Reg18_p       => a8v32_Reg18_s,
        ia8v32_Reg19_p       => a8v32_Reg19_s,
        ia8v32_Reg20_p       => a8v32_Reg20_s,
        ia8v32_Reg21_p       => a8v32_Reg21_s,
        ia8v32_Reg22_p       => a8v32_Reg22_s,
        ia8v32_Reg23_p       => a8v32_Reg23_s,
        ia8v32_Reg24_p       => a8v32_Reg24_s,
        ia8v32_Reg25_p       => a8v32_Reg25_s,
        ia8v32_Reg26_p       => a8v32_Reg26_s,
        ia8v32_Reg27_p       => a8v32_Reg27_s,
        ia8v32_Reg28_p       => a8v32_Reg28_s,
        ia8v32_Reg29_p       => a8v32_Reg29_s,
        ia8v32_Reg30_p       => a8v32_Reg30_s,
        ia8v32_Reg31_p       => a8v32_Reg31_s

    );


    -- Register assignment
    --
    gen_ch_reg: for i in 0 to 7 generate

        v8_TxReset_s(i)           <= a8v32_Reg0_s(i)(0);
        v8_RxReset_s(i)           <= a8v32_Reg0_s(i)(1);
        v8_TxStart_s(i)           <= a8v32_Reg0_s(i)(2);
        v8_RxStart_s(i)           <= a8v32_Reg0_s(i)(3);

        a8v16_TxDivCntVal_s(i)    <= a8v32_Reg1_s(i)(a8v16_TxDivCntVal_s(i)'range);
        a8v16_RxDivCntVal_s(i)    <= a8v32_Reg2_s(i)(a8v16_RxDivCntVal_s(i)'range);
        a8u_TxInitData_s(i)       <= unsigned(a8v32_Reg3_s(i)(a8u_TxInitData_s(i)'range));
        a8u_RxInitData_s(i)       <= unsigned(a8v32_Reg4_s(i)(a8u_RxInitData_s(i)'range));

        a8v32_Reg5_s(i)           <= std_logic_vector(resize(unsigned(a8v32_RxErrorCnt_s(i)),32));
        a8v32_Reg6_s(i)           <= std_logic_vector(resize(unsigned(a8v32_TxOverflow_s(i)),32));
        a8v32_Reg7_s(i)           <= std_logic_vector(resize(unsigned(a8v32_RxUnderrun_s(i)),32));

        a8v32_Reg8_s(i)           <= a8v64_TxStartTime_s(i)(63 downto 32);
        a8v32_Reg9_s(i)           <= a8v64_TxStartTime_s(i)(31 downto 0);
        a8v32_Reg10_s(i)          <= a8v64_TxStopTime_s(i)(63 downto 32);
        a8v32_Reg11_s(i)          <= a8v64_TxStopTime_s(i)(31 downto 0);

        a8v32_Reg12_s(i)          <= a8v64_RxStartTime_s(i)(63 downto 32);
        a8v32_Reg13_s(i)          <= a8v64_RxStartTime_s(i)(31 downto 0);
        a8v32_Reg14_s(i)          <= a8v64_RxStopTime_s(i)(63 downto 32);
        a8v32_Reg15_s(i)          <= a8v64_RxStopTime_s(i)(31 downto 0);

        a8v32_Reg16_s(i)          <= a8v32_TxSampNum_s(i);
        a8v32_Reg17_s(i)          <= a8v32_RxSampNum_s(i);

    end generate ;


    -- Output ports
    --

    -- Write TX data when the write process requested it and
    -- at the first time th ready signal is high
    --      (v8_TxStartR2_s(0) = '1' and v8_TxStarted_s(0) = '0' and v8_TxReady_s(0))
    --
    v8_TxWeReq_s    <= (v8_TxWeFsm_s or (v8_TxStartR2_s and (not v8_TxStarted_s) and v8_TxReady_s));
    v8_TxWe_s       <= v8_TxWeReq_s and v8_TxReady_s;

    o_TxWeCh0_p     <= v8_TxWe_s(0);
    o_TxWeCh1_p     <= v8_TxWe_s(1);
    o_TxWeCh2_p     <= v8_TxWe_s(2);
    o_TxWeCh3_p     <= v8_TxWe_s(3);
    o_TxWeCh4_p     <= v8_TxWe_s(4);
    o_TxWeCh5_p     <= v8_TxWe_s(5);
    o_TxWeCh6_p     <= v8_TxWe_s(6);
    o_TxWeCh7_p     <= v8_TxWe_s(7);


    v8_RxReReq_s <= (v8_RxReFsm_s or (v8_RxStartR2_s and (not v8_RxStarted_s) and v8_RxReady_s));
    v8_RxRe_s       <= v8_RxReReq_s and v8_RxReady_s;

    o_RxReCh0_p     <= v8_RxRe_s(0);
    o_RxReCh1_p     <= v8_RxRe_s(1);
    o_RxReCh2_p     <= v8_RxRe_s(2);
    o_RxReCh3_p     <= v8_RxRe_s(3);
    o_RxReCh4_p     <= v8_RxRe_s(4);
    o_RxReCh5_p     <= v8_RxRe_s(5);
    o_RxReCh6_p     <= v8_RxRe_s(6);
    o_RxReCh7_p     <= v8_RxRe_s(7);

    ov_TxDataCh0_p <= std_logic_vector(a8u_DataGen_s(0));
    ov_TxDataCh1_p <= std_logic_vector(a8u_DataGen_s(1));
    ov_TxDataCh2_p <= std_logic_vector(a8u_DataGen_s(2));
    ov_TxDataCh3_p <= std_logic_vector(a8u_DataGen_s(3));
    ov_TxDataCh4_p <= std_logic_vector(a8u_DataGen_s(4));
    ov_TxDataCh5_p <= std_logic_vector(a8u_DataGen_s(5));
    ov_TxDataCh6_p <= std_logic_vector(a8u_DataGen_s(6));
    ov_TxDataCh7_p <= std_logic_vector(a8u_DataGen_s(7));

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


    -------------------------------------------------------
    -- Delay Start signals
    -- Helps timings too.
    -------------------------------------------------------

    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            v8_TxStartR1_s <= v8_TxStart_s;
            v8_TxStartR2_s <= v8_TxStartR1_s;

            v8_RxStartR1_s <= v8_RxStart_s;
            v8_RxStartR2_s <= v8_RxStartR1_s;

        end if;
    end process;

    v8_TxStartRedge_s <= not(v8_TxStartR2_s) and v8_TxStartR1_s;
    v8_RxStartRedge_s <= not(v8_RxStartR2_s) and v8_RxStartR1_s;


    -------------------------------------------------------
    -- Generate TX data and monitor status
    -------------------------------------------------------
    ChDivCnt_gen : for i in 0 to (C_RTDEX_TX_NUMER_OF_CHANNELS - 1) generate
    ChDivCnt_Proc : process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then

            v8_TxReadyR1_s <= v8_TxReady_s;

            -- Default values
            --
            v8_TxWeFsm_s(i) <= '0';

            -- Detect start
            --
            if (v8_TxReady_s(i) = '1' and v8_TxStartR2_s(i) = '1' and v8_TxStarted_s(i) = '0') then
                v8_TxStarted_s(i)       <= '1';
                a8v64_TxStartTime_s(i)  <= iv64_Time_p;
            end if;

            -- Logic once started
            --
            if (v8_TxReady_s(i) = '1' and v8_TxStartR2_s(i) = '1' and v8_TxStarted_s(i) = '0') or (v8_TxStarted_s(i) = '1') then

                -- Transmit every DivCntVal clock cycles
                --
                if(a8v16_TxDivCounter_s(i) < a8v16_TxDivCntVal_s(i)) then
                    a8v16_TxDivCounter_s(i)   <= a8v16_TxDivCounter_s(i) + '1';

                else
                    a8v16_TxDivCounter_s(i)   <= (others => '0');
                    v8_TxWeFsm_s(i)           <= '1';

                end if;

                -- Count the current overflow number
                -- Set this value to the "real" status only at a potential stop
                --
                if v8_TxWeFsm_s(i) = '1' and v8_TxReady_s(i) = '0' then
                    a8v32_TxOverflowTmp_s(i) <= a8v32_TxOverflowTmp_s(i) + 1;
                end if;

                -- Latch the potential stop time on the ready falling edge
                -- The value will be written again if there is a new falling edge
                --
                if v8_TxReady_s(i) = '0' and v8_TxReadyR1_s(i) = '1' then
                    a8v64_TxStopTime_s(i) <= iv64_Time_p;
                    a8v32_TxOverflow_s(i) <= a8v32_TxOverflowTmp_s(i);
                end if;

            end if;

            -- Synchronous reset on core reset or start rising edge
            --
            if v8_TxReset_s(i) = '1' or v8_TxStartRedge_s(i) = '1' then

                a8v16_TxDivCounter_s(i)       <= (others => '0');
                v8_TxWeFsm_s(i)                <= '0';
                v8_TxStarted_s(i)           <= '0';
                a8v32_TxOverflowTmp_s(i)    <= (others => '0');
                a8v32_TxOverflow_s(i)       <= (others => '0');
                a8v64_TxStartTime_s(i) <= (others=>'0');
                a8v64_TxStopTime_s(i) <= (others=>'0');
            end if;

        end if;
    end process ChDivCnt_Proc;
    end generate ChDivCnt_gen;


    -------------------------------------------------------
    -- Data gen for tx channel
    -- Ramp or RX FIFO if LoopBackEn is '1'
    --
    -------------------------------------------------------
    TxData_gen : for i in 0 to (C_RTDEX_TX_NUMER_OF_CHANNELS - 1) generate
    TxData_Proc : process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            
            -- Increment the generated data on write enable request
            if v8_TxWeReq_s(i) = '1'then
                a8u_DataGen_s(i) <= a8u_DataGen_s(i) + 1;
            end if;
            
            -- Count the number of sample sent
            if v8_TxWe_s(i) = '1' then
                a8v32_TxSampNum_s(i) <= a8v32_TxSampNum_s(i) + 1;
            end if;
            
            -- Synchronous reset on core reset or start rising edge
            --
            if v8_TxReset_s(i) = '1' or v8_TxStartRedge_s(i) = '1' then
                a8v32_TxSampNum_s(i) <= (others => '0');
                a8u_DataGen_s(i) <= a8u_TxInitData_s(i);
            end if;

        end if;
    end process TxData_Proc;
    end generate TxData_gen;


    -------------------------------------------------------
    -- One cycle delay of the data valid and rxData signals
    -- from RX FIFO to meet timing.
    -------------------------------------------------------

    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            v8_RxDataValidR1_s <= v8_RxDataValid_s;
            v8_RxDataValidR2_s <= v8_RxDataValidR1_s;
            a8u_RecvDataR1_s <= a8u_RecvData_s;

            -- 2 clock cycles delay to match RTDEx latency
            v8_RxReR1_s  <= v8_RxReReq_s;
            v8_RxReR2_s  <= v8_RxReR1_s;

            v8_ExpectedRxDataValidR1_s <= v8_RxReR2_s;

        end if;
    end process;

    -------------------------------------------------------
    -- Receive RX data and monitor status
    -------------------------------------------------------

    RxRe_gen : for i in 0 to (C_RTDEX_RX_NUMER_OF_CHANNELS - 1) generate
    Process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then

            v8_RxReadyR1_s <= v8_RxReady_s;

            -- Default values
            --
            v8_RxReFsm_s(i) <= '0';

            -- Detect start
            --
            if (v8_RxReady_s(i) = '1' and v8_RxStartR2_s(i) = '1' and v8_RxStarted_s(i) = '0') then
                v8_RxStarted_s(i)       <= '1';
                
                 -- +2 to compensate for the FIFO latency
                a8v64_RxStartTime_s(i)  <= iv64_Time_p + 2;
            end if;

            -- Logic once started
            --
            if (v8_RxReady_s(i) = '1' and v8_RxStartR2_s(i) = '1' and v8_RxStarted_s(i) = '0') or (v8_RxStarted_s(i) = '1') then

                -- Transmit every DivCntVal clock cycles
                --
                if(a8v16_RxDivCounter_s(i) < a8v16_RxDivCntVal_s(i)) then
                    a8v16_RxDivCounter_s(i)   <= a8v16_RxDivCounter_s(i) + '1';

                else
                    a8v16_RxDivCounter_s(i) <= (others => '0');
                    v8_RxReFsm_s(i)         <= '1';

                end if;

                -- Count the current overflow number
                -- Set this value to the "real" status only at a potential stop
                --
                if v8_RxReFsm_s(i) = '1' and v8_RxReady_s(i) = '0' then
                    a8v32_RxUnderrunTmp_s(i) <= a8v32_RxUnderrunTmp_s(i) + 1;
                end if;

                -- Latch the potential stop time on the ready falling edge
                -- The value will be written again if there is a new falling edge
                --
                if v8_RxReady_s(i) = '0' and v8_RxReadyR1_s(i) = '1' then
                    a8v32_RxUnderrun_s(i) <= a8v32_RxUnderrunTmp_s(i);
                
                    -- +2 to compensate for the FIFO latency
                    a8v64_RxStopTime_s(i) <= iv64_Time_p + 2;
                end if;

            end if;

            -- Synchronous reset on core reset or start rising edge
            --
            if v8_RxReset_s(i) = '1' or v8_RxStartRedge_s(i) = '1' then

                v8_RxStarted_s(i) <= '0';
                a8v16_RxDivCounter_s(i) <= (others=>'0');
                v8_RxReFsm_s(i) <= '0';
                a8v32_RxUnderrun_s(i) <= (others=>'0');
                a8v32_RxUnderrunTmp_s(i) <= (others=>'0');
                a8v64_RxStartTime_s(i) <= (others=>'0');
                a8v64_RxStopTime_s(i) <= (others=>'0');
            end if;

        end if;
    end process;
    end generate RxRe_gen;

    -------------------------------------------------------
    -- RX Ramp validation and error counters
    -------------------------------------------------------
    RxDataCheck_gen : for i in 0 to (C_RTDEX_RX_NUMER_OF_CHANNELS - 1) generate
    RxDataCheck_proc : process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then

            if v8_RxDataValidR1_s(i) = '1' then
                -- Count the number of sample received
                a8v32_RxSampNum_s(i) <= a8v32_RxSampNum_s(i) + 1;
            end if;
            
            
            if v8_RxDataValidR2_s(i) = '1' then
                -- Set the error counter value only when receiving valid data
                -- in order to avoid counting errors after the transmission end
                --
                a8v32_RxErrorCnt_s(i)   <= a8v32_RxErrorCntTmp_s(i);
            end if;

            -- When expecting a new sample (even if the FIFO was not ready)
            if v8_ExpectedRxDataValidR1_s(i) = '1' then

                -- Detect error
                --
                if (a8u_ExpectedData_s(i) /= a8u_RecvDataR1_s(i)) or v8_RxDataValidR1_s(i) = '0' then
                    a8v32_RxErrorCntTmp_s(i) <= a8v32_RxErrorCntTmp_s(i) + 1;
                end if;

                -- The next expected data should be + 1 than the currect expected data
                --
                a8u_ExpectedData_s(i) <= a8u_ExpectedData_s(i) + 1;

            end if;

            -- Synchronous reset on core reset or start rising edge
            --
            if v8_RxReset_s(i) = '1' or v8_RxStartRedge_s(i) = '1' then
                a8v32_RxErrorCnt_s(i)       <= ( others => '0' );
                a8v32_RxErrorCntTmp_s(i)    <= ( others => '0' );
                a8v32_RxSampNum_s(i)        <= ( others => '0' );
                a8u_ExpectedData_s(i)       <= a8u_RxInitData_s(i);

            end if;

        end if;
    end process RxDataCheck_proc;
    end generate RxDataCheck_gen;

end rtl;