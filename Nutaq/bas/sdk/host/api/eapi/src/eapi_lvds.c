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
 * @file      eapi_lvds.c
 * 
 * @brief     Function definitions for the eapi lvds control
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
#include "eapi_lvds.h"
#include "eapi_serialize.h"

Result lvds_presence_send(connection_state * state, unsigned int group)
{    
    cmd_send(state, LVDS_PRESENCE);
    uint_send(state, group);
    
    return result_recv(state);
}

Result lvds_getmode_send(connection_state * state, unsigned int group)
{
    cmd_send(state, LVDS_GETMODE);
    uint_send(state, group);
    
    return result_recv(state);
}

Result lvds_setoutputenable_send(connection_state * state, unsigned int group, unsigned int outputenable)
{
    cmd_send(state, LVDS_SETOUTPUTENABLE);
    uint_send(state, group);
    uint_send(state, outputenable);
    
    return result_recv(state);
}

Result lvds_setvalue_send(connection_state * state, unsigned int group, unsigned int value)
{
    cmd_send(state, LVDS_SETVALUE);
    uint_send(state, group);
    uint_send(state, value);
    
    return result_recv(state);
}

Result lvds_getvalue_send(connection_state * state, unsigned int group, unsigned int *value)
{
    cmd_send(state, LVDS_GETVALUE);
    uint_send(state, group);
    
    *value = uint_recv(state);
    return result_recv(state);
}

Result lvds_reset_send(connection_state * state, unsigned int group)
{
    cmd_send(state, LVDS_RESET);
    uint_send(state, group);
    
    return result_recv(state);
}

Result lvds_reset_fifo_send(connection_state * state, unsigned int group)
{
    cmd_send(state, LVDS_RESET_FIFO);
    uint_send(state, group);
    
    return result_recv(state);
}


