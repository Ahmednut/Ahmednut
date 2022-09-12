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
 * @file      eapi_mi250.c
 * 
 * @brief     Function definitions for the eapi MI250 control
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
#include "mi250_types.h"
#include "mi250_pll.h"
#include "eapi_mi250.h"
#include "eapi_serialize.h"
#include "mi250_serialize.h"
#include "mi250_types.h"

Result MI250_Select_send(connection_state * state, unsigned uConnectorNum)
{
    Result ret;
    cmd_send(state, MI250_SELECT);
	uint_send(state, uConnectorNum-1);
    ret = result_recv(state);
    return ret;
}

Result MI250_Init_send(connection_state * state)
{
    Result ret;
    cmd_send(state, MI250_INIT);
    ret = result_recv(state);
    return ret;
}

Result MI250_Powerup_send(connection_state * state)
{
    Result ret;
    cmd_send(state, MI250_POWERUP);
    ret = result_recv(state);
    return ret;
}

Result MI250_Presence_send(connection_state * state)
{
    Result ret;
    cmd_send(state, MI250_PRESENCE);
    ret = result_recv(state);
    return ret;
}

Result MI250_ReadSPIRegisters_send(connection_state * state,  MI250_DEVICES DeviceId, int BaseRegister, unsigned char * value)
{
    cmd_send(state, MI250_READSPI);
    uint_send(state, DeviceId);
    uint_send(state, BaseRegister);
    connection_flush(state);
    *value = uchar_recv(state);
    return result_recv(state);
}

Result MI250_WriteSPIRegisters_send(connection_state * state,  MI250_DEVICES DeviceId, int BaseRegister, unsigned char value)
{
    cmd_send(state, MI250_WRITESPI);
    uint_send(state, DeviceId);
    uint_send(state, BaseRegister);
    uchar_send(state, value);
    return result_recv(state);
}

Result MI250_PLLConfig_send(connection_state * state, unsigned int reffreq, unsigned int adcfreq, MI250_CLOCK_SOURCE clocksource)
{
	Result ret;
	MI250_PLL_PARAM param;
	switch(clocksource)
	{
		case MI250_CLOCKIN_100MHZ:
		  ret = MI250_PLL_CalculateParams(reffreq, adcfreq, MI250_PLL_PHASE_NOISE, &param);
		  if(ret)
		  {
			  return ret;
		  }
		  break;
		case MI250_CLOCKIN_EXTERNALREF:
		  ret = MI250_PLL_CalculateParams(reffreq, adcfreq, MI250_PLL_PHASE_NOISE, &param);
		  if(ret)
		  {
			  return ret;
		  }
		  break;
		case MI250_CLOCKIN_EXTERNALCLOCK:
			param.Afactor = 4;
			param.Bfactor = 0;
			param.divisorADCClk = 1;
			param.divisorClkOutClk = 0;
			param.divisorRsvdClk = 0;
			param.Rfactor = 0;
			param.Pfactor= 0;
			param.VCOFreq = 4;
		  break;
		default:
		  return MI250INVALIDCLOCKSOURCE;
	}
	cmd_send(state, MI250_PLLCONFIG);
	uint_send(state, clocksource);
	uint_send(state, adcfreq);
	MI250_pll_param_send(state, &param);
	ret = result_recv(state);
	return ret;
}

Result MI250_PLLGetStatus_send(connection_state * state, MI250_PLL_STATUS * locked)
{
    Result ret;
    cmd_send(state, MI250_GETPLLSTATUS);
    *locked = (MI250_PLL_STATUS)uint_recv(state);
    ret = result_recv(state);
    return ret;
}


/*Result MI250_SetConfig_send(connection_state * state, unsigned freq, MI250_DEVICES device, MI250_ADC_DATAFORMAT format, MI250_ADC_SLEEP_MODE mode, MI250_CHANNEL_OCT octa, MI250_CHANNEL_OCT octb)
{
    Result ret;
    cmd_send(state, MI250_SETADCCONFIG);
    uint_send(state, freq);
    uint_send(state, device);
    uint_send(state, format);
    uint_send(state, mode);
    uint_send(state, octa);
    uint_send(state, octb);
    ret = result_recv(state);
    return ret;
}*/

Result MI250_SetGain_send(connection_state * state, MI250_ADC_CHANNEL channel, MI250_DEVICES device, MI250_ADC_GAIN gain)
{
    Result ret;
    cmd_send(state, MI250_SETADCGAIN);
    uint_send(state, channel);
	uint_send(state, device);
    uint_send(state, gain);
    ret = result_recv(state);
    return ret;
}

Result MI250_SetFineGain_send(connection_state * state, MI250_ADC_CHANNEL channel, MI250_DEVICES device, char gain)
{
    Result ret;
    cmd_send(state, MI250_SETADCFINEGAIN);
    uint_send(state, channel);
	uint_send(state, device);
    uchar_send(state, gain);
    ret = result_recv(state);
    return ret;
}

Result MI250_SetPedestal_send(connection_state * state, MI250_ADC_CHANNEL channel,  MI250_DEVICES device, char value)
{
    Result ret;
    cmd_send(state, MI250_SETADCPEDESTAL);
    uint_send(state, channel);
	uint_send(state, device);
    uchar_send(state, value);
    ret = result_recv(state);
    return ret;
}


Result MI250_ADCreset_send(connection_state * state)
{
    Result ret;
    cmd_send(state, MI250_ADCRESET);
    ret = result_recv(state);
    return ret;
}

Result MI250_ADCarm_send(connection_state * state, MI250_ADC_ARMED_STATUS armedstatus)
{
    Result ret;
    cmd_send(state, MI250_ARMADC);
    uint_send(state, armedstatus);
    ret = result_recv(state);
    return ret;
}

Result MI250_SetDataFormat_send(connection_state * state, MI250_DEVICES device, MI250_ADC_DATAFORMAT format)
{
    Result ret;
    cmd_send(state, MI250_SETDATAFORMAT);
    uint_send(state, device);
    uint_send(state, format);
    ret = result_recv(state);
    return ret;
}

Result MI250_SetSleepMode_send(connection_state * state, MI250_DEVICES device, MI250_ADC_SLEEP_MODE mode)
{
    Result ret;
    cmd_send(state, MI250_SETSLEEPMODE);
    uint_send(state, device);
    uint_send(state, mode);
    ret = result_recv(state);
    return ret;
}


Result MI250_SetOffsetCorrectionTime_send(connection_state * state, MI250_DEVICES device, MI250_CHANNEL_OCT octa, MI250_CHANNEL_OCT octb)
{
    Result ret;
    cmd_send(state, MI250_SETOCT);
    uint_send(state, device);
    uint_send(state, octa);
    uint_send(state, octb);
    ret = result_recv(state);
    return ret;
}




