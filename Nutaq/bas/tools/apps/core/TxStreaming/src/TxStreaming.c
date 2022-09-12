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
 * @file      TxStreaming.c
 * 
 * @brief     Function for TX Streaming
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
#include "terminal_utils.h"
#include "bas_error.h"
#include "bas_time.h"
#include "bas_buffer.h"
#include "bas_semaphore.h"
#include "bas_thread.h"
#include "TxStreaming.h"

#define	FIFOALMOSTFULLTHRESHOLD 0.75
#define	FIFOREENABLEFLOWTHRESHOLD 0.5
// WARNING: to run this demo under LINUX, you may need root privilege

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
static int g_stopDisplayTask = 0;

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
adp_mutex_t g_DataRateMutex[MAX_RTDEX];

static adp_result_t PrintTxStreamingStats(uint32_t u32ChannelNumber, eMediaType eMedia);
static adp_thread_dec_t TxTask( void *param );
static adp_thread_dec_t ReadTask( void *param );
static adp_thread_dec_t DisplayTask( void *param );

/************************************************************************************************************************
											   TxStreaming 
************************************************************************************************************************/
Result TxStreaming(char ** pcIpAddresses, uint32_t NbIpAddresses, handle_streaming handle_txstreaming, handle_rtdex_channel * handle_txchannel)
{
	adp_result_t res;	
	uint32_t i, u32ChNum, u32TotalChNum;

	connection_state state[MAX_PERSEUS];
	connection_state * pStateTmp;
	uint32_t u32StatsIsIncluded;
	uint32_t u32RTDExFifoDepth;

    unsigned int uiRTDExBaseAddr;

	// local to global for threads
	hStreaming = handle_txstreaming;	

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
		printf( "    - Perseus Ip Address : %s\n", pcIpAddresses[i] );

		// Detecting media
		res = DetectMedia(&(state[i]), &eMedia);
		if(FAILURE(res))
		{
			goto done;
		}

		// Printing media
		if(eMedia == eMediaEth)
		{
			printf( "\t - RTDEx Media is Gigabit Ethernet\n"  );
		}
		else if(eMedia == eMediaPcie)
		{
			printf( "\t - RTDEx Media is PCI Express\n"  );
		}
		else
		{
			printf( "\t - Unsupported RTDEx Media.\n" );
			goto done;
		}
	}

	/************************************************************************************************************************
													Parameter validation
	************************************************************************************************************************/
	//BurstSize must be a multiple of FrameSize
	if (handle_txstreaming->u32BurstSize % handle_txstreaming->u32FrameSize != 0)
	{
		printf("Burst Size (%u bytes) is not a multiple of Frame Size (%u bytes)\n", handle_txstreaming->u32BurstSize, handle_txstreaming->u32FrameSize);
		res = RTDEXTRANSFERSIZENOTFRAMESIZEMULTIPLE;
		goto done;
	}

	/************************************************************************************************************************
												Streaming Configuration Display
	************************************************************************************************************************/
	printf("\n    - General streaming parameters\n");
	printf("\t - RTDEx frame size : %u bytes\n",handle_txstreaming->u32FrameSize);
	printf("\t - RTDEx queue size : %u\n",handle_txstreaming->u32QueueSize);
	printf("\t - RTDEx transfer size : %u bytes\n",handle_txstreaming->u32TransferSize);
	printf("\t - RTDEx burst size : %u bytes\n",handle_txstreaming->u32BurstSize);
	printf("\t - Real time flag : %u\n",handle_txstreaming->u8RealTimeFlag);
	printf("\t - Display stats flag : %u\n",handle_txstreaming->u8DisplayStatsFlag);
	printf("\n");

	/************************************************************************************************************************
												Channels Configuration Display
	************************************************************************************************************************/
	u32TotalChNum = handle_txstreaming->u32NumberOfChannels;
	printf("    - Total channels number : %u\n",u32TotalChNum);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		printf("\t - Channel : #%u\n",handle_txchannel[u32ChNum]->u8ChannelNumber);
		printf("\t   - Carrier position : #%u\n",handle_txchannel[u32ChNum]->u8CarrierPosition);
		printf("\t   - Carrier IP address : %s\n",pcIpAddresses[handle_txchannel[u32ChNum]->u8CarrierPosition-1]);
		printf("\t   - Filename : %s\n",handle_txchannel[u32ChNum]->pcFileName);		
	}	

	for(i = 0 ; i < NbIpAddresses ; i++)
	{
		pStateTmp = &state[i];
        res = Carrier_GetModuleBaseAddr_send( pStateTmp, Carrier_eModuleRTDExGigE, 0, &uiRTDExBaseAddr);
		if (eMedia == eMediaEth)
		{

			// reset the RTDEx core
            res = RTDExResetCoreEth( pStateTmp, uiRTDExBaseAddr);
			if(FAILURE(res))
			{
				goto done;
			}

			res = RTDExGetRxConfigExtentedEth( pStateTmp, uiRTDExBaseAddr, &u32RTDExFifoDepth, &u32StatsIsIncluded );
			if( FAILURE(res) )
			{
				goto done; 
			}

			res = RTDExEnableFlowControlEth( pStateTmp, uiRTDExBaseAddr, (uint32_t)(FIFOALMOSTFULLTHRESHOLD * u32RTDExFifoDepth), (uint32_t)(FIFOREENABLEFLOWTHRESHOLD * u32RTDExFifoDepth) );
			if( FAILURE(res) )
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
		uint32_t uiCarrierPosTmp = handle_txchannel[u32ChNum]->u8CarrierPosition-1;
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
                                      handle_txchannel[u32ChNum]->u8ChannelNumber,
                                      eToFpga, // eToFpga or eFromFpga
                                      eContinuous); // eSingle, eContinuous

			if(FAILURE(res))
			{
				goto done;
			}			
		}
		else if(eMedia == eMediaPcie)
		{
			// Open RTDEx channel
			res = RTDExOpenHostAndFpgaPcie( &g_ahRTDEx[u32ChNum], 
										    &perseusParams[uiCarrierPosTmp], 
											handle_txchannel[u32ChNum]->u8ChannelNumber, 
											eToFpga, 
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
		hRTDExChannel[u32ChNum] = handle_txchannel[u32ChNum];

		res = Buffer_InitQueue( &g_aFreeBuffQ[u32ChNum], handle_txstreaming->u32QueueSize, handle_txstreaming->u32BurstSize );
        if(FAILURE(res))
		{
			goto done;
		}

		res = Buffer_InitQueue( &g_aFullBuffQ[u32ChNum], 0, handle_txstreaming->u32BurstSize );
		if(FAILURE(res))
		{
			goto done;
		}
	}

	/************************************************************************************************************************
												Create the working thread
	************************************************************************************************************************/
#if defined(_WIN32) || defined(_WIN64)
    if ( handle_txstreaming->u8RealTimeFlag )
    {
        res = SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
        if ( res == 0 ) 
		{
			printf( "Failed to set process priority class to real-time.\n" );
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


	// Init all thread mutex
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		res = InitSemaphore(&g_aSem[u32ChNum],1);
		if(FAILURE(res))
		{
			goto done;
		}
        res = InitMutex(&g_DataRateMutex[u32ChNum]);
        if(FAILURE(res))
	    {
		    goto done;
	    }
	}
	
	// Starting working threads
	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		res = StartThread(&g_atFileTask[u32ChNum], ReadTask, (void *)(intptr_t)u32ChNum, 2);
		if(FAILURE(res))
		{
			goto done;
		}		
	}

	// Let time to the first thread to start and be ready receive data
    Sleep(1000);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
    {
        res = RTDExStart( g_ahRTDEx[u32ChNum], hStreaming->u32FrameSize, hStreaming->u32TransferSize);
	    if ( FAILURE(res) )
	    {
		    res = RTDExGetLastError(g_ahRTDEx[u32ChNum]);
		    goto done;
	    }
    }

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		res = StartThread(&g_atRTDExTask[u32ChNum], TxTask, (void *)(intptr_t)u32ChNum, 1);
		if(FAILURE(res))
		{
			goto done;
		}		
	}
	
	g_stopDisplayTask = 0;
	//Start Display Thread
	res = StartThread(&g_atDisplayTask, DisplayTask, (void *)(intptr_t)u32TotalChNum, 6);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		// Wait for the end of the file task
        JoinThread( g_atFileTask[u32ChNum] );
        // Then for the end of the RTDEx task
        JoinThread( g_atRTDExTask[u32ChNum] );		
	}
    printf("\nThreads stopped\n");
	// Disable Display Data rate
	g_stopDisplayTask = 1;
	// Then for the end of the display task
	//JoinThread( &g_atDisplayTask );

	// Print stats
	if (handle_txstreaming->u8DisplayStatsFlag)
	{
        printf( "Stats\n");
		for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
		{
			res = PrintTxStreamingStats(u32ChNum, eMedia);
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
        printf( "\n  - Full to FPGA throughput:%8.3f MB/s \n", fTotalThroughput);
    else
        printf( "\n  - Test time too short to calculate full to FPGA throughput. \n");

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

    //Sleeping 100 ms before closing and resetting the RTDEx core, so that 
    //it has enough time to empty its FIFO
    Sleep(100);

	// Clean up
	DestroyMutex(&g_RtdexTransferDoneMutex);

	for(u32ChNum = 0 ; u32ChNum < u32TotalChNum ; u32ChNum++)
	{
		DestroySemaphore(&g_aSem[u32ChNum]);
        DestroyMutex(&g_DataRateMutex[u32ChNum]);

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

/****************************************************************************
 * Function : ReadBinFile
 ************************************************************************//**
 * 
 * Read a binary file with wrapping.
 *
 * @param [in] fp
 *      File pointer.
 *
 * @param [out] pu8Buffer
 *      Address of the buffer that receives the read data.
 *
 * @param [in] iNum
 *      Number of bytes to read.
 *
 * @param [in] bWrap
 *      Wrap flag. If 0, the function returns the number of bytes read at the 
 *      end of the file. Otherwise, it restarts at the beginning of the file 
 *      until the requested amount of data has been read.
 *
 * @return
 *      Number of bytes read. It might be smaller than the number of requested 
 *      bytes if the end of the file has been reached and bWrap was 0.   
 *
 ****************************************************************************/ 

static int ReadBinFile( FILE *fp, uint8_t *pu8Buffer, int iNum, int bWrap )
{
    int read = 0;
    while ( read < iNum )
    {
        int n = (int)fread( pu8Buffer + read, sizeof(uint8_t), iNum - read, fp );
        if ( n < 0 ) return n;
        if ( n < (iNum - read) )
        {
            if ( !bWrap ) return read + n;
            fseek( fp, 0, SEEK_SET );
        }

        read += n;
    }
    return read;
}

/************************************************************************************************************************
												Static function definition
************************************************************************************************************************/
static adp_result_t PrintTxStreamingStats(uint32_t u32ChannelNumber, eMediaType eMedia)
{
    adp_result_t res;
    unsigned int value;

    if(eMedia == eMediaEth)
    {
        res = RTDExGetTxFramesCountEth(g_ahRTDEx[u32ChannelNumber], &value);    
        if(FAILURE(res))
        {
			return res;
        }
        printf( "  - RTDEx Packets transmitted (To FPGA, Ch %d): %d\n", hRTDExChannel[u32ChannelNumber]->u8ChannelNumber, value);
        res = RTDExGetRxFramesLostCountEth(g_ahRTDEx[u32ChannelNumber], &value);
        if(FAILURE(res))
        {
           return res;
        }
        printf( "  - Packets lost (To FPGA, Ch %d): %d\n", hRTDExChannel[u32ChannelNumber]->u8ChannelNumber, value);
    }
    else if (eMedia == eMediaPcie)
    {
         RTDExGetTxFrameCountPcie(g_ahRTDEx[u32ChannelNumber], &value);
         printf( "  - RTDEx Packets transmitted (To FPGA, Ch %d): %d\n", hRTDExChannel[u32ChannelNumber]->u8ChannelNumber, value);
    }
    else
    {
        printf( "Invalid media for statistics\n");
    }

    printf( "  - Byte(s) transmitted by host (To FPGA, Ch %d):%10u \n\n", hRTDExChannel[u32ChannelNumber]->u8ChannelNumber, u32TotalBytesTransfered[u32ChannelNumber]);

    return 0;
}


/************************************************************************************************************************
														Working Tasks
************************************************************************************************************************/

/****************************************************************************
 * Function: TxTask
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
static adp_thread_dec_t TxTask( void *param )
{
    adp_result_t res = RTDEXOK;
    int bufRes;
    int chStarted = 0;
    uint32_t u32TotalTransfer = 0;
    uint32_t u32NumRtdexSyncEvt = 0;
    Buffer_Desc_t *pBuffDesc = NULL;
	adp_handle_rtdex_conn hRTDEx;
	handle_streaming hStream;
	handle_rtdex_channel hChan;
    unsigned int u32ChanNum;
    Buffer_Queue_t freeBuffQ;
    Buffer_Queue_t fullBuffQ;
    unsigned int k;

    int printfdone = 0;

	// Data rate calculation
	unsigned long long streamingstart;  // streaming start time
    unsigned long long streamingstop;   // streaming stop time
	float transferlength = 0;           // transfer length
    float transfersize = 0;             // transfer size

    // Get the channel number from thread paramter
    u32ChanNum = (uint32_t)(intptr_t)param;

    // Get the RTDEx handle and buffers according to the channel number
	hStream = hStreaming;
    hRTDEx = g_ahRTDEx[u32ChanNum];
	hChan = hRTDExChannel[u32ChanNum];
    freeBuffQ = g_aFreeBuffQ[u32ChanNum];
    fullBuffQ = g_aFullBuffQ[u32ChanNum];

	// Start Tick Count
	streamingstart = GetTickCount();

    // Perform the transfers 
    while ( !g_stopTask[u32ChanNum] )
    {
        uint32_t trSize;
        
 
        // Get a buffer to transmit
        pBuffDesc = Buffer_Get( fullBuffQ, 1 );
		if ( pBuffDesc == NULL ) goto error;

        // Look for the end 
        if ( pBuffDesc->i32Size < 0 )
        {
            // Free the buffer
            bufRes = Buffer_Put( freeBuffQ, pBuffDesc );
            pBuffDesc = NULL;
			if ( bufRes  < 0 ) goto error;

            break;
        }

        // Get the transfer size from the buffer descriptor
        //trSize = pBuffDesc->i32Size / (2*sizeof(int16_t));
        trSize = pBuffDesc->i32Size;

        // Limit the size of the next transfer (Stop Immediately after N)
		if ( hStream->u32TransferSize && ((uint32_t)trSize > (hStream->u32TransferSize - u32TotalTransfer)) )
        {
            trSize = hStream->u32TransferSize - u32TotalTransfer;
        }

        /*
         * Wait for the thread semaphore.
         * This is done to make sure every thread sends the same amount of samples
         * and thread does not jam the data pipe while the other thread starves.
         * This must be done since the flow control mechanism is global to all the RTDEx channels.
         */
        WaitSemaphore(&g_aSem[u32ChanNum], 1);
        // Send the data
		res = RTDExSend( hRTDEx, pBuffDesc->pMem, trSize);
		if ( FAILURE(res) ) 
		{
			res = RTDExGetLastError(hRTDEx);
			goto error; 
		}

        /*
         * Increment transfer done counter and verify if all the transfer has been done.
         * If so, release the mutex for all the threads.
         */
        LockMutex(&g_RtdexTransferDoneMutex);
		if ( ++g_RtdexTransferDoneCnt == hStream->u32NumberOfChannels )
        {
            g_RtdexTransferDoneCnt = 0;

            // Unlock all mutex
            for(k = 0; k < hStream->u32NumberOfChannels; k++)
            {
                PostSemaphore(&g_aSem[k]);
            }
        }
        UnlockMutex(&g_RtdexTransferDoneMutex);

        u32TotalTransfer += res;
		streamingstop = GetTickCount(); // get current time
		transferlength = 1000*((float)(streamingstop - streamingstart)); // get length of transfer
		transfersize = ((float)u32TotalTransfer);   // get transfer size in bytes

        LockMutex(&g_DataRateMutex[u32ChanNum]);
        if (transferlength > 0)
            fDataRate[u32ChanNum] = transfersize/ transferlength;
        else
            fDataRate[u32ChanNum] = -1;
        UnlockMutex(&g_DataRateMutex[u32ChanNum]);

        // Free the buffer
        bufRes = Buffer_Put( freeBuffQ, pBuffDesc );
        pBuffDesc = NULL;
		if ( bufRes  < 0 ) goto error;

        u32TotalBytesTransfered[u32ChanNum] = u32TotalTransfer;
        
        // Look if we have reached the number of requested transfer 
        if (u32TotalTransfer >= hStream->u32TransferSize )
        {
            break;
        }     

    }
    LockMutex(&g_TasksDoneMutex);
    g_stopTask[u32ChanNum] = 1;
    UnlockMutex(&g_TasksDoneMutex);
    // Unlock all semaphore
    // Does not check if they are previously owned
    // but since it is the end of the application
    // we do not care about setting a semaphore count to 2
    for(k = 0; k < hStream->u32NumberOfChannels; k++)
    {
        PostSemaphore(&g_aSem[k]);
    }

    // Put a buffer back into free queue to unblock ReadTask
    pBuffDesc = Buffer_Get( fullBuffQ, 0 );
    if ( pBuffDesc )
    {
        Buffer_Put( freeBuffQ, pBuffDesc );
        pBuffDesc = NULL;
    }
    goto done;

error:
    if ( FAILURE(res) ) 
    {
        fprintf( stderr, "TX Task Error: %s\n\n\n", ErrorToString(res) );
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
 * Function: ReadTask
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
static adp_thread_dec_t ReadTask( void *param )
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

    fp = fopen( hRTDExChannel[u32ChanNum]->pcFileName, "rb");
    if ( fp == NULL )
    {
        perror( "fopen");
        LockMutex(&g_TasksDoneMutex);
        g_stopTask[u32ChanNum] = 1;
        UnlockMutex(&g_TasksDoneMutex);
        goto done;
    }
    
    // Perform the read
    while ( !g_stopTask[u32ChanNum] )
    {
        // Get a buffer to fill
        pBuffDesc = Buffer_Get( freeBuffQ, 1 );
        if ( pBuffDesc == NULL ) goto error; 

        // Look for the end 
        if ( g_stopTask[u32ChanNum] ) { break; }
        
        // Read data from the file
		n = ReadBinFile( fp, pBuffDesc->pMem, hStreaming->u32BurstSize, 1 );
        if ( n < 0 )
        {
            perror( "ReadBinFile:" );
            LockMutex(&g_TasksDoneMutex);
            g_stopTask[u32ChanNum] = 1;
            UnlockMutex(&g_TasksDoneMutex);
            goto error;
        }
             
        pBuffDesc->i32Size = n;

        // Store the buffer to TX thread
        bufRes = Buffer_Put( fullBuffQ, pBuffDesc );
        pBuffDesc = NULL;
        if ( bufRes  < 0 ) goto error; 
        
        // Verify if the ReadBinFile return less bytes than requested
        if ( (uint32_t)n < hStreaming->u32BurstSize)
        {
            //EOF
            break;
        }
    }

    goto done;

error:
    if ( FAILURE(res) ) 
    {
        fprintf( stderr, "Read Task Error: %s\n\n\n", ErrorToString(res) );
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

    // Put a final empty buffer to signal to the TX task that we are done
    pBuffDesc = Buffer_Get( freeBuffQ, 1 );
    if ( pBuffDesc ) 
    {
        pBuffDesc->i32Size = -1;    // EOF
        Buffer_Put( fullBuffQ, pBuffDesc );
        pBuffDesc = NULL;
    }

    // Close the file handle
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

	 printf("Carrier     :   Channel        DataRate\n");

	 while(!g_stopDisplayTask)
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
             LockMutex(&g_DataRateMutex[i]);
             if(fDataRate[i] != -1)
             {
			     printf("   #%u       :     #%u           %2.2fMBps\n",
					    hChan->u8CarrierPosition,
					    hChan->u8ChannelNumber,
					    fDataRate[i]);
             }
             else
             {
			     printf("Test time too short to calculate rate \n");
             }
             UnlockMutex(&g_DataRateMutex[i]);

		 }
		 Sleep(1000);
         cursorToBeReset = 1;
	 }

	 return (adp_thread_ret_t)0;
 }
