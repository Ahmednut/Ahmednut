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
-- Description: mi250 data interface to Record-Playback ip core
-- This module is used to pack the mi250 channels into the record IP. Since the
-- DDR3 can't keep with the throughput required for all 16 channels at 250MHz, this
-- module will record :
--  - If iv2_MuxSel_p == 0 only first 8 channels (1-8) are recorded.
--  - If iv2_MuxSel_p == 1 only last  8 channels (9-16) are recorded.
--  - If iv2_MuxSel_p == 2 the 16 channels downsampled by 2 are recorded.
--------------------------------------------------------------------------------
-- Copyright (c) 2015 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_mi250_record_if.vhd,v $
-- Revision 1.2  2015/06/25 13:33:56  julien.roy
-- Now support 2 MI250 boards
--
--
--
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lyt_mi250_record_if is
 	port(
 		i_Reset_p			    : in std_logic;
 		i_Clk_p			        : in std_logic;
 		-- mi250 IF --
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
 		i_AdcValid_p	        : in std_logic;
 		-- Trigger IF --
 		i_SoftOrExternalTriggSel_p : in std_logic;
 		i_ExternalTrigger_p     : in std_logic;
 		i_SoftTrigger_p         : in std_logic;
 		-- Control IF --
 		iv2_MuxSel_p            : in std_logic_vector(1 downto 0);
 		-- Record IF --
 		ov16_RecordData1_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData2_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData3_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData4_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData5_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData6_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData7_p	    : out std_logic_vector(15 downto 0);
 		ov16_RecordData8_p	    : out std_logic_vector(15 downto 0);
 		o_RecordValid_p		    : out std_logic;
        o_RecordTrigger_p		: out std_logic
  );
end lyt_mi250_record_if;

architecture arch of lyt_mi250_record_if is

  -- attribute keep_hierarchy : string;
  -- attribute keep_hierarchy of lyt_mi250_record_if_behav : architecture is "true";

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
  signal v2_MuxSel_R1_s                 : std_logic_vector(1 downto 0);
  signal v2_MuxSel_R2_s                 : std_logic_vector(1 downto 0);

  signal v128_AdcData_Ch1_Ch8_s         : std_logic_vector(127 downto 0);
  signal v128_AdcData_Ch9_Ch16_s        : std_logic_vector(127 downto 0);

  signal v128_AdcData_Ch9_Ch16_R1_s     : std_logic_vector(127 downto 0);

  signal u2_DownSampleState_s           : unsigned(1 downto 0);

  signal v128_RecordData_s              : std_logic_vector(127 downto 0);

begin


    v128_AdcData_Ch1_Ch8_s <=
        SignExtend(iv14_AdcDataCh8_p,'1') &
        SignExtend(iv14_AdcDataCh7_p,'1') &
        SignExtend(iv14_AdcDataCh6_p,'1') &
        SignExtend(iv14_AdcDataCh5_p,'1') &
        SignExtend(iv14_AdcDataCh4_p,'1') &
        SignExtend(iv14_AdcDataCh3_p,'1') &
        SignExtend(iv14_AdcDataCh2_p,'1') &
        SignExtend(iv14_AdcDataCh1_p,'1');

    v128_AdcData_Ch9_Ch16_s <=
        SignExtend(iv14_AdcDataCh16_p,'1') &
        SignExtend(iv14_AdcDataCh15_p,'1') &
        SignExtend(iv14_AdcDataCh14_p,'1') &
        SignExtend(iv14_AdcDataCh13_p,'1') &
        SignExtend(iv14_AdcDataCh12_p,'1') &
        SignExtend(iv14_AdcDataCh11_p,'1') &
        SignExtend(iv14_AdcDataCh10_p,'1') &
        SignExtend(iv14_AdcDataCh9_p,'1');


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
    begin
        if rising_edge(i_Clk_p) then

            -- Double latch input data
            v2_MuxSel_R1_s <= iv2_MuxSel_p;
            v2_MuxSel_R2_s <= v2_MuxSel_R1_s;

            -- Latch Top data for synchronization with bottom data
            -- in downsample mode
            v128_AdcData_Ch9_Ch16_R1_s  <= v128_AdcData_Ch9_Ch16_s;

            -- Trigger selection and edge detection
            if i_SoftOrExternalTriggSel_p = '1' then
                TriggerPulse := i_ExternalTrigger_p and (not ExternalTrigger_r1);
            else
                TriggerPulse := SoftTrigger_r2 and (not SoftTrigger_r3);
            end if;
            
            o_RecordValid_p <= i_AdcValid_p;

            -- Select mi250 data
            case v2_MuxSel_R2_s is

                when "00" =>
                    v128_RecordData_s <= v128_AdcData_Ch1_Ch8_s;
                    o_RecordTrigger_p <= TriggerPulse;

                when "01" =>
                    v128_RecordData_s <= v128_AdcData_Ch9_Ch16_s;
                    o_RecordTrigger_p <= TriggerPulse;

                when "10" =>
                    if u2_DownSampleState_s(0) = '0' then
                        v128_RecordData_s <= v128_AdcData_Ch1_Ch8_s;
                        o_RecordTrigger_p <= TriggerPulse or TriggerPulse_r1;
                    else
                        v128_RecordData_s <= v128_AdcData_Ch9_Ch16_R1_s;
                        o_RecordTrigger_p <= '0';
                    end if;

                when others =>
                    v128_RecordData_s <= v128_AdcData_Ch1_Ch8_s;
                    o_RecordTrigger_p <= TriggerPulse;

            end case;

            u2_DownSampleState_s <= u2_DownSampleState_s + 1;

            -- Register section ----------
            ExternalTrigger_r1  := i_ExternalTrigger_p;
            SoftTrigger_r3      := SoftTrigger_r2;
            SoftTrigger_r2      := SoftTrigger_r1;
            SoftTrigger_r1      := i_SoftTrigger_p;
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
                o_RecordTrigger_p       <= '0';
            end if;

        end if;
    end process;

    ov16_RecordData1_p  <= v128_RecordData_s(16*1-1  downto 16*0);
    ov16_RecordData2_p  <= v128_RecordData_s(16*2-1  downto 16*1);
    ov16_RecordData3_p  <= v128_RecordData_s(16*3-1  downto 16*2);
    ov16_RecordData4_p  <= v128_RecordData_s(16*4-1  downto 16*3);
    ov16_RecordData5_p  <= v128_RecordData_s(16*5-1  downto 16*4);
    ov16_RecordData6_p  <= v128_RecordData_s(16*6-1  downto 16*5);
    ov16_RecordData7_p  <= v128_RecordData_s(16*7-1  downto 16*6);
    ov16_RecordData8_p  <= v128_RecordData_s(16*8-1  downto 16*7);

 end arch;