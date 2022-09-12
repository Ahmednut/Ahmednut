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
 * @file       mi250.c
 * @brief      MI250 configuration functions API
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI250
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
#include "mi250.h"
#include "mi250_pll.h"
#include "mi250_types.h"
#include "mi250_defines.h"
#include "ADS62P49_drv.h"
#include "mi250_AD9510_drv.h"
#include "bas_fmc_device_def.h"
#include "bas_error.h"

#define CheckCall(A)    A

#define MALLOC(A)  malloc(A)
#define FREE       free

#ifdef DEBUG
    #define func_enter()      printf("MI250: enter %s\n", __FUNCTION__)
    #define func_exit()       printf("MI250: exit  %s\n", __FUNCTION__)
    #define debug(f, ...) printf(f, __VA_ARGS__)
#else
    #define func_enter()
    #define func_exit()
    #define debug(f, ...)
#endif


adp_result_t  MI250_Init (void)
{
    return 0;
}

adp_result_t  MI250_Open(unsigned int MI250BaseAdd,
                          int (*pfUsleep)(unsigned int usec),
                          MI250Board_t ** MI250Board)
{
    //CheckCall(FMC_GetTransportDriver(FMCAdapter, SPI_0, (void ** ) &SPITransport));

    * MI250Board = (MI250Board_t *) MALLOC (sizeof(MI250Board_t ));
    memset(*MI250Board,0,sizeof(MI250Board_t));
    
    (*MI250Board)->BaseAdd = MI250BaseAdd;
    (*MI250Board)->Adaptor = (MI250Adaptator_t *) MI250BaseAdd;
    (*MI250Board)->pfUsleep = pfUsleep;

   // Open the SPI driver.
    SPI_Open((int) &(* MI250Board )->Adaptor->SPIDevice,
             125000000,
             (void *) & (* MI250Board )->SPI_Transport);

    (*MI250Board)->Device[0].Services = &Device_ADS62P49;
    (*MI250Board)->Device[0].Address = 0;  //chip select;
    (*MI250Board)->Device[0].TransportDriver = (* MI250Board )->SPI_Transport;

    (*MI250Board)->Device[1].Services = &Device_ADS62P49;
    (*MI250Board)->Device[1].Address = 1;  //chip select;
    (*MI250Board)->Device[1].TransportDriver = (* MI250Board )->SPI_Transport;

    (*MI250Board)->Device[2].Services = &Device_ADS62P49;
    (*MI250Board)->Device[2].Address = 2;  //chip select;
    (*MI250Board)->Device[2].TransportDriver = (* MI250Board )->SPI_Transport;

    (*MI250Board)->Device[3].Services = &Device_ADS62P49;
    (*MI250Board)->Device[3].Address = 3;  //chip select;
    (*MI250Board)->Device[3].TransportDriver = (* MI250Board )->SPI_Transport;

    (*MI250Board)->Device[4].Services = &MI250_Device_AD9510;
    (*MI250Board)->Device[4].Address = 4;  //chip select;
    (*MI250Board)->Device[4].TransportDriver = (* MI250Board )->SPI_Transport;

    //func_exit();

    return 0;
}


adp_result_t  MI250_WriteSPIRegisters(MI250Board_t * MI250Board,
                                       MI250_DEVICES DeviceId,
                                       int BaseRegister, int Nb, char * Value)
{
    int * addread;
//    int idx;

    FMCBoardDevice * TheDevice;

    addread   = (int * ) MI250Board->Adaptor;

   // func_enter();
    TheDevice = &MI250Board->Device[DeviceId];

    // get access to SPI bus
    MI250Board->Adaptor->SpiReg_Control.Register |= MI250_MISC_CTRL_REQUEST_2;
    while(!(MI250Board->Adaptor->SpiReg_Control.Register &  MI250_MISC_CTRL_GRANT_2));

    //check pointer, check device ID
    TheDevice->Services->WriteRegisters(TheDevice->TransportDriver,
                                        TheDevice->Address,
                                        BaseRegister, Nb, Value);

    // release SPI bus
    MI250Board->Adaptor->SpiReg_Control.Register &= ~MI250_MISC_CTRL_REQUEST_2;

   // func_exit();
    return 0;
}


adp_result_t  MI250_ReadSPIRegisters(MI250Board_t * MI250Board,
                                      MI250_DEVICES DeviceId,
                                      int BaseRegister, int Nb, char * Value)
{
    FMCBoardDevice * TheDevice;

   // func_enter();
    debug("MI250Board: %x  DeviceId: %x  BaseRegister: %x  Nb: %x  Value: %p\n", MI250Board, DeviceId, BaseRegister, Nb, Value);
    TheDevice = &MI250Board->Device[DeviceId];
    debug("read device %p\n", TheDevice);

    // get access to SPI bus
    debug("Control register addr: %p\n", &MI250Board->Adaptor->SpiReg_Control);
    MI250Board->Adaptor->SpiReg_Control.Register |= MI250_MISC_CTRL_REQUEST_2;
    debug("Control register: %x\n", MI250Board->Adaptor->SpiReg_Control.Register);
    while(!(MI250Board->Adaptor->SpiReg_Control.Register &  MI250_MISC_CTRL_GRANT_2));

    //check pointer, check device ID
    TheDevice->Services->ReadRegisters( TheDevice->TransportDriver,
                                        TheDevice->Address,
                                        BaseRegister, Nb, Value);
    // release SPI bus
    MI250Board->Adaptor->SpiReg_Control.Register &= ~MI250_MISC_CTRL_REQUEST_2;

  //  func_exit();
    return 0;
}

adp_result_t  MI250_WriteControlRegister(MI250Board_t * MI250Board,
                                          MI250_CONTROL_REGISTER Register,
                                          int RegisterValue)
{
    debug("MI250_WriteControl %x : %x\n", Register, RegisterValue);
    switch(Register)
    {
        case MI250_ADC_CONTROL:
            MI250Board->Adaptor->ADC_Control.Register = RegisterValue;
            break;
        case MI250_PLL_CONTROL:
            MI250Board->Adaptor->Pll_Control.Register = RegisterValue;
            break;
        case MI250_SPIREG_CONTROL:
            MI250Board->Adaptor->SpiReg_Control.Register = RegisterValue;
            break;
        case MI250_ADC_12_CONTROL:
            MI250Board->Adaptor->ADC_12_Control.Register = RegisterValue;
            break;
        case MI250_ADC_34_CONTROL:
            MI250Board->Adaptor->ADC_34_Control.Register = RegisterValue;
            break;
        case MI250_ADC_56_CONTROL:
            MI250Board->Adaptor->ADC_56_Control.Register = RegisterValue;
            break;
        case MI250_ADC_78_CONTROL:
            MI250Board->Adaptor->ADC_78_Control.Register = RegisterValue;
            break;
        case MI250_VOLT_MON_CONTROL:
            MI250Board->Adaptor->VoltMon_Control.Register = RegisterValue;
            break;
        case MI250_ADC_BURST_SIZE:
            MI250Board->Adaptor->ADC_BurstSize.Register = RegisterValue;
            break;
        case MI250_ADC_NB_BURST:
            MI250Board->Adaptor->ADC_NbBurst.Register = RegisterValue;
            break;
        case MI250_ADC_INTERVAL:
            MI250Board->Adaptor->ADC_Interval.Register = RegisterValue;
            break;
        default:
            return -1;
            break;
    }
    return 0;
}

