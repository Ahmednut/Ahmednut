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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       ev10aq190.c
 * @brief      EV10AQ190 configuration API
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADC5000
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
#include "ev10aq190.h"
#include "com_wrapper.h"

unsigned char ev10aq190_default_config_addr[] = {
    0x00
};

unsigned short ev10aq190_default_config_data[] = {
    0x00
};

Result ev10aq190_open(ev10aq190_core * core, com_core * com)
{
    core->com = com;
    return OK;
}

Result ev10aq190_reset(ev10aq190_core * core)
{
    Result ret = OK;
    //ev10aq190_Register_Map regs;
    
    // Reset command has been commented because it causes a strange behavior
    // of the SPI communication. The last bit is always '1'
    /*
    regs.Register_04.Bit.reset = 1;
    ret = com_write(core->com, COM_ADC, 0x4, &regs.Register_04.raw);
    usleep(1000);
    */
    
    
    return ret;
}

Result ev10aq190_close(ev10aq190_core * core)
{
    return OK;
}

Result ev10aq190_getstatus(ev10aq190_core * core, unsigned char * adc_A_active, unsigned char * adc_B_active, unsigned char * adc_C_active, unsigned char * adc_D_active)
{
    Result ret;
    ev10aq190_Register_Map regs;

    ret = com_read(core->com, COM_ADC, 0x2, (unsigned*) &regs.Register_02.raw);
    if(FAILURE(ret)) return ret;
    
    if(adc_A_active)
      *adc_A_active = ((regs.Register_02.Bit.adcxup >> 0) & 0x1);
    if(adc_B_active)
      *adc_B_active = ((regs.Register_02.Bit.adcxup >> 1) & 0x1);
    if(adc_C_active)
      *adc_C_active = ((regs.Register_02.Bit.adcxup >> 2) & 0x1);
    if(adc_D_active)
      *adc_D_active = ((regs.Register_02.Bit.adcxup >> 3) & 0x1);
    
    return OK;
}

Result ev10aq190_setmode(ev10aq190_core * core, ev10aq190_channel mode)
{
    Result ret;
    ev10aq190_Register_Map regs;

    ret = com_read(core->com, COM_ADC, 0x1, (unsigned*) &regs.Register_01.raw);
    if(FAILURE(ret)) return ret;
    
    regs.Register_01.Bit.adc_mode = mode;
    
    ret = com_write(core->com, COM_ADC, 0x1, &regs.Register_01.raw);
    return ret;
}

Result ev10aq190_setstandby(ev10aq190_core * core, ev10aq190_standby standby)
{
    Result ret;
    ev10aq190_Register_Map regs;

    ret = com_read(core->com, COM_ADC, 0x1, (unsigned*) &regs.Register_01.raw);
    if(FAILURE(ret)) return ret;
    
    regs.Register_01.Bit.standby = standby;
    
    ret = com_write(core->com, COM_ADC, 0x1, &regs.Register_01.raw);
    return ret;
}

Result ev10aq190_setcoding(ev10aq190_core * core, ev10aq190_coding coding)
{
    Result ret;
    ev10aq190_Register_Map regs;

    ret = com_read(core->com, COM_ADC, 0x1, (unsigned*) &regs.Register_01.raw);
    if(FAILURE(ret)) return ret;
    
    regs.Register_01.Bit.coding = coding;
    
    ret = com_write(core->com, COM_ADC, 0x1, &regs.Register_01.raw);
    return ret;
}

Result ev10aq190_setbandwidth(ev10aq190_core * core, ev10aq190_bandwidth bandwidth)
{
    Result ret;
    ev10aq190_Register_Map regs;

    ret = com_read(core->com, COM_ADC, 0x1, (unsigned*) &regs.Register_01.raw);
    if(FAILURE(ret)) return ret;
    
    regs.Register_01.Bit.bdw = bandwidth;
    
    ret = com_write(core->com, COM_ADC, 0x1, &regs.Register_01.raw);
    return ret;
}

