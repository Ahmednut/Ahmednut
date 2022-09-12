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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       fmc_spi.c   
 * @brief      Abstraction layer for the SPI interface
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RADIO420
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
#include "fmc_spi.h"

Result spi_core_open(spi_core_t * spi, void * ctrl_regs, void * data_regs, unsigned start_mask, unsigned busy_mask)
{
    spi->ctrl_regs = (volatile unsigned int *) ctrl_regs;
    spi->data_regs = (volatile unsigned int *) data_regs;
    spi->start_mask = start_mask;
    spi->busy_mask = busy_mask;
    spi->timeout = 1000;
    spi->mode = SPI_BLOCKING;
    return OK;
}

Result spi_core_close(spi_core_t * spi)
{
    return OK;
}

Result spi_core_write(spi_core_t * spi, unsigned data)
{
    Result ret = OK;
    ret = spi_core_complete(spi);
    if(FAILURE(ret)) return ret;
    *spi->data_regs = data;
    *spi->ctrl_regs |= spi->start_mask;
    *spi->ctrl_regs &= ~spi->start_mask;
    return ret;
}

Result spi_core_read(spi_core_t * spi, unsigned * data)
{
    Result ret;
    ret = spi_core_complete(spi);
    if(FAILURE(ret)) return ret;
    *spi->data_regs = *data;
    *spi->ctrl_regs |= spi->start_mask;
    *spi->ctrl_regs &= ~spi->start_mask;
    ret = spi_core_complete(spi);
    if(FAILURE(ret)) return ret;
    *spi->data_regs = 1;
    *spi->ctrl_regs |= spi->start_mask;
    *spi->ctrl_regs &= ~spi->start_mask;
    ret = spi_core_complete(spi);
    *data = *spi->data_regs;
    return ret;
}

Result spi_core_complete(spi_core_t * spi)
{
    unsigned i;
    if(spi->mode == SPI_NONBLOCKING)
    {
        return OK;
    }

    for(i = spi->timeout; i != 0; i--)
    {
        if(!(*spi->ctrl_regs & spi->busy_mask))
        {
            return OK;
        }
    }
    return FMCSPITIMEOUT;
}


