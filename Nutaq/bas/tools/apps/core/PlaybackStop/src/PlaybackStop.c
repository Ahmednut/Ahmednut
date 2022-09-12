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
 * @file      PlaybackStop.c
 * 
 * @brief     Function that stops Playback
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
#include <string.h>
#include <stdlib.h>
#include "bas_types.h"
#include "media_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "eapi_carrier.h"
#include "carrier_service_def.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_eth.h"
#include "rtdex_pci.h"
#include "recplay_types.h"
#include "recplay.h"
#include "mac_utils.h" 
#include "media_utils.h" 
#include "bas_error.h"
#include "bas_time.h"
#include "PlaybackStop.h"

// WARNING: to run this demo under LINUX, you may need root privilege


#ifdef __linux__
    RTDExPerseusParamsPcie perseusParams;
#endif

Result PlaybackStop(connection_state * state)
{
	adp_result_t res;
	unsigned int uiRecPlayBaseAddr;
	adp_handle_recplay hRecPlay = NULL;

	res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRecPlay, 0, &uiRecPlayBaseAddr);
    if(FAILURE(res)) goto end;

	// Record/Playback initialization
	res = RecPlay_Open( &hRecPlay, state, uiRecPlayBaseAddr );
    if(FAILURE(res)) goto end;

	// PlaybackStop
	printf( "Stopping the current Record Playback module mode...\n");
	res = RecPlayResetCore( hRecPlay );
	if(FAILURE(res)) goto end;

	printf( "Done.\n");
    res = OK;

end:
    if(hRecPlay) RecPlay_Close(hRecPlay);

    return res;
}
