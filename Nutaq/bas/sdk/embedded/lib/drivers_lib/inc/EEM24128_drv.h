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
 * @file      EEM24128_drv.h
 *
 * @brief     EEM24128 drivers
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

#ifndef _EEM24128_DRV_H_
#define _EEM24128_DRV_H_

// Total size of 16384 Bytes.
#define EEM24128_SIZE   16384

#include "carrier_service.h"

/****************************************************************************
 * Function: EEM24128_writeMemory
 ****************************************************************************
 *
 * Sends the number of bytes to the I2C device.
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone the I2C device
 *    is located.
 *
 * @param[in] eeadr is the 7 bits i2c eeprom address.
 *
 * @param[in] addr is the start address where to send byte(s).
 *                 MSB is sent first in case of a 2 bytes address.
 *
 * @param[in] data is the data byte(s) to send.
 *
 * @param[in] nbByte indicates the number of bytes to send.
 *
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
int EEM24128_writeMemory(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char eeadr, unsigned short addr, unsigned char * data, unsigned nbByte);

/****************************************************************************
 * Function: EEM24128_readMemory
 ****************************************************************************
 *
 * Reads the number of bytes from the I2C device.
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone the I2C device
 *    is located.
 *
 * @param[in] eeadr is the 7 bits i2c eeprom address.
 *
 * @param[in] addr is the start address where to read byte(s).
 *                 MSB is sent first in case of a 2 bytes address.
 *
 * @param[out] data contains the data byte(s) readed.
 *
 * @param[in] nbByte indicates the number of bytes to read.
 *
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
int EEM24128_readMemory(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char eeadr, unsigned short addr, unsigned char * data, unsigned nbByte);

#endif
