/* -*- c++ -*- */
/* 
 * Copyright 2014 Nutaq Innovation inc.
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

#ifndef INCLUDED_NUTAQ_GENERIC_REGISTER_IMPL_H
#define INCLUDED_NUTAQ_GENERIC_REGISTER_IMPL_H

#include <nutaq/generic_register.h>
#include "nutaq/carrier.h"
#include "nutaq/nutaq_cb.h"
#include "delay_counter.h"

namespace gr {
    namespace nutaq {

        class generic_register_impl : virtual public generic_register
        {
        private:
            carrier * d_carrier;

            unsigned int d_address;
            int d_default_value;
            float d_rate;
            float d_wait_time;
            delay_counter d_delay;

        public:
            generic_register_impl(std::string target_id, int block_priority);
            ~generic_register_impl();

            virtual void set_address(unsigned int address);
            virtual void set_default_value(int data);            
            virtual void set_update_rate(float rate);

            virtual int set_value(int data);
            virtual int set_value(float data);
            virtual int get_value();  

            int callback();
            
            bool start();
            bool stop();
            
            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_GENERIC_REGISTER_IMPL_H */

