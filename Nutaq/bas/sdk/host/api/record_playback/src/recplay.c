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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      recplay.c
 * 
 * @brief     Functions definition for Record Playback access.
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
#include <stdlib.h>

#include "bas_error.h"
#include "bas_types.h"
#include "media_types.h"
#include "connection_state.h"
#include "bas_time.h"
#include "recplay_types.h"
#include "memory.h"
#include "recplay.h"
#include "eapi.h"


#define RecPlayValidateArgSizeM8( Arg )  ( Arg & 0x7 )
#define RecPlayValidateArgSizeM64( Arg )  ( Arg & 0x3f )

static  int Log2( unsigned int uValue )
{
    unsigned int i;

    for( i = 0; i < 32; i++ )
    {
        if( uValue & ( 1 << i ) )
        {
            return i;
        }
    
    }

    return 0;
}

static adp_result_t RecPlay_CheckCoreID(adp_handle_recplay hRecPlay)
{
    adp_result_t res;
    unsigned int uID;

    res = memory_read_send( hRecPlay -> state,
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_COREID_INDEX,
                      &uID );
    if( ((uID & 0xFFFF0000)>>16) != RECPLAY_ACCESS_COREID)
    {
        return RECPLAYWRONGCOREID;
    }
    else
    {
        return 0;
    }
}

adp_result_t RecPlay_GetMemorySize(adp_handle_recplay hRecPlay, RECPLAY_DDR3Size *size)
{
    adp_result_t res;
    unsigned int uSize;

    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_DDR3SIZE_INDEX,
                      &uSize );
    if(uSize == 4) 
    {
        *size = RECPLAY_DDR3SIZE_4G; 
        return 0;
    }
    else if(uSize == 1) 
    {
        *size = RECPLAY_DDR3SIZE_1G; 
        return 0;
    }
    else
    {
        *size = RECPLAY_DDR3SIZE_1G; 
        return 0;
    }
}

adp_result_t RecPlay_Open (adp_handle_recplay *hRecPlay, connection_state * state,
                                     uint64_t u64RecPlayFpgaBaseAddr )
{
    adp_result_t res;
    
    adp_handle_recplay hTempRecPlay = NULL;
    
    *hRecPlay = NULL;
    
    hTempRecPlay = (adp_handle_recplay)malloc(sizeof(sRecPlayStruct));
    if(hTempRecPlay == NULL)
    {
        return RECPLAYMEMALLOC;
    }

    hTempRecPlay -> state = state;
    hTempRecPlay -> u64RecPlayFpgaBaseAdd = u64RecPlayFpgaBaseAddr;
    
    res = RecPlay_CheckCoreID(hTempRecPlay);
    if(res)
    {
        free(hTempRecPlay);
        return res;
    }
    
    res = RecPlay_GetMemorySize(hTempRecPlay, &hTempRecPlay -> DDR3Size);
    if(res)
    {
        free(hTempRecPlay);
        return res;
    }
    
    
    res = memory_read_send( state, (unsigned int)u64RecPlayFpgaBaseAddr + RECPLAY_ACCESS_MODERECORD_INDEX,
                             &hTempRecPlay -> uRecordModeValue );
    if(res)
    {
        free(hTempRecPlay);
        return res;
    }

    res = memory_read_send( state, (unsigned int)u64RecPlayFpgaBaseAddr + RECPLAY_ACCESS_MODEPLAYBACKSINGLE_INDEX,
                             &hTempRecPlay -> uPlayBackSingleModeValue );
    if(res)
    {
        free(hTempRecPlay);
        return res;
    }

    res = memory_read_send( state, (unsigned int)u64RecPlayFpgaBaseAddr + RECPLAY_ACCESS_MODEPLAYBACKCONTINOUS_INDEX,
                             &hTempRecPlay -> uPlayBackContinuousModeValue );
    if(res)
    {
        free(hTempRecPlay);
        return res;
    }

    res = memory_read_send( state, (unsigned int)u64RecPlayFpgaBaseAddr + RECPLAY_ACCESS_MODERTDEXMEM2HOST_INDEX,
                             &hTempRecPlay -> uRtdexMem2HostModeValue );
    if(res)
    {
        free(hTempRecPlay);
        return res;
    }

    res = memory_read_send( state, (unsigned int)u64RecPlayFpgaBaseAddr + RECPLAY_ACCESS_MODERTDEXHOST2MEM_INDEX,
                             &hTempRecPlay -> uRtdexHost2MemModeValue );
    if(res)
    {
    
        free(hTempRecPlay);
        return res;
    }

    *hRecPlay = hTempRecPlay;

    return(res);
}


