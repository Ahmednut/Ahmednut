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
 * @file       FMCEEprom_eeprom_id.h
 * @brief      fmceeprom eeprom library modules identification related data
 *             and structure definitions.
 *
 * $Date: 2015/10/21 13:08:16 $
 * $Revision: 1.5 $
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

#ifndef __FMCEEPROM_EEPROM_ID_H__
#define __FMCEEPROM_EEPROM_ID_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/


/** @name FMCEEPROM eeprom module types enum
 * 
 *  Hold information of different possible module types eeproms.   
 */
typedef enum FMCEEprom_E_module_types {
    FMCEEPROM_EEPROM_MOD_UNKNOWN=0,                             ///< Unknown module type
    FMCEEPROM_EEPROM_MOD_ADAC250=1,                             ///< ADAC250 module type
    FMCEEPROM_EEPROM_MOD_FMCRADIO=2,                            ///< FMC RADIO old module type (before rev D)
    FMCEEPROM_EEPROM_MOD_ADC5000=3,                             ///< ADC5000 module type
    FMCEEPROM_EEPROM_MOD_MI250=4,                               ///< MI250 module type
    FMCEEPROM_EEPROM_MOD_MI125E=5,                              ///< MI125 expander module type
    FMCEEPROM_EEPROM_MOD_MI125=6,                               ///< MI125 module type
    FMCEEPROM_EEPROM_MOD_LVDSE=7,                               ///< LVDS expander module type
    FMCEEPROM_EEPROM_MOD_LVDS=8,                                ///< LVDS module type
    FMCEEPROM_EEPROM_MOD_SFP=9,                                 ///< SFP module type
    FMCEEPROM_EEPROM_MOD_QSFP_SFPP=10,                          ///< QSFP and SFP+ module type
    FMCEEPROM_EEPROM_MOD_FMCRADIOE=11,                          ///< FMC RADIO expander module type
    FMCEEPROM_EEPROM_MOD_FMCRADIOS=12,                          ///< FMC RADIO siso module type
    FMCEEPROM_EEPROM_MOD_FMCRADIOE1V8=13,                       ///< FMC RADIO expander module type 1.8V
    FMCEEPROM_EEPROM_MOD_FMCRADIOS1V8=14,                       ///< FMC RADIO siso module type 1.8V
    FMCEEPROM_EEPROM_MOD_MI125WE=15,                            ///< MI125 wideband expander module type
    FMCEEPROM_EEPROM_MOD_MI125W=16,                             ///< MI125 wideband  module type
    FMCEEPROM_EEPROM_MOD_MO1000E=17,                            ///< MO1000 wideband expander module type
    FMCEEPROM_EEPROM_MOD_MO1000=18,                             ///< MO1000 wideband  module type
    FMCEEPROM_EEPROM_MOD_RADIO640E=19,                          ///< RADIO640 expander module type
    FMCEEPROM_EEPROM_MOD_RADIO640=20,                           ///< RADIO640 module type
    FMCEEPROM_EEPROM_MOD_LAST=(FMCEEPROM_EEPROM_MOD_RADIO640)+1 ///< Last module type, 
}FMCEEprom_module_types;


/** @name FMCEEPROM eeprom module types fpga core id enum
 * 
 *  Hold information of different possible module types associated fpga core id.
 *  This list follows the fpga cores list.    
 */
typedef enum FMCEEprom_E_fpga_coreid {
    FMCEEPROM_EEPROM_FPGACORE_UNKNOWN=0,                    ///< Unknown fpga core id
    FMCEEPROM_EEPROM_FPGACORE_ADC5000=0xADC5,               ///< ADC5000 fpga core id
    FMCEEPROM_EEPROM_FPGACORE_MI125=0xC125,                 ///< MI125 fpga core id
    FMCEEPROM_EEPROM_FPGACORE_RADIO420X=0xFCC1,             ///< RADIO 420x core id
    FMCEEPROM_EEPROM_FPGACORE_MI250=0xC250,                 ///< MI250 fpga core id
    FMCEEPROM_EEPROM_FPGACORE_LVDSIOPROD=0xB67A,            ///< LVDS IO PRODUCTION fpga core id
    FMCEEPROM_EEPROM_FPGACORE_LVDSIO=0x32CA,                ///< LVDS IO fpga core id
    FMCEEPROM_EEPROM_FPGACORE_ADAC0250=0xA250,              ///< ADAC 250 fpga core id
    FMCEEPROM_EEPROM_FPGACORE_MO1000=0xDAC0,                ///< MO1000 fpga core id
    FMCEEPROM_EEPROM_FPGACORE_RADIO640=0xF640               ///< RADIO640 fpga core id
}FMCEEprom_fpga_coreid;


