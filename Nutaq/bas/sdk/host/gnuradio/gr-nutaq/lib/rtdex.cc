#include <nutaq/rtdex.h>
#include <nutaq/rtdex_ioctl.h>
#include "nutaq_api.h"
#include "carrier_zedboard_impl.h"
#include <boost/algorithm/string.hpp> 
#include <sys/ioctl.h>
#include <stdio.h>

#include <mac_utils.h>

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

#include <eapi_carrier.h>

#define SMALLEST_PACKET (128)

/* Define debugging for use during our driver bringup */
#undef DEBUG
//#define DEBUG(fmt, args...) printf(fmt, ## args)
#define DEBUG(fmt, args...)

namespace gr {
    namespace nutaq {

        rtdex::rtdex(int item_size, int num_chan)
        {
            d_packet_size = 1024;
            d_itemsize = item_size;
            d_num_chan = num_chan;
            
            set_min_noutput_items(SMALLEST_PACKET);
            set_output_multiple(SMALLEST_PACKET);
        }

        rtdex::rtdex()
        {
            d_packet_size = 1024;
            d_itemsize = sizeof(int);
            d_num_chan = 1;
            
            set_min_noutput_items(SMALLEST_PACKET);
            set_output_multiple(SMALLEST_PACKET);
        }

        int rtdex::callback()
        {
            Result res;
    
            res = init();
            if(FAILURE(res))
            {
                std::cout << name() << ": init failed ";
                std::cout << std::hex << res;
                std::cout << std::endl;
            }
            return res;
        }

        Result rtdex::init()
        {
            nutaq_api * api = nutaq_api::get_instance();
            int i;
            Result ret;
            carrier_class lclass;

            std::cout << name() << ": Init " << std::endl;

            d_carriers = api->get_carrier_vector_byid(get_target_id());
            if(d_carriers.size() == 0)
            {
                std::cout << name() << ": Target id \"" << get_target_id() << "\" is invalid." << std::endl;
                return -1;
            }

            lclass = d_carriers[0]->get_class();
            for(i=1; i<d_carriers.size(); i++)
            {
                if(d_carriers[i]->get_class() != lclass)
                {
                    std::cout << name() << ": Carriers do not have the same class." << std::endl;
                    return -3;
                }
            }

            if(d_carriers.size() != d_channels.size())
            {
                std::cout << name() << ": Number of Target ids do not match number of channels." << std::endl;
                std::cout << d_carriers.size() << std::endl;
                std::cout << d_channels.size() << std::endl;
                return -4;
            }

            switch(lclass)
            {
            default:
            case CARRIER_PERSEUS:
                set_output_multiple(d_packet_size / d_itemsize);

                switch(d_type)
                {

                case RTDEX_ETHERNET:
                    ret = init_rtdex_ethernet();
                    break;

                case RTDEX_PCIE:
                    ret = init_rtdex_pcie();
                    break;
                default:
                    ret = ERR_INVHANDLE;
                    break;
                }
                break;

            case CARRIER_ZEDBOARD:
                
                switch(d_type)
                {
                case RTDEX_ETHERNET:
                    ret = init_rtdex_tcpip();
                    break;
                    
                case RTDEX_AXI:
                    ret= init_rtdex_axi();
                    break;
                default:
                    ret = ERR_INVHANDLE;
                    break;
                }
                break;
            }

            return ret;
        }

        Result rtdex::init_rtdex_tcpip()
        {
            nutaq_api * api = nutaq_api::get_instance();
            Result res;
            carrier_zedboard_impl * lcarrier_zedboard;
            adp_handle_rtdex_conn lrtdex;
            int i;
            
            d_rtdex.clear();
            d_rtdex_carriers.clear();
            
            for(i=0; i<d_carriers.size();i++)
            {
                if(d_channels[i].size() > 1)
                {
                    std::cout << name() << ": Error. For Zedboard carrier, there should be only one channel by target." << std::endl;
                    return -6;
                }
                
                std::vector<int>::iterator it = d_channels[i].begin();
                if(*it != 0)
                {
                    std::cout << name() << ": Error. For Zedboard carrier, channel ID should be 0." << std::endl;
                    return -7;
                }
            }
            
            for(i=0; i<d_carriers.size(); i++)
            {
                lcarrier_zedboard = (dynamic_cast<carrier_zedboard_impl*>(d_carriers[i]));
                lcarrier_zedboard->lock();
                
                if(lcarrier_zedboard->is_open() == true)
                {
                    d_rtdex.push_back(lcarrier_zedboard->get_rtdex_handle());
                    d_rtdex_carriers.push_back(d_carriers[i]);
                }
                else
                {
                    res = RTDExOpenTcpIp( &lrtdex, (int8_t*)(lcarrier_zedboard->get_ip_addr().c_str()));      
                    if(FAILURE(res))
                    {
                        std::cout << ErrorToString(res) << std::endl;
                        lcarrier_zedboard->unlock();
                        return res;
                    }                    
                    
                    lcarrier_zedboard->set_open(true);
                       
                    d_rtdex.push_back(lrtdex);
                    d_rtdex_carriers.push_back(d_carriers[i]);
                    
                    lcarrier_zedboard->set_rtdex_handle(lrtdex);   
                }
                
                lcarrier_zedboard->unlock();
            }
            
            return OK;
        }
        
