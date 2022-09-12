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
 * @file      RxStreaming.c
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
#include "mac_utils.h" 
#include "media_utils.h" 
#include "linux_utils.h"
#include "verbose_utils.h"
#include "terminal_utils.h"
#include "bas_error.h"
#include "bas_time.h"
#include "bas_buffer.h"
#include "bas_semaphore.h"
#include "bas_thread.h"
#include "RxStreaming.h"

// WARNING: to run this demo under LINUX, you may need root privilege

connection_state *global_state;

/**
 * RTDEx handles
 */
adp_handle_rtdex_conn g_ahRTDEx[MAX_RTDEX] = {NULL};
handle_rtdex_channel hRTDExChannel[MAX_RTDEX] = {NULL};
handle_streaming hStreaming;
eMediaType eMedia;
RTDExPerseusParamsPcie perseusParams[MAX_PERSEUS];
float fDataRate[MAX_RTDEX] = {0.0};
uint32_t u32TotalBytesTransfered[MAX_RTDEX] = {0};
float fTotalThroughput = 0;

/**
 * Task
 */
static adp_thread_t g_atRTDExTask[MAX_RTDEX];
static adp_thread_t g_atFileTask[MAX_RTDEX];
static adp_thread_t g_atDisplayTask;
static int g_stopTask[MAX_RTDEX] = {0};
static int g_stopDislayTask = 0;

/**
 * Buffer queues
 */
Buffer_Queue_t g_aFreeBuffQ[MAX_RTDEX] = {NULL};
Buffer_Queue_t g_aFullBuffQ[MAX_RTDEX] = {NULL};

/**
 * Mutex and counter to synchronize all the TX threads
 */
adp_semaphore_t g_aSem[MAX_RTDEX];
static unsigned int g_RtdexTransferDoneCnt;
adp_mutex_t g_RtdexTransferDoneMutex;
adp_mutex_t g_TasksDoneMutex;
adp_mutex_t g_RxTaskDoneMutex;

static adp_result_t PrintRxStreamingStats(uint32_t u32ChannelNumber, eMediaType eMedia);
static adp_thread_dec_t RxTask( void *param );
static adp_thread_dec_t SaveTask( void *param );
static adp_thread_dec_t DisplayTask( void *param );

