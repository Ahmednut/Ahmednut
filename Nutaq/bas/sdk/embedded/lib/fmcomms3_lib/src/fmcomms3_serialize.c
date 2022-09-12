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
 * @file    fmcomms3_serialize.c
 * @brief   Serialize data structure of Radio420/Radio421 for use CCE/EAPI
 *
 * (c) 2012, Nutaq. All rights reserved.
 *
 ****************************************************************************/

#include "fmcomms3_serialize.h"
#include "eapi_serialize.h"
#include "bas_debug.h"

Result fmcomms3_initparam_struct_recv(connection_state * state, AD9361_InitParam * param)
{
    unsigned i;

	param->id_no = uint8_recv(state);
	param->reference_clk_rate = uint32_recv(state);
	param->two_rx_two_tx_mode_enable = uint8_recv(state);
	param->frequency_division_duplex_mode_enable = uint8_recv(state);
	param->tdd_use_dual_synth_mode_enable = uint8_recv(state);
	param->tdd_skip_vco_cal_enable = uint8_recv(state);
	param->tx_fastlock_delay_ns = uint32_recv(state);
	param->rx_fastlock_delay_ns = uint32_recv(state);
	param->rx_fastlock_pincontrol_enable = uint8_recv(state);
	param->tx_fastlock_pincontrol_enable = uint8_recv(state);
	param->external_rx_lo_enable = uint8_recv(state);
	param->external_tx_lo_enable = uint8_recv(state);
	param->dc_offset_tracking_update_event_mask = uint8_recv(state);
	param->dc_offset_attenuation_high_range = uint8_recv(state);
	param->dc_offset_attenuation_low_range = uint8_recv(state);
	param->dc_offset_count_high_range = uint8_recv(state);
	param->dc_offset_count_low_range = uint8_recv(state);
	param->tdd_use_fdd_vco_tables_enable = uint8_recv(state);
	param->split_gain_table_mode_enable = uint8_recv(state);
	param->ensm_enable_pin_pulse_mode_enable = uint8_recv(state);
	param->ensm_enable_txnrx_control_enable = uint8_recv(state);
	param->rx_synthesizer_frequency_hz = uint64_recv(state);
	param->tx_synthesizer_frequency_hz = uint64_recv(state);

    for (i=0; i<6; i++)
    {
		param->rx_path_clock_frequencies[i] = uint32_recv(state);
		param->tx_path_clock_frequencies[i] = uint32_recv(state);
    }

	param->rf_rx_bandwidth_hz = uint32_recv(state);
	param->rf_tx_bandwidth_hz = uint32_recv(state);
	param->rx_rf_port_input_select = uint32_recv(state);
	param->tx_rf_port_input_select = uint32_recv(state);
	param->tx_attenuation_mdB = int32_recv(state);
	param->update_tx_gain_in_alert_enable = uint8_recv(state);
	param->xo_disable_use_ext_refclk_enable = uint8_recv(state);

    for (i=0; i<2; i++)
    {
        param->dcxo_coarse_and_fine_tune[i] = uint32_recv(state);
    }

    param->clk_output_mode_select = uint32_recv(state);
	param->gc_rx1_mode = uint8_recv(state);
	param->gc_rx2_mode = uint8_recv(state);
	param->gc_adc_large_overload_thresh = uint8_recv(state);
	param->gc_adc_ovr_sample_size = uint8_recv(state);
	param->gc_adc_small_overload_thresh = uint8_recv(state);
	param->gc_dec_pow_measurement_duration = uint16_recv(state);
	param->gc_dig_gain_enable = uint8_recv(state);
	param->gc_lmt_overload_high_thresh = uint16_recv(state);
	param->gc_lmt_overload_low_thresh = uint16_recv(state);
	param->gc_low_power_thresh = uint8_recv(state);
	param->gc_max_dig_gain = uint8_recv(state);
	param->mgc_dec_gain_step = uint8_recv(state);
	param->mgc_inc_gain_step = uint8_recv(state);
	param->mgc_rx1_ctrl_inp_enable = uint8_recv(state);
	param->mgc_rx2_ctrl_inp_enable = uint8_recv(state);
	param->mgc_split_table_ctrl_inp_gain_mode = uint8_recv(state);
	param->agc_adc_large_overload_exceed_counter = uint8_recv(state);
	param->agc_adc_large_overload_inc_steps = uint8_recv(state);
	param->agc_adc_lmt_small_overload_prevent_gain_inc_enable = uint8_recv(state);
	param->agc_adc_small_overload_exceed_counter = uint8_recv(state);
	param->agc_dig_gain_step_size = uint8_recv(state);
	param->agc_dig_saturation_exceed_counter = uint8_recv(state);
	param->agc_gain_update_interval_us = uint32_recv(state);
	param->agc_immed_gain_change_if_large_adc_overload_enable = uint8_recv(state);
	param->agc_immed_gain_change_if_large_lmt_overload_enable = uint8_recv(state);
	param->agc_inner_thresh_high = uint8_recv(state);
	param->agc_inner_thresh_high_dec_steps = uint8_recv(state);
	param->agc_inner_thresh_low = uint8_recv(state);
	param->agc_inner_thresh_low_inc_steps = uint8_recv(state);
	param->agc_lmt_overload_large_exceed_counter = uint8_recv(state);
	param->agc_lmt_overload_large_inc_steps = uint8_recv(state);
	param->agc_lmt_overload_small_exceed_counter = uint8_recv(state);
	param->agc_outer_thresh_high = uint8_recv(state);
	param->agc_outer_thresh_high_dec_steps = uint8_recv(state);
	param->agc_outer_thresh_low = uint8_recv(state);
	param->agc_outer_thresh_low_inc_steps = uint8_recv(state);
	param->agc_attack_delay_extra_margin_us = uint32_recv(state);
	param->agc_sync_for_gain_counter_enable = uint8_recv(state);
	param->fagc_dec_pow_measuremnt_duration = uint32_recv(state);
	param->fagc_state_wait_time_ns = uint32_recv(state);
	param->fagc_allow_agc_gain_increase = uint8_recv(state);
	param->fagc_lp_thresh_increment_time = uint32_recv(state);
	param->fagc_lp_thresh_increment_steps = uint32_recv(state);
	param->fagc_lock_level = uint32_recv(state);
	param->fagc_lock_level_lmt_gain_increase_en = uint8_recv(state);
	param->fagc_lock_level_gain_increase_upper_limit = uint32_recv(state);
	param->fagc_lpf_final_settling_steps = uint32_recv(state);
	param->fagc_lmt_final_settling_steps = uint32_recv(state);
	param->fagc_final_overrange_count = uint32_recv(state);
	param->fagc_gain_increase_after_gain_lock_en = uint8_recv(state);
	param->fagc_gain_index_type_after_exit_rx_mode = uint32_recv(state);
	param->fagc_use_last_lock_level_for_set_gain_en = uint8_recv(state);
	param->fagc_rst_gla_stronger_sig_thresh_exceeded_en = uint8_recv(state);
	param->fagc_optimized_gain_offset = uint32_recv(state);
	param->fagc_rst_gla_stronger_sig_thresh_above_ll = uint32_recv(state);
	param->fagc_rst_gla_engergy_lost_sig_thresh_exceeded_en = uint8_recv(state);
	param->fagc_rst_gla_engergy_lost_goto_optim_gain_en = uint8_recv(state);
	param->fagc_rst_gla_engergy_lost_sig_thresh_below_ll = uint32_recv(state);
	param->fagc_energy_lost_stronger_sig_gain_lock_exit_cnt = uint32_recv(state);
	param->fagc_rst_gla_large_adc_overload_en = uint8_recv(state);
	param->fagc_rst_gla_large_lmt_overload_en = uint8_recv(state);
	param->fagc_rst_gla_en_agc_pulled_high_en = uint8_recv(state);
	param->fagc_rst_gla_if_en_agc_pulled_high_mode = uint32_recv(state);
	param->fagc_power_measurement_duration_in_state5 = uint32_recv(state);
	param->rssi_delay = uint32_recv(state);
	param->rssi_duration = uint32_recv(state);
	param->rssi_restart_mode = uint8_recv(state);
	param->rssi_unit_is_rx_samples_enable = uint8_recv(state);
	param->rssi_wait = uint32_recv(state);
	param->aux_adc_decimation = uint32_recv(state);
	param->aux_adc_rate = uint32_recv(state);
	param->aux_dac_manual_mode_enable = uint8_recv(state);
	param->aux_dac1_default_value_mV = uint32_recv(state);
	param->aux_dac1_active_in_rx_enable = uint8_recv(state);
	param->aux_dac1_active_in_tx_enable = uint8_recv(state);
	param->aux_dac1_active_in_alert_enable = uint8_recv(state);
	param->aux_dac1_rx_delay_us = uint32_recv(state);
	param->aux_dac1_tx_delay_us = uint32_recv(state);
	param->aux_dac2_default_value_mV = uint32_recv(state);
	param->aux_dac2_active_in_rx_enable = uint8_recv(state);
	param->aux_dac2_active_in_tx_enable = uint8_recv(state);
	param->aux_dac2_active_in_alert_enable = uint8_recv(state);
	param->aux_dac2_rx_delay_us = uint32_recv(state);
	param->aux_dac2_tx_delay_us = uint32_recv(state);
	param->temp_sense_decimation = uint32_recv(state);
	param->temp_sense_measurement_interval_ms = uint16_recv(state);
	param->temp_sense_offset_signed = int8_recv(state);
	param->temp_sense_periodic_measurement_enable = uint8_recv(state);
	param->ctrl_outs_enable_mask = uint8_recv(state);
	param->ctrl_outs_index = uint8_recv(state);
	param->elna_settling_delay_ns = uint32_recv(state);
	param->elna_gain_mdB = uint32_recv(state);
	param->elna_bypass_loss_mdB = uint32_recv(state);
	param->elna_rx1_gpo0_control_enable = uint8_recv(state);
	param->elna_rx2_gpo1_control_enable = uint8_recv(state);
	param->pp_tx_swap_enable = uint8_recv(state);
	param->pp_rx_swap_enable = uint8_recv(state);
	param->tx_channel_swap_enable = uint8_recv(state);
	param->rx_channel_swap_enable = uint8_recv(state);
	param->rx_frame_pulse_mode_enable = uint8_recv(state);
	param->two_t_two_r_timing_enable = uint8_recv(state);
	param->invert_data_bus_enable = uint8_recv(state);
	param->invert_data_clk_enable = uint8_recv(state);
	param->fdd_alt_word_order_enable = uint8_recv(state);
	param->invert_rx_frame_enable = uint8_recv(state);
	param->fdd_rx_rate_2tx_enable = uint8_recv(state);
	param->swap_ports_enable = uint8_recv(state);
	param->single_data_rate_enable = uint8_recv(state);
	param->lvds_mode_enable = uint8_recv(state);
	param->half_duplex_mode_enable = uint8_recv(state);
	param->single_port_mode_enable = uint8_recv(state);
	param->full_port_enable = uint8_recv(state);
	param->full_duplex_swap_bits_enable = uint8_recv(state);
	param->delay_rx_data = uint32_recv(state);
	param->rx_data_clock_delay = uint32_recv(state);
	param->rx_data_delay = uint32_recv(state);
	param->tx_fb_clock_delay = uint32_recv(state);
	param->tx_data_delay = uint32_recv(state);
	param->lvds_bias_mV = uint32_recv(state);
	param->lvds_rx_onchip_termination_enable = uint8_recv(state);
	param->rx1rx2_phase_inversion_en = uint8_recv(state);
	param->low_high_gain_threshold_mdB = uint32_recv(state);
	param->low_gain_dB = uint32_recv(state);
	param->high_gain_dB = uint32_recv(state);
	param->tx_mon_track_en = uint8_recv(state);
	param->one_shot_mode_en = uint8_recv(state);
	param->tx_mon_delay = uint32_recv(state);
	param->tx_mon_duration = uint32_recv(state);
	param->tx1_mon_front_end_gain = uint32_recv(state);
	param->tx2_mon_front_end_gain = uint32_recv(state);
	param->tx1_mon_lo_cm = uint32_recv(state);
	param->tx2_mon_lo_cm = uint32_recv(state);
	param->gpio_resetb = int32_recv(state);
	param->gpio_sync = int32_recv(state);
	param->gpio_cal_sw1 = int32_recv(state);
	param->gpio_cal_sw2 = int32_recv(state);

    return OK;
}

