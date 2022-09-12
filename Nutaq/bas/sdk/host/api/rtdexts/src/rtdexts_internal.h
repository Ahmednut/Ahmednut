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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**    
 *
 * @file    rtdexts_internal.h
 * @brief   Private definitions for the RTDEx Timestamp module. 
 *
 * $Date: 2015/08/10 17:10:41 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RTDEXSYNC
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
#ifndef RTDEXTS_INTERNAL_H__
#define RTDEXTS_INTERNAL_H__

#ifdef __cplusplus
    extern "C" {
#endif

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#include "bas_types.h"
#include "connection_state.h"       // connection_state
#include "rtdexsync_def.h"          // RTDExSync_Handle_t
#include "timestamp_defines.h"      // Timestamp_Handle_t

/*****************************************************************************
 *  RTDExSync Object Definition
 */

/**
 * RTDExTs Object.
 */
typedef struct RTDExTs_Obj
{
    // Chassis
    connection_state state;                     ///< Connection state (platform CCE)
    RTDExTs_Dir_t dir;
    int64_t  i64TickOffset;                     ///< Time offset to compensate for the TX or RX propagation time in timestamp tick
    
    RTDExSync_Handle_t hRTDExSync;              ///< RTDExSync handle
    Timestamp_Handle_t hTimestamp;              ///< Timestamp handle
    

} RTDExTs_Obj_t;


#ifdef __cplusplus
}
#endif

#endif  // RTDEXTS_INTERNAL_H__

