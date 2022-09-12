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
--                       L Y R T E C H   R D
--
--------------------------------------------------------------------------------
-- File        : $Id: generic_fifo.vhd,v 1.1 2015/04/10 20:11:55 sqa Exp $
--------------------------------------------------------------------------------
-- Description : Generic Fifo
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2012 Lyrtech RD.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- $Log: generic_fifo.vhd,v $
-- Revision 1.1  2015/04/10 20:11:55  sqa
-- bas first commit
--
-- Revision 1.1  2013/07/22 20:04:36  david.quinn
-- Generic FIFO
--
-- Revision 1.4  2013/06/03 20:01:23  david.quinn
-- Add support to 32k fifo depth
--
-- Revision 1.3  2013/03/22 18:57:00  julien.roy
-- Change some fifo components to match selected size
--
-- Revision 1.2  2013/03/05 18:43:15  david.quinn
-- Add configurable full status port.
--
-- Revision 1.1  2012/12/07 20:36:30  david.quinn
-- First commit.
--
--
--------------------------------------------------------------------------------

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;


entity generic_fifo is
  generic (
    WRITE_WIDTH_g             : integer := 64;
    READ_WIDTH_g              : integer := 32;
    WRITE_DEPTH_g             : integer := 16384;
    FIRST_WORD_FALL_THROUGH_g : boolean := false;
    PROG_FULL_THRESH_g        : integer := 16384 - 4
  );
  port (
    i_rst_p                   : in std_logic;
    i_wr_clk_p                : in std_logic;
    i_rd_clk_p                : in std_logic;
    iv_din_p                  : in std_logic_vector(WRITE_WIDTH_g-1 downto 0);
    i_wr_en_p                 : in std_logic;
    i_rd_en_p                 : in std_logic;
    ov_dout_p                 : out std_logic_vector(READ_WIDTH_g-1 downto 0);
    o_full_p                  : out std_logic;
    o_prog_full_p             : out std_logic;
    o_almost_full_p           : out std_logic;
    o_overflow_p              : out std_logic;
    o_empty_p                 : out std_logic;
    o_valid_p                 : out std_logic;
    o_underflow_p             : out std_logic
  );
end generic_fifo;

