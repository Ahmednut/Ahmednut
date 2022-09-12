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
-- File : ntq_timestamp.vhd
--------------------------------------------------------------------------------
-- Description : Time stamp generator. This core generates either absolute (UTC)
-- or relative time stamp (sample period) time stamp.
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------

library IEEE;
    use IEEE.STD_LOGIC_1164.ALL;
    use IEEE.STD_LOGIC_ARITH.ALL;
    use IEEE.std_logic_unsigned.all;

library ntq_timestamp_v1_00_a;
    use ntq_timestamp_v1_00_a.all;

entity ntq_timestamp is
    generic
    (
    v16_CoreId_g 	: std_logic_vector(15 downto 0) := x"CC03";
    v16_VersNbr_g	: std_logic_vector(15 downto 0) := x"0100"
    );
    port
    (
    i_SysClk_p              : in  std_logic;    -- Microblaze system clock: 100 Mhz
    i_UserClk_p             : in  std_logic;    -- GPS disciplined user clock
    -- Regs CTRL & Status
    i_Pps_p                 : in  std_logic;
    ov32_CoreIdVers_p       : out std_logic_vector(31 downto 0);
    i_CoreReset_p           : in  std_logic;
    i_SetTime_p             : in  std_logic;
    i_LatchTime_p           : in  std_logic;
    iv64_TimeStamp_p        : in  std_logic_vector(63 downto 0);    -- Timestamp value register when set time
    ov64_UserTimeStamp_p    : out std_logic_vector(63 downto 0);    -- Timestamp in user clock domain
    ov64_SysTimeStamp_p     : out std_logic_vector(63 downto 0)     -- Timestamp in system clock domain
    );
end ntq_timestamp;

architecture Behavioral of ntq_timestamp is

    signal pps_r1_s                     : std_logic;
    signal CoreReset_s                  : std_logic;
    signal SetTime_s                    : std_logic;
    signal SetTime_r1_s                 : std_logic;
    signal LatchTime_s                  : std_logic;
    signal LatchTime_r1_s               : std_logic;
    signal v64_TimeStampReg_s           : std_logic_vector(63 downto 0);
    signal v64_TimeStampRegNewVal_s     : std_logic_vector(63 downto 0);
    signal v64_TimeStampLatch_s         : std_logic_vector(63 downto 0);
    signal next_pps_s                   : std_logic;
    signal counting_s                   : std_logic :='1';
    
    attribute keep : string;
    attribute keep of v64_TimeStampLatch_s : signal is "true";

begin

    ---------------------------------------------------------
    -- Sync ctrl signals with design/FMC clock
    ---------------------------------------------------------
    Syncer_i0: entity ntq_timestamp_v1_00_a.syncer
        port map(
            i_InClk_p     => i_SysClk_p,
            i_InSig_p     => i_SetTime_p,
            i_OutClk_p     => i_UserClk_p,
            o_OutSig_p     => SetTime_s
        );

    Syncer_i1: entity ntq_timestamp_v1_00_a.syncer
        port map(
            i_InClk_p     => i_SysClk_p,
            i_InSig_p     => i_CoreReset_p,
            i_OutClk_p     => i_UserClk_p,
            o_OutSig_p     => CoreReset_s
        );

    Syncer_i2: entity ntq_timestamp_v1_00_a.syncer
        port map(
            i_InClk_p     => i_SysClk_p,
            i_InSig_p     => i_LatchTime_p,
            i_OutClk_p     => i_UserClk_p,
            o_OutSig_p     => LatchTime_s
        );

    -- Timestamp process
    timestamp_proc:process(i_UserClk_p)
    begin
      if rising_edge(i_UserClk_p) then

        -- Save last set time state to detect rising edge
        pps_r1_s            <= i_Pps_p;
        SetTime_r1_s        <= SetTime_s;
        LatchTime_r1_s      <= LatchTime_s;

        -- Increment the current time when counting is enabled
        if counting_s = '1'then
            v64_TimeStampReg_s  <= v64_TimeStampReg_s + 1;
        end if;

        -- Latch time on next PPS rising edge
        -- and start counting
        if (i_Pps_p = '1' and pps_r1_s = '0') and next_pps_s = '1' then
            v64_TimeStampReg_s  <= v64_TimeStampRegNewVal_s;
            next_pps_s          <= '0';
            counting_s          <= '1';
        end if;

        -- Set the time at the next PPS rising edge,
        -- reset the current counter to 0 and stop counting
        if SetTime_s = '1' and SetTime_r1_s = '0' then
            v64_TimeStampRegNewVal_s    <= iv64_TimeStamp_p;
            v64_TimeStampReg_s          <= (others => '0');
            next_pps_s                  <= '1';
            counting_s                  <= '0';
        end if;

        -- Reset the counter to 0 and start counting after the reset
        if CoreReset_s = '1' then
            v64_TimeStampReg_s <= (others => '0');
            next_pps_s <= '0';
            counting_s <= '1';
        end if;

      end if;
    end process;

    -- Timestamp process
    latch_proc:process(i_UserClk_p) 
    begin
        if rising_edge(i_UserClk_p) then
            if LatchTime_r1_s = '1' and LatchTime_s = '0' then
                v64_TimeStampLatch_s <= v64_TimeStampReg_s;
            end if;
        end if;
    end process;

    --Output ports
    ov32_CoreIdVers_p       <= v16_CoreId_g & v16_VersNbr_g;
    ov64_UserTimeStamp_p    <= v64_TimeStampReg_s;
    ov64_SysTimeStamp_p     <= v64_TimeStampLatch_s;

end Behavioral;