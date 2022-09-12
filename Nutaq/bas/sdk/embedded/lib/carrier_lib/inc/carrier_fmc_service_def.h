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
 * @file       carrier_fmc_service_def.h
 * @brief      carrier generic library structure and data definitions for FMC.
 *
 * $Date: 2015/05/15 14:13:42 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup CARRIERLIB
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

#ifndef __CARRIER_FMC_SERVICE_DEF_H__
#define __CARRIER_FMC_SERVICE_DEF_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "carrier_service_def.h"
#include "FMCEEprom_eeprom_def.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

/** @name FMC carrier slot information structure
 * 
 *  Hold information of each slot in the system.   
 */
typedef struct Carrier_sSlotInfo {
    Carrier_eState_t modstate[CARRIER_FMC_MAXSLOT][CARRIER_FMC_MAXBOARD];         ///< modules operation state
    FMCEEprom_module_types boardtype[CARRIER_FMC_MAXSLOT][CARRIER_FMC_MAXBOARD];  ///< board types for each slot/board on the carrier
    FMCEEprom_fpga_coreid fpgacoretype[CARRIER_FMC_MAXSLOT][CARRIER_FMC_MAXBOARD];///< FPGA core types for each slot/board on the carrier
    Carrier_eVadjValue_t vadjreq[CARRIER_FMC_MAXSLOT][CARRIER_FMC_MAXBOARD];      ///< Carrier Vadj requirements for each slot/board on the carrier
    unsigned char module_identindex[CARRIER_FMC_MAXSLOT][CARRIER_FMC_MAXBOARD];   ///< This is an index in the identified structure to which the board is assigned (FMCEEprom_module_types modtype)
} Carrier_sSlotInfo_t;


#ifdef __cplusplus
}
#endif  // extern "C"
#endif

