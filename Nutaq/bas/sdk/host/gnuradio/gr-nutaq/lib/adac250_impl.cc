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
#include "adac250_impl.h"
#include "nutaq_api.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <math.h>

#include <bas_error.h>
#include <adac250_user.h>

namespace gr {
    namespace nutaq {

        adac250::sptr
        adac250::make(std::string target_id, int block_priority)
        {
            return gnuradio::get_initial_sptr
                (new adac250_impl(target_id, block_priority));
        }

        /*
         * The private constructor
         */
        adac250_impl::adac250_impl(std::string target_id, int block_priority)
            : gr::block("adac250",
                        gr::io_signature::make(0, 0, sizeof(int)),
                        gr::io_signature::make(0, 0, sizeof(int))),
              nutaq_cb(block_priority),
              carrier_client(target_id)
        {       
            d_ref_clk_source = ADAC250_CLOCKIN_10MHZREF;
            d_ref_clk_freq = 10e6;
            d_adc_clk_freq = 250e6;
            d_dac_clk_freq = 250e6;
            d_dac_interp = ADAC250_DAC_INTERPOLATION_4X;
            d_adc_gain[0] = ADAC250_ADC_0_0dB;
            d_adc_gain[1] = ADAC250_ADC_0_0dB;
            d_adc_fine_gain[0] = 0;
            d_adc_fine_gain[1] = 0;
            d_dac_gain[0] = 0;
            d_dac_gain[1] = 0;
            d_ref_clk_tuning = 32768;
        }

        /*
         * Our virtual destructor.
         */
        adac250_impl::~adac250_impl()
        {
        }

        void adac250_impl::set_default_ref_clk_source(int source)
        {
            d_ref_clk_source = (ADAC250_CLOCK_SOURCES)source;
        }

        void adac250_impl::set_default_ref_clk_freq(double freq)
        {
            d_ref_clk_freq = (unsigned int)round(freq);
        }

        void adac250_impl::set_default_adc_clk_freq(double freq)
        {
            d_adc_clk_freq = (unsigned int)round(freq);
        }

        void adac250_impl::set_default_dac_clk_freq(double freq)
        {
            d_dac_clk_freq = (unsigned int)round(freq);
        }

        void adac250_impl::set_default_dac_interpolation(int interp)
        {
            d_dac_interp = (ADAC250_DAC_INTERPOLATION)interp;
        }

        void adac250_impl::set_default_adc_gain(int channel, double gain)
        {
            //Verify that the channel is valid
            if (channel < 0 || channel >= ADAC250_CHANNEL_NB)
                return;

            // Convert global gain in dB in both of the ADC gain register values
            calculate_adc_gains(gain, &d_adc_gain[channel], &d_adc_fine_gain[channel]);
            
            //printf("Gain %f dB : coarse %f, fine %d\n",gain, ((float)d_adc_gain[channel])/2.0f, d_adc_fine_gain[channel]);

            return;
        }

        void adac250_impl::set_default_dac_gain(int channel, double gain)
        {
            //Verify that the channel is valid
            if (channel < 0 || channel >= ADAC250_CHANNEL_NB)
                return;

            calculate_dac_gain(gain, &d_dac_gain[channel]);
        }

        void adac250_impl::set_default_ref_clk_tuning(int value)
        {
            d_ref_clk_tuning = std::min(std::max(value,0),65535);
        }

        int adac250_impl::read_spi_register(int device, int addr)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            unsigned char value;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the device ID
            if(device<0 || device>3)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_ReadSPIRegisters_send(pState, (ADAC250_DEVICES)device, addr, &value);
            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return (int)value;
        }

        int adac250_impl::write_spi_register(int device, int addr, int value)
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

            // Verify the validity of the device ID
            if(device<0 || device>3)
                return GNURADIO_INVVALUE;

