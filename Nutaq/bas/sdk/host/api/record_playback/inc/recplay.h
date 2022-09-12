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
 * @file    adp_recplay.h
 * @brief   Function definitions for Record/Playback
 *
 * $Date: 2015/08/10 17:10:40 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RECORDPLAYBACK
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


#ifndef ADP_RECPLAY_H
#define ADP_RECPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bas_types.h"
#include "recplay_types.h"

/**
*****************************************************************************
* @file adp_recplay.h
*****************************************************************************
Record and playback interface functions
****************************************************************************/



/**
*****************************************************************************
* Function: RecPlay_Open
*****************************************************************************
*
* Creates a RecPlay object and returns a handle to this object.
*
* @param [in,out]  hRecPlay
*       Handle to the created record playback object
*
* @param [in]  state
*      Handle to the connection
*      The object must be valid.
*      Otherwise, the function returns an error.
*
* @param [in]  u64RecPlayFpgaBaseAddr
*      The base address in the FPGA (on the internal bus) of the recording and playback core.
*
*
* @return
*      Handle to the adp_handle_recplay object or ADP_INVALID_HANDLE.
*
****************************************************************************/
adp_result_t RecPlay_Open (adp_handle_recplay *hRecPlay, connection_state * state,
                                     uint64_t u64RecPlayFpgaBaseAddr );


/**
*****************************************************************************
* Function: RecPlay_Close
*****************************************************************************
*
* Frees a RecPlay object.
*
* @param [in]  hRecPlay
*      Handle to the valid RecPlay object.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlay_Close( adp_handle_recplay hRecPlay  );


/**
*****************************************************************************
* Function: RecPlayResetMemory
*****************************************************************************
*
* Resets the Xilinx DDR3 controller FPGA core.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlayResetMemory( adp_handle_recplay hRecPlay );


/**
*****************************************************************************
* Function: RecPlayResetCore
*****************************************************************************
*
* Resets the record and playback FPGA core logic.
*
* If the record and playback module is stuck in a old, not over, transfer mode,
* this function can be used to exit the current mode. It will not affect the
* memory content.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlayResetCore( adp_handle_recplay hRecPlay );


/**
*****************************************************************************
* Function: RecPlayGetMemoryInitDoneStatus
*****************************************************************************
*
* Returns the memory initialization status of the Xilinx DDR3 memory 
* controller.  
*
* This function should be called one second after a memomy reset to let
* some time to the memory controller to initialize properly.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [out] pInitDone
*      Returns 1 when the memory initialisation has succeeded.
*      If 0 is returned, try resetting the memory and wait a few seconds
*      before calling this function. If the function returns 0 again, 
*      verify that your DDR3 memory is correctly installed on the carrier or
*      contact Nutaq support.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlayGetMemoryInitDoneStatus( adp_handle_recplay hRecPlay,
                                             unsigned char* pInitDone );


/**
*****************************************************************************
* Function: RecPlayGetTransferStatus
*****************************************************************************
*
* Returns the status of the current transfer (complete or not).  
*
* A transfer is complete when the specified number of bytes has been read
* or written to the memory.
*
* If a transfer is stuck, use RecPlayResetCore function to set the mode
* back to idle. The current mode needs to be in idle before it can be
* configured into another modes with the "SetMode" functions.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [out]  pTransferOver
*      Status of the transfer.
*      0 : The last configured transfer mode is still running
*      1 : The last configured transfer mode is over.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlayGetTransferStatus( adp_handle_recplay hRecPlay,
                                       unsigned char* pTransferOver );

/**
*****************************************************************************
* Function: RecPlayWaitTransferDone
*****************************************************************************
*
* Wait until the last transfer mode is completed.
* This fuction can be used to know if all the requested bytes have been written
* inside the memory in Record mode.
* This function can also be used to know if the transfer from the host, the 
* transfer to the host or the playback is completed.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [out]  uTimeOutMs
*      Timeout value in millisecond.
*
* @return
*      RECPLAYOK if the transfer is done before the timeout is reached.
*      RECPLAYTRANSFERTIMEOUT if the timeout has been reached.
*
****************************************************************************/
adp_result_t RecPlayWaitTransferDone(adp_handle_recplay hRecPlay,
    unsigned int uTimeOutMs);


