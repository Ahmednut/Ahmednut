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
 * @file      ev10aq190.h
 * 
 * @brief     ev10aq190 functions
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

#ifndef __EV10AQ10_HEADERS__
#define __EV10AQ10_HEADERS__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

#include "bas_error.h"
#include "ev10aq190_defines.h"
#include "ev10aq190_regs.h"

Result ev10aq190_open(ev10aq190_core * core, com_core * com);
Result ev10aq190_reset(ev10aq190_core * core);
Result ev10aq190_close(ev10aq190_core * core);
Result ev10aq190_getstatus(ev10aq190_core * core, unsigned char * adc_A_active, unsigned char * adc_B_active, unsigned char * adc_C_active, unsigned char * adc_D_active);
Result ev10aq190_settestmode(ev10aq190_core * core, ev10aq190_testmode state);
Result ev10aq190_setmode(ev10aq190_core * core, ev10aq190_channel mode);
Result ev10aq190_setstandby(ev10aq190_core * core, ev10aq190_standby standby);
Result ev10aq190_setcoding(ev10aq190_core * core, ev10aq190_coding coding);
Result ev10aq190_setbandwidth(ev10aq190_core * core, ev10aq190_bandwidth bandwidth);
Result ev10aq190_offset(ev10aq190_core * core, ev10aq190_channel channel, unsigned short offset);
Result ev10aq190_gain(ev10aq190_core * core, ev10aq190_channel channel, unsigned short gain);
Result ev10aq190_phase(ev10aq190_core * core, ev10aq190_channel channel, unsigned short phase);

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

