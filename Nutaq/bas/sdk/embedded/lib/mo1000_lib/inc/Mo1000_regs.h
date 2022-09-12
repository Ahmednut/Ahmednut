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
 * @file       Mo1000_regs.h
 * @brief      mo1000 module related internal registers definitions
 *
 * $Date: 2015/05/15 17:49:48 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MO1000
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

#ifndef MO1000_REGS_H__
#define MO1000_REGS_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

/** @name MO1000 Board register map
 * 
 *  Hold mo1000 fpga core internal device registers definitions.
 *  @warning
 *  - The way the m_uFrameMode logic (in stControl register) is designed could cause
 *    erratic behavior in some situations. Changing from mode 0, 2 or 3 to another mode
 *    never causes any problem, the frame mode change is done at the end of a 16 dci
 *    clocks period, always at the same synchronous moment. Changing from mode 1
 *    to a different frame mode is changed asynchronously at the moment that it is
 *    written (because the pulse logic is suspended after 16 dci clock periods in single pulse mode
 *    and ready to change when a frame mode is reprogrammed). Because the frame mode is
 *    critical to be synchronous when the dacs synchronisation is done, operations that could
 *    cause frame mode to be changed asynchronously should be avoided after that moment:
 *    writing stControl.m_uRaw, writing stControl.stBit.m_uDciInvert, writing stControl.stBit.m_uFramePattern,
 *    writing stControl.stBit.m_uFrameMode when the current m_uFrameMode is 1.         
 */
typedef struct Mo1000_StructRegisterMap
{
    union
    {
        unsigned int m_auRegisters[9];
        struct 
        {
            union Mo1000_UnionInfo
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uCoreId:16, m_uCoreVersion:16;
#else
                    unsigned int m_uCoreVersion:16, m_uCoreId:16;
#endif                    
                }stBit;
                unsigned int m_uRaw;
            }stInfo;

            union Mo1000_UnionControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:3, m_uDciInvert:1, m_uFramePattern:2, m_uFrameMode:2, m_uDataSerdesPd:8, m_uGlobalSerdesPd:1, m_uDataPatternEn:1, m_uDacReady:8, m_uUpdateDacReady:1, m_uMmcmRst:1, m_uSerdesRst:1, m_uDelayCtrlRst:1, m_uDelayRst:1, m_uCoreRst:1;
#else
                    unsigned int m_uCoreRst:1, m_uDelayRst:1, m_uDelayCtrlRst:1, m_uSerdesRst:1,  m_uMmcmRst:1, m_uUpdateDacReady:1, m_uDacReady:8, m_uDataPatternEn:1, m_uGlobalSerdesPd:1, m_uDataSerdesPd:8, m_uFrameMode:2, m_uFramePattern:2, m_uDciInvert:1, m_uReserved:3;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stControl;

            union Mo1000_UnionStatus
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:25, m_uFmcPositon:4, m_uMmcmLocked:1, m_uDelayCtrlRdy:1, m_uClockMaster:1;
#else
                    unsigned int  m_uClockMaster:1, m_uDelayCtrlRdy:1, m_uMmcmLocked:1, m_uFmcPositon:4, m_uReserved:25;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stStatus;

            union Mo1000_UnionDelayCtrl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:25, m_uDelayWeFrame:1, m_uDelayWeDci:1, m_uDelayValue:5;
#else
                    unsigned int m_uDelayValue:5, m_uDelayWeDci:1, m_uDelayWeFrame:1, m_uReserved:25;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stDelayCtrl;

            union Mo1000_UnionDelayData
            {
                struct
                {
                    unsigned int m_uDelayWeData:32;
                }stBit;
                unsigned int m_uRaw;
            }stDelayData;

            union Mo1000_UnionPatternAB
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uPatternB:16, m_uPatternA:16;
#else
                    unsigned int m_uPatternA:16, m_uPatternB:16;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stPatternAB;

            union Mo1000_UnionPatternCD
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uPatternD:16, m_uPatternC:16;
#else
                    unsigned int m_uPatternC:16, m_uPatternD:16;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stPatternCD;

            union Mo1000_UnionFreqCntClk
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:13, m_uFreqCntSel:1, m_uFreqCntRdy:1, m_uFreqCntRst:1, m_uFreqCnt:16;
#else
                    unsigned int m_uFreqCnt:16, m_uFreqCntRst:1, m_uFreqCntRdy:1, m_uFreqCntSel:1, m_uReserved:13;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stFreqCntClk;

            union Mo1000_UnionTriggerDelay
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:27, m_uTriggerDelay:5;
#else
                    unsigned int m_uTriggerDelay:5, m_uReserved:27;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stTriggerDelay;
        };
    };
} Mo1000_stRegisterMap;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

