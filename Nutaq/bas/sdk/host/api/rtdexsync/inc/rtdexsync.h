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
 * @file    rtdexsync.h
 * @brief   RTDExSync public API.
 *
 * $Date: 2015/08/10 17:10:40 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RTDEXSYNC
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
#ifndef RTDEX_SYNC_H
#define RTDEX_SYNC_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#include "bas_types.h"              // adp_result_t
#include "connection_state.h"       // connection_state
#include "rtdexsync_def.h"


/****************************************************************************
 *                                Public API                                * 
 ****************************************************************************/ 

/****************************************************************************
 * Function: RTDExSync_Reset
 *****************************************************************************
 *
 * Globally reset the RTDExSync core
 *
 * @param [in]  state
 *      Pointer to the connection state with the CCE.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_ResetCore( connection_state * state );

/****************************************************************************
 * Function: RTDExSync_GetChInfo
 *****************************************************************************
 *
 * Get the info about a RTDExSync channel.
 *
 * @param [in]  state
 *      Pointer to the connection state with the CCE.
 *
 * @param [in]  u8Channel
 *      RTDExSync channel number
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
adp_result_t RTDExSync_GetChInfo( connection_state * state,
                                  uint8_t u8Channel,
                                  int *pPresent, 
                                  int *pDir );

/****************************************************************************
 * Function: RTDExSync_Open
 *****************************************************************************
 *
 * Open RTDExSync instance
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
 *      RTDExSync subframe size in bytes. The maximum depends on the media capacity).
 *      Maximum is 8980 bytes on Ethernet using jumbo frames. On PCIe, it controls 
 *      the CPU interrupt rates.
 *
 * @param [in]  u32FrameSize
 *      RTDExSync frame size in bytes. It must be a multiple of the subframe size.
 *      Each RTDExSync data transfer will transfer this quantity of bytes between 
 *      the host and the radio even if the frame is not full. If more data than what 
 *      can be transfer in one frame is requested, it will be split in multiple frames.
 *
 * @param [out]  pu32MaxIQSamples
 *      Address of a variable that receives the maximum number of IQ samples
 *      that can be sent or received per RTDExSync frame.
 *
 * @param [out]  phRTDExSync
 *      Pointer to RTDExSync handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_Open( connection_state state,
                             uint8_t u8ChIdx, 
                             RTDExSync_Dir_t dir, 
                             uint32_t u32RTDExEthBaseAddr,
                             uint32_t u32SubframeSize,
                             uint32_t u32FrameSize,
                             uint32_t *pu32MaxIQSamples,
                             RTDExSync_Handle_t *phRTDExSync );

/****************************************************************************
 * Function: RTDExSync_Close
 *****************************************************************************
 *
 * Close RTDExSync instance
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @return
 *      None.
 *
 ****************************************************************************/
void RTDExSync_Close( RTDExSync_Handle_t hRTDExSync );

/****************************************************************************
 * Function: RTDExSync_Start
 *****************************************************************************
 *
 * Program the start mode of a new stream of transmission. In downlink, the 
 * start command will be sent along the first data packet (RTDExSync_Send).
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @param [in]  mode
 *      Start mode.
 *
 * @param [in]  trigSel
 *      Trigger selection (0, 1, 2, 3). Only used for OnTrigger mode.
 *
 * @param [in]  u64StartTime
 *      Absolute time in clock cycle counts. Only used for AtTime mode.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_Start( RTDExSync_Handle_t hRTDExSync,
                              RTDExSync_Mode_t mode,
                              RTDExSync_TrigSel_t trigSel,
                              uint64_t u64StartTime );

/****************************************************************************
 * Function: RTDExSync_Stop
 *****************************************************************************
 *
 * Program the stop mode of a transmission stream. In downlink, the stop command 
 * will only be sent in the next data packet when the mode is OnTrigger or AtTime. 
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @param [in]  mode
 *      Stop mode.
 *
 * @param [in]  trigSel
 *      Trigger selection (0, 1, 2, 3). Only used for OnTrigger mode.
 *
 * @param [in]  u64StopTime
 *      Absolute time in clock cycle counts. Only used for AtTime mode.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_Stop( RTDExSync_Handle_t hRTDExSync,
                             RTDExSync_Mode_t mode,
                             RTDExSync_TrigSel_t trigSel,
                             uint64_t u64StopTime );

/****************************************************************************
 * Function: RTDExSync_Send
 *****************************************************************************
 *
 * Perform a RTDExSync transfer from the host to the device.
 *
 * @param [in]  hRTDExSync
 *      A pRTDExSync handle.
 *
 * @param [in]  pIQSample
 *      A pointer to the buffer containing the IQ samples to be written.
 *
 * @param [in]  u32NumSample
 *      The number of IQ samples to be written. An IQ sample is a pair of 16-bit value.
 *      If u32NumSample is bigger than the max transfer size, it will be splitted in multiple 
 *      transfers. If u32NumSample is 0, a single transfer with no data will be performed.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its length must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @return
 *      Returns the number of samples written or an error code. The count could 
 *      be smaller than the data size if the stop condition has been reached. 
 *
 ****************************************************************************/
adp_result_t RTDExSync_Send( RTDExSync_Handle_t hRTDExSync, 
                             void *             pIQSample,
                             uint32_t           u32NumSample,
                             uint32_t           u32EvtQueueSize,
                             RTDExSync_Event_t  *pEvtQueue,
                             uint32_t           *pu32EvtCount );

/****************************************************************************
 * Function: RTDExSync_Receive
 *****************************************************************************
 *
 * Perform a RTDExSync transfer form the device to the host.
 *
 * @param [in]  hRTDExSync
 *      A pRTDExSync handle.
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
adp_result_t RTDExSync_Receive( RTDExSync_Handle_t hRTDExSync, 
                                void *             pIQSample,
                                uint32_t           u32NumSample,
                                uint32_t           u32EvtQueueSize,
                                RTDExSync_Event_t  *pEvtQueue,
                                uint32_t           *pu32EvtCount,
                                int                block );

/****************************************************************************
 * Function: RTDExSync_IsStarted
 *****************************************************************************
 *
 * Check if the RTDExSync channel is started or not.
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
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
adp_result_t RTDExSync_IsStarted( RTDExSync_Handle_t hRTDExSync, 
                                  uint32_t           u32EvtQueueSize,
                                  RTDExSync_Event_t  *pEvtQueue,
                                  uint32_t           *pu32EvtCount,
                                  uint32_t           u32TimeoutMs );

/****************************************************************************
 * Function: RTDExSync_IsStopped
 *****************************************************************************
 *
 * Check if the RTDExSync channel is stopped or not.
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
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
adp_result_t RTDExSync_IsStopped( RTDExSync_Handle_t hRTDExSync, 
                                  uint32_t           u32EvtQueueSize,
                                  RTDExSync_Event_t  *pEvtQueue,
                                  uint32_t           *pu32EvtCount,
                                  uint32_t           u32TimeoutMs );

/****************************************************************************
 * Function: RTDExSync_Abort
 *****************************************************************************
 *
 * Abort the current RTDExSync state in order to be able to 
 * handle a new start command.
 *
 * This command will stop any current data transmission and reception since it
 * is not synchronous with the data stream.
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_Abort( RTDExSync_Handle_t hRTDExSync );

#ifdef __cplusplus
}
#endif
#endif    //RTDEX_SYNC_H

