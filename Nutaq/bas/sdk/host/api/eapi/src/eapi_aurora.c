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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      eapi_aurora.c
 * 
 * @brief     Function definitions for the eapi aurora control
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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
#include "cce.h"
#include "bas_debug.h"
#include "bas_types.h"
#include "eapi_serialize.h"
#include "eapi_serialize.h"
#include "aurora_defines.h"
#include "eapi_aurora.h"

Result Aurora_GetVersion_send(connection_state * state, unsigned int nb, unsigned int * pVersion)
{
	Result ret;
	cmd_send(state, AURORA_GETVERSION);

	uint_send(state, nb-1);

    *pVersion = uint_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_ResetCore_send(connection_state * state, unsigned int nb)
{
	Result ret;
	cmd_send(state, AURORA_RESETCORE);

	uint_send(state, nb-1);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetChannelStatus_send(connection_state * state, unsigned int nb, unsigned int * pStatus)
{
	Result ret;
	cmd_send(state, AURORA_GETCHANNELSTATUS);

	uint_send(state, nb-1);

    *pStatus = uint_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_ResetRxFifo_send(connection_state * state, unsigned int nb)
{
	Result ret;
	cmd_send(state, AURORA_RESETRXFIFO);

	uint_send(state, nb-1);

	ret = result_recv(state);
	return ret;
}

Result Aurora_ResetTxFifo_send(connection_state * state, unsigned int nb)
{
	Result ret;
	cmd_send(state, AURORA_RESETTXFIFO);

	uint_send(state, nb-1);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetRxDataCount_send(connection_state * state, unsigned int nb, uint64_t * pValue)
{
	Result ret;
	cmd_send(state, AURORA_GETRXDATACOUNT);

	uint_send(state, nb-1);

    *pValue = uint64_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetTxDataCount_send(connection_state * state, unsigned int nb, uint64_t * pValue)
{
	Result ret;
	cmd_send(state, AURORA_GETTXDATACOUNT);

	uint_send(state, nb-1);

    *pValue = uint64_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetRxDataRate_send(connection_state * state, unsigned int nb, uint32_t * pValue)
{
	Result ret;
	cmd_send(state, AURORA_GETRXDATARATE);

	uint_send(state, nb-1);

    *pValue = uint_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetTxDataRate_send(connection_state * state, unsigned int nb, uint32_t * pValue)
{
	Result ret;
	cmd_send(state, AURORA_GETTXDATARATE);

	uint_send(state, nb-1);

    *pValue = uint_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_SetGtxTxParam_send(connection_state * state, unsigned int nb, uint8_t TxDiffCtrl, uint8_t TxPostEmphasis, uint8_t TxPreEmphasis)
{
	Result ret;
	cmd_send(state, AURORA_SETGTXTXPARAM);

	uint_send(state, nb-1);
    uchar_send(state, TxDiffCtrl);
    uchar_send(state, TxPostEmphasis);
    uchar_send(state, TxPreEmphasis);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetGtxTxParam_send(connection_state * state, unsigned int nb, uint8_t * pTxDiffCtrl, uint8_t * pTxPostEmphasis, uint8_t * pTxPreEmphasis)
{
	Result ret;
	cmd_send(state, AURORA_GETGTXTXPARAM);

	uint_send(state, nb-1);
    
    *pTxDiffCtrl = uchar_recv(state);
    *pTxPostEmphasis = uchar_recv(state);
    *pTxPreEmphasis = uchar_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_SetGtxRxParam_send(connection_state * state, unsigned int nb, uint8_t RxEqMix, uint8_t DfeTap1)
{
	Result ret;
	cmd_send(state, AURORA_SETGTXRXPARAM);

	uint_send(state, nb-1);
    uchar_send(state, RxEqMix);
    uchar_send(state, DfeTap1);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetGtxRxParam_send(connection_state * state, unsigned int nb, uint8_t * pRxEqMix, uint8_t * pDfeTap1)
{
	Result ret;
	cmd_send(state, AURORA_GETGTXRXPARAM);

	uint_send(state, nb-1);
    
    *pRxEqMix = uchar_recv(state);
    *pDfeTap1 = uchar_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetDfeEyeDacMon_send(connection_state * state, unsigned int nb, uint8_t * pLane0, uint8_t * pLane1, uint8_t * pLane2, uint8_t * pLane3)
{
	Result ret;
	cmd_send(state, AURORA_GETDFEEYEDACMON);

	uint_send(state, nb-1);
    
    *pLane0 = uchar_recv(state);
    *pLane1 = uchar_recv(state);
    *pLane2 = uchar_recv(state);
    *pLane3 = uchar_recv(state);

	ret = result_recv(state);
	return ret;
}

Result Aurora_SetChannelEnableState_send(connection_state * state, unsigned int nb, Aurora_eDirection Direction, uint8_t Status)
{
	Result ret;
	cmd_send(state, AURORA_SETCHANNELENABLESTATE);

	uint_send(state, nb-1);
    uchar_send(state, (uint8_t)Direction);
    uchar_send(state, Status);

	ret = result_recv(state);
	return ret;
}

Result Aurora_GetChannelEnableState_send(connection_state * state, unsigned int nb, Aurora_eDirection Direction, uint8_t *Status)
{
	Result ret;
	cmd_send(state, AURORA_GETCHANNELENABLESTATE);

	uint_send(state, nb-1);
    uchar_send(state, (uint8_t)Direction);

    *Status = uchar_recv(state);

	ret = result_recv(state);
	return ret;
}

