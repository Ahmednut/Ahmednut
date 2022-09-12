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
 * @file       Expander.c
 * @brief      Mo1000 IO expander functions
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "Expander.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

// Default reset outputs values
#define EXPANDER_RESET00 0x01        // Clkmux=internal 125MHz, Local Osc=OFF, Dac14ResetN=ON, Dac58ResetN=ON, SpiBridgeResetN=ON, DCIMuxSel=Long, PllMuxSel=MasterRef
#define EXPANDER_RESET01 0x00        // PllPowerDown=ON, PllSync=ON, FrameMuxSel=long path 

// Default reset pins direction
#define EXPANDER_RESET00_C 0x00      // All pins outputs
#define EXPANDER_RESET01_C 0xF8      // All pins inputs except PLL_POWERDOWNN, PLL_SYNCN, FRAME_MUX_SEL are outputs

// Default init outputs values
#define EXPANDER_INIT00 0x11         // Clkmux=internal 125MHz, Local Osc=OFF, Dac14ResetN=ON, Dac58ResetN=ON, SpiBridgeResetN=OFF, DCIMuxSel=Long, PllMuxSel=MasterRef
#define EXPANDER_INIT01 0x00         // PllPowerDown=ON, PllSync=ON, FrameMuxSel=long path

// Default init pins direction
#define EXPANDER_INIT00_C 0x00       // All pins outputs
#define EXPANDER_INIT01_C 0xF8       // All pins inputs except PLL_POWERDOWNN, PLL_SYNCN, FRAME_MUX_SEL are outputs


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


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
Result Expander_Open(Expander_stCore * pCore, Mo1000DevCom_stComCore * pCom, unsigned uNewRev)
{
    // Save the com module information
    pCore->m_pCom = pCom;
    
    // Save if we have a newer board than original 1st one
    pCore->uNewRev = uNewRev;
    
    return eExpanderOk;
}


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
Result Expander_Reset(Expander_stCore * pCore)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg={{{0x0000,0x0000,EXPANDER_RESET00,EXPANDER_RESET01,0x0000,0x0000,EXPANDER_RESET00_C,EXPANDER_RESET01_C}}}; // default powerup reg values

    // Does not write expander reg#00 & #01 registers which are read only.
    for (CAddr=eExpanderAddr02; CAddr<=eExpanderAddr07; CAddr++)
    {                                                                  
        // This will reset all register bits to default expander reset power up values (for all pins)
        Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.m_aucRegisters[CAddr]);
        
        // Check if we got a problem
        if(FAILURE(Ret)) break;
    }
    
    return Ret;
}


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
Result Expander_Init(Expander_stCore * pCore)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg={{{0x0000,0x0000,EXPANDER_INIT00,EXPANDER_INIT01,0x0000,0x0000,EXPANDER_INIT00_C,EXPANDER_INIT01_C}}}; // default powerup reg values

    // Does not write expander reg#00 & #01 registers which are read only.
    for (CAddr=eExpanderAddr02; CAddr<=eExpanderAddr07; CAddr++)
    {                                                                  
        // This will initialize all register bits to default expander power up values (for all pins)
        Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.m_aucRegisters[CAddr]);
        
        // Check if we got a problem
        if(FAILURE(Ret)) break;
    }
    
    return Ret;
}


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
Result Expander_Close(Expander_stCore * pCore)
{
    return eExpanderOk;
}


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
Result Expander_SetLocalOscSw(Expander_stCore * pCore, Expander_eLocalOscSw Switch)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // Read back existing related registers content
    CAddr = eExpanderAddr02;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister02.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Configure local oscillator switch
    stReg.stRegister02.stBit.m_ucLocalOscSwOut = (Switch & 0x1);
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister02.m_ucRaw);

    return Ret;
}


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
Result Expander_SetBridgeReset(Expander_stCore * pCore, Expander_eBridgeReset BridgeReset)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // Read back existing related registers content
    CAddr = eExpanderAddr02;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister02.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Configure bridge reset
    stReg.stRegister02.stBit.m_ucSpiBridgeResetNOut = (BridgeReset & 0x1);
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister02.m_ucRaw);

    return Ret;
}


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
Result Expander_SetDacReset(Expander_stCore * pCore, Expander_eDacSelect Dac, Expander_eDacReset DacReset)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // Read back existing related registers content
    CAddr = eExpanderAddr02;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister02.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Configure the dac reset for the specified dac
    switch (Dac)
    {
        case eDac1:
            stReg.stRegister02.stBit.m_ucDac14ResetNOut = (DacReset & 0x1);
            break;
            
        case eDac2:
            stReg.stRegister02.stBit.m_ucDac58ResetNOut = (DacReset & 0x1);
            break;
            
        default:
            Ret = eExpanderInvalidDac;
            return(Ret);
    } 
    
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister02.m_ucRaw);

    return Ret;
}


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
Result Expander_SetPllShutdown(Expander_stCore * pCore, Expander_ePllPowerDown Shutdown)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // Read back existing related registers content
    CAddr = eExpanderAddr03;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister03.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Configure pll shutdown
    stReg.stRegister03.stBit.m_ucPllPowerDownNOut = (Shutdown & 0x1);
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister03.m_ucRaw);

    return Ret;
}


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
Result Expander_SetPllSync(Expander_stCore * pCore, Expander_ePllSync Sync)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // Read back existing related registers content
    CAddr = eExpanderAddr03;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister03.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Configure pll sync reset
    stReg.stRegister03.stBit.m_ucPllSyncNOut = (Sync & 0x1);
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister03.m_ucRaw);

    return Ret;
}


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
Result Expander_GetPllLock(Expander_stCore * pCore, Expander_ePllLock *pLock)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // By default pll is unlock
    *pLock = ePllLockOff;
        
    // Read back existing related register content
    CAddr = eExpanderAddr01;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister01.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Get pll lock status
    *pLock = stReg.stRegister01.stBit.m_ucPllLockIn;

    return Ret;
}


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
Result Expander_GetDacIrq(Expander_stCore * pCore, Expander_eDacIrq *pDacIrq)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // By default all dacs have errors
    *pDacIrq = eDacBothErrors;
        
    // Read back existing related register content
    CAddr = eExpanderAddr01;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister01.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Get dac irq status
    *pDacIrq = stReg.stRegister01.stBit.m_ucDacIrqNIn;

    return Ret;
}


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
Result Expander_SetClkSource(Expander_stCore * pCore, Expander_eClkSrc Source)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // Read back existing related registers content
    CAddr = eExpanderAddr02;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister02.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Configure the clock source
    switch (Source)
    {
        case eInternalClk:
            stReg.stRegister02.stBit.m_ucClkMuxSel0Out = 0;
            // Newer board rev?
            if (pCore->uNewRev)
            {
                stReg.stRegister02.stBit.m_ucClkMuxSel1Out = 1;
            }
            else
            {
                stReg.stRegister02.stBit.m_ucClkMuxSel1Out = 0;
            }
            break;
            
        case eExternalClk:
            stReg.stRegister02.stBit.m_ucClkMuxSel0Out = 1;
            stReg.stRegister02.stBit.m_ucClkMuxSel1Out = 1;
            break;
            
        case eCarrier2Clk:
            stReg.stRegister02.stBit.m_ucClkMuxSel0Out = 1;
            stReg.stRegister02.stBit.m_ucClkMuxSel1Out = 0;
            break;
            
        case eCarrier3Clk:
            stReg.stRegister02.stBit.m_ucClkMuxSel0Out = 0;
            stReg.stRegister02.stBit.m_ucClkMuxSel1Out = 0;
            break;
            
        default:
            Ret = eExpanderInvalidClkSource;
            return(Ret);
    } 
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister02.m_ucRaw);

    return Ret;
}


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
Result Expander_SetClkReference(Expander_stCore * pCore, Expander_eClkRef Reference)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // Read back existing related registers content
    CAddr = eExpanderAddr02;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister02.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Configure the clock reference (master clock or master ref)
    stReg.stRegister02.stBit.m_ucPllMuxSel0Out = (Reference & 1);
    
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister02.m_ucRaw);

    return Ret;
}


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
Result Expander_WritePortsRaw(Expander_stCore * pCore, Expander_eIoPortAddr Addr, unsigned char ucPort)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    switch (Addr)
    {
        case eIoPortLow:
            // Set port0 output raw
            CAddr = eExpanderAddr02;
            stReg.stRegister02.m_ucRaw = ucPort;
            Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister02.m_ucRaw);
            break;
        
        case eIoPortHigh:
            // Set port1 output raw
            CAddr = eExpanderAddr03;
            stReg.stRegister03.m_ucRaw = ucPort;
            Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister03.m_ucRaw);
            break;
        
        default:
            Ret = eExpanderInvalidPort01;
            break;
    }

    return Ret;
}


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
Result Expander_ReadPortsRaw(Expander_stCore * pCore, Expander_eIoPortAddr Addr, unsigned char *pucPort)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // By default all outputs fake 0
    *pucPort = 0;
    
    switch (Addr)
    {
        case eIoPortLow:
            // Reads port0 output raw
            CAddr = eExpanderAddr02;
            Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister02.m_ucRaw);
            if(FAILURE(Ret)) return Ret;
            *pucPort = stReg.stRegister02.m_ucRaw;
            break;
        
        case eIoPortHigh:
            // Reads port1 output raw
            CAddr = eExpanderAddr03;
            Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister03.m_ucRaw);
            if(FAILURE(Ret)) return Ret;
            *pucPort = stReg.stRegister03.m_ucRaw;
            break;
        
        default:
            Ret = eExpanderInvalidPort02;
            break;
    }

    return Ret;
}


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
Result Expander_ReadPortsPinRaw(Expander_stCore * pCore, Expander_eIoPortAddr Addr, unsigned char *pucPort)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // By default all inputs fake 0
    *pucPort = 0;
    
    switch (Addr)
    {
        case eIoPortLow:
            // Reads port0 inputs raw
            CAddr = eExpanderAddr00;
            Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister00.m_ucRaw);
            if(FAILURE(Ret)) return Ret;
            *pucPort = stReg.stRegister00.m_ucRaw;
            break;
        
        case eIoPortHigh:
            // Reads port1 inputs raw
            CAddr = eExpanderAddr01;
            Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister01.m_ucRaw);
            if(FAILURE(Ret)) return Ret;
            *pucPort = stReg.stRegister01.m_ucRaw;
            break;
        
        default:
            Ret = eExpanderInvalidPort03;
            break;
    }

    return Ret;
}


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
Result Expander_SetDataMux(Expander_stCore * pCore, Expander_ePathMuxSel DciPath, Expander_ePathMuxSel FramePath)
{
    Result Ret;
    Expander_eAddr CAddr;
    Expander_stRegisterMap stReg;

    // Read back existing related registers content
    CAddr = eExpanderAddr02;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister02.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    CAddr = eExpanderAddr03;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, (unsigned char*) &stReg.stRegister03.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    stReg.stRegister02.stBit.m_ucDciMuxSelOut = (DciPath & 0x1);
    stReg.stRegister03.stBit.m_ucFrameMuxSelOut = (FramePath & 0x1);
    
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister03.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    CAddr = eExpanderAddr02;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePorts, CAddr, &stReg.stRegister02.m_ucRaw);

    return Ret;
}


