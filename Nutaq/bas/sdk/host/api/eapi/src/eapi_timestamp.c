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
 * @file      eapi_timestamp.c
 * 
 * @brief     Function definitions for the eapi timestamp control
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

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "bas_types.h"
#include "cce.h"
#include "eapi_timestamp.h"
#include "timestamp_defines.h"
#include "eapi_serialize.h"

#ifdef _MSC_VER
#include <Windows.h>
#define inline __inline

#else // Linux
#include <time.h>
#define Sleep(MS) usleep((MS) * 1000)

#endif

/****************************************************************************
 *                            Timestamp object                              *
 ****************************************************************************/
typedef struct sTimestamp_Obj
{
    // Chassis
    connection_state state;                     ///< Connection state (platform CCE)
    uint32_t u32ClkFreq;                      ///< Clock frequency of the timestamp core

} Timestamp_Obj_t;

/****************************************************************************
 *                            Private constants                             *
 ****************************************************************************/

// Define to activate debug traces
//#define TIMESTAMP_DBG

#ifdef TIMESTAMP_DBG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

#define TIMESTAMP_PPS_UNCERTAINTY_S  0.1
#define TIMESTAMP_PPS_UNCERTAINTY_NS ((unsigned)(TIMESTAMP_PPS_UNCERTAINTY_S * 1e9))

#define FILETIME_TICK_PER_SECOND 10000000
#define FILETIME_TICK_TO_NS 100
#define FILETIME_TO_POSIX_TIME 11644473600LL


/****************************************************************************
 *                       Private function prototypes                        *
 ****************************************************************************/

// Windows time conversion functions
#ifdef _MSC_VER
static inline void Timestamp_PosixTimeToFileTime(
    uint32_t u32sec, uint32_t u32nsec, FILETIME * pft);
    
static inline void Timestamp_FileTimeToPosixTime(
    FILETIME ft, uint32_t * pu32sec, uint32_t * pu32nsec);
#endif


/****************************************************************************
 *                             Public functions                             *
 ****************************************************************************/

/****************************************************************************
 * Function: Timestamp_Open
 *****************************************************************************
 *
 * Open Timestamp instance
 *
 * @param [out]  phTimestamp
 *      Pointer to Timestamp handle.
 *
 * @param [in]  state
 *      Connection state between the host and the CCE.
 *
 * @param [in]  u32ClkFreq
 *      Frequency of the clock used.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_Open( Timestamp_Handle_t *phTimestamp,
                           connection_state state,
                           uint32_t u32ClkFreq)
{
    adp_result_t res;

    // Make sure the handle is NULL in case something goes wrong
    *phTimestamp = NULL;

    /*
     * Alloc and initialize the RTDEx instance structure
     */
    *phTimestamp = (Timestamp_Handle_t)malloc( sizeof(Timestamp_Obj_t) );
    if( *phTimestamp == NULL ) return TIMESTAMPOUTOFMEMORY;

    memset( *phTimestamp, 0, sizeof(Timestamp_Obj_t) );

    // Set connection state
    (*phTimestamp)->state = state;

    // Set parameters
    (*phTimestamp)->u32ClkFreq = u32ClkFreq;


    // Verify timestamp presence
    res = Timestamp_Presence_send(&state);
    if ( FAILURE(res) ) goto error;

    return TIMESTAMPOK;

error:

    Timestamp_Close(*phTimestamp);
    *phTimestamp = NULL;

    return res;
}


/****************************************************************************
 * Function: Timestamp_Close
 *****************************************************************************
 *
 * Close Timestamp instance
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @return
 *      None.
 *
 ****************************************************************************/
void Timestamp_Close( Timestamp_Handle_t hTimestamp )
{
    /*
     * Deallocate the resources
     */
    if ( hTimestamp != NULL )
    {
        free( hTimestamp );
    }

    return;
}

