
#include "cce.h"
#include "eapi_serialize.h"
#include "bas_debug.h"
#include "bas_types.h"

#include "cce_fmcomms3.h"
#include "fmcomms3.h"
#include "fmcomms3_serialize.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_internal_def.h"

extern Carrier_handle_t hCarrier;

extern Fmcomms3_Handle_t ahFmcomms3[];

Result _Fmcomms3_Presence(unsigned int pos)
{
    Result res;
    unsigned int uiFmcMaxNum;
    unsigned int uiFmcBaseAddr;
        
    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if (FAILURE(res)) return res;
    if (pos >= (uiFmcMaxNum / CARRIER_FMC_MAXBOARD)) return CARRIER_INVALID_MODULE_INDEX;
    
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleFMC, pos * CARRIER_FMC_MAXBOARD, &uiFmcBaseAddr);
    if (FAILURE(res)) return res;

    if(((*(unsigned int *)uiFmcBaseAddr) >> 16) != 0x9361)
    {
        // Core not present
        return eFmcomms3NoCore;
    }

    return eFmcomms3Ok;
}

Result Fmcomms3_presence_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    // Get desired board instance
    pos = uint_recv(state);

    debug("Fmcomms3_Presence #%d\n",pos+1); 

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) return res;
    
    return eFmcomms3Ok;
}

Result Fmcomms3_powerup_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    int iConnector, iBoard;

    // Get desired board instance
    pos = uint_recv(state);

	debug("Fmcomms3_powerup #%d\n",pos+1); 

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) return res;
    
    iBoard = 0;
    iConnector = pos;

    /* The right way is to call Carrier_FmcPowerUp function by since
     * we do not support EEPROM detection for the FMCOMMS3 board,
     * we call directly the FmcSetVadj function without any additional 
     * verification
     */
    res = hCarrier->pfCarrier_FmcSetVadj(hCarrier, iConnector, iBoard, VADJ_2_503V);
    if(FAILURE(res)) return res;
    
    return eFmcomms3Ok;
}

Result Fmcomms3_init_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    AD9361_InitParam init_param;

    pos = uint_recv(state);

	debug("Fmcomms3_init #%d\n",pos+1); 
    
    res = fmcomms3_initparam_struct_recv(state, &init_param);
    if(FAILURE(res)) goto end;

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    debug("Fmcomms3 #%d is present\n",pos+1);
    
    res = Fmcomms3_init(ahFmcomms3[pos], &init_param);

end:
    return res;
}

Result Fmcomms3_digital_calibration_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_digital_calibration(ahFmcomms3[pos]);
end:
    return res;
}

Result Fmcomms3_set_sync_delay_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    int32_t delay;
    
    pos = uint_recv(state);
    delay = int_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_sync_delay(ahFmcomms3[pos], delay);
end:
    return res;
}

Result Fmcomms3_set_refclk_delay_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    int32_t delay;
    
    pos = uint_recv(state);
    delay = int_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_refclk_delay(ahFmcomms3[pos], delay);
end:
    return res;
}

Result Fmcomms3_set_sync_bitslip_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    int32_t bitslip;
    
    pos = uint_recv(state);
    bitslip = int_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_sync_bitslip(ahFmcomms3[pos], bitslip);
end:
    return res;
}

Result Fmcomms3_set_sync_src_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    int32_t src;
    
    pos = uint_recv(state);
    src = int_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_sync_src(ahFmcomms3[pos], src);
end:
    return res;
}

Result Fmcomms3_soft_sync_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_soft_sync(ahFmcomms3[pos]);
end:
    return res;
}

Result Fmcomms3_mcs_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    int32_t step;
    
    pos = uint_recv(state);
    step = int_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_mcs(ahFmcomms3[pos], step);
end:
    return res;
}

Result Fmcomms3_set_master_calib_result_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    int32_t master_pos;
    
    pos = uint_recv(state);
    master_pos = int_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_master_calib_result(ahFmcomms3[pos], ahFmcomms3[master_pos]);
