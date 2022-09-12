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
 * @file      rtdex_eth.c
 * 
 * @brief     Functions definition for RTDEx access via ethernet.
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
#include "eapi.h"
#include "pcap.h"
#include "bas_types.h"
#include "media_types.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_eth.h"
#include "rtdex_access.h"
#include "bas_error.h"

#include "bas_time.h"

#define MAX_NUM_OF_RTDEX 8


#if defined(_WIN32) || defined(_WIN64)
#include "Iphlpapi.h"
#include "io.h"
#endif


#ifdef __linux__
//#include <sys/sem.h>
//#include <sys/shm.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#ifndef AF_LINK
  #define AF_LINK 18
#endif

#include <net/ethernet.h>
#include <netpacket/packet.h>

#endif /* linux */

#include "stdio.h"
#include "memory.h"
#include "malloc.h"
#include "pcap.h"




#define READ_TIMEOUT_MS 50 // pcap read timeout, ms


#define g_uEthernetHeaderSize 14
#define g_uSnapHeaderSize 8
#define g_uRTDExHeaderSize 10
#define g_uHeaderSize (g_uEthernetHeaderSize + g_uSnapHeaderSize + g_uRTDExHeaderSize)


#if defined(_WIN32) || defined(_WIN64)
#define INLINE _inline
#endif
#ifdef __linux__
#define INLINE __inline__
#define sscanf_s sscanf
#endif



// convert an unsigned short (16-bit) to big endian order, store into a byte array
static void INLINE vSetU16BigEndian( unsigned char * d, unsigned uVal)
{
    *(d + 0) = (uVal >> 8) & 0xff;
    *(d + 1) = (uVal >> 0) & 0xff;
}


// convert an unsigned to big endian order, store into a byte array
static void INLINE vSetU32BigEndian( unsigned char * d, unsigned uVal)
{
    *(d + 0) = (uVal >> 24) & 0xff;
    *(d + 1) = (uVal >> 16) & 0xff;
    *(d + 2) = (uVal >> 8) & 0xff;
    *(d + 3) = (uVal >> 0) & 0xff;
}


// Get a 4 bytes number (big endian) from a byte array
static unsigned int INLINE u32GetU32BigEndian( unsigned const char * s)
{
    unsigned uVal;

    uVal =  (unsigned)*(s + 0) << 24;
    uVal += (unsigned)*(s + 1) << 16;
    uVal += (unsigned)*(s + 2) << 8;
    uVal += (unsigned)*(s + 3) << 0;
    return uVal;
}



// return relative time in ms
//static unsigned int _inline uGetTimeMs()
//{
//   return GetTickCount();
//}




static adp_result_t ApplyFilter( adp_handle_rtdex_ethernet_conn hEthConn,
                                 unsigned int uNumChannel,
                                 unsigned char bIsCtrl )
{

    struct bpf_program sFilter;
    pcap_t* pInterface;
//    pcap_if_t* pDev;
    eProtocoleId eProtId;

    static char* sz_filter_with_addr =
                             "( ether src %02x:%02x:%02x:%02x:%02x:%02x ) and "
                             "( ether dst %02x:%02x:%02x:%02x:%02x:%02x ) and "
//#ifdef USE_JUMBO_FRAME
//                             "((ether[12:2] & 0xffff = 0x%04x) or (ether[12:2] & 0xffff = 0x%04x)) and "
//#else
//                             "( ether[12:2] & 0xffff = 0x%04x ) and "
//#endif
                             "( ether[14:2] & 0xffff = 0x%04x ) and "
                             "( ether[16:4] & 0xffffffff = 0x%08x ) and "
                             "( ether[20:2] & 0xffff = 0x%04x ) and "
                             "( ether[22:1] & 0xff = %u ) and "
                             "( ether[23:1] & 0xff = %u )";

    static char* sz_filter_without_addr =
//#ifdef USE_JUMBO_FRAME
//                             "((ether[12:2] & 0xffff = 0x%04x) or (ether[12:2] & 0xffff = 0x%04x)) and "
//#else
//                             "( ether[12:2] & 0xffff = 0x%04x ) and "
//#endif
                             "( ether[14:2] & 0xffff = 0x%04x ) and "
                             "( ether[16:4] & 0xffffffff = 0x%08x ) and "
                             "( ether[20:2] & 0xffff = 0x%04x ) and "
                             "( ether[22:1] & 0xff = %u ) and "
                             "( ether[23:1] & 0xff = %u )";


    static char packetFilter[300];


//    pDev = (pcap_if_t*) hEthConn -> pSourceInterface;

    eProtId = ( bIsCtrl ) ? eProtocolCtrl : hEthConn -> eProtId;

    // Set the filter for the dma

    if( hEthConn -> bGetBroadCast )
    {
        sprintf( packetFilter, sz_filter_without_addr,
//                               g_uNormalFrames,
//#ifdef USE_JUMBO_FRAME
//                               g_uJumboFrames,
//#endif
                               g_uSnapHeaders, g_uLyrtechHeaders,
                               g_uSubId, eProtId, uNumChannel );
    }

    else
    {
       sprintf( packetFilter, sz_filter_with_addr,
                              hEthConn->pSrcMacAddr[0], hEthConn->pSrcMacAddr[1],
                              hEthConn->pSrcMacAddr[2], hEthConn->pSrcMacAddr[3],
                              hEthConn->pSrcMacAddr[4], hEthConn->pSrcMacAddr[5],

                              hEthConn->pDstMacAddr[0], hEthConn->pDstMacAddr[1],
                              hEthConn->pDstMacAddr[2], hEthConn->pDstMacAddr[3],
                              hEthConn->pDstMacAddr[4], hEthConn->pDstMacAddr[5],
//                              g_uNormalFrames,
//#ifdef USE_JUMBO_FRAME
//                              g_uJumboFrames,
//#endif
                              g_uSnapHeaders, g_uLyrtechHeaders,
                              g_uSubId, eProtId, uNumChannel );
    }

    pInterface = (pcap_t*) hEthConn -> pInterfaceDmaHandle;

    if( pcap_compile( pInterface, &sFilter, packetFilter, 1, 0xfffffff ) < 0 )
    {
        return RTDEXPCAPCOMPILE;
    }

#if defined(_WIN32) || defined(_WIN64)
    pcap_setbuff( pInterface, 1 << 25 );
#endif

    if( pcap_setfilter( pInterface, &sFilter ) < 0 )
    {
        return RTDEXPCAPSETFILTER;
    }

    return 0;
}





#if defined(_WIN32) || defined(_WIN64)
static adp_result_t adp_GetInterfacePtrFromAddr( unsigned char pMacAddr[6],
                                          void** pSourceInterface,
                                          void** pSourceInterfaceList,
                                          void** pInterfaceDmaHandle,
                                          int pcapReadTo )