Result fmcomms3_initparam_struct_send(connection_state * state, AD9361_InitParam * param)
{
    unsigned i;

	uint8_send(state, param->id_no);
	uint32_send(state, param->reference_clk_rate);
	uint8_send(state, param->two_rx_two_tx_mode_enable);
	uint8_send(state, param->frequency_division_duplex_mode_enable);
	uint8_send(state, param->tdd_use_dual_synth_mode_enable);
	uint8_send(state, param->tdd_skip_vco_cal_enable);
	uint32_send(state, param->tx_fastlock_delay_ns);
	uint32_send(state, param->rx_fastlock_delay_ns);
	uint8_send(state, param->rx_fastlock_pincontrol_enable);
	uint8_send(state, param->tx_fastlock_pincontrol_enable);
	uint8_send(state, param->external_rx_lo_enable);
	uint8_send(state, param->external_tx_lo_enable);
	uint8_send(state, param->dc_offset_tracking_update_event_mask);
	uint8_send(state, param->dc_offset_attenuation_high_range);
	uint8_send(state, param->dc_offset_attenuation_low_range);
	uint8_send(state, param->dc_offset_count_high_range);
	uint8_send(state, param->dc_offset_count_low_range);
	uint8_send(state, param->tdd_use_fdd_vco_tables_enable);
	uint8_send(state, param->split_gain_table_mode_enable);
	uint8_send(state, param->ensm_enable_pin_pulse_mode_enable);
	uint8_send(state, param->ensm_enable_txnrx_control_enable);
	uint64_send(state, param->rx_synthesizer_frequency_hz);
	uint64_send(state, param->tx_synthesizer_frequency_hz);

    for (i=0; i<6; i++)
    {
        uint32_send(state, param->rx_path_clock_frequencies[i]);
        uint32_send(state, param->tx_path_clock_frequencies[i]);
    }

	uint32_send(state, param->rf_rx_bandwidth_hz);
	uint32_send(state, param->rf_tx_bandwidth_hz);
	uint32_send(state, param->rx_rf_port_input_select);
	uint32_send(state, param->tx_rf_port_input_select);
	int32_send(state, param->tx_attenuation_mdB);
	uint8_send(state, param->update_tx_gain_in_alert_enable);
	uint8_send(state, param->xo_disable_use_ext_refclk_enable);

    for (i=0; i<2; i++)
    {
        uint32_send(state, param->dcxo_coarse_and_fine_tune[i]);
    }

    uint32_send(state, param->clk_output_mode_select);
	uint8_send(state, param->gc_rx1_mode);
	uint8_send(state, param->gc_rx2_mode);
	uint8_send(state, param->gc_adc_large_overload_thresh);
	uint8_send(state, param->gc_adc_ovr_sample_size);
	uint8_send(state, param->gc_adc_small_overload_thresh);
	uint16_send(state, param->gc_dec_pow_measurement_duration);
	uint8_send(state, param->gc_dig_gain_enable);
	uint16_send(state, param->gc_lmt_overload_high_thresh);
	uint16_send(state, param->gc_lmt_overload_low_thresh);
	uint8_send(state, param->gc_low_power_thresh);
	uint8_send(state, param->gc_max_dig_gain);
	uint8_send(state, param->mgc_dec_gain_step);
	uint8_send(state, param->mgc_inc_gain_step);
	uint8_send(state, param->mgc_rx1_ctrl_inp_enable);
	uint8_send(state, param->mgc_rx2_ctrl_inp_enable);
	uint8_send(state, param->mgc_split_table_ctrl_inp_gain_mode);
	uint8_send(state, param->agc_adc_large_overload_exceed_counter);
	uint8_send(state, param->agc_adc_large_overload_inc_steps);
	uint8_send(state, param->agc_adc_lmt_small_overload_prevent_gain_inc_enable);
	uint8_send(state, param->agc_adc_small_overload_exceed_counter);
	uint8_send(state, param->agc_dig_gain_step_size);
	uint8_send(state, param->agc_dig_saturation_exceed_counter);
	uint32_send(state, param->agc_gain_update_interval_us);
	uint8_send(state, param->agc_immed_gain_change_if_large_adc_overload_enable);
	uint8_send(state, param->agc_immed_gain_change_if_large_lmt_overload_enable);
	uint8_send(state, param->agc_inner_thresh_high);
	uint8_send(state, param->agc_inner_thresh_high_dec_steps);
	uint8_send(state, param->agc_inner_thresh_low);
	uint8_send(state, param->agc_inner_thresh_low_inc_steps);
	uint8_send(state, param->agc_lmt_overload_large_exceed_counter);
	uint8_send(state, param->agc_lmt_overload_large_inc_steps);
	uint8_send(state, param->agc_lmt_overload_small_exceed_counter);
	uint8_send(state, param->agc_outer_thresh_high);
	uint8_send(state, param->agc_outer_thresh_high_dec_steps);
	uint8_send(state, param->agc_outer_thresh_low);
	uint8_send(state, param->agc_outer_thresh_low_inc_steps);
	uint32_send(state, param->agc_attack_delay_extra_margin_us);
	uint8_send(state, param->agc_sync_for_gain_counter_enable);
	uint32_send(state, param->fagc_dec_pow_measuremnt_duration);
	uint32_send(state, param->fagc_state_wait_time_ns);
	uint8_send(state, param->fagc_allow_agc_gain_increase);
	uint32_send(state, param->fagc_lp_thresh_increment_time);
	uint32_send(state, param->fagc_lp_thresh_increment_steps);
	uint32_send(state, param->fagc_lock_level);
	uint8_send(state, param->fagc_lock_level_lmt_gain_increase_en);
	uint32_send(state, param->fagc_lock_level_gain_increase_upper_limit);
	uint32_send(state, param->fagc_lpf_final_settling_steps);
	uint32_send(state, param->fagc_lmt_final_settling_steps);
	uint32_send(state, param->fagc_final_overrange_count);
	uint8_send(state, param->fagc_gain_increase_after_gain_lock_en);
	uint32_send(state, param->fagc_gain_index_type_after_exit_rx_mode);
	uint8_send(state, param->fagc_use_last_lock_level_for_set_gain_en);
	uint8_send(state, param->fagc_rst_gla_stronger_sig_thresh_exceeded_en);
	uint32_send(state, param->fagc_optimized_gain_offset);
	uint32_send(state, param->fagc_rst_gla_stronger_sig_thresh_above_ll);
	uint8_send(state, param->fagc_rst_gla_engergy_lost_sig_thresh_exceeded_en);
	uint8_send(state, param->fagc_rst_gla_engergy_lost_goto_optim_gain_en);
	uint32_send(state, param->fagc_rst_gla_engergy_lost_sig_thresh_below_ll);
	uint32_send(state, param->fagc_energy_lost_stronger_sig_gain_lock_exit_cnt);
	uint8_send(state, param->fagc_rst_gla_large_adc_overload_en);
	uint8_send(state, param->fagc_rst_gla_large_lmt_overload_en);
	uint8_send(state, param->fagc_rst_gla_en_agc_pulled_high_en);
	uint32_send(state, param->fagc_rst_gla_if_en_agc_pulled_high_mode);
	uint32_send(state, param->fagc_power_measurement_duration_in_state5);
	uint32_send(state, param->rssi_delay);
	uint32_send(state, param->rssi_duration);
	uint8_send(state, param->rssi_restart_mode);
	uint8_send(state, param->rssi_unit_is_rx_samples_enable);
	uint32_send(state, param->rssi_wait);
	uint32_send(state, param->aux_adc_decimation);
	uint32_send(state, param->aux_adc_rate);
	uint8_send(state, param->aux_dac_manual_mode_enable);
	uint32_send(state, param->aux_dac1_default_value_mV);
	uint8_send(state, param->aux_dac1_active_in_rx_enable);
	uint8_send(state, param->aux_dac1_active_in_tx_enable);
	uint8_send(state, param->aux_dac1_active_in_alert_enable);
	uint32_send(state, param->aux_dac1_rx_delay_us);
	uint32_send(state, param->aux_dac1_tx_delay_us);
	uint32_send(state, param->aux_dac2_default_value_mV);
	uint8_send(state, param->aux_dac2_active_in_rx_enable);
	uint8_send(state, param->aux_dac2_active_in_tx_enable);
	uint8_send(state, param->aux_dac2_active_in_alert_enable);
	uint32_send(state, param->aux_dac2_rx_delay_us);
	uint32_send(state, param->aux_dac2_tx_delay_us);
	uint32_send(state, param->temp_sense_decimation);
	uint16_send(state, param->temp_sense_measurement_interval_ms);
	int8_send(state, param->temp_sense_offset_signed);
	uint8_send(state, param->temp_sense_periodic_measurement_enable);
	uint8_send(state, param->ctrl_outs_enable_mask);
	uint8_send(state, param->ctrl_outs_index);
	uint32_send(state, param->elna_settling_delay_ns);
	uint32_send(state, param->elna_gain_mdB);
	uint32_send(state, param->elna_bypass_loss_mdB);
	uint8_send(state, param->elna_rx1_gpo0_control_enable);
	uint8_send(state, param->elna_rx2_gpo1_control_enable);
	uint8_send(state, param->pp_tx_swap_enable);
	uint8_send(state, param->pp_rx_swap_enable);
	uint8_send(state, param->tx_channel_swap_enable);
	uint8_send(state, param->rx_channel_swap_enable);
	uint8_send(state, param->rx_frame_pulse_mode_enable);
	uint8_send(state, param->two_t_two_r_timing_enable);
	uint8_send(state, param->invert_data_bus_enable);
	uint8_send(state, param->invert_data_clk_enable);
	uint8_send(state, param->fdd_alt_word_order_enable);
	uint8_send(state, param->invert_rx_frame_enable);
	uint8_send(state, param->fdd_rx_rate_2tx_enable);
	uint8_send(state, param->swap_ports_enable);
	uint8_send(state, param->single_data_rate_enable);
	uint8_send(state, param->lvds_mode_enable);
	uint8_send(state, param->half_duplex_mode_enable);
	uint8_send(state, param->single_port_mode_enable);
	uint8_send(state, param->full_port_enable);
	uint8_send(state, param->full_duplex_swap_bits_enable);
	uint32_send(state, param->delay_rx_data);
	uint32_send(state, param->rx_data_clock_delay);
	uint32_send(state, param->rx_data_delay);
	uint32_send(state, param->tx_fb_clock_delay);
	uint32_send(state, param->tx_data_delay);
	uint32_send(state, param->lvds_bias_mV);
	uint8_send(state, param->lvds_rx_onchip_termination_enable);
	uint8_send(state, param->rx1rx2_phase_inversion_en);
	uint32_send(state, param->low_high_gain_threshold_mdB);
	uint32_send(state, param->low_gain_dB);
	uint32_send(state, param->high_gain_dB);
	uint8_send(state, param->tx_mon_track_en);
	uint8_send(state, param->one_shot_mode_en);
	uint32_send(state, param->tx_mon_delay);
	uint32_send(state, param->tx_mon_duration);
	uint32_send(state, param->tx1_mon_front_end_gain);
	uint32_send(state, param->tx2_mon_front_end_gain);
	uint32_send(state, param->tx1_mon_lo_cm);
	uint32_send(state, param->tx2_mon_lo_cm);
	int32_send(state, param->gpio_resetb);
	int32_send(state, param->gpio_sync);
	int32_send(state, param->gpio_cal_sw1);
	int32_send(state, param->gpio_cal_sw2);

    return OK;
}

