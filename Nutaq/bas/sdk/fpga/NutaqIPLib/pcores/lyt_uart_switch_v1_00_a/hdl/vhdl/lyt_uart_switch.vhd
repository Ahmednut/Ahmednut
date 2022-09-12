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
-- File : lyt_uart_switch.vhd
--------------------------------------------------------------------------------
-- Description : Uart switch for multiple uart ports
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_uart_switch.vhd,v $
-- Revision 1.2  2015/05/20 14:05:42  julien.roy
-- Merge modifications from Perseus611X branch
--
-- Revision 1.1  2013/05/09 12:37:59  julien.roy
-- Add uart switch pcore to support both UART pins
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

Library UNISIM;
use UNISIM.vcomponents.all;

entity lyt_uart_switch is
  port (
    iv2_SelectMode_p            : in std_logic_vector(1 downto 0);
    i_DaughterAbsent_p          : in std_logic := '1';
    i_RTMAbsent_p               : in std_logic := '1';

    i_Daughter_RX_p             : in std_logic := '1';
    io_Daughter_TX_p_I          : in std_logic;
    io_Daughter_TX_p_O          : out std_logic;
    io_Daughter_TX_p_T          : out std_logic;

    i_AMC_RX_p                  : in std_logic := '1';
    io_AMC_TX_p_I               : in std_logic;
    io_AMC_TX_p_O               : out std_logic;
    io_AMC_TX_p_T               : out std_logic;

    i_RTM_RX_p                  : in std_logic := '1';
    io_RTM_TX_p_I               : in std_logic;
    io_RTM_TX_p_O               : out std_logic;
    io_RTM_TX_p_T               : out std_logic;

    o_AXI_RX_p                  : out std_logic;
    i_AXI_TX_p                  : in std_logic
  );
end entity lyt_uart_switch;


architecture rtl of lyt_uart_switch is
begin

    process(iv2_SelectMode_p, i_DaughterAbsent_p, i_RTMAbsent_p, i_Daughter_RX_p, i_AMC_RX_p, i_RTM_RX_p)
    begin
        if (iv2_SelectMode_p = "00") then
            if i_DaughterAbsent_p = '0' and i_RTMAbsent_p = '0' then
                o_AXI_RX_p          <= i_Daughter_RX_p and i_AMC_RX_p and i_RTM_RX_p;
                io_Daughter_TX_p_T  <= '0';
                io_AMC_TX_p_T       <= '0';
                io_RTM_TX_p_T       <= '0';
            
            elsif i_DaughterAbsent_p = '0' then
                o_AXI_RX_p          <= i_Daughter_RX_p and i_AMC_RX_p;
                io_Daughter_TX_p_T  <= '0';
                io_AMC_TX_p_T       <= '0';
                io_RTM_TX_p_T       <= '1';
                
            elsif i_RTMAbsent_p = '0' then
                o_AXI_RX_p          <= i_RTM_RX_p and i_AMC_RX_p;
                io_RTM_TX_p_T       <= '0';
                io_AMC_TX_p_T       <= '0';
                io_Daughter_TX_p_T  <= '1';
            
            else
                o_AXI_RX_p          <= i_AMC_RX_p;
                io_AMC_TX_p_T       <= '0';
                io_Daughter_TX_p_T  <= '1';
                io_RTM_TX_p_T       <= '1';
                
            end if;
            
        elsif (iv2_SelectMode_p = "01") then
            o_AXI_RX_p          <= i_AMC_RX_p;
            io_AMC_TX_p_T       <= '0';
            io_Daughter_TX_p_T  <= '1';
            io_RTM_TX_p_T       <= '1';
            
            
        elsif (iv2_SelectMode_p = "10") then
            o_AXI_RX_p          <= i_Daughter_RX_p;
            io_Daughter_TX_p_T  <= '0';
            io_AMC_TX_p_T       <= '1';
            io_RTM_TX_p_T       <= '1';
            
        else
            o_AXI_RX_p          <= i_RTM_RX_p;
            io_RTM_TX_p_T       <= '0';
            io_AMC_TX_p_T       <= '1';
            io_Daughter_TX_p_T  <= '1';
            
        end if;
    end process;

    io_Daughter_TX_p_O <= i_AXI_TX_p;
    io_AMC_TX_p_O <= i_AXI_TX_p;
    io_RTM_TX_p_O <= i_AXI_TX_p;

end rtl;
