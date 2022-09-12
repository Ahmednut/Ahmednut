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
 * @file      AuroraInitChannel.h
 * 
 * @brief     Function that intialize an Aurora channel
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
#include "bas_error.h"

#ifndef __AuroraInitChannel__
#define __AuroraInitChannel__


/**
*****************************************************************************
* @file AuroraInitChannel.h
*****************************************************************************
Aurora channel initialization functions
****************************************************************************/


/**
*****************************************************************************
* Function: AuroraInitChannel
*****************************************************************************
*
* Initializes the Aurora channel specified by u32AuroraChannel with direction
* specified by u32AuroraDirection. It configures the associated Gtx to a stable
* configuration and resets the channel's FIFOs
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  u32AuroraChannel
*      Aurora channel number, refering to the Aurora core instance number
*      in the FPGA design.
*
* @param [in]  u32AuroraDirection
*      Direction of the Aurora channel. Possibles values:
*      0: Rx; 1: Tx; 2: Both 
*
* @return
*      Success or error code.
*
****************************************************************************/
Result AuroraInitChannel(connection_state * state, uint32_t u32AuroraChannel, uint32_t u32AuroraDirection);

#endif
