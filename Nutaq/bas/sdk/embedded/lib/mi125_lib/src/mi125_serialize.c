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
 * @file       mi125_serialize.c
 * @brief      mi125 module related additionnal functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI125
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
#include "mi125_serialize.h"
#include "MI125_mi125_defines.h"
#include "eapi_serialize.h"


 /****************************************************************************
 * NAME : mi125_set_config_param_send
 ************************************************************************//**
 *
 * Configure the board with user specified custom options.
 * This is internal function used by MI125_mi125_set_config_send() to send
 * the config information to the module.  
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] config
 *    Board channel config structure to use.
 *
 * @return NONE   
 * 
 ***************************************************************************/ 
void mi125_set_config_param_send(connection_state * state,  MI125_mi125_config *config)
{
    uint_send(state, config->groupch);
    uint_send(state, config->lvds);
    uint_send(state, config->randmode);
    uint_send(state, config->binmode);
}


 /****************************************************************************
 * NAME : mi125_set_config_param_recv
 ************************************************************************//**
 *
 * Get the board user specified custom options.
 * This is internal function used by CCE to read the config information to the module.  
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] config
 *    Board channel config structure to use.
 *
 * @return NONE   
 * 
 ***************************************************************************/ 
void mi125_set_config_param_recv(connection_state * state, MI125_mi125_config *config)
{
    config->groupch = uint_recv(state);
    config->lvds = uint_recv(state);
    config->randmode = uint_recv(state);
    config->binmode = uint_recv(state);
}