adp_result_t  MI250_ReadControlRegister(MI250Board_t * MI250Board, int Register, int * RegisterValue)
{
    if (!RegisterValue)
        return -1;

    switch(Register)
    {
        case MI250_ADC_CONTROL:
            *RegisterValue = MI250Board->Adaptor->ADC_Control.Register;
            break;
        case MI250_PLL_CONTROL:
            *RegisterValue = MI250Board->Adaptor->Pll_Control.Register;
            break;
        case MI250_SPIREG_CONTROL:
            *RegisterValue = MI250Board->Adaptor->SpiReg_Control.Register;
            break;
        case MI250_ADC_12_CONTROL:
            *RegisterValue = MI250Board->Adaptor->ADC_12_Control.Register;
            break;
        case MI250_ADC_34_CONTROL:
            *RegisterValue = MI250Board->Adaptor->ADC_34_Control.Register;
            break;
        case MI250_ADC_56_CONTROL:
            *RegisterValue = MI250Board->Adaptor->ADC_56_Control.Register;
            break;
        case MI250_ADC_78_CONTROL:
            *RegisterValue = MI250Board->Adaptor->ADC_78_Control.Register;
            break;
        case MI250_VOLT_MON_CONTROL:
            *RegisterValue = MI250Board->Adaptor->VoltMon_Control.Register;
            break;
        case MI250_PLL_CLK_FREQ:
            *RegisterValue = MI250Board->Adaptor->Pll_ClockFreq.Register;
            break;
        case MI250_ADC_1234_CLK_FREQ:
            *RegisterValue = MI250Board->Adaptor->ADC_1234_ClkFreq.Register;
            break;
        case MI250_ADC_5678_CLK_FREQ:
            *RegisterValue = MI250Board->Adaptor->ADC_5678_ClkFreq.Register;
            break;
        case MI250_ADC_BURST_SIZE:
            *RegisterValue = MI250Board->Adaptor->ADC_BurstSize.Register;
            break;
        case MI250_ADC_NB_BURST:
            *RegisterValue = MI250Board->Adaptor->ADC_NbBurst.Register;
            break;
        case MI250_ADC_INTERVAL:
            *RegisterValue = MI250Board->Adaptor->ADC_Interval.Register;
            break;
        default:
            return -1;
            break;
    }

    return 0;
}

adp_result_t MI250_InitPLL(MI250Board_t * MI250Board, unsigned int refclk, unsigned int acqclk, MI250_CLOCK_SOURCE clksrc)
{
	MI250_PLL_PARAM param;
	Result result;
	Result calibresult;
	if(acqclk < MI250_MIN_ADC_PLL_CLK_FREQ)
	{
		return (MI250_NO_VALID_PLL_SOLUTION);
	}
	switch(clksrc)
	{
		case MI250_CLOCKIN_100MHZ:
			// If internal reference clock to be used
			result = MI250_PLL_Init(MI250Board);
            if(FAILURE(result)) return result;
            
			// Set PLL to 250MHz
			result = MI250_PLL_CalculateParams(refclk,
								acqclk, MI250_PLL_PHASE_NOISE, &param);
            if(FAILURE(result)) return result;
            
			result = MI250_PLL_SetConfig(MI250Board, param);
            if(FAILURE(result)) return result;
            
			break;
            
		case MI250_CLOCKIN_EXTERNALREF:
			// If external reference clock to be used
			result = MI250_PLL_Init_Extern_Ref(MI250Board);
            if(FAILURE(result)) return result;
            
			// Set PLL to 250MHz
			result = MI250_PLL_CalculateParams(refclk,
								acqclk, MI250_PLL_PHASE_NOISE, &param);
            if(FAILURE(result)) return result;
            
			result = MI250_PLL_SetConfig(MI250Board, param);
            if(FAILURE(result)) return result;
            
			break;
            
		case MI250_CLOCKIN_EXTERNALCLOCK:
			// If external sampling clock to be used
			result = MI250_PLL_Init_Extern(MI250Board);
            if(FAILURE(result)) return result;
			break;
            
        default:
            result = -1;
            if(FAILURE(result)) return result;
            break;
	}

	MI250Board->pfUsleep(1000);
	calibresult = MI250_PLL_Calibrate(MI250Board, acqclk);
	if(FAILURE(calibresult)) return calibresult;

	return(result);
}

adp_result_t  MI250_Close(MI250Board_t * MI250Board)
{
    FREE(MI250Board);

    return 0;
}

adp_result_t MI250_SetLaneDelay(MI250Board_t * MI250Board, FMCEEPROM_HPC_DATA_Specs * pLaneDelay)
{
    MI250Board->pLaneDelay = pLaneDelay;
    return OK;
}

adp_result_t MI250_SetMasterCalibResult(MI250Board_t * MI250Board, MI250Board_t * MasterMI250Board)
{
    MI250Board->pMasterCalibResult = &(MasterMI250Board->CalibResult);
    return OK;
}



