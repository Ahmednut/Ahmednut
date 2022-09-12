#ifndef __CCE_TIMESTAMP__
#define __CCE_TIMESTAMP__

#include "connection_state.h"

Result Timestamp_Presence_recv(connection_state * state);
Result Timestamp_SetTime_recv(connection_state * state);
Result Timestamp_GetTime_recv(connection_state * state);
Result Timestamp_Reset_recv(connection_state * state);

#endif
