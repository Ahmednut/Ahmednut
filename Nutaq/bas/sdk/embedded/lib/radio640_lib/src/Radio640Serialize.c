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
 * @file    Radio640Serialize.c
 * @brief   Serialize data structure of Radio420/Radio421 for use CCE/EAPI
 *
 * (c) 2012, Nutaq. All rights reserved.
 *
 ****************************************************************************/

#include "bas_debug.h"
#include "eapi_serialize.h"
#include "Radio640Serialize.h"

Result Radio640Serialize_RecvInitParamStruct(connection_state * state, Radio640_stInitParam * pInitParam)
{
    unsigned i;

	pInitParam->ClkSrc                              = (Radio640_eClkSrc)uint32_recv(state);
	pInitParam->u32RefClkFreq                       = uint32_recv(state);
    pInitParam->u8FddModeEnable                     = uint8_recv(state);
    pInitParam->u8FddIndependentModeEnable          = uint8_recv(state);
    pInitParam->u8TddSkipVcoCalEnable               = uint8_recv(state);
    pInitParam->u8TddUseFddVcoTablesEnable          = uint8_recv(state);
    pInitParam->u32TxFastlockDelayNs                = uint32_recv(state);
    pInitParam->u32RxFastlockDelayNs                = uint32_recv(state);
    pInitParam->u8RxFastlockPinControlEnable        = uint8_recv(state);
    pInitParam->u8TxFastlockPinControlEnable        = uint8_recv(state);
    pInitParam->u8ExtRxLoEnable                     = uint8_recv(state);
    pInitParam->u8ExtTxLoEnable                     = uint8_recv(state);
    pInitParam->u8SplitGainTableModeEnable          = uint8_recv(state);
    pInitParam->u8RxQuadTrackSlowModeEnable         = uint8_recv(state);
    pInitParam->u8DcOffsetTrackingUpdateEventMask   = uint8_recv(state);
    pInitParam->u8DcOffsetAttenuationHighRange      = uint8_recv(state);
    pInitParam->u8DcOffsetAttenuationLowRange       = uint8_recv(state);
    pInitParam->u8DcOffsetCountHighRange            = uint8_recv(state);
    pInitParam->u8DcOffsetCountLowRange             = uint8_recv(state);
    pInitParam->u8EnsmEnablePinPulseModeEnable      = uint8_recv(state);
    pInitParam->u8EnsmEnableTxnrxControlEnable      = uint8_recv(state);
    pInitParam->u64RxRfFreq                         = uint64_recv(state);
    pInitParam->u64TxRfFreq                         = uint64_recv(state);

    for (i=0; i<6; i++)
    {
		pInitParam->au32RxClockFreq[i] = uint32_recv(state);
		pInitParam->au32TxClockFreq[i] = uint32_recv(state);
    }

	pInitParam->u32RxRfBandwidth = uint32_recv(state);
	pInitParam->u32TxRfBandwidth = uint32_recv(state);
    
	pInitParam->tx_attenuation_mdB = int32_recv(state);
	pInitParam->update_tx_gain_in_alert_enable = uint8_recv(state);
    pInitParam->clk_output_mode_select = uint32_recv(state);
	pInitParam->gc_rx1_mode = uint8_recv(state);
	pInitParam->gc_rx2_mode = uint8_recv(state);
	pInitParam->gc_adc_large_overload_thresh = uint8_recv(state);
	pInitParam->gc_adc_ovr_sample_size = uint8_recv(state);
	pInitParam->gc_adc_small_overload_thresh = uint8_recv(state);
	pInitParam->gc_dec_pow_measurement_duration = uint32_recv(state);
	pInitParam->gc_dig_gain_enable = uint8_recv(state);
	pInitParam->gc_lmt_overload_high_thresh = uint16_recv(state);
	pInitParam->gc_lmt_overload_low_thresh = uint16_recv(state);
	pInitParam->gc_low_power_thresh = uint8_recv(state);
	pInitParam->gc_max_dig_gain = uint8_recv(state);
	pInitParam->mgc_dec_gain_step = uint8_recv(state);
	pInitParam->mgc_inc_gain_step = uint8_recv(state);
	pInitParam->mgc_rx1_ctrl_inp_enable = uint8_recv(state);
	pInitParam->mgc_rx2_ctrl_inp_enable = uint8_recv(state);
	pInitParam->mgc_split_table_ctrl_inp_gain_mode = uint8_recv(state);
	pInitParam->agc_adc_large_overload_exceed_counter = uint8_recv(state);
	pInitParam->agc_adc_large_overload_inc_steps = uint8_recv(state);
	pInitParam->agc_adc_lmt_small_overload_prevent_gain_inc_enable = uint8_recv(state);
	pInitParam->agc_adc_small_overload_exceed_counter = uint8_recv(state);
	pInitParam->agc_dig_gain_step_size = uint8_recv(state);
	pInitParam->agc_dig_saturation_exceed_counter = uint8_recv(state);
	pInitParam->agc_gain_update_interval_us = uint32_recv(state);
	pInitParam->agc_immed_gain_change_if_large_adc_overload_enable = uint8_recv(state);
	pInitParam->agc_immed_gain_change_if_large_lmt_overload_enable = uint8_recv(state);
	pInitParam->agc_inner_thresh_high = uint8_recv(state);
	pInitParam->agc_inner_thresh_high_dec_steps = uint8_recv(state);
	pInitParam->agc_inner_thresh_low = uint8_recv(state);
	pInitParam->agc_inner_thresh_low_inc_steps = uint8_recv(state);
	pInitParam->agc_lmt_overload_large_exceed_counter = uint8_recv(state);
	pInitParam->agc_lmt_overload_large_inc_steps = uint8_recv(state);
	pInitParam->agc_lmt_overload_small_exceed_counter = uint8_recv(state);
	pInitParam->agc_outer_thresh_high = uint8_recv(state);
	pInitParam->agc_outer_thresh_high_dec_steps = uint8_recv(state);
	pInitParam->agc_outer_thresh_low = uint8_recv(state);
	pInitParam->agc_outer_thresh_low_inc_steps = uint8_recv(state);
	pInitParam->agc_attack_delay_extra_margin_us = uint32_recv(state);
	pInitParam->agc_sync_for_gain_counter_enable = uint8_recv(state);
	pInitParam->fagc_dec_pow_measuremnt_duration = uint32_recv(state);
	pInitParam->fagc_state_wait_time_ns = uint32_recv(state);
	pInitParam->fagc_allow_agc_gain_increase = uint8_recv(state);
	pInitParam->fagc_lp_thresh_increment_time = uint32_recv(state);
	pInitParam->fagc_lp_thresh_increment_steps = uint32_recv(state);
	pInitParam->fagc_lock_level = uint32_recv(state);
	pInitParam->fagc_lock_level_lmt_gain_increase_en = uint8_recv(state);
	pInitParam->fagc_lock_level_gain_increase_upper_limit = uint32_recv(state);
	pInitParam->fagc_lpf_final_settling_steps = uint32_recv(state);
	pInitParam->fagc_lmt_final_settling_steps = uint32_recv(state);
	pInitParam->fagc_final_overrange_count = uint32_recv(state);
	pInitParam->fagc_gain_increase_after_gain_lock_en = uint8_recv(state);
	pInitParam->fagc_gain_index_type_after_exit_rx_mode = uint32_recv(state);
	pInitParam->fagc_use_last_lock_level_for_set_gain_en = uint8_recv(state);
	pInitParam->fagc_rst_gla_stronger_sig_thresh_exceeded_en = uint8_recv(state);
	pInitParam->fagc_optimized_gain_offset = uint32_recv(state);
	pInitParam->fagc_rst_gla_stronger_sig_thresh_above_ll = uint32_recv(state);
	pInitParam->fagc_rst_gla_engergy_lost_sig_thresh_exceeded_en = uint8_recv(state);
	pInitParam->fagc_rst_gla_engergy_lost_goto_optim_gain_en = uint8_recv(state);
	pInitParam->fagc_rst_gla_engergy_lost_sig_thresh_below_ll = uint32_recv(state);
	pInitParam->fagc_energy_lost_stronger_sig_gain_lock_exit_cnt = uint32_recv(state);
	pInitParam->fagc_rst_gla_large_adc_overload_en = uint8_recv(state);
	pInitParam->fagc_rst_gla_large_lmt_overload_en = uint8_recv(state);
	pInitParam->fagc_rst_gla_en_agc_pulled_high_en = uint8_recv(state);
	pInitParam->fagc_rst_gla_if_en_agc_pulled_high_mode = uint32_recv(state);
	pInitParam->fagc_power_measurement_duration_in_state5 = uint32_recv(state);
	pInitParam->rssi_delay = uint32_recv(state);
	pInitParam->rssi_duration = uint32_recv(state);
	pInitParam->rssi_restart_mode = uint8_recv(state);
	pInitParam->rssi_unit_is_rx_samples_enable = uint8_recv(state);
	pInitParam->rssi_wait = uint32_recv(state);
	pInitParam->temp_sense_decimation = uint32_recv(state);
	pInitParam->temp_sense_measurement_interval_ms = uint16_recv(state);
	pInitParam->temp_sense_offset_signed = int8_recv(state);
	pInitParam->temp_sense_periodic_measurement_enable = uint8_recv(state);
	pInitParam->ctrl_outs_enable_mask = uint8_recv(state);
	pInitParam->ctrl_outs_index = uint8_recv(state);

    return OK;
}

