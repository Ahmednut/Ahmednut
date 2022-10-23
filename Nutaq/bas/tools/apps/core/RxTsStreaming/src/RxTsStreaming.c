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
 * @file      RxTsStreaming.c
 * 
 * @brief     Function for RX Streaming
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
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_eth.h"
#include "rtdex_pci.h"
#include "rtdexts.h"
#include "rtdexts_def.h"
#include "mac_utils.h" 
#include "media_utils.h" 
#include "linux_utils.h"
#include "terminal_utils.h"
#include "bas_error.h"
#include "bas_time.h"
#include "bas_buffer.h"
#include "bas_semaphore.h"
#include "bas_thread.h"
#include "RxTsStreaming.h"

// WARNING: to run this demo under LINUX, you may need root privilege
#define NUM_RTDEXTS_EVT   8
/**
 * RTDExTs handles
 */
RTDExTs_Handle_t g_ahRTDExTs[MAX_RTDEXTS] = {NULL};
handle_rtdexts_channel hRTDExChannel[MAX_RTDEXTS] = {NULL};
handle_streaming hStreaming;
eMediaType eMedia;
RTDExPerseusParamsPcie perseusParams[MAX_PERSEUS];
float fDataRate[MAX_RTDEXTS] = {0.0};

/**
 * Task
 */
static adp_thread_t g_atRTDExTask[MAX_RTDEXTS];
static adp_thread_t g_atFileTask[MAX_RTDEXTS];
static int g_stopTask[MAX_RTDEXTS] = {0};
static int g_stopDislayTask = 0;

/**
 * Buffer queues
 */
Buffer_Queue_t g_aFreeBuffQ[MAX_RTDEXTS] = {NULL};
Buffer_Queue_t g_aFullBuffQ[MAX_RTDEXTS] = {NULL};

/**
 * Mutex and counter to synchronize all the TX threads
 */
adp_mutex_t g_DisplayEventMutex;

static adp_thread_dec_t RxTask( void *param );
static adp_thread_dec_t SaveTask( void *param );
static void DisplayEvent(  char               *szPrefix,
                           uint32_t         u32ThreadNum,
                           uint32_t           u32EvtCount,
                           RTDExTs_Event_t  *pEvtQueue );
static void DisplayStopEventOnly(  char               *szPrefix,
                                   uint32_t         u32ThreadNum,
                                   uint32_t           u32EvtCount,
                                   RTDExTs_Event_t  *pEvtQueue );

