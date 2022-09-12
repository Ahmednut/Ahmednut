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
 * @file    ppssync.h
 * @brief   PPSSYNC functions
 *
 * $Date: 2015/11/06 18:46:47 $
 * $Revision: 1.5 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup PPSSYNC
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

#ifndef __PPSSYNC_
#define __PPSSYNC__
#include "bas_error.h"
#include "ppssync_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
* Function: PPSSync_Open
*****************************************************************************
*
* Opens a PPSSync instance
*
* @param[in,out] PPSSync
*      Address of a PPSSync pointer that will hold the information of the 
*      allocated PPS Sync object.
*
* @param[in]   PPSSyncBaseAdd
*      Base address on the AXI bus of the MI250 FPGA core
*
* @param[in]   pfDriveDac
*      Function pointer that will be called to update the DAC output value
*      that tune the oscillator frequency. The function must have the same
*      prototype than PPSSync_pfDriveDac_t defined type.
*
* @param[in]   hBoard
*      Board handle that will be used as the first argument of the pfDriveDac
*      function.
*
* Error code: #PPSSync if no problem is found, !=#PPSSync if problem.
*
****************************************************************************/
Result PPSSync_Open(PPSSync_t **PPSSync, unsigned int PPSSyncBaseAdd, 
    PPSSync_pfDriveDac_t pfDriveDac, void * hBoard);
   
/****************************************************************************
* Function: PPSSync_Configure
*****************************************************************************
*
* Configure PPS Sync parameters.
*
* @param[in]   PPSSync
*      Pointer to an PPSSync structure describing the device to use.
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
Result PPSSync_Configure(PPSSync_t *PPSSync,
    float fMinDAC, float fMaxDAC, unsigned int uPpsFreq);
    
/****************************************************************************
* Function: PPSSync_Close
*****************************************************************************
*
* Closes an instance of the PPSSync.
*
* @param[in]   PPSSync
*      Pointer to an PPSSync structure describing the device to use.
*
* Error code: #PPSSync if no problem is found, !=#PPSSync if problem.
*
****************************************************************************/
Result PPSSync_Close(PPSSync_t *PPSSync);
/****************************************************************************
* Function: PPSSync_PPSDetect
*****************************************************************************
*
* Detect PPS pulse
*
* @param[in]   PPSSync
*      Pointer to an PPSSync structure describing the device to use.
*
* Error code: #PPSSync if no problem is found, !=#PPSSync if problem.
*
****************************************************************************/
Result PPSSync_PPSDetect(PPSSync_t *PPSSync);
/****************************************************************************
* Function: PPSSync_Reset
*****************************************************************************
*
* Reset PPS-SYNC core
*
* @param[in]   PPSSync
*      Pointer to an PPSSync structure describing the device to use.
*
* Error code: #PPSSync if no problem is found, !=#PPSSync if problem.
*
****************************************************************************/
void PPSSync_Reset(PPSSync_t *PPSSync);
/****************************************************************************
* Function: PPSSync_Presence
*****************************************************************************
*
* Detect a valid instance of the PPSSync and return core ID and version
*
* @param[in]   PPSSync
*      Pointer to an PPSSync structure describing the device to use.
*
* @param[out]   id
*		PPSSync core ID code
*
* @param[out]   ver
*		PPSSync core version code
* Error code: #PPSSync if no problem is found, !=#PPSSync if problem.
*
****************************************************************************/
Result PPSSync_Presence(PPSSync_t *PPSSync, unsigned int *id, unsigned int *ver);

/****************************************************************************
* Function: PPSSync_DacLimiter
*****************************************************************************
*
* Initialize DAC with value
*
* @param[in]   PPSSync
*      Pointer to an S_PPSSync structure describing the device to use.
*
* @param[in]   DacVal
*      Minimum value of DAC
*
* Error code: #0 if no problem is found, !=0 if problem.
*
****************************************************************************/
Result PPSSync_DacInit(PPSSync_t *PPSSync, float fDacVal);

/****************************************************************************
* Function: PPSSync_GetInfo
*****************************************************************************
*
* Get all information 
*
* @param[in]   PPSSync
*      Pointer to an S_PPSSync structure describing the device to use.
*
* @param[out]   PPSSyncInfo
*      Point to PPSSyncInfo_t struct
*
* Error code: #0 if no problem is found, !=0 if problem.
*
****************************************************************************/
Result PPSSync_GetInfo(PPSSync_t *PPSSync, PPSSync_Info_t *PPSSyncInfo);

/****************************************************************************
* Function: PPSSync_ReadRefDacVal
*****************************************************************************
*
* Load stable ref DAC value from the carrier memory.
*
* @param[out]   pfStableDac
*      Pointer to DAC value stored on the carrier memory.
*
* Error code: #0 if no problem is found, !=0 if problem.
*
****************************************************************************/
Result PPSSync_ReadRefDacVal(float * pfStableDac);

/****************************************************************************
* Function: PPSSync_SaveRefDacVal
*****************************************************************************
*
* Save stable ref DAC value to the carrier memory.
*
* @param[in]   StableDac
*      Stable DAC value
*
* Error code: #0 if no problem is found, !=0 if problem.
*
****************************************************************************/
Result PPSSync_SaveRefDacVal(float fStableDac);

/****************************************************************************
* Function: PPSSync_ReadPIProfile
*****************************************************************************
*
* Load stable PI profile value from FMC EEPROM
*
* @param[out]   Kp
*      Pointer to Kp stored in FMC EEPROM
*
* @param[out]   Ki
*      Pointer to Ki stored in FMC EEPROM
*
* Error code: #0 if no problem is found, !=0 if problem.
*
****************************************************************************/
Result PPSSync_ReadPIProfile(float *Kp, float *Ki);
/****************************************************************************
* Function: PPSSync_SavePIProfile
*****************************************************************************
*
* Save stable PI profile value from FMC EEPROM
*
* @param[in]   Kp
*      Pointer to Kp stored in FMC EEPROM
*
* @param[in]   Ki
*      Pointer to Ki stored in FMC EEPROM
*
* Error code: #0 if no problem is found, !=0 if problem.
*
****************************************************************************/
Result PPSSync_SavePIProfile(float Kp, float Ki);
/****************************************************************************
* Function: PPSSync_Start
*****************************************************************************
*
* Start PPSSYNC algorithm
*
* @param[in]   PPSSync
*      Pointer to an S_PPSSync structure describing the device to use.
*
* @param[in]   TargetFreq
*      Target frequency to be tuned
*
* @param[in]   Kp
*      Proportional gain for PI controller
* @param[in]   Ki
*      Intergral gain for PI controller
*
* Error code: #0 if no problem is found, !=0 if problem.
*
****************************************************************************/
Result PPSSync_Start(PPSSync_t *PPSSync, unsigned int TargetFreq, float Kp, float Ki, unsigned int IntegrationTime);
/****************************************************************************
* Function: PPSSync_Stop
*****************************************************************************
*
* Stop PPSSYNC algorithm
*
* @param[in]   PPSSync
*      Pointer to an S_PPSSync structure describing the device to use.
*
*
****************************************************************************/
Result PPSSync_Stop(PPSSync_t *PPSSync);
/****************************************************************************
* Function: PPSSync_Tick
*****************************************************************************
*
* Update PI controller thread
*
* @param[in]   PPSSync
*      Pointer to an S_PPSSync structure describing the device to use.
*
****************************************************************************/
Result PPSSync_Tick(PPSSync_t *PPSSync);


#ifdef __cplusplus
}
#endif

#endif

