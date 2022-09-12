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
 * @file       Ina226.c
 * @brief      Temperature monitoring functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup DRIVERS
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "bas_types.h"
#include "bas_types.h"

#include "Ina226.h"
#include "DevCom_defines.h"

// Structure for module operation
typedef struct Ina226_stIna226 {
    DevCom_hCom hCom;
    uint32_t u32DeviceId;
    float fCurrentLsb;
    float fVoltageLsb;
    float fPowerLsb;
    Ina226_eMode Mode;
    Ina226_eAveraging Averaging;
    Ina226_eConvTime BusConvTime;
    Ina226_eConvTime ShuntConvTime;
    uint8_t u8IsBlocking;
    int (*pfUsleep)(unsigned int usec);
}Ina226_stIna226;

const uint16_t AveragingTable[8] = {1, 4, 16, 64, 128, 256, 512, 1024};

const uint16_t ConvTimeTable[8] = {140, 204, 332, 588, 1100, 2116, 4156, 8244};

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/
#define CONFIGURATION_REG   0x00
#define SHUNT_VOLTAGE_REG   0x01
#define BUS_VOLTAGE_REG     0x02
#define POWER_REG           0x03
#define CURRENT_REG         0x04
#define CALIBRATION_REG     0x05
#define MASK_ENABLE_REG     0x06
#define ALERT_LIMIT_REG     0x07
#define DIE_ID_REG          0xFF

/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/
static Result _Ina226_GetTotalConvTime(Ina226_hIna226 hIna226, uint32_t * pu32Time);
static Result _Ina226_Trigger(Ina226_hIna226 hIna226);
static Result _Ina226_WaitReady(Ina226_hIna226 hIna226);

/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/
static Result _Ina226_GetTotalConvTime(Ina226_hIna226 hIna226, uint32_t * pu32Time)
{
    if(hIna226 == NULL) return ERR_INVHANDLE;

    uint32_t u32TotalConvTime;
    uint32_t u32AveragingNum;

    u32TotalConvTime = ConvTimeTable[(uint8_t)hIna226->BusConvTime]
        + ConvTimeTable[(uint8_t)hIna226->ShuntConvTime];

    u32AveragingNum = AveragingTable[(uint8_t)hIna226->Averaging];

    *pu32Time = u32TotalConvTime * u32AveragingNum;

    return eIna226Ok;
}

static Result _Ina226_Trigger(Ina226_hIna226 hIna226)
{
    Result res;
    uint16_t u16ConfReg;

    // Write Configuration register to trigger another acquisition
    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;
    res = DevCom_Write(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    return eIna226Ok;
}

static Result _Ina226_WaitReady(Ina226_hIna226 hIna226)
{
    const uint16_t CONVERSION_READY_FLAG_MASK = (1<<3);
    const uint32_t USLEEP_TIME = 100;

    Result res;
    uint32_t u32ExpectedTime;
    uint32_t u32ElapsedTime = 0;
    uint16_t u16MaskReg = 0;

    res = _Ina226_GetTotalConvTime(hIna226, &u32ExpectedTime);
    if(FAILURE(res)) return res;

    while(1)
    {
        // Get Conversion Ready Flag
        res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, MASK_ENABLE_REG, &u16MaskReg);
        if(FAILURE(res)) return res;

        // Verify is value is ready to be read
        if(u16MaskReg & CONVERSION_READY_FLAG_MASK)
            break;

        // Verify if timeout is exceeded
        // Twice the expected time
        if(u32ElapsedTime > (2*u32ExpectedTime))
        {
            return eIna226Timeout;
        }

        hIna226->pfUsleep(USLEEP_TIME);
        u32ElapsedTime += USLEEP_TIME;
    }

    return eIna226Ok;
}

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

Result Ina226_Open(Ina226_hIna226 * phIna226, DevCom_hCom hCom, uint32_t u32DeviceId, int (*pfUsleep)(unsigned int usec))
{
    Ina226_hIna226 hIna226;

    // Allocate Generic Com wrapper structure
    hIna226 = (Ina226_hIna226)malloc(sizeof(Ina226_stIna226));
    if(hIna226 == NULL) return ERR_OUTOFMEMORY;
    memset(hIna226,0,sizeof(Ina226_stIna226));
    *phIna226 = hIna226;

    hIna226->hCom = hCom;
    hIna226->u32DeviceId = u32DeviceId;
    hIna226->pfUsleep = pfUsleep;

    return eIna226Ok;
}

void Ina226_Close(Ina226_hIna226 * phIna226)
{
    if(*phIna226)
    {
        free(*phIna226);
        *phIna226 = NULL;
    }

    return;
}

