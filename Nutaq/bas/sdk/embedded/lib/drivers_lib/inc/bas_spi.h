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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      bas_spi.h
 * 
 * @brief     BAS SPI enumerations and structures
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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


#ifndef _BAS_SPI_H
#define _BAS_SPI_H

typedef struct S_SPI_RegisterMap
{
    union
    {
        struct
        {
             volatile unsigned Padding:22;
             volatile unsigned Busy:1;
             volatile unsigned Start:1;
             volatile unsigned Direction:1;
             volatile unsigned TransferSize:6;
             volatile unsigned ClockPolarity:1;
        };
        volatile unsigned int Value;
    }ControlReg;

    volatile unsigned int Data[6];
    volatile unsigned int ChipSelect;
    volatile unsigned int ClockDiv;
}SPI_RegisterMap;

#define CTRLREG_CLOCK_POL    1
#define CTRLREG_DIRECTION    0x80
#define CTRLREG_START        0x100
#define CTRLREG_BUSY         0x200

typedef enum E_STATE_LEVEL
{
    SPI_LEVEL_LOW,
    SPI_LEVEL_HIGH
}STATE_LEVEL;

typedef enum E_CLOCK_PHASE
{
    SPI_PHASE_POSITIVE,     //data latched on first transition of clock from clk idle state.
    SPI_PHASE_NEGATIVE
}CLOCK_PHASE;

typedef enum E_CLOCK_POLARITY
{
    SPI_POLARITY_LOW,      //Idle clock polarity.
    SPI_POLARITY_HIGH
}CLOCK_POLARITY;

typedef enum E_DATA_DIRECTION
{
    SPI_DATA_DIR_IN = 1,
    SPI_DATA_DIR_OUT = 2
}DATA_DIRECTION;

typedef struct S_SPImode
{
    DATA_DIRECTION   Direction;
    CLOCK_PHASE      ClockPhase;
    CLOCK_POLARITY   ClockPolarity;
    int              ClockSpeed_bps;
    int              BitsPerWord;
    STATE_LEVEL      ChipSelectPolarity;     // optionnal
    int              ChipSelect;             // optionnal
}SPImode;


typedef struct S_SPITransport
{
    int DeviceAddress;
    int BaseClockFreq;         // base clock speed of SPI in Hz
    int DataFrameLength;
    SPImode CurrentConfig;
    const struct S_SPITransportServices *Services;
}SPITransport;

typedef struct S_SPITransportServices
{
   int (* Init)(void);
   int (* Open)(int BaseAdd, int BaseClockSpeed, void ** Driver);
   int (* ConfigureMode)(SPITransport *SPIDevice, SPImode Mode);
   int (* TransferData)(SPITransport *SPIDevice, char * DataIn, char * DataOut, int NbData);
   int (* SetChipSelect)(SPITransport *SPIDevice, int CS_N, STATE_LEVEL Level);
   int (* Close)(void * Driver);
}SPITransportServices;


int SPI_Init(void);
int SPI_Open(int BaseAdd, int BaseClockFreq, void ** Driver);
int SPI_Close(void * Driver);

#endif 
