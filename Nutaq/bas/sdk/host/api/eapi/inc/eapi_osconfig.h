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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//** 
 *
 * @file       eapi_osconfig.h
 * @brief      Function definitions for the eapi os network configuration
 *
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup EAPI
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
 
 #ifndef __EAPI_OSCONFIG__
#define __EAPI_OSCONFIG__

#ifdef __cplusplus
extern "C" {
#endif

#include "bas_error.h"
#include "connection_state.h"

Result os_config_get_parameter(connection_state * state, char *osconfig_inkey, char **osconfig_outvalue, int *osconfig_outvaluesize);
Result os_config_get_first_parameter(connection_state * state, char **osconfig_outkey, int *osconfig_outkeysize, char **osconfig_outvalue, int *osconfig_outvaluesize);
Result os_config_get_next_parameter(connection_state * state, char **osconfig_outkey, int *osconfig_outkeysize, char **osconfig_outvalue, int *osconfig_outvaluesize);
Result os_config_set_parameter(connection_state * state, char *osconfig_inkey, char *osconfig_invalue);
Result os_config_unset_parameter(connection_state * state, char *osconfig_inkey);
Result os_config_load_file(connection_state * state, char *filename);
Result os_config_save_file(connection_state * state, char *filename);
Result os_config_clear_file(connection_state * state);
Result os_config_apply_file(connection_state * state);
Result os_config_select_file(connection_state * state);
Result os_config_get_first_file(connection_state * state, char **osconfig_outvalue, int *osconfig_outvaluesize);
Result os_config_get_next_file(connection_state * state, char **osconfig_outvalue, int *osconfig_outvaluesize);

enum osconfig_error{
	OSCONFIG_SUCCESS = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_OSCONFIG, 0),
    OSCONFIG_BADPARAMETER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_OSCONFIG, 1),
    OSCONFIG_BADVALUE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_OSCONFIG, 2),
    OSCONFIG_BADFILE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_OSCONFIG, 3),
    OSCONFIG_NOTLOADED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_OSCONFIG, 4),
};

#ifdef __cplusplus
}
#endif



#endif

