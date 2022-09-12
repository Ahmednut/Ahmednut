
#include "cce.h"
#include "eapi_serialize.h"
#include "bas_debug.h"
#include "bas_types.h"

#include "cce_radio640.h"
#include "Radio640.h"
#include "Radio640Serialize.h"
#include "Ad9361Api.h"
#include "Ad9361.h"
#include "Ad9361_defines.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

extern Carrier_handle_t hCarrier;
extern Radio640_hRadio640 ahRadio640[];

static FMCEEprom_module_types _ModList[]={FMCEEPROM_EEPROM_MOD_RADIO640, FMCEEPROM_EEPROM_MOD_RADIO640E};

Result _Radio640_Presence(unsigned int pos)
{
    Result res;
    int iConnector, iBoard;
    unsigned int uFmcMaxNum;
    unsigned int uFmcBoardNumMax;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;

    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uFmcMaxNum);
    if (FAILURE(res)) return res;
    if (pos >= uFmcMaxNum) return CARRIER_INVALID_MODULE_INDEX;
    
    res = Carrier_GetFmcBoardNumMax(hCarrier, &uFmcBoardNumMax);
    if (FAILURE(res)) return res;
    
    iBoard = pos % uFmcBoardNumMax;
    iConnector = pos / uFmcBoardNumMax;

    res = Carrier_FmcValidatePresence(hCarrier,
        iConnector, iBoard,
        _ModList, sizeof(_ModList)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if (FAILURE(res)) return res;

    if (mdetected == CARRIER_NOT_DETECTED) return eRadio640Absent;

    return eRadio640Ok;
}

Result Radio640_Presence_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    // Get desired board instance
    pos = uint_recv(state);

    debug("Radio640_Presence #%d\n",pos+1); 

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) return res;
    
    return eRadio640Ok;
}

Result Radio640_PowerUp_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    int iConnector, iBoard;
    unsigned int uFmcBoardNumMax;

    // Get desired board instance
    pos = uint_recv(state);

    debug("Radio640_powerup #%d\n",pos+1); 

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) return res;
    
    res =  Carrier_GetFmcBoardNumMax(hCarrier, &uFmcBoardNumMax);
    if (FAILURE(res)) return res;
    
    iBoard = pos % uFmcBoardNumMax;
    iConnector = pos / uFmcBoardNumMax;

    res = Carrier_FmcPowerUp(hCarrier,
        iConnector, iBoard,
        _ModList, sizeof(_ModList)/sizeof(FMCEEprom_module_types),
        VADJ_2_503V);
    
    return eRadio640Ok;
}

Result Radio640_Init_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_stInitParam InitParam;

    pos = uint_recv(state);

	debug("Radio640_init #%d\n",pos+1); 
    
    res = Radio640Serialize_RecvInitParamStruct(state, &InitParam);
    if(FAILURE(res)) goto end;

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    debug("Radio640 #%d is present\n",pos+1);
    
    res = Radio640_Init(ahRadio640[pos], &InitParam);

end:
    return res;
}

Result Radio640_DigitalCalibration_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_DigitalCalibration(ahRadio640[pos]);
end:
    return res;
}

Result Radio640_SetSyncOutputDelay_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    int32_t delay;
    
    pos = uint_recv(state);
    delay = int_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetSyncOutputDelay(ahRadio640[pos], delay);
end:
    return res;
}

Result Radio640_SetSyncInputDelay_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    int32_t delay;
    
    pos = uint_recv(state);
    delay = int_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetSyncInputDelay(ahRadio640[pos], delay);
end:
    return res;
}

Result Radio640_SetRefClkDelay_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    int32_t delay;
    
    pos = uint_recv(state);
    delay = int_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRefClkDelay(ahRadio640[pos], delay);
end:
    return res;
}

Result Radio640_SetSyncBitslip_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    int32_t bitslip;
    
    pos = uint_recv(state);
    bitslip = int_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetSyncBitslip(ahRadio640[pos], bitslip);
end:
    return res;
}

Result Radio640_SetSyncSrc_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    Radio640_eSyncSrc Src;
    
    pos = uint_recv(state);
    Src = (Radio640_eSyncSrc)uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetSyncSrc(ahRadio640[pos], Src);
end:
    return res;
}

