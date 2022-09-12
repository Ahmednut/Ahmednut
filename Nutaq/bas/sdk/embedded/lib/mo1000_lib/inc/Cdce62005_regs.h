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
 * @file    Cdce62005_regs.h
 * @brief   Ccdce62005 PLL registers definitions
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
#ifndef CDCE62005_REGS_H__
#define CDCE62005_REGS_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

#define CDCE62005_OUTPUTMODELVCMOSACTIVE 0x0
#define CDCE62005_OUTPUTMODELVCMOSINVERTED 0x5
#define CDCE62005_OUTPUTMODELVCMOSLOW 0xf
#define CDCE62005_OUTPUTMODELVCMOSTRISTATE 0xa
#define CDCE62005_OUTPUTMODELVCMOSDISABLED 0xb
#define CDCE62005_OUTPUTMODELVPECL 0x20
#define CDCE62005_OUTPUTMODELVDS 0x1A

#define CDCE62005_CLOCKSELFROMPIN 0x0
#define CDCE62005_CLOCKSELPRI 0x1
#define CDCE62005_CLOCKSELSEC 0x2
#define CDCE62005_CLOCKSELAUTOPRISEC 0x3
#define CDCE62005_CLOCKSELAUX 0x4
#define CDCE62005_CLOCKSELAUTOPRISECAUX 0x7

#define CDCE62005_BUFFERSELLVDS 0x0
#define CDCE62005_BUFFERSELLVPECL 0x2
#define CDCE62005_BUFFERSELLVCMOS 0x3

