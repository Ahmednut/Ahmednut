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
 * @file       Ina226.h
 * @brief      Power monitoring function definitions
 *
 * $Date: 2015/10/05 15:51:27 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup DRIVERS
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

#ifndef INA226_H__
#define INA226_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <stdlib.h>

#include "bas_types.h"
#include "bas_error.h"
#include "DevCom.h"
#include "Ina226_defines.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

Result Ina226_Open(Ina226_hIna226 * phIna226, DevCom_hCom hCom, uint32_t u32DeviceId, int (*pfUsleep)(unsigned int usec));
void Ina226_Close(Ina226_hIna226 * phIna226);
Result Ina226_Init(Ina226_hIna226 hIna226, float fRshunt, float fCurrentLsb);
Result Ina226_SetMode(Ina226_hIna226 hIna226, Ina226_eMode Mode, uint8_t u8IsBlocking);
Result Ina226_SetConversionTime(Ina226_hIna226 hIna226, Ina226_eConvTime BusConvTime, Ina226_eConvTime ShuntConvTime);
Result Ina226_SetAveraging(Ina226_hIna226 hIna226, Ina226_eAveraging Averaging);
Result Ina226_ReadBusVoltage(Ina226_hIna226 hIna226, float * pfVoltage);
Result Ina226_ReadPower(Ina226_hIna226 hIna226, float * pfPower);
Result Ina226_ReadCurrent(Ina226_hIna226 hIna226, float * pfCurrent);


#ifdef __cplusplus
}
#endif  // extern "C"
#endif