end:
    return res;
}

Result Fmcomms3_set_ext_master_calib_result_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    sAd9361CalibResult_t ExtCalibResult;
    
    pos = uint_recv(state);
    res = fmcomms3_calib_result_struct_recv(state, &ExtCalibResult);
    if(FAILURE(res)) goto end;

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_ext_master_calib_result(ahFmcomms3[pos], ExtCalibResult);
end:
    return res;
}

Result Fmcomms3_get_calib_result_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;

    sAd9361CalibResult_t * pCalibResult = NULL;
    
    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_calib_result(ahFmcomms3[pos], &pCalibResult);
end:
    fmcomms3_calib_result_struct_send(state, pCalibResult);
    return res;
}

Result Fmcomms3_set_en_state_machine_mode_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t mode;

    pos = uint_recv(state);
    mode = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_en_state_machine_mode(ahFmcomms3[pos], mode);
    
end:
    return res;
}

Result Fmcomms3_get_en_state_machine_mode_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t mode = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_en_state_machine_mode(ahFmcomms3[pos], &mode);
    
end:
    uint_send(state, mode);
    return res;
}

Result Fmcomms3_set_sampling_freq_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t sampling_freq_hz;

    pos = uint_recv(state);
    sampling_freq_hz = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_sampling_freq(ahFmcomms3[pos], sampling_freq_hz);
    
end:
    return res;
}

Result Fmcomms3_get_sampling_freq_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t sampling_freq_hz = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_sampling_freq(ahFmcomms3[pos], &sampling_freq_hz);
    
end:
    uint_send(state, sampling_freq_hz);
    return res;
}

Result Fmcomms3_set_tx_lo_freq_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint64_t lo_freq_hz;

    pos = uint_recv(state);
    lo_freq_hz = uint64_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_tx_lo_freq(ahFmcomms3[pos], lo_freq_hz);
    
end:
    return res;
}

Result Fmcomms3_get_tx_lo_freq_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint64_t lo_freq_hz = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_tx_lo_freq(ahFmcomms3[pos], &lo_freq_hz);
    
end:
    uint64_send(state, lo_freq_hz);
    return res;
}

Result Fmcomms3_set_rx_rf_gain_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    int32_t gain_db;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    gain_db = int_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_rf_gain(ahFmcomms3[pos], ch, gain_db);
    
end:
    return res;
}

Result Fmcomms3_get_rx_rf_gain_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    int32_t gain_db = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_rf_gain(ahFmcomms3[pos], ch, &gain_db);
    
end:
    int_send(state, gain_db);
    return res;
}

Result Fmcomms3_set_rx_rf_bandwidth_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t bandwidth_hz;

    pos = uint_recv(state);
    bandwidth_hz = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_rf_bandwidth(ahFmcomms3[pos], bandwidth_hz);
    
end:
    return res;
}

Result Fmcomms3_get_rx_rf_bandwidth_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t bandwidth_hz = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_rf_bandwidth(ahFmcomms3[pos], &bandwidth_hz);
    
end:
    uint_send(state, bandwidth_hz);
    return res;
}

Result Fmcomms3_set_rx_lo_freq_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint64_t lo_freq_hz;

    pos = uint_recv(state);
    lo_freq_hz = uint64_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_lo_freq(ahFmcomms3[pos], lo_freq_hz);
    
end:
    return res;
}

Result Fmcomms3_get_rx_lo_freq_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint64_t lo_freq_hz = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_lo_freq(ahFmcomms3[pos], &lo_freq_hz);
    
end:
    uint64_send(state, lo_freq_hz);
    return res;
}

Result Fmcomms3_get_rx_rssi_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    struct rf_rssi rssi;

    pos = uint_recv(state);
    ch = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_rssi(ahFmcomms3[pos], ch, &rssi);
    
end:
    fmcomms3_rf_rssi_struct_send(state, &rssi);
    return res;
}

