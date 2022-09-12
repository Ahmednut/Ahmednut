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
-- File : lyt_rtdex_sync.vhd
--------------------------------------------------------------------------------
-- Description : RTDEx Sync top level
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;
  use ieee.std_logic_arith.all;
  use ieee.numeric_std.all; 
  use ieee.std_logic_unsigned.all;    
    
library lyt_rtdex_sync_v1_00_a;
  use lyt_rtdex_sync_v1_00_a.all; 

entity lyt_rtdex_sync is
  generic (
    C_CHANNEL_CFG_0                     : integer range 0 to 2 := 0;
    C_CHANNEL_CFG_1                     : integer range 0 to 2 := 0;
    C_CHANNEL_CFG_2                     : integer range 0 to 2 := 0;
    C_CHANNEL_CFG_3                     : integer range 0 to 2 := 0;
    C_CHANNEL_CFG_4                     : integer range 0 to 2 := 0;
    C_CHANNEL_CFG_5                     : integer range 0 to 2 := 0;
    C_CHANNEL_CFG_6                     : integer range 0 to 2 := 0;
    C_CHANNEL_CFG_7                     : integer range 0 to 2 := 0;
    C_CORE_ID                           : std_logic_vector(15 downto 0):=x"CC02";
    C_VERSION_NB                        : std_logic_vector(15 downto 0):=x"0200";
    C_DOWNLINK_FIFO_DEPTH               : integer := 65536;
    C_UPLINK_FIFO_DEPTH                 : integer := 8192
  );
  port (
    -- Clocking and reset
    --
    i_RTDExSyncCoreClk_p                : in std_logic;                         -- RTDExSync core clock
    i_userClk_p                         : in std_logic;                         -- User clock
    i_CoreReset_p                       : in std_logic;                         -- Active high core reset
    ov32_CoreIdVers_p                   : out std_logic_vector(31 downto 0);    -- RTDExSync core ID and version
    
    -- External triggers
    -- 
    iv4_ExtTrigger_p                    : in std_logic_vector(3 downto 0);      -- External triggers

    -- Indirect addressing interface
    --
    i_IndWrEn_p                         : in  std_logic;                        -- Indirect addressing write enable
    iv8_IndAddr_p                       : in std_logic_vector(7 downto 0);      -- Indirect addressing address
    iv32_IndWrReg_p                     : in std_logic_vector(31 downto 0);     -- Indirect addressing write register
    ov32_IndRdReg_p                     : out std_logic_vector(31 downto 0);    -- Indirect addressing read register

    -- System status
    --
    i_PllLocked_p                       : in  std_logic;                        -- FMC Radio status
    i_GpsLocked_p                       : in  std_logic;                        -- PPS sync lock status
    
    -- FPGA time
    --
    iv64_FpgaTime_p                     : in std_logic_vector(63 downto 0);     -- Free-running timestamp

    -- RTDEx Rx interface
    --
    i_RTDExRxReadyCh0_p                 : in std_logic;                         -- RTDEx receiver has valid data to read
    i_RTDExRxReadyCh1_p                 : in std_logic;                         -- RTDEx receiver has valid data to read
    i_RTDExRxReadyCh2_p                 : in std_logic;                         -- RTDEx receiver has valid data to read
    i_RTDExRxReadyCh3_p                 : in std_logic;                         -- RTDEx receiver has valid data to read
    i_RTDExRxReadyCh4_p                 : in std_logic;                         -- RTDEx receiver has valid data to read
    i_RTDExRxReadyCh5_p                 : in std_logic;                         -- RTDEx receiver has valid data to read
    i_RTDExRxReadyCh6_p                 : in std_logic;                         -- RTDEx receiver has valid data to read
    i_RTDExRxReadyCh7_p                 : in std_logic;                         -- RTDEx receiver has valid data to read
    o_RTDExRxReadReqCh0_p               : out std_logic;                        -- RTDEx receiver reads FIFO request
    o_RTDExRxReadReqCh1_p               : out std_logic;                        -- RTDEx receiver reads FIFO request
    o_RTDExRxReadReqCh2_p               : out std_logic;                        -- RTDEx receiver reads FIFO request
    o_RTDExRxReadReqCh3_p               : out std_logic;                        -- RTDEx receiver reads FIFO request
    o_RTDExRxReadReqCh4_p               : out std_logic;                        -- RTDEx receiver reads FIFO request
    o_RTDExRxReadReqCh5_p               : out std_logic;                        -- RTDEx receiver reads FIFO request
    o_RTDExRxReadReqCh6_p               : out std_logic;                        -- RTDEx receiver reads FIFO request
    o_RTDExRxReadReqCh7_p               : out std_logic;                        -- RTDEx receiver reads FIFO request
    i_RTDExRxDataValidCh0_p             : in std_logic;                         -- Data at RTDEx receiver data bus is valid
    i_RTDExRxDataValidCh1_p             : in std_logic;                         -- Data at RTDEx receiver data bus is valid
    i_RTDExRxDataValidCh2_p             : in std_logic;                         -- Data at RTDEx receiver data bus is valid
    i_RTDExRxDataValidCh3_p             : in std_logic;                         -- Data at RTDEx receiver data bus is valid
    i_RTDExRxDataValidCh4_p             : in std_logic;                         -- Data at RTDEx receiver data bus is valid
    i_RTDExRxDataValidCh5_p             : in std_logic;                         -- Data at RTDEx receiver data bus is valid
    i_RTDExRxDataValidCh6_p             : in std_logic;                         -- Data at RTDEx receiver data bus is valid
    i_RTDExRxDataValidCh7_p             : in std_logic;                         -- Data at RTDEx receiver data bus is valid
    iv32_RTDExRxDataCh0_p               : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
    iv32_RTDExRxDataCh1_p               : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
    iv32_RTDExRxDataCh2_p               : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
    iv32_RTDExRxDataCh3_p               : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
    iv32_RTDExRxDataCh4_p               : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
    iv32_RTDExRxDataCh5_p               : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
    iv32_RTDExRxDataCh6_p               : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
    iv32_RTDExRxDataCh7_p               : in std_logic_vector(31 downto 0);     -- RTDEx receiver output data bus
               
    -- RTDEx Tx interface
    --
    i_RTDExTxReadyCh0_p                 : in std_logic;                         -- RTDEx transmitter ready
    i_RTDExTxReadyCh1_p                 : in std_logic;                         -- RTDEx transmitter ready
    i_RTDExTxReadyCh2_p                 : in std_logic;                         -- RTDEx transmitter ready
    i_RTDExTxReadyCh3_p                 : in std_logic;                         -- RTDEx transmitter ready
    i_RTDExTxReadyCh4_p                 : in std_logic;                         -- RTDEx transmitter ready
    i_RTDExTxReadyCh5_p                 : in std_logic;                         -- RTDEx transmitter ready
    i_RTDExTxReadyCh6_p                 : in std_logic;                         -- RTDEx transmitter ready
    i_RTDExTxReadyCh7_p                 : in std_logic;                         -- RTDEx transmitter ready
    o_RTDExTxWriteReqCh0_p              : out std_logic;                        -- RTDEx transmitter writes FIFO request
    o_RTDExTxWriteReqCh1_p              : out std_logic;                        -- RTDEx transmitter writes FIFO request
    o_RTDExTxWriteReqCh2_p              : out std_logic;                        -- RTDEx transmitter writes FIFO request
    o_RTDExTxWriteReqCh3_p              : out std_logic;                        -- RTDEx transmitter writes FIFO request
    o_RTDExTxWriteReqCh4_p              : out std_logic;                        -- RTDEx transmitter writes FIFO request
    o_RTDExTxWriteReqCh5_p              : out std_logic;                        -- RTDEx transmitter writes FIFO request
    o_RTDExTxWriteReqCh6_p              : out std_logic;                        -- RTDEx transmitter writes FIFO request
    o_RTDExTxWriteReqCh7_p              : out std_logic;                        -- RTDEx transmitter writes FIFO request
    ov32_RTDExTxDataCh0_p               : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    ov32_RTDExTxDataCh1_p               : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    ov32_RTDExTxDataCh2_p               : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    ov32_RTDExTxDataCh3_p               : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    ov32_RTDExTxDataCh4_p               : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    ov32_RTDExTxDataCh5_p               : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    ov32_RTDExTxDataCh6_p               : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    ov32_RTDExTxDataCh7_p               : out std_logic_vector(31 downto 0);    -- RTDEx transmitter data bus
    
    -- Interface compatible with RTDEx Rx user interface
    --
    o_RTDExSyncRxReadyCh0_p             : out std_logic;                        -- RTDExSync receiver has valid data to read
    o_RTDExSyncRxReadyCh1_p             : out std_logic;                        -- RTDExSync receiver has valid data to read
    o_RTDExSyncRxReadyCh2_p             : out std_logic;                        -- RTDExSync receiver has valid data to read
    o_RTDExSyncRxReadyCh3_p             : out std_logic;                        -- RTDExSync receiver has valid data to read
    o_RTDExSyncRxReadyCh4_p             : out std_logic;                        -- RTDExSync receiver has valid data to read
    o_RTDExSyncRxReadyCh5_p             : out std_logic;                        -- RTDExSync receiver has valid data to read
    o_RTDExSyncRxReadyCh6_p             : out std_logic;                        -- RTDExSync receiver has valid data to read
    o_RTDExSyncRxReadyCh7_p             : out std_logic;                        -- RTDExSync receiver has valid data to read
    i_RTDExSyncRxReReqCh0_p             : in std_logic;                         -- RTDExSync receiver reads FIFO request
    i_RTDExSyncRxReReqCh1_p             : in std_logic;                         -- RTDExSync receiver reads FIFO request
    i_RTDExSyncRxReReqCh2_p             : in std_logic;                         -- RTDExSync receiver reads FIFO requestt
    i_RTDExSyncRxReReqCh3_p             : in std_logic;                         -- RTDExSync receiver reads FIFO request
    i_RTDExSyncRxReReqCh4_p             : in std_logic;                         -- RTDExSync receiver reads FIFO request
    i_RTDExSyncRxReReqCh5_p             : in std_logic;                         -- RTDExSync receiver reads FIFO request
    i_RTDExSyncRxReReqCh6_p             : in std_logic;                         -- RTDExSync receiver reads FIFO request
    i_RTDExSyncRxReReqCh7_p             : in std_logic;                         -- RTDExSync receiver reads FIFO request
    o_RTDExSyncRxDataValidCh0_p         : out std_logic;                        -- Data at RTDExSync receiver data bus is valid
    o_RTDExSyncRxDataValidCh1_p         : out std_logic;                        -- Data at RTDExSync receiver data bus is valid
    o_RTDExSyncRxDataValidCh2_p         : out std_logic;                        -- Data at RTDExSync receiver data bus is valid
    o_RTDExSyncRxDataValidCh3_p         : out std_logic;                        -- Data at RTDExSync receiver data bus is valid
    o_RTDExSyncRxDataValidCh4_p         : out std_logic;                        -- Data at RTDExSync receiver data bus is valid
    o_RTDExSyncRxDataValidCh5_p         : out std_logic;                        -- Data at RTDExSync receiver data bus is valid
    o_RTDExSyncRxDataValidCh6_p         : out std_logic;                        -- Data at RTDExSync receiver data bus is valid
    o_RTDExSyncRxDataValidCh7_p         : out std_logic;                        -- Data at RTDExSync receiver data bus is valid
    ov32_RTDExSyncRxDataCh0_p           : out std_logic_vector(31 downto 0);    -- RTDExSync receiver output data bus
    ov32_RTDExSyncRxDataCh1_p           : out std_logic_vector(31 downto 0);    -- RTDExSync receiver output data bus
    ov32_RTDExSyncRxDataCh2_p           : out std_logic_vector(31 downto 0);    -- RTDExSync receiver output data bus
    ov32_RTDExSyncRxDataCh3_p           : out std_logic_vector(31 downto 0);    -- RTDExSync receiver output data bus
    ov32_RTDExSyncRxDataCh4_p           : out std_logic_vector(31 downto 0);    -- RTDExSync receiver output data bus
    ov32_RTDExSyncRxDataCh5_p           : out std_logic_vector(31 downto 0);    -- RTDExSync receiver output data bus
    ov32_RTDExSyncRxDataCh6_p           : out std_logic_vector(31 downto 0);    -- RTDExSync receiver output data bus
    ov32_RTDExSyncRxDataCh7_p           : out std_logic_vector(31 downto 0);    -- RTDExSync receiver output data bus

    -- Interface compatible with RTDEx Tx user interface
    --
    o_RTDExSyncTxReadyCh0_p             : out std_logic;                        -- RTDExSync transmitter ready
    o_RTDExSyncTxReadyCh1_p             : out std_logic;                        -- RTDExSync transmitter ready
    o_RTDExSyncTxReadyCh2_p             : out std_logic;                        -- RTDExSync transmitter ready
    o_RTDExSyncTxReadyCh3_p             : out std_logic;                        -- RTDExSync transmitter ready
    o_RTDExSyncTxReadyCh4_p             : out std_logic;                        -- RTDExSync transmitter ready
    o_RTDExSyncTxReadyCh5_p             : out std_logic;                        -- RTDExSync transmitter ready
    o_RTDExSyncTxReadyCh6_p             : out std_logic;                        -- RTDExSync transmitter ready
    o_RTDExSyncTxReadyCh7_p             : out std_logic;                        -- RTDExSync transmitter ready
    i_RTDExSyncTxWriteReqCh0_p          : in std_logic;                         -- RTDExSync transmitter writes FIFO request
    i_RTDExSyncTxWriteReqCh1_p          : in std_logic;                         -- RTDExSync transmitter writes FIFO request
    i_RTDExSyncTxWriteReqCh2_p          : in std_logic;                         -- RTDExSync transmitter writes FIFO request
    i_RTDExSyncTxWriteReqCh3_p          : in std_logic;                         -- RTDExSync transmitter writes FIFO request
    i_RTDExSyncTxWriteReqCh4_p          : in std_logic;                         -- RTDExSync transmitter writes FIFO request
    i_RTDExSyncTxWriteReqCh5_p          : in std_logic;                         -- RTDExSync transmitter writes FIFO request
    i_RTDExSyncTxWriteReqCh6_p          : in std_logic;                         -- RTDExSync transmitter writes FIFO request
    i_RTDExSyncTxWriteReqCh7_p          : in std_logic;                         -- RTDExSync transmitter writes FIFO request
    iv32_RTDExSyncTxDataCh0_p           : in std_logic_vector(31 downto 0);     -- RTDExSync transmitter data bus
    iv32_RTDExSyncTxDataCh1_p           : in std_logic_vector(31 downto 0);     -- RTDExSync transmitter data bus
    iv32_RTDExSyncTxDataCh2_p           : in std_logic_vector(31 downto 0);     -- RTDExSync transmitter data bus
    iv32_RTDExSyncTxDataCh3_p           : in std_logic_vector(31 downto 0);     -- RTDExSync transmitter data bus
    iv32_RTDExSyncTxDataCh4_p           : in std_logic_vector(31 downto 0);     -- RTDExSync transmitter data bus
    iv32_RTDExSyncTxDataCh5_p           : in std_logic_vector(31 downto 0);     -- RTDExSync transmitter data bus
    iv32_RTDExSyncTxDataCh6_p           : in std_logic_vector(31 downto 0);     -- RTDExSync transmitter data bus
    iv32_RTDExSyncTxDataCh7_p           : in std_logic_vector(31 downto 0)      -- RTDExSync transmitter data bus
  );

