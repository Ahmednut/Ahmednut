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
 * @file    rtdexpcie_drviface.h
 * @brief   Shared interface between user-space & kernel-space driver
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


#ifndef RTDEXPCIE_DRVIFACE_H
#define RTDEXPCIE_DRVIFACE_H

#ifdef __cplusplus
extern "C" {
#endif

// this is the interface for the driver version:
#define INTERFACE_DRIVER_VERSION_MAJ 1
#define INTERFACE_DRIVER_VERSION_MIN 0


// control mailbox (network) device name:
#define MAILBOX_IFACE_NAME "mailbox"


// rtdex (network) device names:
#define PCIE_PROC_MEM_RW_NAME "pcieMemRW" // debug..
#define PCIE_PROC "perseus%d"

#define RXFIFO_NAME_STR "perseus%d/pcieRtdex/rxFifo%d"
#define TXFIFO_NAME_STR "perseus%d/pcieRtdex/txFifo%d"
#define GLOBAL_NAME_STR "perseus%d/pcieRtdex/global"


// host-data write path:   PCIe(dma) [WrEn]--> RX-FIFO [RdEn]--> userLogic
// host-data read path:    userLogic [WrEn]--> TX-FIFO --> (dma)PCIe

// rtdex io-controls:
// any error is reported in first position of response ==> (errorNo) when user space errno == EINVAL
// misc:
#define IOCTL_GET_DRIVER_VERSION 0x00 // (cmd) ==> (maj, min)
#define IOCTL_GET_CORE_IDVERS    0x01 // (cmd) ==> (id, ver)
#define IOCTL_RESET_CORE         0x02 // (cmd)
#define IOCTL_GET_FIFO_CFG       0x03 // (cmd) ==> (nbRx, nbTx, rxSize, txSize)
#define IOCTL_GET_FREE_FIFO_MASK 0x04 // (cmd) ==> (rxFree, txFree)
#define IOCTL_SET_HOST2DEV_FLUSH_TRESHOLD 0x05 // (cmd, sizeK)
#define IOCTL_FORCE_HOST2DEV_FLUSH 0x06 // (cmd)


// channels setup & status:
#define IOCTL_CONNECT_DEV2DEV    0x10 // (cmd, srcFifoNo, dstFifoNo, dstDevNo)  ==> (chnlNo)
#define IOCTL_CONNECT_DEV2HOST   0x11 // (cmd, uFifoNo) ==> (chnlNo)
#define IOCTL_CONNECT_HOST2DEV   0x12 // (cmd, uFifoNo) ==> (chnlNo)
#define IOCTL_UNCONNECT          0x13 // (cmd, chnlNo)
#define IOCTL_START              0x14 // (cmd, chnlNo, frameSize, transferSizeL [,transferSizeH])
#define IOCTL_STOP               0x15 // (cmd, chnlNo)
#define IOCTL_GET_STATUS         0x16 // (cmd, chnlNo) ==> (bEnded, bSuccess)
#define IOCTL_GET_RX_TFR_SIZE    0x17 // (cmd, chnlNo) ==> (sizeL [,sizeH])
#define IOCTL_GET_TX_TFR_SIZE    0x18 // (cmd, chnlNo) ==> (sizeL [,sizeH])
#define IOCTL_GET_HOST_TFR_SIZE  0x19 // (cmd, chnlNo) ==> (sizeL [,sizeH])

// channel misc control:
//#define IOCTL_RESET_DM           0x20 // (cmd, chnlNo)
#define IOCTL_RESET_RX_FIFO      0x21 // (cmd, chnlNo)
#define IOCTL_RESET_TX_FIFO      0x22 // (cmd, chnlNo)
#define IOCTL_GET_RX_FIFO_STATUS 0x23 // (cmd, chnlNo) ==> (overflow, underflow)
#define IOCTL_GET_TX_FIFO_STATUS 0x24 // (cmd, chnlNo) ==> (overflow, underflow)
#define IOCTL_ENABLE_RX_FIFO     0x25 // (cmd, chnlMsk, wr, rd)
#define IOCTL_ENABLE_TX_FIFO     0x26 // (cmd, chnlMsk, wr, rd)

// special test modes:
#define IOCTL_ROUND_TRIP_TEST    0x40 // (cmd, uEnable, uDmNo)


// rtdex registers access --- DEBUG ONLY --- :
#define IOCTL_WRITE_REG          0x50 // (cmd, addr32)
#define IOCTL_READ_REG           0x51 // (cmd, addr32) ==> (value)
#define IOCTL_RMW_REG            0x52 // (cmd, addr32, andVal, orVal)


#define MAX_USER_FRAME_SIZE (1*1024*1024)
#define WORD_SIZE_B 8 // frame size must be multiple of this value

#define DEFAULT_CTLIO_NO 0x12AB


// error codes (returned by ioctl):
#define rtdxpkdrv_NO_ERROR             0
#define rtdxpkdrv_FIRST_ERROR          0
#define rtdxpkdrv_FIFO_NOT_CONNECTED   (rtdxpkdrv_FIRST_ERROR +0x00)
#define rtdxpkdrv_BAD_FIFO_NO          (rtdxpkdrv_FIRST_ERROR +0x01)
#define rtdxpkdrv_NO_FREE_AXI2BAR      (rtdxpkdrv_FIRST_ERROR +0x02)
#define rtdxpkdrv_BAD_CHNL_NO          (rtdxpkdrv_FIRST_ERROR +0x03)
#define rtdxpkdrv_DMA_ALLOC_FAIL       (rtdxpkdrv_FIRST_ERROR +0x04)
#define rtdxpkdrv_NO_FREE_CHANNEL      (rtdxpkdrv_FIRST_ERROR +0x05)
#define rtdxpkdrv_FIFO_BUSY            (rtdxpkdrv_FIRST_ERROR +0x06)
#define rtdxpkdrv_BAD_FRAME_SIZE       (rtdxpkdrv_FIRST_ERROR +0x07)
#define rtdxpkdrv_BAD_TRANSFER_SIZE    (rtdxpkdrv_FIRST_ERROR +0x08)
#define rtdxpkdrv_INTERNAL_ERROR       (rtdxpkdrv_FIRST_ERROR +0x09)
#define rtdxpkdrv_MISSING_PARAMETER    (rtdxpkdrv_FIRST_ERROR +0x0A)
#define rtdxpkdrv_BAD_ADDRESS          (rtdxpkdrv_FIRST_ERROR +0x0B)
#define rtdxpkdrv_UNKNOW_COMMAND       (rtdxpkdrv_FIRST_ERROR +0x0C)
#define rtdxpkdrv_OUTPUT_BUFFER_TOO_SMALL (rtdxpkdrv_FIRST_ERROR +0x0D)
#define rtdxpkdrv_BAD_CHANNEL_NO       (rtdxpkdrv_FIRST_ERROR +0x0E)
#define rtdxpkdrv_CHANNEL_NOT_CONNECTED (rtdxpkdrv_FIRST_ERROR +0x0F)
#define rtdxpkdrv_BAD_CHANNEL_TYPE     (rtdxpkdrv_FIRST_ERROR +0x10)
#define rtdxpkdrv_BAD_DEST_DEV_NO      (rtdxpkdrv_FIRST_ERROR +0x11)
#define rtdxpkdrv_BAD_DEST_FIFO_NO     (rtdxpkdrv_FIRST_ERROR +0x12)
#define rtdxpkdrv_BAD_SRC_FIFO_NO      (rtdxpkdrv_FIRST_ERROR +0x13)
#define rtdxpkdrv_STILL_RUNNING        (rtdxpkdrv_FIRST_ERROR +0x14)
#define rtdxpkdrv_INPUT_BUFFER_TOO_BIG (rtdxpkdrv_FIRST_ERROR +0x15)
#define rtdxpkdrv_DMRESET_TIMEOUT      (rtdxpkdrv_FIRST_ERROR +0x16)
#define rtdxpkdrv_CDMARESET_TIMEOUT    (rtdxpkdrv_FIRST_ERROR +0x17)
#define rtdxpkdrv_NO_COMMAND           (rtdxpkdrv_FIRST_ERROR +0x18)
#define rtdxpkdrv_LAST_ERROR           rtdxpkdrv_NO_COMMAND


#ifdef __cplusplus
}
#endif

#endif // RTDEXPCIE_DRVIFACE_H
