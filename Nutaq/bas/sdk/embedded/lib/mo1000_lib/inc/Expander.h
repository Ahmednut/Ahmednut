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
 * @file       Expander.h
 * @brief      Mo1000 IO expander functions definitions
 *
 * $Date: 2015/10/16 14:31:21 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
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

#ifndef EXPANDER_H__
#define EXPANDER_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "Mo1000DevCom.h"
#include "Expander_regs.h"
#include "Expander_defines.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : Expander_Open
 ************************************************************************//**
 *
 * Open the mo1000 expander module
 * This function must be the first one used before using this module.
 * When the module is not used anymore, it can be closed with Expander_close().
 *
 * @warning
 * - The Mo1000DevCom module must be opened before using this module.
 *
 * @param [out] pCore
 *    This structure will hold necessary information for the module to be used
 *
 * @param [in] pCom
 *    This structure must hold communication module information
 *
 * @param [in] uNewRev
 *    This flag is !=0 when a newer board revision (not original alpha board)
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @see
 *    Expander_Close().
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_Open(Expander_stCore * pCore, Mo1000DevCom_stComCore * pCom, unsigned uNewRev);


 /****************************************************************************
 *   NAME : Expander_Reset
 ************************************************************************//**
 *
 * Reset the mo1000 expander module to default non operating condition
 * This function must be called at least once to reset this module to default
 * non operating condition.
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Expander_Open().
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_Reset(Expander_stCore * pCore);


 /****************************************************************************
 *   NAME : Expander_Init
 ************************************************************************//**
 *
 * Init the mo1000 expander module to default operating condition
 * This function must be called at least once after reset of this module.
 *  
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Expander_Open(), Expander_Reset().
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_Init(Expander_stCore * pCore);


 /****************************************************************************
 *   NAME : Expander_Close
 ************************************************************************//**
 *
 * Close the mo1000 expander module
 * This function must be the last one used after using this module.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open()
 *
 *   @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_Close(Expander_stCore * pCore);


 /****************************************************************************
 *   NAME : Expander_SetLocalOscSw
 ************************************************************************//**
 *
 * Configure the local switch oscillator according to specified setting
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Switch
 *    This is the oscillator switch setting
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_SetLocalOscSw(Expander_stCore * pCore, Expander_eLocalOscSw Switch);


 /****************************************************************************
 *   NAME : Expander_SetBridgeReset
 ************************************************************************//**
 *
 * Configure the bridge reset according to specified setting
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] BridgeReset
 *    This is the bridge reset setting
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_SetBridgeReset(Expander_stCore * pCore, Expander_eBridgeReset BridgeReset);


 /****************************************************************************
 *   NAME : Expander_SetDacReset
 ************************************************************************//**
 *
 * Configure a dac reset signal according to specified setting
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Dac
 *    This is the dac selected
 *
 * @param [in] DacReset
 *    This is the dac reset setting
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_SetDacReset(Expander_stCore * pCore, Expander_eDacSelect Dac, Expander_eDacReset DacReset);


 /****************************************************************************
 *   NAME : Expander_SetPllShutdown
 ************************************************************************//**
 *
 * Configure the pll shutdown according to specified setting
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Shutdown
 *    This is the pll shutdown setting
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_SetPllShutdown(Expander_stCore * pCore, Expander_ePllPowerDown Shutdown);


 /****************************************************************************
 *   NAME : Expander_SetPllSync
 ************************************************************************//**
 *
 * Configure the pll sync according to specified setting
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Sync
 *    This is the pll sync setting
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_SetPllSync(Expander_stCore * pCore, Expander_ePllSync Sync);


 /****************************************************************************
 *   NAME : Expander_GetPllLock
 ************************************************************************//**
 *
 * Get the pll lock status
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [out] pLock
 *    This is the pll lock status
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_GetPllLock(Expander_stCore * pCore, Expander_ePllLock *pLock);


 /****************************************************************************
 *   NAME : Expander_GetDacIrq
 ************************************************************************//**
 *
 * Get the dacs irq (error) status
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [out] pDacIrq
 *    This is the dacs irq error status
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_GetDacIrq(Expander_stCore * pCore, Expander_eDacIrq *pDacIrq);


 /****************************************************************************
 *   NAME : Expander_SetClkSource
 ************************************************************************//**
 *
 * Configure the clock source according to specified setting
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Source
 *    This is the clock source setting
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_SetClkSource(Expander_stCore * pCore, Expander_eClkSrc Source);


 /****************************************************************************
 *   NAME : Expander_SetClkReference
 ************************************************************************//**
 *
 * Configure the clock reference according to specified setting (master clock
 * or master ref).
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Reference
 *    This is the clock reference setting
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_SetClkReference(Expander_stCore * pCore, Expander_eClkRef Reference);


 /****************************************************************************
 *   NAME : Expander_WritePortsRaw
 ************************************************************************//**
 *
 *   Writes specified output data port in raw form.
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 * - Using this function for diagnostic purpose will destroy all other port settings
 *   programmed with other functions of this module. This function is normally used
 *   for test only purpose.   
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Addr
 *    Which port to access (LSB or MSB)
 *
 * @param [in] ucPort
 *    Port output raw digital value
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_WritePortsRaw(Expander_stCore * pCore, Expander_eIoPortAddr Addr, unsigned char ucPort);


 /****************************************************************************
 *   NAME : Expander_ReadPortsRaw
 ************************************************************************//**
 *
 *   Read specified output data port latched values in raw form.
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 * - This function does not read the ports pins, but the actual output ports registers
 *   content written with Expander_WritePortsRaw().     
 * - This function is normally used for test only purpose.   
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Addr
 *    Which port to access (LSB or MSB)
 *
 * @param [out] pucPort
 *    Port output raw latched digital value
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_ReadPortsRaw(Expander_stCore * pCore, Expander_eIoPortAddr Addr, unsigned char *pucPort);


 /****************************************************************************
 *   NAME : Expander_ReadPortsPinRaw
 ************************************************************************//**
 *
 *   Reads specified data port input pins in raw form.
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 * - This function is normally used for test only purpose.   
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] Addr
 *    Which port to access (LSB or MSB)
 *
 * @param [out] pucPort
 *    Port input pins raw digital value
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_ReadPortsPinRaw(Expander_stCore * pCore, Expander_eIoPortAddr Addr, unsigned char *pucPort);


 /****************************************************************************
 *   NAME : Expander_SetDataMux
 ************************************************************************//**
 *
 * Configure the dac control signal paths (DCI & FRAME) according to specified
 * settings. Normally both paths must be set to the same setting except for test
 * purpose.  
 *
 * @warning
 * - The module must be opened with Expander_Open() before using this function.
 * - The module must be reset with Expander_Reset() before using this function.
 * - The module must be init with Expander_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Expander_Open()
 *
 * @param [in] DciPath
 *    This is the dci path setting
 *
 * @param [in] FramePath
 *    This is the frame path setting
 *
 * @return   Error code: eExpanderOk if no problem is found, !eExpanderOk if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Expander_Open(), Expander_Reset(), Expander_Init()
 *
 * @ingroup MO1000_EXPANDER
 *
 ***************************************************************************/ 
Result Expander_SetDataMux(Expander_stCore * pCore, Expander_ePathMuxSel DciPath, Expander_ePathMuxSel FramePath);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

