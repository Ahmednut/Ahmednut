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
 *  Project     : adp rtdex PCIe
 *  File        : rtdex_pcie.c
 *  Description : Low level interface to Perseus RTDEX PCIe.
 *
 *  Author(s)   : Marc Savary
 *
 *                Copyright (c) Nutaq Inc, 2012
 *
 ***************************************************************************
*/

#ifdef __linux__

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>

#include "bas_error.h"
#include "rtdex_types.h"

#include "perseus2hostregs.h"
#include "rtdexpcie_drviface.h"
#include "rtdex_pcie.h"


#ifdef RTDEX_DEBUG
#  define PERROR( str) perror( str)
#  define PDEBUG(fmt, args...) printf( "%s(): " fmt, __func__, ## args)
#  define PDEBUGA(fmt, args...) printf(fmt, ## args)
#else
#  define PERROR( str)
#  define PDEBUG(fmt, args...)
#  define PDEBUGA(fmt, args...)
#endif




#if rtdxpkdrv_LAST_ERROR > ((0xFFFF & RTDEXPCIEDRIVER_LAST) - (0xFFFF & RTDEXPCIEDRIVER_FIRST) +1)
//???  #error *** check # of errors allocated for PCIe kernel driver in 'adp_rtdex_types.h' & 'rtdexpcie_drviface.h' ***
#endif


typedef struct {
   // control & data:
   int fd;
} RtdexInfo;

#define MAX_NB_REQ_WORDS 5
#define MAX_NB_RESP_WORDS 10



static int DoIoControl( RtdexInfo * pInfo, unsigned char * pucSendData, unsigned uReqLen, 
                       unsigned char * pucRxData, unsigned uMaxRxSize, unsigned * puRxLen)
{
   int len;
   unsigned uMaxSize;
#if MAX_NB_RESP_WORDS > MAX_NB_REQ_WORDS
   unsigned auData[ MAX_NB_RESP_WORDS];
#else
   unsigned auData[ MAX_NB_REQ_WORDS];
#endif

   uMaxSize = sizeof auData;
   if( uReqLen > uMaxSize) {
      PDEBUG( "DoIoControl(): send too big, max is %u\n", uMaxSize);
      return RTDEXPCIEINTERR;
   }
   memset( (void*)auData, 0, sizeof auData);
   memcpy( (void*)auData, pucSendData, uReqLen);

   len = ioctl( pInfo->fd, DEFAULT_CTLIO_NO + (uReqLen/sizeof(unsigned) << 24) + (uMaxSize/sizeof(unsigned) << 16), auData);
   if( len < 0) {
//      PERROR( "len= %d, errno= %d\n", len, errno);
      if( errno == EINVAL) {
//         PERROR( "RTDEx Kernel Driver Error#= %u\n", auData[ 0]);
         return RTDEXPCIEDRIVER_FIRST + auData[ 0];
      }
      else
         PERROR( "ioctl()");
      return RTDEXPCIEIOCTL;
   }

   if( uMaxRxSize < len) {
      PDEBUG( "DoIoControl(): rcv buffer too small, need %u\n", len);
      return RTDEXPCIEINTERR;
   }
   memcpy( pucRxData, (void*)auData, len);
   *puRxLen = len;
   return 0; 
}



// =================================================================================

// =================================================================================


int rtdexpcie_OpenCtrl( rtdexpcie_Handle * pHandle, unsigned uRtdexNum)
{
   RtdexInfo * pInfo;
   char cName[ 128];

   pInfo = (RtdexInfo*)malloc( sizeof * pInfo);
   if( !pInfo) {
      return RTDEXMEMALLOC;
   }
   memset( pInfo, 0, sizeof * pInfo);

   sprintf( cName, "/dev/" GLOBAL_NAME_STR, uRtdexNum);
   pInfo->fd = open( cName, O_RDWR);
   if( pInfo->fd < 0) {
      PERROR( "open()");
      return RTDEXPCIEOPEN;
   }
   *pHandle = (rtdexpcie_Handle*)pInfo;
   return 0;
}



int rtdexpcie_iGetDriverVers( rtdexpcie_Handle handle, unsigned * puMaj, unsigned * puMin)
{
   // (cmd) --> (maj, min)
   uint32_t u32Cmd[ 1];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_DRIVER_VERSION;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *puMaj = u32Rx[ 0];
   *puMin = u32Rx[ 1];
   return 0;
}


