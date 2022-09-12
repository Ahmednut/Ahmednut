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
 * @file       eapi_fmcomms3.h
 * @brief      FMCOMMS3 EAPI module related functions definitions
 *
 * $Date: 2015/09/01 14:24:58 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCOMMS3
 *
 * This file is part of Nutaq's ADP Software Suite.
 *
 * You may at your option receive a license to Nutaq's ADP Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's ADP Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's ADP Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's ADP Software Suite under the GPL requires that your work based on
 * Nutaq's ADP Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's ADP Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's ADP Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef __EAPI_FMCOMMS3__
#define __EAPI_FMCOMMS3__

#include "eapi.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ad9361_api.h"
#include "ad9361.h"

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

/** @defgroup FMCOMMS3 FMCOMMS3 Module
 *
 *  FMCOMMS3 module allows to control an FMCOMMS3 FMC card.
 *  The FMCOMMS3 has an AD9361 RFIC chip on it. This chip can
 *  handle 2 TX and 2 RX channels.
 *
 *  @file
 *
 */

 /****************************************************************************
 * Function: Fmcomms3_presence_send
 ************************************************************************//**
 *
 * Check for a FMCOMMS3 module presence in specified board position.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 ***************************************************************************/
Result Fmcomms3_presence_send(connection_state * state, unsigned uBoardNum);

 /****************************************************************************
 * Function: Fmcomms3_powerup_send
 ************************************************************************//**
 *
 * Power up a FMCOMMS3 module in specified board position.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 ***************************************************************************/
Result Fmcomms3_powerup_send(connection_state * state, unsigned uBoardNum);

 /***************************************************************************
 * Function: Fmcomms3_init_send
 ************************************************************************//**
 *
 * Initialize the FMCOMMS3 card.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] init_param
 *    Pointer to an AD9361_InitParam structure that describes all the initial
 *    parameters of the AD9361 chip.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send()
 *
 * @see
 *    Fmcomms3_powerup_send()
 *
 ***************************************************************************/
Result Fmcomms3_init_send(connection_state * state, unsigned uBoardNum, AD9361_InitParam * init_param);

 /***************************************************************************
 * Function: Fmcomms3_digital_calibration_send
 ************************************************************************//**
 *
 * Calibrate the digital communication between the FPGA and the AD9361 chip.
 *
 * This function should be called once the board is initialized and that the
 * AD9361 internal baseband PLL has its final configuration.
 *
 * This function should be called every time the baseband PLL configuration
 * is changed.
 *
 * In order to have the same digital communication configuration across
 * different FMCOMMS3 on different systems, Fmcomms3_set_master_calib_result_send()
 * and Fmcomms3_set_ext_master_calib_result_send() functions can be used. This will
 * force the board to calibrate near the calibration master board result.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_init_send(), Fmcomms3_powerup_send()
 *
 * @see
 *    Fmcomms3_init_send(), Fmcomms3_powerup_send(),
 *    Fmcomms3_set_master_calib_result_send(), Fmcomms3_set_ext_master_calib_result_send()
 *
 ***************************************************************************/
