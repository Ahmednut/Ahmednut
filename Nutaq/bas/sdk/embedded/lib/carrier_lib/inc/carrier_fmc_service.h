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
 * @file       carrier_service.h
 * @brief      carrier generic library functions definitions.
 *
 * $Date: 2015/07/17 13:20:44 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup CARRIERLIB
 *
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

#ifndef __CARRIER_FMC_SERVICE_H__
#define __CARRIER_FMC_SERVICE_H__

#include "bas_error.h"

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "carrier_service_def.h"
#include "carrier_fmc_service_def.h"
#include "FMCEEprom_eeprom_def.h"

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/****************************************************************************
 * Function: Carrier_FmcDiscover
 ************************************************************************//**
 *
 * Scan the system and assign detected eeprom modules.
 * @warning 
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] printout
 *    CARRIER_printout enum describing the desired console printout.
 *
 * @return
*      Return success or failure result. See CARRIER_service_error enum.
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_FmcDiscover(Carrier_handle_t hCarrier, CARRIER_printout printout);


/****************************************************************************
 * Function: CARRIER_get_info
 ************************************************************************//**
 *
 * Get information about a specific board in a connector. The type of information
 * will provide a specific answer for that type. 
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    Fmc connector used.
 *
 * @param [in] board
 *    Fmc board in connector used.
 *
 * @param [in] type
 *    Type of information needed for the specified connector/board.
 *
 * @param [in/out] stringname
 *    Buffer provided where the result string will be stored.
 *
 * @param [in] stringsize
 *    Size of buffer provided where the result string will be stored.
 *
 * @return
*      Return success or failure result. See CARRIER_service_error enum.
 * 
 * @pre
 *    CARRIER_discover.
 *     
 * @see
 *
 ***************************************************************************/
Result Carrier_FmcGetInfo(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, 
    Carrier_eFmcBoardNum_t board, CARRIER_infotype type, char *stringname, unsigned stringsize);


/****************************************************************************
 * Function: CARRIER_validate_presence
 ************************************************************************//**
 *
 * Allow to validate if a module type is detected in the specified connector/board.
 * If a module is identified correctly, the
 * detection operation will be rejected if it is called for a different type of modules
 * than what is in the connector/board. If the module is not identified (or an error is found) in the connector/board
 * and the specified modlst has module configured for compatibility mode, it
 * will allow the request for the operation if the FPGA core is matching.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    Fmc connector used.
 *
 * @param [in] board
 *    Fmc board in connector used.
 *
 * @param [in] modlst
 *    List of module types concerned by this operation validation.
 *
 * @param [in] nmodlst
 *    Number of module types in modlst list.
 *
 * @param [out] detected
 *    Hold the state of detection: !=CARRIER_NOT_DETECTED if detected, ==CARRIER_NOT_DETECTED if not.
 *
 * @return
 *    Return success or failure result. See CARRIER_service_error enum.
 * 
 * @pre
 *    CARRIER_discover().
 *     
 * @see
 *
 ***************************************************************************/                                   
Result Carrier_FmcValidatePresence(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, 
    Carrier_eFmcBoardNum_t board,  FMCEEprom_module_types *modlst, unsigned nmodlst, CARRIER_detected *detected);


/****************************************************************************
 * Function: CARRIER_powerup_board
 ************************************************************************//**
 *
 * Allow to configure vAdj voltage according to what is needed for a module.
 * According to module type operating mode, the vAdj Value provided is
 * used or ignored, but only possibly used for module operating in compatibility mode.
 * If a module is identified correctly and operating in safe mode, the
 * power operation will be rejected if it is called for a different type of modules
 * than what is in the connector/board. If the module is not identified in the connector/board
 * and the specified modlst has module configured for compatibility mode, it
 * will allow the request with the specified vadj Value. If the module in the connector/board
 * is identified but running in compatibility mode, the Vadj function will be allowed
 * with the specified Value if the specified modlist has some modules defined in
 * compatibility mode.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    Fmc connector used.
 *
 * @param [in] board
 *    Fmc board in connector used.
 *
 * @param [in] modlst
 *    List of module types concerned by this powerup operation.
 *
 * @param [in] nmodlst
 *    Number of module types in modlst list.
 *
 * @param [in] Value
 *    Vadj value to be used for module running in compatibility mode.
 *
 * @return
*      Return success or failure result. See CARRIER_service_error enum.
 * 
 * @pre
 *    CARRIER_discover().
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_FmcPowerUp(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, 
    Carrier_eFmcBoardNum_t board, FMCEEprom_module_types *modlst, unsigned nmodlst, Carrier_eVadjValue_t Value);

/****************************************************************************
 * Function: CARRIER_get_signals_info
 ************************************************************************//**
 *
 * This will calculates the data lanes calibration information in delays (ps)
 * for the specified module.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    Fmc connector used.
 *
 * @param [in] board
 *    Fmc board in connector used.
 *
 * @param [inout] pDelay
 *    Pointer on a provided delay structure that will be filled with calculated delays.
 *
 * @return
*      Return success or failure result. See CARRIER_service_error enum.
 * 
 * @pre
 *    CARRIER_init(), CARRIER_discover().
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_GetSignalsInfo(Carrier_handle_t hCarrier,
    Carrier_eFmcConnectorNum_t connector,
    Carrier_eFmcBoardNum_t board,
    FMCEEPROM_HPC_DATA_Specs *pDelay);
                             

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

