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
 * @file      rtdex_pci.c
 * 
 * @brief     Functions for RTDEx access via PCIe.
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
#ifdef __linux__

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <stdint.h>

#include "eapi.h"
#include "bas_types.h"
#include "media_types.h"
#include "bas_error.h"
#include "rtdex_types.h"
#include "rtdex_media.h"
#include "rtdex_pcie.h"
#include "rtdex_pci.h"

#include "perseus2hostregs.h"
#include "rtdexpcie_drviface.h"


#ifdef __linux__
#define _SleepMs(n) usleep( n * 1000)
#endif


#ifdef RTDEX_DEBUG
#  define PDEBUG(fmt, args...) printf( "%s(): " fmt, __func__, ## args)
#  define PDEBUGA(fmt, args...) printf(fmt, ## args)
#else
#  define PDEBUG(fmt, args...)
#  define PDEBUGA(fmt, args...)
#endif


#define DEFAULT_SEND_TO_MS 1000 // ms

typedef struct {
   rtdexpcie_Handle ctrlHndl;
   unsigned uChnlNo;
   unsigned uChnlType; 
      #define TYPE_TO_HOST    0
      #define TYPE_FROM_HOST  1
      #define TYPE_DEV_TO_DEV 2
   union {
      struct {
         rtdexpcie_Handle dataHndl;
//         size_t doneTransferSize;
      } host;
      struct {
         int iDummy;
      } dev2dev;
   } u;
   unsigned uBaseDevNo;
   unsigned uMultiDone;
   unsigned uFrameSize;
   eRTDExMode eMode; // eSingle, eContinuous
   unsigned uLastError;
} AllocChannel;




// Send a command to Host driver via the Perseus Dual-Port RAM and wait answer.
/*
static int iDoPerseusCommand( RTDExPerseusParamsPcie * pPerseusParams, unsigned uCmd, unsigned uNbPrm, unsigned auPrm[ 3], unsigned * puRet)
{
   int res;
   unsigned u;
   unsigned uVal;
   uint64_t u64DpRam, u64StdRegs;

//printf( "mbx= %u, dpram= %u\n", pPerseusParams->uPerseusMailboxBaseAddr, pPerseusParams->uPerseusStdRegsBaseAddr);

   if( !pPerseusParams->state) {
      PDEBUG( "Invalid state (NULL)\n");
      return RTDEXINVALBUSACCESS;
   }

   u64DpRam = pPerseusParams->uMailboxBaseAddr;
   if( u64DpRam == 0)
      u64DpRam = DPRAM_START;
   u64StdRegs = pPerseusParams->uStdRegsBaseAddr;
   if( u64StdRegs == 0)
      u64StdRegs = STD_CTRL_REGS_START;

   {
   // check if DP-RAM is working:
   if( (res = memory_write_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P1, 0x17953)))
      return res;
   if( (res = memory_write_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P2, 0x83190)))
      return res;
   if( (res = memory_read_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P1, &uVal)))
      return res;
   if( uVal != 0x17953)
      return RTDEXPCIEPERSEUS2CPUFAIL;
   if( (res = memory_read_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P2, &uVal)))
      return res;
   if( uVal != 0x83190)
      return RTDEXPCIEPERSEUS2CPUFAIL;
   }

   if( uNbPrm > 3)
      uNbPrm = 3;
   for( u = 0; u < uNbPrm; u++)
      if( (res = memory_write_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P1 + u * 4, auPrm[ u])))
         return res;
   if( (res = memory_write_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_UB_CMD, uCmd)))
      return res;

   // trig interrupt to CPU:
   if( (res = memory_write_send( pPerseusParams->state, u64StdRegs + STD_CTRL_PCIE_CTRL_STAT, 1<<PCIECTRL_PCIEMSI_BN)))
      return res;
   if( (res = memory_write_send( pPerseusParams->state, u64StdRegs + STD_CTRL_PCIE_CTRL_STAT, 0)))
      return res;

   // wait for command done:
   for( u = 200; u > 0; u--) { // ???
      if( (res = memory_read_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_UB_CMD, &uVal)))
         return res;
      if( uVal == CMD_IDLE)
         break;
   }
   if( !u) {
      PDEBUG( "timeout waiting cmd done from Perseus!\n");
      return RTDEXPCIEPERSEUS2CPUFAIL;
   }
//   PDEBUG( "loop = %u\n", 200 -u);

   // get status:
   if( (res = memory_read_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_STATUS, &uVal)))
      return res;
   if( uVal != 0) {
      PDEBUG( "PerseusCommand: return status error= %u\n", uVal);
      return RTDEXPCIEPERSEUS2CPUFAIL;
   }

   // get return value:
   if( (res = memory_read_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_RET_DATA, &uVal)))
      return res;
   *puRet = uVal;
   return 0;
}
*/



