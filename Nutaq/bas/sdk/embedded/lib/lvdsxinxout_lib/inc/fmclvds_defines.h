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
 * @file    fmclvds_defines.h 
 * @brief   This file contains macro, definition and type for FMC LVDS
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCLVDS
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

#ifndef FMCLVDS_DEFINES__
#define FMCLVDS_DEFINES__

#include "bas_error.h"
#include "fmclvds_com_wrapper_defines.h"
#include "fmclvds_registers.h"

#define FMCLVDS_CORE_ID (0x32CA)
#define NB_FMCLVDS (2)

typedef enum E_FMCLVDS_REV {
  FMCLVDSREV_A = 0,     /**< Revision A */
} FMCLVDS_REV;          /**< FMC LVDS revision */

typedef enum E_FMCLVDS_GROUPS {
  FMCLVDS_GROUP_0,      /**< Group 0 (Channels 0-15) */
  FMCLVDS_GROUP_1       /**< Group 1 (Channels 16-31) */
} FMCLVDS_GROUPS;       /**< FMC LVDS groups */

typedef enum E_FMCLVDS_DIR {
  FMCLVDS_OUTPUT,       /**< Group configured as output */
  FMCLVDS_INPUT         /**< Group configured as input */
} FMCLVDS_DIR;          /**< FMC LVDS directions */

typedef enum E_FMCLVDS_CONFIG {
  FMCLVDS_NONE,         /**< NO FMC LVDS is present */
  FMCLVDS_32,           /**< One FMC LVDS is present (32 channels)  */
  FMCLVDS_64            /**< Two FMC LVDS are present (64 channels)*/
} FMCLVDS_CONFIG;       /**< FMC LVDS configurations */

typedef enum E_FMCLVDS_CORE_DETECT {
  FMCLVDS_CORENONE,     /**< NO FMC LVDS core is present */
  FMCLVDS_COREDETECTED  /**< FMC LVDS core is detected  */
} FMCLVDS_CORE_DETECT;  /**< FMC LVDS core detection */

typedef struct S_fmclvds 
{
  volatile fmclvds_core * core;  /**< FPGA core */
  Carrier_handle_t hCarrier;      ///< Carrier handle to access the carrier specific services
  unsigned int pos;              /**< board position */
  FMCLVDS_com_core com;          /**< I2c driver information */
} fmclvds;

/**
*****************************************************************************
*
* FMC LVDS error code
*
****************************************************************************/
enum FMCLVDS_error {
    FMCLVDSOK    = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_FMCLVDS, 0),      /**< No error */
    FMCLVDSIVAL  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCLVDS, 1),        /**< Invalid value. */
    FMCLVDSMMCRELEASE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCLVDS, 2),   /**< MMC I2C bus release timeout */
    FMCLVDSMMCREQUEST = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCLVDS, 3),   /**< MMC I2C bus request timeout */
    FMCLVDSABSENT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCLVDS, 4),       /**< FMC LVDS FPGA core or FMC LVDS card is not present */
};

#endif
