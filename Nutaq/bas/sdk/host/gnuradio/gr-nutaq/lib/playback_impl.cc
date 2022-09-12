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
#include "playback_impl.h"
#include "nutaq_api.h"

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/mman.h>

#include <mac_utils.h>
#include <bas_error.h>
#include <eapi_carrier.h>

namespace gr {
    namespace nutaq {

        playback::sptr
        playback::make(std::string target_id, int block_priority)
        {
            return gnuradio::get_initial_sptr
                (new playback_impl(target_id, block_priority));
        }

        /*
         * The private constructor
         */
        playback_impl::playback_impl(std::string target_id, int block_priority)
            : gr::block("playback",
                        gr::io_signature::make(0, 0, sizeof(int)),
                        gr::io_signature::make(0, 0, sizeof(int))),
              nutaq_cb(block_priority),
              carrier_client(target_id)
        {
            d_type = RP_ETHERNET;
            d_mode = eContinuous;
        }

        /*
         * Our virtual destructor.
         */
        playback_impl::~playback_impl()
        {
        }

        void playback_impl::set_filename(std::string filename)
        {
            d_filename = filename;
        }

        void playback_impl::set_packet_size(int size)
        {
            d_packet_size = size;
        }

        void playback_impl::set_channel(int channel)
        {
            d_channel = channel;
        }

        void playback_impl::set_type(rp_type type)
        {
            d_type = type;
        }

        void playback_impl::set_continuous_mode(bool cont)
        {
            if(cont)
            {
                d_mode = eContinuous;
            }else{
                d_mode = eSingle;
            }
        }

