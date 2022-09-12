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
#include "TimeGate.h"
#include "ConfigFile.h"

// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
*										     Command Line Arguments Access Defines
****************************************************************************************************************************/
#define IpAddress			    argv[1]
#define Mode		            strtoul(argv[2],NULL,0)
#define Time		            argv[3]

/****************************************************************************************************************************
*										         Static Function Declarations
****************************************************************************************************************************/
static void Terminate(adp_result_t res, handle_timestampparams *hParams);
static adp_result_t ValidatingInitCall(handle_timestampparams hParams);
static adp_result_t FileParser(const char * Filename, handle_timestampparams *hParams);





/****************************************************************************************************************************
															Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
	adp_result_t res;
    handle_timestampparams hTimestampParams = NULL;

	/************************************************************************************************************************
												 Initialize eapi before use
	************************************************************************************************************************/
    printf("\n\n---------- TimeGate.exe ----------\n\n");

	/************************************************************************************************************************
												Validating number of arguments
	************************************************************************************************************************/
	if (argc < 4)
	{
		printf( "Error: Wrong number of parameters\n"
			   "Parameters 1 must be the Perseus IP address, ex 192.168.0.101,\n"
			   "Parameter  2 must be the mode, ex 0 for relative and 1 for absolute,\n"
			   "Parameter  3 must be the time or tick number, ex 2016-07-05 15:44:30.0 for absolute mode and 1111555  for relative mode,\n"
               " The program will terminate. Push any key to continue\n" );



       GETCH();
       return -1;
	}
    


	
	/************************************************************************************************************************
												Validating if calling main function
	************************************************************************************************************************/	


	/************************************************************************************************************************
													Calling main function
	************************************************************************************************************************/

	res = TimeGate(IpAddress, hTimestampParams,Time,Mode);
	if(FAILURE(res))
	{
		printf("\nError with TimeGate\n");
		Terminate(res, &hTimestampParams);
		return res;
	}

	Terminate(OK, &hTimestampParams);
	return OK;
}

/****************************************************************************************************************************
*										         Static Function Definitions
****************************************************************************************************************************/
static void Terminate(adp_result_t res, handle_timestampparams *hParams)
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

static adp_result_t ValidatingInitCall(handle_timestampparams hParams)
{
	if(hParams != NULL) //The handle is not null, so need to be initialized
	{
		return OK;
	}
	return CONFIGFILENOFMCTOINIT;
}


