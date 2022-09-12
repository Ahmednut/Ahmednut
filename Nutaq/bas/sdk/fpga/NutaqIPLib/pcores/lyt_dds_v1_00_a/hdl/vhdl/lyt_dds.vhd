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
-- File : lyt_dds.vhd
--------------------------------------------------------------------------------
-- Description : DDS for adac250
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_dds.vhd,v $
-- Revision 1.3  2015/07/08 13:28:57  christian.dufour-bou
-- Data valid added
--
-- Revision 1.2  2015/05/20 14:01:55  julien.roy
-- Merge modifications from Perseus611X branch
--
-- Revision 1.1  2014/09/03 18:22:50  julien.roy
-- Add generic DDS pcore
--
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;

entity lyt_dds is
  generic (
    C_OUTPUT_WIDTH      : integer := 16
  );
  port (
    i_Clk_p             : in std_logic;
    i_Rst_p             : in std_logic;
    i_ClkEn_p           : in std_logic;
    iv32_Data_p         : in std_logic_vector(31 downto 0);
    i_WriteEn_p         : in std_logic;
    ov_Cosine_p         : out std_logic_vector(C_OUTPUT_WIDTH-1 downto 0);
    ov_Sine_p           : out std_logic_vector(C_OUTPUT_WIDTH-1 downto 0);
    o_DataValid_p       : out std_logic
  );
end entity lyt_dds;

architecture rtl of lyt_dds is

    component dds_core is
    port (
        ce: in std_logic;
        clk: in std_logic;
        sclr: in std_logic;
        we: in std_logic;
        data: in std_logic_vector(31 downto 0);
        cosine: out std_logic_vector(15 downto 0);
        sine: out std_logic_vector(15 downto 0));
    end component dds_core;

    component dds_12bit_p32b is
    port (
        ce: in std_logic;
        clk: in std_logic;
        sclr: in std_logic;
        we: in std_logic;
        data: in std_logic_vector(31 downto 0);
        cosine: out std_logic_vector(11 downto 0);
        sine: out std_logic_vector(11 downto 0));
    end component dds_12bit_p32b;
	
	signal DataValid_s : std_logic;
	
begin
    
    dds12_gen : if (C_OUTPUT_WIDTH = 12) generate
        dds_core_inst : dds_12bit_p32b
        port map (
            ce              => i_ClkEn_p,
            clk             => i_Clk_p,
            sclr            => i_Rst_p,
            we              => i_WriteEn_p,
            data            => iv32_Data_p,
            cosine          => ov_Cosine_p,
            sine            => ov_Sine_p
        );
    end generate;

    dds16_gen : if (C_OUTPUT_WIDTH /= 12) generate
        dds_core_inst : dds_core
        port map (
            ce              => i_ClkEn_p,
            clk             => i_Clk_p,
            sclr            => i_Rst_p,
            we              => i_WriteEn_p,
            data            => iv32_Data_p,
            cosine          => ov_Cosine_p,
            sine            => ov_Sine_p
        );
    end generate;
    
    o_DataValid_p <= DataValid_s;

    DataValid_prc : process(i_Clk_p)
    begin
        if rising_edge(i_Clk_p) then
            if i_ClkEn_p ='1' then
                DataValid_s <= '1';
            else
                DataValid_s <= '0';
            end if;
        end if;
    end process DataValid_prc;
  
end architecture rtl;

