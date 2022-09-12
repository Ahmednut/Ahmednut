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
 * @file       perseus611x_clkctrl.h
 * @brief      Perseus611x clock related functions definitions.
 *
 * $Date: 2015/07/09 20:32:02 $
 * $Revision: 1.2 $
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

#ifndef __PERSEUS611X_CLKCTRL_H__
#define __PERSEUS611X_CLKCTRL_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

 /****************************************************************************
 *   Function: Perseus611X_TClkSetState
 ************************************************************************//**
 *
 *   Set the state (enable or disable) of the specified TClk in the specified 
 *   direction
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [in] dir
 *    Specify the direction of the TCLK
 *
 * @param [in] state
 *    Specify the desired state (Carrier_eClkDisable or Carrier_eClkEnable)
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_TClkSetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t state);


 /****************************************************************************
 *   Function: Perseus611X_TClkGetState
 ************************************************************************//**
 *
 *   Get the state (enable, disable, or unknown) of the specified TClk in the specified 
 *   direction
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] tclk
 *    Specify the TClk
 *
 * @param [in] dir
 *    Specify the direction of the TCLK
 *
 * @param [out] state
 *    Pointer that will hold the retrieved state
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_TClkGetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t * state);


 /****************************************************************************
 *   Function: Perseus611X_TClkSetTxSource
 ************************************************************************//**
 *
 *   Set the driver of the specified TClk in TX direction (from carrier board
 *   to MicroTCA backplane).
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
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
Result Perseus611X_TClkSetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t source);

 /****************************************************************************
 *   Function: Perseus611X_TClkGetTxSource
 ************************************************************************//**
 *
 *   Get the driver of the specified TClk in TX direction (from carrier board
 *   to MicroTCA backplane).
 *
 *   TClk are backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
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
Result Perseus611X_TClkGetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t * source);


 /****************************************************************************
 *   Function: Perseus611X_FmcBidirClkSetConnection
 ************************************************************************//**
 *
 *   Set the connection of the specified FMC bidirectional clock on the 
 *   carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
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
Result Perseus611X_FmcBidirClkSetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t connection);

 /****************************************************************************
 *   Function: Perseus611X_FmcBidirClkGetConnection
 ************************************************************************//**
 *
 *   Get the connection of the specified FMC bidirectional clock on the 
 *   carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
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
Result Perseus611X_FmcBidirClkGetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t * connection);


 /****************************************************************************
 *   Function: Perseus611X_SetOscState
 ************************************************************************//**
 *
 *   Enable of disable the specified clock oscillator located on the carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] osc
 *    Specify the clock oscillator to change its state
 *
 * @param [in] state
 *    Specify the desired state (Carrier_eClkDisable or Carrier_eClkEnable)
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_SetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t state);

 /****************************************************************************
 *   Function: Perseus611X_GetOscState
 ************************************************************************//**
 *
 *   Get the state of the specified clock oscillator located on the carrier board.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] osc
 *    Specify the clock oscillator to change its state
 *
 * @param [out] state
 *    Pointer that will hold the retrieved state
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_GetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t * state);


 /****************************************************************************
 *   Function: Perseus611X_FClkASetConnection
 ************************************************************************//**
 *
 *   Set the connection of the FCLKA clock on the carrier board.
 *
 *   FCLKA is a backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [in] connection
 *    Specify the desired connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_FClkASetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t connection);

 /****************************************************************************
 *   Function: Perseus611X_FClkAGetConnection
 ************************************************************************//**
 *
 *   Get the connection of the FCLKA clock on the carrier board.
 *
 *   FCLKA is a backplane clocks in the MicroTCA stardard.
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open
 *
 * @param [out] connection
 *    Pointer that will hold the retrieved connection
 *                               
 * @return
 *      Return success or failure result. See CARRIER_service_error enum.
 *   
 ***************************************************************************/ 
Result Perseus611X_FClkAGetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t * connection);

#ifdef __cplusplus
}
#endif  // extern "C"
#endif

