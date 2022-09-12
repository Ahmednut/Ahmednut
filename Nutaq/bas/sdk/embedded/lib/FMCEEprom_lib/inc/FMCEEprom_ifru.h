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
 * @file       FMCEEprom_ifru.h
 * @brief      fmceeprom library common routines from ifru.c.
 *
 * $Date: 2015/04/10 14:08:11 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup FMCEEPROMLIB
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
#ifndef __FMCEEPROM_IFRU_H__
#define __FMCEEPROM_IFRU_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


#ifndef uchar
#define uchar unsigned char
#endif

#include "FMCEEprom_ifru_vita.h"

extern int _allows_printout;

int decode_string(unsigned char type, unsigned char language_code, unsigned char *source, int slen, char *target,int tsize);
int show_fru(uchar sa, uchar frudev, uchar frutype, uchar *pfrubuf,int msfru,char mfdebug,char mfgetintchk,char mfcanonical,char mforcecont,FMCEEPROM_content *fmceeprom,
			 FMCEEPROM_vita_requierements *vitalevel, int prnout);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif
/* end ifru.h */
