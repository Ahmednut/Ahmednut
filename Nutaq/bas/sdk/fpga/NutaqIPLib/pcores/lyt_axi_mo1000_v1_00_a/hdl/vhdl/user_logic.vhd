--------------------------------------------------------------------------------
-- Filename:          user_logic.vhd
-- Version:           v1_00_a
-- Description:       User Logic implementation module
-- Generated by:      julien.roy
-- Date:              2015-03-09 14:13:30
-- Generated:         using LyrtechRD REGGENUTIL based on Xilinx IPIF Wizard.
-- VHDL Standard:     VHDL'93
------------------------------------------------------------------------------
-- Copyright (c) 2001-2012 LYRtech RD Inc.  All rights reserved.
------------------------------------------------------------------------------
--
-----------------------------------------------------------------------------
-- Register Memory Map & Description
-----------------------------------------------------------------------------
-- BASEADDR + 0x0   INFO    Core ID and version identifier
--   15:0 core_version R
--   31:16 core_id R

-- BASEADDR + 0x4   CONTROL    General control register
--   0:0 core_rst P O=o_core_rst_p
--   1:1 delay_rst P O=o_delay_rst_p
--   13:6 dac_ready R W O=ov8_dac_ready_p
--   14:14 data_pattern_en R W O=o_data_pattern_en_p
--   15:15 global_serdes_pd R W O=o_global_serdes_pd_p
--   2:2 delay_ctrl_rst P O=o_delay_ctrl_rst_p
--   23:16 data_serdes_pd R W O=ov8_data_serdes_pd_p
--   25:24 frame_mode R W+ O=ov2_frame_mode_p
--   27:26 frame_pattern R W O=ov2_frame_pattern_p
--   28:28 invert_dci R W O=o_invert_dci_p
--   3:3 serdes_rst P O=o_serdes_rst_p
--   31:29 rsvd R
--   4:4 mmcm_rst P O=o_mmcm_rst_p
--   5:5 update_dac_ready P O=o_update_dac_ready_p

-- BASEADDR + 0x8   STATUS    General status register
--   0:0 clock_master R I=i_clock_master_p
--   1:1 delay_ctrl_rdy R I=i_delay_ctrl_rdy_p
--   2:2 mmcm_locked R I=i_mmcm_locked_p
--   31:7 rsvd R
--   6:3 FmcPosition R I=iv4_FmcPosition_p

-- BASEADDR + 0xc   DELAY_CTRL    Delay value and DCI/Frame lane write enable mask
--   31:7 rsvd R
--   4:0 delay_value R W O=ov5_delay_value_p
--   5:5 delay_we_dci P O=o_delay_we_dci_p
--   6:6 delay_we_frame P O=o_delay_we_frame_p

-- BASEADDR + 0x10   DELAY_DATA    Data lane delay write enable mask
--   31:0 delay_we_data P O=ov32_delay_we_data_p

-- BASEADDR + 0x14   PATTERN_AB    Pattern A and B sent during calibration 
--   15:0 pattern_a R W O=ov16_pattern_a_p
--   31:16 pattern_b R W O=ov16_pattern_b_p

-- BASEADDR + 0x18   PATTERN_CD    Pattern C and D sent during calibration
--   15:0 pattern_c R W O=ov16_pattern_c_p
--   31:16 pattern_d R W O=ov16_pattern_d_p

-- BASEADDR + 0x1c   FREQ_CNT_CLK    Control and status of the clock frequency counter
--   15:0 freq_cnt R I=iv16_freq_cnt_p
--   16:16 freq_cnt_rst P O=o_freq_cnt_rst_p
--   17:17 freq_cnt_rdy R I=i_freq_cnt_rdy_p
--   18:18 freq_cnt_sel R W O=o_freq_cnt_sel_p
--   31:19 rsvd R

-- BASEADDR + 0x20   TRIGGER_DELAY    Control the trigger delay register path.
--   31:5 rsvd R
--   4:0 trigger_delay R W O=ov5_trigger_delay_p

--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

library proc_common_v3_00_a;
use proc_common_v3_00_a.proc_common_pkg.all;

-- DO NOT EDIT ABOVE THIS LINE --------------------

--USER libraries added here

