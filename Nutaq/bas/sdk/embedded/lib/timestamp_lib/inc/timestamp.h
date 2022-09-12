/****************************************************************************   
 * 
 *    ****                              I                                     
 *   ******                            ***                                    
 *   *******                           ****                                   
 *   ********    ****  ****     **** *********    ******* ****    *********** 
 *   *********   ****  ****     **** *********  **************  ************* 
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     **** 
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      **** 
 *  ****    *********  ****     ****   ****   ****       ****  ****      **** 
 *  ****     ********  ****    ****I  ****    *****     *****  ****      **** 
 *  ****      ******   ***** ******   *****    ****** *******  ****** ******* 
 *  ****        ****   ************    ******   *************   ************* 
 *  ****         ***     ****  ****     ****      *****  ****     *****  **** 
 *                                                                       **** 
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**  
 * 
*  @file       : timestamp.h
*  @brief      : timestamp core register and functions definitions 
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup TIMESTAMP
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
 
 /** @defgroup TIMESTAMP TIMESTAMP Module
 *
 * TODO: Write description
 * 
 * @file
 *     
 */

#ifndef TIMESTAMP_HEADER__
#define TIMESTAMP_HEADER__

#include "bas_types.h"
#include "timestamp_defines.h"

/***************************************************************************
* Function: timestamp_open
************************************************************************//**
*
* Create handle to timestamp core.
*
* @param[in,out] phTimestamp
*   Pointer to receive the handle of the timestamp object that will be created
*
* @param[in] u32BaseAddress
*   Base address of the timestamp core instance
*
* @param[in]   pfUsleep
*      Pointer to a sleep function that sleeps during N microsecond.
*      This function is the one that will be used inside the library functions.
*
* @return
*      return TIMESTAMPOK on success.
*
***************************************************************************/
Result Timestamp_Open(Timestamp_Handle_t * phTimestamp, uint32_t u32BaseAddress, int (*pfUsleep)(unsigned int usec));

/***************************************************************************
* Function: Timestamp_Presence
************************************************************************//**
*
* Verifies the timestamp presence by verifying the core ID at the base address
*
* @param[in] hTimestamp
*   Timestamp handle
*
* @return
*      return TIMESTAMPOK on success.
*      return TIMESTAMPABSENT is the core ID does not match
*
***************************************************************************/
Result Timestamp_Presence(Timestamp_Handle_t hTimestamp);

/***************************************************************************
* Function: Timestamp_GetVersion
************************************************************************//**
*
* Get the timestamp core version
*
* @param[in] hTimestamp
*   Timestamp handle
*
* @param[out] puiVersion
*   Pointer to unsigned int variable to receive the version number
*
* @return
*      return TIMESTAMPOK on success.
*
***************************************************************************/
Result Timestamp_GetVersion(Timestamp_Handle_t hTimestamp, unsigned int * puiVersion);

/***************************************************************************
* Function: Timestamp_SetTime
************************************************************************//**
*
* Configure the core to set the desired time at the next PPS rising edge
* The time will be set and hold to zero until the PPS rising edge event
*
* @param[in] hTimestamp
*   Timestamp handle
*
* @param[in] u64Timestamp
*   64-bit timestamp to set
*
* @return
*      return TIMESTAMPOK on success.
*
***************************************************************************/
Result Timestamp_SetTime(Timestamp_Handle_t hTimestamp, uint64_t u64Timestamp);


/***************************************************************************
* Function: Timestamp_GetTime
************************************************************************//**
*
* Read the current timestamp value
*
* @param[in] hTimestamp
*   Timestamp handle
*
* @param[in] pu64Timestamp
*   64-bit pointer to return the timestamp value read
*
* @return
*      return TIMESTAMPOK on success.
*
***************************************************************************/
Result Timestamp_GetTime(Timestamp_Handle_t hTimestamp, uint64_t * pu64Timestamp);

/***************************************************************************
* Function: Timestamp_Reset
************************************************************************//**
*
* Reset timestamp core
*
* @param[in] hTimestamp
*   Timestamp handle
*
* @return
*      return TIMESTAMPOK on success.
*
***************************************************************************/
Result Timestamp_Reset(Timestamp_Handle_t hTimestamp);

/***************************************************************************
* Function: Timestamp_Close
************************************************************************//**
*
* Free the timestamp object
*
* @param[in] hTimestamp
*   Timestamp handle
*
* @return
*      return TIMESTAMPOK on success.
*
***************************************************************************/
Result Timestamp_Close(Timestamp_Handle_t hTimestamp);

#endif 

