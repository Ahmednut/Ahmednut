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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       adc5000_cpld_regs.h
 * @brief      ADC5000 CPLD module related internal registers definitions
 *
 * $Date: 2015/04/10 14:08:12 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup ADC5000
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

#ifndef __ADC5000_CPLD_REGS__
#define __ADC5000_CPLD_REGS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

/** @name ADC5000 CPLD register map
 * 
 *  Hold ADC5000 CPLD internal device registers definitions.
 */
typedef struct S_adc5000_cpld_Register_Map
{
    union
    {
        unsigned char RegisterArray[0x3];
        struct 
        {
            union U_adc5000_cpld_regs_00
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:1, adc_reset:1, pll_reset:1, sync_src:2, clk_src:3;
#else
                    unsigned char clk_src:3, sync_src:2, pll_reset:1, adc_reset:1, reserved:1;
#endif
                }Bit;
                unsigned char raw;
            }Register_00;

            union U_adc5000_cpld_regs_01
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:2, fan1:1, fan0:1, dir3:1, dir2:1, dir1:1, dir0:1;
#else
                    unsigned char dir0:1, dir1:1, dir2:1, dir3:1, fan0:1, fan1:1, reserved:2;
#endif
                }Bit;
                unsigned char raw;
            }Register_01;

            union U_adc5000_cpld_regs_02
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:3, irq:1, pll_int:1, pll_status:1, pll_ld:1, pll_refmon:1;
#else
                    unsigned char pll_refmon:1, pll_ld:1, pll_status:1, pll_int:1, irq:1, reserved:3;
#endif
                }Bit;
                unsigned char raw;
            }Register_02;
        };
    };
} adc5000_cpld_Register_Map;

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