Result Radio640_SoftSync_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SoftSync(ahRadio640[pos]);
end:
    return res;
}

Result Radio640_Mcs_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    int32_t step;
    
    pos = uint_recv(state);
    step = int_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_Mcs(ahRadio640[pos], step);
end:
    return res;
}

Result Radio640_SetMasterCalibResult_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    int32_t master_pos;
    
    pos = uint_recv(state);
    master_pos = int_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetMasterCalibResult(ahRadio640[pos], ahRadio640[master_pos]);
end:
    return res;
}

Result Radio640_SetExtMasterCalibResult_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    Ad9361_stCalibResult CalibResult;
    
    pos = uint_recv(state);
    res = Radio640Serialize_RecvCalibResultStruct(state, &CalibResult);
    if(FAILURE(res)) goto end;

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetExtMasterCalibResult(ahRadio640[pos], CalibResult);
end:
    return res;
}

Result Radio640_GetCalibResult_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    Ad9361_stCalibResult CalibResult;
    
    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetCalibResult(ahRadio640[pos], &CalibResult);
end:
    Radio640Serialize_SendCalibResultStruct(state, &CalibResult);
    return res;
}

Result Radio640_SetEnsmMode_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Ad9361_eEnsmMode mode;

    pos = uint_recv(state);
    mode = (Ad9361_eEnsmMode)uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetEnsmMode(ahRadio640[pos], mode);
    
end:
    return res;
}

Result Radio640_GetEnsmMode_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Ad9361_eEnsmMode mode;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetEnsmMode(ahRadio640[pos], &mode);
    
end:
    uint_send(state, (uint32_t)mode);
    return res;
}

Result Radio640_GetEnsmState_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Ad9361_eEnsmState EnsmState;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetEnsmState(ahRadio640[pos], &EnsmState);
    
end:
    uint_send(state, (uint32_t)EnsmState);
    return res;
}

Result Radio640_SetSamplingFreq_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint32_t sampling_freq_hz;

    pos = uint_recv(state);
    sampling_freq_hz = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetSamplingFreq(ahRadio640[pos], sampling_freq_hz);
    
end:
    return res;
}

Result Radio640_GetSamplingFreq_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint32_t sampling_freq_hz = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetSamplingFreq(ahRadio640[pos], &sampling_freq_hz);
    
end:
    uint_send(state, sampling_freq_hz);
    return res;
}

Result Radio640_SetTxLoFreq_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint64_t lo_freq_hz;

    pos = uint_recv(state);
    lo_freq_hz = uint64_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxLoFreq(ahRadio640[pos], lo_freq_hz);
    
end:
    return res;
}

Result Radio640_GetTxLoFreq_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint64_t lo_freq_hz = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTxLoFreq(ahRadio640[pos], &lo_freq_hz);
    
end:
    uint64_send(state, lo_freq_hz);
    return res;
}

Result Radio640_SetRxRfGain_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    int32_t gain_db;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    gain_db = int_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxRfGain(ahRadio640[pos], ch, gain_db);
    
end:
    return res;
}

Result Radio640_GetRxRfGain_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    int32_t gain_db = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxRfGain(ahRadio640[pos], ch, &gain_db);
    
end:
    int_send(state, gain_db);
    return res;
}

Result Radio640_SetRxRfBandwidth_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint32_t bandwidth_hz;

    pos = uint_recv(state);
    bandwidth_hz = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxRfBandwidth(ahRadio640[pos], bandwidth_hz);
    
end:
    return res;
}

Result Radio640_GetRxRfBandwidth_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint32_t bandwidth_hz = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxRfBandwidth(ahRadio640[pos], &bandwidth_hz);
    
end:
    uint_send(state, bandwidth_hz);
    return res;
}

Result Radio640_SetRxLoFreq_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint64_t lo_freq_hz;

    pos = uint_recv(state);
    lo_freq_hz = uint64_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxLoFreq(ahRadio640[pos], lo_freq_hz);
    
end:
    return res;
}

Result Radio640_GetRxLoFreq_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint64_t lo_freq_hz = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxLoFreq(ahRadio640[pos], &lo_freq_hz);
    
end:
    uint64_send(state, lo_freq_hz);
    return res;
}

Result Radio640_GetRxRssi_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    struct Ad9361_stRfRssi RfRssi;

    pos = uint_recv(state);
    ch = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxRssi(ahRadio640[pos], ch, &RfRssi);
    
