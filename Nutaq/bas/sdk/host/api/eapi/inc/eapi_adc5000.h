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
 * @file     eapi_adc5000.h
 * @brief    Functions controlling the ADC5000 module
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADC5000
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
 
 #ifndef __EAPI_ADC5000__
#define __EAPI_ADC5000__

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "eapi.h"
#include "adc5000_defines.h"
#include "ev10aq190_defines.h"

/** @defgroup ADC5000 ADC5000 Module
 *
 *  ADC5000 module allow to control the ADC5000 FMC card. The ADC5000 FMC has 
 *  four 1.25 GHz ADCs and a configurable PLL.
 *
 *  @file
 *
 */
 
/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

/**
*****************************************************************************
* Function: ADC5000_Presence_send
*************************************************************************//**
*
* Verifies the ADC5000 FMC card and FPGA core presence
*
* @param[in] state Pointer to ethernet connection
*
* @return
*      Return success or error status
*
****************************************************************************/
Result ADC5000_Presence_send(connection_state * state);

/**
*****************************************************************************
* Function: ADC5000_Powerup_send
*************************************************************************//**
*
* Powers up the FMC site for ADC5000 operation
*
* @param[in] state Pointer to ethernet connection
*
* @return
*      Return success or error status
*
****************************************************************************/
Result ADC5000_Powerup_send(connection_state * state);

/***************************************************************************
* Function: ADC5000_Reset_send
************************************************************************//**
*
* Reset the ADC5000 FMC card and
* configure this card with a basic configuration for ADC operation.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_Reset_send(connection_state * state);

/***************************************************************************
* Function: ADC5000_SetMode_send
************************************************************************//**
*
* Select the acquisition mode. 4 channels allow data rate up to 1.25 GSps,
* 2 channels allows data rate up to 2.5 GSps and 1 channel allow data rate 
* up 5 GSps. All available modes are described by the ev10aq190_mode enum. 
* These modes select the number of channel and which front panel connector
* is chosen.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] mode
*    Mode to be applied to the ADC5000 ADC chip
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_SetMode_send(connection_state * state, ev10aq190_mode mode);

/***************************************************************************
* Function: ADC5000_SetSync_send
************************************************************************//**
*
* Set the sync source of the ADC.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] sync_source 
*    Sync source.
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_SetSync_send(connection_state * state, adc5000_sync_source sync_source);

/***************************************************************************
* Function: ADC5000_PLLLock_send
************************************************************************//**
*
* Read the lock status of the PLL.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [out] lockstatus 
*    PLL is lock or unlock.
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_PLLLock_send(connection_state * state, ad9517_lock_status *lockstatus);

/***************************************************************************
* Function: ADC5000_SetTestMode_send
************************************************************************//**
*
* Configure the ADC so that it output a test signal (Flashing bit or ramp).
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] testmode 
*    Disable the test mode, generate a ramp or generate flashing bits.
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_SetTestMode_send(connection_state * state, ev10aq190_testmode testmode);

/***************************************************************************
* Function: adc5000_calibrateiodelay
************************************************************************//**
*
* Calibrate the digital communication between the FMC ADC5000 ADC
* and the FPGA carrier board. Sweep IO delay, to find the most stable delay
* to apply between the data lanes and the ADC clock.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @return
*      Return success or error status
*
***************************************************************************/
Result ADC5000_CalibrateIoDelay_send(connection_state * state);

/***************************************************************************
* Function: ADC5000_GetAdcStatus_send
************************************************************************//**
*
* Get the enable status of the ADC channel.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [out] adc_A_active
*    return the status of the ADC channel A
*    0 : if channel A is disabled
*    1 : if channel A is enabled
*
* @param [out] adc_B_active
*    return the status of the ADC channel B
*    0 : if channel B is disabled
*    1 : if channel B is enabled
*
* @param [out] adc_C_active
*    return the status of the ADC channel C
*    0 : if channel C is disabled
*    1 : if channel C is enabled
*
* @param [out] adc_D_active
*    return the status of the ADC channel D
*    0 : if channel D is disabled
*    1 : if channel D is enabled
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_GetAdcStatus_send(connection_state * state, unsigned char * adc_A_active, unsigned char * adc_B_active, unsigned char * adc_C_active, unsigned char * adc_D_active);

/***************************************************************************
* Function: ADC5000_SetStandby_send
************************************************************************//**
*
* Select the standby configuration of the ADC5000 ADC chip.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] standby
*    Standby mode that indicate which ADC channel to enable and disable
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_SetStandby_send(connection_state * state, ev10aq190_standby standby);

/***************************************************************************
* Function: ADC5000_SetCoding_send
************************************************************************//**
*
* Select the coding configuration of the ADC5000 ADC chip.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] coding
*    Coding mode that configure the ADC output to Binary or Gray coding
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_SetCoding_send(connection_state * state, ev10aq190_coding coding);

/***************************************************************************
* Function: ADC5000_SetBandwidth_send
************************************************************************//**
*
* Select the bandwidth configuration of the ADC5000 ADC chip.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] bandwidth
*    Bandwidth mode that configure the ADC bandwidth to Nominal (1.5 GHz)
*    or Full bandwidth (3.2 GHz)
*
* @return
*    Return success or error status
*
***************************************************************************/
Result ADC5000_SetBandwidth_send(connection_state * state, ev10aq190_bandwidth bandwidth);

