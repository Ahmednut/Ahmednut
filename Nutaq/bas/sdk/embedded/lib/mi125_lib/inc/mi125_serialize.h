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
 * @file       mi125_serialize.h
 * @brief      mi125 module related functions additionnal definitions
 *
 * $Date: 2015/04/10 14:08:14 $
 * $Revision: 1.1 $
 *
 * (c) 2012, Nutaq Inc. All rights reserved.              
 *
 ***************************************************************************/

#include "connection_state.h"

#ifndef __MI125_SERIALIZE__
#define __MI125_SERIALIZE__

#ifdef __cplusplus
extern "C" {
#endif

#include "MI125_mi125_defines.h"


 /****************************************************************************
 * NAME : mi125_set_config_param_send
 ************************************************************************//**
 *
 * Configure the board with user specified custom options.
 * This is internal function used by MI125_mi125_set_config_send() to send
 * the config information to the module.  
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] config
 *    Board channel config structure to use.
 *
 * @return NONE   
 * 
 ***************************************************************************/ 
void mi125_set_config_param_send(connection_state * state,  MI125_mi125_config *config);


 /****************************************************************************
 * NAME : mi125_set_config_param_recv
 ************************************************************************//**
 *
 * Get the board user specified custom options.
 * This is internal function used by CCE to read the config information to the module.  
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] config
 *    Board channel config structure to use.
 *
 * @return NONE   
 * 
 ***************************************************************************/ 
void mi125_set_config_param_recv(connection_state * state, MI125_mi125_config *config);


#ifdef __cplusplus
}
#endif

#endif
