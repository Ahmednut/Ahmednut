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

#include "linux_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "verbose_utils.h"
#include "media_types.h"
#include "bas_error.h"
#include "bas_types.h"
#include "eapi.h"
#include "connection_state.h"
#include "RxStreaming.h"
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
static adp_result_t ValidatingInitCall(handle_rtdex_channel * handle_rxchannel);
static adp_result_t FileParser(const char * Filename, handle_streaming * handle_rxstreaming, handle_rtdex_channel * handle_rxchannel);

handle_streaming hStreamingMain;
handle_rtdex_channel hChannel[MAX_RTDEX] = {NULL};

uint8_t err_lvl = INFO;
uint8_t VERBOSE_CTRL = FALSE;
/****************************************************************************************************************************
															Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
	adp_result_t res;
	uint32_t i;
	uint32_t RTDEx_idx =  0;
	uint32_t n_addi_args =  0;
	/************************************************************************************************************************
												 Initialize eapi before use
	************************************************************************************************************************/
	if(!_stricmp(argv[argc-1], "INFO"))
	{
		err_lvl = INFO;
		VERBOSE_CTRL = TRUE;
	}
	else if(!_stricmp(argv[argc-1], "WARN"))
	{
		err_lvl = WARN;
		VERBOSE_CTRL = TRUE;
	}
	else if(!_stricmp(argv[argc-1], "ERR"))
	{
		err_lvl = ERR;
		VERBOSE_CTRL = TRUE;
	}
	else
	{
		err_lvl = INFO;
		VERBOSE_CTRL = FALSE;
	}
	
	bas_printf(INFO,"\n\n---------- RxStreaming.exe ----------\n\n");

	/************************************************************************************************************************
												Validating number of arguments
	************************************************************************************************************************/
	if (argc < 3)
	{
		bas_printf(ERR, "Error: Wrong number of parameters\n"
               "Parameters 1 must be the Configuration file name, ex RxStreaming.ini,\n"
			   "Parameters 2 must be the Perseus IP address, ex 192.168.0.101,\n"
			   "(Optional) Parameters 3,4,5,... must be the overwritten parameter name followed by its value,\n"
			   "ex RTDEx_transfersize, 65536 \n"
               " The program will terminate. Push any key to continue\n" );
		fflush(stdout);
       GETCH();
       return -1;
	}

	/************************************************************************************************************************
													  Parsing .ini file
	************************************************************************************************************************/
	bas_printf(INFO,"Parsing %s file for needed parameters...",ConfigFileName);
	res = FileParser(ConfigFileName, &hStreamingMain, hChannel);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with FileParser\n");
		Terminate(res);
		return res;
	}
	bas_printf(INFO,"Done!\n\n");

	/************************************************************************************************************************
												Validating if calling main function
	************************************************************************************************************************/	
	res = ValidatingInitCall(hChannel);
	if(FAILURE(res))	
	{		
		bas_printf(ERR, "Error with ValidatingInitCall\n");
		Terminate(res);
		return res;
	}

	/************************************************************************************************************************
												Validating if calling main function
	************************************************************************************************************************/
	if (argc > 3)
	{
			for (i = 0; i < argc ; i = i++)
			{
				if (!_stricmp(argv[i] , "RTDEx_transfersize"))
				{
					hStreamingMain->u32TransferSize = atoi(argv[i+1]);
					i++;
					n_addi_args = n_addi_args + 2;
				}
				else 
				{
					if (!_stricmp(argv[i] , "filename"))
					{
						strcpy(hChannel[RTDEx_idx]->pcFileName, argv[i+1]);
						RTDEx_idx++; 
						i++;
						n_addi_args = n_addi_args + 2;
					}
				}
			}				
	}

	/************************************************************************************************************************
													Calling main function
	************************************************************************************************************************/
	res = RxStreaming(&(argv[2]), argc-2-n_addi_args-VERBOSE_CTRL, hStreamingMain, hChannel);
	if(FAILURE(res))
	{
		bas_printf(ERR,"\nError with RxStreaming\n");
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
		bas_printf(ERR,"%s\n",ErrorToString(res));
		bas_printf(ERR, "\nThe program will terminate. Push any key to continue\n" );
		fflush(stdout);
		GETCH();
	}
	fflush(stdout);
}

