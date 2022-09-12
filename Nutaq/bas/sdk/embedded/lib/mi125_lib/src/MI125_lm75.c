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
 * @file       MI125_lm75.c
 * @brief      Temperature monitoring functions
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
#include "MI125_lm75.h"

 
/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : MI125_lm75_open(MI125_lm75_core * core, MI125_com_core * com)
 ************************************************************************//**
 *
 *   Open the lm75 monitoring module
 *   This function must be the first one used before using this module.
 *   When the module is not used anymore, it can be closed with MI125_lm75_close.
 *   The com_wrapper module must be opened before using this module. 
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: MI125_LM75OK if no problem is found, !MI125_LM75OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LM75
 *
 ***************************************************************************/ 
Result MI125_lm75_open(MI125_lm75_core * core, MI125_com_core * com)
{
    core->com = com;
    
    return MI125_LM75OK;
}


 /****************************************************************************
 *   NAME : MI125_lm75_reset(MI125_lm75_core * core)
 ************************************************************************//**
 *
 *   Reset the lm75 monitoring module to default operating condition
 *   This function must be called at least once to reset this module to default
 *   operating condition.
 *   The module must be opened with MI125_lm75_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_lm75_open
 *
 *   @return   Error code: MI125_LM75OK if no problem is found, !MI125_LM75OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LM75
 *
 ***************************************************************************/ 
Result MI125_lm75_reset(MI125_lm75_core * core)
{
    Result ret;
    MI125_lm75_addr caddr;
    MI125_lm75_Register_Map reg={{{0x0000,0x0000,0x4b00,0x5000}}}; // default powerup lm75 reg values

    // Does not write LM75_TEMP (1st register) which is read only.
    for (caddr=MI125_LM75_CONF; caddr<=MI125_LM75_TOS; caddr++)
    {
        // This will reset all register bits to default lm75 power value
        ret = MI125_com_write(core->com, MI125_COM_SENSORS, caddr, &reg.RegisterArray[caddr]);
        
        // Check if we got a problem
        if(FAILURE(ret)) break;
    }
    
    return ret;
}


 /****************************************************************************
 *   NAME : MI125_lm75_close(MI125_lm75_core * core)
 ************************************************************************//**
 *
 *   Close the lm75 monitoring module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_lm75_open
 *
 *   @return   Error code: MI125_LM75OK if no problem is found, !MI125_LM75OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LM75
 *
 ***************************************************************************/ 
Result MI125_lm75_close(MI125_lm75_core * core)
{
    return MI125_LM75OK;
}


 /****************************************************************************
 *   NAME : MI125_lm75_readtemp10x(MI125_lm75_core * core, short* temp10x)
 ************************************************************************//**
 *
 *   Read the lm75 temperature with a resolution of 0.1 Celsius.
 *   The temperature returned is of integer type and multiplied X10 to
 *   allow keeping 0.1C resolution without using floating point.  
 *   The module must be opened with MI125_lm75_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_lm75_open
 *
 *   @param [out] temp10x
 *      Return the temperature read (0.1C resolution) X 10
 *
 *   @return   Error code: MI125_LM75OK if no problem is found, !MI125_LM75OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_LM75
 *
 ***************************************************************************/ 
Result MI125_lm75_readtemp10x(MI125_lm75_core * core, short* temp10x)
{
    Result ret;
    MI125_lm75_addr caddr;
    short data;

    // Read the temperature
    caddr = MI125_LM75_TEMP;
    ret = MI125_com_read(core->com, MI125_COM_SENSORS, caddr, &data);

    // Check if we got an error?
    if(FAILURE(ret))
    {
        // Make sure we returns a temperature outside operating range of the sensor
        // to clearly show we are in problem
        *temp10x = MI125_LM75_INVALIDTEMP;
    }
    else
    {
        // Data value is in the form nnnnnnnn.f0000000 (binary fix point compl2), the resolution
        // is 0.5C. This value is converted to 10 X temperature to avoid using floating point.
        // Ex. 22.5C would give temp10x = 225. This value could be divided by 10 later to get
        // 1C resolution (through away the 0.5C info)
        *temp10x = (data / 128) * 5;
    }

    return ret;
}
