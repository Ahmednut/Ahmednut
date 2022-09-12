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
#else
	#include <conio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bas_error.h"
#include "eapi.h"
#include "connection_state.h"
#include "ConfigFile.h"
#include "DACMux_Setup.h"


// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
*										     Command Line Arguments Access Defines
****************************************************************************************************************************/
#define IpAddress			argv[1]
#define DacDataSource		argv[2]

/****************************************************************************************************************************
*										         Static Function Declarations
****************************************************************************************************************************/
static void Terminate(adp_result_t res);

/****************************************************************************************************************************
															Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
	adp_result_t res;
	connection_state state;
    uint32_t u32DacDataSource;

	/************************************************************************************************************************
												 Initialize eapi before use
	************************************************************************************************************************/
    printf("\n\n---------- DACMux_Setup.exe -----------\n\n");

	/************************************************************************************************************************
												Validating arguments
	************************************************************************************************************************/
	if (argc != 3)
	{
		printf( "Error: Wrong number of parameters\n"
               "Parameters 1 must be the Perseus IP address, ex 192.168.0.101\n"
               "Parameters 2 must be the DAC data source specifier.\n" );

       GETCH();
       return -1;
	}

    if(!_stricmp(DacDataSource, "dds"))
        u32DacDataSource = DdsInput;
    else if(!_stricmp(DacDataSource, "playback"))
        u32DacDataSource = PlaybackInput;
    else if(!_stricmp(DacDataSource, "rtdex"))
        u32DacDataSource = RtdexInput;
    else if(!_stricmp(DacDataSource, "adc"))
        u32DacDataSource = AdcInput;
    else
    {
        printf("Wrong specifier for parameter 2.\n"
                "Possible specifiers are:\n"
                "- 'dds'\n"
                "- 'playback'\n"
                "- 'rtdex'\n"
                "- 'adc'\n");
        GETCH();
        return -1;
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

	printf( "Configuring DAC multiplexer...\n");
	res = DACMux_Setup(&state, u32DacDataSource);
	if(FAILURE(res))
	{
		printf("\nError with DACMux_Setup\n");
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

	return OK;
}

/****************************************************************************************************************************
*										         Static Function Definitions
****************************************************************************************************************************/
static void Terminate(adp_result_t res)
{
	printf("%s\n",ErrorToString(res));
	printf( "\nThe program will terminate. Push any key to continue\n" );	
	GETCH();	
}
