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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       DevCom.h
 * @brief      communication wrapper function definitions for all devices control
 *
 * $Date: 2015/12/07 21:24:12 $
 * $Revision: 1.4 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#ifndef RADIO640DEVCOM_H__
#define RADIO640DEVCOM_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "bas_types.h"
#include "bas_error.h"
#include "Radio640DevCom_defines.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

Result Radio640DevCom_Open(Radio640DevCom_hComCore * phCom,
    Carrier_handle_t hCarrier, unsigned int Zone,
    uint32_t u32TrxClkSpiRegAddr,
    uint32_t u32TrxClkSpiReadRegAddr,
    uint32_t u32TrxClkSpiClkDownsamplingRegAddr,
    uint32_t u32DacOscSpiRegAddr,
    uint32_t u32RfAttSpiRegAddr,
    Radio640DevCom_stI2cAddr I2cAddr,
    uint32_t u8IsRevA);
Result Radio640DevCom_Close(Radio640DevCom_hComCore * phCom);
Result Radio640DevCom_WriteArray(void * hCom, uint32_t u32DeviceId, uint16_t u16Addr, const void * pData, int32_t i32Size);
Result Radio640DevCom_ReadArray(void * hCom, uint32_t u32DeviceId, uint16_t u16Addr, void * pData, int32_t i32Size);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

