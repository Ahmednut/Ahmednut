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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**    
 *
 * @file    rtdexts.h
 * @brief   RTDExTs public API.
 *
 * $Date: 2015/08/10 17:10:41 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RTDEXTS
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
#ifndef RTDEXTS_H
#define RTDEXTS_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#include "bas_types.h"              // adp_result_t
#include "connection_state.h"       // connection_state
#include "rtdexts_def.h"


/****************************************************************************
 *                                Public API                                * 
 ****************************************************************************/

/****************************************************************************
 * Function: RTDExSync_Reset
 *****************************************************************************
 *
 * Globally reset the RTDExTs core.
 * The RTDExSync core will be reset but not the Timestamp core.
 *
 * @param [in]  state
 *      Pointer to the connection state with the CCE.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_ResetCore( connection_state * pState );

/****************************************************************************
 * Function: RTDExTs_GetChInfo
 *****************************************************************************
 *
 * Get the info about a RTDExTs channel.
 *
 * @param [in]  state
 *      Pointer to the connection state with the CCE.
 *
 * @param [in]  u8Channel
 *      RTDExTs channel number
 *
 * @param [out]  pPresent
 *      Presence flag (0:not present, 1:present)
 *
 * @param [out]  pDir (0:uplink, 1:downlink ())
 *      Channel direction.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_GetChInfo( connection_state * pState,
                                uint8_t u8Channel,
                                int * pPresent,
                                int * pDir );

/****************************************************************************
 * Function: RTDExTs_Open
 *****************************************************************************
 *
 * Open RTDExTs instance
 *
 * @param [out]  phRTDExTs
 *      Pointer to RTDExTs handle.
 *
 * @param [in]  state
 *      Connection state between the host and the CCE.
 *
 * @param [in]  u8ChIdx;
 *      Channel to be configured.
 *
 * @param [in]  dir
 *      Direction of transfer is either 0:uplink (Radio -> Host) or 1:downlink
 *      (Host -> Radio). Note that the direction is determined at build time in
 *      the FPGA of the radio.
 *
 * @param [in]  u32RTDExEthBaseAddr
 *      Base address of the RTDEx Ethernet core in the target FPGA. This value
 *      is don't care if the RTDEx core is mapped on the PCIe bus. It is usually
 *      set to 'module_rtdex_eth_base' from the file 'memory_map.h'.
 *
 * @param [in]  u32SubframeSize
 *      RTDExTs subframe size in bytes. The maximum depends on the media capacity).
 *      Maximum is 8980 bytes on Ethernet using jumbo frames. On PCIe, it controls
 *      the CPU interrupt rates.
 *
 * @param [in]  u32FrameSize
 *      RTDExTs frame size in bytes. It must be a multiple of the subframe size.
 *      Each RTDExTs data transfer will transfer this quantity of bytes between
 *      the host and the radio even if the frame is not full. If more data than what
 *      can be transfer in one frame is requested, it will be split in multiple frames.
 *
 * @param [in]  u32TsClkFreq
 *      Frequency of the clock used by the timestamp module.
 *
 * @param [in]  bTsInit
 *      0: Do not initialize the timestamp module.
 *      1: Initialize the timestamp module with the host time.
 *         It requires a valid PPS signal and a host time synchronized within 
 *         ±100 ms compare to the PPS signal.
 *
 * @param [out]  pu32MaxIQSamples
 *      Address of a variable that receives the maximum number of IQ samples
 *      that can be sent or received per RTDExTs frame.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_Open( RTDExTs_Handle_t *phRTDExTs,
                           connection_state state,
                           uint8_t u8ChIdx,
                           RTDExTs_Dir_t dir,
                           uint32_t u32RTDExEthBaseAddr,
                           uint32_t u32SubframeSize,
                           uint32_t u32FrameSize,
                           uint32_t u32TsClkFreq,
                           uint8_t  bTsInit,
                           uint32_t *pu32MaxIQSamples);

/****************************************************************************
 * Function: RTDExTs_Close
 *****************************************************************************
 *
 * Close RTDExTs instance
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
 *
 * @return
 *      None.
 *
 ****************************************************************************/
void RTDExTs_Close( RTDExTs_Handle_t hRTDExTs );

/****************************************************************************
 * Function: RTDExTs_VerifyTime
 *****************************************************************************
 *
 * Verify if a PPS signal is present.
 * If yes, verify if the device time is within 1 second 
 * from the host computer time
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_VerifyTime( RTDExTs_Handle_t hRTDExTs );

/****************************************************************************
 * Function: RTDExTs_SetPropagationDelay
 *****************************************************************************
 *
 * This function is used to compensate the propagation delay.
 *
 * The propagation delay can be used to compensate for the propagation time
 * in the analogical path, the ADC/DAC chips and the user logic algorithm
 *
 * The propagation delay resolution is limited by the timestamp
 * clock period. The time offset value is truncated to the previously clock 
 * period multiple if the specified offset does not fit the current resolution
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
 *
 * @param [in]  i32nsecDelay
 *      Propagation delay of the path expressed in nanosecond
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_SetPropagationDelay(
    RTDExTs_Handle_t hRTDExTs,
    int32_t i32nsecDelay);

/****************************************************************************
 * Function: RTDExTs_Start
 *****************************************************************************
 *
 * Program the start mode of a new stream of transmission. In downlink, the 
 * start command will be sent along the first data packet (RTDExTs_Send).
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
 *
 * @param [in]  mode
 *      Start mode.
 *
 * @param [in]  trigSel
 *      Trigger selection (0, 1, 2, 3). Only used for OnTrigger mode.
 *
 * @param [in]  u32secStartTime
 *      Absolute start time in UTC POSIX time. Only used for AtTime mode.
 *
 * @param [in]  u32nsecStartTime
 *      Absolute start time offset in nanosecond from the UTC POSIX time. Only used for AtTime mode.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_Start( RTDExTs_Handle_t hRTDExTs,
                            RTDExTs_Mode_t mode,
                            RTDExTs_TrigSel_t trigSel,
                            uint32_t u32secStartTime, uint32_t u32nsecStartTime);

/****************************************************************************
 * Function: RTDExTs_Stop
 *****************************************************************************
 *
 * Program the stop mode of a transmission stream. In downlink, the stop command 
 * will only be sent in the next data packet when the mode is OnTrigger or AtTime. 
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
 *
 * @param [in]  mode
 *      Stop mode.
 *
 * @param [in]  trigSel
 *      Trigger selection (0, 1, 2, 3). Only used for OnTrigger mode.
 *
 * @param [in]  u32secStopTime
 *      Absolute stop time in UTC POSIX time. Only used for AtTime mode.
 *
 * @param [in]  u32nsecStopTime
 *      Absolute stop time offset in nanosecond from the UTC POSIX time. Only used for AtTime mode.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_Stop( RTDExTs_Handle_t hRTDExTs,
                           RTDExTs_Mode_t mode,
                           RTDExTs_TrigSel_t trigSel,
                           uint32_t u32secStopTime, uint32_t u32nsecStopTime);

/****************************************************************************
 * Function: RTDExTs_Send
 *****************************************************************************
 *
 * Perform a RTDExTs transfer from the host to the device.
 *
 * @param [in]  hRTDExTs
 *      A pRTDExTs handle.
 *
 * @param [in]  pIQSample
 *      A pointer to the buffer containing the IQ samples be written.
 *
 * @param [in]  u32NumSample
 *      The number of IQ samples to be written. An IQ sample is a pair of 16-bit value.
 *      If u32NumSample is biffer than max transfer size, it will be splitted in multiple
 *      transfers. If u32NumSample is 0, a single transfer with no data will be performed.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its lenght must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @return
 *      Returns the number of samples written or an error code. The count could
 *      be smaller than the data size if the stop condition has been is reached.
 *
 ****************************************************************************/
adp_result_t RTDExTs_Send( RTDExTs_Handle_t hRTDExTs,
                           void *           pIQSample,
                           uint32_t         u32NumSample,
                           uint32_t         u32EvtQueueSize,
                           RTDExTs_Event_t  *pEvtQueue,
                           uint32_t         *pu32EvtCount );

/****************************************************************************
 * Function: RTDExTs_Receive
 *****************************************************************************
 *
 * Perform a RTDExTs transfer form the device to the host.
 *
 * @param [in]  hRTDExTs
 *      A pRTDExTs handle.
 *
 * @param [out]  pIQSample
 *      A pointer to the buffer that receives the transfered IQ samples.
 *
 * @param [in]  u32NumSample
 *      The number of IQ samples to receive.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its lenght must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @param [in]  block
 *      Blocking mode flag. If '0', the function returns only the data available.
 *      If not '0', the routine wait for the requested amount of data before returning.
 *
 * @return
 *      Returns the number of samples received or an error code. The count could
 *      be smaller than the data size if the stop condition has been is reached.
 *
 ****************************************************************************/
