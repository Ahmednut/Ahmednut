#ifndef __CCE_ADC5000__
#define __CCE_ADC5000__
#include "cce.h"
#include "bas_error.h"
#include "ad9517_defines.h"
#include "ev10aq190_defines.h"
#include "adt7411_defines.h"

Result ADC5000_Presence_recv(connection_state * state);
Result ADC5000_Powerup_recv(connection_state * state);
Result ADC5000_Reset_recv(connection_state * state);
Result ADC5000_SetMode_recv(connection_state * state);
Result ADC5000_SetPLLParam_recv(connection_state * state);
Result ADC5000_SetSync_recv(connection_state * state);
Result ADC5000_PLLLock_recv(connection_state * state);
//Result ADC5000_ReadTemperature_recv(connection_state * state);
Result ADC5000_SetTestMode_recv(connection_state * state);
Result ADC5000_CalibrateIoDelay_recv(connection_state * state);
Result ADC5000_GetAdcStatus_recv(connection_state * state);
Result ADC5000_SetStandby_recv(connection_state * state);
Result ADC5000_SetCoding_recv(connection_state * state);
Result ADC5000_SetBandwidth_recv(connection_state * state);
Result ADC5000_SetAdjTriggerDelay_recv(connection_state * state);
Result ADC5000_SetOffset_recv(connection_state * state);
Result ADC5000_SetGain_recv(connection_state * state);
Result ADC5000_SetPhase_recv(connection_state * state);

#endif
