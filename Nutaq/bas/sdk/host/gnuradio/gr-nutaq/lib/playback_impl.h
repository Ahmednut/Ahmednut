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

#ifndef INCLUDED_NUTAQ_PLAYBACK_IMPL_H
#define INCLUDED_NUTAQ_PLAYBACK_IMPL_H

#include <nutaq/playback.h>
#include "nutaq/carrier.h"
#include "nutaq/nutaq_cb.h"
#include "delay_counter.h"

#include <recplay_types.h>
#include <recplay.h>
#include <rtdex_types.h>
#include <rtdex_eth_types.h>
#include <rtdex_media.h>
#include <rtdex_access.h>
#include <rtdex.h>
#include <rtdex_eth.h>
#include <rtdex_pci.h>

namespace gr {
    namespace nutaq {

        class playback_impl : virtual public playback
        {
        private:
            rp_type d_type;
            eRTDExMode d_mode;
            int d_packet_size;
            int d_channel;
            std::string d_filename;
            adp_handle_rtdex_conn d_rtdex;

        public:
            playback_impl(std::string target_id, int block_priority);
            ~playback_impl();

            void set_filename(std::string filename);
            void set_packet_size(int size);
            void set_channel(int channel);
            void set_type(rp_type type);
            void set_continuous_mode(bool cont);

            int callback();

            Result init_rtdex_pcie(carrier * lcarrier);
            Result init_rtdex_ethernet(carrier * lcarrier);
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_PLAYBACK_IMPL_H */

