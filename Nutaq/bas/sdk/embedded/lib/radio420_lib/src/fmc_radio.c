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
 * @file       fmc_radio.c      
 * @brief      Calculate valid configuration and configure the Radio420X and
 *             the Radio421X
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
#include "fmc_radio.h"
#include "fmc_spi.h"
#include <stddef.h>
#include "radio420_cdce62005.h"
#include "lms6002_regs.h"
#include <string.h>
#include "bas_debug.h"
#include <limits.h>
#include <math.h>

int (*pfRadio420_usleep)(unsigned int usec);

Result fmc_radio_setrevision(fmc_radio * fmc_radio, FMCRADIO_REV revision)
{
    fmc_radio->rev = revision;
    return OK;
}


Result fmc_radio_pll_calculate(cdce62005_param * param, unsigned ref_freq, unsigned adac_freq, unsigned lime_freq)
{
    memset(param, 0, sizeof(cdce62005_param));
    param->ref_freq = ref_freq ? ref_freq : FMC_RADIO_SDR_REVC_REFERENCE_CLOCK;
    param->out_freq[0] = lime_freq;
    param->out_freq[1] = adac_freq;
    param->out_freq[2] = adac_freq;
    param->out_freq[3] = lime_freq;
    param->out_freq[4] = adac_freq;
    param->pfd_freq_min = CDCE62005_PFD_MIN;
    param->pfd_freq_max = CDCE62005_PFD_MAX;
    return cdce62005_calculate(param);
}

Result fmc_radio_pll_calculate_withpfd(cdce62005_param * param, unsigned ref_freq, unsigned pfd_freq, unsigned adac_freq, unsigned lime_freq)
{
    memset(param, 0, sizeof(cdce62005_param));
    param->ref_freq = ref_freq ? ref_freq : FMC_RADIO_SDR_REVC_REFERENCE_CLOCK;
    param->out_freq[0] = lime_freq;
    param->out_freq[1] = adac_freq;
    param->out_freq[2] = adac_freq;
    param->out_freq[3] = lime_freq;
    param->out_freq[4] = adac_freq;
    param->pfd_freq_min = pfd_freq;
    param->pfd_freq_max = pfd_freq;
    return cdce62005_calculate(param);
}

Result fmc_radio_lime_calculate(lms6002_pll_param * param, unsigned ref_freq, unsigned carrier_freq)
{
    param->out_freq = carrier_freq;
    param->ref_freq = ref_freq;
    return lms6002_pll_calculate(param);
}

#ifndef __EAPI_BUILD__
Result fmc_radio_sdr_open(fmc_radio * fmc_radio, fmc_radio_core * base_addr, FMCRADIO_REV revision, int (*pfUsleep)(unsigned int usec))
{
    if(pfUsleep == NULL) return ERR_INVPTR;
    
    pfRadio420_usleep = pfUsleep;
    
    fmc_radio->core = base_addr;
    fmc_radio->spimode = SPI_BLOCKING;
    fmc_radio_setrevision(fmc_radio, revision);
    //io expander and pll share the same SPI bus
    spi_core_open(&fmc_radio->pll, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, pll_ctrl_spi_data), 0x100, 0x400);
    spi_core_open(&fmc_radio->iox, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, pll_ctrl_spi_data), 0x200, 0x400);

    spi_core_open(&fmc_radio->radio, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, lime_spi_data), 0x1, 0x2);
    spi_core_open(&fmc_radio->txgain, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, tx_gain_spi_data), 0x4, 0x8);
    spi_core_open(&fmc_radio->rxgain, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, rx_gain_spi_data), 0x10, 0x80);
    spi_core_open(&fmc_radio->tcvcxo, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, ref_dac_spi_data), 0x800, 0x1000);

    lms6002_open(&fmc_radio->lime_core, &fmc_radio->radio);

    fmc_radio->cpld_reg_shadow = 0;

    return OK;
}

Result fmc_radio_lte_open(fmc_radio * fmc_radio, fmc_radio_core * base_addr)
{
    fmc_radio->core = base_addr;
    fmc_radio->spimode = SPI_BLOCKING;
    fmc_radio_setrevision(fmc_radio, FMCRADIOREVLTE_A);
    //io expander and pll share the same SPI bus
    spi_core_open(&fmc_radio->pll, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, pll_ctrl_spi_data), 0x100, 0x400);
    spi_core_open(&fmc_radio->iox, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, pll_ctrl_spi_data), 0x200, 0x400);

    spi_core_open(&fmc_radio->radio, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, lime_spi_data), 0x1, 0x2);
    spi_core_open(&fmc_radio->txgain, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, tx_gain_spi_data), 0x4, 0x8);
    spi_core_open(&fmc_radio->rxgain, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, rx_gain_spi_data), 0x10, 0x80);
    spi_core_open(&fmc_radio->tcvcxo, ((char*)base_addr) + offsetof(fmc_radio_core, spi_ctrl), ((char*)base_addr) + offsetof(fmc_radio_core, ref_dac_spi_data), 0x800, 0x1000);

    lms6002_open(&fmc_radio->lime_core, &fmc_radio->radio);

    fmc_radio->cpld_reg_shadow = 0;

    return OK;
}


