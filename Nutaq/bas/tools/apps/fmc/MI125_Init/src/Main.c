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
 * @file      main.c
 *
 * @brief     Main function
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
#ifdef __linux__
	#include "linux_utils.h"
	#include <strings.h>
#else
	#include <conio.h>
#endif 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bas_error.h"
#include "bas_types.h"
#include "eapi.h"
#include "connection_state.h"
#include "MI125_Init.h"
#include "ConfigFile.h"

// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
*										     Command Line Arguments Access Defines
****************************************************************************************************************************/
#define IpAddress			argv[1]
#define ConfigFileName		argv[2]

/****************************************************************************************************************************
*										         Static Function Declarations
****************************************************************************************************************************/
static void Terminate(adp_result_t res);
static adp_result_t ValidatingInitCall(handle_mi125 * handle);
static adp_result_t FileParser(const char * Filename, handle_mi125 * handle);

handle_mi125 hMi125[MAX_MI125] = {NULL};

/****************************************************************************************************************************
															Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
	adp_result_t res;
	connection_state state;	

	/************************************************************************************************************************
												 Initialize eapi before use
	************************************************************************************************************************/
	printf("\n\n---------- MI125_Init ----------\n\n");

	/************************************************************************************************************************
												Validating number of arguments
	************************************************************************************************************************/
	if (argc != 3)
	{
		printf( "Error: Wrong number of parameters\n"
               "Parameters 1 must be the Perseus IP address, ex 192.168.0.101,\n"
			   "Parameters 2 must be the Configuration file name, ex MI125_Init.ini,\n"
               " The program will terminate. Push any key to continue\n" );
       GETCH();
       return -1;
	}

	/************************************************************************************************************************
													  Parsing .ini file
	************************************************************************************************************************/
	printf("Parsing %s file for needed parameters...",ConfigFileName);
	res = FileParser(ConfigFileName, hMi125);
	if(FAILURE(res))
	{
		printf("Error with FileParser\n");
		Terminate(res);
		return res;
	}
	printf("Done!\n\n");

	/************************************************************************************************************************
												Validating if calling main function
	************************************************************************************************************************/	
	res = ValidatingInitCall(hMi125);
	if(FAILURE(res))	
	{		
		printf("Error with ValidatingInitCall\n");
		Terminate(res);
		return res;
	}

	/************************************************************************************************************************
													Connecting to Perseus
	************************************************************************************************************************/
	printf( "Configuring the Perseus at IP = %s, please wait.\n", IpAddress );
	res = connect_cce(IpAddress, &state);
	if(FAILURE(res))
	{
		printf("Error with connect_cce\n");
		Terminate(res);
		return res;
	}

	/************************************************************************************************************************
													Calling main function
	************************************************************************************************************************/
	printf( "MI125 Init...\n");
	res = MI125_Init(&state, hMi125);
	if(FAILURE(res))
	{
		printf("\nError with MI125_Init\n");
		Terminate(res);
		return res;
	}
	printf( "Done!\n");

	/************************************************************************************************************************
												Closing connection to Perseus
	************************************************************************************************************************/
	res = disconnect_cce(&state);
	if(FAILURE(res))
	{
		printf("Error with disconnect_cce\n");
		Terminate(res);
		return res;
	}

	Terminate(OK);
	return OK;
}


/****************************************************************************************************************************
*										         Static Function Definitions
****************************************************************************************************************************/
static void Terminate(adp_result_t res)
{
	uint32_t i;

	for (i = 0; i < MAX_MI125 ; i++)
	{
		if (hMi125[i] != NULL)
		{
			free(hMi125[i]);
			hMi125[i] = NULL;
		}
	}

	if (res != OK)
	{
		printf("%s\n",ErrorToString(res));
		printf( "\nThe program will terminate. Push any key to continue\n" );	
		GETCH();
	}		
}

static adp_result_t ValidatingInitCall(handle_mi125 * handle)
{
	uint32_t i;

	for (i = 0; i < MAX_MI125 ; i++)
	{
		if(hMi125[i] != NULL) //At least one handle is not null, so need to be initialized
		{
			return OK;
		}
	}

	return CONFIGFILENOFMCTOINIT;
}

static adp_result_t FileParser(const char * Filename, handle_mi125 * handle)
{
	adp_result_t res;

	const char * ccSection[MAX_MI125] = {"FMC1","FMC2","FMC3","FMC4"};
	char type[255];
	uint32_t i;
	uint32_t u32ActiveFlag;	

	uint32_t u32ValueType;

	for ( i = 0 ; i < MAX_MI125 ; i++ )
	{
		//Validate if good type
		res = ConfigFile_GetParamValue(Filename, ccSection[i], "type", "%s", &type, sizeof(type));
		if(FAILURE(res) || _stricmp(type, "mi125")) //Not MI125 type
		{
			continue;
		}
		
		//Validate if active
		res = ConfigFile_GetParamValue(Filename, ccSection[i], "active_flag", "%u", &u32ActiveFlag, 0);
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for active_flag\n");
			Terminate(res);
			return res;
		}
		if (u32ActiveFlag == 0) //Not active
		{
			continue;
		}

		// Malloc handle
		hMi125[i] = (handle_mi125)(malloc(sizeof(sMi125Struct)));
		if (hMi125[i] == NULL)
		{
			return ERR_OUTOFMEMORY;
		}

		// Sampling Clock Source
		res = ConfigFile_GetParamValue(Filename, ccSection[i], "sampling_clock_source", "%u", &u32ValueType, 0);
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for sampling_clock_source\n");
			Terminate(res);
			return res;
		}
		hMi125[i]->eSamplingClockSource = (MI125_mi125_clock_source)u32ValueType;

		// Trigger Output IO
		res = ConfigFile_GetParamValue(Filename, ccSection[i], "trigger_out", "%s", &type, sizeof(type));
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for trigger_out\n");
			Terminate(res);
			return res;
		}

		if(!_stricmp(type, "ON"))
		{
			hMi125[i]->eTrigOut = MI125_TRIGOUTON;			
		}
		else
		{
			hMi125[i]->eTrigOut = MI125_TRIGOUTOFF;
		}	
	}

	return OK;
}