/************************************************************************************************************************
											   RxStreaming 
************************************************************************************************************************/
Result RxStreaming(char ** pcIpAddresses, uint32_t NbIpAddresses, handle_streaming handle_rxstreaming, handle_rtdex_channel * handle_rxchannel)
{
	adp_result_t res;	
	uint32_t i, u32ChNum, u32TotalChNum;

	connection_state state[MAX_PERSEUS];
	connection_state * pStateTmp;

    unsigned int uiRTDExBaseAddr;

	// local to global for threads
	//hStreaming = handle_rxstreaming;	

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
													   Detecting Media
	************************************************************************************************************************/
	for(i = 0 ; i < NbIpAddresses ; i++)
	{
		bas_printf(INFO, "    - Perseus Ip Address : %s\n", pcIpAddresses[i] );

		// Detecting media
		res = DetectMedia(&(state[i]), &eMedia);
		if(FAILURE(res))
		{
			goto done;
		}

		// Printing media
		if(eMedia == eMediaEth)
		{
			bas_printf(INFO, "\t - RTDEx Media is Gigabit Ethernet\n"  );
		}
		else if(eMedia == eMediaPcie)
		{
			bas_printf(INFO, "\t - RTDEx Media is PCI Express\n"  );
		}
		else
		{
			bas_printf(INFO, "\t - Unsupported RTDEx Media.\n" );
			goto done;
		}
	}

	/************************************************************************************************************************
													Parameter validation
	************************************************************************************************************************/
	//BurstSize must be a multiple of FrameSize
	if (handle_rxstreaming->u32BurstSize % handle_rxstreaming->u32FrameSize != 0)
	{
		bas_printf(ERR,"Burst Size (%u bytes) is not a multiple of Frame Size (%u bytes)\n", handle_rxstreaming->u32BurstSize, handle_rxstreaming->u32FrameSize);
		res = RTDEXTRANSFERSIZENOTFRAMESIZEMULTIPLE;
		goto done;
	}

	/************************************************************************************************************************
												Streaming Configuration Display
	************************************************************************************************************************/
	bas_printf(INFO,"\n    - General streaming parameters\n");
	bas_printf(INFO,"\t - RTDEx frame size : %u bytes\n",handle_rxstreaming->u32FrameSize);
	bas_printf(INFO,"\t - RTDEx queue size : %u\n",handle_rxstreaming->u32QueueSize);
	bas_printf(INFO,"\t - RTDEx transfer size : %u bytes\n",handle_rxstreaming->u32TransferSize);
	bas_printf(INFO,"\t - RTDEx burst size : %u bytes\n",handle_rxstreaming->u32BurstSize);
	bas_printf(INFO,"\t - Real time flag : %u\n",handle_rxstreaming->u8RealTimeFlag);
	bas_printf(INFO,"\t - Display stats flag : %u\n",handle_rxstreaming->u8DisplayStatsFlag);
	bas_printf(INFO,"\n");

	/************************************************************************************************************************
												Channels Configuration Display
	************************************************************************************************************************/
	u32TotalChNum = handle_rxstreaming->u32NumberOfChannels;
	bas_printf(INFO,"    - Total channels number : %u\n",u32TotalChNum);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		bas_printf(INFO,"\t - Channel : #%u\n",handle_rxchannel[u32ChNum]->u8ChannelNumber);
		bas_printf(INFO,"\t   - Carrier position : #%u\n",handle_rxchannel[u32ChNum]->u8CarrierPosition);
		bas_printf(INFO,"\t   - Carrier IP address : %s\n",pcIpAddresses[handle_rxchannel[u32ChNum]->u8CarrierPosition-1]);
		bas_printf(INFO,"\t   - Filename : %s\n",handle_rxchannel[u32ChNum]->pcFileName);		
	}	

	for(i = 0 ; i < NbIpAddresses ; i++)
	{
		pStateTmp = &state[i];

		if (eMedia == eMediaEth)
		{
            res = Carrier_GetModuleBaseAddr_send( pStateTmp, Carrier_eModuleRTDExGigE, 0, &uiRTDExBaseAddr);
            if(FAILURE(res))
			{
				goto done;
			}
			// reset the RTDEx core
            res = RTDExResetCoreEth( pStateTmp, uiRTDExBaseAddr);
			if(FAILURE(res))
			{
				goto done;
			}

			// Set frame gap for FPGA transmitter
            res = RTDExSetTxFrameGapValEth( pStateTmp, uiRTDExBaseAddr, handle_rxstreaming->u32FrameGap );
			if(FAILURE(res))
			{
				goto done;
			}
		}
		else if(eMedia == eMediaPcie)
		{
			// reset the RTDEx core
            res = RTDExInitParamsPcie(&perseusParams[i], pStateTmp);
			if(FAILURE(res))
			{
				goto done;
			}

            res = RTDExResetCorePcie(&perseusParams[i]);
			if(FAILURE(res))
			{
				goto done;
			}
		}
	}

	/************************************************************************************************************************
												Open the RTDEx Interfaces
	************************************************************************************************************************/
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		uint32_t uiCarrierPosTmp = handle_rxchannel[u32ChNum]->u8CarrierPosition-1;
		pStateTmp = &state[uiCarrierPosTmp];

		if (eMedia == eMediaEth)
		{
            res = Carrier_GetModuleBaseAddr_send( pStateTmp, Carrier_eModuleRTDExGigE, 0, &uiRTDExBaseAddr);
            if(FAILURE(res))
			{
				goto done;
			}

            res = RTDExOpenHostAndFpgaEth( &g_ahRTDEx[u32ChNum],
                            pStateTmp,
                            uiRTDExBaseAddr,
                            handle_rxchannel[u32ChNum]->u8ChannelNumber,
                            eFromFpga, // eToFpga or eFromFpga
                            eContinuous); // eSingle, eContinuous

			if(FAILURE(res))
			{
                bas_printf(ERR,"Open failure\n\n");
				goto done;
			}			
		}
		else if(eMedia == eMediaPcie)
		{
			// Open RTDEx channel
			res = RTDExOpenHostAndFpgaPcie( &g_ahRTDEx[u32ChNum], 
										    &perseusParams[uiCarrierPosTmp], 
											handle_rxchannel[u32ChNum]->u8ChannelNumber, 
											eFromFpga, 
											eContinuous);
			if(FAILURE(res))
			{
				goto done;
			}
		}
	}

	/************************************************************************************************************************
											Alloc a buffer to store the IQ samples.
	************************************************************************************************************************/
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		// local to global for threads
		hRTDExChannel[u32ChNum] = handle_rxchannel[u32ChNum];

		res = Buffer_InitQueue( &g_aFreeBuffQ[u32ChNum], handle_rxstreaming->u32QueueSize, handle_rxstreaming->u32BurstSize );
        if(FAILURE(res))
		{
			goto done;
		}

		res = Buffer_InitQueue( &g_aFullBuffQ[u32ChNum], 0, handle_rxstreaming->u32BurstSize );
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
			bas_printf(ERR, "Failed to set process priority class to real-time.\n" );
		}
    }
