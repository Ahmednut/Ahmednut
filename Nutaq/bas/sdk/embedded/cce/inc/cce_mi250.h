#ifndef __CCE_MI250__
#define __CCE_MI250__

#include "cce.h"
#include "bas_error.h"
#include "connection_state.h"

Result MI250_Init_recv(connection_state * state);
Result MI250_ADCreset_recv(connection_state * state);
Result MI250_Presence_recv(connection_state * state);
Result MI250_Powerup_recv(connection_state * state);
Result MI250_ReadSPIRegisters_recv(connection_state * state);
Result MI250_WriteSPIRegisters_recv(connection_state * state);
Result MI250_PLLConfig_recv(connection_state * state);
Result MI250_PLLGetStatus_recv(connection_state * state);
Result MI250_SetDataFormat_recv(connection_state * state);
Result MI250_SetSleepMode_recv(connection_state * state);
Result MI250_SetOCT_recv(connection_state * state);
Result MI250_SetConfig_recv(connection_state * state);
Result MI250_SetGain_recv(connection_state * state);
Result MI250_SetFineGain_recv(connection_state * state);
Result MI250_SetPedestal_recv(connection_state * state);
Result MI250_GetStatus_recv(connection_state * state);
Result MI250_ArmADC_recv(connection_state * state);
Result MI250_Select_recv(connection_state * state);

#endif
