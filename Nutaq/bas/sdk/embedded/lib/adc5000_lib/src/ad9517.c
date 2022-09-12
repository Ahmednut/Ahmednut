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
 * @file       ad9517.c
 * @brief      AD9517 configuration API
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
#include "ad9517.h"
#include "adc5000_defines.h"
#include "com_wrapper.h"
#include <string.h> //memset
#include <stdio.h>
#include "bas_debug.h"

const unsigned short ad9517_default_config_addr[] =
{
    0x00,                                           //SPI configuration
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, //pll control 1
    0x19, 0x1a, 0x1b, 0x1c, 0x1d,                   //pll control 2
    0xf0, 0xf1, 0xf4, 0xf5,                         //output 0-3
    0x140,0x141,0x142,0x143,                        //output 4-7
    0x190,0x191,0x192,                              //divider 0
    0x196,0x197,0x198,                              //divider 1
    0x199,0x19a,0x19b,0x19c,0x19d,                  //divider 2
    0x19e,0x19f,0x1a0,0x1a1,0x1a2,                  //divider 3
    0x1e0,0x1e1,                                    //vco divider, clock input
    0x230,0x232,                                    //Power down and flush register
};

const unsigned char ad9517_default_config_data[] =
{
    0x99,                                           //SPI configuration
    0x1c, 0x0a, 0x00, 0x0a, 0x0F, 0x00, 0x02, 0xb4, //pll control 1
    0x00, 0x00, 0x00, 0x87, 0x00,                   //pll control 2
    0x02, 0x0c, 0x02, 0x0c,                         //output 0-3
    0x00, 0x01, 0x00, 0x01,                         //output 4-7
    0xbc, 0x00, 0x00,                               //divider 0
    0x22, 0x80, 0x02,                               //divider 1
    0x00, 0x00, 0x00, 0x00, 0x00,                   //divider 2
    0x00, 0x00, 0x00, 0x00, 0x00,                   //divider 3
    0x00, 0x00,                                     //vco divider, clock input
    0x00, 0x01,                                     //Power down and flush register
};

Result ad9517_open(ad9517_core * core, com_core * com)
{
    core->com = com;
    return OK;
}

Result ad9517_close(ad9517_core * core)
{
    return OK;
}

// http://en.wikipedia.org/wiki/Greatest_common_divisor
static unsigned gcd ( unsigned a, unsigned b )
{
  unsigned c;
  while ( a != 0 )
  {
     c = a;
     a = b%a;
     b = c;
  }
  return b;
}

// http://en.wikipedia.org/wiki/Least_common_multiple
static unsigned lcm(unsigned a, unsigned b)
{
    return (b / gcd(a, b) ) * a;
}

static unsigned lcm_arr(unsigned * n, unsigned size)  
{  
    unsigned last_lcm, i;
    if(size < 2) return 0;
    last_lcm = lcm(n[0], n[1]);
    for(i=2; i < size; i++)
    {
        last_lcm = lcm(last_lcm, n[i]);
    }
    return last_lcm;
}

static Result ad9517_GetFactors(ad9517_param * param, unsigned int vco)
{
    unsigned psclr[] = {2,4,8,16,32};
    unsigned P, R, B, A;
    unsigned N, i, divi;
    unsigned pfdFreq, refTemp;
    
    divi = 0;
    while(1)
    {
        divi++;
        refTemp = param->ref_freq / divi;
        if ( refTemp < AD9517_PFD_MIN_FREQ )
        {
            return AD9517_PFDFREQ_OUT_OF_RANGE;
        }
        if( param->ref_freq % divi != 0) continue;
        pfdFreq = gcd(refTemp, vco);
        if ( (pfdFreq > param->pfd_freq_min) && (pfdFreq < param->pfd_freq_max)) break;
    }

    R = param->ref_freq / pfdFreq;
    if (R<1)
    {
        return AD9517_NO_VALID_R;
    }

    if (R>16383)
    {
        return AD9517_REF_CLOCK_TOO_HIGH;
    }

    if( vco <= 200e6) //P = 2 DM: <200 MHz
    {
        i=0;
    } else if (vco <= 1e9) { //P = 4 DM: <1000 MHz
        i=1;
    } else if (vco <= 2400e6) { //P = 8 DM: <2400 MHz
        i=2;
    } else if (vco <= param->vco_freq_max) { 
        i=3;
    }else {
        return AD9517_VCO_OUT_OF_RANGE;
    }
    // refIn / R = pfdFreq = vco / ( P*B + A)
    N = vco / pfdFreq;
    for(; i<(sizeof(psclr)/sizeof(int)); i++ )
    {
        P = psclr[i];
        B = N/P;
        A = N % P;
        if (vco/psclr[i] > 300e6) //max prescaler output frequency is 300 MHz
            continue;
        if (B == 2) //B must be >= 3 or bypassed
        {
            B--;
            A += psclr[i];
        }
        if ( !( B & ~0x1FFF ) ) //13 bit
        {
            if( !( A & ~0x3F ) ) //6 bit
            {
                param->Bfactor = B;
                param->Rfactor = R;
                param->Afactor = A;
                param->Pfactor = P;
                param->pfd_freq = pfdFreq;
                param->vco_freq = vco;
                return OK;
            }
        }
    }

    return AD9517_NO_VALID_SOLUTION;
}

