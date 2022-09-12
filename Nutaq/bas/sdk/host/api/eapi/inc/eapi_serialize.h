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
 * @file       eapi_serialize.h
 * @brief      Function definitions for the eapi serialization of data
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

#ifndef __EAPI_SERIALIZE__
#define __EAPI_SERIALIZE__

#ifdef __cplusplus
extern "C" {
#endif

#include "bas_types.h"
#include "connection_state.h"

// uint64
void uint64_send(connection_state * state, unsigned long long data);
unsigned long long uint64_recv(connection_state * state);

// uint
void uint_send(connection_state * state, unsigned data);
unsigned uint_recv(connection_state * state);

// uint32 (same as uint)
void uint32_send(connection_state * state, unsigned data);
uint32_t uint32_recv(connection_state * state);

// int
void int_send(connection_state * state, int32_t data);
int32_t int_recv(connection_state * state);

// int32 (same as int)
void int32_send(connection_state * state, int32_t data);
int32_t int32_recv(connection_state * state);

// ushort
void ushort_send(connection_state * state, unsigned short data);
unsigned short ushort_recv(connection_state * state);

// uint16 (same as ushort)
void uint16_send(connection_state * state, uint16_t data);
uint16_t uint16_recv(connection_state * state);

// short
void short_send(connection_state * state, short data);
short short_recv(connection_state * state);

// int16 (same as short)
void int16_send(connection_state * state, int16_t data);
int16_t int16_recv(connection_state * state);

// uchar
void uchar_send(connection_state * state, unsigned char data);
unsigned char uchar_recv(connection_state * state);

// uint8 (same as uchar)
void uint8_send(connection_state * state, uint8_t data);
uint8_t uint8_recv(connection_state * state);

// char
void char_send(connection_state * state, char data);
char char_recv(connection_state * state);

// int8 (same as uhar)
void int8_send(connection_state * state, uint8_t data);
int8_t int8_recv(connection_state * state);

Result cmd_send(connection_state * state, unsigned cmd);
Result result_recv(connection_state * state);

Result buf_recv_alloc(connection_state * state, char ** buf, int * bufsize);
Result buf_free(char * buf);

Result buf_recv(connection_state * state, char * buf, unsigned maxsize);
Result buf_recv_with_size(connection_state * state, char * buf, unsigned * size);
Result buf_send(connection_state * state, const char * buf, unsigned size);

void float_send(connection_state * state, float fData);
float float_recv(connection_state * state);

#ifdef __cplusplus
}
#endif

#endif

