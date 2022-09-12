
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
*  @file       : eapi_lvds.h
*  @brief      : Functions controling the LVDS from the host
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup LVDS
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

#ifndef __EAPI_LVDS__
#define __EAPI_LVDS__

#ifdef __cplusplus
extern "C" {
#endif

#include "connection_state.h"
#include "bas_error.h"

  
/** @defgroup LVDS LVDS EAPI
 *
 * TODO
 * 
 * @file
 *     
 */

/***************************************************************************
* Function: lvds_presence_send
************************************************************************//**
*
* Verifies the LVDS presence
*
* @param[in] state Handle to the connection
*
* @param[in] group The address space group number of the LVDS core
*
* @return
*      return LVDSOK if a core is present, otherwise LVDSABSENT
*
***************************************************************************/
Result lvds_presence_send(connection_state * state, unsigned int group);

/***************************************************************************
* Function: lvds_getmode_send
************************************************************************//**
*
* Get the LVDS mode. 
*
* @param[in] state Handle to the connection
*
* @param[in] group The address space group number of the LVDS core
*
* @return
*      return LVDS_NONE, LVDS_GPIO or LVDS_SYNC
*
***************************************************************************/
Result lvds_getmode_send(connection_state * state, unsigned int group);

/***************************************************************************
* Function: lvds_setoutputenable_send
************************************************************************//**
*
* Sets the output enable value to the LVDS core.
* The LVDS lanes are tri-state buffers and their output can be enabled
* with this function.
*
* @param[in] state Handle to the connection
*
* @param[in] group The address space group number of the LVDS core
* 
* @param[in] outputenable In LVDS_GPIO mode: Output enable bitmask for each LVDS lane
*           In LVDS_SYNC mode: 1 = global output enable for all LVDS lanes, 0 = disable output for all LVDS lanes
*
* @return
*      return LVDSOK on success.
*
***************************************************************************/
Result lvds_setoutputenable_send(connection_state * state, unsigned int group, unsigned int outputenable);

/***************************************************************************
* Function: lvds_setvalue_send
************************************************************************//**
*
* This function is only valid in LVDS_GPIO mode.
* Sets the output value of the LVDS GPIO.
* A logical OR function is applied on this value and on the FPGA input data port value
* to determined to real LVDS output value.
* If you only want to control the GPIO value with this function, make sure the FPGA
* input data port is set to 0.
*
* @param[in] state Handle to the connection
*
* @param[in] group The address space group number of the LVDS core
* 
* @param[in] value Each bit of value controls the corresponding lane output if the lane has its output enabled
*
* @return
*      return LVDSOK on success.
*
***************************************************************************/
Result lvds_setvalue_send(connection_state * state, unsigned int group, unsigned int value);

/***************************************************************************
* Function: lvds_getvalue_send
************************************************************************//**
*
* This function is only valid in LVDS_GPIO mode.
* Gets the input value of the LVDS GPIO.
* Return the value of the input lanes as well as the value of the output lanes
*
* @param[in] state Handle to the connection
*
* @param[in] group The address space group number of the LVDS core
* 
* @param[in] value Each bit of value represent a lane value
*
* @return
*      return LVDSOK on success.
*
***************************************************************************/
Result lvds_getvalue_send(connection_state * state, unsigned int group, unsigned int *value);

/***************************************************************************
* Function: lvds_reset_send
************************************************************************//**
*
* Resets the LVDS core
*
* @param[in] state Handle to the connection
*
* @param[in] group The address space group number of the LVDS core
*
* @return
*      return LVDSOK on success.
*
***************************************************************************/
Result lvds_reset_send(connection_state * state, unsigned int group);

/***************************************************************************
* Function: lvds_reset_send
************************************************************************//**
*
* This function is only valid in LVDS_SYNC mode.
* Resets the LVDS FIFO that buffer input and output data in synchronous mode.
*
* @param[in] state Handle to the connection
*
* @param[in] group The address space group number of the LVDS core
*
* @return
*      return LVDSOK on success.
*
***************************************************************************/
Result lvds_reset_fifo_send(connection_state * state, unsigned int group);

#ifdef __cplusplus
}
#endif

#endif 