/************************************************************************************************************************
											   RxTsStreaming 
************************************************************************************************************************/
Result RxTsStreaming(char ** pcIpAddresses, uint32_t NbIpAddresses, handle_streaming handle_rxstreaming, handle_rtdexts_channel * handle_rxchannel)
{
	adp_result_t res;	
	uint32_t i, u32ChNum, u32TotalChNum;

	connection_state state[MAX_PERSEUS];
	connection_state StateTmp;
    unsigned int uiRTDExBaseAddr;

	// local to global for threads
	memcpy(&hStreaming, &handle_rxstreaming, sizeof(handle_streaming));
	
	/************************************************************************************************************************
													Connecting to Perseus
	************************************************************************************************************************/
	for(i = 0 ; i < NbIpAddresses ; i++)
	{	
		res = connect_cce(pcIpAddresses[i], &(state[i]));
		if(FAILURE(res))
		{
			return res;
		}
	}

	/************************************************************************************************************************
												Streaming Configuration Display
	************************************************************************************************************************/
	printf("\n    - General streaming parameters\n");
	printf("\t - RTDExTs sub-frame size : %u bytes\n",handle_rxstreaming->u32SubFrameSize);
	printf("\t - RTDExTs frame size : %u bytes\n",handle_rxstreaming->u32FrameSize);
	printf("\t - RTDExTs queue size : %u\n",handle_rxstreaming->u32QueueSize);
	printf("\t - RTDExTs total number of samples to transfer : %u bytes\n",handle_rxstreaming->u32NumOfSamples);
	printf("\t - Real time flag : %u\n",handle_rxstreaming->u8RealTimeFlag);
	printf("\n");

	/************************************************************************************************************************
												Channels Configuration Display
	************************************************************************************************************************/
	u32TotalChNum = handle_rxstreaming->u32NumberOfChannels;
	printf("    - Total channels number : %u\n",u32TotalChNum);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		printf("\t - Channel : #%u\n",handle_rxchannel[u32ChNum]->u8ChannelNumber);
		printf("\t   - Carrier position : #%u\n",handle_rxchannel[u32ChNum]->u8CarrierPosition);
		printf("\t   - Carrier IP address : %s\n",pcIpAddresses[handle_rxchannel[u32ChNum]->u8CarrierPosition-1]);
		printf("\t   - Filename : %s\n",handle_rxchannel[u32ChNum]->pcFileName);		
	}	

	/************************************************************************************************************************
												Open the RTDEx Interfaces
	************************************************************************************************************************/

    res = RTDExTs_ResetCore( &state[0] );
    if ( FAILURE(res) ) goto done;

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		uint32_t uiCarrierPosTmp = handle_rxchannel[u32ChNum]->u8CarrierPosition-1;
		StateTmp = state[uiCarrierPosTmp];
        res = Carrier_GetModuleBaseAddr_send( &StateTmp, Carrier_eModuleRTDExGigE, 0, &uiRTDExBaseAddr);
        if(FAILURE(res))
		{
			goto done;
		}	
		res = RTDExTs_Open(&g_ahRTDExTs[u32ChNum],                  // Pointer to the RTDExTs handle
                    StateTmp,                                       // Connection state
                    handle_rxchannel[u32ChNum]->u8ChannelNumber,    // RTDExTs channel index         
                    RTDExTs_Dir_Uplink,                             // RTDExTs direction
                    uiRTDExBaseAddr,                                // Base address of the RTDEx Ethernet core in the target FPGA.
                    handle_rxstreaming->u32SubFrameSize,            // Media frame size in bytes. Limited by the RTDEx media used.
                    handle_rxstreaming->u32FrameSize,               // Maximum number of media frames per transfer.
                    handle_rxstreaming->u32TsClkFreq,               // Timestamp module clock frequency
                    handle_rxstreaming->bTsInit & (u32ChNum == 0),  // Timestamp module initialization flag
                    &handle_rxstreaming->u32MaxTransferSize);       // Address of a variable that receives the maximum number of IQ samples

		if(FAILURE(res))
		{
			goto done;
		}
	}

    for( u32ChNum=0; u32ChNum < u32TotalChNum; u32ChNum++ )
    {
        // Verify if the time has been correctly set
        res = RTDExTs_VerifyTime( g_ahRTDExTs[u32ChNum] );
        if ( FAILURE(res) )
        {
            fprintf( stderr, "Time verification failed\n");
            goto done;
        }

        // Set propagation time for compensation
        res = RTDExTs_SetPropagationDelay( g_ahRTDExTs[u32ChNum], handle_rxstreaming->i32PropDelayNs);
        if ( FAILURE(res) )
        {
            fprintf( stderr, "Failed to set the propagation delay\n");
            goto done;
        }
    }

	/************************************************************************************************************************
											Alloc a buffer to store the IQ samples.
	************************************************************************************************************************/
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		// local to global for threads
		hRTDExChannel[u32ChNum] = handle_rxchannel[u32ChNum];

		res = Buffer_InitQueue( &g_aFreeBuffQ[u32ChNum], handle_rxstreaming->u32QueueSize, handle_rxstreaming->u32MaxTransferSize * 2*sizeof(int16_t));
        if(FAILURE(res))
		{
			goto done;
		}

		res = Buffer_InitQueue( &g_aFullBuffQ[u32ChNum], 0, handle_rxstreaming->u32MaxTransferSize * 2*sizeof(int16_t));
		if(FAILURE(res))
		{
			goto done;
		}
	}

	/************************************************************************************************************************
												Create the working thread
	************************************************************************************************************************/
