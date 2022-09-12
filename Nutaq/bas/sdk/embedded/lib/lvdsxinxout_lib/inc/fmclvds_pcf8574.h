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
 * @file       fmclvds_pcf8574.h
 * @brief      IO expander functions definitions
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

#ifndef __FMCLVDS_PCF8574_HEADERS__
#define __FMCLVDS_PCF8574_HEADERS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "fmclvds_com_wrapper.h"
#include "fmclvds_defines.h"
#include "fmclvds_pcf8574_regs.h"
#include "fmclvds_pcf8574_defines.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : FMCLVDS_pcf8574_open(FMCLVDS_pcf8574_core * core, FMCLVDS_com_core * com)
 ************************************************************************//**
 *
 *   Open the pcf8574 io expander module
 *   This function must be the first one used before using this module.
 *   When the module is not used anymore, it can be closed with FMCLVDS_pcf8574_close.
 *   The com_wrapper module must be opened before using this module.
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
Result FMCLVDS_pcf8574_open(FMCLVDS_pcf8574_core * core, FMCLVDS_com_core * com);


 /****************************************************************************
 *   NAME : FMCLVDS_pcf8574_reset(FMCLVDS_pcf8574_core * core)
 ************************************************************************//**
 *
 *   Reset the pcf8574 module to default operating condition
 *   This function must be called at least once to reset this module to default
 *   operating condition.
 *   The module must be opened with FMCLVDS_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with FMCLVDS_pcf8574_open
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
Result FMCLVDS_pcf8574_reset(FMCLVDS_pcf8574_core * core);


 /****************************************************************************
 *   NAME : FMCLVDS_pcf8574_close(FMCLVDS_pcf8574_core * core)
 ************************************************************************//**
 *
 *   Close the pcf8574 io expander module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with FMCLVDS_pcf8574_open
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
Result FMCLVDS_pcf8574_close(FMCLVDS_pcf8574_core * core);


 /****************************************************************************
 *   NAME : FMCLVDS_pcf8574_powerdown(FMCLVDS_pcf8574_core * core, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_pwdn powerdown)
 ************************************************************************//**
 *
 *   Sets the FMCLVDS powedown mode
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with FMCLVDS_pcf8574_open 
 *
 *   @param [in] group
 *      LVDS channel group to be powered down or up
 *
 *   @param [in] powerdown
 *      Powerdown mode to be set
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
Result FMCLVDS_pcf8574_powerdown(FMCLVDS_pcf8574_core * core, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_pwdn powerdown);


 /****************************************************************************
 *   NAME : FMCLVDS_pcf8574_preemphasis(FMCLVDS_pcf8574_core * core, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_pwdn powerdown)
 ************************************************************************//**
 *
 *   Sets the FMCLVDS preemphasis mode
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with FMCLVDS_pcf8574_open 
 *
 *   @param [in] group
 *      LVDS channel group to be powered down or up
 *
 *   @param [in] preemphasis
 *      Preemphasis mode to be set
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
Result FMCLVDS_pcf8574_preemphasis(FMCLVDS_pcf8574_core * core,  FMCLVDS_GROUPS group, FMCLVDS_pcf8574_preemp preemphasis);




#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

