 /****************************************************************************
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
 * @file     eapi_aurora.h
 * @brief    Functions controlling the AURORA module
 *
 * $Date: 2016/04/26 14:10:31 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup AURORA
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

 #ifndef __EAPI_AURORA__
#define __EAPI_AURORA__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_debug.h"
#include "bas_types.h"
#include "aurora_defines.h"
#include "eapi_serialize.h"

/** @defgroup AURORA AURORA Module
 *
 *  AURORA module allow to transfer data between two carrier using Gigabit 
 *  transceivers.
 *
 *  @file
 *
 */

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/***************************************************************************
* Function: Aurora_GetVersion_send
************************************************************************//**
*
* Retrieve the FPGA core version of the opened Aurora instance.
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pVersion
*    Aurora FPGA core version when a success result is returned
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetVersion_send(connection_state * state, unsigned int nb, unsigned int * pVersion);


/***************************************************************************
* Function: Aurora_ResetCore_send
************************************************************************//**
*
* Reset the Xilinx Aurora FPGA core.
*
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_ResetCore_send(connection_state * state, unsigned int nb);


/***************************************************************************
* Function: Aurora_GetChannelStatus_send
************************************************************************//**
*
* Get the channel status. 
*
* The status is 1 when a valid Aurora link is etablished between two carriers.
* 
* The status is 0 when there is no physical connection between the Aurora core
* or if the signal integrity is not good enough over the physical link.
*
* When the channel is down, it automatically reset the RX and TX FIFO.
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pStatus
*    The channel status when the returned result is a success.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetChannelStatus_send(connection_state * state, unsigned int nb, unsigned int * pStatus);

/***************************************************************************
* Function: Aurora_ResetRxFifo_send
************************************************************************//**
*
* Reset the reception FIFO memory. It also resets the RX data and data rate counts.
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] status
*    The channel status when the returned result is a success.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_ResetRxFifo_send(connection_state * state, unsigned int nb);


/***************************************************************************
* Function: Aurora_ResetTxFifo_send
************************************************************************//**
*
* Reset the transmission FIFO memory. It also reset the TX data and data rate count.
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] status
*    The channel status when the returned result is a success.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_ResetTxFifo_send(connection_state * state, unsigned int nb);


/***************************************************************************
* Function: Aurora_GetRxDataCount_send
************************************************************************//**
*
* Retrieve the total number of bytes received by the Aurora core since the
* last RX FIFO reset.
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pValue
*    The data count in byte.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetRxDataCount_send(connection_state * state, unsigned int nb, uint64_t * pValue);


/***************************************************************************
* Function: Aurora_GetTxDataCount_send
************************************************************************//**
*
* Retrieve the total number of bytes received by the Aurora core since the
* last TX FIFO reset.
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pValue
*    The data count in byte.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetTxDataCount_send(connection_state * state, unsigned int nb, uint64_t * pValue);


/***************************************************************************
* Function: Aurora_GetRxDataRate_send
************************************************************************//**
*
* Retrieve the reception data rate (bytes/second) by the Aurora core.
* THis value is updated every second.
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pValue
*    The data rate in bytes/second.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetRxDataRate_send(connection_state * state, unsigned int nb, uint32_t * pValue);


/***************************************************************************
* Function: Aurora_GetTxDataRate_send
************************************************************************//**
*
* Retrieve the transmission data rate (bytes/second) by the Aurora core.
* THis value is updated every second.
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pValue
*    The data rate in bytes/second.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetTxDataRate_send(connection_state * state, unsigned int nb, uint32_t * pValue);


/***************************************************************************
* Function: Aurora_SetGtxTxParam_send
************************************************************************//**
*
* Set the GTX parameters in transmission. 
*
* See Xilinx UG366, Table 3-31: TX Configurable Driver Ports for more information
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [in] TxDiffCtrl
*    Driver Swing Control. Value from 0 to 15
*
* @param [in] TxPostEmphasis
*    Transmitter Post-Cursor TX Pre-Emphasis Control. Value from 0 to 31
*
* @param [in] TxPreEmphasis
*    Transmitter Pre-Cursor TX Pre-Emphasis Control. Value from 0 to 15
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_SetGtxTxParam_send(connection_state * state, unsigned int nb, uint8_t TxDiffCtrl, uint8_t TxPostEmphasis, uint8_t TxPreEmphasis);

/***************************************************************************
* Function: Aurora_GetGtxTxParam_send
************************************************************************//**
*
* Get the GTX parameters in transmission. 
*
* See Xilinx UG366, Table 3-31: TX Configurable Driver Ports for more information
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pTxDiffCtrl
*    Driver Swing Control. Value from 0 to 15
*
* @param [out] pTxPostEmphasis
*    Transmitter Post-Cursor TX Pre-Emphasis Control. Value from 0 to 31
*
* @param [out] pTxPreEmphasis
*    Transmitter Pre-Cursor TX Pre-Emphasis Control. Value from 0 to 15
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetGtxTxParam_send(connection_state * state, unsigned int nb, uint8_t * pTxDiffCtrl, uint8_t * pTxPostEmphasis, uint8_t * pTxPreEmphasis);

/***************************************************************************
* Function: Aurora_SetGtxRxParam_send
************************************************************************//**
*
* Set the GTX parameters in reception. 
*
* See Xilinx UG366, Table 4-11: RX Equalizer Ports for more information
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [in] RxEqMix
*    Receiver Equalization Control. Value from 0 to 7
*
* @param [in] DfeTap1
*    DFE tap 1 weight value control. Value from 0 to 31
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_SetGtxRxParam_send(connection_state * state, unsigned int nb, uint8_t RxEqMix, uint8_t DfeTap1);


/***************************************************************************
* Function: Aurora_GetGtxRxParam_send
************************************************************************//**
*
* Get the GTX parameters in reception. 
*
* See Xilinx UG366, Table 4-11: RX Equalizer Ports for more information
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pRxEqMix
*    Receiver Equalization Control. Value from 0 to 7
*
* @param [out] pDfeTap1
*    DFE tap 1 weight value control. Value from 0 to 31
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetGtxRxParam_send(connection_state * state, unsigned int nb, uint8_t * pRxEqMix, uint8_t * pDfeTap1);

/***************************************************************************
* Function: Aurora_GetDfeEyeDacMon_send
************************************************************************//**
*
* Averaged Vertical Eye Height (voltage domain) used by the
* DFE as an optimization criterion.
*
* See Xilinx UG366, Table 4-11: RX Equalizer Ports for more information
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [out] pLane0
*    Averaged Vertical Eye Height of the first lane. Value from 0 to 31
*
* @param [out] pLane1
*    Averaged Vertical Eye Height of the second lane. Value from 0 to 31
*
* @param [out] pLane2
*    Averaged Vertical Eye Height of the third lane. Value from 0 to 31
*
* @param [out] pLane3
*    Averaged Vertical Eye Height of the fourth lane. Value from 0 to 31
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetDfeEyeDacMon_send(connection_state * state, unsigned int nb, uint8_t * pLane0, uint8_t * pLane1, uint8_t * pLane2, uint8_t * pLane3);

/***************************************************************************
* Function: Aurora_SetChannelEnableState_send
************************************************************************//**
*
* Set the state of an Aurora channel
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [in] Direction to enable/disable
*    eAuroraDirectionRx : Rx direction (data coming to the Aurora core)
     eAuroraDirectionTx : Tx direction (data coming from the Aurora core)
*
* @param [in] Status
*    1 : Enable the channel.
*    0 : Disable the channel.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_SetChannelEnableState_send(connection_state * state, unsigned int nb, Aurora_eDirection Direction, uint8_t Status);

/***************************************************************************
* Function: Aurora_GetChannelEnableState_send
************************************************************************//**
*
* Get the state of an Aurora channel
* 
* @param[in] state 
*    Pointer to the connection state structure that handles the TCP connection between the host and the carrier platform.
*
* @param [in] nb
*    Aurora instance number to target. 
*    nb starts at 1.
*
* @param [in] Direction to enable/disable
*    eAuroraDirectionRx : Rx direction (data coming to the Aurora core)
     eAuroraDirectionTx : Tx direction (data coming from the Aurora core)
*
* @param [out] Status
*    1 : The channel is enabled.
*    0 : The channel is disabled.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetChannelEnableState_send(connection_state * state, unsigned int nb, Aurora_eDirection Direction, uint8_t *Status);

#ifdef __cplusplus
}
#endif // extern "C"

#endif

