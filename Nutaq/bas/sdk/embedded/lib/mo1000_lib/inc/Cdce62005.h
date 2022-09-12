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
 * @file    Cdce62005.h 
 * @brief   Ccdce62005 PLL functions definitions
 *
 * $Date: 2015/10/16 14:31:21 $
 * $Revision: 1.4 $
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
 
#ifndef CDCE62005_H__
#define CDCE62005_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "bas_error.h"
#include "Mo1000DevCom.h"
#include "Cdce62005_regs.h"
#include "Cdce62005_defines.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : Cdce62005_Open
 ************************************************************************//**
 *
 * Open the cdce62005 pll module
 *
 * @warning
 * - The Mo1000DevCom module must be opened before using this module.
 * - This function must be the first one used before using this module.
 * - When the module is not used anymore, it can be closed with Cdce62005_Close().  
 *
 * @param [out] pCore
 *    This structure will hold necessary information for the module to be used
 *
 * @param [in] pCom
 *    This structure must hold communication module information
 *
 *
 * @param [in] hCarrier
 *    Handle to the carrier object previously open.
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @see
 *    Cdce62005_Close().
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_Open(Cdce62005_stCore * pCore, Mo1000DevCom_stComCore * pCom, Carrier_handle_t hCarrier);


 /****************************************************************************
 *   NAME : Cdce62005_Reset
 ************************************************************************//**
 *
 * Reset the cdce62005 module to default non operating condition
 *
 * @warning
 * - The module must be opened with Cdce62005_Open() before using this function.
 * - This function must be called at least once to reset this module to default 
 *   non operating condition.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Cdce62005_Open()
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Cdce62005_Open().
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_Reset(Cdce62005_stCore * pCore);


 /****************************************************************************
 *   NAME : Cdce62005_Init
 ************************************************************************//**
 *
 * Initilaize the cdce62005 pll module to default operating condition
 * This function must be called at least once after reset of this module.
 *  
 * @warning
 * - The module must be opened with Cdce62005_Open() before using this function.
 * - The module must be reset with Cdce62005_Reset() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Cdce62005_Open()
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Cdce62005_Open(), Cdce62005_Reset().
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_Init(Cdce62005_stCore * pCore);


 /****************************************************************************
 *   NAME : Cdce62005_WriteReg
 ************************************************************************//**
 *
 * Write the specified cdce62005 pll registers with specified value
 *  
 * @warning
 * - The module must be initialize with Cdce62005_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Cdce62005_Open()
 *
 * @param [in] Addr
 *    Register address to access
 *
 * @param [in] uVal
 *    Register value to be written
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init().
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_WriteReg(Cdce62005_stCore * pCore, Cdce62005_eAddr Addr, unsigned uVal);


 /****************************************************************************
 *   NAME : Cdce62005_ReadReg
 ************************************************************************//**
 *
 * Read the specified cdce62005 pll registers
 *  
 * @warning
 * - The module must be initialize with Cdce62005_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Cdce62005_Open()
 *
 * @param [in] Addr
 *    Register address to access
 *
 * @param [out] puVal
 *    Register read value
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init().
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_ReadReg(Cdce62005_stCore * pCore, Cdce62005_eAddr Addr, unsigned *puVal);


 /****************************************************************************
 *   NAME : Cdce62005_Close
 ************************************************************************//**
 *
 * Close the Cdce62005 pll module
 * This function must be the last one used after using this module.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Cdce62005_Open()
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Cdce62005_Open()
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_Close(Cdce62005_stCore * pCore);


 /****************************************************************************
 *   NAME : Cdce62005_Calculate
 ************************************************************************//**
 *
 * Calculate the pll parameters according to specified requierements.
 *
 * @warning
 * - When the loop filter type #LoopFilterType is configured for external #eCdce62005LoopFilterExt
 *   or when it is configured for internal #eCdce62005LoopFilterInt and an internal loop filter
 *   cannot be calculated for the clocks requierements, an error of #eCdce62005NoFilterSolution
 *   will be returned which means there is a synthetized solution for the specified requierements,
 *   but a loop filter must be manually calculated to complete the solution. Calculated solution results
 *   could be used as inputs for a manually calculated loop filter. When the loop filter is calculated
 *   manually (member sLoopFilterIndex = -1 when manual calculation is needed), the members R2Value,
 *   R3Value, C1Value, C2Value, C3Value and ChargePump must be manually provided after calculation
 *   to complete the solution. 
 * - When an output reference is configured to anything else than #eCdce62005OutRefModeSynth (pll mode)
 *   it will force to configure an output in fanout mode (without using the pll, but only outputs dividers).
 *   This forced requierement can easily keep to find a working solution. When pll mode is used instead,
 *   the calculation logic will automatically attempt to revert to fanout mode (not using pll)
 *   if possible (this could give a better chance of solution if fanout mode is not possible).
 * - An output reference set to #eCdce62005OutRefModeSmartmux should only be used if the source of the
 *   smartmux configured by SmartMuxRefSelect member is #eCdce62005InputRefModeAux (and not for
 *   prim/sec smartmux reference).
 * - Auxiliary output parameter would only allow to enable it or not and to choose from which
 *   output #2 or #3 it is referenced. But there is no calculation supported for it's frequency generation.
 * - Each Output usPhaseAdj member allow to configure a needed output phase as a requierement. While the calculation
 *   will attempt to match the nearest possible phase to meet the requierement, if not possible, the phase
 *   could be left to 0 deg. A solution will be synthetized no matter if all the phase requierements are met
 *   or not (see aucOutPhaseAdjNdx[] member to check the nearest phase found after calculation when a solution
 *   exists for all the outputs).
 * - The last output is a virtual output. It can be used to setup the vco to a specific closed loop
 *   configuration to meet this virtual output requierements, even if any real output is using the vco directly
 *   (for exemple all real outputs are in fanout mode). If not used, sets its frequency to 0.   
 *
 * @param [inout] stParam
 *    pll parameters structure
 *    in:  RefInput, SmartMuxRefSelect, OutParams, SyncMode, AuxParams, PllLockParams,
 *         LoopFilterType, uPfdFreqMin, uPfdFreqMax, members are used for calculation.
 *    out: uRefFreq, uPfdFreq, uVcoFreq, uSynthFreq, uAuxFreq, usFeedbackDivider,
 *         ucBypassDivider, usCFeedbackDivider, usCFeedbackDividerIndex,
 *         usReferenceDivider, ucPrimaryInputDivider, ucSecondaryInputDivider,
 *         usInputDivider, aucOutDivider, aucOutPhaseAdjNdx, ucPrescaleDivider,
 *         ucVcoBypassAll, aucVcoBypass, ucUsableRefDivider, sLoopFilterIndex,
 *         R2Value, R3Value, C1Value, C2Value, C3Value, ChargePump, 
 *         OutParams[].OutputRef (will be changed only for an output configured on the
 *         pll eCdce62005OutRefModeSynth, where we have found a fan-out solution without
 *         using the pll), members will return the computed pll setup.
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @see
 *    Cdce62005_SetParameters().
 *     
 * Example:
 * @code
 * // ...
 *  {
 *  Cdce62005_stCore pCore;
 *	Result Ret;
 *  Cdce62005_stParam stParam;
 *	int iI, iJ;
 *
 *  // ... (missing Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init() calls)
 *   *    
 *	iI = 50000000;     (data frequency)
 *  iJ = 1;            (interpolation rate)
 *  // Uses all possible comparison frequency pfd range (could be limited for a partial higher range if no external loop filter is used)
 *	stParam.uPfdFreqMin = CDCE62005_PFDMINEXT;
 *	stParam.uPfdFreqMax = CDCE62005_PFDMAX;
 *
 *	stParam.SmartMuxRefSelect = eCdce62005InputRefModePrimary;        // Use the primary input as the pll reference clock
 *	stParam.LoopFilterType = eCdce62005LoopFilterInt;                 // Attempt to use internally calculated loop filter
 *  stParam.RefInput.InputsType=eCdce62005InputLvPecl;                // prim/sec inputs are lvpecl type
 *  stParam.RefInput.InputsTermination=eCdce62005InputDcTermination;  // Input uses internal dc termination
 *  stParam.RefInput.LvdsFailSafe=eCdce62005LvdsInputFailDisable;     // Configure lvds failsafe mode
 *  stParam.RefInput.uAuxInputFrequency=0;                            // We do not use auxiliary input
 *  stParam.RefInput.PrimaryInput.ucForceDiv2=0;                      // Do not force primary input prediv by 2: can be freely adjusted by the calculations
 *  stParam.RefInput.PrimaryInput.uInputFrequency=125000000;          // Primary input frequency
 *  stParam.RefInput.PrimaryInput.InputVbb=eCdce62005InputVbbNormal;  // Primary input not inverted
 *  stParam.RefInput.PrimaryInput.InputTerm=eCdce62005InputTermEnabled; // Enable internal input termination
 *  stParam.RefInput.SecondaryInput.ucForceDiv2=0;                    // Do not force secondary input prediv by 2: can be freely adjusted by the calculations
 *  stParam.RefInput.SecondaryInput.InputVbb=eCdce62005InputVbbNormal;// Secondary input not inverted
 *  stParam.RefInput.SecondaryInput.InputTerm=eCdce62005InputTermEnabled;// Enable internal input termination
 *	stParam.SyncMode = Cdce62005_eSyncNextRefAndOutput;               // Sync by waiting the detection of input rise and gen clocks rise
 *	stParam.AuxParams.AuxState = eCdce62005AuxDisable;                // Do not use auxiliary output
 *	stParam.AuxParams.AuxSelect = eCdce62005AuxSelect2;               // Auxiliary output based on output divider 2 when used
 *	stParam.PllLockParams.PllLockMode = eCdce62005DigitalPllLock;     // Pll lock pin is configured for digital mode
 *	stParam.PllLockParams.PllLockWidth = eCdce62005PllLockW1D5Ns;     // Use 1.5 nSec pll lock window
 *	stParam.PllLockParams.PllLockCount = eCdce62005PllLockCount1Pfd;  // Use immediate (fast) reaction of the pll lock
 *	stParam.OutParams[2].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output
 *	stParam.OutParams[2].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl
 *	stParam.OutParams[2].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[2].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin
 *	stParam.OutParams[2].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin
 *	stParam.OutParams[3].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
 *	stParam.OutParams[3].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                                               
 *	stParam.OutParams[3].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[3].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[3].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[0].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
 *	stParam.OutParams[0].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                                               
 *	stParam.OutParams[0].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[0].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[0].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[1].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[1].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                         
 *	stParam.OutParams[1].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[1].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[1].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[4].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[4].OutputMode = eCdce62005OutModeLvds;          // Output pin type is lvds                                         
 *	stParam.OutParams[4].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[4].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[4].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[5].usPhaseAdj = 0;                              // Virtual output phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[5].OutputMode = eCdce62005OutModeLvPecl;        // Virtual Output pin type is lvpecl                                         
 *	stParam.OutParams[5].OutputRef = eCdce62005OutRefModeSynth;       // Virtual Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[5].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Virtual positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[5].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Virtual negative output in lvcmos mode would be a positive active pin     
 *  // Output Frequencies calculation (simulation)
 *  stParam.OutParams[1].uOutputFreq = iI * iJ;
 *  stParam.OutParams[0].uOutputFreq = iI / 8;
 *  stParam.OutParams[4].uOutputFreq = iI * 2;
 *	if (stParam.OutParams[4].uOutputFreq > stParam.OutParams[1].uOutputFreq)
 *      stParam.OutParams[2].uOutputFreq = stParam.OutParams[4].uOutputFreq;
 *	else
 *      stParam.OutParams[2].uOutputFreq = stParam.OutParams[1].uOutputFreq;
 *	stParam.OutParams[3].uOutputFreq = stParam.OutParams[2].uOutputFreq;
 *	stParam.OutParams[5].uOutputFreq = stParam.OutParams[2].uOutputFreq; // Virtual output frequency
 *  stParam.RefInput.SecondaryInput.uInputFrequency=stParam.OutParams[2].uOutputFreq; // loop backed to a clock input (reference)
 *
 *	Ret = Cdce62005_Calculate(&stParam);
 *	if (FAILURE(Ret))
 *      {}
 * // ...
 * @endcode
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_Calculate(Cdce62005_stParam * stParam);


 /****************************************************************************
 *   NAME : Cdce62005_SetParameters
 ************************************************************************//**
 *
 * Set the pll parameters calculated with Cdce62005_Calculate() to the device.
 *  
 * @warning
 * - The module must be initialize with Cdce62005_Init() before using this function.
 * - The virtual output (last one) is not programmed in the device. 
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Cdce62005_Open()
 *
 * @param [in] stParam
 *    Pll parameters structure
 *    
 * @param [in] uSync
 *    when != 0, deal with the outputs synch through software without using external sync output
 *    The function Cdce62005_DoSync() must be use at the end to complete the sync process.
 *    when == 0, assumes the sync is done externally with the sync pin (outside of this driver).  
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init(), Cdce62005_Calculate().
 *
 * @see
 *    Cdce62005_Calculate().
 *     
 * Example:
 * @code
 * // ...
 *  {
 *  Cdce62005_stCore pCore;
 *	Result Ret;
 *  Cdce62005_stParam stParam;
 *	int iI, iJ;
 *
 *  // ... (missing Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init() calls)
 *   *    
 *	iI = 50000000;     (data frequency)
 *  iJ = 1;            (interpolation rate)
 *  // Uses all possible comparison frequency pfd range (could be limited for a partial higher range if no external loop filter is used)
 *	stParam.uPfdFreqMin = CDCE62005_PFDMINEXT;
 *	stParam.uPfdFreqMax = CDCE62005_PFDMAX;
 *
 *	stParam.SmartMuxRefSelect = eCdce62005InputRefModePrimary;        // Use the primary input as the pll reference clock
 *	stParam.LoopFilterType = eCdce62005LoopFilterInt;                 // Attempt to use internally calculated loop filter
 *  stParam.RefInput.InputsType=eCdce62005InputLvPecl;                // prim/sec inputs are lvpecl type
 *  stParam.RefInput.InputsTermination=eCdce62005InputDcTermination;  // Input uses internal dc termination
 *  stParam.RefInput.LvdsFailSafe=eCdce62005LvdsInputFailDisable;     // Configure lvds failsafe mode
 *  stParam.RefInput.uAuxInputFrequency=0;                            // We do not use auxiliary input
 *  stParam.RefInput.PrimaryInput.ucForceDiv2=0;                      // Do not force primary input prediv by 2: can be freely adjusted by the calculations
 *  stParam.RefInput.PrimaryInput.uInputFrequency=125000000;          // Primary input frequency
 *  stParam.RefInput.PrimaryInput.InputVbb=eCdce62005InputVbbNormal;  // Primary input not inverted
 *  stParam.RefInput.PrimaryInput.InputTerm=eCdce62005InputTermEnabled; // Enable internal input termination
 *  stParam.RefInput.SecondaryInput.ucForceDiv2=0;                    // Do not force secondary input prediv by 2: can be freely adjusted by the calculations
 *  stParam.RefInput.SecondaryInput.InputVbb=eCdce62005InputVbbNormal;// Secondary input not inverted
 *  stParam.RefInput.SecondaryInput.InputTerm=eCdce62005InputTermEnabled;// Enable internal input termination
 *	stParam.SyncMode = Cdce62005_eSyncNextRefAndOutput;               // Sync by waiting the detection of input rise and gen clocks rise
 *	stParam.AuxParams.AuxState = eCdce62005AuxDisable;                // Do not use auxiliary output
 *	stParam.AuxParams.AuxSelect = eCdce62005AuxSelect2;               // Auxiliary output based on output divider 2 when used
 *	stParam.PllLockParams.PllLockMode = eCdce62005DigitalPllLock;     // Pll lock pin is configured for digital mode
 *	stParam.PllLockParams.PllLockWidth = eCdce62005PllLockW1D5Ns;     // Use 1.5 nSec pll lock window
 *	stParam.PllLockParams.PllLockCount = eCdce62005PllLockCount1Pfd;  // Use immediate (fast) reaction of the pll lock
 *	stParam.OutParams[2].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output
 *	stParam.OutParams[2].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl
 *	stParam.OutParams[2].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[2].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin
 *	stParam.OutParams[2].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin
 *	stParam.OutParams[3].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
 *	stParam.OutParams[3].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                                               
 *	stParam.OutParams[3].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[3].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[3].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[0].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
 *	stParam.OutParams[0].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                                               
 *	stParam.OutParams[0].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[0].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[0].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[1].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[1].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                         
 *	stParam.OutParams[1].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[1].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[1].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[4].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[4].OutputMode = eCdce62005OutModeLvds;          // Output pin type is lvds                                         
 *	stParam.OutParams[4].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[4].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[4].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[5].usPhaseAdj = 0;                              // Virtual output phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[5].OutputMode = eCdce62005OutModeLvPecl;        // Virtual Output pin type is lvpecl                                         
 *	stParam.OutParams[5].OutputRef = eCdce62005OutRefModeSynth;       // Virtual Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[5].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Virtual positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[5].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Virtual negative output in lvcmos mode would be a positive active pin     
 *  // Output Frequencies calculation (simulation)
 *  stParam.OutParams[1].uOutputFreq = iI * iJ;
 *  stParam.OutParams[0].uOutputFreq = iI / 8;
 *  stParam.OutParams[4].uOutputFreq = iI * 2;
 *	if (stParam.OutParams[4].uOutputFreq > stParam.OutParams[1].uOutputFreq)
 *      stParam.OutParams[2].uOutputFreq = stParam.OutParams[4].uOutputFreq;
 *	else
 *      stParam.OutParams[2].uOutputFreq = stParam.OutParams[1].uOutputFreq;
 *	stParam.OutParams[3].uOutputFreq = stParam.OutParams[2].uOutputFreq;
 *	stParam.OutParams[5].uOutputFreq = stParam.OutParams[2].uOutputFreq; // Virtual output frequency
 *  stParam.RefInput.SecondaryInput.uInputFrequency=stParam.OutParams[2].uOutputFreq; // loop backed to a clock input (reference)
 *
 *	Ret = Cdce62005_Calculate(&stParam);
 *	if (FAILURE(Ret))
 *  {
 *      // Did we get a synthetized solution, but only needing to manually provides the loop filter params? 
 *      if (Ret == eCdce62005NoFilterSolution)
 *      {
 *          // Ok we have a synthetized solution but needing to manually provides the loop filter params
 *          stParam.C1Value = aaa;  
 *          stParam.R2Value = bbb;  
 *          stParam.C2Value = ccc;  
 *          stParam.R3Value = ddd;  
 *          stParam.C3Value = eee;  
 *          stParam.ChargePump = xxx;  
 *      }
 *      else
 *          return(Ret);    
 *  }
 *  Ret = Cdce62005_SetParameters(&pCore, &stParam, 1);
 *	if (FAILURE(Ret))
 *	    {} 
 *  } 
 *    
 * // ...
 * @endcode
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_SetParameters(Cdce62005_stCore * pCore, Cdce62005_stParam * stParam, unsigned uSync);


/****************************************************************************
 *   NAME : Cdce62005_DoSync
 ************************************************************************//**
 *
 * Executes the output synchronization procedure. This function is only
 * needed when the external device sync pin is not used to control the
 * outputs synchronization process.  
 *  
 * @warning
 * - The module must be initialize with Cdce62005_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Cdce62005_Open()
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init(), Cdce62005_Calculate(),
 *    Cdce62005_SetParameters().
 *
 * @see
 *    Cdce62005_Calculate(), Cdce62005_SetParameters().
 *     
 * Example:
 * @code
 * // ...
 *  {
 *  Cdce62005_stCore pCore;
 *	Result Ret;
 *  Cdce62005_stParam stParam;
 *	int iI, iJ;
 *
 *  // ... (missing Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init() calls)
 *   *    
 *	iI = 50000000;     (data frequency)
 *  iJ = 1;            (interpolation rate)
 *  // Uses all possible comparison frequency pfd range (could be limited for a partial higher range if no external loop filter is used)
 *	stParam.uPfdFreqMin = CDCE62005_PFDMINEXT;
 *	stParam.uPfdFreqMax = CDCE62005_PFDMAX;
 *
 *	stParam.SmartMuxRefSelect = eCdce62005InputRefModePrimary;        // Use the primary input as the pll reference clock
 *	stParam.LoopFilterType = eCdce62005LoopFilterInt;                 // Attempt to use internally calculated loop filter
 *  stParam.RefInput.InputsType=eCdce62005InputLvPecl;                // prim/sec inputs are lvpecl type
 *  stParam.RefInput.InputsTermination=eCdce62005InputDcTermination;  // Input uses internal dc termination
 *  stParam.RefInput.LvdsFailSafe=eCdce62005LvdsInputFailDisable;     // Configure lvds failsafe mode
 *  stParam.RefInput.uAuxInputFrequency=0;                            // We do not use auxiliary input
 *  stParam.RefInput.PrimaryInput.ucForceDiv2=0;                      // Do not force primary input prediv by 2: can be freely adjusted by the calculations
 *  stParam.RefInput.PrimaryInput.uInputFrequency=125000000;          // Primary input frequency
 *  stParam.RefInput.PrimaryInput.InputVbb=eCdce62005InputVbbNormal;  // Primary input not inverted
 *  stParam.RefInput.PrimaryInput.InputTerm=eCdce62005InputTermEnabled; // Enable internal input termination
 *  stParam.RefInput.SecondaryInput.ucForceDiv2=0;                    // Do not force secondary input prediv by 2: can be freely adjusted by the calculations
 *  stParam.RefInput.SecondaryInput.InputVbb=eCdce62005InputVbbNormal;// Secondary input not inverted
 *  stParam.RefInput.SecondaryInput.InputTerm=eCdce62005InputTermEnabled;// Enable internal input termination
 *	stParam.SyncMode = Cdce62005_eSyncNextRefAndOutput;               // Sync by waiting the detection of input rise and gen clocks rise
 *	stParam.AuxParams.AuxState = eCdce62005AuxDisable;                // Do not use auxiliary output
 *	stParam.AuxParams.AuxSelect = eCdce62005AuxSelect2;               // Auxiliary output based on output divider 2 when used
 *	stParam.PllLockParams.PllLockMode = eCdce62005DigitalPllLock;     // Pll lock pin is configured for digital mode
 *	stParam.PllLockParams.PllLockWidth = eCdce62005PllLockW1D5Ns;     // Use 1.5 nSec pll lock window
 *	stParam.PllLockParams.PllLockCount = eCdce62005PllLockCount1Pfd;  // Use immediate (fast) reaction of the pll lock
 *	stParam.OutParams[2].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output
 *	stParam.OutParams[2].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl
 *	stParam.OutParams[2].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[2].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin
 *	stParam.OutParams[2].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin
 *	stParam.OutParams[3].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
 *	stParam.OutParams[3].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                                               
 *	stParam.OutParams[3].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[3].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[3].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[0].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
 *	stParam.OutParams[0].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                                               
 *	stParam.OutParams[0].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[0].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[0].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[1].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[1].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                         
 *	stParam.OutParams[1].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[1].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[1].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[4].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[4].OutputMode = eCdce62005OutModeLvds;          // Output pin type is lvds                                         
 *	stParam.OutParams[4].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[4].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[4].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
 *  // Output Frequencies calculation (simulation)
 *  stParam.OutParams[1].uOutputFreq = iI * iJ;
 *  stParam.OutParams[0].uOutputFreq = iI / 8;
 *  stParam.OutParams[4].uOutputFreq = iI * 2;
 *	if (stParam.OutParams[4].uOutputFreq > stParam.OutParams[1].uOutputFreq)
 *      stParam.OutParams[2].uOutputFreq = stParam.OutParams[4].uOutputFreq;
 *	else
 *      stParam.OutParams[2].uOutputFreq = stParam.OutParams[1].uOutputFreq;
 *	stParam.OutParams[3].uOutputFreq = stParam.OutParams[2].uOutputFreq;
 *  stParam.RefInput.SecondaryInput.uInputFrequency=stParam.OutParams[2].uOutputFreq; // loop backed to a clock input (reference)
 *
 *	Ret = Cdce62005_Calculate(&stParam);
 *	if (FAILURE(Ret))
 *  {
 *      // Did we get a synthetized solution, but only needing to manually provides the loop filter params? 
 *      if (Ret == eCdce62005NoFilterSolution)
 *      {
 *          // Ok we have a synthetized solution but needing to manually provides the loop filter params
 *          stParam.C1Value = aaa;  
 *          stParam.R2Value = bbb;  
 *          stParam.C2Value = ccc;  
 *          stParam.R3Value = ddd;  
 *          stParam.C3Value = eee;  
 *          stParam.ChargePump = xxx;  
 *      }
 *      else
 *          return(Ret);    
 *  }
 *  Ret = Cdce62005_SetParameters(&pCore, &stParam, 1);
 *	if (FAILURE(Ret))
 *	    {}
 *	      
 *  Ret = Cdce62005_DoSync(&pCore);
 *	if (FAILURE(Ret))
 *	    {} 
 *  } 
 *    
 * // ...
 * @endcode
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_DoSync(Cdce62005_stCore * pCore);


/****************************************************************************
 *   NAME : Cdce62005_WaitPllLock
 ************************************************************************//**
 *
 * Once the pll is configured, this function allows to wait for the pll
 * to lock. This operation is not necessary as the pll is checked
 * automatically after each configuration with Cdce62005_SetParameters().
 * But using this function could allow to recheck the pll is correctly locked
 * each time that this is necessary.  
 *  
 * @warning
 * - The module must be initialize with Cdce62005_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Cdce62005_Open()
 *
 * @param [out] puLock
 *    when !=0, this means the pll has correctly locked
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init(), Cdce62005_Calculate(),
 *    Cdce62005_SetParameters().
 *
 * @see
 *    Cdce62005_Calculate(), Cdce62005_SetParameters().
 *     
 * Example:
 * @code
 * // ...
 *  {
 *  Cdce62005_stCore pCore;
 *	Result Ret;
 *  Cdce62005_stParam stParam;
 *	int iI, iJ;
 *
 *  // ... (missing Cdce62005_Open(), Cdce62005_Reset(), Cdce62005_Init() calls)
 *   *    
 *	iI = 50000000;     (data frequency)
 *  iJ = 1;            (interpolation rate)
 *  // Uses all possible comparison frequency pfd range (could be limited for a partial higher range if no external loop filter is used)
 *	stParam.uPfdFreqMin = CDCE62005_PFDMINEXT;
 *	stParam.uPfdFreqMax = CDCE62005_PFDMAX;
 *
 *	stParam.SmartMuxRefSelect = eCdce62005InputRefModePrimary;        // Use the primary input as the pll reference clock
 *	stParam.LoopFilterType = eCdce62005LoopFilterInt;                 // Attempt to use internally calculated loop filter
 *  stParam.RefInput.InputsType=eCdce62005InputLvPecl;                // prim/sec inputs are lvpecl type
 *  stParam.RefInput.InputsTermination=eCdce62005InputDcTermination;  // Input uses internal dc termination
 *  stParam.RefInput.LvdsFailSafe=eCdce62005LvdsInputFailDisable;     // Configure lvds failsafe mode
 *  stParam.RefInput.uAuxInputFrequency=0;                            // We do not use auxiliary input
 *  stParam.RefInput.PrimaryInput.ucForceDiv2=0;                      // Do not force primary input prediv by 2: can be freely adjusted by the calculations
 *  stParam.RefInput.PrimaryInput.uInputFrequency=125000000;          // Primary input frequency
 *  stParam.RefInput.PrimaryInput.InputVbb=eCdce62005InputVbbNormal;  // Primary input not inverted
 *  stParam.RefInput.PrimaryInput.InputTerm=eCdce62005InputTermEnabled; // Enable internal input termination
 *  stParam.RefInput.SecondaryInput.ucForceDiv2=0;                    // Do not force secondary input prediv by 2: can be freely adjusted by the calculations
 *  stParam.RefInput.SecondaryInput.InputVbb=eCdce62005InputVbbNormal;// Secondary input not inverted
 *  stParam.RefInput.SecondaryInput.InputTerm=eCdce62005InputTermEnabled;// Enable internal input termination
 *	stParam.SyncMode = Cdce62005_eSyncNextRefAndOutput;               // Sync by waiting the detection of input rise and gen clocks rise
 *	stParam.AuxParams.AuxState = eCdce62005AuxDisable;                // Do not use auxiliary output
 *	stParam.AuxParams.AuxSelect = eCdce62005AuxSelect2;               // Auxiliary output based on output divider 2 when used
 *	stParam.PllLockParams.PllLockMode = eCdce62005DigitalPllLock;     // Pll lock pin is configured for digital mode
 *	stParam.PllLockParams.PllLockWidth = eCdce62005PllLockW1D5Ns;     // Use 1.5 nSec pll lock window
 *	stParam.PllLockParams.PllLockCount = eCdce62005PllLockCount1Pfd;  // Use immediate (fast) reaction of the pll lock
 *	stParam.OutParams[2].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output
 *	stParam.OutParams[2].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl
 *	stParam.OutParams[2].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[2].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin
 *	stParam.OutParams[2].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin
 *	stParam.OutParams[3].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
 *	stParam.OutParams[3].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                                               
 *	stParam.OutParams[3].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
 *	stParam.OutParams[3].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[3].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[0].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
 *	stParam.OutParams[0].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                                               
 *	stParam.OutParams[0].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[0].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[0].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
 *	stParam.OutParams[1].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[1].OutputMode = eCdce62005OutModeLvPecl;        // Output pin type is lvpecl                                         
 *	stParam.OutParams[1].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[1].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[1].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[4].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
 *	stParam.OutParams[4].OutputMode = eCdce62005OutModeLvds;          // Output pin type is lvds                                         
 *	stParam.OutParams[4].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
 *	stParam.OutParams[4].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
 *	stParam.OutParams[4].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
 *  // Output Frequencies calculation (simulation)
 *  stParam.OutParams[1].uOutputFreq = iI * iJ;
 *  stParam.OutParams[0].uOutputFreq = iI / 8;
 *  stParam.OutParams[4].uOutputFreq = iI * 2;
 *	if (stParam.OutParams[4].uOutputFreq > stParam.OutParams[1].uOutputFreq)
 *      stParam.OutParams[2].uOutputFreq = stParam.OutParams[4].uOutputFreq;
 *	else
 *      stParam.OutParams[2].uOutputFreq = stParam.OutParams[1].uOutputFreq;
 *	stParam.OutParams[3].uOutputFreq = stParam.OutParams[2].uOutputFreq;
 *  stParam.RefInput.SecondaryInput.uInputFrequency=stParam.OutParams[2].uOutputFreq; // loop backed to a clock input (reference)
 *
 *	Ret = Cdce62005_Calculate(&stParam);
 *	if (FAILURE(Ret))
 *  {
 *      // Did we get a synthetized solution, but only needing to manually provides the loop filter params? 
 *      if (Ret == eCdce62005NoFilterSolution)
 *      {
 *          // Ok we have a synthetized solution but needing to manually provides the loop filter params
 *          stParam.C1Value = aaa;  
 *          stParam.R2Value = bbb;  
 *          stParam.C2Value = ccc;  
 *          stParam.R3Value = ddd;  
 *          stParam.C3Value = eee;  
 *          stParam.ChargePump = xxx;  
 *      }
 *      else
 *          return(Ret);    
 *  }
 *  Ret = Cdce62005_SetParameters(&pCore, &stParam, 1);
 *	if (FAILURE(Ret))
 *	    {}
 *	      
 *  {
 *      unsigned uLock;
 *      Ret = Cdce62005_WaitPllLock(&pCore, &uLock);
 *	    if (FAILURE(Ret))
 *	        {} 
 *  } 
 *  } 
 *    
 * // ...
 * @endcode
 *
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
Result Cdce62005_WaitPllLock(Cdce62005_stCore * pCore, unsigned *puLock);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif


