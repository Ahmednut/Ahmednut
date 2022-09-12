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
 * @file      AD9511_def.h
 * 
 * @brief     AD9511 defines
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

#ifndef _AD9511_DEF_H_
#define _AD9511_DEF_H_


#define     AD9511_REGISTER_00      0x00
#define     AD9511_REGISTER_04      0x04
#define     AD9511_REGISTER_05      0x05
#define     AD9511_REGISTER_06      0x06
#define     AD9511_REGISTER_07      0x07
#define     AD9511_REGISTER_08      0x08
#define     AD9511_REGISTER_09      0x09
#define     AD9511_REGISTER_0A      0x0A
#define     AD9511_REGISTER_0B      0x0B
#define     AD9511_REGISTER_0C      0x0C
#define     AD9511_REGISTER_0D      0x0D  
#define     AD9511_REGISTER_34      0x34
#define     AD9511_REGISTER_35      0x35
#define     AD9511_REGISTER_36      0x36
#define     AD9511_REGISTER_3D      0x3D
#define     AD9511_REGISTER_3E      0x3E
#define     AD9511_REGISTER_3F      0x3F
#define     AD9511_REGISTER_40      0x40
#define     AD9511_REGISTER_41      0x41
#define     AD9511_REGISTER_45      0x45
#define     AD9511_REGISTER_4A      0x4A
#define     AD9511_REGISTER_4B      0x4B
#define     AD9511_REGISTER_4C      0x4C
#define     AD9511_REGISTER_4D      0x4D
#define     AD9511_REGISTER_4E      0x4E
#define     AD9511_REGISTER_4F      0x4F
#define     AD9511_REGISTER_50      0x50
#define     AD9511_REGISTER_51      0x51
#define     AD9511_REGISTER_52      0x52
#define     AD9511_REGISTER_53      0x53
#define     AD9511_REGISTER_58      0x58
#define     AD9511_REGISTER_5A      0x5A

#define     AD9511_NB_REGISTER      32

typedef struct AD9511_Register_Map
{
    union
    {
        char RegisterArray[AD9511_NB_REGISTER];
        struct 
        {
            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	SDO_Inactive:1,
									LSB_First:1,
									Soft_Reset:1, 
									Long_Instruction:1,
									Unused:4;
#else
					unsigned char	Unused:4,
									Long_Instruction:1,
									Soft_Reset:1, 
									LSB_First:1,
									SDO_Inactive:1;
#endif
                }Bit;
                char Byte;
            }Register_00;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Unused:2,
									A_Counter:6;
#else
					unsigned char	A_Counter:6,
									Unused:2;

#endif
				}Bit;
                char  Byte;
            }Register_04;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Unused:3,
									B_Counter_MSB:5;
#else
					unsigned char	B_Counter_MSB:5,
									Unused:3;

#endif
                }Bit;
                char  Byte;
            }Register_05;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char B_Counter_LSB:8;
#else
					unsigned char B_Counter_LSB:8;

#endif
                }Bit;
                char  Byte;
            }Register_06;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused3:1,
									LOR_Lock_Del:2,
									Unused2:2,
									LOR_Enable:1,
									Unused:2;
#else
					unsigned char	Unused:2,
									LOR_Enable:1,
									Unused2:2,
									LOR_Lock_Del:2,
									Unused3:1;

#endif
                }Bit;
                char  Byte;
            }Register_07;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:1,
									PFD_Polarity:1,
									PLL_MUX_Select:4,
									CP_Mode:2;
#else
					unsigned char	CP_Mode:2,
									PLL_MUX_Select:4,
									PFD_Polarity:1,
									Unused:1;

#endif
                }Bit;
                int Byte;
            }Register_08;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:1,
									CP_Current:3,
									Unused2:1,
									Reset_R_Counter:1,
									Reset_N_Counter:1,
									Reset_All_Counter:1;
#else
					unsigned char	Reset_All_Counter:1,
									Reset_N_Counter:1,
									Reset_R_Counter:1,
									Unused2:1,
									CP_Current:3,
									Unused:1;

#endif
                }Bit;
                char Byte;
            }Register_09;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:1,
									B_Bypass:1,
									Unused2:1,
									Prescaler_P:3,
				                    Power_Down:2;
#else
					unsigned char	Power_Down:2,
									Prescaler_P:3,
									Unused2:1,
									B_Bypass:1,
				                    Unused:1;

#endif
                }Bit;
                char Byte;
            }Register_0A;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:2,
									R_Divider_MSB:6;
#else
					unsigned char	R_Divider_MSB:6,
									Unused:2;

#endif
                }Bit;
                char Byte;
            }Register_0B;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char R_Divider_LSB:8;
#else
					unsigned char R_Divider_LSB:8;

#endif
                }Bit;
                char Byte;
            }Register_0C;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:1,
									Digital_Lock_Det_Enable:1,
									Digital_Lock_Det_Window:1,
									Unsused:3,
									Antibacklash_Pulse_Width:2;
#else
					unsigned char	Antibacklash_Pulse_Width:2,
									Unsused:3,
									Digital_Lock_Det_Window:1,
									Digital_Lock_Det_Enable:1,
									Unused:1;

#endif
                }Bit;
                char Byte;
            }Register_0D;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:7,
									ByPass:1;
#else
					unsigned char	ByPass:1,
									Unused:7;

#endif

                }Bit;
                char Byte;
            }Register_34;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:2,
									Ramp_Capacitor:3,
									Ramp_Current:3;
#else
					unsigned char	Ramp_Current:3,
									Ramp_Capacitor:3,
									Unused:2;

