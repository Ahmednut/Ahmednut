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
 * @file    rtdex_eth.h
 * @brief   Functions definition for RTDEx access via Ethernet.
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
 

#ifndef ADP_RTDEX_ETH_H
#define ADP_RTDEX_ETH_H

#include "rtdex_eth_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @name MAC address source
 * 
 *  Possible RTDEx Ethernet MAC address source (Host/FPGA)
 */
typedef enum
{
  eHostMacAddress, ///< Host MAC address
  eFpgaMacAddress  ///< FPGA MAC address
}
eMacAddrSrc;


/****************************************************************************
 * Function: RTDExOpenEth
 ************************************************************************//**
 *
 * Open an RTDEx channel instance 
 * When the module is not used anymore, it can be closed with RTDEx_close().
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] pPeerMacAddr
 *    Peer MAC address (Host or Perseus) 
 *
 * @param [in] ePeerType
 *    Peer type, eHost or ePerseus
 *
 * @param [in] pFpgaMacAddr
 *    FPGA MAC address
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [in] uNumChannel
 *    Number of the channel to be openedeDir
 *
 * @param [in] eDir
 *    Direction of the transfer, to FPGA or from FPGA
 *
 * @param [in] eMode
 *    Transfer mode, single or continuous
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 * 
 *
 * @see
 *    RTDExCloseEth
 *
 ***************************************************************************/ 
// old interface - deprecated
adp_result_t RTDExOpenEth(  adp_handle_rtdex_conn *hConn,
                            unsigned char pPeerMacAddr[ 6], // peer MAC address (Host or Persus)
                            ePeerEndpointType ePeerType,  // eHost or ePerseus
                            unsigned char pFpgaMacAddr[6], // fpga MAC address
               			    connection_state * state,
                            uint64_t u64RTDExFpgaBaseAddr,
                            unsigned int uNumChannel,
                            eRTDExTransferDir eDir, // eToFpga or eFromFpga
                            eRTDExMode eMode); // eSingle, eContinuous


/****************************************************************************
 * Function: RTDExOpenHostAndFpgaEth
 ************************************************************************//**
 *
 * Open an RTDEx channel instance for data exchange between a Perseus ans a Host.
 * When the channel is not used anymore, it can be closed with RTDExCloseEth().
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [in] uChannelNo
 *    Number of the channel to be openedeDir
 *
 * @param [in] eDir
 *    Direction of the transfer, to FPGA or from FPGA
 *
 * @param [in] eMode
 *    Transfer mode, single or continuous
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 * 
 *
 * @see
 *    RTDExCloseEth
 *
 ***************************************************************************/ 
// new interface
adp_result_t RTDExOpenHostAndFpgaEth( adp_handle_rtdex_conn *hConn,
                                      connection_state * state,
                                      uint64_t u64RTDExFpgaBaseAddr,
                                      unsigned int uChannelNo,
                                      eRTDExTransferDir eDir, // eToFpga or eFromFpga
                                      eRTDExMode eMode); // eSingle, eContinuous


/****************************************************************************
 * Function: RTDExOpenHostAndFpgaEthTo
 ************************************************************************//**
 *
 * Open an RTDEx channel instance for data exchange between a Perseus ans a Host.
 * When the channel is not used anymore, it can be closed with RTDExCloseEth().
 * This routine is similar to RTDExOpenHostAndFpgaEth excepted that the PCAD
 * read timeout can be configured.
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [in] uChannelNo
 *    Number of the channel to be openedeDir
 *
 * @param [in] eDir
 *    Direction of the transfer, to FPGA or from FPGA
 *
 * @param [in] eMode
 *    Transfer mode, single or continuous
 *
 * @param [in] pcapReadTo
 *    Specifies the PCAP read timeout in milliseconds.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 * 
 *
 * @see
 *    RTDExCloseEth
 *
 ***************************************************************************/ 
// new interface
adp_result_t RTDExOpenHostAndFpgaEthTo( adp_handle_rtdex_conn *hConn,
                                        connection_state * state,
                                        uint64_t u64RTDExFpgaBaseAddr,
                                        unsigned int uChannelNo,
                                        eRTDExTransferDir eDir, // eToFpga or eFromFpga
                                        eRTDExMode eMode, // eSingle, eContinuous
                                        int pcapReadTo );

