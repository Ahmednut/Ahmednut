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
 * @file       mi250_ADT7411_drv.c 
 * @brief      MI250 ADT7411 driver functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI250
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
 * @file    mi250_ADT7411_drv.c 
 * @brief   
 * (c) 2012, Nutaq. All rights reserved. 
 * 
 ****************************************************************************/

#include "mi250_ADT7411_drv.h"
#include "i2c_drv.h"

/*
    I2C ADD: A0, A1 and A2 are used.
    IMPORTANT NOTE: It is not possible to do a block write or a block read operation.

    I2C configuration
    ------------------
    Add size: 7 bits
    Speed: 400KHz
    Control: 1001xxx(R/W) R=1 W=0    

    Transfer sequence
    ---------------------
    Write address to read (one byte)
    Start Control (ACK) Add(n) (ACK) Stop 

    Current read operation (after write address)
    Start Control (ACK) Data(n) (NACK) Stop
    
    Write single byte 
    ------------------
    Start Control (ACK) Add(n) (ACK) DATA(n) (ACK) Stop

*/

// 1 Byte address to access this device.
#define NB_BYTE_ADDRESS  1

int ADT7411_writeSingleReg(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data)
{
    int result;
    I2C_Options_t options;
    
    options.nbByteAddr = NB_BYTE_ADDRESS;
    result = I2C_writeData(i2cBaseAddr, i2cDeviceAddr, addr, data, 1, options);

    return(result);                              
}

int ADT7411_readSingleReg(unsigned i2cBaseAddr, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data)
{
    int result;
    I2C_Options_t options;
    
    options.nbByteAddr = NB_BYTE_ADDRESS;
    result = I2C_readData(i2cBaseAddr, i2cDeviceAddr, addr, data, 1, options);

    return(result);
}




