{

   ULONG dwSize;
   IP_ADAPTER_INFO* pIfTable = 0;
   IP_ADAPTER_INFO* pIfIterator = 0;
   int retVal;
   //int i, j;

   static char pErrorBuffer[PCAP_ERRBUF_SIZE];
   pcap_if_t* pAllDev;
   pcap_if_t* pDevIterator;

   *pSourceInterface = 0;
   *pInterfaceDmaHandle = 0;
   *pSourceInterfaceList = 0;

   dwSize = 0;

   retVal = GetAdaptersInfo( pIfTable, &dwSize );

   pIfTable = (IP_ADAPTER_INFO*) malloc( dwSize );

   if( pIfTable == 0 )
   {
       return RTDEXMEMALLOC;
   }

   retVal = GetAdaptersInfo( pIfTable, &dwSize );

   for( pIfIterator = pIfTable;
        pIfIterator != 0;
        pIfIterator = pIfIterator->Next )
   {
       if( pIfIterator -> AddressLength != 6 )
       {
           continue;
       }

       if( memcmp( pIfIterator->Address, pMacAddr, 6 ) == 0 )
       {
           break;
       }
   }

   if( pIfIterator == 0 )
   {

       free( pIfTable );
       return RTDEXINTERFACEITERATOR;
   }

   if( pcap_findalldevs( &pAllDev, pErrorBuffer ) == -1 )
   {
       *pSourceInterfaceList = pAllDev;
       free( pIfTable );
       return RTDEXPCAPFINDALLDEVS;
   }

   for( pDevIterator = pAllDev;
        pDevIterator != 0;
        pDevIterator = pDevIterator -> next )
   {
       if( strstr( pDevIterator -> name, pIfIterator -> AdapterName ) )
       {
           break;
       }
   }

   if( pDevIterator == 0 )
   {
       return RTDEXDEVICEITERATOR;
   }

   *pInterfaceDmaHandle = pcap_open_live( pDevIterator -> name,
                                          65535, 0, pcapReadTo,
                                          pErrorBuffer );

   if( *pInterfaceDmaHandle == 0 )
   {
       return RTDEXINVINTERFACEDMAHANDLE;
   }

#if defined(_WIN32) || defined(_WIN64)
   pcap_setbuff( (pcap_t *)(*pInterfaceDmaHandle), 1 << 25 );
#endif

   *pSourceInterface = pDevIterator;
   *pSourceInterfaceList = pAllDev;

   return 0;
}
#endif



#ifdef __linux__
static adp_result_t adp_GetInterfacePtrFromAddr( unsigned char pMacAddr[6],
                                          void** pSourceInterface,
                                          void** pSourceInterfaceList,
                                          void** pInterfaceDmaHandle,
                                          int pcapReadTo )
{
   static char pErrorBuffer[PCAP_ERRBUF_SIZE];
   pcap_if_t* pAllDev;
   pcap_if_t* pDevIterator;
   pcap_addr_t* pAddrIterator;

   *pSourceInterface = 0;
   *pInterfaceDmaHandle = 0;
   *pSourceInterfaceList = 0;
   unsigned char bFound = 0;

   if( pcap_findalldevs( &pAllDev, pErrorBuffer ) == -1 )
   {
       return -1;
   }

   for( pDevIterator = pAllDev;
        pDevIterator != 0;
        pDevIterator = pDevIterator -> next )
   {
        for ( pAddrIterator = pDevIterator-> addresses;
              pAddrIterator != 0;
              pAddrIterator = pAddrIterator -> next )
        {

            //check if this address is a mac address
            if(((struct sockaddr*)pAddrIterator->addr)->sa_family == PF_PACKET)
            {
                if(!memcmp(((struct sockaddr_ll*)pAddrIterator->addr)->sll_addr, pMacAddr, 6))
                {
                    bFound = 1;
                    break;
                }
            }
        }

        if( bFound == 1 )
        {
            break;
        }
   }


   if( pDevIterator == 0 )
   {
       return -1;
   }

   *pInterfaceDmaHandle = pcap_open_live( pDevIterator -> name,
                                          1 << 15, 0, pcapReadTo,
                                          pErrorBuffer );

   if( *pInterfaceDmaHandle == 0 )
   {
       return -1;
   }

   *pSourceInterface = pDevIterator;
   *pSourceInterfaceList = pAllDev;

   return 0;
}
#endif // linux



static adp_result_t SetTxModeContinuous (adp_handle_rtdex_conn hConnRtDex, int continuous )
{
//    uint32_t u32RegValue;
   uint64_t u64RegAddr;
   adp_result_t res = 0;

//    adp_handle_rtdex_ethernet_conn pEthConn = ((adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn));
   u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + RTDEX_ACCESS_RX_TX_MODE;
//  u32RegValue = ( 1 << pEthConn->uChannelId );
   res = memory_write_send( hConnRtDex->SrcMediaState, (unsigned int)u64RegAddr, continuous ? RTDEX_RXTXMODE_TX_CONTINUOUS_BM : 0);
   return res;
}


static adp_result_t RxStartTransfer( adp_handle_rtdex_conn hConnRtDex )
{
    adp_result_t res;
    uint32_t u32Bit;
    uint64_t u64RegAddr;

    adp_handle_rtdex_ethernet_conn pEthConn = ((adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn));
    
    // Calculate register address
    u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + ( RTDEX_ACCESS_RXSTARTNEWTRANSFER);

    // Select which bit to set to 1
    u32Bit = (1 << pEthConn->uDstChannelId);

    // Set the selected bit to 1
    res = memory_write_with_mask_send( hConnRtDex->DstMediaState, (unsigned int)u64RegAddr, 0xFFFFFFFF, u32Bit);
    return res;
}


static adp_result_t RxResetTransfer( adp_handle_rtdex_conn hConnRtDex )
{
    uint32_t u32Bit;
    uint64_t u64RegAddr;
    adp_result_t res;

    adp_handle_rtdex_ethernet_conn pEthConn = ((adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn));
    
    // Calculate register address
    u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + ( RTDEX_ACCESS_RXSTARTNEWTRANSFER );

    // Select which bit to set to 0
    u32Bit = (1 << pEthConn->uDstChannelId);

    // Set the selected bit to 0
    res = memory_write_with_mask_send( hConnRtDex->DstMediaState, (uint32_t)u64RegAddr, 0x00000000, u32Bit);
    return res;
}


static adp_result_t SetTxFrameSize( adp_handle_rtdex_conn hConnRtDex, uint32_t u32Size)
{
   uint64_t u64RegAddr;
   adp_result_t res;

   adp_handle_rtdex_ethernet_conn pEthConn = ((adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn));
   u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + (RTDEX_ACCESS_TXFRAMESIZE_ARRAY + (pEthConn->uSrcChannelId * 4));

   res = memory_write_send( hConnRtDex->SrcMediaState, (unsigned int)u64RegAddr, u32Size );
   return res;
}

static adp_result_t SetTxTransferSize( adp_handle_rtdex_conn hConnRtDex,
                                        uint32_t u32Size )
{
   uint64_t u64RegAddr;
   adp_result_t res;

   adp_handle_rtdex_ethernet_conn pEthConn = ((adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn));

   u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + ( RTDEX_ACCESS_TXTRANSFERSIZE_ARRAY + ( pEthConn->uSrcChannelId * 4));

   res = memory_write_send( hConnRtDex->SrcMediaState, (unsigned int)u64RegAddr, u32Size );
   return res;
}



static adp_result_t TxStartTransfer( adp_handle_rtdex_conn hConnRtDex )
{
    uint32_t u32Bit;
    uint64_t u64RegAddr;
    adp_result_t res;

    adp_handle_rtdex_ethernet_conn pEthConn = ((adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn));

    // Calculate register address
    u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + ( RTDEX_ACCESS_TXSTARTNEWTRANSFER );

    // Select which bit to set to 1
    u32Bit = (1 << pEthConn->uSrcChannelId);

    // Set the selected bit to 1
    res = memory_write_with_mask_send( hConnRtDex->SrcMediaState, (uint32_t)u64RegAddr, 0xFFFFFFFF, u32Bit);
    return res;
}


static adp_result_t TxResetTransfer( adp_handle_rtdex_conn hConnRtDex )
{
    uint32_t u32Bit;
    uint64_t u64RegAddr;
    adp_result_t res;

    adp_handle_rtdex_ethernet_conn pEthConn = ((adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn));

    // Calculate register address
    u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + ( RTDEX_ACCESS_TXSTARTNEWTRANSFER );

    // Select which bit to set to 0
    u32Bit = (1 << pEthConn->uSrcChannelId);

    // Set the selected bit to 0
    res = memory_write_with_mask_send( hConnRtDex->SrcMediaState, (uint32_t)u64RegAddr, 0x00000000, u32Bit);
    return res;
}



