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
 * @file      DAC5682_def.h
 * 
 * @brief     DAC5682 defines
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

#ifndef _DAC5682_DEF_H_
#define _DAC5682_DEF_H_

#define     DAC5682_REGISTER_STATUS0     0
#define     DAC5682_REGISTER_CONFIG1     1
#define     DAC5682_REGISTER_CONFIG2     2
#define     DAC5682_REGISTER_CONFIG3     3
#define     DAC5682_REGISTER_STATUS4     4
#define     DAC5682_REGISTER_CONFIG5     5
#define     DAC5682_REGISTER_CONFIG6     6
#define     DAC5682_REGISTER_CONFIG7     7
#define     DAC5682_REGISTER_CONFIG8     8
#define     DAC5682_REGISTER_CONFIG9     9
#define     DAC5682_REGISTER_CONFIG10    10
#define     DAC5682_REGISTER_CONFIG11    11
#define     DAC5682_REGISTER_CONFIG12    12
#define     DAC5682_REGISTER_CONFIG13    13
#define     DAC5682_REGISTER_CONFIG14    14
#define     DAC5682_REGISTER_CONFIG15    15

#define     DAC5682_NB_REGISTER          16

typedef struct DAC5682_Register_Map
{
    union
    {
        char RegisterArray[DAC5682_NB_REGISTER];
        struct
        {
            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	PLL_Lock:1,
									DLL_Lock:1,
									Unused:1,
									Device_ID:3,
									version:2;
#else
					unsigned char	version:2,
									Device_ID:3,
									Unused:1,
									DLL_Lock:1,
									PLL_Lock:1;
#endif
                }Bit;
                char Byte;
            }Status0;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	DAC_Delay:2,
									Unused:1,
									FIR_enable:1,
									SLFTST_enable:1,
									FIFO_Offset:3;
#else
					unsigned char	FIFO_Offset:3,
									SLFTST_enable:1,
									FIR_enable:1,
									Unused:1,
									DAC_Delay:2;
#endif
                }Bit;
                char Byte;
            }Config1;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Two_Complement:1,
									Dual_DAC:1,
									FIR2x4x:1,
									Unused:1,
									CMIX1_Mode:2,
									CMIX0_Mode:2;
#else
					unsigned char	CMIX0_Mode:2,
									CMIX1_Mode:2,
									Unused:1,
									FIR2x4x:1,
									Dual_DAC:1,
									Two_Complement:1;
#endif
                }Bit;
                char Byte;
            }Config2;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	DAC_Offset_Enable:1,
									SLFTST_Error_Mask:1,
									FIFO_Error_Mask:1,
									Pattern_Error_Mask:1,
									SwapAB_out:1,
									B_Equals_A:1,
									SW_Sync:1,
									SW_Sync_Select:1;
#else
					unsigned char	SW_Sync_Select:1,
									SW_Sync:1,
									B_Equals_A:1,
									SwapAB_out:1,
									Pattern_Error_Mask:1,
									FIFO_Error_Mask:1,
									SLFTST_Error_Mask:1,
									DAC_Offset_Enable:1;
#endif
                }Bit;
                char Byte;
            }Config3;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Unused:1,
									SLFTST_Error:1,
									FIFO_Error:1,
									Pattern_Error:1,
									Unused2:4;
#else
					unsigned char	Unused2:4,
									Pattern_Error:1,
									FIFO_Error:1,
									SLFTST_Error:1,
									Unused:1;
#endif
                }Bit;
                char Byte;
            }Status4;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	SIF4:1,
									Reverse_Bus:1,
									ClkDiv_Sync_Disable:1,
									FIFO_Sync_Disable:1,
									Reserved:1,
									DLL_Bypass:1,
									PLL_Bypass:1,
									Reserved2:1;
#else
					unsigned char	Reserved2:1,
									PLL_Bypass:1,
									DLL_Bypass:1,
									Reserved:1,
									FIFO_Sync_Disable:1,
									ClkDiv_Sync_Disable:1,
									Reverse_Bus:1,
									SIF4:1;
#endif
                }Bit;
                char Byte;
            }Config5;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Hold_Sync_Disable:1,
									Unused:1,
									Sleep_B:1,
									Sleep_A:1,
									BiasLPF_A:1,
									BiasLPF_B:1,
									PLL_Sleep:1,
									DLL_Sleep:1;
#else
					unsigned char	DLL_Sleep:1,
									PLL_Sleep:1,
									BiasLPF_B:1,
									BiasLPF_A:1,
									Sleep_A:1,
									Sleep_B:1,
									Unused:1,
									Hold_Sync_Disable:1;
#endif
                }Bit;
                char Byte;
            }Config6;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	DACA_Gain:4,
									DACB_Gain:4;
#else
					unsigned char	DACB_Gain:4,
									DACA_Gain:4;
#endif
                }Bit;
                char Byte;
            }Config7;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:5,
									DLL_Restart:1,
									Reserved2:2;
#else
					unsigned char	Reserved2:2,
									DLL_Restart:1,
									Reserved:5;
#endif
                }Bit;
                char Byte;
            }Config8;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	PLL_m:5,
									PLL_n:3;
#else
					unsigned char	PLL_n:3,
									PLL_m:5;
#endif
                }Bit;
                char Byte;
            }Config9;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	DLL_Delay:4,
									DLL_InvClk:1,
									DLL_Ifixed:3;
#else
					unsigned char	DLL_Ifixed:3,
									DLL_InvClk:1,
									DLL_Delay:4;
#endif
                }Bit;
                char Byte;
            }Config10;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	PLL_LPF_Reset:1,
									VCO_Div2:1,
									PLL_Gain:2,
									PLL_Range:4;
#else
					unsigned char	PLL_Range:4,
									PLL_Gain:2,
									VCO_Div2:1,
									PLL_LPF_Reset:1;
#endif
                }Bit;
                char Byte;
            }Config11;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:2,
									Offset_Sync:1,
									OffsetA_MSB:5;
#else
					unsigned char	OffsetA_MSB:5,
									Offset_Sync:1,
									Reserved:2;
#endif
                }Bit;
                char Byte;
            }Config12;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	OffsetA_LSB:8;
#else
					unsigned char	OffsetA_LSB:8;
#endif
                }Bit;
                char Byte;
            }Config13;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	SDO_Function_Sel:3,
									OffsetB_MSB:5;
#else
					unsigned char	OffsetB_MSB:5,
									SDO_Function_Sel:3;
#endif
                }Bit;
                char Byte;
            }Config14;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	OffsetB_LSB:8;
#else
					unsigned char	OffsetB_LSB:8;
#endif
                }Bit;
                char Byte;
            }Config15;
        };
    };
} DAC5682_Register_Map_t;


#endif
