#ifndef __CCE_ADAC250__
#define __CCE_ADAC250__

#include "cce.h"
#include "bas_error.h"
#include "connection_state.h"

Result ADAC250_Presence_recv(connection_state * state);
Result ADAC250_PowerUp_recv(connection_state * state);

Result ADAC250_ReadSPIRegisters_recv(connection_state * state);
Result ADAC250_WriteSPIRegisters_recv(connection_state * state);

Result ADAC250_DAC_SetGain_recv(connection_state * state);
Result ADAC250_DAC_SetOffset_recv(connection_state * state);
Result ADAC250_DAC_SetDDSFreq_recv(connection_state * state);
Result ADAC250_DAC_SetDDSDownSampler_recv(connection_state * state);
Result ADAC250_DAC_SetDataSource_recv(connection_state * state);
Result ADAC250_DAC_Init_recv(connection_state * state);
Result ADAC250_DAC_Sync_recv(connection_state * state);
Result ADAC250_DAC_Calibrate_recv(connection_state * state);

Result ADAC250_PLL_Init_recv(connection_state * state);
Result ADAC250_PLL_CalculateParams_recv(connection_state * state);
Result ADAC250_PLL_SetConfig_recv(connection_state * state);
Result ADAC250_PLL_trigRegisterTransfert_recv(connection_state * state);
Result ADAC250_DAC_SyncSource_recv(connection_state * state);
Result ADAC250_PLL_SyncOutput_recv(connection_state * state);
Result ADAC250_PLL_getStatus_recv(connection_state * state);
Result ADAC250_PLL_SetSleep_recv(connection_state * state);
Result ADAC250_PLL_setRefClkTuning_recv(connection_state * state);

Result ADAC250_ADC_init_recv(connection_state * state);
Result ADAC250_ADC_SetConfig_recv(connection_state * state);
Result ADAC250_ADC_setGain_recv(connection_state * state);
Result ADAC250_ADC_setFineGain_recv(connection_state * state);
Result ADAC250_ADC_setPedestal_recv(connection_state * state);
Result ADAC250_ADC_getStatus_recv(connection_state * state);
Result ADAC250_ADC_Calibrate_recv(connection_state * state);
Result ADAC250_ADC_SetDataFormat_recv(connection_state * state);
Result ADAC250_ADC_SetSleepMode_recv(connection_state * state);
Result ADAC250_ADC_SetOCT_recv(connection_state * state);
Result ADAC250_ADC_SetTriggerDelay_recv(connection_state * state);

Result ADAC250_MUX_ConfigClockOutput_recv(connection_state * state);

#endif
