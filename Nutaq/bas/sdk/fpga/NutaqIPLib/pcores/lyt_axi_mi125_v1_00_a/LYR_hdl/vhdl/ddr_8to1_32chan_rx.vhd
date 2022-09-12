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
-- File : $Id: ddr_8to1_32chan_rx.vhd,v 1.2 2015/06/03 18:14:07 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Desirilizer and alignement wrapper for MI125.
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
-- 

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.NUMERIC_STD.ALL;

library unisim;
use unisim.vcomponents.all;


entity DDR_8TO1_32CHAN_RX is
    generic (
        NBRCHANNELS             : integer := 32;                                    -- width of the data for the system
        C_SYSCLK_FREQ_MHZ       : integer := 100
    );
    port (
        i_RefClk200MHz_p        : in std_logic;                                     -- 200 MHZ REFERENCE CLOCK TO IDELAYCTRL
        i_SystemClock_p         : in std_logic;                                     -- SYSTEM Bus Clock    (AXI Lite or PLB)
        i_reset_p               : in std_logic;                                     -- LOGIC reset
        
        idp_DataFromADC_p       : in std_logic_vector(NBRCHANNELS-1 DOWNTO 0);      -- SERIAL SIDE RX DATA (P)
        idn_DataFromADC_p       : in std_logic_vector(NBRCHANNELS-1 DOWNTO 0);      -- SERIAL SIDE RX DATA (N)
        idp_FrameFromADC_p      : in std_logic;                                     -- FORWARDED FRAMING signal FROM TX (P)
        idn_FrameFromADC_p      : in std_logic;                                     -- FORWARDED FRAMING signal FROM TX (N)
        
        i_RxClk_p               : in std_logic;                                     -- FORWARDED CLOCK FROM OTHER MI125 INSTANCE
        i_RxClkDiv_p            : in std_logic;                                     -- PARALLEL SIDE RX CLOCK FROM OTHER MI125 INSTANCE
       
        o_idelay_ready_p        : out std_logic;                                    -- FLAG INDICATING THAT IDELAYCTRL IS LOCKED
        ov_Data_p               : out std_logic_vector(8*NBRCHANNELS-1 DOWNTO 0);   -- PARALLEL SIDE RX DATA

        i_FreqCntRst_p          : in std_logic;                                     -- reset the frequency counter for debug purpose
        iv6_FreqCntClkSel_p     : in std_logic_vector(5 downto 0);                  -- Select the signal to be pushed to the frequency counter for debug purpose
        ov16_FreqCntClkCnt_p    : out std_logic_vector(15 downto 0);                -- Output the frequency of the signal pushed to the frequency counter for debug purpose

        i_IdelayCtrlRst_p       : in std_logic;                                     -- IDELAYCTRL CIRCUIT reset
        i_adcIdelay_rst_p       : in std_logic;
        iv5_adcIdelay_value_p   : in std_logic_vector(4 downto 0);
        iv32_adcIdelay_we_p     : in std_logic_vector(31 downto 0);

        i_serdesRst_p           : in std_logic;
        iv32_bitslip_p          : in std_logic_vector(31 downto 0);
        
        i_reset_calib_detection_p   : in std_logic;
        ov_calib_error_p            : out std_logic_vector(31 downto 0);
        ov_calib_pattern_error_p    : out std_logic_vector(31 downto 0);
        o_calib_detection_done_p    : out std_logic
    );
end DDR_8TO1_32CHAN_RX;

