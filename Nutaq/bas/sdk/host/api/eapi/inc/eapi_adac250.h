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
 * @file     eapi_adac250.h 
 * @brief    Functions controlling the ADAC250 module
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADAC250
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


#ifndef __EAPI_ADAC250__
#define __EAPI_ADAC250__

#ifdef __cplusplus
extern "C" {
#endif

#include "eapi.h"
#include "adac250_defines.h"
#include "adac250_types.h"

/**
*****************************************************************************
* Function: ADAC250_Presence_send
*************************************************************************//**
*
* Verifies the ADAC250 FMC card and FPGA core presence
*
* @param[in] state Pointer to Ethernet connection
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_Presence_send(connection_state * state);

/**
*****************************************************************************
* Function: ADAC250_PowerUp_send
*************************************************************************//**
*
* Powers up the FMC site for ADAC250 operation
*
* @param[in] state Pointer to Ethernet connection
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_PowerUp_send(connection_state * state);

/**
*****************************************************************************
* Function: ADAC250_ReadSPIRegisters_send
*************************************************************************//**
*
* Reads an SPI register on the ADAC250 hardware
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] DeviceId   SPI device to be read
*
* @param[in] BaseRegister   Regeister address to be read
*
* @param[out] value  Value read
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ReadSPIRegisters_send(connection_state * state, ADAC250_DEVICES DeviceId, int BaseRegister, unsigned char * value);

/**
*****************************************************************************
* Function: ADAC250_WriteSPIRegisters_send
*************************************************************************//**
*
* Writes an SPI register on the ADAC250 hardware
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] DeviceId   SPI device to be wtitten
*
* @param[in] BaseRegister   Regeister address to be written
*
* @param[in] value   Value to be written
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_WriteSPIRegisters_send(connection_state * state, ADAC250_DEVICES DeviceId, int BaseRegister, unsigned char value);

/**
*****************************************************************************
* Function: ADAC250_DAC_SetGain_send
*************************************************************************//**
*
* Sets the DAC gain of the specified channel.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] channel   Channel to be set
*
* @param[in] gain   Gain to be set
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_DAC_SetGain_send(connection_state * state, ADAC250_DAC_CHANNEL channel, int gain);

/**
*****************************************************************************
* Function: ADAC250_DAC_SetOffset_send
*************************************************************************//**
*
* Sets the DAC offset of the specified channel.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] channel   Channel to be set
*
* @param[in] offset   Offset to be set
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_DAC_SetOffset_send(connection_state * state, ADAC250_DAC_CHANNEL channel, int offset);

/**
*****************************************************************************
* Function: ADAC250_DAC_Init_send
*************************************************************************//**
*
* Initializes the DAC to specified values.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] sleep Powers down DAC A, DAC B, or both
*
* @param[in] interpolation DAC interpolation mode
*
* @param[in] mixmode1 Mix mode for I-Q signals. Refer to the DAC5682 datasheet.
*
* @param[in] mixmode2 Mix mode for I-Q signals. Refer to the DAC5682 datasheet.
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_DAC_Init_send(connection_state * state, ADAC250_DAC_SLEEP sleep, ADAC250_DAC_INTERPOLATION interpolation, ADAC250_DAC_CMIX_MODE mixmode1, ADAC250_DAC_CMIX_MODE mixmode2);

/**
*****************************************************************************
* Function: ADAC250_DAC_Sync_send
*************************************************************************//**
*
* Enables or disables DAC outputs.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] sync Sync enable. Either ADAC250_DAC_SYNC_OFF or ADAC250_DAC_SYNC_ON
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_DAC_Sync_send(connection_state * state, ADAC250_DAC_SYNC sync);

/**
*****************************************************************************
* Function: ADAC250_DAC_SyncSource_send
*************************************************************************//**
*
* Selects source for the sync signal. Either from FPGA or from DAC registers.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] syncsource Sync source. Either ADAC250_DAC_SYNC_SOURCE_FPGA or ADAC250_DAC_SYNC_SOURCE_DACREG
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_DAC_SyncSource_send(connection_state * state, ADAC250_DAC_SYNC_SOURCE syncsource);

/**
*****************************************************************************
* Function: ADAC250_DAC_Calibrate_send
*************************************************************************//**
*
* Calibrates the digital communication between the FPGA and the DAC.
*
* @param[in] state Pointer to Ethernet connection
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_DAC_Calibrate_send(connection_state * state);

/**
*****************************************************************************
* Function: ADAC250_PLL_Init_send
*************************************************************************//**
*
* Resets and initializes the ADAC250 PLL (AD9511)
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] refclk Reference clock value in Hertz
*
* @param[in] adcclk Required ADC clock value in Hertz
*
* @param[in] dacclk Required DAC clock value in Hertz
*
* @param[in] interp Required interpolation factor (1, 2 or 4)
*
* @param[in] clksrc Clock source.  ADAC250_CLOCKIN_10MHZREF for onboard reference,
*                                  ADAC250_CLOCKIN_EXTERNALREF for external reference from clk in connector          
*   							   ADAC250_CLOCKIN_EXTERNALCLOCK for external clock from clk in connector
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_PLL_Init_send(connection_state * state, unsigned int refclk, unsigned int adcclk, unsigned int dacclk, ADAC250_DAC_INTERPOLATION interp, ADAC250_CLOCK_SOURCES clksrc);

/**
*****************************************************************************
* Function: ADAC250_PLL_getStatus_send
*************************************************************************//**
*
* Returns the current PLL lock status.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[out] locked  Current PLL lock value read
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_PLL_getStatus_send(connection_state * state, int * locked);

/**
*****************************************************************************
* Function: ADAC250_PLL_SetSleep_send
*************************************************************************//**
*
* Configures PLL power down states.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] sleep Powerdown part of the AD9511 PLL. Check PLL_SLEEP structure for more detail.
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_PLL_SetSleep_send(connection_state * state, ADAC250_PLL_SLEEP sleep);

/**
*****************************************************************************
* Function: ADAC250_PLL_setRefClkTuning_send
*************************************************************************//**
*
* Tunes the ADAC250 reference clock.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] value unsigned 16-bit value
*
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_PLL_setRefClkTuning_send(connection_state * state, unsigned short value);

/**
*****************************************************************************
* Function: ADAC250_PLL_SetConfig_send
*************************************************************************//**
*
* Set the PLL with the ADAC250_PLL_PARAM structure.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] param ADAC250 PLL parameter structure
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_PLL_SetConfig_send(connection_state * state, ADAC250_PLL_PARAM param);

/**
*****************************************************************************
* Function: ADAC250_ADC_init
*************************************************************************//**
*
* Initializes the A/D converter at the default ADAC250 values
*
* @param[in] state Pointer to Ethernet connection
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_init_send(connection_state * state);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetGain_send
*************************************************************************//**
*
* Sets the ADC gain. 
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] channel Channel to be modified
*
* @param[in] gain Gain from ADAC250_ADC_GAIN enumeration
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_setGain_send(connection_state * state, ADAC250_ADC_CHANNEL channel, ADAC250_ADC_GAIN gain);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetFineGain_send
*************************************************************************//**
*
* Sets the ADC fine gain. 
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] channel Channel to be modified
*
* @param[in] gain  Fine gain. Value are 0 to 128. Gain adjustement is 0 to 0.134 dB
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_setFineGain_send(connection_state * state, ADAC250_ADC_CHANNEL channel, char gain);

/**
*****************************************************************************
* Function: ADAC250_ADC_setPedestal_send
*************************************************************************//**
*
* Sets the ADC offset. 
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] channel Channel to be modified
*
* @param[in] value  ADC offset value ( values -32 to 31 in ADC increments)
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_setPedestal_send(connection_state * state, ADAC250_ADC_CHANNEL channel, char value);

/**
*****************************************************************************
* Function: ADAC250_ADC_getStatus_send
*************************************************************************//**
*
* Retrieves the ADC status.
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] channel Channel for the ADC status
* 
* @param[in] status  Status retrieved
*
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_getStatus_send(connection_state * state, ADAC250_ADC_CHANNEL channel, ADAC250_ADC_STATUSES * status);

/**
*****************************************************************************
* Function: ADAC250_ADC_Calibrate_send
*************************************************************************//**
*
* Calibrates the ADC lvds data bus timings
*
* @param[in] state Pointer to Ethernet connection
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_Calibrate_send(connection_state * state);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetDataFormat_send
*************************************************************************//**
*
* Sets the ADC data format. 
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] format Data format. Either 2's complement or offset
*
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_SetDataFormat_send(connection_state * state, ADAC250_ADC_DATAFORMAT format);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetSleepMode_send
*************************************************************************//**
*
* Sets the ADC sleep mode
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] mode Sleep mode from ADAC250_ADC_SLEEP_MODE enumeration
*
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_SetSleepMode_send(connection_state * state, ADAC250_ADC_SLEEP_MODE mode);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetOffsetCorrectionTime_send
*************************************************************************//**
*
* Sets the ADC offset correction time 
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] octa OCT from ADAC250_CHANNEL_OCT enumeration for channel A
*
* @param[in] octb OCT from ADAC250_CHANNEL_OCT enumeration for channel B
*
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_SetOffsetCorrectionTime_send(connection_state * state, ADAC250_CHANNEL_OCT octa, ADAC250_CHANNEL_OCT octb);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetTriggerDelay_send
*************************************************************************//**
*
* Sets the ADC trigger delay value
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] triggerdelay Trigger delay in ADC clock cycles (1 to 32).
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_ADC_SetTriggerDelay_send(connection_state * state, char triggerdelay);
/**
*****************************************************************************
* Function: ADAC250_MUX_ConfigClockOutput_send
*************************************************************************//**
*
* Sets the ADAC250 clock multiplexer
*
* @param[in] state Pointer to Ethernet connection
*
* @param[in] ClockToConfig Clock output selected for the mux configuration.
*
* @param[in] ClockInputSelection Clock to be routed to the selected mux output. 
*
* @return
*      Always returns 0.
*
****************************************************************************/
Result ADAC250_MUX_ConfigClockOutput_send(connection_state * state, ADAC250_CLOCK_OUTPUT_ID ClockToConfig, ADAC250_CLOCK_INPUT_ID ClockInputSelection);


#ifdef __cplusplus
}
#endif



#endif

