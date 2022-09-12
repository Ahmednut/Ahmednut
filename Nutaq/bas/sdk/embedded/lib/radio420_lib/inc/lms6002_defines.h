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
 * @file    lms6002.h 
 * @brief   Calculate valid configuration and configure the LMS6002
 *          integrated RF front-end
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
#ifndef __LMS6002_DEFINES__
#define __LMS6002_DEFINES__

#define LMS6002_PLL_OFF_DIV1        0x0
#define LMS6002_PLL_OFF_DIV2        0x1
#define LMS6002_PLL_OFF_DIV3        0x2
#define LMS6002_PLL_OFF_DIV4        0x3
#define LMS6002_PLL_OFF_FREQSEL     0x5
#define LMS6002_PLL_OFF_VCOCAP      0x9
#define LMS6002_PLL_OFF_VTUNE       0xA
#define LMS6002_PLL_OFF_COMPARATOR  0xB

typedef struct S_lms6002_pll_param {
    unsigned out_freq;
    unsigned ref_freq;
    unsigned n_divider; //9 bit integer + 23 bit fractional integer
} lms6002_pll_param;

typedef struct S_lms6002_pll_core {
    unsigned char base_addr;
} lms6002_pll_core;

typedef struct S_lms6002_core {
    spi_core_t * spi;
    lms6002_pll_core tx_core;
    lms6002_pll_core rx_core;
    unsigned char saved_gain;
    unsigned char saved_pa;
} lms6002_core;

typedef struct S_lms6002_lo_leakage_calib {
    unsigned char i_adjust;
    unsigned char q_adjust;
} lms6002_lo_leakage_calib;

typedef struct S_lms6002_ssb_calib {
    unsigned phase_offset;
    unsigned gain_offset;
    unsigned backoff_offset;
} lms6002_ssb_calib;

typedef struct S_lms6002_rx_dc_calib {
    unsigned char i_comp;
    unsigned char q_comp;
} lms6002_rx_dc_calib;

typedef enum E_LMS6002_PLL {
    LMS6002_PLL_TX,
    LMS6002_PLL_RX,
} LMS6002_PLL;

typedef enum E_LMS6002_DIR {
    LMS6002_TX = 1,
    LMS6002_RX = 2,
    LMS6002_TXRX=3,
} LMS6002_DIR;

typedef enum E_LMS6002_BAND {
    LMS6002_LOWBAND,
    LMS6002_HIGHBAND,
} LMS6002_BAND;

typedef enum E_LMS6002_STATE {
    LMS6002_DISABLE = 0,
    LMS6002_ENABLE  = 1,
} LMS6002_STATE;

typedef enum E_LMS6002_LNA_GAIN {
    LMS6002_LNA_MAXGAIN = 0x3,
    LMS6002_LNA_MIDGAIN = 0x2,
    LMS6002_LNA_BYPASS  = 0x1,
} LMS6002_LNA_GAIN;

typedef enum E_LMS6002_VGA1_GAIN {
    LMS_VGA1_GAIN30DB = 120,
    LMS_VGA1_GAIN19DB = 102,
    LMS_VGA1_GAIN5DB  =   2,
} LMS6002_VGA1_GAIN;

typedef enum E_LMS6002_LPF_BANDWIDTH {
    LMS6002_LPF_14MHZ,
    LMS6002_LPF_10MHZ,
    LMS6002_LPF_7MHZ,
    LMS6002_LPF_6MHZ,
    LMS6002_LPF_5MHZ,
    LMS6002_LPF_4DOT375MHZ,
    LMS6002_LPF_3DOT5MHZ,
    LMS6002_LPF_3MHZ,
    LMS6002_LPF_2DOT75MHZ,
    LMS6002_LPF_2DOT5MHZ,
    LMS6002_LPF_1DOT92MHZ,
    LMS6002_LPF_1DOT5MHZ,
    LMS6002_LPF_1DOT375MHZ,
    LMS6002_LPF_1DOT25MHZ,
    LMS6002_LPF_0DOT875MHZ,
    LMS6002_LPF_0DOT75MHZ,
    LMS6002_LPF_BYPASS,
} LMS6002_LPF_BANDWIDTH;

typedef enum E_LMS6002_LOOPBACK_MODE {
    LMS6002_LOOPBACK_DISABLED,
    LMS6002_LOOPBACK_LNA1,
    LMS6002_LOOPBACK_LNA2,
    LMS6002_LOOPBACK_LNA3,
} LMS6002_LOOPBACK_MODE;

typedef enum E_LMS6002_CHANNEL {
    LMS6002_CHANNEL_I,
    LMS6002_CHANNEL_Q,
} LMS6002_CHANNEL;

typedef enum E_LMS6002_MIX_RXFE {
    LMS6002_PADS_CONNECTED      = 0,
    LMS6002_LNA_INTERNAL_OUTPUT = 1,
} LMS6002_MIX_RXFE;

#endif

