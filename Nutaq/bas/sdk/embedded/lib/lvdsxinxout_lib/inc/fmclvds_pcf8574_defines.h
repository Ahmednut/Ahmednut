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
 * @file    fmclvds_pcf8574_defines.h 
 * @brief   IO expander data and structure definitions
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCLVDS
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

#ifndef __FMCLVDS_PCF8574_DEFINES__
#define __FMCLVDS_PCF8574_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "fmclvds_com_wrapper_defines.h"
#include "fmclvds_pcf8574_regs.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Default register cache value
#define FMCLVDS_PCF8574_DEFAULT00 0xFF        /**< All on */

// Structure for module operation
typedef struct FMCLVDS_S_pcf8574_core {
    FMCLVDS_com_core * com;
    FMCLVDS_pcf8574_Register_Map regcache;    
} FMCLVDS_pcf8574_core;

typedef enum FMCLVDS_E_pcf8574_channel {
    FMCLVDS_PCF8574_CHANNELS0to7=0x0,          /**< Channels 0 to 7 */
    FMCLVDS_PCF8574_CHANNELS8to15=0x1,         /**< Channels 8 to 15 */
    FMCLVDS_PCF8574_CHANNELS16to23=0x2,        /**< Channels 16 to 23 */
    FMCLVDS_PCF8574_CHANNELS24to31=0x3         /**< Channels 24 to 31  */
} FMCLVDS_pcf8574_channel;                     /**< Structure to define for each 4 channels group which ADC must be active and in shutdown */


// Internal register address
typedef enum FMCLVDS_E_pcf8574_addr {
    FMCLVDS_PCF8574_00=0 /**< Internal register address */
} FMCLVDS_pcf8574_addr;


// Trigger out signal possible mode
typedef enum FMCLVDS_E_pcf8574_preemphasis {
    FMCLVDS_PCF8574_PREEMPHASISOFF=0x0,         /**< Preemphasys off */
    FMCLVDS_PCF8574_PREEMPHASISON=0x1           /**< Preemphasys on */
}FMCLVDS_pcf8574_preemp;                        /**<Preemphasis modes */


// ADC possible clock source
typedef enum FMCLVDS_E_pcf8574_powerdown{
    FMCLVDS_PCF8574_POWERDOWN=0x0,    /**<Power down is applied */
    FMCLVDS_PCF8574_POWERUP=0x1       /**<Power down is not applied */
}FMCLVDS_pcf8574_pwdn;                /**<Powerdown modes */


// Error codes
enum FMCLVDS_pcf8574_error {
    FMCLVDS_PCF8574OK       = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_PCF8574, 0) /**< No error */
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

