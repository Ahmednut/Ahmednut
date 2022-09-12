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
 * @file       mi250_serialize.c
 * @brief      MI250 module related additionnal functions
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
#include "mi250_serialize.h"
#include "eapi_serialize.h"
#include "connection_state.h"

void MI250_pll_param_send(connection_state * state, MI250_PLL_PARAM * param)
{
    uint_send(state, param->Afactor);
    uint_send(state, param->Bfactor);
    uint_send(state, param->Rfactor);
    uint_send(state, param->Pfactor);
    uint_send(state, param->VCOFreq);
    uchar_send(state, param->divisorADCClk);
    uchar_send(state, param->divisorClkOutClk);
    uchar_send(state, param->divisorRsvdClk);
}


void MI250_pll_param_recv(connection_state * state, MI250_PLL_PARAM * param)
{
    param->Afactor = uint_recv(state);
    param->Bfactor = uint_recv(state);
    param->Rfactor = uint_recv(state);
    param->Pfactor = uint_recv(state);
    param->VCOFreq = uint_recv(state);
    param->divisorADCClk = uchar_recv(state);
    param->divisorClkOutClk = uchar_recv(state);
    param->divisorRsvdClk = uchar_recv(state);
}