#endif
	g_RtdexTransferDoneCnt = 0;

	// Init the RTDEx transfer done counter mutex
    res = InitMutex(&g_RtdexTransferDoneMutex);
    if(FAILURE(res))
	{
		goto done;
	}
    res = InitMutex(&g_TasksDoneMutex);
    if(FAILURE(res))
	{
		goto done;
	}

	res = InitMutex(&g_RxTaskDoneMutex);
    if(FAILURE(res))
	{
		goto done;
	}
	
	// Init all thread mutex
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		res = InitSemaphore(&g_aSem[u32ChNum],1);
		if(FAILURE(res))
		{
			goto done;
		}
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

	// Let time to the first thread to start and be ready to receive data
    Sleep(1000);
    global_state = state;
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		res = StartThread(&g_atRTDExTask[u32ChNum], RxTask, (void *)(intptr_t)u32ChNum, 1);
		if(FAILURE(res))
		{
			goto done;
		}		
	}
	
	g_stopDislayTask = 0;
	//Start Display Thread
	res = StartThread(&g_atDisplayTask, DisplayTask, (void *)(intptr_t)u32TotalChNum, 6);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		// Wait for the end of the file task
        JoinThread( g_atFileTask[u32ChNum] );
        // Then for the end of the RTDEx task
        JoinThread( g_atRTDExTask[u32ChNum] );		
	}

	// Disable Display Data rate
	g_stopDislayTask = 1;
	// Then for the end of the display task
	//JoinThread( &g_atDisplayTask );

	// Print stats and calculate combined throughput
	if (handle_rxstreaming->u8DisplayStatsFlag)
	{
        bas_printf(INFO, "Stats\n");
		for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
		{
			res = PrintRxStreamingStats(u32ChNum, eMedia);
			if(FAILURE(res))
			{
				goto done;
			}
		}
	}
    
    for(i = 0; i < MAX_RTDEX; i++)
    {
        if(fDataRate[i] != -1)
            fTotalThroughput += fDataRate[i];
        else
        {
            fTotalThroughput = -1;
            break;
        }
    }
    if(fTotalThroughput != -1)
	{
        bas_printf(INFO, "\n  - Full from FPGA throughput:%8.3f MB/s \n", fTotalThroughput);
	}
    else
	{
        bas_printf(INFO, "\n  - Test time too short to calculate full from FPGA throughput. \n");
	}

	// Stop Transfer
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
        if(eMedia == eMediaPcie)
		{
            RTDExStopPcie(g_ahRTDEx[u32ChNum]);
		}
	}

	res = 0;	

