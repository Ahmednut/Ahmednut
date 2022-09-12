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
 * @file       cce_mo1000.c
 * @brief      cce mo1000 module related functions
 *
 * $Date: 2015/05/19 15:08:27 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MO1000_CCE
 *
 * This file is part of Nutaq’s ADP Software Suite.
 *
 * You may at your option receive a license to Nutaq’s ADP Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s ADP Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s ADP Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s ADP Software Suite under the GPL requires that your work based on
 * Nutaq’s ADP Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s ADP Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s ADP Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <unistd.h>
#include <string.h>

#include "eapi_serialize.h"
#include "cce.h"
#include "bas_debug.h"

#include "cce_mo1000.h"
#include "Mo1000.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

#ifdef DEBUG
// Uncomment following line for debug
#define MO1000CCE_DEBUG 1
#endif


//debug or no debug
#ifdef MO1000CCE_DEBUG
#include <stdio.h>
#include <stdlib.h>
#define mdebug(f, ...) printf(f, __VA_ARGS__)
#else
#define mdebug(f, ...)
#endif

extern Carrier_handle_t hCarrier;
extern Mo1000_stMo1000 Mo1000Board[];

/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/

static FMCEEprom_module_types _ModList[]={FMCEEPROM_EEPROM_MOD_MO1000, FMCEEPROM_EEPROM_MOD_MO1000E};


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/
 
 Result _Mo1000_Presence(unsigned int pos)
{
    Result res;
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;

    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if (FAILURE(res)) return res;
    if (pos >= uiFmcMaxNum) return CARRIER_INVALID_MODULE_INDEX;

    iBoard = pos % CARRIER_FMC_MAXBOARD;
    iConnector = pos / CARRIER_FMC_MAXBOARD;

    res = Carrier_FmcValidatePresence(hCarrier,
        iConnector, iBoard,
        _ModList, sizeof(_ModList)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if (FAILURE(res)) return res;

    if (mdetected == CARRIER_NOT_DETECTED) return eMo1000Absent;

    return eMo1000Ok;
}


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 * NAME : Mo1000_Presence_recv
 ************************************************************************//**
 *
 * Check for a mo1000 module presence in specified board position.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:   module instance to be verified
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *
 ***************************************************************************/
Result Mo1000_Presence_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;

    // Get desired board instance
    uNb = uint_recv(state);

    mdebug("Mo1000_Presence #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    return res;
}

 /****************************************************************************
 * NAME : Mo1000_PowerUp_recv
 ************************************************************************//**
 *
 * Power up a mo1000 module in specified board position.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:   module instance to be used
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *
 ***************************************************************************/
Result Mo1000_PowerUp_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    int iConnector, iBoard;

    // Get desired board instance
    uNb = uint_recv(state);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    iBoard = uNb % CARRIER_FMC_MAXBOARD;
    iConnector = uNb / CARRIER_FMC_MAXBOARD;

	mdebug("Mo1000_PowerUp #%d\n",uNb+1);

    res = Carrier_FmcPowerUp(hCarrier,
        iConnector, iBoard,
        _ModList, sizeof(_ModList)/sizeof(FMCEEprom_module_types),
        VADJ_2_503V);

    return res;
}

 /****************************************************************************
 * NAME : Mo1000_Reset_recv
 ************************************************************************//**
 *
 * Reset a mo1000 module (default non working state) in specified board position.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:   module instance to be used
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *
 ***************************************************************************/
