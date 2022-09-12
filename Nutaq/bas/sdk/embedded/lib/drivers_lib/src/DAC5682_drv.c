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
 * @file       DAC5682_drv.c
 * @brief      DAC5682 driver configuration API
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
 #include "DAC5682_drv.h"
#include "DAC5682_def.h"
#include "bas_spi.h"
#include "bas_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define INSTR_READ          0x80
#define INSTR_N1            0x40
#define INSTR_N0            0x20
#define INSTR_A4            0x10
#define INSTR_A3            0x08
#define INSTR_A2            0x04
#define INSTR_A1            0x02
#define INSTR_A0            0x01

/*
    SPI Configuration of device
    ---------------------------
    Data clocked In: Rising 
    Data clocked Out: Falling
    Idle clock: Low
    CS enable: Low

    Data Stream 
    ------------
              7  6  5  4  3  2  1  0
    Byte[0]: R/W N1 N0 A4 A3 A2 A1 A0
        R/W:  Read or Write operation
        [N1:N0]: Nb byte MSB 00:1 01:2 10:3 11:4
        [A4:A0]: Regsiter base address
    Byte[1-4]: Data

    Multibyte transfer begin at start address downward.

*/

static void InvertDataBuffer(char *DataIn, char *DataOut, int CurrentDataSize)
{
    int Idx;

    for (Idx = 0; Idx < CurrentDataSize; Idx ++)
        DataOut[CurrentDataSize - Idx - 1] = DataIn[Idx];
}

adp_result_t DAC5682_XferRegisters(void * SPI_Transport, int SPIAdd,
                                     int BaseReg, int NbReg, char * Data, int ReadOrWrite)
{
    int XferFrameSize;
    int CurrentDataSize, RemainingDataSize;
    int CurrentBaseReg;
    char DataBuffer[5];        // device can xfer up to 5 bytes
    char DataBufferIn[5];      // device can xfer up to 5 bytes
    SPITransport * SPIDriver;
	SPImode Mode;

    // HM must state in documentation that the baseadd is used upward 
    // not downward as the it is in the chip documentation.
    // it make more senses to make all devices behave this way.

    // Check that all funciton pointers are ok.
    // Check that SPIDriver->DataFrameLength is >= 2

    SPIDriver = (SPITransport *) SPI_Transport;

    // Calculate the number Data per transfer that will be needed.
    // Chip can xfer up to 4 data bytes at a time and to xfer 4 bytes we 
    // need a frame of 5 bytes.
    if (NbReg > 4)
    {
        if (SPIDriver->DataFrameLength >= 5)
            XferFrameSize = 5;
        else
            XferFrameSize = SPIDriver->DataFrameLength;
    }
    else
    {
        XferFrameSize = NbReg + 1;
    }

    // Configure SPI.
    Mode.ChipSelect = 0; //unused
    Mode.ChipSelectPolarity = SPI_LEVEL_LOW;
    Mode.ClockPhase = SPI_PHASE_POSITIVE;
    Mode.ClockPolarity = SPI_POLARITY_LOW;
	Mode.Direction = SPI_DATA_DIR_OUT;
    Mode.BitsPerWord = 8;

    SPIDriver->Services->ConfigureMode(SPI_Transport, Mode);

    CurrentBaseReg = BaseReg;
    RemainingDataSize = NbReg;
    do    
    {                
        if (RemainingDataSize >= (XferFrameSize-1))
            CurrentDataSize = (XferFrameSize-1);
        else
            CurrentDataSize = RemainingDataSize;

        memset(DataBuffer, 0, sizeof(DataBuffer));

        // Create instruction register.
        if (ReadOrWrite)
            DataBuffer[0] = 0;
        else
            DataBuffer[0] = INSTR_READ;

        DataBuffer[0] |= ((CurrentDataSize-1)<<5)|(CurrentBaseReg+(CurrentDataSize-1));

        if (ReadOrWrite)
            InvertDataBuffer(&Data[NbReg-RemainingDataSize], &DataBuffer[1], CurrentDataSize);

        SPIDriver->Services->SetChipSelect(SPI_Transport, SPIAdd, SPI_LEVEL_LOW);

        if (ReadOrWrite)
        {
            SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, NULL, CurrentDataSize+1);
        }
        else
        {
            Mode.Direction = SPI_DATA_DIR_OUT;
            SPIDriver->Services->ConfigureMode(SPI_Transport, Mode);
            SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, NULL, 1);
            Mode.Direction = SPI_DATA_DIR_IN;
            SPIDriver->Services->ConfigureMode(SPI_Transport, Mode);
            SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, DataBufferIn, CurrentDataSize);
        }

        SPIDriver->Services->SetChipSelect(SPI_Transport, SPIAdd, SPI_LEVEL_HIGH);    

        if (!ReadOrWrite)
            InvertDataBuffer(&DataBufferIn[0], &Data[NbReg-RemainingDataSize], CurrentDataSize);

        CurrentBaseReg += CurrentDataSize;
        RemainingDataSize -= CurrentDataSize;

    }while(RemainingDataSize > 0);
       
    return 0;
}

adp_result_t DAC5682_WriteRegisters (void * SPI_Transport, int SPIAdd,
                                     int BaseReg, int NbReg, char * DataIn)
{
    return (DAC5682_XferRegisters(SPI_Transport, SPIAdd, 
                                  BaseReg, NbReg, DataIn, 1));                              
}

adp_result_t DAC5682_ReadRegisters (void * SPI_Transport, int SPIAdd,
                                     int BaseReg, int NbReg, char * DataOut)
{
    return (DAC5682_XferRegisters(SPI_Transport, SPIAdd, 
                                  BaseReg, NbReg, DataOut, 0));
}

adp_result_t DAC5682_UpdateConfiguration (void * SPI_Transport, int SPIAdd,
                                          void * Configuration)
{
    //HM add at a later time if we still have budget ...
    return 0;
}

adp_result_t DAC5682_ReadConfiguration (void * SPI_Transport, int SPIAdd,
                                          void * Configuration)
{
    DAC5682_Register_Map_t * DevConfiguration;
    DevConfiguration = (DAC5682_Register_Map_t *) Configuration;

    DAC5682_ReadRegisters(SPI_Transport, SPIAdd,
                          DAC5682_REGISTER_STATUS0, 16, &DevConfiguration->RegisterArray[0]);

    return 0;
}


adp_result_t DAC5682_WriteConfiguration (void * SPI_Transport, int SPIAdd,
                                          void * Configuration)
{
    DAC5682_Register_Map_t * DevConfiguration;
    DevConfiguration = (DAC5682_Register_Map_t *) Configuration;

    DAC5682_WriteRegisters(SPI_Transport, SPIAdd,
                          DAC5682_REGISTER_STATUS0, 16, &DevConfiguration->RegisterArray[0]);

    return 0;
 }

FMCBoardDeviceServices Device_DAC5682 = 
{
    NULL,
    DAC5682_ReadRegisters,
    DAC5682_WriteRegisters,
    DAC5682_UpdateConfiguration,
    DAC5682_ReadConfiguration,
    DAC5682_WriteConfiguration 
};