static adp_result_t ValidatingInitCall(handle_rtdex_channel * handle_rxchannel)
{
	uint32_t i;

	for (i = 0; i < MAX_RTDEX ; i++)
	{
		if(handle_rxchannel[i] != NULL) //At least one handle is not null, so need to be initialized
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


static adp_result_t FileParser(const char * Filename, handle_streaming * handle_rxstreaming, handle_rtdex_channel * handle_rxchannel)
{
	adp_result_t res;

	char type[255];	
	uint32_t i, c = 0;

	char pcValueType[255];
	uint32_t u32ValueType;

    uint32_t u32NbSections;
	
	char ** Section = (char**)malloc(MAX_RTDEX * sizeof(char*));
	if ( Section == NULL)
	{
		return ERR_OUTOFMEMORY;
	}
    memset(Section,0,MAX_RTDEX * sizeof(char*));

	for ( i = 0 ; i < MAX_RTDEX ; i++ )
	{
		Section[i] = (char*)malloc(255 * sizeof(char));
		if (Section[i] == NULL)
		{
			res = ERR_OUTOFMEMORY;
			goto end;
		}
        memset(Section[i],'\0',255 * sizeof(char));
	}

	// Malloc handle
	*handle_rxstreaming = (handle_streaming)(malloc(sizeof(sRTDExStreamingParams)));

	// RTDEx Framesize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_framesize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with ConfigFile_GetParamValue for RTDEx_framesize\n");
		goto end;
	}
	(*handle_rxstreaming)->u32FrameSize = u32ValueType;

	// RTDEx Queuesize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_queuesize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with ConfigFile_GetParamValue for RTDEx_queuesize\n");
		goto end;
	}
	(*handle_rxstreaming)->u32QueueSize = u32ValueType;

	// RTDEx Transfersize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_transfersize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with ConfigFile_GetParamValue for RTDEx_transfersize\n");
		goto end;
	}
	(*handle_rxstreaming)->u32TransferSize = u32ValueType;

	// RTDEx Burstsize
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_burstsize", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with ConfigFile_GetParamValue for RTDEx_burstsize\n");
		goto end;
	}
	(*handle_rxstreaming)->u32BurstSize = u32ValueType;

    // RTDEx Frame gap
	res = ConfigFile_GetParamValue(Filename, NULL, "RTDEx_framegap", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with ConfigFile_GetParamValue for RTDEx_framegap\n");
		goto end;
	}
	(*handle_rxstreaming)->u32FrameGap = u32ValueType;

	// Real Time Flag
	res = ConfigFile_GetParamValue(Filename, NULL, "real_time_flag", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with ConfigFile_GetParamValue for real_time_flag\n");
		goto end;
	}
	(*handle_rxstreaming)->u8RealTimeFlag = (uint8_t)u32ValueType;

	// Display Stats Flag
	res = ConfigFile_GetParamValue(Filename, NULL, "display_stats_flag", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with ConfigFile_GetParamValue for display_stats_flag\n");
		goto end;
	}
	(*handle_rxstreaming)->u8DisplayStatsFlag = (uint8_t)u32ValueType;
	
	// Get all section names for parameter "type"
	res = ConfigFile_GetSectionNamesFromParameter(Filename,"type",Section, &u32NbSections);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with ConfigFile_GetSectionNamesFromParameter for type\n");
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
		res = ConfigFile_GetParamValue(Filename, Section[i], "type", "%s", type, sizeof(type));
		if(FAILURE(res))
		{
			bas_printf(ERR,"Error with ConfigFile_GetParamValue for type\n");
			goto end;
		}
		if(_stricmp(type, "rtdex")) //Not RTDEx type
		{
			continue;
		}

		// Malloc handle
		handle_rxchannel[c] = (handle_rtdex_channel)(malloc(sizeof(sRTDExChannelStruct)));
		if (handle_rxchannel[c] == NULL)
		{
			res = ERR_OUTOFMEMORY;
			goto end;
		}

		// Carrier Position
		res = ConfigFile_GetParamValue(Filename, Section[i], "carrier_position", "%u", &u32ValueType, 0);
		if(FAILURE(res))
		{
			bas_printf(ERR,"Error with ConfigFile_GetParamValue for carrier_position\n");
			goto end;
		}
		handle_rxchannel[c]->u8CarrierPosition = (uint8_t)u32ValueType;

		// Channel Number
		res = ConfigFile_GetParamValue(Filename, Section[i], "channel_number", "%u", &u32ValueType, 0);
		if(FAILURE(res))
		{
			bas_printf(ERR,"Error with ConfigFile_GetParamValue for channel_number\n");
			goto end;
		}
		handle_rxchannel[c]->u8ChannelNumber = (uint8_t)u32ValueType;

		// Filename
		res = ConfigFile_GetParamValue(Filename, Section[i], "filename", "%s", &pcValueType, sizeof(pcValueType));
		if(FAILURE(res))
		{
			bas_printf(ERR,"Error with ConfigFile_GetParamValue for filename\n");
			goto end;
		}
		memcpy(handle_rxchannel[c]->pcFileName, pcValueType, sizeof(pcValueType));
		c++; //increment handle_rxchannel index

	}

    (*handle_rxstreaming)->u32NumberOfChannels = c;
	fflush(stdout);
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
	fflush(stdout);
	return res;
}
