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
 * @file       perseus611x.h
 * @brief      Perseus611x carrier related functions definitions.
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.3 $
 *
 * @ingroup PERSEUS611X
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

#ifndef __PERSEUS611X_H__
#define __PERSEUS611X_H__


#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "bas_error.h"
#include "bas_debug.h"
#include "perseus611x_def.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service_def.h"
#include "FMCEEprom_def.h"      // FMCEEPROM_HPC_DATA_Specs

/** @defgroup PERSEUS611X Perseus611X FMC carrier related library
 *
 *  Allows to control and access information specific to the Perseus611X
 *  carrier board.
 *
 *  @file
 *
 */

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/****************************************************************************
 * Function: Perseus611X_Open
 ************************************************************************//**
 *
 * Open an instance of the Perseus611X carrier object. Must be on a
 * Perseus611X carrier board in order for this function to make sense.
 *
 * @param [out] phCarrier
 *    Pointer to a carrier handle to open the allocated and initialized
 *    carrier object.
 *
 * @param[in] pfUsleep
 *    Pointer to a sleep function that sleep during N microsecond.
 *    This function is the one that will be used inside the library functions.
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
Result Perseus611X_Open(Carrier_handle_t * phCarrier, int (*pf_usleep)(unsigned int usec));

/****************************************************************************
 * Function: Perseus611X_Close
 ************************************************************************//**
 *
 * Deallocate the Perseus611X carrier object.
 *
 * @param [in] hCarrier
 *    Carrier handle to deallocate.
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
Result Perseus611X_Close(Carrier_handle_t hCarrier);

 /****************************************************************************
 *   Function: Perseus611X_FmcGetI2cZone
 ************************************************************************//**
 *
 *   Retrieve the I2C zone of the specified FMC connector for
 *   the Perseus611X.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    FMC connector number. Connector instance starts at 0.
 *
 *   @param [out] I2cZone
 *      Pointer that will hold the retrieved I2C zone.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_FmcGetI2cZone(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, unsigned int * I2cZone);

/****************************************************************************
 * Function: Perseus611X_FmcAssignSlot
 ************************************************************************//**
 *
 * This function assign to each connector of the Perseus611X carrier, the detected fmc
 * modules.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    Connector number used (Perseus611X has 2 connectors)
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
Result Perseus611X_FmcAssignSlot(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector);


/****************************************************************************
 * Function: Perseus611X_FmcSetVadj
 ************************************************************************//**
 *
 * This function sets the requiered vadj voltage for the carrier board/connector.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    Connector number used (Perseus611X has 2 connectors)
 *
 * @param [in] board
 *    Which board in the connector to set
 *
 * @param [in] Value
 *    Carrier Vadj value setting required
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
Result Perseus611X_FmcSetVadj(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board, Carrier_eVadjValue_t Value);

/****************************************************************************
 * Function: Perseus611X_FmcGetVadj
 ************************************************************************//**
 *
 * This function verifies if the requiered vadj voltage is compatible
 * with what could be provided for a specific board slot.
 * It returns the vadj level setting needed for the board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    Connector number used (Perseus611X has 2 connectors)
 *
 * @param [in] board
 *    Which board in the slot to verify
 *
 * @param [in] vadjnom
 *    Nominal vadj requiered voltage in 10mV
 *
 * @param [in] vadjmin
 *    Minimum vadj requiered voltage in 10mV
 *
 * @param [in] vadjmax
 *    Maximum vadj requiered voltage in 10mV
 *
 * @param [out] pVadjValue
 *    Vadj value setting needed to match for the provided Vadj voltage range requiered
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
Result Perseus611X_FmcGetVadj(Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board,
    unsigned vadjnom, unsigned vadjmin, unsigned vadjmax, Carrier_eVadjValue_t *pVadjValue);
    
 /****************************************************************************
 *   Function: Perseus611X_FmcGetGA
 ************************************************************************//**
 *
 *   Retrieve the Geographical Address (GA) of the specified FMC connector.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    FMC connector number. Connector instance starts at 0.
 *
 *   @param [out] ga
 *      Pointer that will hold the retrieved geographical address.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *
 ***************************************************************************/
Result Perseus611X_FmcGetGA(Carrier_eFmcConnectorNum_t connector, unsigned char * ga);

