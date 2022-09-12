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
 * @file       Mo1000.c
 * @brief      MO1000 module related functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MO1000
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "Mo1000.h"
#include "Mo1000DevCom.h"
#include "DevCom.h"

#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"

#include "FMCEEprom_eeprom_id.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

 // Version of MO1000 driver
#define MO1000_DVERSION 0x001b

// Uncomment following line to add tests functions
#define MO1000_TEST 1

#ifdef DEBUG
// Uncomment this line to add some trace debug output strings
#define MO1000_MODDEBUG 1
#endif

// Debug related definitions
#ifdef MO1000_MODDEBUG
    // These macros are used to get printout info while debugging
    #define FUNC_ENTER()      printf("MO1000: enter %s\n", __FUNCTION__)
    #define FUNC_EXIT()       printf("MO1000: exit  %s\n", __FUNCTION__)
    #define PDEBUG(...) printf(__VA_ARGS__)

#else

    #define FUNC_ENTER()
    #define FUNC_EXIT()
    #define PDEBUG(...)

#endif

// This identifies the MO1000 fpga pCore
#define MO1000_COREID 0xDAC0

#define MO1000_SMALL_DELAY (1)                      ///< Smallest possible delay
#define MO1000_DACMCMCLOCKRDY_DELAY (100 * 2)       ///< from FPGA m_uMmcmRst 1->0 to m_uMmcmLocked = 0->1, Virtex-6-1 (max 100usec)

// Local osc stabilization time specified by manufacturer, need to measure approx.
// Startup is approx ?? (measured)
#define MO1000_LOCALOSC_DELAY (10000)               ///< Delay to stabilize the 125 MHz local oscillator after powerup

// Local osc stabilization time specified by manufacturer, need to measure approx.
// Startup is approx ?? (measured)
#define MO1000_FREQMEASURE_DELAY (10000)            ///< Delay to measure the core frequencies

// Defines the needed delay for the mo1000 trig signal (to the record core) so
// that the dac captured data match the position of the trigger of other boards (like mi125). The prog delay
// is programmed value + 1
#define MO1000_TRIG_DAC_SYNC_DELAY 6

// Not Specified by manuf, use at least 1 mSec here
#define MO1000_BRIDGESTART_DELAY (500000)           ///< Worst case delay before the spi bridge is becoming ready for used after reset

// This is the minimum reset delay time
#define MO1000_RESET_DELAY (1000)

// This is the synchronisation timeout for slowliest dac clock (50 MHz)
// tsync max = (11 + avg) X 128 X t DacClk_min = 0.356 msec (avec avg =128 et t DacClk_min = 1/50 MHz)
#define MO1000_SYNC_TIMEOUT (356)

// 4 times tsync max
#define MO1000_4SYNC_TIMEOUT (MO1000_SYNC_TIMEOUT * 4)

#define MO1000_REV_NOT_FOUND -1

// Number of fifo thermometer that must be stable at synchronisation
#define MO1000_NFIFO_STABLE 5

// Number of fifo thermometer stable trails at synchronisation (must be minimum > (MO1000_NFIFO_STABLE * MO1000_DAC_FIFO_SIZE))
// We also choose the value to help 1 bin greater than the other (the value is not a multiple of no matter how many bin it is splitted) 
#define MO1000_NFIFO_TRIALS 53

// This is pattern compare validation time (could be set as long as needed to get all pattern errors)
#define MO1000_PATTERN_DELAY (1000)

// Pll sync time is related to ref frequency period added with possible 6usec
#define MO1000_PLLSYNC_DELAY (1000)             ///< Delay for pll synchronisation

// Internal source clock frequency
#define MO1000_INT_FREQUENCY 125000000

// Default output frequency
#define MO1000_INT_DEFAULTOUT 50000000

// Max dac operating frequency according to interpolation mode
#define MO1000_MAX_DAC_FREQ_1X 300000000
#define MO1000_MAX_DAC_FREQ_2X 600000000
#define MO1000_MAX_DAC_FREQ_4X 1000000000
#define MO1000_MAX_DAC_FREQ_8X 1000000000
#define MO1000_MAX_MASTER_CLK_FREQ 1178000000

// Minimum fpga dvi data rate * 2 (50 MHz data rate minimum supported)
#define MO1000_MIN_FPGA_DVI_FREQ 100000000



/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/

// Allows to power down some data channels when inactive
static const unsigned char Mo1000_aucValid[4]={0xFC, 0xF0, 0xC0, 0x00};

// Warning, all revision strings must be same size
static const char *Mo1000_apcRev[] = {"HREV:B", "HREV:C"};

// Dac fifo thermometer conversion table
static const unsigned char Mo1000_aucTherm[]={0, 0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f};

// Dac data fmc lanes position in pausLaneDelays delay table in order of data bits from D0 to D15
static const unsigned char Mo1000_aucDataFmc[32]={2, 3, 4, 6, 5, 7, 8, 10, 9, 12, 11, 13, 14, 16, 15, 17,
                                                  20, 18, 19, 23, 22, 21, 27, 26, 25, 24, 29, 28, 31, 30, 33, 32};

// dac fmc dci position in pausLaneDelays delay table 
static const unsigned char Mo1000_aucDciFmc = 0;

// dac fmc frame position in pausLaneDelays delay table 
static const unsigned char Mo1000_aucFrameFmc = 1;


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/
static int _Mo1000_UCCmp(const void *ElemA, const void *ElemB);
static int _Mo1000_USCmp(const void *ElemA, const void *ElemB);
static int _Mo1000_iSign(int iElement);
static Result _Mo1000_GetDelayAnalysis(unsigned short *pausLaneDelays,
                                      unsigned short *pusMin, unsigned short *pusMax,
                                      unsigned uFindRef, unsigned char *pucRef);
static Result _Mo1000_CalculateSignalPaths(unsigned short *pausLaneDelays, 
                                         char *pucLaneOffset, unsigned short usRefDelay,
                                         Mo1000_eIoDelayRef Reference, char cDciTapOffset);
static Result _Mo1000_DoFifoReadSync(Mo1000_stMo1000 * pBoard, Mo1000DevCom_eDevices DacDev);
static Result _Mo1000_DoFifoWriteThermStats(Mo1000_stMo1000 * pBoard, Mo1000DevCom_eDevices DacDev,
                                            Ad9148_eFifoOffset FifoOffset, unsigned short *usFifoTherm,
                                            int *piFifoPtr, unsigned char *pucFifoThermOff);
static Result _Mo1000_UpdateAllTapDelays(Mo1000_stMo1000 * pBoard);
static Result _Mo1000_DoFifoWriteSync(Mo1000_stMo1000 * pBoard, Mo1000DevCom_eDevices DacDev, unsigned uRelax);
static Result _Mo1000_DoFifoSync(Mo1000_stMo1000 * pBoard, Mo1000DevCom_eDevices DacDev, unsigned uKeepFrame, unsigned uRelax);
static Result _Mo1000_DoSedSetup(Mo1000_stMo1000 * pBoard, unsigned short usP0, unsigned short usP1,
                                 unsigned short usP2, unsigned short usP3, unsigned short usCoreP0,
                                 unsigned short usCoreP1, unsigned short usCoreP2, unsigned short usCoreP3,
                                 unsigned short usDac1Enable, unsigned short usDac2Enable, int iSwap);
static Result _Mo1000_DoScanDataTaps(Mo1000_stMo1000 * pBoard, int iSwap, unsigned uDataToCalib, unsigned *puErrors,
                                     unsigned char ucTapStart, unsigned char ucTapEnd, unsigned uRetry,
                                     unsigned uFlagCheck);
static Result _Mo1000_DoDataCalib(Mo1000_stMo1000 * pBoard);
static Result _Mo1000_DoScanFrameTaps(Mo1000_stMo1000 * pBoard, unsigned uDataToUse, unsigned *puErrors,
                                      unsigned char ucTapStart, unsigned char ucTapEnd, unsigned uRetry,
                                      unsigned uFlagCheck);
static Result _Mo1000_DoFrameCalib(Mo1000_stMo1000 * pBoard);
static Result _Mo1000_DoFrameScanCheck(Mo1000_stMo1000 * pBoard, unsigned *puMin, unsigned *puMax);
static Result _Mo1000_SetClkSource(Mo1000_stMo1000 * pBoard, Expander_eClkSrc ClkSrc,
                                   Expander_eClkRef Reference);
static Result _Mo1000_ClkReset(Mo1000_stMo1000 * pBoard);
static Result _Mo1000_SetupTestPll(Mo1000_stMo1000 * pBoard, int iRef);


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : _Mo1000_UCCmp
 ************************************************************************//**
 *
 * Comparison function for bsearch.
 *
 * @param [in] ElemA
 *    element A to be compared
 *
 * @param [in] ElemB
 *    element B to be compared
 *
 * @return   ==0 if equal.
 *           !=0 if not equal. 
 * 
 ***************************************************************************/ 
static int _Mo1000_UCCmp(const void *ElemA, const void *ElemB)
{
    return ( (*(unsigned char*)ElemA) - (*(unsigned char*)ElemB));
}


 /****************************************************************************
 *   NAME : _Mo1000_USCmp
 ************************************************************************//**
 *
 * Comparison function for qsort.
 *
 * @param [in] ElemA
 *    element A to be compared
 *
 * @param [in] ElemB
 *    element B to be compared
 *
 * @return   ==0 if equal.
 *           !=0 if not equal. 
 * 
 ***************************************************************************/ 
static int _Mo1000_USCmp(const void *ElemA, const void *ElemB)
{
    return ( (*(unsigned short*)ElemA) - (*(unsigned short*)ElemB));
}


 /****************************************************************************
 *   NAME : _Mo1000_iSign
 ************************************************************************//**
 *
 * Get the sign of an integer.
 *
 * @param [in] iElement
 *    element to get the sign
 *
 * @return   ==0 if value is 0.
 *           ==1 if value is positive. 
 *           ==-1 if value is negative. 
 * 
 ***************************************************************************/ 
static int _Mo1000_iSign(int iElement)
{
    return ( (iElement > 0) - (iElement < 0));
}


 /****************************************************************************
 * NAME : _Mo1000_GetDelayAnalysis
 ************************************************************************//**
 *
 * Scan the provided delay table to find the minimum and maximum value. It
 * will also find the reference data lane in the delay (the data lane
 * nearest to the mean value between min & max delays) is requiered. 
 *
 * @warning
 *
 * @param [in] pausLaneDelays
 *    This is the complete vector of each fmc lane delays (LAxx, HAxx, HBxx).
 *
 * @param [out] pusMin
 *    Minimum delay found for all data lanes.
 *
 * @param [out] pusMax
 *    Maximum delay found for all data lanes.
 *
 * @param [in] uFindRef
 *    When !=0, will attempt to find the reference data lane (the one with the
 *    delay nearest to the mean delay (of min / max)).
 *
 * @param [out] pucRef
 *    Return the reference data lane bit position (the one with the
 *    delay nearest to the mean delay (of min / max)) when uFindRef != 0.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open().
 *
 * @see
 *
 ***************************************************************************/ 
static Result _Mo1000_GetDelayAnalysis(unsigned short *pausLaneDelays,
                                       unsigned short *pusMin, unsigned short *pusMax,
                                       unsigned uFindRef, unsigned char *pucRef)
{
    Result Ret = eMo1000Ok;
    unsigned short usCount, usValue;
    unsigned short pausDataDelays[32][2];
    unsigned uMiddle;
    int iCurrDiff,iLastDiff;
    
    FUNC_ENTER();

    // Initial values
    *pusMin = pausLaneDelays[Mo1000_aucDataFmc[0]];
    *pusMax = pausLaneDelays[Mo1000_aucDataFmc[0]];
    pausDataDelays[0][0] = *pusMin;
    pausDataDelays[0][1] = 0;
    *pucRef = 255;
    
    // Scan for each data bit
    for (usCount= 1; usCount < 32; usCount++)
    {
        usValue = pausLaneDelays[Mo1000_aucDataFmc[usCount]];
        
        // Saves data delays associated with this bit position
        pausDataDelays[usCount][0] = usValue;
        pausDataDelays[usCount][1] = usCount;
        
        // Update max value so far
        if (usValue > *pusMax)
            *pusMax = usValue;

        // Update max value so far
        if (usValue < *pusMin)
            *pusMin = usValue;
    }        
    
    // Do we need to find the best reference data lane (the one located nearer the mean of min & max)?
    if (uFindRef)
    {
        // Calculates middle delay value
        uMiddle = ((unsigned )(*pusMax) + *pusMin) / 2;
        
        // Sorts the data delays table
        qsort(&pausDataDelays[0][0], 32, sizeof(pausDataDelays[0]), _Mo1000_USCmp);

        iCurrDiff = uMiddle - pausDataDelays[0][0];
        
        // Scan for each data bit and search for the delay value nearest to the mean one between the lanes
        for (usCount= 0; usCount < 32; usCount++)
        {
            // Do we find the exact delay value?
            if (pausDataDelays[usCount][0] == uMiddle)
            {
                // Get the related data bit position as the reference
                *pucRef = pausDataDelays[usCount][1];
                break;                
            }
            else
            {
                iLastDiff = iCurrDiff;
                
                // Calculates the difference
                iCurrDiff = (int)(uMiddle) - pausDataDelays[usCount][0];
                
                // Did we get a change of sign between current and last value?
                if (_Mo1000_iSign(iCurrDiff) != _Mo1000_iSign(iLastDiff))
                {
                    // Yes, which one is the absolute greater difference value? 
                    if (abs(iCurrDiff) > abs(iLastDiff))
                        *pucRef = pausDataDelays[usCount - 1][1];
                    else
                        *pucRef = pausDataDelays[usCount][1];
                }
            }
        }
        
        // Did we find a reference
        if (*pucRef == 255)
        {
           // Uses data bit 0 as a reference...
           *pucRef = 0;
        }
    }
    
    FUNC_EXIT();
    
    return(Ret);
}


 /****************************************************************************
 * NAME : _Mo1000_CalculateSignalPaths
 ************************************************************************//**
 *
 * This function will calculate a correction in fpga delay taps for each data lane
 * according to reference lane delay value specified.
 *
 * @warning
 *
 * @param [in] pausLaneDelays
 *    This is the complete vector of each fmc lane delays (LAxx, HAxx, HBxx).
 *
 * @param [out] pucLaneOffset
 *    This will return the calculated delay correction in fpga taps for each data lane.
 *
 * @param [in] usRefDelay
 *    This is the reference data line delay in ps.
 *
 * @param [in] Reference
 *    This is the fpga reference clock used.
 *
 * @param [in] cDciTapOffset
 *    This is a global dci tap offset to use in the calculation.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open().
 *
 * @see
 *
 ***************************************************************************/ 
static Result _Mo1000_CalculateSignalPaths(unsigned short *pausLaneDelays, 
                                         char *pucLaneOffset, unsigned short usRefDelay,
                                         Mo1000_eIoDelayRef Reference, char cDciTapOffset)
{
    Result Ret = eMo1000Ok;
    unsigned short usCount, usValue;
    int iCurrDiff;
    
    FUNC_ENTER();

    // Scan for each data bit
    for (usCount= 0; usCount < 32; usCount++)
    {
        // Get the lane delay for this data bit
        usValue = pausLaneDelays[Mo1000_aucDataFmc[usCount]];
        
        // Calculates the delay delta from the reference lane in fpga taps
        iCurrDiff = ((int)(usRefDelay) - (int)(usValue)) / (int)(Reference) + (int)(cDciTapOffset);
        
        // Checks for difference overflow
        if (iCurrDiff > 127)
            iCurrDiff = 127;
            
        if (iCurrDiff < -128)
            iCurrDiff = -128;
            
        pucLaneOffset[usCount] = (char)(iCurrDiff);
    }        
    
    FUNC_EXIT();
    
    return(Ret);
}


 /****************************************************************************
 * NAME : _Mo1000_DoFifoReadSync
 ************************************************************************//**
 *
 * Execute a fifo read synchronisation procedure (fifo rate) for the specified
 * dac device.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.    
 * - Before attempting this synchronisation, the dac must be initialized
 *   with stable clocks, interpolation mode must be set and dac outputs
 *   should be disabled.
 * - Fpga frame mode must be set to 2 before runnning this function.        
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init().
 *
 * @see
 *    Mo1000_SetClockConfig(), Mo1000_SetDacParInterpolation(), Mo1000_SetDacOutCtrl().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoFifoReadSync(Mo1000_stMo1000 * pBoard, Mo1000DevCom_eDevices DacDev)
{
    Result Ret;
    Ad9148_stDacEvent Event, ReadEvent;
    unsigned uTimeOut;
    
    FUNC_ENTER();

    // Clean sync register
    Ret = Ad9148_WriteReg(&pBoard->Dac, DacDev, eAd9148Addr10, 0);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Set sync averaging to 128
    Ret = Ad9148_SetSyncAvg(&pBoard->Dac, DacDev, eAd9148SyncAveraging128);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Set fifo rate sync mode
    Ret = Ad9148_SetSyncMode(&pBoard->Dac, DacDev, eAd9148SyncFifoRate);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Set falling edge synchronisation
    Ret = Ad9148_SetSyncEdge(&pBoard->Dac, DacDev, eAd9148SyncFallingEdge);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Disable sync lost irq
    Event.m_uRaw = 0;
    Event.stBit.m_uSyncLockLost = 1;
    Ret = Ad9148_ResetIrqEvent(&pBoard->Dac, DacDev, Event);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Clears any pending sync lost irq event
    Ret = Ad9148_ClearIrqEvent(&pBoard->Dac, DacDev, Event);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Clean sync status register (as a safety measure)
    Ret = Ad9148_WriteReg(&pBoard->Dac, DacDev, eAd9148Addr12, 0x80);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Enable sync lost irq
    Ret = Ad9148_SetIrqEvent(&pBoard->Dac, DacDev, Event);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Enable synchronisation mode
    Ret = Ad9148_SetSyncCtrl(&pBoard->Dac, DacDev, eAd9148SyncEnable);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait for sync until timeout
    for (uTimeOut = MO1000_4SYNC_TIMEOUT; uTimeOut > 0; uTimeOut --)
    {
        // Get sync event
        Ret = Ad9148_GetIrqEvents(&pBoard->Dac, DacDev, &ReadEvent);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Do we have a synchronisation?
        if (ReadEvent.stBit.m_uSyncLock == 1)
            break;

        // Wait small delay
        Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
    }
    
    // Did we get a timeout?
    if (uTimeOut == 0)
    {
        Ret = eMo1000NoReadSync;
    }
    else
    {
        // Makes sure sync is stable
        for (uTimeOut = MO1000_SYNC_TIMEOUT; uTimeOut > 0; uTimeOut --)
        {
            // Get sync lost event
            Ret = Ad9148_GetIrqEvents(&pBoard->Dac, DacDev, &ReadEvent);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
            // Did we get synchronisation lost event?
            if (ReadEvent.stBit.m_uSyncLockLost == 1)
                break;
    
            // Wait small delay
            Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
        }
        // Did we get the sync lost error?
        if (ReadEvent.stBit.m_uSyncLockLost == 1)
        {
            Ret = eMo1000ReadSyncLost;
        }
        else
        {
            Ret = eMo1000Ok;
        }
    }
    
    FUNC_EXIT();
    
    return(Ret);
}


 /****************************************************************************
 * NAME : _Mo1000_DoFifoWriteThermStats
 ************************************************************************//**
 *
 * Find the dac fifo write pointer greatest occurence value.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.    
 * - Before attempting this synchronisation, the dac must be initialized
 *   with stable clocks, interpolation mode must be set and dac outputs
 *   should be disabled.
 * - Fpga frame mode must be set to 2 before runnning this function.
 * - For measurement purpose, the dac write fifo offset correction could be
 *   changed to a different value than expected (it would need to be
 *   setback later to desired value).          
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] FifoOffset
 *    Current write pointer offset correction requiered
 *
 * @param [out] usFifoTherm
 *    Fifo write pointer statistics buffer
 *
 * @param [out] piFifoPtr
 *    Fifo write pointer value with greatest occurence
 *
 * @param [out] pucFifoThermOff
 *    Fifo write pointer correction used for statistics measurements
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init(), _Mo1000_DoFifoReadSync().
 *
 * @see
 *    Mo1000_SetClockConfig(), Mo1000_SetDacParInterpolation(), Mo1000_SetDacOutCtrl().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoFifoWriteThermStats(Mo1000_stMo1000 * pBoard, Mo1000DevCom_eDevices DacDev,
                                            Ad9148_eFifoOffset FifoOffset, unsigned short *usFifoTherm,
                                            int *piFifoPtr, unsigned char *pucFifoThermOff)
{
    Result Ret;
    unsigned uTrials, uStable, uAttempt;
    unsigned char aucLevel[MO1000_NFIFO_TRIALS];
    Ad9148_eFifoOffset LastFifoOffset, FirstFifoOffset = FifoOffset;
    unsigned int *puFound = 0, uIndex;
    unsigned short usIndexSol, usMax;
    
    FUNC_ENTER();

    *piFifoPtr = 0;
    *pucFifoThermOff = 0;
    
    // Erase stats
    memset(usFifoTherm, 0, MO1000_DAC_FIFO_SIZE * sizeof(unsigned short));
    
    // This algo deal with the case were the fifos difference is near 0 and the thermometer value could be wrong (unusable) in this case
    for (uAttempt = 0, uStable = 0; uAttempt < 2 && uStable == 0; uAttempt++, FifoOffset = (FifoOffset + 2) % 8)
    {
        // Save current offset
        LastFifoOffset = FifoOffset;
        
        // Set sync fifo offset to start offset
        Ret = Ad9148_SetSyncOffset(&pBoard->Dac, DacDev, FifoOffset);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Wait small delay
        Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
    
        // Cumul some stats about fifo thermometer values
        for (uTrials = 0; uTrials < MO1000_NFIFO_TRIALS; uTrials++)
        {
            // Read fifo thermometer
            Ret = Ad9148_GetFifoLvl(&pBoard->Dac, DacDev, &aucLevel[uTrials]);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
            // Is the thermometer value can be converted to write fifo pointer position?
            if ((puFound = (unsigned *)bsearch(&aucLevel[uTrials], Mo1000_aucTherm,
                 sizeof(Mo1000_aucTherm)/sizeof(Mo1000_aucTherm[0]), sizeof(Mo1000_aucTherm[0]),
                 _Mo1000_UCCmp)) != NULL)
            {
                // Get pointer position index of the matching thermometer
                usIndexSol =  (((unsigned)(puFound)) - ((unsigned)(&Mo1000_aucTherm[0]))) / sizeof(Mo1000_aucTherm[0]);
                
                // Add 1 to the occurence of this pointer value in statistics
                usFifoTherm[usIndexSol] = usFifoTherm[usIndexSol] + 1;
                
                // We found at least one valid thermometer value
                uStable = 1;
            }
        }
    }
    
    // Did we get any correct thermometer value
    if (!uStable)
    {
        // Invalid fifo thermometer value error (the thermometer value was always wrong)
        Ret = eMo1000FifoTherWrong;

        FUNC_EXIT();
        
        return(Ret);
    }
    
    // Get the fifo thermometer element with greatest occurence
    for (uTrials = 1, usMax = usFifoTherm[0], uIndex = 0; uTrials < MO1000_DAC_FIFO_SIZE; uTrials++)
    {
        // Greater ocurence so far?
        if (usFifoTherm[uTrials] > usMax)
        {
            usMax = usFifoTherm[uTrials];
            uIndex = uTrials;
        }
    }
    
    // Check the greatest occurence stability
    if (usMax < MO1000_NFIFO_STABLE)
    {
        // Unstable fifo thermometer value error
        Ret = eMo1000FifoTher;

        FUNC_EXIT();
        
        return(Ret);
    }

    PDEBUG("_Mo1000_DoFifoWriteThermStats curr Fifo ptr %d, occurence: %d.\n",uIndex, usMax);
    PDEBUG("_Mo1000_DoFifoWriteThermStats offset correction %d, attempts: %d.\n",LastFifoOffset,uAttempt);

    // Remove current offset and adjust to be positive (between 0 and 7)
    *piFifoPtr = uIndex - (LastFifoOffset - FirstFifoOffset);
    if (*piFifoPtr < 0)
        *piFifoPtr += 8;
    *piFifoPtr = *piFifoPtr % 8;            
    
    // Fifo write pointer correction Offset used for the statistics
    *pucFifoThermOff = LastFifoOffset;
        
    PDEBUG("_Mo1000_DoFifoWriteThermStats real Fifo ptr %d.\n",*piFifoPtr);

    FUNC_EXIT();
    
    return(Ret);
}


 /****************************************************************************
 * NAME : _Mo1000_UpdateAllTapDelays
 ************************************************************************//**
 *
 * Update all data lanes, frame and dci tap delays according to specified offset.
 * When the offset is 0, it will just update the currents tap delays. The offset
 * is provided through member structure cTapsDelayOffset.  
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 * - There is no validation in this function to check if the offset applied
 *   to a tap delay will make it overflow the possible hardware tap delay range [0,31].
 * - All current taps delay are read from the Mo1000_stMo1000 structure and
 *   must be initialized before calling this function.
 * - Mo1000_stMo1000 structure member cTapsDelayOffset must be initialized before
 *   calling this function.         
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *    Member cTapsDelayOffset is used as an input to apply a global tap offset on all
 *    taps before updating.  
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init(), _Mo1000_DoDataCalib(),
 *    _Mo1000_DoFrameCalib().
 *
 * @see
 *
 ***************************************************************************/ 