#endif
                }Bit;
                char Byte;
            }Register_35;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:2,
									Fine_Delay:5,
									Unused2:1;
#else
					unsigned char	Unused2:1,
									Fine_Delay:5,
									Unused:2;

#endif
                }Bit;
                char Byte;
            }Register_36;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:4,
									Output_Level:2,
									Power_Down:2;
#else
					unsigned char	Power_Down:2,
									Output_Level:2,
									Unused:4;

#endif
                }Bit;
                char Byte;
            }Register_3D;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:4,
									Output_Level:2,
									Power_Down:2;
#else
					unsigned char	Power_Down:2,
									Output_Level:2,
									Unused:4;

#endif
                }Bit;
                char Byte;
            }Register_3E;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:4,
									Output_Level:2,
									Power_Down:2;
#else
					unsigned char	Power_Down:2,
									Output_Level:2,
									Unused:4;

#endif
                }Bit;
                char Byte;
            }Register_3F;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:3,
									CMOS_Inverted_On:1,
									Logic_Select:1,
									Output_Level:2,
									Output_Power:1;
#else
					
					unsigned char	Output_Power:1,
									Output_Level:2,
									Logic_Select:1,
									CMOS_Inverted_On:1,
									Unused:3;

#endif
                }Bit;
                char Byte;
            }Register_40;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:3,
									CMOS_Inverted_On:1,
									Logic_Select:1,
									Output_Level:2,
									Output_Power:1;
#else
					unsigned char	Output_Power:1,
									Output_Level:2,
									Logic_Select:1,
									CMOS_Inverted_On:1,
									Unused:3;

#endif
                }Bit;
                char Byte;
            }Register_41;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:2,
									CLK_In_PD:1,
									REF_In_PD:1,
									CLK_To_PLL_PD:1,
									CLK2_PD:1,
									CLK1_PD:1,
									Select_CLK_In:1;
#else
					unsigned char	Select_CLK_In:1,
									CLK1_PD:1,
									CLK2_PD:1,
									CLK_To_PLL_PD:1,
									REF_In_PD:1,
									CLK_In_PD:1,
									Unused:2;

#endif
                }Bit;
                char Byte;
            }Register_45;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Low_Cycles:4,
									High_Cycles:4;
#else
					unsigned char	High_Cycles:4,
									Low_Cycles:4;

#endif
                             
                }Bit;
                char Byte;
            }Register_4A;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Bypass:1,
									No_Sync:1,
									Force:1,
									Start_HL:1,
									Phase_Offset:4;
#else
					unsigned char	Phase_Offset:4,
									Start_HL:1,
									Force:1,
									No_Sync:1,
									Bypass:1;

#endif
                }Bit;
                char Byte;
            }Register_4B;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Low_Cycles:4,
									High_Cycles:4;
#else
					unsigned char	High_Cycles:4,
									Low_Cycles:4;

#endif
                }Bit;
                char Byte;
            }Register_4C;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Bypass:1,
									No_Sync:1,
									Force:1,
									Start_HL:1,
									Phase_Offset:4;
#else
					unsigned char	Phase_Offset:4,
									Start_HL:1,
									Force:1,
									No_Sync:1,
									Bypass:1;

#endif
                }Bit;
                char Byte;
            }Register_4D;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Low_Cycles:4,
									High_Cycles:4;
#else
					unsigned char	High_Cycles:4,
									Low_Cycles:4;

#endif
                }Bit;
                char Byte;
            }Register_4E;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Bypass:1,
									No_Sync:1,
									Force:1,
									Start_HL:1,
									Phase_Offset:4;
#else
					unsigned char	Phase_Offset:4,
									Start_HL:1,
									Force:1,
									No_Sync:1,
									Bypass:1;

#endif
                }Bit;
                char Byte;
            }Register_4F;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Low_Cycles:4,
									High_Cycles:4;
#else
					unsigned char	High_Cycles:4,
									Low_Cycles:4;

#endif
                }Bit;
                char Byte;
            }Register_50;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Bypass:1,
									No_Sync:1,
									Force:1,
									Start_HL:1,
									Phase_Offset:4;
#else
					unsigned char	Phase_Offset:4,
									Start_HL:1,
									Force:1,
									No_Sync:1,
									Bypass:1;

#endif
                }Bit;
                char Byte;
            }Register_51;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Low_Cycles:4,
									High_Cycles:4;
#else
					unsigned char	High_Cycles:4,
									Low_Cycles:4;

#endif
                }Bit;
                char Byte;
            }Register_52;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Bypass:1,
									No_Sync:1,
									Force:1,
									Start_HL:1,
									Phase_Offset:4;
#else
					unsigned char	Phase_Offset:4,
									Start_HL:1,
									Force:1,
									No_Sync:1,
									Bypass:1;
#endif
                }Bit;
                char Byte;
            }Register_53;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:1,
									Function_Pin:2,
									PD_Sync:1,
									PD_All_Ref:1,
									Sync_Reg:1,
									Sync_Select:1,
									Sync_Enable:1;
#else
					unsigned char	Sync_Enable:1,
									Sync_Select:1,
									Sync_Reg:1,
									PD_All_Ref:1,
									PD_Sync:1,
									Function_Pin:2,
									Unused:1;
#endif
                }Bit;
                char Byte;
            }Register_58;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Unused:7,
									Update_Registers:1;
#else
					unsigned char	Update_Registers:1,
									Unused:7;
#endif
                }Bit;
                char Byte;
            }Register_5A;
        };
    };
} AD9511_Register_Map_t;

#endif
