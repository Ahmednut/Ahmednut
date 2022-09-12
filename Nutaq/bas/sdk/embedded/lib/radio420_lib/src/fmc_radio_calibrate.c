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
 * @file       fmc_radio_calibrate.c       
 * @brief      Executes single side band and LO Leakage calibration
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup RADIO420
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
#include "fmc_radio_calibration.h"
#include "fmc_radio.h"
#include "fmc_spi.h"
#include <stddef.h>
#include "radio420_cdce62005.h"
#include "lms6002_regs.h"
#include <string.h>
#include "bas_debug.h"
#include "calibration_lut.h"
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Point the the pfRadio420_usleep function pointer define in the fmc_radio.c
extern int (*pfRadio420_usleep)(unsigned int usec);

// Used to calculate the correct value to write in register (fmc_radio->core->err_fct_dds)
// for specified frequency from RX Carrier and Data frequency
#define DFT_BYTES 16777216.

static Result fmc_radio_read_erf(fmc_radio * fmc_radio, unsigned long long *erf_out, int N)
{
    unsigned long long err_I, err_Q, sum = 0;
    int i;
    
    for (i=0; i<N; i++) 
    {
        // start the DFT and wait for the done bit
        fmc_radio->core->err_fct_ctrl.err_start = 0;
        pfRadio420_usleep(1);
        fmc_radio->core->err_fct_ctrl.err_start = 1;

        // Even if the FPGA resets the done bit with the start signal,
        // since the uBlaze clock is faster, we need to waste few cycles.
        pfRadio420_usleep(10);
        while (!fmc_radio->core->err_fct_ctrl.err_done) {;}        

        // Get DFT output
        err_I = fmc_radio->core->err_fct_I;
        err_Q = fmc_radio->core->err_fct_Q;

        // abs(real) + abs(imag) implementation
        //*erf_out = err_I + err_Q;
        
        // square version      
		//sum += sqrt((err_I)*(err_I) + (err_Q)*(err_Q));
        
        // faster not square version
        sum += (err_I)*(err_I) + (err_Q)*(err_Q);
    }
    
    *erf_out = sum/N;

    return OK;
}

/*
This is for both ADC and DAC over range
Return 0 if over range
Return 1 if not over range
*/
static int fmc_radio_check_adc_overange(fmc_radio * fmc_radio)
{
    return !((FMC_RADIO_ADC_I_OVR | FMC_RADIO_ADC_Q_OVR) & fmc_radio->core->board_control);
}

static int fmc_radio_check_dac_overange(fmc_radio * fmc_radio)
{
    return !((FMC_RADIO_DAC_I_OVR | FMC_RADIO_DAC_Q_OVR) & fmc_radio->core->board_control);
}

/*  
This function iterates for each :
 -LNA gains (Bypass (0 dB), Midgain (3 dB), Maxgain (6 dB)) 
 -RX1 gains (2 (5 dB), 102 (19 dB), 120 (30 dB))
 -RX2 gains (range from 0 to max_vga2_gain or 30, with a step of 3)
until ADC over range. (LMS6002 chip controllable gains only)
 
It starts from the lowest gains for all, and then it tests with higher gains
for each iteration and check the ADC over range.
 
If it is  ADC over range, it will set the RX gains with the LNA and RX1 gains found,
but will set the RX2 gain to (gain found - 9).
*/
static Result fmc_radio_auto_gain(fmc_radio * fmc_radio, int max_vga2_gain)
{
    Result ret;
    int ovr, vga2_gain_tmp = 0;
    unsigned int tmp_lna_gain, tmp_vga1_gain, i, j;
    LMS6002_LNA_GAIN lna_gain[3] = {LMS6002_LNA_BYPASS, LMS6002_LNA_MIDGAIN, LMS6002_LNA_MAXGAIN};
    LMS6002_VGA1_GAIN vga1_gain[3] = {LMS_VGA1_GAIN5DB, LMS_VGA1_GAIN19DB, LMS_VGA1_GAIN30DB};  

    //Setting initial RX gains to minimal
    lms6002_rx_gain(&fmc_radio->lime_core, lna_gain[0], vga1_gain[0], vga2_gain_tmp);
    
    fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
    pfRadio420_usleep(1);
    fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;
    pfRadio420_usleep(1);
    
    ovr = fmc_radio_check_adc_overange(fmc_radio);
    while (ovr) 
    {
        if (vga2_gain_tmp > max_vga2_gain)
        {
            break;
        }
        
        for(i = 0; i < 3 ; i++)
        {
            for(j = 0; j < 3 ; j++)
            {
                tmp_lna_gain = lna_gain[j];
                tmp_vga1_gain = vga1_gain[i];
                lms6002_rx_gain(&fmc_radio->lime_core, tmp_lna_gain, tmp_vga1_gain, vga2_gain_tmp);
                pfRadio420_usleep(100);
                ovr = fmc_radio_check_adc_overange(fmc_radio);
                if(!ovr)
                {
                    break;
                }
            }
            if(!ovr)
            {
                break;
            }
        }
        vga2_gain_tmp += 3;        
    }

    if(!ovr)
    {
        debug("overange detected!\n",ovr);
        vga2_gain_tmp -= 9;
        if(vga2_gain_tmp < 0) vga2_gain_tmp = 0;
        lms6002_rx_gain(&fmc_radio->lime_core, tmp_lna_gain, tmp_vga1_gain, vga2_gain_tmp);
    }
    
    debug("Autogain set LNA to %u\n", tmp_lna_gain);
    debug("Autogain set VGA1 to %u\n", tmp_vga1_gain);
    debug("Autogain set VGA2 to %i\n", vga2_gain_tmp);

    ret = fmc_radio_rxvga_calibrate(fmc_radio);
    if(FAILURE(ret))
    {
        printf("fmc_radio_rxvga_calibrate failed\n");
    }
	
    fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
	pfRadio420_usleep(1);
    fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;

    return OK;
}

