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
 * @file       carrier_service.h
 * @brief      Carrier generic service functions
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.7 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup CARRIERLIB
 *
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

#ifndef __CARRIER_SERVICE_H__
#define __CARRIER_SERVICE_H__

#include "bas_error.h"

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "carrier_service_def.h"


/** @defgroup CARRIERLIB Carrier generic services library
 *  
 *  Functions to deal with generic carrier services in the system.
 *               
 *  @file
 *     
 */

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


/****************************************************************************
 * Function: Carrier_Close
 ************************************************************************//**
 *
 * Close the current carrier object 
 *
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_Close(Carrier_handle_t hCarrier);

/****************************************************************************
 * Function: Carrier_I2cRead
 ************************************************************************//**
 *
 * Perform a I2C Read transaction.
 *
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param[in] i2cDeviceAddr is the 7 bits i2c eeprom address.
 *
 * @param[in] addr is the start address where to read byte(s).
 *                 MSB is sent first in case of a 2 bytes address.
 *                 addr can be seen as the register address of the internal 
 *                 memory map of the I2C chip.
 *
 * @param[out] data contains the data byte(s) readed.
 *
 * @param[in] size indicates the number of bytes to read.
 *
 * @param[in] options contains two parameters: 
 *                    nbByteAddr = number of bytes of the addr parameter (can be 1 or 2 bytes).
 *                    stopCondition = not used for the moment. Indicates whether to hold of free
 *                    the bus after sending the data.  
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_I2cRead(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options);


/****************************************************************************
 * Function: Carrier_I2cWrite
 ************************************************************************//**
 *
 * Perform a I2C Write transaction.
 *
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param[in] i2cDeviceAddr is the 7 bits i2c eeprom address.
 *
 * @param[in] addr is the start address where to send byte(s).
 *                 MSB is sent first in case of a 2 bytes address.
 *                 addr can be seen as the register address of the internal 
 *                 memory map of the I2C chip.
 *
 * @param[in] data is the data byte(s) to send.
 *
 * @param[in] size indicates the number of bytes to send.
 *
 * @param[in] options contains two parameters: 
 *                    nbByteAddr = number of bytes of the addr parameter (can be 1 or 2 bytes).
 *                    stopCondition = not used for the moment. Indicates whether to hold of free
 *                    the bus after sending the data.  
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_I2cWrite(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options);


/****************************************************************************
 * Function: Carrier_I2cReadNoWrData
 ************************************************************************//**
 *
 * Read data from a I2C device.
 * This function does not send the register start address before reading from
 * the I2C interface. It only read the incoming data.
 * It will start reading from current internal device address and so no
 * assumption could be made on the content read from what device address.
 *
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param[in] i2cDeviceAddr is the 7 bits i2c eeprom address.
 *
 * @param[out] data contains the data byte(s) readed.
 *
 * @param[in] size indicates the number of bytes to read.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_I2cReadNoWrData(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned char * data, unsigned int size);

 /****************************************************************************
 *  Function: Carrier_I2cScanDevices
 ************************************************************************//**
 *
 * Scan the i2c bus to detect all available devices.
 * This function is used for production purpose to check if all devices
 * are detected correctly on the i2c bus.
 *
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *
 * @param [out] pScanRaw
 *    Returns the raw i2c scan results structure (each detected device address in the vector
 *    hold != 0 when a device is detected)
 *
 * @param [out] pucDevices
 *    Number of i2c devices detected on the bus.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *
 ***************************************************************************/
