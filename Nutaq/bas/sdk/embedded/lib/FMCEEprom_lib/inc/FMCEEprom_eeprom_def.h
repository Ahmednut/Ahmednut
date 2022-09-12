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
 * @file       FMCEEprom_eeprom_def.h
 * @brief      fmceeprom eeprom library related data and structure definitions.
 *
 * $Date: 2015/07/10 15:20:27 $
 * $Revision: 1.2 $
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

#ifndef __FMCEEPROM_EEPROM_DEF_H__
#define __FMCEEPROM_EEPROM_DEF_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "FMCEEprom_oldeeprom_def.h"
#include "FMCEEprom_def.h"
#include "FMCEEprom_ifru_vita.h"
#include "FMCEEprom_eeprom_id.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define FMCEEPROM_EEPROM_MAX      2                     ///< Maximum different possibles eeprom in a system

// Do not change this value
#define FMCEEPROM_EEPROM_MAX_ADDR 8                     ///< Maximum different possibles eeprom address


/** @name FMCEEPROM eeprom sizes enum
 * 
 *  Hold information of different possible eeproms sizes.   
 */
typedef enum FMCEEprom_E_eeprom_sizes {
    FMCEEPROM_EEPROM_NOSIZE=0,                          ///< Unknown eeprom size
    FMCEEPROM_EEPROM_SZ2402=256,                        ///< Size of 2402 type eeprom in bytes
    FMCEEPROM_EEPROM_SZ24128=16384,                     ///< Size of 24128 type eeprom in bytes
    FMCEEPROM_EEPROM_MAXSIZE=FMCEEPROM_EEPROM_SZ24128,  ///< Biggest eeprom size
}FMCEEprom_eeprom_sizes;


/** @name FMCEEPROM eeprom types enum
 * 
 *  Hold information of different possible eeproms types.   
 */
typedef enum FMCEEprom_E_eeprom_types {
    FMCEEPROM_EEPROM_NONE=0x0,               ///< No eeprom detected
    FMCEEPROM_EEPROM_24128=0x1,              ///< 24128 (16 Kbytes) eeprom type
    FMCEEPROM_EEPROM_2402=0x2,               ///< 2402 (256 bytes) eeprom type
    FMCEEPROM_EEPROM_UNKNOWN=0x3             ///< Epprom detected but was not identified correctly (empty eeprom?)
}FMCEEprom_eeprom_types;


/** @name Module connector identification
 * 
 *  Hold information of different possible module connector identification.   
 */
typedef enum FMCEEprom_E_eeprom_modconn {
    FMCEEPROM_EEPROM_CONNP1=0x0,             ///< Module connector P1 specified
    FMCEEPROM_EEPROM_CONNP2=0x1,             ///< Module connector P2 specified (valid for double width module only)
    FMCEEPROM_EEPROM_CONN_LAST=FMCEEPROM_EEPROM_CONNP2+1  ///< Last connector type
}FMCEEprom_eeprom_modconn;


/** @name FMCEEPROM eeprom decode types enum
 * 
 *  Hold information of different eeproms decoding types.   
 */
typedef enum FMCEEprom_E_decode_types {
    FMCEEPROM_EEPROM_NOTYPE=0,                               ///< Unknown eeprom decode type
    FMCEEPROM_EEPROM_OLDDECODE=1,                            ///< Old eeprom decoding format
    FMCEEPROM_EEPROM_VITADECODE=2,                           ///< Standard vita 57 decode format
    FMCEEPROM_EEPROM_LASTDECODE=FMCEEPROM_EEPROM_VITADECODE, ///< Last decode type
}FMCEEprom_decode_types;


/** @name FMCEEPROM eeprom discovery structure
 * 
 *  Hold information of eeproms discovered in the system.   
 */
typedef struct FMCEEprom_S_eepromdef {
    unsigned char neeprom;                                 ///< number of eeprom discovered
    unsigned char eeadr[FMCEEPROM_EEPROM_MAX_ADDR];        ///< eeprom i2c 7 bits address
    FMCEEprom_eeprom_types type[FMCEEPROM_EEPROM_MAX_ADDR];///< eeprom type
} FMCEEprom_eepromdef;

/** @name FMCEEPROM eeprom discovery structure
 * 
 *  Hold information of eeproms discovered in the system.   
 */
typedef struct FMCEEprom_S_eepromdata {
    // Do not change field order
    unsigned char data[FMCEEPROM_EEPROM_MAXSIZE];       ///< hold eeprom data (binary content)
    FMCEEprom_eeprom_sizes size;                        ///< hold eeprom data size (<=FMCEEPROM_EEPROM_MAXSIZE)
} FMCEEprom_eepromdata;


union FMCEEPROM_U_EEprom_OldDecode
{
    // Do not change field order
    
    // Old custom eeprom decoding format
    fmcInfoStruct_t olddecode;

