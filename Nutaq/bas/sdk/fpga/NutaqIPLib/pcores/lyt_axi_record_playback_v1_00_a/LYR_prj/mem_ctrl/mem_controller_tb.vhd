--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   11:56:06 05/06/2013
-- Design Name:   
-- Module Name:   C:/projets/adp/fpga/Nutaq_ip_cores/pcores/lyt_axi_record_playback_v1_00_a/LYR_prj/mem_ctrl/mem_controller_tb.vhd
-- Project Name:  mem_controller
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: mem_controller
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 use ieee.numeric_std.all;
 use ieee.std_logic_misc.all;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY mem_controller_tb IS
END mem_controller_tb;
 
ARCHITECTURE behavior OF mem_controller_tb IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT mem_controller
    PORT(
         i_Reset_p : IN  std_logic;
         i_MemClk_p : IN  std_logic;
         iv_StartAddress_p : IN  std_logic_vector(29 downto 0);
         iv_TransferSize_p : IN  std_logic_vector(29 downto 0);
         o_TransferOver_p : OUT  std_logic;
         iv32_Record_TriggerIn_p : IN  std_logic_vector(31 downto 0);
         iv256_Record_Data_p : IN  std_logic_vector(255 downto 0);
         i_Record_DValid_p : IN  std_logic;
         o_Record_ReadData_p : OUT  std_logic;
         i_Record_FifoEmpty_p : IN  std_logic;
         iv256_RtdexWr_Data_p : IN  std_logic_vector(255 downto 0);
         i_RtdexWr_DValid_p : IN  std_logic;
         o_RtdexWr_ReadData_p : OUT  std_logic;
         i_RtdexWr_FifoEmpty_p : IN  std_logic;
         ov256_Playback_FifoWrData_p : OUT  std_logic_vector(255 downto 0);
         o_Playback_FifoWrite_p : OUT  std_logic;
         o_Playback_ResetFifo_p : OUT  std_logic;
         i_Playback_FifoProgFull_p : IN  std_logic;
         ov256_RtdexRd_FifoWrData_p : OUT  std_logic_vector(255 downto 0);
         o_RtdexRd_FifoWrite_p : OUT  std_logic;
         o_RtdexRd_ResetFifo_p : OUT  std_logic;
         i_RtdexRd_FifoProgFull_p : IN  std_logic;
         o_Record_On_p : OUT  std_logic;
         i_Record_En_p : IN  std_logic;
         i_RtdexWr_En_p : IN  std_logic;
         i_Playback_En_p : IN  std_logic;
         i_RtdexRd_En_p : IN  std_logic;
         i_ModeIsContinuous_p : IN  std_logic;
         ov32_Record_Cnt_p : OUT  std_logic_vector(31 downto 0);
         ov32_RtdexWr_Cnt_p : OUT  std_logic_vector(31 downto 0);
         ov32_Playback_Cnt_p : OUT  std_logic_vector(31 downto 0);
         ov32_RtdexRd_Cnt_p : OUT  std_logic_vector(31 downto 0);
         iv_Record_TrigDly_p : IN  std_logic_vector(30 downto 0);
         ov_Record_TrigAddr_p : OUT  std_logic_vector(29 downto 0);
         ov32_Record_TrigAddrIndex_p : OUT  std_logic_vector(31 downto 0);
         o_Record_ParityAddrReg_p : OUT  std_logic;
         i_PlaybackTriggerIn_p : IN  std_logic;
         o_AppWdfWren_p : OUT  std_logic;
         ov256_AppWdfData_p : OUT  std_logic_vector(255 downto 0);
         o_AppWdfEnd_p : OUT  std_logic;
         ov3_AppCmd_p : OUT  std_logic_vector(2 downto 0);
         o_AppEn_p : OUT  std_logic;
         ov_TgAddr_p : OUT  std_logic_vector(29 downto 0);
         iv256_AppRdData_p : IN  std_logic_vector(255 downto 0);
         i_AppRdDataValid_p : IN  std_logic;
         i_AppRdy_p : IN  std_logic;
         i_AppWdfRdy_p : IN  std_logic;
         i_PhyInitDone_p : IN  std_logic
        );
    END COMPONENT;
    

   --Inputs
   signal i_Reset_p : std_logic := '0';
   signal i_MemClk_p : std_logic := '0';
   signal iv_StartAddress_p : std_logic_vector(29 downto 0) := (others => '0');
   signal iv_TransferSize_p : std_logic_vector(29 downto 0) := (others => '0');
   signal iv32_Record_TriggerIn_p : std_logic_vector(31 downto 0) := (others => '0');
   signal iv256_Record_Data_p : std_logic_vector(255 downto 0) := (others => '0');
   signal i_Record_DValid_p : std_logic := '0';
   signal i_Record_FifoEmpty_p : std_logic := '0';
   signal iv256_RtdexWr_Data_p : std_logic_vector(255 downto 0) := (others => '0');
   signal i_RtdexWr_DValid_p : std_logic := '0';
   signal i_RtdexWr_FifoEmpty_p : std_logic := '0';
   signal i_Playback_FifoProgFull_p : std_logic := '0';
   signal i_RtdexRd_FifoProgFull_p : std_logic := '0';
   signal i_Record_En_p : std_logic := '0';
   signal i_RtdexWr_En_p : std_logic := '0';
   signal i_Playback_En_p : std_logic := '0';
   signal i_RtdexRd_En_p : std_logic := '0';
   signal i_ModeIsContinuous_p : std_logic := '0';
   signal iv_Record_TrigDly_p : std_logic_vector(30 downto 0) := (others => '0');
   signal i_PlaybackTriggerIn_p : std_logic := '0';
   signal iv256_AppRdData_p : std_logic_vector(255 downto 0) := (others => '0');
   signal i_AppRdDataValid_p : std_logic := '0';
   signal i_AppRdy_p : std_logic := '0';
   signal i_AppWdfRdy_p : std_logic := '0';
   signal i_PhyInitDone_p : std_logic := '0';

 	--Outputs
   signal o_TransferOver_p : std_logic;
   signal o_Record_ReadData_p : std_logic;
   signal o_RtdexWr_ReadData_p : std_logic;
   signal ov256_Playback_FifoWrData_p : std_logic_vector(255 downto 0);
   signal o_Playback_FifoWrite_p : std_logic;
   signal o_Playback_ResetFifo_p : std_logic;
   signal ov256_RtdexRd_FifoWrData_p : std_logic_vector(255 downto 0);
   signal o_RtdexRd_FifoWrite_p : std_logic;
   signal o_RtdexRd_ResetFifo_p : std_logic;
   signal o_Record_On_p : std_logic;
   signal ov32_Record_Cnt_p : std_logic_vector(31 downto 0);
   signal ov32_RtdexWr_Cnt_p : std_logic_vector(31 downto 0);
   signal ov32_Playback_Cnt_p : std_logic_vector(31 downto 0);
   signal ov32_RtdexRd_Cnt_p : std_logic_vector(31 downto 0);
   signal ov_Record_TrigAddr_p : std_logic_vector(29 downto 0);
   signal ov32_Record_TrigAddrIndex_p : std_logic_vector(31 downto 0);
   signal o_Record_ParityAddrReg_p : std_logic;
   signal o_AppWdfWren_p : std_logic;
   signal ov256_AppWdfData_p : std_logic_vector(255 downto 0);
   signal o_AppWdfEnd_p : std_logic;
   signal ov3_AppCmd_p : std_logic_vector(2 downto 0);
   signal o_AppEn_p : std_logic;
   signal ov_TgAddr_p : std_logic_vector(29 downto 0);
   -- No clocks detected in port list. Replace <clock> below with 
   -- appropriate port name 
 
   constant clock_period : time := 5 ns;
   
   signal pulse_done_s  : std_logic;
   
   signal app_en_cnt        : unsigned(15 downto 0);
   signal AppWdfWren_cnt    : unsigned(15 downto 0);
   signal PlaybackFifo_cnt  : unsigned(15 downto 0);
   signal RtdexFifo_cnt     : unsigned(15 downto 0);
 
