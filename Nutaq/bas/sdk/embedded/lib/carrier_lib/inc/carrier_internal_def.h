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
 * @file       carrier_internal_def.h
 * @brief      carrier internal generic library structure and data definitions.
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.4 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup CARRIERLIB
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef __CARRIER_INTERNAL_DEF_H__
#define __CARRIER_INTERNAL_DEF_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"

#include "carrier_service_def.h"
#include "carrier_fmc_service_def.h"
#include "FMCEEprom_eeprom_def.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/
typedef Result (*pfCarrier_Close_t)(Carrier_handle_t hCarrier);

typedef Result (*pfCarrier_FmcGetI2cZone_t)(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, unsigned int * I2cZone);
typedef Result (*pfCarrier_I2cRead_t)(Carrier_handle_t hCarrier, unsigned int zone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options);
typedef Result (*pfCarrier_I2cWrite_t)(Carrier_handle_t hCarrier, unsigned int zone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options);
typedef Result (*pfCarrier_I2cReadNoWrData_t)(Carrier_handle_t hCarrier, unsigned int zone, unsigned char i2cDeviceAddr, unsigned char * data, unsigned int size);
typedef Result (*pfCarrier_I2cCheckRaw_t)(Carrier_handle_t hCarrier, unsigned int zone, unsigned char ucDevAddr, unsigned char * pucDetected);

typedef Result (*pfCarrier_GetModuleBaseAddr_t)(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int index, unsigned int * addr);
typedef Result (*pfCarrier_GetModuleSize_t)(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * size);
typedef Result (*pfCarrier_GetModuleMaxNumber_t)(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * num);

typedef Result (*pfCarrier_GetMemSpaceBaseAddr_t)(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * addr);
typedef Result (*pfCarrier_GetMemSpaceSize_t)(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * size);

typedef Result (*pfCarrier_FmcGetGA_t)(Carrier_eFmcConnectorNum_t connector, unsigned char * ga);

typedef Result (*pfCarrier_GetSignalsInfo_t)(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, FMCEEPROM_HPC_DATA_Specs **ppSignal);

typedef Result (*pfCarrier_CustomRegisterWrite_t)(Carrier_handle_t hCarrier, unsigned int index, unsigned int value);
typedef Result (*pfCarrier_CustomRegisterRead_t)(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value);

typedef Result (*pfCarrier_LedSetValue_t)(Carrier_handle_t hCarrier, unsigned int index, unsigned int data);
typedef Result (*pfCarrier_LedGetValue_t)(Carrier_handle_t hCarrier, unsigned int index, unsigned int * data);
typedef Result (*pfCarrier_Reboot_t)(Carrier_handle_t hCarrier);

// Clock control functions
typedef Result (*pfCarrier_TClkSetState_t)(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t state);
typedef Result (*pfCarrier_TClkGetState_t)(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t * state);
typedef Result (*pfCarrier_TClkSetTxSource_t)(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t source);
typedef Result (*pfCarrier_TClkGetTxSource_t)(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t * source);
typedef Result (*pfCarrier_FmcBidirClkSetConnection_t)(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t connection);
typedef Result (*pfCarrier_FmcBidirClkGetConnection_t)(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t * connection);
typedef Result (*pfCarrier_SetOscState_t)(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t state);
typedef Result (*pfCarrier_GetOscState_t)(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t * state);
typedef Result (*pfCarrier_FClkASetConnection_t)(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t connection);
typedef Result (*pfCarrier_FClkAGetConnection_t)(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t * connection);

typedef Result (*pfCarrier_FmcSetVadj_t)(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board, Carrier_eVadjValue_t Value);
typedef Result (*pfCarrier_FmcAssignSlot_t)(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector);

/** @name Carrier information type
 * 
 *  Hold information of what kind of info and service could be requested from carrier   
 */
typedef struct sCarrier {
    
    Carrier_eType_t Type;
    unsigned int uFmcConnectorNumMax;
    unsigned int uFmcBoardNumMax;
    
    volatile void * pCore;
    
    unsigned int bInit;
    unsigned int bDiscover;
    
    Carrier_sSlotInfo_t * pSlot;            // A carrier will have the Carrier_sSlotInfo_t allocated uFmcConnectorNumMax times
    FMCEEPROM_EEprom_Decode * pEepromData;  // A carrier will have the FMCEEPROM_EEprom_Decode allocated uFmcConnectorNumMax times
    FMCEEprom_eepromdef * pEepromList;      // A carrier will have the FMCEEprom_eepromdef allocated uFmcConnectorNumMax times
    
    unsigned int uCoreNum;
    unsigned int uCoreNameNum;
    unsigned int uModuleNameNum;
    FMCEEprom_module_fpgacoreids * pCoreList;
    char ** ppcCoreNameList;
    char ** ppcModuleNameList;
    
    pfCarrier_Close_t pfCarrier_Close;
    
    pfCarrier_FmcGetI2cZone_t pfCarrier_FmcGetI2cZone;
    pfCarrier_I2cRead_t pfCarrier_I2cRead;
    pfCarrier_I2cWrite_t pfCarrier_I2cWrite;
    pfCarrier_I2cReadNoWrData_t pfCarrier_I2cReadNoWrData;
    pfCarrier_I2cCheckRaw_t pfCarrier_I2cCheckRaw;
    
    pfCarrier_GetModuleBaseAddr_t pfCarrier_GetModuleBaseAddr;
    pfCarrier_GetModuleSize_t pfCarrier_GetModuleSize;
    pfCarrier_GetModuleMaxNumber_t pfCarrier_GetModuleMaxNumber;
    
    pfCarrier_GetMemSpaceBaseAddr_t pfCarrier_GetMemSpaceBaseAddr;
    pfCarrier_GetMemSpaceSize_t pfCarrier_GetMemSpaceSize;
    
    pfCarrier_FmcGetGA_t pfCarrier_FmcGetGA;
    
    pfCarrier_GetSignalsInfo_t pfCarrier_GetSignalsInfo;
    
    pfCarrier_CustomRegisterWrite_t pfCarrier_CustomRegisterWrite;
    pfCarrier_CustomRegisterRead_t pfCarrier_CustomRegisterRead;
    
    pfCarrier_LedSetValue_t pfCarrier_LedSetValue;
    pfCarrier_LedGetValue_t pfCarrier_LedGetValue;
    pfCarrier_Reboot_t pfCarrier_Reboot;
    
    // Clock control function
    pfCarrier_TClkSetState_t pfCarrier_TClkSetState;
    pfCarrier_TClkGetState_t pfCarrier_TClkGetState;
    pfCarrier_TClkSetTxSource_t pfCarrier_TClkSetTxSource;
    pfCarrier_TClkGetTxSource_t pfCarrier_TClkGetTxSource;
    pfCarrier_FmcBidirClkSetConnection_t pfCarrier_FmcBidirClkSetConnection;
    pfCarrier_FmcBidirClkGetConnection_t pfCarrier_FmcBidirClkGetConnection;
    pfCarrier_SetOscState_t pfCarrier_SetOscState;
    pfCarrier_GetOscState_t pfCarrier_GetOscState;
    pfCarrier_FClkASetConnection_t pfCarrier_FClkASetConnection;
    pfCarrier_FClkAGetConnection_t pfCarrier_FClkAGetConnection;    
    
    Carrier_pfUsleep pfCarrier_usleep;
    
    pfCarrier_FmcSetVadj_t pfCarrier_FmcSetVadj;
    pfCarrier_FmcAssignSlot_t pfCarrier_FmcAssignSlot;
}sCarrier_t;

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

