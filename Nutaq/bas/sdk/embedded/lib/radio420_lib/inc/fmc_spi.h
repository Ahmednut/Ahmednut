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
 * @file    fmc_spi.h 
 * @brief   Function definitions for spi access
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
#ifndef __FMC_SPI__
#define __FMC_SPI__

#include "bas_error.h"

typedef enum E_spi_mode {
    SPI_BLOCKING,
    SPI_NONBLOCKING
} spi_mode;

typedef struct spi_core {
    volatile unsigned * ctrl_regs;
    volatile unsigned * data_regs;
    unsigned start_mask;
    unsigned busy_mask;
    spi_mode mode;
    unsigned timeout;
} spi_core_t;


Result spi_core_open(spi_core_t * spi, void * ctrl_regs, void * data_regs, unsigned start_mask, unsigned busy_mask);
Result spi_core_close(spi_core_t * spi);
Result spi_core_write(spi_core_t * spi, unsigned data);
Result spi_core_read(spi_core_t * spi, unsigned * data);
Result spi_core_complete(spi_core_t * spi);

enum FMCSPI_error {
    FMCSPIOK       = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_FMCSPI, 0),
    FMCSPITIMEOUT  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCSPI, 1),   /** < time out while waiting for SPI core. */
};

#endif

