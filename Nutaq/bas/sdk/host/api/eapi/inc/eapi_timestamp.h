
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
*  @file       : eapi_timestamp.h
*  @brief      : Functions controlling the System Monitor from the host
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup TIMESTAMP
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

#ifndef __EAPI_TIMESTAMP__
#define __EAPI_TIMESTAMP__

#ifdef __cplusplus
extern "C" {
#endif

#include "connection_state.h"
#include "timestamp_defines.h"
#include "bas_error.h"
#include "bas_types.h"

/** @defgroup TIMESTAMP TIMESTAMP EAPI
 *
 * Functions to control the Timestamp FPGA core.
 * The Timestamp library can be used to provide a counter as a time reference.
 *
 * The counter can be free running and used for relative time comparison.
 * The counter can be relative a known event and its value give an absolute
 * time reference.
 *
 * @file
 *
 */

/****************************************************************************
 * Function: Timestamp_Open
 *****************************************************************************
 *
 * Open Timestamp instance
 *
 * @param [out]  phTimestamp
 *      Pointer to Timestamp handle.
 *
 * @param [in]  state
 *      Connection state between the host and the CCE.
 *
 * @param [in]  u32ClkFreq
 *      Frequency of the clock used.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_Open( Timestamp_Handle_t *phTimestamp,
    connection_state state,
    uint32_t u32ClkFreq);

/****************************************************************************
 * Function: Timestamp_Close
 *****************************************************************************
 *
 * Close Timestamp instance
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @return
 *      None.
 *
 ****************************************************************************/
void Timestamp_Close( Timestamp_Handle_t hTimestamp );

/****************************************************************************
 * Function: Timestamp_Reset
 *****************************************************************************
 *
 * Reset the counter value to 0 and set the counter in free-running mode
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_Reset( Timestamp_Handle_t hTimestamp );

/****************************************************************************
 * Function: Timestamp_GetClkFreq
 *****************************************************************************
 *
 * Get the current clock frequency of the timestamp.
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [out] pu32ClkFreq
 *      Pointer to return the clock frequency value on success
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetClkFreq( Timestamp_Handle_t hTimestamp, uint32_t * pu32ClkFreq );

/****************************************************************************
 * Function: Timestamp_ApplyHostTimeToDevice
 *****************************************************************************
 *
 * Apply the current host time to the device.
 * It requires a valid PPS signal and a host time synchronized within
 * ±100 ms compare to the PPS signal.
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_ApplyHostTimeToDevice( Timestamp_Handle_t hTimestamp );

/****************************************************************************
 * Function: Timestamp_VerifyTime
 *****************************************************************************
 *
 * This function should be called after Timestamp_ApplyHostTimeToDevice
 * function call in order to verify that the host time has correctly been
 * set to the device.
 *
 * Verify if a PPS signal is present.
 * If yes, verify if the device time is within ±100 ms
 * from the host computer time
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_VerifyTime( Timestamp_Handle_t hTimestamp );

/****************************************************************************
 * Function: Timestamp_SetDeviceTickTime
 *****************************************************************************
 *
 * Set the current device time in tick (clock cycle number)
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [in]  u64Tick
 *      Time in number of clock ticks
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_SetDeviceTickTime( Timestamp_Handle_t hTimestamp, uint64_t u64Tick);

/****************************************************************************
 * Function: Timestamp_SetDevicePosixTime
 *****************************************************************************
 *
 * Set the current device UTC time in POSIX format
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [in]  u32sec
 *      UTC POSIX time
 *
 * @param [in]  u32nsec
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_SetDevicePosixTime( Timestamp_Handle_t hTimestamp, uint32_t u32sec, uint32_t u32nsec);

/****************************************************************************
 * Function: Timestamp_SetDeviceReadableLocalTime
 *****************************************************************************
 *
 * Set the current device local time in a readable format.
 *
 * ex.: 2014-11-20 10:22:06.123456789
 *
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [in]  pcTime
 *      Char array to that express local time in a readable string format
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_SetDeviceReadableLocalTime( Timestamp_Handle_t hTimestamp, char * pcTime);

/****************************************************************************
 * Function: Timestamp_GetHostPosixTime
 *****************************************************************************
 *
 * Get the current host computer UTC time in POSIX format
 *
 * @param [out]  pu32sec
 *      UTC POSIX time
 *
 * @param [out]  pu32nsec
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetHostPosixTime( uint32_t * pu32sec, uint32_t * pu32nsec);

/****************************************************************************
 * Function: Timestamp_GetHostReadableLocalTime
 *****************************************************************************
 *
 * Get the current host computer local time in a readable format.
 *
 * ex.: 2014-11-20 10:22:06.123456789
 *
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [out]  pcTime
 *      Output char array to store the readable time in a string format
 *
 * @param [in]  u32Length
 *      Maximal length of pcTime. The pcTime length must be at least 30.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetHostReadableLocalTime( char * pcTime, uint32_t u32Length );

/****************************************************************************
 * Function: Timestamp_GetDeviceTickTime
 *****************************************************************************
 *
 * Get the current device time in tick (clock cycle number)
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [out]  pu64Tick
 *      Pointer to a uint64_t variable.
 *      Time in number of clock ticks
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetDeviceTickTime( Timestamp_Handle_t hTimestamp, uint64_t * pu64Tick);

/****************************************************************************
 * Function: Timestamp_GetDevicePosixTime
 *****************************************************************************
 *
 * Get the current device UTC time in POSIX format
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [out]  pu32sec
 *      Pointer to a uint32_t variable.
 *      UTC POSIX time
 *
 * @param [out]  pu32nsec
 *      Pointer to a uint32_t variable.
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetDevicePosixTime( Timestamp_Handle_t hTimestamp, uint32_t * pu32sec, uint32_t * pu32nsec);

/****************************************************************************
 * Function: Timestamp_GetDeviceReadableLocalTime
 *****************************************************************************
 *
 * Get the current device local time in a readable format.
 *
 * ex.: 2014-11-20 10:22:06.123456789
 *
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [out]  pcTime
 *      Output char array to store the readable time in a string format
 *
 * @param [in]  u32Length
 *      Maximal length of pcTime. The pcTime length must be at least 30.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetDeviceReadableLocalTime( Timestamp_Handle_t hTimestamp, char * pcTime, uint32_t u32Length );

/****************************************************************************
 * Function: Timestamp_PosixToReadableLocalTime
 *****************************************************************************
 *
 * Convert a UTC POSIX time into a readable local time format
 *
 * ex.: 2014-11-20 10:22:06.123456789
 *
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [in]  u32sec
 *      UTC POSIX time
 *
 * @param [in]  u32nsec
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @param [out]  pcString
 *      Output char array to store the readable time in a string format
 *
 * @param [in]  u32Length
 *      Maximal length of pcTime. The pcTime length must be at least 30.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_PosixToReadableLocalTime(uint32_t u32sec, uint32_t u32nsec, char *pcString, uint32_t u32Length);


/****************************************************************************
 * Function: Timestamp_ReadableLocalTimeToPosix
 *****************************************************************************
 *
 * Convert a readable local time format to UTC time in POSIX format
 *
 * ex.: 2014-11-20 10:22:06.123456789
 *
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * Careful!
 *  "2014-11-20 10:22:06.5" means 10:22:06 +5 ns (not +500 ms !!!)
 *  To express +500 ms add the remaining zeros to express the
 *  value in nanosecond: "2014-11-20 10:22:06.500000000"
 *
 * @param [in]  pcTime
 *      Output char array to store the readable time in a string format
 *
 * @param [out]  pu32sec
 *      Pointer to a uint32_t variable.
 *      UTC POSIX time
 *
 * @param [out]  pu32nsec
 *      Pointer to a uint32_t variable.
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_ReadableLocalTimeToPosix(char *pcString, uint32_t * pu32sec, uint32_t * pu32nsec);


/****************************************************************************
* Function: Timestamp_PosixTimeToTick
*****************************************************************************
*
* Convert a POSIX time + offset in nanosecond in number of clock ticks.
*
* @param [in]  hTimestamp
*      A Timestamp handle.
*
* @param [in]  u32sec
*      Pointer to a uint32_t variable.
*      UTC POSIX time
*
* @param [in]  u32nsec
*      Offset in nanosecond from the UTC POSIX time
*
* @param [out]  pu64Tick
*      Pointer to a uint64_t variable.
*      Timestamp tick
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t Timestamp_PosixTimeToTick(
    Timestamp_Handle_t hTimestamp,
    uint32_t u32sec, uint32_t u32nsec,
    uint64_t * pu64Tick);

/****************************************************************************
* Function: Timestamp_TickToPosixTime
*****************************************************************************
*
* Convert Timestamp module clock tick to a POSIX time + offset in nanosecond
*
* @param [in]  hTimestamp
*      A Timestamp handle.
*
* @param [out]  u64Tick
*      Timestamp tick
*
* @param [out]  pu32sec
*      Pointer to a uint32_t variable.
*      UTC POSIX time
*
* @param [out] pu32nsec
*      Pointer to a uint32_t variable.
*      Offset in nanosecond from the UTC POSIX time
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t Timestamp_TickToPosixTime(
    Timestamp_Handle_t hTimestamp,
    uint64_t u64Tick,
    uint32_t * pu32sec, uint32_t * pu32nsec);


// EAPI function that communicate with the CCE
Result Timestamp_Presence_send(connection_state * state);
Result Timestamp_SetTime_send(connection_state * state, uint64_t u64Timestamp);
Result Timestamp_GetTime_send(connection_state * state, uint64_t * pu64Timestamp);
Result Timestamp_Reset_send(connection_state * state);

#ifdef __cplusplus
}
#endif

#endif
