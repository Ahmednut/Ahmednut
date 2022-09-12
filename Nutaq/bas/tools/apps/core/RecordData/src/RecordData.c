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
 * @file      RecordData.c
 * 
 * @brief     Function that Records to RAM
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
#include "connection_state.h"
#include "eapi.h"
#include "eapi_carrier.h"
#include "carrier_service_def.h"
#include "recplay_types.h"
#include "recplay.h"
#include "linux_utils.h"
#include "bas_error.h"
#include "bas_time.h"
#include "RecordData.h"

Result RecordData(connection_state * state,
                eRecPlayTrigSrc eTriggerSource,
                uint32_t u32RecordSize,      //in bytes
                uint32_t u32StartAddress,    //in bytes
                uint32_t u32TriggerDelay)    //in bytes
{
    adp_result_t res;

	unsigned int uiRecPlayBaseAddr;
	adp_handle_recplay hRecPlay = NULL;
    unsigned char uInitDone;
    
    /*************************************************************************
        Opening interface with the Record Playback module
    *************************************************************************/
    res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRecPlay, 0, &uiRecPlayBaseAddr);
    if(FAILURE(res)) goto end;

	// Record/Playback initialization
	res = RecPlay_Open( &hRecPlay, state, uiRecPlayBaseAddr );
    if(FAILURE(res)) goto end;

    /*************************************************************************
        Verify memory status and reset the core
    *************************************************************************/
    res = RecPlayGetMemoryInitDoneStatus( hRecPlay, &uInitDone );
    if(FAILURE(res)) goto end;

    if( uInitDone == 0 )
    {
        printf( "The DDR3 sodimm initialization is not done.\n"
                "Check if the DDR3 chip is correctly connected.\n" );
        goto end;
    }

    printf( "Resetting Record Playback module...\n" );
    res = RecPlayResetCore( hRecPlay );
    if(FAILURE(res)) goto end;

    
    /*************************************************************************
        Configure record mode
    *************************************************************************/
    if (eTriggerSource == eRecPlayTrigSoft) printf( "Setting the Record trigger in software mode...\n");
    else if (eTriggerSource == eRecPlayTrigExternal) printf( "Setting the Record trigger in external mode...\n");
    else printf( "Setting the Record in no trigger mode...\n");
    
    res = RecordSetTriggerSource( hRecPlay, eTriggerSource );
    if(FAILURE(res)) goto end;

    printf( "Start recording...\n");
    res = RecPlaySetModeRecord( hRecPlay, u32StartAddress, u32TriggerDelay, u32RecordSize );
    if(FAILURE(res)) goto end;
    
    if(eTriggerSource == eRecPlayTrigSoft)
    {
        printf( "Trigger the Record by software...\n");
        res = RecordSoftTrig( hRecPlay );
        if(FAILURE(res))
        if(FAILURE(res)) goto end;
    }

    res = OK;

end:
    RecPlay_Close(hRecPlay);
    
    return res;
}

