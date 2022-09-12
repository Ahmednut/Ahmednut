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
 * @file      MI125_Init.h
 * 
 * @brief     Function that initialize the MI125 FMC
 *
 * Copyright (C) 2015, Nutaq, Canada. All rights reserved.
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
#include "bas_error.h"
#include "connection_state.h"
#include "MI125_mi125_defines.h"

#ifndef __MI125_Init__
#define __MI125_Init__

#define MAX_MI125 4

typedef struct
{
	MI125_mi125_clock_source eSamplingClockSource;
	MI125_mi125_trigout eTrigOut;	
} sMi125Struct;

typedef sMi125Struct* handle_mi125;

/**
*****************************************************************************
* @file MI125_Init.h
*****************************************************************************
* MI125 initialization functions 
*
****************************************************************************/

/**
*****************************************************************************
* Function: MI125_Init
*****************************************************************************
*
* Performs an initialization routine on the MI125, with parameters contained
* in the sMi125Struct object pointed to by argument "handle"
*
* @param [in]  state
*      Pointer to the connection state structure that handles 
*      the TCP connection between the host and the carrier platform.
*
* @param [in]  handle
*      Array of pointers to sMi125Struct objects, each one holding parameters
*      particular to each MI125 to be initialized
*
* @return
*      Success or error code.
*
****************************************************************************/
Result MI125_Init(connection_state * state, 
					handle_mi125 * handle);

#endif
