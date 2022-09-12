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
 * @file    sysmon_def.h
 * @brief   This file contains macro, definition and type for System Monitor
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup SYSMON
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

#ifndef SYSMON_DEF__
#define SYSMON_DEF__

 /** @name Carrier object
 *
 *  Holds a pointer to access the carrier
 */
typedef void * Sysmon_handle_t;

/** @name System Monitor type
 *
 *  Hold information of all the different information that can be read from System Monitor
 */
typedef enum Sysmon_eType {
    Sysmon_eTemp,           /**< FPGA current temperature */
    Sysmon_eTempMin,        /**< FPGA minimal temperature */
    Sysmon_eTempMax,        /**< FPGA maximal temperature */
    Sysmon_eVint,           /**< Vint FPGA power supply current voltage */
    Sysmon_eVintMin,        /**< Vint FPGA power supply minimal voltage */
    Sysmon_eVintMax,        /**< Vint FPGA power supply maximal voltage */
    Sysmon_eVaux,           /**< Vaux FPGA power supply current voltage */
    Sysmon_eVauxMin,        /**< Vaux FPGA power supply minimal voltage */
    Sysmon_eVauxMax         /**< Vaux FPGA power supply maximal voltage */
} Sysmon_eType_t;


#define SYSMON_TEMP_ADDR        0x200
#define SYSMON_TEMP_MIN_ADDR    0x290
#define SYSMON_TEMP_MAX_ADDR    0x280
#define SYSMON_VINT_ADDR        0x204
#define SYSMON_VINT_MIN_ADDR    0x294
#define SYSMON_VINT_MAX_ADDR    0x284
#define SYSMON_VAUX_ADDR        0x208
#define SYSMON_VAUX_MIN_ADDR    0x298
#define SYSMON_VAUX_MAX_ADDR    0x288

#endif
