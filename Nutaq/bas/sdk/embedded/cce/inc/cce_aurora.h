#ifndef __CCE_AURORA__
#define __CCE_AURORA__

#include "connection_state.h"
#include "bas_error.h"

Result Aurora_GetVersion_recv(connection_state * state);
Result Aurora_ResetCore_recv(connection_state * state);
Result Aurora_GetChannelStatus_recv(connection_state * state);
Result Aurora_ResetRxFifo_recv(connection_state * state);
Result Aurora_ResetTxFifo_recv(connection_state * state);
Result Aurora_GetRxDataCount_recv(connection_state * state);
Result Aurora_GetTxDataCount_recv(connection_state * state);
Result Aurora_GetRxDataRate_recv(connection_state * state);
Result Aurora_GetTxDataRate_recv(connection_state * state);
Result Aurora_SetGtxTxParam_recv(connection_state * state);
Result Aurora_GetGtxTxParam_recv(connection_state * state);
Result Aurora_SetGtxRxParam_recv(connection_state * state);
Result Aurora_GetGtxRxParam_recv(connection_state * state);
Result Aurora_GetDfeEyeDacMon_recv(connection_state * state);
Result Aurora_SetChannelEnableState_recv(connection_state * state);
Result Aurora_GetChannelEnableState_recv(connection_state * state);

#endif
