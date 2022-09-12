/****************************************************************************   
 * 
 *    ****                              *                                     
 *   ******                            ***                                    
 *   *******                           ****                                   
 *   ********    ****  ****     **** *********    ******* ****    *********** 
 *   *********   ****  ****     **** *********  **************  ************* 
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     **** 
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      **** 
 *  ****    *********  ****     ****   ****   ****       ****  ****      **** 
 *  ****     ********  ****    *****  ****    *****     *****  ****      **** 
 *  ****      ******   ***** ******   *****    ****** *******  ****** ******* 
 *  ****        ****   ************    ******   *************   ************* 
 *  ****         ***     ****  ****     ****      *****  ****     *****  **** 
 *                                                                       **** 
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//** 
 * 
 * @file    StringToEnum.h 
 * @brief   Convert a string to an enum value
 * 
 * Copyright (C) 2015, Nutaq, Canada. All rights reserved.
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

#include "bas_error.h"
#include "bas_types.h"

#ifndef STRINGTOENUM_H
#define STRINGTOENUM_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    
static const char UNKNOWN_ENUM[] = "Unknown";
    
#define StringToEnum(StringEnumTable, Name, EnumValue) StringToEnumWithSize(StringEnumTable, sizeof(StringEnumTable), Name, (void*)(&EnumValue), sizeof(EnumValue))
#define EnumToString(StringEnumTable, EnumValue) EnumToStringWithSize(StringEnumTable, sizeof(StringEnumTable), (uint64_t)(EnumValue))

Result StringToEnumWithSize(const string_enum_element_t * StringEnumTable, uint32_t TableSize, const char * Name, void * pEnumValue, uint32_t EnumSize);
const char * EnumToStringWithSize(const string_enum_element_t * StringEnumTable, uint32_t TableSize, uint64_t EnumValue);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CONFIGFILE_H