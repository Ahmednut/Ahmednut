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
 * @file      LoadDataToPlayback.h
 * 
 * @brief     Functions that control the RAM transfer to carrier from a file
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
#include "media_types.h"
#include "connection_state.h"
#include "bas_types.h"
#include "recplay_types.h"
#include "bas_error.h"

#ifndef __LoadDataToPlayback__
#define __LoadDataToPlayback__

/**
*****************************************************************************
* @file LoadDataToPlayback.h
*****************************************************************************
Playback data load functions 
****************************************************************************/

/**
*****************************************************************************
* Function: LoadDataToPlayback
*****************************************************************************
*
* Loads data to carrier memory using the Record/Playback FPGA core
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  u32Channel
*      RTDEx channel used to transmit data from the host
*      to the Record/Playback core
*
* @param [in]  u32FrameSize
*      Size of the frame, in bytes.
*      With Ethernet, can be configured up to 8960 bytes when the 
*      MAC supports jumbo frames.
*      With PCIExpress, can be configured up to 1024*1024 = 1048576 bytes,
*      with an optimal value usually around 128 kB = 131072 bytes.
*
* @param [in]  uStartAddr
*      Address in bytes, in the carrier memory, where the chunk of data 
*      is to be put.
*
* @param [in]  pcDataBuffer
*      Pointer to the buffer containing the data to be transmitted to
*      carrier memory
*
* @param [in]  uFileSize
*      Number of bytes to transfer
*
* @return
*      Success or error code.
*
****************************************************************************/
Result LoadDataToPlayback(connection_state * state,
						 uint32_t u32Channel,
                         uint32_t u32FrameSize,
						 uint32_t uStartAddr,  
                         char *pcDataBuffer,
                         uint32_t uFileSize);  
						  
/**
*****************************************************************************
* Function: LoadDataToPlaybackFromFile
*****************************************************************************
*
* Loads data to carrier memory using the Record/Playback FPGA core, from 
* a file given by pcFileName
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  u32Channel
*      RTDEx channel used to transmit data from the host
*      to the Record/Playback core
*
* @param [in]  u32FrameSize
*      Size of the frame, in bytes.
*      With Ethernet, can be configured up to 8960 bytes when the 
*      MAC supports jumbo frames.
*      With PCIExpress, can be configured up to 1024*1024 = 1048576 bytes,
*      with an optimal value usually around 128 kB = 131072 bytes.
*
* @param [in]  uStartAddr
*      Address in bytes, in the carrier memory, where the chunk of data 
*      is to be put. It must be a multiple of 64 bytes.
*
* @param [in]  pcFileName
*      Name of the file that holds the data to be transmitted
*
* @return
*      Success or error code.
*
****************************************************************************/
Result LoadDataToPlaybackFromFile(connection_state * state,
						          uint32_t u32Channel,
                                  uint32_t u32FrameSize,
						          uint32_t u32StartAddress,
						          char *pcFileName);


#endif
