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
 * @brief      communication wrapper data structure definitions for all devices control
 *
 * $Date: 2016/02/11 22:08:09 $
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

#ifndef RADIO640DEVCOM_DEFINES_H__
#define RADIO640DEVCOM_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "carrier_service_def.h"
#include "Radio640Spi.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

typedef struct Radio640DevCom_stComCore * Radio640DevCom_hComCore;

// List of devices
typedef enum Radio640DevCom_eDevices {
    eRadio640DeviceTrx = 0,
    eRadio640DeviceClk = 1,
    eRadio640DeviceDacOsc = 2,
    eRadio640DeviceRfAtt1 = 3,
    eRadio640DeviceRfAtt2 = 4,
    eRadio640DeviceTemp = 5,
    eRadio640DevicePower12V = 6,
    eRadio640DevicePowerVadj = 7,
    eRadio640DeviceEeprom = 8
} Radio640DevCom_eDevices;

static const string_enum_element_t RADIO640_DEVICES_STRING_TABLE[] = {
    {"eRadio640DeviceTrx",      eRadio640DeviceTrx},
    {"eRadio640DeviceClk",      eRadio640DeviceClk},
    {"eRadio640DeviceDacOsc",   eRadio640DeviceDacOsc},
    {"eRadio640DeviceRfAtt1",   eRadio640DeviceRfAtt1},
    {"eRadio640DeviceRfAtt2",   eRadio640DeviceRfAtt2},
    {"eRadio640DeviceTemp",     eRadio640DeviceTemp},
    {"eRadio640Device12V",      eRadio640DevicePower12V},
    {"eRadio640DeviceVadj",     eRadio640DevicePowerVadj},
    {"eRadio640DeviceEeprom",   eRadio640DeviceEeprom}
};

/** @name Radio640 I2C devices address structure
 *
 *  Hold i2c devices address when opening Radio640 instance.
 */
typedef struct Radio640DevCom_stI2cAddr {
    uint8_t u8Eeprom;       ///< EEPROM i2c device address
    uint8_t u8Temp;         ///< Temperature sensor device
    uint8_t u8Power12V;     ///< 12V power monitor device
    uint8_t u8PowerVadj;    ///< Vadj power monitor device
} Radio640DevCom_stI2cAddr;

// List of errors
enum Radio640DevCom_eError {
    eRadio640DevComOk                  = MAKE_RESULT(eBasSeveritySuccess,   MODULE_RADIO640DEVCOM, 0),  /**> Success */
    eRadio640DevComUnimplemented01     = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 1),  /**> Com direction with this device is unimplemented location 01 */ 
    eRadio640DevComUnimplemented02     = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 2),  /**> Com direction with this device is unimplemented location 02 */ 
    eRadio640DevComUnimplemented03     = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 3),  /**> Com direction with this device is unimplemented location 03 */ 
    eRadio640DevComUnimplemented04     = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 4),  /**> Com direction with this device is unimplemented location 04 */ 
    eRadio640DevComFailed01            = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 5),  /**> Error during I2C transfer location 01 */
    eRadio640DevComFailed02            = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 6),  /**> Error during I2C transfer location 02 */
    eRadio640DevComFailed03            = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 7),  /**> Error during I2C transfer location 03 */
    eRadio640DevComFailed04            = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 8),  /**> Error during I2C transfer location 04 */
    eRadio640DevComInvalidDataBuffer01 = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 9),  /**> Invalid data buffer given location 01 */
    eRadio640DevComInvalidDataBuffer02 = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 10), /**> Invalid data buffer given location 02 */
    eRadio640DevComInvalidDataBuffer03 = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640DEVCOM, 11)  /**> Invalid data buffer given location 03 */
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

