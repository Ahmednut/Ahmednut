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
 * @file       aurora_regs.h
 * @brief      Aurora module related internal registers definitions
 *
 * $Date: 2016/04/22 20:47:41 $
 * $Revision: 1.4 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup AURORA
  *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef __AURORA_REGS__
#define __AURORA_REGS__
#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

/** @name Aurora register map
 * 
 *  Hold Aurora internal device registers definitions.
 */
 
#define COREID_REG_IDX 0
#define GLOBAL_REGISTERS_REG_IDX 1
#define RECEIVE_DATA_COUNT_MSW_REG_IDX 2
#define RECEIVE_DATA_COUNT_LSW_REG_IDX 3
#define TRANSMIT_DATA_COUNT_MSW_REG_IDX 4
#define TRANSMIT_DATA_COUNT_LSW_REG_IDX 5
#define RECEIVE_DATA_RATE_REG_IDX 6
#define TRANSMIT_DATA_RATE_REG_IDX 7
#define GTX_PARAMETER_REG_IDX 8
#define GTX_DFEEYEDACMON_REG_IDX 9
 
typedef struct S_aurora_IndRegister_Map
{
    union
    {
        unsigned RegisterArray[10];
        struct
        {
            union U_aurora_regs_0
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned CoreID:16, Core_Version:16;
#else
					unsigned Core_Version:16, CoreID:16;
#endif

                }Bit;
                unsigned raw;
            }CoreID;
            union U_aurora_regs_1
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned 	reserved0:6,
								Latch_Data_Count:1,       
								reserved1:1,              
								Aurora_Reset:1,           
								Aurora_Hard_Error:1,      
								Aurora_Soft_Error:1,      
								Aurora_Channel_Up:1,      
								Aurora_Lanes_Up:4,        
								TX_Fifo_Reset:1,          
								reserved2:3,              
								TX_Fifo_Full:1,           
								reserved3:3,              
								RX_Fifo_Reset:1,          
                TX_Enable:1,              
                RX_Enable:1,              
								reserved4:4,              
								RX_Fifo_Empty:1;          
#else
                    unsigned	RX_Fifo_Empty:1,
								reserved4:4,
                RX_Enable:1,
                TX_Enable:1,
								RX_Fifo_Reset:1,
								reserved3:3,
								TX_Fifo_Full:1,
								reserved2:3,
								TX_Fifo_Reset:1,
								Aurora_Lanes_Up:4,
								Aurora_Channel_Up:1,
								Aurora_Soft_Error:1,
								Aurora_Hard_Error:1,
								Aurora_Reset:1,
								reserved1:1,
								Latch_Data_Count:1,
								reserved0:6;
#endif
                }Bit;
                unsigned raw;
            }Global_Registers;

            union U_aurora_regs_2
            {
                struct
                {
                    unsigned Receive_Data_Count_MSW:32;
                }Bit;
                unsigned raw;
            }Receive_Data_Count_MSW;

            union U_aurora_regs_3
            {
                struct
                {
                    unsigned Receive_Data_Count_LSW:32;
                }Bit;
                unsigned raw;
            }Receive_Data_Count_LSW;

            union U_aurora_regs_4
            {
                struct
                {
                    unsigned Transmit_Data_Count_MSW:32;
                }Bit;
                unsigned raw;
            }Transmit_Data_Count_MSW;

            union U_aurora_regs_5
            {
                struct
                {
                    unsigned Transmit_Data_Count_LSW:32;
                }Bit;
                unsigned raw;
            }Transmit_Data_Count_LSW;

            union U_aurora_regs_6
            {
                struct
                {
                    unsigned Receive_Data_Rate:32;
                }Bit;
                unsigned raw;
            }Receive_Data_Rate;

            union U_aurora_regs_7
            {
                struct
                {
                    unsigned Transmit_Data_Rate:32;
                }Bit;
                unsigned raw;
            }Transmit_Data_Rate;

            union U_aurora_regs_8
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned 	reserved:11,
								rxeqmix:3,
								dfetap1:5,
								txdiffctrl:4,
								txpreemphasis:4,
								txpostemphasis:5;
#else
                    unsigned	txpostemphasis:5,
                                txpreemphasis:4,
                                txdiffctrl:4,
                                dfetap1:5,
                                rxeqmix:3,
								reserved:11;
#endif
                }Bit;
                unsigned raw;
            }GTX_Parameter;

            union U_aurora_regs_9
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned 	reserved:12,
								lane3:5,
								lane2:5,
								lane1:5,
								lane0:5;
#else
                    unsigned	lane0:5,
                                lane1:5,
                                lane2:5,
                                lane3:5,
								reserved:12;
#endif
                }Bit;
                unsigned raw;
            }GTX_DfeEyeDacMon;

         };
     };
} aurora_IndRegister_Map;

typedef struct S_aurora_Register_Map
{
    union
    {
        unsigned RegisterArray[2];
        struct
        {
            union U_aurora_RegCtrl
            {
                struct
                {
#ifdef _BIG_ENDIAN
                    unsigned rsvd:15, RegWrEn:1, RegIdx:16;
#else
					unsigned RegIdx:16, RegWrEn:1, rsvd:15;
#endif

                }Bit;
                unsigned raw;
            }RegCtrl;
            
            unsigned RegData;

         };
     };
} aurora_Register_Map;

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

