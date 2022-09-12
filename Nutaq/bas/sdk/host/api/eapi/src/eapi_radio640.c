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
 * @file       eapi_radio640.c
 * @brief      radio640 eapi module related functions
 *
 * $Date: 2016/05/10 21:03:10 $
 * $Revision: 1.10 $
 *
 * (c) 2012, Nutaq Inc. All rights reserved.
 *
 * @ingroup RADIO640_RADIO640
 *
 ***************************************************************************/

#include "cce.h"
#include "eapi_serialize.h"

#include "bas_types.h"

#include <memory.h>
#include "Radio640.h"
#include "Radio640_defines.h"
#include "Radio640Serialize.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_internal_def.h"
#include "Ad9361Api.h"
#include "Ad9361.h"
#include "Ad9361_defines.h"

typedef struct Radio640_stRadio640
{
    // TCP/IP connection
    hConnectionState_t hConnectionState;
    
    // Radio640 instance number
    uint32_t u32BoardNum;
    
}Radio640_stRadio640;


Result Radio640_Open (Radio640_hRadio640 * phRadio640,
    uint32_t u32BaseAddr,
    Carrier_handle_t hCarrier)
{
    return eRadio640EmbedFuncOnly;
}

Result Radio640_Connect(Radio640_hRadio640 * phRadio640,
    hConnectionState_t hConnectionState,
    uint32_t u32BoardNum)
{
	Radio640_hRadio640 hRadio640;
    
    *phRadio640 = (Radio640_hRadio640)malloc(sizeof(Radio640_stRadio640));
    if (!*phRadio640) {
		return eRadio640OutOfMem;
	}
    memset(*phRadio640, 0, sizeof(Radio640_stRadio640));
    
    // Define a handle to easier understanding
    hRadio640 = *phRadio640;
    
    hRadio640->hConnectionState = hConnectionState;
    hRadio640->u32BoardNum = u32BoardNum;
    
    return eRadio640Ok;
}

void Radio640_Close(Radio640_hRadio640 * phRadio640)
{
    if(*phRadio640 != NULL)
    {
        free(*phRadio640);
        *phRadio640 = NULL;
    }
}

