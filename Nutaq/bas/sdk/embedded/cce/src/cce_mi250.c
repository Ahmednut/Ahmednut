
#include <string.h>
#include "bas_debug.h"
#include "eapi_serialize.h"

#include "cce_mi250.h"
#include "mi250.h"
#include "mi250_defines.h"
#include "mi250_types.h"
#include "mi250_user.h"
#include "mi250_pll.h"
#include "mi250_serialize.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

extern Carrier_handle_t hCarrier;
extern MI250Board_t * mi250_fmcboard[];
extern int mi250_selected_inst;

static FMCEEprom_module_types _modlst[]={FMCEEPROM_EEPROM_MOD_MI250};

Result _Mi250_Presence()
{
    Result res;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;

    res = Carrier_FmcValidatePresence(hCarrier,
        mi250_selected_inst, Carrier_eFmcBoard0,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if (FAILURE(res)) return res;

    if (mdetected == CARRIER_NOT_DETECTED) return CCEINVALIDMOD;

    return OK;
}

Result MI250_Select_recv(connection_state * state)
{
    unsigned uConnectorNum;
    Result res;
    unsigned int uiFmcMaxNum;

    uConnectorNum = uint_recv(state);

    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(res)) return res;

    if(uConnectorNum >= (uiFmcMaxNum / CARRIER_FMC_MAXBOARD))
    {
        return CCEINVALIDVALUE;
    }

    mi250_selected_inst = uConnectorNum;

    return OK;
}

Result MI250_Init_recv(connection_state * state)
{
    Result res = OK;

    debug("ADC init\n", 0);

    res = _Mi250_Presence();
    if(FAILURE(res)) return res;
    
    res = MI250_ADC_init(mi250_fmcboard[mi250_selected_inst]);

    return res;
}

Result MI250_Powerup_recv(connection_state * state)
{
    Result res;
    debug("setting VAdj to %x\n", VADJ_2_503V);
    
    res = Carrier_FmcPowerUp(hCarrier,
        mi250_selected_inst, Carrier_eFmcBoard0,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        VADJ_2_503V);
    if (FAILURE(res)) return res;
    
    return OK;
}

Result MI250_Presence_recv(connection_state * state)
{    
    debug("Verifying presence\n",0);
      
    return _Mi250_Presence();
}

Result MI250_ReadSPIRegisters_recv(connection_state * state)
{
    Result res;
    MI250_DEVICES DeviceId;
    int BaseRegister;
    char value = 0;
    
    DeviceId = uint_recv(state);
    BaseRegister = uint_recv(state);

    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        uchar_send(state, value);
        return res;
    }
    
    res = MI250_ReadSPIRegisters(mi250_fmcboard[mi250_selected_inst], DeviceId, BaseRegister, 1, &value);

    debug("ReadSPI %d %d %d\n",DeviceId,BaseRegister,value);
    uchar_send(state, value);
    return res;
}

Result MI250_WriteSPIRegisters_recv(connection_state * state)
{
    MI250_DEVICES DeviceId;
    int BaseRegister;
    char value;
    Result res;

    DeviceId = uint_recv(state);
    BaseRegister = uint_recv(state);
    value = uchar_recv(state);    
    debug("WriteSPI %d %d %d\n",DeviceId,BaseRegister,value);

    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        uchar_send(state, value);
        return res;
    }
    
    res = MI250_WriteSPIRegisters(mi250_fmcboard[mi250_selected_inst], DeviceId, BaseRegister, 1, &value);
    
    return res;
}

Result MI250_PLLConfig_recv(connection_state * state)
{
    Result res;
	MI250_CLOCK_SOURCE clocksource;
	unsigned int acqclk;
	MI250_PLL_PARAM param;
    
    clocksource = uint_recv(state);
	acqclk = uint_recv(state);
    debug("PLL Config %d \n", clocksource);
    debug("AcqClk %d \n", acqclk);

	MI250_pll_param_recv(state, &param);
	debug("Afactor: %i\n", param.Afactor);
	debug("Bfactor: %i\n", param.Bfactor);
	debug("Rfactor: %i\n", param.Rfactor);
	debug("Pfactor: %i\n", param.Pfactor);
	debug("VCOFreq: %i\n", param.VCOFreq);
	debug("divisorADCClk: %i\n", param.divisorADCClk);
	debug("divisorClkOutClk: %i\n", param.divisorClkOutClk);
	debug("divisorRsvdClk: %i\n", param.divisorRsvdClk);

    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    if(acqclk < MI250_MIN_ADC_PLL_CLK_FREQ)
    {
        debug("AcqClk too low \n", acqclk);
        return (MI250_NO_VALID_PLL_SOLUTION);
    }
    
    switch(clocksource)
    {
        case MI250_CLOCKIN_100MHZ:
          // If internal reference clock to be used
          res = MI250_PLL_Init(mi250_fmcboard[mi250_selected_inst]);
          if(res)
          {
            return res;
          }
          break;
        case MI250_CLOCKIN_EXTERNALREF:
          // If external reference clock to be used
          res = MI250_PLL_Init_Extern_Ref(mi250_fmcboard[mi250_selected_inst]);
          if(res)
          {
            return res;
          }
          break;
        case MI250_CLOCKIN_EXTERNALCLOCK:
          // If external sampling clock to be used
          res = MI250_PLL_Init_Extern(mi250_fmcboard[mi250_selected_inst]);
          if(res)
          {
            return res;
          }
          break;
    }
    
    res = MI250_PLL_SetConfig(mi250_fmcboard[mi250_selected_inst], param);
    if(FAILURE(res)) return res;
    
    res = MI250_PLL_Calibrate(mi250_fmcboard[mi250_selected_inst], acqclk);

	return res;
}	
	
