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
-- File        : $Id: lyt_axi_lvds_io.vhd,v 1.1 2015/04/10 19:47:33 sqa Exp $
--------------------------------------------------------------------------------
-- Description :
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Nutaq inc.
--------------------------------------------------------------------------------
-- $Log: lyt_axi_lvds_io.vhd,v $
-- Revision 1.1  2015/04/10 19:47:33  sqa
-- bas first commit (CBOU)
--
-- Revision 1.8  2014/04/01 20:37:18  julien.roy
-- latch core reset to ease timing
--
-- Revision 1.7  2013/02/08 14:04:36  julien.roy
-- Add IDELAY to ease timing in sync mode
--
-- Revision 1.6  2013/01/30 19:52:04  julien.roy
-- Modify LVDS example for Synchronous examples
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

library lyt_axi_lvds_io_v1_00_a;
use lyt_axi_lvds_io_v1_00_a.all;

entity lyt_axi_lvds_io is
    generic
    (
        -- ADD USER GENERICS BELOW THIS LINE ---------------
        USE_SYNCHRONOUS_GPIO_GROUP0	: integer := 1;
        USE_SYNCHRONOUS_GPIO_GROUP1	: integer := 1;
        DATA_IDELAY_VALUE_GROUP0    : integer := 0;
        CLK_IDELAY_VALUE_GROUP0     : integer := 0;
        DATA_IDELAY_VALUE_GROUP1    : integer := 0;
        CLK_IDELAY_VALUE_GROUP1     : integer := 0;
        C_BOTTOM_POSITION           : boolean := true;
        -- ADD USER GENERICS ABOVE THIS LINE ---------------

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
    i_RefClk200MHz_p          : in std_logic;
    i_UserClk_p               : in std_logic;

        iv_UserLvdsGroup0_p				: in  std_logic_vector( 15 - USE_SYNCHRONOUS_GPIO_GROUP0*2  downto 0);
        iv_UserLvdsGroup1_p				: in  std_logic_vector( 15 - USE_SYNCHRONOUS_GPIO_GROUP1*2  downto 0);

        ov_UserLvdsGroup0_p				: out std_logic_vector( 15 - USE_SYNCHRONOUS_GPIO_GROUP0*2 downto 0);
        ov_UserLvdsGroup1_p				: out std_logic_vector( 15 - USE_SYNCHRONOUS_GPIO_GROUP1*2 downto 0);

        -- fifo flag and control ( NOT ACTIVE in GPIO mode )
        -- TX FIFO 0
        i_TxClk0_p             : in  std_logic;
        i_inWrEn0_p            : in  std_logic;
        o_inWrAck0_p           : out std_logic;
        o_full0_p              : out std_logic;
        -- RX FIFO 0
        i_outRdEn0_p           : in  std_logic;
        o_outValid0_p          : out std_logic;
        o_empty0_p             : out std_logic;
        o_RxClk0_p             : out std_logic;
        -- TX FIFO 1
        i_TxClk1_p             : in  std_logic;
        i_inWrEn1_p            : in  std_logic;
        o_inWrAck1_p           : out std_logic;
        o_full1_p              : out std_logic;
        -- RX FIFO 1
        i_outRdEn1_p           : in  std_logic;
        o_outValid1_p          : out std_logic;
        o_empty1_p             : out std_logic;
        o_RxClk1_p             : out std_logic;

        -- external --
        iov16_Group0_padIOp_p				: inout std_logic_vector( 15 downto 0 );
        iov16_Group1_padIOp_p				: inout std_logic_vector( 15 downto 0 );
        iov16_Group0_padIOn_p				: inout std_logic_vector( 15 downto 0 );
        iov16_Group1_padIOn_p				: inout std_logic_vector( 15 downto 0 );


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

end entity lyt_axi_lvds_io;

------------------------------------------------------------------------------
-- Architecture section
------------------------------------------------------------------------------

architecture arch of lyt_axi_lvds_io is

    ----------------------------------------
    -- Signal declaration
    ----------------------------------------
    signal CoreResetPulse_s               : std_logic;
    signal dirGroup0_s                    : std_logic;
    signal dirGroup1_s                    : std_logic;

    -- Reset signals
    signal v8_SignalStretch_s		  : std_logic_vector(7 downto 0);
    signal CoreReset_s				    : std_logic;

begin

    ------------------------------------------
    -- LVDS Wrapper
    ------------------------------------------
    lvds_wrapper_u0 : entity lyt_axi_lvds_io_v1_00_a.lvds_wrapper
    generic map(
        USE_SYNCHRONOUS_GPIO_GROUP0	=> USE_SYNCHRONOUS_GPIO_GROUP0,
        USE_SYNCHRONOUS_GPIO_GROUP1	=> USE_SYNCHRONOUS_GPIO_GROUP1,
        DATA_IDELAY_VALUE_GROUP0    => DATA_IDELAY_VALUE_GROUP0,
        CLK_IDELAY_VALUE_GROUP0     => CLK_IDELAY_VALUE_GROUP0,
        DATA_IDELAY_VALUE_GROUP1    => DATA_IDELAY_VALUE_GROUP1,
        CLK_IDELAY_VALUE_GROUP1     => CLK_IDELAY_VALUE_GROUP1
    )
    port map(
        i_rst_p 					=> CoreReset_s,
        i_SystemClk_p               => S_AXI_ACLK,
        i_RefClk200MHz_p            => i_RefClk200MHz_p,
        i_UserClk_p                 => i_UserClk_p,

        i_directionGroup0_p         => dirGroup0_s,
        i_directionGroup1_p         => dirGroup1_s,

        iv_UserLvdsGroup0_p 		=> iv_UserLvdsGroup0_p,
        iv_UserLvdsGroup1_p 		=> iv_UserLvdsGroup1_p,
        ov_UserLvdsGroup0_p 		=> ov_UserLvdsGroup0_p,
        ov_UserLvdsGroup1_p 		=> ov_UserLvdsGroup1_p,

        -- fifo flag and control ( NOT ACTIVE in GPIO mode )
        -- TX FIFO 0
        i_TxClkGroup0_p             => i_TxClk0_p,
        i_inWrEnGroup0_p            => i_inWrEn0_p,
        o_inWrAckGroup0_p           => o_inWrAck0_p,
        o_fullGroup0_p              => o_full0_p,
        -- RX FIFO 0
        i_outRdEnGroup0_p           => i_outRdEn0_p,
        o_outValidGroup0_p          => o_outValid0_p,
        o_emptyGroup0_p             => o_empty0_p,
        o_RxClkGroup0_p             => o_RxClk0_p,
        -- TX FIFO 1
        i_TxClkGroup1_p             => i_TxClk1_p,
        i_inWrEnGroup1_p            => i_inWrEn1_p,
        o_inWrAckGroup1_p           => o_inWrAck1_p,
        o_fullGroup1_p              => o_full1_p,
        -- RX FIFO 1
        i_outRdEnGroup1_p           => i_outRdEn1_p,
        o_outValidGroup1_p          => o_outValid1_p,
        o_emptyGroup1_p             => o_empty1_p,
        o_RxClkGroup1_p             => o_RxClk1_p,

        iov16_Group0_padIOp_p	 	=> iov16_Group0_padIOp_p,
        iov16_Group1_padIOp_p 		=> iov16_Group1_padIOp_p,
        iov16_Group0_padIOn_p 		=> iov16_Group0_padIOn_p,
        iov16_Group1_padIOn_p 		=> iov16_Group1_padIOn_p
    );

    --------------------------------------------
    -- instantiate AXI Memory Mapped User Logic
    --------------------------------------------
    register_u0 : entity lyt_axi_lvds_io_v1_00_a.axi_lvds_io
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
        i_CoreReset_p                 => CoreReset_s,
        o_CoreResetPulse_p            => CoreResetPulse_s,
        o_dirGroup0_p                 => dirGroup0_s,
        o_dirGroup1_p                 => dirGroup1_s,
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
    
    --------------------------------------------
    -- SW reset pulse stretcher.
    --------------------------------------------
    process(S_AXI_ACLK)
    begin
        if rising_edge(S_AXI_ACLK) then
            v8_SignalStretch_s <= v8_SignalStretch_s(6 downto 0) & CoreResetPulse_s; 
            CoreReset_s <= or_reduce(v8_SignalStretch_s);			 		
        end if;
    end process;


end arch;
