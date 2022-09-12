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
 * @file    Radio640Serialize.c 
 * @brief   Serialize data structure of Radio640 for use CCE/EAPI
 * 
 * (c) 2012, Nutaq. All rights reserved. 
 * 
 ****************************************************************************/
 
#ifndef __RADIO640_SERIALIZE__
#define __RADIO640_SERIALIZE__

#include "bas_error.h"
#include "connection_state.h"
#include "eapi_serialize.h"

#include "Ad9361Api.h"
#include "Ad9361.h"
#include "Ad9361_defines.h"
#include "Radio640_defines.h"

Result Radio640Serialize_RecvInitParamStruct(connection_state * state, Radio640_stInitParam * pInitParam);
Result Radio640Serialize_SendInitParamStruct(connection_state * state, Radio640_stInitParam * pInitParam);
Result Radio640Serialize_RecvRfRssiStruct(connection_state * state, Ad9361_stRfRssi * pRfRssi);
Result Radio640Serialize_SendRfRssiStruct(connection_state * state, Ad9361_stRfRssi * pRfRssi);
Result Radio640Serialize_RecvRxFirConfigStruct(connection_state * state, Ad9361Api_stRxFirConfig * pConfig);
Result Radio640Serialize_SendRxFirConfigStruct(connection_state * state, Ad9361Api_stRxFirConfig * pConfig);
Result Radio640Serialize_RecvTxFirConfigStruct(connection_state * state, Ad9361Api_stTxFirConfig * pConfig);
Result Radio640Serialize_SendTxFirConfigStruct(connection_state * state, Ad9361Api_stTxFirConfig * pConfig);
Result Radio640Serialize_RecvCalibResultStruct(connection_state * state, Ad9361_stCalibResult * pCalibResult);
Result Radio640Serialize_SendCalibResultStruct(connection_state * state, Ad9361_stCalibResult * pCalibResult);

#endif
