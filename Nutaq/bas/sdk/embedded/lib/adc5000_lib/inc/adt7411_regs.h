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
 * @file      adt7411_regs.h
 * 
 * @brief     adt7411 registers
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

#ifndef __ADT7411_REGS__
#define __ADT7411_REGS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

typedef struct {
    union
    {
        char RegisterArray[0x80];
        struct 
        {
            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char AIN4Limits:1,
                                  AIN3Limits:1,
                                  AIN2Limits:1,
                                  extTemprFault:1,
                                  extTemprLow:1,
                                  extTemprHigh:1, 
                                  internTemprLow:1,
                                  internTemprHigh:1;
#else 
                    unsigned char internTemprHigh:1,
                                  internTemprLow:1,
                                  extTemprHigh:1,
                                  extTemprLow:1,
                                  extTemprFault:1,
                                  AIN2Limits:1, 
                                  AIN3Limits:1,
                                  AIN4Limits:1;
#endif
                }Bit;
                char raw;
            }Register_00;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:3,
                                  vddLimits:1,
                                  AIN8Limits:1,
                                  AIN7Limits:1,
                                  AIN6Limits:1,
                                  AIN5Limits:1;
#else 
                    unsigned char AIN5Limits:1,
                                  AIN6Limits:1,
                                  AIN7Limits:1,
                                  AIN8Limits:1,
                                  vddLimits:1,
                                  reserved:3;
#endif
                }Bit;
                char  raw;
            }Register_01;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:4,
                                  lsbVdd:2,
                                  lsbIntTempr:2;
#else 
                    unsigned char lsbIntTempr:2,
                                  lsbVdd:2,
                                  reserved:4;
#endif
                }Bit;
                char  raw;
            }Register_03;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char lsbAIN4:2,
                                  lsbAIN3:2,
                                  lsbAIN2:2,
                                  lsbExtTempr:2;
#else 
                    unsigned char lsbExtTempr:2,
                                  lsbAIN2:2,
                                  lsbAIN3:2,
                                  lsbAIN4:2;
#endif
                }Bit;
                char  raw;
            }Register_04;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char lsbAIN8:2,
                                  lsbAIN7:2,
                                  lsbAIN6:2,
                                  lsbAIN5:2;
#else 
                    unsigned char lsbAIN5:2,
                                  lsbAIN6:2,
                                  lsbAIN7:2,
                                  lsbAIN8:2;
#endif
                }Bit;
                char  raw;
            }Register_05;

            union
            {
                struct
                {
                    unsigned char msbVdd:8;
                }Bit;
                char raw;
            }Register_06;

            union
            {
                struct
                {
                    unsigned char msbIntTempr:8;
                }Bit;
                char raw;
            }Register_07;

            union
            {
                struct
                {
                    unsigned char msbExtTempr:8;
                }Bit;
                char raw;
            }Register_08;

            union
            {
                struct
                {
                    unsigned char msbAIN2:8;
                }Bit;
                char raw;
            }Register_09;

            union
            {
                struct
                {
                    unsigned char msbAIN3:8;
                }Bit;
                char raw;
            }Register_0A;

            union
            {
                struct
                {
                    unsigned char msbAIN4:8;
                }Bit;
                char raw;
            }Register_0B;

            union
            {
                struct
                {
                    unsigned char msbAIN5:8;
                }Bit;
                char raw;
            }Register_0C;

            union
            {
                struct
                {
                    unsigned char msbAIN6:8;
                }Bit;
                char raw;
            }Register_0D;

            union
            {
                struct
                {
                    unsigned char msbAIN7:8;
                }Bit;
                char raw;
            }Register_0E;

            union
            {
                struct
                {
                    unsigned char msbAIN8:8;
                }Bit;
                char raw;
            }Register_0F;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char powerDown:1,
                                  interruptPolarity:1,
                                  enableInterrupt:1,
                                  reserved:1,  // Must write 0 to this bit
                                  reserved2:1, // Must write 1 to this bit
                                  selectAIN1andAIN2OrExtTempr:2,
                                  startMonitoring:1;
#else 
                    unsigned char startMonitoring:1,
                                  selectAIN1andAIN2OrExtTempr:2,
                                  reserved2:1, // Must write 1 to this bit
                                  reserved:1,  // Must write 0 to this bit
                                  enableInterrupt:1, 
                                  interruptPolarity:1,
                                  powerDown:1;
#endif
                }Bit;
                char raw;
            }Register_18;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char softReset:1,
                                  enableSMBusTimeout:1,
                                  enableAveraging:1,
                                  selectRoudRobSingleChnl:1,
                                  selectSingleChannel:4; 
#else 
                    unsigned char selectSingleChannel:4,
                                  selectRoudRobSingleChnl:1,
                                  enableAveraging:1,
                                  enableSMBusTimeout:1,
                                  softReset:1; 
#endif
                }Bit;
                char raw;
            }Register_19;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:3,  // Must write 0 to those bits.
                                  selectADCRef:1,
                                  reserved2:1, // Must write 1 to this bit.
                                  reserved3:2, // Must write 0 to those bits.
                                  selectADCSpeed:1; 
#else 
                    unsigned char selectADCSpeed:1, 
                                  reserved3:2,  // Must write 0 to those bits.
                                  reserved2:1, // Must write 1 to this bit.
                                  selectADCRef:1, 
                                  reserved:3;   // Must write 0 to those bits.
