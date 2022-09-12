 /***************************************************************************
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
 * @file       Radio640.h
 * @brief      RADIO640 public functions
 *
 * $Date: 2016/05/10 21:00:40 $
 * $Revision: 1.24 $
 *
 * Copyright (C) 2015, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RADIO640
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

#ifndef RADIO640_H_
#define RADIO640_H_

 /*****************************************************************************/
/***************************** Include Files **********************************/
 /*****************************************************************************/
#include "bas_error.h"

#include "Radio640_defines.h"
#include "Radio640DevCom_defines.h"

#include "Ad9361Util.h"
#include "Ad9361Api.h"
#include "Ad9361AdcCore.h"

#include "Ina226_defines.h"

/** @defgroup RADIO640 RADIO640 Module
 *
 *  RADIO640 module allows to control an RADIO640 FMC card.
 *  The RADIO640 has an AD9361 RFIC chip on it. This chip can
 *  handle 2 TX and 2 RX channels.
 *
 *  @file
 *
 */

 /***************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

 /***************************************************************************
 * Function: Radio640_Open
 ************************************************************************//**
 *
 * Allocate memory and initialize structures for embedded Radio640 control.
 *
 * This function should only be used on the processor that directly configures
 * the Radio640 core. On a processor that remotely configures the Radio640,
 * use the Radio640_Connect function instead.
 *
 * @param [in,out] phRadio640
 *    Pointer to receive the handle of the Radio640 object that will be created
 *
 * @param [in] u32BaseAddr
 *    Base address of the Radio640 FPGA core.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open.
 *    The carrier is used to handle the I2C communication, to calculate the
 *    the I2C device addresses and to sleep.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @see
 *    Radio640_Close()
 *
 ***************************************************************************/
Result Radio640_Open(Radio640_hRadio640 * phRadio640,
    uint32_t u32BaseAddr,
    Carrier_handle_t hCarrier);

 /***************************************************************************
 * Function: Radio640_Connect
 ************************************************************************//**
 *
 * Allocate memory and initialize structures for remote Radio640 control.
 *
 * This function should only be used on the processor that remotely configures
 * the Radio640. On a processor that directly configures the Radio640 core,
 * use the Radio640_Open function instead.
 *
 * @param [in,out] phRadio640
 *    Pointer to receive the handle of the Radio640 object that will be created
 *
 * @param [in] hConnectionState
 *    Pointer to the connection state between the host and the CCE.
 *
 * @param [in] u32BoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @see
 *    Radio640_Close()
 *
 ***************************************************************************/
Result Radio640_Connect(Radio640_hRadio640 * phRadio640,
    hConnectionState_t hConnectionState,
    uint32_t u32BoardNum);

/***************************************************************************
 * Function: Radio640_Presence
 ************************************************************************//**
 *
 * Verify the presence of the Radio640 core in the FPGA and the Radio640 
 * FMC board with an EEPROM containing the Radio640 information.
 *
 * This function can only be used on the processor that remotely configures
 * the Radio640. 
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Connect()
 *
 * @see
 *    Radio640_Connect()
 *
 ***************************************************************************/
Result Radio640_Presence(Radio640_hRadio640 hRadio640);

/***************************************************************************
 * Function: Radio640_PowerUp
 ************************************************************************//**
 *
 * Power up the Radio640 VADJ power supply to the value specified
 * in the Radio640 EEPROM.
 *
 * This function can only be used on the processor that remotely configures
 * the Radio640. 
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Connect()
 *
 * @see
 *    Radio640_Connect()
 *
 ***************************************************************************/
Result Radio640_PowerUp(Radio640_hRadio640 hRadio640);

 /***************************************************************************
 * Function: Radio640_Close
 ************************************************************************//**
 *
 * Free the memory allocated by the Radio640_Open or Radio640_Connect function.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect()
 *
 ***************************************************************************/
void Radio640_Close(Radio640_hRadio640 * phRadio640);

 /***************************************************************************
 * Function: Radio640_Init
 ************************************************************************//**
 *
 * Initialize the RADIO640 card and reset all previous configurations.
 *
 * After an initialization, at least the Radio640_DigitalCalibration()
 * function should be called before it is possible to receive and send
 * digital samples between the Radio640 board and the FPGA.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pInitParam
 *    Pointer to an Radio640_stInitParam structure that describes all the initial
 *    parameters of the Radio640 chip.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_DigitalCalibration()
 *
 ***************************************************************************/
Result Radio640_Init(Radio640_hRadio640 hRadio640, Radio640_stInitParam * pInitParam);

 /***************************************************************************
 * Function: Radio640_DigitalCalibration
 ************************************************************************//**
 *
 * Calibrate the digital communication between the FPGA and the AD9361 chip.
 *
 * This function should be called once the board is initialized and that the
 * AD9361 internal baseband PLL is in its final configuration.
 *
 * This function should be called every time the baseband PLL configuration
 * is changed.
 *
 * In order to have the same digital communication configuration across
 * different RADIO640 on different systems, Radio640_SetMasterCalibResult()
 * and Radio640_SetExtMasterCalibResult() functions can be used. This will
 * force the board to calibrate near the calibration master board result.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Init(), Radio640_Open(), Radio640_Connect()
 *
 * @see
 *    Radio640_Init(), Radio640_Open(), Radio640_Connect(),
 *    Radio640_SetMasterCalibResult(), Radio640_SetExtMasterCalibResult()
 *
 ***************************************************************************/
Result Radio640_DigitalCalibration(Radio640_hRadio640 hRadio640);


/***************************************************************************
 * Function: Radio640_Write
 ************************************************************************//**
 *
 * Direct write transaction to a Radio640 device.
 *
 * Not all devices are writable.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Device
 *    The targeted device of the write operation
 *
 * @param [in] u16Addr
 *    Register address of the write operation.
 *    Some devices do not have internal memory space.
 *    If so, this parameter is ignored.
 *
 * @param [in] u32Data
 *    Value to write to the specified device and register address.
 *    Every device, except the "DAC Oscillator", have 8-bit transaction
 *    so only the 8 least significant bits of the u32Data will be transfered
 *    The "DAC Oscillator" device has 16-bit transaction.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect()
 *
 ***************************************************************************/
Result Radio640_Write(Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device, uint16_t u16Addr, uint32_t u32Data);

/***************************************************************************
 * Function: Radio640_Read
 ************************************************************************//**
 *
 * Direct read transaction to a Radio640 device.
 *
 * Not all devices are readable.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Device
 *    The targeted device of the read operation
 *
 * @param [in] u16Addr
 *    Register address of the read operation.
 *    Some devices do not have internal memory space.
 *    If so, this parameter is ignored.
 *
 * @param [in] u32Data
 *    Value read from the specified device and register address.
 *    Every device, except the "DAC Oscillator", have 8-bit transaction
 *    so only the 8 least significant bits of the u32Data will be updated.
 *    The "DAC Oscillator" device has 16-bit transaction.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect()
 *
 ***************************************************************************/
Result Radio640_Read(Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device, uint16_t u16Addr, uint32_t * pu32Data);


/***************************************************************************
 * Function: Radio640_IsClockMaster
 ************************************************************************//**
 *
 * Get the information if the Radio640 FPGA core is configured in clock master
 * or clock slave configuration
 *
 * In clock master configuration, the FPGA core uses its own Serial, Design
 * and Reference clocks.
 *
 * In clock slave configuration, the FPGA core uses the Serial, Design
 * and Reference clocks coming from a clock master Radio640 core. Every logic
 * inside the core is clock by the clocks of the clock master core. This
 * is done in order to have a single design clock when interfacing several 
 * Radio640 FMC boards to a common FPGA. In order for the clock slave core
 * to be functional, the Radio640 board must share a common reference with 
 * the clock master Radio640 board. Also, both baseband PLL (BBPLL) must be
 * in the same configuration.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8ClockMaster
 *    Pointer that will contain the FPGA core configuration.
 *    0 : Clock slave
 *    1 : Clock master
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect()
 *
 ***************************************************************************/