Result Mo1000_Reset_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;

    uNb = uint_recv(state);

	mdebug("Mo1000_Reset #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

	res = Mo1000_Reset(&Mo1000Board[uNb]);

    return res;
}

 /****************************************************************************
 * NAME : Mo1000_Init_recv
 ************************************************************************//**
 *
 * Initialize a mo1000 module (default working state) in specified board position.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:   module instance to be used
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_Init_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;

    uNb = uint_recv(state);

	mdebug("Mo1000_Init #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

	res = Mo1000_Init(&Mo1000Board[uNb]);

    return res;
}

 /****************************************************************************
 * NAME : Mo1000_WriteReg_recv
 ************************************************************************//**
 *
 * Write a mo1000 module peripheral register with specified values.
 *
 * @warning
 * - This function is for test purpose only and could corrupt module operation.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:            module instance to be used
 *   [in] Mo1000_eDevices Device:  peripheral device to be used
 *   [in] unsigned char ucAddr:    address of peripheral to be used
 *   [in] unsigned uValue:         register value to be written
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_WriteReg_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    Mo1000_eDevices Device;
    unsigned char ucAddr;
    unsigned uNb, uValue;

    uNb = uint_recv(state);

    Device = uint_recv(state);
	ucAddr = uchar_recv(state);
    uValue = uint_recv(state);

	mdebug("Mo1000_WriteReg #%d Device: #%d\n",uNb+1,Device);
	mdebug("Mo1000_WriteReg Addr: 0x%02X, Value: 0x%08X\n",ucAddr, uValue);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_WriteReg(&Mo1000Board[uNb], Device, ucAddr, uValue);

    return res;
}

 /****************************************************************************
 * NAME : Mo1000_ReadReg_recv
 ************************************************************************//**
 *
 * Read a mo1000 module peripheral register for specified address.
 *
 * @warning
 * - This function is for test purpose only.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:            module instance to be used
 *   [in] Mo1000_eDevices Device:  peripheral device to be used
 *   [in] unsigned char ucAddr:    address of peripheral to be used
 * - Communication packet sent will include:
 *   [out] unsigned uValue:        register value read
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_ReadReg_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    Mo1000_eDevices Device;
    unsigned char ucAddr;
    unsigned uNb, uValue = 0;

    uNb = uint_recv(state);

    Device = uint_recv(state);
	ucAddr = uchar_recv(state);

	mdebug("Mo1000_ReadReg #%d Addr: 0x%02X\n",uNb+1,ucAddr);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uValue);
        return res;
    }

    res = Mo1000_ReadReg(&Mo1000Board[uNb], Device, ucAddr, &uValue);

	mdebug("Mo1000_ReadReg Value: 0x%08X\n",uValue);
	uint_send(state, uValue);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_GetStatus_recv
 ************************************************************************//**
 *
 * Write a mo1000 module peripheral register with specified values.
 *
 * @warning
 * - This function is for test purpose only and could corrupt module operation.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used
 * - Communication packet sent will include:
 *   [out] Mo1000_stBoardStatus Status:    module status
 *   [out] Mo1000_stCompareErrors Compare: module test pattern errors
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_GetStatus_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_stBoardStatus Status;
    Mo1000_stCompareErrors Compare;

    // Default values
    Status.m_uRaw = 0;
    Compare.m_uRaw = 0;

    uNb = uint_recv(state);

    if( uNb >= MO1000_NBMAX)
    {
	    // Dummy send
        uint_send(state, Status.m_uRaw);
        uint_send(state, Compare.m_uRaw);
        return CCEINVALIDVALUE;
    }

	mdebug("Mo1000_GetStatus #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, Status.m_uRaw);
        uint_send(state, Compare.m_uRaw);
        return res;
    }

    res = Mo1000_GetStatus(&Mo1000Board[uNb], &Status, &Compare);

	mdebug("Mo1000_GetStatus Status: 0x%08X, Compare: 0x%08X\n", Status, Compare);
    uint_send(state, Status.m_uRaw);
    uint_send(state, Compare.m_uRaw);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_GetTemperature_recv
 ************************************************************************//**
 *
 * Read board pcb, DAC1 and DAC2 temperatures in specified mode (resolution of 1C or 0.1C).
 *
 * @attention
 * Board temperature is for information only. Different locations
 * on the board and environment conditions could affect the difference of temperature
 * seen on the board. User is responsible to keep the temperatures inside the allowed
 * operating condition.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used
 *   [in] Mo1000_eTempMode Mode:        temperature mode
 * - Communication packet sent will include:
 *   [out] short sTemp:                 board temperature
 *   [out] short sDac1:                 dac1 temperature
 *   [out] short sDac2:                 dac2 temperature
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_GetTemperature_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    short sTemp=5555, sDac1=5555, sDac2=5555;
    Mo1000_eTempMode Mode;

    uNb = uint_recv(state);
	Mode = uint_recv(state);

	mdebug("Mo1000_GetTemperature #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        ushort_send(state, sTemp);
        ushort_send(state, sDac1);
        ushort_send(state, sDac2);
        return res;
    }

    res = Mo1000_GetTemperature(&Mo1000Board[uNb], Mode, &sTemp, &sDac1, &sDac2);

	mdebug("Mo1000_GetTemperature Board: %d, Dac1: %d\n", sTemp, sDac1);
	mdebug("Mo1000_GetTemperature Dac2: %d\n", sDac2);
    ushort_send(state, sTemp);
    ushort_send(state, sDac1);
    ushort_send(state, sDac2);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetClockConfig_recv
 ************************************************************************//**
 *
 * Setup and calculates the desired clocks configuration. Once setup, you
 * must use the function Mo1000_WriteClockConfig_recv() to complete the programming
 * of the generated clock setup and makes it effective.
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Interpolation rate must be choosen before configuring clocks with this function using
 *   Mo1000_SetDacParInterpolation_recv(). While it is allowed to change the interpolation
 *   mode later, it must be the same rate as the one set before configuring the clocks.
 * - For a mo1000 double stack (used in master/slave clock mode), it is assumed that
 *   MasterClkMode is set to eMo1000MasterClkAuto for the bottom board clock configuration,
 *   and the same configuration is used for the top board (except that SrcClk must be set
 *   to eMo1000ClkSrcBottomfmc for top adapter), to achieve correct stack clocks
 *   configuration. MasterClkMode set to eMo1000MasterClkManual for the bottom adapter should
 *   only be used when the top adapter is anything else than a mo1000 (uFreqMasterClock could
 *   be set to 0 if no top adapter is present).
 * - When and an internal pll loop filter cannot be calculated for the clocks requierements,
 *   an error of #eCdce62005NoFilterSolution will be returned which means there is a synthetized
 *   solution for the specified requierements, but a loop filter must be manually calculated
 *   to complete the solution. Calculated solution results could be used as inputs for a manually
 *   calculated loop filter. When the loop filter is calculated manually
 *   the stParam members R2Value, R3Value, C1Value, C2Value, C3Value
 *   and ChargePump must be manually provided after calculation to complete the solution. Functions
 *   Mo1000_GetPllConfig_recv() and Mo1000_SetPllConfig_recv() can be used to manually calculates a new
 *   loop filter configuration (then Mo1000_WriteClockConfig_recv() is used to program everything).
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] Mo1000_eClockSource SrcClk: Reference clock source used
 *   [in] unsigned uFreqSrcClk:       Frequency of reference clock source when SrcClk is not eMo1000ClkSrc125Mhz
 *   [in] unsigned uFreqDacClock:     Frequency of dacs clock desired
 *   [in] Mo1000_eMasterClkMode MasterClkMode: Master clock generation mode
 *   [in] unsigned uFreqMasterClock:  Frequency of master clock (provided to top board) when MasterClkMode is not eMo1000MasterClkAuto. It could be set to 0 for bottom adapter when no top adapter is used.
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(), Mo1000_SetDacParInterpolation_recv().
 *
 * @see
 *    Mo1000_WriteClockConfig_recv().
 *
 ***************************************************************************/
Result Mo1000_SetClockConfig_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eClockSource SrcClk;
    Mo1000_eMasterClkMode MasterClkMode;
    unsigned uFreqSrcClk, uFreqDacClock, uFreqMasterClock;

    uNb = uint_recv(state);
	SrcClk = uint_recv(state);
	uFreqSrcClk = uint_recv(state);
	uFreqDacClock = uint_recv(state);
	MasterClkMode = uint_recv(state);
	uFreqMasterClock = uint_recv(state);

	mdebug("Mo1000_SetClockConfig #%d\n",uNb+1);
	mdebug("Mo1000_SetClockConfig Source clock: %d, Frequency source clock: %d\n", SrcClk, uFreqSrcClk);
	mdebug("Mo1000_SetClockConfig Frequency dac clock: %d, Master clock mode: %d\n", uFreqDacClock, MasterClkMode);
	mdebug("Mo1000_SetClockConfig Frequency master clock: %d\n", uFreqMasterClock);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetClockConfig(&Mo1000Board[uNb], SrcClk, uFreqSrcClk, uFreqDacClock, MasterClkMode, uFreqMasterClock);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_WriteClockConfig_recv
 ************************************************************************//**
 *
 * Complete the clocks programming prepared with Mo1000_SetClockConfig_recv()
 * and makes it effective.
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - When and an internal pll loop filter cannot be calculated for the clocks requierements,
 *   an error of #eCdce62005NoFilterSolution was returned with Mo1000_SetClockConfig_recv(),
 *   which means there is a synthetized solution but a loop filter must be manually calculated
 *   to complete the solution. Calculated solution results could be used as inputs for a manually
 *   calculated loop filter. When the loop filter is calculated manually
 *   the stParam members R2Value, R3Value, C1Value, C2Value, C3Value
 *   and ChargePump must be manually provided after calculation to complete the solution. Functions
 *   Mo1000_GetPllConfig_recv() and Mo1000_SetPllConfig_recv() can be used to manually calculates a new
 *   loop filter configuration (then Mo1000_WriteClockConfig_recv() is used to program everything).
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(), Mo1000_SetDacParInterpolation_recv(),
 *    Mo1000_SetClockConfig_recv().
 *
 * @see
 *    Mo1000_SetClockConfig_recv(), Mo1000_GetPllConfig_recv(), Mo1000_SetPllConfig_recv().
 *
 ***************************************************************************/
