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
 * @file       MI125_pcf8574.h
 * @brief      IO expander functions definitions
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

#ifndef __MI125_PCF8574_HEADERS__
#define __MI125_PCF8574_HEADERS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "MI125_com_wrapper.h"
#include "MI125_pcf8574_regs.h"
#include "MI125_pcf8574_defines.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : MI125_pcf8574_open(MI125_pcf8574_core * core, MI125_com_core * com)
 ************************************************************************//**
 *
 *   Open the pcf8574 io expander module
 *   This function must be the first one used before using this module.
 *   When the module is not used anymore, it can be closed with MI125_pcf8574_close.
 *   The com_wrapper module must be opened before using this module.
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_open(MI125_pcf8574_core * core, MI125_com_core * com);


 /****************************************************************************
 *   NAME : MI125_pcf8574_reset(MI125_pcf8574_core * core)
 ************************************************************************//**
 *
 *   Reset the pcf8574 module to default operating condition
 *   This function must be called at least once to reset this module to default
 *   operating condition.
 *   The module must be opened with MI125_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_reset(MI125_pcf8574_core * core);


 /****************************************************************************
 *   NAME : MI125_pcf8574_close(MI125_pcf8574_core * core)
 ************************************************************************//**
 *
 *   Close the pcf8574 io expander module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_close(MI125_pcf8574_core * core);


 /****************************************************************************
 *   NAME : MI125_pcf8574_setshutdown(MI125_pcf8574_core * core, MI125_pcf8574_channel actchannel)
 ************************************************************************//**
 *
 *   Configure ADC shutdown mode according to specified active channels
 *   The module must be opened with MI125_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @param [in] actchannel
 *      This is the number of 4 channels group not in shutdown (active).
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_setshutdown(MI125_pcf8574_core * core, MI125_pcf8574_channel actchannel);


 /****************************************************************************
 *   NAME : MI125_pcf8574_settrgout(MI125_pcf8574_core * core, MI125_pcf8574_trigout mode)
 ************************************************************************//**
 *
 *   Configure if the trigger out signal is connected or not from fpga to outside.
 *   The module must be opened with MI125_pcf8574_open before using this function.
 *   By default, the trigger out signal is disconnected from outside. 
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @param [in] mode
 *      Specify if the trigger out signal is connected (MI125_PCF8574_TRIGOUTON)
 *      or not (MI125_PCF8574_TRIGOUTOFF).
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_settrgout(MI125_pcf8574_core * core, MI125_pcf8574_trigout mode);


 /****************************************************************************
 *   NAME : MI125_pcf8574_setclksource(MI125_pcf8574_core * core, MI125_pcf8574_clksrc clksrc)
 ************************************************************************//**
 *
 *   Configure the ADC clock source
 *   The module must be opened with MI125_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @param [in] clksrc
 *      Specify which clock source the ADCs will used
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_setclksource(MI125_pcf8574_core * core, MI125_pcf8574_clksrc clksrc);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