Result Radio640_IsClockMaster(Radio640_hRadio640 hRadio640, uint8_t * pu8ClockMaster);

 /***************************************************************************
 * Function: Radio640_SetSyncOutputDelay
 ************************************************************************//**
 *
 * Set the output analog delay of the Sync signal that is going out of the FPGA
 * to the SYNC pin of the AD9361 chip.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] i32Delay
 *    Delay from 0 to 31. Each step correspond to 73 ps.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetSyncOutputDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay);

 /***************************************************************************
 * Function: Radio640_SetSyncInputDelay
 ************************************************************************//**
 *
 * Set the input analog delay of the Sync signal that is coming from the
 * front-panel Sync connector that is going to the FPGA.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] i32Delay
 *    Delay from 0 to 31. Each step correspond to 73 ps.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetSyncInputDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay);

 /***************************************************************************
 * Function: Radio640_SetRefClkDelay
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
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] i32Delay
 *    Delay from 0 to 31. Each step correspond to 73 ps.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRefClkDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay);

 /***************************************************************************
 * Function: Radio640_SetSyncBitslip
 ************************************************************************//**
 *
 * Set the clock edge sensitivity of the Sync signal that is going out of the FPGA
 * to the SYNC pin of the AD9361 chip, relative to the reference clock.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] i32Bitslip
 *    0: The Sync signal is synchronous to the rising edge of the reference clock
 *    1: The Sync signal is synchronous to the falling edge of the reference clock
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetSyncBitslip(Radio640_hRadio640 hRadio640, int32_t i32Bitslip);

 /***************************************************************************
 * Function: Radio640_SetSyncSrc
 ************************************************************************//**
 *
 * Select the source of the Sync signal that goes out of the FPGA
 * to the SYNC pin of the AD9361 chip.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Src
 *    eRadio640SyncSrcSoft : Software sync signal that is toggled by 
 *      the Radio640_SoftSync function.
 *
 *    eRadio640SyncSrcInputSyncOnRisingEdge : External input sync signal
 *      from the front-panel Sync connector. The sync signal will be latched
 *      on reference clock rising-edge.
 *
 *    eRadio640SyncSrcInputSyncOnFallingEdge : External input sync signal
 *      from the front-panel Sync connector. The sync signal will be latched
 *      on reference clock falling-edge.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetSyncSrc(Radio640_hRadio640 hRadio640, Radio640_eSyncSrc Src);

 /***************************************************************************
 * Function: Radio640_SoftSync
 ************************************************************************//**
 *
 * Toggle the software sync signal from low to high and set it back to low.
 *
 * In order for the software sync to be outputted to the AD9361 chip,
 * the source of the sync signal should be set to 0 with the
 * Radio640_SetSyncSrc() function call.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_SetSyncSrc()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_SetSyncSrc()
 *
 ***************************************************************************/
Result Radio640_SoftSync(Radio640_hRadio640 hRadio640);

 /***************************************************************************
 * Function: Radio640_Mcs
 ************************************************************************//**
 *
 * Set the AD9361 chip in the different states required in order to do
 * multi chip synchronization.
 *
 * A Sync rising-edge event must have happened between steps 1 and 2, and between
 * steps 2 and 3.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] step
 *    0: Disable sensitivity to Sync signal.
 *    1: BBPLL will be synchronized on the next Sync signal rising-edge.
 *    2: Verify if the sync from step 1 was sent and
 *       BB dividers will be synchronized on the next Sync signal rising-edge.
 *    3: Verify if the sync from step 1 was sent and
 *       disable sensitivity to Sync signal.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_Mcs(Radio640_hRadio640 hRadio640, int32_t step);

 /***************************************************************************
 * Function: Radio640_SetMasterCalibResult
 ************************************************************************//**
 *
 * In order to make sure all board digital calibrations converge to the same
 * result, one board is designed as the calibration master. The calibration
 * result of the master is then passed to the other board in order for them
 * to converge to the same result than the master.
 *
 * The master calibration should have succeeded before calling
 * Radio640_DigitalCalibration() function on the slave board.
 * However, Radio640_SetMasterCalibResult() function can be called before
 * the master board is calibrated.
 *
 * Since this function used the Radio640 handle of the master board as
 * argument, its scope is limited to board on the same carrier board.
 * If two Radio640 board are located on different carriers, use
 * Radio640_SetExtMasterCalibResult() function instead.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] hMasterRadio640
 *    Radio640 handle of the master board.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_DigitalCalibration(), Radio640_SetExtMasterCalibResult()
 *
 ***************************************************************************/
Result Radio640_SetMasterCalibResult(Radio640_hRadio640 hRadio640, Radio640_hRadio640 hMasterRadio640);

 /***************************************************************************
 * Function: Radio640_SetExtMasterCalibResult
 ************************************************************************//**
 *
 * In order to make sure all board digital calibrations converge to the same
 * result, one board is designed as the calibration master. The calibration
 * result of the master is then passed to the other board in order for them
 * to converge to the same result than the master.
 *
 * The master calibration should have succeeded and the result retrieved before
 * calling Radio640_SetExtMasterCalibResult() function on the slave board.
 *
 * Since this function used the calibration result structure of the master board as
 * argument, it should be retrieved from the master board first using the
 * Radio640_GetCalibResult() function.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] ExtCalibResult
 *    Structure containing the master board calibration result.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_GetCalibResult()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_GetCalibResult()
 *
 ***************************************************************************/
Result Radio640_SetExtMasterCalibResult(Radio640_hRadio640 hRadio640, Ad9361_stCalibResult ExtCalibResult);

 /***************************************************************************
 * Function: Radio640_GetCalibResult
 ************************************************************************//**
 *
 * Once the Radio640_DigitalCalibration() function is called and has succeeded,
 * this function can be called to retrieve the calibration result.
 *
 * The calibration result information is required when synchronizing multiple
 * RADIO640 boards on different carriers with the
 * Radio640_SetExtMasterCalibResult() function.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pCalibResult
 *    Ad9361_stCalibResult pointer. The function will copy the current
 *    calibration result content in the structure pointed by pCalibResult.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_DigitalCalibration()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_DigitalCalibration(), Radio640_SetExtMasterCalibResult()
 *
 ***************************************************************************/
Result Radio640_GetCalibResult(Radio640_hRadio640 hRadio640, Ad9361_stCalibResult * pCalibResult);

 /***************************************************************************
 * Function: Radio640_SetEnsmMode
 ************************************************************************//**
 *
 * Set the desired Enable State Machine (ENSM) mode.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Mode
 *    The desired ENSM mode.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetEnsmMode(Radio640_hRadio640 hRadio640, Ad9361_eEnsmMode Mode);

 /***************************************************************************
 * Function: Radio640_GetEnsmMode
 ************************************************************************//**
 *
 * Get the current Enable State Machine (ENSM) mode.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu32Mode
 *    Pointer that will contain the current ENSM mode.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetEnsmMode(Radio640_hRadio640 hRadio640, Ad9361_eEnsmMode * pMode);

 /***************************************************************************
 * Function: Radio640_GetEnsmState
 ************************************************************************//**
 *
 * Get the current Enable State Machine (ENSM) state.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pEnsmState
 *    Pointer that will contain the current ENSM state.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetEnsmState (Radio640_hRadio640 hRadio640, Ad9361_eEnsmState * pEnsmState);

 /***************************************************************************
 * Function: Radio640_SetSamplingFreq
 ************************************************************************//**
 *
 * Calculate and apply the RX and TX clock chain configuration based on
 * the requested sampling frequency and the current FIR configurations.
 *
 * This function has the advantage of being simple to use but for
 * more control of the clock chain, it is recommended to use the
 * Radio640_SetTrxPathClks() function.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u32SamplingFreq
 *    Desired RX and TX frequency in Hz.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_SetTrxPathClks()
 *
 ***************************************************************************/
