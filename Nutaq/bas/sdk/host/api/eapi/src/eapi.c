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
 * @file      eapi.c
 *
 * @brief     Function definitions for the eapi core functionalities
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
#include <stdlib.h>
#include <stdio.h>
#include "bas_error.h"
#include "cce.h"
#include "eapi.h"
#include "eapi_serialize.h"
#include "eapi_carrier.h"
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "eapi_carrier.h"

#if 0
#include <stdio.h>
#define debug printf
#else
#define debug
#endif

#ifdef _MSC_VER  //all MS compilers define this secure version
#define snprintf(str, size, format, ...) _snprintf_s(str, size, _TRUNCATE, format, __VA_ARGS__)
#define strncpy(dst, src, size)  strncpy_s(dst, size, src, _TRUNCATE)
#endif

#define IP_STRING_LENGTH ( 15 + 1 + 1 )

#define BUFFERSIZE 4096

char ipaddress[20];

static unsigned int convert_cce_version_to_uint(char * version);

void split_path_file(char *pf, char** p, char** f) {
    char *slash = pf, *next;
    while ((next = strpbrk(slash + 1, "\\/"))) slash = next;
    if (pf != slash) slash++;

    if(p)
    {
        *p = malloc(slash - pf + 1);
        memcpy(*p, pf, slash - pf);
        (*p)[slash - pf] = '\0';
    }
    if(f)
    {
        *f = malloc(strlen(slash)+1);
        memcpy(*f, slash, strlen(slash));
        (*f)[strlen(slash)] = '\0';
    }
}

static Result _eapi_init()
{
#ifdef WIN32
	static int inited;
	if (!inited)
	{
		WSADATA wsadata;
		if (WSAStartup(MAKEWORD(2,2), &wsadata) != 0)
		{
			fprintf(stderr, "Error WSAStartup  %i\n", WSAGetLastError());
			return EAPIWSA;
		}
		inited = 1;
	}
#endif
    srand((unsigned)time(NULL));
	return OK;
}

Result connect_cce(const char * servip, connection_state * stateout)
{
#if defined(__GNUC__) || defined(SWIG)
	int sock;
#else
    SOCKET sock;
#endif
    struct sockaddr_in ServAddr;
    int i, res;
	char cce_version[50];
	unsigned int ver;

    Carrier_eType_t type;

    if(_eapi_init()) return -1;

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock<0) return EAPISOCKET;
    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family      = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(servip);
    ServAddr.sin_port        = htons(CCEIPPORT);
	snprintf(ipaddress, 20, "%s" ,servip);

    memset(stateout, 0, sizeof(connection_state));
    i = connect(sock, (struct sockaddr *) &ServAddr, sizeof(ServAddr));
    if(i>=0)
    {
        stateout->stream = sock;
        stateout->has_ended = 0;
        stateout->send_buffer.length = 0;
        res = Carrier_Presence_send(stateout);
        if(SUCCESS(res))
        {
            res = get_cce_version(stateout, cce_version);
            if(res)
            {
                return res;
            }
            ver = convert_cce_version_to_uint(cce_version);
            if (ver <= MAX_VERSION && ver >= MIN_VERSION)
            {
                // OK
            }
            else if (ver >= MIN_VERSION_UPDATE_COMPATIBLE)
            {
                return EAPICCELIMITEDACCESS;
            }
            else
            {
                stateout->has_ended = 1;
                #ifdef __GNUC__
                    close(sock);
                #else
                    closesocket(sock);
                #endif
                return EAPICCEVERSIONMISMATCH;
            }

            res = Carrier_GetType_send(stateout, &type);
            if(FAILURE(res))
            {
                stateout->has_ended = 1;
                #ifdef __GNUC__
                    close(sock);
                #else
                    closesocket(sock);
                #endif
                return res;
            }

            if(type == Carrier_eUnknown)
            {
                return EAPICARRIERLIMITEDACCESS;
            }
            
            return OK;

        }
        return OK;
    }else{
        stateout->has_ended = 1;
#ifdef __GNUC__
		close(sock);
#else
		closesocket(sock);
#endif
        return EAPICONNECT;
    }
}

