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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       FMCEEprom_ifru.c
 * @brief      fmceeprom library common routines.
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCEEPROMLIB_EEPROM
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
/*
 * ifru  (was fruconfig.c)
 *
 * This tool reads the FRU configuration, and optionally sets the asset
 * tag field in the FRU data.  See IPMI v1.5 spec section 28.
 * It can use either the Intel /dev/imb driver or VALinux /dev/ipmikcs.
 *
 * Author:  Andy Cress  arcress at users.sourceforge.net
 *
 * Copyright (c) 2009 Kontron America, Inc.
 *
 * 10/28/02 Andy Cress - created
 * 12/11/02 Andy Cress v0.9  - disable write until checksum fixed.
 * 12/13/02 Andy Cress v0.91 - don't abort if cc=c9 in load_fru loop.
 * 01/27/03 Andy Cress v0.92 - more debug, do checksums,
 * 01/28/03 Andy Cress v1.0  do writes in small chunks, tested w SCB2 & STL2
 * 02/19/03 Andy Cress v1.1  also get System GUID
 * 03/10/03 Andy Cress v1.2  do better bounds checking on FRU size
 * 04/30/03 Andy Cress v1.3  Board Part# & Serial# reversed
 * 05/13/03 Andy Cress v1.4  Added Chassis fields
 * 06/19/03 Andy Cress v1.5  added errno.h (email from Travers Carter)
 * 05/03/04 Andy Cress v1.6  BladeCenter has no product area, only board area
 * 05/05/04 Andy Cress v1.7  call ipmi_close before exit,
 *                           added WIN32 compile options.
 * 11/01/04 Andy Cress v1.8  add -N / -R for remote nodes   
 * 12/10/04 Andy Cress v1.9  add gnu freeipmi interface
 * 01/13/05 Andy Cress v1.10 add logic to scan SDRs for all FRU devices,
 *                           and interpret them
 * 01/17/05 Andy Cress v1.11 decode SPD Manufacturer
 * 01/21/05 Andy Cress v1.12 format SystemGUID display
 * 02/03/05 Andy Cress v1.13 fixed fwords bit mask in load_fru,
 *                           decode DIMM size from SPD also.
 * 02/04/05 Andy Cress v1.14 decode FRU Board Mfg DateTime
 * 03/16/05 Andy Cress v1.15 show Asset Tag Length earlier
 * 05/24/05 Andy Cress v1.16 only do write_asset if successful show_fru
 * 06/20/05 Andy Cress v1.17 handle Device SDRs also for ATCA
 * 08/22/05 Andy Cress v1.18 allow setting Product Serial Number also (-s),
 *                           also add -b option to show only baseboard data.
 * 10/31/06 Andy Cress v1.25 handle 1-char asset/serial strings (avoid c1)
 */
/*M*
Copyright (c) 2009 Kontron America, Inc.
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

  a.. Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer. 
  b.. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation 
      and/or other materials provided with the distribution. 
  c.. Neither the name of Kontron nor the names of its contributors 
      may be used to endorse or promote products derived from this software 
      without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *M*/

#ifdef WIN32

// Uncomment this line to add some console output strings (mandatory to display decoded info on the console)
#define FMCEEPROM_PRINT 1

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>

#elif defined(DOS)

// Uncomment this line to add some console output strings (mandatory to display decoded info on the console)
#define FMCEEPROM_PRINT 1

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "getopt.h"
#else

#ifdef DEBUG
// Uncomment this line to add some console output strings (mandatory to display decoded info on the console)
#define FMCEEPROM_PRINT 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#ifndef HPUX
#include <errno.h>
#endif
#endif

// Console related definitions
#ifdef FMCEEPROM_PRINT
    // These macros are used to get printout info
    #define FUNC_ENTER()      if (_allows_printout) printf("FMCEEPROM: enter %s\n", __FUNCTION__)
    #define FUNC_EXIT()       if (_allows_printout) printf("FMCEEPROM: exit  %s\n", __FUNCTION__)
    #define PDEBUG(f, ...)    if (_allows_printout) printf(f, __VA_ARGS__)

#else

    #define FUNC_ENTER()
    #define FUNC_EXIT()
    #define PDEBUG(f, ...)

#endif

// Update an old error to new error value but does not override the old error condition if already one 
#define UPDATE_ERROR(olderr, newerr) SUCCESS(olderr) ? (newerr) : (olderr)

#include <time.h>

#include "FMCEEprom_ipmicmd.h" 
#include "FMCEEprom_ipicmg.h" 
#include "FMCEEprom_ifru.h" 
#include "FMCEEprom_ifru_vita.h" 
#include "FMCEEprom_def.h" 

#define PICMG_CHILD   1
#define MIN_SDR_SZ  8
#ifndef URNLOOPS
#define URNLOOPS 1000
#endif
extern int get_BiosVersion(char *str);
extern int get_SystemGuid(uchar *guid);
extern void fmt_time(time_t etime, char *buf, int bufsz); /*see ievents.c*/
extern int get_LastError( void );  /* ipmilan.c */
extern int GetSDRRepositoryInfo(int *nret, int *fdev);  /*isensor.h*/
#ifdef METACOMMAND
extern int ipmi_kontronoem_main(void * intf, int  argc, char ** argv);
#endif

static int show_fru_multi(char *tag, int midx, uchar mtype, uchar *pdata, int dlen,FMCEEPROM_content *fmceeprom, int chkerr);

static int  vend_id = 0; 
//static int  prod_id = 0;
static char fdebug = 0;
static char fgetintchk = 0;
static char fpicmg = 0;
static char force_cont = 0;
//static char fonlybase = 0;
//static char fonlyhsc = 0;
//static int  fwritefru = 0;
//static int  fdevsdrs = 0;
static char fshowlen = 0;
//static char ftestshow = 0;
// static char fgetfru  = 0;
static char fdoanyway = 0;
//static char fprivset = 0;
//static char fset_mc = 0;
static char fcanonical = 0;
//static char foemkontron = 0;
int _allows_printout = 0;
//static char fbasefru = 1;
//static char fdump = 0;
//static char frestore = 0;
//static char fchild = 0;        /* =1 follow child MCs if picmg bladed*/
//static char do_systeminfo = 1;
static char do_guid = 0;
static char bdelim = ':';
static uchar bmc_sa = BMC_SA;  /*defaults to 0x20*/
static uchar guid[17] = "";
//static uchar *frubuf = NULL;
#define FIELD_LEN   20
#define SZ_PRODAREA   520 /* product area max is 8*32 + 3 = 259 mod 8 = 264 */
static int    sfru = 0;
static int    asset_offset = -1;
static int    asset_len = 0;
static int    sernum_offset = -1;
static int    sernum_len = 0;
static int    prodver_offset = -1;
static int    prodver_len = 0;
static int    chassis_offset = -1;
static int    chassis_len = 0;
//static char   asset_tag[FIELD_LEN]  = {0};
//static char   serial_num[FIELD_LEN] = {0};
//static char   prod_ver[FIELD_LEN] = {0};
//static char   chassis_name[FIELD_LEN] = {0};
//static char   ps_prod[FIELD_LEN] = {0};
static int    maxprod = 0;
static int ctype;
//static uchar g_bus = PUBLIC_BUS;
//static uchar g_sa  = 0;
//static uchar g_lun = BMC_LUN;
//static uchar g_addrtype = ADDR_SMI;
//static uchar g_fruid = 0;  /* default to fruid 0 */
//static uchar g_frutype = 0;
//static uchar lastfru[3] = {0,0,0};
/* g_frutype values (detected), see also FruTypeString
 * --TAG----   FRU   IPMB
 * Baseboard = 0x0c  0x07 
 * PowerSply = 0x0a
 * PowerCage = 0x15
 * DIMM      = 0x20
 * HotSwapCt =       0x0f
 * ME        =       0x2e
 * SysInfo   =    0x40
 * Component =  *     *  (all others)
 */
