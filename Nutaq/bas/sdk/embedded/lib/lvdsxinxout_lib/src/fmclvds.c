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
 * @file       fmclvds.c
 * @brief      Initialize and configure the FMC LVDS card
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCLVDS
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
#include "fmclvds.h"
#include "fmclvds_registers.h"
#include "fmclvds_defines.h"
#include "fmclvds_com_wrapper.h"
#include "fmclvds_com_wrapper_defines.h"
#include "bas_error.h"
#include "bas_debug.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"
#include "FMCEEprom_eeprom_id.h"

Result _fmclvds_get_i2c_addr(Carrier_handle_t hCarrier, unsigned int pos, unsigned char * pExpanderAddr)
{
    Result ret;
    unsigned char ga;
    Carrier_eFmcBoardNum_t board;
    Carrier_eFmcConnectorNum_t connector;
    char FmcInfoType[16];
    
    board = (Carrier_eFmcBoardNum_t)(pos % Carrier_eFmcBoardLast);
    connector = (Carrier_eFmcConnectorNum_t)(pos / Carrier_eFmcBoardLast);
    
    ret = Carrier_FmcGetInfo(hCarrier, connector, board,
        CARRIER_INFO_BOARDTYPE,
        FmcInfoType, sizeof(FmcInfoType));
    if(FAILURE(ret)) {return ret;}

    ret = Carrier_FmcGetGA(hCarrier, connector, &ga);
    if(FAILURE(ret)) {return ret;}

    if(ga==0)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_LVDSIO_E)==0)
            *pExpanderAddr = EXPANDER_GA00_BOTTOM;
        else
            *pExpanderAddr = EXPANDER_GA00_TOP;
    }
    else if(ga==1)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_LVDSIO_E)==0)
            *pExpanderAddr = EXPANDER_GA01_BOTTOM;
        else
            *pExpanderAddr = EXPANDER_GA01_TOP;
    }
    else if(ga==2)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_LVDSIO_E)==0)
            *pExpanderAddr = EXPANDER_GA10_BOTTOM;
        else
            *pExpanderAddr = EXPANDER_GA10_TOP;
    }
    else
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_LVDSIO_E)==0)
            *pExpanderAddr = EXPANDER_GA11_BOTTOM;
        else
            *pExpanderAddr = EXPANDER_GA11_TOP;
    }

    return OK;
}


Result fmclvds_open(fmclvds * fmclvds, fmclvds_core * core, unsigned int pos, Carrier_handle_t hCarrier)
{
  Result ret;
  unsigned char ucExpanderAddr;
  Carrier_eFmcConnectorNum_t connector;
  unsigned int I2cZone;
  
  connector = (Carrier_eFmcConnectorNum_t)(pos / Carrier_eFmcBoardLast);
  
  fmclvds->hCarrier = hCarrier;
  fmclvds->core = core;
  fmclvds->pos = pos;
  
  ret = _fmclvds_get_i2c_addr(hCarrier, pos, &ucExpanderAddr);
  if(FAILURE(ret)) return ret;
  
  // Get the I2C zone based on the FMC position
  ret = Carrier_FmcGetI2cZone(hCarrier, connector, &I2cZone);
  if(FAILURE(ret)) return ret;

  ret = FMCLVDS_com_open(&fmclvds->com, hCarrier, I2cZone, ucExpanderAddr);
  if(FAILURE(ret)) return ret;

  fmclvds->core->control.CoreResetPulse = 1;

  return OK;
}

Result fmclvds_checkcoreid(fmclvds *fmclvds, FMCLVDS_CORE_DETECT *core_detect)
{
  // Verify instance if lvds core id is detected
  if(fmclvds->core->coreversid.coreid == FMCLVDS_CORE_ID)
      *core_detect = FMCLVDS_COREDETECTED;
  else
      *core_detect = FMCLVDS_CORENONE;

  return OK;
}

Result fmclvds_setdir(fmclvds * fmclvds, FMCLVDS_GROUPS group, FMCLVDS_DIR dir)
{
    switch(group)
    {
    default:
    case FMCLVDS_GROUP_0:
      fmclvds->core->control.dirGroup0 = dir;
      break;
    case FMCLVDS_GROUP_1:
      fmclvds->core->control.dirGroup1 = dir;
      break;
    }
    return OK;
}

Result fmclvds_close(fmclvds * fmclvds)
{
    return OK;
}

