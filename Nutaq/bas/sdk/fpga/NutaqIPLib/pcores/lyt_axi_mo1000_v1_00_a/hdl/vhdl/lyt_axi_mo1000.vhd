--------------------------------------------------------------------------------
-- Filename:          lyt_axi_mo1000.vhd
-- Version:           v1_00_a
-- Description:       User Logic implementation module
-- Generated by:      julien.roy
-- Date:              2014-04-28 15:28:04
-- Generated:         using LyrtechRD REGGENUTIL based on Xilinx IPIF Wizard.
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Copyright (c) 2001-2012 LYRtech RD Inc.  All rights reserved.
------------------------------------------------------------------------------

library unisim;
use unisim.vcomponents.all;

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

library lyt_axi_mo1000_v1_00_a;
use lyt_axi_mo1000_v1_00_a.all;

entity lyt_axi_mo1000 is
generic
(
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    C_BUILD_REVISION            : std_logic_vector      := X"0200";
    C_CLOCK_MASTER              : boolean               := true;
    C_FMC_POSITION              : integer               := 0;
    C_AXI_CLK_FREQ_MHZ          : integer               := 100;
    C_REF_CLK_FREQ_MHZ          : real                  := 200.0;
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_S_AXI_DATA_WIDTH             : integer              := 32;
    C_S_AXI_ADDR_WIDTH             : integer              := 32;
    C_S_AXI_MIN_SIZE               : std_logic_vector     := X"000001FF";
    C_USE_WSTRB                    : integer              := 0;
    C_DPHASE_TIMEOUT               : integer              := 8;
    C_BASEADDR                     : std_logic_vector     := X"FFFFFFFF";
    C_HIGHADDR                     : std_logic_vector     := X"00000000";
    C_FAMILY                       : string               := "virtex6"
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
);
port
(
    -- System ports
    i_Rst_p                     : in  std_logic;
    i_RefClk_p                  : in  std_logic;

    -- User ports
    o_DesignClk_p               : out std_logic;
    iv16_DacDataCh1_p           : in  std_logic_vector(15 downto 0);
    iv16_DacDataCh2_p           : in  std_logic_vector(15 downto 0);
    iv16_DacDataCh3_p           : in  std_logic_vector(15 downto 0);
    iv16_DacDataCh4_p           : in  std_logic_vector(15 downto 0);
    iv16_DacDataCh5_p           : in  std_logic_vector(15 downto 0);
    iv16_DacDataCh6_p           : in  std_logic_vector(15 downto 0);
    iv16_DacDataCh7_p           : in  std_logic_vector(15 downto 0);
    iv16_DacDataCh8_p           : in  std_logic_vector(15 downto 0);
    o_DacRdyCh1_p               : out std_logic;
    o_DacRdyCh2_p               : out std_logic;
    o_DacRdyCh3_p               : out std_logic;
    o_DacRdyCh4_p               : out std_logic;
    o_DacRdyCh5_p               : out std_logic;
    o_DacRdyCh6_p               : out std_logic;
    o_DacRdyCh7_p               : out std_logic;
    o_DacRdyCh8_p               : out std_logic;
    o_Trigger_p                 : out std_logic;

    -- FMC interface ports
    idp_ClockFromFMC_p          : in  std_logic;
    idn_ClockFromFMC_p          : in  std_logic;
    odp_DataToFMC_p             : out std_logic_vector(31 downto 0);
    odn_DataToFMC_p             : out std_logic_vector(31 downto 0);
    odp_DciToFMC_p              : out std_logic;
    odn_DciToFMC_p              : out std_logic;
    odp_FrameToFMC_p            : out std_logic;
    odn_FrameToFMC_p            : out std_logic;
    i_TriggerFromFMC_p          : in  std_logic;

    -- MO1000 interconnection ports
    i_DesignClk_p               : in  std_logic;
    o_SerialClk_p               : out std_logic;
    i_SerialClk_p               : in  std_logic;

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol ports, do not add to or delete
    S_AXI_ACLK                     : in  std_logic;
    S_AXI_ARESETN                  : in  std_logic;
    S_AXI_AWADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
    S_AXI_AWVALID                  : in  std_logic;
    S_AXI_WDATA                    : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    S_AXI_WSTRB                    : in  std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
    S_AXI_WVALID                   : in  std_logic;
    S_AXI_BREADY                   : in  std_logic;
    S_AXI_ARADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
    S_AXI_ARVALID                  : in  std_logic;
    S_AXI_RREADY                   : in  std_logic;
    S_AXI_ARREADY                  : out std_logic;
    S_AXI_RDATA                    : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
    S_AXI_RRESP                    : out std_logic_vector(1 downto 0);
    S_AXI_RVALID                   : out std_logic;
    S_AXI_WREADY                   : out std_logic;
    S_AXI_BRESP                    : out std_logic_vector(1 downto 0);
    S_AXI_BVALID                   : out std_logic;
    S_AXI_AWREADY                  : out std_logic
    -- DO NOT EDIT ABOVE THIS LINE --
);

 attribute MAX_FANOUT                     : string;
 attribute SIGIS                          : string;
 attribute MAX_FANOUT of S_AXI_ACLK       : signal is "10000";
 attribute MAX_FANOUT of S_AXI_ARESETN    : signal is "10000";
 attribute SIGIS of S_AXI_ACLK            : signal is "Clk";
 attribute SIGIS of S_AXI_ARESETN         : signal is "Rst";