adp_result_t RTDExTs_Receive( RTDExTs_Handle_t  hRTDExTs,
                                void *          pIQSample,
                                uint32_t        u32NumSample,
                                uint32_t        u32EvtQueueSize,
                                RTDExTs_Event_t *pEvtQueue,
                                uint32_t        *pu32EvtCount,
                                int             block );

/****************************************************************************
 * Function: RTDExTs_IsStarted
 *****************************************************************************
 *
 * Check if the RTDExTs channel is started or not.
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its lenght must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @param [in]  u32TimeoutMs
 *      Timeout in msec.
 *
 * @return
 *      Returns "1" if started, "0" if nor, or an error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_IsStarted( RTDExTs_Handle_t hRTDExTs,
                                uint32_t         u32EvtQueueSize,
                                RTDExTs_Event_t  *pEvtQueue,
                                uint32_t         *pu32EvtCount,
                                uint32_t         u32TimeoutMs );

/****************************************************************************
 * Function: RTDExTs_IsStopped
 *****************************************************************************
 *
 * Check if the RTDExTs channel is stopped or not.
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its lenght must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @param [in]  u32TimeoutMs
 *      Timeout in msec.
 *
 * @return
 *      Returns "1" if stopped, "0" if nor, or an error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_IsStopped( RTDExTs_Handle_t hRTDExTs,
                                uint32_t         u32EvtQueueSize,
                                RTDExTs_Event_t  *pEvtQueue,
                                uint32_t         *pu32EvtCount,
                                uint32_t         u32TimeoutMs );

/****************************************************************************
 * Function: RTDExTs_Abort
 *****************************************************************************
 *
 * Abort the current RTDExTs state in order to be able to
 * handle a new start command.
 *
 * This command will stop any current data transmission and reception since it
 * is not synchronous with the data stream.
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExTs_Abort( RTDExTs_Handle_t hRTDExTs );

/****************************************************************************
 * Function: RTDExTs_GetHostPosixTime
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
adp_result_t RTDExTs_GetHostPosixTime( uint32_t * pu32sec, uint32_t * pu32nsec);

/****************************************************************************
 * Function: RTDExTs_GetHostReadableLocalTime
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
adp_result_t RTDExTs_GetHostReadableLocalTime( char * pcTime, uint32_t u32Length );

/****************************************************************************
 * Function: RTDExTs_GetDevicePosixTime
 *****************************************************************************
 *
 * Get the current device UTC time in POSIX format
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
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
adp_result_t RTDExTs_GetDevicePosixTime( RTDExTs_Handle_t hRTDExTs, uint32_t * pu32sec, uint32_t * pu32nsec);

/****************************************************************************
 * Function: RTDExTs_GetDeviceReadableLocalTime
 *****************************************************************************
 *
 * Get the current device local time in a readable format.
 * 
 * ex.: 2014-11-20 10:22:06.123456789
 *  
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [in]  hRTDExTs
 *      A RTDExTs handle.
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
adp_result_t RTDExTs_GetDeviceReadableLocalTime( RTDExTs_Handle_t hRTDExTs, char * pcTime, uint32_t u32Length );

/****************************************************************************
 * Function: RTDExTs_PosixToReadableLocalTime
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
adp_result_t RTDExTs_PosixToReadableLocalTime(uint32_t u32sec, uint32_t u32nsec, char *pcString, uint32_t u32Length);


/****************************************************************************
 * Function: RTDExTs_ReadableLocalTimeToPosix
 *****************************************************************************
 *
 * Convert a readable local time format to UTC time in POSIX format
 * 
 * ex.: 2014-11-20 10:22:06.123456789
 *  
 * where the last 9 digits after the '.' represent an offset in nanosecond
 *
 * Careful!
 *  "2014-11-20 10:22:06.5" means 10:22:06 +5 ns (not +500 ms !)
 *  To express +500 ms, add the remaining zeros to express the 
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
adp_result_t RTDExTs_ReadableLocalTimeToPosix(char *pcString, uint32_t * pu32sec, uint32_t * pu32nsec);


#ifdef __cplusplus
}
#endif
#endif    //RTDEX_SYNC_H

