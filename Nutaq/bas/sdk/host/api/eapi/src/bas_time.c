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
 * @file      bas_time.c
 * 
 * @brief     Function definitions for BAS time
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
#include "bas_time.h"
#include <memory.h>
#define NSEC_PER_SEC    1000000000L
#define MSEC_PER_SEC    1000L

Result lyr_counter_reset(lyr_delay_counter * counter)
{
    memset(counter, 0, sizeof(*counter));
    return CLOCKOK;
}

#ifdef _MSC_VER

Result lyr_counter_begin(lyr_delay_counter * counter)
{
    counter->begin = timeGetTime();
    return CLOCKOK;
}
Result lyr_counter_end(lyr_delay_counter * counter)
{
    counter->end = timeGetTime();
    return CLOCKOK;
}
Result lyr_counter_get(lyr_delay_counter * counter, double * delay)
{
    DWORD delta;
    if(!counter->begin || !counter->end)
        return CLOCKNOTSET;
    delta = counter->end - counter->begin;
    *delay = ((double)delta)/((double)MSEC_PER_SEC);
    return CLOCKOK;
}
Result lyr_counter_resolution(lyr_delay_counter * counter, double * resolution)
{
    *resolution = ((double)1)/((double)MSEC_PER_SEC);
    return CLOCKOK;
}
#else
#include <time.h>
#include <errno.h>
#include <memory.h>

static struct timespec timespec_sub(struct timespec lhs, struct timespec rhs)
{
  struct timespec ts_delta;
  ts_delta.tv_sec = lhs.tv_sec - rhs.tv_sec;
  ts_delta.tv_nsec = lhs.tv_nsec - rhs.tv_nsec;
  while (ts_delta.tv_nsec < 0){
    ts_delta.tv_sec--;
    ts_delta.tv_nsec += NSEC_PER_SEC;
  }
  return ts_delta;
}

static double timespec2double(struct timespec delta)
{
  double fdelta;
  fdelta = (double)delta.tv_sec;
  fdelta += ((double)delta.tv_nsec) / ((double)NSEC_PER_SEC);
  return fdelta;
}

static Result get_clock(struct timespec * time)
{
    int ret;
    Result result = CLOCKOK;
    ret = clock_gettime(CLOCK_MONOTONIC, time);
    if (ret<0)
    {
        switch(errno){
            case EINVAL:
                result = CLOCKNOTSUPPORTED;
                break;
            case EFAULT:
                result = CLOCKINVALIDCOUNTER;
                break;
            default:
                result = CLOCKFAILURE;
                break;
        }
    }
    return result;
}
Result lyr_counter_begin(lyr_delay_counter * counter)
{
    return get_clock(&counter->begin);
}
Result lyr_counter_end(lyr_delay_counter * counter)
{
    return get_clock(&counter->end);
}
Result lyr_counter_get(lyr_delay_counter * counter, double * delay)
{
    struct timespec delta;
    if((!counter->begin.tv_nsec && !counter->begin.tv_sec) ||
        (!counter->end.tv_nsec && !counter->end.tv_sec))
        return CLOCKNOTSET;
    delta = timespec_sub(counter->end, counter->begin);
    *delay = timespec2double(delta);
    return CLOCKOK;
}
Result lyr_counter_resolution(lyr_delay_counter * counter, double * resolution)
{
    struct timespec res;
    int ret;
    ret = clock_getres(CLOCK_MONOTONIC, &res);
    *resolution = timespec2double(res);
    if (ret<0)
        return CLOCKFAILURE;
    else
        return CLOCKOK;
}

#endif

/* return bytes/second */
double calculate_speed(double delay, unsigned size)
{
  double fsize = (double)size;//bytes
  return fsize/delay;
}
