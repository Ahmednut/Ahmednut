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
-- File        : $Id: lyt_lvds_sync.vhd,v 1.2 2015/11/27 19:34:26 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description :
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_lvds_sync.vhd,v $
-- Revision 1.2  2015/11/27 19:34:26  julien.roy
-- Add Save attribute to avoid optimization that will remove the T connection.
-- The T connection is required when the IODELAY configured in IO direction.
--
-- Revision 1.1  2015/04/10 19:48:59  sqa
-- bas first commit
--
-- Revision 1.1  2013/09/24 18:23:08  julien.roy
-- Add lyt_lvds_sync_v1_00_a core
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

library lyt_lvds_sync_v1_00_a;
use lyt_lvds_sync_v1_00_a.all;

library unisim;
  use unisim.vcomponents.all;

entity lyt_lvds_sync is
    generic
    (
        DATA_WIDTH          : integer range 1 to 32 := 8;
        DATA_INPUT_DELAY    : integer range 0 to 31 := 0;
        DATA_OUTPUT_DELAY   : integer range 0 to 31 := 0;
        CLOCK_INPUT_DELAY   : integer range 0 to 31 := 0;
        CLOCK_OUTPUT_DELAY  : integer range 0 to 31 := 0;
        FALLING_EDGE_INPUT  : boolean := false;
        FALLING_EDGE_OUTPUT : boolean := false;
        RX_CLK_USE_BUFG     : boolean := false
    );
    port
    (
        -- Register ports
        i_SystemClk_p       : in  std_logic;
        ov32_InfoRegister_p : out std_logic_vector(31 downto 0);
        iv32_CtrlRegister_p : in  std_logic_vector(31 downto 0);
        iv32_OERegister_p   : in  std_logic_vector(31 downto 0);
        -- User ports
        i_UserClk_p         : in  std_logic;
        i_OutputEnable_p    : in  std_logic;
        -- RX
        o_RxClk_p           : out std_logic;
        o_RxReady_p         : out std_logic;
        i_RxRe_p            : in  std_logic;
        ov_RxData_p         : out std_logic_vector(DATA_WIDTH-1 downto 0);
        o_RxDataValid_p     : out std_logic;
        -- TX
        i_TxClk_p           : in  std_logic;
        o_TxReady_p         : out std_logic;
        iv_TxData_p         : in  std_logic_vector(DATA_WIDTH-1 downto 0);
        i_TxWe_p            : in  std_logic;
        -- LVDS ports
        iodp_lvds_clk_p     : inout std_logic;
        iodn_lvds_clk_p     : inout std_logic;
        iodp_lvds_valid_p   : inout std_logic;
        iodn_lvds_valid_p   : inout std_logic;
        iovdp_lvds_data_p   : inout std_logic_vector(DATA_WIDTH-1 downto 0);
        iovdn_lvds_data_p   : inout std_logic_vector(DATA_WIDTH-1 downto 0)
    );

end entity lyt_lvds_sync;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture arch of lyt_lvds_sync is

    component fifo32w16d
        port (
            rst : in std_logic;
            wr_clk : in std_logic;
            rd_clk : in std_logic;
            din : in std_logic_vector(31 downto 0);
            wr_en : in std_logic;
            rd_en : in std_logic;
            dout : out std_logic_vector(31 downto 0);
            full : out std_logic;
            almost_full : out std_logic;
            wr_ack : out std_logic;
            empty : out std_logic;
            almost_empty : out std_logic;
            valid : out std_logic
        );
    end component;

    -- System signal
    signal input_reset_s        : std_logic;
    signal v8_SignalStretch_s   : std_logic_vector(7 downto 0) := (others => '0');
    signal CoreReset_s          : std_logic;

    signal OEn_s                : std_logic;
    
    -- Clock
    signal RxClkBufr_s              : std_logic;
    signal clk_iodelay_DATAOUT_s    : std_logic;
    signal clk_iobufds_O_s          : std_logic;
    signal oddr_tx_Q_s              : std_logic;

    -- Data and valid
    signal v_TxData_s           : std_logic_vector(DATA_WIDTH downto 0);
    signal v_RxData_s           : std_logic_vector(DATA_WIDTH downto 0);
    signal v_iobufds_O_s        : std_logic_vector(DATA_WIDTH downto 0);
    signal v_iodelay_DATAOUT_s  : std_logic_vector(DATA_WIDTH downto 0);
    signal v_io_dff_tx_dout_s  : std_logic_vector(DATA_WIDTH downto 0);

    -- RX FIFO
    signal fifo_rx_din          : std_logic_vector(31 downto 0);
    signal fifo_rx_dout         : std_logic_vector(31 downto 0);
    signal fifo_rx_wr_en        : std_logic;
    signal fifo_rx_empty        : std_logic;
    
    -- TX FIFO
    signal fifo_tx_din          : std_logic_vector(31 downto 0);
    signal fifo_tx_dout         : std_logic_vector(31 downto 0);
    signal fifo_tx_almost_full  : std_logic;
    signal fifo_tx_valid        : std_logic;
    
    attribute S : string;
    attribute S of v_io_dff_tx_dout_s   : signal is "true";
    attribute S of OEn_s                : signal is "true";
    

