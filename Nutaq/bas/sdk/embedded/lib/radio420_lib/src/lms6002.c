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
 * @file       lms6002.c 
 * @brief      Calculate valid configuration and configure the LMS6002
 *             integrated RF front-end
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
#include "lms6002.h"
#include "lms6002_regs.h"
#include <math.h>
#include <string.h>
#include "bas_debug.h"
#include "bas_error.h"
// Point the the pfRadio420_usleep function pointer define in the fmc_radio.c
extern int (*pfRadio420_usleep)(unsigned int usec);

unsigned freqsel_values [][2] = {{232500000, 0x27}, {285625000, 0x2f}, {336875000, 0x37}, {405000000, 0x3f}, {465000000, 0x26}, {571250000, 0x2e}, {673750000, 0x36}, {810000000, 0x3e}, {930000000, 0x25}, {1142500000, 0x2d}, {1347500000, 0x35}, {1620000000, 0x3d}, {1860000000, 0x24}, {2285000000, 0x2c}, {2695000000, 0x34}, {3240000000, 0x3c}};

static Result find_freqsel(unsigned freq, char * freqsel)
{
    int i;

    for(i=sizeof(freqsel_values)/8-1; i>=0; i--)
    {
        if(freq >= freqsel_values[i][0])
        {
            *freqsel = freqsel_values[i][1];
            return OK;
        }
    }
    return LMS6002IVAL;
}

Result lms6002_pll_calculate(lms6002_pll_param * param)
{
    Result ret;
    double x;
    double dN;
    double dfractpart, dintpart;
    unsigned intpart, fractpart;
    char freqsel;

    ret = find_freqsel(param->out_freq, &freqsel);
    if(FAILURE(ret)) return ret;

    freqsel &= 0x7;
    freqsel -= 3;

    x = pow(2, (double)freqsel);

    dN = x * (double)param->out_freq / (double)param->ref_freq;
    dfractpart = modf(dN, &dintpart);
    intpart = (unsigned)dintpart;
    dfractpart *= pow(2,23);
    fractpart = (unsigned)dfractpart;
    param->n_divider = (intpart << 23) | fractpart;
    if( param->out_freq / param->ref_freq >= 130 )
    {
        return LMS6002MULTIPLIER;
    }else{
        return OK;
    }
}

#ifndef __ONLY_CALCULATE__
Result lms6002_pll_setparam(lms6002_core * core, lms6002_pll_param * param, LMS6002_PLL pll)
{
    Result ret;
    unsigned char base_addr;
    char freqsel;
    lms6002_Register_Map regs;

    switch(pll)
    {
    case LMS6002_PLL_TX:
        base_addr = core->tx_core.base_addr;
        break;
    case LMS6002_PLL_RX:
        base_addr = core->rx_core.base_addr;
        break;
    default:
        return LMS6002IVAL;
    }

    ret = find_freqsel(param->out_freq, &freqsel);
    if(FAILURE(ret)) return ret;

    ret = lms6002_spi_write(core, base_addr + LMS6002_PLL_OFF_DIV1, (unsigned char)(param->n_divider >> 24));
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_write(core, base_addr + LMS6002_PLL_OFF_DIV2, (unsigned char)(param->n_divider >> 16));
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_write(core, base_addr + LMS6002_PLL_OFF_DIV3, (unsigned char)(param->n_divider >> 8));
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_write(core, base_addr + LMS6002_PLL_OFF_DIV4, (unsigned char)(param->n_divider));
    if(FAILURE(ret)) return ret;

    ret = lms6002_spi_read(core, base_addr + LMS6002_PLL_OFF_FREQSEL, &regs.Register_25.raw);
    regs.Register_25.Bit.freqsel = freqsel;
    ret = lms6002_spi_write(core, base_addr + LMS6002_PLL_OFF_FREQSEL, regs.Register_25.raw);

    return ret;
}

static Result lms6002_SetVCOCAP(lms6002_core * core, LMS6002_PLL pll, unsigned char vcocap)
{
    Result ret;
    unsigned char base_addr;
    unsigned char data;

    switch(pll)
    {
    case LMS6002_PLL_TX:
        base_addr = core->tx_core.base_addr;
        break;
    case LMS6002_PLL_RX:
        base_addr = core->rx_core.base_addr;
        break;
    default:
        return LMS6002IVAL;
    }

    ret = lms6002_spi_read(core, base_addr + LMS6002_PLL_OFF_VCOCAP, &data);
    if(FAILURE(ret)) return ret;
    
    //update bits
    data &= 0xC0;
    data |= (vcocap & 0x3F);
    return lms6002_spi_write(core, base_addr + LMS6002_PLL_OFF_VCOCAP, data);
}

static Result lms6002_getTune(lms6002_core * core, LMS6002_PLL pll, unsigned char * rtune)
{
    Result ret;
    unsigned char tune;
    unsigned char base_addr;

    switch(pll)
    {
    case LMS6002_PLL_TX:
        base_addr = core->tx_core.base_addr;
        break;
    case LMS6002_PLL_RX:
        base_addr = core->rx_core.base_addr;
        break;
    default:
        return LMS6002IVAL;
    }

    pfRadio420_usleep(10);

    ret = lms6002_spi_read(core, base_addr + LMS6002_PLL_OFF_VTUNE, &tune);
    if(FAILURE(ret)) return ret;
    tune >>= 6;
    *rtune = tune;
    return OK;
}

