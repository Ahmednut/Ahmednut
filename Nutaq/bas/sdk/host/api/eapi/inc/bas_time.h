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
 * @file      LYR_time.h
 * 
 * @brief     Nutaq time defines and functions
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


#ifndef __LYR_TIME__
#define __LYR_TIME__

#ifdef __cplusplus
extern "C" {
#endif

#include "bas_error.h"
enum lyr_clock_error {
  CLOCKOK = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_TIME, 0),
  CLOCKFAILURE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_TIME, 1),
  CLOCKNOTSUPPORTED = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_TIME, 2),
  CLOCKINVALIDCOUNTER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_TIME, 3),
  CLOCKNOTSET = MAKE_RESULT(LYR_SEVERITY_WARNING, MODULE_TIME, 4),
};
#ifdef _MSC_VER
#include <Windows.h>
typedef struct {
    DWORD begin;
    DWORD end;
} lyr_delay_counter;
#else
#include <time.h>
typedef struct {
    struct timespec begin;
    struct timespec end;
} lyr_delay_counter;
#endif
Result lyr_counter_reset(lyr_delay_counter * counter);
Result lyr_counter_begin(lyr_delay_counter * counter);
Result lyr_counter_end(lyr_delay_counter * counter);
Result lyr_counter_get(lyr_delay_counter * counter, double * delay);
Result lyr_counter_resolution(lyr_delay_counter * counter, double * resolution);
double calculate_speed(double delay, unsigned size);

#ifdef __cplusplus
}
#endif

#endif




