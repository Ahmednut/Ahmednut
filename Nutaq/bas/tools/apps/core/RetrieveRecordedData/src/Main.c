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
#include <string.h>

#include "linux_utils.h"
#include "media_types.h"
#include "bas_error.h"
#include "eapi.h"
#include "connection_state.h"
#include "ConfigFile.h"
#include "media_utils.h" 
#include "eapi_carrier.h"
#include "recplay.h"
#include "RetrieveRecordedData.h"

// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
*                                             Command Line Arguments Access Defines
****************************************************************************************************************************/
#define IpAddress               argv[1]
#define Channel                 strtoul(argv[2],NULL,0)
#define FrameSize               strtoul(argv[3],NULL,0)
#define RecordSize              strtoul(argv[4],NULL,0)
#define StartAddress            argv[5]
#define RecordTimeout		    strtoul(argv[6],NULL,0)
#define FileName                argv[7]

/****************************************************************************************************************************
*                                                 Static Function Declarations
****************************************************************************************************************************/
static void Terminate(adp_result_t res);

/****************************************************************************************************************************
                                                            Main
****************************************************************************************************************************/
int main( int argc, char* argv[] )
{
    adp_result_t res;
    connection_state state;

    printf("\n\n---------- RetrieveRecordedData ----------\n\n");

    /************************************************************************************************************************
                                                Validating number of arguments
    ************************************************************************************************************************/
    if (argc != 8)
    {
        printf( "Error: Wrong number of parameters\n"
               "Usage: RetrieveRecordedData <ip address> (0 | 1 | 2 | 3 | 4 | 5 | 6 | 7) <frame size> <record size> <start address> <record timeout> <file name>\n"
               "Parameter 1 must be the Perseus IP address, ex 192.168.0.101,\n"
               "Parameter 2 must be the RTDEx channel,\n"
               "Parameter 3 must be the Frame size in bytes,\n"
               "Parameter 4 must be the Record size in bytes,\n"
               "Parameter 5 must be the Start Address in bytes or \"trigger\",\n"
			   "Parameter 6 must be the Record timeout in milliseconds,\n"
               "Parameter 7 must be the File name, ex sinewave.bin,\n"
               " The program will terminate. Push any key to continue\n" );
       GETCH();
       return -1;
    }

    /************************************************************************************************************************
                                               Display current parameters to set
    ************************************************************************************************************************/
	printf("\t - RTDEx channel : %"PRI_U64"\n",Channel);
	printf("\t - Frame size : %"PRI_U64" bytes\n",FrameSize);
	printf("\t - Record size : %"PRI_U64" bytes\n",RecordSize);
    if(_stricmp(StartAddress,"trigger") == 0)
    {
            printf("\t - Start address : %s\n",StartAddress);
    }
    else
    {
            printf("\t - Start address : %"PRI_U64" bytes\n",strtoul(StartAddress,NULL,0));
    }
	printf("\t - Record timeout : %"PRI_U64" ms\n",RecordTimeout);
	printf("\t - Filename : %s\n",FileName);
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
    if(_stricmp(StartAddress,"trigger") == 0)
    {
        res = RetrieveRecordedDataFromTriggerToFile(&state, 
            Channel, FrameSize,
            RecordSize, RecordTimeout, FileName);
    }
    else
    {
        res = RetrieveRecordedDataToFile(&state, 
            Channel, FrameSize, 
            RecordSize, strtoul(StartAddress,NULL,0), RecordTimeout, FileName);
    }
    if(FAILURE(res))
    {
        printf("\nError retrieving recorded data to file\n");
        Terminate(res);
    }

    /************************************************************************************************************************
                                                Closing connection to Perseus
    *************************************************************************************************************************/

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
*                                                 Static Function Definitions
****************************************************************************************************************************/
static void Terminate(adp_result_t res)
{
    printf("%s\n",ErrorToString(res));
    printf( "\nThe program will terminate. Push any key to continue\n" );    
    GETCH();    
}