static Result divider64_exist(unsigned divider)
{
    int i;

    for(i=32; i!=0; i--)
    {
        if(divider % i == 0)
        {
            if(divider / i <= AD9517_MAX_OUTPUT_DIVIDER)
            {
                return OK;
            }
        }
    }

    return AD9517_DIVIDER;
}

static Result ad9517_calculate_prescaler(ad9517_param * param, unsigned synth)
{
    unsigned char vco_divider_values[] = {1, 2, 3, 4, 5, 6};
    unsigned long long vco;
    int i;
    for (i = 0; i < sizeof(vco_divider_values); i++)
    {
        vco = ((unsigned long long)synth) * vco_divider_values[i];
        if( (vco <= param->vco_freq_max) && (vco >= param->vco_freq_min))
        {
            param->vco_divider = vco_divider_values[i];
            param->vco_freq = (unsigned)vco;
            return OK;
        }
    }   
    return AD9517_VCODIVIDER;
}

static int present_in_array(unsigned value, unsigned * arr, int array_size)
{
    int i;
    for(i=0; i<array_size; i++)
    {
        if(arr[i] == value)
        {
            return 1;
        }
    }
    return 0;
}

typedef struct S_synth_freq_generator {
    unsigned multiple;
    unsigned i;
    unsigned max;
} synth_freq_generator;

static void synth_freq_generator_init(synth_freq_generator * gen, unsigned multiple, unsigned max)
{
    gen->i = 1;
    gen->multiple = multiple;
    gen->max = max;
}

static int synth_freq_generator_next(synth_freq_generator * gen, unsigned * freq_out)
{
    unsigned long long value;
    value = ((unsigned long long) gen->multiple) * gen->i;
    if(value > gen->max) return 0;
    gen->i++;
    *freq_out = (unsigned)value;
    return 1;
}

