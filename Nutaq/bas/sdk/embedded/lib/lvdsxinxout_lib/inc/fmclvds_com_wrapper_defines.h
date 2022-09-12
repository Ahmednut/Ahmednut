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
 * @file       FMCLVDS_com_wrapper_defines.h
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

#ifndef __FMCLVDS_COM_WRAPPER_DEFINES__
#define __FMCLVDS_COM_WRAPPER_DEFINES__

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



typedef struct FMCLVDS_S_com_i2c_release {
// i2c bus ownership register definition
#ifdef _BIG_ENDIAN 
    unsigned reserved:30, MmcI2cReleaseAck:1, MmcI2cReleaseReq:1;
#else
    unsigned MmcI2cReleaseReq:1, MmcI2cReleaseAck:1, reserved:30;
#endif                
} FMCLVDS_i2c_release;


// main structure for module operation
typedef struct FMCLVDS_S_com_core {
    unsigned base_addr;
    Carrier_handle_t hCarrier;      ///< Carrier handle to access the carrier specific services
    unsigned int zone;        ///< I2c zone to access the FMC LVDS devices
    unsigned char expander_addr;
} FMCLVDS_com_core;


// List of I2C logical devices
typedef enum FMCLVDS_E_COM_DEVICES {
     FMCLVDS_COM_PORTS,
    FMCLVDS_COM_EEPROM
} FMCLVDS_COM_DEVICES;

typedef enum FMCLVDS_E_COM_PORTS_ADDRESSES {
    EXPANDER_GA00_BOTTOM = 0x3C,
    EXPANDER_GA01_BOTTOM = 0x3D,
    EXPANDER_GA10_BOTTOM = 0x3E,
    EXPANDER_GA11_BOTTOM = 0x3F,
    EXPANDER_GA00_TOP = 0x38,
    EXPANDER_GA01_TOP = 0x39,
    EXPANDER_GA10_TOP = 0x3A,
    EXPANDER_GA11_TOP = 0x3B,
} FMCLVDS_COM_PORTS_ADDRESSES;

// List of errors
enum FMCLVDS_com_wrapper_error {
    FMCLVDS_COMWRAPPER_OK     = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_COMWRAPPER, 0),
    FMCLVDS_COMWRAPPER_UNIMPLEMENTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 1),  /**> Com direction with this device is UNIMPLEMENTED */ 
    FMCLVDS_COMWRAPPER_FAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 2),         /**> Error during I2C transfer */
    FMCLVDS_COMWRAPPER_TIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 3),        /**> I2C bus busy timeout */
    FMCLVDS_COMWRAPPER_RQTIMEOUT  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 4),     /**> I2C bus ownership request timeout */
    FMCLVDS_COMWRAPPER_INVALIDDATABUF  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 5),/**> Invalid data buffer given */
    FMCLVDS_COMWRAPPER_BTIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 6),       /**> I2C bus busy before operation timeout */
    FMCLVDS_COMWRAPPER_RLTIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 7),      /**> I2C bus ownership release timeout */ 
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

