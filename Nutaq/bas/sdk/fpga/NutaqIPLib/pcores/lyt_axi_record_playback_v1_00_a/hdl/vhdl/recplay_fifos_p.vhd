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
-- File        : $Id: recplay_fifos_p.vhd,v 1.2 2015/08/04 14:00:27 julien.roy Exp $
--------------------------------------------------------------------------------
-- Description : Record Playback FIFOs
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2011 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- François Blackburn - Initial revision 2011/02/22
-- $Log: recplay_fifos_p.vhd,v $
-- Revision 1.2  2015/08/04 14:00:27  julien.roy
-- Add programmable full port
--
-- Revision 1.1  2015/04/10 19:48:36  sqa
-- bas first commit
--
-- Revision 1.15  2015/01/13 14:33:58  julien.roy
-- Add programmable full port.
--
-- Revision 1.14  2014/09/11 13:22:29  julien.roy
-- Change playback fifo depth to 512 to avoid starvation during refresh or page change of the ddr3.
--
-- Revision 1.13  2013/05/24 14:24:53  julien.roy
-- pull out the buffer input mux out of mem_controller
--
-- Revision 1.12  2013/05/23 12:22:52  julien.roy
-- Add fifo reset output port
--
-- Revision 1.11  2013/05/08 18:06:49  julien.roy
-- Modify record playback memory controller to solve timing errors
--
-- Revision 1.10  2013/04/09 20:34:51  julien.roy
-- Add status registers
--
-- Revision 1.9  2013/03/15 19:38:27  julien.roy
-- Modification to support 4GB record playback
--
-- Revision 1.8  2013/01/31 16:35:24  khalid.bensadek
-- Corrected bugs related to bus_width x Port_Number < 32 bits. Corrected endianness issue for these configurations. Corrected other bugs that were found during this process: Fifo under reset and continuing to write data anyway (Reset edge sensitive).
--
-- Revision 1.7  2012/12/12 03:25:19  khalid.bensadek
-- Solved timings issues with big FPGA, SX475.
--
-- Revision 1.6  2012/12/03 19:44:14  khalid.bensadek
-- Trig pos, address and ofset bug fixed + some other bugs. Trig dly still to be dbg.
--
-- Revision 1.5  2012/10/12 12:19:09  khalid.bensadek
-- Removed the recently added fifo and changed acquisition packer fifo's to be FWFT to accomodate high truput (Prevent user FIFO full).
--
-- Revision 1.4  2012/10/02 19:57:59  khalid.bensadek
-- added component declaration
--
-- Revision 1.3  2011/03/08 16:08:21  jeffrey.johnson
-- Increased FIFO depth.
--
-- Revision 1.2  2011/02/24 20:48:46  jeffrey.johnson
-- Changed FIFO definitions.
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;

