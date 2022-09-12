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
*  @file       : fmclvds.h
*  @brief      : FMCLVDS core register and functions definitions 
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCLVDS
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
 
 /** @defgroup FMCLVDS FMCLVDS Module
 *
 * Nutaq LVDS-xIn-xOut mezzanine card (FMC) is 32/64 LVDS channels I/O card 
 * designed around the Texas Instruments DS15BR401 4-channel LVDS buffer/ 
 * repeater to allow direct use of the highspeed LVDS pairs of a carrier  
 * FPGA to communicate with the outside world. The LVDS card has 32 LVDS  
 * pairs on LPC-FMC form factor or 64 LVDS pairs on Nutaq double-stack  
 * HPC-FMC form factor. The direction of these pairs is fixed by hardware  
 * and can be selected by groups of 16. The LVDS-xIn-xOut card comes in two  
 * different default standard configurations which are 16in-16out (LPC) or  
 * 32in-32out (HPC); other configurations available upon requests. 
 * 
 * @file
 *     
 */

#ifndef FMCLVDS_HEADER__
#define FMCLVDS_HEADER__

#include "fmclvds_defines.h"
#include "fmclvds_registers.h"
#include "fmclvds_com_wrapper_defines.h"
#include "bas_error.h"
#include "carrier_service_def.h"


/***************************************************************************
* Function: fmclvds_open
************************************************************************//**
*
* Create handle to FMCLVDS core.
*
* @param[in,out] fmclvds pointer to fmclvds struct
*
* @param[in,out] core pointer to fmclvds core
*
* @param[in] pos Bottom or top lvds 
*    (FMCLVDSBOTTOM = 0 for bottom,  FMCLVDSTOP = 1 for top) 
*
* @param [in] hCarrier 
*    Handle to a carrier object. It is the carrier that will handle the I2C
*    and sleep functions.
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_open(fmclvds * fmclvds, fmclvds_core * core, unsigned int pos, Carrier_handle_t hCarrier);

/***************************************************************************
* Function: fmclvds_checkcoreid
************************************************************************//**
*
* Verifies if the FMC LVDS core is detected for a board instance.
*
* @param[in] fmclvds
*     pointer to fmclvds struct
*
* @param[out] core_detect
*     verified core identification. Either FMCLVDS_COREDETECTED or FMCLVDS_CORENONE
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_checkcoreid(fmclvds *fmclvds, FMCLVDS_CORE_DETECT *core_detect);


/***************************************************************************
* Function: fmclvds_setdir
************************************************************************//**
*
* Sets the direction for a 16-pin group
*
* @param[in] fmclvds pointer to  fmclvds struct
*
* @param[in] group group to be set. FMCLVDS_GROUP_0 (pins 0-15) or FMCLVDS_GROUP_1 (pins 16-31)
*
* @param[in] dir Direction to be set.   FMCLVDS_OUTPUT or FMCLVDS_INPUT
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_setdir(fmclvds * fmclvds, FMCLVDS_GROUPS group, FMCLVDS_DIR dir);

/***************************************************************************
* Function: fmclvds_close
************************************************************************//**
*
* Closes the FMCLVDS handle
*
* @param[in] fmclvds pointer to bottom fmclvds struct to be closed
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_close(fmclvds * fmclvds);

#endif 

