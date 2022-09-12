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
 * @file      rtdex_tcpip.c
 * 
 * @brief     Functions definition for RTDEx access via protocole TCP/IP.
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
#include <stdio.h>
#include <stdlib.h>

#include "eapi.h"
#include "bas_types.h"
#include "media_types.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_tcpip.h"
#include "rtdex_access.h"
#include "bas_error.h"

#ifndef __linux__

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Winsock2.h>
#include <Ws2tcpip.h>

#else

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <strings.h>

#endif

#define RTDEX_TCP_BUFFER_SIZE 131072
#define RTDEX_DATA_PORT 19850

adp_result_t RTDExOpenTcpIp(  adp_handle_rtdex_conn *hConn,
							int8_t *server_ip) // The IP address of the target board in "***.***.***.***" format.
{
	//server structs
#ifndef __linux__
	SOCKET sock;
#else
	int sock;
#endif
	adp_handle_rtdex_tcpip_conn hTcpIpRTDEx;
	adp_handle_rtdex_conn hTempConn = NULL;
	unsigned buff_size,i;
	
	hTempConn = (adp_handle_rtdex_conn)(malloc(sizeof(sMediaRTDExStruct)));
	hTcpIpRTDEx = (adp_handle_rtdex_tcpip_conn)(malloc(sizeof(sTcpIpRTDExStruct)));

	if(hTempConn == NULL) 
	{
		return (RTDEXMEMALLOC); // Allocation error
	}
	hTempConn->eMedia = eMediaTcpIp;
	hTempConn->hMediaRTDexConn = (adp_handle_rtdex_media_conn)hTcpIpRTDEx;
#ifndef __linux__
	//winsock init
	{
		WSADATA wsadata;
		if (WSAStartup(MAKEWORD(2,2), &wsadata) != 0)
		{
			return RTDEXTCPIPWSASTART;
		}
	}
#endif

#ifndef __linux__
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	sock = socket(AF_INET, SOCK_STREAM, 0);
#endif
	if(sock<0) return RTDEXTCPIPSOCKET;
	
	buff_size = RTDEX_TCP_BUFFER_SIZE;
	i = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (const char*)&buff_size, sizeof(buff_size));
	if(i<0)
	{
		return RTDEXTCPIPSETSOCK;
	}
	
	buff_size = RTDEX_TCP_BUFFER_SIZE;
	i = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&buff_size, sizeof(buff_size));
	if(i<0)
	{
		return RTDEXTCPIPSETSOCK;
	}
	
	hTcpIpRTDEx->m_sock = sock;
	hTcpIpRTDEx->m_server_ip = server_ip;
	*hConn = hTempConn;
	
	return RTDEXOK;
}

adp_result_t RTDExCloseTcpIp( adp_handle_rtdex_conn hConnRtDex )
{
	adp_handle_rtdex_tcpip_conn pTcpIpConn = (adp_handle_rtdex_tcpip_conn)(hConnRtDex->hMediaRTDexConn);

#ifndef __linux__
	closesocket(pTcpIpConn->m_sock);
	WSACleanup();
#else
	close(pTcpIpConn->m_sock);
#endif
	free( hConnRtDex->hMediaRTDexConn);
	free( hConnRtDex);
	return RTDEXOK;
}

adp_result_t RTDExStartTcpIp( adp_handle_rtdex_conn hConnRtDex, uint32_t u32FrameSize, uint32_t u32TransferSize)
{
	struct sockaddr_in serv_addr;
#ifdef __linux__
	struct hostent *server;
#endif
	int i;
	adp_handle_rtdex_tcpip_conn pTcpIpConn = (adp_handle_rtdex_tcpip_conn)(hConnRtDex->hMediaRTDexConn);

#ifndef __linux__
	memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr((const char*)(pTcpIpConn->m_server_ip));
    serv_addr.sin_port        = htons( RTDEX_DATA_PORT );
#else
    server = gethostbyname((const char*)(pTcpIpConn->m_server_ip));
    if (server == NULL)
    {
        return RTDEXTCPIPSERVER;
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(RTDEX_DATA_PORT);
#endif
	
	i = connect(pTcpIpConn->m_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

	if (i<0)
	{
		return RTDEXTCPIPCONNECT;
	}
	return RTDEXOK;
}

int RTDExSendTcpIp( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize)
{
	int j;
	adp_handle_rtdex_tcpip_conn pTcpIpConn = (adp_handle_rtdex_tcpip_conn)(hConnRtDex->hMediaRTDexConn);

#ifndef __linux__
	DWORD SendBytes;
	WSABUF DataBuf;
	
	DataBuf.len = u32MaxSize;
	DataBuf.buf = pDataBuffer;
#else
	unsigned int SendBytes;
#endif

#ifndef __linux__
	j = WSASend(pTcpIpConn->m_sock, &DataBuf, 1, &SendBytes, 0, 0, 0);
	if(j!=0)
	{
		return RTDEXTCPIPSEND;
	}
#else
	SendBytes = write(pTcpIpConn->m_sock, (unsigned char*) pDataBuffer, u32MaxSize);
	if(SendBytes <= 0)
	{
		return RTDEXTCPIPSEND;
	}
#endif
	return (int)SendBytes;
	
}

int RTDExReceiveTcpIp( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, 
					  eRTDExTransferMode eMode, unsigned int uTimeOut )
{
	int j;
	adp_handle_rtdex_tcpip_conn pTcpIpConn = (adp_handle_rtdex_tcpip_conn)(hConnRtDex->hMediaRTDexConn);

#ifndef __linux__
	DWORD RecvBytes;
	DWORD flags=0;
	WSABUF DataBuf;
	
	DataBuf.len = u32MaxSize;
	DataBuf.buf = pDataBuffer;
#else
	unsigned int RecvBytes;
#endif
	
#ifndef __linux__
	j = WSARecv(pTcpIpConn->m_sock, &DataBuf, 1, &RecvBytes, &flags, 0, 0);
	if(j!=0)
	{
		return RTDEXTCPIPRECEIVE;
	}
#else
    j = read(pTcpIpConn->m_sock, (unsigned char*)pDataBuffer, u32MaxSize);
    if(j <= 0)
    {
    	return RTDEXTCPIPRECEIVE;
    }
    RecvBytes = j;
#endif

	return (int)RecvBytes;
}