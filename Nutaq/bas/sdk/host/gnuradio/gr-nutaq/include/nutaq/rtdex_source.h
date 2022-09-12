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


#ifndef INCLUDED_NUTAQ_RTDEX_SOURCE_H
#define INCLUDED_NUTAQ_RTDEX_SOURCE_H

#include <nutaq/api.h>
#include <nutaq/carrier_client.h>
#include <nutaq/rtdex.h>

#include <string>

namespace gr {
    namespace nutaq {

        /*!
         * \brief Receive data from a carrier board
         * \ingroup nutaq
         *
         */
        class NUTAQ_API rtdex_source : virtual public rtdex
        {
        public:
            typedef boost::shared_ptr<rtdex_source> sptr;

            static sptr make(std::string target_id, size_t itemsize, int num_chan, int block_priority);

            virtual int work(int noutput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items)=0;
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_RTDEX_SOURCE_H */