/****************************************************************************
 * Function: RTDExOpenFpga2FpgaEth
 ************************************************************************//**
 *
 * Open an RTDEx channel instance for data exchange between two Perseus.
 * When the channel is not used anymore, it can be closed with RTDExCloseEth().
 *
 * @param [in,out] hConn
 *    This structure will hold necessary information for the module to be used.
 *    An empty structure is provided and it is filled before returning.
 *
 * @param [in] SrcState
 *    Handle to the connection state object used for sending RTDEx core access
 *
 * @param [in] DstState
 *    Handle to the connection state object used for receiving RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus (source & destination must be same)
 *
 * @param [in] uChannelNo
 *    Number of the channel to be openedeDir
 *
 * @param [in] eMode
 *    Transfer mode, single or continuous
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 * 
 *
 * @see
 *    RTDExCloseEth
 *
 ***************************************************************************/ 
// new interface
adp_result_t RTDExOpenFpga2FpgaEth( adp_handle_rtdex_conn *hConn,
                                    connection_state * SrcState,
                                    connection_state * DstState,
                                    uint64_t u64RTDExFpgaBaseAddr,
                                    unsigned int uChannelNo,
                                    eRTDExMode eMode); // eSingle, eContinuous

/****************************************************************************
 * Function: RTDExCloseEth
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
adp_result_t RTDExCloseEth( adp_handle_rtdex_conn hConnRtDex );


/****************************************************************************
* Function: RTDExStartEth
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
* @param [in] u32TransferSize
*      Total size of the current transfer (in bytes).
*       Cannot be higher than the size of the buffer specified in the RTDExOpen uBufferSize parameters.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExStartEth( adp_handle_rtdex_conn hConnRtDex, uint32_t u32FrameSize, uint32_t u32TransferSize);


/****************************************************************************
* Function: RTDExSendEth
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
int RTDExSendEth( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize);


/****************************************************************************
* Function: RTDExReceiveEth
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
int RTDExReceiveEth( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, 
                     eRTDExTransferMode eMode, unsigned int uTimeOut );


/****************************************************************************
* Function: RTDExGetLastErrorEth
*************************************************************************//**
*
* Return the last error of RTDEx functions RTDExSendEth() or RTDExReceiveEth().
*
* @param [in]  hConnRtDex
*      Handle to an RTDEx object.
* @return
*      The last error code, RTDEXOK if success.
*
****************************************************************************/
adp_result_t RTDExGetLastErrorEth( adp_handle_rtdex_conn hConnRtDex);


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
void RTDExSetLastErrorEth( adp_handle_rtdex_conn hConnRtDex, adp_result_t Result);

/****************************************************************************
* Function: RTDExResetFifoEth
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
adp_result_t RTDExResetFifoEth( adp_handle_rtdex_conn hConnRtDex);



/****************************************************************************
 * Function: RTDExGetTxFramesCountEth
 ************************************************************************//**
 *
 * Retrieves the number of frames transmitted by the RTDEx on the FPGA for
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
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxFramesCountEth( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal);


/****************************************************************************
 * Function: RTDExGetRxFramesCountEth
 ************************************************************************//**
 *
 * Retrieves the number of frames received of the RX side of the channel.
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
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxFramesCountEth( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal);

/****************************************************************************
 * Function: RTDExGetRxFramesLostCountEth
 ************************************************************************//**
 *
 * Retrieves the number of frames lost (based of built-in sequence numbering)
 * of the RX side of the channel.
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
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxFramesLostCountEth( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal);


/****************************************************************************
 * Function: RTDExGetRxDropFramesCntEth
 ************************************************************************//**
 *
 * Retrieves the number of frames dropped of the RX side of the channel.
 * Frames drop occurs when the RTDEx receiver fifos are full and more
 * data arrives to RTDEx. Only FPGA may drop frames intentionally, never Host.
 * For a Host to FPGA, RX side is FPGA, for a FPGA to Host, RX side is Host.
 *
 * @param [in] hConnRtDex
 *    Handle to the channel instance
 *
 * @param [out] pu32Cnt
 *    Number of frames
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxDropFramesCntEth( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32Cnt);


/****************************************************************************
 * Function: RTDExGetTxFifoDataCntEth
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
adp_result_t RTDExGetTxFifoDataCntEth( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32DataCount);


// per core functions:

/****************************************************************************
 * Function: RTDExGetCoreIdVerEth
 ************************************************************************//**
 *
 * Get the RTDEx Core ID and Version number.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [out] puId
 *    Core Id number
 *
 * @param [out] puVer
 *    Core Version number
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 *
 ***************************************************************************/ 
adp_result_t RTDExGetCoreIdVerEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t * puId, uint32_t * puVer);


/****************************************************************************
 * Function: RTDExResetCoreEth
 ************************************************************************//**
 *
 * Reset the RTDEx core. All registers are reset to their default values.
 * This function should be called first, before calling any other
 * functions of the library.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 * 
 ***************************************************************************/ 