Result fmc_radio_loopback(fmc_radio * fmc_radio, LMS6002_LOOPBACK_MODE mode)
{
    return lms6002_loopback(&fmc_radio->lime_core, mode);
}

Result fmc_radio_dft_freq(fmc_radio * fmc_radio, int dds_val)
{
	fmc_radio->core->err_fct_dds = dds_val;
	pfRadio420_usleep(1);
	fmc_radio->core->err_fct_ctrl.freq_wen = 1;
	pfRadio420_usleep(1);
	fmc_radio->core->err_fct_ctrl.freq_wen = 0;

    return OK;
}

/* fmc_radio_lo_leakage_calibration
Spectrum analysing :   
                            
                ^           ^
                |           |
                |           |
                |<--------->|
                |     2     |
________________|___________|_______________Freq
               RXC         TXC             (MHz)  
              X - 2         X        

where :
    - RXC   : RX carrier frequency                                      ( X - 2.0 )     [MHz]
    - TXC   : TX carrier frequency                                      ( X )           [MHz]       
    
*/
Result fmc_radio_lo_leakage_calibration(fmc_radio * fmc_radio, unsigned carrier_freq, unsigned acquisition_freq, lms6002_lo_leakage_calib * calib_out)
{
    Result ret;
    cdce62005_param cdce_pll_param;
    lms6002_lo_leakage_calib best_calib;
    lms6002_Register_Map regs;
    lms6002_pll_param lime_pll_param;
    int dds_val, read_erf_N, N = 2048;
    unsigned long long best_erf, erf_tmp;
    unsigned reference_frequency, lime_frequency, data_frequency, i, j;
    unsigned char i_tmp, q_tmp;
    unsigned char stepsize[] = {5, 2, 1};
    unsigned char loopcount[] = {51, 20, 4};
    
    printf("LO Leakage Calibration started...\n");   

    //Set frequencies with specified Radio420 Revision
    switch(fmc_radio->rev)
    {
    case FMCRADIOREVLTE_A:
    case FMCRADIOREVSDR_C:
    case FMCRADIOREVSDR_D:
    case FMCRADIOREVSPA_A:
        reference_frequency = FMC_RADIO_SDR_REVC_REFERENCE_CLOCK;
        data_frequency = acquisition_freq;
        lime_frequency = FMC_RADIO_SDR_REVC_LIME_CLOCK;
        break;
    case FMCRADIOREVSDR_A:
    case FMCRADIOREVSDR_B:
        reference_frequency = FMC_RADIO_SDR_REVB_REFERENCE_CLOCK;
        data_frequency = acquisition_freq;
        lime_frequency = FMC_RADIO_SDR_REVB_LIME_CLOCK;
        break;
    default:
        return FMCRADIOREVISION;
    }
    
    //DAC output control, Null vector - data always equal to zero    
    fmc_radio->core->dac_data_control.dac_data_sel = FMC_RADIO_DATA_NULL;

    //Activate LNA 1 (will be used during loopback (see lms6002_loopback))
    ret = fmc_radio_sdr_band(fmc_radio, FMC_RADIO_SDR_LOW_BAND);  
    if(FAILURE(ret)) return ret;
    
    //Setting lpfilter for RX and TX to LMS6002_LPF_3MHZ
    ret = fmc_radio_lpf_set(fmc_radio, LMS6002_RX, LMS6002_LPF_3MHZ);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lpf_set(fmc_radio, LMS6002_TX, LMS6002_LPF_3MHZ);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lpf_calibrate(fmc_radio, LMS6002_TXRX, lime_frequency);
    if(FAILURE(ret)) return ret;
    
    //Setting TX Lime PLL at carrier_freq
    ret = fmc_radio_lime_calculate(&lime_pll_param, lime_frequency, carrier_freq);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lime_setpllparam(fmc_radio, LMS6002_TX, &lime_pll_param);
    if(FAILURE(ret)) return ret;
    
    //Setting RX Lime PLL at (carrier_freq - 2e6)
    ret = fmc_radio_lime_calculate(&lime_pll_param, lime_frequency, carrier_freq - (unsigned)2e6);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lime_setpllparam(fmc_radio, LMS6002_RX, &lime_pll_param);
    if(FAILURE(ret)) return ret;
    
    //Calibration Lime PLL
    ret = fmc_radio_lime_calibratePLL(fmc_radio, LMS6002_RX);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lime_calibratePLL(fmc_radio, LMS6002_TX);
    if(FAILURE(ret)) return ret;
    
    //Setting RF loopback to LNA1
    ret = lms6002_loopback(&fmc_radio->lime_core, LMS6002_LOOPBACK_LNA1);
    if(FAILURE(ret)) return ret;

    //Setting RX LMS6002 gains
    ret = fmc_radio_auto_gain(fmc_radio, 18);
    if(FAILURE(ret)) return ret;
    
    //Setting RX External gain
    ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RXGAIN, 0, 0 & 0x3f); // Min External Gain of -13 to register => ((-13+13)*2) & 0x3f
    if(FAILURE(ret)) return ret;

    //Resetting lpfilter calibrate for TX and RX
    ret = fmc_radio_lpf_calibrate(fmc_radio, LMS6002_TXRX, lime_frequency);
    if(FAILURE(ret)) return ret;

    //RX VGA calibrate
    ret = fmc_radio_rxvga_calibrate(fmc_radio);
    if(FAILURE(ret)) return ret;

    //Setting RX DFT freq at TXC freq
    fmc_radio->core->err_fct_ctrl.rx_errfct_nb_point = N;    
    dds_val = (2.0e6 / (data_frequency/2)) * DFT_BYTES;
    debug("ddsval %i\n",dds_val);
    ret = fmc_radio_dft_freq(fmc_radio, dds_val);
    if(FAILURE(ret)) return ret;    
    
    //4.8 TX LO Leakage Calibration Flow Charts, Adjusted loop by SJL
    //Initialize values
    best_erf = 0xFFFFFFFFFFFFFFFFLL;
    best_calib.i_adjust = 255;
    best_calib.q_adjust = 128;
    q_tmp = 128;
    
    //Reading LO Leakage LMS6002 registers
    ret = lms6002_spi_read(&fmc_radio->lime_core, 0x42, &regs.Register_42.raw);
    ret = lms6002_spi_read(&fmc_radio->lime_core, 0x43, &regs.Register_43.raw);
    
    //Lo Leakage loop
    for (i=0; i<sizeof(loopcount); i++)
    {
        //Adjust i_tmp first value depending of loop iteration
        switch (i)
        {
        case 0:
            i_tmp = 255;
            read_erf_N = 2;
            break;
        case 1:
            i_tmp = best_calib.i_adjust+(loopcount[i]/2);
            read_erf_N = 4;
            break;
        case 2:
            i_tmp = best_calib.i_adjust+(loopcount[i]/2);
            read_erf_N = 32;
            break;
        }
        
        //Writing i_tmp and q_tmp in LMS6002 registers
        regs.Register_42.Bit.txvga1_dc_leakage_cancel_i = i_tmp;
        regs.Register_43.Bit.txvga1_dc_leakage_cancel_q = q_tmp;
        ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x43, regs.Register_43.raw);
        
        //Iteration of i_tmp with stepsize[i]
        for (j = loopcount[i]; j != 0; j--)
        {
            //Writing i_tmp in LMS6002 registers 
            regs.Register_42.Bit.txvga1_dc_leakage_cancel_i = i_tmp;
            ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x42, regs.Register_42.raw);

            //Clearing ADC overrange bit
            fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
            pfRadio420_usleep(1);
            fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;

            //Reading fft
            ret = fmc_radio_read_erf(fmc_radio, &erf_tmp, read_erf_N);
            if(FAILURE(ret)) return ret;
            
            //Checking if current_fft is better than the best so far
            if (erf_tmp < best_erf && fmc_radio_check_adc_overange(fmc_radio))
            {
                best_erf = erf_tmp;
                best_calib.i_adjust = i_tmp;
                best_calib.q_adjust = q_tmp;
                debug("best_calib: %i %i\n", i_tmp, q_tmp);
            }

            i_tmp -= stepsize[i];
        }

        //Setting i_tmp current best calib
        regs.Register_42.Bit.txvga1_dc_leakage_cancel_i = best_calib.i_adjust;
        i_tmp = best_calib.i_adjust;
        ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x42, regs.Register_42.raw);

        //Adjust q_tmp first value depending of loop iteration
        switch (i)
        {
        case 0:
            q_tmp = 255;
            read_erf_N = 2;
            break;
        case 1:
            q_tmp = best_calib.q_adjust+(loopcount[i]/2);
            read_erf_N = 4;
            break;
        case 2:
            q_tmp = best_calib.q_adjust+(loopcount[i]/2);
            read_erf_N = 32;
            break;
        }

        //Iteration of q_tmp with stepsize[i]
        for (j = loopcount[i]; j != 0; j--)
        {
            //Writing q_tmp in LMS6002 registers 
            regs.Register_43.Bit.txvga1_dc_leakage_cancel_q = q_tmp;
            ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x43, regs.Register_43.raw);

            //Clearing ADC overrange bit
            fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
            pfRadio420_usleep(1);
            fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;

            //Reading fft
            ret = fmc_radio_read_erf(fmc_radio, &erf_tmp, read_erf_N);
            if(FAILURE(ret)) return ret;            
            
            //Checking if current_fft is better than the best so far
            if (erf_tmp < best_erf && fmc_radio_check_adc_overange(fmc_radio))
            {
                best_erf = erf_tmp;
                best_calib.i_adjust = i_tmp;
                best_calib.q_adjust = q_tmp;
                debug("best_calib: %i %i\n", i_tmp, q_tmp);
            }

            q_tmp -= stepsize[i];
        }

        //Setting q_tmp current best calib for next iteration
        q_tmp = best_calib.q_adjust;
    }
    printf("\r%3d%% Complete\n",100);    
    debug("final LO leakage best_calib: %i %i\n", best_calib.i_adjust, best_calib.q_adjust);

    //Setting LO leakage best calib
    ret = fmc_radio_lo_leakage_calibration_set(fmc_radio, &best_calib);
    if(FAILURE(ret)) return ret;

    //DAC output control, User data - bypass the IQ compensation core
    fmc_radio->core->dac_data_control.dac_data_sel = FMC_RADIO_DATA_USER;
    
    //Disabling RF loopback
    ret = lms6002_loopback(&fmc_radio->lime_core, LMS6002_LOOPBACK_DISABLED);
    if(FAILURE(ret)) return ret;    
    
    //Resetting band correctly    
    if(carrier_freq <= 1500000000)
    {
        ret = fmc_radio_sdr_band(fmc_radio, FMC_RADIO_SDR_LOW_BAND);
    }
    else
    {
        ret = fmc_radio_sdr_band(fmc_radio, FMC_RADIO_SDR_HIGH_BAND);
    }   
    if(FAILURE(ret)) return ret;
    
    //Best calib out
    if( calib_out != NULL)
    {
        *calib_out = best_calib;
    }
    return ret;
}

