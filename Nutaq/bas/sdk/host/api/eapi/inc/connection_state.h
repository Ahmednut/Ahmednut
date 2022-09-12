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
 *  @file       : connection_state.h
 *  @brief      : Function definitions for the connection state
 *
 * $Date: 2015/08/10 17:10:38 $
 * $Revision: 1.1 $
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

#ifndef __EAPI_CONNECTION_STATE__
#define __EAPI_CONNECTION_STATE__
#include "bas_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <windows.h>
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

typedef enum connection_mode {
    NORMAL_MODE,
    TURBO_MODE
} connection_mode_t;

typedef struct connection_buffer {
    int length;
    char buffer[4096];
} connection_buffer;

typedef struct connection_state {
#if defined(__GNUC__) || defined(SWIG)
	int stream;
#else
    SOCKET stream;
#endif
    int has_ended;
    connection_mode_t mode;
    connection_buffer send_buffer;    
} connection_state;

Result connection_write(connection_state * state, const char * buf, unsigned size);
Result connection_flush(connection_state * state);

#ifdef __cplusplus
}
#endif

#endif