Result lms6002_pll_calibrate(lms6002_core * core, LMS6002_PLL pll)
{
    unsigned char vcocap;
    Result ret;
    int vcocapH,vcocapL;
    unsigned char tune, comp;
    unsigned char base_addr;
    int findL = 0;
    int findH = 0;

    switch(pll)
    {
    case LMS6002_PLL_TX:
        base_addr = core->tx_core.base_addr;
        break;
    case LMS6002_PLL_RX:
        base_addr = core->rx_core.base_addr;
        break;
    default:
        return LMS6002IVAL;
    }

    //enable comparator
    ret = lms6002_spi_read(core, base_addr + LMS6002_PLL_OFF_COMPARATOR, &comp);
    comp |= 1 << 3;
    ret = lms6002_spi_write(core, base_addr + LMS6002_PLL_OFF_COMPARATOR, comp);

    //algo based on section 4.6 of programming guide 1.1

 /*   //set VCOCAP = 31
    vcocap = 31;
    lms6002_SetVCOCAP(core, pll, vcocap);

    //read status comp
    ret = lms6002_getTune(core, pll, &tune);
    if(FAILURE(ret)) return ret;

    switch (tune)
    {
    case 0:
        //goto 1 in graph
        do{
            vcocap--;
            lms6002_SetVCOCAP(core, pll, vcocap);
            ret = lms6002_getTune(core, pll, &tune);
        } while ( tune !=2 && vcocap>0 );
        vcocapL = vcocap;

        vcocap = 31;
        lms6002_SetVCOCAP(core, pll, vcocap);
        do{
            vcocap++;
            lms6002_SetVCOCAP(core, pll, vcocap);
            ret = lms6002_getTune(core, pll, &tune);
        } while ( tune!=1 && vcocap<63);
        vcocapH = vcocap;
        break;
    
    case 1:
        //goto 2 in graph
        do{
            vcocap--;
            lms6002_SetVCOCAP(core, pll, vcocap);
            ret = lms6002_getTune(core, pll, &tune);
        } while ( tune!=0 && vcocap>0 );
        vcocapL = vcocap;

        do{
            vcocap--;
            lms6002_SetVCOCAP(core, pll, vcocap);
            ret = lms6002_getTune(core, pll, &tune);
        } while ( tune!=2 && vcocap>0 );
        vcocapH = vcocap;
        break;

    case 2:
        //goto 3 in graph
        do{
            vcocap++;
            lms6002_SetVCOCAP(core, pll, vcocap);
            pfRadio420_usleep(1000);
            ret = lms6002_getTune(core, pll, &tune);
        } while ( tune != 0 && vcocap<63);

        vcocapL = vcocap;
        do{
            vcocap++;
            lms6002_SetVCOCAP(core, pll, vcocap);
            ret = lms6002_getTune(core, pll, &tune);
        } while ( tune != 1 && vcocap<63);
        vcocapH = vcocap;
        break;


    default:
        return LMS6002GENERALERROR;
    }
 */
    //FIXME  DL  Algo refait de manière simple
    unsigned char tune_results[64];
    for (vcocap = 0; vcocap < 64; vcocap++)
    {
    	lms6002_SetVCOCAP(core, pll, vcocap);
    	pfRadio420_usleep(50); //2e4 équivalent 1 sec.
    	lms6002_getTune(core, pll, &tune);
    	tune_results[vcocap]=tune;
    }

	vcocapH = 63;
	vcocapL = 0;
    for (vcocap = 1; vcocap < 64; vcocap++)
    {
    	if(findL == 0)
    	{
    		if(tune_results[vcocap]==0)
    		{
    			vcocapL = vcocap;
    			findL = 1;
    		}
    	}
    	else if(findH == 0)
    	{
    		if(tune_results[vcocap]==1)
			{
				vcocapH = vcocap - 1;
				findH = 1;
			}
    	}
    }
    vcocap = (vcocapH + vcocapL)/2;

    lms6002_SetVCOCAP(core, pll, vcocap);

    //verify value;
    ret = lms6002_getTune(core, pll, &tune);
    debug("VCO cap %i\n",vcocap);
    if(tune != 0)
    {
        ret = LMS6002PLLUNLOCKED;
    }

    //disable comparator
    comp &= ~(1 << 3);
    lms6002_spi_write(core, base_addr + LMS6002_PLL_OFF_COMPARATOR, comp);

    return ret;
}

Result lms6002_open(lms6002_core * core, spi_core_t * spi)
{
    Result ret;
    unsigned char res;

    memset(core, 0, sizeof(lms6002_core));
    core->spi = spi;
    ret = lms6002_pll_open(&core->rx_core, 0x20);
    if(FAILURE(ret)) return ret;
    ret = lms6002_pll_open(&core->tx_core, 0x10);
    if(FAILURE(ret)) return ret;

    core->saved_gain = 0;
    core->saved_pa = 1;

    //read version of the lime to detect it.
    ret = lms6002_spi_read(core, 0x04, &res);
    if (res != 0x22)
    {
        return LMS6002ABSENT;
    }

    return OK;
}