Result Radio640Serialize_SendInitParamStruct(connection_state * state, Radio640_stInitParam * pInitParam)
{
    unsigned i;

	uint32_send(state,      (uint32_t)pInitParam->ClkSrc                  );
	uint32_send(state,      pInitParam->u32RefClkFreq                     );
    uint8_send(state,       pInitParam->u8FddModeEnable                   );
    uint8_send(state,       pInitParam->u8FddIndependentModeEnable        );
    uint8_send(state,       pInitParam->u8TddSkipVcoCalEnable             );
    uint8_send(state,       pInitParam->u8TddUseFddVcoTablesEnable        );
    uint32_send(state,      pInitParam->u32TxFastlockDelayNs              );
    uint32_send(state,      pInitParam->u32RxFastlockDelayNs              );
    uint8_send(state,       pInitParam->u8RxFastlockPinControlEnable      );
    uint8_send(state,       pInitParam->u8TxFastlockPinControlEnable      );
    uint8_send(state,       pInitParam->u8ExtRxLoEnable                   );
    uint8_send(state,       pInitParam->u8ExtTxLoEnable                   );
    uint8_send(state,       pInitParam->u8SplitGainTableModeEnable        );
    uint8_send(state,       pInitParam->u8RxQuadTrackSlowModeEnable       );
    uint8_send(state,       pInitParam->u8DcOffsetTrackingUpdateEventMask );
    uint8_send(state,       pInitParam->u8DcOffsetAttenuationHighRange    );
    uint8_send(state,       pInitParam->u8DcOffsetAttenuationLowRange     );
    uint8_send(state,       pInitParam->u8DcOffsetCountHighRange          );
    uint8_send(state,       pInitParam->u8DcOffsetCountLowRange           );
    uint8_send(state,       pInitParam->u8EnsmEnablePinPulseModeEnable    );
    uint8_send(state,       pInitParam->u8EnsmEnableTxnrxControlEnable    );
    uint64_send(state,      pInitParam->u64RxRfFreq                       );
    uint64_send(state,      pInitParam->u64TxRfFreq                       );

    for (i=0; i<6; i++)
    {
        uint32_send(state,  pInitParam->au32RxClockFreq[i]);
        uint32_send(state,  pInitParam->au32TxClockFreq[i]);
    }

	uint32_send(state,      pInitParam->u32RxRfBandwidth);
	uint32_send(state,      pInitParam->u32TxRfBandwidth);
    
	int32_send(state,       pInitParam->tx_attenuation_mdB);
	uint8_send(state,       pInitParam->update_tx_gain_in_alert_enable);
    uint32_send(state,      pInitParam->clk_output_mode_select);
	uint8_send(state,       pInitParam->gc_rx1_mode);
	uint8_send(state,       pInitParam->gc_rx2_mode);
	uint8_send(state,       pInitParam->gc_adc_large_overload_thresh);
	uint8_send(state,       pInitParam->gc_adc_ovr_sample_size);
	uint8_send(state,       pInitParam->gc_adc_small_overload_thresh);
	uint32_send(state,      pInitParam->gc_dec_pow_measurement_duration);
	uint8_send(state,       pInitParam->gc_dig_gain_enable);
	uint16_send(state,      pInitParam->gc_lmt_overload_high_thresh);
	uint16_send(state,      pInitParam->gc_lmt_overload_low_thresh);
	uint8_send(state,       pInitParam->gc_low_power_thresh);
	uint8_send(state,       pInitParam->gc_max_dig_gain);
	uint8_send(state,       pInitParam->mgc_dec_gain_step);
	uint8_send(state,       pInitParam->mgc_inc_gain_step);
	uint8_send(state,       pInitParam->mgc_rx1_ctrl_inp_enable);
	uint8_send(state,       pInitParam->mgc_rx2_ctrl_inp_enable);
	uint8_send(state,       pInitParam->mgc_split_table_ctrl_inp_gain_mode);
	uint8_send(state,       pInitParam->agc_adc_large_overload_exceed_counter);
	uint8_send(state,       pInitParam->agc_adc_large_overload_inc_steps);
	uint8_send(state,       pInitParam->agc_adc_lmt_small_overload_prevent_gain_inc_enable);
	uint8_send(state,       pInitParam->agc_adc_small_overload_exceed_counter);
	uint8_send(state,       pInitParam->agc_dig_gain_step_size);
	uint8_send(state,       pInitParam->agc_dig_saturation_exceed_counter);
	uint32_send(state,      pInitParam->agc_gain_update_interval_us);
	uint8_send(state,       pInitParam->agc_immed_gain_change_if_large_adc_overload_enable);
	uint8_send(state,       pInitParam->agc_immed_gain_change_if_large_lmt_overload_enable);
	uint8_send(state,       pInitParam->agc_inner_thresh_high);
	uint8_send(state,       pInitParam->agc_inner_thresh_high_dec_steps);
	uint8_send(state,       pInitParam->agc_inner_thresh_low);
	uint8_send(state,       pInitParam->agc_inner_thresh_low_inc_steps);
	uint8_send(state,       pInitParam->agc_lmt_overload_large_exceed_counter);
	uint8_send(state,       pInitParam->agc_lmt_overload_large_inc_steps);
	uint8_send(state,       pInitParam->agc_lmt_overload_small_exceed_counter);
	uint8_send(state,       pInitParam->agc_outer_thresh_high);
	uint8_send(state,       pInitParam->agc_outer_thresh_high_dec_steps);
	uint8_send(state,       pInitParam->agc_outer_thresh_low);
	uint8_send(state,       pInitParam->agc_outer_thresh_low_inc_steps);
	uint32_send(state,      pInitParam->agc_attack_delay_extra_margin_us);
	uint8_send(state,       pInitParam->agc_sync_for_gain_counter_enable);
	uint32_send(state,      pInitParam->fagc_dec_pow_measuremnt_duration);
	uint32_send(state,      pInitParam->fagc_state_wait_time_ns);
	uint8_send(state,       pInitParam->fagc_allow_agc_gain_increase);
	uint32_send(state,      pInitParam->fagc_lp_thresh_increment_time);
	uint32_send(state,      pInitParam->fagc_lp_thresh_increment_steps);
	uint32_send(state,      pInitParam->fagc_lock_level);
	uint8_send(state,       pInitParam->fagc_lock_level_lmt_gain_increase_en);
	uint32_send(state,      pInitParam->fagc_lock_level_gain_increase_upper_limit);
	uint32_send(state,      pInitParam->fagc_lpf_final_settling_steps);
	uint32_send(state,      pInitParam->fagc_lmt_final_settling_steps);
	uint32_send(state,      pInitParam->fagc_final_overrange_count);
	uint8_send(state,       pInitParam->fagc_gain_increase_after_gain_lock_en);
	uint32_send(state,      pInitParam->fagc_gain_index_type_after_exit_rx_mode);
	uint8_send(state,       pInitParam->fagc_use_last_lock_level_for_set_gain_en);
	uint8_send(state,       pInitParam->fagc_rst_gla_stronger_sig_thresh_exceeded_en);
	uint32_send(state,      pInitParam->fagc_optimized_gain_offset);
	uint32_send(state,      pInitParam->fagc_rst_gla_stronger_sig_thresh_above_ll);
	uint8_send(state,       pInitParam->fagc_rst_gla_engergy_lost_sig_thresh_exceeded_en);
	uint8_send(state,       pInitParam->fagc_rst_gla_engergy_lost_goto_optim_gain_en);
	uint32_send(state,      pInitParam->fagc_rst_gla_engergy_lost_sig_thresh_below_ll);
	uint32_send(state,      pInitParam->fagc_energy_lost_stronger_sig_gain_lock_exit_cnt);
	uint8_send(state,       pInitParam->fagc_rst_gla_large_adc_overload_en);
	uint8_send(state,       pInitParam->fagc_rst_gla_large_lmt_overload_en);
	uint8_send(state,       pInitParam->fagc_rst_gla_en_agc_pulled_high_en);
	uint32_send(state,      pInitParam->fagc_rst_gla_if_en_agc_pulled_high_mode);
	uint32_send(state,      pInitParam->fagc_power_measurement_duration_in_state5);
	uint32_send(state,      pInitParam->rssi_delay);
	uint32_send(state,      pInitParam->rssi_duration);
	uint8_send(state,       pInitParam->rssi_restart_mode);
	uint8_send(state,       pInitParam->rssi_unit_is_rx_samples_enable);
	uint32_send(state,      pInitParam->rssi_wait);
	uint32_send(state,      pInitParam->temp_sense_decimation);
	uint16_send(state,      pInitParam->temp_sense_measurement_interval_ms);
	int8_send(state,        pInitParam->temp_sense_offset_signed);
	uint8_send(state,       pInitParam->temp_sense_periodic_measurement_enable);
	uint8_send(state,       pInitParam->ctrl_outs_enable_mask);
	uint8_send(state,       pInitParam->ctrl_outs_index);

    return OK;
}

