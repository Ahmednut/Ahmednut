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
 * @file      aurora.c
 * 
 * @brief     Functions controlling the AURORA module
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

#include <string.h>

#include "bas_types.h"
#include "bas_debug.h"
#include "aurora.h"
#include "aurora_defines.h"
#include "aurora_regs.h"


/****************************************************************************
 *                            Private functions                             *
 ****************************************************************************/
void _Aurora_WriteReg( aurora_core * aurora,
                       uint16_t      reg,
                       uint32_t      val )
{
    aurora->registers->RegCtrl.Bit.RegWrEn = 0;
    aurora->pfUsleep(1);

    aurora->registers->RegCtrl.Bit.RegIdx = reg;
    aurora->registers->RegData = val;
    aurora->pfUsleep(1);

    aurora->registers->RegCtrl.Bit.RegWrEn = 1;
    aurora->pfUsleep(1);
    aurora->registers->RegCtrl.Bit.RegWrEn = 0;

    return;
}

void _Aurora_ReadReg( aurora_core * aurora,
                      uint16_t      reg,
                      uint32_t    * pu32Val )
{
    aurora->registers->RegCtrl.Bit.RegWrEn = 0;
    aurora->pfUsleep(1);

    aurora->registers->RegCtrl.Bit.RegIdx = reg;
    aurora->pfUsleep(1);

    *pu32Val = aurora->registers->RegData;

    return;
}

/****************************************************************************
 *                            Public functions                              *
 ****************************************************************************/
Result Aurora_Open(aurora_core * aurora, unsigned int uiCoreBaseAddr, int (*pfUsleep)(unsigned int usec))
{
    aurora_IndRegister_Map IndReg;

    memset(aurora, 0, sizeof(aurora_core));

    aurora->registers = (aurora_Register_Map *)uiCoreBaseAddr;
    aurora->pfUsleep = pfUsleep;

    _Aurora_ReadReg(aurora, COREID_REG_IDX, &IndReg.CoreID.raw);
    if(IndReg.CoreID.Bit.CoreID != 0xAAAA) return AURORA_INVALID_CORE;

    aurora->ucIsOpen = 1;

    return AURORAOK;
}

Result Aurora_GetVersion(aurora_core * aurora, unsigned int * puiVersion)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, COREID_REG_IDX, &IndReg.CoreID.raw);
    if(IndReg.CoreID.Bit.CoreID != 0xAAAA) return AURORA_INVALID_CORE;

    *puiVersion = IndReg.CoreID.Bit.Core_Version;

    return AURORAOK;
}

Result Aurora_ResetCore(aurora_core * aurora)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GLOBAL_REGISTERS_REG_IDX, &IndReg.Global_Registers.raw);

    IndReg.Global_Registers.Bit.Aurora_Reset = 1;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);
    aurora->pfUsleep(10);

    IndReg.Global_Registers.Bit.Aurora_Reset = 0;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);

    return AURORAOK;
}

Result Aurora_GetChannelStatus(aurora_core * aurora, unsigned int * status)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GLOBAL_REGISTERS_REG_IDX, &IndReg.Global_Registers.raw);
    *status = IndReg.Global_Registers.Bit.Aurora_Channel_Up;
    return AURORAOK;
}

Result Aurora_ResetRxFifo(aurora_core * aurora)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GLOBAL_REGISTERS_REG_IDX, &IndReg.Global_Registers.raw);

    IndReg.Global_Registers.Bit.RX_Fifo_Reset = 1;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);
    aurora->pfUsleep(1);

    IndReg.Global_Registers.Bit.RX_Fifo_Reset = 0;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);

    return AURORAOK;
}

Result Aurora_ResetTxFifo(aurora_core * aurora)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GLOBAL_REGISTERS_REG_IDX, &IndReg.Global_Registers.raw);

    IndReg.Global_Registers.Bit.TX_Fifo_Reset = 1;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);
    aurora->pfUsleep(1);

    IndReg.Global_Registers.Bit.TX_Fifo_Reset = 0;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);

    return AURORAOK;
}

Result Aurora_GetRxDataCount(aurora_core * aurora, uint64_t * value)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GLOBAL_REGISTERS_REG_IDX, &IndReg.Global_Registers.raw);

    IndReg.Global_Registers.Bit.Latch_Data_Count = 1;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);
    aurora->pfUsleep(1);

    IndReg.Global_Registers.Bit.Latch_Data_Count = 0;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);
    aurora->pfUsleep(1);

    _Aurora_ReadReg(aurora, RECEIVE_DATA_COUNT_MSW_REG_IDX, &IndReg.Receive_Data_Count_MSW.raw);
    *value = ((uint64_t)IndReg.Receive_Data_Count_MSW.raw) << 32;

    _Aurora_ReadReg(aurora, RECEIVE_DATA_COUNT_LSW_REG_IDX, &IndReg.Receive_Data_Count_LSW.raw);
    *value |= (uint64_t)IndReg.Receive_Data_Count_LSW.raw;

    return AURORAOK;
}

Result Aurora_GetTxDataCount(aurora_core * aurora, uint64_t * value)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GLOBAL_REGISTERS_REG_IDX, &IndReg.Global_Registers.raw);

    IndReg.Global_Registers.Bit.Latch_Data_Count = 1;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);
    aurora->pfUsleep(1);

    IndReg.Global_Registers.Bit.Latch_Data_Count = 0;
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);
    aurora->pfUsleep(1);

    _Aurora_ReadReg(aurora, TRANSMIT_DATA_COUNT_MSW_REG_IDX, &IndReg.Transmit_Data_Count_MSW.raw);
    *value = ((uint64_t)IndReg.Transmit_Data_Count_MSW.raw) << 32;

    _Aurora_ReadReg(aurora, TRANSMIT_DATA_COUNT_LSW_REG_IDX, &IndReg.Transmit_Data_Count_LSW.raw);
    *value |= (uint64_t)IndReg.Transmit_Data_Count_LSW.raw;

    return AURORAOK;
}