#ifdef METACOMMAND
extern int verbose;  
extern int sdr_get_reservation(uchar *res_id, int fdev);
#else
//static int verbose;
//static int sdr_get_reservation(uchar *res_id, int fdev) { return(-1); }
#endif

#define STRING_DATA_TYPE_BINARY         0x00
#define STRING_DATA_TYPE_BCD_PLUS       0x01
#define STRING_DATA_TYPE_SIX_BIT_ASCII  0x02
#define STRING_DATA_TYPE_LANG_DEPENDENT 0x03

#define FRUCHUNK_SZ   16
#define FRU_END         0xC1
#define FRU_EMPTY_FIELD 0xC0
#define FRU_TYPE_MASK   0xC0
#define FRU_LEN_MASK    0x3F

// Do not change these values without changing the associated strings
#define OEM_EXTRA_FILEDS    9
#define NUM_BOARD_FIELDS    (6 + OEM_EXTRA_FILEDS)
#define NUM_PRODUCT_FIELDS  (8 + OEM_EXTRA_FILEDS)
#define NUM_CHASSIS_FIELDS  (3 + OEM_EXTRA_FILEDS)
#define MAX_CTYPE   26
char *ctypes[MAX_CTYPE] = { "", "Other", "Unknown", "Desktop", 
	"Low Profile Desktop", "Pizza Box", "Mini-Tower", "Tower", 
	"Portable", "Laptop", "Notebook", "Handheld", "Docking Station", 
	"All-in-One", "Sub-Notebook", "Space-saving", "Lunch Box", 
	"Main Server Chassis", "Expansion Chassis", "SubChassis", 
	"Bus Expansion Chassis", "Peripheral Chassis", 
	"RAID Chassis", /*0x17=23.*/ "Rack-Mount Chassis", 
	"New24" , "New25"};
	/* what about bladed chassies? */
char *ctype_hdr = 
"Chassis Type        "; 
char *chassis[NUM_CHASSIS_FIELDS] = {
"Chassis Part Number ", 
"Chassis Serial Num  ",
"Chassis OEM Field 0 ",
"Chassis OEM Field 1 ",
"Chassis OEM Field 2 ",
"Chassis OEM Field 3 ",
"Chassis OEM Field 4 ",
"Chassis OEM Field 5 ",
"Chassis OEM Field 6 ",
"Chassis OEM Field 7 ",
"Chassis OEM Field 8 ",
"Chassis OEM Field 9 " };
char *board[NUM_BOARD_FIELDS] = {
"Board Manufacturer  ",
"Board Product Name  ",
"Board Serial Number ",
"Board Part Number   ",
"Board FRU File ID   ",
"Board OEM Field 0   ",
"Board OEM Field 1   ",
"Board OEM Field 2   ",
"Board OEM Field 3   ",
"Board OEM Field 4   ",
"Board OEM Field 5   ",
"Board OEM Field 6   ",
"Board OEM Field 7   ",
"Board OEM Field 8   ",
"Board OEM Field 9   " };
char *product[NUM_PRODUCT_FIELDS] = {
"Product Manufacturer",
"Product Name        ",
"Product Part Number ",
"Product Version     ",
"Product Serial Num  ",  
"Product Asset Tag   ",
"Product FRU File ID ",
"Product OEM Field 0 ",
"Product OEM Field 1 ",
"Product OEM Field 2 ",
"Product OEM Field 3 ",
"Product OEM Field 4 ",
"Product OEM Field 5 ",
"Product OEM Field 6 ",
"Product OEM Field 7 ",
"Product OEM Field 8 ",
"Product OEM Field 9 " };
char *asset_hdr = 
"   Asset Tag Length ";

#if 0
typedef struct {
	uchar len :6;
	uchar type:2;
	} TYPE_LEN;  /*old, assumes lo-hi byte order*/
#else
typedef struct {
	uchar len;
	uchar type;
	} TYPE_LEN;
#endif

int decode_string(unsigned char type,
                   unsigned char language_code,
                   unsigned char *source,
                   int           slen,
                   char          *target,
                   int           tsize)
{
  static const char bcd_plus[] = "0123456789 -.:,_";
  unsigned char *s = &source[0];
  //unsigned char *d = &target[0];
  int len, i, j, k;
  union { uint32_t bits; char chars[4]; } u;
  int ret=FMCEEPROM_OK;
 
  //if (slen == 0 || slen == 1) return;
  if (slen == 0) return(ret);
  if (slen == 1 && type==STRING_DATA_TYPE_LANG_DEPENDENT)
  {
	  PDEBUG("ERROR - Invalid single character ascii string!%c\n",32);
	  return(FMCEEPROM_ERR_TLEN_ASC);
  }

  switch(type) {
     case STRING_DATA_TYPE_BINARY: /* 00: binary/unspecified */
        len = (slen*2); break; /* hex dump -> 2x length */
     case STRING_DATA_TYPE_SIX_BIT_ASCII:  /*type 2 6-bit ASCII*/
        /* 4 chars per group of 1-3 bytes */
        len = ((((slen+2)*4)/3) & ~3); break;
     case STRING_DATA_TYPE_LANG_DEPENDENT:  /* 03 language dependent */
     case STRING_DATA_TYPE_BCD_PLUS: /* 01b: BCD plus */
     default:  
	len = slen; break;
  }
  if (len >= tsize) len = tsize - 1;
  memset(target, 0, len);
  if (type == STRING_DATA_TYPE_BCD_PLUS) { /*type 1: BCD plus*/
     for (k=0; k<len; k++)
         target[k] = bcd_plus[(s[k] & 0x0f)];
     target[k] = '\0';
  } else if (type == STRING_DATA_TYPE_SIX_BIT_ASCII) { /*type 2: 6-bit ASCII*/
     for (i=j=0; i<slen; i+=3) {
            u.bits = 0;
            k = ((slen-i) < 3 ? (slen-i) : 3);
#if WORDS_BIGENDIAN
            u.chars[3] = s[i];
            u.chars[2] = (k > 1 ? s[i+1] : 0);
            u.chars[1] = (k > 2 ? s[i+2] : 0);
#define CHAR_IDX 3
#else
            memcpy((void *)&u.bits, &s[i], k);
#define CHAR_IDX 0
#endif
            for (k=0; k<4; k++) {
                    target[j++] = ((u.chars[CHAR_IDX] & 0x3f) + 0x20);
                    u.bits >>= 6;
            }
     }
     target[j] = '\0';
  } else if (type == STRING_DATA_TYPE_LANG_DEPENDENT) {  /*type 3*/
     if ((language_code == 0x00) || (language_code == 0x25) || 
	(language_code == 0x19)) {
       memcpy(target, source, len);
       target[len] = 0;
     } else {
       PDEBUG("Language 0x%x dependent decode not supported\n",language_code);
     }
  } else if (type == STRING_DATA_TYPE_BINARY) { /* 00: binary/unspecified */
     strncpy(target, buf2str(s, slen), len);
  } else {   /* other */
     PDEBUG("ERROR -  Unable to decode type 0x%.2x\n",type);
	 ret = FMCEEPROM_ERR_DEC_TYPE;
  }
  return(ret);
}