/*
// Retreive rtdex devNo (instance) associate with this connection state
static int iGetDevNoFromBusAccess( RTDExPerseusParamsPcie * pPerseusParams, unsigned * puDevNo)
{
   int res;
   unsigned uVal;

   res = iDoPerseusCommand( pPerseusParams, CMD_GET_INSTANCE, 0, NULL, &uVal);
   if( res)
      return res;
   *puDevNo = uVal;
   return 0;
}
*/


// MSA-22apr2013
// Retreive rtdex devNo (instance) associate with this connection state
static int iGetDevNoFromBusAccess( RTDExPerseusParamsPcie * pPerseusParams, unsigned * puDevNo)
{
   int res;
   unsigned u;
   unsigned uVal;
   uint64_t u64DpRam, u64StdRegs;

//printf( "mbx= %u, dpram= %u\n", pPerseusParams->uPerseusMailboxBaseAddr, pPerseusParams->uPerseusStdRegsBaseAddr);

   if( !pPerseusParams->state) {
      PDEBUG( "Invalid state (NULL)\n");
      return RTDEXINVALBUSACCESS;
   }

   u64DpRam = pPerseusParams->uMailboxBaseAddr;
   if( u64DpRam == 0)
      u64DpRam = DPRAM_START;
   u64StdRegs = pPerseusParams->uStdRegsBaseAddr;
   if( u64StdRegs == 0)
      u64StdRegs = STD_CTRL_REGS_START;

   {
   // check if DP-RAM is working:
   if( (res = memory_write_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P1, 0x17953)))
      return res;
   if( (res = memory_write_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P2, 0x83190)))
      return res;
   if( (res = memory_read_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P1, &uVal)))
      return res;
   if( uVal != 0x17953)
      return RTDEXPCIEPERSEUS2CPUFAIL;
   if( (res = memory_read_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_P2, &uVal)))
      return res;
   if( uVal != 0x83190)
      return RTDEXPCIEPERSEUS2CPUFAIL;
   }

   if( (res = memory_read_send( pPerseusParams->state, u64DpRam + REG_OFFSET + REG32_INSTANCE, &uVal)))
      return res;
   *puDevNo = uVal;
   return 0;
}



// ==========================================================================================================
//
// ==========================================================================================================


adp_result_t RTDExInitParamsPcie( RTDExPerseusParamsPcie * pPerseusParams, connection_state * state)
{
   memset( pPerseusParams, 0, sizeof *pPerseusParams);
   pPerseusParams->state = state;
   return 0;
}



