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
 * @file       timestamp.c  
 * @brief      Initialize and configure the timestamp core
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
#include <stdlib.h>

#include "bas_types.h"
#include "bas_error.h"
#include "timestamp.h"
#include "timestamp_internal_def.h"
#include "timestamp_defines.h"

Result Timestamp_Open(Timestamp_Handle_t * phTimestamp, uint32_t u32BaseAddress, int (*pfUsleep)(unsigned int usec))
{
    if(phTimestamp == NULL) return ERR_INVPTR;
        
    *phTimestamp = (Timestamp_Handle_t)malloc( sizeof(sTimestamp_Obj_t) );
    if( *phTimestamp == NULL ) return ERR_OUTOFMEMORY;
        
    (*phTimestamp)->Core = (sTimestampCore_t *)u32BaseAddress;
    (*phTimestamp)->pfUsleep = pfUsleep;
    
    if( SUCCESS( Timestamp_Presence(*phTimestamp) ) )
        Timestamp_Reset(*phTimestamp);

    return TIMESTAMPOK;
}

Result Timestamp_Presence(Timestamp_Handle_t hTimestamp)
{
    if(hTimestamp == NULL)
        return ERR_INVHANDLE;

    if(hTimestamp->Core->CoreVersID.ID == TIMESTAMP_CORE_ID)
        return TIMESTAMPOK;
    else
        return TIMESTAMPABSENT;
}

Result Timestamp_GetVersion(Timestamp_Handle_t hTimestamp, unsigned int * puiVersion)
{
    if(hTimestamp == NULL)
        return ERR_INVHANDLE;

    if(hTimestamp->Core->CoreVersID.ID != TIMESTAMP_CORE_ID)
        return TIMESTAMPABSENT;
    
    *puiVersion = hTimestamp->Core->CoreVersID.Version;
    
    return TIMESTAMPOK;
}

Result Timestamp_SetTime(Timestamp_Handle_t hTimestamp, uint64_t u64Timestamp)
{
    if(hTimestamp == NULL)
        return ERR_INVHANDLE;
        
    if( FAILURE( Timestamp_Presence(hTimestamp) ) ) return TIMESTAMPABSENT;
        
    hTimestamp->Core->TimestampMSW = (u64Timestamp >> 32) & 0xFFFFFFFF;
    hTimestamp->Core->TimestampLSW = (u64Timestamp      ) & 0xFFFFFFFF;
    
    hTimestamp->pfUsleep(1);
    
    hTimestamp->Core->Control.SetTime = 1;
    
    return TIMESTAMPOK;
}

Result Timestamp_GetTime(Timestamp_Handle_t hTimestamp, uint64_t * pu64Timestamp)
{
    if(hTimestamp == NULL)
        return ERR_INVHANDLE;
        
    if( FAILURE( Timestamp_Presence(hTimestamp) ) ) return TIMESTAMPABSENT;
    
    hTimestamp->Core->Control.LatchTime = 1;
    
    hTimestamp->pfUsleep(1);
        
    *pu64Timestamp = (uint64_t)hTimestamp->Core->TimestampLSW;
    *pu64Timestamp |= (((uint64_t)hTimestamp->Core->TimestampMSW) << 32);
    
    return TIMESTAMPOK;
}

Result Timestamp_Reset(Timestamp_Handle_t hTimestamp)
{
    if(hTimestamp == NULL)
        return ERR_INVHANDLE;
        
    if( FAILURE( Timestamp_Presence(hTimestamp) ) ) return TIMESTAMPABSENT;
    
    hTimestamp->Core->Control.CoreReset = 1;
    
    return TIMESTAMPOK;
}

Result Timestamp_Close(Timestamp_Handle_t hTimestamp)
{
    if(hTimestamp == NULL)
        return ERR_INVHANDLE;

    free(hTimestamp);
    
    return TIMESTAMPOK;
}

