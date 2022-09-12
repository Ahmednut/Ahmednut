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
 * @file      eapi_osconfig.c
 *
 * @brief     Function definitions for the eapi os network configuration
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
 ***************************************************************************/
#include <string.h>
#include "eapi_osconfig.h"
#include "cce.h"
#include "eapi_serialize.h"

Result string_send(connection_state * state, const char * str)
{
    size_t size;
    size = strlen(str);
    size++; //include NULL (\0) character
    return buf_send(state, str, (unsigned int)size);
}

Result os_config_get_parameter(connection_state * state, char *key, char **value, int *value_size)
{
    cmd_send(state, OSCONFIG_GET_PARAMETER_VALUE);
    string_send(state, key);
    buf_recv_alloc(state, value, value_size);
    return result_recv(state);
}

Result os_config_get_first_parameter(connection_state * state, char **key, int *key_size, char **value, int *value_size)
{
    cmd_send(state, OSCONFIG_GET_FIRST_PARAMETER);
    buf_recv_alloc(state, key, key_size);
    buf_recv_alloc(state, value, value_size);
    return result_recv(state);
}

Result os_config_get_next_parameter(connection_state * state, char **key, int *key_size, char **value, int *value_size)
{
    cmd_send(state, OSCONFIG_GET_NEXT_PARAMETER);
    buf_recv_alloc(state, key, key_size);
    buf_recv_alloc(state, value, value_size);
    return result_recv(state);
}

Result os_config_set_parameter(connection_state * state, char *key, char *value)
{
    cmd_send(state, OSCONFIG_SET_PARAMETER_VALUE);
    string_send(state, key);
    string_send(state, value);
    return result_recv(state);
}

Result os_config_unset_parameter(connection_state * state, char *key)
{
    cmd_send(state, OSCONFIG_UNSET_PARAMETER);
    string_send(state, key);
    return result_recv(state);
}

Result os_config_load_file(connection_state * state, char *filename)
{
    cmd_send(state, OSCONFIG_LOAD_FILE);
    string_send(state, filename);
    return result_recv(state);
}

Result os_config_save_file(connection_state * state, char *filename)
{
    cmd_send(state, OSCONFIG_SAVE_FILE);
    string_send(state, filename);
    return result_recv(state);
}

Result os_config_clear_file(connection_state * state)
{
    cmd_send(state, OSCONFIG_CLEAR_FILE);
    return result_recv(state);
}

Result os_config_apply_file(connection_state * state)
{
    cmd_send(state, OSCONFIG_APPLY_FILE);
    return result_recv(state);
}

Result os_config_select_file(connection_state * state)
{
    cmd_send(state, OSCONFIG_SELECT_FILE);
    return result_recv(state);
}

Result os_config_get_first_file(connection_state * state, char **value, int *value_size)
{
    cmd_send(state, OSCONFIG_GET_FIRST_FILE);
    buf_recv_alloc(state, value, value_size);
    return result_recv(state);
}

Result os_config_get_next_file(connection_state * state, char **value, int *value_size)
{
    cmd_send(state, OSCONFIG_GET_NEXT_FILE);
    buf_recv_alloc(state, value, value_size);
    return result_recv(state);
}

