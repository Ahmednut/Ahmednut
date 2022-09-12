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
 * @file       Radio640Spi.c   
 * @brief      Generic SPI driver for Radio640
 *
 * Copyright (C) 2015, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RADIO640
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
#include <stdlib.h>
#include <string.h>

#include "bas_types.h"
#include "bas_error.h"
#include "Radio640Spi.h"

/****************************************************************************
 *                         Private Structures                               *
 ****************************************************************************/
typedef union
{
    struct
    {
        unsigned int Data:24, Ctrl:1, Start:5, ClkPol:1, Busy:1;
    }stBit;
    unsigned int uRaw;
} Radio640Spi_SpiReg;
    
typedef struct Radio640Spi_stSpi
{
    Radio640Spi_SpiReg * Write;
    uint32_t * Read;
    int (*pfUsleep)(unsigned int usec);
    uint32_t * ClkDownsampling;
} Radio640Spi_stSpi;


/****************************************************************************
 *                       Private Function Definitions                       *
 ****************************************************************************/
Result _Radio640Spi_Request(Radio640Spi_hSpi hSpi);
Result _Radio640Spi_Release(Radio640Spi_hSpi hSpi);
Result _Radio640Spi_WaitBusy(Radio640Spi_hSpi hSpi);


/****************************************************************************
 *                           Public Functions                               *
 ****************************************************************************/
Result Radio640Spi_Open(Radio640Spi_hSpi * phSpi, 
    uint32_t ui32WriteBaseAddr, uint32_t ui32ReadBaseAddr, 
    int (*pfUsleep)(unsigned int usec), uint32_t ui32ClkDownsamplingBaseAddr)
{
    (*phSpi) = (Radio640Spi_hSpi)malloc(sizeof(Radio640Spi_stSpi));
    if((*phSpi) == NULL) return eRadio640SpiMemAlloc;
    
    memset((*phSpi), 0, sizeof(Radio640Spi_stSpi));

    (*phSpi)->Write = (Radio640Spi_SpiReg *)ui32WriteBaseAddr;
    (*phSpi)->Read = (uint32_t *)ui32ReadBaseAddr;
    (*phSpi)->ClkDownsampling = (uint32_t *)ui32ClkDownsamplingBaseAddr;
    (*phSpi)->pfUsleep = pfUsleep;
    
    return eRadio640SpiOk;
}

Result Radio640Spi_Close(Radio640Spi_hSpi * phSpi)
{
    if(*phSpi) free(*phSpi);
    *phSpi = NULL;
    
    return eRadio640SpiOk;
}

Result Radio640Spi_SetClkPol(Radio640Spi_hSpi hSpi, uint8_t u8ClkPol)
{
    if(hSpi->Write == NULL)
        return eRadio640SpiWriteUnimplemented;
    
    hSpi->Write->stBit.ClkPol = (u8ClkPol & 0x1);

    return eRadio640SpiOk;
}

Result Radio640Spi_Transfer(Radio640Spi_hSpi hSpi, uint8_t u8Id, uint32_t u32ClkDownsampling, uint32_t ui32InData, uint32_t * pui32OutData)
{
    Result res = eRadio640SpiOk;
    
    if(u8Id >= 5) return eRadio640SpiInvId;
    
    res = _Radio640Spi_Request(hSpi);
    if(FAILURE(res)) return res;
    
    // Set SPI clock frequency specified by the user
    if(hSpi->ClkDownsampling != NULL) (*hSpi->ClkDownsampling) = u32ClkDownsampling;
    
    hSpi->Write->stBit.Data = ui32InData;
    hSpi->Write->stBit.Start = ((1 << u8Id) & 0x1F);
    
    res = _Radio640Spi_WaitBusy(hSpi);
    if(FAILURE(res)) goto end;
    
    hSpi->Write->stBit.Start = 0;
    
    if(pui32OutData != NULL)
    {        
        if(hSpi->Read == NULL)
        {
            res = eRadio640SpiReadUnimplemented;
            goto end;
        }
        
        // Fetch received data
        *pui32OutData = *hSpi->Read;
    }
    
end:
    
    // Set SPI clock frequency to the default value (50 MHz)
    if(hSpi->ClkDownsampling != NULL) (*hSpi->ClkDownsampling) = 2;
    
    _Radio640Spi_Release(hSpi);
    
    return res;
}

/****************************************************************************
 *                      Private Function Declarations                       *
 ****************************************************************************/
Result _Radio640Spi_Request(Radio640Spi_hSpi hSpi)
{
    if(hSpi->Write == NULL)
        return eRadio640SpiWriteUnimplemented;
    
    hSpi->Write->stBit.Ctrl = 1;

    hSpi->pfUsleep(10);
    
    return _Radio640Spi_WaitBusy(hSpi);
}

Result _Radio640Spi_Release(Radio640Spi_hSpi hSpi)
{
    if(hSpi->Write == NULL)
        return eRadio640SpiWriteUnimplemented;
    
    hSpi->Write->stBit.Ctrl = 0;

    hSpi->pfUsleep(1);
    
    return eRadio640SpiOk;
}

Result _Radio640Spi_WaitBusy(Radio640Spi_hSpi hSpi)
{
    unsigned i;
    
    if(hSpi->Write == NULL)
        return eRadio640SpiWriteUnimplemented;
    
    hSpi->pfUsleep(1);
    
    for(i = 1000; i != 0; i--)
    {
        if(hSpi->Write->stBit.Busy == 0) return eRadio640SpiOk;
        
        hSpi->pfUsleep(1);
    }
    
    return eRadio640SpiBusyTimeOut;
}


