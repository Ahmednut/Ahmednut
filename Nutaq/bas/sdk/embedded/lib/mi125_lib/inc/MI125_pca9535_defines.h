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
 * @file       MI125_pca9535_defines.h
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

#ifndef __MI125_PCA9535_DEFINES__
#define __MI125_PCA9535_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "MI125_com_wrapper_defines.h"
#include "MI125_pca9535_regs.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Structure for module operation
typedef struct MI125_S_pca9535_core {
    MI125_com_core * com;
} MI125_pca9535_core;


// Structure to define for each 4 channels group which ADC must be active and in shutdown
typedef enum MI125_E_pca9535_channel {
    MI125_PCA9535_04CHANNELS=0x1,          // Configure ADC ext shutdown for 4 channels
    MI125_PCA9535_08CHANNELS=0x3,          // Configure ADC ext shutdown for 8 channels
    MI125_PCA9535_12CHANNELS=0x7,          // Configure ADC ext shutdown for 12 channels
    MI125_PCA9535_16CHANNELS=0xF           // Configure ADC ext shutdown for 16 channels (default)
} MI125_pca9535_channel;


// Internal register address
typedef enum MI125_E_pca9535_addr {
    MI125_PCA9535_00=0,
    MI125_PCA9535_01=1,
    MI125_PCA9535_02=2,
    MI125_PCA9535_03=3,
    MI125_PCA9535_04=4,
    MI125_PCA9535_05=5,
    MI125_PCA9535_06=6,
    MI125_PCA9535_07=7
} MI125_pca9535_addr;


// Trigger out signal possible mode
typedef enum MI125_E_pca9535_trigout {
    MI125_PCA9535_TRIGOUTOFF=0x0,         // Trigger out signal disable (default)
    MI125_PCA9535_TRIGOUTON=0x1           // Trigger out signal enable
}MI125_pca9535_trigout;


// ADC possible clock source
typedef enum MI125_E_pca9535_clksrc {
    MI125_PCA9535_CLKSRC125MHZ=0x0,        // Clk src int 125 MHz
    MI125_PCA9535_CLKSRCEXT=0x1,           // Ext clk in source
    MI125_PCA9535_CLKSRCBOTTOMFMC=0x2,     // Bottom FMC adapter clock source: this setting must not be used for bottom FMC (main) card
    MI125_PCA9535_CLKSRCFMCCARRIER=0x4     // FMC carrier clock in source
}MI125_pca9535_clksrc;


// Bridge reset signal control
typedef enum MI125_E_pca9535_bridgereset {
    MI125_PCA9535_BRRESETON=0x0,           // Bridge reset signal is active
    MI125_PCA9535_BRRESETOFF=0x1           // Bridge reset signal is released (default)
}MI125_pca9535_bridgereset;


// Local 125MHz oscillator power switch control
typedef enum MI125_E_pca9535_localoscsw {
    MI125_PCA9535_LOSWON=0x1,              // Local oscillator power ON (default)
    MI125_PCA9535_LOSWOFF=0x0              // Local oscillator power OFF
}MI125_pca9535_localoscsw;


// Clock buffer power switch control
typedef enum MI125_E_pca9535_bufclkpwr {
    MI125_PCA9535_CKBUFON=0x1,             // Clock buffer power ON (default)
    MI125_PCA9535_CKBUFOFF=0x0             // Clock buffer power OFF
}MI125_pca9535_bufclkpwr;


// Clock buffer configuration signal
typedef enum MI125_E_pca9535_bufclkconf {
    MI125_PCA9535_CKBUFCONFON=0x1,         // Clock buffer configuration signal active
    MI125_PCA9535_CKBUFCONFOFF=0x0         // Clock buffer configuration signal inactive (default)
}MI125_pca9535_bufclkconf;


// Clock buffer load signal
typedef enum MI125_E_pca9535_bufclkload {
    MI125_PCA9535_CKBUFLOADON=0x1,         // Clock buffer load signal active
    MI125_PCA9535_CKBUFLOADOFF=0x0         // Clock buffer load signal inactive (default)
}MI125_pca9535_bufclkload;


// Clock buffer output selection signals
typedef enum MI125_E_pca9535_bufclkoutput {
    MI125_PCA9535_CKBUFOUT0=0x0,           // Clock buffer output #0 selection (default)
    MI125_PCA9535_CKBUFOUT1=0x1,           // Clock buffer output #1 selection
    MI125_PCA9535_CKBUFOUT2=0x2,           // Clock buffer output #2 selection
    MI125_PCA9535_CKBUFOUT3=0x3            // Clock buffer output #3 selection
}MI125_pca9535_bufclkoutput;


// Error codes
enum MI125_pca9535_error {
    MI125_PCA9535OK       = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_PCA9535, 0)
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

