#ifndef __CCE_FMCOMMS3__
#define __CCE_FMCOMMS3__
#include "cce.h"
#include "bas_error.h"

Result Fmcomms3_presence_recv(connection_state * state);
Result Fmcomms3_powerup_recv(connection_state * state);
Result Fmcomms3_init_recv(connection_state * state);
Result Fmcomms3_digital_calibration_recv(connection_state * state);
Result Fmcomms3_set_sync_delay_recv(connection_state * state);
Result Fmcomms3_set_refclk_delay_recv(connection_state * state);
Result Fmcomms3_set_sync_bitslip_recv(connection_state * state);
Result Fmcomms3_set_sync_src_recv(connection_state * state);
Result Fmcomms3_soft_sync_recv(connection_state * state);
Result Fmcomms3_mcs_recv(connection_state * state);
Result Fmcomms3_set_master_calib_result_recv(connection_state * state);
Result Fmcomms3_set_ext_master_calib_result_recv(connection_state * state);
Result Fmcomms3_get_calib_result_recv(connection_state * state);
Result Fmcomms3_set_en_state_machine_mode_recv(connection_state * state);
Result Fmcomms3_get_en_state_machine_mode_recv(connection_state * state);
Result Fmcomms3_set_sampling_freq_recv(connection_state * state);
Result Fmcomms3_get_sampling_freq_recv(connection_state * state);
Result Fmcomms3_set_tx_lo_freq_recv(connection_state * state);
Result Fmcomms3_get_tx_lo_freq_recv(connection_state * state);
Result Fmcomms3_set_rx_rf_gain_recv(connection_state * state);
Result Fmcomms3_get_rx_rf_gain_recv(connection_state * state);
Result Fmcomms3_set_rx_rf_bandwidth_recv(connection_state * state);
Result Fmcomms3_get_rx_rf_bandwidth_recv(connection_state * state);
Result Fmcomms3_set_rx_lo_freq_recv(connection_state * state);
Result Fmcomms3_get_rx_lo_freq_recv(connection_state * state);
Result Fmcomms3_get_rx_rssi_recv(connection_state * state);
Result Fmcomms3_set_rx_gain_control_mode_recv(connection_state * state);
Result Fmcomms3_get_rx_gain_control_mode_recv(connection_state * state);
Result Fmcomms3_set_rx_fir_config_recv(connection_state * state);
Result Fmcomms3_get_rx_fir_config_recv(connection_state * state);
Result Fmcomms3_set_rx_fir_en_dis_recv(connection_state * state);
Result Fmcomms3_get_rx_fir_en_dis_recv(connection_state * state);
Result Fmcomms3_set_rx_rfdc_track_en_dis_recv(connection_state * state);
Result Fmcomms3_get_rx_rfdc_track_en_dis_recv(connection_state * state);
Result Fmcomms3_set_rx_bbdc_track_en_dis_recv(connection_state * state);
Result Fmcomms3_get_rx_bbdc_track_en_dis_recv(connection_state * state);
Result Fmcomms3_set_rx_quad_track_en_dis_recv(connection_state * state);
Result Fmcomms3_get_rx_quad_track_en_dis_recv(connection_state * state);
Result Fmcomms3_set_tx_attenuation_recv(connection_state * state);
Result Fmcomms3_get_tx_attenuation_recv(connection_state * state);
Result Fmcomms3_set_tx_rf_bandwidth_recv(connection_state * state);
Result Fmcomms3_get_tx_rf_bandwidth_recv(connection_state * state);
Result Fmcomms3_set_tx_fir_config_recv(connection_state * state);
Result Fmcomms3_get_rx_fir_config_recv(connection_state * state);
Result Fmcomms3_set_tx_fir_en_dis_recv(connection_state * state);
Result Fmcomms3_get_tx_fir_en_dis_recv(connection_state * state);
Result Fmcomms3_set_trx_path_clks_recv(connection_state * state);
Result Fmcomms3_get_trx_path_clks_recv(connection_state * state);
Result Fmcomms3_set_dac_datasel_recv(connection_state * state);
Result Fmcomms3_get_dac_datasel_recv(connection_state * state);
Result Fmcomms3_set_dds_freq_recv(connection_state * state);
Result Fmcomms3_get_dds_freq_recv(connection_state * state);
Result Fmcomms3_set_dds_phase_recv(connection_state * state);
Result Fmcomms3_get_dds_phase_recv(connection_state * state);
Result Fmcomms3_set_dds_scale_recv(connection_state * state);
Result Fmcomms3_get_dds_scale_recv(connection_state * state);
Result Fmcomms3_spi_write_recv(connection_state * state);
Result Fmcomms3_spi_read_recv(connection_state * state);

#endif
