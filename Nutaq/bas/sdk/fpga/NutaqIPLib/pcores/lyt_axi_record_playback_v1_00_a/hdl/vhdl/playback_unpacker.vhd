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
-- File        : $Id: playback_unpacker.vhd,v 1.1 2015/04/10 19:48:36 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Record Playback Playback Unpacker
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Jeffrey Johnson - Initial revision 2011/02/22
-- $Log: playback_unpacker.vhd,v $
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.9  2014/09/11 13:24:35  julien.roy
-- Change playback fifo depth to 512 to avoid starvation during refresh or page change of the ddr3.
--
-- Revision 1.8  2013/05/27 14:28:22  julien.roy
-- Add keep_hierarchy to avoid unrelated LUT packing with other components
--
-- Revision 1.7  2013/05/22 15:06:46  julien.roy
-- Decrease load on AppRdy signal and add location constraint on memory controller and AppRdy
--
-- Revision 1.6  2013/04/19 20:45:45  julien.roy
-- Enable "keep_hierarchy" to solve LUT packing timing problem
--
-- Revision 1.5  2013/04/11 13:56:12  julien.roy
-- Disable "keep_hierarchy"
--
-- Revision 1.4  2013/02/22 22:58:51  julien.roy
-- Synchronous reset
--
-- Revision 1.3  2013/02/07 15:57:08  julien.roy
-- Ease timing by adding register stage
--
-- Revision 1.2  2013/01/31 16:30:54  khalid.bensadek
-- Corrected bugs related to bus_width x Port_Number < 32 bits. Corrected endianness issue for these configurations. Corrected other bugs that were found during this process: Fifo under reset and continuing to write data anyway (Reset edge sensitive).
--
-- Revision 1.1  2012/10/04 14:02:33  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
-- Bug: ne fournie pas plein bandwidth pour l instant.
--
-- Revision 1.5  2011/03/08 16:16:34  jeffrey.johnson
-- Increased FIFO depths.
-- Added playback software trigger and trigger selection.
--
-- Revision 1.4  2011/02/24 20:48:05  jeffrey.johnson
-- Changed FIFO definitions.
-- For designs 64 bit wide and higher, reordered blocks of 32 bits for
-- correct memory reading via RTDEx.
--
--
--------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.std_logic_unsigned.all;
use work.recplay_fifos_p.all;


entity playback_unpacker is
generic
(
  PortWidth_g                : integer := 8;
  NumberOfPorts_g            : integer := 1
);
port
(
  -- Interface to memory controller
  i_WrClk_p                  : in std_logic;
  iv256_FifoWrData_p         : in std_logic_vector(255 downto 0);
  i_WriteEn_p                : in std_logic;
  i_ResetFifo_p              : in std_logic;
  o_FifoProgFull_p           : out std_logic;
  iv9_ProgFullThresAssert_p  : in std_logic_vector(8 downto 0);
  iv9_ProgFullThresNegate_p  : in std_logic_vector(8 downto 0);

  -- User interface
  i_RdClk_p                  : in std_logic;
  ov_DataPort0_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort1_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort2_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort3_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort4_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort5_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort6_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort7_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort8_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort9_p             : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort10_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort11_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort12_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort13_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort14_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  ov_DataPort15_p            : out std_logic_vector(PortWidth_g - 1 downto 0);
  o_Valid_p                  : out std_logic;
  o_Empty_p                  : out std_logic;
  i_ReadEn_p                 : in std_logic

);

end playback_unpacker;

architecture beh of playback_unpacker is

  constant FifoOutputWidth_c    : integer := PortWidth_g * NumberOfPorts_g;
  constant BlocksOf32Bits_c     : integer := FifoOutputWidth_c / 32;

  constant MaxDataWidth_c       : integer := PortWidth_g * 16;
  
  signal v256_FifoWrDataD1_s    : std_logic_vector(255 downto 0);
  signal WriteEnD1_s            : std_logic;
  signal v256_FifoWrDataD2_s    : std_logic_vector(255 downto 0);
  signal WriteEnD2_s            : std_logic;

  signal DataPort_s             : std_logic_vector(MaxDataWidth_c - 1 downto 0);
  signal FifoOutputData_s       : std_logic_vector(FifoOutputWidth_c - 1 downto 0);
  signal FifoOutputReordered_s  : std_logic_vector(FifoOutputWidth_c - 1 downto 0);
  signal FifoLinkData_s         : std_logic_vector(63 downto 0);
  signal FifoInputData_s        : std_logic_vector(255 downto 0);

  signal FifoLinkValid_s        : std_logic;
  signal FifoLinkAlmostFull_s   : std_logic;
  signal FifoLinkTransferEn_s   : std_logic;
  signal Empty_s                : std_logic;
  signal Valid_s                : std_logic;
  
  -- Use to avoid unrelated LUT packing with other components
  attribute keep_hierarchy : string;
  attribute keep_hierarchy of beh : architecture is "true";
  
  attribute keep : string;
  attribute keep of v256_FifoWrDataD1_s : signal is "true";
  attribute keep of WriteEnD1_s         : signal is "true";
  
