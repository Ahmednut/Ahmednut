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
 * Copyright (C) 2015, Nutaq, Canada. All rights reserved.           
 *
 * @ingroup DRIVERS   
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

#ifndef DEVCOM_H__
#define DEVCOM_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "bas_types.h"
#include "bas_error.h"
#include "DevCom_defines.h"

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

Result DevCom_Open(DevCom_hCom * phCom, void * hComCore, DevCom_pfWriteArray_t pfWriteArray, DevCom_pfReadArray_t pfReadArray);
Result DevCom_Close(DevCom_hCom * phCom);
Result DevCom_WriteArray(DevCom_hCom hCom, uint32_t u32DeviceId, uint16_t u16Addr, const void * pData, int32_t i32Size);
Result DevCom_ReadArray(DevCom_hCom hCom, uint32_t u32DeviceId, uint16_t u16Addr, void * pData, int32_t i32Size);
Result DevCom_Write(DevCom_hCom hCom, uint32_t u32DeviceId, uint16_t u16Addr, const void * pData);
Result DevCom_Read(DevCom_hCom hCom, uint32_t u32DeviceId, uint16_t u16Addr, void * pData);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

