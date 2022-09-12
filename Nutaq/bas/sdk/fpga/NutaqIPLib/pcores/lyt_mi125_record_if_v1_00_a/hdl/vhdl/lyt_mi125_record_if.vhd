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
-- Notes / Assumptions :
-- Description: MI125 data interface to LyrtechRD Record-Playback ip core
-- This module is used to pack the MI125 32 channels into the record IP. Since the
-- DDR3 can't keep with the throughput required for all 32 channels at 125MHz, this
-- module will record :
-- A- If iv2_MuxSel_p(1) = 0 only 16 channels (up or bottom) depending on
--    the i_BottomTopMuxSel_p input (if 0, the 16 bottom channels are recorded, if 1
--    the 16 top module channels are recorded).
-- B- If iv2_MuxSel_p(0) = 1 the 32 channels (up and bottom) downsampled by
--    2. This component will get a sample from 16 channels bottom then a sample from 16
--    channels top and so on.
-- First version by Abdelkarim Ouadid
-- 2012/08
-- Major mofidications by Julien Roy
-- 2012/11
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: lyt_mi125_record_if.vhd,v $
-- Revision 1.2  2015/07/03 19:25:22  julien.roy
-- Add 64 channels support
--
-- Revision 1.1  2015/04/10 19:49:00  sqa
-- bas first commit
--
-- Revision 1.3  2013/04/12 14:44:38  julien.roy
-- Disable "keep_hierarchy"
--
-- Revision 1.2  2012/12/10 14:26:26  julien.roy
-- Add adc chip enabled port,
-- delay trigger in downsampled mode instead of resetting the data sequence,
-- lighter code
--
--
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lyt_mi125_record_if is
 	port(
 		i_Reset_p			    : in std_logic;
 		i_Clk_p			        : in std_logic;
 		-- MI125 IF --
 		iv14_AdcDataCh1_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh2_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh3_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh4_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh5_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh6_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh7_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh8_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh9_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh10_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh11_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh12_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh13_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh14_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh15_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh16_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh17_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh18_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh19_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh20_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh21_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh22_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh23_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh24_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh25_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh26_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh27_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh28_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh29_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh30_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh31_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh32_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh33_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh34_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh35_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh36_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh37_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh38_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh39_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh40_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh41_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh42_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh43_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh44_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh45_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh46_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh47_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh48_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh49_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh50_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh51_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh52_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh53_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh54_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh55_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh56_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh57_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh58_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh59_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh60_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh61_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh62_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh63_p	    : in std_logic_vector(13 downto 0);
 		iv14_AdcDataCh64_p	    : in std_logic_vector(13 downto 0);
 		i_AdcValid_p	        : in std_logic;
 		i_DataFormat_p          : in std_logic;
 		-- Trigger IF --
 		i_SoftOrExternalTriggSel_p : in std_logic;
 		i_ExternalTrigger_p     : in std_logic;
 		i_SoftTrigger_p         : in std_logic;
 		-- Control IF --
 		iv3_MuxSel_p            : in std_logic_vector(2 downto 0);
 		-- Record IF --
 		ov16_RecordData1_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData2_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData3_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData4_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData5_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData6_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData7_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData8_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData9_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData10_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData11_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData12_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData13_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData14_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData15_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData16_p	    : out std_logic_vector(15 downto 0);
 		o_RecordValid_p		    : out std_logic;
        o_RecordTrigger_p		: out std_logic
  );
end lyt_mi125_record_if;

architecture arch of lyt_mi125_record_if is

  -- attribute keep_hierarchy : string;
  -- attribute keep_hierarchy of lyt_mi125_record_if_behav : architecture is "true";

  --------------------------------------------------------------------------
  -- Component declaration
  --------------------------------------------------------------------------

  ---------------------------------------------------------------------------------
  --     ************** Function declaration *******************                 --
  -- Return a std_logic_vector with a sign extension depending on the data format--
  ---------------------------------------------------------------------------------
  function SignExtend(val: std_logic_vector(13 downto 0);DataFormat:std_logic) return std_logic_vector is
  variable tmp: std_logic_vector(15 downto 0);
  begin
     if DataFormat='1' then
        tmp:= val(13) & val(13) & val;
      else
        tmp:='0'&'0'&val;
      end if;
    return tmp;
  end SignExtend;

  --------------------------------------------------------------------------
  -- signals
  --------------------------------------------------------------------------
  signal v3_MuxSel_R1_s                 : std_logic_vector(2 downto 0);
  signal v3_MuxSel_R2_s                 : std_logic_vector(2 downto 0);

  signal v256_AdcData_Ch1_Ch16_s        : std_logic_vector(255 downto 0);
  signal v256_AdcData_Ch17_Ch32_s       : std_logic_vector(255 downto 0);
  signal v256_AdcData_Ch33_Ch48_s       : std_logic_vector(255 downto 0);
  signal v256_AdcData_Ch49_Ch64_s       : std_logic_vector(255 downto 0);

  signal v256_AdcData_Ch17_Ch32_R1_s    : std_logic_vector(255 downto 0);
  signal v256_AdcData_Ch33_Ch48_R1_s    : std_logic_vector(255 downto 0);
  signal v256_AdcData_Ch49_Ch64_R1_s    : std_logic_vector(255 downto 0);

  signal v256_AdcData_Ch33_Ch48_R2_s    : std_logic_vector(255 downto 0);
  signal v256_AdcData_Ch49_Ch64_R2_s    : std_logic_vector(255 downto 0);

  signal v256_AdcData_Ch49_Ch64_R3_s    : std_logic_vector(255 downto 0);

  signal u2_DownSampleState_s           : unsigned(1 downto 0);

  signal v256_RecordData_s              : std_logic_vector(255 downto 0);

