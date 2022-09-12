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
 * @file       MI125_ltm9012.h
 * @brief      ADC related functions definitions
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

#ifndef __MI125_LTM9012_HEADERS__
#define __MI125_LTM9012_HEADERS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "MI125_com_wrapper.h"
#include "MI125_ltm9012_defines.h"
#include "MI125_ltm9012_regs.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

 /****************************************************************************
 *   NAME : MI125_ltm9012_open(MI125_ltm9012_core * core, MI125_com_core * com)
 ************************************************************************//**
 *
 *   Open the ltm9012 module
 *   This function must be the first one used before using an ADC module.
 *   When the module is not used anymore, it can be closed with MI125_ltm9012_close.
 *   The MI125_com_wrapper module must be opened before using this module. 
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_open(MI125_ltm9012_core * core, MI125_com_core * com);


 /****************************************************************************
 *   NAME : MI125_ltm9012_reset(MI125_ltm9012_core * core, MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Reset the specified ltm9012 ADC module to default operating condition
 *   Default chip operation is configured with complement 2 binary output format. 
 *   This function must be called at least once to reset this device to default
 *   operating condition.
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_reset(MI125_ltm9012_core * core, MI125_COM_DEVICES adcdev);


 /****************************************************************************
 *   NAME : MI125_ltm9012_close(MI125_ltm9012_core * core)
 ************************************************************************//**
 *
 *   Close the ltm9012 adc module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_close(MI125_ltm9012_core * core);


 /****************************************************************************
 *   NAME : MI125_ltm9012_setdigitalcalib(MI125_ltm9012_core * core, MI125_ltm9012_dcalib control, unsigned short pattern, MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Configure the digital output pattern used for calibration or test purpose
 *   and enable or not this mode of operation.
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [in] control
 *      This configures if the digital pattern is active (MI125_LTM9012_DCALIBON)
 *      or not (MI125_LTM9012_DCALIBOFF) on adc outputs
 *
 *   @param [in] pattern
 *      adc 14 bits pattern used for the outputs (only 14 lsb bits are used, 2 msb bits are force to 0)
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_setdigitalcalib(MI125_ltm9012_core * core, MI125_ltm9012_dcalib control, unsigned short pattern, MI125_COM_DEVICES adcdev);


 /****************************************************************************
 *   NAME : MI125_ltm9012_setchannelmode(MI125_ltm9012_core * core, MI125_ltm9012_channel mode, MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Configure which adc channel is active or not on the device. Channels
 *   are configure by group of 4 (0 channel means device in sleep mode) 
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [in] mode
 *      This configures which channels are active, all channels (MI125_LTM9012_4CHANNEL)
 *      or no (MI125_LTM9012_0CHANNEL) channel
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_setchannelmode(MI125_ltm9012_core * core, MI125_ltm9012_channel mode, MI125_COM_DEVICES adcdev);


 /****************************************************************************
 *   NAME : MI125_ltm9012_setconfig(MI125_ltm9012_core * core, MI125_ltm9012_randmode rand, MI125_ltm9012_binmode bin, MI125_ltm9012_lvds lvds, MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Configure adc operating options
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [in] rand
 *      This configures if output randomize mode is used (MI125_LTM9012_RANDOMIZEON)
 *      or not (MI125_LTM9012_RANDOMIZEOFF).
 *      This mode can be used to minimize potential adc output crosstalk
 *      FPGA core must also be configured accordingly 
 *      This format is ignored when MI125_ltm9012_setdigitalcalib is active.
 *
 *   @param [in] bin
 *      This configures adc output data format mode. It could be binary offset (MI125_LTM9012_OFFSETBINARYFORMAT)
 *      or 2 compl (default) (MI125_LTM9012_TWOCOMPLEMENTFORMAT).
 *      This format is ignored when MI125_ltm9012_setdigitalcalib is active.
 *
 *   @param [in] lvds
 *      This configures adc output lvds configuration. This configuration must be choosen to
 *      according to FPGA inputs. An additionnal ADC internal termination can be added and the
 *      drive current can be configured.
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_setconfig(MI125_ltm9012_core * core, MI125_ltm9012_randmode rand, MI125_ltm9012_binmode bin, MI125_ltm9012_lvds lvds, MI125_COM_DEVICES adcdev);


 /****************************************************************************
 *   NAME : MI125_ltm9012_checkadc(MI125_ltm9012_core * core, unsigned char *written,
 *                                 unsigned char *verify, unsigned char *detected,
 *                                 MI125_COM_DEVICES adcdev)
 ************************************************************************//**
 *
 *   Diagnostic function to detect if adc communication interface is working
 *   correctly. 
 *   Warning:  this function must not be used when adc data calibration is
 *   currently running (as set by MI125_ltm9012_setdigitalcalib), because they
 *   are not compatible and will interfere each other. Usually, this function
 *   would only be used for manufacturing purpose.  
 *   The module must be opened with MI125_ltm9012_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_ltm9012_open
 *
 *   @param [out] written
 *      Data pattern that was written for the test
 *
 *   @param [out] verify
 *      Data pattern that was readback (should match written value if everything ok)
 *
 *   @param [out] detected
 *      return a value =0 if the tested adc was not detected correctly, !=0 if it was OK
 *
 *   @param [in] adcdev
 *      Which adc devices to do this function (MI125_COM_ADC0, MI125_COM_ADC1, MI125_COM_ADC2, MI125_COM_ADC3)
 *
 *   @return   Error code: MI125_LTM9012OK if no problem is found, !MI125_LTM9012OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LTM9012
 *
 ***************************************************************************/ 
Result MI125_ltm9012_checkadc(MI125_ltm9012_core * core, unsigned char *written, unsigned char *verify, unsigned char *detected, MI125_COM_DEVICES adcdev);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

