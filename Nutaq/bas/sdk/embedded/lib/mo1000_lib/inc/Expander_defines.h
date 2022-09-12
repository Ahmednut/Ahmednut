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
 * @file       Expander_defines.h
 * @brief      Mo1000 IO expander data and structure definitions
 *
 * $Date: 2015/10/16 14:31:21 $
 * $Revision: 1.2 $
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

#ifndef EXPANDER_DEFINES_H__
#define EXPANDER_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "Mo1000DevCom_defines.h"
#include "Expander_regs.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Structure for module operation
typedef struct Expander_StructCore {
    Mo1000DevCom_stComCore * m_pCom;
    unsigned uNewRev;
}Expander_stCore;


// Enum to choose controlled dac
typedef enum Expander_EnumDacSelect {
    eDac1,                                 // Dac1 selection
    eDac2                                  // Dac2 selection
}Expander_eDacSelect;


// Enum to choose clock source
typedef enum Expander_EnumClkSrc {
    eInternalClk = 0,                      // Internal 125 MHz clock source
    eExternalClk = 1,                      // External panel clock source
    eCarrier2Clk = 2,                      // Carrier CLK2 fmc clock source
    eCarrier3Clk = 3                       // Carrier CLK3 fmc clock source
}Expander_eClkSrc;

// Enum to choose clock reference
typedef enum Expander_EnumClkRef {
    eMasterRef = 0,                        // Use our own master reference (for master clock mode only)
    eMasterClk = 1                         // Use master provided clock (possible for top board only, requiered when clock slave, optionnal for clock master)
}Expander_eClkRef;

// Local 125MHz oscillator power switch control
typedef enum Expander_EnumLocalOscSw {
    eLocalOscSwOff=0x0,                    // Local oscillator power OFF (default)
    eLocalOscSwOn=0x1                      // Local oscillator power ON
}Expander_eLocalOscSw;


// Bridge reset signal control
typedef enum Expander_EnumBridgeReset {
    eBridgeResetOn=0x0,                    // Bridge reset signal is active (default)
    eBridgeResetOff=0x1                    // Bridge reset signal is released
}Expander_eBridgeReset;


// Dac reset signal control
typedef enum Expander_EnumDacReset {
    eDacResetOn=0x0,                       // Dac reset signal is active (default)
    eDacResetOff=0x1                       // Dac reset signal is released
}Expander_eDacReset;


// Path mux selection signal control
typedef enum Expander_EnumPathMuxSel {
    eMuxLongPath=0x0,                     // Path mux set to long path (PATHB)(default for bottom board)
    eMuxShortPath=0x1                     // Path mux set to short path (PATHA)(default for top board)
}Expander_ePathMuxSel;


// Pll power down signal control
typedef enum Expander_EnumPllPowerDown {
    ePllPowerDownOn=0x0,                  // Pll power down signal is active (default)
    ePllPowerDownOff=0x1                  // Pll power down signal is released
}Expander_ePllPowerDown;


// Pll sync signal control
typedef enum Expander_EnumPllSync {
    ePllSyncOn=0x0,                       // Pll sync signal is active (default)
    ePllSyncOff=0x1                       // Pll sync signal is released
}Expander_ePllSync;


// Dac irq signal status
typedef enum Expander_EnumDacIrq {
    eDacBothErrors=0x0,                   // Both dacs have errors
    eDac2Error=0x1,                       // Dac2 has errors
    eDac1Error=0x2,                       // Dac1 has errors
    eDacNoError=0x3                       // No dac has an error
}Expander_eDacIrq;


// Pll lock signal status
typedef enum Expander_EnumPllLock {
    ePllLockOff=0x0,                      // Pll lock status signal inactive
    ePllLockOn=0x1                        // Pll lock status signal active
}Expander_ePllLock;


// Low level port access
typedef enum Expander_EnumIoPortAddr {
    eIoPortLow=0x0,                      // Io port bits 8 LSB
    eIoPortHigh=0x1                      // Io port bits 8 MSB
}Expander_eIoPortAddr;


// Internal register address
typedef enum Expander_EnumAddr {
    eExpanderAddr00=0,
    eExpanderAddr01=1,
    eExpanderAddr02=2,
    eExpanderAddr03=3,
    eExpanderAddr04=4,
    eExpanderAddr05=5,
    eExpanderAddr06=6,
    eExpanderAddr07=7
} Expander_eAddr;


// Error codes
enum Expander_eError {
    eExpanderOk                = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_MO1000EXPANDER, 0),
    eExpanderInvalidDac        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000EXPANDER, 1),  /**> Invalid dac specified */ 
    eExpanderInvalidClkSource  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000EXPANDER, 2),  /**> Invalid clock source specified */ 
    eExpanderInvalidPort01     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000EXPANDER, 3),  /**> Invalid raw port specified at location 01 */ 
    eExpanderInvalidPort02     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000EXPANDER, 4),  /**> Invalid raw port specified at location 02 */ 
    eExpanderInvalidPort03     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000EXPANDER, 5)   /**> Invalid raw port specified at location 03 */ 
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