// Check if first MAC addr is equal to second MAC addr
static unsigned bIsEquateMacAddr( unsigned char puMac1[ 6], unsigned char puMac2[ 6])
{
   return memcmp( puMac1, puMac2, 6) ? 0 : 1;
}


// Get actual MAC address set on Perseus
static adp_result_t GetRtdexMacAddrs( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, unsigned char pPeerAddr[6], unsigned char pFpgaAddr[6])
{
   int res;
   uint32_t uHigh, uLow;

   if( (res = memory_read_send( state, (unsigned int)u64RTDExFpgaBaseAddr + RTDEX_ACCESS_PEERMACADDRHI, &uHigh)))
      return res;
   if( (res = memory_read_send( state, (unsigned int)u64RTDExFpgaBaseAddr + RTDEX_ACCESS_PEERMACADDRLO, &uLow)))
      return res;
   pPeerAddr[ 0] = (uLow >> 24) & 0xff;
   pPeerAddr[ 1] = (uLow >> 16) & 0xff;
   pPeerAddr[ 2] = (uLow >> 8) & 0xff;
   pPeerAddr[ 3] = (uLow >> 0) & 0xff;
   pPeerAddr[ 4] = (uHigh >> 8) & 0xff;
   pPeerAddr[ 5] = (uHigh >> 0) & 0xff;

   if( (res = memory_read_send( state, (unsigned int)u64RTDExFpgaBaseAddr + RTDEX_ACCESS_FPGAMACADDRHI, &uHigh)))
      return res;
   if( (res = memory_read_send( state, (unsigned int)u64RTDExFpgaBaseAddr + RTDEX_ACCESS_FPGAMACADDRLO, &uLow)))
      return res;
   pFpgaAddr[ 0] = (uLow >> 24) & 0xff;
   pFpgaAddr[ 1] = (uLow >> 16) & 0xff;
   pFpgaAddr[ 2] = (uLow >> 8) & 0xff;
   pFpgaAddr[ 3] = (uLow >> 0) & 0xff;
   pFpgaAddr[ 4] = (uHigh >> 8) & 0xff;
   pFpgaAddr[ 5] = (uHigh >> 0) & 0xff;
   return 0;
}




static adp_result_t SetPeerMacAddr( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, unsigned char pMacAddr[6])
{
    uint64_t u64Reg1Addr, u64Reg2Addr;
    adp_result_t res;

    uint32_t u32LowAddr;
    uint32_t u32HighAddr;

    u32LowAddr = (pMacAddr[4] << 8) | pMacAddr[5];
    u32HighAddr = (pMacAddr[0] << 24) | (pMacAddr[1] << 16) | (pMacAddr[2] << 8) | (pMacAddr[3]);

    u64Reg1Addr = u64RTDExFpgaBaseAddr + RTDEX_ACCESS_PEERMACADDRLO;
    u64Reg2Addr = u64RTDExFpgaBaseAddr + RTDEX_ACCESS_PEERMACADDRHI;
    res = memory_write_send( state, (unsigned int)u64Reg1Addr, u32HighAddr);
    if( res) {
        return res;
    }
    res = memory_write_send( state, (unsigned int)u64Reg2Addr, u32LowAddr);
	 if( res) {
        return res;
    }
    return RTDEXOK;
}


static adp_result_t SetFpgaMacAddr( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, unsigned char pMacAddr[6])
{
    uint64_t u64Reg1Addr, u64Reg2Addr;
    adp_result_t res;

    uint32_t u32LowAddr;
    uint32_t u32HighAddr;

    u32LowAddr = (pMacAddr[4] << 8) | pMacAddr[5];
    u32HighAddr = (pMacAddr[0] << 24) | (pMacAddr[1] << 16) | (pMacAddr[2] << 8) | (pMacAddr[3]);

    u64Reg1Addr = u64RTDExFpgaBaseAddr + RTDEX_ACCESS_FPGAMACADDRLO ;
    u64Reg2Addr = u64RTDExFpgaBaseAddr + RTDEX_ACCESS_FPGAMACADDRHI ;
    res = memory_write_send( state, (unsigned int)u64Reg1Addr, u32HighAddr );
    if( res ) {
        return res;
    }
    res = memory_write_send( state, (unsigned int)u64Reg2Addr, u32LowAddr );
    if( res ) {
        return res;
    }
    return RTDEXOK;
}



static int sscanMacAddress( char * s, unsigned char * pucMacAddr)
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


// Get the MAC address of the Perseus
static int _GetPerseusMacAddr( connection_state * state, unsigned char pucPeerMacAddr[ 6])
{
   int res;
   char acMacAddrString[ 20];

   if( (res = get_mac_address( state, acMacAddrString)))
      return res;
   res = sscanMacAddress( acMacAddrString, pucPeerMacAddr);
   return res;
}

static int _GetLocalMacAddr( connection_state * state, unsigned char pucLocalMacAddr[ 6])
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
	   if( localIp == inet_addr( acHostIpAddr))
          break;
   }
   if(res == 0)
      memcpy( pucLocalMacAddr, aucHostMacAddr, 6);
   return res;
}



#if defined(_WIN32) || defined(_WIN64)

// Helper to retreive MAC address of Host Interface
adp_result_t RTDExGetMacEth( unsigned int uIndex, unsigned char pHostMacAddr[6], char pHostIpAddr[16], char pDescription[261])
{
   ULONG dwSize;
   IP_ADAPTER_INFO* pIfTable = 0;
   IP_ADAPTER_INFO* pIfIterator = 0;
   int retVal;
   int res;
   unsigned u;

   retVal = GetAdaptersInfo( pIfTable, &dwSize );
   pIfTable = (IP_ADAPTER_INFO*) malloc( dwSize );
   if( pIfTable == NULL)
       return -1;
   retVal = GetAdaptersInfo( pIfTable, &dwSize);
   if( retVal)
      return -1;

   for( u = 0, pIfIterator = pIfTable; pIfIterator != 0 && u < uIndex; pIfIterator = pIfIterator->Next, u++)
      ;

   pDescription[ 0] = 0;
   if( pIfIterator != NULL
   && pIfIterator->AddressLength == 6) {
       strcpy( pHostIpAddr, pIfIterator->IpAddressList.IpAddress.String);
       memcpy( pHostMacAddr, pIfIterator->Address, 6);
       strcpy( pDescription, pIfIterator->AdapterName);
       res = 0;
   }
   else
      res = -1;
   free( pIfTable);
   return res;
}
#endif



#ifdef __linux__
// Helper to retreive MAC address of Host Interface
adp_result_t RTDExGetMacEth( unsigned int uIndex, unsigned char pHostMacAddr[6], char pHostIpAddr[16], char pDescription[261])
{
   unsigned int i;
   struct in_addr addr;

   static char pErrorBuffer[ PCAP_ERRBUF_SIZE];
   pcap_if_t* pAllDev;
   pcap_if_t* pDevIterator;
   pcap_addr_t* pAddrIterator;

   if( pcap_findalldevs( &pAllDev, pErrorBuffer ) == -1) {
       return -1;
   }

   for( pDevIterator = pAllDev, i = 0; pDevIterator != 0 && i < uIndex; pDevIterator = pDevIterator->next, i++)
       ;

   if( pDevIterator == NULL)
       return -1;

   addr.s_addr = 0;
   memset( pHostMacAddr, 0, 6);
   for( pAddrIterator = pDevIterator-> addresses; pAddrIterator != NULL; pAddrIterator = pAddrIterator->next) {
//        printf( "4");
        // check for mac address:
        if( pAddrIterator->addr->sa_family == PF_PACKET) {
//           printf( "5");
           if( pHostMacAddr != NULL)
               memcpy( pHostMacAddr, ((struct sockaddr_ll*)pAddrIterator->addr)->sll_addr, 6);
        }
        // check for ip addr:
        if( pAddrIterator->addr->sa_family == AF_INET) {
//            printf( "6");
            memcpy( &addr.s_addr, &pAddrIterator->addr->sa_data[ 2], 4);
        }
   }

   if( pHostIpAddr != NULL) {
       strcpy( pHostIpAddr, inet_ntoa( addr));
   }

   if( pDescription != NULL) {
       *pDescription  = 0;
       if( pDevIterator->description != NULL) {
//           printf( "9");
           strcpy( pDescription, pDevIterator->description);
       }
   }

   pcap_freealldevs( pAllDev);

//   printf( "0\n");
   return 0;
}
#endif