architecture translated of DDR_8TO1_32CHAN_RX is

    -------------------------------------------------------------------------------
    --         ************** Component declaration *******************
    -------------------------------------------------------------------------------

    -------------------------------------------------------------------------------
    --         ************** Signals declaration *******************
    -------------------------------------------------------------------------------

    signal v_DataRxBuf_s                    : std_logic_vector(NBRCHANNELS-1 DOWNTO 0);
    signal v_DataRxBufDS_s                  : std_logic_vector(NBRCHANNELS-1 DOWNTO 0);
    signal v_DataRxDelay_s                  : std_logic_vector(NBRCHANNELS-1 DOWNTO 0);

    signal v_shift1_s                       : std_logic_vector(NBRCHANNELS-1    DOWNTO 0);
    signal v_shift2_s                       : std_logic_vector(NBRCHANNELS-1    DOWNTO 0);

    signal v_IdelayCtrlRst_delay_line_s     : std_logic_vector(9 DOWNTO 0) := "1111111111";
    signal p_IdelayCtrlRst_s                : std_logic := '1';
    
    type v32_array_t is array (0 to NBRCHANNELS-1) of std_logic_vector(31 downto 0);
    signal av32_serdes_data_s                : v32_array_t := (( others => (others => '0')));

    type v8_array_t is array (0 to NBRCHANNELS-1) of std_logic_vector(7 downto 0);
    signal av8_serdes_data_inv_s            : v8_array_t := (( others => (others => '0')));

    type u4_array_t is array (0 to NBRCHANNELS-1) of unsigned(3 downto 0);
    signal au4_BitSlipCounter_s             : u4_array_t := (( others => (others => '0')));
    
    signal p_serdesRst_s                    : std_logic;
    signal v32p_bitslip_s                   : std_logic_vector(32 downto 0);

    -- IDELAY signals
    type idelay_array is array (0 to 31) of std_logic_vector(4 downto 0);
    signal av5_adcIdelayValue_s             : idelay_array := (others => (others => '0'));
    signal idelay_ready_s                   : std_logic;
    
    -- Frame
    signal FrameFromADC_s                   : std_logic;

    -- Frequency counters
    signal v_testClocks_s                   : std_logic_vector(2 downto 0);
    signal freqcnt_rst_s                    : std_logic;

    -------------------------------------------------------------------------------
    --         ************** Attribute declaration *******************
    -------------------------------------------------------------------------------

    attribute BUFFER_TYPE : string;
    attribute BUFFER_TYPE of v_testClocks_s : signal is "NONE";

    attribute S : string;
    attribute S of v_DataRxDelay_s          : signal is "TRUE";
    attribute S of v_DataRxBuf_s            : signal is "TRUE";
    attribute S of FrameFromADC_s           : signal is "TRUE";