/****************************************************************************
 * Function: Timestamp_Reset
 *****************************************************************************
 *
 * Set the current device UTC time in POSIX format
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_Reset( Timestamp_Handle_t hTimestamp )
{
    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    return Timestamp_Reset_send(&hTimestamp->state);
}

/****************************************************************************
 * Function: Timestamp_GetClkFreq
 *****************************************************************************
 *
 * Get the current clock frequency of the timestamp.
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [out] pu32ClkFreq
 *      Pointer to return the clock frequency value on success
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetClkFreq( Timestamp_Handle_t hTimestamp, uint32_t * pu32ClkFreq )
{
    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    *pu32ClkFreq = hTimestamp->u32ClkFreq;

    if(hTimestamp->u32ClkFreq == 0)
        return TIMESTAMPINVFREQ;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_ApplyHostTimeToDevice
 *****************************************************************************
 *
 * Apply the current host time to the device.
 * It requires a valid PPS signal and a host time synchronized within 
 * ±100 ms compare to the PPS signal.
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_ApplyHostTimeToDevice( Timestamp_Handle_t hTimestamp )
{
    adp_result_t res;
    uint32_t u32sec, u32nsec;

    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    /*
     *  Set next second timestamp
     */
    while(1)
    {

        res = Timestamp_GetHostPosixTime( &u32sec, &u32nsec);
        if ( FAILURE(res) ) return res;

        // If enough margin around the second transition
        // break the set the current timestamp
        if(u32nsec > TIMESTAMP_PPS_UNCERTAINTY_NS &&
            u32nsec < (1000000000 - 2 * TIMESTAMP_PPS_UNCERTAINTY_NS))
        {
            break;
        }

        // If to close the a second transition
        // sleep and retry later
        Sleep(10);
    }

    // The timestamp will be initialized at the next PPS rising edge
    // the next second of the current time   
    res = Timestamp_SetDevicePosixTime( hTimestamp, (u32sec + 1), 0);
    if ( FAILURE(res) ) return res;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_VerifyTime
 *****************************************************************************
 *
 * This function should be called after Timestamp_ApplyHostTimeToDevice 
 * function call in order to verify that the host time has correctly been
 * set to the device.
 *
 * Verify if a PPS signal is present.
 * If yes, verify if the device time is within ±100 ms 
 * from the host computer time
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_VerifyTime( Timestamp_Handle_t hTimestamp )
{
    adp_result_t res;
    uint64_t u64TsTick;
    uint64_t u64HostTsTick;
    uint32_t u32sec, u32nsec;
    int32_t  i32TimeoutMs;
    int64_t  i64TickDiff;
    const int32_t i32LoopSleepTimeMs = 10;

    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    // Wait for the PPS pulse to happen
    // Return if timeout
    i32TimeoutMs = 1500;
    while(1)
    {
        /*
         * The current timestamp should be 0 when the PPS is not happened.
         * It should be the initial counter value + the number of clock cycles elapsed
         * since the PPS event after the PPS event has happened.
         */
        res = Timestamp_GetTime_send(&hTimestamp->state, &u64TsTick);
        if ( FAILURE(res) ) return res;


        if ( u64TsTick )
            break;

        if ( i32TimeoutMs <= 0)
        {
            DEBUG_PRINT("The timestamp has not been configured\n");
            DEBUG_PRINT("and more than 1 second has been elapsed.\n");
            DEBUG_PRINT("It seems the PPS pulse is not connected to the system.\n");
            res = TIMESTAMPPPSABSENT;
            return res;
        }
        Sleep(i32LoopSleepTimeMs);
        i32TimeoutMs -= i32LoopSleepTimeMs;
    }

    // Get the current host time in tick count
    res = Timestamp_GetHostPosixTime( &u32sec, &u32nsec);
    if ( FAILURE(res) ) return res;
        
    res = Timestamp_PosixTimeToTick( hTimestamp, u32sec, u32nsec, &u64HostTsTick);
    if ( FAILURE(res) ) return res;

    // Compare system and host time
    // Should be not be more different than a second
    i64TickDiff = (int64_t)(u64HostTsTick - u64TsTick);
    
    DEBUG_PRINT("%9f ms\n",(double)i64TickDiff/(double)hTimestamp->u32ClkFreq*1000.0);
    
    if ( fabs((double)i64TickDiff/(double)(int64_t)hTimestamp->u32ClkFreq) > TIMESTAMP_PPS_UNCERTAINTY_S )
    {
        DEBUG_PRINT("The system timestamp does not fit the host time\n");
        DEBUG_PRINT("  System : %016"PRI_X64".\n", u64TsTick);
        DEBUG_PRINT("  Host   : %016"PRI_X64".\n", u64HostTsTick);
        res = TIMESTAMPOUTOFSYNC;
        return res;
    }


    return TIMESTAMPOK;
}


