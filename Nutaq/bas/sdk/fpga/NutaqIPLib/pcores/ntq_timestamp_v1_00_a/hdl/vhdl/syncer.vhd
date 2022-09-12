--
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity syncer is
	port(
		i_InClk_p 	: in std_logic;
		i_InSig_p 	: in std_logic;
		i_OutClk_p 	: in std_logic;
		o_OutSig_p 	: out std_logic
	);
end entity syncer;

architecture rtl of syncer is

 signal InSigClear_s 		: std_logic := '0';
 signal OutSig_s			: std_logic := '0';
 signal OutSigTmp1_s		: std_logic := '0';
 signal OutSigTmp2_s		: std_logic := '0';
 signal InSigClearTmp1_s	: std_logic := '0';
 
  -- Keep constraints (timing constraints outside this code on these signals)
  --
  attribute keep                        : boolean;
  attribute keep of OutSig_s            : signal is true;
  attribute keep of OutSigTmp1_s        : signal is true;
  attribute keep of OutSigTmp2_s        : signal is true;
  attribute keep of InSigClearTmp1_s    : signal is true;
 
begin

 -- Latch input signal and only clear it after it was seen at other clock domain
 process(i_InClk_p)
 begin
 	if rising_edge(i_InClk_p) then 		
 		if i_InSig_p = '1' then
 			OutSig_s <= '1';
 		elsif InSigClear_s = '1' then
 			OutSig_s <= '0';
 		end if; 		
 	end if;
 end process;
  
 -- Double flip-flop
 process(i_OutClk_p)
 begin
 	if rising_edge(i_OutClk_p) then
 		OutSigTmp1_s <= OutSig_s;
 		OutSigTmp2_s <= OutSigTmp1_s;
 	end if;
 end process;

 o_OutSig_p <= OutSigTmp2_s; 
   
 -- Double flip-flop
 process(i_InClk_p)
 begin
 	if rising_edge(i_InClk_p) then
 		InSigClearTmp1_s <= OutSigTmp2_s;
 		InSigClear_s <= InSigClearTmp1_s;
 	end if;
 end process;
 
end rtl;