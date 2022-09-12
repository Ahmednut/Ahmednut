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
 * @file    bas_buffer.h
 * @brief   Very basic buffer queue management. 
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
#ifndef BUFFER_H__
#define BUFFER_H__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#include "bas_types.h"  // stdint.h 


/****************************************************************************
 *                               Public types                               *
 ****************************************************************************/

/**
 * Buffer queue
 */
typedef struct Buffer_QueueObj *Buffer_Queue_t;

/**
 * Buffer descriptor
 */
typedef struct Buffer_Desc
{
    int32_t i32Size;        // Quantity of data in the buffer in bytes
    uint8_t *pMem;          // Data buffer
} Buffer_Desc_t;


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
int Buffer_InitQueue( Buffer_Queue_t *pq, int num, int size );

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
void Buffer_DestroyQueue( Buffer_Queue_t q );

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
Buffer_Desc_t *Buffer_Get( Buffer_Queue_t q, int iWait );

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
int Buffer_Put( Buffer_Queue_t q, Buffer_Desc_t *pBufDesc );



#ifdef __cplusplus
}
#endif
#endif    // BUFFER_H__