end lyt_rtdex_sync;

architecture Behavioral of lyt_rtdex_sync is

  component fifo_async_w64_d16 is
    port (
      rst       : in std_logic;
      wr_clk    : in std_logic;
      rd_clk    : in std_logic;
      din       : in std_logic_vector(63 downto 0);
      wr_en     : in std_logic;
      rd_en     : in std_logic;
      dout      : out std_logic_vector(63 downto 0);
      full      : out std_logic;
      empty     : out std_logic
    );
  end component;

  type a8_v32_t is array(7 downto 0) of std_logic_vector(31 downto 0); 
  type a8_v20_t is array(7 downto 0) of std_logic_vector(19 downto 0); 

  -- Indirect addressing interface
  --
  signal IndWrEn_s                  : std_logic;
  signal v3_IndChanIdx_s            : std_logic_vector(2 downto 0);
  signal v3_IndRegIdx_s             : std_logic_vector(2 downto 0);
  signal v32_IndWrReg_s             : std_logic_vector(31 downto 0);

  -- Indirect addressing registers
  --
  signal v8_chReset_s                       : std_logic_vector(7 downto 0);
  signal v8_chEnable_s                      : std_logic_vector(7 downto 0);
  signal v8_chPresent_s                     : std_logic_vector(7 downto 0);
  signal v8_chDirection_s                   : std_logic_vector(7 downto 0);
  signal a8v20_RTDExSyncRxFrameSize_s       : a8_v20_t;
  signal a8v20_RTDExSyncTxFrameSize_s       : a8_v20_t;
  signal a8v20_RTDExSyncTxCntxFrameSize_s   : a8_v20_t;
  signal a8v32_Status_s                     : a8_v32_t;
  signal a8v32_IndRdReg_s                   : a8_v32_t;

  -- RTDEx Rx interface
  --
  signal v8_RTDExRxReady_s              : std_logic_vector(7 downto 0);
  signal v8_RTDExRxReadReq_s            : std_logic_vector(7 downto 0);
  signal v8_RTDExRxDataValid_s          : std_logic_vector(7 downto 0);
  signal a8v32_RTDExRxData_s            : a8_v32_t;

  -- RTDEx Tx interface
  --
  signal v8_RTDExTxReady_s              : std_logic_vector(7 downto 0);
  signal v8_RTDExTxWriteReq_s           : std_logic_vector(7 downto 0);
  signal a8v32_RTDExTxData_s            : a8_v32_t;

  -- Interface compatible with RTDEx Rx user interface
  --
  signal v8_RTDExSyncRxReady_s              : std_logic_vector(7 downto 0);
  signal v8_RTDExSyncRxReReq_s              : std_logic_vector(7 downto 0);
  signal v8_RTDExSyncRxDataValid_s          : std_logic_vector(7 downto 0);
  signal a8v32_RTDExSyncRxData_s            : a8_v32_t;

  -- Interface compatible with RTDEx Tx user interface
  --
  signal v8_RTDExSyncTxReady_s              : std_logic_vector(7 downto 0);
  signal v8_RTDExSyncTxWriteReq_s           : std_logic_vector(7 downto 0);
  signal a8v32_RTDExSyncTxData_s            : a8_v32_t;

  signal core_reset_s                   : std_logic;
  signal pll_locked_s                   : std_logic;
  signal gps_locked_s                   : std_logic;

  type a8_int_t is array(7 downto 0) of integer;
  constant v8_ChannelCfg_s              : a8_int_t := (C_CHANNEL_CFG_7, C_CHANNEL_CFG_6, C_CHANNEL_CFG_5, C_CHANNEL_CFG_4, C_CHANNEL_CFG_3, C_CHANNEL_CFG_2, C_CHANNEL_CFG_1, C_CHANNEL_CFG_0);

  -- Keep constraints (timing constraints outside this code on these signals)
  --
  attribute keep                        : boolean;
  attribute keep of i_IndWrEn_p         : signal is true;
  attribute keep of iv8_IndAddr_p       : signal is true;
  attribute keep of iv32_IndWrReg_p     : signal is true;
  attribute keep of v8_chEnable_s       : signal is true;