Result Fmcomms3_digital_calibration_send(connection_state * state, unsigned uBoardNum);

 /***************************************************************************
 * Function: Fmcomms3_set_sync_delay_send
 ************************************************************************//**
 *
 * Set the output analog delay of the Sync signal that going out of the FPGA
 * to the SYNC pin of the AD9361 chip.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] delay
 *    Delay from 0 to 31. Each step correspond to 73 ps.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_sync_delay_send(connection_state * state, unsigned uBoardNum, int32_t delay);

 /***************************************************************************
 * Function: Fmcomms3_set_refclk_delay_send
 ************************************************************************//**
 *
 * Set the input analog delay of the Clock Out signal that goes out of the
 * AD9361 chip to the FPGA.
 *
 * This clock is used to generate the Sync signal. Since the Sync signal must
 * be synchronous to the reference clock of the AD9361 chip, the AD9361 chip
 * must be configured to output the buffered version of the reference clock
 * on its Clock Out pin.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] delay
 *    Delay from 0 to 31. Each step correspond to 73 ps.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_refclk_delay_send(connection_state * state, unsigned uBoardNum, int32_t delay);

 /***************************************************************************
 * Function: Fmcomms3_set_sync_bitslip_send
 ************************************************************************//**
 *
 * Set the clock edge sensitivity of the Sync signal that is going out of the FPGA
 * to the SYNC pin of the AD9361 chip related to the reference clock.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] bitslip
 *    0: The Sync signal is synchronous to the rising edge of the reference clock
 *    1: The Sync signal is synchronous to the falling edge of the reference clock
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_sync_bitslip_send(connection_state * state, unsigned uBoardNum, int32_t bitslip);

 /***************************************************************************
 * Function: Fmcomms3_set_sync_src_send
 ************************************************************************//**
 *
 * Select the source of the Sync signal that goes out of the FPGA
 * to the SYNC pin of the AD9361 chip.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] src
 *    0: Software sync signal that is toggled by the Fmcomms3_soft_sync_send() function.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_sync_src_send(connection_state * state, unsigned uBoardNum, int32_t src);

 /***************************************************************************
 * Function: Fmcomms3_soft_sync_send
 ************************************************************************//**
 *
 * Toggle the software sync signal from low to high and set it back to low.
 *
 * In order for the software sync to be outputted to the AD9361 chip,
 * the source of the sync signal should be set at 0 with the
 * Fmcomms3_set_sync_src() function call.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_sync_src()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_sync_src()
 *
 ***************************************************************************/
