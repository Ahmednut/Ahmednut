/****************************************************************************  
 *
 *    ****                              I                                    
 *   ******                            ***                                   
 *   *******                           ****                                  
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    ****I  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       MI125_com_wrapper.h
 * @brief      I2C communication wrapper definitions for all devices control
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
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
#ifndef __MI125_COM_WRAPPER__
#define __MI125_COM_WRAPPER__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "bas_error.h"
#include "MI125_com_wrapper_defines.h"


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
 *   @param [in] zone
 *      I2C zone to access the MI125 devices
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
                 unsigned char sensor_addr, unsigned char expander_addr, unsigned char expander2_addr, unsigned char eeprom_addr);


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
Result MI125_com_init(MI125_com_core * core);


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
Result MI125_com_close(MI125_com_core * core);


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
Result MI125_com_write_arr(MI125_com_core * core, MI125_COM_DEVICES device, unsigned short addr, const void * data, int size);


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
Result MI125_com_read_arr(MI125_com_core * core, MI125_COM_DEVICES device, unsigned short addr, void * data, int size);


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
Result MI125_com_write(MI125_com_core * core, MI125_COM_DEVICES device, unsigned short addr, const void * data);


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
Result MI125_com_read(MI125_com_core * core, MI125_COM_DEVICES device, unsigned short addr, void * data);


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
Result MI125_com_check_raw(MI125_com_core * core, unsigned char devaddr, unsigned char * detected);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