Result Radio640Serialize_RecvRfRssiStruct(connection_state * state, Ad9361_stRfRssi * pRfRssi)
{
    pRfRssi->ant = uint32_recv(state);
    pRfRssi->symbol = uint32_recv(state);
    pRfRssi->preamble = uint32_recv(state);
    pRfRssi->multiplier = int32_recv(state);
    pRfRssi->duration = uint8_recv(state);

    return OK;
}

Result Radio640Serialize_SendRfRssiStruct(connection_state * state, Ad9361_stRfRssi * pRfRssi)
{
    uint32_send(state, pRfRssi->ant);
    uint32_send(state, pRfRssi->symbol);
    uint32_send(state, pRfRssi->preamble);
    int32_send(state, pRfRssi->multiplier);
    uint8_send(state, pRfRssi->duration);

    return OK;
}

Result Radio640Serialize_RecvRxFirConfigStruct(connection_state * state, Ad9361Api_stRxFirConfig * pConfig)
{
    int i;

    pConfig->rx = uint32_recv(state);
    pConfig->rx_gain = int32_recv(state);
    pConfig->rx_dec = uint32_recv(state);

    for(i=0 ;i<128; i++) pConfig->rx_coef[i] = int16_recv(state);

    pConfig->rx_coef_size = uint8_recv(state);

    return OK;
}

