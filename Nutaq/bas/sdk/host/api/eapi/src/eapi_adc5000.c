/****************************************************************************  
 *
 *    ****                              *                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    *****  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      eapi_adc5000.c
 * 
 * @brief     Function definitions for the eapi adc5000 control
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * This file is part of Nutaq’s BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq’s BAS Software Suite under 
 * either the terms of the GNU General Public License (GPL) or the 
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s BAS Software Suite may be used under the terms of the GNU General 
 * Public License version 3 as published by the Free Software Foundation 
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF 
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A 
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s BAS Software Suite under the GPL requires that your work based on 
 * Nutaq’s BAS Software Suite, if  distributed, must be licensed under the GPL. 
 * If you wish to distribute a work based on Nutaq’s BAS Software Suite but desire 
 * to license it under your own terms, e.g. a closed source license, you may purchase 
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s BAS Software Suite. 
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
#include "cce.h"
#include "eapi_adc5000.h"
#include "eapi_serialize.h"
#include "adc5000_serialize.h"
#include "ev10aq190_defines.h"
#include "ad9517.h"


Result ADC5000_Presence_send(connection_state * state)
{
    Result ret;
	cmd_send(state, ADC5000_PRESENCE);
	ret = result_recv(state);
    return ret;
}

Result ADC5000_Powerup_send(connection_state * state)
{
    Result ret;
	cmd_send(state, ADC5000_POWERUP);
	ret = result_recv(state);
    return ret;
}

Result ADC5000_Reset_send(connection_state * state)
{
    Result ret;
	cmd_send(state, ADC5000_RESET);
	ret = result_recv(state);
    return ret;
}

Result ADC5000_SetMode_send(connection_state * state, ev10aq190_mode mode)
{
    Result ret;
	cmd_send(state, ADC5000_SET_MODE);
    uint_send(state, mode);
	ret = result_recv(state);
    return ret;
}

static Result ADC5000_SetPLLParam_send(connection_state * state, ad9517_param* pllparameters)
{
    Result ret; 
	cmd_send(state, ADC5000_SET_PLL_PARAM);
	adc5000_pll_param_send(state, pllparameters);
	ret = result_recv(state);
    return ret;
}

Result ADC5000_PLLConfigure_send(connection_state * state, unsigned int ref_freq, unsigned int adc_freq, unsigned int clock_output_freq, 
									unsigned int fmc_freq, unsigned int sync_output_freq, adc5000_clock_source clock_source)
{
	Result ret; 
	ad9517_param params;
    params.ref_freq = ref_freq;
    params.pfd_freq_min = AD9517_PFD_MIN_FREQ;
    params.pfd_freq_max = AD9517_PFD_MAX_FREQ;
    params.vco_freq_max = ADC5000_VCO_FREQ_MAX;
    params.vco_freq_min = ADC5000_VCO_FREQ_MIN;
    params.out_freq[0] = clock_output_freq;
    params.out_freq[1] = adc_freq;
    params.out_freq[2] = sync_output_freq;
    params.out_freq[3] = fmc_freq;
    switch(clock_source)
    {
		case ADC5000_CLOCK_EXTERNAL:
			params.external_clock = 2;
            params.vco_freq_max = ref_freq;
            params.vco_freq_min = ref_freq;
        break;
		case ADC5000_CLOCK_EXTERNAL_REF:
			params.external_clock = 1;
        break;
		default:
		case ADC5000_CLOCK_INTERNAL_REF:
			params.external_clock = 0;
        break;
    }		
	ret = ad9517_calculate(&params);
	if(ret)
	{
		return ret;
	}
	ret = ADC5000_SetPLLParam_send(state, &params);
    return ret;
}
 
Result ADC5000_SetSync_send(connection_state * state, adc5000_sync_source sync_source)
{
    Result ret; 
	cmd_send(state, ADC5000_SET_SYNC);
    uint_send(state, sync_source);
	ret = result_recv(state);
    return ret;
}

Result ADC5000_PLLLock_send(connection_state * state, ad9517_lock_status * lockstatus)
{
    Result ret; 
	cmd_send(state, ADC5000_PLL_LOCK);
	*lockstatus = (ad9517_lock_status) (uint_recv(state)); 
	ret = result_recv(state);
    return ret;
}

/*
Result ADC5000_ReadTemperature_send(connection_state * state, adc5000_sensor sensor, float * temp)
{
  Result ret; 
  unsigned int pVal[1];
  float* pFloat = (float*)pVal;
  
  cmd_send(state, ADC5000_READ_TEMPERATURE);
  uint_send(state, sensor);
	pVal[0] = uint_recv(state); 
  *temp = pFloat[0];
	ret = result_recv(state);
  return ret;
}
*/