begin

  pipeline_input:process(i_RTDExSyncCoreClk_p)
  begin
    if rising_edge(i_RTDExSyncCoreClk_p) then
      core_reset_s                      <= i_CoreReset_p;
      pll_locked_s                      <= i_PllLocked_p;
      gps_locked_s                      <= i_GpsLocked_p;
  
      IndWrEn_s                         <= i_IndWrEn_p;
      v3_IndChanIdx_s                   <= iv8_IndAddr_p(6 downto 4);
      v3_IndRegIdx_s                    <= iv8_IndAddr_p(2 downto 0);
      v32_IndWrReg_s                    <= iv32_IndWrReg_p;
    end if;
  end process;

  -- RTDEx Rx interface
  --    
  v8_RTDExRxReady_s(0)                  <= i_RTDExRxReadyCh0_p;
  v8_RTDExRxReady_s(1)                  <= i_RTDExRxReadyCh1_p;
  v8_RTDExRxReady_s(2)                  <= i_RTDExRxReadyCh2_p;
  v8_RTDExRxReady_s(3)                  <= i_RTDExRxReadyCh3_p;
  v8_RTDExRxReady_s(4)                  <= i_RTDExRxReadyCh4_p;
  v8_RTDExRxReady_s(5)                  <= i_RTDExRxReadyCh5_p;
  v8_RTDExRxReady_s(6)                  <= i_RTDExRxReadyCh6_p;
  v8_RTDExRxReady_s(7)                  <= i_RTDExRxReadyCh7_p;

  o_RTDExRxReadReqCh0_p                 <= v8_RTDExRxReadReq_s(0);
  o_RTDExRxReadReqCh1_p                 <= v8_RTDExRxReadReq_s(1);
  o_RTDExRxReadReqCh2_p                 <= v8_RTDExRxReadReq_s(2);
  o_RTDExRxReadReqCh3_p                 <= v8_RTDExRxReadReq_s(3);
  o_RTDExRxReadReqCh4_p                 <= v8_RTDExRxReadReq_s(4);
  o_RTDExRxReadReqCh5_p                 <= v8_RTDExRxReadReq_s(5);
  o_RTDExRxReadReqCh6_p                 <= v8_RTDExRxReadReq_s(6);
  o_RTDExRxReadReqCh7_p                 <= v8_RTDExRxReadReq_s(7);
  
  v8_RTDExRxDataValid_s(0)              <= i_RTDExRxDataValidCh0_p;
  v8_RTDExRxDataValid_s(1)              <= i_RTDExRxDataValidCh1_p;
  v8_RTDExRxDataValid_s(2)              <= i_RTDExRxDataValidCh2_p;
  v8_RTDExRxDataValid_s(3)              <= i_RTDExRxDataValidCh3_p;
  v8_RTDExRxDataValid_s(4)              <= i_RTDExRxDataValidCh4_p;
  v8_RTDExRxDataValid_s(5)              <= i_RTDExRxDataValidCh5_p;
  v8_RTDExRxDataValid_s(6)              <= i_RTDExRxDataValidCh6_p;
  v8_RTDExRxDataValid_s(7)              <= i_RTDExRxDataValidCh7_p;

  a8v32_RTDExRxData_s(0)                <= iv32_RTDExRxDataCh0_p;
  a8v32_RTDExRxData_s(1)                <= iv32_RTDExRxDataCh1_p;
  a8v32_RTDExRxData_s(2)                <= iv32_RTDExRxDataCh2_p;
  a8v32_RTDExRxData_s(3)                <= iv32_RTDExRxDataCh3_p;
  a8v32_RTDExRxData_s(4)                <= iv32_RTDExRxDataCh4_p;
  a8v32_RTDExRxData_s(5)                <= iv32_RTDExRxDataCh5_p;
  a8v32_RTDExRxData_s(6)                <= iv32_RTDExRxDataCh6_p;
  a8v32_RTDExRxData_s(7)                <= iv32_RTDExRxDataCh7_p;

  -- RTDEx Tx interface
  --
  v8_RTDExTxReady_s(0)                  <= i_RTDExTxReadyCh0_p;
  v8_RTDExTxReady_s(1)                  <= i_RTDExTxReadyCh1_p;
  v8_RTDExTxReady_s(2)                  <= i_RTDExTxReadyCh2_p;
  v8_RTDExTxReady_s(3)                  <= i_RTDExTxReadyCh3_p;
  v8_RTDExTxReady_s(4)                  <= i_RTDExTxReadyCh4_p;
  v8_RTDExTxReady_s(5)                  <= i_RTDExTxReadyCh5_p;
  v8_RTDExTxReady_s(6)                  <= i_RTDExTxReadyCh6_p;
  v8_RTDExTxReady_s(7)                  <= i_RTDExTxReadyCh7_p;

  o_RTDExTxWriteReqCh0_p                <= v8_RTDExTxWriteReq_s(0);
  o_RTDExTxWriteReqCh1_p                <= v8_RTDExTxWriteReq_s(1);
  o_RTDExTxWriteReqCh2_p                <= v8_RTDExTxWriteReq_s(2);
  o_RTDExTxWriteReqCh3_p                <= v8_RTDExTxWriteReq_s(3);
  o_RTDExTxWriteReqCh4_p                <= v8_RTDExTxWriteReq_s(4);
  o_RTDExTxWriteReqCh5_p                <= v8_RTDExTxWriteReq_s(5);
  o_RTDExTxWriteReqCh6_p                <= v8_RTDExTxWriteReq_s(6);
  o_RTDExTxWriteReqCh7_p                <= v8_RTDExTxWriteReq_s(7);

  ov32_RTDExTxDataCh0_p                 <= a8v32_RTDExTxData_s(0);
  ov32_RTDExTxDataCh1_p                 <= a8v32_RTDExTxData_s(1);
  ov32_RTDExTxDataCh2_p                 <= a8v32_RTDExTxData_s(2);
  ov32_RTDExTxDataCh3_p                 <= a8v32_RTDExTxData_s(3);
  ov32_RTDExTxDataCh4_p                 <= a8v32_RTDExTxData_s(4);
  ov32_RTDExTxDataCh5_p                 <= a8v32_RTDExTxData_s(5);
  ov32_RTDExTxDataCh6_p                 <= a8v32_RTDExTxData_s(6);
  ov32_RTDExTxDataCh7_p                 <= a8v32_RTDExTxData_s(7);


  -- Interface compatible with RTDEx Rx user interface
  --
  v8_RTDExSyncRxReReq_s(0)                  <= i_RTDExSyncRxReReqCh0_p;
  v8_RTDExSyncRxReReq_s(1)                  <= i_RTDExSyncRxReReqCh1_p;
  v8_RTDExSyncRxReReq_s(2)                  <= i_RTDExSyncRxReReqCh2_p;
  v8_RTDExSyncRxReReq_s(3)                  <= i_RTDExSyncRxReReqCh3_p;
  v8_RTDExSyncRxReReq_s(4)                  <= i_RTDExSyncRxReReqCh4_p;
  v8_RTDExSyncRxReReq_s(5)                  <= i_RTDExSyncRxReReqCh5_p;
  v8_RTDExSyncRxReReq_s(6)                  <= i_RTDExSyncRxReReqCh6_p;
  v8_RTDExSyncRxReReq_s(7)                  <= i_RTDExSyncRxReReqCh7_p;
  
  o_RTDExSyncRxReadyCh0_p                   <= v8_RTDExSyncRxReady_s(0);
  o_RTDExSyncRxReadyCh1_p                   <= v8_RTDExSyncRxReady_s(1);
  o_RTDExSyncRxReadyCh2_p                   <= v8_RTDExSyncRxReady_s(2);
  o_RTDExSyncRxReadyCh3_p                   <= v8_RTDExSyncRxReady_s(3);
  o_RTDExSyncRxReadyCh4_p                   <= v8_RTDExSyncRxReady_s(4);
  o_RTDExSyncRxReadyCh5_p                   <= v8_RTDExSyncRxReady_s(5);
  o_RTDExSyncRxReadyCh6_p                   <= v8_RTDExSyncRxReady_s(6);
  o_RTDExSyncRxReadyCh7_p                   <= v8_RTDExSyncRxReady_s(7);

  o_RTDExSyncRxDataValidCh0_p               <= v8_RTDExSyncRxDataValid_s(0);
  o_RTDExSyncRxDataValidCh1_p               <= v8_RTDExSyncRxDataValid_s(1);
  o_RTDExSyncRxDataValidCh2_p               <= v8_RTDExSyncRxDataValid_s(2);
  o_RTDExSyncRxDataValidCh3_p               <= v8_RTDExSyncRxDataValid_s(3);
  o_RTDExSyncRxDataValidCh4_p               <= v8_RTDExSyncRxDataValid_s(4);
  o_RTDExSyncRxDataValidCh5_p               <= v8_RTDExSyncRxDataValid_s(5);
  o_RTDExSyncRxDataValidCh6_p               <= v8_RTDExSyncRxDataValid_s(6);
  o_RTDExSyncRxDataValidCh7_p               <= v8_RTDExSyncRxDataValid_s(7);

  ov32_RTDExSyncRxDataCh0_p                 <= a8v32_RTDExSyncRxData_s(0);
  ov32_RTDExSyncRxDataCh1_p                 <= a8v32_RTDExSyncRxData_s(1);
  ov32_RTDExSyncRxDataCh2_p                 <= a8v32_RTDExSyncRxData_s(2);
  ov32_RTDExSyncRxDataCh3_p                 <= a8v32_RTDExSyncRxData_s(3);
  ov32_RTDExSyncRxDataCh4_p                 <= a8v32_RTDExSyncRxData_s(4);
  ov32_RTDExSyncRxDataCh5_p                 <= a8v32_RTDExSyncRxData_s(5);
  ov32_RTDExSyncRxDataCh6_p                 <= a8v32_RTDExSyncRxData_s(6);
  ov32_RTDExSyncRxDataCh7_p                 <= a8v32_RTDExSyncRxData_s(7);

  -- Interface compatible with RTDEx Tx user interface
  --
  o_RTDExSyncTxReadyCh0_p                   <= v8_RTDExSyncTxReady_s(0);
  o_RTDExSyncTxReadyCh1_p                   <= v8_RTDExSyncTxReady_s(1);
  o_RTDExSyncTxReadyCh2_p                   <= v8_RTDExSyncTxReady_s(2);
  o_RTDExSyncTxReadyCh3_p                   <= v8_RTDExSyncTxReady_s(3);
  o_RTDExSyncTxReadyCh4_p                   <= v8_RTDExSyncTxReady_s(4);
  o_RTDExSyncTxReadyCh5_p                   <= v8_RTDExSyncTxReady_s(5);
  o_RTDExSyncTxReadyCh6_p                   <= v8_RTDExSyncTxReady_s(6);
  o_RTDExSyncTxReadyCh7_p                   <= v8_RTDExSyncTxReady_s(7);
    
  v8_RTDExSyncTxWriteReq_s(0)               <= i_RTDExSyncTxWriteReqCh0_p;
  v8_RTDExSyncTxWriteReq_s(1)               <= i_RTDExSyncTxWriteReqCh1_p;
  v8_RTDExSyncTxWriteReq_s(2)               <= i_RTDExSyncTxWriteReqCh2_p;
  v8_RTDExSyncTxWriteReq_s(3)               <= i_RTDExSyncTxWriteReqCh3_p;
  v8_RTDExSyncTxWriteReq_s(4)               <= i_RTDExSyncTxWriteReqCh4_p;
  v8_RTDExSyncTxWriteReq_s(5)               <= i_RTDExSyncTxWriteReqCh5_p;
  v8_RTDExSyncTxWriteReq_s(6)               <= i_RTDExSyncTxWriteReqCh6_p;
  v8_RTDExSyncTxWriteReq_s(7)               <= i_RTDExSyncTxWriteReqCh7_p;
    
  a8v32_RTDExSyncTxData_s(0)                <= iv32_RTDExSyncTxDataCh0_p;
  a8v32_RTDExSyncTxData_s(1)                <= iv32_RTDExSyncTxDataCh1_p;
  a8v32_RTDExSyncTxData_s(2)                <= iv32_RTDExSyncTxDataCh2_p;
  a8v32_RTDExSyncTxData_s(3)                <= iv32_RTDExSyncTxDataCh3_p;
  a8v32_RTDExSyncTxData_s(4)                <= iv32_RTDExSyncTxDataCh4_p;
  a8v32_RTDExSyncTxData_s(5)                <= iv32_RTDExSyncTxDataCh5_p;
  a8v32_RTDExSyncTxData_s(6)                <= iv32_RTDExSyncTxDataCh6_p;
  a8v32_RTDExSyncTxData_s(7)                <= iv32_RTDExSyncTxDataCh7_p;


  ov32_CoreIdVers_p                     <= C_CORE_ID & C_VERSION_NB;


  ------------------------------------------------------------
  -- Generate the present and direction bits
  ------------------------------------------------------------
    
  genPresentDirBits_l: for i in 0 to 7 generate

    genPresentDirBitsCfg0_l: if (v8_ChannelCfg_s(i) = 0) generate
      v8_chPresent_s(i)       <= '0';         -- Present bit
      v8_chDirection_s(i)     <= '0';         -- Direction bit
    end generate genPresentDirBitsCfg0_l;

    genPresentDirBitsCfg1_l: if (v8_ChannelCfg_s(i) = 1) generate
      v8_chPresent_s(i)       <= '1';         -- Present bit
      v8_chDirection_s(i)     <= '0';         -- Direction bit
    end generate genPresentDirBitsCfg1_l;
    
    genPresentDirBitsCfg2_l: if (v8_ChannelCfg_s(i) = 2) generate
      v8_chPresent_s(i)       <= '1';         -- Present bit
      v8_chDirection_s(i)     <= '1';         -- Direction bit
    end generate genPresentDirBitsCfg2_l;
   
  end generate genPresentDirBits_l;


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

  -- Write interface
  --
  gen_IndessingWrite_l: for i in 0 to 7 generate
    IndessingWrite_l : process(i_RTDExSyncCoreClk_p)
    begin
      if rising_edge(i_RTDExSyncCoreClk_p) then

        if (IndWrEn_s = '1' and v3_IndChanIdx_s = std_logic_vector(to_unsigned(i, 3))) then    
          
          case v3_IndRegIdx_s is
          
            when "000" =>
              
              v8_chReset_s(i)                 <= v32_IndWrReg_s(0);
              v8_chEnable_s(i)                <= v32_IndWrReg_s(1);
              
            when "001" =>

              -- The frame size specified in this register are in bytes, VITA49
              -- works in 32-bit words. No validation of the 2 LSB.
              --
              a8v20_RTDExSyncRxFrameSize_s(i) <= v32_IndWrReg_s(21 downto 2);
  
            when "010" =>
            
              a8v20_RTDExSyncTxFrameSize_s(i) <= v32_IndWrReg_s(21 downto 2);
  
            when "100" =>
            
              a8v20_RTDExSyncTxCntxFrameSize_s(i) <= v32_IndWrReg_s(21 downto 2);
              
            when others =>
              
              -- Read only or reserved registers
              --
              null;
          
          end case;
        end if;
        
        -- Synchronous core reset
        if core_reset_s = '1' then
            v8_chReset_s(i)                     <= '0';
            v8_chEnable_s(i)                    <= '0';
            a8v20_RTDExSyncRxFrameSize_s(i)     <= (others => '0');
            a8v20_RTDExSyncTxFrameSize_s(i)     <= (others => '0');
            a8v20_RTDExSyncTxCntxFrameSize_s(i) <= (others => '0');
        end if;
        
      end if;
    end process IndessingWrite_l;
  end generate gen_IndessingWrite_l;

  -- Read interface : Select register
  --
  gen_IndessingRead_l: for i in 0 to 7 generate
    IndessingRead_l : process(v3_IndRegIdx_s, v8_chReset_s, v8_chEnable_s, v8_chDirection_s, v8_chPresent_s, a8v20_RTDExSyncRxFrameSize_s, 
                                  a8v20_RTDExSyncTxFrameSize_s, a8v32_Status_s, a8v20_RTDExSyncTxCntxFrameSize_s)
    
    begin
      
      case v3_IndRegIdx_s is
      
        when "000" =>

          a8v32_IndRdReg_s(i)   <= X"0000000" & v8_chPresent_s(i) & v8_chDirection_s(i) & v8_chEnable_s(i) & v8_chReset_s(i);

        when "001" =>

          a8v32_IndRdReg_s(i)   <= "0000000000" & a8v20_RTDExSyncRxFrameSize_s(i) & "00";

        when "010" =>
        
          a8v32_IndRdReg_s(i)   <= "0000000000" & a8v20_RTDExSyncTxFrameSize_s(i) & "00";

        when "011" =>

          a8v32_IndRdReg_s(i)   <= a8v32_Status_s(i);

        when "100" =>
        
          a8v32_IndRdReg_s(i)   <= "0000000000" & a8v20_RTDExSyncTxCntxFrameSize_s(i) & "00";
          
        when others =>
        
          a8v32_IndRdReg_s(i)   <= (others => '0');
      
      end case;

    end process IndessingRead_l;
    
  end generate gen_IndessingRead_l;


  -- Read interface : Select channel
  --
  --   (add one cycle latency for possible timing issues)
  --