static adp_result_t FileParser(const char * Filename, handle_timestampparams *hParams)
{
	adp_result_t res;

	char type[255];	
    int i;
    int32_t i32ValueType;
	char pcValueType[255];
	uint32_t u32ValueType;
	uint64_t u64ValueType;

	const char acSection[] = "Timestamp";

    //Validate if good type
	
    res = ConfigFile_GetParamValue(Filename, acSection, "type", "%s", &type, sizeof(type));
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for type\n");
		goto end;
	}
	if(_stricmp(type, "timestamp")) //Not timestamp type
	{
		return OK;
	}

    // Malloc handle
	(*hParams) = (handle_timestampparams)malloc(sizeof(sTimestampParams));
    if((*hParams) == NULL)
    {
        res = ERR_OUTOFMEMORY;
        goto end;
    }

    (*hParams)->u32ClkFreq = 0;
    (*hParams)->bGetTickTimeOnly = 0;
    (*hParams)->bGetPosixTimeOnly = 0;
    (*hParams)->bGetReadableLocalTimeOnly = 0;
    (*hParams)->bResetOnly = 0;
    (*hParams)->u64Value = 0;
    (*hParams)->i32WaitSec = 0;
    (*hParams)->bAbsoluteTime = 0;
    (*hParams)->bWaitUntilTime = 0;
    (*hParams)->i32NanoSecOffset = 0;
    (*hParams)->bReadCustomRegisterTickTime = 0;
    (*hParams)->bReadCustomRegisterPosixTime = 0;
    (*hParams)->bReadCustomRegisterReadableTime = 0;


	// Timestamp module clock frequency
	res = ConfigFile_GetParamValue(Filename, acSection, "clock_frequency", "%u", &u32ValueType, 0);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for clock_frequency\n");
		goto end;
	}
    (*hParams)->u32ClkFreq = u32ValueType;

	// Only print device tick time flag
	res = ConfigFile_GetParamValue(Filename, acSection, "action", "%u", &u32ValueType, 0);

    printf("Action: %d\n\n", u32ValueType);
	if(FAILURE(res))
	{
		printf("Error with ConfigFile_GetParamValue for action\n");
		goto end;
	}

    switch(u32ValueType)
    {
    case 0:
        (*hParams)->bGetTickTimeOnly = TRUE;
        break;
    case 1:
        (*hParams)->bGetPosixTimeOnly = TRUE;
        break;
    case 2:
        (*hParams)->bGetReadableLocalTimeOnly = TRUE;
        break;
    case 3:
        (*hParams)->bResetOnly = TRUE;
        break;
    case 4:
        //Initial value
        res = ConfigFile_GetParamValue(Filename, acSection, "initial_value", "%"PRI_U64, &u64ValueType, 0);
	    if(FAILURE(res))
	    {
		    printf("Error with ConfigFile_GetParamValue for initial_value\n");
		    goto end;
	    }
        (*hParams)->u64Value = u64ValueType;

        // Trigger timout
	    res = ConfigFile_GetParamValue(Filename, acSection, "trigger_timeout", "%u", &i32ValueType, 0);
	    if(FAILURE(res))
	    {
		    printf("Error with ConfigFile_GetParamValue for trigger_timeout\n");
		    goto end;
	    }
        (*hParams)->i32WaitSec = i32ValueType;
        break;
    case 5:
        (*hParams)->bAbsoluteTime = TRUE;
        break;
    case 6:
        (*hParams)->bWaitUntilTime = TRUE;
        res = ConfigFile_GetParamValue(Filename, acSection, "wait_until_time", "%s", &pcValueType, 40);
	    if(FAILURE(res))
	    {
		    printf("Error with ConfigFile_GetParamValue for wait_until_time\n");
		    goto end;
	    }

        strcpy((*hParams)->acTime, pcValueType);
    
        // Replace '_' by a space character
        for(i=0; i<strlen((*hParams)->acTime); i++)
        {
            if(*((*hParams)->acTime + i) == '_')
            {
                *((*hParams)->acTime + i) = ' ';
            }
        }

        // Nanosecond offset
	    res = ConfigFile_GetParamValue(Filename, acSection, "nanosec_offset", "%d", &i32ValueType, 0);
	    if(FAILURE(res))
	    {
		    printf("Error with ConfigFile_GetParamValue for nanosec_offset\n");
		    goto end;
	    }

        (*hParams)->i32NanoSecOffset = (int)i32ValueType;
        break;
    case 7:
    case 8:
    case 9:
        if(u32ValueType == 7)
            (*hParams)->bReadCustomRegisterTickTime = TRUE;
        else if(u32ValueType == 8)
            (*hParams)->bReadCustomRegisterPosixTime = TRUE;
        else
            (*hParams)->bReadCustomRegisterReadableTime = TRUE;

        // Parsing least significant word custom register id
        res = ConfigFile_GetParamValue(Filename, acSection, "lsw_custom_register_id", "%d", &u32ValueType, 0);
	    if(FAILURE(res))
	    {
		    printf("Error with ConfigFile_GetParamValue for lsw_custom_register_id\n");
		    goto end;
	    }
        (*hParams)->u32LswCustomRegisterId = u32ValueType;

        // Parsing most significant word custom register id
        res = ConfigFile_GetParamValue(Filename, acSection, "msw_custom_register_id", "%d", &u32ValueType, 0);
	    if(FAILURE(res))
	    {
		    printf("Error with ConfigFile_GetParamValue for msw_custom_register_id\n");
		    goto end;
	    }
        (*hParams)->u32MswCustomRegisterId = u32ValueType;

        break;
    default:
        printf("Error with action parameter. Value must be in the range 0 - 9.\n");
        res = CONFIGFILEWRONGVALUE;
        goto end;
        break;
    }

    res = OK;
end:
	return res;
}