Result fmc_radio_enable_clock(fmc_radio * fmc_radio)
{
    fmc_radio->core->board_control |= FMC_RADIO_DESIGN_CLK_EN;
    
    fmc_radio->core->board_control |= FMC_RADIO_FIFO_RESET;
    pfRadio420_usleep(1);
    fmc_radio->core->board_control &= ~FMC_RADIO_FIFO_RESET;
    return OK;
}

Result fmc_radio_disable_clock(fmc_radio * fmc_radio)
{
    fmc_radio->core->board_control &= ~FMC_RADIO_DESIGN_CLK_EN;
    fmc_radio->core->board_control |= FMC_RADIO_FIFO_RESET;
    return OK;
}


Result fmc_radio_sdr_reset(fmc_radio * fmc_radio)
{
    Result ret;
	unsigned int udata;
	
	//Check LMS6002 Chip Version and Revision to see if a Radio is really connected to the Carrier
	ret = fmc_radio_spi_read(fmc_radio, FMCRADIO_RF, 0x4, &udata);
	if(FAILURE(ret)) return ret;
	if(udata == 0xFF) return FMCRADIOABSENT; //Normally, udata should be 0x22

    //Disable design clock and hold the ADC and DAC FIFO in reset
    fmc_radio_disable_clock(fmc_radio);
    
    //Reset Radio420 FPGA core
    fmc_radio->core->board_control |= FMC_RADIO_CORE_RESET_PULSE;

    //Reset and disable IO of Lime 6002
    fmc_radio->core->board_control &= ~(FMC_RADIO_LIME_ENABLE | FMC_RADIO_LIME_TX_EN | FMC_RADIO_LIME_RX_EN);

    //turn off CDCE62005 PLL
    fmc_radio_setIOexp( fmc_radio, FMCRADIO_CPLD_PLLSYNC | FMCRADIO_CPLD_PLLREFSEL, FMCRADIO_CPLD_PLL_EN );

    pfRadio420_usleep(1000);   //delay loop

    //turn on CDCE62005 PLL
    fmc_radio_setIOexp(fmc_radio, FMCRADIO_CPLD_PLLSYNC |FMCRADIO_CPLD_PLL_EN | FMCRADIO_CPLD_PLLREFSEL, 0);
    pfRadio420_usleep(10000);   //delay loop min 50ms.

    cdce62005_flush_register(&fmc_radio->pll);

    pfRadio420_usleep(5000);

    fmc_radio->core->board_control |= FMC_RADIO_LIME_ENABLE | FMC_RADIO_LIME_TX_EN | FMC_RADIO_LIME_RX_EN ;

    pfRadio420_usleep(10000);

    ret = lms6002_reset(&fmc_radio->lime_core);
    if(FAILURE(ret)) return ret;

    if(fmc_radio->rev == FMCRADIOREVSDR_C || fmc_radio->rev == FMCRADIOREVSDR_D)
    {
        ret = fmc_radio_sdr_setrxfilter(fmc_radio, FMCRADIOSDR_REVC_FILTER_BYPASS);
    }
    else
    {
        ret = fmc_radio_sdr_setrxfilter(fmc_radio, FMCRADIOSDR_REVB_FILTER_NONE);
    }
    if(FAILURE(ret)) return ret;
	
	//Disable RX and TX by default
	ret = fmc_radio_path_disable(fmc_radio, LMS6002_TXRX);
	if(FAILURE(ret)) return ret;
    
    return ret;
}

#ifndef DISABLE_FMC_RADIO_LTE
Result fmc_radio_lte_reset(fmc_radio * fmc_radio)
{
    Result ret;

    //Reset and disable IO of LMS6002
    fmc_radio->core->board_control &= ~(FMC_RADIO_LIME_ENABLE | FMC_RADIO_LIME_TX_EN | FMC_RADIO_LIME_RX_EN);

    //turn off CDCE62005 PLL
    fmc_radio_setIOexp( fmc_radio, FMCRADIO_CPLD_PLLSYNC | FMCRADIO_CPLD_PLLREFSEL, FMCRADIO_CPLD_PLL_EN );

    pfRadio420_usleep(1000);   //delay loop

    //turn on CDCE62005 PLL
    fmc_radio_setIOexp(fmc_radio, FMCRADIO_CPLD_PLLSYNC |FMCRADIO_CPLD_PLL_EN | FMCRADIO_CPLD_PLLREFSEL, 0);
    pfRadio420_usleep(10000);   //delay loop min 50ms.

    cdce62005_flush_register(&fmc_radio->pll);

    ret = fmc_radio_pll_waitforlock(fmc_radio);
	if(FAILURE(ret)) return ret;


    fmc_radio->core->board_control |= FMC_RADIO_LIME_ENABLE | FMC_RADIO_LIME_TX_EN | FMC_RADIO_LIME_RX_EN ;

    pfRadio420_usleep(10000);

    ret = lms6002_reset(&fmc_radio->lime_core);

    return ret;
}
#endif /*DISABLE_FMC_RADIO_LTE*/

Result fmc_radio_sdr_close(fmc_radio * fmc_radio)
{
    lms6002_close(&fmc_radio->lime_core);
    spi_core_close(&fmc_radio->pll);
    spi_core_close(&fmc_radio->radio);
    spi_core_close(&fmc_radio->rxgain);
    spi_core_close(&fmc_radio->txgain);
    spi_core_close(&fmc_radio->tcvcxo);
    return OK;
}

