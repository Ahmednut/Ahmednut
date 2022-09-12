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
 * @file       eapi_ppsync.h
 * @brief      PPSSYNC module related functions additionnal definitions
 *
 * $Date: 2015/11/06 20:47:30 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * This file is part of Nutaq’s BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq’s BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s BAS Software Suite under the GPL requires that your work based on
 * Nutaq’s BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef __PPSSYNC_SERIALIZE__
#define __PPSSYNC_SERIALIZE__

#ifdef __cplusplus
extern "C" {
#endif
#include "connection_state.h"
#include "ppssync_types.h"



/** @defgroup PPSSYNC PPSSYNC EAPI
 *  
 *  PPSSYNC module EAPI allows to control Radio420 VCXO based on GPS PPS pulses
 *               
 *  @file
 *     
 */


/****************************************************************************
* NAME : PPSSync_Presence_send
*************************************************************************//**
*
* Detect a valid instance of the PPSSync and return core ID and versione.  
* 
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
* @param[out] idcode
*      Core ID
*
* @param[out] ppsver
*      Core Version
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem. 
* 
***************************************************************************/ 
Result PPSSync_Presence_send(connection_state * state,  unsigned int *idcode, unsigned int *ppsver);

/****************************************************************************
* Function: PPSSync_Configure_send
*****************************************************************************
*
* Configure PPS Sync parameters.
* 
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
* @param[in]   fMinDAC
*      Minimum tuning oscillator value. This value must match the 
*      implementation of pfDriveDac function provided during the open function.
*
* @param[in]   fMaxDAC
*      Maximum tuning oscillator value. This value must match the 
*      implementation of pfDriveDac function provided during the open function.
*
* @param[in]   uPpsFreq
*      Frequency of input PPS rising edge event during one second.
*
* Error code: #PPSSync if no problem is found, !=#PPSSync if problem.
*
****************************************************************************/
Result PPSSync_Configure_send(connection_state * state,  float fMinDAC, float fMaxDAC, unsigned int uPpsFreq);

/****************************************************************************
* NAME : PPSSync_DacInit_send
*************************************************************************//**
*
* Limits value to be written to DAC 
* 
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
* @param[in]   fDacVal
*      Initialize DAC value
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem.
* 
***************************************************************************/ 
Result PPSSync_DacInit_send(connection_state * state,  float fDacVal);

/****************************************************************************
* Function: PPSSync_Start_send
*************************************************************************//**
*
* Start PPSSYNC algorithm
*
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
* @param[in]   vcxo_freq
*      target frequency
*
* @param[in]   integration_time
*      PI controller integration time
*
* @param[in]   Kp
*      PI controller proportional gain
*
* @param[in]   Ki
*      PI controller integral gain
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem.
*
****************************************************************************/
Result PPSSync_Start_send(connection_state * state, unsigned int vcxo_freq, unsigned int integration_time, float Kp, float Ki);
/****************************************************************************
* Function: PPSSync_Stop_send
*************************************************************************//**
*
* Stop PPSSYNC algorithm
*
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem.
*
****************************************************************************/
Result PPSSync_Stop_send(connection_state * state);

/****************************************************************************
* Function: PPSSync_ReadRefDacVal_send
*************************************************************************//**
*
* Load stable ref DAC value on the carrier memory.
*
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
* @param[out]   pfRefDacVal
*      DAC value stored on the carrier memory.
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem.
*
****************************************************************************/
Result PPSSync_ReadRefDacVal_send(connection_state * state, float * pfRefDacVal);

/****************************************************************************
* Function: PPSSync_SaveRefDacVal_send
*************************************************************************//**
*
* Save stable ref DAC value to the carrier memory.
*
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem.
*
****************************************************************************/
Result PPSSync_SaveRefDacVal_send(connection_state * state);

/****************************************************************************
* Function: PPSSync_ReadPIProfile_send
*************************************************************************//**
*
* Load stable PI profile value from FMC EEPROM
*
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
* @param[out]   Kp
*      PI controller proportional gain
*
* @param[out]   Ki
*      PI controller integral gain
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem.
*
****************************************************************************/
Result PPSSync_ReadPIProfile_send(connection_state * state, float *Kp, float *Ki);
/****************************************************************************
* Function: PPSSync_SavePIProfile_recv
*************************************************************************//**
*
* Save stable PI profile value from FMC EEPROM
*
* @param [in] state
*    This structure is the one returned with BusAccess_GetConnectionState().
*
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem.
*
****************************************************************************/
Result PPSSync_SavePIProfile_send(connection_state * state);
/****************************************************************************
* Function: PPSSync_GetInfo
*************************************************************************//**
*
* Get all information 
*
* @param[in]   PPSSync
*      Pointer to an S_PPSSync structure describing the device to use.
*
* @param[out]   PPSSyncInfo
*      Point to PPSSyncInfo_t struct
*
*    Error code: #PPSYNCOK if no problem is found, !=#PPSYNCOK if problem.
*
****************************************************************************/
Result PPSSync_GetInfo_send(connection_state * state, PPSSync_Info_t *PPSSyncInfo);

#ifdef __cplusplus
}
#endif



#endif

