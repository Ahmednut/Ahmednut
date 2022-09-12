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
 * @file      com_wrapper_defines.h
 *
 * @brief     com_wrapper defines
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

#ifndef __COM_WRAPPER_DEFINES__
#define __COM_WRAPPER_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "bas_error.h"
#include "carrier_service_def.h"

#define SPI_CS_PLL (1<<0)
#define SPI_CS_ADC (1<<1)
#define SPI_CS_CPLD (1<<3)

typedef struct S_com_i2c_release {
#ifdef _BIG_ENDIAN
    unsigned reserved:30, MmcI2cReleaseAck:1, MmcI2cReleaseReq:1;
#else
    unsigned MmcI2cReleaseReq:1, MmcI2cReleaseAck:1, reserved:30;
#endif
} i2c_release;

typedef struct S_com_core {
    Carrier_handle_t hCarrier;      ///< Carrier handle to access the carrier specific services
    unsigned int zone;        ///< I2c zone to access the mi125 devices
    unsigned char spi_clock_rate;
} com_core;

typedef enum E_COM_DEVICES {
    COM_EEPROM,
    COM_I2C_BRIDGE,
    COM_SENSORS,
    COM_PLL,
    COM_ADC,
    COM_CPLD,
} COM_DEVICES;

enum com_wrapper_error {
    COMWRAPPEROK     = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_COMWRAPPER, 0),
    COMWRAPPERTIMEOUT  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 1),
    COMWRAPPERRELEASE  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_COMWRAPPER, 2),
};

#ifdef __cplusplus
}
#endif  // extern "C"

#endif

