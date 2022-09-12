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
 *  @file       : adac250.c
 *  @brief      : ADAC250 core register and functions definitions 
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADAC250
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
#include "adac250.h"
#include "bas_fmc_device_def.h"
#include "bas_spi.h"
#include "DAC5682_drv.h"
#include "ADS62P49_drv.h"
#include "AD9511_drv.h"
#include "LTC2641_drv.h"
#include "bas_error.h"


#define CheckCall(A)    A
#define MALLOC(A)  malloc(A)
#define FREE       free

#ifdef DEBUG
    #define func_enter()      printf("ADAC250: enter %s\n", __FUNCTION__)
    #define func_exit()       printf("ADAC250: exit  %s\n", __FUNCTION__)
    #define debug(f, ...) printf(f, __VA_ARGS__)
#else
    #define func_enter()
    #define func_exit()
    #define debug(f, ...)
#endif

int (*pfADAC250_usleep)(unsigned int usec);

adp_result_t  ADAC250_Init (void)
{
    return 0;
}


adp_result_t  ADAC250_Open(unsigned int ADAC250BaseAdd,
                           int (*pfUsleep)(unsigned int usec),
                           ADAC250Board_t ** ADAC250Board)
{
    //HM Check Open already done.
    func_enter();
    
    if(pfUsleep == NULL) return ERR_INVPTR;
    
    pfADAC250_usleep = pfUsleep;

    //HM detection of the board will occur at a higher level, because we need
    // to parse the eeprom which is totaly out of the scope of the driver. So
    // when the ADAC250 high level API is completed, removes thoses fake detection.
    * ADAC250Board = (ADAC250Board_t*) MALLOC (sizeof(ADAC250Board_t ));
    //HM check pointer
    (* ADAC250Board )->BaseAdd = ADAC250BaseAdd;
    (* ADAC250Board )->Adaptor = (ADAC250Adaptator_t *) ADAC250BaseAdd;

   // Open the SPI driver.
    SPI_Open((int) &(* ADAC250Board )->Adaptor->SPIDevice,
            125000000,
            (void *) & (* ADAC250Board )->SPI_Transport);

    // Create a FMC  board with 4 devices.

    (*ADAC250Board)->Device[0].Services = &Device_AD9511;
    (*ADAC250Board)->Device[0].Address = 0;  //chip select;
    (*ADAC250Board)->Device[0].TransportDriver = (* ADAC250Board )->SPI_Transport;

    (*ADAC250Board)->Device[1].Services = &Device_DAC5682;
    (*ADAC250Board)->Device[1].Address = 2;  //chip select;
    (*ADAC250Board)->Device[1].TransportDriver = (* ADAC250Board )->SPI_Transport;

    (*ADAC250Board)->Device[2].Services = &Device_ADS62P49;
    (*ADAC250Board)->Device[2].Address = 1;  //chip select;
    (*ADAC250Board)->Device[2].TransportDriver = (* ADAC250Board )->SPI_Transport;

    (*ADAC250Board)->Device[3].Services = &Device_LTC2641;
    (*ADAC250Board)->Device[3].Address = 3;  //chip select;
    (*ADAC250Board)->Device[3].TransportDriver = (* ADAC250Board )->SPI_Transport;
    
    // Reset core register
    (*ADAC250Board)->Adaptor->ADC_Control.CoreResetPulse = 1;

    func_exit();

    return 0;
}


adp_result_t  ADAC250_WriteSPIRegisters(ADAC250Board_t * ADAC250Board,
                                    ADAC250_DEVICES DeviceId,
                                    int BaseRegister, int Nb, char * Value)
{
    FMCBoardDevice * TheDevice;

  //  func_enter();
    TheDevice = &ADAC250Board->Device[DeviceId];

    // get access to SPI bus
    ADAC250Board->Adaptor->Misc_Control.Register |= MISC_CTRL_REQUEST_2;
    while(!(ADAC250Board->Adaptor->Misc_Control.Register &  MISC_CTRL_GRANT_2));

    //check pointer, check device ID
    TheDevice->Services->WriteRegisters(TheDevice->TransportDriver,
                                        TheDevice->Address,
                                        BaseRegister, Nb, Value);

    // release SPI bus
    ADAC250Board->Adaptor->Misc_Control.Register &= ~MISC_CTRL_REQUEST_2;    
    debug("WriteSPIRegisters: %x  DeviceId: %x  BaseRegister: %x  Nb: %x  Value: %p\n", ADAC250Board, DeviceId, BaseRegister, Nb, *Value);

   // func_exit();
    return 0;
}