        int playback_impl::callback()
        {
            nutaq_api * api = nutaq_api::get_instance();
            carrier * lcarrier;
            connection_state * pState;
            unsigned int uiRecPlayBaseAddr;
            adp_handle_recplay lrecplay;
            int timeout;
            unsigned char done;
            unsigned char uTrigOffset;
            unsigned int uAddrTrig;
            char pHostMacAddr[6];
            char pDeviceMacAddr[6];
            int lfiledes;
            struct stat filestat;
            off_t filesize;
            void * buf;
            int lframesize;
            Result res;

            //std::cout << name() << ": callback\n";
            
            if (api->get_carrier_byid(get_target_id())->get_class() != CARRIER_PERSEUS)
            {
                std::cout << name() << ": Error. The carrier board must be a Perseus" << std::endl;
                return -1;
            }

            std::cout << std::hex;
            
            lfiledes = open(d_filename.c_str(), O_RDONLY);
           
            if(!lfiledes)
            {
                std::cout << name() << ": cannot open " << d_filename << std::endl;
                return -1;
            }

            fstat(lfiledes, &filestat);

            filesize = filestat.st_size;
            
            if(filesize == 0)
            {
                std::cout << name() << ": Error. The specified file is empty" << std::endl;
                return -1;
            }
            else if(filesize % 64 != 0)
            {
                std::cout << name() << ": Error. The playback file size must be an integer multiple of 64 bytes" << std::endl;
                return -1;
            }

            buf = mmap(NULL, filesize, PROT_READ, MAP_SHARED, lfiledes, 0);

            if(!buf)
            {
                std::cout << name() << ": Out of virtual memory" << std::endl;
                return -1;
            }

            madvise(buf, filesize, MADV_SEQUENTIAL);
            
            lcarrier = api->get_carrier_byid(get_target_id());

            if(lcarrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return -10;
            }

            pState = lcarrier->get_connectionstate();

            lcarrier->lock();

            switch(d_type)
            {
            case RP_ETHERNET:
                res = init_rtdex_ethernet(lcarrier);
                if(FAILURE(res))
                {
                    std::cout << name() << ": Error 0x" << res << " during RTDEx Ethernet initialization.\n\n";
					std::cout << ErrorToString(res) << std::endl;
                    goto theEnd;
                }
                break;

            case RP_PCIE:
                res = init_rtdex_pcie(lcarrier);
                if(FAILURE(res))
                {
                    std::cout << name() << ": Error 0x" << res << " during RTDEx PCIe initialization.\n\n";
					std::cout << ErrorToString(res) << std::endl;
                    goto theEnd;
                }
                break;
            }
            
            res = Carrier_GetModuleBaseAddr_send( pState, Carrier_eModuleRecPlay, 0, &uiRecPlayBaseAddr);
            if(FAILURE(res))
            {
				std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = RecPlay_Open( &lrecplay, pState, uiRecPlayBaseAddr );
            if(FAILURE(res))
            {
                std::cout << name() << ": Error 0x" << res << " while creating a record playback object.\n\n";
				std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = RecPlayGetMemoryInitDoneStatus( lrecplay, &done );
            if(FAILURE(res))
            {
                std::cout << name() << ": Error 0x" << res << " with RecPlayGetMemoryInitDoneStatus.\n";
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }
            
            while(!done)
            {
                res = RecPlayResetMemory( lrecplay );
                if(FAILURE(res))
                {
                    std::cout << name() << ": Error 0x" << res << " with RecPlayResetMemory.\n";
                    std::cout << ErrorToString(res) << std::endl;
                    goto theEnd;
                }
                
                usleep(1000000);
                
                res = RecPlayGetMemoryInitDoneStatus( lrecplay, &done );
                if(FAILURE(res))
                {
                    std::cout << name() << ": Error 0x" << res << " with RecPlayGetMemoryInitDoneStatus.\n";
					std::cout << ErrorToString(res) << std::endl;
                    goto theEnd;
                }
            }

            res = RecPlayResetCore( lrecplay );
            if(FAILURE(res))
            {
                std::cout << name() << ": Error 0x" << res << " with RecPlayResetCore.\n";
				std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            lframesize = (filesize < d_packet_size) ? filesize : d_packet_size;

            res = RecPlaySetModeRtdexHost2Mem( lrecplay, 0, filesize);
            if(FAILURE(res))
            {
                std::cout << name() << ": Error 0x" << res << " RTDEx host to memory mode could not be configured.\n";
				std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = RTDExStart( d_rtdex, lframesize, filesize);
            if(FAILURE(res))
            {
                std::cout << name() << ": Error 0x" << res << " RTDEX transfer could not be started.\n";
				std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = RTDExSend( d_rtdex, (unsigned char*)buf, filesize);
            if( res != filesize)
            {
                std::cout << " :  " << name() << ": Error 0x" << res << " RTDEX send failed\n";
				std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = PlaybackSetTriggerSource( lrecplay, eRecPlayTrigSoft);
            if(FAILURE(res))
            {
                std::cout << name() << ": Error 0x" << res << " with PlaybackSetTriggerSource.\n";
				std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            if(d_mode == eContinuous)
            {
                res = RecPlaySetModePlayBackContinuous( lrecplay, 0, filesize ) ;
                if(FAILURE(res))
                {
                    std::cout << name() << ": Error 0x" << res << " with RecPlaySetModePlayBackContinuous.\n";
					std::cout << ErrorToString(res) << std::endl;
                    goto theEnd;
                }
            }
            else{
                res = RecPlaySetModePlayBackSingle( lrecplay, 0, filesize ) ;
                if(FAILURE(res))
                {
                    std::cout << name() << ": Error 0x" << res << " with RecPlaySetModePlayBackSingle.\n";
					std::cout << ErrorToString(res) << std::endl;
                    goto theEnd;
                }          
            }

            res = PlaybackSoftTrig( lrecplay );
            if(FAILURE(res))
            {
                std::cout << name() << ": Error 0x" << res << " with PlaybackSoftTrig.\n";
				std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            //std::cout << name() << ": callback end successfully\n";

        theEnd:
            RecPlay_Close(lrecplay);
            switch(d_type)
            {
            case RP_ETHERNET:
                RTDExCloseEth(d_rtdex);
                break;
            case RP_PCIE:
                usleep(100000); // if the driver is still running, it will not close well.
                RTDExClosePcie(d_rtdex);
                break;
            }
            lcarrier->unlock();
            munmap(buf, filesize);
            close(lfiledes);
            return res;
        }

        Result playback_impl::init_rtdex_pcie( carrier * lcarrier )
        {
            nutaq_api * api = nutaq_api::get_instance();
            Result res;
            connection_state * pState = lcarrier->get_connectionstate();
            RTDExPerseusParamsPcie lpcie_param;
            int i,j;

            res = RTDExInitParamsPcie(&lpcie_param, pState);
            if(FAILURE(res))
            {
                std::cout << name() << ": Failed to initialize PCIe parameters " << d_channel << std::endl;
				std::cout << ErrorToString(res) << std::endl;
                return GNURADIO_TXFAILED;
            }
            
            res = RTDExResetCorePcie(&lpcie_param);
            if(FAILURE(res))
            {
                std::cout << name() << ": Failed to reset PCIe core " << d_channel << std::endl;
				std::cout << ErrorToString(res) << std::endl;
                return GNURADIO_TXFAILED;
            }

            res = RTDExOpenHostAndFpgaPcie(&d_rtdex, &lpcie_param, d_channel, eToFpga, eSingle);
            if(FAILURE(res))
            {
                std::cout << name() << ": Failed to open RTDEX PCIe channel " << d_channel << std::endl;
				std::cout << ErrorToString(res) << std::endl;
                return GNURADIO_TXFAILED;
            }

            return OK;
        }

        Result playback_impl::init_rtdex_ethernet( carrier * lcarrier )
        {
            nutaq_api * api = nutaq_api::get_instance();
            Result res;
            connection_state * pState = lcarrier->get_connectionstate();
            unsigned int uiRTDExBaseAddr;
            unsigned char pHostMacAddr[6];
            unsigned char pDeviceMacAddr[6];
            int i;

            memset(pHostMacAddr, 0, 6);
            memset(pDeviceMacAddr, 0, 6);

            res = iGetConnectionPeerMacAddr(pState, (unsigned char *) pDeviceMacAddr);
            if(FAILURE(res))
            {
				std::cout << ErrorToString(res) << std::endl;
                res = GNURADIO_MACADDR;
                goto exit;
            }

            std::cout << name() << ": Device MAC Address: ";
            for(i=0; i<5; i++)
            {
                std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(pDeviceMacAddr[i]);
                std::cout << ":";
            }
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(pDeviceMacAddr[5]) << std::endl;

            res = iGetConnectionLocalMacAddr(pState, (unsigned char *) pHostMacAddr);
            if(FAILURE(res))
            {
				std::cout << ErrorToString(res) << std::endl;
                res = GNURADIO_MACADDR;
                goto exit;
            }

            std::cout << name() << ": Host MAC Address: ";
            for(i=0; i<5; i++)
            {
                std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(pHostMacAddr[i]);
                std::cout << ":";
            }
            std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(pHostMacAddr[5]) << std::endl;
            
            
            res = Carrier_GetModuleBaseAddr_send( pState, Carrier_eModuleRTDExGigE, 0, &uiRTDExBaseAddr);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto exit;
            }

            res = RTDExResetCoreEth(pState, uiRTDExBaseAddr);
            if(FAILURE(res))
            {
				std::cout << ErrorToString(res) << std::endl;
                res = GNURADIO_RESETCORE;
                goto exit;
            }

            res = RTDExOpenEth( &d_rtdex, pHostMacAddr, eHost, pDeviceMacAddr, pState, uiRTDExBaseAddr, d_channel, eToFpga, eSingle);
            if(FAILURE(res))
            {
                std::cout << name() << ": Failed to open RTDEX channel " << d_channel << std::endl;
				std::cout << ErrorToString(res) << std::endl;
                res = GNURADIO_TXFAILED;
                goto exit;
            }

            res = RTDExEnableFlowControlEth(pState, uiRTDExBaseAddr, 32768 - ((d_packet_size >> 2) *  4), (d_packet_size >> 2) * 2 );
            if(FAILURE(res))
            {
                std::cout << name() << ": Failed to set flow control for RTDEX channel " << d_channel << std::endl;
				std::cout << ErrorToString(res) << std::endl;
                res = GNURADIO_FLOWCONTROL;
                goto exit;
            }

            return OK;

        exit:
            std::cout << name() << ": Init failed with error " << std::hex << res << std::endl;
            return res;

        }

    } /* namespace nutaq */
} /* namespace gr */