Result disconnect_cce(connection_state * state)
{
    connection_flush(state);
#ifdef __GNUC__
	close(state->stream);
#else
	closesocket(state->stream);
#endif
    state->stream = 0;
    state->has_ended = 1;
    return OK;
}

Result set_turbo_mode(connection_state * state)
{
    Result ret;
    cmd_send(state, CCECTRL_TURBO_MODE);
    ret = result_recv(state);
    if(SUCCESS(ret))
    {
        state->mode = TURBO_MODE;
    }
    return ret;
}

Result set_normal_mode(connection_state * state)
{
    Result ret;
    cmd_send(state, CCECTRL_NORMAL_MODE);
    state->mode = NORMAL_MODE;
    ret = result_recv(state);
    return ret;
}

Result get_mac_address(connection_state * state, char *macstring)
{
	Result ret;
    cmd_send(state, CCECTRL_GET_MAC);
    buf_recv(state, macstring, 20);
    ret = result_recv(state);
    return ret;
}

char * get_ip_address(void)
{
    return ipaddress;
}

Result fpgaflash_send(connection_state * state, char * buf, unsigned size, unsigned index, char * comment)
{
//    unsigned transferred=0;
	unsigned int i;
    cmd_send(state, FPGAFLASH_RECV);
    debug("bitstream_size = %i\n", size);
    uint_send(state, index);
    uint_send(state, size);
    connection_write(state, buf, size);
	debug("comment_length = %u\n", (unsigned int)strlen(comment));
	for (i = 0; i < (unsigned int)strlen(comment) ; i++)
	{
		debug("%c",comment[i]);
	}
	debug("\n",0);
    uint_send(state, (unsigned int)strlen(comment));
    connection_write(state, comment, (unsigned int)strlen(comment));
    return result_recv(state);
}

Result fpgaflash_send_with_ID(connection_state * state, char * buf, unsigned size, unsigned char id, unsigned char bank)
{
//    unsigned transferred=0;
    unsigned net_size;
	cmd_send(state, FPGAFLASH_RECVWID);
    uchar_send(state, id);
    uchar_send(state, bank);
    debug("bitstream_size = %i\n", size);
    net_size = htonl(size);
    connection_write(state, (char*)&net_size, sizeof(net_size));
    connection_write(state, buf, size);
    return result_recv(state);
}

#ifdef __linux__
#include <sys/mman.h>
Result fpgaflash_fromfile_with_comment_send(connection_state * state, char * name, unsigned index, char * comment)
{
    Result ret;
    int size, file;
    struct stat sb;
    char * bitstream;

    file = open(name, O_RDONLY);
    if(!file) return EAPIFILE;
    if (fstat (file, &sb) == -1) return EAPIFILE;
    size = sb.st_size;
    bitstream = mmap(0, size, PROT_READ, MAP_PRIVATE, file, 0);
    if (bitstream == MAP_FAILED) return EAPIMMAP;
    close(file);
    madvise(bitstream, size, MADV_SEQUENTIAL);

    ret = fpgaflash_send(state, bitstream, size, index, comment);

    munmap (bitstream, size);
    return ret;
}
#else
Result fpgaflash_fromfile_with_comment_send(connection_state * state, char * name, unsigned index, char * comment)
{
    Result ret;
    int size;
	size_t i;
    FILE * file;
    char * bitstream = NULL;

    file = fopen(name, "rb");
    if(!file)
	{
		return EAPIFILE;
	}
    fseek(file,0,SEEK_END);
    size = ftell(file);
    if(size == 0)
	{
		return OK;
	}
    fseek(file, 0, SEEK_SET);
    bitstream = malloc(size);
    if(!bitstream)
	{
		return EAPINOMEM;
	}
    i = fread(bitstream, size, 1, file);
    fclose(file);
    if(i != 1)
    {
        ret = EAPIFILE;
        goto file_failed;
    }

    ret = fpgaflash_send(state, bitstream, size, index, comment);

file_failed:
    if (bitstream) free(bitstream);
    return ret;
}
#endif