Result Carrier_I2cScanDevices(Carrier_handle_t hCarrier, unsigned int I2cZone, I2C_stI2cScan *pScanRaw, unsigned char *pucDevices);


 /****************************************************************************
 *   Function: Carrier_I2cCheckRaw
 ************************************************************************//**
 *
 *   Check if an i2c device is detected on the i2c bus
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *
 *   @param [in] ucDevAddr
 *      i2c device address to be checked
 *
 *   @param [out] pucDetected
 *      !=0 if the specified device is detected
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_I2cCheckRaw(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char ucDevAddr, unsigned char * pucDetected);


 /****************************************************************************
 *   Function: Carrier_GetModuleBaseAddr
 ************************************************************************//**
 *
 *   Retrieve the base address corresponding to the specified module.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] module
 *    Carrier_eModule_t enum that indicates the module type
 *
 *   @param [in] index
 *      Index of the module to get the base address. Index starts at 0.
 *
 *   @param [out] addr
 *      Pointer that will hold the retrieved base address.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetModuleBaseAddr(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int index, unsigned int * addr);


 /****************************************************************************
 *   Function: Carrier_GetModuleSize
 ************************************************************************//**
 *
 *   Retrieve the memory space size of the specified module type.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] module
 *    Carrier_eModule_t enum that indicates the module type
 *
 *   @param [out] size
 *      Pointer that will hold the retrieved memory space size.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetModuleSize(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * size);


 /****************************************************************************
 *   Function: Carrier_GetModuleMaxNumber
 ************************************************************************//**
 *
 *   Retrieve the maximal number of instance of the specified module type the
 *   current carrier can handle.
 *
 *   The retrieved instance number minus 1, is the maximal index value that
 *   can be used when calling the Carrier_GetModuleBaseAddr function.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] module
 *    Carrier_eModule_t enum that indicates the module type
 *
 *   @param [out] num
 *      Pointer that will hold the retrieved instance number.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetModuleMaxNumber(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * num);


 /****************************************************************************
 *   Function: Carrier_GetMemSpaceBaseAddr
 ************************************************************************//**
 *
 *   Retrieve the base address corresponding to the specified memory space.
 *
 *   A memory space is a range of memory address that is available to the
 *   user to connect its custom logic to the processor. Once the custom logic
 *   is mapped inside a memory space, its registers are reachable using the memory
 *   read and write functions.  
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] space
 *    Carrier_eMemSpace_t enum that indicates a specific memory space
 *
 *   @param [out] addr
 *      Pointer that will hold the retrieved base address.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetMemSpaceBaseAddr(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * addr);


 /****************************************************************************
 *   Function: Carrier_GetModuleSize
 ************************************************************************//**
 *
 *   Retrieve the memory space size of the specified memory space type.
 *
 *   A memory space is a range of memory address that is available to the
 *   user to connect its custom logic to the processor. Once the custom logic
 *   is mapped inside a memory space, its registers are reachable using the memory
 *   read and write functions.  
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] space
 *    Carrier_eMemSpace_t enum that indicates a specific memory space
 *
 *   @param [out] size
 *      Pointer that will hold the retrieved memory space size.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetMemSpaceSize(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * size);


 /****************************************************************************
 *   Function: Carrier_FmcGetGA
 ************************************************************************//**
 *
 *   Retrieve the Geographical Address (GA) of the specified FMC connector for
 *   the current carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    FMC connector number. Connector instance starts at 0.
 *
 *   @param [out] ga
 *      Pointer that will hold the retrieved geographical address.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FmcGetGA(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, unsigned char * ga);


 /****************************************************************************
 *   Function: Carrier_FmcGetI2cZone
 ************************************************************************//**
 *
 *   Retrieve the I2C zone of the specified FMC connector for
 *   the current carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    FMC connector number. Connector instance starts at 0.
 *
 *   @param [out] I2cZone
 *      Pointer that will hold the retrieved I2C zone.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FmcGetI2cZone(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, unsigned int * I2cZone);


 /****************************************************************************
 *   Function: Carrier_CustomRegisterWrite
 ************************************************************************//**
 *
 *   Update the value of a custom register in the platform register FPGA core.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] index
 *    Index of the custom register. Index starts at 0.
 *
 *   @param [in] value
 *      32-bit value to write in the custom register.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_CustomRegisterWrite(Carrier_handle_t hCarrier, unsigned int index, unsigned int value);


 /****************************************************************************
 *   Function: Carrier_CustomRegisterRead
 ************************************************************************//**
 *
 *   Retrieve the value of a custom register in the platform register FPGA core.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] index
 *    Index of the custom register. Index starts at 0.
 *
 *   @param [out] value
 *      Pointer that will hold the retrieved 32-bit value of the custom register.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_CustomRegisterRead(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value);


 /****************************************************************************
 *   Function: Carrier_LedSetValue
 ************************************************************************//**
 *
 *   Set the state of the carrier user LEDs.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] index
 *    Index of the LED. Index starts at 0.
 *
 *   @param [in] value
 *      Specify the desired LED state. 0: Turned off,  1: Turned on
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_LedSetValue(Carrier_handle_t hCarrier, unsigned int index, unsigned int value);


 /****************************************************************************
 *   Function: Carrier_LedGetValue
 ************************************************************************//**
 *
 *   Get the state of the carrier user LEDs.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] index
 *    Index of the LED. Index starts at 0.
 *
 *   @param [out] value
 *      Pointer that will hold the retrieved LED state. 0: Turned off,  1: Turned on
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_LedGetValue(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value);


 /****************************************************************************
 *   Function: Carrier_Reboot
 ************************************************************************//**
 *
 *   Reboot the carrier platform.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_Reboot(Carrier_handle_t hCarrier);


 /****************************************************************************
 *   Function: Carrier_TClkSetState
 ************************************************************************//**
 *
 *   Set the state (enable or disable) of the specified TClk in the specified 
 *   direction
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [in] dir
 *    Specify the direction of the TCLK
 *
 * @param [in] state
 *    Specify the desired state (Carrier_eClkDisable or Carrier_eClkEnable)
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_TClkSetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t state);


 /****************************************************************************
 *   Function: Carrier_TClkGetState
 ************************************************************************//**
 *
 *   Get the state (enable, disable, or unknown) of the specified TClk in the specified 
 *   direction
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [in] dir
 *    Specify the direction of the TCLK
 *
 * @param [out] state
 *    Pointer that will hold the retrieved state
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_TClkGetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t * state);


 /****************************************************************************
 *   Function: Carrier_TClkSetTxSource
 ************************************************************************//**
 *
 *   Set the driver of the specified TClk in TX direction (from carrier board
 *   to MicroTCA backplane).
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [in] source
 *    Specify the desired source of the TX clock
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_TClkSetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t source);


 /****************************************************************************
 *   Function: Carrier_TClkGetTxSource
 ************************************************************************//**
 *
 *   Get the driver of the specified TClk in TX direction (from carrier board
 *   to MicroTCA backplane).
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [out] source
 *    Pointer that will hold the retrieved source of the TX clock
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_TClkGetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t * source);


 /****************************************************************************
 *   Function: Carrier_FmcBidirClkSetConnection
 ************************************************************************//**
 *
 *   Set the connection of the specified FMC bidirectional clock on the 
 *   carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] clk
 *    Specify the FMC bidirectional clock to change its connection
 *
 * @param [in] connection
 *    Specify the desired connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FmcBidirClkSetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t connection);


 /****************************************************************************
 *   Function: Carrier_FmcBidirClkGetConnection
 ************************************************************************//**
 *
 *   Get the connection of the specified FMC bidirectional clock on the 
 *   carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] clk
 *    Specify the FMC bidirectional clock to change its connection
 *
 * @param [out] connection
 *    Pointer that will hold the retrieved connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FmcBidirClkGetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t * connection);

 /****************************************************************************
 *   Function: Carrier_SetOscState
 ************************************************************************//**
 *
 *   Enable of disable the specified clock oscillator located on the carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] osc
 *    Specify the clock oscillator to change its state
 *
 * @param [in] state
 *    Specify the desired state (Carrier_eClkDisable or Carrier_eClkEnable)
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_SetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t state);


 /****************************************************************************
 *   Function: Carrier_GetOscState
 ************************************************************************//**
 *
 *   Get the state of the specified clock oscillator located on the carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] osc
 *    Specify the clock oscillator to change its state
 *
 * @param [out] state
 *    Pointer that will hold the retrieved state
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t * state);


 /****************************************************************************
 *   Function: Carrier_FClkASetConnection
 ************************************************************************//**
 *
 *   Set the connection of the FCLKA clock on the carrier board.
 *
 *   FCLKA is a backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connection
 *    Specify the desired connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FClkASetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t connection);


 /****************************************************************************
 *   Function: Carrier_FClkAGetConnection
 ************************************************************************//**
 *
 *   Get the connection of the FCLKA clock on the carrier board.
 *
 *   FCLKA is a backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [out] connection
 *    Pointer that will hold the retrieved connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FClkAGetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t * connection);


 /****************************************************************************
 *   Function: Carrier_usleep
 ************************************************************************//**
 *
 *   Sleep during N microseconds. The sleep function has been assign during
 *   specific carrier open function.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] usec
 *    Specify the number of microseconds to sleep.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_usleep(Carrier_handle_t hCarrier, unsigned int usec);


Carrier_pfUsleep Carrier_GetUsleepFunc(Carrier_handle_t hCarrier);


 /****************************************************************************
 *   Function: Carrier_GetType
 ************************************************************************//**
 *
 *   Get the currrent type of the carrier.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [out] pType
 *    Pointer that will hold the carrier type.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetType(Carrier_handle_t hCarrier, Carrier_eType_t * pType);


 /****************************************************************************
 *   Function: Carrier_GetFmcConnectorNumMax
 ************************************************************************//**
 *
 *   Get the number of FMC connectors of the current carrier.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [out] puFmcConnectorNumMax
 *    Pointer that will hold the number of FMC connectors.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetFmcConnectorNumMax(Carrier_handle_t hCarrier, unsigned int * puFmcConnectorNumMax);


 /****************************************************************************
 *   Function: Carrier_GetFmcBoardNumMax
 ************************************************************************//**
 *
 *   Get the maximal number of FMC boards that can be stacked on an FMC 
 *   connector of the current carrier.
 *
 *   For example, to be able to use two Nutaq FMC boards in a double-stack
 *   configuration, the maximal board number should at least be 2.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [out] puFmcBoardNumMax
 *    Pointer that will hold the maximal number of FMC board a connector can
 *    handle.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetFmcBoardNumMax(Carrier_handle_t hCarrier, unsigned int * puFmcBoardNumMax);
                             

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

