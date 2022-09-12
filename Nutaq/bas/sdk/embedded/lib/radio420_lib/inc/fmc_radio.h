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
 * @file    fmc_radio.h 
 * @brief    Functions controlling the Radio420X and the Radio421X. Function's name
*  that contain "_sdr" are specific for the Radio420X module, function's
*  name that contain "_lte" are specific for the Radio421X module while all
*  the other functions can be used with both types of modules.
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCRADIO
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/


/** @defgroup FMCRADIO FMCRADIO Module
 *
 *  The Radio420X FPGA mezzanine card (FMC) is a powerful multimode SDR RF
 *  transceiver module designed around the state-of-the-art, multistandard,
 *  multiband Lime Microsystems LMS6002D RF transceiver IC, which supports
 *  broadband coverage, as well as TDD and FDD full duplex modes of operation.
 *  The LMS6002D RF transceiver ICs bandwidth (1.5 to 28 MHz), selectable on-the-fly,
 *  makes it suitable for a large number of narrowband and broadband applications
 *  with excellent channel selectivity. Combined with multiple references and
 *  synchronization modes, the Radio420X is right at home in such applications as:
 *  FMC LPC (1 TRX : SISO)
 *  FMC HPC (2 TRX : MIMO or Dual Band)
 *  - SISO, dual-band and 2x2 MIMO RF transceivers
 *  - Wide frequency range : 300 MHz to 3 GHz
 *  - Selectable bandwidth : 1.5 to 28 MHz
 *  - Selectable RF bands with multistandard filter banks
 *  - Multiple reference configurations and expansion modes
 *  - Individually shielded RX and TX analog paths
 * 
 * @file
 *     
 */

#ifndef __FMC_RADIO__
#define __FMC_RADIO__

// Add this define to disable FMC Radio LTE function because it is not supported yet
#define DISABLE_FMC_RADIO_LTE

#include "bas_error.h"
#include "fmc_spi.h"
#include "lms6002.h"
#include "radio420_cdce62005.h"
#include "fmc_radio_registers.h"
#include "fmc_radio_defines.h"
#include "fmc_radio_common.h"



/***************************************************************************
* Function: fmc_radio_sdr_open
************************************************************************//**
*
* Create handle to Radio420X core.
*
* @param[in,out] fmc_radio pointer to fmc_radio struct
*
* @param[in] base_addr address of the Radio420X FPGA core
*
* @param[in] revision Select the revision of the board that you are using. Check the part number on the Radio420X board. LSP163-121X is the part number where X is the revision number.
*
* @param[in]   pfUsleep
*      Pointer to a sleep function that sleep during N microsecond.
*      This function is the one that will be used inside the library functions.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_open(fmc_radio * fmc_radio, fmc_radio_core * base_addr, FMCRADIO_REV revision, int (*pfUsleep)(unsigned int usec));

#ifndef DISABLE_FMC_RADIO_LTE
/***************************************************************************
* Function: fmc_radio_lte_open
************************************************************************//**
*
* Create handle to Radio421X core.
*
* @param[in,out] fmc_radio pointer to fmc_radio struct
*
* @param[in] base_addr address of the Radio421X FPGA core
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lte_open(fmc_radio * fmc_radio, fmc_radio_core * base_addr);
#endif /*DISABLE_FMC_RADIO_LTE*/

/***************************************************************************
* Function: fmc_radio_sdr_reset
************************************************************************//**
*
* Resets and initializes the Radio420X with default values.
*
* @param[in,out] fmc_radio pointer to fmc_radio struct
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_reset(fmc_radio * fmc_radio);

#ifndef DISABLE_FMC_RADIO_LTE
/***************************************************************************
* Function: fmc_radio_lte_reset
************************************************************************//**
*
* Resets and initializes the Radio421X with default values.
*
* @param[in,out] fmc_radio pointer to fmc_radio struct
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lte_reset(fmc_radio * fmc_radio);
#endif /*DISABLE_FMC_RADIO_LTE*/

/***********************************************************************//**
* Function: fmc_radio_sdr_close
****************************************************************************
*
* Closes Radio420X
*
* @param[in,out] fmc_radio pointer to fmc_radio structure.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_close(fmc_radio * fmc_radio);

#ifndef DISABLE_FMC_RADIO_LTE
/***************************************************************************
* Function: fmc_radio_lte_close
************************************************************************//**
*
* Closes Radio421X
*
* @param[in,out] fmc_radio pointer to fmc_radio structure.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lte_close(fmc_radio * fmc_radio);
#endif /*DISABLE_FMC_RADIO_LTE*/