end:
    Radio640Serialize_SendRfRssiStruct(state, &RfRssi);
    return res;
}

Result Radio640_SetRxGainControlMode_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t gc_mode;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    gc_mode = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxGainControlMode(ahRadio640[pos], ch, gc_mode);
    
end:
    return res;
}

Result Radio640_GetRxGainControlMode_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t gc_mode = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxGainControlMode(ahRadio640[pos], ch, &gc_mode);
    
end:
    uchar_send(state, gc_mode);
    return res;
}

Result Radio640_SetRxFirConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Ad9361Api_stRxFirConfig Config;

    pos = uint_recv(state);
    res = Radio640Serialize_RecvRxFirConfigStruct(state, &Config);
    if(FAILURE(res)) goto end;

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxFirConfig(ahRadio640[pos], Config);

end:
    return res;
}

Result Radio640_GetRxFirConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    Ad9361Api_stRxFirConfig Config;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxFirConfig(ahRadio640[pos], ch, &Config);
    
end:
    Radio640Serialize_SendRxFirConfigStruct(state, &Config);
    return res;
}

Result Radio640_SetRxFirEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxFirEnDis(ahRadio640[pos], en_dis);
    
end:
    return res;
}

Result Radio640_GetRxFirEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxFirEnDis(ahRadio640[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Radio640_SetRxRfdcTrackEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxRfdcTrackEnDis(ahRadio640[pos], en_dis);
    
end:
    return res;
}

Result Radio640_GetRxRfdcTrackEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxRfdcTrackEnDis(ahRadio640[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Radio640_SetRxBbdcTrackEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxBbdcTrackEnDis(ahRadio640[pos], en_dis);
    
end:
    return res;
}

Result Radio640_GetRxBbdcTrackEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxBbdcTrackEnDis(ahRadio640[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Radio640_SetRxQuadTrackEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxQuadTrackEnDis(ahRadio640[pos], en_dis);
    
end:
    return res;
}

Result Radio640_GetRxQuadTrackEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetRxQuadTrackEnDis(ahRadio640[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Radio640_SetTxAttenuation_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    uint32_t attenuation_mdb;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    attenuation_mdb = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxAttenuation(ahRadio640[pos], ch, attenuation_mdb);
    
end:
    return res;
}

Result Radio640_GetTxAttenuation_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    uint32_t attenuation_mdb = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTxAttenuation(ahRadio640[pos], ch, &attenuation_mdb);
    
end:
    uint_send(state, attenuation_mdb);
    return res;
}

Result Radio640_SetTxRfBandwidth_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint32_t bandwidth_hz;

    pos = uint_recv(state);
    bandwidth_hz = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxRfBandwidth(ahRadio640[pos], bandwidth_hz);
    
end:
    return res;
}

Result Radio640_GetTxRfBandwidth_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint32_t bandwidth_hz = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTxRfBandwidth(ahRadio640[pos], &bandwidth_hz);
    
end:
    uint_send(state, bandwidth_hz);
    return res;
}

Result Radio640_SetTxFirConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Ad9361Api_stTxFirConfig Config;

    pos = uint_recv(state);
    res = Radio640Serialize_RecvTxFirConfigStruct(state, &Config);
    if(FAILURE(res)) goto end;

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxFirConfig(ahRadio640[pos], Config);

end:
    return res;
}

Result Radio640_GetTxFirConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t ch;
    Ad9361Api_stTxFirConfig Config;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTxFirConfig(ahRadio640[pos], ch, &Config);
    
end:
    Radio640Serialize_SendTxFirConfigStruct(state, &Config);
    return res;
}

Result Radio640_SetTxFirEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxFirEnDis(ahRadio640[pos], en_dis);
    
end:
    return res;
}

