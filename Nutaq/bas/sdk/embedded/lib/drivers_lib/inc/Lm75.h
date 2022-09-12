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
 * @file       Lm75.h
 * @brief      Temperature monitoring function definitions
 *
 * $Date: 2015/10/05 15:51:27 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.         
 *
 * @ingroup DRIVERS     
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

#ifndef LM75_H__
#define LM75_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <stdlib.h>
#include "bas_error.h"
#include "DevCom.h"
#include "Lm75_regs.h"
#include "Lm75_defines.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

 /****************************************************************************
 *   NAME : Lm75_Open
 ************************************************************************//**
 *
 * Open the lm75 monitoring module
 *
 * @warning
 * - This function must be the first one used before using this module.
 * - The DevCom module must be opened before using this module.
 * - When the module is not used anymore, it can be closed with Lm75_Close().
 *
 * @param [out] pCore
 *    This structure will hold necessary information for the module to be used
 *
 * @param [in] pCom
 *    This structure must hold communication module information
 *
 * @return   Error code: eLm75Ok if no problem is found, !eLm75Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occurred. 
 *   
 * @see
 *    Lm75_Close()
 *
 * @ingroup LM75
 *
 ***************************************************************************/ 
Result Lm75_Open(Lm75_stCore * pCore, DevCom_hCom hCom, uint32_t u32DeviceId);


 /****************************************************************************
 *   NAME : Lm75_Reset
 ************************************************************************//**
 *
 * Reset the lm75 monitoring module to default non operating condition.
 *
 * @warning
 * - This function must be called at least once to reset this module to default
 *   non operation.
 * - The Lm75_Open() module must be opened before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Lm75_Open()
 *
 * @return   Error code: eLm75Ok if no problem is found, !eLm75Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occurred. 
 *   
 * @pre
 *    Lm75_Open()
 *
 * @ingroup LM75
 *
 ***************************************************************************/ 
Result Lm75_Reset(Lm75_stCore * pCore);


 /****************************************************************************
 *   NAME : Lm75_Init
 ************************************************************************//**
 *
 * Initialize the lm75 monitoring module to operating condition.
 *
 * @warning
 * - This function must be called at least once to initialize this module
 *   for operation.
 * - The Lm75_Reset() module must be called before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Lm75_Open()
 *
 * @return   Error code: eLm75Ok if no problem is found, !eLm75Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occurred. 
 *   
 * @pre
 *    Lm75_Open(), Lm75_Reset() 
 *
 * @ingroup LM75
 *
 ***************************************************************************/ 
Result Lm75_Init(Lm75_stCore * pCore);


 /****************************************************************************
 *   NAME : Lm75_Close
 ************************************************************************//**
 *
 * Close the lm75 monitoring module
 *
 * @warning
 * - This function must be the last one used after using this module.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Lm75_Open()
 *
 * @return   Error code: eLm75Ok if no problem is found, !eLm75Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occurred. 
 * 
 * @pre
 *    Lm75_Open() 
 *
 * @ingroup LM75
 *
 ***************************************************************************/ 
Result Lm75_Close(Lm75_stCore * pCore);


 /****************************************************************************
 *   NAME : Lm75_ReadTemp10X
 ************************************************************************//**
 *
 * Read the lm75 temperature with a resolution of 0.1 Celsius.
 * The temperature returned is of integer type and multiplied X10 to
 * allow keeping 0.1C resolution without using floating point.  
 *
 * @warning
 * - The module must be opened with Lm75_Open() before using this function.
 * - The Lm75_Reset() module must be called before using this function.
 * - The Lm75_Init() module must be called before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Lm75_Open()
 *
 * @param [out] psTemp10x
 *    Return the temperature read (0.1C resolution) X 10
 *
 * @return   Error code: eLm75Ok if no problem is found, !eLm75Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occurred. 
 *   
 * @pre
 *    Lm75_Open(), Lm75_Reset(), Lm75_Init()  
 *
 * @ingroup LM75
 *
 ***************************************************************************/ 
Result Lm75_ReadTemp10X(Lm75_stCore * pCore, short* psTemp10x);

                             
#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

