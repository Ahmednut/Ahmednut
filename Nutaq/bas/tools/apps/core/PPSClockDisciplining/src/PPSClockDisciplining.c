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
 * @file      PPSClockDisciplining.c
 * 
 * @brief     Function that disciplines the sampling clock to an external PPS signal
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
#include <stdio.h>
#include <stdlib.h>
#include "bas_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "bas_error.h"
#include "eapi_ppssync.h"
#include "ppssync_defines.h"
#include "PPSClockDisciplining.h"
#include "linux_utils.h"

#define PPS_INIT_DAC_VAL        0.0f
#define PPS_INTEGRATION_TIME    10
#define PPS_KP	                60000.0f
#define PPS_KI	                4000.0f
#define PPS_TIMEOUT             100

// WARNING: to run this demo under LINUX, you may need root privilege

Result PPSClockDisciplining(connection_state * state, uint32_t uClockFrequency, uint32_t uPpsFrequency)
{
    float fRefDac;        
    PPSSync_Info_t ppsSyncInfo;
    uint32_t u32PpsId, u32PpsVer;
    adp_result_t res = PPSSYNCOK;
    float Kp, Ki;
    int32_t iTimeout = PPS_TIMEOUT;
    
        // Verify presence of PPS sync core
    res = PPSSync_Presence_send( state, &u32PpsId, &u32PpsVer );
    if ( FAILURE(res) )
    {
        fprintf( stderr, "Failed to detect PPS-SYNC core\n" );
        return res;
    }

    res = PPSSync_Stop_send( state );
    if ( FAILURE(res) )
    {
        fprintf( stderr, "Failed to stop PPS-SYNC\n" );
        return res;
    }

    
    // Read DAC value from file 
    res = PPSSync_Configure_send( state, -1.0f, 1.0f, uPpsFrequency);
    if ( FAILURE(res) )
    {
        fprintf( stderr, "Failed to configure PPS-SYNC parameters\n" );
        return res;
    }


    // Read DAC value from file 
    res = PPSSync_ReadRefDacVal_send( state, &fRefDac );
    if ( FAILURE(res) )
    {
        fRefDac = PPS_INIT_DAC_VAL;
    }
    
    // Set DAC init value
    res = PPSSync_DacInit_send( state, fRefDac );
    if ( FAILURE(res) )
    {
        fprintf( stderr, "Failed to init DAC\n" );  
        return res;
    }

    // Read PI profile from file 
	res = PPSSync_ReadPIProfile_send( state, &Kp, &Ki);
	if(FAILURE(res))
	{
		printf("Failed to open stable PI file. Using default value\n");				
	}

	if(Kp != PPS_KP)
	{
		Kp = (float)PPS_KP;
	}

	if(Ki != PPS_KI)
	{
		Ki = (float)PPS_KI;
	}

    printf("\nPPSSync Read PI profile successful.\n\n");
    // Start disciplining
    res = PPSSync_Start_send( state, uClockFrequency, PPS_INTEGRATION_TIME, (float)PPS_KP, (float)PPS_KI );
    if ( FAILURE(res) )
    {
        fprintf( stderr, "Failed to start PPS-SYNC. Please check PPS connection\n" );  
        return res;
    } 

    // Waiting for VCXO lock to GPS
    do
    {
        // verify information
        res = PPSSync_GetInfo_send( state, &ppsSyncInfo );
        if ( FAILURE(res) )
        {
            fprintf( stderr, "Failed to get PPS-SYNC info\n" );
            return res; 
        }

        if ( --iTimeout <= 0 )
        {                
            fprintf( stderr, "\nVCXO failed to lock to GPS\n" );
            return PPSSYNCLOCKEDFAILED; 
        }
        printf("tick=%10u cte=%6.1f ppb=%6.1f dac=%7.6f lock=%1u\r", ppsSyncInfo.TickPerPPS, ppsSyncInfo.fCTE, ppsSyncInfo.fPPB, ppsSyncInfo.fDACValue, ppsSyncInfo.Lock);
		fflush(stdout);
        sleep( 2 );
    } while( !ppsSyncInfo.Lock );

    res = PPSSync_SaveRefDacVal_send( state );
    if ( FAILURE(res) )
    {
        fprintf( stderr, "Failed to store calibrated DAC value to storage device\n" );
        return res;
    } 
    res = PPSSync_SavePIProfile_send( state );
    if ( FAILURE(res) )
    {
        fprintf( stderr, "Failed to store PI profile to storage device\n" );
        return res;
    }   
    return res;
}
