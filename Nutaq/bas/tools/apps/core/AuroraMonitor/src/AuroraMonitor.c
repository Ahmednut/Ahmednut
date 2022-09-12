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
 * @file      AuroraMonitor.c
 * 
 * @brief     Function that monitors Aurora transfer
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
#ifdef __linux__
	#include "linux_utils.h"
#else
	#include <conio.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "bas_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "eapi_aurora.h"
#include "bas_error.h"
#include "terminal_utils.h"
#include "AuroraMonitor.h"

// WARNING: to run this demo under LINUX, you may need root privilege

Result AuroraMonitor(connection_state * state)
{
	adp_result_t res;
	uint32_t i;
	int32_t iCurrentLine;
	int32_t iLineWritten;

	printf("Aurora : Status RX(Gbps) TX(Gbps) RX(GB) TX(GB)\n");
	iCurrentLine = GetCursorLine();

	while(!_kbhit())
	{
		iLineWritten = 0;

		for(i = 1; i <= AURORA_MAX_NUM; i++)
		{
			unsigned int uiStatus, uiVersion;
			uint64_t ui64RxDataCount, ui64TxDataCount;
            uint32_t ui32RxDataRate, ui32TxDataRate;

			res = Aurora_GetVersion_send( state, i, &uiVersion );
			if(FAILURE(res))
            {
				continue;
			}

			res = Aurora_GetChannelStatus_send( state, i, &uiStatus );
			if(FAILURE(res))
			{
				return res;
			}

			res = Aurora_GetRxDataCount_send( state, i, &ui64RxDataCount);
			if(FAILURE(res))
			{
				return res;
			}

			res = Aurora_GetTxDataCount_send( state, i, &ui64TxDataCount);
            if(FAILURE(res))
			{
				return res;
			}

            res = Aurora_GetRxDataRate_send( state, i, &ui32RxDataRate);
            if(FAILURE(res))
			{
				return res;
			}

            res = Aurora_GetTxDataRate_send( state, i, &ui32TxDataRate);
            if(FAILURE(res))
			{
				return res;
			}

			printf("    #%d : %6s %8.1f %8.1f %6.0f %6.0f\n",
                    i,
                    uiStatus?"Up":"Down",
                    ui32RxDataRate*8.0/1e9, ui32TxDataRate*8.0/1e9,
                    ui64RxDataCount/1e9, ui64TxDataCount/1e9);

			iLineWritten++;
		}

		Sleep(1000);
		
		iCurrentLine = GetCursorLine();
		SetCursorPosition( 0, iCurrentLine - iLineWritten );
	}
    GETCH();
	SetCursorPosition( 0, iCurrentLine );
	printf("\n");
	
    return OK;
}
