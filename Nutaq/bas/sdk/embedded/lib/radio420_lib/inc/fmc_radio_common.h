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
 * @file    fmc_radio_common.h 
 * @brief   Pll Calculation functions.
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCRADIO
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
#ifndef __FMC_RADIO_COMMON__
#define __FMC_RADIO_COMMON__

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************
* Function: fmc_radio_pll_calculate
*************************************************************************//**
*
* Calculate PLL parameters for given operating frequencies.
*
* @param[out] param pointer to cdce62005_param struct
*
* @param[in] ref_freq Reference clock frequency. 0 is for internal 10MHz reference clock
*
* @param[in] adac_freq Operating frequency of Lime Micro ADC and DAC
*
* @param[in] lime_freq Clock sent to Lime Micro LMS6002 PLL
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result fmc_radio_pll_calculate(cdce62005_param * param, unsigned ref_freq, unsigned adac_freq, unsigned lime_freq);

/****************************************************************************
* Function: fmc_radio_pll_calculate_withpfd
************************************************************************//**
*
* Calculate PLL parameters for given operating frequencies.
*
* @param[out] param pointer to cdce62005_param struct
*
* @param[in] ref_freq Reference clock frequency. 0 is for internal 10MHz reference clock
*
* @param[in] pfd_freq PLL PFD Frequency
*
* @param[in] adac_freq Operating frequency of Lime Micro ADC and DAC
*
* @param[in] lime_freq Clock sent to Lime Micro LMS6002 PLL
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result fmc_radio_pll_calculate_withpfd(cdce62005_param * param, unsigned ref_freq, unsigned pfd_freq, unsigned adac_freq, unsigned lime_freq);

/****************************************************************************
* Function: fmc_radio_lime_calculate
*************************************************************************//**
*
* Calculate Lime micro PLLs parameters 
*
* @param[in] param FMC Radio lms6002 parameters struct
*
* @param[in] ref_freq Reference clock frequency. 0 is for internal 10MHz reference clock
*
* @param[in] carrier_freq Carrier frequency
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result fmc_radio_lime_calculate(lms6002_pll_param * param, unsigned ref_freq, unsigned carrier_freq);

#ifdef __cplusplus
}
#endif

#endif

