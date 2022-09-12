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
-- File : ntq_axi_freq_counter.vhd
--------------------------------------------------------------------------------
-- Description : Core that instanciate a flipflop in the IOB if USE_FF is true
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2013 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: ntq_axi_freq_counter.vhd,v $
-- Revision 1.1  2015/10/30 14:13:23  julien.roy
-- Add clock frequency monitoring core.
--
-- Revision 1.1  2014/02/19 16:33:58  julien.roy
-- Add the first version of Nutaq ip core used in the BIST project for the Integrated Transceiver and Processor boards (DSOD1301)
--
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

library ntq_axi_freq_counter_v1_00_a;
use ntq_axi_freq_counter_v1_00_a.all;

library unisim;
use unisim.vcomponents.all;

entity ntq_axi_freq_counter is
    generic
    (
        REFCLK_FREQ_HZ      : integer := 100000000;
        CLOCK_NUMBER        : integer range 1 to 64 := 8;
        
        -- DO NOT EDIT BELOW THIS LINE ---------------------
        -- Bus protocol parameters, do not add to or delete
        C_S_AXI_DATA_WIDTH             : integer              := 32;
        C_S_AXI_ADDR_WIDTH             : integer              := 32;
        C_S_AXI_MIN_SIZE               : std_logic_vector     := X"000001FF";
        C_USE_WSTRB                    : integer              := 0;
        C_DPHASE_TIMEOUT               : integer              := 8;
        C_BASEADDR                     : std_logic_vector     := X"FFFFFFFF";
        C_HIGHADDR                     : std_logic_vector     := X"00000000";
        C_FAMILY                       : string               := "virtex6"
        -- DO NOT EDIT ABOVE THIS LINE ---------------------
    );
    port
    (
        i_Rst_p  : in std_logic;
        i_RefClk_p : in std_logic;
        iv_TestClk_p : in std_logic_vector((CLOCK_NUMBER-1) downto 0);
        
        -- DO NOT EDIT BELOW THIS LINE ---------------------
        -- Bus protocol ports, do not add to or delete
        S_AXI_ACLK                     : in  std_logic;
        S_AXI_ARESETN                  : in  std_logic;
        S_AXI_AWADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
        S_AXI_AWVALID                  : in  std_logic;
        S_AXI_WDATA                    : in  std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
        S_AXI_WSTRB                    : in  std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
        S_AXI_WVALID                   : in  std_logic;
        S_AXI_BREADY                   : in  std_logic;
        S_AXI_ARADDR                   : in  std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
        S_AXI_ARVALID                  : in  std_logic;
        S_AXI_RREADY                   : in  std_logic;
        S_AXI_ARREADY                  : out std_logic;
        S_AXI_RDATA                    : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
        S_AXI_RRESP                    : out std_logic_vector(1 downto 0);
        S_AXI_RVALID                   : out std_logic;
        S_AXI_WREADY                   : out std_logic;
        S_AXI_BRESP                    : out std_logic_vector(1 downto 0);
        S_AXI_BVALID                   : out std_logic;
        S_AXI_AWREADY                  : out std_logic
        -- DO NOT EDIT ABOVE THIS LINE --
    );
    
    attribute MAX_FANOUT                     : string;
    attribute SIGIS                          : string;
    attribute MAX_FANOUT of S_AXI_ACLK       : signal is "10000";
    attribute MAX_FANOUT of S_AXI_ARESETN    : signal is "10000";
    attribute SIGIS of S_AXI_ACLK            : signal is "Clk";
    attribute SIGIS of S_AXI_ARESETN         : signal is "Rst";
    
end entity ntq_axi_freq_counter;


architecture rtl of ntq_axi_freq_counter is

    ----------------------------------------
    -- Signals declaration
    ----------------------------------------
    signal v32_freq_s       : std_logic_vector(31 downto 0);
    signal v32_index_s      : std_logic_vector(31 downto 0);
  
begin

    frequency_counter_mux_INST: entity ntq_axi_freq_counter_v1_00_a.frequency_counter_mux
       generic map (
          C_REFCLK_FREQ_HZ  => REFCLK_FREQ_HZ,
          C_NUM_OF_TEST_CLK => CLOCK_NUMBER)
       port map (
          i_Rst_p           => i_Rst_p,
          i_RefClk_p        => i_RefClk_p,
          iv_TestClks_p     => iv_TestClk_p,
          iv32_TestClkSel_p => v32_index_s,
          ov32_Freq_p       => v32_freq_s);

    --------------------------------------------
    -- instantiate AXI Memory Mapped User Logic
    --------------------------------------------
    USER_LOGIC_I : entity ntq_axi_freq_counter_v1_00_a.axi_freq_counter
        generic map
        (
            -- MAP USER GENERICS BELOW THIS LINE ---------------
            --USER generics mapped here
            -- MAP USER GENERICS ABOVE THIS LINE ---------------
            C_S_AXI_DATA_WIDTH        => C_S_AXI_DATA_WIDTH,
            C_S_AXI_ADDR_WIDTH        => C_S_AXI_ADDR_WIDTH,
            C_S_AXI_MIN_SIZE          => C_S_AXI_MIN_SIZE  ,
            C_USE_WSTRB               => C_USE_WSTRB       ,
            C_DPHASE_TIMEOUT          => C_DPHASE_TIMEOUT  ,
            C_BASEADDR                => C_BASEADDR        ,
            C_HIGHADDR                => C_HIGHADDR        ,
            C_FAMILY                  => C_FAMILY          
        )
        port map
        (
            -- user_logic entity ports mapping  ---------------
            i_CoreReset_p             => i_Rst_p,

            iv32_freq_p               => v32_freq_s,
            ov32_index_p              => v32_index_s,
            -- Bus Protocol Ports mapping --
            S_AXI_ACLK                => S_AXI_ACLK    ,
            S_AXI_ARESETN             => S_AXI_ARESETN ,
            S_AXI_AWADDR              => S_AXI_AWADDR  ,
            S_AXI_AWVALID             => S_AXI_AWVALID ,
            S_AXI_WDATA               => S_AXI_WDATA   ,
            S_AXI_WSTRB               => S_AXI_WSTRB   ,
            S_AXI_WVALID              => S_AXI_WVALID  ,
            S_AXI_BREADY              => S_AXI_BREADY  ,
            S_AXI_ARADDR              => S_AXI_ARADDR  ,
            S_AXI_ARVALID             => S_AXI_ARVALID ,
            S_AXI_RREADY              => S_AXI_RREADY  ,
            S_AXI_ARREADY             => S_AXI_ARREADY ,
            S_AXI_RDATA               => S_AXI_RDATA   ,
            S_AXI_RRESP               => S_AXI_RRESP   ,
            S_AXI_RVALID              => S_AXI_RVALID  ,
            S_AXI_WREADY              => S_AXI_WREADY  ,
            S_AXI_BRESP               => S_AXI_BRESP   ,
            S_AXI_BVALID              => S_AXI_BVALID  ,
            S_AXI_AWREADY             => S_AXI_AWREADY  
        );

end rtl;
