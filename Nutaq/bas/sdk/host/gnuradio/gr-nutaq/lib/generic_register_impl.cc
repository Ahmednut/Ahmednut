/* -*- c++ -*- */
/*
 * Copyright 2014 Nutaq Innovation inc.
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
#include "generic_register_impl.h"
#include "nutaq_api.h"

#include <stdlib.h>
#include <unistd.h>

#include <eapi.h>
#include <bas_error.h>

namespace gr {
    namespace nutaq {

        generic_register::sptr
        generic_register::make(std::string target_id, int block_priority)
        {
            return gnuradio::get_initial_sptr
                (new generic_register_impl(target_id, block_priority));
        }

        /*
         * The private constructor
         */
       generic_register_impl::generic_register_impl(std::string target_id, int block_priority)
            : gr::sync_block("custom_register",
                        gr::io_signature::make(0, 1, sizeof(int)),
                        gr::io_signature::make(0, 1, sizeof(int))),
			  nutaq_cb(block_priority),
              carrier_client(target_id)
        {
			d_default_value = 0;
            d_rate = 1;
            d_wait_time = 1;
            delay_counter_reset(&d_delay);
            delay_counter_begin(&d_delay);
            set_alignment(1);
            set_max_noutput_items(1);
            set_max_output_buffer(2);
        }

        /*
         * Our virtual destructor.
         */
        generic_register_impl::~generic_register_impl()
        {
        }

        void generic_register_impl::set_address(unsigned int address)
        {
            d_address = address;
        }

        void generic_register_impl::set_default_value(int data)
        {
            d_default_value = data;
        }

        void generic_register_impl::set_update_rate(float rate)
        {
            d_rate = rate;
            d_wait_time = 1 / rate;
        }

        int generic_register_impl::set_value(int data)
        {
            nutaq_api * api = nutaq_api::get_instance();
            carrier * carrier;
            connection_state * pState;
            Result res;

            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();

            carrier->lock();
            res = memory_write_send(pState, d_address, data);
            carrier->unlock();
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
            }
            else
            {
                std::cout << "Writing " << data << " at address 0x" << std::hex << std::uppercase << d_address << std::nouppercase << std::dec << std::endl;
            }
            return res;
        }

        int generic_register_impl::set_value(float data)
        {
            return set_value((int)data);
        }

        int generic_register_impl::get_value()
        {
            nutaq_api * api = nutaq_api::get_instance();
            connection_state * pState;
            carrier * carrier;
            Result res;
            unsigned data;

            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();

            carrier->lock();

            res = memory_read_send(pState, d_address, &data);

            carrier->unlock();

            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                return res;
            }

            return (int)data;
        }

        int generic_register_impl::callback()
        {
            nutaq_api * api = nutaq_api::get_instance();
            carrier * carrier;

            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            set_value(d_default_value);

            return 0;
        }

        bool generic_register_impl::start()
        {
            nutaq_api * api = nutaq_api::get_instance();

            api->try_init();

            std::cout << "generic_register: started" << std::endl;
        }

        bool generic_register_impl::stop()
        {
            std::cout << "generic_register: stopped" << std::endl;
        }

        int generic_register_impl::work(int noutput_items,
                 gr_vector_const_void_star &input_items,
                 gr_vector_void_star &output_items)
        {
            nutaq_api * api = nutaq_api::get_instance();
            connection_state * pState;
            carrier * carrier;
            int i;
            double delay;
            Result res;

            //std::cout << "generic_register: work " << noutput_items << " " << input_items.size() << " " << output_items.size() << std::endl;

            res = api->wait_for_init();
            if (res < 0)
            {
                std::cout << name() << ": Initialization failed, this work function will stop" << std::endl;
                exit(-1);
            }

            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                exit(-1);
            }

            pState = carrier->get_connectionstate();

            for(i=0; i<noutput_items; i++)
            {
                delay_counter_end(&d_delay);
                delay_counter_get(&d_delay, &delay);
                while (delay < d_wait_time)
                {
                    usleep((d_wait_time - delay) * 1000000);
                    delay_counter_end(&d_delay);
                    delay_counter_get(&d_delay, &delay);
                }

                delay_counter_reset(&d_delay);
                delay_counter_begin(&d_delay);

                if(input_items.size())
                {
                    set_value(((int*)input_items[0])[i]);
                }

                delay_counter_reset(&d_delay);
                delay_counter_begin(&d_delay);

                if(output_items.size())
                {
                    ((int*)output_items[0])[i] = get_value();
                    if(FAILURE(res))
                    {
                        std::cout << ErrorToString(res) << std::endl;
                        throw std::runtime_error("End of generic_register work function");
                    }
                }
            }

            // Tell runtime system how many output items we produced.
            return noutput_items;
        }

    } /* namespace nutaq */
} /* namespace gr */