/****************************************************************************
 * Function: Timestamp_GetDeviceTickTime
 *****************************************************************************
 *
 * Set the current device time in tick (clock cycle number)
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [in]  u64Tick
 *      Time in number of clock ticks
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_SetDeviceTickTime( Timestamp_Handle_t hTimestamp, uint64_t u64Tick)
{
    adp_result_t res;

    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    res = Timestamp_SetTime_send(&hTimestamp->state, u64Tick);
    if ( FAILURE(res) ) return res;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_SetDevicePosixTime
 *****************************************************************************
 *
 * Set the current device UTC time in POSIX format
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [in]  u32sec
 *      UTC POSIX time
 *
 * @param [in]  u32nsec
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_SetDevicePosixTime( Timestamp_Handle_t hTimestamp, uint32_t u32sec, uint32_t u32nsec)
{
    adp_result_t res;
    uint64_t u64TsTick;

    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    res = Timestamp_PosixTimeToTick(hTimestamp, u32sec, u32nsec, &u64TsTick);
    if ( FAILURE(res) ) return res;
        
    res = Timestamp_SetDeviceTickTime(hTimestamp, u64TsTick);
    if ( FAILURE(res) ) return res;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_SetDeviceReadableLocalTime
 *****************************************************************************
 *
 * Set the current device local time in a readable format.
 * 
 * ex.: 2014-11-20 10:22:06.123456789
 *  
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [in]  pcTime
 *      Char array to that express local time in a readable string format
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_SetDeviceReadableLocalTime( Timestamp_Handle_t hTimestamp, char * pcTime)
{
    adp_result_t res;
    uint32_t u32sec, u32nsec;

    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;
        
    res = Timestamp_ReadableLocalTimeToPosix(pcTime, &u32sec, &u32nsec);
    if ( FAILURE(res) ) return res;

    res = Timestamp_SetDevicePosixTime(hTimestamp, u32sec, u32nsec);
    if ( FAILURE(res) ) return res;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_GetHostPosixTime
 *****************************************************************************
 *
 * Get the current host computer UTC time in POSIX format
 *
 * @param [out]  pu32sec
 *      UTC POSIX time
 *
 * @param [out]  pu32nsec
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
#ifdef _MSC_VER
adp_result_t Timestamp_GetHostPosixTime( uint32_t * pu32sec, uint32_t * pu32nsec)
{
    adp_result_t res;
    SYSTEMTIME st;
    FILETIME ft;
    BOOL result;

    // Get time in ReadableLocal foramt
    GetSystemTime(&st);

    // Convert it to number of 100-nanosecond intervals since January 1, 1601
    result = SystemTimeToFileTime(&st, &ft);
    if(result == 0)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    Timestamp_FileTimeToPosixTime(ft, pu32sec, pu32nsec);

    return TIMESTAMPOK;
}

#else // Linux
adp_result_t Timestamp_GetHostPosixTime( uint32_t * pu32sec, uint32_t * pu32nsec)
{
    adp_result_t res;
    int result;
    struct timespec ts;

    result = clock_gettime(CLOCK_REALTIME, &ts);
    if( result != 0 )
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    if(pu32nsec)
    {
        *pu32nsec = (uint32_t)ts.tv_nsec;
    }

    if(pu32sec)
    {
        *pu32sec = (uint32_t)ts.tv_sec;
    }

    return TIMESTAMPOK;
}

#endif

/****************************************************************************
 * Function: Timestamp_GetHostReadableLocalTime
 *****************************************************************************
 *
 * Get the current host computer local time in a readable format.
 * 
 * ex.: 2014-11-20 10:22:06.123456789
 *  
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [out]  pcTime
 *      Output char array to store the readable time in a string format
 *
 * @param [in]  u32Length
 *      Maximal length of pcTime. The pcTime length must be at least 30.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetHostReadableLocalTime( char * pcTime, uint32_t u32Length )
{
    adp_result_t res;
    uint32_t u32sec, u32nsec;

    res = Timestamp_GetHostPosixTime( &u32sec, &u32nsec);
    if (FAILURE(res)) return res;

    res = Timestamp_PosixToReadableLocalTime( u32sec, u32nsec, pcTime, u32Length);
    if (FAILURE(res)) return res;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_GetDeviceTickTime
 *****************************************************************************
 *
 * Get the current device time in tick (clock cycle number)
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [out]  pu64Tick
 *      Pointer to a uint64_t variable.
 *      Time in number of clock ticks
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetDeviceTickTime( Timestamp_Handle_t hTimestamp, uint64_t * pu64Tick)
{
    adp_result_t res;

    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    res = Timestamp_GetTime_send(&hTimestamp->state, pu64Tick);
    if ( FAILURE(res) ) return res;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_GetDevicePosixTime
 *****************************************************************************
 *
 * Get the current device UTC time in POSIX format
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [out]  pu32sec
 *      Pointer to a uint32_t variable.
 *      UTC POSIX time
 *
 * @param [out]  pu32nsec
 *      Pointer to a uint32_t variable.
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetDevicePosixTime( Timestamp_Handle_t hTimestamp, uint32_t * pu32sec, uint32_t * pu32nsec)
{
    adp_result_t res;
    uint64_t u64TsTick;

    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;
        
    res = Timestamp_GetDeviceTickTime(hTimestamp, &u64TsTick);
    if ( FAILURE(res) ) return res;

    res = Timestamp_TickToPosixTime(hTimestamp, u64TsTick, pu32sec, pu32nsec);
    if ( FAILURE(res) ) return res;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_GetDeviceReadableLocalTime
 *****************************************************************************
 *
 * Get the current device local time in a readable format.
 * 
 * ex.: 2014-11-20 10:22:06.123456789
 *  
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [in]  hTimestamp
 *      A Timestamp handle.
 *
 * @param [out]  pcTime
 *      Output char array to store the readable time in a string format
 *
 * @param [in]  u32Length
 *      Maximal length of pcTime. The pcTime length must be at least 30.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t Timestamp_GetDeviceReadableLocalTime( Timestamp_Handle_t hTimestamp, char * pcTime, uint32_t u32Length )
{
    adp_result_t res;
    uint32_t u32sec, u32nsec;

    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    res = Timestamp_GetDevicePosixTime(hTimestamp, &u32sec, &u32nsec);
    if ( FAILURE(res) ) return res;

    res = Timestamp_PosixToReadableLocalTime( u32sec, u32nsec, pcTime, u32Length);
    if (FAILURE(res)) return res;

    return TIMESTAMPOK;
}

/****************************************************************************
 * Function: Timestamp_PosixToReadableLocalTime
 *****************************************************************************
 *
 * Convert a UTC POSIX time into a readable local time format
 * 
 * ex.: 2014-11-20 10:22:06.123456789
 *  
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * @param [in]  u32sec
 *      UTC POSIX time
 *
 * @param [in]  u32nsec
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @param [out]  pcTime
 *      Output char array to store the readable time in a string format
 *
 * @param [in]  u32Length
 *      Maximal length of pcTime. The pcTime length must be at least 30.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
#ifdef _MSC_VER
adp_result_t Timestamp_PosixToReadableLocalTime(uint32_t u32sec, uint32_t u32nsec, char *pcString, uint32_t u32Length)
{
    adp_result_t res;
    FILETIME ftutc, ftlocal;
    SYSTEMTIME st;
    BOOL result;
    int ret;

    // Verify buffer size
    if (u32Length < (strlen("2014-11-20 10:22:06.123456789") + 1))
    {
        res = TIMESTAMPINVARRAYLENGTH;
        return res;
    }

    Timestamp_PosixTimeToFileTime(u32sec, u32nsec, &ftutc);

    // Convert POSIX to local time
    result = FileTimeToLocalFileTime(&ftutc, &ftlocal);
    if(result == 0)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    result = FileTimeToSystemTime(&ftlocal, &st);
    if(result == 0)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    ret = sprintf(pcString, "%d-%02d-%02d %02d:%02d:%02d.%09u",
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond,
        u32nsec);
    if (ret == 0)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    return TIMESTAMPOK;
}

#else // Linux
adp_result_t Timestamp_PosixToReadableLocalTime(uint32_t u32sec, uint32_t u32nsec, char *pcString, uint32_t u32Length)
{
    adp_result_t res;
    int ret;
    struct tm t;
    struct tm * ptmTemp;
    time_t sec = (time_t)u32sec;

    // Verify buffer size
    if (u32Length < (strlen("2014-11-20 10:22:06.123456789") + 1))
    {
        res = TIMESTAMPINVARRAYLENGTH;
        return res;
    }

    tzset();
    ptmTemp = localtime_r(&sec, &t);
    if (ptmTemp == NULL)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    ret = sprintf(pcString, "%d-%02d-%02d %02d:%02d:%02d.%09u",
        t.tm_year+1900,
        t.tm_mon+1,
        t.tm_mday,
        t.tm_hour,
        t.tm_min,
        t.tm_sec,
        u32nsec);
    if (ret == 0)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    return TIMESTAMPOK;
}
#endif


/****************************************************************************
 * Function: Timestamp_ReadableLocalTimeToPosix
 *****************************************************************************
 *
 * Convert a readable local time format to UTC time in POSIX format
 * 
 * ex.: 2014-11-20 10:22:06.123456789
 *  
 * where the last 9 digit after the '.' represent an offset in nanosecond
 *
 * Careful!
 *  "2014-11-20 10:22:06.5" means 10:22:06 +5 ns (not +500 ms !!!)
 *  To express +500 ms add the remaining zeros to express the 
 *  value in nanosecond: "2014-11-20 10:22:06.500000000"
 *
 * @param [in]  pcTime
 *      Output char array to store the readable time in a string format
 *
 * @param [out]  pu32sec
 *      Pointer to a uint32_t variable.
 *      UTC POSIX time
 *
 * @param [out]  pu32nsec
 *      Pointer to a uint32_t variable.
 *      Offset in nanosecond from the UTC POSIX time
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
#ifdef _MSC_VER
adp_result_t Timestamp_ReadableLocalTimeToPosix(char *pcString, uint32_t * pu32sec, uint32_t * pu32nsec)
{
    adp_result_t res;
    int argNum;
    FILETIME ftutc, ftlocal;
    SYSTEMTIME st;
    BOOL result;

    st.wMilliseconds = 0;

    // Parse the time string
    argNum = sscanf(pcString,"%d-%d-%d %d:%d:%d.%u",
        &st.wYear,
        &st.wMonth,
        &st.wDay,
        &st.wHour,
        &st.wMinute,
        &st.wSecond,
        pu32nsec);
    if ( argNum == 6 )
    {
        *pu32nsec = 0;
    }
    else if ( argNum != 7 )
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    result = SystemTimeToFileTime(&st, &ftlocal);
    if(result == 0)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    result = LocalFileTimeToFileTime(&ftlocal, &ftutc);
    if(result == 0)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    Timestamp_FileTimeToPosixTime(ftutc, pu32sec, NULL);

    return TIMESTAMPOK;
}

#else // Linux
adp_result_t Timestamp_ReadableLocalTimeToPosix(char *pcString, uint32_t * pu32sec, uint32_t * pu32nsec)
{
    int result;
    int argNum;
    struct tm t;
    struct tm * ptmTemp;
    adp_result_t res;
    time_t sec;

    struct timespec cts;
    struct tm ctm;

    /*
     * Get to current time to now if currently in daylight saving time (dst)
     */
    result = clock_gettime(CLOCK_REALTIME, &cts);
    if( result != 0 )
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    tzset();
    ptmTemp = localtime_r(&cts.tv_sec, &ctm);
    if( ptmTemp == NULL )
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    // Parse the time string
    argNum = sscanf(pcString,"%d-%d-%d %d:%d:%d.%u",
        &t.tm_year,
        &t.tm_mon,
        &t.tm_mday,
        &t.tm_hour,
        &t.tm_min,
        &t.tm_sec,
        pu32nsec);
    if ( argNum == 6 )
    {
        *pu32nsec = 0;
    }
    else if ( argNum != 7 )
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    // Add offset from the tm structure definition
    t.tm_year -= 1900;
    t.tm_mon -= 1;

    // Apply current (dst) setting
    t.tm_isdst = ctm.tm_isdst;

    // Convert tm structure to POSIX time
    sec = mktime(&t);
    if(sec == -1)
    {
        res = TIMESTAMPINVTIME;
        return res;
    }

    *pu32sec = (uint32_t)sec;

    return TIMESTAMPOK;
}
#endif

