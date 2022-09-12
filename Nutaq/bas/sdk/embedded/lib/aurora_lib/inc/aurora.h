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
 * @file       aurora.h
 * @brief      Functions controlling the AURORA module
 *
 * $Date: 2016/04/22 20:01:29 $
 * $Revision: 1.5 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup AURORA
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

#ifndef __AURORA__
#define __AURORA__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
 
#include "bas_types.h"
#include "bas_error.h"
#include "aurora_defines.h"
#include "aurora_regs.h"

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
* Function: Aurora_Open
************************************************************************//**
*
* Create a handle for use in the Aurora API.
*
* @param [in,out] aurora
*    Pointer to aurora_core struct
*
* @param [in] uiCoreBaseAddr
*    Aurora FPGA core base address
*
* @param [in] pfUsleep
*      Pointer to a sleep function that sleep during N microsecond.
*      This function is the one that will be used inside the library functions.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_Open(aurora_core * aurora, unsigned int uiCoreBaseAddr, int (*pfUsleep)(unsigned int usec));


/***************************************************************************
* Function: Aurora_GetVersion
************************************************************************//**
*
* Retrieve the FPGA core version of the opened Aurora instance.
*
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] puiVersion
*    Aurora FPGA core version when a success result is returned
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetVersion(aurora_core * aurora, unsigned int * puiVersion);


/***************************************************************************
* Function: Aurora_ResetCore
************************************************************************//**
*
* Reset the Xilinx Aurora FPGA core.
*
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_ResetCore(aurora_core * aurora);


/***************************************************************************
* Function: Aurora_GetChannelStatus
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
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] status
*    The channel status when the returned result is a success.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetChannelStatus(aurora_core * aurora, unsigned int * status);


/***************************************************************************
* Function: Aurora_ResetRxFifo
************************************************************************//**
*
* Reset the reception FIFO memory. It also reset the RX data and data rate count.
* 
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] status
*    The channel status when the returned result is a success.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_ResetRxFifo(aurora_core * aurora);


/***************************************************************************
* Function: Aurora_ResetTxFifo
************************************************************************//**
*
* Reset the transmission FIFO memory. It also reset the TX data and data rate count.
* 
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] status
*    The channel status when the returned result is a success.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_ResetTxFifo(aurora_core * aurora);


/***************************************************************************
* Function: Aurora_GetRxDataCount
************************************************************************//**
*
* Retrieve the total number of bytes received by the Aurora core since the
* last RX FIFO reset.
* 
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] value
*    The data count in byte.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetRxDataCount(aurora_core * aurora, uint64_t * value);


/***************************************************************************
* Function: Aurora_GetTxDataCount
************************************************************************//**
*
* Retrieve the total number of bytes received by the Aurora core since the
* last TX FIFO reset.
* 
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] value
*    The data count in byte.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetTxDataCount(aurora_core * aurora, uint64_t * value);


/***************************************************************************
* Function: Aurora_GetRxDataRate
************************************************************************//**
*
* Retrieve the reception data rate (bytes/second) by the Aurora core.
* This value is updated every second.
* 
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] value
*    The data rate in bytes/second.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetRxDataRate(aurora_core * aurora, uint32_t * value);


/***************************************************************************
* Function: Aurora_GetTxDataRate
************************************************************************//**
*
* Retrieve the transmission data rate (bytes/second) by the Aurora core.
* This value is updated every second.
* 
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] value
*    The data rate in bytes/second.
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetTxDataRate(aurora_core * aurora, uint32_t * value);


/***************************************************************************
* Function: Aurora_SetGtxTxParam
************************************************************************//**
*
* Set the GTX parameters in transmission. 
*
* See Xilinx UG366, Table 3-31: TX Configurable Driver Ports for more information
* 
* @param [in] aurora
*    Pointer to aurora_core struct
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
Result Aurora_SetGtxTxParam(aurora_core * aurora, uint8_t TxDiffCtrl, uint8_t TxPostEmphasis, uint8_t TxPreEmphasis);


/***************************************************************************
* Function: Aurora_GetGtxTxParam
************************************************************************//**
*
* Get the GTX parameters in transmission. 
*
* See Xilinx UG366, table TX Configurable Driver Ports for more information
* 
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] TxDiffCtrl
*    Driver Swing Control. Value from 0 to 15
*
* @param [out] TxPostEmphasis
*    Transmitter Post-Cursor TX Pre-Emphasis Control. Value from 0 to 31
*
* @param [out] TxPreEmphasis
*    Transmitter Pre-Cursor TX Pre-Emphasis Control. Value from 0 to 15
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetGtxTxParam(aurora_core * aurora, uint8_t * pTxDiffCtrl, uint8_t * pTxPostEmphasis, uint8_t * pTxPreEmphasis);


/***************************************************************************
* Function: Aurora_SetGtxRxParam
************************************************************************//**
*
* Set the GTX parameters in reception. 
*
* See Xilinx UG366, table RX Equalizer Ports for more information
* 
* @param [in] aurora
*    Pointer to aurora_core struct
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
Result Aurora_SetGtxRxParam(aurora_core * aurora, uint8_t RxEqMix, uint8_t DfeTap1);


/***************************************************************************
* Function: Aurora_GetGtxRxParam
************************************************************************//**
*
* Get the GTX parameters in reception. 
*
* See Xilinx UG366, Table 4-11: RX Equalizer Ports for more information
* 
* @param [in] aurora
*    Pointer to aurora_core struct
*
* @param [out] RxEqMix
*    Receiver Equalization Control. Value from 0 to 7
*
* @param [out] DfeTap1
*    DFE tap 1 weight value control. Value from 0 to 31
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_GetGtxRxParam(aurora_core * aurora, uint8_t * pRxEqMix, uint8_t * pDfeTap1);


/***************************************************************************
* Function: Aurora_GetDfeEyeDacMon
************************************************************************//**
*
* Averaged Vertical Eye Height (voltage domain) used by the
* DFE as an optimization criterion.
*
* See Xilinx UG366, table RX Equalizer Ports for more information
* 
* @param [in] aurora
*    Pointer to aurora_core struct
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
Result Aurora_GetDfeEyeDacMon(aurora_core * aurora, uint8_t * pLane0, uint8_t * pLane1, uint8_t * pLane2, uint8_t * pLane3);

/***************************************************************************
* Function: Aurora_Close
************************************************************************//**
*
* Close the Aurora core handle
*
* @param [in,out] aurora
*    Pointer to aurora_core struct
*
* @return
*      Return success or failure result. See aurora_error enum.
*
***************************************************************************/
Result Aurora_Close(aurora_core * aurora);

Result Aurora_SetChannelEnableState(aurora_core * aurora, Aurora_eDirection Direction, uint8_t State);
Result Aurora_GetChannelEnableState(aurora_core * aurora, Aurora_eDirection Direction, uint8_t *state);

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