Result Radio640_GetTxFirEnDis_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTxFirEnDis(ahRadio640[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Radio640_SetTrxPathClks_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    int i;
    
    uint32_t rx_path_clks[6];
    uint32_t tx_path_clks[6];

    pos = uint_recv(state);
    
    for(i=0; i<6; i++) rx_path_clks[i] = uint32_recv(state);
    for(i=0; i<6; i++) tx_path_clks[i] = uint32_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTrxPathClks(ahRadio640[pos], rx_path_clks, tx_path_clks);
    
end:
    return res;
}

Result Radio640_GetTrxPathClks_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    int i;
    
    uint32_t rx_path_clks[6];
    uint32_t tx_path_clks[6];

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTrxPathClks(ahRadio640[pos], rx_path_clks, tx_path_clks);
    
end:
    for(i=0; i<6; i++) uint32_send(state, rx_path_clks[i]);
    for(i=0; i<6; i++) uint32_send(state, tx_path_clks[i]);
    return res;
}


Result Radio640_Write_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640DevCom_eDevices Device;
    uint16_t u16Addr;
    uint32_t u32Data;

    pos = uint_recv(state);
    
    Device = (Radio640DevCom_eDevices)uint_recv(state);
    u16Addr = uint16_recv(state);
    u32Data = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_Write(ahRadio640[pos], Device, u16Addr, u32Data);
    
end:
    return res;
}

Result Radio640_Read_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640DevCom_eDevices Device;
    uint16_t u16Addr;
    uint32_t u32Data = 0;

    pos = uint_recv(state);
    
    Device = (Radio640DevCom_eDevices)uint_recv(state);
    u16Addr = uint16_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_Read(ahRadio640[pos], Device, u16Addr, &u32Data);
    
end:
    uint_send(state, u32Data);
    return res;
}

Result Radio640_DisableDesignClk_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_DisableDesignClk(ahRadio640[pos]);
    
end:
    return res;
}

Result Radio640_EnableDesignClk_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_EnableDesignClk(ahRadio640[pos]);
    
end:
    return res;
}

Result Radio640_IsClockMaster_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8ClockMaster = 0;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_IsClockMaster(ahRadio640[pos], &u8ClockMaster);
    
end:
    uint8_send(state, u8ClockMaster);
    return res;
}

Result Radio640_SetClkSrc_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_eClkSrc ClkSrc;

    pos = uint_recv(state);
    
    ClkSrc = (Radio640_eClkSrc)uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetClkSrc(ahRadio640[pos], ClkSrc);
    
end:
    return res;
}

Result Radio640_GetClkSrc_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_eClkSrc ClkSrc;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetClkSrc(ahRadio640[pos], &ClkSrc);
    
end:
    uint_send(state, (uint32_t)ClkSrc);
    return res;
}

Result Radio640_GetBoardTemp_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    int16_t i16Temp;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetBoardTemp(ahRadio640[pos], &i16Temp);
    
end:
    int16_send(state, i16Temp);
    return res;
}

Result Radio640_GetTrxTemp_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    int16_t i16Temp;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTrxTemp(ahRadio640[pos], &i16Temp);
    
end:
    int16_send(state, i16Temp);
    return res;
}

Result Radio640_SetEnAgcPinConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Force;
    uint8_t u8Value;

    pos = uint_recv(state);
    
    u8Force = uint8_recv(state);
    u8Value = uint8_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetEnAgcPinConfig(ahRadio640[pos], u8Force, u8Value);
    
end:
    return res;
}

Result Radio640_SetEnablePinConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Force;
    uint8_t u8Value;

    pos = uint_recv(state);
    
    u8Force = uint8_recv(state);
    u8Value = uint8_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetEnablePinConfig(ahRadio640[pos], u8Force, u8Value);
    
end:
    return res;
}

Result Radio640_SetTxnrxPinConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Force;
    uint8_t u8Value;

    pos = uint_recv(state);
    
    u8Force = uint8_recv(state);
    u8Value = uint8_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxnrxPinConfig(ahRadio640[pos], u8Force, u8Value);
    
end:
    return res;
}

Result Radio640_SetCtrlInPinConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Force;
    uint8_t u8Value;

    pos = uint_recv(state);
    
    u8Force = uint8_recv(state);
    u8Value = uint8_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetCtrlInPinConfig(ahRadio640[pos], u8Force, u8Value);
    
end:
    return res;
}

Result Radio640_SetCtrlOutConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Config;
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    u8Config = uint8_recv(state);
    u8Enable = uint8_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetCtrlOutConfig(ahRadio640[pos], u8Config, u8Enable);
    
end:
    return res;
}

Result Radio640_GetCtrlOutPinValue_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Value;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetCtrlOutPinValue(ahRadio640[pos], &u8Value);
    
