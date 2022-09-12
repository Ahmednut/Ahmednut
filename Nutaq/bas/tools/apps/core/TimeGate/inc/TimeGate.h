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
 * @file      TimestampUtil.h
 * 
 * @brief     Function for timestamp module initialization
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
#include "bas_types.h"
#include "bas_error.h"

#ifndef __TimeGate__
#define __TimeGate__

#define eRELATIVE 0
#define eABSOLUTE 1

typedef struct
{
    uint32_t u32ClkFreq;                    ///< Timestamp module clock frequency.
    int bGetTickTimeOnly;                   ///< Only print device tick time
    int bGetPosixTimeOnly;                  ///< Only print device POSIX time
    int bGetReadableLocalTimeOnly;          ///< Only print device readable local time
    int bResetOnly;                         ///< Reset only the timestamp module
    uint64_t u64Value;                      ///< Value to be latched by the timestamp module at the next trigger event
    int32_t i32WaitSec;                     ///< If not 0, verify if the trigger event has happened. If the specified time elapses, a timeout error will occur.
    int bAbsoluteTime;                      ///< Configure the timestamp module with absolute time.
    
    int bWaitUntilTime;                     ///< Wait until time
    char acTime[50];                        ///< Time (in readable local time)
    int32_t i32NanoSecOffset;               ///< Nano second offset to add to the time until this app returns

    int bReadCustomRegisterTickTime;        ///< Get time from custom registers, in ticks 
    int bReadCustomRegisterPosixTime;       ///< Get time from custom registers, converted to POSIX time
    int bReadCustomRegisterReadableTime;    ///< Get time from custom registers, converted to readable local time
    uint32_t u32LswCustomRegisterId;        ///< Custom register holding the least significant word of the time
    uint32_t u32MswCustomRegisterId;        ///< Custom register holding the most significant word of the time
} sTimestampParams;


typedef sTimestampParams *handle_timestampparams;

/**
*****************************************************************************
* @file TimeGate.h
*****************************************************************************
* Utility functions to control the TimeGate module
*
****************************************************************************/

/**
*****************************************************************************
* Function: TimeGate
*****************************************************************************
*
* Configure/use the timestamp module according to parameters/instructions contained
* in the sTimestampParams object pointed to by hParams
*
* @param [in]  pcIpAddress
*      IP address of the carrier
*
* @param [in]  hParams
*      Pointer to a sTimestampParams object containing parameters/instructions
*      to apply on the timestamp module
*
* @param [in]  Time
*     String of the date or tick number to write in the Timegate 
*
* @param [in]  mode
*     0 Relative mode, time parameter must be a number of tick
*     1 Abslute  mode, time parameter must be a time string ex: 2016-07-05 15:44:30.0
*
* @return
*      Success or error code.
*
****************************************************************************/
Result TimeGate(char *pcIpAddress, handle_timestampparams hParams, char * Time,uint32_t mode);

#endif
