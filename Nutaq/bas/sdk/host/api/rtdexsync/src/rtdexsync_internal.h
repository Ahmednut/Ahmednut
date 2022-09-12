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
 * @file    rtdexsync_internal.h
 * @brief   Private definitions for the RTDEx Sync module. 
 *
 * $Date: 2015/08/10 17:10:41 $
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
#ifndef RTDEXSYNC_INTERNAL_H__
#define RTDEXSYNC_INTERNAL_H__

#ifdef __cplusplus
    extern "C" {
#endif

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#include "rtdexsync_vita49.h"
#include "connection_state.h"       // connection_state

/****************************************************************************
 *                            Private constants                             * 
 ****************************************************************************/ 

        /* 
 * RTDExSync channel info tructure.
 */
typedef struct RTDExSync_ChStat 
{
    uint32_t u32FrameCount;
    uint32_t u32LostFrameCount;
    uint32_t u32DropFrameCount;
    uint32_t u32BadFrameCount;
    uint32_t u32HwErrCode;
} RTDExSync_ChStat_t;

/**
 * RTDExSync channel state.
 */
typedef enum RTDExSync_ChState
{
    RTDExSync_ChState_Idle  = 0,
    RTDExSync_ChState_Armed,
    RTDExSync_ChState_Running,
    RTDExSync_ChState_StopPending,
    RTDExSync_ChState_Stopped,
    RTDExSync_ChState_Num,
} RTDExSync_ChState_t;


/*****************************************************************************
 *  RTDExSync Object Definition
 */

/**
 * RTDExSync Object.
 */
typedef struct RTDExSync_Obj
{
    uint32_t u32MaxIQSamples;                   ///< Maximum transfer size in IQ samples (set by the user, limited by the maximum VRL size)

    // Chassis
    connection_state state;                     ///< Connection state (platform CCE)
    eMediaType eRTDExMedia;                     ///< RTDEx media (ethernet or PCIe)
    uint32_t u32RTDExEthBaseAddr;               ///< RTDEx Ethernet core base address

    // Events
    struct
    {
        int overrun;                            ///< Overrun event flag
        uint32_t u32EventCnt;                   ///< Event count
    } evt;

    // Channel info
    struct
    {
        uint8_t u8Idx;                          ///< RTDExSync channel number (0:7)
        uint16_t u16SessionId;                  ///< Current session ID
        RTDExSync_Dir_t dir;                    ///< RTDExSync channel direction (0:uplink, 1:downlink)    
        RTDExSync_ChState_t localState;         ///< RTDExSync local channel state
        RTDExSync_ChState_t remoteState;        ///< RTDExSync remote channel state
    } ch;

    // Start mode
    struct
    {
        RTDExSync_Mode_t mode;                  ///< Trigger mode (Immediate, OnTrigger, AtTime)
        RTDExSync_TrigSel_t trigSel;            ///< Trigger selection (0, 1, 2 or 3)
        uint64_t u64TickTime;                   ///< Tick time
    } start;

    // Stop mode
    struct
    {
        RTDExSync_Mode_t mode;                  ///< Trigger mode (Immediate, OnTrigger, AtTime)
        RTDExSync_TrigSel_t trigSel;            ///< Trigger selection (0, 1, 2 or 3)
        uint64_t u64TickTime;                   ///< Tick time
    } stop;

    // TX (Host -> Device)
    struct
    {
        adp_handle_rtdex_conn hRTDEx;           ///< TX RTDEx handle (raw)
        uint32_t u32MediaFrameSize;             ///< Media transport frame size in bytes (set by the user, limited by the media capacity)
        uint32_t u22MaxFrameSize;               ///< Maximum frame size in TX (in bytes, must be multiple eof 4) 
        uint32_t *pu32Buffer;                   ///< TX buffer

        uint16_t u12VrlFrameCount;              ///< VRL frame count number
        uint8_t  u4VrtPacketCount;              ///< VRT packet count number
        uint32_t u32SampleCount;                ///< Sample count
    } tx;

    // RX (Device -> Host)
    struct
    {
        adp_handle_rtdex_conn hRTDEx;           ///< RX RTDEx handle (raw)
        uint32_t u32MediaFrameSize;             ///< Media transport frame size in bytes (set by the user, limited by the media capacity)
        uint32_t u22MaxFrameSize;               ///< Maximum frame size in RX (in bytes, must be multiple eof 4) 
        uint32_t *pu32Buffer;                   ///< RX buffer

        RTDExSync_VrtPacket_t vrtPkt;           ///< Decoded VRT packet
        uint16_t u16ReadIdx;                    ///< RX sample read index
        uint16_t u16ReadCnt;                    ///< Number os sample remaining in the RX packet
        
        uint16_t u12VrlFrameCount;              ///< VRL frame count number
        uint8_t  u4VrtPacketCount;              ///< VRT packet count number
        uint32_t u32SampleCount;                ///< Sample count

        uint32_t u32MissVrlFrameCount;          ///< Miss VRL frame count number
        uint32_t u32MissCrtPacketCount;         ///< Miss VRL frame count number
    } rx;

    ///< Remote Indicators
    struct
    {
        uint32_t calTimeChInd   : 1;            ///< Calibrated time change indicator 
        uint32_t calTimeInd     : 1;            ///< Calibrated time indicator 
        uint32_t validDataChInd : 1;            ///< Valid data change indicator 
        uint32_t validDataInd   : 1;            ///< Valid data indicator
        uint32_t refLockChInd   : 1;            ///< Reference lock change indicator 
        uint32_t refLockInd     : 1;            ///< Reference lock indicator
        uint32_t sampLossChInd  : 1;            ///< Sample loss change indicator 
        uint32_t sampLossInd    : 1;            ///< Sample loss indicator
        uint32_t chStateChInd   : 1;            ///< Channel state change indicator 
        uint32_t chStateInd     : 2;            ///< Channel state indicator
    } ind;

} RTDExSync_Obj_t;


#ifdef __cplusplus
}
#endif

#endif  // RTDEXSYNC_INTERNAL_H__