adp_result_t RTDExResetCoreEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr);



/****************************************************************************
 * Function: RTDExGetRxNbChannelsEth
 ************************************************************************//**
 *
 * Retrieves the number of RX channel of the RTDEx core.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64BaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [out] pNbChannels
 *    Number of RX channels
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxNbChannelsEth( connection_state * state, uint64_t u64BaseAddr, unsigned int* pNbChannels);


/****************************************************************************
 * Function: RTDExGetTxNbChannelsEth
 ************************************************************************//**
 *
 * Retrieves the number of TX channel of the RTDEx core.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64BaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [out] pNbChannels
 *    Number of TX channels
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxNbChannelsEth( connection_state * state, uint64_t u64BaseAddr, unsigned int* pNbChannels);


/****************************************************************************
 * Function: RTDExGetRxConfigExtentedEth
 ************************************************************************//**
 *
 * Retrieves the RX channel FIFO depth, and a flag indicating if statistics are included
 * in the RTDEx core.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [out] puFifoDepth
 *    Fifo size, in bytes
 *
 * @param [out] puStatsIsIncluded
 *    1 if statistics are included, else 0.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxConfigExtentedEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded);


/****************************************************************************
 * Function: RTDExGetTxConfigExtentedEth
 ************************************************************************//**
 *
 * Retrieves the TX channel FIFO depth, and a flag indicating if statistics are included
 * in the RTDEx core.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [out] puFifoDepth
 *    Fifo size, in bytes
 *
 * @param [out] puStatsIsIncluded
 *    1 if statistics are included, else 0.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetTxConfigExtentedEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded);


/****************************************************************************
 * Function: RTDExSetTxFrameGapValEth
 ************************************************************************//**
 *
 * Set the interval time to insert between two transmitted Ethernet frame by RTDEx.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [in] u32FrameGap
 *    The interval time count, in unit of 10ns. ???
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExSetTxFrameGapValEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t u32FrameGap);


/****************************************************************************
 * Function: RTDExSetTxFrameBurstEth
 ************************************************************************//**
 *
 * Set the maximum number of Ethernet frames that could be send in line for a channel,
 * before changing to another channel
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64RTDExFpgaBaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [in] u32Burst
 *    The number of frames, 1 to 255
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExSetTxFrameBurstEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t u32Burst);



/****************************************************************************
 * Function: RTDExGetRxBadFramesCntEth
 ************************************************************************//**
 *
 * Return the number of Ethernet frames received by RTDEx consider as invalid or badly formated.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64BaseAddr
 *    RTDEx module base address on the axi bus
 *
 * @param [out] pu32Cnt
 *    The number of bad frames.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetRxBadFramesCntEth( connection_state * state, uint64_t u64BaseAddr, uint32_t* pu32Cnt);



/****************************************************************************
 * Function: RTDExEnableFlowControlEth
 ************************************************************************//**
 *
 * Set the FIFO level to start the Ethernet Flow Control.
 *
 * @param [in] state
 *    Handle to the connection state object used for RTDEx core access
 *
 * @param [in] u64Address
 *    RTDEx module base address on the axi bus
 *
 * @param [in] uFifoFull
 *    RX FIFO fill size to reach (increasing) before sending PAUSE handshake
 *
 * @param [in] uFifoEmpty
 *    RX FIFO fill size to reach (decreasing) before sending END of PAUSE handshake
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExEnableFlowControlEth( connection_state * state, uint64_t u64Address, unsigned uFifoFull, unsigned uFifoEmpty);


/****************************************************************************
 * Function: RTDExGetMacEth
 ************************************************************************//**
 *
 * This is helper function to retrieve the MAC address of the Host.
 * Call this function repeatedly with uIndex varying from 0 to n to get the MAC address
 * and the IP address of an adapter present on the Host.
 *
 * NOTE: This function require 'Super-User' privilege to execute.
 *
 * @param [in] uIndex
 *    Index (from 0 to n) of adapter information to get from.
 *
 * @param [out] pHostMacAddr
 *    Array of 6 bytes of the MAC address
 *
 * @param [out] pHostIpAddr
 *    Array of 16 characters of the IP address, ASCII string.
 *
 * @param [out] pDescription
 *    Array of 261 characters of the adapter description, ASCII string.
 *
 * @return
 *    Error code: #RTDEXOK if no problem is found, !=#RTDEXOK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occurred. 
 ***************************************************************************/ 
adp_result_t RTDExGetMacEth( unsigned int uIndex, unsigned char pHostMacAddr[6], char pHostIpAddr[16], char pDescription[261]);

#ifdef __cplusplus
}
#endif

#endif
