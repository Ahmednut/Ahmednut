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
 * @file       adt7411.c
 * @brief      ADT7411 configuration API
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADC5000
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
#include "adt7411.h"
#include "adt7411_regs.h"
#include <string.h>

Result adt7411_open(adt7411_core * core, com_core * com)
{
    core->com = com;
    return OK;
}

Result adt7411_reset(adt7411_core * core)
{
    Result ret;
    ADT7411_Register_Map_t regs;

    // Default setting: register 18 Control configuration 1
    // Interrupt polarity is active low.
    // Interrupt output is enabled.
    ret = com_read(core->com, COM_SENSORS, 0x18, &regs.Register_18.raw);
    
    // Set Pin7 and Pin8 to external temperature.
    regs.Register_18.Bit.selectAIN1andAIN2OrExtTempr = ADT7411_EXT_TEMPR_SELECT;
    regs.Register_18.Bit.reserved2 = 1;
    regs.Register_18.Bit.enableInterrupt = 1;
    ret = com_write(core->com, COM_SENSORS, 0x18, &regs.Register_18.raw);

    // Default setting: register 19 Control configuration 2
    // SMBus timeout is disabled.
    // Averaging is enabled.
    // Conversion cycle type is round robin.
    // Single channel selection is Vdd.

    // Default setting: register 1A Control configuration 3
    // ADC reference is internal Vref.
    // ADC speed is normal 1.4KHz.

    // Disable interrupt AIN2.
    regs.Register_1D.raw = 0x20;
    ret = com_write(core->com, COM_SENSORS, 0x1d, &regs.Register_1D.raw);

    // Set internal temperature High limit to 85.
    regs.Register_25.raw = 85;
    ret = com_write(core->com, COM_SENSORS, 0x25, &regs.Register_25.raw);

    // Set internal temperature Low limit to -40.
    regs.Register_26.raw = -40;
    ret = com_write(core->com, COM_SENSORS, 0x26, &regs.Register_26.raw);

    // Set external temperature High limit to 85.
    regs.Register_27.raw = 85;
    ret = com_write(core->com, COM_SENSORS, 0x27, &regs.Register_27.raw);

    // Set external temperature Low limit to -40.
    regs.Register_28.raw = -40;
    ret = com_write(core->com, COM_SENSORS, 0x28, &regs.Register_28.raw);
    
    // Start monitoring.
    return adt7411_setMonitoringOnOff(core, ADT7411_ENABLE);
}

Result adt7411_close(adt7411_core * core)
{
    return OK;
}

Result adt7411_read_sensor_raw(adt7411_core * core, ad7411_sensor sensor, short * data)
{
    Result ret;
    ADT7411_Register_Map_t regs;
    unsigned char lsb;
    unsigned char msb;
    unsigned short value;

    switch(sensor)
    {
    case AD7411_SENSOR_INTERNAL_TEMP:
        ret = com_read(core->com, COM_SENSORS, 0x3, &regs.Register_03.raw);
        lsb = regs.Register_03.Bit.lsbIntTempr;
        ret = com_read(core->com, COM_SENSORS, 0x7, &regs.Register_07.raw);
        msb = regs.Register_07.Bit.msbIntTempr;
        break;

    case AD7411_SENSOR_EXTERNAL_TEMP:
        ret = com_read(core->com, COM_SENSORS, 0x4, &regs.Register_04.raw);
        lsb = regs.Register_04.Bit.lsbExtTempr;
        ret = com_read(core->com, COM_SENSORS, 0x8, &regs.Register_08.raw);
        msb = regs.Register_08.Bit.msbExtTempr;
        break;

    case AD7411_SENSOR_VDD:
        ret = com_read(core->com, COM_SENSORS, 0x3, &regs.Register_03.raw);
        lsb = regs.Register_03.Bit.lsbVdd;
        ret = com_read(core->com, COM_SENSORS, 0x6, &regs.Register_06.raw);
        msb = regs.Register_06.Bit.msbVdd;
        break;

    case AD7411_SENSOR_AIN3:
        ret = com_read(core->com, COM_SENSORS, 0x4, &regs.Register_04.raw);
        lsb = regs.Register_04.Bit.lsbAIN3;
        ret = com_read(core->com, COM_SENSORS, 0xA, &regs.Register_0A.raw);
        msb = regs.Register_0A.Bit.msbAIN3;
        break;

    case AD7411_SENSOR_AIN4:
        ret = com_read(core->com, COM_SENSORS, 0x4, &regs.Register_04.raw);
        lsb = regs.Register_04.Bit.lsbAIN4;
        ret = com_read(core->com, COM_SENSORS, 0xB, &regs.Register_0B.raw);
        msb = regs.Register_0B.Bit.msbAIN4;
        break;

    case AD7411_SENSOR_AIN5:
        ret = com_read(core->com, COM_SENSORS, 0x5, &regs.Register_05.raw);
        lsb = regs.Register_05.Bit.lsbAIN5;
        ret = com_read(core->com, COM_SENSORS, 0xC, &regs.Register_0C.raw);
        msb = regs.Register_0C.Bit.msbAIN5;
        break;

    case AD7411_SENSOR_AIN6:
        ret = com_read(core->com, COM_SENSORS, 0x5, &regs.Register_05.raw);
        lsb = regs.Register_05.Bit.lsbAIN6;
        ret = com_read(core->com, COM_SENSORS, 0xD, &regs.Register_0D.raw);
        msb = regs.Register_0D.Bit.msbAIN6;
        break;

    case AD7411_SENSOR_AIN7:
        ret = com_read(core->com, COM_SENSORS, 0x5, &regs.Register_05.raw);
        lsb = regs.Register_05.Bit.lsbAIN7;
        ret = com_read(core->com, COM_SENSORS, 0xE, &regs.Register_0E.raw);
        msb = regs.Register_0E.Bit.msbAIN7;
        break;

    case AD7411_SENSOR_AIN8:
        ret = com_read(core->com, COM_SENSORS, 0x5, &regs.Register_05.raw);
        lsb = regs.Register_05.Bit.lsbAIN8;
        ret = com_read(core->com, COM_SENSORS, 0xF, &regs.Register_0F.raw);
        msb = regs.Register_0F.Bit.msbAIN8;
        break;

    case AD7411_SENSOR_AIN1:
    case AD7411_SENSOR_AIN2:
        return ADT7411UNIMPLEMENTED;
    default:
        return ADT7411IVAL;
    }

    if(data)
    {
        value = msb << 2 | lsb;
        
        *data = value;

        //sign extend
        if(value & (1 << 9))
        	*data |= 0xFC00;
    }
    return OK;
}

Result adt7411_setMonitoringOnOff(adt7411_core * core, adt7411_state state)
{
    Result ret;
    ADT7411_Register_Map_t reg;

    memset(&reg, 0, sizeof(reg));

    ret = com_read(core->com, COM_SENSORS, 0x18, &reg.Register_18.raw);
    reg.Register_18.Bit.startMonitoring = state;
    return com_write(core->com, COM_SENSORS, 0x18, &reg.Register_18.raw);
}