//larger freq than AD9517_MAXSYNTHFREQ cannot be synthesized unless they equal ref_freq
Result ad9517_calculate(ad9517_param * param)
{
    Result ret;
    unsigned i;
    int j;
    unsigned unique_freq[4];
    unsigned unique_freq_size;
    unsigned multiple;
    unsigned test_synth;
    unsigned divider_tmp;
    synth_freq_generator synth_freq;
    
    for(i=0; i<2; i++)
    {
        if (param->out_freq[i]  > ((unsigned)2950e6))
        {
            return AD9517FREQHIGH;
        }
        else if(param->out_freq[i]  < ((unsigned)400e6))
        {
               return AD9517FREQLOW;
        }

    }

    for(i=2; i<4; i++)
    {
        if (param->out_freq[i]  > ((unsigned)800e6))
        {
            return AD9517FREQHIGH;
        }
    }

    unique_freq_size = 0;

    for (i=0; i<4; i++)
    {
        if(!present_in_array(param->out_freq[i], unique_freq, unique_freq_size) && param->out_freq[i] <= AD9517_MAXSYNTHFREQ)
        {
            unique_freq[unique_freq_size++] = param->out_freq[i];
        }
    }

    //Calculate possible VCO frequencies
    if( unique_freq_size > 1)
    {
        multiple = lcm_arr(unique_freq, unique_freq_size);
    }else{
        multiple = unique_freq[0];
    }

    for(i=0; i<unique_freq_size; i++)
    {
        if(multiple % unique_freq[i] != 0)
            return AD9517OVERFLOW;
    }

    synth_freq_generator_init(&synth_freq, multiple, param->vco_freq_max);

    while (synth_freq_generator_next(&synth_freq, &test_synth))
    {
        ret = ad9517_calculate_prescaler(param, test_synth);
        if(SUCCESS(ret))
        {
            ret = ad9517_GetFactors(param, param->vco_freq);
            if(FAILURE(ret)) continue;
            for (j=0; j<4; j++)
            {
                if(param->out_freq[j] > AD9517_MAXSYNTHFREQ && param->out_freq[j] != param->vco_freq) return AD9517_NO_VALID_SOLUTION;
            }
            for (j=0; j<2; j++)
            {
                if(param->out_freq[j])
                {
                    divider_tmp = test_synth / param->out_freq[j];
                    if(divider_tmp > AD9517_MAX_OUTPUT_DIVIDER)
                    {
                        return AD9517_DIVIDER;
                    }else{
                        param->out_divider[j] = (unsigned short) divider_tmp;
                    }
                }
            }
            for(j=2; j<4; j++)
            {
                if(param->out_freq[j])
                {
                    divider_tmp = test_synth / param->out_freq[j];
                    ret = divider64_exist(divider_tmp);
                    if(FAILURE(ret))
                    {
                        return ret;
                    }else{
                        param->out_divider[j] = (unsigned short) divider_tmp;
                    }
                }
            }
            return ret;
        }
    }
    return AD9517_NO_VALID_SOLUTION;
}

#ifndef ONLY_CALCULATE

/*static Result ADAC250_PLLSoftReset(ADAC250Board * board)
{
    AD9511_Register_Map_t reg;
    ADAC250_ReadSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_00, 1, &reg.Register_00.Byte);
    reg.Register_00.Bit.Soft_Reset=1;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_00, 1, &reg.Register_00.Byte);
    reg.Register_00.Bit.Soft_Reset=0;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_00, 1, &reg.Register_00.Byte);
    return OK;
}*/

//P value to value we need to set in the register
static char prescaler_2reg(char value)
{
    switch(value)
    {
    default:
    case 2:
        return 2;
    case 4:
        return 3;
    case 8:
        return 4;
    case 16:
        return 5;
    case 32:
        return 6;
    }
}


static unsigned char divider_2reg(unsigned char value)
{
    unsigned char tmp;
    unsigned char reg;
    switch(value)
    {
    case 0:
    case 1:
        return 0;
    default:
        tmp = value >> 1; // div 2
        tmp--;
        reg = tmp | tmp<<4;
        if(value & 0x1)
        {
            reg++;
        }
    }
    return reg;
}

static Result divider64_2reg(unsigned short divider, unsigned char * div1_reg, unsigned char * div2_reg, unsigned char * div1, unsigned char * div2)
{
    int i;
    unsigned tmp;

    for(i=32; i!=0; i--)
    {
        if(divider % i == 0)
        {
            if(divider / i <= AD9517_MAX_OUTPUT_DIVIDER)
            {
                *div1_reg = divider_2reg(i);
                *div1 = i;
                tmp = divider / i;
                *div2_reg = divider_2reg(tmp);
                *div2 = tmp;
                return OK;
            }
        }
    }
    return AD9517_DIVIDER;
}


