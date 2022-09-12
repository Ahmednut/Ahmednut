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
 * @file       MI125_ltm9012_defines.h
 * @brief      ADC related data and structure definitions
 *
 * $Date: 2015/04/10 14:08:14 $
 * $Revision: 1.1 $
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

#ifndef __MI125_LTM9012_DEFINES__
#define __MI125_LTM9012_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "MI125_com_wrapper_defines.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Structure for module operation
typedef struct MI125_S_ltm9012_core {
    MI125_com_core * com;
} MI125_ltm9012_core;

// Defines how many channel of the adc are used
typedef enum MI125_E_ltm9012_channel {
    MI125_LTM9012_4CHANNEL=0,
    MI125_LTM9012_0CHANNEL,
} MI125_ltm9012_channel;

// Defines if digital calib mode is used (output fix pattern)
typedef enum MI125_E_ltm9012_dcalib {
    MI125_LTM9012_DCALIBOFF=0,
    MI125_LTM9012_DCALIBON=1
} MI125_ltm9012_dcalib;

// Defines internal ltm9012 adc registers address
typedef enum MI125_E_ltm9012_addr {
    MI125_LTM9012_A0=0,
    MI125_LTM9012_A1=1,
    MI125_LTM9012_A2=2,
    MI125_LTM9012_A3=3,
    MI125_LTM9012_A4=4
} MI125_ltm9012_addr;

// Defines adc output randomize mode
typedef enum MI125_E_ltm9012_randmode {
    MI125_LTM9012_RANDOMIZEOFF=0,
    MI125_LTM9012_RANDOMIZEON=1,
}MI125_ltm9012_randmode;

// Defines adc output binary format used
typedef enum MI125_E_ltm9012_binmode {
    MI125_LTM9012_OFFSETBINARYFORMAT=0,
    MI125_LTM9012_TWOCOMPLEMENTFORMAT=1,
}MI125_ltm9012_binmode;

// Defines lvds output configuration by setting drive current and enabling internal termination or not
typedef enum MI125_E_ltm9012_lvds {
    MI125_LTM9012_TERMOFF1750UA,        // No internal termination, 1.75 mA
    MI125_LTM9012_TERMOFF2100UA,        // No internal termination, 2.1 mA
    MI125_LTM9012_TERMOFF2500UA,        // No internal termination, 2.5 mA
    MI125_LTM9012_TERMOFF3000UA,        // No internal termination, 3.0 mA
    MI125_LTM9012_TERMOFF3500UA,        // No internal termination, 3.5 mA
    MI125_LTM9012_TERMOFF4000UA,        // No internal termination, 4.0 mA
    MI125_LTM9012_TERMOFF4500UA,        // No internal termination, 4.5 mA
    MI125_LTM9012_TERMON1750UA,         // With internal termination, 1.75 mA
    MI125_LTM9012_TERMON2100UA,         // With internal termination, 2.1 mA
    MI125_LTM9012_TERMON2500UA          // With internal termination, 2.5 mA
}MI125_ltm9012_lvds;

// Module errors
enum MI125_ltm9012_error {
    MI125_LTM9012OK       = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_LTM9012, 0),
    MI125_LTM9012_IVAL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LTM9012, 1),      /**> Invalid channel mode parameter */
    MI125_LTM9012_ILVDSVAL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LTM9012, 2),  /**> Invalid lvds parameter */ 
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

