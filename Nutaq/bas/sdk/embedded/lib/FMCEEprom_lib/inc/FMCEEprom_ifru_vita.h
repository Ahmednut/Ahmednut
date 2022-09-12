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
 * @file       FMCEEprom_ifru_vita.h
 * @brief      fmceeprom library vita routines and definitions.
 *
 * $Date: 2015/04/10 14:08:11 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup FMCEEPROMLIB
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
#ifndef __FMCEEPROM_IFRU_VITA_H__
#define __FMCEEPROM_IFRU_VITA_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


#ifndef uchar
#define uchar unsigned char
#endif

#include "FMCEEprom_def.h"

/** @name VITA FMC requierements levels
 * 
 *  List of possible vita conformance levels.   
 */
typedef enum FMCEEPROM_E_vita_requierements {
    FMCEEPROM_VITA_DOESNOTMEET=0x0,  ///< Does not meet vita fmc minimum requierements
    FMCEEPROM_VITA_MINIMUM=0x1,      ///< Meet vita minimal requierements
    FMCEEPROM_VITA_FULL=0x2,         ///< Meet all vita requierements including optionnal ones for i2c devices
    FMCEEPROM_VITA_NUTAQMIN=0x3,     ///< Meet vita minimal requierements (no i2c devices) and Nutaq custom requierements (must be used for boards without i2c devices)
    FMCEEPROM_VITA_NUTAQFULL=0x4     ///< Meet all vita requierements including optionnal ones (i2c devices) and Nutaq custom requierements (must be used for boards with i2c devices)
}FMCEEPROM_vita_requierements;

/// @cond VITAFCT 
int show_fru_vita(uchar *pdata, int dlen,FMCEEPROM_content *fmceeprom,int chkerr);

int check_vita_requierements(FMCEEPROM_content *fmceeprom, FMCEEPROM_vita_requierements level, FMCEEPROM_vita_requierements *rdlevel );
/// @endcond

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif
