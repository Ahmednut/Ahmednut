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
 * @file     adac250_user.h 
 * @brief    Functions controlling the ADAC250 module
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADAC250
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
#ifndef __ADAC250USER__
#define __ADAC250USER__
#include "bas_error.h"
#include "adac250.h"
#include "adac250_defines.h"
#include "adac250_types.h"



/**
*****************************************************************************
* Function: ADAC250_DAC_Init
*************************************************************************//**
*
* Initializes the D/A converter tp the specified values
*
* @param[in] board Pointer to ADAC250 structure
*
* @param[in] sleep Powers down DAC A, DAC B, or both
*
* @param[in] interpolation D/A converter interpolation mode
*
* @param[in] mixmode0 Mix mode for I-Q signals. Refer to the DAC5682 datasheet.
*
* @param[in] mixmode1 Mix mode for I-Q signals. Refer to the DAC5682 datasheet.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_DAC_Init(ADAC250Board_t * board, ADAC250_DAC_SLEEP sleep, ADAC250_DAC_INTERPOLATION interpolation, ADAC250_DAC_CMIX_MODE mixmode0, ADAC250_DAC_CMIX_MODE mixmode1);

/**
*****************************************************************************
* Function: ADAC250_DAC_Calibrate
*************************************************************************//**
*
* Calibrates the digital communication between the FPGA and the DAC
*
* @param[in] board Pointer to the ADAC250 structure
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_DAC_Calibrate(ADAC250Board_t * board);

/**
*****************************************************************************
* Function: ADAC250_DAC_SetGain
*************************************************************************//**
*
* Configures the D/A conversion gain.
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] channel Channel A or B
*
* @param[in] gain 4-bit gain
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_DAC_SetGain(ADAC250Board_t * board, ADAC250_DAC_CHANNEL channel, int gain);

/**
*****************************************************************************
* Function: ADAC250_DAC_SetOffset
*************************************************************************//**
*
* Configures the D/A conversion offset. This value is added to the data stream values.
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] channel Channel A or B
*
* @param[in] offset 13-bit offset
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_DAC_SetOffset(ADAC250Board_t * board, ADAC250_DAC_CHANNEL channel, int offset);

/**
*****************************************************************************
* Function: ADAC250_DAC_Sync
*************************************************************************//**
*
* Enables or disables DAC outputs
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] sync Sync enable. Either ADAC250_DAC_SYNC_OFF or ADAC250_DAC_SYNC_ON
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_DAC_Sync(ADAC250Board_t * board, ADAC250_DAC_SYNC sync);

/**
*****************************************************************************
* Function: ADAC250_DAC_SyncSource
*************************************************************************//**
*
* Selects source for the sync signal. Either from FPGA or from DAC registers
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] syncsource Sync source. Either ADAC250_DAC_SYNC_SOURCE_FPGA or ADAC250_DAC_SYNC_SOURCE_DACREG
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_DAC_SyncSource(ADAC250Board_t * board, ADAC250_DAC_SYNC_SOURCE syncsource);

/**
*****************************************************************************
* Function: ADAC250_InitPLL
*************************************************************************//**
*
* Resets and initializes the ADAC250 PLL (AD9511)
*
* @param[in] board Pointer to the ADAC250 structure
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
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
adp_result_t ADAC250_InitPLL(ADAC250Board_t * board, unsigned int refclk, unsigned int adcclk, unsigned int dacclk, ADAC250_DAC_INTERPOLATION interp, ADAC250_CLOCK_SOURCES clksrc);

/**
*****************************************************************************
* Function: ADAC250_PLL_getStatus
*************************************************************************//**
*
* Returns the current PLL status.
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[out] status Returns ADAC250_PLL_LOCKED(1) when locked, ADAC250_PLL_UNLOCKED(0) when unlocked   
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_PLL_getStatus(ADAC250Board_t * board, ADAC250_PLL_STATUS * status);

Result ADAC250_PLL_CalculateParams(unsigned refClk, unsigned AdcClk, unsigned DacDataClk, ADAC250_DAC_INTERPOLATION interpolation, ADAC250_PLL_PARAM * param);
/**
*****************************************************************************
* Function: ADAC250_PLL_SetSleep
*************************************************************************//**
*
* Configures power down states
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] sleep Powerdown part of the AD9511 PLL. Check PLL_SLEEP structure for more detail.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_PLL_SetSleep(ADAC250Board_t * board, ADAC250_PLL_SLEEP sleep);

/**
*****************************************************************************
* Function: ADAC250_PLL_setRefClkTuning
*************************************************************************//**
*
* Tunes the reference clock
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] fValue Floating point value.
*   Its range is from -1.0f to 1.0f. This range covers the full oscillator
*   tuning range.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_PLL_setRefClkTuning(ADAC250Board_t * board, float fValue);


/**
*****************************************************************************
* Function: ADAC250_ADC_init
*************************************************************************//**
*
* Initizlizes the A/D converter at the default ADAC250 values
*
* @param[in] board Pointer to the ADAC250 structure
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_init(ADAC250Board_t * board);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetDataFormat
*************************************************************************//**
*
* Sets the ADC data format. 
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] format Format from ADAC250_ADC_DATAFORMAT enumeration
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_SetDataFormat(ADAC250Board_t * board, ADAC250_ADC_DATAFORMAT format);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetOffsetCorrectionTime
*************************************************************************//**
*
* Sets the ADC Offset correction time
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] octa Offset correction time for channel A from ADAC250_CHANNEL_OCT enumeration
*
* @param[in] octb Offset correction time for channel B from ADAC250_CHANNEL_OCT enumeration
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_SetOffsetCorrectionTime(ADAC250Board_t * board,  ADAC250_CHANNEL_OCT octa, ADAC250_CHANNEL_OCT octb);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetSleepMode
*************************************************************************//**
*
* Sets the ADC sleep mode. 
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] mode Sleep mode from ADAC250_ADC_SLEEP_MODE enumeration
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_SetSleepMode(ADAC250Board_t * board, ADAC250_ADC_SLEEP_MODE mode);

/**
*****************************************************************************
* Function: ADAC250_ADC_setGain
*************************************************************************//**
*
* Configures the A/D conversion gain
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] channel Channel A or B
*
* @param[in] gain A/D conversion gain. See ADC_GAIN for possible values
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_setGain(ADAC250Board_t * board, ADAC250_ADC_CHANNEL channel, ADAC250_ADC_GAIN gain);

/**
*****************************************************************************
* Function: ADAC250_ADC_setFineGain
*************************************************************************//**
*
* Configures the fine A/D conversion gain
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] channel Channel A or B
*
* @param[in] gain Fine A/D conversion gain. 128 steps. +0.001 dB to +0.134 dB
*      fine gain (dB) = 20*log10[1 + (gain/8192)]
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_setFineGain(ADAC250Board_t * board, ADAC250_ADC_CHANNEL channel, char gain);

/**
*****************************************************************************
* Function: ADAC250_ADC_setPedestal
*************************************************************************//**
*
* Configures the A/D conversion pedestal
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] channel Channel A or B
*
* @param[in] value 6-bit pedestal value. Refer to the ADS62P49 datasheet.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_setPedestal(ADAC250Board_t * board, ADAC250_ADC_CHANNEL channel, char value);

/**
*****************************************************************************
* Function: ADAC250_ADC_Calibrate
*************************************************************************//**
*
* Calibrates the digital communication between ADCs and the FPGA
*
* @param[in] board Pointer to the ADAC250 structure
*
* @return
*      Returns 0 if the calibration succeed 
*      otherwise -1.
*
****************************************************************************/
Result ADAC250_ADC_Calibrate(ADAC250Board_t * board);

