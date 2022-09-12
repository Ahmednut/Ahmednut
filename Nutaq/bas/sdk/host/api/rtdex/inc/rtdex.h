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
 * @file    rtdex.h
 * @brief   Function declaration for RTDEx
 *
 * $Date: 2015/08/10 17:10:40 $
 * $Revision: 1.1 $
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


#ifndef ADP_RTDEx_H
#define ADP_RTDEx_H

#ifdef __cplusplus
extern "C" {
#endif




/****************************************************************************
* Function: RTDExResetFifo
*************************************************************************//**
*
* Resets the FIFO of the channel.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExResetFifo( adp_handle_rtdex_conn hConnRtDex );

/****************************************************************************
 * Function: RTDExClose
 ************************************************************************//**
 *
 * Close an RTDEx channel instance 
 *
 * @param [in,out] hConnRtDex
 *    Handle to the channel instance to be closed
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExClose( adp_handle_rtdex_conn hConnRtDex );

/****************************************************************************
* Function: RTDExStart
*************************************************************************//**
*
* Starts an RTDEx transfer.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [in] uFrameSize
*      Size of the frame (depending on the media capacity), in bytes.
*      With Ethernet, can be configured to 8960 when the MAC supports jumbo frames.
*
* @param [in] uTransferSize
*      Total size of the current transfer (in bytes).
*       Cannot be higher than the size of the buffer specified in the RTDExOpen uBufferSize parameters.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExStart( adp_handle_rtdex_conn hConnRtDex, unsigned uFrameSize, size_t uTransferSize);
						 

/****************************************************************************
* Function: RTDExReceive
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
*      Maximum size of data received
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
int RTDExReceive( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, 
				      eRTDExTransferMode eMode, unsigned int uTimeOut);
				 
/****************************************************************************
* Function: RTDExSend
*************************************************************************//**
*
* Sends RTDEx data
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [in]  pDataBuffer
*      Ptr to buffer to receive the RTDEx data
*
* @param [in] u32MaxSize
*      The desired amount of bytes to send. Must be a multiple of 4.
*
* @return
*      Number of bytes sent. 
*      Error when -1 is returned. Use RTDExGetLastError function to know 
*      the error type.
*
****************************************************************************/
int RTDExSend( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize);

/****************************************************************************
* Function: RTDExGetLastError
*************************************************************************//**
*
* Return the last error of RTDExReceive() or RTDExSend():
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @return
*      The last error code, RTDEXOK if success.
*
****************************************************************************/
adp_result_t RTDExGetLastError ( adp_handle_rtdex_conn hConnRtDex);

/****************************************************************************
* Function: RTDExGetRxFramesCnt
*************************************************************************//**
*
* Retrieves the number of frames received of the RX side of the channel.
* For a Host to FPGA, RX side is FPGA, for a FPGA to Host, RX side is Host.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [out] pu32Cnt
*      Number of frames
*
* @return
*    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
*    Use SUCCESS(ret) macro to detect if an error occurred.
****************************************************************************/
adp_result_t RTDExGetRxFramesCnt( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32Cnt);

/****************************************************************************
* Function: RTDExGetTxFramesCnt
*************************************************************************//**
*
* Retrieves the number of frames transmitted by the RTDEx on the FPGA for
* a channel FPGA to FPGA or FPGA to Host. If channel direction is Host to FPGA,
* this value is the number of frames transmitted by the Host.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [out] pu32Cnt
*      Number of frames transmitted
*
* @return
*    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
*    Use SUCCESS(ret) macro to detect if an error occurred. 
****************************************************************************/
adp_result_t RTDExGetTxFramesCnt( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32Cnt);

/****************************************************************************
 * Function: RTDExGetTxFifoDataCnt
 ************************************************************************//**
 *
 * Retrieves the number of bytes stalled in the TX RTDEx fifo.
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] pu32DataCount
 *    Number of frames
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxFifoDataCnt( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32DataCount);


#ifdef __cplusplus
}
#endif


#endif
