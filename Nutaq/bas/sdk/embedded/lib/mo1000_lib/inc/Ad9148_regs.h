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
 * @file       Ad9148_regs.h
 * @brief      DAC related internal registers definitions
 *
 * $Date: 2015/04/10 14:08:14 $
 * $Revision: 1.1 $
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

#ifndef AD9148_REGS_H__
#define AD9148_REGS_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// internal device registers definitions
typedef struct Ad9148_StructRegisterMap
{
    union
    {
        unsigned char m_aucRegisters[0x73];
        struct 
        {
            union Mo1000_UnionA00Comm
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSdioDirection:1, m_ucLsbMsbFirst:1, m_ucSoftwareReset:1, m_ucDacSpiSelect:1, m_ucReserved:4;
#else
                    unsigned char m_ucReserved:4, m_ucDacSpiSelect:1, m_ucSoftwareReset:1, m_ucLsbMsbFirst:1, m_ucSdioDirection:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA00Comm;

            union Mo1000_UnionA01PowerControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPowerDownDacSet1:1, m_ucPowerDownDacSet2:1, m_ucPowerDownDataRec:1, m_ucReserved:5;
#else
                    unsigned char m_ucReserved:5, m_ucPowerDownDataRec:1, m_ucPowerDownDacSet2:1, m_ucPowerDownDacSet1:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA01PowerControl;

            union Mo1000_UnionA02Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA02Reserved;

            union Mo1000_UnionA03DataFormat
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucBinaryFormat:1, m_ucQFirstEnable:1, m_ucDualPortMode:1, m_ucBusSwap:1, m_ucByteMode:1, m_ucByteSwap:1, m_ucReserved:2;
#else
                    unsigned char m_ucReserved:2, m_ucByteSwap:1, m_ucByteMode:1, m_ucBusSwap:1, m_ucDualPortMode:1, m_ucQFirstEnable:1, m_ucBinaryFormat:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA03DataFormat;

            union Mo1000_UnionA04InterruptEnable0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucEnablePllLockLost:1, m_ucEnablePllLock:1, m_ucEnableSyncLostLock:1, m_ucEnableSyncLock:1, m_ucReserved:1, m_ucEnableFifoSpiAligned:1, m_ucEnableFifoWarning1:1, m_ucEnableFifoWarning2:1;
#else
                    unsigned char m_ucEnableFifoWarning2:1, m_ucEnableFifoWarning1:1, m_ucEnableFifoSpiAligned:1, m_ucReserved:1, m_ucEnableSyncLock:1, m_ucEnableSyncLostLock:1, m_ucEnablePllLock:1, m_ucEnablePllLockLost:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA04InterruptEnable0;

            union Mo1000_UnionA05InterruptEnable1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:3, m_ucEnableAedComparePass:1, m_ucEnableAedCompareFail:1, m_ucEnableSedCompareFail:1, m_ucReserved1:2;
#else
                    unsigned char m_ucReserved1:2, m_ucEnableSedCompareFail:1, m_ucEnableAedCompareFail:1, m_ucEnableAedComparePass:1, m_ucReserved:3;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA05InterruptEnable1;

            union Mo1000_UnionA06EventFlag0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPllLockLost:1, m_ucPllLock:1, m_ucSyncLostLock:1, m_ucSyncLock:1, m_ucReserved:1, m_ucFifoSpiAligned:1, m_ucFifoWarning1:1, m_ucFifoWarning2:1;
#else
                    unsigned char m_ucFifoWarning2:1, m_ucFifoWarning1:1, m_ucFifoSpiAligned:1, m_ucReserved:1, m_ucSyncLock:1, m_ucSyncLostLock:1, m_ucPllLock:1, m_ucPllLockLost:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA06EventFlag0;

            union Mo1000_UnionA07EventFlag1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:3, m_ucAedComparePass:1, m_ucAedCompareFail:1, m_ucSedCompareFail:1, m_ucReserved1:2;
#else
                    unsigned char m_ucReserved1:2, m_ucSedCompareFail:1, m_ucAedCompareFail:1, m_ucAedComparePass:1, m_ucReserved:3;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA07EventFlag1;

            union Mo1000_UnionA08ClockReceiverControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucClkDutyCorrection:1, m_ucRefclkDutyCorrection:1, m_ucClkCrossCorrection:1, m_ucRefclkCrossCorrection:1, m_ucBit03Eq0:1, m_ucBitEq111:3;
#else
                    unsigned char m_ucBitEq111:3, m_ucBit03Eq0:1, m_ucRefclkCrossCorrection:1, m_ucClkCrossCorrection:1, m_ucRefclkDutyCorrection:1, m_ucClkDutyCorrection:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA08ClockReceiverControl;

            union Mo1000_UnionA09Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA09Reserved;

            union Mo1000_UnionA0APllControl0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPllEnable:1, m_ucPllManualEnable:1, m_ucManualVcoBand:6;
#else
                    unsigned char m_ucManualVcoBand:6, m_ucPllManualEnable:1, m_ucPllEnable:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA0APllControl0;

            union Mo1000_UnionA0BReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA0BReserved;

            union Mo1000_UnionA0CPllControl1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPllLoopBandwidth:3, m_ucBit04Eq0:1, m_ucBit03Eq1:1, m_ucBit02Eq0:1, m_ucBit01Eq0:1, m_ucBit00Eq1:1;
#else
                    unsigned char m_ucBit00Eq1:1, m_ucBit01Eq0:1, m_ucBit02Eq0:1, m_ucBit03Eq1:1, m_ucBit04Eq0:1, m_ucPllLoopBandwidth:3;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA0CPllControl1;

            union Mo1000_UnionA0DPllControl2
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucN2:2, m_ucReserved:1, m_ucPllCrossEnable:1, m_ucN0:2, m_ucN1:2;
#else
                    unsigned char m_ucN1:2, m_ucN0:2, m_ucPllCrossEnable:1, m_ucReserved:1, m_ucN2:2;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA0DPllControl2;

            union Mo1000_UnionA0EPllStatus0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:4, m_ucPllControlVoltage:4;
#else
                    unsigned char m_ucPllControlVoltage:4, m_ucReserved:4;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA0EPllStatus0;

            union Mo1000_UnionA0FPllStatus1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:2, m_ucVcoBandReadback:6;
#else
                    unsigned char m_ucVcoBandReadback:6, m_ucReserved:2;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA0FPllStatus1;

            union Mo1000_UnionA10SyncControl0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSyncEnable:1, m_ucFifoRateDataRateToggle:1, m_ucReserved:2, m_ucRisingEdgeSync:1, m_ucSyncAveraging:3;
#else
                    unsigned char m_ucSyncAveraging:3, m_ucRisingEdgeSync:1, m_ucReserved:2, m_ucFifoRateDataRateToggle:1, m_ucSyncEnable:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA10SyncControl0;

            union Mo1000_UnionA11SyncControl1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:2, m_ucSyncPhaseRequest:6;
#else
                    unsigned char m_ucSyncPhaseRequest:6, m_ucReserved:2;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA11SyncControl1;

            union Mo1000_UnionA12SyncStatus0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSyncLost:1, m_ucSyncLocked:1, m_ucReserved:6;
#else
                    unsigned char m_ucReserved:6, m_ucSyncLocked:1, m_ucSyncLost:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA12SyncStatus0;

            union Mo1000_UnionA13Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA13Reserved;

            union Mo1000_UnionA14DataReceiverControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved1:1, m_ucOneDci:1, m_ucReserved:6;
#else
                    unsigned char m_ucReserved:6, m_ucOneDci:1, m_ucReserved1:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA14DataReceiverControl;

            union Mo1000_UnionA15DataReceiverStatus
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucLvdsRcvrFrameHigh:1, m_ucLvdsRcvrFrameLow:1, m_ucLvdsRcvrDciHigh:1, m_ucLvdsRcvrDciLow:1, m_ucLvdsRcvrPortBHigh:1, m_ucLvdsRcvrPortBLow:1, m_ucLvdsRcvrPortAHigh:1, m_ucLvdsRcvrPortALow:1;
#else
                    unsigned char m_ucLvdsRcvrPortALow:1, m_ucLvdsRcvrPortAHigh:1, m_ucLvdsRcvrPortBLow:1, m_ucLvdsRcvrPortBHigh:1, m_ucLvdsRcvrDciLow:1, m_ucLvdsRcvrDciHigh:1, m_ucLvdsRcvrFrameLow:1, m_ucLvdsRcvrFrameHigh:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA15DataReceiverStatus;

            union Mo1000_UnionA16Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA16Reserved;

            union Mo1000_UnionA17FifoStatusControlPortA
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucFifoWarning1:1, m_ucFifoWarning2:1, m_ucFifoResetAligned:1, m_ucFifoSpiAckAligned:1, m_ucFifoSpiAlignRequesting:1, m_ucFifoPhaseOffset:3;
#else
                    unsigned char m_ucFifoPhaseOffset:3, m_ucFifoSpiAlignRequesting:1, m_ucFifoSpiAckAligned:1, m_ucFifoResetAligned:1, m_ucFifoWarning2:1, m_ucFifoWarning1:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA17FifoStatusControlPortA;

            union Mo1000_UnionA18FifoStatusPortA
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucFifoLevel:8;
#else
                    unsigned char m_ucFifoLevel:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA18FifoStatusPortA;

            union Mo1000_UnionA19FifoStatusControlPortB
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucFifoWarning1:1, m_ucFifoWarning2:1, m_ucFifoResetAligned:1, m_ucFifoSpiAckAligned:1, m_ucFifoSpiAlignRequesting:1, m_ucFifoPhaseOffset:3;
#else
                    unsigned char m_ucFifoPhaseOffset:3, m_ucFifoSpiAlignRequesting:1, m_ucFifoSpiAckAligned:1, m_ucFifoResetAligned:1, m_ucFifoWarning2:1, m_ucFifoWarning1:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA19FifoStatusControlPortB;

            union Mo1000_UnionA1AFifoStatusPortB
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucFifoLevel:8;
#else
                    unsigned char m_ucFifoLevel:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA1AFifoStatusPortB;

            union Mo1000_UnionA1BReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA1BReserved;

            union Mo1000_UnionA1CHb1Control
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucEnablePreMod:1, m_ucBypassSinc:1, m_ucReserved:3, m_ucHb1:2, m_ucBypassHb1:1;
#else
                    unsigned char m_ucBypassHb1:1, m_ucHb1:2, m_ucReserved:3, m_ucBypassSinc:1, m_ucEnablePreMod:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA1CHb1Control;

            union Mo1000_UnionA1DHb2Control
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:4, m_ucHb2:3, m_ucBypassHb2:1;
#else
                    unsigned char m_ucBypassHb2:1, m_ucHb2:3, m_ucReserved:4;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA1DHb2Control;

            union Mo1000_UnionA1EHb3Control
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucBypassDigitalGainAndPhase:1, m_ucReserved:3, m_ucHb3:3, m_ucBypassHb3:1;
#else
                    unsigned char m_ucBypassHb3:1, m_ucHb3:3, m_ucReserved:3, m_ucBypassDigitalGainAndPhase:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA1EHb3Control;

            union Mo1000_UnionA1FChipId
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucChipId:8;
#else
                    unsigned char m_ucChipId:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA1FChipId;

            union Mo1000_UnionA20CoeffIByte0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucBit07Eq0:1, m_ucCoeff1I:4, m_ucCoeff0I:3;
#else
                    unsigned char m_ucCoeff0I:3, m_ucCoeff1I:4, m_ucBit07Eq0:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA20CoeffIByte0;

            union Mo1000_UnionA21CoeffIByte1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucCoeff3I20:3, m_ucCoeff2I:5;
#else
                    unsigned char m_ucCoeff2I:5, m_ucCoeff3I20:3;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA21CoeffIByte1;

            union Mo1000_UnionA22CoeffIByte2
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucCoeff4I20:3, m_ucBit04Eq0:1, m_ucCoeff3I63:4;
#else
                    unsigned char m_ucCoeff3I63:4, m_ucBit04Eq0:1, m_ucCoeff4I20:3;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA22CoeffIByte2;

            union Mo1000_UnionA23CoeffIByte3
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucBit07Eq0:1, m_ucCoeff4I93:7;
#else
                    unsigned char m_ucCoeff4I93:7, m_ucBit07Eq0:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA23CoeffIByte3;

            union Mo1000_UnionA24CoeffQByte0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucBit07Eq0:1, m_ucCoeff1Q:4, m_ucCoeff0Q:3;
#else
                    unsigned char m_ucCoeff0Q:3, m_ucCoeff1Q:4, m_ucBit07Eq0:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA24CoeffQByte0;

            union Mo1000_UnionA25CoeffQByte1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucCoeff3Q20:3, m_ucCoeff2Q:5;
#else
                    unsigned char m_ucCoeff2Q:5, m_ucCoeff3Q20:3;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA25CoeffQByte1;

            union Mo1000_UnionA26CoeffQByte2
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucCoeff4Q20:3, m_ucBit04Eq0:1, m_ucCoeff3Q63:4;
#else
                    unsigned char m_ucCoeff3Q63:4, m_ucBit04Eq0:1, m_ucCoeff4Q20:3;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA26CoeffQByte2;

            union Mo1000_UnionA27CoeffQByte3
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucBit07Eq0:1, m_ucCoeff4Q93:7;
#else
                    unsigned char m_ucCoeff4Q93:7, m_ucBit07Eq0:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA27CoeffQByte3;

            union Mo1000_UnionA28IPhaseAdjLsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPhaseWordI70:8;
#else
                    unsigned char m_ucPhaseWordI70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA28IPhaseAdjLsb;

            union Mo1000_UnionA29IPhaseAdjMsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPhaseWordI98:8;
#else
                    unsigned char m_ucPhaseWordI98:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA29IPhaseAdjMsb;

            union Mo1000_UnionA2AQPhaseAdjLsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPhaseWordQ70:8;
#else
                    unsigned char m_ucPhaseWordQ70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA2AQPhaseAdjLsb;

            union Mo1000_UnionA2BQPhaseAdjMsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPhaseWordQ98:8;
#else
                    unsigned char m_ucPhaseWordQ98:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA2BQPhaseAdjMsb;

            union Mo1000_UnionA2CIDcOffsetLsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucDcOffsetI70:8;
#else
                    unsigned char m_ucDcOffsetI70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA2CIDcOffsetLsb;

            union Mo1000_UnionA2DIDcOffsetMsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucDcOffsetI158:8;
#else
                    unsigned char m_ucDcOffsetI158:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA2DIDcOffsetMsb;

            union Mo1000_UnionA2EQDcOffsetLsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucDcOffsetQ70:8;
#else
                    unsigned char m_ucDcOffsetQ70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA2EQDcOffsetLsb;

            union Mo1000_UnionA2FQDcOffsetMsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucDcOffsetQ158:8;
#else
                    unsigned char m_ucDcOffsetQ158:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA2FQDcOffsetMsb;

            union Mo1000_UnionA30IDacFscAdj
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucIDacFscAdj70:8;
#else
                    unsigned char m_ucIDacFscAdj70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA30IDacFscAdj;

            union Mo1000_UnionA31IDacControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucIDacSleep:1, m_ucReserved:5, m_ucIDacFscAdj98:2;
#else
                    unsigned char m_ucIDacFscAdj98:2, m_ucReserved:5, m_ucIDacSleep:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA31IDacControl;

            union Mo1000_UnionA32AuxIDacData
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucAuxIDacData70:8;
#else
                    unsigned char m_ucAuxIDacData70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA32AuxIDacData;

            union Mo1000_UnionA33AuxIDacControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucAuxIDacSign:1, m_ucAuxIDacCurrentDirection:1, m_ucAuxIDacPowerDown:1, m_ucReserved:3, m_ucAuxIDacData98:2;
#else
                    unsigned char m_ucAuxIDacData98:2, m_ucReserved:3, m_ucAuxIDacPowerDown:1, m_ucAuxIDacCurrentDirection:1, m_ucAuxIDacSign:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA33AuxIDacControl;

            union Mo1000_UnionA34QDacFscAdj
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucQDacFscAdj70:8;
#else
                    unsigned char m_ucQDacFscAdj70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA34QDacFscAdj;

            union Mo1000_UnionA35QDacControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucQDacSleep:1, m_ucReserved:5, m_ucQDacFscAdj98:2;
#else
                    unsigned char m_ucQDacFscAdj98:2, m_ucReserved:5, m_ucQDacSleep:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA35QDacControl;

            union Mo1000_UnionA36AuxQDacData
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucAuxQDacData70:8;
#else
                    unsigned char m_ucAuxQDacData70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA36AuxQDacData;

            union Mo1000_UnionA37AuxQDacControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucAuxQDacSign:1, m_ucAuxQDacCurrentDirection:1, m_ucAuxQDacPowerDown:1, m_ucReserved:3, m_ucAuxQDacData98:2;
#else
                    unsigned char m_ucAuxQDacData98:2, m_ucReserved:3, m_ucAuxQDacPowerDown:1, m_ucAuxQDacCurrentDirection:1, m_ucAuxQDacSign:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA37AuxQDacControl;

            union Mo1000_UnionA38SedS0L
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedComparePatternSample0L:8;
#else
                    unsigned char m_ucSedComparePatternSample0L:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA38SedS0L;

            union Mo1000_UnionA39SedS0H
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedComparePatternSample0H:8;
#else
                    unsigned char m_ucSedComparePatternSample0H:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA39SedS0H;

            union Mo1000_UnionA3ASedS1L
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedComparePatternSample1L:8;
#else
                    unsigned char m_ucSedComparePatternSample1L:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA3ASedS1L;

            union Mo1000_UnionA3BSedS1H
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedComparePatternSample1H:8;
#else
                    unsigned char m_ucSedComparePatternSample1H:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA3BSedS1H;

            union Mo1000_UnionA3CSedS2L
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedComparePatternSample2L:8;
#else
                    unsigned char m_ucSedComparePatternSample2L:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA3CSedS2L;

            union Mo1000_UnionA3DSedS2H
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedComparePatternSample2H:8;
#else
                    unsigned char m_ucSedComparePatternSample2H:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA3DSedS2H;

            union Mo1000_UnionA3ESedS3L
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedComparePatternSample3L:8;
#else
                    unsigned char m_ucSedComparePatternSample3L:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA3ESedS3L;

            union Mo1000_UnionA3FSedS3H
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedComparePatternSample3H:8;
#else
                    unsigned char m_ucSedComparePatternSample3H:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA3FSedS3H;

            union Mo1000_UnionA40SedControlStatus
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedCompareEnable:1, m_ucPortBErrorDetected:1, m_ucPortAErrorDetected:1, m_ucReserved:1, m_ucAutoClearEnable:1, m_ucPortBCompareFailed:1, m_ucPortACompareFailed:1, m_ucComparePassed:1;
#else
                    unsigned char m_ucComparePassed:1, m_ucPortACompareFailed:1, m_ucPortBCompareFailed:1, m_ucAutoClearEnable:1, m_ucReserved:1, m_ucPortAErrorDetected:1, m_ucPortBErrorDetected:1, m_ucSedCompareEnable:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA40SedControlStatus;

            union Mo1000_UnionA41SedRL
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedStatusRisingEdgeSamplesL:8;
#else
                    unsigned char m_ucSedStatusRisingEdgeSamplesL:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA41SedRL;

            union Mo1000_UnionA42SedRH
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedStatusRisingEdgeSamplesH:8;
#else
                    unsigned char m_ucSedStatusRisingEdgeSamplesH:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA42SedRH;

            union Mo1000_UnionA43SedFL
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedStatusFallingEdgeSamplesL:8;
#else
                    unsigned char m_ucSedStatusFallingEdgeSamplesL:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA43SedFL;

            union Mo1000_UnionA44SedFH
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucSedStatusFallingEdgeSamplesH:8;
#else
                    unsigned char m_ucSedStatusFallingEdgeSamplesH:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA44SedFH;

            union Mo1000_UnionA45Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA45Reserved;

            union Mo1000_UnionA46Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA46Reserved;

            union Mo1000_UnionA47Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA47Reserved;

            union Mo1000_UnionA48Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA48Reserved;

            union Mo1000_UnionA49Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA49Reserved;

            union Mo1000_UnionA4AReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA4AReserved;

            union Mo1000_UnionA4BReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA4BReserved;

            union Mo1000_UnionA4CReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA4CReserved;

            union Mo1000_UnionA4DReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA4DReserved;

            union Mo1000_UnionA4EReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA4EReserved;

            union Mo1000_UnionA4FReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA4FReserved;

            union Mo1000_UnionA50IGainControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucIGain:8;
#else
                    unsigned char m_ucIGain:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA50IGainControl;

            union Mo1000_UnionA51QGainControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucQGain:8;
#else
                    unsigned char m_ucQGain:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA51QGainControl;

            union Mo1000_UnionA52Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA52Reserved;

            union Mo1000_UnionA53Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA53Reserved;

            union Mo1000_UnionA54FtwLsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucFtw70:8;
#else
                    unsigned char m_ucFtw70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA54FtwLsb;

            union Mo1000_UnionA55Ftw2
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucFtw158:8;
#else
                    unsigned char m_ucFtw158:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA55Ftw2;

            union Mo1000_UnionA56Ftw3
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucFtw2316:8;
#else
                    unsigned char m_ucFtw2316:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA56Ftw3;

            union Mo1000_UnionA57FtwMsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucFtw3124:8;
#else
                    unsigned char m_ucFtw3124:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA57FtwMsb;

            union Mo1000_UnionA58PhaseOffsetMsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucNcoPhaseOffset158:8;
#else
                    unsigned char m_ucNcoPhaseOffset158:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA58PhaseOffsetMsb;

            union Mo1000_UnionA59PhaseOffsetLsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucNcoPhaseOffset70:8;
#else
                    unsigned char m_ucNcoPhaseOffset70:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA59PhaseOffsetLsb;

            union Mo1000_UnionA5ADdsModControl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucBypassDdsMod:1, m_ucReserved:1, m_ucFrameNcoResetAck:1, m_ucFrameNcoResetRequest:1, m_ucFtwUpdateAck:1, m_ucFtwUpdateRequest:1, m_ucReserved2:1, m_ucSidebandSelect:1;
#else
                    unsigned char m_ucSidebandSelect:1, m_ucReserved2:1, m_ucFtwUpdateRequest:1, m_ucFtwUpdateAck:1, m_ucFrameNcoResetRequest:1, m_ucFrameNcoResetAck:1, m_ucReserved:1, m_ucBypassDdsMod:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA5ADdsModControl;

            union Mo1000_UnionA5BReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA5BReserved;

            union Mo1000_UnionA5CDieTempControl0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:6, m_ucLatchTempData:1, m_ucTempSensorPowerDown:1;
#else
                    unsigned char m_ucTempSensorPowerDown:1, m_ucLatchTempData:1, m_ucReserved:6;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA5CDieTempControl0;

            union Mo1000_UnionA5DDieTempControl1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucDieTempControl1:8;
#else
                    unsigned char m_ucDieTempControl1:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA5DDieTempControl1;

            union Mo1000_UnionA5EDieTempLsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucDieTempLsb:8;
#else
                    unsigned char m_ucDieTempLsb:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA5EDieTempLsb;

            union Mo1000_UnionA5FDieTempMsb
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucDieTempMsb:8;
#else
                    unsigned char m_ucDieTempMsb:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA5FDieTempMsb;

            union Mo1000_UnionA60Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA60Reserved;

            union Mo1000_UnionA61Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA61Reserved;

            union Mo1000_UnionA62Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA62Reserved;

            union Mo1000_UnionA63Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA63Reserved;

            union Mo1000_UnionA64Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA64Reserved;

            union Mo1000_UnionA65Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA65Reserved;

            union Mo1000_UnionA66Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA66Reserved;

            union Mo1000_UnionA67Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA67Reserved;

            union Mo1000_UnionA68Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA68Reserved;

            union Mo1000_UnionA69Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA69Reserved;

            union Mo1000_UnionA6AReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA6AReserved;

            union Mo1000_UnionA6BReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA6BReserved;

            union Mo1000_UnionA6CReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA6CReserved;

            union Mo1000_UnionA6DReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA6DReserved;

            union Mo1000_UnionA6EReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA6EReserved;

            union Mo1000_UnionA6FReserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA6FReserved;

            union Mo1000_UnionA70Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA70Reserved;

            union Mo1000_UnionA71Reserved
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:8;
#else
                    unsigned char m_ucReserved:8;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA71Reserved;

            union Mo1000_UnionA72DciDelay
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucReserved:6, m_ucDciDelay:2;
#else
                    unsigned char m_ucDciDelay:2, m_ucReserved:6;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stA72DciDelay;
        };
    };
} Ad9148_stRegisterMap;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

