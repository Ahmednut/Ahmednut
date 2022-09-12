/****************************************************************************
 *
 *    ****                              I
 *   ******                            ***
 *   *******                           ****
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    ****I  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       cce_mainloop.c
 * @brief      Main loop of the CCE where command are decoded
 *
 * $Date: 2016/07/19 15:03:01 $
 * $Revision: 1.25 $
 *
 * (c) 2013, Nutaq Inc. All rights reserved.
 *
 * @ingroup CCE
 *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <pthread.h>

#include "cce.h"
#include "bas_error.h"
#include "bas_debug.h"

#include "carrier_service.h"
#include "carrier_service_def.h"

#include "eapi_serialize.h"
#include "cce_io.h"
#include "cce_fmcvita.h"
#include "common_utils.h"
#include "utils.h"

#ifdef PERSEUS
#include "bitstream.h"
#include "cce_adac250.h"
#include "adac250_user.h"
#include "cce_radio420.h"
#include "fmc_radio.h"
#include "cce_mi125.h"
#include "cce_mo1000.h"
#include "cce_mi250.h"
#include "cce_adc5000.h"
#include "cce_aurora.h"
#include "cce_lvdsxinxout.h"
#include "cce_lvds.h"
#include "cce_osconfig.h"
#include "cce_rtdexsync.h"
#include "cce_sysmon.h"
#include "cce_ppssync.h"
#include "ppssync.h"
#include "cce_timestamp.h"
#endif //PERSEUS

#include "cce_radio640.h"
#include "cce_carrier.h"

#ifdef FMCOMMS3_IN_USE
#include "cce_fmcomms3.h"
#endif // FMCOMMS3_IN_USE

extern Carrier_handle_t hCarrier;

extern const char cce_version[];
extern char cce_mac[20];

//Multi-thread
pthread_mutex_t bcl = PTHREAD_MUTEX_INITIALIZER; //big command lock

void * cce_mainloop(void *state)
{
    int i;
    connection_state * tcp_con = (connection_state*) state;
    unsigned cmd, cmdbuf;

    while(!tcp_con->has_ended)
    {
		Result ret=ERR_GENERIC;
		connection_flush(state);
		/* Receive command (4 bytes) and send back Result (4 bytes)*/
		//read command
		i = recv(tcp_con->stream, &cmdbuf, 4, 0);
		cmd = ntohl(cmdbuf);

		debug("recv %i\tcmd %08x\n", i, cmd);
		if(i<4)//error occured (disconnect)
		{
			debug("receive command error occured %i\n", i);
			tcp_con->has_ended=1;
		}
		else
		{
			//acquire lock
			//debug("lock %08x\n", cmd);
			pthread_mutex_lock(&bcl);
			//debug("post lock %08x\n", cmd);
		 	//debug("tcp_con %x\n", tcp_con);
			switch(cmd)
			{
			case ECHO_ECHO:
				ret = OK;
				break;

			case CCECTRL_EXIT:
				tcp_con->has_ended = 1;
				ret = OK;
				break;

			case MEMORY_INIT:
				ret = OK;
				break;
			case MEMORY_READ:
    			debug("command: memory read\r\n", 0);
				ret = memory_read_recv(tcp_con);
				break;
			case MEMORY_WRITE:
				ret = memory_write_recv(tcp_con);
				break;
			case MEMORY_READ_WITH_ID:
   				debug("command: memory read with id\r\n", 0);
				ret = memory_read_recv_with_id(tcp_con);
				break;
			case MEMORY_WRITE_WITH_MASK:
   				debug("command: memory read with mask\r\n", 0);
				ret = memory_write_with_mask_recv(tcp_con);
				break;
			case MEMORY_WRITE_WITH_ID:
				ret = memory_write_recv_with_id(tcp_con);
				break;
			case MEMORY_READ_CUSTOM_REG_ID:
    			debug("command: memory read customreg id\r\n", 0);
				ret = custom_register_read_recv(tcp_con);
				break;
			case MEMORY_WRITE_CUSTOM_REG_ID:
				ret = custom_register_write_recv(tcp_con);
				break;
			case MEMORY_SCAN_I2C_DEVICES:
				ret = i2c_scan_devices_recv(tcp_con);
				break;
#ifdef PERSEUS
			case FPGAFLASH_INIT:
				ret = OK;
				break;
			case FPGAFLASH_RECV:
				ret = fpgaflash_recv(tcp_con);
                break;
			case FPGAFLASH_GET_INDEX:
				ret = fpgaflash_get_index_recv(tcp_con);
				break;
            case FPGAFLASH_SET_INDEX:
                ret = fpgaflash_set_index_recv(tcp_con);
                break;
            case FPGAFLASH_GET_INFO:
                ret = fpgaflash_get_info_recv(tcp_con);
                break;
			case FPGAFLASH_UPDATE_RECV:
				ret = updateflash_recv(tcp_con);
				break;
			case FPGAFLASH_DTB_RECV:
				ret = dtbflash_recv(tcp_con);
				break;

			case ADAC250_PRESENCE:
                            debug("ADAC Presence\r\n", 0);
				ret = ADAC250_Presence_recv(tcp_con);
				break;
			case ADAC250_POWERUP:
                            debug("ADAC PowerUp\r\n", 0);
				ret = ADAC250_PowerUp_recv(tcp_con);
				break;
			case ADAC250_READSPI:
                            debug("ADAC ReadSPIRegisters\r\n", 0);
				ret = ADAC250_ReadSPIRegisters_recv(tcp_con);
				break;
			case ADAC250_WRITESPI:
				ret = ADAC250_WriteSPIRegisters_recv(tcp_con);
				break;
			case ADAC250_INITPLL:
				ret = ADAC250_PLL_Init_recv(tcp_con);
				break;
			case ADAC250_CALCULATEPLLPARAMS:
				ret = ADAC250_PLL_CalculateParams_recv(tcp_con);
				break;
			case ADAC250_SETPLLCONFIG:
				ret = ADAC250_PLL_SetConfig_recv(tcp_con);
				break;
			case ADAC250_TRIGPLLREGISTERTRANSFERT:
				ret = ADAC250_PLL_trigRegisterTransfert_recv(tcp_con);
				break;
			case ADAC250_PLLSYNCOUTPUT:
				ret =  ADAC250_PLL_SyncOutput_recv(tcp_con);
				break;
			case ADAC250_GETPLLSTATUS:
				ret =  ADAC250_PLL_getStatus_recv(tcp_con);
				break;
			case ADAC250_SETPLLSLEEP:
				ret =  ADAC250_PLL_SetSleep_recv(tcp_con);
				break;
			case ADAC250_SETREFCLKTUNING:
				ret =  ADAC250_PLL_setRefClkTuning_recv(tcp_con);
				break;
			case ADAC250_DACINIT:
				ret =  ADAC250_DAC_Init_recv(tcp_con);
				break;
			case ADAC250_DACSETGAIN:
				ret =  ADAC250_DAC_SetGain_recv(tcp_con);
				break;
			case ADAC250_DACSETOFFSET:
				ret =  ADAC250_DAC_SetOffset_recv(tcp_con);
				break;
			case ADAC250_DACSYNC:
				ret = ADAC250_DAC_Sync_recv(tcp_con);
				break;
			case ADAC250_DACCALIBRATE:
				ret = ADAC250_DAC_Calibrate_recv(tcp_con);
				break;
			case ADAC250_DACSYNCSOURCE:
				ret = ADAC250_DAC_SyncSource_recv(tcp_con);
				break;
			case ADAC250_INITADC:
				ret =  ADAC250_ADC_init_recv(tcp_con);
				break;
			case ADAC250_SETADCGAIN:
				ret =  ADAC250_ADC_setGain_recv(tcp_con);
				break;
			case ADAC250_SETADCFINEGAIN:
				ret =  ADAC250_ADC_setFineGain_recv(tcp_con);
				break;
			case ADAC250_SETADCPEDESTAL:
				ret =  ADAC250_ADC_setPedestal_recv(tcp_con);
				break;
			case ADAC250_GETADCSTATUS:
				ret =  ADAC250_ADC_getStatus_recv(tcp_con);
				break;
			case ADAC250_ADCCALIBRATE:
				ret =  ADAC250_ADC_Calibrate_recv(tcp_con);
				break;
			case ADAC250_SETADCDATAFORMAT :
				ret = ADAC250_ADC_SetDataFormat_recv(tcp_con);
				break;
			case ADAC250_SETADCSLEEPMODE :
				ret = ADAC250_ADC_SetSleepMode_recv(tcp_con);
				break;
			case ADAC250_SETADCOFFSETCORRECTIONTIME :
				ret = ADAC250_ADC_SetOCT_recv(tcp_con);
				break;
			case ADAC250_SETADCTRIGGERDELAY :
				ret = ADAC250_ADC_SetTriggerDelay_recv(tcp_con);
				break;
			case ADAC250_SETMUX:
				ret = ADAC250_MUX_ConfigClockOutput_recv(tcp_con);
				break;
#endif //PERSEUS
			// New fmc eeprom, vita and carrier module fcts
            case FMCEEPROM_EEPROM_READ:
				ret = FMCEEprom_eeprom_read_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_WRITE:
				ret = FMCEEprom_eeprom_write_recv(tcp_con);
				break;
      case FMCEEPROM_EEPROM_WIPE:
				ret = FMCEEprom_eeprom_wipe_recv(tcp_con);
				break;  
			case FMCEEPROM_EEPROM_GET_IDSTRING:
				ret = FMCEEprom_eeprom_get_idstring_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_MANUF:
				ret = FMCEEprom_eeprom_get_manuf_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_BOARD_STATE:
				ret = FMCEEprom_eeprom_get_board_state_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_BOARD_TYPE:
				ret = FMCEEprom_eeprom_get_board_type_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_CORE:
				ret = FMCEEprom_eeprom_get_core_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_CARRIERVADJ:
				ret = FMCEEprom_eeprom_get_carriervadj_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_PARTNUM:
				ret = FMCEEprom_eeprom_get_partnum_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_SERIALNUM:
				ret = FMCEEprom_eeprom_get_serialnum_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_HARDREV:
				ret = FMCEEprom_eeprom_get_hardrev_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_FIRMREV:
				ret = FMCEEprom_eeprom_get_firmrev_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_MODVADJ:
				ret = FMCEEprom_eeprom_get_modvadj_recv(tcp_con);
				break;
			case FMCEEPROM_EEPROM_GET_CORENAME:
				ret = FMCEEprom_eeprom_get_corename_recv(tcp_con);
				break;
#ifdef PERSEUS
			case FMCRADIO_INIT:
				ret = fmc_radio_presence(tcp_con);
				break;
			case FMCRADIO_POWERUP:
				ret = fmc_radio_powerup_recv(tcp_con);
				break;
			case FMCRADIO_RESET:
				ret = fmc_radio_sdr_reset_recv(tcp_con);
				break;
			case FMCRADIO_SELECT:
				ret = fmc_radio_sdr_select_recv(tcp_con);
				break;
			case FMCRADIO_SETREVISION:
				ret = fmc_radio_setrevision_recv(tcp_con);
				break;
			case FMCRADIO_SPIWRITE:
				ret = fmc_radio_spi_write_recv(tcp_con);
				break;
			case FMCRADIO_SPIREAD:
				ret = fmc_radio_spi_read_recv(tcp_con);
				break;
			case FMCRADIO_PLLSETPARAM:
				ret = fmc_radio_pll_setparam_recv(tcp_con);
				break;
			case FMCRADIO_PLLLOCKSTATUS:
				ret = fmc_radio_pll_lockstatus_recv(tcp_con);
				break;
			case FMCRADIO_PLLWAITLOCK:
				ret = fmc_radio_pll_waitforlock_recv(tcp_con);
				break;
			case FMCRADIO_CLOCKMUXSET:
				ret = fmc_radio_clockmux_set_recv(tcp_con);
				break;
			case FMCRADIO_LIMESETPLLPARAM:
				ret = fmc_radio_lime_setpllparam_recv(tcp_con);
				break;
			case FMCRADIO_LIMEPLLCALIBRATE:
				ret = fmc_radio_lime_calibratePLL_recv(tcp_con);
				break;
			case FMCRADIO_LIMERESET:
				ret = fmc_radio_lime_reset_recv(tcp_con);
				break;
			case FMCRADIO_SETBAND:
				ret = fmc_radio_sdr_band_recv(tcp_con);
				break;
			case FMCRADIO_SETRXGAIN:
				ret = fmc_radio_sdr_rx_gain_recv(tcp_con);
				break;
			case FMCRADIO_SETTXGAIN:
				ret = fmc_radio_tx_gain_recv(tcp_con);
				break;
			case FMCRADIO_RXVGACALIBRATE:
				ret = fmc_radio_rxvga_calibrate_recv(tcp_con);
				break;
			case FMCRADIO_RXFILTER:
				ret = fmc_radio_lime_setrxfilter_recv(tcp_con);
				break;
			case FMCRADIO_LPFSET:
				ret = fmc_radio_lpf_set_recv(tcp_con);
				break;
			case FMCRADIO_LPFCALIBRATE:
				ret = fmc_radio_lpf_calibrate_recv(tcp_con);
				break;
			case FMCRADIO_ENABLEPATH:
				ret = fmc_radio_path_enable_recv(tcp_con);
				break;
			case FMCRADIO_DISABLEPATH:
				ret = fmc_radio_path_disable_recv(tcp_con);
				break;
			case FMCRADIO_LOOPBACK:
				ret = fmc_radio_loopback_recv(tcp_con);
				break;
			case FMCRADIO_LO_CALIBRATE:
				ret = fmc_radio_lo_leakage_calibration_recv(tcp_con);
				break;
			case FMCRADIO_LO_CALIBRATE_SET:
				ret = fmc_radio_lo_leakage_calibration_set_recv(tcp_con);
				break;
			case FMCRADIO_SSB_CALIBRATE:
				ret = fmc_radio_ssb_calibration_recv(tcp_con);
				break;
			case FMCRADIO_SSB_CALIBRATE_SET:
				ret = fmc_radio_ssb_calibration_set_recv(tcp_con);
				break;
			case FMCRADIO_RXFE_DC_CALIBRATE:
				ret = fmc_radio_rx_dc_offset_calibration_recv(tcp_con);
				break;
			case FMCRADIO_RXFE_DC_CALIBRATE_SET:
				ret = fmc_radio_rx_dc_offset_calibration_set_recv(tcp_con);
				break;
			case FMCRADIO_SPI_CONTROL_SET:
				ret = fmc_radio_spi_control_set_recv(tcp_con);
				break;
            case FMCRADIO_RX_CONNECT_SET:
                ret = fmc_radio_rx_connect_recv(tcp_con);
                break;
            case FMCRADIO_RX_DISCONNECT_SET:
                ret = fmc_radio_rx_disconnect_recv(tcp_con);
                break;

			case ADC5000_PRESENCE:
				ret = ADC5000_Presence_recv(tcp_con);
				break;
			case ADC5000_POWERUP:
				ret = ADC5000_Powerup_recv(tcp_con);
				break;
			case ADC5000_RESET:
				ret = ADC5000_Reset_recv(tcp_con);
				break;
			case ADC5000_SET_MODE:
				ret = ADC5000_SetMode_recv(tcp_con);
				break;
			case ADC5000_SET_PLL_PARAM:
				ret = ADC5000_SetPLLParam_recv(tcp_con);
				break;
			case ADC5000_SET_SYNC:
				ret = ADC5000_SetSync_recv(tcp_con);
				break;
			case ADC5000_PLL_LOCK:
				ret = ADC5000_PLLLock_recv(tcp_con);
				break;
			//case ADC5000_READ_TEMPERATURE:
			//	ret = ADC5000_ReadTemperature_recv(tcp_con);
			//	break;
			case ADC5000_SET_TEST_MODE:
				ret = ADC5000_SetTestMode_recv(tcp_con);
				break;
			case ADC5000_CALIBRATE_IODELAY:
				ret = ADC5000_CalibrateIoDelay_recv(tcp_con);
				break;
			case ADC5000_GET_ADC_STATUS:
				ret = ADC5000_GetAdcStatus_recv(tcp_con);
				break;
			case ADC5000_SET_STANDBY:
				ret = ADC5000_SetStandby_recv(tcp_con);
				break;
			case ADC5000_SET_CODING:
				ret = ADC5000_SetCoding_recv(tcp_con);
				break;
			case ADC5000_SET_BANDWIDTH:
				ret = ADC5000_SetBandwidth_recv(tcp_con);
				break;
			case ADC5000_SET_ADJ_TRIGGER_DELAY:
				ret = ADC5000_SetAdjTriggerDelay_recv(tcp_con);
				break;
			case ADC5000_SET_OFFSET:
				ret = ADC5000_SetOffset_recv(tcp_con);
				break;
			case ADC5000_SET_GAIN:
				ret = ADC5000_SetGain_recv(tcp_con);
				break;
			case ADC5000_SET_PHASE:
				ret = ADC5000_SetPhase_recv(tcp_con);
				break;

			case AURORA_GETVERSION:
				ret = Aurora_GetVersion_recv(tcp_con);
				break;
			case AURORA_RESETCORE:
				ret = Aurora_ResetCore_recv(tcp_con);
				break;
			case AURORA_GETCHANNELSTATUS:
				ret = Aurora_GetChannelStatus_recv(tcp_con);
				break;
			case AURORA_RESETRXFIFO:
				ret = Aurora_ResetRxFifo_recv(tcp_con);
				break;
			case AURORA_RESETTXFIFO:
				ret = Aurora_ResetTxFifo_recv(tcp_con);
				break;
			case AURORA_GETRXDATACOUNT:
				ret = Aurora_GetRxDataCount_recv(tcp_con);
				break;
			case AURORA_GETTXDATACOUNT:
				ret = Aurora_GetTxDataCount_recv(tcp_con);
				break;
			case AURORA_GETRXDATARATE:
				ret = Aurora_GetRxDataRate_recv(tcp_con);
				break;
			case AURORA_GETTXDATARATE:
				ret = Aurora_GetTxDataRate_recv(tcp_con);
				break;
			case AURORA_SETGTXTXPARAM:
				ret = Aurora_SetGtxTxParam_recv(tcp_con);
				break;
			case AURORA_GETGTXTXPARAM:
				ret = Aurora_GetGtxTxParam_recv(tcp_con);
				break;
			case AURORA_SETGTXRXPARAM:
				ret = Aurora_SetGtxRxParam_recv(tcp_con);
				break;
			case AURORA_GETGTXRXPARAM:
				ret = Aurora_GetGtxRxParam_recv(tcp_con);
				break;
			case AURORA_GETDFEEYEDACMON:
				ret = Aurora_GetDfeEyeDacMon_recv(tcp_con);
				break;
			case AURORA_SETCHANNELENABLESTATE:
				ret = Aurora_SetChannelEnableState_recv(tcp_con);
				break;
			case AURORA_GETCHANNELENABLESTATE:
				ret = Aurora_GetChannelEnableState_recv(tcp_con);
				break;
        
			case MI125_INIT:
				ret = MI125_Presence_recv(tcp_con);
				break;
			case MI125_POWERUP:
				ret = MI125_powerup_recv(tcp_con);
				break;
			case MI125_MI125_RESET:
				ret = MI125_mi125_reset_recv(tcp_con);
				break;
			case MI125_MI125_DIGITAL_ADCCALIBRATION:
				ret = MI125_mi125_digital_adccalibration_recv(tcp_con);
				break;
			case MI125_MI125_SET_CONFIG:
				ret = MI125_mi125_set_config_recv(tcp_con);
				break;
			case MI125_MI125_SET_CLKSRC:
				ret = MI125_mi125_set_clksrc_recv(tcp_con);
				break;
			case MI125_MI125_CHECKCORE:
				ret = MI125_mi125_checkcore_recv(tcp_con);
				break;
			case MI125_MI125_SET_TESTMODE:
				ret = MI125_mi125_set_testmode_recv(tcp_con);
				break;
			case MI125_MI125_GET_TEMPERATURE:
				ret = MI125_mi125_get_temperature_recv(tcp_con);
				break;
			case MI125_MI125_GET_CHANNELCALIBSTATUS:
				ret = MI125_mi125_get_channelcalibstatus_recv(tcp_con);
				break;
			case MI125_MI125_GET_VERSIONS:
				ret = MI125_mi125_get_versions_recv(tcp_con);
				break;
			case MI125_MI125_CHECKADC:
				ret = MI125_mi125_checkadc_recv(tcp_con);
				break;
			case MI125_MI125_SET_TRIGOUT:
				ret = MI125_mi125_set_trigout_recv(tcp_con);
				break;
			case MI125_MI125_GET_CLKMASTER:
				ret = MI125_mi125_get_clkmaster_recv(tcp_con);
				break;
			case MI125_MI125_CLOCKRESET:
				ret = MI125_mi125_clockreset_recv(tcp_con);
				break;
			case MI125_MI125_SCAN_I2C:
				ret = MI125_mi125_scan_i2c_recv(tcp_con);
				break;
			case MI125_MI125_TEST_LOCALOSCSW:
				ret = MI125_mi125_test_localoscsw_recv(tcp_con);
				break;

            case MO1000_PRESENCE:
				ret = Mo1000_Presence_recv(tcp_con);
				break;
			case MO1000_POWERUP:
				ret = Mo1000_PowerUp_recv(tcp_con);
				break;
			case MO1000_RESET:
				ret = Mo1000_Reset_recv(tcp_con);
				break;
			case MO1000_INIT:
				ret = Mo1000_Init_recv(tcp_con);
				break;
			case MO1000_WRITEREG:
				ret = Mo1000_WriteReg_recv(tcp_con);
				break;
			case MO1000_READREG:
				ret = Mo1000_ReadReg_recv(tcp_con);
				break;
			case MO1000_GETSTATUS:
				ret = Mo1000_GetStatus_recv(tcp_con);
				break;
			case MO1000_GETTEMPERATURE:
				ret = Mo1000_GetTemperature_recv(tcp_con);
				break;
			case MO1000_SETCLOCKCONFIG:
				ret = Mo1000_SetClockConfig_recv(tcp_con);
				break;
			case MO1000_DODACCALIBRATION:
				ret = Mo1000_DoDacCalibration_recv(tcp_con);
				break;
			case MO1000_GETCHANNELCALIBSTATUS:
				ret = Mo1000_GetChannelCalibStatus_recv(tcp_con);
				break;
			case MO1000_SETDACOUTCTRL:
				ret = Mo1000_SetDacOutCtrl_recv(tcp_con);
				break;
			case MO1000_SETDACPARINTERPOLATION:
				ret = Mo1000_SetDacParInterpolation_recv(tcp_con);
				break;
			case MO1000_SETDACPARDCOFFSET:
				ret = Mo1000_SetDacParDcOffset_recv(tcp_con);
				break;
			case MO1000_SETDACPARGAIN:
				ret = Mo1000_SetDacParGain_recv(tcp_con);
				break;
            case MO1000_DODACUPDATE:
				ret = Mo1000_DoDacUpdate_recv(tcp_con);
				break;
            case MO1000_SETDACPARACTCHANNEL:
				ret = Mo1000_SetDacParActChannel_recv(tcp_con);
				break;
            case MO1000_GETCLKMASTER:
				ret = Mo1000_GetClkMaster_recv(tcp_con);
				break;
            case MO1000_SETDACPARISINC:
				ret = Mo1000_SetDacParISinc_recv(tcp_con);
				break;
            case MO1000_SETDACPARISINCCTRL:
				ret = Mo1000_SetDacParISincCtrl_recv(tcp_con);
				break;
            case MO1000_SETDACPARFINEMOD:
				ret = Mo1000_SetDacParFineMod_recv(tcp_con);
				break;
            case MO1000_SETDACPARFINEMODCTRL:
				ret = Mo1000_SetDacParFineModCtrl_recv(tcp_con);
				break;
            case MO1000_SETDACPARDATAFORMAT:
				ret = Mo1000_SetDacParDataFormat_recv(tcp_con);
				break;
            case MO1000_SETDACPARQUADPHASE:
				ret = Mo1000_SetDacParQuadPhase_recv(tcp_con);
				break;
            case MO1000_GETVERSIONS:
				ret = Mo1000_GetVersions_recv(tcp_con);
				break;
            case MO1000_TESTLOCALOSCSW:
				ret = Mo1000_TestLocalOscSw_recv(tcp_con);
				break;
            case MO1000_TESTBRIDGERESET:
				ret = Mo1000_TestBridgeReset_recv(tcp_con);
				break;
            case MO1000_TESTDACRESET:
				ret = Mo1000_TestDacReset_recv(tcp_con);
				break;
            case MO1000_TESTPLLSEL:
				ret = Mo1000_TestPllSel_recv(tcp_con);
				break;
            case MO1000_TESTPLLPOWDOWN:
				ret = Mo1000_TestPllPowDown_recv(tcp_con);
				break;
            case MO1000_TESTPLLSYNC:
				ret = Mo1000_TestPllSync_recv(tcp_con);
				break;
            case MO1000_TESTDACIRQ:
				ret = Mo1000_TestDacIrq_recv(tcp_con);
				break;
            case MO1000_TESTDACCLKS:
				ret = Mo1000_TestDacClks_recv(tcp_con);
				break;
            case MO1000_TESTDCIFRAME:
				ret = Mo1000_TestDciFrame_recv(tcp_con);
				break;
            case MO1000_CHECKI2C:
				ret = Mo1000_CheckI2c_recv(tcp_con);
				break;
            case MO1000_GETCOREFREQUENCY:
				ret = Mo1000_GetCoreFrequency_recv(tcp_con);
				break;
            case MO1000_SETTESTMODE:
				ret = Mo1000_SetTestMode_recv(tcp_con);
				break;
            case MO1000_WRITECLOCKCONFIG:
				ret = Mo1000_WriteClockConfig_recv(tcp_con);
				break;
            case MO1000_GETPLLCONFIG:
				ret = Mo1000_GetPllConfig_recv(tcp_con);
				break;
            case MO1000_SETPLLCONFIG:
				ret = Mo1000_SetPllConfig_recv(tcp_con);
				break;

			case MI250_INIT:
				ret = MI250_Init_recv(tcp_con);
				break;
			case MI250_POWERUP:
				ret = MI250_Powerup_recv(tcp_con);
				break;
			case MI250_PRESENCE:
				ret = MI250_Presence_recv(tcp_con);
				break;
			case MI250_READSPI:
				ret = MI250_ReadSPIRegisters_recv(tcp_con);
				break;
			case MI250_WRITESPI:
				ret = MI250_WriteSPIRegisters_recv(tcp_con);
				break;
			case MI250_PLLCONFIG:
				ret = MI250_PLLConfig_recv(tcp_con);
				break;
			case MI250_GETPLLSTATUS:
				ret = MI250_PLLGetStatus_recv(tcp_con);
				break;
			/*case MI250_SETADCCONFIG:
				ret = MI250_SetConfig_recv(tcp_con);
				break;*/
			case MI250_SETADCGAIN:
				ret = MI250_SetGain_recv(tcp_con);
				break;
			case MI250_SETADCFINEGAIN:
				ret = MI250_SetFineGain_recv(tcp_con);
				break;
			case MI250_SETADCPEDESTAL:
				ret = MI250_SetPedestal_recv(tcp_con);
				break;
			/*case MI250_GETADCSTATUS:
				ret = MI250_GetStatus_recv(tcp_con);
				break;	*/
			case MI250_ADCRESET:
				ret = MI250_ADCreset_recv(tcp_con);
				break;
            case MI250_ARMADC:
				ret = MI250_ArmADC_recv(tcp_con);
				break;
			case MI250_SETDATAFORMAT :
				ret = MI250_SetDataFormat_recv(tcp_con);
				break;
			case MI250_SETSLEEPMODE :
				ret = MI250_SetSleepMode_recv(tcp_con);
				break;
			case MI250_SETOCT :
				ret = MI250_SetOCT_recv(tcp_con);
				break;
			case MI250_SELECT :
				ret = MI250_Select_recv(tcp_con);
				break;

			case OSCONFIG_GET_PARAMETER_VALUE:
				ret = os_config_get_parameter(tcp_con);
				break;
			case OSCONFIG_GET_FIRST_PARAMETER:
				ret = os_config_get_first_parameter(tcp_con);
				break;
			case OSCONFIG_GET_NEXT_PARAMETER:
				ret = os_config_get_next_parameter(tcp_con);
				break;
			case OSCONFIG_SET_PARAMETER_VALUE:
				ret = os_config_set_parameter(tcp_con);
				break;
			case OSCONFIG_UNSET_PARAMETER:
				ret = os_config_unset_parameter(tcp_con);
				break;
			case OSCONFIG_CLEAR_FILE:
				ret = os_config_clear_file(tcp_con);
				break;
			case OSCONFIG_APPLY_FILE:
				ret = os_config_apply_file(tcp_con);
				break;
			case OSCONFIG_SELECT_FILE:
				ret = os_config_select_file(tcp_con);
				break;
			case OSCONFIG_LOAD_FILE:
				ret = os_config_load_file(tcp_con);
				break;
			case OSCONFIG_SAVE_FILE:
				ret = os_config_save_file(tcp_con);
				break;
			case OSCONFIG_GET_FIRST_FILE:
				ret = os_config_get_first_file(tcp_con);
				break;
			case OSCONFIG_GET_NEXT_FILE:
				ret = os_config_get_next_file(tcp_con);
				break;

			case FMCLVDS_INIT:
				ret = fmclvds_init_recv(tcp_con);
				break;
			case FMCLVDS_POWERUP:
				ret = fmclvds_powerup_recv(tcp_con);
				break;
			case FMCLVDS_PRESENCE:
				ret = fmclvds_presence_recv(tcp_con);
				break;
			case FMCLVDS_SELECT:
				ret = fmclvds_select_send(tcp_con);
				break;
			case FMCLVDS_SETDIR:
				ret = fmclvds_setdir_recv(tcp_con);
				break;
			case FMCLVDS_SET_CHANNEL_PWDN:
				ret = fmclvds_pwdn_recv(tcp_con);
				break;
			case FMCLVDS_SET_CHANNEL_PREEMP:
				ret = fmclvds_preemp_recv(tcp_con);
				break;
			case PPSSYNC_PRESENCE:
				ret = PPSSync_Presence_recv(tcp_con);
				break;
			case PPSSYNC_CONFIGURE:
				ret = PPSSync_Configure_recv(tcp_con);
				break;
			case PPSSYNC_DACINIT:
				ret = PPSSync_DacInit_recv(tcp_con);
				break;
			case PPSSYNC_START:
				ret = PPSSync_Start_recv(tcp_con);
				break;
			case PPSSYNC_STOP:
				ret = PPSSync_Stop_recv(tcp_con);
				break;
			case PPSSYNC_GETINFO:
				ret = PPSSync_GetInfo_recv(tcp_con);
				break;
			case PPSSYNC_SAVEDAC:
				ret = PPSSync_SaveRefDacVal_recv(tcp_con);
				break;
			case PPSSYNC_READDAC:
				ret = PPSSync_ReadRefDacVal_recv(tcp_con);
				break;
			case PPSSYNC_SAVEPROFILE:
				ret = PPSSync_SavePIProfile_recv(tcp_con);
				break;
			case PPSSYNC_READPROFILE:
				ret = PPSSync_ReadPIProfile_recv(tcp_con);
				break;
			case LVDS_PRESENCE:
				ret = lvds_presence_recv(tcp_con);
				break;
			case LVDS_GETMODE:
				ret = lvds_getmode_recv(tcp_con);
				break;
			case LVDS_SETOUTPUTENABLE:
				ret = lvds_setoutputenable_recv(tcp_con);
				break;
			case LVDS_SETVALUE:
				ret = lvds_setvalue_recv(tcp_con);
				break;
			case LVDS_GETVALUE:
				ret = lvds_getvalue_recv(tcp_con);
				break;
			case LVDS_RESET:
				ret = lvds_reset_recv(tcp_con);
				break;
			case LVDS_RESET_FIFO:
				ret = lvds_reset_fifo_recv(tcp_con);
				break;
      case RTDEXSYNC_PRESENCE:
          ret = RTDExSync_Presence_recv(tcp_con);
          break;
      case RTDEXSYNC_CORERESET:
          ret = RTDExSync_CoreReset_recv(tcp_con);
          break;
      case RTDEXSYNC_CHANNELRESET:
          ret = RTDExSync_ChannelReset_recv(tcp_con);
          break;
      case RTDEXSYNC_CHANNELSTART:
          ret = RTDExSync_ChannelStart_recv(tcp_con);
          break;
      case RTDEXSYNC_CHANNELSTOP:
          ret = RTDExSync_ChannelStop_recv(tcp_con);
          break;
      case RTDEXSYNC_CHANNELINFO:
          ret = RTDExSync_ChannelInfo_recv(tcp_con);
          break;

            case SYSMON_READ:
                ret = Sysmon_Read_recv(tcp_con);
                break;
            case TIMESTAMP_PRESENCE:
                ret = Timestamp_Presence_recv(tcp_con);
                break;
            case TIMESTAMP_SETTIME:
                ret = Timestamp_SetTime_recv(tcp_con);
                break;
            case TIMESTAMP_GETTIME:
                ret = Timestamp_GetTime_recv(tcp_con);
                break;
            case TIMESTAMP_RESET:
                ret = Timestamp_Reset_recv(tcp_con);
                break;
                
#ifdef FMCOMMS3_IN_USE
            case FMCOMMS3_PRESENCE:                   
                ret = Fmcomms3_presence_recv(tcp_con);
                break;
            case FMCOMMS3_POWERUP:                    
                ret = Fmcomms3_powerup_recv(tcp_con);
                break;
            case FMCOMMS3_INIT:                       
                ret = Fmcomms3_init_recv(tcp_con);
                break;
            case FMCOMMS3_DIGITAL_CALIBRATION:                       
                ret = Fmcomms3_digital_calibration_recv(tcp_con);
                break;
            case FMCOMMS3_SET_SYNC_DELAY:                       
                ret = Fmcomms3_set_sync_delay_recv(tcp_con);
                break;
            case FMCOMMS3_SET_REFCLK_DELAY:                       
                ret = Fmcomms3_set_refclk_delay_recv(tcp_con);
                break;
            case FMCOMMS3_SET_SYNC_BITSLIP:                       
                ret = Fmcomms3_set_sync_bitslip_recv(tcp_con);
                break;
            case FMCOMMS3_SET_SYNC_SRC:                       
                ret = Fmcomms3_set_sync_src_recv(tcp_con);
                break;
            case FMCOMMS3_SOFT_SYNC:                       
                ret = Fmcomms3_soft_sync_recv(tcp_con);
                break;
            case FMCOMMS3_MCS:                       
                ret = Fmcomms3_mcs_recv(tcp_con);
                break;
            case FMCOMMS3_SET_MASTER_CALIB_RESULT:                       
                ret = Fmcomms3_set_master_calib_result_recv(tcp_con);
                break;
            case FMCOMMS3_SET_EXT_MASTER_CALIB_RESULT:                       
                ret = Fmcomms3_set_ext_master_calib_result_recv(tcp_con);
                break;
            case FMCOMMS3_GET_CALIB_RESULT:                       
                ret = Fmcomms3_get_calib_result_recv(tcp_con);
                break;                
            case FMCOMMS3_SET_EN_STATE_MACHINE_MODE:  
                ret = Fmcomms3_set_en_state_machine_mode_recv(tcp_con);
                break;
            case FMCOMMS3_GET_EN_STATE_MACHINE_MODE:  
                ret = Fmcomms3_get_en_state_machine_mode_recv(tcp_con);
                break;
            case FMCOMMS3_SET_SAMPLING_FREQ:          
                ret = Fmcomms3_set_sampling_freq_recv(tcp_con);
                break;
            case FMCOMMS3_GET_SAMPLING_FREQ:          
                ret = Fmcomms3_get_sampling_freq_recv(tcp_con);
                break;
            case FMCOMMS3_SET_TX_LO_FREQ:             
                ret = Fmcomms3_set_tx_lo_freq_recv(tcp_con);
                break;
            case FMCOMMS3_GET_TX_LO_FREQ:             
                ret = Fmcomms3_get_tx_lo_freq_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_RF_GAIN:             
                ret = Fmcomms3_set_rx_rf_gain_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_RF_GAIN:             
                ret = Fmcomms3_get_rx_rf_gain_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_RF_BANDWIDTH:        
                ret = Fmcomms3_set_rx_rf_bandwidth_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_RF_BANDWIDTH:        
                ret = Fmcomms3_get_rx_rf_bandwidth_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_LO_FREQ:             
                ret = Fmcomms3_set_rx_lo_freq_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_LO_FREQ:             
                ret = Fmcomms3_get_rx_lo_freq_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_RSSI:                
                ret = Fmcomms3_get_rx_rssi_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_GAIN_CONTROL_MODE:   
                ret = Fmcomms3_set_rx_gain_control_mode_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_GAIN_CONTROL_MODE:   
                ret = Fmcomms3_get_rx_gain_control_mode_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_FIR_CONFIG:          
                ret = Fmcomms3_set_rx_fir_config_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_FIR_CONFIG:          
                ret = Fmcomms3_get_rx_fir_config_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_FIR_EN_DIS:          
                ret = Fmcomms3_set_rx_fir_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_FIR_EN_DIS:          
                ret = Fmcomms3_get_rx_fir_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_RFDC_TRACK_EN_DIS:   
                ret = Fmcomms3_set_rx_rfdc_track_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_RFDC_TRACK_EN_DIS:   
                ret = Fmcomms3_get_rx_rfdc_track_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_BBDC_TRACK_EN_DIS:   
                ret = Fmcomms3_set_rx_bbdc_track_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_BBDC_TRACK_EN_DIS:   
                ret = Fmcomms3_get_rx_bbdc_track_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_SET_RX_QUAD_TRACK_EN_DIS:   
                ret = Fmcomms3_set_rx_quad_track_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_GET_RX_QUAD_TRACK_EN_DIS:   
                ret = Fmcomms3_get_rx_quad_track_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_SET_TX_ATTENUATION:         
                ret = Fmcomms3_set_tx_attenuation_recv(tcp_con);
                break;
            case FMCOMMS3_GET_TX_ATTENUATION:         
                ret = Fmcomms3_get_tx_attenuation_recv(tcp_con);
                break;
            case FMCOMMS3_SET_TX_RF_BANDWIDTH:        
                ret = Fmcomms3_set_tx_rf_bandwidth_recv(tcp_con);
                break;
            case FMCOMMS3_GET_TX_RF_BANDWIDTH:        
                ret = Fmcomms3_get_tx_rf_bandwidth_recv(tcp_con);
                break;
            case FMCOMMS3_SET_TX_FIR_CONFIG:          
                ret = Fmcomms3_set_tx_fir_config_recv(tcp_con);
                break;
            case FMCOMMS3_GET_TX_FIR_CONFIG:          
                ret = Fmcomms3_get_rx_fir_config_recv(tcp_con);
                break;
            case FMCOMMS3_SET_TX_FIR_EN_DIS:          
                ret = Fmcomms3_set_tx_fir_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_GET_TX_FIR_EN_DIS:          
                ret = Fmcomms3_get_tx_fir_en_dis_recv(tcp_con);
                break;
            case FMCOMMS3_SET_TRX_PATH_CLKS:          
                ret = Fmcomms3_set_trx_path_clks_recv(tcp_con);
                break;
            case FMCOMMS3_GET_TRX_PATH_CLKS:          
                ret = Fmcomms3_get_trx_path_clks_recv(tcp_con);
                break;
            case FMCOMMS3_SET_DAC_DATASEL:            
                ret = Fmcomms3_set_dac_datasel_recv(tcp_con);
                break;
            case FMCOMMS3_GET_DAC_DATASEL:            
                ret = Fmcomms3_get_dac_datasel_recv(tcp_con);
                break;
            case FMCOMMS3_SET_DDS_FREQ:               
                ret = Fmcomms3_set_dds_freq_recv(tcp_con);
                break;
            case FMCOMMS3_GET_DDS_FREQ:               
                ret = Fmcomms3_get_dds_freq_recv(tcp_con);
                break;
            case FMCOMMS3_SET_DDS_PHASE:              
                ret = Fmcomms3_set_dds_phase_recv(tcp_con);
                break;
            case FMCOMMS3_GET_DDS_PHASE:              
                ret = Fmcomms3_get_dds_phase_recv(tcp_con);
                break;
            case FMCOMMS3_SET_DDS_SCALE:              
                ret = Fmcomms3_set_dds_scale_recv(tcp_con);
                break;
            case FMCOMMS3_GET_DDS_SCALE:              
                ret = Fmcomms3_get_dds_scale_recv(tcp_con);
                break;
            case FMCOMMS3_SPI_WRITE:                                                               
                ret = Fmcomms3_spi_write_recv(tcp_con);                                             
                break;
            case FMCOMMS3_SPI_READ:              
                ret = Fmcomms3_spi_read_recv(tcp_con);
                break;
#endif // FMCOMMS3_IN_USE
#endif //PERSEUS                
            case RADIO640_PRESENCE:
                ret = Radio640_Presence_recv(tcp_con);
                break;
            case RADIO640_POWERUP:
                ret = Radio640_PowerUp_recv(tcp_con);
                break;
            case RADIO640_INIT:
                ret = Radio640_Init_recv(tcp_con);
                break;
            case RADIO640_DIGITAL_CALIBRATION:
                ret = Radio640_DigitalCalibration_recv(tcp_con);
                break;
            case RADIO640_SET_SYNC_OUTPUT_DELAY:
                ret = Radio640_SetSyncOutputDelay_recv(tcp_con);
                break;
            case RADIO640_SET_SYNC_INPUT_DELAY:
                ret = Radio640_SetSyncInputDelay_recv(tcp_con);
                break;
            case RADIO640_SET_REFCLK_DELAY:
                ret = Radio640_SetRefClkDelay_recv(tcp_con);
                break;
            case RADIO640_SET_SYNC_BITSLIP:
                ret = Radio640_SetSyncBitslip_recv(tcp_con);
                break;
            case RADIO640_SET_SYNC_SRC:
                ret = Radio640_SetSyncSrc_recv(tcp_con);
                break;
            case RADIO640_SOFT_SYNC:
                ret = Radio640_SoftSync_recv(tcp_con);
                break;
            case RADIO640_MCS:
                ret = Radio640_Mcs_recv(tcp_con);
                break;
            case RADIO640_SET_MASTER_CALIB_RESULT:
                ret = Radio640_SetMasterCalibResult_recv(tcp_con);
                break;
            case RADIO640_SET_EXT_MASTER_CALIB_RESULT:
                ret = Radio640_SetExtMasterCalibResult_recv(tcp_con);
                break;
            case RADIO640_GET_CALIB_RESULT:
                ret = Radio640_GetCalibResult_recv(tcp_con);
                break;
            case RADIO640_SET_EN_STATE_MACHINE_MODE:
                ret = Radio640_SetEnsmMode_recv(tcp_con);
                break;
            case RADIO640_GET_EN_STATE_MACHINE_MODE:
                ret = Radio640_GetEnsmMode_recv(tcp_con);
                break;
            case RADIO640_GET_EN_STATE_MACHINE_STATE:
                ret = Radio640_GetEnsmState_recv(tcp_con);
                break;
            case RADIO640_SET_SAMPLING_FREQ:
                ret = Radio640_SetSamplingFreq_recv(tcp_con);
                break;
            case RADIO640_GET_SAMPLING_FREQ:
                ret = Radio640_GetSamplingFreq_recv(tcp_con);
                break;
            case RADIO640_SET_TX_LO_FREQ:
                ret = Radio640_SetTxLoFreq_recv(tcp_con);
                break;
            case RADIO640_GET_TX_LO_FREQ:
                ret = Radio640_GetTxLoFreq_recv(tcp_con);
                break;
            case RADIO640_SET_RX_RF_GAIN:
                ret = Radio640_SetRxRfGain_recv(tcp_con);
                break;
            case RADIO640_GET_RX_RF_GAIN:
                ret = Radio640_GetRxRfGain_recv(tcp_con);
                break;
            case RADIO640_SET_RX_RF_BANDWIDTH:
                ret = Radio640_SetRxRfBandwidth_recv(tcp_con);
                break;
            case RADIO640_GET_RX_RF_BANDWIDTH:
                ret = Radio640_GetRxRfBandwidth_recv(tcp_con);
                break;
            case RADIO640_SET_RX_LO_FREQ:
                ret = Radio640_SetRxLoFreq_recv(tcp_con);
                break;
            case RADIO640_GET_RX_LO_FREQ:
                ret = Radio640_GetRxLoFreq_recv(tcp_con);
                break;
            case RADIO640_GET_RX_RSSI:
                ret = Radio640_GetRxRssi_recv(tcp_con);
                break;
            case RADIO640_SET_RX_GAIN_CONTROL_MODE:
                ret = Radio640_SetRxGainControlMode_recv(tcp_con);
                break;
            case RADIO640_GET_RX_GAIN_CONTROL_MODE:
                ret = Radio640_GetRxGainControlMode_recv(tcp_con);
                break;
            case RADIO640_SET_RX_FIR_CONFIG:
                ret = Radio640_SetRxFirConfig_recv(tcp_con);
                break;
            case RADIO640_GET_RX_FIR_CONFIG:
                ret = Radio640_GetRxFirConfig_recv(tcp_con);
                break;
            case RADIO640_SET_RX_FIR_EN_DIS:
                ret = Radio640_SetRxFirEnDis_recv(tcp_con);
                break;
            case RADIO640_GET_RX_FIR_EN_DIS:
                ret = Radio640_GetRxFirEnDis_recv(tcp_con);
                break;
            case RADIO640_SET_RX_RFDC_TRACK_EN_DIS:
                ret = Radio640_SetRxRfdcTrackEnDis_recv(tcp_con);
                break;
            case RADIO640_GET_RX_RFDC_TRACK_EN_DIS:
                ret = Radio640_GetRxRfdcTrackEnDis_recv(tcp_con);
                break;
            case RADIO640_SET_RX_BBDC_TRACK_EN_DIS:
                ret = Radio640_SetRxBbdcTrackEnDis_recv(tcp_con);
                break;
            case RADIO640_GET_RX_BBDC_TRACK_EN_DIS:
                ret = Radio640_GetRxBbdcTrackEnDis_recv(tcp_con);
                break;
            case RADIO640_SET_RX_QUAD_TRACK_EN_DIS:
                ret = Radio640_SetRxQuadTrackEnDis_recv(tcp_con);
                break;
            case RADIO640_GET_RX_QUAD_TRACK_EN_DIS:
                ret = Radio640_GetRxQuadTrackEnDis_recv(tcp_con);
                break;
            case RADIO640_SET_TX_ATTENUATION:
                ret = Radio640_SetTxAttenuation_recv(tcp_con);
                break;
            case RADIO640_GET_TX_ATTENUATION:
                ret = Radio640_GetTxAttenuation_recv(tcp_con);
                break;
            case RADIO640_SET_TX_RF_BANDWIDTH:
                ret = Radio640_SetTxRfBandwidth_recv(tcp_con);
                break;
            case RADIO640_GET_TX_RF_BANDWIDTH:
                ret = Radio640_GetTxRfBandwidth_recv(tcp_con);
                break;
            case RADIO640_SET_TX_FIR_CONFIG:
                ret = Radio640_SetTxFirConfig_recv(tcp_con);
                break;
            case RADIO640_GET_TX_FIR_CONFIG:
                ret = Radio640_GetTxFirConfig_recv(tcp_con);
                break;
            case RADIO640_SET_TX_FIR_EN_DIS:
                ret = Radio640_SetTxFirEnDis_recv(tcp_con);
                break;
            case RADIO640_GET_TX_FIR_EN_DIS:
                ret = Radio640_GetTxFirEnDis_recv(tcp_con);
                break;
            case RADIO640_SET_TRX_PATH_CLKS:
                ret = Radio640_SetTrxPathClks_recv(tcp_con);
                break;
            case RADIO640_GET_TRX_PATH_CLKS:
                ret = Radio640_GetTrxPathClks_recv(tcp_con);
                break;
            case RADIO640_WRITE:
                ret = Radio640_Write_recv(tcp_con);
                break;
            case RADIO640_READ:
                ret = Radio640_Read_recv(tcp_con);
                break;
            case RADIO640_DISABLE_DESIGN_CLK:
                ret = Radio640_DisableDesignClk_recv(tcp_con);
                break;
            case RADIO640_ENABLE_DESIGN_CLK:
                ret = Radio640_EnableDesignClk_recv(tcp_con);
                break;
            case RADIO640_IS_CLOCK_MASTER:
                ret = Radio640_IsClockMaster_recv(tcp_con);
                break;
            case RADIO640_SET_CLK_SRC:
                ret = Radio640_SetClkSrc_recv(tcp_con);
                break;
            case RADIO640_GET_CLK_SRC:
                ret = Radio640_GetClkSrc_recv(tcp_con);
                break;
            case RADIO640_GET_BOARD_TEMP:
                ret = Radio640_GetBoardTemp_recv(tcp_con);
                break;
            case RADIO640_GET_TRX_TEMP:
                ret = Radio640_GetTrxTemp_recv(tcp_con);
                break;
            case RADIO640_SET_EN_AGC_PIN_CONFIG:
                ret = Radio640_SetEnAgcPinConfig_recv(tcp_con);
                break;
            case RADIO640_SET_ENABLE_PIN_CONFIG:
                ret = Radio640_SetEnablePinConfig_recv(tcp_con);
                break;
            case RADIO640_SET_TXNRX_PIN_CONFIG:
                ret = Radio640_SetTxnrxPinConfig_recv(tcp_con);
                break;
            case RADIO640_SET_CTRL_IN_PIN_CONFIG:
                ret = Radio640_SetCtrlInPinConfig_recv(tcp_con);
                break;
            case RADIO640_SET_CTRL_OUT_CONFIG:
                ret = Radio640_SetCtrlOutConfig_recv(tcp_con);
                break;
            case RADIO640_GET_CTRL_OUT_PIN_VALUE:
                ret = Radio640_GetCtrlOutPinValue_recv(tcp_con);
                break;
            case RADIO640_GET_CTRL_OUT_PIN_LOW_HIGH_VALUE:
                ret = Radio640_GetCtrlOutPinLowHighValue_recv(tcp_con);
                break;
            case RADIO640_SET_POWER_MONITOR_MODE:
                ret = Radio640_SetPowerMonitorMode_recv(tcp_con);
                break;
            case RADIO640_SET_POWER_MONITOR_CONVERSION_TIME:
                ret = Radio640_SetPowerMonitorConversionTime_recv(tcp_con);
                break;
            case RADIO640_SET_POWER_MONITOR_AVERAGING:
                ret = Radio640_SetPowerMonitorAveraging_recv(tcp_con);
                break;
            case RADIO640_READ_POWER_MONITOR_BUS_VOLTAGE:
                ret = Radio640_ReadPowerMonitorBusVoltage_recv(tcp_con);
                break;
            case RADIO640_READ_POWER_MONITOR_BUS_POWER:
                ret = Radio640_ReadPowerMonitorPower_recv(tcp_con);
                break;
            case RADIO640_READ_POWER_MONITOR_BUS_CURRENT:
                ret = Radio640_ReadPowerMonitorCurrent_recv(tcp_con);
                break;
            case RADIO640_SET_RX_RF_FILTER:
                ret = Radio640_SetRxRfFilter_recv(tcp_con);
                break;
            case RADIO640_SET_TX_EXT_ATTENUATION:
                ret = Radio640_SetTxExtAttenuation_recv(tcp_con);
                break;
            case RADIO640_TUNE_OSC:
                ret = Radio640_TuneOsc_recv(tcp_con);
                break;
            case RADIO640_SET_TX_ENABLE_STATE:
                ret = Radio640_SetTxEnableState_recv(tcp_con);
                break;
            case RADIO640_SET_RX_ENABLE_STATE:
                ret = Radio640_SetRxEnableState_recv(tcp_con);
                break;
            case RADIO640_STORE_RX_FAST_LOCK_PROFILE:
                ret = Radio640_StoreRxFastLockProfile_recv(tcp_con);
                break;
            case RADIO640_RECALL_RX_FAST_LOCK_PROFILE:
                ret = Radio640_RecallRxFastLockProfile_recv(tcp_con);
                break;
            case RADIO640_LOAD_RX_FAST_LOCK_PROFILE:
                ret = Radio640_LoadRxFastLockProfile_recv(tcp_con);
                break;
            case RADIO640_SAVE_RX_FAST_LOCK_PROFILE:
                ret = Radio640_SaveRxFastLockProfile_recv(tcp_con);
                break;
            case RADIO640_STORE_TX_FAST_LOCK_PROFILE:
                ret = Radio640_StoreTxFastLockProfile_recv(tcp_con);
                break;
            case RADIO640_RECALL_TX_FAST_LOCK_PROFILE:
                ret = Radio640_RecallTxFastLockProfile_recv(tcp_con);
                break;
            case RADIO640_LOAD_TX_FAST_LOCK_PROFILE:
                ret = Radio640_LoadTxFastLockProfile_recv(tcp_con);
                break;
            case RADIO640_SAVE_TX_FAST_LOCK_PROFILE:
                ret = Radio640_SaveTxFastLockProfile_recv(tcp_con);
                break;
            case RADIO640_SET_RX_LO_EXT:
                ret = Radio640_SetRxLoExt_recv(tcp_con);
                break;
            case RADIO640_SET_TX_LO_EXT:
                ret = Radio640_SetTxLoExt_recv(tcp_con);
                break;
            case RADIO640_SET_TX_AUTO_CALIB_ENABLE:
                ret = Radio640_SetTxAutoCalibEnable_recv(tcp_con);
                break;
            case RADIO640_GET_TX_AUTO_CALIB_ENABLE:
                ret = Radio640_GetTxAutoCalibEnable_recv(tcp_con);
                break;
            case RADIO640_DO_CALIB:
                ret = Radio640_DoCalib_recv(tcp_con);
                break;
            case RADIO640_SET_RF_SWITCH_CONFIG:
                ret = Radio640_SetRfSwitchConfig_recv(tcp_con);
                break;
            case RADIO640_SET_TRIGGER_DELAY:
                ret = Radio640_SetTriggerDelay_recv(tcp_con);
                break;
            case RADIO640_SET_TRIGGER_BITSLIP:
                ret = Radio640_SetTriggerBitslip_recv(tcp_con);
                break;
            case RADIO640_READ_CLK_FREQ:
                ret = Radio640_ReadClkFreq_recv(tcp_con);
                break;
            case RADIO640_GET_TRIGGER_VALUE:
                ret = Radio640_GetTriggerValue_recv(tcp_con);
                break;
            case RADIO640_GET_TRIGGER_LOW_HIGH_VALUE:
                ret = Radio640_GetTriggerLowHighValue_recv(tcp_con);
                break;
            case RADIO640_GET_INPUT_SYNC_VALUE:
                ret = Radio640_GetInputSyncValue_recv(tcp_con);
                break;
            case RADIO640_GET_INPUT_SYNC_LOW_HIGH_VALUE:
                ret = Radio640_GetInputSyncLowHighValue_recv(tcp_con);
                break;
            case RADIO640_SET_CLK_TO_EXTENDED_FMC_ENABLE:
                ret = Radio640_SetClkToExtendedFmcEnable_recv(tcp_con);
                break;
            case RADIO640_SET_REF_OUT_ENABLE:
                ret = Radio640_SetRefOutEnable_recv(tcp_con);
                break;
             case RADIO640_SET_COMPLEX_MULTIPLIER:
                ret = Radio640_SetComplexMultiplier_recv(tcp_con);
                break;
            case RADIO640_GET_COMPLEX_MULTIPLIER:
                ret = Radio640_GetComplexMultiplier_recv(tcp_con);
                break;
            case RADIO640_SET_DATA_DELAY:
                ret = Radio640_SetDataDelay_recv(tcp_con);
                break;
            case RADIO640_GET_DATA_DELAY:
                ret = Radio640_GetDataDelay_recv(tcp_con);
                break;               
                
                
			case CCECTRL_VERSION:
				ret = string_send(tcp_con, cce_version);
				break;
			case CCECTRL_NORMAL_MODE:
				tcp_con->mode = NORMAL_MODE;
				ret = OK;
				break;
			case CCECTRL_TURBO_MODE:
				tcp_con->mode = TURBO_MODE;
				uint_send(tcp_con, OK);
				break;
			case CCECTRL_GET_MAC:
				ret = string_send(tcp_con, cce_mac);
				break;
                
			case CARRIER_PRESENCE:
                // Command deprecated.
                // Only supported for old EAPI to not return error
                // when trying to connect to the platform in order to update
                // to the new CCE.
                ret = 1;
				break;
			case CARRIER_REBOOT:
                // Send response
                uint_send(tcp_con, OK);
                // Close TCP connection
                close(tcp_con->stream);
                free(tcp_con);
                // Sleep and reboot platform
                usleep(100);
				Carrier_Reboot(hCarrier);
				break;
            case CARRIER_GETTYPE:
                ret = Carrier_GetType_recv(tcp_con);
                break;
            case CARRIER_GETMODULEBASEADDR:
                ret = Carrier_GetModuleBaseAddr_recv(tcp_con);
                break;
            case CARRIER_LEDSETVALUE:
                ret = Carrier_LedSetValue_recv(tcp_con);
                break;
            case CARRIER_LEDGETVALUE:
                ret = Carrier_LedGetValue_recv(tcp_con);
                break;
            case CARRIER_TCLKSETSTATE:
                ret = Carrier_TClkSetState_recv(tcp_con);
                break;
            case CARRIER_TCLKGETSTATE:
                ret = Carrier_TClkGetState_recv(tcp_con);
                break;
            case CARRIER_TCLKSETTXSOURCE:
                ret = Carrier_TClkSetTxSource_recv(tcp_con);
                break;
            case CARRIER_TCLKGETTXSOURCE:
                ret = Carrier_TClkGetTxSource_recv(tcp_con);
                break;
            case CARRIER_FMCBIDIRCLKSETCONNECTION:
                ret = Carrier_FmcBidirClkSetConnection_recv(tcp_con);
                break;
            case CARRIER_FMCBIDIRCLKGETCONNECTION:
                ret = Carrier_FmcBidirClkGetConnection_recv(tcp_con);
                break;
            case CARRIER_SETOSCSTATE:
                ret = Carrier_SetOscState_recv(tcp_con);
                break;
            case CARRIER_GETOSCSTATE:
                ret = Carrier_GetOscState_recv(tcp_con);
                break;
            case CARRIER_FCLKASETCONNECTION:
                ret = Carrier_FClkASetConnection_recv(tcp_con);
                break;
            case CARRIER_FCLKAGETCONNECTION:
                ret = Carrier_FClkAGetConnection_recv(tcp_con);
                break;
            case CARRIER_I2CREAD:
                ret = Carrier_I2cRead_recv(tcp_con);
                break;
            case CARRIER_I2CWRITE:
                ret = Carrier_I2cWrite_recv(tcp_con);
                break;
            case CARRIER_I2CREADNOWRDATA:
                ret = Carrier_I2cReadNoWrData_recv(tcp_con);
                break;

			default:
				ret = CCEUNIMPLEMENTED;
				break;
			}

            //send result
            //debug("mode\n", ret);
            if(tcp_con->mode == NORMAL_MODE)
            {
                uint_send(tcp_con, ret);
                debug("return value: 0x%08X\n", ret);
            }
            pthread_mutex_unlock(&bcl);
        }
    }
    
    debug("connection has ended: %i\n", tcp_con->has_ended);

    close(tcp_con->stream);
    free(tcp_con);
    pthread_exit(NULL);
}



