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
 * @file    timestamp_internal_def.h 
 * @brief   This file contains internal macro, definition and type for TIMESTAMP
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup TIMESTAMP
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef TIMESTAMP_INTERNAL_DEF__
#define TIMESTAMP_INTERNAL_DEF__

#include "bas_error.h"
#include "timestamp_defines.h"

#define TIMESTAMP_CORE_ID (0xCC03)

// INFO
typedef struct sTimestampCoreVersID {
#ifdef _BIG_ENDIAN
    unsigned ID:16, Version:16;
#else
    unsigned Version:16, ID:16;
#endif
} sTimestampCoreVersID_t;

// CONTROL
typedef struct sTimestampCtrl{
#ifdef _BIG_ENDIAN
	unsigned rsvd:29, LatchTime:1, SetTime:1, CoreReset:1;
#else
	unsigned CoreReset:1, SetTime:1, LatchTime:1, rsvd:29;
#endif
} sTimestampCtrl_t;

typedef struct sTimestampCore {
    sTimestampCoreVersID_t CoreVersID;
    sTimestampCtrl_t Control;
    unsigned int TimestampMSW;
    unsigned int TimestampLSW;
} sTimestampCore_t;

typedef struct sTimestamp_Obj
{
    volatile sTimestampCore_t * Core;    /**< FPGA core */
    int (*pfUsleep)(unsigned int usec);
} sTimestamp_Obj_t;

#endif
