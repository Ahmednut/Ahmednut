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
 * @file       adc5000.h
 * @brief      Functions controlling the ADC5000 FMC board.
 *
 * $Date: 2015/07/08 18:06:15 $
 * $Revision: 1.3 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup ADC5000
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

#ifndef __ADC5000__
#define __ADC5000__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
 
#include "bas_error.h"
#include "adc5000_defines.h"
#include "adc5000_cpld_regs.h"
#include "ad9517.h"
#include "ev10aq190.h"
#include "adt7411.h"
#include "adc5000_common.h"
#include "carrier_service_def.h"

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

/***************************************************************************
* Function: adc5000_open
************************************************************************//**
*
* Create a handle for use in the ADC5000 API.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [in] core 
*    address of the ADC5000 FPGA core
*
* @param [in] hCarrier 
*    Handle to a carrier object. It is the carrier that will handle the I2C
*    and sleep functions.
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_open(adc5000 * board, adc5000_core * core, Carrier_handle_t hCarrier);


/***************************************************************************
* Function: adc5000_reset
************************************************************************//**
*
* Reset the ADC5000 FMC card corresponding to the 'board' handle and
* configure this card with a basic configuration for ADC operation.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_reset(adc5000 * board);


/***************************************************************************
* Function: adc5000_close
************************************************************************//**
*
* Close a handle to a ADC5000 FMC card.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @return
*      Return success or error status
*
***************************************************************************/
Result adc5000_close(adc5000 * board);


/***************************************************************************
* Function: adc5000_calibrateiodelay
************************************************************************//**
*
* Calibrate the digital communication between the FMC ADC5000 ADC
* and the FPGA carrier board. Sweep IO delay, to find the most stable delay
* to apply between the data lanes and the ADC clock.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @return
*      Return success or error status
*
***************************************************************************/
Result adc5000_calibrateiodelay(adc5000 * board);


/***************************************************************************
* Function: adc5000_getadcstatus
************************************************************************//**
*
* Get the enable status of the ADC channel.
*
* @param [in,out] board 
*    pointer to adc5000 struct
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
Result adc5000_getadcstatus(adc5000 * board, unsigned char * adc_A_active, unsigned char * adc_B_active, unsigned char * adc_C_active, unsigned char * adc_D_active);


/***************************************************************************
* Function: adc5000_setmode
************************************************************************//**
*
* Select the acquisition mode. 4 channels allow data rate up to 1.25 GSps,
* 2 channels allows data rate up to 2.5 GSps and 1 channel allow data rate 
* up 5 GSps. All available modes are described by the ev10aq190_mode enum. 
* These modes select the number of channel and which front panel connector
* is chosen.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [in] mode
*    Mode to be applied to the ADC5000 ADC chip
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_setmode(adc5000 * board, ev10aq190_mode mode);


/***************************************************************************
* Function: adc5000_setstandby
************************************************************************//**
*
* Select the standby configuration of the ADC5000 ADC chip.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [in] standby
*    Standby mode that indicate which ADC channel to enable and disable
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_setstandby(adc5000 * board, ev10aq190_standby standby);


/***************************************************************************
* Function: adc5000_setcoding
************************************************************************//**
*
* Select the coding configuration of the ADC5000 ADC chip.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [in] coding
*    Coding mode that configure the ADC output to Binary or Gray coding
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_setcoding(adc5000 * board, ev10aq190_coding coding);


/***************************************************************************
* Function: adc5000_setbandwidth
************************************************************************//**
*
* Select the bandwidth configuration of the ADC5000 ADC chip.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [in] bandwidth
*    Bandwidth mode that configure the ADC bandwidth to Nominal (1.5 GHz)
*    or Full bandwidth (3.2 GHz)
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_setbandwidth(adc5000 * board, ev10aq190_bandwidth bandwidth);


/***************************************************************************
* Function: adc5000_pll_setparam
************************************************************************//**
*
* Configure the PLL with the parameters contain in the ad9517_param struct.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [in] param 
*    Struct containing the PLL parameters.
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_pll_setparam(adc5000 * board, ad9517_param * param);


/***************************************************************************
* Function: adc5000_setsync
************************************************************************//**
*
* Set the sync source of the ADC.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [in] sync_source 
*    Sync source.
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_setsync(adc5000 * board, adc5000_sync_source sync_source);


/***************************************************************************
* Function: adc5000_pll_lock
************************************************************************//**
*
* Read the lock status of the PLL.
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [out] status 
*    PLL is lock or unlock.
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_pll_lock(adc5000 * board, ad9517_lock_status * status);


/***************************************************************************
* Function: adc5000_settestmode
************************************************************************//**
*
* Configure the ADC so that it output a test signal (Flashing bit or ramp).
*
* @param [in,out] board 
*    pointer to adc5000 struct
*
* @param [in] state 
*    Disable the test mode, generate a ramp or generate flashing bits.
*
* @return
*    Return success or error status
*
***************************************************************************/
Result adc5000_settestmode(adc5000 * board, ev10aq190_testmode state);

/****************************************************************************
* Function: adc5000_setadjtriggerdelay
*************************************************************************//**
*
* Adjusts the trigger delay. 
* The trigger from the trig in connector can be delay for 1 to 32 ADC clock cycles
*
* @param [in] board 
*    Pointer to the ADC5000 structure
*
* @param [in] delay 
*    Delay in ADC clock cycles : [1,32]
*
* @return
*    Return success or error status
*
****************************************************************************/
Result adc5000_setadjtriggerdelay(adc5000 * board, char delay);

/****************************************************************************
* Function: adc5000_setoffset
*************************************************************************//**
*
* Adjust the channel offset
* Offset variation range: ~± 40 LSB, 1024 steps.
* 0x000 : Maximum positive offset applied, 
* 0x1FF : Minimum positive offset applied, 
* 0x200 : Minimum negative offset applied (0 LSB offset), 
* 0x3FF : Maximum negative offset applied.
*
* @param [in] board 
*    Pointer to the ADC5000 structure
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
Result adc5000_setoffset(adc5000 * board, ev10aq190_channel channel, unsigned short offset);

/****************************************************************************
* Function: adc5000_setgain
*************************************************************************//**
*
* Adjust the channel gain
* Gain variation range: ~±10%, 1024 steps (1 step ~0.02%).
* 0x000 : Gain shrunk to min accessible value, 
* 0x200 : Gain at Default value (0 dB), 
* 0x3FF : Gain Increased to max accessible value.
*
* @param [in] board 
*    Pointer to the ADC5000 structure
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
Result adc5000_setgain(adc5000 * board, ev10aq190_channel channel, unsigned short gain);

/****************************************************************************
* Function: adc5000_setphase
*************************************************************************//**
*
* Adjust the channel phase
* Delay control range for edges of internal sampling clocks: ~±15 ps (1 step ~30 fs).
* 0x000 : ~ -15ps correction on selected channel aperture Delay, 
* 0x200 : 0ps correction on selected channel aperture Delay, 
* 0x3FF : ~ +15ps correction on selected channel aperture Delay.
*
* @param [in] board Pointer to the ADC5000 structure
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
Result adc5000_setphase(adc5000 * board, ev10aq190_channel channel, unsigned short phase);

#ifdef __cplusplus
} 
#endif  // extern "C"    

#endif