Result Fmcomms3_set_rx_gain_control_mode_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t gc_mode;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    gc_mode = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_gain_control_mode(ahFmcomms3[pos], ch, gc_mode);
    
end:
    return res;
}

Result Fmcomms3_get_rx_gain_control_mode_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t gc_mode = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_gain_control_mode(ahFmcomms3[pos], ch, &gc_mode);
    
end:
    uchar_send(state, gc_mode);
    return res;
}

Result Fmcomms3_set_rx_fir_config_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    AD9361_RXFIRConfig fir_cfg;

    pos = uint_recv(state);
    res = fmcomms3_rxfirconfig_struct_recv(state, &fir_cfg);
    if(FAILURE(res)) goto end;

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_fir_config(ahFmcomms3[pos], fir_cfg);

end:
    return res;
}

Result Fmcomms3_get_rx_fir_config_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    AD9361_RXFIRConfig fir_cfg;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    
    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_fir_config(ahFmcomms3[pos], ch, &fir_cfg);
    
end:
    fmcomms3_rxfirconfig_struct_send(state, &fir_cfg);
    return res;
}

Result Fmcomms3_set_rx_fir_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_fir_en_dis(ahFmcomms3[pos], en_dis);
    
end:
    return res;
}

Result Fmcomms3_get_rx_fir_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_fir_en_dis(ahFmcomms3[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Fmcomms3_set_rx_rfdc_track_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_rfdc_track_en_dis(ahFmcomms3[pos], en_dis);
    
end:
    return res;
}

Result Fmcomms3_get_rx_rfdc_track_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_rfdc_track_en_dis(ahFmcomms3[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Fmcomms3_set_rx_bbdc_track_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_bbdc_track_en_dis(ahFmcomms3[pos], en_dis);
    
end:
    return res;
}

Result Fmcomms3_get_rx_bbdc_track_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_bbdc_track_en_dis(ahFmcomms3[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Fmcomms3_set_rx_quad_track_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_rx_quad_track_en_dis(ahFmcomms3[pos], en_dis);
    
end:
    return res;
}

Result Fmcomms3_get_rx_quad_track_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_rx_quad_track_en_dis(ahFmcomms3[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Fmcomms3_set_tx_attenuation_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint32_t attenuation_mdb;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    attenuation_mdb = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_tx_attenuation(ahFmcomms3[pos], ch, attenuation_mdb);
    
end:
    return res;
}

Result Fmcomms3_get_tx_attenuation_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint32_t attenuation_mdb = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_tx_attenuation(ahFmcomms3[pos], ch, &attenuation_mdb);
    
end:
    uint_send(state, attenuation_mdb);
    return res;
}

Result Fmcomms3_set_tx_rf_bandwidth_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t bandwidth_hz;

    pos = uint_recv(state);
    bandwidth_hz = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_tx_rf_bandwidth(ahFmcomms3[pos], bandwidth_hz);
    
end:
    return res;
}

Result Fmcomms3_get_tx_rf_bandwidth_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t bandwidth_hz = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_tx_rf_bandwidth(ahFmcomms3[pos], &bandwidth_hz);
    
end:
    uint_send(state, bandwidth_hz);
    return res;
}

Result Fmcomms3_set_tx_fir_config_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    AD9361_TXFIRConfig fir_cfg;

    pos = uint_recv(state);
    res = fmcomms3_txfirconfig_struct_recv(state, &fir_cfg);
    if(FAILURE(res)) goto end;

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_tx_fir_config(ahFmcomms3[pos], fir_cfg);

end:
    return res;
}

Result Fmcomms3_get_tx_fir_config_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    AD9361_TXFIRConfig fir_cfg;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    
    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_tx_fir_config(ahFmcomms3[pos], ch, &fir_cfg);
    
end:
    fmcomms3_txfirconfig_struct_send(state, &fir_cfg);
    return res;
}

Result Fmcomms3_set_tx_fir_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis;

    pos = uint_recv(state);
    en_dis = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_tx_fir_en_dis(ahFmcomms3[pos], en_dis);
    
