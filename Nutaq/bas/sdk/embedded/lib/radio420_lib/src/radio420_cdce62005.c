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
 * @file       radio420_cdce62005.c  
 * @brief      Calculate valid configuration and configure the cdce62005 PLL
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
#include "radio420_cdce62005.h"
#include "radio420_cdce62005_regs.h"
#ifndef __EAPI_BUILD__
#endif

// Point the the pfRadio420_usleep function pointer define in the fmc_radio.c
extern int (*pfRadio420_usleep)(unsigned int usec);

const unsigned char possible_output_divider[][2] = {{1,0x20}, {2,0x40}, {3,0x41}, {4,0x42}, {5,0x43}, {6,0x01}, {8,0x02}, {10,0x03}, {12,0x05}, {16,0x06}, {18,0x09}, {20,0x07}, {24,0x0a},  {30,0x0b}, {32,0x0e}, {36,0x15}, {40,0x0f}, {42,0x19}, {48,0x16}, {50,0x13}, {56,0x1a}, {60,0x17}, {64,0x1e}, {70,0x1b}, {80,0x1f}};

//feedback divider setting lookup - from datasheet
// { divider value, value to be written in register }
const unsigned short possible_feedback_divider0[][2] = {{8,0x0}, {12,0x1}, {16,0x2}, {20,0x3}, {24,0x5}, {32,0x6}, {36,0x9}, {40,0x7}, {48,0xa}, {56,0x18}, {60,0xb}, {64,0xe}, {72,0x15}, {80,0xf}, {84,0x19}, {96,0x16}, {100,0x13}, {108,0x49}, {112,0x1a}, {120,0x17}, {128,0x1e}, {140,0x1b}, {144,0x35}, {160,0x1f}, {168,0x39}, {180,0x4b}, {192,0x36}, {200,0x33}, {216,0x55}, {224,0x3a}, {240,0x37}, {252,0x59}, {256,0x3e}, {280,0x3b}, {288,0x56}, {300,0x53}, {320,0x3f}, {336,0x59}, {360,0x57}, {384,0x5e}, {392,0xd8}, {400,0x73}, {420,0x5b}, {432,0xb5}, {448,0x79}, {480,0x5f}, {500,0x93}, {504,0xb9}, {512,0x7e}, {560,0x7b}, {576,0xb6}, {588,0xd9}, {600,0x97}, {640,0x7f}, {672,0xba}, {700,0x9b}, {720,0xb7}, {768,0xbe}, {784,0xda}, {800,0x9f}, {840,0xbb}, {896,0xde}, {960,0xbf}, {980,0xdb}, {1024,0xfe}, {1120,0xdf}, {1280,0xff}};

//feedback bypass divider lookup - from datasheet
const unsigned char possible_feedback_divider1[][2] = {{1, 0x7}, {2, 0x0}, {5, 0x1}, {8, 0x2}, {10, 0x3}, {16, 0x4}, {20, 0x5}};


//90 degree Phase adjust value for divider - from datasheet
//const unsigned char phase_adjust_lookup[][2] = {{2, 0x01}, {3, 0x01}, {4, 0x01}, {5, 0x01}, {6, 0x02}, {8, 0x02}, {10, 0x03}, {12, 0x08}, {16, 0x08}, {18, 0x0a}, {20, 0x08}, {24, 0x0a}, {30, 0x0b}, {32, 0x10}, {36, 0x18}, {40, 0x10}, {42, 0x19}, {48, 0x18}, {50, 0x12}, {56, 0x1a}, {60, 0x18}, {64, 0x20}, {70, 0x1b}, {80, 0x20}};

//180 degree Phase adjust value for divider - from datasheet
const unsigned char phase_adjust_lookup[][2] = {{2, 0x01}, {3, 0x02}, {4, 0x02}, {5, 0x03}, {6, 0x40}, {8, 0x40}, {10, 0x40}, {12, 0x10}, {16, 0x10}, {18, 0x18}, {20, 0x10}, {24, 0x18}, {30, 0x18}, {32, 0x20}, {36, 0x40}, {40, 0x20}, {42, 0x40}, {48, 0x40}, {50, 0x40}, {56, 0x40}, {60, 0x40}, {64, 0x40}, {70, 0x40}, {80, 0x40}};


//See TI excel sheet for computation
/*frequency, charge pump current, R2 */
const unsigned loop_filter_config[][3] = {{360000000, 0x3, 0x18}, {480000000, 0x3, 0x15}, {540000000
, 0x3, 0x13},  {600000000, 0x3, 0x18}, {720000000, 0x3, 0x18}, {960000000, 0x3, 0x17}, {1080000000, 0x3, 0x17} };

unsigned int cdce62005_shadow[9] = {
                               0x2106032,
                               0x2186030,
                               0x2186030,
                               0x2186030,
                               0xeb84031,
                               0x284c1e4,
                               0xa28e4f8,
                               0xbd8875f,
                               0x20008cd,
                               };

static int output_divider_exist(unsigned char div)
{
    int i;
    for (i=0; i < sizeof(possible_output_divider) /2; i++)
    {
        if (possible_output_divider[i][0] == div)
        {
            return 1;
        }
    }
    return 0;
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

static Result cdce62005_calculate_synth_freq(unsigned freq1, unsigned freq2, unsigned * synth_freq, unsigned * synth_freq_size)
{
    unsigned synth, multiple, i, k;
    unsigned div1, div2;

    synth = multiple = lcm(freq1, freq2);

    //check for overflow, could happen for large coprime numbers
    if(multiple % freq1 != 0 || multiple % freq2 != 0)
    {
        return CDCE62005_OVERFLOW;
    }

    i = 2;
    k = 0;
    while(synth <= CDCE62005_SYNTH_FREQ_MAX)
    {
        if(synth >= CDCE62005_SYNTH_FREQ_MIN)
        {
            div1 = synth / freq1;
            div2 = synth / freq2;
            if ( output_divider_exist(div1) && output_divider_exist(div2))
            {
                synth_freq[k] = synth;
                k++;
            }
            if(k >= *synth_freq_size)
                break;
        }
        synth = multiple * i;
        i++;
    }
    *synth_freq_size = k;
    return k>0 ? OK : CDCE62005DIVIDER;
}

static Result cdce62005_calculate_factors(cdce62005_param * param, unsigned synth)
{
    int i, j;
    unsigned total_divider, pfd_freq, input_divider;

    for (i=0; i < sizeof(possible_feedback_divider0) / sizeof(unsigned short) / 2; i++)
    {
        for (j=0; j < sizeof(possible_feedback_divider1) / 2; j++)
        {
            total_divider = (unsigned)possible_feedback_divider0[i][0];
            total_divider *= (unsigned)possible_feedback_divider1[j][0];
            pfd_freq = synth / total_divider;
            if( (pfd_freq >= param->pfd_freq_min) && (pfd_freq <= param->pfd_freq_max))
            {
                input_divider = param->ref_freq / pfd_freq;
                if ( (input_divider >= 1) && input_divider <= 256)
                {
                    if ( input_divider * pfd_freq == param->ref_freq)
                    {
                        param->input_divider = input_divider;
                        param->pfd_freq = pfd_freq;
                        param->feedback_divider = total_divider;
                        return OK;
                    }
                }
            }
        }
    }
    return CDCE62005_PFDIMPOSSIBLE;
}

static Result cdce62005_calculate_prescaler(cdce62005_param * param, unsigned synth)
{
    unsigned char prescaler_values[] = {2, 3, 4, 5};
    unsigned vco;
    int i;
    for (i = 0; i < sizeof(prescaler_values); i++)
    {
        vco = synth * prescaler_values[i];
        if( (vco <= CDCE62005_VCO_FREQ_MAX) && (vco >= CDCE62005_VCO_FREQ_MIN))
        {
            param->prescale_divider = prescaler_values[i];
            param->vco_freq = vco;
            return OK;
        }
    }   
    return CDCE62005_NOPRESCALER;
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

Result cdce62005_calculate(cdce62005_param * param)
{
    Result ret;
    unsigned i, j;
    unsigned synth_freq[20];
    unsigned unique_freq[5];
    unsigned unique_freq1, unique_freq2;
    unsigned unique_freq_size;
    unsigned synth_freq_size = sizeof(synth_freq) / sizeof(unsigned);
    unsigned divider_tmp;
    
    //check if it is possible to bypass VCO
    unique_freq_size = 0;
    for(i=0; i<5; i++)
    {
        if ( param->out_freq[i] == 0) continue;

        if ( param->ref_freq % param->out_freq[i] == 0)
        {
            divider_tmp = param->ref_freq / param->out_freq[i];
            if(output_divider_exist(divider_tmp))
            {
                param->vco_bypass[i] = 1;
                param->out_divider[i] = divider_tmp;
            }else{
                if(!present_in_array(param->out_freq[i], unique_freq, unique_freq_size))
                {
                    unique_freq[unique_freq_size++] = param->out_freq[i];
                }
            }
        }else{
            if(!present_in_array(param->out_freq[i], unique_freq, unique_freq_size))
            {
                unique_freq[unique_freq_size++] = param->out_freq[i];
            }
        }
    }

    if(unique_freq_size > 2) return CDCE62005_INVALIDFREQ;

    if(unique_freq_size == 0)
    {
        param->vco_bypass_all = 1;
        return OK;
    }

    if(unique_freq_size == 1)
    {
    	unique_freq1 = unique_freq2 = unique_freq[0];
    }else{
    	unique_freq1 = unique_freq[0];
    	unique_freq2 = unique_freq[1];
    }
    //support only 2 uniques output freq...
    ret = cdce62005_calculate_synth_freq(unique_freq1, unique_freq2, synth_freq, &synth_freq_size);
    if(FAILURE(ret)) return ret;
    for (i=0; i < synth_freq_size; i++)
    {
        ret = cdce62005_calculate_factors(param, synth_freq[i]);
        if(SUCCESS(ret))
        {
            ret = cdce62005_calculate_prescaler(param, synth_freq[i]);
            param->synth_freq = synth_freq[i];
            for(j=0; j<5; j++)
            {
                if(param->out_freq[j] && !param->vco_bypass[j])
                {
                    divider_tmp = synth_freq[i] / param->out_freq[j];
                    if(divider_tmp > CDCE62005_VCO_MAX_OUTPUT_DIVIDER)
                    {
                        return CDCE62005_OUTPUTDIVIMPOSSIBLE;
                    }else{
                        param->out_divider[j] = divider_tmp;
                    }
                }
            }
            return ret;
        }
    }

    return CDCE62005_NOSOLUTION;
}


#ifndef __ONLY_CALCULATE__
Result cdce62005_spi_read(spi_core_t * spi, unsigned addr, unsigned * data)
{
    //PLL read not working
/*    addr &= 0xF;
    *data = (addr << 4) | 0xE;

    return spi_core_read(spi, data);*/

    *data = cdce62005_shadow[addr];
    return OK;
}

Result cdce62005_spi_write(spi_core_t * spi, unsigned addr, unsigned data)
{
    unsigned data2send;
    addr &= 0xF;
    data2send = (data << 4) | addr;
    cdce62005_shadow[addr] = data;
    return spi_core_write(spi, data2send);
}

static Result divider2reg(unsigned char div, unsigned char * regs)
{
    unsigned i;
    for(i=0; i < sizeof(possible_output_divider)/2; i++)
    {
        if(possible_output_divider[i][0] == div)
        {
            *regs = possible_output_divider[i][1];
            return OK;
        }
    }
    return CDCE62005DIVIDER;
}

static Result prescaler2reg(unsigned char prescaler, unsigned char * regs)
{
    unsigned char prescaler_values[][2] = { {5, 0x0}, {4, 0x2}, {3, 0x1}, {2, 0x3}};
    int i;

    for (i=0; i < sizeof(prescaler_values) / 2; i++)
    {
        if( prescaler_values[i][0] == prescaler )
        {
            *regs = prescaler_values[i][1];
            return OK;
        }
    }
    return CDCE62005_INVALIDPRESCALER;
}


Result cdce62005_setparam(cdce62005_param * param, spi_core_t * spi)
{
    Result ret;
    int i, j;
    unsigned char div_tmp;
    cdce62005_Register_Map regs;
    unsigned char r2, cp;

    for (i=0; i<=8; i++)
    {
        ret = cdce62005_spi_read(spi, i, &regs.RegisterArray[i]);
        if(FAILURE(ret)) return ret;
    }

    //disable outputs
    regs.Register_8.Bit.sync = 0;
    ret = cdce62005_spi_write(spi, 8, regs.RegisterArray[8]);

    //loop filter
    for (i=0; i < sizeof(loop_filter_config)/sizeof(unsigned)/3; i++)
    {
        if(param->synth_freq == loop_filter_config[i][0])
        {
            //found config
            cp = loop_filter_config[i][1];
            r2 = loop_filter_config[i][2];
            break;
        }
    }

    if (i == sizeof(loop_filter_config)/sizeof(unsigned)/3)
    {
        //No config found -- Fallback
        //r2 24 ohm
        r2 = 0x1d;
        //charge pump 1000 uA
        cp = 0x08;
    }
    
    regs.Register_6.Bit.charge_pump_current = cp;
    regs.Register_7.Bit.loopfilter_r2 = r2;
    //r3 5 k ohm
    regs.Register_7.Bit.loopfilter_r3 = 0x3;
    //c1 1.5 pF
    regs.Register_7.Bit.loopfilter_c1 = 0x1;
    //c2 473.5 pF
    regs.Register_7.Bit.loopfilter_c2 = 0x1f;
    //c3 2.5 pF
    regs.Register_7.Bit.loopfilter_c3 = 0x1;

    //Reference divider
    if(param->secondary_clock)
    {
        regs.Register_0.Bit.primary_predivider = 0;
        regs.Register_1.Bit.secondary_predivider = 2;
    }else{
        regs.Register_0.Bit.primary_predivider = 2;
        regs.Register_1.Bit.secondary_predivider = 0;
    }
    //divide by 1
    regs.Register_2.Bit.reference_divider = 0;
    regs.Register_3.Bit.reference_divider = 0;
    regs.Register_4.Bit.reserved1 = 1;

    //smart mux
    regs.Register_5.Bit.soft_input_sel = 1;
    if(param->secondary_clock)
    {
        regs.Register_5.Bit.clock_select = CDCE62005_CLOCK_SEL_SEC;
        regs.Register_5.Bit.buf_select = 0x2; //LVPECL
        regs.Register_5.Bit.dc_termination_en = 1;
    }else{
        regs.Register_5.Bit.clock_select = CDCE62005_CLOCK_SEL_PRI;
        regs.Register_5.Bit.buf_select = 0x0; //LVCMOS
        regs.Register_5.Bit.dc_termination_en = 0;
    }

    //prescale divider
    ret = prescaler2reg(param->prescale_divider, &div_tmp);
    regs.Register_6.Bit.prescaler = div_tmp;

    //choose VCO
    if ( param->vco_freq > ((unsigned)2040e6))
    {
        regs.Register_6.Bit.vco_range = 1;
    }else{
        regs.Register_6.Bit.vco_range = 0;
    }

    //input divider
    regs.Register_5.Bit.input_divider = param->input_divider - 1;

    if( !(param->vco_bypass[0] == 1 && param->vco_bypass[1] == 1 && param->vco_bypass[2] == 1 && param->vco_bypass[3] == 1 && param->vco_bypass[4] == 1))
    {
        unsigned feedback_div0, feedback_div1;
        unsigned test_divider;
        unsigned found = 0;
        //feedback divider
        for (i=0; i < sizeof(possible_feedback_divider0) / sizeof(unsigned short) / 2; i++)
        {
            for (j=0; j < sizeof(possible_feedback_divider1) / 2; j++)
            {
                feedback_div0 = possible_feedback_divider0[i][0];
                feedback_div1 = possible_feedback_divider1[j][0];
                test_divider = feedback_div0 * feedback_div1;
                if( test_divider == param->feedback_divider)
                {
                    found = 1;
                    goto feedback_divider_exit;
                }
            }
        }
        feedback_divider_exit:
        if(found)
        {
            regs.Register_6.Bit.feedback_divider = possible_feedback_divider0[i][1];
            regs.Register_6.Bit.divider_bypass = possible_feedback_divider1[j][1];
        }else{
            return CDCE62005DIVIDER;
        }
    }

    //Output 0
    regs.Register_0.Bit.phase_adjust = 0;
    if(param->vco_bypass[0])
    {
        regs.Register_0.Bit.output_mux = param->secondary_clock ? 1 : 0;
    }else{
        regs.Register_0.Bit.output_mux = 3;   
    }
    if (param->out_divider[0] >= 1)
    {
        regs.Register_0.Bit.divider_enable = 1;
        ret = divider2reg(param->out_divider[0], &div_tmp);
        if(FAILURE(ret)) return CDCE62005IVAL;
        regs.Register_0.Bit.divider = div_tmp;
    }else{
        regs.Register_0.Bit.divider_enable = 0;
    }
    
    //Output 1
    regs.Register_1.Bit.phase_adjust = 0;
    if(param->vco_bypass[1])
    {
        regs.Register_1.Bit.output_mux = param->secondary_clock ? 1 : 0;
    }else{
        regs.Register_1.Bit.output_mux = 3;
    }
    if (param->out_divider[1] >= 1)
    {
        regs.Register_1.Bit.divider_enable = 1;
        ret = divider2reg(param->out_divider[1], &div_tmp);
        if(FAILURE(ret)) return CDCE62005IVAL;
        regs.Register_1.Bit.divider = div_tmp;
    }else{
        regs.Register_1.Bit.divider_enable = 0;
    }
    
    
    //Phase is 180 degrees for FPGA to DAC propagation delay for acq freq > 60 MHz
    if (param->out_freq[1] > 60000000)
    {
        for(i=0; i<sizeof(phase_adjust_lookup)/2; i++)
        {
            if(phase_adjust_lookup[i][0] == param->out_divider[1])
            {
                regs.Register_1.Bit.phase_adjust = phase_adjust_lookup[i][1];
                break;
            }
        }
    }
    
    //Output 2
    regs.Register_2.Bit.phase_adjust = 0;
    if(param->vco_bypass[2])
    {
        regs.Register_2.Bit.output_mux = param->secondary_clock ? 1 : 0;
    }else{
        regs.Register_2.Bit.output_mux = 3;
    }
    if (param->out_divider[2] >= 1)
    {
        regs.Register_2.Bit.divider_enable = 1;
        ret = divider2reg(param->out_divider[2], &div_tmp);
        if(FAILURE(ret)) return CDCE62005IVAL;
        regs.Register_2.Bit.divider = div_tmp;
    }else{
        regs.Register_2.Bit.divider_enable = 0;
    }

    //Output 3
    regs.Register_3.Bit.phase_adjust = 0;
    if(param->vco_bypass[3])
    {
        regs.Register_3.Bit.output_mux = param->secondary_clock ? 1 : 0;
    }else{
        regs.Register_3.Bit.output_mux = 3;
    }
    if (param->out_divider[3] >= 1)
    {
        regs.Register_3.Bit.divider_enable = 1;
        ret = divider2reg(param->out_divider[3], &div_tmp);
        if(FAILURE(ret)) return CDCE62005IVAL;
        regs.Register_3.Bit.divider = div_tmp;
    }else{
        regs.Register_3.Bit.divider_enable = 0;
    }

    //Output 4
    regs.Register_4.Bit.phase_adjust = 0x0;
    //regs.Register_4.Bit.hi_swing = 0;
    if(param->vco_bypass[4])
    {
        regs.Register_4.Bit.output_mux = param->secondary_clock ? 1 : 0;
    }else{
        regs.Register_4.Bit.output_mux = 3;
    }
    if (param->out_divider[4] >= 1)
    {
        regs.Register_4.Bit.divider_enable = 1;
        ret = divider2reg(param->out_divider[4], &div_tmp);
        if(FAILURE(ret)) return CDCE62005IVAL;
        regs.Register_4.Bit.divider = div_tmp;
    }else{
        regs.Register_4.Bit.divider_enable = 0;
    }


    for (i=0; i<=8; i++)
    {
        cdce62005_spi_write(spi, i, regs.RegisterArray[i]);
    }

    //calibrate VCO to new Freq.
    regs.Register_6.Bit.pll_calibration = 1;
    regs.Register_6.Bit.vco_calibration = 0;
    cdce62005_spi_write(spi, 6, regs.RegisterArray[6]);
    pfRadio420_usleep(1000);
    regs.Register_6.Bit.vco_calibration = 1;
    cdce62005_spi_write(spi, 6, regs.RegisterArray[6]);
    //add fixed delay of ~1ms to let the PLL stabilize.
    pfRadio420_usleep(1000);
    
    //enable outputs
    cdce62005_sync(spi);

    return OK;
}

Result cdce62005_lock_mode(spi_core_t * spi, pll_lock_mode mode)
{
    Result ret;
    cdce62005_Register_Map regs;

    ret = cdce62005_spi_read(spi, 5, &regs.RegisterArray[5]);
    if(FAILURE(ret)) return ret;

    regs.Register_5.Bit.digital_lock_en = mode;

    return cdce62005_spi_write(spi, 5, regs.RegisterArray[5]);
}

/*Result cdce62005_lock_state(spi_core_t * spi, pll_lock_status * state)
{
    Result ret;
    cdce62005_Register_Map regs;

    ret = cdce62005_spi_read(spi, 5, &regs.RegisterArray[5]);
    if(FAILURE(ret)) return ret;

    *state = regs.Register_5.Bit.lock_detected;

    return OK;
}*/

Result cdce62005_sync(spi_core_t * spi)
{
    Result ret;
    cdce62005_Register_Map regs;

    ret = cdce62005_spi_read(spi, 8, &regs.RegisterArray[8]);
    if(FAILURE(ret)) return ret;

    regs.Register_8.Bit.sync = 0;
    ret = cdce62005_spi_write(spi, 8, regs.RegisterArray[8]);
    if(FAILURE(ret)) return ret;
    pfRadio420_usleep(1000);
    regs.Register_8.Bit.sync = 1;
    return cdce62005_spi_write(spi, 8, regs.RegisterArray[8]);
}

Result cdce62005_flush_register(spi_core_t * spi)
{
    Result ret;
    int i;

    for (i=0; i<=8; i++)
    {
        ret = cdce62005_spi_write(spi, i, cdce62005_shadow[i]);
        if(FAILURE(ret)) return ret;
    }
    return ret;
}

#endif