Result fmc_radio_lo_leakage_calibration_set(fmc_radio * fmc_radio, lms6002_lo_leakage_calib * calib)
{
    Result ret;
    lms6002_Register_Map regs;

    regs.Register_42.Bit.txvga1_dc_leakage_cancel_i = calib->i_adjust;
    regs.Register_43.Bit.txvga1_dc_leakage_cancel_q = calib->q_adjust;
    
    ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x42, regs.Register_42.raw);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x43, regs.Register_43.raw);
    if(FAILURE(ret)) return ret;
    
    return ret;
}

static Result fmc_radio_adjust_backoff(fmc_radio * fmc_radio, int * backoff)
{
    int ovr, last_ovr;
    int dir;
    int last_backoff, backoff_tmp;

    backoff_tmp = last_backoff = *backoff;

    fmc_radio->core->tx_gain_imbalance_backoff = gain_lookup[backoff_tmp];
    pfRadio420_usleep(1);
    fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
    pfRadio420_usleep(1);
    fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;

    last_ovr = fmc_radio_check_dac_overange(fmc_radio);
    if(last_ovr)
    {
        dir = 1;
        debug("backoff up %i\n", last_ovr);
    }else{
        dir = -1;
        debug("backoff down %i\n", last_ovr);
    }
    do 
    {
        backoff_tmp += dir;
        if (backoff_tmp >= sizeof(gain_lookup) / sizeof(gain_lookup[0]) - 1 || backoff_tmp <= 0)
        {
            goto exit;
        }
        fmc_radio->core->tx_gain_imbalance_backoff = gain_lookup[backoff_tmp];
        pfRadio420_usleep(1);
        fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
        pfRadio420_usleep(1);
        fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;
        ovr = fmc_radio_check_dac_overange(fmc_radio);
    } while(ovr == last_ovr);
exit:
    backoff_tmp -= 3;

    if ( backoff_tmp >= sizeof(gain_lookup) / sizeof(gain_lookup[0]) - 1 )
    {
        backoff_tmp = sizeof(gain_lookup) / sizeof(gain_lookup[0]) - 1;
    }

    if (backoff_tmp <= 0)
    {
        backoff_tmp = 0;
    }

    fmc_radio->core->tx_gain_imbalance_backoff = gain_lookup[backoff_tmp];
    pfRadio420_usleep(1);
    fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
    pfRadio420_usleep(1);
    fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;

    debug("backoff value: %i\n", backoff_tmp);

    if (backoff) 
    {
        *backoff = backoff_tmp;
    }
    
    if (fmc_radio_check_dac_overange(fmc_radio))
    {
        return OK;
    }
    else
    {
        return FMCRADIOBACKOFF;
    }
}