// Open a channel for "dev to host" or "host to dev" operation.
adp_result_t RTDExOpenHostAndFpgaPcie( adp_handle_rtdex_conn *hConn,
                                  RTDExPerseusParamsPcie * pPerseusParams,
                                  unsigned uFifoNo,
                                  eRTDExTransferDir eDir, // eToFpga or eFromFpga
                                  eRTDExMode eMode) // eSingle, eContinuous
{
   AllocChannel * pAC;
   int res;
   unsigned uDevNo;
   adp_handle_rtdex_conn hTempConn;

   res = iGetDevNoFromBusAccess( pPerseusParams, &uDevNo);
   if( res)
      return res;

   hTempConn = (adp_handle_rtdex_conn)(malloc( sizeof( sMediaRTDExStruct)));
   if( hTempConn == NULL) {
      PDEBUG( "No memory!\n");
		return RTDEXMEMALLOC;
   }
   memset( hTempConn, 0, sizeof * hTempConn);

   pAC = (AllocChannel*)malloc( sizeof *pAC);
   if( NULL == pAC) {
      PDEBUG( "No memory!\n");
      free( hTempConn);
		return RTDEXMEMALLOC;
   }
   memset( pAC, 0, sizeof *pAC);

   res = rtdexpcie_OpenCtrl( &pAC->ctrlHndl, uDevNo);
   if( res) {
      free( hTempConn);
      free( pAC);
      return res;
   }

   if( eDir == eFromFpga) {
      pAC->uChnlType = TYPE_TO_HOST;
      res = rtdexpcie_ConnectDevToHost( &pAC->uChnlNo, pAC->ctrlHndl, uFifoNo);
      if( !res)
         res = rtdexpcie_OpenData( &pAC->u.host.dataHndl, uDevNo, rtdexpcie_SELECT_TX_FIFO, uFifoNo);
      if( !res)
         res = rtdexpcie_iEnableTxFifo( pAC->ctrlHndl, 1<<pAC->uChnlNo, 0, 1);
   }
   else { // eToFpga
      pAC->uChnlType = TYPE_FROM_HOST;
      res = rtdexpcie_ConnectHostToDev( &pAC->uChnlNo, pAC->ctrlHndl, uFifoNo);
      if( !res)
         res = rtdexpcie_OpenData( &pAC->u.host.dataHndl, uDevNo, rtdexpcie_SELECT_RX_FIFO, uFifoNo);
      if( !res)
         res = rtdexpcie_iEnableRxFifo( pAC->ctrlHndl, 1<<pAC->uChnlNo, 1, 1);
   }
   if( res) {
      rtdexpcie_Close( pAC->ctrlHndl);
      free( hTempConn);
      free( pAC);
      return res;
   }

   pAC->eMode = eMode;
   pAC->uBaseDevNo = uDevNo;
   hTempConn->eMedia = eMediaPcie;
   hTempConn->hMediaRTDexConn = (adp_handle_rtdex_media_conn)pAC;
//   hTempConn->state = state;

   RTDExResetFifoPcie( hTempConn);

   *hConn = hTempConn;
   return 0;
}


// Open a channel for "dev to dev" operation.
adp_result_t RTDExOpenFpga2FpgaPcie( adp_handle_rtdex_conn *hConn,
                            RTDExPerseusParamsPcie * pSrcPerseusParams,
                            RTDExPerseusParamsPcie * pDstPerseusParams,
                            unsigned uSrcFifoNo,
                            unsigned uDstFifoNo,
                            eRTDExMode eMode) // eSingle, eContinuous
{
   AllocChannel * pAC;
   int res;
   unsigned uSrcDevNo, uDstDevNo;
   adp_handle_rtdex_conn hTempConn;

   res = iGetDevNoFromBusAccess( pSrcPerseusParams, &uSrcDevNo);
   if( res)
      return res;

   res = iGetDevNoFromBusAccess( pDstPerseusParams, &uDstDevNo);
   if( res)
      return res;

   hTempConn = (adp_handle_rtdex_conn)(malloc( sizeof( sMediaRTDExStruct)));
   if( hTempConn == NULL) {
      PDEBUG( "No memory!\n");
		return RTDEXMEMALLOC;
   }
   memset( hTempConn, 0, sizeof * hTempConn);

   pAC = (AllocChannel*)malloc( sizeof *pAC);
   if( NULL == pAC) {
      PDEBUG( "No memory!\n");
      free( hTempConn);
		return RTDEXMEMALLOC;
   }
   memset( pAC, 0, sizeof *pAC);

   res = rtdexpcie_OpenCtrl( &pAC->ctrlHndl, uSrcDevNo);
   if( res) {
      free( hTempConn);
      free( pAC);
      return res;
   }

   res = rtdexpcie_ConnectDevToDev( &pAC->uChnlNo, pAC->ctrlHndl, uSrcFifoNo, uDstFifoNo, uDstDevNo);
   if( !res)
      res = rtdexpcie_iEnableTxFifo( pAC->ctrlHndl, 1<<pAC->uChnlNo, 0, 1);
   if( !res)
      res = rtdexpcie_iEnableRxFifo( pAC->ctrlHndl, 1<<pAC->uChnlNo, 1, 1);
   if( res) {
      rtdexpcie_Close( pAC->ctrlHndl);
      free( hTempConn);
      free( pAC);
      return res;
   }
   pAC->uChnlType = TYPE_DEV_TO_DEV;
   pAC->eMode = eMode;
   pAC->uBaseDevNo = uSrcDevNo;
   hTempConn->eMedia = eMediaPcie;
   hTempConn->hMediaRTDexConn = (adp_handle_rtdex_media_conn)pAC;
//   hTempConn->hSrcMediaBusAccess = hSrcBusAccess;
//   hTempConn->hDstMediaBusAccess = hDstBusAccess;

   RTDExResetFifoPcie( hTempConn);

   *hConn = hTempConn;
   return 0;
}



