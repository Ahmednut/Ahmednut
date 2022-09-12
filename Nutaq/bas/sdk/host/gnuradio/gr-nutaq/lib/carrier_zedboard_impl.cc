/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "carrier_zedboard_impl.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <iostream>

namespace gr {
    namespace nutaq {

        /*
         * The private constructor
         */
        carrier_zedboard_impl::carrier_zedboard_impl(std::string id, std::string ip_addr)
            : gr::block("carrier_zedboard",
                        gr::io_signature::make(0, 0, sizeof(int)),
                        gr::io_signature::make(0, 0, sizeof(int))),
              carrier(id, ip_addr)
        {
			d_is_open = false;
			d_is_started = false;
        }

        /*
         * Our virtual destructor.
         */
        carrier_zedboard_impl::~carrier_zedboard_impl()
        {
        }

        carrier_class carrier_zedboard_impl::get_class()
        {
            return CARRIER_ZEDBOARD;
        }        
		
		bool carrier_zedboard_impl::is_open()
		{
			return d_is_open;
		}
		
		void carrier_zedboard_impl::set_open(bool is_open)
		{
			d_is_open = is_open;
			return;
		}
		
		bool carrier_zedboard_impl::is_started()
		{
			return d_is_started;
		}
		
		void carrier_zedboard_impl::set_started(bool is_started)
		{
			d_is_started = is_started;
			return;
		}
		
		adp_handle_rtdex_conn carrier_zedboard_impl::get_rtdex_handle()
		{
			return d_rtdex_handle;
		}
		
		void carrier_zedboard_impl::set_rtdex_handle(adp_handle_rtdex_conn rtdex_handle)
		{
			d_rtdex_handle = rtdex_handle;
			return;
		}
    } /* namespace nutaq */
} /* namespace gr */

