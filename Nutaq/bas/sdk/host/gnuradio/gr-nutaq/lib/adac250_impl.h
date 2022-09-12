/* -*- c++ -*- */
/* 
 * Copyright 2013 Nutaq Innovation inc.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_NUTAQ_ADAC250_IMPL_H
#define INCLUDED_NUTAQ_ADAC250_IMPL_H

#include <nutaq/adac250.h>
#include "nutaq/carrier.h"
#include "nutaq/nutaq_cb.h"

#include "eapi_adac250.h"
#include "adac250_defines.h"
#include "adac250_types.h"

#define ADAC250_CHANNEL_NB 2

namespace gr {
    namespace nutaq {

        class adac250_impl : virtual public adac250
        {
        private:
            ADAC250_CLOCK_SOURCES d_ref_clk_source;
            unsigned int d_ref_clk_freq;
            unsigned int d_adc_clk_freq;
            unsigned int d_dac_clk_freq;
            ADAC250_DAC_INTERPOLATION d_dac_interp;
            ADAC250_ADC_GAIN d_adc_gain[ADAC250_CHANNEL_NB];
            int d_adc_fine_gain[ADAC250_CHANNEL_NB];
            int d_dac_gain[ADAC250_CHANNEL_NB];
            int d_ref_clk_tuning;
            
            void calculate_adc_gains(double gain, ADAC250_ADC_GAIN * coarse_gain, int * fine_gain);
            void calculate_dac_gain(double gain, int * dac_gain);
            
        public:
            adac250_impl(std::string target_id, int block_priority);
            ~adac250_impl();

            virtual void set_default_ref_clk_source(int source);
            virtual void set_default_ref_clk_freq(double freq);
            virtual void set_default_adc_clk_freq(double freq);
            virtual void set_default_dac_clk_freq(double freq);
            virtual void set_default_dac_interpolation(int interp);
            virtual void set_default_adc_gain(int channel, double gain);
            virtual void set_default_dac_gain(int channel, double gain);
            virtual void set_default_ref_clk_tuning(int value);
            
            virtual int read_spi_register(int device, int addr);
            virtual int write_spi_register(int device, int addr, int value);
            virtual int set_dac_gain(int channel, double gain);
            virtual int init_dac(int sleep, int interp, int mixmode1, int mixmode2);
            virtual int set_dac_sync(int sync);
            virtual int set_dac_sync_source(int sync_source);
            virtual int calibrate_dac();
            virtual int init_pll(int refclk, int adcclk, int dacclk, int interp, int clksrc);
            virtual int get_pll_status();
            virtual int tune_ref_clk(int value);
            virtual int set_pll_config(int refclk, int adcclk, int dacclk, int interp);
            virtual int init_adc();
            virtual int set_adc_gain(int channel, double gain);
            virtual int set_adc_pedestal(int channel, int pedestal);
            virtual int get_adc_status(int channel);
            virtual int calibrate_adc();
            virtual int set_adc_data_format(int format);
            virtual int set_adc_sleep_mode(int mode);
            virtual int set_adc_trigger_delay(int triggerdelay);
            virtual int set_clk_mux(int clock_to_config, int clock_input_selection);
            virtual int set_adc_offset_correction_time(int time_a, int time_b);

            int callback();
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_ADAC250_IMPL_H */