adp_result_t RecPlay_Close( adp_handle_recplay hRecPlay  )
{
    free(hRecPlay);
    return RECPLAYOK;
}


adp_result_t RecPlayResetMemory( adp_handle_recplay hRecPlay )
{
 
    adp_result_t res;
    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_CONTROL_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_CONTROL_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    return RECPLAYOK;
}

adp_result_t RecPlayResetCore( adp_handle_recplay hRecPlay )
{
    adp_result_t res;
    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_CONTROL_INDEX,
                       2 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_CONTROL_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state, (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_PFULLASSERT_INDEX, 450 );

    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state, (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_PFULLNEGATE_INDEX, 449 );

    if(res)
    {
        return res;
    }

    return RECPLAYOK;
}


 adp_result_t RecPlayGetMemoryInitDoneStatus( adp_handle_recplay hRecPlay,
                                              unsigned char* pInitDone )
 {
    adp_result_t res;
    unsigned int uInitDone;
 
    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_CONTROL_INDEX,
                      &uInitDone );

    *pInitDone = (uInitDone & 4) >> 2; 

    return res;
 }

 adp_result_t RecPlayGetTransferStatus( adp_handle_recplay hRecPlay,
                                        unsigned char* pTransferOver )
 {
    adp_result_t res;
    unsigned int uTransferOver;

    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_TRANSFEROVER_INDEX,
                      &uTransferOver );

    *pTransferOver = uTransferOver; 

    return res;
 }

adp_result_t RecPlayWaitTransferDone(adp_handle_recplay hRecPlay,
    unsigned int uTimeOutMs)
{
	adp_result_t res = 0;
	lyr_delay_counter timer;
	unsigned int u32DelayMsec;
	double dDelaySec;	
	unsigned char uTransferOver;

	// Start Timeout timer
	lyr_counter_reset(&timer);
    lyr_counter_begin(&timer);

	do
    {
		lyr_counter_end(&timer);
        lyr_counter_get(&timer, &dDelaySec);
        u32DelayMsec = (unsigned int)(dDelaySec * 1000);
        res = RecPlayGetTransferStatus( hRecPlay, &uTransferOver );
    }while(uTransferOver == 0 && res == 0 && u32DelayMsec <= uTimeOutMs);

	if(FAILURE(res)) return res;

    if( uTransferOver == 0 )
    {
        return RECPLAYTRANSFERTIMEOUT;
    }

	return RECPLAYOK;
}

 adp_result_t RecPlaySetModeRecord( adp_handle_recplay hRecPlay,
                                    unsigned int uStartAddress,
                                    unsigned int uTrigDelayVal,
                                    unsigned int uRecSizeFromTrigOffset )
 {
    adp_result_t res;
	unsigned int uMaxVal = 0;
	
	if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_1G)
	{
		uMaxVal = 0x3FFFFFFF;
	}
	else if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_4G)
	{
		uMaxVal = 0xFFFFFFFF;
	}
	else
	{
		uMaxVal = 0;
	}
	
    // Input args
    if( RecPlayValidateArgSizeM64( uStartAddress ) != 0  )
    {
        return RECPLAYINVSTARTADDR;
    }    
	if(uStartAddress > uMaxVal)
	{
        return RECPLAYINVSTARTADDR;
	}
    
    if( RecPlayValidateArgSizeM64( uRecSizeFromTrigOffset ) != 0  )
    {
        return RECPLAYINVARGSIZE;
    }
    if (uRecSizeFromTrigOffset > uMaxVal)
    {
        return RECPLAYINVARGSIZE;
    }
    
    if( RecPlayValidateArgSizeM64( uTrigDelayVal ) != 0  )
    {
        return RECPLAYINVDELAY;
    }
    if (uTrigDelayVal > uMaxVal)
    {
        return RECPLAYINVDELAY;
    }
	
    
    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_STARTADDR_INDEX,
                       uStartAddress/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_TRANSFERSIZE_INDEX,
                       uRecSizeFromTrigOffset/8 );
    if(res)
    {
        return res;
    }


    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_MODEVALUE_INDEX,
                       hRecPlay -> uRecordModeValue );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_DELAYVAL_INDEX,
                       uTrigDelayVal/64 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETSTARTADDR_INDEX,
                       1 );
    if(res)
    {
        return res;
    }


    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }


    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETMODE_INDEX,
                       1 );

    return res;
}

