
#include "cce_adac250.h"
#include "eapi_serialize.h"

#include "adac250_user.h"
#include "adac250_pll.h"
#include "adac250_serialize.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

//debug or no debug
#if DEBUG
#include <stdio.h>
#define debug(f, ...) printf(f, __VA_ARGS__)
#else
#define debug(f, ...)
#endif

extern Carrier_handle_t hCarrier;
extern ADAC250Board_t * adac250[];
extern int adac250_selected_inst;

static FMCEEprom_module_types _modlst[]={FMCEEPROM_EEPROM_MOD_ADAC250};


Result _ADAC250_Presence()
{
    Result ret;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;

    ret = Carrier_FmcValidatePresence(hCarrier,
        adac250_selected_inst, Carrier_eFmcBoard0,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if (FAILURE(ret)) return ret;

    if (mdetected == CARRIER_NOT_DETECTED) return CCEINVALIDMOD;

    return OK;
}

Result ADAC250_Presence_recv(connection_state * state)
{
    return _ADAC250_Presence();
}

Result ADAC250_PowerUp_recv(connection_state * state)
{
    Result ret;

    ret = Carrier_FmcPowerUp(hCarrier,
        adac250_selected_inst, Carrier_eFmcBoard0,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        VADJ_2_503V);
    if (FAILURE(ret)) return ret;

    return OK;
}

Result ADAC250_ReadSPIRegisters_recv(connection_state * state)
{
    Result ret;
    ADAC250_DEVICES DeviceId;
    int BaseRegister;
    char value = 0;

    DeviceId = uint_recv(state);
    BaseRegister = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        uchar_send(state, value);
        return ret;
    }

    ret = ADAC250_ReadSPIRegisters(adac250[adac250_selected_inst], DeviceId, BaseRegister, 1, &value);
    uchar_send(state, value);

    return ret;
}

Result ADAC250_WriteSPIRegisters_recv(connection_state * state)
{
    ADAC250_DEVICES DeviceId;
    int BaseRegister;
    char value;
    Result ret;

    DeviceId = uint_recv(state);
    BaseRegister = uint_recv(state);
    value = uchar_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_WriteSPIRegisters(adac250[adac250_selected_inst], DeviceId, BaseRegister, 1, &value);

    return ret;
}

Result ADAC250_DAC_SetGain_recv(connection_state * state)
{
    ADAC250_DAC_CHANNEL channel;
    int gain, i;
    Result ret;

    channel = uint_recv(state);

    i = recv(state->stream, &gain, sizeof(gain), 0);
    gain = ntohl(gain);
    if(i!=sizeof(gain)) state->has_ended=1;

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_DAC_SetGain(adac250[adac250_selected_inst], channel, gain);

    return ret;
}

Result ADAC250_DAC_SetOffset_recv(connection_state * state)
{
    ADAC250_DAC_CHANNEL channel;
    int offset, i;
    Result ret;

    i = recv(state->stream, &channel, sizeof(channel), 0);
    channel = ntohl(channel);
    if(i!=sizeof(channel)) state->has_ended=1;

    i = recv(state->stream, &offset, sizeof(offset), 0);
    offset = ntohl(offset);
    if(i!=sizeof(offset)) state->has_ended=1;

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_DAC_SetOffset(adac250[adac250_selected_inst], channel, offset);

    return ret;
}

Result ADAC250_DAC_Init_recv(connection_state * state)
{
    ADAC250_DAC_SLEEP sleep;
    ADAC250_DAC_INTERPOLATION interpolation;
    ADAC250_DAC_CMIX_MODE mixmode1;
    ADAC250_DAC_CMIX_MODE mixmode2;
    Result ret;

    sleep = uint_recv(state);
    interpolation = uint_recv(state);
    mixmode1 = uint_recv(state);
    mixmode2 = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_DAC_Init(adac250[adac250_selected_inst], sleep, interpolation, mixmode1, mixmode2);

    return ret;
}

Result ADAC250_DAC_Sync_recv(connection_state * state)
{
    Result ret;
    ADAC250_DAC_SYNC sync;
    sync =  uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_DAC_Sync(adac250[adac250_selected_inst], sync);

    return ret;
}