static Result _Mo1000_UpdateAllTapDelays(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;
    unsigned uCount;
    int iBit;
    
    FUNC_ENTER();

    // Update all data taps delay
    for (uCount = 0; uCount < 32; uCount++)
    {
        // Generate a bit match for the bit position
        iBit = 1 << uCount;
        
        // Used original calibrated delay corrected with an offset
        pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = pBoard->ucDataIoDelTable[uCount] + pBoard->cTapsDelayOffset;

        // Apply to the current lane
        pBoard->pCore->stDelayData.stBit.m_uDelayWeData = iBit;
    }
    
    // Update frame delay
    // Used original calibrated delay corrected with an offset
    pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = pBoard->ucFrameIoDelay + pBoard->cTapsDelayOffset;
    pBoard->pCore->stDelayCtrl.stBit.m_uDelayWeFrame = 1;

    // Update dci delay
    // Used original calibrated delay corrected with an offset
    pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = pBoard->ucDciIoDelay + pBoard->cTapsDelayOffset;
    pBoard->pCore->stDelayCtrl.stBit.m_uDelayWeDci = 1;

    FUNC_EXIT();
    
    return(Ret);
}


 /****************************************************************************
 * NAME : _Mo1000_DoFifoWriteSync
 ************************************************************************//**
 *
 * Execute fifo write synchronisation procedure for the specified
 * dac device.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.    
 * - Before attempting this synchronisation, the dac must be initialized
 *   with stable clocks, interpolation mode must be set and dac outputs
 *   should be disabled.
 * - Fpga frame mode must be set to 2 before runnning this function.        
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] uRelax
 *    When !=0, relax write pointer correction requierement (correction >=3 to <=5 will be allowed)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init(), _Mo1000_DoFifoReadSync().
 *
 * @see
 *    Mo1000_SetClockConfig(), Mo1000_SetDacParInterpolation(), Mo1000_SetDacOutCtrl().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoFifoWriteSync(Mo1000_stMo1000 * pBoard, Mo1000DevCom_eDevices DacDev, unsigned uRelax)
{
    Result Ret;
    Ad9148_stDacEvent Event, ReadEvent;
    unsigned uTimeOut;
    int iConv;
    unsigned char ucValue = 0;
    Ad9148_eFifoOffset FifoOffset = eAd9148FifoOffset0;
    
    FUNC_ENTER();

    {
        // Evaluates current write fifo pointer value
        Ret = _Mo1000_DoFifoWriteThermStats(pBoard, DacDev, FifoOffset, pBoard->usFifoTherm[DacDev], &iConv, &pBoard->ucFifoThermOff[DacDev]);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
        // Correction and validation steps
        // According to fifo level, do
        if (iConv <= 4)
        {
            ucValue = 4 - iConv;
        }
        else
        {
            ucValue = 12 - iConv;
        }
        // Set sync fifo offset to calculated value so read/write fifo difference is around 4
        // Evaluates corrected write fifo pointer value
        Ret = _Mo1000_DoFifoWriteThermStats(pBoard, DacDev, ucValue, pBoard->usFifoThermV[DacDev], &iConv, &pBoard->ucFifoThermVOff[DacDev]);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // If the desired correction has been changed for measurement
        if (ucValue != pBoard->ucFifoThermVOff[DacDev])
        {
            // Set back sync fifo offset to calculated value so read/write fifo difference is near 4
            Ret = Ad9148_SetSyncOffset(&pBoard->Dac, DacDev, ucValue);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            // Wait small delay
            Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
        }
        
        PDEBUG("WriteSync verified Ther %d.\n",iConv);

        // Are we using relaxed correction requierement (in a last resort to sync)
        if (uRelax)
        {
            PDEBUG("WriteSync relax validation.\n");
            
            // Check for expected stable value
            if (iConv > 5 || iConv < 3)
            {
                // Invalid fifo thermometer value error
                Ret = eMo1000FifoTherNotSet;
    
                FUNC_EXIT();
                
                return(Ret);
            }
        }
        else
        {
            // Check for expected stable value
            if (iConv != 4)
            {
                // Invalid fifo thermometer value error
                Ret = eMo1000FifoTherNotSet;
    
                FUNC_EXIT();
                
                return(Ret);
            }
        }
        
        // Disable fifo warnings irq
        Event.m_uRaw = 0;
        Event.stBit.m_uFifoWarn1 = 1;
        Event.stBit.m_uFifoWarn2 = 1;
        Ret = Ad9148_ResetIrqEvent(&pBoard->Dac, DacDev, Event);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Clears any pending fifo warnings irq event
        Ret = Ad9148_ClearIrqEvent(&pBoard->Dac, DacDev, Event);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Enable fifo warnings irq
        Ret = Ad9148_SetIrqEvent(&pBoard->Dac, DacDev, Event);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Makes sure we do not get any fifo warnings
        for (uTimeOut = MO1000_SYNC_TIMEOUT; uTimeOut > 0; uTimeOut --)
        {
            // Get fifo warnings event
            Ret = Ad9148_GetIrqEvents(&pBoard->Dac, DacDev, &ReadEvent);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
            // Did we get any fifo warnings event?
            if (ReadEvent.stBit.m_uFifoWarn1 == 1 || ReadEvent.stBit.m_uFifoWarn2 == 1)
                break;
    
            // Wait small delay
            Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
        }
        
        // Did we get any fifo warning error?
        if (uTimeOut != 0)
        {
            // We got a fifo error, problem with write synchronisation
            Ret = eMo1000FifoWar;
        }
        else
        {
            Ret = eMo1000Ok;
        }
    }
    
    FUNC_EXIT();
    
    return(Ret);
}


 /****************************************************************************
 * NAME : _Mo1000_DoFifoSync
 ************************************************************************//**
 *
 * Execute fifo read/write synchronisation procedure for the specified
 * dac device.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 * - Before attempting this synchronisation, the dac must be initialized
 *   with stable clocks, interpolation mode must be set and dac outputs
 *   should be disabled.       
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] DacDev
 *    Which dac device to do this function (eMo1000DevComDeviceDac1, eMo1000DevComDeviceDac2)
 *
 * @param [in] uKeepFrame
 *    When !=0, will keep the fpga core sync frame mode after execution
 *
 * @param [in] uRelax
 *    When !=0, relax write pointer correction requierement (correction >=3 to <=5 will be allowed)
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init().
 *
 * @see
 *    Mo1000_SetClockConfig(), Mo1000_SetDacParInterpolation(), Mo1000_SetDacOutCtrl().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoFifoSync(Mo1000_stMo1000 * pBoard, Mo1000DevCom_eDevices DacDev, unsigned uKeepFrame, unsigned uRelax)
{
    Result Ret;
    
    FUNC_ENTER();

    // Check if we have the correct frmae mode already
    if (pBoard->pCore->stControl.stBit.m_uFrameMode != 2)
    {
        // Frame mode: pulse on the frame during two DCI clock periods and hold the frame
        // low during the following 14 DCI clock periods (then repeat this sequence)
        pBoard->pCore->stControl.stBit.m_uFrameMode = 2;
    }

    // Execute the fifo read synchronisation
    Ret = _Mo1000_DoFifoReadSync(pBoard, DacDev);

    if(SUCCESS(Ret))
    {
        // Execute the fifo write synchronisation
        Ret = _Mo1000_DoFifoWriteSync(pBoard, DacDev, uRelax);
    }
    
    // Do we keep the frame setting 
    if (!uKeepFrame)
    {
        // Frame mode: toggle the frame value each DCI clock period
        pBoard->pCore->stControl.stBit.m_uFrameMode = 0;
    }

    FUNC_EXIT();
    
    return(Ret);
}


 /****************************************************************************
 * NAME : _Mo1000_DoSedSetup
 ************************************************************************//**
 *
 * Setup the pattern detection mechanism in the core and all dacs.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 * - Before attempting this pattern mode, the dac must be initialized
 *   with stable clocks.       
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] usP0
 *    1st 16 bits dac pattern to compare
 *
 * @param [in] usP1
 *    2nd 16 bits dac pattern to compare
 *
 * @param [in] usP2
 *    3rd 16 bits dac pattern to compare
 *
 * @param [in] usP3
 *    4th 16 bits dac pattern to compare
 *
 * @param [in] usCoreP0
 *    1st 16 bits core pattern to compare
 *
 * @param [in] usCoreP1
 *    2nd 16 bits core pattern to compare
 *
 * @param [in] usCoreP2
 *    3rd 16 bits core pattern to compare
 *
 * @param [in] usCoreP3
 *    4th 16 bits core pattern to compare
 *
 * @param [in] usDac1Enable
 *    When different 0, will enable pattern setup for dac1
 *
 * @param [in] usDac2Enable
 *    When different 0, will enable pattern setup for dac2
 *
 * @param [in] iSwap
 *    When !=0, swap the patterns pair in case edge (frame) is incorect
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init().
 *
 * @see
 *    Mo1000_SetClockConfig().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoSedSetup(Mo1000_stMo1000 * pBoard, unsigned short usP0, unsigned short usP1,
                                 unsigned short usP2, unsigned short usP3, unsigned short usCoreP0,
                                 unsigned short usCoreP1, unsigned short usCoreP2, unsigned short usCoreP3,
                                 unsigned short usDac1Enable, unsigned short usDac2Enable, int iSwap)
{
    Result Ret = eMo1000Ok;

    FUNC_ENTER();

	// Do we have to swap the pattern for the other edge?
    if (iSwap)
    {
        // Any bit to check on dac1 (if yes setup sed)
        if (usDac1Enable)
        {
            // Get sed compare results
            Ret = Ad9148_SetSedPatterns(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SelectPortA, usP1, usP0, usP3, usP2);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    
        // Any bit to check on dac2 (if yes setup sed)
        if (usDac2Enable)
        {
            // Get sed compare results
            Ret = Ad9148_SetSedPatterns(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SelectPortA, usP1, usP0, usP3, usP2);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    }
    else
    {
        // Any bit to check on dac1 (if yes setup sed)
        if (usDac1Enable)
        {
            // Get sed compare results
            Ret = Ad9148_SetSedPatterns(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SelectPortA, usP0, usP1, usP2, usP3);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    
        // Any bit to check on dac2 (if yes setup sed)
        if (usDac2Enable)
        {
            // Get sed compare results
            Ret = Ad9148_SetSedPatterns(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SelectPortA, usP0, usP1, usP2, usP3);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    }
    
	// Setup the core patterns mode
    pBoard->pCore->stPatternAB.stBit.m_uPatternA = usCoreP0;
	pBoard->pCore->stPatternAB.stBit.m_uPatternB = usCoreP1;
	pBoard->pCore->stPatternCD.stBit.m_uPatternC = usCoreP2;
	pBoard->pCore->stPatternCD.stBit.m_uPatternD = usCoreP3;

    FUNC_EXIT();

    return Ret;
}


 /****************************************************************************
 * NAME : _Mo1000_DoScanDataTaps
 ************************************************************************//**
 *
 * Scan specified data delay taps range in the core (one single bit at a time).
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 * - Before attempting this pattern mode, the dac must be initialized
 *   with stable clocks.
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] iSwap
 *    When != 0, swap the patterns pair in case edge (frame) is incorect.
 *
 * @param [in] uDataToCalib
 *    Each bit position set to 1 will force to attempt to scan specified delay taps
 *    range for this specific data lines pair.
 *
 * @param [out] puErrors
 *    This will return the pattern validation for each tap delay available for the
 *    specified enable bits in uDataToCalib. This vector must be of size MO1000_DELAY_TAPS
 *    (not verified in this function) 
 *
 * @param [in] ucTapStart
 *    Tap to start the scan.
 *
 * @param [in] ucTapEnd
 *    Last Tap to be scanned.
 *
 * @param [in] uRetry
 *    Number of time to retry for waiting to read compare results errors.
 *
 * @param [in] uFlagCheck
 *    When !=0, do not reprogram data lane iodelays but only verifies if the data lanes to be calibrated
 *    are stabilized (error free). ucTapStart and ucTapEnd could be set to 0 in this case
 *    because taps does not need to be scanned (result will return in puErrors[0]).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init().
 *
 * @see
 *    Mo1000_SetClockConfig().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoScanDataTaps(Mo1000_stMo1000 * pBoard, int iSwap, unsigned uDataToCalib, unsigned *puErrors,
                                     unsigned char ucTapStart, unsigned char ucTapEnd, unsigned uRetry,
                                     unsigned uFlagCheck)
{
    Result Ret = eMo1000Ok;
    int iTap;
    unsigned short usEvenPattern = 0x5555, usOddPattern = 0xaaaa;
    unsigned uErrors, uRetryCount;
    unsigned short usRisingEdge, usFallingEdge;
    unsigned short usDac1Enable, usDac2Enable;
    
    FUNC_ENTER();

    // Extracts the bit to be calibrated in each dac
    usDac1Enable = (uDataToCalib & 0xffff);
    usDac2Enable = ((uDataToCalib >> 16) & 0xffff);
    
    // Setup core pattern mode and dacs sed logic
    Ret = _Mo1000_DoSedSetup(pBoard, usEvenPattern, usOddPattern, usEvenPattern, usOddPattern,
                             usEvenPattern, usOddPattern, usEvenPattern, usOddPattern,
                             usDac1Enable, usDac2Enable, iSwap);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Enable core pattern mode
    pBoard->pCore->stControl.stBit.m_uDataPatternEn = 1;
        
    // Scan core delay taps in the range specified
    for (iTap = ucTapStart; iTap <= ucTapEnd; iTap++)
    {
        // Clean the errors for the specified delay taps with the bits that needs scan
        puErrors[iTap] &= (~uDataToCalib);
        
        // Are we checking the stability only for the current programmed iodelay tap?
        if (uFlagCheck == 0)
        {
            // No we need to program the iodelay tap
            
            // Setup current data tap delay value
            pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = iTap;
            
            // Change the delay tap for all specified data lanes
            pBoard->pCore->stDelayData.m_uRaw = uDataToCalib;
        }

        // Sed setup auto clear mode/normal sed on each dac device to be scanned
        // Any calibrated bit on dac1 (if yes setup sed)
        if (usDac1Enable)
        {
            // Enable dac sed compare auto clear mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedAutoClearEnable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // Enable dac sed compare normal mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedEnable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    
        // Any calibrated bit on dac2 (if yes setup sed)
        if (usDac2Enable)
        {
            // Enable dac sed compare auto clear mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedAutoClearEnable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // Enable dac sed compare normal mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedEnable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    
        // Wait for the time specifies for the number of retries
        for (uRetryCount = 0, uErrors = 0; uRetryCount < uRetry; uRetryCount++)
        {
            // Wait pattern compare time to cumul errors
            Carrier_usleep(pBoard->hCarrier, MO1000_PATTERN_DELAY);
    
        }

        // Any calibrated bit on dac1 (if yes read results)
        if (usDac1Enable)
        {
            // Get sed compare results
            Ret = Ad9148_GetSedResult(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SelectPortA, &usRisingEdge, &usFallingEdge);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
            // Concatenate each dac errors in the same var
            uErrors |= (usRisingEdge | usFallingEdge);
        }

        // Any calibrated bit on dac2 (if yes read results)
        if (usDac2Enable)
        {
            // Get sed compare results
            Ret = Ad9148_GetSedResult(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SelectPortA, &usRisingEdge, &usFallingEdge);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
            // Concatenate each dac errors in the same var
            uErrors |= ((usRisingEdge | usFallingEdge) << 16);
        }
    
        // Add the errors for the specified delay taps with the bits we are testing
        puErrors[iTap] |= (uDataToCalib & uErrors);

        // Any calibrated bit on dac1 (if yes disable sed)
        if (usDac1Enable)
        {
            // Disable dac sed compare mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedDisable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
        // Any calibrated bit on dac2 (if yes disable sed)
        if (usDac2Enable)
        {
            // Disable dac sed compare mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedDisable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    }

    // Disable core pattern mode
    pBoard->pCore->stControl.stBit.m_uDataPatternEn = 0;
    
    FUNC_EXIT();

    return Ret;
}

 /****************************************************************************
 * NAME : _Mo1000_DoDataCalib
 ************************************************************************//**
 *
 * Execute the data paths calibration by scanning data delay taps
 * and finding the optimum delay taps value for each data lanes pair.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 * - Before attempting data calibration, the dac must be initialized
 *   with stable clocks.
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init().
 *
 * @see
 *    Mo1000_SetClockConfig().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoDataCalib(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;
    //unsigned uDciDelay = 0;
    unsigned int uCalibEn=0xffffffff, uCalibrated=0;
    unsigned uExcursionMax, uTemp, uErrors;
    int iFirstTapOffset=0, iMinBorder, iMaxBorder, iDelayGroupMax, iDelayGroupCnt, iSwap = 0, iCount, iIndex;
    int iSubCount, iBit, iCountStart, iFirstTapOffsetFeed, iActAsMaster;
    unsigned char ucTapStart = 0, ucTapEnd = 31;
    unsigned uRetry = 1;
    unsigned char ucDelayGroupMiddle, ucDist, ucRefCalib;
    unsigned short usMin, usMax;
    
    FUNC_ENTER();

    // Keep trace if data calib was made correctly or not
    pBoard->iDataCalibDone = 0;

    iActAsMaster = pBoard->pCore->stStatus.stBit.m_uClockMaster || pBoard->FmcPosition == eMo1000PositBottom;
    
    // Are we doing as a clock master?
    if (iActAsMaster)
    {
        // For a clock master board, this will attempt to calibrate the reference lane 1st 
        iCountStart = -2;

        // Do we have a board revision different than original revB
        if (pBoard->HardwareRev != eMo1000RevB)
        {
            // Find the reference data lane
            Ret = _Mo1000_GetDelayAnalysis(pBoard->pausLaneDelays, &usMin, &usMax, 1, &pBoard->ucDataRefLane);
            
            // Checks if we got an error?
            if (FAILURE(Ret))
            {
                FUNC_EXIT();
                return(Ret);
            }
    
            // Saves the reference lane delay in ps
            pBoard->usRefDelay = pBoard->pausLaneDelays[Mo1000_aucDataFmc[pBoard->ucDataRefLane]];
    
            // Calculates each lane delay correction to apply (using no dci global correction)
            Ret = _Mo1000_CalculateSignalPaths(pBoard->pausLaneDelays, &pBoard->cLaneOffset[0], pBoard->usRefDelay, pBoard->Reference, 0);
    
            // Checks if we got an error?
            if (FAILURE(Ret))
            {
                FUNC_EXIT();
                return(Ret);
            }
            
            // Get dci delay in ps
            pBoard->usDciDelay = pBoard->pausLaneDelays[Mo1000_aucDciFmc];
        }
        else
        {
            pBoard->usDciDelay = 0;
            
            // Setup calibration reference line (this should be the one with the middle lane delay)
            pBoard->ucDataRefLane = 0;
    
            // Saves the reference lane delay in ps
            pBoard->usRefDelay = 0;

            // By default no correction is used
            memset(&pBoard->cLaneOffset[0], 0, sizeof(pBoard->cLaneOffset));
        }

        // We are trying to calibrate our reference lane in the middle of the possible taps
        iFirstTapOffsetFeed = 16;

        // Default dci delay value set to 0, this allow maximum excursion for data away from dci
        pBoard->ucDciIoDelay = 0;
        
        // Dci is not inverted by default
        pBoard->ucDciInverted = 0;
    }
    else
    {
        // We are a clock slave, our reference lane is coming from other clock master
        iCountStart = 0;

        // We do not use our own reference but bottom one
        pBoard->ucDataRefLane = 255;

        // Checks if the pointer is valid?
        if (pBoard->pucBottomDataRefIoDelay == 0)
        {
            FUNC_EXIT();
            return(eMo1000BadBotTapRefPtr);
        }
        
        // We are getting the value from the master board
        iFirstTapOffsetFeed = *pBoard->pucBottomDataRefIoDelay;
        
        // Checks if bottom reference tap delay value is valid
        if (iFirstTapOffsetFeed > 31 || iFirstTapOffsetFeed < 0)
        {
            FUNC_EXIT();
            return(eMo1000BadBotTapRefValue);
        }

        // Checks if the pointer is valid?
        if (pBoard->pucBottomDciIoDelay == 0)
        {
            FUNC_EXIT();
            return(eMo1000BadBotDciDelPtr);
        }
        
        // Get dci delay from bottom
        pBoard->ucDciIoDelay = *pBoard->pucBottomDciIoDelay;
        
        // Checks if bottom reference tap delay value is valid
        if (pBoard->ucDciIoDelay > 31)
        {
            FUNC_EXIT();
            return(eMo1000BadBotDciDelValue);
        }

        // Checks if the pointer is valid?
        if (pBoard->pucBottomDciInverted == 0)
        {
            FUNC_EXIT();
            return(eMo1000BadBotDciInvPtr);
        }
        
        // Get dci inverted state from bottom
        pBoard->ucDciInverted = *pBoard->pucBottomDciInverted;
        
        // Checks if bottom dci inverted state value is valid
        if (pBoard->ucDciInverted > 1)
        {
            FUNC_EXIT();
            return(eMo1000BadBotDciInvValue);
        }

        // Do we have a board revision different than original revB
        if (pBoard->HardwareRev != eMo1000RevB)
        {
            // Saves the reference lane delay in ps
            pBoard->usRefDelay = 0;

            // Checks if the pointer is valid?
            if (pBoard->pusBottomRefDelay == 0)
            {
                FUNC_EXIT();
                return(eMo1000BadBotRefDelayPtr);
            }
            
            // Checks if the pointer is valid?
            if (pBoard->pusBottomDciDelay == 0)
            {
                FUNC_EXIT();
                return(eMo1000BadBotDciDelayPtr);
            }
            
            // Calculates each lane delay correction to apply
            Ret = _Mo1000_CalculateSignalPaths(pBoard->pausLaneDelays, &pBoard->cLaneOffset[0],
                                               *pBoard->pusBottomRefDelay, pBoard->Reference,
                                               ((int)(*pBoard->pusBottomDciDelay) - (int)(pBoard->pausLaneDelays[Mo1000_aucDciFmc])) / (int)(pBoard->Reference));
    
            // Checks if we got an error?
            if (FAILURE(Ret))
            {
                FUNC_EXIT();
                return(Ret);
            }
        }
        else
        {
            // Saves the reference lane delay in ps
            pBoard->usRefDelay = 0;

            // By default no correction is used
            memset(&pBoard->cLaneOffset[0], 0, sizeof(pBoard->cLaneOffset));

            // Temporary mo1000 calibration data on perseus 601x for revB board
            pBoard->cLaneOffset[31] = -5;
            pBoard->cLaneOffset[30] = -4;
            pBoard->cLaneOffset[29] = -3;
            pBoard->cLaneOffset[28] = -3;
            pBoard->cLaneOffset[27] = -3;
            pBoard->cLaneOffset[26] = -2;
        }
    }
    
    // Makes sure frame mode won't affect data calibration
    pBoard->pCore->stControl.stBit.m_uFrameMode = 1;

    // Frame pattern: 0
    pBoard->pCore->stControl.stBit.m_uFramePattern = 0;
    
    // Setup current dci tap delay value
    pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = pBoard->ucDciIoDelay;
    
    // Change the delay tap for dci
    pBoard->pCore->stDelayCtrl.stBit.m_uDelayWeDci = 1;
    
    // Setup the core with dci inversion
    pBoard->pCore->stControl.stBit.m_uDciInvert = pBoard->ucDciInverted;
    
    // Maximum excursion calculation (40% more than a data clock period)
    // Calculation based on excursion = (1 / (2 * abs(pBoard->iFreqCoreRef4Calculated) * pBoard->Reference * 1e-12 )) * 140% 
    uTemp =  2 * abs(pBoard->iFreqCoreRef4Calculated) / 1000 * pBoard->Reference;
    uExcursionMax = 1400000000 / uTemp;
    if (uExcursionMax * uTemp < 1400000000)
        uExcursionMax++;

    // Default value for tap delay calibration statistics
    pBoard->ucDataSmallestPosDciTol = 0xff;
    pBoard->ucDataSmallestNegDciTol = 0xff;
    pBoard->ucDataSmallestDelay = 0xff;
    pBoard->ucDataGreatestDelay = 0;
    
    // This loop attempts to change dci settings while calibrating
    do
    {
        // This means the reference has not been calibrated
        ucRefCalib = 0;
        
        // This loop attempts to retry calibration without changing anything 
        do
        {
            // Scan all iodelay taps for all data lanes to be calibrated
            Ret = _Mo1000_DoScanDataTaps(pBoard, iSwap, uCalibEn, &pBoard->uDataTapTable0[0], ucTapStart,
                                         ucTapEnd, uRetry, 0);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // Is the reference calibrated
            if (!ucRefCalib)
                iFirstTapOffset = iFirstTapOffsetFeed;
            
            // Find the longest valid delay group and its middle value for all lanes
            // Steps -2 and -1 are needed to correctly calibrates the reference lane
            for(iCount = iCountStart; iCount < 32; iCount++)
            {
                iDelayGroupCnt = 0;
                iDelayGroupMax = 0;
    
                // We do not need to calibrate the reference lane (it was made before)
                if (iCount == pBoard->ucDataRefLane || (iCount < 0 && ucRefCalib != 0))
                    continue;
                    
                // Are we at reference calibration steps?
                if (iCount < 0)
                    // Yes, use the reference lane
                    iIndex = pBoard->ucDataRefLane;
                else
                {
                    // Did we need a reference lane calibrated
                    if (iCountStart < 0 && !ucRefCalib)
                        // Oups we need it so abort analysis for next trial
                        break;

                    // No, use the current lane
                    iIndex = iCount;
                }
                    
                // Generate a bit match for the bit position
                iBit = 1 << iIndex;
                 
                // If calibration is performed on the current lane
                if ((uCalibEn & iBit) == 0)
                    continue;         // Yes already done so skip it
    
                // Calculate minimum/maximum scanning index
                iMinBorder = iFirstTapOffset + pBoard->cLaneOffset[iIndex] - uExcursionMax / 2;
                iMaxBorder = iFirstTapOffset + pBoard->cLaneOffset[iIndex] + uExcursionMax / 2;
    
                // Check minimum index if outside the iodelay range
                if (iMinBorder < 0)
                    iMinBorder = 0;
    
                // Check maximum index if outside the iodelay range
                if (iMaxBorder > 31)
                    iMaxBorder = 31;
    
                // Loop only on subset of taps and the selected taps are offseted depending of the board lane delays
                for(iSubCount = iMinBorder; iSubCount <= iMaxBorder; iSubCount++)
                {
                    // If the current iodelay value for this tap is a valid value
                    if ((pBoard->uDataTapTable0[iSubCount] & iBit) == 0)
                    {
                        // Increment current group counter
                        iDelayGroupCnt++;
                    }
                    else
                    {
                        // Did we found a group?
                        if (iDelayGroupCnt != 0)
                        {
                            // Biggest group found so far?
                            if(iDelayGroupCnt > iDelayGroupMax)
                            {
                                iDelayGroupMax = iDelayGroupCnt;
    
                                // Calculate middle value
                                ucDelayGroupMiddle = iSubCount - 1 - iDelayGroupCnt / 2;
        
                                // Adjust reference lane tap offset according to the middle value find for it
                                if (iCount < 0)
                                {
                                    iFirstTapOffset = ucDelayGroupMiddle;
                                    pBoard->ucDataRefIoDelay = ucDelayGroupMiddle;
                                }
        
                                // Set IDELAY value to current solution
                                pBoard->ucDataIoDelTable[iIndex] = ucDelayGroupMiddle;
                                pBoard->ucDataIoGroupCount[iIndex] = iDelayGroupCnt;
                                pBoard->ucDataIoGroupEnd[iIndex] = (iSubCount - 1);
                                pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = ucDelayGroupMiddle;
        
                                // Apply to the current lane
                                pBoard->pCore->stDelayData.stBit.m_uDelayWeData = iBit;
        
                                // If the valid group is >= 3, remove the lane for the next calibration try
                                // since a valid solution has been found
                                if (iDelayGroupCnt >= 3)
                                {
                                    if (iCount >= 0)
                                    {
                                        // This is any lane but reference calibration
                                        uCalibEn &= ~(iBit);
                                        
                                    }
                                    else
                                    {
                                        // This is reference lane calibration
                                        if (iCount == -1)
                                        {
                                            uCalibEn &= ~(iBit);
                                            ucRefCalib = 1;
                                        }
                                    }
                                }
                            }
    
                            iDelayGroupCnt = 0;
                        }
                    }
                }
                // Did we found a group not processed so far?
                if (iDelayGroupCnt != 0)
                {
                    // Biggest group found so far?
                    if(iDelayGroupCnt > iDelayGroupMax)
                    {
                        iDelayGroupMax = iDelayGroupCnt;
    
                        // Calculate middle value
                        ucDelayGroupMiddle = iSubCount - 1 - iDelayGroupCnt / 2;
    
                        // Adjust reference lane  tap offset according to the middle value find for it
                        if (iCount < 0)
                        {
                            iFirstTapOffset = ucDelayGroupMiddle;
                            pBoard->ucDataRefIoDelay = ucDelayGroupMiddle;
                        }
    
                        // Set IDELAY value to current solution
                        pBoard->ucDataIoDelTable[iIndex] = ucDelayGroupMiddle;
                        pBoard->ucDataIoGroupCount[iIndex] = iDelayGroupCnt;
                        pBoard->ucDataIoGroupEnd[iIndex] = (iSubCount - 1);
                        pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = ucDelayGroupMiddle;
    
                        // Apply to the current lane
                        pBoard->pCore->stDelayData.stBit.m_uDelayWeData = iBit;
    
                        // If the valid group is >= 3, remove the lane for the next calibration try
                        // since a valid solution has been found
                        if (iDelayGroupCnt >= 3)
                        {
                            if (iCount >= 0)
                            {
                                // This is any lane but reference calibration
                                uCalibEn &= ~(iBit);
                                
                            }
                            else
                            {
                                // This is reference lane calibration
                                if (iCount == -1)
                                {
                                    uCalibEn &= ~(iBit);
                                    ucRefCalib = 1;
                                }
                            }
                        }
                    }
                }
            }
    
            // Did we abort the analysis?
            if (iCount >= 32)
            {
                // Verifies if current calibration is ok?
                Ret = _Mo1000_DoScanDataTaps(pBoard, iSwap, 0xffffffff, &uErrors, 0, 0, uRetry, 1);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
                // if there are errors, add lanes in error for the next calibration try
                uCalibEn |= uErrors;
            }
            else
            {
                PDEBUG("Trial #%02d, reference lane uncalibrated.\n",pBoard->uCalibNdx);

                // No lanes calibrated so far because our reference lane is not calibrated
                uCalibEn=0xffffffff;
            }
            
            // Keep trace of the channels lanes that were calibrated
            pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] = uCalibEn;

            // Are all data lanes calibrated?
            if (uCalibEn==0)
            	uCalibrated = 1;
            else
            	uCalibrated = 0;
    
            // Compute which lanes to recalibrate
            if (!uCalibrated)
            {
                PDEBUG("Trial #%02d, to recalibrate: 0x%08X.\n",pBoard->uCalibNdx, uCalibEn);
            }
        // Check if the digital data calibration went correctly
        }while(!uCalibrated && ( (++pBoard->uCalibNdx) < MO1000_MAX_CALIB_ATTEMPT) && (pBoard->uCalibNdx & 3) != 0);

        // Compute which lanes to recalibrate
        if (!uCalibrated)
        {
            // Lanes calibrated so far
            uCalibEn=0xffffffff;
        
            PDEBUG("Trial #%02d, to recalibrate: 0x%08X.\n",pBoard->uCalibNdx, uCalibEn);

            if (( (pBoard->uCalibNdx) < MO1000_MAX_CALIB_ATTEMPT))
            {
                // Are we a master (if yes attempt to change dci setting, if not keep master settings)
                if (iActAsMaster)
                {
                    // Invert the dci for next pass
                    pBoard->ucDciInverted ^= 1;
                    
                    // Setup the core with dci inversion
                    pBoard->pCore->stControl.stBit.m_uDciInvert = pBoard->ucDciInverted;
    
                    // Ok time to try another dci tap delay position
                    if (((pBoard->uCalibNdx) & 1) == 0)
                    {
                        if ( pBoard->ucDciIoDelay != 31)
                            pBoard->ucDciIoDelay = 31;
                        else
                            pBoard->ucDciIoDelay = 0;
                        
                        // Setup current dci tap delay value
                        pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = pBoard->ucDciIoDelay;
                        
                        // Change the delay tap for dci
                        pBoard->pCore->stDelayCtrl.stBit.m_uDelayWeDci = 1;
                    }
                }
                PDEBUG("Next trial: Dci invert %d, Dci delay: %d.\n",pBoard->ucDciInverted, pBoard->ucDciIoDelay);
            }
        }
    }while(!uCalibrated && ( (pBoard->uCalibNdx) < MO1000_MAX_CALIB_ATTEMPT));
    
    if (!uCalibrated)
    {
        // Set the index inside allowed buffer
        pBoard->uCalibNdx--;

        FUNC_EXIT();

        // Problem with data lanes calibration
        return(eMo1000DataTraining);
    }

    // Find the smallest eye positive and negative taps tolerance around calibrated middle value
    for(iCount = 0; iCount < 32; iCount++)
    {
        // Calculates positive tap tolerance
        ucDist = pBoard->ucDataIoGroupEnd[iCount] - pBoard->ucDataIoDelTable[iCount];
        // Update the minimum that we got so far
        if (pBoard->ucDataSmallestPosDciTol > ucDist)
            pBoard->ucDataSmallestPosDciTol = ucDist;

        // Calculates negative tap tolerance
        ucDist = pBoard->ucDataIoDelTable[iCount] - (pBoard->ucDataIoGroupEnd[iCount] + 1 - pBoard->ucDataIoGroupCount[iCount]);
        // Update the minimum that we got so far
        if (pBoard->ucDataSmallestNegDciTol > ucDist)
            pBoard->ucDataSmallestNegDciTol = ucDist;
            
        // Update smallest calibrated value so far
        if (pBoard->ucDataIoDelTable[iCount] < pBoard->ucDataSmallestDelay)
            pBoard->ucDataSmallestDelay = pBoard->ucDataIoDelTable[iCount];

        // Update greatest calibrated value so far
        if (pBoard->ucDataIoDelTable[iCount] > pBoard->ucDataGreatestDelay)
            pBoard->ucDataGreatestDelay = pBoard->ucDataIoDelTable[iCount];
    }

    // Display the number of calibration retries
    PDEBUG("Data calibration trials: %d.\n", pBoard->uCalibNdx);

    // Display the smallest eyes tolerance around their middle calibrated values
    PDEBUG("Data calibration smallest tolerance pos: %d, neg: %d.\n", pBoard->ucDataSmallestPosDciTol, pBoard->ucDataSmallestNegDciTol);

    // Keep trace if data calib was made correctly or not
    pBoard->iDataCalibDone = 1;

    FUNC_EXIT();

    return Ret;
}


 /****************************************************************************
 * NAME : _Mo1000_DoScanFrameTaps
 ************************************************************************//**
 *
 * Scan specified frame delay taps range in the core for the specified dacs.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 * - Before attempting this pattern mode, the dac must be initialized
 *   with stable clocks.
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] uDataToUse
 *    Each half 16 bits word which contains at least 1 bit set to 1 will force a frame calibration
 *    for the associated dac (LSW = dac1, MSW = dac2).
 *
 * @param [out] puErrors
 *    This will return the pattern validation for each tap delay available for the
 *    specified enable dac in uDataToUse. This vector must be of size MO1000_DELAY_TAPS
 *    (not verified in this function) 
 *
 * @param [in] ucTapStart
 *    Tap to start the scan.
 *
 * @param [in] ucTapEnd
 *    Last Tap to be scanned.
 *
 * @param [in] uRetry
 *    Number of time to retry for waiting to read compare results errors.
 *
 * @param [in] uFlagCheck
 *    When !=0, do not reprogram frame lane iodelays but only verifies if the data lanes
 *    are stabilized (error free). ucTapStart and ucTapEnd could be set to 0 in this case
 *    because taps does not need to be scanned (result will return in puErrors[0]).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init().
 *
 * @see
 *    Mo1000_SetClockConfig().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoScanFrameTaps(Mo1000_stMo1000 * pBoard, unsigned uDataToUse, unsigned *puErrors,
                                      unsigned char ucTapStart, unsigned char ucTapEnd, unsigned uRetry,
                                      unsigned uFlagCheck)
{
    Result Ret = eMo1000Ok;
    int iTap;
    unsigned short usPattern0 = 0x5555, usPattern1 = 0x3cc3 , usPattern2 = 0xaaaa, usPattern3 = 0xc33c;
    unsigned uErrors, uRetryCount, uNeededDac;
    unsigned short usRisingEdge, usFallingEdge;
    unsigned short usDac1Enable, usDac2Enable;
    int iSwap = 0;
    
    FUNC_ENTER();

    // Extracts each dac that need frame calibration
    if (uDataToUse & 0xffff)
        usDac1Enable = 0xffff;
    else
        usDac1Enable = 0;
        
    if ((uDataToUse >> 16) & 0xffff)
        usDac2Enable = 0xffff;
    else
        usDac2Enable = 0;
        
    uNeededDac = (unsigned)(usDac1Enable) | ((unsigned)(usDac2Enable) << 16);
    
    // Setup core pattern mode and dacs sed logic
    // Only pattern0 and pattern1 are checked on the dacs, others would need to check
    // also port B which would be slowlier and not needed for frame calib purpose
    Ret = _Mo1000_DoSedSetup(pBoard, usPattern0, usPattern1, usPattern0, usPattern1,
                             usPattern0, usPattern1, usPattern2, usPattern3,
                             usDac1Enable, usDac2Enable, iSwap);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Enable core pattern mode
    pBoard->pCore->stControl.stBit.m_uDataPatternEn = 1;
        
    // Scan core delay taps in the range specified
    for (iTap = ucTapStart; iTap <= ucTapEnd; iTap++)
    {
        // Clean the errors for the specified delay taps with the dac that needs scan
        puErrors[iTap] &= (~uNeededDac);
        
        // Are we checking the stability only for the current programmed iodelay tap?
        if (uFlagCheck == 0)
        {
            // No we need to program the iodelay tap
            
            // Setup current data tap delay value
            pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = iTap;
            
            // Change the delay tap for the frame signal
            pBoard->pCore->stDelayCtrl.stBit.m_uDelayWeFrame = 1;
        }

        // Sed setup auto clear mode/normal sed on each dac device to be scanned
        // Dac1 frame calibration needed (if yes setup sed)
        if (usDac1Enable)
        {
            // Enable dac sed compare auto clear mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedAutoClearEnable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // Enable dac sed compare normal mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedEnable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    
        // Dac2 frame calibration needed (if yes setup sed)
        if (usDac2Enable)
        {
            // Enable dac sed compare auto clear mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedAutoClearEnable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // Enable dac sed compare normal mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedEnable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    
        // Wait for the time specifies for the number of retries
        for (uRetryCount = 0, uErrors = 0; uRetryCount < uRetry; uRetryCount++)
        {
            // Wait pattern compare time to cumul errors
            Carrier_usleep(pBoard->hCarrier, MO1000_PATTERN_DELAY);
    
        }

        // Dac1 frame calibration needed (if yes read results)
        if (usDac1Enable)
        {
            // Get sed compare results
            Ret = Ad9148_GetSedResult(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SelectPortA, &usRisingEdge, &usFallingEdge);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
            // Concatenate each dac errors in the same var
            uErrors |= (usRisingEdge | usFallingEdge);
        }

        // Dac2 frame calibration needed (if yes read results)
        if (usDac2Enable)
        {
            // Get sed compare results
            Ret = Ad9148_GetSedResult(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SelectPortA, &usRisingEdge, &usFallingEdge);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
            // Concatenate each dac errors in the same var
            uErrors |= ((usRisingEdge | usFallingEdge) << 16);
        }
    
        // Add the errors for the specified delay taps with the bits we are testing
        puErrors[iTap] |= (uNeededDac & uErrors);

        // Dac1 frame calibration needed (if yes disable sed)
        if (usDac1Enable)
        {
            // Disable dac sed compare mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedDisable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
        // Dac2 frame calibration needed (if yes disable sed)
        if (usDac2Enable)
        {
            // Disable dac sed compare mode
            Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedDisable);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
    }

    // Disable core pattern mode
    pBoard->pCore->stControl.stBit.m_uDataPatternEn = 0;
    
    FUNC_EXIT();

    return Ret;
}


 /****************************************************************************
 * NAME : _Mo1000_DoFrameCalib
 ************************************************************************//**
 *
 * Execute the frame signal calibration by scanning frame delay taps
 * and finding the optimum delay taps value.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 * - Before attempting frame calibration, the dac must be initialized
 *   with stable clocks and data calibration must be done.
 * - Data lanes calibration must be done before attempting 
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init(), _Mo1000_DoDataCalib.
 *
 * @see
 *    Mo1000_SetClockConfig().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoFrameCalib(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;
    unsigned uFrameMode;
    unsigned int uCalibEn=0xffffffff, uCalibrated=0;
    unsigned uErrors;
    int iMinBorder = 0, iMaxBorder = 31, iDelayGroupMax, iDelayGroupCnt;
    int iSubCount;
    unsigned char ucTapStart = 0, ucTapEnd = 31;
    unsigned uRetry = 1;
    unsigned char ucDelayGroupMiddle;
    
    FUNC_ENTER();

    // Keep trace if frame calib was made correctly or not
    pBoard->iFrameCalibDone = 0;

    // Default value for smallest dci tolerance
    pBoard->ucFrameSmallestPosDciTol = 0xff;
    pBoard->ucFrameSmallestNegDciTol = 0xff;

    // Set current core frame mode to 3 (continous normal sed mode)
    pBoard->pCore->stControl.stBit.m_uFrameMode = 3;
    do
    {
        // For each possible frame pattern mode
        for (uFrameMode = 0; uFrameMode < MO1000_FRAME_MODES; uFrameMode++)
        {
            // Set current core frame pattern mode
            pBoard->pCore->stControl.stBit.m_uFramePattern = uFrameMode;
            
            // Scan all iodelay taps for the frame signal for each dac that need to be calibrated
            Ret = _Mo1000_DoScanFrameTaps(pBoard, uCalibEn, &pBoard->uFrameTapTable[uFrameMode][0], ucTapStart,
                                          ucTapEnd, uRetry, 0);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }

        // Reset group search variables
        iDelayGroupCnt = 0;
        iDelayGroupMax = 0;

        // Find the longest valid delay group and its middle value for all pattern mode
        for (uFrameMode = 0; uFrameMode < MO1000_FRAME_MODES; uFrameMode++)
        {
            // Loop only on subset of taps
            for(iSubCount = iMinBorder; iSubCount <= iMaxBorder; iSubCount++)
            {
                // If the current iodelay value for this tap is a valid value
                if ((pBoard->uFrameTapTable[uFrameMode][iSubCount]) == 0)
                {
                    // Increment current group counter
                    iDelayGroupCnt++;
                }
                else
                {
                    // Did we found a group?
                    if (iDelayGroupCnt != 0)
                    {
                        // Biggest group found so far?
                        if(iDelayGroupCnt > iDelayGroupMax)
                        {
                            iDelayGroupMax = iDelayGroupCnt;

                            // Calculate middle value
                            ucDelayGroupMiddle = iSubCount - 1 - iDelayGroupCnt / 2;
    
                            // Set IDELAY value and pattern to current solution
                            pBoard->ucFramePattern = uFrameMode;
                            pBoard->pCore->stControl.stBit.m_uFramePattern = uFrameMode;
                            pBoard->ucFrameIoDelay = ucDelayGroupMiddle;
                            pBoard->ucFrameGroupCount = iDelayGroupCnt;
                            pBoard->ucFrameGroupEnd = (iSubCount - 1);
                            pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = ucDelayGroupMiddle;
    
                            // Apply to the frame lane
                            pBoard->pCore->stDelayCtrl.stBit.m_uDelayWeFrame = 1;
    
                            // If the valid group is >= 3, remove the next calibration try
                            // since a valid solution has been found
                            if (iDelayGroupCnt >= 3)
                            {
                                uCalibrated = 1;
                            }
                        }
                        iDelayGroupCnt = 0;
                    }
                }
            }
            // Did we found a group not processed so far?
            if (iDelayGroupCnt != 0)
            {
                // Biggest group found so far?
                if(iDelayGroupCnt > iDelayGroupMax)
                {
                    iDelayGroupMax = iDelayGroupCnt;

                    // Calculate middle value
                    ucDelayGroupMiddle = iSubCount - 1 - iDelayGroupCnt / 2;

                    // Set IDELAY value and pattern to current solution
                    pBoard->ucFramePattern = uFrameMode;
                    pBoard->pCore->stControl.stBit.m_uFramePattern = uFrameMode;
                    pBoard->ucFrameIoDelay = ucDelayGroupMiddle;
                    pBoard->ucFrameGroupCount = iDelayGroupCnt;
                    pBoard->ucFrameGroupEnd = (iSubCount - 1);
                    pBoard->pCore->stDelayCtrl.stBit.m_uDelayValue = ucDelayGroupMiddle;

                    // Apply to the frame lane
                    pBoard->pCore->stDelayCtrl.stBit.m_uDelayWeFrame = 1;

                    // If the valid group is >= 3, remove the lane for the next calibration try
                    // since a valid solution has been found
                    if (iDelayGroupCnt >= 3)
                    {
                        uCalibrated = 1;
                    }
                }
                iDelayGroupCnt = 0;
            }
        }

        // If we found a solution
        if (uCalibrated)
        {
            // Verifies if current frame calibration is ok?
    
            // Set current core frame pattern mode
            Ret = _Mo1000_DoScanFrameTaps(pBoard, uCalibEn, &uErrors, 0, 0, uRetry, 1);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // if there are errors, go for the next calibration try
            if (uErrors)
                uCalibrated = 0;
            
            // Keep trace of the errors we got
            pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = uErrors;
    
            // Displays which lanes are in errors
            if (!uCalibrated)
            {
                PDEBUG("Frame Trial #%02d, errors: 0x%08X.\n",pBoard->uCalibFrameNdx, uErrors);
            }
        }
        else
        {
            // Because we did not find any group of minimum length, mean it by setting all bits in errors 
            pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = 0xffffffff;
        }
    // Check if the digital data calibration went correctly
    }while(!uCalibrated && ( (++pBoard->uCalibFrameNdx) < MO1000_MAX_FRAME_CALIB_ATTEMPT));
    
    // Did we calibrate correctly
    if (!uCalibrated)
    {
        // Set the index inside allowed buffer
        pBoard->uCalibFrameNdx--;

        FUNC_EXIT();

        // Problem with frame lane calibration
        return(eMo1000FrameTraining);
    }

    // Calculates positive tap tolerance
    pBoard->ucFrameSmallestPosDciTol = pBoard->ucFrameGroupEnd - pBoard->ucFrameIoDelay;

    // Calculates negative tap tolerance
    pBoard->ucFrameSmallestNegDciTol = pBoard->ucFrameIoDelay - (pBoard->ucFrameGroupEnd + 1 - pBoard->ucFrameGroupCount);

    // Display the number of calibration retries
    PDEBUG("Frame calibration trials: %d.\n", pBoard->uCalibFrameNdx);

    // Display the smallest eyes tolerance around their middle calibrated values
    PDEBUG("Frame calibration tolerance pos: %d, neg: %d.\n", pBoard->ucFrameSmallestPosDciTol, pBoard->ucFrameSmallestNegDciTol);

    // Keep trace if frame calib was made correctly or not
    pBoard->iFrameCalibDone = 1;

    FUNC_EXIT();

    return Ret;
}


 /****************************************************************************
 * NAME : _Mo1000_DoFrameScanCheck
 ************************************************************************//**
 *
 * Execute a frame check by scanning frame delay taps
 * and finding the min & max tap position of the 1st found group without errors
 * for each frame mode. 
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 * - Before attempting this routine, the dac must be initialized
 *   with stable clocks and data calibration must be done.
 * - Data lanes calibration must be done before attempting 
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] puMin
 *    This vector will return the min stable position for each frame mode (must be a vector
 *    of 4 values).
 *
 * @param [in] puMax
 *    This vector will return the max stable position for each frame mode (must be a vector
 *    of 4 values).
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init(), _Mo1000_DoDataCalib.
 *
 * @see
 *    Mo1000_SetClockConfig().
 *
 ***************************************************************************/ 
