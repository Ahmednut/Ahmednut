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


#ifndef INCLUDED_NUTAQ_RADIO420_TX_H
#define INCLUDED_NUTAQ_RADIO420_TX_H

#include <nutaq/api.h>
#include <nutaq/carrier_client.h>

#include <gnuradio/sync_block.h>

namespace gr {
    namespace nutaq {

        /*!
         * \brief Configure a single Radio420X TX channel.
         * \ingroup nutaq
         *
         */
        class NUTAQ_API radio420_tx : virtual public gr::sync_block, virtual public carrier_client
        {
        public:
            typedef boost::shared_ptr<radio420_tx> sptr;

            static sptr make(std::string target_id, unsigned selected_card, int block_priority);

            virtual void set_default_selected_card(unsigned selected)=0;
            
            virtual void set_default_enable(bool enable)=0;

            virtual void set_default_datarate(double datarate)=0;
            virtual void set_default_tx_freq(double freq)=0;
            virtual void set_default_reference(int reference)=0;

            virtual void set_default_calibrate(bool calibrate)=0;
            virtual void set_default_band(unsigned band)=0; // 0 is low band, 1 is high band

            virtual void set_default_update_rate(float rate)=0;
            virtual void set_default_tx_vga1_gain(int tx_vga1_gain)=0; //tx_vga1_gain can be between -35 and -4
            virtual void set_default_tx_vga2_gain(unsigned tx_vga2_gain)=0; //tx_vga2_gain can be between 0 and 25
            virtual void set_default_tx_gain3(int tx_gain3)=0; //tx_gain3 can be between -13 and 18
            virtual void set_default_tx_lpf_bandwidth(unsigned bandwidth)=0;
            //for SPI control
			virtual void set_default_ref_clk_ctrl(unsigned ref_clk_ctrl)=0;
			virtual void set_default_rf_ctrl(unsigned rf_ctrl)=0;
			virtual void set_default_tx_gain_ctrl(unsigned tx_gain_ctrl)=0;
			virtual void set_default_pll_cpld_ctrl(unsigned pll_cpld_ctrl)=0;
            
            virtual int set_tx_gain(int tx_gain)=0; //set tx_gain3 value
            virtual int set_tx_freq(double freq)=0;			

            virtual int reset()=0;
            virtual int write_spi(int device, int addr, int data)=0;
            virtual int read_spi(int device, int addr)=0;
            virtual int set_pll(double ref_freq, double adac_freq, double lime_freq)=0;
            virtual int read_pll_lockstatus()=0;
            virtual int wait_pll_lockstatus()=0;
            virtual int set_clockmux(int dest, int src)=0;
            virtual int calibrate_pll()=0;
            virtual int reset_lime()=0;
            virtual int set_band(int band)=0;            
            virtual int set_lpf(int bwt)=0;
            virtual int calibrate_lpf(double ref_freq)=0;
            virtual int enable_path()=0;
            virtual int disable_path()=0;
            virtual int set_loopback(int mode)=0;
            virtual int set_calibration_lo_leakage(double carrier_freq, double acq_freq)=0;
            virtual int set_calibration_ssb(double carrier_freq, double acq_freq)=0;
            virtual int set_spi_control(int control, int spi_controller)=0;
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_RADIO420_TX_H */

