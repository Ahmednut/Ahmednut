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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "nutaq_api.h"

#include <stdio.h>
#include <string.h>

#include <iostream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define likely(x)    __builtin_expect (!!(x), 1)
#define unlikely(x)  __builtin_expect (!!(x), 0)

namespace gr {
    namespace nutaq {

        //singleton
        nutaq_api * nutaq_api::_singleton = NULL;

        nutaq_api::nutaq_api()
        {
            pthread_mutex_init(&mutex, 0);
            pthread_mutex_init(&init_mutex, 0);
            pthread_cond_init(&init_cond, 0);
            
            d_inited = false;
            d_init_result = 0;
        }

        nutaq_api * nutaq_api::get_instance()
        {
            if (unlikely(_singleton == NULL))
            {
                _singleton = new nutaq_api;
            }
            return _singleton;
        }

        void nutaq_api::register_carrier(std::string targetid, carrier * c)
        {
            std::cout << "Registering carrier " << targetid << std::endl;
            lock();
            d_carriers.insert(std::pair<std::string, carrier*>(targetid, c));
            unlock();
        }

        void nutaq_api::unregister_carrier(std::string targetid)
        {
            std::cout << "Unregistering carrier " << targetid << std::endl;
            lock();
            d_carriers.erase(targetid);
            unlock();
        }

        carrier * nutaq_api::get_carrier_byid(std::string targetid)
        {
            carrier * c;
    
            c = d_carriers[targetid];
    
            return c;
        }

        std::vector<carrier*> nutaq_api::get_carrier_vector_byid(std::string targetid)
        {
            std::vector<carrier*> carrier_list_tmp;
            std::vector<std::string> carrier_names;
            carrier * carrier_tmp;
            int i;

            boost::split(carrier_names, targetid, boost::is_any_of(";, "));
            
            for(i=0; i<carrier_names.size(); i++)
            {
                carrier_tmp = get_carrier_byid(carrier_names[i]);
                if(carrier_tmp)
                    carrier_list_tmp.push_back(carrier_tmp);
            }

            return carrier_list_tmp;
        }

        unsigned nutaq_api::get_datarate(std::string targetid)
        {
            return d_datarates[targetid];
        }

        void nutaq_api::set_datarate(std::string targetid, unsigned datarate)
        {
            d_datarates[targetid] = datarate;
        }

        void nutaq_api::register_callback(nutaq_cb * cb)
        {
            lock();
            d_cb.push_back(cb);
            unlock();
        }

        int nutaq_api::do_callback()
        {
            int res = 0;
            
            std::cout << "Global init begin" << std::endl;
            //sort callback
            std::sort (d_cb.begin(), d_cb.end(), nutaq_cb::cb_compare);
            
            //execute callbacks
            for(std::vector<nutaq_cb*>::iterator it = d_cb.begin(); it!=d_cb.end(); ++it)
            {				
                res = (*it)->callback();
                if (res < 0)
                    break;
            }

            d_cb.clear();

            if (res < 0)
                std::cout << "Global init end with error" << std::endl;
            else
                std::cout << "Global init end successfully" << std::endl;
            set_inited(true);
            pthread_cond_broadcast(&init_cond);
            
            return res;            
        }

        bool nutaq_api::is_inited()
        {
            __sync_synchronize();
            return d_inited;
        }

        void nutaq_api::set_inited(bool init)
        {
            d_inited = init;
            __sync_synchronize();
        }
        
        int nutaq_api::try_init()
        {
            lock();
            if (!is_inited())
            {
                d_init_result = do_callback();
                unlock();
            }else{
                unlock();
            }
            
            return d_init_result;
        }

        int nutaq_api::wait_for_init()
        {
            lock();
            if(!is_inited())
            {
                unlock();
                pthread_mutex_lock(&init_mutex);
                pthread_cond_wait(&init_cond, &init_mutex);
            }else{
                unlock();
            }
            
            return d_init_result;
        }


    } // namespace nutaq
} // namespace gr