Result ad9517_setparam(ad9517_core * core, ad9517_param * param)
{
    Result ret;
    ad9517_Register_Map regs;
    unsigned char div1, div2;

    memset(&regs, 0, sizeof(regs));

    //set factors
    regs.Register_011.Bit.r_divider_lsb = param->Rfactor & 0xFF;
    regs.Register_012.Bit.r_divider_msb = param->Rfactor>>8;
    regs.Register_013.Bit.a_counter = param->Afactor;
    regs.Register_014.Bit.b_counter_lsb = param->Bfactor & 0xFF;
    regs.Register_015.Bit.b_counter_msb = param->Bfactor>>8; 
    regs.Register_016.Bit.prescaler = prescaler_2reg(param->Pfactor);
    regs.Register_016.Bit.b_counter_bypass = param->Bfactor == 1 ? 1 : 0;
    ret = com_write_arr(core->com, COM_PLL, 0x11, &regs.RegisterArray[0x11], 6);
    if(FAILURE(ret)) return ret;

    //TODO disable when output freq is 0

    //set dividers
    //CLK out <-> OUT0
	if (param->out_freq[0] == param->vco_freq)
    {
	 debug("out_divider %d top\n", 0);
        ret = com_read(core->com, COM_PLL, 0x192, &regs.RegisterArray[0x192]);
		if(FAILURE(ret)) return ret;
		
        regs.Register_192.Bit.divider0_direct2output = 1;
        ret = com_write_arr(core->com, COM_PLL, 0x192, &regs.RegisterArray[0x192], 1);
		if(FAILURE(ret)) return ret;
    }else{
	 debug("out_divider %d  bot\n", 0);
        regs.Register_190.raw = divider_2reg((unsigned char)param->out_divider[0]);
        ret = com_read_arr(core->com, COM_PLL, 0x191, &regs.RegisterArray[0x191], 2);
		if(FAILURE(ret)) return ret;
	 	
        if(param->out_divider[0] <= 1) 
	 {
		regs.Register_191.Bit.divider0_bypass = 1;
	 }
	 else		 
	 {
		regs.Register_191.Bit.divider0_bypass = 0;
	 }
        regs.Register_192.Bit.divider0_direct2output = 0;
        ret = com_write_arr(core->com, COM_PLL, 0x190, &regs.RegisterArray[0x190], 3);
		if(FAILURE(ret)) return ret;
    }
    
    //ADC <-> OUT1
    if (param->out_freq[1] == param->vco_freq)
    {	 
	 debug("out_divider %d top\n",1);
        ret = com_read(core->com, COM_PLL, 0x198, &regs.RegisterArray[0x198]);
		if(FAILURE(ret)) return ret;

        regs.Register_198.Bit.divider1_direct2output = 1;
        ret = com_write_arr(core->com, COM_PLL, 0x198, &regs.RegisterArray[0x198], 1);
		if(FAILURE(ret)) return ret;
		
    }else{
	 debug("out_divider %d  bot\n",1);
        regs.Register_196.raw = divider_2reg((unsigned char)param->out_divider[1]);
        ret = com_read_arr(core->com, COM_PLL, 0x197, &regs.RegisterArray[0x197], 2);
		if(FAILURE(ret)) return ret;
		
        if(param->out_divider[1] <= 1) 
	 {
		regs.Register_197.Bit.divider1_bypass = 1;
	 }
	 else		 
	 {
		regs.Register_197.Bit.divider1_bypass = 0;
	 }
        regs.Register_198.Bit.divider1_direct2output = 0;
        ret = com_write_arr(core->com, COM_PLL, 0x196, &regs.RegisterArray[0x196], 3);
		if(FAILURE(ret)) return ret;
		
    }


    //Sync <-> OUT2
    ret = divider64_2reg(param->out_divider[2], &regs.RegisterArray[0x199], &regs.RegisterArray[0x19B], &div1, &div2);
	if(FAILURE(ret)) return ret;
	
    ret = com_write(core->com, COM_PLL, 0x199, &regs.RegisterArray[0x199]);
	if(FAILURE(ret)) return ret;
		
    ret = com_read(core->com, COM_PLL, 0x19C, &regs.RegisterArray[0x19C]);
	if(FAILURE(ret)) return ret;
		
    regs.Register_19C.Bit.divider2DOT1_bypass = div1 <= 1 ? 1 : 0;
    regs.Register_19C.Bit.divider2DOT2_bypass = div2 <= 1 ? 1 : 0;
    ret = com_write_arr(core->com, COM_PLL, 0x19B, &regs.RegisterArray[0x19B], 2);
	if(FAILURE(ret)) return ret;
	

    //FMC <-> OUT3
    ret = divider64_2reg(param->out_divider[3], &regs.RegisterArray[0x19E], &regs.RegisterArray[0x1A0], &div1, &div2);
	if(FAILURE(ret)) return ret;
	
    ret = com_write(core->com, COM_PLL, 0x19E, &regs.RegisterArray[0x19E]);
	if(FAILURE(ret)) return ret;
	
    ret = com_read(core->com, COM_PLL, 0x1A1, &regs.RegisterArray[0x1A1]);
	if(FAILURE(ret)) return ret;
	
    regs.Register_1A1.Bit.divider3DOT1_bypass = div1 <= 1 ? 1 : 0;
    regs.Register_1A1.Bit.divider3DOT2_bypass = div2 <= 1 ? 1 : 0;
    ret = com_write_arr(core->com, COM_PLL, 0x1A0, &regs.RegisterArray[0x1A0], 2);
	if(FAILURE(ret)) return ret;
	

    //Configure VCO divider
    if(param->vco_divider == 1)
    {
        regs.Register_1E1.raw = 0;
        regs.Register_1E1.Bit.vco_divider_bypass = 1;
        ret = com_write(core->com, COM_PLL, 0x1E1, &regs.RegisterArray[0x1E1]);
		if(FAILURE(ret)) return ret;
	
    }else{
        regs.Register_1E0.raw = 0;
        regs.Register_1E0.Bit.vco_divider = param->vco_divider - 2;
        regs.Register_1E1.raw = 0;
        ret = com_write_arr(core->com, COM_PLL, 0x1E0, &regs.RegisterArray[0x1E0], 2);
		if(FAILURE(ret)) return ret;
	
    }

    ret = ad9517_trigRegisterTransfer(core);
	if(FAILURE(ret)) return ret;
	
    ret = ad9517_SyncOutput(core);
	if(FAILURE(ret)) return ret;
	
    return OK;
}