#if defined(_WIN32) || defined(_WIN64)
    if ( handle_rxstreaming->u8RealTimeFlag )
    {
        res = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
        if ( res == 0 ) 
		{
			printf( "Failed to set process priority class to real-time.\n" );
		}
    }
#endif

	// Init the display event mutex
    res = InitMutex(&g_DisplayEventMutex);
    if ( res )
    {
        fprintf( stderr, "Failed to initialize mutex.\n");
        goto done;
    }

    // Starting working threads
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		res = StartThread(&g_atFileTask[u32ChNum], SaveTask, (void *)(intptr_t)u32ChNum, 2);
		if(FAILURE(res))
		{
			goto done;
		}		
	}

	// Let time to the first thread to start and be ready receive data
    Sleep(1000);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		res = StartThread(&g_atRTDExTask[u32ChNum], RxTask, (void *)(intptr_t)u32ChNum, 1);
		if(FAILURE(res))
		{
			goto done;
		}		
	}

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		// Wait for the end of the file task
        JoinThread( g_atFileTask[u32ChNum] );
        // Then for the end of the RTDEx task
        JoinThread( g_atRTDExTask[u32ChNum] );		
	}

	// Disable Display Data rate
	g_stopDislayTask = 1;

	res = 0;	

done :	

	if(FAILURE(res))
    {
        fprintf( stderr, "%s\n\n\n", ErrorToString(res) );
    }

	// Clean up
	DestroyMutex(&g_DisplayEventMutex);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		if ( g_aFreeBuffQ[u32ChNum] != NULL )
        {
            Buffer_DestroyQueue( g_aFreeBuffQ[u32ChNum] );
            g_aFreeBuffQ[u32ChNum] = NULL;
        }
        if ( g_aFullBuffQ[u32ChNum] != NULL )
        {
            Buffer_DestroyQueue( g_aFullBuffQ[u32ChNum] );
            g_aFullBuffQ[u32ChNum] = NULL;
        }
		if ( g_ahRTDExTs[u32ChNum] != NULL )
		{
			RTDExTs_Close( g_ahRTDExTs[u32ChNum] );
			g_ahRTDExTs[u32ChNum] = NULL;
		}
	}

	/************************************************************************************************************************
												Closing connection to Perseus
	************************************************************************************************************************/
	for(i = 0 ; i < NbIpAddresses ; i++)
	{
		disconnect_cce(&(state[i]));
	}

    return res;
}

/************************************************************************************************************************
														Working Tasks
************************************************************************************************************************/

/****************************************************************************
 * Function: RxTask
 ************************************************************************//** 
 * 
 * Reception task.
 * 
 * @param param
 *      RTDEx handle.
 *
 * @return
 *      None.
 *
 ****************************************************************************/