end:
    return res;
}

Result Fmcomms3_get_tx_fir_en_dis_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t en_dis = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_tx_fir_en_dis(ahFmcomms3[pos], &en_dis);
    
end:
    uchar_send(state, en_dis);
    return res;
}

Result Fmcomms3_set_trx_path_clks_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    int i;
    
    uint32_t rx_path_clks[6];
    uint32_t tx_path_clks[6];

    pos = uint_recv(state);
    
    for(i=0; i<6; i++) rx_path_clks[i] = uint32_recv(state);
    for(i=0; i<6; i++) tx_path_clks[i] = uint32_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_trx_path_clks(ahFmcomms3[pos], rx_path_clks, tx_path_clks);
    
end:
    return res;
}

Result Fmcomms3_get_trx_path_clks_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    int i;
    
    uint32_t rx_path_clks[6];
    uint32_t tx_path_clks[6];

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_trx_path_clks(ahFmcomms3[pos], rx_path_clks, tx_path_clks);
    
end:
    for(i=0; i<6; i++) uint32_send(state, rx_path_clks[i]);
    for(i=0; i<6; i++) uint32_send(state, tx_path_clks[i]);
    return res;
}

Result Fmcomms3_set_dac_datasel_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    dds_data_select_e sel;

    pos = uint_recv(state);
    sel = (dds_data_select_e)uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_dac_datasel(ahFmcomms3[pos], sel);
    
end:
    return res;
}

Result Fmcomms3_get_dac_datasel_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    dds_data_select_e sel = 0;

    pos = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_dac_datasel(ahFmcomms3[pos], &sel);
    
end:
    uint_send(state, (uint32_t)sel);
    return res;
}

Result Fmcomms3_set_dds_freq_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t tone;
    uint32_t freq;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    tone = uchar_recv(state);
    freq = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_dds_freq(ahFmcomms3[pos], ch, tone, freq);
    
end:
    return res;
}

Result Fmcomms3_get_dds_freq_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t tone;
    uint32_t freq = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    tone = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_dds_freq(ahFmcomms3[pos], ch, tone, &freq);
    
end:
    uint_send(state, freq);
    return res;
}

Result Fmcomms3_set_dds_phase_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t tone;
    uint32_t phase;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    tone = uchar_recv(state);
    phase = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_dds_phase(ahFmcomms3[pos], ch, tone, phase);
    
end:
    return res;
}

Result Fmcomms3_get_dds_phase_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t tone;
    uint32_t phase = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    tone = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_dds_phase(ahFmcomms3[pos], ch, tone, &phase);
    
end:
    uint_send(state, phase);
    return res;
}

Result Fmcomms3_set_dds_scale_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t tone;
    int32_t scale;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    tone = uchar_recv(state);
    scale = int_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_set_dds_scale(ahFmcomms3[pos], ch, tone, scale);
    
end:
    return res;
}

Result Fmcomms3_get_dds_scale_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint8_t ch;
    uint8_t tone;
    int32_t scale = 0;

    pos = uint_recv(state);
    ch = uchar_recv(state);
    tone = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_get_dds_scale(ahFmcomms3[pos], ch, tone, &scale);
    
end:
    int_send(state, scale);
    return res;
}

Result Fmcomms3_spi_write_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t reg;
    uint8_t val;

    pos = uint_recv(state);
    reg = uint_recv(state);
    val = uchar_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_spi_write(ahFmcomms3[pos], reg, val);
    
end:
    return res;
}

Result Fmcomms3_spi_read_recv(connection_state * state)
{
    Result res = eFmcomms3Ok;
    unsigned pos;
    
    uint32_t reg;
    uint8_t val = 0;

    pos = uint_recv(state);
    reg = uint_recv(state);

    res = _Fmcomms3_Presence(pos);
    if(FAILURE(res)) goto end;
    
    res = Fmcomms3_spi_read(ahFmcomms3[pos], reg, &val);
    
end:
    uchar_send(state, val);
    return res;
}

