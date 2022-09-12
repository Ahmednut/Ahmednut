/****************************************************************************
 *
 *    ****                              I
 *   ******                            ***
 *   *******                           ****
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    ****I  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       eapi_fmcomms3.c
 * @brief      fmcomms3 eapi module related functions
 *
 * $Date: 2015/08/28 14:11:02 $
 * $Revision: 1.2 $
 *
 * (c) 2012, Nutaq Inc. All rights reserved.
 *
 * @ingroup FMCOMMS3_FMCOMMS3
 *
 ***************************************************************************/

#include "cce.h"
#include "eapi_fmcomms3.h"
#include "eapi_serialize.h"
#include "fmcomms3_serialize.h"
#include "ad9361_api.h"
#include "ad9361.h"

Result Fmcomms3_presence_send(connection_state * state, unsigned uBoardNum)
{
    cmd_send(state, FMCOMMS3_PRESENCE);
    uint_send(state, uBoardNum-1);

    return result_recv(state);
}

Result Fmcomms3_powerup_send(connection_state * state, unsigned uBoardNum)
{
    cmd_send(state, FMCOMMS3_POWERUP);
    uint_send(state, uBoardNum-1);

    return result_recv(state);
}

Result Fmcomms3_init_send(connection_state * state, unsigned uBoardNum, AD9361_InitParam * init_param)
{
    cmd_send(state, FMCOMMS3_INIT);
    uint_send(state, uBoardNum-1);

    fmcomms3_initparam_struct_send(state, init_param);

    return result_recv(state);
}

Result Fmcomms3_digital_calibration_send(connection_state * state, unsigned uBoardNum)
{
    cmd_send(state, FMCOMMS3_DIGITAL_CALIBRATION);
    uint_send(state, uBoardNum-1);

    return result_recv(state);
}

Result Fmcomms3_set_sync_delay_send(connection_state * state, unsigned uBoardNum, int32_t delay)
{
    cmd_send(state, FMCOMMS3_SET_SYNC_DELAY);
    uint_send(state, uBoardNum-1);
    
    int_send(state, delay);

    return result_recv(state);
}

Result Fmcomms3_set_refclk_delay_send(connection_state * state, unsigned uBoardNum, int32_t delay)
{
    cmd_send(state, FMCOMMS3_SET_REFCLK_DELAY);
    uint_send(state, uBoardNum-1);
    
    int_send(state, delay);

    return result_recv(state);
}

Result Fmcomms3_set_sync_bitslip_send(connection_state * state, unsigned uBoardNum, int32_t bitslip)
{
    cmd_send(state, FMCOMMS3_SET_SYNC_BITSLIP);
    uint_send(state, uBoardNum-1);
    
    int_send(state, bitslip);

    return result_recv(state);
}

Result Fmcomms3_set_sync_src_send(connection_state * state, unsigned uBoardNum, int32_t src)
{
    cmd_send(state, FMCOMMS3_SET_SYNC_SRC);
    uint_send(state, uBoardNum-1);
    
    int_send(state, src);

    return result_recv(state);
}

Result Fmcomms3_soft_sync_send(connection_state * state, unsigned uBoardNum)
{
    cmd_send(state, FMCOMMS3_SOFT_SYNC);
    uint_send(state, uBoardNum-1);

    return result_recv(state);
}

Result Fmcomms3_mcs_send(connection_state * state, unsigned uBoardNum, int32_t step)
{
    cmd_send(state, FMCOMMS3_MCS);
    uint_send(state, uBoardNum-1);
    
    int_send(state, step);

    return result_recv(state);
}

Result Fmcomms3_set_master_calib_result_send(connection_state * state, unsigned uBoardNum, unsigned uMasterBoardNum)
{
    cmd_send(state, FMCOMMS3_SET_MASTER_CALIB_RESULT);
    uint_send(state, uBoardNum-1);
    
    int_send(state, uMasterBoardNum-1);

    return result_recv(state);
}

Result Fmcomms3_set_ext_master_calib_result_send(connection_state * state, unsigned uBoardNum, sAd9361CalibResult_t * pCalibResult)
{
    cmd_send(state, FMCOMMS3_SET_EXT_MASTER_CALIB_RESULT);
    uint_send(state, uBoardNum-1);
    
    fmcomms3_calib_result_struct_send(state, pCalibResult);

    return result_recv(state);
}

Result Fmcomms3_get_calib_result_send(connection_state * state, unsigned uBoardNum, sAd9361CalibResult_t * pCalibResult)
{
    cmd_send(state, FMCOMMS3_GET_CALIB_RESULT);
    uint_send(state, uBoardNum-1);
    
    fmcomms3_calib_result_struct_recv(state, pCalibResult);

    return result_recv(state);
}