------------------------------------------------------------------------------
-- Entity section
------------------------------------------------------------------------------
-- Definition of Generics:
--   C_NUM_REG                    -- Number of software accessible registers
--   C_SLV_DWIDTH                 -- Slave interface data bus width
--
-- Definition of Ports:
--   Bus2IP_Clk                   -- Bus to IP clock
--   Bus2IP_Resetn                -- Bus to IP reset
--   Bus2IP_Data                  -- Bus to IP data bus
--   Bus2IP_BE                    -- Bus to IP byte enables
--   Bus2IP_RdCE                  -- Bus to IP read chip enable
--   Bus2IP_WrCE                  -- Bus to IP write chip enable
--   IP2Bus_Data                  -- IP to Bus data bus
--   IP2Bus_RdAck                 -- IP to Bus read transfer acknowledgement
--   IP2Bus_WrAck                 -- IP to Bus write transfer acknowledgement
--   IP2Bus_Error                 -- IP to Bus error response
------------------------------------------------------------------------------

entity user_logic is
  generic
  (
    -- ADD USER GENERICS BELOW THIS LINE ---------------
    C_BUILD_REVISION            : std_logic_vector      := X"0200";
    -- ADD USER GENERICS ABOVE THIS LINE ---------------

    -- DO NOT EDIT BELOW THIS LINE ---------------------
    -- Bus protocol parameters, do not add to or delete
    C_NUM_REG                      : integer              := 9;
    C_SLV_DWIDTH                   : integer              := 32
    -- DO NOT EDIT ABOVE THIS LINE ---------------------
  );
  port
  (
    -- ADD USER PORTS BELOW THIS LINE ------------------
    --USER ports added here
    -- ADD USER PORTS ABOVE THIS LINE ------------------
    -- User ports
  i_CoreReset_p : in std_logic;

    o_core_rst_p : out std_logic;
    o_delay_rst_p : out std_logic;
    ov8_dac_ready_p : out std_logic_vector(7 downto 0);
    o_data_pattern_en_p : out std_logic;
    o_global_serdes_pd_p : out std_logic;
    o_delay_ctrl_rst_p : out std_logic;
    ov8_data_serdes_pd_p : out std_logic_vector(7 downto 0);
    ov2_frame_mode_p : out std_logic_vector(1 downto 0);
    o_frame_modeWrite_p : out std_logic;
    ov2_frame_pattern_p : out std_logic_vector(1 downto 0);
    o_invert_dci_p : out std_logic;
    o_serdes_rst_p : out std_logic;
    o_mmcm_rst_p : out std_logic;
    o_update_dac_ready_p : out std_logic;
    i_clock_master_p : in std_logic;
    i_delay_ctrl_rdy_p : in std_logic;
    i_mmcm_locked_p : in std_logic;
    iv4_FmcPosition_p : in std_logic_vector(3 downto 0);
    ov5_delay_value_p : out std_logic_vector(4 downto 0);
    o_delay_we_dci_p : out std_logic;
    o_delay_we_frame_p : out std_logic;
    ov32_delay_we_data_p : out std_logic_vector(31 downto 0);
    ov16_pattern_a_p : out std_logic_vector(15 downto 0);
    ov16_pattern_b_p : out std_logic_vector(15 downto 0);
    ov16_pattern_c_p : out std_logic_vector(15 downto 0);
    ov16_pattern_d_p : out std_logic_vector(15 downto 0);
    iv16_freq_cnt_p : in std_logic_vector(15 downto 0);
    o_freq_cnt_rst_p : out std_logic;
    i_freq_cnt_rdy_p : in std_logic;
    o_freq_cnt_sel_p : out std_logic;
    ov5_trigger_delay_p : out std_logic_vector(4 downto 0);
    -- Bus protocol ports, do not add to or delete
    Bus2IP_Clk                     : in  std_logic;
    Bus2IP_Resetn                  : in  std_logic;
    Bus2IP_Data                    : in  std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    Bus2IP_BE                      : in  std_logic_vector(C_SLV_DWIDTH/8-1 downto 0);
    Bus2IP_RdCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    Bus2IP_WrCE                    : in  std_logic_vector(C_NUM_REG-1 downto 0);
    IP2Bus_Data                    : out std_logic_vector(C_SLV_DWIDTH-1 downto 0);
    IP2Bus_RdAck                   : out std_logic;
    IP2Bus_WrAck                   : out std_logic;
    IP2Bus_Error                   : out std_logic
  );

 attribute MAX_FANOUT : string;
 attribute SIGIS : string;
 attribute SIGIS of Bus2IP_Clk    : signal is "CLK";
 attribute SIGIS of Bus2IP_Resetn : signal is "RST";

