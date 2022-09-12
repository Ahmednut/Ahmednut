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
 * @file      RxTsStreaming.h
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
#include "connection_state.h"
#include "media_types.h"
#include "rtdex_types.h"
#include "rtdex_media.h"
#include "rtdexts_def.h"
#include "bas_types.h"
#include "bas_error.h"
#include "bas_buffer.h"
#include "bas_semaphore.h"
#include "bas_thread.h"

#ifndef __RxTsStreaming__
#define __RxTsStreaming__

#define MAX_PERSEUS				20
#define MAX_RTDEXTS 			25

typedef struct
{
	uint32_t u32SubFrameSize;	  //in bytes
	uint32_t u32FrameSize;		  //in bytes
	uint32_t u32QueueSize;		  //integer (default = 100)
	uint32_t u32NumOfSamples;	  //in bytes
	uint32_t u32MaxTransferSize;  //in bytes
	uint8_t u8RealTimeFlag;       //1 = real time (For Windows only)
	uint32_t u32NumberOfChannels; //total number of channel to use
    uint8_t bTsInit;
    uint32_t u32TsClkFreq;                  ///< Timestamp module clock frequency
    int32_t i32PropDelayNs;                 ///< Propagation delay of the path expressed in nanosecond
	struct
    {
        RTDExTs_Mode_t mode;         ///< Mode
        char pcTime[50];             ///< Time (in readable local time)
        uint32_t u32secTime;         ///< Time (in POSIX)
        uint32_t u32nsecTime;        ///< Time offset nsec (from POSIX time)
        RTDExTs_TrigSel_t trigSel;   ///< Trigger selection
    } start, stop;
} sRTDExTsStreamingParams;

typedef sRTDExTsStreamingParams* handle_streaming;

typedef struct
{
	uint8_t u8CarrierPosition;
	uint8_t u8ChannelNumber;
	char pcFileName[255];
} sRTDExTsChannelStruct;

typedef sRTDExTsChannelStruct* handle_rtdexts_channel;

/**
*****************************************************************************
* @file RxTsStreaming.h
*****************************************************************************
* RTDExTimestamping Rx streaming functions
*
****************************************************************************/

/**
*****************************************************************************
* Function: RxTsStreaming
*****************************************************************************
*
* Performs RxTimestamping streaming (FPGA to Host). This function is blocking.
*
* @param [in]  pcIpAddresses
*      Array of IP addresses of the carriers from which data is to be streamed
*
* @param [in]  NbIpAddresses
*      Number of IP addresses pcIpAddresses holds
*
* @param [in]  handle_rxstreaming
*      Pointer to a sRTDExTsStreamingParams object, holding parameters common to all
*      RTDExTimestamping channels to be used
*
* @param [in]  handle_rxchannel
*      Array of pointers to sRTDExTsChannelStruct objects, each one holding parameters
*      particular to each RTDExTimestamping channel to be used
*
* @return
*      Success or error code.
*
****************************************************************************/
Result RxTsStreaming(char ** pcIpAddresses, 
                     uint32_t NbIpAddresses, 
                     handle_streaming handle_rxstreaming, 
                     handle_rtdexts_channel * handle_rxchannel);

#endif