static Result _Mo1000_DoFrameScanCheck(Mo1000_stMo1000 * pBoard, unsigned *puMin, unsigned *puMax)
{
    Result Ret = eMo1000Ok;
    unsigned uFrameMode;
    unsigned int uCalibEn=0xffffffff;
    int iMinBorder = 0, iMaxBorder = 31, iDelayGroupCnt;
    int iSubCount;
    unsigned char ucTapStart = 0, ucTapEnd = 31;
    unsigned uRetry = 1;
    
    FUNC_ENTER();

    // Keep trace if frame calib was made correctly or not
    pBoard->iFrameCalibDone = 0;

    // Set current core frame mode to 3 (continous normal sed mode)
    pBoard->pCore->stControl.stBit.m_uFrameMode = 3;
    // For each possible frame pattern mode
    for (uFrameMode = 0; uFrameMode < MO1000_FRAME_MODES; uFrameMode++)
    {
        // Set current core frame pattern mode
        pBoard->pCore->stControl.stBit.m_uFramePattern = uFrameMode;
        
        // Scan all iodelay taps for the frame signal for each dac that need to be calibrated
        Ret = _Mo1000_DoScanFrameTaps(pBoard, uCalibEn, &pBoard->uFrameTapTable[uFrameMode][0], ucTapStart,
                                      ucTapEnd, uRetry, 0);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    }

    // Reset group search variables
    iDelayGroupCnt = 0;

    // Find the 1st valid delay group and its min max value for all pattern mode
    for (uFrameMode = 0; uFrameMode < MO1000_FRAME_MODES; uFrameMode++)
    {
        puMin[uFrameMode] = 255;
        puMax[uFrameMode] = 254;

        // Loop only on subset of taps
        for(iSubCount = iMinBorder; iSubCount <= iMaxBorder; iSubCount++)
        {
            // If the current iodelay value for this tap is a valid value
            if ((pBoard->uFrameTapTable[uFrameMode][iSubCount]) == 0)
            {
                // Increment current group counter
                iDelayGroupCnt++;
            }
            else
            {
                // Did we found a group?
                if (iDelayGroupCnt != 0)
                {
                    puMax[uFrameMode] = (iSubCount - 1);
                    puMin[uFrameMode] = (iSubCount - iDelayGroupCnt);
                    iDelayGroupCnt = 0;
                    break;
                }
            }
        }
        // Did we found a group not processed so far?
        if (iDelayGroupCnt != 0)
        {
            puMax[uFrameMode] = (iSubCount - 1);
            puMin[uFrameMode] = (iSubCount - iDelayGroupCnt);
            iDelayGroupCnt = 0;
        }
    }

    FUNC_EXIT();

    return Ret;
}


 /****************************************************************************
 * NAME : _Mo1000_SetClkSource
 ************************************************************************//**
 *
 * Configure the clock source and the clock reference for the pll.
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] ClkSrc
 *    Desired clock source.
 *
 * @param [in] Reference
 *    Desired clock reference.
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init().
 *
 * @see
 *
 ***************************************************************************/ 