end entity user_logic;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture IMP of user_logic is

-------------------------------------------------------------------------------
-- Constant declarations
-------------------------------------------------------------------------------


-------------------------------------------------------------------------------
--     ************** Function declaratin *******************                   
-- Return a std_logic_vector with only one bit set to one.
-- The argument BitPosition represent the bit position to set to one, starting with 0.
-- The argument Width represent the width of the returned std_logic_vector.
-------------------------------------------------------------------------------
  function OneHotVector( BitPosition : integer;                              
                Width : integer)                                             
                return std_logic_vector                                      
  is                                                                         
    variable Result                   : std_logic_vector(Width - 1 downto 0);

  begin                        
    Result := (others => '0'); 
    Result(BitPosition) := '1';
    return Result;             
  end OneHotVector;            
-------------------------------------------------------------------------------
-- Signal and Type Declarations
-------------------------------------------------------------------------------

  signal core_rst_s                     : std_logic;
  signal delay_rst_s                     : std_logic;
  signal v8_dac_ready_s                     : std_logic_vector(7 downto 0);
  signal data_pattern_en_s                     : std_logic;
  signal global_serdes_pd_s                     : std_logic;
  signal delay_ctrl_rst_s                     : std_logic;
  signal v8_data_serdes_pd_s                     : std_logic_vector(7 downto 0);
  signal v2_frame_mode_s                     : std_logic_vector(1 downto 0);
  signal v2_frame_pattern_s                     : std_logic_vector(1 downto 0);
  signal invert_dci_s                     : std_logic;
  signal serdes_rst_s                     : std_logic;
  signal mmcm_rst_s                     : std_logic;
  signal update_dac_ready_s                     : std_logic;
  signal v5_delay_value_s                     : std_logic_vector(4 downto 0);
  signal delay_we_dci_s                     : std_logic;
  signal delay_we_frame_s                     : std_logic;
  signal v32_delay_we_data_s                     : std_logic_vector(31 downto 0);
  signal v16_pattern_a_s                     : std_logic_vector(15 downto 0);
  signal v16_pattern_b_s                     : std_logic_vector(15 downto 0);
  signal v16_pattern_c_s                     : std_logic_vector(15 downto 0);
  signal v16_pattern_d_s                     : std_logic_vector(15 downto 0);
  signal freq_cnt_rst_s                     : std_logic;
  signal freq_cnt_sel_s                     : std_logic;
  signal v5_trigger_delay_s                     : std_logic_vector(4 downto 0);
  signal slv_reg_write_sel              : std_logic_vector(8 downto 0);
  signal slv_reg_read_sel               : std_logic_vector(8 downto 0);
  signal slv_ip2bus_data                : std_logic_vector(C_SLV_DWIDTH-1 downto 0);
  signal slv_read_ack                   : std_logic;
  signal slv_write_ack                  : std_logic;

------------------------------------------------------------------------------
begin
------------------------------------------------------------------------------

-------------------------------------------------------------------------------
-- Begin architecture
-------------------------------------------------------------------------------