// enable/disable user-logic side of fifos
// change enable/disable status of fifo for one device at time simultaneously, rx(adc)-fifo first, then tx(dac)-fifo
// adc --> wrEn/TX-FIFO --> wrEn/RX-FIFO/rdEn --> dac
adp_result_t RTDExEnableFifoMultiPcie( adp_handle_rtdex_conn hConnRtDex[], unsigned uNbConn, unsigned uEnRx, unsigned uEnTx)
{
   unsigned uRxChnlMsk, uTxChnlMsk;
   unsigned uDevNo;
   unsigned u, k;
   AllocChannel * pAC, * pHold;
   int res;

   res = 0;
   for( u = 0; u < uNbConn; u++) {
      if( !hConnRtDex[ u])
         continue;
      pAC = (AllocChannel*)(hConnRtDex[ u]->hMediaRTDexConn);
      pAC->uMultiDone = 0;
   }

   for( u = 0; u < uNbConn; u++) {
      if( !hConnRtDex[ u])
         continue;
      pAC = (AllocChannel*)(hConnRtDex[ u]->hMediaRTDexConn);
      if( pAC->uMultiDone)
         continue;
      uDevNo = pAC->uBaseDevNo;
      uRxChnlMsk = 0;
      uTxChnlMsk = 0;
      for( k = u; k < uNbConn; k++) {
         if( !hConnRtDex[ k])
            continue;
         pAC = (AllocChannel*)(hConnRtDex[ k]->hMediaRTDexConn);
         if( pAC->uMultiDone)
            continue;
         if( pAC->uBaseDevNo != uDevNo)
            continue;
         pHold = pAC;
         if( pAC->uChnlType == TYPE_FROM_HOST)
            uRxChnlMsk |= 1 << pAC->uChnlNo;
         else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
            uRxChnlMsk |= 1 << pAC->uChnlNo;
            uTxChnlMsk |= 1 << pAC->uChnlNo;
         }
         else if( pAC->uChnlType == TYPE_TO_HOST)
            uTxChnlMsk |= 1 << pAC->uChnlNo;
         pAC->uMultiDone = 1;
      }
      if( uRxChnlMsk)
         if( (res = rtdexpcie_iEnableRxFifo( pHold->ctrlHndl, uRxChnlMsk, /*wrEn*/ 1, /*rdEn*/ uEnRx)))
            break;
      if( uTxChnlMsk)
         if( (res = rtdexpcie_iEnableTxFifo( pHold->ctrlHndl, uTxChnlMsk, /*wrEn*/ uEnTx, /*rdEn*/ 1)))
            break;
   }
   return res;
}



adp_result_t RTDExClosePcie( adp_handle_rtdex_conn hConnRtDex)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   int res;
   int i;
   for (i = 0; i < 5; i++)
   {
		res = RTDExResetFifoPcie(hConnRtDex);
		usleep(50000);
		if (res)
		{
			return res;
		}
	}
	res = RTDExResetFifoPcie(hConnRtDex);
   res = rtdexpcie_Unconnect( pAC->ctrlHndl, pAC->uChnlNo);
   if( pAC->uChnlType == TYPE_TO_HOST || pAC->uChnlType == TYPE_FROM_HOST) {
      if( pAC->u.host.dataHndl) {
         rtdexpcie_Close( pAC->u.host.dataHndl);
         }
      }
   else /*if( pAC->uChnlType == TYPE_DEV_TO_DEV)*/ {
   }
   res = rtdexpcie_Close( pAC->ctrlHndl);
   free( pAC);
   return res;
}



