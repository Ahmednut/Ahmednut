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
 * @file       LTC2641_drv.c
 * @brief      LTC2641 driver configuration API
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup DRIVERS
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
#include "LTC2641_drv.h"
#include "LTC2641_def.h"
#include "bas_spi.h"
#include "bas_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
    SPI Configuration of device
    ---------------------------
    Data clocked In: Raising
    Idle clock: Low
    CS enable: Low

    Data Stream 
    ------------
    Byte[0]: B15 B14 B13 B12 B11 B10 B9 B8 
    Byte[1]: B7  B6  B5  B4  B3  B2  B1 B0 


*/

adp_result_t LTC2641_XferRegisters(void * SPITransport, int SPIAdd,
                                   char * Data, int ReadOrWrite)
{
    char DataBuffer[2];        // device can xfer up to 2 bytes
    struct S_SPITransport * SPIDriver;
    SPImode Mode;

    
    // Check that all function pointers are ok.
    // Check that SPIDriver->DataFrameLength is >= 2

    SPIDriver = (struct S_SPITransport *) SPITransport;

    // Configure SPI.
    Mode.ChipSelect = 0; //unused
    Mode.ChipSelectPolarity = SPI_LEVEL_LOW;
    Mode.ClockPhase = SPI_PHASE_POSITIVE;
    Mode.ClockPolarity = SPI_POLARITY_LOW;
    Mode.Direction = SPI_DATA_DIR_OUT;
    Mode.BitsPerWord = 8;

    SPIDriver->Services->ConfigureMode(SPITransport, Mode);

    // Setup data;
    DataBuffer[0] = Data[0];
    DataBuffer[1] = Data[1];

    SPIDriver->Services->SetChipSelect(SPITransport, SPIAdd, SPI_LEVEL_LOW);

    SPIDriver->Services->TransferData(SPITransport, DataBuffer, NULL, 2);

    SPIDriver->Services->SetChipSelect(SPITransport, SPIAdd, SPI_LEVEL_HIGH);

    return 0;
}

adp_result_t LTC2641_WriteRegisters (void * SPITransport, int SPIAdd,
                                     int BaseReg, int NbReg, char * DataIn)
{
    return (LTC2641_XferRegisters(SPITransport, SPIAdd, 
                                   DataIn, 1));
}


struct S_FMCBoardDeviceServices Device_LTC2641 = 
{
    NULL,
    NULL,
    LTC2641_WriteRegisters,
    NULL,
    NULL,
    NULL
};