adp_result_t RTDExResetFifoEth( adp_handle_rtdex_conn hConnRtDex)
{
    adp_result_t res;
    uint64_t u64RegAddr;
    uint32_t uBit;
    adp_handle_rtdex_ethernet_conn pEthConn = ((adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn));

    u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + RTDEX_ACCESS_RXTXFIFORST;

    if( pEthConn->eConnType == eHostToFpga || pEthConn->eConnType == eFpgaToFpga) {
    
        // Select which bit to toggle
        uBit = (1 << pEthConn->uDstChannelId) << RTDEX_RXFIFORESET_BO;

        // Set the bit high
        res = memory_write_with_mask_send( hConnRtDex->DstMediaState, (uint32_t)u64RegAddr, 0xFFFFFFFF, uBit);
        if( FAILURE(res) ) return res;
        // Set the bit low
        res = memory_write_with_mask_send( hConnRtDex->DstMediaState, (uint32_t)u64RegAddr, 0x00000000, uBit);
        if( FAILURE(res) ) return res;
    }

    if( pEthConn->eConnType == eFpgaToHost || pEthConn->eConnType == eFpgaToFpga) {
    
        // Select which bit to toggle
        uBit = (1 << pEthConn->uSrcChannelId) << RTDEX_TXFIFORESET_BO;

        // Set the bit high
        res = memory_write_with_mask_send( hConnRtDex->SrcMediaState, (uint32_t)u64RegAddr, 0xFFFFFFFF, uBit);
        if( FAILURE(res) ) return res;
        // Set the bit low
        res = memory_write_with_mask_send( hConnRtDex->SrcMediaState, (uint32_t)u64RegAddr, 0x00000000, uBit);
        if( FAILURE(res) ) return res;
    }

    return res;
}




// old interface - depreciated
adp_result_t RTDExOpenEth(  adp_handle_rtdex_conn *hConn,
                            unsigned char pPeerMacAddr[ 6], // peer MAC address (Host or Persus)
                            ePeerEndpointType ePeerType,  // eHost or ePerseus
                            unsigned char pFpgaMacAddr[6], // fpga MAC address
               			    connection_state * state,
                            uint64_t u64RTDExFpgaBaseAddr,
                            unsigned int uNumChannel,
                            eRTDExTransferDir eDir, // eToFpga or eFromFpga
                            eRTDExMode eMode) // eSingle, eContinuous
{
   unsigned int uNbChannels;
   adp_handle_rtdex_ethernet_conn hEthernetRTDEx;
   adp_result_t res = 0;
   uint32_t uCoreId, uCoreVer;
   adp_handle_rtdex_conn hTempConn = NULL;

if( ePeerType == ePerseus) {
   return RTDEXOLDETHNOTSUPPORTED;
}
   if( (res = RTDExGetCoreIdVerEth( state, u64RTDExFpgaBaseAddr, &uCoreId, &uCoreVer)))
      return res;
   if( uCoreId != RTDEX_COREID || uCoreVer < RTDEX_COREVER)
      return RTDEXBADCOREIDORVER;

   if( uNumChannel >= MAX_NUM_OF_RTDEX) {
        return (RTDEXINVCHANNELNUM);
   }

   hTempConn = (adp_handle_rtdex_conn)(malloc(sizeof(sMediaRTDExStruct)));
   if(hTempConn == NULL) {
        return (RTDEXMEMALLOC); // Allocation error
   }
   hEthernetRTDEx = (adp_handle_rtdex_ethernet_conn)(malloc(sizeof(sEthernetRTDExStruct)));
   if(hEthernetRTDEx == NULL) {
        return (RTDEXMEMALLOC); // Allocation error
   }
   memset( hEthernetRTDEx, 0, sizeof *hEthernetRTDEx);

   hTempConn->eMedia = eMediaSocketIp;
   hTempConn->hMediaRTDexConn = (adp_handle_rtdex_media_conn)hEthernetRTDEx;
   hTempConn->SrcMediaState = state;
   hTempConn->DstMediaState = state;

   if( ePeerType == eHost) {
      res = adp_GetInterfacePtrFromAddr( pPeerMacAddr,
                                          &hEthernetRTDEx -> pSourceInterface,
                                          &hEthernetRTDEx -> pSourceInterfaceList,
                                          &hEthernetRTDEx -> pInterfaceDmaHandle,
                                          READ_TIMEOUT_MS );
      if( res)
          return res;
   }

   hTempConn->u64RTDExFpgaBaseAddr = u64RTDExFpgaBaseAddr;
   hEthernetRTDEx->uDstChannelId = uNumChannel;
   hEthernetRTDEx->uSrcChannelId = uNumChannel;
//   hEthernetRTDEx->eDir = eDir;
   hEthernetRTDEx->eMode = eMode;
//   hEthernetRTDEx->ePeerType = ePeerType;

   // if rtdex is RX, read instanciated RX channels
   if( eDir == eToFpga) {
       RTDExGetRxNbChannelsEth( hTempConn->SrcMediaState,
                             hTempConn->u64RTDExFpgaBaseAddr,
                             &uNbChannels );
   }

   // if rtdex is TX, read instanciated TX channels
   else {
       RTDExGetTxNbChannelsEth( hTempConn->SrcMediaState,
                             hTempConn->u64RTDExFpgaBaseAddr,
                             &uNbChannels );
   }
   // raise errors if channel does not exist
   if( uNumChannel >= uNbChannels ) {
       return (RTDEXINVCHANNELNUM);
   }
   if(eDir == eToFpga)
   {
       memcpy( hEthernetRTDEx->pSrcMacAddr, pPeerMacAddr, 6 );
       memcpy( hEthernetRTDEx->pDstMacAddr, pFpgaMacAddr, 6 );
   }
   else
   {
       memcpy( hEthernetRTDEx->pDstMacAddr, pPeerMacAddr, 6 );
       memcpy( hEthernetRTDEx->pSrcMacAddr, pFpgaMacAddr, 6 );
   }
   hEthernetRTDEx->eProtId = eProtocolRTDEX;
   hEthernetRTDEx->bGetBroadCast = 0;
   hEthernetRTDEx->eConnType = eDir == eToFpga ? eHostToFpga : eFpgaToHost;

    // if host receives data, apply ethernet filter
   if( ePeerType == eHost && eDir == eFromFpga) {
      res = ApplyFilter( hEthernetRTDEx, uNumChannel, 0 );
       if( res != 0 )
           return (res);
   }

   res = SetPeerMacAddr( state, u64RTDExFpgaBaseAddr, pPeerMacAddr);
   if(res) {
       return (res);
   }

   res = SetFpgaMacAddr( state, u64RTDExFpgaBaseAddr, pFpgaMacAddr);
   if(res) {
       return (res);
   }

   RTDExResetFifoEth( hTempConn);
   if( eDir == eFromFpga)
      TxResetTransfer( hTempConn);
   else // eToFpga
      RxResetTransfer( hTempConn);

   *hConn = hTempConn;

   return (RTDEXOK);
}