adp_result_t RTDExResetFifoPcie( adp_handle_rtdex_conn hConnRtDex)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   int res;

   if( pAC->uChnlType == TYPE_FROM_HOST) {
      res = rtdexpcie_iResetRxFifo( pAC->ctrlHndl, pAC->uChnlNo);
   }
   else if( pAC->uChnlType == TYPE_TO_HOST) {
      res = rtdexpcie_iResetTxFifo( pAC->ctrlHndl, pAC->uChnlNo);
      if( res)
         return res;
//      res = rtdexpcie_iResetDm( pAC->ctrlHndl, pAC->uChnlNo);
   }
   else { // TYPE_DEV_TO_DEV
      // user-logic--->TX-FIFO--->devA--->PCIe--->devB--->RX-FIFO--->user-logic
      // user-logic<---RX-FIFO<---devA<---PCIe<---devB<---TX-FIFO<---user-logic
      //
      // src side:
      res = rtdexpcie_iResetTxFifo( pAC->ctrlHndl, pAC->uChnlNo);
      if( res)
         return res;
//      res = rtdexpcie_iResetDm( pAC->ctrlHndl, pAC->uChnlNo);
      if( res)
         return res;
      // dst side:
      res = rtdexpcie_iResetRxFifo( pAC->ctrlHndl, pAC->uChnlNo);
   }
   return res;
}


adp_result_t RTDExStartPcie( adp_handle_rtdex_conn hConnRtDex, uint32_t uFrameSize, size_t transferSize)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   int res;

   if( uFrameSize == 0)
      return RTDEXINVFRAMESIZE;
   if( transferSize == 0 && pAC->eMode == eSingle)
      return RTDEXINVFRAMESIZE;

   if( pAC->uChnlType == TYPE_FROM_HOST) {
//      uFrameSize = uNbFrames = 0;
//      pAC->u.host.doneTransferSize = 0;
   }

   else if( pAC->uChnlType == TYPE_TO_HOST) {
;//      pAC->u.host.doneTransferSize = 0;
   }

   else { // TYPE_DEV_TO_DEV
   }

   pAC->uFrameSize = uFrameSize;

   res = rtdexpcie_Start( pAC->ctrlHndl, pAC->uChnlNo, uFrameSize, transferSize);
   if(res) {
	return res; 
   }

   if( pAC->uChnlType == TYPE_FROM_HOST) {
//      uFrameSize = uNbFrames = 0;
//      pAC->u.host.doneTransferSize = 0;
   }

   else if( pAC->uChnlType == TYPE_TO_HOST) {
	res = rtdexpcie_iEnableTxFifo( pAC->ctrlHndl, 1<<pAC->uChnlNo, 1, 1);
   }

   
   return res;
}


adp_result_t RTDExStopPcie( adp_handle_rtdex_conn hConnRtDex)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   int res;

   if( pAC->uChnlType == TYPE_FROM_HOST) {
//      uFrameSize = uNbFrames = 0;
//      pAC->u.host.doneTransferSize = 0;
   }
   else if( pAC->uChnlType == TYPE_TO_HOST) 
   {
      res = rtdexpcie_iEnableTxFifo( pAC->ctrlHndl, 1<<pAC->uChnlNo, 0, 1);
      if(res) 
      {
	return res; 
      }
   }
   res = rtdexpcie_Stop( pAC->ctrlHndl, pAC->uChnlNo);
   pAC->uFrameSize = 0;
   return res;
}


