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
 * @file    rtdexsync_def.h
 * @brief   RTDExSync public constants and types.
 *
 * $Date: 2015/08/10 17:10:40 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RTDEXSYNC
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
#ifndef RTDEX_SYNC_DEF_H
#define RTDEX_SYNC_DEF_H

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

/**
 * RTDExSync core ID
 */
#define RTDEXSYNC_COREID    0xCC02

/**
 * Supported RTDExSync core version
 */
#define RTDEXSYNC_COREVER   0x0100



/****************************************************************************
 *                               Public types                               *
 ****************************************************************************/

/**
 * RTDExSync Handle
 */
typedef struct RTDExSync_Obj *RTDExSync_Handle_t;

/**
 * RTDExSync direction.
 */
typedef enum RTDExSync_Dir
{
    RTDExSync_Dir_Uplink = 0,                   ///< Uplink : Device to Host
    RTDExSync_Dir_Downlink,                     ///< Downlnk: Host to Device
    RTDExSync_Dir_Num                           ///< Number of direction
} RTDExSync_Dir_t;

/**
 * RTDExSync operation mode.
 */
typedef enum RTDExSync_Mode
{
    RTDExSync_TrigMode_Immediately = 0,         ///< Start/Stop immediately
    RTDExSync_TrigMode_OnTrigger = 1,           ///< Start/Stop on trigger
    RTDExSync_TrigMode_AtTime = 2,           ///< Start/Stop at absolute time
    RTDExSync_TrigMode_Num                      ///< Number of trigger mode
} RTDExSync_Mode_t;

/**
 * RTDExSync trigger selection.
 */
typedef enum RTDExSync_TrigSel
{
    RTDExSync_TrigSel_NA = 0,                   // Not applicable
    RTDExSync_TrigSel_0  = 0,                   // RTDExSync Core Trigger Input 0
    RTDExSync_TrigSel_1  = 1,                   // RTDExSync Core Trigger Input 1
    RTDExSync_TrigSel_2  = 2,                   // RTDExSync Core Trigger Input 2
    RTDExSync_TrigSel_3  = 3,                   // RTDExSync Core Trigger Input 3
    RTDExSync_TrigSel_Num                       // Number of external trigger
} RTDExSync_TrigSel_t;

/**
 * RTDExSync errors (ADP style)
 */
typedef enum RTDExSync_Error
{
	RTDEXSYNCOK  		    = MAKE_RESULT( LYR_SEVERITY_SUCCESS, MODULE_RTDEXSYNC,  0 ),    /**< Success. */
	RTDEXSYNCFAILED  	    = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC,  1 ),    /**< RTDExSync generic error */
	RTDEXSYNCCOREABSENT  	= MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC,  2 ),    /**< Core absent. */
	RTDEXSYNCOUTOFMEMORY  	= MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC,  3 ),    /**< Failed to allocate memory. */
	RTDEXSYNCTIMEOUT  	    = MAKE_RESULT( LYR_SEVERITY_WARNING, MODULE_RTDEXSYNC,  4 ),    /**< Timed out. */
    RTDEXSYNCINVSUBFRSIZE   = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC,  5 ),    /**< Invalid subframe size. */
    RTDEXSYNCINVFRSIZE      = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC,  6 ),    /**< Invalid frame size. */
    RTDEXSYNCINVDIR         = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC,  7 ),    /**< Invalid channel direction. */
    RTDEXSYNCINVSTATE       = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC,  8 ),    /**< Invalid channel state. */
    RTDEXSYNCINVSTARTMODE   = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC,  9 ),    /**< Invalid start mode */
    RTDEXSYNCINVSTOPMODE    = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 10 ),    /**< Invalid stop mode */
    RTDEXSYNCINVTIME        = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 11 ),    /**< Invalid time */
    RTDEXSYNCINVTRIGSEL     = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 12 ),    /**< Invalid trigger selection */
    RTDEXSYNCINVPKTSIZE     = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 13 ),    /**< Invalid packet size */
    RTDEXSYNCINVPKTTYPE     = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 14 ),    /**< Invalid packet type */
    RTDEXSYNCINVEVTQUEUE    = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 15 ),    /**< Invalid event queue */
    RTDEXSYNCPROTOERROR     = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 16 ),    /**< Protocol error */
    RTDEXSYNCHOSTIPERR      = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 17 ),    /**< Failed to retrieve HOST IP address */
	RTDEXSYNCCHANNELABSENT  = MAKE_RESULT( LYR_SEVERITY_ERROR,   MODULE_RTDEXSYNC, 18 ),    /**< Specified RTDExSync channel is absent */
} RTDExSync_Error_t;

/**
 * RTDExSync channel event ID
 */
typedef enum RTDExSync_EventId
{
	RTDExSync_EventId_None = 0,         ///< No event
	RTDExSync_EventId_StartPending,     ///< Start pending
	RTDExSync_EventId_Started,          ///< Started
	RTDExSync_EventId_StopPending,      ///< Stop pending
	RTDExSync_EventId_Stopped,          ///< Stopped
	RTDExSync_EventId_FifoUnderrun,     ///< FIFO underrun (playback)
	RTDExSync_EventId_FifoOverrun,      ///< FIFO overrun (capture)
	RTDExSync_EventId_FifoResume,       ///< Recover from a FIFO underrun (playback) or overrun (capture)
	RTDExSync_EventId_MissTimeCommand,  ///< Miss-timed command error
    RTDExSync_EventId_Num               ///< Number of RTDExSync events
} RTDExSync_EventId_t;

/**
 * RTDExSync channel events
 */
typedef struct RTDExSync_Event
{
    RTDExSync_EventId_t id;             ///< RTDExSync channel event ID
    uint32_t u32EventCount;             ///< Increases by one on every received event.
    int      gTiming;                   ///< General timing (0:Precise timing, 1:General timing)
    uint32_t u32SampleCount;            /**< Sample count of the first sample of data buffer at the moment of the event */
    uint64_t u64TickCount;              /**< Tick count of the first sample of data buffer at the moment of
                                             the event  (general) or exactly at the moment of the event (precise) */
    uint64_t u64TickCommand;            /**< Time requested by the miss-timed command (only valid for MissTimeCommand event) */
} RTDExSync_Event_t;


#ifdef __cplusplus
}
#endif
#endif    //RTDEX_SYNC_DEF_H

