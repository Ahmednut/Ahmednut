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
-- File        : $Id: mo1000_wrapper.vhd,v 1.1 2015/04/10 19:48:35 sqa Exp $
--------------------------------------------------------------------------------
-- Description : MO1000 logic wrapper
--------------------------------------------------------------------------------
-- Notes / Assumptions : Simple wrapper to the MO1000 interface logic.
--------------------------------------------------------------------------------
-- Copyright (c) 2014 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: mo1000_wrapper.vhd,v $
-- Revision 1.1  2015/04/10 19:48:35  sqa
-- bas first commit
--
-- Revision 1.3  2014/10/07 17:14:36  julien.roy
-- Fix a coding error for frame pulse mode
--
-- Revision 1.2  2014/09/16 13:46:08  julien.roy
-- Change clock architecture to solve DAC FIFO warning errors.
-- Add new frame mode.
-- Add more register stage to ease timing.
--
-- Revision 1.1  2014/06/18 14:43:05  julien.roy
-- Add first version of the mo1000 core
--
--
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library unisim;
use unisim.vcomponents.all;

library lyt_axi_mo1000_v1_00_a;
use lyt_axi_mo1000_v1_00_a.all;

entity mo1000_wrapper is
    generic
    (
        C_AXI_CLK_FREQ_MHZ      : integer   := 100;
        C_REF_CLK_FREQ_MHZ      : real      := 200.0;
        C_DATA_REGISTER_STAGE   : integer   := 1
    );
    port
    (
        -- system  ports
        i_Rst_p                 : in  std_logic;
        i_AxiClk_p              : in  std_logic;
        i_RefClk_p              : in  std_logic;
        i_DesignClk_p           : in  std_logic;
        i_SerialClk_p           : in  std_logic;

        -- FMC interface ports
        i_TriggerFromFMC_p      : in  std_logic;
        odp_DataToFMC_p         : out std_logic_vector(31 downto 0);
        odn_DataToFMC_p         : out std_logic_vector(31 downto 0);
        odp_DciToFMC_p          : out std_logic;
        odn_DciToFMC_p          : out std_logic;
        odp_FrameToFMC_p        : out std_logic;
        odn_FrameToFMC_p        : out std_logic;

        -- User ports
        iv16_DacDataCh1_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh2_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh3_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh4_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh5_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh6_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh7_p       : in  std_logic_vector(15 downto 0);
        iv16_DacDataCh8_p       : in  std_logic_vector(15 downto 0);
        o_Trigger_p             : out std_logic;

        -- AXI register ports
        i_delay_rst_p           : in  std_logic;
        i_data_pattern_en_p     : in  std_logic;
        i_global_serdes_pd_p    : in  std_logic;
        i_delay_ctrl_rst_p      : in  std_logic;
        iv8_data_serdes_pd_p    : in  std_logic_vector(7 downto 0);
        iv2_frame_mode_p        : in  std_logic_vector(1 downto 0);
        i_frame_mode_write_p    : in  std_logic;
        iv2_frame_pattern_p     : in  std_logic_vector(1 downto 0);
        i_invert_dci_p          : in  std_logic;
        i_serdes_rst_p          : in  std_logic;
        o_delay_ctrl_rdy_p      : out std_logic;
        iv5_delay_value_p       : in  std_logic_vector(4 downto 0);
        i_delay_we_dci_p        : in  std_logic;
        i_delay_we_frame_p      : in  std_logic;
        iv32_delay_we_data_p    : in  std_logic_vector(31 downto 0);
        iv16_pattern_a_p        : in  std_logic_vector(15 downto 0);
        iv16_pattern_b_p        : in  std_logic_vector(15 downto 0);
        iv16_pattern_c_p        : in  std_logic_vector(15 downto 0);
        iv16_pattern_d_p        : in  std_logic_vector(15 downto 0);
        ov16_freq_cnt_p         : out std_logic_vector(15 downto 0);
        i_freq_cnt_rst_p        : in  std_logic;
        o_freq_cnt_rdy_p        : out std_logic;
        i_freq_cnt_sel_p        : in  std_logic;
        iv5_trigger_delay_p     : in  std_logic_vector(4 downto 0)
    );
