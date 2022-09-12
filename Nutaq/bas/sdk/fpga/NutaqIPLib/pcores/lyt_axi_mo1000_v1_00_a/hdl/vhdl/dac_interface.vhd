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
-- File : $Id: dac_interface.vhd,v 1.1 2015/04/10 19:48:35 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Serializer for MO1000 DAC (data, frame, dci).
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2014 Nutaq inc.
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.NUMERIC_STD.ALL;

library unisim;
use unisim.vcomponents.all;

library lyt_axi_mo1000_v1_00_a;
use lyt_axi_mo1000_v1_00_a.all;

entity dac_interface is
    generic (
        C_REF_CLK_FREQ_MHZ      : real  := 200.0
    );
    port(
        -- system  ports
        i_Rst_p                 : in  std_logic;
        i_AxiClk_p              : in  std_logic;
        i_RefClk_p              : in  std_logic;
        i_DesignClk_p           : in  std_logic;
        i_SerialClk_p           : in  std_logic;

        -- FMC interface ports
        odp_DataToFMC_p         : out std_logic_vector(31 downto 0);
        odn_DataToFMC_p         : out std_logic_vector(31 downto 0);
        odp_DciToFMC_p          : out std_logic;
        odn_DciToFMC_p          : out std_logic;
        odp_FrameToFMC_p        : out std_logic;
        odn_FrameToFMC_p        : out std_logic;

        -- User ports
        iv4_Frame_p             : in  std_logic_vector(3 downto 0);
        iv16_DacDataCh1_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh2_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh3_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh4_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh5_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh6_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh7_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh8_p       : in  std_logic_vector(15 downto 0);

        -- AXI register ports
        i_invert_dci_p          : in  std_logic;
        i_delay_rst_p           : in  std_logic;
        i_global_serdes_pd_p    : in  std_logic;
        i_delay_ctrl_rst_p      : in  std_logic;
        i_serdes_rst_p          : in  std_logic;
        o_delay_ctrl_rdy_p      : out std_logic;
        iv5_delay_value_p       : in  std_logic_vector(4 downto 0);
        i_delay_we_dci_p        : in  std_logic;
        i_delay_we_frame_p      : in  std_logic;
        iv32_delay_we_data_p    : in  std_logic_vector(31 downto 0)
    );
end dac_interface;

architecture arch of dac_interface is

    --------------------------------------------
    -- Type section
    --------------------------------------------
    type a32v5 is array (0 to 31) of std_logic_vector(4 downto 0);

    --------------------------------------------
    -- Signal section
    --------------------------------------------
    signal v32_data_A_s                     : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_data_B_s                     : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_data_C_s                     : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_data_D_s                     : std_logic_vector(31 downto 0) := (others => '0');

    -- Delay controller
    signal delay_ctrl_rdy_s                 : std_logic := '0';
    signal delay_ctrl_rdy_r1_s              : std_logic := '0';
    signal v_delay_ctrl_rst_delay_line_s    : std_logic_vector(9 DOWNTO 0) := "1111111111";
    signal p_delay_ctrl_rst_s               : std_logic := '1';

    -- DELAY signals
    signal a32v5_delay_value_data_s         : a32v5 := (others => (others => '0'));
    signal v5_delay_value_dci_s             : std_logic_vector(4 downto 0) := (others => '0');
    signal v5_delay_value_frame_s           : std_logic_vector(4 downto 0) := (others => '0');

    -- SERDES control
    signal v_serdes_rst_delay_line_s        : std_logic_vector(9 DOWNTO 0) := "1111111111";
    signal p_serdes_rst_s                   : std_logic := '1';
    signal global_serdes_pd_r1_s            : std_logic := '0';
    signal global_serdes_pd_r2_s            : std_logic := '0';
    signal v34_serdes_rst_r1_s              : std_logic_vector(33 downto 0) := (others => '1');
    signal v34_serdes_rst_s                 : std_logic_vector(33 downto 0) := (others => '1');
    
    -- Set a maxdelay attibute to avoid timing error on serdes reset signal
    -- it is a GPIO signal, no need to be precise 
    -- and the 2 register will make sure metastability is avoided
    signal serdes_rst_s_TIG : std_logic := '1';

    -- SERDES instantiation
    signal v32_serial_data_s                : std_logic_vector(31 downto 0) := (others => '0');
    signal v32_serial_data_dly_s            : std_logic_vector(31 downto 0) := (others => '0');
    signal serial_frame_s                   : std_logic := '0';
    signal serial_frame_dly_s               : std_logic := '0';
    signal serial_dci_s                     : std_logic := '0';
    signal serial_dci_dly_s                 : std_logic := '0';

    -- DCI signals
    signal invert_dci_r1_s                  : std_logic := '0';
    signal invert_dci_r2_s                  : std_logic := '0';
    signal v4_dci_s                         : std_logic_vector(3 downto 0)  := (others => '0');

    --------------------------------------------
    -- Attribute section
    --------------------------------------------
    attribute keep_hierarchy : string;
    attribute keep_hierarchy of arch : architecture is "false";

    attribute keep                          : string;
    attribute keep of delay_ctrl_rdy_r1_s   : signal is "true";
    attribute keep of serdes_rst_s_TIG      : signal is "true";
    attribute keep of v34_serdes_rst_r1_s   : signal is "true";
    attribute keep of v34_serdes_rst_s      : signal is "true";
   

begin

    v32_data_A_s(15 downto 0)   <= iv16_DacDataCh1_p;
    v32_data_B_s(15 downto 0)   <= iv16_DacDataCh2_p;
    v32_data_C_s(15 downto 0)   <= iv16_DacDataCh3_p;
    v32_data_D_s(15 downto 0)   <= iv16_DacDataCh4_p;
    v32_data_A_s(31 downto 16)  <= iv16_DacDataCh5_p;
    v32_data_B_s(31 downto 16)  <= iv16_DacDataCh6_p;
    v32_data_C_s(31 downto 16)  <= iv16_DacDataCh7_p;
    v32_data_D_s(31 downto 16)  <= iv16_DacDataCh8_p;

    --------------------------------------------
    -- Delay controller section
    --------------------------------------------
    -- clock domain crossing of delay controller reset pulse signal
    p2p_delay_ctrl_rst_i : entity lyt_axi_mo1000_v1_00_a.pulse2pulse
    port map (
        in_clk   => i_AxiClk_p,
        out_clk  => i_RefClk_p,
        rst      => i_Rst_p,
        pulsein  => i_delay_ctrl_rst_p,
        inbusy   => open,
        pulseout => v_delay_ctrl_rst_delay_line_s(0)
    );

    -- Stretch i_IdelayCtrlRst_p pulse
    process(i_Rst_p, i_RefClk_p)
    begin
        if i_Rst_p = '1' then
            v_delay_ctrl_rst_delay_line_s(v_delay_ctrl_rst_delay_line_s'high downto 1) <= (others => '1');
            p_delay_ctrl_rst_s <= '1';

        elsif(rising_edge(i_RefClk_p)) then
            v_delay_ctrl_rst_delay_line_s(v_delay_ctrl_rst_delay_line_s'high downto 1) <=
                v_delay_ctrl_rst_delay_line_s(v_delay_ctrl_rst_delay_line_s'high-1 downto 0);

            -- Hold i_reset_p pulse during 10 clock cycles (5 ns * 10 = 50 ns = Minimum i_reset_p pulse width)
            if v_delay_ctrl_rst_delay_line_s /= std_logic_vector(to_unsigned(0,v_delay_ctrl_rst_delay_line_s'length)) then
                p_delay_ctrl_rst_s <= '1';
            else
                p_delay_ctrl_rst_s <= '0';
            end if;
        end if;
    end process;

    -- delay controller
    delay_ctrl_i : IDELAYCTRL
    port map (
        RDY     => delay_ctrl_rdy_s,
        REFCLK  => i_RefClk_p,
        RST     => p_delay_ctrl_rst_s
    );

    -- clock domain crossing of delay controller ready signal
    process(i_AxiClk_p)
    begin
        if rising_edge(i_AxiClk_p) then
            delay_ctrl_rdy_r1_s <= delay_ctrl_rdy_s;
            o_delay_ctrl_rdy_p <= delay_ctrl_rdy_r1_s;
        end if;
    end process;

    ----------------------------------------------------------
    -- Write input delay value in the corresponding lane specified by
    -- iv32_delay_we_data_p, i_delay_we_dci_p or i_delay_we_frame_p mask
    ----------------------------------------------------------
    process(i_AxiClk_p)
    begin
        if rising_edge(i_AxiClk_p) then

            -- data
            for i in 0 to 31 loop
                if iv32_delay_we_data_p(i) = '1' then
                    a32v5_delay_value_data_s(i) <= iv5_delay_value_p;
                end if;
            end loop;

            -- dci
            if i_delay_we_dci_p = '1' then
                v5_delay_value_dci_s <= iv5_delay_value_p;
            end if;

            -- frame
            if i_delay_we_frame_p = '1' then
                v5_delay_value_frame_s <= iv5_delay_value_p;
            end if;

            -- Synchronous reset
            if i_delay_rst_p = '1' then
                for i in 0 to 31 loop
                    a32v5_delay_value_data_s(i) <= (others => '0');
                end loop;

                v5_delay_value_dci_s <= (others => '0');
                v5_delay_value_frame_s <= (others => '0');
            end if;
        end if;
    end process;

    ----------------------------------------------------------
    -- SERDES control
    ----------------------------------------------------------
    p2p_serdes_reset_i : entity lyt_axi_mo1000_v1_00_a.pulse2pulse
    port map (
        in_clk   => i_AxiClk_p,
        out_clk  => i_DesignClk_p,
        rst      => i_Rst_p,
        pulsein  => i_serdes_rst_p,
        inbusy   => open,
        pulseout => v_serdes_rst_delay_line_s(0)
    );

    -- Stretch i_IdelayCtrlRst_p pulse
    process(i_Rst_p, i_DesignClk_p)
    begin
        if i_Rst_p = '1' then
            v_serdes_rst_delay_line_s(v_serdes_rst_delay_line_s'high downto 1) <= (others => '1');
            p_serdes_rst_s <= '1';
            global_serdes_pd_r1_s <= '1';
            global_serdes_pd_r2_s <= '1';
            serdes_rst_s_TIG <= '1';

        elsif(rising_edge(i_DesignClk_p)) then
            v_serdes_rst_delay_line_s(v_serdes_rst_delay_line_s'high downto 1) <=
                v_serdes_rst_delay_line_s(v_serdes_rst_delay_line_s'high-1 downto 0);

            -- Hold reset pulse during 10 design clock cycles
            if v_serdes_rst_delay_line_s /= std_logic_vector(to_unsigned(0,v_serdes_rst_delay_line_s'length)) then
                p_serdes_rst_s <= '1';
            else
                p_serdes_rst_s <= '0';
            end if;

            global_serdes_pd_r1_s <= i_global_serdes_pd_p;
            global_serdes_pd_r2_s <= global_serdes_pd_r1_s;

            serdes_rst_s_TIG    <= p_serdes_rst_s or global_serdes_pd_r2_s;

        end if;
    end process;

    serdes_rst_gen: for i in 0 to 33 generate
    begin
        process(i_DesignClk_p)
        begin
            if(rising_edge(i_DesignClk_p)) then
                v34_serdes_rst_r1_s(i) <= serdes_rst_s_TIG;
                v34_serdes_rst_s(i) <= v34_serdes_rst_r1_s(i);
            end if;
        end process;
    end generate;

    ----------------------------------------------------------
    -- SERDES instantiation
    ----------------------------------------------------------
    serdes_data_gen: for i in 0 to 31 generate
    begin

        serdes_i : OSERDESE1
        generic map (
            DATA_RATE_OQ => "DDR",
            DATA_RATE_TQ => "SDR",
            DATA_WIDTH => 4, -- Parallel data width (1-8,10)
            DDR3_DATA => 1,
            INIT_OQ => '0', -- Initial value of OQ output (0/1)
            INIT_TQ => '0', -- Initial value of TQ output (0/1)
            INTERFACE_TYPE => "DEFAULT", -- Must leave at "DEFAULT" (MIG-only parameter)
            ODELAY_USED => 0, -- Must leave at 0 (MIG-only parameter)
            SERDES_MODE => "MASTER", -- "MASTER" or "SLAVE"
            SRVAL_OQ => '0', -- OQ output value when SR is used (0/1)
            SRVAL_TQ => '0', -- TQ output value when SR is used (0/1)
            TRISTATE_WIDTH => 1 -- Parallel to serial 3-state converter width (1 or 4)
        )
        port map (
            OFB => v32_serial_data_s(i), -- 1-bit serial data output to IODELAY

            CLK => i_SerialClk_p,
            CLKDIV => i_DesignClk_p,

            OCE => '1', -- 1-bit Active high clock data path enable input
            RST => v34_serdes_rst_s(i), -- 1-bit Active high reset input
            TCE => '0', -- 1-bit Active high clock enable input for 3-state

            -- D1 - D6: 1-bit (each) Parallel data inputs
            D1 => v32_data_A_s(i),
            D2 => v32_data_B_s(i),
            D3 => v32_data_C_s(i),
            D4 => v32_data_D_s(i),
            D5 => '0',
            D6 => '0',

            -- MIG-only Signals: set to GND
            CLKPERF => '0',
            CLKPERFDELAY => '0',
            ODV => '0',
            WC => '0',

            SHIFTIN1 => '0',
            SHIFTIN2 => '0',

            -- T1 - T4: 1-bit (each) Parallel 3-state inputs
            T1 => '0',
            T2 => '0',
            T3 => '0',
            T4 => '0'
        );

        iodelay_i : IODELAYE1
        generic map (
            CINVCTRL_SEL => FALSE,
            DELAY_SRC => "O",
            HIGH_PERFORMANCE_MODE => TRUE,
            IDELAY_TYPE => "VAR_LOADABLE",
            IDELAY_VALUE => 0,
            ODELAY_TYPE => "VAR_LOADABLE",
            ODELAY_VALUE => 0,
            REFCLK_FREQUENCY => C_REF_CLK_FREQ_MHZ,
            SIGNAL_PATTERN => "DATA"
        )
        port map (
            CNTVALUEOUT => open,
            DATAOUT => v32_serial_data_dly_s(i),
            C => i_AxiClk_p,
            CE => '1',
            INC => '1',
            CINVCTRL => '0',
            CLKIN => '0',
            CNTVALUEIN => a32v5_delay_value_data_s(i),
            DATAIN => '0',
            IDATAIN => '0',
            ODATAIN => v32_serial_data_s(i),
            RST => '1',
            T => '0'
        );

        obufds_i : OBUFDS
        port map (
            O   => odp_DataToFMC_p(i),
            OB  => odn_DataToFMC_p(i),
            I   => v32_serial_data_dly_s(i)
        );

    end generate;

    ----------------------------------------------------------
    -- Frame logic
    ----------------------------------------------------------
    serdes_frame_i : OSERDESE1
    generic map (
        DATA_RATE_OQ => "DDR",
        DATA_RATE_TQ => "SDR",
        DATA_WIDTH => 4, -- Parallel data width (1-8,10)
        DDR3_DATA => 1,
        INIT_OQ => '0', -- Initial value of OQ output (0/1)
        INIT_TQ => '0', -- Initial value of TQ output (0/1)
        INTERFACE_TYPE => "DEFAULT", -- Must leave at "DEFAULT" (MIG-only parameter)
        ODELAY_USED => 0, -- Must leave at 0 (MIG-only parameter)
        SERDES_MODE => "MASTER", -- "MASTER" or "SLAVE"
        SRVAL_OQ => '0', -- OQ output value when SR is used (0/1)
        SRVAL_TQ => '0', -- TQ output value when SR is used (0/1)
        TRISTATE_WIDTH => 1 -- Parallel to serial 3-state converter width (1 or 4)
    )
    port map (
        OFB => serial_frame_s, -- 1-bit serial data output to IODELAY

        CLK => i_SerialClk_p,
        CLKDIV => i_DesignClk_p,

        OCE => '1', -- 1-bit Active high clock data path enable input
        RST => v34_serdes_rst_s(32), -- 1-bit Active high reset input
        TCE => '0', -- 1-bit Active high clock enable input for 3-state

        -- D1 - D6: 1-bit (each) Parallel data inputs
        D1 => iv4_Frame_p(0),
        D2 => iv4_Frame_p(1),
        D3 => iv4_Frame_p(2),
        D4 => iv4_Frame_p(3),
        D5 => '0',
        D6 => '0',

        -- MIG-only Signals: set to GND
        CLKPERF => '0',
        CLKPERFDELAY => '0',
        ODV => '0',
        WC => '0',

        SHIFTIN1 => '0',
        SHIFTIN2 => '0',

        -- T1 - T4: 1-bit (each) Parallel 3-state inputs
        T1 => '0',
        T2 => '0',
        T3 => '0',
        T4 => '0'
    );

    iodelay_frame_i : IODELAYE1
    generic map (
        CINVCTRL_SEL => FALSE,
        DELAY_SRC => "O",
        HIGH_PERFORMANCE_MODE => TRUE,
        IDELAY_TYPE => "VAR_LOADABLE",
        IDELAY_VALUE => 0,
        ODELAY_TYPE => "VAR_LOADABLE",
        ODELAY_VALUE => 0,
        REFCLK_FREQUENCY => C_REF_CLK_FREQ_MHZ,
        SIGNAL_PATTERN => "DATA"
    )
    port map (
        CNTVALUEOUT => open,
        DATAOUT => serial_frame_dly_s,
        C => i_AxiClk_p,
        CE => '1',
        INC => '1',
        CINVCTRL => '0',
        CLKIN => '0',
        CNTVALUEIN => v5_delay_value_frame_s,
        DATAIN => '0',
        IDATAIN => '0',
        ODATAIN => serial_frame_s,
        RST => '1',
        T => '0'
    );

    obufds_frame_i : OBUFDS
    port map (
        O   => odp_FrameToFMC_p,
        OB  => odn_FrameToFMC_p,
        I   => serial_frame_dly_s
    );

    ----------------------------------------------------------
    -- DCI logic
    ----------------------------------------------------------
    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then
            invert_dci_r1_s <= i_invert_dci_p;
            invert_dci_r2_s <= invert_dci_r1_s;

            if (invert_dci_r2_s = '0') then
                v4_dci_s <= "0101";
            else
                v4_dci_s <= "1010";
            end if;
        end if;
    end process;

    serdes_dci_i : OSERDESE1
    generic map (
        DATA_RATE_OQ => "DDR",
        DATA_RATE_TQ => "SDR",
        DATA_WIDTH => 4, -- Parallel data width (1-8,10)
        DDR3_DATA => 1,
        INIT_OQ => '0', -- Initial value of OQ output (0/1)
        INIT_TQ => '0', -- Initial value of TQ output (0/1)
        INTERFACE_TYPE => "DEFAULT", -- Must leave at "DEFAULT" (MIG-only parameter)
        ODELAY_USED => 0, -- Must leave at 0 (MIG-only parameter)
        SERDES_MODE => "MASTER", -- "MASTER" or "SLAVE"
        SRVAL_OQ => '0', -- OQ output value when SR is used (0/1)
        SRVAL_TQ => '0', -- TQ output value when SR is used (0/1)
        TRISTATE_WIDTH => 1 -- Parallel to serial 3-state converter width (1 or 4)
    )
    port map (
        OFB => serial_dci_s, -- 1-bit serial data output to IODELAY

        CLK => i_SerialClk_p,
        CLKDIV => i_DesignClk_p,

        OCE => '1', -- 1-bit Active high clock data path enable input
        RST => v34_serdes_rst_s(33), -- 1-bit Active high reset input
        TCE => '0', -- 1-bit Active high clock enable input for 3-state

        -- D1 - D6: 1-bit (each) Parallel data inputs
        D1 => v4_dci_s(0),
        D2 => v4_dci_s(1),
        D3 => v4_dci_s(2),
        D4 => v4_dci_s(3),
        D5 => '0',
        D6 => '0',

        -- MIG-only Signals: set to GND
        CLKPERF => '0',
        CLKPERFDELAY => '0',
        ODV => '0',
        WC => '0',

        SHIFTIN1 => '0',
        SHIFTIN2 => '0',

        -- T1 - T4: 1-bit (each) Parallel 3-state inputs
        T1 => '0',
        T2 => '0',
        T3 => '0',
        T4 => '0'
    );

    iodelay_dci_i : IODELAYE1
    generic map (
        CINVCTRL_SEL => FALSE,
        DELAY_SRC => "O",
        HIGH_PERFORMANCE_MODE => TRUE,
        IDELAY_TYPE => "VAR_LOADABLE",
        IDELAY_VALUE => 0,
        ODELAY_TYPE => "VAR_LOADABLE",
        ODELAY_VALUE => 0,
        REFCLK_FREQUENCY => C_REF_CLK_FREQ_MHZ,
        SIGNAL_PATTERN => "DATA"
    )
    port map (
        CNTVALUEOUT => open,
        DATAOUT => serial_dci_dly_s,
        C => i_AxiClk_p,
        CE => '1',
        INC => '1',
        CINVCTRL => '0',
        CLKIN => '0',
        CNTVALUEIN => v5_delay_value_dci_s,
        DATAIN => '0',
        IDATAIN => '0',
        ODATAIN => serial_dci_s,
        RST => '1',
        T => '0'
    );

    obufds_dci_i : OBUFDS
    port map (
        O   => odp_DciToFMC_p,
        OB  => odn_DciToFMC_p,
        I   => serial_dci_dly_s
    );

end architecture;