// internal device registers definitions
typedef struct Cdce62005_StructRegisterMap
{
    union
    {
        unsigned m_auRegisters[9];
        struct 
        {
            union Cdce62005_UnionRegister00
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned m_uPadding:4, m_uOutputMode:6, m_uHiSwing:1, m_uDividerEnable:1, m_uDivider:7, m_uPhaseAdjust:7, m_uOutputMux:2, m_uReserved:2, m_uPrimaryPredivider:2;
#else
                    unsigned m_uPrimaryPredivider:2, m_uReserved:2, m_uOutputMux:2, m_uPhaseAdjust:7, m_uDivider:7, m_uDividerEnable:1, m_uHiSwing:1, m_uOutputMode:6, m_uPadding:4;
#endif
                }stBit;
                unsigned m_uRaw;
            }stRegister00;

            union Cdce62005_UnionRegister01
            {
                struct
                {
#ifdef _BIG_ENDIAN    
                    unsigned m_uPadding:4, m_uOutputMode:6, m_uHiSwing:1, m_uDividerEnable:1, m_uDivider:7, m_uPhaseAdjust:7, m_uOutputMux:2, m_uReserved:2, m_uSecondaryPredivider:2;
#else
                    unsigned m_uSecondaryPredivider:2, m_uReserved:2, m_uOutputMux:2, m_uPhaseAdjust:7, m_uDivider:7, m_uDividerEnable:1, m_uHiSwing:1, m_uOutputMode:6, m_uPadding:4;
#endif
                }stBit;
                unsigned m_uRaw;
            }stRegister01;

            union Cdce62005_UnionRegister02
            {
                struct
                {
#ifdef _BIG_ENDIAN            
                    unsigned m_uPadding:4, m_uOutputMode:6, m_uHiSwing:1, m_uDividerEnable:1, m_uDivider:7, m_uPhaseAdjust:7, m_uOutputMux:2, m_uReserved:2, m_uReferenceDivider:2;
#else
					unsigned m_uReferenceDivider:2, m_uReserved:2, m_uOutputMux:2, m_uPhaseAdjust:7, m_uDivider:7, m_uDividerEnable:1, m_uHiSwing:1, m_uOutputMode:6, m_uPadding:4;
#endif
                }stBit;
                unsigned m_uRaw;
            }stRegister02;

            union Cdce62005_UnionRegister03
            {
                struct
                {
#ifdef _BIG_ENDIAN                    
                    unsigned m_uPadding:4, m_uOutputMode:6, m_uHiSwing:1, m_uDividerEnable:1, m_uDivider:7, m_uPhaseAdjust:7, m_uOutputMux:2, m_uReserved:3, m_uReferenceDivider:1;
#else
					unsigned m_uReferenceDivider:1, m_uReserved:3, m_uOutputMux:2, m_uPhaseAdjust:7, m_uDivider:7, m_uDividerEnable:1, m_uHiSwing:1, m_uOutputMode:6, m_uPadding:4;
#endif
                }stBit;
                unsigned m_uRaw;
            }stRegister03;

            union Cdce62005_UnionRegister04
            {
                struct
                {
#ifdef _BIG_ENDIAN                    
                    unsigned m_uPadding:4, m_uOutputMode:6, m_uHiSwing:1, m_uDividerEnable:1, m_uDivider:7, m_uPhaseAdjust:7, m_uOutputMux:2, m_uReserved2:2, m_uSyncMode1:1, m_uReserved1:1;
#else
					unsigned m_uReserved1:1, m_uSyncMode1:1,m_uReserved2:2, m_uOutputMux:2, m_uPhaseAdjust:7, m_uDivider:7, m_uDividerEnable:1, m_uHiSwing:1, m_uOutputMode:6, m_uPadding:4;
#endif
                }stBit;
                unsigned m_uRaw;
            }stRegister04;

            union Cdce62005_UnionRegister05
            {
                struct
                {
#ifdef _BIG_ENDIAN              
                    unsigned m_uPadding:4, m_uDigitalLockEn:1, m_uLongLock:1, m_uLockWindows:4, m_uInputDivider:8, m_uReserved:2, m_uFailSafe:1, m_uSecondaryInputBiased:1, m_uPrimaryInputBiased:1, m_uPrimaryInputTerminationDis:1, m_uBufferHysteresis:1, m_uDcTerminationEn:1, m_uSoftInputSel:1, m_uClockSelect:3, m_uBufSelect:2;
#else
					unsigned m_uBufSelect:2, m_uClockSelect:3, m_uSoftInputSel:1, m_uDcTerminationEn:1, m_uBufferHysteresis:1, m_uPrimaryInputTerminationDis:1, m_uPrimaryInputBiased:1, m_uSecondaryInputBiased:1, m_uFailSafe:1, m_uReserved:2, m_uInputDivider:8, m_uLockWindows:4, m_uLongLock:1, m_uDigitalLockEn:1, m_uPadding:4;
#endif
                }stBit;
                unsigned m_uRaw;
            }stRegister05;

            union Cdce62005_UnionRegister06
            {
                struct
                {
#ifdef _BIG_ENDIAN                    
                    unsigned m_uPadding:4, m_uPllCalibration:1, m_uExternalLoopFilter:1, m_uAuxSel:1, m_uAuxEn:1, m_uReserved3:1, m_uVcoCalibration:1, m_uChargePumpPulseWidth:1, m_uSyncMode2:1, m_uChargePumpCurrent:4, m_uDividerBypass:3, m_uSecondaryInputTerminationDis:1, m_uReserved1:1, m_uFeedbackDivider:8, m_uPrescaler:2, m_uVcoRange:1;
#else
					unsigned m_uVcoRange:1, m_uPrescaler:2, m_uFeedbackDivider:8, m_uReserved1:1, m_uSecondaryInputTerminationDis:1, m_uDividerBypass:3, m_uChargePumpCurrent:4, m_uSyncMode2:1, m_uChargePumpPulseWidth:1, m_uVcoCalibration:1, m_uReserved3:1, m_uAuxEn:1, m_uAuxSel:1, m_uExternalLoopFilter:1, m_uPllCalibration:1, m_uPadding:4;
#endif       	
                }stBit;
                unsigned m_uRaw;
            }stRegister06;

            union Cdce62005_UnionRegister07
            {
                struct
                {
#ifdef _BIG_ENDIAN                    
                    unsigned m_uPadding:4, m_uReserved3:1, m_uEepromLock:1, m_uSelDel1:1, m_uReserved0:1, m_uSelDel2:1, m_uReserved1:1, m_uReserved2:1, m_uLoopFilterR3:2, m_uLoopFilterC3:4, m_uLoopFilterC1:5, m_uLoopFilterR2:5, m_uLoopFilterC2:5;
#else                  
					unsigned m_uLoopFilterC2:5, m_uLoopFilterR2:5, m_uLoopFilterC1:5, m_uLoopFilterC3:4, m_uLoopFilterR3:2, m_uReserved2:1, m_uReserved1:1, m_uSelDel2:1, m_uReserved0:1, m_uSelDel1:1, m_uEepromLock:1, m_uReserved3:1, m_uPadding:4;    
#endif          
                }stBit;
                unsigned m_uRaw;
            }stRegister07;

            union Cdce62005_UnionRegister08
            {
                struct
                {
#ifdef _BIG_ENDIAN                                   
                    unsigned m_uPadding:4, m_uAuxActivity:1, m_uSecActivity:1, m_uPriActivity:1, m_uTestConfig:4, m_uReserved1:1, m_uCalibrationWord:6, m_uReserved2:1, m_uVersion:3, m_uReserved3:1, m_uSync:1, m_uSleep:1, m_uPllLock:1, m_uCalWord:6;
#else                  
					unsigned m_uCalWord:6, m_uPllLock:1, m_uSleep:1, m_uSync:1, m_uReserved3:1, m_uVersion:3, m_uReserved2:1, m_uCalibrationWord:6, m_uReserved1:1, m_uTestConfig:4, m_uPriActivity:1, m_uSecActivity:1, m_uAuxActivity:1, m_uPadding:4;    
#endif          
                }stBit;
                unsigned m_uRaw;
            }stRegister08;
         };
     };
} Cdce62005_stRegisterMap;

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