/* fmc_radio_ssb_calibration
Spectrum analysing :   

                                        ^
                ^                       |
                |                       |
    ^           |                       |
    |<--------->|           ^           |
    |   4 - 2   |           |           |
____|___________|___________|___________|_________Freq
   RXC        TXLSB        TXC        TXDDS       (MHz)  
  X - 4       X - 2         X         X + 2

where :
    - RXC   : RX carrier frequency                                      ( X - 4 )       [MHz]
    - TXC   : TX carrier frequency                                      ( X )           [MHz]
    - TXDDS : TX signal from DAC output control, Internal DDS signal    ( X + 2 )       [MHz]
    - TXLSB : TX Lower Side Band to lower with this calibration         ( X - 2 )       [MHz]
*/
Result fmc_radio_ssb_calibration(fmc_radio * fmc_radio, unsigned carrier_freq, unsigned acquisition_freq, lms6002_ssb_calib * calib_out)
{
    Result ret;
    cdce62005_param cdce_pll_param;
    lms6002_ssb_calib best_calib;
    lms6002_pll_param lime_pll_param;
    int dds_val, read_erf_N, N = 2048;
    int phase_tmp; //each step is 0.10 degrees, check in calibration_lut.c to be sure
    int gain_tmp;  //each step is 0.01 dB, check in calibration_lut.c to be sure   
    int backoff_tmp; //each step is 0.01 dB, check in calibration_lut.c to be sure   
    unsigned long long best_erf, erf_tmp;
    unsigned reference_frequency, lime_frequency, data_frequency, i, j;    
    unsigned char erf_loop[] = {30, 61, 11};
    unsigned char stepsize_gain[] = {5, 2, 1};
    unsigned char stepsize_phase[] = {10, 2, 1};
    
    printf("SSB Calibration started...\n");

    //Set frequencies with specified Radio420 Revision
    switch(fmc_radio->rev)
    {
    case FMCRADIOREVLTE_A:
    case FMCRADIOREVSDR_C:
    case FMCRADIOREVSDR_D:
    case FMCRADIOREVSPA_A:
        reference_frequency = FMC_RADIO_SDR_REVC_REFERENCE_CLOCK;
        data_frequency = acquisition_freq;
        lime_frequency = FMC_RADIO_SDR_REVC_LIME_CLOCK;
        break;
    case FMCRADIOREVSDR_A:
    case FMCRADIOREVSDR_B:
        reference_frequency = FMC_RADIO_SDR_REVB_REFERENCE_CLOCK;
        data_frequency = acquisition_freq;
        lime_frequency = FMC_RADIO_SDR_REVB_LIME_CLOCK;
        break;
    default:
        return FMCRADIOREVISION;
    }
    
    //DAC output control, Null vector - data always equal to zero    
    fmc_radio->core->dac_data_control.dac_data_sel = FMC_RADIO_DATA_NULL;

    //Activate LNA 1 (will be used during loopback (see lms6002_loopback))
    ret = fmc_radio_sdr_band(fmc_radio, FMC_RADIO_SDR_LOW_BAND);  
    if(FAILURE(ret)) return ret;

    //Setting lpfilter for RX and TX to LMS6002_LPF_3MHZ
    ret = fmc_radio_lpf_set(fmc_radio, LMS6002_RX, LMS6002_LPF_3MHZ);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lpf_set(fmc_radio, LMS6002_TX, LMS6002_LPF_3MHZ);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lpf_calibrate(fmc_radio, LMS6002_TXRX, lime_frequency);
    if(FAILURE(ret)) return ret;     

    //Setting TX Lime PLL at carrier_freq ; 
    ret = fmc_radio_lime_calculate(&lime_pll_param, lime_frequency, carrier_freq);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lime_setpllparam(fmc_radio, LMS6002_TX, &lime_pll_param);
    if(FAILURE(ret)) return ret;
    
    //Setting RX Lime PLL away from TX at (carrier_freq - 4e6) 
    ret = fmc_radio_lime_calculate(&lime_pll_param, lime_frequency, carrier_freq - (unsigned)4e6);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lime_setpllparam(fmc_radio, LMS6002_RX, &lime_pll_param);
    if(FAILURE(ret)) return ret;
    
    //Calibration Lime PLL
    ret = fmc_radio_lime_calibratePLL(fmc_radio, LMS6002_RX);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_lime_calibratePLL(fmc_radio, LMS6002_TX);
    if(FAILURE(ret)) return ret;    

    //TX DAC output control, Internal DDS signal compensated for IQ gain imbalance and IQ phase imbalance
    fmc_radio->core->dac_data_control.dds_freq = (unsigned)( 2 * 2e6 * DFT_BYTES / data_frequency );
    fmc_radio->core->dac_data_control.dac_data_sel = FMC_RADIO_DATA_DDS_CALIBRATED;
    debug("ddsval %i\n",fmc_radio->core->dac_data_control.dds_freq);
    
    //Setting RF loopback to LNA1
    ret = lms6002_loopback(&fmc_radio->lime_core, LMS6002_LOOPBACK_LNA1);
    if(FAILURE(ret)) return ret;

    //Setting RX LMS6002 gains
    ret = fmc_radio_auto_gain(fmc_radio, 18);
    if(FAILURE(ret)) return ret;
    
    //Setting RX External gain 
    ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RXGAIN, 0, 0 & 0x3f); // Min External Gain of -13 to register => ((-13+13)*2) & 0x3f
    if(FAILURE(ret)) return ret;
    
    //Setting RX DFT freq at TX Lower Side Band
    fmc_radio->core->err_fct_ctrl.rx_errfct_nb_point = N;
    dds_val = ((4e6 - 2e6) / (data_frequency/2)) * DFT_BYTES;
    debug("ddsval %i\n",dds_val);
    ret = fmc_radio_dft_freq(fmc_radio, dds_val);
    if(FAILURE(ret)) return ret;   
    
    //4.9 TX Single Side Band Calibration Flow Charts, Adjusted loop by SJL    
    // Initialize phase and gain to 0 dB/degrees
    best_erf = 0xFFFFFFFFFFFFFFFFLL;
    gain_tmp = sizeof(gain_lookup) / sizeof(gain_lookup[0]) - 6;     // 0 dB
    phase_tmp = sizeof(phase_lookup) / sizeof(phase_lookup[0]) - 1;  // 0 degrees
    best_calib.gain_offset = gain_lookup[gain_tmp];
    best_calib.phase_offset = phase_lookup[phase_tmp]; 

    // Setting Initial Backoff Offset
    backoff_tmp = 7; // -0.73 dB
    best_calib.backoff_offset = backoff_tmp;
    fmc_radio->core->tx_gain_imbalance_backoff = gain_lookup[best_calib.backoff_offset];
    
    // 3 loop iteration to find best SSB calib 
    for (i=0; i<sizeof(erf_loop); i++)
    {
        // Initialize gain_tmp and read_erf_N depending of iteration of i
        switch(i)
        {
        case 0:
            read_erf_N = 2;
            break;
        case 1:
            //Prevent gain_tmp to exceed max index
            if (best_calib.gain_offset + (erf_loop[i]/2) > sizeof(gain_lookup) / sizeof(gain_lookup[0]) -1) 
            {
                gain_tmp = sizeof(gain_lookup) / sizeof(gain_lookup[0]) - 1;
            }
            else
            {
                gain_tmp = best_calib.gain_offset + (erf_loop[i]/2);
            }
            best_erf = 0xFFFFFFFFFFFFFFFFLL; // Reset best_erf
            read_erf_N = 4;           
            break;
        case 2:
            //Prevent gain_tmp to exceed max index
            if (best_calib.gain_offset + (erf_loop[i]/2) > sizeof(gain_lookup) / sizeof(gain_lookup[0]) -1) 
            {
                gain_tmp = sizeof(gain_lookup) / sizeof(gain_lookup[0]) - 1;
            }
            else
            {
                gain_tmp = best_calib.gain_offset + (erf_loop[i]/2);
            }            
            read_erf_N = 64;
            break;
        }

        // Set phase
        fmc_radio->core->tx_phase_imbalance = phase_lookup[phase_tmp];
        
        // Adjust gain
        for (j = erf_loop[i]; j != 0; j--)
        {
            //Setting tx_gain_imbalance in fmc_radio core 
            fmc_radio->core->tx_gain_imbalance = gain_lookup[gain_tmp];
            
            //Clearing ADC overrange bit
            fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
            pfRadio420_usleep(1);
            fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;

            //Reading fft
            ret = fmc_radio_read_erf(fmc_radio, &erf_tmp, read_erf_N);
            if(FAILURE(ret)) return ret;

            //Checking if current_fft is better than the best so far
            if (erf_tmp < best_erf)
            {
                best_erf = erf_tmp;
                best_calib.gain_offset = gain_tmp;
                best_calib.phase_offset = phase_tmp;                
                debug("best_calib: %i %i\n", gain_tmp, phase_tmp);
            }
            
            //Prevent gain_tmp to be negative
            if (gain_tmp - stepsize_gain[i] < 0)
            {
                break;
            }
            else
            {
                gain_tmp -= stepsize_gain[i];
            }            
        }

        // Set best gain so far
        fmc_radio->core->tx_gain_imbalance = gain_lookup[best_calib.gain_offset];
        gain_tmp = best_calib.gain_offset;
        
        // Initialize phase_tmp and read_erf_N depending of iteration of i
        switch(i)
        {
        case 0:
            phase_tmp = sizeof(phase_lookup) / 2 - 2;
            read_erf_N = 2; 
            break;
        case 1:
            //Prevent phase_tmp to exceed max index
            if (best_calib.phase_offset + (erf_loop[i]/2) > sizeof(phase_lookup) / 2 - 1) 
            {
                phase_tmp = sizeof(phase_lookup) / 2 - 1;
            }
            else
            {
                phase_tmp = best_calib.phase_offset + (erf_loop[i]/2);
            }
            read_erf_N = 4;             
            break;
        case 2:
            //Prevent phase_tmp to exceed max index
            if (best_calib.phase_offset + (erf_loop[i]/2) > sizeof(phase_lookup) / 2 - 1) 
            {
                phase_tmp = sizeof(phase_lookup) / 2 - 1;
            }
            else
            {
                phase_tmp = best_calib.phase_offset + (erf_loop[i]/2);
            }
            read_erf_N = 64;             
            break;
        }
        
        // Adjust phase
        for (j = erf_loop[i]; j != 0; j--)
        {
            //Setting tx_phase_imbalance in fmc_radio core
            fmc_radio->core->tx_phase_imbalance = phase_lookup[phase_tmp];
            
            //Clearing ADC overrange bit
            fmc_radio->core->board_control |= FMC_RADIO_OVR_CLEAR;
            pfRadio420_usleep(1);
            fmc_radio->core->board_control &= ~FMC_RADIO_OVR_CLEAR;
            
            //Reading fft
            ret = fmc_radio_read_erf(fmc_radio, &erf_tmp, read_erf_N);
            if(FAILURE(ret)) return ret;

            //Checking if current_fft is better than the best so far
            if (erf_tmp < best_erf)
            {
                best_erf = erf_tmp;
                best_calib.gain_offset = gain_tmp;
                best_calib.phase_offset = phase_tmp;
                debug("best_calib: %i %i\n", gain_tmp, phase_tmp);
            } 
            
            //Prevent phase_tmp to be negative
            if (phase_tmp - stepsize_phase[i] < 0)
            {
                break;
            }
            else
            {
                phase_tmp -= stepsize_phase[i];
            }          
        }

         // Set best phase so far
        fmc_radio->core->tx_phase_imbalance = phase_lookup[best_calib.phase_offset];
        phase_tmp = best_calib.phase_offset;
    }
    printf("\r%3d%% Complete\n",100);  
    
    //Setting SSB best calib (Gain and Phase)
    ret = fmc_radio_ssb_calibration_set(fmc_radio, &best_calib);
    if(FAILURE(ret)) return ret;
    
    //Setting Best Backoff offset
    ret = fmc_radio_adjust_backoff(fmc_radio, &backoff_tmp);
    //if(FAILURE(ret)) return ret;
    best_calib.backoff_offset = backoff_tmp;   

    //Priting final SSB best_calib
    debug("final SSB best_calib: %i %i %i\n", best_calib.gain_offset, best_calib.phase_offset, best_calib.backoff_offset);
    
    //DAC output control, User data compensated for IQ gain imbalance and IQ phase imbalance
    fmc_radio->core->dac_data_control.dac_data_sel = FMC_RADIO_DATA_USER_CALIBRATED;

    //Disabling RF loopback
    ret = lms6002_loopback(&fmc_radio->lime_core, LMS6002_LOOPBACK_DISABLED);
    if(FAILURE(ret)) return ret;
    
    //Resetting band correctly    
    if(carrier_freq <= 1500000000)
    {
        ret = fmc_radio_sdr_band(fmc_radio, FMC_RADIO_SDR_LOW_BAND);
    }
    else
    {
        ret = fmc_radio_sdr_band(fmc_radio, FMC_RADIO_SDR_HIGH_BAND);
    }   
    if(FAILURE(ret)) return ret;

    //Best calib out
    if (calib_out != NULL)
    {
        *calib_out = best_calib;
    }
    return ret;
}

