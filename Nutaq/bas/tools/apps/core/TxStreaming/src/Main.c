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
#include "TxStreaming.h"
#include "ConfigFile.h"

// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
*										     Command Line Arguments Access Defines
****************************************************************************************************************************/
#define ConfigFileName			argv[1]

/****************************************************************************************************************************
*										         Static Function Declarations
****************************************************************************************************************************/
static void Terminate(adp_result_t res);
static adp_result_t ValidatingInitCall(handle_rtdex_channel * handle_txchannel);
static adp_result_t FileParser(const char * Filename, handle_streaming * handle_txstreaming, handle_rtdex_channel * handle_txchannel);

handle_streaming hStreamingMain;
handle_rtdex_channel hChannel[MAX_RTDEX] = {NULL};

/****************************************************************************************************************************
															Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
	adp_result_t res;

	/************************************************************************************************************************
												 Initialize eapi before use
	************************************************************************************************************************/
    printf("\n\n---------- TxStreaming.exe ----------\n\n");

	/************************************************************************************************************************
												Validating number of arguments
	************************************************************************************************************************/
	if (argc < 3)
	{
		printf( "Error: Wrong number of parameters\n"
               "Parameters 1 must be the Configuration file name, ex TxStreaming.ini,\n"
			   "Parameters 2 must be the Perseus IP address, ex 192.168.0.101,\n"
               " The program will terminate. Push any key to continue\n" );
       GETCH();
       return -1;
	}

	/************************************************************************************************************************
													  Parsing .ini file
	************************************************************************************************************************/
	printf("Parsing %s file for needed parameters...",ConfigFileName);
	res = FileParser(ConfigFileName, &hStreamingMain, hChannel);
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
	res = ValidatingInitCall(hChannel);
	if(FAILURE(res))	
	{		
		printf("Error with ValidatingInitCall\n");
		Terminate(res);
		return res;
	}

	/************************************************************************************************************************
													Calling main function
	************************************************************************************************************************/
	res = TxStreaming(&(argv[2]), argc-2, hStreamingMain, hChannel);
	if(FAILURE(res))
	{
		printf("\nError with TxStreaming\n");
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

	for (i = 0; i < MAX_RTDEX ; i++)
	{
		if (hChannel[i] != NULL)
		{
			free(hChannel[i]);
            hChannel[i] = NULL;
		}
	}

	if (res != OK)
	{
		printf("%s\n",ErrorToString(res));
		printf( "\nThe program will terminate. Push any key to continue\n" );	
		GETCH();
	}	
}

static adp_result_t ValidatingInitCall(handle_rtdex_channel * handle_txchannel)
{
	uint32_t i;

	for (i = 0; i < MAX_RTDEX ; i++)
	{
		if(handle_txchannel[i] != NULL) //At least one handle is not null, so need to be initialized
		{
			return OK;
		}
	}

	if (hStreamingMain != NULL)
	{
		free(hStreamingMain);
	}

	return CONFIGFILENOFMCTOINIT;
}


static adp_result_t FileParser(const char * Filename, handle_streaming * handle_txstreaming, handle_rtdex_channel * handle_txchannel)
{
	adp_result_t res;

	char type[255];	
	uint32_t i, c = 0;

	char pcValueType[255];
	uint32_t u32ValueType;

    uint32_t u32NbSections;
	
	char ** Section = (char **)malloc(MAX_RTDEX * sizeof(char*));
	if ( Section == NULL)
	{
		return ERR_OUTOFMEMORY;
	}
    memset(Section,0,MAX_RTDEX * sizeof(char*));

	for ( i = 0 ; i < MAX_RTDEX ; i++ )
	{
		Section[i] = (char *)malloc(255 * sizeof(char));
		if (Section[i] == NULL)
		{
			res = ERR_OUTOFMEMORY;
			goto end;
		}
        memset(Section[i],'\0',255 * sizeof(char));
	}

	// Malloc handle
	*handle_txstreaming = (handle_streaming)(malloc(sizeof(sRTDExStreamingParams)));

	// RTDEx Framesize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_framesize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDEx_framesize\n");
		goto end;
	}
	(*handle_txstreaming)->u32FrameSize = u32ValueType;

	// RTDEx Queuesize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_queuesize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDEx_queuesize\n");
		goto end;
	}
	(*handle_txstreaming)->u32QueueSize = u32ValueType;

	// RTDEx Transfersize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_transfersize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDEx_transfersize\n");
		goto end;
	}
	(*handle_txstreaming)->u32TransferSize = u32ValueType;

	// RTDEx Burstsize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_burstsize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDEx_burstsize\n");
		goto end;
	}
	(*handle_txstreaming)->u32BurstSize = u32ValueType;

	// Real Time Flag
	res = ConfigFile_GetParamValue(Filename, NULL, "real_time_flag", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for real_time_flag\n");
		goto end;
	}
	(*handle_txstreaming)->u8RealTimeFlag = (uint8_t)u32ValueType;

	// Display Stats Flag
	res = ConfigFile_GetParamValue(Filename, NULL, "display_stats_flag", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for display_stats_flag\n");
		goto end;
	}
	(*handle_txstreaming)->u8DisplayStatsFlag = (uint8_t)u32ValueType;
	
	// Get all section names for parameter "type"
	res = ConfigFile_GetSectionNamesFromParameter(Filename,"type",Section, &u32NbSections);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetSectionNamesFromParameter for type\n");
		goto end;
	}
	
	for ( i = 0 ; (i < u32NbSections && i < MAX_RTDEX) ; i++ )
	{
		// Make sure we use a valid section
		if (strlen(Section[i]) == 0)
		{
			break;
		}

		//Validate if good type
		res = ConfigFile_GetParamValue(Filename, Section[i], "type", "%s", &type, sizeof(type));
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for type\n");
			goto end;
		}
		if(_stricmp(type, "rtdex")) //Not RTDEx type
		{
			continue;
		}

		// Malloc handle
		handle_txchannel[c] = (handle_rtdex_channel)(malloc(sizeof(sRTDExChannelStruct)));
		if (handle_txchannel[c] == NULL)
		{
			res = ERR_OUTOFMEMORY;
			goto end;
		}

		// Carrier Position
		res = ConfigFile_GetParamValue(Filename, Section[i], "carrier_position", "%u", &u32ValueType, 0);
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for carrier_position\n");
			goto end;
		}
		handle_txchannel[c]->u8CarrierPosition = (uint8_t)u32ValueType;

		// Channel Number
		res = ConfigFile_GetParamValue(Filename, Section[i], "channel_number", "%u", &u32ValueType, 0);
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for channel_number\n");
			goto end;
		}
		handle_txchannel[c]->u8ChannelNumber = (uint8_t)u32ValueType;

		// Filename
		res = ConfigFile_GetParamValue(Filename, Section[i], "filename", "%s", &pcValueType, sizeof(pcValueType));
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for filename\n");
			goto end;
		}
		memcpy(handle_txchannel[c]->pcFileName, pcValueType, sizeof(pcValueType));
		c++; //increment handle_txchannel index
	}

    (*handle_txstreaming)->u32NumberOfChannels = c;

	res = OK;

end:
	
	for ( i = 0 ; i < MAX_RTDEX ; i++ )
	{
		if(Section[i] != NULL)
		{
			free(Section[i]);
		}
	}
	if(Section != NULL)
	{
		free(Section);
	}
	return res;
}
