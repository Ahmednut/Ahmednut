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
 * @file      TimestampUtil.c
 * 
 * @brief     Function for initializing the timestamp module or using some of its features
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
#include <string.h>
#include <stdlib.h>
#include "eapi_carrier.h"
#include "bas_types.h"
#include "media_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "mac_utils.h" 
#include "media_utils.h" 
#include "linux_utils.h"
#include "terminal_utils.h"
#include "bas_error.h"
#include "bas_time.h"
#include "TimestampUtil.h"
#include "eapi_timestamp.h"


/************************************************************************************************************************
											   Timestamp Init 
************************************************************************************************************************/
Result TimestampUtil(char *pcIpAddress, handle_timestampparams hParams)
{
	adp_result_t res;	
    int32_t i32TimeoutMs;
    uint64_t u64TsTick;

    connection_state state;
    Timestamp_Handle_t hTimestamp;

    uint32_t u32TimeLsw, u32TimeMsw;

    const int32_t i32LoopSleepTimeMs = 10;


	/************************************************************************************************************************
												Configure the timestamp module
	************************************************************************************************************************/
    /*
     * Wait until time only, then return
     */
    if(hParams->bWaitUntilTime)
    {
        uint32_t u32secTarget, u32nsecTarget;
        uint32_t u32secCurrent, u32nsecCurrent;
        int32_t i32NanoSecRemainder = 0;

        res = Timestamp_ReadableLocalTimeToPosix(hParams->acTime, &u32secTarget, &u32nsecTarget);
        if ( FAILURE(res) ) goto error;

        //Treating offset values
        if (hParams->i32NanoSecOffset*-1 > (signed)u32nsecTarget)
        {
            i32NanoSecRemainder = hParams->i32NanoSecOffset*-1 - u32nsecTarget;
            u32nsecTarget = 1000000000 - i32NanoSecRemainder;
            u32secTarget--;
        }
        else
        {
            u32nsecTarget += hParams->i32NanoSecOffset;
            if( u32nsecTarget >= 1000000000)
            {
                i32NanoSecRemainder = u32nsecTarget - 1000000000;
                u32nsecTarget = i32NanoSecRemainder;
                u32secTarget++;
            }
        }
        printf( "Waiting until %s with offset of %d nanoseconds...\n", hParams->acTime, hParams->i32NanoSecOffset);
        while(1)
        {
            res = Timestamp_GetHostPosixTime( &u32secCurrent, &u32nsecCurrent);
            if ( FAILURE(res) ) goto error;
            
            // Return when the time has been reached
            if((u32secCurrent > u32secTarget) ||
                (u32secCurrent == u32secTarget && u32nsecCurrent >= u32nsecTarget))
            {
                printf( "The specified time has been reached.\n");
                return 0;
            }
            
            // Sleep 1 ms
            Sleep(1);
        }
    }

    /*
     * Connect to the board
     */

    // Connect the Perseus AMC
    printf( "Connecting to the board..."); 
    fflush(stdin);
    res = connect_cce(pcIpAddress, &state);
    if ( FAILURE(res) )
    {
       fprintf( stderr, "The Perseus is not responding.\n" );
       fprintf( stderr, "Please ensure the Perseus IP address you specified (%s) is correct.\n", pcIpAddress );
       goto error;
    }
    printf( " Done\n" );

    /*
     * Verify Timestamp module presence
     */
    printf( "Opening timestamp module..." );
    fflush(stdin);
    res = Timestamp_Open(&hTimestamp, state, hParams->u32ClkFreq);
    if ( FAILURE(res) )
    {
        fprintf( stderr, "The timestamp module cannot be opened.\n" );
        goto error;
    }
    printf( " Done\n" );

    // Read time value stored in custom registers
    if(hParams->bReadCustomRegisterTickTime || 
       hParams->bReadCustomRegisterReadableTime ||
       hParams->bReadCustomRegisterPosixTime)
    {
       uint32_t u32sec;
       uint32_t u32nsec;
       res = custom_register_read_send(&state, hParams->u32LswCustomRegisterId, &u32TimeLsw);
       if ( FAILURE(res) )
       {
           fprintf( stderr, "The custom register holding the least significant word of the time value cannot be opened.\n" );
           goto error;
       }
       res = custom_register_read_send(&state, hParams->u32MswCustomRegisterId, &u32TimeMsw);
       if ( FAILURE(res) )
       {
           fprintf( stderr, "The custom register holding the most significant word of the time value cannot be opened.\n" );
           goto error;
       }
       u64TsTick = ((uint64_t)u32TimeMsw << 32) | (uint64_t)u32TimeLsw;
       
       if(hParams->bReadCustomRegisterPosixTime || hParams->bReadCustomRegisterReadableTime)
       {
           res = Timestamp_TickToPosixTime(hTimestamp, u64TsTick, &u32sec, &u32nsec);
           if ( FAILURE(res) )
           {
               fprintf( stderr, "Error with tick time to POSIX time conversion.\n" );
               goto error;
           }
       }
       else
       {
           printf("Tick time stored in custom registers %d and %d : %"PRI_U64" ticks\n", hParams->u32LswCustomRegisterId, hParams->u32MswCustomRegisterId, u64TsTick);
           goto done;
       }
       if(hParams->bReadCustomRegisterReadableTime)
       {
           char pcReadableTime[50];
           res = Timestamp_PosixToReadableLocalTime(u32sec, u32nsec, pcReadableTime, sizeof(pcReadableTime));
           if ( FAILURE(res) )
           {
               fprintf( stderr, "Error with POSIX time to readable local time conversion.\n" );
               goto error;
           }
           printf("Readable local time stored in custom registers %d and %d : %s\n", hParams->u32LswCustomRegisterId, hParams->u32MswCustomRegisterId, pcReadableTime);
           goto done;
       }
       else
       {
           printf("POSIX time stored in custom registers %d and %d : %d secs, %d nsecs\n", hParams->u32LswCustomRegisterId, hParams->u32MswCustomRegisterId, u32sec, u32nsec);
           goto done;
       }

    }

    /*
     * Get current device time
     */
    if ( hParams->bGetTickTimeOnly )
    {
        uint64_t u64TickTime;

        printf( "Getting timestamp tick time..." ); 
        fflush(stdin);
        res = Timestamp_GetDeviceTickTime(hTimestamp, &u64TickTime);
        if ( FAILURE(res) ) goto error;
        printf( " Done\n" );

        printf( "Device tick time : %"PRI_U64"\n", u64TickTime );
        goto done;
    }
    else if ( hParams->bGetPosixTimeOnly )
    {
        uint32_t u32sec;
        uint32_t u32nsec;

        printf( "Getting timestamp POSIX time..." ); 
        fflush(stdin);
        res = Timestamp_GetDevicePosixTime(hTimestamp, &u32sec, &u32nsec);
        if ( FAILURE(res) ) goto error;
        printf( " Done\n" );

        printf( "Device POSIX time : %u.%09u\n", u32sec, u32nsec );
        goto done;
    }
    else if ( hParams->bGetReadableLocalTimeOnly )
    {
        char acDeviceTime[30];

        printf( "Getting timestamp readable local time..." );
        fflush(stdin);
        res = Timestamp_GetDeviceReadableLocalTime(hTimestamp, acDeviceTime, sizeof(acDeviceTime));
        if ( FAILURE(res) ) goto error;
        printf( " Done\n" );


        printf( "Device time : %s\n", acDeviceTime );
        goto done;
    }

    printf( "Opening timestamp module..." );
    fflush(stdin);
    res = Timestamp_Open(&hTimestamp, state, hParams->u32ClkFreq);
    if ( FAILURE(res) )
    {
        fprintf( stderr, "The timestamp module cannot be opened.\n" );
        goto error;
    }
    printf( " Done\n" );

    /*
     * Reset the timestamp module
     *         Set the counter value to 0 and start counting
     */
    printf( "Resetting timestamp..." );
    fflush(stdin);
    res = Timestamp_Reset(hTimestamp);
    if ( FAILURE(res) ) goto error;
    printf( " Done\n" );

    if(hParams->bResetOnly)
        goto done;

    /*
     * Configure timestamp module with absolute time if requested.
     */
    if(hParams->bAbsoluteTime)
    {
        char acDeviceTime[30];
        char acHostTime[30];
        
        res = Timestamp_ApplyHostTimeToDevice(hTimestamp);
        if ( FAILURE(res) ) goto error;

        res = Timestamp_VerifyTime(hTimestamp);
        if ( FAILURE(res) ) goto error;

        res = Timestamp_GetDeviceReadableLocalTime(hTimestamp, acDeviceTime, sizeof(acDeviceTime));
        if ( FAILURE(res) ) goto error;

        res = Timestamp_GetHostReadableLocalTime(acHostTime, sizeof(acHostTime));
        if ( FAILURE(res) ) goto error;

        printf("Device time : %s\n", acDeviceTime);
        printf("Host time   : %s\n", acHostTime);

        goto done;
    }

    /*
     * Set the counter value to be set on the next trigger event
     */
    printf( "Setting timestamp time..." );
    fflush(stdin);
    res = Timestamp_SetTime_send(&state, hParams->u64Value);
    if ( FAILURE(res) ) goto error;
    printf( " Done\n" );

    if(hParams->i32WaitSec == 0)
        goto done;

    /*
     * Verify if the trigger event has happened
     */
    printf( "Verifying timestamp configuration:\n" );
    i32TimeoutMs = (hParams->i32WaitSec * 1000);
    while(1)
    {
        res = Timestamp_GetTime_send(&state, &u64TsTick);
        if ( FAILURE(res) ) goto error;

        if ( u64TsTick )
            break;

        if ( i32TimeoutMs <= 0)
        {
            fprintf( stderr, "\tThe timestamp is still not configured\n");
            fprintf( stderr, "\tand more than %d second has been elapsed.\n", hParams->i32WaitSec);
            fprintf( stderr, "\tIt seems the PPS pulse is not connected to the system.\n");
            goto done;
        }
        Sleep(i32LoopSleepTimeMs);
        i32TimeoutMs -= i32LoopSleepTimeMs;
    }
    printf( "\tThe trigger event has happened.\n" );
    printf( "\tCurrent time is : %"PRI_U64"\n", u64TsTick );
    printf( "\tDone\n" );

    res = 0;
    goto done;


done :	

	if(FAILURE(res))
    {
        fprintf( stderr, "%s\n\n\n", ErrorToString(res) );
    }


	/************************************************************************************************************************
												Closing connection to Perseus
	************************************************************************************************************************/
	
    disconnect_cce(&state);

error:
    if ( FAILURE(res) )
    {
        fprintf( stderr, "%s\n\n\n", ErrorToString(res) );
    }

    return res;
}
