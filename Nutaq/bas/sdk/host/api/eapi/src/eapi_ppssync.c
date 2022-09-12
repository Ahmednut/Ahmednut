/****************************************************************************  
 *
 *    ****                              I                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    ****I  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file      eapi_ppsync.c
 *
 * @brief     Function definitions for the eapi PPS-SYNC module
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
 ***************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "cce.h"
#include "eapi_serialize.h"
#include "ppssync_types.h"

Result PPSSync_Presence_send(connection_state * state,  unsigned int *idcode, unsigned int *ppsver)
{	
	cmd_send(state, PPSSYNC_PRESENCE);
	//receive from MB
	*idcode = uint_recv(state);
	*ppsver = uint_recv(state);
	return result_recv(state);
}

Result PPSSync_Configure_send(connection_state * state,  float fMinDAC, float fMaxDAC, unsigned int uPpsFreq)
{
	cmd_send(state, PPSSYNC_CONFIGURE);
	float_send(state, fMinDAC);	
	float_send(state, fMaxDAC);	
	uint_send(state, uPpsFreq);	
	return result_recv(state);
}

Result PPSSync_DacInit_send(connection_state * state,  float fDacVal)
{
	cmd_send(state, PPSSYNC_DACINIT);
	float_send(state, fDacVal);	
	return result_recv(state);
}

Result PPSSync_Start_send(connection_state * state, unsigned int vcxo_freq, unsigned int integration_time, float Kp, float Ki)
{
	cmd_send(state, PPSSYNC_START);
	uint_send(state, vcxo_freq);
	uint_send(state, integration_time);
	float_send(state, Kp);
	float_send(state, Ki);
	return result_recv(state);
}

Result PPSSync_Stop_send(connection_state * state)
{
	cmd_send(state, PPSSYNC_STOP);
	return result_recv(state);
}

Result PPSSync_ReadRefDacVal_send(connection_state * state, float * pfRefDacVal)
{
	cmd_send(state, PPSSYNC_READDAC);
	*pfRefDacVal = float_recv(state);
	return result_recv(state);
}

Result PPSSync_SaveRefDacVal_send(connection_state * state)
{
	cmd_send(state, PPSSYNC_SAVEDAC);
	return result_recv(state);
}

Result PPSSync_ReadPIProfile_send(connection_state * state, float *Kp, float *Ki)
{
	cmd_send(state, PPSSYNC_READPROFILE);	//read PI profile	
	*Kp = float_recv(state);	
	*Ki = float_recv(state);	
	return result_recv(state);
}


Result PPSSync_SavePIProfile_send(connection_state * state)
{
	cmd_send(state, PPSSYNC_SAVEPROFILE);
	return result_recv(state);
}

Result PPSSync_GetInfo_send(connection_state * state, PPSSync_Info_t *PPSSyncInfo)
{
	cmd_send(state, PPSSYNC_GETINFO);
	PPSSyncInfo->TargetFreq = uint_recv(state);
	PPSSyncInfo->TickPerPPS = uint_recv(state);
	PPSSyncInfo->fCTE = float_recv(state);
	PPSSyncInfo->fDACValue = float_recv(state);
	PPSSyncInfo->Lock = uint_recv(state);
	PPSSyncInfo->fPPB = float_recv(state);
	return result_recv(state);
}