adp_result_t RTDExOpenHostAndFpgaEth( adp_handle_rtdex_conn *hConn,
                                      connection_state * state,
                                      uint64_t u64RTDExFpgaBaseAddr,
                                      unsigned int uChannelNo,
                                      eRTDExTransferDir eDir, // eToFpga or eFromFpga
                                      eRTDExMode eMode) // eSingle, eContinuous
{
    return RTDExOpenHostAndFpgaEthTo( hConn, state, u64RTDExFpgaBaseAddr, uChannelNo, eDir, eMode, READ_TIMEOUT_MS );
}
// new interface
adp_result_t RTDExOpenHostAndFpgaEthTo( adp_handle_rtdex_conn *hConn,
                                      connection_state * state,
                                      uint64_t u64RTDExFpgaBaseAddr,
                                      unsigned int uChannelNo,
                                      eRTDExTransferDir eDir, // eToFpga or eFromFpga
                                      eRTDExMode eMode, // eSingle, eContinuous
                                      int pcapReadTo ) 
//###
{
   unsigned int uNbChannels;
   adp_handle_rtdex_ethernet_conn hEthernetRTDEx;
   adp_result_t res;
   uint32_t uCoreId, uCoreVer;
   adp_handle_rtdex_conn hTempConn = NULL;
   unsigned char pFpgaMacAddr[ 6];
   unsigned char pHostMacAddr[ 6];
   unsigned char pucNullMac[6] = { 0, 0, 0, 0, 0, 0 };

   if( (res = RTDExGetCoreIdVerEth( state, u64RTDExFpgaBaseAddr, &uCoreId, &uCoreVer)))
      return res;
   if( uCoreId != RTDEX_COREID || uCoreVer < RTDEX_COREVER)
      return RTDEXBADCOREIDORVER;
   if( uChannelNo >= MAX_NUM_OF_RTDEX) {
      return (RTDEXINVCHANNELNUM);
   }

   // Get and set FPGA MAC address
   if( (res = _GetPerseusMacAddr( state, pFpgaMacAddr)))
      return res;
   if( (res = SetFpgaMacAddr( state, u64RTDExFpgaBaseAddr, pFpgaMacAddr)))
      return (res);

   // Get and set Host MAC address
   if( (res = _GetLocalMacAddr( state, pHostMacAddr)))
      return res;
   if( bIsEquateMacAddr( pHostMacAddr, pucNullMac))
      return RTDEXINVALHOSTMACADDR;
   if( (res = SetPeerMacAddr( state, u64RTDExFpgaBaseAddr, pHostMacAddr)))
      return (res);

   hTempConn = (adp_handle_rtdex_conn)(malloc(sizeof(sMediaRTDExStruct)));
   if( hTempConn == NULL) {
        return (RTDEXMEMALLOC); // Allocation error
   }
   memset( hTempConn, 0, sizeof *hTempConn);

   hEthernetRTDEx = (adp_handle_rtdex_ethernet_conn)(malloc(sizeof(sEthernetRTDExStruct)));
   if(hEthernetRTDEx == NULL) {
      free( hTempConn);
        return (RTDEXMEMALLOC); // Allocation error
   }
   memset( hEthernetRTDEx, 0, sizeof *hEthernetRTDEx);

   hTempConn->eMedia = eMediaSocketIp;
   hTempConn->hMediaRTDexConn = (adp_handle_rtdex_media_conn)hEthernetRTDEx;
   if( eDir == eToFpga)
      hTempConn->DstMediaState = state;
   else
      hTempConn->SrcMediaState = state;

   res = adp_GetInterfacePtrFromAddr( pHostMacAddr,
                                          &hEthernetRTDEx -> pSourceInterface,
                                          &hEthernetRTDEx -> pSourceInterfaceList,
                                          &hEthernetRTDEx -> pInterfaceDmaHandle,
                                          pcapReadTo );
   if( res)
      return res;

   hTempConn->u64RTDExFpgaBaseAddr = u64RTDExFpgaBaseAddr;
   if( eDir == eToFpga)
      hEthernetRTDEx->uDstChannelId = uChannelNo;
   else
      hEthernetRTDEx->uSrcChannelId = uChannelNo;
   hEthernetRTDEx->eMode = eMode;
   hEthernetRTDEx->eConnType = eDir == eToFpga ? eHostToFpga : eFpgaToHost;

   // if rtdex is RX, read instanciated RX channels
   if( eDir == eToFpga)
       RTDExGetRxNbChannelsEth( state, hTempConn->u64RTDExFpgaBaseAddr, &uNbChannels );
   else // else rtdex is TX, read instanciated TX channels
       RTDExGetTxNbChannelsEth( state, hTempConn->u64RTDExFpgaBaseAddr, &uNbChannels );
   // raise errors if channel does not exist
   if( uChannelNo >= uNbChannels ) {
       return (RTDEXINVCHANNELNUM);
   }

   hEthernetRTDEx->eProtId = eProtocolRTDEX;
   hEthernetRTDEx->bGetBroadCast = 0;

   if( eDir == eFromFpga) {
      memcpy( hEthernetRTDEx->pSrcMacAddr, pFpgaMacAddr, 6);
      memcpy( hEthernetRTDEx->pDstMacAddr, pHostMacAddr, 6);
   }
   else {
      memcpy( hEthernetRTDEx->pSrcMacAddr, pHostMacAddr, 6);
      memcpy( hEthernetRTDEx->pDstMacAddr, pFpgaMacAddr, 6);
   }

   // if host receives data, apply ethernet filter
   if( eDir == eFromFpga) {
      res = ApplyFilter( hEthernetRTDEx, uChannelNo, 0 );
       if( res != 0 )
           return (res);
   }

   RTDExResetFifoEth( hTempConn);

   if( eDir == eFromFpga)
      TxResetTransfer( hTempConn);
   else // eToFpga
      RxResetTransfer( hTempConn);

   *hConn = hTempConn;

   return (RTDEXOK);
}