static adp_thread_dec_t RxTask( void *param )
{
    adp_result_t res = RTDEXTSOK;
    int bufRes;
    uint32_t u32TotalTransfer = 0;
    uint32_t u32NumRtdexTsEvt = 0;
	RTDExTs_Event_t rtdexTsEvtQ[NUM_RTDEXTS_EVT];
    Buffer_Desc_t *pBuffDesc = NULL;
    RTDExTs_Handle_t hRTDExTs;
	handle_streaming hStream;
	handle_rtdexts_channel hChan;
    unsigned int u32ChanNum;
    Buffer_Queue_t freeBuffQ;
    Buffer_Queue_t fullBuffQ;
    uint32_t u32RelativeHour, u32RelativeMinute, u32RelativeSecond, u32RelativeNSec;
    uint32_t u32HostSec, u32HostNSec;

    // Get the channel number from thread parameter
    u32ChanNum = (uint32_t)(intptr_t)param;

    // Get the RTDEx handle and buffers according to the channel number
	hStream = hStreaming;
    hRTDExTs = g_ahRTDExTs[u32ChanNum];
	hChan = hRTDExChannel[u32ChanNum];
    freeBuffQ = g_aFreeBuffQ[u32ChanNum];
    fullBuffQ = g_aFullBuffQ[u32ChanNum];

    /*
     * Convert readable local time to POSIX time
     */
    
    if(hStream->start.mode == RTDExTs_TrigMode_AtTime)
    {
        // Treat relative times
        if(hStream->start.pcTime[0] == '+')
        {
            if(sscanf(&hStream->start.pcTime[1],"%d:%d:%d.%u",
                &u32RelativeHour,
                &u32RelativeMinute,
                &u32RelativeSecond,
                &u32RelativeNSec) != 4)
            {
                fprintf( stderr, "Error: Cannot parse relative start time <%s>\n", hStream->start.pcTime );
                goto error; 
            }
            res = RTDExTs_GetHostPosixTime( &u32HostSec, &u32HostNSec);
            if ( FAILURE(res) )
            {
                goto error; 
            }
            hStream->start.u32secTime = u32HostSec +
                u32RelativeHour * 3600 +
                u32RelativeMinute * 60 +
                u32RelativeSecond;
            hStream->start.u32nsecTime = u32HostNSec +
                u32RelativeNSec;
        }
        // Treat absolute times
        else
        {
            res = RTDExTs_ReadableLocalTimeToPosix(hStream->start.pcTime, 
                &hStream->start.u32secTime, &hStream->start.u32nsecTime);
            if ( FAILURE(res) )
            {
                fprintf( stderr, "Error: Cannot parse start time <%s>\n", hStream->start.pcTime );
                goto error; 
            }
        }
    }

    if(hStream->stop.mode == RTDExTs_TrigMode_AtTime)
    {
        // Treat relative times
        if(hStream->stop.pcTime[0] == '+')
        {
            if(sscanf(&hStream->stop.pcTime[1],"%d:%d:%d.%u",
                &u32RelativeHour,
                &u32RelativeMinute,
                &u32RelativeSecond,
                &u32RelativeNSec) != 4)
            {
                fprintf( stderr, "Error: Cannot parse relative stop time <%s>\n", hStream->stop.pcTime );
                goto error; 
            }
            res = RTDExTs_GetHostPosixTime( &u32HostSec, &u32HostNSec);
            if ( FAILURE(res) )
            {
                goto error; 
            }
            hStream->stop.u32secTime = u32HostSec +
                u32RelativeHour * 3600 +
                u32RelativeMinute * 60 +
                u32RelativeSecond;
            hStream->stop.u32nsecTime = u32HostNSec +
                u32RelativeNSec;
        }
        // Treat absolute times
        else
        {
            res = RTDExTs_ReadableLocalTimeToPosix(
                hStream->stop.pcTime, 
                &hStream->stop.u32secTime, &hStream->stop.u32nsecTime);
            if ( FAILURE(res) )
            {
                fprintf( stderr, "Error: Cannot parse stop time (%s)\n", hStream->stop.pcTime );
                goto error;
            }
        }
    }

    // Configure the start mode the RTDExTs transfers.
    res = RTDExTs_Start( hRTDExTs,                                      // RTDExTs handle
                           hStream->start.mode,        // Start mode
                           hStream->start.trigSel,  // Start trigger selection (if OnTrigger mode)
                           hStream->start.u32secTime, 
                           hStream->start.u32nsecTime);                     // Start time (if AtTime mode)
    if ( FAILURE(res) ) goto error;

    // In some cases, we will want to program the stop condition as soon as possible.
    // In RX, it can be done anytime after the start.
    if ( (hStream->stop.mode == RTDExTs_TrigMode_OnTrigger)
      || (hStream->stop.mode == RTDExTs_TrigMode_AtTime))
    {
        // For these two, we can pre-programmed the stop condition
        res = RTDExTs_Stop( hRTDExTs,                                   // RTDExTs handle
                            hStream->stop.mode,        // Stop mode
                            hStream->stop.trigSel,  // Stop trigger selection (if OnTrigger mode)
                            hStream->stop.u32secTime,
                            hStream->stop.u32nsecTime);                     // Stop time (if AtTime mode)
        if ( FAILURE(res) ) goto error;
    }


    // Perform the transfers
    while ( !g_stopTask[u32ChanNum] )
    {
        uint32_t trSize = hStream->u32MaxTransferSize;

        // Limit the size of the next transfer (Stop Immediately after N)
        if ( hStream->stop.mode == RTDExTs_TrigMode_Immediately && ((uint32_t)trSize > (hStream->u32NumOfSamples-u32TotalTransfer)) )
        {
            trSize = hStream->u32NumOfSamples - u32TotalTransfer;
        }

        // Get an empty buffer
        pBuffDesc = Buffer_Get( freeBuffQ, 1 );
        if ( pBuffDesc == NULL ) goto error;

        // Read some samples
        res = RTDExTs_Receive( hRTDExTs, pBuffDesc->pMem, trSize, NUM_RTDEXTS_EVT, rtdexTsEvtQ, &u32NumRtdexTsEvt, 1 );
        if ( u32NumRtdexTsEvt ) DisplayEvent( "RX ", u32ChanNum, u32NumRtdexTsEvt, rtdexTsEvtQ );
        if ( FAILURE(res) ) goto error;

        pBuffDesc->i32Size = res * (2*sizeof(int16_t));
        u32TotalTransfer += res;

        // Put the sample in the fifo
        bufRes = Buffer_Put( fullBuffQ, pBuffDesc );
        pBuffDesc = NULL;
        if ( bufRes  < 0 ) goto error;

        // Look if we have reached the number of requested transfer
        if ( (hStream->stop.mode == RTDExTs_TrigMode_Immediately)
            && (u32TotalTransfer >= hStream->u32NumOfSamples) )
        {
            res = RTDExTs_Stop( hRTDExTs, RTDExTs_TrigMode_Immediately, RTDExTs_TrigSel_NA, 0, 0 );
            if ( FAILURE(res) ) goto error;
            break;
        }

        // Stop condition has probably been reached
        if ( (uint32_t)res < trSize )
        {
            res = RTDExTs_IsStopped( hRTDExTs, NUM_RTDEXTS_EVT, rtdexTsEvtQ,&u32NumRtdexTsEvt, 0 );
            if ( u32NumRtdexTsEvt ) DisplayEvent( "RX ", u32ChanNum, u32NumRtdexTsEvt, rtdexTsEvtQ );
            if ( FAILURE(res) ) goto error;
            if (res ) break;
        }
    }

    // Put a final empty buffer to signal to the File task that we are done
    pBuffDesc = Buffer_Get( freeBuffQ, 1 );
    if ( pBuffDesc == NULL ) goto error;
    pBuffDesc->i32Size = -1;    // EOF
    bufRes = Buffer_Put( fullBuffQ, pBuffDesc );
    pBuffDesc = NULL;
    if ( bufRes < 0 ) goto error;


    // Get an empty buffer
    pBuffDesc = Buffer_Get( freeBuffQ, 1 );
    if ( pBuffDesc == NULL ) goto error;

    // Optionnaly, wait for the streams to stop
    do
    {
        // We have sent the stop immediately at T = N sample but the readio received it only a T = N+M,
        // read the addisional M sample and discard them
        res = RTDExTs_Receive( hRTDExTs, pBuffDesc->pMem, hStream->u32MaxTransferSize, NUM_RTDEXTS_EVT, rtdexTsEvtQ, &u32NumRtdexTsEvt, 0 );
        if ( u32NumRtdexTsEvt ) DisplayStopEventOnly( "RX ", u32ChanNum, u32NumRtdexTsEvt, rtdexTsEvtQ );
        if ( FAILURE(res) ) goto error;

        res = RTDExTs_IsStopped( hRTDExTs, NUM_RTDEXTS_EVT, rtdexTsEvtQ, &u32NumRtdexTsEvt, 0 );
        if ( u32NumRtdexTsEvt ) DisplayStopEventOnly( "RX ", u32ChanNum, u32NumRtdexTsEvt, rtdexTsEvtQ );
        if ( FAILURE(res) ) goto error;
    } while( !res );

    // Put the sample in the fifo
    bufRes = Buffer_Put( freeBuffQ, pBuffDesc );
    pBuffDesc = NULL;
    if ( bufRes < 0 ) goto error;

    goto done;

error:
    if ( FAILURE(res) )
    {
        fprintf( stderr, "RX Task Error: %s\n\n\n", ErrorToString(res) );
        g_stopTask[u32ChanNum] = 1;
    }

done:
    if ( pBuffDesc != NULL )
    {
        // We still have a buffer, put it back in a queue before leaving
        Buffer_Put( freeBuffQ, pBuffDesc );
        pBuffDesc = NULL;
    }
    return (adp_thread_ret_t)0;
}

 /****************************************************************************
 * Function: SaveTask
 ************************************************************************//** 
 * 
 * Task to store the received data to a file.
 * 
 * @param param
 *      RTDEx handle.
 *
 * @return
 *      None.
 *
 ****************************************************************************/
