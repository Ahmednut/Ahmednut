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
*  @file       : lvds.h
*  @brief      : LVDS core register and functions definitions 
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup LVDS
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
 
 /** @defgroup LVDS LVDS Module
 *
 * LVDS is a library to control the LVDS FPGA core. The LVDS core has been 
 * designed to be generic and can be mapped on any FPGA pins. 
 *
 * It can provide a GPIO interface over the LVDS pairs.
 *
 * It can also provide a synchronous data transmission bus using a clock and
 * data valid LVDS pairs in addition to the LVDS pairs used for the data
 * transmission.
 * 
 * @file
 *     
 */

#ifndef LVDS_HEADER__
#define LVDS_HEADER__

#include "lvds_defines.h"
#include "lvds_registers.h"
#include "bas_error.h"

/***************************************************************************
* Function: lvds_open
************************************************************************//**
*
* Create handle to LVDS core.
*
* @param[out] board pointer to LVDS struct
*
* @param[in] core pointer to FPGA LVDS core.
*
* @param[in]   pfUsleep
*      Pointer to a sleep function that sleep during N microsecond.
*      This function is the one that will be used inside the library functions.
*
* @return
*      return LVDSOK on success.
*
***************************************************************************/
Result lvds_open(lvds * board, lvds_core * core, int (*pfUsleep)(unsigned int usec));

/***************************************************************************
* Function: lvds_presence
************************************************************************//**
*
* Verifies the LVDS presence
*
* @param[in] board pointer to bottom lvds struct
*
* @return
*      return LVDSOK on success.
*
***************************************************************************/
Result lvds_presence(lvds * board);

/***************************************************************************
* Function: lvds_getmode
************************************************************************//**
*
* Get the LVDS mode. 
*
* @param[in] board pointer to lvds struct
*
* @return
*      return LVDS_GPIO or LVDS_SYNC on success, return an error when no
*      LVDS core is present at the core address.
*
***************************************************************************/
Result lvds_getmode(lvds * board);

/***************************************************************************
* Function: lvds_setoutputenable
************************************************************************//**
*
* Sets the output enable value to the LVDS core.
* The LVDS lanes are tri-state buffers and their output can be enabled
* with this function.
*
* @param[in] board pointer to lvds struct
*
* @param[in] outputenable In LVDS_GPIO mode: Output enable bitmask for each LVDS lane
*           In LVDS_SYNC mode: 1 = global output enable for all LVDS lanes, 0 = disable output for all LVDS lanes
*      
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result lvds_setoutputenable(lvds * board, unsigned int outputenable);

/***************************************************************************
* Function: lvds_setvalue
************************************************************************//**
*
* This function is only valid in LVDS_GPIO mode.
* Sets the output value of the LVDS GPIO.
* A logical OR function is applied on this value and on the FPGA input data port value
* to determined to real LVDS output value.
* If you only want to control the GPIO value with this function, make sure the FPGA
* input data port is set to 0.
*
* @param[in] board pointer to lvds struct
*
* @param[in] value to be set
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result lvds_setvalue(lvds * board, unsigned int value);

/***************************************************************************
* Function: lvds_getvalue
************************************************************************//**
*
* This function is only valid in LVDS_GPIO mode.
* Gets the input value of the LVDS GPIO.
* Return the value of the input lanes as well as the value of the output lanes
*
* @param[in] board pointer to lvds struct
*
* @param[in] pointer to store the read value
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result lvds_getvalue(lvds * board, unsigned int *value);

/***************************************************************************
* Function: lvds_reset
************************************************************************//**
*
* Reset the LVDS core
*
* @param[in] board pointer to bottom lvds struct to be closed
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result lvds_reset(lvds * board);

/***************************************************************************
* Function: lvds_reset_fifo
************************************************************************//**
*
* This function is only valid in LVDS_SYNC mode.
* Resets the LVDS FIFO that buffer input and output data in synchronous mode.
*
* @param[in] board pointer to bottom lvds struct to be closed
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result lvds_reset_fifo(lvds * board);

/***************************************************************************
* Function: lvds_close
************************************************************************//**
*
* Closes the LVDS handle
*
* @param[in] board pointer to bottom lvds struct to be closed
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result lvds_close(lvds * board);

#endif 

