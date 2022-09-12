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
 * @file    rtdex_access.h
 * @brief   RTDEx (Ethernet version) registers definitions
 *
 * $Date: 2015/11/25 20:54:33 $
 * $Revision: 1.2 $
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


#ifndef ADP_RTDEX_ACCESS_H
#define ADP_RTDEX_ACCESS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
*****************************************************************************
* @file rtdex.h
*****************************************************************************
* RTDEx interface functions
****************************************************************************/
 
#define g_uNormalFrames 0x0600
#define g_uJumboFrames  0x8870
#define g_uSnapHeaders   0xaaaa
#define g_uLyrtechHeaders   0x0300d0cc
#define g_uSubId  0x10

// memory map of RTDEx Ethernet core, version 2.01:
//
#define RTDEX_ACCESS_COREIDVERS          0x00 // READ-ONLY
  #define RTDEX_COREIDVER_VER_BO 0
  #define RTDEX_COREIDVER_VER_BM (0xFFFF<<RTDEX_COREIDVER_VER_BO)
  #define RTDEX_COREVER 0x0201 // actual version
  #define RTDEX_COREIDVER_ID_BO 16
  #define RTDEX_COREIDVER_ID_BM (0xFFFF<<RTDEX_COREIDVER_ID_BO)
  #define RTDEX_COREID 0xDE88 // actual core id
#define RTDEX_ACCESS_CORERST             0x04
  #define RTDEX_CORERST_BM 0x01
#define RTDEX_ACCESS_RXTXFIFORST         0x08
  #define RTDEX_RXFIFORESET_BO 0
  #define RTDEX_RXFIFORESET_BM (0xFF<<RXFIFORESET_BO)
  #define RTDEX_TXFIFORESET_BO 16
  #define RTDEX_TXFIFORESET_BM (0xFF<<TXFIFORESET_BO)
#define RTDEX_ACCESS_PEERMACADDRLO       0x0C // first 4 bytes (left to right) of MAC addr (1 2 3 4)
#define RTDEX_ACCESS_PEERMACADDRHI       0x10 // last 2 bytes of MAC addr (x x 5 6) 
#define RTDEX_ACCESS_FPGAMACADDRLO       0x14
#define RTDEX_ACCESS_FPGAMACADDRHI       0x18
#define RTDEX_ACCESS_RX_TX_MODE          0X1C
  #define RTDEX_RXTXMODE_TX_CONTINUOUS_BM    (1<<0)
  #define RTDEX_RXTXMODE_RX_CONTINUOUS_BM    (1<<1)
#define RTDEX_ACCESS_RX_TIMEOUT_2DROPFRM 0x20
#define RTDEX_ACCESS_RX_CONFIG_INFO      0x24 // READ-ONLY
  #define RTDEX_RXCFGINF_NBCHNL_BM 0x0F
  #define RTDEX_RXCFGINF_FIFODEPTH_BO 12
  #define RTDEX_RXCFGINF_FIFODEPTH_BM (0xFFFFF<<RTDEX_RXCFGINF_FIFODEPTH_BO)
  #define RTDEX_RXCFGINF_STATSENABLED_BM (1<<10)
  #define RTDEX_RXCFGINF_PAUSEERCSENDCAP_BM (1<<11)
#define RTDEX_ACCESS_RXSTARTNEWTRANSFER  0x28
#define RTDEX_ACCESS_RXFRAMECNT_ARRAY    0x2C // 8 regs: 0x2C...0x4B
#define RTDEX_ACCESS_RX_ERR_STATUS       0x4C
  #define RTDEX_RXERRSTATUS_BAD_FRAME_BM 0x01
  #define RTDEX_RXERRSTATUS_LOST_FRAME_BM 0x02
  #define RTDEX_RXERRSTATUS_DROP_FRAME_BM 0x04
#define RTDEX_ACCESS_RXBADFRAMECNT       0x50
#define RTDEX_ACCESS_RXFRAMELOSTCNT_ARRAY 0x54 // 8 regs: 0x54...0x73
#define RTDEX_ACCESS_RX_DROP_FRAME_CNT_ARRAY 0x74 // 8 regs: 0x74...0x93
#define RTDEX_ACCESS_TXFRAMESIZE_ARRAY   0xA0 // 8 regs: 0xA0...0xBF
#define RTDEX_ACCESS_TXTRANSFERSIZE_ARRAY 0xC0 // 8 regs: 0xC0...0xDF
#define RTDEX_ACCESS_TXFRAMEGAP          0xE0
#define RTDEX_ACCESS_TXFRAMEBURST        0xE4
  #define RTDEX_TXFRMBURST_BM      (255<<0)
#define RTDEX_ACCESS_TX_CONFIG_INFO      0xE8 // READ-ONLY
  #define RTDEX_TXCFGINF_NBCHNL_BM 0x0F
  #define RTDEX_TXCFGINF_FIFODEPTH_BO 12
  #define RTDEX_TXCFGINF_FIFODEPTH_BM (0xFFFFF<<RTDEX_TXCFGINF_FIFODEPTH_BO)
  #define RTDEX_TXCFGINF_STATSENABLED_BM (10<<9)
#define RTDEX_ACCESS_TXSTARTNEWTRANSFER  0xEC
#define RTDEX_ACCESS_TXFRAMECNT_ARRAY    0xF0 // 8 regs: xF0...x10F
#define RTDEX_ACCESS_TXDATACNT_ARRAY     0x110 // 8 regs: x110...x12F
#define RTDEX_ACCESS_FIFOFULL    		 0x130 // 
#define RTDEX_ACCESS_FIFOEMPTY     		 0x134 // 


#ifdef __cplusplus
}
#endif


#endif
