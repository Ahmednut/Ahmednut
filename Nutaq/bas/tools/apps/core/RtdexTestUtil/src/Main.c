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
	#include <strings.h>
#else
	#include <conio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "media_types.h"
#include "bas_error.h"
#include "bas_types.h"
#include "eapi.h"
#include "connection_state.h"
#include "RtdexTestUtil.h"
#include "ConfigFile.h"

// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
*										     Command Line Arguments Access Defines
****************************************************************************************************************************/
#define IpAddress			    argv[1]
#define ConfigFileName			argv[2]
#define Action       			argv[3]

/****************************************************************************************************************************
*										         Static Function Declarations
****************************************************************************************************************************/
static void Terminate(adp_result_t res, handle_rtdextestparams *hParams);
static adp_result_t ValidatingInitCall(handle_rtdextestparams hParams);
static adp_result_t FileParser(const char * Filename, handle_rtdextestparams *hParams);

/****************************************************************************************************************************
															Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
	adp_result_t res;
    handle_rtdextestparams hRtdexParams = NULL;
	/************************************************************************************************************************
												 Initialize eapi before use
	************************************************************************************************************************/
    printf("\n\n---------- RtdexTestUtil ----------\n\n");

	/************************************************************************************************************************
												Validating number of arguments
	************************************************************************************************************************/
	if (argc < 3)
	{
		printf( "Error: Wrong number of parameters\n"
               "Parameter 1 must be the Configuration file name, ex RtdexTestUtil.ini,\n"
			   "Parameter 2 must be the Perseus IP address, ex 192.168.0.101,\n"
               "Parameter 3 must be the action to execute. Possible actions are 'init', 'enable' and 'stop'.\n"
               " The program will terminate. Push any key to continue\n" );
       GETCH();
       return -1;
	}

	/************************************************************************************************************************
													  Parsing .ini file
	************************************************************************************************************************/
	printf("Parsing %s file for needed parameters...",ConfigFileName);
	res = FileParser(ConfigFileName, &hRtdexParams);
	if(FAILURE(res))
	{
		printf("Error with FileParser\n");
		Terminate(res, &hRtdexParams);
		return res;
	}
	printf("Done!\n\n");
	/************************************************************************************************************************
												Validating if calling main function
	************************************************************************************************************************/	
	res = ValidatingInitCall(hRtdexParams);
	if(FAILURE(res))	
	{		
		printf("Error with ValidatingInitCall\n");
		Terminate(res, &hRtdexParams);
		return res;
	}

	/************************************************************************************************************************
													Calling main function
	************************************************************************************************************************/
	res = RtdexTestUtil(IpAddress, hRtdexParams, Action);
	if(FAILURE(res))
	{
		printf("\nError with RtdexTestUtil\n");
		Terminate(res, &hRtdexParams);
		return res;
	}

	Terminate(OK, &hRtdexParams);
	return OK;
}

/****************************************************************************************************************************
*										         Static Function Definitions
****************************************************************************************************************************/
static void Terminate(adp_result_t res, handle_rtdextestparams *hParams)
{
    if((*hParams) != NULL)
    {
        free((*hParams));
        (*hParams) = NULL;
    }

	if (res != OK)
	{
		printf("%s\n",ErrorToString(res));
		printf( "\nThe program will terminate. Push any key to continue\n" );	
		GETCH();
	}
}

static adp_result_t ValidatingInitCall(handle_rtdextestparams hParams)
{
	if(hParams != NULL) //The handle is not null, so need to be initialized
	{
		return OK;
	}
	return CONFIGFILENOFMCTOINIT;
}


static adp_result_t FileParser(const char * Filename, handle_rtdextestparams *hParams)
{
	adp_result_t res;

	char type[255];	
	uint32_t u32ValueType;
    char pcValueType[500];

	const char acSection[] = "RtdexTest";

    //Validate if right type
	
    res = ConfigFile_GetParamValue(Filename, acSection, "type", "%s", &type, sizeof(type));
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for type\n");
		goto end;
	}
	if(_stricmp(type, "rtdextest")) //Not rtdextest type
	{
		return OK;
	}

    // Malloc handle
	(*hParams) = (handle_rtdextestparams)malloc(sizeof(sRtdexTestParams));
    if((*hParams) == NULL)
    {
        res = ERR_OUTOFMEMORY;
        goto end;
    }

    (*hParams)->bAdcDacMode = 0;          
    (*hParams)->bSingleTransfer = 0;

    (*hParams)->u32RtdexTestBaseCustomRegister = 0;
    (*hParams)->u32TestRate = 0;
    (*hParams)->u32TransferSize = 0;
    (*hParams)->u32Direction = 0;

    res = ConfigFile_GetParamValue(Filename, acSection, "adc_dac_mode", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for adc_dac_mode\n");
		goto end;
	}
    (*hParams)->bAdcDacMode = u32ValueType;

    res = ConfigFile_GetParamValue(Filename, acSection, "rtdex_test_base_custom_register", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for rtdex_test_base_custom_register\n");
		goto end;
	}
    (*hParams)->u32RtdexTestBaseCustomRegister = u32ValueType;

    res = ConfigFile_GetParamValue(Filename, acSection, "test_rate", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for test_rate\n");
		goto end;
	}
    (*hParams)->u32TestRate = u32ValueType;

    res = ConfigFile_GetParamValue(Filename, acSection, "single_transfer", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for single_transfer\n");
		goto end;
	}
    (*hParams)->bSingleTransfer = u32ValueType;

    res = ConfigFile_GetParamValue(Filename, acSection, "transfer_size", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for transfer_size\n");
		goto end;
	}
    (*hParams)->u32TransferSize = u32ValueType;

    res = ConfigFile_GetParamValue(Filename, acSection, "filenames", "%s", &pcValueType, sizeof(pcValueType));
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for filename\n");
		goto end;
	}
    memcpy((*hParams)->pcFileNames, pcValueType, sizeof(pcValueType));

    res = ConfigFile_GetParamValue(Filename, acSection, "directions_to_enable", "%s", &pcValueType, sizeof(pcValueType));
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for directions_to_enable\n");
		goto end;
	}

    if(!_stricmp(pcValueType, "uplink"))
        (*hParams)->u32Direction = UPLINK;
    else if(!_stricmp(pcValueType, "downlink"))
        (*hParams)->u32Direction = DOWNLINK;
    else if(!_stricmp(pcValueType, "both"))
        (*hParams)->u32Direction = BOTH;
    else
    {
        printf("Error with ConfigFile_GetParamValue for directions_to_enable, invalid choice.\n");
        goto end;
    }
    
    res = ConfigFile_GetParamValue(Filename, acSection, "number_of_channels_to_analyze", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for channel_number_to_analyze\n");
		goto end;
	}
    (*hParams)->u32NumberOfChannelsToAnalyze = u32ValueType;

    res = OK;
end:
	return res;
}