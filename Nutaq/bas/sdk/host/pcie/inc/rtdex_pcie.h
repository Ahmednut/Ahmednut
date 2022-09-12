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
 * @file    rtdex_pcie.h
 * @brief   Low level Interface to Perseus RTDEX PCIe.
 * 
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup PCIE
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

#ifndef RTDEX_PCIE_H
#define RTDEX_PCIE_H

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __linux__

typedef void * rtdexpcie_Handle;


int rtdexpcie_OpenCtrl( rtdexpcie_Handle * pCtrlHandle, unsigned uRtdexNum);
int rtdexpcie_iGetDriverVers( rtdexpcie_Handle ctrlHandle, unsigned * puMaj, unsigned * puMin);
int rtdexpcie_iResetCore( rtdexpcie_Handle ctrlHandle);
int rtdexpcie_iGetCoreIdVers( rtdexpcie_Handle ctrlHandle, unsigned * puId, unsigned * puVer);
int rtdexpcie_iGetFifoConfig( rtdexpcie_Handle ctrlHandle, unsigned * puNbRxFifo, unsigned * puRxFifoSize, unsigned * puNbTxFifo, unsigned * puTxFifoSize);
int rtdexpcie_IoControl( rtdexpcie_Handle ctrlHandle, unsigned char * pucSendData, unsigned uSendLen, unsigned char * pucRxData, unsigned uMaxRxSize, unsigned * puRxLen);
int rtdexpcie_WriteReg( rtdexpcie_Handle ctrlHandle, unsigned uAddr, unsigned uVal);
int rtdexpcie_ReadModifyWriteReg( rtdexpcie_Handle ctrlHandle, unsigned uAddr, unsigned uAnd, unsigned uOr);
int rtdexpcie_ReadReg( rtdexpcie_Handle ctrlHandle, unsigned uAddr, unsigned * puVal);

int rtdexpcie_OpenData( rtdexpcie_Handle * pHandle, unsigned uRtdexNum, unsigned uSelectFifo, unsigned uFifoNo);
  #define rtdexpcie_SELECT_RX_FIFO 1
  #define rtdexpcie_SELECT_TX_FIFO 0
int rtdexpcie_Close( rtdexpcie_Handle handle);
int rtdexpcie_ConnectDevToHost( unsigned * puChnlNo, rtdexpcie_Handle ctrlHandle, unsigned uFifoNo);
int rtdexpcie_ConnectHostToDev( unsigned * puChnlNo, rtdexpcie_Handle ctrlHandle, unsigned uFifoNo);
int rtdexpcie_ConnectDevToDev( unsigned * puChnlNo, rtdexpcie_Handle ctrlHandle, unsigned uSrcFifoNo, unsigned uDstFifoNo, unsigned uDstDevNo);
int rtdexpcie_Unconnect( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo);
int rtdexpcie_Start( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo, unsigned uFrameSize, size_t transferSize);
int rtdexpcie_Stop( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo);
int rtdexpcie_Send( rtdexpcie_Handle handle, unsigned char * pucData, unsigned uDataLen, unsigned uTimeoutMs);
int rtdexpcie_Recv( rtdexpcie_Handle handle, unsigned char * pucBuff, unsigned uBuffSize, unsigned uTimeoutMs, unsigned * puRxLen);
int rtdexpcie_iGetChannelStatus( rtdexpcie_Handle handle, unsigned uChnlNo, unsigned * pbEnded, unsigned * pbSucces);
int rtdexpcie_iGetRxSize( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo, size_t * pRxSize);
int rtdexpcie_iGetTxSize( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo, size_t * pTxSize);
int rtdexpcie_iGetHostXferedSize( rtdexpcie_Handle handle, unsigned uChnlNo, size_t * pXferedSize);
int rtdexpcie_iEnableRxFifo( rtdexpcie_Handle ctrlHandle, unsigned uChnlMsk, unsigned uEnableWr, unsigned uEnableRd);
int rtdexpcie_iEnableTxFifo( rtdexpcie_Handle ctrlHandle, unsigned uChnlMsk, unsigned uEnableWr, unsigned uEnableRd);
int rtdexpcie_iResetRxFifo( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo);
int rtdexpcie_iResetTxFifo( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo);
int rtdexpcie_iGetRxFifoStatus( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo, unsigned * pbOverflow, unsigned * pbUnderflow);
int rtdexpcie_iGetTxFifoStatus( rtdexpcie_Handle ctrlHandle, unsigned uChnlNo, unsigned * pbOverflow, unsigned * pbUnderflow);

#endif // __linux__

#ifdef __cplusplus
}
#endif

#endif // RTDEX_PCIE_H
