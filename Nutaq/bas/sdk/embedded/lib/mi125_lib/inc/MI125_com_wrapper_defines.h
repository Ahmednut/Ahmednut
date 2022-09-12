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
 * @file       MI125_com_wrapper_defines.h
 * @brief      I2C communication wrapper data and structure definitions for all devices control
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

#ifndef __MI125_COM_WRAPPER_DEFINES__
#define __MI125_COM_WRAPPER_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "carrier_service_def.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Use to generate correct SPI CS for each ADC
#define MI125_SPI_CS_ADC0 (1<<0)
#define MI125_SPI_CS_ADC1 (1<<1)
#define MI125_SPI_CS_ADC2 (1<<2)
#define MI125_SPI_CS_ADC3 (1<<3)


typedef struct MI125_S_com_i2c_release {
// i2c bus ownership register definition
#ifdef _BIG_ENDIAN 
    unsigned reserved:30, MmcI2cReleaseAck:1, MmcI2cReleaseReq:1;
#else
    unsigned MmcI2cReleaseReq:1, MmcI2cReleaseAck:1, reserved:30;
#endif                
} MI125_i2c_release;


// main structure for module operation
typedef struct MI125_S_com_core {
    Carrier_handle_t hCarrier;      ///< Carrier handle to access the carrier specific services
    unsigned int zone;        ///< I2c zone to access the mi125 devices
    unsigned char spi_clock_rate;
    unsigned char bridge_addr;
    unsigned char sensor_addr;
    unsigned char expander_addr;
    unsigned char expander2_addr;
    unsigned char eeprom_addr;
} MI125_com_core;


// List of I2C logical devices
typedef enum MI125_E_COM_DEVICES {
    MI125_COM_ADC0 = 0,
    MI125_COM_ADC1 = 1,
    MI125_COM_ADC2 = 2,
    MI125_COM_ADC3 = 3,
    MI125_COM_PORTS,
    MI125_COM_I2C_BRIDGE,
    MI125_COM_SENSORS,
    MI125_COM_EEPROM,
    MI125_COM_PORTS2
} MI125_COM_DEVICES;

// List of errors
enum MI125_com_wrapper_error {
    MI125_COMWRAPPER_OK     = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_COMWRAPPER, 0),
    MI125_COMWRAPPER_UNIMPLEMENTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 1),  /**> Com direction with this device is UNIMPLEMENTED */ 
    MI125_COMWRAPPER_FAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 2),         /**> Error during I2C transfer */
    MI125_COMWRAPPER_TIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 3),        /**> I2C bus busy timeout */
    MI125_COMWRAPPER_RQTIMEOUT  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 4),     /**> I2C bus ownership request timeout */
    MI125_COMWRAPPER_INVALIDDATABUF  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 5),/**> Invalid data buffer given */
    MI125_COMWRAPPER_BTIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 6),       /**> I2C bus busy before operation timeout */
    MI125_COMWRAPPER_RLTIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 7),      /**> I2C bus ownership release timeout */ 
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

