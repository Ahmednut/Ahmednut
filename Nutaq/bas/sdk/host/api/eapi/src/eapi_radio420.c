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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      eapi_radio420.c
 * 
 * @brief     Function definitions for the eapi radio420 control
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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
#include "cce.h"
#include "fmc_radio.h"
#include "eapi_radio420.h"
#include "eapi_serialize.h"
#include "fmcradio_serialize.h"

Result fmc_radio_presence(connection_state * state)
{
    cmd_send(state, FMCRADIO_INIT);
    return result_recv(state);
}

Result fmc_radio_sdr_reset_send(connection_state * state)
{
    Result ret;
    cmd_send(state, FMCRADIO_RESET);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_powerup_send(connection_state * state)
{
    Result ret;
    cmd_send(state, FMCRADIO_POWERUP);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_select_send(connection_state * state, unsigned nb)
{
    Result ret;
    nb -= 1;
    cmd_send(state, FMCRADIO_SELECT);
    uint_send(state, nb);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_setrevision_send(connection_state * state, FMCRADIO_REV revision)
{
    Result ret;
    cmd_send(state, FMCRADIO_SETREVISION);
    uint_send(state, revision);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_spi_write_send(connection_state * state, FMCRADIO_DEVICES device, unsigned addr, unsigned data)
{
    Result ret;
    cmd_send(state, FMCRADIO_SPIWRITE);
    uint_send(state, device);
    uint_send(state, addr);
    uint_send(state, data);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_spi_read_send(connection_state * state, FMCRADIO_DEVICES device, unsigned addr, unsigned * spidata)
{
    Result ret;
    cmd_send(state, FMCRADIO_SPIREAD);
    uint_send(state, device);
    uint_send(state, addr);
    connection_flush(state);
    *spidata = uint_recv(state);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_pll_setparam_send(connection_state * state, cdce62005_param * param)
{
    Result ret;
    cmd_send(state, FMCRADIO_PLLSETPARAM);
    cdce62005_param_send(state, param);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_pll_lockstatus_send(connection_state * state, int * pllstatus)
{
    Result ret;
    cmd_send(state, FMCRADIO_PLLLOCKSTATUS);
    *pllstatus = uint_recv(state);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_pll_waitforlock_send(connection_state * state)
{
	Result ret;
	cmd_send(state, FMCRADIO_PLLWAITLOCK);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_clockmux_set_send(connection_state * state, FMCRADIO_CLOCK_OUTPUT_ID dest, FMCRADIO_CLOCK_INPUT_ID src)
{
    Result ret;
    cmd_send(state, FMCRADIO_CLOCKMUXSET);
    uint_send(state, dest);
    uint_send(state, src);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_lime_setpllparam_send(connection_state * state, LMS6002_DIR dir, lms6002_pll_param * param)
{
    Result ret;
    cmd_send(state, FMCRADIO_LIMESETPLLPARAM);
    uint_send(state, dir);
    lms6002_pll_param_send(state, param);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_lime_calibratePLL_send(connection_state * state, LMS6002_DIR pll)
{
    Result ret;
    cmd_send(state, FMCRADIO_LIMEPLLCALIBRATE);
    uint_send(state, pll);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_lime_reset_send(connection_state * state)
{
    Result ret;
    cmd_send(state, FMCRADIO_LIMERESET);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_sdr_band_send(connection_state * state, FMC_RADIO_SDR_BAND band)
{
    Result ret;
    cmd_send(state, FMCRADIO_SETBAND);
    uint_send(state, band);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_sdr_rx_gain_send(connection_state * state, LMS6002_LNA_GAIN lna_gain, LMS6002_VGA1_GAIN rx_gain1, unsigned char rx_gain2, char rx_gain3)
{
    Result ret;
    cmd_send(state, FMCRADIO_SETRXGAIN);
    uint_send(state, lna_gain);
    uint_send(state, rx_gain1);
    uchar_send(state, rx_gain2);
    uchar_send(state, rx_gain3);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_rxvga_calibrate_send(connection_state * state)
{
    Result ret;
    cmd_send(state, FMCRADIO_RXVGACALIBRATE);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_tx_gain_send(connection_state * state, char tx_vga1_gain, unsigned char tx_vga2_gain, char tx_gain3)
{
    Result ret;
    cmd_send(state, FMCRADIO_SETTXGAIN);
    uchar_send(state, tx_vga1_gain);
    uchar_send(state, tx_vga2_gain);
    uchar_send(state, tx_gain3);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_sdr_setrxfilter_send(connection_state * state, FMCRADIOSDR_FILTER_BANK bank)
{
    Result ret;
    cmd_send(state, FMCRADIO_RXFILTER);
    uint_send(state, bank);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_lpf_set_send(connection_state * state, LMS6002_DIR dir, LMS6002_LPF_BANDWIDTH bwt)
{
    Result ret;
    cmd_send(state, FMCRADIO_LPFSET);
    uint_send(state, dir);
    uint_send(state, bwt);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_lpf_calibrate_send(connection_state * state,  LMS6002_DIR dir, unsigned ref_freq)
{
    Result ret;
    cmd_send(state, FMCRADIO_LPFCALIBRATE);
    uint_send(state, ref_freq);
    uint_send(state, dir);
    ret = result_recv(state);
    return ret;
}

/*Result fmc_radio_setIOexp_send(connection_state * state, unsigned data, unsigned mask)
{
    Result ret;
    cmd_send(state, FMCRADIO_SETIOX);
    uint_send(state, data);
    uint_send(state, mask);
    ret = result_recv(state);
    return ret;
}*/

Result fmc_radio_path_enable_send(connection_state * state, LMS6002_DIR dir)
{
    Result ret;
    cmd_send(state, FMCRADIO_ENABLEPATH);
    uint_send(state, dir);
    ret = result_recv(state);
    return ret;
}
Result fmc_radio_path_disable_send(connection_state * state, LMS6002_DIR dir)
{
    Result ret;
    cmd_send(state, FMCRADIO_DISABLEPATH);
    uint_send(state, dir);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_rx_connect_send(connection_state * state, LMS6002_BAND band)
{
	Result ret;
    cmd_send(state, FMCRADIO_RX_CONNECT_SET);
    uint_send(state, band);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_rx_disconnect_send(connection_state * state, LMS6002_BAND band)
{
	Result ret;
    cmd_send(state, FMCRADIO_RX_DISCONNECT_SET);
    uint_send(state, band);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_loopback_send(connection_state * state, LMS6002_LOOPBACK_MODE mode)
{
    Result ret;
    cmd_send(state, FMCRADIO_LOOPBACK);
    uint_send(state, mode);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_lo_leakage_calibration_send(connection_state * state, unsigned carrier_freq, unsigned acq_freq, lms6002_lo_leakage_calib * calib_out)
{
    Result ret;
    cmd_send(state, FMCRADIO_LO_CALIBRATE);
    uint_send(state, carrier_freq);
    uint_send(state, acq_freq);
    ret = connection_flush(state);
    if(FAILURE(ret)) return ret;
    lms6002_lo_leakage_calib_recv(state, calib_out);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_lo_leakage_calibration_set_send(connection_state * state, lms6002_lo_leakage_calib * calib)
{
    Result ret;
    cmd_send(state, FMCRADIO_LO_CALIBRATE_SET);
    lms6002_lo_leakage_calib_send(state, calib);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_ssb_calibration_send(connection_state * state, unsigned carrier_freq, unsigned acq_freq, lms6002_ssb_calib * calib_out)
{
    Result ret;
    cmd_send(state, FMCRADIO_SSB_CALIBRATE);
    uint_send(state, carrier_freq);
    uint_send(state, acq_freq);
    connection_flush(state);
    lms6002_ssb_calib_recv(state, calib_out);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_ssb_calibration_set_send(connection_state * state, lms6002_ssb_calib * calib)
{
    Result ret;
    cmd_send(state, FMCRADIO_SSB_CALIBRATE_SET);
    lms6002_ssb_calib_send(state, calib);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_rx_dc_offset_calibration_send(connection_state * state, lms6002_rx_dc_calib * calib_out)
{
    Result ret;
    cmd_send(state, FMCRADIO_RXFE_DC_CALIBRATE);
    connection_flush(state);
    lms6002_rx_dc_calib_recv(state, calib_out);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_rx_dc_offset_calibration_set_send(connection_state * state, lms6002_rx_dc_calib * calib)
{
    Result ret;
    cmd_send(state, FMCRADIO_RXFE_DC_CALIBRATE_SET);
    lms6002_rx_dc_calib_send(state, calib);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_spi_control_set_send(connection_state * state, FMCRADIO_FPGA_CTRL_LIST control, FMCRADIO_SPI_CTRL spi_controller)
{
    Result ret;
    cmd_send(state, FMCRADIO_SPI_CONTROL_SET);
    uint_send(state, control);
    uint_send(state, spi_controller);
    ret = result_recv(state);
    return ret;
}

Result fmc_radio_set_sampling_frequency(connection_state * state, FMCRADIO_CLK_SOURCE clk_src, unsigned uRefFreq, unsigned uAcqFreq, unsigned uLimeFreq)
{
    Result ret;
    cdce62005_param param;
    int pllstatus = 0;

    ret = fmc_radio_pll_calculate(&param, uRefFreq, uAcqFreq, uLimeFreq);
    if(FAILURE(ret)) return ret;

    // Setting clock source
    param.secondary_clock = (unsigned char)clk_src; 

    // Sending parameters
    ret = fmc_radio_pll_setparam_send(state, &param); 
    if(FAILURE(ret)) return ret;

    if(param.vco_bypass_all == 1)
    {
        return ret;
    }
    else
    {
        // Verifying lock
        ret = fmc_radio_pll_lockstatus_send(state, &pllstatus);
        if(FAILURE(ret)) return ret;

         // Check if the pll is locked
        if( pllstatus != CDCE62005_PLL_LOCKED )
        {
            return FMCRADIOPLLUNLOCKED;
        }
    }

    return ret;
}

Result fmc_radio_set_local_oscillator(connection_state * state, LMS6002_DIR dir, unsigned uLimeFreq, unsigned uCarrierFreq)
{
    Result ret;
    lms6002_pll_param param;

    ret = fmc_radio_lime_calculate(&param, uLimeFreq, uCarrierFreq);
    if(FAILURE(ret)) return ret;

    ret = fmc_radio_lime_setpllparam_send(state, dir, &param);
    if(FAILURE(ret)) return ret;

    ret = fmc_radio_lime_calibratePLL_send(state, dir);
    if(FAILURE(ret)) return ret;

    return ret;
}