/****************************************************************************
 * Function: Perseus611X_GetSignalsInfo
 ************************************************************************//**
 *
 * This function gets the data pairs (LA, HA, HB) length (delay in ps) for the carrier.
 * The delays are estimated value for material DK 4.5 (180 ps/po).
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connector
 *    FMC connector number. Connector instance starts at 0.
 *
 * @param [out] ppSignal
 *    Will return a pointer on the delay structure pointer
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
Result Perseus611X_GetSignalsInfo(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, FMCEEPROM_HPC_DATA_Specs **ppSignal);

 /****************************************************************************
 *   Function: Perseus611X_GetModuleBaseAddr
 ************************************************************************//**
 *
 *   Retrieve the base address corresponding to the specified module.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] module
 *    Carrier_eModule_t enum that indicates the module type
 *
 *   @param [in] index
 *      Index of the module to get the base address. Index starts at 0.
 *
 *   @param [out] addr
 *      Pointer that will hold the retrieved base address.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_GetModuleBaseAddr(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int index, unsigned int * addr);

 /****************************************************************************
 *   Function: Perseus611X_GetModuleSize
 ************************************************************************//**
 *
 *   Retrieve the memory space size of the specified module type.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] module
 *    Carrier_eModule_t enum that indicates the module type
 *
 *   @param [out] size
 *      Pointer that will hold the retrieved memory space size.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_GetModuleSize(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * size);

 /****************************************************************************
 *   Function: Perseus611X_GetModuleMaxNumber
 ************************************************************************//**
 *
 *   Retrieve the maximal number of instance of the specified module type the
 *   current carrier can handle.
 *
 *   The retrieved instance number minus 1, is the maximal index value that
 *   can be used when calling the Carrier_GetModuleBaseAddr function.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] module
 *    Carrier_eModule_t enum that indicates the module type
 *
 *   @param [out] num
 *      Pointer that will hold the retrieved instance number.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_GetModuleMaxNumber(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * num);

 /****************************************************************************
 *   Function: Perseus611X_GetMemSpaceBaseAddr
 ************************************************************************//**
 *
 *   Retrieve the base address corresponding to the specified memory space.
 *
 *   A memory space is a range of memory address that is available to the
 *   user to connect its custom logic to the processor. Once the custom logic
 *   is mapped inside a memory space, its registers are reachable using the memory
 *   read and write functions.  
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] space
 *    Carrier_eMemSpace_t enum that indicates a specific memory space
 *
 *   @param [out] addr
 *      Pointer that will hold the retrieved base address.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_GetMemSpaceBaseAddr(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * addr);

 /****************************************************************************
 *   Function: Perseus611X_GetMemSpaceSize
 ************************************************************************//**
 *
 *   Retrieve the memory space size of the specified memory space type.
 *
 *   A memory space is a range of memory address that is available to the
 *   user to connect its custom logic to the processor. Once the custom logic
 *   is mapped inside a memory space, its registers are reachable using the memory
 *   read and write functions.  
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] space
 *    Carrier_eMemSpace_t enum that indicates a specific memory space
 *
 *   @param [out] size
 *      Pointer that will hold the retrieved memory space size.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_GetMemSpaceSize(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * size);

 /****************************************************************************
 *   Function: Perseus611X_CustomRegisterWrite
 ************************************************************************//**
 *
 *   Update the value of a custom register in the platform register FPGA core.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] index
 *    Index of the custom register. Index starts at 0.
 *
 *   @param [in] value
 *      32-bit value to write in the custom register.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_CustomRegisterWrite(Carrier_handle_t hCarrier, unsigned int index, unsigned int value);

 /****************************************************************************
 *   Function: Perseus611X_CustomRegisterRead
 ************************************************************************//**
 *
 *   Retrieve the value of a custom register in the platform register FPGA core.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] index
 *    Index of the custom register. Index starts at 0.
 *
 *   @param [out] value
 *      Pointer that will hold the retrieved 32-bit value of the custom register.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_CustomRegisterRead(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value);

 /****************************************************************************
 *   Function: Perseus611X_LedSetValue
 ************************************************************************//**
 *
 *   Set the state of the carrier user LEDs.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] index
 *    Index of the LED. Index starts at 0.
 *    Index from 0 to 7 represent the red front-panel LEDs
 *    Index from 8 to 15 represent the green front-panel LEDs
 *
 *   @param [in] value
 *      Specify the desired LED state. 0: Turned off,  1: Turned on
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_LedSetValue(Carrier_handle_t hCarrier, unsigned int index, unsigned int value);

 /****************************************************************************
 *   Function: Perseus611X_LedGetValue
 ************************************************************************//**
 *
 *   Get the state of the carrier user LEDs.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] index
 *    Index of the LED. Index starts at 0.
 *    Index from 0 to 7 represent the red front-panel LEDs
 *    Index from 8 to 15 represent the green front-panel LEDs
 *
 *   @param [out] value
 *      Pointer that will hold the retrieved LED state. 0: Turned off,  1: Turned on
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_LedGetValue(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value);

 /****************************************************************************
 *   Function: Perseus611X_Reboot
 ************************************************************************//**
 *
 *   Reboot the carrier platform. This will reset the FPGA and program it
 *   from the Flash memory content.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_Reboot(Carrier_handle_t hCarrier);

#ifdef __cplusplus
}
#endif  // extern "C"
#endif

