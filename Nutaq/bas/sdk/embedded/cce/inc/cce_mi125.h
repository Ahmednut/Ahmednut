#ifndef __CCE_MI125__
#define __CCE_MI125__
#include "cce.h"
#include "bas_error.h"


Result MI125_Presence_recv(connection_state * state);

Result MI125_powerup_recv(connection_state * state);

Result MI125_mi125_reset_recv(connection_state * state);

Result MI125_mi125_digital_adccalibration_recv(connection_state * state);

Result MI125_mi125_set_config_recv(connection_state * state);

Result MI125_mi125_set_clksrc_recv(connection_state * state);

Result MI125_mi125_checkcore_recv(connection_state * state);

Result MI125_mi125_set_testmode_recv(connection_state * state);

Result MI125_mi125_get_temperature_recv(connection_state * state);

Result MI125_mi125_get_channelcalibstatus_recv(connection_state * state);

Result MI125_mi125_get_versions_recv(connection_state * state);

Result MI125_mi125_checkadc_recv(connection_state * state);

Result MI125_mi125_set_trigout_recv(connection_state * state);

Result MI125_mi125_get_clkmaster_recv(connection_state * state);

Result MI125_mi125_clockreset_recv(connection_state * state);

Result MI125_mi125_scan_i2c_recv(connection_state * state);

Result MI125_mi125_test_localoscsw_recv(connection_state * state);

#endif
