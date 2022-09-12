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
 * @file       MI125_lm75_regs.h
 * @brief      Temperature monitoring lm75 internal registers definitions
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

#ifndef __MI125_LM75_REGS__
#define __MI125_LM75_REGS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// internal device registers definitions
typedef struct MI125_S_lm75_Register_Map
{
    union
    {
        unsigned short RegisterArray[0x04];
        struct 
        {
            union U_lm75_regs_00
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned short temp_msb:8,temp_lsb:1,reserved:7;
#else
                    unsigned short reserved:7,temp_lsb:1,temp_msb:8;
#endif                
                }Bit;
                unsigned short raw;
            }Register_00;

            union U_lm75_regs_01
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned short reserved:8,reserved2:3,fault_queue:2,os_polarity:1,comp_int:1,shutdown:1;
#else
                    unsigned short shutdown:1,comp_int:1,os_polarity:1,fault_queue:2,reserved2:3,reserved:8;
#endif                
                }Bit;
                unsigned short raw;
            }Register_01;


            union U_lm75_regs_02
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned short thyst_msb:8,thyst_lsb:1,reserved:7;
#else
                    unsigned short reserved:7,thyst_lsb:1,thyst_msb:8;
#endif                
                }Bit;
                unsigned short raw;
            }Register_02;


            union U_lm75_regs_03
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned short tos_msb:8,tos_lsb:1,reserved:7;
#else
                    unsigned short reserved:7,tos_lsb:1,tos_msb:8;
#endif                
                }Bit;
                unsigned short raw;
            }Register_03;
        };
    };
} MI125_lm75_Register_Map;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

