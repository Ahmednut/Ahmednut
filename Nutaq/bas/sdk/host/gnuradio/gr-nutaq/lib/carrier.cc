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

#include <nutaq/carrier.h>
#include "nutaq_api.h"

#include "carrier_zedboard_impl.h"
#include "carrier_perseus_impl.h"
#include <bas_error.h>

#include <stdlib.h>

namespace gr {
    namespace nutaq {
        
        carrier::carrier(std::string id, std::string ip_addr)
        {
            nutaq_api * api = nutaq_api::get_instance();
            int res;

            memset(d_peripheral_reseted, 0, sizeof(d_peripheral_reseted));
            memset(d_peripheral_rx_enable, 0, sizeof(d_peripheral_rx_enable));

            pthread_mutex_init(&mutex, 0);

            res = connect_cce((const char*)ip_addr.c_str(), &d_state);

            if(!res)
            {
                // success
                std::cout << id << " connected to " << ip_addr << std::endl;
                d_id = id;
                d_ip_addr = ip_addr;
                api->register_carrier(id, this);
            }else{
                std::cout << "Failed to connect " << id << " using " << ip_addr << std::endl;
				std::cout << ErrorToString(res) << std::endl;
            }

        }

        carrier::~carrier()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            disconnect_cce(&d_state);
            api->unregister_carrier(d_id);
        }

        carrier::sptr
        carrier::make(carrier_class lclass, std::string id, std::string ip_addr)
        {
            switch(lclass)
            {
            case CARRIER_PERSEUS:
                return gnuradio::get_initial_sptr
                    (new carrier_perseus_impl(id, ip_addr));

            case CARRIER_ZEDBOARD:
                return gnuradio::get_initial_sptr
                    (new carrier_zedboard_impl(id, ip_addr));
            }
        }

        

        std::string carrier::get_ip_addr()
        {
            return d_ip_addr;
        }

        connection_state * carrier::get_connectionstate()
        {
            return (connection_state *)(&d_state);
        }


        bool carrier::is_peripheral_reseted(unsigned num)
        {
            if(num >= MAX_NUMBER_OF_PERIPHERAL)
                return true;
            
            return d_peripheral_reseted[num];
        }
        
        void carrier::set_peripheral_reset(unsigned num, bool reseted)
        {
            if(num >= MAX_NUMBER_OF_PERIPHERAL)
                return;
            d_peripheral_reseted[num] = reseted;
        }

	    bool carrier::is_peripheral_rx_enable(unsigned num)
        {
            if(num >= MAX_NUMBER_OF_PERIPHERAL)
                return true;
            
            return d_peripheral_rx_enable[num];
        }
        
        void carrier::set_peripheral_rx_enable(unsigned num, bool enable)
        {
            if(num >= MAX_NUMBER_OF_PERIPHERAL)
                return;
            d_peripheral_rx_enable[num] = enable;
        }

    } // namespace nutaq
} // namespace gr


