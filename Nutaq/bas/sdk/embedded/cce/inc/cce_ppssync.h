#ifndef __CCE_PPSSYNC__
#define __CCE_PPSSYNC__

#ifdef __cplusplus
extern "C" {
#endif

#include "cce.h"
#include "bas_error.h"


Result PPSSync_Presence_recv(connection_state * state);
Result PPSSync_Configure_recv(connection_state * state);
Result PPSSync_DacInit_recv(connection_state * state);
Result PPSSync_Start_recv(connection_state * state);
Result PPSSync_Stop_recv(connection_state * state);
Result PPSSync_ReadRefDacVal_recv(connection_state * state);
Result PPSSync_SaveRefDacVal_recv(connection_state * state);
Result PPSSync_ReadPIProfile_recv(connection_state * state);
Result PPSSync_SavePIProfile_recv(connection_state * state);
Result PPSSync_GetInfo_recv(connection_state * state);

#ifdef __cplusplus
}
#endif

#endif
