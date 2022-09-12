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

#include "eapi_radio420.h"
#include "fmc_radio_common.h"
#include "fmc_radio_defines.h"
#include "lms6002_defines.h"

#include <gnuradio/io_signature.h>
#include "radio420_rx_impl.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace gr {
    namespace nutaq {

        radio420_rx::sptr
        radio420_rx::make(std::string target_id, unsigned selected_card, int block_priority)
        {
            return gnuradio::get_initial_sptr
                (new radio420_rx_impl(target_id, selected_card, block_priority));
        }

        /*
         * The private constructor
         */
        radio420_rx_impl::radio420_rx_impl(std::string target_id, unsigned selected_card, int block_priority)
            : gr::sync_block("radio420_rx",
                             gr::io_signature::make(0, 2, sizeof(float)),
                             gr::io_signature::make(0, 0, sizeof(float))),
              nutaq_cb(block_priority),
              carrier_client(target_id)
        {
            d_enable = false;
            d_calibrate = false;
            d_rev = FMCRADIOREVSDR_D;
            d_datarate = FMCRADIO_DATARATE_REF_30_72MHZ_7DOT68MHZ;
            d_reffreq = FMC_RADIO_SDR_REVC_REFERENCE_CLOCK;
            d_reference = 0;
            d_band = 0;
            d_rx_lna_gain = 0;
            d_rx_vga1_gain = 0;
            d_rx_gain2 = 0;
            d_rx_gain3 = 0;
            d_rx_rf_filter = FMCRADIOSDR_REVC_FILTER_BYPASS;
            d_rx_lpf_bandwidth = LMS6002_LPF_14MHZ;
			d_ref_clk_ctrl = FMCRADIO_HOST;
			d_rf_ctrl = FMCRADIO_HOST;
			d_rx_gain_ctrl = FMCRADIO_HOST;
			d_pll_cpld_ctrl = FMCRADIO_HOST;
            d_rate = 0;
            d_wait_time = 0;
            delay_counter_reset(&d_delay);
            delay_counter_begin(&d_delay);
            d_selected_card = selected_card;
            set_alignment(1);
            set_max_noutput_items(1);            
        }

        /*
         * Our virtual destructor.
         */
        radio420_rx_impl::~radio420_rx_impl()
        {
        }

        int radio420_rx_impl::callback()
        {
			return init();
        }

        int radio420_rx_impl::init()
        {
            nutaq_api * api = nutaq_api::get_instance();
            carrier * carrier;
            Result res;
            
            connection_state * pState;
            carrier_class lclass;

            cdce62005_param cdce_param;
            int pllstatus = 0;
            FMCRADIOSDR_FILTER_BANK filter;
            lms6002_pll_param lime_pll_param;
            unsigned int uRefFreq = 0;
            unsigned int uLimeFreq = 0;
            LMS6002_VGA1_GAIN tmp_gain;
            lms6002_rx_dc_calib rx_dc_calib;
            lms6002_lo_leakage_calib lo_leakage_calib;
            lms6002_ssb_calib ssb_calib;
            unsigned datarate_tmp;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();
            
            lclass = carrier->get_class();

            std::cout << "radio420_rx::InitRadio420x: " << d_selected_card << std::endl;

            if (d_rev == FMCRADIOREVSDR_B || d_rev == FMCRADIOREVSDR_A)
            {
                uRefFreq = FMC_RADIO_SDR_REVB_REFERENCE_CLOCK;
                uLimeFreq = FMC_RADIO_SDR_REVB_LIME_CLOCK;
            }
            else if (d_rev == FMCRADIOREVSDR_C || d_rev == FMCRADIOREVSDR_D)
            {
                uRefFreq = FMC_RADIO_SDR_REVC_REFERENCE_CLOCK;
                uLimeFreq = FMC_RADIO_SDR_REVC_LIME_CLOCK;
            }
            else
            {
                printf("Error: Erroneous board d_rev\r\n");
                return -1;
            }

            //start init procedure
            carrier->lock();
            
            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
				printf("Error 0x%x: Cannot select card\r\n",res);
                printf("%s\n",ErrorToString(res));
            }
			
			//Sets SPI control to Host
			res = fmc_radio_spi_control_set_send(pState, FMCRADIO_CTRL_REF_CLOCK, FMCRADIO_HOST);
			if(FAILURE(res))
			{
				printf("Error : Could not set SPI control for reference clock to Host for init\r\n");
				printf("%s\n",ErrorToString(res));
			}
			res = fmc_radio_spi_control_set_send(pState, FMCRADIO_CTRL_RF, FMCRADIO_HOST);
			if(FAILURE(res))
			{
				printf("Error : Could not set SPI control for RF to Host for init\r\n");
				printf("%s\n",ErrorToString(res));
			}
			res = fmc_radio_spi_control_set_send(pState, FMCRADIO_CTRL_RX_GAIN, FMCRADIO_HOST);
			if(FAILURE(res))
			{
				printf("Error : Could not set SPI control for RX gain to Host for init\n");
				printf("%s\n",ErrorToString(res));
			}
			res = fmc_radio_spi_control_set_send(pState, FMCRADIO_CTRL_PLL_CPLD, FMCRADIO_HOST);
			if(FAILURE(res))
			{
				printf("Error : Could not set SPI control for PLL/CPLD to Host for init\r\n");
				printf("%s\n",ErrorToString(res));
			}
			//Ends of SPI control to Host			

            if(!carrier->is_peripheral_reseted(d_selected_card))
            {
                res = fmc_radio_setrevision_send(pState, d_rev);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not set revision\r\n", res);
                    goto theEnd;
                }
                
                res = fmc_radio_presence(pState);   // verify presence
                if(FAILURE(res))
                {
                    printf("Error: Could detect Radi420 FPGA core\r\n");
                    return res;
                }
            
                if(lclass == CARRIER_PERSEUS && d_selected_card == 1)
                {
                    // fmcradio_powerup
                    printf("    Powering up hardware\r\n");
                    res = fmc_radio_powerup_send(pState);
                    if(FAILURE(res))
                    {
                        printf("Error 0x%x: Could not power up radio\r\n", res);
                        goto theEnd;
                    }
                }
                res = fmc_radio_setrevision_send(pState, d_rev);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not set revision\r\n", res);
                    goto theEnd;
                }

                printf("    Resetting hardware\r\n");
                // fmcradio_reset
                res = fmc_radio_sdr_reset_send(pState);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not reset radio\r\n", res);
                    goto theEnd;
                }
                
                carrier->set_peripheral_rx_enable(d_selected_card, false);

                res = fmc_radio_clockmux_set_send(pState, FMCRADIO_CLOCKOUT_FMCCLK0, FMCRADIO_CLOCKIN_PLLCLKOUT);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: could not configure clock mutliplexer\n", res);
					printf("%s\n",ErrorToString(res));
                }
            }

            //Verify that all Radios of a carrier have the same data rate
            datarate_tmp = api->get_datarate(get_target_id());
            if(!datarate_tmp)
            {
                api->set_datarate(get_target_id(), d_datarate);
            }else{
                if(datarate_tmp != d_datarate)
                {
                    std::cout << "radio420_rx: ERROR, All data rates need to be set to the same value when targeting the same carrier" << std::endl;
                    goto theEnd;
                }
            }

            if(!carrier->is_peripheral_reseted(d_selected_card))
            {
                printf("    Configuring PLL\r\n");
                res = fmc_radio_pll_calculate(&cdce_param, uRefFreq, d_datarate, uLimeFreq);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not calculate pll parameters\r\n", res);
                    goto theEnd;
                }
                
                if(d_reference == 0)
                {
                    printf("      - Reference frequency %d Hz from internal oscillator\r\n", uRefFreq);
                }
                else if(d_reference == 1)
                {
                    cdce_param.secondary_clock = 1;
                    printf("      - Reference frequency %d Hz from external connector\r\n", uRefFreq);
                }
                else
                {
                    printf("Error: Invalid reference value\r\n");
                    printf("       0: Internal, 1: External\r\n");
                    goto theEnd;
                }

                printf("      - Acquisition frequency %d Hz\r\n", d_datarate);
                printf("      - Lime frequency %d Hz \r\n", uLimeFreq);

                res = fmc_radio_pll_setparam_send(pState, &cdce_param);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not write pll parameters\r\n", res);
                    goto theEnd;
                }
                if(cdce_param.vco_bypass_all == 1)
                {
                    printf("    PLL is set in bypass\r\n");
                }
                else
                {
                    //  fmcradio_pll_lock
                    res = fmc_radio_pll_lockstatus_send(pState, &pllstatus);
                    if(FAILURE(res))
                    {
                        printf("Error 0x%x: Could not read pll status\r\n", res);
                        goto theEnd;
                    }
                    // Check if the pll is locked
                    if( pllstatus != CDCE62005_PLL_LOCKED )
                    {
                        printf("Error: PLL is not locked\r\n");
                    }
                    else
                    {
                        printf("    PLL is locked\r\n");
                    }
                }
            }
            
            if(d_enable)
            {
                //Configure RX
                res = fmc_radio_path_enable_send(pState, LMS6002_RX);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not enable RX path\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }
                
                carrier->set_peripheral_rx_enable(d_selected_card, true);

                res = fmc_radio_sdr_setrxfilter_send(pState, (FMCRADIOSDR_FILTER_BANK)d_rx_rf_filter);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not set rx filter\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }

                switch(d_rx_vga1_gain)
                {
                default:
                case 1:
                    tmp_gain = LMS_VGA1_GAIN5DB;
                    break;
                case 2:
                    tmp_gain = LMS_VGA1_GAIN19DB;
                    break;
                case 3:
                    tmp_gain = LMS_VGA1_GAIN30DB;
                    break;
                }

                res = fmc_radio_sdr_rx_gain_send(pState, (LMS6002_LNA_GAIN)d_rx_lna_gain, tmp_gain, d_rx_gain2, d_rx_gain3);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not set rx gain\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }
                
                res = fmc_radio_sdr_band_send(pState, (FMC_RADIO_SDR_BAND)d_band);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Cannot select band\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }

                res = fmc_radio_lpf_set_send(pState, LMS6002_RX, (LMS6002_LPF_BANDWIDTH)d_rx_lpf_bandwidth);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not set RX low-pass filter\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }

                res = fmc_radio_lime_calculate(&lime_pll_param, uLimeFreq, d_rx_freq);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not calculate rx lime pll parameters. Invalid configuration\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }

                res = fmc_radio_lime_setpllparam_send(pState, LMS6002_RX, &lime_pll_param);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not set lime rx pll param\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }

                res = fmc_radio_lime_calibratePLL_send(pState, LMS6002_RX);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not calibrate lime rx pll\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }
                                
                res = fmc_radio_rxvga_calibrate_send(pState);
                if(FAILURE(res))
                {
                    printf("Error: Could not calibrate RXVGA\r\n");
                }

                if(!carrier->is_peripheral_reseted(d_selected_card))
                    res = fmc_radio_lpf_calibrate_send(pState, LMS6002_RX, uRefFreq);
                else
                    res = fmc_radio_lpf_calibrate_send(pState, LMS6002_TXRX, uRefFreq);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not calibrate low pass filter\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }
                
                if(d_calibrate)
                {
                    printf("    Automatic calibration\r\n");
                    fmc_radio_rx_dc_offset_calibration_send(pState, &rx_dc_calib);
                }
            }
            else
            {
                res = fmc_radio_path_disable_send(pState, LMS6002_RX);
                if(FAILURE(res))
                {
                    printf("Error 0x%x: Could not disable RX path\r\n", res);
                    printf("%s\n",ErrorToString(res));
                }
                
                carrier->set_peripheral_rx_enable(d_selected_card, false);
            }
			
			//Sets SPI control			
			res = fmc_radio_spi_control_set_send(pState, FMCRADIO_CTRL_REF_CLOCK, (FMCRADIO_SPI_CTRL)d_ref_clk_ctrl);
			if(FAILURE(res))
			{
				printf("Error : Could not set SPI control for reference clock\r\n");
				printf("%s\n",ErrorToString(res));
			}
			res = fmc_radio_spi_control_set_send(pState, FMCRADIO_CTRL_RF, (FMCRADIO_SPI_CTRL)d_rf_ctrl);
			if(FAILURE(res))
			{
				printf("Error : Could not set SPI control for RF\r\n");
				printf("%s\n",ErrorToString(res));
			}
			res = fmc_radio_spi_control_set_send(pState, FMCRADIO_CTRL_RX_GAIN, (FMCRADIO_SPI_CTRL)d_rx_gain_ctrl);
			if(FAILURE(res))
			{
				printf("Error : Could not set SPI control for RX gain\n");
				printf("%s\n",ErrorToString(res));
			}
			res = fmc_radio_spi_control_set_send(pState, FMCRADIO_CTRL_PLL_CPLD, (FMCRADIO_SPI_CTRL)d_pll_cpld_ctrl);
			if(FAILURE(res))
			{
				printf("Error : Could not set SPI control for PLL/CPLD\r\n");
				printf("%s\n",ErrorToString(res));
			}
			//Ends of SPI control to Host

            carrier->set_peripheral_reset(d_selected_card, true);
            res = 0;
			printf("rx ended\n");
                        
        theEnd:
            carrier->unlock();
            return res;
        }

        void radio420_rx_impl::set_default_selected_card(unsigned selected)
        {
            d_selected_card = selected;
        }

        void radio420_rx_impl::set_default_enable(bool enable)
        {
            d_enable = enable;
        }

        void radio420_rx_impl::set_default_datarate(double datarate)
        {
            d_datarate = datarate;
        }

        void radio420_rx_impl::set_default_rx_freq(double freq)
        {
            d_rx_freq = freq;
        }

        void radio420_rx_impl::set_default_reference(int reference)
        {
            d_reference = reference;
        }

        void radio420_rx_impl::set_default_calibrate(bool calibrate)
        {
            d_calibrate = calibrate;
        }
        
        void radio420_rx_impl::set_default_band(unsigned band)
        {
            d_band = band;
        }

        void radio420_rx_impl::set_default_update_rate(float rate)
        {
            d_rate = rate;
            d_wait_time = 1 / rate;
        }
        
        void radio420_rx_impl::set_default_rx_lna_gain(unsigned lna_gain)
        {
            d_rx_lna_gain = lna_gain;
        }

        void radio420_rx_impl::set_default_rx_vga1_gain(unsigned vga1_gain)
        {
            d_rx_vga1_gain = vga1_gain;
        }

        void radio420_rx_impl::set_default_rx_gain2(unsigned rx_gain2)
        {
            d_rx_gain2 = rx_gain2;
        }

        void radio420_rx_impl::set_default_rx_gain3(int rx_gain3)
        {
            d_rx_gain3 = rx_gain3;
        }

        void radio420_rx_impl::set_default_rx_rf_filter(unsigned filter)
        {
            d_rx_rf_filter = filter;
        }

        void radio420_rx_impl::set_default_rx_lpf_bandwidth(unsigned bandwidth)
        {
            d_rx_lpf_bandwidth = bandwidth;
        }

        void radio420_rx_impl::set_default_ref_clk_ctrl(unsigned ref_clk_ctrl)
		{
			d_ref_clk_ctrl = ref_clk_ctrl;
		}
		void radio420_rx_impl::set_default_rf_ctrl(unsigned rf_ctrl)
		{
			d_rf_ctrl = rf_ctrl;
		}
		void radio420_rx_impl::set_default_rx_gain_ctrl(unsigned rx_gain_ctrl)
		{
			d_rx_gain_ctrl = rx_gain_ctrl;
		}
		void radio420_rx_impl::set_default_pll_cpld_ctrl(unsigned pll_cpld_ctrl)
		{
			d_pll_cpld_ctrl = pll_cpld_ctrl;
		}
        
        int radio420_rx_impl::set_rx_gain(int rx_gain)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            LMS6002_VGA1_GAIN tmp_gain;
            int gain = rx_gain;

            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();
            
            carrier->lock();
            
            
            // Select the current radio card
            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                printf("Error 0x%x: Cannot select card\r\n",res);
				printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            
            switch(d_rx_vga1_gain)
            {
                default:
                case 1:
                    tmp_gain = LMS_VGA1_GAIN5DB;
                    break;
                case 2:
                    tmp_gain = LMS_VGA1_GAIN19DB;
                    break;
                case 3:
                    tmp_gain = LMS_VGA1_GAIN30DB;
                    break;
            }
            
            // Clip gain when limit values have been reached
            if(gain < -13) gain = -13;
            if(gain > 18) gain = 18;
            
            //printf("Set RX gain : %d\r\n", gain);

            // Send the set tx gain command
            res = fmc_radio_sdr_rx_gain_send(pState, (LMS6002_LNA_GAIN)d_rx_lna_gain, tmp_gain, d_rx_gain2, gain);
            d_rx_gain3 = gain;
            if(FAILURE(res))
            {
                printf("Error 0x%x: Error setting rx gain\r\n", res);
				printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            
        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::set_rx_freq(double carrier_freq)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            long long llfreq;
            lms6002_pll_param lime_pll_param;
            unsigned int uLimeFreq;

            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();
            
            carrier->lock();
            
            
            // Select the current radio card
            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                printf("Error 0x%x: Cannot select card\r\n",res);
				printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            
            // Clip gain when limit values have been reached
            llfreq = llroundf(carrier_freq);
            if(llfreq < 300e6) llfreq = 300e6;
            if(llfreq > 3800e6) llfreq = 3800e6;
            
            //printf("Set RX freq : %d\r\n", llfreq);
            
            if (d_rev == FMCRADIOREVSDR_B || d_rev == FMCRADIOREVSDR_A)
            {
                uLimeFreq = FMC_RADIO_SDR_REVB_LIME_CLOCK;
            }
            else
            {
                uLimeFreq = FMC_RADIO_SDR_REVC_LIME_CLOCK;
            }
        
            res = fmc_radio_lime_calculate(&lime_pll_param, uLimeFreq, llfreq);
            if(FAILURE(res))
            {
                printf("Error 0x%x: Error calculating PLL settings\r\n",res);
				printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            
            res = fmc_radio_lime_setpllparam_send(pState, LMS6002_RX, &lime_pll_param);
            d_rx_freq = llfreq;
            if(FAILURE(res))
            {
                printf("Error 0x%x: Error configuring PLL\r\n",res);
				printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

            res = fmc_radio_lime_calibratePLL_send(pState, LMS6002_RX);
            if(FAILURE(res))
            {
                printf("Error 0x%x: Could not calibrate lime rx pll\r\n", res);
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

        theEnd:
            carrier->unlock();
            return res;
        }	

        int radio420_rx_impl::reset()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;

            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();
            
            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_sdr_reset_send(pState);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }
            
            carrier->set_peripheral_rx_enable(d_selected_card, false);

        theEnd:
            carrier->unlock();
            return res;
        }

        int radio420_rx_impl::write_spi(int device, int addr, int data)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;         
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();

            // Verify the validity of the device
            if(device<0 || device>6)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_spi_write_send(pState, (FMCRADIO_DEVICES)device, (unsigned)addr, (unsigned)data);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::read_spi(int device, int addr)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            unsigned data; 

            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();

            // Verify the validity of the device
            if(device<0 || device>6)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_spi_read_send(pState, (FMCRADIO_DEVICES) device, addr, &data);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }
            carrier->unlock();
            return data;


        theEnd:
            carrier->unlock();
            return res;
        }

        int radio420_rx_impl::set_pll(double ref_freq, double adac_freq, double lime_freq)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            cdce62005_param param;       
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_pll_calculate(&param, (unsigned)ref_freq, (unsigned)adac_freq, (unsigned)lime_freq);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_pll_setparam_send(pState, &param);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::read_pll_lockstatus()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            int pllstatus;           
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_pll_lockstatus_send(pState, &pllstatus);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }
            
            carrier->unlock();
            return pllstatus;

        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::wait_pll_lockstatus()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;        
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_pll_waitforlock_send(pState);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }                        

        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::set_clockmux(int dest, int src)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res; 
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the destination
            if(dest<0 || dest>3)
                return GNURADIO_INVVALUE;

            // Verify the validity of the source
            if(src<0 || src>3)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_clockmux_set_send(pState, (FMCRADIO_CLOCK_OUTPUT_ID)dest, (FMCRADIO_CLOCK_INPUT_ID)src);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }                        

        theEnd:
            carrier->unlock();
            return res;
        }       
        
        int radio420_rx_impl::calibrate_pll()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
                       
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_lime_calibratePLL_send(pState, LMS6002_RX);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }  
            

        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::reset_lime()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;          
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_lime_reset_send(pState);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::set_band(int band)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;           
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the band
            if(band<0 || band>1)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_sdr_band_send(pState, (FMC_RADIO_SDR_BAND)band);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::calibrate_vga()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;           
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_rxvga_calibrate_send(pState);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::set_filter(int filter)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;           
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the filter
            if(filter<0 || filter>7)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_sdr_setrxfilter_send(pState, (FMCRADIOSDR_FILTER_BANK)filter);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::set_lpf(int bwt)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;          
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the bwt
            if(bwt<0 || bwt>16)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_lpf_set_send(pState, LMS6002_RX, (LMS6002_LPF_BANDWIDTH)bwt);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::calibrate_lpf(double ref_freq)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;         
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_lpf_calibrate_send(pState, LMS6002_RX, (unsigned)ref_freq);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::enable_path()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;          
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_path_enable_send(pState, LMS6002_RX);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }
            
            carrier->set_peripheral_rx_enable(d_selected_card, true);


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::disable_path()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;          
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_path_disable_send(pState, LMS6002_RX);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }
            
            carrier->set_peripheral_rx_enable(d_selected_card, false);


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::set_loopback(int mode)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;          
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the mode
            if(mode<0 || mode>3)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_loopback_send(pState, (LMS6002_LOOPBACK_MODE)mode);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
             
        int radio420_rx_impl::set_calibration_dc_offset()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            lms6002_rx_dc_calib calib;         
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_rx_dc_offset_calibration_send(pState, &calib);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_rx_dc_offset_calibration_set_send(pState, &calib);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }


        theEnd:
            carrier->unlock();
            return res;
        }
        
        int radio420_rx_impl::set_spi_control(int control, int spi_controller)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;                    
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << "radio420_rx: target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the control
            if(control<0 || control>4)
                return GNURADIO_INVVALUE;

            // Verify the validity of the spi_controller
            if(spi_controller<0 || spi_controller>1)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            res = fmc_radio_spi_control_set_send(pState, (FMCRADIO_FPGA_CTRL_LIST)control, (FMCRADIO_SPI_CTRL)spi_controller);
            if(FAILURE(res))
            {
                std::cout << ErrorToString(res) << std::endl;
                goto theEnd;
            }

            
        theEnd:
            carrier->unlock();
            return res;
        }
        

        bool radio420_rx_impl::start()
        {
            Result res = 0;
            nutaq_api * api = nutaq_api::get_instance();

            res = api->try_init();
            if (res < 0)
            {
                return false;
            }
            
            res = api->wait_for_init();
            if (res < 0)
            {
                return false;
            }

            std::cout << "radio420_rx: started" << std::endl;

            return true;
        }

        bool radio420_rx_impl::stop()
        {
            std::cout << "radio420_rx: stopped" << std::endl;

            return true;
        }

        int radio420_rx_impl::work(int noutput_items,
                                   gr_vector_const_void_star &input_items,
                                   gr_vector_void_star &output_items)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            int gain;
            long long freq;
            double delay;
            lms6002_pll_param lime_pll_param;
            unsigned int uRefFreq;
            unsigned int uLimeFreq;
            LMS6002_VGA1_GAIN tmp_gain;
            Result res;

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

            if (d_rev == FMCRADIOREVSDR_B || d_rev == FMCRADIOREVSDR_A)
            {
                uRefFreq = FMC_RADIO_SDR_REVB_REFERENCE_CLOCK;
                uLimeFreq = FMC_RADIO_SDR_REVB_LIME_CLOCK;
            }
            else
            {
                uRefFreq = FMC_RADIO_SDR_REVC_REFERENCE_CLOCK;
                uLimeFreq = FMC_RADIO_SDR_REVC_LIME_CLOCK;
            }

            //std::cout << "radio420_rx: work " << noutput_items << " " << input_items.size() << " " << output_items.size() << std::endl;

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

            switch(d_rx_vga1_gain)
            {
            default:
            case 1:
                tmp_gain = LMS_VGA1_GAIN5DB;
                break;
            case 2:
                tmp_gain = LMS_VGA1_GAIN19DB;
                break;
            case 3:
                tmp_gain = LMS_VGA1_GAIN30DB;
                break;
            }


            carrier->lock();
            
            res = fmc_radio_select_send(pState, d_selected_card);
            if(FAILURE(res))
            {
                printf("Error 0x%x: Cannot select card\r\n", res);
				printf("%s\n",ErrorToString(res));
				exit(-1);
            }
            
            if(input_items.size() >= 1 )
            {
                gain = lroundf(((float*)input_items[0])[0]);
                //saturate
                if(gain < -13) gain = -13;
                if(gain > 18) gain = 18;
                if(d_rx_gain3 != gain)
                {
                    res = fmc_radio_sdr_rx_gain_send(pState, (LMS6002_LNA_GAIN)d_rx_lna_gain, tmp_gain, d_rx_gain2, gain);
                    if(FAILURE(res))
                    {
                        printf("Error 0x%x: Cannot set gain\r\n", res);
						printf("%s\n",ErrorToString(res));
						exit(-1);
                    }
                    d_rx_gain3 = gain;
                }
            }

            //Both input are connected
            if(input_items.size() >= 2 )
            {
                freq = llroundf(((float*)input_items[1])[0]);
                //saturate
                if(freq < 300e6) freq = 300e6;
                if(freq > 3800e6) freq = 3800e6;
                if(d_rx_freq != freq)
                {
                    res = fmc_radio_lime_calculate(&lime_pll_param, uLimeFreq, freq);
                    if(FAILURE(res))
                    {
                        printf("Error 0x%x: Error calculating PLL settings\r\n",res);
						printf("%s\n",ErrorToString(res));
						throw std::runtime_error("End of radio420_rx work function");
                    }
                    res = fmc_radio_lime_setpllparam_send(pState, LMS6002_RX, &lime_pll_param);
                    if(FAILURE(res))
                    {
                        printf("Error 0x%x: Error configuring PLL\r\n",res);
						printf("%s\n",ErrorToString(res));
						exit(-1);
                    }

                    res = fmc_radio_lime_calibratePLL_send(pState, LMS6002_RX);
                    if(FAILURE(res))
                    {
                        printf("Error 0x%x: Could not calibrate lime rx pll\r\n", res);
                        printf("%s\n",ErrorToString(res));
                    }

                    d_rx_freq = freq;
                }
            }

            carrier->unlock();

            // Tell runtime system how many output items we produced.
            return noutput_items;
        }

    } /* namespace nutaq */
} /* namespace gr */

