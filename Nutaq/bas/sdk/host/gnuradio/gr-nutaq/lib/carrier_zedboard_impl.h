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

#ifndef INCLUDED_NUTAQ_CARRIER_ZEDBOARD_IMPL_H
#define INCLUDED_NUTAQ_CARRIER_ZEDBOARD_IMPL_H

#include <nutaq/carrier.h>

#include "nutaq_api.h"

#include <rtdex_types.h>
#include <rtdex_eth_types.h>
#include <rtdex_media.h>
#include <rtdex_access.h>

#include <string>

#define RTDEX_TCP_PORT 19850
#define RTDEX_TCP_BUFFER_SIZE 65536

namespace gr {
    namespace nutaq {

        class carrier_zedboard_impl : public carrier
        {

        public:
            carrier_zedboard_impl(std::string id, std::string ip_addr);
            ~carrier_zedboard_impl();

            carrier_class get_class();
			
			bool is_open();
			void set_open(bool is_open);
			
			bool is_started();
			void set_started(bool is_started);
            
			adp_handle_rtdex_conn get_rtdex_handle();
			void set_rtdex_handle( adp_handle_rtdex_conn rtdex_handle);
			
		protected:
			bool d_is_open;
			bool d_is_started;
			adp_handle_rtdex_conn d_rtdex_handle;
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_CARRIER_ZEDBOARD_IMPL_H */