/** @name FMCEEPROM eeprom module types fpga core name enum
 * 
 *  Hold information of different possible module fpga core id associated fpga core name.
 *  This list must match exactly FMCEEprom_fpga_coreid (one on one and same order).    
 */
#define FMCEEPROM_EEPROM_FPGACORENAME_UNKNOWN    "UNKNOWN"     ///< Unknown fpga core name
#define FMCEEPROM_EEPROM_FPGACORENAME_ADC5000    "ADC5000"     ///< ADC5000 fpga core name
#define FMCEEPROM_EEPROM_FPGACORENAME_MI125      "MI125"       ///< MI125 fpga core name
#define FMCEEPROM_EEPROM_FPGACORENAME_RADIO420X  "RADIO420X"   ///< RADIO 420x core name
#define FMCEEPROM_EEPROM_FPGACORENAME_MI250      "MI250"       ///< MI250 fpga core name
#define FMCEEPROM_EEPROM_FPGACORENAME_LVDSIOPROD "LVDSIOPROD"  ///< LVDS IO PRODUCTION fpga core name
#define FMCEEPROM_EEPROM_FPGACORENAME_LVDSIO     "LVDSIO"      ///< LVDS IO fpga core name
#define FMCEEPROM_EEPROM_FPGACORENAME_ADAC0250   "ADAC250"     ///< ADAC 250 fpga core name
#define FMCEEPROM_EEPROM_FPGACORENAME_MO1000     "MO1000"      ///< MO1000 fpga core name
#define FMCEEPROM_EEPROM_FPGACORENAME_RADIO640   "RADIO640"    ///< RADIO640 fpga core name


/** @name FMCEEPROM eeprom module types name enum
 * 
 *  Hold information of different possible module name.
 *  This list must match exactly FMCEEprom_module_types (one on one and same order).    
 */
#define FMCEEPROM_EEPROM_MODNAME_UNKNOWN         "UNKNOWN"     ///< Unknown module name
#define FMCEEPROM_EEPROM_MODNAME_ADAC0250        "ADAC250"     ///< ADAC 250 module name
#define FMCEEPROM_EEPROM_MODNAME_RADIO420X       "RADIO420X"   ///< RADIO module name
#define FMCEEPROM_EEPROM_MODNAME_ADC5000         "ADC5000"     ///< ADC5000 module name
#define FMCEEPROM_EEPROM_MODNAME_MI250           "MI250"       ///< MI250 module name
#define FMCEEPROM_EEPROM_MODNAME_MI125_E         "MI125-E"     ///< MI125 expander module name
#define FMCEEPROM_EEPROM_MODNAME_MI125           "MI125"       ///< MI125 module name
#define FMCEEPROM_EEPROM_MODNAME_LVDSIO_E        "LVDS-E"      ///< LVDS expander module name
#define FMCEEPROM_EEPROM_MODNAME_LVDSIO          "LVDS"        ///< LVDS module name
#define FMCEEPROM_EEPROM_MODNAME_SFP             "SFP"         ///< SFP module name
#define FMCEEPROM_EEPROM_MODNAME_QSFP_SFPP       "QSFP-SFP"    ///< QSFP-SFP module name
#define FMCEEPROM_EEPROM_MODNAME_RADIO420E       "RADIO420E"   ///< FMC RADIO expander module type
#define FMCEEPROM_EEPROM_MODNAME_RADIO420S       "RADIO420S"   ///< FMC RADIO siso module type
#define FMCEEPROM_EEPROM_MODNAME_RADIO420E1V8    "RADIO420E1V8"///< FMC RADIO expander module type 1.8V
#define FMCEEPROM_EEPROM_MODNAME_RADIO420S1V8    "RADIO420S1V8"///< FMC RADIO siso module type 1.8V
#define FMCEEPROM_EEPROM_MODNAME_MI125W_E        "MI125W-E"    ///< MI125 wideband expander module name
#define FMCEEPROM_EEPROM_MODNAME_MI125W          "MI125W"      ///< MI125 wideband module name
#define FMCEEPROM_EEPROM_MODNAME_MO1000_E        "MO1000-E"    ///< MO1000 expander module name
#define FMCEEPROM_EEPROM_MODNAME_MO1000          "MO1000"      ///< MO1000 module name
#define FMCEEPROM_EEPROM_MODNAME_RADIO640_E      "RADIO640-E"  ///< RADIO640 expander module name
#define FMCEEPROM_EEPROM_MODNAME_RADIO640        "RADIO640"    ///< RADIO640 module name


