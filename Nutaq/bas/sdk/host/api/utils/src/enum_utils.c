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
 * @file      enum_utils.c
 * 
 * @brief     Functions for module enumeration
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bas_types.h"
#include "media_types.h"
#include "eapi.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_eth.h"
#include "buff_fct.h"
#include "enum_utils.h"
#include "eapi_fmcvita.h"

#define ENUM_PDEBUG(f, ...) {if (printflg) printf(f, __VA_ARGS__);}

/**
*****************************************************************************
* Function: iGetModulesInfo
*****************************************************************************
*
* Reads system module information for the specified number of module. 
*
* @param [in]  hConnState
*      Pointer to a valid connection state.
*
* @param [in]  nmod
*      number of modules to scan to get information.
*      WARNING: scan vector must be the size of nmod to hold sufficient information holder
*               for each module to scan.
*
* @param [in]  printflg
*      when !=0 will print the module information.
*
* @param [out]  scan
*      Vector of module information structure. Vector must be the size of the expected
*      number of modules to read. Will returns modules information.
*
* @return
*      Success or error code.
*
****************************************************************************/                              
adp_result_t iGetModulesInfo(connection_state *hConnState,  int nmod, int printflg, ENUM_UTILS_enuminfo scan[])
{
    int inst;
    adp_result_t res;
    
    // Scan for the number of specified boards the carrier/module information
    for (inst = 0; inst < nmod; inst++)
    {
        ENUM_PDEBUG("\nBoard #%d:\n", inst+1);

		// Get module state in this board position
		scan[inst].carrier_modstate[0]=0;
		res = FMCEEprom_eeprom_get_board_state_send(hConnState, inst+1, scan[inst].carrier_modstate, sizeof(scan[inst].carrier_modstate));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_board_state_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("%s\n",scan[inst].carrier_modstate);

		// Get module type in this board position
		scan[inst].carrier_boardtype[0]=0;
		res = FMCEEprom_eeprom_get_board_type_send(hConnState, inst+1, scan[inst].carrier_boardtype, sizeof(scan[inst].carrier_boardtype));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_board_type_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Identified board type: %s\n",scan[inst].carrier_boardtype);

		// Get module fpga core id in this board position
		scan[inst].carrier_modcoretype[0]=0;
		res = FMCEEprom_eeprom_get_core_send(hConnState, inst+1, scan[inst].carrier_modcoretype, sizeof(scan[inst].carrier_modcoretype));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_core_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Board FPGA core id: %s\n",scan[inst].carrier_modcoretype);

		// Get module fpga core name in this board position
		scan[inst].carrier_modcorename[0]=0;
		res = FMCEEprom_eeprom_get_corename_send(hConnState, inst+1, scan[inst].carrier_modcorename, sizeof(scan[inst].carrier_modcorename));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_corename_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Board FPGA core name: %s\n",scan[inst].carrier_modcorename);

		// Get carrier vadj setting in this board position
		scan[inst].carrier_vadjsettings[0]=0;
		res = FMCEEprom_eeprom_get_carriervadj_send(hConnState, inst+1, scan[inst].carrier_vadjsettings, sizeof(scan[inst].carrier_vadjsettings));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_carriervadj_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Board vAdj carrier setting: %s\n",scan[inst].carrier_vadjsettings);

		// Get module identification setting in this board position
		scan[inst].module_idname[0]=0;
		res = FMCEEprom_eeprom_get_idstring_send(hConnState, inst+1, scan[inst].module_idname, sizeof(scan[inst].module_idname));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_idstring_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Module Identification: %s\n",scan[inst].module_idname);
		
		// Get module manufacturer in this board position
		scan[inst].module_manuf[0]=0;
		res = FMCEEprom_eeprom_get_manuf_send(hConnState, inst+1, scan[inst].module_manuf, sizeof(scan[inst].module_manuf));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_manuf_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Module Manufacturer: %s\n", scan[inst].module_manuf);
		
		// Get module part number in this board position
		scan[inst].module_partnumber[0]=0;
		res = FMCEEprom_eeprom_get_partnum_send(hConnState, inst+1, scan[inst].module_partnumber, sizeof(scan[inst].module_partnumber));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_partnum_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Module Part Number: %s\n",scan[inst].module_partnumber);
		
		// Get module serial number in this board position
		scan[inst].module_serialnumber[0]=0;
		res = FMCEEprom_eeprom_get_serialnum_send(hConnState, inst+1, scan[inst].module_serialnumber, sizeof(scan[inst].module_serialnumber));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_serialnum_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Module Serial Number: %s\n",scan[inst].module_serialnumber);

		// Get module hardware revision in this board position
		scan[inst].module_hardrev[0]=0;
		res = FMCEEprom_eeprom_get_hardrev_send(hConnState, inst+1, scan[inst].module_hardrev, sizeof(scan[inst].module_hardrev));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_hardrev_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Module Hardware Revision: %s\n",scan[inst].module_hardrev);
		
		// Get module firmware revision in this board position
		scan[inst].module_firmrev[0]=0;
		res = FMCEEprom_eeprom_get_firmrev_send(hConnState, inst+1, scan[inst].module_firmrev, sizeof(scan[inst].module_firmrev));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_firmrev_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}

        ENUM_PDEBUG("Module Firmware Revision: %s\n",scan[inst].module_firmrev);
		
		// Get module vadj info in this board position
		scan[inst].module_vadjinfo[0]=0;
		res = FMCEEprom_eeprom_get_modvadj_send(hConnState, inst+1, scan[inst].module_vadjinfo, sizeof(scan[inst].module_vadjinfo));
		if(FAILURE(res))
		{
			//printf("FMCEEprom_eeprom_get_modvadj_send FAILED: 0x%08X\n",RESULT_CODE(res));
			return (res);
		}
        
		ENUM_PDEBUG("%s \n",scan[inst].module_vadjinfo);
    }
    
    return(OK);
}