// open a FIFO on a RTDEX device.
int rtdexpcie_OpenData( rtdexpcie_Handle * pHandle, unsigned uRtdexNum, unsigned uSelectRxFifo, unsigned uFifoNo)
{
   RtdexInfo * pInfo;
   char cName[ 128];

   pInfo = (RtdexInfo*)malloc( sizeof * pInfo);
   if( !pInfo) {
      return RTDEXMEMALLOC;
   }
   memset( pInfo, 0, sizeof * pInfo);

   if( uSelectRxFifo == rtdexpcie_SELECT_RX_FIFO)
      sprintf( cName, "/dev/" RXFIFO_NAME_STR, uRtdexNum, uFifoNo);
   else
      sprintf( cName, "/dev/" TXFIFO_NAME_STR, uRtdexNum, uFifoNo);
   PDEBUG( "opening '%s'\n", cName);
   pInfo->fd = open( cName, O_RDWR);
   if( pInfo->fd < 0) {
      PERROR( "open()");
      return RTDEXPCIEOPEN;
   }

   *pHandle = (rtdexpcie_Handle*)pInfo;
   return 0;
}



int rtdexpcie_Close( rtdexpcie_Handle handle)
{
   RtdexInfo * pInfo = (RtdexInfo*) handle;

   close( pInfo->fd);
   free( pInfo);
   return 0;
}


#if 1
// Use select() to implement timeouts

// Write all the data to device.
// Return 0 if ok, else RTDEX error code.
int rtdexpcie_Send( rtdexpcie_Handle handle, unsigned char * pucData, unsigned uDataLen, unsigned uTimeoutMs)
{
   RtdexInfo * pInfo = (RtdexInfo*) handle;
   ssize_t len;
   unsigned uOffset;
   fd_set set;
   struct timeval timeout;
   int rv;

   uOffset = 0;
   timeout.tv_sec = 0;
   timeout.tv_usec = uTimeoutMs * 1000;
   while( !uTimeoutMs || timeout.tv_usec > 0 || timeout.tv_sec > 0) {
      if( uTimeoutMs) {
         FD_ZERO( &set); // clear the set
         FD_SET( pInfo->fd, &set); // add our file descriptor to the set
//         PDEBUG( "timeout.tv_usec= %u\n", timeout.tv_usec);
         rv = select( pInfo->fd + 1, NULL, &set, NULL, &timeout);
         if( rv == -1) {
            PERROR( "send select()"); // an error accured
            return RTDEXPCIEWRITE;
         }
         else if( rv == 0) {
            //PERROR( "send timeout"); // a timeout occured
            return uOffset;
         }
         else // there is space for write
            ;
      }
      len = write( pInfo->fd, pucData + uOffset, uDataLen - uOffset);
      if( len < 0) {
         PERROR( "write()");
         return RTDEXPCIEWRITE;
      }
      uOffset += len;
      if( uOffset >= uDataLen)
         return uOffset;
   }
   return uOffset;
}

// Read as much data as possible, until timeout
// uTimeoutMs == 0 ==> no timeout
// *puRxLen will contains the exact data size read.
// Return 0 if OK, else RTDEX error code.
int rtdexpcie_Recv( rtdexpcie_Handle handle, unsigned char * pucBuff, unsigned uBuffSize, unsigned uTimeoutMs, unsigned * puRxLen)
{
   RtdexInfo * pInfo = (RtdexInfo*) handle;
   ssize_t len;
   unsigned uOffset;
   fd_set set;
   struct timeval timeout;
   int rv;

   uOffset = 0;
   timeout.tv_sec = 0;
   timeout.tv_usec = uTimeoutMs * 1000;
   while( !uTimeoutMs || timeout.tv_usec > 0 || timeout.tv_sec > 0) {

      FD_ZERO( &set); // clear the set
      FD_SET( pInfo->fd, &set); // add our file descriptor to the set
      
      rv = select( pInfo->fd + 1, &set, NULL, NULL, &timeout);
      if( rv == -1) {
         PERROR( "recv select()"); // an error occured
         return RTDEXPCIEREAD;
      }
      else if( rv == 0) {
         PERROR( "recv timeout"); // a timeout occured
         *puRxLen = uOffset;
         return 0;
      }
      else // there is data to read
         ;

      len = read( pInfo->fd, pucBuff + uOffset, uBuffSize - uOffset);
      if( len < 0) {
         PERROR( "read()");
         return RTDEXPCIEREAD;
      }
      if( len == 0)
         break; // EOF
      uOffset += len;
      if( uOffset >= uBuffSize)
         break;
      }
   if( uOffset) {
      *puRxLen = uOffset;
      return 0;
   }
   return RTDEXPCIEEOF;
}