adp_result_t RecPlayGetTriggerPos( adp_handle_recplay hRecPlay,
                                   unsigned int* puTrigAddress )
{
    adp_result_t res;

    unsigned int uTrigAddr = 0;
	unsigned int uTrigAddrMask = 0;
    unsigned int uTrigOffset;
    unsigned int uParity;
    unsigned int uDataOffset;
    unsigned uNbRecPort, uRecPortWidth;
    unsigned int uLsbBitPos;
	
	if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_1G)
	{
		uTrigAddrMask = RECPLAY_TRIGGERADDRMASK_1G;
	}
	else if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_4G)
	{
		uTrigAddrMask = RECPLAY_TRIGGERADDRMASK_4G;
	}
	else
	{
		uTrigAddrMask = 1;
	}

    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_TRIGGERADDR_INDEX,
                      &uTrigAddr );    
    if(FAILURE(res)) return res;

    uTrigAddr &= uTrigAddrMask;

    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_TRIGGERADDROFFSET_INDEX,
                      &uTrigOffset );    
    if(FAILURE(res)) return res;

    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_PARITY_INDEX,
                      &uParity );    
    if(FAILURE(res)) return res;

    res = memory_read_send( hRecPlay -> state, 
                             (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_NBRECPORT_INDEX,
                             &uNbRecPort );    
    if(FAILURE(res)) return res;

    res = memory_read_send( hRecPlay -> state, 
                             (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_RECPORTWIDTH_INDEX,
                             &uRecPortWidth );    
    if(FAILURE(res)) return res;

    // To avoid impossible log2 of 0, this register (RECPLAY_ACCESS_TRIGGERADDROFFSET_INDEX) cannot returns 0
    if( uTrigOffset == 0 )
    {
        return RECPLAYINVTRIGOFFSET;
    }

    // Find MSB at 1
    uLsbBitPos = 31;
    while ( (uTrigOffset & 0x80000000) == 0 )
    {
        uTrigOffset <<= 1;
        uLsbBitPos--;
    }

    uDataOffset = ( 32 - uLsbBitPos ) +  ( 32 * uParity ) - uNbRecPort * uRecPortWidth / 8;
	
	*puTrigAddress = uTrigAddr * 8 + uDataOffset;

    return RECPLAYOK;
}


 adp_result_t RecPlaySetModePlayBackSingle( adp_handle_recplay hRecPlay,
                                            unsigned int uStartAddress,
                                            unsigned int uTransferSize )
 {
    adp_result_t res;
	unsigned int uLastAddr;

	if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_1G)
	{
		uLastAddr = 0x3FFFFFFF;
	}
    
	else if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_4G)
	{
		uLastAddr = 0xFFFFFFFF;
	}
	else
	{
		uLastAddr = 0;
	}

    if( RecPlayValidateArgSizeM64( uStartAddress ) != 0  )
    {
        return RECPLAYINVSTARTADDR;
    }
	if(uStartAddress > uLastAddr)
	{
		return RECPLAYINVSTARTADDR;
	}	
 
    if( RecPlayValidateArgSizeM64( uTransferSize ) != 0  )
    {
        return RECPLAYINVARGSIZE;
    }
	if(uTransferSize > uLastAddr)
	{
		return RECPLAYINVARGSIZE;
	}	

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_STARTADDR_INDEX,
                       uStartAddress/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_TRANSFERSIZE_INDEX,
                       uTransferSize/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_MODEVALUE_INDEX,
                       hRecPlay -> uPlayBackSingleModeValue );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETSTARTADDR_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETMODE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETSTARTADDR_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETMODE_INDEX,
                       0 );
    if(res)
    {
        return res;
    }


    return RECPLAYOK; 
 
 
 }

 adp_result_t RecPlaySetModePlayBackContinuous( adp_handle_recplay hRecPlay,
                                                unsigned int uStartAddress,
                                                unsigned int uTransferSize )
  {
    adp_result_t res;
	unsigned int uLastAddr;

	if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_1G)
	{
		uLastAddr = 0x3FFFFFFF;
	}
    
	else if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_4G)
	{
		uLastAddr = 0xFFFFFFFF;
	}
	else
	{
		uLastAddr = 0;
	}

    if( RecPlayValidateArgSizeM64( uStartAddress ) != 0  )
    {
        return RECPLAYINVSTARTADDR;
    }
	if(uStartAddress > uLastAddr)
	{
		return RECPLAYINVSTARTADDR;
	}	
 
    if( RecPlayValidateArgSizeM64( uTransferSize ) != 0  )
    {
        return RECPLAYINVARGSIZE;
    }
	if(uTransferSize > uLastAddr)
	{
		return RECPLAYINVARGSIZE;
	}	

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_STARTADDR_INDEX,
                       uStartAddress/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_TRANSFERSIZE_INDEX,
                       uTransferSize/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_MODEVALUE_INDEX,
                       hRecPlay -> uPlayBackContinuousModeValue );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETSTARTADDR_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETMODE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETSTARTADDR_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETMODE_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    return 0; 
 
 }

 adp_result_t RecPlaySetModeRtdexHost2Mem( adp_handle_recplay hRecPlay,
                                           unsigned int uStartAddress,
                                           unsigned int uTransferSize )
{
    adp_result_t res;
	unsigned int uLastAddr;

	if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_1G)
	{
		uLastAddr = 0x3FFFFFFF;
	}
    
	else if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_4G)
	{
		uLastAddr = 0xFFFFFFFF;
	}
	else
	{
		uLastAddr = 0;
	}

    if( RecPlayValidateArgSizeM64( uStartAddress ) != 0  )
    {
        return RECPLAYINVSTARTADDR;
    }
	if(uStartAddress > uLastAddr)
	{
		return RECPLAYINVSTARTADDR;
	}	
 
    if( RecPlayValidateArgSizeM64( uTransferSize ) != 0  )
    {
        return RECPLAYINVARGSIZE;
    }
	if(uTransferSize > uLastAddr)
	{
		return RECPLAYINVARGSIZE;
	}	

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_STARTADDR_INDEX,
                       uStartAddress/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_TRANSFERSIZE_INDEX,
                       uTransferSize/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_MODEVALUE_INDEX,
                       hRecPlay -> uRtdexHost2MemModeValue );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETSTARTADDR_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETMODE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }


	return 0;
}


 adp_result_t RecPlaySetModeRtdexMem2Host( adp_handle_recplay hRecPlay,
                                           unsigned int uStartAddress,
                                           unsigned int uTransferSize )
 {
    adp_result_t res;
	unsigned int uLastAddr;

	if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_1G)
	{
		uLastAddr = 0x3FFFFFFF;
	}
    
	else if(hRecPlay -> DDR3Size == RECPLAY_DDR3SIZE_4G)
	{
		uLastAddr = 0xFFFFFFFF;
	}
	else
	{
		uLastAddr = 0;
	}

    if( RecPlayValidateArgSizeM64( uStartAddress ) != 0  )
    {
        return RECPLAYINVSTARTADDR;
    }
	if(uStartAddress > uLastAddr)
	{
		return RECPLAYINVSTARTADDR;
	}	
 
    if( RecPlayValidateArgSizeM64( uTransferSize ) != 0  )
    {
        return RECPLAYINVARGSIZE;
    }
	if(uTransferSize > uLastAddr)
	{
		return RECPLAYINVARGSIZE;
	}	
 
    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_STARTADDR_INDEX,
                       uStartAddress/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_TRANSFERSIZE_INDEX,
                       uTransferSize/8 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_MODEVALUE_INDEX,
                       hRecPlay -> uRtdexMem2HostModeValue );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETSTARTADDR_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETMODE_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETSTARTADDR_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_SETMODE_INDEX,
                       0 );
    if(res)
    {
        return res;
    }

    return 0; 
 
 
 }

