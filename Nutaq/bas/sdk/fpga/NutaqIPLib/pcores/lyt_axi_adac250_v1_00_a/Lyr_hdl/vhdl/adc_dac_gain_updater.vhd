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
-- File        : $Id: adc_dac_gain_updater.vhd,v 1.1 2015/04/10 19:47:29 sqa Exp $
--------------------------------------------------------------------------------
-- Description : ADC and DAC GainUpdater module from custom logic to ADAC250
--
--
--------------------------------------------------------------------------------
-- Notes / Assumptions :
--
--------------------------------------------------------------------------------
-- Copyright (c) 2009 Lyrtech RD inc.
-- TODO = legal notice
--------------------------------------------------------------------------------
-- Abdelkarim Ouadid - Initial revision 2009/10/08
-- $Log: adc_dac_gain_updater.vhd,v $
-- Revision 1.1  2015/04/10 19:47:29  sqa
-- bas first commit (CBOU)
--
-- Revision 1.1  2013/01/09 15:32:20  julien.roy
-- Add ADAC250 axi pcore
--
-- Revision 1.1  2010/06/17 15:42:02  francois.blackburn
-- first commit
--
-- Revision 1.1  2010/01/14 22:48:20  karim.ouadid
-- first commit
--
-- Revision     1.0     2009/10/08 15:35:58  karim.ouadid
--------------------------------------------------------------------------------

library ieee;
    use ieee.std_logic_1164.all;
    use ieee.numeric_std.all;
    use work.misc_p.all;

-- pragma translate_off
library unisim;
    use unisim.vcomponents.all;
-- pragma translate_on

entity ADCDACGainUpdater is
generic(
        ADCChAOFFSetCorrectionTime  : std_logic_vector (3 downto 0):= X"0";
        ADCChBOFFSetCorrectionTime  : std_logic_vector (3 downto 0):= X"0"
        );
port (
    --Global signals
    i_clk_p                         : in   std_logic;                         -- design clock
    i_rst_p                         : in   std_logic;                         -- global reset
    i_WriteCmd_p                    : in   std_logic;
    --User ports
    iv6_ADCChAFineGain_p            : in   std_logic_vector(5 downto 0);      -- Update 0
    iv6_ADCChBFineGain_p            : in   std_logic_vector(5 downto 0);      -- Update
    iv4_ADCChACoarseGain_p          : in   std_logic_vector(3 downto 0);
    iv4_ADCChBCoarseGain_p          : in   std_logic_vector(3 downto 0);
    iv4_DACChAGain_p                : in   std_logic_vector(3 downto 0);      -- DAC ChA gain and DAC ChB gain
    iv4_DACChBGain_p                : in   std_logic_vector(3 downto 0);      -- updated at the same time
    i_UpdateADCChAFineGain_p        : in   std_logic;
    i_UpdateADCChBFineGain_p        : in   std_logic;
    i_UpdateADCChACoarseGain_p      : in   std_logic;
    i_UpdateADCChBCoarseGain_p      : in   std_logic;
    i_UpdateDACChAGain_p            : in   std_logic;
    i_UpdateDACChBGain_p            : in   std_logic;
    o_AckUpdateADCChAFineGain_p     : out  std_logic;
    o_AckUpdateADCChBFineGain_p     : out  std_logic;
    o_AckUpdateADCChACoarseGain_p   : out  std_logic;
    o_AckUpdateADCChBCoarseGain_p   : out  std_logic;
    o_AckUpdateDACChAGain_p         : out  std_logic;
    o_AckUpdateDACChBGain_p         : out  std_logic;
    o_UpdaterBusy_p                 : out  std_logic;
    --To SPI module ports
    i_SPIBusy_p                     : in   std_logic;
    ov9_writeaddr_p                 : out  std_logic_vector (8 downto 0);
    o_req_p                         : out  std_logic;
    --o_we_p                          : out  std_logic;
    i_gnt_p                         : in   std_logic;
    ov32_dout_p                     : out  std_logic_vector (31 downto 0);
    i_ack_p                         : in   std_logic
 );
end entity ADCDACGainUpdater;

architecture arch of  ADCDACGainUpdater is

-- Data types
type cmd_state is (idle_state,
                  process_cmd_state,
                  checkUpdateP1_state,
                  checkUpdateP2_state,
                  checkUpdateP3_state,
                  checkUpdateP4_state,
                  checkUpdateP5_state,
                  sendP1_state, sendP2_state,
                  sendP3_state, sendP4_state,
                  sendP5_state, Unused1,
                  Unused2, Unused3, Unused4);

