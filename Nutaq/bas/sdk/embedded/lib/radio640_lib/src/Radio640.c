/***************************************************************************//**
 *   @file   ad9361_api.c
 *   @brief  Implementation of AD9361 API Driver.
 *   @author DBogdan (dragos.bogdan@analog.com)
********************************************************************************
 * Copyright 2013(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "bas_debug.h"

#include "Radio640.h"
#include "Radio640_regs.h"
#include "Radio640_config.h"

#include "Radio640DevCom.h"
#include "Radio640DevCom_defines.h"

#include "DevCom.h"
#include "DevCom_defines.h"

#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

#include "Ad9361Api.h"
#include "Ad9361.h"
#include "Ad9361DacCore.h"
#include "Ad9361Util.h"

#include "Lm75.h"
#include "Lm75_defines.h"

#include "Ina226.h"
#include "Ina226_defines.h"

/******************************************************************************/
/************************ Constants Definitions *******************************/
/******************************************************************************/

typedef struct Radio640_stRadio640
{
    // Memory map
    Radio640_stCrRegisterMap * pCrRegs;

    // Allocated objects
    Radio640DevCom_hComCore hComCore;
    DevCom_hCom hCom;

    struct Ad9361_stPhy * pAd9361Phy;
    Lm75_stCore Lm75;                   ///< Temperature related driver information
    Ina226_hIna226 hPowerMonitor12V;    ///< Power monitor 12V related driver information
    Ina226_hIna226 hPowerMonitorVadj;   ///< Power monitor Vadj related driver information

    // Local variables
    Carrier_handle_t hCarrier;
    unsigned int Zone;
    Carrier_eFmcConnectorNum_t FmcConnector;
    Carrier_eFmcBoardNum_t BoardPosition;
    uint8_t u8ClockMaster;
    uint8_t u8RefOutEnable;
    uint8_t u8ClkToExtendedFmcEnable;
    uint8_t u8HasExtendedFmcConnector;
    uint8_t u8IsRevA;

}Radio640_stRadio640;

int8_t Perseus611xLanesDelays[8][7] =
{
    //FMC1
    //Rx
    {0, 5, 2, 0, -4, -4, 2},
    //Tx
    {0, 3, 0, 1, -2, 0, -2},
    
    //FMC2
    //Rx
    {0, 6, 2, 1, -2, -3, 3},
    //Tx
    {0, 1, -2, -2, -5, -2, -2},
    
    //FMC3
    //Rx
    {0, 4, 1, 0, -4, -4, 3},
    //Tx
    {0, 3, 0, 0, -2, 0, 0},    
    
    //FMC4
    //Rx
    {0, 2, 0, 0, -6, -6, 2},
    //Tx
    {0, 1, 0, 0, -1, -1, -1}    
};

int8_t Perseus601xLanesDelays[4][7] =
{
    //FMC1
    //Rx
    {0, 1, 0, -1, -2, -3, 1},
    //Tx
    {0, 1, 0, 1, -1, -1, -4},
    
    //FMC2
    //Rx
    {0, 3, 1, 1, -1, -2, 2},
    //Tx
    {0, 1, 0, -1, -1, -1, -4}  
};

/******************************************************************************/
/*                          Private functions                                 */
/******************************************************************************/

 /****************************************************************************
 *   NAME : _Radio640_GetI2cAddr
 ************************************************************************//**
 *
 * Get the address list of all the I2C devices. The device addresses depend
 * on the FMC connector GA and on the type of the RADIO640 board.
 *
 * @param [in,out] board
 *    This structure is the one returned at module opening with Radio640_Open().
 *
 * @param [out] pI2cDevList
 *    Pointer to a structure to return the address list of all the I2C devices
 *
 * @return   Error code: #eRadio640Ok if no problem is found, !#eRadio640Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured.
 *
 ***************************************************************************/
Result _Radio640_GetI2cAddr(Radio640_hRadio640 hRadio640, Radio640DevCom_stI2cAddr * pI2cDevList)
{
    Result res;
    unsigned char ga;
    char FmcInfoType[16];
    char FmcInfoHrev[16];

    res = Carrier_FmcGetInfo(hRadio640->hCarrier, hRadio640->FmcConnector,
                             hRadio640->BoardPosition, CARRIER_INFO_BOARDTYPE,
                             FmcInfoType, sizeof(FmcInfoType));
     if(FAILURE(res)) return res;

    res = Carrier_FmcGetInfo(hRadio640->hCarrier, hRadio640->FmcConnector,
                             hRadio640->BoardPosition, CARRIER_INFO_HARDREV,
                             FmcInfoHrev, sizeof(FmcInfoHrev));
     if(FAILURE(res)) return res;

    res = Carrier_FmcGetGA(hRadio640->hCarrier, hRadio640->FmcConnector, &ga);
    if(FAILURE(res)) return res;
    
    // Populate internal Radio640 variable to know if the 
    // has board has an extended FMC connector on top of its board
    // or not.
    if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_RADIO640_E)==0)
    {
        hRadio640->u8HasExtendedFmcConnector = 1;
    }
    else
    {
        hRadio640->u8HasExtendedFmcConnector = 0;
    }
    
    //
    if(strcmp(FmcInfoHrev, "HREV:A")==0)
    {
        hRadio640->u8IsRevA = 1;
        debug("Radio640 hardware is Rev A\n");
    }
    else
    {
        hRadio640->u8IsRevA = 0;
        debug("Radio640 hardware is not Rev A\n");
    }

    // Get I2C addr from GA
    if(ga==0)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_RADIO640_E)==0)
            *pI2cDevList = RADIO640_I2C_ADDR_MAIN_GA00;
        else
            *pI2cDevList = RADIO640_I2C_ADDR_SEC_GA00;
    }
    else if(ga==1)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_RADIO640_E)==0)
            *pI2cDevList = RADIO640_I2C_ADDR_MAIN_GA01;
        else
            *pI2cDevList = RADIO640_I2C_ADDR_SEC_GA01;
    }
    else if(ga==2)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_RADIO640_E)==0)
            *pI2cDevList = RADIO640_I2C_ADDR_MAIN_GA10;
        else
            *pI2cDevList = RADIO640_I2C_ADDR_SEC_GA10;
    }
    else
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_RADIO640_E)==0)
            *pI2cDevList = RADIO640_I2C_ADDR_MAIN_GA11;
        else
            *pI2cDevList = RADIO640_I2C_ADDR_SEC_GA11;
    }

    return eRadio640Ok;
}

Result _Radio640_DisableHmc7043(Radio640_hRadio640 hRadio640)
{
    // Hardware reset the HMC7043 chip with the reset pin
    hRadio640->pCrRegs->stIO.uClkResetPin = 1;
    
    return eRadio640Ok;
}

Result _Radio640_InitHmc7043(Radio640_hRadio640 hRadio640)
{
    Result res;
    uint32_t i;
    
    // Hardware reset the HMC7043 chip with the reset pin
    hRadio640->pCrRegs->stIO.uClkResetPin = 1;
    Carrier_usleep(hRadio640->hCarrier, 10);
    hRadio640->pCrRegs->stIO.uClkResetPin = 0;
    Carrier_usleep(hRadio640->hCarrier, 10);

    debug("Radio640_InitHmc7043 : num reg = %u\n",(unsigned int)(sizeof(RADIO640_HMC7043_INIT_REGS)/sizeof(RADIO640_HMC7043_INIT_REGS[0])));
    
    // Load all initialization rregisters
    for(i=0; i<(sizeof(RADIO640_HMC7043_INIT_REGS)/sizeof(RADIO640_HMC7043_INIT_REGS[0])); i++)
    {        
        res = Radio640_Write(hRadio640, eRadio640DeviceClk, 
            RADIO640_HMC7043_INIT_REGS[i][0], RADIO640_HMC7043_INIT_REGS[i][1]);
        if(FAILURE(res)) return res;
    }
    
    // Set GLBL_RESTART to 1
    res = Radio640_Write(hRadio640, eRadio640DeviceClk, 
        0x2d, 0x2);
    if(FAILURE(res)) return res;
    
    // Set GLBL_RESTART back to 0
    res = Radio640_Write(hRadio640, eRadio640DeviceClk, 
        0x2d, 0x0);
    if(FAILURE(res)) return res;

    return eRadio640Ok;
}