static Result _Mo1000_SetClkSource(Mo1000_stMo1000 * pBoard, Expander_eClkSrc ClkSrc,
                                   Expander_eClkRef Reference)
{
    Result Ret;
    Expander_eLocalOscSw Sw;

    FUNC_ENTER();

    // Check if we don't use the internal oscillator and turn it off to lower noise
    if (ClkSrc == eInternalClk)
    {
        Sw = eLocalOscSwOn;
        Ret = Expander_SetLocalOscSw(&pBoard->Expander, Sw);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Make sure the local oscillator has started and stabilized
        Carrier_usleep(pBoard->hCarrier, MO1000_LOCALOSC_DELAY);
    }
    else
    {
        Sw = eLocalOscSwOff;
        Ret = Expander_SetLocalOscSw(&pBoard->Expander, Sw);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    }

    // Configure the clock source
    Ret = Expander_SetClkSource(&pBoard->Expander, ClkSrc);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Configure the clock reference
    Ret = Expander_SetClkReference(&pBoard->Expander, Reference);

    FUNC_EXIT();

    return Ret;
}


 /****************************************************************************
 * NAME : _Mo1000_ClkReset
 ************************************************************************//**
 *
 * Force a MCM clock reset. This function is related to the dac calibration and
 * clock operation. Each time the dac clocks or reference source frequency are
 * changed, this function must be used before doing a dac calibration with
 * Mo1000_DoDacCalibration().
 *
 * @warning
 * - Call Mo1000_Init() before using this function.
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset(), Mo1000_Init().
 *
 * @see
 *    Mo1000_DoDacCalibration(). 
 *
 ***************************************************************************/ 
static Result _Mo1000_ClkReset(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;

    FUNC_ENTER();

    // Keep trace of the number of data calibration trials
    pBoard->uCalibNdx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] = 0xffffffff;

    // Keep trace of the number of frame calibration trials
    pBoard->uCalibFrameNdx = 0;

    // Keep trace of the frame calibration errors
    pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = 0xffffffff;

    // Keep trace of the number of sync calibration trials
    pBoard->uCalibSyncNdx = 0;

    // Keep trace of the sync calibration errors
    pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;

    // Keep trace if calib was made correctly or not
    pBoard->iCalibDone = 0;
    pBoard->iDataCalibDone = 0;
    pBoard->iFrameCalibDone = 0;
    pBoard->iSyncCalibDone = 0;

    // Are we a clock master
    if (pBoard->pCore->stStatus.stBit.m_uClockMaster == 1)
    {
        // Reset the mo1000 core clock MCM
        pBoard->pCore->stControl.stBit.m_uMmcmRst = 1;
    
        // Check m_uMmcmLocked bit in register is getting to 1
        Carrier_usleep(pBoard->hCarrier, MO1000_DACMCMCLOCKRDY_DELAY);
    
        // Check if we got a timeout
        if ((pBoard->pCore->stStatus.stBit.m_uMmcmLocked) == 0)
        {
            FUNC_EXIT();
    
            // Problem with signal not rising after reset
            return(eMo1000MmcmLock);
        }
    }

    FUNC_EXIT();

    return Ret;
}


 /****************************************************************************
 * NAME : _Mo1000_SetupTestPll
 ************************************************************************//**
 *
 * Setup pll with primary reference and output4 set to divide by 1 of primary
 * input. This setup can be used to test the pll, local oscillator switch
 * and local oscillator frequency.
 *
 * @warning
 * - Call Mo1000_Reset() before using this function.
 *
 * @param [in] pBoard
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [in] iRef
 *    Choose primary (0) or secondary (1) reference input for the pll
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 *   
 * @pre
 *    Mo1000_Open(), Mo1000_Reset().
 *
 * @see
 *    Mo1000_TestLocalOscSw(), Mo1000_TestPllSync(). 
 *
 ***************************************************************************/ 
