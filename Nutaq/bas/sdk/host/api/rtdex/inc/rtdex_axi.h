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
 * @file    rtdex_axi.h
 * @brief   Functions definition for RTDEx access via Axi protocol.
 *
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RTDEX
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

#ifndef ADP_RTDEX_AXI_H
#define ADP_RTDEX_AXI_H

#ifdef __cplusplus
extern "C" {
#endif

/** @name RTDEx Axi parameters
 * 
 *  This structure is used to hold some communication parameters with Zedboard RTDEx
 *  library.
 */
typedef struct {
	int m_dev; ///< Connection with device
} sAxiRTDExStruct;

typedef sAxiRTDExStruct * adp_handle_rtdex_axi_conn;	


/****************************************************************************
 * Function: RTDExOpenAxi
 ************************************************************************//**
 *
 * Open an RTDEx channel instance 
 * When the module is not used anymore, it can be closed with RTDExCloseAxi().
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] device_name
 *    The name of the RTDEX character device.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @see
 *    RTDExCloseAxi
 *
 ***************************************************************************/ 
adp_result_t RTDExOpenAxi( adp_handle_rtdex_conn *hConn, int8_t *device_name);


/****************************************************************************
 * Function: RTDExCloseAxi
 ************************************************************************//**
 *
 * Close an RTDEx channel instance 
 *
 * @param [in,out] hConnRtDex
 *    Handle to the channel instance to be closed
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExCloseAxi( adp_handle_rtdex_conn hConnRtDex );


/****************************************************************************
* Function: RTDExStartAxi
*************************************************************************//**
*
* Connect to socket
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [in] u32FrameSize
*      Size of the frame (depending on the media capacity), in bytes.
*      With Ethernet, can be configured to 8000 when the MAC supports jumbo frames.
*
* @param [in] u32TransferSize
*      Total size of the current transfer (in bytes).
*       Cannot be higher than the size of the buffer specified in the RTDExOpen uBufferSize parameters.
*
* @return
*      Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
****************************************************************************/
adp_result_t RTDExStartAxi( adp_handle_rtdex_conn hConnRtDex, uint32_t u32FrameSize, uint32_t u32TransferSize);


/****************************************************************************
* Function: RTDExSendAxi
*************************************************************************//**
*
* Sends RTDEx data
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [in]  pDataBuffer
*      Ptr to data to send.
*
* @param [in] u32MaxSize
*      Maximum size of data to send
*
* @return
*      Success or error code.
*
****************************************************************************/
int RTDExSendAxi( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize);


/****************************************************************************
* Function: RTDExReceiveAxi
*************************************************************************//**
*
* Receives RTDEx data
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [in]  pDataBuffer
*      Ptr to buffer to receive the RTDEx data
*
* @param [in] u32MaxSize
*      Maximum size of data to receive
*
* @param [in] eMode
*      Receiving mode (blocking or not)
*
* @param [in] uTimeOut
*      Time out length if receive is blocking, ms
*
* @return
*      Number of bytes received
*
****************************************************************************/
int RTDExReceiveAxi( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, 
                     eRTDExTransferMode eMode, unsigned int uTimeOut );

#ifdef __cplusplus
}
#endif

#endif
