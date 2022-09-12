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
 * @file       Mo1000DevCom.h
 * @brief      communication wrapper function definitions for all devices control
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.3 $
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
#ifndef MO1000DEVCOM_H__
#define MO1000DEVCOM_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "bas_types.h"
#include "bas_error.h"
#include "Mo1000DevCom_defines.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : Mo1000DevCom_Open
 ************************************************************************//**
 *
 *   Open the Mo1000DevCom module before use.
 *   This function must be called before using any other function of this module.
 *
 *   @param [out] pCore
 *      returns a communication core data structure
 *
 *   @param [in] hCarrier
 *      Handle to a carrier object. It is the carrier that will handle the I2C
 *      communication.
 *   
 *   @param [in] zone
 *      Integer that indicates in which I2C zone is the I2C device
 *      is located.
 *
 *   @param [in] ucBridgeAddr
 *      7 bits I2C address of I2C/SPI bridge device
 *
 *   @param [in] ucSensorAddr
 *      7 bits I2C address of temperature sensor device
 *
 *   @param [in] ucExpanderAddr
 *      7 bits I2C address of 16 bits IO expander device (beta and later)
 *
 *   @param [in] ucEepromAddr
 *      7 bits I2C address of eeprom device
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/ 
Result Mo1000DevCom_Open(Mo1000DevCom_stComCore * pCore, Carrier_handle_t hCarrier, unsigned int zone, unsigned char ucBridgeAddr,
                 unsigned char ucSensorAddr, unsigned char ucExpanderAddr, unsigned char ucEepromAddr);


 /****************************************************************************
 *   NAME : Mo1000DevCom_Reset
 ************************************************************************//**
 *
 *   Initialize the Mo1000DevCom to a default non working condition.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/ 
Result Mo1000DevCom_Reset(Mo1000DevCom_stComCore * pCore);


 /****************************************************************************
 *   NAME : Mo1000DevCom_Init
 ************************************************************************//**
 *
 *   Initialize the Mo1000DevCom to a default working condition.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/ 
Result Mo1000DevCom_Init(Mo1000DevCom_stComCore * pCore);


 /****************************************************************************
 *   NAME : Mo1000DevCom_Close
 ************************************************************************//**
 *
 *   Close the Mo1000DevCom module when not needed anymore.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/ 
Result Mo1000DevCom_Close(Mo1000DevCom_stComCore * pCore);


 /****************************************************************************
 *   NAME : Mo1000DevCom_WriteArray
 ************************************************************************//**
 *
 *   Write an array of data to a specified device. Data is MSB 1st with
 *   incrementing address.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] hCom
 *      pointer to the structure returns from Mo1000DevCom_Open function
 *
 *   @param [in] u32DeviceId
 *      which logical device to write
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
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_WriteArray(void * hCom, uint32_t u32DeviceId, uint16_t u16Addr, const void * pData, int32_t i32Size);


 /****************************************************************************
 *   NAME : Mo1000DevCom_ReadArray
 ************************************************************************//**
 *
 *   Read an array of data from a specified device. Data is MSB 1st with
 *   incrementing address.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] hCom
 *      pointer to the structure returns from Mo1000DevCom_Open function
 *
 *   @param [in] u32DeviceId
 *      which logical device to read
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
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_ReadArray(void * hCom, uint32_t u32DeviceId, uint16_t u16Addr, void * pData, int32_t i32Size);


 /****************************************************************************
 *   NAME : Mo1000DevCom_Write
 ************************************************************************//**
 *
 *   Write 1 data item to a specified device (size of data item vary according
 *   to the device).
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @param [in] eDevice
 *      which logical device to write
 *
 *   @param [in] usAddr
 *      at which internal device address starting to write
 *
 *   @param [in] pData
 *      data buffer used for writing
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/ 
Result Mo1000DevCom_Write(Mo1000DevCom_stComCore * pCore, Mo1000DevCom_eDevices eDevice, unsigned short usAddr, const void * pData);


 /****************************************************************************
 *   NAME : Mo1000DevCom_Read
 ************************************************************************//**
 *
 *   Read 1 data item from a specified device. Data item size vary according
 *   to the device. 
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @param [in] eDevice
 *      which logical device to read
 *
 *   @param [in] usAddr
 *      at which internal device address starting to read
 *
 *   @param [out] pData
 *      data buffer used for reading
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/ 
Result Mo1000DevCom_Read(Mo1000DevCom_stComCore * pCore, Mo1000DevCom_eDevices eDevice, unsigned short usAddr, void * pData);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