Result Fmcomms3_set_en_state_machine_mode_send(connection_state * state, unsigned uBoardNum, uint32_t mode)
{
    cmd_send(state, FMCOMMS3_SET_EN_STATE_MACHINE_MODE);
    uint_send(state, uBoardNum-1);
    uint_send(state, mode);

    return result_recv(state);
}

Result Fmcomms3_get_en_state_machine_mode_send(connection_state * state, unsigned uBoardNum, uint32_t * mode)
{
    cmd_send(state, FMCOMMS3_GET_EN_STATE_MACHINE_MODE);
    uint_send(state, uBoardNum-1);

    *mode = uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_sampling_freq_send(connection_state * state, unsigned uBoardNum, uint32_t sampling_freq_hz)
{
    cmd_send(state, FMCOMMS3_SET_SAMPLING_FREQ);
    uint_send(state, uBoardNum-1);
    uint_send(state, sampling_freq_hz);

    return result_recv(state);
}

Result Fmcomms3_get_sampling_freq_send(connection_state * state, unsigned uBoardNum, uint32_t * sampling_freq_hz)
{
    cmd_send(state, FMCOMMS3_GET_SAMPLING_FREQ);
    uint_send(state, uBoardNum-1);

    *sampling_freq_hz = uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_tx_lo_freq_send(connection_state * state, unsigned uBoardNum,  uint64_t lo_freq_hz)
{
    cmd_send(state, FMCOMMS3_SET_TX_LO_FREQ);
    uint_send(state, uBoardNum-1);
    uint64_send(state, lo_freq_hz);

    return result_recv(state);
}

Result Fmcomms3_get_tx_lo_freq_send(connection_state * state, unsigned uBoardNum, uint64_t * lo_freq_hz)
{
    cmd_send(state, FMCOMMS3_GET_TX_LO_FREQ);
    uint_send(state, uBoardNum-1);

    *lo_freq_hz = uint64_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_rx_rf_gain_send(connection_state * state, unsigned uBoardNum, uint8_t ch, int32_t gain_db)
{
    cmd_send(state, FMCOMMS3_SET_RX_RF_GAIN);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    int_send(state, gain_db);

    return result_recv(state);
}

Result Fmcomms3_get_rx_rf_gain_send(connection_state * state, unsigned uBoardNum, uint8_t ch, int32_t * gain_db)
{
    cmd_send(state, FMCOMMS3_GET_RX_RF_GAIN);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);

    *gain_db = int_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_rx_rf_bandwidth_send(connection_state * state, unsigned uBoardNum, uint32_t bandwidth_hz)
{
    cmd_send(state, FMCOMMS3_SET_RX_RF_BANDWIDTH);
    uint_send(state, uBoardNum-1);
    uint_send(state, bandwidth_hz);

    return result_recv(state);
}

Result Fmcomms3_get_rx_rf_bandwidth_send(connection_state * state, unsigned uBoardNum, uint32_t * bandwidth_hz)
{
    cmd_send(state, FMCOMMS3_GET_RX_RF_BANDWIDTH);
    uint_send(state, uBoardNum-1);

    *bandwidth_hz = uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_rx_lo_freq_send(connection_state * state, unsigned uBoardNum, uint64_t lo_freq_hz)
{
    cmd_send(state, FMCOMMS3_SET_RX_LO_FREQ);
    uint_send(state, uBoardNum-1);
    uint64_send(state, lo_freq_hz);

    return result_recv(state);
}

Result Fmcomms3_get_rx_lo_freq_send(connection_state * state, unsigned uBoardNum, uint64_t * lo_freq_hz)
{
    cmd_send(state, FMCOMMS3_GET_RX_LO_FREQ);
    uint_send(state, uBoardNum-1);

    *lo_freq_hz = uint64_recv(state);

    return result_recv(state);
}

Result Fmcomms3_get_rx_rssi_send(connection_state * state, unsigned uBoardNum, uint8_t ch, struct rf_rssi * rssi)
{
    cmd_send(state, FMCOMMS3_GET_RX_RSSI);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);

    fmcomms3_rf_rssi_struct_send(state, rssi);

    return result_recv(state);
}

Result Fmcomms3_set_rx_gain_control_mode_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t gc_mode)
{
    cmd_send(state, FMCOMMS3_SET_RX_GAIN_CONTROL_MODE);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    uchar_send(state, gc_mode);

    return result_recv(state);
}

Result Fmcomms3_get_rx_gain_control_mode_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t * gc_mode)
{
    cmd_send(state, FMCOMMS3_GET_RX_GAIN_CONTROL_MODE);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);

    *gc_mode = uchar_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_rx_fir_config_send(connection_state * state, unsigned uBoardNum, AD9361_RXFIRConfig * fir_cfg)
{
    cmd_send(state, FMCOMMS3_SET_RX_FIR_CONFIG);
    uint_send(state, uBoardNum-1);
    fmcomms3_rxfirconfig_struct_send(state, fir_cfg);

    return result_recv(state);
}

Result Fmcomms3_get_rx_fir_config_send(connection_state * state, unsigned uBoardNum, uint8_t ch, AD9361_RXFIRConfig * fir_cfg)
{
    cmd_send(state, FMCOMMS3_GET_RX_FIR_CONFIG);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);

    fmcomms3_rxfirconfig_struct_recv(state, fir_cfg);

    return result_recv(state);
}