/*
Use this cleanup version of the PLL init when convenient
unsigned char regadd[82] =  {
    0x82,0x83,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,                           // 1 top
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,                      // 2 tx pll
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,                      // 3 rx 
    0xB2,0xB3,0xB4,0xB5,0xB6,                                               // 4 Tx LPF

    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,            // 5 tx RF module
    0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,  // 6 rx LPF ADC DAC
    0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xEE,                                // 7 rx vga2
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD   // 8  rxFE
    };
    unsigned char regdata[82] = {
    0x1F,0x08,0x3E,0x0C,0x09,0x00,0x05,0x00,0x00,                           // 1 top
    //0x78,0x00,0x00,0x00,0xFC,0xF9,0x8F,0xE5,0x45,0xBE,                      // 2 tx pll
    //0x56,0xAA,0xAA,0xAA,0xFC,0xB5,0x8F,0xE5,0x45,0xBE,                      // 3 rx
    //0x78,0x00,0x00,0x00,0xFC,0xF9,0x91,0xE5,0x4f,0xBE,                      // 2 tx pll
    //0x56,0xAA,0xAA,0xAA,0xFC,0xB5,0x83,0xE1,0x41,0xBE,                      // 3 rx
    0x78,0x00,0x00,0x00,0xFC,0xF9,0x8F,0xE5,0x45,0xBE,                      // 2 tx pll
    0x56,0xAA,0xAA,0xAA,0xFC,0xB5,0x8F,0xE5,0x45,0xBE,                      // 3 rx
    0x1F,0x08,0x12,0x0A,0x38,                                               // 4 Tx LPF
    0x02,0x17,0x7B,0x8F,0x0B,0xA0,0x00,0x50,0x0D,0x0C,0x18,0x50,            // 5 tx RF module
    0x1F,0x08,0x12,0x0A,0x48,0xA5,0x00,0x09,0x30,0x00,0x00,0x00,0x00,0x1F,  // 6 rx LPF ADC DAC
    0x1F,0x08,0x32,0x0A,0x00,0x00,0x01,0xC0,                                // 7 rx vga2
    0x01,0x80,0x80,0x00,0x00,0xD0,0x66,0x00,0x1C,0x1C,0x77,0x77,0x18,0x00   // 8  rxFE

*/

Result lms6002_reset(lms6002_core * core)
{
    Result ret;
    int i;
    //echo "Now programming the Lime Top modules"
    unsigned char regadd[82] =  {
    0x82,0x83,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,                     //top   #9
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,                //tx pll #13
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,                //rx pll #13
    0xB2,0xB3,0xB4,0xB5,0xB6,                                         //Tx LPF #5

    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,  //tx RF module  #12
    0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF, //rx LPF ADC DAC  #14
    0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xEE,                               //rx vga2   #7
    0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD   //rxFE    //  #14
    };
    unsigned char regdata[82] = {
      0x1F,0x08,0x3E,0x0C,0x09,0x00,0x05,0x00,0x00,     //top
    //0x1F,0x08,0x3E,0x0C,0x09,0x00,0x85,0x00,0x00,     //adc test mode;

    //0x45,0xA9,0x55,0x55,0x88,0x91,0x8C,0xE3,0x40,0xA6,0x03,0x76,0x38,      //tx pll
    //0x50,0x00,0x00,0x00,0x88,0xDD,0x8C,0xE3,0x40,0x94,0x03,0x76,0x38,      //TX 400MHz
    //0x70,0x00,0x00,0x00,0x88,0xD5,0x8C,0xE3,0x40,0xBE,0x03,0x76,0x38,      //TX 1140MHz
    //0x54,0x02,0x26,0x8e,0x88,0xD5,0x8C,0xE3,0x40,0xBE,                      //TX 1400MHz ref 33
    0x78,0x00,0x00,0x00,0xFC,0xF9,0x8F,0xE5,0x45,0xBE,                      //TX 900MHz ref 30
    //0x78,0x00,0x00,0x00,0x88,0xF9,0x89,0x03,0x40,0xBE,                      //LPL
    //  0x66,0x2,0x9C,0x88,0x88,0xF5,0x8C,0xE3,0x40,0xBE,                       //Tx 1700 ref 33

    //0x54,0x02,0x26,0x8e,0x88,0xD5,0x8C,0xE3,0x40,0xBE,                      //close PLL
    //0x3F,0x7A,0x00,0x00,0x88,0x91,0x8C,0xE3,0x40,0x8F,0x03,0x76,0x38,      //rx pll
    //0x50,0x00,0x00,0x00,0x88,0xDD,0x8C,0xE3,0x40,0x8F,0x03,0x76,0x38,      //rx pll
    //0x70,0x00,0x00,0x00,0x88,0xD5,0x8C,0xE3,0x40,0xBE,0x03,0x76,0x38,      //ref:25 lo:1140
    //0x44,0x68,0x26,0xb6,0x88,0x95,0x8C,0xE3,0x40,0xBE,0x03,0x76,0x38,      //1140 ref 33
    //0x3C,0x00,0x00,0x00,0x88,0x91,0x8C,0xE3,0x40,0xBE,                       //~~1999
    //0x60,0x2,0x75,0x35,0x88,0xD5,0x8C,0xE3,0x40,0xBE,                       //1600 ref 33
    //0x66,0x2,0x9C,0x88,0x88,0xF5,0x8C,0xE3,0x40,0xBE,                       //1700 ref 33
      0x56,0xAA,0xAA,0xAA,0xFC,0xB5,0x8F,0xE5,0x45,0xBE,                       //1300 ref 30
    //0x78,0x00,0x00,0x00,0x88,0xF9,0x89,0x03,0x40,0xBE,                        //LPL

      0x1F,0x08,0x12,0x0A,0x38,                     //Tx LPF 5MHz.
    //0x1F,0x08,0x37,0x1F,0x30,                     //Tx LPF bypass LPF

    //0x02,0x1C,0x80,0x80,0x0B,0x78,0x00,0x40,0x0C,0x0C,0x18,0x50,0x00,0x00,
    //0x02,0x15,0x80,0x80,0x0B,0x78,0x00,0x40,0x0C,0x0C,0x18,0x50,   //tx RF module
      0x02,0x17,0x7B,0x8F,0x0B,0xA0,0x00,0x50,0x0D,0x0C,0x18,0x50,   //tx RF module  10dB VGA1 caract Hiband

    //0x1F,0x08,0x12,0x0A,0x48,0xBE,0x00,0x09,0x20,0x00,0x00,0x00,0x00,0x1F,  //RX LPF 5MHz  adc 1V - dac 50ohms 2.5mA
      0x1F,0x08,0x12,0x0A,0x48,0xA5,0x00,0x29,0x30,0x00,0x00,0x00,0x00,0x1F,  //RX LPF 5MHz  adc 1V - dac 100ohms 10mA
    //0x1F,0x08,0x02,0x5F,0x40,0x94,0x00,0x09,0x20,0x00,0x00,0x00,0x00,0x1F,  //bypass RX LPF

      0x1F,0x08,0x36,0x0A,0x00,0x00,0x01,0xC0, // RX VGA2 default.
    //0x1F,0x08,0x1f,0x0A,0x00,0x00,0x01, // RX VGA2 down

      0x01,0x80,0x80,0x00,0x00,0xD0,0x66,0x00,0x1C,0x1C,0x77,0x77,0x18,0x00    //RXFE low band gain 19dB
    };

    for (i=0;i<sizeof(regadd);i++)
    {
         ret = lms6002_spi_write(core, regadd[i],regdata[i]);
         if(FAILURE(ret)) return ret;
    }

#ifdef DEBUG
    {
        unsigned char res;
        for (i=0; i<sizeof(regadd);i++)
        {
            ret = lms6002_spi_read(core, regadd[i]&0x7f, &res);
            if(regdata[i] != res)
            {
                debug("lime reg %x  %x != %x\n", regadd[i]&0x7f, regdata[i], res);
            }
        }
    }
#endif

    return ret;
}