Result Radio640_SetSamplingFreq(Radio640_hRadio640 hRadio640, uint32_t u32SamplingFreq);

 /***************************************************************************
 * Function: Radio640_GetSamplingFreq
 ************************************************************************//**
 *
 * Get the current RX and TX sampling frequency.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu32SamplingFreq
 *    Pointer that will contain the current RX and TX frequency in Hz.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetSamplingFreq(Radio640_hRadio640 hRadio640, uint32_t * pu32SamplingFreq);

 /***************************************************************************
 * Function: Radio640_SetTrxPathClks
 ************************************************************************//**
 *
 * Set the RX and TX clock chains.
 *
 * It is recommended to design the clock chains using the AD9361 filter design
 * wizard MATLAB apps provided by Analog Devices.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pu32RxPathClks
 *    Array containing the 6 clock frequency values in Hz for the RX path.
 *
 * @param [in] pu32TxPathClks
 *    Array containing the 6 clock frequency values in Hz for the TX path.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTrxPathClks(Radio640_hRadio640 hRadio640, uint32_t *pu32RxPathClks, uint32_t *pu32TxPathClks);

 /***************************************************************************
 * Function: Radio640_GetTrxPathClks
 ************************************************************************//**
 *
 * Get the current RX and TX clock chains.
 *
 * It is recommended to design the clock chains using the AD9361 filter design
 * wizard MATLAB apps provided by Analog Devices.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu32RxPathClks
 *    Array that will be updated with the 6 clock frequency values of the current RX path.
 *
 * @param [out] pu32TxPathClks
 *    Array that will be updated with the 6 clock frequency values of the current TX path.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTrxPathClks(Radio640_hRadio640 hRadio640, uint32_t *pu32RxPathClks, uint32_t *pu32TxPathClks);

 /***************************************************************************
 * Function: Radio640_SetTxLoFreq
 ************************************************************************//**
 *
 * Set the TX local oscillator frequency.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u64LoFreq
 *    TX local oscillator frequency in Hz.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxLoFreq(Radio640_hRadio640 hRadio640, uint64_t u64LoFreq);

 /***************************************************************************
 * Function: Radio640_GetTxLoFreq
 ************************************************************************//**
 *
 * Get the TX local oscillator frequency.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu64LoFreq
 *    Pointer that will contain the TX local oscillator frequency in Hz.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTxLoFreq(Radio640_hRadio640 hRadio640, uint64_t * pu64LoFreq);

 /***************************************************************************
 * Function: Radio640_SetRxLoFreq
 ************************************************************************//**
 *
 * Set the RX local oscillator frequency.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u64LoFreq
 *    RX local oscillator frequency in Hz.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxLoFreq(Radio640_hRadio640 hRadio640, uint64_t u64LoFreq);

 /***************************************************************************
 * Function: Radio640_GetRxLoFreq
 ************************************************************************//**
 *
 * Get the RX local oscillator frequency.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu64LoFreq
 *    Pointer that will contain the RX local oscillator frequency in Hz.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxLoFreq(Radio640_hRadio640 hRadio640, uint64_t * pu64LoFreq);

 /***************************************************************************
 * Function: Radio640_SetTxAttenuation
 ************************************************************************//**
 *
 * Set the transmit attenuation for the selected channel.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    Channel to apply the specified TX attenuation [1,2].
 *
 * @param [in] u32Attenuation
 *    Attenuation in milli dB to apply.
 *    Valid values are from 0 to 89750 mdB. The values is floored to the previous
 *    250 mdB step.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxAttenuation(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint32_t u32Attenuation);

 /***************************************************************************
 * Function: Radio640_SetTxExtAttenuation
 ************************************************************************//**
 *
 * Set the transmit attenuation of the device (PE43705) external to the AD9361
 * device for the selected channel.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Channel
 *    Specify to which TX channels to apply the specified attenuation value.
 *
 * @param [in] u32Attenuation
 *    Attenuation in milli dB to apply.
 *    Valid values are from 0 to 31750 mdB. The values is floored to the previous
 *    250 mdB step.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxExtAttenuation(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint32_t u32Attenuation);

 /***************************************************************************
 * Function: Radio640_GetTxAttenuation
 ************************************************************************//**
 *
 * Get the transmit attenuation for the selected channel.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    Channel to retrieve the TX attenuation [1,2].
 *
 * @param [out] pu32Attenuation
 *    Pointer that will contain the current TX attenuation in milli dB.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTxAttenuation(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint32_t * pu32Attenuation);

 /***************************************************************************
 * Function: Radio640_SetRxGainControlMode
 ************************************************************************//**
 *
 * Set the gain control mode for the selected channel.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    Channel to apply the specified mode [1,2].
 *
 * @param [in] u8GcMode
 *    RX gain control mode.
 *    [RF_GAIN_MGC,RF_GAIN_FASTATTACK_AGC,
 *    RF_GAIN_SLOWATTACK_AGC,RF_GAIN_HYBRID_AGC]
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxGainControlMode(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8GcMode);

 /***************************************************************************
 * Function: Radio640_GetRxGainControlMode
 ************************************************************************//**
 *
 * Get the gain control mode for the selected channel.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    Channel to retrieve the mode [1,2].
 *
 * @param [in] pu8GcMode
 *    Pointer that will contain the current RX gain control mode.
 *    [RF_GAIN_MGC,RF_GAIN_FASTATTACK_AGC,
 *    RF_GAIN_SLOWATTACK_AGC,RF_GAIN_HYBRID_AGC]
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxGainControlMode(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t * pu8GcMode);

 /***************************************************************************
 * Function: Radio640_SetRxRfGain
 ************************************************************************//**
 *
 * Set the receive RF gain for the selected channel.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    Channel to apply the specified RX gain [1,2].
 *
 * @param [in] i32Gain
 *    RX gain in dB. The valid values can range from -10 to 77 dB and it depends on
 *    the RX LO frequency:
 *
 *      From  70 MHz to 1.3 GHz :   1 dB to 77 dB
 *      From 1.3 GHz to 4.0 GHz :  -4 dB to 71 dB
 *      From 4.0 MHz to 6.0 GHz : -10 dB to 62 dB
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxRfGain(Radio640_hRadio640 hRadio640, uint8_t u8Channel, int32_t i32Gain);

 /***************************************************************************
 * Function: Radio640_GetRxRfGain
 ************************************************************************//**
 *
 * Get the receive RF gain for the selected channel.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    Channel to retrieve the current RX gain [1,2].
 *
 * @param [in] pi32Gain
 *    Pointer that will contain the current RX gain in dB.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxRfGain(Radio640_hRadio640 hRadio640, uint8_t u8Channel, int32_t * pi32Gain);

 /***************************************************************************
 * Function: Radio640_SetTxRfBandwidth
 ************************************************************************//**
 *
 * Set the TX RF bandwidth.
 *
 * It is recommended to calculate the bandwidth value using the AD9361 
 * Filter Design Wizard App provided by Analog Devices since it takes into
 * account every filter stage of the AD9361 chip.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u32Bandwidth
 *    Bandwidth to apply to the TX path.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxRfBandwidth(Radio640_hRadio640 hRadio640, uint32_t u32Bandwidth);

 /***************************************************************************
 * Function: Radio640_GetTxRfBandwidth
 ************************************************************************//**
 *
 * Get the TX RF bandwidth.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pu32Bandwidth
 *    Pointer that will contain the current TX bandwidth.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTxRfBandwidth(Radio640_hRadio640 hRadio640, uint32_t * pu32Bandwidth);

 /***************************************************************************
 * Function: Radio640_SetRxRfBandwidth
 ************************************************************************//**
 *
 * Set the RX RF bandwidth.
 *
 * It is recommended to calculate the bandwidth value using the AD9361 
 * Filter Design Wizard App provided by Analog Devices since it takes into
 * account every filter stage of the AD9361 chip.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u32Bandwidth
 *    Bandwidth to apply to the RX path.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxRfBandwidth(Radio640_hRadio640 hRadio640, uint32_t u32Bandwidth);

 /***************************************************************************
 * Function: Radio640_GetRxRfBandwidth
 ************************************************************************//**
 *
 * Get the RX RF bandwidth.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pu32Bandwidth
 *    Pointer that will contain the current RX bandwidth.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxRfBandwidth(Radio640_hRadio640 hRadio640, uint32_t * pu32Bandwidth);

 /***************************************************************************
 * Function: Radio640_SetRxFirConfig
 ************************************************************************//**
 *
 * Set the RX FIR filter configuration
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Config
 *    Ad9361Api_stRxFirConfig structure containing the desired RX FIR configuration.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxFirConfig(Radio640_hRadio640 hRadio640, Ad9361Api_stRxFirConfig Config);

 /***************************************************************************
 * Function: Radio640_GetRxFirConfig
 ************************************************************************//**
 *
 * Get the RX FIR filter configuration
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] rx_ch
 *    Rx channel (1 or 2). 
 *
 * @param [out] pConfig
 *    Pointer to a Ad9361Api_stRxFirConfig structure that will contain the
 *    current RX FIR configuration.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxFirConfig(Radio640_hRadio640 hRadio640, uint8_t rx_ch, Ad9361Api_stRxFirConfig * pConfig);

 /***************************************************************************
 * Function: Radio640_SetTxFirConfig
 ************************************************************************//**
 *
 * Set the TX FIR filter configuration
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Config
 *    Ad9361Api_stTxFirConfig structure containing the desired TX FIR configuration.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxFirConfig(Radio640_hRadio640 hRadio640, Ad9361Api_stTxFirConfig Config);

 /***************************************************************************
 * Function: Radio640_GetTxFirConfig
 ************************************************************************//**
 *
 * Get the TX FIR filter configuration
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] tx_ch
 *    Tx channel (1 or 2).
 *
 * @param [out] pConfig
 *    Pointer to a Ad9361Api_stTxFirConfig structure that will contain the
 *    current TX FIR configuration.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTxFirConfig(Radio640_hRadio640 hRadio640, uint8_t tx_ch, Ad9361Api_stTxFirConfig * pConfig);

 /***************************************************************************
 * Function: Radio640_SetRxFirEnDis
 ************************************************************************//**
 *
 * Enable/disable the RX FIR filter.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0: Disable RX FIR filter
 *    1: Enable RX FIR filter
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxFirEnDis(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

 /***************************************************************************
 * Function: Radio640_GetRxFirEnDis
 ************************************************************************//**
 *
 * Get current RX FIR filter enable state.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    Pointer that will hold the current RX filter enable state.
 *    0: RX FIR filter is disabled
 *    1: RX FIR filter is enabled
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxFirEnDis(Radio640_hRadio640 hRadio640, uint8_t *u8Enable);

 /***************************************************************************
 * Function: Radio640_SetTxFirEnDis
 ************************************************************************//**
 *
 * Enable/disable the TX FIR filter.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0: Disable TX FIR filter
 *    1: Enable TX FIR filter
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxFirEnDis(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

 /***************************************************************************
 * Function: Radio640_GetTxFirEnDis
 ************************************************************************//**
 *
 * Get current TX FIR filter enable state.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    Pointer that will hold the current TX filter enable state.
 *    0: TX FIR filter is disabled
 *    1: TX FIR filter is enabled
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTxFirEnDis(Radio640_hRadio640 hRadio640, uint8_t *u8Enable);

 /***************************************************************************
 * Function: Radio640_GetRxRssi
 ************************************************************************//**
 *
 * Get the Received Signal Strength Indicator (RSSI) for the selected channel
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    Channel to retrieve the RSSI [1,2].
 *
 * @param [out] pRfRssi
 *    Pointer on a Ad9361_stRfRssi structure that will hold the RSSI of the
 *    selected channel.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxRssi(Radio640_hRadio640 hRadio640, uint8_t u8Channel, struct Ad9361_stRfRssi * pRfRssi);

 /***************************************************************************
 * Function: Radio640_SetRxRfdcTrackEnDis
 ************************************************************************//**
 *
 * Enable/disable the RX RFDC Tracking.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0: Disable RX RFDC Tracking
 *    1: Enable RX RFDC Tracking
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxRfdcTrackEnDis(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

 /***************************************************************************
 * Function: Radio640_GetRxRfdcTrackEnDis
 ************************************************************************//**
 *
 * Get the status of the RX RFDC Tracking.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] u8Enable
 *    0: RX RFDC Tracking is disabled
 *    1: RX RFDC Tracking is enabled
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxRfdcTrackEnDis(Radio640_hRadio640 hRadio640, uint8_t *u8Enable);

 /***************************************************************************
 * Function: Radio640_SetRxBbdcTrackEnDis
 ************************************************************************//**
 *
 * Enable/disable the RX Baseband DC Tracking.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0: Disable RX Baseband DC Tracking
 *    1: Enable RX Baseband DC Tracking
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxBbdcTrackEnDis(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

 /***************************************************************************
 * Function: Radio640_GetRxBbdcTrackEnDis
 ************************************************************************//**
 *
 * Get the status of the RX Baseband DC Tracking.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] u8Enable
 *    0: RX Baseband DC Tracking is disabled
 *    1: RX Baseband DC Tracking is enabled
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxBbdcTrackEnDis(Radio640_hRadio640 hRadio640, uint8_t *u8Enable);

 /***************************************************************************
 * Function: Radio640_SetRxQuadTrackEnDis
 ************************************************************************//**
 *
 * Enable/disable the RX Quadrature Tracking.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0: Disable RX Quadrature Tracking
 *    1: Enable RX Quadrature Tracking
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxQuadTrackEnDis(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

 /***************************************************************************
 * Function: Radio640_GetRxQuadTrackEnDis
 ************************************************************************//**
 *
 * Get the status of the RX Quadrature Tracking.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] u8Enable
 *    0: RX Quadrature Tracking is disabled
 *    1: RX Quadrature Tracking is enabled
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetRxQuadTrackEnDis(Radio640_hRadio640 hRadio640, uint8_t *u8Enable);

/***************************************************************************
 * Function: Radio640_DisableDesignClk
 ************************************************************************//**
 *
 * Disable the FPGA design clock. This function can be used when the user know
 * that its action will make the design clock instable (ex.: Reference source or
 * BBPLL frequency change) and he does not want to have an unstable design clock
 * that propagates to its logic (which can corrupt their state).
 *
 * This function is automatically called when using the Radio640 API function.
 *
 * It should only be called when using direct register write or when the
 * reference clock changed.
 *
 * Use Radio640_EnableDesignClk() the enable back the design clock once the
 * BBPLL has became stable again.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_EnableDesignClk()
 *
 ***************************************************************************/
