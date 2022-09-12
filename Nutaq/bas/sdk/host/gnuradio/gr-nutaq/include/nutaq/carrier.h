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


#ifndef INCLUDED_NUTAQ_CARRIER_H
#define INCLUDED_NUTAQ_CARRIER_H

#include <nutaq/api.h>
#include <gnuradio/block.h>

#include <bas_types.h>
#include <eapi.h>
#include <connection_state.h>

#include <string>
#include <iostream>

#include <pthread.h>

#define MAX_NUMBER_OF_PERIPHERAL 32

namespace gr {
    namespace nutaq {

        typedef enum E_carrier_class {
            CARRIER_PERSEUS,
            CARRIER_ZEDBOARD,
        } carrier_class;

        /*!
         * \brief This block represent a generic carrier board
         * \ingroup nutaq
         *
         */
        class NUTAQ_API carrier : virtual public gr::block
        {
        public:
            typedef boost::shared_ptr<carrier> sptr;

            carrier(std::string id, std::string ip_addr);
            ~carrier();

            static sptr make(carrier_class lclass, std::string id, std::string ip_addr);

            std::string get_ip_addr();

            connection_state * get_connectionstate();

            inline void lock()
            {
                //std::cout << "carrier lock " << (void*)this << std::endl;
                pthread_mutex_lock(&mutex);
            }

            inline void unlock()
            {
                //std::cout << "carrier unlock " << (void*)this << std::endl;
                pthread_mutex_unlock(&mutex);
            }

            virtual carrier_class get_class()=0;

            bool is_peripheral_reseted(unsigned num);
            void set_peripheral_reset(unsigned num, bool reseted);

            bool is_peripheral_rx_enable(unsigned num);
            void set_peripheral_rx_enable(unsigned num, bool enable);
            
        protected:
            pthread_mutex_t mutex;
            connection_state d_state;

            std::string d_id;
            std::string d_ip_addr;

        private:    
            bool d_peripheral_reseted[MAX_NUMBER_OF_PERIPHERAL];
            bool d_peripheral_rx_enable[MAX_NUMBER_OF_PERIPHERAL];
	
        };

    } // namespace nutaq
} // namespace gr

#endif /* INCLUDED_NUTAQ_CARRIER_H */

