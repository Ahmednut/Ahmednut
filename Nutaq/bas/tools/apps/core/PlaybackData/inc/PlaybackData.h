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
 * @file      PlaybackData.h
 * 
 * @brief     Function that Playback from RAM
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
#include "connection_state.h"
#include "bas_types.h"
#include "recplay_types.h"
#include "recplay.h"
#include "bas_error.h"

#ifndef __PlaybackData__
#define __PlaybackData__

/**
*****************************************************************************
* @file PlaybackData.h
*****************************************************************************
Data playback functions
****************************************************************************/

/**
*****************************************************************************
* Function: PlaybackData
*****************************************************************************
*
* Playbacks data from the carrier memory to the user logic, using the
* Record/Playback core
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  eTriggerSource
*      Source of the trigger from possible values as described in the 
*      eRecPlayTrigSrc enumeration.
*
* @param [in]  u32PlaybackMode
*      Possibles values:
*      0: Single playback. The Record/Playback core playbacks u32PlaybackSize 
*      bytes from carrier memory and then stops;
*      1: Continuous playback. The Record/Playback core continuously playbacks
*      u32PlaybackSize bytes from carrier memory.
*
* @param [in]  u32PlaybackSize
*      Number of bytes to playback from carrier memory.
*
* @param [in]  u32StartAddress
*      Address in bytes, in the carrier memory, where to start playback.
*      This value must be a multiple of 64 bytes.
*
* @return
*      Success or error code.
*
****************************************************************************/
Result PlaybackData(connection_state * state, 
					eRecPlayTrigSrc eTriggerSource, 
					uint32_t u32PlaybackMode, 
					uint32_t u32PlaybackSize,
					uint32_t u32StartAddress);


#endif
