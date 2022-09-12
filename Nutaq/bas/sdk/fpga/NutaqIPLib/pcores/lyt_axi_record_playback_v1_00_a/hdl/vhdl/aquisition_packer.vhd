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
-- File        : $Id: aquisition_packer.vhd,v 1.1 2015/04/10 19:48:36 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Record Playback Aquisition Packer
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
-- $Log: aquisition_packer.vhd,v $
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.13  2015/02/27 16:10:50  julien.roy
-- Add an additional delay register to ease timing.
--
-- Revision 1.12  2015/02/02 19:14:31  julien.roy
-- Add 1 more clock delay on prog full flag to ease timing
--
-- Revision 1.11  2015/01/13 14:32:06  julien.roy
-- Add programmable full port to FIFO.
-- Add 1 clock cycle delay for Data, Trigger, We to ease timing.
--
-- Revision 1.10  2013/05/27 14:28:22  julien.roy
-- Add keep_hierarchy to avoid unrelated LUT packing with other components
--
-- Revision 1.9  2013/05/22 15:06:46  julien.roy
-- Decrease load on AppRdy signal and add location constraint on memory controller and AppRdy
--
-- Revision 1.8  2013/05/08 18:10:11  julien.roy
-- Modify record playback memory controller to solve timing errors
--
-- Revision 1.7  2013/04/19 20:45:45  julien.roy
-- Enable "keep_hierarchy" to solve LUT packing timing problem
--
-- Revision 1.6  2013/04/12 13:12:29  julien.roy
-- Add monitoring registers
-- Enable the core reset register
-- Add core ID
-- Merge previous register 0x0 with register0x4 at register 0x4 address
-- Disable keep_hierarchy
--
-- Revision 1.5  2013/01/31 16:31:37  khalid.bensadek
-- Corrected bugs related to bus_width x Port_Number < 32 bits. Corrected endianness issue for these configurations. Corrected other bugs that were found during this process: Fifo under reset and continuing to write data anyway (Reset edge sensitive).
--
-- Revision 1.3  2012/12/12 03:17:47  khalid.bensadek
-- Solved timings issues with big FPGA, SX475.
--
-- Revision 1.2  2012/10/12 12:17:34  khalid.bensadek
-- Removed the recently added fifo and changed acquisition packer fifo's to be FWFT to accomodate high truput (Prevent user FIFO full).
--
-- Revision 1.1  2012/10/04 14:02:33  khalid.bensadek
-- First commit of a stable AXI version. Xilinx 13.4
-- Bug: ne fournie pas plein bandwidth pour l instant.
--
-- Revision 1.6  2011/03/01 14:08:10  jeffrey.johnson
-- Fixed link FIFO connection bug for 8x1, 8x2 and 16x1 configs.
--
-- Revision 1.5  2011/02/24 20:48:05  jeffrey.johnson
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


entity aquisition_packer is
generic
(
  PortWidth_g                : integer := 8;
  NumberOfPorts_g            : integer := 1
);
port
(
  i_WrClk_p                  : in std_logic;
  i_RdClk_p                  : in std_logic;
  i_ResetFifo_p              : in std_logic;

  -- User interface
  i_Trigger_p                : in std_logic;
  iv_DataPort0_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort1_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort2_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort3_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort4_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort5_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort6_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort7_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort8_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort9_p             : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort10_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort11_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort12_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort13_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort14_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  iv_DataPort15_p            : in std_logic_vector(PortWidth_g - 1 downto 0);
  i_WriteEn_p                : in std_logic;
  o_FifoFull_p               : out std_logic;

  -- Ctrl Interface
  i_RecordStarted_p          : in std_logic;

  -- Interface to memory controller
  ov32_Trigger_p             : out std_logic_vector(31 downto 0);
  ov256_Data_p               : out std_logic_vector(255 downto 0);
  o_DValid_p				 : out std_logic;
  i_ReadEn_p                 : in std_logic;
  o_FifoEmpty_p              : out std_logic

);

end aquisition_packer;