end:
    uint8_send(state, u8Value);
    return res;
}

Result Radio640_GetCtrlOutPinLowHighValue_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8LowValue;
    uint8_t u8HighValue;

    pos = uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetCtrlOutPinLowHighValue(ahRadio640[pos], &u8LowValue, &u8HighValue);
    
end:
    uint8_send(state, u8LowValue);
    uint8_send(state, u8HighValue);
    return res;
}

Result Radio640_SetPowerMonitorMode_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640DevCom_eDevices Device;
    Ina226_eMode Mode;
    uint8_t u8IsBlocking;

    pos = uint_recv(state);
    
    Device = (Radio640DevCom_eDevices)uint_recv(state);
    Mode = (Ina226_eMode)uint_recv(state);
    u8IsBlocking = uint8_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetPowerMonitorMode(ahRadio640[pos], Device, Mode, u8IsBlocking);
    
end:
    return res;
}

Result Radio640_SetPowerMonitorConversionTime_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640DevCom_eDevices Device;
    Ina226_eConvTime BusConvTime;
    Ina226_eConvTime ShuntConvTime;

    pos = uint_recv(state);
    
    Device = (Radio640DevCom_eDevices)uint_recv(state);
    BusConvTime = (Ina226_eConvTime)uint_recv(state);
    ShuntConvTime = (Ina226_eConvTime)uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetPowerMonitorConversionTime(ahRadio640[pos], Device, BusConvTime, ShuntConvTime);
    
end:
    return res;
}

Result Radio640_SetPowerMonitorAveraging_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640DevCom_eDevices Device;
    Ina226_eAveraging Averaging;

    pos = uint_recv(state);
    
    Device = (Radio640DevCom_eDevices)uint_recv(state);
    Averaging = (Ina226_eAveraging)uint_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetPowerMonitorAveraging(ahRadio640[pos], Device, Averaging);
    
end:
    return res;
}

Result Radio640_ReadPowerMonitorBusVoltage_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640DevCom_eDevices Device;
    float fVoltage;

    pos = uint_recv(state);
    
    Device = (Radio640DevCom_eDevices)uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_ReadPowerMonitorBusVoltage(ahRadio640[pos], Device, &fVoltage);
    
end:
    float_send(state, fVoltage);
    return res;
}

Result Radio640_ReadPowerMonitorPower_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640DevCom_eDevices Device;
    float fPower;

    pos = uint_recv(state);
    
    Device = (Radio640DevCom_eDevices)uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_ReadPowerMonitorPower(ahRadio640[pos], Device, &fPower);
    
end:
    float_send(state, fPower);
    return res;
}

Result Radio640_ReadPowerMonitorCurrent_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640DevCom_eDevices Device;
    float fCurrent;

    pos = uint_recv(state);
    
    Device = (Radio640DevCom_eDevices)uint_recv(state);

    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_ReadPowerMonitorCurrent(ahRadio640[pos], Device, &fCurrent);
    
end:
    float_send(state, fCurrent);
    return res;
}

Result Radio640_SetRxRfFilter_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_eChannel Channel;
    Radio640_eRxRfFilter Filter;

    pos = uint_recv(state);
    
    Channel = (Radio640_eChannel)uint_recv(state);
    Filter = (Radio640_eRxRfFilter)uint_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxRfFilter(ahRadio640[pos], Channel, Filter);
    
end:
    return res;
}

Result Radio640_SetTxExtAttenuation_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_eChannel Channel;
    uint32_t u32Attenuation;

    pos = uint_recv(state);
    
    Channel = (Radio640_eChannel)uint_recv(state);
    u32Attenuation = uint32_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxExtAttenuation(ahRadio640[pos], Channel, u32Attenuation);
    
end:
    return res;
}

Result Radio640_TuneOsc_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    float fValue;

    pos = uint_recv(state);
    
    fValue = float_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_TuneOsc(ahRadio640[pos], fValue);
    
end:
    return res;
}

Result Radio640_SetTxEnableState_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_eChannel Channel;
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    Channel = (Radio640_eChannel)uint_recv(state);
    u8Enable = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxEnableState(ahRadio640[pos], Channel, u8Enable);
    
end:
    return res;
}