// new interface
adp_result_t RTDExOpenFpga2FpgaEth( adp_handle_rtdex_conn *hConn,
                                    connection_state * SrcState,
                                    connection_state * DstState,
                                    uint64_t u64RTDExFpgaBaseAddr,
                                    unsigned int uChannelNo,
                                    eRTDExMode eMode)
{
   unsigned int uNbChannels;
   adp_handle_rtdex_ethernet_conn hEthernetRTDEx;
   adp_result_t res;
   uint32_t uCoreId, uCoreVer;
   adp_handle_rtdex_conn hTempConn = NULL;
   unsigned char pucSrcFpgaMacAddr[ 6];
   unsigned char pucDstFpgaMacAddr[ 6];
   unsigned char pucNullMac[6] = { 0, 0, 0, 0, 0, 0 };
   unsigned char pucSrcActualPeerMac[ 6];
   unsigned char pucSrcActualFpgaMac[ 6];
#define uSrcChannelNo uChannelNo
#define uDstChannelNo uChannelNo

   if( (res = RTDExGetCoreIdVerEth( SrcState, u64RTDExFpgaBaseAddr, &uCoreId, &uCoreVer)))
      return res;
   if( uCoreId != RTDEX_COREID || uCoreVer < RTDEX_COREVER)
      return RTDEXBADCOREIDORVER;

   if( (res = RTDExGetCoreIdVerEth( DstState, u64RTDExFpgaBaseAddr, &uCoreId, &uCoreVer)))
      return res;
   if( uCoreId != RTDEX_COREID || uCoreVer < RTDEX_COREVER)
      return RTDEXBADCOREIDORVER;

   if( uSrcChannelNo >= MAX_NUM_OF_RTDEX || uDstChannelNo >= MAX_NUM_OF_RTDEX) {
      return RTDEXINVCHANNELNUM;
   }

   // make sure src FPGA MAC address is set:
   if( (res = _GetPerseusMacAddr( SrcState, pucSrcFpgaMacAddr)))
      return res;
   if( (res = SetFpgaMacAddr( SrcState, u64RTDExFpgaBaseAddr, pucSrcFpgaMacAddr)))
      return (res);

   // make sure dst FPGA MAC address is set:
   if( (res = _GetPerseusMacAddr( DstState, pucDstFpgaMacAddr)))
      return res;
   if( (res = SetFpgaMacAddr( DstState, u64RTDExFpgaBaseAddr, pucDstFpgaMacAddr)))
      return res;


   if( (res = GetRtdexMacAddrs( SrcState, u64RTDExFpgaBaseAddr, pucSrcActualPeerMac, pucSrcActualFpgaMac)))
      return res;
   if( !bIsEquateMacAddr( pucSrcActualPeerMac, pucNullMac) && !bIsEquateMacAddr( pucSrcActualPeerMac, pucDstFpgaMacAddr))
      return RTDEXPEERMACADDRCONFLICT;
   if( (res = SetPeerMacAddr( SrcState, u64RTDExFpgaBaseAddr, pucDstFpgaMacAddr)))
      return res;

   hTempConn = (adp_handle_rtdex_conn)(malloc(sizeof(sMediaRTDExStruct)));
   if(hTempConn == NULL) {
        return RTDEXMEMALLOC; // Allocation error
   }
   memset( hTempConn, 0, sizeof *hTempConn);

   hEthernetRTDEx = (adp_handle_rtdex_ethernet_conn)(malloc(sizeof(sEthernetRTDExStruct)));
   if(hEthernetRTDEx == NULL) {
      free( hTempConn);
        return RTDEXMEMALLOC; // Allocation error
   }
   memset( hEthernetRTDEx, 0, sizeof *hEthernetRTDEx);

   hTempConn->eMedia = eMediaSocketIp;
   hTempConn->hMediaRTDexConn = (adp_handle_rtdex_media_conn)hEthernetRTDEx;
   hTempConn->SrcMediaState = SrcState;
   hTempConn->DstMediaState = DstState;

   hTempConn->u64RTDExFpgaBaseAddr = u64RTDExFpgaBaseAddr;
   hEthernetRTDEx->uSrcChannelId = uSrcChannelNo;
   hEthernetRTDEx->uDstChannelId = uDstChannelNo;
   hEthernetRTDEx->eMode = eMode;

   RTDExGetRxNbChannelsEth( hTempConn->SrcMediaState, hTempConn->u64RTDExFpgaBaseAddr, &uNbChannels);
   // raise errors if channel does not exist
   if( uSrcChannelNo >= uNbChannels)
       return (RTDEXINVCHANNELNUM);
   RTDExGetTxNbChannelsEth( hTempConn->DstMediaState, hTempConn->u64RTDExFpgaBaseAddr, &uNbChannels);
   // raise errors if channel does not exist
   if( uDstChannelNo >= uNbChannels)
       return (RTDEXINVCHANNELNUM);

   hEthernetRTDEx->eConnType = eFpgaToFpga;
   hEthernetRTDEx->eProtId = eProtocolRTDEX;
   hEthernetRTDEx->bGetBroadCast = 0;

   RTDExResetFifoEth( hTempConn);
   TxResetTransfer( hTempConn);
   RxResetTransfer( hTempConn);

   *hConn = hTempConn;
   return (RTDEXOK);
}




adp_result_t RTDExCloseEth( adp_handle_rtdex_conn hConnRtDex)
{
   adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);

   if( pEthConn->eConnType == eFpgaToHost || pEthConn->eConnType == eFpgaToFpga) // added by MSA 25mar13
      TxResetTransfer( hConnRtDex);

   if( pEthConn -> pInterfaceDmaHandle)
   {
       pcap_close( (pcap_t*) pEthConn -> pInterfaceDmaHandle );
       pEthConn -> pInterfaceDmaHandle = 0;
   }

   if( pEthConn -> pSourceInterfaceList)
   {
       pcap_freealldevs( (pcap_if_t *)pEthConn -> pSourceInterfaceList  );
       pEthConn -> pSourceInterfaceList = 0;
       pEthConn -> pSourceInterface = 0;
   }

   free( hConnRtDex->hMediaRTDexConn);
   free( hConnRtDex);

   return RTDEXOK;
}


adp_result_t RTDExStartEth( adp_handle_rtdex_conn hConnRtDex, uint32_t u32FrameSize, uint32_t u32TransferSize)
{
#ifdef USE_JUMBO_FRAME
    #define MAX_PACKET_SIZE 9050
#else
    #define MAX_PACKET_SIZE 1550
#endif
   adp_result_t res;
   adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);

   if( pEthConn->eConnType == eFpgaToHost || pEthConn->eConnType == eFpgaToFpga) {
      if( u32FrameSize == 0 ) {
          return RTDEXINVFRAMESIZE;
      }
      if( ( u32FrameSize > u32TransferSize ) && ( u32TransferSize != 0 ) ) {
          u32FrameSize = u32TransferSize;
      }
   }

//   res = SetTxModeContinuous( hConnRtDex, pEthConn->eMode == eContinuous ? 1 : 0); // removed by MSA, 25mar13
   if( pEthConn->eMode == eContinuous) {
        u32TransferSize = 0;
   }
   
   if( u32FrameSize == 0) {
      return RTDEXINVFRAMESIZE;
   }

   if( u32FrameSize + g_uHeaderSize > MAX_PACKET_SIZE) {
      return RTDEXFRAMESIZETOOBIG;
   }

   pEthConn -> u32FrameSize = u32FrameSize;
   pEthConn -> u32TransferSize = u32TransferSize;

   if( pEthConn->eConnType == eHostToFpga || pEthConn->eConnType == eFpgaToFpga) {
       res = RxStartTransfer( hConnRtDex);
       if(res) {
//           printf("Error starting transfer. Exception:%x",res);
         return res;
      }
   }
   if( pEthConn->eConnType == eFpgaToHost || pEthConn->eConnType == eFpgaToFpga) {
      res = SetTxModeContinuous( hConnRtDex, pEthConn->eMode == eContinuous ? 1 : 0); // moved here by MSA, 25mar13
      if( res)
         return res;
      res = SetTxFrameSize( hConnRtDex, u32FrameSize );
      if( res)
         return res;
      res = SetTxTransferSize( hConnRtDex, u32TransferSize / u32FrameSize );
      if( res)
         return res;
      res = TxStartTransfer( hConnRtDex );
      if(res) {
//           printf("Error starting transfer. Exception:%x",res);
         return res;
       }
   }
   return RTDEXOK;
}



