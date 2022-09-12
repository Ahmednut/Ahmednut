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
 * @file       MI125_mi125_regs.h
 * @brief      mi125 module related internal registers definitions
 *
 * $Date: 2015/06/03 18:26:17 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MI125_MI125
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

#ifndef __MI125_MI125_REGS__
#define __MI125_MI125_REGS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

/** @name MI125 Board register map
 * 
 *  Hold mi125 fpga core internal device registers definitions.
 */
typedef struct MI125_S_mi125_Register_Map
{
    union
    {
        unsigned int RegisterArray[10];
        struct 
        {
            union U_mi125_regs_mi125info
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int coreid:16, version:16;
#else
                    unsigned int version:16, coreid:16;
#endif                    
                }Bit;
                unsigned int raw;
            }Register_mi125info;

            union U_mi125_regs_mi125ctrl
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int AdcValid:16, reset_calib_detection:1, TriggerDelay:5, ADCClockMMCMRst:1, DataFormat:1, ChannelConfig:2, IPSoftRst:1, DigOutRandEn:1, TrigSyncRst:1, IdelayCtrlRst:1, IdelayRst:1, UpdateADCStatus:1;
#else
                    unsigned int UpdateADCStatus:1, IdelayRst:1, IdelayCtrlRst:1, IserdesRst:1, DigOutRandEn:1, IPSoftRst:1, ChannelConfig:2, DataFormat:1, ADCClockMMCMRst:1, TriggerDelay:5, reset_calib_detection:1, AdcValid:16;
#endif
                }Bit;
                unsigned int raw;
            }Register_mi125ctrl;

            union U_mi125_regs_mi125status
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int rsvd:24, FmcPositon:4, MI125ClkMaster:1, AdcClockMmcmLock:1, calib_detection_done:1, IdelayRdy:1;
#else
                    unsigned int IdelayRdy:1, calib_detection_done:1, AdcClockMmcmLock:1, MI125ClkMaster:1, FmcPositon:4, rsvd:24;
#endif
                }Bit;
                unsigned int raw;
            }Register_mi125status;

            union U_mi125_regs_mi125adcidelayvalue
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int rsvd:22, clkIdelay_value:5, adcIdelay_value:5;
#else
                    unsigned int adcIdelay_value:5, clkIdelay_value:5, rsvd:22;
#endif
                }Bit;
                unsigned int raw;
            }Register_mi125adcidelayvalue;

            union U_mi125_regs_mi125adcidelaywe
            {
                struct
                {
                    unsigned int adcIdelay_we:32;
                }Bit;
                unsigned int raw;
            }Register_mi125adcidelaywe;

            union U_mi125_regs_mi125bitslip
            {
                struct
                {
                    unsigned int bitslip:32;
                }Bit;
                unsigned int raw;
            }Register_mi125bitslip;

            union U_mi125_regs_mi125caliberror
            {
                struct
                {
                    unsigned int calib_error:32;
                }Bit;
                unsigned int raw;
            }Register_mi125caliberror;

            union U_mi125_regs_mi125calibpatternerror
            {
                struct
                {
                    unsigned int calib_pattern_error:32;
                }Bit;
                unsigned int raw;
            }Register_mi125calibpatternerror;

            union U_mi125_regs_mi125freqcntclk
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned int  rsvd:9, FreqCntRst:1, FreqCntClkSel:6, FreqCntClkCnt:16;
#else
                    unsigned int FreqCntClkCnt:16, FreqCntClkSel:6, FreqCntRst:1, rsvd:9;
#endif
                }Bit;
                unsigned int raw;
            }Register_mi125freqcntclk;

        };
    };
} MI125_mi125_Register_Map;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

