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
 * @file      RetrieveRecordedData.c
 * 
 * @brief     Functions that control the RAM transfer to host in a file
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
#include "RetrieveRecordedData.h"

// WARNING: to run this demo under LINUX, you may need root privilege

// Frame Gap sets the ethernet frame inter packets time pause in 125MHz clk counts when the module is sending data packets to the host.
// Could be used if data packets are lost. Hardware "Pause" frame support could also be used for this
// purpose, but all elements including the computer software drivers in the connection between
// the module and the host must support it and be configured for it. Make this number bigger
// to increase inter-packets delay. This number could be made lower if pause frame is supported
// Around 200000 ~ 2 msec inter packet delay

// Use for data transfer time estimation to the host (record). Estimation is for gigabit ethernet (see ETH_SPEED)...
#define FRAME_GAP_CLK 125000000
#define ETH_SPEED 1000000000
#define ETH_FRAME_OVERHEAD 38
#define PACKET_OVERHEAD 30

#define DESIRED_BURST_SIZE (64*1024*1024)
#define BURST_SIZE (DESIRED_BURST_SIZE-(DESIRED_BURST_SIZE%u32FrameSize))

#define MAX_RETRY_ALLOWED 10

RTDExPerseusParamsPcie perseusParams;

Result RetrieveRecordedData(connection_state * state,
                            uint32_t u32Channel,
                            uint32_t u32FrameSize,                 //in bytes 
                            uint32_t u32RecordSize,                //in bytes 
                            uint32_t u32StartAddress,              //in bytes
                            uint32_t u32RecordTimeout,
                            uint8_t *pu8DataBuffer)
{
	eMediaType eMedia;
	unsigned int uiRecPlayBaseAddr;
	adp_handle_recplay hRecPlay = NULL;             // Record Playback module handle
	unsigned int uiRTDExGigeBaseAddr;
    adp_handle_rtdex_conn hConnRecPlayRx = NULL;    // RTDEx handle

    unsigned char pHostMacAddr[6];                  // Host MAC addresses
    unsigned char pDeviceMacAddr[6];                // Device MAC addresses
    char * pTempDataBuffer = NULL;                  // Temporary RTDEx Data buffer
    int iRxTime;                                    // Time required to receive data
    uint32_t uNFramesUsed;                               // Number of RTDEx frames uses
    uint32_t uRecStartAddress = 0;              // Record Start Address (multiple of record core (64 bytes))
    uint32_t uRecStopAddress = 0;               // Record Stop Address  (multiple of record core (64 bytes))
    uint64_t uRecTransferSize = 0;              // Record Transfer Size in bytes (uRecStopAddress - uRecStartAddress)
    uint32_t uStopAddress = 0;                  // Real Stop Address (u32StartAddressess + u32RecordSize)

    uint64_t uRTDExTranferSize;                 // RTDEX Transfer Size
    uint32_t uRTDExBurstTransferSize;           // RTDEX Burst Transfer Size
    uint32_t uBurstStartAddress;                // Record Start Address (multiple of record core (64 bytes))

    uint32_t uOffsetStartRec = 0;               // Offset from u32StartAddressess and uRecStartAddress
    uint32_t uOffsetStopRec = 0;                // Offset from uStopAddress and uRecStopAddress

    uint64_t uNumByteTotalCopied = 0;           // Total number of bytes copied for each iteration of RTDExReceive

    uint64_t uTotalTransfer = 0;                // Total bytes transfered by RTDExReceive
    unsigned int uRetryNumber = 0;                  // Counter for the number of retry for the RTDExReceive

    unsigned int uFrameGap = 0;                     // Frame gap used. Will be incremented if retry occured.

    adp_result_t res = 0;                           // Error result from functions

    /*
    This explains how the memory works with the different variables defined

    For example :

    Address (in bytes)
      1024              1088              1152              1216              1280              1344              1408              1472
    ...|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|...
    ...|     64 bytes    |     64 bytes    |     64 bytes    |     64 bytes    |     64 bytes    |     64 bytes    |     64 bytes    |...
    ...|                 |                 |                 |                 |                 |                 |                 |...
    ...|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|...
                         ^      ^                                                                           ^      ^
    uRecStartAddress_____|      |                                                                           |      |
    u32StartAddress_____________|                                                                           |      |
    uStopAddress____________________________________________________________________________________________|      |
    uRecStopAddress________________________________________________________________________________________________|

    uOffsetStartRec      <------>
    u32RecordSize                <------------------------------------------------------------------------->
    uOffsetStopRec                                                                                          <------>
    */
    
    /*************************************************************************
        Opening interface with the Record Playback module
    *************************************************************************/
    // Detecting RTDEx media
	res = DetectMedia(state, &eMedia);
	if(FAILURE(res))
    {
        printf( "\n");
        printf( "Cannot determine RTDEx media type. Make sure an RTDEx core is used in your FPGA design.\n");
        goto end;
    }

    printf("The data will be transferred through %s\n",
        (eMedia == eMediaPcie) ? "RTDEx PCIe" : "RTDEx Gigabit Ethernet");

    res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRecPlay, 0, &uiRecPlayBaseAddr);
    if(FAILURE(res)) goto end;

	// Record/Playback initialization
	res = RecPlay_Open( &hRecPlay, state, uiRecPlayBaseAddr );
    if(FAILURE(res))
    {
        printf( "\n");
        printf( "Cannot open Record Playback module. Make sure the Record Playback core is used in your FPGA design.\n");
        goto end;
    }

    res = RecPlayWaitTransferDone(hRecPlay, u32RecordTimeout);
    if(FAILURE(res))
    {
        printf( "\n");
        printf( "The record process is still running and the %d ms timeout has been reached.\n", u32RecordTimeout);
        goto end;
    }
    
    /************************************************************************************************************************
                                         Calculating the transfer parameters
    ************************************************************************************************************************/
    // Force frame size multiple of 64 bytes
    u32FrameSize -= (u32FrameSize % 64);

    // Check multiple of record core (64)
    uOffsetStartRec = (u32StartAddress % 64); // This is the offset from the Absolute Start Address and the Record Start Address (which needs to be a multiple of record core)
    uRecStartAddress = u32StartAddress - uOffsetStartRec; //Get the Record Start Address to be a multiple of 64 bytes.

    uStopAddress = u32StartAddress + u32RecordSize;
    if (uStopAddress % 64 == 0)
    {
        uOffsetStopRec = 0;
    }
    else
    {
        uOffsetStopRec = 64 - (uStopAddress % 64);
    }

    uRecStopAddress = uStopAddress + uOffsetStopRec;

    uRecTransferSize = uRecStopAddress - uRecStartAddress; //multiple of 64 bytes

    // Check if uRecTransferSize is a multiple of u32FrameSize
    if ( uRecTransferSize % u32FrameSize != 0)
    {
        // if it is not, add one u32FrameSize 
        uRTDExTranferSize = ((uRecTransferSize / (uint64_t)u32FrameSize + 1) * (uint64_t)u32FrameSize);
        uRecTransferSize = uRTDExTranferSize;
    }
    else
    {
        // It is a multiple of u32FrameSize
        uRTDExTranferSize = uRecTransferSize;
    }

    // Malloc the TempBuffer depending of BURST_SIZE, to prevent huge malloc if small uRTDExTranferSize
    if(uRTDExTranferSize < BURST_SIZE)
    {
        pTempDataBuffer = (char *)malloc(uRTDExTranferSize);
    }
    else
    {
        pTempDataBuffer = (char *)malloc(BURST_SIZE);
    }

    // Check if Malloc failed
    if (pTempDataBuffer == NULL)
    {
        printf( "Memory allocation failed.\n\n" );
        goto end;
    }
    
    /************************************************************************************************************************
                                         Retrieving the data
    ************************************************************************************************************************/
    if(eMedia == eMediaEth)
    {
        // get MAC address of target and host:
        res = GetMACAddress(state, (char *)pHostMacAddr, (char *)pDeviceMacAddr);
        if(FAILURE(res)) 
        {
            printf( "Error getting MAC Address, Code: 0x%08X.\n\n", res );
            goto end;
        }
    }

	res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRTDExGigE, 0, &uiRTDExGigeBaseAddr);
	if(FAILURE(res)) return res;
    
    printf( "Setting the Record Playback module in memory to host transfer mode...\n" );
    while(uTotalTransfer < uRTDExTranferSize)
    {
    
        if(eMedia == eMediaEth)
        {
            // Reset RTDEx core
            res = RTDExResetCoreEth( state, uiRTDExGigeBaseAddr);
            if(FAILURE(res))  goto end;

            //opening RTDEx channel for record
            res = RTDExOpenHostAndFpgaEth(&hConnRecPlayRx,
                                      state,
                                      uiRTDExGigeBaseAddr,
                                      u32Channel,
                                      eFromFpga, 
                                      eSingle);

            if(FAILURE(res))  
            {
                printf( "\n");
                printf( "Error opening the RTDEx channel.\n" );
                goto end;
            }

            // setting frame gap value
            res = RTDExSetTxFrameGapValEth( state, uiRTDExGigeBaseAddr, uFrameGap );
            if(FAILURE(res)) goto end;
        }
        else if(eMedia == eMediaPcie)
        {
            //Init Param RTDEx PCIe
            RTDExInitParamsPcie(&perseusParams, state);
            res = RTDExResetCorePcie(&perseusParams);
            if(FAILURE(res)) goto end;

            //opening RTDEx channel for record
            res = RTDExOpenHostAndFpgaPcie( &hConnRecPlayRx, &perseusParams, u32Channel, eFromFpga, eSingle);
            if(FAILURE(res)) 
            {
                printf( "Error opening the RTDEx channel.\n" );
                goto end;
            }
        }
        else
        {
            printf( "\n");
            printf( "Unsupported RTDEx Media.\n" );
            goto end;
        }
        
        
        if(uRTDExTranferSize-uTotalTransfer < BURST_SIZE) // If remaining to transfer if smaller than BURST_SIZE
        {
            uRTDExBurstTransferSize = (uint32_t)(uRTDExTranferSize-uTotalTransfer);
        }
        else
        {
            uRTDExBurstTransferSize = BURST_SIZE;
        }

        uBurstStartAddress = (uRecStartAddress + uTotalTransfer) & 0xFFFFFFFF;

        //set record playback core to MEM2Host mode
        res = RecPlaySetModeRtdexMem2Host( hRecPlay, uBurstStartAddress, uRTDExBurstTransferSize);
        if(FAILURE(res)) goto end;

        // Compute an estimate of worst transfer time
        uNFramesUsed = uRTDExBurstTransferSize / u32FrameSize;
        
        iRxTime = (int) (1000.0f * (((float)(uNFramesUsed) * (float)uFrameGap / (float)FRAME_GAP_CLK) +
                (float)(uNFramesUsed) * (((float)(u32FrameSize + PACKET_OVERHEAD + ETH_FRAME_OVERHEAD) * 8)) / (float)ETH_SPEED));

        // Add 100 ms for setup time and margin
        iRxTime += 100;

        // Transfer time estimation is around (if no pause frame is used): (Inter frame time + packet time) * nbr frame
        // This assumes that the host is able to process these packets at that speed.
        // This estimate assumes the full ethernet bandwidth of the module <-> host link can be used for data transfer
        // Performance could be affected if OS page fault in the buffer occurs

        // Get the transfer data and wait until completed (it will hang if the transfer does not complete correctly)
        res = RTDExStart( hConnRecPlayRx, u32FrameSize, uRTDExBurstTransferSize);
        if(FAILURE(res)) 
        {
            printf( "\n");
            printf( "Error starting RTDEx channel.\n");
            goto end;
        }

        // Receive data in pTempDataBuffer
        res = RTDExReceive(hConnRecPlayRx, (unsigned char *)pTempDataBuffer, uRTDExBurstTransferSize, eRTDExWaitTimeoutOrFirstError, iRxTime);
        if(FAILURE(res)) 
        {
            res = RTDExGetLastError(hConnRecPlayRx);
            printf( "\n");
            printf( "RTDEx transfer failed\n" );
            goto end;
        }

        // Check if we received the expected size
        if (res == 0)
        {
            // Retry same memory transfer
            uRetryNumber++;
            if(uRetryNumber > MAX_RETRY_ALLOWED)
            {
                printf( "\n");
                printf( "The number of RTDExReceive retries has been exceeded.\n");
                printf( "Make sure you selected the right RTDEx channel and that you have a valid frame size.\n");
                goto end;
            }
        }
        else
        {
            uint32_t u32NumByteToCopy;

            //Calculate Number of bytes to copy in pu8DataBuffer 
            if(uTotalTransfer < uOffsetStartRec) //Used at beginning, when we didn't transfer anything yet, or lower than the uOffsetStartRec.
                u32NumByteToCopy = res-uOffsetStartRec;
            else
                u32NumByteToCopy = res;

            //This condition is for the last copy, to make sure we copy only the rest of the size of u32RecordSize.
            //This is what is missing from u32RecordSize,and not to copy more
            if ((unsigned int)u32NumByteToCopy > (u32RecordSize-uNumByteTotalCopied))
                u32NumByteToCopy = (uint32_t)(u32RecordSize - uNumByteTotalCopied);

            //We did receive data bytes
            if(u32NumByteToCopy > 0)
            {
                //Copy the pTempDataBuffer+uOffsetStartRec to pu8DataBuffer, because the values before uOffsetStartRec are invalid (First copy to be done)
                if(uTotalTransfer < uOffsetStartRec)
                {
                    memcpy(pu8DataBuffer, 
                        pTempDataBuffer+uOffsetStartRec, 
                        u32NumByteToCopy);
                }
                else //Copy the pTempDataBuffer to pu8DataBuffer+uNumByteTotalCopied, depending on how many of uNumByteTotalCopied were already copied to pu8DataBuffer
                {
                    memcpy(pu8DataBuffer+uNumByteTotalCopied, 
                        pTempDataBuffer, 
                        u32NumByteToCopy);
                }

                // Calculate Total Bytes copied
                uNumByteTotalCopied += u32NumByteToCopy;
            }

            // Increment address to retrieve next memory section
            uTotalTransfer += res;
        }

        // If less than 1 MB is received
        // increment the frame gap to receive more valid data for the next try
        // Limit the maximum frame gap to 50000.
        if(res < (1<<20) && uFrameGap < 50000)
        {
            uFrameGap += 100;
        }
        
        printf( "\r%.3f MB received.",(float)uTotalTransfer/1024.0f/1024.0f);
        fflush(stdout);

        res = RecPlayResetCore(hRecPlay);
        if(FAILURE(res)) 
        {
            printf( "\n");
            printf( "Error resetting the Record Playback core.\n");
            goto end;
        }
        
        // Close RTDEx because it is reopened at every loop
        res = RTDExClose(hConnRecPlayRx);
        hConnRecPlayRx = NULL;
        if(FAILURE(res)) goto end;
    }

    res = OK;

    printf( "\n" );    
    printf( "Done.\n\n");
    