            // Verify the validity of the value, must be a unsigned char value
            if(value<0 || value>=256)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_WriteSPIRegisters_send(pState, (ADAC250_DEVICES)device, addr, (unsigned char)value);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return (int)value;
        }

        int adac250_impl::set_dac_gain(int channel, double gain)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            int gain_int;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the channel
            if(channel<0 || channel>1)
                return GNURADIO_INVVALUE;

            calculate_dac_gain(gain, &gain_int);

            carrier->lock();
            

            res = ADAC250_DAC_SetGain_send(pState, (ADAC250_DAC_CHANNEL)channel, gain_int);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::init_dac(int sleep, int interp, int mixmode1, int mixmode2)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            int gain_int;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the sleep
            if(sleep<0 || sleep>3)
                return GNURADIO_INVVALUE;
            // Verify the validity of the interp
            if((ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_NONE ||
                (ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_2X ||
                (ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_4X)
                return GNURADIO_INVVALUE;
            // Verify the validity of the mixmode1 and mixmode2
            if(mixmode1<0 || mixmode1>3 || mixmode2<0 || mixmode2>3 )
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_DAC_Init_send(pState, (ADAC250_DAC_SLEEP)sleep, (ADAC250_DAC_INTERPOLATION)interp, (ADAC250_DAC_CMIX_MODE)mixmode1, (ADAC250_DAC_CMIX_MODE)mixmode2);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_dac_sync(int sync)
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

            // Verify the validity of the sync
            if(sync<0 || sync>1)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_DAC_Sync_send(pState, (ADAC250_DAC_SYNC)sync);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_dac_sync_source(int sync_source)
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

            // Verify the validity of the sync_source
            if(sync_source<0 || sync_source>1)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_DAC_SyncSource_send(pState, (ADAC250_DAC_SYNC_SOURCE)sync_source);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::calibrate_dac()
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
            

            res = ADAC250_DAC_Calibrate_send(pState);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::init_pll(int refclk, int adcclk, int dacclk, int interp, int clksrc)
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
            

            // Verify the validity of the interp
            if((ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_NONE ||
                (ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_2X ||
                (ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_4X)
                return GNURADIO_INVVALUE;
            // Verify the validity of the clksrc
            if(clksrc<0 || clksrc>2)
                return GNURADIO_INVVALUE;

            res = ADAC250_PLL_Init_send(pState, (int)refclk, (int)adcclk, (int)dacclk, (ADAC250_DAC_INTERPOLATION)interp, (ADAC250_CLOCK_SOURCES)clksrc);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::get_pll_status()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            int locked = 0;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = ADAC250_PLL_getStatus_send(pState, &locked);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return locked;
        }

        int adac250_impl::tune_ref_clk(int value)
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
            

            res = ADAC250_PLL_setRefClkTuning_send(pState, (unsigned short)std::min(std::max(value,0),65535));

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_pll_config(int refclk, int adcclk, int dacclk, int interp)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            ADAC250_PLL_PARAM param;            
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();

            // Verify the validity of the interp
            if((ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_NONE ||
                (ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_2X ||
                (ADAC250_DAC_INTERPOLATION)interp != ADAC250_DAC_INTERPOLATION_4X)
                return GNURADIO_INVVALUE;

            res = ADAC250_PLL_CalculateParams(refclk, adcclk, dacclk, (ADAC250_DAC_INTERPOLATION)interp, &param);
            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            carrier->lock();
            

            res = ADAC250_PLL_SetConfig_send(pState, param);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::init_adc()
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
            

            res = ADAC250_ADC_init_send(pState);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_adc_gain(int channel, double gain)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            ADAC250_ADC_GAIN coarse_gain;
            int fine_gain;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();
            
            //printf("channel : %d\n",channel);
            
            // Verify the validity of the channel
            if(channel<0 || channel>1)
                return GNURADIO_INVVALUE;

            calculate_adc_gains(gain, &coarse_gain, &fine_gain);
            
            //printf("Gain %f dB : coarse %f, fine %d\n",gain, ((float)coarse_gain)/2.0f, fine_gain);

            carrier->lock();
            

            res = ADAC250_ADC_setGain_send(pState, (ADAC250_ADC_CHANNEL)channel, coarse_gain);
            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                carrier->unlock();
                return res;
            }

            res = ADAC250_ADC_setFineGain_send(pState, (ADAC250_ADC_CHANNEL)channel, fine_gain);
            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                carrier->unlock();
                return res;
            }

            carrier->unlock();
            return OK;
        }

        int adac250_impl::set_adc_pedestal(int channel, int pedestal)
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
            
            // Verify the validity of the channel
            if(channel<0 || channel>1)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_ADC_setPedestal_send(pState, (ADAC250_ADC_CHANNEL)channel, std::min(std::max(pedestal,-32),31));

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::get_adc_status(int channel)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            ADAC250_ADC_STATUSES status;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();
            
            // Verify the validity of the channel
            if(channel<0 || channel>1)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_ADC_getStatus_send(pState, (ADAC250_ADC_CHANNEL)channel, &status);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return (int)status;
        }

        int adac250_impl::calibrate_adc()
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
            

            res = ADAC250_ADC_Calibrate_send(pState);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_adc_data_format(int format)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            ADAC250_ADC_DATAFORMAT format_enum;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();
                
            // Convert int to valid data format enum
            if(format == 0)
                format_enum = ADAC250_ADC_TWOCMP;
            else if(format == 1)
                format_enum = ADAC250_ADC_OFFSET;
            else
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_ADC_SetDataFormat_send(pState, format_enum);

            carrier->unlock();
            
            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_adc_sleep_mode(int mode)
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res;
            ADAC250_ADC_SLEEP_MODE sleep_enum;
            
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }
            
            pState = carrier->get_connectionstate();
                
            // Convert int to valid data format enum
            if(mode == 0)
                sleep_enum = ADAC250_ADC_NORMAL;
            else if(mode == 1)
                sleep_enum = ADAC250_ADC_OUTB_DISABLED;
            else if(mode == 2)
                sleep_enum = ADAC250_ADC_OUTA_DISABLED;
            else if(mode == 3)
                sleep_enum = ADAC250_ADC_OUTAB_DISABLED;
            else if(mode == 4)
                sleep_enum = ADAC250_ADC_GLOBALPOWERDN;
            else if(mode == 5)
                sleep_enum = ADAC250_ADC_CHB_STANDBY;
            else if(mode == 6)
                sleep_enum = ADAC250_ADC_CHA_STANDBY;
            else
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_ADC_SetSleepMode_send(pState, sleep_enum);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_adc_trigger_delay(int triggerdelay)
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
            

            res = ADAC250_ADC_SetTriggerDelay_send(pState, std::min(std::max(triggerdelay,0),31));

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_clk_mux(int clock_to_config, int clock_input_selection)
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
            
            // Verify the validity of the clock_to_config
            if(clock_to_config<0 || clock_to_config>3)
                return GNURADIO_INVVALUE;
            // Verify the validity of the clock_input_selection
            if(clock_input_selection<0 || clock_input_selection>3)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_MUX_ConfigClockOutput_send(pState, (ADAC250_CLOCK_OUTPUT_ID)clock_to_config, (ADAC250_CLOCK_INPUT_ID)clock_input_selection);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::set_adc_offset_correction_time(int time_a, int time_b)
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
            
            // Verify the validity of the time_a
            if(time_a<0 || time_a>15)
                return GNURADIO_INVVALUE;
            // Verify the validity of the time_b
            if(time_b<0 || time_b>15)
                return GNURADIO_INVVALUE;

            carrier->lock();
            

            res = ADAC250_ADC_SetOffsetCorrectionTime_send(pState, (ADAC250_CHANNEL_OCT)time_a, (ADAC250_CHANNEL_OCT)time_b);

            carrier->unlock();

            if(FAILURE(res))
            {
                printf("%s\n",ErrorToString(res));
                return res;
            }

            return OK;
        }

        int adac250_impl::callback()
        {
            nutaq_api * api = nutaq_api::get_instance();
            
            carrier * carrier;
            connection_state * pState;
            Result res = OK;

            // ADAC250 is only supported on the Perseus board
            if (api->get_carrier_byid(get_target_id())->get_class() != CARRIER_PERSEUS)
            {
                std::cout << name() << ": Error. The carrier board must be a Perseus" << std::endl;
                return -1;
            }

            // Get the carrier object for the associated ID
            carrier = api->get_carrier_byid(get_target_id());
            if(carrier == NULL)
            {
                std::cout << name() << ": target id \"" << get_target_id() << "\" does not exist. QUITTING" << std::endl;
                return GNURADIO_CARRIER_NOT_INIT;
            }

            // Get the TCP connection with the CCE of the carrier board
            pState = carrier->get_connectionstate();

            carrier->lock();
            

            res = ADAC250_Presence_send(pState );
            if( res != 0 )
            {
                printf("Error: Could not detect ADAC250\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

            res = ADAC250_PowerUp_send(pState );
            if( res != 0 )
            {
                printf("Error: Could not power up ADAC250\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
           
            //Init PLL
            res = ADAC250_PLL_Init_send(pState, d_ref_clk_freq, d_adc_clk_freq, d_dac_clk_freq, d_dac_interp, d_ref_clk_source);
            if( res != 0 )
            {
                printf("Error: Could not initialize PLL \r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

            res = ADAC250_MUX_ConfigClockOutput_send(pState, ADAC250_CLOCKOUT_REFOUT, ADAC250_CLOCKIN_PLLCLKOUT2);
            if( res != 0 )
            {
                printf("Error: Could not set clock output \r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            
            //Init ADC
            res = ADAC250_ADC_init_send(pState );
            if( res != 0 )
            {
                printf("Error: Could not initialize ADC\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

            res = ADAC250_ADC_Calibrate_send(pState ); 
            if( res != 0 )
            {
                printf("Error: Could not calibrate ADC\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
          
            //Init DAC
            res = ADAC250_DAC_Init_send(pState, ADAC250_DAC_SLEEP_NONE, d_dac_interp, ADAC250_DAC_CMIX_LP, ADAC250_DAC_CMIX_LP );
            if( res != 0 )
            {
                printf("Error: Could not initialize DAC\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

            res = ADAC250_DAC_SyncSource_send(pState, ADAC250_DAC_SYNC_SOURCE_DACREG);
            if( res != 0 )
            {
                printf("Error: Could not sync DAC\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

            res = ADAC250_DAC_Sync_send(pState, ADAC250_DAC_SYNC_ON);
            if( res != 0 )
            {
                printf("Error: Could not sync DAC\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

            res = ADAC250_DAC_Calibrate_send(pState );
            if( res != 0 )
            {
                printf("Error: Could not calibrate DAC\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

            //Set gains
            res = ADAC250_ADC_setGain_send(pState, ADAC250_ADC_CHANNEL_A, d_adc_gain[0]);
            if( res != 0 )
            {
                printf("Error: Could not set ADC gain\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            res = ADAC250_ADC_setGain_send(pState, ADAC250_ADC_CHANNEL_B, d_adc_gain[1]);
            if( res != 0 )
            {
                printf("Error: Could not set ADC gain\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            res = ADAC250_ADC_setFineGain_send(pState, ADAC250_ADC_CHANNEL_A, d_adc_fine_gain[0]);
            if( res != 0 )
            {
                printf("Error: Could not set ADC fine gain\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            res = ADAC250_ADC_setFineGain_send(pState, ADAC250_ADC_CHANNEL_B, d_adc_fine_gain[1]);
            if( res != 0 )
            {
                printf("Error: Could not set ADC fine gain\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            
            res = ADAC250_DAC_SetGain_send(pState, ADAC250_DAC_CHANNEL_A, d_dac_gain[0]);
            if( res != 0 )
            {
                printf("Error: Could not set DAC gain\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            res = ADAC250_DAC_SetGain_send(pState, ADAC250_DAC_CHANNEL_B, d_dac_gain[1]);
            if( res != 0 )
            {
                printf("Error: Could not set DAC gain\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }
            
            //Set default value to the DAC used for the reference oscillator frequency tuning
            res = ADAC250_PLL_setRefClkTuning_send(pState, d_ref_clk_tuning);
            if( res != 0 )
            {
                printf("Error: Could not tune the reference clock\r\n");
                printf("%s\n",ErrorToString(res));
                goto theEnd;
            }

        theEnd:
            carrier->unlock();
            return res;
        }

        void adac250_impl::calculate_adc_gains(double gain, ADAC250_ADC_GAIN * coarse_gain, int * fine_gain)
        {
            const double max_total_gain = 6.134;
            const double min_total_gain = 0.0;
            const double max_fine_gain = 0.134;
            const double min_fine_gain = 0.0;

            const double gain_step = 0.5;

            const int max_fine_gain_index = 127;
            const int min_fine_gain_index = 0;

            double remainder;

            // Verify is gain exceed limits
            // if so, apply max or min values
            if(gain>=max_total_gain)
            {
                *coarse_gain = ADAC250_ADC_6_0dB;
                *fine_gain = max_fine_gain_index;
                return;
            }
            else if(gain<=min_total_gain)
            {
                *coarse_gain = ADAC250_ADC_0_0dB;
                *fine_gain = min_fine_gain_index;
                return;
            }

            remainder = fmod(gain, gain_step);
            
            //printf("1st: %f, 2nd: %f\n", std::abs(remainder-max_fine_gain), std::abs(gain_step-remainder));

            // Use fine gain
            if(remainder <= max_fine_gain)
            {
                // floor gain to the closest available value
                *coarse_gain = (ADAC250_ADC_GAIN)round((gain-remainder)/gain_step);
                // Calculate fine gain value
                // gain(dB) = 20*log10[1 + (fine_gain/8192)]
                // from ADC62P49 datasheet
                *fine_gain = (int)round((pow(10, remainder/20.0)-1) * 8192);
            }
            // Closest to lower available gain
            else if(std::abs(remainder-max_fine_gain) < std::abs(gain_step-remainder))
            {
                // floor gain to the closest available value
                *coarse_gain = (ADAC250_ADC_GAIN)round((gain-remainder)/gain_step);
                // set max fine gain
                *fine_gain = max_fine_gain_index;
            }
            // Closest to the higher available gain
            else
            {
                // ceil gain to the closest available value
                *coarse_gain = (ADAC250_ADC_GAIN)round((gain-remainder+gain_step)/gain_step);
                // set min fine gain
                *fine_gain = min_fine_gain_index;
            }

            return;
        }

        void adac250_impl::calculate_dac_gain(double gain, int * dac_gain)
        {
            // from DAC5682Z datasheet
            *dac_gain = (int)round(pow(10, gain/20.0)-1);
        }

    } /* namespace nutaq */
} /* namespace gr */