#ifndef DISABLE_FMC_RADIO_LTE
Result fmc_radio_lte_close(fmc_radio * fmc_radio)
{
    lms6002_close(&fmc_radio->lime_core);
    spi_core_close(&fmc_radio->pll);
    spi_core_close(&fmc_radio->radio);
    spi_core_close(&fmc_radio->rxgain);
    spi_core_close(&fmc_radio->txgain);
    spi_core_close(&fmc_radio->tcvcxo);
    return OK;
}
#endif /*DISABLE_FMC_RADIO_LTE*/

Result fmc_radio_tune_osc(fmc_radio * fmc_radio, float fValue)
{
    Result res;
    unsigned uValue;
    
    if(fmc_radio == NULL) return FMCRADIOINVPTR;
    
    if(fValue < -1.0f || fValue > 1.0f) return FMCRADIOARGOUTOFRANGE;
    
    uValue = (unsigned)((fValue + 1.5f)/ 2.5f * 65535.0f);

    res = fmc_radio_spi_write(fmc_radio, FMCRADIO_TCVCXODAC,0,uValue);
    if(FAILURE(res)) return res;

    return FMCRADIOOK;
}

Result fmc_radio_spi_write(fmc_radio * fmc_radio, FMCRADIO_DEVICES device, unsigned addr, unsigned data)
{
    Result ret;
    spi_core_t * spi_core;

    switch(device)
    {
    case FMCRADIO_RF:
        spi_core = &fmc_radio->radio;
        ret = spi_core_complete(spi_core);
        if(FAILURE(ret)) return ret;
        ret = lms6002_spi_write(&fmc_radio->lime_core, addr, data);
        if(FAILURE(ret)) return ret;
        break;

    case FMCRADIO_TXGAIN:
        spi_core = &fmc_radio->txgain;
        ret = spi_core_write(&fmc_radio->txgain, data);
        break;

    case FMCRADIO_RXGAIN:
        spi_core = &fmc_radio->rxgain;
        ret = spi_core_write(&fmc_radio->rxgain, data);
        break;

    case FMCRADIO_PLL:
        spi_core = &fmc_radio->pll;
        ret = cdce62005_spi_write(&fmc_radio->pll, addr, data);
        if(FAILURE(ret)) return ret;
        break;

    case FMCRADIO_TCVCXODAC:
        spi_core = &fmc_radio->tcvcxo;
        ret = spi_core_write(&fmc_radio->tcvcxo, data);
        break;

    case FMCRADIO_CPLD:
        spi_core = &fmc_radio->iox;
        ret = spi_core_write(&fmc_radio->iox, (data << 4) | addr);
        break;

    default:
        return FMCRADIOIVAL;
    }

    if(fmc_radio->spimode == SPI_BLOCKING)
    {
        ret = spi_core_complete(spi_core);
    }

    return ret;
}

Result fmc_radio_spi_read(fmc_radio * fmc_radio, FMCRADIO_DEVICES device, unsigned addr, unsigned * data)
{
    Result ret=OK;
    unsigned char cdata;
    switch(device)
    {
    case FMCRADIO_PLL:
        ret = cdce62005_spi_read(&fmc_radio->pll, addr, data);
        break;
    case FMCRADIO_RF:
        ret = lms6002_spi_read(&fmc_radio->lime_core, addr, &cdata);
        *data = cdata;
        break;
    default:
        ret = FMCRADIONOTSUPPORTED;
        break;
    }
    return ret;
}

/****************************************************************************
 * Function: fmc_radio_spi_mode
 *****************************************************************************
 *
 * Set access mode to SPI device : blocking or non-blocking IO
 *
 * @param[inout] fmc_radio pointer to fmc_radio struct
 *
 * @param[in] mode Mode: blocking or non-blocking IO
 *
 * @return
 *      return 0 on success.
 *
 *****************************************************************************/
Result fmc_radio_spi_mode(fmc_radio * fmc_radio, spi_mode mode)
{
    fmc_radio->spimode = mode;
    return OK;
}

/****************************************************************************
 * Function: fmc_radio_spi_complete
 *****************************************************************************
 *
 * Check if SPI transaction is complete (Use when in non-blocking mode).
 *
 * @param[inout] fmc_radio pointer to fmc_radio struct
 *
 * @param[in] device Device ID
 *
 * @return
 *      return 0 on success.
 *
 *****************************************************************************/
Result fmc_radio_spi_complete(fmc_radio * fmc_radio, FMCRADIO_DEVICES device)
{
    spi_core_t * spi_core;

    switch(device)
    {
    case FMCRADIO_RF:
        spi_core = &fmc_radio->radio;
        break;

    case FMCRADIO_TXGAIN:
        spi_core = &fmc_radio->txgain;
        break;

    case FMCRADIO_RXGAIN:
        spi_core = &fmc_radio->rxgain;
        break;

    case FMCRADIO_PLL:
        spi_core = &fmc_radio->pll;
        break;

    case FMCRADIO_TCVCXODAC:
        spi_core = &fmc_radio->tcvcxo;
        break;

    case FMCRADIO_CPLD:
        spi_core = &fmc_radio->iox;
        break;

    default:
        return FMCRADIOIVAL;
    }

    return spi_core_complete(spi_core);
}