int RTDExSendPcie( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, uint32_t u32MaxSize)
{
   int res;
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
//   uint32_t u32RemainingSize, u32BufferDataPos, u32CurrentFrameSize;
//   int i;

   pAC->uLastError = 0;
   if( pAC->uChnlType != TYPE_FROM_HOST) {
      PDEBUG( "Bad direction!\n");
      pAC->uLastError = RTDEXWRONGDIRECTION;
      return -1;
   }
   if( !pAC->uFrameSize) {
      pAC->uLastError = RTDEXINVFRAMESIZE;
      return -1;
   }

   res = rtdexpcie_Send( pAC->u.host.dataHndl, pDataBuffer, u32MaxSize, DEFAULT_SEND_TO_MS);
   if(FAILURE(res)) {
      pAC->uLastError = res;
      return -1;
   }
//   pAC->u.host.doneTransferSize += u32MaxSize;
   return res;

/*
   u32BufferDataPos = 0;
   u32RemainingSize = u32MaxSize;
   while( u32RemainingSize > 0) {
		if( u32RemainingSize <= pAC->uFrameSize) {
			u32CurrentFrameSize = u32RemainingSize;
		}
		else {
			u32CurrentFrameSize = pAC->uFrameSize;
		}
      for( i = SEND_TIMEOUT_MS; i > 0; i -= SEND_SLEEP_MS) {
         if( !res)
            break;
         _SleepMs( SEND_SLEEP_MS);
      }
      if( i <= 0)
         return u32MaxSize - u32RemainingSize;
		u32BufferDataPos += u32CurrentFrameSize;
		u32RemainingSize -= u32CurrentFrameSize;
      pAC->u.host.doneTransferSize += u32CurrentFrameSize;
   }
   return (int)u32MaxSize;
*/
}


// Return the number of bytes read if OK,
// else 0 if EOF,
// else -1 if any error. Call RTDExGetLastErrorPcie() to get the exact error code.
int RTDExReceivePcie( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, eRTDExTransferMode eMode, unsigned int uTimeoutMs)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
//   uint32_t u32RemainingSize;
//   uint32_t u32ReceivedSize;
   uint32_t uReadTransferSize;
//   uint32_t u32BufferDataPos;
   int res;

   pAC->uLastError = 0;
   if( pAC->uChnlType != TYPE_TO_HOST) {
//      PDEBUG( "Bad direction!\n");
      pAC->uLastError = RTDEXWRONGDIRECTION;
      return -1;
   }
   if( !pAC->uFrameSize) {
      pAC->uLastError = RTDEXINVFRAMESIZE;
      return -1;
   }

   res = rtdexpcie_Recv( pAC->u.host.dataHndl, pDataBuffer, u32MaxSize, (eMode == eRTDExWaitTimeout || eMode == eRTDExWaitTimeoutOrFirstError) ? uTimeoutMs : 0, &uReadTransferSize);
   if( res == RTDEXPCIEEOF)
      return 0; // 0 byte == EOF
   if( res) {
      pAC->uLastError = res;
      return -1;
   }
//   pAC->u.host.doneTransferSize += uReadTransferSize;
   return (int)uReadTransferSize;

/*      
   u32RemainingSize = u32MaxSize;
   u32BufferDataPos = 0;
   u32ReceivedSize = 0;
   while( u32RemainingSize) {
      res = rtdexpcie_Recv( pAC->u.host.dataHndl, pDataBuffer + u32BufferDataPos, u32RemainingSize, uTimeoutMs, &uReadTransferSize);
		if( res ) {
         if( res != rtdexpcie_EOF) {
            pAC->uLastError = RTDEXPCIEDRIVER;
            return -1;
         }
         if( eMode == eRTDExDontWait) {
            return u32ReceivedSize;
         }
         else if( eMode == eRTDExWaitTimeout) {
            if( uTimeoutMs <= RXTX_TIMEOUT_MS)
               return u32ReceivedSize;
            uTimeoutMs -= RXTX_TIMEOUT_MS;
            continue;
			}
         else {
	   	   continue;
         }
	 	}
      pAC->u.host.doneTransferSize += uReadTransferSize;
      if( u32RemainingSize > uReadTransferSize ) {
         u32RemainingSize -= uReadTransferSize;
		   u32ReceivedSize += uReadTransferSize;
      }
      else {
         uReadTransferSize = u32RemainingSize;
         u32RemainingSize = 0;
         u32ReceivedSize += uReadTransferSize;
      }
      u32BufferDataPos += uReadTransferSize;
   } // while

   return u32ReceivedSize;
*/
}


