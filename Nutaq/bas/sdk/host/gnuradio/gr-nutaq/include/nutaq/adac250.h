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


#ifndef INCLUDED_NUTAQ_ADAC250_H
#define INCLUDED_NUTAQ_ADAC250_H

#include <nutaq/api.h>
#include <gnuradio/block.h>
#include <nutaq/nutaq_cb.h>
#include <nutaq/carrier_client.h>
#include <string>

namespace gr {
    namespace nutaq {

        /*!
         * \brief Configure the ADAC250 FMC 
         * \ingroup nutaq
         *
         */
        class NUTAQ_API adac250 : virtual public gr::block, virtual public nutaq_cb, virtual public carrier_client
        {
        public:
            typedef boost::shared_ptr<adac250> sptr;

            static sptr make(std::string target_id, int block_priority);

            virtual void set_default_ref_clk_source(int source)=0;
            virtual void set_default_ref_clk_freq(double freq)=0;
            virtual void set_default_adc_clk_freq(double freq)=0;
            virtual void set_default_dac_clk_freq(double freq)=0;
            virtual void set_default_dac_interpolation(int interp)=0;
            virtual void set_default_adc_gain(int channel, double gain)=0;
            virtual void set_default_dac_gain(int channel, double gain)=0;
            virtual void set_default_ref_clk_tuning(int value)=0;
            
            virtual int read_spi_register(int device, int addr)=0;
            virtual int write_spi_register(int device, int addr, int value)=0;
            virtual int set_dac_gain(int channel, double gain)=0;
            virtual int init_dac(int sleep, int interp, int mixmode1, int mixmode2)=0;
            virtual int set_dac_sync(int sync)=0;
            virtual int set_dac_sync_source(int sync_source)=0;
            virtual int calibrate_dac()=0;
            virtual int init_pll(int refclk, int adcclk, int dacclk, int interp, int clksrc)=0;
            virtual int get_pll_status()=0;
            virtual int tune_ref_clk(int value)=0;
            virtual int set_pll_config(int refclk, int adcclk, int dacclk, int interp)=0;
            virtual int init_adc()=0;
            virtual int set_adc_gain(int channel, double gain)=0;
            virtual int set_adc_pedestal(int channel, int pedestal)=0;
            virtual int get_adc_status(int channel)=0;
            virtual int calibrate_adc()=0;
            virtual int set_adc_data_format(int format)=0;
            virtual int set_adc_sleep_mode(int mode)=0;
            virtual int set_adc_trigger_delay(int triggerdelay)=0;
            virtual int set_clk_mux(int clock_to_config, int clock_input_selection)=0;
            virtual int set_adc_offset_correction_time(int time_a, int time_b)=0;
            
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_ADAC250_H */

