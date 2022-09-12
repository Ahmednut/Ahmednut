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
 * @file       eapi_mo1000.h
 * @brief      mo1000 eapi module related functions definitions
 *
 * $Date: 2015/08/10 17:10:38 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MO1000
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

#ifndef EAPI_MO1000_H__
#define EAPI_MO1000_H__

#include "eapi.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "Mo1000_defines.h"

/** @defgroup MO1000 MO1000 EAPI
 *  
 *  MO1000 module allows to control a single 8 DAC channels MO1000-8, or
 *  a dual MO1000-8 & MO1000-8E 16 DAC channels 250 MHz, fmc stack.
 *  The DAC channels have 16 bits resolution. The module can use an
 *  external/internal clock source, and external/internal (software)
 *  trigger source.    
 *               
 *  @file
 *     
 */


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 * NAME : Mo1000_Presence_send
 ************************************************************************//**
 *
 * Check for a mo1000 module presence in specified board position.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_Presence_send(connection_state * state, unsigned uBoardNum);


 /****************************************************************************
 * NAME : Mo1000_PowerUp_send
 ************************************************************************//**
 *
 * Power up a mo1000 module in specified board position.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_PowerUp_send(connection_state * state, unsigned uBoardNum);


 /****************************************************************************
 * NAME : Mo1000_Reset_send
 ************************************************************************//**
 *
 * Reset a mo1000 module (default non working state) in specified board position.
 *
 * @attention
 * The module which is the clock master in a multi modules configuration
 * must be reset and configured first before the non master module. Resetting, reconfiguring
 * the clock source or changing any external clock source frequency for the clock master will
 * requiere that the non master module is calibrated again (see Mo1000_GetClkMaster()).
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_Reset_send(connection_state * state, unsigned uBoardNum);


 /****************************************************************************
 * NAME : Mo1000_Init_send
 ************************************************************************//**
 *
 * Initialize a mo1000 module (default working state) in specified board position.
 *
 * @warning
 * - This function must be called at least once.
 * - The module must be reset with Mo1000_Reset_send() before using this function.
 *
 * @attention
 * The module which is the clock master in a multi modules configuration
 * must be initialize and configured first before the non master module. Initializing, reconfiguring
 * the clock source or changing any external clock source frequency for the clock master will
 * requiere that the non master module is calibrated again (see Mo1000_GetClkMaster()).
 *  
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_Init_send(connection_state * state, unsigned uBoardNum);


 /****************************************************************************
 * NAME : Mo1000_WriteReg_send
 ************************************************************************//**
 *
 * Write a mo1000 module peripheral register with specified values.
 *
 * @warning
 * - Call Mo1000_Init_send() before using this function.    
 * - This function is for test purpose only. It could give unpredictable effects
 *   on the correct module operation. 
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Device
 *    Peripheral device to access
 *
 * @param [in] ucAddr
 *    Register address in peripheral to access
 *
 * @param [in] uValue
 *    Register value to be written
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_WriteReg_send(connection_state * state, unsigned uBoardNum, Mo1000_eDevices Device, unsigned char ucAddr, unsigned uValue);


 /****************************************************************************
 * NAME : Mo1000_ReadReg_send
 ************************************************************************//**
 *
 * Read a mo1000 module peripheral register for specified address.
 *
 * @warning
 * - Call Mo1000_Init_send() before using this function.
 * - This function is for test purpose only.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Device
 *    Peripheral device to access
 *
 * @param [in] ucAddr
 *    Register address in peripheral to access
 *
 * @param [out] puValue
 *    Register value read
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_ReadReg_send(connection_state * state, unsigned uBoardNum, Mo1000_eDevices Device, unsigned char ucAddr, unsigned int *puValue);


 /****************************************************************************
 * NAME : Mo1000_GetStatus_send
 ************************************************************************//**
 *
 * Write a mo1000 module peripheral register with specified values.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] pStatus
 *    Module status
 *   
 * @param [out] pCompare
 *    Module test pattern errors
 *     
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_GetStatus_send(connection_state * state, unsigned uBoardNum, Mo1000_stBoardStatus *pStatus, Mo1000_stCompareErrors *pCompare);


 /****************************************************************************
 * NAME : Mo1000_GetTemperature_send
 ************************************************************************//**
 *
 * Read board pcb, DAC1 and DAC2 temperatures in specified mode (resolution of 1C or 0.1C).
 *
 * @attention
 * Board temperature is for information only. Different locations
 * on the board and environment conditions could affect the difference of temperature
 * seen on the board. User is responsible to keep the temperatures inside the allowed
 * operating condition.
 *
 * @warning
 * - When all dac channels are in power down with Mo1000_SetDacParActChannel_send(), the dac
 *   temperature cannot be read and will return AD9148_INVALIDTEMP.
 *          
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Mode
 *    This is to select the temperature reading mode (resolution of 1C or 0.1C)
 *    (see #Mo1000_eTempMode).
 *
 * @param [out] psTemp
 *    Returns the board read temperature in 2 compl format. If the choosen mode is 1C resolution,
 *    the value is directly the temp in C. If the mode is #eMo1000Temp0dot1c, the temperature
 *    returned is X 10 to allow returning an extra 0.x digit in an integer value.
 *
 * @param [out] psDac1
 *    Returns the Dac1 read temperature in 2 compl format. If the choosen mode is 1C resolution,
 *    the value is directly the temp in C. If the mode is #eMo1000Temp0dot1c, the temperature
 *    returned is X 10 to allow returning an extra 0.x digit in an integer value.
 *
 * @param [out] psDac2
 *    Returns the Dac2 read temperature in 2 compl format. If the choosen mode is 1C resolution,
 *    the value is directly the temp in C. If the mode is #eMo1000Temp0dot1c, the temperature
 *    returned is X 10 to allow returning an extra 0.x digit in an integer value.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_GetTemperature_send(connection_state * state, unsigned uBoardNum, Mo1000_eTempMode Mode, short *psTemp, short *psDac1, short *psDac2);


 /****************************************************************************
 * NAME : Mo1000_SetClockConfig_send
 ************************************************************************//**
 *
 * Setup and calculates the desired clocks configuration. Once setup, you
 * must use the function Mo1000_WriteClockConfig_send() to complete the programming
 * of the generated clock setup and makes it effective.  
 *
 * @warning
 * - Interpolation rate must be choosen before configuring clocks with this function using
 *   Mo1000_SetDacParInterpolation_send(). While it is allowed to change the interpolation
 *   mode later, it must be the same rate as the one set before configuring the clocks.
 * - For a mo1000 double stack (used in master/slave clock mode), it is assumed that
 *   MasterClkMode is set to #eMo1000MasterClkAuto for the bottom board clock configuration,
 *   and the same configuration is used for the top board (except that SrcClk must be set
 *   to #eMo1000ClkSrcBottomfmc for top adapter), to achieve correct stack clocks
 *   configuration. MasterClkMode set to #eMo1000MasterClkManual for the bottom adapter should
 *   only be used when the top adapter is anything else than a mo1000 (uFreqMasterClock could
 *   be set to 0 if no top adapter is present).          
 * - When and an internal pll loop filter cannot be calculated for the clocks requierements,
 *   an error of #eCdce62005NoFilterSolution will be returned which means there is a synthetized
 *   solution for the specified requierements, but a loop filter must be manually calculated
 *   to complete the solution. Calculated solution results could be used as inputs for a manually
 *   calculated loop filter. When the loop filter is calculated manually
 *   R2Value, R3Value, C1Value, C2Value, C3Value
 *   and ChargePump must be manually provided after calculation to complete the solution. Functions
 *   Mo1000_GetPllConfig_send() and Mo1000_SetPllConfig_send() can be used to manually calculates a new
 *   loop filter configuration (then Mo1000_WriteClockConfig_send() is used to program everything).
 *        
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] SrcClk
 *    Reference clock source used.
 *
 * @param [in] uFreqSrcClk
 *    Frequency of reference clock source when SrcClk is not #eMo1000ClkSrc125mhz
 *
 * @param [in] uFreqDacClock
 *    Frequency of dacs clock desired
 *
 * @param [in] MasterClkMode
 *    Master clock generation mode
 *
 * @param [in] uFreqMasterClock
 *    Frequency of master clock (provided to top board) when MasterClkMode is not
 *    #eMo1000MasterClkAuto. It could be set to 0 for bottom adapter when no
 *    top adapter is used. 
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetClockConfig_send(connection_state * state, unsigned uBoardNum,
                                  Mo1000_eClockSource SrcClk, unsigned uFreqSrcClk,
                                  unsigned uFreqDacClock, Mo1000_eMasterClkMode MasterClkMode,
                                  unsigned uFreqMasterClock);


 /****************************************************************************
 * NAME : Mo1000_WriteClockConfig_send
 ************************************************************************//**
 *
 * Complete the clocks programming prepared with Mo1000_SetClockConfig_send()
 * and makes it effective.  
 *
 * @warning
 * - When and an internal pll loop filter cannot be calculated for the clocks requierements,
 *   an error of #eCdce62005NoFilterSolution was returned with Mo1000_SetClockConfig_send(),
 *   which means there is a synthetized solution but a loop filter must be manually calculated
 *   to complete the solution. Calculated solution results could be used as inputs for a manually
 *   calculated loop filter. When the loop filter is calculated manually 
 *   R2Value, R3Value, C1Value, C2Value, C3Value
 *   and ChargePump must be manually provided after calculation to complete the solution. Functions
 *   Mo1000_GetPllConfig_send() and Mo1000_SetPllConfig_send() can be used to manually calculates a new
 *   loop filter configuration (then Mo1000_WriteClockConfig_send() is used to program everything).    
 *        
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_WriteClockConfig_send(connection_state * state, unsigned uBoardNum);


 /****************************************************************************
 * NAME : Mo1000_GetPllConfig_send
 ************************************************************************//**
 *
 * Get the calculated pll setup prepared with Mo1000_SetClockConfig_send(). This
 * function can be used to recover the pll information, for example to
 * manually calculates the loop filter components when not automatically
 * supported.  
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] pParam
 *    This structure will hold the pll configuration.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_GetPllConfig_send(connection_state * state, unsigned uBoardNum, Mo1000_stPllParam *pParam);


 /****************************************************************************
 * NAME : Mo1000_SetPllConfig_send
 ************************************************************************//**
 *
 * Set pll configuration. This function can be used to change the pll configuration,
 * for example to manually calculates the loop filter components when not automatically
 * supported.  
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] pParamIn
 *    This structure holds the pll configuration. Here are the members that can be updated:
 *    uVcoFreq, ucPrimaryInputDivider, usReferenceDivider, usInputDivider, usFeedbackDivider,
 *    ucBypassDivider, ucPrescaleDivider, aucOutDivider[]. These members can also be updated
 *    and are modified when a manual loop filter configuration is used: R2Value, R3Value, C1Value,
 *    C2Value, C3Value, ChargePump.
 *       
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetPllConfig_send(connection_state * state, unsigned uBoardNum, Mo1000_stPllParam *pParamIn);


 /****************************************************************************
 * NAME : Mo1000_DoDacCalibration_send
 ************************************************************************//**
 *
 * Force a digital calibration of all dac with active channels. This will
 * calibrates their data lanes, frame signal and synchronize dacs.  
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_DoDacCalibration_send(connection_state * state, unsigned uBoardNum);


 /****************************************************************************
 * NAME : Mo1000_GetChannelCalibStatus_send
 ************************************************************************//**
 *
 * Read DAC channels calibration information. This info was the last time
 * the DACs were calibrated. It will return global calibration process
 * success flag, data channels lanes which were calibrated correctly or not
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puChannelLaneCalib
 *    When puCalibStatus=0, this will return the status of data lanes calibration
 *    that could be in error. Each bit that is set to 1 means that this data lane did
 *    not calibrate correctly. The bits can be interpreted this way:
 *    3322222222221111 1111110000000000
 *    1098765432109876 5432109876543210
 *    I-----DAC2-----I I-----DAC1-----I
 *    1111110000000000 1111110000000000
 *    5432109876543210 5432109876543210
 *    If all data lanes are 0, this means the data calibration step went correctly.       
 *
 * @param [out] puChannelFrameCalib
 *    When puCalibStatus=0, this will return the status of frame lane calibration
 *    that could be in error. Each bit that is set to 1 means that this data lane did
 *    not work correctly after frame calibration. The bits can be interpreted this way:
 *    3322222222221111 1111110000000000
 *    1098765432109876 5432109876543210
 *    I-----DAC2-----I I-----DAC1-----I
 *    1111110000000000 1111110000000000
 *    5432109876543210 5432109876543210
 *    If all data lanes are 0, this means the frame calibration step went correctly.       
 *
 * @param [out] puChannelSyncCalib
 *    When puCalibStatus=0, this will return the status of dac synchronisation calibration
 *    that could be in error. Each dac with bit that is set to 1 means that this dac did
 *    not work correctly after dac synchronisation calibration. The bits can be interpreted this way:
 *    3322222222221111 1111110000000000
 *    1098765432109876 5432109876543210
 *    I-----DAC2-----I I-----DAC1-----I
 *    1111110000000000 1111110000000000
 *    5432109876543210 5432109876543210
 *    If all dacs lanes are 0, this means the dac synchronisation calibration step went correctly.       
 *
 * @param [out] puCalibStatus
 *    When !=0, this means the whole channels calibration was successfull. There
 *    are 3 steps in the calibration process: data lanes calibration, frame signal calibration
 *    and dacs synchronisation.  If puCalibStatus = 0, there was an error somewhere in the
 *    calibration steps and puChannelLaneCalib, puChannelFrameCalib, puChannelSyncCalib
 *    will give information of what calibration step that went wrong.   
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_GetChannelCalibStatus_send(connection_state * state, unsigned uBoardNum, unsigned int *puChannelLaneCalib,
                                    unsigned int *puChannelFrameCalib, unsigned int *puChannelSyncCalib,
                                    unsigned int *puCalibStatus);


 /****************************************************************************
 * NAME : Mo1000_SetDacOutCtrl_send
 ************************************************************************//**
 *
 * Control the dac output state (enable/disable) for the specified channel.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Channel
 *    This is the dac channel to change the output state.
 *
 * @param [in] State
 *    This is the desired output state.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacOutCtrl_send(connection_state * state, unsigned uBoardNum, Mo1000_eDacSelect Channel,
                                 Mo1000_eOutputState State);


 /****************************************************************************
 * NAME : Mo1000_SetDacParDcOffset_send
 ************************************************************************//**
 *
 * Control the dac output dc offset for the specified channel.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Channel
 *    This is the dac channel to change the dc offset.
 *
 * @param [in] sOffset
 *    This is the desired output dc offset.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParDcOffset_send(connection_state * state, unsigned uBoardNum, Mo1000_eDacSelect Channel,
                                     short sOffset);


 /****************************************************************************
 * NAME : Mo1000_SetDacParGain_send
 ************************************************************************//**
 *
 * Set the dac gain amplitude correction. The value is a 8 bits unsigned
 * value and each count gives a gain of 0.015625 (ScaleFactor = ucGain / 64). 
 *
 * @warning
 * - Because the gain is digital, setting the correction factor greater than
 *   64 (gain of 1) could cause signal saturation if this cause an overrange.      
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Channel
 *    This is the dac channel to change the gain.
 *
 * @param [in] ucGain
 *    Gain correction desired
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParGain_send(connection_state * state, unsigned uBoardNum, Mo1000_eDacSelect Channel,
                                 unsigned char ucGain);


 /****************************************************************************
 * NAME : Mo1000_SetDacParInterpolation_send
 ************************************************************************//**
 *
 * Control the dacs interpolation mode for all channels.
 *
 * @warning
 * - Changing interpolation rate would affect inverse sinc filter parameters that
 *   would need to be reprogrammed with Mo1000_SetDacParISinc_send().       
 * - Changing the interpolation mode that could change interpolation rate implies
 *   reconfiguring the clocks with Mo1000_SetClockConfig_send() to achieve the new rate.
 *   A complete calibration with Mo1000_DoDacCalibration_send() would also be needed in this case.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Mode
 *    This is the desired interpolation mode.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParInterpolation_send(connection_state * state, unsigned uBoardNum, 
                                          Ad9148_eInterpolationMode Mode);


 /****************************************************************************
 * NAME : Mo1000_DoDacUpdate_send
 ************************************************************************//**
 *
 * Configure the dacs for operation with all the current parameters (default
 * parameters if coming from Mo1000_Reset_send(), or user specified with
 * Mo1000_SetDacParxxx functions). This function will only update dacs parameter
 * which recently changed and are not up to date.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_DoDacUpdate_send(connection_state * state, unsigned uBoardNum);


 /****************************************************************************
 * NAME : Mo1000_SetDacParActChannel_send
 ************************************************************************//**
 *
 * Set the dac active channels. Active channels can be activated in pair,
 * the other inactive channels are powered down.
 *
 * @warning
 * - Enabling more active channels than the last calibration time will requiere
 *   a new calibration with Mo1000_DoDacCalibration_send().  
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ActChannel
 *    This is the number of active dac channels.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParActChannel_send(connection_state * state, unsigned uBoardNum, 
                                       Mo1000_eActChannel ActChannel);


 /****************************************************************************
 * NAME : Mo1000_GetClkMaster_send
 ************************************************************************//**
 *
 * Detect if this current module instance is a clock master for the FPGA (uses
 * its own clock reference). In a multiple module configuration, the FPGA clock
 * master is the one that provides it's own reference clock to the fpga from
 * which the fpga will derive it's data clock (DCI). Normally, only the module
 * instance which is defined as the clock master can use the function related
 * to clock source selection (Mo1000_SetClockConfig_send()) to select internal
 * (#eMo1000ClkSrc125mhz), external (#eMo1000ClkSrcExt) or carrier clock
 * sources. If external clock source is selected for the clock master module,
 * it must be connected on the EC signal of this module connector. The module
 * which is not the FPGA clock master can only selects as a clock source, the
 * other module (#eMo1000ClkSrcBottomfmc).        
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puMaster
 *    Returns if the mo1000 core instance is the clock master or not.
 *    Master is true (!=0) when the MO1000 fpga core instance is the clock master.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_GetClkMaster_send(connection_state * state,  unsigned uBoardNum, unsigned int *puMaster);


 /****************************************************************************
 * NAME : Mo1000_SetDacParISinc_send
 ************************************************************************//**
 *
 * Set inverse sinc filter coefficients (real and imaginary) for a dac
 * pair (real coefficients for I dac (odd dac) and imaginary for Q dac (even dac)).
 *
 * @warning
 * - Call Mo1000_Init_send() before using this function.    
 * - Dac output center frequency affects the isinc filter coefficients.
 * - Dac interpolation rate affects the isinc filter coefficients.  
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] DacPair
 *    This is the dac channel pair to change the inverse sinc filter options.
 *
 * @param [in] sRealC0
 *    Inverse sinc filter real coefficients C0 (for I dac i.e. odd dac)
 *    range [-4,3] 
 *
 * @param [in] sRealC1
 *    Inverse sinc filter real coefficients C1 (for I dac i.e. odd dac)
 *    range [-8,7] 
 *
 * @param [in] sRealC2
 *    Inverse sinc filter real coefficients C2 (for I dac i.e. odd dac)
 *    range [-16,15] 
 *
 * @param [in] sRealC3
 *    Inverse sinc filter real coefficients C3 (for I dac i.e. odd dac)
 *    range [-64,63] 
 *
 * @param [in] sRealC4
 *    Inverse sinc filter real coefficients C4 (for I dac i.e. odd dac)
 *    range [-512,511] 
 *
 * @param [in] sImgC0
 *    Inverse sinc filter imaginary coefficients C0 (for Q dac i.e. even dac)
 *    range [-4,3] 
 *
 * @param [in] sImgC1
 *    Inverse sinc filter imaginary coefficients C1 (for Q dac i.e. even dac)
 *    range [-8,7] 
 *
 * @param [in] sImgC2
 *    Inverse sinc filter imaginary coefficients C2 (for Q dac i.e. even dac)
 *    range [-16,15] 
 *
 * @param [in] sImgC3
 *    Inverse sinc filter imaginary coefficients C3 (for Q dac i.e. even dac)
 *    range [-64,63] 
 *
 * @param [in] sImgC4
 *    Inverse sinc filter imaginary coefficients C4 (for Q dac i.e. even dac)
 *    range [-512,511] 
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParISinc_send(connection_state * state, unsigned uBoardNum, Mo1000_eSelectDacPair DacPair,
                             short sRealC0, short sRealC1, short sRealC2, short sRealC3, short sRealC4,
                             short sImgC0, short sImgC1, short sImgC2, short sImgC3, short sImgC4);


 /****************************************************************************
 * NAME : Mo1000_SetDacParISincCtrl_send
 ************************************************************************//**
 *
 * Enable/disable the inverse sinc filter for the specified channels group.
 *
 * @warning
 *  
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Group
 *    This is the dac channels group to change the inverse sinc filter state.
 *
 * @param [in] State
 *    Inverse sinc filter control state desired
 *     
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParISincCtrl_send(connection_state * state, unsigned uBoardNum, 
                                      Mo1000_eDacGroupId Group, Mo1000_eIsincState State);


 /****************************************************************************
 * NAME : Mo1000_SetDacParFineMod_send
 ************************************************************************//**
 *
 * Set fine modulation options for the specified dac channels group.
 *
 * @warning
 * - Call Mo1000_Init_send() before using this function.    
 * - Call Mo1000_SetDacParFineModCtrl_send() to enable the fine modulation before
 *   configuring it with this function.      
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Group
 *    This is the dac channels group to change the fine modulation options.
 *
 * @param [in] uFtw
 *    Frequency tuning word desired (uFtw = Fcenter/Fdac X 4294967296)
 *
 * @param [in] usNcoPhaseOffset
 *    Nco phase offset adjustement desired from 0 to 360 deg 
 *    (each count add 0.0054931640625 deg)
 *
 * @param [in] Sideband
 *    Choose which sideband is desired
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParFineMod_send(connection_state * state, unsigned uBoardNum, Mo1000_eDacGroupId Group,
                                    unsigned uFtw, unsigned short usNcoPhaseOffset, Mo1000_eNcoSideband Sideband);


 /****************************************************************************
 * NAME : Mo1000_SetDacParFineModCtrl_send
 ************************************************************************//**
 *
 * Enable/disable the fine modulation (NCO) for the specified dac channels group.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Group
 *    This is the dac channels group to change the fine modulation state.
 *
 * @param [in] State
 *    Fine modulation control state desired
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParFineModCtrl_send(connection_state * state, unsigned uBoardNum, Mo1000_eDacGroupId Group,
                                        Mo1000_eFineModState State);


 /****************************************************************************
 * NAME : Mo1000_SetDacParDataFormat_send
 ************************************************************************//**
 *
 * Set the data format used for the specified dac channels group.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
  * @param [in] Group
 *    This is the dac channels group to change the data format
 *
 * @param [in] Format
 *    Which data format to use (binary or 2 complement)
 *
* @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParDataFormat_send(connection_state * state, unsigned uBoardNum, Mo1000_eDacGroupId Group,
                                       Mo1000_eDataFormat Format);


 /****************************************************************************
 * NAME : Mo1000_SetDacParQuadPhase_send
 ************************************************************************//**
 *
 * Set the quadrature phase offsets (phase between the I dac (real or odd dac)
 * and Q dac (imaginary or even dac) dacs pair. Each phase offset is in the range
 * of +/- 1.75 deg and when combined could give +/- 3.5 deg between I and Q dacs. 
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] DacPair
 *    This is the dac channel pair to change the quadrature phase options.
 *
 * @param [in] sRealPhase
 *    I dac quadrature phase offset. Each count gives 0.00342 deg offset. The value is
 *    a sign integer to give positive or negative offsets. 
 *
 * @param [in] sImaginaryPhase
 *    Q dac quadrature phase offset. Each count gives 0.00342 deg offset. The value is
 *    a sign integer to give positive or negative offsets. 
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetDacParQuadPhase_send(connection_state * state, unsigned uBoardNum, Mo1000_eSelectDacPair DacPair,
                                 short sRealPhase, short sImaginaryPhase);


 /****************************************************************************
 * NAME : Mo1000_GetVersions_send
 ************************************************************************//**
 *
 * Read mo1000 fpga core module version and driver version information.
 * This info could be used for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puCoreVersion
 *    Returns MO1000 fpga core version.
 *
 * @param [out] puDriverVersion
 *    Returns MO1000 software driver version.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_GetVersions_send(connection_state * state, unsigned uBoardNum,
                               unsigned int* puCoreVersion, unsigned int *puDriverVersion);


 /****************************************************************************
 * NAME : Mo1000_TestLocalOscSw_send
 ************************************************************************//**
 *
 * Test the local onboard clock oscillator switch control for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestLocalOscSw_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_TestBridgeReset_send
 ************************************************************************//**
 *
 * Test the bridge reset operation for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestBridgeReset_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_TestDacReset_send
 ************************************************************************//**
 *
 * Test the dacs reset operation for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestDacReset_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_TestPllSel_send
 ************************************************************************//**
 *
 * Test the pll mux selection function for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestPllSel_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_TestPllPowDown_send
 ************************************************************************//**
 *
 * Test the pll power down function for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestPllPowDown_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_TestPllSync_send
 ************************************************************************//**
 *
 * Test the pll sync function for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestPllSync_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_TestDacIrq_send
 ************************************************************************//**
 *
 * Test the dac irq internal signals for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestDacIrq_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_TestDacClks_send
 ************************************************************************//**
 *
 * Test the dac clocks internal signals for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestDacClks_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_TestDciFrame_send
 ************************************************************************//**
 *
 * Test the dci+frame paths configuration by using correct defaults or
 * forcing to use the wrong paths for diagnostic purpose.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] puResult
 *    Returns test result (0 when passed, anything else is a status code).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_TestDciFrame_send(connection_state * state, unsigned uBoardNum, unsigned int* puResult);


 /****************************************************************************
 * NAME : Mo1000_CheckI2c_send
 ************************************************************************//**
 *
 * Verified a provided scanned i2c bus vector to detect all module devices.
 * This function is used for production purpose to check if all devices for module
 * operation were detected correctly on the i2c bus. This function is not
 * used in the normal user module operation.
 *
 * @warning
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] ucScanMax
 *    This is the maximum number of devices that must be detected to avoid a scan error.
 *    When 0 is specified, this parameter is ignored. When used on a perseus fmc carrier, with a
 *    single mo1000 module, this parameter should be 5 devices.
 *
 * @param [in] pScanRaw
 *    Raw i2c scan results structure (each detected device address in the vector
 *    hold != 0 when a device is detected). This vector is built when the i2c bus is scanned
 *    using a system function (i2c_scan_devices()). 
 *
 * @param [in] iScanRawSize
 *    Size in bytes of the pScanRaw structure/buffer (must be sizeof(#Mo1000_stI2cScan)) 
 *  
 * @param [in] ucNumDevices
 *    Number of i2c devices that were detected on the bus at scan time (from i2c_scan_devices()).
 *
 * @param [out] pScanRes
 *    For each of the device types of the module, this will return != 0 if the device has been
 *    detected correctly. All device types must be detected for the module to work
 *    correctly.
 *
 * @param [out] pucDetectOk
 *    This is the global detection result. This will hold true (!=0) when all the device types have
 *    been detected and no more devices than ucScanMax are detected on the bus (ucScanMax is ignored
 *    when set to 0).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_CheckI2c_send(connection_state * state, unsigned uBoardNum, unsigned char ucScanMax,
                            Mo1000_stI2cScan *pScanRaw, int iScanRawSize, 
                            unsigned char ucNumDevices,
                            Mo1000_stI2cAddr *pScanRes, unsigned char *pucDetectOk);


 /****************************************************************************
 * NAME : Mo1000_GetCoreFrequency_send
 ************************************************************************//**
 *
 * Read the fpga core reference and dacs reference clocks frequency for diagnostic purpose.
 *
 * @warning
 * - pusDacRefClock would only be valid if the dac clocks were setup with
 *   Mo1000_SetClockConfig_send() & Mo1000_WriteClockConfig_send() before using this function.
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [out] pusCoreRefClock
 *    Returns the FPGA core reference clock frequency mesured in MHz.
 *
 * @param [out] pusDacRefClock
 *    Returns the FPGA dacs reference clock frequency mesured in MHz.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_GetCoreFrequency_send(connection_state * state, unsigned uBoardNum,
                                    unsigned short* pusCoreRefClock,
                                    unsigned short* pusDacRefClock);


 /****************************************************************************
 * NAME : Mo1000_SetTestMode_send
 ************************************************************************//**
 *
 * This function could be used to configure special test mode (mostly use at production time
 * for test purpose).
 * Here are some details about possible test modes:
 *
 * #eMo1000TestModeoff:
 * All test modes are disabled.
 *
 * #eMo1000TestMode2:
 * When test mode is active, each dac channel group would captured user's specified data patterns
 * generated by the fpga core.
 * This is usefull to verify the reliability of the complete digital data generation chain.
 * The module Mo1000_DoDacCalibration_send() function must have been done successfully last time
 * before using this function.
 * While test mode is active, user can poll for pattern test errors using Mo1000_GetStatus_send()
 * function pCompare output parameter as often/long as needed. Errors are cumulative between
 * Mo1000_GetStatus_send() calls.  
 *
 * #eMo1000TestMode1:
 * This mode is similar as #eMo1000TestMode2 explained above, but is mainly reserved for
 * production purpose.
 * While test mode is active, user can poll for pattern test errors using Mo1000_GetStatus_send()
 * function pCompare output parameter as often/long as needed. Errors are cumulative between
 * Mo1000_GetStatus_send() calls.  
 * @attention
 * The dac data captured in this mode is not necessary the specified fpga patterns
 * because it could be done uncalibrated (so this mode could not be used to verify the
 * reliability of the complete generated data chain).
 *
 * For normal board DAC operation, test mode must be disable after use.
 *
 * @warning
 * - Doing Mo1000_DoDacCalibration_send() will disable the test mode automatically and revert to normal mode.
 * - The module Mo1000_Init_send() function must be call at least once before using this function.
 * - To test all bits correctly, patterns must be provided that a bit transition occurs between
 *   the even and odd patterns on each bits that need to be tested.
 * - All dac channels group are tested at the same time.   
 * - Dacs Clocks must be initialized correctly with Mo1000_SetClockConfig_send() before testing patterns.    
 *
 * @param [in] state
 *    Communication state information
 *
 * @param [in] uBoardNum
 *    Board instance to control (1st one is 1)
 *
 * @param [in] Test
 *    Test mode desired.
 *
 * @param [in] usEvenPattern
 *    16 bits even test pattern used.
 *
 * @param [in] usOddPattern
 *    16 bits odd test pattern used.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 ***************************************************************************/ 
Result Mo1000_SetTestMode_send(connection_state * state, unsigned uBoardNum, Mo1000_eTestMode Test, 
                               unsigned short usEvenPattern, unsigned short usOddPattern);


#ifdef __cplusplus
}
#endif



#endif

