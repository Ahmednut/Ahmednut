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
 * @file       FMCEEprom_eeprom_cond.h
 * @brief      fmceeprom eeprom library modules identification conditions
 *             related data and structure definitions.
 *
 * $Date: 2016/05/04 21:38:08 $
 * $Revision: 1.4 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup FMCEEPROMLIB_EEPROM
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

#ifndef __FMCEEPROM_EEPROM_COND_H__
#define __FMCEEPROM_EEPROM_COND_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "FMCEEprom_ifru_vita.h"
#include "FMCEEprom_eeprom_def.h"
#include "FMCEEprom_def.h"
#include "FMCEEprom_eeprom_id.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/


/** @name FMCEEPROM eeprom module string match mode
 * 
 *  Hold match string mode.   
 */
typedef enum FMCEEprom_E_match_mode {
    FMCEEPROM_EEPROM_MATCH_BEGIN=0,                       ///< Match at beginning of searched field only
    FMCEEPROM_EEPROM_MATCH_ALL=1                          ///< Match anywhere in the searched field
}FMCEEprom_match_mode;


/** @name FMCEEPROM eeprom module string case match mode
 * 
 *  Hold case match string mode.   
 */
typedef enum FMCEEprom_E_case_mode {
    FMCEEPROM_EEPROM_CASE_SENSITIVE=0,                    ///< Match must be exact character case
    FMCEEPROM_EEPROM_CASE_INSENSITIVE=1                   ///< Match ignoring character case
}FMCEEprom_case_mode;


/** @name FMCEEPROM eeprom module operating mode
 * 
 *  Hold module operating mode when not identified correctly.   
 */
typedef enum FMCEEprom_E_operation_mode {
    FMCEEPROM_EEPROM_OPER_COMP=0,                         ///< Compatibility operating mode: unidentified module will operate anyway (Warning: with a risk of hardware failure)
    FMCEEPROM_EEPROM_OPER_SAFE=1                          ///< Safe mode, when the module is identified but not matching, it will refuse to operate (normal mode for vita 57.1 eeprom newer modules)
}FMCEEprom_operation_mode;