// Create AD9361 InitParam from Radio640 InitParam
Result _Radio640_CreateAd9361Param(Radio640_stInitParam * pInitParam, Ad9361Api_stInitParam * pAd9361Param)
{
    int i;

    if(pInitParam == NULL) return eRadio640InvPtr;
    if(pAd9361Param == NULL) return eRadio640InvPtr;

    /*
     * Set default AD9361 parameters
     */
    *pAd9361Param = DefaultAd9361Param;

    /*
     * Set parameters from the Radio640 InitParam structure
     */

    // Parameters with different names
    pAd9361Param->reference_clk_rate                                    = pInitParam->u32RefClkFreq;
    pAd9361Param->frequency_division_duplex_mode_enable                 = pInitParam->u8FddModeEnable;
    pAd9361Param->frequency_division_duplex_independent_mode_enable     = pInitParam->u8FddIndependentModeEnable;
    pAd9361Param->tdd_skip_vco_cal_enable                               = pInitParam->u8TddSkipVcoCalEnable;
    pAd9361Param->tdd_use_fdd_vco_tables_enable                         = pInitParam->u8TddUseFddVcoTablesEnable;
    pAd9361Param->tx_fastlock_delay_ns                                  = pInitParam->u32TxFastlockDelayNs;
    pAd9361Param->rx_fastlock_delay_ns                                  = pInitParam->u32RxFastlockDelayNs;
    pAd9361Param->rx_fastlock_pincontrol_enable                         = pInitParam->u8RxFastlockPinControlEnable;
    pAd9361Param->tx_fastlock_pincontrol_enable                         = pInitParam->u8TxFastlockPinControlEnable;
    pAd9361Param->external_rx_lo_enable                                 = pInitParam->u8ExtRxLoEnable;
    pAd9361Param->external_tx_lo_enable                                 = pInitParam->u8ExtTxLoEnable;
    pAd9361Param->split_gain_table_mode_enable                          = pInitParam->u8SplitGainTableModeEnable;
    pAd9361Param->qec_tracking_slow_mode_enable                         = pInitParam->u8RxQuadTrackSlowModeEnable;
    pAd9361Param->dc_offset_tracking_update_event_mask                  = pInitParam->u8DcOffsetTrackingUpdateEventMask;
    pAd9361Param->dc_offset_attenuation_high_range                      = pInitParam->u8DcOffsetAttenuationHighRange;
    pAd9361Param->dc_offset_attenuation_low_range                       = pInitParam->u8DcOffsetAttenuationLowRange;
    pAd9361Param->dc_offset_count_high_range                            = pInitParam->u8DcOffsetCountHighRange;
    pAd9361Param->dc_offset_count_low_range                             = pInitParam->u8DcOffsetCountLowRange;
    pAd9361Param->ensm_enable_pin_pulse_mode_enable                     = pInitParam->u8EnsmEnablePinPulseModeEnable;
    pAd9361Param->ensm_enable_txnrx_control_enable                      = pInitParam->u8EnsmEnableTxnrxControlEnable;
    pAd9361Param->rx_synthesizer_frequency_hz                           = pInitParam->u64RxRfFreq;
    pAd9361Param->tx_synthesizer_frequency_hz                           = pInitParam->u64TxRfFreq;
    pAd9361Param->rf_rx_bandwidth_hz                                    = pInitParam->u32RxRfBandwidth;
    pAd9361Param->rf_tx_bandwidth_hz                                    = pInitParam->u32TxRfBandwidth;
    for(i=0; i<6; i++)
    {
        pAd9361Param->rx_path_clock_frequencies[i]                      = pInitParam->au32RxClockFreq[i];
        pAd9361Param->tx_path_clock_frequencies[i]                      = pInitParam->au32TxClockFreq[i];
    }

    // Parameters with the same names
    pAd9361Param->tx_attenuation_mdB                                    = pInitParam->tx_attenuation_mdB;
    pAd9361Param->update_tx_gain_in_alert_enable                        = pInitParam->update_tx_gain_in_alert_enable;
    pAd9361Param->clk_output_mode_select                                = pInitParam->clk_output_mode_select;
    pAd9361Param->gc_rx1_mode                                           = pInitParam->gc_rx1_mode;
    pAd9361Param->gc_rx2_mode                                           = pInitParam->gc_rx2_mode;
    pAd9361Param->gc_adc_large_overload_thresh                          = pInitParam->gc_adc_large_overload_thresh;
    pAd9361Param->gc_adc_ovr_sample_size                                = pInitParam->gc_adc_ovr_sample_size;
    pAd9361Param->gc_adc_small_overload_thresh                          = pInitParam->gc_adc_small_overload_thresh;
    pAd9361Param->gc_dec_pow_measurement_duration                       = pInitParam->gc_dec_pow_measurement_duration;
    pAd9361Param->gc_dig_gain_enable                                    = pInitParam->gc_dig_gain_enable;
    pAd9361Param->gc_lmt_overload_high_thresh                           = pInitParam->gc_lmt_overload_high_thresh;
    pAd9361Param->gc_lmt_overload_low_thresh                            = pInitParam->gc_lmt_overload_low_thresh;
    pAd9361Param->gc_low_power_thresh                                   = pInitParam->gc_low_power_thresh;
    pAd9361Param->gc_max_dig_gain                                       = pInitParam->gc_max_dig_gain;
    pAd9361Param->mgc_dec_gain_step                                     = pInitParam->mgc_dec_gain_step;
    pAd9361Param->mgc_inc_gain_step                                     = pInitParam->mgc_inc_gain_step;
    pAd9361Param->mgc_rx1_ctrl_inp_enable                               = pInitParam->mgc_rx1_ctrl_inp_enable;
    pAd9361Param->mgc_rx2_ctrl_inp_enable                               = pInitParam->mgc_rx2_ctrl_inp_enable;
    pAd9361Param->mgc_split_table_ctrl_inp_gain_mode                    = pInitParam->mgc_split_table_ctrl_inp_gain_mode;
    pAd9361Param->agc_adc_large_overload_exceed_counter                 = pInitParam->agc_adc_large_overload_exceed_counter;
    pAd9361Param->agc_adc_large_overload_inc_steps                      = pInitParam->agc_adc_large_overload_inc_steps;
    pAd9361Param->agc_adc_lmt_small_overload_prevent_gain_inc_enable    = pInitParam->agc_adc_lmt_small_overload_prevent_gain_inc_enable;
    pAd9361Param->agc_adc_small_overload_exceed_counter                 = pInitParam->agc_adc_small_overload_exceed_counter;
    pAd9361Param->agc_dig_gain_step_size                                = pInitParam->agc_dig_gain_step_size;
    pAd9361Param->agc_dig_saturation_exceed_counter                     = pInitParam->agc_dig_saturation_exceed_counter;
    pAd9361Param->agc_gain_update_interval_us                           = pInitParam->agc_gain_update_interval_us;
    pAd9361Param->agc_immed_gain_change_if_large_adc_overload_enable    = pInitParam->agc_immed_gain_change_if_large_adc_overload_enable;
    pAd9361Param->agc_immed_gain_change_if_large_lmt_overload_enable    = pInitParam->agc_immed_gain_change_if_large_lmt_overload_enable;
    pAd9361Param->agc_inner_thresh_high                                 = pInitParam->agc_inner_thresh_high;
    pAd9361Param->agc_inner_thresh_high_dec_steps                       = pInitParam->agc_inner_thresh_high_dec_steps;
    pAd9361Param->agc_inner_thresh_low                                  = pInitParam->agc_inner_thresh_low;
    pAd9361Param->agc_inner_thresh_low_inc_steps                        = pInitParam->agc_inner_thresh_low_inc_steps;
    pAd9361Param->agc_lmt_overload_large_exceed_counter                 = pInitParam->agc_lmt_overload_large_exceed_counter;
    pAd9361Param->agc_lmt_overload_large_inc_steps                      = pInitParam->agc_lmt_overload_large_inc_steps;
    pAd9361Param->agc_lmt_overload_small_exceed_counter                 = pInitParam->agc_lmt_overload_small_exceed_counter;
    pAd9361Param->agc_outer_thresh_high                                 = pInitParam->agc_outer_thresh_high;
    pAd9361Param->agc_outer_thresh_high_dec_steps                       = pInitParam->agc_outer_thresh_high_dec_steps;
    pAd9361Param->agc_outer_thresh_low                                  = pInitParam->agc_outer_thresh_low;
    pAd9361Param->agc_outer_thresh_low_inc_steps                        = pInitParam->agc_outer_thresh_low_inc_steps;
    pAd9361Param->agc_attack_delay_extra_margin_us                      = pInitParam->agc_attack_delay_extra_margin_us;
    pAd9361Param->agc_sync_for_gain_counter_enable                      = pInitParam->agc_sync_for_gain_counter_enable;
    pAd9361Param->fagc_dec_pow_measuremnt_duration                      = pInitParam->fagc_dec_pow_measuremnt_duration;
    pAd9361Param->fagc_state_wait_time_ns                               = pInitParam->fagc_state_wait_time_ns;
    pAd9361Param->fagc_allow_agc_gain_increase                          = pInitParam->fagc_allow_agc_gain_increase;
    pAd9361Param->fagc_lp_thresh_increment_time                         = pInitParam->fagc_lp_thresh_increment_time;
    pAd9361Param->fagc_lp_thresh_increment_steps                        = pInitParam->fagc_lp_thresh_increment_steps;
    pAd9361Param->fagc_lock_level                                       = pInitParam->fagc_lock_level;
    pAd9361Param->fagc_lock_level_lmt_gain_increase_en                  = pInitParam->fagc_lock_level_lmt_gain_increase_en;
    pAd9361Param->fagc_lock_level_gain_increase_upper_limit             = pInitParam->fagc_lock_level_gain_increase_upper_limit;
    pAd9361Param->fagc_lpf_final_settling_steps                         = pInitParam->fagc_lpf_final_settling_steps;
    pAd9361Param->fagc_lmt_final_settling_steps                         = pInitParam->fagc_lmt_final_settling_steps;
    pAd9361Param->fagc_final_overrange_count                            = pInitParam->fagc_final_overrange_count;
    pAd9361Param->fagc_gain_increase_after_gain_lock_en                 = pInitParam->fagc_gain_increase_after_gain_lock_en;
    pAd9361Param->fagc_gain_index_type_after_exit_rx_mode               = pInitParam->fagc_gain_index_type_after_exit_rx_mode;
    pAd9361Param->fagc_use_last_lock_level_for_set_gain_en              = pInitParam->fagc_use_last_lock_level_for_set_gain_en;
    pAd9361Param->fagc_rst_gla_stronger_sig_thresh_exceeded_en          = pInitParam->fagc_rst_gla_stronger_sig_thresh_exceeded_en;
    pAd9361Param->fagc_optimized_gain_offset                            = pInitParam->fagc_optimized_gain_offset;
    pAd9361Param->fagc_rst_gla_stronger_sig_thresh_above_ll             = pInitParam->fagc_rst_gla_stronger_sig_thresh_above_ll;
    pAd9361Param->fagc_rst_gla_engergy_lost_sig_thresh_exceeded_en      = pInitParam->fagc_rst_gla_engergy_lost_sig_thresh_exceeded_en;
    pAd9361Param->fagc_rst_gla_engergy_lost_goto_optim_gain_en          = pInitParam->fagc_rst_gla_engergy_lost_goto_optim_gain_en;
    pAd9361Param->fagc_rst_gla_engergy_lost_sig_thresh_below_ll         = pInitParam->fagc_rst_gla_engergy_lost_sig_thresh_below_ll;
    pAd9361Param->fagc_energy_lost_stronger_sig_gain_lock_exit_cnt      = pInitParam->fagc_energy_lost_stronger_sig_gain_lock_exit_cnt;
    pAd9361Param->fagc_rst_gla_large_adc_overload_en                    = pInitParam->fagc_rst_gla_large_adc_overload_en;
    pAd9361Param->fagc_rst_gla_large_lmt_overload_en                    = pInitParam->fagc_rst_gla_large_lmt_overload_en;
    pAd9361Param->fagc_rst_gla_en_agc_pulled_high_en                    = pInitParam->fagc_rst_gla_en_agc_pulled_high_en;
    pAd9361Param->fagc_rst_gla_if_en_agc_pulled_high_mode               = pInitParam->fagc_rst_gla_if_en_agc_pulled_high_mode;
    pAd9361Param->fagc_power_measurement_duration_in_state5             = pInitParam->fagc_power_measurement_duration_in_state5;
    pAd9361Param->rssi_delay                                            = pInitParam->rssi_delay;
    pAd9361Param->rssi_duration                                         = pInitParam->rssi_duration;
    pAd9361Param->rssi_restart_mode                                     = pInitParam->rssi_restart_mode;
    pAd9361Param->rssi_unit_is_rx_samples_enable                        = pInitParam->rssi_unit_is_rx_samples_enable;
    pAd9361Param->rssi_wait                                             = pInitParam->rssi_wait;
    pAd9361Param->temp_sense_decimation                                 = pInitParam->temp_sense_decimation;
    pAd9361Param->temp_sense_measurement_interval_ms                    = pInitParam->temp_sense_measurement_interval_ms;
    pAd9361Param->temp_sense_offset_signed                              = pInitParam->temp_sense_offset_signed;
    pAd9361Param->temp_sense_periodic_measurement_enable                = pInitParam->temp_sense_periodic_measurement_enable;
    pAd9361Param->ctrl_outs_enable_mask                                 = pInitParam->ctrl_outs_enable_mask;
    pAd9361Param->ctrl_outs_index                                       = pInitParam->ctrl_outs_index;

    
    return eRadio640Ok;
}

static Result _Radio640_GetOutputSyncLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    hRadio640->pCrRegs->stIOLowHigh.uUpdateOutputSyncLatch = 0;
    Carrier_usleep(hRadio640->hCarrier, 1);
    hRadio640->pCrRegs->stIOLowHigh.uUpdateOutputSyncLatch = 1;
    Carrier_usleep(hRadio640->hCarrier, 1);
    hRadio640->pCrRegs->stIOLowHigh.uUpdateOutputSyncLatch = 0;
    Carrier_usleep(hRadio640->hCarrier, 1);

    if(pu8LowValue) *pu8LowValue = hRadio640->pCrRegs->stIOLowHigh.uOutputSyncLow;
    if(pu8HighValue) *pu8HighValue = hRadio640->pCrRegs->stIOLowHigh.uOutputSyncHigh;

    return eRadio640Ok;
}

/******************************************************************************/
/*                           Public functions                                 */
/******************************************************************************/

Result Radio640_Open (Radio640_hRadio640 * phRadio640,
    uint32_t u32BaseAddr,
    Carrier_handle_t hCarrier)
{
    Result res;
	Radio640_hRadio640 hRadio640;
    Radio640DevCom_stI2cAddr I2cDevList;
    Carrier_pfUsleep pfUsleep;
    Carrier_eType_t CarrierType;
    int8_t *RxLanesDelays;
    int8_t *TxLanesDelays;
    
    int8_t NullRxLanesDelays[7] = {0, 0, 0, 0, 0, 0, 0};
    int8_t NullTxLanesDelays[7] = {0, 0, 0, 0, 0, 0, 0};

    if(((*(unsigned int *)u32BaseAddr) >> 16) != 0xF640)
        return eRadio640NoCore;

    pfUsleep = Carrier_GetUsleepFunc(hCarrier);
    if(pfUsleep == NULL) return -1;

    *phRadio640 = (Radio640_hRadio640)malloc(sizeof(Radio640_stRadio640));
    if (!*phRadio640) {
		return eRadio640OutOfMem;
	}
    memset(*phRadio640, 0, sizeof(Radio640_stRadio640));

    // Define a handle to easier understanding
    hRadio640 = *phRadio640;

    // Assign FPGA core register pointer
    hRadio640->pCrRegs = (Radio640_stCrRegisterMap *)(u32BaseAddr + RADIO640_CUSTOM_REG_BASE_ADDR);

    // Read FPGA core parameters
    hRadio640->FmcConnector = (Carrier_eFmcConnectorNum_t)(hRadio640->pCrRegs->stParameter.uFmcConnector - 1);
    hRadio640->BoardPosition = (Carrier_eFmcBoardNum_t)(hRadio640->pCrRegs->stParameter.uBoardPosition - 1);
    hRadio640->u8ClockMaster = hRadio640->pCrRegs->stParameter.uClockMaster;

    // Save Carrier information
    hRadio640->hCarrier = hCarrier;
    res = Carrier_FmcGetI2cZone(hRadio640->hCarrier, hRadio640->FmcConnector, &(hRadio640->Zone));
    if(FAILURE(res)) goto error;

    // Get I2C device address list
    res = _Radio640_GetI2cAddr(hRadio640, &I2cDevList);
    if(FAILURE(res)) goto error;

    // Open device communication object specific to the Radio640
    res = Radio640DevCom_Open(&(hRadio640->hComCore),
        hRadio640->hCarrier,
        hRadio640->Zone,
        (uint32_t)&(hRadio640->pCrRegs->u32TrxClkSpiReg),
        (uint32_t)&(hRadio640->pCrRegs->u32TrxClkSpiReadReg),
        (uint32_t)&(hRadio640->pCrRegs->u32TrxSpiClkDownsampling),
        (uint32_t)&(hRadio640->pCrRegs->u32DacOscSpiReg),
        (uint32_t)&(hRadio640->pCrRegs->u32RfAttSpiReg),
        I2cDevList,
        hRadio640->u8IsRevA);
    if(FAILURE(res)) goto error;

    // Create a generic device communication wrapper for the Radio640 device communication
    res = DevCom_Open(&(hRadio640->hCom), hRadio640->hComCore, Radio640DevCom_WriteArray, Radio640DevCom_ReadArray);
    if(FAILURE(res)) goto error;

    res = Carrier_GetType(hCarrier, &CarrierType);
    if(FAILURE(res)) goto error;
    
    if(CarrierType == Carrier_ePerseus611X)
    {
        RxLanesDelays = Perseus611xLanesDelays[2*(hRadio640->FmcConnector*2 + hRadio640->BoardPosition)];
        TxLanesDelays = Perseus611xLanesDelays[2*(hRadio640->FmcConnector*2 + hRadio640->BoardPosition) + 1];        
    }
    else if(CarrierType == Carrier_ePerseus601X)
    {
        RxLanesDelays = Perseus601xLanesDelays[2*hRadio640->BoardPosition];
        TxLanesDelays = Perseus601xLanesDelays[2*hRadio640->BoardPosition + 1];  
    }
    else
    {
        RxLanesDelays = NullRxLanesDelays;
        TxLanesDelays = NullTxLanesDelays;
    }
    
    // Open the AD9361 object
    res = ad9361_open(&(hRadio640->pAd9361Phy),
        u32BaseAddr + RADIO640_AD9361_REG_BASE_ADDR,
        hRadio640->hCom,
        (uint32_t)eRadio640DeviceTrx,
        pfUsleep,
        RxLanesDelays,
        TxLanesDelays);
    if(FAILURE(res)) goto error;

    // Open the temperature monitor module
    res = Lm75_Open(&(hRadio640->Lm75), hRadio640->hCom, (uint32_t)eRadio640DeviceTemp);
    if(FAILURE(res)) goto error;

    // Open power monitor modules
    res = Ina226_Open(&(hRadio640->hPowerMonitor12V), hRadio640->hCom, (uint32_t)eRadio640DevicePower12V, pfUsleep);
    if(FAILURE(res)) goto error;
    res = Ina226_Open(&(hRadio640->hPowerMonitorVadj), hRadio640->hCom, (uint32_t)eRadio640DevicePowerVadj, pfUsleep);
    if(FAILURE(res)) goto error;

	return eRadio640Ok;

error:

	Radio640_Close(phRadio640);
	debug("%s : RADIO640 open error\n", "Radio640_open");

	return res;
}

Result Radio640_Connect(Radio640_hRadio640 * phRadio640,
    hConnectionState_t hConnectionState,
    uint32_t u32BoardNum)
{
    return eRadio640HostFuncOnly;
}

Result Radio640_Presence(Radio640_hRadio640 hRadio640)
{
    return eRadio640HostFuncOnly;
}

Result Radio640_PowerUp(Radio640_hRadio640 hRadio640)
{
    return eRadio640HostFuncOnly;
}

void Radio640_Close(Radio640_hRadio640 * phRadio640)
{
	Radio640_hRadio640 hRadio640 = *phRadio640;

    if(hRadio640 != NULL)
    {
        Ina226_Close(&(hRadio640->hPowerMonitor12V));
        Ina226_Close(&(hRadio640->hPowerMonitorVadj));

        Lm75_Close(&(hRadio640->Lm75));

        if(hRadio640->pAd9361Phy) ad9361_close(hRadio640->pAd9361Phy);
        hRadio640->pAd9361Phy = NULL;

        DevCom_Close(&(hRadio640->hCom));

        Radio640DevCom_Close(&(hRadio640->hComCore));

        free(*phRadio640);
        *phRadio640 = NULL;
    }
}

Result Radio640_Write(Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device, uint16_t u16Addr, uint32_t u32Data)
{    
    return DevCom_Write(hRadio640->hCom, (uint32_t)Device, u16Addr, (void *)&u32Data);
}

Result Radio640_Read(Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device, uint16_t u16Addr, uint32_t * pu32Data)
{
    if(pu32Data == NULL) return eRadio640InvPtr;

    *pu32Data = 0;
    return DevCom_Read(hRadio640->hCom, (uint32_t)Device, u16Addr, (void *)pu32Data);
}

Result Radio640_SetTxExtAttenuation(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint32_t u32Attenuation)
{    
    Result res;
    uint8_t u8Value;
    
    if(u32Attenuation > 31750) return eRadio640ArgOutOfRange;
    
    // 250 mdB per bit
    u8Value = u32Attenuation/250;
    
    if((uint8_t)Channel & 0x1)
    {
        res = DevCom_Write(hRadio640->hCom, (uint32_t)eRadio640DeviceRfAtt1, 0, (void *)&u8Value);             
        if(FAILURE(res)) return res;                                                            
    }
    
    if((uint8_t)Channel & 0x2)
    {
        res = DevCom_Write(hRadio640->hCom, (uint32_t)eRadio640DeviceRfAtt2, 0, (void *)&u8Value);
        if(FAILURE(res)) return res;
    }
    
    return eRadio640Ok;
}

Result Radio640_IsClockMaster(Radio640_hRadio640 hRadio640, uint8_t * pu8ClockMaster)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(pu8ClockMaster == NULL) return eRadio640InvPtr;

    *pu8ClockMaster = hRadio640->u8ClockMaster;

	return eRadio640Ok;
}

/**
 * Initialize the AD9361 part.
 * @param pInitParam The structure that contains the Radio640 initial parameters.
 * @return A structure that contains the AD9361 current state in case of
 *         success, negative error code otherwise.
 */
