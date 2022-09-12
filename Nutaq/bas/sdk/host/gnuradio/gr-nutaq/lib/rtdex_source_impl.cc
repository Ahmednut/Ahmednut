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

#include <gnuradio/io_signature.h>
#include "rtdex_source_impl.h"

#include "nutaq_api.h"
#include "carrier_zedboard_impl.h"

#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <string>

#include <rtdex_types.h>
#include <rtdex_eth_types.h>
#include <rtdex_media.h>
#include <rtdex_access.h>
#include <rtdex.h>
#include <rtdex_eth.h>
#include <rtdex_pci.h>
#include <rtdex_tcpip.h>
#include <rtdex_axi.h>
#include <bas_error.h>

namespace gr {
    namespace nutaq {

        rtdex_source::sptr
        rtdex_source::make(std::string target_id, size_t itemsize, int num_chan, int block_priority)
        {
            return gnuradio::get_initial_sptr
                (new rtdex_source_impl(target_id, itemsize, num_chan, block_priority));
        }

        /*
         * The private constructor
         */
        rtdex_source_impl::rtdex_source_impl(std::string target_id, size_t itemsize, int num_chan, int block_priority)
            : gr::sync_block("rtdex_source",
                             gr::io_signature::make(0, 0, 0),
                             gr::io_signature::make(1, 8, itemsize)),
              rtdex(itemsize, num_chan),
              nutaq_cb(block_priority),
              carrier_client(target_id)
        {
            d_packet_size = 1024;
        }

        /*
         * Our virtual destructor.
         */
        rtdex_source_impl::~rtdex_source_impl()
        {}

        int rtdex_source_impl::get_direction()
        {
            return eFromFpga;
        }

        // Cannot acces board because the other Nutaq blocks can already be stopped.
        /*
        bool rtdex_source_impl::stop()
        {
            Result res = 0;
            int i;
            unsigned nb_frame;
            carrier_class lclass = d_carriers[0]->get_class(); //Every carrier is checked to be of the same type
            switch(lclass)
            {
            case CARRIER_PERSEUS:
                for(i = 0; i<d_rtdex.size(); i++)
                {
                    RTDExGetRxFramesLostCountEth((adp_handle_rtdex_conn)d_rtdex[i], &nb_frame);
                    if (FAILURE(res))
                    {
                        std::cout << ErrorToString(res) << std::endl;
                    }
                
                    if(nb_frame)
                    {
                        std::cout << name() << ": channel " << i << " lost " << nb_frame << " frames.\n";
                    }
                }
                break;
            default:
                break;
            }

            return rtdex::stop();
        }
        */

        int
        rtdex_source_impl::work(int noutput_items,
                                gr_vector_const_void_star &input_items,
                                gr_vector_void_star &output_items)
        {
            Result res = 0;
            nutaq_api * api = nutaq_api::get_instance();
            carrier_class lclass = d_carriers[0]->get_class(); //Every carrier is checked to be of the same type
            int iteration, rv, remaining;
            unsigned size = ((unsigned)noutput_items) * d_itemsize;
            int j, i;
            adp_handle_rtdex_tcpip_conn pTcpIpConn;

            //std::cout << name() << ": work " << size << std::endl;
            
            res = api->wait_for_init();
            if (res < 0)
            {
                std::cout << name() << ": Initialization failed, this work function will stop" << std::endl;
                exit(GNURADIO_RXFAILED);
            }
            
            switch(lclass)
            {
            default:
            case CARRIER_PERSEUS:
                //std::cout << name() << ": Perseus" << std::endl;
                iteration = size / d_packet_size;
                
                if ( size % d_packet_size)
                {
                    std::cout << name() << ": data is unaligned" << std::endl;
                }

                for(i=0; i<iteration; i++)
                {
                    j = 0;
                    for(std::vector<void*>::iterator it = d_rtdex.begin(); it != d_rtdex.end(); ++it)
                    {
                        remaining = d_packet_size;
                        do
                        {
                            rv = RTDExReceive((adp_handle_rtdex_conn)*it, ((unsigned char*)output_items[j]) + (i * d_packet_size) + (d_packet_size - remaining), remaining, eRTDExWaitTimeout, 1000);
                            
                            if (rv < 0)
                            {
                                std::cout << ErrorToString(RTDExGetLastError((adp_handle_rtdex_conn)*it)) << std::endl;
                                exit(GNURADIO_RXFAILED);
                            }
                            remaining -= rv;
                        } while(remaining);
                        j++;
                    }
                }
                break;
            
            case CARRIER_ZEDBOARD:
                //std::cout << name() << ": Zedboard" << std::endl;
                j = 0;
                for(std::vector<void*>::iterator it = d_rtdex.begin(); it != d_rtdex.end(); ++it)
                {
                    remaining = size;
                    //std::cout << name() << ": Remaining: " << remaining << std::endl;
                    do
                    {
                        rv = RTDExReceive((adp_handle_rtdex_conn)*it, &((unsigned char*) output_items[j])[size-remaining], remaining, eRTDExWaitTimeout, 1000);
                        //std::cout << name() << ": Receive Return: " << rv << std::endl;
                        if(rv < 0)
                        {
                            std::cout << name() << ": failed to receive data" << std::endl;
                            exit(GNURADIO_RXFAILED);
                        }
                        remaining -= rv;
                    } while(remaining);
                    j++;
                    break;
                }
                break;
            }
            // Tell runtime system how many output items we produced.
            //std::cout << name() << ": Done: " << remaining << std::endl;
            return noutput_items;
        }
    } /* namespace nutaq */
} /* namespace gr */

