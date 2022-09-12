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
 * @file    radio420_cdce62005.h 
 * @brief   Calculate valid configuration and configure the cdce62005 PLL
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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
 
#ifndef __CDCE62005__
#define __CDCE62005__

#include "bas_error.h"
#include "fmc_spi.h"
#include "radio420_cdce62005_defines.h"

Result cdce62005_calculate(cdce62005_param * param);
Result cdce62005_setparam(cdce62005_param * param, spi_core_t * spi);
Result cdce62005_spi_read(spi_core_t * spi, unsigned addr, unsigned * data);
Result cdce62005_spi_write(spi_core_t * spi, unsigned addr, unsigned data);
Result cdce62005_lock_mode(spi_core_t * spi, pll_lock_mode mode);
Result cdce62005_lock_state(spi_core_t * spi, pll_lock_status * state);
Result cdce62005_sync(spi_core_t * spi);
Result cdce62005_flush_register(spi_core_t * spi);

enum CDCE62005_error {
    CDCE62005OK    = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_CDCE62005, 0),
    CDCE62005IVAL  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 1),                 /**< Time out while waiting for SPI core. */
    CDCE62005UNIMPLEMENTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 2),         /**< This functionality is not yet implemented */
    CDCE62005DIVIDER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 3),               /**< Requested frequencies cannot be generated because of divider limitation */
    CDCE62005_NOSOLUTION = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 4),           /**< Requested frequencies cannot be generated */
    CDCE62005_PFDIMPOSSIBLE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 5),        /**< Requested frequencies cannot be generated because of phase detector limitation */
    CDCE62005_NOPRESCALER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 6),          /**< Requested frequencies cannot be generated because of prescaler limitation */
    CDCE62005_INVALIDPRESCALER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 7),     /**< Invalid prescaler */
    CDCE62005_INVALIDFREQ = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 8),          /**< Requested frequencies are invalid. Only 2 uniques frequencies can be requested at this time */
    CDCE62005_OUTPUTDIVIMPOSSIBLE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 9),  /**< VCO freq is too high */
    CDCE62005_OVERFLOW = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_CDCE62005, 10),            /**< Overflow during calculation */
};

#endif

