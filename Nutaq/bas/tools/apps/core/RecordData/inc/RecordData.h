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
 * @file      RecordData.h
 * 
 * @brief     Function that Records to RAM
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
#include "recplay.h"
#include "bas_error.h"

#ifndef __RecordData__
#define __RecordData__

/**
*****************************************************************************
* @file RecordData.h
*****************************************************************************
Functions to configure the Record/Playback core to record data
****************************************************************************/


/**
*****************************************************************************
* Function: RecordData
*****************************************************************************
*
* Configures the Record/Playback core to record data from the user logic.
* The function immediately returns after configuring the Record/Playback core
* and so does not wait until record is over before exiting.
*
* @warning
*      Since the memory transaction are 64-byte wide, even if the trigger 
*      does not happen exactly at the beginning of a 64-byte word, 
*      the whole 64-byte word will be counted in the number of 
*      bytes written. This will prevent the last desired bytes to be 
*      written inside the memory. To avoid losing the last bytes if the trigger
*      can happen inside a 64-byte word, add an additional 64 bytes to the record
*      size.
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  eTriggerSource
*     Source of the trigger from possible values as described in the 
*     eRecPlayTrigSrc enumeration.
*
* @param [in]  u32RecordSize
*     Number of bytes stored inside the memory after the trigger event has been 
*     detected. Before the trigger event, the data is written continuously 
*     inside the memory and is not taken into account in the calculation of
*     the number of bytes written.
*     This value must be a multiple of 64 bytes.
*
* @param [in]  u32StartAddress
*      First data storage address in the platform memory in bytes.
*      This value must be a multiple of 64 bytes.
*
* @param [in]  u32TriggerDelay
*      This value will delay the trigger event by the specified number of bytes.
*      This value must be a multiple of 64 bytes.
*
* @return
*      Success or error code.
*
****************************************************************************/
Result RecordData(connection_state * state,
                  eRecPlayTrigSrc eTriggerSource,
                  uint32_t u32RecordSize,    
                  uint32_t u32StartAddress,  
                  uint32_t u32TriggerDelay); 
                


#endif