int RTDExSendEth( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize)
{
    uint32_t u32RemainingSize = 0;
    uint32_t u32CurrentFrameSize = 0;
    pcap_t* pInterfaceDmaHandle;
#ifdef USE_JUMBO_FRAME
   unsigned char pPacketBuffer[ 9050];
#else
   unsigned char pPacketBuffer[ 1550];
#endif
   uint32_t u32BufferDataPos;

    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);

    if(u32MaxSize == 0) {
       pEthConn -> resLastError = RTDEXINVALIDBUFFERSIZE;
        return -1;
    }

   if(pDataBuffer == 0) {
      pEthConn -> resLastError = RTDEXINVALIDBUFFER;
        return -1;
    }

    pInterfaceDmaHandle = (pcap_t*) pEthConn -> pInterfaceDmaHandle;
    if( pInterfaceDmaHandle == 0 ) {
      pEthConn -> resLastError = RTDEXINVINTERFACEDMAHANDLE;
        return -1;
    }

   if( pEthConn->eConnType != eHostToFpga) {
      pEthConn -> resLastError = RTDEXWRONGDIRECTION;
      return -1;
   }

   u32RemainingSize = u32MaxSize;

   memcpy( pPacketBuffer, pEthConn->pDstMacAddr, 6 );  // Dst MAC
   memcpy( &pPacketBuffer[6], pEthConn->pSrcMacAddr, 6 ); // Src MAC

   vSetU16BigEndian( pPacketBuffer + 14, g_uSnapHeaders);
    vSetU32BigEndian( pPacketBuffer +16, g_uLyrtechHeaders);
    vSetU16BigEndian( pPacketBuffer + 20, g_uSubId);
    pPacketBuffer[22] = (unsigned char) pEthConn -> eProtId;
   pPacketBuffer[23] = pEthConn->uDstChannelId;

    u32BufferDataPos = 0;
    while(u32RemainingSize > 0)
    {

        if(u32RemainingSize <= pEthConn->u32FrameSize)
        {
            u32CurrentFrameSize = u32RemainingSize;
        }
        else
        {
            u32CurrentFrameSize = pEthConn->u32FrameSize;
        }

        vSetU32BigEndian( &pPacketBuffer[24], pEthConn->uPacketNumber);
      vSetU32BigEndian( &pPacketBuffer[28], u32CurrentFrameSize);
#ifdef USE_JUMBO_FRAME
      if( u32CurrentFrameSize > 1472)
         vSetU16BigEndian( pPacketBuffer + 12, g_uJumboFrames);
      else
#endif
      vSetU16BigEndian( pPacketBuffer + 12, u32CurrentFrameSize + g_uSnapHeaderSize + g_uRTDExHeaderSize); // eth-type: len

      memcpy( &pPacketBuffer[g_uHeaderSize], &pDataBuffer[u32BufferDataPos],
               u32CurrentFrameSize );

      if( pcap_sendpacket( pInterfaceDmaHandle, pPacketBuffer, u32CurrentFrameSize + g_uHeaderSize) != 0 ) {
         pEthConn -> resLastError = RTDEXPCAPSEND;
            return -1;
        }
        u32BufferDataPos += u32CurrentFrameSize;
        u32RemainingSize -= u32CurrentFrameSize;
        pEthConn->uPacketNumber++;
      pEthConn->uHost2FpgaFrameCount++;
    }
    return u32MaxSize;
}



int RTDExReceiveEth( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, eRTDExTransferMode eMode, unsigned int uTimeoutMs )
{
    uint32_t u32RemainingSize = 0;
    uint32_t u32ReceivedSize = 0;
    uint32_t u32BufferDataPos = 0;
    unsigned short uReadTransferSize;
    pcap_t* pInterfaceDmaHandle;
    unsigned int uFrameNumber;
    int iPcapRet;
    struct pcap_pkthdr *header;
    const u_char *pkt_data = 0;

    lyr_delay_counter timer;
    double dDelaySec;
    uint32_t u32DelayMsec;

    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);

    pEthConn -> resLastError = 0;

    if(pDataBuffer == 0) {
        pEthConn -> resLastError = RTDEXINVALIDBUFFER;
        return -1;
    }
    if( pEthConn -> pSourceInterface == 0 ) {
        pEthConn -> resLastError = RTDEXINVSOURCEINTERFACE;
        return -1;
    }
    if( pEthConn->eConnType != eFpgaToHost) {
        pEthConn -> resLastError = RTDEXWRONGDIRECTION;
        return -1;
    }

    // Start Timeout timer
    if(eMode == eRTDExWaitTimeout || eMode == eRTDExWaitTimeoutOrFirstError)
    {
        lyr_counter_reset(&timer);
        lyr_counter_begin(&timer);
    }

    u32RemainingSize = u32MaxSize;
    while( u32RemainingSize) {

        if(eMode == eRTDExWaitTimeout || eMode == eRTDExWaitTimeoutOrFirstError)
        {
            lyr_counter_end(&timer);
            lyr_counter_get(&timer, &dDelaySec);
            u32DelayMsec = (uint32_t)(dDelaySec * 1000);
            
            if(u32DelayMsec >= uTimeoutMs)
                return u32ReceivedSize;
        }

        pInterfaceDmaHandle = (pcap_t*) pEthConn -> pInterfaceDmaHandle;
        if( pInterfaceDmaHandle == 0 ) {
            pEthConn -> resLastError = RTDEXINVINTERFACEDMAHANDLE;
            return -1;
        }

        iPcapRet = pcap_next_ex( pInterfaceDmaHandle, &header, &pkt_data );
        
        // if pcap_next_ex function timeout
        if( iPcapRet != 1 )
        {
            if(eMode == eRTDExDontWait)
                return u32ReceivedSize;
            else
                continue;
        }
        
        if( header->len == 0 )
        {
            continue;
        }

        pEthConn->uFpga2HostFrameCount++;
        uFrameNumber = u32GetU32BigEndian( pkt_data + 24);
        if( uFrameNumber != pEthConn->uExpectedRxFrameNumber)
        {
            pEthConn->uFpga2HostFrameLostCount++;
//          pEthConn->uFpga2HostFrameLostCount += uFrameNumber - pEthConn->uExpectedRxFrameNumber;
            pEthConn->uExpectedRxFrameNumber = uFrameNumber;
            
            pEthConn->uExpectedRxFrameNumber++;
            if(eMode == eRTDExWaitTimeoutOrFirstError)
            {
                 return u32ReceivedSize;
            }
        }
        else
            pEthConn->uExpectedRxFrameNumber++;
            
        uReadTransferSize = u32GetU32BigEndian( pkt_data + 28);

        if( u32RemainingSize > uReadTransferSize )
        {
            u32RemainingSize -= uReadTransferSize;
            u32ReceivedSize += uReadTransferSize;
        }
        else
        {
            uReadTransferSize = u32RemainingSize;
            u32RemainingSize = 0;
            u32ReceivedSize += uReadTransferSize;
        }

        memcpy( &pDataBuffer[u32BufferDataPos],  &pkt_data[g_uHeaderSize], uReadTransferSize );
        u32BufferDataPos += uReadTransferSize;

    } // while( u32RemainingSize)

    return u32ReceivedSize;
}



adp_result_t RTDExGetLastErrorEth( adp_handle_rtdex_conn hConnRtDex)
{
    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);
    return pEthConn->resLastError;
}

void RTDExSetLastErrorEth( adp_handle_rtdex_conn hConnRtDex, adp_result_t Result)
{
    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);
    
    pEthConn->resLastError = Result;
}


// Return the number of lost frame (based on frame numbering) during reception on Host side.
adp_result_t RTDExGetRxFramesLostCountEth( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal)
{
    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);
   uint64_t u64RegAddr;
   adp_result_t res;

   if( pEthConn->eConnType == eHostToFpga || pEthConn->eConnType == eFpgaToFpga) {
      u64RegAddr =  hConnRtDex->u64RTDExFpgaBaseAddr + (RTDEX_ACCESS_RXFRAMELOSTCNT_ARRAY + (pEthConn->uDstChannelId * 4));
   	res = memory_read_send( hConnRtDex->DstMediaState, (unsigned int)u64RegAddr, pVal);
      return res;
   }
   else // eFpgaToHost
      *pVal = pEthConn->uFpga2HostFrameLostCount;
   return RTDEXOK;
}


adp_result_t RTDExGetRxDropFramesCntEth( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32Cnt)
{
    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);
   uint64_t u64RegAddr;
   adp_result_t res;

   if( pEthConn->eConnType == eHostToFpga || pEthConn->eConnType == eFpgaToFpga) {
      u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + (RTDEX_ACCESS_RX_DROP_FRAME_CNT_ARRAY + (pEthConn->uDstChannelId * 4 ) );
   	res = memory_read_send( hConnRtDex->DstMediaState, (unsigned int)u64RegAddr, pu32Cnt );
      return res;
   }
   else // eFpgaToHost:
      *pu32Cnt = 0;
   return RTDEXOK;
}