static uchar calc_cksum(uchar *pbuf,int len)
{
   int i; 
   uchar cksum;
   uchar sum = 0;
  
   for (i = 0; i < len; i++) sum += pbuf[i];
   cksum = 0 - sum; 
   return(cksum);
}

static void dumpbuf(uchar *pbuf,int sz)
{
   uchar line[17];
   uchar a;
   int i, j;

   line[0] = 0; line[16] = 0;
   j = 0;
   for (i = 0; i < sz; i++) {
	   if (i % 16 == 0) { j = 0; PDEBUG("%s\n  %04d: ",line,i); }
	   a = pbuf[i];
	   if (a < 0x20 || a > 0x7f) a = '.';
	   line[j++] = a;
	   PDEBUG("%02x ",pbuf[i]);
   }
   line[j] = 0;
   PDEBUG("%s\n",line);
   return;
} 

#define NSPDMFG 7
static struct { 
   uchar id;  char *str;
} spd_mfg[NSPDMFG] = {  /* see JEDEC JEP106 doc */
{ 0x2c, "Micron" },
{ 0x15, "Philips Semi" },
{ 0x1c, "Mitsubishi" },
{ 0xce, "Samsung" },
{ 0xc1, "Infineon" },
{ 0x98, "Kingston" },
{ 0x89, "Intel" }
};

static int ValidTL(uchar typelen)
{
   if (vend_id != VENDOR_INTEL) return 1;
   if ((typelen & 0xC0) == 0xC0) return 1;
   else return 0;
}

#define SYS_FRUTYPE 0x40
static char * FruTypeString(uchar frutype, uchar dev)
{
   char *pstr;
   switch (frutype) { 
            case 0x07:        /*IPMB*/
            case 0x0c: 	      /*FRU*/
			if (dev == 0) pstr = "Baseboard"; 
			else          pstr = "Board    "; 
			break; 
            case 0x0a:  pstr = "PowerSply"; break;  /*FRU*/
            case 0x15:  pstr = "PowerCage"; break;  /*FRU*/
            case 0x20:  pstr = "DIMM     "; break;  /*FRU*/
            case 0x0f:  pstr = "HotSwapCt"; break;  /*IPMB*/
            case 0x2e:  pstr = "ME       "; break;  /*IPMB*/
            case SYS_FRUTYPE:  pstr = "SysInfo  "; break;  /*SysInfo*/
            default:    pstr = "Component"; break; 
	}
   return(pstr);
}

static int ChkOverflow(int idx, int sz, uchar len) 
{
   if (idx >= sz) {
        PDEBUG("\nERROR - FRU Buffer Overflow (%d >= %d), last len=%d\n",
		idx,sz,len);
	return 1;   /*overflow error*/
   }
   return 0;
}

static void show_spd(uchar *spd, int lenspd, uchar frudev, uchar frutype)
{
   int sz;
   char *pstr;
   uchar mrev;
   int i;
   char devstr[24];

   sz = spd[0];  /* sz should == lenspd */
   if (fcanonical) devstr[0] = 0;  /*default is empty string*/
   else sprintf(devstr,"[%s,   %02x] ",FruTypeString(frutype,frudev),frudev);
   PDEBUG("%sMemory SPD Size     %c %d\n",
		devstr,bdelim,lenspd);
   if (spd[2] == 0x07) pstr = "DDR";
   else /* usu 0x04 */ pstr = "SDRAM";
   PDEBUG("%sMemory Type         %c %s\n",
		devstr,bdelim,pstr);
   PDEBUG("%sModule Density      %c %d MB per bank\n",
		devstr,bdelim, (spd[31] * 4));
   PDEBUG("%sModule Banks        %c %d banks\n", 
		devstr,bdelim,spd[5]);
   PDEBUG("%sModule Rows, Cols   %c %d rows, %d cols\n",
		devstr,bdelim, spd[3], spd[4]);
   if (spd[11] == 0x00) pstr = "Non-parity";
   else /* usu 0x02 */  pstr = "ECC";
   PDEBUG("%sDIMM Config Type    %c %s\n",devstr,bdelim,pstr);
   for (i = 0; i < NSPDMFG; i++) 
	if (spd_mfg[i].id == spd[64]) break;
   if (i == NSPDMFG) pstr = "";  /* not found, use null string */
   else pstr = spd_mfg[i].str;
   PDEBUG("%sManufacturer ID     %c %s (0x%02x)\n",
		devstr,bdelim, pstr, spd[64]);
   mrev = spd[91];  /* save this byte for later */
   spd[91] = 0;     /*stringify part number */
   PDEBUG("%sManufacturer Part#  %c %s\n",
		devstr,bdelim,&spd[73]);
   PDEBUG("%sManufacturer Rev    %c %02x %02x\n",
		devstr,bdelim,mrev,spd[92]);
   PDEBUG("%sManufacturer Date   %c year=%02d week=%02d\n",
		devstr,bdelim,spd[93],spd[94]);
   PDEBUG("%sAssembly Serial Num %c %02x%02x%02x%02x\n",
		devstr,bdelim,spd[95], spd[96], spd[97], spd[98]);
   spd[91] = mrev;  /* restore byte, so ok to repeat later */
   return;
}

static void show_guid(uchar *pguid)
{
   char *s;
   int i;
	for (i=0; i<16; i++) {
	   if ((i == 4) || (i == 6) || (i == 8) || (i == 10)) s = "-";
	   else s = "";
	   PDEBUG("%s%02x",s,pguid[i]);
	}
}

static char *volt_desc(uchar b)
{
   char *s;
   switch(b) {
   case 0x03: s = "5V"; break;
   case 0x02: s = "3.3V"; break;
   case 0x01: s = "-12V"; break;
   case 0x00: 
   default:   s = "12V"; break;
   }
   return(s);
}

static char *mgt_type(uchar b)
{
   char *s;
   switch(b) {
   case 0x01: s = "SysMgt_URL"; break;
   case 0x02: s = "SystemName"; break;
   case 0x03: s = "SysPingAddr"; break;
   case 0x04: s = "Compon_URL"; break;
   case 0x05: s = "ComponName"; break;
   case 0x06: s = "ComponPing"; break;
   case 0x07: 
   default:   s = "SysGUID"; break;
   }
   return(s);
}