#else

// Write all the data to device.
// Return 0 if ok, else RTDEX error code.
int rtdexpcie_Send( rtdexpcie_Handle handle, unsigned char * pucData, unsigned uDataLen, unsigned uTimeoutMs)
{
   RtdexInfo * pInfo = (RtdexInfo*) handle;
   ssize_t len;
   unsigned uOffset;
   unsigned uTo;

   uOffset = 0;
   uTo = uTimeoutMs;
   while( !uTimeoutMs || uTo > 0) {
      len = write( pInfo->fd, pucData + uOffset, uDataLen - uOffset);
      if( len < 0) {
         PERROR( "write()");
         return RTDEXPCIEWRITE;
      }
      uOffset += len;
      if( uOffset >= uDataLen)
         return 0;
      uTo--;
   }
   if( uOffset)
      return 0;
   return RTDEXPCIETIMEOUT;
}


// Read as much data as possible, until timout
// *puRxLen will contains the exact data size read.
// Return 0 if OK, else RTDEX error code.
int rtdexpcie_Recv( rtdexpcie_Handle handle, unsigned char * pucBuff, unsigned uBuffSize, unsigned uTimeoutMs, unsigned * puRxLen)
{
   RtdexInfo * pInfo = (RtdexInfo*) handle;
   ssize_t len;
   unsigned uOffset;
   unsigned uTo;

   uOffset = 0;
   uTo = uTimeoutMs;
   while( !uTimeoutMs || uTo > 0) {
      len = read( pInfo->fd, pucBuff + uOffset, uBuffSize - uOffset);
      if( len < 0) {
         PERROR( "read()");
         return RTDEXPCIEREAD;
      }
      if( len == 0)
         break; // EOF
      uOffset += len;
      if( uOffset >= uBuffSize)
         break;
      uTo--;
      }
   if( uOffset) {
      *puRxLen = uOffset;
      return 0;
   }
   if( !uTo)
      return RTDEXPCIETIMEOUT;
   return RTDEXPCIEEOF;
}
#endif


