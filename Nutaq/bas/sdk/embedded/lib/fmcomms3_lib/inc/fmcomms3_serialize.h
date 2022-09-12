#ifndef __FMCOMMS3_SERIALIZE__
#define __FMCOMMS3_SERIALIZE__
#include "connection_state.h"
#include "bas_error.h"
#include "ad9361_api.h"
#include "ad9361.h"

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
 * @file    fmcomms3_serialize.c 
 * @brief   Serialize data structure of Radio420/Radio421 for use CCE/EAPI
 * 
 * (c) 2012, Nutaq. All rights reserved. 
 * 
 ****************************************************************************/

#include "fmcomms3_serialize.h"
#include "eapi_serialize.h"
#include "ad9361_api.h"

Result fmcomms3_initparam_struct_recv(connection_state * state, AD9361_InitParam * param);
Result fmcomms3_initparam_struct_send(connection_state * state, AD9361_InitParam * param);
Result fmcomms3_rf_rssi_struct_recv(connection_state * state, struct rf_rssi * rssi);
Result fmcomms3_rf_rssi_struct_send(connection_state * state, struct rf_rssi * rssi);
Result fmcomms3_rxfirconfig_struct_recv(connection_state * state, AD9361_RXFIRConfig * config);
Result fmcomms3_rxfirconfig_struct_send(connection_state * state, AD9361_RXFIRConfig * config);
Result fmcomms3_txfirconfig_struct_recv(connection_state * state, AD9361_TXFIRConfig * config);
Result fmcomms3_txfirconfig_struct_send(connection_state * state, AD9361_TXFIRConfig * config);
Result fmcomms3_calib_result_struct_recv(connection_state * state, struct sAd9361CalibResult * pCalibResult);
Result fmcomms3_calib_result_struct_send(connection_state * state, struct sAd9361CalibResult * pCalibResult);

#endif
