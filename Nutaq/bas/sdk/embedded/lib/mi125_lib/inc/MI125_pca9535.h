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
 * @file       MI125_pca9535.h
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

#ifndef __MI125_PCA9535_HEADERS__
#define __MI125_PCA9535_HEADERS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "MI125_com_wrapper.h"
#include "MI125_pca9535_regs.h"
#include "MI125_pca9535_defines.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : MI125_pca9535_open
 ************************************************************************//**
 *
 *   Open the pca9535 io expander module
 *   This function must be the first one used before using this module.
 *   When the module is not used anymore, it can be closed with MI125_pca9535_close.
 *   The com_wrapper module must be opened before using this module.
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_open(MI125_pca9535_core * core, MI125_com_core * com);


 /****************************************************************************
 *   NAME : MI125_pca9535_reset
 ************************************************************************//**
 *
 *   Reset the pca9535 module to default operating condition
 *   This function must be called at least once to reset this module to default
 *   operating condition.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_reset(MI125_pca9535_core * core);


 /****************************************************************************
 *   NAME : MI125_pca9535_close
 ************************************************************************//**
 *
 *   Close the pca9535 io expander module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_close(MI125_pca9535_core * core);


 /****************************************************************************
 *   NAME : MI125_pca9535_setshutdown
 ************************************************************************//**
 *
 *   Configure ADC shutdown mode according to specified active channels
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] actchannel
 *      This is the number of 4 channels group not in shutdown (active).
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setshutdown(MI125_pca9535_core * core, MI125_pca9535_channel actchannel);


 /****************************************************************************
 *   NAME : MI125_pca9535_settrgout
 ************************************************************************//**
 *
 *   Configure if the trigger out signal is connected or not from fpga to outside.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *   By default, the trigger out signal is disconnected from outside. 
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] mode
 *      Specify if the trigger out signal is connected (MI125_PCA9535_TRIGOUTON)
 *      or not (MI125_PCA9535_TRIGOUTOFF).
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_settrgout(MI125_pca9535_core * core, MI125_pca9535_trigout mode);


 /****************************************************************************
 *   NAME : MI125_pca9535_setclksource
 ************************************************************************//**
 *
 *   Configure the ADC clock source (including clk source for the clock buffer switch)
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] clksrc
 *      Specify which clock source the ADCs will used
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setclksource(MI125_pca9535_core * core, MI125_pca9535_clksrc clksrc);


 /****************************************************************************
 *   NAME : MI125_pca9535_setbridgereset
 ************************************************************************//**
 *
 *   Control the reset of the spi bridge.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] resetn
 *      Specify the state of the reset signal (active low)
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setbridgereset(MI125_pca9535_core * core, MI125_pca9535_bridgereset resetn);


 /****************************************************************************
 *   NAME : MI125_pca9535_setlocaloscsw
 ************************************************************************//**
 *
 *   Configure if the local 125MHz oscillator is powered up or not.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] sw
 *      Specify the state of the local oscillator power switch signal (active high)
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setlocaloscsw(MI125_pca9535_core * core, MI125_pca9535_localoscsw sw);


 /****************************************************************************
 *   NAME : MI125_pca9535_setbufclkpower
 ************************************************************************//**
 *
 *   Configure if the clock buffer is powered up or not.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] sw
 *      Specify the state of the clock buffer power (active high)
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setbufclkpower(MI125_pca9535_core * core, MI125_pca9535_bufclkpwr sw);


 /****************************************************************************
 *   NAME : MI125_pca9535_setclkbufctrl
 ************************************************************************//**
 *
 *   Configure the clock buffer control signals.
 *   The module must be opened with MI125_pca9535_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] config
 *      Specify the state of the clock buffer configuration signal
 *
 *   @param [in] load
 *      Specify the state of the clock buffer load signal
 *
 *   @param [in] sout
 *      Specify the output selection for the clock buffer signals
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_setclkbufctrl(MI125_pca9535_core * core, MI125_pca9535_bufclkconf config,
                                   MI125_pca9535_bufclkload load, MI125_pca9535_bufclkoutput sout);


 /****************************************************************************
 *   NAME : MI125_pca9535_writeportsraw
 ************************************************************************//**
 *
 *   Writes both output data ports in raw form.
 *
 * @warning  
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module must be reset with MI125_pca9535_reset() before using this function.
 * - Using this function for diagnostic purpose will destroy all other port settings
 *   programmed with other functions of this module. This function is normally used
 *   for production purpose.   
 *   
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [in] port0
 *      Port0 output raw digital value
 *
 *   @param [in] port1
 *      Port1 output raw digital value
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_writeportsraw(MI125_pca9535_core * core, unsigned char port0, unsigned char port1);


 /****************************************************************************
 *   NAME : MI125_pca9535_readportsraw
 ************************************************************************//**
 *
 *   Read both output data ports in raw form.
 *
 * @warning  
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module must be reset with MI125_pca9535_reset() before using this function.
 * - Using this function for diagnostic purpose will destroy all other port settings
 *   programmed with other functions of this module. This function is normally used
 *   for production purpose.
 * - This function does not read the ports pins, but the actual output ports registers
 *   content written with MI125_pca9535_writeportsraw().     
 *   
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [out] port0
 *      Port0 output raw digital value
 *
 *   @param [out] port1
 *      Port1 output raw digital value
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_readportsraw(MI125_pca9535_core * core, unsigned char *port0, unsigned char *port1);


 /****************************************************************************
 *   NAME : MI125_pca9535_readportspinraw
 ************************************************************************//**
 *
 *   Reads both data ports input pins in raw form.
 *
 * @warning  
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The module must be reset with MI125_pca9535_reset() before using this function.
 * - This function is normally used for production purpose.   
 *   
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pca9535_open
 *
 *   @param [out] port0
 *      Port0 pins input raw digital value
 *
 *   @param [out] port1
 *      Port1 pins input raw digital value
 *
 *   @return   Error code: MI125_PCA9535OK if no problem is found, !MI125_PCA9535OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCA9535
 *
 ***************************************************************************/ 
Result MI125_pca9535_readportspinraw(MI125_pca9535_core * core, unsigned char *port0, unsigned char *port1);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

