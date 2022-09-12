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
 * @file       Ad9148.c
 * @brief      DAC related functions
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
#include "Ad9148.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

// Structure for module operation
typedef struct Ad9148_InterSetup {
    unsigned short usPremod:1;        // Premod setting
    unsigned short usHb1:2;           // Hb1 setting
    unsigned short usHb2:3;           // Hb2 setting
    unsigned short usHb3:3;           // Hb3 setting
    unsigned short usHb1Bypass:1;     // Hb1 bypass control
    unsigned short usHb2Bypass:1;     // Hb2 bypass control
    unsigned short usHb3Bypass:1;     // Hb3 bypass control
}Ad9148_stInterSetup;

// This loop count is used for the ftw update acknowledge check
#define AD9148_FTWACK_TIME_OUT 1000

// This is default dac registers software reset structure
#define AD9148_RESET {{{ \
0x0, 0x0, 0x0, 0x20,0x0, 0x0, 0x0, 0x0, 0x37,0xc0,0x40,0x0, 0xf1,0xd9,0x0, 0x0,  \
0x8, 0x0, 0x0, 0x0, 0x0, 0xa8,0x0, 0x0, 0x55,0x0, 0x55,0x0, 0x40,0x0, 0x81,0x7,  \
0x0, 0xc0,0xef,0x7f,0x69,0xe6,0xd, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  \
0xf9,0x1, 0x0, 0x0, 0xf9,0x1, 0x0, 0x0, 0xb6,0x7a,0x45,0xea,0x16,0x1a,0xc6,0xaa, \
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  \
0x40,0x40,0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80,0x0, 0x1, 0x20,0x66,0x49, \
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  \
0x0, 0x0, 0x0   \
}}}

/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/
static const Ad9148_stInterSetup InterpolationSet[eAd9148Inter8x_7fsdiv8+1]={
{0,0,0,0,1,1,1},                 /* eAd9148Inter1x         */         
{0,0,0,0,0,1,1},                 /* eAd9148Inter2x      */
{1,1,0,0,0,1,1},                 /* eAd9148Inter2x_fsdiv4  */
{0,2,0,0,0,1,1},                 /* eAd9148Inter2x_fsdiv2  */
{1,3,0,0,0,1,1},                 /* eAd9148Inter2x_3fsdiv4 */
{0,0,0,0,0,0,1},                 /* eAd9148Inter4x      */
{1,1,1,0,0,0,1},                 /* eAd9148Inter4x_fsdiv8  */
{0,2,2,0,0,0,1},                 /* eAd9148Inter4x_fsdiv4  */
{1,3,3,0,0,0,1},                 /* eAd9148Inter4x_3fsdiv8 */
{0,0,4,0,0,0,1},                 /* eAd9148Inter4x_fsdiv2  */
{1,1,5,0,0,0,1},                 /* eAd9148Inter4x_5fsdiv8 */
{0,2,6,0,0,0,1},                 /* eAd9148Inter4x_3fsdiv4 */
{1,3,7,0,0,0,1},                 /* eAd9148Inter4x_7fsdiv8 */
{0,0,0,0,0,0,0},                 /* eAd9148Inter8x      */
{0,2,2,1,0,0,0},                 /* eAd9148Inter8x_fsdiv8  */
{0,0,4,2,0,0,0},                 /* eAd9148Inter8x_fsdiv4  */
{0,2,6,3,0,0,0},                 /* eAd9148Inter8x_3fsdiv8 */
{0,0,0,4,0,0,0},                 /* eAd9148Inter8x_fsdiv2  */
{0,2,2,5,0,0,0},                 /* eAd9148Inter8x_5fsdiv8 */
{0,0,4,6,0,0,0},                 /* eAd9148Inter8x_3fsdiv4 */
{0,2,6,7,0,0,0},                 /* eAd9148Inter8x_7fsdiv8 */
};


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/
static Result _Ad9148_UpdateFineMod(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev);


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 * NAME : _Ad9148_UpdateFineMod
 ************************************************************************//**
 *
 * Execute the frame signal calibration by scanning frame delay taps
 * and finding the optimum delay taps value.
 *
 * @warning
 * - Call Ad9148_Init() before using this function.
 * - Program the nco parameters before using this function to activate them
 *
 * @param [in] pCore
 *    This structure is the one returned at module opening with Ad9148_Open()
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *     
 * @return   Error code: #eAd9148Ok if no problem is found, !#eAd9148Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Ad9148_Open(), Ad9148_Reset(), Ad9148_Init().
 *
 * @see
 *
 ***************************************************************************/ 
