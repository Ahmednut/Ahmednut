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
 * @file      RetrieveRecordedData.h
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
#include "media_types.h"
#include "connection_state.h"
#include "bas_types.h"
#include "recplay_types.h"
#include "bas_error.h"

#ifndef __RetrieveRecordedData__
#define __RetrieveRecordedData__

/**
*****************************************************************************
* @file RetrieveRecordedData.h
*****************************************************************************
Data retrieval functions
****************************************************************************/

/**
*****************************************************************************
* Function: RetrieveRecordedData
*****************************************************************************
*
* Retrieves data stored in the carrier memory
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  u32Channel
*      RTDEx channel used to transmit data from the Record/Playback 
*      core to the host.
*
* @param [in]  u32FrameSize
*      Size of the frame, in bytes.
*      With Ethernet, can be configured up to 8960 bytes when the 
*      MAC supports jumbo frames.
*      With PCIExpress, can be configured up to 1024*1024 = 1048576 bytes,
*      with an optimal value usually around 128 kB = 131072 bytes.
*      In either case, it must be a multiple of 64 bytes.
*      
* @param [in]  u32RecordSize
*      Size of the record, in bytes.
*      This is the size, in bytes, of the contiguous chunk of data to retrieve
*      that starts at the address specified by u32StartAddress.
*      
* @param [in]  u32StartAddress
*      Address in bytes, in the carrier memory, where the chunk of data to 
*      retrieve starts
*      
* @param [in]  u32RecordTimeout
*      Maximum number of milliseconds to wait until the host receives
*      the number of bytes specified by u32RecordSize. After this delay, the function 
*      immediately returns an error.
*      
* @param [out]  pu8DataBuffer
*     Buffer used to store the retrieved data. It must be a pre-allocated array
*     large enough to hold u32RecordSize bytes.
*      
*
* @return
*      Success or error code.
*
****************************************************************************/
Result RetrieveRecordedData(connection_state * state,
                            uint32_t u32Channel,
                            uint32_t u32FrameSize,       
                            uint32_t u32RecordSize,      
                            uint32_t u32StartAddress,   
                            uint32_t u32RecordTimeout,
                            uint8_t *pu8DataBuffer);
/**
*****************************************************************************
* Function: RetrieveRecordedDataFromTrigger
*****************************************************************************
*
* Retrieves data stored in the carrier memory, starting at the position
* where the record trigger occured
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  u32Channel
*      RTDEx channel used to transmit data from the Record/Playback 
*      core to the host.
*
* @param [in]  u32FrameSize
*      Size of the frame, in bytes.
*      With Ethernet, can be configured up to 8960 bytes when the 
*      MAC supports jumbo frames.
*      With PCIExpress, can be configured up to 1024*1024 = 1048576 bytes,
*      with an optimal value usually around 128 kB = 131072 bytes.
*      In either case, it must be a multiple of 64 bytes.
*      
* @param [in]  u32RecordSize
*      Size of the record, in bytes.
*      This is the size, in bytes, of the contiguous chunk of data to retrieve
*      that starts at the address specified by u32StartAddress.     
*      
* @param [in]  u32RecordTimeout
*      Maximum number of milliseconds to wait until the host receives
*      the number of bytes specified by u32RecordSize. After this delay, the function 
*      immediately returns an error.
*      
* @param [out]  pu8DataBuffer
*     Buffer used to store the retrieved data. It must be a pre-allocated array
*     large enough to hold u32RecordSize bytes.
*      
*
* @return
*      Success or error code.
*
****************************************************************************/
Result RetrieveRecordedDataFromTrigger(connection_state * state,
                                       uint32_t u32Channel,
                                       uint32_t u32FrameSize,        
                                       uint32_t u32RecordSize,       
                                       uint32_t u32RecordTimeout,
                                       uint8_t *pu8DataBuffer);
/**
*****************************************************************************
* Function: RetrieveRecordedDataToFile
*****************************************************************************
*
* Retrieves data stored in the carrier memory, storing it in a file
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  u32Channel
*      RTDEx channel used to transmit data from the Record/Playback 
*      core to the host.
*
* @param [in]  u32FrameSize
*      Size of the frame, in bytes.
*      With Ethernet, can be configured up to 8960 bytes when the 
*      MAC supports jumbo frames.
*      With PCIExpress, can be configured up to 1024*1024 = 1048576 bytes,
*      with an optimal value usually around 128 kB = 131072 bytes.
*      In either case, it must be a multiple of 64 bytes.
*      
* @param [in]  u32RecordSize
*      Size of the record, in bytes.
*      This is the size, in bytes, of the contiguous chunk of data to retrieve
*      that starts at the address specified by u32StartAddress.
*      
* @param [in]  u32StartAddress
*      Address, in the carrier memory, where the chunk of data to retrieve starts
*      
* @param [in]  u32RecordTimeout
*      Maximum number of milliseconds to wait until the host receives
*      the number of bytes specified by u32RecordSize. After this delay, the function 
*      immediately returns an error.
*      
* @param [in]  pcRecordFileName
*      Name of the file that will hold the retrieved data. It will be automatically
*      created if it doesn't exist.
*      
*
* @return
*      Success or error code.
*
****************************************************************************/
Result RetrieveRecordedDataToFile(connection_state * state,
                                  uint32_t u32Channel, 
                                  uint32_t u32FrameSize,    
                                  uint32_t u32RecordSize,   
                                  uint32_t u32StartAddress,
                                  uint32_t u32RecordTimeout,
                                  char *pcRecordFileName);
/**
*****************************************************************************
* Function: RetrieveRecordedDataFromTriggerToFile
*****************************************************************************
*
* Retrieves data stored in the carrier memory, starting at the position
* where the record trigger occured.
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  u32Channel
*      RTDEx channel used to transmit data from the Record/Playback 
*      core to the host.
*
* @param [in]  u32FrameSize
*      Size of the frame, in bytes.
*      With Ethernet, can be configured up to 8960 bytes when the 
*      MAC supports jumbo frames.
*      With PCIExpress, can be configured up to 1024*1024 = 1048576 bytes,
*      with an optimal value usually around 128 kB = 131072 bytes.
*      In either case, it must be a multiple of 64 bytes.
*      
* @param [in]  u32RecordSize
*      Size of the record, in bytes.
*      This is the size, in bytes, of the contiguous chunk of data to retrieve
*      that starts at the address specified by u32StartAddress.     
*      
* @param [in]  u32RecordTimeout
*      Maximum number of milliseconds to wait until the host receives
*      the number of bytes specified by u32RecordSize. After this delay, the function 
*      immediately returns an error.
*      
* @param [in]  pcRecordFileName
*      Name of the file that will hold the retrieved data. It will be automatically
*      created if it doesn't exist.
*      
*
* @return
*      Success or error code.
*
****************************************************************************/
Result RetrieveRecordedDataFromTriggerToFile(connection_state * state,
                                             uint32_t u32Channel,
                                             uint32_t u32FrameSize,  
                                             uint32_t u32RecordSize, 
                                             uint32_t u32RecordTimeout,
                                             char *pcRecordFileName);


#endif
