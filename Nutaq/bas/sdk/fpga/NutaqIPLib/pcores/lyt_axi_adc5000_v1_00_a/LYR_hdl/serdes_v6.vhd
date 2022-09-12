------------------------------------------------------------------------------
library ieee;
  use ieee.std_logic_1164.all;
  use ieee.std_logic_unsigned.all;
  use ieee.std_logic_arith.all;
  use ieee.std_logic_misc.all;
  use ieee.numeric_std.all;
library unisim;
  use unisim.vcomponents.all;

entity serdes_v6 is
generic (
  SYS_W                 : integer := 10; -- width of the data for the system
  DEV_W                 : integer := 80  -- width of the data for the device
);
port (
  -- From the system into the device
  data_in_from_pins_p   : in    std_logic_vector(SYS_W-1 downto 0);
  data_in_from_pins_n   : in    std_logic_vector(SYS_W-1 downto 0);
  data_in_to_device     : out   std_logic_vector(DEV_W-1 downto 0);
  -- Input, Output delay control signals
  i_rst_cmd_p               : in    std_logic;
  i_clk_cmd_p               : in    std_logic;
  i_clk_reset_p             : in    std_logic; -- reset signal for clock circuit
  i_io_reset_p              : in    std_logic; -- reset signal for io circuit
  i_adcIdelayValue_rst_p    : in    std_logic;
  iv5_adcIdelayValue_p      : in    std_logic_vector(4 downto 0);
  iv_adcIdelayValue_we_p    : in    std_logic_vector(SYS_W-1 downto 0);
  iv_bitslip_p              : in    std_logic_vector(SYS_W-1 downto 0);
  -- Clock and reset signals
  clk_in_p              : in    std_logic; -- differential fast clock from iob
  clk_in_n              : in    std_logic;
  clk_div_out           : out   std_logic; -- data clock output
  REF_CLOCK             : in    std_logic  -- Ref Clk 200 Mhz for IDELAYCTRL
);
end serdes_v6;

architecture serdes_v6_arch of serdes_v6 is

----------------------------------------------------------------------------------------------------
-- Constant declaration
----------------------------------------------------------------------------------------------------
constant SER_W        : integer := DEV_W/SYS_W;
constant IDELAY_VALUE : integer := 15; -- initial delay value between 0 and 31

----------------------------------------------------------------------------------------------------
-- Signal declaration
----------------------------------------------------------------------------------------------------
signal clk_in                    : std_logic;
signal clk_div                   : std_logic;

signal clk_in_int_buf            : std_logic_vector(SYS_W-1 downto 0);
signal clk_in_int_inv            : std_logic_vector(SYS_W-1 downto 0);

signal data_in_from_pins_int     : std_logic_vector(SYS_W-1 downto 0);
signal data_in_from_pins_delay   : std_logic_vector(SYS_W-1 downto 0);

signal bitslip_int               : std_logic_vector(SYS_W-1 downto 0) := (others => '0');
signal io_reset_int              : std_logic;

type serdes_array is array (0 to SYS_W-1) of std_logic_vector(SER_W-1 downto 0);
signal iserdes_q                 : serdes_array := (others => (others => '0'));
signal icascade1                 : std_logic_vector(SYS_W-1 downto 0);
signal icascade2                 : std_logic_vector(SYS_W-1 downto 0);

type idelay_array is array (0 to SYS_W-1) of std_logic_vector(4 downto 0);
signal av5_adcIdelayValue_s      : idelay_array := (others => (others => '0'));

begin

----------------------------------------------------------
-- Write input idelay value in the corresponding
-- array index specify by iv_adcIdelayValue_we_p mask
----------------------------------------------------------
idelay_generate: for i in 0 to SYS_W-1 generate
  process(i_clk_cmd_p)
  begin
    if rising_edge(i_clk_cmd_p) then
      if iv_adcIdelayValue_we_p(i) = '1' then
        av5_adcIdelayValue_s(i) <= iv5_adcIdelayValue_p;
      end if;

      -- Synchronous reset
      if i_adcIdelayValue_rst_p = '1' then
        av5_adcIdelayValue_s(i) <= (others => '0');
      end if;
    end if;
  end process;
end generate;

----------------------------------------------------------
-- Pulse to pulse clock domain crossing
----------------------------------------------------------
pulse2pulse_io_reset : entity work.pulse2pulse
port map (
  in_clk   => i_clk_cmd_p,
  out_clk  => clk_div,
  rst      => i_rst_cmd_p,
  pulsein  => i_io_reset_p,
  inbusy   => open,
  pulseout => io_reset_int
);