Result Radio640_DisableDesignClk(Radio640_hRadio640 hRadio640);

/***************************************************************************
 * Function: Radio640_EnableDesignClk
 ************************************************************************//**
 *
 * Enable the FPGA design clock. This function must be called to enable back
 * the design clock that has been previously disable by
 * Radio640_DisableDesignClk().
 *
 * The design clock must only be enable once the BBPLL has became stable again.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_DisableDesignClk()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_DisableDesignClk()
 *
 ***************************************************************************/
Result Radio640_EnableDesignClk(Radio640_hRadio640 hRadio640);

 /***************************************************************************
 * Function: Radio640_TuneOsc
 ************************************************************************//**
 *
 * Tune the on-board oscillator frequency. 
 *
 * This function is only relevant in eRadio640ClkSrcInt reference clock
 * source configuration. If not in eRadio640ClkSrcInt, the on-board oscillator
 * is unused and tuning its frequency has no effect.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] fValue
 *    Floating point value that represent the normalized tuning range
 *    of the oscillator from -1.0f to 1.0f.
 *    0.0f represent the default oscillator tuning value.
 *    The oscillator tuning range is approximately from -10 ppm to
 *    10 ppm.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_TuneOsc(Radio640_hRadio640 hRadio640, float fValue);


 /***************************************************************************
 * Function: Radio640_SetClkSrc
 ************************************************************************//**
 *
 * Change the reference clock source of the Radio640.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] ClkSrc
 *    Reference clock source.
 *
 *    eRadio640ClkSrcExt : 
 *      Reference clock source from external front-panel RefIn connector.
 *
 *    eRadio640ClkSrcInt :
 *      Reference clock source from internal on-board oscillator.
 *
 *    eRadio640ClkSrcCarrier :
 *      Reference clock source from carrier CLK2_BIDIR FMC connector.
 *      The carrier must be configured to provided a differential clock
 *      to these FMC connector pins.
 *
 *    eRadio640ClkSrcMaster :
 *      Reference clock source from the extended FMC connector of an other
 *      Radio640 board that holds the current Radio640 board. This configuration
 *      is only relevant when the Radio640 board is in position 2 on a
 *      specific HPC FMC connector.
 *      In this configuration, the clock buffer is bypassed and only the
 *      AD9361 receives the reference clock. The reference clock is not forwarded
 *      to the FPGA and the board's front panel RefOut connector
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetClkSrc(Radio640_hRadio640 hRadio640, Radio640_eClkSrc ClkSrc);

 /***************************************************************************
 * Function: Radio640_GetClkSrc
 ************************************************************************//**
 *
 * Get the current reference clock source configuration.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pClkSrc
 *    Pointer that will hold the current reference clock source information
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetClkSrc(Radio640_hRadio640 hRadio640, Radio640_eClkSrc * pClkSrc);


 /***************************************************************************
 * Function: Radio640_GetBoardTemp
 ************************************************************************//**
 *
 * Get the temperature in Celsius from the on-board temperature sensorc
 * device.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pi16Temp
 *    Pointer that will hold the current board temperature in degree Celsius
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetBoardTemp(Radio640_hRadio640 hRadio640, int16_t *pi16Temp);

 /***************************************************************************
 * Function: Radio640_GetTrxTemp
 ************************************************************************//**
 *
 * Get the temperature in degree Celsius from the AD9361 internal temperature
 * sensor.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pi16Temp
 *    Pointer that will hold the current AD9361 temperature in degree Celsius
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTrxTemp(Radio640_hRadio640 hRadio640, int16_t *pi16Temp);


 /***************************************************************************
 * Function: Radio640_SetRfSwitchConfig
 ************************************************************************//**
 *
 * Change the RF switch configuration of the Radio640.
 *
 * By default, it is the FPGA user port that controls the state of the RF 
 * switch. By using this function and setting the u8Force bits to '1',
 * the software can force a desired configuration and ignore the FPGA
 * user port values.
 *
 * The u8Force and u8TrxInRxDir are bit mask value (bit0 and bit1) to 
 * respectively control the RF switch configuration of the channel 1
 * and channel 2.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Force
 *    Select if it is the FPGA or Software that controls the RF switches.
 *    Allowed value : {0x0, 0x1, 0x2, 0x3}
 *
 *    Bit == 0  : The RF switch is controlled by the FPGA
 *                user port value.
 *
 *    Bit == 1  : The RF switch is controlled by the 
 *                corresponding u8TrxInRxDir bit value
 *
 * @param [in] u8TrxInRxDir
 *    Only bits corresponding to u8Force bits set to 1 are taken into account.
 *    Allowed value : {0x0, 0x1, 0x2, 0x3}
 *
 *    Bit == 0  : The AD9361 TX port is routed to the TRX front-panel connector.
 *                The AD9361 RX port is routed to the RX front-panel connector.
 *
 *    Bit == 1  : The AD9361 RX port is routed to the TRX front-panel connector.
 *                The AD9361 TX port is not routed to any connector and should be disabled.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRfSwitchConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8TrxInRxDir);

 /***************************************************************************
 * Function: Radio640_SetEnAgcPinConfig
 ************************************************************************//**
 *
 * Change the EN_AGC signal configuration to goes to the AD9361.
 *
 * By default, it is the FPGA user port that controls the state of the EN_AGC pin. 
 * By using this function and setting the u8Force to 1,
 * the software can force a desired value and ignore the FPGA
 * user port value.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Force
 *    Select if it is the FPGA or Software that controls the EN_AGC pin.
 *
 *    0  : The EN_AGC is controlled by the FPGA
 *                user port value.
 *
 *    1  : The EN_AGC is controlled by the 
 *                specified u8Value value
 *
 * @param [in] u8Value
 *    Desired EN_AGC state (0 or 1).
 *    This parameter is only taken into account when u8Force is 1.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetEnAgcPinConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8Value);

 /***************************************************************************
 * Function: Radio640_SetEnablePinConfig
 ************************************************************************//**
 *
 * Change the ENABLE signal configuration that goes to the AD9361.
 *
 * By default, it is the FPGA user port that controls the state of the ENABLE pin. 
 * By using this function and setting the u8Force to 1,
 * the software can force a desired value and ignore the FPGA
 * user port value.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Force
 *    Select if it is the FPGA or Software that controls the ENABLE pin.
 *
 *    0  : The ENABLE is controlled by the FPGA
 *                user port value.
 *
 *    1  : The ENABLE is controlled by the 
 *                specified u8Value value
 *
 * @param [in] u8Value
 *    Desired ENABLE state (0 or 1).
 *    This parameter is only taken into account when u8Force is 1.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetEnablePinConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8Value);

 /***************************************************************************
 * Function: Radio640_SetTxnrxPinConfig
 ************************************************************************//**
 *
 * Change the TXNRX signal configuration to goes to the AD9361.
 *
 * By default, it is the FPGA user port that controls the state of the TXNRX pin. 
 * By using this function and setting the u8Force to 1,
 * the software can force a desired value and ignore the FPGA
 * user port value.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Force
 *    Select if it is the FPGA or Software that controls the TXNRX pin.
 *
 *    0  : The TXNRX is controlled by the FPGA
 *                user port value.
 *
 *    1  : The TXNRX is controlled by the 
 *                specified u8Value value
 *
 * @param [in] u8Value
 *    Desired TXNRX state (0 or 1).
 *    This parameter is only taken into account when u8Force is 1.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxnrxPinConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8Value);

 /***************************************************************************
 * Function: Radio640_SetCtrlInPinConfig
 ************************************************************************//**
 *
 * Change the CTRL_IN signal configuration to goes to the AD9361.
 *
 * By default, it is the FPGA user port that controls the state of the 
 * CTRL_IN pins. By using this function and setting the u8Force bits to '1',
 * the software can force a desired configuration and ignore the FPGA
 * user port values.
 *
 * The u8Force and u8Value are bit mask value (bit0 to bit3) to 
 * respectively control the 4 CTRL_IN pins value.
 *
 * The 4 CTRL_IN pins are lanes driven by the FPGA that can controls
 * the AD9361 configuration. 
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Force
 *    Select if it is the FPGA or Software that controls the RF switches.
 *    Allowed value : {0x0 to 0xF}
 *
 *    Bit == 0  : The CTRL_IN pin is controlled by the FPGA
 *                user port value.
 *
 *    Bit == 1  : The CTRL_IN pin is controlled by the 
 *                corresponding u8Value bit value
 *
 * @param [in] u8Value
 *    Desired CTRL_IN pin values if controlled by software.
 *    Only bits corresponding to u8Force bits set to 1 are taken into account.
 *    Allowed value : {0x0 to 0xF}
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetCtrlInPinConfig(Radio640_hRadio640 hRadio640, uint8_t u8Force, uint8_t u8Value);

 /***************************************************************************
 * Function: Radio640_GetCtrlOutPinValue
 ************************************************************************//**
 *
 * Get the CTRL_OUT signal value from the AD9361.
 *
 * The 8 CTRL_OUT pins are lanes driven by the AD9361 that goes
 * to the FPGA to provide status on the AD9361 current configuration/state.
 *
 * The status provided by the CTRL_OUT pins can be chosen using the 
 * Radio640_SetCtrlOutConfig() function. 
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8Value
 *    Pointer that will hold the current 8 CTRL_OUT pins state.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_SetCtrlOutConfig()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_SetCtrlOutConfig()
 *
 ***************************************************************************/
