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
 * @file      eapi_serialize.c
 * 
 * @brief     Function definitions for the eapi serialization of data
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
#include "bas_types.h"
#include "eapi_serialize.h"
#include "cce.h"
#include "eapi.h"
#include "bas_debug.h"
#include "stdlib.h"

#ifdef __linux__
  #include <endian.h>
  #define IS_LITTLE_ENDIAN (__BYTE_ORDER == __LITTLE_ENDIAN)
#elif defined(_MSC_VER) // : windows --> little endian
  #define IS_LITTLE_ENDIAN 1
#else
  #error "what is endianness ??"
#endif


#define BUFFERSIZE 4096

#if _MSC_VER < 1700
#if IS_LITTLE_ENDIAN
static uint64_t htonll(uint64_t v)
{
   uint8_t u;
   uint8_t uc[ 8];

   for( u = 0; u < 8; u++)
      uc[ u] = ((uint8_t*)&v)[ 7 - u];
   return *(uint64_t *)uc;
}
#else
	#define  htonll(a) (a)
#endif



static uint64_t ntohll(uint64_t v) {
    union { uint32_t lv[2]; uint64_t llv; } u;
    u.llv = v;
    return ((uint64_t)ntohl(u.lv[0]) << 32) | (uint64_t)ntohl(u.lv[1]);
}
#endif // _MSC_VER == 1500


void uint64_send(connection_state * state, unsigned long long data)
{
    Result ret;
    data = htonll(data);
    ret = connection_write(state, (char*)&data, sizeof(data));
    if(FAILURE(ret)) state->has_ended=1;
}


unsigned long long uint64_recv(connection_state * state)
{
    Result ret;
    unsigned long long data;
    int i;
    ret = connection_flush(state);
    if(FAILURE(ret)) state->has_ended=1;
    i = recv(state->stream, (char*)&data, sizeof(data), 0);
    data = ntohll(data);
    if(i!=sizeof(data)) state->has_ended=1;
    return data;
}

void uint_send(connection_state * state, unsigned data)
{
    Result ret;
    data = htonl(data);
    ret = connection_write(state, (char*)&data, sizeof(data));
    if(FAILURE(ret)) state->has_ended=1;
}


unsigned uint_recv(connection_state * state)
{
    Result ret;
    unsigned data;
    int i;
    ret = connection_flush(state);
    if(FAILURE(ret)) state->has_ended=1;
    i = recv(state->stream, (char*)&data, sizeof(data), 0);
    data = ntohl(data);
    if(i!=sizeof(data)) state->has_ended=1;
    return data;
}

void int_send(connection_state * state, int32_t data)
{
    uint_send(state, *((uint32_t *)(&data)));
}


int32_t int_recv(connection_state * state)
{
    uint32_t i32Data;
    i32Data = uint_recv(state);
    
    return *((int32_t *)(&i32Data));
}

void ushort_send(connection_state * state, unsigned short data)
{
    Result ret;
    data = htons(data);
    ret = connection_write(state, (char*)&data, sizeof(data));
    if(FAILURE(ret)) state->has_ended=1;
}

unsigned short ushort_recv(connection_state * state)
{
    Result ret;
    unsigned short data;
    int i;
    ret = connection_flush(state);
    if(FAILURE(ret)) state->has_ended=1;
    i = recv(state->stream, (char*)&data, sizeof(data), 0);
    data = ntohs(data);
    if(i!=sizeof(data)) state->has_ended=1;
    return data;
}

void short_send(connection_state * state, short data)
{
    ushort_send(state, *((unsigned short *)(&data)));
}

short short_recv(connection_state * state)
{
    unsigned short usData;
    usData = ushort_recv(state);
    
    return *((short *)(&usData));
}

void uchar_send(connection_state * state, unsigned char data)
{
    Result ret;
    ret = connection_write(state, (char*)&data, sizeof(data));
    if(FAILURE(ret)) state->has_ended=1;
}

unsigned char uchar_recv(connection_state * state)
{
    Result ret;
    unsigned char data;
    int i;
    ret = connection_flush(state);
    if(FAILURE(ret)) state->has_ended=1;
    i = recv(state->stream, &data, sizeof(data), 0);
    if(i!=sizeof(data)) state->has_ended=1;
    return data;
}

void char_send(connection_state * state, char data)
{
    uchar_send(state, *((unsigned char *)(&data)));
}

char char_recv(connection_state * state)
{
    unsigned char ucData;
    ucData = uchar_recv(state);
    
    return *((char *)(&ucData));
}

Result cmd_send(connection_state * state, unsigned cmd)
{
#ifndef WIN32
    int i;
	int tmp;
    //test, if we have remaining data from other function
    i = recv(state->stream, &tmp, sizeof(tmp), MSG_DONTWAIT);
    if(i>0) return EAPIOUTOFSYNC;
#endif
    cmd = htonl(cmd);
    return connection_write(state, (char*)&cmd, sizeof(cmd));
}

