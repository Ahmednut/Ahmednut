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
 * @file       Ad9148.h
 * @brief      DAC related functions definitions
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

#ifndef AD9148_H__
#define AD9148_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "Mo1000DevCom.h"
#include "Ad9148_defines.h"
#include "Ad9148_regs.h"


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

 /****************************************************************************
 * NAME : Ad9148_Open
 ************************************************************************//**
 *
 * Open the Ad9148 dac module
 * 
 * @warning
 * - The Mo1000DevCom module must be opened before using this module.
 * - This function must be the first one used before using a DAC module.
 * - When the module is not used anymore, it can be closed with Ad9148_Close(). 
 *
 * @param [out] pCore
 *    This structure will hold necessary information for the module to be used
 *
 * @param [in] pCom
 *    This structure must hold communication module information
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @see
 *    Ad9148_Close().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_Open(Ad9148_stCore * pCore, Mo1000DevCom_stComCore * pCom);


 /****************************************************************************
 * NAME : Ad9148_Reset
 ************************************************************************//**
 *
 * Reset the specified ad9148 dac module to default non operating condition
 * Default chip operation is configured with complement 2 binary output format. 
 *
 * @warning
 * - This function must be called at least once to reset this device to default
 *   non operating condition.
 * - The module must be opened with Ad9148_Open() before using this function.    
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_Reset(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev);


 /****************************************************************************
 * NAME : Ad9148_Init
 ************************************************************************//**
 *
 * Initialize the specified ad9148 dac module to default operating condition
 *
 * @warning
 * - This function must be called at least once to initialize this device to
 *   default operating condition.
 * - Call Ad9148_Reset() before using this function.    
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_Init(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev);


 /****************************************************************************
 *   NAME : Ad9148_Close
 ************************************************************************//**
 *
 * Close the ad9148 dac module
 * 
 * @warning
 * - This function must be the last one used after using this module.
 *  
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    Ad9148_Open().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_Close(Ad9148_stCore * pCore);


 /****************************************************************************
 * NAME : Ad9148_WriteReg
 ************************************************************************//**
 *
 * Write a dac register with specified value
 *
 * @warning
 * - This function must be called at least once to initialize this device to
 *   default operating condition.
 * - Call Ad9148_Init() before using this function.    
 * - This function is for test purpose only. It could give unpredictable effects
 *   on the correct module operation. 
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Addr
 *    Which dac register address to access
 *
 * @param [in] ucVal
 *    Which dac register value to write
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_WriteReg(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev, Ad9148_eAddr Addr, unsigned char ucVal);


 /****************************************************************************
 * NAME : Ad9148_ReadReg
 ************************************************************************//**
 *
 * Read a dac register with specified value
 *
 * @warning
 * - This function must be called at least once to initialize this device to
 *   default operating condition.
 * - Call Ad9148_Init() before using this function.
 * - This function is for test purpose only.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Addr
 *    Which dac register address to access
 *
 * @param [out] pucVal
 *    will return dac register value read
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_ReadReg(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev, Ad9148_eAddr Addr, unsigned char *pucVal);


 /****************************************************************************
 *   NAME : Ad9148_ReadTemp
 ************************************************************************//**
 *
 * Read the dac temperature sensor with a resolution of +/- 5 Celsius.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 * - If all dac pairs are in deep power with Ad9148_SetActChannel(), it
 *   is not possible to read dac temperature anymore and this will return wrong temperature.  
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [out] psTemp
 *    Return the temperature read from specified dac
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_ReadTemp(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, short* psTemp);


 /****************************************************************************
 *   NAME : Ad9148_SetSyncMode
 ************************************************************************//**
 *
 * Configure the dac synchronisation mode (fifo or data rate).
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] SyncMode
 *    Which synchronisation mode needed
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *    
 * @see
 *    Ad9148_SetSyncCtrl(), Ad9148_SetSyncAvg(),
 *    Ad9148_SetSyncEdge(), Ad9148_SetSyncOffset(). 
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetSyncMode(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSyncMode SyncMode);


 /****************************************************************************
 *   NAME : Ad9148_SetSyncCtrl
 ************************************************************************//**
 *
 * Enable/disable dac synchronisation mode.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] SyncControl
 *    If synchronisation is enable or not
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetSyncMode(), Ad9148_SetSyncAvg(),
 *    Ad9148_SetSyncEdge(), Ad9148_SetSyncOffset(). 
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetSyncCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSyncControl SyncControl);


 /****************************************************************************
 *   NAME : Ad9148_SetSyncAvg
 ************************************************************************//**
 *
 * Configure the synchronisation sync averaging (before a sync is detected).
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] SyncAvg
 *    What averaging amount is used for synchronisation
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetSyncMode(), Ad9148_SetSyncCtrl(),
 *    Ad9148_SetSyncEdge(), Ad9148_SetSyncOffset(). 
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetSyncAvg(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSyncAveraging SyncAvg);


 /****************************************************************************
 *   NAME : Ad9148_SetSyncEdge
 ************************************************************************//**
 *
 * Configure the synchronisation clock edge used.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] SyncEdge
 *    What edge is used for synchronisation
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetSyncMode(), Ad9148_SetSyncCtrl(), Ad9148_SetSyncAvg(),
 *    Ad9148_SetSyncOffset(). 
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetSyncEdge(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSyncEdge SyncEdge);


 /****************************************************************************
 *   NAME : Ad9148_SetSyncOffset
 ************************************************************************//**
 *
 * Configure the synchronisation fifo offset (phase offset between fifo read
 * and write ptr).
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] FifoOffset
 *    What fifo offset is desired
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetSyncMode(), Ad9148_SetSyncCtrl(), Ad9148_SetSyncAvg(),
 *    Ad9148_SetSyncEdge(). 
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetSyncOffset(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eFifoOffset FifoOffset);


 /****************************************************************************
 *   NAME : Ad9148_SetIrqEvent
 ************************************************************************//**
 *
 * Set the irq events used (the one that will trigger an irq). Each
 * event bit configured at eAd9148EventActive will enable irq generation
 * when the related event flag is eAd9148EventActive. The other event bits
 * not set to eAd9148EventActive are not changed.   
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Event
 *    Event bits structure (bit at eAd9148EventActive will have event enable)
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_ResetIrqEvent(), Ad9148_GetIrqEvents(), Ad9148_ClearIrqEvent().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetIrqEvent(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_stDacEvent Event);


 /****************************************************************************
 *   NAME : Ad9148_ResetIrqEvent
 ************************************************************************//**
 *
 * Reset the irq events specified (the one that will not trigger an irq anymore).
 * Each event bit configured at eAd9148EventActive will disable irq generation
 * for that related event. The other event bits not set to eAd9148EventActive
 * are not changed.   
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Event
 *    Event bits structure (bit at eAd9148EventActive will have event disable)
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetIrqEvent(), Ad9148_GetIrqEvents(), Ad9148_ClearIrqEvent().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_ResetIrqEvent(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_stDacEvent Event);


 /****************************************************************************
 *   NAME : Ad9148_GetIrqEvents
 ************************************************************************//**
 *
 * Get the status of irq events.
 * Each event bit at eAd9148EventActive will trigger and irq if
 * the related event bit has been configured to eAd9148EventActive with
 * Ad9148_SetIrqEvent().
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 * - After reading events, they should be cleared using Ad9148_ResetIrqEvent(). 
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [out] pEvent
 *    Returns events structure. (bits at eAd9148EventActive are the one that could
 *    trigger an irq if related bits are configured to eAd9148EventActive with
 *    Ad9148_SetIrqEvent().
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetIrqEvent(), Ad9148_ResetIrqEvent(), Ad9148_ClearIrqEvent().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_GetIrqEvents(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_stDacEvent *pEvent);


 /****************************************************************************
 *   NAME : Ad9148_ClearIrqEvent
 ************************************************************************//**
 *
 * Clear the irq events specified (the one that could have triggered an irq).
 * Each event bit configured at eAd9148EventActive will clear a possible event
 * generated condition for that related event. The other event bits not set
 * to eAd9148EventActive are not changed.   
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Event
 *    Event bits structure (bit at eAd9148EventActive will clear triggered events)
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetIrqEvent(), Ad9148_GetIrqEvents().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_ClearIrqEvent(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_stDacEvent Event);


 /****************************************************************************
 *   NAME : Ad9148_SetOutCtrl
 ************************************************************************//**
 *
 * Set the dac output control state (enable or not).
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Select
 *    Which internal dac is selected
 *
 * @param [in] State
 *    Dac output control state desired
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetOutCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eDacSelect Select, Ad9148_eOutputState State);


 /****************************************************************************
 *   NAME : Ad9148_GetFifoLvl
 ************************************************************************//**
 *
 * Get the fifo level (encoded in thermometer logic). This level can be used
 * at fifo synchronisation moment. 
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [out] pucLevel
 *    Returns the fifo level.
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_GetFifoLvl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, unsigned char *pucLevel);


 /****************************************************************************
 *   NAME : Ad9148_SetActChannel
 ************************************************************************//**
 *
 * Set if an IQ dac pair is active or not. A pair which is inactive is in deep power
 * down state.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Select
 *    Select which dac IQ pair to control
 *
 * @param [in] State
 *    Set dac IQ pair active state
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetActChannel(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSelectDacPair Select,
                            Ad9148_eActPairState State);


 /****************************************************************************
 *   NAME : Ad9148_SetSedCtrl
 ************************************************************************//**
 *
 * Enable/disable the SED pattern compare logic.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 * - State set to eAd9148SedEnable appears to be buggy in the dac.  
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] State
 *    Sed control state desired
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetSedPatterns(), Ad9148_GetSedResult().
 *     
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetSedCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSedControlState State);


 /****************************************************************************
 *   NAME : Ad9148_SetSedPatterns
 ************************************************************************//**
 *
 * Set SED pattern compare logic with the 4 specified patterns.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Select
 *    Select which dac data port (A or B) to control
 *
 * @param [in] usP1
 *    1st 16 bits pattern to compare
 *
 * @param [in] usP2
 *    2nd 16 bits pattern to compare
 *
 * @param [in] usP3
 *    3rd 16 bits pattern to compare
 *
 * @param [in] usP4
 *    4th 16 bits pattern to compare
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetSedCtrl(), Ad9148_GetSedResult().
 *     
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetSedPatterns(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSelectSedPort Select,
                            unsigned short usP1, unsigned short usP2, unsigned short usP3, unsigned short usP4);


 /****************************************************************************
 *   NAME : Ad9148_GetSedResult
 ************************************************************************//**
 *
 * Get SED pattern compare logic results.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Select
 *    Select which dac data port (A or B) to control
 *
 * @param [out] pusRisingEdge
 *    cumulative 16 bits rising edge sed compare errors
 *
 * @param [out] pusFallingEdge
 *    cumulative 16 bits falling edge sed compare errors
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetSedCtrl(), Ad9148_SetSedPatterns().
 *     
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_GetSedResult(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSelectSedPort Select,
                           unsigned short *pusRisingEdge, unsigned short *pusFallingEdge);


 /****************************************************************************
 *   NAME : Ad9148_SetISinc
 ************************************************************************//**
 *
 * Set inverse sinc filter coefficients (real and imaginary) for a dac
 * pair (real coefficients for I dac and imaginary for Q dac).
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 * - Dac output center frequency affects the isinc filter coefficients.
 * - Dac interpolation rate affects the isinc filter coefficients.  
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Select
 *    Select which dac IQ pair to control
 *
 * @param [in] stReal
 *    Inverse sinc filter real coefficients
 *
 * @param [in] stImaginary
 *    Inverse sinc filter imaginary coefficients
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetISincCtrl().
 *     
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetISinc(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSelectDacPair Select,
                       Ad9148_stISincCoefficients stReal, Ad9148_stISincCoefficients stImaginary);


 /****************************************************************************
 *   NAME : Ad9148_SetISincCtrl
 ************************************************************************//**
 *
 * Enable/disable the inverse sinc filter.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 * - Dac output center frequency affects the isinc filter coefficients.
 * - Dac interpolation rate affects the isinc filter coefficients.  
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] State
 *    Inverse sinc filter control state desired
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetISinc().
 *     
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetISincCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eISincControlState State);


 /****************************************************************************
 *   NAME : Ad9148_SetInterpolation
 ************************************************************************//**
 *
 * Set interpolation mode for all 4 dacs.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 * - Dac clock must agree with choosen interpolation mode.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Mode
 *    Choosen interpolation mode
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetInterpolation(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev,
                               Ad9148_eInterpolationMode Mode);


 /****************************************************************************
 *   NAME : Ad9148_SetFineMod
 ************************************************************************//**
 *
 * Set fine modulation options for all 4 dacs.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 * - Call Ad9148_SetDacParFineModCtrl() to enable the fine modulation before
 *   configuring it with this function.      
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
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
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init(), Ad9148_SetFineModCtrl().
 *
 * @see
 *    Ad9148_SetFineModCtrl().
 *     
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetFineMod(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, unsigned uFtw,
                         unsigned short usNcoPhaseOffset, Ad9148_eNcoSideband Sideband);


 /****************************************************************************
 *   NAME : Ad9148_SetFineModCtrl
 ************************************************************************//**
 *
 * Enable/disable the fine modulation.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] State
 *    Fine modulation control state desired
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *    Ad9148_SetFineModCtrl().
 *     
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetFineModCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eNcoControl State);


 /****************************************************************************
 *   NAME : Ad9148_SetDataFormat
 ************************************************************************//**
 *
 * Set the data format used.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Format
 *    Which data format to use
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetDataFormat(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eDataFormat Format);


 /****************************************************************************
 *   NAME : Ad9148_SetQuadPhase
 ************************************************************************//**
 *
 * Set the quadrature phase offsets (phase between the I dac (real) and Q dac
 * imaginary dacs pair. Each phase offset is in the range of +/- 1.75 deg
 * and when combined could give +/- 3.5 deg between I and Q. 
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Select
 *    Select which dac IQ pair to control
 *
 * @param [in] sRealPhase
 *    I dac quadrature phase offset. Each count gives 0.00342 deg offset. The value is
 *    a sign integer to give positive or negative offsets. 
 *
 * @param [in] sImaginaryPhase
 *    Q dac quadrature phase offset. Each count gives 0.00342 deg offset. The value is
 *    a sign integer to give positive or negative offsets. 
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetQuadPhase(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSelectDacPair Select,
                           short sRealPhase, short sImaginaryPhase);


 /****************************************************************************
 *   NAME : Ad9148_SetDcOffset
 ************************************************************************//**
 *
 * Set the dac dc offset amplitude correction. The value is a 16 bits signed
 * value and each count gives 0.305uA offset (from 0 to 20 mA). 
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Select
 *    Which internal dac is selected
 *
 * @param [in] sOffset
 *    Dc offset desired
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetDcOffset(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eDacSelect Select, short sOffset);


 /****************************************************************************
 *   NAME : Ad9148_SetGain
 ************************************************************************//**
 *
 * Set the dac gain amplitude correction. The value is a 8 bits unsigned
 * value and each count gives a gain of 0.015625 (ScaleFactor = ucGain / 64). 
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] Select
 *    Which internal dac is selected
 *
 * @param [in] ucGain
 *    Gain correction desired
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_SetGain(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eDacSelect Select, unsigned char ucGain);


 /****************************************************************************
 * NAME : Ad9148_ResetAndRestore
 ************************************************************************//**
 *
 * Reset the specified ad9148 dac module and restore the state it was before
 * the reset condition. 
 *
 * @warning
 * - The module must be opened with Ad9148_Open() before using this function.    
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @return   Error code: eAd9148Ok if no problem is found, !eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open().
 *
 * @ingroup AD9148
 *
 ***************************************************************************/ 
Result Ad9148_ResetAndRestore(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev);


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