adp_result_t  ADAC250_ReadSPIRegisters(ADAC250Board_t * ADAC250Board,
                                    ADAC250_DEVICES DeviceId,
                                    int BaseRegister, int Nb, char * Value)
{
    FMCBoardDevice * TheDevice;

  //  func_enter();

    TheDevice = &ADAC250Board->Device[DeviceId];
 //   debug("read device %p\n", TheDevice);

    // get access to SPI bus
  //  debug("Control register addr: %p\n", &ADAC250Board->Adaptor->Misc_Control);
    ADAC250Board->Adaptor->Misc_Control.Register |= MISC_CTRL_REQUEST_2;
  //  debug("Control register: %x\n", ADAC250Board->Adaptor->Misc_Control.Register);
    while(!(ADAC250Board->Adaptor->Misc_Control.Register &  MISC_CTRL_GRANT_2));
    
    //check pointer, check device ID
    TheDevice->Services->ReadRegisters( TheDevice->TransportDriver,
                                        TheDevice->Address,
                                        BaseRegister, Nb, Value);
    // release SPI bus
    ADAC250Board->Adaptor->Misc_Control.Register &= ~MISC_CTRL_REQUEST_2;    
    debug("ReadSPIRegisters: %x  DeviceId: %x  BaseRegister: %x  Nb: %x  Value: %p\n", ADAC250Board, DeviceId, BaseRegister, Nb, *Value);

   // func_exit();
    return 0;
}

adp_result_t  ADAC250_WriteControlRegister(ADAC250Board_t * ADAC250Board,
                                         ADAC250_CONTROL_REGISTER Register,
                                         unsigned int RegisterValue)
{
    debug("ADAC250_WriteControl %x : %x\n", Register, RegisterValue);
    switch(Register)
    {
        case ADAC250_ADC_CONTROL:
            ADAC250Board->Adaptor->ADC_Control.Register = RegisterValue;
            break;

        case ADAC250_DAC_CONTROL:
            ADAC250Board->Adaptor->DAC_Control.Register = RegisterValue;
            break;

        case ADAC250_MISC_CONTROL:
            ADAC250Board->Adaptor->Misc_Control.Register = RegisterValue;
            break;

        case ADAC250_ADCDELAY_CONTROL:
            ADAC250Board->Adaptor->AdcDelay_Control.Register = RegisterValue;
            break;
            
        case ADAC250_DACDELAY_CONTROL:
            ADAC250Board->Adaptor->DacDelay_Control.Register = RegisterValue;
            break;
            
        case ADAC250_FREQCNTCLK_CONTROL:
            ADAC250Board->Adaptor->FreqCntClk_Control.Register = RegisterValue;
            break;

        default:
            return -1;
            break;
    }
    return 0;
}

adp_result_t  ADAC250_ReadControlRegister(ADAC250Board_t * ADAC250Board, int Register, unsigned int * RegisterValue)
{
    if (!RegisterValue)
        return -1;

    switch(Register)
    {
        case ADAC250_ADC_CONTROL:
            *RegisterValue = ADAC250Board->Adaptor->ADC_Control.Register;
            break;

        case ADAC250_DAC_CONTROL:
            *RegisterValue = ADAC250Board->Adaptor->DAC_Control.Register;
            break;

        case ADAC250_MISC_CONTROL:
            *RegisterValue = ADAC250Board->Adaptor->Misc_Control.Register;
            break;

        case ADAC250_ADCDELAY_CONTROL:
            *RegisterValue = ADAC250Board->Adaptor->AdcDelay_Control.Register;
            break;

        case ADAC250_DACDELAY_CONTROL:
            *RegisterValue = ADAC250Board->Adaptor->DacDelay_Control.Register;
            break;

        case ADAC250_FREQCNTCLK_CONTROL:
            *RegisterValue = ADAC250Board->Adaptor->FreqCntClk_Control.Register;
            break;

        default:
            return -1;
            break;
    }

    return 0;
}


adp_result_t  ADAC250_Close(ADAC250Board_t * ADAC250Board)
{
    FREE(ADAC250Board);

    return 0;
}


/*
adp_result_t  ADAC250_ClockSwitch()
{

}
*/