end mo1000_wrapper;

architecture arch of mo1000_wrapper is

    type av16 is array (0 to C_DATA_REGISTER_STAGE) of std_logic_vector(15 downto 0);

    --------------------------------------------
    -- Signal section
    --------------------------------------------
    -- Latch control signals
    signal data_pattern_en_r1_s        : std_logic := '0';
    signal data_pattern_en_r2_s        : std_logic := '0';
    signal v8_data_serdes_pd_r1_s      : std_logic_vector(7 downto 0)   := (others => '0');
    signal v8_data_serdes_pd_r2_s      : std_logic_vector(7 downto 0)   := (others => '0');
    signal v8_data_serdes_pd_r3_s      : std_logic_vector(7 downto 0)   := (others => '0');
    signal global_serdes_pd_r1_s       : std_logic := '0';
    signal global_serdes_pd_r2_s       : std_logic := '0';
    signal rst_r1_s                    : std_logic := '0';
    signal rst_r2_s                    : std_logic := '0';
    signal v2_frame_mode_r1_s          : std_logic_vector(1 downto 0)   := (others => '0');
    signal v2_frame_mode_r2_s          : std_logic_vector(1 downto 0)   := (others => '0');
    signal v2_frame_mode_latch_s       : std_logic_vector(1 downto 0)   := (others => '0');
    signal v2_frame_pattern_r1_s       : std_logic_vector(1 downto 0)   := (others => '0');
    signal v2_frame_pattern_r2_s       : std_logic_vector(1 downto 0)   := (others => '0');
    signal v2_frame_pattern_latch_s    : std_logic_vector(1 downto 0)   := (others => '0');
    signal v5_trigger_delay_r1_s       : std_logic_vector(4 downto 0)   := (others => '0');
    signal v5_trigger_delay_r2_s       : std_logic_vector(4 downto 0)   := (others => '0');

    -- Latch data
    signal v16_DacDataCh1_s            : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacDataCh2_s            : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacDataCh3_s            : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacDataCh4_s            : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacDataCh5_s            : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacDataCh6_s            : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacDataCh7_s            : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacDataCh8_s            : std_logic_vector(15 downto 0)  := (others => '0');

    -- Data mux
    signal v16_DacMuxCh1_s             : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh2_s             : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh3_s             : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh4_s             : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh5_s             : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh6_s             : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh7_s             : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh8_s             : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh1_r1_s          : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh2_r1_s          : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh3_r1_s          : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh4_r1_s          : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh5_r1_s          : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh6_r1_s          : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh7_r1_s          : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacMuxCh8_r1_s          : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacCh1_s                : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacCh2_s                : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacCh3_s                : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacCh4_s                : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacCh5_s                : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacCh6_s                : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacCh7_s                : std_logic_vector(15 downto 0)  := (others => '0');
    signal v16_DacCh8_s                : std_logic_vector(15 downto 0)  := (others => '0');
    signal av16_DacMuxCh1_r_s          : av16;
    signal av16_DacMuxCh2_r_s          : av16;
    signal av16_DacMuxCh3_r_s          : av16;
    signal av16_DacMuxCh4_r_s          : av16;
    signal av16_DacMuxCh5_r_s          : av16;
    signal av16_DacMuxCh6_r_s          : av16;
    signal av16_DacMuxCh7_r_s          : av16;
    signal av16_DacMuxCh8_r_s          : av16;

    -- Frame logic
    signal frame_mode_write_s          : std_logic := '0';
    signal frame_mode_write_r1_s       : std_logic := '0';
    signal frame_mode_write_latch_s    : std_logic := '0';
    signal v4_Frame_s                  : std_logic_vector(3 downto 0)   := (others => '0');
    signal v3_FrameCnt_s               : unsigned(2 downto 0)           := (others => '0');
    signal new_frame_logic_s           : std_logic := '0';

    -- Frequency counter section
    signal freq_cnt_rst_s              : std_logic := '0';
    signal v6_freq_cnt_sel_s           : std_logic_vector(5 downto 0)   := (others => '0');
    signal v_testClocks_s              : std_logic_vector(1 downto 0)   := (others => '0');

    --------------------------------------------
    -- Attribute section
    --------------------------------------------
    attribute keep_hierarchy : string;
    attribute keep_hierarchy of arch : architecture is "true";

    attribute keep : string;
    attribute keep of data_pattern_en_r1_s      : signal is "true";
    attribute keep of v8_data_serdes_pd_r1_s    : signal is "true";
    attribute keep of global_serdes_pd_r1_s     : signal is "true";
    attribute keep of rst_r1_s                  : signal is "true";
    attribute keep of v2_frame_mode_r1_s        : signal is "true";
    attribute keep of v2_frame_pattern_r1_s     : signal is "true";
    attribute keep of v5_trigger_delay_r1_s     : signal is "true";
    attribute keep of av16_DacMuxCh1_r_s        : signal is "true";
    attribute keep of av16_DacMuxCh2_r_s        : signal is "true";
    attribute keep of av16_DacMuxCh3_r_s        : signal is "true";
    attribute keep of av16_DacMuxCh4_r_s        : signal is "true";
    attribute keep of av16_DacMuxCh5_r_s        : signal is "true";
    attribute keep of av16_DacMuxCh6_r_s        : signal is "true";
    attribute keep of av16_DacMuxCh7_r_s        : signal is "true";
    attribute keep of av16_DacMuxCh8_r_s        : signal is "true";
    attribute keep of v16_DacCh1_s              : signal is "true";
    attribute keep of v16_DacCh2_s              : signal is "true";
    attribute keep of v16_DacCh3_s              : signal is "true";
    attribute keep of v16_DacCh4_s              : signal is "true";
    attribute keep of v16_DacCh5_s              : signal is "true";
    attribute keep of v16_DacCh6_s              : signal is "true";
    attribute keep of v16_DacCh7_s              : signal is "true";
    attribute keep of v16_DacCh8_s              : signal is "true";
    
    attribute max_fanout: string;
    attribute max_fanout of v8_data_serdes_pd_r3_s   : signal is "1";
    attribute max_fanout of rst_r1_s                 : signal is "8";
    attribute max_fanout of rst_r2_s                 : signal is "8";
    attribute max_fanout of data_pattern_en_r2_s     : signal is "1";
    
    attribute maxdelay: string;
    attribute maxdelay of iv16_pattern_a_p          : signal is "200 ns";
    attribute maxdelay of iv16_pattern_b_p          : signal is "200 ns";
    attribute maxdelay of iv16_pattern_c_p          : signal is "200 ns";
    attribute maxdelay of iv16_pattern_d_p          : signal is "200 ns";
    attribute maxdelay of i_data_pattern_en_p       : signal is "200 ns";
    attribute maxdelay of iv8_data_serdes_pd_p      : signal is "200 ns";
    attribute maxdelay of i_global_serdes_pd_p      : signal is "200 ns";
    attribute maxdelay of iv5_trigger_delay_p       : signal is "200 ns";

begin

    --------------------------------------------
    -- DAC interfaces
    --------------------------------------------
    dac_interface_i : entity lyt_axi_mo1000_v1_00_a.dac_interface
    generic map(
        C_REF_CLK_FREQ_MHZ      => C_REF_CLK_FREQ_MHZ
    )
    port map(
        -- system  ports
        i_Rst_p                 => i_Rst_p,
        i_AxiClk_p              => i_AxiClk_p,
        i_RefClk_p              => i_RefClk_p,
        i_DesignClk_p           => i_DesignClk_p,
        i_SerialClk_p           => i_SerialClk_p,

        -- FMC interface ports
        odp_DataToFMC_p         => odp_DataToFMC_p,
        odn_DataToFMC_p         => odn_DataToFMC_p,
        odp_DciToFMC_p          => odp_DciToFMC_p,
        odn_DciToFMC_p          => odn_DciToFMC_p,
        odp_FrameToFMC_p        => odp_FrameToFMC_p,
        odn_FrameToFMC_p        => odn_FrameToFMC_p,

        -- User ports
        iv4_Frame_p             => v4_Frame_s,
        iv16_DacDataCh1_p       => v16_DacCh1_s,
        iv16_DacDataCh2_p       => v16_DacCh2_s,
        iv16_DacDataCh3_p       => v16_DacCh3_s,
        iv16_DacDataCh4_p       => v16_DacCh4_s,
        iv16_DacDataCh5_p       => v16_DacCh5_s,
        iv16_DacDataCh6_p       => v16_DacCh6_s,
        iv16_DacDataCh7_p       => v16_DacCh7_s,
        iv16_DacDataCh8_p       => v16_DacCh8_s,

        -- AXI register ports
        i_invert_dci_p          => i_invert_dci_p,
        i_delay_rst_p           => i_delay_rst_p,
        i_global_serdes_pd_p    => i_global_serdes_pd_p,
        i_delay_ctrl_rst_p      => i_delay_ctrl_rst_p,
        i_serdes_rst_p          => i_serdes_rst_p,
        o_delay_ctrl_rdy_p      => o_delay_ctrl_rdy_p,
        iv5_delay_value_p       => iv5_delay_value_p,
        i_delay_we_dci_p        => i_delay_we_dci_p,
        i_delay_we_frame_p      => i_delay_we_frame_p,
        iv32_delay_we_data_p    => iv32_delay_we_data_p
    );

    --------------------------------------------
    -- Latch control signals
    --------------------------------------------
    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then
            data_pattern_en_r1_s    <= i_data_pattern_en_p;
            data_pattern_en_r2_s    <= data_pattern_en_r1_s;

            v8_data_serdes_pd_r1_s  <= iv8_data_serdes_pd_p;
            v8_data_serdes_pd_r2_s  <= v8_data_serdes_pd_r1_s;
            
            if rst_r2_s = '1' or global_serdes_pd_r2_s = '1' then
                v8_data_serdes_pd_r3_s <= (others => '1');
            else
                v8_data_serdes_pd_r3_s <= v8_data_serdes_pd_r2_s;
            end if;
            
            global_serdes_pd_r1_s   <= i_global_serdes_pd_p;
            global_serdes_pd_r2_s   <= global_serdes_pd_r1_s;

            rst_r1_s                <= i_Rst_p;
            rst_r2_s                <= rst_r1_s;

            v5_trigger_delay_r1_s   <= iv5_trigger_delay_p;
            v5_trigger_delay_r2_s   <= v5_trigger_delay_r1_s;
        end if;
    end process;

    --------------------------------------------
    -- Latch data
    --------------------------------------------
    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then
            v16_DacDataCh1_s <= iv16_DacDataCh1_p;
            v16_DacDataCh2_s <= iv16_DacDataCh2_p;
            v16_DacDataCh3_s <= iv16_DacDataCh3_p;
            v16_DacDataCh4_s <= iv16_DacDataCh4_p;
            v16_DacDataCh5_s <= iv16_DacDataCh5_p;
            v16_DacDataCh6_s <= iv16_DacDataCh6_p;
            v16_DacDataCh7_s <= iv16_DacDataCh7_p;
            v16_DacDataCh8_s <= iv16_DacDataCh8_p;
        end if;
    end process;

    --------------------------------------------
    -- Data mux
    --------------------------------------------
    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then

            if data_pattern_en_r2_s = '0' then
                v16_DacMuxCh1_s <= v16_DacDataCh1_s;
                v16_DacMuxCh2_s <= v16_DacDataCh2_s;
                v16_DacMuxCh3_s <= v16_DacDataCh3_s;
                v16_DacMuxCh4_s <= v16_DacDataCh4_s;
                v16_DacMuxCh5_s <= v16_DacDataCh5_s;
                v16_DacMuxCh6_s <= v16_DacDataCh6_s;
                v16_DacMuxCh7_s <= v16_DacDataCh7_s;
                v16_DacMuxCh8_s <= v16_DacDataCh8_s;
            else
                v16_DacMuxCh1_s <= iv16_pattern_a_p;
                v16_DacMuxCh2_s <= iv16_pattern_b_p;
                v16_DacMuxCh3_s <= iv16_pattern_c_p;
                v16_DacMuxCh4_s <= iv16_pattern_d_p;
                v16_DacMuxCh5_s <= iv16_pattern_a_p;
                v16_DacMuxCh6_s <= iv16_pattern_b_p;
                v16_DacMuxCh7_s <= iv16_pattern_c_p;
                v16_DacMuxCh8_s <= iv16_pattern_d_p;
            end if;
                
            v16_DacMuxCh1_r1_s <= v16_DacMuxCh1_s;
            v16_DacMuxCh2_r1_s <= v16_DacMuxCh2_s;
            v16_DacMuxCh3_r1_s <= v16_DacMuxCh3_s;
            v16_DacMuxCh4_r1_s <= v16_DacMuxCh4_s;
            v16_DacMuxCh5_r1_s <= v16_DacMuxCh5_s;
            v16_DacMuxCh6_r1_s <= v16_DacMuxCh6_s;
            v16_DacMuxCh7_r1_s <= v16_DacMuxCh7_s;
            v16_DacMuxCh8_r1_s <= v16_DacMuxCh8_s;
                
            if v8_data_serdes_pd_r3_s(0) = '1' then
                v16_DacMuxCh1_r1_s <= (others => '0');
            end if;
            if v8_data_serdes_pd_r3_s(1) = '1' then
                v16_DacMuxCh2_r1_s <= (others => '0');
            end if;
            if v8_data_serdes_pd_r3_s(2) = '1' then
                v16_DacMuxCh3_r1_s <= (others => '0');
            end if;
            if v8_data_serdes_pd_r3_s(3) = '1' then
                v16_DacMuxCh4_r1_s <= (others => '0');
            end if;
            if v8_data_serdes_pd_r3_s(4) = '1' then
                v16_DacMuxCh5_r1_s <= (others => '0');
            end if;
            if v8_data_serdes_pd_r3_s(5) = '1' then
                v16_DacMuxCh6_r1_s <= (others => '0');
            end if;
            if v8_data_serdes_pd_r3_s(6) = '1' then
                v16_DacMuxCh7_r1_s <= (others => '0');
            end if;
            if v8_data_serdes_pd_r3_s(7) = '1' then
                v16_DacMuxCh8_r1_s <= (others => '0');
            end if;

        end if;
    end process;
    
    --------------------------------------------
    -- Data mux register stage
    --------------------------------------------
    av16_DacMuxCh1_r_s(0) <= v16_DacMuxCh1_r1_s;
    av16_DacMuxCh2_r_s(0) <= v16_DacMuxCh2_r1_s;
    av16_DacMuxCh3_r_s(0) <= v16_DacMuxCh3_r1_s;
    av16_DacMuxCh4_r_s(0) <= v16_DacMuxCh4_r1_s;
    av16_DacMuxCh5_r_s(0) <= v16_DacMuxCh5_r1_s;
    av16_DacMuxCh6_r_s(0) <= v16_DacMuxCh6_r1_s;
    av16_DacMuxCh7_r_s(0) <= v16_DacMuxCh7_r1_s;
    av16_DacMuxCh8_r_s(0) <= v16_DacMuxCh8_r1_s;
    
    data_mux_reg_gen: for i in 0 to (C_DATA_REGISTER_STAGE-1) generate
        process(i_DesignClk_p)
        begin
            if rising_edge(i_DesignClk_p) then
                av16_DacMuxCh1_r_s(i+1) <= av16_DacMuxCh1_r_s(i);
                av16_DacMuxCh2_r_s(i+1) <= av16_DacMuxCh2_r_s(i);
                av16_DacMuxCh3_r_s(i+1) <= av16_DacMuxCh3_r_s(i);
                av16_DacMuxCh4_r_s(i+1) <= av16_DacMuxCh4_r_s(i);
                av16_DacMuxCh5_r_s(i+1) <= av16_DacMuxCh5_r_s(i);
                av16_DacMuxCh6_r_s(i+1) <= av16_DacMuxCh6_r_s(i);
                av16_DacMuxCh7_r_s(i+1) <= av16_DacMuxCh7_r_s(i);
                av16_DacMuxCh8_r_s(i+1) <= av16_DacMuxCh8_r_s(i);
            end if;
        end process;
    end generate;
    
    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then
            v16_DacCh1_s <= av16_DacMuxCh1_r_s(C_DATA_REGISTER_STAGE);
            v16_DacCh2_s <= av16_DacMuxCh2_r_s(C_DATA_REGISTER_STAGE);
            v16_DacCh3_s <= av16_DacMuxCh3_r_s(C_DATA_REGISTER_STAGE);
            v16_DacCh4_s <= av16_DacMuxCh4_r_s(C_DATA_REGISTER_STAGE);
            v16_DacCh5_s <= av16_DacMuxCh5_r_s(C_DATA_REGISTER_STAGE);
            v16_DacCh6_s <= av16_DacMuxCh6_r_s(C_DATA_REGISTER_STAGE);
            v16_DacCh7_s <= av16_DacMuxCh7_r_s(C_DATA_REGISTER_STAGE);
            v16_DacCh8_s <= av16_DacMuxCh8_r_s(C_DATA_REGISTER_STAGE);
        end if;
    end process;

    --------------------------------------------
    -- Frame logic
    --------------------------------------------
    p2p_frame_mode_write_i : entity lyt_axi_mo1000_v1_00_a.pulse2pulse
    port map (
        in_clk   => i_AxiClk_p,
        out_clk  => i_DesignClk_p,
        rst      => i_Rst_p,
        pulsein  => i_frame_mode_write_p,
        inbusy   => open,
        pulseout => frame_mode_write_s);

    process(i_DesignClk_p)
    begin
        if rising_edge(i_DesignClk_p) then

            v2_frame_pattern_r1_s   <= iv2_frame_pattern_p;
            v2_frame_pattern_r2_s   <= v2_frame_pattern_r1_s;

            v2_frame_mode_r1_s      <= iv2_frame_mode_p;
            
            frame_mode_write_r1_s   <= frame_mode_write_s;

            -- Latch the event of new frame_mode until the next
            -- frame period (8 design clock cycles) is reached
            if frame_mode_write_r1_s = '1' then
                v2_frame_mode_r2_s          <= v2_frame_mode_r1_s;
                frame_mode_write_latch_s    <= '1';
            end if;

            -- Change only the mode and pattern of the frame logic
            -- when a full period of 8 design clock cycles is completed
            new_frame_logic_s <= '0';
            if v3_FrameCnt_s = "111" then
                v2_frame_mode_latch_s       <= v2_frame_mode_r2_s;
                v2_frame_pattern_latch_s    <= v2_frame_pattern_r2_s;

                -- If new frame_mode event, clear frame_mode_write_latch_s
                -- activate flag new_frame_logic_s for specific
                -- frame sequence start logic
                if frame_mode_write_latch_s = '1' or (v2_frame_pattern_r2_s /= v2_frame_pattern_latch_s) then
                    frame_mode_write_latch_s <= '0';
                    new_frame_logic_s <= '1';
                end if;
            end if;

            -- Do not restart the counter in pulse modes : "01"
            if (v2_frame_mode_r2_s /= "01" or frame_mode_write_latch_s = '1') or v3_FrameCnt_s /= "111" then
                v3_FrameCnt_s <= v3_FrameCnt_s + 1;
            end if;

            -- Set frame to 0 by default
            v4_Frame_s <= "0000";

            -- Normal continuous mode
            if v2_frame_mode_latch_s = "00" then
                if v2_frame_pattern_latch_s = "00" then
                    v4_Frame_s <= "0011";
                elsif v2_frame_pattern_latch_s = "01" then
                    v4_Frame_s <= "0110";
                elsif v2_frame_pattern_latch_s = "10" then
                    v4_Frame_s <= "1100";
                else -- v2_frame_pattern_latch_s = "11"
                    if new_frame_logic_s = '0' then
                        v4_Frame_s <= "1001";
                    else
                        -- To avoid only 1 half DCI period high
                        -- when starting a new frame sequence
                        v4_Frame_s <= "1000";
                    end if;
                end if;

            -- Normal pulse mode
            elsif v2_frame_mode_latch_s = "01" then
                if v3_FrameCnt_s = "000" then
                    if v2_frame_pattern_latch_s = "00" then
                        v4_Frame_s <= "0011";
                    elsif v2_frame_pattern_latch_s = "01" then
                        v4_Frame_s <= "0110";
                    elsif v2_frame_pattern_latch_s = "10" then
                        v4_Frame_s <= "1100";
                    else -- v2_frame_pattern_latch_s = "11"
                        v4_Frame_s <= "1000";
                    end if;
                elsif v3_FrameCnt_s = "001" then
                    if v2_frame_pattern_latch_s = "11" then
                        v4_Frame_s <= "0001";
                    end if;
                end if;
                
            elsif v2_frame_mode_latch_s = "11" then

                if v3_FrameCnt_s(0) = '0' then
                    if v2_frame_pattern_latch_s = "00" then
                        v4_Frame_s <= "0011";
                    elsif v2_frame_pattern_latch_s = "01" then
                        v4_Frame_s <= "0110";
                    elsif v2_frame_pattern_latch_s = "10" then
                        v4_Frame_s <= "1100";
                    else -- v2_frame_pattern_latch_s = "11"
                        v4_Frame_s <= "1000";
                    end if;
                else -- if v3_FrameCnt_s(0) = '1' then
                    if v2_frame_pattern_latch_s = "11" then
                        v4_Frame_s <= "0001";
                    end if;
                end if;
            
            -- Synchronization continuous mode
            -- Synchronization pulse mode (deprecated)
            -- same frame, the difference is that the sequence will restart for the continuous mode
            else -- v2_frame_mode_latch_s = "10"
                if v3_FrameCnt_s = "000" then
                    if v2_frame_pattern_latch_s = "00" then
                        v4_Frame_s <= "1111";
                    elsif v2_frame_pattern_latch_s = "01" then
                        v4_Frame_s <= "1110";
                    elsif v2_frame_pattern_latch_s = "10" then
                        v4_Frame_s <= "1100";
                    else -- v2_frame_pattern_latch_s = "11"
                        v4_Frame_s <= "1000";
                    end if;
                elsif v3_FrameCnt_s = "001" then
                    if v2_frame_pattern_latch_s = "00" then
                        v4_Frame_s <= "0000";
                    elsif v2_frame_pattern_latch_s = "01" then
                        v4_Frame_s <= "0001";
                    elsif v2_frame_pattern_latch_s = "10" then
                        v4_Frame_s <= "0011";
                    else -- v2_frame_pattern_latch_s = "11"
                        v4_Frame_s <= "0111";
                    end if;
                end if;
            end if;

        end if;
    end process;

    --------------------------------------------
    -- Trigger section
    --------------------------------------------
    variable_delay_trigger_i: entity lyt_axi_mo1000_v1_00_a.VariableDelayTrigger
    port map (
        i_clk_p         => i_DesignClk_p,
        iv5_Delay_p     => v5_trigger_delay_r2_s,
        i_Trigger_p     => i_TriggerFromFMC_p,
        o_Trigger_p     => o_Trigger_p
    );

    --------------------------------------------
    -- Frequency counter section
    --------------------------------------------
    freq_cnt_rst_s      <= i_Rst_p or i_freq_cnt_rst_p;
    v6_freq_cnt_sel_s   <= ("00000" & i_freq_cnt_sel_p);
    v_testClocks_s(0)   <= i_RefClk_p;
    v_testClocks_s(1)   <= i_DesignClk_p;

    freq_cnt_top_inst : entity lyt_axi_mo1000_v1_00_a.freq_cnt_top
    generic map(
        C_REFCLK_FREQ_MHZ   => C_AXI_CLK_FREQ_MHZ,
        C_NUM_OF_TEST_CLK   => 2
    )
    port map(
        i_Rst_p             => freq_cnt_rst_s,
        i_RefClk_p          => i_AxiClk_p,
        iv_TestClks_p       => v_testClocks_s,
        iv6_TestClkSel_p    => v6_freq_cnt_sel_s,
        ov16_Freq_p         => ov16_freq_cnt_p,
        o_Rdy_p             => o_freq_cnt_rdy_p
    );

END arch;