Result fmc_radio_ssb_calibration_set(fmc_radio * fmc_radio, lms6002_ssb_calib * calib)
{
    fmc_radio->core->tx_gain_imbalance = gain_lookup[calib->gain_offset];
    fmc_radio->core->tx_phase_imbalance = phase_lookup[calib->phase_offset];
    fmc_radio->core->tx_gain_imbalance_backoff = gain_lookup[calib->backoff_offset];
    return OK;
}

static Result fmc_radio_read_dc(fmc_radio * fmc_radio, LMS6002_CHANNEL channel, int *dc_out)
{
    Result ret = FMCRADIOERFTIMEOUT;
    int dc_tmp, diff_abs, last_dc;
    int i;
    const int min_dc_threshold = 2;
    const int ratio_dc_threshold = 300;
    const int dc_loop_count = 1000;
    int threshold, div_threshold;
    
    last_dc = channel == LMS6002_CHANNEL_I ? fmc_radio->core->rx_dc_offset.i_dc_offset : fmc_radio->core->rx_dc_offset.q_dc_offset;

    for(i = dc_loop_count; i != 0; i--)
    {
        pfRadio420_usleep(10);
        dc_tmp = channel == LMS6002_CHANNEL_I ? fmc_radio->core->rx_dc_offset.i_dc_offset : fmc_radio->core->rx_dc_offset.q_dc_offset;
        diff_abs = dc_tmp - last_dc < 0 ? last_dc - dc_tmp : dc_tmp - last_dc;
        div_threshold = dc_tmp / ratio_dc_threshold;
        threshold = div_threshold > min_dc_threshold ? div_threshold : min_dc_threshold;
        if (diff_abs < threshold)
        {
            ret = OK;
            break;
        }
        last_dc = dc_tmp;
    }
    *dc_out = dc_tmp;
    return ret;
}

