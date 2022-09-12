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
 * @file    mi250.h
 * @brief   MI250 core registers and functions
 *
 * $Date: 2015/07/09 19:21:26 $
 * $Revision: 1.4 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI250
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

/** @defgroup MI250 MI250 Module
 *
 *  MI250 module allows to control a 8-channels, 14 bits 250 MSPS A/D FMC module.
 *
 *  @file
 *
 */


#ifndef __MI250_H
#define __MI250_H

#include "bas_types.h"
#include "mi250_types.h"
#include "bas_fmc_device_def.h"
#include "bas_spi.h"
#include "FMCEEprom_def.h"


#ifdef __cplusplus
    extern "C" {
#endif


/****************************************************************************
* Function: MI250_Open
*****************************************************************************
*
* Opens a MI250 instance
*
* @param[in]   MI250BaseAdd
*      Base address on the AXI bus of the MI250 FPGA core
*
* @param[in]   pfUsleep
*      Pointer to a sleep function that sleep during N microsecond.
*      This function is the one that will be used inside the library functions.
*
* @param[out]   MI250Board
*		Handle to the open MI250 instance
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
****************************************************************************/
adp_result_t  MI250_Open(unsigned int MI250BaseAdd,
                          int (*pfUsleep)(unsigned int usec),
                          MI250Board_t ** MI250Board);

/****************************************************************************
* Function: MI250_WriteSPIRegisters
*****************************************************************************
*
* Writes an array of values in the register of an SPI device on the MI250.
*
* @param[in]   MI250Board
*      Pointer to an S_MI250Board structure describing the device to use.
*
* @param[in]   DeviceId
*      Target SPI device.
*
* @param[in]   BaseRegister
*      Base register address where to write.
*
* @param[in]   Nb
*      Number of registers to write. This value should be equal or less than
*      the size of the array pointed by Value.
*
* @param[in]   Value
*      Pointer to an array that contains the values to write.
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
****************************************************************************/
adp_result_t  MI250_WriteSPIRegisters(MI250Board_t * MI250Board,
                                    MI250_DEVICES DeviceId,
                                    int BaseRegister, int Nb, char * Value);

/****************************************************************************
* Function: MI250_ReadSPIRegisters
*****************************************************************************
*
* Reads an array of values from the register an MI250 onboard SPI device.
*
* @param[in]   MI250Board
*      Pointer to an S_MI250Board structure describing the device to use.
*
* @param[in]   DeviceId
*      Target SPI device.
*
* @param[in]   BaseRegister
*      Base register address from which to read.
*
* @param[in]   Nb
*      Number of registers to read. This value should be equal or less than
*      the size of the array pointed to by Value.
*
* @param[out]   Value
*      Pointer to an array that contains the values read. The array must
*      at least be as large as the Nb value.
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
****************************************************************************/
adp_result_t  MI250_ReadSPIRegisters(MI250Board_t * MI250Board,
                                    MI250_DEVICES DeviceId,
                                    int BaseRegister, int Nb, char * Value);


/****************************************************************************
* Function: MI250_WriteControlRegister
*****************************************************************************
*
* Writes a value in a control register.
*
* @param[in]   MI250Board
*      Pointer to an S_MI250Board structure describing the device to use.
*
* @param[in]   Register
*      Register ID.
*
* @param[in]   RegisterValue
*      Value to write in the register.
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
****************************************************************************/
adp_result_t  MI250_WriteControlRegister(MI250Board_t * MI250Board,
                                         MI250_CONTROL_REGISTER Register,
                                         int RegisterValue);

/****************************************************************************
* Function: MI250_ReadControlRegister
*****************************************************************************
*
* Reads a value from a control register.
*
* @param[in]   MI250Board
*      Pointer to an S_MI250Board structure describing the device to use.
*
* @param[in]   Register
*      Register ID.
*
* @param[out]   RegisterValue
*      Pointer to an integer that contains the value read.
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
****************************************************************************/
adp_result_t  MI250_ReadControlRegister(MI250Board_t * MI250Board,
                                          int Register, int * RegisterValue);

/****************************************************************************
* Function: MI250_Close
*****************************************************************************
*
* Closes an instance of the MI250.
*
* @param[in]   MI250Board
*      Pointer to an MI250Board structure describing the device to use.
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
****************************************************************************/
adp_result_t MI250_Close(MI250Board_t * MI250Board);


/****************************************************************************
* Function: MI250_InitPLL
*****************************************************************************
*
* Inittialize the PLL of the MI250.
*
* @param[in]   MI250Board
*      Pointer to an MI250Board structure describing the device to use.
*
* @param[in]   refclk
*      Reference clock frequency
*
* @param[in]   acqclk
*      Desired sampling clock frequency.
*
* @param[in]   clksrc
*      MI250_CLOCK_SOURCE enum that indicates hwo the sampling clock will be
*      generated.
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
****************************************************************************/
adp_result_t MI250_InitPLL(MI250Board_t * MI250Board, unsigned int refclk, unsigned int acqclk, MI250_CLOCK_SOURCE clksrc);


/****************************************************************************
* Function: MI250_SetLaneDelay
*****************************************************************************
*
* Assign lane delay to the MI250 instance. The lane delay is the delay of all
* FMC lanes in picoseconds.
*
* This function is only required when more than one MI250 board need to be
* configured on the same sampling clock.
*
* @param[in]   MI250Board
*      Pointer to an MI250Board structure describing the device to use.
*
* @param[in]   pLaneDelay
*      Pointer to the structure FMCEEPROM_HPC_DATA_Specs previously initialized
*      by Carrier_GetSignalsInfo function call.
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
* @pre
*    Carrier_GetSignalsInfo()
*
* @see
*    Carrier_GetSignalsInfo
*
****************************************************************************/
adp_result_t MI250_SetLaneDelay(MI250Board_t * MI250Board, FMCEEPROM_HPC_DATA_Specs * pLaneDelay);


/****************************************************************************
* Function: MI250_InitPLL
*****************************************************************************
*
* Assign the calibration result of the MI250 clock master instance to the
* current clock slave board.
*
* This function is only required when more than one MI250 board need to be
* configured on the same sampling clock.
*
* Before calling MI250_InitPLL function for the clock slave board,
* the clock master and clock slave boards must have their lane delays set
* (MI250_SetLaneDelay), the clock master board PLL must be initialized
* (MI250_InitPLL) and the MI250_SetMasterCalibResult function must be called.
*
* @param[in]   MI250Board
*      Pointer to an MI250Board structure describing the device to use.
*
* @param[in]   MasterMI250Board
*      The pointer to the clock master MI250 instance.
*
* Error code: #MI250 if no problem is found, !=#MI250 if problem.
*
****************************************************************************/
adp_result_t MI250_SetMasterCalibResult(MI250Board_t * MI250Board, MI250Board_t * MasterMI250Board);


#ifdef __cplusplus
    }
#endif

#endif