begin

    ov32_InfoRegister_p <= X"1010" & X"0100"; -- Core ID 1010, Version 1.0

    --------------------------------------------
    -- SW reset pulse stretcher.
    --------------------------------------------
    input_reset_s <= iv32_CtrlRegister_p(0);

    process(i_SystemClk_p)
    begin
        if rising_edge(i_SystemClk_p) then
            v8_SignalStretch_s <= v8_SignalStretch_s(6 downto 0) & input_reset_s;
            CoreReset_s <= or_reduce(v8_SignalStretch_s);
        end if;
    end process;

    --------------------------------------------
    -- RX FIFO
    --------------------------------------------
    fifo_rx : fifo32w16d
    port map (
        rst            => CoreReset_s,
        wr_clk         => RxClkBufr_s,
        rd_clk         => i_UserClk_p,
        din            => fifo_rx_din,
        wr_en          => fifo_rx_wr_en,
        rd_en          => i_RxRe_p,
        dout           => fifo_rx_dout,
        full           => open,
        almost_full    => open,
        wr_ack         => open,
        empty          => fifo_rx_empty,
        almost_empty   => open,
        valid          => o_RxDataValid_p
    );

    ov_RxData_p <= fifo_rx_dout(DATA_WIDTH-1 downto 0);
    o_RxReady_p <= not fifo_rx_empty;

    --------------------------------------------
    -- TX FIFO
    --------------------------------------------
    fifo_tx_din <= std_logic_vector(resize(unsigned(iv_TxData_p),fifo_tx_din'length));
    
    fifo_tx : fifo32w16d
    port map (
        rst            => CoreReset_s,
        wr_clk         => i_UserClk_p,
        rd_clk         => i_TxClk_p,
        din            => fifo_tx_din,
        wr_en          => i_TxWe_p,
        rd_en          => '1',
        dout           => fifo_tx_dout,
        full           => open,
        almost_full    => fifo_tx_almost_full,
        wr_ack         => open,
        empty          => open,
        almost_empty   => open,
        valid          => fifo_tx_valid
    );
    o_TxReady_p <= not fifo_tx_almost_full;
    
    -- Enable output if OE register or OE port is high
    OEn_s <= not (iv32_OERegister_p(0) or i_OutputEnable_p);
    
    --------------------------------------------
    -- Clock buffer
    --------------------------------------------
    -- IO differential buffer
    clk_iobufds_inst : IOBUFDS
    generic map (
        IOSTANDARD => "BLVDS_25"
    )
    port map (
        O     => clk_iobufds_O_s,
        IO    => iodp_lvds_clk_p,
        IOB   => iodn_lvds_clk_p,
        I     => clk_iodelay_DATAOUT_s,
        T     => OEn_s
    );
    
    -- Delay input and output clock
    clk_iodelay_inst : IODELAYE1
    generic map (
        CINVCTRL_SEL => FALSE,
        DELAY_SRC => "IO",
        HIGH_PERFORMANCE_MODE => TRUE,
        IDELAY_TYPE => "FIXED",
        IDELAY_VALUE => CLOCK_INPUT_DELAY,
        ODELAY_TYPE => "FIXED",
        ODELAY_VALUE => CLOCK_OUTPUT_DELAY,
        REFCLK_FREQUENCY => 200.0,
        SIGNAL_PATTERN => "CLOCK"
    )
    port map (
        CNTVALUEOUT => open,
        DATAOUT => clk_iodelay_DATAOUT_s,
        C => '0',
        CE => '0',
        INC => '0',
        CINVCTRL => '0',
        CLKIN => '0',
        CNTVALUEIN => "00000",
        DATAIN => '0',
        IDATAIN => clk_iobufds_O_s,
        ODATAIN => oddr_tx_Q_s,
        RST => '0',
        T => OEn_s
    );
    
    -- Generate TX clock output from i_TxClk_p
    ODDR_TX_CLOCK : ODDR
    generic map(
        DDR_CLK_EDGE   => "OPPOSITE_EDGE",
        INIT           => '0',
        SRTYPE         => "ASYNC")
    port map(
        Q           => oddr_tx_Q_s,
        C           => i_TxClk_p,
        CE          => '1',
        D1          => '1',
        D2          => '0',
        R           => '0',
        S           => '0'
    );
    
    -- RX BUFR clock
    BUFR_inst : BUFR
    generic map(
        BUFR_DIVIDE => "BYPASS",
        SIM_DEVICE => "VIRTEX6")
    port map(
        O       => RxClkBufr_s,    
        CE      => '1',
        CLR     => CoreReset_s,
        I       => clk_iodelay_DATAOUT_s
    );
    
    rx_clk_bufg_gen : if RX_CLK_USE_BUFG = true generate
        -- RX BUFG clock
        BUFG_inst : BUFG
        port map (
            O => o_RxClk_p,
            I => RxClkBufr_s 
        );
    end generate;
    
    not_rx_clk_bufg_gen : if RX_CLK_USE_BUFG = false generate
        o_RxClk_p <= RxClkBufr_s;
    end generate;

    --------------------------------------------
    -- Instantiate a buffer for each LVDS pair
    --------------------------------------------
    -- LVDS pair 0 :                data valid
    -- LVDS pair 1 to DATA_WIDTH :  data
    
    v_TxData_s <= fifo_tx_dout(DATA_WIDTH-1 downto 0) & fifo_tx_valid;

    fifo_rx_din <= std_logic_vector(resize(unsigned(v_RxData_s(v_RxData_s'high downto 1)),32));
    fifo_rx_wr_en <= v_RxData_s(0);
    
    lvds_buffer_generate : for i in 0 to DATA_WIDTH generate

        iobufds_valid_gen : if (i = 0) generate
        -- IO differential buffer
            iobufds_inst : IOBUFDS
            generic map (
                IOSTANDARD => "BLVDS_25"
            )
            port map (
                O     => v_iobufds_O_s(i),
                IO    => iodp_lvds_valid_p,
                IOB   => iodn_lvds_valid_p,
                I     => v_iodelay_DATAOUT_s(i),
                T     => OEn_s
            );
        end generate;
        
        iobufds_data_gen : if (i /= 0) generate
        -- IO differential buffer
            iobufds_inst : IOBUFDS
            generic map (
                IOSTANDARD => "BLVDS_25"
            )
            port map (
                O     => v_iobufds_O_s(i),
                IO    => iovdp_lvds_data_p(i-1),
                IOB   => iovdn_lvds_data_p(i-1),
                I     => v_iodelay_DATAOUT_s(i),
                T     => OEn_s
            );
        end generate;

        -- Delay input and output data
        iodelay_inst : IODELAYE1
        generic map (
            CINVCTRL_SEL => FALSE,
            DELAY_SRC => "IO",
            HIGH_PERFORMANCE_MODE => TRUE,
            IDELAY_TYPE => "FIXED",
            IDELAY_VALUE => DATA_INPUT_DELAY,
            ODELAY_TYPE => "FIXED",
            ODELAY_VALUE => DATA_OUTPUT_DELAY,
            REFCLK_FREQUENCY => 200.0,
            SIGNAL_PATTERN => "DATA"
        )
        port map (
            CNTVALUEOUT => open,
            DATAOUT => v_iodelay_DATAOUT_s(i),
            C => '0',
            CE => '0',
            INC => '0',
            CINVCTRL => '0',
            CLKIN => '0',
            CNTVALUEIN => "00000",
            DATAIN => '0',
            IDATAIN => v_iobufds_O_s(i),
            ODATAIN => v_io_dff_tx_dout_s(i),
            RST => '0',
            T => OEn_s
        );
        
        -- Input data and valid flip-flop
        io_dff_rx : entity lyt_lvds_sync_v1_00_a.io_dff
        generic map (
        io_dff_Falling_edge => FALLING_EDGE_INPUT
        )
        port map(
        clk   => RxClkBufr_s,
        din   => v_iodelay_DATAOUT_s(i),
        dout  => v_RxData_s(i)
        );

        -- Output data and valid flip-flop
        io_dff_tx : entity lyt_lvds_sync_v1_00_a.io_dff
        generic map (
            io_dff_Falling_edge => FALLING_EDGE_OUTPUT
        )
        port map(
            clk   => i_TxClk_p,
            din   => v_TxData_s(i),
            dout  => v_io_dff_tx_dout_s(i)
        );

    end generate;
    

end arch;