/****************************************************************************
* Function: ADC5000_SetAdjTriggerDelay_send
*************************************************************************//**
*
* Adjusts the trigger delay. 
* The trigger from the trig in connector can be delay for 1 to 32 ADC clock cycles
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] delay 
*    Delay in ADC clock cycles : [1,32]
*
* @return
*    Return success or error status
*
****************************************************************************/
Result ADC5000_SetAdjTriggerDelay_send(connection_state * state, char delay);

/****************************************************************************
* Function: ADC5000_SetOffset_send
*************************************************************************//**
*
* Adjust the channel offset
* Offset variation range: ~± 40 LSB, 1024 steps.
* 0x000 : Maximum positive offset applied, 
* 0x1FF : Minimum positive offset applied, 
* 0x200 : Minimum negative offset applied (0 LSB offset), 
* 0x3FF : Maximum negative offset applied.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] channel 
*    Channel to apply the new offset
*
* @param [in] offset 
*    Offset to apply : [0,1023]
*
* @return
*    Return success or error status
*
****************************************************************************/
Result ADC5000_SetOffset_send(connection_state * state, ev10aq190_channel channel, unsigned short offset);

/****************************************************************************
* Function: ADC5000_SetGain_send
*************************************************************************//**
*
* Adjust the channel gain
* Gain variation range: ~±10%, 1024 steps (1 step ~0.02%).
* 0x000 : Gain shrunk to min accessible value, 
* 0x200 : Gain at Default value (0 dB), 
* 0x3FF : Gain Increased to max accessible value.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] channel
*    Channel to apply the new gain
*
* @param [in] gain
*    Gain to apply : [0,1023]
*
* @return
*    Return success or error status
*
****************************************************************************/
Result ADC5000_SetGain_send(connection_state * state, ev10aq190_channel channel, unsigned short gain);

/****************************************************************************
* Function: ADC5000_SetPhase_send
*************************************************************************//**
*
* Adjust the channel phase
* Delay control range for edges of internal sampling clocks: ~±15 ps (1 step ~30 fs).
* 0x000 : ~ -15ps correction on selected channel aperture Delay, 
* 0x200 : 0ps correction on selected channel aperture Delay, 
* 0x3FF : ~ +15ps correction on selected channel aperture Delay.
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] channel
*    Channel to apply the new phase
*
* @param [in] phase
*    Phase to apply : [0,1023]
*
* @return
*    Return success or error status
*
****************************************************************************/
Result ADC5000_SetPhase_send(connection_state * state, ev10aq190_channel channel, unsigned short phase);

/***************************************************************************
* Function: adc5000_pll_calculate
************************************************************************//**
*
* Calculate a PLL configuration considering the inputted parameters
* and configure the PLL
*
* @param[in] state 
*    Pointer to ethernet connection
*
* @param [in] ref_freq
*    The reference clock frequency.
*
* @param [in] adc_freq
*    The data clock frequency. 
*    Must be 2x the desired sampling rate 
*
* @param [in] clock_output_freq
*    The frequency outputted to clock ouput connector. 
*    Must be 2x the desired sampling rate 
*
* @param [in] fmc_freq
*    The clock frequency sent to the FMC connector.
*    Must be 1/8 of the desired sampling rate 
*
* @param [in] sync_output_freq
*    The frequency of the sync signal.
*    Can be 1/8 of the desired sampling rate 
*
* @param [in] clock_source
*    The source of the reference clock
*
* @return
*    return ADC5000OK on success. If the requested parameters cannot result
*    in a valid PLL configuration, an error will be returned.
*
***************************************************************************/
Result ADC5000_PLLConfigure_send(connection_state * state, unsigned int ref_freq, unsigned int adc_freq, unsigned int clock_output_freq, 
									unsigned int fmc_freq, unsigned int sync_output_freq, adc5000_clock_source clock_source);	

#ifdef __cplusplus
}
#endif // extern "C"



#endif