Result Fmcomms3_set_rx_fir_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis)
{
    cmd_send(state, FMCOMMS3_SET_RX_FIR_EN_DIS);
    uint_send(state, uBoardNum-1);
    uchar_send(state, en_dis);

    return result_recv(state);
}

Result Fmcomms3_get_rx_fir_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t * en_dis)
{
    cmd_send(state, FMCOMMS3_GET_RX_FIR_EN_DIS);
    uint_send(state, uBoardNum-1);

    *en_dis = uchar_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_rx_rfdc_track_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis)
{
    cmd_send(state, FMCOMMS3_SET_RX_RFDC_TRACK_EN_DIS);
    uint_send(state, uBoardNum-1);
    uchar_send(state, en_dis);

    return result_recv(state);
}

Result Fmcomms3_get_rx_rfdc_track_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t * en_dis)
{
    cmd_send(state, FMCOMMS3_GET_RX_RFDC_TRACK_EN_DIS);
    uint_send(state, uBoardNum-1);

    *en_dis = uchar_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_rx_bbdc_track_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis)
{
    cmd_send(state, FMCOMMS3_SET_RX_BBDC_TRACK_EN_DIS);
    uint_send(state, uBoardNum-1);
    uchar_send(state, en_dis);

    return result_recv(state);
}

Result Fmcomms3_get_rx_bbdc_track_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t * en_dis)
{
    cmd_send(state, FMCOMMS3_GET_RX_BBDC_TRACK_EN_DIS);
    uint_send(state, uBoardNum-1);

    *en_dis = uchar_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_rx_quad_track_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis)
{
    cmd_send(state, FMCOMMS3_SET_RX_QUAD_TRACK_EN_DIS);
    uint_send(state, uBoardNum-1);
    uchar_send(state, en_dis);

    return result_recv(state);
}

Result Fmcomms3_get_rx_quad_track_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t * en_dis)
{
    cmd_send(state, FMCOMMS3_GET_RX_QUAD_TRACK_EN_DIS);
    uint_send(state, uBoardNum-1);

    *en_dis = uchar_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_tx_attenuation_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint32_t attenuation_mdb)
{
    cmd_send(state, FMCOMMS3_SET_TX_ATTENUATION);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    uint_send(state, attenuation_mdb);

    return result_recv(state);
}

Result Fmcomms3_get_tx_attenuation_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint32_t * attenuation_mdb)
{
    cmd_send(state, FMCOMMS3_GET_TX_ATTENUATION);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);

    *attenuation_mdb = uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_tx_rf_bandwidth_send(connection_state * state, unsigned uBoardNum, uint32_t bandwidth_hz)
{
    cmd_send(state, FMCOMMS3_SET_TX_RF_BANDWIDTH);
    uint_send(state, uBoardNum-1);
    uint_send(state, bandwidth_hz);

    return result_recv(state);
}

Result Fmcomms3_get_tx_rf_bandwidth_send(connection_state * state, unsigned uBoardNum, uint32_t * bandwidth_hz)
{
    cmd_send(state, FMCOMMS3_GET_TX_RF_BANDWIDTH);
    uint_send(state, uBoardNum-1);

    *bandwidth_hz = uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_tx_fir_config_send(connection_state * state, unsigned uBoardNum, AD9361_TXFIRConfig * fir_cfg)
{
    cmd_send(state, FMCOMMS3_SET_TX_FIR_CONFIG);
    uint_send(state, uBoardNum-1);
    fmcomms3_txfirconfig_struct_send(state, fir_cfg);

    return result_recv(state);
}

Result Fmcomms3_get_tx_fir_config_send(connection_state * state, unsigned uBoardNum, uint8_t ch, AD9361_TXFIRConfig * fir_cfg)
{
    cmd_send(state, FMCOMMS3_GET_TX_FIR_CONFIG);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);

    fmcomms3_txfirconfig_struct_recv(state, fir_cfg);

    return result_recv(state);
}