Result Radio640_Init(Radio640_hRadio640 hRadio640, Radio640_stInitParam * pInitParam)
{
    Result res;

    Ad9361Api_stInitParam Ad9361Param;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(pInitParam == NULL) return eRadio640InvPtr;
     
    // Hardware reset the AD9361 chip with the reset pin
    hRadio640->pCrRegs->stIO.uAd9361ResetnPin = 0;

    // Reset the external temp monitor device
    res = Lm75_Reset(&(hRadio640->Lm75));
    if(FAILURE(res)) goto error;

    // Initialize the external temp monitor device to operating condition
    res = Lm75_Init(&(hRadio640->Lm75));
    if(FAILURE(res)) goto error;

    // Inittialize power monitor modules
    res = Ina226_Init(hRadio640->hPowerMonitor12V, 0.01f, 0.0002f);
    if(FAILURE(res)) goto error;
    res = Ina226_Init(hRadio640->hPowerMonitorVadj, 0.01f, 0.0002f);
    if(FAILURE(res)) goto error;
    // Start in power down mode
    res = Ina226_SetMode(hRadio640->hPowerMonitor12V, eIna226ModePowerDown, 0);
    if(FAILURE(res)) goto error;
    res = Ina226_SetMode(hRadio640->hPowerMonitorVadj, eIna226ModePowerDown, 0);
    if(FAILURE(res)) goto error;
    
    /*
     * Reset Radio640 controls and statuses to default.
     */
    res = Radio640_SetSyncOutputDelay(hRadio640, 0);
    if(FAILURE(res)) goto error;
    
    res = Radio640_SetSyncInputDelay(hRadio640, 0);
    if(FAILURE(res)) goto error;
    
    res = Radio640_SetRefClkDelay(hRadio640, 0);
    if(FAILURE(res)) goto error;
    
    if(hRadio640->u8IsRevA)
        res = Radio640_SetSyncBitslip(hRadio640, 1);
    else
        res = Radio640_SetSyncBitslip(hRadio640, 0);
    if(FAILURE(res)) goto error;
    
    res = Radio640_SetTriggerDelay(hRadio640, 0);
    if(FAILURE(res)) goto error;
    
    res = Radio640_SetTriggerBitslip(hRadio640, 0);
    if(FAILURE(res)) goto error;
    
    res = Radio640_SetSyncSrc(hRadio640, eRadio640SyncSrcSoft);
    if(FAILURE(res)) goto error;
    
    // Clear software controls of several GPIO
    res = Radio640_SetRfSwitchConfig(hRadio640, 0, 0);
    if(FAILURE(res)) goto error;
    res = Radio640_SetEnAgcPinConfig(hRadio640, 0, 0);
    if(FAILURE(res)) goto error;
    res = Radio640_SetEnablePinConfig(hRadio640, 0, 0);
    if(FAILURE(res)) goto error;
    res = Radio640_SetTxnrxPinConfig(hRadio640, 0, 0);
    if(FAILURE(res)) goto error;
    res = Radio640_SetCtrlInPinConfig(hRadio640, 0, 0);
    if(FAILURE(res)) goto error;
    
    // Clear calibration slave configuration
    res = ad9361_clear_master_calib_result(hRadio640->pAd9361Phy);
    if(FAILURE(res)) goto error;
    
    // Set default external attenuators to maximal attenuation
    res = Radio640_SetTxExtAttenuation(hRadio640, eRadio640BothChannels, 31750);
    if(FAILURE(res)) goto error;
    
    /*
     * Configure clocks
     */
    // Enable the clock to top by default if the board has an extended FMC connector
    // on top of it.
    hRadio640->u8ClkToExtendedFmcEnable = hRadio640->u8HasExtendedFmcConnector;
    
    // Force disable HMC7043.
    // It will be enabled by Radio640_SetClkSrc function.
    res = _Radio640_DisableHmc7043(hRadio640);
    if(FAILURE(res)) goto error;
    
    // Set clock source
    res = Radio640_SetClkSrc(hRadio640, pInitParam->ClkSrc);
    if(FAILURE(res)) goto error;
    
    // Set DAC VCXO to default value
    res = Radio640_TuneOsc(hRadio640, 0.0f);
    if(FAILURE(res)) goto error;
    
    /*
     * AD9361 Initialization
     */
     
    // CLear hardware reset the AD9361 chip with the reset pin
    hRadio640->pCrRegs->stIO.uAd9361ResetnPin = 1;
    Carrier_usleep(hRadio640->hCarrier, 10);

    // Create AD9361 Init param from Radio640 init param
    res = _Radio640_CreateAd9361Param(pInitParam, &Ad9361Param);
    if(FAILURE(res)) goto error;

    // Initialize AD9361
    res = ad9361_init(hRadio640->pAd9361Phy, &Ad9361Param);
    if(FAILURE(res)) goto error;

    // Output zeros to the DAC
    Ad9361DacCore_SetDataSel(hRadio640->pAd9361Phy, -1, DATA_SEL_ZERO);
	Ad9361DacCore_Sync(hRadio640->pAd9361Phy, 0);
    
    // Set AD9361 GPO in manual mode to control the RF bandpass filters
    res = ad9361_gpo_manual_mode_enable(hRadio640->pAd9361Phy, 1);
    if(FAILURE(res)) goto error;
    
    // Set RX RF Filter in bypass by default
	res = Radio640_SetRxRfFilter(hRadio640, eRadio640BothChannels, eRadio640RxRfFilterBypass);
	if(FAILURE(res)) goto error;
    
    // Clear the input signal latched values
    res = Radio640_GetCtrlOutPinLowHighValue(hRadio640, NULL, NULL);
	if(FAILURE(res)) goto error;
    res = Radio640_GetTriggerLowHighValue(hRadio640, NULL, NULL);
	if(FAILURE(res)) goto error;
    res = Radio640_GetInputSyncLowHighValue(hRadio640, NULL, NULL);
	if(FAILURE(res)) goto error;
    res = _Radio640_GetOutputSyncLowHighValue(hRadio640, NULL, NULL);
	if(FAILURE(res)) goto error;

	return eRadio640Ok;

error:
	debug("Radio640 initialization error : 0x%x\n", res);

	return res;
}

Result Radio640_DisableDesignClk(Radio640_hRadio640 hRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    ad9361_disable_design_clock(hRadio640->pAd9361Phy);
    
    return eRadio640Ok;
}

Result Radio640_EnableDesignClk(Radio640_hRadio640 hRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    ad9361_enable_design_clock(hRadio640->pAd9361Phy);
    
    return eRadio640Ok;
}

Result Radio640_SetRfSwitchConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8TrxInRxDir)
{
    const uint8_t RF_SWITCH_CONFIG_BIT_MASK = 0x3;
    const uint8_t CHANNEL1_BIT_POS = 0; 
    const uint8_t CHANNEL2_BIT_POS = 1;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(u8Force & (~RF_SWITCH_CONFIG_BIT_MASK)) return eRadio640ArgOutOfRange;

    // Set the desired value before forcing them to avoid glitches
    if(u8Force)
    {
        if((u8Force >> CHANNEL1_BIT_POS) & 0x1)
        {
            hRadio640->pCrRegs->stIO.uTrx1InRxDirValue = (u8TrxInRxDir >> CHANNEL1_BIT_POS) & 0x1;
        }
        
        if((u8Force >> CHANNEL2_BIT_POS) & 0x1)
        {
            hRadio640->pCrRegs->stIO.uTrx2InRxDirValue = (u8TrxInRxDir >> CHANNEL2_BIT_POS) & 0x1;
        }
        
        Carrier_usleep(hRadio640->hCarrier, 1);
    }

    // Enable force for specified bits
    hRadio640->pCrRegs->stIO.uTrx1InRxDirForce = ((u8Force >> CHANNEL1_BIT_POS) & 0x1);
    hRadio640->pCrRegs->stIO.uTrx2InRxDirForce = ((u8Force >> CHANNEL2_BIT_POS) & 0x1);
    
    return eRadio640Ok;
}

Result Radio640_SetEnAgcPinConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8Value)
{
    const uint8_t EN_AGC_BIT_MASK = 0x1;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(u8Force & (~EN_AGC_BIT_MASK)) return eRadio640ArgIsBoolean;

    // Set the desired value before forcing them to avoid glitches
    if(u8Force)
    {
        hRadio640->pCrRegs->stIO.uEnAgcValue = (u8Value & u8Force);
        Carrier_usleep(hRadio640->hCarrier, 1);
    }

    hRadio640->pCrRegs->stIO.uEnAgcForce = u8Force;

    return eRadio640Ok;
}

Result Radio640_SetEnablePinConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8Value)
{
    const uint8_t ENABLE_BIT_MASK = 0x1;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(u8Force & (~ENABLE_BIT_MASK)) return eRadio640ArgIsBoolean;

    // Set the desired value before forcing them to avoid glitches
    if(u8Force)
    {
        hRadio640->pCrRegs->stIO.uEnableValue = (u8Value & u8Force);
        Carrier_usleep(hRadio640->hCarrier, 1);
    }

    hRadio640->pCrRegs->stIO.uEnableForce = u8Force;

    return eRadio640Ok;
}

Result Radio640_SetTxnrxPinConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8Value)
{
    const uint8_t TXNRX_BIT_MASK = 0x1;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(u8Force & (~TXNRX_BIT_MASK)) return eRadio640ArgIsBoolean;

    // Set the desired value before forcing them to avoid glitches
    if(u8Force)
    {
        hRadio640->pCrRegs->stIO.uTxnrxValue = (u8Value & u8Force);
        Carrier_usleep(hRadio640->hCarrier, 1);
    }

    hRadio640->pCrRegs->stIO.uTxnrxForce = u8Force;

    return eRadio640Ok;
}

Result Radio640_SetCtrlInPinConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8Value)
{
    const uint8_t CTRLIN_BIT_MASK = 0xF;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(u8Force & (~CTRLIN_BIT_MASK)) return eRadio640ArgOutOfRange;
    
    // Disable force for specified bits before changing their values
    hRadio640->pCrRegs->stIO.uCtrlInForce &= u8Force;
    Carrier_usleep(hRadio640->hCarrier, 1);

    // Set the desired value before forcing them to avoid glitches
    hRadio640->pCrRegs->stIO.uCtrlInValue = (u8Value & u8Force);
    Carrier_usleep(hRadio640->hCarrier, 1);

    // Enable force for specified bits
    hRadio640->pCrRegs->stIO.uCtrlInForce = u8Force;

    return eRadio640Ok;
}

Result Radio640_GetCtrlOutPinValue(Radio640_hRadio640 hRadio640, uint8_t * pu8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(pu8Value == NULL) return eRadio640InvPtr;

    *pu8Value = hRadio640->pCrRegs->stCtrlOut.uCtrlOutCurrent;

    return eRadio640Ok;
}

Result Radio640_GetCtrlOutPinLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    hRadio640->pCrRegs->stCtrlOut.uUpdateCtrlOutLatch = 0;
    Carrier_usleep(hRadio640->hCarrier, 1);
    hRadio640->pCrRegs->stCtrlOut.uUpdateCtrlOutLatch = 1;
    Carrier_usleep(hRadio640->hCarrier, 1);
    hRadio640->pCrRegs->stCtrlOut.uUpdateCtrlOutLatch = 0;
    Carrier_usleep(hRadio640->hCarrier, 1);

    if(pu8LowValue) *pu8LowValue = hRadio640->pCrRegs->stCtrlOut.uCtrlOutLow;
    if(pu8HighValue) *pu8HighValue = hRadio640->pCrRegs->stCtrlOut.uCtrlOutHigh;

    return eRadio640Ok;
}

Result Radio640_SetCtrlOutConfig(Radio640_hRadio640 hRadio640, uint8_t u8Config, uint8_t u8Enable)
{
    Result res;
    struct ctrl_outs_control CtrlOutControl;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(u8Config > 0x1F) return eRadio640ArgOutOfRange;

    CtrlOutControl.index = u8Config;
    CtrlOutControl.en_mask = u8Enable;

    res = ad9361_ctrl_outs_setup(hRadio640->pAd9361Phy, &CtrlOutControl);
    if(FAILURE(res)) return res;

    return eRadio640Ok;
}

Result Radio640_GetTriggerValue(Radio640_hRadio640 hRadio640, uint8_t * pu8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(pu8Value == NULL) return eRadio640InvPtr;

    *pu8Value = hRadio640->pCrRegs->stIOLowHigh.uTrigger;

    return eRadio640Ok;
}

Result Radio640_GetTriggerLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    hRadio640->pCrRegs->stIOLowHigh.uUpdateTriggerLatch = 0;
    Carrier_usleep(hRadio640->hCarrier, 1);
    hRadio640->pCrRegs->stIOLowHigh.uUpdateTriggerLatch = 1;
    Carrier_usleep(hRadio640->hCarrier, 1);
    hRadio640->pCrRegs->stIOLowHigh.uUpdateTriggerLatch = 0;
    Carrier_usleep(hRadio640->hCarrier, 1);

    if(pu8LowValue) *pu8LowValue = hRadio640->pCrRegs->stIOLowHigh.uTriggerLow;
    if(pu8HighValue) *pu8HighValue = hRadio640->pCrRegs->stIOLowHigh.uTriggerHigh;

    return eRadio640Ok;
}

