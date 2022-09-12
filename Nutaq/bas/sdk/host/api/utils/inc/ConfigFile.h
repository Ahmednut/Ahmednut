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
 * @file    ConfigFile.h 
 * @brief   Function declarations for a .ini parser to get all necessary parameters for
 *          a specific application
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

#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

typedef enum E_CONFIGFILE_SECTION /**< ConfigFile Section Enum */
{
	CONFIGFILE_SECTION_NOT_VALID = 0,
	CONFIGFILE_SECTION_VALID     = 1,
} CONFIGFILE_SECTION /**< ConfigFile Section Enum */ ;

typedef enum E_CONFIGFILE_PARAMETER /**< ConfigFile Parameter Enum */
{
	CONFIGFILE_PARAM_NOT_FOUND = 0,
	CONFIGFILE_PARAM_FOUND     = 1,
} CONFIGFILE_PARAMETER /**< ConfigFile Parameter Enum */ ;


static Result GetParameterValueInLine(const char line[], const char * Parameter, const char * ValueType, void * pValue, uint32_t ValueMaxSize);
static CONFIGFILE_SECTION ValidateSection(const char line[], const char * Section);
static void string_lower(char string_to_lower[]);
static void remove_character_from_line(char * line, const char character_to_remove);

Result ConfigFile_GetParamValue(const char * Filename, const char * Section, const char * Parameter, const char * ValueType, void * pValue, uint32_t ValueMaxSize);
Result ConfigFile_GetSectionNamesFromParameter(const char * Filename, const char * Parameter, char ** Section, uint32_t * u32NbSections);


/** @name ConfigFile error types
 * 
 *  Possible ConfigFile errors   
 */
enum E_CONFIGFILE_ERROR 
{
	CONFIGFILEOK                = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_CONFIGFILE, 0),      /**< No error */
	CONFIGFILEBADFILEEXTENSION  = MAKE_RESULT(LYR_SEVERITY_ERROR  , MODULE_CONFIGFILE, 1),      /**< Bad file extension (not .ini) */ 
	CONFIGFILEFILENOTFOUND      = MAKE_RESULT(LYR_SEVERITY_ERROR  , MODULE_CONFIGFILE, 2),      /**< .ini file not found */
	CONFIGFILEPARAMNOTFOUND     = MAKE_RESULT(LYR_SEVERITY_ERROR  , MODULE_CONFIGFILE, 3),      /**< Parameter not found in .ini file */
	CONFIGFILEMAXSIZE           = MAKE_RESULT(LYR_SEVERITY_ERROR  , MODULE_CONFIGFILE, 4),      /**< Invalid ValueMaxSize */	
	CONFIGFILEVALUETYPE         = MAKE_RESULT(LYR_SEVERITY_ERROR  , MODULE_CONFIGFILE, 5),      /**< Invalid ValueType */
	CONFIGFILENOFMCTOINIT		= MAKE_RESULT(LYR_SEVERITY_ERROR  , MODULE_CONFIGFILE, 6),      /**< No FMC to initiliaze */
    CONFIGFILEWRONGVALUE		= MAKE_RESULT(LYR_SEVERITY_ERROR  , MODULE_CONFIGFILE, 7),      /**< Value insuitable for parameter */
};


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // CONFIGFILE_H