static int show_fru_multi(char *tag, int midx, uchar mtype, uchar *pdata, int dlen,FMCEEPROM_content *fmceeprom, int chkerr)
{
   int ret=FMCEEPROM_OK,ret1;
   int vend, i;
   char mystr[256];
   char *s1; 
   int v1, v2, v3, v4, v5, v6, v7; 
   uchar b1, b2;

   if (fdebug) dumpbuf(pdata,dlen);  /*multi-record area dump*/
   sprintf(mystr,"%sMulti[%d] ",tag,midx);
   i = strlen(mystr);
   switch(mtype) {
	case 0x00:  /*Power Supply*/
		PDEBUG("%sPower Supply Record %c \n",mystr,bdelim);
		v1 = pdata[0] + ((pdata[1] & 0x0f) << 8);
		PDEBUG("\t Capacity  \t%c %d W\n",bdelim, v1);
		v2 = pdata[2] + (pdata[3] << 8);
		PDEBUG("\t Peak VA   \t%c %d VA\n",bdelim, v2);
		PDEBUG("\t Inrush Current\t%c %d A\n",bdelim, pdata[4]);
		PDEBUG("\t Inrush Interval\t%c %d ms\n",bdelim, pdata[5]);
		v3 = pdata[6] + (pdata[7] << 8);
		v4 = pdata[8] + (pdata[9] << 8);
		PDEBUG("\t Input Voltage Range1\t%c %d-%d V\n",
			bdelim,v3/100,v4/100);
		v3 = pdata[10] + (pdata[11] << 8);
		v4 = pdata[12] + (pdata[13] << 8);
		PDEBUG("\t Input Voltage Range2\t%c %d-%d V\n",
			bdelim,v3/100,v4/100);
		PDEBUG("\t Input Frequency Range\t%c %d-%d Hz\n",
			bdelim,pdata[14],pdata[15]);
		PDEBUG("\t AC Dropout Tolerance\t%c %d ms\n",bdelim, pdata[16]);
		b1 = pdata[17];
                b2 = (b1 & 0x01);
		if (b2) {  /*predictive fail*/
		    if ((b1 & 0x10) != 0) s1 = "DeassertFail ";
		    else s1 = "AssertFail ";
		} else {
		    if ((b1 & 0x10) != 0) s1 = "2pulses/rot ";
		    else s1 = "1pulse/rot ";
		}
		PDEBUG("\t Flags   \t%c %s%s%s%s%s\n",bdelim,
		  b2 ? "PredictFail " : "",
		  ((b1 & 0x02) == 0) ? "" : "PowerFactorCorrect ",
		  ((b1 & 0x04) == 0) ? "" : "AutoswitchVolt ",
		  ((b1 & 0x08) == 0) ? "" : "Hotswap ", s1);
		v5 = pdata[18] + ((pdata[19] & 0x0f) << 8);
		v6 = (pdata[19] & 0xf0) >> 4;
		PDEBUG("\t Peak Capacity \t%c %d W for %d s\n",bdelim, v5,v6);
		if (pdata[20] == 0) {
		   PDEBUG("\t Combined Capacity\t%c not specified\n",bdelim);
		} else {
		   b1 = pdata[20] & 0x0f;
		   b2 = (pdata[20] & 0xf0) >> 4;
		   v7 = pdata[21] + (pdata[22] << 8);
		   PDEBUG("\t Combined Capacity\t%c %d W (%s and %s)\n",
			bdelim, v7,volt_desc(b1),volt_desc(b2));
		}
		if (b2)   /*predictive fail*/
		   PDEBUG("\t Fan low threshold\t%c %d RPS\n",bdelim,pdata[23]);
		break;
	case 0x01:  /*DC Output*/
		b1 = pdata[0] & 0x0f;
		b2 = ((pdata[0] & 0x80) != 0);
		fmceeprom->power[b1].ptype = FMCEEPROM_POWER_DCOUTPUT;
        fmceeprom->power[b1].standby = b2;
        PDEBUG("%sDC Output       %c number %d\n",mystr,bdelim,b1);
		PDEBUG("\t Standby power \t%c %s\n", bdelim,
			(b2 ? "Yes" : "No"));
		v1 = pdata[1] + (pdata[2] << 8);
        fmceeprom->power[b1].nominal_volt = v1;
		PDEBUG("\t Nominal voltage \t%c %.2f V\n", bdelim, (float)v1 / 100);
		v2 = pdata[3] + (pdata[4] << 8);
        fmceeprom->power[b1].min_volt = v2;
		v3 = pdata[5] + (pdata[6] << 8);
        fmceeprom->power[b1].max_volt = v3;
		PDEBUG("\t Voltage deviation \t%c %.2f V / %.2f V\n", 
			bdelim, (float)v2/100, (float)v3/100);
		v4 = pdata[7] + (pdata[8] << 8);
        fmceeprom->power[b1].ripple_pp_volt = v4;
		PDEBUG("\t Ripple and noise pk-pk\t%c %d mV\n", bdelim, v4);
		v5 = pdata[9] + (pdata[10] << 8);
        fmceeprom->power[b1].min_current = v5;
		PDEBUG("\t Min current draw \t%c %.3f A\n", bdelim, (float)v5/1000);
		v6 = pdata[11] + (pdata[12] << 8);
        fmceeprom->power[b1].max_current = v6;
		PDEBUG("\t Max current draw \t%c %.3f A\n", bdelim, (float)v6/1000);
        if (FAILURE(chkerr))
            fmceeprom->power[b1].presence = FMCEEPROM_PRE_CHKERROR;
        else
            fmceeprom->power[b1].presence = FMCEEPROM_PRE_PRESENT;
		break;
	case 0x02:  /*DC Load*/
		b1 = pdata[0] & 0x0f;
		fmceeprom->power[b1].ptype = FMCEEPROM_POWER_DCLOAD;
		PDEBUG("%sDC Load         %c number %d\n",mystr,bdelim,b1);
		v1 = pdata[1] + (pdata[2] << 8);
        fmceeprom->power[b1].nominal_volt = v1;
		PDEBUG("\t Nominal voltage \t%c %.2f V\n", bdelim, (float)v1 / 100);
		v2 = pdata[3] + (pdata[4] << 8);
        fmceeprom->power[b1].min_volt = v2;
		v3 = pdata[5] + (pdata[6] << 8);
        fmceeprom->power[b1].max_volt = v3;
		PDEBUG("\t Min voltage allowed \t%c %.2f V\n", bdelim, (float)v2 / 100);
		PDEBUG("\t Max voltage allowed \t%c %.2f V\n", bdelim, (float)v3 / 100);
		v4 = pdata[7] + (pdata[8] << 8);
        fmceeprom->power[b1].ripple_pp_volt = v4;
		PDEBUG("\t Ripple and noise pk-pk\t%c %d mV\n", bdelim, v4);
		v5 = pdata[9] + (pdata[10] << 8);
        fmceeprom->power[b1].min_current = v5;
		PDEBUG("\t Min current load \t%c %.3f A\n", bdelim, (float)v5/1000);
		v6 = pdata[11] + (pdata[12] << 8);
        fmceeprom->power[b1].max_current = v6;
		PDEBUG("\t Max current load \t%c %.3f A\n", bdelim, (float)v6/1000);
        if (FAILURE(chkerr))
            fmceeprom->power[b1].presence = FMCEEPROM_PRE_CHKERROR;
        else
            fmceeprom->power[b1].presence = FMCEEPROM_PRE_PRESENT;
		break;
	case 0x03:  /*Management Access*/
		b1 = pdata[0];
		PDEBUG("%sManagemt Access %c %s ",mystr,bdelim,mgt_type(b1));
		memcpy(mystr,&pdata[1],dlen-1);
		mystr[dlen-1] = 0;
		PDEBUG("%s\n",mystr);
		break;
	case 0x04:  /*Base Compatibility*/
		vend = pdata[0] + (pdata[1] << 8) + (pdata[2] << 16);
		PDEBUG("%sBasic Compat    %c %06x\n",mystr,bdelim,vend);
		break;
	case 0x05:  /*Extended Compatibility*/
		vend = pdata[0] + (pdata[1] << 8) + (pdata[2] << 16);
		PDEBUG("%sExtended Compat %c %06x\n",mystr,bdelim,vend);
		break;
	default:
	    if  (mtype >= 0xC0)  /*OEM Extension*/
		{
			vend = pdata[0] + (pdata[1] << 8) + (pdata[2] << 16);
			if (vend == OEM_PICMG) {
			   PDEBUG("%sOEM PICMG  %c \n", mystr,bdelim);
			   //show_fru_picmg(pdata,dlen);
			} 
			else
			{
				if (vend == 0x0012A2) /* VITA 57.1 fmc Oui */
				{
				   PDEBUG("%sOEM VITA  %c \n", mystr,bdelim);
				   ret1 = show_fru_vita(pdata,dlen,fmceeprom,chkerr);
                   ret = UPDATE_ERROR(ret, ret1);
				} else
				   PDEBUG("%sOEM Ext    %c %06x %02x\n",
					  mystr,bdelim, vend, pdata[3]);
			}
		}
		else
		    PDEBUG("%s %02x %c %02x\n", mystr,mtype,bdelim, pdata[0]);
		break;
   }
   return(ret);
}