/***************************************************************************
* Function: fmc_radio_spi_write
************************************************************************//**
*
* Writes to the device through SPI
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] device Device ID
*
* @param[in] addr Address of the SPI register
*
* @param[in] data Data to be written
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_spi_write(fmc_radio * fmc_radio, FMCRADIO_DEVICES device, unsigned addr, unsigned data);

/***************************************************************************
* Function: fmc_radio_spi_read
************************************************************************//**
*
* Reads from the device through SPI
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] device Device ID
*
* @param[in] addr Address of the SPI register
*
* @param[out] data Location of where the read data is stored
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_spi_read(fmc_radio * fmc_radio, FMCRADIO_DEVICES device, unsigned addr, unsigned * data);

/***************************************************************************
* Function: fmc_radio_enable_clock
************************************************************************//**
*
* Enables the design clock from the Radio420
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_enable_clock(fmc_radio * fmc_radio);

/***************************************************************************
* Function: fmc_radio_disable_clock
************************************************************************//**
*
* Disables the design clock from the Radio420
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_disable_clock(fmc_radio * fmc_radio);

/***************************************************************************
* Function: fmc_radio_pll_setparam
************************************************************************//**
*
* Configures CDCE62005 PLL parameters.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] param Pointer to the cdce62005_param struct
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_pll_setparam(fmc_radio * fmc_radio, cdce62005_param * param);

/***************************************************************************
* Function: fmc_radio_pll_lockstatus
************************************************************************//**
*
* Returns whether the CDCE62005 PLL is locked.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[out] state PLL is locked or unlocked.
* 			  If all PLL Freq out = Freq Ref In, return unlocked
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_pll_lockstatus(fmc_radio * fmc_radio, pll_lock_status * state);

/***************************************************************************
* Function: fmc_radio_pll_waitforlock
************************************************************************//**
*
* This a blocking call that wait for the CDCE62005 PLL to lock.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @return
*      return 0 on success, or an error if a timeout has been reached.
*
***************************************************************************/
Result fmc_radio_pll_waitforlock(fmc_radio * fmc_radio);

/***************************************************************************
* Function: fmc_radio_clockmux_set
************************************************************************//**
*
* Configures the clock multiplexer.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] dest Where to send the clock.
*
* @param[in] src Where the clock is from.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_clockmux_set(fmc_radio * fmc_radio, FMCRADIO_CLOCK_OUTPUT_ID dest, FMCRADIO_CLOCK_INPUT_ID src);

/***************************************************************************
* Function: fmc_radio_lime_setparam
************************************************************************//**
*
* Configures LMS6002 PLL parameters.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] dir   FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*						FMC_RADIO_DIR_RXTX for both rx and tx
*
* @param[in] param Radio420X LMS6002 parameter structure
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lime_setpllparam(fmc_radio * fmc_radio, LMS6002_DIR dir, lms6002_pll_param * param);

/***************************************************************************
* Function: fmc_radio_lime_calibratePLL
************************************************************************//**
*
* Calibrates the LMS6002 PLL
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] dir   FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*						FMC_RADIO_DIR_RXTX for both rx and tx
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lime_calibratePLL(fmc_radio * fmc_radio, LMS6002_DIR pll);

/***************************************************************************
* Function: fmc_radio_lime_reset
************************************************************************//**
*
* Resets the LMS6002 RF front end
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lime_reset(fmc_radio * fmc_radio);

/***************************************************************************
* Function: fmc_radio_sdr_band
************************************************************************//**
*
* Configures the Radio420X transmission and reception band.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] band Receiver band.  
*	For Radio420:  FMC_RADIO_SDR_LOW_BAND  300 to 1500MHz
*			       FMC_RADIO_SDR_HIGH_BAND 1500 to 3000 MHz
*	For Radio421:  FMC_RADIO_LTE_LOW_BAND  
*                  FMC_RADIO_LTE_HIGH_BAND
*                  FMC_RADIO_LTE_WIDE_BAND
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_band(fmc_radio * fmc_radio, FMC_RADIO_SDR_BAND band);

#ifndef DISABLE_FMC_RADIO_LTE
/***************************************************************************
* Function: fmc_radio_lte_band
************************************************************************//**
*
* Configures the Radio421X transmission and reception band.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] band Receiver band. 
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lte_band(fmc_radio * fmc_radio, FMC_RADIO_LTE_BAND band);
#endif /*DISABLE_FMC_RADIO_LTE*/

/***************************************************************************
* Function: fmc_radio_sdr_rx_gain
************************************************************************//**
*
* Configures the gain of the RX path amplifier of the Radio420X module.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] lna_gain Low-noise amplifier gain. Choose between bypass, mid gain, and high gain. LNA is not available in high band mode. 
*
* @param[in] rx_gain1 See LMS6002_LNA_GAIN enumeration for possible gain values. 
*
* @param[in] rx_gain2 Gain value in dB. Resolution is 3 dB. rx_gain2 must be between 0 dB and 60 dB. Using a gain above 30 dB is not recommended.
*
* @param[in] rx_gain3 Gain value in dB. The gain value must be between -13 dB and 18 dB. Each step equals 1 dB.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_rx_gain(fmc_radio * fmc_radio, LMS6002_LNA_GAIN lna_gain, LMS6002_VGA1_GAIN rx_gain1, unsigned char rx_gain2, char rx_gain3);

#ifndef DISABLE_FMC_RADIO_LTE
/***************************************************************************
* Function: fmc_radio_lte_rx_gain
************************************************************************//**
*
* Configures the gain of the RX path amplifier of the Radio421X module.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] lna_gain Low-noise amplifier gain. Choose between bypass, mid gain, and high gain. LNA is not available in high band mode. 
*
* @param[in] rx_gain1 See LMS6002_LNA_GAIN enumeration for possible gain values. 
*
* @param[in] rx_gain2 Gain value in dB. Resolution is 3 dB. rx_gain2 must be between 0 dB and 60 dB. Using a gain above 30 dB is not recommended.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lte_rx_gain(fmc_radio * fmc_radio, LMS6002_LNA_GAIN lna_gain, LMS6002_VGA1_GAIN rx_gain1, unsigned char rx_gain2);
#endif /*DISABLE_FMC_RADIO_LTE*/

/***************************************************************************
* Function: fmc_radio_rxvga_calibrate
************************************************************************//**
*
* Calibrates the DC offset of the RX VGA.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rxvga_calibrate(fmc_radio * fmc_radio);

/***************************************************************************
* Function: fmc_radio_tx_gain
************************************************************************//**
*
* Configures the gain of the TX path amplifier.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] tx_vga1_gain Gain value in dB. Valid values from -35 dB to -4 dB.
*
* @param[in] tx_vga2_gain Gain value in dB. Valid values from 0 dB to 25 dB.
*
* @param[in] tx_gain3 Gain value in dB. Gain value must be between -13 dB and 18 dB. Each step equals 1 dB.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_tx_gain(fmc_radio * fmc_radio, char tx_vga1_gain, unsigned char tx_vga2_gain, char tx_gain3);

/***************************************************************************
* Function: fmc_radio_lime_setrxfilter
************************************************************************//**
*
* Selects the RX RF filter.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] bank RF filter.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_setrxfilter(fmc_radio * fmc_radio, FMCRADIOSDR_FILTER_BANK bank);

/***************************************************************************
* Function: fmc_radio_lpf_set
************************************************************************//**
*
* Configures the low-pass filter of the LMS6002.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] dir Rx or Tx
*
* @param[in] bwt RF filter
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lpf_set(fmc_radio * fmc_radio, LMS6002_DIR dir, LMS6002_LPF_BANDWIDTH bwt);

/***************************************************************************
* Function: fmc_radio_lpf_calibrate
************************************************************************//**
*
* Calibrates the low-pass filter of the LMS6002.
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.+
*
* @param[in] Calibration to execute
*
* @param[in] ref_freq Reference frequency
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lpf_calibrate(fmc_radio * fmc_radio, LMS6002_DIR dir, unsigned ref_freq);

/***************************************************************************
* Function: fmc_radio_setIOExp
************************************************************************//**
*
* Configures the I/O expander bits
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] data Data to be ORed in the register.
*
* @param[in] mask Data to be masked of the register.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_setIOexp(fmc_radio * fmc_radio, unsigned data, unsigned mask );

/***************************************************************************
* Function: fmc_radio_getIOexp
************************************************************************//**
*
* Reads the I/O expander bits
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[out] data Pointer to the data of the I/O expander bits.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_getIOexp(fmc_radio * fmc_radio, unsigned * data);

/***************************************************************************
* Function: fmc_radio_path_enable
************************************************************************//**
*
* Enable RF path
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] dir Rx or Tx
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_path_enable(fmc_radio * fmc_radio, LMS6002_DIR dir);

/***************************************************************************
* Function: fmc_radio_path_disable
************************************************************************//**
*
* Disable RF path
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] dir Rx or Tx
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_path_disable(fmc_radio * fmc_radio, LMS6002_DIR dir);

/***************************************************************************
* Function: fmc_radio_rx_connect
************************************************************************//**
*
* Connect the RX signal from antenna to the lms6002
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] band LMS6002_BAND bandwidth : LMS6002_LOWBAND or LMS6002_HIGHBAND
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rx_connect(fmc_radio * fmc_radio, LMS6002_BAND band);

/***************************************************************************
* Function: fmc_radio_rx_disconnect
************************************************************************//**
*
* Diconnect the RX signal from antenna to the lms6002
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] band LMS6002_BAND bandwidth : LMS6002_LOWBAND or LMS6002_HIGHBAND
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rx_disconnect(fmc_radio * fmc_radio, LMS6002_BAND band);

#ifndef DISABLE_FMC_RADIO_LTE
/***************************************************************************
* Function: fmc_radio_lte_poweramp_enable
************************************************************************//**
*
* Enable power amplifier on Radio421X
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] band Receiver band.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lte_poweramp_enable(fmc_radio * fmc_radio, FMC_RADIO_LTE_BAND band);
#endif /*DISABLE_FMC_RADIO_LTE*/

#ifndef DISABLE_FMC_RADIO_LTE
/***************************************************************************
* Function: fmc_radio_lte_poweramp_disable
************************************************************************//**
*
* Disable power amplifier on Radio421X
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] band Receiver band.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lte_poweramp_disable(fmc_radio * fmc_radio, FMC_RADIO_LTE_BAND band);
#endif /*DISABLE_FMC_RADIO_LTE*/

#ifndef DISABLE_FMC_RADIO_LTE
/***************************************************************************
* Function: fmc_radio_lte_set_wideband_switch
************************************************************************//**
*
* Control the Wideband input switch on Radio421X
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] switch_id (RF ground, Wideband, High-band or Low-band input).
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lte_set_wideband_switch(fmc_radio * fmc_radio, FMCRADIOLTE_WIDE_INPUT_SWITCH_ID switch_id);
#endif /*DISABLE_FMC_RADIO_LTE*/

/***************************************************************************
* Function: fmc_radio_loopback
************************************************************************//**
*
* Configure RF loopback
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] mode LMS6002_LOOPBACK_DISABLED to disable the loopback or LMS6002_LOOPBACK_LNA1 to enable it
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_loopback(fmc_radio * fmc_radio, LMS6002_LOOPBACK_MODE mode);

/***************************************************************************
* Function: fmc_radio_setrevision
************************************************************************//**
*
* Sets the revision of the hardware used
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] Revision from FMCRADIO_REV enumeration
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_setrevision(fmc_radio * fmc_radio, FMCRADIO_REV revision);

/***************************************************************************
* Function: fmc_radio_spi_ctrl
************************************************************************//**
*
* Sets who controls the SPI
*
* @param[in,out] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] control FPGA Control from FMCRADIO_FPGA_CTRL_LIST enumeration
*
* @param[in] spi_controller SPI controller from FMCRADIO_SPI_CTRL enumeration
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_spi_ctrl(fmc_radio * fmc_radio, FMCRADIO_FPGA_CTRL_LIST control, FMCRADIO_SPI_CTRL spi_controller);


/***************************************************************************
* Function: fmc_radio_tune_osc
************************************************************************//**
*
* Tune the on-board oscillator frequency
*
* @param[in] fmc_radio Pointer to the fmc_radio structure.
*
* @param[in] fValue Floating point value.
*   Its range is from -1.0f to 1.0f. This range covers the full oscillator
*   tuning range.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_tune_osc(fmc_radio * fmc_radio, float fValue);

/** @name FMC Radio error types
 * 
 *  Possible FMC Radio errors   
 */
enum FMCRADIO_error {
    FMCRADIOOK    = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_FMCRADIO, 0),                  /**< No error */
    FMCRADIOIVAL  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 1),                    /**< Timed out while waiting for the SPI core. */
    FMCRADIONOTSUPPORTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 2),             /**< Command is unsupported by device. */
    FMCRADIOABSENT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 3),                   /**< No FMC card is present */
    FMCRADIOERFTIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 4),               /**< Timed out while reading error value function from calibration core */
    FMCRADIOBACKOFF = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 5),                  /**< Back off compensation is out of range */
    FMCRADIODCCALFAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 6),              /**< RX DC calibration failed */
    FMCRADIOREVISION = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 7),                 /**< This board revision is not supported by the library*/
    FMCRADIODCCALOVERANGE = MAKE_RESULT(LYR_SEVERITY_INFORMATIONAL, MODULE_FMCRADIO, 8),    /**< RX RF front-end DC compensation is at maximum */
    FMCRADIOPLLUNLOCKED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 9),              /**< The PLL did not lock and the timeout was reached */
    FMCRADIOARGOUTOFRANGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 0xA),          /**< A function argument is out of its valid range */
    FMCRADIOINVPTR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCRADIO, 0xB),                 /**< Invalid pointer */
};


#endif

