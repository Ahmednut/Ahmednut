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
 * @file    radio420_cdce62005_defines.h
 * @brief   Ccdce62005 PLL definitions
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
#ifndef __CDCE62005_DEFINES__
#define __CDCE62005_DEFINES__

#define CDCE62005_PFD_MAX ((unsigned)40e6)
#define CDCE62005_PFD_MIN ((unsigned)40e3)

#define CDCE62005_SYNTH_FREQ_MAX ((unsigned)1178e6)
#define CDCE62005_SYNTH_FREQ_MIN ((unsigned)450e6)

#define CDCE62005_VCO_FREQ_MAX ((unsigned)2356e6)
#define CDCE62005_VCO_FREQ_MIN ((unsigned)1750e6)

#define CDCE62005_VCO_MAX_OUTPUT_DIVIDER 80

typedef enum E_pll_lock_mode {
    PLL_LOCK_DIGITAL,
    PLL_LOCK_ANALOG,
} pll_lock_mode;

typedef enum E_pll_lock_status {
    CDCE62005_PLL_UNLOCKED,
    CDCE62005_PLL_LOCKED,
} pll_lock_status;

typedef struct S_cdce62005_param {
    unsigned out_freq[5];
    unsigned pfd_freq;
    unsigned pfd_freq_max;
    unsigned pfd_freq_min;
    unsigned vco_freq;
    unsigned ref_freq;
    unsigned synth_freq;
    unsigned short feedback_divider;
    unsigned short input_divider;
    unsigned char out_divider[5];
    unsigned char prescale_divider;
    unsigned char secondary_clock;
    unsigned char vco_bypass_all;
    unsigned char vco_bypass[5];
} cdce62005_param;

#endif