int show_fru(uchar sa, uchar frudev, uchar frutype, uchar *pfrubuf,int msfru,char mfdebug,char mfgetintchk,char mfcanonical,char mforcecont,FMCEEPROM_content *fmceeprom,
			 FMCEEPROM_vita_requierements *vitalevel, int prnout)
{
   int ret = FMCEEPROM_OK, ret1;
   int i, j, n, sz, tsz, multi_end;
   uchar *pfru0;
   uchar *pfru;
   uchar lang;
   TYPE_LEN tl;
   char newstr[64];
   int iaoff, ialen=0, bdoff, bdlen=0; 
   int proff, prlen=0, choff, chlen=0;
   int moff, mlen=0;
   char devstr[24];
   char *pstr;
   uchar header_chk1=0,ia_chk1=0,ch_chk1=0,bd_chk1=0,pr_chk1=0,header_chk0=0,ia_chk0=0,ch_chk0=0,bd_chk0=0,pr_chk0=0,ml_chk0,ml_chk1;

   // Erase the decoding structure
   memset(fmceeprom,0,sizeof(FMCEEPROM_content));

   // Save to internal vars
   sfru = msfru;
   fdebug = mfdebug;
   fgetintchk = mfgetintchk;
   fcanonical = mfcanonical;
   force_cont = mforcecont;
   _allows_printout = prnout;

   if (pfrubuf[0] == 0x80) {    /* 0x80 = type for DIMMs (SPD) */
      /* FRU Header: 80 08 07 0c 0a 01 48 00 (DIMM) */
      sz = pfrubuf[0];
      if (fdebug) {
	PDEBUG("DIMM SPD Body (size=%d/%d): ",sz,sfru);
	dumpbuf(pfrubuf,sfru);
      }
      show_spd(pfrubuf,sfru, frudev,frutype);
      ret = FMCEEPROM_SPD_DECODE;
      *vitalevel = FMCEEPROM_VITA_DOESNOTMEET;
	  return(ret);
   }

   pstr = FruTypeString(frutype,frudev);
   if (fcanonical) devstr[0] = 0;  /*default is empty string*/
   else sprintf(devstr,"[%s,%02x,%02x] ",pstr,sa,frudev);
   PDEBUG("%s%s FRU Size  %c %d\n",devstr,pstr,bdelim,sfru);

   /*
    * FRU header:
    *  0 = format_ver (01 is std, usu 0x80 if DIMM)
    *  1 = internal_use offset
    *  2 = chassis_info offset
    *  3 = board_info offset   (usu 6 fields)
    *  4 = product_info offset (usu 8 fields)
    *  5 = multirecord offset
    *  6 = pad (00)
    *  7 = header checksum (zero checksum)
    * FRU Header: 01 01 02 09 13 00 00 e0 (BMC)
    * FRU Header: 01 00 00 00 01 07 00 f7 (Power Cage)
    */
   pfru0 = &pfrubuf[0];  /*pointer to fru start*/
   pfru = &pfrubuf[0];
   sz = 8;  /*minimum for common header*/
   for (i = 1, tsz = 0; i < 6; i++) 	/* walk thru offsets */
	if (pfrubuf[i] != 0) tsz = pfrubuf[i] * 8;
   if (tsz != 0) {   		/* if have at least one section */
	if (pfrubuf[5] != 0)
	{
		do
		{
			multi_end = pfrubuf[tsz+1] & 0x80; /* End of multirecord list */
			j = 5 + pfrubuf[tsz+2]; /*if multi-record area get its size*/
			tsz += j;
		}while (!multi_end && tsz < sfru);
		sz = tsz;
	}
	else
	{
		j = pfrubuf[tsz+1] * 8;   /* else add length of last section */
	    sz = j + tsz;
	}
   }

   /* Now, sz = size used, sfru = total available size */
   if (sz > sfru) {
      if (fdebug) {
        PDEBUG("FRU Header:%c",32);
        for (i = 0; i < 8; i++) PDEBUG("%02x ",pfrubuf[i]);
        PDEBUG("%c\n",32);
      }
      PDEBUG("FRU size used=%d > available=%d\n",sz,sfru);
      if (fpicmg || fdoanyway) sz = sfru;  /*do it anyway*/
      else {
        PDEBUG("ERROR -  Please apply the correct FRU/SDR file%c\n",32);
        *vitalevel = FMCEEPROM_VITA_DOESNOTMEET;
	    return(FMCEEPROM_ERR_OTHER);
      }
   }

   /* chassis area offset, length */
   choff = pfrubuf[2] * 8;  
   if (choff) chlen = pfrubuf[choff + 1] * 8;
   /* board area offset, length */
   bdoff = pfrubuf[3] * 8;  
   if (bdoff) bdlen = pfrubuf[bdoff + 1] * 8;
   /* product area offset, length */
   proff = pfrubuf[4] * 8;  
   if (proff) prlen = pfrubuf[proff + 1] * 8;
   /* multi-record area offset, length */
   moff = pfrubuf[5] * 8;  
   mlen = 0;
   if (moff > 0) {
      for (i = moff; i < sfru; ) {
         j = 5 + pfrubuf[i+2];
	 mlen += j;
         if (pfrubuf[i+1] & 0x80) break;
	 i += j;
      }
   }
   /* internal area offset, length */
   /* (WARNING: assumption is that the internal area is formatted with the size at offset 1 with is unspecified by fru spec 1.0 rev1.1) 
      if fgetintchk=1 */
   iaoff = pfrubuf[1] * 8;  
   if (iaoff && fgetintchk)
	   ialen = pfrubuf[iaoff + 1] * 8;
   else
   {
	   if (iaoff)
	   {
		   // The ia section length is not specified by the fru spec 1.0 rev1.1, attempt to find the ia section length by looking at where is the
		   // nearest next section (internal section must not be the last one for this to work)...
		   int guess=sfru;
           if ((choff < guess) && (choff >iaoff))
			   guess = choff;
           if ((bdoff < guess) && (bdoff >iaoff))
			   guess = bdoff;
           if ((proff < guess) && (proff >iaoff))
			   guess = proff;
           if ((moff < guess) && (moff >iaoff))
			   guess = moff;
		   if (guess < sz)
			   ialen = guess - iaoff;
	   }
   }

   // Calculate the sections chksum
   header_chk1 = calc_cksum(&pfrubuf[0],7);
   header_chk0 = pfrubuf[7];
   if (iaoff && fgetintchk)
   {
	   ia_chk1 = calc_cksum(&pfrubuf[iaoff],ialen-1);
	   // WARNING: Assumption is that checksum is store at last byte in the section, this is not specified by fru 1.0 rev 1.1 spec when fgetintchk=1
	   ia_chk0 = pfrubuf[iaoff+ialen-1];
   }
   if (choff)
   {
	   ch_chk1 = calc_cksum(&pfrubuf[choff],chlen-1);
	   ch_chk0 = pfrubuf[choff+chlen-1];
   }
   if (bdoff)
   {
	   bd_chk1 = calc_cksum(&pfrubuf[bdoff],bdlen-1);
	   bd_chk0 = pfrubuf[bdoff+bdlen-1];
   }
   if (proff)
   {
	   pr_chk1 = calc_cksum(&pfrubuf[proff],prlen-1);
	   pr_chk0 = pfrubuf[proff+prlen-1];
   }

   if (fdebug) {
      PDEBUG("FRU Header:%c",32);
      for (i = 0; i < 8; i++) PDEBUG("%02x ",pfrubuf[i]);
      PDEBUG("%c\n",32);
      PDEBUG("FRU Body (size=%d/%d): ",sz,sfru);
      //dumpbuf(pfrubuf,sfru);
      dumpbuf(pfrubuf,sz);
      PDEBUG("header, off=0, len=%d, cksum0 = %02x, cksum1 = %02x\n",
                8,header_chk0,header_chk1);
      PDEBUG("internal off=%d, len=%d, cksum0 = %02x, cksum1 = %02x\n",
		iaoff,ialen,ia_chk0,ia_chk1);
      PDEBUG("chassis off=%d, len=%d, cksum0 = %02x, cksum1 = %02x\n",
		choff,chlen,ch_chk0,ch_chk1);
      PDEBUG("board off=%d, len=%d, cksum0 = %02x, cksum1 = %02x\n",
		bdoff,bdlen,bd_chk0,bd_chk1);
      PDEBUG("prod  off=%d, len=%d, cksum0 = %02x, cksum1 = %02x\n",
		proff,prlen,pr_chk0,pr_chk1);
      /* Multi-record area */
      PDEBUG("multi off=%d, len=%d, fru sz=%d\n", moff,mlen,sz);  
   }  /*endif fdebug, show header*/

   if (header_chk0 != header_chk1)
   {
       PDEBUG("\nERROR - invalid checksum in header section%c\n",32);
       ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_HDR_CHK);
   }

   if (iaoff != 0) {
      // Warning, checksum validation is unspecified in fru v1.0 rev1.1 spec
      if ((ia_chk0 != ia_chk1) && fgetintchk)
	  {
		  PDEBUG("\nERROR - invalid checksum in internal section: Rd:0x%02x, Cal:0x%02x\n",ia_chk0,ia_chk1);
          ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_IA_CHK);
		  fmceeprom->internal.presence = FMCEEPROM_PRE_CHKERROR;
	  }
	  else
	      // Assumes the section is correctly present if we cannot check the checksum
		  fmceeprom->internal.presence = FMCEEPROM_PRE_PRESENT;

	  dumpbuf(&pfrubuf[iaoff],ialen);  /*internal area dump*/
      PDEBUG("%sInternal Version  %c %d\n\n",devstr,bdelim,pfrubuf[iaoff]);
	  fmceeprom->internal.internal_area_version = pfrubuf[iaoff];

	  // If the internal section is formatted as other section with len, checksum, etc. (non fru 1.0 rev1.1 standard)
	  // This is a requierement for Nutaq custom section
	  if (fgetintchk)
	  {
		  if ((ialen - (1 + 1 + 1 + 1 + sizeof(fmceeprom->internal.IFields.iversion) + sizeof(fmceeprom->internal.IFields.isize))) < 0 )
		  {
			  fmceeprom->internal.presence = FMCEEPROM_PRE_OTHERROR;
              ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_INTERNAL_SIZECHK);
		  }
		  else
		  {
			  fmceeprom->internal.IFields.iversion = pfrubuf[iaoff+2];
			  fmceeprom->internal.IFields.isize = pfrubuf[iaoff+2+sizeof(fmceeprom->internal.IFields.iversion)] + (pfrubuf[iaoff+2+sizeof(fmceeprom->internal.IFields.iversion)+1] << 8);
			  if (fmceeprom->internal.IFields.isize <= sizeof(fmceeprom->internal.InternalZoneRaw) &&
				  fmceeprom->internal.IFields.isize <= (ialen - (1 + 1 + 1 + 1)) &&
				  fmceeprom->internal.IFields.isize >= (sizeof(fmceeprom->internal.IFields.iversion) + sizeof(fmceeprom->internal.IFields.isize)))
			      memcpy(&fmceeprom->internal.InternalZoneRaw[sizeof(fmceeprom->internal.IFields.iversion)+sizeof(fmceeprom->internal.IFields.isize)],
				         &pfrubuf[iaoff+2+sizeof(fmceeprom->internal.IFields.iversion)+sizeof(fmceeprom->internal.IFields.isize)],
				         fmceeprom->internal.IFields.isize - (sizeof(fmceeprom->internal.IFields.iversion) + sizeof(fmceeprom->internal.IFields.isize)));
			  else
			  {
			      // Invalid internal structure size
				  fmceeprom->internal.presence = FMCEEPROM_PRE_OTHERROR;
                  ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_INTERNAL_SIZECHK2);
			  }
		  }
	  }
	  else
	  {
	      // This section would appear incorrect for fmc decoding, but does not generate any error to support decoding more standard format
		  // without an explicitly defined internal section
		  fmceeprom->internal.presence = FMCEEPROM_PRE_PARERROR;
	  }
   }

   if (choff != 0) {
      if (ch_chk0 != ch_chk1)
	  {
		  PDEBUG("\nERROR - invalid checksum in chassis section: Rd:0x%02x, Cal:0x%02x\n",ch_chk0,ch_chk1);
          ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_CH_CHK);
		  fmceeprom->chassis.presence = FMCEEPROM_PRE_CHKERROR;
	  }
	  else
		  fmceeprom->chassis.presence = FMCEEPROM_PRE_PRESENT;

	   /* show Chassis area fields */
      pfru = &pfrubuf[choff];
      lang = 25;  /* English */
      ctype = pfru[2];  /*chassis type*/
	  fmceeprom->chassis.type = pfru[2];
	  fmceeprom->chassis.chassis_area_version = pfru[0];
      if (fdebug) PDEBUG("ctype=%x\n",ctype);
      if (ctype >= MAX_CTYPE)
	  {
		  ctype = MAX_CTYPE - 1;
		  fmceeprom->chassis.presence = FMCEEPROM_PRE_OTHERROR;
		  if (!force_cont)
			  // !!! Maybe some unknown chassis type are possibles, do not gen an error here?
              ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_CHASSIS_TYPE);
	  }
      PDEBUG("%s%s%c %s\n",devstr, ctype_hdr,bdelim,ctypes[ctype]);
      pfru += 3;  /* skip chassis header */
      tl.len  = 0;
      for (i = 0; i < NUM_CHASSIS_FIELDS; i++)
      {
         if (ChkOverflow((int)(pfru - pfru0),sfru,tl.len))
		 {
             ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_OVERFLOW);
			 fmceeprom->chassis.presence = FMCEEPROM_PRE_OVRERROR;
			 break;
		 }
         if (pfru[0] == FRU_END) break;  /*0xC1 = end of FRU area*/
         if (!ValidTL(pfru[0]))
		 {
	        PDEBUG("\nERROR - Invalid Type/Length %02x for %s\n",
			       pfru[0],chassis[i]);
			fmceeprom->chassis.presence = FMCEEPROM_PRE_TLFERROR;
			
		    if (!force_cont)
			    // !!! don't know if error generation is correct here?
                ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_CHASSIS_TYPELEN);
		 }
         tl.type = (pfru[0] & FRU_TYPE_MASK) >> 6;
         tl.len  = pfru[0] & FRU_LEN_MASK;
         if (i == 2) {  /* OEM field for chassis_name  */
	   chassis_offset = (int)(pfru - pfrubuf);
	   chassis_len    = tl.len;
	   if (fdebug) PDEBUG("chassis oem dtype=%d lang=%d len=%d\n",
				tl.type,lang,tl.len);
         }
         pfru++;
	 {
	   newstr[0] = 0;
	   if (FAILURE(ret1 = decode_string(tl.type,lang,pfru,tl.len,newstr,sizeof(newstr))))
       {
           ret = UPDATE_ERROR(ret, ret1);
		   fmceeprom->chassis.presence = FMCEEPROM_PRE_TLFERROR;
       }
       else
	   {
		   if (i < FMCEEPROM_MAX_CHASSIS_FIELDS)
		   {
		       fmceeprom->chassis.chassis_typelen_fields[i].type = tl.type;
			   fmceeprom->chassis.chassis_typelen_fields[i].len = tl.len;
		       strncpy((char *)fmceeprom->chassis.chassis_typelen_fields[i].data,newstr,sizeof(fmceeprom->chassis.chassis_typelen_fields[i].data)-1);
   		       memcpy(fmceeprom->chassis.chassis_typelen_fields[i].undecode,pfru,tl.len);
			   fmceeprom->chassis.chassis_typelen_fields[i].presence = FMCEEPROM_PRE_PRESENT;
		   }
		   else
		   {
			   fmceeprom->chassis.presence = FMCEEPROM_PRE_PARERROR;
			   if (!force_cont)
                   ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_CHASSIS_MANYFILEDS);
		   }
	   }
	   PDEBUG("%s%s%c %s\n",devstr, chassis[i],bdelim,newstr);
         }
         pfru += tl.len;
      }
      if (fdebug) PDEBUG("num Chassis fields = %d\n",i);
   }

   if (bdoff != 0) {
      long nMin, nSec;
      time_t tsec;
      
      if (bd_chk0 != bd_chk1)
	  {
		  PDEBUG("\nERROR - invalid checksum in board section: Rd:0x%02x, Cal:0x%02x\n",bd_chk0,bd_chk1);
          ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_BD_CHK);
		  fmceeprom->board.presence = FMCEEPROM_PRE_CHKERROR;
	  }
	  else
	  {
		  fmceeprom->board.presence = FMCEEPROM_PRE_PRESENT;
	  }

	  /* show Board area fields */
      pfru = &pfrubuf[bdoff];
      lang = pfru[2];
	  fmceeprom->board.language = lang;
	  fmceeprom->board.board_area_version = pfru[0];
      PDEBUG("%sLanguage  %c %d\n",devstr,bdelim,lang);
      /* Decode board mfg date-time (num minutes since 1/1/96) */
      nMin = pfru[3] + (pfru[4] << 8) + (pfru[5] << 16);
	  memcpy(fmceeprom->board.manuf_date_time,&pfru[3],sizeof(fmceeprom->board.manuf_date_time));
