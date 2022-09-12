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
 * @file       perseus611x_regs.h
 * @brief      Perseus611X definitions.
 *
 * $Date: 2015/05/15 19:47:47 $
 * $Revision: 1.1 $
 *
 * @ingroup PERSEUS611X
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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

#ifndef PERSEUS611X_REGS_H__
#define PERSEUS611X_REGS_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/
 
// This define matches for each vadj voltage setting from Carrier_eVadjValue_t enum, the related voltage
// value in 10mV from Carrier_eVadjValueLevels_t enum. Each item is in the order of Carrier_eVadjValue_t enum.
// Do not change this list content and order (see related Carrier_eVadjValue_t and Carrier_eVadjValueLevels_t)
#define PERSEUS611X_VADJ_VOLTAGE_LEVELS_DEFINITION { \
  VADJ_1_192V_LEVEL, VADJ_1_490V_LEVEL, VADJ_1_803V_LEVEL, \
  VADJ_2_503V_LEVEL, VADJ_DISABLE_LEVEL, VADJ_UNKNOWN_LEVEL \
}

#define PERSEUS611X_VADJ_VALUE_DEFINITION { \
  VADJ_1_192V, VADJ_1_490V, VADJ_1_803V, \
  VADJ_2_503V, VADJ_DISABLE, VADJ_UNKNOWN \
}


/** @name Perseus611X Board register map
 * 
 *  Hold Perseus611X fpga core internal device registers definitions.   
 */
typedef struct Perseus611X_StructRegisterMap
{
    union
    {
        unsigned int m_auRegisters[6];
        struct 
        {
            union Perseus611X_UnionInfo
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

            union Perseus611X_UnionCtrlStat
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:21, m_uPcieMsi:1, m_uPcieClockLock:1, m_uPciePresent:1, m_uDdr3InitDone:1, m_uFmc2ClkDir:1, m_uFmc2StackAbsent:1, m_uFmc2Absent:1, m_uFmc1ClkDir:1, m_uFmc1StackAbsent:1, m_uFmc1Absent:1, m_uDaughterAbsent:1;
#else
                    unsigned int m_uDaughterAbsent:1, m_uFmc1Absent:1, m_uFmc1StackAbsent:1, m_uFmc1ClkDir:1, m_uFmc2Absent:1, m_uFmc2StackAbsent:1, m_uFmc2ClkDir:1, m_uDdr3InitDone:1, m_uPciePresent:1, m_uPcieClockLock:1, m_uPcieMsi:1, m_uReserved:21;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stCtrlStat;

            union Perseus611X_UnionClkCtrl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:21, m_uMgtPllLol:1, m_uMgtPllClk2Los:1, m_uMgtPllClk1Los:1, m_uCtrlAmcTclkcToFmc2Clk3En:1, m_uCtrlAmcTclkcToFmc1Clk3En:1, m_uCtrlAmcTclkaToFmc2Clk2En:1, m_uCtrlAmcTclkaToFmc1Clk2En:1, m_uCtrlFclkaHighz:1, m_uMgtRefClk100En:1, m_uMgtRefClk156En:1, m_uMgtRefClk125En:1;
#else
                    unsigned int m_uMgtRefClk125En:1, m_uMgtRefClk156En:1, m_uMgtRefClk100En:1, m_uCtrlFclkaHighz:1, m_uCtrlAmcTclkaToFmc1Clk2En:1, m_uCtrlAmcTclkaToFmc2Clk2En:1, m_uCtrlAmcTclkcToFmc1Clk3En:1, m_uCtrlAmcTclkcToFmc2Clk3En:1, m_uMgtPllClk1Los:1, m_uMgtPllClk2Los:1, m_uMgtPllLol:1, m_uReserved:21;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stClkCtrl;

            union Perseus611X_UnionVadjResetCtrl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:25, m_uVadj2En:1, m_uVadj2Sel:2, m_uFpgaResetN:1, m_uVadj1En:1, m_uVadj1Sel:2;
#else
                    unsigned int m_uVadj1Sel:2, m_uVadj1En:1, m_uFpgaResetN:1, m_uVadj2Sel:2, m_uVadj2En:1, m_uReserved:25;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stVadjResetCtrl;

            union Perseus611X_UnionLedCtrl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:16, m_uLedGreenEn:8, m_uLedRedEn:8;
#else
                    unsigned int m_uLedRedEn:8, m_uLedGreenEn:8, m_uReserved:16;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stLedCtrl;

            union Perseus611X_UnionI2cCtrl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int m_uReserved:28, m_uI2cBusSel:2, m_uI2cGranted:1, m_uI2cRequest:1;
#else
                    unsigned int m_uI2cRequest:1, m_uI2cGranted:1, m_uI2cBusSel:2, m_uReserved:28;
#endif
                }stBit;
                unsigned int m_uRaw;
            }stI2cCtrl;

        };
    };
}Perseus611X_stRegisterMap;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

