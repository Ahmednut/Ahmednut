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
 * @file       FMCEEprom_vita.c
 * @brief      fmceeprom library helper routines for FRU VITA decoding.
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
#ifdef WIN32
// Uncomment this line to add some console output strings (mandatory to display decoded info on the console)
#define FMCEEPROM_PRINT 1

#include <windows.h>
#include <stdio.h>
#include <string.h>
#else

#ifdef DEBUG
// Uncomment this line to add some console output strings (mandatory to display decoded info on the console)
#define FMCEEPROM_PRINT 1
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#endif

#include <time.h>

// Uncomment following line to check in vita Nutaq conformance if the vadj is one of perseus 601x supported voltages
// This is a safety check to make sure vadj volatge would be regognized on a perseus601x, but would prevent using any other vadj voltage
// for a different carrier
#define FMCEEPROM_PERSEUS601X_VADJCHK 1


#include "FMCEEprom_ipmicmd.h" 
#include "FMCEEprom_ipicmg.h" 
#include "FMCEEprom_ifru.h"
#include "FMCEEprom_ifru_vita.h"

#define VITA_OEM_DATALEN_SUBTYPE_0 11

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

int show_fru_vita(uchar *pdata, int dlen,FMCEEPROM_content *fmceeprom,int chkerr)
{
   int ret = FMCEEPROM_OK, ret1, p1s = -1, p2s = -1, p1p = 0, p2p = 0;
   int id, version, ascii6length, fieldtype;
   unsigned i;
   char newstr[128];
   unsigned char i2c_baseaddr[15] = {0<<3,1<<3,2<<3,3<<3,4<<3,5<<3,6<<3,7<<3,8<<3,9<<3,11<<3,12<<3,13<<3,14<<3,15<<3},ptext;
   char i2cdec[100],i2cname[256];
   unsigned char i2cfound,i2cntype,lenname;

   id = (pdata[3] & 0xf0) >> 4;
   switch(id) {
	case 0:  /* subtype 0 as defined in vita 57.1 */
		PDEBUG("\tFRU_VITA_SUBTYPE_0 Base Definition%c\n",32);

        // Check if oem data len is valid for vita 57.1
        if (dlen != VITA_OEM_DATALEN_SUBTYPE_0)
		{
    	   PDEBUG("\tERROR - Invalid OEM data len: requiered %d, current %d\n",VITA_OEM_DATALEN_SUBTYPE_0,dlen);
		   ret = FMCEEPROM_VITA_INVALIDOEM_DATALEN;
		}
        else
        {
			fmceeprom->vitamain.subtype = 0;
            version = pdata[3] & 0x0f; 
			fmceeprom->vitamain.version = version;
			if (version == 0) /* Version 0 for subtype 0? */
			{
				PDEBUG("\tFRU_VITA_VERSION_0%c\n",32);
				i = (pdata[4] & 0xc0)>>6;
			    fmceeprom->vitamain.modsize = i;
				if (i == 0)
				{
				   PDEBUG("\tModule Size                 : single width%c\n",32);
				   p1p = 1;
				}
				else if (i == 1 )
				{
				   PDEBUG("\tModule Size                 : double width%c\n",32);
				   p1p = 1;
				   p2p = 1;
				}

				else
				{
				   PDEBUG("\tERROR - Module Size         : unknown %d\n", i);
		           ret = FMCEEPROM_VITA_INVALIDMOD_SIZE;
			       fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
				}
	            
				i = (pdata[4] & 0x30)>>4;
			    fmceeprom->vitamain.p1connsize = i;
				if (i == 0)
				{
				   if (!p1p)
				   {
					   PDEBUG("\tERROR - Module P1 presence  : unknown%c\n",32);
					   ret = FMCEEPROM_VITA_INVALIDMOD_SIZE;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
				   }
				   else
				   {
					   PDEBUG("\tP1 Connector Size           : LPC%c\n",32);
					   p1s = 0;
				   }
				}
				else if (i == 1 )
				{
				   if (!p1p)
				   {
					   PDEBUG("\tERROR - Module P1 presence  : unknown%c\n",32);
					   ret = FMCEEPROM_VITA_INVALIDMOD_SIZE;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
				   }
				   else
				   {
					   PDEBUG("\tP1 Connector Size           : HPC%c\n",32);
					   p1s = 1;
				   }
				}
				else
				{
				   PDEBUG("\tERROR - P1 Connector Size   : unknown %d\n", i);
		           ret = FMCEEPROM_VITA_INVALIDP1CON_SIZE;
			       fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
				}
	            
				i = (pdata[4] & 0x0C)>>2;
			    fmceeprom->vitamain.p2connsize = i;
				if (i == 0)
				{
				   if (!p2p)
				   {
					   PDEBUG("\tERROR - Module P2 presence  : unknown%c\n",32);
					   ret = FMCEEPROM_VITA_INVALIDMOD_SIZE;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
				   }
				   else
				   {
				       PDEBUG("\tP2 Connector Size           : LPC%c\n",32);
					   p2s = 0;
				   }
				}
				else if (i == 1 )
				{
				   if (!p2p)
				   {
					   PDEBUG("\tERROR - Module P2 presence  : unknown%c\n",32);
					   ret = FMCEEPROM_VITA_INVALIDMOD_SIZE;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
				   }
				   else
				   {
				       PDEBUG("\tP2 Connector Size           : HPC%c\n",32);
					   p2s = 1;
				   }
				}
				else if (i == 3 )
				{
				   PDEBUG("\tP2 Connector Size           : not fitted%c\n",32);
				   p2s = 2;
				}
				else
				{
				   PDEBUG("\tERROR - P2 Connector Size   : unknown %d\n", i);
		           ret = FMCEEPROM_VITA_INVALIDP2CON_SIZE;
			       fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
				}
	            
				i = (pdata[4] & 0x02)>>1;
			    fmceeprom->vitamain.clkdir = i;
				if (i == 0)
				{
				   PDEBUG("\tClock Direction             : from Mezzanine to Carrier%c\n",32);
				}
				else
				{
				   PDEBUG("\tClock Direction             : from Carrier to Mezzanine%c\n",32);
				}
	            
				i = (pdata[4] & 0x01);
			    fmceeprom->vitamain.rsv = i;
				if (i != 0)
				{
				   PDEBUG("\tERROR - Invalid rsrvd field : unknown %d\n", i);
		           ret = FMCEEPROM_VITA_RESERVED1;
			       fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
				}
	            
				i = pdata[5];
			    fmceeprom->vitamain.p1_banka_sig = i;
				// Check P1 connector presence
				if (p1p)
				{
					// P1 LPC
					if (!p1s)
					{
						if (i > 34 || i < 0)
						{
						   PDEBUG("\tERROR - P1 LPC invalid prs A: unknown %d\n", i);
						   ret = FMCEEPROM_VITA_INVALIDP1CON_ASIGN;
			               fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
						}
					}
					else
					{
						// P1 HPC
						if (p1s==1)
						{
							if (i > 58 || i < 0)
							{
						       PDEBUG("\tERROR - P1 HPC invalid prs A: unknown %d\n", i);
							   ret = FMCEEPROM_VITA_INVALIDP1CON_ASIGN;
			                   fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
							}
						}
					}
				}
				else
				{
					if (i != 0)
					{
				       PDEBUG("\tERROR - P1 invalid pairs A  : unknown %d\n", i);
					   ret = FMCEEPROM_VITA_INVALIDP1CON_ASIGN;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
					}
				}
				PDEBUG("\tP1 Bank A number of signals : %d\n",i);

				i = pdata[6];
			    fmceeprom->vitamain.p1_bankb_sig = i;
				// Check P1 connector presence
				if (p1p)
				{
					// P1 LPC
					if (!p1s)
					{
						if (i != 0)
						{
						   PDEBUG("\tERROR - P1 LPC invalid prs B: unknown %d\n", i);
						   ret = FMCEEPROM_VITA_INVALIDP1CON_BSIGN;
			               fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
						}
					}
					else
					{
						// P1 HPC
						if (p1s==1)
						{
							if (i > 22 || i < 0)
							{
						       PDEBUG("\tERROR - P1 HPC invalid prs B: unknown %d\n", i);
							   ret = FMCEEPROM_VITA_INVALIDP1CON_BSIGN;
			                   fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
							}
						}
					}
				}
				else
				{
					if (i != 0)
					{
				       PDEBUG("\tERROR - P1 invalid pairs B  : unknown %d\n", i);
					   ret = FMCEEPROM_VITA_INVALIDP1CON_BSIGN;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
					}
				}
				PDEBUG("\tP1 Bank B number of signals : %d\n",i);

				i = pdata[7];
			    fmceeprom->vitamain.p2_banka_sig = i;
				// Check P2 connector presence
				if (p2p)
				{
					// P2 LPC
					if (!p2s)
					{
						if (i > 34 || i < 0)
						{
						   PDEBUG("\tERROR - P2 LPC invalid prs A: unknown %d\n", i);
						   ret = FMCEEPROM_VITA_INVALIDP2CON_ASIGN;
			               fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
						}
					}
					else
					{
						// P2 HPC
						if (p2s==1)
						{
							if (i > 58 || i < 0)
							{
						       PDEBUG("\tERROR - P2 HPC invalid prs A: unknown %d\n", i);
							   ret = FMCEEPROM_VITA_INVALIDP2CON_ASIGN;
			                   fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
							}
						}
						else
						{
							// P2 not fitted
							if (p2s==2)
							{
								if (i != 0)
								{
								   PDEBUG("\tERROR - P2 invalid pairs A  : unknown %d\n", i);
								   ret = FMCEEPROM_VITA_INVALIDP2CON_ASIGN;
			                       fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
								}
							}
						}
					}
				}
				else
				{
					if (i != 0)
					{
				       PDEBUG("\tERROR - P2 invalid pairs A  : unknown %d\n", i);
					   ret = FMCEEPROM_VITA_INVALIDP2CON_ASIGN;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
					}
				}
				PDEBUG("\tP2 Bank A number of signals : %d\n",i);

				i = pdata[8];
			    fmceeprom->vitamain.p2_bankb_sig = i;
				// Check P2 connector presence
				if (p2p)
				{
					// P2 LPC
					if (!p2s)
					{
						if (i != 0)
						{
						   PDEBUG("\tERROR - P2 LPC invalid prs B: unknown %d\n", i);
						   ret = FMCEEPROM_VITA_INVALIDP2CON_BSIGN;
			               fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
						}
					}
					else
					{
						// P2 HPC
						if (p2s==1)
						{
							if (i > 22 || i < 0)
							{
						       PDEBUG("\tERROR - P2 HPC invalid prs B: unknown %d\n", i);
							   ret = FMCEEPROM_VITA_INVALIDP2CON_BSIGN;
			                   fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
							}
						}
						else
						{
							// P2 not fitted
							if (p2s==2)
							{
								if (i != 0)
								{
								   PDEBUG("\tERROR - P2 invalid pairs B  : unknown %d\n", i);
								   ret = FMCEEPROM_VITA_INVALIDP2CON_BSIGN;
			                       fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
								}
							}
						}
					}
				}
				else
				{
					if (i != 0)
					{
				       PDEBUG("\tERROR - P2 invalid pairs B  : unknown %d\n", i);
					   ret = FMCEEPROM_VITA_INVALIDP2CON_BSIGN;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
					}
				}
				PDEBUG("\tP2 Bank B number of signals : %d\n",i);

				i = (pdata[9] & 0xF0)>>4;
			    fmceeprom->vitamain.p1_gbt_sig = i;
				if (!p1p)
				{
					if (i!=0)
					{
					   PDEBUG("\tERROR - P1 invalid GBT      : unknown %d\n", i);
					   ret = FMCEEPROM_VITA_INVALIDP1CON_GBT;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
					}
				}
				else
				{
					if (p1s==0 && i > 1)
					{
					   PDEBUG("\tERROR - P1 invalid LPC GBT  : unknown %d\n", i);
					   ret = FMCEEPROM_VITA_INVALIDP1CON_GBT;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
					}
					else
					{
						if (p1s==1 && i > 10)
						{
						   PDEBUG("\tERROR - P1 invalid HPC GBT  : unknown %d\n", i);
						   ret = FMCEEPROM_VITA_INVALIDP1CON_GBT;
			               fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
						}
					}
				}
				PDEBUG("\tP1 GBT Number Transceivers  : %d\n",i);

				i = (pdata[9] & 0x0F);
			    fmceeprom->vitamain.p2_gbt_sig = i;
				if (!p2p)
				{
					if (i!=0)
					{
					   PDEBUG("\tERROR - P2 invalid GBT      : unknown %d\n", i);
					   ret = FMCEEPROM_VITA_INVALIDP2CON_GBT;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
					}
				}
				else
				{
					if (p2s==0 && i > 1)
					{
					   PDEBUG("\tERROR - P2 invalid LPC GBT  : unknown %d\n", i);
					   ret = FMCEEPROM_VITA_INVALIDP2CON_GBT;
			           fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
					}
					else
					{
						if (p2s==1 && i > 10)
						{
						   PDEBUG("\tERROR - P2 invalid HPC GBT  : unknown %d\n", i);
						   ret = FMCEEPROM_VITA_INVALIDP2CON_GBT;
			               fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
						}
						else
						{
							if ((p2s<0 || p2s==2) && i !=0)
							{
							   PDEBUG("\tERROR - P2 invalid GBT      : unknown %d\n", i);
							   ret = FMCEEPROM_VITA_INVALIDP2CON_GBT;
			                   fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
							}
						}
					}
				}
			    PDEBUG("\tP2 GBT Number Transceivers  : %d\n",i);

				i = pdata[10];
			    fmceeprom->vitamain.max_tck_clk = i;
				PDEBUG("\tMax clock for TCK           : %d MHz\n",i);
                
                if (FAILURE(chkerr))
			        fmceeprom->vitamain.presence = FMCEEPROM_PRE_CHKERROR;
                else
                {
                    if (ret == FMCEEPROM_OK)
			            fmceeprom->vitamain.presence = FMCEEPROM_PRE_PRESENT;
                }
			}
			else
			{
			   PDEBUG("\tERROR - Unknown VITA Subtype 0, version %x\n",version);
			   ret = FMCEEPROM_VITA_UNKNOWN_SUBT0_VER;
			   fmceeprom->vitamain.presence = FMCEEPROM_PRE_OTHERROR;
			}
        }
		break;
	case 1:  /* subtype 1 as defined in vita 57.1 */
        i2cfound = 0;
        i2cntype = 0;
        i2cname[0]=0;
        lenname = 0;
		PDEBUG("\tFRU_VITA_SUBTYPE_1 I2C Device Definition%c\n",32);

		fmceeprom->vitai2c.subtype = 1;

        // Get the compressed length of the ascii6 bits string
        ascii6length =  (pdata[4] & 0x3F);
        fieldtype = ((pdata[4] & 0xC0)>>6);
        
        // Check if oem data len is valid for vita 57.1 subtype 1 (must find ascii 6 bits type field there for I2C string)
        // Must have the correct length and length of the strig must be a multiple of 3 for correct decoding
        if (!(fieldtype == 0x02 && dlen == (5 + ascii6length) && (ascii6length % 3) == 0 ))
		{
    	   PDEBUG("\tERROR - Invalid OEM ascii 6 bits field%c\n",32);
		   ret = FMCEEPROM_VITA_INVALID_OEM_ASCII6;
		   fmceeprom->vitai2c.presence = FMCEEPROM_PRE_OTHERROR;
		}
        else
        {
			version = pdata[3] & 0x0f; 
		    fmceeprom->vitai2c.version = version;
			if (version == 0) /* Version 0 for subtype 1? */
			{
				PDEBUG("\tFRU_VITA_VERSION_0%c\n",32);
				
	            newstr[0] = 0;
                
                if (FAILURE(ret1 = decode_string(fieldtype,0,&pdata[5],ascii6length,newstr,sizeof(newstr))))
				{
					PDEBUG("\tERROR - Invalid OEM ascii 6 bits field decoding.%c\n",32);
		            fmceeprom->vitai2c.presence = FMCEEPROM_PRE_TLFERROR;
					// There was an error while decoding the string
					if (SUCCESS(ret))
					    ret = ret1;
				}
                else
                {
    		        fmceeprom->vitai2c.vitai2c_str.type = fieldtype;
    			    fmceeprom->vitai2c.vitai2c_str.len = ascii6length;
    		        strncpy((char*)fmceeprom->vitai2c.vitai2c_str.data,newstr,sizeof(fmceeprom->vitai2c.vitai2c_str.data)-1);
    		        memcpy(fmceeprom->vitai2c.vitai2c_str.undecode,&pdata[5],ascii6length);
    			    fmceeprom->vitai2c.vitai2c_str.presence = FMCEEPROM_PRE_PRESENT;
                }
				PDEBUG("\tI2C Device String: %s\n\tDevices 7 bits(8 bits) i2c base address decoding:",newstr);

				// Check if the i2c string is valid according to vita 57.1 (not all 6 bits ascii chars are valid)
				for (i=0, ptext=1; i < strlen(newstr); i++)
					if (newstr[i]<=' ' || (newstr[i]>'9' && newstr[i]<'A') || newstr[i]>'Z')
					{
						PDEBUG("\n\tERROR - Invalid I2C character: '%c', ascii: %d, position: %d\n",newstr[i], newstr[i], i);
						ret = FMCEEPROM_VITA_INVALID_I2C_CHAR;
		                fmceeprom->vitai2c.presence = FMCEEPROM_PRE_OTHERROR;
						break;
					}
					else
						// Decode the string by displaying each i2c device address and type
						if (newstr[i] >= '!' && newstr[i] <= '/')
						{
							if (ptext)
                            {
								PDEBUG("%c\n\t",32);
								// Are we at beginning of decoding?
								if (i != 0)
								{
									strncpy(fmceeprom->vitai2c.i2cdevicetype[i2cntype].devname,i2cname,sizeof(fmceeprom->vitai2c.i2cdevicetype[i2cntype].devname)-1);
									fmceeprom->vitai2c.i2cdevicetype[i2cntype].devnum = i2cfound;
									fmceeprom->vitai2c.i2cdevicetype[i2cntype].presence = FMCEEPROM_PRE_PRESENT;
									i2cfound = 0;
									i2cname[0]=0;
									lenname = 0;
									i2cntype++;
								}
                            }
							sprintf(i2cdec,"0x%02X(0x%02X)",i2c_baseaddr[newstr[i] - '!'],(i2c_baseaddr[newstr[i] - '!'])<<1);
                            PDEBUG("%s ",i2cdec);
                            strncpy(fmceeprom->vitai2c.i2cdevicetype[i2cntype].addrmsb_dec[i2cfound],i2cdec,sizeof(fmceeprom->vitai2c.i2cdevicetype[i2cntype].addrmsb_dec[i2cfound])-1);
                            fmceeprom->vitai2c.i2cdevicetype[i2cntype].addrmsb[i2cfound] = i2c_baseaddr[newstr[i] - '!'];
                            i2cfound++;
                            //fmceeprom->vitai2c.i2cdevicetype[i2cntype].devnum = i2cfound;
							ptext=0;
						}
						else
						{
						    PDEBUG("%c",newstr[i]);
                            i2cname[lenname++] = newstr[i];
                            i2cname[lenname] = 0;
							ptext=1;
						}
				PDEBUG("%c\n",32);
                if(i2cfound)
                {
					strncpy(fmceeprom->vitai2c.i2cdevicetype[i2cntype].devname,i2cname,sizeof(fmceeprom->vitai2c.i2cdevicetype[i2cntype].devname)-1);
					fmceeprom->vitai2c.i2cdevicetype[i2cntype].devnum = i2cfound;
					fmceeprom->vitai2c.i2cdevicetype[i2cntype].presence = FMCEEPROM_PRE_PRESENT;
					i2cntype++;
                }
                if (i2cntype)
                {
					fmceeprom->vitai2c.i2cdevtypenum = i2cntype;
                }
                if (FAILURE(chkerr))
			        fmceeprom->vitai2c.presence = FMCEEPROM_PRE_CHKERROR;
                else
                {
                    if (ret == FMCEEPROM_OK)
			            fmceeprom->vitai2c.presence = FMCEEPROM_PRE_PRESENT;
                }
			}
			else
			{
			   PDEBUG("\tERROR - Unknown VITA Subtype 1, version %x\n",version);
			   ret = FMCEEPROM_VITA_UNKNOWN_SUBT1_VER;
		       fmceeprom->vitai2c.presence = FMCEEPROM_PRE_OTHERROR;
			}
        }
		break;
	default:
		PDEBUG("\tERROR - Unknown VITA Subtype %x\n",id);
		ret = FMCEEPROM_VITA_UNKNOWN_SUBT;
		break;
   }
   return(ret);
}