Result Radio640_Presence(Radio640_hRadio640 hRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_PRESENCE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_PowerUp(Radio640_hRadio640 hRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_POWERUP);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_Init(Radio640_hRadio640 hRadio640, Radio640_stInitParam * pInitParam)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_INIT);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    Radio640Serialize_SendInitParamStruct(hRadio640->hConnectionState, pInitParam);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_DigitalCalibration(Radio640_hRadio640 hRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_DIGITAL_CALIBRATION);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetSyncOutputDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_SYNC_OUTPUT_DELAY);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    int_send(hRadio640->hConnectionState, i32Delay);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetSyncInputDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_SYNC_INPUT_DELAY);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    int_send(hRadio640->hConnectionState, i32Delay);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRefClkDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_REFCLK_DELAY);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    int_send(hRadio640->hConnectionState, i32Delay);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetSyncBitslip(Radio640_hRadio640 hRadio640, int32_t i32Bitslip)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_SYNC_BITSLIP);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    int_send(hRadio640->hConnectionState, i32Bitslip);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetSyncSrc(Radio640_hRadio640 hRadio640, Radio640_eSyncSrc Src)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_SYNC_SRC);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Src);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SoftSync(Radio640_hRadio640 hRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SOFT_SYNC);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_Mcs(Radio640_hRadio640 hRadio640, int32_t step)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_MCS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    int_send(hRadio640->hConnectionState, step);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetMasterCalibResult(Radio640_hRadio640 hRadio640, Radio640_hRadio640 hMasterRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_MASTER_CALIB_RESULT);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    int_send(hRadio640->hConnectionState, hMasterRadio640->u32BoardNum-1);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetExtMasterCalibResult(Radio640_hRadio640 hRadio640, Ad9361_stCalibResult ExtCalibResult)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_EXT_MASTER_CALIB_RESULT);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    Radio640Serialize_SendCalibResultStruct(hRadio640->hConnectionState, &ExtCalibResult);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetCalibResult(Radio640_hRadio640 hRadio640, Ad9361_stCalibResult * pCalibResult)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_CALIB_RESULT);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    Radio640Serialize_RecvCalibResultStruct(hRadio640->hConnectionState, pCalibResult);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetEnsmMode(Radio640_hRadio640 hRadio640, Ad9361_eEnsmMode Mode)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_EN_STATE_MACHINE_MODE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uint_send(hRadio640->hConnectionState, (uint32_t)Mode);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetEnsmMode(Radio640_hRadio640 hRadio640, Ad9361_eEnsmMode * pMode)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_EN_STATE_MACHINE_MODE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pMode = (Ad9361_eEnsmMode)uint_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetEnsmState (Radio640_hRadio640 hRadio640, Ad9361_eEnsmState * pEnsmState)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_EN_STATE_MACHINE_STATE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pEnsmState = (Ad9361_eEnsmState)uint_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetSamplingFreq (Radio640_hRadio640 hRadio640, uint32_t u32SamplingFreq)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_SAMPLING_FREQ);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uint_send(hRadio640->hConnectionState, u32SamplingFreq);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetSamplingFreq (Radio640_hRadio640 hRadio640, uint32_t * pu32SamplingFreq)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_SAMPLING_FREQ);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu32SamplingFreq = uint_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxLoFreq (Radio640_hRadio640 hRadio640, uint64_t u64LoFreq)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_LO_FREQ);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uint64_send(hRadio640->hConnectionState, u64LoFreq);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTxLoFreq (Radio640_hRadio640 hRadio640, uint64_t * pu64LoFreq)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TX_LO_FREQ);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu64LoFreq = uint64_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxRfGain (Radio640_hRadio640 hRadio640, uint8_t u8Channel, int32_t i32Gain)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_RF_GAIN);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);
    int_send(hRadio640->hConnectionState, i32Gain);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxRfGain (Radio640_hRadio640 hRadio640,uint8_t u8Channel, int32_t * pi32Gain)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_RF_GAIN);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);

    *pi32Gain = int_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxRfBandwidth (Radio640_hRadio640 hRadio640, uint32_t u32Bandwidth)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_RF_BANDWIDTH);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uint_send(hRadio640->hConnectionState, u32Bandwidth);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxRfBandwidth (Radio640_hRadio640 hRadio640, uint32_t *pu32Bandwidth)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_RF_BANDWIDTH);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu32Bandwidth = uint_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxLoFreq (Radio640_hRadio640 hRadio640, uint64_t u64LoFreq)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_LO_FREQ);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uint64_send(hRadio640->hConnectionState, u64LoFreq);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxLoFreq (Radio640_hRadio640 hRadio640, uint64_t *pu64LoFreq)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_LO_FREQ);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu64LoFreq = uint64_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxRssi (Radio640_hRadio640 hRadio640, uint8_t u8Channel, struct Ad9361_stRfRssi *pRfRssi)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_RSSI);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);

    Radio640Serialize_RecvRfRssiStruct(hRadio640->hConnectionState, pRfRssi);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxGainControlMode (Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8GcMode)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_GAIN_CONTROL_MODE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);
    uchar_send(hRadio640->hConnectionState, u8GcMode);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxGainControlMode (Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t * pu8GcMode)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_GAIN_CONTROL_MODE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);

    *pu8GcMode = uchar_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxFirConfig (Radio640_hRadio640 hRadio640, Ad9361Api_stRxFirConfig Config)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_FIR_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    Radio640Serialize_SendRxFirConfigStruct(hRadio640->hConnectionState, &Config);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxFirConfig(Radio640_hRadio640 hRadio640, uint8_t u8Channel, Ad9361Api_stRxFirConfig * pConfig)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_FIR_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);

    Radio640Serialize_RecvRxFirConfigStruct(hRadio640->hConnectionState, pConfig);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxFirEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_FIR_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxFirEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_FIR_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu8Enable = uchar_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxRfdcTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_RFDC_TRACK_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxRfdcTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_RFDC_TRACK_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu8Enable = uchar_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxBbdcTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_BBDC_TRACK_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxBbdcTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_BBDC_TRACK_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu8Enable = uchar_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxQuadTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_QUAD_TRACK_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetRxQuadTrackEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_RX_QUAD_TRACK_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu8Enable = uchar_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxAttenuation (Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint32_t u32Attenuation)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_ATTENUATION);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);
    uint_send(hRadio640->hConnectionState, u32Attenuation);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTxAttenuation (Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint32_t * pu32Attenuation)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TX_ATTENUATION);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);

    *pu32Attenuation = uint_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxRfBandwidth (Radio640_hRadio640 hRadio640, uint32_t u32Bandwidth)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_RF_BANDWIDTH);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uint_send(hRadio640->hConnectionState, u32Bandwidth);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTxRfBandwidth (Radio640_hRadio640 hRadio640, uint32_t * pu32Bandwidth)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TX_RF_BANDWIDTH);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu32Bandwidth = uint_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxFirConfig (Radio640_hRadio640 hRadio640, Ad9361Api_stTxFirConfig Config)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_FIR_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    Radio640Serialize_SendTxFirConfigStruct(hRadio640->hConnectionState, &Config);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTxFirConfig(Radio640_hRadio640 hRadio640, uint8_t u8Channel, Ad9361Api_stTxFirConfig * pConfig)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TX_FIR_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Channel);

    Radio640Serialize_RecvTxFirConfigStruct(hRadio640->hConnectionState, pConfig);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxFirEnDis (Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_FIR_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    uchar_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTxFirEnDis (Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TX_FIR_EN_DIS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu8Enable = uchar_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTrxPathClks(Radio640_hRadio640 hRadio640,
	uint32_t * pu32RxPathClks,
	uint32_t * pu32TxPathClks)
{
    int i;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TRX_PATH_CLKS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    for(i=0; i<6; i++) uint32_send(hRadio640->hConnectionState, pu32RxPathClks[i]);
    for(i=0; i<6; i++) uint32_send(hRadio640->hConnectionState, pu32TxPathClks[i]);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTrxPathClks(Radio640_hRadio640 hRadio640,
	uint32_t * pu32RxPathClks,
	uint32_t * pu32TxPathClks)
{
    int i;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TRX_PATH_CLKS);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    for(i=0; i<6; i++) pu32RxPathClks[i] = uint32_recv(hRadio640->hConnectionState);
    for(i=0; i<6; i++) pu32TxPathClks[i] = uint32_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxRfFilter(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, Radio640_eRxRfFilter Filter)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_RF_FILTER);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Channel);
    uint_send(hRadio640->hConnectionState, (uint32_t)Filter);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_Write(Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device, uint16_t u16Addr, uint32_t u32Data)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_WRITE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Device);
    uint16_send(hRadio640->hConnectionState, u16Addr);
    uint_send(hRadio640->hConnectionState, u32Data);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_Read(Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device, uint16_t u16Addr, uint32_t * pu32Data)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_READ);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Device);
    uint16_send(hRadio640->hConnectionState, u16Addr);

    *pu32Data = uint_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_DisableDesignClk(Radio640_hRadio640 hRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_DISABLE_DESIGN_CLK);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_EnableDesignClk(Radio640_hRadio640 hRadio640)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_ENABLE_DESIGN_CLK);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    return result_recv(hRadio640->hConnectionState);
}
 
Result Radio640_IsClockMaster(Radio640_hRadio640 hRadio640, uint8_t * pu8ClockMaster)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_IS_CLOCK_MASTER);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    *pu8ClockMaster = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}


Result Radio640_SetClkSrc(Radio640_hRadio640 hRadio640, Radio640_eClkSrc ClkSrc)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_CLK_SRC);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)ClkSrc);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetClkSrc(Radio640_hRadio640 hRadio640, Radio640_eClkSrc * pClkSrc)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_CLK_SRC);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pClkSrc = (Radio640_eClkSrc)uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetBoardTemp(Radio640_hRadio640 hRadio640, int16_t *pi16Temp)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_BOARD_TEMP);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pi16Temp = int16_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTrxTemp(Radio640_hRadio640 hRadio640, int16_t *pi16Temp)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TRX_TEMP);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pi16Temp = int16_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetEnAgcPinConfig(Radio640_hRadio640 hRadio640, uint8_t ui8Force, uint8_t ui8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_EN_AGC_PIN_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, ui8Force);
    uint8_send(hRadio640->hConnectionState, ui8Value);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetEnablePinConfig(Radio640_hRadio640 hRadio640, uint8_t ui8Force, uint8_t ui8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_ENABLE_PIN_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, ui8Force);
    uint8_send(hRadio640->hConnectionState, ui8Value);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxnrxPinConfig(Radio640_hRadio640 hRadio640, uint8_t ui8Force, uint8_t ui8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TXNRX_PIN_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, ui8Force);
    uint8_send(hRadio640->hConnectionState, ui8Value);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetCtrlInPinConfig(Radio640_hRadio640 hRadio640, uint8_t ui8Force, uint8_t ui8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_CTRL_IN_PIN_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, ui8Force);
    uint8_send(hRadio640->hConnectionState, ui8Value);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetCtrlOutPinValue(Radio640_hRadio640 hRadio640, uint8_t * pui8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_CTRL_OUT_PIN_VALUE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pui8Value = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetCtrlOutPinLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pui8LowValue, uint8_t * pui8HighValue)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_CTRL_OUT_PIN_LOW_HIGH_VALUE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pui8LowValue = uint8_recv(hRadio640->hConnectionState);
    *pui8HighValue = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetCtrlOutConfig(Radio640_hRadio640 hRadio640, uint8_t ui8Config, uint8_t ui8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_CTRL_OUT_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, ui8Config);
    uint8_send(hRadio640->hConnectionState, ui8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetPowerMonitorMode(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eMode Mode, uint8_t u8IsBlocking)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_POWER_MONITOR_MODE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Device);
    uint_send(hRadio640->hConnectionState, (uint32_t)Mode);
    uint8_send(hRadio640->hConnectionState, u8IsBlocking);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetPowerMonitorConversionTime(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eConvTime BusConvTime, Ina226_eConvTime ShuntConvTime)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_POWER_MONITOR_CONVERSION_TIME);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Device);
    uint_send(hRadio640->hConnectionState, (uint32_t)BusConvTime);
    uint_send(hRadio640->hConnectionState, (uint32_t)ShuntConvTime);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetPowerMonitorAveraging(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eAveraging Averaging)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_POWER_MONITOR_AVERAGING);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Device);
    uint_send(hRadio640->hConnectionState, (uint32_t)Averaging);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_ReadPowerMonitorBusVoltage(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfVoltage)
{    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_READ_POWER_MONITOR_BUS_VOLTAGE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    uint_send(hRadio640->hConnectionState, (uint32_t)Device);
    
    *pfVoltage = float_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_ReadPowerMonitorPower(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfPower)
{    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_READ_POWER_MONITOR_BUS_POWER);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    uint_send(hRadio640->hConnectionState, (uint32_t)Device);
    
    *pfPower = float_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_ReadPowerMonitorCurrent(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfCurrent)
{    
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_READ_POWER_MONITOR_BUS_CURRENT);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    uint_send(hRadio640->hConnectionState, (uint32_t)Device);
    
    *pfCurrent = float_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxExtAttenuation(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint32_t u32Attenuation)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_EXT_ATTENUATION);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Channel);
    uint32_send(hRadio640->hConnectionState, u32Attenuation);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_TuneOsc(Radio640_hRadio640 hRadio640, float fValue)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_TUNE_OSC);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    float_send(hRadio640->hConnectionState, fValue);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxEnableState(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_ENABLE_STATE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Channel);
    uint8_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxEnableState(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_ENABLE_STATE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Channel);
    uint8_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_StoreRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_STORE_RX_FAST_LOCK_PROFILE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Profile);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_RecallRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_RECALL_RX_FAST_LOCK_PROFILE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Profile);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_LoadRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values)
{
    int i;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_LOAD_RX_FAST_LOCK_PROFILE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Profile);
    for(i=0; i<16; i++) uint8_send(hRadio640->hConnectionState, pu8Values[i]);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SaveRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values)
{
    int i;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SAVE_RX_FAST_LOCK_PROFILE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Profile);
    
    for(i=0; i<16; i++) pu8Values[i] = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_StoreTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_STORE_TX_FAST_LOCK_PROFILE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Profile);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_RecallTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_RECALL_TX_FAST_LOCK_PROFILE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Profile);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_LoadTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values)
{
    int i;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_LOAD_TX_FAST_LOCK_PROFILE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Profile);
    for(i=0; i<16; i++) uint8_send(hRadio640->hConnectionState, pu8Values[i]);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SaveTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values)
{
    int i;
    
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SAVE_TX_FAST_LOCK_PROFILE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Profile);
    
    for(i=0; i<16; i++) pu8Values[i] = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRxLoExt(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RX_LO_EXT);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxLoExt(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_LO_EXT);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTxAutoCalibEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TX_AUTO_CALIB_ENABLE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTxAutoCalibEnable(Radio640_hRadio640 hRadio640, uint8_t * pu8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TX_AUTO_CALIB_ENABLE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pu8Enable = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_DoCalib(Radio640_hRadio640 hRadio640, Radio640_eCalib Calib, int32_t i32Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_DO_CALIB);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint_send(hRadio640->hConnectionState, (uint32_t)Calib);
    int32_send(hRadio640->hConnectionState, i32Value);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRfSwitchConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8TrxInRxDir)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_RF_SWITCH_CONFIG);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Force);
    uint8_send(hRadio640->hConnectionState, u8TrxInRxDir);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTriggerDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TRIGGER_DELAY);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    int32_send(hRadio640->hConnectionState, i32Delay);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetTriggerBitslip(Radio640_hRadio640 hRadio640, int32_t i32Bitslip)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_TRIGGER_BITSLIP);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    int32_send(hRadio640->hConnectionState, i32Bitslip);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_ReadClkFreq(Radio640_hRadio640 hRadio640, Radio640_eTestClk Clk, uint32_t * pu32Freq)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_READ_CLK_FREQ);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint32_send(hRadio640->hConnectionState, (uint32_t)Clk);
    
    *pu32Freq = uint32_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTriggerValue(Radio640_hRadio640 hRadio640, uint8_t * pu8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TRIGGER_VALUE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pu8Value = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetTriggerLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_TRIGGER_LOW_HIGH_VALUE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pu8LowValue = uint8_recv(hRadio640->hConnectionState);
    *pu8HighValue = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetInputSyncValue(Radio640_hRadio640 hRadio640, uint8_t * pu8Value)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_INPUT_SYNC_VALUE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pu8Value = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetInputSyncLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_INPUT_SYNC_LOW_HIGH_VALUE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    *pu8LowValue = uint8_recv(hRadio640->hConnectionState);
    *pu8HighValue = uint8_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}



Result Radio640_SetClkToExtendedFmcEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_CLK_TO_EXTENDED_FMC_ENABLE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetRefOutEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
        
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_REF_OUT_ENABLE);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Enable);

    return result_recv(hRadio640->hConnectionState);
}







Result Radio640_SetComplexMultiplier(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint16_t u16RealCoefficient, uint16_t u16ImgCoefficient)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_COMPLEX_MULTIPLIER);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Channel);
    uint8_send(hRadio640->hConnectionState, u8Direction);
    uint16_send(hRadio640->hConnectionState, u16RealCoefficient);
    uint16_send(hRadio640->hConnectionState, u16ImgCoefficient);
    
    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetComplexMultiplier(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint16_t *u16RealCoefficient, uint16_t *u16ImgCoefficient)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_COMPLEX_MULTIPLIER);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    uint8_send(hRadio640->hConnectionState, u8Channel);
    uint8_send(hRadio640->hConnectionState, u8Direction);
    
    *u16RealCoefficient = uint16_recv(hRadio640->hConnectionState);
    *u16ImgCoefficient = uint16_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_SetDataDelay(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint32_t u32DataDelay)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_SET_DATA_DELAY);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);
    
    uint8_send(hRadio640->hConnectionState, u8Channel);
    uint8_send(hRadio640->hConnectionState, u8Direction);
    uint32_send(hRadio640->hConnectionState, u32DataDelay);
    
    return result_recv(hRadio640->hConnectionState);
}

Result Radio640_GetDataDelay(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint32_t *u32DataDelay)
{
    if(hRadio640 == NULL) return eRadio640InvHandle;
    
    cmd_send(hRadio640->hConnectionState, RADIO640_GET_DATA_DELAY);
    uint_send(hRadio640->hConnectionState, hRadio640->u32BoardNum-1);

    uint8_send(hRadio640->hConnectionState, u8Channel);
    uint8_send(hRadio640->hConnectionState, u8Direction);
    
    *u32DataDelay = uint32_recv(hRadio640->hConnectionState);

    return result_recv(hRadio640->hConnectionState);
}


