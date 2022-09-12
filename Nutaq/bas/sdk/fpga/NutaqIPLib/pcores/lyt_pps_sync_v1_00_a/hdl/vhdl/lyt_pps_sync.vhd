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
-- File : lyt_pps_sync.vhd
--------------------------------------------------------------------------------
-- Description : PPS clock sync core
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_pps_sync.vhd,v $
-- Revision 1.3  2015/11/18 21:01:32  julien.roy
-- Remove unused FIFO
--
-- Revision 1.2  2015/11/17 20:53:28  julien.roy
-- Add new PPS Sync implementation that supports PPS input rising edge event rate different than 1 Hz.
--
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.6  2015/01/30 18:22:52  julien.roy
-- Remove timestamp from pps sync core
-- Add constraint for clock domain crossing.
-- Solve PPS Sync problem that lock at 1 Hz higher (Problem in the clock counter).
--
-- Revision 1.5  2013/12/02 13:21:51  minh-quang.nguyen
-- added timestamp generator
--
-- Revision 1.4  2013/09/11 19:31:22  khalid.bensadek
-- fixed a glitches in sync module & fixed a bug in moving sum logic & added cross domain one register to ease timings
--
-- Revision 1.3  2013/09/04 15:24:58  khalid.bensadek
-- added logic for moving sum
--
-- Revision 1.2  2013/08/29 17:55:00  khalid.bensadek
-- Added FIFO and other logic for clock domaine crossing
--
-- Revision 1.1  2013/08/01 20:28:01  khalid.bensadek
-- Fisrt commit: tested with Radio420 only.
--
--
--------------------------------------------------------------------------------

library ieee;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

library unisim;
  use unisim.vcomponents.all;
  
library lyt_pps_sync_v1_00_a;
    use lyt_pps_sync_v1_00_a.all;

entity lyt_pps_sync is
    generic
    (
        v16_CoreId_g     : std_logic_vector(15 downto 0):=x"CC01";
        v16_VersNbr_g    : std_logic_vector(15 downto 0):=x"0200";
        C_DDR_INPUT      : integer := 0
    );
    port
    (
        i_SysClk_p              : in  std_logic;                        -- Micro-controller system clock: 100 Mhz
        i_Clk_p                 : in  std_logic;                        -- Clock under correction
        i_Pps_p                 : in  std_logic;                        -- PPS signal from external GPS
        -- Regs CTRL & Status
        ov32_CoreIdVers_p       : out std_logic_vector(31 downto 0);
        iv32_ClkTarget_p        : in  std_logic_vector(31 downto 0);    -- Clock expected clock frequency
        iv32_PpsFreq_p          : in  std_logic_vector(31 downto 0);    -- PPS rising-edge frequency
        iv32_ProgDelay_p        : in  std_logic_vector(31 downto 0);    -- Prog delay for moving window
        i_CoreReset_p           : in  std_logic;
        i_RstUponRead_p         : in  std_logic;
        i_RstAcc_p              : in  std_logic;
        i_CoreEnable_p          : in  std_logic;
        i_MovingSumEna_p        : in  std_logic;
        i_MovingSumRst_p        : in  std_logic;
        i_DiffAccRead_p         : in  std_logic;                        -- Read ack signal for ov32_DiffAcc_p register
        ov32_PpsCnt_p           : out std_logic_vector(31 downto 0);    -- PPS count: FMC clock cycles count between two consecutive PPS pules
        ov32_DiffAcc_p          : out std_logic_vector(31 downto 0)     -- FMC frequency difference accumulated value
    );
end entity lyt_pps_sync;

architecture rtl of lyt_pps_sync is

 signal AccReset_s            : std_logic;
 signal CoreResetTmp_s        : std_logic;
 signal CoreReset_s            : std_logic;
 signal RstAccTmp_s            : std_logic;
 signal RstAcc_s            : std_logic;
 signal DiffAccReadTmp1_s    : std_logic;
 signal DiffAccReadTmp2_s    : std_logic;
 signal DiffAccReadTmp3_s    : std_logic;
 signal DiffAccRead_s        : std_logic;

 signal PpsRedge_s            : std_logic;
 signal PpsNewCnt_s            : std_logic;
 signal PpsR_s                : std_logic;
 signal PpsF_s                : std_logic;
 signal PpsF_R1_s            : std_logic;

 signal u32_ClkPeriodCurrentCnt_s        : unsigned(31 downto 0);
 signal u32_ClkPeriodCnt_s        : unsigned(31 downto 0);
 signal u32_ClkTargetM1_s        : unsigned(31 downto 0);
 signal v32_ClkTargetDelta_s    : std_logic_vector(31 downto 0);
 signal s32_AccValue_s        : signed(31 downto 0);
 signal v32_SignalStretch_s    : std_logic_vector(31 downto 0);
 signal DiffAccReadStrched_s : std_logic;
 signal ClkDeltaValid_s  : std_logic;
 signal v32_MovingSumVal_s    : std_logic_vector(31 downto 0);
 signal v32_CTEValue_s        : std_logic_vector(31 downto 0);
 signal ProgDlyRst_s        : std_logic;
 signal MovingSumRst_s        : std_logic;
 signal RstUponRead_s          : std_logic;
 signal CoreEnable_s           : std_logic;
 signal MovingSumEna_s         : std_logic;
 signal v4_ProgDelay_s         : std_logic_vector(3 downto 0);

 signal u32_PpsFreqM1_s        : unsigned(31 downto 0);
 signal u32_RedgeCnt_s      : unsigned(31 downto 0) := (others => '0');


 -- For dbg. To be removed --
 -- attribute keep_hierarchy : string;
 -- attribute keep_hierarchy of rtl : architecture is "true";

 -- attribute keep : string;
 -- attribute keep of PpsRedge_s : signal is "true";
 -- attribute keep of AccReset_s : signal is "true";
 -- attribute keep of MovingSumRst_s : signal is "true";
 -- end for dbg --

