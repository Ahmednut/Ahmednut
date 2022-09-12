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
 * @file    bas_semaphore.c
 * @brief   Wrapper to use semaphores.
 *
 * $Date: 2015/08/10 17:11:19 $
 * $Revision: 1.1 $
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
#include <semaphore.h>
#include <errno.h>

#else
#include <windows.h>
#endif

#include "bas_types.h"
#include "bas_semaphore.h"

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

#define MAX_SEM_COUNT 2147483647


int InitSemaphore(adp_semaphore_t * pSemaphore, uint32_t u32InitValue)
{

#if defined(_WIN32) || defined(_WIN64)
    *pSemaphore = CreateSemaphore(
        NULL,           // default security attributes
        u32InitValue,   // initial count
        MAX_SEM_COUNT,  // maximum count
        NULL);          // unnamed semaphore
    if(*pSemaphore == NULL)
    {
        printf("Error: Cannot initialize semaphore\n");
        return -1;
    }

#else // linux
    int err;
    
    err = sem_init(pSemaphore, 0, u32InitValue);
    if(err == -1)
    {
        printf("Error: Cannot initialize semaphore\n");
        return -1;
    }

#endif

    return 0;

}


int DestroySemaphore(adp_semaphore_t * pSemaphore)
{
#if defined(_WIN32) || defined(_WIN64)
    BOOL ret;

    ret = CloseHandle(*pSemaphore);
    if(ret == 0)
    {
        //printf("Error: Cannot destroy the semaphore\n");
        return -1;
    }

#else // linux
    int err;

    err = sem_destroy(pSemaphore);
    if( err )
    {
        //printf("Error: Cannot destroy the semaphore\n");
        return -1;
    }

#endif

    return 0;

}

int PostSemaphore(adp_semaphore_t * pSemaphore)
{
#if defined(_WIN32) || defined(_WIN64)
    BOOL ret;

    ret = ReleaseSemaphore(
        *pSemaphore,  // handle to semaphore
        1,            // increase count by one
        NULL);
    if(ret == 0)
    {
        printf("Error: Cannot post the semaphore\n");
        return -1;
    }

#else // linux
    int err;

    err = sem_post(pSemaphore);
    if( err == -1 )
    {
        printf("Error: Cannot post the semaphore\n");
        return -1;
    }

#endif

    return 0;
}

int WaitSemaphore(adp_semaphore_t * pSemaphore, int32_t bWait)
{
#if defined(_WIN32) || defined(_WIN64)
    DWORD dwWaitResult; 
    
    if(bWait)
    {
        dwWaitResult = WaitForSingleObject( 
            *pSemaphore,
            INFINITE);          
    }
    else
    {
        dwWaitResult = WaitForSingleObject( 
            *pSemaphore,   
            0);
    }
    
    if(dwWaitResult == WAIT_TIMEOUT)
    {
        return 0;
    }
    else if(dwWaitResult != WAIT_OBJECT_0)
    {
        printf("Error: Cannot wait for the semaphore\n");
        return -1;
    }
    else // WAIT_OBJECT_0
    {
        // return 1 to indicate success
        return 1;
    }

#else // linux
    int err;
    
    if (bWait) 
    {
    	err = sem_wait( pSemaphore );
	    if ( err < 0 )
		{
		    if ( errno != ETIMEDOUT )
            {
                printf("Error: Cannot wait for the semaphore\n");
                return -1;
            }
            else
            {
                return 0;
            }
		}
        else
        {
            return 1;
        }
	}
    else
    {
    	err = sem_trywait( pSemaphore );
	    if ( err < 0 )
		{
		    if ( errno != EAGAIN ) 
            {
                printf("Error: Cannot wait for the semaphore\n");
                return -1;
            }
            else
            {
                return 0;
            }
		}
        else
        {
            return 1;
        }
	}

#endif

}




