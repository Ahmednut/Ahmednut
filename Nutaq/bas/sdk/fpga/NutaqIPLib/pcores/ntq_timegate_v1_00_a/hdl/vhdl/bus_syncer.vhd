--
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

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
-- File : bus_syncer.vhd
--------------------------------------------------------------------------------
-- Description : Give access the the input bus in the output clock domain
--               The value is updated approximately every 
--               6 input clock periods + 4 output clock periods
--               This entity is intended to be used for logic that does not 
--               precise timing but that require that every bit of the bus
--               is coherent with the other.
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2014 Nutaq inc.
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------

entity bus_syncer is
    generic(
        C_WIDTH     : natural := 8
    );
	port(
		i_InClk_p 	    : in std_logic;
		iv_InSig_p 	    : in std_logic_vector(C_WIDTH-1 downto 0);
		i_OutClk_p 	    : in std_logic;
		ov_OutSig_p 	: out std_logic_vector(C_WIDTH-1 downto 0)
	);
end entity bus_syncer;

architecture rtl of bus_syncer is

 signal v_ValueFrozen_s         : std_logic_vector(C_WIDTH-1 downto 0) := (others => '0');

 signal NewValuePending         : std_logic := '0';

 signal NewValuePendingOclkD1_s : std_logic := '0';
 signal NewValuePendingOclkD2_s : std_logic := '0';

 signal NewValuePendingIclkD1_s : std_logic := '0';
 signal NewValuePendingIclkD2_s : std_logic := '0';

  -- Keep constraints (timing constraints outside this code on these signals)
  --
  attribute keep                                : boolean;
  attribute keep of v_ValueFrozen_s             : signal is true;
  attribute keep of NewValuePending             : signal is true;
  attribute keep of NewValuePendingOclkD1_s     : signal is true;
  attribute keep of NewValuePendingOclkD2_s     : signal is true;
  attribute keep of NewValuePendingIclkD1_s     : signal is true;

begin

 -- Latch input signal and only clear it after it was seen at other clock domain
 process(i_InClk_p)
 begin
 	if rising_edge(i_InClk_p) then

        -- If value is not frozen and the OutClk is ready to received the new value
 		if NewValuePending = '0' and NewValuePendingIclkD2_s = '0' then
 			v_ValueFrozen_s     <= iv_InSig_p;
            NewValuePending     <= '1';
 		end if;

        NewValuePendingIclkD1_s <= NewValuePendingOclkD2_s;
        NewValuePendingIclkD2_s <= NewValuePendingIclkD1_s;

        -- The OutClk has seen the value so we can clear the frozen value
        if NewValuePendingIclkD2_s = '1' then
            NewValuePending     <= '0';
        end if;

 	end if;
 end process;


 process(i_OutClk_p)
 begin
 	if rising_edge(i_OutClk_p) then

        NewValuePendingOclkD1_s <= NewValuePending;
        NewValuePendingOclkD2_s <= NewValuePendingOclkD1_s;

        -- Latch value on the failing edge of the new value flag
        if NewValuePendingOclkD2_s = '1' and NewValuePendingOclkD1_s = '0' then
            ov_OutSig_p  <= v_ValueFrozen_s;
        end if;

 	end if;
 end process;

end rtl;