Result result_recv(connection_state * state)
{
    Result ret;
    int i;
    ret = connection_flush(state);
    if(FAILURE(ret)) return ret;
    if(state->mode == NORMAL_MODE)
    {
        i = recv(state->stream, (char*)&ret, sizeof(ret), 0);
        if(i!=sizeof(ret)) return EAPIDISCONNECT;
        ret = ntohl(ret);
        return ret;
    }else{
        return OK;
    }
}

// uint32 (same as uint)
void uint32_send(connection_state * state, unsigned data) { uint_send(state, data); }
uint32_t uint32_recv(connection_state * state) { return uint_recv(state); }

// int32 (same as int)
void int32_send(connection_state * state, int32_t data) { int_send(state, data); }
int32_t int32_recv(connection_state * state) { return int_recv(state); }
// uint16 (same as ushort)
void uint16_send(connection_state * state, uint16_t data) { ushort_send(state, data); }
uint16_t uint16_recv(connection_state * state) { return ushort_recv(state); }

// int16 (same as short)
void int16_send(connection_state * state, int16_t data) { short_send(state, data); }
int16_t int16_recv(connection_state * state) { return short_recv(state); }

// uint8 (same as uchar)
void uint8_send(connection_state * state, uint8_t data) { uchar_send(state, data); }
uint8_t uint8_recv(connection_state * state) { return uchar_recv(state); }

// int8 (same as uhar)
void int8_send(connection_state * state, uint8_t data) { char_send(state, data); }
int8_t int8_recv(connection_state * state) { return char_recv(state); }

void float_send(connection_state * state, float fData)
{    
    union{
        uint32_t u32Data;
        float fData;
    } UnionData;
    
    UnionData.fData = fData;

    uint_send(state, UnionData.u32Data);
}


float float_recv(connection_state * state)
{
    union{
        uint32_t u32Data;
        float fData;
    } UnionData;

    UnionData.u32Data = uint_recv(state);
    
    return UnionData.fData;
}

#ifndef __CCE__

static Result flush_data(connection_state * state, int size)
{
    int transferred=0;
    int i, getsize, remaining;
    char databuf[BUFFERSIZE];
    while(transferred<size)
    {
        remaining = size-transferred;
        debug("remaining %i bytes\t", remaining);
        getsize = remaining > BUFFERSIZE ? BUFFERSIZE : remaining ;
        debug("flushing %i bytes\t", getsize);
        i = recv(state->stream, databuf, getsize, 0);
        debug("flushed %i bytes\n", i);
        if (i<=0) return -1;
        transferred+=i;
    }
    return OK;
}

//receive buffer of unknow size
Result buf_recv_alloc(connection_state * state, char ** buf, int * bufsize)
{
    Result ret;
    int i, size, remaining;
    char * tmp_buf;
    ret = connection_flush(state);
    if(FAILURE(ret)) return ret;
    i = recv(state->stream, (char*)&size, sizeof(size), 0);
    if(i!=sizeof(size)) return EAPIDISCONNECT;
    size = ntohl(size);
    tmp_buf = (char*)malloc(size);
    //debug("buf_recv size %i addr %p\n", size, tmp_buf);
    if(!tmp_buf)
    {
        flush_data(state, size);
        return ERR_OUTOFMEMORY;
    }
    remaining = size;
    while(remaining>0)
    {
        i = recv(state->stream, &tmp_buf[size-remaining], remaining, 0);
        if(i<=0)
        {
            free(tmp_buf);
            return EAPIDISCONNECT;
        }
        remaining-=i;
    }
    *buf = tmp_buf;
    *bufsize = size;
    return OK;
}

Result buf_free(char * buf)
{
    free(buf);
    return OK;
}

Result buf_recv(connection_state * state, char * buf, unsigned maxsize)
{
    Result ret;
    int i;
    unsigned size;
    unsigned transferred=0;
    int remaining;

    size = uint_recv(state);
    if(size>maxsize)
    {
        ret = flush_data(state, size);
        if(FAILURE(ret)) return ret;
        return EAPINOMEM;
    }

    while(transferred<size)
    {
        remaining = size-transferred;
        i = recv(state->stream, buf, remaining, 0);
        if(i<=0) return EAPIDISCONNECT;
        transferred += i;
    }
    return OK;
}

Result buf_recv_with_size(connection_state * state, char * buf, unsigned * size)
{
    int i;
    unsigned transferred=0;
    int remaining;

    *size = uint_recv(state);

    while(transferred<*size)
    {
        remaining = *size-transferred;
        i = recv(state->stream, buf, remaining, 0);
        if(i<=0) return EAPIDISCONNECT;
        transferred += i;
    }
    return OK;
}

Result buf_send(connection_state * state, const char * buf, unsigned size)
{
    //debug("buf_send: %p  %i\n", buf, size);
    uint_send(state, size);
    return connection_write(state, buf, size);
}

#endif

