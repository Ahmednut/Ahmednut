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
 * @file      rtdex.c
 * 
 * @brief     Functions definition for RTDEx access.
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
#include "bas_types.h"
#include "media_types.h"
#include "rtdex_types.h"

#ifdef ZEDBOARD_EMBEDDED_HOST

#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_tcpip.h"
#include "rtdex_axi.h"
#include "rtdex_eth_types.h"
#include "rtdex_eth.h"

#else
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_eth.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_pci.h"
#include "rtdex_tcpip.h"
#endif

#if defined(_WIN32) || defined(_WIN64)
#include "windows.h"
#endif

#define MAX_RTDEX_RXTX_SIZE (((uint32_t)-1)>>1) // max # of bytes we can send ou receive each time

adp_result_t RTDExClose( adp_handle_rtdex_conn hConnRtDex )
{
	adp_result_t res;

   if( hConnRtDex->eMedia == eMediaSocketIp) {
		res = RTDExCloseEth( hConnRtDex );
		return res;
   }   
#ifndef ZEDBOARD_EMBEDDED_HOST
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
		res = RTDExClosePcie( hConnRtDex );
		return res;
   }   
#endif //__linux__
#else //ZEDBOARD_EMBEDDED_HOST
   if ( hConnRtDex->eMedia == eMediaAxi) {
		res = RTDExCloseAxi( hConnRtDex );
		return res;
   }
#endif //ZEDBOARD_EMBEDDED_HOST
   if ( hConnRtDex->eMedia == eMediaTcpIp) {
		res = RTDExCloseTcpIp( hConnRtDex );
		return res;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}

adp_result_t RTDExStart( adp_handle_rtdex_conn hConnRtDex, unsigned uFrameSize, size_t transferSize)
{
   adp_result_t res;

   if( hConnRtDex->eMedia == eMediaSocketIp) {
      if( transferSize > (uint32_t)-1) // ethernet RTDEX transfer size is 32-bit wide
         return RTDEXTRANSFERTSIZETOOBIG;
		res = RTDExStartEth( hConnRtDex, uFrameSize, (uint32_t)transferSize);
		return res;
   }   
#ifndef ZEDBOARD_EMBEDDED_HOST
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
      res = RTDExStartPcie( hConnRtDex, uFrameSize, transferSize);
		return res;
   }   
#endif //__linux__
#else //ZEDBOARD_EMBEDDED_HOST
   if ( hConnRtDex->eMedia == eMediaAxi) {
		res = RTDExStartAxi( hConnRtDex, uFrameSize, (uint32_t)transferSize);
		return res;
   }
#endif //ZEDBOARD_EMBEDDED_HOST
   if ( hConnRtDex->eMedia == eMediaTcpIp) {
		res = RTDExStartTcpIp( hConnRtDex, uFrameSize, (uint32_t)transferSize);
		return res;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}


int RTDExReceive( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, 
				      eRTDExTransferMode eMode, unsigned int uTimeoutMs)
{
   adp_result_t res;
   
   if(u32MaxSize > MAX_RTDEX_RXTX_SIZE)
   {
        // Set last error parameter value
        if( hConnRtDex->eMedia == eMediaSocketIp) {
            RTDExSetLastErrorEth( hConnRtDex, RTDEXRECEIVESENDSIZEISTOLARGE);
        }
#ifdef __linux__
        else if( hConnRtDex->eMedia == eMediaPcie) {
            RTDExSetLastErrorPcie( hConnRtDex, RTDEXRECEIVESENDSIZEISTOLARGE);
        }
#endif //__linux__
        
        return RTDEXRECEIVESENDSIZEISTOLARGE;
   }

   if(hConnRtDex->eMedia == eMediaSocketIp) {
		res = RTDExReceiveEth( hConnRtDex, pDataBuffer, u32MaxSize, eMode, uTimeoutMs);
		return res;
   }   
#ifndef ZEDBOARD_EMBEDDED_HOST
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
      res = RTDExReceivePcie( hConnRtDex, pDataBuffer, u32MaxSize, eMode, uTimeoutMs);
		return res;
   }
#endif //__linux__
#else //ZEDBOARD_EMBEDDED_HOST
   else if( hConnRtDex->eMedia == eMediaAxi) {
      res = RTDExReceiveAxi( hConnRtDex, pDataBuffer, u32MaxSize, eMode, uTimeoutMs);
		return res;
   }
#endif //ZEDBOARD_EMBEDDED_HOST
   if(hConnRtDex->eMedia == eMediaTcpIp) {
		res = RTDExReceiveTcpIp( hConnRtDex, pDataBuffer, u32MaxSize, eMode, uTimeoutMs);
		return res;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}


