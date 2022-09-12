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

#ifndef INCLUDED_NUTAQ_CUSTOM_REGISTER_IMPL_H
#define INCLUDED_NUTAQ_CUSTOM_REGISTER_IMPL_H

#include <nutaq/custom_register.h>
#include "nutaq/carrier.h"
#include "nutaq/nutaq_cb.h"
#include "delay_counter.h"

namespace gr {
    namespace nutaq {

        class custom_register_impl : virtual public custom_register
        {
        private:
            int d_index;
            int d_default_value;
            float d_rate;
            float d_wait_time;
            unsigned int d_first_time_done;
            delay_counter d_delay;

        public:
            custom_register_impl(std::string target_id, int block_priority);
            ~custom_register_impl();

            virtual void set_index(int index);
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

#endif /* INCLUDED_NUTAQ_CUSTOM_REGISTER_IMPL_H */

