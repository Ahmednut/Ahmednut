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
 * @file       bas_spi.c
 * @brief      BAS SPI driver
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup DRIVERS
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
 
#include "bas_spi.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define SPI_DEBUG     1
#define MALLOC(A)  malloc(A)
#define FREE       free

#ifdef DEBUG
    #define func_enter()      //printf("spi: enter %s\n", __FUNCTION__)
    #define func_exit()       //printf("spi: exit  %s\n", __FUNCTION__)
    #define debug(f, ...) printf(f, __VA_ARGS__)
#else
    #define func_enter()
    #define func_exit()
    #define debug(f, ...)
#endif
/*
    Using Freescale Convention for defining clock polarity and phase in relation
    with data.
    Clock polarity defines idle clock polarity, ie when chip select is deasserted.
    Clock phase defines on which clock edge to latch data.
        Phase positive: data is latched on transition from idle clock state.
        Phase negative: data is latched on transition to idle clock state.

    Polarity Low, phase positive
    =============================
CS  ___                                      _____
       |____________________________________|
         _   _   _   _   _   _   _   _   
CLK ____| |_| |_| |_| |_| |_| |_| |_| |__
 
MOSI  X L X L X L X L X L X L X L X L X          : X: data changes, L data is latched
MISO  X L X L X L X L X L X L X L X L X          : X: data changes, L data is latched

    Polarity Low, phase negative
    =============================
CS  ___                                      _____
       |____________________________________|
         _   _   _   _   _   _   _   _   
CLK_____| |_| |_| |_| |_| |_| |_| |_| |__
 
MOSI    X L X L X L X L X L X L X L X L          : X: data changes, L data is latched
MISO    X L X L X L X L X L X L X L X L          : X: data changes, L data is latched


    Polarity High, phase positive
    =============================
CS  ___                                      _____
       |____________________________________|
________   _   _   _   _   _   _   _   __________
CLK     |_| |_| |_| |_| |_| |_| |_| |_|
 
MOSI  X L X L X L X L X L X L X L X L            : X: data changes, L data is latched
MISO  X L X L X L X L X L X L X L X L            : X: data changes, L data is latched

    Polarity High, phase negative
    =============================
CS  ___                                      _____
       |____________________________________|
________   _   _   _   _   _   _   _   __________
CLK     |_| |_| |_| |_| |_| |_| |_| |_|
 
MOSI    X L X L X L X L X L X L X L X L          : X: data changes, L data is latched
MISO    X L X L X L X L X L X L X L X L          : X: data changes, L data is latched


    Our SPI interface does not support phase control and require
    a clock transition to output the first data bit.
*/


/* Memory MAP of SPI Driver */
/*
    0: Control (32 bits):  0: clock Polarity
                         1-6: Transfer size in bit.
                           7: Direction 1: Output 0: Input
                           8: Start (Busy)
    1 - 6: Data buffer in/out: (8 bits)
    7: ChipSelect: (16 bits)
    8: Clockspeed divider: (8 bits)
*/

#define CONTROL_CLOCK_POL_      0x1
#define CONTROL_DIRECTION       0x80

#define CONTROL_NB_BITS_POS     (1)



int SPI_Open(int BaseAdd, int BaseClockFreq, void ** Driver);
int SPI_Init(void);
int SPI_Configure(SPITransport * SPIDevice, SPImode Mode);
int SPI_TransferData(SPITransport * SPIDevice, char * DataIn, char * DataOut, int NbData);
int SPI_SetChipSelect(SPITransport * SPIDevice, int CS, STATE_LEVEL State);
int SPI_Close(void * Driver);


const SPITransportServices SPI_Services =
{
     SPI_Init,
     SPI_Open,
     SPI_Configure,
     SPI_TransferData,
     SPI_SetChipSelect,
     SPI_Close,
};


int SPI_Open(int BaseAdd, int BaseClockFreq, void ** Driver)
{
	SPITransport * SPIDriver;

    func_enter();

	 //HM check pointer.
    SPIDriver = (SPITransport *) MALLOC (sizeof(SPITransport));
    if (SPIDriver == NULL)
        return -1;        

    SPIDriver->DeviceAddress = BaseAdd;
    SPIDriver->DataFrameLength = 6;
    SPIDriver->Services = &SPI_Services;
    SPIDriver->BaseClockFreq = BaseClockFreq;

	*Driver = (void *) SPIDriver;
   

    func_exit();

	return 0;
}

int SPI_Init(void)
{
    return 0;
}