Result ADC5000_SetTestMode_send(connection_state * state, ev10aq190_testmode testmode)
{
  Result ret; 
	cmd_send(state, ADC5000_SET_TEST_MODE);
  uint_send(state, testmode);
	ret = result_recv(state);
  return ret;
}

Result ADC5000_CalibrateIoDelay_send(connection_state * state)
{
  Result ret;
	cmd_send(state, ADC5000_CALIBRATE_IODELAY);
	ret = result_recv(state);
  return ret;
}

Result ADC5000_GetAdcStatus_send(connection_state * state, unsigned char * adc_A_active, unsigned char * adc_B_active, unsigned char * adc_C_active, unsigned char * adc_D_active)
{
  unsigned int val;
  Result ret;
	cmd_send(state, ADC5000_GET_ADC_STATUS);
  
	val = uint_recv(state); 
  if(adc_A_active)
    *adc_A_active = (val >> 0) & 0xFF;
  if(adc_B_active)
    *adc_B_active = (val >> 8) & 0xFF;
  if(adc_C_active)
    *adc_C_active = (val >> 16) & 0xFF;
  if(adc_D_active)
    *adc_D_active = (val >> 24) & 0xFF;
    
  ret = result_recv(state);
  return ret;
}

Result ADC5000_SetStandby_send(connection_state * state, ev10aq190_standby standby)
{
  Result ret;
	cmd_send(state, ADC5000_SET_STANDBY);
  uint_send(state, standby);
	ret = result_recv(state);
  return ret;
}

Result ADC5000_SetCoding_send(connection_state * state, ev10aq190_coding coding)
{
  Result ret;
	cmd_send(state, ADC5000_SET_CODING);
  uint_send(state, coding);
	ret = result_recv(state);
  return ret;
}

Result ADC5000_SetBandwidth_send(connection_state * state, ev10aq190_bandwidth bandwidth)
{
  Result ret;
	cmd_send(state, ADC5000_SET_BANDWIDTH);
  uint_send(state, bandwidth);
	ret = result_recv(state);
  return ret;
}

Result ADC5000_SetAdjTriggerDelay_send(connection_state * state, char delay)
{
  Result ret;
	cmd_send(state, ADC5000_SET_ADJ_TRIGGER_DELAY);
  uint_send(state, delay);
	ret = result_recv(state);
  return ret;
}

Result ADC5000_SetOffset_send(connection_state * state, ev10aq190_channel channel, unsigned short offset)
{
  Result ret;
	cmd_send(state, ADC5000_SET_OFFSET);
  uint_send(state, (channel << 16) | offset);
	ret = result_recv(state);
  return ret;
}
Result ADC5000_SetGain_send(connection_state * state, ev10aq190_channel channel, unsigned short gain)
{
  Result ret;
	cmd_send(state, ADC5000_SET_GAIN);
  uint_send(state, (channel << 16) | gain);
	ret = result_recv(state);
  return ret;
}
Result ADC5000_SetPhase_send(connection_state * state, ev10aq190_channel channel, unsigned short phase)
{
  Result ret;
	cmd_send(state, ADC5000_SET_PHASE);
  uint_send(state, (channel << 16) | phase);
	ret = result_recv(state);
  return ret;
}





