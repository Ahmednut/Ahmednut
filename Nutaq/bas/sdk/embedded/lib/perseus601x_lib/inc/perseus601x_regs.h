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
 * @file       perseus601x_regs.h
 * @brief      Perseus601X definitions.
 *
 * $Date: 2015/06/04 19:55:39 $
 * $Revision: 1.3 $
 *
 * @ingroup PERSEUS601X
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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

#ifndef PERSEUS601X_REGS_H__
#define PERSEUS601X_REGS_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *
 * Definition of binary state
 *
 ****************************************************************************/


typedef struct S_PERSEUS601X_INFO
{
    union
        {
            struct
            {
#ifdef _BIG_ENDIAN 
                unsigned core_id:16,
                         core_version:16;
#else
                unsigned core_version:16,
                         core_id:16;
#endif                         
            };
            int Register;
        };
} PERSEUS601X_INFO;

typedef struct S_PERSEUS601X_MODULE_CTRL_STATUS
{
    union
        {
            struct
            {
#ifdef _BIG_ENDIAN 
                unsigned Unused:19,
                         uart_mode:2,
                         pcie_msi:1,
                         pcie_clock_lock:1,
                         pcie_present:1,
                         ddr3_init_done:1,
                         Unused2:4,
                         FMCT:1,
                         FMC:1,
                         Daughter:1;
#else
                unsigned Daughter:1,
                         FMC:1,
                         FMCT:1,
                         Unused2:4,
                         ddr3_init_done:1,
                         pcie_present:1,
                         pcie_clock_lock:1,
                         pcie_msi:1,
                         uart_mode:2,
                         Unused:19;
#endif                         
            };
            int Register;
        };
} PERSEUS601X_MODULE_CTRL_STATUS;

typedef struct S_PERSEUS601X_CLK_CTRL
{
    union
    {
        struct
        {
#ifdef _BIG_ENDIAN 
            unsigned Unused:18,
                     CTRL_100_MHZ_OUT_EN:1,
                     CTRL_FCLKA_HIGHZ:1,
                     CTRL_AMC_TCLKA_2_FMC_CLK2_EN:1,
                     CTRL_AMC_TCLKC_2_FMC_CLK3_EN:1,
                     CTRL_FMC_CLK0_2_AMC_TCLKB_EN:1,
                     CTRL_FMC_CLK1_2_AMC_TCLKD_EN:1,
                     CTRL_TCLKA_RX_DIS:1,
                     CTRL_TCLKA_TX_EN:1,
                     CTRL_TCLKB_RX_DIS:1,
                     CTRL_TCLKB_TX_EN:1,
                     CTRL_TCLKC_RX_DIS:1,
                     CTRL_TCLKC_TX_EN:1,
                     CTRL_TCLKD_RX_DIS:1,
                     CTRL_TCLKD_TX_EN:1;
#else
            unsigned CTRL_TCLKD_TX_EN:1,
                     CTRL_TCLKD_RX_DIS:1,
                     CTRL_TCLKC_TX_EN:1,
                     CTRL_TCLKC_RX_DIS:1,
                     CTRL_TCLKB_TX_EN:1,
                     CTRL_TCLKB_RX_DIS:1,
                     CTRL_TCLKA_TX_EN:1,
                     CTRL_TCLKA_RX_DIS:1,
                     CTRL_FMC_CLK1_2_AMC_TCLKD_EN:1,
                     CTRL_FMC_CLK0_2_AMC_TCLKB_EN:1,
                     CTRL_AMC_TCLKC_2_FMC_CLK3_EN:1,
                     CTRL_AMC_TCLKA_2_FMC_CLK2_EN:1,
                     CTRL_FCLKA_HIGHZ:1,
                     CTRL_100_MHZ_OUT_EN:1,
                     Unused:18;
#endif
        };
        int Register;
    };
} PERSEUS601X_CLK_CTRL;

typedef struct S_PERSEUS601X_VADJ_FPGA_CTRL
{
    union
    {
        struct
        {
#ifdef _BIG_ENDIAN 
            unsigned Unused:28,
                     PROG_N:1,
                     VADJ_EN:1,
                     VADJ_SEL:2;
#else
            unsigned VADJ_SEL:2,
                     VADJ_EN:1,
                     PROG_N:1,
                     Unused:28;
#endif
        };
        int Register;
    };
} PERSEUS601X_VADJ_FPGA_CTRL;

typedef struct S_PERSEUS601X_LED_CTRL
{
    union
    {
        struct
        {
#ifdef _BIG_ENDIAN 
            unsigned Unused:15,
                     CTRL_LED_BUF_OD:1,
                     CTRL_LED7_GRN:1,
                     CTRL_LED6_GRN:1,
                     CTRL_LED5_GRN:1,
                     CTRL_LED4_GRN:1,
                     CTRL_LED3_GRN:1,
                     CTRL_LED2_GRN:1,
                     CTRL_LED1_GRN:1,
                     CTRL_LED0_GRN:1,
                     CTRL_LED7_RED:1,
                     CTRL_LED6_RED:1,
                     CTRL_LED5_RED:1,
                     CTRL_LED4_RED:1,
                     CTRL_LED3_RED:1,
                     CTRL_LED2_RED:1,
                     CTRL_LED1_RED:1,
                     CTRL_LED0_RED:1;
#else
            unsigned CTRL_LED0_RED:1,
                     CTRL_LED1_RED:1,
                     CTRL_LED2_RED:1,
                     CTRL_LED3_RED:1,
                     CTRL_LED4_RED:1,
                     CTRL_LED5_RED:1,
                     CTRL_LED6_RED:1,
                     CTRL_LED7_RED:1,
                     CTRL_LED0_GRN:1,
                     CTRL_LED1_GRN:1,
                     CTRL_LED2_GRN:1,
                     CTRL_LED3_GRN:1,
                     CTRL_LED4_GRN:1,
                     CTRL_LED5_GRN:1,
                     CTRL_LED6_GRN:1,
                     CTRL_LED7_GRN:1,
                     CTRL_LED_BUF_OD:1,
                     Unused:15;
#endif
        };
        int Register;
    };
} PERSEUS601X_LED_CTRL;

typedef struct sPerseus601X
{
    struct S_PERSEUS601X_INFO                  INFO;
    struct S_PERSEUS601X_MODULE_CTRL_STATUS    MODULE_CTRL_STATUS;
    struct S_PERSEUS601X_CLK_CTRL              CLK_CTRL;
    struct S_PERSEUS601X_VADJ_FPGA_CTRL        VADJ_FPGA_CTRL;
    struct S_PERSEUS601X_LED_CTRL              LED_CTRL;
} sPerseus601X_t;

#ifdef __cplusplus
} 
#endif  // extern "C"                        

#endif