end entity lyt_axi_mo1000;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------
architecture arch of lyt_axi_mo1000 is

    ----------------------------------------
    -- Signals declaration
    ----------------------------------------
    signal core_rst_s             : std_logic;
    signal delay_rst_s            : std_logic;
    signal v8_dac_ready_s         : std_logic_vector(7 downto 0);
    signal data_pattern_en_s      : std_logic;
    signal global_serdes_pd_s     : std_logic;
    signal delay_ctrl_rst_s       : std_logic;
    signal v8_data_serdes_pd_s    : std_logic_vector(7 downto 0);
    signal v2_frame_mode_s        : std_logic_vector(1 downto 0);
    signal frame_mode_write_s     : std_logic;
    signal v2_frame_pattern_s     : std_logic_vector(1 downto 0);
    signal invert_dci_s                     : std_logic;
    signal serdes_rst_s           : std_logic;
    signal mmcm_rst_s             : std_logic;
    signal update_dac_ready_s     : std_logic;
    signal clock_master_s         : std_logic;
    signal delay_ctrl_rdy_s       : std_logic;
    signal mmcm_locked_s          : std_logic;
    signal v4_FmcPosition_s       : std_logic_vector(3 downto 0);
    signal v5_delay_value_s       : std_logic_vector(4 downto 0);
    signal delay_we_dci_s         : std_logic;
    signal delay_we_frame_s       : std_logic;
    signal v32_delay_we_data_s    : std_logic_vector(31 downto 0);
    signal v16_pattern_a_s        : std_logic_vector(15 downto 0);
    signal v16_pattern_b_s        : std_logic_vector(15 downto 0);
    signal v16_pattern_c_s        : std_logic_vector(15 downto 0);
    signal v16_pattern_d_s        : std_logic_vector(15 downto 0);
    signal v16_freq_cnt_s         : std_logic_vector(15 downto 0);
    signal freq_cnt_rst_s         : std_logic;
    signal freq_cnt_rdy_s         : std_logic;
    signal freq_cnt_sel_s         : std_logic;
    signal v5_trigger_delay_s     : std_logic_vector(4 downto 0);

    signal pUpdate_dac_ready_s    : std_logic;

    -- Reset signals
    signal v8_CoreResetStretch_s  : std_logic_vector(7 downto 0) := (others => '1');
    signal CoreReset_s            : std_logic;
    signal v8_MmcmRstStretch_s    : std_logic_vector(7 downto 0) := (others => '1');
    signal MmcmRst_s              : std_logic;
    
    -- Internal clocks
    signal MmcmSerialClk_s        : std_logic;
    signal MmcmDesignClk_s        : std_logic;
    signal SerialClk_s            : std_logic;
    signal DesignClk_s            : std_logic;

