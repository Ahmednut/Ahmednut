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
 * @file      ad9517_defines.h
 * 
 * @brief     ad9517 defines
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

#ifndef __AD9517_DEFINES__
#define __AD9517_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

#include "com_wrapper_defines.h"
#include "bas_error.h"

#define AD9517_PFD_MAX_FREQ ((unsigned)100e6)
#define AD9517_PFD_MIN_FREQ ((unsigned)45e3)

#define AD9517_MAX_OUTPUT_DIVIDER 32

#define AD9517_MAXSYNTHFREQ ((unsigned)1600e6)
#define AD9517_MINSYNTHFREQ ((unsigned)1e6)

typedef struct S_ad9517_core {
    com_core * com;
} ad9517_core;

typedef enum E_ad9517_lock_mode {
    AD9517_LOCK_DIGITAL,
    AD9517_LOCK_ANALOG,
} ad9517_lock_mode;

typedef enum E_ad9517_lock_status {
    AD9517_PLL_UNLOCKED,
    AD9517_PLL_LOCKED,
} ad9517_lock_status;

typedef struct S_ad9517_param {
    unsigned out_freq[4];
    unsigned pfd_freq;
    unsigned pfd_freq_max;
    unsigned pfd_freq_min;
    unsigned vco_freq;
    unsigned vco_freq_max;
    unsigned vco_freq_min;
    unsigned ref_freq;
    unsigned Bfactor;
    unsigned Rfactor;
    unsigned Afactor;
    unsigned Pfactor;
    unsigned short out_divider[4];
    unsigned char external_clock;
    unsigned char vco_divider;
} ad9517_param;

typedef enum E_AD9517_CP_CURRENT {
    AD9517_CP_CURRENT_0DOT6MA,
    AD9517_CP_CURRENT_1DOT2MA,
    AD9517_CP_CURRENT_1DOT8MA,
    AD9517_CP_CURRENT_2DOT4MA,
    AD9517_CP_CURRENT_3DOT0MA,
    AD9517_CP_CURRENT_3DOT6MA,
    AD9517_CP_CURRENT_4DOT2MA,
    AD9517_CP_CURRENT_4DOT8MA,
} AD9517_CP_CURRENT;

enum ad9517_error {
    AD9517OK       = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_AD9517, 0),
    AD9517OVERFLOW = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 1),       /**> Overflow during calculation */
    AD9517_INPUT_DIVIDER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 2),
    AD9517FREQLOW = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 3),        /**> Requested output frequency is too low */
    AD9517FREQHIGH = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 4),       /**> Requested output frequency is too high */
    AD9517_NO_VALID_SOLUTION = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 5), /**> There is no possible configuration that will generate the requested parameters */
    AD9517_DIVIDER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 6),       /**> Requested output frequency impossible because output divider would be too large */
    AD9517_PFDFREQ_OUT_OF_RANGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 7), /**> PFP frequency is out of bound */
    AD9517_NO_VALID_R = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 8),    /**> Impossible R divider, reference frequency is too large or PFD frequency is too small */
    AD9517_REF_CLOCK_TOO_HIGH = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 9), /**> Refence frequency is too large */
    AD9517_VCO_OUT_OF_RANGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 10), /**> VCO range constraint is out of bound */
    AD9517_VCODIVIDER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9517, 11),  /**> VCO divider is out of range */
};

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

