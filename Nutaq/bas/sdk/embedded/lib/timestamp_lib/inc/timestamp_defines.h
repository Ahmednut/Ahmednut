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
 * @file    timestamp_defines.h 
 * @brief   This file contains macro, definition and type for TIMESTAMP
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup TIMESTAMP
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

#ifndef TIMESTAMP_DEFINES__
#define TIMESTAMP_DEFINES__

#include "bas_error.h"

typedef struct sTimestamp_Obj * Timestamp_Handle_t;

/****************************************************************************
*
* TIMESTAMP error code
*
****************************************************************************/
enum eTimestampError {
    TIMESTAMPOK             = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_TIMESTAMP, 0),    /**< No error */
    TIMESTAMPABSENT         = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_TIMESTAMP, 1),    /**< Timestamp FPGA core is not present */
    TIMESTAMPOUTOFMEMORY    = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_TIMESTAMP, 2),    /**< Timestamp out of memory */
    TIMESTAMPINVHANDLE      = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_TIMESTAMP, 3),    /**< Timestamp invalid handle */
    TIMESTAMPPPSABSENT      = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_TIMESTAMP, 4),    /**< PPS signal is not present */
    TIMESTAMPOUTOFSYNC      = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_TIMESTAMP, 5),    /**< The platform time is not synchronize with the host time*/
    TIMESTAMPINVFREQ        = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_TIMESTAMP, 6),    /**< Invalid timestamp clock frequency*/
    TIMESTAMPINVARRAYLENGTH = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_TIMESTAMP, 7),    /**< Invalid array length for returning the result*/
    TIMESTAMPINVTIME        = MAKE_RESULT(LYR_SEVERITY_ERROR,   MODULE_TIMESTAMP, 8),    /**< Invalid time*/
};

#endif
