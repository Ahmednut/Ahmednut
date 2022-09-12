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
 * @file       adc5000_serialize.c
 * @brief      Functions for the serialization of ADC5000 pll parameters
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADC5000
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
#include "adc5000_serialize.h"
#include "ad9517_defines.h"
#include "eapi_serialize.h"

void adc5000_pll_param_send(connection_state * state, ad9517_param * param)
{
    uint_send(state, param->out_freq[0]);
	uint_send(state, param->out_freq[1]);
	uint_send(state, param->out_freq[2]);
	uint_send(state, param->out_freq[3]);
	uint_send(state, param->pfd_freq);
	uint_send(state, param->pfd_freq_max);
	uint_send(state, param->pfd_freq_min);
	uint_send(state, param->vco_freq);
	uint_send(state, param->vco_freq_max);
	uint_send(state, param->vco_freq_min);
	uint_send(state, param->ref_freq);
    uint_send(state, param->Bfactor);
    uint_send(state, param->Rfactor);
    uint_send(state, param->Afactor);
    uint_send(state, param->Pfactor);
	ushort_send(state, param->out_divider[0]);
	ushort_send(state, param->out_divider[1]);
	ushort_send(state, param->out_divider[2]);
	ushort_send(state, param->out_divider[3]);
    uchar_send(state, param->external_clock);
    uchar_send(state, param->vco_divider);	
}

void adc5000_pll_param_recv(connection_state * state, ad9517_param * param)
{
    param->out_freq[0] = uint_recv(state);
    param->out_freq[1] = uint_recv(state);
    param->out_freq[2] = uint_recv(state);
    param->out_freq[3] = uint_recv(state);
    param->pfd_freq = uint_recv(state);
	param->pfd_freq_max = uint_recv(state);
    param->pfd_freq_min = uint_recv(state);
    param->vco_freq = uint_recv(state);
    param->vco_freq_max = uint_recv(state);
    param->vco_freq_min = uint_recv(state);
    param->ref_freq = uint_recv(state);
    param->Bfactor = uint_recv(state);
    param->Rfactor = uint_recv(state);
    param->Afactor = uint_recv(state);
    param->Pfactor = uint_recv(state);
    param->out_divider[0] = ushort_recv(state);
    param->out_divider[1] = ushort_recv(state);
    param->out_divider[2] = ushort_recv(state);
    param->out_divider[3] = ushort_recv(state);
    param->external_clock = uchar_recv(state);
    param->vco_divider = uchar_recv(state);
}