static adp_thread_dec_t SaveTask( void *param )
{
    adp_result_t res = OK;
    int bufRes;
    Buffer_Desc_t *pBuffDesc = NULL;
    int n;
    FILE * fp;
    unsigned int u32ChanNum;
    Buffer_Queue_t freeBuffQ;
    Buffer_Queue_t fullBuffQ;

    // Get the channel number from thread paramter
    u32ChanNum = (uint32_t)(intptr_t)param;

    // Get the buffers according to the channel number
    freeBuffQ = g_aFreeBuffQ[u32ChanNum];
    fullBuffQ = g_aFullBuffQ[u32ChanNum];


    fp = fopen( hRTDExChannel[u32ChanNum]->pcFileName, "wb");
    if ( fp == NULL )
    {
        perror( "fopen");
        g_stopTask[u32ChanNum] = 1;
        goto done;
    }

    // Perform the save
    while ( 1 )
    {
        // Get a buffer to transmit
        pBuffDesc = Buffer_Get( fullBuffQ, 1 );
        if ( pBuffDesc == NULL ) goto error; 

        // Look for the end 
        if ( pBuffDesc->i32Size < 0 )
        {
            break;
        }
        
        n = (int)fwrite( pBuffDesc->pMem, 1, pBuffDesc->i32Size, fp ); 
        if ( n < 0 )
        {
            perror( "fwrite:" );
            g_stopTask[u32ChanNum] = 1;
            goto done;
        } 

        // Free the buffer
        bufRes = Buffer_Put( freeBuffQ, pBuffDesc );
        pBuffDesc = NULL;
        if ( bufRes  < 0 ) goto error; 
    }

    goto done;

error:
    if ( FAILURE(res) ) 
    {
        fprintf( stderr, "Save Task Error: %s\n\n\n", ErrorToString(res) );
        g_stopTask[u32ChanNum] = 1;
    }
    
done:
    // If we still have a buffer, put it back in a queue before leaving
    if ( pBuffDesc != NULL )
    {
        Buffer_Put( freeBuffQ, pBuffDesc );
        pBuffDesc = NULL;
    }

    if ( fp != NULL )
    {
        fclose( fp );
        fp = NULL;
    }
    return (adp_thread_ret_t)0;
}

 /****************************************************************************

 * Function : DisplayStopEventOnly
 ************************************************************************//**

 * 
 * Display RTDExTs stop events only.

 *
 * @param [in]  szPrefix
 *      String prefix

 *
 * @param [in]  u32EvtCount

 *      Number of event in the the queue.
 *

 * @param [in]  pEvtQueue
 *      Array of RTDExTs Events.
 *

 * @return
 *      None.      

 *
 ****************************************************************************/ 
