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
 * @file       AD9511_drv.c
 * @brief      AD9511 driver configuration API
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
#include "AD9511_drv.h"
#include "AD9511_def.h"
#include "bas_spi.h"
#include "bas_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MALLOC(A)  malloc(A)
#define FREE       free
#ifdef DEBUG
    #define func_enter()      printf("AD9511: enter %s\n", __FUNCTION__)
    #define func_exit()       printf("AD9511: exit  %s\n", __FUNCTION__)
    #define debug(f, ...) printf(f, __VA_ARGS__)
#else
    #define func_enter()
    #define func_exit()
    #define debug(f, ...)
#endif

/*
    SPI Configuration of device
    ---------------------------
    Data clocked In: Rising 
    Data clocked Out: Falling
    Idle clock: Low
    CS enable: Low

    Data Stream 
    ------------
              15  14  13  12  11  10  9   8 
    Byte[0]: R/W  W1  W0  A12 A11 A10 A9  A8
        R/W:  Read or Write operation
        [W1:W0]: Nb byte 00:1 01:2 10:3 11: streaming
        [A12:A9]: Register base address
               7  6  5  4  3  2  1  0
    Byte[1]:  A7 A6 A5 A4 A3 A2 A1 A0

    Byte[N] Register data

    Unused addresses are not skipped during multibyte I/O operations; 
    therefore, it is important to avoid multibyte I/O operations that 
    would include these addresse

    Multibyte transfer begin at start address upward.
*/

static void InvertDataBuffer(char *DataIn, char *DataOut, int CurrentDataSize)
{
    int Idx;

    for (Idx = 0; Idx < CurrentDataSize; Idx ++)
        DataOut[CurrentDataSize - Idx - 1] = DataIn[Idx];
}

adp_result_t AD9511_XferRegisters(void * SPI_Transport, int SPIAdd,
                                     int BaseReg, int NbReg, char * Data, int WriteOp)
{
    int XferFrameSize;
    int CurrentDataSize, RemainingDataSize;
    int CurrentBaseReg;
    char DataBuffer[34];        // device can xfer up to 32 bytes + 2 command bytes
    char DataBufferIn[34];      // device can xfer up to 32 bytes + 2 command bytes
    SPITransport * SPIDriver;
    SPImode Mode;

    // HM must state in documentation that the baseadd is used upward 
    // not downward as the it is in the chip documentation.
    // it make more senses to make all devices behave this way.

    // Check that all funciton pointers are ok.
    // Check that SPIDriver->DataFrameLength is >= 3
    //func_enter();
    SPIDriver = (SPITransport *) SPI_Transport;

    // Calculate the number Data per transfer that will be needed.
    // Chip can xfer up to 32 data bytes at a time and it needs 34byte 
    // to do it.
    if (SPIDriver->DataFrameLength >= (NbReg+2))
        XferFrameSize = (NbReg+2);
    else
        XferFrameSize = SPIDriver->DataFrameLength;

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
        if (RemainingDataSize >= (XferFrameSize-2))
            CurrentDataSize = (XferFrameSize-2);
        else
            CurrentDataSize = RemainingDataSize;

        memset(DataBuffer, 0, sizeof(DataBuffer));

        // Create instruction register.
		//hm check the next line ...
        //DataBuffer[0] = 3<<5;       //[W1:W0] = 0x3 streaming
	     DataBuffer[0] = 0<<5;       //[W1:W0] = 0x3 streaming
        if (!WriteOp)
            DataBuffer[0] |= 0x80;                

        DataBuffer[1] = (CurrentBaseReg+(CurrentDataSize-1));

        if (WriteOp)
            InvertDataBuffer(&Data[NbReg-RemainingDataSize], &DataBuffer[2], CurrentDataSize);

        SPIDriver->Services->SetChipSelect(SPI_Transport, SPIAdd, SPI_LEVEL_LOW);

        if (WriteOp)
            SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, NULL, CurrentDataSize+2);
        else
		  {
            Mode.Direction = SPI_DATA_DIR_OUT;
            SPIDriver->Services->ConfigureMode(SPI_Transport, Mode);
            SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, NULL, 2);
            Mode.Direction = SPI_DATA_DIR_IN;
            SPIDriver->Services->ConfigureMode(SPI_Transport, Mode);
            SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, DataBufferIn, CurrentDataSize);
		}
		
        SPIDriver->Services->SetChipSelect(SPI_Transport, SPIAdd, SPI_LEVEL_HIGH);

        if (!WriteOp)
            InvertDataBuffer(&DataBufferIn[0], &Data[NbReg-RemainingDataSize], CurrentDataSize);

        CurrentBaseReg += CurrentDataSize;
        RemainingDataSize -= CurrentDataSize;

    }while(RemainingDataSize > 0);

   // func_exit();       

    return 0;
}

adp_result_t AD9511_WriteRegisters (void * SPI_Transport, int SPIAdd,
                                     int BaseReg, int NbReg, char * DataIn)
{
   // func_enter();
    return (AD9511_XferRegisters(SPI_Transport, SPIAdd, 
                                  BaseReg, NbReg, DataIn, 1));
  //  func_exit();
}

adp_result_t AD9511_ReadRegisters (void * SPI_Transport, int SPIAdd,
                                   int BaseReg, int NbReg, char * DataOut)
{
  //  func_enter();
    return (AD9511_XferRegisters(SPI_Transport, SPIAdd, 
                                  BaseReg, NbReg, DataOut, 0));
  //  func_exit();
}

adp_result_t AD9511_UpdateConfiguration (void * SPI_Transport, int SPIAdd,
                                          void * Configuration)
{
    //HM add at a later time if we still have budget ...
    return 0;
}

adp_result_t AD9511_ReadConfiguration (void * SPI_Transport, int SPIAdd,
                                          void * Configuration)
{
    AD9511_Register_Map_t * DevConfiguration;
    DevConfiguration = (AD9511_Register_Map_t *) Configuration;

    AD9511_ReadRegisters(SPI_Transport, SPIAdd,
                          0, AD9511_NB_REGISTER, &DevConfiguration->Register_00.Byte);

    return 0;
}


adp_result_t AD9511_WriteConfiguration (void * SPI_Transport, int SPIAdd,
                                          void * Configuration)
{
    AD9511_Register_Map_t * DevConfiguration;
    DevConfiguration = (AD9511_Register_Map_t *) Configuration;

    AD9511_WriteRegisters(SPI_Transport, SPIAdd,
                          0, AD9511_NB_REGISTER, &DevConfiguration->Register_00.Byte);

    return 0;
 }

FMCBoardDeviceServices Device_AD9511 = 
{
    NULL,
    AD9511_ReadRegisters,
    AD9511_WriteRegisters,
    AD9511_UpdateConfiguration,
    AD9511_ReadConfiguration,
    AD9511_WriteConfiguration 
};
