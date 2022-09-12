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

#ifndef INCLUDED_NUTAQ_RTDEX_SOURCE_IMPL_H
#define INCLUDED_NUTAQ_RTDEX_SOURCE_IMPL_H

#include <nutaq/rtdex_source.h>
#include "nutaq/carrier.h"

#include <bas_error.h>

#include <string>

namespace gr {
    namespace nutaq {

        class rtdex_source_impl : virtual public rtdex_source
        {
        public:
            rtdex_source_impl(std::string target_id, size_t itemsize, int num_chan, int block_priority);
            ~rtdex_source_impl();

            // Cannot acces board because the other Nutaq blocks can already be stopped.
            // bool stop();

            int work(int noutput_items,
                     gr_vector_const_void_star &input_items,
                     gr_vector_void_star &output_items);
            
        protected:
            int get_direction();
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_RTDEX_SOURCE_IMPL_H */