Result Radio640_SetRxEnableState_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_eChannel Channel;
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    Channel = (Radio640_eChannel)uint_recv(state);
    u8Enable = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxEnableState(ahRadio640[pos], Channel, u8Enable);
    
end:
    return res;
}

Result Radio640_StoreRxFastLockProfile_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Profile;

    pos = uint_recv(state);
    
    u8Profile = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_StoreRxFastLockProfile(ahRadio640[pos], u8Profile);
    
end:
    return res;
}

Result Radio640_RecallRxFastLockProfile_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Profile;

    pos = uint_recv(state);
    
    u8Profile = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_RecallRxFastLockProfile(ahRadio640[pos], u8Profile);
    
end:
    return res;
}

Result Radio640_LoadRxFastLockProfile_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    int i;
    
    uint8_t u8Profile;
    uint8_t au8Values[16];

    pos = uint_recv(state);
    
    u8Profile = uint8_recv(state);
    for(i=0; i<16; i++) au8Values[i] = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_LoadRxFastLockProfile(ahRadio640[pos], u8Profile, au8Values);
    
end:
    return res;
}

Result Radio640_SaveRxFastLockProfile_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    int i;
    
    uint8_t u8Profile;
    uint8_t au8Values[16];

    pos = uint_recv(state);
    
    u8Profile = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SaveRxFastLockProfile(ahRadio640[pos], u8Profile, au8Values);
    
end:
    for(i=0; i<16; i++) uint8_send(state, au8Values[i]);
    return res;
}

Result Radio640_StoreTxFastLockProfile_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Profile;

    pos = uint_recv(state);
    
    u8Profile = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_StoreTxFastLockProfile(ahRadio640[pos], u8Profile);
    
end:
    return res;
}

Result Radio640_RecallTxFastLockProfile_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Profile;

    pos = uint_recv(state);
    
    u8Profile = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_RecallTxFastLockProfile(ahRadio640[pos], u8Profile);
    
end:
    return res;
}

Result Radio640_LoadTxFastLockProfile_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    int i;
    
    uint8_t u8Profile;
    uint8_t au8Values[16];

    pos = uint_recv(state);
    
    u8Profile = uint8_recv(state);
    for(i=0; i<16; i++) au8Values[i] = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_LoadTxFastLockProfile(ahRadio640[pos], u8Profile, au8Values);
    
end:
    return res;
}

Result Radio640_SaveTxFastLockProfile_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    int i;
    
    uint8_t u8Profile;
    uint8_t au8Values[16];

    pos = uint_recv(state);
    
    u8Profile = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SaveTxFastLockProfile(ahRadio640[pos], u8Profile, au8Values);
    
end:
    for(i=0; i<16; i++) uint8_send(state, au8Values[i]);
    return res;
}

Result Radio640_SetRxLoExt_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    u8Enable = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRxLoExt(ahRadio640[pos], u8Enable);
    
end:
    return res;
}

Result Radio640_SetTxLoExt_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    u8Enable = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxLoExt(ahRadio640[pos], u8Enable);
    
end:
    return res;
}

Result Radio640_SetTxAutoCalibEnable_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    u8Enable = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTxAutoCalibEnable(ahRadio640[pos], u8Enable);
    
end:
    return res;
}

Result Radio640_GetTxAutoCalibEnable_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTxAutoCalibEnable(ahRadio640[pos], &u8Enable);
    
end:
    uint8_send(state, u8Enable);
    return res;
}

Result Radio640_DoCalib_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_eCalib Calib;
    int32_t i32Value;

    pos = uint_recv(state);
    
    Calib = (Radio640_eCalib)uint_recv(state);
    i32Value = int32_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_DoCalib(ahRadio640[pos], Calib, i32Value);
    
end:
    return res;
}

Result Radio640_SetRfSwitchConfig_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Force;
    uint8_t u8TrxInRxDir;

    pos = uint_recv(state);
    
    u8Force = uint8_recv(state);
    u8TrxInRxDir = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRfSwitchConfig(ahRadio640[pos], u8Force, u8TrxInRxDir);
    
end:
    return res;
}

Result Radio640_SetTriggerDelay_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    int32_t i32Delay;

    pos = uint_recv(state);
    
    i32Delay = int32_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTriggerDelay(ahRadio640[pos], i32Delay);
    
end:
    return res;
}

