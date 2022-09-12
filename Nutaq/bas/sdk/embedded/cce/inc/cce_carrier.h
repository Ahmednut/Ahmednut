#ifndef __CCE_CARRIER__
#define __CCE_CARRIER__

#include "connection_state.h"
#include "bas_error.h"

Result Carrier_GetType_recv(connection_state * state);
Result Carrier_GetModuleBaseAddr_recv(connection_state * state);
Result Carrier_LedSetValue_recv(connection_state * state);
Result Carrier_LedGetValue_recv(connection_state * state);
Result Carrier_TClkSetState_recv(connection_state * state);
Result Carrier_TClkGetState_recv(connection_state * state);
Result Carrier_TClkSetTxSource_recv(connection_state * state);
Result Carrier_TClkGetTxSource_recv(connection_state * state);
Result Carrier_FmcBidirClkSetConnection_recv(connection_state * state);
Result Carrier_FmcBidirClkGetConnection_recv(connection_state * state);
Result Carrier_SetOscState_recv(connection_state * state);
Result Carrier_GetOscState_recv(connection_state * state);
Result Carrier_FClkASetConnection_recv(connection_state * state);
Result Carrier_FClkAGetConnection_recv(connection_state * state);
Result Carrier_I2cRead_recv(connection_state * state);
Result Carrier_I2cWrite_recv(connection_state * state);
Result Carrier_I2cReadNoWrData_recv(connection_state * state);


#endif
