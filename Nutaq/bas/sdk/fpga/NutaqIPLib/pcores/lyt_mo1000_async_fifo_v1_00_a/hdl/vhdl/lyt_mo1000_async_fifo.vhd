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
-- File : lyt_mo1000_async_fifo.vhd
--------------------------------------------------------------------------------
-- Description : Asynchronous fifo for adac250
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2016 Nutaq inc.
--------------------------------------------------------------------------------
--
--------------------------------------------------------------------------------

library ieee;
  use ieee.std_logic_1164.all;

entity lyt_mo1000_async_fifo is

  port (
    i_Rst_p             : in std_logic;
    i_DesignClk_p       : in std_logic;
    i_Mo1000Clk_p       : in std_logic;        
        
    iv16_Data1_p         : in std_logic_vector(15 downto 0);
    iv16_Data2_p         : in std_logic_vector(15 downto 0);
    iv16_Data3_p         : in std_logic_vector(15 downto 0);
    iv16_Data4_p         : in std_logic_vector(15 downto 0);
    iv16_Data5_p         : in std_logic_vector(15 downto 0);
    iv16_Data6_p         : in std_logic_vector(15 downto 0);
    iv16_Data7_p         : in std_logic_vector(15 downto 0);
    iv16_Data8_p         : in std_logic_vector(15 downto 0);    
    i_Ready1_p           : in std_logic;
    i_Ready2_p           : in std_logic;
    i_Ready3_p           : in std_logic;
    i_Ready4_p           : in std_logic;
    i_Ready5_p           : in std_logic;
    i_Ready6_p           : in std_logic;
    i_Ready7_p           : in std_logic;
    i_Ready8_p           : in std_logic;    
    i_Trigger_p          : in std_logic;
    
    ov16_Data1_p         : out std_logic_vector(15 downto 0);
    ov16_Data2_p         : out std_logic_vector(15 downto 0);
    ov16_Data3_p         : out std_logic_vector(15 downto 0);
    ov16_Data4_p         : out std_logic_vector(15 downto 0);
    ov16_Data5_p         : out std_logic_vector(15 downto 0);
    ov16_Data6_p         : out std_logic_vector(15 downto 0);
    ov16_Data7_p         : out std_logic_vector(15 downto 0);
    ov16_Data8_p         : out std_logic_vector(15 downto 0);    
    o_Ready1_p           : out std_logic;
    o_Ready2_p           : out std_logic;
    o_Ready3_p           : out std_logic;
    o_Ready4_p           : out std_logic;
    o_Ready5_p           : out std_logic;
    o_Ready6_p           : out std_logic;
    o_Ready7_p           : out std_logic;
    o_Ready8_p           : out std_logic;    
    o_Trigger_p          : out std_logic
	
  );
end entity lyt_mo1000_async_fifo;

architecture rtl of lyt_mo1000_async_fifo is

  component async_fifo_w128_d512
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(127 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      dout : out std_logic_vector(127 downto 0);
      full : out std_logic;
      empty : out std_logic;
      valid : out std_logic
    );
  end component;
  
 component async_fifo_w9_d512
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(8 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      dout : out std_logic_vector(8 downto 0);
      full : out std_logic;
      empty : out std_logic;
      valid : out std_logic
    );
  end component;
  
  signal fifo_1_din   : std_logic_vector(127 downto 0);
  signal fifo_1_dout  : std_logic_vector(127 downto 0);       
  
  signal fifo_2_din   : std_logic_vector(8 downto 0);
  signal fifo_2_dout  : std_logic_vector(8 downto 0);
  
begin

 ---------------------------------------------------------------------
 -- Passing Data from design clock to MO1000 clock
 ---------------------------------------------------------------------
  fifo_1_din <= iv16_Data8_p 
              & iv16_Data7_p 
			  & iv16_Data6_p 
			  & iv16_Data5_p
			  & iv16_Data4_p 
			  & iv16_Data3_p 
			  & iv16_Data2_p 
			  & iv16_Data1_p;
    
  async_fifo1_inst : async_fifo_w128_d512
    port map (
      rst => i_Rst_p,
      wr_clk => i_DesignClk_p,
      rd_clk => i_Mo1000Clk_p,
      din => fifo_1_din,
      wr_en => '1',
      rd_en => '1',
      dout => fifo_1_dout,
      full => open,
      empty => open,
      valid => open
    );
	
  process(i_Mo1000Clk_p)
  begin
    if rising_edge(i_Mo1000Clk_p) then            
      ov16_Data1_p        <= fifo_1_dout(15 downto 0);
      ov16_Data2_p        <= fifo_1_dout(31 downto 16);
      ov16_Data3_p        <= fifo_1_dout(47 downto 32);
      ov16_Data4_p        <= fifo_1_dout(63 downto 48);
      ov16_Data5_p        <= fifo_1_dout(79 downto 64);
      ov16_Data6_p        <= fifo_1_dout(95 downto 80);
      ov16_Data7_p        <= fifo_1_dout(111 downto 96);
      ov16_Data8_p        <= fifo_1_dout(127 downto 112);	 
    end if;
  end process;
 
 ---------------------------------------------------------------------
 -- Passing Flags from MO1000 clock to design clock
 --------------------------------------------------------------------- 
 fifo_2_din <= i_Trigger_p               
             & i_Ready8_p 
             & i_Ready7_p 
             & i_Ready6_p 
             & i_Ready5_p 
             & i_Ready4_p 
             & i_Ready3_p 
             & i_Ready2_p 
             & i_Ready1_p;	        			
			    
  async_fifo2_inst : async_fifo_w9_d512
    port map (
      rst => i_Rst_p,
      wr_clk => i_Mo1000Clk_p,
      rd_clk => i_DesignClk_p,
      din => fifo_2_din,
      wr_en => '1',
      rd_en => '1',
      dout => fifo_2_dout,
      full => open,
      empty => open,
      valid => open
    );
    
 
  
  process(i_DesignClk_p)
  begin
    if rising_edge(i_DesignClk_p) then
		o_Ready1_p      <= fifo_2_dout(0);
		o_Ready2_p      <= fifo_2_dout(1);
		o_Ready3_p      <= fifo_2_dout(2);
		o_Ready4_p      <= fifo_2_dout(3);
		o_Ready5_p      <= fifo_2_dout(4);
		o_Ready6_p      <= fifo_2_dout(5);
		o_Ready7_p      <= fifo_2_dout(6);
		o_Ready8_p      <= fifo_2_dout(7);
		o_Trigger_p     <= fifo_2_dout(8);      
    end if;
  end process;
         
end architecture rtl;

