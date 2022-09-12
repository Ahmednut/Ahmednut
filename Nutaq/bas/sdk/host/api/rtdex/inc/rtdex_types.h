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
 * @file    rtdex_types.h
 * @brief   RTDEx error definitions
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

#ifndef ADP_RTDEX_TYPESX_H
#define ADP_RTDEX_TYPESX_H

#ifdef __cplusplus
extern "C" {
#endif


/** @name RTDEx transfer direction
 * 
 *  Possible RTDEx transfer direction.  
 */
typedef enum {
   eToFpga, ///< Transfer is from peer device to FPGA
   eFromFpga, ///< Transfer is from FPGA to peer device
} eRTDExTransferDir;


/** @name RTDEx modes
 * 
 *  Possible RTDEx modes
 */
typedef enum {
   eSingle,		///< Transfer has a finite size
   eContinuous, ///< Transfer is infinite
} eRTDExMode;


/** @name RTDEx receive timeout modes
 * 
 *  Possible RTDEx receive timeout modes
 */
typedef enum {
   eRTDExWaitForever, ///< Receiver waits until arrival of packet
   eRTDExWaitTimeout, ///< Receiver waits until arrival of packet or until timeout
   eRTDExDontWait,    ///< Receiver does not wait if no packet has been received
   eRTDExWaitTimeoutOrFirstError    ///< Receiver waits until arrival of packet or until timeout or when a packet dropped has been detected
} eRTDExTransferMode;

typedef void * adp_handle_rtdex_media_conn;


// Base handle types for RTDEX.

/** @name RTDEx Error codes
 * 
 *  List all runtime related error codes of a RTDEx module.
 */
enum RTDEX_error {
    RTDEXOK    = 0,			///< No error
	RTDEXINVHANDLE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x01),			///< Invalid RTDEx Handle is used
	RTDEXINVCONTEXT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x02),			///< Invalid RTDEx context
	RTDEXMEMALLOC = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x03),			///< Memory allocation failed for RTDEx object
	RTDEXINVCHANNELNUM = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x04),		///< Channel number used is not available on FPGA
	RTDEXINVMUTEXHANDLE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x05),		///< Invalid RTDEx mutex handle
	RTDEXMUTEXINIT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x06),			///< Mutex error - init
	RTDEXMUTEXLOCK = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x07),			///< Mutex error - lock
	RTDEXMUTEXUNLOCK = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x08),		    ///< Mutex error - unlock
	RTDEXCONDINIT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x09),			///< RTDEx initial condition
	RTDEXCONDWAIT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x0A),			///< RTDEx waiting condition
	RTDEXPTHREADARRAY = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x0B),		///< RTDEx thread array
	RTDEXINVFRAMESIZE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x0C),		///< Invalid frame size is used
	RTDEXTRANSFERSIZENOTFRAMESIZEMULTIPLE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x0D),	///< Transfer size is not a multiple of frame size
	RTDEXBUFFERSIZETOOSMALL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x0E),	///< Buffer size is too small
	RTDEXWAITTHREADCALL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x0F),      ///< Wait thread call
	RTDEXWRONGDIRECTION = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x10),	///< RTDEx channel handle is being used in the wrong direction
	RTDEXPCAPCOMPILE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x11),		///< PCAP compile error
	RTDEXPCAPSETFILTER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x12),		///< PCAP receive filter error
	RTDEXPCAPFINDALLDEVS = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x13),	///< PCAP could not verify network device
	RTDEXINTERFACEITERATOR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x14), ///< Could not verify network interface
	RTDEXDEVICEITERATOR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x15),	///< Could not verify network device
	RTDEXADAPTORSINFO = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x16),		///< Could not get network adaptors info
	RTDEXTHREADISDEAD = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x17),      ///< Thread is dead
	RTDEXINVINTERFACEDMAHANDLE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x18), ///< Could match networkdevice to MAC address
	RTDEXINVSOURCEINTERFACE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x19),    ///< Network interface used is not valid
	RTDEXSENDPACKET = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x1A),			///< Packet send error
	RTDEXMEDIANOTSUPPORTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x1B),		///< Media not supported
	RTDEXINVALIDBUFFER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x1C),			///< Invalid data buffer
	RTDEXINVALIDBUFFERSIZE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x1D),		///< Invalid buffer size
	RTDEXFRAMESIZETOOBIG = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x1E),	    ///< Frame size is too big
	RTDEXPCAPSEND = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x1F),				///< PCAP send error
   RTDEXBADCOREIDORVER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x20), ///< Invalid RTDEX core ID/Ver
   RTDEXRESSOURCEBUSY = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x21), ///< requested ressource busy
   RTDEXINVALBUSACCESS = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x22), ///< invalid bus access handle
   RTDEXTRANSFERTSIZETOOBIG = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x23), ///< transfert size too big
   RTDEXTOOMANYDEVS = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x24), ///< too many perseus devices
   RTDEXINVALRESSOURCE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x25), ///< invalid ressource #
   RTDEXINVALHOSTMACADDR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x26), ///< invalid Host MAC addr
   RTDEXPEERMACADDRCONFLICT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x27), ///< target MAC address conflict

   RTDEXPCIEPERSEUS2CPUFAIL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x28), ///< PCIe perseus to cpu driver command error
   RTDEXPCIEINTERR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x29), ///< PCIe internal error
   RTDEXPCIESYSERR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x2A), ///< PCIe system error
   RTDEXPCIEOPEN = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x2B), ///< PCIe system error - open
   RTDEXPCIEREAD = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x2C), ///< PCIe system error - read
   RTDEXPCIEWRITE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x2D), ///< PCIe system error - write
   RTDEXPCIEIOCTL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x2E), ///< PCIe system error - ioctl
   RTDEXPCIEEOF = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x30), ///< PCIe end of file reading or writing
   RTDEXPCIETIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x31), ///< PCIe timeout reading or writing

   RTDEXRECEIVESENDSIZEISTOLARGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x32), ///< Receive size to large
   RTDEXOLDETHNOTSUPPORTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x33), ///< Not suported (Perseus to Perseus - old eth interface)
   RTDEXBADTESTLOGICCOREID = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x34), ///< Bad Test Logic Core-Id 

   RTDEXTCPIPWSASTART = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x35), ///< TCP/IP - function WSAStartup
   RTDEXTCPIPSOCKET = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x36), ///< TCP/IP - Cannot open socket
   RTDEXTCPIPSETSOCK = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x37), ///< TCP/IP - function setsockopt
   RTDEXTCPIPSERVER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x38), ///< TCP/IP - function gethostbyname
   RTDEXTCPIPCONNECT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x39), ///< TCP/IP - Cannot connect to test server
   RTDEXTCPIPSEND = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x3A), ///< TCP/IP - Error sending. Got disconnected?
   RTDEXTCPIPRECEIVE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x3B), ///< TCP/IP - Error receiving. Got disconnected?
   
   RTDEXAXIOPEN = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x3C), ///< AXI - cannot open RTDEx character device
   RTDEXAXISEND = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x3D), ///< AXI - error writing to FIFO
   RTDEXAXIRECEIVE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x3E), ///< AXI - error reading from FIFO
   
   RTDEXPCIEDRIVER_FIRST = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0x80), ///< PCIe kernel driver first error - see "rtdexpcie_drviface.h"
   RTDEXPCIEDRIVER_LAST = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RTDEX, 0xA0), ///< PCIe kernel driver last error
	};

#ifdef __cplusplus
}
#endif

#endif // ADP_RTDEX_TYPESX_H