begin

  --------------------------------------------------
  -- Add register stage to ease timing
  --------------------------------------------------
  process( i_WrClk_p, i_ResetFifo_p )
  begin
    if( rising_edge( i_WrClk_p ) ) then
        v256_FifoWrDataD1_s <= iv256_FifoWrData_p;
        WriteEnD1_s         <= i_WriteEn_p;
        v256_FifoWrDataD2_s <= v256_FifoWrDataD1_s;
        WriteEnD2_s         <= WriteEnD1_s;
    end if;
  end process;  
  
  GenerateFifos8Bit_l:
    if ( FifoOutputWidth_c = 8 ) generate
      begin

        FifoLinkTransferEn_s <= (not Empty_s) and (not FifoLinkAlmostFull_s);

        fifo_256_to_64_l : fifo_256_to_64
          port map (
          rst                     => i_ResetFifo_p,
          wr_clk                  => i_WrClk_p,
          rd_clk                  => i_WrClk_p,
          din                     => v256_FifoWrDataD2_s,
          wr_en                   => WriteEnD2_s,
          rd_en                   => FifoLinkTransferEn_s,
          prog_full_thresh_assert => iv9_ProgFullThresAssert_p(8 downto 0),
          prog_full_thresh_negate => iv9_ProgFullThresNegate_p(8 downto 0),
          dout                    => FifoLinkData_s,
          full                    => open,
          empty                   => Empty_s,
          almost_full             => open,
          valid                   => FifoLinkValid_s,
          prog_full               => o_FifoProgFull_p);

        fifo_64_to_8_l : fifo_64_to_8
          port map (
          rst                     => i_ResetFifo_p,
          wr_clk                  => i_WrClk_p,
          rd_clk                  => i_RdClk_p,
          din                     => FifoLinkData_s,
          wr_en                   => FifoLinkValid_s,
          rd_en                   => i_ReadEn_p,
          dout                    => FifoOutputData_s,
          full                    => open,
          empty                   => o_Empty_p,
          valid                   => Valid_s,
          prog_full             => FifoLinkAlmostFull_s); -- @ 12 of 16 depth

      end generate;

  GenerateFifos16Bit_l:
    if ( FifoOutputWidth_c = 16 ) generate
      begin

        FifoLinkTransferEn_s <= (not Empty_s) and (not FifoLinkAlmostFull_s);

        fifo_256_to_64_l : fifo_256_to_64
          port map (
          rst                     => i_ResetFifo_p,
          wr_clk                  => i_WrClk_p,
          rd_clk                  => i_WrClk_p,
          din                     => v256_FifoWrDataD2_s,
          wr_en                   => WriteEnD2_s,
          rd_en                   => FifoLinkTransferEn_s,
          prog_full_thresh_assert => iv9_ProgFullThresAssert_p(8 downto 0),
          prog_full_thresh_negate => iv9_ProgFullThresNegate_p(8 downto 0),
          dout                    => FifoLinkData_s,
          full                    => open,
          almost_full             => open,
          empty                   => Empty_s,
          valid                   => FifoLinkValid_s,
          prog_full               => o_FifoProgFull_p); -- @ 12 of 16 depth

        fifo_64_to_16_l : fifo_64_to_16
          port map (
          rst                     => i_ResetFifo_p,
          wr_clk                  => i_WrClk_p,
          rd_clk                  => i_RdClk_p,
          din                     => FifoLinkData_s,
          wr_en                   => FifoLinkValid_s,
          rd_en                   => i_ReadEn_p,
          dout                    => FifoOutputData_s,
          full                    => open,
          empty                   => o_Empty_p,
          valid                   => Valid_s,
          prog_full             => FifoLinkAlmostFull_s); -- @ 12 of 16 depth

      end generate;

  GenerateFifos32Bit_l:
    if ( FifoOutputWidth_c = 32 ) generate
      begin

        fifo_256_to_32_l : fifo_256_to_32
          port map (
          rst                     => i_ResetFifo_p,
          wr_clk                  => i_WrClk_p,
          rd_clk                  => i_RdClk_p,
          din                     => v256_FifoWrDataD2_s,
          wr_en                   => WriteEnD2_s,
          rd_en                   => i_ReadEn_p,
          prog_full_thresh_assert => iv9_ProgFullThresAssert_p(8 downto 0),
          prog_full_thresh_negate => iv9_ProgFullThresNegate_p(8 downto 0),
          dout                    => FifoOutputData_s,
          full                    => open,
          almost_full             => open,
          empty                   => o_Empty_p,
          valid                   => Valid_s,
          prog_full               => o_FifoProgFull_p);

      end generate;

  GenerateFifos64Bit_l:
    if ( FifoOutputWidth_c = 64 ) generate
      begin

        fifo_256_to_64_l : fifo_256_to_64
          port map (
          rst                     => i_ResetFifo_p,
          wr_clk                  => i_WrClk_p,
          rd_clk                  => i_RdClk_p,
          din                     => v256_FifoWrDataD2_s,
          wr_en                   => WriteEnD2_s,
          rd_en                   => i_ReadEn_p,
          prog_full_thresh_assert => iv9_ProgFullThresAssert_p(8 downto 0),
          prog_full_thresh_negate => iv9_ProgFullThresNegate_p(8 downto 0),
          dout                    => FifoOutputReordered_s,
          full                    => open,
          almost_full             => open,
          empty                   => o_Empty_p,
          valid                   => Valid_s,
          prog_full               => o_FifoProgFull_p);

      end generate;

  GenerateFifos128Bit_l:
    if ( FifoOutputWidth_c = 128 ) generate
      begin

        fifo_256_to_128_l : fifo_256_to_128
          port map (
          rst                     => i_ResetFifo_p,
          wr_clk                  => i_WrClk_p,
          rd_clk                  => i_RdClk_p,
          din                     => v256_FifoWrDataD2_s,
          wr_en                   => WriteEnD2_s,
          rd_en                   => i_ReadEn_p,
          prog_full_thresh_assert => iv9_ProgFullThresAssert_p(8 downto 0),
          prog_full_thresh_negate => iv9_ProgFullThresNegate_p(8 downto 0),
          dout                    => FifoOutputReordered_s,
          full                    => open,
          almost_full             => open,
          empty                   => o_Empty_p,
          valid                   => Valid_s,
          prog_full               => o_FifoProgFull_p);

      end generate;

  GenerateFifos256Bit_l:
    if ( FifoOutputWidth_c = 256 ) generate
      begin

        fifo_256_to_256_l : fifo_256_to_256
          port map (
          rst                     => i_ResetFifo_p,
          wr_clk                  => i_WrClk_p,
          rd_clk                  => i_RdClk_p,
          din                     => v256_FifoWrDataD2_s,
          wr_en                   => WriteEnD2_s,
          rd_en                   => i_ReadEn_p,
          prog_full_thresh_assert => iv9_ProgFullThresAssert_p(8 downto 0),
          prog_full_thresh_negate => iv9_ProgFullThresNegate_p(8 downto 0),
          dout                    => FifoOutputReordered_s,
          full                    => open,
          almost_full             => open,
          empty                   => o_Empty_p,
          valid                   => Valid_s,
          prog_full               => o_FifoProgFull_p);

      end generate;

  -- Swap the blocks of 32 bits (keeping triggers in same place)
  GenerateIfMoreThan32BitsOutput_l : if( FifoOutputWidth_c >= 64 ) generate 
    GenerateSwap32bitBlocksOutput_l:
      for i in 0 to BlocksOf32Bits_c - 1 generate
        begin
          FifoOutputData_s( ((BlocksOf32Bits_c - 1 - i) * 32) + 31 downto (BlocksOf32Bits_c - 1 - i) * 32 ) <= FifoOutputReordered_s( (i * 32) + 31 downto i * 32 );
      end generate;
  end generate;
  
  -- Concatenated data ports
  DataPort_s( FifoOutputWidth_c - 1 downto 0 ) <= FifoOutputData_s;
  DataPort_s( MaxDataWidth_c - 1 downto FifoOutputWidth_c ) <= (others => '0');

  -- Mapping of FIFO output
  ov_DataPort0_p  <= DataPort_s((PortWidth_g *  1) - 1 downto PortWidth_g *  0);
  ov_DataPort1_p  <= DataPort_s((PortWidth_g *  2) - 1 downto PortWidth_g *  1);
  ov_DataPort2_p  <= DataPort_s((PortWidth_g *  3) - 1 downto PortWidth_g *  2);
  ov_DataPort3_p  <= DataPort_s((PortWidth_g *  4) - 1 downto PortWidth_g *  3);
  ov_DataPort4_p  <= DataPort_s((PortWidth_g *  5) - 1 downto PortWidth_g *  4);
  ov_DataPort5_p  <= DataPort_s((PortWidth_g *  6) - 1 downto PortWidth_g *  5);
  ov_DataPort6_p  <= DataPort_s((PortWidth_g *  7) - 1 downto PortWidth_g *  6);
  ov_DataPort7_p  <= DataPort_s((PortWidth_g *  8) - 1 downto PortWidth_g *  7);
  ov_DataPort8_p  <= DataPort_s((PortWidth_g *  9) - 1 downto PortWidth_g *  8);
  ov_DataPort9_p  <= DataPort_s((PortWidth_g * 10) - 1 downto PortWidth_g *  9);
  ov_DataPort10_p <= DataPort_s((PortWidth_g * 11) - 1 downto PortWidth_g * 10);
  ov_DataPort11_p <= DataPort_s((PortWidth_g * 12) - 1 downto PortWidth_g * 11);
  ov_DataPort12_p <= DataPort_s((PortWidth_g * 13) - 1 downto PortWidth_g * 12);
  ov_DataPort13_p <= DataPort_s((PortWidth_g * 14) - 1 downto PortWidth_g * 13);
  ov_DataPort14_p <= DataPort_s((PortWidth_g * 15) - 1 downto PortWidth_g * 14);
  ov_DataPort15_p <= DataPort_s((PortWidth_g * 16) - 1 downto PortWidth_g * 15);
  
  o_Valid_p <= Valid_s;

end beh;