Result ADAC250_DAC_SyncSource_recv(connection_state * state)
{
    Result ret;
    ADAC250_DAC_SYNC_SOURCE syncsource;
    syncsource = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_DAC_SyncSource(adac250[adac250_selected_inst], syncsource);

    return ret;
}

Result ADAC250_DAC_Calibrate_recv(connection_state * state)
{
    Result ret;

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_DAC_Calibrate(adac250[adac250_selected_inst]);

    return ret;
}

Result ADAC250_PLL_Init_recv(connection_state * state)
{
    Result ret;
    ADAC250_DAC_SYNC_SOURCE syncsource;
    syncsource = uint_recv(state);
    debug("ADAC250_PLL_Init_recv source: %d\n", syncsource );

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    if(syncsource == ADAC250_CLOCKIN_10MHZREF)
    {
        ret = ADAC250_PLL_Init(adac250[adac250_selected_inst]);
    }
    else if(syncsource == ADAC250_CLOCKIN_EXTERNALREF)
    {
        ret = ADAC250_PLL_Init(adac250[adac250_selected_inst]);
    }
    else if(syncsource == ADAC250_CLOCKIN_EXTERNALCLOCK)
    {
        ret = ADAC250_PLL_Init_Extern(adac250[adac250_selected_inst], 100000000, 100000000, 100000000, ADAC250_DAC_INTERPOLATION_NONE);
    }
    else
    // Added because of potential bug if syncsource is not correctly specified
        ret = CCEINVALIDVALUE;

    return ret;
}

Result ADAC250_PLL_CalculateParams_recv(connection_state * state)
{
    unsigned refClk;
    unsigned AdcClk;
    unsigned DacDataClk;
    ADAC250_DAC_INTERPOLATION interpolation;
    ADAC250_PLL_PARAM param;
    Result ret;

    refClk = uint_recv(state);
    AdcClk = uint_recv(state);
    DacDataClk = uint_recv(state);
    interpolation = uint_recv(state);
    debug("ADAC250_PLL_CalculateParams_recv refClk: %i  AdcClk: %i  DacDataClk: %i  interpolation: %i\n", refClk, AdcClk, DacDataClk, interpolation);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        adac250_pll_param_send(state, &param);
        return ret;
    }

    ret = ADAC250_PLL_CalculateParams(refClk, AdcClk, DacDataClk, interpolation, &param);
    adac250_pll_param_send(state, &param);

    return ret;
}

Result ADAC250_PLL_SetConfig_recv(connection_state * state)
{
    ADAC250_PLL_PARAM param;
    Result ret;

    debug("SET CONFIG\n", 0);
    adac250_pll_param_recv(state, &param);
    debug("Afactor: %i\n", param.Afactor);
    debug("Bfactor: %i\n", param.Bfactor);
    debug("Rfactor: %i\n", param.Rfactor);
    debug("Pfactor: %i\n", param.Pfactor);
    debug("VCOFreq: %i\n", param.VCOFreq);
    debug("divisorADCClk: %i\n", param.divisorADCClk);
    debug("divisorDACInClk: %i\n", param.divisorDACInClk);
    debug("divisorClkOutClk: %i\n", param.divisorClkOutClk);
    debug("divisorRsvdClk: %i\n", param.divisorRsvdClk);
    debug("divisorDACDataClk: %i\n", param.divisorDACDataClk);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_PLL_SetConfig(adac250[adac250_selected_inst], param);

    return ret;
}

Result ADAC250_PLL_trigRegisterTransfert_recv(connection_state * state)
{
    Result ret;

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_PLL_trigRegisterTransfert(adac250[adac250_selected_inst]);

    return ret;
}

Result ADAC250_PLL_SyncOutput_recv(connection_state * state)
{
    Result ret;

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_PLL_SyncOutput(adac250[adac250_selected_inst]);

    return ret;
}

Result ADAC250_PLL_getStatus_recv(connection_state * state)
{
    ADAC250_PLL_STATUS status = ADAC250_PLL_UNLOCKED;
    Result ret;

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        uint_send(state, status);
        return ret;
    }

    ret = ADAC250_PLL_getStatus(adac250[adac250_selected_inst], &status);
    uint_send(state, status);

    return ret;
}