type spi_write_state is (Write_idle_state,
                         Write_Wait_gnt,
                         Write_Second_Byte,
                         Write_CS,
                         Write_Ctrl,
                         Wait_Ack,
                         Unused1,
                         Unused2);

-- Used signals
signal v9_writeaddr_s                        : std_logic_vector(8 downto 0);
signal req_s                            : std_logic;
signal we_s                             : std_logic;
signal v32_dout_s                       : std_logic_vector(31 downto 0);
signal UpdaterBusy_s                    : std_logic;
signal AckUpdateADCChAFineGain_s        : std_logic;
signal AckUpdateADCChBFineGain_s        : std_logic;


signal AckUpdateADCChACoarseGain_s      : std_logic;
signal AckUpdateADCChBCoarseGain_s      : std_logic;
signal AckUpdateDACGain_s               : std_logic;
signal v32_datatospi_s                  : std_logic_vector(31 downto 0);
signal v32_SPICtrl_s                    : std_logic_vector(31 downto 0);
signal v32_SPICs_s                      : std_logic_vector(31 downto 0);
signal SPIWrite_s                       : std_logic;



--signal AckUpdateDACGain_s               : std_logic;
signal UpdateADCChAFineGain_s           : std_logic;
signal UpdateADCChBFineGain_s           : std_logic;
signal UpdateADCChACoarseGain_s         : std_logic;
signal UpdateADCChBCoarseGain_s         : std_logic;
signal UpdateDACChAGain_s               : std_logic;
signal UpdateDACChBGain_s               : std_logic;
signal WriteCommandLatchSet_s           : std_logic;
signal WriteCommandLatchClear_s         : std_logic;
signal WriteCmd_s                       : std_logic;
signal AtLeastOneFctUpdate_s            : std_logic;
signal UpdateCmd_s                      : std_logic;
signal nextState                        : cmd_state;
signal nextspiwriteState                : spi_write_state;

attribute fsm_encoding: string;
attribute fsm_encoding of nextState         : signal is "sequential";
attribute fsm_encoding of nextspiwriteState : signal is "sequential";

begin

u_detect_w_cmd : entity work.edge_detector_1t
port map
(
    d       => i_WriteCmd_p,
    rising  => '1',
    clk     => i_clk_p,
    q       => WriteCommandLatchSet_s
);

u_write_cmd_l : entity work.ALatchSClear
port map(
  i_Clk_p                   => i_clk_p,
  i_Status_p                => WriteCommandLatchSet_s,
  i_StatusClear_p           => WriteCommandLatchClear_s,
  o_StatusLatched_p         => WriteCmd_s
);

process_reg_l : process( i_rst_p, i_clk_p )
      begin
        if( i_rst_p = '1' ) then
             UpdateADCChAFineGain_s     <= '0';
             UpdateADCChBFineGain_s     <= '0';
             UpdateADCChACoarseGain_s   <= '0';
             UpdateADCChBCoarseGain_s   <= '0';
             UpdateDACChAGain_s         <= '0';
             UpdateDACChBGain_s         <= '0';
        elsif ( rising_edge( i_clk_p ) ) then
            if( WriteCommandLatchSet_s = '1' ) then
                UpdateADCChAFineGain_s      <= i_UpdateADCChAFineGain_p;
                UpdateADCChBFineGain_s      <= i_UpdateADCChBFineGain_p;
                UpdateADCChACoarseGain_s    <= i_UpdateADCChACoarseGain_p;
                UpdateADCChBCoarseGain_s    <= i_UpdateADCChBCoarseGain_p;
                UpdateDACChAGain_s          <= i_UpdateDACChAGain_p;
                UpdateDACChBGain_s          <= i_UpdateDACChBGain_p;
            end if;
        end if;
end process process_reg_l;

AtLeastOneFctUpdate_s <= i_UpdateADCChAFineGain_p or i_UpdateADCChBFineGain_p or i_UpdateADCChACoarseGain_p  or i_UpdateADCChBCoarseGain_p or  i_UpdateDACChAGain_p or  i_UpdateDACChBGain_p ;

--LacthUpdate_process_l : process (i_clk_p)   --- WHY I'M USING IT
--begin
--    if rising_edge(i_clk_p) then
--        if (i_rst_p = '1') then
--           UpdateCmd_s <= '0';
--        elsif (LatchUpdateCmd_s = '1') then
--           UpdateCmd_s <= '1';
--        end if;
--     end if;
--end process LacthUpdate_process_l;

