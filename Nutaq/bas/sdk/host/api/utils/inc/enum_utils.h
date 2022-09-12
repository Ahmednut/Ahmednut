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
 * @file       enum_utils.h
 * @brief      Definitions for module enumeration
 *
 * $Date: 2015/08/10 17:10:41 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
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

#ifndef __ENUM_UTILS_H__
#define __ENUM_UTILS_H__

#ifdef __cplusplus
extern "C" {
#endif


// Max size of enum string
#define ENUM_STRINGSIZE 200

/** @name ENUM UTILS enumeration info
 * 
 *  Hold module enumeration information
 */
typedef struct ENUM_UTILS_S_enuminfo {
   char carrier_modstate[ENUM_STRINGSIZE];     ///< Carrier current module status
   char carrier_boardtype[ENUM_STRINGSIZE];    ///< Carrier identified board type
   char carrier_modcoretype[ENUM_STRINGSIZE];  ///< Carrier fpga core type for this module
   char carrier_modcorename[ENUM_STRINGSIZE];  ///< Carrier fpga core name for this module
   char carrier_vadjsettings[ENUM_STRINGSIZE]; ///< Carrier current vadj setting
   char module_idname[ENUM_STRINGSIZE];        ///< Module info: module identification type
   char module_manuf[ENUM_STRINGSIZE];         ///< Module info: module manufacturer
   char module_partnumber[ENUM_STRINGSIZE];    ///< Module info: module part number
   char module_serialnumber[ENUM_STRINGSIZE];  ///< Module info: module serial number
   char module_hardrev[ENUM_STRINGSIZE];       ///< Module info: module hardware revision
   char module_firmrev[ENUM_STRINGSIZE];       ///< Module info: module firmware revision
   char module_vadjinfo[ENUM_STRINGSIZE];      ///< Module info: module vadj information
} ENUM_UTILS_enuminfo;

/**
*****************************************************************************
* Function: iGetModulesInfo
*****************************************************************************
*
* Reads system module information for the specified number of module. 
*
* @param [in]  hConnState
*      Pointer to a valid connection state.
*
* @param [in]  nmod
*      number of modules to scan to get information.
*      WARNING: scan vector must be the size of nmod to hold sufficient information holder
*               for each module to scan.
*
* @param [in]  printflg
*      when !=0 will print the module information.
*
* @param [out]  scan
*      Vector of module information structure. Vector must be the size of the expected
*      number of modules to read. Will returns modules information.
*
* @return
*      Success or error code.
*
****************************************************************************/                              
adp_result_t iGetModulesInfo(connection_state *hConnState,  int nmod, int printflg, ENUM_UTILS_enuminfo scan[]);

#ifdef __cplusplus
}
#endif

#endif
