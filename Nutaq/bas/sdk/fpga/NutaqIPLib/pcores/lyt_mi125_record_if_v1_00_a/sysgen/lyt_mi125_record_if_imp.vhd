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

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity lyt_mi125_record_if_imp is
 	port(
        clk                     : in std_logic;
        ce                      : in std_logic;
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
end lyt_mi125_record_if_imp;

architecture arch of lyt_mi125_record_if_imp is

    component lyt_mi125_record_if is
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
    end component;

begin

    lyt_mi125_record_if_inst : lyt_mi125_record_if
    port map(
 		i_Reset_p			        => '0',
 		i_Clk_p			            => clk,
 		-- MI125 IF --
 		iv14_AdcDataCh1_p	        => iv14_AdcDataCh1_p,
 		iv14_AdcDataCh2_p	        => iv14_AdcDataCh2_p,
 		iv14_AdcDataCh3_p	        => iv14_AdcDataCh3_p,
 		iv14_AdcDataCh4_p	        => iv14_AdcDataCh4_p,
 		iv14_AdcDataCh5_p	        => iv14_AdcDataCh5_p,
 		iv14_AdcDataCh6_p	        => iv14_AdcDataCh6_p,
 		iv14_AdcDataCh7_p	        => iv14_AdcDataCh7_p,
 		iv14_AdcDataCh8_p	        => iv14_AdcDataCh8_p,
 		iv14_AdcDataCh9_p	        => iv14_AdcDataCh9_p,
 		iv14_AdcDataCh10_p	        => iv14_AdcDataCh10_p,
 		iv14_AdcDataCh11_p	        => iv14_AdcDataCh11_p,
 		iv14_AdcDataCh12_p	        => iv14_AdcDataCh12_p,
 		iv14_AdcDataCh13_p	        => iv14_AdcDataCh13_p,
 		iv14_AdcDataCh14_p	        => iv14_AdcDataCh14_p,
 		iv14_AdcDataCh15_p	        => iv14_AdcDataCh15_p,
 		iv14_AdcDataCh16_p	        => iv14_AdcDataCh16_p,
 		iv14_AdcDataCh17_p	        => iv14_AdcDataCh17_p,
 		iv14_AdcDataCh18_p	        => iv14_AdcDataCh18_p,
 		iv14_AdcDataCh19_p	        => iv14_AdcDataCh19_p,
 		iv14_AdcDataCh20_p	        => iv14_AdcDataCh20_p,
 		iv14_AdcDataCh21_p	        => iv14_AdcDataCh21_p,
 		iv14_AdcDataCh22_p	        => iv14_AdcDataCh22_p,
 		iv14_AdcDataCh23_p	        => iv14_AdcDataCh23_p,
 		iv14_AdcDataCh24_p	        => iv14_AdcDataCh24_p,
 		iv14_AdcDataCh25_p	        => iv14_AdcDataCh25_p,
 		iv14_AdcDataCh26_p	        => iv14_AdcDataCh26_p,
 		iv14_AdcDataCh27_p	        => iv14_AdcDataCh27_p,
 		iv14_AdcDataCh28_p	        => iv14_AdcDataCh28_p,
 		iv14_AdcDataCh29_p	        => iv14_AdcDataCh29_p,
 		iv14_AdcDataCh30_p	        => iv14_AdcDataCh30_p,
 		iv14_AdcDataCh31_p	        => iv14_AdcDataCh31_p,
 		iv14_AdcDataCh32_p	        => iv14_AdcDataCh32_p,
 		iv14_AdcDataCh33_p	        => iv14_AdcDataCh33_p,
 		iv14_AdcDataCh34_p	        => iv14_AdcDataCh34_p,
 		iv14_AdcDataCh35_p	        => iv14_AdcDataCh35_p,
 		iv14_AdcDataCh36_p	        => iv14_AdcDataCh36_p,
 		iv14_AdcDataCh37_p	        => iv14_AdcDataCh37_p,
 		iv14_AdcDataCh38_p	        => iv14_AdcDataCh38_p,
 		iv14_AdcDataCh39_p	        => iv14_AdcDataCh39_p,
 		iv14_AdcDataCh40_p	        => iv14_AdcDataCh40_p,
 		iv14_AdcDataCh41_p	        => iv14_AdcDataCh41_p,
 		iv14_AdcDataCh42_p	        => iv14_AdcDataCh42_p,
 		iv14_AdcDataCh43_p	        => iv14_AdcDataCh43_p,
 		iv14_AdcDataCh44_p	        => iv14_AdcDataCh44_p,
 		iv14_AdcDataCh45_p	        => iv14_AdcDataCh45_p,
 		iv14_AdcDataCh46_p	        => iv14_AdcDataCh46_p,
 		iv14_AdcDataCh47_p	        => iv14_AdcDataCh47_p,
 		iv14_AdcDataCh48_p	        => iv14_AdcDataCh48_p,
 		iv14_AdcDataCh49_p	        => iv14_AdcDataCh49_p,
 		iv14_AdcDataCh50_p	        => iv14_AdcDataCh50_p,
 		iv14_AdcDataCh51_p	        => iv14_AdcDataCh51_p,
 		iv14_AdcDataCh52_p	        => iv14_AdcDataCh52_p,
 		iv14_AdcDataCh53_p	        => iv14_AdcDataCh53_p,
 		iv14_AdcDataCh54_p	        => iv14_AdcDataCh54_p,
 		iv14_AdcDataCh55_p	        => iv14_AdcDataCh55_p,
 		iv14_AdcDataCh56_p	        => iv14_AdcDataCh56_p,
 		iv14_AdcDataCh57_p	        => iv14_AdcDataCh57_p,
 		iv14_AdcDataCh58_p	        => iv14_AdcDataCh58_p,
 		iv14_AdcDataCh59_p	        => iv14_AdcDataCh59_p,
 		iv14_AdcDataCh60_p	        => iv14_AdcDataCh60_p,
 		iv14_AdcDataCh61_p	        => iv14_AdcDataCh61_p,
 		iv14_AdcDataCh62_p	        => iv14_AdcDataCh62_p,
 		iv14_AdcDataCh63_p	        => iv14_AdcDataCh63_p,
 		iv14_AdcDataCh64_p	        => iv14_AdcDataCh64_p,
 		i_AdcValid_p	            => i_AdcValid_p,
 		i_DataFormat_p              => i_DataFormat_p,
 		i_SoftOrExternalTriggSel_p  => i_SoftOrExternalTriggSel_p,
 		i_ExternalTrigger_p         => i_ExternalTrigger_p,
 		i_SoftTrigger_p             => i_SoftTrigger_p,
 		iv3_MuxSel_p                => iv3_MuxSel_p,
 		ov16_RecordData1_p	        => ov16_RecordData1_p,
 		ov16_RecordData2_p	        => ov16_RecordData2_p,
 		ov16_RecordData3_p	        => ov16_RecordData3_p,
 		ov16_RecordData4_p	        => ov16_RecordData4_p,
 		ov16_RecordData5_p	        => ov16_RecordData5_p,
 		ov16_RecordData6_p	        => ov16_RecordData6_p,
 		ov16_RecordData7_p	        => ov16_RecordData7_p,
 		ov16_RecordData8_p	        => ov16_RecordData8_p,
 		ov16_RecordData9_p	        => ov16_RecordData9_p,
 		ov16_RecordData10_p	        => ov16_RecordData10_p,
 		ov16_RecordData11_p	        => ov16_RecordData11_p,
 		ov16_RecordData12_p	        => ov16_RecordData12_p,
 		ov16_RecordData13_p	        => ov16_RecordData13_p,
 		ov16_RecordData14_p	        => ov16_RecordData14_p,
 		ov16_RecordData15_p	        => ov16_RecordData15_p,
 		ov16_RecordData16_p	        => ov16_RecordData16_p,
 		o_RecordValid_p		        => o_RecordValid_p,
        o_RecordTrigger_p		    => o_RecordTrigger_p);

 end arch;