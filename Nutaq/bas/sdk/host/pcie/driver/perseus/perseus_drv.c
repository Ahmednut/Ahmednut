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
 *  File        : perseus-pci.c
 *  Description : Linux Kernel Driver for RTDEX PCIe on Perseus
 *
 *  Author(s)   : Marc Savary
 *
 *                Copyright (c) Nutaq Inc, 2012
 *
 ***************************************************************************
*/
/*
 * Author: Eric Moreau <eric.moreau@nutaq.com>
 *         Marc Savary
 *
 * Source for perseus-pci module on PCIe enabled
 * platforms, and perseus_mailbox module on MicroBlaze.
 *
 * A number of procfs entries are declared for debugging
 * only, they should be removed in a production environment.
 *
 * TODO:
 *
 * - Error checking is deficient in many places,
 *   along with undoing what we done in case
 *   of failure.
 * - Locking is deficient. Check documentation
 *   on network devices and NAPI for references.
 * - Interrupts shall be used (along with polling
 *   if NAPI is used)
 * - On MicroBlaze, the device tree should be
 *   used (see of_get_property()) instead of
 *   fixed values.
 *
 */

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


#include "perseus2hostregs.h"



/***************************************************************/
/****************************************************************
 *
 * The following sections should be kept in order
 *
 ***************************************************************/
/***************************************************************/

/**************** First level configuration variables **********/

//#define PERSEUS_DEBUG

//#define BYPASS_REGION_DPRAM

/*
 * The folloing defines shall be
 * for debugging purposes only
 *
 */
#define PERSEUS_PROCFS

#define USE_MEM_RW_PROC_FS // set this define to enable PROC-FS read/write to debug
#ifdef USE_MEM_RW_PROC_FS
 #define PCIE_PROC_MEM_RW_NAME "pcieMemRW"
#endif


/********* End of first level configuration variables **********/

/**************** Module name *****************************************/
#define DEVICE_NAME "perseus-mailbox"

/********* End of module name *****************************************/

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
#  ifdef __KERNEL__
#    define PERROR(fmt, args...) printk( KERN_ERR DEVICE_NAME ": %s " fmt, __func__, ## args)
#  else
#    define PERROR(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif

#undef PINFO
#  ifdef __KERNEL__
#    define PINFO(fmt, args...) printk( KERN_INFO DEVICE_NAME ": %s " fmt, __func__, ## args)
#  else
#    define PINFO(fmt, args...) fprintf(stderr, fmt, ## args)
#  endif

/**************** End of debug function *******************************/

/* FIXME: Most of the following values should be fetched from the
 *        device tree instead, using the of_get_property()
 *        function
 */


#define IRQ_NO 8


struct class *perseus_class;


typedef struct {
	int instance;
#ifdef PERSEUS_PROCFS
	struct proc_dir_entry *proc_perseus;
	struct proc_dir_entry *proc_instance;
	struct proc_dir_entry *proc_interrupt;
	struct proc_dir_entry *proc_pcie_bridge;
	struct proc_dir_entry *proc_config;
 #ifdef USE_MEM_RW_PROC_FS
   struct proc_dir_entry * procEntryMemRW;
   unsigned uReadOffset;
   unsigned uZoneNo;
 #endif
#endif
	int pcie_irq;
   void __iomem *stdCtrl;   // standard control regs
	void __iomem *dpRam;
	void __iomem *inbox;
	void __iomem *outbox;
//	void __iomem *mRegs;
	struct net_device *ndev;
//	struct sk_buff *mailbox_skb;
   unsigned dpRamSize;
} PerseusData;


atomic_t perseus_count;



#ifdef USE_MEM_RW_PROC_FS