#ifdef OLD
		  /* 13674540 min from 1/1/70 to 1/1/96 */
      nSec = (nMin + 13674540) * 60;
#else
		  /* 820454400 (820472400) sec from 1/1/70 to 1/1/96 */
      //nSec = (nMin * 60) + 820454400;
      nSec = (nMin * 60) + 820472400;
#endif
      tsec = (time_t)(nSec & 0x0ffffffff);
      // fmt_time(tsec,newstr,sizeof(newstr)); 
      PDEBUG("%sBoard Mfg DateTime  %c %s",devstr,bdelim,ctime(&tsec));
	  strncpy((char *)fmceeprom->board.manuf_date_time_str,ctime(&tsec),sizeof(fmceeprom->board.manuf_date_time_str)-1);
	  // Remove linefeed if string exists
	  if (strlen(ctime(&tsec)) > 0)
          fmceeprom->board.manuf_date_time_str[strlen(ctime(&tsec)) - 1] = 0;

      pfru += 6;  /* skip board header */
      tl.len  = 0;
      for (i = 0; i < NUM_BOARD_FIELDS; i++)
      {
         if (ChkOverflow((int)(pfru - pfru0),sfru,tl.len))
		 {
             ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_OVERFLOW);
			 fmceeprom->board.presence = FMCEEPROM_PRE_OVRERROR;
			 break;
		 }
         if (pfru[0] == FRU_END) break;  /*0xC1 = end*/
         if (!ValidTL(pfru[0]))
		 {
	        PDEBUG("\nERROR - Invalid Type/Length %02x for %s\n",
			pfru[0],board[i]);
			fmceeprom->board.presence = FMCEEPROM_PRE_TLFERROR;
		    if (!force_cont)
			    // !!! don't know if error generation is correct here?
                ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_BOARD_TYPELEN);
		 }
         tl.type = (pfru[0] & FRU_TYPE_MASK) >> 6;
         tl.len  = pfru[0] & FRU_LEN_MASK;
         pfru++;
         {
	   newstr[0] = 0;

	   if (FAILURE(ret1 = decode_string(tl.type,lang,pfru,tl.len,newstr,sizeof(newstr))))
	   {
           ret = UPDATE_ERROR(ret, ret1);
		   fmceeprom->board.presence = FMCEEPROM_PRE_TLFERROR;
	   }
       else
	   {
		   if (i < FMCEEPROM_MAX_BOARD_FIELDS)
		   {
		       fmceeprom->board.board_typelen_fields[i].type = tl.type;
			   fmceeprom->board.board_typelen_fields[i].len = tl.len;
		       strncpy((char *)fmceeprom->board.board_typelen_fields[i].data,newstr,sizeof(fmceeprom->board.board_typelen_fields[i].data)-1);
   		       memcpy(fmceeprom->board.board_typelen_fields[i].undecode,pfru,tl.len);
			   fmceeprom->board.board_typelen_fields[i].presence = FMCEEPROM_PRE_PRESENT;
		   }
		   else
		   {
			   fmceeprom->board.presence = FMCEEPROM_PRE_PARERROR;
			   if (!force_cont)
                   ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_BOARD_MANYFILEDS);
		   }
	   }
	   
	   PDEBUG("%s%s%c %s\n",devstr, board[i],bdelim,newstr);
         }
         pfru += tl.len;
      }
      if (fdebug) PDEBUG("num Board fields = %d\n",i);
   }

   if (proff != 0) 
   {
      if (pr_chk0 != pr_chk1)
	  {
		  PDEBUG("\nERROR - invalid checksum in product section: Rd:0x%02x, Cal:0x%02x\n",pr_chk0,pr_chk1);
          ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_PR_CHK);
		  fmceeprom->product.presence = FMCEEPROM_PRE_CHKERROR;
	  }
	  else
		  fmceeprom->product.presence = FMCEEPROM_PRE_PRESENT;

      /* show Product area fields */
      pfru = &pfrubuf[proff];
      maxprod = pfru[1] * 8;
      lang = pfru[2];
	  fmceeprom->product.language = lang;
	  fmceeprom->product.product_area_version = pfru[0];
      PDEBUG("%sLanguage  %c %d\n",devstr,bdelim,lang);
      pfru += 3;  /* skip product header */
      tl.len  = 0;
      for (i = 0; i < NUM_PRODUCT_FIELDS; i++)
      {
         if (ChkOverflow((int)(pfru - pfru0),sfru,tl.len))
		 {
             ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_OVERFLOW);
			 fmceeprom->product.presence = FMCEEPROM_PRE_OVRERROR;
			 break;
		 }
         if (*pfru == FRU_END) break;  /*0xC1 = end*/
         //if (*pfru == 0) *pfru = FRU_EMPTY_FIELD; /* fix a broken table */
         if (!ValidTL(pfru[0]))
		 {
	        PDEBUG("\nERROR - Invalid Type/Length %02x for %s\n",
			pfru[0],product[i]);
            ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_TYPE);
			fmceeprom->product.presence = FMCEEPROM_PRE_TLFERROR;
		    if (!force_cont)
			    // !!! don't know if error generation is correct here?
                ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_PRODUCT_TYPELEN);
		 }
         tl.type = (pfru[0] & FRU_TYPE_MASK) >> 6;
         tl.len  = pfru[0] & FRU_LEN_MASK;
	 if ((frudev == 0) && (sa == bmc_sa)) {  /*baseboard FRU*/
           if (i == 5) {  /* asset tag */
	      asset_offset = (int)(pfru - pfrubuf);
	      asset_len    = tl.len;
	      if (fdebug) PDEBUG("asset off=%d dtype=%d lang=%d len=%d\n",
				 asset_offset,tl.type,lang,tl.len);
   	      if (fshowlen)  /* show asset tag length for main board */
		 PDEBUG("%s%c %d\n",asset_hdr,bdelim,asset_len);
           } else if (i == 4) {  /* serial number */
	      sernum_offset = (int)(pfru - pfrubuf);
	      sernum_len    = tl.len;
	      if (fdebug) PDEBUG("sernum dtype=%d lang=%d len=%d\n",
				 tl.type, lang, tl.len);
           } else if (i == 3) {  /* product version number */
	      prodver_offset = (int)(pfru - pfrubuf);
	      prodver_len    = tl.len;
	      if (fdebug) PDEBUG("prodver dtype=%d lang=%d len=%d\n",
				 tl.type, lang, tl.len);
	   }
	 }  /*if baseboard sa*/
         pfru++;
         {
	   newstr[0] = 0;
	   if (FAILURE(ret1 = decode_string(tl.type,lang,pfru,tl.len,newstr,sizeof(newstr))))
       {
           ret = UPDATE_ERROR(ret, ret1);
		   fmceeprom->product.presence = FMCEEPROM_PRE_TLFERROR;
       }
       else
       {
		   if (i < FMCEEPROM_MAX_PROD_FIELDS)
		   {
		       fmceeprom->product.prod_typelen_fields[i].type = tl.type;
			   fmceeprom->product.prod_typelen_fields[i].len = tl.len;
		       strncpy((char *)fmceeprom->product.prod_typelen_fields[i].data,newstr,sizeof(fmceeprom->product.prod_typelen_fields[i].data)-1);
   		       memcpy(fmceeprom->product.prod_typelen_fields[i].undecode,pfru,tl.len);
			   fmceeprom->product.prod_typelen_fields[i].presence = FMCEEPROM_PRE_PRESENT;
		   }
		   else
		   {
			   fmceeprom->product.presence = FMCEEPROM_PRE_PARERROR;
			   if (!force_cont)
                   ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_PRODUCT_MANYFILEDS);
		   }
       }
	   PDEBUG("%s%s%c %s\n",devstr, product[i],bdelim,newstr);
         }
         pfru += tl.len;
      }
      if (fdebug) 
	   PDEBUG("num Product fields = %d, last=%x, max = %d\n", 
		   i,*pfru,maxprod );
      //if (*pfru == 0x00) *pfru = FRU_END;  /* insert end char if broken */
   }

   if (moff != 0) 
   {
      /* multi-record area may contain several record headers 
       * 0 = record type id
       * 1 = 0x02 or 0x82 if End-of-List
       * 2 = record len
       * 3 = record chksum
       * 4 = header chksum
       */
      pfru = &pfrubuf[moff];
      j = moff;
      for (i = 0; j < sz ; i++)
      {
          int mret;
          
          mret=FMCEEPROM_OK;
          
		  n = pfru[2];  /* len of this record */

	      ml_chk1 = calc_cksum(&pfru[0],4);
	      ml_chk0 = pfru[4];

  		  if (ml_chk1 != ml_chk0)
		  {
		      PDEBUG("\nERROR - invalid checksum in m-record header[%d]: Rd:0x%02x, Cal:0x%02x\n",i,ml_chk0,ml_chk1);
              ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_MLHDR_CHK);
              mret = FMCEEPROM_ERR_MLHDR_CHK;
		  }

	      ml_chk1 = calc_cksum(&pfru[5],n);
	      ml_chk0 = pfru[3];

  		  if (ml_chk1 != ml_chk0)
		  {
		      PDEBUG("\nERROR - invalid checksum in m-record data[%d]: Rd:0x%02x, Cal:0x%02x\n",i,ml_chk0,ml_chk1);
              ret = UPDATE_ERROR(ret, FMCEEPROM_ERR_MLDAT_CHK);
              mret = FMCEEPROM_ERR_MLDAT_CHK;
		  }

		 ret1 = show_fru_multi(devstr,i,pfru[0],&pfru[5],n,fmceeprom,mret);
         ret = UPDATE_ERROR(ret, ret1);

         j += (5 + n);
         if (pfru[1] & 0x80) j = sz;  /*0x80 = last in list, break*/
         pfru += (5 + n);
      }
   }

   if ((frudev == 0) && (sa == bmc_sa) && do_guid) {
	PDEBUG("%sSystem GUID         %c ",devstr,bdelim);
	show_guid(guid);
	PDEBUG("%c\n",32);
   }
   
   // If no error, check for vita compliance
   if (SUCCESS(ret))
   {
	   ret = check_vita_requierements(fmceeprom, *vitalevel, vitalevel);
   }
   else
   {
      *vitalevel = FMCEEPROM_VITA_DOESNOTMEET;
   }
   return(ret);
}

/* end ifru.c */