Result Mo1000_WriteClockConfig_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;

    uNb = uint_recv(state);

	mdebug("Mo1000_WriteClockConfig #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_WriteClockConfig(&Mo1000Board[uNb]);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_GetPllConfig_recv
 ************************************************************************//**
 *
 * Get the calculated pll setup prepared with Mo1000_SetClockConfig_recv(). This
 * function can be used to recover the pll information, for example to
 * manually calculates the loop filter components when not automatically
 * supported.
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                                  module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uPrimRefFreq:                        Primary pll reference frequency
 *   [out] unsigned uSecRefFreq:                         Secondary pll reference frequency
 *   [out] Cdce62005_eOutRefMode OutputRef[CDCE62005_NPLL]: Output divider reference config
 *   [out] unsigned uVcoFreq:                            Calculated vco frequency
 *   [out] unsigned char  ucPrimaryInputDivider:         Calculated primary input pre divider (0 disable,
 *   [out] unsigned short usReferenceDivider:            Calculated reference divider (means divide by 1
 *   [out] unsigned short usInputDivider:                Calculated input divider
 *   [out] unsigned short usFeedbackDivider:             Calculated feedback divider
 *   [out] unsigned char  ucBypassDivider:               Calculated bypass divider
 *   [out] unsigned char ucPrescaleDivider:              Calculated prescaler divider
 *   [out] unsigned char aucOutDivider[CDCE62005_NPLL]:  Calculated outputs dividers
 *   [out] Cdce62005_eR2Values R2Value:                  Calculated loop filter R2 value
 *   [out] Cdce62005_eR3Values R3Value:                  Calculated loop filter R3 value
 *   [out] Cdce62005_eC1Values C1Value:                  Calculated loop filter C1 value
 *   [out] Cdce62005_eC2Values C2Value:                  Calculated loop filter C2 value
 *   [out] Cdce62005_eC3Values C3Value:                  Calculated loop filter C3 value
 *   [out] Cdce62005_eChargePump ChargePump:             Calculated loop filter charge pump current value
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(), Mo1000_SetDacParInterpolation_recv(),
 *    Mo1000_SetClockConfig_recv().
 *
 * @see
 *    Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/
Result Mo1000_GetPllConfig_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_stPllParam Param;

    memset(&Param, 0, sizeof(Mo1000_stPllParam));
    uNb = uint_recv(state);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, Param.uPrimRefFreq);
        uint_send(state, Param.uSecRefFreq);
        uint_send(state, Param.OutputRef0);
        uint_send(state, Param.OutputRef1);
        uint_send(state, Param.OutputRef2);
        uint_send(state, Param.OutputRef3);
        uint_send(state, Param.OutputRef4);
        uint_send(state, Param.uVcoFreq);
        uchar_send(state, Param.ucPrimaryInputDivider);
        ushort_send(state, Param.usReferenceDivider);
        ushort_send(state, Param.usInputDivider);
        ushort_send(state, Param.usFeedbackDivider);
        uchar_send(state, Param.ucBypassDivider);
        uchar_send(state, Param.ucPrescaleDivider);
        uchar_send(state, Param.aucOutDivider0);
        uchar_send(state, Param.aucOutDivider1);
        uchar_send(state, Param.aucOutDivider2);
        uchar_send(state, Param.aucOutDivider3);
        uchar_send(state, Param.aucOutDivider4);
        uint_send(state, Param.R2Value);
        uint_send(state, Param.R3Value);
        uint_send(state, Param.C1Value);
        uint_send(state, Param.C2Value);
        uint_send(state, Param.C3Value);
        uint_send(state, Param.ChargePump);
        return res;
    }

    res = Mo1000_GetPllConfig(&Mo1000Board[uNb], &Param);

	mdebug("Mo1000_GetPllConfig Primary ref freq: %d\n", Param.uPrimRefFreq);
	mdebug("Mo1000_GetPllConfig Secondary ref freq: %d\n", Param.uSecRefFreq);
	mdebug("Mo1000_GetPllConfig Output 0 mux ref: %d\n", Param.OutputRef0);
	mdebug("Mo1000_GetPllConfig Output 1 mux ref: %d\n", Param.OutputRef1);
	mdebug("Mo1000_GetPllConfig Output 2 mux ref: %d\n", Param.OutputRef2);
	mdebug("Mo1000_GetPllConfig Output 3 mux ref: %d\n", Param.OutputRef3);
	mdebug("Mo1000_GetPllConfig Output 4 mux ref: %d\n", Param.OutputRef4);
	mdebug("Mo1000_GetPllConfig Vco freq: %d\n", Param.uVcoFreq);
	mdebug("Mo1000_GetPllConfig Primary input divider: %d\n", Param.ucPrimaryInputDivider);
	mdebug("Mo1000_GetPllConfig Reference divider: %d\n", Param.usReferenceDivider);
	mdebug("Mo1000_GetPllConfig Input divider: %d\n", Param.usInputDivider);
	mdebug("Mo1000_GetPllConfig Feedback divider: %d\n", Param.usFeedbackDivider);
	mdebug("Mo1000_GetPllConfig Bypass divider: %d\n", Param.ucBypassDivider);
	mdebug("Mo1000_GetPllConfig Prescale divider: %d\n", Param.ucPrescaleDivider);
	mdebug("Mo1000_GetPllConfig Output 0 divider: %d\n", Param.aucOutDivider0);
	mdebug("Mo1000_GetPllConfig Output 1 divider: %d\n", Param.aucOutDivider1);
	mdebug("Mo1000_GetPllConfig Output 2 divider: %d\n", Param.aucOutDivider2);
	mdebug("Mo1000_GetPllConfig Output 3 divider: %d\n", Param.aucOutDivider3);
	mdebug("Mo1000_GetPllConfig Output 4 divider: %d\n", Param.aucOutDivider4);
	mdebug("Mo1000_GetPllConfig R2 index value: %d\n", Param.R2Value);
	mdebug("Mo1000_GetPllConfig R3 index value: %d\n", Param.R3Value);
	mdebug("Mo1000_GetPllConfig C1 index value: %d\n", Param.C1Value);
	mdebug("Mo1000_GetPllConfig C2 index value: %d\n", Param.C2Value);
	mdebug("Mo1000_GetPllConfig C3 index value: %d\n", Param.C3Value);
	mdebug("Mo1000_GetPllConfig Charge pump: %d\n", Param.ChargePump);
    // Serialize the pll structure
    uint_send(state, Param.uPrimRefFreq);
    uint_send(state, Param.uSecRefFreq);
    uint_send(state, Param.OutputRef0);
    uint_send(state, Param.OutputRef1);
    uint_send(state, Param.OutputRef2);
    uint_send(state, Param.OutputRef3);
    uint_send(state, Param.OutputRef4);
    uint_send(state, Param.uVcoFreq);
    uchar_send(state, Param.ucPrimaryInputDivider);
    ushort_send(state, Param.usReferenceDivider);
    ushort_send(state, Param.usInputDivider);
    ushort_send(state, Param.usFeedbackDivider);
    uchar_send(state, Param.ucBypassDivider);
    uchar_send(state, Param.ucPrescaleDivider);
    uchar_send(state, Param.aucOutDivider0);
    uchar_send(state, Param.aucOutDivider1);
    uchar_send(state, Param.aucOutDivider2);
    uchar_send(state, Param.aucOutDivider3);
    uchar_send(state, Param.aucOutDivider4);
    uint_send(state, Param.R2Value);
    uint_send(state, Param.R3Value);
    uint_send(state, Param.C1Value);
    uint_send(state, Param.C2Value);
    uint_send(state, Param.C3Value);
    uint_send(state, Param.ChargePump);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetPllConfig_recv
 ************************************************************************//**
 *
 * Set pll configuration. This function can be used to change the pll configuration,
 * for example to manually calculates the loop filter components when not automatically
 * supported.
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                                 module instance to be used
 *   [in] unsigned uPrimRefFreq:                        Primary pll reference frequency (ignored)
 *   [in] unsigned uSecRefFreq:                         Secondary pll reference frequency (ignored)
 *   [in] Cdce62005_eOutRefMode OutputRef[CDCE62005_NPLL]: Output divider reference config (ignored)
 *   [in] unsigned uVcoFreq:                            Calculated vco frequency
 *   [in] unsigned char  ucPrimaryInputDivider:         Calculated primary input pre divider (0 disable,
 *   [in] unsigned short usReferenceDivider:            Calculated reference divider (means divide by 1
 *   [in] unsigned short usInputDivider:                Calculated input divider
 *   [in] unsigned short usFeedbackDivider:             Calculated feedback divider
 *   [in] unsigned char  ucBypassDivider:               Calculated bypass divider
 *   [in] unsigned char ucPrescaleDivider:              Calculated prescaler divider
 *   [in] unsigned char aucOutDivider[CDCE62005_NPLL]:  Calculated outputs dividers
 *   [in] Cdce62005_eR2Values R2Value:                  Calculated loop filter R2 value
 *   [in] Cdce62005_eR3Values R3Value:                  Calculated loop filter R3 value
 *   [in] Cdce62005_eC1Values C1Value:                  Calculated loop filter C1 value
 *   [in] Cdce62005_eC2Values C2Value:                  Calculated loop filter C2 value
 *   [in] Cdce62005_eC3Values C3Value:                  Calculated loop filter C3 value
 *   [in] Cdce62005_eChargePump ChargePump:             Calculated loop filter charge pump current value
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(), Mo1000_SetDacParInterpolation_recv(),
 *    Mo1000_SetClockConfig_recv(), Mo1000_GetPllConfig_recv().
 *
 * @see
 *    Mo1000_GetPllConfig_recv(),Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/
Result Mo1000_SetPllConfig_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_stPllParam Param;

    memset(&Param, 0, sizeof(Mo1000_stPllParam));
    uNb = uint_recv(state);
    // Deserialize the pll structure
    Param.uPrimRefFreq = uint_recv(state);
    Param.uSecRefFreq = uint_recv(state);
    Param.OutputRef0 = uint_recv(state);
    Param.OutputRef1= uint_recv(state);
    Param.OutputRef2 = uint_recv(state);
    Param.OutputRef3 = uint_recv(state);
    Param.OutputRef4 = uint_recv(state);
    Param.uVcoFreq = uint_recv(state);
    Param.ucPrimaryInputDivider = uchar_recv(state);
    Param.usReferenceDivider = ushort_recv(state);
    Param.usInputDivider = ushort_recv(state);
    Param.usFeedbackDivider = ushort_recv(state);
    Param.ucBypassDivider = uchar_recv(state);
    Param.ucPrescaleDivider = uchar_recv(state);
    Param.aucOutDivider0 = uchar_recv(state);
    Param.aucOutDivider1 = uchar_recv(state);
    Param.aucOutDivider2 = uchar_recv(state);
    Param.aucOutDivider3 = uchar_recv(state);
    Param.aucOutDivider4 = uchar_recv(state);
    Param.R2Value = uint_recv(state);
    Param.R3Value = uint_recv(state);
    Param.C1Value = uint_recv(state);
    Param.C2Value = uint_recv(state);
    Param.C3Value = uint_recv(state);
    Param.ChargePump = uint_recv(state);

	mdebug("Mo1000_SetPllConfig #%d\n",uNb+1);

	mdebug("Mo1000_SetPllConfig Primary ref freq: %d\n", Param.uPrimRefFreq);
	mdebug("Mo1000_SetPllConfig Secondary ref freq: %d\n", Param.uSecRefFreq);
	mdebug("Mo1000_SetPllConfig Output 0 mux ref: %d\n", Param.OutputRef0);
	mdebug("Mo1000_SetPllConfig Output 1 mux ref: %d\n", Param.OutputRef1);
	mdebug("Mo1000_SetPllConfig Output 2 mux ref: %d\n", Param.OutputRef2);
	mdebug("Mo1000_SetPllConfig Output 3 mux ref: %d\n", Param.OutputRef3);
	mdebug("Mo1000_SetPllConfig Output 4 mux ref: %d\n", Param.OutputRef4);
	mdebug("Mo1000_SetPllConfig Vco freq: %d\n", Param.uVcoFreq);
	mdebug("Mo1000_SetPllConfig Primary input divider: %d\n", Param.ucPrimaryInputDivider);
	mdebug("Mo1000_SetPllConfig Reference divider: %d\n", Param.usReferenceDivider);
	mdebug("Mo1000_SetPllConfig Input divider: %d\n", Param.usInputDivider);
	mdebug("Mo1000_SetPllConfig Feedback divider: %d\n", Param.usFeedbackDivider);
	mdebug("Mo1000_SetPllConfig Bypass divider: %d\n", Param.ucBypassDivider);
	mdebug("Mo1000_SetPllConfig Prescale divider: %d\n", Param.ucPrescaleDivider);
	mdebug("Mo1000_SetPllConfig Output 0 divider: %d\n", Param.aucOutDivider0);
	mdebug("Mo1000_SetPllConfig Output 1 divider: %d\n", Param.aucOutDivider1);
	mdebug("Mo1000_SetPllConfig Output 2 divider: %d\n", Param.aucOutDivider2);
	mdebug("Mo1000_SetPllConfig Output 3 divider: %d\n", Param.aucOutDivider3);
	mdebug("Mo1000_SetPllConfig Output 4 divider: %d\n", Param.aucOutDivider4);
	mdebug("Mo1000_SetPllConfig R2 index value: %d\n", Param.R2Value);
	mdebug("Mo1000_SetPllConfig R3 index value: %d\n", Param.R3Value);
	mdebug("Mo1000_SetPllConfig C1 index value: %d\n", Param.C1Value);
	mdebug("Mo1000_SetPllConfig C2 index value: %d\n", Param.C2Value);
	mdebug("Mo1000_SetPllConfig C3 index value: %d\n", Param.C3Value);
	mdebug("Mo1000_SetPllConfig Charge pump: %d\n", Param.ChargePump);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetPllConfig(&Mo1000Board[uNb], &Param);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_DoDacCalibration_recv
 ************************************************************************//**
 *
 * Force a digital calibration of all dac with active channels. This will
 * calibrates their data lanes, frame signal and synchronize dacs.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_DoDacCalibration_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;

    uNb = uint_recv(state);

	mdebug("Mo1000_DoDacCalibration #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_DoDacCalibration(&Mo1000Board[uNb]);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_GetChannelCalibStatus_recv
 ************************************************************************//**
 *
 * Read DAC channels calibration information. This info was the last time
 * the DACs were calibrated. It will return global calibration process
 * success flag, data channels lanes which were calibrated correctly or not
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uChannelLaneCalib:  When uCalibStatus=0, this will return the status of data lanes calibration
 *                                      that could be in error
 *   [out] unsigned uChannelFrameCalib: When uCalibStatus=0, this will return the status of frame lane calibration
 *                                      that could be in error
 *   [out] unsigned uChannelSyncCalib:  When uCalibStatus=0, this will return the status of dac synchronisation calibration
 *                                      that could be in error
 *   [out] unsigned uCalibStatus:       When !=0, this means the whole channels calibration was successfull. There
 *                                      are 3 steps in the calibration process: data lanes calibration, frame signal calibration
 *                                      and dacs synchronisation.  If uCalibStatus = 0, there was an error somewhere in the
 *                                      calibration steps and uChannelLaneCalib, uChannelFrameCalib, uChannelSyncCalib
 *                                      will give information of what calibration step that went wrong.
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *
 ***************************************************************************/
Result Mo1000_GetChannelCalibStatus_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uChannelLaneCalib=0, uChannelFrameCalib=0, uChannelSyncCalib=0, uCalibStatus=0;

    uNb = uint_recv(state);

	mdebug("Mo1000_GetChannelCalibStatus #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uChannelLaneCalib);
        uint_send(state, uChannelFrameCalib);
        uint_send(state, uChannelSyncCalib);
        uint_send(state, uCalibStatus);
        return res;
    }

    res = Mo1000_GetChannelCalibStatus(&Mo1000Board[uNb], &uChannelLaneCalib, &uChannelFrameCalib, &uChannelSyncCalib, &uCalibStatus);

	mdebug("Mo1000_GetChannelCalibStatus Data lane calib: 0x%08X, Frame lane calib: 0x%08X\n", uChannelLaneCalib, uChannelFrameCalib);
	mdebug("Mo1000_GetChannelCalibStatus Sync calib: 0x%08X, Global Status: 0x%08X\n", uChannelSyncCalib, uCalibStatus);
    uint_send(state, uChannelLaneCalib);
    uint_send(state, uChannelFrameCalib);
    uint_send(state, uChannelSyncCalib);
    uint_send(state, uCalibStatus);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacOutCtrl_recv
 ************************************************************************//**
 *
 * Control the dac output state (enable/disable) for the specified channel.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] Mo1000_eDacSelect Channel:  dac channel to change the output state
 *   [in] Mo1000_eOutputState State:  desired output state
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacOutCtrl_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eDacSelect Channel;
    Mo1000_eOutputState State;

    uNb = uint_recv(state);
	Channel = uint_recv(state);
	State = uint_recv(state);

	mdebug("Mo1000_SetDacOutCtrl #%d\n",uNb+1);
	mdebug("Mo1000_SetDacOutCtrl Channel: %d, Output State: %d\n", Channel, State);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacOutCtrl(&Mo1000Board[uNb], Channel, State);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParDcOffset_recv
 ************************************************************************//**
 *
 * Control the dac output dc offset for the specified channel.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] Mo1000_eDacSelect Channel:  dac channel
 *   [in] short sOffset:              desired output dc offset
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParDcOffset_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eDacSelect Channel;
    short sOffset;

    uNb = uint_recv(state);
	Channel = uint_recv(state);
	sOffset = ushort_recv(state);

	mdebug("Mo1000_SetDacParDcOffset #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParDcOffset Channel: %d, Dc Offset: %d\n", Channel, sOffset);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParDcOffset(&Mo1000Board[uNb], Channel, sOffset);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParGain_recv
 ************************************************************************//**
 *
 * Set the dac gain amplitude correction. The value is a 8 bits unsigned
 * value and each count gives a gain of 0.015625 (ScaleFactor = ucGain / 64).
 *
 * @warning
 * - Because the gain is digital, setting the correction factor greater than
 *   64 (gain of 1) could cause signal saturation if this cause an overrange.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] Mo1000_eDacSelect Channel:  dac channel
 *   [in] unsigned char ucGain:       desired output gain correction desired
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParGain_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eDacSelect Channel;
    unsigned char ucGain;

    uNb = uint_recv(state);
	Channel = uint_recv(state);
	ucGain = uchar_recv(state);

	mdebug("Mo1000_SetDacParGain #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParGain Channel: %d, Gain (1/64 unit): %d\n", Channel, ucGain);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParGain(&Mo1000Board[uNb], Channel, ucGain);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParInterpolation_recv
 ************************************************************************//**
 *
 * Control the dacs interpolation mode for all channels.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] unsigned uMode:             desired channels interpolation mode
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParInterpolation_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Ad9148_eInterpolationMode Mode;

    uNb = uint_recv(state);
	Mode = uint_recv(state);

	mdebug("Mo1000_SetDacParInterpolation #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParInterpolation Mode: %d\n", Mode);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParInterpolation(&Mo1000Board[uNb], Mode);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_DoDacUpdate_recv
 ************************************************************************//**
 *
 * Configure the dacs for operation with all the current parameters (default
 * parameters if coming from Mo1000_Reset_recv(), or user specified with
 * Mo1000_SetDacParxxx functions). This function will only update dacs parameter
 * which recently changed and are not up to date.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_DoDacUpdate_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;

    uNb = uint_recv(state);

	mdebug("Mo1000_DoDacUpdate #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_DoDacUpdate(&Mo1000Board[uNb]);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParActChannel_recv
 ************************************************************************//**
 *
 * Set the dac active channels. Active channels can be activated in pair,
 * the other inactive channels are powered down.
 *
 * @warning
 * - Enabling more active channels than the last calibration time will requiere
 *   a new calibration with Mo1000_DoDacCalibration_recv().
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] unsigned uActChannel:       number of active dac channels
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParActChannel_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eActChannel ActChannel;

    uNb = uint_recv(state);
	ActChannel = uint_recv(state);

	mdebug("Mo1000_SetDacParActChannel #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParActChannel Active channels: %d\n", ActChannel);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParActChannel(&Mo1000Board[uNb], ActChannel);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_GetClkMaster_recv
 ************************************************************************//**
 *
 * Detect if this current module instance is a clock master for the FPGA (uses
 * its own clock reference). In a multiple module configuration, the FPGA clock
 * master is the one that provides it's own reference clock to the fpga from
 * which the fpga will derive it's data clock (DCI). Normally, only the module
 * instance which is defined as the clock master can use the function related
 * to clock source selection (Mo1000_SetClockConfig_recv()) to select internal
 * (#eMo1000ClkSrc125mhz), external (#eMo1000ClkSrcExt) or carrier clock
 * sources. If external clock source is selected for the clock master module,
 * it must be connected on the EC signal of this module connector. The module
 * which is not the FPGA clock master can only selects as a clock source, the
 * other module (#eMo1000ClkSrcBottomfmc).
 *
 * @attention
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned int master:         !=0 when instance is a clock master
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *
 * @see
 *    Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/
Result Mo1000_GetClkMaster_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uMaster = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_GetClkMaster #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uMaster);
        return res;
    }

    res = Mo1000_GetClkMaster(&Mo1000Board[uNb], &uMaster);

	mdebug("Mo1000_GetClkMaster Board: %d, Master: %d\n", uNb, uMaster);
    uint_send(state, uMaster);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParISinc_recv
 ************************************************************************//**
 *
 * Set inverse sinc filter coefficients (real and imaginary) for a dac
 * pair (real coefficients for I dac (odd dac) and imaginary for Q dac (even dac)).
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Dac output center frequency affects the isinc filter coefficients.
 * - Dac interpolation rate affects the isinc filter coefficients.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                           module instance to be used
 *   [in] Mo1000_eSelectDacPair DacPair:          dac channel pair to change inverse sinc filter options
 *   [in] short sRealC0:                          Inverse sinc filter real coefficients C0
 *   [in] short sRealC1:                          Inverse sinc filter real coefficients C1
 *   [in] short sRealC2:                          Inverse sinc filter real coefficients C2
 *   [in] short sRealC3:                          Inverse sinc filter real coefficients C3
 *   [in] short sRealC4:                          Inverse sinc filter real coefficients C4
 *                                                (for I dac i.e. odd dac)
 *   [in] short sImgC0:                           Inverse sinc filter imaginary coefficients C0
 *   [in] short sImgC1:                           Inverse sinc filter imaginary coefficients C1
 *   [in] short sImgC2:                           Inverse sinc filter imaginary coefficients C2
 *   [in] short sImgC3:                           Inverse sinc filter imaginary coefficients C3
 *   [in] short sImgC4:                           Inverse sinc filter imaginary coefficients C4
 *                                                (for Q dac i.e. even dac)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 * @see
 *    Mo1000_SetDacParInterpolation_recv(), Mo1000_SetDacParISincCtrl_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParISinc_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    short sRealC0, sRealC1, sRealC2, sRealC3, sRealC4;
    short sImgC0, sImgC1, sImgC2, sImgC3, sImgC4;
    Mo1000_eSelectDacPair DacPair;

    uNb = uint_recv(state);
	DacPair = uint_recv(state);
	sRealC0 = (short)(ushort_recv(state));
	sRealC1 = (short)(ushort_recv(state));
	sRealC2 = (short)(ushort_recv(state));
	sRealC3 = (short)(ushort_recv(state));
	sRealC4 = (short)(ushort_recv(state));
	sImgC0  = (short)(ushort_recv(state));
	sImgC1  = (short)(ushort_recv(state));
	sImgC2  = (short)(ushort_recv(state));
	sImgC3  = (short)(ushort_recv(state));
	sImgC4  = (short)(ushort_recv(state));

	mdebug("Mo1000_SetDacParISinc #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParISinc pair: %d, real C0:%d\n", DacPair, sRealC0);
	mdebug("Mo1000_SetDacParISinc real C1:%d, real C2:%d\n", sRealC1, sRealC2);
	mdebug("Mo1000_SetDacParISinc real C3:%d, real C4:%d\n", sRealC3, sRealC4);
	mdebug("Mo1000_SetDacParISinc img C0:%d\n", sImgC0);
	mdebug("Mo1000_SetDacParISinc img C1:%d, img C2:%d\n", sImgC1, sImgC2);
	mdebug("Mo1000_SetDacParISinc img C3:%d, img C4:%d\n", sImgC3, sImgC4);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParISinc(&Mo1000Board[uNb], DacPair, sRealC0, sRealC1, sRealC2, sRealC3,
                                sRealC4, sImgC0, sImgC1, sImgC2, sImgC3, sImgC4);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParISincCtrl_recv
 ************************************************************************//**
 *
 * Enable/disable the inverse sinc filter for the specified channels group.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] Mo1000_eDacGroupId Group:   dac channels group to change the inverse sinc filter state
 *   [in] Mo1000_eIsincState State:   Inverse sinc filter control state desired
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 * @see
 *    Mo1000_SetDacParISinc_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParISincCtrl_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eDacGroupId Group;
    Mo1000_eIsincState State;

    uNb = uint_recv(state);
	Group = uint_recv(state);
	State = uint_recv(state);

	mdebug("Mo1000_SetDacParISincCtrl #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParISincCtrl Group: %d, ISinc State: %d\n", Group, State);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParISincCtrl(&Mo1000Board[uNb], Group, State);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParFineMod_recv
 ************************************************************************//**
 *
 * Set fine modulation options for the specified dac channels group.
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Call Mo1000_SetDacParFineModCtrl_recv() to enable the fine modulation before
 *   configuring it with this function.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] Mo1000_eDacGroupId Group:   dac channels group to change the fine modulation options
 *   [in] unsigned uFtw:              Frequency tuning word desired (uFtw = Fcenter/Fdac X 4294967296)
 *   [in] unsigned short usNcoPhaseOffset: Nco phase offset adjustement desired from 0 to 360 deg
 *                                         (each count add 0.0054931640625 deg)
 *   [in] Mo1000_eNcoSideband Sideband:    Choose which sideband is desired
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(),Mo1000_SetDacParFineModCtrl_recv().
 *
 * @see
 *    Mo1000_SetDacParFineModCtrl_recv(), Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParFineMod_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eDacGroupId Group;
    unsigned uFtw;
    unsigned short usNcoPhaseOffset;
    Mo1000_eNcoSideband Sideband;

    uNb = uint_recv(state);
	Group = uint_recv(state);
	uFtw = uint_recv(state);
	usNcoPhaseOffset = ushort_recv(state);
	Sideband = uint_recv(state);

	mdebug("Mo1000_SetDacParFineMod #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParFineMod Group: %d, Ftw:%d\n", Group, uFtw);
	mdebug("Mo1000_SetDacParFineMod usNcoPhaseOffset:%d, Sideband: %d\n", usNcoPhaseOffset, Sideband);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParFineMod(&Mo1000Board[uNb], Group, uFtw, usNcoPhaseOffset, Sideband);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParFineModCtrl_recv
 ************************************************************************//**
 *
 * Enable/disable the fine modulation (NCO) for the specified dac channels group.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] Mo1000_eDacGroupId Group:   dac channels group to change the fine modulation state
 *   [in] Mo1000_eFineModState State: Fine modulation control state desired
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 * @see
 *    Mo1000_SetDacParFineMod_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParFineModCtrl_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eDacGroupId Group;
    Mo1000_eFineModState State;

    uNb = uint_recv(state);
	Group = uint_recv(state);
	State = uint_recv(state);

	mdebug("Mo1000_SetDacParFineModCtrl #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParFineModCtrl Group: %d, Finemod State: %d\n", Group, State);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParFineModCtrl(&Mo1000Board[uNb], Group, State);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParDataFormat_recv
 ************************************************************************//**
 *
 * Set the data format used for the specified dac channels group.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used
 *   [in] Mo1000_eDacGroupId Group:   dac channels group to change the data format
 *   [in] Mo1000_eDataFormat Format:  Which data format to use (binary or 2 complement)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 * @see
 *    Mo1000_SetDacParFineMod_recv().
 *
 ***************************************************************************/