BEGIN

  iv_StartAddress_p <= std_logic_vector(to_unsigned(0,iv_StartAddress_p'length));
  iv_TransferSize_p <= std_logic_vector(to_unsigned(1024,iv_TransferSize_p'length));
  
	-- Instantiate the Unit Under Test (UUT)
   uut: mem_controller PORT MAP (
          i_Reset_p => i_Reset_p,
          i_MemClk_p => i_MemClk_p,
          iv_StartAddress_p => iv_StartAddress_p,
          iv_TransferSize_p => iv_TransferSize_p,
          o_TransferOver_p => o_TransferOver_p,
          iv32_Record_TriggerIn_p => iv32_Record_TriggerIn_p,
          iv256_Record_Data_p => iv256_Record_Data_p,
          i_Record_DValid_p => i_Record_DValid_p,
          o_Record_ReadData_p => o_Record_ReadData_p,
          i_Record_FifoEmpty_p => i_Record_FifoEmpty_p,
          iv256_RtdexWr_Data_p => iv256_RtdexWr_Data_p,
          i_RtdexWr_DValid_p => i_RtdexWr_DValid_p,
          o_RtdexWr_ReadData_p => o_RtdexWr_ReadData_p,
          i_RtdexWr_FifoEmpty_p => i_RtdexWr_FifoEmpty_p,
          ov256_Playback_FifoWrData_p => ov256_Playback_FifoWrData_p,
          o_Playback_FifoWrite_p => o_Playback_FifoWrite_p,
          o_Playback_ResetFifo_p => o_Playback_ResetFifo_p,
          i_Playback_FifoProgFull_p => i_Playback_FifoProgFull_p,
          ov256_RtdexRd_FifoWrData_p => ov256_RtdexRd_FifoWrData_p,
          o_RtdexRd_FifoWrite_p => o_RtdexRd_FifoWrite_p,
          o_RtdexRd_ResetFifo_p => o_RtdexRd_ResetFifo_p,
          i_RtdexRd_FifoProgFull_p => i_RtdexRd_FifoProgFull_p,
          o_Record_On_p => o_Record_On_p,
          i_Record_En_p => i_Record_En_p,
          i_RtdexWr_En_p => i_RtdexWr_En_p,
          i_Playback_En_p => i_Playback_En_p,
          i_RtdexRd_En_p => i_RtdexRd_En_p,
          i_ModeIsContinuous_p => i_ModeIsContinuous_p,
          ov32_Record_Cnt_p => ov32_Record_Cnt_p,
          ov32_RtdexWr_Cnt_p => ov32_RtdexWr_Cnt_p,
          ov32_Playback_Cnt_p => ov32_Playback_Cnt_p,
          ov32_RtdexRd_Cnt_p => ov32_RtdexRd_Cnt_p,
          iv_Record_TrigDly_p => iv_Record_TrigDly_p,
          ov_Record_TrigAddr_p => ov_Record_TrigAddr_p,
          ov32_Record_TrigAddrIndex_p => ov32_Record_TrigAddrIndex_p,
          o_Record_ParityAddrReg_p => o_Record_ParityAddrReg_p,
          i_PlaybackTriggerIn_p => i_PlaybackTriggerIn_p,
          o_AppWdfWren_p => o_AppWdfWren_p,
          ov256_AppWdfData_p => ov256_AppWdfData_p,
          o_AppWdfEnd_p => o_AppWdfEnd_p,
          ov3_AppCmd_p => ov3_AppCmd_p,
          o_AppEn_p => o_AppEn_p,
          ov_TgAddr_p => ov_TgAddr_p,
          iv256_AppRdData_p => iv256_AppRdData_p,
          i_AppRdDataValid_p => i_AppRdDataValid_p,
          i_AppRdy_p => i_AppRdy_p,
          i_AppWdfRdy_p => i_AppWdfRdy_p,
          i_PhyInitDone_p => i_PhyInitDone_p
        );
        
   i_PhyInitDone_p <= '1';

   -- Clock process definitions
   clock_process :process
   begin
		i_MemClk_p <= '0';
		wait for clock_period/2;
		i_MemClk_p <= '1';
		wait for clock_period/2;
   end process;
   
   i_Reset_p <= '1', '0' after 100 ns;
 

   -- Stimulus process
   process(i_Reset_p, i_MemClk_p)
    variable counter : unsigned(19 downto 0) := (others => '0');
   begin
    if i_Reset_p = '1' then
      i_Record_FifoEmpty_p <= '0';
      i_Record_DValid_p <= '0';
      iv256_Record_Data_p <= (others => '0');
      
      pulse_done_s <= '0';
      
      i_Record_En_p <= '0';
      i_RtdexWr_En_p <= '0';
      i_Playback_En_p <= '0';
      i_RtdexRd_En_p <= '0';
      
      iv_Record_TrigDly_p <= (others => '0');
      
      app_en_cnt <= (others => '0');
      AppWdfWren_cnt <= (others => '0');
      
      i_RtdexRd_FifoProgFull_p <= '1';
      
      counter := (others => '0');
      
    elsif rising_edge(i_MemClk_p) then
    
--      if counter(2 downto 0) = "000" then
        i_Record_FifoEmpty_p <= '0';
--      else
--        i_Record_FifoEmpty_p <= '1';
--      end if;
      i_Record_DValid_p <= '0';
      i_Record_En_p <= '0';
      
--      if counter = "000" then
        i_RtdexWr_FifoEmpty_p <= '0';
--      else
--        i_RtdexWr_FifoEmpty_p <= '1';
--      end if;
      i_RtdexWr_DValid_p <= '0';
      i_RtdexWr_En_p <= '0';
      
      i_ModeIsContinuous_p <= '0';
      
      i_Playback_FifoProgFull_p <= '0';
      i_Playback_En_p <= '0';
      i_RtdexRd_FifoProgFull_p <= '0';
      i_RtdexRd_En_p <= '0';
      
      if o_Record_ReadData_p = '1' then
        iv256_Record_Data_p <= std_logic_vector(unsigned(iv256_Record_Data_p) + 1);
        i_Record_DValid_p <= '1';
      end if;

      if o_RtdexWr_ReadData_p = '1' then
        iv256_RtdexWr_Data_p <= std_logic_vector(unsigned(iv256_RtdexWr_Data_p) + 1);
        i_RtdexWr_DValid_p <= '1';
      end if;
      
      if (unsigned(iv256_Record_Data_p) + 1) = to_unsigned(3,iv256_Record_Data_p'length) then
        iv32_Record_TriggerIn_p <= (0 => '1', others => '0');
      else
        iv32_Record_TriggerIn_p <= (others => '0');
      end if;
 
      
      if pulse_done_s = '0' then
        pulse_done_s <= '1';
        i_Record_En_p <= '1';
--        i_RtdexWr_En_p <= '1';
--        i_RtdexRd_En_p <= '1';

--        i_Playback_En_p <= '1';
        i_ModeIsContinuous_p <= '1';
        
        iv_Record_TrigDly_p <= (others => '0');
      else
        i_PlaybackTriggerIn_p <= '1';
      end if;
      
      -- Count memory transaction
      if(o_AppEn_p = '1') then
        app_en_cnt <= app_en_cnt + 1;
      end if;
      if(o_AppWdfWren_p = '1') then
        AppWdfWren_cnt <= AppWdfWren_cnt + 1; 
      end if;
      if(o_Playback_ResetFifo_p = '1') then
        PlaybackFifo_cnt <= (others => '0');
      elsif(o_Playback_FifoWrite_p = '1') then
        PlaybackFifo_cnt <= PlaybackFifo_cnt + 1; 
      end if;
      if(o_RtdexRd_ResetFifo_p = '1') then
        RtdexFifo_cnt <= (others => '0');
      elsif(o_RtdexRd_FifoWrite_p = '1') then
        RtdexFifo_cnt <= RtdexFifo_cnt + 1; 
      end if;
      
       counter := counter + 1;
      
      i_AppRdy_p    <= '1';
      i_AppWdfRdy_p <= '1';
      if counter(2 downto 0) = "100" then
        i_AppRdy_p    <= '0';
        i_AppWdfRdy_p <= '0';
        counter := (others => '0');
      end if;
      
--      if counter = 80 then
--        i_RtdexWr_En_p <= '1';
--      end if;
       
       
    end if;
   end process;
   
   -- Send 2 words when a read is requested
   process(i_Reset_p, i_MemClk_p)
    variable secondWord_v : std_logic := '0';
   begin
    if i_Reset_p = '1' then
      secondWord_v := '0';
      iv256_AppRdData_p <= (others => '0');
      i_AppRdDataValid_p <= '0';
    elsif rising_edge(i_MemClk_p) then
      if o_AppEn_p = '1' and ov3_AppCmd_p = "001" then
          iv256_AppRdData_p <= std_logic_vector(unsigned(iv256_AppRdData_p)+1);
          i_AppRdDataValid_p <= '1';
          secondWord_v := '1';
      elsif secondWord_v = '1' then
        iv256_AppRdData_p <= std_logic_vector(unsigned(iv256_AppRdData_p)+1);
        i_AppRdDataValid_p <= '1';
        secondWord_v := '0';
      else
        i_AppRdDataValid_p <= '0';
        secondWord_v := '0';
      end if;
    end if;
  end process;

END;
