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
 * @file      PlaybackFromRAM.c
 * 
 * @brief     Function that Playback from RAM
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
#include "verbose_utils.h"
#include "bas_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "eapi_carrier.h"
#include "carrier_service_def.h"
#include "recplay_types.h"
#include "recplay.h"
#include "bas_error.h"
#include "bas_time.h"
#include "PlaybackData.h"

Result PlaybackData(connection_state * state, 
					eRecPlayTrigSrc eTriggerSource, 
					uint32_t u32PlaybackMode, 
					uint32_t u32PlaybackSize,
					uint32_t u32StartAddress)
{
    adp_result_t res = 0;   
	unsigned int uiRecPlayBaseAddr;
	adp_handle_recplay hRecPlay = NULL;	
    
    /*************************************************************************
        Opening interface with the Record Playback module
    *************************************************************************/
	res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRecPlay, 0, &uiRecPlayBaseAddr);
    if(FAILURE(res)) goto end;

	// Record/Playback initialization
	res = RecPlay_Open( &hRecPlay, state, uiRecPlayBaseAddr );
    if(FAILURE(res)) goto end;

    // Check if the previous host to memory transfer (or record) has been completed
    res = RecPlayWaitTransferDone(hRecPlay,2000);
    if(FAILURE(res))
    {
        bas_printf(ERR, "\n");
        bas_printf(ERR, "The Record Playback module is currently running in an other transfer mode.\n");
        goto end;
    }

    // trigger source is software
    if (eTriggerSource == eRecPlayTrigSoft) 
	{
		bas_printf(INFO, "Setting the Playback trigger in software mode...\n");
	}
    else if (eTriggerSource == eRecPlayTrigExternal) 
	{
		bas_printf(INFO, "Setting the Playback trigger in external mode...\n");
	}
    else 
	{
		bas_printf(INFO, "Setting the Playback in no trigger mode...\n");
	}

    res = PlaybackSetTriggerSource(hRecPlay, eTriggerSource);
    if( res != 0 )
    {
       return res;
    }

    if(u32PlaybackMode == 1)
    {
        // mode is continuous playback
        bas_printf(INFO, "Setting the Playback in continuous mode...\n");
        res = RecPlaySetModePlayBackContinuous( hRecPlay,u32StartAddress, u32PlaybackSize ) ;
        if(FAILURE(res)) goto end;
    }
    else
    {
        // mode is single playback
        bas_printf(INFO, "Setting the Playback in single mode...\n");
        res = RecPlaySetModePlayBackSingle( hRecPlay,u32StartAddress, u32PlaybackSize ) ;
        if(FAILURE(res)) goto end;
    }

    if(eTriggerSource == eRecPlayTrigSoft)
    {
        // software trigger
        bas_printf(INFO, "Trigger the Playback start by software...\n");
        res = PlaybackSoftTrig( hRecPlay);
        if(FAILURE(res)) goto end;
    }
    
    bas_printf(INFO, "Done.\n");
	fflush(stdout);
    res = OK;

end:
    if(hRecPlay) RecPlay_Close(hRecPlay);
	fflush(stdout);
    return res;
}
