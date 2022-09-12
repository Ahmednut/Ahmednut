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
 * @file       Fmcomms3_defines.h
 * @brief      mo1000 module related structure and data definitions
 *
 * $Date: 2015/08/17 15:01:09 $
 * $Revision: 1.4 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MO1000
 *
 * This file is part of Nutaq’s ADP Software Suite.
 *
 * You may at your option receive a license to Nutaq’s ADP Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s ADP Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s ADP Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s ADP Software Suite under the GPL requires that your work based on
 * Nutaq’s ADP Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s ADP Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s ADP Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef FMCOMMS3_DEFINES_H__
#define FMCOMMS3_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "ad9361.h"


/******************************************************************************/
/*************************** Types Declarations *******************************/
/******************************************************************************/

#define MAX_FMCOMMS3    2


typedef struct Fmcomms3_stFmcomms3 *Fmcomms3_Handle_t;


/** @name MO1000 Error codes
 * 
 *  List all runtime related error codes of a mo1000 module.
 */
enum Fmcomms3_eError {
    eFmcomms3Ok               = MAKE_RESULT(LYR_SEVERITY_SUCCESS,   MODULE_FMCOMMS3, 0),    ///< No error
    eFmcomms3NoCore           = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 1),    ///< No matching FPGA core 
    eFmcomms3IoError          = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 2),    ///< I/O error
    eFmcomms3GainError        = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 3),    ///< Cannot read the gain
    eFmcomms3OutOfMem         = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 4),    ///< Out of memory
    eFmcomms3BadAddr          = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 5),    ///< Bad address
    eFmcomms3NoDev            = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 6),    ///< Device not present
    eFmcomms3InvArg           = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 7),    ///< Invalid argument
    eFmcomms3Timedout         = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 8),    ///< Connection timed out
    eFmcomms3SpiError         = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 9),    ///< SPI error
    eFmcomms3MasterCalib      = MAKE_RESULT(LYR_SEVERITY_ERROR,     MODULE_FMCOMMS3, 10)    ///< Calibration from the master module is not done
};                                                                                                                           

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif
