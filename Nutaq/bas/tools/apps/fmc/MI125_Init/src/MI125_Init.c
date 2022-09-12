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
 * @file      MI125_Init.c
 * 
 * @brief     Function that initialize the MI125 FMC
 *
 * Copyright (C) 2015, Nutaq, Canada. All rights reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include "bas_types.h"
#include "bas_error.h"
#include "connection_state.h"
#include "eapi.h"
#include "eapi_mi125.h"
#include "MI125_mi125_defines.h"
#include "MI125_Init.h"

// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
														 FMC Init
****************************************************************************************************************************/
Result MI125_Init(connection_state * state, 
					handle_mi125 * handle)
{
	adp_result_t res = OK;	
	uint32_t i, master, core_version, drv_version;
	int16_t temp;

	/************************************************************************************************************************
													FMC Configuration
	************************************************************************************************************************/
	for ( i = 0 ; i < MAX_MI125 ; i++) //TODO Validate index
	{
		// Validate if active
		if (handle[i] == NULL)
		{
			continue;
		}

		// Display current parameters to set
		printf("   - Board number : %u\n",i+1);
		printf("\t - Sampling clock source : %u\n",handle[i]->eSamplingClockSource);
		// Display current parameters to set
		if (handle[i]->eTrigOut == MI125_TRIGOUTOFF)
		{
			printf("\t - Trigger output IO : OFF\n");
		}
		else
		{
			printf("\t - Trigger output IO : ON\n");
		}

		// Verify MI125 FPGA Core and FMC Card presence
		res = MI125_Presence_send( state, i+1 );
		if(FAILURE(res))
		{
			return res;
		}

		// Display if this module is a clock master or not.
		res = MI125_mi125_get_clkmaster_send( state, i+1, &master);
		if(FAILURE(res))
		{
			return res;
		}

		printf("\t - Is a clock master : ");
		if (master)
		{
			printf("Yes.\n");
		}
		else
		{
			printf("No.\n");
		}

		// Display MI125 hardware core and driver versions (for info only)
		res = MI125_mi125_get_versions_send( state, i+1, &core_version, &drv_version);
		if(FAILURE(res))
		{
			return res;
		}

		printf("\t - Core Version: 0x%04X\n", core_version);
		printf("\t - Driver Version: 0x%04X\n", drv_version);

		// Power up MI125
		res = MI125_powerup_send( state, i+1 );
		if(FAILURE(res))
		{
			return res;
		}

		// Reset and setup MI125 with basic default configuration (after module powerup, all 16 channels are configured)
		res = MI125_mi125_reset_send( state, i+1 );
		if(FAILURE(res))
		{
			return res;
		}

		// Setup ADC clock source
		res = MI125_mi125_set_clksrc_send( state, i+1, handle[i]->eSamplingClockSource);
		if(FAILURE(res))
		{
			return res;
		}

		// Setup board trigout function (do not connect an externel trig source on the external trig pin at the same time)
		// To use the trigout function, it must be connected somewhere in the FPGA user logic (not provided by default)
		res = MI125_mi125_set_trigout_send( state, i+1, handle[i]->eTrigOut);
		if(FAILURE(res))
		{
			return res;
		}

		// Config the MI125 with specified user setup (if different than default provided at reset)
		res = MI125_mi125_set_config_send( state, i+1, MI125_16CHANNELS, MI125_TERMON1750UA, MI125_RANDOMIZEOFF, MI125_TWOCOMPLEMENTFORMAT);
		if(FAILURE(res))
		{
			return res;
		}

		// Display the current MI125 pcb temperature in 0.1C resolution
		// WARNING, this temperature is for information only. This does not
		// give ADC core temperature information. User must make sure that
		// temperature of MI125 board (incl ADC) meets the requierements in his
		// system. Some calibration could be needed between this value
		// and different places/parts on the MI125 board.
		res = MI125_mi125_get_temperature_send( state, i+1, MI125_TEMP0DOT1C, &temp);
		if(FAILURE(res))
		{
			return res;
		}
		printf("\t - PCB temp: %d.%01dC\n", temp/10,abs(temp%10));

	}
    return OK;
}