Result Fmcomms3_set_tx_fir_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis)
{
    cmd_send(state, FMCOMMS3_SET_TX_FIR_EN_DIS);
    uint_send(state, uBoardNum-1);
    uchar_send(state, en_dis);

    return result_recv(state);
}

Result Fmcomms3_get_tx_fir_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t * en_dis)
{
    cmd_send(state, FMCOMMS3_GET_TX_FIR_EN_DIS);
    uint_send(state, uBoardNum-1);

    *en_dis = uchar_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_trx_path_clks_send(connection_state * state, unsigned uBoardNum, uint32_t * rx_path_clks, uint32_t * tx_path_clks)
{
    int i;
    
    cmd_send(state, FMCOMMS3_SET_TRX_PATH_CLKS);
    uint_send(state, uBoardNum-1);
    for(i=0; i<6; i++) uint32_send(state, rx_path_clks[i]);
    for(i=0; i<6; i++) uint32_send(state, tx_path_clks[i]);

    return result_recv(state);
}

Result Fmcomms3_get_trx_path_clks_send(connection_state * state, unsigned uBoardNum, uint32_t * rx_path_clks, uint32_t * tx_path_clks)
{
    int i;
    
    cmd_send(state, FMCOMMS3_GET_TRX_PATH_CLKS);
    uint_send(state, uBoardNum-1);

    for(i=0; i<6; i++) rx_path_clks[i] = uint32_recv(state);
    for(i=0; i<6; i++) tx_path_clks[i] = uint32_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_dac_datasel_send(connection_state * state, unsigned uBoardNum, dds_data_select_e sel)
{
    cmd_send(state, FMCOMMS3_SET_DAC_DATASEL);
    uint_send(state, uBoardNum-1);
    uint_send(state, (uint32_t)sel);

    return result_recv(state);
}

Result Fmcomms3_get_dac_datasel_send(connection_state * state, unsigned uBoardNum, dds_data_select_e * sel)
{
    cmd_send(state, FMCOMMS3_GET_DAC_DATASEL);
    uint_send(state, uBoardNum-1);

    *sel = (dds_data_select_e)uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_dds_freq_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t freq)
{
    cmd_send(state, FMCOMMS3_SET_DDS_FREQ);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    uchar_send(state, tone);
    uint_send(state, freq);

    return result_recv(state);
}

Result Fmcomms3_get_dds_freq_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t * freq)
{
    cmd_send(state, FMCOMMS3_GET_DDS_FREQ);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    uchar_send(state, tone);

    *freq = uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_dds_phase_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t phase)
{
    cmd_send(state, FMCOMMS3_SET_DDS_PHASE);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    uchar_send(state, tone);
    uint_send(state, phase);

    return result_recv(state);
}

Result Fmcomms3_get_dds_phase_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t * phase)
{
    cmd_send(state, FMCOMMS3_GET_DDS_PHASE);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    uchar_send(state, tone);

    *phase = uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_set_dds_scale_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t scale)
{
    cmd_send(state, FMCOMMS3_SET_DDS_SCALE);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    uchar_send(state, tone);
    uint_send(state, scale);

    return result_recv(state);
}

Result Fmcomms3_get_dds_scale_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t * scale)
{
    cmd_send(state, FMCOMMS3_GET_DDS_SCALE);
    uint_send(state, uBoardNum-1);
    uchar_send(state, ch);
    uchar_send(state, tone);

    *scale = uint_recv(state);

    return result_recv(state);
}

Result Fmcomms3_spi_write_send(connection_state * state, unsigned uBoardNum, uint32_t reg, uint8_t val)
{
    cmd_send(state, FMCOMMS3_SPI_WRITE);
    uint_send(state, uBoardNum-1);
    uint_send(state, reg);
    uchar_send(state, val);

    return result_recv(state);
}

Result Fmcomms3_spi_read_send(connection_state * state, unsigned uBoardNum, uint32_t reg, uint8_t * val)
{
    cmd_send(state, FMCOMMS3_SPI_READ);
    uint_send(state, uBoardNum-1);
    uint_send(state, reg);

    *val = uchar_recv(state);

    return result_recv(state);
}