process_next_state_l : process( i_rst_p, i_clk_p )
      begin
       if (i_rst_p ='1') then
        nextState          <= idle_state;
       elsif ( rising_edge( i_clk_p ) ) then
        WriteCommandLatchClear_s <= '0';
        UpdaterBusy_s            <= '1';

        case nextState is
           when idle_state =>
             UpdaterBusy_s               <= '0';
             if( WriteCmd_s = '1' ) then
               nextState <= process_cmd_state;
             end if;

          when process_cmd_state =>
             WriteCommandLatchClear_s <= '1';
             if( AtLeastOneFctUpdate_s = '1' ) then
               nextState <= checkUpdateP1_state;
             else
               nextState <= idle_state;
             end if;

          when checkUpdateP1_state =>
             if( UpdateADCChAFineGain_s = '1' ) then
                nextState <= sendP1_state;
             else
                nextState <= checkUpdateP2_state;
             end if;

          when sendP1_state =>
             if( AckUpdateADCChAFineGain_s = '1' ) then
                nextState <= checkUpdateP2_state;
             end if;

          when checkUpdateP2_state =>
             if( UpdateADCChBFineGain_s = '1' ) then
                nextState <= sendP2_state;
             else
                nextState <= checkUpdateP3_state;
             end if;

          when sendP2_state =>
             if( AckUpdateADCChBFineGain_s = '1' ) then
                nextState <= checkUpdateP3_state;
             end if;

          when checkUpdateP3_state =>
             if( UpdateADCChACoarseGain_s = '1' ) then
                nextState <= sendP3_state;
             else
                nextState <= checkUpdateP4_state;
             end if;

          when sendP3_state =>
             if( AckUpdateADCChACoarseGain_s = '1' ) then
                nextState <= checkUpdateP4_state;
             end if;

          when checkUpdateP4_state =>
             if( UpdateADCChBCoarseGain_s = '1' ) then
                nextState <= sendP4_state;
             else
                nextState <= checkUpdateP5_state;
             end if;

         when sendP4_state =>
             if( AckUpdateADCChBCoarseGain_s = '1' ) then
                nextState <= checkUpdateP5_state;
             end if;

          when checkUpdateP5_state =>
             if( UpdateDACChAGain_s  = '1' or UpdateDACChBGain_s = '1') then
                nextState <= sendP5_state;
             else
                nextState <= idle_state;
             end if;

         when sendP5_state =>
             if( AckUpdateDACGain_s = '1' ) then
                nextState <= idle_state;
             end if;
         when others =>
            null;

       end case;
     end if;
end process process_next_state_l;

process_apply_cmd_l : process( i_rst_p, i_clk_p )
      begin
        if( i_rst_p = '1' ) then
           v32_datatospi_s   <= (others =>'0');
           v32_SPICtrl_s     <= (others =>'0');
           v32_SPICs_s       <= (others =>'1');
           SPIWrite_s        <= '0';
           AckUpdateADCChAFineGain_s      <= '0';
           AckUpdateADCChBFineGain_s      <= '0';
           AckUpdateADCChACoarseGain_s    <= '0';
           AckUpdateADCChBCoarseGain_s    <= '0';
           AckUpdateDACGain_s             <= '0';


        elsif (rising_edge(i_clk_p)) then
           SPIWrite_s                     <= '0';
           AckUpdateADCChAFineGain_s      <= '0';
           AckUpdateADCChBFineGain_s      <= '0';
           AckUpdateADCChACoarseGain_s    <= '0';
           AckUpdateADCChBCoarseGain_s    <= '0';
           AckUpdateDACGain_s             <= '0';

           case nextState is

                when  sendP1_state =>
                       v32_datatospi_s                 <= x"0000" & "00" & iv6_ADCChAFineGain_p & x"57";
                       v32_SPICtrl_s                   <= x"0000_01A1";
                       v32_SPICs_s                     <= x"FFFF_FFFE";
                       SPIWrite_s                      <= '1';
                       AckUpdateADCChAFineGain_s       <= i_ack_p;


                when  sendP2_state =>
                       v32_datatospi_s                 <= x"0000" & "00" & iv6_ADCChBFineGain_p & x"6A";
                       v32_SPICtrl_s                   <= x"0000_01A1";
                       v32_SPICs_s                     <= x"FFFF_FFFD";
                       SPIWrite_s                      <= '1';
                       AckUpdateADCChBFineGain_s       <= i_ack_p;


                when  sendP3_state =>
                       v32_datatospi_s                <= x"0000" & iv4_ADCChACoarseGain_p &ADCChAOFFSetCorrectionTime & x"55";
                       v32_SPICtrl_s                  <= x"0000_01A1";
                       v32_SPICs_s                    <= x"FFFF_FFFB";
                       SPIWrite_s                     <= '1';
                       AckUpdateADCChACoarseGain_s    <= i_ack_p;


                when  sendP4_state =>
                       v32_datatospi_s                <= x"0000" & iv4_ADCChBCoarseGain_p &ADCChBOFFSetCorrectionTime & x"68";
                       v32_SPICtrl_s                  <= x"0000_01A1";
                       v32_SPICs_s                    <= x"FFFF_FFF7";
                       SPIWrite_s                     <= '1';
                       AckUpdateADCChBCoarseGain_s    <= i_ack_p;


                when  sendP5_state =>
                       v32_datatospi_s                <= x"0000" & iv4_DACChAGain_p &iv4_DACChBGain_p & x"07";
                       v32_SPICtrl_s                  <= x"0000_01A1";
                       v32_SPICs_s                    <= x"FFFF_FFDF";
                       SPIWrite_s                     <= '1';
                       AckUpdateDACGain_s             <= i_ack_p;

                 when others =>
                       v32_datatospi_s                <= (others =>'0');
                       v32_SPICtrl_s                  <= x"0000_01A1";
                       v32_SPICs_s                    <= (others =>'1');

                 end case;
       end if;