done :	
	if(FAILURE(res))
    {
        fprintf( stderr, "%s\n\n\n", ErrorToString(res) );
    }
	fflush(stdout);
	// Clean up
	DestroyMutex(&g_RtdexTransferDoneMutex);
	
	DestroyMutex(&g_RxTaskDoneMutex);
	
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		DestroySemaphore(&g_aSem[u32ChNum]);

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
		if ( g_ahRTDEx[u32ChNum] != NULL )
		{
			RTDExClose( g_ahRTDEx[u32ChNum] );
			g_ahRTDEx[u32ChNum] = NULL;
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
												Static function definition
************************************************************************************************************************/
static adp_result_t PrintRxStreamingStats(uint32_t u32ChannelNumber, eMediaType eMedia)
{
    adp_result_t res;
    unsigned int value;

    if(eMedia == eMediaEth)
    {
        res = RTDExGetRxFramesCountEth(g_ahRTDEx[u32ChannelNumber],&value);    
        if(FAILURE(res))
        {
			return res;
        }
        bas_printf(INFO, "  - RTDEx Packets received (From FPGA, Ch %d): %d\n", hRTDExChannel[u32ChannelNumber]->u8ChannelNumber, value);
        res = RTDExGetRxFramesLostCountEth(g_ahRTDEx[u32ChannelNumber],&value);
        if(FAILURE(res))
        {
           return res;
        }
        bas_printf(INFO, "  - Packets lost (From FPGA, Ch %d): %d\n", hRTDExChannel[u32ChannelNumber]->u8ChannelNumber, value);
    }
    else if (eMedia == eMediaPcie)
    {
         RTDExGetRxFrameCountPcie(g_ahRTDEx[u32ChannelNumber],&value);
         bas_printf(INFO, "  - RTDEx packets received (From FPGA, Ch %d): %d\n", hRTDExChannel[u32ChannelNumber]->u8ChannelNumber, value);
    }
    else
    {
        bas_printf(INFO, "Invalid media for statistics\n");
    }

    bas_printf(INFO, "  - Byte(s) received by host (From FPGA, Ch %d):%10u \n\n", hRTDExChannel[u32ChannelNumber]->u8ChannelNumber, u32TotalBytesTransfered[u32ChannelNumber]);

    return 0;
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
    adp_result_t res = RTDEXOK;
    int bufRes;
    uint32_t u32TotalTransfer = 0;
    uint32_t u32NumRtdexSyncEvt = 0;
    Buffer_Desc_t *pBuffDesc = NULL;
    adp_handle_rtdex_conn hRTDEx;
	handle_streaming hStream;
	handle_rtdex_channel hChan;
    unsigned int u32ChanNum;
    Buffer_Queue_t freeBuffQ;
    Buffer_Queue_t fullBuffQ;
	uint32_t uRTDExBurstTransferSize;

	// Data rate calculation
	unsigned long long streamingstart;  // streaming start time
    unsigned long long streamingstop;   // streaming stop time
	float transferlength = 0;           // transfer length
    float transfersize = 0;             // transfer size

    // Get the channel number from thread parameter
    u32ChanNum = (uint32_t)(intptr_t)param;

    // Get the RTDEx handle and buffers according to the channel number
	hStream = hStreaming;
    hRTDEx = g_ahRTDEx[u32ChanNum];
	hChan = hRTDExChannel[u32ChanNum];
    freeBuffQ = g_aFreeBuffQ[u32ChanNum];
    fullBuffQ = g_aFullBuffQ[u32ChanNum];

	LockMutex(&g_RxTaskDoneMutex);

	if(eMedia == eMediaEth)
    {        
        // reset RTDEx fifos
        res = RTDExResetFifoEth(hRTDEx);
        if ( FAILURE(res) ) goto error; 
	}

    res = RTDExStart(hRTDEx, hStream->u32FrameSize , 0);
    if ( FAILURE(res) ) goto error;

    UnlockMutex(&g_RxTaskDoneMutex);
		
	// Start Tick Count
	streamingstart = GetTickCount();

    // Perform the transfers
    while ( !g_stopTask[u32ChanNum] )
    {       

        if(hStream->u32TransferSize - u32TotalTransfer < hStream->u32BurstSize ) // If remaining to transfer if smaller than BURST_SIZE
        {
            uRTDExBurstTransferSize = hStream->u32TransferSize - u32TotalTransfer;
        }
        else
        {
            uRTDExBurstTransferSize = hStream->u32BurstSize;
        }

        // Get an empty buffer
        pBuffDesc = Buffer_Get( freeBuffQ, 1 );
        if ( pBuffDesc == NULL ) goto error; 

        // Receive data from RTDEx
        res = RTDExReceive(hRTDEx, pBuffDesc->pMem, uRTDExBurstTransferSize, eRTDExWaitTimeout, 5000);
		if(FAILURE(res))        
        {
			res = RTDExGetLastError(hRTDEx);
            goto error;
        }

        pBuffDesc->i32Size = res;
        u32TotalTransfer += res;

		// Calculate data rate
		streamingstop = GetTickCount(); // get current time
		transferlength = 1000*((float)(streamingstop - streamingstart)); // get length of transfer
		transfersize = ((float)u32TotalTransfer);   // get transfer size in bytes
		//fDataRate[u32ChanNum] = (transfersize / transferlength) /1000000;
        
        if (transferlength > 0)
            fDataRate[u32ChanNum] = transfersize/ transferlength;
        else
            fDataRate[u32ChanNum] = -1;

        // Put the sample in the fifo 
        bufRes = Buffer_Put( fullBuffQ, pBuffDesc );
        pBuffDesc = NULL;
        if ( bufRes  < 0 ) goto error; 

        u32TotalBytesTransfered[u32ChanNum] = u32TotalTransfer;

		if (u32TotalTransfer >= hStream->u32TransferSize)
		{
			break;
		}
    }   

    // Put a final empty buffer to signal to the File task that we are done
    pBuffDesc = Buffer_Get( freeBuffQ, 1 );
    if ( pBuffDesc == NULL ) goto error;     
    pBuffDesc->i32Size = -1;    // EOF
    bufRes = Buffer_Put( fullBuffQ, pBuffDesc );
    pBuffDesc = NULL;
    if ( bufRes < 0 ) goto error;
	
    goto done;

error:
    if ( FAILURE(res) ) 
    {
        fprintf( stderr, "RX Task Error: %s\n\n\n", ErrorToString(res) );
        LockMutex(&g_TasksDoneMutex);
        g_stopTask[u32ChanNum] = 1;
        UnlockMutex(&g_TasksDoneMutex);
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
        LockMutex(&g_TasksDoneMutex);
        g_stopTask[u32ChanNum] = 1;
        UnlockMutex(&g_TasksDoneMutex);
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
            LockMutex(&g_TasksDoneMutex);
            g_stopTask[u32ChanNum] = 1;
            UnlockMutex(&g_TasksDoneMutex);
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
        LockMutex(&g_TasksDoneMutex);
        g_stopTask[u32ChanNum] = 1;
        UnlockMutex(&g_TasksDoneMutex);
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
 * Function: DisplayTask
 ************************************************************************//** 
 * 
 * Task to display the data rate in MBps
 * 
 * @param param
 *      RTDEx handle.
 *
 * @return
 *      None.
 *
 ****************************************************************************/
 static adp_thread_dec_t DisplayTask( void *param )
 {
	 unsigned int u32TotalChanNum;
	 int32_t iCurrentLine;
	 uint32_t i,k;
	 handle_rtdex_channel hChan;
     int cursorToBeReset = 0;

	 // Get the total channel number from thread paramter
     u32TotalChanNum = (uint32_t)(intptr_t)param;

	 printf("Carrier    :  Channel       DataRate\n");

	 while(!g_stopDislayTask)
	 {
   	    if(_kbhit())
            {
                _getch();
                LockMutex(&g_TasksDoneMutex);
                for(k = 0; k < MAX_RTDEX; k++)
                {
                    g_stopTask[k] = 1;
                }
                UnlockMutex(&g_TasksDoneMutex);
                break;
            }

         if(cursorToBeReset)
         {
             iCurrentLine = GetCursorLine();
		     SetCursorPosition( 0, iCurrentLine - u32TotalChanNum );
         }
		 for ( i = 0 ; i < u32TotalChanNum ; i++)
		 {
			 hChan = hRTDExChannel[i];
             if(fDataRate[i] != -1)
             {
			     printf("   #%u      :    #%u          %2.2fMBps\n",
					    hChan->u8CarrierPosition,
					    hChan->u8ChannelNumber,
					    fDataRate[i]);
             }
             else
             {
                 printf("Test time too short to calculate rate \n");
             }
		 }
		 Sleep(1000);
         cursorToBeReset = 1;
	 }

	 return (adp_thread_ret_t)0;
 }
