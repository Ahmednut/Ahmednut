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
#include "verbose_utils.h"
#include "bas_error.h"
#include "bas_types.h"
#include "eapi.h"
#include "connection_state.h"
#include "ConfigFile.h"
#include "CustomRegister_Write.h"

// WARNING: to run this demo under LINUX, you may need root privilege

/****************************************************************************************************************************
*										     Command Line Arguments Access Defines
****************************************************************************************************************************/
#define IpAddress			argv[1]
#define CustomRegisterID    strtoul(argv[2],NULL,0)
#define Value				strtoul(argv[3],NULL,0)

/****************************************************************************************************************************
*										         Static Function Declarations
****************************************************************************************************************************/
static void Terminate(adp_result_t res);

uint8_t err_lvl = INFO;
uint8_t VERBOSE_CTRL = FALSE;
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
	
	bas_printf(INFO,"\n\n---------- CustomRegister_Write -----------\n\n");

	/************************************************************************************************************************
												Validating number of arguments
	************************************************************************************************************************/
	if (argc < 4)
	{
		bas_printf(ERR, "Error: Wrong number of parameters\n"
               "Parameters 1 must be the Perseus IP address, ex 192.168.0.101,\n"
               "Parameters 2 must be the Custom Register ID,\n"
               "Parameters 3 must be the Value,\n"			  
               " The program will terminate. Push any key to continue\n" );
		fflush(stdout);
       GETCH();
       return -1;
	}

	/************************************************************************************************************************
													Connecting to Perseus
	************************************************************************************************************************/
	bas_printf(INFO, "Configuring the Perseus at IP = %s, please wait.\n", IpAddress );
	res = connect_cce(IpAddress, &state);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with connect_cce\n");
		Terminate(res);
		return res;
    }

	/************************************************************************************************************************
													Calling main function
	************************************************************************************************************************/
	bas_printf(INFO, "CustomRegister Write...\n");
	res = CustomRegister_Write(&state, CustomRegisterID, Value);
	if(FAILURE(res))
	{
		bas_printf(ERR,"\nError with CustomRegister_Write\n");
		Terminate(res);
		return res;
	}
	bas_printf(INFO, "Done!\n");

	/************************************************************************************************************************
												Closing connection to Perseus
	************************************************************************************************************************/
	res = disconnect_cce(&state);
	if(FAILURE(res))
	{
		bas_printf(ERR,"Error with disconnect_cce\n");
		Terminate(res);
		return res;
	}
	fflush(stdout);
	return OK;
}

/****************************************************************************************************************************
*										         Static Function Definitions
****************************************************************************************************************************/
static void Terminate(adp_result_t res)
{
	bas_printf(ERR,"%s\n",ErrorToString(res));
	bas_printf(ERR, "\nThe program will terminate. Push any key to continue\n" );	
	fflush(stdout);
	GETCH();	
}