end:
    if(pTempDataBuffer) free(pTempDataBuffer);
    if(hConnRecPlayRx) RTDExClose(hConnRecPlayRx);
    if(hRecPlay) RecPlay_Close(hRecPlay);
        
    return res;
}

Result RetrieveRecordedDataFromTrigger(connection_state * state,
                                       uint32_t u32Channel,
                                       uint32_t u32FrameSize,        //in bytes 
                                       uint32_t u32RecordSize,       //in bytes 
                                       uint32_t u32RecordTimeout,
                                       uint8_t *pu8DataBuffer)
{
    unsigned int uiRecPlayBaseAddr;
    adp_handle_recplay hRecPlay;

    unsigned int uTriggerAddress;
    adp_result_t res = 0;
    
	// Record/Playback initialization
    res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRecPlay, 0, &uiRecPlayBaseAddr);
    if(FAILURE(res)) return res;

	res = RecPlay_Open( &hRecPlay, state, uiRecPlayBaseAddr );
    if(FAILURE(res))
    {
        printf( "\n");
        printf( "Cannot open Record Playback module. Make sure the Record Playback core is used in your FPGA design.\n");
        return res;
    }

    res = RecPlayWaitTransferDone(hRecPlay,u32RecordTimeout);
    if(FAILURE(res))
    {
        printf( "\n");
        printf( "The record process is still running and the %d ms timeout has been reached.\n",u32RecordTimeout);
        RecPlay_Close(hRecPlay);
        return res;
    }

    // Retrieving trigger offset
    res = RecPlayGetTriggerPos( hRecPlay, &uTriggerAddress );
    if(FAILURE(res)) 
    {
        RecPlay_Close(hRecPlay);
        return res;
    }
    printf( "Trigger position: %d\n", uTriggerAddress);

    // CLosing Record Playback handle
    res = RecPlay_Close(hRecPlay);
    if(FAILURE(res)) return res;


    // Retrieving Record Data
    res = RetrieveRecordedData(state, u32Channel, u32FrameSize, u32RecordSize, uTriggerAddress, u32RecordTimeout, pu8DataBuffer);
    if(FAILURE(res)) 
    {
        return res;
    }

    return OK;
}