// Return the last error of RTDExReceivePcie() or RTDExSendPcie().
adp_result_t RTDExGetLastErrorPcie( adp_handle_rtdex_conn hConnRtDex)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);

   return pAC->uLastError;
}

void RTDExSetLastErrorPcie( adp_handle_rtdex_conn hConnRtDex, adp_result_t Result)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);

   pAC->uLastError = Result;
}



// Return the number of sent frame
adp_result_t RTDExGetTxFrameCountPcie( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   int res;
   size_t val;

   res = 0;
   if( pAC->uChnlType == TYPE_FROM_HOST)
      res = rtdexpcie_iGetHostXferedSize( pAC->ctrlHndl, pAC->uChnlNo, &val);
//      val = pAC->u.host.doneTransferSize;
   else /* TYPE_TO_HOST or TYPE_DEV_TO_DEV */
      res = rtdexpcie_iGetTxSize( pAC->ctrlHndl, pAC->uChnlNo, &val);

   if( !res && pAC->uFrameSize)
      *pVal = val / pAC->uFrameSize;
   return res;
}


// Return the number of sent bytes
adp_result_t RTDExGetTxByteCountPcie( adp_handle_rtdex_conn hConnRtDex, size_t *pVal)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   int res;
   size_t val;

   res = 0;
   if( pAC->uChnlType == TYPE_FROM_HOST)
      res = rtdexpcie_iGetHostXferedSize( pAC->ctrlHndl, pAC->uChnlNo, pVal);
//      val = pAC->u.host.doneTransferSize;
   else /* TYPE_TO_HOST or TYPE_DEV_TO_DEV */
      res = rtdexpcie_iGetTxSize( pAC->ctrlHndl, pAC->uChnlNo, pVal);
   return res;
}


// Return the number of recv frame from fpga
adp_result_t RTDExGetRxFrameCountPcie( adp_handle_rtdex_conn hConnRtDex, unsigned *pVal)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   size_t val;
   int res;

   res = RTDEXOK;
   if( pAC->uChnlType == TYPE_TO_HOST) {
      res = rtdexpcie_iGetHostXferedSize( pAC->ctrlHndl, pAC->uChnlNo, &val);
      *pVal = 0;
      if( !res && pAC->uFrameSize)
         *pVal = (unsigned)(val / pAC->uFrameSize);
   }
   else { /* TYPE_FROM_HOST or TYPE_DEV_TO_DEV */
      res = rtdexpcie_iGetRxSize( pAC->ctrlHndl, pAC->uChnlNo, &val);
      if( !res)
         if( pAC->uFrameSize)
            *pVal = (unsigned)(val / pAC->uFrameSize);
   }
   return res;
}


adp_result_t RTDExGetTxFifoStatusPcie( adp_handle_rtdex_conn hConnRtDex, unsigned * puOverflow)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   int res;
   unsigned uUnder;

   res = RTDEXOK;
   if( pAC->uChnlType == TYPE_FROM_HOST) {
      res = RTDEXWRONGDIRECTION;
   }
   else { /* TYPE_TO_HOST or TYPE_DEV_TO_DEV */
      res = rtdexpcie_iGetTxFifoStatus( pAC->ctrlHndl, pAC->uChnlNo, puOverflow, &uUnder);
   }
   return res;
}




adp_result_t RTDExGetRxFifoStatusPcie( adp_handle_rtdex_conn hConnRtDex, unsigned * puUnderflow)
{
   AllocChannel * pAC = (AllocChannel*)(hConnRtDex->hMediaRTDexConn);
   int res;
   unsigned uOver;

   res = RTDEXOK;
   if( pAC->uChnlType == TYPE_TO_HOST) {
      res = RTDEXWRONGDIRECTION;
   }
   else { /* TYPE_FROM_HOST or TYPE_DEV_TO_DEV */
      res = rtdexpcie_iGetRxFifoStatus( pAC->ctrlHndl, pAC->uChnlNo, &uOver, puUnderflow);
   }
   return res;
}





//
// ===============================================================================================================
// Per core functions:
//

typedef struct {
   unsigned uNbRxFifo, uNbTxFifo;
   unsigned uRxFifoSize, uTxFifoSize;
   unsigned uCoreId, uCoreVer;
   unsigned uDriverVerMaj, uDriverVerMin;
} sRtdexConfig;

