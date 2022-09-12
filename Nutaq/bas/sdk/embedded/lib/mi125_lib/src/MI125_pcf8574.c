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
 * @file       MI125_pcf8574.c
 * @brief      IO expander functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI125
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "MI125_pcf8574.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

// Default register cache value
#define MI125_PCF8574_DEFAULT00 0xF8        // No shutdown, TrigOut disable, clk src int 125 MHz


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
 *   NAME : MI125_pcf8574_open(MI125_pcf8574_core * core, MI125_com_core * com)
 ************************************************************************//**
 *
 *   Open the pcf8574 io expander module
 *   This function must be the first one used before using this module.
 *   When the module is not used anymore, it can be closed with MI125_pcf8574_close.
 *   The com_wrapper module must be opened before using this module.
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_open(MI125_pcf8574_core * core, MI125_com_core * com)
{
    // Save the com module information
    core->com = com;
    
    // Preconfigure the register cache because, it could not be read back
    // The device returns the inputs instead of last written output value when read
    // So the cache register will be used to hold the last written value to the device
    core->regcache.Register_00.raw = 0xFF;  // At powerup, starts in input mode  
    
    return MI125_PCF8574OK;
}


 /****************************************************************************
 *   NAME : MI125_pcf8574_reset(MI125_pcf8574_core * core)
 ************************************************************************//**
 *
 *   Reset the pcf8574 module to default operating condition
 *   This function must be called at least once to reset this module to default
 *   operating condition.
 *   The module must be opened with MI125_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_reset(MI125_pcf8574_core * core)
{
    Result ret;
    MI125_pcf8574_addr caddr;

    // Set to default normal operation value
    caddr = MI125_PCF8574_00;
    core->regcache.Register_00.raw = MI125_PCF8574_DEFAULT00;  

    // This will reset all register bits to default value
    ret = MI125_com_write(core->com, MI125_COM_PORTS, caddr, &core->regcache.Register_00.raw);
    
    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pcf8574_close(MI125_pcf8574_core * core)
 ************************************************************************//**
 *
 *   Close the pcf8574 io expander module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_close(MI125_pcf8574_core * core)
{
    return MI125_PCF8574OK;
}


 /****************************************************************************
 *   NAME : MI125_pcf8574_setshutdown(MI125_pcf8574_core * core, MI125_pcf8574_channel actchannel)
 ************************************************************************//**
 *
 *   Configure ADC shutdown mode according to specified active channels
 *   The module must be opened with MI125_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @param [in] actchannel
 *      This is the number of 4 channels group not in shutdown (active).
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_setshutdown(MI125_pcf8574_core * core, MI125_pcf8574_channel actchannel)
{
    Result ret;
    MI125_pcf8574_addr caddr;

    // Configure which channel are active (the others are in shutdown)
    caddr = MI125_PCF8574_00;
    core->regcache.Register_00.Bit.adc_shdn = (actchannel & 0xF);
    ret = MI125_com_write(core->com, MI125_COM_PORTS, caddr, &core->regcache.Register_00.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pcf8574_settrgout(MI125_pcf8574_core * core, MI125_pcf8574_trigout mode)
 ************************************************************************//**
 *
 *   Configure if the trigger out signal is connected or not from fpga to outside.
 *   The module must be opened with MI125_pcf8574_open before using this function.
 *   By default, the trigger out signal is disconnected from outside. 
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @param [in] mode
 *      Specify if the trigger out signal is connected (MI125_PCF8574_TRIGOUTON)
 *      or not (MI125_PCF8574_TRIGOUTOFF).
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_settrgout(MI125_pcf8574_core * core, MI125_pcf8574_trigout mode)
{
    Result ret;
    MI125_pcf8574_addr caddr;

    // Set the trigger out mode
    caddr = MI125_PCF8574_00;
    core->regcache.Register_00.Bit.trig_out_en = (mode & 0x1);
    ret = MI125_com_write(core->com, MI125_COM_PORTS, caddr, &core->regcache.Register_00.raw);

    return ret;
}


 /****************************************************************************
 *   NAME : MI125_pcf8574_setclksource(MI125_pcf8574_core * core, MI125_pcf8574_clksrc clksrc)
 ************************************************************************//**
 *
 *   Configure the ADC clock source
 *   The module must be opened with MI125_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with MI125_pcf8574_open
 *
 *   @param [in] clksrc
 *      Specify which clock source the ADCs will used
 *
 *   @return   Error code: MI125_PCF8574OK if no problem is found, !MI125_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup MI125_PCF8574
 *
 ***************************************************************************/ 
Result MI125_pcf8574_setclksource(MI125_pcf8574_core * core, MI125_pcf8574_clksrc clksrc)
{
    Result ret;
    MI125_pcf8574_addr caddr;

    // Configure the adc clock source
    caddr = MI125_PCF8574_00;
    core->regcache.Register_00.Bit.adc_clk_src = (clksrc & 0x7);
    ret = MI125_com_write(core->com, MI125_COM_PORTS, caddr, &core->regcache.Register_00.raw);

    return ret;
}
