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
 * @file    mi250_pll.h
 * @brief   MI250 PLL functions
 *
 * $Date: 2015/05/15 17:36:49 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI250
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

#ifndef __MI250_PLL_H
#define __MI250_PLL_H

#include "bas_types.h"
#include "mi250.h"
#include "mi250_types.h"
#include "bas_error.h"
#include "bas_fmc_device_def.h"
#include "bas_spi.h"

#ifdef __cplusplus
    extern "C" {
#endif

Result MI250_PLL_CalculateParams(unsigned refClk, unsigned AdcClk, MI250_PLL_CFG_PARAM cfgParam, MI250_PLL_PARAM * param);
Result MI250_PLL_Init(MI250Board_t * board);
Result MI250_PLL_Init_Extern_Ref(MI250Board_t * board);
Result MI250_PLL_Init_Extern(MI250Board_t * board);
Result MI250_PLL_SetConfig(MI250Board_t * board, MI250_PLL_PARAM param);
Result MI250_PLL_Calibrate(MI250Board_t * MI250Board, unsigned int acqfreq);
Result MI250_PLL_trigRegisterTransfert(MI250Board_t * board);
Result MI250_PLL_trigRegisterTransfert(MI250Board_t * board);
Result MI250_PLL_SyncOutput(MI250Board_t * board);
Result MI250_PLL_getStatus(MI250Board_t * board, MI250_PLL_STATUS * status);
Result MI250_PLL_setRefClkTuning(MI250Board_t * board, unsigned short value);

#ifdef __cplusplus
    }
#endif

#endif