// This defines the detection scheme for all modules. To detect a new module scheme,
// 1- add the new module type at the end of FMCEEprom_module_types enum (before FMCEEPROM_EEPROM_MOD_LAST),
// 2- modifies FMCEEPROM_EEPROM_MOD_LAST in FMCEEprom_module_types enum,
// 3- add at least one new module match line at the end of this definition. example:
//    {"LVDS-??-??-E",FMCEEPROM_EEPROM_VITADECODE,FMCEEPROM_VITA_NUTAQFULL,FMCEEPROM_BOARD_PARTNUM,FMCEEPROM_EEPROM_MOD_LVDSE,FMCEEPROM_EEPROM_MATCH_BEGIN,FMCEEPROM_EEPROM_CASE_INSENSITIVE,FMCEEPROM_EEPROM_OPER_SAFE},
//    The line includes a module identification string (this substring will be search to identify the
//    module type "LVDS-??-??-E"). The character '?' can be used as a wildcard (match all character).
//    If different module types use similar module strings (see MI125 definitions below), you must
//    define the simpler string definition last to make sure that the more complex string are tested
//    first while attempting to identify the module (the definitions are tried 1st to last). Use the longest
//    detection string as possible to minimize the risk to wrongly detect a module. A special empty match
//    string can be used for test purpose {""}, this will automatically force to make this condition true
//    and the module will be identified with what is specified for this condition. The first empty match condition
//    will be used, the other are just ignored, if any other preceeding conditions were false. Do not use an empty
//    match string in release software (will provide wrong identification).
//    The 2nd item is the type of eeprom encoding used for that module.
//    Older modules uses FMCEEPROM_EEPROM_OLDDECODE, newer modules must use FMCEEPROM_EEPROM_VITADECODE.
//    The 3rd item is only meaningful for module encoded in vita format: it specifies vita level
//    requiered to approve the decoding as compliant. Third party module will use level FMCEEPROM_VITA_MINIMUM
//    or FMCEEPROM_VITA_FULL (choose the highest level compatible with the module that allows a detection
//    as this will provides safer decoding). Nutaq's module must requiere FMCEEPROM_VITA_NUTAQFULL level.
//    The 4th item is only meaningful for module encoded in vita format: it specifies vita board section
//    typelen (see FMCEEPROM_board_typelenf) field that will be used for the search (usually FMCEEPROM_BOARD_PARTNUM).
//    This board section field must be an ASCII typelen type (FMCEEPROM_ASCII).
//    The 5th item shows which module types will be associated from FMCEEprom_module_types when the search
//    string condition is successful. There could be more than one line with the same 5th item parameter (module type),
//    i.e. more than 1 search condition. Each line should be seen as an 'OR' between differents lines, if one matches
//    the module type is detected.
//    The 6th item shows the string match mode, if FMCEEPROM_EEPROM_MATCH_BEGIN, the string must match at the beginning
//    of the field and it could also be a substring of the searche field. If FMCEEPROM_EEPROM_MATCH_ALL is specified,
//    the module identification string will match if it is located anywhere in the searched field and if it is a substring
//    or the exact string of the searched field. For FMCEEPROM_EEPROM_OLDDECODE eeprom format, the field
//    'partNumber' is searched and for FMCEEPROM_EEPROM_VITADECODE eeprom format, the field used is the
//    one specified in the definition line (vita board section).
//    The 7th item shows the string case match mode, if FMCEEPROM_EEPROM_CASE_SENSITIVE, the string must match exact
//    string case between match string and filed checked. With FMCEEPROM_EEPROM_CASE_INSENSITIVE, the character
//    case is ignored while matching.
//    The 8th item shows the module operating mode. All newer vita57.1 eeproms compatible modules must operate
//    under FMCEEPROM_EEPROM_OPER_SAFE. Older non vita compatible eeproms modules could start with FMCEEPROM_EEPROM_OPER_COMP
//    to continue operation even if the module could not be identified (if the transition phase where it is possible
//    that the module could have invalid eeprom contents)
// Warning: vita level FMCEEPROM_VITA_DOESNOTMEET is used for MI250 because manufacturer generated some non compliant vita
//          eeprom format in some boards. A patch in the source code is requiered to allow decoding this module
//          type because vita level FMCEEPROM_VITA_MINIMUM is the minimum requiered for automatic detection of 3rd party modules. 
#define FMCEEPROM_MODULE_DETECT { \
    {"LSP159-",      FMCEEPROM_EEPROM_OLDDECODE,  FMCEEPROM_VITA_DOESNOTMEET,  FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_ADAC250,      FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_COMP}, \
    {"LSP163-1",     FMCEEPROM_EEPROM_OLDDECODE,  FMCEEPROM_VITA_DOESNOTMEET,  FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_FMCRADIO,     FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_COMP}, \
    {"FMC126",       FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_MINIMUM,      FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_ADC5000,      FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_COMP}, \
    {"MI125-16-E",   FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_MI125E,       FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"MI125-16-S",   FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_MI125,        FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"LVDS-??-??-E", FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_LVDSE,        FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"LVDS-??-??-S", FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_LVDS,         FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"ADEFINIR-SFP", FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_MINIMUM,      FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_SFP,          FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_COMP}, \
    {"ADEFINIR-QSFP",FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_MINIMUM,      FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_QSFP_SFPP,    FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_COMP}, \
    {"FMC108",       FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_MINIMUM,      FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_MI250,        FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_COMP}, \
    {"LSP163-331",   FMCEEPROM_EEPROM_OLDDECODE,  FMCEEPROM_VITA_DOESNOTMEET,  FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_FMCRADIOE,    FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"LSP163-321",   FMCEEPROM_EEPROM_OLDDECODE,  FMCEEPROM_VITA_DOESNOTMEET,  FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_FMCRADIOS,    FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"NTQ002-631",   FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQMIN,     FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_FMCRADIOE1V8, FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"NTQ002-621",   FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQMIN,     FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_FMCRADIOS1V8, FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"MI125W-16-E",  FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_MI125WE,      FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"MI125W-16-S",  FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_MI125W,       FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"MO1000-8-E",   FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_MO1000E,      FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"MO1000-8-S",   FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_MO1000,       FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"RADIO640-E",   FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_RADIO640E,    FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}, \
    {"RADIO640-S",   FMCEEPROM_EEPROM_VITADECODE, FMCEEPROM_VITA_NUTAQFULL,    FMCEEPROM_BOARD_PARTNUM,     FMCEEPROM_EEPROM_MOD_RADIO640,     FMCEEPROM_EEPROM_MATCH_BEGIN,  FMCEEPROM_EEPROM_CASE_INSENSITIVE,  FMCEEPROM_EEPROM_OPER_SAFE}  \
}


/** @name FMCEEPROM eeprom module identification search structure
 * 
 *  Hold information of modules identification conditions.   
 */
typedef struct FMCEEprom_S_moduleid_search {
    char* matchstr;                               ///< Module match string for identification
    FMCEEprom_decode_types decodet;               ///< Eeprom decode type used
    FMCEEPROM_vita_requierements vital;           ///< Vita decoding level needed
    FMCEEPROM_board_typelenf fieldused;           ///< Vita board section decoding field used
	FMCEEprom_module_types moduleid;              ///< Module identification number associated with this condition
    FMCEEprom_match_mode matchmode;               ///< Hold match string mode in the field
    FMCEEprom_case_mode casemode;                 ///< Hold case string match mode
    FMCEEprom_operation_mode operation;           ///< Hold the operating mode for this module type 
} FMCEEprom_moduleid_search;


// Allocates a variable to store the searched conditions.
#ifdef FMCEEPROM_EEPROM_SEARCHCONDITIONS_ALLOCVAR
FMCEEprom_moduleid_search FMCEEprom_modsearchcond[] = FMCEEPROM_MODULE_DETECT;
unsigned FMCEEprom_modsearchcond_size = (sizeof(FMCEEprom_modsearchcond) / sizeof(FMCEEprom_moduleid_search));
#else
extern FMCEEprom_moduleid_search FMCEEprom_modsearchcond[];
extern unsigned FMCEEprom_modsearchcond_size;
#endif


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

