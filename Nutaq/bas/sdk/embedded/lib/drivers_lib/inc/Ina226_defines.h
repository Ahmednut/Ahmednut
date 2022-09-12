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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       Ina226_defines.h
 * @brief      Power monitoring data and structure definitions
 *
 * $Date: 2015/10/05 15:51:27 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.        
 *
 * @ingroup DRIVERS      
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

#ifndef INA226_DEFINES_H__
#define INA226_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <stdlib.h>


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/
  
typedef struct Ina226_stIna226 * Ina226_hIna226;

// List of Ina226 mode
typedef enum Ina226_eMode {
    eIna226ModePowerDown=0,
    eIna226ModeTrigger=3,
    eIna226ModeContinuous=7
} Ina226_eMode;

// List of Ina226 averaging mode
typedef enum Ina226_eAveraging {
    eIna226Averaging1=0,
    eIna226Averaging4=1,
    eIna226Averaging16=2,
    eIna226Averaging64=3,
    eIna226Averaging128=4,
    eIna226Averaging256=5,
    eIna226Averaging512=6,
    eIna226Averaging1024=7
} Ina226_eAveraging;

// List of Ina226 conversion time
typedef enum Ina226_eConvTime {
    eIna226ConvTime140us=0,
    eIna226ConvTime204us=1,
    eIna226ConvTime332us=2,
    eIna226ConvTime588us=3,
    eIna226ConvTime1100us=4,
    eIna226ConvTime2116us=5,
    eIna226ConvTime4156us=6,
    eIna226ConvTime8244us=7
} Ina226_eConvTime;


enum Ina226_eError {
    eIna226Ok       = MAKE_RESULT(eBasSeveritySuccess,  MODULE_INA226, 0),
    eIna226InvMode  = MAKE_RESULT(eBasSeverityError,    MODULE_INA226, 1),
    eIna226Timeout  = MAKE_RESULT(eBasSeverityError,    MODULE_INA226, 2),
    eIna226InvArg   = MAKE_RESULT(eBasSeverityError,    MODULE_INA226, 3)
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