        Result rtdex::init_rtdex_axi()
        {
            nutaq_api * api = nutaq_api::get_instance();
            Result res;
            carrier_zedboard_impl * lcarrier_zedboard;
            adp_handle_rtdex_conn lrtdex;
            adp_handle_rtdex_axi_conn pAxiConn;
            const char device_name[] = "/dev/rtdex0";
            
            if (d_carriers.size() > 1)
            {
                std::cout << name() << ": There can be only one Axi Zedboard Carrier. QUITTING" << std::endl;
                return -8;
            }
            
            if(d_channels[0].size() > 1)
            {
                std::cout << name() << ": There should be only one channel in the channel ID. QUITTING" << std::endl;
                return -6;
            }
            
            std::vector<int>::iterator it = d_channels[0].begin();
            if(*it != 0)
            {
                std::cout << name() << ": Channel ID should be 0. QUITTING" << std::endl;
                return -7;
            }
            
            d_rtdex.clear();
            d_rtdex_carriers.clear();
                        
            lcarrier_zedboard = (dynamic_cast<carrier_zedboard_impl*>(d_carriers[0]));
            lcarrier_zedboard->lock();
            
            if(lcarrier_zedboard->is_open() == true)
            {
                d_rtdex.push_back(lcarrier_zedboard->get_rtdex_handle());
                d_rtdex_carriers.push_back(d_carriers[0]);
            }
            else
            {
                res = RTDExOpenAxi( &lrtdex, (int8_t*)(device_name));
                if(FAILURE(res))
                {
                    std::cout << ErrorToString(res) << std::endl;
                    lcarrier_zedboard->unlock();
                    return res;
                }            
                
                lcarrier_zedboard->set_open(true);
                   
                d_rtdex.push_back(lrtdex);
                d_rtdex_carriers.push_back(d_carriers[0]);
                
                lcarrier_zedboard->set_rtdex_handle(lrtdex);  
            }
            
            //ioctl
            if(d_itemsize == sizeof(std::complex<float>))
            {
                pAxiConn = (adp_handle_rtdex_axi_conn)((lcarrier_zedboard->get_rtdex_handle())->hMediaRTDexConn);
                if (name().compare("rtdex_sink") == 0)
                    ioctl( pAxiConn->m_dev, RTDEX_SET_WRITE_CAST_MODE, RTDEX_CAST_MODE_CMPLX2SHORT);
                else
                    ioctl( pAxiConn->m_dev, RTDEX_SET_WRITE_CAST_MODE, RTDEX_CAST_MODE_NONE);
            
                if (name().compare("rtdex_source") == 0)
                    ioctl( pAxiConn->m_dev, RTDEX_SET_READ_CAST_MODE, RTDEX_CAST_MODE_CMPLX2SHORT);
                else
                    ioctl( pAxiConn->m_dev, RTDEX_SET_READ_CAST_MODE, RTDEX_CAST_MODE_NONE);
            }
            
            lcarrier_zedboard->unlock();
            return OK;
        }

