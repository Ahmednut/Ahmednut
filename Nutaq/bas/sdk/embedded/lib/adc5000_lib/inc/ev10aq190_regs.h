/****************************************************************************  
 *
 *    ****                              *                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    *****  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      ev10aq190_regs.h
 * 
 * @brief     ev10aq190 registers
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

#ifndef __EV10AQ190_REGS__
#define __EV10AQ190_REGS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

typedef struct S_ev10aq190_Register_Map
{
    union
    {
        unsigned short RegisterArray[0x10];
        struct
        {
            union U_ev10aq190_regs_00
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short type:8, branch_number:4, version_number:4;
#else
                    unsigned short version_number:4, branch_number:4, type:8;
#endif
                }Bit;
                unsigned short raw;
            }Register_00;

            union U_ev10aq190_regs_01
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved1:3, test:1, reserved2:3, bdw:1, coding:1, reserved3:1, standby:2, adc_mode:4;
#else
                    unsigned short adc_mode:4, standby:2, reserved3:1, coding:1, bdw:1, reserved2:3, test:1, reserved1:3;
#endif
                }Bit;
                unsigned short raw;
            }Register_01;

            union U_ev10aq190_regs_02
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:12, adcxup:4;
#else
                    unsigned short adcxup:4, reserved:12;
#endif
                }Bit;
                unsigned short raw;
            }Register_02;

            union U_ev10aq190_regs_03
            {
                struct
                {
                    unsigned short reserved:16;
                }Bit;
                unsigned short raw;
            }Register_03;

            union U_ev10aq190_regs_04
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:15, reset:1;
#else
                    unsigned short reset:1, reserved:15;
#endif
                }Bit;
                unsigned short raw;
            }Register_04;

            union U_ev10aq190_regs_05
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:13, flashm:2, test_mode:1;
#else
                    unsigned short test_mode:1, flashm:2, reserved:13;
#endif
                }Bit;
                unsigned short raw;
            }Register_05;

            union U_ev10aq190_regs_06
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:12, sync:4;
#else
                    unsigned short sync:4, reserved:12;
#endif
                }Bit;
                unsigned short raw;
            }Register_06;

            unsigned short reserved1[8];

            union U_ev10aq190_regs_0F
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:13, channel_selector:3;
#else
                    unsigned short channel_selector:3, reserved:13;
#endif
                }Bit;
                unsigned short raw;
            }Register_0F;

            union U_ev10aq190_regs_10
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved1:8, pcalctrl:2, gcalctrl:2, ocalctrl:2, reserved2:2;
#else
                    unsigned short reserved2:2, ocalctrl:2, gcalctrl:2, pcalctrl:2, reserved1:8;
#endif
                }Bit;
                unsigned short raw;
            }Register_10;

            union U_ev10aq190_regs_11
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:15, busy:1;
#else
                    unsigned short busy:1, reserved:15;
#endif
                }Bit;
                unsigned short raw;
            }Register_11;

            union U_ev10aq190_regs_12
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:15, stby:1;
#else
                    unsigned short stby:1, reserved:15;
#endif
                }Bit;
                unsigned short raw;
            }Register_12;

            union U_ev10aq190_regs_13
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:12, trimmer:4;
#else
                    unsigned short trimmer:4, reserved:12;
#endif
                }Bit;
                unsigned short raw;
            }Register_13;

            unsigned short reserved2[12];

            union U_ev10aq190_regs_20
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:6, external_offset:10;
#else
                    unsigned short external_offset:10, reserved:6;
#endif
                }Bit;
                unsigned short raw;
            }Register_20;

            union U_ev10aq190_regs_21
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:6, offset:10;
#else
                    unsigned short offset:10, reserved:6;
#endif
                }Bit;
                unsigned short raw;
            }Register_21;

            union U_ev10aq190_regs_22
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:6, external_gain:10;
#else
                    unsigned short external_gain:10, reserved:6;
#endif
                }Bit;
                unsigned short raw;
            }Register_22;

            union U_ev10aq190_regs_23
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:6, gain:10;
#else
                    unsigned short gain:10, reserved:6;
#endif
                }Bit;
                unsigned short raw;
            }Register_23;

            union U_ev10aq190_regs_24
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:6, external_phase:10;
#else
                    unsigned short external_phase:10, reserved:6;
#endif
                }Bit;
                unsigned short raw;
            }Register_24;

            union U_ev10aq190_regs_25
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned short reserved:6, phase:10;
#else
                    unsigned short phase:10, reserved:6;
#endif
                }Bit;
                unsigned short raw;
            }Register_25;
        };
    };
} ev10aq190_Register_Map;

#ifdef __cplusplus
}
#endif  // extern "C"

#endif

