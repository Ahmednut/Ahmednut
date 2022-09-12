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
 * @file       sysmon.c  
 * @brief      Read the system monitor temperature and voltage values
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup SYSMON
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
#include <stdio.h>
#include <stdlib.h>

#include "sysmon.h"
#include "sysmon_def.h"
#include "sysmon_internal_def.h"
#include "bas_error.h"

/***************************************************************************
* Function: Sysmon_Open
************************************************************************//**
*
* Create handle to System Monitor core.
*
* @param[in,out] hSysmon
*   Return the allocated System Monitor handle.
*
* @param[in] uiBaseAddress
*   Base address of the AXI System Monitor ADC core
*
* @return
*      return OK on success.
*
***************************************************************************/
Result Sysmon_Open(Sysmon_handle_t * hSysmon, unsigned int uiBaseAddress)
{
    sSysmon_t * Sysmon = malloc(sizeof(sSysmon_t));
    if(Sysmon == NULL)
        return ERR_OUTOFMEMORY;

    Sysmon->uiBaseAddress = uiBaseAddress;

    *hSysmon = (Sysmon_handle_t)Sysmon;

    return OK;
}

/***************************************************************************
* Function: Sysmon_Read
************************************************************************//**
*
* Read the information requested from System Monitor
*
* @param[in] hSysmon
*   Handle to the System Monitor previously opened.
*
* @param[in] type
*   Type of the System Monitor information that will be read.
*
* @param[out] pValue
*   Return the read value at this address on success.
*
* @return
*      return OK on success.
*
***************************************************************************/
Result Sysmon_Read(Sysmon_handle_t hSysmon, Sysmon_eType_t type, float * pValue)
{
    sSysmon_t * Sysmon = (sSysmon_t *)hSysmon;
    unsigned int uiRegAddress;
    unsigned int uiRegValue;

    if(Sysmon == NULL)
        return ERR_INVHANDLE;
    if(pValue == NULL)
        return ERR_INVPTR;

    switch(type)
    {
        case Sysmon_eTemp:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_TEMP_ADDR;
            break;
        case Sysmon_eTempMin:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_TEMP_MIN_ADDR;
            break;
        case Sysmon_eTempMax:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_TEMP_MAX_ADDR;
            break;
        case Sysmon_eVint:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_VINT_ADDR;
            break;
        case Sysmon_eVintMin:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_VINT_MIN_ADDR;
            break;
        case Sysmon_eVintMax:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_VINT_MAX_ADDR;
            break;
        case Sysmon_eVaux:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_VAUX_ADDR;
            break;
        case Sysmon_eVauxMin:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_VAUX_MIN_ADDR;
            break;
        case Sysmon_eVauxMax:
            uiRegAddress = Sysmon->uiBaseAddress + SYSMON_VAUX_MAX_ADDR;
            break;
        default:
            return ERR_INVARG;
    }

    switch(type)
    {
        case Sysmon_eTemp:
        case Sysmon_eTempMin:
        case Sysmon_eTempMax:
            // Read the register
            uiRegValue = *((unsigned int *)uiRegAddress);
            // Take the 10 MSB of the 16-bit value
            uiRegValue = (uiRegValue >> 6) & 0x3FF;
            // Convert the ADC value to temperature
            *pValue = ((float)uiRegValue)*503.975f/1024.0f-273.15f;
            break;

        case Sysmon_eVint:
        case Sysmon_eVintMin:
        case Sysmon_eVintMax:
        case Sysmon_eVaux:
        case Sysmon_eVauxMin:
        case Sysmon_eVauxMax:
            // Read the register
            uiRegValue = *((unsigned int *)uiRegAddress);
            // Take the 10 MSB of the 16-bit value
            uiRegValue = (uiRegValue >> 6) & 0x3FF;
            // Convert the ADC value to voltage
            *pValue = ((float)uiRegValue)/1024.0f*3.0f;
            break;

        default:
            return ERR_INVARG;
    }

    return OK;
}

/***************************************************************************
* Function: Sysmon_Close
************************************************************************//**
*
* Close the System Monitor handle.
*
* @param[in] hSysmon
*   Deallocate the System Monitor handle previously opened.
*
* @return
*      return OK on success.
*
***************************************************************************/
Result Sysmon_Close(Sysmon_handle_t hSysmon)
{
    sSysmon_t * Sysmon = (sSysmon_t *)hSysmon;

    if(Sysmon != NULL)
        free(Sysmon);

    return OK;
}