#define MODULE_NAME DEVICE_NAME


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
// "B bar offset data"   -- write a byte
// "W bar offset data"   -- write a short
// "D bar offset data"   -- write a dword
// 'P' bar offset        -- set pointer for direct read via proc_MemRead()
// all constants in hex
static int proc_MemWrite( struct file* file, const char * buffer, unsigned long count, void * data)
   {
   char bf[ 50];
   unsigned uLen;
   unsigned uZoneNo, uOffset, uData, uSize;
   unsigned char ucType;
   PerseusData * pInfo = (PerseusData*)data;

//printk( KERN_INFO MODULE_NAME " data = %Xh\n", (u32)data);
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
         pInfo->uZoneNo = uZoneNo;
         pInfo->uReadOffset = uOffset;
         return uLen;
         break;
      default:
         return -EFAULT;
      }

   if( uZoneNo == 0) {
      if( uOffset + uSize > pInfo->dpRamSize)
         return -EFAULT;
      printk( KERN_INFO MODULE_NAME " proc_write(): %c BAR0 %x %x\n", ucType, uOffset, uData);
      switch( uSize) {
         case 1:
            *((unsigned char*)pInfo->dpRam + uOffset) = (unsigned char) uData;
            break;
         case 2:
            *(unsigned short*)((unsigned char*)pInfo->dpRam + uOffset) = usConvertBusEndianess( (unsigned short)uData);
            break;
         case 4:
            *(unsigned long*)((unsigned char*)pInfo->dpRam + uOffset) = ulConvertBusEndianess( (unsigned long) uData);
            break;
         default:
            return -EFAULT;
         }
   }
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

   printk( KERN_INFO MODULE_NAME " proc_MemRead(): offset= %u, size= %u, *start= %u\n", (u32)offset, size, (u32)*start);

   if( pInfo->uZoneNo == 0) {
      if( pInfo->uReadOffset > pInfo->dpRamSize -4) {
         printk( KERN_INFO MODULE_NAME " proc_MemRead(): out of range, uReadOffset= %Xh\n", pInfo->uReadOffset);
         return 0;
         }
      if( size < 9) {
         printk( KERN_INFO MODULE_NAME " proc_MemRead(): size < 9!\n");
         return 0;
         }
      sprintf( buff, "%08X", (unsigned)ulConvertBusEndianess( *(u32*)((u8*)pInfo->dpRam + pInfo->uReadOffset)));
   }
   else {
      printk( KERN_INFO MODULE_NAME " proc_MemRead(): invalid BAR-Num\n");
      return 0;
   }

//   *eof = 1;
   *start = buff;

   return 9;
   }

#endif // USE_MEM_RW_PROC_FS




/*
 
These are just examples of MODULE_PARM_DESC()

static unsigned io_limit = 128;
module_param(io_limit, uint, 0);
MODULE_PARM_DESC(io_limit, "Maximum bytes per I/O (default 128)");

module_param_string(eata, boot_options, MAX_BOOT_OPTIONS_SIZE, 0);
MODULE_PARM_DESC(eata, " equivalent to the \"eata=...\" kernel boot option."
                 "            Example: modprobe eata \"eata=0x7410,0x230,lc:y,tm:0,mq:4,ep:n\"");

*/



// Trig an irq to Host:
static inline void perseus_irq_rise(PerseusData *perseus)
{
	uint32_t uVal, addr;

   addr = perseus->stdCtrl + STD_CTRL_PCIE_CTRL_STAT;
   uVal = ioread32( addr);
	iowrite32( uVal | (1 << PCIECTRL_PCIEMSI_BN), addr);
	iowrite32( uVal & ~(1 << PCIECTRL_PCIEMSI_BN), addr);
}



/***********************************************************************
 * 
 * Network device functions
 *
 **********************************************************************/

static int perseus_net_set_mac_address(struct net_device *ndev, void *p)
{
	PDEBUG("\n");
        eth_mac_addr(ndev, p);
        return 0;
}

#ifdef CONFIG_NET_POLL_CONTROLLER
static void perseus_net_poll_controller(struct net_device *ndev)
{
	PDEBUG("\n");
}
#endif

extern void perseus_netdev_setup(struct net_device *ndev)
{
	ether_setup(ndev);
}



static int perseus_mailbox_net_change_mtu(struct net_device *ndev, int new_mtu)
{
//	if (new_mtu > MAILBOX_SIZE - 64) {
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
	PerseusData * perseus = *(PerseusData**)netdev_priv( ndev);

	PDEBUG("\n");
//	iowrite16( 0, perseus->inbox + MAILBOX_MSG_LEN16);
   MBX_RESET( (MailboxStruc*)perseus->inbox);
   netif_start_queue(ndev);
	return 0;
}

static int perseus_mailbox_net_stop(struct net_device *ndev)
{
	PDEBUG("\n");
	netif_stop_queue(ndev);
	return 0;
}


