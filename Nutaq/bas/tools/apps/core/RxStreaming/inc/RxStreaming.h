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
 * @file      RxStreaming.h
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
#include "bas_types.h"
#include "bas_error.h"
#include "bas_buffer.h"
#include "bas_semaphore.h"
#include "bas_thread.h"

#ifndef __RxStreaming__
#define __RxStreaming__

#define MAX_PERSEUS				20
#define MAX_RTDEX				50

typedef struct
{
	uint32_t u32FrameSize;		  //in bytes
	uint32_t u32QueueSize;		  //integer (default = 100)
	uint32_t u32TransferSize;	  //in bytes
	uint32_t u32BurstSize;		  //in bytes
	uint8_t u8RealTimeFlag;       //1 = real time (For Windows only)
	uint8_t u8DisplayStatsFlag;   //1 = yes
	uint32_t u32NumberOfChannels; //total number of channel to use
    uint32_t u32FrameGap;         //Frame gap, in clock ticks
} sRTDExStreamingParams;

typedef sRTDExStreamingParams* handle_streaming;

typedef struct
{
	uint8_t u8CarrierPosition;
	uint8_t u8ChannelNumber;
	char pcFileName[255];
} sRTDExChannelStruct;

typedef sRTDExChannelStruct* handle_rtdex_channel;

/**
*****************************************************************************
* @file RxStreaming.h
*****************************************************************************
* RTDEx Rx streaming functions
*
****************************************************************************/

/**
*****************************************************************************
* Function: RxStreaming
*****************************************************************************
*
* Performs Rx streaming (FPGA to Host). This function is blocking.
*
* @param [in]  pcIpAddresses
*      Array of IP addresses of the carriers from which data is to be streamed
*
* @param [in]  NbIpAddresses
*      Number of IP addresses pcIpAddresses holds
*
* @param [in]  handle_rxstreaming
*      Pointer to a sRTDExStreamingParams object, holding parameters common to all
*      RTDEx channels to be used
*
* @param [in]  handle_rxchannel
*      Array of pointers to sRTDExChannelStruct objects, each one holding parameters
*      particular to each RTDEx channel to be used
*
* @return
*      Success or error code.
*
****************************************************************************/
Result RxStreaming(char ** pcIpAddresses, 
                   uint32_t NbIpAddresses, 
                   handle_streaming handle_rxstreaming, 
                   handle_rtdex_channel * handle_rxchannel);

#endif
