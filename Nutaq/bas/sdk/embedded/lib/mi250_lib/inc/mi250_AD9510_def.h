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
 * @file    mi250_AD9510_drv.h
 * @brief   MI250 AD9510 driver
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
 #ifndef _AD9510_DEF_H_
#define _AD9510_DEF_H_


#define     MI250_AD9510_REGISTER_00      0x00
#define     MI250_AD9510_REGISTER_04      0x04
#define     MI250_AD9510_REGISTER_05      0x05
#define     MI250_AD9510_REGISTER_06      0x06
#define     MI250_AD9510_REGISTER_07      0x07
#define     MI250_AD9510_REGISTER_08      0x08
#define     MI250_AD9510_REGISTER_09      0x09
#define     MI250_AD9510_REGISTER_0A      0x0A
#define     MI250_AD9510_REGISTER_0B      0x0B
#define     MI250_AD9510_REGISTER_0C      0x0C
#define     MI250_AD9510_REGISTER_0D      0x0D  
#define     MI250_AD9510_REGISTER_34      0x34
#define     MI250_AD9510_REGISTER_35      0x35
#define     MI250_AD9510_REGISTER_36      0x36
#define     MI250_AD9510_REGISTER_38      0x38
#define     MI250_AD9510_REGISTER_39      0x39
#define     MI250_AD9510_REGISTER_3A      0x3A
#define     MI250_AD9510_REGISTER_3C      0x3C
#define     MI250_AD9510_REGISTER_3D      0x3D
#define     MI250_AD9510_REGISTER_3E      0x3E
#define     MI250_AD9510_REGISTER_3F      0x3F
#define     MI250_AD9510_REGISTER_40      0x40
#define     MI250_AD9510_REGISTER_41      0x41
#define     MI250_AD9510_REGISTER_42      0x42
#define     MI250_AD9510_REGISTER_43      0x43
#define     MI250_AD9510_REGISTER_45      0x45
#define     MI250_AD9510_REGISTER_48      0x48
#define     MI250_AD9510_REGISTER_49      0x49
#define     MI250_AD9510_REGISTER_4A      0x4A
#define     MI250_AD9510_REGISTER_4B      0x4B
#define     MI250_AD9510_REGISTER_4C      0x4C
#define     MI250_AD9510_REGISTER_4D      0x4D
#define     MI250_AD9510_REGISTER_4E      0x4E
#define     MI250_AD9510_REGISTER_4F      0x4F
#define     MI250_AD9510_REGISTER_50      0x50
#define     MI250_AD9510_REGISTER_51      0x51
#define     MI250_AD9510_REGISTER_52      0x52
#define     MI250_AD9510_REGISTER_53      0x53
#define     MI250_AD9510_REGISTER_54      0x54
#define     MI250_AD9510_REGISTER_55      0x55
#define     MI250_AD9510_REGISTER_56      0x56
#define     MI250_AD9510_REGISTER_57      0x57
#define     MI250_AD9510_REGISTER_58      0x58
#define     MI250_AD9510_REGISTER_5A      0x5A

#define     MI250_AD9510_NB_REGISTER      44

typedef struct AD9510_Register_Map
{
    union
    {
        char RegisterArray[MI250_AD9510_NB_REGISTER];
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
                    unsigned char	B_Counter_LSB:8;
#else
					unsigned char	B_Counter_LSB:8;
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
                char Byte;
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
					unsigned char	R_Divider_LSB:8;
#else
					unsigned char	R_Divider_LSB:8;
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
					unsigned char	Unused:7,
									Bypass:1;
#else
					unsigned char	Bypass:1,
									Unused:7;
#endif
                }Bit;
                char Byte;
            }Register_38;

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
            }Register_39;

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
            }Register_3A;

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
            }Register_3C;

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
            }Register_42;

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
            }Register_43;

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
            }Register_48;

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
            }Register_49;

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
					unsigned char	Low_Cycles:4,
									High_Cycles:4;
#else
					unsigned char	High_Cycles:4,
									Low_Cycles:4;
#endif
                }Bit;
                char Byte;
            }Register_54;

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
            }Register_55;

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
            }Register_56;

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
            }Register_57;

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
} MI250_AD9510_Register_Map_t;

#endif
