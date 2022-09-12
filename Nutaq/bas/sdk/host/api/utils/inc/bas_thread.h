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
 * @file    bas_thread.h
 * @brief   Wrapper to use threads. 
 *
 * $Date: 2015/08/10 17:10:41 $
 * $Revision: 1.1 $
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
#ifndef ADP_THREAD_H__
#define ADP_THREAD_H__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#ifdef __linux__
#include <pthread.h>
#include <bas_types.h>

#else
#include <windows.h>
#include "bas_types.h"

#endif


/****************************************************************************
 *                               Public types                               *
 ****************************************************************************/

#ifdef __linux__
typedef pthread_t adp_thread_t;
typedef pthread_mutex_t adp_mutex_t;
#define adp_thread_dec_t void
#define adp_thread_ret_t void

#else //Windows
typedef HANDLE adp_thread_t;
typedef HANDLE adp_mutex_t;
#define adp_thread_dec_t DWORD WINAPI
#define adp_thread_ret_t DWORD

#endif

/***************************************************************************
* Function: StartThread
************************************************************************//**
*
* Create and start a thread
*
* @param[in,out] pThread
*   Pointer to an unused adp_thread_t object.
*   The thread started will be return through this pointer.
*
* @param[in] startFunction
*   Function that will be launched by the new thread
*
* @param[in] parameter
*   Pointer to the parameters that need to be passed to the start function.
*
* @param[in] priority
*   Thread priority. Must be between 0 and 6 included.
*       0: highest priority
*       6: lowest priority
*
*   In Windows, the thread priority is relative to the process class.
*   You can change the process class to real-time in order to maximize the thread priority
*       ex.: SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
*
*   In Linux, a priority of 0 already maximize the thread priority.
*
* @return
*      return 0 on success.
*
***************************************************************************/
int StartThread(adp_thread_t * pThread, void * startFunction, void * parameter, int priority);

/***************************************************************************
* Function: JoinThread
************************************************************************//**
*
* Wait for the thread to end and desallocate the thread ressources
*
* @param[in] Thread
*   Object returned by the StartThread function.
*
* @return
*      return 0 on success.
*
***************************************************************************/
int JoinThread(adp_thread_t Thread);


/***************************************************************************
* Function: InitMutex
************************************************************************//**
*
* Initialized a mutex object. After the initialization, the mutex is not 
* owned.
*
* @param[in,out] pMutex
*   Pointer to an unused mutex object.
*   The initialized mutex will be return through this pointer.
*
* @return
*      return 0 on success.
*
***************************************************************************/
int InitMutex(adp_mutex_t * pMutex);

/***************************************************************************
* Function: DestroyMutex
************************************************************************//**
*
* Destroy a mutex object.
*
* @param[in,out] pMutex
*   Pointer to an initialized mutex.
*
* @return
*      return 0 on success.
*
***************************************************************************/
int DestroyMutex(adp_mutex_t * pMutex);

/***************************************************************************
* Function: LockMutex
************************************************************************//**
*
* Request the ownership of the mutex object.
* If the mutex is already owned, the function will wait forever until
* the mutex is released and owned by the current process.
*
* @param[in,out] pMutex
*   Pointer to an initialized mutex.
*
* @return
*      return 0 on success.
*
***************************************************************************/
int LockMutex(adp_mutex_t * pMutex);

/***************************************************************************
* Function: UnlockMutex
************************************************************************//**
*
* Release the ownership of the mutex object owned by the current process.
*
* @param[in,out] pMutex
*   Pointer to an initialized and owned mutex.
*
* @return
*      return 0 on success.
*
***************************************************************************/
int UnlockMutex(adp_mutex_t * pMutex);

#ifdef __cplusplus
}
#endif
#endif    // ADP_THREAD_H__