Result RetrieveRecordedDataToFile(connection_state * state,
                                  uint32_t u32Channel, 
                                  uint32_t u32FrameSize,             //in bytes 
                                  uint32_t u32RecordSize,            //in bytes 
                                  uint32_t u32StartAddress,          //in bytes
                                  uint32_t u32RecordTimeout,
                                  char *pcRecordFileName)
{
    FILE * pfile;
    uint8_t* pu8RecordBuffer = NULL;
    size_t uNumBytesWritten;
    adp_result_t res = 0;

    // Opening file
    pfile = fopen(pcRecordFileName,"wb");

    if (pfile == NULL)
    {
		printf("Error creating file, make sure that %s is correct\n", pcRecordFileName);
        return EAPIFILE;
    }

    // Malloc Buffer
    pu8RecordBuffer = (uint8_t *)malloc(u32RecordSize);
    if (pu8RecordBuffer == NULL)
    {
		fclose(pfile);
        return ERR_OUTOFMEMORY;
    }

    // Retrieving Record Data
    res = RetrieveRecordedData(state, u32Channel, u32FrameSize, u32RecordSize, u32StartAddress, u32RecordTimeout, pu8RecordBuffer);
    if(FAILURE(res)) 
    {
		free(pu8RecordBuffer);
		fclose(pfile);
        return res;
    }

    // Writing to file
    uNumBytesWritten = fwrite(pu8RecordBuffer, sizeof(char), u32RecordSize, pfile);
    if(uNumBytesWritten != u32RecordSize)
    {
        printf("Value written %u bytes not equal to RecordSize %u bytes\n",res,u32RecordSize);
		free(pu8RecordBuffer);
		fclose(pfile);
        return -1;
    }

    free(pu8RecordBuffer);
    fclose(pfile);
    return OK;
}

