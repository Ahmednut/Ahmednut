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
 * @file    bas_types.h
 * @brief   BAS basic types defintion.
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
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
 
#ifndef BAS_LYR_TYPES_H
#define BAS_LYR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SKIPGETCH
#define GETCH() _getch()
#else
#define GETCH() -1
#endif

#if defined(_WIN32) || defined(_WIN64)
  #define _WINSOCKAPI_
  #include "windows.h"
  typedef HANDLE adp_handle_drv;
  typedef HANDLE adp_thread;
  typedef HANDLE adp_mutex;
#endif

#ifdef __linux__

  typedef int adp_mutex;
  //typedef fd adp_handle_drv;

#endif


typedef int adp_result_t;
typedef int BOOL;

#ifdef __linux__
  #include <stdint.h>
#else
  //#ifndef __int8_t_defined
  //#define __int8_t_defined
  #ifndef _STDINT_H
    typedef char  		  	    int8_t;
    typedef short 			    int16_t;       
    typedef int  			    int32_t;         
    typedef long long   	    int64_t;       
    typedef unsigned char 	    uint8_t;         
    typedef unsigned short      uint16_t;   
    typedef unsigned int 	    uint32_t;       
    typedef unsigned long long  uint64_t;
  #endif
#endif

#ifndef TRUE
#define TRUE    1
#endif

#ifndef FALSE
#define FALSE   0
#endif

#ifndef true
#define true	1
#endif

#ifndef false
#define false	0
#endif

typedef struct string_enum_element {
	char     enum_string[255];
	uint64_t enum_value;
} string_enum_element_t;

typedef int adp_handle;

typedef adp_handle adp_handle_media;

typedef struct connection_state * hConnectionState_t;

#define ADP_INVALID_HANDLE 0xffffffff

#ifdef _LP64
#define PRI_U64	"lu"
#define PRI_X64	"lX"
#else
#define PRI_U64	"llu"
#define PRI_X64	"llX"
#endif


typedef enum
{
    eProtocolCtrl,
    eProtocolRTDEX,
    eProtocolRecPlay,
    eProtocolUser
}
eProtocoleId;



#ifdef __cplusplus
}
#endif

#endif