Result Mo1000_SetDacParDataFormat_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eDacGroupId Group;
    Mo1000_eDataFormat Format;

    uNb = uint_recv(state);
	Group = uint_recv(state);
	Format = uint_recv(state);

	mdebug("Mo1000_SetDacParDataFormat #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParDataFormat Group: %d, Format: %d\n", Group, Format);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParDataFormat(&Mo1000Board[uNb], Group, Format);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetDacParQuadPhase_recv
 ************************************************************************//**
 *
 * Set the quadrature phase offsets (phase between the I dac (real or odd dac)
 * and Q dac (imaginary or even dac) dacs pair. Each phase offset is in the range
 * of +/- 1.75 deg and when combined could give +/- 3.5 deg between I and Q dacs.
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                  module instance to be used
 *   [in] Mo1000_eSelectDacPair DacPair: dac channel pair to change the quadrature phase options
 *   [in] short sRealPhase:              I dac quadrature phase offset. Each count gives
 *                                       0.00342 deg offset. The value is a sign integer to give
 *                                       positive or negative offsets.
 *   [in] short sImaginaryPhase:         Q dac quadrature phase offset. Each count gives
 *                                       0.00342 deg offset. The value is a sign integer to give
 *                                       positive or negative offsets.
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 * @see
 *
 ***************************************************************************/
Result Mo1000_SetDacParQuadPhase_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    short sRealPhase, sImaginaryPhase;
    Mo1000_eSelectDacPair DacPair;

    uNb = uint_recv(state);
	DacPair = uint_recv(state);
	sRealPhase = (short)(ushort_recv(state));
	sImaginaryPhase = (short)(ushort_recv(state));

	mdebug("Mo1000_SetDacParQuadPhase #%d\n",uNb+1);
	mdebug("Mo1000_SetDacParQuadPhase Pair: %d, real:%d\n", DacPair, sRealPhase);
	mdebug("Mo1000_SetDacParQuadPhase imaginary:%d\n", sImaginaryPhase);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;

    res = Mo1000_SetDacParQuadPhase(&Mo1000Board[uNb], DacPair, sRealPhase, sImaginaryPhase);

    return res;
}


 /****************************************************************************
 * NAME : Mo1000_GetVersions_recv
 ************************************************************************//**
 *
 * Read mo1000 fpga core module version and driver version information.
 * This info could be used for diagnostic purpose.
 *
 * @attention
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned pCoreVersion:       MO1000 fpga core version
 *   [out] unsigned pDriverVersion:     MO1000 software driver version
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *
 ***************************************************************************/
