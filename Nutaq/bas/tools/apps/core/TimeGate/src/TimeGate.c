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
 * @file      TimeGate.c
 * 
 * @brief     Function for initializing the timestamp module or using some of its features
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "eapi_carrier.h"
#include "bas_types.h"
#include "media_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "mac_utils.h" 
#include "media_utils.h" 
#include "linux_utils.h"
#include "terminal_utils.h"
#include "bas_error.h"
#include "bas_time.h"
#include "TimeGate.h"
#include "eapi_timestamp.h"
#include "CustomRegister_Write.h"

#include "cce.h"
#include "eapi_serialize.h"
#include <memory.h>
#include "Radio640.h"
#include "Radio640_defines.h"
#include "Radio640Serialize.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_internal_def.h"
#include "CustomRegister_Write.h"


/************************************************************************************************************************
											   Conversion  function string to 64 bit 
************************************************************************************************************************/

long long nu_atoll(char *instr)
{
long long retval;
  
  retval = 0;
  for (; *instr; instr++) {
    retval = 10*retval + (*instr - '0' );
  }
  return retval;
}



/************************************************************************************************************************
											   Timestamp Init 
************************************************************************************************************************/
Result TimeGate(char *pcIpAddress, handle_timestampparams hParams,char * Time,uint32_t mode)
{
	adp_result_t res;	
   
    

    connection_state state;
    Timestamp_Handle_t hTimestamp;
    Radio640_hRadio640 hRadio640 = NULL;

    uint32_t u32TimeLsw, u32TimeMsw;
	uint32_t sampling;
    const int32_t i32LoopSleepTimeMs = 10;
	uint32_t u32secGate, u32nsecGate;
    int32_t i32NanoSecRemainder = 0;
    uint64_t tick_number;
	uint64_t tick__Device_number;
	uint64_t relative_posix,relative_hour,relative_min,relative_sec;



	// Connection to system and allocation of all handler

	res = connect_cce(pcIpAddress,&state);
	if ( FAILURE(res) ) goto error;

    res = Radio640_Connect(&hRadio640, &state, 1);
	if ( FAILURE(res) ) goto error;

    res = Radio640_GetSamplingFreq(hRadio640,&sampling);
	if ( FAILURE(res) ) goto error;

	res = Timestamp_Open(&hTimestamp,state,sampling);
	if ( FAILURE(res) ) goto error;




	if(mode == eRELATIVE)                          //Relative mode condition
	{
    
		tick_number = nu_atoll(Time);                //convertion of the tick number string to 64 bit data 
		relative_posix = tick_number/sampling;    //Get total time to wait in second
		relative_hour = relative_posix / 3600;    //Get number of hour to wait
		relative_min = (relative_posix % 3600)/60;//Get number of minute to wait
		relative_sec = (relative_posix % 3600)%60;//Get number of second to wait
		printf("Record will be execute in %02llu:%02llu:%02llu\n",(unsigned long long)relative_hour,(unsigned long long)relative_min,(unsigned long long)relative_sec ); // print time left before the record

	}
	else if(mode == eABSOLUTE) //Absolute condition
	{
	  res = Timestamp_ReadableLocalTimeToPosix(Time, &u32secGate, &u32nsecGate); // Convert the date string into second 
      if ( FAILURE(res) ) goto error;

	  Timestamp_GetDeviceTickTime( hTimestamp, &tick__Device_number);            //Get the device time
	  tick_number = (uint64_t)sampling * (uint64_t)u32secGate;                  // Convert the number of second into tick number
	  if(tick_number < tick__Device_number)                                     // Test if the Record is in the past
	  {
	    printf("Time to write in the Time Gate is in the past\n");
		return -1;
	  }
	}
	else
	{
	  printf("Wrong time format\n");
	}

    u32TimeLsw =  tick_number & 0x00000000FFFFFFFF;                           // Write the tick count into register
    u32TimeMsw =  (tick_number & 0xFFFFFFFF00000000) >> 32;
    printf("Writing %s in the TimeGate (%llu tick)\n",Time,(unsigned long long)tick_number);

    CustomRegister_Write(&state,5,u32TimeLsw);
	CustomRegister_Write(&state,6,u32TimeMsw);


	/************************************************************************************************************************
												Closing connection to Perseus
	************************************************************************************************************************/
	
    disconnect_cce(&state);

error:
    if ( FAILURE(res) )
    {
        fprintf( stderr, "%s\n\n\n", ErrorToString(res) );
    }

    return res;
}


