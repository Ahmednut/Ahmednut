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
 * @file       com_wrapper.c
 * @brief      COM WRAPPER configuration API
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
#include "com_wrapper.h"
#include "carrier_service.h"
#include "bas_debug.h"

#define I2C_BRIDGE_ADDR  (0x28)
#define I2C_SENSORS_ADDR (0x48)

#ifndef ONLY_CALCULATE

static const unsigned short i2c_bridge_spi_speed[] = {1843, 461, 115, 58 };
static unsigned char i2c_bridge_initval = 0;

//static unsigned char cpld_shadow[4] = {0, 0, 0, 0};

/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/

Result com_open(com_core * core, Carrier_handle_t hCarrier, unsigned int zone)
{
    core->hCarrier = hCarrier;
    core->zone = zone;

    return OK;
}

static Result set_spi_speed(com_core * core, unsigned char speed)
{
    speed &= 0x3;
    core->spi_clock_rate = speed;
    i2c_bridge_initval &= ~0x3;
    i2c_bridge_initval |= speed;
    return com_write(core, COM_I2C_BRIDGE, 0xF0, &i2c_bridge_initval);
}

Result com_init(com_core * core, unsigned char speed, unsigned char mode )
{
    Result ret;

    if (i2c_bridge_initval == (mode << 2 | speed))
      return OK;

    //SPI mode
    i2c_bridge_initval = mode << 2;

    //SPI speed and send parameters to the bridge
    ret = set_spi_speed(core, speed);
    if(FAILURE(ret)) return ret;

    Carrier_usleep(core->hCarrier, 1000000);
    return OK;
}

Result com_close(com_core * core)
{
    return OK;
}

//up to 4 byte at a time
static Result com_write_pll_arr(com_core * core, unsigned short addr, char * data, int size)
{
    int i,j;
    unsigned char buffer[7];
    unsigned char index;
    int sentByteCount;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    if (size > 4 || size < 1)
    {
        return I2C_TOOLARGE;
    }

    //function id
    buffer[0] = SPI_CS_PLL;

    index = size - 1;
    addr &= 0x3FF;
    addr += index;
    //SPI data
    //long instruction word
    buffer[1] = index << 5 | addr >> 8;
    buffer[2] = addr & 0xFF;
    //data
    j = 3;
    for (i=index;i>=0; i--)
    {
        buffer[j++] = data[i];
	 debug("I2C W PLL: %x \n", data[i]);

    }

    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer[0], buffer+1, size + 2, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (sentByteCount == size + 2)
    {
        debug("S I2C W PLL: %x size %x\n", addr - size + 1, size);
        return OK;
    }
    else{
        debug("F I2C W PLL: %x size %x\n", addr - size + 1,size);
        return I2C_FAILED;
    }
}

//up to 1 16-bit transfer
static Result com_write_adc(com_core * core, unsigned short addr, unsigned short data)
{
    unsigned char buffer[7];
    int sentByteCount;
    const int transfer_size = 4;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //function id
    buffer[0] = SPI_CS_ADC;

    addr &= 0x3F;
    //SPI data
    //short instruction word
    buffer[1] = addr | 0x80;
    //data
    buffer[2] = data >> 8;
    buffer[3] = data & 0xFF;

    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer[0], buffer+1, transfer_size-1, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (sentByteCount == transfer_size - 1)
    {
	 debug("S I2C W CPLD: %x %x\n", addr, data);
        return OK;
    }else{

        debug("F I2C W ADC: %x %x\n", addr, data);
        return I2C_FAILED;
    }
}

//up to 1 8-bit transfer
static Result com_write_cpld(com_core * core, unsigned short addr, unsigned char data)
{
    unsigned char buffer[7];
    int sentByteCount;
    const int transfer_size = 3;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //function id
    buffer[0] = SPI_CS_CPLD;

    addr &= 0x3F;
    //SPI data
    //short instruction word
    buffer[1] = addr & 0x7F;
    //data
    buffer[2] = data & 0xFF;

    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer[0], buffer+1, transfer_size-1, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (sentByteCount == transfer_size - 1)
    {
	 debug("S I2C W CPLD: %x %x\n", addr, data);
        return OK;
    }else
    {
	 debug("F I2C W CPLD: %x %x\n", addr, data);
        return I2C_FAILED;
    }
}

