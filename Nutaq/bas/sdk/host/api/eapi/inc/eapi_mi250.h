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
 * @file       eapi_mi250.h
 * @brief      Function definitions for the MI250 eapi functionalities
 *
 * $Date: 2016/01/25 22:09:44 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MI250
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

#ifndef __EAPI_MI250__
#define __EAPI_MI250__

#include "eapi.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "mi250_defines.h"
#include "mi250_types.h"



/** @defgroup MI250 MI250 EAPI
 *  
 *  MI250 module EAPI allows to control a 8-channels, 14 bits 250 MSPS A/D FMC module.
 *               
 *  @file
 *     
 */

/*****************************************************************************
* Function: MI250_Select_send
**************************************************************************//**
*
* Select the MI250 module associated to the specified connector number.
* All the following MI250 functions will be applied to the selected module.
*
* @param [in]  state
*      Handle to the connection
 *
 * @param [in] uConnectorNum
 *    FMC connector number of the instance to control (1st one is 1)
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_Select_send(connection_state * state, unsigned uConnectorNum);

/*****************************************************************************
* Function: MI250_Presence_send
**************************************************************************//**
*
* Verifies the MI250 presence
*
* @param [in]  state
*      Handle to the connection
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_Presence_send(connection_state * state);

/*****************************************************************************
* Function: MI250_Powerup_send
**************************************************************************//**
*
* Powers
*
* @param [in]  state
*      Handle to the connection
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_Powerup_send(connection_state * state);

/*****************************************************************************
* Function: MI250_Init_send
**************************************************************************//**
*
* Initializes the MI250
*
* @param [in]  state
*      Handle to the connection
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_Init_send(connection_state * state);

/*****************************************************************************
* Function: MI250_ReadSPIRegisters_send
**************************************************************************//**
*
* Writes on the MI250 SPI Bus
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  DeviceId
*      SPI device chip to be read
*
* @param [in]  BaseRegister
*      SPI register to be read
*
* @param [out]  value
*      Value read
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_ReadSPIRegisters_send(connection_state * state,  MI250_DEVICES DeviceId, int BaseRegister, unsigned char * value);

/*****************************************************************************
* Function: MI250_WriteSPIRegisters_send
**************************************************************************//**
*
* Writes on the MI250 SPI Bus
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  DeviceId
*      SPI device chip to be written
*
* @param [in]  BaseRegister
*      SPI register to be written
*
* @param [out]  value
*      Value to be written
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_WriteSPIRegisters_send(connection_state * state,  MI250_DEVICES DeviceId, int BaseRegister, unsigned char value);

/*****************************************************************************
* Function: MI250_PLLConfig_send
**************************************************************************//**
*
* Calculates the MI250 PLL Parameters and executes configuration
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  reffreq
*      PLL reference clock frequency
*
* @param [in]  adcfreq
*      PLL acquisition clock frequency required 
*
* @param [out]  clocksource
*      Source of the acquisition clock
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_PLLConfig_send(connection_state * state, unsigned int reffreq, unsigned int adcfreq, MI250_CLOCK_SOURCE clocksource);

/*****************************************************************************
* Function: MI250_PLLGetStatus_send
**************************************************************************//**
*
* Verifies and returns the MI250 pll status
*
* @param [in]  state
*      Handle to the connection
*
* @param [out]  locked
*      PLL lock status
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_PLLGetStatus_send(connection_state * state, MI250_PLL_STATUS * locked);


/*****************************************************************************
* Function: MI250_setGain_send
**************************************************************************//**
*
* Sets ADC gain for a channel
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  channel
*      Device channel to be set, 1 or 2
*
* @param [in]  device
*      Device set, MI250_ADC_12, MI250_ADC_34, MI250_ADC_56 or MI250_ADC_78
*
* @param [in]  gain
*      Gain in increments of 0.5 dB. Range is from 0 to 6dB
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_SetGain_send(connection_state * state, MI250_ADC_CHANNEL channel, MI250_DEVICES device, MI250_ADC_GAIN gain);

/*****************************************************************************
* Function: MI250_setFineGain_send
**************************************************************************//**
*
* Sets ADC fine gain for a channel
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  channel
*      Device channel to be set, 1 or 2
*
* @param [in]  device
*      Device set, MI250_ADC_12, MI250_ADC_34, MI250_ADC_56 or MI250_ADC_78
*
* @param [in]  gain
*      Fine Gain. Range is from 0 to 0.134dB with values from 0 to 127
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_SetFineGain_send(connection_state * state, MI250_ADC_CHANNEL channel, MI250_DEVICES device, char gain);

/*****************************************************************************
* Function: MI250_setPedestal_send
**************************************************************************//**
*
* Sets ADC offset for a channel
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  channel
*      Device channel to be set, 1 or 2
*
* @param [in]  device
*      Device set, MI250_ADC_12, MI250_ADC_34, MI250_ADC_56 or MI250_ADC_78
*
* @param [in]  value
*      Pedestal value. Range is from -32 to 31 in ADC increments
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_SetPedestal_send(connection_state * state, MI250_ADC_CHANNEL channel,  MI250_DEVICES device, char value);

/*****************************************************************************
* Function: MI250_ADCreset_send
**************************************************************************//**
*
* Resets the ADC for operation. To be used after PLL is configured
*
* @param [in]  state
*      Handle to the connection
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_ADCreset_send(connection_state * state);

/*****************************************************************************
* Function: MI250_ADCarm_send
**************************************************************************//**
*
* Arms the MI250 core to output a data valid on the ADC data outputs.
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  armedstatus
*      Status is either #MI250_NOT_ARMED or #MI250_ARMED
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_ADCarm_send(connection_state * state, MI250_ADC_ARMED_STATUS armedstatus);

/*****************************************************************************
* Function: MI250_SetDataFormat_send
**************************************************************************//**
*
* Sets the MI250 data format
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  device
*      ADC pair to modify
*
* @param [in]  format
*      Data format to be set.
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_SetDataFormat_send(connection_state * state, MI250_DEVICES device, MI250_ADC_DATAFORMAT format);

/*****************************************************************************
* Function: MI250_SetSleepMode_send
**************************************************************************//**
*
* Sets the MI250 sleep mode
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  device
*      ADC pair to modify
*
* @param [in]  mode
*      Sleep mode to be set.
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_SetSleepMode_send(connection_state * state, MI250_DEVICES device, MI250_ADC_SLEEP_MODE mode);

/*****************************************************************************
* Function: MI250_SetOffsetCorrectionTime_send
**************************************************************************//**
*
* Sends a software trigger to the MI250 FPGA core
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  device
*      ADC pair to modify
*
* @param [in]  octa
*      Offset correction time for channel 1
*
* @param [in]  octb
*      Offset correction time for channel 2
*
*    Error code: #MI250OK if no problem is found, !=#MI250OK if problem.
*
****************************************************************************/
Result MI250_SetOffsetCorrectionTime_send(connection_state * state, MI250_DEVICES device, MI250_CHANNEL_OCT octa, MI250_CHANNEL_OCT octb);

#ifdef __cplusplus
}
#endif



#endif