begin

    ov32_CoreIdVers_p   <= v16_CoreId_g & v16_VersNbr_g;

    ---------------------------------------------------------
    -- Sync ctrl signals with design/FMC clock
    ---------------------------------------------------------
    Syncer_i0: entity lyt_pps_sync_v1_00_a.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => i_CoreReset_p,
        i_OutClk_p  => i_Clk_p,
        o_OutSig_p  => CoreReset_s
    );

    Syncer_i1: entity lyt_pps_sync_v1_00_a.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => i_RstAcc_p,
        i_OutClk_p  => i_Clk_p,
        o_OutSig_p  => RstAcc_s
    );

    Syncer_i2: entity lyt_pps_sync_v1_00_a.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => i_DiffAccRead_p,
        i_OutClk_p  => i_Clk_p,
        o_OutSig_p  => DiffAccReadStrched_s
    );

    Syncer_i3: entity lyt_pps_sync_v1_00_a.syncer
    port map(
        i_InClk_p   => i_SysClk_p,
        i_InSig_p   => i_MovingSumRst_p,
        i_OutClk_p  => i_Clk_p,
        o_OutSig_p  => MovingSumRst_s
    );

    -- detect rising edge
    DiffAccReadTmp2_s <= DiffAccReadStrched_s;
    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            DiffAccReadTmp3_s <= DiffAccReadTmp2_s;
        end if;
    end process;

    DiffAccRead_s <= DiffAccReadTmp2_s and not(DiffAccReadTmp3_s);

    -- Registering input signals to aide timing
    process(i_Clk_p)
    begin
        if rising_Edge(i_Clk_p) then
            RstUponRead_s      <= i_RstUponRead_p;
            CoreEnable_s       <= i_CoreEnable_p;
            MovingSumEna_s     <= i_MovingSumEna_p;
            v4_ProgDelay_s     <= iv32_ProgDelay_p(3 downto 0);
            u32_ClkTargetM1_s  <= unsigned(iv32_ClkTarget_p)-1;
            u32_PpsFreqM1_s    <= unsigned(iv32_PpsFreq_p)-1;
        end if;
    end process;

    ---------------------------------------------------------
    -- Accumulated value (CTE) reset management
    ---------------------------------------------------------
    AccReset_s    <= CoreReset_s or RstAcc_s or (RstUponRead_s and DiffAccRead_s);

    -------------------------------------------------------
    -- PPS rising_edge detect
    -------------------------------------------------------
    gen_ddr_input : if C_DDR_INPUT /= 0 generate
        ADCDataIDDR_l : IDDR
        generic map (
            DDR_CLK_EDGE => "SAME_EDGE_PIPELINED",
            INIT_Q1 => '0',         -- Initial value of Q1: '0' or '1'
            INIT_Q2 => '0',         -- Initial value of Q2: '0' or '1'
            SRTYPE => "SYNC")       -- Set/Reset type: "SYNC" or "ASYNC"
        port map (
            Q1 => PpsR_s,           -- 1-bit output for positive edge of clock
            Q2 => PpsF_s,           -- 1-bit output for negative edge of clock
            C => i_Clk_p,           -- 1-bit clock input
            CE => '1',              -- 1-bit clock enable input
            D => i_Pps_p,           -- 1-bit DDR data input
            R => '0',               -- 1-bit reset
            S => '0'                -- 1-bit set
        );
    end generate;
    
    gen_sdr_input : if C_DDR_INPUT = 0 generate
        process(i_Clk_p)
        begin
            if rising_edge(i_Clk_p) then
                PpsR_s <= i_Pps_p;
                PpsF_s <= i_Pps_p;
            end if;
        end process;
    end generate;

    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            PpsF_R1_s <= PpsF_s;

            if(PpsF_s = '1' and PpsR_s = '0') then
                PpsRedge_s <= '1';
            elsif(PpsR_s = '1' and PpsF_R1_s = '0') then
                PpsRedge_s <= '1';
            else
                PpsRedge_s <= '0';
            end if;

        end if;
    end process;

    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            -- Default value
            PpsNewCnt_s <= '0';

            if (CoreEnable_s = '1' and PpsRedge_s = '1') then
                if(u32_RedgeCnt_s >= u32_PpsFreqM1_s) then
                    PpsNewCnt_s     <= '1';
                    u32_RedgeCnt_s  <= (others => '0');
                else
                    u32_RedgeCnt_s <= u32_RedgeCnt_s + 1;
                end if;
            end if;

            -- Synchronous reset
            if CoreReset_s = '1' then
                u32_RedgeCnt_s <= (others => '0');
            end if;

        end if;
    end process;


    ----------------------------------------------------------
    -- PPS counter: counts number of FMC clock cycles between
    -- Two consecutive PPS pulse.
    ----------------------------------------------------------
    process(i_Clk_p)
    begin
        if rising_Edge(i_Clk_p) then
            if CoreReset_s = '1' then
                u32_ClkPeriodCurrentCnt_s <= (others=>'0');
                u32_ClkPeriodCnt_s <= (others=>'0');
            elsif CoreEnable_s = '1' then
                if PpsNewCnt_s = '1'then
                    u32_ClkPeriodCnt_s <= (u32_ClkPeriodCurrentCnt_s + 1);     -- Register count value
                    u32_ClkPeriodCurrentCnt_s <= (others=>'0');     -- Reset the counter
                else
                    u32_ClkPeriodCurrentCnt_s <= u32_ClkPeriodCurrentCnt_s + 1;
                end if;
            end if;
        end if;
    end process;

    -------------------------------------------------------
    -- FMC clock delta calculation.
    -------------------------------------------------------
    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            -- Default value
            ClkDeltaValid_s <= '0';
        
            if CoreReset_s = '1' then
                v32_ClkTargetDelta_s <= (others=>'0');
        
            elsif CoreEnable_s = '1' then
        
                if PpsNewCnt_s = '1' then
                    -- Last clock cycle u32_ClkPeriodCurrentCnt_s value
                    -- but ClkTarget is already subtracted by 1
                    v32_ClkTargetDelta_s <= std_logic_vector(u32_ClkTargetM1_s - u32_ClkPeriodCurrentCnt_s);
                    ClkDeltaValid_s <= '1';
                end if;
        
            end if;
        end if;
    end process;

    ------------------------------------------------------------
    -- FMC clock delta accumulator.
    -- This process should be enabled only after PPS is
    -- detected by the microcontroller to avoid erroneous values
    ------------------------------------------------------------
    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            if AccReset_s = '1' then
                s32_AccValue_s <= (others=>'0');
            elsif CoreEnable_s = '1' then -- Should be enabled only after PPS is detected
                if ClkDeltaValid_s = '1' then
                    s32_AccValue_s <= s32_AccValue_s + signed(v32_ClkTargetDelta_s);
                end if;
            end if;
        end if;
    end process;

    ------------------------------------------------------------
    -- Moving sum implementation
    ------------------------------------------------------------
    progdelay_i0: entity lyt_pps_sync_v1_00_a.progdelay
    port map(
        i_clk_p             => i_Clk_p,
        i_reset_p           => ProgDlyRst_s,
        iv4_ProgDelay_p     => v4_ProgDelay_s,
        i_DlyEna_p          => ClkDeltaValid_s,
        iv32_InSig_p        => v32_ClkTargetDelta_s,
        ov32_InSig_p        => v32_MovingSumVal_s
    );

    ProgDlyRst_s <= MovingSumRst_s or AccReset_s;

    -- Mux for output reg value
    process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            if MovingSumEna_s = '1' then
                v32_CTEValue_s <= std_logic_vector(s32_AccValue_s - signed(v32_MovingSumVal_s));
            else
                v32_CTEValue_s <= std_logic_vector(s32_AccValue_s);
            end if;
        end if;
    end process;

    ------------------------------------------------------------
    -- Async FiFo for clock domain crossing
    ------------------------------------------------------------

    BusSyncer_i0: entity lyt_pps_sync_v1_00_a.bus_syncer
    generic map(
        C_WIDTH         => 32
    )
	port map(
		i_InClk_p 	    => i_Clk_p,
		iv_InSig_p 	    => std_logic_vector(u32_ClkPeriodCnt_s),
		i_OutClk_p 	    => i_SysClk_p,
		ov_OutSig_p 	=> ov32_PpsCnt_p
	);

    BusSyncer_i1: entity lyt_pps_sync_v1_00_a.bus_syncer
    generic map(
        C_WIDTH         => 32
    )
	port map(
		i_InClk_p 	    => i_Clk_p,
		iv_InSig_p 	    => v32_CTEValue_s,
		i_OutClk_p 	    => i_SysClk_p,
		ov_OutSig_p 	=> ov32_DiffAcc_p
	);



end rtl;