Result ev10aq190_settestmode(ev10aq190_core * core, ev10aq190_testmode state)
{
    Result ret;
    ev10aq190_Register_Map regs;

    switch(state)
    {
    case EV10AQ190_BITFLASH:
        com_read(core->com, COM_ADC, 0x1, (unsigned*) &regs.Register_01.raw);
        regs.Register_01.Bit.test = 1;
        com_read(core->com, COM_ADC, 0x5, (unsigned *) &regs.Register_05.raw);
        regs.Register_05.Bit.test_mode = 1;
        com_write(core->com, COM_ADC, 0x5, &regs.Register_05.raw);
        ret = com_write(core->com, COM_ADC, 0x1, &regs.Register_01.raw);
        break;
    default:
    case EV10AQ190_DISABLE:
        com_read(core->com, COM_ADC, 0x1, (unsigned*) &regs.Register_01.raw);
        regs.Register_01.Bit.test = 0;
        ret = com_write(core->com, COM_ADC, 0x1, &regs.Register_01.raw);
        break;
	case EV10AQ190_RAMP:
        com_read(core->com, COM_ADC, 0x1, (unsigned*) &regs.Register_01.raw);
        regs.Register_01.Bit.test = 1;
        com_read(core->com, COM_ADC, 0x5, (unsigned *) &regs.Register_05.raw);
        regs.Register_05.Bit.test_mode = 0;
        com_write(core->com, COM_ADC, 0x5, &regs.Register_05.raw);
        ret = com_write(core->com, COM_ADC, 0x1, &regs.Register_01.raw);
        break;
    }
    return ret;
}

Result ev10aq190_offset(ev10aq190_core * core, ev10aq190_channel channel, unsigned short offset)
{
    ev10aq190_Register_Map regs;
    int timeout;

    com_read(core->com, COM_ADC, 0x0F, (unsigned*)&regs.Register_0F.raw);
    regs.Register_0F.Bit.channel_selector = channel;
    com_write(core->com, COM_ADC, 0x0F, &regs.Register_0F.raw);

    regs.Register_20.raw = 0;
    regs.Register_20.Bit.external_offset = offset;
    com_write(core->com, COM_ADC, 0x20, &regs.Register_20.raw);

    regs.Register_10.raw = 0;
    regs.Register_10.Bit.ocalctrl = 0x2;
    com_write(core->com, COM_ADC, 0x10, &regs.Register_10.raw);

    for(timeout=0; timeout<100; timeout++)
    {
        com_read(core->com, COM_ADC, 0x11, (unsigned*)&regs.Register_11.raw);
        if (!regs.Register_11.Bit.busy)
        {
            break;
        }
    }

    if (timeout == 100)
    {
        return EV10AQ190_CALIB_TIMEOUT;
    }

    regs.Register_10.Bit.ocalctrl = 0;
    return com_write(core->com, COM_ADC, 0x10, &regs.Register_10.raw);
}


Result ev10aq190_gain(ev10aq190_core * core, ev10aq190_channel channel, unsigned short gain)
{
    ev10aq190_Register_Map regs;
    int timeout;

    com_read(core->com, COM_ADC, 0x0F, (unsigned*)&regs.Register_0F.raw);
    regs.Register_0F.Bit.channel_selector = channel;
    com_write(core->com, COM_ADC, 0x0F, &regs.Register_0F.raw);

    regs.Register_22.raw = 0;
    regs.Register_22.Bit.external_gain = gain;
    com_write(core->com, COM_ADC, 0x22, &regs.Register_22.raw);

    regs.Register_10.raw = 0;
    regs.Register_10.Bit.gcalctrl = 0x2;
    com_write(core->com, COM_ADC, 0x10, &regs.Register_10.raw);

    for(timeout=0; timeout<100; timeout++)
    {
        com_read(core->com, COM_ADC, 0x11, (unsigned*)&regs.Register_11.raw);
        if (!regs.Register_11.Bit.busy)
        {
            break;
        }
    }

    if (timeout == 100)
    {
        return EV10AQ190_CALIB_TIMEOUT;
    }

    regs.Register_10.Bit.gcalctrl = 0;
    return com_write(core->com, COM_ADC, 0x10, &regs.Register_10.raw);
}

Result ev10aq190_phase(ev10aq190_core * core, ev10aq190_channel channel, unsigned short phase)
{
    ev10aq190_Register_Map regs;
    int timeout;

    com_read(core->com, COM_ADC, 0x0F, (unsigned*)&regs.Register_0F.raw);
    regs.Register_0F.Bit.channel_selector = channel;
    com_write(core->com, COM_ADC, 0x0F, &regs.Register_0F.raw);

    regs.Register_24.raw = 0;
    regs.Register_24.Bit.external_phase = phase;
    com_write(core->com, COM_ADC, 0x24, &regs.Register_24.raw);

    regs.Register_10.raw = 0;
    regs.Register_10.Bit.pcalctrl = 0x2;
    com_write(core->com, COM_ADC, 0x10, &regs.Register_10.raw);

    for(timeout=0; timeout<100; timeout++)
    {
        com_read(core->com, COM_ADC, 0x11, (unsigned*)&regs.Register_11.raw);
        if (!regs.Register_11.Bit.busy)
        {
            break;
        }
    }

    if (timeout == 100)
    {
        return EV10AQ190_CALIB_TIMEOUT;
    }

    regs.Register_10.Bit.pcalctrl = 0;
    return com_write(core->com, COM_ADC, 0x10, &regs.Register_10.raw);
}

