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

#ifndef INCLUDED_NUTAQ_RADIO420_RX_IMPL_H
#define INCLUDED_NUTAQ_RADIO420_RX_IMPL_H

#include <nutaq/radio420_rx.h>
#include "nutaq/carrier.h"
#include "delay_counter.h"
#include <nutaq/nutaq_cb.h>

#include <bas_error.h>
#include <bas_types.h>
#include <connection_state.h>

#include <string>

namespace gr {
    namespace nutaq {

        class radio420_rx_impl : public radio420_rx, public nutaq_cb
        {
        private:
            //Radio420X
            bool d_enable;
            bool d_calibrate;
            unsigned d_selected_card;
            FMCRADIO_REV d_rev;
            unsigned d_datarate;
            unsigned d_reffreq;
            unsigned d_rx_freq;
            unsigned d_reference;
            unsigned d_band;
            float d_rate;
            float d_wait_time;
            delay_counter d_delay;
            unsigned d_rx_lna_gain;
            unsigned d_rx_vga1_gain;
            unsigned d_rx_gain2;
            int d_rx_gain3;
            unsigned d_rx_rf_filter;
            unsigned d_rx_lpf_bandwidth;
			unsigned d_ref_clk_ctrl;
			unsigned d_rf_ctrl;
			unsigned d_rx_gain_ctrl;
			unsigned d_pll_cpld_ctrl;

        public:
            radio420_rx_impl(std::string target_id, unsigned selected_card, int block_priority);
            ~radio420_rx_impl();

            int callback();
            int init();

            virtual void set_default_selected_card(unsigned selected);
            
            virtual void set_default_enable(bool enable);

            virtual void set_default_datarate(double datarate);
            virtual void set_default_rx_freq(double freq);
            virtual void set_default_reference(int reference);

            virtual void set_default_calibrate(bool calibrate);
            virtual void set_default_band(unsigned band); // 0 is low band, 1 is high band

            virtual void set_default_update_rate(float rate);
            virtual void set_default_rx_lna_gain(unsigned lna_gain); //lna_gain can be between 1 and 3
            virtual void set_default_rx_vga1_gain(unsigned vga1_gain); //vga1_gain can be between 1 and 3
            virtual void set_default_rx_gain2(unsigned rx_gain2); //rx_gain2 can be between 0 and 60
            virtual void set_default_rx_gain3(int rx_gain3); //rx_gain3 can be between -13 and 18
            virtual void set_default_rx_rf_filter(unsigned filter);
            virtual void set_default_rx_lpf_bandwidth(unsigned bandwidth);

            //for SPI control
			virtual void set_default_ref_clk_ctrl(unsigned ref_clk_ctrl);
			virtual void set_default_rf_ctrl(unsigned rf_ctrl);
			virtual void set_default_rx_gain_ctrl(unsigned rx_gain_ctrl);
			virtual void set_default_pll_cpld_ctrl(unsigned pll_cpld_ctrl);
            
            virtual int set_rx_gain(int rx_gain); //set rx_gain3 value
            virtual int set_rx_freq(double carrier_freq);			

            virtual int reset();
            virtual int write_spi(int device, int addr, int data);
            virtual int read_spi(int device, int addr);
            virtual int set_pll(double ref_freq, double adac_freq, double lime_freq);
            virtual int read_pll_lockstatus();
            virtual int wait_pll_lockstatus();
            virtual int set_clockmux(int dest, int src);            
            virtual int calibrate_pll();
            virtual int reset_lime();
            virtual int set_band(int band);
            virtual int calibrate_vga();
            virtual int set_filter(int filter);
            virtual int set_lpf(int bwt);
            virtual int calibrate_lpf(double ref_freq);
            virtual int enable_path();
            virtual int disable_path();
            virtual int set_loopback(int mode);            
            virtual int set_calibration_dc_offset();
            virtual int set_spi_control(int control, int spi_controller);

            bool start();
            bool stop();

            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_RADIO420_RX_IMPL_H */