-- DQ_DEBUG an other solution is to remove the FF and make it only a
--          combinatorial path. Add tig constraints then.
--
  vita49_status:process(i_RTDExSyncCoreClk_p)
  begin
    if rising_edge(i_RTDExSyncCoreClk_p) then
      case v3_IndChanIdx_s is
        when "000" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(0);
        when "001" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(1);
        when "010" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(2);
        when "011" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(3);
        when "100" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(4);
        when "101" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(5);
        when "110" =>    
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(6);
        when "111" =>
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(7);
        when others => 
            ov32_IndRdReg_p   <= a8v32_IndRdReg_s(0);
      end case;
    end if;
  end process;
    

  ------------------------------------------------------------
  -- RTDEx Sync downlink module instantiations
  ------------------------------------------------------------

  genDownLink_l: for i in 0 to 7 generate
    genRtdexSyncRx: if (v8_ChannelCfg_s(i) = 2) generate
      rtdexsync_rx: entity lyt_rtdex_sync_v1_00_a.lyt_rtdex_sync_rx
        generic map (      
          C_DOWNLINK_FIFO_DEPTH         => C_DOWNLINK_FIFO_DEPTH,
          C_DOWNLINK_FIFO_FULL_THRES    => C_DOWNLINK_FIFO_DEPTH - 16,
          C_DOWNLINK_FIFO_EMPTY_THRES   => 4
        )
        port map (
          i_userClk_p                   => i_userClk_p,
          i_RTDExSyncCoreClk_p          => i_RTDExSyncCoreClk_p,
          i_CoreReset_p                 => v8_chReset_s(i) or core_reset_s,
          i_CoreEnable_p                => v8_chEnable_s(i),
          
          -- External triggers
          -- 
          iv4_ExtTrigger_p              => iv4_ExtTrigger_p,
         
          -- FMC Radio status
          --
          i_PllLocked_p                 => pll_locked_s,
          
          -- PPS sync lock status
          --
          i_GpsLocked_p                 => gps_locked_s,
          
          -- FPGA time stamping
          --         
          iv64_FpgaTime_p               => iv64_FpgaTime_p,
          
          -- Decoder Regs CTRL & Status via AXI
          --
          ov32_decStatus_p                  => a8v32_Status_s(i),
          iv20_RTDExSyncRxFrameSize_p       => a8v20_RTDExSyncRxFrameSize_s(i),
          iv20_RTDExSyncTxFrameSize_p       => a8v20_RTDExSyncTxFrameSize_s(i),
          iv20_RTDExSyncTxCntxFrameSize_p   => a8v20_RTDExSyncTxCntxFrameSize_s(i),

          -- RTDEx Rx interface
          --
          i_RTDExRxReady_p              => v8_RTDExRxReady_s(i),
          o_RTDExRxReadReq_p            => v8_RTDExRxReadReq_s(i),
          iv32_RTDExRxData_p            => a8v32_RTDExRxData_s(i),
          i_RTDExRxDataValid_p          => v8_RTDExRxDataValid_s(i),
          
          -- RTDEx Tx interface
          --
          i_RTDExTxReady_p              => v8_RTDExTxReady_s(i),
          o_RTDExTxWriteReq_p           => v8_RTDExTxWriteReq_s(i),
          ov32_RTDExTxData_p            => a8v32_RTDExTxData_s(i),
          
          -- Output interface compatible with RTDEx Rx user interface
          --
          o_decReady_p                  => v8_RTDExSyncRxReady_s(i),
          i_decReReq_p                  => v8_RTDExSyncRxReReq_s(i),
          ov32_decData_p                => a8v32_RTDExSyncRxData_s(i),
          o_decDataValid_p              => v8_RTDExSyncRxDataValid_s(i)
        );
    end generate genRtdexSyncRx;
  end generate genDownLink_l;


  ------------------------------------------------------------
  -- RTDEx Sync downlink module instantiations
  ------------------------------------------------------------

  genUpLink_l: for i in 0 to 7 generate
    genRtdexSyncTx: if (v8_ChannelCfg_s(i) = 1) generate
      rtdexsync_tx: entity lyt_rtdex_sync_v1_00_a.lyt_rtdex_sync_tx
        generic map (
          C_UPLINK_FIFO_DEPTH    	    => C_UPLINK_FIFO_DEPTH,
          C_UPLINK_FIFO_FULL_THRES      => C_UPLINK_FIFO_DEPTH - 16,
          C_UPLINK_FIFO_EMPTY_THRES	    => 4
        )
        port map (
          i_userClk_p                   => i_userClk_p,
          i_RTDExSyncCoreClk_p          => i_RTDExSyncCoreClk_p,
          i_CoreReset_p                 => v8_chReset_s(i) or core_reset_s,
          i_CoreEnable_p                => v8_chEnable_s(i),
          
          -- External triggers
          -- 
          iv4_ExtTrigger_p              => iv4_ExtTrigger_p,
          
          -- FMC Radio status
          --
          i_PllLocked_p                 => pll_locked_s,
        
          -- PPS sync lock status
          --
          i_GpsLocked_p                 => gps_locked_s,
          
          -- FPGA time stamping
          --        
          iv64_FpgaTime_p               => iv64_FpgaTime_p,          
          
          -- Encoder Regs CTRL & Status via AXI
          --
          ov32_encStatus_p                  => a8v32_Status_s(i),
          iv20_RTDExSyncRxFrameSize_p       => a8v20_RTDExSyncRxFrameSize_s(i),
          iv20_RTDExSyncTxFrameSize_p       => a8v20_RTDExSyncTxFrameSize_s(i),
          iv20_RTDExSyncTxCntxFrameSize_p   => a8v20_RTDExSyncTxCntxFrameSize_s(i),

          -- RTDEx Rx interface
          --
          i_RTDExRxReady_p              => v8_RTDExRxReady_s(i),
          o_RTDExRxReadReq_p            => v8_RTDExRxReadReq_s(i),
          iv32_RTDExRxData_p            => a8v32_RTDExRxData_s(i),
          i_RTDExRxDataValid_p          => v8_RTDExRxDataValid_s(i),
          
          -- RTDEx Tx interface
          --
          i_RTDExTxReady_p              => v8_RTDExTxReady_s(i),
          o_RTDExTxWriteReq_p           => v8_RTDExTxWriteReq_s(i),
          ov32_RTDExTxData_p            => a8v32_RTDExTxData_s(i),
          
          -- Interface compatible with RTDEx Tx user interface
          --
          o_encReady_p                  => v8_RTDExSyncTxReady_s(i),
          i_encWriteReq_p               => v8_RTDExSyncTxWriteReq_s(i),
          iv32_encData_p                => a8v32_RTDExSyncTxData_s(i)
        );
    end generate genRtdexSyncTx;
  end generate genUpLink_l;

end Behavioral;