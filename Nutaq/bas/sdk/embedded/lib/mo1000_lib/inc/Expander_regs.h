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
 * @file       Expander_regs.h
 * @brief      MO1000 IO expander internal registers definitions
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

#ifndef EXPANDER_REGS_H__
#define EXPANDER_REGS_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// internal device registers definitions
typedef struct Expander_StructRegisterMap
{
    union
    {
        unsigned char m_aucRegisters[0x08];
        struct 
        {
            union Expander_UnionRegister00
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucClkMuxSel1In:1,m_ucPllMuxSel0In:1,m_ucDciMuxSelIn:1,m_ucSpiBridgeResetNIn:1,m_ucDac58ResetNIn:1,m_ucDac14ResetNIn:1,m_ucLocalOscSwIn:1,m_ucClkMuxSel0In:1;
#else
                    unsigned char m_ucClkMuxSel0In:1,m_ucLocalOscSwIn:1,m_ucDac14ResetNIn:1,m_ucDac58ResetNIn:1,m_ucSpiBridgeResetNIn:1,m_ucDciMuxSelIn:1,m_ucPllMuxSel0In:1,m_ucClkMuxSel1In:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stRegister00;
            
            union Expander_UnionRegister01
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPllLockIn:1,m_ucDacIrqNIn:2,m_ucDac58PllLockIn:1,m_ucDac14PllLockIn:1,m_ucFrameMuxSelIn:1,m_ucPllSyncNIn:1,m_ucPllPowerDownNIn:1;
#else
                    unsigned char m_ucPllPowerDownNIn:1,m_ucPllSyncNIn:1,m_ucFrameMuxSelIn:1,m_ucDac14PllLockIn:1,m_ucDac58PllLockIn:1,m_ucDacIrqNIn:2,m_ucPllLockIn:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stRegister01;

            union Expander_UnionRegister02
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucClkMuxSel1Out:1,m_ucPllMuxSel0Out:1,m_ucDciMuxSelOut:1,m_ucSpiBridgeResetNOut:1,m_ucDac58ResetNOut:1,m_ucDac14ResetNOut:1,m_ucLocalOscSwOut:1,m_ucClkMuxSel0Out:1;
#else
                    unsigned char m_ucClkMuxSel0Out:1,m_ucLocalOscSwOut:1,m_ucDac14ResetNOut:1,m_ucDac58ResetNOut:1,m_ucSpiBridgeResetNOut:1,m_ucDciMuxSelOut:1,m_ucPllMuxSel0Out:1,m_ucClkMuxSel1Out:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stRegister02;
            
            union Expander_UnionRegister03
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPllLockOut:1,m_ucDacIrqNOut:2,m_ucDac58PllLockOut:1,m_ucDac14PllLockOut:1,m_ucFrameMuxSelOut:1,m_ucPllSyncNOut:1,m_ucPllPowerDownNOut:1;
#else
                    unsigned char m_ucPllPowerDownNOut:1,m_ucPllSyncNOut:1,m_ucFrameMuxSelOut:1,m_ucDac14PllLockOut:1,m_ucDac58PllLockOut:1,m_ucDacIrqNOut:2,m_ucPllLockOut:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stRegister03;
            
            union Expander_UnionRegister04
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucClkMuxSel1Pol:1,m_ucPllMuxSel0Pol:1,m_ucDciMuxSelPol:1,m_ucSpiBridgeResetNPol:1,m_ucDac58ResetNPol:1,m_ucDac14ResetNPol:1,m_ucLocalOscSwPol:1,m_ucClkMuxSel0Pol:1;
#else
                    unsigned char m_ucClkMuxSel0Pol:1,m_ucLocalOscSwPol:1,m_ucDac14ResetNPol:1,m_ucDac58ResetNPol:1,m_ucSpiBridgeResetNPol:1,m_ucDciMuxSelPol:1,m_ucPllMuxSel0Pol:1,m_ucClkMuxSel1Pol:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stRegister04;
            
            union Expander_UnionRegister05
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPllLockPol:1,m_ucDacIrqNPol:2,m_ucDac58PllLockPol:1,m_ucDac14PllLockPol:1,m_ucFrameMuxSelPol:1,m_ucPllSyncNPol:1,m_ucPllPowerDownNPol:1;
#else
                    unsigned char m_ucPllPowerDownNPol:1,m_ucPllSyncNPol:1,m_ucFrameMuxSelPol:1,m_ucDac14PllLockPol:1,m_ucDac58PllLockPol:1,m_ucDacIrqNPol:2,m_ucPllLockPol:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stRegister05;
            
            union Expander_UnionRegister06
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucClkMuxSel1Con:1,m_ucPllMuxSel0Con:1,m_ucDciMuxSelCon:1,m_ucSpiBridgeResetNCon:1,m_ucDac58ResetNCon:1,m_ucDac14ResetNCon:1,m_ucLocalOscSwCon:1,m_ucClkMuxSel0Con:1;
#else
                    unsigned char m_ucClkMuxSel0Con:1,m_ucLocalOscSwCon:1,m_ucDac14ResetNCon:1,m_ucDac58ResetNCon:1,m_ucSpiBridgeResetNCon:1,m_ucDciMuxSelCon:1,m_ucPllMuxSel0Con:1,m_ucClkMuxSel1Con:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stRegister06;
            
            union Expander_UnionRegister07
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char m_ucPllLockCon:1,m_ucDacIrqNCon:2,m_ucDac58PllLockCon:1,m_ucDac14PllLockCon:1,m_ucFrameMuxSelCon:1,m_ucPllSyncNCon:1,m_ucPllPowerDownNCon:1;
#else
                    unsigned char m_ucPllPowerDownNCon:1,m_ucPllSyncNCon:1,m_ucFrameMuxSelCon:1,m_ucDac14PllLockCon:1,m_ucDac58PllLockCon:1,m_ucDacIrqNCon:2,m_ucPllLockCon:1;
#endif                    
                }stBit;
                unsigned char m_ucRaw;
            }stRegister07;
        };
    };
} Expander_stRegisterMap;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