end process process_apply_cmd_l;


process_set_SPI_l : process( i_rst_p, i_clk_p )
      begin
      if (i_rst_p ='1') then
        nextspiwriteState          <= Write_idle_state;
        req_s <= '0';
       elsif ( rising_edge( i_clk_p ) ) then
        --we_s <= '0';
        v9_writeaddr_s <= (others=>'0');
        req_s <= '0';
        case nextspiwriteState is

           when Write_idle_state =>
             if( SPIWrite_s = '1' ) then
                req_s <= '1';
                nextspiwriteState <= Write_Wait_gnt;
             end if;

          when Write_Wait_gnt =>
              if (i_gnt_p ='1') then
                  v9_writeaddr_s <= "000000010";
                  v32_dout_s <= v32_datatospi_s;
                  --we_s <='1';
                  req_s <= '1';
                  nextspiwriteState <= Write_Second_Byte;
              end if;

          when Write_Second_Byte =>
              if (i_gnt_p ='1') then
                  v9_writeaddr_s <= "000000100";
                  v32_dout_s <= v32_datatospi_s;
                  --we_s <='1';
                  req_s <= '1';
                  nextspiwriteState <= Write_CS;
              end if;

          when Write_CS =>
              if (i_gnt_p ='1') then
                  v9_writeaddr_s <= "010000000";
                  v32_dout_s <= v32_SPICs_s;
                  --we_s <='1';
                  req_s <= '1';
                  nextspiwriteState <= Write_Ctrl;
              end if;

           when Write_Ctrl =>
              if (i_gnt_p ='1') then
                  v9_writeaddr_s <= "000000001";
                  v32_dout_s <= v32_SPICtrl_s;
                  --we_s <='1';
                  req_s <= '1';
                  nextspiwriteState <= Wait_Ack;
              end if;

           when Wait_Ack =>
              if (i_ack_p ='1') then
                  req_s <= '0';
                  nextspiwriteState <= Write_idle_state;
              end if;

           when others =>
                  req_s <= '0';
       end case;
    end if;
end process process_set_SPI_l;


--o_we_p                         <= we_s;
o_req_p                        <= req_s;
ov32_dout_p                    <= v32_dout_s;
ov9_writeaddr_p                <= v9_writeaddr_s;
o_UpdaterBusy_p                <= UpdaterBusy_s;

o_AckUpdateADCChAFineGain_p    <=  AckUpdateADCChAFineGain_s;
o_AckUpdateADCChBFineGain_p    <=  AckUpdateADCChBFineGain_s;
o_AckUpdateADCChACoarseGain_p  <=  AckUpdateADCChACoarseGain_s;
o_AckUpdateADCChBCoarseGain_p  <=  AckUpdateADCChBCoarseGain_s;
o_AckUpdateDACChAGain_p        <=  AckUpdateDACGain_s;
o_AckUpdateDACChBGain_p        <=  AckUpdateDACGain_s;

end architecture arch;