static int iGetRtdexConfig( RTDExPerseusParamsPcie * pPerseusParams, sRtdexConfig * pCfg)
{
   unsigned uDevNo;
   int res;
   rtdexpcie_Handle hndl;

   res = iGetDevNoFromBusAccess( pPerseusParams, &uDevNo);
   if( res)
      return res;
   res = rtdexpcie_OpenCtrl( &hndl, uDevNo);
   if( res)
      return res;

   res = rtdexpcie_iGetFifoConfig( hndl, &pCfg->uNbRxFifo, &pCfg->uRxFifoSize, &pCfg->uNbTxFifo, &pCfg->uTxFifoSize);
   if( !res)
      res = rtdexpcie_iGetCoreIdVers( hndl, &pCfg->uCoreId, &pCfg->uCoreVer);
   if( !res)
      res = rtdexpcie_iGetDriverVers( hndl, &pCfg->uDriverVerMaj, &pCfg->uDriverVerMin);

   /*res= */rtdexpcie_Close( hndl);
   return res;
}


adp_result_t RTDExGetRxNbChannelsPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t* pu32NbChannels)
{
   int res;
   sRtdexConfig cfg;

   res = iGetRtdexConfig( pPerseusParams, &cfg);
   if( !res)
      *pu32NbChannels = cfg.uNbRxFifo;
   return res;
}


adp_result_t RTDExGetTxNbChannelsPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t* pu32NbChannels)
{
   int res;
   sRtdexConfig cfg;

   res = iGetRtdexConfig( pPerseusParams, &cfg);
   if( !res)
      *pu32NbChannels = cfg.uNbTxFifo;
   return res;
}


adp_result_t RTDExGetRxConfigExtentedPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded)
{
   int res;
   sRtdexConfig cfg;

   res = iGetRtdexConfig( pPerseusParams, &cfg);
   if( !res) {
      *puFifoDepth = cfg.uRxFifoSize;
      *puStatsIsIncluded = 1;
   }
   return res;
}


adp_result_t RTDExGetTxConfigExtentedPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puFifoDepth, uint32_t * puStatsIsIncluded)
{
   int res;
   sRtdexConfig cfg;

   res = iGetRtdexConfig( pPerseusParams, &cfg);
   if( !res) {
      *puFifoDepth = cfg.uTxFifoSize;
      *puStatsIsIncluded = 1;
   }
   return res;
}


adp_result_t RTDExGetCoreIdVerPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puId, uint32_t * puVer)
{
   int res;
   sRtdexConfig cfg;

   res = iGetRtdexConfig( pPerseusParams, &cfg);
   if( !res) {
      *puId = cfg.uCoreId;
      *puVer = cfg.uCoreVer;
   }
   return res;
}


adp_result_t RTDExGetDriverVerPcie( RTDExPerseusParamsPcie * pPerseusParams, uint32_t * puMaj, uint32_t * puMin)
{
   int res;
   sRtdexConfig cfg;

   res = iGetRtdexConfig( pPerseusParams, &cfg);
   if( !res) {
      *puMaj = cfg.uDriverVerMaj;
      *puMin = cfg.uDriverVerMin;
   }
   return res;
}



adp_result_t RTDExResetCorePcie( RTDExPerseusParamsPcie * pPerseusParams)
{
   unsigned uDevNo;
   int res;
   rtdexpcie_Handle hndl;

   res = iGetDevNoFromBusAccess( pPerseusParams, &uDevNo);
   if( res)
      return res;
   res = rtdexpcie_OpenCtrl( &hndl, uDevNo);
   if( res)
      return res;
   res = rtdexpcie_iResetCore( hndl);
   /*res= */rtdexpcie_Close( hndl);
   return res;
   }


adp_result_t RTDExGetDriverInstancePcie( RTDExPerseusParamsPcie * pPerseusParams, unsigned * puInst)
{
   unsigned uDevNo;
   int res;

   res = iGetDevNoFromBusAccess( pPerseusParams, &uDevNo);
   if( res)
      return res;
   *puInst = uDevNo;
   return 0;
   }


#endif // __linux__
