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
 * @file    Radio640Spi.h 
 * @brief   Generic SPI driver for Radio640
 * 
 * Copyright (C) 2015, Nutaq, Canada. All rights reserved.
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
#ifndef __RADIO640_SPI__
#define __RADIO640_SPI__

#include "bas_error.h"
#include "bas_types.h"

typedef struct Radio640Spi_stSpi * Radio640Spi_hSpi;

Result Radio640Spi_Open(Radio640Spi_hSpi * phSpi, 
    uint32_t ui32WriteBaseAddr, uint32_t ui32ReadBaseAddr, 
    int (*pfUsleep)(unsigned int usec), uint32_t ui32ClkDownsamplingBaseAddr);
Result Radio640Spi_Close(Radio640Spi_hSpi * phSpi);
Result Radio640Spi_SetClkPol(Radio640Spi_hSpi hSpi, uint8_t ui8ClkPol);
Result Radio640Spi_Transfer(Radio640Spi_hSpi hSpi, uint8_t u8Id, uint32_t u32ClkDownsampling, uint32_t ui32InData, uint32_t * pui32OutData);

// List of errors
enum Radio640Spi_eError {
    eRadio640SpiOk                  = MAKE_RESULT(eBasSeveritySuccess,   MODULE_RADIO640SPI, 0),  /**> Success */
    eRadio640SpiBusyTimeOut         = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640SPI, 1),  /**> SPI bus busy timeout */
    eRadio640SpiWriteUnimplemented  = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640SPI, 2),  /**> Write operation is not supported */
    eRadio640SpiReadUnimplemented   = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640SPI, 3),  /**> Read operation is not supported */
    eRadio640SpiInvId               = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640SPI, 4),  /**> Invalid ID */
    eRadio640SpiMemAlloc            = MAKE_RESULT(eBasSeverityError,     MODULE_RADIO640SPI, 5)   /**> Cannot allocate memory */
};

#endif