Result Radio640Serialize_SendRxFirConfigStruct(connection_state * state, Ad9361Api_stRxFirConfig * pConfig)
{
    int i;

    uint32_send(state, pConfig->rx);
    int32_send(state, pConfig->rx_gain);
    uint32_send(state, pConfig->rx_dec);

    for(i=0 ;i<128; i++) int16_send(state, pConfig->rx_coef[i]);

    uint8_send(state, pConfig->rx_coef_size);

    return OK;
}

Result Radio640Serialize_RecvTxFirConfigStruct(connection_state * state, Ad9361Api_stTxFirConfig * pConfig)
{
    int i;

    pConfig->tx = uint32_recv(state);
    pConfig->tx_gain = int32_recv(state);
    pConfig->tx_int = uint32_recv(state);

    for(i=0 ;i<128; i++) pConfig->tx_coef[i] = int16_recv(state);

    pConfig->tx_coef_size = uint8_recv(state);

    return OK;
}

Result Radio640Serialize_SendTxFirConfigStruct(connection_state * state, Ad9361Api_stTxFirConfig * pConfig)
{
    int i;

    uint32_send(state, pConfig->tx);
    int32_send(state, pConfig->tx_gain);
    uint32_send(state, pConfig->tx_int);

    for(i=0 ;i<128; i++) int16_send(state, pConfig->tx_coef[i]);

    uint8_send(state, pConfig->tx_coef_size);

    return OK;
}

