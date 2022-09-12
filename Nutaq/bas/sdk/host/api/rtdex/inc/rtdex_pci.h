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
 * @file    rtdex_pci.h
 * @brief   Functions declaration for RTDEx access via PCIe.
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


#ifndef ADP_RTDEX_PCI_H
#define ADP_RTDEX_PCI_H

#ifdef __cplusplus
extern "C" {
#endif

/** @name RTDEx Perseus PCIe parameters
 * 
 *  This structure is used to hold some communication parameters with Perseus RTDEx
 *  library.
 */
 typedef struct {
   connection_state * state;  ///< Handle to the connection state object used for RTDEx core access
   unsigned uStdRegsBaseAddr; ///< Perseus Standards Regs base address on the axi bus
   unsigned uMailboxBaseAddr; ///< Perseus Mailbox Dualport RAM base address on the axi bus
} RTDExPerseusParamsPcie;

#ifdef __linux__

/****************************************************************************
 * Function: RTDExInitParamsPcie
 ************************************************************************//**
 *
 * Initialize communication parameters with RTDEx library. This function must be called
 * first, before any other call to the API. After calling this function, user can modify
 * uStdRegsBaseAddr or uMailboxBaseAddr, according to his core design.
 *
 * @param [in] pPerseusParams
 *    Ptr to structure allocated by the application.
 *
 * @param state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExInitParamsPcie( RTDExPerseusParamsPcie * pPerseusParams, connection_state * state);



/****************************************************************************
 * Function: RTDExGetDriverVerPcie
 ************************************************************************//**
 *
 * Retrives the number of TX channel of the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puMaj
 *    Driver version major number, BCD
 *
 * @param [out] puMin
 *    Driver version minor number, BCD
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetDriverVerPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puMaj, uint32_t * puMin);


/****************************************************************************
 * Function: RTDExGetCoreIdVerPcie
 ************************************************************************//**
 *
 * Retrives the number of TX channel of the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puId
 *    Core ID, 0-0ffffh
 *
 * @param [out] puVer
 *    Core Version, 0-0ffffh
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetCoreIdVerPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puId, uint32_t * puVer);



/****************************************************************************
 * Function: RTDExGetRxNbChannelsPcie
 ************************************************************************//**
 *
 * Retrives the number of RX channel of the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] pu32NbChannels
 *    Number of RX channels
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxNbChannelsPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t* pu32NbChannels);


/****************************************************************************
 * Function: RTDExGetTxNbChannelsPcie
 ************************************************************************//**
 *
 * Retrives the number of TX channel of the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] pu32NbChannels
 *    Number of RX channels
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxNbChannelsPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t* pu32NbChannels);



/****************************************************************************
 * Function: RTDExGetRxConfigExtentedPcie
 ************************************************************************//**
 *
 * Retrives the RX channel FIFO depth, and a flag indicating if statistics are included
 * in the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puFifoDepth
 *    Fifo size, in bytes
 *
 * @param [out] puStatsIsIncluded
 *    1 if statistics are included, else 0.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxConfigExtentedPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded);


/****************************************************************************
 * Function: RTDExGetTxConfigExtentedPcie
 ************************************************************************//**
 *
 * Retrives the TX channel FIFO depth, and a flag indicating if statistics are included
 * in the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puFifoDepth
 *    Fifo size, in bytes
 *
 * @param [out] puStatsIsIncluded
 *    1 if statistics are included, else 0.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxConfigExtentedPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded);



/****************************************************************************
 * Function: RTDExResetCorePcie
 ************************************************************************//**
 *
 * Reset the RTDEx core. All registers are reset to their default values.
 * This function should be called first, before calling any other
 * functions of the library.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
adp_result_t RTDExResetCorePcie( RTDExPerseusParamsPcie * pPerseusParams);


/****************************************************************************
 * Function: RTDExGetDriverInstancePcie
 ************************************************************************//**
 *
 * Retreive the Host Kernel Driver Instance number associated with the selected RTDEx.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puInst
 *    The instance #, 0 to n
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
adp_result_t RTDExGetDriverInstancePcie( RTDExPerseusParamsPcie * pPerseusParams, unsigned * puInst);


/****************************************************************************
 * Function: RTDExOpenHostAndFpgaPcie
 ************************************************************************//**
 *
 * Open a channel for dev --> host or dev <-- host operation:
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [in] uFifoNo
 *    channel number, 0 to 7
 *
 * @param [in] eDir
 *    transfert direction: eToFpga or eFromFpga
 *
 * @param [in] eMode
 *    transfert mode: eSingle or eContinuous
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
adp_result_t RTDExOpenHostAndFpgaPcie( adp_handle_rtdex_conn *hConn,
                                  RTDExPerseusParamsPcie * pPerseusParams,
                                  unsigned uFifoNo,
                                  eRTDExTransferDir eDir, // eToFpga or eFromFpga
                                  eRTDExMode eMode); // eSingle, eContinuous


/****************************************************************************
 * Function: RTDExOpenFpga2FpgaPcie
 ************************************************************************//**
 *
 * Open a channel for dev --> dev operation:
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] pSrcPerseusParams
 *    Ptr to comunication parameters for the sourcing RTDEx core.
 *
 * @param [in] pDstPerseusParams
 *    Ptr to comunication parameters for the destination RTDEx core.
 *
 * @param [in] uSrcFifoNo
 *    source channel number, 0 to 7
 *
 * @param [in] uDstFifoNo
 *    destination channel number, 0 to 7
 *
 * @param [in] eMode
 *    transfert mode: eSingle or eContinuous
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
adp_result_t RTDExOpenFpga2FpgaPcie( adp_handle_rtdex_conn *hConn,
                                  RTDExPerseusParamsPcie * pSrcPerseusParams,
                                  RTDExPerseusParamsPcie * pDstPerseusParams,
                                  unsigned uSrcFifoNo,
                                  unsigned uDstFifoNo,
                                  eRTDExMode eMode); // eSingle, eContinuous

//
// Per channel functions:
//
/****************************************************************************
 * Function: RTDExClosePcie
 ************************************************************************//**
 *
 * Close an RTDEx channel
 *
 * @param [in,out] hConnRtDex
 *    Handle to the channel instance to be closed
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExClosePcie( adp_handle_rtdex_conn hConnRtDex);


/****************************************************************************
* Function: RTDExResetFifoPcie
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
adp_result_t RTDExResetFifoPcie( adp_handle_rtdex_conn hConnRtDex);



/****************************************************************************
* Function: RTDExStartPcie
*************************************************************************//**
*
* Starts an RTDEx transfer.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [in] u32FrameSize
*      Size of a PCIe frame, in bytes.
*      Must be a multiple of 8 bytes and can be up to 1024*1024 = 1048576 bytes
*
* @param [in] transferSize
*      Total size of the current transfer (in bytes).
*       Cannot be higher than the size of the buffer specified in the RTDExOpen uBufferSize parameters.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExStartPcie( adp_handle_rtdex_conn hConnRtDex, uint32_t u32FrameSize, size_t transferSize);



/****************************************************************************
* Function: RTDExStopPcie
*************************************************************************//**
*
* Stop an RTDEx transfer.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExStopPcie( adp_handle_rtdex_conn hConnRtDex);



/****************************************************************************
* Function: RTDExSendPcie
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
int RTDExSendPcie( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, uint32_t u32MaxSize);



/****************************************************************************
* Function: RTDExReceivePcie
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
*      Maximum size of data to receive (buffer must be enaugh large for that number)
*
* @param [in] eMode
*      Receiving mode eRTDExWaitForever, eRTDExWaitTimeout or eRTDExDontWait 
*
* @param [in] uTimeoutMs
*      Time out length if receive is blocking, ms
*
* @return
*      Number of bytes received. Could be less than buffer size.
*      Could be 0, which mean that EOF was detected
*      if user try to read more than programmed transfer size.
*
****************************************************************************/
int RTDExReceivePcie( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, eRTDExTransferMode eMode, unsigned int uTimeoutMs);



/****************************************************************************
* Function: RTDExGetLastErrorPcie
*************************************************************************//**
*
* Return the last error of RTDEx functions RTDExSendPcie() or RTDExReceivePcie().
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
* @return
*      The last error code, RTDEXOK if success.
*
****************************************************************************/
adp_result_t RTDExGetLastErrorPcie( adp_handle_rtdex_conn hConnRtDex);


/****************************************************************************
* Function: RTDExSetLastErrorPcie
*************************************************************************//**
*
* Set the last error of RTDEx functions. This value will be returned when
* RTDExGetLastErrorPcie function will be called.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [in]  Result
*      Error value to set
*
****************************************************************************/
void RTDExSetLastErrorPcie( adp_handle_rtdex_conn hConnRtDex, adp_result_t Result);

/****************************************************************************
 * Function: RTDExGetTxFrameCountPcie
 ************************************************************************//**
 *
 * Retreives the number of frames transmitted by the RTDEx on the FPGA for
 * a channel FPGA to FPGA or FPGA to Host. If channel direction is Host to FPGA,
 * this value is the number of frames transmitted by the Host.
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] pVal
 *    Number of frames transmitted
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxFrameCountPcie( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal);



/****************************************************************************
 * Function: RTDExGetTxByteCountPcie
 ************************************************************************//**
 *
 * Retreives the number of bytes transmitted by the RTDEx on the FPGA for
 * a channel FPGA to FPGA or FPGA to Host. If channel direction is Host to FPGA,
 * this value is the number of bytes transmitted by the Host.
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] pVal
 *    Number of frames transmitted
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxByteCountPcie( adp_handle_rtdex_conn hConnRtDex, size_t *pVal);



/****************************************************************************
 * Function: RTDExGetRxFrameCountPcie
 ************************************************************************//**
 *
 * Retrives the number of frames received of the RX side of the channel.
 * For a Host to FPGA, RX side is FPGA, for a FPGA to Host, RX side is Host.
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] pVal
 *    Number of frames
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxFrameCountPcie( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal);



/****************************************************************************
 * Function: RTDExEnableFifoMultiPcie
 ************************************************************************//**
 *
 * Enable or disable simultaneously the FIFOs of a list of RTDEx channels.
 * Usefull to make sure all FIFOs start sending or receiving data at same time.
 *
 * @param [in] hConnRtDex[]
 *    Array of handle(s) to RTDEx channels
 *
 * @param [in] uNbConn
 *    Number of entries of hConnRtDex array
 *
 * @param [in] uEnRx
 *    Enable (1) or disable (0) the RX-FIFO
 *
 * @param [in] uEnTx
 *    Enable (1) or disable (0) the TX-FIFO
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExEnableFifoMultiPcie( adp_handle_rtdex_conn hConnRtDex[], unsigned uNbConn, unsigned uEnRx, unsigned uEnTx);



/****************************************************************************
 * Function: RTDExGetTxFifoStatusPcie
 ************************************************************************//**
 *
 * Determine if the TX-FIFO has overflowed during RTDEx transfer
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] puOverflow
 *    set to 1 if overflow has beend detected, else 0
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxFifoStatusPcie( adp_handle_rtdex_conn hConnRtDex, unsigned * puOverflow);


/****************************************************************************
 * Function: RTDExGetRxFifoStatusPcie
 ************************************************************************//**
 *
 * Determine if the RX-FIFO has underflowed during RTDEx transfer
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] puUnderflow
 *    set to 1 if underflow has beend detected, else 0
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxFifoStatusPcie( adp_handle_rtdex_conn hConnRtDex, unsigned * puUnderflow);

#else   // __linux__
/****************************************************************************
 * Function: RTDExInitParamsPcie
 ************************************************************************//**
 *
 * Initialize communication parameters with RTDEx library. This function must be called
 * first, before any other call to the API. After calling this function, user can modify
 * uStdRegsBaseAddr or uMailboxBaseAddr, according to his core design.
 *
 * @param [in] pPerseusParams
 *    Ptr to structure allocated by the application.
 *
 * @param state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExInitParamsPcie( RTDExPerseusParamsPcie * pPerseusParams, connection_state * state) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetDriverVerPcie
 ************************************************************************//**
 *
 * Retrives the number of TX channel of the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puMaj
 *    Driver version major number, BCD
 *
 * @param [out] puMin
 *    Driver version minor number, BCD
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetDriverVerPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puMaj, uint32_t * puMin) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetCoreIdVerPcie
 ************************************************************************//**
 *
 * Retrives the number of TX channel of the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puId
 *    Core ID, 0-0ffffh
 *
 * @param [out] puVer
 *    Core Version, 0-0ffffh
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetCoreIdVerPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puId, uint32_t * puVer) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetRxNbChannelsPcie
 ************************************************************************//**
 *
 * Retrives the number of RX channel of the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] pu32NbChannels
 *    Number of RX channels
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetRxNbChannelsPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t* pu32NbChannels) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetTxNbChannelsPcie
 ************************************************************************//**
 *
 * Retrives the number of TX channel of the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] pu32NbChannels
 *    Number of RX channels
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetTxNbChannelsPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t* pu32NbChannels) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetRxConfigExtentedPcie
 ************************************************************************//**
 *
 * Retrives the RX channel FIFO depth, and a flag indicating if statistics are included
 * in the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puFifoDepth
 *    Fifo size, in bytes
 *
 * @param [out] puStatsIsIncluded
 *    1 if statistics are included, else 0.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetRxConfigExtentedPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetTxConfigExtentedPcie
 ************************************************************************//**
 *
 * Retrives the TX channel FIFO depth, and a flag indicating if statistics are included
 * in the RTDEx core.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puFifoDepth
 *    Fifo size, in bytes
 *
 * @param [out] puStatsIsIncluded
 *    1 if statistics are included, else 0.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetTxConfigExtentedPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExResetCorePcie
 ************************************************************************//**
 *
 * Reset the RTDEx core. All registers are reset to their default values.
 * This function should be called first, before calling any other
 * functions of the library.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
static _inline adp_result_t RTDExResetCorePcie( RTDExPerseusParamsPcie * pPerseusParams) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetDriverInstancePcie
 ************************************************************************//**
 *
 * Retreive the Host Kernel Driver Instance number associated with the selected RTDEx.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [out] puInst
 *    The instance #, 0 to n
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetDriverInstancePcie( RTDExPerseusParamsPcie * pPerseusParams, unsigned * puInst) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExOpenHostAndFpgaPcie
 ************************************************************************//**
 *
 * Open a channel for dev --> host or dev <-- host operation:
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] pPerseusParams
 *    Ptr to comunication parameters for a RTDEx core.
 *
 * @param [in] uFifoNo
 *    channel number, 0 to 7
 *
 * @param [in] eDir
 *    transfert direction: eToFpga or eFromFpga
 *
 * @param [in] eMode
 *    transfert mode: eSingle or eContinuous
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
static _inline adp_result_t RTDExOpenHostAndFpgaPcie( adp_handle_rtdex_conn *hConn,
                                  RTDExPerseusParamsPcie * pPerseusParams,
                                  unsigned uFifoNo,
                                  eRTDExTransferDir eDir, // eToFpga or eFromFpga
                                  eRTDExMode eMode) // eSingle, eContinuous 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExOpenFpga2FpgaPcie
 ************************************************************************//**
 *
 * Open a channel for dev --> dev operation:
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] pSrcPerseusParams
 *    Ptr to comunication parameters for the sourcing RTDEx core.
 *
 * @param [in] pDstPerseusParams
 *    Ptr to comunication parameters for the destination RTDEx core.
 *
 * @param [in] uSrcFifoNo
 *    source channel number, 0 to 7
 *
 * @param [in] uDstFifoNo
 *    destination channel number, 0 to 7
 *
 * @param [in] eMode
 *    transfert mode: eSingle or eContinuous
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
static _inline adp_result_t RTDExOpenFpga2FpgaPcie( adp_handle_rtdex_conn *hConn,
                                  RTDExPerseusParamsPcie * pSrcPerseusParams,
                                  RTDExPerseusParamsPcie * pDstPerseusParams,
                                  unsigned uSrcFifoNo,
                                  unsigned uDstFifoNo,
                                  eRTDExMode eMode) // eSingle, eContinuous 
{
    return RTDEXMEDIANOTSUPPORTED;
}


//
// Per channel functions:
//
/****************************************************************************
 * Function: RTDExClosePcie
 ************************************************************************//**
 *
 * Close an RTDEx channel
 *
 * @param [in,out] hConnRtDex
 *    Handle to the channel instance to be closed
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExClosePcie( adp_handle_rtdex_conn hConnRtDex) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
* Function: RTDExResetFifoPcie
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
static _inline adp_result_t RTDExResetFifoPcie( adp_handle_rtdex_conn hConnRtDex) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
* Function: RTDExStartPcie
*************************************************************************//**
*
* Starts an RTDEx transfer.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @param [in] u32FrameSize
*      Size of the frame (depending on the media capacity), in bytes.
*      With Ethernet, can be configured to 8000 when the MAC supports jumbo frames.
*
* @param [in] transferSize
*      Total size of the current transfer (in bytes).
*       Cannot be higher than the size of the buffer specified in the RTDExOpen uBufferSize parameters.
*
* @return
*      Success or error code.
*
****************************************************************************/
static _inline adp_result_t RTDExStartPcie( adp_handle_rtdex_conn hConnRtDex, uint32_t u32FrameSize, size_t transferSize) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
* Function: RTDExStopPcie
*************************************************************************//**
*
* Stop an RTDEx transfer.
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
*
* @return
*      Success or error code.
*
****************************************************************************/
static _inline adp_result_t RTDExStopPcie( adp_handle_rtdex_conn hConnRtDex) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
* Function: RTDExSendPcie
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
static _inline int RTDExSendPcie( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, uint32_t u32MaxSize) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
* Function: RTDExReceivePcie
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
*      Maximum size of data to receive (buffer must be enaugh large for that number)
*
* @param [in] eMode
*      Receiving mode eRTDExWaitForever, eRTDExWaitTimeout or eRTDExDontWait 
*
* @param [in] uTimeoutMs
*      Time out length if receive is blocking, ms
*
* @return
*      Number of bytes received. Could be less than buffer size.
*      Could be 0, which mean that EOF was detected
*      if user try to read more than programmed transfer size.
*
****************************************************************************/
static _inline int RTDExReceivePcie( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, eRTDExTransferMode eMode, unsigned int uTimeoutMs) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
* Function: RTDExGetLastErrorPcie
*************************************************************************//**
*
* Return the last error of RTDEx functions RTDExSendPcie() or RTDExReceivePcie().
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
* @return
*      The last error code, RTDEXOK if success.
*
****************************************************************************/
static _inline adp_result_t RTDExGetLastErrorPcie( adp_handle_rtdex_conn hConnRtDex) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetTxFrameCountPcie
 ************************************************************************//**
 *
 * Retreives the number of frames transmitted by the RTDEx on the FPGA for
 * a channel FPGA to FPGA or FPGA to Host. If channel direction is Host to FPGA,
 * this value is the number of frames transmitted by the Host.
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] pVal
 *    Number of frames transmitted
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetTxFrameCountPcie( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetTxByteCountPcie
 ************************************************************************//**
 *
 * Retreives the number of bytes transmitted by the RTDEx on the FPGA for
 * a channel FPGA to FPGA or FPGA to Host. If channel direction is Host to FPGA,
 * this value is the number of bytes transmitted by the Host.
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] pVal
 *    Number of frames transmitted
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetTxByteCountPcie( adp_handle_rtdex_conn hConnRtDex, size_t *pVal) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetRxFrameCountPcie
 ************************************************************************//**
 *
 * Retrives the number of frames received of the RX side of the channel.
 * For a Host to FPGA, RX side is FPGA, for a FPGA to Host, RX side is Host.
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] pVal
 *    Number of frames
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetRxFrameCountPcie( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExEnableFifoMultiPcie
 ************************************************************************//**
 *
 * Enable or disable simultaneously the FIFOs of a list of RTDEx channels.
 * Usefull to make sure all FIFOs start sending or receiving data at same time.
 *
 * @param [in] hConnRtDex[]
 *    Array of handle(s) to RTDEx channels
 *
 * @param [in] uNbConn
 *    Number of entries of hConnRtDex array
 *
 * @param [in] uEnRx
 *    Enable (1) or disable (0) the RX-FIFO
 *
 * @param [in] uEnTx
 *    Enable (1) or disable (0) the TX-FIFO
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExEnableFifoMultiPcie( adp_handle_rtdex_conn hConnRtDex[], unsigned uNbConn, unsigned uEnRx, unsigned uEnTx) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetTxFifoStatusPcie
 ************************************************************************//**
 *
 * Determine if the TX-FIFO has overflowed during RTDEx transfer
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] puOverflow
 *    set to 1 if overflow has beend detected, else 0
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetTxFifoStatusPcie( adp_handle_rtdex_conn hConnRtDex, unsigned * puOverflow) 
{
    return RTDEXMEDIANOTSUPPORTED;
}


/****************************************************************************
 * Function: RTDExGetRxFifoStatusPcie
 ************************************************************************//**
 *
 * Determine if the RX-FIFO has underflowed during RTDEx transfer
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] puUnderflow
 *    set to 1 if underflow has beend detected, else 0
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 ***************************************************************************/ 
static _inline adp_result_t RTDExGetRxFifoStatusPcie( adp_handle_rtdex_conn hConnRtDex, unsigned * puUnderflow) 
{
    return RTDEXMEDIANOTSUPPORTED;
}

#endif  // __linux__

#ifdef __cplusplus
}
#endif

#endif // ADP_RTDEX_PCIE_H
