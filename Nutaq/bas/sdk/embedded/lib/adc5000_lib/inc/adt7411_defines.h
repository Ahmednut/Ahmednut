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
 * @file      adt7411_defines.h
 * 
 * @brief     adt7411 defines
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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

#ifndef __ADT7411_DEFINES__
#define __ADT7411_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

#include "com_wrapper.h"

#define ADT7411_EXT_TEMPR_SELECT    2
#define ADT7411_START_MONITORING    1
#define ADT7411_STOP_MONITORING     0

typedef enum E_ad7411_sensor {
    AD7411_SENSOR_INTERNAL_TEMP,
    AD7411_SENSOR_EXTERNAL_TEMP,
    AD7411_SENSOR_VDD,
    AD7411_SENSOR_AIN1,
    AD7411_SENSOR_AIN2,
    AD7411_SENSOR_AIN3,
    AD7411_SENSOR_AIN4,
    AD7411_SENSOR_AIN5,
    AD7411_SENSOR_AIN6,
    AD7411_SENSOR_AIN7,
    AD7411_SENSOR_AIN8,
} ad7411_sensor;

typedef enum E_adt7411_state {
    ADT7411_DISABLE,
    ADT7411_ENABLE,
} adt7411_state;

typedef struct S_adt7411_core {
    com_core * com;
} adt7411_core;

enum adt7411_error {
    ADT7411OK     = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_ADT7411, 0),
    ADT7411UNIMPLEMENTED  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADT7411, 1), /**> This feature is not implemented */
    ADT7411IVAL   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADT7411, 2),         /**> Invalid parameters */
};

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