        Result rtdex::init_rtdex_pcie()
        {
            nutaq_api * api = nutaq_api::get_instance();
            Result res;
            connection_state * pState;
            carrier * lcarrier;
            adp_handle_rtdex_conn lrtdex;
            RTDExPerseusParamsPcie lpcie_param;
            int i,j;

            d_rtdex.clear();
            d_rtdex_carriers.clear();

            for(j=0; j<d_carriers.size(); j++)
            {
                lcarrier = d_carriers[j];
                pState = lcarrier->get_connectionstate();
                lcarrier->lock();

                RTDExInitParamsPcie(&lpcie_param, pState);
                RTDExResetCorePcie(&lpcie_param);

                for(std::vector<int>::iterator it = d_channels[j].begin(); it != d_channels[j].end(); ++it)
                {
                    res = RTDExOpenHostAndFpgaPcie(&lrtdex, &lpcie_param, *it, (eRTDExTransferDir) get_direction(), eContinuous);
                    if(FAILURE(res))
                    {
                        std::cout << name() << ": Failed to open TX channel " << *it << std::endl;
                        std::cout << ErrorToString(res) << std::endl;
                        res = GNURADIO_TXFAILED;
                        goto theEnd;
                    }
                    d_rtdex.push_back(lrtdex);
                    d_rtdex_carriers.push_back(d_carriers[j]);
                }

                lcarrier->unlock();
            }
            return OK;

        theEnd:
            lcarrier->unlock();

            return res;
        }

        Result rtdex::init_rtdex_ethernet()
        {
            nutaq_api * api = nutaq_api::get_instance();
            Result res;
            connection_state * pState;
            unsigned char pHostMacAddr[6];
            unsigned char pDeviceMacAddr[6];
            carrier * lcarrier;
            unsigned int uiRTDExBaseAddr;
            adp_handle_rtdex_conn lrtdex;
            int i,j;

            memset(pHostMacAddr, 0, 6);
            memset(pDeviceMacAddr, 0, 6);

            d_rtdex.clear();
            d_rtdex_carriers.clear();
            
            if(d_packet_size > 9014)
            {
                std::cout << "Error : The maximal RTDEx Ethernet packet size is 9014 Bytes." << std::endl;
                res = GNURADIO_INVVALUE;
                std::cout << name() << ": Init failed with error" << std::endl;
                return res;
            }

            for(j=0; j<d_carriers.size(); j++)
            {
                lcarrier = d_carriers[j];
                pState = lcarrier->get_connectionstate();
                lcarrier->lock();

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


                for(std::vector<int>::iterator it = d_channels[j].begin(); it != d_channels[j].end(); ++it)
                {
                    res = RTDExOpenEth( &lrtdex,  pHostMacAddr, eHost, pDeviceMacAddr, pState, uiRTDExBaseAddr, *it, (eRTDExTransferDir)get_direction(), eContinuous );
                    if(FAILURE(res))
                    {
                        std::cout << name() << ": Failed to open TX channel " << *it << std::endl;
                        std::cout << ErrorToString(res) << std::endl;
                        res = GNURADIO_TXFAILED;
                        goto exit;
                    }
                    d_rtdex.push_back(lrtdex);
                    d_rtdex_carriers.push_back(d_carriers[j]);
                }

                
                res = RTDExEnableFlowControlEth(pState, uiRTDExBaseAddr, 32768 - ((d_packet_size >> 2) *  4), (d_packet_size >> 2) * 2 );
                if(FAILURE(res))
                {
                    std::cout << ErrorToString(res) << std::endl;
                    res = GNURADIO_FLOWCONTROL;
                    goto exit;
                }
                
                lcarrier->unlock();
            }

            return OK;

        exit:
            lcarrier->unlock();
            std::cout << name() << ": Init failed with error" << std::endl;
            return res;
        }

        void rtdex::set_packet_size(int packet_size)
        {
            d_packet_size = packet_size;
        }

        void rtdex::set_channels(std::string channels)
        {
            int i,j;
            std::vector<std::string> carrier_list;
            std::vector<std::string> channel_list;
            int num_chan;

            boost::split(carrier_list, channels, boost::is_any_of("-;"));

            if(carrier_list.size() == 0)
            {
                std::cout << name() << ": No valid channel" << std::endl;
            }

            d_channels.clear();

            num_chan = 0;

            for(j=0; j<carrier_list.size(); j++)
            {
                boost::split(channel_list, carrier_list[j], boost::is_any_of(", "));

                d_channels.push_back(std::vector<int>());
                for (i=0; i<channel_list.size(); i++)
                {
                    d_channels[j].push_back(atoi(channel_list[i].c_str()));
                    num_chan++;
                }
            }
        
            if(num_chan != d_num_chan)
            {
                std::cout << name() << ": Error. Number of channels is not the same as the number of connected streams." << std::endl;
                exit(-5);
            }
        }

        void rtdex::set_type(rtdex_type type)
        {
            d_type = type;
        }

