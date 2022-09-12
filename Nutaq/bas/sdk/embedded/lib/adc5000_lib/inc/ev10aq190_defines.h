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
 * @file      ev10aq190_defines.h
 * 
 * @brief     ev10aq190 defines
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

#ifndef __EV10AQ190_DEFINES__
#define __EV10AQ190_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

#include "com_wrapper_defines.h"

typedef struct S_ev10aq190_core {
    com_core * com;
} ev10aq190_core;

typedef enum E_ev10aq190_testmode {
    EV10AQ190_DISABLE,
    EV10AQ190_BITFLASH,
    EV10AQ190_RAMP,
} ev10aq190_testmode;

typedef enum E_ev10aq190_channel {
    EV10AQ190_CHANNELA=1,
    EV10AQ190_CHANNELB=2,
    EV10AQ190_CHANNELC=3,
    EV10AQ190_CHANNELD=4,
} ev10aq190_channel;

typedef enum E_ev10aq190_mode {
    EV10AQ190_1CHANNEL_A=0x8,
    EV10AQ190_1CHANNEL_B=0x9,
    EV10AQ190_1CHANNEL_C=0xA,
    EV10AQ190_1CHANNEL_D=0xB,
    EV10AQ190_2CHANNEL_A_C=0x4,
    EV10AQ190_2CHANNEL_B_C=0x5,
    EV10AQ190_2CHANNEL_A_D=0x6,
    EV10AQ190_2CHANNEL_B_D=0x7,
    EV10AQ190_4CHANNEL_A_B_C_D=0x0,
    EV10AQ190_4CHANNEL_A_A_A_A=0xC,
    EV10AQ190_4CHANNEL_B_B_B_B=0xD,
    EV10AQ190_4CHANNEL_C_C_C_C=0xE,
    EV10AQ190_4CHANNEL_D_D_D_D=0xF,
} ev10aq190_mode;

typedef enum E_ev10aq190_standby {
    EV10AQ190_FULL_ACTIVE=0x0,
    EV10AQ190_STANDBY_A_B=0x1,
    EV10AQ190_STANDBY_C_D=0x2,
    EV10AQ190_FULL_STANDBY=0x3,
} ev10aq190_standby;

typedef enum E_ev10aq190_coding {
    EV10AQ190_BINARY_CODING=0x0,
    EV10AQ190_GRAY_CODING=0x1,
} ev10aq190_coding;

typedef enum E_ev10aq190_bandwidth {
    EV10AQ190_NOMINAL_BANDWIDTH=0x0,
    EV10AQ190_FULL_BANDWIDTH=0x1,
} ev10aq190_bandwidth;

enum ev10aq190_error {
    EV10AQ190OK       = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_EV10AQ190, 0),
    EV10AQ190_CALIB_TIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_EV10AQ190, 1),       /**> Calibration is not finished */
    EV10AQ190_IVAL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_EV10AQ190, 2),                /**> Invalid parameter */
};

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