int rtdexpcie_iResetCore( rtdexpcie_Handle handle)
{
   // (IOCTL_RESET_CORE)
   uint32_t u32Cmd[ 1];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_RESET_CORE;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_iGetCoreIdVers( rtdexpcie_Handle handle, unsigned * puId, unsigned * puVer)
{
   // (cmd) --> (id, ver)
   uint32_t u32Cmd[ 1];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_CORE_IDVERS;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *puId = u32Rx[ 0];
   *puVer = u32Rx[ 1];
   return 0;
}


int rtdexpcie_ConnectDevToHost( unsigned * puChnlNo, rtdexpcie_Handle handle, unsigned uFifoNo)
{
   // (cmd, uFifoNo) ==> (chnlNo)
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_CONNECT_DEV2HOST;
   u32Cmd[ 1] = uFifoNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *puChnlNo = u32Rx[ 0];
   return 0;
}


int rtdexpcie_ConnectHostToDev( unsigned * puChnlNo, rtdexpcie_Handle handle, unsigned uFifoNo)
{
   // (cmd, uFifoNo) ==> (chnlNo, xx, xx, xx)
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_CONNECT_HOST2DEV;
   u32Cmd[ 1] = uFifoNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *puChnlNo = u32Rx[ 0];
   return 0;
}

int rtdexpcie_ConnectDevToDev( unsigned * puChnlNo, rtdexpcie_Handle handle, unsigned uSrcFifoNo, unsigned uDstFifoNo, unsigned uDstDevNo)
{
   // (cmd, srcFifoNo, dstFifoNo, dstDevNo)  ==> (chnlNo)
   uint32_t u32Cmd[ 4];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_CONNECT_DEV2DEV;
   u32Cmd[ 1] = uSrcFifoNo;
   u32Cmd[ 2] = uDstFifoNo;
   u32Cmd[ 3] = uDstDevNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *puChnlNo = u32Rx[ 0];
   return 0;
}


int rtdexpcie_Unconnect( rtdexpcie_Handle handle, unsigned uChnlNo)
{
   // (cmd, chnlNo)
   uint32_t u32Cmd[ MAX_NB_REQ_WORDS];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_UNCONNECT;
   u32Cmd[ 1] = uChnlNo;
   u32Cmd[ 2] = 0;
   u32Cmd[ 3] = 0;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_Start( rtdexpcie_Handle handle, unsigned uChnlNo, unsigned uFrameSize, size_t transferSize)
{
   // (cmd, chnlNo, frameSize, transferSize) // 32-bit driver
   // (cmd, chnlNo, frameSize, transferSizeH, transferSizeL) // 64-bit driver
   uint32_t u32Cmd[ 5];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_START;
   u32Cmd[ 1] = uChnlNo;
   u32Cmd[ 2] = uFrameSize;
   u32Cmd[ 3] = (unsigned)(transferSize & 0xffffffff);
   if( sizeof( transferSize) > 4)
      u32Cmd[ 4] = (unsigned)(transferSize >> 32);
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_Stop( rtdexpcie_Handle handle, unsigned uChnlNo)
{
   // (cmd, chnlNo)
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_STOP;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}



int rtdexpcie_iEnableRxFifo( rtdexpcie_Handle handle, unsigned uChnlMsk, unsigned uEnableWr, unsigned uEnableRd)
{
   // (IOCTL_ENABLE_RX_FIFO, chnlMsk, wr, rd)
   uint32_t u32Cmd[ 4];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_ENABLE_RX_FIFO;
   u32Cmd[ 1] = uChnlMsk;
   u32Cmd[ 2] = uEnableWr;
   u32Cmd[ 3] = uEnableRd;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_iEnableTxFifo( rtdexpcie_Handle handle, unsigned uChnlMsk, unsigned uEnableWr, unsigned uEnableRd)
{
   // (IOCTL_ENABLE_TX_FIFO, chnlMsk, wr, rd)
   uint32_t u32Cmd[ 4];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_ENABLE_TX_FIFO;
   u32Cmd[ 1] = uChnlMsk;
   u32Cmd[ 2] = uEnableWr;
   u32Cmd[ 3] = uEnableRd;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_iGetRxSize( rtdexpcie_Handle handle, unsigned uChnlNo, size_t * pRxSize)
{
   // (IOCTL_GET_RX_TFR_SIZE) --> (size) // 32-bit driver
   // (IOCTL_GET_RX_TFR_SIZE) --> (sizeL, sizeH) // 64-bit driver
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_RX_TFR_SIZE;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   if( sizeof( *pRxSize) > 4)
      *pRxSize = u32Rx[ 0] + ((size_t)u32Rx[ 1] << 32);
   else
      *pRxSize = u32Rx[ 0];
   return 0;
}


int rtdexpcie_iGetTxSize( rtdexpcie_Handle handle, unsigned uChnlNo, size_t * pTxSize)
{
   // (IOCTL_GET_TX_TFR_SIZE) --> (count) // 32-bit
   // (IOCTL_GET_TX_TFR_SIZE) --> (countL, countH) // 64-bit
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   size_t val;
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_TX_TFR_SIZE;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   val = u32Rx[ 0];
   if( sizeof( val) > 4)
      val += (size_t)u32Rx[ 1] << 32;
   *pTxSize = val;
   return 0;
}


int rtdexpcie_iGetHostXferedSize( rtdexpcie_Handle handle, unsigned uChnlNo, size_t * pXferedSize)
{
   // (IOCTL_GET_HOST_TFR_SIZE) --> (size) // 32-bit 
   // (IOCTL_GET_HOST_TFR_SIZE) --> (sizeL, sizeH) // 64-bit
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   size_t val;
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_HOST_TFR_SIZE;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   val = u32Rx[ 0];
   if( sizeof( val) > 4)
      val += (size_t)u32Rx[ 1] << 32;
   *pXferedSize = val;
   return 0;
}


int rtdexpcie_iResetRxFifo( rtdexpcie_Handle handle, unsigned uChnlNo)
{
   // (IOCTL_RESET_RX_FIFO)
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_RESET_RX_FIFO;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_iResetTxFifo( rtdexpcie_Handle handle, unsigned uChnlNo)
{
   // (IOCTL_RESET_TX_FIFO)
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_RESET_TX_FIFO;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_iGetChannelStatus( rtdexpcie_Handle handle, unsigned uChnlNo, unsigned * pbEnded, unsigned * pbSucces)
{
    // (cmd, chnlNo) ==> (bEnded, bPass)
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_STATUS;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *pbEnded = u32Rx[ 0];
   *pbSucces = u32Rx[ 1];
   return 0;
}


int rtdexpcie_iGetFifoConfig( rtdexpcie_Handle ctrlHandle, unsigned * puNbRxFifo, unsigned * puRxFifoSize, unsigned * puNbTxFifo, unsigned * puTxFifoSize)
{
   // (cmd) ==> (nbRx, nbTx, rxSize, txSize)
   uint32_t u32Cmd[ 1];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_FIFO_CFG;
   res = DoIoControl( ctrlHandle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *puNbRxFifo = u32Rx[ 0];
   *puNbTxFifo = u32Rx[ 1];
   *puRxFifoSize = u32Rx[ 2];
   *puTxFifoSize = u32Rx[ 3];
   return 0;
}



int rtdexpcie_iGetTxFifoStatus( rtdexpcie_Handle handle, unsigned uChnlNo, unsigned * pbOverflow, unsigned * pbUnderflow)
{
   // (cmd, chnlNo) ==> (overflow, underflow)
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_TX_FIFO_STATUS;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *pbOverflow = u32Rx[ 0];
   *pbUnderflow = u32Rx[ 1];
   return 0;
}



int rtdexpcie_iGetRxFifoStatus( rtdexpcie_Handle handle, unsigned uChnlNo, unsigned * pbOverflow, unsigned * pbUnderflow)
{
   // (cmd, chnlNo) ==> (overflow, underflow)
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_GET_RX_FIFO_STATUS;
   u32Cmd[ 1] = uChnlNo;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *pbOverflow = u32Rx[ 0];
   *pbUnderflow = u32Rx[ 1];
   return 0;
}




// ===============================================================================================
// DEBUG only
// ===============================================================================================

int rtdexpcie_IoControl( rtdexpcie_Handle handle, unsigned char * pucSendData, unsigned uReqLen,
                      unsigned char * pucRxData, unsigned uMaxRxSize, unsigned * puRxLen)
{
   return DoIoControl( handle, pucSendData, uReqLen, pucRxData, uMaxRxSize, puRxLen);
}



int rtdexpcie_WriteReg( rtdexpcie_Handle handle, unsigned uAddr, unsigned uVal)
{
   uint32_t u32Cmd[ 3];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_WRITE_REG;
   u32Cmd[ 1] = uAddr;
   u32Cmd[ 2] = uVal;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_ReadModifyWriteReg( rtdexpcie_Handle handle, unsigned uAddr, unsigned uAnd, unsigned uOr)
{
   uint32_t u32Cmd[ 4];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_RMW_REG;
   u32Cmd[ 1] = uAddr;
   u32Cmd[ 2] = uAnd;
   u32Cmd[ 3] = uOr;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   return res;
}


int rtdexpcie_ReadReg( rtdexpcie_Handle handle, unsigned uAddr, unsigned * puVal)
{
   uint32_t u32Cmd[ 2];
   uint32_t u32Rx[ MAX_NB_RESP_WORDS];
   unsigned uRxLen;
   int res;

   u32Cmd[ 0] = IOCTL_READ_REG;
   u32Cmd[ 1] = uAddr;
   res = DoIoControl( handle, (uint8_t*)u32Cmd, sizeof u32Cmd, (uint8_t*)u32Rx, sizeof u32Rx, &uRxLen);
   if( res)
      return res;
   *puVal = u32Rx[ 0];
   return 0;
}


#endif // __linux__
