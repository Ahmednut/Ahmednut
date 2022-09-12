#ifndef __CCE_FMCRADIO__
#define __CCE_FMCRADIO__

#include "connection_state.h"

Result fmc_radio_presence(connection_state * state);
Result fmc_radio_powerup_recv(connection_state * state);
Result fmc_radio_sdr_reset_recv(connection_state * state);
Result fmc_radio_sdr_select_recv(connection_state * state);
Result fmc_radio_spi_write_recv(connection_state * state);
Result fmc_radio_spi_read_recv(connection_state * state);
Result fmc_radio_pll_setparam_recv(connection_state * state);
Result fmc_radio_pll_lockstatus_recv(connection_state * state);
Result fmc_radio_pll_waitforlock_recv(connection_state * state);
Result fmc_radio_clockmux_set_recv(connection_state * state);
Result fmc_radio_lime_setpllparam_recv(connection_state * state);
Result fmc_radio_lime_calibratePLL_recv(connection_state * state);
Result fmc_radio_lime_reset_recv(connection_state * state);
Result fmc_radio_sdr_band_recv(connection_state * state);
Result fmc_radio_sdr_rx_gain_recv(connection_state * state);
Result fmc_radio_rxvga_calibrate_recv(connection_state * state);
Result fmc_radio_tx_gain_recv(connection_state * state);
Result fmc_radio_lime_setrxfilter_recv(connection_state * state);
Result fmc_radio_lpf_set_recv(connection_state * state);
Result fmc_radio_lpf_calibrate_recv(connection_state * state);
Result fmc_radio_setIOexp_recv(connection_state * state);
Result fmc_radio_path_enable_recv(connection_state * state);
Result fmc_radio_path_disable_recv(connection_state * state);
Result fmc_radio_rx_connect_recv(connection_state * state);
Result fmc_radio_rx_disconnect_recv(connection_state * state);
Result fmc_radio_loopback_recv(connection_state * state);
Result fmc_radio_lo_leakage_calibration_recv(connection_state * state);
Result fmc_radio_lo_leakage_calibration_set_recv(connection_state * state);
Result fmc_radio_ssb_calibration_recv(connection_state * state);
Result fmc_radio_ssb_calibration_set_recv(connection_state * state);
Result fmc_radio_rx_dc_offset_calibration_recv(connection_state * state);
Result fmc_radio_rx_dc_offset_calibration_set_recv(connection_state * state);
Result fmc_radio_setrevision_recv(connection_state * state);
Result fmc_radio_spi_control_set_recv(connection_state * state);

#endif