BEGIN

    ----------------------------------------------------------
    -- Write input idelay value in the corresponding
    -- array index specify by iv_adcIdelayValue_we_p mask
    ----------------------------------------------------------
    idelay_generate: for i in 0 to 31 generate
        process(i_SystemClock_p)
        begin
            if rising_edge(i_SystemClock_p) then
                if iv32_adcIdelay_we_p(i) = '1' then
                    av5_adcIdelayValue_s(i) <= iv5_adcIdelay_value_p;
                end if;

                -- Synchronous i_reset_p
                if i_adcIdelay_rst_p = '1' then
                    av5_adcIdelayValue_s(i) <= (others => '0');
                end if;
            end if;
        end process;
    end generate;

    ----------------------------------------------------------
    -- IDELAYCTRL MODULE
    ----------------------------------------------------------
    pulse2pulse_io_reset : entity work.pulse2pulse
        port map (
            in_clk   => i_SystemClock_p,
            out_clk  => i_RefClk200MHz_p,
            rst      => i_reset_p,
            pulsein  => i_IdelayCtrlRst_p,
            inbusy   => open,
            pulseout => v_IdelayCtrlRst_delay_line_s(0)
        );

    -- Stretch i_IdelayCtrlRst_p pulse
    process(i_RefClk200MHz_p)
    begin
        if(rising_edge(i_RefClk200MHz_p)) then
            for i in 0 to v_IdelayCtrlRst_delay_line_s'high-1 loop
                v_IdelayCtrlRst_delay_line_s(i + 1) <= v_IdelayCtrlRst_delay_line_s(i);
            end loop;

            -- Hold i_reset_p pulse during 10 clock cycles (5 ns * 10 = 50 ns = Minimum i_reset_p pulse width)
            if v_IdelayCtrlRst_delay_line_s /=
                std_logic_vector(to_unsigned(0,v_IdelayCtrlRst_delay_line_s'length)) then
                p_IdelayCtrlRst_s <= '1';
            else
                p_IdelayCtrlRst_s <= '0';
            end if;
        end if;
    end process;

    RX_IDELAYCTRL : IDELAYCTRL
        port map (
            RDY     => idelay_ready_s,
            REFCLK  => i_RefClk200MHz_p,
            RST     => p_IdelayCtrlRst_s
        );

    synch_idelay : entity work.synchronizer_2t
        port map(
            Clk             => i_SystemClock_p,
            reset           => i_reset_p,
            Reset_value     => '0',
            Async           => idelay_ready_s,
            Sync            => o_idelay_ready_p
        );

    ----------------------------------------------------------
    -- ISERDES control
    ----------------------------------------------------------
    pulse2pulse_serdes_reset : entity work.pulse2pulse
        port map (
            in_clk   => i_SystemClock_p,
            out_clk  => i_RxClkDiv_p,
            rst      => i_reset_p,
            pulsein  => i_serdesRst_p,
            inbusy   => open,
            pulseout => p_serdesRst_s
        );

    bitslip_generate: for i in 0 to 31 generate
        pulse2pulse_bitslip : entity work.pulse2pulse
        port map (
            in_clk   => i_SystemClock_p,
            out_clk  => i_RxClkDiv_p,
            rst      => i_reset_p,
            pulsein  => iv32_bitslip_p(i),
            inbusy   => open,
            pulseout => v32p_bitslip_s(i)
        );
    end generate;

    -- Reorder output data
    Channels : for j in 0 to NBRCHANNELS/2-1        generate
        ov_Data_p((8*NBRCHANNELS-1-j*16) downto (8*NBRCHANNELS-1-((j+1)*16-1))) <=
              av8_serdes_data_inv_s(2*j)(7)& av8_serdes_data_inv_s(2*j+1)(7)
            & av8_serdes_data_inv_s(2*j)(6)& av8_serdes_data_inv_s(2*j+1)(6)
            & av8_serdes_data_inv_s(2*j)(5)& av8_serdes_data_inv_s(2*j+1)(5)
            & av8_serdes_data_inv_s(2*j)(4)& av8_serdes_data_inv_s(2*j+1)(4)
            & av8_serdes_data_inv_s(2*j)(3)& av8_serdes_data_inv_s(2*j+1)(3)
            & av8_serdes_data_inv_s(2*j)(2)& av8_serdes_data_inv_s(2*j+1)(2)
            & av8_serdes_data_inv_s(2*j)(1)& av8_serdes_data_inv_s(2*j+1)(1)
            & av8_serdes_data_inv_s(2*j)(0)& av8_serdes_data_inv_s(2*j+1)(0);
    end generate Channels;

    pins: for pin_count in 0 to NBRCHANNELS-1 generate
    begin
        -- data input differential buffers
        RX_DATA_IN : IBUFDS
        generic map(
            DIFF_TERM => TRUE, IOSTANDARD =>"LVDS_25")
        port map (
            O => v_DataRxBufDS_s(pin_count),
            I => idp_DataFromADC_p(pin_count),
            IB => idn_DataFromADC_p(pin_count)
        );

        v_DataRxBuf_s(pin_count)<= v_DataRxBufDS_s(pin_count);

        -- data idelay
        IODELAY_RX_DATA : IODELAYE1
        generic map (
            IDELAY_TYPE  => "VAR_LOADABLE",
            IDELAY_VALUE => 0,
            DELAY_SRC    => "I",
            HIGH_PERFORMANCE_MODE => TRUE
        )
        port map (
            dataout => v_DataRxDelay_s(pin_count),
            idatain => v_DataRxBuf_s(pin_count),
            c => i_RxClkDiv_p,
            ce => '1',
            inc => '1',
            datain => '0',
            odatain => '0',
            clkin => '0',
            rst => '1',
            cntvaluein => av5_adcIdelayValue_s(pin_count)(4 downto 0),
            cinvctrl => '0',
            t => '1'
        );

        iserdese_master : iserdese1
        generic map (
            DATA_RATE          => "DDR",
            DATA_WIDTH         => 8,
            INTERFACE_TYPE     => "NETWORKING",
            DYN_CLKDIV_INV_EN  => FALSE,
            DYN_CLK_INV_EN     => FALSE,
            NUM_CE             => 2,
            OFB_USED           => FALSE,
            IOBDELAY           => "BOTH",
            SERDES_MODE        => "MASTER"
        )
        port map (
            Q1                 => av32_serdes_data_s(pin_count)(0),
            Q2                 => av32_serdes_data_s(pin_count)(1),
            Q3                 => av32_serdes_data_s(pin_count)(2),
            Q4                 => av32_serdes_data_s(pin_count)(3),
            Q5                 => av32_serdes_data_s(pin_count)(4),
            Q6                 => av32_serdes_data_s(pin_count)(5),
            SHIFTOUT1          => v_shift1_s(pin_count),
            SHIFTOUT2          => v_shift2_s(pin_count),
            BITSLIP            => '0', --v32p_bitslip_s(pin_count),
            CE1                => '1',
            CE2                => '1',
            CLK                => i_RxClk_p,
            CLKB               => NOT i_RxClk_p,
            CLKDIV             => i_RxClkDiv_p,
            D                  => '0',
            DDLY               => v_DataRxDelay_s(pin_count),
            RST                => p_serdesRst_s,
            SHIFTIN1           => '0',
            SHIFTIN2           => '0',
            DYNCLKDIVSEL       => '0',
            DYNCLKSEL          => '0',
            OFB                => '0',
            OCLK               => '0',
            O                  => open
        );

        iserdese_slave : iserdese1
        generic map (
            DATA_RATE          => "DDR",
            DATA_WIDTH         => 8,
            INTERFACE_TYPE     => "NETWORKING",
            DYN_CLKDIV_INV_EN  => FALSE,
            DYN_CLK_INV_EN     => FALSE,
            NUM_CE             => 2,

            OFB_USED           => FALSE,
            IOBDELAY           => "BOTH",
            SERDES_MODE        => "SLAVE"
        )
        port map (
            Q1                 => open,
            Q2                 => open,
            Q3                 => av32_serdes_data_s(pin_count)(6),
            Q4                 => av32_serdes_data_s(pin_count)(7),
            Q5                 => open,
            Q6                 => open,
            SHIFTOUT1          => open,
            SHIFTOUT2          => open,
            SHIFTIN1           => v_shift1_s(pin_count),
            SHIFTIN2           => v_shift2_s(pin_count),
            BITSLIP            => '0', --v32p_bitslip_s(pin_count),
            CE1                => '1',
            CE2                => '1',
            CLK                => i_RxClk_p,
            CLKB               => NOT i_RxClk_p,
            CLKDIV             => i_RxClkDiv_p,
            D                  => '0',
            DDLY               => '0',
            RST                => p_serdesRst_s,
            DYNCLKDIVSEL       => '0',
            DYNCLKSEL          => '0',
            OFB                => '0',
            OCLK               => '0',
            O                  => open
        );
        
        -- Do the bit slip process manually since it seems to corrupt the SERDES when the two input clock are not in phase
        bitslip_proc : process(i_RxClkDiv_p)
        begin
            if rising_edge(i_RxClkDiv_p) then
            
                -- Bit slip operation and
                -- Inversion, since the _p et _n LVDS pairs are swapped at the FPGA pins
                av8_serdes_data_inv_s(pin_count)(0) <= NOT av32_serdes_data_s(pin_count)(to_integer( resize(au4_BitSlipCounter_s(pin_count),5) + 0 ));
                av8_serdes_data_inv_s(pin_count)(1) <= NOT av32_serdes_data_s(pin_count)(to_integer( resize(au4_BitSlipCounter_s(pin_count),5) + 1 ));
                av8_serdes_data_inv_s(pin_count)(2) <= NOT av32_serdes_data_s(pin_count)(to_integer( resize(au4_BitSlipCounter_s(pin_count),5) + 2 ));
                av8_serdes_data_inv_s(pin_count)(3) <= NOT av32_serdes_data_s(pin_count)(to_integer( resize(au4_BitSlipCounter_s(pin_count),5) + 3 ));
                av8_serdes_data_inv_s(pin_count)(4) <= NOT av32_serdes_data_s(pin_count)(to_integer( resize(au4_BitSlipCounter_s(pin_count),5) + 4 ));
                av8_serdes_data_inv_s(pin_count)(5) <= NOT av32_serdes_data_s(pin_count)(to_integer( resize(au4_BitSlipCounter_s(pin_count),5) + 5 ));
                av8_serdes_data_inv_s(pin_count)(6) <= NOT av32_serdes_data_s(pin_count)(to_integer( resize(au4_BitSlipCounter_s(pin_count),5) + 6 ));
                av8_serdes_data_inv_s(pin_count)(7) <= NOT av32_serdes_data_s(pin_count)(to_integer( resize(au4_BitSlipCounter_s(pin_count),5) + 7 ));
                
                -- The previous sample is located in the MSB of av32_serdes_data_s array
                av32_serdes_data_s(pin_count)(15 downto 8)  <= av32_serdes_data_s(pin_count)(7 downto 0);
                av32_serdes_data_s(pin_count)(23 downto 16) <= av32_serdes_data_s(pin_count)(15 downto 8);
                av32_serdes_data_s(pin_count)(31 downto 24) <= av32_serdes_data_s(pin_count)(23 downto 16);
                
                -- Increment bit slip counter
                if (v32p_bitslip_s(pin_count) = '1') then
                    au4_BitSlipCounter_s(pin_count) <= au4_BitSlipCounter_s(pin_count) + 1;
                end if;
                
                -- Reset the bit slip counter
                if (p_serdesRst_s = '1') then
                    au4_BitSlipCounter_s(pin_count) <= (others => '0');
                end if;
            
            end if;
        end process;
        
        gen_pin_count_0 : if pin_count = 0 generate
            calibration_detection_INST: entity work.calibration_detection
            generic map (
                DATA_LENGTH => av8_serdes_data_inv_s(pin_count)'length)
            port map (
                i_clk_p            => i_RxClkDiv_p,
                i_reset_p          => i_reset_calib_detection_p,
                iv_pattern_p       => X"2C",
                iv_data_p          => av8_serdes_data_inv_s(pin_count),
                o_error_p          => ov_calib_error_p(pin_count),
                o_pattern_error_p  => ov_calib_pattern_error_p(pin_count),
                o_detection_done_p => o_calib_detection_done_p);
        end generate;
        
        gen_pin_count_else : if pin_count /= 0 generate
            calibration_detection_INST: entity work.calibration_detection
            generic map (
                DATA_LENGTH => av8_serdes_data_inv_s(pin_count)'length)
            port map (
                i_clk_p            => i_RxClkDiv_p,
                i_reset_p          => i_reset_calib_detection_p,
                iv_pattern_p       => X"2C",
                iv_data_p          => av8_serdes_data_inv_s(pin_count),
                o_error_p          => ov_calib_error_p(pin_count),
                o_pattern_error_p  => ov_calib_pattern_error_p(pin_count),
                o_detection_done_p => open);
        end generate;

    end generate;

    
    ----------------------------------------------------------------------------------------------------
    -- Frame
    ----------------------------------------------------------------------------------------------------
    -- data input differential buffers
    FRAME_IN : IBUFDS
    generic map(
        DIFF_TERM => TRUE, IOSTANDARD =>"LVDS_25")
    port map (
        O => FrameFromADC_s,
        I => idp_FrameFromADC_p,
        IB => idn_FrameFromADC_p
    );
    
    ----------------------------------------------------------------------------------------------------
    -- Frequency counter for Profuction debug purpose
    ----------------------------------------------------------------------------------------------------
    freqcnt_rst_s <= i_reset_p or i_FreqCntRst_p;
    freq_cnt_top_inst : entity work.freq_cnt_top
        generic map(
            C_REFCLK_FREQ_MHZ   => C_SYSCLK_FREQ_MHZ,
            C_NUM_OF_TEST_CLK   => 3
        )
        port map(
            i_Rst_p             => freqcnt_rst_s,
            i_RefClk_p          => i_SystemClock_p,                                 -- AXI 100MHz
            iv_TestClks_p       => v_testClocks_s(2 downto 0),                       -- Freqs to be measured
            iv6_TestClkSel_p    => ("0000" & iv6_FreqCntClkSel_p(1 downto 0)),      -- select the clock to measure
            ov16_Freq_p         => ov16_FreqCntClkCnt_p                             -- measured freq value in MHz
        );

    v_testClocks_s(0) <= i_RefClk200MHz_p;   -- 200MHz ref clk
    v_testClocks_s(1) <= '0';
    v_testClocks_s(2) <= i_RxClkDiv_p;       -- Parallel ADC clock

END translated;
