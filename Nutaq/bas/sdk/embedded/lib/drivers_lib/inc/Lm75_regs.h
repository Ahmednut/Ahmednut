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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       Lm75_regs.h
 * @brief      Temperature monitoring lm75 internal registers definitions
 *
 * $Date: 2015/10/05 15:51:27 $
 * $Revision: 1.1 $
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

#ifndef LM75_REGS_H__
#define LM75_REGS_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// internal device registers definitions
typedef struct Lm75_StructRegisterMap
{
    union
    {
        unsigned short m_ausRegisters[0x04];
        struct 
        {
            union Lm75_UnionRegister00
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned short m_usTempMsb:8,m_usTempLsb:1,m_usReserved:7;
#else
                    unsigned short m_usReserved:7,m_usTempLsb:1,m_usTempMsb:8;
#endif                
                }stBit;
                unsigned short m_usRaw;
            }stRegister00;

            union Lm75_UnionRegister01
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned short usReserved:8,usReserved2:3,usFaultQueue:2,usOsPolarity:1,usCompInt:1,usShutdown:1;
#else
                    unsigned short usShutdown:1,usCompInt:1,usOsPolarity:1,usFaultQueue:2,usReserved2:3,usReserved:8;
#endif                
                }stBit;
                unsigned short m_usRaw;
            }stRegister01;


            union Lm75_UnionRegister02
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned short usThystMsb:8,usThystLsb:1,usReserved:7;
#else
                    unsigned short usReserved:7,usThystLsb:1,usThystMsb:8;
#endif                
                }stBit;
                unsigned short m_usRaw;
            }stRegister02;


            union Lm75_UnionRegister03
            {
                struct
                {
#ifdef _BIG_ENDIAN 
                    unsigned short usTosMsb:8,usTosLsb:1,usReserved:7;
#else
                    unsigned short usReserved:7,usTosLsb:1,usTosMsb:8;
#endif                
                }stBit;
                unsigned short m_usRaw;
            }stRegister03;
        };
    };
} Lm75_stRegisterMap;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif

