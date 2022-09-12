/****************************************************************************   
 * 
 *    ****                              *                                     
 *   ******                            ***                                    
 *   *******                           ****                                   
 *   ********    ****  ****     **** *********    ******* ****    *********** 
 *   *********   ****  ****     **** *********  **************  ************* 
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     **** 
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      **** 
 *  ****    *********  ****     ****   ****   ****       ****  ****      **** 
 *  ****     ********  ****    *****  ****    *****     *****  ****      **** 
 *  ****      ******   ***** ******   *****    ****** *******  ****** ******* 
 *  ****        ****   ************    ******   *************   ************* 
 *  ****         ***     ****  ****     ****      *****  ****     *****  **** 
 *                                                                       **** 
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//** 
 * 
 * @file    eapi_fmcradio.h 
 * @brief   Functions controlling the Radio420X and the Radio421X from a host PC.
 * Function's name that contain "_sdr" are specific for the Radio420X module, function's
 * name that contain "_lte" are specific for the Radio421X module while all
 * the other functions can be used with both types of modules.
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
 
 
/** @defgroup FMCRADIO FMCRADIO EAPI
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

#ifndef __EAPI_FMC_RADIO__
#define __EAPI_FMC_RADIO__

#ifdef __cplusplus
extern "C" {
#endif

#include "connection_state.h"
#include "fmc_radio_defines.h"
#include "radio420_cdce62005.h"


/***************************************************************************
* Function: fmc_radio_presence
************************************************************************//**
*
* Verifies the FMC Radio FPGA core and hardware presence
*
* @param[in,out] state  Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_presence(connection_state * state);

/***************************************************************************
* Function: fmc_radio_sdr_reset_send
************************************************************************//**
*
* Resets and initializes the Radio420X with default values.
*
* @param[in,out] state  Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_reset_send(connection_state * state);

/***************************************************************************
* Function: fmc_radio_powerup_send
************************************************************************//**
*
* Powers up the FMC site for the FMC Radio
*
* @param[in,out] state  Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_powerup_send(connection_state * state);

/***************************************************************************
* Function: fmc_radio_select_send
************************************************************************//**
*
* Selects which FMC Radio is beign configured in a MIMO configuration
*
* @param[in,out] state  Handle to the connection
*
* @param[in] nb  Radio to be configured. 1 for bottom radio, 2 for top radio
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_select_send(connection_state * state, unsigned nb);

/***************************************************************************
* Function: fmc_radio_setrevision_send
************************************************************************//**
*
* Selects the revision of the FMCRadio hardware
*
* @param[in,out] state  Handle to the connection
*
* @param[in] revision  Revision from FMCRADIO_REV enumeration
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_setrevision_send(connection_state * state, FMCRADIO_REV revision);

/***************************************************************************
* Function: fmc_radio_spi_write_send
************************************************************************//**
*
* Writes on the FMC Radio SPI bus
*
* @param[in,out] state  Handle to the connection
*
* @param[in] device  SPI device from FMCRADIO_DEVICES enumeration
*
* @param[in] addr  Register address of the device to access
*
* @param[in] data  Data to write
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_spi_write_send(connection_state * state, FMCRADIO_DEVICES device, unsigned addr, unsigned data);

/***************************************************************************
* Function: fmc_radio_spi_read_send
************************************************************************//**
*
* Reads on the FMC Radio SPI bus
*
* @param[in,out] state  Handle to the connection
*
* @param[in] device  SPI device from FMCRADIO_DEVICES enumeration
*
* @param[in] addr  Register address of the device to access
*
* @param[out] spidata  Data read
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_spi_read_send(connection_state * state, FMCRADIO_DEVICES device, unsigned addr, unsigned * spidata);

/***************************************************************************
* Function: fmc_radio_pll_setparam_send
************************************************************************//**
*
* Writes the PLL parameters to PLL chip
*
* @param[in,out] state  Handle to the connection
*
* @param[in] param  PLL parameters structure
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_pll_setparam_send(connection_state * state, cdce62005_param * param);

/***************************************************************************
* Function: fmc_radio_pll_lockstatus_send
************************************************************************//**
*
* Reads the PLL lock status from the FPGA
*
* @param[in,out] state  Handle to the connection
*
* @param[out] pllstatus  PLL lock status read. Either CDCE62005_PLL_UNLOCKED 
*							or CDCE62005_PLL_LOCKED
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_pll_lockstatus_send(connection_state * state, int * pllstatus);

/***************************************************************************
* Function: fmc_radio_pll_waitforlock_send
************************************************************************//**
*
* Reads the PLL lock status from the FPGA and returns when a lock is detected
*
* @param[in,out] state  Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_pll_waitforlock_send(connection_state * state);


/***************************************************************************
* Function: fmc_radio_clockmux_set_send
************************************************************************//**
*
* Sets the FMC Radio clock multiplexer
*
* @param[in,out] state  Handle to the connection
*
* @param[in] dest  Clock destination from FMCRADIO_CLOCK_OUTPUT_ID enumeration
*
* @param[in] src  Clock source from FMCRADIO_CLOCK_OUTPUT_ID enumeration
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_clockmux_set_send(connection_state * state, FMCRADIO_CLOCK_OUTPUT_ID dest, FMCRADIO_CLOCK_INPUT_ID src);

/***************************************************************************
* Function: fmc_radio_lime_setpllparam_send
************************************************************************//**
*
* Writes the Lime (RF Chip) PLL parameters
*
* @param[in,out] state  Handle to the connection
*
* @param[in] dir   FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*						FMC_RADIO_DIR_RXTX for both rx and tx
*
* @param[in] param  Parameters structure to be set
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lime_setpllparam_send(connection_state * state, LMS6002_DIR dir, lms6002_pll_param * param);

/***************************************************************************
* Function: fmc_radio_lime_calibratePLL_send
************************************************************************//**
*
* Calibrates the LMS6002 PLL
*
* @param[in,out] state  Handle to the connection
*
* @param[in] pll   FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*						FMC_RADIO_DIR_RXTX for both rx and tx
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lime_calibratePLL_send(connection_state * state, LMS6002_DIR pll);

/***************************************************************************
* Function: fmc_radio_lime_reset_send
************************************************************************//**
*
* Resets the RF front end
*
* @param[in,out] state  Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lime_reset_send(connection_state * state);

/***************************************************************************
* Function: fmc_radio_sdr_band_send
************************************************************************//**
*
* Configures the Radio420X transmission and reception band.
*
* @param[in,out] state  Handle to the connection
*
* @param[in] band Receiver band.  
*	  FMC_RADIO_SDR_LOW_BAND  300 to 1500MHz
*	  FMC_RADIO_SDR_HIGH_BAND 1500 to 3000 MHz
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_band_send(connection_state * state, FMC_RADIO_SDR_BAND band);

/***************************************************************************
* Function: fmc_radio_sdr_rx_gain
************************************************************************//**
*
* Configures the gain of the RX path amplifier of the Radio420X module.
*
* @param[in,out] state  Handle to the connection
*
* @param[in] lna_gain Low-noise amplifier gain. 
* 			Choose between LMS6002_LNA_MAXGAIN, LMS6002_LNA_MIDGAIN, 
*           LMS6002_LNA_BYPASS. LNA is not available in high band mode. 
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
Result fmc_radio_sdr_rx_gain_send(connection_state * state, LMS6002_LNA_GAIN lna_gain, LMS6002_VGA1_GAIN rx_gain1, unsigned char rx_gain2, char rx_gain3);

/***************************************************************************
* Function: fmc_radio_rxvga_calibrate_send
************************************************************************//**
*
* Calibrates the DC offset of the RX VGA.
*
* @param[in,out] state  Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rxvga_calibrate_send(connection_state * state);

/***************************************************************************
* Function: fmc_radio_tx_gain_send
************************************************************************//**
*
* Configures the gain of the TX path amplifier.
*
* @param[in,out] state  Handle to the connection
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
Result fmc_radio_tx_gain_send(connection_state * state, char tx_vga1_gain, unsigned char tx_vga2_gain, char tx_gain3);

/***************************************************************************
* Function: fmc_radio_sdr_setrxfilter_send
************************************************************************//**
*
* Selects the RX RF filter.
*
* @param[in,out] state  Handle to the connection
*
* @param[in] bank RF filter from FMCRADIOSDR_FILTER_BANK enumeration
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_sdr_setrxfilter_send(connection_state * state, FMCRADIOSDR_FILTER_BANK bank);

/***************************************************************************
* Function: fmc_radio_lpf_set_send
************************************************************************//**
*
* Selects the RX RF filter.
*
* @param[in,out] state  Handle to the connection
*
* @param[in] dir   FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*						FMC_RADIO_DIR_RXTX for both rx and tx
*
* @param[in] bwt RF filter from LMS6002_LPF_BANDWIDTH enumeration
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lpf_set_send(connection_state * state, LMS6002_DIR dir, LMS6002_LPF_BANDWIDTH bwt);

/***************************************************************************
* Function: fmc_radio_lpf_calibrate_send
************************************************************************//**
*
* Calibrates the low-pass filter of the LMS6002.
*
* @param[in,out] state  Handle to the connection
*
* @param[in] dir   FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*						FMC_RADIO_DIR_RXTX for both rx and tx
*
* @param[in] ref_freq  Reference frequency
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lpf_calibrate_send(connection_state * state,  LMS6002_DIR dir, unsigned ref_freq);

/***************************************************************************
* Function: fmc_radio_path_enable_send
************************************************************************//**
*
* Enables the RF path for a direction
*
* @param[in,out] state  Handle to the connection
*
* @param[in] dir   FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*						FMC_RADIO_DIR_RXTX for both rx and tx
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_path_enable_send(connection_state * state, LMS6002_DIR dir);

/***************************************************************************
* Function: fmc_radio_path_disable_send
************************************************************************//**
*
* Disables the RF path for a direction
*
* @param[in,out] state  Handle to the connection
*
* @param[in] dir   FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*						FMC_RADIO_DIR_RXTX for both rx and tx
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_path_disable_send(connection_state * state, LMS6002_DIR dir);

/***************************************************************************
* Function: fmc_radio_rx_connect_send
************************************************************************//**
*
* Connect the RX signal from antenna to the lms6002
*
* @param[in,out] state  Handle to the connection
*
* @param[in] band LMS6002_BAND bandwidth : LMS6002_LOWBAND or LMS6002_HIGHBAND
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rx_connect_send(connection_state * state, LMS6002_BAND band);

/***************************************************************************
* Function: fmc_radio_rx_disconnect_send
************************************************************************//**
*
* Diconnect the RX signal from antenna to the lms6002
*
* @param[in,out] state  Handle to the connection
*
* @param[in] band LMS6002_BAND bandwidth : LMS6002_LOWBAND or LMS6002_HIGHBAND
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rx_disconnect_send(connection_state * state, LMS6002_BAND band);

/***************************************************************************
* Function: fmc_radio_loopback_send
************************************************************************//**
*
* Configure RF loopback
*
* @param[in,out] state  Handle to the connection
*
* @param[in] mode LMS6002_LOOPBACK_DISABLED to disable the loopback or LMS6002_LOOPBACK_LNA1 to enable it
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_loopback_send(connection_state * state, LMS6002_LOOPBACK_MODE mode);

/***************************************************************************
* Function: fmc_radio_lo_leakage_calibration_send
************************************************************************//**
*
* Finds calibration values and calibrate IQ path DC offset to reduce local oscillator leakage
*
* @param[in,out] state  Handle to the connection
*
* @param[in] carrier_freq Carrier frequency in Hz
*
* @param[in] acq_freq Acquisition frequency in Hz
*
* @param[out] calib_out Return optimal calibration values. They should remain at about 
* the same value for a given frequency and a given board. 
* You may set with fmc_radio_lo_leakage_calibration_set without doing the calibration again.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lo_leakage_calibration_send(connection_state * state, unsigned carrier_freq, unsigned acq_freq, lms6002_lo_leakage_calib * calib_out);

/***************************************************************************
* Function: fmc_radio_lo_leakage_calibration_set_send
************************************************************************//**
*
* Calibrate IQ path DC offset to reduce local oscillator leakage
*
* @param[in,out] state  Handle to the connection
*
* @param[out] calib  Calibration values to be set
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_lo_leakage_calibration_set_send(connection_state * state, lms6002_lo_leakage_calib * calib);

/***************************************************************************
* Function: fmc_radio_ssb_calibration_send
************************************************************************//**
*
* Finds calibration values and calibrate IQ path gain imbalance and phase imbalance
*
* @param[in,out] state  Handle to the connection
*
* @param[in] carrier_freq Carrier frequency in Hz
*
* @param[in] acq_freq Acquisition frequency in Hz
*
* @param[out] calib_out Return optimal calibration values. They should remain at about 
* the same value for a given frequency and a given board. 
* You may set with fmc_radio_ssb_calibration_set_send without doing the calibration again.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_ssb_calibration_send(connection_state * state, unsigned carrier_freq, unsigned acq_freq, lms6002_ssb_calib * calib_out);

/***************************************************************************
* Function: fmc_radio_ssb_calibration_set_send
************************************************************************//**
*
* Calibrates IQ path gain imbalance and phase imbalance
*
* @param[in,out] state  Handle to the connection
*
* @param[out] calib  Calibration values to be set
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_ssb_calibration_set_send(connection_state * state, lms6002_ssb_calib * calib);

/***************************************************************************
* Function: fmc_radio_rx_dc_offset_calibration_send
************************************************************************//**
*
* Calibrate and configure the DC offset of the RX front-end
*
* @param[in,out] state  Handle to the connection
*
* @param[out] calib_out Return optimal calibration values. They should remain at about 
* the same value for a given frequency and a given board. 
* You may set with fmc_radio_ssb_calibration_set_send without doing the calibration again.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rx_dc_offset_calibration_send(connection_state * state, lms6002_rx_dc_calib * calib_out);

/***************************************************************************
* Function: fmc_radio_rx_dc_offset_calibration_set_send
************************************************************************//**
*
* Configure the DC offset of the RX front-end
*
* @param[in,out] state  Handle to the connection
*
* @param[out] calib  Calibration values to be set
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_rx_dc_offset_calibration_set_send(connection_state * state, lms6002_rx_dc_calib * calib);


/***************************************************************************
* Function: fmc_radio_spi_control_set_send
************************************************************************//**
*
* Sets who controls the SPI
*
* @param[in,out] state  Handle to the connection
*
* @param[in] control FPGA Control from FMCRADIO_FPGA_CTRL_LIST enumeration
*
* @param[in] spi_controller SPI controller from FMCRADIO_SPI_CTRL enumeration
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_spi_control_set_send(connection_state * state, FMCRADIO_FPGA_CTRL_LIST control, FMCRADIO_SPI_CTRL spi_controller);

/***************************************************************************
* Function: fmc_radio_set_sampling_frequency
************************************************************************//**
*
* Configuring main PLL
*
* @param[in,out] state  Handle to the connection
*
* @param[in] clk_src FMC clock source
*
* @param[in] uRefFreq Reference clock frequency in Hz
*
* @param[in] uAcqFreq Operating frequency of Lime Micro ADC and DAC in Hz
*
* @param[in] uLimeFreq Clock sent to Lime Micro LMS6002 PLL in Hz
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmc_radio_set_sampling_frequency(connection_state * state, FMCRADIO_CLK_SOURCE clk_src, unsigned uRefFreq, unsigned uAcqFreq, unsigned uLimeFreq);

/****************************************************************************
* Function: fmc_radio_set_local_oscillator
*************************************************************************//**
*
* Configuring Local Oscillator
*
* @param[in,out] state  Handle to the connection
*
* @param[in] dir  FMC_RADIO_DIR_RX for rx, FMC_RADIO_DIR_TX for tx or 
*				  FMC_RADIO_DIR_RXTX for both rx and tx
*
* @param[in] uLimeFreq Clock sent to Lime Micro LMS6002 PLL in Hz
*
* @param[in] uCarrierFreq Carrier frequency in Hz
*
* @return
*      return 0 on success.
*
*****************************************************************************/
Result fmc_radio_set_local_oscillator(connection_state * state, LMS6002_DIR dir, unsigned uLimeFreq, unsigned uCarrierFreq);

#ifdef __cplusplus
}
#endif

#endif
