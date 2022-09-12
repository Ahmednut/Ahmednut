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
 * @file       Radio640_regs.h
 * @brief      radio640 module related internal registers definitions
 *
 * $Date: 2015/12/04 16:59:21 $
 * $Revision: 1.6 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RADIO640
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

#ifndef RADIO640_REGS_H__
#define RADIO640_REGS_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "bas_types.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define RADIO640_AD9361_REG_BASE_ADDR   0x0000
#define RADIO640_CUSTOM_REG_BASE_ADDR   0x8000

/** @name RADIO640 Board custom register map
 *
 *  Hold Radio640 FPGA core internal device custom registers definitions.
 */
typedef struct Radio640_stCrRegisterMap
{
    union
    {
        uint32_t au32Registers[5];
        struct
        {
            uint32_t u32TrxClkSpiReg;
            uint32_t u32TrxClkSpiReadReg;
            uint32_t u32DacOscSpiReg;
            uint32_t u32RfAttSpiReg;

            union Radio640_Parameter
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    uint32_t uReserved:19, uClockMaster:1, uDesignClkRate:4, uBoardPosition:4, uFmcConnector:4;
#else
                    uint32_t uFmcConnector:4, uBoardPosition:4, uDesignClkRate:4, uClockMaster:1, uReserved:19;
#endif
                };
                uint32_t u32Raw;
            }stParameter;

            union Radio640_IO
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    uint32_t uReserved2:2,
                             uTriggerDelay:5,
                             uTriggerBitSlip:1,
                             uCtrlInValue:4,
                             uCtrlInForce:4,
                             uClkResetPin:1,
                             uOscEnPin:1,
                             uAd9361ResetnPin:1,
                             uClkSwitch2:1,
                             uClkSwitch1:2,
                             uTrx2InRxDirForce:1,
                             uTrx2InRxDirValue:1,
                             uTrx1InRxDirForce:1,
                             uTrx1InRxDirValue:1,
                             uEnAgcValue:1,
                             uEnAgcForce:1,
                             uEnableValue:1,
                             uEnableForce:1,
                             uTxnrxValue:1,
                             uTxnrxForce:1;
#else
                    uint32_t uTxnrxForce:1,
                             uTxnrxValue:1,
                             uEnableForce:1,
                             uEnableValue:1,
                             uEnAgcForce:1,
                             uEnAgcValue:1,
                             uTrx1InRxDirValue:1,
                             uTrx1InRxDirForce:1,
                             uTrx2InRxDirValue:1,
                             uTrx2InRxDirForce:1,
                             uClkSwitch1:2,
                             uClkSwitch2:1,
                             uAd9361ResetnPin:1,
                             uOscEnPin:1,
                             uClkResetPin:1,
                             uCtrlInForce:4,
                             uCtrlInValue:4,
                             uTriggerBitSlip:1,
                             uTriggerDelay:5,
                             uReserved2:2;
#endif
                };
                uint32_t u32Raw;
            }stIO;
            
            union Radio640_CtrlOut
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    uint32_t uCtrlOutHigh:8, uCtrlOutLow:8, uCtrlOutCurrent:8, uReserved:7, uUpdateCtrlOutLatch:1;
#else
                    uint32_t uUpdateCtrlOutLatch:1, uReserved:7, uCtrlOutCurrent:8, uCtrlOutLow:8, uCtrlOutHigh:8;
#endif
                };
                uint32_t u32Raw;
            }stCtrlOut;
            
            uint32_t u32TestClkSelFreq;
            
            uint32_t u32TrxSpiClkDownsampling;

            union Radio640_IOLowHigh
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    uint32_t uReserved:21,
                             uTrigger:1,
                             uOutputSyncHigh:1,
                             uOutputSyncLow:1,
                             uInputSyncHigh:1,
                             uInputSyncLow:1,
                             uInputSync:1,
                             uUpdateOutputSyncLatch:1,
                             uUpdateInputSyncLatch:1,
                             uTriggerHigh:1,
                             uTriggerLow:1,
                             uUpdateTriggerLatch:1;
#else
                    uint32_t uUpdateTriggerLatch:1,
                             uTriggerLow:1,
                             uTriggerHigh:1,
                             uUpdateInputSyncLatch:1,
                             uUpdateOutputSyncLatch:1,
                             uInputSync:1,
                             uInputSyncLow:1,
                             uInputSyncHigh:1,
                             uOutputSyncLow:1,
                             uOutputSyncHigh:1,
                             uTrigger:1,
                             uReserved:21;
#endif
                };
                uint32_t u32Raw;
            }stIOLowHigh;
        };
    };
} Radio640_stCrRegisterMap;

#ifdef __cplusplus
}
#endif  // extern "C"

#endif