Result Radio640_GetInputSyncValue(Radio640_hRadio640 hRadio640, uint8_t * pu8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(pu8Value == NULL) return eRadio640InvPtr;

    *pu8Value = hRadio640->pCrRegs->stIOLowHigh.uInputSync;

    return eRadio640Ok;
}

Result Radio640_GetInputSyncLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    hRadio640->pCrRegs->stIOLowHigh.uUpdateInputSyncLatch = 0;
    Carrier_usleep(hRadio640->hCarrier, 1);
    hRadio640->pCrRegs->stIOLowHigh.uUpdateInputSyncLatch = 1;
    Carrier_usleep(hRadio640->hCarrier, 1);
    hRadio640->pCrRegs->stIOLowHigh.uUpdateInputSyncLatch = 0;
    Carrier_usleep(hRadio640->hCarrier, 1);

    if(pu8LowValue) *pu8LowValue = hRadio640->pCrRegs->stIOLowHigh.uInputSyncLow;
    if(pu8HighValue) *pu8HighValue = hRadio640->pCrRegs->stIOLowHigh.uInputSyncHigh;

    return eRadio640Ok;
}

Result Radio640_SetPowerMonitorMode(Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eMode Mode, uint8_t u8IsBlocking)
{
    Result res;
    Ina226_hIna226 hIna226;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    switch(Device)
    {
        case eRadio640DevicePower12V:
            hIna226 = hRadio640->hPowerMonitor12V;
            break;
        case eRadio640DevicePowerVadj:
            hIna226 = hRadio640->hPowerMonitorVadj;
            break;
        default:
            return eRadio640InvDevice;
    }

    res = Ina226_SetMode(hIna226, Mode, u8IsBlocking);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_SetPowerMonitorConversionTime(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eConvTime BusConvTime, Ina226_eConvTime ShuntConvTime)
{
    Result res;
    Ina226_hIna226 hIna226;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    switch(Device)
    {
        case eRadio640DevicePower12V:
            hIna226 = hRadio640->hPowerMonitor12V;
            break;
        case eRadio640DevicePowerVadj:
            hIna226 = hRadio640->hPowerMonitorVadj;
            break;
        default:
            return eRadio640InvDevice;
    }

    res = Ina226_SetConversionTime(hIna226, BusConvTime, ShuntConvTime);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_SetPowerMonitorAveraging(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eAveraging Averaging)
{
    Result res;
    Ina226_hIna226 hIna226;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    switch(Device)
    {
        case eRadio640DevicePower12V:
            hIna226 = hRadio640->hPowerMonitor12V;
            break;
        case eRadio640DevicePowerVadj:
            hIna226 = hRadio640->hPowerMonitorVadj;
            break;
        default:
            return eRadio640InvDevice;
    }

    res = Ina226_SetAveraging(hIna226, Averaging);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_ReadPowerMonitorBusVoltage(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfVoltage)
{
    Result res;
    Ina226_hIna226 hIna226;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    switch(Device)
    {
        case eRadio640DevicePower12V:
            hIna226 = hRadio640->hPowerMonitor12V;
            break;
        case eRadio640DevicePowerVadj:
            hIna226 = hRadio640->hPowerMonitorVadj;
            break;
        default:
            return eRadio640InvDevice;
    }

    res = Ina226_ReadBusVoltage(hIna226, pfVoltage);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_ReadPowerMonitorPower(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfPower)
{
    Result res;
    Ina226_hIna226 hIna226;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    switch(Device)
    {
        case eRadio640DevicePower12V:
            hIna226 = hRadio640->hPowerMonitor12V;
            break;
        case eRadio640DevicePowerVadj:
            hIna226 = hRadio640->hPowerMonitorVadj;
            break;
        default:
            return eRadio640InvDevice;
    }

    res = Ina226_ReadPower(hIna226, pfPower);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_ReadPowerMonitorCurrent(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfCurrent)
{
    Result res;
    Ina226_hIna226 hIna226;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    switch(Device)
    {
        case eRadio640DevicePower12V:
            hIna226 = hRadio640->hPowerMonitor12V;
            break;
        case eRadio640DevicePowerVadj:
            hIna226 = hRadio640->hPowerMonitorVadj;
            break;
        default:
            return eRadio640InvDevice;
    }

    res = Ina226_ReadCurrent(hIna226, pfCurrent);
    if(FAILURE(res)) return res;
    
    // Vadj power monitor Vin inputs are swapped
    // Correct it, to provide a positive current value
    if(Device == eRadio640DevicePowerVadj) *pfCurrent *= -1.0f;
    
    return eRadio640Ok;
}

Result Radio640_GetBoardTemp(Radio640_hRadio640 hRadio640, int16_t *pi16Temp)
{
    Result res;

    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(pi16Temp == NULL) return eRadio640InvPtr;

    // Read pcb temperature in 0.1C format
    res = Lm75_ReadTemp10X(&(hRadio640->Lm75), pi16Temp);
    if(FAILURE(res)) return res;

    // Rounding to Celsius
    *pi16Temp = *pi16Temp / 10;

    return eRadio640Ok;
}

Result Radio640_GetTrxTemp(Radio640_hRadio640 hRadio640, int16_t *pi16Temp)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(pi16Temp == NULL) return eRadio640InvPtr;

    *pi16Temp = (ad9361_get_temp(hRadio640->pAd9361Phy) / 1000);

    return eRadio640Ok;
}

Result Radio640_TuneOsc(Radio640_hRadio640 hRadio640, float fValue)
{
    Result res;
    uint16_t u16Value;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(fValue < -1.0f || fValue > 1.0f) return eRadio640ArgOutOfRange;
    
    u16Value = (uint16_t)((fValue + 1.5f)/ 2.5f * 65535.0f);

    res = DevCom_Write(hRadio640->hCom, (uint32_t)eRadio640DeviceDacOsc, 0, &u16Value);
    if(FAILURE(res)) return res;

    return eRadio640Ok;
}

Result Radio640_SetClkSrc(Radio640_hRadio640 hRadio640, Radio640_eClkSrc ClkSrc)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(ClkSrc == eRadio640ClkSrcMaster && hRadio640->BoardPosition == Carrier_eFmcBoard0)
    {
        return eRadio640InvClkSrcMaster;
    }
    
    if(ClkSrc == eRadio640ClkSrcCarrier && hRadio640->BoardPosition != Carrier_eFmcBoard0)
    {
        return eRadio640InvClkSrcCarrier;
    }

    // Clock switches
    switch(ClkSrc)
    {
        case eRadio640ClkSrcInt:
        case eRadio640ClkSrcExt:
        case eRadio640ClkSrcCarrier:
            hRadio640->pCrRegs->stIO.uClkSwitch1 = (uint8_t)ClkSrc; // Select input
            hRadio640->pCrRegs->stIO.uClkSwitch2 = 1;
            
            break;

        case eRadio640ClkSrcMaster:
            hRadio640->pCrRegs->stIO.uClkSwitch1 = (uint8_t)0x3;    // Disable
            hRadio640->pCrRegs->stIO.uClkSwitch2 = 0;
            break;

        default:
            return eRadio640InvEnumValue;
    }

    // Enable/disable local oscillator
    if(ClkSrc == eRadio640ClkSrcInt)
        hRadio640->pCrRegs->stIO.uOscEnPin = 1;
    else
        hRadio640->pCrRegs->stIO.uOscEnPin = 0;
    
    if(ClkSrc != eRadio640ClkSrcMaster)
    {
        // If the HMC7043 is currently in reset state,
        // we need to deassert the reset and configure
        // its internal register.
        if(hRadio640->pCrRegs->stIO.uClkResetPin == 1)
        {
            res = _Radio640_InitHmc7043(hRadio640);
            if(FAILURE(res)) return res;
        }
        
        // Restore u8RefOutEnable state
        res = Radio640_SetRefOutEnable(hRadio640, hRadio640->u8RefOutEnable);
        if(FAILURE(res)) return res;
        
        // Restore u8ClkToExtendedFmcEnable state
        res = Radio640_SetClkToExtendedFmcEnable(hRadio640, hRadio640->u8ClkToExtendedFmcEnable);
        if(FAILURE(res)) return res;
    }
    else
    {
        // Disable HMC7043 since no valid input clock is provided
        
        res = _Radio640_DisableHmc7043(hRadio640);
        if(FAILURE(res)) return res;
    }

	return eRadio640Ok;
}

Result Radio640_GetClkSrc(Radio640_hRadio640 hRadio640, Radio640_eClkSrc * pClkSrc)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;

    if(pClkSrc == NULL) return eRadio640InvPtr;

    if(hRadio640->pCrRegs->stIO.uClkSwitch2 == 0)
    {
        *pClkSrc = eRadio640ClkSrcMaster;
        return eRadio640Ok;
    }

    if(hRadio640->pCrRegs->stIO.uClkSwitch1 >= 0x3)
    {
        *pClkSrc = eRadio640ClkSrcUnknown;
        return eRadio640Ok;
    }

    *pClkSrc = (Radio640_eClkSrc)hRadio640->pCrRegs->stIO.uClkSwitch1;
	return eRadio640Ok;
}

Result Radio640_DigitalCalibration(Radio640_hRadio640 hRadio640)
{
    Result res;
    uint8_t clockmaster = 0;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    res = Radio640_IsClockMaster(hRadio640, &clockmaster);
    if(FAILURE(res)) return res;
    
    res = ad9361_post_setup(hRadio640->pAd9361Phy, clockmaster);
    if(FAILURE(res)) return res;

    Ad9361DacCore_SetDataSel(hRadio640->pAd9361Phy, -1, DATA_SEL_USER);
	Ad9361DacCore_Sync(hRadio640->pAd9361Phy, 0);
    if(FAILURE(res)) return res;

	return eRadio640Ok;
}

Result Radio640_SetSyncOutputDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;
    uint32_t reg;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;

    reg = Ad9361AdcCore_ReadValue(phy, REG_SYNC_DELAY);

    // Clear target bits
    reg &= (~SYNC_ODELAY_OFFSET(0xFFFFFFFF));
    // Set desired bits
	reg |= SYNC_ODELAY_OFFSET(i32Delay);

    Ad9361AdcCore_Write(phy, REG_SYNC_DELAY, reg);

    return eRadio640Ok;
}

Result Radio640_SetSyncInputDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;
    uint32_t reg;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;

    reg = Ad9361AdcCore_ReadValue(phy, REG_SYNC_DELAY);

    // Clear target bits
    reg &= (~SYNC_IDELAY_OFFSET(0xFFFFFFFF));
    // Set desired bits
	reg |= SYNC_IDELAY_OFFSET(i32Delay);

    Ad9361AdcCore_Write(phy, REG_SYNC_DELAY, reg);

    return eRadio640Ok;
}

Result Radio640_SetRefClkDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;
    uint32_t reg;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;

    reg = Ad9361AdcCore_ReadValue(phy, REG_SYNC_DELAY);

    // Clear target bits
    reg &= (~REFCLK_IODELAY_OFFSET(0xFFFFFFFF));
    // Set desired bits
	reg |= REFCLK_IODELAY_OFFSET(i32Delay);

    Ad9361AdcCore_Write(phy, REG_SYNC_DELAY, reg);

    return eRadio640Ok;
}

