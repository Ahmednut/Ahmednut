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
 * @file    lms6002.h 
 * @brief   Function definitions for the LMS6002 control
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
#ifndef __LMS6002__
#define __LMS6002__

#include "bas_error.h"
#include "fmc_spi.h"
#include "lms6002_defines.h"

Result lms6002_lpf_set(lms6002_core * core, LMS6002_DIR dir, LMS6002_LPF_BANDWIDTH bwt);

/****************************************************************************
* Function: lms6002_lpf_calibrateTuningModule
*****************************************************************************
*
* Calibrate the Low Pass Filter calibration module
*
* @param[inout] core pointer to lms6002_core struct
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result lms6002_lpf_calibrateTuningModule(lms6002_core * core);

/****************************************************************************
* Function: lms6002_lpf_tune
*****************************************************************************
*
* Calibrate the Rx and Tx Low Pass Filter
*
* @param[inout] core pointer to lms6002_core struct
*
* @param[in] ref_freq lms6002 PLL reference frequency
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result lms6002_lpf_tune(lms6002_core * core, unsigned ref_freq);

Result lms6002_pll_calculate(lms6002_pll_param * param);
Result lms6002_pll_setparam(lms6002_core * core, lms6002_pll_param * param, LMS6002_PLL pll);

Result lms6002_open(lms6002_core * core, spi_core_t * spi);
Result lms6002_reset(lms6002_core * core);
Result lms6002_enablePath(lms6002_core * core, LMS6002_DIR dir);
Result lms6002_disablePath(lms6002_core * core, LMS6002_DIR dir);
Result lms6002_close(lms6002_core * core);


/****************************************************************************
* Function: lms6002_rx_connect
*****************************************************************************
*
* Connect the RX signal from antenna to the lms6002
*
* @param[inout] core pointer to lms6002_core struct
*
* @param[in] band LMS6002_BAND bandwidth : LMS6002_LOWBAND or LMS6002_HIGHBAND
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result lms6002_rx_connect(lms6002_core * core, LMS6002_BAND band);

/****************************************************************************
* Function: lms6002_rx_disconnect
*****************************************************************************
*
* Disconnect the RX signal from antenna to the lms6002
*
* @param[inout] core pointer to lms6002_core struct
*
* @param[in] band LMS6002_BAND bandwidth : LMS6002_LOWBAND or LMS6002_HIGHBAND
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result lms6002_rx_disconnect(lms6002_core * core, LMS6002_BAND band);

/****************************************************************************
* Function: lms6002_rx_gain
*****************************************************************************
*
* Set gain of lms6002 RX RF front end.
*
* @param[inout] core pointer to lms6002_core struct
*
* @param[in] lna_gain Low noise amplifier gain. Choose between bypass, midgain, and high gain
*
* @param[in] rx_gain1 see LMS6002_LNA_GAIN enum for possible gain values. 
*
* @param[in] rx_gain2 Gain value is in dB. Resolution is 3 dB. Gain above 30 dB is not recommended. Max gain is 60 dB
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result lms6002_rx_gain(lms6002_core * core, LMS6002_LNA_GAIN lna_gain, LMS6002_VGA1_GAIN rx_gain1, unsigned rx_gain2);

/****************************************************************************
* Function: lms6002_tx_gain
*****************************************************************************
*
* Set gain of lms6002 TX RF front end.
*
* @param[inout] core pointer to lms6002_core struct
*
* @param[in] tx_vga1_gain Minimum gain: code 110 gain -35dB  Maximum gain: code 111111111 gain -4dB
*
* @param[in] tx_vga2_gain gain = 20 * log(0.038 * tx_vga2_gain)
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result lms6002_tx_gain(lms6002_core * core, unsigned char tx_vga1_gain, unsigned char tx_vga2_gain);

Result lms6002_loopback(lms6002_core * core, LMS6002_LOOPBACK_MODE mode);

Result lms6002_lpf_calibrateDCOffset(lms6002_core * core, LMS6002_DIR dir);
Result lms6002_rxvga2_calibrate(lms6002_core * core);
Result lms6002_pll_calibrate(lms6002_core * core, LMS6002_PLL pll);

/* Private Function */
Result lms6002_pll_open(lms6002_pll_core * core, unsigned char base_addr);
Result lms6002_pll_close(lms6002_pll_core * core);
Result lms6002_spi_read(lms6002_core * core, unsigned char addr, unsigned char * data);
Result lms6002_spi_write(lms6002_core * core, unsigned char addr, unsigned char data);

enum LMS6002_error {
    LMS6002OK            = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_LMS6002, 0),
    LMS6002IVAL          = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LMS6002, 1),     /**< Invalid parameters */
    LMS6002UNIMPLEMENTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LMS6002, 2),     /**< Functionality is not implemented */
    LMS6002TUNINGERROR   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LMS6002, 3),     /**< Cannot tune to selected frequency */
    LMS6002GENERALERROR  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LMS6002, 4),     /**< PLL calibration failed */
    LMS6002ABSENT        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LMS6002, 5),     /**< Communication with LMS6002 has failed */
    LMS6002DCCALFAILED   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LMS6002, 6),     /**< DC offset calibration has failed */
    LMS6002RXVGA2CALIB   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LMS6002, 7),     /**< RXVGA DC offset calibration has failed */
    LMS6002PLLUNLOCKED   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_LMS6002, 8),     /**< PLL is unlocked */
    LMS6002MULTIPLIER    = MAKE_RESULT(LYR_SEVERITY_WARNING, MODULE_LMS6002, 9),   /**< Frequency is too high */
};

#endif
