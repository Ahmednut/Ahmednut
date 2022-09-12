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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**    
 *
 * @file    rtdexts_def.h
 * @brief   RTDExTs public constants and types.
 *
 * $Date: 2015/08/10 17:10:41 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RTDEXTS
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
#ifndef RTDEXTS_DEF_H
#define RTDEXTS_DEF_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#include "bas_error.h"              // MAKE_RESULT


/****************************************************************************
 *                             Public constants                             * 
 ****************************************************************************/     


/****************************************************************************
 *                               Public types                               * 
 ****************************************************************************/ 

/**
 * RTDExTs Handle
 */
typedef struct RTDExTs_Obj *RTDExTs_Handle_t;

/**
 * RTDExTs direction.
 */
typedef enum RTDExTs_Dir
{
    RTDExTs_Dir_Uplink = 0,                   ///< Uplink : Device to Host
    RTDExTs_Dir_Downlink,                     ///< Downlnk: Host to Device
    RTDExTs_Dir_Num                           ///< Number of direction
} RTDExTs_Dir_t;

/**
 * RTDExTs operation mode.
 */
typedef enum RTDExTs_Mode
{
    RTDExTs_TrigMode_Immediately = 0,         ///< Start/Stop immediately
    RTDExTs_TrigMode_OnTrigger = 1,           ///< Start/Stop on trigger
    RTDExTs_TrigMode_AtTime = 2,              ///< Start/Stop at absolute time
    RTDExTs_TrigMode_Num                      ///< Number of trigger mode
} RTDExTs_Mode_t;

/**
 * RTDExTs trigger selection.
 */
typedef enum RTDExTs_TrigSel
{
    RTDExTs_TrigSel_NA = 0,                   // Not applicable
    RTDExTs_TrigSel_0  = 0,                   // RTDExTs Core Trigger Input 0
    RTDExTs_TrigSel_1  = 1,                   // RTDExTs Core Trigger Input 1
    RTDExTs_TrigSel_2  = 2,                   // RTDExTs Core Trigger Input 2
    RTDExTs_TrigSel_3  = 3,                   // RTDExTs Core Trigger Input 3
    RTDExTs_TrigSel_Num                       // Number of external trigger
} RTDExTs_TrigSel_t;

/**
 * RTDExTs errors (ADP style)
 */
typedef enum RTDExTs_Error
{
    RTDEXTSOK             = MAKE_RESULT( LYR_SEVERITY_SUCCESS, MODULE_RTDEXTS,  0 ),    /**< Success */
    RTDEXTSOUTOFMEMORY    = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXTS,  1 ),    /**< Failed to allocate memory. */
    RTDEXTSINVHANDLE      = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXTS,  2 )     /**< Timestamp invalid handle */
} RTDExTs_Error_t;

/**
 * RTDExTs channel event ID
 */
typedef enum RTDExTs_EventId
{
    RTDExTs_EventId_None = 0,         ///< No event
    RTDExTs_EventId_StartPending,     ///< Start pending
    RTDExTs_EventId_Started,          ///< Started
    RTDExTs_EventId_StopPending,      ///< Stop pending
    RTDExTs_EventId_Stopped,          ///< Stopped
    RTDExTs_EventId_FifoUnderrun,     ///< FIFO underrun (playback) 
    RTDExTs_EventId_FifoOverrun,      ///< FIFO overrun (capture)
    RTDExTs_EventId_FifoResume,       ///< Recover from a FIFO underrun (playback) or overrun (capture)
    RTDExTs_EventId_MissTimeCommand,  ///< Miss-timed command error
    RTDExTs_EventId_Num               ///< Number of RTDExTs events
} RTDExTs_EventId_t;

/**
 * RTDExTs channel events
 */
typedef struct RTDExTs_Event
{
    RTDExTs_EventId_t id;               ///< RTDExTs channel event ID
    uint32_t u32EventCount;             ///< Increases by one on every received event.
    int      gTiming;                   ///< General timing (0:Precise timing, 1:General timing)
    uint32_t u32SampleCount;            /**< Sample count of the first sample of data buffer at the moment of the event*/
    uint32_t u32secTime;                /**< POSIX time in second at the moment of the event (general) or exactly at the moment of the event (precise) */
    uint32_t u32nsecTime;               /**< Time offset in nanosecond from the POSIX time*/
    uint32_t u32secTimeCommand;         /**< POSIX time in second requested by the miss-timed command (only valid for MissTimeCommand event) */
    uint32_t u32nsecTimeCommand;        /**< Time offset in nanosecond from the POSIX time requested by the miss-timed command (only valid for MissTimeCommand event)*/
} RTDExTs_Event_t;


#ifdef __cplusplus
}
#endif
#endif    //RTDEXTS_DEF_H

