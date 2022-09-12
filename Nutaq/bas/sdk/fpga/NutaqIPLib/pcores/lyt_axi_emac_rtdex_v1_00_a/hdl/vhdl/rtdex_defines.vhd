--------------------------------------------------------------------------------
--
--
--          **  **     **  ******  ********  ********  ********  **    **
--         **    **   **  **   ** ********  ********  ********  **    **
--        **     *****   **   **    **     **        **        **    **
--       **       **    ******     **     ****      **        ********
--      **       **    **  **     **     **        **        **    **
--     *******  **    **   **    **     ********  ********  **    **
--    *******  **    **    **   **     ********  ********  **    **
--
--                       L Y R T E C H   R D   I N C
--
--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
-- Khalid Bensadek 
-- 2012/05
-- File that contains only common constants.
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: 
--------------------------------------------------------------------------------
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package rtdex_defines is

-------------------------------------------------------------------
---- Constants definition ---
-------------------------------------------------------------------
-- Ethernet header constants --
constant v16_SnapId_c    	: std_logic_vector(15 downto 0)  := x"AAAA";
constant v4_Ctrl_c       	: std_logic_vector(7 downto 0)   := x"03";
constant v24_OrgId_c     	: std_logic_vector(23 downto 0)  := x"00D0CC";
constant v16_UserId_c    	: std_logic_vector(15 downto 0)  := x"0010";
constant v8_ProtocolId_c 	: std_logic_vector(7 downto 0)   := x"01";
constant v16_RegFrmType_c  	: std_logic_vector(15 downto 0)  := x"0600";
constant v16_MacCtrlType_c 	: std_logic_vector(15 downto 0)  := x"8808"; 
constant v16_CtrlOpcode_c 	: std_logic_vector(15 downto 0)  := x"0001";
constant v16_JumboFrmType_c : std_logic_vector(15 downto 0)  := x"8870";
constant v16_SubHeaderSize_s : std_logic_vector(15 downto 0) := std_logic_vector(to_unsigned(18,16));

constant v16_PauseMaxVal_c 		: std_logic_vector(15 downto 0)  := x"FFFF";
constant v16_PauseCancelVal_c 	: std_logic_vector(15 downto 0)  := x"0000";
constant iv48_MulticastMacAddr_c : std_logic_vector(47 downto 0)  := x"01_80_C2_00_00_01";
--constant iv48_MulticastMacAddr_c : std_logic_vector(47 downto 0)  := x"00_1B_21_6C_0F_93";

-- Minimume Frame size that will be concedered jumbo frame. According to IEEE = 1500. We're multiple of 64 bytes, 1472 is our max none jumbo frames:
constant JumboFramesMinSize_c : std_logic_vector(14 downto 0) := std_logic_vector(to_unsigned(1472,15));

-- RTDEx Modes Constants
constant SingleMode_c 		: std_logic:='0';
constant ContinuousMode_c 	: std_logic:='1';

-- Xilinx Tx flag word constant for TEMAC.
constant v32_TxcFlagWord_c    : std_logic_vector(31 downto 0):=x"A0000000";
constant v32_TxEmptyAppWord_c : std_logic_vector(31 downto 0):=x"00000000";
constant v32_TxFCSAppWord_c : std_logic_vector(31 downto 0):=x"00000002";

-- Misc --
constant MaxChannelNb_c : integer :=8;


--------------------------------------------------------------------------
-- Data Types for arrays
--------------------------------------------------------------------------
type array8_u32_t is array(7 downto 0) of unsigned(31 downto 0);
type array8_u14_t is array(7 downto 0) of unsigned(13 downto 0);
type array8_u15_t is array(7 downto 0) of unsigned(14 downto 0);
type array8_u16_t is array(7 downto 0) of unsigned(15 downto 0);
type array8_u24_t is array(7 downto 0) of unsigned(23 downto 0);
type array8_v11_t is array(7 downto 0) of std_logic_vector(10 downto 0);
type array8_v15_t is array(7 downto 0) of std_logic_vector(14 downto 0);
type array8_v16_t is array(7 downto 0) of std_logic_vector(15 downto 0);
type array8_v24_t is array(7 downto 0) of std_logic_vector(23 downto 0);
type array8_v32_t is array(7 downto 0) of std_logic_vector(31 downto 0);
type array8_v14_t is array(7 downto 0) of std_logic_vector(13 downto 0);
type array8_v256_t is array(7 downto 0) of std_logic_vector(255 downto 0);

end rtdex_defines;