-- swap bits
WrCeBitSwap: for i in 0 to slv_reg_write_sel'high generate
  slv_reg_write_sel(i) <= Bus2IP_WrCE(slv_reg_write_sel'high - i);
end generate WrCeBitSwap;

RdCeBitSwap: for i in 0 to slv_reg_read_sel'high generate
  slv_reg_read_sel(i)  <= Bus2IP_RdCE(slv_reg_read_sel'high - i);
end generate RdCeBitSwap;

-- generate write/read ack
  slv_write_ack <=   Bus2IP_WrCE(0) or   Bus2IP_WrCE(1) or   Bus2IP_WrCE(2) or   Bus2IP_WrCE(3) or   Bus2IP_WrCE(4) or   Bus2IP_WrCE(5) or   Bus2IP_WrCE(6) or   Bus2IP_WrCE(7) or   Bus2IP_WrCE(8);
  slv_read_ack  <=   Bus2IP_RdCE(0) or   Bus2IP_RdCE(1) or   Bus2IP_RdCE(2) or   Bus2IP_RdCE(3) or   Bus2IP_RdCE(4) or   Bus2IP_RdCE(5) or   Bus2IP_RdCE(6) or   Bus2IP_RdCE(7) or   Bus2IP_RdCE(8);

 -- implement slave model software accessible register(s)
 SLAVE_REG_WRITE_PROC : process( Bus2IP_Clk ) is
 begin

  if Bus2IP_Clk'event and Bus2IP_Clk = '1' then
    if Bus2IP_Resetn = '0' then
      core_rst_s <= '0';
      delay_rst_s <= '0';
      v8_dac_ready_s <= "00000000";
      data_pattern_en_s <= '0';
      global_serdes_pd_s <= '1';
      delay_ctrl_rst_s <= '0';
      v8_data_serdes_pd_s <= "11111111";
      v2_frame_mode_s <= "00";
      o_frame_modeWrite_p <= '0';
      v2_frame_pattern_s <= "00";
      invert_dci_s <= '0';
      serdes_rst_s <= '0';
      mmcm_rst_s <= '0';
      update_dac_ready_s <= '0';
      v5_delay_value_s <= "00000";
      delay_we_dci_s <= '0';
      delay_we_frame_s <= '0';
      v32_delay_we_data_s <= X"00000000";
      v16_pattern_a_s <= X"0000";
      v16_pattern_b_s <= X"0000";
      v16_pattern_c_s <= X"0000";
      v16_pattern_d_s <= X"0000";
      freq_cnt_rst_s <= '0';
      freq_cnt_sel_s <= '0';
      v5_trigger_delay_s <= "00000";

    else

  -- Synchronous reset
  if ( i_CoreReset_p = '1' ) then
    core_rst_s <= '0';
    delay_rst_s <= '0';
    v8_dac_ready_s <= "00000000";
    data_pattern_en_s <= '0';
    global_serdes_pd_s <= '1';
    delay_ctrl_rst_s <= '0';
    v8_data_serdes_pd_s <= "11111111";
    v2_frame_mode_s <= "00";
    v2_frame_pattern_s <= "00";
    invert_dci_s <= '0';
    serdes_rst_s <= '0';
    mmcm_rst_s <= '0';
    update_dac_ready_s <= '0';
    v5_delay_value_s <= "00000";
    delay_we_dci_s <= '0';
    delay_we_frame_s <= '0';
    v32_delay_we_data_s <= X"00000000";
    v16_pattern_a_s <= X"0000";
    v16_pattern_b_s <= X"0000";
    v16_pattern_c_s <= X"0000";
    v16_pattern_d_s <= X"0000";
    freq_cnt_rst_s <= '0';
    freq_cnt_sel_s <= '0';
    v5_trigger_delay_s <= "00000";
  end if;

  core_rst_s <= '0';
  delay_rst_s <= '0';
  delay_ctrl_rst_s <= '0';
  o_frame_modeWrite_p <= '0';
  serdes_rst_s <= '0';
  mmcm_rst_s <= '0';
  update_dac_ready_s <= '0';
  delay_we_dci_s <= '0';
  delay_we_frame_s <= '0';
  v32_delay_we_data_s <= X"00000000";
  freq_cnt_rst_s <= '0';
      case slv_reg_write_sel is

        when OneHotVector(1,9) =>
          if (Bus2IP_BE(0) = '1') then
            core_rst_s <= Bus2IP_Data(0);
          end if;
          if (Bus2IP_BE(0) = '1') then
            delay_rst_s <= Bus2IP_Data(1);
          end if;
          if (Bus2IP_BE(0) = '1') then
            v8_dac_ready_s(1 downto 0) <= Bus2IP_Data(7 downto 6);
          end if;
          if (Bus2IP_BE(1) = '1') then
            v8_dac_ready_s(7 downto 2) <= Bus2IP_Data(13 downto 8);
          end if;
          if (Bus2IP_BE(1) = '1') then
            data_pattern_en_s <= Bus2IP_Data(14);
          end if;
          if (Bus2IP_BE(1) = '1') then
            global_serdes_pd_s <= Bus2IP_Data(15);
          end if;
          if (Bus2IP_BE(0) = '1') then
            delay_ctrl_rst_s <= Bus2IP_Data(2);
          end if;
          if (Bus2IP_BE(2) = '1') then
            v8_data_serdes_pd_s <= Bus2IP_Data(23 downto 16);
          end if;
          if (Bus2IP_BE(3) = '1') then
            v2_frame_mode_s <= Bus2IP_Data(25 downto 24);
            o_frame_modeWrite_p <= '1';
          end if;
          if (Bus2IP_BE(3) = '1') then
            v2_frame_pattern_s <= Bus2IP_Data(27 downto 26);
          end if;
          if (Bus2IP_BE(3) = '1') then
            invert_dci_s <= Bus2IP_Data(28);
          end if;
          if (Bus2IP_BE(0) = '1') then
            serdes_rst_s <= Bus2IP_Data(3);
          end if;
          if (Bus2IP_BE(0) = '1') then
            mmcm_rst_s <= Bus2IP_Data(4);
          end if;
          if (Bus2IP_BE(0) = '1') then
            update_dac_ready_s <= Bus2IP_Data(5);
          end if;

        when OneHotVector(3,9) =>
          if (Bus2IP_BE(0) = '1') then
            v5_delay_value_s <= Bus2IP_Data(4 downto 0);
          end if;
          if (Bus2IP_BE(0) = '1') then
            delay_we_dci_s <= Bus2IP_Data(5);
          end if;
          if (Bus2IP_BE(0) = '1') then
            delay_we_frame_s <= Bus2IP_Data(6);
          end if;

        when OneHotVector(4,9) =>
          if (Bus2IP_BE(0) = '1') then
            v32_delay_we_data_s(7 downto 0) <= Bus2IP_Data(7 downto 0);
          end if;
          if (Bus2IP_BE(1) = '1') then
            v32_delay_we_data_s(15 downto 8) <= Bus2IP_Data(15 downto 8);
          end if;
          if (Bus2IP_BE(2) = '1') then
            v32_delay_we_data_s(23 downto 16) <= Bus2IP_Data(23 downto 16);
          end if;
          if (Bus2IP_BE(3) = '1') then
            v32_delay_we_data_s(31 downto 24) <= Bus2IP_Data(31 downto 24);
          end if;

        when OneHotVector(5,9) =>
          if (Bus2IP_BE(0) = '1') then
            v16_pattern_a_s(7 downto 0) <= Bus2IP_Data(7 downto 0);
          end if;
          if (Bus2IP_BE(1) = '1') then
            v16_pattern_a_s(15 downto 8) <= Bus2IP_Data(15 downto 8);
          end if;
          if (Bus2IP_BE(2) = '1') then
            v16_pattern_b_s(7 downto 0) <= Bus2IP_Data(23 downto 16);
          end if;
          if (Bus2IP_BE(3) = '1') then
            v16_pattern_b_s(15 downto 8) <= Bus2IP_Data(31 downto 24);
          end if;

        when OneHotVector(6,9) =>
          if (Bus2IP_BE(0) = '1') then
            v16_pattern_c_s(7 downto 0) <= Bus2IP_Data(7 downto 0);
          end if;
          if (Bus2IP_BE(1) = '1') then
            v16_pattern_c_s(15 downto 8) <= Bus2IP_Data(15 downto 8);
          end if;
          if (Bus2IP_BE(2) = '1') then
            v16_pattern_d_s(7 downto 0) <= Bus2IP_Data(23 downto 16);
          end if;
          if (Bus2IP_BE(3) = '1') then
            v16_pattern_d_s(15 downto 8) <= Bus2IP_Data(31 downto 24);
          end if;

        when OneHotVector(7,9) =>
          if (Bus2IP_BE(2) = '1') then
            freq_cnt_rst_s <= Bus2IP_Data(16);
          end if;
          if (Bus2IP_BE(2) = '1') then
            freq_cnt_sel_s <= Bus2IP_Data(18);
          end if;

        when OneHotVector(8,9) =>
          if (Bus2IP_BE(0) = '1') then
            v5_trigger_delay_s <= Bus2IP_Data(4 downto 0);
          end if;
        when others =>
          null;
      end case;
    end if;
  end if;

 end process SLAVE_REG_WRITE_PROC;

 -- implement slave model software accessible register(s) read mux
SLAVE_REG_READ_PROC : process( slv_reg_read_sel, v8_dac_ready_s, data_pattern_en_s, global_serdes_pd_s, v8_data_serdes_pd_s, v2_frame_mode_s, v2_frame_pattern_s, invert_dci_s, i_clock_master_p, i_delay_ctrl_rdy_p, i_mmcm_locked_p, iv4_FmcPosition_p, v5_delay_value_s, v16_pattern_a_s, v16_pattern_b_s, v16_pattern_c_s, v16_pattern_d_s, iv16_freq_cnt_p, i_freq_cnt_rdy_p, freq_cnt_sel_s, v5_trigger_delay_s) is
 begin
   case slv_reg_read_sel is

        when OneHotVector(0,9) =>
          slv_ip2bus_data(15 downto 0) <= C_BUILD_REVISION;
          slv_ip2bus_data(31 downto 16) <= X"DAC0";

        when OneHotVector(1,9) =>
          slv_ip2bus_data(13 downto 6) <= v8_dac_ready_s;
          slv_ip2bus_data(14) <= data_pattern_en_s;
          slv_ip2bus_data(15) <= global_serdes_pd_s;
          slv_ip2bus_data(23 downto 16) <= v8_data_serdes_pd_s;
          slv_ip2bus_data(25 downto 24) <= v2_frame_mode_s;
          slv_ip2bus_data(27 downto 26) <= v2_frame_pattern_s;
          slv_ip2bus_data(28) <= invert_dci_s;
          slv_ip2bus_data(31 downto 29) <= "000";

        when OneHotVector(2,9) =>
          slv_ip2bus_data(0) <= i_clock_master_p;
          slv_ip2bus_data(1) <= i_delay_ctrl_rdy_p;
          slv_ip2bus_data(2) <= i_mmcm_locked_p;
          slv_ip2bus_data(31 downto 7) <= "0000000000000000000000000";
          slv_ip2bus_data(6 downto 3) <= iv4_FmcPosition_p;

        when OneHotVector(3,9) =>
          slv_ip2bus_data(31 downto 7) <= "0000000000000000000000000";
          slv_ip2bus_data(4 downto 0) <= v5_delay_value_s;

        when OneHotVector(5,9) =>
          slv_ip2bus_data(15 downto 0) <= v16_pattern_a_s;
          slv_ip2bus_data(31 downto 16) <= v16_pattern_b_s;

        when OneHotVector(6,9) =>
          slv_ip2bus_data(15 downto 0) <= v16_pattern_c_s;
          slv_ip2bus_data(31 downto 16) <= v16_pattern_d_s;

        when OneHotVector(7,9) =>
          slv_ip2bus_data(15 downto 0) <= iv16_freq_cnt_p;
          slv_ip2bus_data(17) <= i_freq_cnt_rdy_p;
          slv_ip2bus_data(18) <= freq_cnt_sel_s;
          slv_ip2bus_data(31 downto 19) <= "0000000000000";

        when OneHotVector(8,9) =>
          slv_ip2bus_data(31 downto 5) <= "000000000000000000000000000";
          slv_ip2bus_data(4 downto 0) <= v5_trigger_delay_s;
        when others =>
          slv_ip2bus_data <= (others => '0');
      end case;

 end process SLAVE_REG_READ_PROC;

------------------------------------------
-- drive IP to Bus signals
------------------------------------------
IP2Bus_Data  <= slv_ip2bus_data when slv_read_ack = '1' else (others => '0');
IP2Bus_WrAck <= slv_write_ack;
IP2Bus_RdAck <= slv_read_ack;
IP2Bus_Error <= '0';

------------------------------------------
-- Output assignments
------------------------------------------
o_core_rst_p <= core_rst_s;
o_delay_rst_p <= delay_rst_s;
ov8_dac_ready_p <= v8_dac_ready_s;
o_data_pattern_en_p <= data_pattern_en_s;
o_global_serdes_pd_p <= global_serdes_pd_s;
o_delay_ctrl_rst_p <= delay_ctrl_rst_s;
ov8_data_serdes_pd_p <= v8_data_serdes_pd_s;
ov2_frame_mode_p <= v2_frame_mode_s;
ov2_frame_pattern_p <= v2_frame_pattern_s;
o_invert_dci_p <= invert_dci_s;
o_serdes_rst_p <= serdes_rst_s;
o_mmcm_rst_p <= mmcm_rst_s;
o_update_dac_ready_p <= update_dac_ready_s;
ov5_delay_value_p <= v5_delay_value_s;
o_delay_we_dci_p <= delay_we_dci_s;
o_delay_we_frame_p <= delay_we_frame_s;
ov32_delay_we_data_p <= v32_delay_we_data_s;
ov16_pattern_a_p <= v16_pattern_a_s;
ov16_pattern_b_p <= v16_pattern_b_s;
ov16_pattern_c_p <= v16_pattern_c_s;
ov16_pattern_d_p <= v16_pattern_d_s;
o_freq_cnt_rst_p <= freq_cnt_rst_s;
o_freq_cnt_sel_p <= freq_cnt_sel_s;
ov5_trigger_delay_p <= v5_trigger_delay_s;

end IMP;

