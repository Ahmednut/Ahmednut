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
 * @file      aurora_defines.h
 * @brief     Aurora module related data and structure definitions.
 *
 * $Date: 2016/04/22 20:01:29 $
 * $Revision: 1.4 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup AURORA
  *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef __AURORA_DEFINES__
#define __AURORA_DEFINES__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "aurora_regs.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

/** @name Aurora core instance structure
 *
 *  Hold all information regarding Aurora core instance used.
 */
typedef struct S_aurora {
    volatile aurora_Register_Map * registers;
    unsigned char ucIsOpen;
    int (*pfUsleep)(unsigned int usec);
} aurora_core;

/** @name Aurora channel direction
 *
 *  Enum holding Aurora channel directions
 */
typedef enum Aurora_eDirection {
    eAuroraDirectionRx = 0,
    eAuroraDirectionTx = 1,
} Aurora_eDirection;

/** @name Aurora Error codes
 *
 *  List all runtime related error codes of a Aurora module.
 */
enum aurora_error {
    AURORAOK            = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_AURORA, 0),                ///<  No error
    AURORA_INVALID_CORE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AURORA, 1),                  ///<  Aurora core is present
    AURORA_NOT_OPEN     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AURORA, 2),                  ///<  Aurora core is not open
    AURORA_ALREADY_OPEN = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AURORA, 3),                  ///<  Aurora core is already open
    AURORA_CHANNEL_DOWN = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AURORA, 4),                  ///<  Aurora channel is down
    AURORA_INVALID_ARG  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AURORA, 5),                  ///<  Aurora invalid argument
};

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
