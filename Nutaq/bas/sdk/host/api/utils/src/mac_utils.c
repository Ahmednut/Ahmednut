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
 * @file      mac_utils.c
 * 
 * @brief     Functions that retrieve the MAC of the host and the FPGA device
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under 
 * either the terms of the GNU General Public License (GPL) or the 
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General 
 * Public License version 3 as published by the Free Software Foundation 
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF 
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on 
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL. 
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire 
 * to license it under your own terms, e.g. a closed source license, you may purchase 
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite. 
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bas_types.h"
#include "media_types.h"
#include "eapi.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex_eth.h"
#include "linux_utils.h"

int iGetConnectionLocalMacAddr( connection_state * state, unsigned char pucLocalMacAddr[ 6])
{
   char acDescription[ 261];
   char acHostIpAddr[ 16];
   unsigned char aucHostMacAddr[ 6];
   unsigned u;

   struct sockaddr name;
   unsigned iNameLen;
   int res;
   unsigned long localIp;

   iNameLen = sizeof name;
   res = getsockname(  state->stream, &name, &iNameLen);
   if( name.sa_family != AF_INET)
      return -1;
   localIp = ((unsigned)(unsigned char)(name.sa_data[ 5]) << 24)
         | ((unsigned)(unsigned char)(name.sa_data[ 4]) << 16)
         | ((unsigned)(unsigned char)(name.sa_data[ 3]) << 8)
         | ((unsigned)(unsigned char)(name.sa_data[ 2]) << 0);

   for( u = 0; ; u++) {
	   res = RTDExGetMacEth( u, aucHostMacAddr, acHostIpAddr, acDescription);
	   if( res)
		   break;
//	   printf( "mac: %02X:%02X:%02X:%02X:%02X:%02X ", aucHostMacAddr[ 0], aucHostMacAddr[ 1], aucHostMacAddr[ 2], aucHostMacAddr[ 3], aucHostMacAddr[ 4], aucHostMacAddr[ 5]);
//	   printf( "ip = %s ", acHostIpAddr);
//	   printf( "desc= %s\n", acDescription);
	   if( localIp == inet_addr( acHostIpAddr))
          break;
   }
   if(res == 0)
      memcpy( pucLocalMacAddr, aucHostMacAddr, 6);
   return res;
}



int sscanMacAddress( char * s, unsigned char * pucMacAddr)
{
   int n, i;
   unsigned pTempMacAddr[ 6];

   n = sscanf_s( s, "%02x:%02x:%02x:%02x:%02x:%02x",
               &pTempMacAddr[0], &pTempMacAddr[1], &pTempMacAddr[2],
               &pTempMacAddr[3], &pTempMacAddr[4], &pTempMacAddr[5]);
   if( n != 6)
      return -1;
   for( i = 0; i < 6; i++)
      pucMacAddr[ i] = (unsigned char)pTempMacAddr[ i] & 0xff;
   return 0;
}

int iGetConnectionPeerMacAddr( connection_state * state, unsigned char pucPeerMacAddr[ 6])
{
   int res;
   char acMacAddrString[ 20];

   if( (res = get_mac_address( state, acMacAddrString)))
      return res;
   res = sscanMacAddress( acMacAddrString, pucPeerMacAddr);
   return res;
}

int GetMACAddress( connection_state * state, char* pHostMacAddr, char* pDeviceMacAddr)
{
    adp_result_t res;
    int i;
    const char* pMacStr = "%02x:%02x:%02x:%02x:%02x:%02x:%02x";

    // get MAC address of target
    res = iGetConnectionPeerMacAddr( state, (unsigned char *)pDeviceMacAddr);
    if( res ) 
    {
       printf( "Problem getting target MAC address.\n" );
       return res;
    }

    // Print mac address
    printf( "MAC Address of Perseus is: '");
    for( i = 0; i < 5; i++)
    printf( "%02X:", (unsigned char)pDeviceMacAddr[ i ]);
    printf( "%02X'\n", (unsigned char)pDeviceMacAddr[ i ]);
   
    // get MAC address of host
    res = iGetConnectionLocalMacAddr( state, (unsigned char *)pHostMacAddr);
    if( res ) 
    {
       printf( "Problem getting host MAC address.\n" );
       return res;
    }

    // Print mac address
    printf( "MAC Address of Host is: '");
    for( i = 0; i < 5; i++)
    printf( "%02X:", (unsigned char)pHostMacAddr[ i ]);
    printf( "%02X'\n\n", (unsigned char)pHostMacAddr[ i ]); 

    return 0;
}
