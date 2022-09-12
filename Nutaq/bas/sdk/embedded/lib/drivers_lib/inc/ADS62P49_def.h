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
 * @file      ADS62P49_def.h
 * 
 * @brief     ADS62P49 defines
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

#ifndef _ADS62P49_DEF_H_
#define _ADS62P49_DEF_H_

#define     ADS62P49_REGISTER_00     0
#define     ADS62P49_REGISTER_20     0x20
#define     ADS62P49_REGISTER_3F     0x3F
#define     ADS62P49_REGISTER_40     0x40
#define     ADS62P49_REGISTER_41     0x41
#define     ADS62P49_REGISTER_44     0x44
#define     ADS62P49_REGISTER_50     0x50
#define     ADS62P49_REGISTER_51     0x51
#define     ADS62P49_REGISTER_52     0x52
#define     ADS62P49_REGISTER_53     0x53
#define     ADS62P49_REGISTER_55     0x55
#define     ADS62P49_REGISTER_57     0x57
#define     ADS62P49_REGISTER_62     0x62
#define     ADS62P49_REGISTER_63     0x63
#define     ADS62P49_REGISTER_66     0x66
#define     ADS62P49_REGISTER_68     0x68
#define     ADS62P49_REGISTER_6A     0x6A
#define     ADS62P49_REGISTER_75     0x75
#define     ADS62P49_REGISTER_76     0x76

#define     ADS62P49_NB_REGISTER  19

typedef struct ADS62P49_Register_Map
{
    union
    {
        char RegisterArray[ADS62P49_NB_REGISTER];
        struct
        {
            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Reset:1,
									Reserved:6,
									Serial_ReadOut:1;
#else
					unsigned char	Serial_ReadOut:1,
									Reserved:6,
									Reset:1;
#endif

                }Bit;
                char Byte;
            }Register_00;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:5,
									Enable_LowSpeed_Mode:1,
									Reserved2:2;
#else
					unsigned char	Reserved2:2,
									Enable_LowSpeed_Mode:1,
									Reserved:5;
#endif
                }Bit;
                char Byte;
            }Register_20;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	Reserved:1,
									Ref:1,
									Reserved2:4,
									Standby:1,
									Reserved3:1;
#else
					unsigned char	Reserved3:1,
									Standby:1,
									Reserved2:4,
									Ref:1,
									Reserved:1;
#endif
                }Bit;
                char Byte;
            }Register_3F;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:4,
									PowerDown_Mode:4;
#else
					unsigned char	PowerDown_Mode:4,
									Reserved:4;
#endif
                }Bit;
                char Byte;
            }Register_40;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
					unsigned char	LVDS_CMOS:1,
									Reserved:7;
#else
					unsigned char	Reserved:7,
									LVDS_CMOS:1;
#endif
                }Bit;
                char Byte;
            }Register_41;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Clock_Edge_Control:6,
									Reserved:2;
#else
					unsigned char	Reserved:2,
									Clock_Edge_Control:6;
#endif
                }Bit;
                char Byte;
            }Register_44;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:1,
									Enable_Ind_Chan_Ctrl:1,
									Reserved1:3,
									Data_Format:2,
									Reserved2:1;
#else
					unsigned char	Reserved2:1,
									Data_Format:2,
									Reserved1:3,
									Enable_Ind_Chan_Ctrl:1,
									Reserved:1;
#endif
                }Bit;
                char Byte;
            }Register_50;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char CustomPattern_low:8;
#else
					unsigned char CustomPattern_low:8;
#endif
                }Bit;
                char Byte;
            }Register_51;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:2,
									CustomPattern_High:6;
#else
					unsigned char	Reserved:2,
									CustomPattern_High:6;
#endif
                }Bit;
                char Byte;
            }Register_52;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:1, 
									ChanA_Enable_Offset:1,
									Reserved1:6;
#else
					unsigned char	Reserved1:6,
									ChanA_Enable_Offset:1,
                  Reserved:1;
#endif
                }Bit;
                char Byte;
            }Register_53;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	ChanA_Gain:4,
									ChanA_Offset_Time:4;
#else
					unsigned char	ChanA_Offset_Time:4,
									ChanA_Gain:4;
#endif
                }Bit;
                char Byte;
            }Register_55;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:1,
									ChanA_Gain_Fine:7;
#else
					unsigned char	ChanA_Gain_Fine:7,
									Reserved:1;
#endif
                }Bit;
                char Byte;
            }Register_57;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:5,
									ChanA_Test_Pattern:3;
#else
					unsigned char	ChanA_Test_Pattern:3,
									Reserved:5;
#endif
                }Bit;
                char Byte;
            }Register_62;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:2,
									ChanA_Offset_Pedestal:6;
#else
					unsigned char	ChanA_Offset_Pedestal:6,
									Reserved:2;
#endif
                }Bit;
                char Byte;
            }Register_63;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:1,
									ChanB_Enable_Offset:1,
									Reserved2:6;
#else
					unsigned char	Reserved2:6,
									ChanB_Enable_Offset:1,
									Reserved:1;
#endif
                }Bit;
                char Byte;
            }Register_66;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	ChanB_Gain:4,
									ChanB_Offset_Time:4;
#else
					unsigned char	ChanB_Offset_Time:4,
									ChanB_Gain:4;
#endif
                }Bit;
                char Byte;
            }Register_68;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:1,
									ChanB_Gain_Fine:7;
#else
					unsigned char	ChanB_Gain_Fine:7,
									Reserved:1;
#endif
                }Bit;
                char Byte;
            }Register_6A;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:5,
									ChanB_Test_Pattern:3;
#else
					unsigned char	ChanB_Test_Pattern:3,
									Reserved:5;
#endif
                }Bit;
                char Byte;
            }Register_75;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned char	Reserved:2,
									ChanB_Offset_Pedestal:6;
#else
					unsigned char	ChanB_Offset_Pedestal:6,
									Reserved:2;
#endif
                }Bit;
                char Byte;
            }Register_76;        
        };
    };
} ADS62P49_Register_Map_t;


#endif
