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
 * @file    mi250_user.h
 * @brief   Functions to control the MI250 FMC card
 * 
 * $Date: 2016/01/22 22:26:51 $
 * $Revision: 1.2 $
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
#ifndef __MI250USER__
#define __MI250USER__
#include "bas_error.h"
#include "mi250.h"
#include "mi250_defines.h"
#include "mi250_types.h"



/****************************************************************************
* Function: MI250_PLL_Calibrate
*************************************************************************//**
*
* Calibrates the PLL and the data lanes for operation at a frequency
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] acqfreq ADC clock frequency in Hertz
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_PLL_Calibrate(MI250Board_t * MI250Board, unsigned int acqfreq);

/****************************************************************************
* Function: MI250_PLL_CalculateParams
*************************************************************************//**
*
* Calculates PLL parameters from requested clock frequencies and fills a
* PLL_PARAM structure with the results.
*
* @param[in] refClk PLL reference clock frequency in hertz
*
* @param[in] AdcClk ADC clock frequency in hertz
*
* @param[in] cfgParam PLL configuration parameter: PLL_PHASE_NOISE (set vco frequency to lowest) 
*                         or PLL_DUTY_50 (set vco frequency to lowest with duty cyle of 50%, if possible).
*
* @param[out] param parameters for PLL
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_PLL_CalculateParams(unsigned refClk, unsigned AdcClk, MI250_PLL_CFG_PARAM cfgParam, MI250_PLL_PARAM * param);
/****************************************************************************
* Function: MI250_InitPLL
*************************************************************************//**
*
* Initializes the PLL for the specified ADC frequency
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] refclk Reference clock used in Hertz
*
* @param[in] acqclk ADC clock frequency in Hertz
*
* @param[in] clksrc PLL clock source. Either MI250_CLOCKIN_100MHZ for onboard reference,       
*    MI250_CLOCKIN_EXTERNALREF for external reference and MI250_CLOCKIN_EXTERNALCLOCK for external clock
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
adp_result_t MI250_InitPLL(MI250Board_t * MI250Board, unsigned int refclk, unsigned int acqclk, MI250_CLOCK_SOURCE clksrc);

/****************************************************************************
* Function: MI250_PLL_getStatus
*************************************************************************//**
*
* Return PLL current status.
*
* @param[in] board pointer to MI250 board struct
*
* @param[out] status return 1 if locked, 0 if unlocked
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_PLL_getStatus(MI250Board_t * board, MI250_PLL_STATUS * status);


/****************************************************************************
* Function: MI250_ADC_init
************************************************************************//**
*
* Init ADC with MI250 default values
*
* @param[in] board pointer to MI250 board struct
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_ADC_init(MI250Board_t * board);

/****************************************************************************
* Function: MI250_ADC_setGain
*************************************************************************//**
*
* Set ADC gain
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] channel Channel A or B
*
* @param[in] gain ADC gain. See ADC_GAIN for possible values
*
* @param[in] gain DeviceId. DeviceId ID of the ADC pair to modify  (MI250_ADC_AB, MI250_ADC_CD, MI250_ADC_EF or MI250_ADC_GH)
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_ADC_setGain(MI250Board_t * board, MI250_ADC_CHANNEL channel, MI250_ADC_GAIN gain, enum MI250_DEVICES DeviceId);

/****************************************************************************
* Function: MI250_ADC_setFineGain
*************************************************************************//**
*
* Set ADC fine gain
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] channel Channel A or B
*
* @param[in] gain ADC fine gain. 128 step. +0.001 dB to +0.134 dB
*
* @param[in] gain DeviceId. DeviceId ID of the ADC pair to modify  (MI250_ADC_AB, MI250_ADC_CD, MI250_ADC_EF or MI250_ADC_GH)
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_ADC_setFineGain(MI250Board_t * board, MI250_ADC_CHANNEL channel, char gain, enum MI250_DEVICES DeviceId);

/****************************************************************************
* Function: MI250_ADC_setPedestal
*************************************************************************//**
*
* Set ADC Pedestal
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] channel Channel A or B
*
* @param[in] value pedestal 6 bit value. See ADS62P49 data sheet.
*
* @param[in] gain DeviceId. DeviceId ID of the ADC pair to modify (MI250_ADC_AB, MI250_ADC_CD, MI250_ADC_EF or MI250_ADC_GH)
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_ADC_setPedestal(MI250Board_t * board, MI250_ADC_CHANNEL channel, char value, enum MI250_DEVICES DeviceId);

/****************************************************************************
* Function: MI250_ADC_arm
*************************************************************************//**
*
* Arm ADC. Enables the data valid on all 8 channels
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] armed Armed status to be set
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_ADC_arm(MI250Board_t * board, MI250_ADC_ARMED_STATUS armed);

/****************************************************************************
* Function: MI250_ADC_SetDataFormat
*************************************************************************//**
*
* Sets the ADC data format for a device
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] DeviceId ID of the ADC pair to modify (MI250_ADC_AB, MI250_ADC_CD, MI250_ADC_EF or MI250_ADC_GH)
*
* @param[in] format Data format to be set. Either MI250_ADC_TWOCMP or MI250_ADC_OFFSET
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_ADC_SetDataFormat(MI250Board_t * board, MI250_DEVICES DeviceId,  MI250_ADC_DATAFORMAT format);

/****************************************************************************
* Function: MI250_ADC_SetSleepMode
*************************************************************************//**
*
* Sets the ADC sleep mode for a device
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] DeviceId ID of the ADC pair to modify (MI250_ADC_AB, MI250_ADC_CD, MI250_ADC_EF or MI250_ADC_GH)
*
* @param[in] mode Sleep mode to be set from MI250_ADC_SLEEP_MODE enumaration. 
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_ADC_SetSleepMode(MI250Board_t * board, MI250_DEVICES DeviceId,  MI250_ADC_SLEEP_MODE mode);


/****************************************************************************
* Function: MI250_ADC_SetOffsetCorrectionTime
*************************************************************************//**
*
* Sets the offset correction time for a both channels of a device
*
* @param[in] board pointer to MI250 board struct
*
* @param[in] DeviceId ID of the ADC pair to modify (MI250_ADC_AB, MI250_ADC_CD, MI250_ADC_EF or MI250_ADC_GH)
*
* @param[in] octa Time for channel A. From MI250_CHANNEL_OCT enumeration.
*
* @param[in] octb Time for channel B. From MI250_CHANNEL_OCT enumeration.
*
* Error code: #MI250_OK if no problem is found, !=#MI250_OK if problem.
*
****************************************************************************/
Result MI250_ADC_SetOffsetCorrectionTime(MI250Board_t * board,  MI250_DEVICES DeviceId, MI250_CHANNEL_OCT octa, MI250_CHANNEL_OCT octb);





#endif