// This define match for each modules type in FMCEEprom_module_types, the fpga related core id
// It must have the same number of fpga core id as module types enum and they must be in the exact
// same order has the modules types enum.
#define FMCEEPROM_EEPROM_FPGA_COREID { \
  {FMCEEPROM_EEPROM_FPGACORE_UNKNOWN}, {FMCEEPROM_EEPROM_FPGACORE_ADAC0250}, {FMCEEPROM_EEPROM_FPGACORE_RADIO420X}, \
  {FMCEEPROM_EEPROM_FPGACORE_ADC5000}, {FMCEEPROM_EEPROM_FPGACORE_MI250}, {FMCEEPROM_EEPROM_FPGACORE_MI125}, \
  {FMCEEPROM_EEPROM_FPGACORE_MI125}, {FMCEEPROM_EEPROM_FPGACORE_LVDSIO}, {FMCEEPROM_EEPROM_FPGACORE_LVDSIO}, \
  {FMCEEPROM_EEPROM_FPGACORE_UNKNOWN}, {FMCEEPROM_EEPROM_FPGACORE_UNKNOWN}, {FMCEEPROM_EEPROM_FPGACORE_RADIO420X}, \
  {FMCEEPROM_EEPROM_FPGACORE_RADIO420X}, {FMCEEPROM_EEPROM_FPGACORE_RADIO420X},{FMCEEPROM_EEPROM_FPGACORE_RADIO420X}, \
  {FMCEEPROM_EEPROM_FPGACORE_MI125}, {FMCEEPROM_EEPROM_FPGACORE_MI125}, {FMCEEPROM_EEPROM_FPGACORE_MO1000}, {FMCEEPROM_EEPROM_FPGACORE_MO1000}, \
  {FMCEEPROM_EEPROM_FPGACORE_RADIO640}, {FMCEEPROM_EEPROM_FPGACORE_RADIO640} \
}

// This define match for each modules type in FMCEEprom_module_types, the fpga related core name
// It must have the same number of fpga core name as module types enum and they must be in the exact
// same order has the modules types enum.
#define FMCEEPROM_EEPROM_FPGA_CORENAME { \
  FMCEEPROM_EEPROM_FPGACORENAME_UNKNOWN, FMCEEPROM_EEPROM_FPGACORENAME_ADAC0250, FMCEEPROM_EEPROM_FPGACORENAME_RADIO420X, \
  FMCEEPROM_EEPROM_FPGACORENAME_ADC5000, FMCEEPROM_EEPROM_FPGACORENAME_MI250, FMCEEPROM_EEPROM_FPGACORENAME_MI125, \
  FMCEEPROM_EEPROM_FPGACORENAME_MI125, FMCEEPROM_EEPROM_FPGACORENAME_LVDSIO, FMCEEPROM_EEPROM_FPGACORENAME_LVDSIO, \
  FMCEEPROM_EEPROM_FPGACORENAME_UNKNOWN, FMCEEPROM_EEPROM_FPGACORENAME_UNKNOWN, FMCEEPROM_EEPROM_FPGACORENAME_RADIO420X, \
  FMCEEPROM_EEPROM_FPGACORENAME_RADIO420X,FMCEEPROM_EEPROM_FPGACORENAME_RADIO420X,FMCEEPROM_EEPROM_FPGACORENAME_RADIO420X, \
  FMCEEPROM_EEPROM_FPGACORENAME_MI125, FMCEEPROM_EEPROM_FPGACORENAME_MI125, FMCEEPROM_EEPROM_FPGACORENAME_MO1000, FMCEEPROM_EEPROM_FPGACORENAME_MO1000, \
  FMCEEPROM_EEPROM_FPGACORENAME_RADIO640, FMCEEPROM_EEPROM_FPGACORENAME_RADIO640 \
}

