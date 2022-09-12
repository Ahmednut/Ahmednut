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
 * @file      com_wrapper.h
 * 
 * @brief     com_wrapper functions
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

#ifndef __COM_WRAPPER__
#define __COM_WRAPPER__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

#include "bas_error.h"
#include "com_wrapper_defines.h"

Result com_open(com_core * core, Carrier_handle_t hCarrier, unsigned int zone);
Result com_init(com_core * core, unsigned char speed, unsigned char mode);
Result com_close(com_core * core);

Result com_write(com_core * core, COM_DEVICES device, unsigned short addr, const void * data);
Result com_read(com_core * core, COM_DEVICES device, unsigned short addr, void * data);
Result com_write_arr(com_core * core, COM_DEVICES device, unsigned short addr, const void * data, int size);
Result com_read_arr(com_core * core, COM_DEVICES device, unsigned short addr, void * data, int size);

enum i2c_error {
    I2C_OK = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_I2C, 0),
    I2C_ERROR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_I2C, 1),
    I2C_UNIMPLEMENTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_I2C, 2),
    I2C_TOOLARGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_I2C, 3), /**< Transfer size is too large */
    I2C_FAILED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_I2C, 4), /**< Error during I2C transfer */
    I2C_TIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_I2C, 5),
};

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