/**
*****************************************************************************
* Function: ADAC250_ADC_getStatus
*************************************************************************//**
*
* Acquire the A/D converter status. Verify whether an overange has occured.
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] channel Channel A or B
*
* @param[out] status Returns 0 for normal, 1 for overrange.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_getStatus(ADAC250Board_t * board, ADAC250_ADC_CHANNEL channel, ADAC250_ADC_STATUSES * status);

/**
*****************************************************************************
* Function: ADAC250_ADC_SetAdjTriggerDelay
*************************************************************************//**
*
* Adjusts the trigger delay. 
* The trigger from the trig in connector can be delay for 1 to 32 ADC clock cycles
*
* @param[in] board Pointer to the ADAC250 structure
*
* @param[in] delay Delay in ADC clock cycles
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_ADC_SetAdjTriggerDelay(ADAC250Board_t * board, char delay);

/**
*****************************************************************************
* Function: ADAC250_MUX_ConfigClockOutput
*************************************************************************//**
*
* Configures the clock multiplexer.
*
* @param[in]  board
*      Pointer to an ADAC250Board structure describing the device to use.
*
* @param[in]   ClockToConfig
*      Clock output to configure.
*
* @param[in]   ClockInputSelection
*      Clock input signal to use.
*
* @return
*      Return success or failure result. See adac250_error enum.
*
****************************************************************************/
Result ADAC250_MUX_ConfigClockOutput(ADAC250Board_t * board, ADAC250_CLOCK_OUTPUT_ID ClockToConfig, ADAC250_CLOCK_INPUT_ID ClockInputSelection);


#endif