Result Radio640_GetCtrlOutPinValue(Radio640_hRadio640 hRadio640, uint8_t * pu8Value);

 /***************************************************************************
 * Function: Radio640_GetCtrlOutPinLowHighValue
 ************************************************************************//**
 *
 * Get the lowest and highest CTRL_OUT pin value from the AD9361 from each 8
 * pins since the last call of this function. After calling this function,
 * the lowest and highest latched values are reset, so that new lowest and highest
 * states will be available at the next function call.
 *
 * This function can be used to detect short events that cannot be detected
 * even if the processor was continuously calling the Radio640_GetCtrlOutPinValue()
 * function. The CTRL_OUT pin states are continuously monitored at each
 * design clock period, so event as short as one design clock period can be 
 * detected (ex.: Unlocked, Overflow event).
 *
 * The 8 CTRL_OUT pins are lanes driven by the AD9361 that goes
 * to the FPGA to provide status on the AD9361 current configuration/state.
 *
 * The status provided by the CTRL_OUT pins can be chosen using the 
 * Radio640_SetCtrlOutConfig() function. 
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8LowValue
 *    Pointer that will hold the lowest state of each 8 CTRL_OUT pin since
 *    the last call of this function.
 *
 * @param [out] pu8HighValue
 *    Pointer that will hold the highest state of each 8 CTRL_OUT pin since
 *    the last call of this function.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_SetCtrlOutConfig()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_SetCtrlOutConfig(), Radio640_GetCtrlOutPinValue()
 *
 ***************************************************************************/