architecture arch of generic_fifo is


  ------------------------------------------------------------------
  -- Functions
  ------------------------------------------------------------------
  
  -- Find minimum number of bits required to represent N as an unsigned 
  -- binary number.
  -- simple recursive implementation...
  --
  function log2_ceil(N: natural) return positive is
    begin
      if (N < 2) then
        return 1;
      else
        return 1 + log2_ceil(N/2);
      end if;
    end function log2_ceil;
  
  
  ------------------------------------------------------------------
  -- FIFOs read 32-bit, write 32-bit
  ------------------------------------------------------------------
  
  component fifo512_rw32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(8 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(8 downto 0);
      wr_data_count : out std_logic_vector(8 downto 0)
    );
  end component;

  component fifo1k_rw32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(9 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(9 downto 0);
      wr_data_count : out std_logic_vector(9 downto 0)
    );
  end component;

  component fifo2k_rw32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(10 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(10 downto 0);
      wr_data_count : out std_logic_vector(10 downto 0)
    );
  end component;

  component fifo4k_rw32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(11 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(11 downto 0);
      wr_data_count : out std_logic_vector(11 downto 0)
    );
  end component;

  component fifo8k_rw32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(12 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(12 downto 0);
      wr_data_count : out std_logic_vector(12 downto 0)
    );
  end component;

  component fifo16k_rw32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(13 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(13 downto 0);
      wr_data_count : out std_logic_vector(13 downto 0)
    );
  end component;

  component fifo32k_rw32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(14 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(14 downto 0);
      wr_data_count : out std_logic_vector(14 downto 0)
    );
  end component;

  component fifo512_rw32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(8 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(8 downto 0);
      wr_data_count : out std_logic_vector(8 downto 0)
    );
  end component;
  
  component fifo1k_rw32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(9 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(9 downto 0);
      wr_data_count : out std_logic_vector(9 downto 0)
    );
  end component;

  component fifo2k_rw32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(10 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(10 downto 0);
      wr_data_count : out std_logic_vector(10 downto 0)
    );
  end component;

  component fifo4k_rw32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(11 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(11 downto 0);
      wr_data_count : out std_logic_vector(11 downto 0)
    );
  end component;

  component fifo8k_rw32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(12 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(12 downto 0);
      wr_data_count : out std_logic_vector(12 downto 0)
    );
  end component;

  component fifo16k_rw32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(13 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(13 downto 0);
      wr_data_count : out std_logic_vector(13 downto 0)
    );
  end component;

  component fifo32k_rw32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(14 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(14 downto 0);
      wr_data_count : out std_logic_vector(14 downto 0)
    );
  end component;
  
  
  ------------------------------------------------------------------
  -- FIFOs read 32-bit, write 64-bit
  ------------------------------------------------------------------

  component fifo512_r32_w64_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(8 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(9 downto 0);
      wr_data_count : out std_logic_vector(8 downto 0)
    );
  end component;

  component fifo1k_r32_w64_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(9 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(10 downto 0);
      wr_data_count : out std_logic_vector(9 downto 0)
    );
  end component;

  component fifo2k_r32_w64_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(10 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(11 downto 0);
      wr_data_count : out std_logic_vector(10 downto 0)
    );
  end component;

  component fifo4k_r32_w64_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(11 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(12 downto 0);
      wr_data_count : out std_logic_vector(11 downto 0)
    );
  end component;

  component fifo8k_r32_w64_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(12 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(13 downto 0);
      wr_data_count : out std_logic_vector(12 downto 0)
    );
  end component;

  component fifo16k_r32_w64_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(13 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(14 downto 0);
      wr_data_count : out std_logic_vector(13 downto 0)
    );
  end component;

  component fifo32k_r32_w64_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(14 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(15 downto 0);
      wr_data_count : out std_logic_vector(14 downto 0)
    );
  end component;

  component fifo512_r32_w64_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(8 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(9 downto 0);
      wr_data_count : out std_logic_vector(8 downto 0)
    );
  end component;
  
  component fifo1k_r32_w64_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(9 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(10 downto 0);
      wr_data_count : out std_logic_vector(9 downto 0)
    );
  end component;

  component fifo2k_r32_w64_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(10 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(11 downto 0);
      wr_data_count : out std_logic_vector(10 downto 0)
    );
  end component;

  component fifo4k_r32_w64_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(11 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(12 downto 0);
      wr_data_count : out std_logic_vector(11 downto 0)
    );
  end component;

  component fifo8k_r32_w64_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(12 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(13 downto 0);
      wr_data_count : out std_logic_vector(12 downto 0)
    );
  end component;

  component fifo16k_r32_w64_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(13 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(14 downto 0);
      wr_data_count : out std_logic_vector(13 downto 0)
    );
  end component;
  
  component fifo32k_r32_w64_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(63 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(14 downto 0);
      dout : out std_logic_vector(31 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(15 downto 0);
      wr_data_count : out std_logic_vector(14 downto 0)
    );
  end component;

  
  ------------------------------------------------------------------
  -- FIFOs read 64-bit, write 32-bit
  ------------------------------------------------------------------

  component fifo512_r64_w32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(8 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(7 downto 0);
      wr_data_count : out std_logic_vector(8 downto 0)
    );
  end component;
  
  component fifo1k_r64_w32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(9 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(8 downto 0);
      wr_data_count : out std_logic_vector(9 downto 0)
    );
  end component;

  component fifo2k_r64_w32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(10 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(9 downto 0);
      wr_data_count : out std_logic_vector(10 downto 0)
    );
  end component;

  component fifo4k_r64_w32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(11 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(10 downto 0);
      wr_data_count : out std_logic_vector(11 downto 0)
    );
  end component;

  component fifo8k_r64_w32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(12 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(11 downto 0);
      wr_data_count : out std_logic_vector(12 downto 0)
    );
  end component;

  component fifo16k_r64_w32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(13 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(12 downto 0);
      wr_data_count : out std_logic_vector(13 downto 0)
    );
  end component;

  component fifo32k_r64_w32_async
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(14 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(13 downto 0);
      wr_data_count : out std_logic_vector(14 downto 0)
    );
  end component;
  
  component fifo512_r64_w32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(8 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(7 downto 0);
      wr_data_count : out std_logic_vector(8 downto 0)
    );
  end component;
  
  component fifo1k_r64_w32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(9 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(8 downto 0);
      wr_data_count : out std_logic_vector(9 downto 0)
    );
  end component;

  component fifo2k_r64_w32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(10 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(9 downto 0);
      wr_data_count : out std_logic_vector(10 downto 0)
    );
  end component;

  component fifo4k_r64_w32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(11 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(10 downto 0);
      wr_data_count : out std_logic_vector(11 downto 0)
    );
  end component;

  component fifo8k_r64_w32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(12 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(11 downto 0);
      wr_data_count : out std_logic_vector(12 downto 0)
    );
  end component;

  component fifo16k_r64_w32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(13 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(12 downto 0);
      wr_data_count : out std_logic_vector(13 downto 0)
    );
  end component;

  component fifo32k_r64_w32_async_fwft
    port (
      rst : in std_logic;
      wr_clk : in std_logic;
      rd_clk : in std_logic;
      din : in std_logic_vector(31 downto 0);
      wr_en : in std_logic;
      rd_en : in std_logic;
      prog_full_thresh : in std_logic_vector(14 downto 0);
      dout : out std_logic_vector(63 downto 0);
      full : out std_logic;
      prog_full : out std_logic;
      almost_full : out std_logic;
      overflow : out std_logic;
      empty : out std_logic;
      valid : out std_logic;
      underflow : out std_logic;
      rd_data_count : out std_logic_vector(13 downto 0);
      wr_data_count : out std_logic_vector(14 downto 0)
    );
  end component;
  

  ------------------------------------------------------------------
  -- Read / Write data counts
  ------------------------------------------------------------------

  signal rd_data_count : std_logic_vector(15 downto 0);
  signal wr_data_count : std_logic_vector(15 downto 0);
  
begin


  --------------------------------------------------------------------------------
  -- Generic parameters validation
  --------------------------------------------------------------------------------

  -- FIFO depht validation
  --

  assert (WRITE_DEPTH_g = 512) or
         (WRITE_DEPTH_g = 1024) or
         (WRITE_DEPTH_g = 2048) or
         (WRITE_DEPTH_g = 4096) or
         (WRITE_DEPTH_g = 8192) or
         (WRITE_DEPTH_g = 16384) or
         (WRITE_DEPTH_g = 32768)
  report "invalid FIFO size (write depth)"
  severity failure;


  -- FIFO width validation
  --
  
  assert ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) or
         ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) or
         ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64))
  report "invalid FIFO write/read port widths"
  severity failure;


  --------------------------------------------------------------------------------
  -- FIFO instantiation
  --------------------------------------------------------------------------------

   Fifo512: if (WRITE_DEPTH_g = 512 and not FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo512_rw32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64: if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo512_r32_w64_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo512_r64_w32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;

   end generate Fifo512;
   

   Fifo512_FWFT: if (WRITE_DEPTH_g = 512 and FIRST_WORD_FALL_THROUGH_g) generate
   
     read_32_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo512_rw32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64: if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo512_r32_w64_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo512_r64_w32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;

   end generate Fifo512_FWFT;


   Fifo1K: if (WRITE_DEPTH_g = 1024 and not FIRST_WORD_FALL_THROUGH_g) generate
   
     read_32_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo1k_rw32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64: if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo1k_r32_w64_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo1k_r64_w32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;

   end generate Fifo1K;
   

   Fifo1K_FWFT: if (WRITE_DEPTH_g = 1024 and FIRST_WORD_FALL_THROUGH_g) generate
   
     read_32_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo1k_rw32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64: if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo1k_r32_w64_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo1k_r64_w32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;

   end generate Fifo1K_FWFT;

   
   Fifo2K: if (WRITE_DEPTH_g = 2048 and not FIRST_WORD_FALL_THROUGH_g) generate    
    
     read_32_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
        U0_ChanFifo: fifo2k_rw32_async
          port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64: if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
        U0_ChanFifo: fifo2k_r32_w64_async
          port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
        U0_ChanFifo: fifo2k_r64_w32_async
          port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo2K;


   Fifo2K_FWFT: if (WRITE_DEPTH_g = 2048 and FIRST_WORD_FALL_THROUGH_g) generate    
    
     read_32_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
        U0_ChanFifo: fifo2k_rw32_async_fwft
          port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64: if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
        U0_ChanFifo: fifo2k_r32_w64_async_fwft
          port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32: if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
        U0_ChanFifo: fifo2k_r64_w32_async_fwft
          port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo2K_FWFT;

   
   Fifo4K: if (WRITE_DEPTH_g = 4096 and not FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo4k_rw32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64 : if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo4k_r32_w64_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo4k_r64_w32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo4K;


   Fifo4K_FWFT: if (WRITE_DEPTH_g = 4096 and FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo4k_rw32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64 : if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo4k_r32_w64_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo4k_r64_w32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo4K_FWFT;

   
   Fifo8K: if (WRITE_DEPTH_g = 8192 and not FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo8k_rw32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64 : if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo8k_r32_w64_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo8k_r64_w32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo8K;


   Fifo8K_FWFT: if (WRITE_DEPTH_g = 8192 and FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo8k_rw32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64 : if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo8k_r32_w64_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo8k_r64_w32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo8K_FWFT;

   
   Fifo16K: if (WRITE_DEPTH_g = 16384 and not FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo16k_rw32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64 : if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo16k_r32_w64_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo16k_r64_w32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo16K;


   Fifo16K_FWFT: if (WRITE_DEPTH_g = 16384 and FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo16k_rw32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64 : if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo16k_r32_w64_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo16k_r64_w32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo16K_FWFT;







   Fifo32K: if (WRITE_DEPTH_g = 32768 and not FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo32k_rw32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64 : if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo32k_r32_w64_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo32k_r64_w32_async
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo32K;


   Fifo32K_FWFT: if (WRITE_DEPTH_g = 32768 and FIRST_WORD_FALL_THROUGH_g) generate

     read_32_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo32k_rw32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_32;
     
     read_32_write_64 : if ((WRITE_WIDTH_g = 64) and (READ_WIDTH_g = 32)) generate
       U0_ChanFifo: fifo32k_r32_w64_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_32_write_64;

     read_64_write_32 : if ((WRITE_WIDTH_g = 32) and (READ_WIDTH_g = 64)) generate
       U0_ChanFifo: fifo32k_r64_w32_async_fwft
         port map (
           rst          => i_rst_p,
           wr_clk       => i_wr_clk_p,
           rd_clk       => i_rd_clk_p,
           din          => iv_din_p,
           wr_en        => i_wr_en_p,
           rd_en        => i_rd_en_p,
           prog_full_thresh => std_logic_vector(to_unsigned(PROG_FULL_THRESH_g, log2_ceil(WRITE_DEPTH_g)-1)),
           dout         => ov_dout_p,
           full         => o_full_p,
           prog_full    => o_prog_full_p,
           almost_full  => o_almost_full_p,
           overflow     => o_overflow_p,
           empty        => o_empty_p,
           valid        => o_valid_p,
           underflow    => o_underflow_p,
           rd_data_count => open,
           wr_data_count => open
         );
     end generate read_64_write_32;
     
   end generate Fifo32K_FWFT;

end arch;