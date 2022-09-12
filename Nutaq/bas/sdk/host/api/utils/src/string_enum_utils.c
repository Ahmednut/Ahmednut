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
 * @file    StringToEnum.c 
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

#include <string.h>
#include "bas_error.h"
#include "bas_types.h"
#include "string_enum_utils.h"

Result StringToEnumWithSize(const string_enum_element_t * StringEnumTable, uint32_t TableSize, const char * Name, void * pEnumValue, uint32_t EnumSize)
{
    int32_t i;
    
    for(i=0; i<TableSize/sizeof(string_enum_element_t); i++)
    {
        if(strcmp(Name,StringEnumTable[i].enum_string)==0)
        {
            if(EnumSize==1)      *(uint8_t *)pEnumValue  = (uint8_t)StringEnumTable[i].enum_value;
            else if(EnumSize==2) *(uint16_t *)pEnumValue = (uint16_t)StringEnumTable[i].enum_value;
            else if(EnumSize==4) *(uint32_t *)pEnumValue = (uint32_t)StringEnumTable[i].enum_value;
            else if(EnumSize==8) *(uint64_t *)pEnumValue = (uint64_t)StringEnumTable[i].enum_value;
            else return -1;

            return 0;
        }
    }
	
    return -1;
}

const char * EnumToStringWithSize(const string_enum_element_t * StringEnumTable, uint32_t TableSize, uint64_t EnumValue)
{
    int32_t i;
    
    for(i=0; i<TableSize/sizeof(string_enum_element_t); i++)
    {
        if(StringEnumTable[i].enum_value == EnumValue)
        {
            return StringEnumTable[i].enum_string;
        }
    }
	
    return UNKNOWN_ENUM;
}