    // Eeprom binary content
    FMCEEprom_eepromdata eebindata;
};


/** @name FMCEEPROM eeprom decoding information
 * 
 *  Hold all eeprom decoding information.
 */
typedef struct FMCEEPROM_S_EEprom_Decode
{
    unsigned char neepromrd;                                          ///< number of eeprom read correctly
    unsigned char neeprom_decoded;                                    ///< number of eeprom decoded correctly
    unsigned char nmodule_identified;                                 ///< number of eeprom modules identified correctly
    unsigned char eescan_index[FMCEEPROM_EEPROM_MAX];                 ///< hold eeprom scan inventory index for a read eeprom. (max index neepromrd)
    union FMCEEPROM_U_EEprom_OldDecode eeprom[FMCEEPROM_EEPROM_MAX];  ///< Hold eeprom binary content/ old decoding when eedecode != FMCEEPROM_EEPROM_VITADECODE. (max index neepromrd)

    FMCEEprom_decode_types eedecode[FMCEEPROM_EEPROM_MAX];            ///< hold eeprom decode type. (max index neeprom_decoded.)
    unsigned char eescan_dindex[FMCEEPROM_EEPROM_MAX];                ///< hold eeprom index for a decoded eeprom. (max index neeprom_decoded)
    // Vita decoding for newer module with eeprom format as vita 57
    FMCEEPROM_content vitadecode[FMCEEPROM_EEPROM_MAX];               ///< Hold vita new decoding when eedecode = FMCEEPROM_EEPROM_VITADECODE. (index max neeprom_decoded)
    FMCEEPROM_vita_requierements dvitalevel[FMCEEPROM_EEPROM_MAX];    ///< Hold vita level detected while decoding in vitadecode. (index max neeprom_decoded)
    unsigned char eescan_identindex[FMCEEPROM_EEPROM_MAX];            ///< hold eeprom decode index for an identified module. (index max nmodule_identified)
    FMCEEprom_module_types modtype[FMCEEPROM_EEPROM_MAX];             ///< Hold modules types when they are identified with their eeprom (index max nmodule_identified)
} FMCEEPROM_EEprom_Decode;


/** @name FMCEEPROM Error codes
 * 
 *  List all runtime related error codes of a FMCEEPROM ee library.
 */
enum FMCEEPROM_eeprom_error {
    FMCEEPROM_EEPROM_OK                  = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_FMCEEPROM_EEPROM, 0), ///< No error
    FMCEEPROM_ERR_INVALID_EEPROMNUM      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 1),   ///< Invalid eeprom id given
    FMCEEPROM_EEPROM_ERR_READSIZE        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 2),   ///< EEprom read size error
    FMCEEPROM_EEPROM_ERR_WRITESIZE       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 3),   ///< EEprom write size error
    FMCEEPROM_ERR_INVALID_EEPROMTYPE     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 4),   ///< Invalid eeprom type
    FMCEEPROM_ERR_INVALID_IDENTVITAFIELD = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 5),   ///< Invalid vita identification filed used in matching condition
    FMCEEPROM_ERR_INVALID_IDENTEEDECODE  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 6),   ///< Wrong eeprom decoding type specified in matching condition
    FMCEEPROM_ERR_COULDNOTREADALLEEPROM  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 7),   ///< Error, could not read all available eeprom (some maybe unidentified)
    FMCEEPROM_ERR_I2CWAIT_TIMEOUT        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 8),   ///< I2C peripheral busy timeout
    FMCEEPROM_ERR_UNKNOWN_EEPROMTYPE     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 9),   ///< Unknown eeprom type error while reading
    FMCEEPROM_ERR_UNKNOWN_MODULETYPE     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 10),  ///< Unknown module type, not identified correctly
    FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 11),  ///< Unknown module eeprom decoded format
    FMCEEPROM_ERR_INVALID_VADJINFO       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 12),  ///< Invalid vadj information discovered (or not present)
    FMCEEPROM_ERR_INVALID_CONNINFO       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 13),  ///< Invalid module connector info specified
    FMCEEPROM_ERR_INVALID_MODID_INFO     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 14),  ///< Invalid module id string information
    FMCEEPROM_ERR_INVALID_PARTNUM_INFO   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 15),  ///< Invalid part number information
    FMCEEPROM_ERR_INVALID_SERIALNUM_INFO = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 16),  ///< Invalid serial number information
    FMCEEPROM_ERR_INVALID_HARDREV_INFO   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 17),  ///< Invalid hardware revision information
    FMCEEPROM_ERR_INVALID_FIRMREV_INFO   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 18),  ///< Invalid firmware revision information
    FMCEEPROM_ERR_INVALID_INTERNAL_INFO  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 19),  ///< Invalid internal custom section
    FMCEEPROM_ERR_INVALID_MANUF_INFO     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_FMCEEPROM_EEPROM, 20)   ///< Invalid manufacturer information
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

