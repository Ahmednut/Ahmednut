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
 * @file    bas_buffer.c
 * @brief   Very basic buffer queue management. 
 *
 * $Date: 2015/08/10 17:11:19 $
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

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#include <stddef.h>
#include "bas_thread.h"
#include "bas_semaphore.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>


#include "bas_types.h"  // stdint.h 
#include "bas_buffer.h"



/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

/****************************************************************************
 *                               Local types                                *
 ****************************************************************************/
 
/**************************************************************************** 
 * Struct : Buffer_Obj_t
 *************************************************************************//*
 *      
 * Queue element definition.
 *
 ****************************************************************************/
typedef struct Buffer_Obj 
{
    struct Buffer_Obj *pPrev;
    struct Buffer_Obj *pNext;
    Buffer_Desc_t desc;
    uint8_t  memStart[1];
    // Memory follow !!
} Buffer_Obj_t;


/**************************************************************************** 
 * Struct : Queue_Obj_t 
 *************************************************************************//*
 *      
 * Queue object definition.
 *
 ****************************************************************************/
typedef struct Buffer_QueueObj 
{
    adp_mutex_t mutex; 
    adp_semaphore_t cnt;   
    Buffer_Obj_t    *pFirst;
    Buffer_Obj_t    *pLast;
} Buffer_QueueObj_t;


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                                Prototypes                                *
 ****************************************************************************/
static int Buffer_Queue( Buffer_Queue_t q, Buffer_Obj_t *pBO );
static Buffer_Obj_t *Buffer_Dequeue( Buffer_Queue_t q, int iWait );


/****************************************************************************
 *                             Public functions                             *
 ****************************************************************************/

/****************************************************************************
 * Function : Buffer_Init
 ************************************************************************//**
 *
 * Pre-allocated buffers and initialize the pool of buffer.  
 *    
 * @param q
 *      Queue to initialize
 *
 * @param num
 *      Number of buffer to preallocate.
 *
 * @param size
 *      Buffer size in bytes.
 *
 * @return
 *    Success(0) or failure(-1).
 *
 ****************************************************************************/
// unsigned char BO[100][16383]
int Buffer_InitQueue( Buffer_Queue_t *pq, int num, int size )
{
    int res;
    int32_t n;
    Buffer_Obj_t *pBO = NULL;

    // Alloc the memory for the queue itself
    *pq = (Buffer_Queue_t)malloc( sizeof(Buffer_QueueObj_t) );
    if ( *pq == NULL )
    {
        perror( "malloc:" );
        return -1;
    }

    // Init the queue itself
    res = InitSemaphore( &(*pq)->cnt, 0 );
    if ( res < 0 )
    {
        free( *pq );
        *pq = NULL;
        return res;
    }

    res = InitMutex( &(*pq)->mutex );
    if ( res < 0 )
    {
        DestroySemaphore( &(*pq)->cnt );
        free( *pq );
        *pq = NULL;
        return res;
    }
    (*pq)->pFirst = NULL;
    (*pq)->pLast  = NULL;
    
    // Alloc some buffer to put in the queue
    for ( n = 0; n < num; n++ )
    {
        // Alloc the buffer description and the buffer itself
        pBO = (Buffer_Obj_t *)malloc( sizeof(Buffer_Obj_t) + size - sizeof(uint8_t) );
        if ( pBO == NULL ) goto error;

        pBO->desc.i32Size = 0;
        pBO->desc.pMem = pBO->memStart;

        res = Buffer_Queue( *pq, pBO );
        pBO = NULL;
        if ( res < 0 ) goto error;
    }
    return 0;

error:
    Buffer_DestroyQueue( *pq );
    *pq = NULL;

    return -1;
}

/****************************************************************************
 * Function : Buffer_DestroyQueue
 ************************************************************************//**
 *
 * Free all resources used by the queue
 *    
 * @param q
 *      Queue 
 *
 * @return
 *    none.
 *
 ****************************************************************************/
void Buffer_DestroyQueue( Buffer_Queue_t q )
{
    Buffer_Obj_t *b = NULL;

    while ( (b = Buffer_Dequeue(q,0)) != NULL )
    {
        free( b );
    }

    DestroySemaphore( &q->cnt );
    DestroyMutex( &q->mutex );

    free( q );
}

/****************************************************************************
 * Function : Buffer_Get
 ************************************************************************//**
 *
 * Get a buffer from the queue. 
 *    
 * @param q
 *      Buffer queue 
 *
 * @param [in] iWait
 *    Wait for a buffer (1) if none are available or not (0).
 *
 * @return
 *    The address of an allocated buffer.
 *
 ****************************************************************************/
Buffer_Desc_t *Buffer_Get( Buffer_Queue_t q, int iWait )
{
    Buffer_Obj_t *pBO = Buffer_Dequeue( q, iWait );

    if ( pBO != NULL )
    {
        return &pBO->desc;
    }
    return NULL;
}


/****************************************************************************
 * Function : Buffer_Put
 ************************************************************************//**
 *
 * Put a buffer in the queue.
 *    
 * @param q
 *      Buffer queue 
 *
 * @param pBufDesc
 *      Address of a buffer descriptor.
 *
 * @return
 *    Success(0) or failure(-1).
 *
 ****************************************************************************/
int Buffer_Put( Buffer_Queue_t q, Buffer_Desc_t *pBufDesc )
{
    Buffer_Obj_t *pBO = (Buffer_Obj_t *)((uint64_t)pBufDesc->pMem - offsetof(Buffer_Obj_t, memStart));

    return Buffer_Queue( q, pBO );
}


/****************************************************************************
 *                             Private routines                             * 
 ****************************************************************************/

/****************************************************************************
 * Function : Buffer_Queue
 ************************************************************************//**
 *
 * Queue a new buffer in the buffer queue.
 *          
 * @param [in] q 
 *    Pointer of queue.
 *
 * @param [in] pBO
 *    Pointer to a buffer object
 * 
 * @return
 *    Success(0) or an error code.
 *
 ***************************************************************************/
static int Buffer_Queue( Buffer_Queue_t q, Buffer_Obj_t *pBO )
{
    int res;

    // Begin critical section
    res = LockMutex( &q->mutex );
    
    // Insert at the end of the double chained list
    pBO->pPrev = q->pLast;
    pBO->pNext = NULL;

    if ( q->pLast != NULL ) 
    {
        q->pLast->pNext = pBO;
    }

    q->pLast = pBO;
   
    if ( q->pFirst == NULL ) 
    {
        q->pFirst = pBO;
    }
    
    // Increment the queue counter
    res = PostSemaphore( &q->cnt );
    
    // End critical section
    UnlockMutex( &q->mutex );

    return 0;
}

/****************************************************************************
 * Function : Buffer_Dequeue
 ************************************************************************//**
 *
 * Remove a buffer from the queue and return a pointer to this dequeued buffer.
 *          
 * @param [in] q 
 *    Pointer of queue.
 *
 * @param [in] iWait
 *    Wait for a buffer if none are available (1) 
 *    Do not wait for a buffer if none are available (0) 
 * 
 * @return
 *    Address of the buffer (NULL if none was available)
 *   
 ***************************************************************************/
static Buffer_Obj_t *Buffer_Dequeue( Buffer_Queue_t q, int32_t bWait )
{
    int res;
    Buffer_Obj_t *pBO;

    // Decrement the queue counter
    res = WaitSemaphore( &q->cnt, bWait);
    if ( res <= 0 ) 
    {
        return NULL;
    }

    res = LockMutex( &q->mutex );

    // Remove a message from the beginning of the double chained list
    pBO = q->pFirst;
    q->pFirst = q->pFirst->pNext;

    if ( q->pFirst != NULL )
    {   
        q->pFirst->pPrev = NULL;       
    }

    if ( q->pFirst == NULL ) 
    {
        q->pLast = NULL;
    }
    
    pBO->pPrev = NULL;
    pBO->pNext = NULL;
    
    // End critical section
    UnlockMutex( &q->mutex );
    return pBO;
}

