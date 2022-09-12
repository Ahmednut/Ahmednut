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
 * @file       fmcradio_serialize.c       
 * @brief      Serialize data structure of Radio420/Radio421 for use CCE/EAPI
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RADIO420
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
#include "fmcradio_serialize.h"
#include "eapi_serialize.h"

Result cdce62005_param_recv(connection_state * state, cdce62005_param * param)
{
    unsigned i;

    for (i=0; i<5; i++)
    {
        param->out_freq[i] = uint_recv(state);
    }
    param->pfd_freq = uint_recv(state);
    param->pfd_freq_max = uint_recv(state);
    param->pfd_freq_min = uint_recv(state);
    param->vco_freq = uint_recv(state);
    param->ref_freq = uint_recv(state);
    param->synth_freq = uint_recv(state);
    param->feedback_divider = ushort_recv(state);
    param->input_divider = ushort_recv(state);
    for (i=0; i<5; i++)
    {
        param->out_divider[i] = uchar_recv(state);
    }
    param->prescale_divider = uchar_recv(state);
    param->secondary_clock = uchar_recv(state);
    param->vco_bypass_all = uchar_recv(state);
    for(i=0; i<5; i++)
    {
        param->vco_bypass[i] = uchar_recv(state);
    }
    return OK;
}

Result cdce62005_param_send(connection_state * state, cdce62005_param * param)
{
    unsigned i;

    for (i=0; i<5; i++)
    {
        uint_send(state, param->out_freq[i]);
    }
    uint_send(state, param->pfd_freq);
    uint_send(state, param->pfd_freq_max);
    uint_send(state, param->pfd_freq_min);
    uint_send(state, param->vco_freq);
    uint_send(state, param->ref_freq);
    uint_send(state, param->synth_freq);
    ushort_send(state, param->feedback_divider);
    ushort_send(state, param->input_divider);
    for(i=0; i<5; i++)
    {
        uchar_send(state, param->out_divider[i]);
    }
    uchar_send(state, param->prescale_divider);
    uchar_send(state, param->secondary_clock);
    uchar_send(state, param->vco_bypass_all);
    for (i=0; i<5; i++)
    {
        uchar_send(state, param->vco_bypass[i]);
    }
    return OK;
}

Result lms6002_pll_param_send(connection_state * state, lms6002_pll_param * param)
{
    uint_send(state, param->out_freq);
    uint_send(state, param->ref_freq);
    uint_send(state, param->n_divider);
    return OK;
}

Result lms6002_pll_param_recv(connection_state * state, lms6002_pll_param * param)
{
    param->out_freq = uint_recv(state);
    param->ref_freq = uint_recv(state);
    param->n_divider = uint_recv(state);
    return OK;
}

Result lms6002_lo_leakage_calib_send(connection_state * state, lms6002_lo_leakage_calib * calib)
{
    uchar_send(state, calib->i_adjust);
    uchar_send(state, calib->q_adjust);
    return OK;
}

Result lms6002_lo_leakage_calib_recv(connection_state * state, lms6002_lo_leakage_calib * calib)
{
    calib->i_adjust = uchar_recv(state);
    calib->q_adjust = uchar_recv(state);
    return OK;
}

Result lms6002_ssb_calib_send(connection_state * state, lms6002_ssb_calib * calib)
{
    uint_send(state, calib->phase_offset);
    uint_send(state, calib->gain_offset);
    uint_send(state, calib->backoff_offset);
    return OK;
}

Result lms6002_ssb_calib_recv(connection_state * state, lms6002_ssb_calib * calib)
{
    calib->phase_offset = uint_recv(state);
    calib->gain_offset = uint_recv(state);
    calib->backoff_offset = uint_recv(state);
    return OK;
}

Result lms6002_rx_dc_calib_send(connection_state * state, lms6002_rx_dc_calib * calib)
{
    uchar_send(state, calib->i_comp);
    uchar_send(state, calib->q_comp);
    return OK;
}

Result lms6002_rx_dc_calib_recv(connection_state * state, lms6002_rx_dc_calib * calib)
{
    calib->i_comp = uchar_recv(state);
    calib->q_comp = uchar_recv(state);
    return OK;
}

