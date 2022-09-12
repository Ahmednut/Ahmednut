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
-- File : lyt_async_fifo_w256.vhd
--------------------------------------------------------------------------------
-- Description : Asynchronous fifo for adac250
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_mi125_async_fifo.vhd,v $
-- Revision 1.2  2015/07/08 13:03:51  julien.roy
-- Split Channel into multiple ports to ease the mapping in mhs
--
-- Revision 1.1  2015/04/10 19:48:59  sqa
-- bas first commit
--
-- Revision 1.1  2014/10/16 13:14:03  julien.roy
-- Add mi125 async fifo core to use with the mo1000 core.
--
-- Revision 1.1  2014/07/28 18:14:10  julien.roy
-- Add mo1000 mi125 bsdk example
--
-- Revision 1.1  2013/01/08 18:52:09  julien.roy
-- Add first version of adac250 loopback edk project
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;

entity lyt_mi125_async_fifo is

  port (
    i_Rst_p             : in std_logic;
    i_WrClk_p           : in std_logic;
    i_RdClk_p           : in std_logic;
    i_WeEn_p            : in std_logic;
    i_RdEn_p            : in std_logic;
    o_DataRdy_p         : out std_logic;
    
    iv14_Data1_p         : in std_logic_vector(13 downto 0);
    iv14_Data2_p         : in std_logic_vector(13 downto 0);
    iv14_Data3_p         : in std_logic_vector(13 downto 0);
    iv14_Data4_p         : in std_logic_vector(13 downto 0);
    iv14_Data5_p         : in std_logic_vector(13 downto 0);
    iv14_Data6_p         : in std_logic_vector(13 downto 0);
    iv14_Data7_p         : in std_logic_vector(13 downto 0);
    iv14_Data8_p         : in std_logic_vector(13 downto 0);
    iv14_Data9_p         : in std_logic_vector(13 downto 0);
    iv14_Data10_p        : in std_logic_vector(13 downto 0);
    iv14_Data11_p        : in std_logic_vector(13 downto 0);
    iv14_Data12_p        : in std_logic_vector(13 downto 0);
    iv14_Data13_p        : in std_logic_vector(13 downto 0);
    iv14_Data14_p        : in std_logic_vector(13 downto 0);
    iv14_Data15_p        : in std_logic_vector(13 downto 0);
    iv14_Data16_p        : in std_logic_vector(13 downto 0);
    i_Data1Valid_p       : in std_logic;
    i_Data2Valid_p       : in std_logic;
    i_Data3Valid_p       : in std_logic;
    i_Data4Valid_p       : in std_logic;
    i_Data5Valid_p       : in std_logic;
    i_Data6Valid_p       : in std_logic;
    i_Data7Valid_p       : in std_logic;
    i_Data8Valid_p       : in std_logic;
    i_Data9Valid_p       : in std_logic;
    i_Data10Valid_p      : in std_logic;
    i_Data11Valid_p      : in std_logic;
    i_Data12Valid_p      : in std_logic;
    i_Data13Valid_p      : in std_logic;
    i_Data14Valid_p      : in std_logic;
    i_Data15Valid_p      : in std_logic;
    i_Data16Valid_p      : in std_logic;
    i_DataFormat_p       : in std_logic;
    i_Adc1to4En_p        : in std_logic;
    i_Adc5to8En_p        : in std_logic;
    i_Adc9to12En_p       : in std_logic;
    i_Adc13to16En_p      : in std_logic;
    i_Trigger_p          : in std_logic;
    
    ov14_Data1_p         : out std_logic_vector(13 downto 0);
    ov14_Data2_p         : out std_logic_vector(13 downto 0);
    ov14_Data3_p         : out std_logic_vector(13 downto 0);
    ov14_Data4_p         : out std_logic_vector(13 downto 0);
    ov14_Data5_p         : out std_logic_vector(13 downto 0);
    ov14_Data6_p         : out std_logic_vector(13 downto 0);
    ov14_Data7_p         : out std_logic_vector(13 downto 0);
    ov14_Data8_p         : out std_logic_vector(13 downto 0);
    ov14_Data9_p         : out std_logic_vector(13 downto 0);
    ov14_Data10_p        : out std_logic_vector(13 downto 0);
    ov14_Data11_p        : out std_logic_vector(13 downto 0);
    ov14_Data12_p        : out std_logic_vector(13 downto 0);
    ov14_Data13_p        : out std_logic_vector(13 downto 0);
    ov14_Data14_p        : out std_logic_vector(13 downto 0);
    ov14_Data15_p        : out std_logic_vector(13 downto 0);
    ov14_Data16_p        : out std_logic_vector(13 downto 0);
    o_Data1Valid_p       : out std_logic;
    o_Data2Valid_p       : out std_logic;
    o_Data3Valid_p       : out std_logic;
    o_Data4Valid_p       : out std_logic;
    o_Data5Valid_p       : out std_logic;
    o_Data6Valid_p       : out std_logic;
    o_Data7Valid_p       : out std_logic;
    o_Data8Valid_p       : out std_logic;
    o_Data9Valid_p       : out std_logic;
    o_Data10Valid_p      : out std_logic;
    o_Data11Valid_p      : out std_logic;
    o_Data12Valid_p      : out std_logic;
    o_Data13Valid_p      : out std_logic;
    o_Data14Valid_p      : out std_logic;
    o_Data15Valid_p      : out std_logic;
    o_Data16Valid_p      : out std_logic;
    o_DataFormat_p       : out std_logic;
    o_Adc1to4En_p        : out std_logic;
    o_Adc5to8En_p        : out std_logic;
    o_Adc9to12En_p       : out std_logic;
    o_Adc13to16En_p      : out std_logic;
    o_Trigger_p          : out std_logic
  );