Result Radio640_SetSyncBitslip(Radio640_hRadio640 hRadio640, int32_t i32Bitslip)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;
    uint32_t reg;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;

    reg = Ad9361AdcCore_ReadValue(phy, REG_SYNC_SEL);

    // Clear bit
    reg &= (~SYNC_BIT_SLIP);

    if(i32Bitslip)
        reg |= SYNC_BIT_SLIP;

    Ad9361AdcCore_Write(phy, REG_SYNC_SEL, reg);

    return eRadio640Ok;
}

Result Radio640_SetSyncSrc(Radio640_hRadio640 hRadio640, Radio640_eSyncSrc Src)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;
    uint32_t reg;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;

    reg = Ad9361AdcCore_ReadValue(phy, REG_SYNC_SEL);

    // Clear target bits
    reg &= (~SYNC_SEL_OFFSET(0xFFFFFFFF));
    // Set desired bits
	reg |= SYNC_SEL_OFFSET((int32_t)Src);

    Ad9361AdcCore_Write(phy, REG_SYNC_SEL, reg);

    return eRadio640Ok;
}

Result Radio640_SoftSync(Radio640_hRadio640 hRadio640)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;

	Ad9361AdcCore_Write(phy, REG_SOFT_SYNC, 0);
    phy->pfUsleep(10);

	Ad9361AdcCore_Write(phy, REG_SOFT_SYNC, SOFT_SYNC);
    phy->pfUsleep(10);

	Ad9361AdcCore_Write(phy, REG_SOFT_SYNC, 0);

    return eRadio640Ok;
}

Result Radio640_SetComplexMultiplier(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint16_t u16RealCoefficient, uint16_t u16ImgCoefficient)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;

    //Rx
    if(u8Direction == 0)
        Ad9361AdcCore_Write(phy, REG_ADC_COEFFICIENT_CH0 + 4*(u8Channel - 1), ((((uint32_t)u16ImgCoefficient << 16) & 0xFFFF0000) | ((uint32_t)u16RealCoefficient & 0x0000FFFF)));
    //Tx
    else if(u8Direction == 1)  
        Ad9361DacCore_Write(phy, REG_DAC_COEFFICIENT_CH0 + 4*(u8Channel - 1), ((((uint32_t)u16ImgCoefficient << 16) & 0xFFFF0000) | ((uint32_t)u16RealCoefficient & 0x0000FFFF)));
    else  
        return eRadio640ArgOutOfRange;

    return eRadio640Ok;
}

Result Radio640_GetComplexMultiplier(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint16_t *u16RealCoefficient, uint16_t *u16ImgCoefficient)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;
    uint32_t reg;
    
    //Rx
    if(u8Direction == 0)
    {
        reg = Ad9361AdcCore_ReadValue(phy, REG_ADC_COEFFICIENT_CH0 + 4*(u8Channel - 1));
        *u16RealCoefficient = (uint16_t)(reg & 0x0000FFFF);
        *u16ImgCoefficient = (uint16_t)(reg & 0xFFFF0000);
    }
    //Tx
    else if(u8Direction == 1)  
    {
        reg = Ad9361DacCore_ReadValue(phy, REG_DAC_COEFFICIENT_CH0 + 4*(u8Channel - 1));
        *u16RealCoefficient = (uint16_t)(reg & 0x0000FFFF);
        *u16ImgCoefficient = (uint16_t)(reg & 0xFFFF0000);
    }
    else  
        return eRadio640ArgOutOfRange;

    return eRadio640Ok;
}

Result Radio640_SetDataDelay(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint32_t u32DataDelay)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;

    //Rx
    if(u8Direction == 0)
        Ad9361AdcCore_Write(phy, REG_ADC_DATA_DELAY_CH0 + 4*(u8Channel - 1), u32DataDelay);
    //Tx
    else if(u8Direction == 1)  
        Ad9361DacCore_Write(phy, REG_DAC_DATA_DELAY_CH0 + 4*(u8Channel - 1), u32DataDelay);
    else  
        return eRadio640ArgOutOfRange;

    return eRadio640Ok;
}

Result Radio640_GetDataDelay(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint32_t *u32DataDelay)
{
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;
    
    //Rx
    if(u8Direction == 0)
    {
        *u32DataDelay = Ad9361AdcCore_ReadValue(phy, REG_ADC_DATA_DELAY_CH0 + 4*(u8Channel - 1));
    }
    //Tx
    else if(u8Direction == 1)  
        *u32DataDelay = Ad9361DacCore_ReadValue(phy, REG_DAC_DATA_DELAY_CH0 + 4*(u8Channel - 1));
    else  
        return eRadio640ArgOutOfRange;

    return eRadio640Ok;
}

Result Radio640_SetTriggerDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(i32Delay < 0 || i32Delay > 31) return eRadio640ArgOutOfRange;

    hRadio640->pCrRegs->stIO.uTriggerDelay = i32Delay;
    
    return eRadio640Ok;
}

Result Radio640_SetTriggerBitslip(Radio640_hRadio640 hRadio640, int32_t i32Bitslip)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(i32Bitslip < 0 || i32Bitslip > 1) return eRadio640ArgOutOfRange;

    hRadio640->pCrRegs->stIO.uTriggerBitSlip = i32Bitslip;
    
    return eRadio640Ok;
}