//!!!
static int perseus_mailbox_net_tx(struct sk_buff *skb, struct net_device *ndev)
{
	PerseusData * perseus = *(PerseusData**)netdev_priv( ndev);
   MailboxStruc * pMbx = (MailboxStruc*)perseus->outbox;
   unsigned uIdx;

   if( skb->len > MAILBOX_MSG_DATA_LEN) {
      PDEBUG( "perseus_mailbox_net_tx(): skb->len too big!\n");
      ndev->stats.tx_dropped++;
      return -EFAULT;
	}

   //PDEBUG("%d\n", skb->len);
//   if( 0 != ioread16( perseus->outbox + MAILBOX_MSG_LEN16)) {
   if( 0 == MBX_FREE_SPACE( pMbx)) {
      PDEBUG("perseus_mailbox_net_tx(): cannot send, outbox full!\n");
		return -EBUSY;
   }
   uIdx = MBX_FREE_IDX( pMbx);

	memcpy_toio( pMbx->aMsgs[ uIdx].au8Data, skb->data, skb->len);
	iowrite16( skb->len, &pMbx->aMsgs[ uIdx].u16Size);
   MBX_PUT( pMbx, 1);
	perseus_irq_rise( perseus);

   ndev->stats.tx_packets++;
	ndev->stats.tx_bytes += skb->len;
   dev_kfree_skb( skb);

   return 0;
}


static void perseus_mailbox_net_rx(PerseusData * perseus)
{
	struct sk_buff *skb;
	ssize_t size;
	int ret;
   MailboxStruc * pMbx = (MailboxStruc*)perseus->inbox;
   unsigned uIdx;

while( 1) {
   if( 0 == MBX_BUSY_SPACE( pMbx))
      return;

   uIdx = MBX_BUSY_IDX( pMbx);
   size = pMbx->aMsgs[ uIdx].u16Size;

	skb = dev_alloc_skb(size + 2);
	if (!skb) {
		perseus->ndev->stats.rx_dropped++;
		PDEBUG("no skb\n");
		goto perseus_mailbox_net_rx_out;
	}
	skb_reserve(skb, 2);
	memcpy_fromio( skb_put(skb, size), pMbx->aMsgs[ uIdx].au8Data, size);

	//PDEBUG("%d\n", skb->len);

	skb->dev = perseus->ndev;
	skb->protocol = eth_type_trans(skb, perseus->ndev);
	skb->ip_summed = CHECKSUM_UNNECESSARY;
	perseus->ndev->stats.rx_packets++;
	perseus->ndev->stats.rx_bytes += skb->len;
	// print_hex_dump(KERN_DEBUG, DEVICE_NAME ": ", DUMP_PREFIX_OFFSET, 72, 1, skb->data, skb->len, 1);
	if ((ret = netif_rx(skb))) {
		perseus->ndev->stats.rx_errors++;
		PERROR("netif_rx: %d\n", ret);
	}

perseus_mailbox_net_rx_out:
   MBX_GET( pMbx, 1);

	perseus_irq_rise( perseus); // MSA-19apr2013
}

   return;
}


// !!!
static irqreturn_t perseus_irq_handler(int irq, void *dev_id)
{
	PerseusData * perseus = (PerseusData *)dev_id;
	int rise = 0;

   if (perseus->pcie_irq == irq) {
//      u32 u32Req;

		PDEBUG("PCIe IRQ (%d)\n", irq);

      // test for mailbox:
      if( MBX_BUSY_SPACE((MailboxStruc*)perseus->inbox)) {
			perseus_mailbox_net_rx( perseus);
		}

	} else
      PDEBUG("IRQ %d\n", irq);

   if( rise)
   	perseus_irq_rise( perseus);

   return IRQ_HANDLED;
}


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


static int perseus_create_mailbox_ndev(PerseusData * perseus)
{
	PerseusData **priv;
/* FIXME: get mac address from device tree */
	unsigned char mac_address[8] = {0x00, 0x00, 0x00, 0xd0, 0xcc, 0xba, 0xbe, 0x02 };

	PDEBUG("\n");
	perseus->ndev = alloc_netdev(sizeof(PerseusData *), "mailbox%d", perseus_netdev_setup);
	if (!(perseus->ndev)) {
		PERROR("can't allocate net device\n");
		return -ENOMEM;
	}
	perseus_netdev_setup(perseus->ndev);
	perseus->ndev->netdev_ops = &perseus_mailbox_ndev_ops;
	perseus->ndev->flags &= ~IFF_MULTICAST;
	perseus->ndev->mtu = MAILBOX_MSG_DATA_LEN; // MAILBOX_SIZE - 64;
	priv = netdev_priv(perseus->ndev);
	*priv = perseus;
	
	PDEBUG("net device at %p\n", perseus->ndev);
	if (register_netdev(perseus->ndev)) {
		PERROR("can't register net device\n");
		return -ENODEV;
	}

	perseus_net_set_mac_address(perseus->ndev, mac_address);
	PDEBUG("returning\n");

	return 0;
}