Result Ina226_Init(Ina226_hIna226 hIna226, float fRshunt, float fCurrentLsb)
{
    Result res;
    uint16_t u16CalibReg;
    uint16_t u16ConfReg;

    if(hIna226 == NULL) return ERR_INVHANDLE;

    u16CalibReg = (uint16_t)(0.00512f/fCurrentLsb/fRshunt);

    // Verify if the value exceed 15 bits
    if(u16CalibReg & 0x8000) return ERR_INVARG;

    // Set Reset
    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;
    u16ConfReg |= 0x8000;
    res = DevCom_Write(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    // Clear Reset
    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;
    u16ConfReg &= ~(0x8000);
    res = DevCom_Write(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    // Default values after reset
    hIna226->Mode = eIna226ModeContinuous;
    hIna226->Averaging = eIna226Averaging1;
    hIna226->BusConvTime = eIna226ConvTime1100us;
    hIna226->ShuntConvTime = eIna226ConvTime1100us;

    // Set Calib
    res = DevCom_Write(hIna226->hCom, hIna226->u32DeviceId, CALIBRATION_REG, &u16CalibReg);
    if(FAILURE(res)) return res;

    hIna226->fCurrentLsb = fCurrentLsb;
    hIna226->fVoltageLsb = 0.00125;
    hIna226->fPowerLsb = hIna226->fCurrentLsb * hIna226->fVoltageLsb * 20000.0f;

    return eIna226Ok;
}


Result Ina226_SetMode(Ina226_hIna226 hIna226, Ina226_eMode Mode, uint8_t u8IsBlocking)
{
    Result res;
    uint16_t u16ConfReg;

    if(hIna226 == NULL) return ERR_INVHANDLE;

    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    // Clear mode bits
    u16ConfReg &= ~(0x7);
    // Set mode bits
    u16ConfReg |= ((uint16_t)Mode & 0x7);

    res = DevCom_Write(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    hIna226->Mode = Mode;
    hIna226->u8IsBlocking = u8IsBlocking;

    return eIna226Ok;
}

Result Ina226_SetConversionTime(Ina226_hIna226 hIna226, Ina226_eConvTime BusConvTime, Ina226_eConvTime ShuntConvTime)
{
    Result res;
    uint16_t u16ConfReg;

    if(hIna226 == NULL) return ERR_INVHANDLE;

    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    // Clear time bits
    u16ConfReg &= ~(0x7 << 3);
    u16ConfReg &= ~(0x7 << 6);
    // Set time bits
    u16ConfReg |= (((uint16_t)BusConvTime & 0x7) << 6);
    u16ConfReg |= (((uint16_t)ShuntConvTime & 0x7) << 3);

    res = DevCom_Write(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    hIna226->BusConvTime = BusConvTime;
    hIna226->ShuntConvTime = ShuntConvTime;

    return eIna226Ok;
}

Result Ina226_SetAveraging(Ina226_hIna226 hIna226, Ina226_eAveraging Averaging)
{
    Result res;
    uint16_t u16ConfReg;

    if(hIna226 == NULL) return ERR_INVHANDLE;

    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    // Clear time bits
    u16ConfReg &= ~(0x7 << 9);
    // Set time bits
    u16ConfReg |= (((uint16_t)Averaging & 0x7) << 9);

    res = DevCom_Write(hIna226->hCom, hIna226->u32DeviceId, CONFIGURATION_REG, &u16ConfReg);
    if(FAILURE(res)) return res;

    hIna226->Averaging = Averaging;

    return eIna226Ok;
}

Result Ina226_ReadBusVoltage(Ina226_hIna226 hIna226, float * pfVoltage)
{
    Result res;
    uint16_t u16VoltageReg;

    if(hIna226->Mode == eIna226ModePowerDown) return eIna226InvMode;

    if(pfVoltage == NULL) return eIna226InvArg;

    if(hIna226->Mode <= eIna226ModeTrigger)
    {
        res = _Ina226_Trigger(hIna226);
        if(FAILURE(res)) return res;
    }

    if(hIna226->u8IsBlocking)
    {
        res = _Ina226_WaitReady(hIna226);
        if(FAILURE(res)) return res;
    }

    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, BUS_VOLTAGE_REG, &u16VoltageReg);
    if(FAILURE(res)) return res;
    
    *pfVoltage = ((float)u16VoltageReg) * hIna226->fVoltageLsb;

    return eIna226Ok;
}

Result Ina226_ReadPower(Ina226_hIna226 hIna226, float * pfPower)
{
    Result res;
    uint16_t u16PowerReg;

    if(hIna226->Mode == eIna226ModePowerDown) return eIna226InvMode;

    if(pfPower == NULL) return eIna226InvArg;

    if(hIna226->Mode <= eIna226ModeTrigger)
    {
        res = _Ina226_Trigger(hIna226);
        if(FAILURE(res)) return res;
    }

    if(hIna226->u8IsBlocking)
    {
        res = _Ina226_WaitReady(hIna226);
        if(FAILURE(res)) return res;
    }

    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, POWER_REG, &u16PowerReg);
    if(FAILURE(res)) return res;
    
    *pfPower = ((float)u16PowerReg) * hIna226->fPowerLsb;

    return eIna226Ok;
}

Result Ina226_ReadCurrent(Ina226_hIna226 hIna226, float * pfCurrent)
{
    Result res;
    int16_t i16CurrentReg;

    if(hIna226->Mode == eIna226ModePowerDown) return eIna226InvMode;

    if(pfCurrent == NULL) return eIna226InvArg;

    if(hIna226->Mode <= eIna226ModeTrigger)
    {
        res = _Ina226_Trigger(hIna226);
        if(FAILURE(res)) return res;
    }

    if(hIna226->u8IsBlocking)
    {
        res = _Ina226_WaitReady(hIna226);
        if(FAILURE(res)) return res;
    }

    res = DevCom_Read(hIna226->hCom, hIna226->u32DeviceId, CURRENT_REG, &i16CurrentReg);
    if(FAILURE(res)) return res;

    *pfCurrent = ((float)i16CurrentReg) * hIna226->fCurrentLsb;

    return eIna226Ok;
}



