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

#ifndef INCLUDED_NUTAQ_RTDEX_IMPL_H
#define INCLUDED_NUTAQ_RTDEX_IMPL_H

#include <nutaq/carrier.h>
#include <nutaq/carrier_client.h>
#include <nutaq/nutaq_cb.h>
#include <nutaq/api.h>

#include <gnuradio/sync_block.h>

#include <string>
#include <vector>

namespace gr {
    namespace nutaq {

        typedef enum E_RTDEX_TYPE {
            RTDEX_ETHERNET,
            RTDEX_PCIE,			
			RTDEX_AXI,
        } rtdex_type;

        class NUTAQ_API rtdex : virtual public gr::sync_block, virtual public nutaq_cb, virtual public carrier_client
        {
        protected:
            size_t d_itemsize;
            int d_num_chan;
            int d_packet_size;
            std::vector<std::vector<int> > d_channels;

            std::vector<void*> d_rtdex;
            std::vector<carrier*> d_rtdex_carriers;

            std::vector<carrier*> d_carriers;
            rtdex_type d_type;

            std::vector<int> d_sd;

        public:
            rtdex(int item_size, int num_chan);
            rtdex();
            
            virtual int get_direction()=0;
            void set_packet_size(int packet_size);
            void set_channels(std::string channels);
            void set_type(rtdex_type type);

            int init();
            int callback();

            virtual bool start();
            virtual bool stop();

        protected:
            Result init_rtdex_tcpip();
			Result init_rtdex_axi();
            Result init_rtdex_ethernet();
            Result init_rtdex_pcie();

        };
    } // namespace nutaq
} // namespace gr

#endif