// Check for vita requierements level according to what is specified by level. If the level is
// invalid, it will return an error. The detected level is also returned in rdlevel.
// It is assumed before calling this function that there was no errors while decoding the
// eeprom structure.
int check_vita_requierements(FMCEEPROM_content *fmceeprom, FMCEEPROM_vita_requierements level, FMCEEPROM_vita_requierements *rdlevel )
{
    FMCEEPROM_vita_requierements dlevel = FMCEEPROM_VITA_DOESNOTMEET;
    int ret = FMCEEPROM_OK, count;
    
	// Check if the level specified is valid (we cannot ask for FMCEEPROM_VITA_DOESNOTMEET neither)
    if (level > FMCEEPROM_VITA_NUTAQFULL || level <= FMCEEPROM_VITA_DOESNOTMEET)
    {
        ret = FMCEEPROM_ERR_INVALID_VITA_REQ_LEVEL;
    }
    else
    {
        // Check if we asked at least to meet minimum vita requierements
        //if (level >= FMCEEPROM_VITA_MINIMUM)
        {
            // Check for board section presence and validity
			if (fmceeprom -> board.presence != FMCEEPROM_PRE_PRESENT) goto checkexit;
			
			// Check for Vita main section presence and validity
			if (fmceeprom -> vitamain.presence != FMCEEPROM_PRE_PRESENT) goto checkexit;

			// Check for power dc load sections related to connector P1
			for(count = FMCEEPROM_POWER_P1_VADJ_DCLD; count <= FMCEEPROM_POWER_P1_12P0V_DCLD; count++)
			{
				if (fmceeprom->power[count].presence != FMCEEPROM_PRE_PRESENT) goto checkexit;
				if (fmceeprom->power[count].ptype != FMCEEPROM_POWER_DCLOAD) goto checkexit;
			}

			// Check for power dc output sections related to connector P1
			for(count = FMCEEPROM_POWER_P1_VIOBM2C_DCO; count <= FMCEEPROM_POWER_P1_VRFBM2C_DCO; count++)
			{
				if (fmceeprom->power[count].presence != FMCEEPROM_PRE_PRESENT) goto checkexit;
				if (fmceeprom->power[count].ptype != FMCEEPROM_POWER_DCOUTPUT) goto checkexit;
			}

			// Check if connector P2 is fitted and verifies related power sections
			if (fmceeprom->vitamain.modsize == FMCEEPROM_VITAMODSZ_DBLWIDTH &&
				fmceeprom->vitamain.p2connsize != FMCEEPROM_VITACONNT_NOTFITTED)
			{
				// Check for power dc load sections related to connector P2
				for(count = FMCEEPROM_POWER_P2_VADJ_DCLD; count <= FMCEEPROM_POWER_P2_12P0V_DCLD; count++)
				{
					if (fmceeprom->power[count].presence != FMCEEPROM_PRE_PRESENT) goto checkexit;
					if (fmceeprom->power[count].ptype != FMCEEPROM_POWER_DCLOAD) goto checkexit;
				}

				// Check for power dc output sections related to connector P2
				for(count = FMCEEPROM_POWER_P2_VIOBM2C_DCO; count <= FMCEEPROM_POWER_P2_VRFBM2C_DCO; count++)
				{
					if (fmceeprom->power[count].presence != FMCEEPROM_PRE_PRESENT) goto checkexit;
					if (fmceeprom->power[count].ptype != FMCEEPROM_POWER_DCOUTPUT) goto checkexit;
				}
			}
			else
			{
				// Check for power sections related to connector P2
				for(count = FMCEEPROM_POWER_P2_VADJ_DCLD; count <= FMCEEPROM_POWER_P2_VRFBM2C_DCO; count++)
				{
					if (fmceeprom->power[count].presence != FMCEEPROM_PRE_ABSENT) goto checkexit;
				}
			}

			// There must be no additionnal power sections
			for(count = FMCEEPROM_POWER_UNKNOWN1; count <= FMCEEPROM_POWER_UNKNOWN4; count++)
			{
				if (fmceeprom->power[count].presence != FMCEEPROM_PRE_ABSENT) goto checkexit;
			}

			// Level requierements met
			dlevel = FMCEEPROM_VITA_MINIMUM;
		}

        // Check if we meet all vita minimum requierements including optionnal ones, and nutaq ones
        if (dlevel == FMCEEPROM_VITA_MINIMUM)
        {
            // Check for vita i2c section presence and validity
			if (fmceeprom->vitai2c.presence == FMCEEPROM_PRE_PRESENT)
                // Level requierements met
		        dlevel = FMCEEPROM_VITA_FULL;
            
            // Check for internal section presence and validity
			if (fmceeprom->internal.presence != FMCEEPROM_PRE_PRESENT) goto checkexit;
			
            // Check for internal section really decoded in the structure
			if (fmceeprom->internal.IFields.isize == 0) goto checkexit;
			
            // Check for internal section really decoded in the structure
			if (fmceeprom->internal.internal_area_version != 1) goto checkexit;
			
			// Check if board section version is valid
			if (fmceeprom->board.board_area_version != 1) goto checkexit;

			// Check if board section language is valid
			if (fmceeprom->board.language != 25) goto checkexit;

			// Check how many ascii fields are decoded in the board section, if the correct fields types and minimum expected length
			for (count = FMCEEPROM_BOARD_MANUFACTURER; count <= FMCEEPROM_BOARD_CUSTOM2; count++)
			{
				if (fmceeprom->board.board_typelen_fields[count].presence != FMCEEPROM_PRE_PRESENT) goto checkexit;

				if (fmceeprom->board.board_typelen_fields[count].type != FMCEEPROM_ASCII ||
					fmceeprom->board.board_typelen_fields[count].len < 2) goto checkexit;
			}

			{
				unsigned int nMin;
#ifndef ZC706 //time function not supported for now        
                time_t t1;
                time(&t1);

				// Current time in min from 1996 (820472400 seconds from 1970 to 1996) + extra 90 min (allows to manufacture 90mn in future)
        
				t1 = (t1 - 820472400) / 60 + 90;
#endif
	            // Check minimum manufacturer date (1 fev 2013 17h00) and not greater than current time
				nMin = fmceeprom->board.manuf_date_time[0] + (fmceeprom->board.manuf_date_time[1] << 8) + (fmceeprom->board.manuf_date_time[2] << 16);

#ifndef ZC706 //time function not supported for now				
				// Check if manufacturer time is smaller than (1 fev 2013 17h00) or greater than (current time + 90 mn)
				if (nMin > t1 || nMin < 8988060) goto checkexit;
#endif
			}

			// Check nominal voltage for dc load fix power supplies
			if (fmceeprom->power[FMCEEPROM_POWER_P1_3P3V_DCLD].nominal_volt != 330) goto checkexit;

			if (fmceeprom->power[FMCEEPROM_POWER_P1_12P0V_DCLD].nominal_volt != 1200) goto checkexit;

#ifdef FMCEEPROM_PERSEUS601X_VADJCHK
			// Check if vadj is one of perseus voltage 2.5V, 1.8V, 1.5V, 1.2V
			if (fmceeprom->power[FMCEEPROM_POWER_P1_VADJ_DCLD].nominal_volt != 250 &&
				fmceeprom->power[FMCEEPROM_POWER_P1_VADJ_DCLD].nominal_volt != 180 &&
				fmceeprom->power[FMCEEPROM_POWER_P1_VADJ_DCLD].nominal_volt != 150 &&
				fmceeprom->power[FMCEEPROM_POWER_P1_VADJ_DCLD].nominal_volt != 120) goto checkexit;
#endif

			// Check if connector P2 is fitted and verifies related power sections
			if (fmceeprom->vitamain.modsize == FMCEEPROM_VITAMODSZ_DBLWIDTH &&
				fmceeprom->vitamain.p2connsize != FMCEEPROM_VITACONNT_NOTFITTED)
			{
				if (fmceeprom->power[FMCEEPROM_POWER_P2_3P3V_DCLD].nominal_volt != 330) goto checkexit;

				if (fmceeprom->power[FMCEEPROM_POWER_P2_12P0V_DCLD].nominal_volt != 1200) goto checkexit;

#ifdef FMCEEPROM_PERSEUS601X_VADJCHK
				// Check if vadj is one of perseus voltage 2.5V, 1.8V, 1.5V, 1.2V
				if (fmceeprom->power[FMCEEPROM_POWER_P2_VADJ_DCLD].nominal_volt != 250 &&
					fmceeprom->power[FMCEEPROM_POWER_P2_VADJ_DCLD].nominal_volt != 180 &&
					fmceeprom->power[FMCEEPROM_POWER_P2_VADJ_DCLD].nominal_volt != 150 &&
					fmceeprom->power[FMCEEPROM_POWER_P2_VADJ_DCLD].nominal_volt != 120) goto checkexit;
#endif
			}

			if (dlevel == FMCEEPROM_VITA_FULL)
                // Level requierements met
			    dlevel = FMCEEPROM_VITA_NUTAQFULL;
            else
                // Level requierements met
			    dlevel = FMCEEPROM_VITA_NUTAQMIN;
        }
    }

checkexit:
   // Returns the level reach for what was asked to be verified
   *rdlevel = dlevel;
   return(ret);
}