Result fmc_radio_clockmux_set(fmc_radio * fmc_radio, FMCRADIO_CLOCK_OUTPUT_ID dest, FMCRADIO_CLOCK_INPUT_ID src)
{
    int InVal, OutVal;
    volatile unsigned int* regPtr = (volatile unsigned int*)&fmc_radio->core->board_control;
    unsigned int save_clk_en;


    switch (dest)
    {
    case FMCRADIO_CLOCKOUT_PLLIN2:
        OutVal = 0;
        break;

    case FMCRADIO_CLOCKOUT_NC:
        OutVal = 1;
        break;

    case FMCRADIO_CLOCKOUT_FMCCLK0:
        OutVal = 2;
        break;

    case FMCRADIO_CLOCKOUT_FMCCLK1:
        OutVal = 3;
        break;

    default:
        return FMCRADIOIVAL;
    }


    switch (src)
    {
    case FMCRADIO_CLOCKIN_PLLCLKOUT:
        InVal = 0;
        break;

    case FMCRADIO_CLOCKIN_EXTCLK:
        InVal = 1;
        break;

    case FMCRADIO_CLOCKIN_FMCCLK2:
        InVal = 2;
        break;

    case FMCRADIO_CLOCKIN_FMCCLK3:
        InVal = 3;
        break;

    default:
        return FMCRADIOIVAL;
    }

    save_clk_en = (fmc_radio->core->board_control & FMC_RADIO_DESIGN_CLK_EN);

    if((dest == FMCRADIO_CLOCKOUT_FMCCLK0 || dest == FMCRADIO_CLOCKOUT_FMCCLK1) && save_clk_en != 0)
      fmc_radio_disable_clock(fmc_radio);


    *regPtr &= (~(FMC_RADIO_CLK_MUX_ALL_BITS));  //erase all clk ctrl bit;

    *regPtr |= FMC_RADIO_2CLKMUXSIN(InVal) | FMC_RADIO_2CLKMUXSOUT(OutVal);

    *regPtr |= FMC_RADIO_CLK_MUX_LOAD;
    *regPtr &= ~FMC_RADIO_CLK_MUX_LOAD;

    *regPtr |= FMC_RADIO_CLK_MUX_CONFIG;
    *regPtr &= ~FMC_RADIO_CLK_MUX_CONFIG;

    if((dest == FMCRADIO_CLOCKOUT_FMCCLK0 || dest == FMCRADIO_CLOCKOUT_FMCCLK1) && save_clk_en != 0)
      fmc_radio_enable_clock(fmc_radio);

    return OK;
}

Result fmc_radio_sdr_band(fmc_radio * fmc_radio, FMC_RADIO_SDR_BAND band)
{
    Result ret;
    lms6002_Register_Map regs;

    switch(band)
    {
    case FMC_RADIO_SDR_LOW_BAND:
    {
        //set RF path
        ret = fmc_radio_setIOexp(fmc_radio, FMCRADIO_CPLD_TXLOWBANDSEL, FMCRADIO_CPLD_RXHIBANDSEL);   //select low band
        if(FAILURE(ret)) return ret;
        
        // Connect to LNA internal output       
        ret = lms6002_spi_read(&fmc_radio->lime_core, 0x71, &regs.Register_71.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_71.Bit.input_mixer = 1;
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x71, regs.Register_71.raw);
        if(FAILURE(ret)) return ret;
        
        // Enable LNA 1
        ret = lms6002_spi_read(&fmc_radio->lime_core, 0x75, &regs.Register_75.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_75.Bit.active_lna = 1;        
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x75, regs.Register_75.raw);
        if(FAILURE(ret)) return ret;

        //select first RX buffer
        ret = lms6002_spi_read(&fmc_radio->lime_core, 0x25, &regs.Register_25.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_25.Bit.selout = 1; 
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x25, regs.Register_25.raw);
        if(FAILURE(ret)) return ret;

        //select first TX buffer
        ret = lms6002_spi_read(&fmc_radio->lime_core, 0x44, &regs.Register_44.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_44.Bit.pa_en = 2;        
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x44, regs.Register_44.raw);
        if(FAILURE(ret)) return ret;
    }
    break;

    case FMC_RADIO_SDR_HIGH_BAND:
    {
        //set RF path
        ret = fmc_radio_setIOexp(fmc_radio, FMCRADIO_CPLD_RXHIBANDSEL, FMCRADIO_CPLD_TXLOWBANDSEL);    //select high band
        if(FAILURE(ret)) return ret;
       
        // Connect to pads
        ret = lms6002_spi_read(&fmc_radio->lime_core, 0x71, &regs.Register_71.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_71.Bit.input_mixer = 0;
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x71, regs.Register_71.raw);
        if(FAILURE(ret)) return ret;
        
        // Active LNA 2 to bypass
        ret = lms6002_spi_read(&fmc_radio->lime_core, 0x75, &regs.Register_75.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_75.Bit.active_lna = 2;
        regs.Register_75.Bit.lna_gain_mode = 1;
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x75, regs.Register_75.raw);
        if(FAILURE(ret)) return ret;

        //select second RX buffer
        ret = lms6002_spi_read(&fmc_radio->lime_core, 0x25, &regs.Register_25.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_25.Bit.selout = 2; 
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x25, regs.Register_25.raw);
        if(FAILURE(ret)) return ret;

        //select second TX buffer
        ret = lms6002_spi_read(&fmc_radio->lime_core, 0x44, &regs.Register_44.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_44.Bit.pa_en = 4;        
        ret = lms6002_spi_write(&fmc_radio->lime_core, 0x44, regs.Register_44.raw);
        if(FAILURE(ret)) return ret;
    }
    break;
    }
    return OK;
}

#ifndef DISABLE_FMC_RADIO_LTE
Result fmc_radio_lte_band(fmc_radio * fmc_radio, FMC_RADIO_LTE_BAND band)
{
    int i;
    Result ret;
    unsigned int res;
    unsigned char regadd[] =          {0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD};

    switch(band)
    {

    case FMC_RADIO_LTE_WIDE_BAND:
    {
        unsigned char regdata[] = {0x01,0x80,0x80,0x00,0x00,0xF0,0x00,0x1C,0x1C,0x77,0x77,0x18,0x00};

        for (i=0;i<sizeof(regadd);i++)
        {
            fmc_radio_spi_write(fmc_radio, FMCRADIO_RF,regadd[i],regdata[i]);
        }

        //select third RX buffer
        ret = fmc_radio_spi_read(fmc_radio, FMCRADIO_RF, 0x25, &res);
        res &= 0xFC;
        res |= 0x3;
        ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x25, res);

        //keep actual TX buffer
    }
    break;

    case FMC_RADIO_LTE_LOW_BAND:
    {
        unsigned char regdata[] = {0x01,0x80,0x80,0x00,0x00,0xD0,0x00,0x1C,0x1C,0x77,0x77,0x18,0x00};

        for (i=0;i<sizeof(regadd);i++)
        {
            fmc_radio_spi_write(fmc_radio, FMCRADIO_RF,regadd[i],regdata[i]);
        }

        //select first RX buffer
        ret = fmc_radio_spi_read(fmc_radio, FMCRADIO_RF, 0x25, &res);
        res &= 0xFC;
        res |= 0x1;
        ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x25, res);

        //select first TX buffer
        ret = fmc_radio_spi_read(fmc_radio, FMCRADIO_RF, 0x44, &res);
        res &= ~0x1C;
        res |= 0x08;
        ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x44, res);
    }
    break;

    case FMC_RADIO_LTE_HIGH_BAND:
    {
        unsigned char regdata[] = {0x01,0x80,0x80,0x00,0x00,0xE0,0x00,0x1C,0x1C,0x77,0x77,0x18,0x00};

        //set RF path
        fmc_radio_setIOexp(fmc_radio, FMCRADIO_CPLD_RXHIBANDSEL, FMCRADIO_CPLD_TXLOWBANDSEL);

        for (i=0;i<sizeof(regadd);i++)
        {
            fmc_radio_spi_write(fmc_radio, FMCRADIO_RF,regadd[i],regdata[i]);
        }

        //select second RX buffer
        ret = fmc_radio_spi_read(fmc_radio, FMCRADIO_RF, 0x25, &res);
        res &= 0xFC;
        res |= 0x2;
        fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x25, res);

        //select second TX buffer
        ret = fmc_radio_spi_read(fmc_radio, FMCRADIO_RF, 0x44, &res);
        res &= ~0x1C;
        res |= 0x10;
        ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_RF, 0x44, res);
    }
    break;
    }

    return OK;
}
#endif /*DISABLE_FMC_RADIO_LTE*/

#ifndef DISABLE_FMC_RADIO_LTE
Result fmc_radio_lte_set_wideband_switch(fmc_radio * fmc_radio, FMCRADIOLTE_WIDE_INPUT_SWITCH_ID switch_id)
{
    int InVal;
    volatile unsigned int* regPtr = (volatile unsigned int*)&fmc_radio->core->board_control;

    switch (switch_id)
    {
    case FMCRADIOLTE_WIDESWITCH_RF_GROUND:
        InVal = 0;
        break;

    case FMCRADIOLTE_WIDESWITCH_WIDE_INPUT:
        InVal = 1;
        break;

    case FMCRADIOLTE_WIDESWITCH_HIGH_INPUT:
        InVal = 2;
        break;

    case FMCRADIOLTE_WIDESWITCH_LOW_INPUT:
        InVal = 3;
        break;

    default:
        return FMCRADIOIVAL;
    }

    *regPtr |= FMC_RADIO_WIDE_INPUT_SW(InVal);

    return OK;
}
#endif /*DISABLE_FMC_RADIO_LTE*/

Result fmc_radio_sdr_rx_gain(fmc_radio * fmc_radio, LMS6002_LNA_GAIN lna_gain, LMS6002_VGA1_GAIN rx_gain1, unsigned char rx_gain2, char rx_gain3)
{
    Result ret;

    //convert gain values to register value
    rx_gain3 += 13;
    rx_gain3 *= 2;
    ret = lms6002_rx_gain(&fmc_radio->lime_core, lna_gain, rx_gain1, rx_gain2);
    if(FAILURE(ret)) return ret;
    return fmc_radio_spi_write(fmc_radio, FMCRADIO_RXGAIN, 0, rx_gain3 & 0x3f);
}

#ifndef DISABLE_FMC_RADIO_LTE
Result fmc_radio_lte_rx_gain(fmc_radio * fmc_radio, LMS6002_LNA_GAIN lna_gain, LMS6002_VGA1_GAIN rx_gain1, unsigned char rx_gain2)
{
    //convert gain values to register value
    return lms6002_rx_gain(&fmc_radio->lime_core, lna_gain, rx_gain1, rx_gain2);
}
#endif /*DISABLE_FMC_RADIO_LTE*/

Result fmc_radio_pll_lockstatus(fmc_radio * fmc_radio, pll_lock_status * state)
{
    //return cdce62005_lock_state(&fmc_radio->pll, state);
    *state = fmc_radio->core->board_control & FMC_RADIO_PLL_LOCK ? CDCE62005_PLL_LOCKED : CDCE62005_PLL_UNLOCKED;
    return OK;
}

Result fmc_radio_pll_waitforlock(fmc_radio * fmc_radio)
{
	pll_lock_status status;
	unsigned timeout = 1000;

	fmc_radio_pll_lockstatus(fmc_radio, &status);

	while ( status != CDCE62005_PLL_LOCKED && timeout != 0)
	{
		pfRadio420_usleep(1000);
		debug("wait for PLL lock: %i %i\n", timeout, status);
		fmc_radio_pll_lockstatus(fmc_radio, &status);
		timeout--;
	}

	if (timeout)
	{
		return OK;
	}else{
		return FMCRADIOPLLUNLOCKED;
	}
}

Result fmc_radio_pll_setparam(fmc_radio * fmc_radio, cdce62005_param * param)
{
  Result res;

  fmc_radio_disable_clock(fmc_radio);
  res = cdce62005_setparam(param, &fmc_radio->pll);
  pfRadio420_usleep(1000);
  fmc_radio_enable_clock(fmc_radio);

  return res;
}

Result fmc_radio_lime_setpllparam(fmc_radio * fmc_radio, LMS6002_DIR dir, lms6002_pll_param * param)
{
    Result ret;
    switch(dir)
    {
    case LMS6002_TX:
        ret = lms6002_pll_setparam(&fmc_radio->lime_core, param, LMS6002_PLL_TX);
        break;
    case LMS6002_RX:
        ret = lms6002_pll_setparam(&fmc_radio->lime_core, param, LMS6002_PLL_RX);
        break;
    case LMS6002_TXRX:
        ret = lms6002_pll_setparam(&fmc_radio->lime_core, param, LMS6002_PLL_TX);
        if(FAILURE(ret)) return ret;
        ret = lms6002_pll_setparam(&fmc_radio->lime_core, param, LMS6002_PLL_RX);
    default:
        return FMCRADIOIVAL;
    }
    return ret;
}

Result fmc_radio_lime_calibratePLL(fmc_radio * fmc_radio, LMS6002_DIR pll)
{
    Result ret;
    switch(pll)
    {
    case LMS6002_TX:
        ret = lms6002_pll_calibrate(&fmc_radio->lime_core, LMS6002_PLL_TX);
        break;
    case LMS6002_RX:
        ret = lms6002_pll_calibrate(&fmc_radio->lime_core, LMS6002_PLL_RX);
        break;
    case LMS6002_TXRX:
        ret = lms6002_pll_calibrate(&fmc_radio->lime_core, LMS6002_PLL_TX);
        if(FAILURE(ret)) return ret;
        ret = lms6002_pll_calibrate(&fmc_radio->lime_core, LMS6002_PLL_RX);
    default:
        return FMCRADIOIVAL;
    }
    return ret;
}

Result fmc_radio_lime_reset(fmc_radio * fmc_radio)
{
    fmc_radio->core->board_control &= ~FMC_RADIO_LIME_ENABLE;
    fmc_radio->core->board_control |=  FMC_RADIO_LIME_ENABLE;
    return OK;
}

Result fmc_radio_lpf_set(fmc_radio * fmc_radio, LMS6002_DIR dir, LMS6002_LPF_BANDWIDTH bwt)
{
    return lms6002_lpf_set(&fmc_radio->lime_core, dir, bwt);
}

Result fmc_radio_lpf_calibrate(fmc_radio * fmc_radio, LMS6002_DIR dir, unsigned ref_freq)
{
    Result ret;
    unsigned char dac_data_sel;
    
    // Save current dac data sel
    dac_data_sel = fmc_radio->core->dac_data_control.dac_data_sel;
    
    // Set dac data sel to null
    fmc_radio->core->dac_data_control.dac_data_sel = FMC_RADIO_DATA_NULL;
    pfRadio420_usleep(100);
    
    switch(dir)
    {
    case LMS6002_TXRX:

		ret = lms6002_lpf_calibrateTuningModule(&fmc_radio->lime_core);
		debug("lms6002_lpf_calibrateTuningModule %x\n", ret);
		if(FAILURE(ret)) break;

		ret = lms6002_lpf_tune(&fmc_radio->lime_core, ref_freq);
		debug("lms6002_lpf_tune %x\n", ret);
		if(FAILURE(ret)) break;

		ret = lms6002_lpf_calibrateDCOffset(&fmc_radio->lime_core, LMS6002_TX);
		debug("lms6002_lpf_calibrateDCOffset %x\n", ret);
		if(FAILURE(ret)) break;

		ret = lms6002_lpf_calibrateDCOffset(&fmc_radio->lime_core, LMS6002_RX);
		debug("lms6002_lpf_calibrateDCOffset %x\n", ret);
		if(FAILURE(ret)) break;

        break;

    case LMS6002_RX:

		ret = lms6002_lpf_calibrateTuningModule(&fmc_radio->lime_core);
		debug("lms6002_lpf_calibrateTuningModule %x\n", ret);
		if(FAILURE(ret)) break;

		ret = lms6002_lpf_tune(&fmc_radio->lime_core, ref_freq);
		debug("lms6002_lpf_tune %x\n", ret);
		if(FAILURE(ret)) break;

    	ret = lms6002_lpf_calibrateDCOffset(&fmc_radio->lime_core, LMS6002_RX);
    	debug("lms6002_lpf_calibrateDCOffset %x\n", ret);
		if(FAILURE(ret)) break;
    	break;

    case LMS6002_TX:

		ret = lms6002_lpf_calibrateTuningModule(&fmc_radio->lime_core);
		debug("lms6002_lpf_calibrateTuningModule %x\n", ret);
		if(FAILURE(ret)) break;

		ret = lms6002_lpf_tune(&fmc_radio->lime_core, ref_freq);
		debug("lms6002_lpf_tune %x\n", ret);
		if(FAILURE(ret)) break;

    	ret = lms6002_lpf_calibrateDCOffset(&fmc_radio->lime_core, LMS6002_TX);
		debug("lms6002_lpf_calibrateDCOffset %x\n", ret);
		if(FAILURE(ret)) break;

        break;

    default:
        ret = -1;
        break;
    }

    // Restore dac data sel
    fmc_radio->core->dac_data_control.dac_data_sel = dac_data_sel;
    pfRadio420_usleep(100);

    return ret;
}

Result fmc_radio_rxvga_calibrate(fmc_radio * fmc_radio)
{
    Result ret;
    unsigned char dac_data_sel;
    
    // Save current dac data sel
    dac_data_sel = fmc_radio->core->dac_data_control.dac_data_sel;
    
    // Set dac data sel to null
    fmc_radio->core->dac_data_control.dac_data_sel = FMC_RADIO_DATA_NULL;
    pfRadio420_usleep(100);

    ret = lms6002_rxvga2_calibrate(&fmc_radio->lime_core);

    // Restore dac data sel
    fmc_radio->core->dac_data_control.dac_data_sel = dac_data_sel;
    pfRadio420_usleep(100);

    return ret;
}

Result fmc_radio_setIOexp(fmc_radio * fmc_radio, unsigned int data, unsigned int mask )
{
    unsigned tmp = fmc_radio->cpld_reg_shadow;
    tmp &= ~mask;
    tmp |= data;
    fmc_radio->cpld_reg_shadow = tmp;
    return fmc_radio_spi_write(fmc_radio, FMCRADIO_CPLD, 0xA, tmp);
}

Result fmc_radio_getIOexp(fmc_radio * fmc_radio, unsigned * data)
{
    //return fmc_radio_spi_read(fmc_radio, FMCRADIO_CPLD, 0xA, data);
    *data = fmc_radio->cpld_reg_shadow;
    return OK;
}

Result fmc_radio_sdr_setrxfilter(fmc_radio * fmc_radio, FMCRADIOSDR_FILTER_BANK bank)
{
    return fmc_radio_setIOexp(fmc_radio, bank << 5, FMCRADIO_CPLD_FILTERBANLSEL);
}

Result fmc_radio_tx_gain(fmc_radio * fmc_radio, char tx_vga1_gain, unsigned char tx_vga2_gain, char tx_gain3)
{
    Result ret;
    debug("Set Tx gain %i %i %i\n", tx_vga1_gain, tx_vga2_gain, tx_gain3);
	tx_vga1_gain += 35;
    tx_gain3 += 13;
    tx_gain3 *= 2;
    ret = lms6002_tx_gain(&fmc_radio->lime_core, tx_vga1_gain, tx_vga2_gain);
    if(FAILURE(ret)) return ret;
    ret = fmc_radio_spi_write(fmc_radio, FMCRADIO_TXGAIN, 0, tx_gain3 & 0x3f);
    return ret;
}

Result fmc_radio_path_disable(fmc_radio * fmc_radio, LMS6002_DIR dir)
{
    Result ret;
    ret = lms6002_disablePath(&fmc_radio->lime_core, dir);
    if(FAILURE(ret)) return ret;
    switch(dir)
    {
    case LMS6002_TX:
        fmc_radio->core->board_control &= ~(FMC_RADIO_LIME_TX_EN);
        break;
    case LMS6002_RX:
        fmc_radio->core->board_control &= ~(FMC_RADIO_LIME_RX_EN);
        break;
    case LMS6002_TXRX:
        fmc_radio->core->board_control &= ~(FMC_RADIO_LIME_TX_EN | FMC_RADIO_LIME_RX_EN);
        break;
    default:
        return FMCRADIOIVAL;
    }
    return OK;
}

