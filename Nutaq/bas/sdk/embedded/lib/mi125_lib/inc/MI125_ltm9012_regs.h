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
 * @file       MI125_ltm9012_regs.h
 * @brief      ADC related ltm9012 internal registers definitions
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

#ifndef __MI125_LTM9012_REGS__
#define __MI125_LTM9012_REGS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// internal device registers definitions
typedef struct MI125_S_ltm9012_Register_Map
{
    union
    {
        unsigned char RegisterArray[0x05];
        struct 
        {
            union U_ltm9012_regs_a0
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reset:1, reserved:7;
#else
                    unsigned char reserved:7,reset:1;
#endif                
                }Bit;
                unsigned char raw;
            }Register_a0;

            union U_ltm9012_regs_a1
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char dcsoff:1, rand:1, twoscomp:1, sleep:1, nap_4:1, nap_3:1, nap_2:1, nap_1:1;
#else
                    unsigned char nap_1:1, nap_2:1, nap_3:1, nap_4:1, sleep:1, twoscomp:1, rand:1, dcsoff:1;
#endif                
                }Bit;
                unsigned char raw;
            }Register_a1;

            union U_ltm9012_regs_a2
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char ilvds:3, termon:1, outoff:1, outmode:3;
#else
                    unsigned char outmode:3,outoff:1, termon:1, ilvds:3;
#endif                
                }Bit;
                unsigned char raw;
            }Register_a2;

            union U_ltm9012_regs_a3
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char outtest:1, reserved:1, tp_msb:6;
#else
                    unsigned char tp_msb:6, reserved:1, outtest:1;
#endif                
                }Bit;
                unsigned char raw;
            }Register_a3;

            union U_ltm9012_regs_a4
            {
                struct
                {
                    unsigned char tp_lsb:8;
                }Bit;
                unsigned char raw;
            }Register_a4;
        };
    };
} MI125_ltm9012_Register_Map;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