Result RetrieveRecordedDataFromTriggerToFile(connection_state * state,
                                             uint32_t u32Channel,
                                             uint32_t u32FrameSize,  //in bytes 
                                             uint32_t u32RecordSize, //in bytes 
                                             uint32_t u32RecordTimeout,
                                             char *pcRecordFileName)
{
    FILE * pfile;
    uint8_t * pu8RecordBuffer = NULL;
    size_t uNumBytesWritten;
    adp_result_t res = 0;

    // Opening file
    pfile = fopen(pcRecordFileName,"wb");
    if (pfile == NULL)
    {   
		printf("Error creating file, make sure that %s is correct\n", pcRecordFileName);
        return EAPIFILE;
    }

    // Malloc Buffer
    pu8RecordBuffer = (uint8_t *)malloc(u32RecordSize);

    if (pu8RecordBuffer == NULL)
    {
        free(pu8RecordBuffer);
        return ERR_OUTOFMEMORY;
    }

    // Retrieving Record Data
    res = RetrieveRecordedDataFromTrigger(state, u32Channel, u32FrameSize, u32RecordSize, u32RecordTimeout, pu8RecordBuffer);
    if(FAILURE(res)) 
    {
        return res;
    }

    // Writing to file
    uNumBytesWritten = fwrite(pu8RecordBuffer, sizeof(char), u32RecordSize, pfile);
    if(uNumBytesWritten != u32RecordSize)
    {
        printf("Value written %u bytes not equal to RecordSize %u bytes\n",res,u32RecordSize);
        return -1;
    }

    free(pu8RecordBuffer);
    fclose(pfile);
    return OK;
}