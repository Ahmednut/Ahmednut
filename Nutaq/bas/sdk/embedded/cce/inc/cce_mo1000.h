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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       cce_mo1000.h
 * @brief      cce mo1000 module related function definitions
 *
 * $Date: 2015/05/19 14:40:21 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MO1000_CCE
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

#ifndef CCE_MO1000_H__
#define CCE_MO1000_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "cce.h"
#include "bas_error.h"


/** @defgroup MO1000_CCE CCE MO1000 Module
 *  
 *  CCE MO1000 module allows to control a Mo1000 through a communication
 *  environment.
 *               
 *  @file
 *     
 */

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 * NAME : Mo1000_Presence_recv
 ************************************************************************//**
 *
 * Check for a mo1000 module presence in specified board position.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:   module instance to be verified 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *
 ***************************************************************************/ 
Result Mo1000_Presence_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_PowerUp_recv
 ************************************************************************//**
 *
 * Power up a mo1000 module in specified board position.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:   module instance to be used 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *
 ***************************************************************************/ 
Result Mo1000_PowerUp_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_Reset_recv
 ************************************************************************//**
 *
 * Reset a mo1000 module (default non working state) in specified board position.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:   module instance to be used 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *
 ***************************************************************************/ 
Result Mo1000_Reset_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_Init_recv
 ************************************************************************//**
 *
 * Initialize a mo1000 module (default working state) in specified board position.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:   module instance to be used 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_Init_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_WriteReg_recv
 ************************************************************************//**
 *
 * Write a mo1000 module peripheral register with specified values.
 *
 * @warning
 * - This function is for test purpose only and could corrupt module operation. 
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:            module instance to be used 
 *   [in] Mo1000_eDevices Device:  peripheral device to be used 
 *   [in] unsigned char ucAddr:    address of peripheral to be used 
 *   [in] unsigned uValue:         register value to be written 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_WriteReg_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_ReadReg_recv
 ************************************************************************//**
 *
 * Read a mo1000 module peripheral register for specified address.
 *
 * @warning
 * - This function is for test purpose only. 
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:            module instance to be used 
 *   [in] Mo1000_eDevices Device:  peripheral device to be used 
 *   [in] unsigned char ucAddr:    address of peripheral to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uValue:        register value read 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_ReadReg_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_GetStatus_recv
 ************************************************************************//**
 *
 * Write a mo1000 module peripheral register with specified values.
 *
 * @warning
 * - This function is for test purpose only and could corrupt module operation. 
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used 
 * - Communication packet sent will include:
 *   [out] Mo1000_stBoardStatus Status:    module status 
 *   [out] Mo1000_stCompareErrors Compare: module test pattern errors 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_GetStatus_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_GetTemperature_recv
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
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used 
 *   [in] Mo1000_eTempMode Mode:        temperature mode 
 * - Communication packet sent will include:
 *   [out] short sTemp:                 board temperature 
 *   [out] short sDac1:                 dac1 temperature 
 *   [out] short sDac2:                 dac2 temperature 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_GetTemperature_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetClockConfig_recv
 ************************************************************************//**
 *
 * Setup and calculates the desired clocks configuration. Once setup, you
 * must use the function Mo1000_WriteClockConfig_recv() to complete the programming
 * of the generated clock setup and makes it effective.  
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Interpolation rate must be choosen before configuring clocks with this function using
 *   Mo1000_SetDacParInterpolation_recv(). While it is allowed to change the interpolation
 *   mode later, it must be the same rate as the one set before configuring the clocks.
 * - For a mo1000 double stack (used in master/slave clock mode), it is assumed that
 *   MasterClkMode is set to eMo1000MasterClkAuto for the bottom board clock configuration,
 *   and the same configuration is used for the top board (except that SrcClk must be set
 *   to eMo1000ClkSrcBottomfmc for top adapter), to achieve correct stack clocks
 *   configuration. MasterClkMode set to eMo1000MasterClkManual for the bottom adapter should
 *   only be used when the top adapter is anything else than a mo1000 (uFreqMasterClock could
 *   be set to 0 if no top adapter is present).          
 * - When and an internal pll loop filter cannot be calculated for the clocks requierements,
 *   an error of #eCdce62005NoFilterSolution will be returned which means there is a synthetized
 *   solution for the specified requierements, but a loop filter must be manually calculated
 *   to complete the solution. Calculated solution results could be used as inputs for a manually
 *   calculated loop filter. When the loop filter is calculated manually
 *   the stParam members R2Value, R3Value, C1Value, C2Value, C3Value
 *   and ChargePump must be manually provided after calculation to complete the solution. Functions
 *   Mo1000_GetPllConfig_recv() and Mo1000_SetPllConfig_recv() can be used to manually calculates a new
 *   loop filter configuration (then Mo1000_WriteClockConfig_recv() is used to program everything).    
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] Mo1000_eClockSource SrcClk: Reference clock source used
 *   [in] unsigned uFreqSrcClk:       Frequency of reference clock source when SrcClk is not eMo1000ClkSrc125Mhz
 *   [in] unsigned uFreqDacClock:     Frequency of dacs clock desired
 *   [in] Mo1000_eMasterClkMode MasterClkMode: Master clock generation mode
 *   [in] unsigned uFreqMasterClock:  Frequency of master clock (provided to top board) when MasterClkMode is not eMo1000MasterClkAuto. It could be set to 0 for bottom adapter when no top adapter is used.
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(), Mo1000_SetDacParInterpolation_recv(). 
 *
 * @see
 *    Mo1000_WriteClockConfig_recv().
 *
 ***************************************************************************/ 
