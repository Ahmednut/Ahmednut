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
 * @file      RecPlayTestUtil.c
 * 
 * @brief     Functions definition for Record Playback access.
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
#include <string.h>

#include "linux_utils.h"
#include "bas_error.h"
#include "bas_types.h"
#include "media_types.h"
#include "connection_state.h"
#include "RecPlayTestUtil.h"
#include "eapi.h"


Result RecPlayTestUtil(char *pcIpAddress, handle_recplaytestparams hParams, char acAction[])
{
    adp_result_t res;	
    connection_state state;
    uint32_t uMaxData;
    FILE *fp;
    uint32_t u32DataCount;
    uint32_t u32NumberOfErrors;
    int64_t i64FileSize;
    unsigned int i;
    uint16_t u16SamplePair[2];
    uint32_t u32StartTime;
    uint8_t u8AllDataPlaybacked = 0;
    uint8_t u8TimeoutOccurred = 0;
    uint32_t u32TestFailed = 0;
    uint32_t u32ExpectedValuePort0 = 0;
    uint32_t u32ExpectedValuePort1 = 0;
    uint32_t u32PresentValue = 0;
    uint32_t u32RecordErrorCount = 0;
    unsigned int *pDataBuffer = NULL;

    printf("Action: %s\n\n", acAction);
    if(!strcmp(acAction, "init"))
        hParams->bInitRecPlayTest = TRUE;
    else if(!strcmp(acAction, "enable"))
        hParams->bStartRecPlayTest = TRUE;
    else if(!strcmp(acAction, "stop"))
        hParams->bStopRecPlayTest = TRUE;
    else{
        printf("Error with action parameter. Value must be in the range 0 - 2.\n");
        goto error;
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

    //Hardcoded for port width of 16 bits
    uMaxData = (1 << 16) - 1;    //65535

    if(!strcmp(acAction, "init"))
    {
        // Test logic reset
        res = RecPlayTestStop(&state, hParams->u32RecPlayTestBaseCustomRegister );
        if(FAILURE(res))
        {
            goto error;
        }

	    res = RecPlayTestReset(&state, hParams->u32RecPlayTestBaseCustomRegister );
        if(FAILURE(res))
        {
            goto error;
        }
    
	    // Trigger position has no impact in this test, but just allowing to record specified data
	    // Do not change it
	    res = RecPlaySetRecTrig(&state, hParams->u32RecPlayTestBaseCustomRegister, 1 );
        if(FAILURE(res))
        {
            goto error;
        }

        if(hParams->u32Mode == PLAYBACK)
        {
	        res = RecPlaySetTestMaxData( &state, hParams->u32RecPlayTestBaseCustomRegister, uMaxData );
            if(FAILURE(res))
            {
                goto error;
            }

            pDataBuffer = (unsigned int*) malloc (hParams->u32TestSize);
            if (pDataBuffer==NULL)
            {
                free(pDataBuffer);
                printf( "Memory allocation failed.\n\n" );
            }

            // generation of ramp
	        for( i = 0; i < hParams->u32TestSize/2; i+=2 )
	        {
		        ((unsigned short*)pDataBuffer)[i] = i/2;
		        ((unsigned short*)pDataBuffer)[i+1] = i/2;
	        }

            fp = fopen( hParams->pcFileName, "wb");
            if ( fp == NULL )
            {
                perror( "fopen");
                goto error;
            }

            if(fwrite(pDataBuffer, sizeof(short), hParams->u32TestSize/2, fp) != hParams->u32TestSize/2)
            {
                printf("fail here\n");
                printf("Error with fwrite while writing ramp file.");
                goto error;
            }
            if(fclose(fp))
            { 
                printf("Error closing generated ramp file.");
                goto error;
            }
        }
    }
    if(!strcmp(acAction, "enable"))
    {
        res = RecPlayTestStart( &state, hParams->u32RecPlayTestBaseCustomRegister );
        if(FAILURE(res))
        {
            goto error;
        }
    }
    if(!strcmp(acAction, "stop"))
    {
        if(hParams->u32Mode == PLAYBACK)
        {
            u32StartTime = GetTickCount();
            while(!u8TimeoutOccurred && !u8AllDataPlaybacked)
            {
                // validating number of data validated
	            res = RecPlayGetTestDataCnt(&state, hParams->u32RecPlayTestBaseCustomRegister , &u32DataCount );
                if(FAILURE(res))
                {
                    goto error;
                }

	            if (u32DataCount * 2 * 2 >= hParams->u32TestSize)
                    u8AllDataPlaybacked = 1;

                if(GetTickCount() - u32StartTime >= hParams->u32PlaybackTimeout && !u8AllDataPlaybacked)
                {
                    printf( "\nError, number of bytes verified is lower than expected: expected: %u, verified: %u.\n", hParams->u32TestSize, u32DataCount * 4);
                    u8TimeoutOccurred = 1;
                    u32TestFailed++;
                }
            }
            if(u32DataCount * 4 > hParams->u32TestSize)
            {
                printf("\nError, number of bytes verified is higher than expected: expected: %u, verified: %u.\n", hParams->u32TestSize, u32DataCount * 4);
                u32TestFailed++;
            }
                
            printf( "Verified  %u bytes\n", u32DataCount * 4);
	        // verify ramp validator for errors. Here hardcoded (i < 2) to verify 2 ports.
	        for( i = 0 ; i < 2; i++ )
	        {
		        res = RecPlayGetNbErrors( &state, hParams->u32RecPlayTestBaseCustomRegister, i, &u32NumberOfErrors  );
                if(FAILURE(res))
                {
                    goto error;
                }

		        if( u32NumberOfErrors != 0 )
		        {
			        printf( " %u errors on port %u.\n\n\n\n", u32NumberOfErrors, i );
			        u32TestFailed++;
		        }
	        }
	
	        if(u32TestFailed == 0 )
	        {
		        printf( "Success.\n" );
	        }
        }
        
        //Clean up test logic
        res = RecPlayTestStop( &state, hParams->u32RecPlayTestBaseCustomRegister );
        if(FAILURE(res))
        {
            goto error;
        }
	    res = RecPlayTestReset( &state, hParams->u32RecPlayTestBaseCustomRegister );
        if(FAILURE(res))
        {
            goto error;
        }

        if(hParams->u32Mode == RECORD)
        {
            fp = fopen( hParams->pcFileName, "rb");
            if(fp == NULL)
            {
                printf("Error with fopen while analyzing received ramp file.");;
                goto error;
            }
            if(_fseeki64(fp, 0, SEEK_END))
            {
                printf("Error analyzing received ramp file.");
                goto error;
            }
            i64FileSize = _ftelli64(fp);
            if(i64FileSize < 0)
            {
                printf("Error analyzing received ramp file.");
                goto error;
            }
            if(i64FileSize != hParams->u32TestSize)
            {
                printf("Received ramp file is of different size than configured test size");
                goto error;
            }
            if(_fseeki64(fp, 0, SEEK_SET))
            {
                printf("Error analyzing received ramp file.");
                goto error;
            }
            for( i = 0; i < hParams->u32TestSize/4; i++ ) 
	        {
                if(fread(u16SamplePair, sizeof(short), 2, fp) != 2)
                {
                    printf("Error analyzing received ramp file.");
                    goto error;
                }
                //Verifying port 0
		        if( u16SamplePair[0] != u32ExpectedValuePort0) 
		        {
			        u32RecordErrorCount++;
                    if(u16SamplePair[0] == uMaxData)
                        u32ExpectedValuePort0 = 0;
                    else
		                u32ExpectedValuePort0 = u16SamplePair[0] + 1;
		        }
                else 
		        {
                    if(u16SamplePair[0] == uMaxData)
                        u32ExpectedValuePort0 = 0;
                    else
			            u32ExpectedValuePort0++;
		        }

                //Verifying port 1
                if( u16SamplePair[1] != u32ExpectedValuePort1) 
		        {
			        u32RecordErrorCount++;
                    if(u16SamplePair[1] == uMaxData)
                        u32ExpectedValuePort1 = 0;
                    else
		                u32ExpectedValuePort1 = u16SamplePair[1] + 1;	
		        }
                else 
		        {
                    if(u16SamplePair[1] == uMaxData)
                        u32ExpectedValuePort1 = 0;
                    else
			            u32ExpectedValuePort1++;
		        }
	        }
            if(fclose(fp))
            {
                printf("Error closing received ramp file.");
                goto error;
            }

            printf("Error(s) detected when analyzing received ramp file: %d\n", u32RecordErrorCount);
            if(!u32RecordErrorCount)
                printf("Success.\n");
        }
	    // exit
        if(pDataBuffer != NULL)
            free(pDataBuffer);
    }


error:
    if ( FAILURE(res) )
    {
        fprintf( stderr, "%s\n\n\n", ErrorToString(res) );
    }
    disconnect_cce(&state);
    return res;
}
    
adp_result_t RecPlayTestReset( connection_state * state, 
                               uint32_t u32BaseCustomRegisterId  )
{
    uint32_t u32CustomRegisterId;
    adp_result_t res;

    u32CustomRegisterId = u32BaseCustomRegisterId + RESET_INDEX;

    res = custom_register_write_send( state, u32CustomRegisterId, 1 );

	if(FAILURE(res))
	{
		return res;
	}

    res = custom_register_write_send( state, u32CustomRegisterId, 0 );

    return res;
}


adp_result_t RecPlayTestStart( connection_state * state, 
                               uint32_t u32BaseCustomRegisterId  )
{
    uint32_t u32CustomRegisterId;
    adp_result_t res;

    //unsigned int uValue;

    u32CustomRegisterId = u32BaseCustomRegisterId + RECPLAY_START_INDEX;

    res = custom_register_write_send( state, u32CustomRegisterId, 1 );

    return res;
}


adp_result_t RecPlayTestStop( connection_state * state, 
                               uint32_t u32BaseCustomRegisterId  )
{
    uint32_t u32CustomRegisterId;
    adp_result_t res;

    u32CustomRegisterId = u32BaseCustomRegisterId + RECPLAY_START_INDEX;

    res = custom_register_write_send( state, u32CustomRegisterId, 0 );

    return res;
}

adp_result_t RecPlaySetRecTrig( connection_state * state, 
                               uint32_t u32BaseCustomRegisterId ,
                               unsigned int uTrigAddr  )
{
    uint32_t u32CustomRegisterId;
    adp_result_t res;

	// Validate trigger position
	if (uTrigAddr > RECPLAYTEST_MAXTRIGPOSITION)
	{
        return RECPLAYTESTINVTRIGGER;
	}

    u32CustomRegisterId = u32BaseCustomRegisterId + TRIG_ADDRESS_INDEX;
    res = custom_register_write_send( state, u32CustomRegisterId, uTrigAddr );
	if(FAILURE(res))
	{
		return res;
	}

    u32CustomRegisterId = u32BaseCustomRegisterId + SET_REC_TRIG_INDEX;

    res = custom_register_write_send( state, u32CustomRegisterId, 1 );
	if(FAILURE(res))
	{
		return res;
	}

    res = custom_register_write_send( state, u32CustomRegisterId, 0 );

    return res;
}

adp_result_t RecPlaySetPlayTrig( connection_state * state, 
                               uint32_t u32BaseCustomRegisterId  )
{
    uint32_t u32CustomRegisterId;
    adp_result_t res;

    u32CustomRegisterId = u32BaseCustomRegisterId + SET_REC_PLAY_INDEX;

    res = custom_register_write_send( state, u32CustomRegisterId, 1 );
	if(FAILURE(res))
	{
		return res;
	}

    res = custom_register_write_send( state, u32CustomRegisterId, 0 );

    return res;
}


adp_result_t RecPlayGetNbErrors( connection_state * state, 
                               uint32_t u32BaseCustomRegisterId ,
                                 unsigned int uChannel,
                                 unsigned int* pNbErrors  )
{
    uint32_t u32CustomRegisterId;
    adp_result_t res;

    if( uChannel >= 16 )
    {
        return -1;
    }

    u32CustomRegisterId = u32BaseCustomRegisterId + ERROR_CNT_CH_INDEX + uChannel;

    res = custom_register_read_send( state, u32CustomRegisterId, pNbErrors );

    return res;
}


adp_result_t RecPlaySetTestCntDiv( connection_state * state, 
                                 uint32_t u32BaseCustomRegisterId , 
                                 unsigned int uValue )
{
    adp_result_t res;
    uint32_t u32CustomRegisterId;

    u32CustomRegisterId = u32BaseCustomRegisterId + DIV_CNT_INDEX;

    res = custom_register_write_send( state, u32CustomRegisterId, uValue );

    return res;

}

adp_result_t RecPlaySetTestMaxData( connection_state * state, 
                                 uint32_t u32BaseCustomRegisterId , 
                                 unsigned int uValue )
{
    adp_result_t res;
    uint32_t u32CustomRegisterId;

    u32CustomRegisterId = u32BaseCustomRegisterId + MAX_DATA_INDEX;

    res = custom_register_write_send( state, u32CustomRegisterId, uValue );

    return res;

}

adp_result_t RecPlayGetTestDataCnt( connection_state * state, 
                                    uint32_t u32BaseCustomRegisterId , 
                                    unsigned int *uValue )
{
    adp_result_t res;
    uint32_t u32CustomRegisterId;

    u32CustomRegisterId = u32BaseCustomRegisterId + DATA_CNT_INDEX;

    res = custom_register_read_send( state, u32CustomRegisterId, uValue );

    return res;
}

