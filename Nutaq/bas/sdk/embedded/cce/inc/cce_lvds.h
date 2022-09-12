#ifndef __CCE_LVDS__
#define __CCE_LVDS__

#include "connection_state.h"
#include "lvds.h"

#define LVDS_GROUP_MAX 4

Result lvds_presence_recv(connection_state * state);
Result lvds_getmode_recv(connection_state * state);
Result lvds_setoutputenable_recv(connection_state * state);
Result lvds_setvalue_recv(connection_state * state);
Result lvds_getvalue_recv(connection_state * state);
Result lvds_reset_recv(connection_state * state);
Result lvds_reset_fifo_recv(connection_state * state);

#endif
