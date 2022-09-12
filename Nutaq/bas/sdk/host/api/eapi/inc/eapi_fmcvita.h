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
 * @file       eapi_fmcvita.h
 * @brief      Function definitions for the eapi fmc vita control
 *
 * $Date: 2016/07/22 14:43:26 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup EAPI_FMCVITA
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
 
#ifndef __EAPI_FMCVITA__
#define __EAPI_FMCVITA__

#include "bas_error.h"
#include "connection_state.h"
#include "FMCEEprom_eeprom_def.h"

// Maximum return string size for any function that returns a string
#define EAPI_FMCVITA_MAXSTRING 1000

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup EAPI_FMCVITA Module Enumeration services library
 *  
 *  Functions to deal with enumeration of modules and related
 *  information (part number, serial number, model, module type, etc).   
 *               
 *  @file
 *     
 */

/// @cond EEPROM

 /****************************************************************************
 * NAME : FMCEEprom_eeprom_read_send
 ************************************************************************//**
 *
 * 	Read module binary eeprom information.
 * 	This function is meant for internal NUTAQ use only. 
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] epdata
 *    Eeprom data structure to return the result.
 *
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_read_send(connection_state * state, FMCEEprom_eepromdata *oepdata);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_write_send
 ************************************************************************//**
 *
 * 	Write module binary eeprom information.
 * 	This function is meant for internal NUTAQ use only. 
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] epdata
 *    Eeprom data structure for writing.
 *
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_write_send(connection_state * state, FMCEEprom_eepromdata *epdata);


/// @endcond

 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_idstring_send
 ************************************************************************//**
 *
 * 	Read module identification string (stored in module eeprom).
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_idstring_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_board_state_send
 ************************************************************************//**
 *
 * 	Read current module operation status.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_board_state_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_board_type_send
 ************************************************************************//**
 *
 * 	Read current module board type information.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_board_type_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_core_send
 ************************************************************************//**
 *
 * 	Read current module associated fpga core identification number.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_core_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_carriervadj_send
 ************************************************************************//**
 *
 * 	Read current module carrier vadj settings.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_carriervadj_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_partnum_send
 ************************************************************************//**
 *
 * 	Read module part number string (stored in module eeprom).
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_partnum_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_serialnum_send
 ************************************************************************//**
 *
 * 	Read module serial number string when available (stored in module eeprom).
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_serialnum_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_hardrev_send
 ************************************************************************//**
 *
 * 	Read module hardware revision string when available (stored in module eeprom).
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_hardrev_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_firmrev_send
 ************************************************************************//**
 *
 * 	Read module firmware revision string when available (stored in module eeprom).
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_firmrev_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_modvadj_send
 ************************************************************************//**
 *
 * 	Read module vadj requierements (stored in module eeprom).
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_modvadj_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_corename_send
 ************************************************************************//**
 *
 * 	Read current module associated fpga core identification name.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_corename_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);


 /****************************************************************************
 * NAME : FMCEEprom_eeprom_get_manuf_send
 ************************************************************************//**
 *
 * 	Read module manufacturer string (stored in module eeprom).
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use (1 for bottom, 2 for top fmc board).
 *
 * @param [in,out] mystring
 *    Buffer with sufficient length to hold the returned information 
 *   
 * @param [in] stringlen
 *    Length of mystring buffer 
 *   
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_manuf_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen);

 /****************************************************************************
 * NAME : FMCEEprom_eeprom_wipe_send
 ************************************************************************//**
 *
 * 	Wipes the FMC EEPROM. This operation is irreversible and it is exclusively
 *  reserved for Nutaq's internal use.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] password
 *    Password
 *
 * @return
 *    Error code: SUCCESS(ret) is true if no problem is found.
 * 
 * @pre
 *    
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 ***************************************************************************/ 
Result FMCEEprom_eeprom_wipe_send(connection_state * state, unsigned short password);


#ifdef __cplusplus
}
#endif

#endif