Result Radio640_GetCtrlOutPinLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue);

 /***************************************************************************
 * Function: Radio640_SetCtrlOutConfig
 ************************************************************************//**
 *
 * Set which statuses are provided by the CTRL_OUT pins.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Config
 *    Configuration of the CTRL_OUT pins.
 *    Allowed values from 0x00 to 0x1F.
 *    See "Control Output Table" of the "AD9361 Reference Manual"
 *    to have the complete list of all CTRL_OUT configuration
 *    available.
 *
 * @param [in] u8Enable
 *    Bit mask value that specifies whether the corresponding CTRL_OUT pin
 *    is enabled or not.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetCtrlOutConfig(Radio640_hRadio640 hRadio640, uint8_t u8Config, uint8_t u8Enable);

 /***************************************************************************
 * Function: Radio640_SetRxRfFilter
 ************************************************************************//**
 *
 * Select RX RF filter for the RX channel paths.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Channel
 *    Specify to which RX channels the specified filter will be applied.
 *
 * @param [in] Filter
 *    The enumeration specifying which filter will be applied.
 *    The Radio640 default filter are:
 *      eRadio640RxRfFilter2G45     (center:  2.45 GHz, 100 MHz wide)
 *      eRadio640RxRfFilter5G387    (center: 5.387 GHz, 975 MHz wide)
 *      eRadio640RxRfFilter3G6      (center:   3.6 GHz, 600 MHz wide)
 *      eRadio640RxRfFilterBypass   (DC to 6 GHz)
 * 
 *    If your Radio640 does not have the default filters, specify the filter
 *    index using integer value from 0 to 3 instead.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxRfFilter(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, Radio640_eRxRfFilter Filter);

 /***************************************************************************
 * Function: Radio640_SetTxEnableState
 ************************************************************************//**
 *
 * Enable or disable the specified TX channels.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Channel
 *    Specify to which TX channels the specified state will be applied to.
 *
 * @param [in] u8Enable
 *    0 : Disable all channels specified by Channel.
 *    1 : Enable all channels specified by Channel.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxEnableState(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint8_t u8Enable);

 /***************************************************************************
 * Function: Radio640_SetRxEnableState
 ************************************************************************//**
 *
 * Enable or disable the specified RX channels.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Channel
 *    Specify to which RX channels the specified state will be applied to.
 *
 * @param [in] u8Enable
 *    0 : Disable all channels specified by Channel.
 *    1 : Enable all channels specified by Channel.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxEnableState(Radio640_hRadio640 hRadio640, Radio640_eChannel Channel, uint8_t u8Enable);


 /***************************************************************************
 * Function: Radio640_SetTriggerDelay
 ************************************************************************//**
 *
 * Set the input analog delay of the Trigger signal that is coming from the
 * front-panel Trigger connector to FPGA user logic.
 *
 * The trigger signal is latched with the Radio640 design clock.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] i32Delay
 *    Delay from 0 to 31. Each step correspond to 73 ps.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTriggerDelay(Radio640_hRadio640 hRadio640, int32_t i32Delay);

 /***************************************************************************
 * Function: Radio640_SetTriggerBitslip
 ************************************************************************//**
 *
 * Set the bit slip of the Trigger signal that is coming from the
 * front-panel Trigger connector that is going to the FPGA. 
 *
 * The trigger signal is latched with the Radio640 design clock.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] i32Bitslip
 *    0 : The trigger value is latched on the Radio640 design clock rising-edge
 *    1 : The trigger value is latched on the Radio640 design clock falling-edge
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTriggerBitslip(Radio640_hRadio640 hRadio640, int32_t i32Bitslip);


 /***************************************************************************
 * Function: Radio640_GetTriggerValue
 ************************************************************************//**
 *
 * Get the current value of the Trigger signal that is coming from the
 * front-panel Trigger connector that is going to the FPGA.
 *
 * The trigger signal is latched with the Radio640 design clock.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8Value
 *    Pointer that will hold the current value of the trigger (0 or 1).
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTriggerValue(Radio640_hRadio640 hRadio640, uint8_t * pu8Value);

 /***************************************************************************
 * Function: Radio640_GetTriggerLowHighValue
 ************************************************************************//**
 *
 * Get the lowest and highest Trigger values that were latched since the last 
 * call of this function. After calling this function, the lowest and highest 
 * latched values are reset, so that new lowest and highest states will be 
 * available at the next function call.
 *
 * The trigger signal is latched with the Radio640 design clock.
 *
 * This function can be used to detect short events that cannot be detected
 * even if the processor was continuously calling the Radio640_GetTriggerValue()
 * function. The trigger state is continuously monitored at each
 * design clock period, so event as short as one design clock period can be 
 * detected.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8LowValue
 *    Pointer that will hold the lowest (0 or 1) trigger value since
 *    the last call of this function.
 *
 * @param [out] pu8HighValue
 *    Pointer that will hold the highest (0 or 1) trigger value since
 *    the last call of this function.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_GetTriggerValue()
 *
 ***************************************************************************/
