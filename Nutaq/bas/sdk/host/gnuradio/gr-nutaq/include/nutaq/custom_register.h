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


#ifndef INCLUDED_NUTAQ_CUSTOM_REGISTER_H
#define INCLUDED_NUTAQ_CUSTOM_REGISTER_H

#include <nutaq/api.h>
#include <gnuradio/sync_block.h>
#include <nutaq/nutaq_cb.h>
#include <nutaq/carrier_client.h>
#include <string>

namespace gr {
    namespace nutaq {

        /*!
         * \brief Read and write data on the data bus
         * \ingroup nutaq
         *
         */
        class NUTAQ_API custom_register : virtual public gr::sync_block, virtual public nutaq_cb, virtual public carrier_client
        {
        public:
            typedef boost::shared_ptr<custom_register> sptr;

            static sptr make(std::string target_id, int block_priority);

            virtual void set_index(int index)=0;
            virtual void set_default_value(int data)=0;            
            virtual void set_update_rate(float rate)=0;

            virtual int set_value(int data)=0;
            virtual int set_value(float data)=0;
            virtual int get_value()=0;            
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_CUSTOM_REGISTER_H */

