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
 * @file    rtdex_eth_types.h
 * @brief   RTDEx type definitions for Ethernet
 *
 * $Date: 2015/08/10 17:10:40 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RTDEX
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

#ifndef ADP_RTDEX_ETH_TYPES_H
#define ADP_RTDEX_ETH_TYPES_H


#ifdef __cplusplus
extern "C" {
#endif


#define USE_JUMBO_FRAME // uncomment to enable jumbo-frame

/** @name RTDEx peer types
 * 
 *  Possible RTDEx peer types
 */
typedef enum {
   ePerseus,	///< Peer is another FPGA device
   eHost		///< Peer is a host PC
} ePeerEndpointType;

typedef enum {
   eHostToFpga, ///< from Host to FPGA
   eFpgaToHost, ///< from FPGA to Host
   eFpgaToFpga   ///< from FPGA to FPGA
} eConnectionType;




/** @name RTDEx Object structure
 * 
 *   RTDEx Object structure maintains all RTDEx variables
 */
typedef struct {
  unsigned uSrcChannelId;
  unsigned uDstChannelId;

//  eRTDExTransferDir eDir;
  eRTDExMode eMode; // eSingle, eContinuous
//  ePeerEndpointType ePeerType;
  eConnectionType eConnType; // eHostToDev, eDevToHost, eDevToDev

  char* pDataBuffer;
  int pStreamingPipe[2];
  uint32_t u32BufferSize;
  uint32_t u32PipeSize;

  adp_result_t resLastError;

  uint32_t u32FrameSize;
  uint32_t u32TransferSize;

  unsigned char bIsThreadCall;
  unsigned char bThreadIsDead;
  unsigned int uThreadFrameNbr;
  unsigned int uNbTransfers;
  
  void* pSourceInterface;
  void* pSourceInterfaceList;
  void* pInterfaceDmaHandle;
  unsigned char pSrcMacAddr[6];
  unsigned char pDstMacAddr[6];
  eProtocoleId eProtId;
  char bAllreadyAllocated;
  char bGetBroadCast;

  unsigned uPacketNumber; // TX packet #
  unsigned uExpectedRxFrameNumber;
  unsigned uFpga2HostFrameLostCount;
  unsigned uFpga2HostFrameCount;
  unsigned uHost2FpgaFrameCount;
} sEthernetRTDExStruct;

typedef sEthernetRTDExStruct * adp_handle_rtdex_ethernet_conn;

#ifdef __cplusplus
}
#endif

#endif