static void DisplayStopEventOnly(  char               *szPrefix,
                                   uint32_t         u32ThreadNum,
                                   uint32_t           u32EvtCount,
                                   RTDExTs_Event_t  *pEvtQueue )
{
    while ( u32EvtCount-- )

    {
        if ( (uint32_t)pEvtQueue->id == RTDExTs_EventId_StopPending ||
            (uint32_t)pEvtQueue->id == RTDExTs_EventId_Stopped)
        {
            DisplayEvent(szPrefix, u32ThreadNum, 1, pEvtQueue);
        }
        pEvtQueue++;
    }

}

 /****************************************************************************
 * Function : DisplayEvent
 ************************************************************************//**
 * 
 * Display RTDExTs events.
 *
 * @param [in]  szPrefix
 *      String prefix
 *
 * @param [in]  u32EvtCount
 *      Number of event in the the queue.
 *
 * @param [in]  pEvtQueue
 *      Array of RTDExTs Events.
 *
 * @return
 *      None.      
 *
 ****************************************************************************/ 

 static void DisplayEvent(  char               *szPrefix,
                           uint32_t         u32ThreadNum,
                           uint32_t           u32EvtCount,
                           RTDExTs_Event_t  *pEvtQueue )
{
    static const char *szEvt[RTDExTs_EventId_Num] = 
    {
        "No event",                 // RTDExTs_EventId_None
        "Start pending",            // RTDExTs_EventId_StartPending
        "Started",                  // RTDExTs_EventId_Started
        "Stop pending",             // RTDExTs_EventId_StopPending
        "Stopped",                  // RTDExTs_EventId_Stopped
        "FIFO underrun",            // RTDExTs_EventId_FifoUnderrun
        "FIFO overrun",             // RTDExTs_EventId_FifoOverrun
        "FIFO resume",              // RTDExTs_EventId_FifoResume
        "Miss-timed command",       // RTDExTs_EventId_MissTimeCommand
    };
    char pcTimeString[50];

    while ( u32EvtCount-- )
    {    
        if ( pEvtQueue->id == RTDExTs_EventId_MissTimeCommand  )
        {
            RTDExTs_PosixToReadableLocalTime(
                pEvtQueue->u32secTime, pEvtQueue->u32nsecTime, 
                pcTimeString, sizeof(pcTimeString));

            RTDExTs_PosixToReadableLocalTime(
                pEvtQueue->u32secTimeCommand, pEvtQueue->u32nsecTimeCommand, 
                pcTimeString, sizeof(pcTimeString));

            LockMutex(&g_DisplayEventMutex);
            printf( "%s#%2u EVT #%2d: %21s : <%c>%s\n", 
                szPrefix,
                u32ThreadNum,
                pEvtQueue->u32EventCount, 
                szEvt[pEvtQueue->id],
                pEvtQueue->gTiming ? 'G' : 'P',
                pcTimeString);
            printf("                         Command time :    %s\n",pcTimeString);
            UnlockMutex(&g_DisplayEventMutex);
        }
        else if ( (uint32_t)pEvtQueue->id < RTDExTs_EventId_Num  )
        {
            RTDExTs_PosixToReadableLocalTime(
                pEvtQueue->u32secTime, pEvtQueue->u32nsecTime, 
                pcTimeString, sizeof(pcTimeString));

            LockMutex(&g_DisplayEventMutex);
            printf( "%s#%2u EVT #%2d: %21s : <%c>%s\n", 
                szPrefix,
                u32ThreadNum,
                pEvtQueue->u32EventCount, 
                szEvt[pEvtQueue->id],
                pEvtQueue->gTiming ? 'G' : 'P',
                pcTimeString);
            UnlockMutex(&g_DisplayEventMutex);
        }
        else
        {
            RTDExTs_PosixToReadableLocalTime(
                pEvtQueue->u32secTime, pEvtQueue->u32nsecTime, 
                pcTimeString, sizeof(pcTimeString));
            
            LockMutex(&g_DisplayEventMutex);
            printf( "%s#%2u EVT #%2d: Unknown event : <%c>%s\n", 
                szPrefix,
                u32ThreadNum,
                pEvtQueue->u32EventCount, 
                pEvtQueue->gTiming ? 'G' : 'P',
                pcTimeString);
            UnlockMutex(&g_DisplayEventMutex);
        }
        pEvtQueue++;
    }


}