Result fpgaflash_fromfile_send(connection_state * state, char * name, unsigned index)
{
    Result ret;
    char * filename = NULL;

    split_path_file(name, NULL, &filename);

    ret = fpgaflash_fromfile_with_comment_send(state, name, index, filename);

    if (filename) free(filename);
    return ret;
}

Result updateflash_send(connection_state * state, char * buf, unsigned size, unsigned partition)
{
	cmd_send(state, FPGAFLASH_UPDATE_RECV);
	debug("FPGAFLASH_UPDATE_RECV\n",0);
    debug("file_size = %i\n", size);
    uint_send(state, partition);
    uint_send(state, size);
    connection_write(state, buf, size);
    return result_recv(state);
}

Result updateflash_fromfile_send(connection_state * state, char * name, unsigned partition)
{
    Result ret;
    char * filename = NULL;
    int size;
	size_t i;
    FILE * file;

    file = fopen(name, "rb");
    if(!file)
	{
		return EAPIFILE;
	}
    fseek(file,0,SEEK_END);
    size = ftell(file);
    if(size == 0)
	{
		return OK;
	}
    fseek(file, 0, SEEK_SET);
    filename = malloc(size);
    if(!filename)
	{
		return EAPINOMEM;
	}
    i = fread(filename, size, 1, file);
    if(i != 1)
    {
        ret = EAPIFILE;
        goto file_failed;
    }
	fclose(file);

    ret = updateflash_send(state, filename, size, partition);

file_failed:
	if (filename) free(filename);
    return ret;
}

Result dtbflash_fromfile_send(connection_state * state, char * name, unsigned code, char * mac_address)
{
	Result ret;
    char * filename = NULL;
    int size;
	size_t i;
    FILE * file;

    file = fopen(name, "rb");
    if(!file)
	{
		return EAPIFILE;
	}
    fseek(file,0,SEEK_END);
    size = ftell(file);
    if(size == 0)
	{
		return OK;
	}
    fseek(file, 0, SEEK_SET);
    filename = malloc(size);
    if(!filename)
	{
		return EAPINOMEM;
	}
    i = fread(filename, size, 1, file);
    if(i != 1)
    {
        ret = EAPIFILE;
        goto file_failed;
    }
	fclose(file);

    ret = dtbflash_send(state, filename, size, code, mac_address);

file_failed:
	if (filename) free(filename);
    return ret;
}

Result dtbflash_send(connection_state * state, char * buf, unsigned size, unsigned code, char * mac_address)
{
	unsigned int i;
	cmd_send(state, FPGAFLASH_DTB_RECV);
	debug("FPGAFLASH_DTB_RECV\n",0);
    debug("file_size = %i\n", size);
	debug("mac_address_size = %u\n", (unsigned int)strlen(mac_address));
	for (i = 0; i < (unsigned int)strlen(mac_address) ; i++)
	{
		debug("%c",mac_address[i]);
	}
	debug("\n",0);
    uint_send(state, code);
    uint_send(state, size);
    connection_write(state, buf, size);
	uint_send(state, (unsigned int)strlen(mac_address));
	connection_write(state, mac_address, (unsigned int)strlen(mac_address));

    return result_recv(state);
}

