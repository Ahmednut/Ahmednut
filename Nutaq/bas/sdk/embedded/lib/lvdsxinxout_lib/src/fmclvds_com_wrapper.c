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
 * @file       fmclvds_com_wrapper.c
 * @brief      I2C communication wrapper for all devices control
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCLVDS
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
#include <stdio.h>
#include "fmclvds_com_wrapper.h"
#include "carrier_service.h"
#include "carrier_service_def.h"


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
static Result _FMCLVDS_com_write_expander(FMCLVDS_com_core * core, unsigned char data);


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/

 /****************************************************************************
 *   NAME : _FMCLVDS_com_write_expander(FMCLVDS_com_core * core, unsigned char data)
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to an IO expander device
 *   Use this function between a _FMCLVDS_wait_for_i2c_bus_release/_FMCLVDS_giveback_i2c_bus call.
 *
 *   @param [in] core
 *      communication core specified at module opening
 *
 *   @param [in] data
 *      data to write
 *
 *   @return   Error code: FMCLVDS_COMWRAPPER_OK if no problem is found, !FMCLVDS_COMWRAPPER_OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_COM_WRAPPER
 *
 ***************************************************************************/ 
static Result _FMCLVDS_com_write_expander(FMCLVDS_com_core * core, unsigned char data)
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
        return FMCLVDS_COMWRAPPER_OK;
    }else{
        return FMCLVDS_COMWRAPPER_FAILED;
    }
}

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

Result FMCLVDS_com_open(FMCLVDS_com_core * core, Carrier_handle_t hCarrier, unsigned int zone, unsigned char expander_addr)
{
    core->hCarrier = hCarrier;
    core->zone = zone;
    core->expander_addr = expander_addr;
    
    return FMCLVDS_COMWRAPPER_OK;
}


Result FMCLVDS_com_close(FMCLVDS_com_core * core)
{
    return FMCLVDS_COMWRAPPER_OK;
}


Result FMCLVDS_com_write_arr(FMCLVDS_com_core * core, FMCLVDS_COM_DEVICES device, unsigned short addr, const void * data, int size)
{
    int ret;
    int i;

    // Which device we need to write by looping one item size at a time (vary according to device)
    switch(device)
    {
        // Write to an ADC device
      
        // Write to IO expander device
        case FMCLVDS_COM_PORTS:
            for(i=0; i<size; i++)
            {
                ret = _FMCLVDS_com_write_expander(core, ((unsigned char*)data)[i]);
                if(FAILURE(ret)) {return ret;}
            }
            break;
    
        // Write to an eeprom device
        case FMCLVDS_COM_EEPROM:
            return FMCLVDS_COMWRAPPER_UNIMPLEMENTED;
    }

    // Release I2C bus ownership
    return FMCLVDS_COMWRAPPER_OK;
}


Result FMCLVDS_com_read_arr(FMCLVDS_com_core * core, FMCLVDS_COM_DEVICES device, unsigned short addr, void * data, int size)
{
    // select which i2c device type
    switch(device)
    {
          
        // This device types are currently not supported
        case FMCLVDS_COM_EEPROM:
        case FMCLVDS_COM_PORTS:
            return FMCLVDS_COMWRAPPER_UNIMPLEMENTED;
    }

    // Release bus ownership
    return FMCLVDS_COMWRAPPER_OK;
}


Result FMCLVDS_com_write(FMCLVDS_com_core * core, FMCLVDS_COM_DEVICES device, unsigned short addr, const void * data)
{
    // Use the array write routine with an item size of 1
    return FMCLVDS_com_write_arr(core, device, addr, data, 1);
}


Result FMCLVDS_com_read(FMCLVDS_com_core * core, FMCLVDS_COM_DEVICES device, unsigned short addr, void * data)
{
    return FMCLVDS_com_read_arr(core, device, addr, data, 1);
}