static Result _Mo1000_SetupTestPll(Mo1000_stMo1000 * pBoard, int iRef)
{
    Result Ret = eMo1000Ok;
    Expander_eLocalOscSw Sw;
    unsigned uMRef = 0x000c0e6;
    
    FUNC_ENTER();

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
    // Using secondary input as reference?
    if (iRef)
    {
        uMRef += 4;
    }
    
    // Initialize the io expander to operating condition for all devices
    Ret = Expander_Init(&pBoard->Expander);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait a small bridge get out of reset delay (device ready after reset not specified by manufacturer)
    Carrier_usleep(pBoard->hCarrier, MO1000_BRIDGESTART_DELAY);

    // Initialize the spi bridge (for operation)
    // At this moment we are also sure that all spi cs lines are correctly set to 1
    // We are doing 2 bridge initialization tentatives because to be successfull always on the 1st trial,
    // we need to wait at least around 500 mSec after bridge reset is released which is slow. This technic
    // is faster...
    Ret = Mo1000DevCom_Init(&pBoard->Com);
    if(FAILURE(Ret))
    {
        // Retry to configure the bridge which now must be ready
        Ret = Mo1000DevCom_Init(&pBoard->Com);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    }

    // Set local osc switch on
    Sw = eLocalOscSwOn;
    Ret = Expander_SetLocalOscSw(&pBoard->Expander, Sw);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Make sure the local oscillator has started and stabilized
    Carrier_usleep(pBoard->hCarrier, MO1000_LOCALOSC_DELAY);

    // Configure the clock source
    Ret = Expander_SetClkSource(&pBoard->Expander, eInternalClk);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Remove pll power down
    Ret = Expander_SetPllShutdown(&pBoard->Expander, ePllPowerDownOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait small delay to make sure pll spi is ready for operation after power-on state (eeprom loading)
    Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);

    // Write lockable pll setup
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 0, 0x8104002);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 1, 0x8140002);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 2, 0x8140000);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 3, 0x8140000);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 4, 0xeb84031);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 5, uMRef);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 6, 0x800e036);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 7, 0xbdb9b74);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 8, 0x20009d9);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Cdce62005_WriteReg(&pBoard->Pll, 6, 0x840e036);
    
    FUNC_EXIT();

    return Ret;
}

 /****************************************************************************
 *   NAME : _Mo1000_GetI2cAddr
 ************************************************************************//**
 *
 * Get the address list of all the I2C devices. The device addresses depend
 * on the FMC connector GA and on the type of the MO1000 board.
 *
 * @param [in,out] board
 *    This structure is the one returned at module opening with Mo1000_Open().
 *
 * @param [out] pI2cDevList
 *    Pointer to a structure to return the address list of all the I2C devices
 *
 * @return   Error code: #eMo1000Ok if no problem is found, !#eMo1000Ok if problem
 *           Use SUCCESS(Ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result _Mo1000_GetI2cAddr(Mo1000_stMo1000 * pBoard, Mo1000_stI2cAddr * pI2cDevList)
{
    Result Ret;
    unsigned char ga;
    char FmcInfoType[16];

    Mo1000_stI2cAddr i2cadr_main_ga00 = MO1000_I2CADR_MAIN_GA00;                               
    Mo1000_stI2cAddr i2cadr_sec_ga00  = MO1000_I2CADR_SEC_GA00;                                 
    Mo1000_stI2cAddr i2cadr_main_ga01 = MO1000_I2CADR_MAIN_GA01;                               
    Mo1000_stI2cAddr i2cadr_sec_ga01  = MO1000_I2CADR_SEC_GA01;                                 
    Mo1000_stI2cAddr i2cadr_main_ga10 = MO1000_I2CADR_MAIN_GA10;                               
    Mo1000_stI2cAddr i2cadr_sec_ga10  = MO1000_I2CADR_SEC_GA10;                                 
    Mo1000_stI2cAddr i2cadr_main_ga11 = MO1000_I2CADR_MAIN_GA11;                               
    Mo1000_stI2cAddr i2cadr_sec_ga11  = MO1000_I2CADR_SEC_GA11;                                 

    Ret = Carrier_FmcGetInfo(pBoard->hCarrier, pBoard->FmcConnector,
        (Carrier_eFmcBoardNum_t)pBoard->FmcPosition, CARRIER_INFO_BOARDTYPE,
        FmcInfoType, sizeof(FmcInfoType));
    if(FAILURE(Ret)) {return Ret;}

    Ret = Carrier_FmcGetGA(pBoard->hCarrier, pBoard->FmcConnector, &ga);
    if(FAILURE(Ret)) {return Ret;}

    if(ga==0)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MO1000_E)==0)
            *pI2cDevList = i2cadr_main_ga00;
        else
            *pI2cDevList = i2cadr_sec_ga00;
    }
    else if(ga==1)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MO1000_E)==0)
            *pI2cDevList = i2cadr_main_ga01;
        else
            *pI2cDevList = i2cadr_sec_ga01;
    }
    else if(ga==2)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MO1000_E)==0)
            *pI2cDevList = i2cadr_main_ga10;
        else
            *pI2cDevList = i2cadr_sec_ga10;
    }
    else
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MO1000_E)==0)
            *pI2cDevList = i2cadr_main_ga11;
        else
            *pI2cDevList = i2cadr_sec_ga11;
    }

    return eMo1000Ok;
}

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

Result Mo1000_Open(Mo1000_stMo1000 * pBoard, Mo1000_stRegisterMap * pCore, Carrier_handle_t hCarrier,
                   char *pcHardRevision, Mo1000_eIoDelayRef Reference,
                   unsigned char *pucBottomDataRefIoDelay, unsigned char *pucBottomDciIoDelay,
                   unsigned char *pucBottomDciInverted, unsigned short *pausLaneDelays,
                   unsigned short *pusBottomRefDelay, unsigned short *pusBottomDciDelay)
{
    Result Ret = eMo1000Ok;
    unsigned int FmcPosition;
    unsigned int I2cZone;
    Mo1000_stI2cAddr I2cDevList;
    Mo1000_eHardwareRev Rev;
    int iRevFound;
    unsigned int uFmcBoardNumMax;

    FUNC_ENTER();

    // Erase all board structure by default
    memset(pBoard,0,sizeof(Mo1000_stMo1000));
    
    // Assign carrier handle
    pBoard->hCarrier = hCarrier;

    // Saves a pointer on the bottom reference line lane delay in ps
    pBoard->pusBottomRefDelay = pusBottomRefDelay;
    
    // Saves a pointer on the bottom dci line lane delay in ps
    pBoard->pusBottomDciDelay = pusBottomDciDelay;
    
    // Saves the reference tap delay of bottom board
    pBoard->pucBottomDataRefIoDelay = pucBottomDataRefIoDelay;
    
    // Saves the dci tap delay of bottom board
    pBoard->pucBottomDciIoDelay = pucBottomDciIoDelay;
    
    // Saves the dci inversion state of bottom board
    pBoard->pucBottomDciInverted = pucBottomDciInverted;
    
    // Save the lanes offset delays
    pBoard->pausLaneDelays = pausLaneDelays;

    // Keep trace if global module reset was done or not
    pBoard->iResetDone = 0;

    // This shows that we are not currently in the reset procedure
    pBoard->iResetInProg = 0;

    // Keep trace if module calib was done correctly or not
    pBoard->iCalibDone = 0;
    pBoard->iDataCalibDone = 0;
    pBoard->iFrameCalibDone = 0;
    pBoard->iSyncCalibDone = 0;

    // Keep trace of the number of calibration trials
    pBoard->uCalibNdx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] = 0xffffffff;

    // Keep trace of the number of frame calibration trials
    pBoard->uCalibFrameNdx = 0;

    // Keep trace of the frame calibration errors
    pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = 0xffffffff;

    // Keep trace of the number of sync calibration trials
    pBoard->uCalibSyncNdx = 0;

    // Keep trace of the sync calibration errors
    pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;

    // Clear mask used to check calibration (default all 8 channels lanes)
    pBoard->uChannelLaneMask = 0xffffffff;

    // Keep trace if test mode is active
    pBoard->iTestMode = 0;

    // By default, all 8 channels are used
    pBoard->GroupCh = eMo1000ActChannels08;

    // Assign the board memory map registers
    pBoard->pCore = pCore;
    
    // Make sure fmcposition is possible
    FmcPosition = pBoard->pCore->stStatus.stBit.m_uFmcPositon;

    Ret = Carrier_GetFmcBoardNumMax(hCarrier, &uFmcBoardNumMax);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Save the board position on a specific connector
    pBoard->FmcPosition = (Mo1000_eFmcPosition)(FmcPosition % uFmcBoardNumMax);
    
    // Save the FMC connector number
    pBoard->FmcConnector = (Carrier_eFmcConnectorNum_t)(FmcPosition / uFmcBoardNumMax);

    // Save the core reference clock
    pBoard->Reference = Reference;

    // Check for a valid hardware revision
    for (iRevFound = MO1000_REV_NOT_FOUND, Rev = eMo1000RevB; Rev <= eMo1000RevLast && iRevFound == MO1000_REV_NOT_FOUND; Rev++)
    {
        if (strncmp(pcHardRevision, Mo1000_apcRev[Rev], strlen(Mo1000_apcRev[0])) == 0)
        {
            // Yes we found one
            iRevFound = Rev;
        }
    }
    
    // Did we found a valid hardware revision
    if (iRevFound != MO1000_REV_NOT_FOUND)
    {
        // Yes save it
        pBoard->HardwareRev = iRevFound;
    }
    else
    {
        FUNC_EXIT(); return eMo1000UnknownRevision;
    }
    
    // Get the I2C device addresses
    Ret = _Mo1000_GetI2cAddr(pBoard, &I2cDevList);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Get the I2C zone based on the FMC position
    Ret = Carrier_FmcGetI2cZone(hCarrier, pBoard->FmcConnector, &I2cZone);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Open the communication channel for other devices that are i2c based
    // For the moment, the expander & expander2 addresses are hard coded
    // generated from the known fix part of device address
    Ret = Mo1000DevCom_Open(&pBoard->Com, hCarrier, I2cZone, I2cDevList.m_ucSpiBridge,
                 I2cDevList.m_ucSensor, I2cDevList.m_ucExpander, I2cDevList.m_ucEeprom);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Create a generic device communication wrapper for the MO1000 device communication
    Ret = DevCom_Open(&pBoard->hCom, &pBoard->Com, Mo1000DevCom_WriteArray, Mo1000DevCom_ReadArray);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Open the io expander module (alpha boards)
    Ret = Expander_Open(&pBoard->Expander, &pBoard->Com, (pBoard->HardwareRev != eMo1000RevB));
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Open the dac module
    Ret = Ad9148_Open(&pBoard->Dac, &pBoard->Com);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Open the pll module
    Ret = Cdce62005_Open(&pBoard->Pll, &pBoard->Com, hCarrier);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Open the temperature monitor module
    Ret = Lm75_Open(&pBoard->Monitor, pBoard->hCom, (uint32_t)eMo1000DevComDeviceSensors);

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_Reset(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;
    int iCount, iDac;

    FUNC_ENTER();

    // Keep trace if global module reset was done or not
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;

    // Clocks synthetization and loop filter params are not ok anymore
    pBoard->iSynthDone = 0;
    pBoard->iLoopFilterDone = 0;

    // Keep trace if module calib was done correctly or not
    pBoard->iCalibDone = 0;
    pBoard->iDataCalibDone = 0;
    pBoard->iFrameCalibDone = 0;
    pBoard->iSyncCalibDone = 0;

    // Keep trace of the number of calibration trials
    pBoard->uCalibNdx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] = 0xffffffff;

    // Keep trace of the number of frame calibration trials
    pBoard->uCalibFrameNdx = 0;

    // Keep trace of the frame calibration errors
    pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = 0xffffffff;

    // Keep trace of the number of sync calibration trials
    pBoard->uCalibSyncNdx = 0;

    // Keep trace of the sync calibration errors
    pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;

    // Clear mask used to check calibration (default all 8 channels lanes)
    pBoard->uChannelLaneMask = 0xffffffff;

    // By default, all 8 channels are used
    pBoard->GroupCh = eMo1000ActChannels08;

    // Check if the core is correct MO1000 type
    if (pBoard->pCore->stInfo.stBit.m_uCoreId != MO1000_COREID)
    {
        FUNC_EXIT();
        return(eMo1000NoCore);
    }

    // Reset the mo1000 core
    pBoard->pCore->stControl.stBit.m_uCoreRst = 1;

    // Wait small delay
    Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);

    // Configure the trigger delay to align with other capture boards like mi125
    pBoard->pCore->stTriggerDelay.stBit.m_uTriggerDelay = MO1000_TRIG_DAC_SYNC_DELAY;

    // Configure the hardware with the number of active channels
    pBoard->pCore->stControl.stBit.m_uDataSerdesPd = 0xFF;
    pBoard->pCore->stControl.stBit.m_uGlobalSerdesPd = 1;
    
    // Set all channel to not valid
    pBoard->pCore->stControl.stBit.m_uDacReady = 0x00;
    // Update DacReady values
    pBoard->pCore->stControl.stBit.m_uUpdateDacReady = 1;
    
    // Disable the data pattern generation
    pBoard->pCore->stControl.stBit.m_uDataPatternEn = 0;

    // Reset the io expander and set everything to low power
    Ret = Expander_Reset(&pBoard->Expander);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Reset the spi bridge (currently, bridge is hold in reset through the expander and cannot be configured through software)
    //Ret = Mo1000DevCom_Reset(&pBoard->Com);
    //if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Reset the pll (currently, pll is in power down after Expander_Reset and cannot be configured through software)
    //Ret = Cdce62005_Reset(&pBoard->Pll);
    //if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Reset each dac (because each dac is already held in reset with the bridge also through expander reset,
    // no software configuration is possible)
    // Reset each dac
    //Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac1);
    //if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    //Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac2);
    //if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Reset the external temp monitor device
    Ret = Lm75_Reset(&pBoard->Monitor);

    // Reset the dacs options structure to default here.
    // Clear dac configuration
    memset(&pBoard->DacConfig,0,sizeof(pBoard->DacConfig));
    
    for (iCount = 0; iCount < MO1000_DAC_PER_BOARD; iCount++)
    {
        // Default inverse sinc filter disable
        pBoard->DacConfig[iCount].ISincState = eAd9148ISincDisable;
        
        // Default fine modulation disable
        pBoard->DacConfig[iCount].FineModState = eAd9148FineModDisable;
        
        // Default gain = 1
        for (iDac = 0; iDac < AD9148_CHANNELS_PER_DAC; iDac++)
        {
            pBoard->DacConfig[iCount].ucGain[iDac] = 64;
        }
    }
    
    // This is supposed to be done automatically by the core at startup...
    //pBoard->pCore->stControl.stBit.m_uDelayCtrlRst = 1;
    
    // Makes sure the reset is long enough in all cases
    Carrier_usleep(pBoard->hCarrier, MO1000_RESET_DELAY);

    // Check if we got a timeout with idelay ready
    if ((pBoard->pCore->stStatus.stBit.m_uDelayCtrlRdy) == 0)
    {
        FUNC_EXIT();

        // Problem with signal not rising after reset
        return(eMo1000NoIDelayReady);
    }

    if(SUCCESS(Ret))
        // Keep trace if global module reset was done or not
        pBoard->iResetDone = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_Init(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;
    Expander_eClkSrc ClkSrc;
    Expander_eClkRef Reference;
    Expander_ePathMuxSel DciFramePath;
    
    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset01);
    }

    // Keep trace if global module init was done or not
    pBoard->iInitDone = 0;

    // Clocks synthetization and loop filter params are not ok anymore
    pBoard->iSynthDone = 0;
    pBoard->iLoopFilterDone = 0;

    // Initialize the io expander to operating condition for all devices
    Ret = Expander_Init(&pBoard->Expander);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait a small bridge get out of reset delay (device ready after reset not specified by manufacturer)
    Carrier_usleep(pBoard->hCarrier, MO1000_BRIDGESTART_DELAY);

    // Initialize the spi bridge (for operation)
    // At this moment we are also sure that all spi cs lines are correctly set to 1
    // We are doing 2 bridge initialization tentatives because to be successfull always on the 1st trial,
    // we need to wait at least around 500 mSec after bridge reset is released which is slow. This technic
    // is faster...
    Ret = Mo1000DevCom_Init(&pBoard->Com);
    if(FAILURE(Ret))
    {
        // Retry to configure the bridge which now must be ready
        Ret = Mo1000DevCom_Init(&pBoard->Com);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    }

    // Release dacs reset
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac1, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac2, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Reset each dac
    Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac2);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Release the pll power down, this will load default eeprom registers content
    Ret = Expander_SetPllShutdown(&pBoard->Expander, ePllPowerDownOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait small delay to make sure pll spi is ready for operation after power-on state (eeprom loading)
    Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);

    // Initialize the pll
    Ret = Cdce62005_Init(&pBoard->Pll);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Initialize each dac
    Ret = Ad9148_Init(&pBoard->Dac, eMo1000DevComDeviceDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_Init(&pBoard->Dac, eMo1000DevComDeviceDac2);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Configure the hardware with the number of active channels
    pBoard->pCore->stControl.stBit.m_uDataSerdesPd = 0;
    pBoard->pCore->stControl.stBit.m_uGlobalSerdesPd = 0;
    
    // Initialize the external temp monitor device to operating condition
    Ret = Lm75_Init(&pBoard->Monitor);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Adjust default starting clock source
    if (pBoard->FmcPosition != eMo1000PositBottom)
    {
        // Top board is using short dci/frame paths
        DciFramePath = eMuxShortPath;

        // For top board, the clk source could not be different than eMo1000ClkSrcBottomfmc
        // if the top board is not the clock master.
        if (pBoard->pCore->stStatus.stBit.m_uClockMaster == 0)
        {
            // Setup to use master provided clock
            ClkSrc = eInternalClk;
            Reference = eMasterClk;
        }
        else
        {
            // Check for alpha board?
            if (pBoard->HardwareRev != eMo1000RevB)
            {
                // We are the clock master so we can start on our own clock
                // Setup to use master own reference
                ClkSrc = eInternalClk;
                Reference = eMasterRef;
            }
            else
            {
                // Before beta board, top board cannot be a clock master...
                Ret = eMo1000InvalidClkTop;
                FUNC_EXIT();
                return Ret;
            }
        }
    }
    else
    {
        // Bottom board is using long dci/frame paths
        DciFramePath = eMuxLongPath;

        // Default for bottom adapter
        // We are the clock master so we can start on our own clock
        // Setup to use master own reference
        ClkSrc = eInternalClk;
        Reference = eMasterRef;
    }

    // Adjust dci/frame paths according to what board we are (bottom or top)
    Ret = Expander_SetDataMux(&pBoard->Expander, DciFramePath, DciFramePath);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Configures the clock source and pll reference
    Ret = _Mo1000_SetClkSource(pBoard, ClkSrc, Reference);

    if(SUCCESS(Ret))
        // Keep trace if global module init was done or not
        pBoard->iInitDone = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_WriteReg(Mo1000_stMo1000 * pBoard, Mo1000_eDevices Device, unsigned char ucAddr, unsigned uVal)
{
    Result Ret = eMo1000Ok;

    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit01);
    }

    switch(Device)
    {
        // Register access to dac devices
        case eMo1000DeviceDac1:
        case eMo1000DeviceDac2:
            Ret = Ad9148_WriteReg(&pBoard->Dac, Device, ucAddr, (unsigned char)(uVal & 0xFF));
            break;
            
        // Register access to pll device
        case eMo1000DevicePll:
            Ret = Cdce62005_WriteReg(&pBoard->Pll, ucAddr, uVal);
            break;
            
        // Low level access to io expander device
        case eMo1000DevicePorts:
            Ret = Expander_WritePortsRaw(&pBoard->Expander, ucAddr, (unsigned char)(uVal & 0xFF));
            break;
            
        // Write specified register in fpga core
        case eMo1000DeviceCore:
            // Check if specified register address is valid
            if (ucAddr >= (sizeof(pBoard->pCore->m_auRegisters) / sizeof(pBoard->pCore->m_auRegisters[0])))
                Ret = eMo1000InvalidCoreReg01;
            else
                pBoard->pCore->m_auRegisters[ucAddr] = uVal;
            break;
            
        default:
            Ret = eMo1000InvalidDevice01;
    }

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_ReadReg(Mo1000_stMo1000 * pBoard, Mo1000_eDevices Device, unsigned char ucAddr, unsigned *puVal)
{
    Result Ret = eMo1000Ok;
    unsigned char ucVal = 0;

    *puVal = 0;
        
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit02);
    }

    switch(Device)
    {
        // Register access to dac devices
        case eMo1000DeviceDac1:
        case eMo1000DeviceDac2:
            Ret = Ad9148_ReadReg(&pBoard->Dac, Device, ucAddr, &ucVal);
            *puVal = ucVal; 
            break;
            
        // Register access to pll device
        case eMo1000DevicePll:
            Ret = Cdce62005_ReadReg(&pBoard->Pll, ucAddr, puVal);
            break;
            
        // Low level access to io expander device latched output port
        case eMo1000DevicePorts:
            Ret = Expander_ReadPortsRaw(&pBoard->Expander, ucAddr, &ucVal);
            *puVal = ucVal; 
            break;
            
        // Read specified register in fpga core
        case eMo1000DeviceCore:
            // Check if specified register address is valid
            if (ucAddr >= (sizeof(pBoard->pCore->m_auRegisters) / sizeof(pBoard->pCore->m_auRegisters[0])))
                Ret = eMo1000InvalidCoreReg02;
            else
                *puVal = pBoard->pCore->m_auRegisters[ucAddr];
            break;
            
        default:
            Ret = eMo1000InvalidDevice02;
    }

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_Close(Mo1000_stMo1000 * board)
{
    Result Ret = eMo1000Ok;

    FUNC_ENTER();

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_GetStatus(Mo1000_stMo1000 * pBoard, Mo1000_stBoardStatus *pStatus, Mo1000_stCompareErrors *pCompare)
{
    Result Ret = eMo1000Ok;
    Expander_eDacIrq DacIrq;
    Expander_ePllLock PllLock;
    Ad9148_stDacEvent stEvent;
    unsigned short usRisingEdge, usFallingEdge;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit01);
    }

    // By default, status is 0 (No error)
    pStatus->m_uRaw = 0;
    pCompare->m_uRaw = 0;
    
    // Check if we have any dac errors?
    Ret = Expander_GetDacIrq(&pBoard->Expander, &DacIrq);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Any errors to read from dac 1
    if (DacIrq == eDacBothErrors || DacIrq == eDac1Error)
    {
        // Get the irq events for this dac device
        Ret = Ad9148_GetIrqEvents(&pBoard->Dac,  eMo1000DevComDeviceDac1, &stEvent);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Clear the irq events for this dac device
        Ret = Ad9148_ClearIrqEvent(&pBoard->Dac,  eMo1000DevComDeviceDac1, stEvent);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        pStatus->stEventParts.m_uEvent10 = stEvent.stEventParts.m_uEvent0;
        pStatus->stEventParts.m_uEvent11 = stEvent.stEventParts.m_uEvent1;
    }

    // Any errors to read from dac 2
    if (DacIrq == eDacBothErrors || DacIrq == eDac2Error)
    {
        // Get the irq events for this dac device
        Ret = Ad9148_GetIrqEvents(&pBoard->Dac,  eMo1000DevComDeviceDac2, &stEvent);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Clear the irq events for this dac device
        Ret = Ad9148_ClearIrqEvent(&pBoard->Dac,  eMo1000DevComDeviceDac2, stEvent);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        pStatus->stEventParts.m_uEvent20 = stEvent.stEventParts.m_uEvent0;
        pStatus->stEventParts.m_uEvent21 = stEvent.stEventParts.m_uEvent1;
    }
    
    // Check if sed error and read sed results eAd9148SelectPortA dac1
    Ret = Ad9148_GetSedResult(&pBoard->Dac,  eMo1000DevComDeviceDac1, eAd9148SelectPortA,
                              &usRisingEdge, &usFallingEdge);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    pCompare->stBit.m_uDac1CompareErr |= (usRisingEdge | usFallingEdge);

    // Check if sed error and read sed results eAd9148SelectPortA dac2
    Ret = Ad9148_GetSedResult(&pBoard->Dac,  eMo1000DevComDeviceDac2, eAd9148SelectPortA,
                              &usRisingEdge, &usFallingEdge);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    pCompare->stBit.m_uDac2CompareErr |= (usRisingEdge | usFallingEdge);

    // Read clock pll lock status
    Ret = Expander_GetPllLock(&pBoard->Expander, &PllLock);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    pStatus->stBit.m_uPllLock = PllLock;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_GetTemperature(Mo1000_stMo1000 * pBoard, Mo1000_eTempMode Mode, short* psTemp, short* psDac1, short* psDac2)
{
    Result Ret = eMo1000Ok;

    FUNC_ENTER();

    // !!! M.T. to be tweaked
    *psDac1 = AD9148_INVALIDTEMP;
    *psDac2 = AD9148_INVALIDTEMP;

    // If temperature mode is not rounding to Celsius
    if (Mode != eMo1000Temp1c)
        *psDac1 = *psDac1 * 10;

    // If temperature mode is not rounding to Celsius
    if (Mode != eMo1000Temp1c)
        *psDac2 = *psDac2 * 10;

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit03);
    }

    // Read pcb temperature
    Ret = Lm75_ReadTemp10X(&pBoard->Monitor, psTemp);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // If temperature mode is rounding to Celsius
    if (Mode == eMo1000Temp1c)
        *psTemp = *psTemp / 10;

    // Read dac1 temperature
    Ret = Ad9148_ReadTemp(&pBoard->Dac, eMo1000DevComDeviceDac1, psDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // If temperature mode is not rounding to Celsius
    if (Mode != eMo1000Temp1c)
        *psDac1 = *psDac1 * 10;

    // If dac2 is not completly powered down
    if (pBoard->GroupCh > eMo1000ActChannels04)
    {
        // Read dac2 temperature
        Ret = Ad9148_ReadTemp(&pBoard->Dac, eMo1000DevComDeviceDac2, psDac2);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // If temperature mode is not rounding to Celsius
        if (Mode != eMo1000Temp1c)
            *psDac2 = *psDac2 * 10;
    }

    FUNC_EXIT();

    return(Ret);
}


Result Mo1000_DoDacUpdate(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;
    int iCount, iPair, iChannel;

    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit04);
    }

    // Configure each dac device
    for (iCount = 0; iCount < MO1000_DAC_PER_BOARD; iCount++)
    {
        // For each dac pair
        for (iPair = 0; iPair < AD9148_CHANNELS_PER_DAC/2; iPair++)
        {
            // Default is set for all 8 channels not processed here would need to do what is
            // in Mo1000_SetDacParActChannel(). So the default must stay at 8 channels active

            // Check if channel pair state has been updated
            if (!pBoard->DacConfig[iCount].ucActPairStateUpdate[iPair])
            {
            //    Ret = Ad9148_SetActChannel(&pBoard->Dac, iCount, iPair, pBoard->DacConfig[iCount].ActPairState[iPair]);
            //    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
               pBoard->DacConfig[iCount].ActPairState[iPair] = eAd9148DacPairEnable;
               pBoard->DacConfig[iCount].ucActPairStateUpdate[iPair] = 1;
            }

            // If inverse sinc parameters have not been updated
            if (!pBoard->DacConfig[iCount].ucISincUpdate[iPair])
            {
                Ret = Ad9148_SetISinc(&pBoard->Dac, iCount, iPair, pBoard->DacConfig[iCount].stISincReal[iPair],
                                      pBoard->DacConfig[iCount].stISincImaginary[iPair]);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
                pBoard->DacConfig[iCount].ucISincUpdate[iPair] = 1;
            }
            
            // If quadrature parameters have not been updated
            if (!pBoard->DacConfig[iCount].ucQuadPhaseUpdate[iPair])
            {
                Ret = Ad9148_SetQuadPhase(&pBoard->Dac, iCount, iPair, pBoard->DacConfig[iCount].sRealPhase[iPair],
                                          pBoard->DacConfig[iCount].sImaginaryPhase[iPair]);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
                pBoard->DacConfig[iCount].ucQuadPhaseUpdate[iPair] = 1;
            }
        }
        // For each channel in a dac
        for (iChannel = 0; iChannel < AD9148_CHANNELS_PER_DAC; iChannel++)
        {
            // If Dc offsets have not been updated
            if (!pBoard->DacConfig[iCount].ucDcOffsetUpdate[iChannel])
            {
                Ret = Ad9148_SetDcOffset(&pBoard->Dac, iCount, iChannel, pBoard->DacConfig[iCount].sDcOffset[iChannel]);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
                pBoard->DacConfig[iCount].ucDcOffsetUpdate[iChannel] = 1;
            }

            // If Gains have not been updated
            if (!pBoard->DacConfig[iCount].ucGainUpdate[iChannel])
            {
                Ret = Ad9148_SetGain(&pBoard->Dac, iCount, iChannel, pBoard->DacConfig[iCount].ucGain[iChannel]);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
                pBoard->DacConfig[iCount].ucGainUpdate[iChannel] = 1;
            }
        }
        // If interpolation mode has not been updated
        if (!pBoard->DacConfig[iCount].ucInterUpdate)
        {
            Ret = Ad9148_SetInterpolation(&pBoard->Dac, iCount, pBoard->DacConfig[iCount].InterMode);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            pBoard->DacConfig[iCount].ucInterUpdate = 1;
        }
        // If fine modulation control state has not been updated
        if (!pBoard->DacConfig[iCount].ucFineModStateUpdate)
        {
            Ret = Ad9148_SetFineModCtrl(&pBoard->Dac, iCount, pBoard->DacConfig[iCount].FineModState);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            pBoard->DacConfig[iCount].ucFineModStateUpdate = 1;
        }
        // If fine modulation has not been updated
        if (!pBoard->DacConfig[iCount].ucFineModUpdate)
        {
            Ret = Ad9148_SetFineMod(&pBoard->Dac, iCount, pBoard->DacConfig[iCount].uFtw,
                                    pBoard->DacConfig[iCount].usNcoPhaseOffset, pBoard->DacConfig[iCount].Sideband);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            pBoard->DacConfig[iCount].ucFineModUpdate = 1;
        }
        // If data format has not been updated
        if (!pBoard->DacConfig[iCount].ucDataFormatUpdate)
        {
            Ret = Ad9148_SetDataFormat(&pBoard->Dac, iCount, pBoard->DacConfig[iCount].DataFormat);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            pBoard->DacConfig[iCount].ucDataFormatUpdate = 1;
        }
        // If inverse sinc control state has not been updated
        if (!pBoard->DacConfig[iCount].ucISincStateUpdate)
        {
            Ret = Ad9148_SetISincCtrl(&pBoard->Dac, iCount, pBoard->DacConfig[iCount].ISincState);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            pBoard->DacConfig[iCount].ucISincStateUpdate = 1;
        }
    }
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_DoDacCalibration(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;
    int iCount, iTemp;
    unsigned uKeep;
    char cMin, cMax, cOffset = 0;

    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit05);
    }

    // Default global tap delay offset
    pBoard->cTapsDelayOffset = cOffset;
    
    // Keep trace of the number of calibration trials
    pBoard->uCalibNdx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] = 0xffffffff;
    
    // Keep trace of the number of frame calibration trials
    pBoard->uCalibFrameNdx = 0;

    // Keep trace of the frame calibration errors
    pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = 0xffffffff;

    // Keep trace of the number of sync calibration trials
    pBoard->uCalibSyncNdx = 0;

    // Keep trace of the sync calibration errors
    pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;

    // Set all channel to not valid
    pBoard->pCore->stControl.stBit.m_uDacReady = 0x00;
    // Update DacReady values
    pBoard->pCore->stControl.stBit.m_uUpdateDacReady = 1;

    // Keep trace if calib was made correctly or not
    pBoard->iCalibDone = 0;
    pBoard->iDataCalibDone = 0;
    pBoard->iFrameCalibDone = 0;
    pBoard->iSyncCalibDone = 0;

    // Keep trace if test mode is active
    pBoard->iTestMode = 0;

    // Make sure the clocks are stabilized if any clock changed was done

    pBoard->pCore->stControl.stBit.m_uDelayRst = 1;
    pBoard->pCore->stControl.stBit.m_uSerdesRst = 1;

    // Wait small delay for resets to complete
    Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);

    // Check if mmcm is locked when we are a clock master as a security measure
    if ((pBoard->pCore->stStatus.stBit.m_uClockMaster) == 1 && (pBoard->pCore->stStatus.stBit.m_uMmcmLocked) == 0)
    {
        FUNC_EXIT();

        // Problem with signal not rising after reset
        return(eMo1000NoMmcmLock);
    }

    // Attempt data calibration
    Ret = _Mo1000_DoDataCalib(pBoard);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Attempt frame calibration
    Ret = _Mo1000_DoFrameCalib(pBoard);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    PDEBUG("Sync smallest/greatest data delay min:%d, max:%d.\n",pBoard->ucDataSmallestDelay,pBoard->ucDataGreatestDelay);
    PDEBUG("Sync frame delay:%d, dci delay:%d.\n",pBoard->ucFrameIoDelay,pBoard->ucDciIoDelay);

    // Calculates our global taps offset possible range [-cMin,(31 - cMax)]
    if (pBoard->ucDataSmallestDelay < pBoard->ucFrameIoDelay)
        cMin = pBoard->ucDataSmallestDelay;
    else
        cMin = pBoard->ucFrameIoDelay;
        
    if (pBoard->ucDciIoDelay < cMin)
        cMin = pBoard->ucDciIoDelay;
        
    // This is minimum global tap offset correction possible
    cMin *= -1;
    
    if (pBoard->ucDataGreatestDelay > pBoard->ucFrameIoDelay)
        cMax = pBoard->ucDataGreatestDelay;
    else
        cMax = pBoard->ucFrameIoDelay;
        
    if (pBoard->ucDciIoDelay > cMax)
        cMax = pBoard->ucDciIoDelay;

    // This is maximum global tap offset correction possible
    cMax = 31 - cMax;

    PDEBUG("Sync possible global offset min:%d, max:%d.\n",cMin,cMax);

    // Loop for the maximum number of sync calibration retries
    do
    {
        // Try all possible global tap delay offset until no error is found on all dacs sync calibration
        do
        {
            Ret = eMo1000Ok;
            
            // Keep trace of the sync calibration errors
            pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;
        
            // Synchronize each dac device
            for (iCount = 0; iCount < MO1000_DAC_PER_BOARD && SUCCESS(Ret); iCount++)
            {
            	if (iCount == (MO1000_DAC_PER_BOARD-1))
            		uKeep =0;
            	else
            		uKeep =1;
        
            	iTemp = 0xffff << (16 * iCount);
                pBoard->iSyncCalibDone &= (~iTemp);
                
                // Check if this dac need to be sync (if not in power down)
                if (pBoard->uChannelLaneMask & iTemp)
                {
                    // Synchronize each dac and keep sync mode until last dac
                    Ret = _Mo1000_DoFifoSync(pBoard, iCount, uKeep, 0);
                
                    if(SUCCESS(Ret))
                    {
                        // Save sync calibration status for current dac
                        pBoard->iSyncCalibDone |= iTemp;
                    }
    
                    // Keep trace of the errors we got
                    pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = Ret;
                    PDEBUG("Sync Dac#%02d.\n",iCount);
                    PDEBUG("Sync Trial #%02d, p1 errors: 0x%08X.\n",pBoard->uCalibSyncNdx, Ret);
                }
            }
            
            PDEBUG("Sync global tap offset %d.\n",cOffset);

            // Did we got an error
            if (FAILURE(Ret))
            {
                // Yes compute a new global tap delay offset for next turn
                
                // Here we will explore the possible global taps offset in a non sequential manner
                // by incresing in an absolute manner the distance from 0 offset to cover all possible offset range
                
                // Do we have a zero value 
                if (!cOffset)
                {
                    cOffset = 1;
                }
                else
                {
                    // Is it a positive value
                    if (cOffset > 0)
                    {
                        cOffset = -cOffset;
                    }
                    else
                    {
                        cOffset = -cOffset + 1;
                    }
                }
                // Are we still inside possible offset range
                if (cOffset > cMax || cOffset < cMin)
                {
                    // No try opposite offset
                    cOffset = -cOffset;

                    // To avoid retrying 2 times the same positive offset
                    if (cOffset > 0)
                        cOffset++;
                }
                    
                // If global tap offset is withing range, use it and update all system taps delay with this new offset
                if (cOffset >= cMin && cOffset <= cMax)
                {
                    pBoard->cTapsDelayOffset = cOffset;
                    _Mo1000_UpdateAllTapDelays(pBoard);
                }
            }
        }while((cOffset >= cMin && cOffset <= cMax) && FAILURE(Ret));
        
        // Keep trace of the errors we got
        pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = Ret;
        PDEBUG("Sync Trial #%02d, p2 errors: 0x%08X.\n",pBoard->uCalibSyncNdx, Ret);
        
        // Do we have to retry?
        if (FAILURE(Ret) && ( (pBoard->uCalibSyncNdx + 1) < MO1000_MAX_SYNC_CALIB_ATTEMPT))
        {
            // Set back global tap offset to 0 for next attempt
            cOffset = 0;
            pBoard->cTapsDelayOffset = cOffset;
            _Mo1000_UpdateAllTapDelays(pBoard);
        }
    }while(FAILURE(Ret) && ( (++pBoard->uCalibSyncNdx) < MO1000_MAX_SYNC_CALIB_ATTEMPT));

    if(FAILURE(Ret))
    {
        // Can we try to relax write pointer correction and make it work?
        if (RESULT_CODE(Ret) == eMo1000FifoTherNotSet)
        {
            // Keep trace of the sync calibration errors
            pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;
        
            // Synchronize each dac device
            for (iCount = 0, Ret = eMo1000Ok; iCount < MO1000_DAC_PER_BOARD && SUCCESS(Ret); iCount++)
            {
            	if (iCount == (MO1000_DAC_PER_BOARD-1))
            		uKeep =0;
            	else
            		uKeep =1;
        
            	iTemp = 0xffff << (16 * iCount);
                pBoard->iSyncCalibDone &= (~iTemp);
                
                // Check if this dac need to be sync (if not in power down)
                if (pBoard->uChannelLaneMask & iTemp)
                {
                    // Synchronize each dac and keep sync mode until last dac
                    Ret = _Mo1000_DoFifoSync(pBoard, iCount, uKeep, 1);
                
                    if(SUCCESS(Ret))
                    {
                        // Save sync calibration status for current dac
                        pBoard->iSyncCalibDone |= iTemp;
                    }
    
                    // Keep trace of the errors we got
                    pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = Ret;
                    PDEBUG("Sync Dac#%02d.\n",iCount);
                    PDEBUG("Sync Trial #%02d, p1 errors: 0x%08X.\n",pBoard->uCalibSyncNdx, Ret);
                }
            }
            
            PDEBUG("Sync global tap offset %02d.\n",cOffset);
        }
        else
        {
            // Set the index inside allowed buffer
            pBoard->uCalibSyncNdx--;
            
            FUNC_EXIT();
            
            // Problem with sync training
            return Ret;
        }
    }

    // Set channels not in powered down to valid
    pBoard->pCore->stControl.stBit.m_uDacReady = (0xff & ~(0xfc << (pBoard->GroupCh *2)));
    // Update DacReady values
    pBoard->pCore->stControl.stBit.m_uUpdateDacReady = 1;

    // Keep trace if calib was made correctly or not
    pBoard->iCalibDone = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetClockConfig(Mo1000_stMo1000 * pBoard, Mo1000_eClockSource SrcClk, unsigned uFreqSrcClk, 
                             unsigned uFreqDacClock, Mo1000_eMasterClkMode MasterClkMode,
                             unsigned uFreqMasterClock)
{
    Result Ret = eMo1000Ok;
    unsigned uInterRate;
    Expander_eClkSrc ClkSrc;
    Expander_eClkRef Reference;
    unsigned uSecondaryRef;
    int iCount;

    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit06);
    }

    // Clocks synthetization and loop filter params are not ok anymore
    pBoard->iSynthDone = 0;
    pBoard->iLoopFilterDone = 0;

    // For top board, the clk source could not be different than eMo1000ClkSrcBottomfmc
    // if the top board is not the clock master. When a RevB board, the top clk source
    // must be eMo1000ClkSrcBottomfmc
    if ((pBoard->FmcPosition == eMo1000PositTop && SrcClk != eMo1000ClkSrcBottomfmc) &&
        (pBoard->pCore->stStatus.stBit.m_uClockMaster == 0 || pBoard->HardwareRev == eMo1000RevB))
    {
        FUNC_EXIT();
        return(eMo1000InvalidTopClk);
    }

    // Source clock validation
    switch(SrcClk)
    {
        case eMo1000ClkSrc125mhz:
            uFreqSrcClk = MO1000_INT_FREQUENCY;
            ClkSrc = eInternalClk;
            Reference = eMasterRef;
            break;
            
        case eMo1000ClkSrcExt:
            // This clock source is not supported on revB
            if (pBoard->HardwareRev == eMo1000RevB)
            {
                FUNC_EXIT();
                return(eMo1000UnsupportedClkSrc);
            }
            ClkSrc = eExternalClk;
            Reference = eMasterRef;
            break;
            
        case eMo1000ClkSrcBottomfmc:
            if ((pBoard->FmcPosition == eMo1000PositBottom))
            {
                FUNC_EXIT();
                return(eMo1000InvalidBottomClk);
            }

            ClkSrc = eInternalClk;
            Reference = eMasterClk;
            break;
            
        case eMo1000ClkSrcFmccarrier2:
            // This clock source is not supported on revB
            if (pBoard->HardwareRev == eMo1000RevB)
            {
                FUNC_EXIT();
                return(eMo1000UnsupportedClkSrc);
            }
            ClkSrc = eCarrier2Clk;
            Reference = eMasterRef;
            break;
            
        case eMo1000ClkSrcFmccarrier3:
            // This clock source is not supported on revB
            if (pBoard->HardwareRev == eMo1000RevB)
            {
                FUNC_EXIT();
                return(eMo1000UnsupportedClkSrc);
            }
            ClkSrc = eCarrier3Clk;
            Reference = eMasterRef;
            break;
            
        default:
        FUNC_EXIT();
        return(eMo1000InvalidClkSrc);
    }
    
    // Saves source clock frequency
    pBoard->uFreqSrcClk = uFreqSrcClk;
    
    // According interpolation rates, check the specified clocks frequency
    // Because the rate must be the same for all dacs, only interpolation rate of dac 0 is verified here
    switch(pBoard->DacConfig[0].InterMode)
    {
        // 1x
        case eAd9148Inter1x:
            // Check if dac clock overrange
            if (uFreqDacClock > MO1000_MAX_DAC_FREQ_1X)
            {
                FUNC_EXIT();
                return(eMo1000DacClkOver);
            }
            uInterRate = 1;
            break;

        // 2x
        case eAd9148Inter2x:
        case eAd9148Inter2x_fsdiv4:
        case eAd9148Inter2x_fsdiv2:
        case eAd9148Inter2x_3fsdiv4:
            // Check if dac clock overrange
            if (uFreqDacClock > MO1000_MAX_DAC_FREQ_2X)
            {
                FUNC_EXIT();
                return(eMo1000DacClkOver);
            }
            uInterRate = 2;
            break;

        // 4x
        case eAd9148Inter4x:
        case eAd9148Inter4x_fsdiv8:
        case eAd9148Inter4x_fsdiv4:
        case eAd9148Inter4x_3fsdiv8:
        case eAd9148Inter4x_fsdiv2:
        case eAd9148Inter4x_5fsdiv8:
        case eAd9148Inter4x_3fsdiv4:
        case eAd9148Inter4x_7fsdiv8:
            // Check if dac clock overrange
            if (uFreqDacClock > MO1000_MAX_DAC_FREQ_4X)
            {
                FUNC_EXIT();
                return(eMo1000DacClkOver);
            }
            uInterRate = 4;
            break;

        // 8x
        case eAd9148Inter8x:
        case eAd9148Inter8x_fsdiv8:
        case eAd9148Inter8x_fsdiv4:
        case eAd9148Inter8x_3fsdiv8:
        case eAd9148Inter8x_fsdiv2:
        case eAd9148Inter8x_5fsdiv8:
        case eAd9148Inter8x_3fsdiv4:
        case eAd9148Inter8x_7fsdiv8:
            // Check if dac clock overrange
            if (uFreqDacClock > MO1000_MAX_DAC_FREQ_8X)
            {
                FUNC_EXIT();
                return(eMo1000DacClkOver);
            }
            uInterRate = 8;
            break;
            
        default:
        FUNC_EXIT();
        return(eMo1000InvalidInterRate);
    }
    
    // Saves dacs clock frequency
    pBoard->iFreqDacClock1 = uFreqDacClock;
    
    // Compute fpga reference frequency
    pBoard->iFreqCoreRef4 = uFreqDacClock / uInterRate * 2;
    pBoard->iFreqCoreRef4Calculated = pBoard->iFreqCoreRef4;
    
    // Check if data rate frequency is under range?
    if (pBoard->iFreqCoreRef4 < MO1000_MIN_FPGA_DVI_FREQ)
    {
        FUNC_EXIT();
        return(eMo1000DataFreqUnder);
    }
    
    // Compute synchronisation frequency
    pBoard->iFreqSyncClock0 = uFreqDacClock / uInterRate / 8;
    
    // Compute master ref clock
    pBoard->iFreqMasterRef2 = pBoard->iFreqDacClock1;
    
    uSecondaryRef = pBoard->iFreqMasterRef2;
    
    // Master clock mode validation
    switch(MasterClkMode)
    {
        case eMo1000MasterClkAuto:
            pBoard->iFreqMasterClock3 = pBoard->iFreqMasterRef2;
            break;
            
        case eMo1000MasterClkManual:
            if (uFreqMasterClock > MO1000_MAX_MASTER_CLK_FREQ)
            {
                FUNC_EXIT();
                return(eMo1000BadMasterClkFreq);
            }
            
            // If the provided clock value is not 0
            if (uFreqMasterClock != 0)
                // Set it to user requierement
                pBoard->iFreqMasterClock3 = uFreqMasterClock;
            else
                // Disable that clock (virtually set the frequency to same thing as the reference)
                pBoard->iFreqMasterClock3 = -abs(pBoard->iFreqMasterRef2);
            break;
            
        default:
        FUNC_EXIT();
        return(eMo1000BadMasterClkMode);
    }
    
    // Top board?
    if (pBoard->FmcPosition == eMo1000PositTop)
    {
        // For top board, master clock is not used and can be disable (for emi and power save)
        pBoard->iFreqMasterClock3 = -abs(pBoard->iFreqMasterClock3);
        
        // If top adapter is using bottom reference, this clock is not needed and could be turn off
        if (Reference == eMasterClk)
        {
            pBoard->iFreqMasterRef2 = 0;
        }
        
        // For a top slave adapter, the fpga reference is not needed and could be turned off
        if (pBoard->pCore->stStatus.stBit.m_uClockMaster == 0)
            pBoard->iFreqCoreRef4 = 0;
    }

    // Configure the virtual output channel to control the vco configuration no matter what real outputs are...        
    // Are we reference on the bottom board clock?
    if (SrcClk == eMo1000ClkSrcBottomfmc)
    {
        // All possible cases are covered here...
        // Yes, If internal bottom reference is internal clock, attempt to set vco for possible sec freq.
        if (uFreqSrcClk == MO1000_INT_FREQUENCY)
        {
            // For internal clock, makes sure the vco is programmed for this 2 cases where it won't be used on real outputs
            if ((pBoard->iFreqDacClock1 == MO1000_INT_FREQUENCY && uInterRate == 2) ||
               (pBoard->iFreqDacClock1 == MO1000_INT_FREQUENCY / 2 && uInterRate == 1)){ 
                pBoard->iFreqVirtualClock5 = MO1000_INT_FREQUENCY * 2;
               }
            else
            {
                // Computes highest possible synthetized clock
                if (uInterRate == 1)
                    pBoard->iFreqVirtualClock5 = pBoard->iFreqCoreRef4Calculated;
                else
                    pBoard->iFreqVirtualClock5 = uSecondaryRef;
            }
        }
        else
        {
            pBoard->iFreqVirtualClock5 = MO1000_INT_DEFAULTOUT;
            
            // Will lock the pll with the primary internal reference
            uFreqSrcClk = MO1000_INT_FREQUENCY;
        }
    }
    else
    {
        // This section is for master board reference on it's own clock
        // !!! Warning: This is an attempt to lock the pll and cover most of the cases but not necessary all
        if ((pBoard->iFreqDacClock1 == uFreqSrcClk && uInterRate > 1) ||
            (pBoard->iFreqDacClock1 == uFreqSrcClk / 2 && uInterRate == 1) || 
            (pBoard->iFreqDacClock1 == uFreqSrcClk / 2 && uInterRate > 1) ||
            (pBoard->iFreqDacClock1 == uFreqSrcClk / 4 && uInterRate == 1)){
            pBoard->iFreqVirtualClock5 = uFreqSrcClk * 2;
        }
        else
            pBoard->iFreqVirtualClock5 = 0;
    }

    // Keep trace of the number of data calibration trials
    pBoard->uCalibNdx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] = 0xffffffff;

    // Keep trace of the number of frame calibration trials
    pBoard->uCalibFrameNdx = 0;

    // Keep trace of the frame calibration errors
    pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = 0xffffffff;

    // Keep trace of the number of sync calibration trials
    pBoard->uCalibSyncNdx = 0;

    // Keep trace of the sync calibration errors
    pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;

    // Keep trace if calib was made correctly or not
    pBoard->iCalibDone = 0;
    pBoard->iDataCalibDone = 0;
    pBoard->iFrameCalibDone = 0;
    pBoard->iSyncCalibDone = 0;

    // Turn off synchronize on each dac device
    for (iCount = 0; iCount < MO1000_DAC_PER_BOARD && SUCCESS(Ret); iCount++)
    {
        // Disable synchronisation mode before killing the clocks
        Ret = Ad9148_SetSyncCtrl(&pBoard->Dac, iCount, eAd9148SyncDisable);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    }

    // Freeze the clock outputs before changing the configuration
    Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOn);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Configures the clock source and pll reference
    Ret = _Mo1000_SetClkSource(pBoard, ClkSrc, Reference);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Needs to configure the pll here
    // Uses only internal loop filter comparison frequency range (to save some calculation times)
	pBoard->stParam.uPfdFreqMin = CDCE62005_PFDMININT;
	pBoard->stParam.uPfdFreqMax = CDCE62005_PFDMAX;

	pBoard->stParam.SmartMuxRefSelect = eCdce62005InputRefModePrimary;        // Use the primary input as the pll reference clock
	pBoard->stParam.LoopFilterType = eCdce62005LoopFilterInt;                 // Attempt to use internally calculated loop filter
    pBoard->stParam.RefInput.InputsType=eCdce62005InputLvPecl;                // prim/sec inputs are lvpecl type
    pBoard->stParam.RefInput.InputsTermination=eCdce62005InputDcTermination;  // Input uses internal dc termination
    pBoard->stParam.RefInput.LvdsFailSafe=eCdce62005LvdsInputFailDisable;     // Configure lvds failsafe mode
    pBoard->stParam.RefInput.uAuxInputFrequency=0;                            // We do not use auxiliary input
    pBoard->stParam.RefInput.PrimaryInput.ucForceDiv2=0;                      // Do not force primary input prediv by 2: can be freely adjusted by the calculations
    pBoard->stParam.RefInput.PrimaryInput.uInputFrequency=uFreqSrcClk;        // Primary input frequency
    pBoard->stParam.RefInput.PrimaryInput.InputVbb=eCdce62005InputVbbNormal;  // Primary input not inverted
    pBoard->stParam.RefInput.PrimaryInput.InputTerm=eCdce62005InputTermEnabled; // Enable internal input termination
    pBoard->stParam.RefInput.SecondaryInput.ucForceDiv2=0;                    // Do not force secondary input prediv by 2: can be freely adjusted by the calculations
    pBoard->stParam.RefInput.SecondaryInput.InputVbb=eCdce62005InputVbbNormal;// Secondary input not inverted
    pBoard->stParam.RefInput.SecondaryInput.InputTerm=eCdce62005InputTermEnabled;// Enable internal input termination
	pBoard->stParam.SyncMode = Cdce62005_eSyncNextRefAndOutput;               // Sync by detection of input rise and gen clocks rise
	pBoard->stParam.AuxParams.AuxState = eCdce62005AuxDisable;                // Do not use auxiliary output
	pBoard->stParam.AuxParams.AuxSelect = eCdce62005AuxSelect2;               // Auxiliary output based on output divider 2 when used
	pBoard->stParam.PllLockParams.PllLockMode = eCdce62005DigitalPllLock;     // Pll lock pin is configured for digital mode
	pBoard->stParam.PllLockParams.PllLockWidth = eCdce62005PllLockW1D5Ns;     // Use 1.5 nSec pll lock window
	pBoard->stParam.PllLockParams.PllLockCount = eCdce62005PllLockCount1Pfd;  // Use immediate (fast) reaction of the pll lock
	pBoard->stParam.OutParams[2].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output
	pBoard->stParam.OutParams[2].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
	pBoard->stParam.OutParams[2].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin
	pBoard->stParam.OutParams[2].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin
	pBoard->stParam.OutParams[3].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
	pBoard->stParam.OutParams[3].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
	pBoard->stParam.OutParams[3].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
	pBoard->stParam.OutParams[3].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
	pBoard->stParam.OutParams[0].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                                           
	pBoard->stParam.OutParams[0].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
	pBoard->stParam.OutParams[0].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin                           
	pBoard->stParam.OutParams[0].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin                           
	pBoard->stParam.OutParams[1].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
	pBoard->stParam.OutParams[1].OutputRef = eCdce62005OutRefModeSecondary;   // Output will be referenced to secondary input without using the pll
	pBoard->stParam.OutParams[1].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
	pBoard->stParam.OutParams[1].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
	pBoard->stParam.OutParams[4].usPhaseAdj = 0;                              // Phase is requiered to be 0 deg on this output                     
	pBoard->stParam.OutParams[4].OutputRef = eCdce62005OutRefModeSynth;       // Output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
	pBoard->stParam.OutParams[4].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Positive output in lvcmos mode would be a positive active pin     
	pBoard->stParam.OutParams[4].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Negative output in lvcmos mode would be a positive active pin     
	pBoard->stParam.OutParams[5].usPhaseAdj = 0;                              // Virtual output Phase is requiered to be 0 deg on this output                                           
	pBoard->stParam.OutParams[5].OutputRef = eCdce62005OutRefModeSynth;       // Virtual output should be referenced to pll (will used fanout mode if possible to bypass the pll and could be modified by calculation)
	pBoard->stParam.OutParams[5].PLvCmosMode = eCdce62005LvCmosOutPinActive;  // Virtual Positive output in lvcmos mode would be a positive active pin                           
	pBoard->stParam.OutParams[5].NLvCmosMode = eCdce62005LvCmosOutPinActive;  // Virtual Negative output in lvcmos mode would be a positive active pin                           
    // Output Frequencies configuration, keep positive or 0 values output enable or disable the output when frequency is negative
    // A clock of freq = 0 would keep the output enable but no clocking
    if (pBoard->iFreqSyncClock0 >= 0)
    {
        pBoard->stParam.OutParams[0].uOutputFreq = pBoard->iFreqSyncClock0;
	    pBoard->stParam.OutParams[0].OutputMode = eCdce62005OutModeLvPecl;    // Output pin type is lvpecl                                                               
    }
    else
    {
        pBoard->stParam.OutParams[0].uOutputFreq = 0;
	    pBoard->stParam.OutParams[0].OutputMode = eCdce62005OutModeDisabled;  // Output pin type is disabled                                                               
    }
    if (pBoard->iFreqDacClock1 >= 0)
    {
        pBoard->stParam.OutParams[1].uOutputFreq = pBoard->iFreqDacClock1;
	    pBoard->stParam.OutParams[1].OutputMode = eCdce62005OutModeLvPecl;    // Output pin type is lvpecl                                         
    }
    else
    {
        pBoard->stParam.OutParams[1].uOutputFreq = 0;
	    pBoard->stParam.OutParams[1].OutputMode = eCdce62005OutModeDisabled;  // Output pin type is disabled                                                               
    }
    if (pBoard->iFreqMasterRef2 >= 0)
    {
        pBoard->stParam.OutParams[2].uOutputFreq = pBoard->iFreqMasterRef2;
	    pBoard->stParam.OutParams[2].OutputMode = eCdce62005OutModeLvPecl;    // Output pin type is lvpecl
    }
    else
    {
        pBoard->stParam.OutParams[2].uOutputFreq = 0;
	    pBoard->stParam.OutParams[2].OutputMode = eCdce62005OutModeDisabled;  // Output pin type is disabled                                                               
    }
    if (pBoard->iFreqMasterClock3 >= 0)
    {
	    pBoard->stParam.OutParams[3].uOutputFreq = pBoard->iFreqMasterClock3;
	    pBoard->stParam.OutParams[3].OutputMode = eCdce62005OutModeLvPecl;    // Output pin type is lvpecl                                                               
    }
    else
    {
	    pBoard->stParam.OutParams[3].uOutputFreq = 0;
	    pBoard->stParam.OutParams[3].OutputMode = eCdce62005OutModeDisabled;  // Output pin type is disabled                                                               
    }
    if (pBoard->iFreqCoreRef4 >= 0)
    {
        pBoard->stParam.OutParams[4].uOutputFreq = pBoard->iFreqCoreRef4;
	    pBoard->stParam.OutParams[4].OutputMode = eCdce62005OutModeLvds;      // Output pin type is lvds                                         
    }
    else
    {
        pBoard->stParam.OutParams[4].uOutputFreq = 0;
	    pBoard->stParam.OutParams[4].OutputMode = eCdce62005OutModeDisabled;  // Output pin type is disabled                                                               
    }
    if (pBoard->iFreqVirtualClock5 >= 0)
    {
        pBoard->stParam.OutParams[5].uOutputFreq = pBoard->iFreqVirtualClock5;
	    pBoard->stParam.OutParams[5].OutputMode = eCdce62005OutModeLvPecl;    // Virtual output pin type is lvpecl                                         
    }
    else
    {
        pBoard->stParam.OutParams[5].uOutputFreq = 0;
	    pBoard->stParam.OutParams[5].OutputMode = eCdce62005OutModeDisabled;  // Virtual output pin type is disabled                                                               
    }
    pBoard->stParam.RefInput.SecondaryInput.uInputFrequency=uSecondaryRef;    // Secondary reference configuration

	// Calculates the pll parameters
    Ret = Cdce62005_Calculate(&pBoard->stParam);

    // Did we get any errors?
    if(SUCCESS(Ret))
    {
        // Clocks synthetization and loop filter params were both ok
        pBoard->iSynthDone = 1;
        pBoard->iLoopFilterDone = 1;
    }
    else
    {
        if (Ret == eCdce62005NoFilterSolution)
        {
            // Clock synth is ok, but missing loop filter params
            pBoard->iSynthDone = 1;
        }
    }
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_WriteClockConfig(Mo1000_stMo1000 * pBoard)
{
    Result Ret = eMo1000Ok;
    int iCount;

    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit19);
    }

    // Check if a valid synthetized clock setup was provided
    if (!pBoard->iSynthDone)
    {
        FUNC_EXIT();
        return(eMo1000NoClockSetup01);
    }

    // Check if correct pll loop filter parameters were provided 
    if (!pBoard->iLoopFilterDone)
    {
        FUNC_EXIT();
        return(eMo1000NoClockLoopFilter);
    }

    // Turn off synchronize on each dac device
    for (iCount = 0; iCount < MO1000_DAC_PER_BOARD && SUCCESS(Ret); iCount++)
    {
        // Disable synchronisation mode before killing the clocks
        Ret = Ad9148_SetSyncCtrl(&pBoard->Dac, iCount, eAd9148SyncDisable);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    }

    // Freeze the outputs
    Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOn);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
	// Set the pll according to the calculated parameters
    Ret = Cdce62005_SetParameters(&pBoard->Pll, &pBoard->stParam, 0);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Complete the output sync operation
    Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    //add fixed delay to let the synchronisation to complete.
    Carrier_usleep(pBoard->hCarrier, MO1000_PLLSYNC_DELAY);
    
    // Reset the clock mcm after we changed the clock source and frequencies
    Ret = _Mo1000_ClkReset(pBoard);

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_GetPllConfig(Mo1000_stMo1000 * pBoard, Mo1000_stPllParam *pParam)
{
    Result Ret = eMo1000Ok;

    FUNC_ENTER();

    // Everything to 0
    memset(pParam, 0 , sizeof(Mo1000_stPllParam));
    
    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit20);
    }

    // Check if a valid synthetized clock setup was provided
    if (!pBoard->iSynthDone)
    {
        FUNC_EXIT();
        return(eMo1000NoClockSetup02);
    }

    // Recover currently pll calculated values
    pParam->uPrimRefFreq = pBoard->stParam.RefInput.PrimaryInput.uInputFrequency;
    pParam->uSecRefFreq = pBoard->stParam.RefInput.SecondaryInput.uInputFrequency;
    pParam->OutputRef0 = pBoard->stParam.OutParams[0].OutputRef;
    pParam->OutputRef1 = pBoard->stParam.OutParams[1].OutputRef;
    pParam->OutputRef2 = pBoard->stParam.OutParams[2].OutputRef;
    pParam->OutputRef3 = pBoard->stParam.OutParams[3].OutputRef;
    pParam->OutputRef4 = pBoard->stParam.OutParams[4].OutputRef;
    pParam->uVcoFreq = pBoard->stParam.uVcoFreq;
    pParam->ucPrimaryInputDivider = pBoard->stParam.ucPrimaryInputDivider;
    pParam->usReferenceDivider = pBoard->stParam.usReferenceDivider;
    pParam->usInputDivider = pBoard->stParam.usInputDivider;
    pParam->usFeedbackDivider = pBoard->stParam.usFeedbackDivider;
    pParam->ucBypassDivider = pBoard->stParam.ucBypassDivider;
    pParam->ucPrescaleDivider = pBoard->stParam.ucPrescaleDivider;
    pParam->R2Value = pBoard->stParam.R2Value;
    pParam->R3Value = pBoard->stParam.R3Value;
    pParam->C1Value = pBoard->stParam.C1Value;
    pParam->C2Value = pBoard->stParam.C2Value;
    pParam->C3Value = pBoard->stParam.C3Value;
    pParam->ChargePump = pBoard->stParam.ChargePump;
    pParam->aucOutDivider0 = pBoard->stParam.aucOutDivider[0];
    pParam->aucOutDivider1 = pBoard->stParam.aucOutDivider[1];
    pParam->aucOutDivider2 = pBoard->stParam.aucOutDivider[2];
    pParam->aucOutDivider3 = pBoard->stParam.aucOutDivider[3];
    pParam->aucOutDivider4 = pBoard->stParam.aucOutDivider[4];
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetPllConfig(Mo1000_stMo1000 * pBoard, Mo1000_stPllParam *pParam)
{
    Result Ret = eMo1000Ok;

    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit21);
    }

    // Check if a valid synthetized clock setup was provided
    if (!pBoard->iSynthDone)
    {
        FUNC_EXIT();
        return(eMo1000NoClockSetup03);
    }

    // Set newly pll calculated values
    pBoard->stParam.uVcoFreq = pParam->uVcoFreq;
    pBoard->stParam.ucPrimaryInputDivider = pParam->ucPrimaryInputDivider;
    pBoard->stParam.usReferenceDivider = pParam->usReferenceDivider;
    pBoard->stParam.usInputDivider = pParam->usInputDivider;
    pBoard->stParam.usFeedbackDivider = pParam->usFeedbackDivider;
    pBoard->stParam.ucBypassDivider = pParam->ucBypassDivider;
    pBoard->stParam.ucPrescaleDivider = pParam->ucPrescaleDivider;
    pBoard->stParam.R2Value = pParam->R2Value;
    pBoard->stParam.R3Value = pParam->R3Value;
    pBoard->stParam.C1Value = pParam->C1Value;
    pBoard->stParam.C2Value = pParam->C2Value;
    pBoard->stParam.C3Value = pParam->C3Value;
    pBoard->stParam.ChargePump = pParam->ChargePump;
    pBoard->stParam.aucOutDivider[0] = pParam->aucOutDivider0;
    pBoard->stParam.aucOutDivider[1] = pParam->aucOutDivider1;
    pBoard->stParam.aucOutDivider[2] = pParam->aucOutDivider2;
    pBoard->stParam.aucOutDivider[3] = pParam->aucOutDivider3;
    pBoard->stParam.aucOutDivider[4] = pParam->aucOutDivider4;
    
    // This means that a new filter configuration was programmed
    pBoard->iLoopFilterDone = 1;
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_GetChannelCalibStatus(Mo1000_stMo1000 * pBoard, unsigned int *puChannelLaneCalib,
                                    unsigned int *puChannelFrameCalib, unsigned int *puChannelSyncCalib,
                                    unsigned int *puCalibStatus)
{
    Result Ret = eMo1000Ok;
#if MO1000_MODDEBUG
    int iIndx;
#endif

    FUNC_ENTER();

    // Get the current lane calibration status for the active channels
    *puChannelLaneCalib = (pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] & pBoard->uChannelLaneMask);

    // Get the current frame calibration status for the active channels
    *puChannelFrameCalib = (pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] & pBoard->uChannelLaneMask);

    // Get the current sync calibration status for the active channels
    *puChannelSyncCalib = ((~(pBoard->iSyncCalibDone)) & pBoard->uChannelLaneMask);

#if MO1000_MODDEBUG
    for (iIndx = 0; iIndx <= pBoard->uCalibNdx; iIndx++)
            PDEBUG("Trial #%02d, data calib status: 0x%08X.\n",iIndx, (pBoard->auChannelsLaneCalib[iIndx] & pBoard->uChannelLaneMask));

    for (iIndx = 0; iIndx <= pBoard->uCalibFrameNdx; iIndx++)
            PDEBUG("Trial #%02d, frame calib status: 0x%08X.\n",iIndx, (pBoard->auFrameLaneCalib[iIndx] & pBoard->uChannelLaneMask));

    for (iIndx = 0; iIndx <= pBoard->uCalibSyncNdx; iIndx++)
            PDEBUG("Trial #%02d, sync calib error: 0x%08X.\n",iIndx, (pBoard->auSyncCalib[iIndx]));
#endif

    // Keep trace if the whole calib process was made correctly or not
    *puCalibStatus = pBoard->iCalibDone;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacOutCtrl(Mo1000_stMo1000 * pBoard, Mo1000_eDacSelect Channel,
                            Mo1000_eOutputState State)
{
    Result Ret = eMo1000Ok;
    Mo1000DevCom_eDevices DacDev;
    Ad9148_eDacSelect Select;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit07);
    }

    // Channel validation
    switch(Channel)
    {
        case eMo1000SelectDac1:
        case eMo1000SelectDac2:
        case eMo1000SelectDac3:
        case eMo1000SelectDac4:
        case eMo1000SelectDac5:
        case eMo1000SelectDac6:
        case eMo1000SelectDac7:
        case eMo1000SelectDac8:
            DacDev = Channel / AD9148_CHANNELS_PER_DAC;
            Select = Channel - DacDev * AD9148_CHANNELS_PER_DAC;
            break;
        
        default:
            Ret = eMo1000InvalidChannel01;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // State validation
    switch(State)
    {
        case eMo1000OutputEnable:
        case eMo1000OutputDisable:
            break;
        
        default:
            Ret = eMo1000InvalidState01;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Set dac output control according to what user wants
    Ret = Ad9148_SetOutCtrl(&pBoard->Dac, DacDev, Select, State);

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParInterpolation(Mo1000_stMo1000 * pBoard, Ad9148_eInterpolationMode Mode)
{
    Result Ret = eMo1000Ok;
    int iCount;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit08);
    }

    // Mode validation
    switch(Mode)
    {
        case eAd9148Inter1x:
        case eAd9148Inter2x:
        case eAd9148Inter2x_fsdiv4:
        case eAd9148Inter2x_fsdiv2:
        case eAd9148Inter2x_3fsdiv4:
        case eAd9148Inter4x:
        case eAd9148Inter4x_fsdiv8:
        case eAd9148Inter4x_fsdiv4:
        case eAd9148Inter4x_3fsdiv8:
        case eAd9148Inter4x_fsdiv2:
        case eAd9148Inter4x_5fsdiv8:
        case eAd9148Inter4x_3fsdiv4:
        case eAd9148Inter4x_7fsdiv8:
        case eAd9148Inter8x:
        case eAd9148Inter8x_fsdiv8:
        case eAd9148Inter8x_fsdiv4:
        case eAd9148Inter8x_3fsdiv8:
        case eAd9148Inter8x_fsdiv2:
        case eAd9148Inter8x_5fsdiv8:
        case eAd9148Inter8x_3fsdiv4:
        case eAd9148Inter8x_7fsdiv8:
            break;
        
        default:
            Ret = eMo1000InvalidInterMode;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Configure each dac device
    for (iCount = 0; iCount < MO1000_DAC_PER_BOARD; iCount++)
    {
        // Saves interpolation mode
        pBoard->DacConfig[iCount].InterMode = Mode;
        
        // Configure dac interpolation mode
        Ret = Ad9148_SetInterpolation(&pBoard->Dac, iCount, Mode);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Shows that interpolation mode has been updated
        pBoard->DacConfig[iCount].ucInterUpdate = 1;
    }

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParDcOffset(Mo1000_stMo1000 * pBoard, Mo1000_eDacSelect Channel,
                                short sOffset)
{
    Result Ret = eMo1000Ok;
    Mo1000DevCom_eDevices DacDev;
    Ad9148_eDacSelect Select;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit09);
    }

    // Channel validation
    switch(Channel)
    {
        case eMo1000SelectDac1:
        case eMo1000SelectDac2:
        case eMo1000SelectDac3:
        case eMo1000SelectDac4:
        case eMo1000SelectDac5:
        case eMo1000SelectDac6:
        case eMo1000SelectDac7:
        case eMo1000SelectDac8:
            DacDev = Channel / AD9148_CHANNELS_PER_DAC;
            Select = Channel - DacDev * AD9148_CHANNELS_PER_DAC;
            break;
        
        default:
            Ret = eMo1000InvalidChannel02;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Saves dc offset
    pBoard->DacConfig[DacDev].sDcOffset[Select] = sOffset;
    
    // Set dac output dc offset according to what user wants
    Ret = Ad9148_SetDcOffset(&pBoard->Dac, DacDev, Select, sOffset);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Shows that dc offset has been updated
    pBoard->DacConfig[DacDev].ucDcOffsetUpdate[Select] = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParGain(Mo1000_stMo1000 * pBoard, Mo1000_eDacSelect Channel,
                            unsigned char ucGain)
{
    Result Ret = eMo1000Ok;
    Mo1000DevCom_eDevices DacDev;
    Ad9148_eDacSelect Select;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit10);
    }

    // Channel validation
    switch(Channel)
    {
        case eMo1000SelectDac1:
        case eMo1000SelectDac2:
        case eMo1000SelectDac3:
        case eMo1000SelectDac4:
        case eMo1000SelectDac5:
        case eMo1000SelectDac6:
        case eMo1000SelectDac7:
        case eMo1000SelectDac8:
            DacDev = Channel / AD9148_CHANNELS_PER_DAC;
            Select = Channel - DacDev * AD9148_CHANNELS_PER_DAC;
            break;
        
        default:
            Ret = eMo1000InvalidChannel03;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Saves digital gain offset
    pBoard->DacConfig[DacDev].ucGain[Select] = ucGain;
    
    // Set dac output digital gain according to what user wants
    Ret = Ad9148_SetGain(&pBoard->Dac, DacDev, Select, ucGain);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Shows that digital gain has been updated
    pBoard->DacConfig[DacDev].ucGainUpdate[Select] = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParActChannel(Mo1000_stMo1000 * pBoard, Mo1000_eActChannel ActChannel)
{
    Result Ret = eMo1000Ok;
    int iCount, iPair;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit11);
    }

    // Channel validation
    switch(ActChannel)
    {
        case eMo1000ActChannels08:
            pBoard->DacConfig[eMo1000DevComDeviceDac1].ActPairState[eAd9148SelectIQDac12] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac1].ActPairState[eAd9148SelectIQDac34] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac2].ActPairState[eAd9148SelectIQDac12] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac2].ActPairState[eAd9148SelectIQDac34] = eAd9148DacPairEnable;
            pBoard->pCore->stControl.stBit.m_uDataSerdesPd = 0;
    
            // Set mask use to check calibration (8 channels = 2 dacs)
            pBoard->uChannelLaneMask = 0xffffffff;
            break;

        case eMo1000ActChannels06:
            pBoard->DacConfig[eMo1000DevComDeviceDac1].ActPairState[eAd9148SelectIQDac12] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac1].ActPairState[eAd9148SelectIQDac34] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac2].ActPairState[eAd9148SelectIQDac12] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac2].ActPairState[eAd9148SelectIQDac34] = eAd9148DacPairDisable;
            pBoard->pCore->stControl.stBit.m_uDataSerdesPd = 0xc0;
    
            // Set mask use to check calibration (6 channels = 2 dacs)
            pBoard->uChannelLaneMask = 0xffffffff;
            break;

        case eMo1000ActChannels04:
            pBoard->DacConfig[eMo1000DevComDeviceDac1].ActPairState[eAd9148SelectIQDac12] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac1].ActPairState[eAd9148SelectIQDac34] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac2].ActPairState[eAd9148SelectIQDac12] = eAd9148DacPairDisable;
            pBoard->DacConfig[eMo1000DevComDeviceDac2].ActPairState[eAd9148SelectIQDac34] = eAd9148DacPairDisable;
            pBoard->pCore->stControl.stBit.m_uDataSerdesPd = 0xf0;
    
            // Set mask use to check calibration (4 channels = 1 dac)
            pBoard->uChannelLaneMask = 0x0000ffff;
            break;
            
        case eMo1000ActChannels02:
            pBoard->DacConfig[eMo1000DevComDeviceDac1].ActPairState[eAd9148SelectIQDac12] = eAd9148DacPairEnable;
            pBoard->DacConfig[eMo1000DevComDeviceDac1].ActPairState[eAd9148SelectIQDac34] = eAd9148DacPairDisable;
            pBoard->DacConfig[eMo1000DevComDeviceDac2].ActPairState[eAd9148SelectIQDac12] = eAd9148DacPairDisable;
            pBoard->DacConfig[eMo1000DevComDeviceDac2].ActPairState[eAd9148SelectIQDac34] = eAd9148DacPairDisable;
            pBoard->pCore->stControl.stBit.m_uDataSerdesPd = 0xfc;
    
            // Set mask use to check calibration (2 channels = 1 dac)
            pBoard->uChannelLaneMask = 0x0000ffff;
            break;
            
        
        default:
            Ret = eMo1000InvalidActChannel;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Check if the calibration is still valid (not valid if the number of active channels is greater than before)
    if (ActChannel > pBoard->GroupCh)
    {
        // Keep trace of the number of calibration trials
        pBoard->uCalibNdx = 0;
    
        // Keep trace of the channels lane that were calibrated correctly the last time it was done
        pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] = 0xffffffff;
        
        // Keep trace of the number of frame calibration trials
        pBoard->uCalibFrameNdx = 0;
    
        // Keep trace of the frame calibration errors
        pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = 0xffffffff;
    
        // Keep trace of the number of sync calibration trials
        pBoard->uCalibSyncNdx = 0;
    
        // Keep trace of the sync calibration errors
        pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;
    
        // Keep trace if calib was made correctly or not
        pBoard->iCalibDone = 0;
        pBoard->iDataCalibDone = 0;
        pBoard->iFrameCalibDone = 0;
        pBoard->iSyncCalibDone = 0;

        // Set all channels not valid to force a calibration (because some channels could be uncalibrated)
        pBoard->pCore->stControl.stBit.m_uDacReady = 0;
    }
    else
    {
        if (pBoard->iCalibDone == 1)
            // Set channels not in powered down to valid
            pBoard->pCore->stControl.stBit.m_uDacReady = (0xff & ~(0xfc << (ActChannel *2)));
        else
            // Set all channels not valid
            pBoard->pCore->stControl.stBit.m_uDacReady = 0;
    }

    // Update DacReady values
    pBoard->pCore->stControl.stBit.m_uUpdateDacReady = 1;
    // Saves the number of active channels
    pBoard->GroupCh = ActChannel;

    // Configure each dac device
    for (iCount = 0; iCount < MO1000_DAC_PER_BOARD; iCount++)
    {
        // For each dac pair
        for (iPair = 0; iPair < AD9148_CHANNELS_PER_DAC/2; iPair++)
        {
            Ret = Ad9148_SetActChannel(&pBoard->Dac, iCount, iPair, pBoard->DacConfig[iCount].ActPairState[iPair]);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            pBoard->DacConfig[iCount].ucActPairStateUpdate[iPair] = 1;
        }
    }

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_GetClkMaster(Mo1000_stMo1000 * pBoard, unsigned int *puMaster)
{
    Result Ret = eMo1000Ok;

    FUNC_ENTER();

    // Check if this fpga core instance is the clock master or not in a multiple boards configuration
    *puMaster = pBoard->pCore->stStatus.stBit.m_uClockMaster;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParISinc(Mo1000_stMo1000 * pBoard, Mo1000_eSelectDacPair DacPair,
                             short sRealC0, short sRealC1, short sRealC2, short sRealC3, short sRealC4,
                             short sImgC0, short sImgC1, short sImgC2, short sImgC3, short sImgC4)
{
    Result Ret = eMo1000Ok;
    Mo1000DevCom_eDevices DacDev;
    Ad9148_eDacSelect Select;
    Ad9148_stISincCoefficients stReal, stImaginary;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit12);
    }

    // Dac pair validation
    switch(DacPair)
    {
        case eMo1000SelectIQDac12:
        case eMo1000SelectIQDac34:
        case eMo1000SelectIQDac56:
        case eMo1000SelectIQDac78:
            DacDev = 2 * DacPair / AD9148_CHANNELS_PER_DAC;
            Select = DacPair - 2 * DacDev;
            break;
        
        default:
            Ret = eMo1000InvalidDacPair01;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Coefficients validation
    if (sRealC0 > 3 || sRealC0 < -4)
    {
        Ret = eMo1000BadIsincRealC0;
        FUNC_EXIT();
        return Ret;
    }
    if (sRealC1 > 7 || sRealC1 < -8)
    {
        Ret = eMo1000BadIsincRealC1;
        FUNC_EXIT();
        return Ret;
    }
    if (sRealC2 > 15 || sRealC2 < -16)
    {
        Ret = eMo1000BadIsincRealC2;
        FUNC_EXIT();
        return Ret;
    }
    if (sRealC3 > 63 || sRealC3 < -64)
    {
        Ret = eMo1000BadIsincRealC3;
        FUNC_EXIT();
        return Ret;
    }
    if (sRealC4 > 511 || sRealC4 < -512)
    {
        Ret = eMo1000BadIsincRealC4;
        FUNC_EXIT();
        return Ret;
    }
    if (sImgC0 > 3 || sImgC0 < -4)
    {
        Ret = eMo1000BadIsincImgC0;
        FUNC_EXIT();
        return Ret;
    }
    if (sImgC1 > 7 || sImgC1 < -8)
    {
        Ret = eMo1000BadIsincImgC1;
        FUNC_EXIT();
        return Ret;
    }
    if (sImgC2 > 15 || sImgC2 < -16)
    {
        Ret = eMo1000BadIsincImgC2;
        FUNC_EXIT();
        return Ret;
    }
    if (sImgC3 > 63 || sImgC3 < -64)
    {
        Ret = eMo1000BadIsincImgC3;
        FUNC_EXIT();
        return Ret;
    }
    if (sImgC4 > 511 || sImgC4 < -512)
    {
        Ret = eMo1000BadIsincImgC4;
        FUNC_EXIT();
        return Ret;
    }

    // Merge the coefficients in the structures
    stReal.stBit.m_iC0 = sRealC0;
    stReal.stBit.m_iC1 = sRealC1;
    stReal.stBit.m_iC2 = sRealC2;
    stReal.stBit.m_iC3 = sRealC3;
    stReal.stBit.m_iC4 = sRealC4;

    stImaginary.stBit.m_iC0 = sImgC0;
    stImaginary.stBit.m_iC1 = sImgC1;
    stImaginary.stBit.m_iC2 = sImgC2;
    stImaginary.stBit.m_iC3 = sImgC3;
    stImaginary.stBit.m_iC4 = sImgC4;
    
    // Saves isinc coefficients
    pBoard->DacConfig[DacDev].stISincReal[Select] = stReal;
    pBoard->DacConfig[DacDev].stISincImaginary[Select] = stImaginary;
    
    // Set dac inverse sinc filter options according to what user wants
    Ret = Ad9148_SetISinc(&pBoard->Dac, DacDev, Select, stReal, stImaginary);

    // Shows that isinc options have been updated
    pBoard->DacConfig[DacDev].ucISincUpdate[Select] = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParISincCtrl(Mo1000_stMo1000 * pBoard, Mo1000_eDacGroupId Group,
                                 Mo1000_eIsincState State)
{
    Result Ret = eMo1000Ok;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit13);
    }

    // Dac group validation
    switch(Group)
    {
        case eMo1000DacGroup14:
        case eMo1000DacGroup58:
            break;
        
        default:
            Ret = eMo1000InvalidDacGroup01;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // State validation
    switch(State)
    {
        case eMo1000IsincEnable:
        case eMo1000IsincDisable:
            break;
        
        default:
            Ret = eMo1000InvalidState02;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Saves isinc state
    pBoard->DacConfig[Group].ISincState = State;
    
    // Set dac inverse sinc filter state according to what user wants
    Ret = Ad9148_SetISincCtrl(&pBoard->Dac, Group, State);

    // Shows that isinc state has been updated
    pBoard->DacConfig[Group].ucISincStateUpdate = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParFineMod(Mo1000_stMo1000 * pBoard, Mo1000_eDacGroupId Group, unsigned uFtw,
                               unsigned short usNcoPhaseOffset, Mo1000_eNcoSideband Sideband)
{
    Result Ret = eMo1000Ok;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit14);
    }

    // Dac group validation
    switch(Group)
    {
        case eMo1000DacGroup14:
        case eMo1000DacGroup58:
            break;
        
        default:
            Ret = eMo1000InvalidDacGroup02;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Sideband validation
    switch(Sideband)
    {
        case eMo1000NcoHighside:
        case eMo1000NcoLowside:
            break;
        
        default:
            Ret = eMo1000InvalidSideBand;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Saves fine modulation options
    pBoard->DacConfig[Group].uFtw = uFtw;
    pBoard->DacConfig[Group].usNcoPhaseOffset = usNcoPhaseOffset;
    pBoard->DacConfig[Group].Sideband = Sideband;
    
    // Set fine modulation options according to what user wants
    Ret = Ad9148_SetFineMod(&pBoard->Dac, Group, uFtw, usNcoPhaseOffset, Sideband);

    // Shows that fine modulation options have been updated
    pBoard->DacConfig[Group].ucFineModUpdate = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParFineModCtrl(Mo1000_stMo1000 * pBoard, Mo1000_eDacGroupId Group,
                                   Mo1000_eFineModState State)
{
    Result Ret = eMo1000Ok;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit15);
    }

    // Dac group validation
    switch(Group)
    {
        case eMo1000DacGroup14:
        case eMo1000DacGroup58:
            break;
        
        default:
            Ret = eMo1000InvalidDacGroup03;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // State validation
    switch(State)
    {
        case eMo1000FineModEnable:
        case eMo1000FineModDisable:
            break;
        
        default:
            Ret = eMo1000InvalidNCOState;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Saves fine modulation control state
    pBoard->DacConfig[Group].FineModState = State;
    
    // Set dac fine modulation state according to what user wants
    Ret = Ad9148_SetFineModCtrl(&pBoard->Dac, Group, State);

    // Shows that fine modulation control state has been updated
    pBoard->DacConfig[Group].ucFineModStateUpdate = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParDataFormat(Mo1000_stMo1000 * pBoard, Mo1000_eDacGroupId Group,
                                  Mo1000_eDataFormat Format)
{
    Result Ret = eMo1000Ok;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit16);
    }

    // Dac group validation
    switch(Group)
    {
        case eMo1000DacGroup14:
        case eMo1000DacGroup58:
            break;
        
        default:
            Ret = eMo1000InvalidDacGroup04;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Format validation
    switch(Format)
    {
        case eMo1000Format2complement:
        case eMo1000FormatBinary:
            break;
        
        default:
            Ret = eMo1000InvalidDataFormat;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Saves data format
    pBoard->DacConfig[Group].DataFormat = Format;
    
    // Set dac data format according to what user wants
    Ret = Ad9148_SetDataFormat(&pBoard->Dac, Group, Format);

    // Shows that data format has been updated
    pBoard->DacConfig[Group].ucDataFormatUpdate = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetDacParQuadPhase(Mo1000_stMo1000 * pBoard, Mo1000_eSelectDacPair DacPair,
                                 short sRealPhase, short sImaginaryPhase)
{
    Result Ret = eMo1000Ok;
    Mo1000DevCom_eDevices DacDev;
    Ad9148_eDacSelect Select;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit17);
    }

    // Dac pair validation
    switch(DacPair)
    {
        case eMo1000SelectIQDac12:
        case eMo1000SelectIQDac34:
        case eMo1000SelectIQDac56:
        case eMo1000SelectIQDac78:
            DacDev = 2 * DacPair / AD9148_CHANNELS_PER_DAC;
            Select = DacPair - 2 * DacDev;
            break;
        
        default:
            Ret = eMo1000InvalidDacPair02;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Saves dac quadrature phase
    pBoard->DacConfig[DacDev].sRealPhase[Select] = sRealPhase;
    pBoard->DacConfig[DacDev].sImaginaryPhase[Select] = sImaginaryPhase;
    
    // Set dac quadrature phase options according to what user wants
    Ret = Ad9148_SetQuadPhase(&pBoard->Dac, DacDev, Select, sRealPhase, sImaginaryPhase);

    // Shows that dac quadrature phase options have been updated
    pBoard->DacConfig[DacDev].ucQuadPhaseUpdate[Select] = 1;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_GetVersions(Mo1000_stMo1000 * pBoard,  unsigned int* puCoreVersion, unsigned int *puDriverVersion)
{
    Result Ret = eMo1000Ok;
    
    FUNC_ENTER();

    // Get the MO1000 fpga hardware core version
    *puCoreVersion = pBoard->pCore->stInfo.stBit.m_uCoreVersion;

    // Get the MO1000 software driver version
    *puDriverVersion = MO1000_DVERSION;

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestLocalOscSw(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    unsigned uLock;
    Expander_eLocalOscSw Sw;
    Expander_ePllLock PllLock;
#endif
    
    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been reset correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset02);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST

    // Setup the pll for the test configuration (primary input as pll reference)
    Ret = _Mo1000_SetupTestPll(pBoard, 0);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Wait for pll to lock
    Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Read clock pll lock status
    Ret = Expander_GetPllLock(&pBoard->Expander, &PllLock);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Did we got a pll lock
    if (uLock)
    {
        // Check if pll locked pin is also locked
        if (PllLock == ePllLockOn)
        {
            // Set local osc switch off
            Sw = eLocalOscSwOff;
            Ret = Expander_SetLocalOscSw(&pBoard->Expander, Sw);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // Wait for pll to lock
            Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // Read clock pll lock status
            Ret = Expander_GetPllLock(&pBoard->Expander, &PllLock);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

            // Did we got a pll lock
            if (uLock)
            {
                // Error, primary 125MHz reference is still on
                *puResult = 2;
            }
            else
            {
                // Check if pll locked pin is also locked
                if (PllLock == ePllLockOn)
                {
                    // Problem pll lock pin is stucked high
                    *puResult = 4;
                }
            }
        }
        else
        {
            // Problem pll lock pin is stucked low
            *puResult = 3;
        }
    }
    else
    {
        // Error, primary 125MHz reference is not ok
        *puResult = 1;
    }
#endif
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestBridgeReset(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    unsigned char ucData, ucData1;
#endif
    
    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been resetted correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset03);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST

    // Initialize the io expander to operating condition for all devices
    Ret = Expander_Init(&pBoard->Expander);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait a small bridge reset time
    Carrier_usleep(pBoard->hCarrier, MO1000_BRIDGESTART_DELAY);

    // Setup bridge SS3 pin gpio mode
    ucData1 = 0x08;
    Ret = Mo1000DevCom_Write(&pBoard->Com, eMo1000DevComDeviceI2cBridge, 0xF6, &ucData1);
    if(FAILURE(Ret))
    {
         if (Ret == eDevComFailed03)
         {
            // Error, bridge kept in reset
            *puResult = 3;
            return eMo1000Ok;
         }
         else
            return Ret;
    }
        
    // Setup bridge SS3 pin to quasi-bidir mode
    ucData1 = 0x00;
    Ret = Mo1000DevCom_Write(&pBoard->Com, eMo1000DevComDeviceI2cBridge, 0xF7, &ucData1);
    if(FAILURE(Ret)) return Ret;
        
    // Read gpio pins
    ucData = 255;
    Ret = Mo1000DevCom_Read(&pBoard->Com, eMo1000DevComDeviceI2cBridge, 0xF5, &ucData);
    if(FAILURE(Ret)) return Ret;
        
    // Setup bridge SS3 pin to push-pull mode
    ucData1 = 0x40;
    Ret = Mo1000DevCom_Write(&pBoard->Com, eMo1000DevComDeviceI2cBridge, 0xF7, &ucData1);
    if(FAILURE(Ret)) return Ret;
        
    // Write gpio pins to inverse of the pin state read before
    ucData = ucData ^ 0x8;
    Ret = Mo1000DevCom_Write(&pBoard->Com, eMo1000DevComDeviceI2cBridge, 0xF4, &ucData);
    if(FAILURE(Ret)) return Ret;
        
    // Read gpio pins
    ucData1 = ucData ^ 0x8;
    Ret = Mo1000DevCom_Read(&pBoard->Com, eMo1000DevComDeviceI2cBridge, 0xF5, &ucData1);
    if(FAILURE(Ret)) return Ret;
        
    // Check if pin current state is what we have written
    if (ucData1 == ucData)
    {
        // Resets the bridge
        Ret = Expander_SetBridgeReset(&pBoard->Expander, eBridgeResetOn);
        if(FAILURE(Ret)) return Ret;

        // Wait a small bridge reset time
        Carrier_usleep(pBoard->hCarrier, MO1000_BRIDGESTART_DELAY);

        // Get the bridge out of reset
        Ret = Expander_SetBridgeReset(&pBoard->Expander, eBridgeResetOff);
        if(FAILURE(Ret)) return Ret;

        // Wait a small bridge get out of reset delay (device ready after reset not specified by manufacturer)
        Carrier_usleep(pBoard->hCarrier, MO1000_BRIDGESTART_DELAY);

        // Read gpio pins
        Ret = Mo1000DevCom_Read(&pBoard->Com, eMo1000DevComDeviceI2cBridge, 0xF5, &ucData1);
        if(FAILURE(Ret)) return Ret;
        
        // Are we still having the same result (must be different if bridge reset worked)
        if (ucData1 == ucData)
        {
            // Error, cannot reset bridge
            *puResult = 2;
        }
    }
    else
    {
        // Error, cannot setup bridge for reset test
        *puResult = 1;
    }
#endif
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestDacReset(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    Ad9148_stRegisterMap stReg, stReg1;
#endif

    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been resetted correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset04);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST

    // Initialize the io expander to operating condition for all devices
    Ret = Expander_Init(&pBoard->Expander);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait a small bridge get out of reset delay (device ready after reset not specified by manufacturer)
    Carrier_usleep(pBoard->hCarrier, MO1000_BRIDGESTART_DELAY);

    // Initialize the spi bridge (for operation)
    // At this moment we are also sure that all spi cs lines are correctly set to 1
    // We are doing 2 bridge initialization tentatives because to be successfull always on the 1st trial,
    // we need to wait at least around 500 mSec after bridge reset is released which is slow. This technic
    // is faster...
    Ret = Mo1000DevCom_Init(&pBoard->Com);
    if(FAILURE(Ret))
    {
        // Retry to configure the bridge which now must be ready
        Ret = Mo1000DevCom_Init(&pBoard->Com);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    }

    // Release dacs reset
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac1, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac2, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Wait small delay to make sure dacs are out of reset
    Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);

    // Write bank select bit
    stReg.stA00Comm.m_ucRaw = 0;
    stReg.stA00Comm.stBit.m_ucDacSpiSelect = 1;
    Ret = Ad9148_WriteReg(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148Addr00, stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_WriteReg(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148Addr00, stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    stReg1.stA00Comm.m_ucRaw = 0;
    Ret = Ad9148_ReadReg(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148Addr00, &stReg1.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    stReg.stA00Comm.m_ucRaw = 0;
    Ret = Ad9148_ReadReg(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148Addr00, &stReg.stA00Comm.m_ucRaw);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Check back the bit we have written for Dac1
    if (stReg1.stA00Comm.stBit.m_ucDacSpiSelect == 1)
    {
        // Checks if device is stuck in reset
        if (stReg1.stA00Comm.m_ucRaw == 255)
        {
            // Problem, dac1 is stucked in reset
            *puResult = 6;
            FUNC_EXIT();
            return Ret;            
        }
        
        // Sets dac1 reset
        Ret = Expander_SetDacReset(&pBoard->Expander, eDac1, eDacResetOn);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Wait small delay to make sure dacs are out of reset
        Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
    
        // Release dac1 reset
        Ret = Expander_SetDacReset(&pBoard->Expander, eDac1, eDacResetOff);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Wait small delay to make sure dacs are out of reset
        Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
    
        Ret = Ad9148_ReadReg(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148Addr00, &stReg1.stA00Comm.m_ucRaw);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Check back the bit we have written in Dac1
        if (stReg1.stA00Comm.stBit.m_ucDacSpiSelect == 0)
        {
            // Check dac2 bank select bit was correctly written
            if (stReg.stA00Comm.stBit.m_ucDacSpiSelect == 1)
            {
                // Checks if device is stuck in reset
                if (stReg.stA00Comm.m_ucRaw == 255)
                {
                    // Problem, dac2 is stucked in reset
                    *puResult = 7;
                    FUNC_EXIT();
                    return Ret;            
                }
        
                stReg.stA00Comm.m_ucRaw = 0;
                Ret = Ad9148_ReadReg(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148Addr00, &stReg.stA00Comm.m_ucRaw);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

                // Check dac2 bank select bit was correctly written
                if (stReg.stA00Comm.stBit.m_ucDacSpiSelect == 1)
                {
                    // Sets dac2 reset
                    Ret = Expander_SetDacReset(&pBoard->Expander, eDac2, eDacResetOn);
                    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
                    // Wait small delay to make sure dacs are out of reset
                    Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
                
                    // Release dac2 reset
                    Ret = Expander_SetDacReset(&pBoard->Expander, eDac2, eDacResetOff);
                    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
                    // Wait small delay to make sure dacs are out of reset
                    Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
                
                    Ret = Ad9148_ReadReg(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148Addr00, &stReg.stA00Comm.m_ucRaw);
                    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            
                    // Check back the bit we have written
                    if (stReg.stA00Comm.stBit.m_ucDacSpiSelect != 0)
                    {
                        // Problem, cannot reset dac2 correctly
                        *puResult = 5;
                    }
                }
                else
                {
                    // Problem, dac2 incorrectly resets with dac1
                    *puResult = 4;
                }
            }
            else
            {
                // Problem, cannot write to dac2 correctly
                *puResult = 3;
            }
        }
        else
        {
            // Problem, cannot reset dac1 correctly
            *puResult = 2;
        }
    }
    else
    {
        // Problem, cannot write to dac1 correctly
        *puResult = 1;
    }
#endif
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestPllSel(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    unsigned uLock;
#endif
    
    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been resetted correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset05);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST

    // Setup the pll for the test configuration (secondary input as pll reference)
    Ret = _Mo1000_SetupTestPll(pBoard, 1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Configure the clock reference our own clock
    Ret = Expander_SetClkReference(&pBoard->Expander, eMasterRef);

    // Complete the output sync operation
    Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Wait for pll to lock
    Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Did we got a pll lock
    if (uLock)
    {
        // Configure the clock reference from bottom board (this will remove our secondary reference)
        Ret = Expander_SetClkReference(&pBoard->Expander, eMasterClk);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Wait for pll to lock
        Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Did we got a pll lock
        if (uLock)
        {
            // Error, secondary 125MHz reference is still on
            *puResult = 2;
        }
    }
    else
    {
        // Error, secondary 125MHz reference is not ok
        *puResult = 1;
    }
#endif

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestPllPowDown(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    unsigned char ucAddr = eCdce62005Addr08;
    unsigned uVal1, uVal2;
#endif
    
    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been resetted correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset06);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST

    // Initialize the io expander to operating condition for all devices
    Ret = Expander_Init(&pBoard->Expander);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait a small bridge get out of reset delay (device ready after reset not specified by manufacturer)
    Carrier_usleep(pBoard->hCarrier, MO1000_BRIDGESTART_DELAY);

    // Initialize the spi bridge (for operation)
    // At this moment we are also sure that all spi cs lines are correctly set to 1
    // We are doing 2 bridge initialization tentatives because to be successfull always on the 1st trial,
    // we need to wait at least around 500 mSec after bridge reset is released which is slow. This technic
    // is faster...
    Ret = Mo1000DevCom_Init(&pBoard->Com);
    if(FAILURE(Ret))
    {
        // Retry to configure the bridge which now must be ready
        Ret = Mo1000DevCom_Init(&pBoard->Com);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    }

    // Set pll power down
    Ret = Expander_SetPllShutdown(&pBoard->Expander, ePllPowerDownOn);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Wait small delay to make sure pll spi is ready for operation after power-on state (eeprom loading)
    Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);

    // Read pll reg 8
    Ret = Cdce62005_ReadReg(&pBoard->Pll, ucAddr, &uVal1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // In power down, this reg must read 0
    if (uVal1 == 0)
    {
        // Remove pll power down
        Ret = Expander_SetPllShutdown(&pBoard->Expander, ePllPowerDownOff);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
        // Wait small delay to make sure pll spi is ready for operation after power-on state (eeprom loading)
        Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
    
        // Read pll reg 8
        Ret = Cdce62005_ReadReg(&pBoard->Pll, ucAddr, &uVal2);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        if (uVal2 == 0)
            // Problem cannot remove pll power down
            *puResult = 2;
    }
    else
    {
        // Problem cannot power down the pll
        *puResult = 1;
    }
#endif
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestPllSync(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    unsigned uLock;
#endif
    
    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been resetted correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset07);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST

    // Setup the pll for the test configuration (secondary input as pll reference)
    Ret = _Mo1000_SetupTestPll(pBoard, 1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Configure the clock reference our own clock
    Ret = Expander_SetClkReference(&pBoard->Expander, eMasterRef);

    // Complete the output sync operation
    Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Wait for pll to lock
    Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Did we got a pll lock
    if (uLock)
    {
        // Freeze outputs
        Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOn);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Wait for pll to lock
        Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Did we got a pll lock
        if (uLock)
        {
            // Error, secondary 125MHz reference is still on
            *puResult = 2;
        }
    }
    else
    {
        // Error, secondary 125MHz reference is not ok
        *puResult = 1;
    }
#endif

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestDacIrq(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    Result Ret1;
    unsigned uLock;
    int iCount;
    Ad9148_stDacEvent Event;
    Expander_eDacIrq DacIrq[2];
#endif
    
    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been resetted correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset08);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST
    // Setup the pll for the test configuration (primary input as pll reference)
    Ret = _Mo1000_SetupTestPll(pBoard, 0);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Release dacs reset
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac1, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac2, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Reset each dac
    Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac2);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Initialize each dac
    Ret = Ad9148_Init(&pBoard->Dac, eMo1000DevComDeviceDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_Init(&pBoard->Dac, eMo1000DevComDeviceDac2);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    Ret = Ad9148_SetInterpolation(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148Inter1x);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_SetInterpolation(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148Inter1x);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Complete the output sync operation
    Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Wait for pll to lock
    Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Did we got a pll lock
    if (uLock)
    {
        // Cleans all possible irq of each dac device
        for (iCount = 0, Ret1 = eMo1000Ok; iCount < MO1000_DAC_PER_BOARD && SUCCESS(Ret1); iCount++)
        {
            // Disable all irq
            Event.m_uRaw = 0xffff;
            Ret = Ad9148_ResetIrqEvent(&pBoard->Dac, iCount, Event);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
            // Clears any pending irq event
            Ret = Ad9148_ClearIrqEvent(&pBoard->Dac, iCount, Event);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        }
        
        // Check if we have any dac errors?
        Ret = Expander_GetDacIrq(&pBoard->Expander, &DacIrq[0]);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Checks if we have any pending irq
        if (DacIrq[0] == eDacNoError)
        {
            // Synchronize each dac device
            for (iCount = 0, Ret1 = eMo1000Ok; iCount < MO1000_DAC_PER_BOARD && SUCCESS(Ret1); iCount++)
            {
                // Enable sync lock irq
                Event.m_uRaw = 0;
                Event.stBit.m_uSyncLock = 1;
                Ret = Ad9148_SetIrqEvent(&pBoard->Dac, iCount, Event);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

                // Execute the fifo read synchronisation
                Ret1 = _Mo1000_DoFifoReadSync(pBoard, iCount);
                if(FAILURE(Ret1))
                {
                    // Sync problems?
                    if (Ret1 == eMo1000NoReadSync || Ret1 == eMo1000ReadSyncLost)
                    {
                        // Dac clocks problem
                        *puResult = 2 + iCount;
                    }
                    else
                    {
                        FUNC_EXIT();
                        return Ret1;
                    }
                }
                else
                {
                    // Check if we have any dac errors?
                    Ret = Expander_GetDacIrq(&pBoard->Expander, &DacIrq[iCount]);
                    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
                }
            }
            
            // Checks the irq we got
            if (DacIrq[0] == eDac1Error)
            {
                if (DacIrq[1] != eDacBothErrors)
                {
                    // Error, dac2 irq did not happen
                    *puResult = 8;
                }
            }
            else
            {
                // Error, dac1 irq did not happen
                *puResult = 7;
            }
        }
        else
        {
            // Error, some dacs have stuck irq
            *puResult = 4 + DacIrq[0];
        }
    }
    else
    {
        // Error, secondary 125MHz reference is not ok
        *puResult = 1;
    }
#endif
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestDacClks(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    Result Ret1;
    unsigned uLock;
    int iCount;
#endif
    
    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been resetted correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset09);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST

    // Setup the pll for the test configuration (primary input as pll reference)
    Ret = _Mo1000_SetupTestPll(pBoard, 0);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Release dacs reset
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac1, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac2, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Reset each dac
    Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac2);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Initialize each dac
    Ret = Ad9148_Init(&pBoard->Dac, eMo1000DevComDeviceDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_Init(&pBoard->Dac, eMo1000DevComDeviceDac2);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    Ret = Ad9148_SetInterpolation(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148Inter1x);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_SetInterpolation(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148Inter1x);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Complete the output sync operation
    Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Wait for pll to lock
    Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Did we got a pll lock
    if (uLock)
    {
        // Synchronize each dac device
        for (iCount = 0, Ret1 = eMo1000Ok; iCount < MO1000_DAC_PER_BOARD && SUCCESS(Ret1); iCount++)
        {
            // Execute the fifo read synchronisation
            Ret1 = _Mo1000_DoFifoReadSync(pBoard, iCount);
            if(FAILURE(Ret1))
            {
                // Sync problems?
                if (Ret1 == eMo1000NoReadSync || Ret1 == eMo1000ReadSyncLost)
                {
                    // Dac clocks problem
                    *puResult = 2 + iCount;
                }
                else
                {
                    FUNC_EXIT();
                    return Ret1;
                }
            }
        }
    }
    else
    {
        // Error, secondary 125MHz reference is not ok
        *puResult = 1;
    }
#endif
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_TestDciFrame(Mo1000_stMo1000 * pBoard,  unsigned int* puResult)
{
    Result Ret = eMo1000Ok;
#ifdef MO1000_TEST
    unsigned uLock, uMin[4], uMax[4], uMin1[4], uMax1[4], uMin2[4], uMax2[4];
    unsigned uMovedLow, uMovedHigh;
    int iCount;
    char cOffset = 0;
#endif
    
    FUNC_ENTER();

    // Returns default test result
    *puResult = 0;

    // Check if the board has been resetted correctly at least once
    if (!pBoard->iResetDone)
    {
        FUNC_EXIT();
        return(eMo1000NoReset10);
    }

    // Needed a complete reset after this step
    pBoard->iResetDone = 0;
    pBoard->iInitDone = 0;
    
#ifdef MO1000_TEST

    // Setup the pll for the test configuration (primary input as pll reference)
    Ret = _Mo1000_SetupTestPll(pBoard, 0);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Release dacs reset
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac1, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Expander_SetDacReset(&pBoard->Expander, eDac2, eDacResetOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Reset each dac
    Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_Reset(&pBoard->Dac, eMo1000DevComDeviceDac2);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Initialize each dac
    Ret = Ad9148_Init(&pBoard->Dac, eMo1000DevComDeviceDac1);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_Init(&pBoard->Dac, eMo1000DevComDeviceDac2);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    Ret = Ad9148_SetInterpolation(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148Inter1x);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    Ret = Ad9148_SetInterpolation(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148Inter1x);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Configure the hardware with the number of active channels
    pBoard->pCore->stControl.stBit.m_uDataSerdesPd = 0;
    pBoard->pCore->stControl.stBit.m_uGlobalSerdesPd = 0;
    
    // Adjust dci/frame paths
    Ret = Expander_SetDataMux(&pBoard->Expander, eMuxShortPath, eMuxShortPath);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

    // Complete the output sync operation
    Ret = Expander_SetPllSync(&pBoard->Expander, ePllSyncOff);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // Wait for pll to lock
    Ret = Cdce62005_WaitPllLock(&pBoard->Pll, &uLock);
    if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
    
    // This is matching fpga clock for our test pll setup
    pBoard->iFreqCoreRef4Calculated = 250000000;
    
    if (uLock)
    {
        // Default global tap delay offset
        pBoard->cTapsDelayOffset = cOffset;
        
        // Keep trace of the number of calibration trials
        pBoard->uCalibNdx = 0;
    
        // Keep trace of the channels lane that were calibrated correctly the last time it was done
        pBoard->auChannelsLaneCalib[pBoard->uCalibNdx] = 0xffffffff;
        
        // Keep trace of the number of frame calibration trials
        pBoard->uCalibFrameNdx = 0;
    
        // Keep trace of the frame calibration errors
        pBoard->auFrameLaneCalib[pBoard->uCalibFrameNdx] = 0xffffffff;
    
        // Keep trace of the number of sync calibration trials
        pBoard->uCalibSyncNdx = 0;
    
        // Keep trace of the sync calibration errors
        pBoard->auSyncCalib[pBoard->uCalibSyncNdx] = 0xffffffff;
    
        // Set all channel to not valid
        pBoard->pCore->stControl.stBit.m_uDacReady = 0x00;
        // Update DacReady values
        pBoard->pCore->stControl.stBit.m_uUpdateDacReady = 1;
    
        // Keep trace if calib was made correctly or not
        pBoard->iCalibDone = 0;
        pBoard->iDataCalibDone = 0;
        pBoard->iFrameCalibDone = 0;
        pBoard->iSyncCalibDone = 0;
    
        // Keep trace if test mode is active
        pBoard->iTestMode = 0;
    
        // Make sure the clocks are stabilized if any clock changed was done
    
        pBoard->pCore->stControl.stBit.m_uDelayRst = 1;
        pBoard->pCore->stControl.stBit.m_uSerdesRst = 1;
    
        // Wait small delay for resets to complete
        Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
    
        // Check if mmcm is locked when we are a clock master as a security measure
        if ((pBoard->pCore->stStatus.stBit.m_uClockMaster) == 1 && (pBoard->pCore->stStatus.stBit.m_uMmcmLocked) == 0)
        {
            FUNC_EXIT();
    
            // Problem with signal not rising after reset
            return(eMo1000NoMmcmLock);
        }
    
        // Attempt data calibration
        Ret = _Mo1000_DoDataCalib(pBoard);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Do a frame scan check to find the first group position
        Ret = _Mo1000_DoFrameScanCheck(pBoard, uMin, uMax);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Adjust dci/frame paths
        Ret = Expander_SetDataMux(&pBoard->Expander, eMuxShortPath, eMuxLongPath);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Do a frame scan check to find the first group position
        Ret = _Mo1000_DoFrameScanCheck(pBoard, uMin1, uMax1);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Adjust dci/frame paths
        Ret = Expander_SetDataMux(&pBoard->Expander, eMuxLongPath, eMuxShortPath);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

        // Do a frame scan check to find the first group position
        Ret = _Mo1000_DoFrameScanCheck(pBoard, uMin2, uMax2);
        if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
        // Check the moving tendency of the group for each frame for 2nd verification
        uMovedLow = 0;
        uMovedHigh = 0;
        for (iCount = 0; iCount < 4; iCount++)
        {
            if (uMin1[iCount] < uMin[iCount])
                uMovedLow++;    
            if (uMax1[iCount] < uMax[iCount])
                uMovedLow++;    
            if (uMin1[iCount] > uMin[iCount])
                uMovedHigh++;    
            if (uMax1[iCount] > uMax[iCount])
                uMovedHigh++;    
        }

        // Check if we have the same path length for 1st 2 verifications?
        if (uMovedLow == 0 && uMovedHigh == 0)
        {
            // Error, frame path stuck eMuxShortPath
            *puResult = 2;
        }
        else
        {
            // Check if group is moving low (it must because long path for frame , will requiere least frame tap delays)
            if (uMovedLow < uMovedHigh)
            {
                // Error, dci path stuck eMuxLongPath
                *puResult = 3;
            }
            else
            {
                // Check the moving tendency of the group for each frame of 3rd verification
                uMovedLow = 0;
                uMovedHigh = 0;
                for (iCount = 0; iCount <4; iCount++)
                {
                    if (uMin2[iCount] < uMin[iCount])
                        uMovedLow++;    
                    if (uMax2[iCount] < uMax[iCount])
                        uMovedLow++;    
                    if (uMin2[iCount] > uMin[iCount])
                        uMovedHigh++;    
                    if (uMax2[iCount] > uMax[iCount])
                        uMovedHigh++;    
                }
                // Check if we have the same path length for 1st with 3rd verifications?
                if (uMovedLow == 0 && uMovedHigh == 0)
                {
                    // Error, dci path stuck eMuxShortPath
                    *puResult = 4;
                }
                else
                {
                    // Check if group is moving high (it must because long path for dci , will requiere more frame tap delays)
                    if (uMovedLow > uMovedHigh)
                    {
                        // Error, frame path stuck eMuxLongPath
                        *puResult = 5;
                    }
                }
            }
        }
    }
    else
    {
        // Error, secondary 125MHz reference is not ok
        *puResult = 1;
    }
#endif
    
    FUNC_EXIT();

    return Ret;
}


Result Mo1000_CheckI2c(Mo1000_stMo1000 * pBoard, unsigned char ucScanMax, Mo1000_stI2cScan *pScanRaw,
                       unsigned char ucNumDevices, Mo1000_stI2cAddr *pScanRes, unsigned char *pucDetectOk)
{
    Result Ret = eMo1000Ok;
    unsigned char ucDevAddr;

    FUNC_ENTER();

    // Initialize to no detection
    memset(pScanRes,0,sizeof(Mo1000_stI2cAddr));
    *pucDetectOk = 0;

    // Check every 7 bits i2c addresses that was scanned on the bus
    for (ucDevAddr = 0; ucDevAddr < MO1000_I2SCAN_MAX_DEV; ucDevAddr++)
    {
        // Check if a device was detected at this address
        if (pScanRaw->m_ucDevDetect[ucDevAddr])
        {
            // Check if we have found the bridge
            if (ucDevAddr == pBoard->Com.m_ucBridgeAddr)
                pScanRes->m_ucSpiBridge = 1;
            else
            {
                // Check if we have found the temp sensor
                if (ucDevAddr == pBoard->Com.m_ucSensorAddr)
                    pScanRes->m_ucSensor = 1;
                else
                {
                    // Check if we have found the eeprom
                    if (ucDevAddr == pBoard->Com.m_ucEepromAddr)
                        pScanRes->m_ucEeprom = 1;
                    else
                    {
                        // Check if we have found the io expander
                        if (ucDevAddr == pBoard->Com.m_ucExpanderAddr)
                            pScanRes->m_ucExpander = 1;
                    }
                }
            }
        }
    }

    // Check if we have detected all of our devices and nothing else
    if (pScanRes->m_ucExpander == 1 && pScanRes->m_ucEeprom == 1 && pScanRes->m_ucSensor == 1 &&
        pScanRes->m_ucSpiBridge == 1 && (ucNumDevices <= ucScanMax || ucScanMax == 0))
        *pucDetectOk = 1;

    FUNC_EXIT();

    return(Ret);
}


Result Mo1000_GetCoreFrequency(Mo1000_stMo1000 * pBoard,  unsigned short* pusCoreRefClock,
                               unsigned short* pusDacRefClock)
{
    Result Ret = eMo1000Ok;
    unsigned uCount;
    
    FUNC_ENTER();

    // Returns default frequencies
    *pusCoreRefClock = 0;
    *pusDacRefClock = 0;    

    // Get core ref frequency
    pBoard->pCore->stFreqCntClk.stBit.m_uFreqCntSel = 0;    
    pBoard->pCore->stFreqCntClk.stBit.m_uFreqCntRst = 1;    
    
    // Wait for the frequency measurement to be ok
    for (uCount = 0; uCount < MO1000_FREQMEASURE_DELAY; uCount++)
    {
        // Wait small time
        Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
        
        // Check if the measured frequency is now valid
        if (pBoard->pCore->stFreqCntClk.stBit.m_uFreqCntRdy !=0)
            break;
    }
    
    // Did we get an error?
    if (uCount >= MO1000_FREQMEASURE_DELAY)
    {
        Ret = eMo1000BadCoreFrequency;
    }
    else
    {
        // Get the core frequency measurement
        *pusCoreRefClock = pBoard->pCore->stFreqCntClk.stBit.m_uFreqCnt;
                 
        // Get dacs ref frequency
        pBoard->pCore->stFreqCntClk.stBit.m_uFreqCntSel = 1;    
        pBoard->pCore->stFreqCntClk.stBit.m_uFreqCntRst = 1;    
        
        // Wait for the frequency measurement to be ok
        for (uCount = 0; uCount < MO1000_FREQMEASURE_DELAY; uCount++)
        {
            // Wait small time
            Carrier_usleep(pBoard->hCarrier, MO1000_SMALL_DELAY);
            
            // Check if the measured frequency is now valid
            if (pBoard->pCore->stFreqCntClk.stBit.m_uFreqCntRdy !=0)
                break;
        }
        
        // Did we get an error?
        if (uCount >= MO1000_FREQMEASURE_DELAY)
        {
            Ret = eMo1000BadDacsFrequency;
        }
        else
        {
            // Get the dacs frequency measurement
            *pusDacRefClock = pBoard->pCore->stFreqCntClk.stBit.m_uFreqCnt;
        }
    }

    FUNC_EXIT();

    return Ret;
}


Result Mo1000_SetTestMode(Mo1000_stMo1000 * pBoard, Mo1000_eTestMode Test,
                          unsigned short usEvenPattern, unsigned short usOddPattern)
{
    Result Ret = eMo1000Ok;
    unsigned short usDac1Enable = 0xffff, usDac2Enable = 0xffff;
    int iSwap = 0;
    
    FUNC_ENTER();

    // Check if the board has been initialized correctly at least once
    if (!pBoard->iInitDone)
    {
        FUNC_EXIT();
        return(eMo1000NoInit18);
    }

    // Check if the last board calibration was successfull: this is critical for the test mode
    // eMo1000TestMode2 to work correctly
    if (!pBoard->iCalibDone && Test == eMo1000TestMode2)
    {
        FUNC_EXIT();
        return(eMo1000NoCalibDone);
    }

    // Test mode validation
    switch(Test)
    {
        case eMo1000TestModeoff:
            // sed on dac1 (if yes disable sed)
            if (usDac1Enable)
            {
                // Disable dac sed compare mode
                Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedDisable);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            }
            // sed on dac2 (if yes disable sed)
            if (usDac2Enable)
            {
                // Disable dac sed compare mode
                Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedDisable);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            }

            // Disable core pattern mode
            pBoard->pCore->stControl.stBit.m_uDataPatternEn = 0;
            break;
        
        case eMo1000TestMode1:
        case eMo1000TestMode2:
            // Setup core pattern mode and dacs sed logic
            Ret = _Mo1000_DoSedSetup(pBoard, usEvenPattern, usOddPattern, usEvenPattern, usOddPattern,
                                     usEvenPattern, usOddPattern, usEvenPattern, usOddPattern,
                                     usDac1Enable, usDac2Enable, iSwap);
            if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}

            // Enable core pattern mode
            pBoard->pCore->stControl.stBit.m_uDataPatternEn = 1;
                
            // Sed setup auto clear mode/normal sed on each dac device to be scanned
            // Any calibrated bit on dac1 (if yes setup sed)
            if (usDac1Enable)
            {
                // Enable dac sed compare auto clear mode
                Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedAutoClearEnable);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
                // Enable dac sed compare normal mode
                Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac1, eAd9148SedEnable);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            }
        
            // Any calibrated bit on dac2 (if yes setup sed)
            if (usDac2Enable)
            {
                // Enable dac sed compare auto clear mode
                Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedAutoClearEnable);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
        
                // Enable dac sed compare normal mode
                Ret = Ad9148_SetSedCtrl(&pBoard->Dac, eMo1000DevComDeviceDac2, eAd9148SedEnable);
                if(FAILURE(Ret)) {FUNC_EXIT(); return Ret;}
            }
            break;
        
        default:
            Ret = eMo1000InvalidTestMode;
            FUNC_EXIT();
            return Ret;
            break;
    }
    
    // Keep trace if test mode is active
    pBoard->iTestMode = Test;

    FUNC_EXIT();

    return Ret;
}