static Result lms6002_path_state(lms6002_core * core, LMS6002_DIR dir, LMS6002_STATE state)
{
    Result ret;
    lms6002_Register_Map regs;

    switch(dir)
    {
    case LMS6002_TXRX:
        ret = lms6002_path_state(core, LMS6002_TX, state);
        if(FAILURE(ret)) return ret;
        return lms6002_path_state(core, LMS6002_RX, state);
        break;

    case LMS6002_RX:
        ret = lms6002_spi_read(core, 0x24, &regs.Register_24.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_24.Bit.en = state;
        ret = lms6002_spi_write(core, 0x24, regs.Register_24.raw);
        if(FAILURE(ret)) return ret;

        ret = lms6002_spi_read(core, 0x70, &regs.Register_70.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_70.Bit.rxfe_en = state;
        ret = lms6002_spi_write(core, 0x70, regs.Register_70.raw);
        break;

    case LMS6002_TX:
        ret = lms6002_spi_read(core, 0x14, &regs.Register_14.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_14.Bit.en = state;
        ret = lms6002_spi_write(core, 0x14, regs.Register_14.raw);
        if(FAILURE(ret)) return ret;

        ret = lms6002_spi_read(core, 0x40, &regs.Register_40.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_40.Bit.txfe_en = state;
        ret = lms6002_spi_write(core, 0x40, regs.Register_40.raw);
        break;
    default:
        return LMS6002IVAL;
    }
    return OK;
}

Result lms6002_enablePath(lms6002_core * core, LMS6002_DIR dir)
{
    return lms6002_path_state(core, dir, LMS6002_ENABLE);
}

Result lms6002_disablePath(lms6002_core * core, LMS6002_DIR dir)
{
    return lms6002_path_state(core, dir, LMS6002_DISABLE);
}

Result lms6002_rx_connect(lms6002_core * core, LMS6002_BAND band)
{
    Result ret;
    lms6002_Register_Map regs;
    
    if(band == LMS6002_LOWBAND) //Make sure band is a valid argument
    { 
        ret = lms6002_spi_read(core, 0x25, &regs.Register_25.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_25.Bit.selout = 1; // RX PLL First buffer enabled
        ret = lms6002_spi_write(core, 0x25, regs.Register_25.raw);
        if(FAILURE(ret)) return ret;  
    }
    else if(band == LMS6002_HIGHBAND) //Make sure band is a valid argument
    {
        ret = lms6002_spi_read(core, 0x25, &regs.Register_25.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_25.Bit.selout = 2; // RX PLL Second buffer enabled
        ret = lms6002_spi_write(core, 0x25, regs.Register_25.raw);
        if(FAILURE(ret)) return ret;  
    }
    else
    {
        return LMS6002IVAL;
    }
    return OK;
}

Result lms6002_rx_disconnect(lms6002_core * core, LMS6002_BAND band)
{
    Result ret;
    lms6002_Register_Map regs;
    
    if(band == LMS6002_HIGHBAND || band == LMS6002_LOWBAND) //Make sure band is a valid argument
    { 
        ret = lms6002_spi_read(core, 0x25, &regs.Register_25.raw);
        if(FAILURE(ret)) return ret;
        regs.Register_25.Bit.selout = 0; // All output buffers for RX PLL are powered down
        ret = lms6002_spi_write(core, 0x25, regs.Register_25.raw);
        if(FAILURE(ret)) return ret;  
    }
    else
    {
        return LMS6002IVAL;
    }
    return OK;
}

Result lms6002_lpf_set(lms6002_core * core, LMS6002_DIR dir, LMS6002_LPF_BANDWIDTH bwt)
{
    Result ret;
    lms6002_Register_Map regs;
    unsigned char base_addr = 0x30;

    if (dir == LMS6002_RX)
    {
        base_addr += 0x20;

        ret = lms6002_spi_read(core, 0x7, &regs.Register_07.raw);
        regs.Register_07.Bit.bwc_lpfcal = bwt;
        ret = lms6002_spi_write(core, 0x7, regs.Register_07.raw);
    }

    if (bwt == LMS6002_LPF_BYPASS)
    {
        ret = lms6002_spi_read(core, base_addr + 4, &regs.Register_34.raw);
        regs.Register_34.Bit.en = 0; //disable lpf
        ret = lms6002_spi_write(core, base_addr + 4, regs.Register_34.raw);

        ret = lms6002_spi_read(core, base_addr + 5, &regs.Register_35.raw);
        regs.Register_35.Bit.byp_en_lpf = 1;
        ret = lms6002_spi_write(core, base_addr + 5, regs.Register_35.raw);
    }else{
        ret = lms6002_spi_read(core, base_addr + 5, &regs.Register_35.raw);
        regs.Register_35.Bit.byp_en_lpf = 0;
        ret = lms6002_spi_write(core, base_addr + 5, regs.Register_35.raw);

        regs.Register_34.raw = 0;
        regs.Register_34.Bit.bwc_lpf = bwt;
        regs.Register_34.Bit.en = 1;
        ret = lms6002_spi_write(core, base_addr + 4, regs.Register_34.raw);
    }
    return ret;
}

static unsigned int fmcRadioSdr_RFDCCal(lms6002_core * core, unsigned int offset,unsigned int addr)
{
    Result ret;
    //based on "General DC calibration Procedure" of the lime prog guide
    unsigned int tryCnt;
    unsigned int temp;
    unsigned char res,res1,res2;

    //set Reset...
    lms6002_spi_write(core,offset+3,0);
    //write addr + remove reset...
    lms6002_spi_write(core,offset+3,(0x07&addr)|0x08);
    tryCnt = 0;

    ret = lms6002_spi_read(core,offset+3,&res);
    res1 = res|(1<<5);

    //Start calibration
    lms6002_spi_write(core,offset+3,res1);
    //Deactivate start calibration command
    lms6002_spi_write(core,offset+3,res);

    while (tryCnt < 1000)   //arbitrary number of try.
    {
        //Calibration procedure like LMS FAQ PDF 1.0r10 29/10/2012 Answer 4.7
        do //Wait for calibration to be done
        {
            pfRadio420_usleep(7); //wait 6.4uS
            lms6002_spi_read(core,offset+1,&res2);
            //debug("Wait for calibration to be done : %i DC_LOCK=%i\n", tryCnt,(res2 >> 2) & 0x07);
        }while((res2 & 0x2) != 0);  //Calibration Done

            //Read DC_REG_VAL
            lms6002_spi_read(core,offset,&res2);

            if( res2 != 31)
            {
                  //debug("Calibration done at DC_REG_VAL(31)=%i : %i\n", res2, tryCnt);
                  return OK;
            }
            else
            {
                  //set DC_REG_VAL(0)
                  lms6002_spi_write(core,offset+2,0);
                  //Start calibration again
                  lms6002_spi_write(core,offset+3,res1);
                  //Deactivate start calibration command
                  lms6002_spi_write(core,offset+3,res);
                  do //Wait for calibration to be done
                  {
                        pfRadio420_usleep(7); //wait 6.4uS
                        lms6002_spi_read(core,offset+1,&res2);
                        //debug("Wait for calibration to be done : %i DC_LOCK=%i\n", tryCnt,(res2 >> 2) & 0x07);
                  }while((res2 & 0x2) != 0);  //Calibration Done

                  //Read DC_REG_VAL
                  lms6002_spi_read(core,offset,&res2);
                  if( res2 != 0)
                  {
                        //debug("Calibration done at DC_REG_VAL(0)=%i : %i\n", res2, tryCnt);
                        return OK;
                  }
                  else
                  {
                        debug("////////////////////////////////////////Calibration Failed after DC_REG_VAL(0) : %i\n", tryCnt);
                        return LMS6002DCCALFAILED;
                  }
            }

        tryCnt++;
    }
    debug("////////////////////////////////////////Calibration Failed after tryCnt max : %i\n", tryCnt);
    return LMS6002DCCALFAILED;
}


Result lms6002_lpf_calibrateTuningModule(lms6002_core * core)
{
    Result ret=OK;
    lms6002_Register_Map regs;
    unsigned int res;

    ret = lms6002_spi_read(core, 0x9, &regs.Register_09.raw);
    regs.Register_09.Bit.clk_en_lpf = 1;
    lms6002_spi_write(core, 0x9, regs.Register_09.raw);

    ret = fmcRadioSdr_RFDCCal(core, 0x00,0);
    if(SUCCESS(ret))
    {    
        //if the calibration converges
        res = lms6002_spi_read(core, 0x00, &regs.Register_00.raw);   //read calibration value
        res = lms6002_spi_read(core, 0x35, &regs.Register_35.raw);
        regs.Register_35.Bit.dco_daccal = regs.Register_00.Bit.dc_regval;
        res = lms6002_spi_write(core, 0x35, regs.Register_35.raw);   //write TX:DCO_DACCAL
        res = lms6002_spi_read(core, 0x55, &regs.Register_55.raw);
        regs.Register_55.Bit.dco_daccal = regs.Register_00.Bit.dc_regval;
        res = lms6002_spi_write(core, 0x55, regs.Register_55.raw);   //write RX:DCO_DACCAL
    }

    //restore register
    regs.Register_09.Bit.clk_en_lpf = 0;
    lms6002_spi_write(core, 0x9, regs.Register_09.raw);

    return ret;
}

Result lms6002_lpf_tune(lms6002_core * core, unsigned ref_freq)
{
    Result ret=OK;
    unsigned i;
    unsigned char pa_en;
    lms6002_Register_Map regs;
    lms6002_pll_param pll_param;
    unsigned char pll_save[7];
    unsigned char calib;

    //Disable txVGA2
    lms6002_spi_read(core, 0x44, &regs.Register_44.raw);
    pa_en = regs.Register_44.Bit.pa_en;
    regs.Register_44.Bit.pa_en = 0;
    lms6002_spi_write(core, 0x44, regs.Register_44.raw);
    
    //Set txPLL to 320 MHz
    for(i=0; i<sizeof(pll_save); i++)
    {
        lms6002_spi_read(core, 0x10 + i, &pll_save[i]);
    }
    pll_param.ref_freq = ref_freq;
    pll_param.out_freq = ((unsigned)320e6);
    lms6002_pll_calculate(&pll_param);
    lms6002_pll_setparam(core, &pll_param, LMS6002_PLL_TX);

    lms6002_spi_read(core, 0x6, &regs.Register_06.raw);
    regs.Register_06.Bit.clksel_lpfcal = 0;
    lms6002_spi_write(core, 0x6, regs.Register_06.raw);
    regs.Register_06.Bit.pd_clklpfcal = 0;
    lms6002_spi_write(core, 0x6, regs.Register_06.raw);

    lms6002_spi_read(core, 0x7, &regs.Register_07.raw);
    regs.Register_07.Bit.en_cal_lpfcal = 1;
    lms6002_spi_write(core, 0x7, regs.Register_07.raw);

    regs.Register_06.Bit.rst_cal_lpfcal = 1;
    lms6002_spi_write(core, 0x6, regs.Register_06.raw);
    regs.Register_06.Bit.rst_cal_lpfcal = 0;
    lms6002_spi_write(core, 0x6, regs.Register_06.raw);

    //read calibration value
    lms6002_spi_read(core, 1, &calib);
    calib >>= 5;

#ifdef DEBUG
    for(i=0; i<5; i++)
    {
        unsigned char temp;
        lms6002_spi_read(core, 1, &temp);
        temp >>= 5;
        if(temp != calib)
        {
            debug("_lms6002_lpf_tune not done... %x\n", temp);
        }
    }
#endif

    //Write calibration value
    lms6002_spi_read(core, 0x36, &regs.Register_36.raw);
    regs.Register_36.Bit.rccal_lpf = calib;
    lms6002_spi_write(core, 0x36, regs.Register_36.raw);

    lms6002_spi_read(core, 0x56, &regs.Register_56.raw);
    regs.Register_56.Bit.rccal_lpf = calib;
    lms6002_spi_write(core, 0x56, regs.Register_56.raw);

    //Powerdown calibration circuit
    regs.Register_06.Bit.pd_clklpfcal = 1;
    lms6002_spi_write(core, 0x6, regs.Register_06.raw);
    regs.Register_07.Bit.en_cal_lpfcal = 0;
    lms6002_spi_write(core, 0x7, regs.Register_07.raw);

    //Restore PLL
    for(i=0; i<sizeof(pll_save); i++)
    {
        lms6002_spi_write(core, 0x10 + i, pll_save[i]);
    }

    //Restore txVGA2
    regs.Register_44.Bit.pa_en = pa_en;
    lms6002_spi_write(core, 0x44, regs.Register_44.raw);
    
    return ret;
}

Result lms6002_lpf_calibrateDCOffset(lms6002_core * core, LMS6002_DIR dir)
{
    Result ret = OK;
    Result lret;
    lms6002_Register_Map regs;
    unsigned int i;
    unsigned char reg9_save,reg56_save;
    unsigned char base_addr;

    ret = lms6002_spi_read(core, 0x09, &regs.Register_09.raw);
    reg9_save = regs.Register_09.raw;
    switch(dir)
    {
    default:
    case LMS6002_RX:
        regs.Register_09.Bit.clk_en_rx_lpf_dccal = 1;
        base_addr = 0x50;
        break;
    case LMS6002_TX:
        regs.Register_09.Bit.clk_en_tx_lpf_dccal = 1;
        base_addr = 0x30;
        break;
    }
    ret = lms6002_spi_write(core, 0x9, regs.Register_09.raw);

    //enable DC comp
    ret = lms6002_spi_read(core, base_addr + 6, &reg56_save);
    lms6002_spi_write(core, base_addr + 6, reg56_save & 0xF7);

    for (i=0;i<2;i++)
    {
        lret = fmcRadioSdr_RFDCCal(core, base_addr,i);
        if(FAILURE(lret)) ret = lret;
    }

    //Disable dccomp
    lms6002_spi_write(core, base_addr + 6, reg56_save | 0x8);

    lms6002_spi_write(core, 0x9, reg9_save);

    return ret;
}

Result lms6002_rxvga2_calibrate(lms6002_core * core)
{
    Result ret;
    unsigned int i;
    lms6002_Register_Map regs;
    unsigned int res;

    ret = lms6002_spi_read(core, 0x09, &regs.Register_09.raw);
    regs.Register_09.Bit.clk_en_vga2_dccal = 1;
    lms6002_spi_write(core, 0x9, regs.Register_09.raw);

    //enable dc comp...fix the rev 000 of the chip.
    res = lms6002_spi_read(core, 0x66, &regs.Register_66.raw);  //enable DC comp
    regs.Register_66.Bit.pd_calibration_dac_vga2a = 0;
    regs.Register_66.Bit.pd_calibration_comparator_vga2a = 0;
    regs.Register_66.Bit.pd_calibration_dac_vga2b = 0;
    regs.Register_66.Bit.pd_calibration_comparator_vga2b = 0;
    lms6002_spi_write(core, 0x66, regs.Register_66.raw);
    lms6002_spi_read(core, 0x6e, &regs.Register_6E.raw);
    regs.Register_6E.Bit.pd_vga2_comparator = 0;
    lms6002_spi_write(core, 0x6e, regs.Register_6E.raw);

    ret = lms6002_spi_read(core,0x09,&regs.Register_09.raw);

    for (i=0;i<5;i++)
    {
        ret = fmcRadioSdr_RFDCCal(core, 0x60,i);
        if(FAILURE(ret)) break;
    }

    //close DCComp after dc cal: mail lime
    regs.Register_09.Bit.clk_en_vga2_dccal = 0;
    lms6002_spi_write(core, 0x9, regs.Register_09.raw);
    //do if reg 0x0e = 0;
    //res = lms6002_spi_read(FMCRADIOSDR_RF, 0x64, &res);       //read reg.
    //lms6002_spi_write(core, 0x80|0x64, res|0x01);//set in test mode.
    //res = fmcRadioSdr_writeSPI(FMCRADIOSDR_RF,0x65,0);  //copy gain setting - already set.
    //fmcRadioSdr_writeSPI(FMCRADIOSDR_RF,0x80|0x68,res);

    //Disable DC calibration comparator
    regs.Register_6E.Bit.pd_vga2_comparator = 3;
    lms6002_spi_write(core, 0x6e, regs.Register_6E.raw);

    regs.Register_66.Bit.pd_calibration_comparator_vga2a = 1;
    regs.Register_66.Bit.pd_calibration_comparator_vga2b = 1;
    lms6002_spi_write(core, 0x66, regs.Register_66.raw);

    //xil_printf("rxVGA2 dc cal done\n");
    return ret;
}   

Result lms6002_rx_gain(lms6002_core * core, LMS6002_LNA_GAIN lna_gain, LMS6002_VGA1_GAIN rx_gain1, unsigned rx_gain2)
{
    Result ret;
    lms6002_Register_Map regs;

    //LNA
    ret = lms6002_spi_read(core, 0x75, &regs.Register_75.raw);
    regs.Register_75.Bit.lna_gain_mode = lna_gain;
    ret = lms6002_spi_write(core,0x75, regs.Register_75.raw);

    ret = lms6002_spi_read(core, 0x76, &regs.Register_76.raw);
    regs.Register_76.Bit.feedback_resistor = rx_gain1;
    ret = lms6002_spi_write(core,0x76, regs.Register_76.raw);

    //VGA2
    ret = lms6002_spi_read(core, 0x65, &regs.Register_65.raw);
    regs.Register_65.Bit.rx_vga2_gain = rx_gain2 / 3;
    ret = lms6002_spi_write(core,0x65, regs.Register_65.raw);
    
    return OK;
}

Result lms6002_tx_gain(lms6002_core * core, unsigned char tx_vga1_gain, unsigned char tx_vga2_gain)
{
    Result ret;
    lms6002_Register_Map regs;

    regs.Register_41.Bit.not_used = 0;
    regs.Register_41.Bit.txvga1_gain = tx_vga1_gain;
    ret = lms6002_spi_write(core, 0x41, regs.Register_41.raw);
    ret = lms6002_spi_read(core, 0x45, &regs.Register_45.raw);
    regs.Register_45.Bit.txvga2_gain = tx_vga2_gain;
    ret = lms6002_spi_write(core, 0x45, regs.Register_45.raw);

    return ret;
}

Result lms6002_close(lms6002_core * core)
{
    Result ret;
    ret = lms6002_pll_close(&core->tx_core);
    if(FAILURE(ret)) return ret;
    ret = lms6002_pll_close(&core->rx_core);
    return ret;
}

/* Private Functions */
Result lms6002_pll_open(lms6002_pll_core * core, unsigned char base_addr)
{
    core->base_addr = base_addr;
    return OK;
}

Result lms6002_pll_close(lms6002_pll_core * core)
{
    return OK;
}

Result lms6002_spi_read(lms6002_core * core, unsigned char addr, unsigned char * data)
{
    Result ret;
    unsigned wdata;
    wdata = addr;
    wdata <<= 8;
    ret = spi_core_write(core->spi, wdata);
    if(FAILURE(ret)) return ret;
    ret = spi_core_complete(core->spi);
    //FIXME unprotected data read
    *data = *(core->spi->data_regs);
    return ret;
}

Result lms6002_spi_write(lms6002_core * core, unsigned char addr, unsigned char data)
{
    unsigned wdata;
    wdata = addr | 0x80;
    wdata <<= 8;
    wdata |= data;
    if((addr  & ~0x80) == 0x9)
    {
        debug("writing %x to %i\n", data, addr);
    }
    return spi_core_write(core->spi, wdata);
}

Result lms6002_loopback(lms6002_core * core, LMS6002_LOOPBACK_MODE mode)
{
    Result ret;
    lms6002_Register_Map regs;    

    //Read values
    ret = lms6002_spi_read(core, 0x44, &regs.Register_44.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_read(core, 0x45, &regs.Register_45.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_read(core, 0x0b, &regs.Register_0B.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_read(core, 0x08, &regs.Register_08.raw);
    if(FAILURE(ret)) return ret;
    
    //printf("regs.Register_44.raw = 0x%8x\n",(unsigned int)regs.Register_44.raw);
    //printf("regs.Register_45.raw = 0x%8x\n",(unsigned int)regs.Register_45.raw);
    //printf("regs.Register_0B.raw = 0x%8x\n",(unsigned int)regs.Register_0B.raw);
    //printf("regs.Register_08.raw = 0x%8x\n",(unsigned int)regs.Register_08.raw);

    if (mode != LMS6002_LOOPBACK_DISABLED)
    {
        //Saving PA enable to core
        core->saved_pa = regs.Register_44.Bit.pa_en;
        //Deactivate TX VGA2
        regs.Register_44.Bit.pa_en = 0;
        //Activate AUX PA
        regs.Register_44.Bit.aux_pa_pd = 0;
        //RF loop back switch powered up
        regs.Register_0B.Bit.pd_rf_loopback = 1; 
        //Saving TX VGA2 Gain
        core->saved_gain = regs.Register_45.Bit.txvga2_gain;
        /*
        When DECODE control signals (user mode) (register 0x40) = 0 (default), loop back PA gain is controlled by VGA2GAIN (register 0x45) as below:
        0b00010 min gain (default)
        0b10101 mid gain
        0b00101 max gain
        */
        //Set AUX PA Gain
        regs.Register_45.Bit.txvga2_gain = 5; //full gain => 0b00101
    }
    else
    {
        //Activate TX VGA2 (PA 1 or PA2 from saved)
        regs.Register_44.Bit.pa_en = core->saved_pa;
        //Deactivate AUX PA
        regs.Register_44.Bit.aux_pa_pd = 1;
        //RF loop back switch powered down (default)
        regs.Register_0B.Bit.pd_rf_loopback = 0; 
        //Set TX VGA2 Gain
        regs.Register_45.Bit.txvga2_gain = core->saved_gain;        
    }
    /* 
    RF loop back control mode (regs.Register_08.Bit.lbrfen)
    - LMS6002_LOOPBACK_DISABLED = 0, RF loopback disabled (default)
    - LMS6002_LOOPBACK_LNA1     = 1, TXMIX output connected to LNA1 path
    - LMS6002_LOOPBACK_LNA2     = 2, TXMIX output connected to LNA2 path
    - LMS6002_LOOPBACK_LNA3     = 3, TXMIX output connected to LNA3 path
    */
    regs.Register_08.raw = 0;  //To be sure that LBEN_LPFIN, LBEN_VGA2IN and LBEN_OPIN are disabled
    regs.Register_08.Bit.lbrfen = mode;

    //Write values
    ret = lms6002_spi_write(core, 0x44, regs.Register_44.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_write(core, 0x45, regs.Register_45.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_write(core, 0x0b, regs.Register_0B.raw);
    if(FAILURE(ret)) return ret;
    ret = lms6002_spi_write(core, 0x08, regs.Register_08.raw);
    if(FAILURE(ret)) return ret;     

    return ret;
}

#endif