int RTDExSend( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize)
{
   adp_result_t res;
   
   if(u32MaxSize > MAX_RTDEX_RXTX_SIZE)
   {
        // Set last error parameter value
        if( hConnRtDex->eMedia == eMediaSocketIp) {
            RTDExSetLastErrorEth( hConnRtDex, RTDEXRECEIVESENDSIZEISTOLARGE);
        }
#ifdef __linux__
        else if( hConnRtDex->eMedia == eMediaPcie) {
            RTDExSetLastErrorPcie( hConnRtDex, RTDEXRECEIVESENDSIZEISTOLARGE);
        }
#endif //__linux__
        
        return RTDEXRECEIVESENDSIZEISTOLARGE;
   }

   if( hConnRtDex->eMedia == eMediaSocketIp) {
		res = RTDExSendEth( hConnRtDex, pDataBuffer, u32MaxSize);
		return res;
   }   
#ifndef ZEDBOARD_EMBEDDED_HOST
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
      res = RTDExSendPcie( hConnRtDex, pDataBuffer, u32MaxSize);
      return res;
   }
#endif //__linux__
#else //ZEDBOARD_EMBEDDED_HOST
   else if( hConnRtDex->eMedia == eMediaAxi) {
      res = RTDExSendAxi( hConnRtDex, pDataBuffer, u32MaxSize);
      return res;
   }
#endif //ZEDBOARD_EMBEDDED_HOST
   if( hConnRtDex->eMedia == eMediaTcpIp) {
		res = RTDExSendTcpIp( hConnRtDex, pDataBuffer, u32MaxSize);
		return res;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}



adp_result_t RTDExGetLastError( adp_handle_rtdex_conn hConnRtDex)
{

   adp_result_t res;
   if( hConnRtDex->eMedia == eMediaSocketIp) {
      res = RTDExGetLastErrorEth( hConnRtDex);
		return res;
   }
#ifndef ZEDBOARD_EMBEDDED_HOST
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
      res = RTDExGetLastErrorPcie( hConnRtDex);
		return res;
   }
#endif
#endif
   if( hConnRtDex->eMedia == eMediaTcpIp) {
	   return RTDEXMEDIANOTSUPPORTED;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}



adp_result_t RTDExResetFifo( adp_handle_rtdex_conn hConnRtDex)
{
#ifndef ZEDBOARD_EMBEDDED_HOST
   adp_result_t res;
   if( hConnRtDex->eMedia == eMediaSocketIp) {
		res = RTDExResetFifoEth( hConnRtDex);
		return res;
   }
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
      res = RTDExResetFifoPcie( hConnRtDex);
		return res;
   }
#endif
#endif
   if( hConnRtDex->eMedia == eMediaTcpIp) {
	   return RTDEXMEDIANOTSUPPORTED;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}



adp_result_t RTDExGetTxFramesCnt( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32Cnt)
{


#ifndef ZEDBOARD_EMBEDDED_HOST
   adp_result_t res;
   *pu32Cnt = 0;
   if( hConnRtDex->eMedia == eMediaSocketIp) {
      res = RTDExGetTxFramesCountEth( hConnRtDex, pu32Cnt);
      return res;
   }
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
      res = RTDExGetTxFrameCountPcie( hConnRtDex, pu32Cnt);
		return res;
   }
#endif
#endif
   if( hConnRtDex->eMedia == eMediaTcpIp) {
	   return RTDEXMEDIANOTSUPPORTED;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}



adp_result_t RTDExGetRxFramesCnt( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32Cnt )
{
#ifndef ZEDBOARD_EMBEDDED_HOST
   adp_result_t res;
   *pu32Cnt = 0;
   if( hConnRtDex->eMedia == eMediaSocketIp) {
      res = RTDExGetRxFramesCountEth( hConnRtDex, pu32Cnt);
      return res;
   }
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
      res = RTDExGetRxFrameCountPcie( hConnRtDex, pu32Cnt);
		return res;
   }
#endif
#endif
   if( hConnRtDex->eMedia == eMediaTcpIp) {
	   return RTDEXMEDIANOTSUPPORTED;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}



adp_result_t RTDExGetTxFifoDataCnt( adp_handle_rtdex_conn hConnRtDex, uint32_t* pu32DataCount)
{
#ifndef ZEDBOARD_EMBEDDED_HOST
   adp_result_t res;
   if( hConnRtDex->eMedia == eMediaSocketIp) {
      res = RTDExGetTxFifoDataCntEth( hConnRtDex, pu32DataCount);
      return res;
   }
#ifdef __linux__
   else if( hConnRtDex->eMedia == eMediaPcie) {
      *pu32DataCount = 0; //???
		return RTDEXOK;
   }
#endif
#endif
   if( hConnRtDex->eMedia == eMediaTcpIp) {
	   return RTDEXMEDIANOTSUPPORTED;
   }
   else {
		return RTDEXMEDIANOTSUPPORTED;
   }
}