architecture beh of aquisition_packer is

  constant DataWidth_c          : integer := PortWidth_g * NumberOfPorts_g;
  constant Triggers_c           : integer := DataWidth_c / 8;
  constant BlocksOf32Bits_c     : integer := DataWidth_c / 32;
  constant FifoInputWidth_c     : integer := DataWidth_c + Triggers_c;

  constant MaxDataWidth_c       : integer := PortWidth_g * 16;

  signal DataPort_s             : std_logic_vector(MaxDataWidth_c - 1 downto 0);
  signal FifoInputData_s        : std_logic_vector(FifoInputWidth_c - 1 downto 0);
  signal FifoInputReordered_s   : std_logic_vector(FifoInputWidth_c - 1 downto 0);
  signal FifoLinkData_s         : std_logic_vector(71 downto 0);
  signal FifoOutputData_s       : std_logic_vector(287 downto 0);

  signal FifoLinkValid_s        : std_logic;
  signal FifoLinkEmpty_s        : std_logic;
  signal FifoAlmostFull_s       : std_logic;
  signal FifoLinkTransferEn_s   : std_logic;

  signal RecordStartedD1_s      : std_logic;
  signal RecordStartedD2_s      : std_logic;
  signal WrEn_s                 : std_logic;

  signal FifoFull_s             : std_logic;

  signal FifoFullR1_s           : std_logic;
  signal FifoFullR2_s           : std_logic;
  signal WriteEnR1_s            : std_logic;
  signal WriteEnR2_s            : std_logic;

  signal DValid_s               : std_logic;
  signal v256_Data_s            : std_logic_vector(255 downto 0);
  signal v32_Trigger_s          : std_logic_vector(31 downto 0);
  
  signal DataPortR1_s           : std_logic_vector(MaxDataWidth_c - 1 downto 0);
  signal TriggerR1_s             : std_logic;

-- This table lists the acceptable generic values
-- and the corresponding FIFO input widths.

-------------------------------------------------
-- PortWidth_g  NumberOfPorts_g  FIFO Input width
-------------------------------------------------
--     8              1                 9
--     8              2                 18
--     8              4                 36
--     8              8                 72
--     8              16                144
-------------------------------------------------
--     16             1                 18
--     16             2                 36
--     16             4                 72
--     16             8                 144
--     16             16                288
-------------------------------------------------
--     32             1                 36
--     32             2                 72
--     32             4                 144
--     32             8                 288
-------------------------------------------------
--     64             1                 72
--     64             2                 144
--     64             4                 288
-------------------------------------------------


-- This table lists the six possible FIFO input
-- widths and the FIFO combination required.

-------------------------------------------------
-- FIFO Input width   FIFO combination
-------------------------------------------------
--     9              fifo_9_to_72  -> fifo_72_to_288
--     18             fifo_18_to_72 -> fifo_72_to_288
--     36             fifo_36_to_288
--     72             fifo_72_to_288
--     144            fifo_144_to_288
--     288            fifo_288_to_288

  -- Use to avoid unrelated LUT packing with other components
  attribute keep_hierarchy : string;
  attribute keep_hierarchy of beh : architecture is "true";

  attribute keep : string;
  attribute keep of RecordStartedD1_s : signal is "true";
  attribute keep of FifoFullR1_s : signal is "true";
  attribute keep of DataPort_s : signal is "true";
  attribute keep of TriggerR1_s : signal is "true";
  attribute keep of WriteEnR1_s : signal is "true";