Result Radio640_GetTriggerLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue);

 /***************************************************************************
 * Function: Radio640_GetInputSyncValue
 ************************************************************************//**
 *
 * Get the current value of the Sync signal that is coming from the
 * front-panel Sync connector that is going to the FPGA.
 *
 * The Sync signal is latched with the reference clock.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8Value
 *    Pointer that will hold the current value of the sync (0 or 1).
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetInputSyncValue(Radio640_hRadio640 hRadio640, uint8_t * pu8Value);

 /***************************************************************************
 * Function: Radio640_GetInputSyncLowHighValue
 ************************************************************************//**
 *
 * Get the lowest and highest Sync value that is coming from the
 * front-panel Sync connector that is going to the FPGA since the last 
 * call of this function. After calling this function, the lowest and highest 
 * latched values are reseted, so new lowest and highest states will be 
 * available at the next function call.
 *
 * The sync signal is latched with the reference clock.
 *
 * This function can be used to detect short events that cannot be detected
 * even if the processor was continuously calling the Radio640_GetInputSyncValue()
 * function. The sync state is continuously monitored at each
 * reference clock period, so event as small as one reference clock period can be 
 * detected.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8LowValue
 *    Pointer that will hold the lowest (0 or 1) sync value since
 *    the last call of this function.
 *
 * @param [out] pu8HighValue
 *    Pointer that will hold the highest (0 or 1) sync value since
 *    the last call of this function.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init(), Radio640_GetInputSyncValue()
 *
 ***************************************************************************/
