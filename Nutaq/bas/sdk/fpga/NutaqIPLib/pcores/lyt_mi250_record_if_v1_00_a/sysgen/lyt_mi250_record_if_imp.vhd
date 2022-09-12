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

entity lyt_mi250_record_if_imp is
 	port(
        clk                     : in std_logic;
        ce                      : in std_logic;
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
end lyt_mi250_record_if_imp;

architecture arch of lyt_mi250_record_if_imp is

    component lyt_mi250_record_if is
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
    end component;

begin

    lyt_mi250_record_if_inst : lyt_mi250_record_if
    port map(
 		i_Reset_p			        => '0',
 		i_Clk_p			            => clk,
        
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
        i_AdcValid_p	            => i_AdcValid_p,
        i_SoftOrExternalTriggSel_p  => i_SoftOrExternalTriggSel_p,
        i_ExternalTrigger_p         => i_ExternalTrigger_p,
        i_SoftTrigger_p             => i_SoftTrigger_p,
        iv2_MuxSel_p                => iv2_MuxSel_p,
        ov16_RecordData1_p	        => ov16_RecordData1_p,
        ov16_RecordData2_p	        => ov16_RecordData2_p,
        ov16_RecordData3_p	        => ov16_RecordData3_p,
        ov16_RecordData4_p	        => ov16_RecordData4_p,
        ov16_RecordData5_p	        => ov16_RecordData5_p,
        ov16_RecordData6_p	        => ov16_RecordData6_p,
        ov16_RecordData7_p	        => ov16_RecordData7_p,
        ov16_RecordData8_p	        => ov16_RecordData8_p,
        o_RecordValid_p		        => o_RecordValid_p,
        o_RecordTrigger_p		    => o_RecordTrigger_p
    );

 end arch;