Result fmcomms3_rf_rssi_struct_recv(connection_state * state, struct rf_rssi * rssi)
{
    rssi->ant = uint32_recv(state);
    rssi->symbol = uint32_recv(state);
    rssi->preamble = uint32_recv(state);
    rssi->multiplier = int32_recv(state);
    rssi->duration = uint8_recv(state);

    return OK;
}

Result fmcomms3_rf_rssi_struct_send(connection_state * state, struct rf_rssi * rssi)
{
    uint32_send(state, rssi->ant);
    uint32_send(state, rssi->symbol);
    uint32_send(state, rssi->preamble);
    int32_send(state, rssi->multiplier);
    uint8_send(state, rssi->duration);

    return OK;
}

Result fmcomms3_rxfirconfig_struct_recv(connection_state * state, AD9361_RXFIRConfig * config)
{
    int i;

    config->rx = uint32_recv(state);
    config->rx_gain = int32_recv(state);
    config->rx_dec = uint32_recv(state);

    for(i=0 ;i<128; i++) config->rx_coef[i] = int16_recv(state);

    config->rx_coef_size = uint8_recv(state);

    return OK;
}

Result fmcomms3_rxfirconfig_struct_send(connection_state * state, AD9361_RXFIRConfig * config)
{
    int i;

    uint32_send(state, config->rx);
    int32_send(state, config->rx_gain);
    uint32_send(state, config->rx_dec);

    for(i=0 ;i<128; i++) int16_send(state, config->rx_coef[i]);

    uint8_send(state, config->rx_coef_size);

    return OK;
}

Result fmcomms3_txfirconfig_struct_recv(connection_state * state, AD9361_TXFIRConfig * config)
{
    int i;

    config->tx = uint32_recv(state);
    config->tx_gain = int32_recv(state);
    config->tx_int = uint32_recv(state);

    for(i=0 ;i<128; i++) config->tx_coef[i] = int16_recv(state);

    config->tx_coef_size = uint8_recv(state);

    return OK;
}

Result fmcomms3_txfirconfig_struct_send(connection_state * state, AD9361_TXFIRConfig * config)
{
    int i;

    uint32_send(state, config->tx);
    int32_send(state, config->tx_gain);
    uint32_send(state, config->tx_int);

    for(i=0 ;i<128; i++) int16_send(state, config->tx_coef[i]);

    uint8_send(state, config->tx_coef_size);

    return OK;
}

Result fmcomms3_calib_result_struct_recv(connection_state * state, struct sAd9361CalibResult * pCalibResult)
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

Result fmcomms3_calib_result_struct_send(connection_state * state, struct sAd9361CalibResult * pCalibResult)
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
