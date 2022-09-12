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
 * @file      eapi_lvdsxinxout.c
 * 
 * @brief     Function definitions for the eapi lvdsxinxout control
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
 ****************************************************************************/
#include "cce.h"
#include "eapi_lvdsxinxout.h"
#include "eapi_serialize.h"
#include "fmclvds_pcf8574_defines.h"


Result fmclvds_reset_send(connection_state * state)
{
    cmd_send(state, FMCLVDS_INIT);
    return result_recv(state);
}

Result fmclvds_powerup_send(connection_state * state)
{
    cmd_send(state, FMCLVDS_POWERUP);
    return result_recv(state);
}

Result fmclvds_presence_send(connection_state * state)
{
    cmd_send(state, FMCLVDS_PRESENCE);
    return result_recv(state);
}

Result fmclvds_select_send(connection_state * state, unsigned nb)
{
    cmd_send(state, FMCLVDS_SELECT);
    uint_send(state, nb);
    return result_recv(state);
}

Result fmclvds_set_channel_dir_send(connection_state * state, FMCLVDS_GROUPS group, FMCLVDS_DIR dir)
{
    cmd_send(state, FMCLVDS_SETDIR);
    uint_send(state, group);
    uint_send(state, dir);
    return result_recv(state);
}

Result fmclvds_set_channel_powerdown_send(connection_state * state, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_pwdn pwdn)
{
    cmd_send(state, FMCLVDS_SET_CHANNEL_PWDN);
    uint_send(state, group);
    uint_send(state, pwdn);
    return result_recv(state);
}

Result fmclvds_set_channel_preemphasis_send(connection_state * state, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_preemp preemp)
{
    cmd_send(state, FMCLVDS_SET_CHANNEL_PREEMP);
    uint_send(state, group);
    uint_send(state, preemp);
    return result_recv(state);
}