end entity lyt_mi125_async_fifo;

architecture rtl of lyt_mi125_async_fifo is

  component async_fifo_w256
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(255 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      dout : out std_logic_vector(255 downto 0);
      full : out std_logic;
      empty : out std_logic;
      valid : out std_logic
    );
  end component;
  
  signal fifo_din   : std_logic_vector(255 downto 0);
  signal fifo_dout  : std_logic_vector(255 downto 0);
  signal fifo_rd_en : std_logic;
  signal fifo_rdy   : std_logic;
  signal fifo_empty : std_logic;
  signal fifo_valid : std_logic;

begin

  fifo_din <= "1111111111"
    & i_Trigger_p
    & iv14_Data1_p   
    & iv14_Data2_p   
    & iv14_Data3_p   
    & iv14_Data4_p   
    & iv14_Data5_p   
    & iv14_Data6_p   
    & iv14_Data7_p   
    & iv14_Data8_p   
    & iv14_Data9_p   
    & iv14_Data10_p  
    & iv14_Data11_p  
    & iv14_Data12_p  
    & iv14_Data13_p  
    & iv14_Data14_p  
    & iv14_Data15_p  
    & iv14_Data16_p  
    & i_Data1Valid_p 
    & i_Data2Valid_p 
    & i_Data3Valid_p 
    & i_Data4Valid_p 
    & i_Data5Valid_p 
    & i_Data6Valid_p 
    & i_Data7Valid_p 
    & i_Data8Valid_p 
    & i_Data9Valid_p 
    & i_Data10Valid_p
    & i_Data11Valid_p
    & i_Data12Valid_p
    & i_Data13Valid_p
    & i_Data14Valid_p
    & i_Data15Valid_p
    & i_Data16Valid_p
    & i_DataFormat_p 
    & i_Adc1to4En_p  
    & i_Adc5to8En_p  
    & i_Adc9to12En_p 
    & i_Adc13to16En_p;
    
  async_fifo_w256_inst : async_fifo_w256
    port map (
      rst => i_Rst_p,
      wr_clk => i_WrClk_p,
      rd_clk => i_RdClk_p,
      din => fifo_din,
      wr_en => i_WeEn_p,
      rd_en => fifo_rd_en,
      dout => fifo_dout,
      full => open,
      empty => fifo_empty,
      valid => fifo_valid
    );
    
  process(i_RdClk_p)
  begin
    if rising_edge(i_RdClk_p) then
      fifo_rdy <= (not fifo_empty);
      
      o_Trigger_p         <= fifo_dout(245);
      
      ov14_Data1_p        <= fifo_dout(14*16+20 downto 14*15+21);
      ov14_Data2_p        <= fifo_dout(14*15+20 downto 14*14+21);
      ov14_Data3_p        <= fifo_dout(14*14+20 downto 14*13+21);
      ov14_Data4_p        <= fifo_dout(14*13+20 downto 14*12+21);
      ov14_Data5_p        <= fifo_dout(14*12+20 downto 14*11+21);
      ov14_Data6_p        <= fifo_dout(14*11+20 downto 14*10+21);
      ov14_Data7_p        <= fifo_dout(14*10+20 downto 14*9+21);
      ov14_Data8_p        <= fifo_dout(14*9+20 downto 14*8+21);
      ov14_Data9_p        <= fifo_dout(14*8+20 downto 14*7+21);
      ov14_Data10_p       <= fifo_dout(14*7+20 downto 14*6+21);
      ov14_Data11_p       <= fifo_dout(14*6+20 downto 14*5+21);
      ov14_Data12_p       <= fifo_dout(14*5+20 downto 14*4+21);
      ov14_Data13_p       <= fifo_dout(14*4+20 downto 14*3+21);
      ov14_Data14_p       <= fifo_dout(14*3+20 downto 14*2+21);
      ov14_Data15_p       <= fifo_dout(14*2+20 downto 14*1+21);
      ov14_Data16_p       <= fifo_dout(14*1+20 downto 14*0+21);
      
      if fifo_valid = '1' then
          o_Data1Valid_p      <= fifo_dout(20);
          o_Data2Valid_p      <= fifo_dout(19);
          o_Data3Valid_p      <= fifo_dout(18);
          o_Data4Valid_p      <= fifo_dout(17);
          o_Data5Valid_p      <= fifo_dout(16);
          o_Data6Valid_p      <= fifo_dout(15);
          o_Data7Valid_p      <= fifo_dout(14);
          o_Data8Valid_p      <= fifo_dout(13);
          o_Data9Valid_p      <= fifo_dout(12);
          o_Data10Valid_p     <= fifo_dout(11);
          o_Data11Valid_p     <= fifo_dout(10);
          o_Data12Valid_p     <= fifo_dout(9);
          o_Data13Valid_p     <= fifo_dout(8);
          o_Data14Valid_p     <= fifo_dout(7);
          o_Data15Valid_p     <= fifo_dout(6);
          o_Data16Valid_p     <= fifo_dout(5);
      else
          o_Data1Valid_p      <= '0';
          o_Data2Valid_p      <= '0';
          o_Data3Valid_p      <= '0';
          o_Data4Valid_p      <= '0';
          o_Data5Valid_p      <= '0';
          o_Data6Valid_p      <= '0';
          o_Data7Valid_p      <= '0';
          o_Data8Valid_p      <= '0';
          o_Data9Valid_p      <= '0';
          o_Data10Valid_p     <= '0';
          o_Data11Valid_p     <= '0';
          o_Data12Valid_p     <= '0';
          o_Data13Valid_p     <= '0';
          o_Data14Valid_p     <= '0';
          o_Data15Valid_p     <= '0';
          o_Data16Valid_p     <= '0';
      end if;
      
      o_DataFormat_p      <= fifo_dout(4);
      o_Adc1to4En_p       <= fifo_dout(3);
      o_Adc5to8En_p       <= fifo_dout(2);
      o_Adc9to12En_p      <= fifo_dout(1);
      o_Adc13to16En_p     <= fifo_dout(0);
      
    end if;
  end process;
  
  fifo_rd_en <= i_RdEn_p and fifo_rdy;
  
  o_DataRdy_p <= fifo_rdy;
  
end architecture rtl;