/****************************************************************************
* Function: Timestamp_PosixTimeToTick
*****************************************************************************
*
* Convert a POSIX time + offset in nanosecond to Timestamp module clock tick
*
* @param [in]  hTimestamp
*      A Timestamp handle.
*
* @param [in]  u32sec
*      Pointer to a uint32_t variable.
*      UTC POSIX time
*
* @param [in]  u32nsec
*      Offset in nanosecond from the UTC POSIX time
*
* @param [out]  pu64Tick
*      Pointer to a uint64_t variable.
*      Timestamp tick
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t Timestamp_PosixTimeToTick(
    Timestamp_Handle_t hTimestamp,
    uint32_t u32sec, uint32_t u32nsec,
    uint64_t * pu64Tick)
{
    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    if(hTimestamp->u32ClkFreq == 0)
        return TIMESTAMPINVFREQ;

    *pu64Tick = ((uint64_t)u32sec * hTimestamp->u32ClkFreq)
        + ((uint64_t)u32nsec * (uint64_t)hTimestamp->u32ClkFreq / 1000000000);
        
    return TIMESTAMPOK;
}

/****************************************************************************
* Function: Timestamp_FileTimeToPosixTime
*****************************************************************************
*
* Convert Timestamp module clock tick to a POSIX time + offset in nanosecond
*
* @param [in]  hTimestamp
*      A Timestamp handle.
*
* @param [out]  u64Tick
*      Timestamp tick
*
* @param [out]  pu32sec
*      Pointer to a uint32_t variable.
*      UTC POSIX time
*
* @param [out] pu32nsec
*      Pointer to a uint32_t variable.
*      Offset in nanosecond from the UTC POSIX time
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t Timestamp_TickToPosixTime(
    Timestamp_Handle_t hTimestamp,
    uint64_t u64Tick,
    uint32_t * pu32sec, uint32_t * pu32nsec)
{
    if(hTimestamp == NULL)
        return TIMESTAMPINVHANDLE;

    if(hTimestamp->u32ClkFreq == 0)
        return TIMESTAMPINVFREQ;
        
    *pu32sec = (uint32_t)(u64Tick / (uint64_t)hTimestamp->u32ClkFreq);
    *pu32nsec = (uint32_t)(((u64Tick % (uint64_t)hTimestamp->u32ClkFreq) * 1000000000) / hTimestamp->u32ClkFreq);
        
    return TIMESTAMPOK;
}

/****************************************************************************
 *                            Private functions                             *
 ****************************************************************************/
 
#ifdef _MSC_VER
/****************************************************************************
* Function: Timestamp_PosixTimeToFileTime
*****************************************************************************
*
* Convert a POSIX time + offset in nanosecond to Windows FILETIME format
*
* @param [in]  u32sec
*      Pointer to a uint32_t variable.
*      UTC POSIX time
*
* @param [in]  u32nsec
*      Offset in nanosecond from the UTC POSIX time
*
* @param [out]  pft
*      Time expressed in Windows FILETIME format
*
* @return
*      NULL
*
****************************************************************************/
static inline void Timestamp_PosixTimeToFileTime(uint32_t u32sec, uint32_t u32nsec, FILETIME * pft)
{
    ULARGE_INTEGER uli;

    // 64-bit unsigned to FILETIME conversion
    uli.QuadPart = ((uint64_t)u32sec + FILETIME_TO_POSIX_TIME) * FILETIME_TICK_PER_SECOND
        + (uint64_t)u32nsec / FILETIME_TICK_TO_NS;

    pft->dwLowDateTime = uli.LowPart;
    pft->dwHighDateTime = uli.HighPart;
}

/****************************************************************************
* Function: Timestamp_FileTimeToPosixTime
*****************************************************************************
*
* Convert time in Windows FILETIME format to POSIX time + offset in nanosecond
*
* @param [out]  pft
*      Time expressed in Windows FILETIME format
*
* @param [out]  pu32sec
*      Pointer to a uint32_t variable.
*      UTC POSIX time
*
* @param [out] pu32nsec
*      Pointer to a uint32_t variable.
*      Offset in nanosecond from the UTC POSIX time
*
* @return
*      NULL
*
****************************************************************************/
static inline void Timestamp_FileTimeToPosixTime(FILETIME ft, uint32_t * pu32sec, uint32_t * pu32nsec)
{
    ULARGE_INTEGER uli;

    // Convert it to a 64 bit integer
    uli.LowPart = ft.dwLowDateTime;
    uli.HighPart = ft.dwHighDateTime;

    if(pu32nsec)
    {
        // Convert windows tick in nanosecond and only keep nanosecond under the seconds
        *pu32nsec = (uint32_t)((uli.QuadPart * FILETIME_TICK_TO_NS) % 1000000000);
    }

    if(pu32sec)
    {
        // Convert windows tick in seconds and translate it to POSIX time (1601 -> 1970)
        *pu32sec = (uint32_t)((uli.QuadPart / FILETIME_TICK_PER_SECOND) - FILETIME_TO_POSIX_TIME);
    }
}
#endif //_MSC_VER


/****************************************************************************
 *                          Raw EAPI functions                              *
 ****************************************************************************/
Result Timestamp_Presence_send(connection_state * state)
{
    cmd_send(state, TIMESTAMP_PRESENCE);

    return result_recv(state);
}

Result Timestamp_SetTime_send(connection_state * state, uint64_t u64Timestamp)
{
    cmd_send(state, TIMESTAMP_SETTIME);
    
    uint64_send(state, u64Timestamp);

    return result_recv(state);
}

Result Timestamp_GetTime_send(connection_state * state, uint64_t * pu64Timestamp)
{
    cmd_send(state, TIMESTAMP_GETTIME);
    
    *pu64Timestamp = uint64_recv(state);

    return result_recv(state);
}

Result Timestamp_Reset_send(connection_state * state)
{
    cmd_send(state, TIMESTAMP_RESET);

    return result_recv(state);
}