Result Radio640Serialize_RecvCalibResultStruct(connection_state * state, Ad9361_stCalibResult * pCalibResult)
{
    pCalibResult->cSucceeded = uint8_recv(state);
    pCalibResult->cBitSlip[0] = uint8_recv(state);
    pCalibResult->cBitSlip[1] = uint8_recv(state);
    pCalibResult->cClkDelay[0] = uint8_recv(state);
    pCalibResult->cClkDelay[1] = uint8_recv(state);
    pCalibResult->cDataDelay[0] = uint8_recv(state);
    pCalibResult->cDataDelay[1] = uint8_recv(state);
    pCalibResult->cDataDelayWidth[0] = uint8_recv(state);
    pCalibResult->cDataDelayWidth[1] = uint8_recv(state);

    return OK;
}

Result Radio640Serialize_SendCalibResultStruct(connection_state * state, Ad9361_stCalibResult * pCalibResult)
{
    if(pCalibResult!=NULL)
    {
        uint8_send(state, pCalibResult->cSucceeded);
        uint8_send(state, pCalibResult->cBitSlip[0]);
        uint8_send(state, pCalibResult->cBitSlip[1]);
        uint8_send(state, pCalibResult->cClkDelay[0]);
        uint8_send(state, pCalibResult->cClkDelay[1]);
        uint8_send(state, pCalibResult->cDataDelay[0]);
        uint8_send(state, pCalibResult->cDataDelay[1]);
        uint8_send(state, pCalibResult->cDataDelayWidth[0]);
        uint8_send(state, pCalibResult->cDataDelayWidth[1]);
    }
    else
    {
        uint8_send(state, 0);
        uint8_send(state, 0);
        uint8_send(state, 0);
        uint8_send(state, 0);
        uint8_send(state, 0);
        uint8_send(state, 0);
        uint8_send(state, 0);
        uint8_send(state, 0);
        uint8_send(state, 0);
    }

    return OK;
}
