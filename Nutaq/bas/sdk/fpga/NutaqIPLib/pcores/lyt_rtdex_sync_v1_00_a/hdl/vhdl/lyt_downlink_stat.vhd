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
-- File : lyt_downlink_stat.vhd
--------------------------------------------------------------------------------
-- Description : VITA 49 transport protocol decoder
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------    
library ieee;
    use ieee.std_logic_1164.all;
  use ieee.std_logic_arith.all;
  use ieee.numeric_std.all; 
  use ieee.std_logic_unsigned.all;    
    
library lyt_rtdex_sync_v1_00_a;
  use lyt_rtdex_sync_v1_00_a.all; 
  use lyt_rtdex_sync_v1_00_a.lyt_rtdex_sync_p.all; 

entity lyt_downlink_stat is
  port (
    i_SysClk_p                  : in  std_logic;            -- Perseus system clock: 200 Mhz      
    i_CoreReset_p               : in  std_logic;
    i_CoreEnable_p              : in  std_logic;            
    i_DecoderNewFrame_p         : in  std_logic;                -- VITA49 decoder new frame indicator
    i_DecoderError_p            : in  std_logic;                -- VITA49 decoder error indicator
    iv16_DecoderErrCode_p       : in  std_logic_vector(15 downto 0);  -- VITA49 decoder error codes
    -- Output interface   
    ov32_DecoderRcvFrame_p      : out std_logic_vector(31 downto 0);    -- Total number of receive frames
    ov32_DecoderBadFrame_p      : out std_logic_vector(31 downto 0);    -- Total number of bad frames
    ov32_DecoderDropFrame_p     : out std_logic_vector(31 downto 0);    -- Total number of drop frames
    ov32_DecoderLostFrame_p     : out std_logic_vector(31 downto 0)     -- Total number of lost frames
  );

end lyt_downlink_stat;

architecture Behavioral of lyt_downlink_stat is

  signal v32_rcv_frame_s      : std_logic_vector(31 downto 0);
  signal v32_bad_frame_s      : std_logic_vector(31 downto 0);
  signal v32_drop_frame_s      : std_logic_vector(31 downto 0);
  signal v32_lost_frame_s      : std_logic_vector(31 downto 0);

begin

  ----------------------------------------------------------------------------
  -- Packet statistic proc
  ----------------------------------------------------------------------------

  statistic_proc : process(i_SysClk_p)
  begin
    if rising_edge(i_SysClk_p) then
      if i_CoreReset_p = '1' then
        v32_rcv_frame_s  <= (others=>'0');
        v32_bad_frame_s  <= (others=>'0');
        v32_drop_frame_s  <= (others=>'0');
        v32_lost_frame_s  <= (others=>'0');
        
      elsif i_CoreEnable_p = '1' then
        if i_DecoderNewFrame_p = '1' then
          v32_rcv_frame_s <= v32_rcv_frame_s + 1;
        end if;
        
--        if i_DecoderError_p = '1' then
--          case iv16_DecoderErrCode_p is
--            when C_VRL_HDR_ERR =>
--              v32_drop_frame_s  <= v32_drop_frame_s + 1;
--              
--            when C_VRL_FRMSIZE_ERR =>
--            when C_VRT_PKTSIZE_ERR =>
--              v32_lost_frame_s   <= v32_lost_frame_s + 1;
--              
--            when others =>
--              v32_bad_frame_s   <= v32_bad_frame_s + 1;
--          end case;
--        end if;
        
      end if;
    end if;
  end process;


  -- output ports
  --
  ov32_DecoderRcvFrame_p  <= v32_rcv_frame_s;
  ov32_DecoderBadFrame_p  <= v32_bad_frame_s;
  ov32_DecoderDropFrame_p  <= v32_drop_frame_s;
  ov32_DecoderLostFrame_p  <= v32_lost_frame_s;


end Behavioral;