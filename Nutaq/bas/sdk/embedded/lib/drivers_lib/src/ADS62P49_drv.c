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
 * @file       ADS62P49_drv.c
 * @brief      ADS62P49 driver configuration API
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
#include "ADS62P49_drv.h"
#include "ADS62P49_def.h"
#include "bas_spi.h"
#include "bas_types.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
/*
    SPI Configuration of device
    ---------------------------
    Data clocked In: Falling 
    Data clocked Out: Raising
    Idle clock: High
    CS enable: Low

    Data Stream 
    ------------
              7  6  5  4  3  2  1  0
    Byte[0]: A7 A6 A5 A4 A3 A2 A1 A0
    Byte[1]: D7 D6 D5 D4 D3 D2 D1 D0

    No Multi byte support.

*/

static const int RegisterMap[ADS62P49_NB_REGISTER] =
{
    ADS62P49_REGISTER_00,
    ADS62P49_REGISTER_20,
    ADS62P49_REGISTER_3F,
    ADS62P49_REGISTER_40,
    ADS62P49_REGISTER_41,
    ADS62P49_REGISTER_44,
    ADS62P49_REGISTER_50,
    ADS62P49_REGISTER_51,
    ADS62P49_REGISTER_52,
    ADS62P49_REGISTER_53,
    ADS62P49_REGISTER_55,
    ADS62P49_REGISTER_57,
    ADS62P49_REGISTER_62,
    ADS62P49_REGISTER_63,
    ADS62P49_REGISTER_66,
    ADS62P49_REGISTER_68,
    ADS62P49_REGISTER_6A,
    ADS62P49_REGISTER_75,
    ADS62P49_REGISTER_76
};

int FindNextReg(int CurrentBaseReg)
{
    int Idx;
   
    for(Idx =0 ; Idx < (sizeof(RegisterMap)/sizeof(int)) - 1; Idx ++)
        if (RegisterMap[Idx] == CurrentBaseReg)
            return (RegisterMap[Idx+1]);

    return -1;
}


adp_result_t ADS62P49_XferRegisters(void * SPI_Transport, int SPIAdd,
                                     int BaseReg, int NbReg, char * Data, int ReadOrWrite)
{
    int CurrentDataSize, RemainingDataSize;
    int CurrentBaseReg;
    char DataBuffer[2];        // device can xfer up to 2 bytes
    char DataBufferIn[2];      // device can xfer up to 2 bytes
    SPITransport * SPIDriver;
    SPImode Mode;

    // HM must state in documentation that the baseadd is used upward 
    // not downward as the it is in the chip documentation.
    // it make more senses to make all devices behave this way.

    // Check that all function pointers are ok.
    // Check that SPIDriver->DataFrameLength is >= 2

    SPIDriver = (SPITransport *) SPI_Transport;

    // Configure SPI.
    Mode.ChipSelect = 0; //unused
    Mode.ChipSelectPolarity = SPI_LEVEL_LOW;
    Mode.ClockPhase = SPI_PHASE_POSITIVE;
    Mode.ClockPolarity = SPI_POLARITY_HIGH;
    Mode.Direction = SPI_DATA_DIR_OUT;
    Mode.BitsPerWord = 8;

    SPIDriver->Services->ConfigureMode(SPI_Transport, Mode);

    // Setup the transfer direction;
    DataBuffer[0] = ADS62P49_REGISTER_00;    
    if (ReadOrWrite)
        DataBuffer[1] = 0x0;
    else
        DataBuffer[1] = 0x1;

    // Write to register 0 to set Serial readout bit. 
    // Enable SDOUT pin if wants to read. Disable if write.
    SPIDriver->Services->SetChipSelect(SPI_Transport, SPIAdd, SPI_LEVEL_LOW);
    SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, NULL, 2);
    SPIDriver->Services->SetChipSelect(SPI_Transport, SPIAdd, SPI_LEVEL_HIGH);

    CurrentBaseReg = BaseReg;
    RemainingDataSize = NbReg;
    do    
    {                
        CurrentDataSize = 1;

        memset(DataBuffer, 0, sizeof(DataBuffer));

        DataBuffer[0] = CurrentBaseReg;
    
        if (ReadOrWrite)
            DataBuffer[1] = Data[(NbReg-RemainingDataSize)];

        SPIDriver->Services->SetChipSelect(SPI_Transport, SPIAdd, SPI_LEVEL_LOW);

        if (ReadOrWrite)
            SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, NULL, CurrentDataSize+1);
        else
			{
                Mode.Direction = SPI_DATA_DIR_OUT;
                SPIDriver->Services->ConfigureMode(SPI_Transport, Mode);
				SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, DataBufferIn, 1);
				Mode.Direction = SPI_DATA_DIR_IN;
				SPIDriver->Services->ConfigureMode(SPI_Transport, Mode);
				SPIDriver->Services->TransferData(SPI_Transport, DataBuffer, DataBufferIn, 1);
			}
            

        SPIDriver->Services->SetChipSelect(SPI_Transport, SPIAdd, SPI_LEVEL_HIGH);    

        if (!ReadOrWrite)
            Data[(NbReg-RemainingDataSize)] = DataBufferIn[0];

        CurrentBaseReg = FindNextReg(CurrentBaseReg);
        RemainingDataSize -= CurrentDataSize;

    }while((RemainingDataSize > 0) && (CurrentBaseReg != -1));
       
    return 0;
}

adp_result_t ADS62P49_WriteRegisters (void * SPITransport, int SPIAdd,
                                     int BaseReg, int NbReg, char * DataIn)
{
    return (ADS62P49_XferRegisters(SPITransport, SPIAdd, 
                                  BaseReg, NbReg, DataIn, 1));                              
}

adp_result_t ADS62P49_ReadRegisters (void * SPITransport, int SPIAdd,
                                     int BaseReg, int NbReg, char * DataOut)
{
    return (ADS62P49_XferRegisters(SPITransport, SPIAdd, 
                                  BaseReg, NbReg, DataOut, 0));
}

adp_result_t ADS62P49_UpdateConfiguration (void * SPITransport, int SPIAdd,
                                          void * Configuration)
{
    //HM add at a later time if we still have budget ...
    return 0;
}

adp_result_t ADS62P49_ReadConfiguration (void * SPITransport, int SPIAdd,
                                          void * Configuration)
{
    struct ADS62P49_Register_Map * DevConfiguration;
    DevConfiguration = (struct ADS62P49_Register_Map *) Configuration;

    ADS62P49_ReadRegisters(SPITransport, SPIAdd,
                          ADS62P49_REGISTER_00, ADS62P49_NB_REGISTER, 
                          &DevConfiguration->RegisterArray[0]);

    return 0;
}

adp_result_t ADS62P49_WriteConfiguration (void * SPITransport, int SPIAdd,
                                          void * Configuration)
{
    struct ADS62P49_Register_Map * DevConfiguration;
    DevConfiguration = (struct ADS62P49_Register_Map *) Configuration;

    ADS62P49_WriteRegisters(SPITransport, SPIAdd,
                            ADS62P49_REGISTER_00, ADS62P49_NB_REGISTER, 
                            &DevConfiguration->RegisterArray[0]);

    return 0;
 }

FMCBoardDeviceServices Device_ADS62P49 = 
{
    NULL,
    ADS62P49_ReadRegisters,
    ADS62P49_WriteRegisters,
    ADS62P49_UpdateConfiguration,
    ADS62P49_ReadConfiguration,
    ADS62P49_WriteConfiguration 
};
