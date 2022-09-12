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


#ifndef INCLUDED_NUTAQ_PLAYBACK_H
#define INCLUDED_NUTAQ_PLAYBACK_H

#include <nutaq/api.h>
#include <gnuradio/block.h>
#include <nutaq/nutaq_cb.h>
#include <nutaq/carrier_client.h>
#include <string>

namespace gr {
    namespace nutaq {

        typedef enum E_RP_TYPE {
            RP_ETHERNET,
            RP_PCIE,
        } rp_type;

        /*!
         * \brief Read and write data to DDR3 RAM
         * \ingroup nutaq
         *
         */
        class NUTAQ_API playback : virtual public gr::block, virtual public nutaq_cb, virtual public carrier_client
        {
        public:
            typedef boost::shared_ptr<playback> sptr;

            static sptr make(std::string target_id, int block_priority);

            virtual void set_filename(std::string filename)=0;
            virtual void set_packet_size(int size)=0;
            virtual void set_channel(int channel)=0;
            virtual void set_type(rp_type type)=0;
            virtual void set_continuous_mode(bool cont)=0;
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_PLAYBACK_H */

