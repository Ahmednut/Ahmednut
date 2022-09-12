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
 * @file      rtdex_param_eth.c
 * 
 * @brief     Functions that create and destroy the ethernet object for RTDEx transfer.
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
#include "pcap.h"
#include "bas_types.h"
#include "adp_modules_handles.h"
#include "adp_media_modules_handles.h"
#include "media_types.h"
#include "adp_handles.h"
#include "rtdex_types.h"
#include "adp_media_eth.h"
#include "adp_plb_access_types.h"
#include "adp_plb_access_modules_handles.h"
#include "adp_plb_access_media.h"
#include "rtdex_modules_handles.h"
#include "rtdex_eth.h"
#include "rtdex_param_eth.h"
#include "rtdex_media.h"
#include "memory.h"



adp_handle_rtdex_media SetRTDExConnParamEth( adp_handle hBoard,
                                             char bGetBroadCast,
                                             char pHostMacAddr[6],
                                             char pDeviceMacAddr[6],
                                             eProtocoleId eProtId )
{
   sMediaEthStruct* pMediaEth;
   sMediaRTDExStruct* pMediaRTDex;

   adp_handle_rtdex_media hMediaRtdex;
   adp_handle hMediaEth;

   hMediaEth = adp_OpenHandle( ADP_MEDIA_ETH_ID, sizeof( sMediaEthStruct ) );

   if( hMediaEth == ADP_INVALID_HANDLE )
   {
       return ADP_INVALID_HANDLE;
   }

   pMediaEth = (sMediaEthStruct* ) adp_GetHandleRef( hMediaEth );

   if( pMediaEth )
   {
       pMediaEth -> pSourceInterface = 0;
       memcpy( pMediaEth -> pHostMacAddr, pHostMacAddr, 6 );
       memcpy( pMediaEth -> pDeviceMacAddr, pDeviceMacAddr, 6 );
       pMediaEth -> eProtId = eProtId;
       pMediaEth -> bAllreadyAllocated = 0;
       pMediaEth -> bGetBroadCast = bGetBroadCast;
   }

   else
   {
       adp_CloseHandle( hMediaEth );
       return ADP_INVALID_HANDLE;
   }

   hMediaRtdex = adp_OpenHandle( ADP_RTDEX_MEDIA_ID, sizeof( sMediaRTDExStruct ) );

   if( hMediaRtdex == ADP_INVALID_HANDLE )
   {
       adp_CloseHandle( hMediaEth );
       return ADP_INVALID_HANDLE;
   }

   pMediaRTDex = (sMediaRTDExStruct* ) adp_GetHandleRef( hMediaRtdex );

   if( pMediaRTDex )
   {
       pMediaRTDex -> eMedia = eMediaEth;
       pMediaRTDex -> hMediaObj = hMediaEth;
       pMediaRTDex -> hBoard = hBoard;

       pMediaRTDex -> fctRTDExCreateConn = RTDExCreateConnectionEth;
       pMediaRTDex -> fctRTDExDestroyConn = RTDExDestroyConnectionEth;
       pMediaRTDex -> fctRTDExOpen = RTDExOpenEth;
       pMediaRTDex -> fctRTDExStart = RTDExStartEth;
       pMediaRTDex -> fctRTDExFlush = RTDExFlushEth;
       pMediaRTDex -> fctRTDExClose = RTDExCloseEth;
       pMediaRTDex -> fctRTDExGetLastError = RTDExGetLastErrorEth;
       pMediaRTDex -> fctRTDExWait = RTDExWaitEth;
       pMediaRTDex -> fctRTDExGetTransferPos = RTDExGetTransferPosEth;
       pMediaRTDex -> fctRTDExGetTransferFrameNumber = RTDExGetTransferFrameNumberEth;
       pMediaRTDex -> fctRTDExResetFifo = RTDExResetFifoEth;
       pMediaRTDex -> fctRTDExResetCore = RTDExResetCoreEth;
   }

   else
   {
       adp_CloseHandle( hMediaEth );
       return ADP_INVALID_HANDLE;
   }


   return hMediaRtdex;
}


adp_result_t FreeRTDExConnParamEth( adp_handle_rtdex_media hRtdexMedia )
{

   sMediaRTDExStruct* pMediaRTDex;
   unsigned short uObjType;

   uObjType = adp_GetHandleObjectTypeId( hRtdexMedia );

   if( uObjType != ADP_RTDEX_MEDIA_ID )
   {
     return -1;
   }

   pMediaRTDex = (sMediaRTDExStruct* ) adp_GetHandleRef( hRtdexMedia );

   if( pMediaRTDex )
   {
      adp_CloseHandle( pMediaRTDex -> hMediaObj );
   }

   return 0;
}
