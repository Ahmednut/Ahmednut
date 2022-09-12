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
 * @file       bas_fmc_device_def.h
 * @brief      FMC device defines.
 *
 * $Date: 2015/05/15 14:39:04 $
 * $Revision: 1.1 $
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

#ifndef _BAS_FMC_DEVICE_DEF_H
#define _BAS_FMC_DEVICE_DEF_H

#include "bas_types.h"

/** \file device.h
Device Module API.
*/


/*==========================================================================
==  PUBLIC ENUMERATIONS
==========================================================================*/

/****************************************************************************
 *
 * FMC device access function definition.
 *
 ****************************************************************************/
typedef struct S_FMCBoardDeviceServices
{
    adp_result_t (*Init) (void * Transport, int Add, 
                                  int * DataIn);
    adp_result_t (*ReadRegisters) (void * Transport, int Add, 
                                    int BaseReg, int NbReg, char * DataIn);
    adp_result_t (*WriteRegisters) (void * Transport, int Add,
                                    int BaseReg, int NbReg, char * DataIn);
    adp_result_t (*UpdateConfigration) (void * Transport, int Add, 
                                        void * Config);
    adp_result_t (*ReadConfigration) (void * Transport, int Add, 
                                        void * Config);
    adp_result_t (*WriteConfigration) (void * Transport, int Add, 
                                        void * Config);
}FMCBoardDeviceServices;

/****************************************************************************
 *
 * FMC on board device instance definition
 *
 ****************************************************************************/
typedef struct S_FMCBoardDevice
{
    int    Address;                        //(SPI is Chip select,  I2C is an address)
    FMCBoardDeviceServices *Services;
    void * TransportDriver;
}FMCBoardDevice;





#endif
