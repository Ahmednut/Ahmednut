
#include "cce.h"
#include "eapi_serialize.h"
#include "bas_debug.h"

#include "cce_adc5000.h"
#include "adc5000_serialize.h"
#include "adc5000.h"
#include "adc5000_defines.h"
#include "ad9517_defines.h"
#include "ev10aq190_defines.h"
#include "adt7411_defines.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

extern Carrier_handle_t hCarrier;
extern adc5000 adc5000board[];
extern int adc5000_selected_inst;

static FMCEEprom_module_types _modlst[]={FMCEEPROM_EEPROM_MOD_ADC5000};

Result _ADC5000_Presence()
{
    Result ret;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;

    ret = Carrier_FmcValidatePresence(hCarrier,
        adc5000_selected_inst, Carrier_eFmcBoard0,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if (FAILURE(ret)) return ret;

    if (mdetected == CARRIER_NOT_DETECTED) return CCEINVALIDMOD;

    return OK;
}

Result ADC5000_Presence_recv(connection_state * state)
{
    return _ADC5000_Presence();
}

Result ADC5000_Powerup_recv(connection_state * state)
{
  Result ret;

  ret = Carrier_FmcPowerUp(hCarrier,
    adc5000_selected_inst, Carrier_eFmcBoard0,
    _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
    VADJ_2_503V);
  if (FAILURE(ret)) return ret;

  return ADC5000OK;
}

Result ADC5000_Reset_recv(connection_state * state)
{
  Result ret;

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }

  ret = adc5000_reset(&adc5000board[adc5000_selected_inst]);

  return ret;
}

Result ADC5000_SetMode_recv(connection_state * state)
{
  Result ret;
  ev10aq190_mode adcmode;

  adcmode = uint_recv(state);

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }

  ret = adc5000_setmode(&adc5000board[adc5000_selected_inst], adcmode);

  return ret;
}


Result ADC5000_SetPLLParam_recv(connection_state * state)
{
  Result ret;
  ad9517_param pllparameters;

  adc5000_pll_param_recv(state, &pllparameters);
  
  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_pll_setparam(&adc5000board[adc5000_selected_inst], &pllparameters);

  return ret;
}

Result ADC5000_SetSync_recv(connection_state * state)
{
  Result ret;
  adc5000_sync_source sync_source;

  sync_source = uint_recv(state);
  
  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_setsync(&adc5000board[adc5000_selected_inst],sync_source);

  return ret;
}

Result ADC5000_PLLLock_recv(connection_state * state)
{
  Result ret;
  ad9517_lock_status status = AD9517_PLL_UNLOCKED;

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    uint_send(state, status);
    return ret;
  }
  
  ret = adc5000_pll_lock(&adc5000board[adc5000_selected_inst],  &status);

  uint_send(state, status);
  
  return ret;
}

/*
Result ADC5000_ReadTemperature_recv(connection_state * state)
{
  Result ret;
  adc5000_sensor sensor;
  float temp =0.0;
  unsigned int * pTemp;

  pTemp = &temp;
  sensor = uint_recv(state);

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    uint_send(state, 0);
    return ret;
  }
  
  ret = adc5000_read_temperature(&adc5000board[adc5000_selected_inst],  sensor, &temp);

  uint_send(state, *pTemp);
  return ret;
}
*/

Result ADC5000_SetTestMode_recv(connection_state * state)
{
  Result ret;
  ev10aq190_testmode testmode;

  testmode = uint_recv(state);

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_settestmode(&adc5000board[adc5000_selected_inst],  testmode);

  return ret;
}

Result ADC5000_CalibrateIoDelay_recv(connection_state * state)
{
  Result ret;

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_calibrateiodelay(&adc5000board[adc5000_selected_inst]);

  return ret;
}

Result ADC5000_GetAdcStatus_recv(connection_state * state)
{
  Result ret;
  unsigned char adc_A_active = 0;
  unsigned char adc_B_active = 0;
  unsigned char adc_C_active = 0;
  unsigned char adc_D_active = 0;
  unsigned int val = 0;


  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    uint_send(state, val);
    return ret;
  }
  
  ret = adc5000_getadcstatus(&adc5000board[adc5000_selected_inst], &adc_A_active, &adc_B_active, &adc_C_active, &adc_D_active);
  val = (adc_D_active << 24) | (adc_C_active << 16) | (adc_B_active << 8) | (adc_A_active << 0);

  uint_send(state, val);

  return ret;
}

Result ADC5000_SetStandby_recv(connection_state * state)
{
  Result ret;
  ev10aq190_standby standby;

  standby = (ev10aq190_standby) uint_recv(state);

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_setstandby(&adc5000board[adc5000_selected_inst], standby);

  return ret;
}

Result ADC5000_SetCoding_recv(connection_state * state)
{
  Result ret;
  ev10aq190_coding coding;

  coding = (ev10aq190_coding) uint_recv(state);

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_setcoding(&adc5000board[adc5000_selected_inst], coding);

  return ret;
}

Result ADC5000_SetBandwidth_recv(connection_state * state)
{
  Result ret;
  ev10aq190_bandwidth bandwidth;

  bandwidth = (ev10aq190_bandwidth) uint_recv(state);

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_setbandwidth(&adc5000board[adc5000_selected_inst], bandwidth);

  return ret;
}

Result ADC5000_SetAdjTriggerDelay_recv(connection_state * state)
{
  Result ret;
  char delay;

  delay = (char) uint_recv(state);

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_setadjtriggerdelay(&adc5000board[adc5000_selected_inst], delay);

  return ret;
}

Result ADC5000_SetOffset_recv(connection_state * state)
{
  Result ret;
  unsigned int temp;
  ev10aq190_channel channel;
  unsigned short offset;

  temp = uint_recv(state);
  channel = (ev10aq190_channel)((temp >> 16) & 0xFF);
  offset = temp & 0xFFFF;

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_setoffset(&adc5000board[adc5000_selected_inst], channel, offset);

  return ret;
}

Result ADC5000_SetGain_recv(connection_state * state)
{
  Result ret;
  unsigned int temp;
  ev10aq190_channel channel;
  unsigned short gain;

  temp = uint_recv(state);
  channel = (ev10aq190_channel)((temp >> 16) & 0xFF);
  gain = temp & 0xFFFF;

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_setgain(&adc5000board[adc5000_selected_inst], channel, gain);

  return ret;
}

Result ADC5000_SetPhase_recv(connection_state * state)
{
  Result ret;
  unsigned int temp;
  ev10aq190_channel channel;
  unsigned short phase;

  temp = uint_recv(state);
  channel = (ev10aq190_channel)((temp >> 16) & 0xFF);
  phase = temp & 0xFFFF;

  ret = _ADC5000_Presence();
  if(FAILURE(ret))
  {
    return ret;
  }
  
  ret = adc5000_setphase(&adc5000board[adc5000_selected_inst], channel, phase);

  return ret;
}