Result Mo1000_GetVersions_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int pCoreVersion = 0;
    unsigned int pDriverVersion = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_GetVersions #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, pCoreVersion);
        uint_send(state, pDriverVersion);
        return res;
    }

    res = Mo1000_GetVersions(&Mo1000Board[uNb], &pCoreVersion, &pDriverVersion);

	mdebug("Mo1000_GetVersions FPGA: %d, Driver: %d\n", pCoreVersion, pDriverVersion);
    uint_send(state, pCoreVersion);
    uint_send(state, pDriverVersion);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestLocalOscSw_recv
 ************************************************************************//**
 *
 * Test the local onboard clock oscillator switch control for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestLocalOscSw_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestLocalOscSw #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestLocalOscSw(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestLocalOscSw Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestBridgeReset_recv
 ************************************************************************//**
 *
 * Test the bridge reset operation for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestBridgeReset_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestBridgeReset #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestBridgeReset(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestBridgeReset Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestDacReset_recv
 ************************************************************************//**
 *
 * Test the dacs reset operation for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestDacReset_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestDacReset #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestDacReset(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestDacReset Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestPllSel_recv
 ************************************************************************//**
 *
 * Test the pll mux selection function for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestPllSel_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestPllSel #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestPllSel(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestPllSel Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestPllPowDown_recv
 ************************************************************************//**
 *
 * Test the pll power down function for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestPllPowDown_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestPllPowDown #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestPllPowDown(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestPllPowDown Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestPllSync_recv
 ************************************************************************//**
 *
 * Test the pll sync function for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestPllSync_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestPllSync #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestPllSync(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestPllSync Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestDacIrq_recv
 ************************************************************************//**
 *
 * Test the dac irq internal signals for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestDacIrq_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestDacIrq #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestDacIrq(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestDacIrq Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestDacClks_recv
 ************************************************************************//**
 *
 * Test the dac clocks internal signals for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestDacClks_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestDacClks #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestDacClks(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestDacClks Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_TestDciFrame_recv
 ************************************************************************//**
 *
 * Test the dci+frame paths configuration by using correct defaults or
 * forcing to use the wrong paths for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv().
 *
 ***************************************************************************/
