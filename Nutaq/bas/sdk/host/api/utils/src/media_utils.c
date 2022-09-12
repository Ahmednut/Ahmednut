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
 * @file      media_utils.c
 * 
 * @brief     Functions used for different medias
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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
#include "bas_types.h"
#include "media_types.h"
#include "eapi.h"
#include "eapi_carrier.h"
#include "carrier_service_def.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_eth.h"
#include "rtdex_pci.h"
#include "bas_error.h"
#include "media_utils.h"

Result DetectMedia(connection_state * state, eMediaType *eMedia)
{
    Result res = OK;
	unsigned int uiRTDExGigeBaseAddr;
    unsigned int uCoreId, uVer;
    RTDExPerseusParamsPcie perseusParams;

    *eMedia = 0xFF;

	res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRTDExGigE, 0, &uiRTDExGigeBaseAddr);
    if(FAILURE(res)) return res;

    // detect Ethernet
    res = RTDExGetCoreIdVerEth( state, uiRTDExGigeBaseAddr, &uCoreId, &uVer);
    if(FAILURE(res)) return res;

    if( uCoreId == RTDEX_ETH_CORE_ID)
    {
        *eMedia = eMediaEth;        
        return OK;
    }

    // detect PCIe
    res = RTDExInitParamsPcie( &perseusParams, state);
    if(FAILURE(res)) return res;

    res = RTDExGetCoreIdVerPcie( &perseusParams, &uCoreId, &uVer);
    if(FAILURE(res)) return res;

    if( uCoreId == RTDEX_PCIE_CORE_ID)
    {
        *eMedia = eMediaPcie;
        return OK;
    }

    // Failed to detect
    return RTDEXMEDIANOTSUPPORTED;
}
 