int SPI_Configure(SPITransport * SPIDevice, SPImode Mode)
{
    volatile SPI_RegisterMap * Device;

    func_enter();

    Device = (SPI_RegisterMap *) SPIDevice->DeviceAddress;

#ifdef DEBUG_SPI

	printk("... In Control Reg Add: %x \n", &(Device->ControlReg.Value));        
	printk("... In Control Reg Val: %x \n", Device->ControlReg.Value);        

#endif

    SPIDevice->CurrentConfig = Mode;

    /* Our SPI interface has configurable clock polarity but not phase.
       The SPI state machine require a clock transition to shift out the 
       first data bit. To overcome this limitation, The clock polarity of device 
       requiring a positive clock phase will be inverted. */
    if (Mode.ClockPhase == SPI_PHASE_POSITIVE)
    {
        if (Mode.ClockPolarity == SPI_POLARITY_LOW)            
            Device->ControlReg.Value = Device->ControlReg.Value | CTRLREG_CLOCK_POL;
        else
            Device->ControlReg.Value = Device->ControlReg.Value & (~CTRLREG_CLOCK_POL);
    }
    else
    {
        if (Mode.ClockPolarity)
            Device->ControlReg.Value = Device->ControlReg.Value | CTRLREG_CLOCK_POL;
        else
            Device->ControlReg.Value = Device->ControlReg.Value & (~CTRLREG_CLOCK_POL);
     }
        

//    Device->ClockDiv = (SPIDevice->BaseClockFreq + (Mode.ClockSpeed_bps -1 ) / 
//                        Mode.ClockSpeed_bps);

    //HMremove next line
    Device->ClockDiv =15;
#ifdef DEBUG_SPI
	printk("... Out Control Reg Add: %x \n", &(Device->ControlReg.Value));        
	printk("... Out Control Reg Val: %x \n", Device->ControlReg.Value);        
#endif
    func_exit();

    return 0;
}
volatile int pause;
int SPI_TransferData(SPITransport * SPIDevice, char * DataIn,
                      char * DataOut, int NbData)
{
    int Tempval;
    int CountMax = 0;
    char NbTx, NbTxIdx, CurrentTx;
    volatile SPI_RegisterMap * Device;

    func_enter();

    Device = (SPI_RegisterMap *) SPIDevice->DeviceAddress;
#ifdef DEBUG_SPI  	
    printk("... NbData: %x \n", NbData);        
	printk("... In Control Reg Add: %x \n", &(Device->ControlReg.Value));        
	printk("... In Control Reg Val: %x \n", Device->ControlReg.Value);        
#endif

    // FPGA's spi interface can send up to 6 data in the single transfert.
    NbTx = (NbData+(SPIDevice->DataFrameLength-1)) / SPIDevice->DataFrameLength;

    for (NbTxIdx = 0; NbTxIdx < NbTx; NbTxIdx ++)
    {
        if (NbData - (NbTxIdx * SPIDevice->DataFrameLength) > SPIDevice->DataFrameLength)
            CurrentTx = SPIDevice->DataFrameLength;
        else
            CurrentTx = NbData - (NbTxIdx * SPIDevice->DataFrameLength);
#ifdef DEBUG_SPI
    	printk("... CurrentTx %x \n",CurrentTx);        
#endif
        Device->ControlReg.Value =  Device->ControlReg.Value  & 0xff81;       
        Device->ControlReg.Value =  Device->ControlReg.Value | ((CurrentTx*8-1)<<1);
//        Device->ControlReg.TransferSize = CurrentTx*8-1;

        // Put data in SPI buffer if output data direction is set
        if (SPIDevice->CurrentConfig.Direction & SPI_DATA_DIR_OUT)
        {
            int Index;
            for(Index = 0; Index  < NbData; Index ++)
                Device->Data[Index ] = DataIn[(NbTxIdx * SPIDevice->DataFrameLength)+Index ];

//            Device->ControlReg.Direction = 1;
              Device->ControlReg.Value =  Device->ControlReg.Value | CTRLREG_DIRECTION;
        }
        else
//            Device->ControlReg.Direction = 0;
              Device->ControlReg.Value =  Device->ControlReg.Value & (~CTRLREG_DIRECTION);
        
#ifdef DEBUG_SPI
    	printk("... In Control Reg Add: %x \n", &(Device->ControlReg.Value));        
	    printk("... In Control Reg Val: %x \n", Device->ControlReg.Value);        
#endif
        // kick spi.        
        //Device->ControlReg.Start = 1;
        Tempval = Device->ControlReg.Value;
        Device->ControlReg.Value = Device->ControlReg.Value | 0x100;
#ifdef DEBUG_SPI
    	printk("... Start 1 Control Reg Add: %x \n", &(Device->ControlReg.Value));        
	    printk("... Start 1 Control Reg Val: %x \n", Device->ControlReg.Value);        
#endif
//        Device->ControlReg.Start = 0;
#ifdef DEBUG_SPI
    	printk("... Start 2 Control Reg Add: %x \n", &(Device->ControlReg.Value));        
    	printk("... Start 2 Control Reg Val: %x \n", Device->ControlReg.Value);        
#endif
        while(Device->ControlReg.Value & 0x200)
        {
            CountMax ++;
            if(CountMax >= 10000)
            {
                return -1;
                func_exit();
            }

        }

        Device->ControlReg.Value = Tempval;

        // Put data in user buffer if input data direction is set.
        if (SPIDevice->CurrentConfig.Direction & SPI_DATA_DIR_IN)
        {
            int Index;
            for(Index = 0; Index < NbData; Index++)
                DataOut[(NbTxIdx * SPIDevice->DataFrameLength)+Index] = Device->Data[Index];
        }

    }
#ifdef DEBUG_SPI
	printk("... Out Control Reg Add: %x \n", &(Device->ControlReg.Value));        
	printk("... Out Control Reg Val: %x \n", Device->ControlReg.Value);        
#endif
    func_exit();
    return 0;
}

int SPI_SetChipSelect(SPITransport * SPIDevice, int CS, 
                      STATE_LEVEL State)
{
    volatile SPI_RegisterMap * Device;
    int ChipSelBit;

    func_enter();

    Device = (SPI_RegisterMap *) SPIDevice->DeviceAddress;

    ChipSelBit = 1<<CS;

    if (State == SPI_LEVEL_LOW)
        Device->ChipSelect = ~ChipSelBit;
    else
        Device->ChipSelect = 0xffff;

    func_exit();
    return 0;
}

int SPI_Close(void * Driver)
{
    //hm check pointer
    FREE(Driver);
    return 0;
}

