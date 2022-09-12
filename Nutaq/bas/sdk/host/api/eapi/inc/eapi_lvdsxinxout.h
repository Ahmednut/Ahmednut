
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
 *  @file       : eapi_fmclvds.h
 *  @brief      : Functions controling the fmc lvds from the host
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCLVDS
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

#ifndef __EAPI_FMC_LVDS__
#define __EAPI_FMC_LVDS__

#ifdef __cplusplus
extern "C" {
#endif

#include "connection_state.h"
#include "bas_error.h"
#include "fmclvds_defines.h"
#include "fmclvds_pcf8574_defines.h"

  
/** @defgroup FMCLVDS FMCLVDS EAPI
 *
 * Nutaq's LVDS-xIn-xOut mezzanine card (FMC) is 32/64 LVDS channels I/O card 
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
 
 
/***************************************************************************
* Function: fmclvds_reset_send
************************************************************************//**
*
* Resets the FMCLVDS
*
* @param[in] state Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_reset_send(connection_state * state);


/***************************************************************************
* Function: fmclvds_powerup_send
************************************************************************//**
*
* Power up the FMCLVDS
*
* @param[in] state Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_powerup_send(connection_state * state);

/***************************************************************************
* Function: fmclvds_presence
************************************************************************//**
*
* Verifies the FMC LVDS presence of the selected instance
*
* @param[in] state Handle to the connection
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_presence_send(connection_state * state);

/***************************************************************************
* Function: fmclvds_select_send
************************************************************************//**
*
* Selects the current FMCLVDS. 
*
* @param[in] state Handle to the connection
*
* @param[in] nb 0 for bottom LVDS, 1 for top
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_select_send(connection_state * state, unsigned nb);

/***************************************************************************
* Function: fmclvds_set_channel_dir_send
************************************************************************//**
*
* Sets the channel direction for the 16 pin group. 
*
* @param[in] state Handle to the connection
*
* @param[in] group FMCLVDS_GROUP_0 for pins 0-15, FMCLVDS_GROUP_1 for pins 16-31
*
* @param[in] dir Direction to be set  FMCLVDS_OUTPUT or FMCLVDS_INPUT
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_set_channel_dir_send(connection_state * state, FMCLVDS_GROUPS group, FMCLVDS_DIR dir);

/***************************************************************************
* Function: fmclvds_set_channel_powerdown_send
************************************************************************//**
*
* Sets the powerdown mode for a group 
*
* @param[in] state Handle to the connection
*
* @param[in] group FMCLVDS_GROUP_0 for pins 0-15, FMCLVDS_GROUP_1 for pins 16-31
*
* @param[in] pwdn Powerdown mode   FMCLVDS_PCF8574_POWERDOWN or FMCLVDS_PCF8574_POWERUP
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_set_channel_powerdown_send(connection_state * state, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_pwdn pwdn);

/***************************************************************************
* Function: fmclvds_select_send
************************************************************************//**
*
* Sets the preemphasis mode for a group 
*
* @param[in] state Handle to the connection
*
* @param[in] group FMCLVDS_GROUP_0 for pins 0-15, FMCLVDS_GROUP_1 for pins 16-31
*
* @param[in] preemp Preemphasis mode  FMCLVDS_PCF8574_PREEMPHASISON or FMCLVDS_PCF8574_PREEMPHASISOFF
*
* @return
*      return 0 on success.
*
***************************************************************************/
Result fmclvds_set_channel_preemphasis_send(connection_state * state, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_preemp preemp);

#ifdef __cplusplus
}
#endif

#endif 

