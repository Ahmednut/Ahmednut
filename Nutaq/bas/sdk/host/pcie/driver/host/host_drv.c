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
 *  Project     : Adp rtdex PCIe
 *  File        : host_drv.c
 *  Description : Linux Kernel Driver for RTDEX PCIe Perseus on Host
 *
 *  Author(s)   : Marc Savary, Eric Moreau
 *
 *                Copyright (c) Nutaq Inc, 2013
 *
 ***************************************************************************
*/
/*
 * This module provides network devices to access mailboxes and rtdex.
 *
 * A number of procfs entries are declared for debugging
 * only, they should be removed in a production environment.
 *
 */

#include <stdarg.h>

#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/ethtool.h>
#include <linux/interrupt.h>
#include <linux/if_arp.h>
#include <linux/proc_fs.h>
#include <linux/if.h>
#include <linux/semaphore.h>
#include <linux/wait.h>
#include <linux/percpu.h>
#include <linux/poll.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
 #include <linux/sched/signal.h>
#else
 #include <linux/sched.h>
#endif

#include "../../inc/perseus2hostregs.h" // interface between Perseus & Host driver
#include "../../inc/rtdexpcie_drviface.h" // interface with Host user space

// __devinit, __devexit was removed in linux 3.8:
#ifndef __devinit
 #define __devinit
#endif
#ifndef __devexit
 #define __devexit
#endif


#define DRIVER_VER_MAJ 0x01 // bcd
//#define DRIVER_VER_MIN 0x01 // bcd - 09jan2013
#define DRIVER_VER_MIN 0x02 // bcd - 15aug13



/**************** First level configuration variables **********/

//#define PERSEUS_DEBUG defined by Makefile

/* The following defines shall be for debugging purposes only */
#define PERSEUS_PROCFS
#define ROUND_TRIP_TEST // uncomment to include round-trip test 
//#define USE_DEBUG_DMA_BUFFER // uncomment to enable permanent DMA buffer - debug


/**************** Debug functions *************************************/

#undef PDEBUG
#undef PDEBUGA
#ifdef PERSEUS_DEBUG
#  ifdef __KERNEL__
#    define PDEBUG(fmt, args...) printk( KERN_DEBUG DEVICE_NAME ": %s " fmt, __func__, ## args)
#    define PDEBUGA(fmt, args...) printk(fmt, ## args)
#  else
#    define PDEBUG(fmt, args...) fprintf(stderr, fmt, ## args)
#    define PDEBUGA(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#  define PDEBUG(fmt, args...)
#  define PDEBUGA(fmt, args...)
#endif

#undef PERROR
#if 1
#  ifdef __KERNEL__
#    define PERROR(fmt, args...) printk( KERN_ERR DEVICE_NAME ": %s " fmt, __func__, ## args)
#  else
#    define PERROR(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif
#else
#    define PERROR(fmt, args...)
#endif

#undef PINFO
#  ifdef __KERNEL__
#    define PINFO(fmt, args...) printk( KERN_INFO DEVICE_NAME ": %s " fmt, __func__, ## args)
#  else
#    define PINFO(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif

#define MYERR "/// "


/**************** End of debug function *******************************/


#define DEVICE_NAME "perseus"
#define RTDEX_NAME "rtdex"

// number of perseus devices supported:
#define MAX_NB_DEVS 4


// rtdex ressources (max):
#define NB_AXI_BARS      6 // fixed
#define NB_DATA_MOVERS   8 // could be less - see RTDEX_RX_CONFIG_INFO, RTDEX_TX_CONFIG_INFO
#define NB_FIFOS         8 // ""  ""  ""
#define NB_CDMAS         1 // could be absent - see RTDEX_CTRL_STAT


// RTDEX Core registers - core version 0x0000 - as of 31jan2013:
#define RTDEX_REGS_START          0x22000 // from CPU BAR0, space= 64KB
 #define RTDEX_CORE_ID_VERS        (0x00*4)
   #define COREID_BO 16
   #define COREID_BM (0xffff<<COREID_BO)
   #define COREVER_BO 0
   #define COREVER_BM (0xffff<<COREVER_BO)
 #define RTDEX_CTRL_STAT           (0x01*4)
   #define CTRLSTAT_CDMA_PRESENT_BN   2 // CDMA present
   #define CTRLSTAT_MB_IRQ_BN         1        // trig irq to uBlaze
   #define CTRLSTAT_RESET_BN          0
 #define RTDEX_RXTX_RESET          (0x2*4)
   #define FIFO_RST_RX_BM_n(n)        (1<<(n)) // n = 0..NB_FIFOS-1
   #define FIFO_RST_CLR_WDCNT_BM_n(n) (1<<((n)+8)) // n = 0..NB_DATA_MOVERS-1
   #define FIFO_RST_TX_BM_n(n)        (1<<((n)+16)) // n = 0..NB_FIFOS-1
 #define RTDEX_RX_CONFIG_INFO      (0x03*4)
   #define RTDEX_CFGINF_NBCHNL_BM 0x0F
   #define RTDEX_CFGINF_FIFODEPTH_BO 4
   #define RTDEX_CFGINF_FIFODEPTH_BM (0x3F<<RTDEX_CFGINF_FIFODEPTH_BO) // size = 2**(n+1) bytes
   #define RTDEX_CFGINF_STATSENABLED_BM (1<<9)
 #define RTDEX_RX_FIFO_STATUS      (0x04*4)
   #define FIFOSTAT_OVERF_BM_n(n)     (1<<((n)+8)) // n = 0..0..NB_FIFOS-1
   #define FIFOSTAT_UNDERF_BM_n(n)    (1<<(n)) // n = 0..NB_FIFOS-1
 #define RTDEX_FIFO_ENABLE         (0x05*4)
   #define FIFOEN_RXWR_BM_n(n)        (1<<(n)) // n = 0..NB_FIFOS-1
   #define FIFOEN_RXWR_BM_all         (0xFF<<0)
   #define FIFOEN_RXRD_BM_n(n)        (1<<((n)+8)) // n = 0..NB_FIFOS-1
   #define FIFOEN_RXRD_BM_all         (0xff<<8)
   #define FIFOEN_TXWR_BM_n(n)        (1<<((n)+16)) // n = 0..NB_FIFOS-1
   #define FIFOEN_TXWR_BM_all         (0xFF<<16)
 #define RTDEX_RX_WORDCNT_n(n)     (0x06*4+(n)*4) // reg 0x06 to 0x0D, n = 0..NB_FIFOS-1
// (0x0E*4)
// (0x0F*4)
 #define RTDEX_TX_CONFIG_INFO      (0x10*4)
  // same as RTDEX_RX_CONFIG_INFO
 #define RTDEX_TX_FIFO_STATUS      (0x11*4)
  // bit mask same as RTDEX_RX_FIFO_STATUS
 #define RTDEX_TX_DM_TFR_CTRL      (0x12*4)
   #define TFRCTRL_DONE_BM_n(n)       (1<<((n)+16)) // n = 0..NB_DATA_MOVERS-1
   #define TFRCTRL_DONE_BM_all        (0xFF<<16)
   #define TFRCTRL_STREAM_BM_n(n)     (1<<((n)+8)) // n = 0..NB_DATA_MOVERS-1
   #define TFRCTRL_START_BM_n(n)      (1<<(n)) // rising, n = 0..NB_DATA_MOVERS-1
 #define RTDEX_TX_DM_IRQ_CTRL      (0x13*4)
   #define IRQCTRL_EN_LAST_TFR_BM_n(n) (1<<(n)) // n = 0..NB_DATA_MOVERS-1
   #define IRQCTRL_EN_LAST_TFR_BM_all  (0xFF)
 #define RTDEX_TX_DM_CTRL          (0x14*4)
   #define DMCTRL_SHUTDOWN_BM_n(n)     (1<<(n)) // n = 0.. NB_DATA_MOVERS-1
   #define DMCTRL_HALTED_BM_n(n)       (1<<((n)+8)) // n = 0..NB_DATA_MOVERS-1
   #define DMCTRL_ERROR_BM_n(n)        (1<<((n)+16)) // n = 0..NB_DATA_MOVERS-1
   #define DMCTRL_RESET_BM_n(n)        (1<<((n)+24)) // n = 0..NB_DATA_MOVERS-1
 #define RTDEX_TX_DM_TFRCNT_n(n)   (0x15*4 +8*4*(n)) // n = 0..NB_DATA_MOVERS-1
 #define RTDEX_TX_DM_CUR_TFRCNT_n(n)(0x16*4 +8*4*(n)) // n = 0..NB_DATA_MOVERS-1
 #define RTDEX_TX_DM_CTRL_n(n)     (0x17*4 +8*4*(n)) // n = 0..NB_DATA_MOVERS-1
 #define RTDEX_TX_DM_ADDR_n(n)     (0x18*4 +8*4*(n)) // n = 0..NB_DATA_MOVERS-1
 #define RTDEX_TX_DM_TAG_n(n)      (0x19*4 +8*4*(n)) // n = 0..NB_DATA_MOVERS-1
 #define RTDEX_TX_DM_STAT_n(n)     (0x1A*4 +8*4*(n)) // n = 0..NB_DATA_MOVERS-1
// (0x1C*4)
// (0x1D*4)


// Xilinx AXI to PCIe Bridge Config registers:
#define PCIE_BRIDGE_START        0x24000 // from CPU BAR0
#define PCIE_BRIDGE_LIMIT        0x238
#define PCIE_BRIDGE_SIZE         0x1000
 #define AXIBAR2PCIEBARH_n(n)     (0x208 +(n)*8) // n = 0..NB_AXI_BARS-1
 #define AXIBAR2PCIEBARL_n(n)     (0x20c +(n)*8) // n = 0..NB_AXI_BARS-1

// Bridge Translation Windows:
#define AXIBAR_BASE_ADDR   0x88000000
#define AXIBAR_ADDR_SPACE  (8*1024*1024) // each BAR cover this space
#define AXIBAR_START_n(n)  (AXIBAR_BASE_ADDR +(n)*AXIBAR_ADDR_SPACE) // n = 0..NB_AXI_BARS-1


// base address of devices, as see by CDMA, DM, etc..
#define AXI_MM_PCIE 0x50000000


// Xilinx CDMA Core:
#define CDMA_START          0x20000 // from CPU BAR0
#define CDMA_LIMIT          0x2c
#define CDMA_SIZE           0x80
 #define CDMACR  0x00
   #define CDMACR_RESET_BM     (1<<2)
   #define CDMACR_SGMODE_BM    (1<<3)
   #define CDMACR_IOC_IRQEN_BM (1<<12)
   #define CDMACR_DLY_IRQEN_BM (1<<13)
   #define CDMACR_ERR_IRQEN_BM (1<<14)
   #define CDMACR_IRQ_TRESHOLD_BN 16
   #define CDMACR_IRQ_TRESHOLD_BC 8
   #define CDMACR_IRQ_DELAY_BN 24
   #define CDMACR_IRQ_DELAY_BC 8
 #define CDMASR  0x04
   #define CDMASR_IDLE_BM      (1<<1)
   #define CDMASR_IOC_IRQ_BM   (1<<12)
   #define CDMASR_DLY_IRQ_BM   (1<<13)
   #define CDMASR_ERR_IRQ_BM   (1<<14)
 #define CDMACURDESC 0x08
 #define CDMATAILDESC 0x10
 #define CDMASA  0x18
 #define CDMADA  0x20
 #define CDMABTT 0x28

// Xilinx CDMA Scatter/Gatter:
/*
#define SGDESC_START        0x10000 // from CPU BAR0
#define SGDESC_BASE_ADDR    (AXI_MM_PCIE +SGDESC_START) // from CDMA controller
#define SGDESC_SIZE         (8*1024)
 #define SGDESC_NXTDESC_PNTR 0x00
 #define SGDESC_SA           0x08
 #define SGDESC_DA           0x10
 #define SGDESC_CONTROL      0x18
   #define SGDESC_CTRL_BTT_BM 0x7fffff
 #define SGDESC_STATUS       0x1c
*/

// FIFOs:
#define FIFO_BASE_ADDR     (AXI_MM_PCIE +0x1000000) // == 0 from CPU second BAR
#define FIFO_SIZE          (8*1024) // bytes
#define FIFO_ADDR_SPACE    (128*1024) // bytes
#define FIFO_OFFSET_n( n)  ((n) * FIFO_ADDR_SPACE) // n = 0..NB_FIFOS-1



// AXI to PCIe Bus Master Window size:
#define AXI2BAR_IS_64BITS // uncomment to change AXI2BAR translation from 32 to 64 bits

#ifdef CONFIG_PHYS_ADDR_T_64BIT // from <linux/types.h>
 #define IS_64BIT_PTR
#endif
#if !defined(AXI2BAR_IS_64BITS) && defined(IS_64BIT_PTR)
   #error *** potential problem with device 32-bit on 64-bit system - check DMA adressing.. ***
#endif
#define AXI2BAR_WINDOW_SIZE_BC 23 // 20 // window size for Host-to-Dev(CDMA) and Dev-to-Host(DM) transfers
 #define DMA_HIGH_ADDR_MASK32 ((u32)~0<<AXI2BAR_WINDOW_SIZE_BC)
 #define DMA_LOW_ADDR_MASK32 ((u32)~0>>(32-AXI2BAR_WINDOW_SIZE_BC))


#ifdef USE_DEBUG_DMA_BUFFER
  #define DMA_BUFFER_SIZE    (64*1024) // debug
#endif

#define MAX_DM_TFR_COUNT ((1<<24)-1)
#define MAX_DM_TO_DEV_FRAME_SIZE FIFO_ADDR_SPACE // must be <= FIFO address space (FIFO_ADDR_SPACE)
#define MAX_DM_TO_HOST_FRAME_SIZE ((1<<(AXI2BAR_WINDOW_SIZE_BC - 1)) - WORD_SIZE_B) // must be < (1<<AXI2BAR_WINDOW_SIZE_BC - 1). "-1" because two DMs share the same AXI-BAR.
#define MAX_CDMA_FRAME_SIZE FIFO_ADDR_SPACE // must be < (1<<AXI2BAR_WINDOW_SIZE_BC) && <= FIFO address space

#define DEF_DM_BUFF_SIZE_K 256 // default queue size for each DM to Host, KBytes
#define MAX_DM_BUFF_SIZE_K (4 * 1024) // max queue size of each DM to Host, KBytes
#define DEF_SQ_CDMA_BUFF_SIZE_K (1 * 1024) // default common queue size for CDMA, KBytes
#define MAX_SQ_CDMA_BUFF_SIZE_K (4 * 1024) // max common queue size for CDMA, KBytes
#define NB_SQ_DESC_ENTRIES 256

// Minor numbers:
// 0..NB_FIFO-1         = rx-fifos
// NB_FIFO..NB_FIFO*2-1 = tx-fifos
// NB_FIFO*2            = global control
#define MINOR_GLOBAL (NB_FIFOS*2)


#define MAILBOX_TX_QUEUE_LEN 32


// CDMA memory descriptor definition:
typedef struct cdmaDesc {
   union {
      u8 au8[ 64];  // CDMA-SG require 64-bytes alignement

      struct {
         u32 uNextDesc;
         u32 filler1;
         u32 uSrcAddr;
         u32 filler2;
         u32 uDstAddr;
         u32 filler3;
         u32 uControl;
            #define SGDESC_CTRL_BTT_BM 0x7fffff
         u32 uStatus;

         // extension -- for use by CPU only:
         struct cdmaDesc * next; 
         unsigned uFifoNo;
      } desc;
   } u;
} CdmaDesc;



typedef struct {
   unsigned uChnlNo; // 0..n
   unsigned bAlloc; // 0 == free, 1 = allocated
   unsigned uChnlType; 
      #define TYPE_DEV_TO_HOST 0
      #define TYPE_HOST_TO_DEV 1
      #define TYPE_DEV_TO_DEV  2
   unsigned uState; // ST_IDLE..ST_STOP_OK
      #define ST_IDLE        0 // just after connect
      #define ST_RUN         1 // dma in progress
      #define ST_WAIT_RXFIFO 2 // dma done but wait for rx-fifo empty
      #define ST_STOP_ABORT  3 // aborted by user before transfer end
      #define ST_STOP_FAIL   4 // stop with error
      #define ST_STOP_OK     5 // all done & ok
   unsigned uFrameSize; // requested frame size
   size_t transferSize; // requested transfer size, 0 == continuous
   union {
      struct {
         unsigned uDevNo;
         unsigned uFifoNo;
      } host;
      struct {
         unsigned uSrcFifoNo; // fifo# == DM#
         unsigned uDstFifoNo;
         unsigned uDstDevNo;
      } dev2dev;
   } u;
   // stats:
   size_t dmaXferedSize; // progress of DM/CDMA transfer, bytes
   size_t hostXferedSize; // progress of Host rd/wr transfer, bytes
} AllocChannel;
static char * acTypeNames[] = { "dev2host", "host2dev", "dev2dev"};

typedef struct {
   dma_addr_t physAddr;
   void * pvBuffer;
   unsigned uBuffSize;
   unsigned uHead, uTail;
   atomic_t length;
} Queue;


#if 0
// Macros to manipulate data queues (version using atomic length counter):
#define QUEUE_ALLOCATED( queue) ((queue)->uBuffSize > 0)
#define QUEUE_INIT( queue)
#define QUEUE_RESET( queue) do{ (queue)->uHead = (queue)->uTail = 0; atomic_set( &(queue)->length, 0);} while(0)
#define QUEUE_BUSY_SPACE( queue) (unsigned)atomic_read( &(queue)->length)
#define QUEUE_FREE_SPACE( queue) ((queue)->uBuffSize - QUEUE_BUSY_SPACE( queue))
#define QUEUE_PUT( queue, inc) do{(queue)->uTail= ((queue)->uTail + (inc)) % (queue)->uBuffSize; atomic_add( (inc), &(queue)->length);} while(0)
#define QUEUE_GET( queue, dec) do{(queue)->uHead= ((queue)->uHead + (dec)) % (queue)->uBuffSize; atomic_sub( (dec), &(queue)->length);} while(0)

#define QUEUE_NOWRAP_FREE_SPACE( queue) min( (queue)->uBuffSize - (queue)->uTail, QUEUE_FREE_SPACE( queue))
#define QUEUE_NOWRAP_BUSY_SPACE( queue) min( (queue)->uBuffSize - (queue)->uHead, QUEUE_BUSY_SPACE( queue))
#define QUEUE_FREE_PTR( queue) ((queue)->uTail + (queue)->pvBuffer)
#define QUEUE_FREE_PHYSADDR( queue) ((queue)->uTail + (queue)->physAddr)
#define QUEUE_BUSY_PTR( queue) ((queue)->uHead + (queue)->pvBuffer)
#define QUEUE_BUSY_PTR_N( queue, n) (((queue)->uHead + (n)) % (queue)->uBuffSize + (queue)->pvBuffer)
#define QUEUE_BUSY_PHYSADDR( queue) ((queue)->uHead + (queue)->physAddr)
#define QUEUE_BUSY_PHYSADDR_N( queue, n) (((queue)->uHead + (n)) % (queue)->uBuffSize + (queue)->physAddr)

#else

// Macros to manipulate data queues:
#define QUEUE_ALLOCATED( queue) ((queue)->pvBuffer != NULL)
#define QUEUE_INIT( queue) ((queue)->uBuffSize = -1)
#define QUEUE_RESET( queue) ((queue)->uHead = (queue)->uTail = 0)
#define QUEUE_BUSY_SPACE( queue) ((queue)->uTail - (queue)->uHead)
#define QUEUE_FREE_SPACE( queue) ((queue)->uBuffSize - QUEUE_BUSY_SPACE( queue))
#define QUEUE_PUT( queue, inc) ((queue)->uTail += (inc))
#define QUEUE_GET( queue, dec) ((queue)->uHead += (dec))

#define QUEUE_NOWRAP_FREE_SPACE( queue) min( (queue)->uBuffSize - (queue)->uTail % (queue)->uBuffSize, QUEUE_FREE_SPACE( queue))
#define QUEUE_NOWRAP_BUSY_SPACE( queue) min( (queue)->uBuffSize - (queue)->uHead % (queue)->uBuffSize, QUEUE_BUSY_SPACE( queue))
#define QUEUE_FREE_PTR( queue) ((queue)->uTail % (queue)->uBuffSize + (queue)->pvBuffer)
#define QUEUE_FREE_PHYSADDR( queue) ((queue)->uTail % (queue)->uBuffSize + (queue)->physAddr)
#define QUEUE_BUSY_PTR( queue) ((queue)->uHead % (queue)->uBuffSize + (queue)->pvBuffer)
#define QUEUE_BUSY_PTR_N( queue, n) (((queue)->uHead + (n)) % (queue)->uBuffSize + (queue)->pvBuffer)
#define QUEUE_BUSY_PHYSADDR( queue) ((queue)->uHead % (queue)->uBuffSize + (queue)->physAddr)
#define QUEUE_BUSY_PHYSADDR_N( queue, n) (((queue)->uHead + (n)) % (queue)->uBuffSize + (queue)->physAddr)
#endif