adp_result_t RecPlayGetRecordNbPortAndWdt( adp_handle_recplay hRecPlay,
                                           unsigned int* pNbPorts,
                                           unsigned int* pPortWidth )
{
     adp_result_t res;


    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_NBRECPORT_INDEX,
                      pNbPorts );
    if(res)
    {
        return res;
    }

    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_RECPORTWIDTH_INDEX,
                      pPortWidth );
    if(res)
    {
        return res;
    }

    return 0;

 
}

adp_result_t RecPlayGetPlaybackNbPortAndWdt( adp_handle_recplay hRecPlay,
                                             unsigned int* pNbPorts,
                                             unsigned int* pPortWidth )
{
     adp_result_t res;

    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_NBPLAYPORT_INDEX,
                      pNbPorts );
    if(res)
    {
        return res;
    }

    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_PLAYPORTWIDTH_INDEX,
                      pPortWidth );
    if(res)
    {
        return res;
    }

    return 0;

 
}

adp_result_t RecordSetTriggerSource( adp_handle_recplay hRecPlay,
                                      eRecPlayTrigSrc eSrc )
{
    adp_result_t res;
    unsigned int uChoice;

    uChoice = (unsigned int)eSrc;

    res =  memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_RECTRIGMUX_INDEX,
                       uChoice );

    return res;

}