static Result _Ad9148_UpdateFineMod(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;
    volatile int iTimeOut;

    // Read the power control register
    CAddr = eAd9148Addr01;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA01PowerControl.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Checks if all dacs are in power down for this device?
    if (stReg.stA01PowerControl.stBit.m_ucPowerDownDacSet1 == 0 || stReg.stA01PowerControl.stBit.m_ucPowerDownDacSet2 == 0)
    {
        // No, Force a ftw update

        // Read the nco control reg
        CAddr = eAd9148Addr5a;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA5ADdsModControl.m_ucRaw);
        if(FAILURE(Ret)) return Ret;

        stReg.stA5ADdsModControl.stBit.m_ucFtwUpdateRequest = 0;
    
        // Prepare ftw update
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA5ADdsModControl.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        stReg.stA5ADdsModControl.stBit.m_ucFtwUpdateRequest = 1;
    
        // Request ftw update
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA5ADdsModControl.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
    
        // Wait until ftw update is acknowledge or until timeout
        for(iTimeOut=AD9148_FTWACK_TIME_OUT; iTimeOut!=0; iTimeOut--)
        {
            // Check if the request was acknowledge
            // Read back and verified that the ftw update has been acknowledge
            Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA5ADdsModControl.m_ucRaw);
            if(FAILURE(Ret)) return Ret;
    
            if (stReg.stA5ADdsModControl.stBit.m_ucFtwUpdateAck == 1)
            {
                break;
            }
        }
        
        // Did we get a timeout
        if (!iTimeOut)
        {
            // Problem, we didn't get the ftw update acknowledge in time
            Ret = eAd9148FtwUpdateTimeOut;
        }
    }

    return Ret;
}


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
Result Ad9148_Open(Ad9148_stCore * pCore, Mo1000DevCom_stComCore * pCom)
{
    pCore->m_pCom = pCom;
    return eAd9148Ok;
}


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
Result Ad9148_Reset(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev)
{
    Result Ret=eAd9148Ok;
    Ad9148_eAddr CAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    
    // Read the Comm register
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Prepare for reset
    stReg.stA00Comm.stBit.m_ucSoftwareReset = 0;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Execute the software reset
    stReg.stA00Comm.stBit.m_ucSoftwareReset = 1;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // End reset
    stReg.stA00Comm.stBit.m_ucSoftwareReset = 0;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Set everything in deep power down
    CAddr = eAd9148Addr01;
    stReg.stA01PowerControl.stBit.m_ucPowerDownDacSet1 = 1;
    stReg.stA01PowerControl.stBit.m_ucPowerDownDacSet2 = 1;
    stReg.stA01PowerControl.stBit.m_ucPowerDownDataRec = 1;
    stReg.stA01PowerControl.stBit.m_ucReserved = 0;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA01PowerControl.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Reg 33 *
    CAddr = eAd9148Addr33;
    stReg.stA33AuxIDacControl.m_ucRaw = 0;
    stReg.stA33AuxIDacControl.stBit.m_ucAuxIDacPowerDown = 1;       // Auxiliary idac power down (not used)
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA33AuxIDacControl.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Reg 37 *
    CAddr = eAd9148Addr37;
    stReg.stA37AuxQDacControl.m_ucRaw = 0;
    stReg.stA37AuxQDacControl.stBit.m_ucAuxQDacPowerDown = 1;       // Auxiliary Q dac power down (not used)
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA37AuxQDacControl.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Select the dac bank 1
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = 1;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr00, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Reg 33 *
    CAddr = eAd9148Addr33;
    stReg.stA33AuxIDacControl.m_ucRaw = 0;
    stReg.stA33AuxIDacControl.stBit.m_ucAuxIDacPowerDown = 1;       // Auxiliary idac power down (not used)
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA33AuxIDacControl.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Reg 37 *
    CAddr = eAd9148Addr37;
    stReg.stA37AuxQDacControl.m_ucRaw = 0;
    stReg.stA37AuxQDacControl.stBit.m_ucAuxQDacPowerDown = 1;       // Auxiliary Q dac power down (not used)
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA37AuxQDacControl.m_ucRaw);

    return Ret;
}


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
Result Ad9148_Init(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev)
{
    Result Ret = eAd9148Ok;
    Ad9148_eAddr CAddr;
    unsigned short sData;
    Ad9148_stRegisterMap stReg = AD9148_RESET;

    // Reg 03
    stReg.stA03DataFormat.stBit.m_ucDualPortMode = 0;               // Single port mode
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr03, &stReg.stA03DataFormat.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Reg 14
    stReg.stA14DataReceiverControl.stBit.m_ucOneDci = 1;            // use only one dci (DCIA_x)
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr14, &stReg.stA14DataReceiverControl.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Reg 1e
    stReg.stA1EHb3Control.stBit.m_ucBypassDigitalGainAndPhase = 0;  // Do not bypass digital gain and phase adjustement
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr1e, &stReg.stA1EHb3Control.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Reg 30 *
    stReg.stA30IDacFscAdj.m_ucRaw = 0xfb;                           // Full scale current 0x1fb ( (0x1fb * 22.5e-6 + 8.64e-3) = 20.0475mA -> v = 49.9 * 20.0475e-3 = 1.00037025V

    // Reg 31 *
    stReg.stA31IDacControl.stBit.m_ucIDacSleep = 1;                 // I dac sleep

    // Reg 34 *
    stReg.stA34QDacFscAdj.m_ucRaw = 0xfb;                           // Full scale current 0x1fb ( (0x1fb * 22.5e-6 + 8.64e-3) = 20.0475mA -> v = 49.9 * 20.0475e-3 = 1.00037025V

    // Reg 35 *
    stReg.stA35QDacControl.stBit.m_ucQDacSleep = 1;                 // Q dac sleep

    // Select the dac bank 0
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = 0;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr00, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Write for dacs bank 0
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, eAd9148Addr30, &stReg.stA30IDacFscAdj.m_ucRaw, 6);
    if(FAILURE(Ret)) return Ret;

    // Select the dac bank 1
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = 1;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr00, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Write for dacs bank 1
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, eAd9148Addr30, &stReg.stA30IDacFscAdj.m_ucRaw, 6);
    if(FAILURE(Ret)) return Ret;

    // Reg 5d
    stReg.stA5DDieTempControl1.m_ucRaw = 0x0a;                      // Setup to use the temp die
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr5d, &stReg.stA5DDieTempControl1.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Reg 5c
    stReg.stA5CDieTempControl0.m_ucRaw = 0x0;                       // Remove temp sensor power down
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr5c, &stReg.stA5CDieTempControl0.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Reg 01 (remove main dac power downs from reset)
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr01, &stReg.m_aucRegisters[eAd9148Addr01]);
    if(FAILURE(Ret)) return Ret;

    // Do a dummy temp read to prevent getting 1st corrupted temp
    // Read the temperature
    // !!! M.T. To be modified later
    stReg.stA5CDieTempControl0.stBit.m_ucLatchTempData = 1;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr5c, &stReg.stA5CDieTempControl0.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    CAddr = eAd9148Addr5e;
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, CAddr, &sData, 2);

    return Ret;
}


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
Result Ad9148_Close(Ad9148_stCore * pCore)
{
    return eAd9148Ok;
}


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
Result Ad9148_WriteReg(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev, Ad9148_eAddr Addr, unsigned char ucVal)
{
    Result Ret = eAd9148InvalidAddr01;

    // Check if address is valid
    if (Addr >= eAd9148Addr00 && Addr <= eAd9148Addr72)
    {
        // Write register content
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, Addr, (unsigned char*) &ucVal);
    }

    return Ret;
}


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
Result Ad9148_ReadReg(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev, Ad9148_eAddr Addr, unsigned char *pucVal)
{
    Result Ret = eAd9148InvalidAddr02;

    *pucVal = 0;
    
    // Check if address is valid
    if (Addr >= eAd9148Addr00 && Addr <= eAd9148Addr72)
    {
        // Read register content
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, Addr, (unsigned char*) pucVal);
    }

    return Ret;
}


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
Result Ad9148_ReadTemp(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, short* psTemp)
{
    Result Ret;
    Ad9148_eAddr CAddr = eAd9148Addr5c;
    unsigned short sData;
    Ad9148_stRegisterMap stReg;

    // Read temp control reg
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA5CDieTempControl0.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Prepare for reading
    stReg.stA5CDieTempControl0.stBit.m_ucLatchTempData = 0;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA5CDieTempControl0.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // Latch the temperature
    stReg.stA5CDieTempControl0.stBit.m_ucLatchTempData = 1;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA5CDieTempControl0.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    CAddr = eAd9148Addr5e;
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, CAddr, &sData, 2);

    // Check if we got an error (or if we cannot read temperature because of power down?)
    if(FAILURE(Ret))
    {
        // Make sure we returns a temperature outside operating range of the sensor
        // to clearly show we are in problem
        *psTemp = AD9148_INVALIDTEMP;
    }
    else
    {
        // Temperature resolution is 5C
        *psTemp = (short)(((int)(sData) - 13700) / 130);
    }

    return Ret;
}


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
Result Ad9148_SetSyncMode(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSyncMode SyncMode)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct sync mode
    if (SyncMode != eAd9148SyncFifoRate && SyncMode != eAd9148SyncDataRate)
    {
        Ret = eAd9148InvalidSyncMode;
    }
    else
    {
        // Read the sync control reg
        CAddr = eAd9148Addr10;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA10SyncControl0.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Configure new sync mode
        stReg.stA10SyncControl0.stBit.m_ucFifoRateDataRateToggle = SyncMode & 1;
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA10SyncControl0.m_ucRaw);
    }
    
    return Ret;
}


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
Result Ad9148_SetSyncCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSyncControl SyncControl)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct sync control
    if (SyncControl != eAd9148SyncDisable && SyncControl != eAd9148SyncEnable)
    {
        Ret = eAd9148InvalidSyncCtrl;
    }
    else
    {
        // Read the sync control reg
        CAddr = eAd9148Addr10;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA10SyncControl0.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Configure sync control
        stReg.stA10SyncControl0.stBit.m_ucSyncEnable = SyncControl & 1;
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA10SyncControl0.m_ucRaw);
    }

    return Ret;
}


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
Result Ad9148_SetSyncAvg(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSyncAveraging SyncAvg)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct sync avg
    if (SyncAvg < eAd9148SyncAveraging1 || SyncAvg > eAd9148SyncAveraging128)
    {
        Ret = eAd9148InvalidSyncAvg;
    }
    else
    {
        // Read the sync control reg
        CAddr = eAd9148Addr10;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA10SyncControl0.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Configure sync averaging
        stReg.stA10SyncControl0.stBit.m_ucSyncAveraging = SyncAvg & 7;
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA10SyncControl0.m_ucRaw);
    }

    return Ret;
}


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
Result Ad9148_SetSyncEdge(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSyncEdge SyncEdge)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct sync edge
    if (SyncEdge != eAd9148SyncFallingEdge && SyncEdge != eAd9148SyncRisingEdge)
    {
        Ret = eAd9148InvalidSyncEdge;
    }
    else
    {
        // Read the sync control reg
        CAddr = eAd9148Addr10;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA10SyncControl0.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Configure sync averaging
        stReg.stA10SyncControl0.stBit.m_ucRisingEdgeSync = SyncEdge & 1;
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA10SyncControl0.m_ucRaw);
    }

    return Ret;
}


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
Result Ad9148_SetSyncOffset(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eFifoOffset FifoOffset)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct fifo offset
    if (FifoOffset < eAd9148FifoOffset0 || FifoOffset > eAd9148FifoOffset7)
    {
        Ret = eAd9148InvalidSyncOffset;
    }
    else
    {
        // Read the Port A offset reg
        CAddr = eAd9148Addr17;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA17FifoStatusControlPortA.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Configure fifo offset
        stReg.stA17FifoStatusControlPortA.stBit.m_ucFifoPhaseOffset = FifoOffset & 7;
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA17FifoStatusControlPortA.m_ucRaw);
        if(FAILURE(Ret)) return Ret;

        // Read the Port B offset reg
        CAddr = eAd9148Addr19;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA19FifoStatusControlPortB.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Configure fifo offset
        stReg.stA19FifoStatusControlPortB.stBit.m_ucFifoPhaseOffset = FifoOffset & 7;
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA19FifoStatusControlPortB.m_ucRaw);
    }

    return Ret;
}


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
Result Ad9148_SetIrqEvent(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_stDacEvent Event)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Read the events interrupt enable regs
    CAddr = eAd9148Addr04;
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 2);
    if(FAILURE(Ret)) return Ret;
    
    // Enable specified irq events
    stReg.stA04InterruptEnable0.m_ucRaw |= Event.stEventParts.m_uEvent0;
    stReg.stA05InterruptEnable1.m_ucRaw |= Event.stEventParts.m_uEvent1;
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 2);

    return Ret;
}


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
Result Ad9148_ResetIrqEvent(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_stDacEvent Event)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Read the events interrupt enable regs
    CAddr = eAd9148Addr04;
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 2);
    if(FAILURE(Ret)) return Ret;
    
    // Disable specified irq events
    stReg.stA04InterruptEnable0.m_ucRaw &= ~(Event.stEventParts.m_uEvent0);
    stReg.stA05InterruptEnable1.m_ucRaw &= ~(Event.stEventParts.m_uEvent1);
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 2);

    return Ret;
}


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
Result Ad9148_GetIrqEvents(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_stDacEvent *pEvent)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // By default return all events 0
    pEvent->stEventParts.m_uEvent0 = 0;
    pEvent->stEventParts.m_uEvent1 = 0;

    // Read the events flags 0 & 1 regs
    CAddr = eAd9148Addr06;
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 2);
    if(FAILURE(Ret)) return Ret;
    
    // Keep only existing bits
    pEvent->stEventParts.m_uEvent0 = stReg.stA06EventFlag0.m_ucRaw & 0xf7;
    pEvent->stEventParts.m_uEvent1 = stReg.stA07EventFlag1.m_ucRaw & 0x1c; 

    return Ret;
}


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
Result Ad9148_ClearIrqEvent(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_stDacEvent Event)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    CAddr = eAd9148Addr06;
    
    // Clear specified irq events
    stReg.stA06EventFlag0.m_ucRaw = Event.stEventParts.m_uEvent0;
    stReg.stA07EventFlag1.m_ucRaw = Event.stEventParts.m_uEvent1;
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 2);

    return Ret;
}


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
Result Ad9148_SetOutCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eDacSelect Select, Ad9148_eOutputState State)
{
    Result Ret;
    Ad9148_eAddr CAddr, BAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    unsigned char ucBank;

    // Internal dac selection
    switch (Select)
    {
        case eAd9148SelectDac1I:
            CAddr = eAd9148Addr31;
            ucBank = 0;
            break;
            
        case eAd9148SelectDac3I:
            CAddr = eAd9148Addr31;
            ucBank = 1;
            break;

        case eAd9148SelectDac2Q:
            CAddr = eAd9148Addr35;
            ucBank = 0;
            break;

        case eAd9148SelectDac4Q:
            CAddr = eAd9148Addr35;
            ucBank = 1;
            break;
            
        default:
            // Invalid dac selected
            Ret = eAd9148InvalidDacSelect01;
            return(Ret);    
    }
    
    // Check for correct control
    if (State != eAd9148OutputDisable && State != eAd9148OutputEnable)
    {
        Ret = eAd9148InvalidOutCtrl;
    }
    else
    {
        // Read the Comm register (for bank selection)
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Select the needed dac bank
        stReg.stA00Comm.stBit.m_ucDacSpiSelect = ucBank;
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Read the dac control register
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr]);
        if(FAILURE(Ret)) return Ret;
        
        // Configure the dac output in specified state
        if (Select == eAd9148SelectDac1I || Select == eAd9148SelectDac3I)
            stReg.stA31IDacControl.stBit.m_ucIDacSleep = State & 1;
        else
            stReg.stA35QDacControl.stBit.m_ucQDacSleep = State & 1;
        
        // Write back the dac control register
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr]);
    }

    return Ret;
}


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
Result Ad9148_GetFifoLvl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, unsigned char *pucLevel)
{
    Result Ret;
    Ad9148_eAddr CAddr;

    // By default level is 0
    *pucLevel = 0;

    // Read the fifo thermometer encoded level
    CAddr = eAd9148Addr18;
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, pucLevel);

    return Ret;
}


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
                            Ad9148_eActPairState State)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct state
    if (State != eAd9148DacPairEnable && State != eAd9148DacPairDisable)
    {
        Ret = eAd9148InvalidOutCtrl;
    }
    else
    {
        // Read the power control register
        CAddr = eAd9148Addr01;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA01PowerControl.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Which dac pair is selected
        switch(Select)
        {
            case eAd9148SelectIQDac12:
                stReg.stA01PowerControl.stBit.m_ucPowerDownDacSet1 = State & 1;
                break;
                
            case eAd9148SelectIQDac34:
                stReg.stA01PowerControl.stBit.m_ucPowerDownDacSet2 = State & 1;
                break;
                
            default:
                // Invalid IQ dac pair selected
                Ret = eAd9148InvalidDacPair01;
                return(Ret);
        }
        
        // Write the power control register
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA01PowerControl.m_ucRaw);
        if(FAILURE(Ret)) return Ret;

        // Update fine mod parameters if a channel has been activated
        if ((State & 1) == 0)
            Ret = _Ad9148_UpdateFineMod(pCore, DacDev);
    }

    return Ret;
}


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
Result Ad9148_SetSedCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eSedControlState State)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Read the sed control status register (we do not read anymore, there are only 2 bits to be written)
    CAddr = eAd9148Addr40;
    //Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA40SedControlStatus.m_ucRaw);
    //if(FAILURE(Ret)) return Ret;
    stReg.stA40SedControlStatus.m_ucRaw = 0;
    
    switch (State)
    {
       case eAd9148SedDisable:
           stReg.stA40SedControlStatus.stBit.m_ucSedCompareEnable = 0;
           break;
       
       case eAd9148SedEnable:
           stReg.stA40SedControlStatus.stBit.m_ucSedCompareEnable = 1;
           stReg.stA40SedControlStatus.stBit.m_ucAutoClearEnable = 0;
           break;
       
       case eAd9148SedAutoClearEnable:
           stReg.stA40SedControlStatus.stBit.m_ucSedCompareEnable = 1;
           stReg.stA40SedControlStatus.stBit.m_ucAutoClearEnable = 1;
           break;
           
        default:
           Ret = eAd9148InvalidSedCtrl;
           return Ret;
    }
     
    // Write back the register
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA40SedControlStatus.m_ucRaw);

    return Ret;
}


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
                            unsigned short usP1, unsigned short usP2, unsigned short usP3, unsigned short usP4)
{
    Result Ret;
    Ad9148_eAddr CAddr, BAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    unsigned char ucBank;

    // Internal dac port selection
    switch (Select)
    {
        case eAd9148SelectPortA:
            ucBank = 0;
            break;
            
        case eAd9148SelectPortB:
            ucBank = 1;
            break;

        default:
            // Invalid dac port selected
            Ret = eAd9148InvalidDacPort01;
            return(Ret);    
    }
    
    // Read the Comm register (for bank selection)
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Select the needed dac bank
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = ucBank;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    CAddr = eAd9148Addr38;
    stReg.stA38SedS0L.stBit.m_ucSedComparePatternSample0L = (usP1 & 0xFF);
    stReg.stA39SedS0H.stBit.m_ucSedComparePatternSample0H = ((usP1 >> 8) & 0xFF);
    stReg.stA3ASedS1L.stBit.m_ucSedComparePatternSample1L = (usP2 & 0xFF);
    stReg.stA3BSedS1H.stBit.m_ucSedComparePatternSample1H = ((usP2 >> 8) & 0xFF);
    stReg.stA3CSedS2L.stBit.m_ucSedComparePatternSample2L = (usP3 & 0xFF);
    stReg.stA3DSedS2H.stBit.m_ucSedComparePatternSample2H = ((usP3 >> 8) & 0xFF);
    stReg.stA3ESedS3L.stBit.m_ucSedComparePatternSample3L = (usP4 & 0xFF);
    stReg.stA3FSedS3H.stBit.m_ucSedComparePatternSample3H = ((usP4 >> 8) & 0xFF);
    
    // Write all 4 SED patterns
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 8);

    return Ret;
}


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
                           unsigned short *pusRisingEdge, unsigned short *pusFallingEdge)
{
    Result Ret;
    Ad9148_eAddr CAddr, BAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    unsigned char ucBank;

    // Default to no errors
    *pusRisingEdge = 0;
    *pusFallingEdge = 0;
            
    // Internal dac port selection
    switch (Select)
    {
        case eAd9148SelectPortA:
            ucBank = 0;
            break;
            
        case eAd9148SelectPortB:
            ucBank = 1;
            break;

        default:
            // Invalid dac port selected
            Ret = eAd9148InvalidDacPort02;
            return(Ret);    
    }
    
    // Read the Comm register (for bank selection)
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Select the needed dac bank
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = ucBank;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    CAddr = eAd9148Addr41;

    // Read all SED compare results
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 4);
    if(FAILURE(Ret)) return Ret;

    *pusRisingEdge = stReg.stA41SedRL.m_ucRaw | (stReg.stA42SedRH.m_ucRaw << 8);
    *pusFallingEdge = stReg.stA43SedFL.m_ucRaw | (stReg.stA44SedFH.m_ucRaw << 8);
    
    return Ret;
}


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
                       Ad9148_stISincCoefficients stReal, Ad9148_stISincCoefficients stImaginary)
{
    Result Ret;
    Ad9148_eAddr CAddr, BAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    unsigned char ucBank;

    // Internal dac selection
    switch (Select)
    {
        case eAd9148SelectIQDac12:
            ucBank = 0;
            break;
            
        case eAd9148SelectIQDac34:
            ucBank = 1;
            break;

        default:
            // Invalid dac selected
            Ret = eAd9148InvalidDacPair03;
            return(Ret);    
    }
    
    // Read the Comm register (for bank selection)
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Select the needed dac bank
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = ucBank;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    CAddr = eAd9148Addr20;
    
    // Set the isinc filter coefficients
    stReg.stA20CoeffIByte0.stBit.m_ucCoeff0I = stReal.stBit.m_iC0;
    stReg.stA20CoeffIByte0.stBit.m_ucCoeff1I = stReal.stBit.m_iC1;
    stReg.stA20CoeffIByte0.stBit.m_ucBit07Eq0 = 0;
    stReg.stA21CoeffIByte1.stBit.m_ucCoeff2I = stReal.stBit.m_iC2;
    stReg.stA21CoeffIByte1.stBit.m_ucCoeff3I20 = (stReal.stBit.m_iC3 & 0x7);
    stReg.stA22CoeffIByte2.stBit.m_ucCoeff3I63 = ((stReal.stBit.m_iC3 >> 3) & 0xf);
    stReg.stA22CoeffIByte2.stBit.m_ucCoeff4I20 = (stReal.stBit.m_iC4 & 0x7);
    stReg.stA22CoeffIByte2.stBit.m_ucBit04Eq0 = 0;
    stReg.stA23CoeffIByte3.stBit.m_ucCoeff4I93 = ((stReal.stBit.m_iC4 >> 3) & 0x7f);
    stReg.stA23CoeffIByte3.stBit.m_ucBit07Eq0 = 0;

    stReg.stA24CoeffQByte0.stBit.m_ucCoeff0Q = stImaginary.stBit.m_iC0;
    stReg.stA24CoeffQByte0.stBit.m_ucCoeff1Q = stImaginary.stBit.m_iC1;
    stReg.stA24CoeffQByte0.stBit.m_ucBit07Eq0 = 0;
    stReg.stA25CoeffQByte1.stBit.m_ucCoeff2Q = stImaginary.stBit.m_iC2;
    stReg.stA25CoeffQByte1.stBit.m_ucCoeff3Q20 = (stImaginary.stBit.m_iC3 & 0x7);
    stReg.stA26CoeffQByte2.stBit.m_ucCoeff3Q63 = ((stImaginary.stBit.m_iC3 >> 3) & 0xf);
    stReg.stA26CoeffQByte2.stBit.m_ucCoeff4Q20 = (stImaginary.stBit.m_iC4 & 0x7);
    stReg.stA26CoeffQByte2.stBit.m_ucBit04Eq0 = 0;
    stReg.stA27CoeffQByte3.stBit.m_ucCoeff4Q93 = ((stImaginary.stBit.m_iC4 >> 3) & 0x7f);
    stReg.stA27CoeffQByte3.stBit.m_ucBit07Eq0 = 0;

    // Write all coefficients
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 8);

    return Ret;
}


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
Result Ad9148_SetISincCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eISincControlState State)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct control
    if (State != eAd9148ISincDisable && State != eAd9148ISincEnable)
    {
        Ret = eAd9148InvalidISincCtrl;
    }
    else
    {
        // Read the hb1 control register
        CAddr = eAd9148Addr1c;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA1CHb1Control.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Set the control state
        stReg.stA1CHb1Control.stBit.m_ucBypassSinc = State & 1;
        
        // Write back the register
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA1CHb1Control.m_ucRaw);
    }

    return Ret;
}


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
                               Ad9148_eInterpolationMode Mode)
{
    Result Ret;
    Ad9148_eAddr CAddr = eAd9148Addr1c;
    Ad9148_stRegisterMap stReg;

    // Read the HBx control registers
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, CAddr, &stReg.stA1CHb1Control.m_ucRaw, 3);
    if(FAILURE(Ret)) return Ret;
    
    // Check for correct interpolation mode
    if (Mode > eAd9148Inter8x_7fsdiv8 || Mode < eAd9148Inter1x)
    {
        // Invalid interpolation mode
        Ret = eAd9148InvalidInterMode;
    }
    else
    {
        // Set the interpolation mode
        stReg.stA1CHb1Control.stBit.m_ucEnablePreMod = InterpolationSet[Mode].usPremod;
        stReg.stA1CHb1Control.stBit.m_ucHb1 = InterpolationSet[Mode].usHb1;
        stReg.stA1CHb1Control.stBit.m_ucBypassHb1 = InterpolationSet[Mode].usHb1Bypass;
        stReg.stA1DHb2Control.stBit.m_ucHb2 = InterpolationSet[Mode].usHb2;
        stReg.stA1DHb2Control.stBit.m_ucBypassHb2 = InterpolationSet[Mode].usHb2Bypass;
        stReg.stA1EHb3Control.stBit.m_ucHb3 = InterpolationSet[Mode].usHb3;
        stReg.stA1EHb3Control.stBit.m_ucBypassHb3 = InterpolationSet[Mode].usHb3Bypass;
    
        // Write all HBx control registers
        Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.stA1CHb1Control.m_ucRaw, 3);
    }

    return Ret;
}


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
                         unsigned short usNcoPhaseOffset, Ad9148_eNcoSideband Sideband)
{
    Result Ret;
    Ad9148_eAddr CAddr = eAd9148Addr54;
    Ad9148_stRegisterMap stReg;

    // Check for correct sideband
    if (Sideband != eAd9148NcoLowSide && Sideband != eAd9148NcoHighSide)
    {
        Ret = eAd9148InvalidSideband;
    }
    else
    {
        // Set the FTW
        stReg.stA54FtwLsb.m_ucRaw = uFtw & 0xff;
        stReg.stA55Ftw2.m_ucRaw = (uFtw >> 8) & 0xff;
        stReg.stA56Ftw3.m_ucRaw = (uFtw >> 16) & 0xff;
        stReg.stA57FtwMsb.m_ucRaw = (uFtw >> 24) & 0xff;
        // Set the phase offset
        stReg.stA59PhaseOffsetLsb.m_ucRaw = usNcoPhaseOffset & 0xff;
        stReg.stA58PhaseOffsetMsb.m_ucRaw = (usNcoPhaseOffset >> 8) & 0xff;
    
        // Write all FTW and phase offset parameters
        Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.stA54FtwLsb.m_ucRaw, 6);
        if(FAILURE(Ret)) return Ret;
    
        // Read the dds mod control registers
        CAddr = eAd9148Addr5a;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA5ADdsModControl.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
    
        // Configure the desired sideband
        stReg.stA5ADdsModControl.stBit.m_ucSidebandSelect = Sideband & 1;
        stReg.stA5ADdsModControl.stBit.m_ucFtwUpdateRequest = 0;
    
        // Write sideband and prepare for ftw update
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA5ADdsModControl.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
        
        // Update fine mod parameters
        Ret = _Ad9148_UpdateFineMod(pCore, DacDev);
    }

    return Ret;
}


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
Result Ad9148_SetFineModCtrl(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eNcoControl State)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct control
    if (State != eAd9148FineModEnable && State != eAd9148FineModDisable)
    {
        Ret = eAd9148InvalidNcoCtrl;
    }
    else
    {
        // Read the dds mod control registers
        CAddr = eAd9148Addr5a;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA5ADdsModControl.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
    
        // Set the fine modulation control
        stReg.stA5ADdsModControl.stBit.m_ucBypassDdsMod = State & 1;
        
        // Write back the register
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA5ADdsModControl.m_ucRaw);
    }

    return Ret;
}


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
Result Ad9148_SetDataFormat(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eDataFormat Format)
{
    Result Ret;
    Ad9148_eAddr CAddr;
    Ad9148_stRegisterMap stReg;

    // Check for correct control
    if (Format != eAd9148FormatBinary && Format != eAd9148Format2Complement)
    {
        Ret = eAd9148InvalidDataFormat;
    }
    else
    {
        // Read the data format register
        CAddr = eAd9148Addr03;
        Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA03DataFormat.m_ucRaw);
        if(FAILURE(Ret)) return Ret;
    
        // Set the data format
        stReg.stA03DataFormat.stBit.m_ucBinaryFormat = Format & 1;
        
        // Write back the register
        Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA03DataFormat.m_ucRaw);
    }

    return Ret;
}


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
                           short sRealPhase, short sImaginaryPhase)
{
    Result Ret;
    Ad9148_eAddr CAddr, BAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    unsigned char ucBank;

    // Internal dac selection
    switch (Select)
    {
        case eAd9148SelectIQDac12:
            ucBank = 0;
            break;
            
        case eAd9148SelectIQDac34:
            ucBank = 1;
            break;

        default:
            // Invalid dac selected
            Ret = eAd9148InvalidDacPair02;
            return(Ret);    
    }
    
    // Check if I quadrature phase is ok
    if (sRealPhase > AD9148_QUADPHASERANGEMAX || sRealPhase < AD9148_QUADPHASERANGEMIN)
        return(eAd9148IQuadPhaseErr);    
    
    // Check if Q quadrature phase is ok
    if (sImaginaryPhase > AD9148_QUADPHASERANGEMAX || sImaginaryPhase < AD9148_QUADPHASERANGEMIN)
        return(eAd9148QQuadPhaseErr);    
    
    // Read the Comm register (for bank selection)
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Select the needed dac bank
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = ucBank;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    CAddr = eAd9148Addr28;
    
    // Set the quadrature phases
    stReg.stA28IPhaseAdjLsb.stBit.m_ucPhaseWordI70 = (sRealPhase & 0xff);
    stReg.stA29IPhaseAdjMsb.m_ucRaw = 0;
    stReg.stA29IPhaseAdjMsb.stBit.m_ucPhaseWordI98 = (sRealPhase >> 8) & 3;
    stReg.stA2AQPhaseAdjLsb.stBit.m_ucPhaseWordQ70 = (sImaginaryPhase & 0xff);
    stReg.stA2BQPhaseAdjMsb.m_ucRaw = 0;
    stReg.stA2BQPhaseAdjMsb.stBit.m_ucPhaseWordQ98 = (sImaginaryPhase >> 8) & 3;

    // Write all coefficients
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 4);

    return Ret;
}


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
Result Ad9148_SetDcOffset(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eDacSelect Select, short sOffset)
{
    Result Ret;
    Ad9148_eAddr CAddr, BAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    unsigned char ucBank;

    // Internal dac selection
    switch (Select)
    {
        case eAd9148SelectDac1I:
            CAddr = eAd9148Addr2c;
            ucBank = 0;
            break;
            
        case eAd9148SelectDac3I:
            CAddr = eAd9148Addr2c;
            ucBank = 1;
            break;

        case eAd9148SelectDac2Q:
            CAddr = eAd9148Addr2e;
            ucBank = 0;
            break;

        case eAd9148SelectDac4Q:
            CAddr = eAd9148Addr2e;
            ucBank = 1;
            break;
            
        default:
            // Invalid dac selected
            Ret = eAd9148InvalidDacSelect02;
            return(Ret);    
    }
    
    // Read the Comm register (for bank selection)
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Select the needed dac bank
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = ucBank;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Set the dc offset
    stReg.m_aucRegisters[CAddr] = (sOffset & 0xff);
    stReg.m_aucRegisters[CAddr+1] = (sOffset >> 8);
    
    // Write back the dac dc offset registers
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr], 2);

    return Ret;
}


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
Result Ad9148_SetGain(Ad9148_stCore * pCore,  Mo1000DevCom_eDevices DacDev, Ad9148_eDacSelect Select, unsigned char ucGain)
{
    Result Ret;
    Ad9148_eAddr CAddr, BAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    unsigned char ucBank;

    // Internal dac selection
    switch (Select)
    {
        case eAd9148SelectDac1I:
            CAddr = eAd9148Addr50;
            ucBank = 0;
            break;
            
        case eAd9148SelectDac3I:
            CAddr = eAd9148Addr50;
            ucBank = 1;
            break;

        case eAd9148SelectDac2Q:
            CAddr = eAd9148Addr51;
            ucBank = 0;
            break;

        case eAd9148SelectDac4Q:
            CAddr = eAd9148Addr51;
            ucBank = 1;
            break;
            
        default:
            // Invalid dac selected
            Ret = eAd9148InvalidDacSelect03;
            return(Ret);    
    }
    
    // Read the Comm register (for bank selection)
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Select the needed dac bank
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = ucBank;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, BAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Set the gain correction
    stReg.m_aucRegisters[CAddr] = ucGain;
    
    // Write back the dac gain correction register
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.m_aucRegisters[CAddr]);

    return Ret;
}


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
Result Ad9148_ResetAndRestore(Ad9148_stCore * pCore, Mo1000DevCom_eDevices DacDev)
{
    Result Ret=eAd9148Ok;
    Ad9148_eAddr CAddr = eAd9148Addr00;
    Ad9148_stRegisterMap stReg;
    Ad9148_stRegisterMap stReg1;
    
    // Read the Comm register
    Ret = Mo1000DevCom_Read(pCore->m_pCom, DacDev, CAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Prepare for reset
    stReg.stA00Comm.stBit.m_ucSoftwareReset = 0;

    // Select the dac bank 0
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = 0;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Read for dacs bank 0
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, eAd9148Addr01, &stReg.m_aucRegisters[eAd9148Addr01], sizeof(stReg.m_aucRegisters) - 1);
    if(FAILURE(Ret)) return Ret;

    // Select the dac bank 1
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = 1;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr00, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Read for dacs bank 1
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, eAd9148Addr20, &stReg1.m_aucRegisters[eAd9148Addr20], 32);
    if(FAILURE(Ret)) return Ret;
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, eAd9148Addr41, &stReg1.m_aucRegisters[eAd9148Addr41], 4);
    if(FAILURE(Ret)) return Ret;
    Ret = Mo1000DevCom_ReadArray(pCore->m_pCom, DacDev, eAd9148Addr50, &stReg1.m_aucRegisters[eAd9148Addr50], 2);
    if(FAILURE(Ret)) return Ret;

    // Execute the software reset
    stReg.stA00Comm.stBit.m_ucSoftwareReset = 1;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;

    // End reset
    stReg.stA00Comm.stBit.m_ucSoftwareReset = 0;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, CAddr, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Restore back registers as pre reset condition
    // Write for dacs bank 0
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr5a, &stReg.m_aucRegisters[eAd9148Addr5a]); // Needed for nco
    if(FAILURE(Ret)) return Ret;
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, eAd9148Addr01, &stReg.m_aucRegisters[eAd9148Addr01], sizeof(stReg.m_aucRegisters) - 1);
    if(FAILURE(Ret)) return Ret;

    // Select the dac bank 1
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = 1;
    Ret = Mo1000DevCom_Write(pCore->m_pCom, DacDev, eAd9148Addr00, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) return Ret;
    
    // Write for dacs bank 1
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, eAd9148Addr20, &stReg1.m_aucRegisters[eAd9148Addr20], 32);
    if(FAILURE(Ret)) return Ret;
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, eAd9148Addr41, &stReg1.m_aucRegisters[eAd9148Addr41], 4);
    if(FAILURE(Ret)) return Ret;
    Ret = Mo1000DevCom_WriteArray(pCore->m_pCom, DacDev, eAd9148Addr50, &stReg1.m_aucRegisters[eAd9148Addr50], 2);
    if(FAILURE(Ret)) return Ret;

    // Update nco setting
    Ret = _Ad9148_UpdateFineMod(pCore, DacDev);
    
    return Ret;
}


