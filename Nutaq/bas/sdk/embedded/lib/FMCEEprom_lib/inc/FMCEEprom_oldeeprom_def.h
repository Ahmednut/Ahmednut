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
 * @file      FMCEEprom_oldeeprom_def.h
 * 
 * @brief     Old FMC EEPROM defines
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCEEPROMLIB_EEPROM
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

#ifndef __FMCEEPROM_OLDEEPROM_DEF_H__
#define __FMCEEPROM_OLDEEPROM_DEF_H__

#define VERSION_SIZE        1
#define VENDOR_NAME_SIZE    31
#define PART_NUMBER_SIZE    32
#define HARDWARE_REV_SIZE   32
#define FIRMWARE_REV_SIZE   32
#define GENERAL_DESCR_SIZE  64

/** @name Old FMC EEPROM information structure.
 * 
 *  Old FMC identification structure that hold the EEPROM information.
 *  Used for backward compatibility.
 *
 */
typedef struct fmcInfoStruct {
    char version[VERSION_SIZE];                    //!< Version (address 0) 
    char vendorName[VENDOR_NAME_SIZE];             //!< Vendor name (address 1)
    char partNumber[PART_NUMBER_SIZE];             //!< Part number (address 32)
    char hardwareRevision[HARDWARE_REV_SIZE];      //!< Hardware revision (address 64)
    char firmwareRevision[FIRMWARE_REV_SIZE];      //!< Firmware revision (address 96)
    char generalDescription[GENERAL_DESCR_SIZE];   //!< General description (address 128)
} fmcInfoStruct_t;

#endif