// user-logic--->TX-FIFO--->devA--->PCIe--->devB--->RX-FIFO--->user-logic
//
//!!!
typedef struct {
   unsigned uInstance; // 0..MAX_NB_DEVS-1

   struct pci_dev *pdev;
   struct cdev rtdex_cdev;
	dev_t rtdex_dev_number; // kernel to user space device #
	struct net_device *mailbox_ndev;
   unsigned bMbxStoped; // MSA-19apr2013

   // detected fifo config:
   struct {
      unsigned uNbFifos; // detected # of fifos
      unsigned uFifoSizeB; // detected size of of each fifos, bytes
   } rxCfg, txCfg;

     #define MAX_LOGICAL_CHANNELS (NB_FIFOS * 2)
   AllocChannel devChannels[ MAX_LOGICAL_CHANNELS];

   struct {
      unsigned bOpen;
      AllocChannel * pAC;
   } rxFifos[ NB_FIFOS]; // PCIe--->dev--->RX-FIFO--->user-logic

   struct {
      unsigned bOpen;
      Queue queue;
      AllocChannel * pAC;
   } txFifos[ NB_FIFOS]; // user-logic--->TX-FIFO--->dev--->PCIe

   struct {
      unsigned bPresent;
      // DMA (data + desc-table) buffer:
      struct {
         void * pvBase;
         dma_addr_t physAddr;
         unsigned uSize;
      } buffer;
      Queue dataQueue; // data queue (fit into 'buffer')
      Queue descQueue; // descriptors queue (fit into 'buffer')

      unsigned uFlushThreshold;
      unsigned bForceFlush;
      unsigned bDelayTrig;
      unsigned bWorking; // Cdma active
      unsigned uNbActDesc; // # of active Cdma descriptors

      unsigned uIrqErrors; // # of irq errors
      unsigned uIrqDelays; // # of irq delays
      unsigned uIrqIocs; // # of irq IOC

      unsigned uBusyErrors; // # of (driverState == IDLE but chip is BUSY)
   } sqCdma;

   struct {
      size_t size; // current size (data) for active transfer
      unsigned bWorking; // DM active
   } dmState[ NB_DATA_MOVERS]; // [fpga to host] or [fpga to fpga] -- dm

   unsigned uBarNoUseCount[ NB_AXI_BARS];
   unsigned uDmBarNo[ NB_FIFOS]; // AXI BAR# associated with DM#
   unsigned uCdmaBarNo;
   unsigned uCdmaBarNoUseCount;

   unsigned uGlobalOpen;

   spinlock_t cdmaSpinlock; // protect all cdma regs
   spinlock_t dmSpinlock; // protect all dm regs
   spinlock_t miscRegsSpinlock; // protect other regs

   wait_queue_head_t inq, outq;       /* read and write queues */
   struct semaphore rdSem;              /* mutual exclusion read semaphore */
   struct semaphore wrSem;              /* mutual exclusion write semaphore */
   struct semaphore ctlSem;              /* mutual exclusion write semaphore */

	void __iomem *firstBar;  // host to control memory
     #define FIRST_BAR_NO 0
   unsigned firstBarSize;
   void __iomem *secondBar;  // fifos memory
#ifdef AXI2BAR_IS_64BITS
     #define SECOND_BAR_NO 2 // 64-bit --> bar1 do not exist
#else
     #define SECOND_BAR_NO 1
#endif
   unsigned secondBarSize;
   dma_addr_t fifoBaseAddrPhys;

   void __iomem *inbox;
	void __iomem *outbox;
	void __iomem *mRegs;
   void __iomem *cdma;
//   void __iomem *sg;
   void __iomem *rtdex;
   void __iomem *pcieBr;


#ifdef PERSEUS_PROCFS
	struct proc_dir_entry * proc_instance;
   struct proc_dir_entry * procEntryMemRW;
   struct proc_dir_entry * procEntryState;
   unsigned uReadOffset;
   unsigned uReadZoneNo;
   char acPrintBuff[ 8 * 1024];
   unsigned uPrintSize;
#endif
#ifdef ROUND_TRIP_TEST
   unsigned bRoundTripTest;
   unsigned uRTP_dmNo;
#endif
#ifdef USE_DEBUG_DMA_BUFFER
   void * _pucBuff;
	dma_addr_t physAddr;
#endif
} PerseusData;



// sanity check:
#if MAX_DM_TO_DEV_FRAME_SIZE > FIFO_ADDR_SPACE
  #error *** MAX_DM_TO_DEV_FRAME_SIZE too big! ***
#endif

#if MAX_DM_TO_HOST_FRAME_SIZE >= (1<<(AXI2BAR_WINDOW_SIZE_BC - 1))
  #error *** MAX_DM_TO_HOST_FRAME_SIZE too big! ***
#endif

#if MAX_CDMA_FRAME_SIZE > FIFO_ADDR_SPACE || MAX_CDMA_FRAME_SIZE >= (1<<AXI2BAR_WINDOW_SIZE_BC)
  #error *** MAX_CDMA_FRAME_SIZE too big! ***
#endif




static struct class *perseus_class;
static struct class *rtdex_class;

static atomic_t perseus_count;

static struct semaphore fifoSem;

static struct {
   PerseusData * perseus;
   } allDevs[ MAX_NB_DEVS];

static unsigned cdmasizek = DEF_SQ_CDMA_BUFF_SIZE_K;
static unsigned dmsizek = DEF_DM_BUFF_SIZE_K;
module_param( cdmasizek, uint, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
module_param( dmsizek, uint, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);

static struct pci_device_id perseus_pci_id_table[] = {
	{ PCI_DEVICE(LYRTECH_VENDOR_ID, 0x2525), },
	{ PCI_DEVICE(LYRTECH_VENDOR_ID, 0x6010), },
	{0,}
};

MODULE_DEVICE_TABLE( pci, perseus_pci_id_table);


/*
These are just examples of MODULE_PARM_DESC()

static unsigned io_limit = 128;
module_param(io_limit, uint, 0);
MODULE_PARM_DESC(io_limit, "Maximum bytes per I/O (default 128)");

module_param_string(eata, boot_options, MAX_BOOT_OPTIONS_SIZE, 0);
MODULE_PARM_DESC(eata, " equivalent to the \"eata=...\" kernel boot option."
                 "            Example: modprobe eata \"eata=0x7410,0x230,lc:y,tm:0,mq:4,ep:n\"");
*/





/*
 =========================================================================================
 * 
 * Mailbox (cce control port) network device functions
 *
 =========================================================================================
 */

static int perseus_net_set_mac_address( struct net_device *ndev, void *p)
{
	PDEBUG("\n");
        eth_mac_addr( ndev, p);
        return 0;
}


#ifdef CONFIG_NET_POLL_CONTROLLER
static void perseus_net_poll_controller( struct net_device *ndev)
{
	PDEBUG("\n");
}
#endif


extern void perseus_netdev_setup( struct net_device *ndev)
{
	ether_setup( ndev);
}


static int perseus_mailbox_net_change_mtu(struct net_device *ndev, int new_mtu)
{
//   if (new_mtu > MAILBOX_SIZE - 64) {
//		new_mtu = MAILBOX_SIZE - 64;
//	}
   if (new_mtu > MAILBOX_MSG_DATA_LEN) {
		new_mtu = MAILBOX_MSG_DATA_LEN;
	}
	ndev->mtu = new_mtu;
	PDEBUG("%d\n", new_mtu);
	return 0;
}


static int perseus_mailbox_net_open(struct net_device *ndev)
{
	PerseusData * perseus = *(PerseusData**)netdev_priv(ndev);

	PDEBUG("\n");
//	iowrite16( 0, perseus->inbox + MAILBOX_MSG_LEN16);
   MBX_RESET( (MailboxStruc*)perseus->inbox);
   netif_start_queue(ndev);
	return 0;
}


static int perseus_mailbox_net_stop( struct net_device *ndev)
{
	PDEBUG("\n");
	netif_stop_queue( ndev);
	return 0;
}



// ###
static int perseus_mailbox_net_tx( struct sk_buff *skb, struct net_device *ndev)
{
	PerseusData * perseus = *(PerseusData**)netdev_priv( ndev);
	uint32_t uVal;
   MailboxStruc * pMbx = (MailboxStruc*)perseus->outbox;
   unsigned uIdx;

   PDEBUG( "len= %u", skb->len);
   if( skb->len > MAILBOX_MSG_DATA_LEN) {
      PDEBUG( "skb->len too big!\n");
      ndev->stats.tx_dropped++;
      return -EFAULT;
	}

   if( 0 == MBX_FREE_SPACE( pMbx)) {
      PDEBUG("cannot send, outbox full!\n");
      netif_stop_queue( ndev); // MSA-19apr2013
      perseus->bMbxStoped = 1; // MSA-19apr2013
		return -EBUSY;
//		return -EAGAIN;
   }

   uIdx = MBX_FREE_IDX( pMbx);
   // format message:
	memcpy_toio( pMbx->aMsgs[ uIdx].au8Data, skb->data, skb->len);
	iowrite16( skb->len, &pMbx->aMsgs[ uIdx].u16Size);
   MBX_PUT( pMbx, 1);

   // trig an interrupt to uBlaze:
   uVal = ioread32( perseus->rtdex + RTDEX_CTRL_STAT);
	iowrite32( uVal | (1 << CTRLSTAT_MB_IRQ_BN), perseus->rtdex + RTDEX_CTRL_STAT);
	iowrite32( uVal & ~(1 << CTRLSTAT_MB_IRQ_BN), perseus->rtdex + RTDEX_CTRL_STAT);
    
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,7,0)
    netif_trans_update(ndev);
#else
    ndev->trans_start = jiffies; // MSA-19apr2013
#endif
    
    
   // update stats:
   ndev->stats.tx_packets++;
	ndev->stats.tx_bytes += skb->len;
   dev_kfree_skb( skb);

   return 0;
}

/*
static int perseus_mailbox_net_tx( struct sk_buff *skb, struct net_device *ndev)
{
	PerseusData * perseus = *(PerseusData**)netdev_priv( ndev);
	uint32_t uVal;

   PDEBUG( "len= %u", skb->len);
   if( skb->len > MAILBOX_SIZE - MAILBOX_HEADER_SIZE) {
      PDEBUG( "skb->len too big!\n");
      ndev->stats.tx_dropped++;
      return -EFAULT;
	}

   //PDEBUG("%d\n", skb->len);
   if( 0 != ioread16( perseus->outbox + MAILBOX_MSG_LEN16)) {
      PDEBUG("cannot send, outbox not empty!\n");
      netif_stop_queue( ndev); // MSA-19apr2013
      perseus->bMbxStoped = 1; // MSA-19apr2013
		return -EBUSY;
//		return -EAGAIN;
   }

   // format message:
	memcpy_toio( perseus->outbox + MAILBOX_DATA, skb->data, skb->len);
	iowrite16( skb->len, perseus->outbox + MAILBOX_MSG_LEN16);

   // trig an interrupt to uBlaze:
   uVal = ioread32( perseus->rtdex + RTDEX_CTRL_STAT);
	iowrite32( uVal | (1 << CTRLSTAT_MB_IRQ_BN), perseus->rtdex + RTDEX_CTRL_STAT);
	iowrite32( uVal & ~(1 << CTRLSTAT_MB_IRQ_BN), perseus->rtdex + RTDEX_CTRL_STAT);

   ndev->trans_start = jiffies; // // MSA-19apr2013

   // update stats:
   ndev->stats.tx_packets++;
	ndev->stats.tx_bytes += skb->len;
   dev_kfree_skb( skb);

   return 0;
}
*/

static const struct net_device_ops perseus_mailbox_ndev_ops = {
	.ndo_open = perseus_mailbox_net_open,
	.ndo_stop = perseus_mailbox_net_stop,
	.ndo_start_xmit = perseus_mailbox_net_tx,
	.ndo_change_mtu = perseus_mailbox_net_change_mtu,
	.ndo_set_mac_address = perseus_net_set_mac_address,
	.ndo_validate_addr = eth_validate_addr,
#ifdef CONFIG_NET_POLL_CONTROLLER
	.ndo_poll_controller = perseus_net_poll_controller,
#endif
};


static int perseus_create_mailbox_ndev( PerseusData *perseus)
{
	PerseusData **priv;
/* FIXME: get mac address from device tree */
	unsigned char mac_address[8] = {0x00, 0x00, 0x00, 0xd0, 0xcc, 0xbe, 0xef, 0x00 };

	PDEBUG("\n");
   
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,0)
	perseus->mailbox_ndev = alloc_netdev(sizeof(PerseusData *), MAILBOX_IFACE_NAME "%d", NET_NAME_UNKNOWN, perseus_netdev_setup);
#else
    perseus->mailbox_ndev = alloc_netdev(sizeof(PerseusData *), MAILBOX_IFACE_NAME "%d", perseus_netdev_setup);
#endif
	if (!(perseus->mailbox_ndev)) {
		PERROR("can't allocate net device\n");
		return -ENOMEM;
	}
	mac_address[7] = perseus->uInstance;	

   perseus_netdev_setup( perseus->mailbox_ndev);
	perseus->mailbox_ndev->netdev_ops = &perseus_mailbox_ndev_ops;
	perseus->mailbox_ndev->flags &= ~IFF_MULTICAST;
//   perseus->mailbox_ndev->flags |= IFF_NOARP | IFF_POINTOPOINT;
	perseus->mailbox_ndev->mtu = MAILBOX_MSG_DATA_LEN; // MAILBOX_SIZE - 64;
//   perseus->mailbox_ndev->features |= NETIF_F_NO_CSUM;
//   perseus->mailbox_ndev->hard_header_cache = NULL;
	perseus->mailbox_ndev->tx_queue_len = MAILBOX_TX_QUEUE_LEN;

   priv = netdev_priv( perseus->mailbox_ndev);
	*priv = perseus;
	
	PDEBUG("net device at %p\n", perseus->mailbox_ndev);
	if (register_netdev(perseus->mailbox_ndev)) {
		PERROR("can't register net device\n");
		return -ENODEV;
	}

	perseus_net_set_mac_address( perseus->mailbox_ndev, mac_address);
	PDEBUG("returning\n");

	return 0;
}


static int perseus_destroy_mailbox_ndev( PerseusData *perseus)
{
	unregister_netdev(perseus->mailbox_ndev);
	free_netdev(perseus->mailbox_ndev);
	PDEBUG("net device destroyed\n");
	return 0;
}



// Called by irq()
static void ProcessMailboxRx( struct net_device *ndev)
{
	PerseusData * perseus = *(PerseusData**)netdev_priv( ndev);
	struct sk_buff *skb;
	ssize_t size;
	int ret;
   MailboxStruc * pMbx = (MailboxStruc*)perseus->inbox;
   unsigned uIdx;

   if( 0 == MBX_BUSY_SPACE( pMbx))
      return;

   uIdx = MBX_BUSY_IDX( pMbx);
   size = pMbx->aMsgs[ uIdx].u16Size;

	skb = dev_alloc_skb( size + 2);
	if (!skb) {
		ndev->stats.rx_dropped++;
		PDEBUG("no skb\n");
		goto perseus_mailbox_net_rx_out;
	}

   skb_reserve(skb, 2);

	memcpy_fromio( skb_put( skb, size), pMbx->aMsgs[ uIdx].au8Data, size);
	skb->dev = ndev;
	skb->protocol = eth_type_trans( skb, ndev);
	skb->ip_summed = CHECKSUM_UNNECESSARY;
   if ((ret = netif_rx( skb))) {
		PERROR("netif_rx() error: %d\n", ret);
      dev_kfree_skb_irq( skb);
		ndev->stats.rx_errors++;
	} else {
      PDEBUG( "size= %u\n", size);
   	ndev->stats.rx_packets++;
	   ndev->stats.rx_bytes += size;
   }
perseus_mailbox_net_rx_out:
   MBX_GET( pMbx, 1);
//	iowrite16( 0, perseus->inbox + MAILBOX_MSG_LEN16);
	return;
}

/*
static void ProcessMailboxRx( struct net_device *ndev)
{
	PerseusData * perseus = *(PerseusData**)netdev_priv( ndev);
	struct sk_buff *skb;
	ssize_t size;
	int ret;

	size = ioread16( perseus->inbox + MAILBOX_MSG_LEN16);
   if( !size)
      return;

	skb = dev_alloc_skb( size + 2);
	if (!skb) {
		ndev->stats.rx_dropped++;
		PDEBUG("no skb\n");
		goto perseus_mailbox_net_rx_out;
	}

   skb_reserve(skb, 2);

	memcpy_fromio( skb_put( skb, size), perseus->inbox + MAILBOX_DATA, size);
	skb->dev = ndev;
	skb->protocol = eth_type_trans( skb, ndev);
	skb->ip_summed = CHECKSUM_UNNECESSARY;
   if ((ret = netif_rx( skb))) {
		PERROR("netif_rx() error: %d\n", ret);
      dev_kfree_skb_irq( skb);
		ndev->stats.rx_errors++;
	} else {
      PDEBUG( "size= %u\n", size);
   	ndev->stats.rx_packets++;
	   ndev->stats.rx_bytes += size;
   }
perseus_mailbox_net_rx_out:
	iowrite16( 0, perseus->inbox + MAILBOX_MSG_LEN16);
	return;
}
*/


// =========================================================================================
//
// RTDEx char device
//
// =========================================================================================


static int iGetFreeBarNo( PerseusData * perseus, unsigned * puBarNo)
{
   unsigned u;

   for( u = 0; u < NB_AXI_BARS; u++) {
      if( perseus->uBarNoUseCount[ u] == 0)
         break;
   }
   if( u < NB_AXI_BARS) {
      *puBarNo = u;
      return 0;
      }
   PDEBUG( MYERR "No more free Bar!\n");
   return rtdxpkdrv_NO_FREE_AXI2BAR;
}



static unsigned bIsSrcFifoFree( PerseusData * perseus, unsigned uFifoNo)
{
   unsigned u;

   for( u = 0; u < MAX_LOGICAL_CHANNELS; u++) {
      if( perseus->devChannels[ u].bAlloc) {
         if( perseus->devChannels[ u].uChnlType == TYPE_DEV_TO_HOST) {
            if( perseus->devChannels[ u].u.host.uFifoNo == uFifoNo) {
               PDEBUG( MYERR "Fifo (%u) busy!\n", uFifoNo);
               return 0; // busy
            }
         }
         else if( perseus->devChannels[ u].uChnlType == TYPE_DEV_TO_DEV) {
            if( perseus->devChannels[ u].u.dev2dev.uSrcFifoNo == uFifoNo) {
               PDEBUG( MYERR "Fifo (%u) busy!\n", uFifoNo);
               return 0; // busy
            }
         }
      }
   }
   return 1; // free!
}


static unsigned bIsDstFifoFree( PerseusData * destPers, unsigned uFifoNo)
{
   unsigned u, k;
   PerseusData * pers;

   for( u = 0; u < MAX_LOGICAL_CHANNELS; u++) {
      if( destPers->devChannels[ u].bAlloc) {
         if( destPers->devChannels[ u].uChnlType == TYPE_HOST_TO_DEV) {
            if( destPers->devChannels[ u].u.host.uFifoNo == uFifoNo) {
               PDEBUG( MYERR "Fifo (%u) busy!\n", uFifoNo);
               return 0; // busy
            }
         }
      }
   }

   for( k = 0; k < MAX_NB_DEVS; k++) {
      pers = allDevs[ k].perseus;
      if( !pers)
         continue;
      for( u = 0; u < MAX_LOGICAL_CHANNELS; u++) {
         if( pers->devChannels[ u].bAlloc) {
            if( pers->devChannels[ u].uChnlType == TYPE_DEV_TO_DEV) {
               if( pers->devChannels[ u].u.dev2dev.uDstDevNo == destPers->uInstance
               && pers->devChannels[ u].u.dev2dev.uDstFifoNo == uFifoNo) {
                  PDEBUG( MYERR "Fifo (%u) busy!\n", uFifoNo);
                  return 0; // busy
               }
            }
         }
      }
   }

   return 1; // free!
}



// dev --> dev
static int iAllocateDev2DevFifos( PerseusData * perseus, unsigned uDstDevNo, unsigned uSrcFifoNo, unsigned uDstFifoNo, AllocChannel ** ppAC)
{
   unsigned u, k;
   unsigned uBarNo;
   int res;

   if( down_interruptible( &fifoSem))
      return -ERESTARTSYS;

   res = 0;
   if( !bIsDstFifoFree( allDevs[ uDstDevNo].perseus, uDstFifoNo)) {
      res = rtdxpkdrv_FIFO_BUSY;
      goto unlock;
   }
   if( !bIsSrcFifoFree( perseus, uSrcFifoNo)) {
      res = rtdxpkdrv_FIFO_BUSY;
      goto unlock;
   }

   // find a free channel:
   for( u = 0; u < MAX_LOGICAL_CHANNELS; u++) {
      if( !perseus->devChannels[ u].bAlloc)
         break;
   }
   if( u >= MAX_LOGICAL_CHANNELS) {
      PDEBUG( MYERR "All channels allocated!\n");
      res = rtdxpkdrv_NO_FREE_CHANNEL;
      goto unlock;
   }

   // get a free bar:
   // check if dest dev is already targeted by the src dev:
   for( k = 0; k < MAX_LOGICAL_CHANNELS; k++) {
      if( !perseus->devChannels[ k].bAlloc)
         continue;
      if( perseus->devChannels[ k].uChnlType != TYPE_DEV_TO_DEV)
         continue;
      if( perseus->devChannels[ k].u.dev2dev.uDstDevNo == uDstDevNo)
         break; // found
   }
   if( k >= MAX_LOGICAL_CHANNELS) {
      // allocate a new one:
      res = iGetFreeBarNo( perseus, &uBarNo);
      if( res)
         goto unlock;
   }
   else
      uBarNo = perseus->uDmBarNo[ perseus->devChannels[ k].u.dev2dev.uSrcFifoNo];
   perseus->uDmBarNo[ uSrcFifoNo] = uBarNo;
   perseus->uBarNoUseCount[ uBarNo]++;

   memset( &perseus->devChannels[ u], 0, sizeof perseus->devChannels[ u]);
   perseus->devChannels[ u].uChnlNo = u;
   perseus->devChannels[ u].uChnlType = TYPE_DEV_TO_DEV;
   perseus->devChannels[ u].u.dev2dev.uSrcFifoNo = uSrcFifoNo;
   perseus->devChannels[ u].u.dev2dev.uDstFifoNo = uDstFifoNo;
   perseus->devChannels[ u].u.dev2dev.uDstDevNo = uDstDevNo;
   perseus->devChannels[ u].bAlloc = 1;

   *ppAC = &perseus->devChannels[ u];
   PDEBUG( "dev[%u]-->dev[%u], uBarNo= %u, uSrcFifoNo= %u, uDstFifoNo= %u\n", perseus->uInstance, uDstDevNo, uBarNo, uSrcFifoNo, uDstFifoNo);
unlock:;
   up( &fifoSem);
   return res;
}