Result Mo1000_SetClockConfig_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_WriteClockConfig_recv
 ************************************************************************//**
 *
 * Complete the clocks programming prepared with Mo1000_SetClockConfig_recv()
 * and makes it effective.  
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - When and an internal pll loop filter cannot be calculated for the clocks requierements,
 *   an error of #eCdce62005NoFilterSolution was returned with Mo1000_SetClockConfig_recv(),
 *   which means there is a synthetized solution but a loop filter must be manually calculated
 *   to complete the solution. Calculated solution results could be used as inputs for a manually
 *   calculated loop filter. When the loop filter is calculated manually 
 *   the stParam members R2Value, R3Value, C1Value, C2Value, C3Value
 *   and ChargePump must be manually provided after calculation to complete the solution. Functions
 *   Mo1000_GetPllConfig_recv() and Mo1000_SetPllConfig_recv() can be used to manually calculates a new
 *   loop filter configuration (then Mo1000_WriteClockConfig_recv() is used to program everything).    
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(), Mo1000_SetDacParInterpolation_recv(),
 *    Mo1000_SetClockConfig_recv(). 
 *
 * @see
 *    Mo1000_SetClockConfig_recv(), Mo1000_GetPllConfig_recv(), Mo1000_SetPllConfig_recv().
 *
 ***************************************************************************/ 
