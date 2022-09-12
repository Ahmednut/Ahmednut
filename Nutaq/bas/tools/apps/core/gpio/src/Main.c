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
 * This file is part of Nutaq�s BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq�s BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq�s BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq�s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq�s BAS Software Suite under the GPL requires that your work based on
 * Nutaq�s BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq�s BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq�s BAS Software Suite.
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
#include "bas_error.h"
#include "eapi.h"
#include "connection_state.h"
#include "ConfigFile.h"
#include "Gpio.h"
#include "string.h"
// WARNING: to run this demo under LINUX, you may need root privilege


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

    /************************************************************************************************************************
                                                Validating number of arguments
    ************************************************************************************************************************/
    printf("\n\n---------- GPIO -----------\n\n");

    if (argc < 3)
    {
        printf( "Error: Wrong number of parameters.\nPress any key to quit...");
        GETCH();
        return -1;
    }

    if(strcmp(GpioCommandFromCLI, "init") == 0 || strcmp(GpioCommandFromCLI, "write") == 0)
        if (argc != 4)
            {
                printf( "Error: Wrong number of parameters.\nPress any key to quit...");
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
                                                    GPIO
    ************************************************************************************************************************/
    gpio(&state, argv);

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
*                                                 Static Function Definitions
****************************************************************************************************************************/
static void Terminate(adp_result_t res)
{
    printf("%s\n",ErrorToString(res));
    printf( "\nThe program will terminate. Push any key to continue\n" );    
    GETCH();    
}