Result Mo1000_TestDciFrame_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned int uResult = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_TestDciFrame #%d\n",uNb+1);

    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        uint_send(state, uResult);
        return res;
    }

    res = Mo1000_TestDciFrame(&Mo1000Board[uNb], &uResult);

	mdebug("Mo1000_TestDciFrame Result: %d\n", uResult);
    uint_send(state, uResult);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_CheckI2c_recv
 ************************************************************************//**
 *
 * Verified a provided scanned i2c bus vector to detect all module devices.
 * This function is used for production purpose to check if all devices for module
 * operation were detected correctly on the i2c bus. This function is not
 * used in the normal user module operation.
 *
 * @attention
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used
 *   [in] unsigned char ucScanMax:      This is the maximum number of devices that must be detected
 *                                      to avoid a scan error. When 0 is specified, this parameter
 *                                      is ignored. When used on a perseus fmc carrier, with a
 *                                      single mo1000 module, this parameter should be 5 devices.
 *   [in] Mo1000_stI2cScan ScanRaw:     Raw i2c scan results structure (each detected device
 *                                      address in the vector hold != 0 when a device is detected).
 *                                      This vector is built when the i2c bus is scanned using a
 *                                      system function (i2c_scan_devices()).
 *   [in] unsigned char ucNumDevices:   Number of i2c devices that were detected on the bus at
 *                                      scan time (from i2c_scan_devices()).
 * - Communication packet sent will include:
 *   [out] Mo1000_stI2cAddr ScanRes:    For each of the device types of the module, this will
 *                                      return != 0 if the device has been detected correctly.
 *                                      All device types must be detected for the module to work correctly.
 *   [out] unsigned char ucDetectOk:    This is the global detection result. This will hold true (!=0)
 *                                      when all the device types havebeen detected and no more
 *                                      devices than ucScanMax are detected on the bus (ucScanMax
 *                                      is ignored when set to 0).
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 ***************************************************************************/
Result Mo1000_CheckI2c_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned char ucScanMax;
    Mo1000_stI2cScan ScanRaw;
    unsigned char ucNumDevices;
    Mo1000_stI2cAddr ScanRes;
    unsigned char pucDetectOk = 0;

    memset(&ScanRes,0,sizeof(Mo1000_stI2cAddr));

    uNb = uint_recv(state);
	ucScanMax = uchar_recv(state);

    res = buf_recv(state, (char*)&ScanRaw, sizeof(Mo1000_stI2cScan));
    ucNumDevices = uchar_recv(state);

    if(FAILURE(res))
    {
        // Dummy send
        buf_send(state, (char*)&ScanRes, sizeof(Mo1000_stI2cAddr));
        uchar_send(state, pucDetectOk);
        return CCEINVALIDVALUE;
    }

	mdebug("Mo1000_CheckI2c #%d\n",uNb+1);
    
    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        // Dummy send
        buf_send(state, (char*)&ScanRes, sizeof(Mo1000_stI2cAddr));
        uchar_send(state, pucDetectOk);
        return res;
    }
    
    res = Mo1000_CheckI2c(&Mo1000Board[uNb], ucScanMax, &ScanRaw, ucNumDevices, &ScanRes, &pucDetectOk);

	mdebug("Mo1000_CheckI2c Detect ok: %d\n", pucDetectOk);
    buf_send(state, (char*)&ScanRes, sizeof(Mo1000_stI2cAddr));
    uchar_send(state, pucDetectOk);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_GetCoreFrequency_recv
 ************************************************************************//**
 *
 * Read the fpga core reference and dacs reference clocks frequency for diagnostic purpose.
 *
 * @attention
 * - usDacRefClock would only be valid if the dac clocks were setup with
 *   Mo1000_SetClockConfig_recv() before using this function.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used
 * - Communication packet sent will include:
 *   [out] unsigned short usCoreRefClock: FPGA core reference clock frequency mesured in MHz
 *   [out] unsigned short usDacRefClock:  FPGA dacs reference clock frequency mesured in MHz
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 * @see
 *    Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/
