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
#include <stdio.h>
#include <stdlib.h>

#include "linux_utils.h"
#include "bas_error.h"
#include "eapi.h"
#include "connection_state.h"
#include "ConfigFile.h"
#include "RecordData.h"

// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
*										     Command Line Arguments Access Defines
****************************************************************************************************************************/
#define IpAddress			argv[1]
#define TriggerSource		strtoul(argv[2],NULL,0)
#define RecordSize			strtoul(argv[3],NULL,0)
#define StartAddress		strtoul(argv[4],NULL,0)
#define TriggerDelay		strtoul(argv[5],NULL,0)
                                           
/****************************************************************************************************************************
*										         Static Function Declarations
****************************************************************************************************************************/
static void Terminate(Result res);

/****************************************************************************************************************************
															Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
	Result res;
	connection_state state;

    printf("\n\n---------- RecordData ----------\n\n");

	/************************************************************************************************************************
												Validating number of arguments
	************************************************************************************************************************/
	if (argc != 6)
	{
		printf( "Error: Wrong number of parameters\n"
               "Usage: RecordData <ip address> (0 | 1 | 2) <record size> <start address> <trigger delay>\n"
               "Parameter 1 must be the Perseus IP address, ex 192.168.0.101,\n"
               "Parameter 2 must be the Trigger source; External = 0, Software = 1, No Trigger = 2\n"
               "Parameter 3 must be the Record size in bytes,\n"
			   "Parameter 4 must be the Start address in bytes,\n"
			   "Parameter 5 must be the Trigger delay in bytes,\n"
               " The program will terminate. Push any key to continue\n" );
       GETCH();
       return -1;
	}
    
	/************************************************************************************************************************
											Display current parameters to set
	************************************************************************************************************************/
	printf("\t - Trigger source : %"PRI_U64"\n",TriggerSource);
	printf("\t - Record size : %"PRI_U64" bytes\n",RecordSize);	
	printf("\t - Start address : %"PRI_U64" bytes\n",StartAddress);
	printf("\t - Trigger delay : %"PRI_U64" bytes\n",TriggerDelay);
	printf("\n");

	/************************************************************************************************************************
													Connecting to Perseus
	************************************************************************************************************************/
	printf( "Connecting to the platform at IP address : %s ...\n", IpAddress );
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
	res = RecordData(&state, (eRecPlayTrigSrc)TriggerSource, RecordSize, StartAddress, TriggerDelay);
	if(FAILURE(res))
	{
		printf("\nError with RecordData\n");
		Terminate(res);
		return res;
	}

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
static void Terminate(Result res)
{
	printf("%s",ErrorToString(res));
	printf( "\nThe program will terminate. Push any key to continue\n" );	
	GETCH();	
}