Result fmc_radio_path_enable(fmc_radio * fmc_radio, LMS6002_DIR dir)
{
    switch(dir)
    {
    case LMS6002_TX:
        fmc_radio->core->board_control |= FMC_RADIO_LIME_TX_EN;
        break;
    case LMS6002_RX:
        fmc_radio->core->board_control |= FMC_RADIO_LIME_RX_EN;
        break;
    case LMS6002_TXRX:
        fmc_radio->core->board_control |= FMC_RADIO_LIME_TX_EN | FMC_RADIO_LIME_RX_EN;
        break;
    default:
        return FMCRADIOIVAL;
    }
    return lms6002_enablePath(&fmc_radio->lime_core, dir);
}

Result fmc_radio_rx_connect(fmc_radio * fmc_radio, LMS6002_BAND band)
{
    return lms6002_rx_connect(&fmc_radio->lime_core, band);
}

Result fmc_radio_rx_disconnect(fmc_radio * fmc_radio, LMS6002_BAND band)
{
    return lms6002_rx_disconnect(&fmc_radio->lime_core, band);
}

#ifndef DISABLE_FMC_RADIO_LTE
Result fmc_radio_lte_poweramp_enable(fmc_radio * fmc_radio, FMC_RADIO_LTE_BAND band)
{
    switch(band)
    {
    case FMC_RADIO_LTE_LOW_BAND:
        fmc_radio->core->board_control |= FMC_RADIO_PA_LOW_EN;
        break;
    case FMC_RADIO_LTE_HIGH_BAND:
        fmc_radio->core->board_control |= FMC_RADIO_PA_HIGH_EN;
        break;
    case FMC_RADIO_LTE_WIDE_BAND:
        fmc_radio->core->board_control |= FMC_RADIO_PA_LOW_EN;
        fmc_radio->core->board_control |= FMC_RADIO_PA_HIGH_EN;
        break;
    default:
        return FMCRADIOIVAL;
    }

    return OK;
}
#endif /*DISABLE_FMC_RADIO_LTE*/

#ifndef DISABLE_FMC_RADIO_LTE
Result fmc_radio_lte_poweramp_disable(fmc_radio * fmc_radio, FMC_RADIO_LTE_BAND band)
{

    switch(band)
    {
    case FMC_RADIO_LTE_LOW_BAND:
        fmc_radio->core->board_control &= ~(FMC_RADIO_PA_LOW_EN);
        break;
    case FMC_RADIO_LTE_HIGH_BAND:
        fmc_radio->core->board_control &= ~(FMC_RADIO_PA_HIGH_EN);
        break;
    case FMC_RADIO_LTE_WIDE_BAND:
        fmc_radio->core->board_control &= ~(FMC_RADIO_PA_LOW_EN);
        fmc_radio->core->board_control &= ~(FMC_RADIO_PA_HIGH_EN);
        break;
    default:
        return FMCRADIOIVAL;
    }

    return OK;
}
#endif /*DISABLE_FMC_RADIO_LTE*/

Result fmc_radio_spi_ctrl(fmc_radio * fmc_radio, FMCRADIO_FPGA_CTRL_LIST control, FMCRADIO_SPI_CTRL spi_controller)
{
	switch(control)
	{
	case FMCRADIO_CTRL_REF_CLOCK:
		if (spi_controller == FMCRADIO_HOST)
		{
			fmc_radio->core->board_control &= ~(FMC_RADIO_REF_CLK);
		}
		else if(spi_controller == FMCRADIO_FPGA)
		{
			fmc_radio->core->board_control |= FMC_RADIO_REF_CLK;
		}
		else
		{
			return FMCRADIOIVAL;
		}
		break;
	case FMCRADIO_CTRL_RF:
		if (spi_controller == FMCRADIO_HOST)
		{
			fmc_radio->core->board_control &= ~(FMC_RADIO_RF);
		}
		else if(spi_controller == FMCRADIO_FPGA)
		{
			fmc_radio->core->board_control |= FMC_RADIO_RF;
		}
		else
		{
			return FMCRADIOIVAL;
		}
		break;
	case FMCRADIO_CTRL_RX_GAIN:
		if (spi_controller == FMCRADIO_HOST)
		{
			fmc_radio->core->board_control &= ~(FMC_RADIO_RX_GAIN);
		}
		else if(spi_controller == FMCRADIO_FPGA)
		{
			fmc_radio->core->board_control |= FMC_RADIO_RX_GAIN;
		}
		else
		{
			return FMCRADIOIVAL;
		}
		break;
	case FMCRADIO_CTRL_TX_GAIN:
		if (spi_controller == FMCRADIO_HOST)
		{
			fmc_radio->core->board_control &= ~(FMC_RADIO_TX_GAIN);
		}
		else if(spi_controller == FMCRADIO_FPGA)
		{
			fmc_radio->core->board_control |= FMC_RADIO_TX_GAIN;
		}
		else
		{
			return FMCRADIOIVAL;
		}
		break;
	case FMCRADIO_CTRL_PLL_CPLD:
		if (spi_controller == FMCRADIO_HOST)
		{
			fmc_radio->core->board_control &= ~(FMC_RADIO_PLL_CPLD);
		}
		else if(spi_controller == FMCRADIO_FPGA)
		{
			fmc_radio->core->board_control |= FMC_RADIO_PLL_CPLD;
		}
		else
		{
			return FMCRADIOIVAL;
		}
		break;
	default:
		return FMCRADIOIVAL;
	}
	
	return OK;
}

#endif