package recplay_fifos_p is

  component fifo128_w32_r256_std_ff
  port (
    rst : in std_logic;
    wr_clk : in std_logic;
    rd_clk : in std_logic;
    din : in std_logic_vector(31 downto 0);
    wr_en : in std_logic;
    rd_en : in std_logic;
    prog_full_thresh : in std_logic_vector(9 downto 0);
    dout : out std_logic_vector(255 downto 0);
    full : out std_logic;
    empty : out std_logic;
    valid : out std_logic;
    prog_full : out std_logic
  );
  end component;

  component fifo64_w256_r32_std_ff is
  port
  (
    rst: in std_logic;
    wr_clk: in std_logic;
    rd_clk: in std_logic;
    din: in std_logic_vector(255 downto 0);
    wr_en: in std_logic;
    rd_en: in std_logic;
    dout: out std_logic_vector(31 downto 0);
    full: out std_logic;
    empty: out std_logic;
    valid: out std_logic;
    prog_full: out std_logic
  );
  end component fifo64_w256_r32_std_ff;
  
  component fifo_9_to_72
  port (
    rst : in std_logic;
    wr_clk : in std_logic;
    rd_clk : in std_logic;
    din : in std_logic_vector(8 downto 0);
    wr_en : in std_logic;
    rd_en : in std_logic;
    prog_full_thresh : in std_logic_vector(6 downto 0);
    dout : out std_logic_vector(71 downto 0);
    full : out std_logic;
    almost_full : out std_logic;
    empty : out std_logic;
    valid : out std_logic;
    prog_full : out std_logic
  );
  end component;
  
  component fifo_18_to_72
  port (
    rst : in std_logic;
    wr_clk : in std_logic;
    rd_clk : in std_logic;
    din : in std_logic_vector(17 downto 0);
    wr_en : in std_logic;
    rd_en : in std_logic;
    prog_full_thresh : in std_logic_vector(6 downto 0);
    dout : out std_logic_vector(71 downto 0);
    full : out std_logic;
    almost_full : out std_logic;
    empty : out std_logic;
    valid : out std_logic;
    prog_full : out std_logic
  );
  end component;

  component fifo_36_to_288
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(35 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    prog_full_thresh : in std_logic_vector(8 downto 0);
    dout: OUT std_logic_VECTOR(287 downto 0);
    full: OUT std_logic;
    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full : out std_logic);
  end component;

  component fifo_72_to_288
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(71 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    prog_full_thresh : in std_logic_vector(8 downto 0);
    dout: OUT std_logic_VECTOR(287 downto 0);
    full: OUT std_logic;
    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full : out std_logic);
  end component;

  component fifo_144_to_288
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(143 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    prog_full_thresh : in std_logic_vector(8 downto 0);
    dout: OUT std_logic_VECTOR(287 downto 0);
    full: OUT std_logic;
    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full : out std_logic);
  end component;

  component fifo_288_to_288
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(287 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    prog_full_thresh : in std_logic_vector(8 downto 0);
    dout: OUT std_logic_VECTOR(287 downto 0);
    full: OUT std_logic;
    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full : out std_logic);    
  end component;

  component fifo_64_to_8
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(63 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    dout: OUT std_logic_VECTOR(7 downto 0);
    full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full : OUT STD_LOGIC);
  end component;

  component fifo_64_to_16
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(63 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    dout: OUT std_logic_VECTOR(15 downto 0);
    full: OUT std_logic;
--    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full : OUT STD_LOGIC);
  end component;

  component fifo_256_to_32
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(255 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    dout: OUT std_logic_VECTOR(31 downto 0);
    full: OUT std_logic;
    prog_full_thresh_assert: IN std_logic_VECTOR(8 downto 0);
    prog_full_thresh_negate: IN std_logic_VECTOR(8 downto 0);
    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full: OUT std_logic);
  end component;

  component fifo_256_to_64
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(255 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    dout: OUT std_logic_VECTOR(63 downto 0);
    full: OUT std_logic;
    prog_full_thresh_assert: IN std_logic_VECTOR(8 downto 0);
    prog_full_thresh_negate: IN std_logic_VECTOR(8 downto 0);
    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full: OUT std_logic);
  end component;

  component fifo_256_to_128
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(255 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    dout: OUT std_logic_VECTOR(127 downto 0);
    full: OUT std_logic;
    prog_full_thresh_assert: IN std_logic_VECTOR(8 downto 0);
    prog_full_thresh_negate: IN std_logic_VECTOR(8 downto 0);
    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full: OUT std_logic);
  end component;

  component fifo_256_to_256
    port (
    rst: IN std_logic;
    wr_clk: IN std_logic;
    rd_clk: IN std_logic;
    din: IN std_logic_VECTOR(255 downto 0);
    wr_en: IN std_logic;
    rd_en: IN std_logic;
    dout: OUT std_logic_VECTOR(255 downto 0);
    full: OUT std_logic;
    prog_full_thresh_assert: IN std_logic_VECTOR(8 downto 0);
    prog_full_thresh_negate: IN std_logic_VECTOR(8 downto 0);
    almost_full: OUT std_logic;
    empty: OUT std_logic;
    valid: OUT std_logic;
    prog_full: OUT std_logic);
  end component;

  component mem_writer_controller
    generic
    (
      ADDR_WIDTH  : integer := 30
    );
    port
    (
      --  FIFO interface
      iv32_TriggerIn_p 		: in std_logic_vector(31 downto 0);
      iv256_Data_p 			: in std_logic_vector(255 downto 0);
      i_DValid_p				: in std_logic;
      o_ReadData_p 			: out std_logic;     
      i_FifoEmpty_p 			: in std_logic;

      -- Control and registers
      i_Reset_p 				: in std_logic;
      i_RecordEn_p 			: in std_logic;     -- This is a pulse signal
      iv_StartAddress_p 	: in std_logic_vector(ADDR_WIDTH-1 downto 0);
      iv_TrigDly_p 		: in std_logic_vector(ADDR_WIDTH downto 0);     
      iv_TransferSize_p 	: in std_logic_vector(ADDR_WIDTH-1 downto 0);
      ov_TrigAddr_p 		: out std_logic_vector(ADDR_WIDTH-1 downto 0);
      ov32_TrigAddrIndex_p 	: out std_logic_vector(31 downto 0);
      o_TransferOver_p 		: out std_logic;
      o_ParityAddrReg_p 		: out std_logic;
      o_AcquisitionOn_p : out std_logic;
      ov32_StorageCnt_p : out std_logic_vector(31 downto 0);


      -- memory interface
      i_MemClk_p 			: in std_logic;

      o_AppWdfWren_p     	: out std_logic;
      ov256_AppWdfData_p 	: out std_logic_vector(255 downto 0);
      o_AppWdfEnd_p      	: out std_logic;
      ov3_AppCmd_p       	: out std_logic_vector(2 downto 0);
      o_AppEn_p          	: out std_logic;
      ov_TgAddr_p      	  : out std_logic_vector(ADDR_WIDTH-1 downto 0);

      i_AppRdy_p 			: in std_logic;
      i_AppWdfRdy_p 			: in std_logic;

      i_PhyInitDone_p 		: in std_logic     
    );
  end component;

  component mem_reader_controller
    generic
    (
      ADDR_WIDTH  : integer := 30
    );
    port
    (

         --  FIFO interface
         ov256_FifoWrData_p : out std_logic_vector(255 downto 0);
         o_FifoWrite_p : out std_logic;
         o_ResetFifo_p : out std_logic;
         i_FifoProgFull_p : in std_logic;

         -- Control and registers
         i_TriggerIn_p : in std_logic;
         i_PlayStart_p : in std_logic;
         i_Reset_p : in std_logic;
         i_ModeIsContinuous_p : in std_logic;     
         iv_StartAddress_p : in std_logic_vector(ADDR_WIDTH-1 downto 0);
         iv_TransferSize_p : in std_logic_vector(ADDR_WIDTH-1 downto 0);
         o_TransferOver_p : out std_logic;
         ov32_ReadCnt_p : out std_logic_vector(31 downto 0);

         -- memory interface
         i_MemClk_p : in std_logic;

         ov3_AppCmd_p : out std_logic_vector(2 downto 0);
         o_AppEn_p  : out std_logic;
         ov_TgAddr_p : out std_logic_vector(ADDR_WIDTH-1 downto 0);

         i_AppRdy_p :          in std_logic;
         iv256_AppRdData_p :    in std_logic_vector(255 downto 0);
         i_AppRdDataValid_p :   in std_logic;

         i_PhyInitDone_p :      in std_logic


    );
  end component;
  
  component mem_controller is
    generic
    (
      ADDR_WIDTH  : integer := 30
    );
    port
    (
     -- Control and registers
     i_Reset_p 				    : in std_logic;
     i_MemClk_p 			    : in std_logic;
     iv_StartAddress_p 	  : in std_logic_vector(ADDR_WIDTH-1 downto 0);
     iv_TransferSize_p 	  : in std_logic_vector(ADDR_WIDTH-1 downto 0);
     o_TransferOver_p 		: out std_logic;
     
     iv288_BufferDin_p	  : in std_logic_vector(287 downto 0);
     i_BufferWen_p        : in std_logic;

     --  Record FIFO interface
     o_Record_ResetFifo_p         : out std_logic;
     -- iv32_Record_TriggerIn_p 		  : in std_logic_vector(31 downto 0);
     -- iv256_Record_Data_p 			    : in std_logic_vector(255 downto 0);
     -- i_Record_DValid_p				    : in std_logic;
     -- o_Record_ReadData_p 			    : out std_logic;
     -- i_Record_FifoEmpty_p 			  : in std_logic;

     --  RtdexWr FIFO interface
     o_RtdexWr_ResetFifo_p        : out std_logic;
     -- iv256_RtdexWr_Data_p 			  : in std_logic_vector(255 downto 0);
     -- i_RtdexWr_DValid_p				    : in std_logic;
     -- o_RtdexWr_ReadData_p 			  : out std_logic;
     -- i_RtdexWr_FifoEmpty_p 			  : in std_logic;

     --  Playback FIFO interface
     ov256_Playback_FifoWrData_p  : out std_logic_vector(255 downto 0);
     o_Playback_FifoWrite_p       : out std_logic;
     o_Playback_ResetFifo_p       : out std_logic;
     i_Playback_FifoProgFull_p    : in std_logic;

     --  RtdexRd FIFO interface
     ov256_RtdexRd_FifoWrData_p  : out std_logic_vector(255 downto 0);
     o_RtdexRd_FifoWrite_p       : out std_logic;
     o_RtdexRd_ResetFifo_p       : out std_logic;
     i_RtdexRd_FifoProgFull_p    : in std_logic;

     -- Control and status
     o_BufferProgFull_p         : out std_logic;
     o_Record_On_p              : out std_logic;
     o_RtdexWr_On_p             : out std_logic;
     i_Record_En_p 			        : in std_logic;     -- This is a pulse signal
     i_RtdexWr_En_p 			      : in std_logic;     -- This is a pulse signal
     i_Playback_En_p 			      : in std_logic;     -- This is a pulse signal
     i_RtdexRd_En_p 			      : in std_logic;     -- This is a pulse signal
     i_ModeIsContinuous_p       : in std_logic;
     ov32_Record_Cnt_p          : out std_logic_vector(31 downto 0);
     ov32_RtdexWr_Cnt_p         : out std_logic_vector(31 downto 0);
     ov32_Playback_Cnt_p        : out std_logic_vector(31 downto 0);
     ov32_RtdexRd_Cnt_p         : out std_logic_vector(31 downto 0);

     -- Trigger control and status for record mode
     iv_Record_TrigDly_p 		      : in std_logic_vector(ADDR_WIDTH downto 0);
     ov_Record_TrigAddr_p 		    : out std_logic_vector(ADDR_WIDTH-1 downto 0);
     ov32_Record_TrigAddrIndex_p 	: out std_logic_vector(31 downto 0);
     o_Record_ParityAddrReg_p 		: out std_logic;

     i_PlaybackTriggerIn_p        : in std_logic;


     -- memory interface
     o_AppWdfWren_p     	: out std_logic;
     ov256_AppWdfData_p 	: out std_logic_vector(255 downto 0);
     o_AppWdfEnd_p      	: out std_logic;
     ov3_AppCmd_p       	: out std_logic_vector(2 downto 0);
     o_AppEn_p          	: out std_logic;
     ov_TgAddr_p      	  : out std_logic_vector(ADDR_WIDTH-1 downto 0);
     iv256_AppRdData_p    : in std_logic_vector(255 downto 0);
     i_AppRdDataValid_p   : in std_logic;

     i_AppRdy_p 			    : in std_logic;
     i_AppWdfRdy_p 			  : in std_logic;
     i_PhyInitDone_p 		  : in std_logic
    );
  end component;


end package recplay_fifos_p;