//up to 1 8-bit transfer
static Result com_write_sensors(com_core * core, unsigned short addr, unsigned char data)
{
    unsigned char buffer[2];
    int sentByteCount;
    const int transfer_size = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    buffer[0] = addr & 0xFF;
    buffer[1] = data;

    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, I2C_SENSORS_ADDR, buffer[0], buffer+1, transfer_size-1, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (sentByteCount == transfer_size - 1)
    {	 debug("S I2C W Sensors: %x %x\n", addr, data);
        return OK;
    }else{
	 debug("F I2C W Sensors: %x %x\n", addr, data);
        return I2C_FAILED;
    }
}

//up to 1 8-bit transfer
static Result com_write_bridge(com_core * core, unsigned char addr, unsigned char data)
{
    unsigned char buffer[2];
    int sentByteCount;
    const int transfer_size = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    buffer[0] = addr;
    buffer[1] = data;

    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer[0], buffer+1, transfer_size-1, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (sentByteCount == transfer_size - 1)
    {
	debug("S I2C W bridge: %x %x\n", addr, data);
        return OK;
    }
    else
    {
	 debug("F I2C W bridge: %x %x\n", addr, data);
        return I2C_FAILED;
    }
}

//Keep user sane, data is always MSB first with incrementing address
Result com_write_arr(com_core * core, COM_DEVICES device, unsigned short addr, const void * data, int size)
{
    int ret;
    int i;

    switch(device)
    {
    case COM_PLL:
        com_init(core, 0, 2);
        //TODO support streaming mode
        for(i=0; i<size; i+=4)
        {
            if( size - i >= 4)
            {
                ret = com_write_pll_arr(core, addr + i, &((char*)data)[i], 4);
            }else{
                ret = com_write_pll_arr(core, addr + i, &((char*)data)[i], size - i);
            }
            if(FAILURE(ret)) return ret;
        }

        break;

    case COM_ADC:
        //com_init(core, 0, 2);
        for(i=0; i<size; i++)
        {
            ret = com_write_adc(core, addr+i, ((unsigned short*)data)[i]);
            if(FAILURE(ret)) return ret;
        }
        break;

    case COM_CPLD:
        com_init(core, 0, 2);
        for(i=0; i<size; i++)
        {
            ret = com_write_cpld(core, addr+i, ((unsigned char*)data)[i]);
            if(FAILURE(ret)) return ret;
            //cpld_shadow[addr+i] = ((unsigned char*)data)[i];
        }
        break;

    case COM_SENSORS:
        for(i=0; i<size; i++)
        {
            ret = com_write_sensors(core, addr+i, ((unsigned char*)data)[i]);
            if(FAILURE(ret)) return ret;
        }
        break;

    case COM_I2C_BRIDGE:
        for(i=0; i<size; i++)
        {
            ret = com_write_bridge(core, addr+i, ((unsigned char*)data)[i]);
            if(FAILURE(ret)) return ret;
        }
        break;

    case COM_EEPROM:
        return I2C_UNIMPLEMENTED;
    }

    return OK;
}

//up to 4 byte at a time
static Result com_read_pll_arr(com_core * core, unsigned short addr, unsigned char * data, int size)
{
    int i;
    unsigned char buffer[7];
    unsigned char index;
    int sentByteCount, receivedByteCount;
    unsigned char tmp;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    if (size > 4 || size < 1)
    {
        return I2C_TOOLARGE;
    }

    //function id
    buffer[0] = SPI_CS_PLL;

    index = size - 1;
    addr &= 0x3FF;
    addr += index;
    //SPI data
    //long instruction word
    buffer[1] = 1 << 7 | index << 5 | addr >> 8;
    buffer[2] = addr & 0xFF;
    //data
    for (i=3; i < size + 3; i++)
    {
        buffer[i] = 0xFF;
    }

    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer[0], buffer+1, size + 2, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (sentByteCount != size + 2)
    {
	 debug("C I2C R PLL:%x %x %x\n", addr, sentByteCount, size + 3);
        return I2C_FAILED;
    }

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cReadNoWrData(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer, size + 2);

    for (i=0; i < size; i++)
    {
        data[i] = buffer[i+2];
    }

    //data is in decrementing order, switch for incrementing order
    for (i=0; i<size/2; i++)
    {
        tmp = data[i];
        data[i] = data[size-i-1];
        data[size-i-1] = tmp;
    }

    if (receivedByteCount == size + 2)
    {
	 debug("S I2C R PLL: %x\n", addr);
        return OK;
    }else{
	 debug("F I2C R PLL: %x\n", addr);
        return I2C_FAILED;
    }
}