#ifdef __linux__
#include <sys/mman.h>
Result fpgaflash_fromfile_send_with_ID(connection_state * state, char * name, unsigned char id, unsigned char bank)
{
    Result ret;
    int size, file;
    struct stat sb;
    char * bitstream;

    file = open(name, O_RDONLY);
    if(!file) return EAPIFILE;
    if (fstat (file, &sb) == -1) return EAPIFILE;
    size = sb.st_size;
    bitstream = mmap(0, size, PROT_READ, MAP_PRIVATE, file, 0);
    if (bitstream == MAP_FAILED) return EAPIMMAP;
    close(file);
    madvise(bitstream, size, MADV_SEQUENTIAL);
//MSA    ret = fpgaflash_send_multiple(state, bitstream, size, id);
    ret = fpgaflash_send_with_ID(state, bitstream, size, id, bank); // MSA01oct2012
    munmap (bitstream, size);
    return ret;
}
#else
Result fpgaflash_fromfile_send_with_ID(connection_state * state, char * name, unsigned char id, unsigned char bank)
{
    Result ret;
    int size;
	size_t i;
    FILE * file;
    char * bitstream;
    file = fopen(name, "rb");
    if(!file) return EAPIFILE;
    fseek(file,0,SEEK_END);
    size = ftell(file);
    if(size == 0) return OK;
    fseek(file, 0, SEEK_SET);
    bitstream = malloc(size);
    if(!bitstream) return EAPINOMEM;
    i = fread(bitstream, size, 1, file);
    fclose(file);
    if(i != 1)
    {
        ret = EAPIFILE;
        goto file_failed;
    }
    ret = fpgaflash_send_with_ID(state, bitstream, size, id, bank);
file_failed:
    free(bitstream);
    return ret;
}
#endif

Result fpgaflash_get_info_send(connection_state * state, unsigned int index,
    char ** name, unsigned int * name_length,
    char ** type, unsigned int * type_length,
    char ** date, unsigned int * date_length,
    char ** hour, unsigned int * hour_length,
    char ** custom, unsigned int * custom_length)
{
    Result ret;
    char buf[256] = {'\0'};

    cmd_send(state, FPGAFLASH_GET_INFO);

    uint_send(state, index);

    if(uint_recv(state) == 1)
    {
        buf_recv_with_size(state, buf, name_length);
        *name = malloc(*name_length);
        memcpy(*name, buf, *name_length);

        buf_recv_with_size(state, buf, type_length);
        *type = malloc(*type_length);
        memcpy(*type, buf, *type_length);

        buf_recv_with_size(state, buf, date_length);
        *date = malloc(*date_length);
        memcpy(*date, buf, *date_length);

        buf_recv_with_size(state, buf, hour_length);
        *hour = malloc(*hour_length);
        memcpy(*hour, buf, *hour_length);

        buf_recv_with_size(state, buf, custom_length);
        *custom = malloc(*custom_length);
        memcpy(*custom, buf, *custom_length);
    }
    else
    {
        *name = NULL;
        *type = NULL;
        *date = NULL;
        *hour = NULL;
        *custom = NULL;
        *name_length = 0;
        *type_length = 0;
        *date_length = 0;
        *hour_length = 0;
        *custom_length = 0;
    }

    ret = result_recv(state);
    return ret;
}

Result fpgaflash_get_index_send(connection_state * state, unsigned int * index)
{
    Result ret;

    cmd_send(state, FPGAFLASH_GET_INDEX);

    *index = uint_recv(state);

    ret = result_recv(state);
    return ret;
}

Result fpgaflash_set_index_send(connection_state * state, unsigned int index)
{
    Result ret;

    cmd_send(state, FPGAFLASH_SET_INDEX);

    uint_send(state, index);

    ret = result_recv(state);
    return ret;
}

Result memory_read_send(connection_state * state, unsigned addr, unsigned * data)
{
    Result ret;
    unsigned tmp_data;

	debug("memory_read_send: addr = %08X", addr);
	cmd_send(state, MEMORY_READ);

	uint_send(state, addr);

    ret = connection_flush(state);
    if(FAILURE(ret)) return ret;

	tmp_data = uint_recv(state);
    if(FAILURE(ret)) return ret;

    ret = result_recv(state);
    if(SUCCESS(ret)) *data = tmp_data;

	debug(", data = %08X\n", tmp_data);
	return ret;
}

Result custom_register_read_send(connection_state * state, unsigned id, unsigned * data)
{
    Result ret;
    unsigned tmp_data;

	debug("custom_register_read_send : id = %u", id);
	cmd_send(state, MEMORY_READ_CUSTOM_REG_ID);

	uint_send(state, id);

    ret = connection_flush(state);
    if(FAILURE(ret)) return ret;

	tmp_data = uint_recv(state);
    if(FAILURE(ret)) return ret;

    ret = result_recv(state);
    if(SUCCESS(ret)) *data = tmp_data;

	debug(", data = %08X\n", tmp_data);
	return ret;
}

Result memory_write_send(connection_state * state, unsigned addr, unsigned data)
{
	debug("memory_write_send: addr = %08X, data = %08X\n", addr, data);
	cmd_send(state, MEMORY_WRITE);
	uint_send(state, addr);
	uint_send(state, data);
    return result_recv(state);
}

Result memory_write_with_mask_send(connection_state * state, unsigned addr, unsigned data, unsigned mask)
{
	debug("memory_write_send: addr = %08X, data = %08X\n", addr, data);
	cmd_send(state, MEMORY_WRITE_WITH_MASK);
	uint_send(state, addr);
	uint_send(state, data);
	uint_send(state, mask);
    return result_recv(state);
}

Result custom_register_write_send(connection_state * state, unsigned id, unsigned data)
{
	debug("custom_register_write_send: id = %u, data = %08X\n", id, data);
	cmd_send(state, MEMORY_WRITE_CUSTOM_REG_ID);
	uint_send(state, id);
	uint_send(state, data);
    return result_recv(state);
}

Result echo_send(connection_state * state)
{
    unsigned cmd = ECHO_ECHO;
    cmd_send(state, cmd);
    return result_recv(state);
}

Result ctrl_exit(connection_state * state)
{
    unsigned cmd = CCECTRL_EXIT;
    cmd_send(state, cmd);
    return result_recv(state);
}

 /****************************************************************************
 * NAME : i2c_scan_devices_send
 ************************************************************************//**
 *
 * Scan the specified i2c bus to detect available devices.
 * This function is used for production purpose to check if all devices
 * are detected correctly on the i2c bus.
 *
 * @warning
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Bus to scan (1 for default).
 *
 * @param [out] scanraw
 *    Returns the raw i2c scan results structure (each detected device address in the vector
 *    hold != 0 when a device is detected)
 *
 * @param [out] ndevices
 *    Number of i2c devices detected on the bus.
 *
 * @return   Error code: 0 if no problem is found, != 0 if problem.
 *
 * @pre
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used).
 *    BusAccess_GetConnectionState().
 *
 * Example:
 * @code
 * @endcode
 *
 * @see
 *
 ***************************************************************************/
Result i2c_scan_devices_send(connection_state * state, unsigned nb, I2C_stI2cScan *scanraw,
                             unsigned char *ndevices)
{
    Result ret, ret2;

	cmd_send(state, MEMORY_SCAN_I2C_DEVICES);
	uint_send(state, nb-1);
	*ndevices = uchar_recv(state);
    ret2 = buf_recv(state, (char*)scanraw, sizeof(I2C_stI2cScan));
	ret = result_recv(state);
    if (SUCCESS(ret))
    {
        if (FAILURE(ret2))
            ret = ret2;
    }
    return ret;
}


Result get_cce_version(connection_state * state, char * version)
{
	unsigned cmd = CCECTRL_VERSION;
	cmd_send(state, cmd);
	buf_recv(state, version, 50);
	return result_recv(state);
}

int get_eapi_version()
{
    return (int)EAPI_VERSION;
}

static unsigned int convert_cce_version_to_uint(char * version)
{
	unsigned int i, counter;
	char m_ver[10] = "";
	char temp[2];

	counter = 0;
	for (i=0;i<strlen(version);i++)
	{
		if (version[i] == ' ')
		{
			break;
		}
		else if (version[i] != '.')
		{
			if(counter)
			{
				if((version[i+1] == '.' || version[i+1] == ' ') && version[i-1] == '.')
				{
					temp[0] = '0';
					temp[1] = '\0';
					strcat(m_ver,temp);
				}
			}
			temp[0] = version[i];
			temp[1] = '\0';
			strcat(m_ver,temp);
		}
		else
		{
			counter++;
		}
	}
	return atoi(m_ver);
}