static int perseus_destroy_mailbox_ndev(PerseusData *perseus)
{
	unregister_netdev(perseus->ndev);
	free_netdev(perseus->ndev);
	PDEBUG("net device destroyed\n");
	return 0;
}



/***********************************************************************
 * 
 * Character device functions
 *
 **********************************************************************/




/***********************************************************************
 * 
 * Device type-agnostic functions
 *
 **********************************************************************/

static PerseusData *perseus_alloc_data(PerseusData **perseus)
{
	*perseus = kmalloc(sizeof(PerseusData), GFP_KERNEL);
	if (!(*perseus)) {
		PDEBUG("can't allocate memory for perseus\n");
		goto perseus_alloc_exit_0;
	}
	memset(*perseus, 0, sizeof(PerseusData));
	PDEBUG("private data at %p\n", *perseus);
	PDEBUG("private data sized %d bytes\n", sizeof(PerseusData));
	return *perseus;

	kfree(*perseus);

perseus_alloc_exit_0:
	return 0;
}

static void perseus_free_data(PerseusData *perseus)
{
	kfree(perseus);
}

static void perseus_init_data(PerseusData *perseus)
{
   u32 u32Val;

	PDEBUG("\n");
//	perseus->mRegs = perseus->dpRam + REG_OFFSET;
	perseus->inbox = perseus->dpRam + MAILBOXES_OFFSET + TO_FPGA_MAILBOX   * MAILBOX_SIZE;
	perseus->outbox = perseus->dpRam + MAILBOXES_OFFSET + FROM_FPGA_MAILBOX * MAILBOX_SIZE;
	PDEBUG("inbox  at %p\n", perseus->inbox);
	PDEBUG("outbox at %p\n", perseus->outbox);
	memset_io(perseus->inbox,   0, MAILBOX_SIZE);
	PDEBUG("inbox cleared\n");
//	memset_io(perseus->mRegs,  0, REG_SIZE);
//	PDEBUG("mRegs cleared\n");

//   iowrite32( 0, perseus->mRegs + REG32_UBLAZE_DONE_COUNT);

   PDEBUG("done\n");
}



#ifdef PERSEUS_PROCFS

static PerseusData *perseus;

static int perseus_pcie_bridge_config_proc_read(char *buf, char **start, off_t off,int count, int *eof, void *data)
{
	PerseusData *perseus = (PerseusData *)data;
//	if (count > PCIE_BRIDGE_LIMIT) count = PCIE_BRIDGE_LIMIT;
//	memcpy_fromio(buf, perseus->pcie_bridge, count);
	*eof = 1;
	return count;
}

static int perseus_pcie_bridge_msi_proc_read(char *buf, char **start, off_t offset, int count, int *eof, void *data )
{
	*eof = 1;
	return 0;
}

static int perseus_pcie_bridge_msi_proc_write(struct file *file, const char __user *buf, unsigned long count, void *data )
{
	PerseusData * perseus = (PerseusData *)data;

	perseus_irq_rise(perseus);

	return count;
}


static int perseus_nonpci_setup_procfs( PerseusData * perseus)
{
	char proc_name[32];

	perseus->proc_perseus = proc_mkdir(DEVICE_NAME, NULL);
	sprintf(proc_name, "%d", perseus->instance);
	perseus->proc_instance = proc_mkdir(proc_name, perseus->proc_perseus);
	perseus->proc_pcie_bridge = proc_mkdir("pcie_bridge", perseus->proc_instance);
	perseus->proc_config = create_proc_read_entry("config", 0, perseus->proc_pcie_bridge, perseus_pcie_bridge_config_proc_read, perseus);
	perseus->proc_interrupt = create_proc_entry("interrupt", 0666, perseus->proc_pcie_bridge);
	perseus->proc_interrupt->read_proc = perseus_pcie_bridge_msi_proc_read;
	perseus->proc_interrupt->write_proc = perseus_pcie_bridge_msi_proc_write;
	perseus->proc_interrupt->data = perseus;

#ifdef USE_MEM_RW_PROC_FS
   perseus->procEntryMemRW = create_proc_entry( PCIE_PROC_MEM_RW_NAME, 0666, perseus->proc_instance);
   if( perseus->procEntryMemRW != NULL) {
      perseus->procEntryMemRW->write_proc = proc_MemWrite;
      perseus->procEntryMemRW->read_proc = proc_MemRead;
      perseus->procEntryMemRW->data = perseus;
      }
   perseus->uReadOffset = 0;
   perseus->uZoneNo = 0;
#endif
   return 0;
}