//  host --> dev
static int iAllocateFromHostFifo( PerseusData * perseus, unsigned uFifoNo, AllocChannel ** ppAC)
{
   unsigned uChnl, u;
   unsigned uBarNo;
   int res;

   if( down_interruptible( &fifoSem))
      return -ERESTARTSYS;

   res = 0;
   if( !bIsDstFifoFree( perseus, uFifoNo)) {
      res = rtdxpkdrv_FIFO_BUSY;
      goto unlock;
   }
   for( uChnl = 0; uChnl < MAX_LOGICAL_CHANNELS; uChnl++) {
      if( !perseus->devChannels[ uChnl].bAlloc)
         break;
   }
   if( uChnl >= MAX_LOGICAL_CHANNELS) {
      PDEBUG( MYERR "All channels allocated!\n");
      res = rtdxpkdrv_NO_FREE_CHANNEL;
      goto unlock;
   }
   
   // get a free bar:
   // check if host is already targeted by the dev:
   if( !perseus->uCdmaBarNoUseCount) {
      CdmaDesc * pCD;
      unsigned uCount;

      if( (res = iGetFreeBarNo( perseus, &uBarNo)))
         goto unlock;
      if( !perseus->sqCdma.buffer.pvBase) {
         dma_addr_t pphys;
         unsigned uDelta;

         // allocate data + CdmaDesc-Table buffers:
         perseus->sqCdma.buffer.uSize = cdmasizek * 1024 + NB_SQ_DESC_ENTRIES * sizeof( CdmaDesc);
         perseus->sqCdma.buffer.pvBase = pci_alloc_consistent( perseus->pdev, perseus->sqCdma.buffer.uSize,
                                                          &perseus->sqCdma.buffer.physAddr);
         if( perseus->sqCdma.buffer.pvBase == NULL) {
            PDEBUG( MYERR "Fail to allocate SQ Cdma buffers");
            res = rtdxpkdrv_DMA_ALLOC_FAIL;
            goto unlock;
         }
         perseus->sqCdma.dataQueue.pvBuffer = perseus->sqCdma.buffer.pvBase;
         perseus->sqCdma.dataQueue.physAddr = perseus->sqCdma.buffer.physAddr;
         perseus->sqCdma.dataQueue.uBuffSize = cdmasizek * 1024;
         PDEBUG( "SQ Cdma buffer: size= %u, virt= x%p, phys= x%p", perseus->sqCdma.dataQueue.uBuffSize,
                                          (void*)perseus->sqCdma.dataQueue.pvBuffer, (void*)perseus->sqCdma.dataQueue.physAddr);

         // setup descriptors table:
         pphys = perseus->sqCdma.buffer.physAddr + cdmasizek * 1024; // skip data zone
         // align to sizeof(CdmaDesc) bytes:
         uDelta = (sizeof(CdmaDesc) - pphys) & (sizeof(CdmaDesc)-1);
         perseus->sqCdma.descQueue.pvBuffer = (void*)(perseus->sqCdma.buffer.pvBase + cdmasizek * 1024 + uDelta);
         perseus->sqCdma.descQueue.physAddr = pphys + uDelta;
         uCount = NB_SQ_DESC_ENTRIES;
         if( uDelta)
            uCount--;
         perseus->sqCdma.descQueue.uBuffSize = uCount * sizeof(CdmaDesc);
         pCD = (CdmaDesc*)perseus->sqCdma.descQueue.pvBuffer;
         for( u = 0; u < uCount -1; u++)
            (pCD + u)->u.desc.next = (pCD + u +1);
         (pCD + u)->u.desc.next = pCD;

         PDEBUG( "SQ CdmaDesc-Table buffer: nb-desc= %u, virt= x%p, phys= x%p", uCount, 
                                 perseus->sqCdma.descQueue.pvBuffer, (void*)perseus->sqCdma.descQueue.physAddr);
      }
      perseus->uCdmaBarNo = uBarNo;
      perseus->uBarNoUseCount[ uBarNo] = 1;
      QUEUE_RESET( &perseus->sqCdma.dataQueue);

      pCD = (CdmaDesc*)perseus->sqCdma.descQueue.pvBuffer;
      uCount = perseus->sqCdma.descQueue.uBuffSize / sizeof(CdmaDesc);
      for( u = 0; u < uCount -1; u++) {
         (pCD + u)->u.desc.uNextDesc = AXIBAR_START_n( perseus->uCdmaBarNo)
                  + (( sizeof(*pCD) * (u +1) + perseus->sqCdma.descQueue.physAddr) & DMA_LOW_ADDR_MASK32);
      }
      (pCD + u)->u.desc.uNextDesc = AXIBAR_START_n( perseus->uCdmaBarNo)
                  + (perseus->sqCdma.descQueue.physAddr & DMA_LOW_ADDR_MASK32);
      QUEUE_RESET( &perseus->sqCdma.descQueue);
      perseus->sqCdma.uNbActDesc = 0;

      // setup AXIBAR:
      iowrite32( (u32)perseus->sqCdma.buffer.physAddr & DMA_HIGH_ADDR_MASK32, perseus->pcieBr + AXIBAR2PCIEBARL_n( perseus->uCdmaBarNo));
#ifdef IS_64BIT_PTR
      iowrite32( (u32)(perseus->sqCdma.buffer.physAddr >> 32), perseus->pcieBr + AXIBAR2PCIEBARH_n( perseus->uCdmaBarNo));
#endif
   }
   perseus->uCdmaBarNoUseCount++;

   memset( &perseus->devChannels[ uChnl], 0, sizeof perseus->devChannels[ uChnl]);
   perseus->devChannels[ uChnl].uChnlNo = uChnl;
   perseus->devChannels[ uChnl].uChnlType = TYPE_HOST_TO_DEV;
   perseus->devChannels[ uChnl].u.host.uDevNo = perseus->uInstance;
   perseus->devChannels[ uChnl].u.host.uFifoNo = uFifoNo;
   perseus->devChannels[ uChnl].bAlloc = 1;

   *ppAC = &perseus->devChannels[ uChnl];
   PDEBUG( "host-->dev[%u], uBarNo= %u, uFifoNo= %u\n", perseus->uInstance, uBarNo, uFifoNo);
unlock:;
   up( &fifoSem);
   return res;
}



// dev --> host
// fifo# is same as DM #
static int iAllocateToHostFifoDm( PerseusData * perseus, unsigned uDmAndFifoNo, AllocChannel ** ppAC)
{
    unsigned u;
    unsigned uBarNo;
    int res;
    unsigned uPeerDmAndFifoNo;

    if( down_interruptible( &fifoSem))
        return -ERESTARTSYS;

    res = 0;
    if( !bIsSrcFifoFree( perseus, uDmAndFifoNo)) {
        res = rtdxpkdrv_FIFO_BUSY;
        goto unlock;
    }
    for( u = 0; u < MAX_LOGICAL_CHANNELS; u++) {
        if( !perseus->devChannels[ u].bAlloc)
            break;
    }
    if( u >= MAX_LOGICAL_CHANNELS) {
        PDEBUG( MYERR "All channels allocated!\n");
        res = rtdxpkdrv_NO_FREE_CHANNEL;
        goto unlock;
    }

    if(99 == perseus->uDmBarNo[uDmAndFifoNo]){//DM not assigned
        //Find peer
		if(uDmAndFifoNo % 2){ //Odd-numbered FIFO
			uPeerDmAndFifoNo = uDmAndFifoNo - 1;
		}
		else{ //Even-numbered FIFO
			uPeerDmAndFifoNo = uDmAndFifoNo + 1;
		}
        if(99 == perseus->uDmBarNo[uPeerDmAndFifoNo]){ //Peer DM not assigned
            res = iGetFreeBarNo( perseus, &uBarNo);
            if( res)
                goto unlock;
            perseus->uDmBarNo[ uPeerDmAndFifoNo] = uBarNo;
            perseus->uBarNoUseCount[ uBarNo]++;
        }
        else{
            uBarNo = perseus->uDmBarNo[ uPeerDmAndFifoNo];
        }
		perseus->uDmBarNo[ uDmAndFifoNo] = uBarNo;
		perseus->uBarNoUseCount[ uBarNo]++;

        PDEBUG("Requested FIFO No %u assigned to BAR %u\n", uDmAndFifoNo, uBarNo);
        PDEBUG("Peer FIFO No %u assigned to BAR %u\n", uPeerDmAndFifoNo, uBarNo);
    }     

    memset( &perseus->devChannels[ u], 0, sizeof perseus->devChannels[ u]);
    perseus->devChannels[ u].uChnlNo = u;
    perseus->devChannels[ u].uChnlType = TYPE_DEV_TO_HOST;
    perseus->devChannels[ u].u.host.uDevNo = perseus->uInstance;
    perseus->devChannels[ u].u.host.uFifoNo = uDmAndFifoNo;
    perseus->devChannels[ u].bAlloc = 1;

    *ppAC = &perseus->devChannels[ u];
unlock:;
    up( &fifoSem);
    return res;
}



// !!!
// Setup Data Mover transfer
// Called by read(), ioct(), irq()
static int iSetupDmDev2Host( PerseusData * perseus, AllocChannel * pAC)
{
   u32 uTferCtrl;
   unsigned long uFlags;
   unsigned uFreeSpace;
   unsigned uBarNo;
   unsigned uDmNo;
   unsigned uFrameSize;
   int res;
   size_t left;

   res = 0;
   if( !pAC || !pAC->bAlloc) {
      PDEBUG( "not alloc");
      return rtdxpkdrv_INTERNAL_ERROR;
   }
   if( pAC->uState != ST_RUN) {
      PDEBUG( "txFifo is !running");
      return 0;
   }
   uDmNo = pAC->u.host.uFifoNo;
   if( !QUEUE_ALLOCATED( &perseus->txFifos[ uDmNo].queue)) {
      PDEBUG( MYERR "DM# (%u) buffer not allocated", uDmNo);
      return rtdxpkdrv_INTERNAL_ERROR;
   }

   spin_lock_irqsave( &perseus->dmSpinlock, uFlags);

   if( perseus->dmState[ uDmNo].bWorking) {
      PDEBUG( "DM still running\n");
      res = rtdxpkdrv_INTERNAL_ERROR;
      goto unlock;
   }

//   uFrameSize = -WORD_SIZE_B; // suppose continuous - remove 15aug13 by MSA
   uFrameSize = pAC->uFrameSize; // add 15aug13 by MSA - use high level frame size to have an irq for each frame
   if( pAC->transferSize != 0) { // !continuous
      left = pAC->transferSize - pAC->dmaXferedSize;
      if( left < uFrameSize)
         uFrameSize = (u32)left;
   }
   if( !uFrameSize) {
      PDEBUG( "all data received - Stopped");
      pAC->uState = ST_STOP_OK;
      goto unlock;
   }
   uFreeSpace = QUEUE_NOWRAP_FREE_SPACE( &perseus->txFifos[ uDmNo].queue);
   if( !uFreeSpace) {
      PDEBUG( "Dm# %u, rx DMA buffer: full", uDmNo);
      res = rtdxpkdrv_INTERNAL_ERROR;
      goto unlock;
   }
   if( uFrameSize > uFreeSpace)
      uFrameSize = uFreeSpace;
   if( uFrameSize > MAX_DM_TO_HOST_FRAME_SIZE)
      uFrameSize = MAX_DM_TO_HOST_FRAME_SIZE;
   uBarNo = perseus->uDmBarNo[ uDmNo];

   // setup DataMover transfer:
//   iowrite32( uChnlMsk, perseus->rtdex + RTDEX_TX_DM_CTRL); // reset DataMover-n
//   iowrite32( 0, perseus->rtdex + RTDEX_TX_DM_CTRL); // not-reset DataMover-n
   //Now done only once in the rtdex_open() function.
   // iowrite32( (u32)QUEUE_FREE_PHYSADDR( &perseus->txFifos[ uDmNo].queue) & DMA_HIGH_ADDR_MASK32, perseus->pcieBr + AXIBAR2PCIEBARL_n( uBarNo));
// #ifdef IS_64BIT_PTR
      // iowrite32( (u32)(QUEUE_FREE_PHYSADDR( &perseus->txFifos[ uDmNo].queue) >> 32), perseus->pcieBr + AXIBAR2PCIEBARH_n( uBarNo));
// #endif
   iowrite32( AXIBAR_START_n( uBarNo) + ((u32)QUEUE_FREE_PHYSADDR( &perseus->txFifos[ uDmNo].queue) & DMA_LOW_ADDR_MASK32), perseus->rtdex + RTDEX_TX_DM_ADDR_n( uDmNo)); // dst = PCIe BAR-n
   iowrite32( 0, perseus->rtdex + RTDEX_TX_DM_TAG_n( uDmNo));
   iowrite32( uFrameSize, perseus->rtdex + RTDEX_TX_DM_CTRL_n( uDmNo));
   iowrite32( 1, perseus->rtdex + RTDEX_TX_DM_TFRCNT_n( uDmNo)); // frame count

   perseus->dmState[ uDmNo].size = uFrameSize;
   PDEBUG( "--> setup to %u bytes, Dm# %u, physAddr= x%p", uFrameSize, uDmNo, (void*)QUEUE_FREE_PHYSADDR( &perseus->txFifos[ uDmNo].queue));

   iowrite32( IRQCTRL_EN_LAST_TFR_BM_all, perseus->rtdex + RTDEX_TX_DM_IRQ_CTRL); // enable int at last transfer

   uTferCtrl = ioread32( perseus->rtdex + RTDEX_TX_DM_TFR_CTRL);
   iowrite32( uTferCtrl & ~TFRCTRL_START_BM_n( uDmNo), perseus->rtdex + RTDEX_TX_DM_TFR_CTRL); // pulse start
   iowrite32( uTferCtrl | TFRCTRL_START_BM_n( uDmNo), perseus->rtdex + RTDEX_TX_DM_TFR_CTRL); // ..
   perseus->dmState[ uDmNo].bWorking = 1;

unlock:;
   spin_unlock_irqrestore( &perseus->dmSpinlock, uFlags);
   return res;
}



// !!!
// Setup Dev to Dev DM, source side
// Called by ioctl()
static int iSetupDmDev2DevSrc( PerseusData * perseus, AllocChannel * pAC)
{
   u32 uTferCtrl;
   unsigned long uFlags;
   int res;
   unsigned uFrameSize, uFramesCount;
   unsigned uDmNo, uBarNo;
   unsigned uRmteFifoNo;
   dma_addr_t destBaseAddrPhys;

   res = 0;
   uDmNo = pAC->u.dev2dev.uSrcFifoNo;
   spin_lock_irqsave( &perseus->dmSpinlock, uFlags);

   if( perseus->dmState[ uDmNo].bWorking) {
      PDEBUG( "DM still running!\n");
      res = rtdxpkdrv_INTERNAL_ERROR;
      goto unlock;
   }
   uBarNo = perseus->uDmBarNo[ pAC->u.dev2dev.uSrcFifoNo];
   uRmteFifoNo = pAC->u.dev2dev.uDstFifoNo;
   destBaseAddrPhys = allDevs[ pAC->u.dev2dev.uDstDevNo].perseus->fifoBaseAddrPhys;
   uFrameSize = pAC->uFrameSize;
   uFramesCount = (pAC->transferSize - pAC->dmaXferedSize) / uFrameSize;
   if( pAC->transferSize % uFrameSize)
      uFramesCount++;
   if( uFramesCount > MAX_DM_TFR_COUNT)
      uFramesCount = MAX_DM_TFR_COUNT;
   if( uFramesCount == 0 && pAC->transferSize) {
      PDEBUG( "No more data to transfer");
      res = -1;
      goto unlock;
   }
if( uFramesCount == 0)
 uFramesCount = MAX_DM_TFR_COUNT; // implement continuous mode with max count instead???

   // setup DataMover transfer:
//   iowrite32( uChnlMsk, perseus->rtdex + RTDEX_TX_DM_CTRL); // reset DataMover-n
//   iowrite32( 0, perseus->rtdex + RTDEX_TX_DM_CTRL); // not-reset DataMover-n

   iowrite32( (u32)destBaseAddrPhys & DMA_HIGH_ADDR_MASK32, perseus->pcieBr + AXIBAR2PCIEBARL_n( uBarNo));
#ifdef IS_64BIT_PTR
      iowrite32( (u32)(destBaseAddrPhys >> 32), perseus->pcieBr + AXIBAR2PCIEBARH_n( uBarNo));
#endif
   iowrite32( AXIBAR_START_n( uBarNo) + ((u32)destBaseAddrPhys & DMA_LOW_ADDR_MASK32) + FIFO_OFFSET_n( uRmteFifoNo),
              perseus->rtdex + RTDEX_TX_DM_ADDR_n( uDmNo)); // dst = PCIe BAR-n
   iowrite32( 0, perseus->rtdex + RTDEX_TX_DM_TAG_n( uDmNo));
   iowrite32( uFrameSize, perseus->rtdex + RTDEX_TX_DM_CTRL_n( uDmNo));

   iowrite32( uFramesCount, perseus->rtdex + RTDEX_TX_DM_TFRCNT_n( uDmNo));

   iowrite32( IRQCTRL_EN_LAST_TFR_BM_all, perseus->rtdex + RTDEX_TX_DM_IRQ_CTRL); // enable int at last transfer

   uTferCtrl = ioread32( perseus->rtdex + RTDEX_TX_DM_TFR_CTRL);
   uTferCtrl &= ~TFRCTRL_STREAM_BM_n( uDmNo);
   if( uFramesCount == 0) 
      uTferCtrl |= TFRCTRL_STREAM_BM_n( uDmNo);
   iowrite32( uTferCtrl & ~TFRCTRL_START_BM_n( uDmNo), perseus->rtdex + RTDEX_TX_DM_TFR_CTRL); // pulse start
   iowrite32( uTferCtrl | TFRCTRL_START_BM_n( uDmNo), perseus->rtdex + RTDEX_TX_DM_TFR_CTRL); // ..
   perseus->dmState[ uDmNo].bWorking = 1;
   perseus->dmState[ uDmNo].size = (size_t)uFrameSize * uFramesCount;

   PDEBUG( "--> Setup to %u bytes, frmCount= %u, physAddr= %p, uDmNo= %u, uDstFifoNo= %u, uBarNo= %u", uFrameSize, uFramesCount, (void*)destBaseAddrPhys, uDmNo, uRmteFifoNo, uBarNo);
unlock:;
   spin_unlock_irqrestore( &perseus->dmSpinlock, uFlags);
   return res;
}


// !!!
// Check if it's time to schedule CDMA duty
// return 0 if OK, -1 if nothing setup, else error code.
// Called by write(), ioctl(), irq()
static int iSetupCdmaSg( PerseusData * perseus)
{
   unsigned long uFlags;
   unsigned uSR;
//   unsigned u, uTmp;
   int res;
   unsigned uCount;

   if( !QUEUE_ALLOCATED( &perseus->sqCdma.dataQueue))
      return -1;

   res = 0;
   spin_lock_irqsave( &perseus->cdmaSpinlock, uFlags);

   if( perseus->sqCdma.bWorking) {
      PDEBUG( "Cdma still working");
      res = -1;
      goto out_unlock;
   }
/*
   if( !QUEUE_BUSY_SPACE( &perseus->sqCdma.dataQueue)
          ||
   (QUEUE_FREE_SPACE( &perseus->sqCdma.dataQueue) > 0
   && QUEUE_BUSY_SPACE( &perseus->sqCdma.dataQueue) < perseus->sqCdma.uFlushThreshold
   && QUEUE_FREE_SPACE( &perseus->sqCdma.descQueue) > 0
   && !perseus->sqCdma.bForceFlush
   && !perseus->sqCdma.bDelayTrig)
   ) {
      PDEBUG( "Threshold point not met");
      goto out_unlock;
   }
*/

   if( !QUEUE_BUSY_SPACE( &perseus->sqCdma.descQueue)) {
      PDEBUG( "No busy desc");
      perseus->sqCdma.bForceFlush = 0;
      perseus->sqCdma.bDelayTrig = 0;
      goto out_unlock;
   }

   if( perseus->sqCdma.bForceFlush || perseus->sqCdma.bDelayTrig 
   || 0 == QUEUE_FREE_SPACE( &perseus->sqCdma.descQueue)
   || 0 == QUEUE_FREE_SPACE( &perseus->sqCdma.dataQueue) )
      goto setup;

   if( QUEUE_BUSY_SPACE( &perseus->sqCdma.dataQueue) < perseus->sqCdma.uFlushThreshold) {
      PDEBUG( "Threshold point not met - start timer");
      // start timer???
      goto out_unlock;
   }

setup:;
   // sanity check:
   uSR = ioread32( perseus->cdma + CDMASR);
   if( !(uSR & CDMASR_IDLE_BM)) {
      PDEBUG( MYERR "Cdma busy\n");
      perseus->sqCdma.uBusyErrors++;
      res = rtdxpkdrv_INTERNAL_ERROR;
      goto out_unlock;
   }

   perseus->sqCdma.bForceFlush = 0;
   perseus->sqCdma.bDelayTrig = 0;

   iowrite32( 0, perseus->cdma + CDMACR); // out of SG mode

   uCount = QUEUE_BUSY_SPACE( &perseus->sqCdma.descQueue) / sizeof( CdmaDesc);
   if( uCount >= 1 << CDMACR_IRQ_TRESHOLD_BC)
      uCount = (1 << CDMACR_IRQ_TRESHOLD_BC) -1;

   iowrite32( CDMACR_SGMODE_BM | CDMACR_ERR_IRQEN_BM | CDMACR_IOC_IRQEN_BM | /*CDMACR_DLY_IRQEN_BM | */ (uCount << CDMACR_IRQ_TRESHOLD_BN)
      /*| (255 << CDMACR_IRQ_DELAY_BN)*/, perseus->cdma + CDMACR); // enable irqs, sg mode
   iowrite32( AXIBAR_START_n( perseus->uCdmaBarNo) + (DMA_LOW_ADDR_MASK32 & QUEUE_BUSY_PHYSADDR( &perseus->sqCdma.descQueue)),
      perseus->cdma + CDMACURDESC); // set current desc ptr
   iowrite32( AXIBAR_START_n( perseus->uCdmaBarNo) + (DMA_LOW_ADDR_MASK32 & QUEUE_BUSY_PHYSADDR_N( &perseus->sqCdma.descQueue, (uCount-1)*sizeof(CdmaDesc))),
      perseus->cdma + CDMATAILDESC); // set tail desc ptr, start
#ifdef PERSEUS_DEBUG
   {
      CdmaDesc * pCD = QUEUE_BUSY_PTR( &perseus->sqCdma.descQueue);
      unsigned uSize = 0;
      unsigned n;
      for( n = 0; n < uCount; n++) {
         uSize += pCD->u.desc.uControl & SGDESC_CTRL_BTT_BM;
         pCD = pCD->u.desc.next;
      }
      PDEBUG( "inst= %u, head= x%X, tail= x%X, count= %u, bytes= %u", perseus->uInstance, 
         AXIBAR_START_n( perseus->uCdmaBarNo) + (DMA_LOW_ADDR_MASK32 & QUEUE_BUSY_PHYSADDR( &perseus->sqCdma.descQueue)),
         AXIBAR_START_n( perseus->uCdmaBarNo) + (DMA_LOW_ADDR_MASK32 & QUEUE_BUSY_PHYSADDR_N( &perseus->sqCdma.descQueue, (uCount-1)*sizeof(CdmaDesc))),
         uCount, uSize);
   }
#endif
   perseus->sqCdma.uNbActDesc = uCount;
   perseus->sqCdma.bWorking = 1;
   perseus->sqCdma.bForceFlush = 0;

out_unlock:;
   spin_unlock_irqrestore( &perseus->cdmaSpinlock, uFlags);
   return res;
}



