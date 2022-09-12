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
 * @file      CustomRegister_Write.c
 * 
 * @brief     Function that write in custom registers
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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
 #ifdef __linux__
    #include "linux_utils.h"
#else
    #include <conio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "bas_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "bas_error.h"
#include "bas_time.h"
#include "Gpio.h"
#include "string.h"
// WARNING: to run this demo under LINUX, you may need root privilege

Result gpio(connection_state * state,
            char* argv[])
{
    adp_result_t res;

    // Buffers for values read from the FPGA
    unsigned int DirectionMaskInFPGA = 0;
    unsigned int OwnerMaskInFPGA = 0;
    unsigned int GpioReadValueInFPGA = 0;

    // Value that will be written to GpioWriteRegAddr register
    unsigned int GpioWriteValue = 0;

    // init command
    if( strcmp(GpioCommandFromCLI, "init") == 0 )
    {
        printf("*** WARNING: GPIOs 1 to 4 are inputs and GPIOs 5 to 8 are outputs (GPIO1 is the LSB and GPIO8 is the MSB). ***\n");
    
		//Set GPIO Owner tobe AXI Bus (instead of FPGA interface)
        res = memory_write_send(state, GpioOwnerMaskRegAddr, OwnerMaskFromCLI);
        if(FAILURE(res))
            return res;

		res = memory_read_send(state, GpioDirectionMaskRegAddr, &DirectionMaskInFPGA);
		printf("GPIO direction mask: 0x%x\n", DirectionMaskInFPGA);
    }

    // write command
    if( strcmp(GpioCommandFromCLI, "write") == 0 )
    {
        res = memory_read_send(state, GpioDirectionMaskRegAddr, &DirectionMaskInFPGA);
        if(FAILURE(res))
            return res;

		//temporary
		//DirectionMaskInFPGA = 0xF0;

        DirectionMaskInFPGA &= 0xFF;
        GpioWriteValue = WriteValueFromCLI & DirectionMaskInFPGA;

        res = memory_write_send(state, GpioWriteRegAddr, GpioWriteValue);
        if(FAILURE(res))
            return res;
		printf("GPIO write value: 0x%x\n", GpioWriteValue);
    }

    // read command
    if( strcmp(GpioCommandFromCLI, "read") == 0 )
    {
        res = memory_read_send(state, GpioReadRegAddr, &GpioReadValueInFPGA);
        if(FAILURE(res))
            return res;

        GpioReadValueInFPGA &= 0xFF;
        printf("GPIO read value: 0x%x\n", GpioReadValueInFPGA);
    }

    // getOwnerMask command
    if( strcmp(GpioCommandFromCLI, "getOwnerMask") == 0 )
    {
        res = memory_read_send(state, GpioOwnerMaskRegAddr, &OwnerMaskInFPGA);
        if(FAILURE(res))
            return res;

        OwnerMaskInFPGA &= 0xFF;
        printf("GPIO owner mask: 0x%x\n", OwnerMaskInFPGA);
    }


    return OK;
}