Result Aurora_GetRxDataRate(aurora_core * aurora, uint32_t * value)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, RECEIVE_DATA_RATE_REG_IDX, &IndReg.Receive_Data_Rate.raw);
    *value = IndReg.Receive_Data_Rate.raw;

    return AURORAOK;
}

Result Aurora_GetTxDataRate(aurora_core * aurora, uint32_t * value)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, TRANSMIT_DATA_RATE_REG_IDX, &IndReg.Transmit_Data_Rate.raw);
    *value = IndReg.Transmit_Data_Rate.raw;

    return AURORAOK;
}

Result Aurora_SetGtxTxParam(aurora_core * aurora, uint8_t TxDiffCtrl, uint8_t TxPostEmphasis, uint8_t TxPreEmphasis)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;
    
    if(TxDiffCtrl >= (1<<4) ||
        TxPostEmphasis >= (1<<5) ||
        TxPreEmphasis >= (1<<4))
    {
        return AURORA_INVALID_ARG;
    }

    _Aurora_ReadReg(aurora, GTX_PARAMETER_REG_IDX, &IndReg.GTX_Parameter.raw);

    IndReg.GTX_Parameter.Bit.txdiffctrl = TxDiffCtrl;
    IndReg.GTX_Parameter.Bit.txpostemphasis = TxPostEmphasis;
    IndReg.GTX_Parameter.Bit.txpreemphasis = TxPreEmphasis;
    
    _Aurora_WriteReg(aurora, GTX_PARAMETER_REG_IDX, IndReg.GTX_Parameter.raw);

    return AURORAOK;
}

Result Aurora_GetGtxTxParam(aurora_core * aurora, uint8_t * pTxDiffCtrl, uint8_t * pTxPostEmphasis, uint8_t * pTxPreEmphasis)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GTX_PARAMETER_REG_IDX, &IndReg.GTX_Parameter.raw);

    *pTxDiffCtrl = IndReg.GTX_Parameter.Bit.txdiffctrl;
    *pTxPostEmphasis = IndReg.GTX_Parameter.Bit.txpostemphasis;
    *pTxPreEmphasis = IndReg.GTX_Parameter.Bit.txpreemphasis;

    return AURORAOK;
}

Result Aurora_SetGtxRxParam(aurora_core * aurora, uint8_t RxEqMix, uint8_t DfeTap1)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;
    
    if(RxEqMix >= (1<<3) ||
        DfeTap1 >= (1<<5))
    {
        return AURORA_INVALID_ARG;
    }

    _Aurora_ReadReg(aurora, GTX_PARAMETER_REG_IDX, &IndReg.GTX_Parameter.raw);

    IndReg.GTX_Parameter.Bit.rxeqmix = RxEqMix;
    IndReg.GTX_Parameter.Bit.dfetap1 = DfeTap1;
    
    _Aurora_WriteReg(aurora, GTX_PARAMETER_REG_IDX, IndReg.GTX_Parameter.raw);

    return AURORAOK;
}

Result Aurora_GetGtxRxParam(aurora_core * aurora, uint8_t * pRxEqMix, uint8_t * pDfeTap1)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GTX_PARAMETER_REG_IDX, &IndReg.GTX_Parameter.raw);

    *pRxEqMix = IndReg.GTX_Parameter.Bit.rxeqmix;
    *pDfeTap1 = IndReg.GTX_Parameter.Bit.dfetap1;

    return AURORAOK;
}

Result Aurora_GetDfeEyeDacMon(aurora_core * aurora, uint8_t * pLane0, uint8_t * pLane1, uint8_t * pLane2, uint8_t * pLane3)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GTX_DFEEYEDACMON_REG_IDX, &IndReg.GTX_DfeEyeDacMon.raw);
    *pLane0 = IndReg.GTX_DfeEyeDacMon.Bit.lane0;
    *pLane1 = IndReg.GTX_DfeEyeDacMon.Bit.lane1;
    *pLane2 = IndReg.GTX_DfeEyeDacMon.Bit.lane2;
    *pLane3 = IndReg.GTX_DfeEyeDacMon.Bit.lane3;

    return AURORAOK;
}

Result Aurora_Close(aurora_core * aurora)
{
	if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    aurora->ucIsOpen = 0;

    return AURORAOK;
}

Result Aurora_SetChannelEnableState(aurora_core * aurora, Aurora_eDirection Direction, uint8_t State)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GLOBAL_REGISTERS_REG_IDX, &IndReg.Global_Registers.raw);

    if(Direction == eAuroraDirectionRx)
    {
        IndReg.Global_Registers.Bit.RX_Enable = State;
    }
    else
    {
        IndReg.Global_Registers.Bit.TX_Enable = State;
    }
    _Aurora_WriteReg(aurora, GLOBAL_REGISTERS_REG_IDX, IndReg.Global_Registers.raw);

    return AURORAOK;
}

Result Aurora_GetChannelEnableState(aurora_core * aurora, Aurora_eDirection Direction, uint8_t *State)
{
    aurora_IndRegister_Map IndReg;

    if(aurora->ucIsOpen == 0) return AURORA_NOT_OPEN;

    _Aurora_ReadReg(aurora, GLOBAL_REGISTERS_REG_IDX, &IndReg.Global_Registers.raw);
    if(Direction == eAuroraDirectionRx)
    {
        *State = IndReg.Global_Registers.Bit.RX_Enable;
    }
    else
    {
        *State = IndReg.Global_Registers.Bit.TX_Enable;
    }
    
    return AURORAOK;
}