// Called by release(), ioctl()
static void UnconnectChannel( PerseusData * perseus, AllocChannel * pAC)
{
   if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
      if( 0 == --perseus->uCdmaBarNoUseCount) {
         perseus->uBarNoUseCount[ perseus->uCdmaBarNo] = 0;
         perseus->uCdmaBarNo = 99; // sanity
      }
      perseus->rxFifos[ pAC->u.host.uFifoNo].pAC = NULL;
   }
   else if( pAC->uChnlType == TYPE_DEV_TO_HOST) {
      perseus->uBarNoUseCount[ perseus->uDmBarNo[ pAC->u.host.uFifoNo]]--;
      perseus->uDmBarNo[ pAC->u.host.uFifoNo] = 99; // sanity
      perseus->dmState[ pAC->u.host.uFifoNo].size = 0; // sanity
      perseus->txFifos[ pAC->u.host.uFifoNo].pAC = NULL;
   }
   else /*if( pAC->uChnlType == TYPE_DEV_TO_DEV)*/ {
      perseus->uBarNoUseCount[ perseus->uDmBarNo[ pAC->u.dev2dev.uSrcFifoNo]]--;
      perseus->uDmBarNo[ pAC->u.dev2dev.uSrcFifoNo] = 99; // sanity
      perseus->dmState[ pAC->u.dev2dev.uSrcFifoNo].size = 0; // sanity
      perseus->txFifos[ pAC->u.dev2dev.uSrcFifoNo].pAC = NULL;
   }
   pAC->bAlloc = 0;
}



// Called by iStopChannel(), ioctl()
static int iResetDm( PerseusData * perseus, unsigned uDmNo)
{
   int res;
   unsigned long uFlags;
   unsigned u, uVal;

   res = 0;
   spin_lock_irqsave( &perseus->dmSpinlock, uFlags);
   iowrite32( 0, perseus->rtdex + RTDEX_TX_DM_TFRCNT_n( uDmNo)); //???
   iowrite32( DMCTRL_SHUTDOWN_BM_n( uDmNo), perseus->rtdex + RTDEX_TX_DM_CTRL);
   for( u = 50; u > 0; u--) { // each ioread() is +- 2 uS, so wait 100 uSec max
      uVal = ioread32( perseus->rtdex + RTDEX_TX_DM_CTRL);
      if( uVal & DMCTRL_HALTED_BM_n( uDmNo))
         break;
   }
   if( !u) {
      PDEBUG( MYERR "Timeout halting DM!\n");
      res = rtdxpkdrv_DMRESET_TIMEOUT;
   }
   else {
      iowrite32( 0, perseus->rtdex + RTDEX_TX_DM_CTRL);
      iowrite32( DMCTRL_RESET_BM_n( uDmNo), perseus->rtdex + RTDEX_TX_DM_CTRL);
      iowrite32( 0, perseus->rtdex + RTDEX_TX_DM_CTRL);
   }
   perseus->dmState[ uDmNo].bWorking = 0;
   spin_unlock_irqrestore( &perseus->dmSpinlock, uFlags);
   return 0;
}



// Called by release(), ioctl()
static int iStopChannel( PerseusData * perseus, AllocChannel * pAC)
{
   int res;

   res = 0;
   if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
      // if all CDMA channels are !run, the kill CDMA:  ???
//      iowrite32( CDMACR_RESET_BM, perseus->cdma + CDMACR); // reset CDMA
   }
   else if( pAC->uChnlType == TYPE_DEV_TO_HOST) {
      res = iResetDm( perseus, pAC->u.host.uFifoNo);
   }
   else /*if( pAC->uChnlType == TYPE_DEV_TO_DEV)*/ {
      res = iResetDm( perseus, pAC->u.dev2dev.uSrcFifoNo);
   }
   return res;
}



//!!!
static int rtdex_open( struct inode * inode, struct file * file)
{
   PerseusData *perseus = container_of( inode->i_cdev, PerseusData, rtdex_cdev);
   unsigned uFifoNo, 
      firstFifoNumber, 
      secondFifoNumber;
   unsigned buffSize;
   unsigned uBarNo;

   PDEBUG( "uInstance= %u, CPU= %u, MAJ= %u, MIN= %u\n", perseus->uInstance, smp_processor_id(), imajor( inode), iminor( inode));
   uFifoNo = iminor( inode);
   if( uFifoNo == MINOR_GLOBAL) {
      perseus->uGlobalOpen++;
   }
   else if( uFifoNo >= NB_FIFOS) {
      // tx fifo
      PDEBUG( "tx-fifo");
      uFifoNo -= NB_FIFOS;
      if( !perseus->txFifos[ uFifoNo].bOpen) 
      {
          if(uFifoNo % 2) //Odd-numbered FIFO
          {
              firstFifoNumber = uFifoNo - 1;
              secondFifoNumber = uFifoNo;
          }
          else
          {
              firstFifoNumber = uFifoNo;
              secondFifoNumber = uFifoNo + 1;
          }
          // alloc TX buffer:
          //Pre-allocate both odd and even numbered buffers now
          buffSize = dmsizek * 1024;
          perseus->txFifos[ firstFifoNumber].queue.pvBuffer = pci_alloc_consistent( perseus->pdev, 2*buffSize, &perseus->txFifos[ firstFifoNumber].queue.physAddr);
          if( perseus->txFifos[ firstFifoNumber].queue.pvBuffer == NULL) {
              PDEBUG( MYERR "Fail to allocate DMA buffer for rx queue %u", firstFifoNumber);
              return -EFAULT;
          }
          perseus->txFifos[ firstFifoNumber].queue.uBuffSize = buffSize;
          QUEUE_RESET( &perseus->txFifos[ firstFifoNumber].queue);
          perseus->txFifos[ firstFifoNumber].bOpen = 1;
          
          perseus->txFifos[ secondFifoNumber].queue.physAddr = buffSize + perseus->txFifos[ firstFifoNumber].queue.physAddr;
          perseus->txFifos[ secondFifoNumber].queue.uBuffSize = buffSize;
          perseus->txFifos[ secondFifoNumber].queue.pvBuffer = buffSize + perseus->txFifos[ firstFifoNumber].queue.pvBuffer;
          QUEUE_RESET( &perseus->txFifos[ secondFifoNumber].queue);
          perseus->txFifos[ secondFifoNumber].bOpen = 1;
          PDEBUG("txFifo%u, DMA-BUFF physAddr= x%p\n", firstFifoNumber, (void*)perseus->txFifos[ firstFifoNumber].queue.physAddr);
          PDEBUG("txFifo%u, DMA-BUFF physAddr= x%p\n", secondFifoNumber, (void*)perseus->txFifos[ secondFifoNumber].queue.physAddr);

          uBarNo = perseus->uDmBarNo[ firstFifoNumber];
          //Setup AXIBAR
          iowrite32( (u32)perseus->txFifos[ firstFifoNumber].queue.physAddr & DMA_HIGH_ADDR_MASK32, perseus->pcieBr + AXIBAR2PCIEBARL_n( uBarNo));
#ifdef IS_64BIT_PTR
          iowrite32( (u32)(perseus->txFifos[ firstFifoNumber].queue.physAddr >> 32), perseus->pcieBr + AXIBAR2PCIEBARH_n( uBarNo));
#endif
      }
      //else: Fifo has been opened when its peer got opened
   }
   else {
      // rx fifo
      PDEBUG( "rx-fifo");
      if( perseus->rxFifos[ uFifoNo].bOpen) {
         PDEBUG( MYERR "Already open!\n");
         return -EFAULT;
      }
      perseus->rxFifos[ uFifoNo].bOpen = 1;
   }

   file->private_data = perseus;
   return 0;
}


//!!!
static int rtdex_release( struct inode * inode, struct file * file)
{
	PerseusData *perseus = (PerseusData*)file->private_data;
   unsigned u, uFifoNo;

   PDEBUG( "uInstance= %u, CPU= %u, MAJ= %u, MIN= %u\n", perseus->uInstance, smp_processor_id(), imajor( inode), iminor( inode));
   if( down_interruptible( &perseus->rdSem))
      return -ERESTARTSYS;
   if( down_interruptible( &perseus->wrSem))
      return -ERESTARTSYS;
   uFifoNo = iminor( inode);
   if( uFifoNo == MINOR_GLOBAL) {
      perseus->uGlobalOpen--;
      if( !perseus->uGlobalOpen) {
         // remove any connected channel
         for( u = 0; u < MAX_LOGICAL_CHANNELS; u++) {
            if( perseus->devChannels[ u].bAlloc) {
               iStopChannel( perseus, &perseus->devChannels[ u]);
               UnconnectChannel( perseus, &perseus->devChannels[ u]);
            }
         }
      }
   }
   else if( uFifoNo >= NB_FIFOS) {
      // tx-fifo
      uFifoNo -= NB_FIFOS;
      if( perseus->txFifos[ uFifoNo].queue.pvBuffer) {
         QUEUE_RESET( &perseus->txFifos[ uFifoNo].queue); // sanity
         pci_free_consistent( perseus->pdev, perseus->txFifos[ uFifoNo].queue.uBuffSize, perseus->txFifos[ uFifoNo].queue.pvBuffer, perseus->txFifos[ uFifoNo].queue.physAddr);
         perseus->txFifos[ uFifoNo].queue.pvBuffer = NULL;
      }
      perseus->txFifos[ uFifoNo].bOpen = 0;
   }
   else {
      perseus->rxFifos[ uFifoNo].bOpen = 0;
   }
   up( &perseus->wrSem);
   up( &perseus->rdSem);
   PDEBUG( "done");
   return 0;
}


//!!!
static ssize_t rtdex_read( struct file *file, char *buff, size_t count, loff_t *ppos)
{	
   
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
    struct inode *inode = file->f_path.dentry->d_inode;  
#else
    struct inode *inode = file->f_dentry->d_inode;
#endif
    PerseusData *perseus = (PerseusData*)file->private_data;
   unsigned uPart;
   unsigned uFifoNo;
   AllocChannel * pAC;
   unsigned uMaxTry, uDone;

   PDEBUG( "uInstance= %u, CPU= %u, MAJ= %u, MIN= %u\n", perseus->uInstance, smp_processor_id(), imajor( inode), iminor( inode));

   uFifoNo = iminor( inode) -NB_FIFOS;
   if( uFifoNo >= NB_FIFOS) {
      PDEBUG( "Invalid minor# for read");
      return -EFAULT;
   }
   pAC = perseus->txFifos[ uFifoNo].pAC;
   if( !pAC) {
      PDEBUG( MYERR "Not connected");
      return -EFAULT;
   }

//   if( !pAC->bRun && QUEUE_BUSY_SPACE( &perseus->txFifos[ uFifoNo].queue) == 0) {
   if( pAC->hostXferedSize >= pAC->transferSize && pAC->transferSize) {
      PDEBUG( "EOF");
      return 0;
   }

   count &= ~(WORD_SIZE_B-1);

   uDone = 0;
   uMaxTry = 1;
   while( count > 0  && uMaxTry-- > 0) {
      if( down_interruptible( &perseus->rdSem))
         return -ERESTARTSYS;
      while( (uPart = QUEUE_NOWRAP_BUSY_SPACE( &perseus->txFifos[ uFifoNo].queue)) == 0) { /* nothing to read */
         up( &perseus->rdSem); /* release the lock */
         if( file->f_flags & O_NONBLOCK) {
            PDEBUG( MYERR "Non block io..\n");
            return -EAGAIN;
         }
         PDEBUG("Dm# %u, '%s' going to sleep\n", pAC->u.host.uFifoNo, current->comm);
         if( wait_event_interruptible( perseus->inq, (QUEUE_NOWRAP_BUSY_SPACE( &perseus->txFifos[ uFifoNo].queue) != 0)))
            return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
         PDEBUG( "Dm# %u, '%s' out of sleep\n", pAC->u.host.uFifoNo, current->comm);
         /* otherwise loop, but first reacquire the lock */
         if( down_interruptible( &perseus->rdSem))
            return -ERESTARTSYS;
      }

      if( uPart > count)
         uPart = count;
      PDEBUG( "Dm# %u, copy %u bytes from %p", pAC->u.host.uFifoNo, uPart, QUEUE_BUSY_PTR( &perseus->txFifos[ uFifoNo].queue));
      if( copy_to_user( buff + uDone, QUEUE_BUSY_PTR( &perseus->txFifos[ uFifoNo].queue), uPart)) {
         up( &perseus->rdSem); /* release the lock */
         return -EFAULT;
      }
      pAC->hostXferedSize += uPart;
      QUEUE_GET( &perseus->txFifos[ uFifoNo].queue, uPart);
      PDEBUG( "Dm# %u, space busy= %u, free= %u", pAC->u.host.uFifoNo, QUEUE_BUSY_SPACE( &perseus->txFifos[ uFifoNo].queue), QUEUE_FREE_SPACE( &perseus->txFifos[ uFifoNo].queue));

      up( &perseus->rdSem); /* release the lock */

      // try to receive something:
      iSetupDmDev2Host( perseus, pAC);

      uDone += uPart;
      count -= uPart;
   } // while(

   *ppos += uDone;
   PDEBUG( "Dm# %u, uDone= %u", pAC->u.host.uFifoNo, uDone);
   return uDone;
}




//!!!
static ssize_t rtdex_write( struct file *file, const char *buff, size_t count, loff_t *ppos)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
    struct inode *inode = file->f_path.dentry->d_inode;  
#else
    struct inode *inode = file->f_dentry->d_inode;
#endif
	PerseusData *perseus = (PerseusData*)file->private_data;
   unsigned uPart;
   unsigned uFifoNo;
   unsigned uDone;
   AllocChannel * pAC;
   unsigned uMaxTry;

   PDEBUG( "uInstance= %u, CPU= %u, MAJ= %u, MIN= %u\n", perseus->uInstance, smp_processor_id(), imajor( inode), iminor( inode));
   PDEBUG( "count= %lu", (unsigned long)count);

   uFifoNo = iminor( inode);
   if( uFifoNo >= NB_FIFOS) {
      PDEBUG( MYERR "Invalid minor# for write");
      return -EFAULT;
   }
   pAC = perseus->rxFifos[ uFifoNo].pAC;
   if( !pAC || pAC->uState != ST_RUN) {
      PDEBUG( MYERR "Not connected and/or Not started");
      return -EINVAL;
   }

   if( pAC->hostXferedSize >= pAC->transferSize && pAC->transferSize) {
      PDEBUG( "EOF");
      return 0;
   }

   uDone = 0;
   uMaxTry = MAX_USER_FRAME_SIZE / MAX_CDMA_FRAME_SIZE + 1;
   while( count > 0  && uMaxTry-- > 0) {
      if( down_interruptible( &perseus->wrSem))
         return -ERESTARTSYS;
      while( 0 == (uPart = QUEUE_NOWRAP_FREE_SPACE( &perseus->sqCdma.dataQueue))
      || 0 == QUEUE_FREE_SPACE( &perseus->sqCdma.descQueue)) { // full!
         DEFINE_WAIT( wait);
         up( &perseus->wrSem);
         if( file->f_flags & O_NONBLOCK) {
            PDEBUG( MYERR "Non block IO..\n");
            return -EAGAIN;
         }
         PDEBUG( "FIFO# %u, '%s' going to sleep\n", pAC->u.host.uFifoNo, current->comm);
         prepare_to_wait( &perseus->outq, &wait, TASK_INTERRUPTIBLE);
         if( 0 == (uPart = QUEUE_NOWRAP_FREE_SPACE( &perseus->sqCdma.dataQueue))
         || 0 == QUEUE_FREE_SPACE( &perseus->sqCdma.descQueue))
            schedule();
         finish_wait( &perseus->outq, &wait);
         PDEBUG( "FIFO# %u, '%s' out of sleep\n", pAC->u.host.uFifoNo, current->comm);
         if( signal_pending( current))
            return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
         if( down_interruptible( &perseus->wrSem))
            return -ERESTARTSYS;
      }

      if( uPart > count)
         uPart = count;
      if( uPart > MAX_CDMA_FRAME_SIZE)
         uPart = MAX_CDMA_FRAME_SIZE;

      PDEBUG( "copy %u bytes to %p", uPart, QUEUE_FREE_PTR( &perseus->sqCdma.dataQueue));
      if( 0 == copy_from_user( QUEUE_FREE_PTR( &perseus->sqCdma.dataQueue), buff + uDone, uPart)) {
         CdmaDesc * p = (CdmaDesc*)QUEUE_FREE_PTR( &perseus->sqCdma.descQueue);
         //   PDEBUG( "setupping Cdma desc p= %p", p);
         p->u.desc.uSrcAddr = AXIBAR_START_n( perseus->uCdmaBarNo)
                              +((u32)QUEUE_FREE_PHYSADDR( &perseus->sqCdma.dataQueue) & DMA_LOW_ADDR_MASK32);
         p->u.desc.uDstAddr = FIFO_BASE_ADDR + FIFO_OFFSET_n( uFifoNo);
         p->u.desc.uControl = uPart & SGDESC_CTRL_BTT_BM;
         p->u.desc.uFifoNo = uFifoNo;
         p->u.desc.uStatus = 0;
         QUEUE_PUT( &perseus->sqCdma.descQueue, sizeof(CdmaDesc));
         PDEBUG( "descQueue: busy cnt= %u, free cnt= %u", QUEUE_BUSY_SPACE( &perseus->sqCdma.descQueue) / sizeof(CdmaDesc), QUEUE_FREE_SPACE( &perseus->sqCdma.descQueue) / sizeof(CdmaDesc));
         QUEUE_PUT( &perseus->sqCdma.dataQueue, uPart);
         pAC->hostXferedSize += uPart;
      }
      else {
         up( &perseus->wrSem);
         return -EFAULT;
      }
      PDEBUG( "dataQueue: space busy= %u, free= %u", QUEUE_BUSY_SPACE( &perseus->sqCdma.dataQueue), QUEUE_FREE_SPACE( &perseus->sqCdma.dataQueue));

      up( &perseus->wrSem);
      iSetupCdmaSg( perseus);

      uDone += uPart;
      count -= uPart;
   } // while

   *ppos += uDone;
   return uDone;
}


/*
// Called by ioctl()
static int iEnableRxFifo( PerseusData * perseus, AllocChannel * pAC, unsigned bWr, unsigned bRd)
{
   void * pRegs;
   unsigned uVal;
   unsigned uFifoNo;

   if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
      uFifoNo = pAC->u.host.uFifoNo;
      pRegs = perseus->rtdex;
      spin_lock( &perseus->miscRegsSpinlock);
   }
   else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
      uFifoNo = pAC->u.dev2dev.uDstFifoNo;
      pRegs = allDevs[ pAC->u.dev2dev.uDstDevNo].perseus->rtdex;
      PDEBUG( "pAC->u.dev2dev.uDstDevNo= %u", pAC->u.dev2dev.uDstDevNo);
      // read then write access to other device - need protection:
      spin_lock( &allDevs[ pAC->u.dev2dev.uDstDevNo].perseus->miscRegsSpinlock);
   }
   else {
      PDEBUG( MYERR "Bad channel type");
      return rtdxpkdrv_BAD_CHANNEL_TYPE;
   }

   PDEBUG( "pRegs= x%p", (void*)pRegs);
   uVal = ioread32( pRegs + RTDEX_FIFO_ENABLE);
   uVal &= ~FIFOEN_RXRD_BM_n( uFifoNo) | ~FIFOEN_RXWR_BM_n( uFifoNo);
   uVal |= (bRd ? FIFOEN_RXRD_BM_n( uFifoNo) : 0) | (bWr ? FIFOEN_RXWR_BM_n( uFifoNo) : 0);
   iowrite32( uVal, pRegs + RTDEX_FIFO_ENABLE);
   if( pAC->uChnlType == TYPE_DEV_TO_DEV)
      spin_unlock( &allDevs[ pAC->u.dev2dev.uDstDevNo].perseus->miscRegsSpinlock);
   else
      spin_unlock( &perseus->miscRegsSpinlock);
   return 0;
   }
*/