Result Radio640_SetTriggerBitslip_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    int32_t i32Bitslip;

    pos = uint_recv(state);
    
    i32Bitslip = int32_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetTriggerBitslip(ahRadio640[pos], i32Bitslip);
    
end:
    return res;
}

Result Radio640_ReadClkFreq_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    Radio640_eTestClk Clk;
    uint32_t u32Freq;

    pos = uint_recv(state);
    
    Clk = (Radio640_eTestClk)uint_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_ReadClkFreq(ahRadio640[pos], Clk, &u32Freq);
    
end:
    uint32_send(state, u32Freq);
    return res;
}

Result Radio640_GetTriggerValue_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Value;

    pos = uint_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTriggerValue(ahRadio640[pos], &u8Value);
    
end:
    uint8_send(state, u8Value);
    return res;
}

Result Radio640_GetTriggerLowHighValue_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8LowValue;
    uint8_t u8HighValue;

    pos = uint_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetTriggerLowHighValue(ahRadio640[pos], &u8LowValue, &u8HighValue);
    
end:
    uint8_send(state, u8LowValue);
    uint8_send(state, u8HighValue);
    return res;
}

Result Radio640_GetInputSyncValue_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Value;

    pos = uint_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetInputSyncValue(ahRadio640[pos], &u8Value);
    
end:
    uint8_send(state, u8Value);
    return res;
}

Result Radio640_GetInputSyncLowHighValue_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8LowValue;
    uint8_t u8HighValue;

    pos = uint_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetInputSyncLowHighValue(ahRadio640[pos], &u8LowValue, &u8HighValue);
    
end:
    uint8_send(state, u8LowValue);
    uint8_send(state, u8HighValue);
    return res;
}

Result Radio640_SetClkToExtendedFmcEnable_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    u8Enable = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetClkToExtendedFmcEnable(ahRadio640[pos], u8Enable);
    
end:
    return res;
}

Result Radio640_SetRefOutEnable_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Enable;

    pos = uint_recv(state);
    
    u8Enable = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetRefOutEnable(ahRadio640[pos], u8Enable);
    
end:
    return res;
}


Result Radio640_SetComplexMultiplier_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
 
    uint8_t u8Channel; 
    uint8_t u8Direction;
    uint16_t u16RealCoefficient;
    uint16_t u16ImgCoefficient;
    
    pos = uint_recv(state);

    u8Channel = uint8_recv(state);
    u8Direction = uint8_recv(state);
    u16RealCoefficient = uint16_recv(state);
    u16ImgCoefficient = uint16_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetComplexMultiplier(ahRadio640[pos], u8Channel, u8Direction, u16RealCoefficient, u16ImgCoefficient);
    
end:
    return res;
}

Result Radio640_GetComplexMultiplier_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Channel;
    uint8_t u8Direction;
    uint16_t u16RealCoefficient;
    uint16_t u16ImgCoefficient;
    
    pos = uint_recv(state);
    
    u8Channel = uint8_recv(state);
    u8Direction = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetComplexMultiplier(ahRadio640[pos], u8Channel, u8Direction, &u16RealCoefficient, &u16ImgCoefficient);
    
end:
    uint16_send(state, u16RealCoefficient);
    uint16_send(state, u16ImgCoefficient);
    return res;
}

Result Radio640_SetDataDelay_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Channel; 
    uint8_t u8Direction;
    uint32_t u32DataDelay;
    
    pos = uint_recv(state);

    u8Channel = uint8_recv(state);
    u8Direction = uint8_recv(state);
    u32DataDelay = uint32_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_SetDataDelay(ahRadio640[pos], u8Channel, u8Direction, u32DataDelay);
    
end:
    return res;
}

Result Radio640_GetDataDelay_recv(connection_state * state)
{
    Result res = eRadio640Ok;
    unsigned pos;
    
    uint8_t u8Channel;
    uint8_t u8Direction;
    uint32_t u32DataDelay;
    
    pos = uint_recv(state);
    
    u8Channel = uint8_recv(state);
    u8Direction = uint8_recv(state);
    
    res = _Radio640_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Radio640_GetDataDelay(ahRadio640[pos], u8Channel, u8Direction, &u32DataDelay);
    
end:
    uint32_send(state, u32DataDelay);
    return res;
}