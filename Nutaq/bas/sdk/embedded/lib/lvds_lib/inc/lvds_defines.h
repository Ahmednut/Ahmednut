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
 * @ingroup LVDS
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

#ifndef LVDS_DEFINES__
#define LVDS_DEFINES__

#include "bas_error.h"
#include "lvds_registers.h"

#define LVDS_GPIO_CORE_ID (0x1000)
#define LVDS_SYNC_CORE_ID (0x1010)

typedef enum E_LVDS_MODE {
    LVDS_NONE,        /**< No LVDS core is present */
    LVDS_GPIO,        /**< LVDS GPIO core is present */
    LVDS_SYNC         /**< LVDS Synchronous core is present */
} LVDS_MODE;        /**< LVDS configurations */

typedef struct S_lvds 
{
    volatile lvds_core * core;          /**< FPGA core */
    LVDS_MODE mode;                     /**< LVDS configuration */
    int (*pfUsleep)(unsigned int usec); /**< Pointer to the usleep function */
} lvds;

/****************************************************************************
*
* LVDS error code
*
****************************************************************************/
enum LVDS_error {
    LVDSOK      = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_LVDS, 0),    /**< No error */
    LVDSIVAL    = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_LVDS, 1),    /**< Invalid value. */
    LVDSABSENT  = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_LVDS, 2),    /**< LVDS FPGA core is not present */
    LVDSIMODE   = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_LVDS, 3),    /**< Invalid function for the current LVDS mode */
};

#endif