/**
*****************************************************************************
* Function: RecPlaySetModeRecord
*****************************************************************************
*
* Configures the recording and playback core to record.   
*
* @warning
*      Since the memory transaction are 64-byte wide, even if the trigger 
*      does not happen exactly at the beginning of a 64-byte word, 
*      the whole 64-byte word will be counted in the number of 
*      bytes written. This will prevent the last desired bytes to be 
*      written inside the memory. To avoid losing the last bytes if the trigger
*      can happen inside a 64-byte word, add an additional 64 bytes to the record
*      size.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [in]  uStartAddress
*      First data storage address in the platform memory in bytes.
*      This value must be a multiples of 64 bytes.
*
* @param [in]  uTrigDelayVal
*      This value will delay the trigger event by the specified number of bytes.
*      This value must be a multiple of 64 bytes.
*
* @param [in]  uRecSizeFromTrigOffset
*      Number of bytes stored inside the memory after the trigger event has been 
*      detected. Before the trigger event, the data is written continuously 
*      inside the memory and is not taken into account in the calculation of
*      the number of bytes written.
*      This value must be a multiple of 64 bytes.
*      
* @return
*      Success or error code.
*
* @see
*      RecPlayGetRecordNbPortAndWdt()
*
****************************************************************************/
 adp_result_t RecPlaySetModeRecord( adp_handle_recplay hRecPlay,
                                    unsigned int uStartAddress,
                                    unsigned int uTrigDelayVal,
                                    unsigned int uRecSizeFromTrigOffset );


/**
*****************************************************************************
* Function: RecPlayGetTriggerPos
*****************************************************************************
*
* Returns the address where to read recorded data.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [out]  puTrigAddress
*      Memory address in bytes where the record trigger event occured.
*       
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlayGetTriggerPos( adp_handle_recplay hRecPlay,
                                   unsigned int* puTrigAddress );


/**
*****************************************************************************
* Function: RecPlaySetModePlayBackSingle
*****************************************************************************
*
* Configures the recording and playback core to playback in single mode.   
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [in]  uStartAddress
*      First playback data address in board memory.
*      The start address is specified in bytes and
*      must be a multiple of 64 bytes.
*
* @param [in]  uTransferSize
*      Number of bytes to playback from the memory's uStartAddress 
*      and sent (only once) to playback ports. The transfer size
*      must be a multiple of 64 bytes.
*      
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlaySetModePlayBackSingle( adp_handle_recplay hRecPlay,
                                           unsigned int uStartAddress,
                                           unsigned int uTransferSize );


/**
*****************************************************************************
* Function: RecPlaySetModePlayBackContinuous
*****************************************************************************
*
* Configures the recording and playback core in continuous playback mode.   
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [in]  uStartAddress
*      First playback data address in board memory.
*      The start address is specified in bytes and
*      must be a multiple of 64 bytes.
*
* @param [in]  uTransferSize
*      Number of bytes to read from the card's memory uStartAddress
*      and continuously sent to the playback ports. The transfer size
*      must be a multiple of 64 bytes.
*      
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlaySetModePlayBackContinuous( adp_handle_recplay hRecPlay,
                                               unsigned int uStartAddress,
                                               unsigned int uTransferSize );


/**
*****************************************************************************
* Function: RecPlaySetModeRtdexHost2Mem
*****************************************************************************
*
* Configures the recording and playback core in Host2Mem mode.   
*
* This mode is used to received from an RTDEx channel, data to load into
* the memory.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [in]  uStartAddress
*      The address in the card's memory where the first data is stored.
*      The start address is specified in bytes.
*      and this value must be a multiples of 64 bytes.
*
* @param [in]  uTransferSize
*      The size of the transfer specified in bytes.
*      This value must be a multiples of 64 bytes.
*      
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlaySetModeRtdexHost2Mem( adp_handle_recplay hRecPlay,
                                          unsigned int uStartAddress,
                                          unsigned int uTransferSize );


/**
*****************************************************************************
* Function: RecPlaySetModeRtdexMem2Host
*****************************************************************************
*
* Configures the recording and playback core in Mem2Host mode.   
*
* This mode is used to send to an RTDEx channel the memory content.
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [in]  uStartAddress
*      The address in the card's memory where the first data is read.
*      The start address is specified in bytes.
*      and this value must be a multiples of 64 bytes.
*
* @param [in]  uTransferSize
*      The size of the transfer specified in bytes.
*      This value must be a multiples of 64 bytes.
*      
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlaySetModeRtdexMem2Host( adp_handle_recplay hRecPlay,
                                          unsigned int uStartAddress,
                                          unsigned int uTransferSize );


/**
*****************************************************************************
* Function: RecPlayGetRecordNbPortAndWdt
*****************************************************************************
*
* Returns the current configuration of the recording module.   
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [out]  pNbPorts
*      Number of ports available under the current configuration.
*
* @param [out]  pPortWidth
*      Width of each port under the current configuration.
*      
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlayGetRecordNbPortAndWdt( adp_handle_recplay hRecPlay,
                                           unsigned int* pNbPorts,
                                           unsigned int* pPortWidth );

                                           
/**
*****************************************************************************
* Function: RecPlayGetPlaybackNbPortAndWdt
*****************************************************************************
*
* Returns the current playback configuration.   
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [out]  pNbPorts
*      Number of ports available under the current configuration.
*
* @param [out]  pPortWidth
*      Width of each port under the current configuration.
*      
*
* @return
*      Success or error code.
*
****************************************************************************/ 
adp_result_t RecPlayGetPlaybackNbPortAndWdt( adp_handle_recplay hRecPlay,
                                             unsigned int* pNbPorts,
                                             unsigned int* pPortWidth );