bitslip_generate: for i in 0 to SYS_W-1 generate
  pulse2pulse_bitslip : entity work.pulse2pulse
  port map (
    in_clk   => i_clk_cmd_p,
    out_clk  => clk_div,
    rst      => i_rst_cmd_p,
    pulsein  => iv_bitslip_p(i),
    inbusy   => open,
    pulseout => bitslip_int(i)
  );
end generate;

----------------------------------------------------------
-- Generate serial clock and divided serial clock
----------------------------------------------------------
serdes_clocks_inst : entity work.serdes_clocks
port map (
  CLK_IN1_P => clk_in_p,
  CLK_IN1_N => clk_in_n,
  CLK_OUT1  => clk_in,
  CLK_OUT2  => clk_div,
  RESET     => i_clk_reset_p,
  LOCKED    => open
);

clk_div_out <= clk_div; -- this is regional clock;

-- We have multiple bits - step over every bit, instantiating the required elements
pins: for pin_count in 0 to SYS_W-1 generate

  clk_in_int_buf(pin_count) <= clk_in;
  clk_in_int_inv(pin_count) <= not clk_in_int_buf(pin_count);

  -- Instantiate a buffer for every bit of the data bus
  ibufds_inst : IBUFDS
  generic map (
    DIFF_TERM  => TRUE,             -- Differential termination
    IOSTANDARD => "LVDS_25"
  )
  port map (
    i          => data_in_from_pins_p(pin_count),
    ib         => data_in_from_pins_n(pin_count),
    o          => data_in_from_pins_int(pin_count)
  );

  iodelay_bus : IODELAYE1
    generic map (
      IDELAY_TYPE  => "VAR_LOADABLE",
      IDELAY_VALUE => 0,
      DELAY_SRC    => "I"
    )
    port map (
      dataout => data_in_from_pins_delay(pin_count),
      idatain => data_in_from_pins_int(pin_count),

      c => clk_div,
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

  ----------------------------------------------------------------------------------------------------
  -- Instantiate the serdes primitive, single serdes for 4x deserialisation
  ----------------------------------------------------------------------------------------------------
  serdes_w4: if SER_W = 4 generate
    iserdese1_inst : iserdese1
    generic map (
      DATA_RATE         => "DDR",
      DATA_WIDTH        => 4,
      INTERFACE_TYPE    => "NETWORKING",
      DYN_CLKDIV_INV_EN => FALSE,
      DYN_CLK_INV_EN    => FALSE,
      NUM_CE            => 2,

      OFB_USED          => FALSE,
      IOBDELAY          => "IFD",                              -- Use input at DDLY to output the data on Q1-Q6
      SERDES_MODE       => "MASTER"
    )
    port map (
      Q1                => open,
      Q2                => open,
      Q3                => iserdes_q(pin_count)(3),
      Q4                => iserdes_q(pin_count)(2),
      Q5                => iserdes_q(pin_count)(1),
      Q6                => iserdes_q(pin_count)(0),
      SHIFTOUT1         => open,
      SHIFTOUT2         => open,
      bitslip           => bitslip_int(pin_count),             -- 1-bit Invoke iv_bitslip_p. This can be used with any
                                                               -- DATA_WIDTH, cascaded or not.
      CE1               => '1',                                -- 1-bit Clock enable input
      CE2               => '1',                                -- 1-bit Clock enable input
      CLK               => clk_in_int_buf(pin_count),          -- Fast Source Synchronous SERDES clock from BUFIO
      CLKB              => clk_in_int_inv(pin_count),          -- Locally inverted clock
      CLKDIV            => clk_div,                            -- Slow clock driven by BUFR
      D                 => '0',
      DDLY              => data_in_from_pins_delay(pin_count), -- 1-bit Input signal from IODELAYE1.
      RST               => io_reset_int,                       -- 1-bit Asynchronous reset only.
      SHIFTIN1          => '0',
      SHIFTIN2          => '0',
      -- unused connections
      DYNCLKDIVSEL      => '0',
      DYNCLKSEL         => '0',
      OFB               => '0',
      OCLK              => '0',
      O                 => open                                -- unregistered output of ISERDESE1
    );
  end generate;

  ----------------------------------------------------------------------------------------------------
  -- Instantiate the serdes primitive, master/slave serdes for 8x deserialisation
  ----------------------------------------------------------------------------------------------------
  serdes_w8: if SER_W = 8 generate
    iserdese1_master : iserdese1
    generic map (
      DATA_RATE         => "DDR",
      DATA_WIDTH        => 8,
      INTERFACE_TYPE    => "NETWORKING",
      DYN_CLKDIV_INV_EN => FALSE,
      DYN_CLK_INV_EN    => FALSE,
      NUM_CE            => 2,

      OFB_USED          => FALSE,
      IOBDELAY          => "IFD",                              -- Use input at DDLY to output the data on Q1-Q6
      SERDES_MODE       => "MASTER"
    )
    port map (
      Q1                => iserdes_q(pin_count)(7),
      Q2                => iserdes_q(pin_count)(6),
      Q3                => iserdes_q(pin_count)(5),
      Q4                => iserdes_q(pin_count)(4),
      Q5                => iserdes_q(pin_count)(3),
      Q6                => iserdes_q(pin_count)(2),
      SHIFTOUT1         => icascade1(pin_count),               -- Cascade connection to Slave ISERDES
      SHIFTOUT2         => icascade2(pin_count),               -- Cascade connection to Slave ISERDES
      bitslip           => bitslip_int(pin_count),             -- 1-bit Invoke iv_bitslip_p. This can be used with any
                                                               -- DATA_WIDTH, cascaded or not.
      CE1               => '1',                                -- 1-bit Clock enable input
      CE2               => '1',                                -- 1-bit Clock enable input
      CLK               => clk_in_int_buf(pin_count),          -- Fast Source Synchronous SERDES clock from BUFIO
      CLKB              => clk_in_int_inv(pin_count),          -- Locally inverted clock
      CLKDIV            => clk_div,                            -- Slow clock driven by BUFR
      D                 => '0',
      DDLY              => data_in_from_pins_delay(pin_count), -- 1-bit Input signal from IODELAYE1.
      RST               => io_reset_int,                       -- 1-bit Asynchronous reset only.
      SHIFTIN1          => '0',
      SHIFTIN2          => '0',
      -- unused connections
      DYNCLKDIVSEL      => '0',
      DYNCLKSEL         => '0',
      OFB               => '0',
      OCLK              => '0',
      O                 => open                                -- unregistered output of ISERDESE1
    );

    iserdese1_slave : iserdese1
    generic map (
      DATA_RATE         => "DDR",
      DATA_WIDTH        => 8,
      INTERFACE_TYPE    => "NETWORKING",
      DYN_CLKDIV_INV_EN => FALSE,
      DYN_CLK_INV_EN    => FALSE,
      NUM_CE            => 2,

      OFB_USED          => FALSE,
      IOBDELAY          => "IFD",                              -- Use input at DDLY to output the data on Q1-Q6
      SERDES_MODE       => "SLAVE"
    )
    port map (
      Q1                => open,
      Q2                => open,
      Q3                => iserdes_q(pin_count)(1),
      Q4                => iserdes_q(pin_count)(0),
      Q5                => open,--iserdes_q(pin_count)(8),
      Q6                => open,--iserdes_q(pin_count)(9),
      SHIFTOUT1         => open,
      SHIFTOUT2         => open,
      SHIFTIN1          => icascade1(pin_count),               -- Cascade connections from Master ISERDES
      SHIFTIN2          => icascade2(pin_count),               -- Cascade connections from Master ISERDES
      bitslip           => bitslip_int(pin_count),             -- 1-bit Invoke iv_bitslip_p. This can be used with any
                                                               -- DATA_WIDTH, cascaded or not.
      CE1               => '1',                                -- 1-bit Clock enable input
      CE2               => '1',                                -- 1-bit Clock enable input
      CLK               => clk_in_int_buf(pin_count),          -- Fast source synchronous serdes clock
      CLKB              => clk_in_int_inv(pin_count),          -- locally inverted clock
      CLKDIV            => clk_div,                            -- Slow clock sriven by BUFR.
      D                 => '0',                                -- Slave ISERDES module. No need to connect D, DDLY
      DDLY              => '0',
      RST               => io_reset_int,                       -- 1-bit Asynchronous reset only.
      -- unused connections
      DYNCLKDIVSEL      => '0',
      DYNCLKSEL         => '0',
      OFB               => '0',
      OCLK              => '0',
      O                 => open                                -- unregistered output of ISERDESE1
    );
  end generate;

end generate pins;

----------------------------------------------------------------------------------------------------
-- Reorder bits to samples
----------------------------------------------------------------------------------------------------
sys_bits: for p in 0 to SYS_W-1 generate --loop per pin

  ser_bits: for b in 0 to SER_W-1 generate --loop per bit

    data_in_to_device(b*SYS_W+p) <= iserdes_q(p)(b);

  end generate ser_bits;

end generate sys_bits;

----------------------------------------------------------------------------------------------------
---- IDELAYCTRL is needed for calibration
----------------------------------------------------------------------------------------------------
delayctrl_inst : IDELAYCTRL
   port map (
    RDY    => open,
    REFCLK => REF_CLOCK,
    RST    => i_adcIdelayValue_rst_p
    );
----------------------------------------------------------------------------------------------------
-- End
----------------------------------------------------------------------------------------------------
end serdes_v6_arch;