begin

  WrClkFf_l : process( i_WrClk_p )
  begin
    if( rising_edge( i_WrClk_p ) ) then
      RecordStartedD1_s <= i_RecordStarted_p;
      RecordStartedD2_s <= RecordStartedD1_s;

      -- Delay almost full flag to ease timing.
      -- Since the threshold is configurable, it is safe to delay this signal
      --
      FifoFullR1_s      <= FifoFull_s;
      
      if RecordStartedD2_s = '1' then
        FifoFullR2_s    <= FifoFullR1_s;
      else
        FifoFullR2_s    <= '1';
      end if;


      -- Delay data, trigger and write enable signals to ease timing.
      -- It is safe to do this since prog_full is used.
      --

      -- Concatenate data ports to a single logic vector
      -- Facilitates use of generate for loop on input mapping
      DataPort_s((PortWidth_g *  1) - 1 downto PortWidth_g *  0)    <= iv_DataPort0_p;
      DataPort_s((PortWidth_g *  2) - 1 downto PortWidth_g *  1)    <= iv_DataPort1_p;
      DataPort_s((PortWidth_g *  3) - 1 downto PortWidth_g *  2)    <= iv_DataPort2_p;
      DataPort_s((PortWidth_g *  4) - 1 downto PortWidth_g *  3)    <= iv_DataPort3_p;
      DataPort_s((PortWidth_g *  5) - 1 downto PortWidth_g *  4)    <= iv_DataPort4_p;
      DataPort_s((PortWidth_g *  6) - 1 downto PortWidth_g *  5)    <= iv_DataPort5_p;
      DataPort_s((PortWidth_g *  7) - 1 downto PortWidth_g *  6)    <= iv_DataPort6_p;
      DataPort_s((PortWidth_g *  8) - 1 downto PortWidth_g *  7)    <= iv_DataPort7_p;
      DataPort_s((PortWidth_g *  9) - 1 downto PortWidth_g *  8)    <= iv_DataPort8_p;
      DataPort_s((PortWidth_g * 10) - 1 downto PortWidth_g *  9)    <= iv_DataPort9_p;
      DataPort_s((PortWidth_g * 11) - 1 downto PortWidth_g * 10)    <= iv_DataPort10_p;
      DataPort_s((PortWidth_g * 12) - 1 downto PortWidth_g * 11)    <= iv_DataPort11_p;
      DataPort_s((PortWidth_g * 13) - 1 downto PortWidth_g * 12)    <= iv_DataPort12_p;
      DataPort_s((PortWidth_g * 14) - 1 downto PortWidth_g * 13)    <= iv_DataPort13_p;
      DataPort_s((PortWidth_g * 15) - 1 downto PortWidth_g * 14)    <= iv_DataPort14_p;
      DataPort_s((PortWidth_g * 16) - 1 downto PortWidth_g * 15)    <= iv_DataPort15_p;
      TriggerR1_s                                                   <= i_Trigger_p;
      WriteEnR1_s                                                   <= i_WriteEn_p and (not FifoFullR2_s);
      
      DataPortR1_s                                                  <= DataPort_s;
      FifoInputData_s( 8 )                                          <= TriggerR1_s;
      FifoInputReordered_s( 8 )                                     <= TriggerR1_s;
      WriteEnR2_s                                                   <= WriteEnR1_s;

    end if;
  end process;

  o_FifoFull_p <= FifoFullR2_s;

  WrEn_s <= WriteEnR2_s;

  -- Mapping of FIFO input
  GenerateFifoDataInput_l:
    for i in 0 to Triggers_c - 1 generate
      begin
        FifoInputData_s( ((i * 9) + 7) downto (i * 9) ) <= DataPortR1_s( ((i * 8) + 7) downto (i * 8) );
    end generate;

  TrigIfGen_l : if( Triggers_c > 1 ) generate

      GenerateFifoTriggerInput_l:
        for i in 1 to Triggers_c - 1 generate
          begin
            FifoInputData_s( (i * 9) + 8 ) <= '0';
            FifoInputReordered_s( (i * 9) + 8 ) <= '0';
        end generate;
  end generate;


  -- Swap the blocks of 32 bits (keeping triggers in same place)
  GenerateIfMoreThan32BitsInput_l : if( FifoInputWidth_c >= 72 ) generate
    GenerateSwap32bitBlocks_l:
      for i in 0 to BlocksOf32Bits_c - 1 generate
        begin
          FifoInputReordered_s( ((BlocksOf32Bits_c - 1 - i) * 36) + 34 downto ((BlocksOf32Bits_c - 1 - i) * 36) + 27 ) <= FifoInputData_s( (i * 36) + 34 downto (i * 36) + 27 );
          FifoInputReordered_s( ((BlocksOf32Bits_c - 1 - i) * 36) + 25 downto ((BlocksOf32Bits_c - 1 - i) * 36) + 18 ) <= FifoInputData_s( (i * 36) + 25 downto (i * 36) + 18 );
          FifoInputReordered_s( ((BlocksOf32Bits_c - 1 - i) * 36) + 16 downto ((BlocksOf32Bits_c - 1 - i) * 36) + 9 )  <= FifoInputData_s( (i * 36) + 16 downto (i * 36) + 9 );
          FifoInputReordered_s( ((BlocksOf32Bits_c - 1 - i) * 36) + 7  downto ((BlocksOf32Bits_c - 1 - i) * 36) + 0 )  <= FifoInputData_s( (i * 36) + 7  downto (i * 36) + 0 );
      end generate;
  end generate;


  GenerateFifos9Bit_l:
    if ( FifoInputWidth_c = 9 ) generate
      begin

        FifoLinkTransferEn_s <= (not FifoLinkEmpty_s) and (not FifoAlmostFull_s);

        fifo_9_to_72_l : fifo_9_to_72
          port map (
          rst           => i_ResetFifo_p,
          wr_clk        => i_WrClk_p,
          rd_clk        => i_RdClk_p,
          din           => FifoInputData_s,
          wr_en         => WrEn_s,
          rd_en         => FifoLinkTransferEn_s,
          prog_full_thresh => "1111000",
          dout          => FifoLinkData_s,
          full          => open,
          empty         => FifoLinkEmpty_s,
          almost_full   => open,
          valid         => FifoLinkValid_s,
          prog_full     => FifoFull_s);

        fifo_72_to_288_l : fifo_72_to_288
          port map (
          rst           => i_ResetFifo_p,
          wr_clk        => i_RdClk_p,
          rd_clk        => i_RdClk_p,
          din           => FifoLinkData_s,
          wr_en         => FifoLinkValid_s,
          rd_en         => i_ReadEn_p,
          prog_full_thresh => "111111000",
          dout          => FifoOutputData_s,
          full          => open,
          almost_full   => FifoAlmostFull_s,
          empty         => o_FifoEmpty_p,
          valid 		=> DValid_s,
          prog_full     => open);

      end generate;

  GenerateFifos18Bit_l:
    if ( FifoInputWidth_c = 18 ) generate
      begin

        FifoLinkTransferEn_s <= (not FifoLinkEmpty_s) and (not FifoAlmostFull_s);

        fifo_18_to_72_l : fifo_18_to_72
          port map (
          rst           => i_ResetFifo_p,
          wr_clk        => i_WrClk_p,
          rd_clk        => i_RdClk_p,
          din           => FifoInputData_s,
          wr_en         => WrEn_s,
          rd_en         => FifoLinkTransferEn_s,
          prog_full_thresh => "1111000",
          dout          => FifoLinkData_s,
          full          => open,
          empty         => FifoLinkEmpty_s,
          almost_full   => open,
          valid         => FifoLinkValid_s,
          prog_full     => FifoFull_s);

        fifo_72_to_288_l : fifo_72_to_288
          port map (
          rst           => i_ResetFifo_p,
          wr_clk        => i_RdClk_p,
          rd_clk        => i_RdClk_p,
          din           => FifoLinkData_s,
          wr_en         => FifoLinkValid_s,
          rd_en         => i_ReadEn_p,
          prog_full_thresh => "111111000",
          dout          => FifoOutputData_s,
          full          => open,
          almost_full   => FifoAlmostFull_s,
          empty         => o_FifoEmpty_p,
          valid 		=> DValid_s,
          prog_full     => open);

      end generate;

  GenerateFifos36Bit_l:
    if ( FifoInputWidth_c = 36 ) generate
      begin

        fifo_36_to_288_l : fifo_36_to_288
          port map (
          rst           => i_ResetFifo_p,
          wr_clk        => i_WrClk_p,
          rd_clk        => i_RdClk_p,
          din           => FifoInputData_s,
          wr_en         => WrEn_s,
          rd_en         => i_ReadEn_p,
          prog_full_thresh => "111111000",
          dout          => FifoOutputData_s,
          full          => open,
          empty         => o_FifoEmpty_p,
          almost_full   => open,
          valid         => DValid_s,
          prog_full     => FifoFull_s);

      end generate;

  GenerateFifos72Bit_l:
    if ( FifoInputWidth_c = 72 ) generate
      begin

        fifo_72_to_288_l : fifo_72_to_288
          port map (
          rst           => i_ResetFifo_p,
          wr_clk        => i_WrClk_p,
          rd_clk        => i_RdClk_p,
          din           => FifoInputReordered_s,
          wr_en         => WrEn_s,
          rd_en         => i_ReadEn_p,
          prog_full_thresh => "111111000",
          dout          => FifoOutputData_s,
          full          => open,
          empty         => o_FifoEmpty_p,
          almost_full   => open,
          valid         => DValid_s,
          prog_full     => FifoFull_s);

      end generate;

  GenerateFifos144Bit_l:
    if ( FifoInputWidth_c = 144 ) generate
      begin

        fifo_144_to_288_l : fifo_144_to_288
          port map (
          rst           => i_ResetFifo_p,
          wr_clk        => i_WrClk_p,
          rd_clk        => i_RdClk_p,
          din           => FifoInputReordered_s,
          wr_en         => WrEn_s,
          rd_en         => i_ReadEn_p,
          prog_full_thresh => "111111000",
          dout          => FifoOutputData_s,
          full          => open,
          empty         => o_FifoEmpty_p,
          almost_full   => open,
          valid         => DValid_s,
          prog_full     => FifoFull_s);

      end generate;

  GenerateFifos288Bit_l:
    if ( FifoInputWidth_c = 288 ) generate
      begin

        fifo_288_to_288_l : fifo_288_to_288
          port map (
          rst           => i_ResetFifo_p,
          wr_clk        => i_WrClk_p,
          rd_clk        => i_RdClk_p,
          din           => FifoInputReordered_s,
          wr_en         => WrEn_s,
          rd_en         => i_ReadEn_p,
          prog_full_thresh => "111111000",
          dout          => FifoOutputData_s,
          full          => open,
          empty         => o_FifoEmpty_p,
          almost_full   => open,
          valid         => DValid_s,
          prog_full     => FifoFull_s);

      end generate;

  -- Mapping of FIFO output
  GenerateFifoOutput_l:
    for i in 0 to 31 generate
      begin
        v256_Data_s( ((i * 8) + 7) downto (i * 8) ) <= FifoOutputData_s( ((i * 9) + 7) downto (i * 9) );
        v32_Trigger_s( i ) <= FifoOutputData_s( (i * 9) + 8 );
    end generate;

  process(i_RdClk_p)
  begin
    if rising_edge(i_RdClk_p) then
      o_DValid_p      <= DValid_s;
      ov256_Data_p    <= v256_Data_s;
      ov32_Trigger_p  <= v32_Trigger_s;
    end if;
  end process;

end beh;
