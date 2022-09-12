
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
*  @file       : eapi_carrier.h
*  @brief      : EAPI functions to control the carrier board
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup CARRIERLIB
 *
 * This file is part of Nutaq’s ADP Software Suite.
 *
 * You may at your option receive a license to Nutaq’s ADP Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s ADP Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s ADP Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s ADP Software Suite under the GPL requires that your work based on
 * Nutaq’s ADP Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s ADP Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s ADP Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef __EAPI_CARRIER__
#define __EAPI_CARRIER__

#ifdef __cplusplus
extern "C" {
#endif

#include "connection_state.h"
#include "bas_error.h"
#include "bas_types.h"
#include "carrier_service_def.h"
  
/** @defgroup CARRIERLIB CARRIERLIB EAPI
 *
 * Functions to deal with generic carrier services in the system.
 * 
 * @file
 *     
 */
 
 /****************************************************************************
 *   Function: Carrier_Reboot_send
 ************************************************************************//**
 *
 *   Reboot the carrier platform.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_Reboot_send(connection_state * state);

 /****************************************************************************
 * Function: Carrier_Presence_send
 *************************************************************************//**
 *
 * Get carrier presence.
 *
 * Deprecated. Only defined for backward compatibility.
 * Use Carrier_GetType_send function instead to detect a carrier board.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *
 ****************************************************************************/
Result Carrier_Presence_send(connection_state * state);

 /****************************************************************************
 *   Function: Carrier_GetType_send
 ************************************************************************//**
 *
 *   Get the currrent type of the carrier.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [out] pType
 *    Pointer that will hold the carrier type.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetType_send(connection_state * state, Carrier_eType_t * pType);

 /****************************************************************************
 *   Function: Carrier_GetModuleBaseAddr_send
 ************************************************************************//**
 *
 *   Retrieve the base address corresponding to the specified module.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
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
Result Carrier_GetModuleBaseAddr_send(connection_state * state, Carrier_eModule_t module, unsigned int index, unsigned int * addr);

 /****************************************************************************
 *   Function: Carrier_LedSetValue_send
 ************************************************************************//**
 *
 *   Set the state of the carrier user LEDs.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] index
 *    Index of the LED. Index starts at 0.
 *
 *   @param [in] value
 *      Specify the desired LED state. 0: Turned off,  1: Turned on
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_LedSetValue_send(connection_state * state, unsigned int index, unsigned int value);

 /****************************************************************************
 *   Function: Carrier_LedGetValue_send
 ************************************************************************//**
 *
 *   Get the state of the carrier user LEDs.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] index
 *    Index of the LED. Index starts at 0.
 *
 *   @param [out] value
 *      Pointer that will hold the retrieved LED state. 0: Turned off,  1: Turned on
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_LedGetValue_send(connection_state * state, unsigned int index, unsigned int * value);


 /****************************************************************************
 *   Function: Carrier_TClkSetState
 ************************************************************************//**
 *
 *   Set the state (enable or disable) of the specified TClk in the specified 
 *   direction
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [in] dir
 *    Specify the direction of the TCLK
 *
 * @param [in] clkstate
 *    Specify the desired state (Carrier_eClkDisable or Carrier_eClkEnable)
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_TClkSetState_send(connection_state * state, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t clkstate);

 /****************************************************************************
 *   Function: Carrier_TClkGetState
 ************************************************************************//**
 *
 *   Get the state (enable, disable, or unknown) of the specified TClk in the specified 
 *   direction
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [in] dir
 *    Specify the direction of the TCLK
 *
 * @param [out] clkstate
 *    Pointer that will hold the retrieved state
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_TClkGetState_send(connection_state * state, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t * clkstate);

 /****************************************************************************
 *   Function: Carrier_TClkSetTxSource
 ************************************************************************//**
 *
 *   Set the driver of the specified TClk in TX direction (from carrier board
 *   to MicroTCA backplane).
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [in] source
 *    Specify the desired source of the TX clock
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_TClkSetTxSource_send(connection_state * state, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t source);

 /****************************************************************************
 *   Function: Carrier_TClkGetTxSource
 ************************************************************************//**
 *
 *   Get the driver of the specified TClk in TX direction (from carrier board
 *   to MicroTCA backplane).
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [out] source
 *    Pointer that will hold the retrieved source of the TX clock
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_TClkGetTxSource_send(connection_state * state, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t * source);

 /****************************************************************************
 *   Function: Carrier_FmcBidirClkSetConnection
 ************************************************************************//**
 *
 *   Set the connection of the specified FMC bidirectional clock on the 
 *   carrier board.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] clk
 *    Specify the FMC bidirectional clock to change its connection
 *
 * @param [in] connection
 *    Specify the desired connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FmcBidirClkSetConnection_send(connection_state * state, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t connection);

 /****************************************************************************
 *   Function: Carrier_FmcBidirClkGetConnection
 ************************************************************************//**
 *
 *   Get the connection of the specified FMC bidirectional clock on the 
 *   carrier board.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] clk
 *    Specify the FMC bidirectional clock to change its connection
 *
 * @param [out] connection
 *    Pointer that will hold the retrieved connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FmcBidirClkGetConnection_send(connection_state * state, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t * connection);

 /****************************************************************************
 *   Function: Carrier_SetOscState
 ************************************************************************//**
 *
 *   Enable of disable the specified clock oscillator located on the carrier board.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] osc
 *    Specify the clock oscillator to change its state
 *
 * @param [in] clkstate
 *    Specify the desired state (Carrier_eClkDisable or Carrier_eClkEnable)
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_SetOscState_send(connection_state * state, Carrier_eOscillator_t osc, Carrier_eClkState_t clkstate);

 /****************************************************************************
 *   Function: Carrier_GetOscState
 ************************************************************************//**
 *
 *   Get the state of the specified clock oscillator located on the carrier board.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] osc
 *    Specify the clock oscillator to change its state
 *
 * @param [out] clkstate
 *    Pointer that will hold the retrieved state
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_GetOscState_send(connection_state * state, Carrier_eOscillator_t osc, Carrier_eClkState_t * clkstate);

 /****************************************************************************
 *   Function: Carrier_FClkASetConnection
 ************************************************************************//**
 *
 *   Set the connection of the FCLKA clock on the carrier board.
 *
 *   FCLKA is a backplane clocks in the MicroTCA stardard.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] connection
 *    Specify the desired connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FClkASetConnection_send(connection_state * state, Carrier_eFClkAConnection_t connection);

 /****************************************************************************
 *   Function: Carrier_FClkAGetConnection
 ************************************************************************//**
 *
 *   Get the connection of the FCLKA clock on the carrier board.
 *
 *   FCLKA is a backplane clocks in the MicroTCA stardard.
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [out] connection
 *    Pointer that will hold the retrieved connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Carrier_FClkAGetConnection_send(connection_state * state, Carrier_eFClkAConnection_t * connection);

/****************************************************************************
 * Function: Carrier_I2cRead
 ************************************************************************//**
 *
 * Perform an I2C Read transaction.
 *
 * @warning 
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param[in] i2cDeviceAddr is the 7 bits i2c eeprom address.
 *
 * @param[in] addr is the start address where to read byte(s).
 *                 MSB is sent first in case of a 2 bytes address.
 *                 addr can be seen as the register address of the internal 
 *                 memory map of the I2C chip.
 *
 * @param[out] data contains the data byte(s) readed.
 *
 * @param[in] size indicates the number of bytes to read.
 *
 * @param[in] options contains two parameters: 
 *                    nbByteAddr = number of bytes of the addr parameter (can be 1 or 2 bytes).
 *                    stopCondition = not used for the moment. Indicates whether to hold of free
 *                    the bus after sending the data.  
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_I2cRead_send(connection_state * state, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options);

/****************************************************************************
 * Function: Carrier_I2cWrite
 ************************************************************************//**
 *
 * Perform an I2C Write transaction.
 *
 * @warning 
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param[in] i2cDeviceAddr is the 7 bits i2c eeprom address.
 *
 * @param[in] addr is the start address where to send byte(s).
 *                 MSB is sent first in case of a 2 bytes address.
 *                 addr can be seen as the register address of the internal 
 *                 memory map of the I2C chip.
 *
 * @param[in] data is the data byte(s) to send.
 *
 * @param[in] size indicates the number of bytes to send.
 *
 * @param[in] options contains two parameters: 
 *                    nbByteAddr = number of bytes of the addr parameter (can be 1 or 2 bytes).
 *                    stopCondition = not used for the moment. Indicates whether to hold of free
 *                    the bus after sending the data.  
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_I2cWrite_send(connection_state * state, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options);

/****************************************************************************
 * Function: Carrier_I2cReadNoWrData
 ************************************************************************//**
 *
 * Read data from an I2C device.
 * This function does not send the register start address before reading from
 * the I2C interface. It only read the incoming data.
 * It will start reading from current internal device address and so no
 * assumption could be made on the content read from what device address.
 *
 * @warning 
 *
 * @param[in] state 
 *    Pointer to the connection state structure that handles the TCP connection 
 *    between the host and the carrier platform.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param[in] i2cDeviceAddr is the 7 bits i2c eeprom address.
 *
 * @param[out] data contains the data byte(s) readed.
 *
 * @param[in] size indicates the number of bytes to read.
 *
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *     
 * @see
 *
 ***************************************************************************/ 
Result Carrier_I2cReadNoWrData_send(connection_state * state, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned char * data, unsigned int size);

#ifdef __cplusplus
}
#endif

#endif 
