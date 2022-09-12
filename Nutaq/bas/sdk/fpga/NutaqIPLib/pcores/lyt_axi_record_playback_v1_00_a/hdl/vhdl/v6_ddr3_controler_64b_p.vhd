
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;

package v6Ddr3Controler64b_p is
  
  component v6Ddr3Controler64b
    generic(
     REFCLK_FREQ           : real := 200.0;
                                     -- # = 200 for all design frequencies of
                                     --         -1 speed grade devices
                                     --   = 200 when design frequency < 480 MHz
                                     --         for -2 and -3 speed grade devices.
                                     --   = 300 when design frequency >= 480 MHz
                                     --         for -2 and -3 speed grade devices.
     IODELAY_GRP           : string := "DUAL_DDR3"; --"IODELAY_MIG";
                                     -- It is associated to a set of IODELAYs with
                                     -- an IDELAYCTRL that have same IODELAY CONTROLLER
                                     -- clock frequency.
     MMCM_ADV_BANDWIDTH    : string  := "OPTIMIZED";
                                     -- MMCM programming algorithm
     CLKFBOUT_MULT_F       : integer := 6;
                                     -- write PLL VCO multiplier.
     DIVCLK_DIVIDE         : integer := 2;
                                     -- write PLL VCO divisor.
     CLKOUT_DIVIDE         : integer := 3;
                                     -- VCO output divisor for fast (memory) clocks.
     nCK_PER_CLK           : integer := 2;
                                     -- # of memory CKs per fabric clock.
                                     -- # = 2, 1.
     tCK                   : integer := 2500;
                                     -- memory tCK paramter.
                                     -- # = Clock Period.
     DEBUG_PORT            : string := "OFF";
                                     -- # = "ON" Enable debug signals/controls.
                                     --   = "OFF" Disable debug signals/controls.
     SIM_BYPASS_INIT_CAL   : string := "OFF";
                                     -- # = "OFF" -  Complete memory init &
                                     --              calibration sequence
                                     -- # = "SKIP" - Skip memory init &
                                     --              calibration sequence
                                     -- # = "FAST" - Skip memory init & use
                                     --              abbreviated calib sequence
     nCS_PER_RANK          : integer := 1;
                                     -- # of unique CS outputs per Rank for
                                     -- phy.
     DQS_CNT_WIDTH         : integer := 3;
                                     -- # = ceil(log2(DQS_WIDTH)).
     RANK_WIDTH            : integer := 1;
                                     -- # = ceil(log2(RANKS)).
     BANK_WIDTH            : integer := 3;
                                     -- # of memory Bank Address bits.
     CK_WIDTH              : integer := 2;
                                     -- # of CK/CK# outputs to memory.
     CKE_WIDTH             : integer := 1;
                                     -- # of CKE outputs to memory.
     COL_WIDTH             : integer := 10;
                                     -- # of memory Column Address bits.
     CS_WIDTH              : integer := 1;
                                     -- # of unique CS outputs to memory.
     DM_WIDTH              : integer := 8;
                                     -- # of Data Mask bits.
     DQ_WIDTH              : integer := 64;
                                     -- # of Data (DQ) bits.
     DQS_WIDTH             : integer := 8;
                                     -- # of DQS/DQS# bits.
     ROW_WIDTH             : integer := 16;
                                     -- # of memory Row Address bits.
     BURST_MODE            : string := "8";
                                     -- Burst Length (Mode Register 0).
                                     -- # = "8", "4", "OTF".
     BM_CNT_WIDTH          : integer := 2;
                                     -- # = ceil(log2(nBANK_MACHS)).
     ADDR_CMD_MODE         : string := "1T" ;
                                     -- # = "2T", "1T".
     ORDERING              : string := "NORM";
                                     -- # = "NORM", "STRICT".
     WRLVL                 : string := "ON";
                                     -- # = "ON" - DDR3 SDRAM
                                     --   = "OFF" - DDR2 SDRAM.
     PHASE_DETECT          : string := "ON";
                                     -- # = "ON", "OFF".
     RTT_NOM               : string := "40";
                                     -- RTT_NOM (ODT) (Mode Register 1).
                                     -- # = "DISABLED" - RTT_NOM disabled,
                                     --   = "120" - RZQ/2,
                                     --   = "60"  - RZQ/4,
                                     --   = "40"  - RZQ/6.
     RTT_WR                : string := "OFF";
                                     -- RTT_WR (ODT) (Mode Register 2).
                                     -- # = "OFF" - Dynamic ODT off,
                                     --   = "120" - RZQ/2,
                                     --   = "60"  - RZQ/4,
     OUTPUT_DRV            : string := "HIGH";
                                     -- Output Driver Impedance Control (Mode Register 1).
                                     -- # = "HIGH" - RZQ/7,
                                     --   = "LOW" - RZQ/6.
     REG_CTRL              : string := "OFF";
                                     -- # = "ON" - RDIMMs,
                                     --   = "OFF" - Components, SODIMMs, UDIMMs.
     nDQS_COL0             : integer := 6;
                                     -- Number of DQS groups in I/O column #1.
     nDQS_COL1             : integer := 2;
                                     -- Number of DQS groups in I/O column #2.
     nDQS_COL2             : integer := 0;
                                     -- Number of DQS groups in I/O column #3.
     nDQS_COL3             : integer := 0;
                                     -- Number of DQS groups in I/O column #4.
     DQS_LOC_COL0          : std_logic_vector(47 downto 0) := X"050403020100";
                                     -- DQS groups in column #1.
     DQS_LOC_COL1          : std_logic_vector(15 downto 0) := X"0706";
                                     -- DQS groups in column #2.
     DQS_LOC_COL2          : std_logic_vector(0 downto 0) := "0";
                                     -- DQS groups in column #3.
     DQS_LOC_COL3          : std_logic_vector(0 downto 0) := "0";
                                     -- DQS groups in column #4.
     tPRDI                 : integer := 1000000;
                                     -- memory tPRDI paramter.
     tREFI                 : integer := 7800000;
                                     -- memory tREFI paramter.
     tZQI                  : integer := 128000000;
                                     -- memory tZQI paramter.
     ADDR_WIDTH            : integer := 30;
                                     -- # = RANK_WIDTH + BANK_WIDTH
                                     --     + ROW_WIDTH + COL_WIDTH;
     ECC                   : string := "OFF";
     ECC_TEST              : string := "OFF";
     TCQ                   : integer := 100;
     DATA_WIDTH            : integer := 64;
     -- If parameters overrinding is used for simulation, PAYLOAD_WIDTH
     -- parameter should to be overidden along with the vsim command
     PAYLOAD_WIDTH         : integer := 64;
   
    RST_ACT_LOW             : integer := 0;
                                       -- =1 for active low reset,
                                       -- =0 for active high.
    INPUT_CLK_TYPE          : string  := "SINGLE_ENDED"; --:= "DIFFERENTIAL";
                                       -- input clock type DIFFERENTIAL or SINGLE_ENDED
    STARVE_LIMIT            : integer := 2
                                       -- # = 2,3,4.
    );
  port(
      sys_clk  	  	: in    std_logic;
      clk_ref	    : in    std_logic;
      ddr3_dq       : inout std_logic_vector(DQ_WIDTH-1 downto 0);
      ddr3_dm       : out   std_logic_vector(DM_WIDTH-1 downto 0);
      ddr3_addr     : out   std_logic_vector(ROW_WIDTH-1 downto 0);
      ddr3_ba       : out   std_logic_vector(BANK_WIDTH-1 downto 0);
      ddr3_ras_n    : out   std_logic;
      ddr3_cas_n    : out   std_logic;
      ddr3_we_n     : out   std_logic;
      ddr3_reset_n  : out   std_logic;
      ddr3_cs_n     : out   std_logic_vector((CS_WIDTH*nCS_PER_RANK)-1 downto 0);
      ddr3_odt      : out   std_logic_vector((CS_WIDTH*nCS_PER_RANK)-1 downto 0);
      ddr3_cke      : out   std_logic_vector(CKE_WIDTH-1 downto 0);
      ddr3_dqs_p    : inout std_logic_vector(DQS_WIDTH-1 downto 0);
      ddr3_dqs_n    : inout std_logic_vector(DQS_WIDTH-1 downto 0);
      ddr3_ck_p     : out   std_logic_vector(CK_WIDTH-1 downto 0);
      ddr3_ck_n     : out   std_logic_vector(CK_WIDTH-1 downto 0);
	  iodelay_ctrl_rdy : in std_logic;
      sda           : inout std_logic;
      scl           : out   std_logic;
      app_wdf_wren  : in    std_logic;
      app_wdf_data  : in    std_logic_vector((4*PAYLOAD_WIDTH)-1 downto 0);
      app_wdf_mask  : in    std_logic_vector((4*PAYLOAD_WIDTH)/8-1 downto 0);
      app_wdf_end   : in    std_logic;
      app_addr      : in    std_logic_vector(ADDR_WIDTH-1 downto 0);
      app_cmd       : in    std_logic_vector(2 downto 0);
      app_en        : in    std_logic;
      app_rdy       : out   std_logic;
      app_wdf_rdy   : out   std_logic;
      app_rd_data   : out   std_logic_vector((4*PAYLOAD_WIDTH)-1 downto 0);
      app_rd_data_end : out   std_logic;
      app_rd_data_valid : out   std_logic;
      ui_clk_sync_rst : out   std_logic;
      ui_clk        : out   std_logic;
      phy_init_done : out   std_logic;
      sys_rst        : in std_logic
    );
end component;

end package v6Ddr3Controler64b_p;