#endif
                }Bit;
                char raw;
            }Register_1A;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 

                    unsigned char enableAIN4Interrupt:1,
                                  enableAIN3Interrupt:1,
                                  enableAIN2Interrupt:1,
                                  enableExtTemprFaultInterrupt:1,
                                  enableExtTemprLowInterrupt:1, 
                                  enableExtTemprHighInterrupt:1, 
                                  enableIntTemprLowInterrupt:1, 
                                  enableIntTemprHighInterrupt:1; 
#else 
                    unsigned char enableIntTemprHighInterrupt:1,
                                  enableIntTemprLowInterrupt:1,
                                  enableExtTemprHighInterrupt:1,
                                  enableExtTemprLowInterrupt:1,
                                  enableExtTemprFaultInterrupt:1, 
                                  enableAIN2Interrupt:1, 
                                  enableAIN3Interrupt:1, 
                                  enableAIN4Interrupt:1; 
#endif
                }Bit;
                char raw;
            }Register_1D;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:3, // Must write 0 to those bits.
                                  enableVddInterrupt:1,
                                  enableAIN8Interrupt:1,
                                  enableAIN7Interrupt:1,
                                  enableAIN6Interrupt:1,
                                  enableAIN5Interrupt:1;
#else 
                    unsigned char enableAIN5Interrupt:1, 
                                  enableAIN6Interrupt:1,
                                  enableAIN7Interrupt:1,
                                  enableAIN8Interrupt:1,
                                  enableVddInterrupt:1,
                                  reserved:3; // Must write 0 to those bits.
#endif
                }Bit;
                char raw;
            }Register_1E;

            union
            {
                struct
                {
                    unsigned char intTemprOffset:8;
                }Bit;
                char raw;
            }Register_1F;

            union
            {
                struct
                {
                    unsigned char extTemprOffset:8;
                }Bit;
                char raw;
            }Register_20;

            union
            {
                struct
                {
                    unsigned char vddHighLimit:8;
                }Bit;
                char raw;
            }Register_23;

            union
            {
                struct
                {
                    unsigned char vddLowLimit:8;
                }Bit;
                char raw;
            }Register_24;

            union
            {
                struct
                {
                    unsigned char intTemprHighLimit:8;
                }Bit;
                char raw;
            }Register_25;

            union
            {
                struct
                {
                    unsigned char intTemprLowLimit:8;
                }Bit;
                char raw;
            }Register_26;

            union
            {
                struct
                {
                    unsigned char extTemprHighLimit:8;
                }Bit;
                char raw;
            }Register_27;

            union
            {
                struct
                {
                    unsigned char extTemprLowLimit:8;
                }Bit;
                char raw;
            }Register_28;

            union
            {
                struct
                {
                    unsigned char AIN2HighLimit:8;
                }Bit;
                char raw;
            }Register_2B;

            union
            {
                struct
                {
                    unsigned char AIN2LowLimit:8;
                }Bit;
                char raw;
            }Register_2C;

            union
            {
                struct
                {
                    unsigned char AIN3HighLimit:8;
                }Bit;
                char raw;
            }Register_2D;

            union
            {
                struct
                {
                    unsigned char AIN3LowLimit:8;
                }Bit;
                char raw;
            }Register_2E;

            union
            {
                struct
                {
                    unsigned char AIN4HighLimit:8;
                }Bit;
                char raw;
            }Register_2F;

            union
            {
                struct
                {
                    unsigned char AIN4LowLimit:8;
                }Bit;
                char raw;
            }Register_30;

            union
            {
                struct
                {
                    unsigned char AIN5HighLimit:8;
                }Bit;
                char raw;
            }Register_31;

            union
            {
                struct
                {
                    unsigned char AIN5LowLimit:8;
                }Bit;
                char raw;
            }Register_32;

            union
            {
                struct
                {
                    unsigned char AIN6HighLimit:8;
                }Bit;
                char raw;
            }Register_33;

            union
            {
                struct
                {
                    unsigned char AIN6LowLimit:8;
                }Bit;
                char raw;
            }Register_34;

            union
            {
                struct
                {
                    unsigned char AIN7HighLimit:8;
                }Bit;
                char raw;
            }Register_35;

            union
            {
                struct
                {
                    unsigned char AIN7LowLimit:8;
                }Bit;
                char raw;
            }Register_36;

            union
            {
                struct
                {
                    unsigned char AIN8HighLimit:8;
                }Bit;
                char raw;
            }Register_37;

            union
            {
                struct
                {
                    unsigned char AIN8LowLimit:8;
                }Bit;
                char raw;
            }Register_38;

            union
            {
                struct
                {
                    unsigned char deviceId:8;
                }Bit;
                char raw;
            }Register_4D;

            union
            {
                struct
                {
                    unsigned char manufacturerId:8;
                }Bit;
                char raw;
            }Register_4E;

            union
            {
                struct
                {
                    unsigned char siliconRevision:8;
                }Bit;
                char raw;
            }Register_4F;

            union
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned char reserved:7,
                                  SPILockStatus:1;
#else 
                    unsigned char SPILockStatus:1,
                                  reserved:7;
#endif
                }Bit;
                char raw;
            }Register_7F;
        };
    };
} ADT7411_Register_Map_t;

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

