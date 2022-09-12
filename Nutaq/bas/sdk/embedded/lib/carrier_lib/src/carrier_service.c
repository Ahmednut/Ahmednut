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
 * @file      carrier_service.c
 * 
 * @brief     carrier generic library functions.
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup CARRIERLIB
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "bas_error.h"
#include "bas_debug.h"

#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_internal_def.h"


Result Carrier_Close(Carrier_handle_t hCarrier)
{
    Result ret = CARRIER_OK;

    if (hCarrier == NULL)
    {
        ret = CARRIER_NOT_OPEN;
        return ret;
    }

    return hCarrier->pfCarrier_Close(hCarrier);
}


Result Carrier_GetType(Carrier_handle_t hCarrier, Carrier_eType_t * pType)
{
    Result result=CARRIER_OK;
    
    if (hCarrier == NULL) return CARRIER_NOT_OPEN;
    
    if (pType)
    {
        *pType = hCarrier->Type;
    }
    
    return result;
}

Result Carrier_GetFmcConnectorNumMax(Carrier_handle_t hCarrier, unsigned int * puFmcConnectorNumMax)
{
    Result result=CARRIER_OK;
    
    if (hCarrier == NULL) return CARRIER_NOT_OPEN;
    
    if (puFmcConnectorNumMax)
    {
        *puFmcConnectorNumMax = hCarrier->uFmcConnectorNumMax;
    }
    
    return result;
}

Result Carrier_GetFmcBoardNumMax(Carrier_handle_t hCarrier, unsigned int * puFmcBoardNumMax)
{
    Result result=CARRIER_OK;
    
    if (hCarrier == NULL) return CARRIER_NOT_OPEN;
    
    if (puFmcBoardNumMax)
    {
        *puFmcBoardNumMax = hCarrier->uFmcBoardNumMax;
    }
    
    return result;
}


Result Carrier_FmcGetI2cZone(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, unsigned int * I2cZone)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    if(hCarrier->pfCarrier_I2cRead == NULL)
        return CARRIER_INVALID_FCT;

    return hCarrier->pfCarrier_FmcGetI2cZone(hCarrier, connector, I2cZone);
}

Result Carrier_I2cRead(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    if(hCarrier->pfCarrier_I2cRead == NULL)
        return CARRIER_INVALID_FCT;

    return hCarrier->pfCarrier_I2cRead(hCarrier, I2cZone, i2cDeviceAddr, addr, data, size, options);
}

Result Carrier_I2cWrite(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    if(hCarrier->pfCarrier_I2cRead == NULL)
        return CARRIER_INVALID_FCT;

    return hCarrier->pfCarrier_I2cWrite(hCarrier, I2cZone, i2cDeviceAddr, addr, data, size, options);
}

Result Carrier_I2cReadNoWrData(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned char * data, unsigned int size)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    if(hCarrier->pfCarrier_I2cRead == NULL)
        return CARRIER_INVALID_FCT;

    return hCarrier->pfCarrier_I2cReadNoWrData(hCarrier, I2cZone, i2cDeviceAddr, data, size);
}


Result Carrier_I2cScanDevices(Carrier_handle_t hCarrier, unsigned int I2cZone, I2C_stI2cScan *pScanRaw, unsigned char *pucDevices)
{
    Result Ret;
    unsigned char ucDevAddr;

    // Initialize to no detection
    memset(pScanRaw,0,sizeof(I2C_stI2cScan));
    *pucDevices=0;

    // Scan every 7 bits i2c addresses on the bus
    for (ucDevAddr = 0; ucDevAddr < I2C_I2SCAN_MAX_DEV ; ucDevAddr++)
    {
        // Do the i2c check test (to verify presence)
        Ret = Carrier_I2cCheckRaw(hCarrier, I2cZone, ucDevAddr, &pScanRaw->m_ucDevDetect[ucDevAddr]);
        if(FAILURE(Ret)) return Ret;

        // Check if a device is detected at this address
        if (pScanRaw->m_ucDevDetect[ucDevAddr])
        {
            debug("Device 0x%02x : detected.\n",ucDevAddr);

            // Max devices found counter
            (*pucDevices)++;
        }
        else
        {
            debug("Device 0x%02x : not detected.\n",ucDevAddr);
        }
    }

    return CARRIER_OK;
}