Result Mo1000_GetCoreFrequency_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    unsigned short usCoreRefClock = 0, usDacRefClock = 0;

    uNb = uint_recv(state);

	mdebug("Mo1000_GetCoreFrequency #%d\n",uNb+1);
    
    res = _Mo1000_Presence(uNb);
    if(FAILURE(res))
    {
        ushort_send(state, usCoreRefClock);
        ushort_send(state, usDacRefClock);
        return res;
    }
    
    res = Mo1000_GetCoreFrequency(&Mo1000Board[uNb], &usCoreRefClock, &usDacRefClock);

	mdebug("Mo1000_GetCoreFrequency Fpga Ref: %d, Dac Ref: %d\n", usCoreRefClock, usDacRefClock);
    ushort_send(state, usCoreRefClock);
    ushort_send(state, usDacRefClock);
    return res;
}


 /****************************************************************************
 * NAME : Mo1000_SetTestMode_recv
 ************************************************************************//**
 *
 * This function could be used to configure special test mode (mostly use at production time
 * for test purpose).
 * Here are some details about possible test modes:
 *
 * #eMo1000TestModeOff:
 * All test modes are disabled.
 *
 * #eMo1000TestMode2:
 * When test mode is active, each dac channel group would captured user's specified data patterns
 * generated by the fpga core.
 * This is usefull to verify the reliability of the complete digital data generation chain.
 * The module Mo1000_DoDacCalibration_recv() function must have been done successfully last time
 * before using this function.
 * While test mode is active, user can poll for pattern test errors using Mo1000_GetStatus_recv()
 * function pCompare output parameter as often/long as needed. Errors are cumulative between
 * Mo1000_GetStatus_recv() calls.
 *
 * #eMo1000TestMode1:
 * This mode is similar as #eMo1000TestMode2 explained above, but is mainly reserved for
 * production purpose.
 * While test mode is active, user can poll for pattern test errors using Mo1000_GetStatus_recv()
 * function pCompare output parameter as often/long as needed. Errors are cumulative between
 * Mo1000_GetStatus_recv() calls.
 * @attention
 * The dac data captured in this mode is not necessary the specified fpga patterns
 * because it could be done uncalibrated (so this mode could not be used to verify the
 * reliability of the complete generated data chain).
 *
 * For normal board DAC operation, test mode must be disable after use.
 *
 * @warning
 * - Doing Mo1000_DoDacCalibration_recv() will disable the test mode automatically and revert to normal mode.
 * - The module Mo1000_Init_recv() function must be call at least once before using this function.
 * - To test all bits correctly, patterns must be provided that a bit transition occurs between
 *   the even and odd patterns on each bits that need to be tested.
 * - All dac channels group are tested at the same time.
 * - Dacs Clocks must be initialized correctly with Mo1000_SetClockConfig_recv() before testing patterns.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used
 *   [in] unsigned Test:                Test mode desired
 *   [in] unsigned short usEvenPattern: 16 bits even test pattern used
 *   [in] unsigned short usOddPattern:  16 bits odd test pattern used
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv().
 *
 * @see
 *    Mo1000_DoDacCalibration_recv(), Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/
Result Mo1000_SetTestMode_recv(connection_state * state)
{
    Result res = eMo1000Ok;
    unsigned uNb;
    Mo1000_eTestMode Test;
    unsigned short usEvenPattern;
    unsigned short usOddPattern;

    uNb = uint_recv(state);
	Test = uint_recv(state);
	usEvenPattern = ushort_recv(state);
	usOddPattern = ushort_recv(state);

	mdebug("Mo1000_SetTestMode #%d\n",uNb+1);
	mdebug("Mo1000_SetTestMode Test: %d\n", Test);
	mdebug("Mo1000_SetTestMode Even pattern: %04X, Odd pattern: %04X\n", usEvenPattern, usOddPattern);
    
    res = _Mo1000_Presence(uNb);
    if(FAILURE(res)) return res;
    
    res = Mo1000_SetTestMode(&Mo1000Board[uNb], Test, usEvenPattern, usOddPattern);

    return res;
}