begin


    v256_AdcData_Ch1_Ch16_s <=
        SignExtend(iv14_AdcDataCh16_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh15_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh14_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh13_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh12_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh11_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh10_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh9_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh8_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh7_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh6_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh5_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh4_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh3_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh2_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh1_p,i_DataFormat_p);

    v256_AdcData_Ch17_Ch32_s <=
        SignExtend(iv14_AdcDataCh32_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh31_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh30_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh29_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh28_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh27_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh26_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh25_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh24_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh23_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh22_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh21_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh20_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh19_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh18_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh17_p,i_DataFormat_p);

    v256_AdcData_Ch33_Ch48_s <=
        SignExtend(iv14_AdcDataCh48_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh47_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh46_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh45_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh44_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh43_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh42_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh41_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh40_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh39_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh38_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh37_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh36_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh35_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh34_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh33_p,i_DataFormat_p);

    v256_AdcData_Ch49_Ch64_s <=
        SignExtend(iv14_AdcDataCh64_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh63_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh62_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh61_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh60_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh59_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh58_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh57_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh56_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh55_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh54_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh53_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh52_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh51_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh50_p,i_DataFormat_p) &
        SignExtend(iv14_AdcDataCh49_p,i_DataFormat_p);


    -- When alternating between FMC board samples
    -- only trig when sending the first FMC board samples. This allows
    -- the user to identify the first 256 bits received from
    -- RTDEx to be the first FMC board samples.

    process(i_Clk_p)
        variable ExternalTrigger_r1   : std_logic;
        variable SoftTrigger_r1       : std_logic;
        variable SoftTrigger_r2       : std_logic;
        variable SoftTrigger_r3       : std_logic;
        variable TriggerPulse         : std_logic;
        variable TriggerPulse_r1      : std_logic;
        variable TriggerPulse_r2      : std_logic;
        variable TriggerPulse_r3      : std_logic;
    begin
        if rising_edge(i_Clk_p) then

            -- Double latch input data
            v3_MuxSel_R1_s <= iv3_MuxSel_p;
            v3_MuxSel_R2_s <= v3_MuxSel_R1_s;

            -- Latch Top data for synchronization with bottom data
            -- in downsample mode
            v256_AdcData_Ch17_Ch32_R1_s  <= v256_AdcData_Ch17_Ch32_s;
            v256_AdcData_Ch33_Ch48_R1_s  <= v256_AdcData_Ch33_Ch48_s;
            v256_AdcData_Ch49_Ch64_R1_s  <= v256_AdcData_Ch49_Ch64_s;

            v256_AdcData_Ch33_Ch48_R2_s  <= v256_AdcData_Ch33_Ch48_R1_s;
            v256_AdcData_Ch49_Ch64_R2_s  <= v256_AdcData_Ch49_Ch64_R1_s;

            v256_AdcData_Ch49_Ch64_R3_s  <= v256_AdcData_Ch49_Ch64_R2_s;

            -- Trigger selection and edge detection
            if i_SoftOrExternalTriggSel_p = '1' then
                TriggerPulse := i_ExternalTrigger_p and (not ExternalTrigger_r1);
            else
                TriggerPulse := SoftTrigger_r2 and (not SoftTrigger_r3);
            end if;
            
            o_RecordValid_p <= i_AdcValid_p;

            -- Select MI125 data
            case v3_MuxSel_R2_s is

                when "000" =>
                    v256_RecordData_s <= v256_AdcData_Ch1_Ch16_s;
                    o_RecordTrigger_p <= TriggerPulse;

                when "001" =>
                    v256_RecordData_s <= v256_AdcData_Ch17_Ch32_s;
                    o_RecordTrigger_p <= TriggerPulse;

                when "010" =>
                    v256_RecordData_s <= v256_AdcData_Ch33_Ch48_s;
                    o_RecordTrigger_p <= TriggerPulse;

                when "011" =>
                    v256_RecordData_s <= v256_AdcData_Ch49_Ch64_s;
                    o_RecordTrigger_p <= TriggerPulse;

                when "100" =>
                    if u2_DownSampleState_s(0) = '0' then
                        v256_RecordData_s <= v256_AdcData_Ch1_Ch16_s;
                        o_RecordTrigger_p <= TriggerPulse or TriggerPulse_r1;
                    else
                        v256_RecordData_s <= v256_AdcData_Ch17_Ch32_R1_s;
                        o_RecordTrigger_p <= '0';
                    end if;

                when "101" =>
                    if u2_DownSampleState_s(0) = '0' then
                        v256_RecordData_s <= v256_AdcData_Ch33_Ch48_s;
                        o_RecordTrigger_p <= TriggerPulse or TriggerPulse_r1;
                    else
                        v256_RecordData_s <= v256_AdcData_Ch49_Ch64_R1_s;
                        o_RecordTrigger_p <= '0';
                    end if;

                when "110" =>
                    if u2_DownSampleState_s = "00" then
                        v256_RecordData_s <= v256_AdcData_Ch1_Ch16_s;
                        o_RecordTrigger_p <= TriggerPulse or TriggerPulse_r1 or
                                             TriggerPulse_r2 or TriggerPulse_r3;
                    elsif u2_DownSampleState_s = "01" then
                        v256_RecordData_s <= v256_AdcData_Ch17_Ch32_R1_s;
                        o_RecordTrigger_p <= '0';
                    elsif u2_DownSampleState_s = "10" then
                        v256_RecordData_s <= v256_AdcData_Ch33_Ch48_R2_s;
                        o_RecordTrigger_p <= '0';
                    else
                        v256_RecordData_s <= v256_AdcData_Ch49_Ch64_R3_s;
                        o_RecordTrigger_p <= '0';
                    end if;

                when others =>
                    v256_RecordData_s <= v256_AdcData_Ch1_Ch16_s;
                    o_RecordTrigger_p <= TriggerPulse;

            end case;

            u2_DownSampleState_s <= u2_DownSampleState_s + 1;

            -- Register section ----------
            ExternalTrigger_r1  := i_ExternalTrigger_p;
            SoftTrigger_r3      := SoftTrigger_r2;
            SoftTrigger_r2      := SoftTrigger_r1;
            SoftTrigger_r1      := i_SoftTrigger_p;
            TriggerPulse_r3     := TriggerPulse_r2;
            TriggerPulse_r2     := TriggerPulse_r1;
            TriggerPulse_r1     := TriggerPulse;

            -- Reset section -------------
            if i_Reset_p = '1' then
                u2_DownSampleState_s    <= "00";
                o_RecordValid_p         <= '0';

                ExternalTrigger_r1      := '0';
                SoftTrigger_r1          := '0';
                SoftTrigger_r2          := '0';
                SoftTrigger_r3          := '0';
                TriggerPulse_r1         := '0';
                TriggerPulse_r2         := '0';
                TriggerPulse_r3         := '0';
                o_RecordTrigger_p       <= '0';
            end if;

        end if;
    end process;

    ov16_RecordData1_p  <= v256_RecordData_s(16*1-1  downto 16*0);
    ov16_RecordData2_p  <= v256_RecordData_s(16*2-1  downto 16*1);
    ov16_RecordData3_p  <= v256_RecordData_s(16*3-1  downto 16*2);
    ov16_RecordData4_p  <= v256_RecordData_s(16*4-1  downto 16*3);
    ov16_RecordData5_p  <= v256_RecordData_s(16*5-1  downto 16*4);
    ov16_RecordData6_p  <= v256_RecordData_s(16*6-1  downto 16*5);
    ov16_RecordData7_p  <= v256_RecordData_s(16*7-1  downto 16*6);
    ov16_RecordData8_p  <= v256_RecordData_s(16*8-1  downto 16*7);
    ov16_RecordData9_p  <= v256_RecordData_s(16*9-1  downto 16*8);
    ov16_RecordData10_p <= v256_RecordData_s(16*10-1 downto 16*9);
    ov16_RecordData11_p <= v256_RecordData_s(16*11-1 downto 16*10);
    ov16_RecordData12_p <= v256_RecordData_s(16*12-1 downto 16*11);
    ov16_RecordData13_p <= v256_RecordData_s(16*13-1 downto 16*12);
    ov16_RecordData14_p <= v256_RecordData_s(16*14-1 downto 16*13);
    ov16_RecordData15_p <= v256_RecordData_s(16*15-1 downto 16*14);
    ov16_RecordData16_p <= v256_RecordData_s(16*16-1 downto 16*15);

 end arch;