        bool rtdex::start()
        {
            Result res = 0;
            nutaq_api * api = nutaq_api::get_instance();
            carrier_zedboard_impl * lcarrier_zedboard;
            carrier_class lclass;
            adp_handle_rtdex_conn lrtdex;
            int i,j;

            res = api->try_init();
            if (res < 0)
            {
                exit(-1);
            }
            
            res = api->wait_for_init();
            if (res < 0)
            {
                exit(-1);
            }

            //std::cout << name() << ": Started" << std::endl;

            for(std::vector<carrier*>::iterator it = d_carriers.begin(); it != d_carriers.end(); ++it)
            {
                (*it)->lock();
            }

            for(i=0; i < d_rtdex.size(); i++)
            {
                lrtdex = (adp_handle_rtdex_conn)d_rtdex[i];
                
                if(d_rtdex_carriers[i]->get_class() == CARRIER_ZEDBOARD)
                {
                    lcarrier_zedboard = (dynamic_cast<carrier_zedboard_impl*>(d_rtdex_carriers[i]));
                    if(!lcarrier_zedboard->is_started())
                    {
                        res = RTDExStart(lrtdex, d_packet_size, 0);
                        lcarrier_zedboard->set_started(true);
                    }
                    else
                    {
                        res = OK;
                    }
                }
                else
                {
                    res = RTDExStart(lrtdex, d_packet_size, 0);
                }
                
                if(FAILURE(res))
                {
                    std::cout << name() << ": Failed to start channel " << i << std::endl;
                    goto failure;
                }
            }

            for(std::vector<carrier*>::iterator it = d_carriers.begin(); it != d_carriers.end(); ++it)
            {                
                (*it)->unlock();
            }

            std::cout << name() << ": End Good" << std::endl;
            
            // Sleep 1 second to give time to initial custom register work function
            // to be called before the RTDEx stream work functions start.
            // This patch is done because the RTDEx only send 1 stop packet
            // and if there is no data consumption, the RTDEx will jam the Ethernet link
            // by always sending data since no stop packet will be send again.
            // Since there is a 1 second sleep, the custom register work function can enable
            // the data consumption before the RTDEx start sending data.
            usleep(1000000);
            
            return true;

        failure:
            for(std::vector<carrier*>::iterator it = d_carriers.begin(); it != d_carriers.end(); ++it)
            {
                (*it)->unlock();
            }
            std::cout << name() << ": Failed to start RTDEX" << std::endl;
            return false;
        }

        bool rtdex::stop()
        {
            Result res;
            int rv;
            carrier_class lclass;
            carrier_zedboard_impl * lcarrier_zedboard;
            adp_handle_rtdex_conn lrtdex;
            int i;

            std::cout << name() << ": stopped" << std::endl;

            //This is valid since all carrier are checked to be of the same class
            for(std::vector<carrier*>::iterator it = d_carriers.begin(); it != d_carriers.end(); ++it)
            {
                (*it)->lock();
            }

            lclass = d_carriers[0]->get_class();
            
            for(i=0; i < d_rtdex.size(); i++)
            {
                lrtdex = (adp_handle_rtdex_conn)d_rtdex[i];
                
                switch(d_type)
                {                    
                    case RTDEX_ETHERNET:
                        if (lclass == CARRIER_PERSEUS)
                        {
                            RTDExCloseEth(lrtdex);
                        }
                        else
                        {
                            lcarrier_zedboard = (dynamic_cast<carrier_zedboard_impl*>(d_rtdex_carriers[i]));
                            if(lcarrier_zedboard->is_started())
                            {
                                RTDExCloseTcpIp(lrtdex);
                                lcarrier_zedboard->set_open(false);
                                lcarrier_zedboard->set_started(false);
                            }
                        }
                        break;

                    case RTDEX_PCIE:
                        usleep(200000); // if the driver is still running, it will not close well.
                        RTDExClosePcie(lrtdex);
                        break;
                        
                    case RTDEX_AXI:
                    
                        lcarrier_zedboard = (dynamic_cast<carrier_zedboard_impl*>(d_rtdex_carriers[0]));
                        if(lcarrier_zedboard->is_started())
                        {
                            RTDExCloseAxi(lrtdex);
                            lcarrier_zedboard->set_open(false);
                            lcarrier_zedboard->set_started(false);
                        }
                    
                        break;
                        
                    default:
                        break;
                }
                //TODO reopen connection
            }

            for(std::vector<carrier*>::iterator it = d_carriers.begin(); it != d_carriers.end(); ++it)
            {
                (*it)->unlock();
            }

            return true;
        }

    } /* namespace nutaq */
} /* namespace gr */