Result Mo1000_WriteClockConfig_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_GetPllConfig_recv
 ************************************************************************//**
 *
 * Get the calculated pll setup prepared with Mo1000_SetClockConfig_recv(). This
 * function can be used to recover the pll information, for example to
 * manually calculates the loop filter components when not automatically
 * supported.  
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                                  module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uPrimRefFreq:                        Primary pll reference frequency
 *   [out] unsigned uSecRefFreq:                         Secondary pll reference frequency
 *   [out] Cdce62005_eOutRefMode OutputRef[CDCE62005_NPLL]: Output divider reference config
 *   [out] unsigned uVcoFreq:                            Calculated vco frequency
 *   [out] unsigned char  ucPrimaryInputDivider:         Calculated primary input pre divider (0 disable,
 *   [out] unsigned short usReferenceDivider:            Calculated reference divider (means divide by 1 
 *   [out] unsigned short usInputDivider:                Calculated input divider                        
 *   [out] unsigned short usFeedbackDivider:             Calculated feedback divider                     
 *   [out] unsigned char  ucBypassDivider:               Calculated bypass divider                       
 *   [out] unsigned char ucPrescaleDivider:              Calculated prescaler divider                    
 *   [out] unsigned char aucOutDivider[CDCE62005_NPLL]:  Calculated outputs dividers                     
 *   [out] Cdce62005_eR2Values R2Value:                  Calculated loop filter R2 value                 
 *   [out] Cdce62005_eR3Values R3Value:                  Calculated loop filter R3 value                 
 *   [out] Cdce62005_eC1Values C1Value:                  Calculated loop filter C1 value                 
 *   [out] Cdce62005_eC2Values C2Value:                  Calculated loop filter C2 value                 
 *   [out] Cdce62005_eC3Values C3Value:                  Calculated loop filter C3 value                 
 *   [out] Cdce62005_eChargePump ChargePump:             Calculated loop filter charge pump current value
 *                 
 * @param [in] state
 *    Communication state information
 *   
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(), Mo1000_SetDacParInterpolation_recv(),
 *    Mo1000_SetClockConfig_recv().
 *    
 * @see
 *    Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/ 
Result Mo1000_GetPllConfig_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetPllConfig_recv
 ************************************************************************//**
 *
 * Set pll configuration. This function can be used to change the pll configuration,
 * for example to manually calculates the loop filter components when not automatically
 * supported.  
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                                 module instance to be used 
 *   [in] unsigned uPrimRefFreq:                        Primary pll reference frequency (ignored)
 *   [in] unsigned uSecRefFreq:                         Secondary pll reference frequency (ignored)
 *   [in] Cdce62005_eOutRefMode OutputRef[CDCE62005_NPLL]: Output divider reference config (ignored)
 *   [in] unsigned uVcoFreq:                            Calculated vco frequency
 *   [in] unsigned char  ucPrimaryInputDivider:         Calculated primary input pre divider (0 disable,
 *   [in] unsigned short usReferenceDivider:            Calculated reference divider (means divide by 1 
 *   [in] unsigned short usInputDivider:                Calculated input divider                        
 *   [in] unsigned short usFeedbackDivider:             Calculated feedback divider                     
 *   [in] unsigned char  ucBypassDivider:               Calculated bypass divider                       
 *   [in] unsigned char ucPrescaleDivider:              Calculated prescaler divider                    
 *   [in] unsigned char aucOutDivider[CDCE62005_NPLL]:  Calculated outputs dividers                     
 *   [in] Cdce62005_eR2Values R2Value:                  Calculated loop filter R2 value                 
 *   [in] Cdce62005_eR3Values R3Value:                  Calculated loop filter R3 value                 
 *   [in] Cdce62005_eC1Values C1Value:                  Calculated loop filter C1 value                 
 *   [in] Cdce62005_eC2Values C2Value:                  Calculated loop filter C2 value                 
 *   [in] Cdce62005_eC3Values C3Value:                  Calculated loop filter C3 value                 
 *   [in] Cdce62005_eChargePump ChargePump:             Calculated loop filter charge pump current value
 *                 
 * @param [in] state
 *    Communication state information
 *   
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(), Mo1000_SetDacParInterpolation_recv(),
 *    Mo1000_SetClockConfig_recv(), Mo1000_GetPllConfig_recv().
 *    
 * @see
 *    Mo1000_GetPllConfig_recv(),Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/ 
Result Mo1000_SetPllConfig_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_DoDacCalibration_recv
 ************************************************************************//**
 *
 * Force a digital calibration of all dac with active channels. This will
 * calibrates their data lanes, frame signal and synchronize dacs.  
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_DoDacCalibration_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_GetChannelCalibStatus_recv
 ************************************************************************//**
 *
 * Read DAC channels calibration information. This info was the last time
 * the DACs were calibrated. It will return global calibration process
 * success flag, data channels lanes which were calibrated correctly or not
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uChannelLaneCalib:  When uCalibStatus=0, this will return the status of data lanes calibration
 *                                      that could be in error 
 *   [out] unsigned uChannelFrameCalib: When uCalibStatus=0, this will return the status of frame lane calibration
 *                                      that could be in error 
 *   [out] unsigned uChannelSyncCalib:  When uCalibStatus=0, this will return the status of dac synchronisation calibration
 *                                      that could be in error 
 *   [out] unsigned uCalibStatus:       When !=0, this means the whole channels calibration was successfull. There
 *                                      are 3 steps in the calibration process: data lanes calibration, frame signal calibration
 *                                      and dacs synchronisation.  If uCalibStatus = 0, there was an error somewhere in the
 *                                      calibration steps and uChannelLaneCalib, uChannelFrameCalib, uChannelSyncCalib
 *                                      will give information of what calibration step that went wrong. 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *
 ***************************************************************************/ 
Result Mo1000_GetChannelCalibStatus_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacOutCtrl_recv
 ************************************************************************//**
 *
 * Control the dac output state (enable/disable) for the specified channel.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] Mo1000_eDacSelect Channel:  dac channel to change the output state
 *   [in] Mo1000_eOutputState State:  desired output state
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_SetDacOutCtrl_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParDcOffset_recv
 ************************************************************************//**
 *
 * Control the dac output dc offset for the specified channel.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] Mo1000_eDacSelect Channel:  dac channel
 *   [in] short sOffset:              desired output dc offset
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParDcOffset_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParGain_recv
 ************************************************************************//**
 *
 * Set the dac gain amplitude correction. The value is a 8 bits unsigned
 * value and each count gives a gain of 0.015625 (ScaleFactor = ucGain / 64). 
 *
 * @warning
 * - Because the gain is digital, setting the correction factor greater than
 *   64 (gain of 1) could cause signal saturation if this cause an overrange.      
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] Mo1000_eDacSelect Channel:  dac channel
 *   [in] unsigned char ucGain:       desired output gain correction desired
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParGain_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParInterpolation_recv
 ************************************************************************//**
 *
 * Control the dacs interpolation mode for all channels.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] unsigned uMode:             desired channels interpolation mode
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParInterpolation_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_DoDacUpdate_recv
 ************************************************************************//**
 *
 * Configure the dacs for operation with all the current parameters (default
 * parameters if coming from Mo1000_Reset_recv(), or user specified with
 * Mo1000_SetDacParxxx functions). This function will only update dacs parameter
 * which recently changed and are not up to date.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_DoDacUpdate_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParActChannel_recv
 ************************************************************************//**
 *
 * Set the dac active channels. Active channels can be activated in pair,
 * the other inactive channels are powered down.
 *
 * @warning
 * - Enabling more active channels than the last calibration time will requiere
 *   a new calibration with Mo1000_DoDacCalibration_recv().  
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] unsigned uActChannel:       number of active dac channels
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParActChannel_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_GetClkMaster_recv
 ************************************************************************//**
 *
 * Detect if this current module instance is a clock master for the FPGA (uses
 * its own clock reference). In a multiple module configuration, the FPGA clock
 * master is the one that provides it's own reference clock to the fpga from
 * which the fpga will derive it's data clock (DCI). Normally, only the module
 * instance which is defined as the clock master can use the function related
 * to clock source selection (Mo1000_SetClockConfig_recv()) to select internal
 * (#eMo1000ClkSrc125mhz), external (#eMo1000ClkSrcExt) or carrier clock
 * sources. If external clock source is selected for the clock master module,
 * it must be connected on the EC signal of this module connector. The module
 * which is not the FPGA clock master can only selects as a clock source, the
 * other module (#eMo1000ClkSrcBottomfmc).        
 *
 * @attention
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned int master:         !=0 when instance is a clock master 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *
 * @see
 *    Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/ 
Result Mo1000_GetClkMaster_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParISinc_recv
 ************************************************************************//**
 *
 * Set inverse sinc filter coefficients (real and imaginary) for a dac
 * pair (real coefficients for I dac (odd dac) and imaginary for Q dac (even dac)).
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.    
 * - Dac output center frequency affects the isinc filter coefficients.
 * - Dac interpolation rate affects the isinc filter coefficients.  
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                           module instance to be used 
 *   [in] Mo1000_eSelectDacPair DacPair:          dac channel pair to change inverse sinc filter options
 *   [in] short sRealC0:                          Inverse sinc filter real coefficients C0
 *   [in] short sRealC1:                          Inverse sinc filter real coefficients C1
 *   [in] short sRealC2:                          Inverse sinc filter real coefficients C2
 *   [in] short sRealC3:                          Inverse sinc filter real coefficients C3
 *   [in] short sRealC4:                          Inverse sinc filter real coefficients C4
 *                                                (for I dac i.e. odd dac)
 *   [in] short sImgC0:                           Inverse sinc filter imaginary coefficients C0
 *   [in] short sImgC1:                           Inverse sinc filter imaginary coefficients C1
 *   [in] short sImgC2:                           Inverse sinc filter imaginary coefficients C2
 *   [in] short sImgC3:                           Inverse sinc filter imaginary coefficients C3
 *   [in] short sImgC4:                           Inverse sinc filter imaginary coefficients C4
 *                                                (for Q dac i.e. even dac)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 * @see
 *    Mo1000_SetDacParInterpolation_recv(), Mo1000_SetDacParISincCtrl_recv().
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParISinc_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParISincCtrl_recv
 ************************************************************************//**
 *
 * Enable/disable the inverse sinc filter for the specified channels group.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] Mo1000_eDacGroupId Group:   dac channels group to change the inverse sinc filter state
 *   [in] Mo1000_eIsincState State:   Inverse sinc filter control state desired
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 * @see
 *    Mo1000_SetDacParISinc_recv().
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParISincCtrl_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParFineMod_recv
 ************************************************************************//**
 *
 * Set fine modulation options for the specified dac channels group.
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.    
 * - Call Mo1000_SetDacParFineModCtrl_recv() to enable the fine modulation before
 *   configuring it with this function.      
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] Mo1000_eDacGroupId Group:   dac channels group to change the fine modulation options
 *   [in] unsigned uFtw:              Frequency tuning word desired (uFtw = Fcenter/Fdac X 4294967296)
 *   [in] unsigned short usNcoPhaseOffset: Nco phase offset adjustement desired from 0 to 360 deg
 *                                         (each count add 0.0054931640625 deg)
 *   [in] Mo1000_eNcoSideband Sideband:    Choose which sideband is desired
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(),Mo1000_SetDacParFineModCtrl_recv(). 
 *
 * @see
 *    Mo1000_SetDacParFineModCtrl_recv(), Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParFineMod_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParFineModCtrl_recv
 ************************************************************************//**
 *
 * Enable/disable the fine modulation (NCO) for the specified dac channels group.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] Mo1000_eDacGroupId Group:   dac channels group to change the fine modulation state
 *   [in] Mo1000_eFineModState State: Fine modulation control state desired
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 * @see
 *    Mo1000_SetDacParFineMod_recv().
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParFineModCtrl_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParDataFormat_recv
 ************************************************************************//**
 *
 * Set the data format used for the specified dac channels group.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:               module instance to be used 
 *   [in] Mo1000_eDacGroupId Group:   dac channels group to change the data format
 *   [in] Mo1000_eDataFormat Format:  Which data format to use (binary or 2 complement)
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 * @see
 *    Mo1000_SetDacParFineMod_recv().
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParDataFormat_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetDacParQuadPhase_recv
 ************************************************************************//**
 *
 * Set the quadrature phase offsets (phase between the I dac (real or odd dac)
 * and Q dac (imaginary or even dac) dacs pair. Each phase offset is in the range
 * of +/- 1.75 deg and when combined could give +/- 3.5 deg between I and Q dacs. 
 *
 * @warning
 * - Call Mo1000_Init_recv() before using this function.    
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                  module instance to be used 
 *   [in] Mo1000_eSelectDacPair DacPair: dac channel pair to change the quadrature phase options
 *   [in] short sRealPhase:              I dac quadrature phase offset. Each count gives
 *                                       0.00342 deg offset. The value is a sign integer to give
 *                                       positive or negative offsets.
 *   [in] short sImaginaryPhase:         Q dac quadrature phase offset. Each count gives
 *                                       0.00342 deg offset. The value is a sign integer to give
 *                                       positive or negative offsets.
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 * @see
 *
 ***************************************************************************/ 
Result Mo1000_SetDacParQuadPhase_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_GetVersions_recv
 ************************************************************************//**
 *
 * Read mo1000 fpga core module version and driver version information.
 * This info could be used for diagnostic purpose.
 *
 * @attention
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned pCoreVersion:       MO1000 fpga core version 
 *   [out] unsigned pDriverVersion:     MO1000 software driver version
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *
 ***************************************************************************/ 
Result Mo1000_GetVersions_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestLocalOscSw_recv
 ************************************************************************//**
 *
 * Test the local onboard clock oscillator switch control for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestLocalOscSw_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestBridgeReset_recv
 ************************************************************************//**
 *
 * Test the bridge reset operation for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestBridgeReset_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestDacReset_recv
 ************************************************************************//**
 *
 * Test the dacs reset operation for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestDacReset_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestPllSel_recv
 ************************************************************************//**
 *
 * Test the pll mux selection function for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestPllSel_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestPllPowDown_recv
 ************************************************************************//**
 *
 * Test the pll power down function for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestPllPowDown_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestPllSync_recv
 ************************************************************************//**
 *
 * Test the pll sync function for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestPllSync_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestDacIrq_recv
 ************************************************************************//**
 *
 * Test the dac irq internal signals for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestDacIrq_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestDacClks_recv
 ************************************************************************//**
 *
 * Test the dac clocks internal signals for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestDacClks_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_TestDciFrame_recv
 ************************************************************************//**
 *
 * Test the dci+frame paths configuration by using correct defaults or
 * forcing to use the wrong paths for diagnostic purpose.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:       module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned uResult:  test result (0 when passed, anything else is a status code) 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_TestDciFrame_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_CheckI2c_recv
 ************************************************************************//**
 *
 * Verified a provided scanned i2c bus vector to detect all module devices.
 * This function is used for production purpose to check if all devices for module
 * operation were detected correctly on the i2c bus. This function is not
 * used in the normal user module operation.
 *
 * @attention
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used 
 *   [in] unsigned char ucScanMax:      This is the maximum number of devices that must be detected
 *                                      to avoid a scan error. When 0 is specified, this parameter
 *                                      is ignored. When used on a perseus fmc carrier, with a
 *                                      single mo1000 module, this parameter should be 5 devices.  
 *   [in] Mo1000_stI2cScan ScanRaw:     Raw i2c scan results structure (each detected device
 *                                      address in the vector hold != 0 when a device is detected).
 *                                      This vector is built when the i2c bus is scanned using a
 *                                      system function (i2c_scan_devices()). 
 *   [in] unsigned char ucNumDevices:   Number of i2c devices that were detected on the bus at
 *                                      scan time (from i2c_scan_devices()). 
 * - Communication packet sent will include:
 *   [out] Mo1000_stI2cAddr ScanRes:    For each of the device types of the module, this will
 *                                      return != 0 if the device has been detected correctly.
 *                                      All device types must be detected for the module to work correctly. 
 *   [out] unsigned char ucDetectOk:    This is the global detection result. This will hold true (!=0)
 *                                      when all the device types havebeen detected and no more
 *                                      devices than ucScanMax are detected on the bus (ucScanMax
 *                                      is ignored when set to 0). 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 ***************************************************************************/ 
Result Mo1000_CheckI2c_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_GetCoreFrequency_recv
 ************************************************************************//**
 *
 * Read the fpga core reference and dacs reference clocks frequency for diagnostic purpose.
 *
 * @attention
 * - usDacRefClock would only be valid if the dac clocks were setup with
 *   Mo1000_SetClockConfig_recv() before using this function.
 *
 * @warning
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used 
 * - Communication packet sent will include:
 *   [out] unsigned short usCoreRefClock: FPGA core reference clock frequency mesured in MHz 
 *   [out] unsigned short usDacRefClock:  FPGA dacs reference clock frequency mesured in MHz 
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 * @see
 *    Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/ 
Result Mo1000_GetCoreFrequency_recv(connection_state * state);


 /****************************************************************************
 * NAME : Mo1000_SetTestMode_recv
 ************************************************************************//**
 *
 * This function could be used to configure special test mode (mostly use at production time
 * for test purpose).
 * Here are some details about possible test modes:
 *
 * #eMo1000TestModeOff:
 * All test modes are disabled.
 *
 * #eMo1000TestMode2:
 * When test mode is active, each dac channel group would captured user's specified data patterns
 * generated by the fpga core.
 * This is usefull to verify the reliability of the complete digital data generation chain.
 * The module Mo1000_DoDacCalibration_recv() function must have been done successfully last time
 * before using this function.
 * While test mode is active, user can poll for pattern test errors using Mo1000_GetStatus_recv()
 * function pCompare output parameter as often/long as needed. Errors are cumulative between
 * Mo1000_GetStatus_recv() calls.  
 *
 * #eMo1000TestMode1:
 * This mode is similar as #eMo1000TestMode2 explained above, but is mainly reserved for
 * production purpose.
 * While test mode is active, user can poll for pattern test errors using Mo1000_GetStatus_recv()
 * function pCompare output parameter as often/long as needed. Errors are cumulative between
 * Mo1000_GetStatus_recv() calls.  
 * @attention
 * The dac data captured in this mode is not necessary the specified fpga patterns
 * because it could be done uncalibrated (so this mode could not be used to verify the
 * reliability of the complete generated data chain).
 *
 * For normal board DAC operation, test mode must be disable after use.
 *
 * @warning
 * - Doing Mo1000_DoDacCalibration_recv() will disable the test mode automatically and revert to normal mode.
 * - The module Mo1000_Init_recv() function must be call at least once before using this function.
 * - To test all bits correctly, patterns must be provided that a bit transition occurs between
 *   the even and odd patterns on each bits that need to be tested.
 * - All dac channels group are tested at the same time.   
 * - Dacs Clocks must be initialized correctly with Mo1000_SetClockConfig_recv() before testing patterns.    
 * - Communication packet reveived must include:
 *   [in] unsigned uNb:                 module instance to be used 
 *   [in] unsigned Test:                Test mode desired
 *   [in] unsigned short usEvenPattern: 16 bits even test pattern used
 *   [in] unsigned short usOddPattern:  16 bits odd test pattern used
 *
 * @param [in] state
 *    Communication state information
 *
 * @return   Error code: eMo1000Ok if no problem is found, !eMo1000Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Reset_recv(), Mo1000_Init_recv(). 
 *
 * @see
 *    Mo1000_DoDacCalibration_recv(), Mo1000_SetClockConfig_recv().
 *
 ***************************************************************************/ 
Result Mo1000_SetTestMode_recv(connection_state * state);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif
