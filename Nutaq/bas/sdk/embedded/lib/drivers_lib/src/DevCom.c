/****************************************************************************  
 *
 *    ****                              *                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    *****  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       DevCom.c
 * @brief      Communication wrapper for all specific communication core
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "bas_types.h"
#include "bas_error.h"
#include "DevCom.h"

/****************************************************************************
 *                   Local Structure Declaration                            *
 ****************************************************************************/

typedef struct DevCom_stCom {
    void * hComCore;
    DevCom_pfWriteArray_t pfWriteArray;
    DevCom_pfReadArray_t pfReadArray;
} DevCom_stCom;

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : DevCom_Open
 ************************************************************************//**
 *
 *   Open the DevCom module before use.
 *   This function must be called before using any other function of this module.
 *
 *   @param [out] phCom
 *      Returns a communication core handle
 *
 *   @param [in] hComCore
 *      Specific communication core handle, already opened
 *
 *   @param [in] pfWriteArray
 *      "Write Array" function of the specific communication core.
 *
 *   @param [in] pfReadArray
 *      "Write Read" function of the specific communication core.
 *
 *   @return   Error code: OK if no problem is found, !OK if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup DevCom
 *
 ***************************************************************************/
Result DevCom_Open(DevCom_hCom * phCom, void * hComCore, DevCom_pfWriteArray_t pfWriteArray, DevCom_pfReadArray_t pfReadArray)
{    
    DevCom_hCom hCom;

    // Allocate Generic Com wrapper structure
    hCom = (DevCom_hCom)malloc(sizeof(DevCom_stCom));
    if(hCom == NULL) return ERR_OUTOFMEMORY;
    memset(hCom,0,sizeof(DevCom_stCom));
    *phCom = hCom;
    
    hCom->hComCore = hComCore;
    hCom->pfWriteArray = pfWriteArray;
    hCom->pfReadArray = pfReadArray;

    return OK;
}

 /****************************************************************************
 *   NAME : DevCom_Close
 ************************************************************************//**
 *
 *   Close the DevCom module when not needed anymore.
 *   DevCom_Open must be called before using this function.
 *
 *   @param [in,out] phCom
 *      Pointer to the Handle returned by DevCom_Open function
 *
 *   @return   Error code: OK if no problem is found, !OK if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup DevCom
 *
 ***************************************************************************/
Result DevCom_Close(DevCom_hCom * phCom)
{
    if(*phCom)
    {
        free(*phCom);
        *phCom = NULL;
    }
    
    return OK;
}


 /****************************************************************************
 *   NAME : DevCom_WriteArray
 ************************************************************************//**
 *
 *   Write an array of data to a specified device. Data is MSB 1st with
 *   incrementing address.
 *   DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      Handle returned from DevCom_Open function
 *
 *   @param [in] u32DeviceId
 *      Which logical device to write. The device IDs are relative to the 
 *      specific communication core.
 *
 *   @param [in] u16Addr
 *      at which internal device address starting to write
 *
 *   @param [in] pData
 *      data buffer used for writing
 *
 *   @param [in] i32Size
 *      number of bytes to write
 *
 *   @return   Error code: OK if no problem is found, !OK if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup DEVCOM
 *
 ***************************************************************************/
Result DevCom_WriteArray(DevCom_hCom hCom, uint32_t u32DeviceId, uint16_t u16Addr, const void * pData, int32_t i32Size)
{
    if (hCom == NULL)
    return ERR_INVHANDLE;

    // Check if function is correct
    if (hCom->pfWriteArray == NULL)
    {
        return ERR_NOTIMPLEMENTED;
    }

    return hCom->pfWriteArray(hCom->hComCore, u32DeviceId, u16Addr, pData, i32Size);
}


 /****************************************************************************
 *   NAME : DevCom_ReadArray
 ************************************************************************//**
 *
 *   Read an array of data from a specified device. Data is MSB 1st with
 *   incrementing address.
 *   DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      Handle returned from DevCom_Open function
 *
 *   @param [in] u32DeviceId
 *      Which logical device to read. The device IDs are relative to the 
 *      specific communication core.
 *
 *   @param [in] u16Addr
 *      at which internal device address starting to read
 *
 *   @param [out] pData
 *      data buffer used for reading
 *
 *   @param [in] i32Size
 *      number of bytes to read
 *
 *   @return   Error code: OK if no problem is found, !OK if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup DEVCOM
 *
 ***************************************************************************/
Result DevCom_ReadArray(DevCom_hCom hCom, uint32_t u32DeviceId, uint16_t u16Addr, void * pData, int32_t i32Size)
{
    if (hCom == NULL)
    return ERR_INVHANDLE;

    // Check if function is correct
    if (hCom->pfReadArray == NULL)
    {
        return ERR_NOTIMPLEMENTED;
    }

    return hCom->pfReadArray(hCom->hComCore, u32DeviceId, u16Addr, pData, i32Size);
}

 /****************************************************************************
 *   NAME : DevCom_Write
 ************************************************************************//**
 *
 *   Write 1 data item to a specified device (size of data item vary according
 *   to the device).
 *   DevCom_Open must be called before using this function.
 *
 *   @param [in] hCom
 *      Handle returned from DevCom_Open function
 *
 *   @param [in] u32DeviceId
 *      Which logical device to write. The device IDs are relative to the 
 *      specific communication core.
 *
 *   @param [in] u16Addr
 *      at which internal device address starting to write
 *
 *   @param [in] pData
 *      data buffer used for writing
 *
 *   @return   Error code: OK if no problem is found, !OK if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup DEVCOM
 *
 ***************************************************************************/
Result DevCom_Write(DevCom_hCom hCom, uint32_t u32DeviceId, uint16_t u16Addr, const void * pData)
{
    // Use the array write routine with an item size of 1
    return DevCom_WriteArray(hCom, u32DeviceId, u16Addr, pData, 1);
}


 /****************************************************************************
 *   NAME : DevCom_Read
 ************************************************************************//**
 *
 *   Read 1 data item from a specified device. Data item size vary according
 *   to the device.
 *   DevCom_Open must be called before using this function.
 *
 *   @param [in] hCom
 *      Handle returned from DevCom_Open function
 *
 *   @param [in] u32DeviceId
 *      Which logical device to read. The device IDs are relative to the 
 *      specific communication core.
 *
 *   @param [in] u16Addr
 *      at which internal device address starting to read
 *
 *   @param [out] pData
 *      data buffer used for reading
 *
 *   @return   Error code: OK if no problem is found, !OK if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup DEVCOM
 *
 ***************************************************************************/
Result DevCom_Read(DevCom_hCom hCom, uint32_t u32DeviceId, uint16_t u16Addr, void * pData)
{
    return DevCom_ReadArray(hCom, u32DeviceId, u16Addr, pData, 1);
}

