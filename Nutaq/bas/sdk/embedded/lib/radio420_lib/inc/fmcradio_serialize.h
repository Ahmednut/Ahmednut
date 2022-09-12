#ifndef __FMCRADIO_SERIALIZE__
#define __FMCRADIO_SERIALIZE__
#include "connection_state.h"
#include "radio420_cdce62005.h"
#include "lms6002.h"

Result cdce62005_param_send(connection_state * state, cdce62005_param * param);
Result cdce62005_param_recv(connection_state * state, cdce62005_param * param);
Result lms6002_pll_param_send(connection_state * state, lms6002_pll_param * param);
Result lms6002_pll_param_recv(connection_state * state, lms6002_pll_param * param);
Result lms6002_lo_leakage_calib_send(connection_state * state, lms6002_lo_leakage_calib * calib);
Result lms6002_lo_leakage_calib_recv(connection_state * state, lms6002_lo_leakage_calib * calib);
Result lms6002_ssb_calib_send(connection_state * state, lms6002_ssb_calib * calib);
Result lms6002_ssb_calib_recv(connection_state * state, lms6002_ssb_calib * calib);
Result lms6002_rx_dc_calib_send(connection_state * state, lms6002_rx_dc_calib * calib);
Result lms6002_rx_dc_calib_recv(connection_state * state, lms6002_rx_dc_calib * calib);

#endif
