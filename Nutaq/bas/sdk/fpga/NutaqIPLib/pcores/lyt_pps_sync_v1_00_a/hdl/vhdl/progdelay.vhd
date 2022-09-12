--
library ieee;
 use ieee.std_logic_1164.all;
 use ieee.numeric_std.all;

library UNISIM;
 use UNISIM.Vcomponents.all;

entity progdelay is
	port(
		i_clk_p			: in std_logic;
		i_reset_p		: in std_logic;
		i_DlyEna_p		: in std_logic;
		iv4_ProgDelay_p : in std_logic_vector(3 downto 0);		
		iv32_InSig_p 	: in std_logic_vector(31 downto 0);		
		ov32_InSig_p 	: out std_logic_vector(31 downto 0)
	);
end entity progdelay;

architecture rtl of progdelay is

 signal resetTmp_s 		: std_logic;
 signal resetPulse_s	: std_logic;
 type rst_fsm_t is (Idle_c, ResetSrl16_c);
 signal rst_fsm_s 		: rst_fsm_t;
 signal u4_RstCnt_s		: unsigned(3 downto 0);
 signal v32_Srl16In_s 	: std_logic_vector(31 downto 0);
 signal DlyEna_s		: std_logic;
 
 
begin


 -- Detect rising edge on reset signal
 process(i_clk_p)
 begin
 	if rising_edge(i_clk_p) then
 		resetTmp_s <= i_reset_p;
 	end if;
 end process;
 
 resetPulse_s <= i_reset_p and not(resetTmp_s);
 
 -- implement SRL16 reset
 process(i_clk_p)
 begin
 	if rising_edge(i_clk_p) then
 		case rst_fsm_s is
 		
 			when Idle_c =>
 				--v32_Srl16In_s <= iv32_InSig_p;
 				if resetPulse_s = '1' then
 					rst_fsm_s <= ResetSrl16_c;
 				end if;
 				
 			when ResetSrl16_c =>
 				if u4_RstCnt_s = x"F" then
 					u4_RstCnt_s <= (others=>'0');
 					rst_fsm_s <= Idle_c;
 				else
 					u4_RstCnt_s <= u4_RstCnt_s + 1;
 					--v32_Srl16In_s <= x"0000_0000";
 				end if;
 				 				
 			when others => null;
 		end case;
 	end if;
 end process;

 v32_Srl16In_s 	<= iv32_InSig_p when rst_fsm_s = Idle_c else (others=>'0');  
 DlyEna_s		<= i_DlyEna_p	when rst_fsm_s = Idle_c else '1';
 
 -- 
 ProgDlyGen: for i in 0 to 31 generate
	 PrgDelay_i : SRL16E
	 generic map (
	 INIT => X"0000")
	 port map (
	 	Q 	=> ov32_InSig_p(i), -- SRL data output
	 	A0 	=> iv4_ProgDelay_p(0), 
	 	A1 	=> iv4_ProgDelay_p(1), 
	 	A2 	=> iv4_ProgDelay_p(2), 
	 	A3 	=> iv4_ProgDelay_p(3), 
	 	CE 	=> DlyEna_s, 
	 	CLK => i_clk_p, 
	 	D 	=> v32_Srl16In_s(i) 
	 	);
 end generate;
 	
end rtl;