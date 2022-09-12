#ifndef __CCE_FMCLVDS__
#define __CCE_FMCLVDS__

#include "connection_state.h"
#include "fmclvds.h"
//extern unsigned selected_fmclvds;
//extern fmclvds fmclvds_handle[];

Result fmclvds_init_recv(connection_state * state);
Result fmclvds_powerup_recv(connection_state * state);
Result fmclvds_presence_recv(connection_state * state);
Result fmclvds_setrevision_recv(connection_state * state);
Result fmclvds_select_send(connection_state * state);
Result fmclvds_setdir_recv(connection_state * state);
Result fmclvds_pwdn_recv(connection_state * state);
Result fmclvds_preemp_recv(connection_state * state);
#endif
