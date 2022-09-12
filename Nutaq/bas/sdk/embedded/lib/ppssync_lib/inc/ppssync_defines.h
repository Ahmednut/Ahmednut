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
 * @file    ppssync_defines.h
 * @brief   Numerical values for the PPSSYNC core.
 *
 * $Date: 2015/11/16 18:38:52 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup PPSSYNC
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

#ifndef __PPSSYNC_DEFINES__
#define __PPSSYNC_DEFINES__
#include "bas_error.h"

#ifdef __cplusplus
extern "C" {
#endif
  
#define CTE_INTERVAL 2
#define CTE_MAX_INTERVAL 32
#define MAX_GOOD_CLOCK 10
#define MAX_BAD_CLOCK 50
#define TARGET_PPB 0.000000100f // 100 PPB
#define CTE_TOL 4
  
 /** @anchor PPSSYNC defines
 *  @name Core ID defines
 *    
 *  PPSSYNC core ID
 */
#define PERSEUS_PPS_SYNC_COREID		0xCC01
#define PERSEUS_PPS_SYNC_COREVER	0x0200

/** @name PPSSYNC error codes
 * 
 *  List all runtime related error codes of a PPSSYNC module.
 */
enum ppssync_error {
    PPSSYNCOK    = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_PPSSYNC, 0),                ///<  Success 
    PPSSYNCCOREABSENT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 1), 			///<  FPGA core is absent 
    PPSSYNCDACOUTOFRANGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 2), 			///<  DAC value is out of range
    PPSSYNCPPSABSENT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 3), 				///<  no PPS present
    PPSSYNCPPSBADFREQ = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 4), 			///<  bad input frequency 
    PPSSYNCPPSBADDACLIMIT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 5), 		///<  bad input DAC limit values
    PPSSYNCSTARTFAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 6), 			///<  failed to start PPS-SYNC
    PPSSYNCEEPROMFAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 7), 			///<  failed to access EEPROM
    PPSSYNCGETINFOFAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 8), 			///<  failed to get PPS-SYNC info
	PPSSYNCBUSERROR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 9), 				///<  bus error
	PPSSYNCLOCKEDFAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 10), 			///<  VCXO is not locked
	PPSSYNCNOTINIT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 11), 			    ///<  PPSSync_DACInit function must be called first
	PPSSYNCINVFUNCPTR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_PPSSYNC, 12), 			///<  Invalid function pointer
};

#ifdef __cplusplus
}
#endif
#endif