adp_result_t RecordSoftTrig( adp_handle_recplay hRecPlay )
{

     adp_result_t res;
    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_RECSOFTTRIG_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_RECSOFTTRIG_INDEX,
                       0 );
    if(res)
    {
        return res;
    }
    return 0;
}

adp_result_t PlaybackSetTriggerSource( adp_handle_recplay hRecPlay,
                                      eRecPlayTrigSrc eSrc )
{
    adp_result_t res;
    unsigned int uChoice;

    uChoice = (unsigned int)eSrc;
  
    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_PLAYTRIGMUX_INDEX,
                       uChoice );
    return res;
}



adp_result_t PlaybackSoftTrig( adp_handle_recplay hRecPlay )
{

     adp_result_t res;
    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_PLAYSOFTTRIG_INDEX,
                       1 );
    if(res)
    {
        return res;
    }

    res = memory_write_send( hRecPlay -> state,
                       (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_PLAYSOFTTRIG_INDEX,
                       0 );
    if(res)
    {
        return res;
    }
    return 0;
}


adp_result_t Recplay_GetPlaybackCount( adp_handle_recplay hRecPlay, unsigned int *uCount)
{
    adp_result_t res;
    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_PLAYBACKREADCNT_INDEX,
                      uCount );
    if(res)
    {
        return res;
    }
    *uCount =  *uCount * 8;
    return 0;
}

adp_result_t Recplay_GetRecordCount( adp_handle_recplay hRecPlay, unsigned int *uCount)
{
    adp_result_t res;
    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_RECORDSTORAGECNT_INDEX,
                      uCount );
	
    if(res)
    {
        return res;
    }
    *uCount =  *uCount * 8;
    return 0;

}

adp_result_t Recplay_GetHostToMemCount( adp_handle_recplay hRecPlay, unsigned int *uCount)
{
    adp_result_t res;
    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_RTDEXSTORAGECNT_INDEX,
                      uCount );

    if(res)
    {
        return res;
    }
    *uCount =  *uCount * 8;
    return 0;
}

adp_result_t Recplay_GetMemToHostCount( adp_handle_recplay hRecPlay, unsigned int *uCount)
{
    adp_result_t res;
    res = memory_read_send( hRecPlay -> state, 
                      (unsigned int)hRecPlay->u64RecPlayFpgaBaseAdd + RECPLAY_ACCESS_RTDEXREADCNT_INDEX,
                      uCount );
    if(res)
    {
        return res;
    }
    *uCount =  *uCount * 8;
    return 0;
}

