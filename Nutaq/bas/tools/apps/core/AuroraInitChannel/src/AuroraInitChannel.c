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
 * @file      AuroraInitChannel.c
 * 
 * @brief     Function that intialize an Aurora channel
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
#include "bas_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "eapi_aurora.h"
#include "bas_error.h"
#include "AuroraInitChannel.h"

#define RXEQCTRL 6
#define RXDFETAP1 0
#define TXDRIVERSWING 10
#define TXPOSTEMPHASIS 0
#define TXPREEMPHASIS 0

// WARNING: to run this demo under LINUX, you may need root privilege

Result AuroraInitChannel(connection_state * state, uint32_t u32AuroraChannel, uint32_t u32AuroraDirection)
{
	adp_result_t res;
	unsigned int uiVersion;
	const char * ccAuroraDirection[3] = {"RX","TX","Both"};

	printf("\t - Channel : %u\n",u32AuroraChannel);
	printf("\t - Direction : %s\n",ccAuroraDirection[u32AuroraDirection]);

	// Getting Version
	res = Aurora_GetVersion_send( state, u32AuroraChannel, &uiVersion );
	if(FAILURE(res))
	{
		return res;
	}
	printf("\t - Version : %d.%d\n",(uiVersion >> 8) & 0xFF, (uiVersion >> 0) & 0xFF);
	
	// Setting RX GTX parameters
	if(u32AuroraDirection == 0 || u32AuroraDirection == 2)
	{
		res = Aurora_SetGtxRxParam_send( state, u32AuroraChannel, RXEQCTRL, RXDFETAP1 );
		if(FAILURE(res))
		{
			return res;
		}

		res = Aurora_ResetRxFifo_send( state, u32AuroraChannel );
		if(FAILURE(res))
		{
			return res;
		}

        res = Aurora_SetChannelEnableState_send(state, u32AuroraChannel, eAuroraDirectionRx, 1);
		if(FAILURE(res))
		{
			return res;
		}
	}

	// Setting TX GTX parameters
	if(u32AuroraDirection == 1 || u32AuroraDirection == 2)
	{
		res = Aurora_SetGtxTxParam_send( state, u32AuroraChannel, TXDRIVERSWING, TXPOSTEMPHASIS, TXPREEMPHASIS );
		if(FAILURE(res))
		{
			return res;
		}

		res = Aurora_ResetTxFifo_send( state, u32AuroraChannel );
		if(FAILURE(res))
		{
			return res;
		}

        res = Aurora_SetChannelEnableState_send(state, u32AuroraChannel, eAuroraDirectionTx, 1);
		if(FAILURE(res))
		{
			return res;
		}
	}

    return OK;
}