Result ADAC250_PLL_SetSleep_recv(connection_state * state)
{
    ADAC250_PLL_SLEEP sleep;
    Result ret;

    sleep.Byte = uchar_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_PLL_SetSleep(adac250[adac250_selected_inst], sleep);

    return ret;
}

Result ADAC250_PLL_setRefClkTuning_recv(connection_state * state)
{
    unsigned short value;
    Result ret;

    value = ushort_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_PLL_setRefClkTuning(adac250[adac250_selected_inst], value);

    return ret;
}

Result ADAC250_ADC_init_recv(connection_state * state)
{
    Result ret;

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_init(adac250[adac250_selected_inst]);

    return ret;
}

Result ADAC250_ADC_setGain_recv(connection_state * state)
{
    ADAC250_ADC_CHANNEL channel;
    ADAC250_ADC_GAIN gain;
    Result ret;

    channel = uint_recv(state);
    gain = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_setGain(adac250[adac250_selected_inst], channel, gain);

    return ret;
}

Result ADAC250_ADC_setFineGain_recv(connection_state * state)
{
    ADAC250_ADC_CHANNEL channel;
    char gain;
    Result ret;

    channel = uint_recv(state);
    gain = uchar_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_setFineGain(adac250[adac250_selected_inst], channel, gain);

    return ret;
}

Result ADAC250_ADC_setPedestal_recv(connection_state * state)
{
    ADAC250_ADC_CHANNEL channel;
    char value;
    Result ret;

    channel = uint_recv(state);
    value = uchar_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_setPedestal(adac250[adac250_selected_inst], channel, value);

    return ret;
}

Result ADAC250_ADC_getStatus_recv(connection_state * state)
{
    ADAC250_ADC_CHANNEL channel;
    ADAC250_ADC_STATUSES status = ADAC250_ADC_OVERRANGE;
    Result ret;

    channel = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        uint_send(state, status);
        return ret;
    }

    ret = ADAC250_ADC_getStatus(adac250[adac250_selected_inst], channel, &status);
    uint_send(state, status);

    return ret;
}

Result ADAC250_ADC_Calibrate_recv(connection_state * state)
{
    Result ret;

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_Calibrate(adac250[adac250_selected_inst]);

    return ret;
}

Result ADAC250_MUX_ConfigClockOutput_recv(connection_state * state)
{
    Result ret;
    ADAC250_CLOCK_OUTPUT_ID ClockToConfig;
    ADAC250_CLOCK_INPUT_ID ClockInputSelection;

    ClockToConfig = uint_recv(state);
    ClockInputSelection = uint_recv(state);
    debug("MUX config  out: %d  in: %d\n", ClockToConfig, ClockInputSelection  );

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_MUX_ConfigClockOutput(adac250[adac250_selected_inst], ClockToConfig, ClockInputSelection);
    ret = (ret<0 ? ADAC250_MUXERROR : OK);

    return ret;
}

Result ADAC250_ADC_SetDataFormat_recv(connection_state * state)
{
    Result ret;
    ADAC250_ADC_DATAFORMAT format;

    format = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_SetDataFormat(adac250[adac250_selected_inst], format);

    return ret;
}

Result ADAC250_ADC_SetSleepMode_recv(connection_state * state)
{
    ADAC250_ADC_SLEEP_MODE sleep;
    Result ret;

    sleep = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_SetSleepMode(adac250[adac250_selected_inst], sleep);

    return ret;
}

Result ADAC250_ADC_SetOCT_recv(connection_state * state)
{
    ADAC250_CHANNEL_OCT octa;
    ADAC250_CHANNEL_OCT octb;
    Result ret;

    octa = uint_recv(state);
    octb = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_SetOffsetCorrectionTime(adac250[adac250_selected_inst], octa, octb);

    return ret;
}

Result ADAC250_ADC_SetTriggerDelay_recv(connection_state * state)
{
    unsigned int triggerdelay;
    Result ret;

    triggerdelay = uint_recv(state);

    ret = _ADAC250_Presence();
    if(FAILURE(ret))
    {
        return ret;
    }

    ret = ADAC250_ADC_SetAdjTriggerDelay(adac250[adac250_selected_inst], triggerdelay);

    return ret;
}