Result Carrier_I2cCheckRaw(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char ucDevAddr, unsigned char * pucDetected)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    if(hCarrier->pfCarrier_I2cCheckRaw == NULL)
        return CARRIER_INVALID_FCT;

    return hCarrier->pfCarrier_I2cCheckRaw(hCarrier, I2cZone, ucDevAddr, pucDetected);
}

Result Carrier_usleep(Carrier_handle_t hCarrier, unsigned int usec)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if delay function is correct
    if (hCarrier->pfCarrier_usleep == NULL)
    {
        return CARRIER_INVALID_USLEEPFCT;
    }

    return hCarrier->pfCarrier_usleep(usec);
}

Carrier_pfUsleep Carrier_GetUsleepFunc(Carrier_handle_t hCarrier)
{
    if (hCarrier == NULL)
        return NULL;
    
    return (hCarrier->pfCarrier_usleep);
}

Result Carrier_FmcGetGA(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, unsigned char * ga)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if delay function is correct
    if (hCarrier->pfCarrier_FmcGetGA == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_FmcGetGA(connector,ga);
}

Result Carrier_TClkSetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t state)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if  function is correct
    if (hCarrier->pfCarrier_TClkSetState == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_TClkSetState(hCarrier, tclk, dir, state);
}

Result Carrier_TClkGetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t * state)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_TClkGetState == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_TClkGetState(hCarrier, tclk, dir, state);
}

Result Carrier_TClkSetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t source)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_TClkSetTxSource == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_TClkSetTxSource(hCarrier, tclk, source);
}

Result Carrier_TClkGetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t * source)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_TClkGetTxSource == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_TClkGetTxSource(hCarrier, tclk, source);
}

Result Carrier_FmcBidirClkSetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t connection)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_FmcBidirClkSetConnection == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_FmcBidirClkSetConnection(hCarrier, clk, connection);
}

Result Carrier_FmcBidirClkGetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t * connection)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_FmcBidirClkGetConnection == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_FmcBidirClkGetConnection(hCarrier, clk, connection);
}

Result Carrier_SetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t state)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_SetOscState == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_SetOscState(hCarrier, osc, state);
}

Result Carrier_GetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t * state)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_GetOscState == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_GetOscState(hCarrier, osc, state);
}

Result Carrier_FClkASetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t connection)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_FClkASetConnection == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_FClkASetConnection(hCarrier, connection);
}

Result Carrier_FClkAGetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t * connection)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_FClkAGetConnection == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_FClkAGetConnection(hCarrier, connection);
} 

Result Carrier_GetModuleBaseAddr(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int index, unsigned int * addr)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_GetModuleBaseAddr == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_GetModuleBaseAddr(hCarrier, module, index, addr);
} 

Result Carrier_GetModuleSize(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * size)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_GetModuleSize == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_GetModuleSize(hCarrier, module, size);
}

Result Carrier_GetModuleMaxNumber(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * num)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_GetModuleMaxNumber == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_GetModuleMaxNumber(hCarrier, module, num);
} 

Result Carrier_GetMemSpaceBaseAddr(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * addr)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_GetMemSpaceBaseAddr == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_GetMemSpaceBaseAddr(hCarrier, space, addr);
} 

Result Carrier_GetMemSpaceSize(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * size)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_GetMemSpaceSize == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_GetMemSpaceSize(hCarrier, space, size);
}

Result Carrier_CustomRegisterWrite(Carrier_handle_t hCarrier, unsigned int index, unsigned int value)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_CustomRegisterWrite == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_CustomRegisterWrite(hCarrier, index, value);
} 

Result Carrier_CustomRegisterRead(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_CustomRegisterRead == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_CustomRegisterRead(hCarrier, index, value);
} 

Result Carrier_LedSetValue(Carrier_handle_t hCarrier, unsigned int index, unsigned int value)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_LedSetValue == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_LedSetValue(hCarrier, index, value);
} 

Result Carrier_LedGetValue(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_LedGetValue == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_LedGetValue(hCarrier, index, value);
} 

Result Carrier_Reboot(Carrier_handle_t hCarrier)
{
    if (hCarrier == NULL)
        return CARRIER_NOT_OPEN;

    // Check if function is correct
    if (hCarrier->pfCarrier_Reboot == NULL)
    {
        return CARRIER_INVALID_FCT;
    }

    return hCarrier->pfCarrier_Reboot(hCarrier);
} 