// Enable/Disable RX-FIFOs "SIMULTANEOUSLY", but one device at time.
// Called by ioctl()
static int iEnableRxFifoMask( PerseusData * perseus, unsigned uChnlMask_, unsigned bWr, unsigned bRd)
{
   unsigned uVal, uClear, uSet;
   unsigned uFifoNo;
   unsigned uDevNo;
   unsigned uChnlNo;
   unsigned uChnlMask;
   AllocChannel * pAC;

   for( uDevNo = 0; uDevNo < atomic_read( &perseus_count); uDevNo++) {
      uClear = uSet = 0;
      uChnlMask = uChnlMask_;
      for( uChnlNo = 0; uChnlNo < MAX_LOGICAL_CHANNELS && uChnlMask; uChnlNo++, uChnlMask >>= 1) {
         if( !(uChnlMask & 1))
            continue;
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected, uChnlNo= %u", uChnlNo);
            return rtdxpkdrv_CHANNEL_NOT_CONNECTED;
         }
         if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
            if( perseus->uInstance != uDevNo)
               continue;
            uFifoNo = pAC->u.host.uFifoNo;
         }
         else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
            if( pAC->u.dev2dev.uDstDevNo != uDevNo)
               continue;
            uFifoNo = pAC->u.dev2dev.uDstFifoNo;
         }
         else {
            PDEBUG( MYERR "Bad channel type, uChnlNo= %u", uChnlNo);
            return rtdxpkdrv_BAD_CHANNEL_TYPE;
         }
         uClear |= FIFOEN_RXRD_BM_n( uFifoNo) | FIFOEN_RXWR_BM_n( uFifoNo);
         uSet |= (bRd ? FIFOEN_RXRD_BM_n( uFifoNo) : 0) | (bWr ? FIFOEN_RXWR_BM_n( uFifoNo) : 0);
      }
      spin_lock( &allDevs[ uDevNo].perseus->miscRegsSpinlock);
      uVal = ioread32( allDevs[ uDevNo].perseus->rtdex + RTDEX_FIFO_ENABLE);
      iowrite32( (uVal & ~uClear) | uSet, allDevs[ uDevNo].perseus->rtdex + RTDEX_FIFO_ENABLE);
      spin_unlock( &allDevs[ uDevNo].perseus->miscRegsSpinlock);
   }
   return 0;
}


/*
// TX device IS local instance
// Called by ioctl()
static int iEnableTxFifo( PerseusData * perseus, AllocChannel * pAC, unsigned bWr, unsigned bRd)
{
   unsigned uVal;
   unsigned uFifoNo;

   if( pAC->uChnlType == TYPE_DEV_TO_HOST) {
      uFifoNo = pAC->u.host.uFifoNo;
   }
   else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
      uFifoNo = pAC->u.dev2dev.uDstFifoNo;
   }
   else {
      PDEBUG( MYERR "Bad channel type");
      return rtdxpkdrv_BAD_CHANNEL_TYPE;
   }

   uVal = ioread32( perseus->rtdex + RTDEX_FIFO_ENABLE);
   uVal &= ~FIFOEN_TXWR_BM_n( uFifoNo); // | ~FIFOEN_TXRD_BM_n( uFifoNo);
   uVal |= (bWr ? FIFOEN_TXWR_BM_n( uFifoNo) : 0); // | (uRd ? FIFOEN_TXRD_BM_n( uFifoNo) : 0);
   iowrite32( uVal, perseus->rtdex + RTDEX_FIFO_ENABLE);
   return 0;
}
*/


// Enable/Disable TX-FIFOs "SIMULTANEOUSLY"
// TX device IS local instance
// Called by ioctl()
static int iEnableTxFifoMask( PerseusData * perseus, unsigned uChnlMask, unsigned bWr, unsigned bRd)
{
   AllocChannel * pAC;
   unsigned uVal;
   unsigned uChnlNo;
   unsigned uFifoNo;
   unsigned uClear, uSet;

   uSet = uClear = 0;
   for( uChnlNo = 0; uChnlNo < MAX_LOGICAL_CHANNELS && uChnlMask; uChnlNo++, uChnlMask >>= 1) {
      if( !(uChnlMask & 1))
         continue;
      pAC = &perseus->devChannels[ uChnlNo];
      if( !pAC || !pAC->bAlloc) {
         PDEBUG( MYERR "Not connected, uChnlNo= %u", uChnlNo);
         return rtdxpkdrv_CHANNEL_NOT_CONNECTED;
      }
      if( pAC->uChnlType == TYPE_DEV_TO_HOST) {
         uFifoNo = pAC->u.host.uFifoNo;
      }
      else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
         uFifoNo = pAC->u.dev2dev.uDstFifoNo;
      }
      else {
         PDEBUG( MYERR "Bad channel type, uChnlNo= %u", uChnlNo);
         return rtdxpkdrv_BAD_CHANNEL_TYPE;
      }
      uClear |= /*~FIFOEN_TXRD_BM_n( uFifoNo) | */ FIFOEN_TXWR_BM_n( uFifoNo);
      uSet |= /*(uRd ? FIFOEN_TXRD_BM_n( uFifoNo) : 0) | */ (bWr ? FIFOEN_TXWR_BM_n( uFifoNo) : 0);
   }
   uVal = ioread32( perseus->rtdex + RTDEX_FIFO_ENABLE);
   iowrite32( (uVal & ~uClear) | uSet, perseus->rtdex + RTDEX_FIFO_ENABLE);
   return 0;
}



// !!!
static long rtdex_unlocked_ioctl( struct file * file, unsigned int ioctlNum, unsigned long ioctlParam)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
    struct inode *inode = file->f_path.dentry->d_inode;  
#else
    struct inode *inode = file->f_dentry->d_inode;
#endif
   PerseusData * perseus = (PerseusData*)file->private_data;
   u32 au32Req[ 6];
   u32 au32Resp[ 6];
   int res, iRetLen;
   unsigned uInWords, uOutWords;
   unsigned uTmp, * pu32;
   unsigned uRtdexLastError;
   unsigned long uFlags;

   PDEBUG( "uInstance= %u, CPU= %u, MAJ= %u, MIN= %u\n", perseus->uInstance, smp_processor_id(), imajor( inode), iminor( inode));

// from host/dev: PCIe(cdma/dm) [En]--> RX-FIFO [En]--> userLogic
// from dev:      userLogic [En]--> TX-FIFO --> (dm)PCIe

   if( iminor( inode) != MINOR_GLOBAL) {
      PDEBUG( MYERR "Not global minor#");
      return -EBADF;
   }
   if( (ioctlNum & 0xFFFF) != DEFAULT_CTLIO_NO) {
      PDEBUG( "Invalid ioctlNum");
      return -EINVAL;
   }
   uInWords = (ioctlNum >> 24) & 0xff;
   uOutWords = (ioctlNum >> 16) & 0xff;

   if( uInWords < 1) {
      PDEBUG( MYERR "no cmd"); 
      uRtdexLastError = rtdxpkdrv_NO_COMMAND;
      if( copy_to_user( (void*)ioctlParam, &uRtdexLastError, sizeof uRtdexLastError))
         ;
      return -EINVAL;
   }
   if( uInWords * sizeof( u32) > sizeof au32Req) {
      PDEBUG( MYERR "in buffer too big (%lu)", (unsigned long)uInWords * sizeof( u32)); 
      uRtdexLastError = rtdxpkdrv_INPUT_BUFFER_TOO_BIG;
      if( copy_to_user( (void*)ioctlParam, &uRtdexLastError, sizeof uRtdexLastError))
         ;
      return -EINVAL;
   }
   if( copy_from_user( au32Req, (char*)ioctlParam, uInWords * sizeof( u32)))
      return -EFAULT;

   pu32 = au32Req;
   PDEBUG( "ioctlNum= %x, data= (x%X x%X x%X x%X x%X) ", ioctlNum, uInWords >= 1 ? *(pu32+0) : -1,
      uInWords >= 2 ? *(pu32+1) : -1, uInWords >= 3 ? *(pu32+2) : -1, uInWords >= 4 ? *(pu32+3) : -1,
      uInWords >= 5 ? *(pu32+4) : -1 );
   memset( au32Resp, 0, sizeof au32Resp);

   if( down_interruptible( &perseus->ctlSem))
      return -ERESTARTSYS;
   if( down_interruptible( &perseus->rdSem))
      return -ERESTARTSYS;
   if( down_interruptible( &perseus->wrSem)) {
      up( &perseus->rdSem);
      return -ERESTARTSYS;
   }

   uRtdexLastError = rtdxpkdrv_NO_ERROR;
   res = 0;
   iRetLen = 0;
   switch( *(pu32+0)) {
      case IOCTL_GET_CORE_IDVERS:
         // (cmd) --> (id, ver)
         uTmp = ioread32( perseus->rtdex + RTDEX_CORE_ID_VERS);
         au32Resp[0] = (uTmp & COREID_BM) >> COREID_BO;
         au32Resp[1] = (uTmp & COREVER_BM) >> COREVER_BO;
         iRetLen = 2 * sizeof(u32);
         break;
      case IOCTL_RESET_CORE:
         // (cmd)
         iowrite32( 1 << CTRLSTAT_RESET_BN, perseus->rtdex + RTDEX_CTRL_STAT);
         break;
      case IOCTL_READ_REG:
         // (cmd, addr32)  ==> (value)
         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         if( *(pu32+1) > perseus->firstBarSize -4) {
            PDEBUG( MYERR "bad address (%u)\n", *(pu32+1));
            uRtdexLastError = rtdxpkdrv_BAD_ADDRESS;
            res = -EINVAL;
            break;
         }
         au32Resp[0] = ioread32( perseus->firstBar + *(pu32+1));
         iRetLen = 1 * sizeof(u32);
         break;
      case IOCTL_WRITE_REG:
         // (cmd, addr32, value)
         if( uInWords < 3) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         if( *(pu32+1) > perseus->firstBarSize -4) {
            PDEBUG( MYERR "bad address (%u)\n", *(pu32+1));
            uRtdexLastError = rtdxpkdrv_BAD_ADDRESS;
            res = -EINVAL;
            break;
         }
         iowrite32( *(pu32 +2), perseus->firstBar + *(pu32+1));
         break;
      case IOCTL_RMW_REG:
         // (cmd, addr32, andVal, orVal)
         if( uInWords < 4) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         if( *(pu32+1) > perseus->firstBarSize -4) {
            PDEBUG( MYERR "bad address (%u)\n", *(pu32+1));
            uRtdexLastError = rtdxpkdrv_BAD_ADDRESS;
            res = -EINVAL;
            break;
         }
         spin_lock_irqsave( &perseus->cdmaSpinlock, uFlags);
         spin_lock( &perseus->dmSpinlock);
         uTmp = ioread32( perseus->firstBar + *(pu32+1));
         iowrite32( (uTmp & *(pu32+2)) | *(pu32+3), perseus->firstBar + *(pu32+1));
         spin_unlock( &perseus->dmSpinlock);
         spin_unlock_irqrestore( &perseus->cdmaSpinlock, uFlags);
         break;



      // user-logic--->TX-FIFO--->devA--->PCIe--->devB--->RX-FIFO--->user-logic
      case IOCTL_CONNECT_DEV2DEV:
         // (cmd, srcFifoNo, dstFifoNo ,dstDevNo) --> (chnlNo)
         // perseus instance IS src device
         {
         unsigned uDstDevNo;
         unsigned uSrcFifoNo, uDstFifoNo;
         AllocChannel * pAC;

         if( uInWords < 4) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uSrcFifoNo = *(pu32+1);
         uDstFifoNo = *(pu32+2);
         uDstDevNo = *(pu32+3);

         if( uDstDevNo >= atomic_read( &perseus_count)) {
            PDEBUG( MYERR "Invalid dstDevNo# (%u)\n", uDstDevNo);
            uRtdexLastError = rtdxpkdrv_BAD_DEST_DEV_NO;
            res = -EINVAL;
            break;
         }
         if( uDstFifoNo >= allDevs[ uDstDevNo].perseus->rxCfg.uNbFifos) {
            PDEBUG( MYERR "Invalid dstFifoNo# (%u)\n", uDstFifoNo);
            uRtdexLastError = rtdxpkdrv_BAD_DEST_FIFO_NO;
            res = -EINVAL;
            break;
         }
         if( uSrcFifoNo >= perseus->txCfg.uNbFifos) {
            PDEBUG( MYERR "Invalid srcFifoNo# (%u)\n", uSrcFifoNo);
            uRtdexLastError = rtdxpkdrv_BAD_SRC_FIFO_NO;
            res = -EINVAL;
            break;
         }
         pAC = NULL; // sanity (GCC warning)
         if( (uRtdexLastError = iAllocateDev2DevFifos( perseus, uDstDevNo, uSrcFifoNo, uDstFifoNo, &pAC))) {
            res = -EINVAL;
            break;
         }
//         iEnableRxFifo( perseus, pAC, 0, 0); // disable RX-FIFO
//         iEnableTxFifo( perseus, pAC, 0, 0); // disable TX-FIFO
         iEnableRxFifoMask( perseus, 1<<pAC->uChnlNo, 0, 0); // disable RX-FIFO
         iEnableTxFifoMask( perseus, 1<<pAC->uChnlNo, 0, 0); // disable TX-FIFO
         perseus->txFifos[ uSrcFifoNo].pAC = pAC;
         au32Resp[0] = pAC->uChnlNo;
         iRetLen = 1 * sizeof(u32);
         }
         break;
      case IOCTL_CONNECT_HOST2DEV:
         // (cmd, uFifoNo) ==> (chnlNo)
         {
         unsigned uFifoNo;
         AllocChannel * pAC = NULL;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uFifoNo = *(pu32+1);
         if( uFifoNo >= perseus->rxCfg.uNbFifos) {
            PDEBUG( MYERR "Invalid fifoNo# (%u)\n", uFifoNo);
            uRtdexLastError = rtdxpkdrv_BAD_FIFO_NO;
            res = -EINVAL;
            break;
         }
         if( (uRtdexLastError = iAllocateFromHostFifo( perseus, uFifoNo, &pAC))) {
            res = -EINVAL;
            break;
         }
//         iEnableRxFifo( perseus, pAC, 0, 0); // disable RX-FIFO
         iEnableRxFifoMask( perseus, 1<<pAC->uChnlNo, 0, 0); // disable RX-FIFO
         perseus->rxFifos[ uFifoNo].pAC = pAC;
         au32Resp[0] = pAC->uChnlNo;
         iRetLen = 1 * sizeof(u32);
         }
         break;
      case IOCTL_CONNECT_DEV2HOST:
         // (cmd, uFifoNo) ==> (chnlNo)
         {
         unsigned uFifoNo;
         AllocChannel * pAC;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uFifoNo = *(pu32+1);
         if( uFifoNo >= perseus->txCfg.uNbFifos) {
            PDEBUG( MYERR "Invalid fifoNo# (%u)\n", uFifoNo);
            uRtdexLastError = rtdxpkdrv_BAD_FIFO_NO;
            res = -EINVAL;
            break;
         }
         pAC = NULL; // sanity (GCC warning)
         if( (uRtdexLastError = iAllocateToHostFifoDm( perseus, uFifoNo, &pAC))) {
            res = -EINVAL;
            break;
         }
//         iEnableTxFifo( perseus, pAC, 0, 0); // disable TX-FIFO
         iEnableTxFifoMask( perseus, 1<<pAC->uChnlNo, 0, 0); // disable TX-FIFO
         perseus->txFifos[ uFifoNo].pAC = pAC;
         au32Resp[0] = pAC->uChnlNo;
         iRetLen = 1 * sizeof( u32);
         }
         break;
      case IOCTL_UNCONNECT:
         // (cmd, chnlNo)
         {
         AllocChannel * pAC;
         unsigned uChnlNo;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res = -EINVAL;
            break;
         }
         iStopChannel( perseus, pAC);
         UnconnectChannel( perseus, pAC);
         }
         break;
      case IOCTL_START:
         // (cmd, uChnlNo, frameSize, transferSizeL) // 32-bits driver
         // (cmd, uChnlNo, frameSize, transferSizeL, transferSizeH) // 64-bit driver
         {
         AllocChannel * pAC;
         unsigned uChnlNo;

         if( uInWords < 3 + sizeof( size_t) /4) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res = -EINVAL;
            break;
         }
         if( pAC->uState == ST_RUN) {
            PDEBUG( MYERR "Still running");
            uRtdexLastError = rtdxpkdrv_STILL_RUNNING;
            res = -EINVAL;
            break;
         }
         pAC->uFrameSize = *(pu32+2);
         pAC->transferSize = *(pu32+3);
         if( sizeof( pAC->transferSize) > 4) {
            const int n = 32;
            pAC->transferSize += (size_t)*(pu32+4) << n;
         }
         pAC->dmaXferedSize = 0;
         pAC->hostXferedSize = 0;
         if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
            if( !pAC->uFrameSize || (pAC->uFrameSize & (WORD_SIZE_B-1)) || pAC->uFrameSize > MAX_USER_FRAME_SIZE) {
               PDEBUG( MYERR "Invalid frameSize");
               uRtdexLastError = rtdxpkdrv_BAD_FRAME_SIZE;
               res = -EINVAL;
               break;
               }
            // clear rx words count:
            iowrite32( FIFO_RST_CLR_WDCNT_BM_n( pAC->u.host.uFifoNo), perseus->rtdex +RTDEX_RXTX_RESET);
            iowrite32( 0, perseus->rtdex +RTDEX_RXTX_RESET);
            pAC->uState = ST_RUN;
         }
         else if( pAC->uChnlType == TYPE_DEV_TO_HOST) {
            if( !pAC->uFrameSize || (pAC->uFrameSize & (WORD_SIZE_B-1)) || pAC->uFrameSize > MAX_DM_TO_HOST_FRAME_SIZE) {
               PDEBUG( MYERR "Invalid frameSize");
               uRtdexLastError = rtdxpkdrv_BAD_FRAME_SIZE;
               res = -EINVAL;
               break;
               }
            pAC->uState = ST_RUN;
            if( (uRtdexLastError = iSetupDmDev2Host( perseus, pAC))) {
               pAC->uState = ST_STOP_OK; //???
               res = -EINVAL;
            }
         }
         else { /*if( pAC->uChnlType == TYPE_DEV_TO_DEV)*/
            if( !pAC->uFrameSize || (pAC->uFrameSize & (WORD_SIZE_B-1)) || pAC->uFrameSize > MAX_DM_TO_DEV_FRAME_SIZE) {
               PDEBUG( MYERR "Invalid frameSize"); // BAD_FRAME_SIZE
               uRtdexLastError = rtdxpkdrv_BAD_FRAME_SIZE;
               res = -EINVAL;
               break;
               }
            if( 0 == iSetupDmDev2DevSrc( perseus, pAC))
                pAC->uState = ST_RUN;
         }
         }
         break;
      case IOCTL_GET_STATUS:
         // (cmd, chnlNo) ==> (bEnded, bSuccess)
         {
         AllocChannel * pAC;
         unsigned uChnlNo;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res = -EINVAL;
            break;
         }
         au32Resp[ 0] = pAC->uState > ST_WAIT_RXFIFO;
         au32Resp[ 1] = pAC->uState == ST_STOP_OK;
         iRetLen = 2 * sizeof(u32);
         }
         break;
      case IOCTL_STOP:
         // (cmd, chnlNo)
         {
         AllocChannel * pAC;
         unsigned uChnlNo;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res = -EINVAL;
            break;
         }
         if( pAC->uState == ST_STOP_OK || pAC->uState == ST_STOP_FAIL || pAC->uState == ST_STOP_ABORT) {
//            PDEBUG( MYERR "Still stop"); // not realy an error!
//            uRtdexLastError = rtdxpkdrv_NOT_RUNNING;
//            res = -EINVAL;
            break;
         }
         if( (uRtdexLastError = iStopChannel( perseus, pAC)))
            res = -EINVAL;
         pAC->uState = ST_STOP_ABORT;
         }
         break;
      case IOCTL_SET_HOST2DEV_FLUSH_TRESHOLD:
         // (cmd, sizeK)
         {
         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         perseus->sqCdma.uFlushThreshold = *(pu32+1) * 1024;
         iSetupCdmaSg( perseus);
         }
         break;
      case IOCTL_FORCE_HOST2DEV_FLUSH:
         // (cmd)
         {
         perseus->sqCdma.bForceFlush = 1;
         iSetupCdmaSg( perseus);
         }
         break;

      case IOCTL_GET_RX_TFR_SIZE: // get byte count received by rx-fifo
         // (cmd, uChnlNo) --> (sizeL) // 32-bit driver
         // (cmd, uChnlNo) --> (sizeL ,sizeH) // 64-bit driver
         {
         AllocChannel * pAC;
         unsigned uChnlNo;
         unsigned uFifoNo;
         void * pRegs;
         size_t val;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            PDEBUG( MYERR "Not connected");
            res = -EINVAL;
            break;
         }
         if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
            pRegs = perseus->rtdex;
            uFifoNo = pAC->u.host.uFifoNo;
         }
         else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
            pRegs = allDevs[ pAC->u.dev2dev.uDstDevNo].perseus->rtdex;
            uFifoNo = pAC->u.dev2dev.uDstFifoNo;
         }
         else { // dev to host
            PDEBUG( MYERR "Bad channel type");
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_TYPE;
            res = -EINVAL;
            break;
         }
         val = (size_t)ioread32( pRegs + RTDEX_RX_WORDCNT_n( uFifoNo)) * WORD_SIZE_B; // overflow of hardware counter???
         au32Resp[ 0] = (u32)val;
         if( sizeof( val) > 4) {
            const int n = 32;
            au32Resp[ 1] = (unsigned)(val >> n);
         }
         iRetLen = sizeof( val);
         }
         break;

      case IOCTL_GET_TX_TFR_SIZE: // get bytes count sent by DM
         // (cmd, uChnlNo) --> (sizeL) // 32-bit driver
         // (cmd, uChnlNo) --> (sizeL ,sizeH) // 64-bit driver
         {
         u32 uChnlNo;
         AllocChannel * pAC;
         size_t val;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res =  -EINVAL;
            break;
         }
         if( pAC->uChnlType == TYPE_DEV_TO_HOST)
            val = pAC->dmaXferedSize;
         else if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
            PDEBUG( MYERR "Bad channel type");
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_TYPE;
            res = -EINVAL;
            break;
         }
         else { // dev to dev
            unsigned long uFlags;

            spin_lock_irqsave( &perseus->dmSpinlock, uFlags);
            val = pAC->dmaXferedSize;
            if( pAC->uState == ST_RUN)
               val += ioread32( perseus->rtdex + RTDEX_TX_DM_CUR_TFRCNT_n( pAC->u.dev2dev.uSrcFifoNo))
                      * (size_t)pAC->uFrameSize;
            spin_unlock_irqrestore( &perseus->dmSpinlock, uFlags);
         }
         au32Resp[ 0] = (u32)val;
         if( sizeof( val) > 4) {
            const int n = 32;
            au32Resp[ 1] = (unsigned)(val >> n);
         }
         iRetLen = sizeof( val);
         }
         break;

      case IOCTL_GET_HOST_TFR_SIZE:
         {
         u32 uChnlNo;
         AllocChannel * pAC;
         size_t val;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res =  -EINVAL;
            break;
         }
         if( pAC->uChnlType == TYPE_DEV_TO_HOST)
            val = pAC->hostXferedSize;
         else if( pAC->uChnlType == TYPE_HOST_TO_DEV)
            val = pAC->hostXferedSize;
         else { // dev to dev
            PDEBUG( MYERR "Bad channel type");
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_TYPE;
            res = -EINVAL;
            break;
         }
         au32Resp[ 0] = (u32)val;
         if( sizeof( val) > 4) {
            const int n = 32;
            au32Resp[ 1] = (unsigned)(val >> n);
         }
         iRetLen = sizeof( val);
         }
         break;

