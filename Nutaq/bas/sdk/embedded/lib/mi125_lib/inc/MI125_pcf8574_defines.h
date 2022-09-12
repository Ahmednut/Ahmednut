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
 * @file       pcf8574_defines.h
 * @brief      IO expander data and structure definitions
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

#ifndef __PCF8574_DEFINES__
#define __PCF8574_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "MI125_com_wrapper_defines.h"
#include "MI125_pcf8574_regs.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Structure for module operation
typedef struct MI125_S_pcf8574_core {
    MI125_com_core * com;
    MI125_pcf8574_Register_Map regcache;    // This is a register cache for the device 
} MI125_pcf8574_core;


// Structure to define for each 4 channels group which ADC must be active and in shutdown
typedef enum MI125_E_pcf8574_channel {
    MI125_PCF8574_04CHANNELS=0x1,          // Configure ADC ext shutdown for 4 channels
    MI125_PCF8574_08CHANNELS=0x3,          // Configure ADC ext shutdown for 8 channels
    MI125_PCF8574_12CHANNELS=0x7,          // Configure ADC ext shutdown for 12 channels
    MI125_PCF8574_16CHANNELS=0xF           // Configure ADC ext shutdown for 16 channels (default)
} MI125_pcf8574_channel;


// Internal register address
typedef enum MI125_E_pcf8574_addr {
    MI125_PCF8574_00=0
} MI125_pcf8574_addr;


// Trigger out signal possible mode
typedef enum MI125_E_pcf8574_trigout {
    MI125_PCF8574_TRIGOUTON=0x0,           // Trigger out signal enable
    MI125_PCF8574_TRIGOUTOFF=0x1           // Trigger out signal disable (default)
}MI125_pcf8574_trigout;


// ADC possible clock source
typedef enum MI125_E_pcf8574_clksrc {
    MI125_PCF8574_CLKSRC125MHZ=0x0,      // Clk src int 125 MHz
    MI125_PCF8574_CLKSRCEXT=0x1,         // Ext clk inout source
    MI125_PCF8574_CLKSRCBOTTOMFMC=0x2,   // Bootom FMC adpater clock source: this setting must not be used for bottom FMC (main) card
    MI125_PCF8574_CLKSRCFMCCARRIER=0x4   // Unused
}MI125_pcf8574_clksrc;


// Error codes
enum MI125_pcf8574_error {
    MI125_PCF8574OK       = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_PCF8574, 0)
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