// Return the number of sent frames
adp_result_t RTDExGetTxFramesCountEth( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal)
{
    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);
   uint64_t u64RegAddr;
   adp_result_t res;

   if( pEthConn->eConnType == eFpgaToHost || pEthConn->eConnType == eFpgaToFpga) {
      u64RegAddr =  hConnRtDex->u64RTDExFpgaBaseAddr + (RTDEX_ACCESS_TXFRAMECNT_ARRAY + (pEthConn->uSrcChannelId * 4));
      res = memory_read_send( hConnRtDex->SrcMediaState, (unsigned int)u64RegAddr, pVal);
      return res;
   }
   else // eHost2Dev:
      *pVal = pEthConn->uHost2FpgaFrameCount;
   return RTDEXOK;
}


// Return the number of recv frames
adp_result_t RTDExGetRxFramesCountEth( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal)
{
    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);
   uint64_t u64RegAddr;
   adp_result_t res;

   if( pEthConn->eConnType == eHostToFpga || pEthConn->eConnType == eFpgaToFpga) {
      u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + ( RTDEX_ACCESS_RXFRAMECNT_ARRAY + ( pEthConn->uDstChannelId * 4));
      res = memory_read_send( hConnRtDex->DstMediaState, (unsigned int)u64RegAddr, pVal);
      return res;
   }
   else // eFpgaToHost:
      *pVal = pEthConn->uFpga2HostFrameCount;
   return RTDEXOK;
}


adp_result_t RTDExGetTxFifoDataCntEth( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32DataCount)
{
    adp_handle_rtdex_ethernet_conn pEthConn = (adp_handle_rtdex_ethernet_conn)(hConnRtDex->hMediaRTDexConn);
   uint64_t u64RegAddr;
   adp_result_t res;

   if( pEthConn->eConnType == eFpgaToHost || pEthConn->eConnType == eFpgaToFpga) {
      u64RegAddr = hConnRtDex->u64RTDExFpgaBaseAddr + RTDEX_ACCESS_TXDATACNT_ARRAY + ( pEthConn->uSrcChannelId * 4);
      res = memory_read_send( hConnRtDex->SrcMediaState, (unsigned int)u64RegAddr, pu32DataCount);
      return res;
   }
   else // eHostToFpga
        * pu32DataCount = 0;
   return RTDEXOK;
}


// =============================================================================================

// per core functions:


adp_result_t RTDExGetRxBadFramesCntEth( connection_state * state, uint64_t u64BaseAddr, uint32_t* pu32Cnt)
{
   adp_result_t res;

   res = memory_read_send( state, (unsigned int)u64BaseAddr + RTDEX_ACCESS_RXBADFRAMECNT, pu32Cnt );
   return res;
}


adp_result_t RTDExGetRxNbChannelsEth( connection_state * state, uint64_t u64BaseAddr, uint32_t* pu32NbChannels)
{
    uint64_t u64RegAddr;
    adp_result_t res;
    uint32_t uVal;

    u64RegAddr = u64BaseAddr + RTDEX_ACCESS_RX_CONFIG_INFO;
    if( (res = memory_read_send( state, (unsigned int)u64RegAddr, &uVal)))
       return res;
    *pu32NbChannels = uVal & RTDEX_RXCFGINF_NBCHNL_BM;
    return RTDEXOK;
}


adp_result_t RTDExGetTxNbChannelsEth( connection_state * state, uint64_t u64BaseAddr, uint32_t* pu32NbChannels)
{
   uint64_t u64RegAddr;
   adp_result_t res;
   uint32_t uVal;

   u64RegAddr = u64BaseAddr + RTDEX_ACCESS_TX_CONFIG_INFO;
   if( (res = memory_read_send(state, (unsigned int)u64RegAddr, &uVal)))
      return res;
   *pu32NbChannels = uVal & RTDEX_TXCFGINF_NBCHNL_BM;
   return RTDEXOK;
}


adp_result_t RTDExSetTxFrameGapValEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t u32FrameGap)
{
    adp_result_t res;

    res = memory_write_send( state, 
                       (unsigned int)u64RTDExFpgaBaseAddr + RTDEX_ACCESS_TXFRAMEGAP,
                       u32FrameGap );

    return res;
}


adp_result_t RTDExSetTxFrameBurstEth(  connection_state * state,
                                       uint64_t u64RTDExFpgaBaseAddr,
                                       uint32_t u32Burst  )
{
    adp_result_t res;

    res = memory_write_send( state, 
                       (unsigned int)u64RTDExFpgaBaseAddr + RTDEX_ACCESS_TXFRAMEBURST,
                       u32Burst );

    return res;
}


adp_result_t RTDExGetCoreIdVerEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t * puId, uint32_t * puVer)
{
   adp_result_t res;
   uint32_t uVal;

    if( (res = memory_read_send( state, (unsigned int)u64RTDExFpgaBaseAddr + RTDEX_ACCESS_COREIDVERS, &uVal)))
       return res;
    *puId = (uVal & RTDEX_COREIDVER_ID_BM) >> RTDEX_COREIDVER_ID_BO;
    *puVer = (uVal & RTDEX_COREIDVER_VER_BM) >> RTDEX_COREIDVER_VER_BO;
    return RTDEXOK;
}


adp_result_t RTDExGetRxConfigExtentedEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded)
{
    uint64_t u64RegAddr;
    adp_result_t res;
    uint32_t uVal;

    u64RegAddr = u64RTDExFpgaBaseAddr + RTDEX_ACCESS_RX_CONFIG_INFO;
    if( (res = memory_read_send( state, (unsigned int)u64RegAddr, &uVal)))
       return res;
    *puFifoDepth = ((uVal & RTDEX_RXCFGINF_FIFODEPTH_BM) >> RTDEX_RXCFGINF_FIFODEPTH_BO) * 1024;
    *puStatsIsIncluded = uVal & RTDEX_RXCFGINF_STATSENABLED_BM ? 1 : 0;
    return RTDEXOK;
}


adp_result_t RTDExGetTxConfigExtentedEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded)
{
    uint64_t u64RegAddr;
    adp_result_t res;
    uint32_t uVal;

    u64RegAddr = u64RTDExFpgaBaseAddr + RTDEX_ACCESS_TX_CONFIG_INFO;
    if( (res = memory_read_send( state, (unsigned int)u64RegAddr, &uVal)))
       return res;
    *puFifoDepth = ((uVal & RTDEX_TXCFGINF_FIFODEPTH_BM) >> RTDEX_TXCFGINF_FIFODEPTH_BO) * 1024;
    *puStatsIsIncluded = uVal & RTDEX_TXCFGINF_STATSENABLED_BM ? 1 : 0;
    return RTDEXOK;
}


adp_result_t RTDExResetCoreEth( connection_state * state, uint64_t u64RTDExFpgaBaseAddr)
{
    adp_result_t res;

    res = memory_write_send( state, (unsigned int)u64RTDExFpgaBaseAddr + RTDEX_ACCESS_CORERST, RTDEX_CORERST_BM);
    return res;
}


int RTDExEnableFlowControlEth( connection_state * state, uint64_t u64Address, unsigned uFifoFull, unsigned uFifoEmpty)
{
    uint64_t u64RegAddr;
   adp_result_t res;

	u64RegAddr = u64Address + RTDEX_ACCESS_FIFOFULL;
	if( (res = memory_write_send(state, (unsigned int)u64RegAddr, uFifoFull)))
	{
		return res;
	}

	u64RegAddr = u64Address + RTDEX_ACCESS_FIFOEMPTY;
	if( (res = memory_write_send( state, (unsigned int)u64RegAddr, uFifoEmpty)))
	{
		return res;
	}

	u64RegAddr = u64Address + RTDEX_ACCESS_RX_TIMEOUT_2DROPFRM;
	if( (res = memory_write_send( state, (unsigned int)u64RegAddr, 0xFFFFFFFF)))
	{
		return res;
	}

    return res;
}