//up to 1 16-bit transfer
static Result com_read_adc(com_core * core, unsigned short addr, unsigned short * data)
{
    unsigned char buffer[7];
    int sentByteCount, receivedByteCount;
    const int transfer_size = 4;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //function id
    buffer[0] = SPI_CS_ADC;

    //SPI data
    //short instruction word
    buffer[1] = addr & 0x3F;
    //data
    buffer[2] = 0xFF;
    buffer[3] = 0xFF;

    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer[0], buffer+1, transfer_size-1, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (sentByteCount != transfer_size - 1)
    {
	 debug("C I2C R ADC: %x\n", addr);
        return I2C_FAILED;
    }

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cReadNoWrData(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer, transfer_size - 1);

    if (data) *data = buffer[1] << 8 | buffer[2];

    if (receivedByteCount == transfer_size - 1 )
    {
	debug("S I2C R ADC: %x\n", addr);
        return OK;
    }else{
	 debug("F I2C R ADC: %x\n", addr);
        return I2C_FAILED;
    }
}

//up to 1 8-bit transfer
static Result com_read_cpld(com_core * core, unsigned short addr, unsigned char * data)
{
    unsigned char buffer[7];
    int sentByteCount, receivedByteCount;
    const int transfer_size = 3;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //function id
    buffer[0] = SPI_CS_CPLD;

    //SPI data
    //short instruction word
    buffer[1] = (addr & 0x7F) | 0x80;
    //data
    buffer[2] = 0xFF;

    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer[0], buffer+1, transfer_size-1, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (sentByteCount != transfer_size - 1)
    {
	 debug("C I2C R PLL: %x\n", addr);
        return I2C_FAILED;
    }

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cReadNoWrData(core->hCarrier, core->zone, I2C_BRIDGE_ADDR, buffer, transfer_size - 1);

    if(data) *data = buffer[1];

    if (receivedByteCount == transfer_size - 1 )
    {
	debug("S I2C R CPLD: %x\n", addr);
        return OK;
    }else{
	 debug("F I2C R CPLD: %x\n", addr);
        return I2C_FAILED;
    }
}

//up to 1 8-bit transfer
static Result com_read_sensors(com_core * core, unsigned short addr, unsigned char * data)
{
    int receivedByteCount;
    const int transfer_size = 1;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    addr &= 0xFF;

    receivedByteCount = Carrier_I2cRead(core->hCarrier, core->zone, I2C_SENSORS_ADDR, addr, data, transfer_size, options);

    Carrier_usleep(core->hCarrier, 1000);

    if (receivedByteCount == transfer_size )
    {
	 debug("S I2C R Sensors: %x\n", addr);
        return OK;
    }else{
	 debug("F I2C R Sensors: %x\n", addr);
        return I2C_FAILED;
    }
}

Result com_read_arr(com_core * core, COM_DEVICES device, unsigned short addr, void * data, int size)
{
    int ret;
    int i;

    switch(device)
    {
    case COM_PLL:
        com_init(core, 0, 2);
        //TODO support streaming mode
        for(i=0; i<size; i+=4)
        {
            if( size - i >= 4)
            {
                ret = com_read_pll_arr(core, addr + i, &((unsigned char*)data)[i], 4);
            }else{
                ret = com_read_pll_arr(core, addr + i, &((unsigned char*)data)[i], size - i);
            }
            if(FAILURE(ret)) return ret;
        }
        break;

    case COM_ADC:
        com_init(core, 3, 0);
        for(i=0; i<size; i++)
        {
            ret = com_read_adc(core, addr+i, &((unsigned short*)data)[i]);
            if(FAILURE(ret)) return ret;
        }
        break;

    case COM_CPLD:
        com_init(core, 0, 2);
        for(i=0; i<size; i++)
        {
            ret = com_read_cpld(core, addr+i, &((unsigned char*)data)[i]);
            if(FAILURE(ret)) return ret;
            //((char*)data)[i] = cpld_shadow[addr+i];
        }
        break;

    case COM_SENSORS:
        for(i=0; i<size; i++)
        {
            ret = com_read_sensors(core, addr+i, &((unsigned char*)data)[i]);
            if(FAILURE(ret)) return ret;
        }
        break;

    case COM_EEPROM:
    case COM_I2C_BRIDGE:
        return I2C_UNIMPLEMENTED;
    }

    return OK;
}

Result com_write(com_core * core, COM_DEVICES device, unsigned short addr, const void * data)
{
    return com_write_arr(core, device, addr, data, 1);
}

Result com_read(com_core * core, COM_DEVICES device, unsigned short addr, void * data)
{
    return com_read_arr(core, device, addr, data, 1);
}

#endif


