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
#include "rtdex_sink_impl.h"
#include "nutaq_api.h"
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>

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

        rtdex_sink::sptr
        rtdex_sink::make(std::string target_id, size_t itemsize, int num_chan, int block_priority)
        {
            return gnuradio::get_initial_sptr
                (new rtdex_sink_impl(target_id, itemsize, num_chan, block_priority));
        }

        /*
         * The private constructor
         */
        rtdex_sink_impl::rtdex_sink_impl(std::string target_id, size_t itemsize, int num_chan, int block_priority)
            : gr::sync_block("rtdex_sink",
                             gr::io_signature::make(1, 8, itemsize),
                             gr::io_signature::make(0, 0, 0)),
              rtdex(itemsize, num_chan),
              nutaq_cb(block_priority),
              carrier_client(target_id)
        {
            d_packet_size = 1024;
        }

        /*
         * Our virtual destructor.
         */
        rtdex_sink_impl::~rtdex_sink_impl()
        {}

        int rtdex_sink_impl::get_direction()
        {
            return eToFpga;
        }

        int
        rtdex_sink_impl::work(int noutput_items,
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
            //std::cout << name() << ": res " << res << std::endl;
            if (res < 0)
            {
                std::cout << name() << ": Initialization failed, this work function will stop" << std::endl;
                exit(GNURADIO_TXFAILED);
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
                            rv = RTDExSend((adp_handle_rtdex_conn)*it, ((unsigned char*)input_items[j]) + (i * d_packet_size) + (d_packet_size - remaining), remaining);

                            if (rv < 0)
                            {
                                std::cout << ErrorToString(RTDExGetLastError((adp_handle_rtdex_conn)*it)) << std::endl;
                                exit(GNURADIO_TXFAILED);
                            }
                            remaining -= rv;
                        } while(remaining);
                        j++;
					}
				}
                               break;
			case CARRIER_ZEDBOARD:
			    j=0;
			    //std::cout << name() << ": Zedboard" << std::endl;
				for(std::vector<void*>::iterator it = d_rtdex.begin(); it != d_rtdex.end(); ++it)
                {
                    remaining = size;
                    //std::cout << name() << ": Remaining: " << remaining << std::endl;                  
                    do
                    {
                        rv = RTDExSend((adp_handle_rtdex_conn)*it, &((unsigned char*)input_items[j])[size-remaining], remaining);
						//std::cout << name() << ": RTDExSend Return: " << rv << std::endl;
                        if(rv < 0)
                        {
							std::cout << name() << ": failed to send data" << rv << std::endl;
                            exit(GNURADIO_TXFAILED);
                        }
                        remaining -= rv;
                    } while(remaining);
                    j++;
                }
                break;
			}            
            // Tell runtime system how many output items we produced.
            //std::cout << name() << ": Done: " << remaining << std::endl;
            return noutput_items;
        }
    } /* namespace nutaq */
} /* namespace gr */