#define ABS(a)	   (((a) < 0) ? -(a) : (a))

static unsigned char dc_correction2reg(int dc_correction)
{
    unsigned char abs_correction;

    abs_correction = ABS(dc_correction);

    if(abs_correction >= 64) abs_correction=63;

    return dc_correction < 0 ? abs_correction | 0x40 : abs_correction;
}

Result fmc_radio_rx_dc_offset_calibration(fmc_radio * fmc_radio, lms6002_rx_dc_calib * calib_out)
{
    Result ret = OK;
    lms6002_Register_Map regs;    
    int dc_offset, dc_offset_min_val_I, dc_offset_min_val_Q, dc_offset_min_index = 0, total_correction = 0;
    unsigned int completed_loop = 0, total_loop = 256;
    const int dc_offset_goal = 100;
    unsigned char tmp_selout; //To know the output buffer in RX PLL
    unsigned char dac_datasel_save = fmc_radio->core->dac_data_control.dac_data_sel;
    
    printf("DC Offset Calibration started...\n");
    
    //Deactivate signal from PLL output buffer to prevent RX DC offset calibration to fail
    ret = lms6002_spi_read(&fmc_radio->lime_core, 0x25, &regs.Register_25.raw);
    if(FAILURE(ret)) return ret;
    tmp_selout = (unsigned char)regs.Register_25.Bit.selout;    
    
    regs.Register_25.Bit.selout = 0; // All output buffers for RX PLL are powered down
    ret = lms6002_spi_write(&fmc_radio->lime_core, 0x25, regs.Register_25.raw);
    if(FAILURE(ret)) return ret;  
    
    //DAC output control, Null vector - data always equal to zero
    fmc_radio->core->dac_data_control.dac_data_sel = FMC_RADIO_DATA_NULL;
    pfRadio420_usleep(100);

    ret = lms6002_spi_read(&fmc_radio->lime_core, 0x71, &regs.Register_71.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_read(&fmc_radio->lime_core, 0x72, &regs.Register_72.raw);
    if(FAILURE(ret)) return ret;
    
    // Calculate channel I DC offset for all values
    dc_offset_min_val_I = 4000;
    for(total_correction=-64; total_correction<64; total_correction++)
    {
        regs.Register_71.Bit.dc_offset_cancel_i = dc_correction2reg(total_correction);
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x71, regs.Register_71.raw);
        pfRadio420_usleep(100);
        ret = fmc_radio_read_dc(fmc_radio, LMS6002_CHANNEL_I, &dc_offset);
        //debug("total_correction = %d, dc_offset_I = %d\n",total_correction,dc_offset);
        if (ABS(dc_offset) < ABS(dc_offset_min_val_I))
        {
            dc_offset_min_val_I = dc_offset;
            dc_offset_min_index = total_correction;
        }
        completed_loop++;
        printf("\r%3d%% Complete",(completed_loop*100)/total_loop);
        fflush(stdout);
    }

    // Set index for mininum value I
    debug("dc_offset_min_index = %d, dc_offset_min_val_I = %d\n",dc_offset_min_index,dc_offset_min_val_I);
    regs.Register_71.Bit.dc_offset_cancel_i = dc_correction2reg(dc_offset_min_index);
    ret = lms6002_spi_write(&fmc_radio->lime_core, 0x71, regs.Register_71.raw);
    
    // Calculate channel Q DC offset for all values
    dc_offset_min_val_Q = 4000;
    for(total_correction=-64; total_correction<64; total_correction++)
    {
        regs.Register_72.Bit.dc_offset_cancel_q = dc_correction2reg(total_correction);
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x72, regs.Register_72.raw);
        pfRadio420_usleep(100);
        ret = fmc_radio_read_dc(fmc_radio, LMS6002_CHANNEL_Q, &dc_offset);
        //debug("total_correction = %d, dc_offset_Q = %d\n",total_correction,dc_offset);
        if (ABS(dc_offset) < ABS(dc_offset_min_val_Q))
        {
            dc_offset_min_val_Q = dc_offset;
            dc_offset_min_index = total_correction;
        }
        completed_loop++;
        printf("\r%3d%% Complete",(completed_loop*100)/total_loop);
        fflush(stdout);
    }
    printf("\r%3d%% Complete\n",100);
    
    // Set index for mininum value Q
    debug("dc_offset_min_index = %d, dc_offset_min_val_Q = %d\n",dc_offset_min_index,dc_offset_min_val_Q);
    regs.Register_72.Bit.dc_offset_cancel_q = dc_correction2reg(dc_offset_min_index);
    ret = lms6002_spi_write(&fmc_radio->lime_core, 0x72, regs.Register_72.raw);

    //Best calib out
    if(calib_out != NULL)
    {
        calib_out->i_comp = regs.Register_71.Bit.dc_offset_cancel_i;
        calib_out->q_comp = regs.Register_72.Bit.dc_offset_cancel_q;
    }

    //DAC output control from saved
    fmc_radio->core->dac_data_control.dac_data_sel = dac_datasel_save;
    
    //Reactivate signal from PLL output buffer
    ret = lms6002_spi_read(&fmc_radio->lime_core, 0x25, &regs.Register_25.raw);
    if(FAILURE(ret)) return ret;    
    
    regs.Register_25.Bit.selout = tmp_selout; // Re-set from previous tmp_selout RX PLL output buffer
    ret = lms6002_spi_write(&fmc_radio->lime_core, 0x25, regs.Register_25.raw);
    if(FAILURE(ret)) return ret;
    
    pfRadio420_usleep(100);    

    if((ABS(dc_offset_min_val_I) > dc_offset_goal) || (ABS(dc_offset_min_val_Q) > dc_offset_goal))
    {
        return FMCRADIODCCALFAILED;
    }    
    else
    {
        return OK;
    }
}

Result fmc_radio_rx_dc_offset_calibration_set(fmc_radio * fmc_radio, lms6002_rx_dc_calib * calib)
{
    Result ret = OK;
    lms6002_Register_Map regs;    

    ret = lms6002_spi_read(&fmc_radio->lime_core, 0x71, &regs.Register_71.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_read(&fmc_radio->lime_core, 0x72, &regs.Register_72.raw);
    if(FAILURE(ret)) return ret;
    
    regs.Register_71.Bit.dc_offset_cancel_i = calib->i_comp;
    regs.Register_72.Bit.dc_offset_cancel_q = calib->q_comp;
    
    ret = lms6002_spi_write(&fmc_radio->lime_core, 0x71, regs.Register_71.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_write(&fmc_radio->lime_core, 0x72, regs.Register_72.raw);
    if(FAILURE(ret)) return ret;
    
    return ret;  
}