Result MI250_ADCreset_recv(connection_state * state)
{
    Result res;

	//ADC reset 	  
    MI250Adaptator_t mi250ad;
	
	// Reset the MMCMs
	memset(&mi250ad, 0, sizeof(mi250ad));

    debug("ADC reset!\n",0);
	
    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    //ADC reset 
    mi250ad.ADC_12_Control.ADC_MmcmRst = 1;
    mi250ad.ADC_34_Control.ADC_MmcmRst = 1;
    mi250ad.ADC_56_Control.ADC_MmcmRst = 1;
    mi250ad.ADC_78_Control.ADC_MmcmRst = 1;
    mi250ad.ADC_Control.IoReset = 1;
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_12_CONTROL, mi250ad.ADC_12_Control.Register );
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_34_CONTROL, mi250ad.ADC_34_Control.Register );
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_56_CONTROL, mi250ad.ADC_56_Control.Register );
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_78_CONTROL, mi250ad.ADC_78_Control.Register );
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_CONTROL, mi250ad.ADC_Control.Register );
    // add delay for reset
    usleep(20);
    mi250ad.ADC_12_Control.ADC_MmcmRst = 0;
    mi250ad.ADC_34_Control.ADC_MmcmRst = 0;
    mi250ad.ADC_56_Control.ADC_MmcmRst = 0;
    mi250ad.ADC_78_Control.ADC_MmcmRst = 0;
    mi250ad.ADC_Control.IoReset = 0;
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_12_CONTROL, mi250ad.ADC_12_Control.Register );
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_34_CONTROL, mi250ad.ADC_34_Control.Register );
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_56_CONTROL, mi250ad.ADC_56_Control.Register );
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_78_CONTROL, mi250ad.ADC_78_Control.Register );
    MI250_WriteControlRegister( mi250_fmcboard[mi250_selected_inst], MI250_ADC_CONTROL, mi250ad.ADC_Control.Register );

    return OK;
}

Result MI250_PLLGetStatus_recv(connection_state * state)
{
    Result res;
	MI250_PLL_STATUS pllstatus = MI250_PLL_UNLOCKED;

    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        uint_send(state, pllstatus);
        return res;
    }
    
    res = MI250_PLL_getStatus(mi250_fmcboard[mi250_selected_inst],  &pllstatus);

    debug("PLL status %d \n", pllstatus);
    uint_send(state, pllstatus);
    return res;
}

Result MI250_SetDataFormat_recv(connection_state * state)
{
    MI250_DEVICES device;
    MI250_ADC_DATAFORMAT format;
    Result res;
    
    device = uint_recv(state);
    format = uint_recv(state);

    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
	res = MI250_ADC_SetDataFormat(mi250_fmcboard[mi250_selected_inst], device, format);

	return res;
}

Result MI250_SetSleepMode_recv(connection_state * state)
{
    MI250_DEVICES device;
    MI250_ADC_SLEEP_MODE sleep;
    Result res;

    device = uint_recv(state);
    sleep = uint_recv(state);
	
    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = MI250_ADC_SetSleepMode(mi250_fmcboard[mi250_selected_inst], device, sleep);
    
    return res;
}

Result MI250_SetOCT_recv(connection_state * state)
{
    MI250_DEVICES device;
    MI250_CHANNEL_OCT octa;
    MI250_CHANNEL_OCT octb;
    Result res;
    
    device = uint_recv(state);
    octa = uint_recv(state);
    octb = uint_recv(state);
	
    res = _Mi250_Presence();
    if(FAILURE(res)) return res;
    
    res = MI250_ADC_SetOffsetCorrectionTime(mi250_fmcboard[mi250_selected_inst], device, octa, octb);
	
    return res;
}

Result MI250_SetGain_recv(connection_state * state)
{
    MI250_ADC_CHANNEL channel;
    MI250_DEVICES device;
    MI250_ADC_GAIN gain;
    Result res;
    
    channel = uint_recv(state);
    device = uint_recv(state);
    gain = uint_recv(state);    
    debug("ADC Set Gain \n",0);    
    debug("channel%d \n", channel); 
    debug("device %d \n", device );     
    debug("gain %d \n", gain ); 

    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = MI250_ADC_setGain(mi250_fmcboard[mi250_selected_inst], channel, gain, device);
    
    return res;
}

Result MI250_SetFineGain_recv(connection_state * state)
{
    MI250_ADC_CHANNEL channel;
    MI250_DEVICES device;
    char gain;
    Result res;
       
    channel = uint_recv(state);
    device = uint_recv(state);
    gain = uchar_recv(state);    
    debug("ADC Set Fine Gain \n",0);    
    debug("channel%d \n", channel); 
    debug("device %d \n", device );  
    debug("gain %d \n", gain ); 

    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = MI250_ADC_setFineGain(mi250_fmcboard[mi250_selected_inst], channel, gain, device);
    
    return res;
}

Result MI250_SetPedestal_recv(connection_state * state)
{
    MI250_ADC_CHANNEL channel;
    MI250_DEVICES device;
    char value;
    Result res;

    channel = uint_recv(state);
    device = uint_recv(state);
    value = uchar_recv(state); 
    debug("ADC Set Pedestal \n",0);    
    debug("channel%d \n", channel); 
    debug("device %d \n", device );  
    debug("value %d \n", value ); 
    
    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = MI250_ADC_setPedestal(mi250_fmcboard[mi250_selected_inst], channel, value, device);
    
    return res;
}

Result MI250_ArmADC_recv(connection_state * state)
{
    MI250_ADC_ARMED_STATUS status;
    Result res;

    status = uint_recv(state);
    debug("ARM ADC %d\n",status);    

    res = _Mi250_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = MI250_ADC_arm(mi250_fmcboard[mi250_selected_inst], status); 
    
    return res;
}
