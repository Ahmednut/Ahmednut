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
 * @file       adc5000_common.h
 * @brief      Functions related to the ADC5000 pll parameters.
 *
 * $Date: 2015/04/10 14:08:12 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADC5000
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

#ifndef __ADC5000_COMMON__
#define __ADC5000_COMMON__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

#include "ad9517_defines.h"
#include "adc5000_defines.h"


/***************************************************************************
* Function: adc5000_pll_calculate
************************************************************************//**
*
* Calculate a PLL configuration considering the inputted parameters.
*
* @param [out] param
*    Struct containing the calculated PLL parameters.
*
* @param [in] ref_freq
*    The reference clock frequency.
*
* @param [in] adc_freq
*    The data clock frequency. 
*    Must be 2x the desired sampling rate 
*
* @param [in] clock_output_freq
*    The frequency outputted to clock ouput connector. 
*    Must be 2x the desired sampling rate 
*
* @param [in] fmc_freq
*    The clock frequency sent to the FMC connector.
*    Must be 1/8 of the desired sampling rate 
*
* @param [in] sync_output_freq
*    The frequency of the sync signal.
*    Can be 1/8 of the desired sampling rate 
*
* @param [in] clock_source
*    The source of the reference clock
*
* @return
*    return ADC5000OK on success. If the requested parameters cannot result
*    in a valid PLL configuration, an error will be returned.
*
***************************************************************************/
Result adc5000_pll_calculate(ad9517_param * param, unsigned ref_freq, unsigned adc_freq, unsigned clock_output_freq, unsigned fmc_freq, unsigned sync_output_freq, adc5000_clock_source clock_source);

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
