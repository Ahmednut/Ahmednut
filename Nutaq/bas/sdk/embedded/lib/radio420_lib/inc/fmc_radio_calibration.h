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
 * @file    fmc_radio_calibration.h 
 * @brief   Functions controlling the Radio420X and the Radio421X RF calibration methods.
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
#ifndef __FMC_RADIO_CALIBRATION__
#define __FMC_RADIO_CALIBRATION__

#include "bas_error.h"
#include "fmc_spi.h"
#include "lms6002.h"
#include "radio420_cdce62005.h"
#include "fmc_radio_registers.h"
#include "fmc_radio_defines.h"
#include "fmc_radio_common.h"

/***************************************************************************
* Function: fmc_radio_lo_leakage_calibration
************************************************************************//**
*
* Find calibration values and calibrate IQ path DC offset to reduce local oscillator leakage
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] freq Carrier frequency
*
* @param[out] calib_out Return optimal calibration values. They should remain at about the same value for a given frequency and a given board. You may set with fmc_radio_lo_leakage_calibration_set without doing the calibration again.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lo_leakage_calibration(fmc_radio * fmc_radio, unsigned carrier_freq, unsigned acquisition_freq, lms6002_lo_leakage_calib * calib_out);

/***************************************************************************
* Function: fmc_radio_lo_leakage_calibration_set
************************************************************************//**
*
* Calibrate IQ path DC offset to reduce local oscillator leakage
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] calib Calibration values
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lo_leakage_calibration_set(fmc_radio * fmc_radio, lms6002_lo_leakage_calib * calib);

/***************************************************************************
* Function: fmc_radio_ssb_calibration
************************************************************************//**
*
* Find calibration values and calibrate IQ path gain imbalance and phase imbalance
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] freq Carrier frequency
*
* @param[out] calib_out Return optimal calibration values. They should remain at about the same value for a given frequency and a given board. You may set with fmc_radio_ssb_calibration_set without doing the calibration again.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_ssb_calibration(fmc_radio * fmc_radio, unsigned carrier_freq, unsigned acquisition_freq, lms6002_ssb_calib * calib_out);

/***************************************************************************
* Function: fmc_radio_lo_leakage_calibration_set
************************************************************************//**
*
* Calibrate IQ path gain imbalance and phase imbalance
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] calib Calibration values.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_ssb_calibration_set(fmc_radio * fmc_radio, lms6002_ssb_calib * calib);

/***************************************************************************
* Function: fmc_radio_rx_dc_offset_calibration
************************************************************************//**
*
* Calibrate and configure the DC offset of the RX front-end
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[out] calib_out Calibration values.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rx_dc_offset_calibration(fmc_radio * fmc_radio, lms6002_rx_dc_calib * calib_out);

/***************************************************************************
* Function: fmc_radio_rx_dc_offset_calibration_set
************************************************************************//**
*
* Configure the DC offset of the RX front-end
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] calib Calibration values.
*
* @return
*      return 0 on success.
*
***************************************************************************/

Result fmc_radio_rx_dc_offset_calibration_set(fmc_radio * fmc_radio, lms6002_rx_dc_calib * calib);



#endif

