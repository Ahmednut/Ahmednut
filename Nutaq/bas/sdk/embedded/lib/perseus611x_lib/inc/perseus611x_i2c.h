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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      perseus611x_i2c.h
 * 
 * @brief     Perseus611X I2C drivers
 *
 * @ingroup PERSEUS611X
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


#ifndef _PERSEUS611X_I2C_H
#define _PERSEUS611X_I2C_H

#include "bas_error.h"
#include "carrier_service_def.h"

/****************************************************************************
 * Function: Perseus611X_I2cWrite
 ************************************************************************//**
 *
 * Perform a I2C Write transaction.
 *
 * @warning 
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
Result Perseus611X_I2cWrite(Carrier_handle_t hCarrier, 
    unsigned int I2cZone, unsigned char i2cDeviceAddr, 
    unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options);


/****************************************************************************
 * Function: Perseus611X_I2cRead
 ************************************************************************//**
 *
 * Perform a I2C Read transaction.
 *
 * @warning 
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
Result Perseus611X_I2cRead(Carrier_handle_t hCarrier, 
    unsigned int I2cZone, unsigned char i2cDeviceAddr, 
    unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options);


/****************************************************************************
 * Function: Perseus611X_I2cReadNoWrData
 ************************************************************************//**
 *
 * Read data from a I2C device.
 * This function does not send the register start address before reading from
 * the I2C interface. It only read the incoming data.
 * It will start reading from current internal device address and so no
 * assumption could be made on the content read from what device address.
 *
 * @warning 
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
Result Perseus611X_I2cReadNoWrData(Carrier_handle_t hCarrier, 
    unsigned int I2cZone, unsigned char i2cDeviceAddr, 
    unsigned char * data, unsigned int size);


 /****************************************************************************
 *   Function: Perseus611X_I2cCheckRaw
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
Result Perseus611X_I2cCheckRaw(Carrier_handle_t hCarrier, unsigned int I2cZone, 
    unsigned char ucDevAddr, unsigned char * pucDetected);


#endif 
