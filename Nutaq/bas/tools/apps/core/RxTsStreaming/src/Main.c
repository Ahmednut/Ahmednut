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
#include "RxTsStreaming.h"
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
static adp_result_t ValidatingInitCall(handle_rtdexts_channel * handle_rxchannel);
static adp_result_t FileParser(const char * Filename, handle_streaming * handle_rxstreaming, handle_rtdexts_channel * handle_rxchannel);

handle_streaming hStreaming;
handle_rtdexts_channel hChannel[MAX_RTDEXTS] = {NULL};

/****************************************************************************************************************************
															Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
	adp_result_t res;

	/************************************************************************************************************************
												 Initialize eapi before use
	************************************************************************************************************************/
    printf("\n\n---------- RxTsStreaming.exe ----------\n\n");

	/************************************************************************************************************************
												Validating number of arguments
	************************************************************************************************************************/
	if (argc < 3)
	{
		printf( "Error: Wrong number of parameters\n"
               "Parameters 1 must be the Configuration file name, ex RxTsStreaming.ini,\n"
			   "Parameters 2 must be the Perseus IP address, ex 192.168.0.101,\n"
               " The program will terminate. Push any key to continue\n" );
       GETCH();
       return -1;
	}

	/************************************************************************************************************************
													  Parsing .ini file
	************************************************************************************************************************/
	printf("Parsing %s file for needed parameters...",ConfigFileName);
	res = FileParser(ConfigFileName, &hStreaming, hChannel);
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
	res = RxTsStreaming(&(argv[2]), argc-2, hStreaming, hChannel);
	if(FAILURE(res))
	{
		printf("\nError with RxTsStreaming\n");
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

	for (i = 0; i < MAX_RTDEXTS ; i++)
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

static adp_result_t ValidatingInitCall(handle_rtdexts_channel * handle_rxchannel)
{
	uint32_t i;

	for (i = 0; i < MAX_RTDEXTS ; i++)
	{
		if(handle_rxchannel[i] != NULL) //At least one handle is not null, so need to be initialized
		{
			return OK;
		}
	}

	if (hStreaming != NULL)
	{
		free(hStreaming);
	}

	return CONFIGFILENOFMCTOINIT;
}


static adp_result_t FileParser(const char * Filename, handle_streaming * handle_rxstreaming, handle_rtdexts_channel * handle_rxchannel)
{
	adp_result_t res;

	char type[255];	
	uint32_t i, c = 0, k = 0;
    
    int32_t i32ValueType;
	char pcValueType[255];
	uint32_t u32ValueType;

    uint32_t u32NbSections;
	
	char ** Section = (char **)malloc(MAX_RTDEXTS * sizeof(char*));
	if ( Section == NULL)
	{
		return ERR_OUTOFMEMORY;
	}
    memset(Section,0,MAX_RTDEXTS * sizeof(char*));

	for ( i = 0 ; i < MAX_RTDEXTS ; i++ )
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
	*handle_rxstreaming = (handle_streaming)(malloc(sizeof(sRTDExTsStreamingParams)));

	// RTDExTs Subframesize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_subframesize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_subframesize\n");
		goto end;
	}
	(*handle_rxstreaming)->u32SubFrameSize = u32ValueType;

	// RTDExTs Framesize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_framesize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_framesize\n");
		goto end;
	}
	(*handle_rxstreaming)->u32FrameSize = u32ValueType;

	// RTDExTs Queuesize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_queuesize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_queuesize\n");
		goto end;
	}
	(*handle_rxstreaming)->u32QueueSize = u32ValueType;

	// RTDExTs Transfersize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_numberofsamples", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_numberofsamples\n");
		goto end;
	}
	(*handle_rxstreaming)->u32NumOfSamples = u32ValueType;

	// Real Time Flag
	res = ConfigFile_GetParamValue(Filename, NULL, "real_time_flag", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for real_time_flag\n");
		goto end;
	}
	(*handle_rxstreaming)->u8RealTimeFlag = (uint8_t)u32ValueType;

    // Timestamp module clock frequency
	res = ConfigFile_GetParamValue(Filename, NULL, "timestamp_freq", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for timestamp_freq\n");
		goto end;
	}
	(*handle_rxstreaming)->u32TsClkFreq = u32ValueType;

    // Timestamp initialization flag
	res = ConfigFile_GetParamValue(Filename, NULL, "init_timestamp", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for init_timestamp flag\n");
		goto end;
	}
	(*handle_rxstreaming)->bTsInit = (uint8_t)u32ValueType;

    // Timestamp initialization flag
	res = ConfigFile_GetParamValue(Filename, NULL, "propagation_delay", "%u", &i32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for propagation_delay flag\n");
		goto end;
	}
	(*handle_rxstreaming)->i32PropDelayNs = i32ValueType;

	// Start mode
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_startmode", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_startmode\n");
		goto end;
	}
	(*handle_rxstreaming)->start.mode = (RTDExTs_Mode_t)u32ValueType;

	// Stop mode
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_stopmode", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_stopmode\n");
		goto end;
	}
	(*handle_rxstreaming)->stop.mode = (RTDExTs_Mode_t)u32ValueType;

        // Start mode
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_starttrigger", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_starttrigger\n");
		goto end;
	}
    (*handle_rxstreaming)->start.trigSel = (RTDExTs_TrigSel_t)u32ValueType;

	// Stop mode
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_stoptrigger", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_stoptrigger\n");
		goto end;
	}
	(*handle_rxstreaming)->stop.trigSel = (RTDExTs_TrigSel_t)u32ValueType;

	// Start time
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_starttime", "%s", &pcValueType, 29);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_starttime\n");
		goto end;
	}

	strcpy((*handle_rxstreaming)->start.pcTime, pcValueType);
    // Replace '_' by a space character
    for(k=0; k<strlen((*handle_rxstreaming)->start.pcTime); k++)
    {
        if(*((*handle_rxstreaming)->start.pcTime + k) == '_')
        {
            *((*handle_rxstreaming)->start.pcTime + k) = ' ';
        }
    }

	// Stop time
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDExTs_stoptime", "%s", &pcValueType, 29);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for RTDExTs_stoptime\n");
		goto end;
	}
    strcpy((*handle_rxstreaming)->stop.pcTime, pcValueType);
    // Replace '_' by a space character
    for(k=0; k<strlen((*handle_rxstreaming)->stop.pcTime); k++)
    {
        if(*((*handle_rxstreaming)->stop.pcTime + k) == '_')
        {
            *((*handle_rxstreaming)->stop.pcTime + k) = ' ';
        }
    }

	// Get all section names for parameter "type"
	res = ConfigFile_GetSectionNamesFromParameter(Filename,"type",Section, &u32NbSections);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetSectionNamesFromParameter for type\n");
		goto end;
	}
	
	for ( i = 0 ; (i < u32NbSections && i < MAX_RTDEXTS) ; i++ )
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
		if(_stricmp(type, "rtdexts")) //Not RTDExTs type
		{
			continue;
		}

		// Malloc handle
		handle_rxchannel[c] = (handle_rtdexts_channel)(malloc(sizeof(sRTDExTsChannelStruct)));
		if (handle_rxchannel[c] == NULL)
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
		handle_rxchannel[c]->u8CarrierPosition = (uint8_t)u32ValueType;

		// Channel Number
		res = ConfigFile_GetParamValue(Filename, Section[i], "channel_number", "%u", &u32ValueType, 0);
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for channel_number\n");
			goto end;
		}
		handle_rxchannel[c]->u8ChannelNumber = (uint8_t)u32ValueType;

		// Filename
		res = ConfigFile_GetParamValue(Filename, Section[i], "filename", "%s", &pcValueType, sizeof(pcValueType));
		if(FAILURE(res))
		{
			printf("Error with ConfigFile_GetParamValue for filename\n");
			goto end;
		}
		memcpy(handle_rxchannel[c]->pcFileName, pcValueType, sizeof(pcValueType));
		c++; //increment handle_rxchannel index
	}

    (*handle_rxstreaming)->u32NumberOfChannels = c;

	res = OK;

end:
	
	for ( i = 0 ; i < MAX_RTDEXTS ; i++ )
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