Result Radio640_GetInputSyncLowHighValue(Radio640_hRadio640 hRadio640, uint8_t * pu8LowValue, uint8_t * pu8HighValue);

 /***************************************************************************
 * Function: Radio640_ReadClkFreq
 ************************************************************************//**
 *
 * Read the frequency of the specified clock. This value is counted based
 * on the carrier on-board oscillator. The values is updated each second.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Clk
 *    The clock enumeration to specify from which clock to get 
 *    the frequency value:
 *      eRadio640TestClkSerial
 *      eRadio640TestClkDesign
 *      eRadio640TestClkRef
 *      eRadio640TestClkAd9361ClkOut
 *
 * @param [out] pu32Freq
 *    Pointer that will hold the frequency in Hz.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 * @see
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_ReadClkFreq(Radio640_hRadio640 hRadio640, Radio640_eTestClk Clk, uint32_t * pu32Freq);

 /***************************************************************************
 * Function: Radio640_SetRxLoExt
 ************************************************************************//**
 *
 * Set the RX local oscillator (LO) to come from the external front-panel
 * RX LO connector. A clock at twice (x2) the desired LO frequency must be
 * provided
 *
 * The available LO frequencies are from 70 MHz to 4 GHz 
 * (external clock from 140 MHz to 8 GHz).
 *
 * The provided clock power must be between -3 dBm and 3 dBm.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0 : Use internal synthesizer to generate the LO from the reference clock
 *    1 : Use the external RX LO connector to generate a LO at half 
 *        of the provided clock frequency.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRxLoExt(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

 /***************************************************************************
 * Function: Radio640_SetTxLoExt
 ************************************************************************//**
 *
 * Set the TX local oscillator (LO) to come from the external front-panel
 * TX LO connector. A clock at twice (x2) the desired LO frequency must be
 * provided
 *
 * The available LO frequencies are from 70 MHz to 4 GHz 
 * (external clock from 140 MHz to 8 GHz).
 *
 * The provided clock power must be between -3 dBm and 3 dBm.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0 : Use internal synthesizer to generate the LO from the reference clock
 *    1 : Use the external TX LO connector to generate a LO at half 
 *        of the provided clock frequency.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxLoExt(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

/***************************************************************************
 * Function: Radio640_SetTxAutoCalibEnable
 ************************************************************************//**
 *
 * Enable or disable the TX automatic calibration
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0 : Disable TX automatic calibration
 *    1 : Enable TX automatic calibration
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetTxAutoCalibEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

/***************************************************************************
 * Function: Radio640_GetTxAutoCalibEnable
 ************************************************************************//**
 *
 * Get the current TX automatic calibration enable state.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8Enable
 *    Pointer that will hold the current TX automatic calibration enable state.
 *    0 : Disable TX automatic calibration
 *    1 : Enable TX automatic calibration
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetTxAutoCalibEnable(Radio640_hRadio640 hRadio640, uint8_t * pu8Enable);

/***************************************************************************
 * Function: Radio640_DoCalib
 ************************************************************************//**
 *
 * Force the TX quadrature calibration or the RX RF DC offset calibration
 * to be performed.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Calib
 *    The only calibrations available are eRadio640CalibTxQuad 
 *    and eRadio640CalibRxRfDcOffset.
 *
 * @param [in] i32Value
 *    Only used by eRadio640CalibTxQuad.
 *    Force RX phase value if the value is higher or equal to 0.
 *    A value of -1 mean automatic RX phase.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_DoCalib(Radio640_hRadio640 hRadio640, Radio640_eCalib Calib, int32_t i32Value);


/***************************************************************************
 * Function: Radio640_StoreRxFastLockProfile
 ************************************************************************//**
 *
 * Save the current RX LO frequency configuration and calibration
 * into the specified profile index.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Profile
 *    Profile index, from 0 to 7.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_StoreRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile);

/***************************************************************************
 * Function: Radio640_RecallRxFastLockProfile
 ************************************************************************//**
 *
 * Restore a RX LO frequency configuration and calibration previously stored
 * at specified profile index.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Profile
 *    Profile index, from 0 to 7.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_RecallRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile);

/***************************************************************************
 * Function: Radio640_LoadRxFastLockProfile
 ************************************************************************//**
 *
 * Save a RX LO frequency configuration and calibration
 * into the specified profile index from a set of 16 8-bit values.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pu8Values
 *    Array of 16 8-bit values specifying the desired profile configuration.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_LoadRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values);

/***************************************************************************
 * Function: Radio640_SaveRxFastLockProfile
 ************************************************************************//**
 *
 * Retrieve a RX LO frequency configuration and calibration
 * saved into the specified profile index.
 * 
 * The retrieved configuration will be saved in the array of 16 8-bit value
 * provided.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8Values
 *    Array of 16 8-bit values that will hold the retrieved profile configuration.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SaveRxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values);

/***************************************************************************
 * Function: Radio640_StoreTxFastLockProfile
 ************************************************************************//**
 *
 * Save the current RX LO frequency configuration and calibration
 * into the specified profile index.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Profile
 *    Profile index, from 0 to 7.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_StoreTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile);

/***************************************************************************
 * Function: Radio640_RecallTxFastLockProfile
 ************************************************************************//**
 *
 * Restore a TX LO frequency configuration and calibration previously stored
 * at specified profile index.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Profile
 *    Profile index, from 0 to 7.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_RecallTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile);

/***************************************************************************
 * Function: Radio640_LoadTxFastLockProfile
 ************************************************************************//**
 *
 * Save a TX LO frequency configuration and calibration
 * into the specified profile index from a set of 16 8-bit values.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] pu8Values
 *    Array of 16 8-bit values specifying the desired profile configuration.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_LoadTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values);

/***************************************************************************
 * Function: Radio640_SaveTxFastLockProfile
 ************************************************************************//**
 *
 * Retrieve a TX LO frequency configuration and calibration
 * saved into the specified profile index.
 * 
 * The retrieved configuration will be save in the array of 16 8-bit value
 * provided.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [out] pu8Values
 *    Array of 16 8-bit values that will hold the retrieved profile configuration.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SaveTxFastLockProfile(Radio640_hRadio640 hRadio640, uint8_t u8Profile, uint8_t * pu8Values);


/***************************************************************************
 * Function: Radio640_SetPowerMonitorMode
 ************************************************************************//**
 *
 * Set the specified device in the specified mode
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Device
 *    Available power monitor devices are :
 *      eRadio640DevicePower12V and
 *      eRadio640DevicePowerVadj.
 *
 * @param [in] Mode
 *    Available power monitor mode are :
 *      eIna226ModePowerDown
 *      eIna226ModeTrigger
 *      eIna226ModeContinuous
 *
 *    eIna226ModeTrigger means that the conversion process will be done
 *    only when a ReadPowerMonitor function will be called.
 *
 *    eIna226ModeContinuous means that the conversion process is perform
 *    continuously.
 *
 * @param [in] u8IsBlocking
 *    0 : The ReadPowerMonitor functions instantaneously return the current value.
 *    1 : The ReadPowerMonitor functions wait until a new unread value is available.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetPowerMonitorMode(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eMode Mode, uint8_t u8IsBlocking);
    
/***************************************************************************
 * Function: Radio640_SetPowerMonitorConversionTime
 ************************************************************************//**
 *
 * Set the power monitor conversion time.
 *
 * The total conversion time is the sum of both conversion times
 * multiplied by the averaging number.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Device
 *    Available power monitor devices are :
 *      eRadio640DevicePower12V and
 *      eRadio640DevicePowerVadj.
 *
 * @param [in] BusConvTime
 *    Enumeration (Ina226_eConvTime) specifying the conversion time of the bus voltage.
 *
 * @param [in] ShuntConvTime
 *    Enumeration (Ina226_eConvTime) specifying the conversion time of the shunt voltage.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetPowerMonitorConversionTime(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eConvTime BusConvTime, Ina226_eConvTime ShuntConvTime);
    
/***************************************************************************
 * Function: Radio640_SetPowerMonitorAveraging
 ************************************************************************//**
 *
 * Set the power monitor averaging number.
 *
 * The total conversion time is the sum of both conversion times
 * multiplied by the averaging number.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Device
 *    Available power monitor devices are :
 *      eRadio640DevicePower12V and
 *      eRadio640DevicePowerVadj.
 *
 * @param [in] Averaging
 *    Enumeration (Ina226_eAveraging) specifying the number of averaging done
 *    to perform a measurement.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetPowerMonitorAveraging(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    Ina226_eAveraging Averaging);
    
/***************************************************************************
 * Function: Radio640_SetPowerMonitorAveraging
 ************************************************************************//**
 *
 * Read the bus voltage measured by the power monitor device.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Device
 *    Available power monitor devices are :
 *      eRadio640DevicePower12V and
 *      eRadio640DevicePowerVadj.
 *
 * @param [in] pfVoltage
 *    Pointer that will hold the measured voltage in Volt.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_ReadPowerMonitorBusVoltage(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfVoltage);
    
/***************************************************************************
 * Function: Radio640_ReadPowerMonitorCurrent
 ************************************************************************//**
 *
 * Read the current measured by the power monitor device.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Device
 *    Available power monitor devices are :
 *      eRadio640DevicePower12V and
 *      eRadio640DevicePowerVadj.
 *
 * @param [in] pfCurrent
 *    Pointer that will hold the measured current in Ampere.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_ReadPowerMonitorCurrent(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfCurrent);
    
/***************************************************************************
 * Function: Radio640_ReadPowerMonitorPower
 ************************************************************************//**
 *
 * Read the power measured by the power monitor device.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] Device
 *    Available power monitor devices are :
 *      eRadio640DevicePower12V and
 *      eRadio640DevicePowerVadj.
 *
 * @param [in] pfPower
 *    Pointer that will hold the measured power in Watt.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_ReadPowerMonitorPower(
    Radio640_hRadio640 hRadio640, Radio640DevCom_eDevices Device,
    float * pfPower);

/***************************************************************************
 * Function: Radio640_SetClkToExtendedFmcEnable
 ************************************************************************//**
 *
 * Enable or disable the clock that is going from a Radio640 board with
 * an extended FMC connector on top of it to another Radio640 board connected
 * on top of it.
 *
 * For the Radio640 board on top of the current Radio640 board, if its clock 
 * source is not configured in eRadio640ClkSrcMaster, the clock can be disable
 * using this function.
 *
 * This clock can only be enabled if the Radio640 board has an extended FMC
 * connector.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0 : Disable the clock
 *    1 : Enable the clock
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetClkToExtendedFmcEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

/***************************************************************************
 * Function: Radio640_SetRefOutEnable
 ************************************************************************//**
 *
 * Enable or disable the clock that is going to the RefOut front-panel
 * connector.
 *
 * By default this clock is disabled to avoid any spurs related to this clock
 * that is routed near the RX channel paths.
 *
 * If the reference output clock needs to be used, it can be enabled using
 * this function.
 *
 * This clock cannot be enabled if the clock source configuration is
 * eRadio640ClkSrcMaster, since the clock buffer is bypassed.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Enable
 *    0 : Disable the clock
 *    1 : Enable the clock
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetRefOutEnable(Radio640_hRadio640 hRadio640, uint8_t u8Enable);

/***************************************************************************
 * Function: Radio640_SetComplexMultiplier
 ************************************************************************//**
 *
 * Set a value to which the data coming out from of the ADC or coming in to the DAC
 * is multiplied. This is useful for compensating the LO phase relationships between
 * multiple devices.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    1 : Channel 1
 *    2 : Channel 2
 *
 * @param [in] u8Direction
 *    0 : Rx
 *    1 : Tx
 *
 * @param [in] u16RealCoefficient
 *    Real part of the coefficient. Given in unsigned hexadecimal value.
 * 
 * @param [in] u16ImgCoefficient
 *    Imaginary part of the coefficient. Given in unsigned hexadecimal value.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetComplexMultiplier(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint16_t u16RealCoefficient, uint16_t u16ImgCoefficient);

/***************************************************************************
 * Function: Radio640_GetComplexMultiplier
 ************************************************************************//**
 *
 * Get the value to which the data coming out from of the ADC or coming in to the DAC
 * is multiplied. 
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    1 : Channel 1
 *    2 : Channel 2
 *
 * @param [in] u8Direction
 *    0 : Rx
 *    1 : Tx
 *
 * @param [out] u16RealCoefficient
 *    Real part of the coefficient. Given in unsigned hexadecimal value.
 * 
 * @param [out] u16ImgCoefficient
 *    Imaginary part of the coefficient. Given in unsigned hexadecimal value.
 *
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetComplexMultiplier(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint16_t *u16RealCoefficient, uint16_t *u16ImgCoefficient);

/***************************************************************************
 * Function: Radio640_SetDataDelay
 ************************************************************************//**
 *
 * Set the delay to apply to the data coming out from of the ADC or coming in to the DAC.
 * This is useful for compensating delays between multiple devices.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    1 : Channel 1
 *    2 : Channel 2
 *
 * @param [in] u8Direction
 *    0 : Rx
 *    1 : Tx
 *
 * @param [in] u32DataDelay
 *    Delay to apply to the data. In the current FPGA core implementation, 
 *    valid delays are 0 to 3. 
 * 
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_SetDataDelay(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint32_t u32DataDelay);

/***************************************************************************
 * Function: Radio640_GetDataDelay
 ************************************************************************//**
 *
 * Get the delay applied to the data coming out from of the ADC or coming in to the DAC.
 *
 * @param [in] hRadio640
 *    Radio640 handle previously opened.
 *
 * @param [in] u8Channel
 *    1 : Channel 1
 *    2 : Channel 2
 *
 * @param [in] u8Direction
 *    0 : Rx
 *    1 : Tx
 *
 * @param [out] u32DataDelay
 *    Delay applied to the data.
 * 
 * @return
 *    Error code: #eRadio640Ok if no problem is found, !=#eRadio640Ok if problem.
 *    Use FAILURE(res) macro to detect if an error occurred.
 *
 * @pre
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *                   
 * @see              
 *    Radio640_Open(), Radio640_Connect(), Radio640_Init()
 *
 ***************************************************************************/
Result Radio640_GetDataDelay(Radio640_hRadio640 hRadio640, uint8_t u8Channel, uint8_t u8Direction, uint32_t *u32DataDelay);


#endif
