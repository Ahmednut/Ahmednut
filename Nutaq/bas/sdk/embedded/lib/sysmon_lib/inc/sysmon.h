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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**  
 * 
*  @file       : sysmon.h
*  @brief      : System Monitor core register and functions definitions 
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup SYSMON
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
 
 /** @defgroup SYSMON SYSMON Module
 *
 * System Monitor library is used to access the internal temperature
 * and power supply values of the FPGA
 * 
 * @file
 *     
 */

#ifndef SYSMON_HEADER__
#define SYSMON_HEADER__

#include "bas_error.h"
#include "sysmon_def.h"

/***************************************************************************
* Function: Sysmon_Open
************************************************************************//**
*
* Create handle to System Monitor core.
*
* @param[in,out] hSysmon
*   Return the allocated System Monitor handle.
*
* @param[in] uiBaseAddress
*   Base address of the AXI System Monitor ADC core
*
* @return
*      return OK on success.
*
***************************************************************************/
Result Sysmon_Open(Sysmon_handle_t * hSysmon, unsigned int uiBaseAddress);

/***************************************************************************
* Function: Sysmon_Read
************************************************************************//**
*
* Read the information requested from System Monitor
*
* @param[in] hSysmon
*   Handle to the System Monitor previously opened.
*
* @param[in] type
*   Type of the System Monitor information that will be read.
*
* @param[out] pValue
*   Return the read value at this address on success.
*
* @return
*      return OK on success.
*
***************************************************************************/
Result Sysmon_Read(Sysmon_handle_t hSysmon, Sysmon_eType_t type, float * pValue);

/***************************************************************************
* Function: Sysmon_Close
************************************************************************//**
*
* Close the System Monitor handle.
*
* @param[in] hSysmon
*   Deallocate the System Monitor handle previously opened.
*
* @return
*      return OK on success.
*
***************************************************************************/
Result Sysmon_Close(Sysmon_handle_t hSysmon);


#endif 