// This define match for each modules type in FMCEEprom_module_types, the module related name
// It must have the same number and they must be in the exact
// same order has the modules types enum.
#define FMCEEPROM_EEPROM_MODULE_NAME { \
  FMCEEPROM_EEPROM_MODNAME_UNKNOWN, FMCEEPROM_EEPROM_MODNAME_ADAC0250, FMCEEPROM_EEPROM_MODNAME_RADIO420X, \
  FMCEEPROM_EEPROM_MODNAME_ADC5000, FMCEEPROM_EEPROM_MODNAME_MI250, FMCEEPROM_EEPROM_MODNAME_MI125_E, \
  FMCEEPROM_EEPROM_MODNAME_MI125, FMCEEPROM_EEPROM_MODNAME_LVDSIO_E, FMCEEPROM_EEPROM_MODNAME_LVDSIO, \
  FMCEEPROM_EEPROM_MODNAME_SFP, FMCEEPROM_EEPROM_MODNAME_QSFP_SFPP, FMCEEPROM_EEPROM_MODNAME_RADIO420E, \
  FMCEEPROM_EEPROM_MODNAME_RADIO420S, FMCEEPROM_EEPROM_MODNAME_RADIO420E1V8, FMCEEPROM_EEPROM_MODNAME_RADIO420S1V8, \
  FMCEEPROM_EEPROM_MODNAME_MI125W_E, FMCEEPROM_EEPROM_MODNAME_MI125W, FMCEEPROM_EEPROM_MODNAME_MO1000_E, FMCEEPROM_EEPROM_MODNAME_MO1000, \
  FMCEEPROM_EEPROM_MODNAME_RADIO640_E, FMCEEPROM_EEPROM_MODNAME_RADIO640 \
}

// This define identifies allowed/compatible dual stack modules
// Compatible modules are modules that could be stacked together
// Dual stack modules combinaison not in this list won't be allowed
// Attention: Each line is bottom module id 1st, top module id 2nd.
// WARNING: there must be at least one entry in this table for each
// module that could be at top fmc position
#define FMCEEPROM_EEPROM_MODULESTACK_LIST { \
  {FMCEEPROM_EEPROM_MOD_MI125E,         FMCEEPROM_EEPROM_MOD_MI125}, \
  {FMCEEPROM_EEPROM_MOD_LVDSE,          FMCEEPROM_EEPROM_MOD_LVDS}, \
  {FMCEEPROM_EEPROM_MOD_FMCRADIO,       FMCEEPROM_EEPROM_MOD_FMCRADIO}, \
  {FMCEEPROM_EEPROM_MOD_FMCRADIOE,      FMCEEPROM_EEPROM_MOD_FMCRADIOS}, \
  {FMCEEPROM_EEPROM_MOD_FMCRADIOE1V8,   FMCEEPROM_EEPROM_MOD_FMCRADIOS1V8}, \
  {FMCEEPROM_EEPROM_MOD_FMCRADIOE,      FMCEEPROM_EEPROM_MOD_FMCRADIO}, \
  {FMCEEPROM_EEPROM_MOD_FMCRADIO,       FMCEEPROM_EEPROM_MOD_FMCRADIOS}, \
  {FMCEEPROM_EEPROM_MOD_MI125WE,        FMCEEPROM_EEPROM_MOD_MI125W}, \
  {FMCEEPROM_EEPROM_MOD_MI125WE,        FMCEEPROM_EEPROM_MOD_MI125}, \
  {FMCEEPROM_EEPROM_MOD_MI125E,         FMCEEPROM_EEPROM_MOD_MI125W}, \
  {FMCEEPROM_EEPROM_MOD_MO1000E,        FMCEEPROM_EEPROM_MOD_MO1000}, \
  {FMCEEPROM_EEPROM_MOD_MO1000E,        FMCEEPROM_EEPROM_MOD_MI125}, \
  {FMCEEPROM_EEPROM_MOD_MO1000E,        FMCEEPROM_EEPROM_MOD_MI125W}, \
  {FMCEEPROM_EEPROM_MOD_RADIO640E,      FMCEEPROM_EEPROM_MOD_RADIO640} \
}


/** @name FMCEEPROM eeprom module identification search structure
 * 
 *  Hold information of modules identification conditions.   
 */
typedef struct FMCEEprom_S_module_fpgacoreids {
    FMCEEprom_fpga_coreid fpgacore;                         ///< Hold fpga core id for a module
} FMCEEprom_module_fpgacoreids;


/** @name FMCEEPROM eeprom module compatibility stack structure
 * 
 *  Hold information of dual modules stack compatibility.   
 */
typedef struct FMCEEprom_S_dualmodule_compatibility {
    FMCEEprom_module_types bottommodule;                   ///< Hold bottom module identification
    FMCEEprom_module_types topmodule;                      ///< Hold top module identification
} FMCEEprom_dualmodule_compatibility;


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