/**
*****************************************************************************
* Function: RecPlaySetTriggerSource
*****************************************************************************
*
* Configures the trigger source for Record mode.    
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [in]  eSrc
*     Source of the trigger from possible values as described in the 
*     eRecPlayTrigSrc enumeration.
*
* @return
*      Success or error code.
*
****************************************************************************/ 
adp_result_t RecordSetTriggerSource( adp_handle_recplay hRecPlay,
                                      eRecPlayTrigSrc eSrc );

                                      
/**
*****************************************************************************
* Function: RecordSoftTrig
*****************************************************************************
*
* Sends a software trigger for Record mode to the FPGA module when the trigger 
* source is eRecPlayTrigSoft.  
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @return
*      Success or error code.
*
****************************************************************************/ 
adp_result_t RecordSoftTrig( adp_handle_recplay hRecPlay );



/**
*****************************************************************************
* Function: PlaybackSetTriggerSource
*****************************************************************************
*
* Configures the trigger source for playback.    
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @param [in]  eSrc
*     Source of the trigger from possible values as described in the 
*     eRecPlayTrigSrc enumeration.
*
* @return
*      Success or error code.
*
****************************************************************************/ 
adp_result_t PlaybackSetTriggerSource( adp_handle_recplay hRecPlay,
                                      eRecPlayTrigSrc eSrc );

                                      
/**
*****************************************************************************
* Function: PlaybackSoftTrig
*****************************************************************************
*
* Sends a software trigger for Playback mode to the FPGA module. The trigger 
* source need to be eRecPlayTrigSoft.  
*
* @param [in]  hRecPlay
*      Handle to a valid RecPlay object.
*
* @return
*      Success or error code.
*
****************************************************************************/ 
adp_result_t PlaybackSoftTrig( adp_handle_recplay hRecPlay );


#ifdef __cplusplus
}
#endif


#endif
