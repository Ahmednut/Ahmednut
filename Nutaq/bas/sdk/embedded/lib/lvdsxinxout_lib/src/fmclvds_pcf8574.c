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
 * @file       FMCLVDS_pcf8574.c
 * @brief      IO expander functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCLVDS
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
#include "fmclvds_pcf8574.h"
#include "fmclvds_pcf8574_defines.h"
#include "fmclvds_com_wrapper_defines.h"


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
 *   NAME : FMCLVDS_pcf8574_open(FMCLVDS_pcf8574_core * core, FMCLVDS_com_core * com)
 ************************************************************************//**
 *
 *   Open the pcf8574 io expander module
 *   This function must be the first one used before using this module.
 *   When the module is not used anymore, it can be closed with FMCLVDS_pcf8574_close.
 *   The com_wrapper module must be opened before using this module.
 *
 *   @param [out] core
 *      This structure will hold necessary information for the module to be used
 *
 *   @param [in] com
 *      This structure must hold communication module information
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
int FMCLVDS_pcf8574_open(FMCLVDS_pcf8574_core * core, FMCLVDS_com_core * com)
{
    // Save the com module information
    core->com = com;
    
    // Preconfigure the register cache because, it could not be read back
    // The device returns the inputs instead of last written output value when read
    // So the cache register will be used to hold the last written value to the device
    core->regcache.Register_00.raw = 0xFF;  // At powerup, starts in input mode  
    
    return FMCLVDS_PCF8574OK;
}


 /****************************************************************************
 *   NAME : FMCLVDS_pcf8574_reset(FMCLVDS_pcf8574_core * core)
 ************************************************************************//**
 *
 *   Reset the pcf8574 module to default operating condition
 *   This function must be called at least once to reset this module to default
 *   operating condition.
 *   The module must be opened with FMCLVDS_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with FMCLVDS_pcf8574_open
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
int FMCLVDS_pcf8574_reset(FMCLVDS_pcf8574_core * core)
{
    Result ret;
    FMCLVDS_pcf8574_addr caddr;

    // Set to default normal operation value
    caddr = FMCLVDS_PCF8574_00;
    core->regcache.Register_00.raw = FMCLVDS_PCF8574_DEFAULT00;  

    // This will reset all register bits to default value
    ret = FMCLVDS_com_write(core->com, FMCLVDS_COM_PORTS, caddr, &core->regcache.Register_00.raw);
    
    return ret;
}


 /****************************************************************************
 *   NAME : FMCLVDS_pcf8574_close(FMCLVDS_pcf8574_core * core)
 ************************************************************************//**
 *
 *   Close the pcf8574 io expander module
 *   This function must be the last one used after using this module.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with FMCLVDS_pcf8574_open
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
Result FMCLVDS_pcf8574_close(FMCLVDS_pcf8574_core * core)
{
    return FMCLVDS_PCF8574OK;
}


 /****************************************************************************
 *   NAME : FMCLVDS_pcf8574_powerdown(FMCLVDS_pcf8574_core * core, FMCLVDS_GROUPS actchannel, FMCLVDS_pcf8574_powerdown)
 ************************************************************************//**
 *
 *   Configure ADC shutdown mode according to specified active channels
 *   The module must be opened with FMCLVDS_pcf8574_open before using this function.
 *
 *   @param [in] core
 *      This structure is the one returned at module opening with FMCLVDS_pcf8574_open
 *
 *   @param [in] group
 *  	Channel group to be put to powerdon or powerup
 *
 *   @return   Error code: FMCLVDS_PCF8574OK if no problem is found, !FMCLVDS_PCF8574OK if problem
 *             Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 *   @ingroup FMCLVDS_PCF8574
 *
 ***************************************************************************/ 
Result FMCLVDS_pcf8574_powerdown(FMCLVDS_pcf8574_core * core, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_pwdn powerdown)
{
	Result ret;
    FMCLVDS_pcf8574_addr caddr;
    // Configure which channel are active (the others are in shutdown)
    caddr = FMCLVDS_PCF8574_00;
	if(powerdown == FMCLVDS_PCF8574_POWERUP)
	{
		if(group == FMCLVDS_GROUP_0)
		{
			core->regcache.Register_00.Bit.powerdn |= (0x3);
		}
		else
		{
			core->regcache.Register_00.Bit.powerdn |= (0xC);
		}
		ret = FMCLVDS_com_write(core->com, FMCLVDS_COM_PORTS, caddr, &core->regcache.Register_00.raw);
	}
	else
	{
		if(group == FMCLVDS_GROUP_0)
		{
			core->regcache.Register_00.Bit.powerdn &= ~(0x3);
		}
		else
		{
			core->regcache.Register_00.Bit.powerdn &= ~(0xC);
		}
		ret = FMCLVDS_com_write(core->com, FMCLVDS_COM_PORTS, caddr, &core->regcache.Register_00.raw);
	}
    return FMCLVDS_PCF8574OK;
}

Result FMCLVDS_pcf8574_preemphasis(FMCLVDS_pcf8574_core * core, FMCLVDS_GROUPS group, FMCLVDS_pcf8574_preemp preemphasis)
{
	Result ret;
    FMCLVDS_pcf8574_addr caddr;
    // Configure which channel are active (the others are in shutdown)
    caddr = FMCLVDS_PCF8574_00;
	if(preemphasis == FMCLVDS_PCF8574_PREEMPHASISON)
	{
		if(group == FMCLVDS_GROUP_0)
		{
			core->regcache.Register_00.Bit.preemphasis |= (0x3);
		}
		else
		{
			core->regcache.Register_00.Bit.preemphasis |= (0xC);
		}
		ret = FMCLVDS_com_write(core->com, FMCLVDS_COM_PORTS, caddr, &core->regcache.Register_00.raw);
	}
	else
	{
		if(group == FMCLVDS_GROUP_0)
		{
			core->regcache.Register_00.Bit.preemphasis &= ~(0x3);
		}
		else
		{
			core->regcache.Register_00.Bit.preemphasis &= ~(0xC);
		}
		ret = FMCLVDS_com_write(core->com, FMCLVDS_COM_PORTS, caddr, &core->regcache.Register_00.raw);
	}
    return FMCLVDS_PCF8574OK;
}