Result Radio640_Mcs(Radio640_hRadio640 hRadio640, int32_t step)
{
    Result res;
    struct Ad9361_stPhy *phy = hRadio640->pAd9361Phy;
    uint8_t u8LowValue, u8HighValue;
    
    // Read value to know if a Sync signal has been sent to the AD9361 chip
    res = _Radio640_GetOutputSyncLowHighValue(hRadio640, &u8LowValue, &u8HighValue);
    if(FAILURE(res)) return res;
    
    // Verify that the sync had a rising-edge event since the last MCS step
    if((step == 3 || step == 4) && (u8LowValue == 1 || u8HighValue == 0)) 
        return eRadio640NoSyncSent;
    
    res = ad9361_mcs(phy, step);
    if(FAILURE(res)) return res;
    
    // Read value to clear previous one (before the MCS)
    res = _Radio640_GetOutputSyncLowHighValue(hRadio640, &u8LowValue, &u8HighValue);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_SetMasterCalibResult(Radio640_hRadio640 hRadio640, Radio640_hRadio640 hMasterRadio640)
{
    return ad9361_set_master_calib_result(hRadio640->pAd9361Phy, hMasterRadio640->pAd9361Phy);
}

Result Radio640_SetExtMasterCalibResult(Radio640_hRadio640 hRadio640, Ad9361_stCalibResult ExtCalibResult)
{
    return ad9361_set_ext_master_calib_result(hRadio640->pAd9361Phy, ExtCalibResult);
}

Result Radio640_GetCalibResult(Radio640_hRadio640 hRadio640, Ad9361_stCalibResult * pCalibResult)
{
    *pCalibResult = hRadio640->pAd9361Phy->CalibResult;
    return eRadio640Ok;
}

/**
 * Set the Enable State Machine (ENSM) mode.
 * @param phy The AD9361 current state structure.
 * @param mode The ENSM mode (SLEEP, ALERT, FDD, PINCTRL).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetEnsmMode (Radio640_hRadio640 hRadio640, Ad9361_eEnsmMode Mode)
{
	int32_t ret;

	ret = ad9361_set_en_state_machine_mode (hRadio640->pAd9361Phy,
										  Mode);
	return ret;
}

/**
 * Get the Enable State Machine (ENSM) mode.
 * @param phy The AD9361 current state structure.
 * @param pu32Mode A variable to store the selected ENSM mode.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetEnsmMode (Radio640_hRadio640 hRadio640, Ad9361_eEnsmMode *pMode)
{
	int32_t ret;

	ret = ad9361_get_en_state_machine_mode (hRadio640->pAd9361Phy,
										  pMode);

	return ret;
}

Result Radio640_GetEnsmState (Radio640_hRadio640 hRadio640, Ad9361_eEnsmState * pEnsmState)
{
    ad9361_ensm_get_state(hRadio640->pAd9361Phy, pEnsmState);
    
	return eRadio640Ok;
}

/**
 * Set the receive RF gain for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param u8Channel The desired channel number (1, 2).
 * @param gain_db The RF gain (dB).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxRfGain (Radio640_hRadio640 hRadio640, uint8_t u8Channel, int32_t i32Gain)
{
    int32_t ret;

	ret = ad9361_set_rx_rf_gain (hRadio640->pAd9361Phy,
										  u8Channel, i32Gain);

	return ret;
}

/**
 * Get current receive RF gain for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param u8Channel The desired channel number (1, 2).
 * @param gain_db A variable to store the RF gain (dB).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxRfGain (Radio640_hRadio640 hRadio640,uint8_t u8Channel, int32_t * pi32Gain)
{
    int32_t ret;

	ret = ad9361_get_rx_rf_gain (hRadio640->pAd9361Phy,
										  u8Channel, pi32Gain);

	return ret;
}

/**
 * Set the RX RF bandwidth.
 * @param phy The AD9361 current state structure.
 * @param u32Bandwidth The desired bandwidth (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxRfBandwidth (Radio640_hRadio640 hRadio640, uint32_t u32Bandwidth)
{
    int32_t ret;

	ret = ad9361_set_rx_rf_bandwidth (hRadio640->pAd9361Phy,
										  u32Bandwidth);

	return ret;
}

/**
 * Get the RX RF bandwidth.
 * @param phy The AD9361 current state structure.
 * @param pu32Bandwidth A variable to store the bandwidth value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxRfBandwidth (Radio640_hRadio640 hRadio640, uint32_t *pu32Bandwidth)
{
    int32_t ret;

	ret = ad9361_get_rx_rf_bandwidth (hRadio640->pAd9361Phy,
										  pu32Bandwidth);

	return ret;
}

/**
 * Set the TX and RX sampling frequency.
 * @param phy The AD9361 current state structure.
 * @param u32SamplingFreq The desired frequency (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetSamplingFreq (Radio640_hRadio640 hRadio640, uint32_t u32SamplingFreq)
{
    int32_t ret;

    // ad9361_set_rx_sampling_freq and ad9361_set_tx_sampling_freq functions
    // do exactly the same thing
	ret = ad9361_set_rx_sampling_freq (hRadio640->pAd9361Phy,
										  u32SamplingFreq);
    if(FAILURE(ret))
        return ret;

	return ret;
}

/**
 * Get current TX and RX sampling frequency.
 * @param phy The AD9361 current state structure.
 * @param pu32SamplingFreq A variable to store the frequency value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetSamplingFreq (Radio640_hRadio640 hRadio640, uint32_t * pu32SamplingFreq)
{
    int32_t ret;

    // ad9361_get_rx_sampling_freq and ad9361_get_tx_sampling_freq functions
    // do exactly the same thing
	ret = ad9361_get_rx_sampling_freq (hRadio640->pAd9361Phy,
										  pu32SamplingFreq);

	return ret;
}

/**
 * Set the RX LO frequency.
 * @param phy The AD9361 current state structure.
 * @param u64LoFreq The desired frequency (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxLoFreq (Radio640_hRadio640 hRadio640, uint64_t u64LoFreq)
{
    int32_t ret;

	ret = ad9361_set_rx_lo_freq (hRadio640->pAd9361Phy,
										  u64LoFreq);

	return ret;
}

/**
 * Get current RX LO frequency.
 * @param phy The AD9361 current state structure.
 * @param pu64LoFreq A variable to store the frequency value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxLoFreq (Radio640_hRadio640 hRadio640, uint64_t *pu64LoFreq)
{
    int32_t ret;

	ret = ad9361_get_rx_lo_freq (hRadio640->pAd9361Phy,
										  pu64LoFreq);

	return ret;
}

/**
 * Get the RSSI for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param u8Channel The desired channel (1, 2).
 * @param pRfRssi A variable to store the RSSI.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxRssi (Radio640_hRadio640 hRadio640, uint8_t u8Channel, struct Ad9361_stRfRssi *pRfRssi)
{
    int32_t ret;

	ret = ad9361_get_rx_rssi (hRadio640->pAd9361Phy, u8Channel, pRfRssi);

	return ret;
}

/**
 * Set the gain control mode for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param u8Channel The desired channel (1, 2).
 * @param u8GcMode The gain control mode (GAIN_MGC, GAIN_FASTATTACK_AGC,
 *                GAIN_SLOWATTACK_AGC, GAIN_HYBRID_AGC).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxGainControlMode (Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8GcMode)
{
    int32_t ret;

	ret = ad9361_set_rx_gain_control_mode (hRadio640->pAd9361Phy,
										  u8Channel, u8GcMode);

	return ret;
}

/**
 * Get the gain control mode for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param u8Channel The desired channel (1, 2).
 * @param pu8GcMode A variable to store the gain control mode.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxGainControlMode (Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t * pu8GcMode)
{
    int32_t ret;

	ret = ad9361_get_rx_gain_control_mode (hRadio640->pAd9361Phy,
										  u8Channel, pu8GcMode);

	return ret;
}

/**
 * Set the RX FIR filter configuration.
 * @param phy The AD9361 current state structure.
 * @param Config FIR filter configuration.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxFirConfig (Radio640_hRadio640 hRadio640, Ad9361Api_stRxFirConfig Config)
{
    int32_t ret;

	ret = ad9361_set_rx_fir_config (hRadio640->pAd9361Phy, Config);

	return ret;
}

/**
 * Get the RX FIR filter configuration.
 * @param phy The AD9361 current state structure.
 * @param tx_ch The selected RX channel (1, 2).
 * @param fir_cfg FIR filter configuration output file.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxFirConfig(Radio640_hRadio640 hRadio640, uint8_t u8Channel, Ad9361Api_stRxFirConfig * pConfig)
{
    int32_t ret;

	ret = ad9361_get_rx_fir_config (hRadio640->pAd9361Phy,
			u8Channel, pConfig);

	return ret;
}

/**
 * Enable/disable the RX FIR filter.
 * @param phy The AD9361 current state structure.
 * @param u8Enable The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxFirEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    int32_t ret;

	ret = ad9361_set_rx_fir_en_dis (hRadio640->pAd9361Phy, u8Enable);

	return ret;
}

/**
 * Get the status of the RX FIR filter.
 * @param phy The AD9361 current state structure.
 * @param pu8Enable The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxFirEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    int32_t ret;

	ret = ad9361_get_rx_fir_en_dis (hRadio640->pAd9361Phy,
										  pu8Enable);

	return ret;
}

/**
 * Enable/disable the RX RFDC Tracking.
 * @param phy The AD9361 current state structure.
 * @param u8Enable The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxRfdcTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    int32_t ret;

	ret = ad9361_set_rx_rfdc_track_en_dis (hRadio640->pAd9361Phy,
										  u8Enable);

	return ret;
}

/**
 * Get the status of the RX RFDC Tracking.
 * @param phy The AD9361 current state structure.
 * @param pu8Enable The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxRfdcTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    int32_t ret;

	ret = ad9361_get_rx_rfdc_track_en_dis (hRadio640->pAd9361Phy,
										  pu8Enable);

	return ret;
}

/**
 * Enable/disable the RX BasebandDC Tracking.
 * @param phy The AD9361 current state structure.
 * @param u8Enable The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxBbdcTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    int32_t ret;

	ret = ad9361_set_rx_bbdc_track_en_dis (hRadio640->pAd9361Phy,
										  u8Enable);

	return ret;
}

/**
 * Get the status of the RX BasebandDC Tracking.
 * @param phy The AD9361 current state structure.
 * @param pu8Enable The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxBbdcTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    int32_t ret;

	ret = ad9361_get_rx_bbdc_track_en_dis (hRadio640->pAd9361Phy,
										  pu8Enable);

	return ret;
}

/**
 * Enable/disable the RX Quadrature Tracking.
 * @param phy The AD9361 current state structure.
 * @param u8Enable The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetRxQuadTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    int32_t ret;

	ret = ad9361_set_rx_quad_track_en_dis (hRadio640->pAd9361Phy,
										  u8Enable);

	return ret;
}

/**
 * Get the status of the RX Quadrature Tracking.
 * @param phy The AD9361 current state structure.
 * @param pu8Enable The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetRxQuadTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    int32_t ret;

	ret = ad9361_get_rx_quad_track_en_dis (hRadio640->pAd9361Phy,
										  pu8Enable);

	return ret;
}

/**
 * Set the transmit attenuation for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param u8Channel The desired channel number (1, 2).
 * @param attenuation_mdb The attenuation (mdB).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetTxAttenuation (Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint32_t u32Attenuation)
{
    int32_t ret;

	ret = ad9361_set_tx_attenuation (hRadio640->pAd9361Phy, u8Channel, u32Attenuation);

	return ret;
}

/**
 * Get current transmit attenuation for the selected channel.
 * @param phy The AD9361 current state structure.
 * @param u8Channel The desired channel number (1, 2).
 * @param attenuation_mdb A variable to store the attenuation value (mdB).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetTxAttenuation (Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint32_t * pu32Attenuation)
{
    int32_t ret;

	ret = ad9361_get_tx_attenuation (hRadio640->pAd9361Phy,
										  u8Channel, pu32Attenuation);

	return ret;
}

/**
 * Set the TX RF bandwidth.
 * @param phy The AD9361 current state structure.
 * @param bandwidth_hz The desired bandwidth (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetTxRfBandwidth (Radio640_hRadio640 hRadio640, uint32_t u32Bandwidth)
{
    int32_t ret;

	ret = ad9361_set_tx_rf_bandwidth (hRadio640->pAd9361Phy,
										  u32Bandwidth);

	return ret;
}

/**
 * Get the TX RF bandwidth.
 * @param phy The AD9361 current state structure.
 * @param bandwidth_hz A variable to store the bandwidth value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetTxRfBandwidth (Radio640_hRadio640 hRadio640, uint32_t * pu32Bandwidth)
{
    int32_t ret;

	ret = ad9361_get_tx_rf_bandwidth (hRadio640->pAd9361Phy,
										  pu32Bandwidth);

	return ret;
}

/**
 * Set the TX LO frequency.
 * @param phy The AD9361 current state structure.
 * @param lo_freq_hz The desired frequency (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetTxLoFreq (Radio640_hRadio640 hRadio640, uint64_t u64LoFreq)
{
    int32_t ret;

	ret = ad9361_set_tx_lo_freq (hRadio640->pAd9361Phy, u64LoFreq);

	return ret;
}

/**
 * Get current TX LO frequency.
 * @param phy The AD9361 current state structure.
 * @param lo_freq_hz A variable to store the frequency value (Hz).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetTxLoFreq (Radio640_hRadio640 hRadio640, uint64_t * pu64LoFreq)
{
    int32_t ret;

	ret = ad9361_get_tx_lo_freq (hRadio640->pAd9361Phy, pu64LoFreq);

	return ret;
}

/**
 * Set the TX FIR filter configuration.
 * @param phy The AD9361 current state structure.
 * @param fir_cfg FIR filter configuration.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetTxFirConfig (Radio640_hRadio640 hRadio640, Ad9361Api_stTxFirConfig Config)
{
    int32_t ret;

	ret = ad9361_set_tx_fir_config (hRadio640->pAd9361Phy,
										  Config);

	return ret;
}

/**
 * Get the TX FIR filter configuration.
 * @param phy The AD9361 current state structure.
 * @param tx_ch The selected TX channel (1, 2).
 * @param fir_cfg FIR filter configuration output file.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetTxFirConfig(Radio640_hRadio640 hRadio640, uint8_t u8Channel, Ad9361Api_stTxFirConfig * pConfig)
{
    int32_t ret;

	ret = ad9361_get_tx_fir_config (hRadio640->pAd9361Phy,
			u8Channel, pConfig);

	return ret;
}

/**
 * Enable/disable the TX FIR filter.
 * @param phy The AD9361 current state structure.
 * @param u8Enable The option (ENABLE, DISABLE).
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetTxFirEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    int32_t ret;

	ret = ad9361_set_tx_fir_en_dis (hRadio640->pAd9361Phy,
										  u8Enable);

	return ret;
}

/**
 * Get the status of the TX FIR filter.
 * @param phy The AD9361 current state structure.
 * @param pu8Enable The enable/disable status buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetTxFirEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    int32_t ret;

	ret = ad9361_get_tx_fir_en_dis (hRadio640->pAd9361Phy,
										  pu8Enable);

	return ret;
}

/**
 * Set the RX and TX path rates.
 * @param phy The AD9361 state structure.
 * @param pu32RxPathClks RX path rates buffer.
 * @param pu32TxPathClks TX path rates buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SetTrxPathClks(Radio640_hRadio640 hRadio640,
	uint32_t * pu32RxPathClks,
	uint32_t * pu32TxPathClks)
{
    int32_t ret;

	ret = ad9361_set_trx_path_clks (hRadio640->pAd9361Phy,
			pu32RxPathClks, pu32TxPathClks);

	return ret;
}

/**
 * Get the RX and TX path rates.
 * @param phy The AD9361 state structure.
 * @param pu32RxPathClks RX path rates buffer.
 * @param pu32TxPathClks TX path rates buffer.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_GetTrxPathClks(Radio640_hRadio640 hRadio640,
	uint32_t * pu32RxPathClks,
	uint32_t * pu32TxPathClks)
{
    int32_t ret;

	ret = ad9361_get_trx_path_clks (hRadio640->pAd9361Phy,
										  pu32RxPathClks, pu32TxPathClks);

	return ret;
}


Result Radio640_SetRxRfFilter(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, Radio640_eRxRfFilter Filter)
{
    const uint8_t CHANNEL_BIT_MASK = 0x3;
    const uint8_t CHANNEL1_BIT_POS = 2; 
    const uint8_t CHANNEL2_BIT_POS = 0; 
    
    Result res;
    uint8_t u8BitMask = 0;
    uint8_t u8Value = 0;

    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if((uint8_t)Channel & 0x1)
    {
        u8BitMask |= (CHANNEL_BIT_MASK << CHANNEL1_BIT_POS); 
        u8Value   |= ((uint8_t)Filter & CHANNEL_BIT_MASK) << CHANNEL1_BIT_POS;
    }
    
    if((uint8_t)Channel & 0x2)
    {
        u8BitMask |= (CHANNEL_BIT_MASK << CHANNEL2_BIT_POS);
        u8Value   |= ((uint8_t)Filter & CHANNEL_BIT_MASK) << CHANNEL2_BIT_POS;
    }
    
    res = ad9361_set_gpo_manual_value(hRadio640->pAd9361Phy, u8Value, u8BitMask);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}


Result Radio640_SetTxEnableState(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint8_t u8Enable)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(Channel == eRadio640ChannelNone) return eRadio640Ok;
    
    if(u8Enable > 1) return eRadio640ArgIsBoolean;
    
    res = ad9361_en_dis_tx(hRadio640->pAd9361Phy, (uint32_t)Channel, u8Enable);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_SetRxEnableState(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint8_t u8Enable)
{
    Result res;
    uint32_t u32Value;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(Channel == eRadio640ChannelNone) return eRadio640Ok;
    
    if(u8Enable > 1) return eRadio640ArgIsBoolean;
    
    if(u8Enable==1)
    {
        u32Value = 0x3;
    }
    else
    {
        u32Value = 0x0;
    }
    
    res = ad9361_en_dis_rx(hRadio640->pAd9361Phy, (uint32_t)Channel, u32Value);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}


/**
 * Store RX fastlock profile.
 * To create a profile tune the synthesizer (ad9361_set_rx_lo_freq()) and then
 * call this function specifying the target profile number.
 * @param phy The AD9361 state structure.
 * @param profile The profile number (0 - 7).
 * 				  Accepted values:
 * 				   0 - 7
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_StoreRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Profile >= AD9361_NUM_PROFILE_MAX) return eRadio640ArgOutOfRange;
    
	res = ad9361_rx_fastlock_store(hRadio640->pAd9361Phy, (uint32_t)u8Profile);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

/**
 * Recall specified RX fastlock profile.
 * When in fastlock pin select mode (init_param->rx_fastlock_pincontrol_enable),
 * the function needs to be called before then the pin-control can be used.
 * @param phy The AD9361 state structure.
 * @param profile The profile number (0 - 7).
 * 				  Accepted values:
 * 				   0 - 7
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_RecallRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile)
{
	Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Profile >= AD9361_NUM_PROFILE_MAX) return eRadio640ArgOutOfRange;
    
	res = ad9361_rx_fastlock_recall(hRadio640->pAd9361Phy, (uint32_t)u8Profile);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

/**
 * Load RX fastlock profile. A previously saved profile can be loaded in any
 * of the 8 available slots.
 * @param phy The AD9361 state structure.
 * @param profile The profile number (0 - 7).
 * 				  Accepted values:
 * 				   0 - 7
 * @param values Fastlock profile program data.
 * 				 Example:
 * 				  val0,val1,val2,,val15
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_LoadRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values)
{
	Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Profile >= AD9361_NUM_PROFILE_MAX) return eRadio640ArgOutOfRange;
    
	res = ad9361_rx_fastlock_load(hRadio640->pAd9361Phy, (uint32_t)u8Profile, pu8Values);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

/**
 * Save RX fastlock profile. In order to use more than 8 Profiles, an existing
 * profile can be read back and stored by the user application.
 * @param phy The AD9361 state structure.
 * @param profile The profile number (0 - 7).
 * 				  Accepted values:
 * 				   0 - 7
 * @param values Fastlock profile program data.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SaveRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Profile >= AD9361_NUM_PROFILE_MAX) return eRadio640ArgOutOfRange;
    
	res = ad9361_rx_fastlock_save(hRadio640->pAd9361Phy, (uint32_t)u8Profile, pu8Values);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

/**
 * Store TX fastlock profile.
 * To create a profile tune the synthesizer (ad9361_set_TX_lo_freq()) and then
 * call this function specifying the target profile number.
 * @param phy The AD9361 state structure.
 * @param profile The profile number (0 - 7).
 * 				  Accepted values:
 * 				   0 - 7
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_StoreTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile)
{
	Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Profile >= AD9361_NUM_PROFILE_MAX) return eRadio640ArgOutOfRange;
    
	res = ad9361_tx_fastlock_store(hRadio640->pAd9361Phy, (uint32_t)u8Profile);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

/**
 * Recall specified TX fastlock profile.
 * When in fastlock pin select mode (init_param->TX_fastlock_pincontrol_enable),
 * the function needs to be called before then the pin-control can be used.
 * @param phy The AD9361 state structure.
 * @param profile The profile number (0 - 7).
 * 				  Accepted values:
 * 				   0 - 7
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_RecallTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile)
{
	Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Profile >= AD9361_NUM_PROFILE_MAX) return eRadio640ArgOutOfRange;
    
	res = ad9361_tx_fastlock_recall(hRadio640->pAd9361Phy, (uint32_t)u8Profile);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

/**
 * Load TX fastlock profile. A previously saved profile can be loaded in any
 * of the 8 available slots.
 * @param phy The AD9361 state structure.
 * @param profile The profile number (0 - 7).
 * 				  Accepted values:
 * 				   0 - 7
 * @param values Fastlock profile program data.
 * 				 Example:
 * 				  val0,val1,val2,,val15
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_LoadTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values)
{
	Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Profile >= AD9361_NUM_PROFILE_MAX) return eRadio640ArgOutOfRange;
    
	res = ad9361_tx_fastlock_load(hRadio640->pAd9361Phy, (uint32_t)u8Profile, pu8Values);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

/**
 * Save TX fastlock profile. In order to use more than 8 Profiles, an existing
 * profile can be read back and stored by the user application.
 * @param phy The AD9361 state structure.
 * @param profile The profile number (0 - 7).
 * 				  Accepted values:
 * 				   0 - 7
 * @param values Fastlock profile program data.
 * @return 0 in case of success, negative error code otherwise.
 */
Result Radio640_SaveTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Profile >= AD9361_NUM_PROFILE_MAX) return eRadio640ArgOutOfRange;
    
	res = ad9361_tx_fastlock_save(hRadio640->pAd9361Phy, (uint32_t)u8Profile, pu8Values);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_SetRxLoExt(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Enable > 1) return eRadio640ArgIsBoolean;
    
	res = ad9361_set_rx_lo_int_ext(hRadio640->pAd9361Phy, u8Enable);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_SetTxLoExt(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Enable > 1) return eRadio640ArgIsBoolean;
    
	res = ad9361_set_tx_lo_int_ext(hRadio640->pAd9361Phy, u8Enable);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_SetTxAutoCalibEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(u8Enable > 1) return eRadio640ArgIsBoolean;
    
	res = ad9361_set_tx_auto_cal_en_dis(hRadio640->pAd9361Phy, u8Enable);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_GetTxAutoCalibEnable(Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    Result res;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(pu8Enable == NULL) return eRadio640InvPtr;
    
	res = ad9361_get_tx_auto_cal_en_dis(hRadio640->pAd9361Phy, pu8Enable);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

/**
 * Perform the selected calibration.
 *
 * @param i32Value Used by TX_QUAD_CAL only
 *                 Force RX phase value if the value is higher than 0
 *                 A value of -1 mean automatic RX phase
 *                  
 * @return 0 in case of success, negative error code otherwise.
 *
 * Note: This function will/may affect the data path.
 */
Result Radio640_DoCalib(Radio640_hRadio640 hRadio640, Radio640_eCalib Calib, int32_t i32Value)
{
    Result res;
    uint32_t u32Calib;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(Calib == eRadio640CalibTxQuad) u32Calib = TX_QUAD_CAL;
    else if(Calib == eRadio640CalibRxRfDcOffset) u32Calib = RFDC_CAL;
    else return eRadio640InvEnumValue;
    
	res = ad9361_do_calib(hRadio640->pAd9361Phy, u32Calib, i32Value);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_ReadClkFreq(Radio640_hRadio640 hRadio640, Radio640_eTestClk Clk, uint32_t * pu32Freq)
{    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    if(pu32Freq == NULL) return eRadio640InvPtr;
    
    // Select clock
    hRadio640->pCrRegs->u32TestClkSelFreq = (uint32_t)Clk;
    
    // Sleep to let the selector become stable
    Carrier_usleep(hRadio640->hCarrier, 1);
    
    // Read clock frequency
	*pu32Freq = hRadio640->pCrRegs->u32TestClkSelFreq;
    
    return eRadio640Ok;
}

Result Radio640_SetRefOutEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    Result res;
    Radio640_eClkSrc ClkSrc;
    
    if(u8Enable > 1) return eRadio640ArgIsBoolean;
    
    res = Radio640_GetClkSrc(hRadio640, &ClkSrc);
    if(FAILURE(res)) return res;
    
    if(ClkSrc == eRadio640ClkSrcMaster)
    {
        if(u8Enable == 0)
        {
            // Nothing to do
            // HMC7043 is in reset state.
        }
        else //if(u8Enable == 1)
        {
            //The HMC is bypassed. Cannot output RefOut.
            return eRadio640RefOutInvCfg;
        }
    }
    else
    {
        if(u8Enable == 1)
        {
            // Enable Clk0 Leg0 and Leg1
            res = Radio640_Write(hRadio640, eRadio640DeviceClk, 
                0xD0, 0x1B);
            if(FAILURE(res)) return res;
        }
        else
        {
            // On Radio640 Rev A, Ref Out is on Clk0 Leg1
            // On Radio640 Rev B, Ref Out is on Clk0 Leg0
            if(hRadio640->u8IsRevA)
            {
                // Disable Clk0 Leg1
                res = Radio640_Write(hRadio640, eRadio640DeviceClk, 
                    0xD0, 0x19);
                if(FAILURE(res)) return res;
            }
            else
            {
                // Disable Clk0 Leg0
                res = Radio640_Write(hRadio640, eRadio640DeviceClk, 
                    0xD0, 0x1A);
                if(FAILURE(res)) return res;
            }
        }
    }
    
    // Save the configuration
    hRadio640->u8RefOutEnable = u8Enable;
    
    return eRadio640Ok;
}

Result Radio640_SetClkToExtendedFmcEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    Result res;
    
    if(u8Enable > 1) return eRadio640ArgIsBoolean;
    
    if(u8Enable == 0)
    {
        // Disable Clk6
        res = Radio640_Write(hRadio640, eRadio640DeviceClk, 
            0x104, 0x72);
        if(FAILURE(res)) return res;
    }
    else //if(u8Enable == 1)
    {
        if(hRadio640->u8HasExtendedFmcConnector == 0)
        {
            //The board does not have an FMC extended connector
            return eRadio640ClkToExtendedFmcInvCfg;
        }
        else
        {
            // Enable Clk6
            res = Radio640_Write(hRadio640, eRadio640DeviceClk, 
                0x104, 0xF3);
            if(FAILURE(res)) return res;
        }
    }
    
    // Save the configuration
    hRadio640->u8ClkToExtendedFmcEnable = u8Enable;
    
    return eRadio640Ok;
}