begin

    -------------------------------------------------------------------------------
    --  Instantiate MMCM module to generate ADC serial and data clocks
    -------------------------------------------------------------------------------
    U_MMCM_V6_Inst : entity lyt_axi_mo1000_v1_00_a.MMCMExtended_V6
    port map (
        -- Clock in ports from external diff pins
        CLK_IN1_P         => idp_ClockFromFMC_p,
        CLK_IN1_N         => idn_ClockFromFMC_p,
        -- Clock out ports
        CLK_OUT1          => MmcmSerialClk_s,
        CLK_OUT2          => MmcmDesignClk_s,
        RESET             => MmcmRst_s,
        LOCKED            => mmcm_locked_s
    );

    o_SerialClk_p   <= MmcmSerialClk_s;
    o_DesignClk_p   <= MmcmDesignClk_s;

    -------------------------------------------------------------------------------
    --  If clock master instance, use MMCM generated clock
    -------------------------------------------------------------------------------
    clock_master_gen: if C_CLOCK_MASTER generate
    begin
        clock_master_s  <= '1';
        SerialClk_s     <= MmcmSerialClk_s;
        DesignClk_s     <= MmcmDesignClk_s;
    end generate clock_master_gen;

    -------------------------------------------------------------------------------
    --  If clock slave instance, use clocks from an other MO1000 core
    -------------------------------------------------------------------------------
    clock_slave_gen: if not C_CLOCK_MASTER generate
    begin
        clock_master_s  <= '0';
        SerialClk_s     <= i_SerialClk_p;
        DesignClk_s     <= i_DesignClk_p;
    end generate clock_slave_gen;

    -------------------------------------------------------------------------------
    --  MO1000 wrapper
    -------------------------------------------------------------------------------
    mo1000_wrapper_i : entity lyt_axi_mo1000_v1_00_a.mo1000_wrapper
    generic map
    (
        C_AXI_CLK_FREQ_MHZ => C_AXI_CLK_FREQ_MHZ,
        C_REF_CLK_FREQ_MHZ => C_REF_CLK_FREQ_MHZ
    )
    port map
    (
        -- system  ports
        i_Rst_p                 => CoreReset_s,
        i_AxiClk_p              => S_AXI_ACLK,
        i_RefClk_p              => i_RefClk_p,
        i_DesignClk_p           => DesignClk_s,
        i_SerialClk_p           => SerialClk_s,

        -- FMC interface ports
        i_TriggerFromFMC_p      => i_TriggerFromFMC_p,
        odp_DataToFMC_p         => odp_DataToFMC_p,
        odn_DataToFMC_p         => odn_DataToFMC_p,
        odp_DciToFMC_p          => odp_DciToFMC_p,
        odn_DciToFMC_p          => odn_DciToFMC_p,
        odp_FrameToFMC_p        => odp_FrameToFMC_p,
        odn_FrameToFMC_p        => odn_FrameToFMC_p,

        -- User ports
        iv16_DacDataCh1_p       => iv16_DacDataCh1_p,
        iv16_DacDataCh2_p       => iv16_DacDataCh2_p,
        iv16_DacDataCh3_p       => iv16_DacDataCh3_p,
        iv16_DacDataCh4_p       => iv16_DacDataCh4_p,
        iv16_DacDataCh5_p       => iv16_DacDataCh5_p,
        iv16_DacDataCh6_p       => iv16_DacDataCh6_p,
        iv16_DacDataCh7_p       => iv16_DacDataCh7_p,
        iv16_DacDataCh8_p       => iv16_DacDataCh8_p,
        o_Trigger_p             => o_Trigger_p,

        -- AXI register ports
        i_delay_rst_p           => delay_rst_s,
        i_data_pattern_en_p     => data_pattern_en_s,
        i_global_serdes_pd_p    => global_serdes_pd_s,
        i_delay_ctrl_rst_p      => delay_ctrl_rst_s,
        iv8_data_serdes_pd_p    => v8_data_serdes_pd_s,
        iv2_frame_mode_p        => v2_frame_mode_s,
        i_frame_mode_write_p    => frame_mode_write_s,
        iv2_frame_pattern_p     => v2_frame_pattern_s,
        i_invert_dci_p          => invert_dci_s,
        i_serdes_rst_p          => serdes_rst_s,
        o_delay_ctrl_rdy_p      => delay_ctrl_rdy_s,
        iv5_delay_value_p       => v5_delay_value_s,
        i_delay_we_dci_p        => delay_we_dci_s,
        i_delay_we_frame_p      => delay_we_frame_s,
        iv32_delay_we_data_p    => v32_delay_we_data_s,
        iv16_pattern_a_p        => v16_pattern_a_s,
        iv16_pattern_b_p        => v16_pattern_b_s,
        iv16_pattern_c_p        => v16_pattern_c_s,
        iv16_pattern_d_p        => v16_pattern_d_s,
        ov16_freq_cnt_p         => v16_freq_cnt_s,
        i_freq_cnt_rst_p        => freq_cnt_rst_s,
        o_freq_cnt_rdy_p        => freq_cnt_rdy_s,
        i_freq_cnt_sel_p        => freq_cnt_sel_s,
        iv5_trigger_delay_p     => v5_trigger_delay_s
    );

    --------------------------------------------
    -- instantiate AXI Memory Mapped User Logic
    --------------------------------------------
    v4_FmcPosition_s <= std_logic_vector(to_unsigned(C_FMC_POSITION,4));
    
    USER_LOGIC_I : entity lyt_axi_mo1000_v1_00_a.axi_mo1000
    generic map
    (
        -- MAP USER GENERICS BELOW THIS LINE ---------------
        C_BUILD_REVISION => C_BUILD_REVISION,

        -- MAP USER GENERICS ABOVE THIS LINE ---------------
        C_S_AXI_DATA_WIDTH        => C_S_AXI_DATA_WIDTH,
        C_S_AXI_ADDR_WIDTH        => C_S_AXI_ADDR_WIDTH,
        C_S_AXI_MIN_SIZE          => C_S_AXI_MIN_SIZE  ,
        C_USE_WSTRB               => C_USE_WSTRB       ,
        C_DPHASE_TIMEOUT          => C_DPHASE_TIMEOUT  ,
        C_BASEADDR                => C_BASEADDR        ,
        C_HIGHADDR                => C_HIGHADDR        ,
        C_FAMILY                  => C_FAMILY
    )
    port map
    (
        -- user_logic entity ports mapping  ---------------
        i_CoreReset_p           => CoreReset_s,
        o_core_rst_p            => core_rst_s,
        o_delay_rst_p           => delay_rst_s,
        ov8_dac_ready_p         => v8_dac_ready_s,
        o_data_pattern_en_p     => data_pattern_en_s,
        o_global_serdes_pd_p    => global_serdes_pd_s,
        o_delay_ctrl_rst_p      => delay_ctrl_rst_s,
        ov8_data_serdes_pd_p    => v8_data_serdes_pd_s,
        ov2_frame_mode_p        => v2_frame_mode_s,
        o_frame_modeWrite_p     => frame_mode_write_s,
        ov2_frame_pattern_p     => v2_frame_pattern_s,
        o_invert_dci_p          => invert_dci_s,
        o_serdes_rst_p          => serdes_rst_s,
        o_mmcm_rst_p            => mmcm_rst_s,
        o_update_dac_ready_p    => update_dac_ready_s,
        i_clock_master_p        => clock_master_s,
        i_delay_ctrl_rdy_p      => delay_ctrl_rdy_s,
        i_mmcm_locked_p         => mmcm_locked_s,
        iv4_FmcPosition_p       => v4_FmcPosition_s,
        ov5_delay_value_p       => v5_delay_value_s,
        o_delay_we_dci_p        => delay_we_dci_s,
        o_delay_we_frame_p      => delay_we_frame_s,
        ov32_delay_we_data_p    => v32_delay_we_data_s,
        ov16_pattern_a_p        => v16_pattern_a_s,
        ov16_pattern_b_p        => v16_pattern_b_s,
        ov16_pattern_c_p        => v16_pattern_c_s,
        ov16_pattern_d_p        => v16_pattern_d_s,
        iv16_freq_cnt_p         => v16_freq_cnt_s,
        o_freq_cnt_rst_p        => freq_cnt_rst_s,
        i_freq_cnt_rdy_p        => freq_cnt_rdy_s,
        o_freq_cnt_sel_p        => freq_cnt_sel_s,
        ov5_trigger_delay_p     => v5_trigger_delay_s,
        -- Bus Protocol Ports mapping --
        S_AXI_ACLK              => S_AXI_ACLK    ,
        S_AXI_ARESETN           => S_AXI_ARESETN ,
        S_AXI_AWADDR            => S_AXI_AWADDR  ,
        S_AXI_AWVALID           => S_AXI_AWVALID ,
        S_AXI_WDATA             => S_AXI_WDATA   ,
        S_AXI_WSTRB             => S_AXI_WSTRB   ,
        S_AXI_WVALID            => S_AXI_WVALID  ,
        S_AXI_BREADY            => S_AXI_BREADY  ,
        S_AXI_ARADDR            => S_AXI_ARADDR  ,
        S_AXI_ARVALID           => S_AXI_ARVALID ,
        S_AXI_RREADY            => S_AXI_RREADY  ,
        S_AXI_ARREADY           => S_AXI_ARREADY ,
        S_AXI_RDATA             => S_AXI_RDATA   ,
        S_AXI_RRESP             => S_AXI_RRESP   ,
        S_AXI_RVALID            => S_AXI_RVALID  ,
        S_AXI_WREADY            => S_AXI_WREADY  ,
        S_AXI_BRESP             => S_AXI_BRESP   ,
        S_AXI_BVALID            => S_AXI_BVALID  ,
        S_AXI_AWREADY           => S_AXI_AWREADY
    );

    --------------------------------------------
    -- SW reset pulse stretcher.
    --------------------------------------------
    process(S_AXI_ACLK)
    begin
        if rising_edge(S_AXI_ACLK) then
            if i_Rst_p = '1' then
                v8_CoreResetStretch_s  <= (others => '1');
                CoreReset_s         <= '1';
            else
                v8_CoreResetStretch_s  <= v8_CoreResetStretch_s(6 downto 0) & core_rst_s;
                CoreReset_s         <= or_reduce(v8_CoreResetStretch_s);
            end if;
        end if;
    end process;
    
    --------------------------------------------
    -- MMCM reset pulse stretcher.
    --------------------------------------------
    process(S_AXI_ACLK)
    begin
        if rising_edge(S_AXI_ACLK) then
            if i_Rst_p = '1' then
                v8_MmcmRstStretch_s <= (others => '1');
                MmcmRst_s           <= '1';
            else
                v8_MmcmRstStretch_s <= v8_MmcmRstStretch_s(6 downto 0) & core_rst_s;
                MmcmRst_s           <= or_reduce(v8_MmcmRstStretch_s);
            end if;
        end if;
    end process;
    
    --------------------------------------------
    -- Convert update_dac_ready_s pulse from AXI clock domain
    -- to pulse in the design clock domain
    --------------------------------------------
    p2p_update_dac_ready : entity lyt_axi_mo1000_v1_00_a.pulse2pulse
    port map (
        in_clk   => S_AXI_ACLK,
        out_clk  => DesignClk_s,
        rst      => CoreReset_s,
        pulsein  => update_dac_ready_s,
        inbusy   => open,
        pulseout => pUpdate_dac_ready_s);

    --------------------------------------------
    -- Update dac ready signal only when pulse on update_dac_ready
    --------------------------------------------
    process(CoreReset_s, DesignClk_s)
    begin
        if (CoreReset_s = '1') then
            o_DacRdyCh1_p   <= '0';
            o_DacRdyCh2_p   <= '0';
            o_DacRdyCh3_p   <= '0';
            o_DacRdyCh4_p   <= '0';
            o_DacRdyCh5_p   <= '0';
            o_DacRdyCh6_p   <= '0';
            o_DacRdyCh7_p   <= '0';
            o_DacRdyCh8_p   <= '0';

        elsif rising_edge(DesignClk_s) then
            if pUpdate_dac_ready_s = '1' then
                o_DacRdyCh1_p   <= v8_dac_ready_s(0);
                o_DacRdyCh2_p   <= v8_dac_ready_s(1);
                o_DacRdyCh3_p   <= v8_dac_ready_s(2);
                o_DacRdyCh4_p   <= v8_dac_ready_s(3);
                o_DacRdyCh5_p   <= v8_dac_ready_s(4);
                o_DacRdyCh6_p   <= v8_dac_ready_s(5);
                o_DacRdyCh7_p   <= v8_dac_ready_s(6);
                o_DacRdyCh8_p   <= v8_dac_ready_s(7);
            end if;
        end if;
    end process;

end arch;
