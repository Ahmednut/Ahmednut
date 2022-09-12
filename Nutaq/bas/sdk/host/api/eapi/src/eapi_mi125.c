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
 * @file      eapi_mi125.c
 * 
 * @brief     Function definitions for the eapi MI125 control
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
#include "eapi_mi125.h"
#include "eapi_serialize.h"
#include "mi125_serialize.h"


Result MI125_Presence_send(connection_state * state, unsigned nb)
{
    Result ret;
    
	cmd_send(state, MI125_INIT);
	uint_send(state, nb-1);
	ret = result_recv(state);
    return ret;
}


Result MI125_powerup_send(connection_state * state, unsigned nb)
{
    Result ret;
    
	cmd_send(state, MI125_POWERUP);
	uint_send(state, nb-1);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_reset_send(connection_state * state, unsigned nb)
{
    Result ret;
    
	cmd_send(state, MI125_MI125_RESET);
	uint_send(state, nb-1);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_digital_adccalibration_send(connection_state * state, unsigned nb)
{
    Result ret;
    
	cmd_send(state, MI125_MI125_DIGITAL_ADCCALIBRATION);
	uint_send(state, nb-1);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_set_config_send(connection_state * state, unsigned nb, MI125_mi125_channel_mode groupch, MI125_mi125_lvds lvds, MI125_mi125_randmode randmode, MI125_mi125_binmode binmode)
{
    Result ret; 
    MI125_mi125_config config;
    
    config.groupch = groupch;
    config.lvds = lvds;
    config.randmode = randmode;
    config.binmode = binmode;    
	cmd_send(state, MI125_MI125_SET_CONFIG);
	uint_send(state, nb-1);
	mi125_set_config_param_send(state, &config);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_set_clksrc_send(connection_state * state, unsigned nb, MI125_mi125_clock_source clksrc)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_SET_CLKSRC);
	uint_send(state, nb-1);
	uint_send(state, clksrc);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_checkcore_send(connection_state * state, unsigned nb, unsigned char *detected)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_CHECKCORE);
	uint_send(state, nb-1);
	*detected = uchar_recv(state);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_set_testmode_send(connection_state * state, unsigned nb, MI125_mi125_testmode test, unsigned short pattern)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_SET_TESTMODE);
	uint_send(state, nb-1);
	uint_send(state, test);
	ushort_send(state, pattern);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_get_temperature_send(connection_state * state, unsigned nb, MI125_mi125_tempmode mode, short *temp)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_GET_TEMPERATURE);
	uint_send(state, nb-1);
	uint_send(state, mode);
	*temp = ushort_recv(state);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_get_channelcalibstatus_send(connection_state * state, unsigned nb, unsigned int *channellanecalib, unsigned int *channelcalibstatus)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_GET_CHANNELCALIBSTATUS);
	uint_send(state, nb-1);
	*channellanecalib = uint_recv(state);
	*channelcalibstatus = uint_recv(state);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_get_versions_send(connection_state * state, unsigned nb, unsigned int *core_version, unsigned int *drv_version)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_GET_VERSIONS);
	uint_send(state, nb-1);
	*core_version = uint_recv(state);
	*drv_version = uint_recv(state);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_checkadc_send(connection_state * state, unsigned nb, unsigned adcid, unsigned char *written, unsigned char *verify,  unsigned char *detected)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_CHECKADC);
	uint_send(state, nb-1);
	uint_send(state, adcid);
	*written = uchar_recv(state);
	*verify = uchar_recv(state);
	*detected = uchar_recv(state);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_set_trigout_send(connection_state * state, unsigned nb, MI125_mi125_trigout trigout)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_SET_TRIGOUT);
	uint_send(state, nb-1);
	uint_send(state, trigout);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_get_clkmaster_send(connection_state * state, unsigned nb, unsigned int *master)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_GET_CLKMASTER);
	uint_send(state, nb-1);
	*master = uint_recv(state);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_clockreset_send(connection_state * state, unsigned nb)
{
    Result ret;
    
	cmd_send(state, MI125_MI125_CLOCKRESET);
	uint_send(state, nb-1);
	ret = result_recv(state);
    return ret;
}


Result MI125_mi125_scan_i2c_send(connection_state * state, unsigned nb, unsigned char scanmax, MI125_mi125_i2cscan *scanraw,
                                 MI125_mi125_i2caddr *scan_res, unsigned char *ndevices, unsigned char *detect_ok)
{
    Result ret, ret1, ret2;
    
	cmd_send(state, MI125_MI125_SCAN_I2C);
	uint_send(state, nb-1);
	uchar_send(state, scanmax);
	*ndevices = uchar_recv(state);
	*detect_ok = uchar_recv(state);
    ret2 = buf_recv(state, (char*)scanraw, sizeof(MI125_mi125_i2cscan));
    ret1 = buf_recv(state, (char*)scan_res, sizeof(MI125_mi125_i2caddr));
	ret = result_recv(state);
    if (SUCCESS(ret))
    {
        if (FAILURE(ret2))
            ret = ret2;
        else
            if (FAILURE(ret1))
                ret = ret1;
    }
    return ret;
}


Result MI125_mi125_test_localoscsw_send(connection_state * state, unsigned nb, unsigned char *result_ok)
{
    Result ret; 
    
	cmd_send(state, MI125_MI125_TEST_LOCALOSCSW);
	uint_send(state, nb-1);
	*result_ok = uchar_recv(state);
	ret = result_recv(state);
    return ret;
}


