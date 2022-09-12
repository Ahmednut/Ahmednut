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
 * @file      eapi_adac250.c
 * 
 * @brief     Function definitions for the eapi adac250 control
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
#include "string.h"
#include "eapi_adac250.h"
#include "eapi_serialize.h"
#include "adac250_serialize.h"
#include "adac250_user.h"

Result ADAC250_Presence_send(connection_state * state)
{
    Result ret;
    cmd_send(state, ADAC250_PRESENCE);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PowerUp_send(connection_state * state)
{
    Result ret;
    cmd_send(state, ADAC250_POWERUP);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ReadSPIRegisters_send(connection_state * state, ADAC250_DEVICES DeviceId, int BaseRegister, unsigned char * value)
{
    cmd_send(state, ADAC250_READSPI);
    uint_send(state, DeviceId);
    uint_send(state, BaseRegister);
    connection_flush(state);
    *value = uchar_recv(state);
    return result_recv(state);
}

Result ADAC250_WriteSPIRegisters_send(connection_state * state, ADAC250_DEVICES DeviceId, int BaseRegister, unsigned char value)
{
    cmd_send(state, ADAC250_WRITESPI);
    uint_send(state, DeviceId);
    uint_send(state, BaseRegister);
    uchar_send(state, value);
    return result_recv(state);
}

Result ADAC250_DAC_SetGain_send(connection_state * state, ADAC250_DAC_CHANNEL channel, int gain)
{
    Result ret;
    cmd_send(state, ADAC250_DACSETGAIN);
    uint_send(state, channel);
    uint_send(state, gain);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_DAC_SetOffset_send(connection_state * state, ADAC250_DAC_CHANNEL channel, int offset)
{
    Result ret;
    cmd_send(state, ADAC250_DACSETOFFSET);
    uint_send(state, channel);
    uint_send(state, offset);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_DAC_Init_send(connection_state * state, ADAC250_DAC_SLEEP sleep, ADAC250_DAC_INTERPOLATION interpolation, ADAC250_DAC_CMIX_MODE mixmode1, ADAC250_DAC_CMIX_MODE mixmode2)
{
    Result ret;
    cmd_send(state, ADAC250_DACINIT);
    uint_send(state, sleep);
    uint_send(state, interpolation);
    uint_send(state, mixmode1);
    uint_send(state, mixmode2);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_DAC_Sync_send(connection_state * state, ADAC250_DAC_SYNC sync)
{
    Result ret;
    cmd_send(state, ADAC250_DACSYNC);
    uint_send(state, sync);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_DAC_SyncSource_send(connection_state * state, ADAC250_DAC_SYNC_SOURCE syncsource)
{
    Result ret;
    cmd_send(state, ADAC250_DACSYNCSOURCE);
    uint_send(state, syncsource);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_DAC_Calibrate_send(connection_state * state)
{
    Result ret;
    cmd_send(state, ADAC250_DACCALIBRATE);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PLL_reset_send(connection_state * state,  ADAC250_CLOCK_SOURCES clksrc)
{
    Result ret;
    cmd_send(state, ADAC250_INITPLL);
    uint_send(state, clksrc);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PLL_Init_send(connection_state * state, unsigned int refclk, unsigned int adcclk, unsigned int dacclk, ADAC250_DAC_INTERPOLATION interp, ADAC250_CLOCK_SOURCES clksrc)
{
    Result ret;
	ADAC250_PLL_PARAM param;
	int locked = 0;
	volatile int wait;
	memset(&param,0,sizeof(ADAC250_PLL_PARAM));
	if(adcclk < ADAC250_MIN_ADC_PLL_CLK_FREQ)
	{
		return (ADAC250_NO_VALID_PLL_SOLUTION);
	}
	if(dacclk < ADAC250_MIN_DAC_PLL_CLK_FREQ)
	{
		return (ADAC250_NO_VALID_PLL_SOLUTION);
	}
	switch(clksrc)
	{
		case ADAC250_CLOCKIN_10MHZREF:
		{
			ret  = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_REFOUT, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_PLLCLK1, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				return ret;
			}

			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_PLLREF, ADAC250_CLOCKIN_10MHZ);
			if(ret != 0) 
			{
				return ret;
			}

			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_FMCCLK1, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				return ret;
			}
			
			ret = ADAC250_PLL_reset_send(state, clksrc);
			if(ret != 0) 
			{
				return ret;
			}

			ret = ADAC250_PLL_CalculateParams(refclk, adcclk, dacclk, interp, &param);
			if(ret != 0) 
			{
				return ret;
			}

			ret = ADAC250_PLL_SetConfig_send(state, param);
			if(ret != 0) 
			{
				return ret;
			}

			for(wait = 0; wait < 10000000;wait++);

			ADAC250_PLL_getStatus_send(state, &locked);
			if(ret != 0) 
			{
				return ret;
			}
			if(!locked)
			{
				return ADAC250_PLLNOTLOCKED;
			}

			break;
		}
		case ADAC250_CLOCKIN_EXTERNALREF:
		
			ret  = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_REFOUT, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_PLLCLK1, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_PLLREF, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_FMCCLK1, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				return ret;
			}
			ret = ADAC250_PLL_reset_send(state, clksrc);
			if(ret != 0) 
			{
				return ret;
			}
			ret = ADAC250_PLL_CalculateParams(refclk, adcclk, dacclk, interp, &param);
			if(ret != 0) 
			{
				return ret;
			}
			ret = ADAC250_PLL_SetConfig_send(state, param);
			if(ret != 0) 
			{
				//debug("Error: Writing pll config \n\r");
				return ret;
			}

			for(wait = 0; wait < 10000000;wait++);

			ADAC250_PLL_getStatus_send(state, &locked);
			if(ret != 0) 
			{
				return ret;
			}
			if(!locked)
			{
				return ADAC250_PLLNOTLOCKED;
			}

			break;
		case ADAC250_CLOCKIN_EXTERNALCLOCK:
			ret  = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_REFOUT, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_PLLCLK1, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_PLLREF, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput_send(state, ADAC250_CLOCKOUT_FMCCLK1, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			//param
			ret = ADAC250_PLL_reset_send(state, clksrc);
			if(ret != 0) 
			{
				return ret;
			}
	
			if(refclk%adcclk)
			{
				return ADAC250_NO_VALID_PLL_SOLUTION;
			}
			else
			{
				param.divisorADCClk = refclk / adcclk;
			}
			
			if(refclk%dacclk)
			{
				return ADAC250_NO_VALID_PLL_SOLUTION;
			}
			else
			{
				param.divisorDACDataClk = refclk / dacclk;
				param.divisorClkOutClk = refclk / dacclk;
			}
			
			if(refclk < (dacclk  * interp))
			{
				return ADAC250_NO_VALID_PLL_SOLUTION;
			}
			else
			{
				param.divisorDACInClk = param.divisorDACDataClk / (interp);
			}
			ret = ADAC250_PLL_SetConfig_send(state, param);
			if(ret != 0) 
			{
				//debug("Error: Writing pll config \n\r");
				return ret;
			}
			break;
	}
    return ret;
}

Result ADAC250_PLL_CalculateParams_send(connection_state * state, unsigned refClk, unsigned AdcClk, unsigned DacDataClk, ADAC250_DAC_INTERPOLATION interpolation, ADAC250_PLL_PARAM * param)
{
    Result ret;
    cmd_send(state, ADAC250_CALCULATEPLLPARAMS);

    uint_send(state, refClk);
    uint_send(state, AdcClk);
    uint_send(state, DacDataClk);
    uint_send(state, interpolation);
    ret = connection_flush(state);
    if(FAILURE(ret)) return ret;

    adac250_pll_param_recv(state, param);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PLL_SetConfig_send(connection_state * state, ADAC250_PLL_PARAM param)
{
    Result ret;
    cmd_send(state, ADAC250_SETPLLCONFIG);
    adac250_pll_param_send(state, &param);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PLL_trigRegisterTransfert_send(connection_state * state)
{
    Result ret;
    cmd_send(state, ADAC250_TRIGPLLREGISTERTRANSFERT);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PLL_SyncOutput_send(connection_state * state)
{
    Result ret;
    cmd_send(state, ADAC250_PLLSYNCOUTPUT);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PLL_getStatus_send(connection_state * state, int * locked)
{
    Result ret;
    cmd_send(state, ADAC250_GETPLLSTATUS);
    *locked = uint_recv(state);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PLL_SetSleep_send(connection_state * state, ADAC250_PLL_SLEEP sleep)
{
    Result ret;
    cmd_send(state, ADAC250_SETPLLSLEEP);
    uchar_send(state, sleep.Byte);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_PLL_setRefClkTuning_send(connection_state * state, unsigned short value)
{
    Result ret;
    cmd_send(state, ADAC250_SETREFCLKTUNING);
    ushort_send(state, value);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_init_send(connection_state * state)
{
    Result ret;
    cmd_send(state, ADAC250_INITADC);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_setGain_send(connection_state * state, ADAC250_ADC_CHANNEL channel, ADAC250_ADC_GAIN gain)
{
    Result ret;
    cmd_send(state, ADAC250_SETADCGAIN);
    uint_send(state, channel);
    uint_send(state, gain);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_setFineGain_send(connection_state * state, ADAC250_ADC_CHANNEL channel, char gain)
{
    Result ret;
    cmd_send(state, ADAC250_SETADCFINEGAIN);
    uint_send(state, channel);
    uchar_send(state, gain);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_setPedestal_send(connection_state * state, ADAC250_ADC_CHANNEL channel, char value)
{
    Result ret;
    cmd_send(state, ADAC250_SETADCPEDESTAL);
    uint_send(state, channel);
    uchar_send(state, value);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_getStatus_send(connection_state * state, ADAC250_ADC_CHANNEL channel, ADAC250_ADC_STATUSES * status)
{
    Result ret;
    cmd_send(state, ADAC250_GETADCSTATUS);
    uint_send(state, channel);
    *status = uint_recv(state);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_Calibrate_send(connection_state * state)
{
    Result ret;
    cmd_send(state, ADAC250_ADCCALIBRATE);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_SetDataFormat_send(connection_state * state, ADAC250_ADC_DATAFORMAT format)
{
    Result ret;
    cmd_send(state, ADAC250_SETADCDATAFORMAT);
    uint_send(state, format);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_SetSleepMode_send(connection_state * state, ADAC250_ADC_SLEEP_MODE mode)
{
    Result ret;
    cmd_send(state, ADAC250_SETADCSLEEPMODE);
    uint_send(state, mode);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_SetOffsetCorrectionTime_send(connection_state * state, ADAC250_CHANNEL_OCT octa, ADAC250_CHANNEL_OCT octb)
{
    Result ret;
    cmd_send(state, ADAC250_SETADCOFFSETCORRECTIONTIME);
    uint_send(state, octa);
    uint_send(state, octb);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_ADC_SetTriggerDelay_send(connection_state * state, char triggerdelay)
{
    Result ret;
    cmd_send(state, ADAC250_SETADCTRIGGERDELAY);
    uint_send(state, triggerdelay);
    ret = result_recv(state);
    return ret;
}

Result ADAC250_MUX_ConfigClockOutput_send(connection_state * state, ADAC250_CLOCK_OUTPUT_ID ClockToConfig, ADAC250_CLOCK_INPUT_ID ClockInputSelection)
{
    Result ret;
    cmd_send(state, ADAC250_SETMUX);
    uint_send(state, ClockToConfig);
    uint_send(state, ClockInputSelection);
    ret = result_recv(state);
    return ret;
}