/*
      case IOCTL_RESET_DM:
         // (cmd, chnlNo)
         {
         u32 uDmNo;
         u32 uVal;
         unsigned u;
         AllocChannel * pAC;
         unsigned uChnlNo;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res = -EINVAL;
            break;
         }
         if( pAC->uChnlType == TYPE_DEV_TO_HOST)
            uDmNo = pAC->u.host.uFifoNo;
         else if( pAC->uChnlType == TYPE_DEV_TO_DEV)
            uDmNo = pAC->u.dev2dev.uSrcFifoNo;
         else {
            PDEBUG( MYERR "Bad channel type");
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_TYPE;
            res = -EINVAL;
            break;
         }
         if( (uRtdexLastError = iResetDm( uDmNo))
            res = -EINVAL;
         }
         break;
*/

      case IOCTL_RESET_RX_FIFO:
         // (cmd, chnlNo)
         {
         AllocChannel * pAC;
         unsigned uChnlNo;
         unsigned uFifoNo;
         void * pRegs;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res = -EINVAL;
            break;
         }
         if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
            uFifoNo = pAC->u.host.uFifoNo;
            pRegs = perseus->rtdex;
         }
         else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
            uFifoNo = pAC->u.dev2dev.uDstFifoNo;
            pRegs = allDevs[ pAC->u.dev2dev.uDstDevNo].perseus->rtdex;
         }
         else {
            PDEBUG( MYERR "Bad channel type");
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_TYPE;
            res = -EINVAL;
            break;
         }
         // no protection needed:
         iowrite32( FIFO_RST_RX_BM_n( uFifoNo), pRegs +RTDEX_RXTX_RESET);
         iowrite32( 0, pRegs +RTDEX_RXTX_RESET);
         }
         break;
      case IOCTL_RESET_TX_FIFO:
         // (cmd, uChnlNo)
         {
         AllocChannel * pAC;
         unsigned uChnlNo;
         unsigned uFifoNo;
         void * pRegs;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res = -EINVAL;
            break;
         }
         if( pAC->uChnlType == TYPE_DEV_TO_HOST) {
            uFifoNo = pAC->u.host.uFifoNo;
            pRegs = perseus->rtdex;
         }
         else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
            uFifoNo = pAC->u.dev2dev.uSrcFifoNo;
            pRegs = perseus->rtdex;
         }
         else {
            PDEBUG( MYERR "Bad channel type");
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_TYPE;
            res = -EINVAL;
            break;
         }
         // no protection needed:
         iowrite32( FIFO_RST_TX_BM_n( uFifoNo), pRegs +RTDEX_RXTX_RESET);
         iowrite32( 0, pRegs +RTDEX_RXTX_RESET);
         if( (uRtdexLastError = iResetDm( perseus, uFifoNo)))
            res = -EINVAL;
         }
         break;

      case IOCTL_GET_TX_FIFO_STATUS:
         // (cmd, chnlNo) --> (overflow, underflow)
         {
         unsigned uStat;
         AllocChannel * pAC;
         unsigned uChnlNo;
         unsigned uFifoNo;
         void * pRegs;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            PDEBUG( MYERR "Not connected");
            res = -EINVAL;
            break;
         }
         if( pAC->uChnlType == TYPE_DEV_TO_HOST) {
            uFifoNo = pAC->u.host.uFifoNo;
            pRegs = perseus->rtdex;
         }
         else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
            uFifoNo = pAC->u.dev2dev.uSrcFifoNo;
            pRegs = perseus->rtdex;
         }
         else {
            PDEBUG( MYERR "Bad channel type");
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_TYPE;
            res = -EINVAL;
            break;
         }

         uStat = ioread32( pRegs + RTDEX_TX_FIFO_STATUS);
         au32Resp[0] = uStat & FIFOSTAT_OVERF_BM_n( uFifoNo) ? 1 : 0;
         au32Resp[1] = uStat & FIFOSTAT_UNDERF_BM_n( uFifoNo) ? 1 : 0;
         iRetLen = 2 * sizeof(u32);
         }
         break;
      case IOCTL_GET_RX_FIFO_STATUS:
         // (cmd, chnlNo) --> (overflow, underflow)
         {
         unsigned uStat;
         AllocChannel * pAC;
         unsigned uChnlNo;
         unsigned uFifoNo;
         void * pRegs;

         if( uInWords < 2) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlNo = *(pu32+1);
         if( uChnlNo >= MAX_LOGICAL_CHANNELS) {
            PDEBUG( MYERR "Invalid connection# %u", uChnlNo);
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_NO;
            res = -EINVAL;
            break;
         }
         pAC = &perseus->devChannels[ uChnlNo];
         if( !pAC || !pAC->bAlloc) {
            PDEBUG( MYERR "Not connected");
            uRtdexLastError = rtdxpkdrv_CHANNEL_NOT_CONNECTED;
            res = -EINVAL;
            break;
         }
         if( pAC->uChnlType == TYPE_HOST_TO_DEV) {
            uFifoNo = pAC->u.host.uFifoNo;
            pRegs = perseus->rtdex;
         }
         else if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
            uFifoNo = pAC->u.dev2dev.uDstFifoNo;
            pRegs = allDevs[ pAC->u.dev2dev.uDstDevNo].perseus->rtdex;
         }
         else {
            PDEBUG( MYERR "Bad channel type");
            uRtdexLastError = rtdxpkdrv_BAD_CHANNEL_TYPE;
            res = -EINVAL;
            break;
         }

         uStat = ioread32( pRegs + RTDEX_RX_FIFO_STATUS);
         au32Resp[0] = uStat & FIFOSTAT_OVERF_BM_n( uFifoNo) ? 1 : 0;
         au32Resp[1] = uStat & FIFOSTAT_UNDERF_BM_n( uFifoNo) ? 1 : 0;
         iRetLen = 2 * sizeof(u32);
         }
         break;

      case IOCTL_ENABLE_RX_FIFO:
         // (cmd, chnlMsk, wr, rd)
         {
         u32 uRd, uWr, uChnlMsk;
         int iTmp;

         if( uInWords < 4) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlMsk = *(pu32+1);
         uWr = *(pu32+2);
         uRd = *(pu32+3);
         iTmp = iEnableRxFifoMask( perseus, uChnlMsk, uWr, uRd);
         if( iTmp > 0) {
            uRtdexLastError = iTmp;
            res = -EINVAL;
         }
         else
            res = iTmp;
         }
         break;
      case IOCTL_ENABLE_TX_FIFO:
         // (cmd, chnlMsk, wr, rd)
         {
         u32 uRd, uWr, uChnlMsk;
         int iTmp;

         if( uInWords < 4) {
            PDEBUG( MYERR "Missing parameters"); 
            uRtdexLastError = rtdxpkdrv_MISSING_PARAMETER;
            res = -EINVAL;
            break;
         }
         uChnlMsk = *(pu32+1);
         uWr = *(pu32+2);
         uRd = *(pu32+3);
         iTmp = iEnableTxFifoMask( perseus, uChnlMsk, uWr, uRd);
         if( iTmp > 0) {
            uRtdexLastError = iTmp;
            res = -EINVAL;
         }
         else
            res = iTmp;
         }
         break;
      case IOCTL_GET_FIFO_CFG:
         // (cmd) ==> (nbRx, nbTx, rxSize, txSize)
         {
         au32Resp[0] = perseus->rxCfg.uNbFifos;
         au32Resp[1] = perseus->txCfg.uNbFifos;
         au32Resp[2] = perseus->rxCfg.uFifoSizeB;
         au32Resp[3] = perseus->txCfg.uFifoSizeB;
         iRetLen = 4 * sizeof(u32);
         }
         break;
      case IOCTL_GET_FREE_FIFO_MASK:
         // (cmd) ==> (rxFree, txFree)
         {
         unsigned uTx, uRx, u;

         uTx = 0;
         for( u = 0; u < perseus->txCfg.uNbFifos; u++)
            if( bIsSrcFifoFree( perseus, u))
               uTx |= 1 << u;
         uRx = 0;
         for( u = 0; u < perseus->rxCfg.uNbFifos; u++)
            if( bIsDstFifoFree( perseus, u))
               uRx |= 1 << u;
         au32Resp[0] = uRx;
         au32Resp[1] = uTx;
         iRetLen = 2 * sizeof(u32);
         }
         break;
      case IOCTL_GET_DRIVER_VERSION:
         // (cmd) ==> (maj, min)
         {
         au32Resp[0] = DRIVER_VER_MAJ;
         au32Resp[1] = DRIVER_VER_MIN;
         iRetLen = 2 * sizeof(u32);
         }
         break;


#ifdef ROUND_TRIP_TEST
      case IOCTL_ROUND_TRIP_TEST:
         // (cmd, enable, dmNo)
         perseus->bRoundTripTest = *(pu32+1);
         perseus->uRTP_dmNo = *(pu32+2);
         break;
#endif


      case 0x200:
         // (cmd, loopCount, offset, uData)
         // loop write a data value at fix offset from 'perseus->firstBar' - PCIe test
         {
         u32 uLoop, uData;
         size_t addr;

         uLoop = *(pu32+1);
         addr = *(pu32+2) + (size_t)perseus->firstBar;
         uData = *(pu32+3);

         asm (
            "cli \n\t"
#ifdef IS_64BIT_PTR
            "ici: mov %%eax, (%%rbx) \n\t"
#else
            "ici: mov %%eax, (%%ebx) \n\t"
#endif
            "loop ici \n\t"
            "sti" ::"a"(uData), "b"(addr), "c"(uLoop));
         }
         break;

     case 0x201:
         // (cmd, loopCount, offset)
         // loop read a data value at fix offset from 'perseus->firstBar' - PCIe test
         {
         u32 uLoop;
         size_t addr;

         uLoop = *(pu32+1);
         addr = *(pu32+2) + (size_t)perseus->firstBar;

         asm (
            "cli \n\t"
#ifdef IS_64BIT_PTR
            "ici2: mov (%%rbx), %%eax \n\t"
#else
            "ici2: mov (%%ebx), %%eax \n\t"
#endif
            "loop ici2 \n\t"
            "sti" :: "b"(addr), "c"(uLoop));
         }
         break;


      default:
         PDEBUG( MYERR "Unknow cmd");
         uRtdexLastError = rtdxpkdrv_UNKNOW_COMMAND;
         res = -EINVAL;
         break;
   }
   up( &perseus->wrSem);
   up( &perseus->rdSem);
   up( &perseus->ctlSem);

   if( !res) {
      if( iRetLen > uOutWords * sizeof( u32)) {
         PDEBUG( MYERR "return length > output buffer");
         uRtdexLastError = rtdxpkdrv_OUTPUT_BUFFER_TOO_SMALL;
         res = -EINVAL;
      }
      else if( copy_to_user( (void*)ioctlParam, au32Resp, iRetLen))
         res = -EFAULT;
      else { // ok!
         res = iRetLen;
         PDEBUG( "--> len= %u (x%X x%X x%X x%X)\n", iRetLen, au32Resp[0], au32Resp[1], au32Resp[2], au32Resp[3]);
      }
   }
   else if( res == -EINVAL && uRtdexLastError)
      if( copy_to_user( (void*)ioctlParam, &uRtdexLastError, sizeof uRtdexLastError))
         ;

   return res;
}


//!!!
static unsigned int rtdex_poll( struct file *filp, poll_table *wait)
{
   PerseusData * perseus = (PerseusData*)filp->private_data;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,19,0)
    struct inode *inode = filp->f_path.dentry->d_inode;  
#else
    struct inode *inode = filp->f_dentry->d_inode;
#endif
   unsigned int mask = 0;
   unsigned uFifoNo;

   uFifoNo = iminor( inode);
   if( uFifoNo < NB_FIFOS) {
      // rxFifos : cdma
      PDEBUG( "rxFifos %u", uFifoNo);
      down( &perseus->wrSem);
      poll_wait( filp, &perseus->outq, wait);
      if( QUEUE_FREE_SPACE( &perseus->sqCdma.dataQueue) && QUEUE_FREE_SPACE( &perseus->sqCdma.descQueue))
         mask |= POLLOUT | POLLWRNORM;   /* writable */
      up( &perseus->wrSem);
      PDEBUG( "done, mask= %x", mask);
   }
   else {
      // txFifos : dm
      uFifoNo -= NB_FIFOS;
      PDEBUG( "txFifos %u", uFifoNo);
      down( &perseus->rdSem);
      poll_wait( filp, &perseus->inq, wait);
      if( QUEUE_BUSY_SPACE( &perseus->txFifos[ uFifoNo].queue))
         mask |= POLLIN | POLLRDNORM;    /* readable */
      up( &perseus->rdSem);
      PDEBUG( "done, mask= %x", mask);
   }
   return mask;
}



// !!!
static struct file_operations perseus_rtdex_fops = {
	.owner   = THIS_MODULE,
   .open    = rtdex_open,
   .unlocked_ioctl = rtdex_unlocked_ioctl,
	.read    = rtdex_read,
	.write   = rtdex_write,
   .release = rtdex_release,
   .poll =    rtdex_poll,
};



static int rtdex_create_cdev( PerseusData *perseus)
{
	int i;

   if( alloc_chrdev_region( &perseus->rtdex_dev_number, 0, MINOR_GLOBAL +1, RTDEX_NAME)) {
		PERROR( "can't register device %s\n", RTDEX_NAME);
		return -1;
	}
   PDEBUG( "DevNum: MAJ( %u), MIN( %u)", MAJOR( perseus->rtdex_dev_number), MINOR( perseus->rtdex_dev_number));

   cdev_init( &(perseus->rtdex_cdev), &perseus_rtdex_fops);
	perseus->rtdex_cdev.owner = THIS_MODULE;
	cdev_add( &(perseus->rtdex_cdev), MKDEV( MAJOR( perseus->rtdex_dev_number), 0), MINOR_GLOBAL +1);

   for( i = 0; i < perseus->rxCfg.uNbFifos; i++) {
		PDEBUG( "creating " RXFIFO_NAME_STR " using dev %x\n", perseus->uInstance, i, MKDEV( MAJOR(perseus->rtdex_dev_number), i));
		device_create( rtdex_class, NULL, MKDEV( MAJOR( perseus->rtdex_dev_number), i), (void *)perseus, RXFIFO_NAME_STR, perseus->uInstance, i);
	}
   for( i = 0; i < perseus->txCfg.uNbFifos; i++) {
		PDEBUG( "creating " TXFIFO_NAME_STR " using dev %x\n", perseus->uInstance, i, MKDEV( MAJOR(perseus->rtdex_dev_number), NB_FIFOS + i));
		device_create( rtdex_class, NULL, MKDEV( MAJOR( perseus->rtdex_dev_number), NB_FIFOS + i), (void *)perseus, TXFIFO_NAME_STR, perseus->uInstance, i);
	}
	PDEBUG( "creating " GLOBAL_NAME_STR " using dev %x\n", perseus->uInstance, MKDEV( MAJOR(perseus->rtdex_dev_number), MINOR_GLOBAL));
   device_create( rtdex_class, NULL, MKDEV( MAJOR( perseus->rtdex_dev_number), MINOR_GLOBAL), (void *)perseus, GLOBAL_NAME_STR, perseus->uInstance);
	return 0;
}


static void rtdex_destroy_cdev( PerseusData *perseus)
{
	int i;

	for( i = perseus->txCfg.uNbFifos -1; i >= 0; i--) {
		PDEBUG( "destroying " TXFIFO_NAME_STR "using dev %x\n", perseus->uInstance, i, MKDEV( MAJOR(perseus->rtdex_dev_number), NB_FIFOS +i));
		device_destroy( rtdex_class, MKDEV( MAJOR( perseus->rtdex_dev_number), NB_FIFOS +i));
	}
	for( i = perseus->rxCfg.uNbFifos -1; i >= 0; i--) {
		PDEBUG( "destroying " RXFIFO_NAME_STR "using dev %x\n", perseus->uInstance, i, MKDEV( MAJOR(perseus->rtdex_dev_number), i));
		device_destroy( rtdex_class, MKDEV( MAJOR( perseus->rtdex_dev_number), i));
	}
	PDEBUG( "destroying " GLOBAL_NAME_STR "using dev %x\n", perseus->uInstance, MKDEV( MAJOR(perseus->rtdex_dev_number), MINOR_GLOBAL));
   device_destroy( rtdex_class, MKDEV( MAJOR( perseus->rtdex_dev_number), MINOR_GLOBAL));

	cdev_del( &(perseus->rtdex_cdev));
	unregister_chrdev_region( perseus->rtdex_dev_number, NB_FIFOS * 2 + 1);
}



// interrupt context!!
static void ProcessPerseusCommandRx( PerseusData * perseus)
{
   u32 uP1, uP2, uP3, uVal, uOk, uCmd;

   uP1 = uP2 = uP3 = 0;
   uOk = 1; // default to success!
   uCmd = ioread32( perseus->mRegs +REG32_UB_CMD);
   switch( uCmd) {
      case CMD_RD: // *P1
         uP1 = ioread32( perseus->mRegs +REG32_P1);
         if( uP1 <= perseus->firstBarSize -4) {
            uVal = ioread32( perseus->firstBar + uP1);
            iowrite32( uVal, perseus->mRegs +REG32_RET_DATA);
         }
         else
            uOk = 0;
         break;
      case CMD_RMW: // *P1 = (*P1 & P2) | P3
         uP1 = ioread32( perseus->mRegs +REG32_P1);
         uP2 = ioread32( perseus->mRegs +REG32_P2);
         uP3 = ioread32( perseus->mRegs +REG32_P3);
         if( uP1 <= perseus->firstBarSize -4) {
            uVal = ioread32( perseus->firstBar +uP1);
            iowrite32( (uVal & uP2) | uP3, perseus->firstBar +uP1);
            iowrite32( uVal, perseus->mRegs +REG32_RET_DATA);
         }
         else
            uOk = 0;
         break;
      case CMD_WR: // *P1 = P2
         uP1 = ioread32( perseus->mRegs +REG32_P1);
         uP2 = ioread32( perseus->mRegs +REG32_P2);
         if( uP1 <= perseus->firstBarSize -4) {
            iowrite32( uP2, perseus->firstBar +uP1);
         }
         else
            uOk = 0;
         break;
      case CMD_GET_INSTANCE:
         iowrite32( perseus->uInstance, perseus->mRegs +REG32_RET_DATA);
         break;
      default:
//         PDEBUG( "Bad command!\n");
         uOk = 0;
         break;
   }
   iowrite32( uOk ? 0 : ~0, perseus->mRegs +REG32_STATUS);
   PDEBUG( "Perseus cmd (x%x, x%x, x%x, x%x) --> (x%x) %s\n", uCmd, uP1, uP2, uP3, ioread32( perseus->mRegs +REG32_RET_DATA), uOk ? "Success!" : "Failed!");
   iowrite32( CMD_IDLE, perseus->mRegs +REG32_UB_CMD);
}


   
// !!!
static irqreturn_t IrqHandler( int irq, void *dev_id)
{
	PerseusData *perseus = (PerseusData *)dev_id;
   u32 uSR, uTfrCtrl;
   unsigned u, uDmNo;

   if( perseus->pdev->irq == irq) {
	   PDEBUG("MSI (%d), instance= %u, CPU= %u\n", irq, perseus->uInstance, smp_processor_id());

      // test for CDMA irq:
//      PDEBUG( "CDMASR= x%X\n", ioread32( perseus->cdma+CDMASR));
      if( (uSR = ioread32( perseus->cdma + CDMASR)) & (CDMASR_ERR_IRQ_BM | CDMASR_IOC_IRQ_BM | CDMASR_DLY_IRQ_BM)) {
         PDEBUG( "Cdma Interrupting, SR= x%X\n", uSR);
         iowrite32( CDMASR_ERR_IRQ_BM | CDMASR_IOC_IRQ_BM | CDMASR_DLY_IRQ_BM, perseus->cdma + CDMASR);
         if( uSR & CDMASR_ERR_IRQ_BM) {
            PDEBUG( "Cdma irqErrors++");
            perseus->sqCdma.uIrqErrors++;
            iowrite32( CDMACR_RESET_BM, perseus->cdma + CDMACR); // reset Cdma
            for( u = 0; u < 5; u++)
               uSR = ioread32( perseus->cdma + CDMASR);
         }
         if( uSR & CDMASR_DLY_IRQ_BM) {
            perseus->sqCdma.bDelayTrig = 1;
            PDEBUG( "Cdma irqDelays++");
            perseus->sqCdma.uIrqDelays++;
            if( !perseus->sqCdma.bWorking)
               iSetupCdmaSg( perseus);
         }
         if( uSR & CDMASR_IOC_IRQ_BM) {
            PDEBUG( "Cdma irqIocs++");
            perseus->sqCdma.uIrqIocs++;
         }

         spin_lock( &perseus->cdmaSpinlock);
//         if( (uSR & CDMASR_IDLE_BM) && perseus->sqCdma.bWorking) { // removed MSA-28may13
         if( (uSR & (CDMASR_IOC_IRQ_BM | CDMASR_ERR_IRQ_BM)) && perseus->sqCdma.bWorking) { // added MSA-28may2013
            AllocChannel * pAC;
            CdmaDesc * list;
            unsigned uTotalSize, uDescSize, u;

//            if( printk_ratelimit())
            PDEBUG( "Cdma return to idle");

            perseus->sqCdma.bWorking = 0;

#ifdef ROUND_TRIP_TEST
            if( perseus->bRoundTripTest) {
               if( !perseus->txFifos[ perseus->uRTP_dmNo].pAC)
                  PDEBUG( MYERR "RTT : Channel dev2host, dm= %u not connected", perseus->uRTP_dmNo);
               else {
                  spin_unlock( &perseus->cdmaSpinlock);
                  iSetupDmDev2Host( perseus, perseus->txFifos[ perseus->uRTP_dmNo].pAC);
                  spin_lock( &perseus->cdmaSpinlock);
               }
            }
#endif
            // update stats:
            uTotalSize = 0;
            list = QUEUE_BUSY_PTR( &perseus->sqCdma.descQueue);
//if( perseus->sqCdma.uNbActDesc > NB_SQ_DESC_ENTRIES) {
//   PDEBUG( MYERR "Invalid uNbActDesc= %u", perseus->sqCdma.uNbActDesc);
//   perseus->sqCdma.uNbActDesc = 0;
//   }
            for( u = 0; u < perseus->sqCdma.uNbActDesc; u++, list = list->u.desc.next) {
               uDescSize = list->u.desc.uControl & SGDESC_CTRL_BTT_BM;
               uTotalSize += uDescSize;
               pAC = perseus->rxFifos[ list->u.desc.uFifoNo].pAC;
               if( pAC)
                  pAC->dmaXferedSize += uDescSize;
            }
            QUEUE_GET( &perseus->sqCdma.descQueue, perseus->sqCdma.uNbActDesc * sizeof( CdmaDesc));
            perseus->sqCdma.uNbActDesc = 0;
            QUEUE_GET( &perseus->sqCdma.dataQueue, uTotalSize);
            spin_unlock( &perseus->cdmaSpinlock);

            iSetupCdmaSg( perseus);
            wake_up_interruptible( &perseus->outq);  /* wake-up writer(s) */
         }
        else{
            spin_unlock( &perseus->cdmaSpinlock);
            }
      }


      // Test for Data-Mover irq:
      uTfrCtrl = ioread32( perseus->rtdex + RTDEX_TX_DM_TFR_CTRL);
      if( !(uTfrCtrl & TFRCTRL_DONE_BM_all))
         goto no_dm1;
      spin_lock( &perseus->dmSpinlock);
      uTfrCtrl = ioread32( perseus->rtdex + RTDEX_TX_DM_TFR_CTRL);
      PDEBUG( "Data-Mover interrupting, uTfrCtrl= x%X\n", uTfrCtrl);
      for( uDmNo = 0; (uTfrCtrl & TFRCTRL_DONE_BM_all) && (uDmNo < NB_DATA_MOVERS); uDmNo++) {
         if( (perseus->dmState[ uDmNo].bWorking)
         && (uTfrCtrl & TFRCTRL_DONE_BM_n( uDmNo))) {
            AllocChannel * pAC;

            PDEBUG( "DataMover Done channel %u\n", uDmNo);
            uTfrCtrl &= ~TFRCTRL_START_BM_n( uDmNo);
            iowrite32( uTfrCtrl, perseus->rtdex + RTDEX_TX_DM_TFR_CTRL); // clear interrupt source
            perseus->dmState[ uDmNo].bWorking = 0;
            pAC = perseus->txFifos[ uDmNo].pAC;
            if( !pAC) {
               PDEBUG( "Channel dev2host, dm= %u not connected", uDmNo);
               goto nextDm;
            }
            if( pAC->uChnlType == TYPE_DEV_TO_DEV) {
               pAC->dmaXferedSize += perseus->dmState[ uDmNo].size;
               perseus->dmState[ uDmNo].size = 0; // sanity
               spin_unlock( &perseus->dmSpinlock);
               if( iSetupDmDev2DevSrc( perseus, pAC))
                  pAC->uState = ST_STOP_OK; //???
               spin_lock( &perseus->dmSpinlock);
            }
            else if( pAC->uChnlType == TYPE_DEV_TO_HOST) {
               QUEUE_PUT( &perseus->txFifos[ uDmNo].queue, (unsigned)perseus->dmState[ uDmNo].size);
               pAC->dmaXferedSize += perseus->dmState[ uDmNo].size;
               perseus->dmState[ uDmNo].size = 0; // sanity
               // setup next input frame:
               spin_unlock( &perseus->dmSpinlock);
               iSetupDmDev2Host( perseus, pAC);
               spin_lock( &perseus->dmSpinlock);
               }
            }
nextDm:;
         uTfrCtrl = ioread32( perseus->rtdex + RTDEX_TX_DM_TFR_CTRL);
         } // for
      spin_unlock( &perseus->dmSpinlock);
      wake_up_interruptible( &perseus->inq);  /* wake-up reader(s) */
no_dm1:;

      // test for mailbox (inbox) messages:
//      if( 0 != ioread16( perseus->inbox + MAILBOX_MSG_LEN16)) {
      if( MBX_BUSY_SPACE((MailboxStruc*)perseus->inbox)) {
		   ProcessMailboxRx( perseus->mailbox_ndev);
	   }
      // MSA-19apr2013 (all the paragraph):
      if( perseus->bMbxStoped && MBX_FREE_SPACE((MailboxStruc*)perseus->outbox)) {
         PDEBUG( "Restarting mailbox out queue");
         perseus->bMbxStoped = 0;
         netif_wake_queue( perseus->mailbox_ndev);
      }

      // test for Perseus command:
      if( CMD_IDLE != ioread32( perseus->mRegs +REG32_UB_CMD))
         ProcessPerseusCommandRx( perseus);

	   PDEBUG("leaving MSI (%d)\n", irq);
   }
   else
   {
      PDEBUG("IRQ %d\n", irq);
      }

    return IRQ_HANDLED;
}

/***********************************************************************
 * 
 * Device type-agnostic functions
 *
 **********************************************************************/

static int iPerseus_alloc_data( PerseusData ** hPerseus)
{
   PerseusData * perseus;

	perseus = kmalloc( sizeof( PerseusData), GFP_KERNEL);
	if( !perseus) {
		PDEBUG("can't allocate memory for perseus\n");
		goto perseus_alloc_exit_0;
	}
	memset( perseus, 0, sizeof *perseus);

	PDEBUG("private data at %p\n", perseus);
	PDEBUG("private data sized %lu bytes\n", (unsigned long)sizeof * perseus);

#ifdef USE_DEBUG_DMA_BUFFER
   perseus->_pucBuff = pci_alloc_consistent( perseus->pdev, DMA_BUFFER_SIZE, &perseus->physAddr);
{
u32 * pu = (u32*)perseus->_pucBuff;
 unsigned u;
 for( u = 0; u < DMA_BUFFER_SIZE; u += 4) {
   *pu++ = u;
 }
}
   PDEBUG( "DEBUG-DMA(instance %d) buffer size= %u, virt= x%p, phys= x%p\n", perseus->uInstance, DMA_BUFFER_SIZE, perseus->_pucBuff, (void*)perseus->physAddr);
#endif

   *hPerseus = perseus;
   PDEBUG("done\n");
   return 0;

//perseus_alloc_exit_1:
	kfree( perseus);
perseus_alloc_exit_0:
	return -1;
}


static void perseus_free_data( PerseusData *perseus)
{
   PDEBUG( "");
#ifdef USE_DEBUG_DMA_BUFFER
   pci_free_consistent( perseus->pdev, DMA_BUFFER_SIZE, perseus->_pucBuff, perseus->physAddr);
#endif

   if( perseus->sqCdma.buffer.pvBase) {
      pci_free_consistent( perseus->pdev, perseus->sqCdma.buffer.uSize, perseus->sqCdma.buffer.pvBase, perseus->sqCdma.buffer.physAddr);
      perseus->sqCdma.buffer.pvBase = NULL;
   }

	kfree( perseus);
}


static void perseus_init_data( PerseusData *perseus)
{
//	int i;
   unsigned u, uVal;

	PDEBUG("\n");
   perseus->uCdmaBarNo = 99; // sanity
   for( u = 0; u < NB_FIFOS; u++)
      perseus->uDmBarNo[ u] = 99; // sanity

   perseus->mRegs = perseus->firstBar + REG_OFFSET;
	perseus->inbox = perseus->firstBar + MAILBOXES_OFFSET + FROM_FPGA_MAILBOX * MAILBOX_SIZE;
	perseus->outbox = perseus->firstBar + MAILBOXES_OFFSET + TO_FPGA_MAILBOX * MAILBOX_SIZE;
   perseus->cdma = perseus->firstBar + CDMA_START;
//   perseus->sg = perseus->firstBar + SGDESC_START;
   perseus->rtdex = perseus->firstBar + RTDEX_REGS_START;
   perseus->pcieBr = perseus->firstBar + PCIE_BRIDGE_START;

	PDEBUG( "inbox  at %p\n", perseus->inbox);
	PDEBUG( "outbox at %p\n", perseus->outbox);
	memset_io( perseus->inbox, 0, MAILBOX_SIZE);
	PDEBUG( "inbox cleared\n");
	memset_io( perseus->mRegs,  0, REG_SIZE);
	PDEBUG( "mRegs cleared\n");
   iowrite32( perseus->uInstance, perseus->mRegs + REG32_INSTANCE); // MSA-22apr2013

   // reset rtdex:
   iowrite32( 1 << CTRLSTAT_RESET_BN, perseus->rtdex + RTDEX_CTRL_STAT);

   // get rx-fifo config:
   uVal = ioread32( perseus->rtdex + RTDEX_RX_CONFIG_INFO);
   perseus->rxCfg.uFifoSizeB = 1 << (((uVal & RTDEX_CFGINF_FIFODEPTH_BM) >> RTDEX_CFGINF_FIFODEPTH_BO));
   perseus->rxCfg.uNbFifos = uVal & RTDEX_CFGINF_NBCHNL_BM;
   if( perseus->rxCfg.uNbFifos > NB_FIFOS) {
      PDEBUG( "too many detected rx-fifo count, truncated to %u", NB_FIFOS);
      perseus->rxCfg.uNbFifos = NB_FIFOS;
   }
   PDEBUG( "rx-fifo: nbr= %u, size= %u", perseus->rxCfg.uNbFifos, perseus->rxCfg.uFifoSizeB);

   // get tx-fifo config:
   uVal = ioread32( perseus->rtdex + RTDEX_TX_CONFIG_INFO);
   perseus->txCfg.uFifoSizeB = 1 << (((uVal & RTDEX_CFGINF_FIFODEPTH_BM) >> RTDEX_CFGINF_FIFODEPTH_BO));
   perseus->txCfg.uNbFifos = uVal & RTDEX_CFGINF_NBCHNL_BM;
   if( perseus->txCfg.uNbFifos > NB_FIFOS) {
      PDEBUG( "too many detected tx-fifo count, truncated to %u", NB_FIFOS);
      perseus->txCfg.uNbFifos = NB_FIFOS;
   }
   PDEBUG( "tx-fifo: nbr= %u, size= %u", perseus->txCfg.uNbFifos, perseus->txCfg.uFifoSizeB);

   // get cdma presence:
   uVal = ioread32( perseus->rtdex + RTDEX_CTRL_STAT);
   perseus->sqCdma.bPresent = uVal & (1 << CTRLSTAT_CDMA_PRESENT_BN) ? 1 : 0;
   PDEBUG( "Cdma present: %s", perseus->sqCdma.bPresent ? "Yes" : "No");


   QUEUE_INIT( &perseus->sqCdma.descQueue);
   QUEUE_INIT( &perseus->sqCdma.dataQueue);
   for( u = 0; u < NB_FIFOS; u++)
      QUEUE_INIT( &perseus->txFifos[ u].queue);

   iowrite32( CDMACR_RESET_BM, perseus->cdma + CDMACR); // reset CDMA
   for( u = 0; u < perseus->txCfg.uNbFifos; u++)
      iResetDm( perseus, u);

//perseus->sqCdma.uFlushThreshold = 512;

	PDEBUG("done\n");
}



#ifdef PERSEUS_PROCFS

// input HH-MH-ML-LL --> LL-ML-MH-HH
static inline unsigned long ulConvertBusEndianess( unsigned long a)
   {
   return a;
//   return ( ((a)<<24 & 0xFF000000)  |  ((a)>>24 & 0xFF)  |  ((a)<<8 & 0xFF0000)  |  ((a)>>8 & 0xFF00) );
   }
static inline unsigned short usConvertBusEndianess( unsigned short b)
   {
   return b;
//   return (b << 8) | (b>>8);
   }


// Write a register to FPGA communication aera - debug
// "B zone offset data"   -- write a byte
// "W zone offset data"   -- write a short
// "D zone offset data"   -- write a dword
// 'P' zone offset        -- set pointer for direct read via proc_MemRead()
// all constants in hex
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
static ssize_t proc_MemWrite(struct file *file, const char __user *buffer, size_t count, loff_t *pos) 
{
   struct seq_file *seq = file->private_data;
   void *data = seq->private;
#else
static int proc_MemWrite( struct file* file, const char * buffer, unsigned long count, void * data)
{
#endif
   char bf[ 50];
   unsigned uLen;
   unsigned uZoneNo, uOffset, uData, uSize;
   unsigned char ucType;
   PerseusData * pInfo = (PerseusData*)data;

//printk( KERN_INFO DEVICE_NAME " data = %Xh\n", (u32)data);
//return 0;

   uLen = count;
   if( uLen >= sizeof bf)
      uLen = sizeof bf -1;

   if( copy_from_user( bf, buffer, uLen))
      return -EFAULT;
   bf[ uLen] = 0; // MSA-20jan11

   sscanf( bf, "%c %x %x %x", &ucType, &uZoneNo, &uOffset, &uData);
   switch( ucType) {
      case 'b':
      case 'B':
         uSize = 1;
         break;
      case 'w':
      case 'W':
         uSize = 2;
         break;
      case 'd':
      case 'D':
         uSize = 4;
         break;
      case 'p':
      case 'P': // P <bar> <offset>
         pInfo->uReadZoneNo = uZoneNo;
         pInfo->uReadOffset = uOffset;
         return uLen;
         break;
      default:
         return -EFAULT;
      }

   if( uZoneNo == 0) { // BAR0
      if( uOffset + uSize > pInfo->firstBarSize)
         return -EFAULT;
//      printk( KERN_INFO DEVICE_NAME " proc_write(): %c BAR0 %x %x\n", ucType, uOffset, uData);
      switch( uSize) {
         case 1:
            iowrite8( (u8) uData, (u8*)pInfo->firstBar + uOffset);
            break;
         case 2:
            iowrite16( (u16)uData, (u8*)pInfo->firstBar + uOffset);
            break;
         case 4:
            iowrite32( (u32)uData, (u8*)pInfo->firstBar + uOffset);
            break;
         default:
            return -EFAULT;
         }
   }
   else if( uZoneNo == 1) { // second BAR
      if( uOffset + uSize > pInfo->secondBarSize)
         return -EFAULT;
//      printk( KERN_INFO DEVICE_NAME " proc_write(): %c BAR1 %x %x\n", ucType, uOffset, uData);
      switch( uSize) {
         case 1:
            iowrite8( (u8) uData, (u8*)pInfo->secondBar + uOffset);
            break;
         case 2:
            iowrite16( (u16)uData, (u8*)pInfo->secondBar + uOffset);
            break;
         case 4:
            iowrite32( (u32)uData, (u8*)pInfo->secondBar + uOffset);
            break;
         default:
            return -EFAULT;
         }
   }
#ifdef USE_DEBUG_DMA_BUFFER
   else if( uZoneNo == 2) { // dma buffer
      if( pInfo->_pucBuff == NULL)
         return -EFAULT;
      if( uOffset + uSize > DMA_BUFFER_SIZE)
         return -EFAULT;
//      printk( KERN_INFO DEVICE_NAME " proc_write(): %c BAR1 %x %x\n", ucType, uOffset, uData);
      switch( uSize) {
         case 1:
            *((unsigned char*)pInfo->_pucBuff + uOffset) = (unsigned char) uData;
            break;
         case 2:
            *(unsigned short*)((unsigned char*)pInfo->_pucBuff + uOffset) = (unsigned short)uData;
            break;
         case 4:
            *(unsigned long*)((unsigned char*)pInfo->_pucBuff + uOffset) = (unsigned long)uData;
            break;
         default:
            return -EFAULT;
         }
   }
#endif
   else {
      return -EFAULT;
   }

   return uLen;
   }



// Read (ASCII) FPGA communication aera - debug
// Return the DWORD value pointed by 'uReadOffset'.
static int proc_MemRead( char * buff, char ** start, off_t offset, int size, int *eof, void *data)
   {
   PerseusData * pInfo = (PerseusData*)data;

//   printk( KERN_INFO DEVICE_NAME " proc_MemRead(): offset= %u, size= %u, *start= %u\n", (u32)offset, size, (u32)*start);
   if( pInfo->uReadZoneNo == 0) { // BAR0
      if( pInfo->uReadOffset > pInfo->firstBarSize -4) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): out of range, uReadOffset= %Xh, zone= %u\n", pInfo->uReadOffset, pInfo->uReadZoneNo);
         return 0;
         }
      if( size < 9) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): size < 9!\n");
         return 0;
         }
      sprintf( buff, "%08X", (unsigned)ulConvertBusEndianess( ioread32( (u8*)pInfo->firstBar + pInfo->uReadOffset)));
   }
   else if( pInfo->uReadZoneNo == 1) { // second bar - FIFOs
      if( pInfo->uReadOffset > pInfo->secondBarSize -4) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): out of range, uReadOffset= %Xh, zone= %u\n", pInfo->uReadOffset, pInfo->uReadZoneNo);
         return 0;
         }
      if( size < 9) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): size < 9!\n");
         return 0;
         }
      sprintf( buff, "%08X", (unsigned)ulConvertBusEndianess( ioread32( (u8*)pInfo->secondBar + pInfo->uReadOffset)));
   }
#ifdef USE_DEBUG_DMA_BUFFER
   else if( pInfo->uReadZoneNo == 2) { // test DMA buffer
      if( pInfo->_pucBuff == NULL
      || pInfo->uReadOffset > DMA_BUFFER_SIZE -4) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): out of range, uReadOffset= %Xh, zone= %u\n", pInfo->uReadOffset, pInfo->uReadZoneNo);
         return 0;
         }
      if( size < 9) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): size < 9!\n");
         return 0;
         }
      sprintf( buff, "%08X", (unsigned)*(u32*)((u8*)pInfo->_pucBuff + pInfo->uReadOffset));
   }
#endif
   else if( pInfo->uReadZoneNo == 3) { // single queue DMA buffer
      if( pInfo->sqCdma.dataQueue.pvBuffer == NULL
      || pInfo->uReadOffset > pInfo->sqCdma.dataQueue.uBuffSize -4) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): out of range, uReadOffset= %Xh, zone= %u\n", pInfo->uReadOffset, pInfo->uReadZoneNo);
         return 0;
         }
      if( size < 9) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): size < 9!\n");
         return 0;
         }
      sprintf( buff, "%08X", (unsigned)*(u32*)((u8*)pInfo->sqCdma.dataQueue.pvBuffer + pInfo->uReadOffset));
   }

   else if( pInfo->uReadZoneNo == 4) { // single queue descriptors table
//      printk( KERN_INFO DEVICE_NAME " proc_MemRead - zone-4: pDesc= %p, nbDesc= %u, descSize= %lu", pInfo->sqCdma.pDescs, pInfo->sqCdma.uNbDescs, (unsigned long)(sizeof *pInfo->sqCdma.pDescs));
      if( pInfo->sqCdma.descQueue.pvBuffer == NULL
      || pInfo->uReadOffset > pInfo->sqCdma.descQueue.uBuffSize -4) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): out of range, uReadOffset= %Xh, zone= %u\n", pInfo->uReadOffset, pInfo->uReadZoneNo);
         return 0;
         }
      if( size < 9) {
         printk( KERN_INFO DEVICE_NAME " proc_MemRead(): size < 9!\n");
         return 0;
         }
      sprintf( buff, "%08X", (unsigned)ulConvertBusEndianess( *(u32*)((u8*)pInfo->sqCdma.descQueue.pvBuffer + pInfo->uReadOffset)));
   }
   else {
      printk( KERN_INFO DEVICE_NAME " proc_MemRead(): invalid uReadZoneNo= %u\n", pInfo->uReadZoneNo);
      return 0;
   }