static void perseus_nonpci_cleanup_procfs( PerseusData * perseus)
{
	char proc_name[32];

#ifdef USE_MEM_RW_PROC_FS
   remove_proc_entry( PCIE_PROC_MEM_RW_NAME, perseus->proc_instance);
#endif
   remove_proc_entry("interrupt", perseus->proc_pcie_bridge);
	remove_proc_entry("config", perseus->proc_pcie_bridge);
	remove_proc_entry("pcie_bridge", perseus->proc_instance);
	sprintf(proc_name, "%d", perseus->instance);
	remove_proc_entry(proc_name, perseus->proc_perseus);
	remove_proc_entry(DEVICE_NAME, NULL);
}
#endif
	

static int perseus_nonpci_init (void)
{
	int ret;

	if (!(perseus_alloc_data(&perseus))) return -ENOMEM;
#ifndef BYPASS_REGION_DPRAM
	if (! request_mem_region(DPRAM_START, DPRAM_SIZE, DEVICE_NAME)) {
		PERROR("can't request dpRam mem region\n");
		goto perseus_nonpci_exit_0;
	}
#endif
   if (!(perseus->dpRam = ioremap_nocache(DPRAM_START, DPRAM_SIZE))) {
		PERROR("can't iomap dpRam");
		goto perseus_nonpci_exit_1;
	}
	PDEBUG("dpRam at %p\n", perseus->dpRam);
   perseus->dpRamSize = DPRAM_SIZE;

	if (! request_mem_region(STD_CTRL_REGS_START, STD_CTRL_REGS_SIZE, DEVICE_NAME)) {
		PERROR("can't request stdCtrl mem region\n");
	}
   if (!(perseus->stdCtrl = ioremap_nocache( STD_CTRL_REGS_START, STD_CTRL_REGS_SIZE))) {
		PERROR("can't iomap stdCtrl");
	}
	PDEBUG("stdCtrl at %p\n", perseus->stdCtrl);

	perseus->instance = atomic_add_return(1, &perseus_count) - 1;

	perseus_init_data(perseus);

#ifdef PERSEUS_PROCFS
	perseus_nonpci_setup_procfs(perseus);
#endif

	perseus->pcie_irq = IRQ_NO;

	if ((ret = request_irq(perseus->pcie_irq, perseus_irq_handler, IRQF_SHARED, DEVICE_NAME "-pcie_bridge", perseus))) {
		PERROR("failed to request pcie irq\n");
	}

	if (perseus_create_mailbox_ndev(perseus)) goto perseus_nonpci_exit_5;
	return 0;

perseus_nonpci_exit_5:

/*FIXME: error handling*/

   iounmap(perseus->stdCtrl);

perseus_nonpci_exit_3:
	release_mem_region(STD_CTRL_REGS_START, STD_CTRL_REGS_SIZE);

perseus_nonpci_exit_2:
	iounmap(perseus->dpRam);

perseus_nonpci_exit_1:
	release_mem_region(DPRAM_START, DPRAM_SIZE);

perseus_nonpci_exit_0:
	perseus_free_data(perseus);
	return -ENOMEM;
}


static void perseus_nonpci_exit(void)
{
	PDEBUG("\n");

	free_irq(perseus->pcie_irq, perseus);

	perseus_destroy_mailbox_ndev(perseus);

#ifdef PERSEUS_PROCFS
	perseus_nonpci_cleanup_procfs(perseus);
#endif

   iounmap(perseus->stdCtrl);
   release_mem_region( STD_CTRL_REGS_START, STD_CTRL_REGS_SIZE);

   iounmap(perseus->dpRam);
	release_mem_region(DPRAM_START, DPRAM_SIZE);

   perseus_free_data(perseus);
}


/***********************************************************************
 * 
 * Module functions
 *
 **********************************************************************/

static int __init perseus_module_init(void)
{
	PINFO("compiled " __DATE__ ", " __TIME__ "\n");
   
   perseus_class = class_create(THIS_MODULE, DEVICE_NAME);
	
	atomic_set(&perseus_count, 0);

	return perseus_nonpci_init();
}

static void __exit perseus_module_exit(void)
{
	PDEBUG("\n");
	perseus_nonpci_exit();

	PDEBUG("destroying classes\n");
	class_destroy(perseus_class);

	PDEBUG("egress\n");
}

MODULE_LICENSE("GPL");

module_init(perseus_module_init);
module_exit(perseus_module_exit);
