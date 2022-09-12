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
 * @file       MI125_com_wrapper.c
 * @brief      I2C communication wrapper for all devices control
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI125
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <stdlib.h>
#include "MI125_com_wrapper.h"
#include "carrier_service.h"

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/
static const unsigned short i2c_bridge_spi_speed[] = {1843, 461, 115, 58 };
static unsigned char i2c_bridge_initval;


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/
static Result _MI125_set_spi_speed(MI125_com_core * core, unsigned char speed);
static Result _MI125_com_write_adc(MI125_com_core * core, unsigned short addr, unsigned char data, unsigned char spics);
static Result _MI125_com_write_sensors(MI125_com_core * core, unsigned short addr, unsigned short data);
static Result _MI125_com_write_expander(MI125_com_core * core, unsigned char data);
static Result _MI125_com_write_bridge(MI125_com_core * core, unsigned char addr, unsigned char data);
static Result _MI125_com_write_expander2(MI125_com_core * core, unsigned short addr, unsigned char data);
static Result _MI125_com_read_adc(MI125_com_core * core, unsigned short addr, unsigned char * data, unsigned char spics);
static Result _MI125_com_read_sensors(MI125_com_core * core, unsigned short addr, short * data);
static Result _MI125_com_read_expander2(MI125_com_core * core, unsigned short addr, unsigned char * data);


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : _MI125_set_spi_speed
 ************************************************************************//**
 *
 *   Configure the I2C/SPI bridge for correct SPI devices operation.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] speed
 *      speed of spi clock on the bridge
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_set_spi_speed(MI125_com_core * core, unsigned char speed)
{
    // Speed is only 2 bits (4 values)
    speed &= 0x3;
    core->spi_clock_rate = speed;
    i2c_bridge_initval &= ~0x3;
    i2c_bridge_initval |= speed;
    
    // Configure the spi bridge
    return MI125_com_write(core, MI125_COM_I2C_BRIDGE, 0xF0, &i2c_bridge_initval);
}


 /****************************************************************************
 *   NAME : _MI125_com_write_adc
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to an ADC device.
 *   Use this function between a _MI125_wait_for_i2c_bus_release/_MI125_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] addr
 *      internal device register address where to start writing
 *
 *   @param [in] data
 *      data to write
 *
 *   @param [in] spics
 *      spi cs to use to select the correct adc (MI125_SPI_CS_ADC0,MI125_SPI_CS_ADC1,
 *                                               MI125_SPI_CS_ADC2,MI125_SPI_CS_ADC3)
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_com_write_adc(MI125_com_core * core, unsigned short addr, unsigned char data, unsigned char spics)
{
    unsigned char buffer[3];
    int sentByteCount;
    const int transfer_size = 3;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //function id
    buffer[0] = spics;

    addr &= 0x3F;
    //SPI data
    //short instruction word
    buffer[1] = addr;
    //data
    buffer[2] = data;

    // Write the i2c message to the device
    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, core->bridge_addr, buffer[0], buffer+1, transfer_size-1, options);

    // Check if all bytes where sent correctly
    if (sentByteCount == (transfer_size-1))
    {
        return MI125_COMWRAPPER_OK;
    }else{
        return MI125_COMWRAPPER_FAILED;
    }
}


 /****************************************************************************
 *   NAME : _MI125_com_write_sensors
 ************************************************************************//**
 *
 *   Write up to 1 X 16-bit transfer to a temperature sensor device
 *   (configuration reg needs needs both 8 bits char of the 16 bits been the same).
 *   Use this function between a _MI125_wait_for_i2c_bus_release/_MI125_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] addr
 *      internal device register address where to start writing
 *
 *   @param [in] data
 *      data to write
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_com_write_sensors(MI125_com_core * core, unsigned short addr, unsigned short data)
{
    unsigned char buffer[3];
    int sentByteCount;
    const int transfer_size = 3;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    buffer[0] = addr & 0xFF;
     //data
    buffer[1] = data >> 8;
    buffer[2] = data & 0xFF;

    // Write i2c message
    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, core->sensor_addr, buffer[0], buffer+1, transfer_size-1, options);

    // Check if transfer went correctly
    if (sentByteCount == (transfer_size - 1))
    {
        return MI125_COMWRAPPER_OK;
    }else{
        return MI125_COMWRAPPER_FAILED;
    }
}


 /****************************************************************************
 *   NAME : _MI125_com_write_expander
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to an IO expander device
 *   Use this function between a _MI125_wait_for_i2c_bus_release/_MI125_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] data
 *      data to write
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_com_write_expander(MI125_com_core * core, unsigned char data)
{
    unsigned char buffer[1];
    int sentByteCount;
    const int transfer_size = 1;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    buffer[0] = data;

    // Write the i2c message
    
    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, core->expander_addr, buffer[0], NULL, 0, options);

    // If data was sent correctly
    if (sentByteCount == (transfer_size - 1))
    {
        return MI125_COMWRAPPER_OK;
    }else{
        return MI125_COMWRAPPER_FAILED;
    }
}


 /****************************************************************************
 *   NAME : _MI125_com_write_bridge
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to an I2C/SPI bridge device
 *   Use this function between a _MI125_wait_for_i2c_bus_release/_MI125_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] addr
 *      internal device register address where to start writing
 *
 *   @param [in] data
 *      data to write
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_com_write_bridge(MI125_com_core * core, unsigned char addr, unsigned char data)
{
    unsigned char buffer[2];
    int sentByteCount;
    const int transfer_size = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    buffer[0] = addr;
    buffer[1] = data;

    // Write the I2C message
    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, core->bridge_addr, buffer[0], buffer+1, transfer_size-1, options);

    // Did the sending went correctly?
    if (sentByteCount == (transfer_size-1))
    {
        return MI125_COMWRAPPER_OK;
    }else{
        return MI125_COMWRAPPER_FAILED;
    }
}


 /****************************************************************************
 *   NAME : _MI125_com_write_expander2
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to an 16 bits (2 X 8 bits) io expander.
 *   Use this function between a _MI125_wait_for_i2c_bus_release/_MI125_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] addr
 *      internal device register address where to start writing
 *
 *   @param [in] data
 *      data to write
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_com_write_expander2(MI125_com_core * core, unsigned short addr, unsigned char data)
{
    unsigned char buffer[2];
    int sentByteCount;
    const int transfer_size = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    buffer[0] = addr & 0xff;
    buffer[1] = data;

    // Write the I2C message
    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, core->expander2_addr, buffer[0], buffer+1, transfer_size-1, options);

    // If data was sent correctly
    if (sentByteCount == (transfer_size-1))
    {
        return MI125_COMWRAPPER_OK;
    }else{
        return MI125_COMWRAPPER_FAILED;
    }
}


 /****************************************************************************
 *   NAME : _MI125_com_read_adc
 ************************************************************************//**
 *
 *   Read up to 1 X 8-bit transfer from an ADC device.
 *   Use this function between a _MI125_wait_for_i2c_bus_release/_MI125_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] addr
 *      internal device register address where to start reading
 *
 *   @param [out] data
 *      data read
 *
 *   @param [in] spics
 *      spi cs to use to select the correct adc (MI125_SPI_CS_ADC0,MI125_SPI_CS_ADC1,
 *                                               MI125_SPI_CS_ADC2,MI125_SPI_CS_ADC3)
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_com_read_adc(MI125_com_core * core, unsigned short addr, unsigned char * data, unsigned char spics)
{
    unsigned char buffer[7];
    int sentByteCount, receivedByteCount;
    const int transfer_size = 3;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //function id
    buffer[0] = spics;

    //SPI data
    //short instruction word
    buffer[1] = (addr & 0x3F) | 0x80;
    //data
    buffer[2] = 0xFF;

    // Write the i2c message to prepare the reading
    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, core->bridge_addr, buffer[0], buffer+1, transfer_size-1, options);

    // This the read preparation was ok?
    if (sentByteCount != (transfer_size-1))
    {
        return MI125_COMWRAPPER_FAILED;
    }

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cReadNoWrData(core->hCarrier, core->zone, core->bridge_addr, buffer, transfer_size - 1);

    // Check if data buffer is not empty
    if (data)
        *data = buffer[1];
    else        
        return MI125_COMWRAPPER_INVALIDDATABUF;

    // Check if we read the correct number of byte
    if (receivedByteCount == transfer_size - 1 )
    {
        return MI125_COMWRAPPER_OK;
    }else{
        return MI125_COMWRAPPER_FAILED;
    }
}

 /****************************************************************************
 *   NAME : _MI125_com_read_sensors
 ************************************************************************//**
 *
 *   Read up to 1 X 16-bit transfer from a temperature sensor device
 *   Use this function between a _MI125_wait_for_i2c_bus_release/_MI125_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] addr
 *      internal device register address where to start reading
 *
 *   @param [out] data
 *      data read
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_com_read_sensors(MI125_com_core * core, unsigned short addr, short * data)
{
    unsigned char buffer[5];
    int receivedByteCount;
    const int read_size = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    addr &= 0xFF;

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cRead(core->hCarrier, core->zone, core->sensor_addr, addr, buffer, read_size, options);

    // Check if data buffer is not empty
    if (data)
    {
        *data = buffer[0] << 8 | buffer[1];
        if (addr != 1)
        {
        	// Remove don't care bits
        	*data &= 0xFF80;
        }
    }
    else        
        return MI125_COMWRAPPER_INVALIDDATABUF;
    
    // Check if we read the correct number of byte
    if (receivedByteCount == read_size )
    {
        return MI125_COMWRAPPER_OK;
    }else{
        return MI125_COMWRAPPER_FAILED;
    }
}


 /****************************************************************************
 *   NAME : _MI125_com_read_expander2
 ************************************************************************//**
 *
 *   Read up to 1 X 8-bit transfer from a 16 bits io expander device.
 *   Use this function between a _MI125_wait_for_i2c_bus_release/_MI125_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] addr
 *      internal device register address where to start reading
 *
 *   @param [out] data
 *      data read
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _MI125_com_read_expander2(MI125_com_core * core, unsigned short addr, unsigned char * data)
{
    unsigned char buffer[5];
    int receivedByteCount;
    const int read_size = 1;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    addr &= 0xFF;
    
    receivedByteCount = Carrier_I2cRead(core->hCarrier, core->zone, core->expander2_addr, addr, buffer, read_size, options);

    // Check if data buffer is not empty
    if (data)
    {
        *data = buffer[0];
    }
    else        
        return MI125_COMWRAPPER_INVALIDDATABUF;

    // Check if we read the correct number of byte
    if (receivedByteCount == read_size )
    {
        return MI125_COMWRAPPER_OK;
    }else{
        return MI125_COMWRAPPER_FAILED;
    }
}


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : MI125_com_open
 ************************************************************************//**
 *
 *   Open the com wrapper module before use.
 *   This function must be called before using any other function of this module.
 *
 *   @param [out] core
 *      returns a communication core data structure
 *
 *   @param [in] hCarrier
 *      Handle to the carrier object previously open
 *
 *   @param [in] bridge_addr
 *      7 bits I2C address of I2C/SPI bridge device
 *
 *   @param [in] sensor_addr
 *      7 bits I2C address of temperature sensor device
 *
 *   @param [in] expander_addr
 *      7 bits I2C address of IO expander device (alpha)
 *
 *   @param [in] expander2_addr
 *      7 bits I2C address of 16 bits IO expander device (beta and later)
 *
 *   @param [in] eeprom_addr
 *      7 bits I2C address of eeprom device
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
Result MI125_com_open(MI125_com_core * core, Carrier_handle_t hCarrier, unsigned int zone, unsigned char bridge_addr,
                 unsigned char sensor_addr, unsigned char expander_addr, unsigned char expander2_addr, unsigned char eeprom_addr)
{
    core->hCarrier = hCarrier;
    core->zone = zone;
    core->bridge_addr = bridge_addr;
    core->sensor_addr = sensor_addr;
    core->expander_addr = expander_addr;
    core->expander2_addr = expander2_addr;
    core->eeprom_addr = eeprom_addr;
    
    return MI125_COMWRAPPER_OK;
}


 /****************************************************************************
 *   NAME : MI125_com_init
 ************************************************************************//**
 *
 *   Initialize the com wrapper to a default working condition.
 *   MI125_com_open must be called before using this function.
 *
 *   @param [in] core
 *      structure returns from MI125_com_open function
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
Result MI125_com_init(MI125_com_core * core)
{
    //SPI clock high when idle, data clocked in on trailing edge
    //i2c_bridge_initval = 0x8;

    //SPI clock low when idle, data clocked in on leading edge
    i2c_bridge_initval = 0x0;

    // Configure the spi parameters of the bridge
    return _MI125_set_spi_speed(core, 0);
}


 /****************************************************************************
 *   NAME : MI125_com_close
 ************************************************************************//**
 *
 *   Close the com wrapper module when not needed anymore.
 *   MI125_com_open must be called before using this function.
 *
 *   @param [in] core
 *      structure returns from MI125_com_open function
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
Result MI125_com_close(MI125_com_core * core)
{
    return MI125_COMWRAPPER_OK;
}


 /****************************************************************************
 *   NAME : MI125_com_write_arr
 ************************************************************************//**
 *
 *   Write an array of data to an I2C specified device. Data is MSB 1st with
 *   incrementing address. 
 *   MI125_com_open must be called before using this function.
 *
 *   @param [in] core
 *      structure returns from MI125_com_open function
 *
 *   @param [in] device
 *      which I2C logical device to write
 *
 *   @param [in] addr
 *      at which internal device address starting to write
 *
 *   @param [in] data
 *      data buffer used for writing
 *
 *   @param [in] size
 *      number of bytes to write
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
Result MI125_com_write_arr(MI125_com_core * core, MI125_COM_DEVICES device, unsigned short addr, const void * data, int size)
{
    int ret;
    int i;

    // Which device we need to write by looping one item size at a time (vary according to device)
    switch(device)
    {
        // Write to an ADC device
        case MI125_COM_ADC0:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_write_adc(core, addr+i, ((unsigned char*)data)[i], MI125_SPI_CS_ADC0);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to an ADC device
        case MI125_COM_ADC1:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_write_adc(core, addr+i, ((unsigned char*)data)[i], MI125_SPI_CS_ADC1);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to an ADC device
        case MI125_COM_ADC2:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_write_adc(core, addr+i, ((unsigned char*)data)[i], MI125_SPI_CS_ADC2);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to an ADC device
        case MI125_COM_ADC3:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_write_adc(core, addr+i, ((unsigned char*)data)[i], MI125_SPI_CS_ADC3);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to a temperature sensor device
        case MI125_COM_SENSORS:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_write_sensors(core, addr+i, ((unsigned short*)data)[i]);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to IO expander device
        case MI125_COM_PORTS:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_write_expander(core, ((unsigned char*)data)[i]);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to IO expander16 device
        case MI125_COM_PORTS2:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_write_expander2(core, addr+i, ((unsigned char*)data)[i]);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to an I2C/SPI bridge device
        case MI125_COM_I2C_BRIDGE:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_write_bridge(core, addr+i, ((unsigned char*)data)[i]);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to an eeprom device
        case MI125_COM_EEPROM:
            return MI125_COMWRAPPER_UNIMPLEMENTED;
    }

    return MI125_COMWRAPPER_OK;
}


 /****************************************************************************
 *   NAME : MI125_com_read_arr
 ************************************************************************//**
 *
 *   Read an array of data from an I2C specified device. Data is MSB 1st with
 *   incrementing address. 
 *   MI125_com_open must be called before using this function.
 *
 *   @param [in] core
 *      structure returns from MI125_com_open function
 *
 *   @param [in] device
 *      which I2C logical device to read
 *
 *   @param [in] addr
 *      at which internal device address starting to read
 *
 *   @param [out] data
 *      data buffer used for reading
 *
 *   @param [in] size
 *      number of bytes to read
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
Result MI125_com_read_arr(MI125_com_core * core, MI125_COM_DEVICES device, unsigned short addr, void * data, int size)
{
    int ret;
    int i;

    // select which i2c device type
    switch(device)
    {
        // ADC device type
        case MI125_COM_ADC0:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_read_adc(core, addr+i, &(((unsigned char*)data)[i]), MI125_SPI_CS_ADC0);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // ADC device type
        case MI125_COM_ADC1:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_read_adc(core, addr+i, &(((unsigned char*)data)[i]), MI125_SPI_CS_ADC1);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // ADC device type
        case MI125_COM_ADC2:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_read_adc(core, addr+i, &(((unsigned char*)data)[i]), MI125_SPI_CS_ADC2);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // ADC device type
        case MI125_COM_ADC3:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_read_adc(core, addr+i, &(((unsigned char*)data)[i]), MI125_SPI_CS_ADC3);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Temperature sensor device type
        case MI125_COM_SENSORS:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_read_sensors(core, addr+i, &(((short*)data)[i]));
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Read from io expander16 device
        case MI125_COM_PORTS2:
            for(i=0; i<size; i++)
            {
                ret = _MI125_com_read_expander2(core, addr+i, &(((unsigned char*)data)[i]));
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // This device types are currently not supported
        case MI125_COM_EEPROM:
        case MI125_COM_PORTS:
        case MI125_COM_I2C_BRIDGE:
            return MI125_COMWRAPPER_UNIMPLEMENTED;
    }

    return MI125_COMWRAPPER_OK;
}


 /****************************************************************************
 *   NAME : MI125_com_write
 ************************************************************************//**
 *
 *   Write 1 data item to an I2C specified device (size of data item vary according
 *   to the device).
 *   MI125_com_open must be called before using this function.
 *
 *   @param [in] core
 *      structure returns from MI125_com_open function
 *
 *   @param [in] device
 *      which I2C logical device to write
 *
 *   @param [in] addr
 *      at which internal device address starting to write
 *
 *   @param [in] data
 *      data buffer used for writing
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
Result MI125_com_write(MI125_com_core * core, MI125_COM_DEVICES device, unsigned short addr, const void * data)
{
    // Use the array write routine with an item size of 1
    return MI125_com_write_arr(core, device, addr, data, 1);
}


 /****************************************************************************
 *   NAME : MI125_com_read
 ************************************************************************//**
 *
 *   Read 1 data item from an I2C specified device. Data item size vary according
 *   to the device. 
 *   MI125_com_open must be called before using this function.
 *
 *   @param [in] core
 *      structure returns from MI125_com_open function
 *
 *   @param [in] device
 *      which I2C logical device to read
 *
 *   @param [in] addr
 *      at which internal device address starting to read
 *
 *   @param [out] data
 *      data buffer used for reading
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
Result MI125_com_read(MI125_com_core * core, MI125_COM_DEVICES device, unsigned short addr, void * data)
{
    return MI125_com_read_arr(core, device, addr, data, 1);
}

 /****************************************************************************
 *   NAME : MI125_com_check_raw
 ************************************************************************//**
 *
 *   Check if an i2c device is detected on the i2c bus
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] devaddr
 *      i2c device address to be checked
 *
 *   @param [out] detected
 *      !=0 if the specified device is detected
 *
 *   @return   Error code: MI125_COMWRAPPER_OK if no problem is found, !MI125_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_COM_WRAPPER
 *
 ***************************************************************************/ 
Result MI125_com_check_raw(MI125_com_core * core, unsigned char devaddr, unsigned char * detected)
{
    unsigned char buffer[5];
    int sentByteCount;
    unsigned char addr=0;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    *detected = 0;

    buffer[0] = addr & 0xFF;

    // Write i2c message
    sentByteCount = Carrier_I2cWrite(core->hCarrier, core->zone, devaddr, buffer[0], NULL, 0, options);

    // Did the preparation message was written correctly?
    if (sentByteCount == 0)
        *detected = 1;

    return MI125_COMWRAPPER_OK;
}