Result Fmcomms3_soft_sync_send(connection_state * state, unsigned uBoardNum);

 /***************************************************************************
 * Function: Fmcomms3_mcs_send
 ************************************************************************//**
 *
 * Set the AD9361 chip in the different states required in order to do
 * multi chip synchronization.
 *
 * A Sync rising-edge event must happened between steps 1 and 2, and between
 * steps 2 and 3.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] step
 *    1: BBPLL will be synchronized on the next Sync signal rising-edge.
 *    2: BB dividers will be synchronized on the next Sync signal rising-edge.
 *    3: Disable sensitivity to Sync signal.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_mcs_send(connection_state * state, unsigned uBoardNum, int32_t step);

 /***************************************************************************
 * Function: Fmcomms3_set_master_calib_result_send
 ************************************************************************//**
 *
 * In order to make sure all board digital calibrations converge to the same
 * result, one board is designed as the calibration master. The calibration
 * result of the master is then passed to the other board in order for them
 * to converge to the same result than the master.
 *
 * The master calibration should have succeeded before calling
 * Fmcomms3_digital_calibration_send() function on the slave board.
 * However, Fmcomms3_set_master_calib_result_send() function can be called before
 * the master board is calibrated.
 *
 * Since this function used the Fmcomms3 board number of the master board as
 * argument, its scope is limited to board on the same carrier board.
 * If two Fmcomms3 board are located on different carriers, use
 * Fmcomms3_set_ext_master_calib_result_send() function instead.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] uMasterBoardNum
 *    Board number of the master board located on the same carrier.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(),
 *    Fmcomms3_digital_calibration_send(), Fmcomms3_set_ext_master_calib_result_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_master_calib_result_send(connection_state * state, unsigned uBoardNum, unsigned uMasterBoardNum);

 /***************************************************************************
 * Function: Fmcomms3_set_ext_master_calib_result_send
 ************************************************************************//**
 *
 * In order to make sure all board digital calibrations converge to the same
 * result, one board is designed as the calibration master. The calibration
 * result of the master is then passed to the other board in order for them
 * to converge to the same result than the master.
 *
 * The master calibration should have succeeded and the result retrieved before
 * calling Fmcomms3_set_ext_master_calib_result_send() function on the slave board.
 *
 * Since this function used the calibration result structure of the master board as
 * argument, it should be retrieved from the master board first using the
 * Fmcomms3_get_calib_result_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] pCalibResult
 *    Pointer to a sAd9361CalibResult_t structure containing the master board calibration result.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_get_calib_result_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_get_calib_result_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_ext_master_calib_result_send(connection_state * state, unsigned uBoardNum, sAd9361CalibResult_t * pCalibResult);

 /***************************************************************************
 * Function: Fmcomms3_get_calib_result_send
 ************************************************************************//**
 *
 * Once the Fmcomms3_digital_calibration_send() function is called and has succeeded,
 * this function can be called to retrieve the calibration result.
 *
 * The calibration result information is required when synchronizing multiple
 * FMCOMMS3 boards on different carriers with the
 * Fmcomms3_set_ext_master_calib_result_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in,out] pCalibResult
 *    Pointer to a sAd9361CalibResult_t structure that will contain the
 *    calibration result.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_digital_calibration_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(),
 *    Fmcomms3_digital_calibration_send(), Fmcomms3_set_ext_master_calib_result_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_calib_result_send(connection_state * state, unsigned uBoardNum, sAd9361CalibResult_t * pCalibResult);

 /***************************************************************************
 * Function: Fmcomms3_set_en_state_machine_mode_send
 ************************************************************************//**
 *
 * Set the desired Enable State Machine (ENSM) state.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] mode
 *    The desired ENSM state .
 *    [ENSM_STATE_SLEEP_WAIT, ENSM_STATE_ALERT,
 *    ENSM_STATE_TX, ENSM_STATE_TX_FLUSH, ENSM_STATE_RX,
 *    ENSM_STATE_RX_FLUSH, ENSM_STATE_FDD, ENSM_STATE_FDD_FLUSH]
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_en_state_machine_mode_send(connection_state * state, unsigned uBoardNum, uint32_t mode);

 /***************************************************************************
 * Function: Fmcomms3_get_en_state_machine_mode_send
 ************************************************************************//**
 *
 * Get the current Enable State Machine (ENSM) state.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] mode
 *    Pointer that will contain the current ENSM state.
 *    [ENSM_STATE_SLEEP_WAIT, ENSM_STATE_ALERT,
 *    ENSM_STATE_TX, ENSM_STATE_TX_FLUSH, ENSM_STATE_RX,
 *    ENSM_STATE_RX_FLUSH, ENSM_STATE_FDD, ENSM_STATE_FDD_FLUSH]
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_en_state_machine_mode_send(connection_state * state, unsigned uBoardNum, uint32_t * mode);

 /***************************************************************************
 * Function: Fmcomms3_set_sampling_freq_send
 ************************************************************************//**
 *
 * Calculate and apply the RX and TX clock chain configuration based on
 * the requested sampling frequency and the current FIR configurations.
 *
 * This function has the advantage of being simple to use but for
 * more control the the clock chain, it is recommended to use the
 * Fmcomms3_set_trx_path_clks_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] sampling_freq_hz
 *    Desired RX and TX frequency in Hz.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_trx_path_clks_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_sampling_freq_send(connection_state * state, unsigned uBoardNum, uint32_t sampling_freq_hz);

 /***************************************************************************
 * Function: Fmcomms3_set_sampling_freq_send
 ************************************************************************//**
 *
 * Get the current RX and TX sampling frequency.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] sampling_freq_hz
 *    Pointer that will contain the current RX and TX frequency in Hz.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_sampling_freq_send(connection_state * state, unsigned uBoardNum, uint32_t * sampling_freq_hz);

 /***************************************************************************
 * Function: Fmcomms3_set_trx_path_clks_send
 ************************************************************************//**
 *
 * Set the RX and TX clock chains.
 *
 * It is recommended to design the clock chains using the AD9361 filter design
 * wizard MATLAB apps provided by Analog Device.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] rx_path_clks
 *    Array containing the 6 clock frequency values in Hz for the RX path.
 *
 * @param [in] tx_path_clks
 *    Array containing the 6 clock frequency values in Hz for the TX path.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_trx_path_clks_send(connection_state * state, unsigned uBoardNum, uint32_t * rx_path_clks, uint32_t * tx_path_clks);

 /***************************************************************************
 * Function: Fmcomms3_get_trx_path_clks_send
 ************************************************************************//**
 *
 * Get the current RX and TX clock chains.
 *
 * It is recommended to design the clock chains using the AD9361 filter design
 * wizard MATLAB apps provided by Analog Device.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] rx_path_clks
 *    Array that will be updated with the 6 clock frequency values of the current RX path.
 *
 * @param [out] tx_path_clks
 *    Array that will be updated with the 6 clock frequency values of the current TX path.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_trx_path_clks_send(connection_state * state, unsigned uBoardNum, uint32_t * rx_path_clks, uint32_t * tx_path_clks);

 /***************************************************************************
 * Function: Fmcomms3_set_tx_lo_freq_send
 ************************************************************************//**
 *
 * Set the TX local oscillator frequency.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] lo_freq_hz
 *    TX local oscillator frequency in Hz.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_tx_lo_freq_send(connection_state * state, unsigned uBoardNum,  uint64_t lo_freq_hz);

 /***************************************************************************
 * Function: Fmcomms3_get_tx_lo_freq_send
 ************************************************************************//**
 *
 * Get the TX local oscillator frequency.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] lo_freq_hz
 *    Pointer that will contain the TX local oscillator frequency in Hz.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_tx_lo_freq_send(connection_state * state, unsigned uBoardNum, uint64_t * lo_freq_hz);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_lo_freq_send
 ************************************************************************//**
 *
 * Set the RX local oscillator frequency.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] lo_freq_hz
 *    RX local oscillator frequency in Hz.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_lo_freq_send(connection_state * state, unsigned uBoardNum, uint64_t lo_freq_hz);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_lo_freq_send
 ************************************************************************//**
 *
 * Get the RX local oscillator frequency.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] lo_freq_hz
 *    Pointer that will contain the RX local oscillator frequency in Hz.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_lo_freq_send(connection_state * state, unsigned uBoardNum, uint64_t * lo_freq_hz);

 /***************************************************************************
 * Function: Fmcomms3_set_tx_attenuation_send
 ************************************************************************//**
 *
 * Set the transmit attenuation for the selected channel.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to apply the specified TX attenuation [1,2].
 *
 * @param [in] attenuation_mdb
 *    Attenuation in milli dB to apply.
 *    Valid values are from 0 to 89750 mdB. The values is floored to the previous
 *    250 mdB step.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_tx_attenuation_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint32_t attenuation_mdb);

 /***************************************************************************
 * Function: Fmcomms3_get_tx_attenuation_send
 ************************************************************************//**
 *
 * Get the transmit attenuation for the selected channel.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to retrieve the TX attenuation [1,2].
 *
 * @param [out] attenuation_mdb
 *    Pointer that will contain the current TX attenuation in milli dB.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_tx_attenuation_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint32_t * attenuation_mdb);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_gain_control_mode_send
 ************************************************************************//**
 *
 * Set the gain control mode for the selected channel.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to apply the specified mode [1,2].
 *
 * @param [in] gc_mode
 *    RX gain control mode.
 *    [RF_GAIN_MGC,RF_GAIN_FASTATTACK_AGC,
 *    RF_GAIN_SLOWATTACK_AGC,RF_GAIN_HYBRID_AGC]
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_gain_control_mode_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t gc_mode);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_gain_control_mode_send
 ************************************************************************//**
 *
 * Get the gain control mode for the selected channel.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to retrieve the mode [1,2].
 *
 * @param [in] gc_mode
 *    Pointer that will contain the current RX gain control mode.
 *    [RF_GAIN_MGC,RF_GAIN_FASTATTACK_AGC,
 *    RF_GAIN_SLOWATTACK_AGC,RF_GAIN_HYBRID_AGC]
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_gain_control_mode_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t * gc_mode);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_rf_gain_send
 ************************************************************************//**
 *
 * Set the receive RF gain for the selected channel.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to apply the specified RX gain [1,2].
 *
 * @param [in] gain_db
 *    RX gain in dB. The valid values can range from -10 to 77 dB and it depends on
 *    the RX LO frequency.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_rf_gain_send(connection_state * state, unsigned uBoardNum, uint8_t ch, int32_t gain_db);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_rf_gain_send
 ************************************************************************//**
 *
 * Get the receive RF gain for the selected channel.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to retrieve the current RX gain [1,2].
 *
 * @param [in] gain_db
 *    Pointer that will contain the current RX gain in dB.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_rf_gain_send(connection_state * state, unsigned uBoardNum, uint8_t ch, int32_t * gain_db);

 /***************************************************************************
 * Function: Fmcomms3_set_tx_rf_bandwidth_send
 ************************************************************************//**
 *
 * Set the TX RF bandwidth.
 *
 * It is recommended to calculate the bandwidth value using the AD9361 filter
 * design wizard MATLAB apps provided by Analog Device since it takes into
 * account every filter of the AD9361 chip.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] bandwidth_hz
 *    Bandwidth to apply to the TX path.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_tx_rf_bandwidth_send(connection_state * state, unsigned uBoardNum, uint32_t bandwidth_hz);

 /***************************************************************************
 * Function: Fmcomms3_get_tx_rf_bandwidth_send
 ************************************************************************//**
 *
 * Get the TX RF bandwidth.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] bandwidth_hz
 *    Pointer that will contain the current TX bandwidth.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_tx_rf_bandwidth_send(connection_state * state, unsigned uBoardNum, uint32_t * bandwidth_hz);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_rf_bandwidth_send
 ************************************************************************//**
 *
 * Set the RX RF bandwidth.
 *
 * It is recommended to calculate the bandwidth value using the AD9361 filter
 * design wizard MATLAB apps provided by Analog Device since it takes into
 * account every filter of the AD9361 chip.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] bandwidth_hz
 *    Bandwidth to apply to the RX path.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_rf_bandwidth_send(connection_state * state, unsigned uBoardNum, uint32_t bandwidth_hz);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_rf_bandwidth_send
 ************************************************************************//**
 *
 * Get the RX RF bandwidth.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] bandwidth_hz
 *    Pointer that will contain the current RX bandwidth.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_rf_bandwidth_send(connection_state * state, unsigned uBoardNum, uint32_t * bandwidth_hz);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_fir_config_send
 ************************************************************************//**
 *
 * Set the RX FIR filter configuration
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] fir_cfg
 *    Pointer to an AD9361_RXFIRConfig structure containing the desired
 *    RX FIR configuration.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_fir_config_send(connection_state * state, unsigned uBoardNum, AD9361_RXFIRConfig * fir_cfg);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_fir_config_send
 ************************************************************************//**
 *
 * Get the RX FIR filter configuration
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] fir_cfg
 *    Pointer to a AD9361_RXFIRConfig structure that will contain the
 *    current RX FIR configuration.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_fir_config_send(connection_state * state, unsigned uBoardNum, uint8_t ch, AD9361_RXFIRConfig * fir_cfg);

 /***************************************************************************
 * Function: Fmcomms3_set_tx_fir_config_send
 ************************************************************************//**
 *
 * Set the TX FIR filter configuration
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] fir_cfg
 *    Pointer to an AD9361_TXFIRConfig structure containing the desired
 *    TX FIR configuration.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_tx_fir_config_send(connection_state * state, unsigned uBoardNum, AD9361_TXFIRConfig * fir_cfg);

 /***************************************************************************
 * Function: Fmcomms3_get_tx_fir_config_send
 ************************************************************************//**
 *
 * Get the TX FIR filter configuration
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] fir_cfg
 *    Pointer to a AD9361_TXFIRConfig structure that will contain the
 *    current TX FIR configuration.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_tx_fir_config_send(connection_state * state, unsigned uBoardNum, uint8_t ch, AD9361_TXFIRConfig * fir_cfg);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_fir_en_dis_send
 ************************************************************************//**
 *
 * Enable/disable the RX FIR filter.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] en_dis
 *    0: Disable RX FIR filter
 *    1: Enable RX FIR filter
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_fir_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t en_dis);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_fir_en_dis_send
 ************************************************************************//**
 *
 * Get current RX FIR filter enable state.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] en_dis
 *    Pointer that will hold the current RX filter enable state.
 *    0: RX FIR filter is disabled
 *    1: RX FIR filter is enabled
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_fir_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t * en_dis);

 /***************************************************************************
 * Function: Fmcomms3_set_tx_fir_en_dis_send
 ************************************************************************//**
 *
 * Enable/disable the TX FIR filter.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] en_dis
 *    0: Disable TX FIR filter
 *    1: Enable TX FIR filter
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_tx_fir_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis);

 /***************************************************************************
 * Function: Fmcomms3_get_tx_fir_en_dis_send
 ************************************************************************//**
 *
 * Get current TX FIR filter enable state.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] en_dis
 *    Pointer that will hold the current TX filter enable state.
 *    0: TX FIR filter is disabled
 *    1: TX FIR filter is enabled
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_tx_fir_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t * en_dis);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_rssi_send
 ************************************************************************//**
 *
 * Get the Received Signal Strength Indicator (RSSI) for the selected channel
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to retrieve the RSSI [1,2].
 *
 * @param [out] en_dis
 *    Pointer on a rf_rssi structure that will hold the RSSI of the
 *    selected channel.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_rssi_send(connection_state * state, unsigned uBoardNum, uint8_t ch, struct rf_rssi * rssi);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_rfdc_track_en_dis_send
 ************************************************************************//**
 *
 * Enable/disable the RX RFDC Tracking.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] en_dis
 *    0: Disable RX RFDC Tracking
 *    1: Enable RX RFDC Tracking
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_rfdc_track_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_rfdc_track_en_dis_send
 ************************************************************************//**
 *
 * Get the status of the RX RFDC Tracking.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] en_dis
 *    0: RX RFDC Tracking is disabled
 *    1: RX RFDC Tracking is enabled
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_rfdc_track_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t * en_dis);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_bbdc_track_en_dis_send
 ************************************************************************//**
 *
 * Enable/disable the RX Baseband DC Tracking.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] en_dis
 *    0: Disable RX Baseband DC Tracking
 *    1: Enable RX Baseband DC Tracking
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_bbdc_track_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_bbdc_track_en_dis_send
 ************************************************************************//**
 *
 * Get the status of the RX Baseband DC Tracking.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] en_dis
 *    0: RX Baseband DC Tracking is disabled
 *    1: RX Baseband DC Tracking is enabled
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_bbdc_track_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t * en_dis);

 /***************************************************************************
 * Function: Fmcomms3_set_rx_quad_track_en_dis_send
 ************************************************************************//**
 *
 * Enable/disable the RX Quadrature Tracking.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] en_dis
 *    0: Disable RX Quadrature Tracking
 *    1: Enable RX Quadrature Tracking
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_rx_quad_track_en_dis_send(connection_state * state, unsigned uBoardNum,  uint8_t en_dis);

 /***************************************************************************
 * Function: Fmcomms3_get_rx_quad_track_en_dis_send
 ************************************************************************//**
 *
 * Get the status of the RX Quadrature Tracking.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] en_dis
 *    0: RX Quadrature Tracking is disabled
 *    1: RX Quadrature Tracking is enabled
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_rx_quad_track_en_dis_send(connection_state * state, unsigned uBoardNum, uint8_t * en_dis);

 /***************************************************************************
 * Function: Fmcomms3_spi_write_send
 ************************************************************************//**
 *
 * Write SPI register of the AD9361 chip
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] reg
 *    Address of the register to write
 *
 * @param [in] val
 *    Value to write.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_spi_write_send(connection_state * state, unsigned uBoardNum, uint32_t reg, uint8_t val);

 /***************************************************************************
 * Function: Fmcomms3_spi_read_send
 ************************************************************************//**
 *
 * Read SPI register of the AD9361 chip
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] reg
 *    Address of the register to read
 *
 * @param [out] val
 *   Pointer that will hold the value read.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_spi_read_send(connection_state * state, unsigned uBoardNum, uint32_t reg, uint8_t * val);


 /***************************************************************************
 * Function: Fmcomms3_set_dac_datasel_send
 ************************************************************************//**
 *
 * Sets data source for the DAC.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] sel
 *    Source of the data that will be outputted by the DAC.
 *    Only DATA_SEL_USER, DATA_SEL_DDS and DATA_SEL_ZERO are available.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_dac_datasel_send(connection_state * state, unsigned uBoardNum, dds_data_select_e sel);

 /***************************************************************************
 * Function: Fmcomms3_get_dac_datasel_send
 ************************************************************************//**
 *
 * Gets current data selection for the DAC.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] sel
 *    Pointer that will hold the current data source of the DAC.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_dac_datasel_send(connection_state * state, unsigned uBoardNum, dds_data_select_e * sel);

 /***************************************************************************
 * Function: Fmcomms3_get_dds_freq_send
 ************************************************************************//**
 *
 * Sets the DDS frequency.
 *
 * The DDS is only outputted if the DDS data source is selected using
 * Fmcomms3_set_dac_datasel_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to apply the new frequency [1,2].
 *
 * @param [in] tone
 *    Each channel can output two tones at different frequencies.
 *    Specify the tone to apply the new frequency [1,2].
 *
 * @param [in] freq
 *    Frequency to apply in Hz.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_dac_datasel_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_dds_freq_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t freq);

 /***************************************************************************
 * Function: Fmcomms3_get_dds_freq_send
 ************************************************************************//**
 *
 * Gets the DDS frequency.
 *
 * The DDS is only outputted if the DDS data source is selected using
 * Fmcomms3_set_dac_datasel_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to get the current frequency [1,2].
 *
 * @param [in] tone
 *    Each channel can output two tones at different frequencies.
 *    Specify the tone to get the current frequency [1,2].
 *
 * @param [in] freq
 *    Pointer that will hold the current DDS frequency value in Hz.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_dac_datasel_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_dds_freq_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t * freq);

 /***************************************************************************
 * Function: Fmcomms3_set_dds_phase_send
 ************************************************************************//**
 *
 * Sets the DDS phase.
 *
 * The DDS is only outputted if the DDS data source is selected using
 * Fmcomms3_set_dac_datasel_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to apply the new phase [1,2].
 *
 * @param [in] tone
 *    Each channel can output two tones at different phase.
 *    Specify the tone to apply the new phase [1,2].
 *
 * @param [in] phase
 *    Phase to apply in milli degrees.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_dac_datasel_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_dds_phase_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t phase);

 /***************************************************************************
 * Function: Fmcomms3_get_dds_phase_send
 ************************************************************************//**
 *
 * Gets the DDS phase.
 *
 * The DDS is only outputted if the DDS data source is selected using
 * Fmcomms3_set_dac_datasel_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to get the current phase [1,2].
 *
 * @param [in] tone
 *    Each channel can output two tones at different phase.
 *    Specify the tone to get the current phase [1,2].
 *
 * @param [in] phase
 *    Pointer that will hold the current DDS phase value in milli degrees.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_dac_datasel_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_dds_phase_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t * phase);

 /***************************************************************************
 * Function: Fmcomms3_set_dds_scale_send
 ************************************************************************//**
 *
 * Sets the DDS scale in micro unit.
 *
 * The DDS is only outputted if the DDS data source is selected using
 * Fmcomms3_set_dac_datasel_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to apply the new scale [1,2].
 *
 * @param [in] tone
 *    Each channel can output two tones at different scale.
 *    Specify the tone to apply the new scale [1,2].
 *
 * @param [in] scale
 *    Scale to apply in micro unit.
 *    A value of 1000000 corresponds to a full scale DDS signal.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_dac_datasel_send()
 *
 ***************************************************************************/
Result Fmcomms3_set_dds_scale_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t scale);

 /***************************************************************************
 * Function: Fmcomms3_set_dds_scale_send
 ************************************************************************//**
 *
 * Gets the DDS scale in micro unit.
 *
 * The DDS is only outputted if the DDS data source is selected using
 * Fmcomms3_set_dac_datasel_send() function.
 *
 * @param [in] state
 *    Connection state between the host and the CCE.
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ch
 *    Channel to get the current scale [1,2].
 *
 * @param [in] tone
 *    Each channel can output two tones at different scale.
 *    Specify the tone to get the current scale [1,2].
 *
 * @param [in] scale
 *    Pointer that will hold the current DDS scale value in micro unit.
 *
 * @return
 *    Error code: #eFmcomms3Ok if no problem is found, !=#eFmcomms3Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send()
 *
 * @see
 *    Fmcomms3_powerup_send(), Fmcomms3_init_send(), Fmcomms3_set_dac_datasel_send()
 *
 ***************************************************************************/
Result Fmcomms3_get_dds_scale_send(connection_state * state, unsigned uBoardNum, uint8_t ch, uint8_t tone, uint32_t * scale);


#ifdef __cplusplus
}
#endif

#endif

