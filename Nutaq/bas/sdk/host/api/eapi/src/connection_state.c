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
 * @file      connection_state.c
 * 
 * @brief     Function definitions for the connection state
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
#include "connection_state.h"
#include "cce.h"
#include "eapi.h"
#include <string.h>

//debug or no debug
#if 0
#include <stdio.h>
#define debug(f, ...) printf(f, __VA_ARGS__)
#else
#define debug(f, ...)
#endif

Result connection_write(connection_state * state, const char * buf, unsigned size)
{
    Result ret = OK;
    //debug("connection_write: %p  %i\n", buf, size);
    if(size > sizeof(state->send_buffer.buffer))
    { //don't fit in buffer, send it now
        int i;
        int remaining = size;
        ret = connection_flush(state);
        if(FAILURE(ret)) return ret;
        while (remaining > 0)
        {
            i = send(state->stream, &buf[size-remaining], remaining, 0);
            if(i<=0) return EAPIDISCONNECT;
            remaining-=i;
        }
    }else{ // buffer it
        if (state->send_buffer.length + size > sizeof(state->send_buffer.buffer))
        {
            ret = connection_flush(state);
        }
        memcpy(state->send_buffer.buffer + state->send_buffer.length, buf, size);
        state->send_buffer.length += size;
    }
    return ret;
}

Result connection_flush(connection_state * state)
{
    int i;
    int remaining = state->send_buffer.length;
  //  debug("connection_flush: buf_length %i\n", state->send_buffer.length);
    while (remaining > 0)
    {
        i = send(state->stream, &state->send_buffer.buffer[state->send_buffer.length-remaining], remaining, 0);
        if(i<=0) return EAPIDISCONNECT;
        remaining-=i;
    }
    state->send_buffer.length = 0;
    return OK;
}