Result ad9517_trigRegisterTransfer(ad9517_core * core)
{
    ad9517_Register_Map regs;

    regs.Register_232.raw = 0;
    regs.Register_232.Bit.commit = 1;
    return com_write(core->com, COM_PLL, 0x232, &regs.RegisterArray[0x232]);
}


Result ad9517_SyncOutput(ad9517_core * core)
{
    ad9517_Register_Map regs;
    Result ret;
	
    //Reset All counters
    ret = com_read(core->com, COM_PLL, 0x16, &regs.RegisterArray[0x016]);
	if(FAILURE(ret)) return ret;
	
    regs.Register_016.Bit.reset_all_counter = 1;
    ret = com_write(core->com, COM_PLL, 0x16, &regs.RegisterArray[0x016]);
	if(FAILURE(ret)) return ret;
	
    regs.Register_016.Bit.reset_all_counter = 0;
    ret = com_write(core->com, COM_PLL, 0x16, &regs.RegisterArray[0x016]);
	if(FAILURE(ret)) return ret;
	

    //soft SYNCB
    ret = com_read(core->com, COM_PLL, 0x231, &regs.RegisterArray[0x231]);
	if(FAILURE(ret)) return ret;
	
    regs.Register_230.Bit.soft_sync = 1;
    ret = com_write(core->com, COM_PLL, 0x231, &regs.RegisterArray[0x231]);
	if(FAILURE(ret)) return ret;
	
    regs.Register_230.Bit.soft_sync = 0;
    ret = com_write(core->com, COM_PLL, 0x231, &regs.RegisterArray[0x231]);
	if(FAILURE(ret)) return ret;
	

    return OK;
}

Result ad9517_reset(ad9517_core * core)
{
    Result ret;
    int i;

    for(i=0; i<sizeof(ad9517_default_config_addr)/sizeof(ad9517_default_config_addr[0]); i++)
    {
        ret = com_write(core->com, COM_PLL, ad9517_default_config_addr[i], &ad9517_default_config_data[i]);
        if(FAILURE(ret)) return ret;
    }

    return OK;
}

Result ad9517_lock_state(ad9517_core * core, ad9517_lock_status * state)
{
    Result ret;
    ad9517_Register_Map regs;
    ret = com_read(core->com, COM_PLL, 0x1F, &regs.RegisterArray[0x01F]);
	if(FAILURE(ret)) return ret;
    if(state)
    {
        if(regs.Register_01F.Bit.digital_lock)
        {
             *state = AD9517_PLL_LOCKED;
        }
	 else
	 {
            *state = AD9517_PLL_UNLOCKED;
        }
    }

    return ret;
}

#endif

