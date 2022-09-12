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
 * @file    perseus2hostregs.h
 * @brief   Definitions common to Perseus & Host Driver for Adp RTDEX PCIe
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

#ifndef PERSEUS2HOSTREGS_H
#define PERSEUS2HOSTREGS_H

#define LYRTECH_VENDOR_ID   0x1ae2

// Perseus standard regs:
#define STD_CTRL_REGS_START 0x70000000
#define STD_CTRL_REGS_SIZE  (64*1024)
 #define STD_CTRL_PCIE_CTRL_STAT  (0x01*4)
  #define PCIECTRL_PCIEMSI_BN        10 // bit number

// Perseus <--> Host DualPort Ram:
#define DPRAM_START         0x80100000
#define DPRAM_SIZE          (4*1024)
#define REG_SIZE            64

#define MAILBOXES_OFFSET    0
#define MAILBOXES_SIZE      (DPRAM_SIZE -REG_SIZE)

#define TO_FPGA_MAILBOX     0
#define FROM_FPGA_MAILBOX   1
#define NB_MAILBOX          2
#define MAILBOX_SIZE        (MAILBOXES_SIZE / NB_MAILBOX)

#define MAILBOX_MSG_DATA_LEN 256
#define MAILBOX_NB_MSG ((MAILBOX_SIZE -4) / (MAILBOX_MSG_DATA_LEN +4))
typedef struct {
   unsigned char u8Head;
   unsigned char u8Tail;
   struct {
      unsigned short u16Size;
      unsigned char au8Data[ MAILBOX_MSG_DATA_LEN];
   } aMsgs[ MAILBOX_NB_MSG];
} MailboxStruc;

#define MBX_RESET( mbx) iowrite8( ioread8( &(mbx)->u8Tail), &(mbx)->u8Head)
#define MBX_BUSY_SPACE( mbx) ((ioread8( &(mbx)->u8Tail) - ioread8( &(mbx)->u8Head)) & 0xff)
#define MBX_FREE_SPACE( mbx) (MAILBOX_NB_MSG - MBX_BUSY_SPACE( mbx))
#define MBX_PUT( mbx, inc) iowrite8( ioread8( &(mbx)->u8Tail) + (inc), &(mbx)->u8Tail)
#define MBX_GET( mbx, dec) iowrite8( ioread8( &(mbx)->u8Head) + (dec), &(mbx)->u8Head)
#define MBX_FREE_IDX( mbx) (ioread8( &(mbx)->u8Tail) % MAILBOX_NB_MSG)
#define MBX_BUSY_IDX( mbx) (ioread8( &(mbx)->u8Head) % MAILBOX_NB_MSG)

// misc regs numbers:
#define REG_OFFSET          (DPRAM_SIZE-REG_SIZE)

// command zone : Perseus --> Host rtdex control
#define REG32_UB_CMD        0x00 // set by Perseus, clear by Host
  #define CMD_IDLE 0
  #define CMD_RD   1
  #define CMD_RMW  2
  #define CMD_WR   3
  #define CMD_GET_INSTANCE 5
#define REG32_P1            0x04 // set by Perseus
#define REG32_P2            0x08 // set by Perseus
#define REG32_P3            0x0C // set by Perseus
#define REG32_RET_DATA      0x10 // set by Host
#define REG32_STATUS        0x14 // set by Host

#define REG32_INSTANCE      0x18 // RO set by host driver to indicate instance #

#endif //PERSEUS2HOSTREGS_H