//   *eof = 1;
   *start = buff;

   return 9;
   }


// return -1 if beyong end of output buffer
// else return number of chars copied.
static int iCumPrintf( char * buffer, off_t offset, int len, int * pCurrIdx, char * fmt, ...)
{
   char cBuff[ 256];
   int n, left, done, before;
   va_list args;

   if( *pCurrIdx >= offset + len)
      return -1;

   va_start( args, fmt);
   n = vsnprintf( cBuff, sizeof cBuff, fmt, args);
   va_end( args);

   before = done = 0;
   if( *pCurrIdx < offset)
      before = offset - *pCurrIdx;
   else
      done = *pCurrIdx - offset;
   if( before > n) {
      *pCurrIdx += n;
      return 0;
   }
   *pCurrIdx += before;

   n -= before;
   left = len - done;
   if( n > left)
      n = left;

   memcpy( buffer + done, cBuff + before, n);
   *pCurrIdx += n;
   return n;
}

static int proc_DumpState( char * buffer, char ** start, off_t offset, int size, int *eof, void *data)
{
   PerseusData * perseus = (PerseusData*)data;
   unsigned u;
   int n;
//   int iLen;
   int cpy;
   char * buf;
   off_t off;
   int sz;

   PDEBUG( "off= %lu, sz= %u", (unsigned long)off, sz);
   if( offset > 0)
      goto after;

   // take new snap-shot:
   n = 0;
   buf = perseus->acPrintBuff;
   off = 0;
   sz = sizeof perseus->acPrintBuff;
   cpy = iCumPrintf( buf, off, sz, &n, "instance= %u\n", perseus->uInstance);
   cpy = iCumPrintf( buf, off, sz, &n, "first BAR virt addr= x%p\n", perseus->firstBar);
   cpy = iCumPrintf( buf, off, sz, &n, "FIFOs phys addr= %p, total space sz= %u KB\n",
                     (void*)perseus->fifoBaseAddrPhys, perseus->secondBarSize / 1024);

   cpy = iCumPrintf( buf, off, sz, &n, "RX-FIFOs: qty= %u, sz= %u\n", perseus->rxCfg.uNbFifos, perseus->rxCfg.uFifoSizeB);
   for( u = 0; u < min( (unsigned)NB_FIFOS, perseus->rxCfg.uNbFifos); u++) {
      cpy = iCumPrintf( buf, off, sz, &n, "RX-FIFO[ %u]: open= %u, allocated chnl= %u\n",
                        u, perseus->rxFifos[ u].bOpen, (void*)perseus->rxFifos[ u].pAC == NULL ? 99 : perseus->rxFifos[ u].pAC->uChnlNo);
   }

   cpy = iCumPrintf( buf, off, sz, &n, "TX-FIFOs: qty= %u, sz= %u\n", perseus->txCfg.uNbFifos, perseus->txCfg.uFifoSizeB);
   for( u = 0; u < min( (unsigned)NB_FIFOS, perseus->txCfg.uNbFifos); u++) {
      cpy = iCumPrintf( buf, off, sz, &n, "TX-FIFO[ %u]: open= %u, allocated chnl= %u, "
                     " queue busy sz= %u, queue free sz= %u, DM working= %u, current xfer sz= %lu\n",
                     u, perseus->txFifos[ u].bOpen,
                     perseus->txFifos[ u].pAC == NULL ? 99 : perseus->txFifos[ u].pAC->uChnlNo,
                     QUEUE_BUSY_SPACE( &perseus->txFifos[ u].queue), QUEUE_FREE_SPACE( &perseus->txFifos[ u].queue),
                     perseus->dmState[ u].bWorking, (unsigned long)perseus->dmState[ u].size);
   }

   cpy = iCumPrintf( buf, off, sz, &n, "Cdma present: %s\n", perseus->sqCdma.bPresent ? "Yes" : "No");
   cpy = iCumPrintf( buf, off, sz, &n, "uCdmaBarNo= %u, uCdmaBarNoUseCount= %u\n", perseus->uCdmaBarNo, perseus->uCdmaBarNoUseCount);
   for( u = 0; u < NB_FIFOS; u++)
      cpy = iCumPrintf( buf, off, sz, &n, "uDmBarNo[ %u]= %u\n", u, perseus->uDmBarNo[ u]);
   for( u = 0; u < NB_AXI_BARS; u++)
      cpy = iCumPrintf( buf, off, sz, &n, "uBarNoUseCount[ %u]= %u\n", u, perseus->uBarNoUseCount[ u]);

   cpy = iCumPrintf( buf, off, sz, &n, "cdma/sg working= %u, irqErrors= %u, irqIocs= %u, irqDelays= %u, busyErrors= %u\n",
                     perseus->sqCdma.bWorking, perseus->sqCdma.uIrqErrors, perseus->sqCdma.uIrqIocs, perseus->sqCdma.uIrqDelays, perseus->sqCdma.uBusyErrors);
   cpy = iCumPrintf( buf, off, sz, &n, "cdma/sg dataQueue busy size= %u, free size= %u\n", QUEUE_BUSY_SPACE( &perseus->sqCdma.dataQueue),
                     QUEUE_FREE_SPACE( &perseus->sqCdma.dataQueue));
   cpy = iCumPrintf( buf, off, sz, &n, "cdma/sg descQueue busy cnt= %u, free cnt= %u\n", QUEUE_BUSY_SPACE( &perseus->sqCdma.descQueue) /sizeof(CdmaDesc),
                     QUEUE_FREE_SPACE( &perseus->sqCdma.descQueue) /sizeof(CdmaDesc));
   cpy = iCumPrintf( buf, off, sz, &n, "cdma/sg flushThreshold= %u\n", perseus->sqCdma.uFlushThreshold);
   cpy = iCumPrintf( buf, off, sz, &n, "cdma/sg uNbActDesc= %u\n", perseus->sqCdma.uNbActDesc);

   for( u = 0; u < MAX_LOGICAL_CHANNELS; u++) {
      if( perseus->devChannels[ u].bAlloc) {
         cpy = iCumPrintf( buf, off, sz, &n, "------------------\n");
         cpy = iCumPrintf( buf, off, sz, &n, "uChnlNo= %u\n", u);
         cpy = iCumPrintf( buf, off, sz, &n, "ChnlType= %s\n", acTypeNames[ perseus->devChannels[ u].uChnlType]);
         if( perseus->devChannels[ u].uChnlType == TYPE_DEV_TO_DEV) {
            cpy = iCumPrintf( buf, off, sz, &n, "uSrcFifoNo= %u\n", perseus->devChannels[ u].u.dev2dev.uSrcFifoNo);
            cpy = iCumPrintf( buf, off, sz, &n, "uDstFifoNo= %u\n", perseus->devChannels[ u].u.dev2dev.uDstFifoNo);
            cpy = iCumPrintf( buf, off, sz, &n, "uDstDevNo= %u\n", perseus->devChannels[ u].u.dev2dev.uDstDevNo);
         }
         else {
            cpy = iCumPrintf( buf, off, sz, &n, "uFifoNo= %u\n", perseus->devChannels[ u].u.host.uFifoNo);
            cpy = iCumPrintf( buf, off, sz, &n, "uDevNo= %u\n", perseus->devChannels[ u].u.host.uDevNo);
         }
         cpy = iCumPrintf( buf, off, sz, &n, "uFrameSize= %u\n", perseus->devChannels[ u].uFrameSize);
         cpy = iCumPrintf( buf, off, sz, &n, "uState= %u\n", perseus->devChannels[ u].uState);
         cpy = iCumPrintf( buf, off, sz, &n, "transferSize= %lu\n", (unsigned long)perseus->devChannels[ u].transferSize);
         cpy = iCumPrintf( buf, off, sz, &n, "dmaXferedSize= %lu\n", (unsigned long)perseus->devChannels[ u].dmaXferedSize);
         cpy = iCumPrintf( buf, off, sz, &n, "hostXferedSize= %lu\n", (unsigned long)perseus->devChannels[ u].hostXferedSize);
      }
   }
   perseus->uPrintSize = n;
   perseus->acPrintBuff[ sizeof perseus->acPrintBuff -2] = '$';

after:;
   if( offset >= perseus->uPrintSize) {
      *eof = 1;
      return 0;
   }
   if( size + offset > perseus->uPrintSize)
      size  = perseus->uPrintSize - offset;
   *start = buffer;
   memcpy( buffer, perseus->acPrintBuff + offset, size);
   PDEBUG( "uPrintSize= %d, size= %d, *eof= %d", perseus->uPrintSize, size, *eof);
   return size;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,8)
static int proc_MemOpen(struct inode *inode, struct file *file){
	return single_open(file, NULL, PDE_DATA(inode));
}
static ssize_t proc_MemReadWrapper(struct file *file, char __user *buf, size_t count, loff_t *offp)
{
   ssize_t sz;
   char kbuf[9];
   char *start;
   int eof;
   struct seq_file *seq = file->private_data;

   sz = proc_MemRead( kbuf, &start, *offp, count, &eof, seq->private );
   sz = copy_to_user( buf, kbuf, sz );

   return sz;
}

static struct proc_ops proc_perseus_fops = {
   .proc_open    = proc_MemOpen, 
   .proc_read    = proc_MemReadWrapper,
   .proc_write   = proc_MemWrite,
   .proc_release = single_release,
};

static int proc_DumpOpen(struct inode *inode, struct file *file){
	return single_open(file, NULL, PDE_DATA(inode));
}
static ssize_t proc_SumpStateWrapper(struct file *file, char __user *buf, size_t count, loff_t *offp)
{
   ssize_t sz;
   char *kbuf;
   char *start;
   int eof;
   struct seq_file *seq = file->private_data;

   kbuf = kmalloc(count, GFP_KERNEL);
   if (!kbuf) return -ENOMEM;

   sz = proc_DumpState( kbuf, &start, *offp, count, &eof, seq->private );
   sz = copy_to_user( buf, kbuf, sz );

   kfree(kbuf);
   return sz;
}

static struct proc_ops proc_perseus_state_fops = {
   .proc_open    = proc_DumpOpen, 
   .proc_read    = proc_SumpStateWrapper,
   .proc_release = single_release,
};

#else
static int proc_MemOpen(struct inode *inode, struct file *file){
	return single_open(file, NULL, PDE_DATA(inode));
}
static ssize_t proc_MemReadWrapper(struct file *file, char __user *buf, size_t count, loff_t *offp)
{
   ssize_t sz;
   char kbuf[9];
   char *start;
   int eof;
   struct seq_file *seq = file->private_data;

   sz = proc_MemRead( kbuf, &start, *offp, count, &eof, seq->private );
   sz = copy_to_user( buf, kbuf, sz );

   return sz;
}

static struct file_operations proc_perseus_fops = {
   .owner   = THIS_MODULE,
   .open    = proc_MemOpen, 
   .read    = proc_MemReadWrapper,
   .write   = proc_MemWrite,
   .release = single_release,
};

static int proc_DumpOpen(struct inode *inode, struct file *file){
	return single_open(file, NULL, PDE_DATA(inode));
}
static ssize_t proc_SumpStateWrapper(struct file *file, char __user *buf, size_t count, loff_t *offp)
{
   ssize_t sz;
   char *kbuf;
   char *start;
   int eof;
   struct seq_file *seq = file->private_data;

   kbuf = kmalloc(count, GFP_KERNEL);
   if (!kbuf) return -ENOMEM;

   sz = proc_DumpState( kbuf, &start, *offp, count, &eof, seq->private );
   sz = copy_to_user( buf, kbuf, sz );

   kfree(kbuf);
   return sz;
}


static struct file_operations proc_perseus_state_fops = {
   .owner   = THIS_MODULE,
   .open    = proc_DumpOpen, 
   .read    = proc_SumpStateWrapper,
   .release = single_release,
};

#endif

static int perseus_pci_setup_procfs( PerseusData *perseus)
{
	char proc_name[32];

	sprintf( proc_name, PCIE_PROC, perseus->uInstance);
	perseus->proc_instance = proc_mkdir( proc_name, NULL);

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
   perseus->procEntryMemRW = proc_create_data( PCIE_PROC_MEM_RW_NAME, 0666, perseus->proc_instance, &proc_perseus_fops, perseus );
#else
   perseus->procEntryMemRW = create_proc_entry( PCIE_PROC_MEM_RW_NAME, 0666, perseus->proc_instance);
   if( perseus->procEntryMemRW != NULL) {
      perseus->procEntryMemRW->write_proc = proc_MemWrite;
      perseus->procEntryMemRW->read_proc = proc_MemRead;
      perseus->procEntryMemRW->data = perseus;
   }
#endif

   perseus->uReadOffset = 0;
   perseus->uReadZoneNo = 0;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,10,0)
   perseus->procEntryState = proc_create( "state", 0666, perseus->proc_instance, &proc_perseus_state_fops );
#else
   perseus->procEntryState = create_proc_entry( "state", 0666, perseus->proc_instance);
   if( perseus->procEntryState != NULL) {
      perseus->procEntryState->read_proc = proc_DumpState;
      perseus->procEntryState->data = perseus;
      }
#endif

   return 0;
}


static void perseus_pci_cleanup_procfs( PerseusData *perseus)
{
	char proc_name[32];

	remove_proc_entry( "state", perseus->proc_instance);
	remove_proc_entry( PCIE_PROC_MEM_RW_NAME, perseus->proc_instance);

	sprintf( proc_name, PCIE_PROC, perseus->uInstance);
	remove_proc_entry( proc_name, NULL);

}
#endif


static int __devinit perseus_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
   resource_size_t rsStartAddr, rsEndAddr;
	PerseusData *perseus;
	int ret;
	u8 ucRevision;

	PDEBUG("\n");
   if (iPerseus_alloc_data( &perseus))
      return -ENOMEM;
   perseus->uInstance = atomic_add_return( 1, &perseus_count) - 1;
   if( perseus->uInstance >= MAX_NB_DEVS) {
      PDEBUG( "Too many perseus instances, max is %u", MAX_NB_DEVS);
		goto perseus_probe_exit_0;
   }
   PDEBUG( "uInstance= %u, bus= x%x, devfn= x%x \n", perseus->uInstance, pdev->bus->number, pdev->devfn);
   allDevs[ perseus->uInstance].perseus = perseus;

   perseus->pdev = pdev;
   if ((ret = pci_request_region(pdev, FIRST_BAR_NO, DEVICE_NAME))) { // (dev, bar, name)
		PERROR("can't request region BAR%u, error %d\n", FIRST_BAR_NO, ret);
		goto perseus_probe_exit_0;
	}
	if (!(perseus->firstBar = pci_iomap(pdev, FIRST_BAR_NO, 0))) { // (dev, bar, maxSize)
		PERROR("can't iomap region BAR%u\n", FIRST_BAR_NO);
		goto perseus_probe_exit_1;
	}
   rsStartAddr = pci_resource_start( pdev, FIRST_BAR_NO);
   rsEndAddr = pci_resource_end( pdev, FIRST_BAR_NO);
   perseus->firstBarSize = (u32)(rsEndAddr - rsStartAddr + 1);
	PDEBUG("inst-%u, BAR-%u at %p, phys= x%p, size %u\n", perseus->uInstance, FIRST_BAR_NO, perseus->firstBar, (void*)rsStartAddr, perseus->firstBarSize);

   if ((ret = pci_request_region( pdev, SECOND_BAR_NO, DEVICE_NAME))) { // (dev, bar, name)
		PERROR("can't request region BAR%u, error %d\n", SECOND_BAR_NO, ret);
		goto perseus_probe_exit_2;
	}
	if (!(perseus->secondBar = pci_iomap( pdev, SECOND_BAR_NO, 0))) {
		PERROR("can't iomap region BAR%u\n", SECOND_BAR_NO);
		goto perseus_probe_exit_3;
	}
   rsStartAddr = pci_resource_start( pdev, SECOND_BAR_NO);
   rsEndAddr = pci_resource_end( pdev, SECOND_BAR_NO);
   perseus->secondBarSize = (u32)(rsEndAddr - rsStartAddr + 1);
   perseus->fifoBaseAddrPhys = rsStartAddr;
	PDEBUG("inst-%u, BAR-%u at %p, phys= x%p, size %u\n", perseus->uInstance, SECOND_BAR_NO, perseus->secondBar, (void*)rsStartAddr, perseus->secondBarSize);

	if ((ret = pci_enable_device( pdev))) {
		PERROR("can't enable device, error %d\n", ret);
		goto perseus_probe_exit_4;
	}

	if (pdev->slot) {
		PDEBUG("slot number %d\n", pdev->slot->number);
	}

   spin_lock_init( &perseus->cdmaSpinlock);
   spin_lock_init( &perseus->dmSpinlock);
   spin_lock_init( &perseus->miscRegsSpinlock);

   init_waitqueue_head( &perseus->inq);
   init_waitqueue_head( &perseus->outq);
   sema_init( &perseus->rdSem, 1);
   sema_init( &perseus->wrSem, 1);
   sema_init( &perseus->ctlSem, 1);

   // get pdev->irq:
   if ((ret = pci_enable_msi( perseus->pdev))) {
		PERROR("failed to enable MSI\n");
      goto perseus_probe_exit_7;
	}

   if ((ret = request_irq( perseus->pdev->irq, IrqHandler, 0/*IRQF_SHARED*/, DEVICE_NAME, perseus))){
		PERROR("failed to request irq\n");
      goto perseus_probe_exit_7_5;
	}

	perseus_init_data( perseus);

#ifdef PERSEUS_PROCFS
	perseus_pci_setup_procfs(perseus);
#endif

	if( perseus_create_mailbox_ndev(perseus))
      goto perseus_probe_exit_8;

	if( rtdex_create_cdev(perseus)) goto perseus_probe_exit_8;

   PDEBUG("setting drvdata\n");
	pci_set_drvdata( pdev, perseus);

	pci_read_config_byte( pdev, PCI_REVISION_ID, &ucRevision);
   PDEBUG("hardware revision %d\n", ucRevision);
	PDEBUG("name %s\n", pci_name(pdev));

	pci_set_master( perseus->pdev);
   return 0;

perseus_probe_exit_8:
	free_irq(perseus->pdev->irq, perseus);
perseus_probe_exit_7_5:
	pci_disable_msi(pdev);
perseus_probe_exit_7:
	pci_disable_device(pdev);
perseus_probe_exit_4:
	pci_iounmap(pdev, perseus->secondBar);
perseus_probe_exit_3:
	pci_release_region(pdev, SECOND_BAR_NO);
perseus_probe_exit_2:
	pci_iounmap(pdev, perseus->firstBar);
perseus_probe_exit_1:
	pci_release_region( pdev, 0);
perseus_probe_exit_0:
	perseus_free_data( perseus);
   atomic_add( -1, &perseus_count);
	return -EFAULT;
}


static void __devexit perseus_remove(struct pci_dev *pdev)
{
	PerseusData *perseus;

	PDEBUG("\n");

	perseus = pci_get_drvdata(pdev);

	free_irq(perseus->pdev->irq, perseus);
	pci_disable_msi(pdev);

   perseus_destroy_mailbox_ndev( perseus);

   rtdex_destroy_cdev( perseus);

#ifdef PERSEUS_PROCFS
	perseus_pci_cleanup_procfs( perseus);
#endif

	pci_iounmap( pdev, perseus->firstBar);
	pci_release_region( pdev, FIRST_BAR_NO);
	pci_iounmap( pdev, perseus->secondBar);
	pci_release_region( pdev, SECOND_BAR_NO);

	pci_disable_device( pdev);
	pci_set_drvdata( pdev, NULL);

   perseus_free_data( perseus);
   atomic_add( -1, &perseus_count);

	PDEBUG( "removed\n");
}


static struct pci_driver pci_driver = {
	.name = DEVICE_NAME,
	.id_table = perseus_pci_id_table,
	.probe = perseus_probe,
	.remove = perseus_remove,
};


/***********************************************************************
 * 
 * Module functions
 *
 **********************************************************************/

static int __init perseus_module_init(void)
{
	PDEBUG("compiled " __DATE__ ", " __TIME__ "\n");

   perseus_class = class_create(THIS_MODULE, DEVICE_NAME);
	rtdex_class = class_create( THIS_MODULE, RTDEX_NAME);

	atomic_set( &perseus_count, 0);
   sema_init( &fifoSem, 1);

   memset( &allDevs, 0, sizeof allDevs);

   PDEBUG( "cdmasizek= %u", cdmasizek);
   if( !cdmasizek) {
      cdmasizek = DEF_SQ_CDMA_BUFF_SIZE_K;
      PDEBUG( "Invalid cdmasizek, fixed to %u", cdmasizek);
   }
   if( cdmasizek > MAX_SQ_CDMA_BUFF_SIZE_K) {
      PDEBUG( "Invalid cdmasizek, fixed to %u", MAX_SQ_CDMA_BUFF_SIZE_K);
      cdmasizek = MAX_SQ_CDMA_BUFF_SIZE_K;
   }

   PDEBUG( "dmsizek= %u", dmsizek);
   if( !dmsizek) {
      dmsizek = DEF_DM_BUFF_SIZE_K;
      PDEBUG( "Invalid dmsizek, fixed to %u", dmsizek);
   }
   if( dmsizek > MAX_DM_BUFF_SIZE_K) {
      PDEBUG( "Invalid dmsizek, fixed to %u", MAX_DM_BUFF_SIZE_K);
      dmsizek = DEF_DM_BUFF_SIZE_K;
   }

	return pci_register_driver( &pci_driver);
}


static void __exit perseus_module_exit(void)
{
	PDEBUG("\n");

   PDEBUG( "unregistering pci driver\n");
	pci_unregister_driver( &pci_driver);

	PDEBUG( "destroying classes\n");
	class_destroy( rtdex_class);
	class_destroy( perseus_class);

	PDEBUG( "egress\n");
}

MODULE_LICENSE("GPL");

module_init(perseus_module_init);
module_exit(perseus_module_exit);
