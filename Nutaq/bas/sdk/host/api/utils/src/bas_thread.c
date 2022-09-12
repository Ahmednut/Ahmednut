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
 * @file    bas_thread.c
 * @brief   Wrapper to use threads.
 *
 * $Date: 2016/01/13 19:36:16 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
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
#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__
#include <pthread.h>
#else
#include <windows.h>
#endif

#include "bas_types.h"
#include "bas_thread.h"

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

/****************************************************************************
 *                               Local types                                *
 ****************************************************************************/

/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                                Prototypes                                *
 ****************************************************************************/


/****************************************************************************
 *                             Public functions                             *
 ****************************************************************************/
 

/*
 * Thread
 */
int StartThread(adp_thread_t * pThread, void * startFunction, void * parameter, int priority)
{
#if defined(_WIN32) || defined(_WIN64)
    BOOL ret;
    
#else //linux   
    int err;
    pthread_attr_t thread_attr;
    struct sched_param thread_param;
    
#endif
    
    
    if(priority<0 || priority >= 7)
    {
        printf("Error: Invalid priority. Must be between 0 and 6\n");
        return -1;
    }
    
    
#if defined(_WIN32) || defined(_WIN64)
   *pThread = CreateThread( 
            NULL,                   // default security attributes
            0,                      // use default stack size  
            startFunction,          // thread function name
            parameter,              // argument to thread function 
            CREATE_SUSPENDED,       // does not run until the ResumeThread function is called
            NULL);                  // do not return the thread identifier 

    if(*pThread == NULL)
    {
        printf("Error: Cannot create thread\n");
        return -1;
    }

    // Set priority
    if( (priority%7) == 0 )
    {
        ret = SetThreadPriority(*pThread, THREAD_PRIORITY_TIME_CRITICAL);
    }
    else if( (priority%7) == 1 )
    {
        ret = SetThreadPriority(*pThread, THREAD_PRIORITY_HIGHEST);
    }
    else if( (priority%7) == 2 )
    {
        ret = SetThreadPriority(*pThread, THREAD_PRIORITY_ABOVE_NORMAL);
    }
    else if( (priority%7) == 3 )
    {
        ret = SetThreadPriority(*pThread, THREAD_PRIORITY_NORMAL);
    }
    else if( (priority%7) == 4 )
    {
        ret = SetThreadPriority(*pThread, THREAD_PRIORITY_BELOW_NORMAL);
    }
    else if( (priority%7) == 5 )
    {
        ret = SetThreadPriority(*pThread, THREAD_PRIORITY_LOWEST);
    }
    else // if( (priority%7) == 6 )
    {
        ret = SetThreadPriority(*pThread, THREAD_PRIORITY_IDLE);
    }

    if(ret == 0)
    {
        printf("Error: Cannot set thread priority\n");
        return -1;
    }
    
    // Start thread
    ret = ResumeThread(*pThread);
    if(ret == 0)
    {
        printf("Error: Cannot resume thread\n");
        return -1;
    }
    
#else //linux    
    pthread_attr_init( &thread_attr );
    pthread_attr_setschedpolicy( &thread_attr, SCHED_RR );
    thread_param.sched_priority = sched_get_priority_max( SCHED_RR ) - (priority * 8);
    pthread_attr_setschedparam( &thread_attr, &thread_param );
    pthread_attr_setinheritsched( &thread_attr, PTHREAD_EXPLICIT_SCHED );
    
    err = pthread_create( pThread, &thread_attr, startFunction, parameter );
    if ( err )
    {
        printf("Error: Cannot create thread\n");
        return -1;
    }

#endif

    return 0;
}



int JoinThread(adp_thread_t Thread)
{
#if defined(_WIN32) || defined(_WIN64)
    DWORD ret;
    
    ret = WaitForSingleObject(Thread,INFINITE);
    if(ret != WAIT_OBJECT_0)
    {
        printf("Error: Cannot wait for the thread\n");
        return -1;
    }
    
    ret = CloseHandle(Thread);
    if(ret == 0)
    {
        printf("Error: Cannot close the thread\n");
        return -1;
    }
    
#else // 
    int err;

    err = pthread_join( Thread, NULL );
    if( err )
    {
        printf("Error: Cannot wait for the thread\n");
        return -1;
    }

#endif

    return 0;

}

/*
 * Mutex
 */
int InitMutex(adp_mutex_t * pMutex)
{
#if defined(_WIN32) || defined(_WIN64)
    *pMutex = CreateMutex(
        NULL,           // default security attributes
        FALSE,          // do not have the initial ownership
        NULL);          // unnamed mutex
    if(*pMutex == NULL)
    {
        printf("Error: Cannot initialize the mutex\n");
        return -1;
    }
    
#else // linux
    int err;
    
    err = pthread_mutex_init( pMutex, NULL );
    if( err )
    {
        printf("Error: Cannot initialize the mutex\n");
        return -1;
    }

#endif

    return 0;

}

int DestroyMutex(adp_mutex_t * pMutex)
{
#if defined(_WIN32) || defined(_WIN64)
    BOOL ret;

    ret = CloseHandle(*pMutex);
    if(ret == 0)
    {
        //printf("Error: Cannot destroy the mutex\n");
        return -1;
    }
    
#else // linux
    int err;
    
    err = pthread_mutex_destroy( pMutex );
    if( err )
    {
        //printf("Error: Cannot destroy the mutex\n");
        return -1;
    }
#endif

    return 0;

}

int LockMutex(adp_mutex_t * pMutex)
{
#if defined(_WIN32) || defined(_WIN64)
    DWORD dwWaitResult; 

    dwWaitResult = WaitForSingleObject( 
        *pMutex,
        INFINITE);      
    
    if(dwWaitResult != WAIT_OBJECT_0)
    {
        printf("Error: Cannot lock the mutex\n");
        return -1;
    }
    
#else // linux
    int err;
    
    err = pthread_mutex_lock( pMutex );
    if( err )
    {
        printf("Error: Cannot lock the mutex\n");
        return -1;
    }


#endif

    return 0;

}

int UnlockMutex(adp_mutex_t * pMutex)
{
#if defined(_WIN32) || defined(_WIN64)
    BOOL ret;

    ret = ReleaseMutex(*pMutex);
    if(ret == 0)
    {
        printf("Error: Cannot unlock the mutex\n");
        return -1;
    }
    
#else // linux
    int err;
    
    err = pthread_mutex_unlock( pMutex );
    if( err )
    {
        printf("Error: Cannot unlock the mutex\n");
        return -1;
    }

#endif

    return 0;

}




