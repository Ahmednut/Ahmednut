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
 * @file       MI125_mi125.c
 * @brief      mi125 module related functions
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI125
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

#include "MI125_mi125.h"
#include "MI125_mi125_defines.h"
#include "MI125_com_wrapper.h"

#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

#include "FMCEEprom_eeprom_id.h"


/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

 // Version of MI125 driver
#define MI125_DVERSION 0x0023


// Comment this line for the real MI125 target
//#define MI125_SIMUL 1

#ifdef DEBUG
// Uncomment this line to add some trace debug output strings
#define MI125_MODDEBUG 1
#endif

// Debug related definitions
#ifdef MI125_MODDEBUG
    // These macros are used to get printout info while debugging
    #define FUNC_ENTER()      printf("MI125: enter %s\n", __FUNCTION__)
    #define FUNC_EXIT()       printf("MI125: exit  %s\n", __FUNCTION__)
    #define PDEBUG(...) printf(__VA_ARGS__)

#else

    #define FUNC_ENTER()
    #define FUNC_EXIT()
    #define PDEBUG(...)

#endif

// This identifies the MI125 fpga core
#define MI125_COREID_MI125 0xC125

#define MI125_ADCCLKPLL_DELAY (25 * 2)             ///< Delay to stabilize the adc clock input pll (min 25 usec, keep some margin)
#define MI125_ADCSMALL_DELAY (1)                   ///< Smallest possible delay
#define MI125_ADCIDELAYRDY_DELAY (3 * 5)           ///< from FPGA IDELAYCTRLRst 1->0 to IDELAYRDY = 0->1, Virtex-6-1 at 100MHz
#define MI125_ADCMCMCLOCKRDY_DELAY (100 * 2)       ///< from FPGA ADCClockMMCMRst 1->0 to AdcClockMmcmLock = 0->1, Virtex-6-1 (max 100usec)
// !!! M.T. to be tuned later
#define MI125_ADCDIGITALCALIB (50000)              ///< Max time allowed for a single calibration trial process
#define MI125_ADCSLEEPOUT_DELAY (2000 * 2)         ///< Delay to stabilize the adc recovering from sleep mode (when new additionnal channels are configured)
#define MI125_CALIB_DETECTION_DELAY (5)            ///< Delay to let time to reset the calibration processes before reading the done flag

// Local osc stabilization time specified by manufacturer, need to measure approx.
// Startup is approx. 2msec (measured)
#define MI125_LOCALOSC_DELAY (10000)               ///< Delay to stabilize the 125 MHz local oscillator after powerup

// Specified by manuf is max 195msec
#define MI125_BRIDGESTART_DELAY (200000)           ///< Worst case delay before the spi bridge is out of reset and becoming ready for used

// Local osc stabilization time specified by manufacturer, need to measure approx.
// Startup is approx 4msec (measured)
#define MI125_BUF_CLOCK_PWRN (10000)               ///< Delay to stabilize the 125 MHz local oscillator after powerup

// ADC Digital calibration pattern used
#define MI125_ADCCALIB 0x033C   // This will calibrate to 0x2C value for each output lane
// #define MI125_ADCCALIB 0x0000   // This will calibrate to 0x2C value for each output lane

// Pattern to test single calib support mode
#define MI125_TEST_CALIBSNG 0x55AA33CC

// Max iteration number while waiting for the calibration detection process to be done
// !!! Warning, this must be long enough so the fpga could do is job without being interrupted
#define MAX_ITERATION_CALIB_DETECTION_DONE 50000

// Defines the needed delay for the mi125 trig signal (to the record core) so
// that the adc captured data match the position of the trigger. The prog delay
// is programmed value + 1
#define MI125_TRIG_ADC_SYNC_DELAY 6

// Fix part of io expander i2c addresses
#define PCF8574_I2CID 0x38
#define PCA9535_I2CID 0x20

// Maximum data rate
#define MI125_MAX_DATA_RATE (8 * 125000000)

/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/
// ADC data fmc lanes position in pausLaneDelays delay table in order of data lanes from 0 to 31
static const unsigned char Mi125_aucDataFmc[32]={8, 7, 5, 9, 3, 2, 6, 4, 16, 15, 14, 17, 10, 13, 12, 11,
                                                 22, 21, 26, 27, 18, 23, 20, 19, 33, 32, 31, 30, 25, 24, 29, 28};

// adc fmc dco position in pausLaneDelays delay table 
static const unsigned char Mi125_aucDcoFmc = 0;


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/
static Result _MI125_mi125_setclkbuf14fanout(MI125_mi125 * board);
static Result _MI125_mi125_isetclksrc(MI125_mi125 * board, MI125_mi125_clock_source clksrc);
static Result _MI125_mi125_detectbetaboard(MI125_mi125 * board);
static Result _MI125_mi125_GetDelayAnalysis(unsigned short *pausLaneDelays,
                                            unsigned short *pusMin, unsigned short *pusMax,
                                            unsigned uFindRef, unsigned char *pucRef);
static Result _MI125_mi125_CalculateSignalPaths(unsigned short *pausLaneDelays, 
                                                char *pucLaneOffset, unsigned short usRefDelay,
                                                MI125_mi125_IoDelayRef Reference, char cDcoTapOffset);
static int _MI125_mi125_USCmp(const void *ElemA, const void *ElemB);
static int _MI125_mi125_iSign(int iElement);


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


 /****************************************************************************
 * NAME : _MI125_mi125_setclkbuf14fanout
 ************************************************************************//**
 *
 * Configure the clock buffer switch in 1:4 fanout buffer mode according
 * to the current selected clock source.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The clock source input selection must be set with MI125_mi125_set_clksrc() before
 *   using this function to select which clock source to program in the clock buffer outputs.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset(), MI125_mi125_set_clksrc().
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close, MI125_mi125_set_clksrc.
 *
 ***************************************************************************/
static Result _MI125_mi125_setclkbuf14fanout(MI125_mi125 * board)
{
    Result ret = MI125_MI125OK;

#ifndef SIMUL
    int outval;

    // For each clock buffer output to configure to the selected clock source
    for (outval = MI125_PCA9535_CKBUFOUT0; outval <= MI125_PCA9535_CKBUFOUT3; outval++)
    {
        // Wait small delay to meet clock input selection setup time (from MI125_mi125_set_clksrc)
        // or config hold time after each output is programmed
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFOFF,
                                                             MI125_PCA9535_CKBUFLOADOFF,
                                                             outval);
        if(FAILURE(ret)) {return ret;}

        // Wait small delay to meet clock output selection setup time
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFOFF,
                                                             MI125_PCA9535_CKBUFLOADON,
                                                             outval);
        if(FAILURE(ret)) {return ret;}

        // Wait small delay to meet load pulse width time
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFOFF,
                                                             MI125_PCA9535_CKBUFLOADOFF,
                                                             outval);
        if(FAILURE(ret)) {return ret;}

        // Wait small delay to meet load to config setup time
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFON,
                                                             MI125_PCA9535_CKBUFLOADOFF,
                                                             outval);
        if(FAILURE(ret)) {return ret;}

        // Wait small delay to meet config pulse width time
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        ret = MI125_pca9535_setclkbufctrl(&board->expander2, MI125_PCA9535_CKBUFCONFOFF,
                                                             MI125_PCA9535_CKBUFLOADOFF,
                                                             outval);
        if(FAILURE(ret)) {return ret;}
    }

    // Wait small delay to meet config hold time
    Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);

#endif

    return ret;
}


 /****************************************************************************
 * NAME : _MI125_mi125_isetclksrc
 ************************************************************************//**
 *
 * Configure the clock source according to what is specified.
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - The MI125_mi125_reset() function must be call at least once before using this function.
 *
 * @param [in] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [in] clksrc
 *    This is the desired clock source.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close.
 *
 ***************************************************************************/
static Result _MI125_mi125_isetclksrc(MI125_mi125 * board, MI125_mi125_clock_source clksrc)
{
    Result ret = MI125_MI125OK;

#ifndef SIMUL
    // Change the clksrc according to board type
    if (board->beta)
    {
        MI125_pca9535_localoscsw sw;

        // Check if we don't use the internal oscillator and turn it off to lower noise
        if (clksrc == MI125_CLKSRC125MHZ)
        {
            sw = MI125_PCA9535_LOSWON;
            ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
            if(FAILURE(ret)) {return ret;}

            // Make sure the local oscillator has started and stabilized
            Carrier_usleep(board->hCarrier, MI125_LOCALOSC_DELAY);
        }
        else
        {
            sw = MI125_PCA9535_LOSWOFF;
            ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
            if(FAILURE(ret)) {return ret;}
        }

        ret = MI125_pca9535_setclksource(&board->expander2, clksrc);
        if(FAILURE(ret)) {return ret;}
        ret = _MI125_mi125_setclkbuf14fanout(board);
    }
    else
        ret = MI125_pcf8574_setclksource(&board->expander, clksrc);
#endif

    return ret;

}


 /****************************************************************************
 * NAME : _MI125_mi125_detectbetaboard
 ************************************************************************//**
 *
 * Attempt to detect if we have a beta board (or later) or not (alpha).
 * This function will set in global structure information the detected board
 * type (beta member).
 *
 * @warning
 * - The module must be opened with MI125_mi125_open() before using this function.
 * - This function may wrongly detect a beta board as an alpha board if the beta
 *   board is defective.
 *
 * @param [in,out] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *    The beta member is set correctly according to board type.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @pre
 *    MI125_mi125_open(), MI125_mi125_reset().
 *
 * @see
 *    MI125_mi125_open, MI125_mi125_reset, MI125_mi125_close.
 *
 ***************************************************************************/
static Result _MI125_mi125_detectbetaboard(MI125_mi125 * board)
{
    Result ret;

#ifndef SIMUL
    MI125_pca9535_addr caddr;
    volatile unsigned char inval;

    // Read back existing related registers content
    caddr = MI125_PCA9535_00;
    ret = MI125_com_read(board->expander2.com, MI125_COM_PORTS2, caddr, (unsigned char*)&inval);
    if(FAILURE(ret))
        // Set operation to alpha board
        board->beta = 0;
    else
        // Set operation to beta board and later
        board->beta = 1;
#else
    // Set operation to beta board
    board->beta = 1;
#endif

    return MI125_MI125OK;
}


 /****************************************************************************
 * NAME : _MI125_mi125_GetDelayAnalysis
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
 * @return   Error code: #MI125_MI125OK if no problem is found, !#MI125_MI125OK if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    MI125_mi125_open(), MI125_mi125_set_datapaths_calibration().
 *
 * @see
 *
 ***************************************************************************/ 
static Result _MI125_mi125_GetDelayAnalysis(unsigned short *pausLaneDelays,
                                            unsigned short *pusMin, unsigned short *pusMax,
                                            unsigned uFindRef, unsigned char *pucRef)
{
    Result Ret = MI125_MI125OK;
    unsigned short usCount, usValue;
    unsigned short pausDataDelays[32][2];
    unsigned uMiddle;
    int iCurrDiff,iLastDiff;
    
    FUNC_ENTER();

    // Initial values (for lane 0)
    *pusMin = pausLaneDelays[Mi125_aucDataFmc[0]];
    *pusMax = pausLaneDelays[Mi125_aucDataFmc[0]];
    pausDataDelays[0][0] = *pusMin;
    pausDataDelays[0][1] = 0;
    *pucRef = 255;
    
    // Scan for each data lane
    for (usCount= 1; usCount < 32; usCount++)
    {
        usValue = pausLaneDelays[Mi125_aucDataFmc[usCount]];
        
        // Saves data delays associated with this lane position
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
        qsort(&pausDataDelays[0][0], 32, sizeof(pausDataDelays[0]), _MI125_mi125_USCmp);

        iCurrDiff = uMiddle - pausDataDelays[0][0];
        
        // Scan for each data lane and search for the delay value nearest to the mean one between the lanes
        for (usCount= 0; usCount < 32; usCount++)
        {
            // Do we find the exact delay value?
            if (pausDataDelays[usCount][0] == uMiddle)
            {
                // Get the related data lane position as the reference
                *pucRef = pausDataDelays[usCount][1];
                break;                
            }
            else
            {
                iLastDiff = iCurrDiff;
                
                // Calculates the difference
                iCurrDiff = (int)(uMiddle) - pausDataDelays[usCount][0];
                
                // Did we get a change of sign between current and last value?
                if (_MI125_mi125_iSign(iCurrDiff) != _MI125_mi125_iSign(iLastDiff))
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
           // Uses data lane 0 as a reference...
           *pucRef = 0;
        }
    }
    
    FUNC_EXIT();
    
    return(Ret);
}


 /****************************************************************************
 * NAME : _MI125_mi125_CalculateSignalPaths
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
 * @param [in] cDcoTapOffset
 *    This is a global dco tap offset to use in the calculation.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !#MI125_MI125OK if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @pre
 *    MI125_mi125_open(), MI125_mi125_set_datapaths_calibration().
 *
 * @see
 *
 ***************************************************************************/ 
static Result _MI125_mi125_CalculateSignalPaths(unsigned short *pausLaneDelays, 
                                                char *pucLaneOffset, unsigned short usRefDelay,
                                                MI125_mi125_IoDelayRef Reference, char cDcoTapOffset)
{
    Result Ret = MI125_MI125OK;
    unsigned short usCount, usValue;
    int iCurrDiff;
    
    FUNC_ENTER();

    // Scan for each data lane
    for (usCount= 0; usCount < 32; usCount++)
    {
        // Get the lane delay for this data lane
        usValue = pausLaneDelays[Mi125_aucDataFmc[usCount]];
        
        // Calculates the delay delta from the reference lane in fpga taps
        iCurrDiff = ((int)(usRefDelay) - (int)(usValue)) / (int)(Reference) + (int)(cDcoTapOffset);
        
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
 *   NAME : _MI125_mi125_USCmp
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
static int _MI125_mi125_USCmp(const void *ElemA, const void *ElemB)
{
    return ( (*(unsigned short*)ElemA) - (*(unsigned short*)ElemB));
}


 /****************************************************************************
 *   NAME : _MI125_mi125_iSign
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
static int _MI125_mi125_iSign(int iElement)
{
    return ( (iElement > 0) - (iElement < 0));
}

 /****************************************************************************
 *   NAME : _MI125_mi125_get_i2c_addr
 ************************************************************************//**
 *
 * Get the address list of all the I2C devices. The device addresses depend
 * on the FMC connector GA and on the type of the MI125 board.
 *
 * @param [in,out] board
 *    This structure is the one returned at module opening with MI125_mi125_open().
 *
 * @param [out] i2c_dev_list
 *    Pointer to a structure to return the address list of all the I2C devices
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured.
 * 
 ***************************************************************************/ 
Result _MI125_mi125_get_i2c_addr(MI125_mi125 * board, MI125_mi125_i2caddr * i2c_dev_list)
{
    Result ret;
    unsigned char ga;
    char FmcInfoType[16];

    MI125_mi125_i2caddr i2cadr_main_ga00 = MI125_I2CADR_MAIN_GA00;
    MI125_mi125_i2caddr i2cadr_sec_ga00  = MI125_I2CADR_SEC_GA00;
    MI125_mi125_i2caddr i2cadr_main_ga01 = MI125_I2CADR_MAIN_GA01;
    MI125_mi125_i2caddr i2cadr_sec_ga01  = MI125_I2CADR_SEC_GA01;
    MI125_mi125_i2caddr i2cadr_main_ga10 = MI125_I2CADR_MAIN_GA10;
    MI125_mi125_i2caddr i2cadr_sec_ga10  = MI125_I2CADR_SEC_GA10;
    MI125_mi125_i2caddr i2cadr_main_ga11 = MI125_I2CADR_MAIN_GA11;
    MI125_mi125_i2caddr i2cadr_sec_ga11  = MI125_I2CADR_SEC_GA11;

    ret = Carrier_FmcGetInfo(board->hCarrier, board->FmcConnector,
        (Carrier_eFmcBoardNum_t)board->fmcposition, CARRIER_INFO_BOARDTYPE,
        FmcInfoType, sizeof(FmcInfoType));
    if(FAILURE(ret)) {return ret;}

    ret = Carrier_FmcGetGA(board->hCarrier, board->FmcConnector, &ga);
    if(FAILURE(ret)) {return ret;}

    if(ga==0)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MI125_E)==0 ||
            strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MI125W_E)==0)
            *i2c_dev_list = i2cadr_main_ga00;
        else
            *i2c_dev_list = i2cadr_sec_ga00;
    }
    else if(ga==1)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MI125_E)==0 ||
            strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MI125W_E)==0)
            *i2c_dev_list = i2cadr_main_ga01;
        else
            *i2c_dev_list = i2cadr_sec_ga01;
    }
    else if(ga==2)
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MI125_E)==0 ||
            strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MI125W_E)==0)
            *i2c_dev_list = i2cadr_main_ga10;
        else
            *i2c_dev_list = i2cadr_sec_ga10;
    }
    else
    {
        if(strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MI125_E)==0 ||
            strcmp(FmcInfoType, FMCEEPROM_EEPROM_MODNAME_MI125W_E)==0)
            *i2c_dev_list = i2cadr_main_ga11;
        else
            *i2c_dev_list = i2cadr_sec_ga11;
    }

    return MI125_MI125OK;
}


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

Result MI125_mi125_open(MI125_mi125 * board, MI125_mi125_Register_Map * core, Carrier_handle_t hCarrier)
{
    Result ret;
    unsigned int fmcposition;
    unsigned int I2cZone;
    MI125_mi125_i2caddr i2c_dev_list;
    // Default lane offset delays for a dual fmc mi125 stack
    // !!! M.T. These values would need to be changed if a mi125 is on top of a different fmc board than mi125,
    // !!! J.R. or a different carrier board than perseus 601x.
    MI125_mi125_laneoffset lanedelay[4] = {{{0}},{{0}},{{0}},{{0}}};

    FUNC_ENTER();

    // Verify if the core match a mi125 instance
    if(core->Register_mi125info.Bit.coreid != MI125_COREID_MI125)
        return MI125_MI125NOCORE;

    // Assign the board memory map registers
    board->core = core;

    // Assign carrier handle
    board->hCarrier = hCarrier;

    // Make sure fmcposition is possible
    fmcposition = board->core->Register_mi125status.Bit.FmcPositon;

    // Set lane offset
    lanedelay[1].laneoffset[24] = -5; // FPGA CH15 HA03
    lanedelay[1].laneoffset[25] = -3; // FPGA CH15 HB09
    lanedelay[1].laneoffset[26] = -5; // FPGA CH16 HA02
    lanedelay[1].laneoffset[27] = -4; // FPGA CH16 HB08
    lanedelay[1].laneoffset[30] = -5; // FPGA CH14 HA01
    lanedelay[1].laneoffset[31] = -5; // FPGA CH14 HA08

    // Save the core reference clock (default for perseus 601x)
    board->Reference = MI125_REF200MHZ;

    // By default, we are trying to calibrate our reference lane in the middle of the possible taps
    board->iFirstTapOffsetFeed = 16;

    // Keep trace if using default taps delay or not
    board->defaulttaps = 1;

    board->usDcoDelay = 0;
    
    // Setup calibration reference lane (this should be the one with the middle lane delay)
    board->ucDataRefLane = 0;

    // Saves the reference lane delay in ps
    board->usRefDelay = 0;

    // Save the lanes offset delays
    board->lanedelays = lanedelay[fmcposition];

    // Keep trace if global module reset was done or not
    board->resetdone = 0;

    // Default operation is alpha board
    board->beta = 0;

    // This shows that we are not currently in the reset procedure
    board->resetinprog = 0;

    // Keep trace if module calib was done correctly or not
    board->calibdone = 0;

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Clear mask used to check calibration (default all 16 channels lanes)
    board->channellanemask = 0xffffffff;

    // Set default randomize mode
    board->randommode = 0;

    // Keep trace if test mode is active
    board->testmode = 0;

    // By default, all 16 channels are used
    board->groupch = MI125_16CHANNELS;

    // Save the board position (top or bottom)
    board->fmcposition = (MI125_mi125_fmc_position)fmcposition%2;

    // Save the FMC connector number
    board->FmcConnector = (Carrier_eFmcConnectorNum_t)(fmcposition/2);

    // Get the I2C device addresses
    ret = _MI125_mi125_get_i2c_addr(board, &i2c_dev_list);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Get the I2C zone based on the FMC position
    ret = Carrier_FmcGetI2cZone(board->hCarrier, board->FmcConnector, &I2cZone);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the communication channel for other devices that are i2c based
    // For the moment, the expander & expander2 addresses are hard coded
    // generated from the known fix part of device address
    ret = MI125_com_open(&board->com, hCarrier, I2cZone,
                    i2c_dev_list.spibridge,
                    i2c_dev_list.sensor,
                    (i2c_dev_list.expander & 0x07) | PCF8574_I2CID,
                    (i2c_dev_list.expander & 0x07) | PCA9535_I2CID,
                    i2c_dev_list.eeprom);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the io expander module (alpha boards)
    ret = MI125_pcf8574_open(&board->expander, &board->com);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the io expander2 module (beta boards and later)
    ret = MI125_pca9535_open(&board->expander2, &board->com);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the adc module
    ret = MI125_ltm9012_open(&board->adc, &board->com);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Open the temperature monitor module
    ret = MI125_lm75_open(&board->monitor, &board->com);

    FUNC_EXIT();

    return ret;
}


Result MI125_mi125_set_datapaths_calibration(MI125_mi125 * board, MI125_mi125_IoDelayRef Reference,
                                             unsigned short *pausLaneDelays,
                                             unsigned char *pucBottomDataRefIoDelay,
                                             unsigned short *pusBottomRefDelay,
                                             unsigned short *pusBottomDcoDelay,
                                             unsigned char *pucBottomBitslipIndex)
{
    Result ret = MI125_MI125OK;

    FUNC_ENTER();

    // Save the lanes offset delays
    board->pausLaneDelays = pausLaneDelays;

    // Save the core reference clock
    board->Reference = Reference;

    // Saves the reference tap delay of bottom board
    board->pucBottomDataRefIoDelay = pucBottomDataRefIoDelay;
    
    // Saves a pointer on the bottom reference line lane delay in ps
    board->pusBottomRefDelay = pusBottomRefDelay;
    
    // Saves a pointer on the bottom dco line lane delay in ps
    board->pusBottomDcoDelay = pusBottomDcoDelay;

    // Saves a pointer on the bottom dco line lane delay in ps
    board->pucBottomBitslipIndex = pucBottomBitslipIndex;

    // Keep trace if using default taps delay or not
    board->defaulttaps = 0;

    // Get dco delay in ps
    board->usDcoDelay = board->pausLaneDelays[Mi125_aucDcoFmc];

    FUNC_EXIT();

    return ret;
}


Result MI125_mi125_reset(MI125_mi125 * board)
{
    Result ret = MI125_MI125OK;
    int jj;
    MI125_mi125_clock_source clksrc;

    FUNC_ENTER();

    // Keep trace if global module reset was done or not
    board->resetdone = 0;

    // Default operation is alpha board
    board->beta = 0;

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Clear mask used to check calibration (default all 16 channels lanes)
    board->channellanemask = 0xffffffff;

    // Keep trace if calib was made correctly or not
    board->calibdone = 0;

    // By default, all 16 channels are used
    board->groupch = MI125_16CHANNELS;

#ifndef SIMUL
    // Check if the core is correct MI125 type
    if (board->core->Register_mi125info.Bit.coreid != MI125_COREID_MI125)
    {
        FUNC_EXIT();
        return(MI125_MI125NOCORE);
    }
#endif

#ifndef SIMUL
    // Reset the mi125 core
    board->core->Register_mi125ctrl.Bit.IPSoftRst = 1;

    // Reset the mi125 core clock MCM
    board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 1;

    // Wait small delay
    Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);

    // Remove the core reset
    board->core->Register_mi125ctrl.Bit.IPSoftRst = 0;

    // Remove core MCM clock reset
    board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 0;

    // Configure the trigger delay to compensate with adc lag and internal fpga delays
    board->core->Register_mi125ctrl.Bit.TriggerDelay = MI125_TRIG_ADC_SYNC_DELAY;

    // Configure the hardware with the number of active channels group
    board->core->Register_mi125ctrl.Bit.ChannelConfig = board->groupch;
    // Set all channel to not valid
    board->core->Register_mi125ctrl.Bit.AdcValid = 0x0000;
    // Update ChannelConfig and AdcValid values
    board->core->Register_mi125ctrl.Bit.UpdateADCStatus = 1;
#endif

    // Attempt to detect the board type (will set the beta member according to detection)
    ret = _MI125_mi125_detectbetaboard(board);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset the io expander according to board type
    if (board->beta)
    {
        // Reset the io expander
        ret = MI125_pca9535_reset(&board->expander2);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

        // Because it is possible a beta board could have the bridge in reset before
        // resetting the io expander, an additionnal delay before the bridge is ready
        // could be needed to make sure that the bridge is ready for operation.
        // Attempt to initialize the com channel and the bridge (is it ready for used?);
        ret = MI125_com_init(&board->com);
        if(FAILURE(ret))
        {
            // Wait the worst time bridge reset delay
            Carrier_usleep(board->hCarrier, MI125_BRIDGESTART_DELAY);

            // Retry to configure the bridge which now must be ready
            ret = MI125_com_init(&board->com);
        }
    }
    else
    {
        // Initialize the com channel and the bridge
        ret = MI125_com_init(&board->com);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

        // Reset the io expander
        ret = MI125_pcf8574_reset(&board->expander);
    }
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // If our board is at top position, the clk source could not be anything else than bottom fmc clk
    // because of an fpga clock distribution limitation when not a beta board
    if (board->fmcposition != MI125_BOTTOM)
    {
        // For top board, the clk source could not be different than MI125_CLKSRCBOTTOMFMC
        // if the top board is not the clock master.
        if (board->core->Register_mi125status.Bit.MI125ClkMaster == 0)
        {
            clksrc = MI125_CLKSRCBOTTOMFMC;
        }
        else
        {
            if (board->beta)
            {
                // We are the clock master so we can start on our own clock
                clksrc = MI125_CLKSRC125MHZ;
            }
            else
            {
                // Before beta board, top board cannot be a clock master...
                ret = MI125_MI125INVALIDCLKTOP;
                FUNC_EXIT();
                return ret;
            }
        }
    }
    else
    {
        if (board->core->Register_mi125status.Bit.MI125ClkMaster == 0)
        {
            clksrc = MI125_CLKSRCEXT;
        }
        else
        {
			// Default for bottom adapter
			clksrc = MI125_CLKSRC125MHZ;
        }
    }

    // Change the clksrc according to board type
    if (board->beta)
    {
        MI125_pca9535_localoscsw sw;

        // Check if we don't use the internal oscillator and turn it off to lower noise
        if (clksrc == MI125_CLKSRC125MHZ)
        {
            sw = MI125_PCA9535_LOSWON;
            ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
            if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

            // Make sure the local oscillator has started and stabilized
            // Wait for the bigger of MI125_LOCALOSC_DELAY or MI125_BUF_CLOCK_PWRN
            Carrier_usleep(board->hCarrier, (MI125_LOCALOSC_DELAY > MI125_BUF_CLOCK_PWRN ? MI125_LOCALOSC_DELAY : MI125_BUF_CLOCK_PWRN));
        }
        else
        {
            sw = MI125_PCA9535_LOSWOFF;
            ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
            if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

            // Make sure the clock switch is power correctly after the io expander reset
            // before using _MI125_mi125_setclkbuf14fanout()...
            Carrier_usleep(board->hCarrier, MI125_BUF_CLOCK_PWRN);
        }

        ret = MI125_pca9535_setclksource(&board->expander2, clksrc);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
        ret = _MI125_mi125_setclkbuf14fanout(board);
    }
    else
        ret = MI125_pcf8574_setclksource(&board->expander, clksrc);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset each adc
    for (jj = 0; jj <= board->groupch; jj++)
    {
        // Reset the adc
        ret = MI125_ltm9012_reset(&board->adc, MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // Set fpga to match adc data format to record correctly in memory (two complement)
    board->core->Register_mi125ctrl.Bit.DataFormat = 1;

    // Set default randomize mode
    board->randommode = 0;

    // Set fpga to match adc randomize mode
    board->core->Register_mi125ctrl.Bit.DigOutRandEn = board->randommode;

    // Reset the temp monitor device to default configuration
    ret = MI125_lm75_reset(&board->monitor);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset the clock mcm before the calibration
    ret = MI125_mi125_clockreset(board);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // This shows that we are currently in the reset procedure (to force a calibration anyway)
    board->resetinprog = 1;

    // Digital output calibration
    ret = MI125_mi125_digital_adccalibration(board);

    // Remove what we added before
    board->resetinprog = 0;

    if(SUCCESS(ret))
        // Keep trace if global module reset was done or not
        board->resetdone = 1;

    FUNC_EXIT();

    return ret;
}


Result MI125_mi125_close(MI125_mi125 * board)
{
    Result ret = MI125_MI125OK;

    FUNC_ENTER();

    // Keep trace if global module reset was done or not
    board->resetdone = 0;

    // This shows that we are not currently in the reset procedure
    board->resetinprog = 0;

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Clear mask used to check calibration (default all 16 channels lanes)
    board->channellanemask = 0xffffffff;

    // Keep trace if calib was made corerectly or not
    board->calibdone = 0;

    // Close all board devices
    ret =  MI125_lm75_close(&board->monitor);
    ret =  MI125_ltm9012_close(&board->adc);
    ret =  MI125_pcf8574_close(&board->expander);
    ret =  MI125_pca9535_close(&board->expander2);
    ret =  MI125_com_close(&board->com);

    FUNC_EXIT();

    return ret;
}


Result MI125_mi125_digital_adccalibration(MI125_mi125 * board)
{
    Result ret = MI125_MI125OK;
    int jj, kk, val;
    unsigned int caliben=0xffffffff, calibrated=0;
    unsigned int bitslipEn=0xffffffff;
    unsigned int iteration_calib_detection_done;
    unsigned char delayGroupCnt = 0;
    unsigned char delayGroupMax = 0;
    unsigned char delayGroupMiddle;
    int min_border, max_border;
    int index;
    unsigned int first_tap_offset = 16;
    int iBit, iActAsMaster;
    int iCountStart;
    unsigned uExcursionMax, uTemp;
    unsigned char ucDist, ucRefCalib;
    unsigned short usMin, usMax;

    FUNC_ENTER();

    // Reset error delay to 0
    memset(&board->errorDelay[0],0,sizeof(board->errorDelay));
    
    // Maximum excursion calculation
    // Calculation based on excursion = (1 / (SerialDataClockFrequency * pBoard->Reference * 1e-12 ))
    // !!! Improvement would be to use read data rate frequency here and not the max one
    uTemp =  MI125_MAX_DATA_RATE / 1000 * board->Reference;
    uExcursionMax = 1000000000 / uTemp;

    // Default value for tap delay calibration statistics
    board->ucDataSmallestPosDcoTol = 0xff;
    board->ucDataSmallestNegDcoTol = 0xff;
    board->ucDataSmallestDelay = 0xff;
    board->ucDataGreatestDelay = 0;
    
    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;
    
    // Set all channel to not valid
    board->core->Register_mi125ctrl.Bit.AdcValid = 0x0000;
    // Update ChannelConfig and AdcValid values
    board->core->Register_mi125ctrl.Bit.UpdateADCStatus = 1;

    // Keep trace if calib was made correctly or not
    board->calibdone = 0;

    // Check if the board has been reset correctly at least once in not currently in reset!
    if (!board->resetdone && board->resetinprog == 0)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // 0. Make sure the adc pll is stabilized if any clock changed was done (min 25 usec)
    Carrier_usleep(board->hCarrier, MI125_ADCCLKPLL_DELAY);

    // Calibration Steps
    // 1. Reset FPGA complete clock logic
#ifndef SIMUL
    // Reset the mi125 core clock MCM
    //board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 1;

    // Wait small delay
    //Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);

    // Remove core MCM clock reset
    //board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 0;
#endif

    // Testmode would be disable by this routine
    board->testmode = 0;

    // Set fpga to match adc randomize mode OFF
    board->core->Register_mi125ctrl.Bit.DigOutRandEn = 0;

    // 2. Setup used ADC correctly with test pattern and calib test mode
    // Setup each adc
    for (jj = 0; jj <= board->groupch; jj++)
    {
        // Set adc in calib mode
        ret = MI125_ltm9012_setdigitalcalib(&board->adc, MI125_LTM9012_DCALIBON, MI125_ADCCALIB, MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // 3. Reset IdelayCtrlRst and IdelayRst.
#ifndef SIMUL
    // These resets are positive edge trigger
    board->core->Register_mi125ctrl.Bit.IdelayCtrlRst = 0;
    board->core->Register_mi125ctrl.Bit.IdelayRst = 0;
    board->core->Register_mi125ctrl.Bit.IserdesRst = 0;

    // Wait small delay
    Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);

    board->core->Register_mi125ctrl.Bit.IdelayCtrlRst = 1;
    board->core->Register_mi125ctrl.Bit.IdelayRst = 1;
    board->core->Register_mi125ctrl.Bit.IserdesRst = 1;

    // Wait small delay
    Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);

    board->core->Register_mi125ctrl.Bit.IdelayCtrlRst = 0;
    board->core->Register_mi125ctrl.Bit.IdelayRst = 0;
    board->core->Register_mi125ctrl.Bit.IserdesRst = 0;
#endif

    // 4. Check IdelayRdy register is getting to 1
    // Wait at least 3 usec timeout which should be max time after delay reset
#ifndef SIMUL
    Carrier_usleep(board->hCarrier, MI125_ADCIDELAYRDY_DELAY);

    // Check if we got a timeout
    if ((val = board->core->Register_mi125status.Bit.IdelayRdy) == 0)
    {
        FUNC_EXIT();

        // Problem with signal not rising after reset
        return(MI125_MI125IDELAYRDY);
    }

    // 4.5 Check AdcClockMmcmLock register is getting to 1
    // !!! M.T. Wait at least 3??? usec timeout which should be max time after delay reset
    //Carrier_usleep(board->hCarrier, MI125_ADCIDELAYRDY_DELAY);

    // Check if mcm is locked as a security measure
    if ((val = board->core->Register_mi125status.Bit.AdcClockMmcmLock) == 0)
    {
        FUNC_EXIT();

        // Problem with signal not rising after reset
        return(MI125_MI125MMCMLOCK);
    }

    iActAsMaster = board->core->Register_mi125status.Bit.MI125ClkMaster;

    // Are we doing as a clock master (or using default older calib)?
    if (iActAsMaster || board->defaulttaps)
    {
        // For a clock master board, this will attempt to calibrate the reference lane 1st 
        iCountStart = -4;
        
        board->ucBitslipIndex = 12;

        // Are we using default older perseus 601x calib?
        if (!board->defaulttaps)
        {
            // Find the reference data lane
            ret = _MI125_mi125_GetDelayAnalysis(board->pausLaneDelays, &usMin, &usMax, 1, &board->ucDataRefLane);
            
            // Checks if we got an error?
            if (FAILURE(ret))
            {
                FUNC_EXIT();
                return(ret);
            }
    
            // Saves the reference lane delay in ps
            board->usRefDelay = board->pausLaneDelays[Mi125_aucDataFmc[board->ucDataRefLane]];
    
            // Calculates each lane delay correction to apply (using no dco global correction)
            ret = _MI125_mi125_CalculateSignalPaths(board->pausLaneDelays, &board->lanedelays.laneoffset[0], board->usRefDelay, board->Reference, 0);
    
            // Checks if we got an error?
            if (FAILURE(ret))
            {
                FUNC_EXIT();
                return(ret);
            }
            
            // We are trying to calibrate our reference lane in the middle of the possible taps
            board->iFirstTapOffsetFeed = 16;
        }
    }
    else
    {
        // We are a clock slave, our reference lane is coming from other clock master
        iCountStart = 0;

        // We do not use our own reference but bottom one
        board->ucDataRefLane = 255;
        board->ucDataRefIoDelay = 0;
        
        // Checks if the pointer is valid?
        if (board->pucBottomDataRefIoDelay == 0)
        {
            FUNC_EXIT();
            return(MI125_MI125BADBOTTAPREFPTR);
        }
        
        // We are getting the value from the master board
        board->iFirstTapOffsetFeed = *board->pucBottomDataRefIoDelay;
        
        // Checks if bottom reference tap delay value is valid
        if (board->iFirstTapOffsetFeed > 31 || board->iFirstTapOffsetFeed < 0)
        {
            FUNC_EXIT();
            return(MI125_MI125BADBOTTAPREFVALUE);
        }

        // Saves the reference lane delay in ps
        board->usRefDelay = 0;

        // Checks if the pointer is valid?
        if (board->pusBottomRefDelay == 0)
        {
            FUNC_EXIT();
            return(MI125_MI125BADBOTREFDELAYPTR);
        }
        
        // Checks if the pointer is valid?
        if (board->pusBottomDcoDelay == 0)
        {
            FUNC_EXIT();
            return(MI125_MI125BADBOTDCODELAYPTR);
        }
        
        // Calculates each lane delay correction to apply
        ret = _MI125_mi125_CalculateSignalPaths(board->pausLaneDelays, &board->lanedelays.laneoffset[0],
                                           *board->pusBottomRefDelay, board->Reference,
                                           /*((int)(*board->pusBottomDcoDelay) - (int)(board->pausLaneDelays[Mi125_aucDcoFmc])) / (int)(board->Reference)*/ 0);
        /*
         * JROY: The DCO calculation code has been replaced by 0
         * since the DCO signal from the clock slave module are not used.
         * It is the DCO from the clock master module that is used by all
         * the clock slave modules.
         */
         
        board->ucBitslipIndex = *board->pucBottomBitslipIndex;

        // Checks if we got an error?
        if (FAILURE(ret))
        {
            FUNC_EXIT();
            return(ret);
        }
    }
    
    // This means the reference has not been calibrated
    ucRefCalib = 0;
    
    // Set the clock delay in the middle of its range to have room in both directions
    board->core->Register_mi125adcidelayvalue.Bit.clkIdelay_value = 16;
    
    // Try to calibrate to a max number of trials if we are getting problems
    do
    {
begin:
        // Get an analysis of stability for all lanes that need to be calibrated, for all fpga taps delays
        for(jj=0; jj<32; jj++)
        {
            // Set IDELAY value to current iteration number
            board->core->Register_mi125adcidelayvalue.Bit.adcIdelay_value = jj;

            // Apply to all lanes
            board->core->Register_mi125adcidelaywe.Bit.adcIdelay_we = caliben;

            // Reset calibration detection process
            board->core->Register_mi125ctrl.Bit.reset_calib_detection = 1;
            Carrier_usleep(board->hCarrier, MI125_CALIB_DETECTION_DELAY);
            board->core->Register_mi125ctrl.Bit.reset_calib_detection = 0;
            Carrier_usleep(board->hCarrier, MI125_CALIB_DETECTION_DELAY);

            // Wait for the calibration detection process to be done
            iteration_calib_detection_done = 0;
            while (board->core->Register_mi125status.Bit.calib_detection_done != 1)
            {
                // If it excess maximum iteration number, return an error
                if (iteration_calib_detection_done > MAX_ITERATION_CALIB_DETECTION_DONE)
                    return(MI125_MI125TRAININGDONE);

                Carrier_usleep(board->hCarrier, MI125_CALIB_DETECTION_DELAY);
                iteration_calib_detection_done++;
            }
            
            // Sleep to let time to the other calibration processes to finish
            // since the calib_detection_done flag is only on the first lane
            Carrier_usleep(board->hCarrier, MI125_CALIB_DETECTION_DELAY);

            // Save current idelay value result for calibrated lanes only
            board->errorDelay[jj] &= (~caliben);
            board->errorDelay[jj] |= (board->core->Register_mi125caliberror.Bit.calib_error & caliben);

            PDEBUG("\tDelay %2d: 0x%08x\n",jj,board->errorDelay[jj]);
        }
        // Is the reference calibrated
        if (!ucRefCalib)
            first_tap_offset = board->iFirstTapOffsetFeed;
        
        // Find the longest valid delay group and its middle value for all lanes
        // Steps -2 and -1 are needed to correctly calibrates the reference lane
        for(jj=iCountStart; jj<32; jj++)
        {
            delayGroupCnt = 0;
            delayGroupMax = 0;

            // We do not need to calibrate the reference lane (it was made before)
            if (jj == board->ucDataRefLane || (jj < 0 && ucRefCalib != 0))
                continue;
                    
            // Are we at reference calibration steps?
            if (jj < 0)
                // Yes, use the reference lane
                index = board->ucDataRefLane;
            else
            {
                // Did we need a reference lane calibrated
                if (iCountStart < 0 && !ucRefCalib)
                    // Oups we need it so abort analysis for next trial
                    break;

                // No, use the current lane
                index = jj;
            }
                
            // Generate a bit match for the bit position
            iBit = 1 << index;
                 
            // If calibration is not perform on the current lane skip it
            if ((caliben & iBit) == 0)
                continue;

            min_border = first_tap_offset + (int)board->lanedelays.laneoffset[index] - uExcursionMax / 2;
            max_border = first_tap_offset + (int)board->lanedelays.laneoffset[index] + uExcursionMax / 2;

            if (min_border < 0)
                min_border = 0;

            if (max_border > 31)
                max_border = 31;
            
            // Loop only on subset of taps and the selected taps are offseted depending of the board lane delays
            for(kk=min_border; kk<=max_border; kk++)
            {
                // If the current iodelay value for this tap is a valid value
                if ((board->errorDelay[kk] & iBit) == 0)
                {
                    // Increment current group counter and if it is the biggest group
                    // save its middle value
                    delayGroupCnt++;
                    
                    if(kk<max_border) continue;
                }
                
                // Did we found a group?
                if (delayGroupCnt != 0)
                {
                    // Biggest group found so far?
                    if(delayGroupCnt > delayGroupMax)
                    {
                        delayGroupMax = delayGroupCnt;
                        delayGroupMiddle = kk - 1 - delayGroupCnt / 2;

                        // Set IDELAY value to current solution
                        board->ucDataIoDelTable[index] = delayGroupMiddle;
                        board->ucDataIoGroupCount[index] = delayGroupCnt;
                        board->ucDataIoGroupEnd[index] = (kk - 1);
                        board->core->Register_mi125adcidelayvalue.Bit.adcIdelay_value = delayGroupMiddle;

                        // Apply to the current lane
                        board->core->Register_mi125adcidelaywe.Bit.adcIdelay_we = iBit;

                        // Adjust reference lane tap offset according to the middle value find for it
                        if(jj == -1)
                        {
                            first_tap_offset = delayGroupMiddle;
                            board->ucDataRefIoDelay = delayGroupMiddle;
                        }

                        // If the valid group is >= 3, remove the lane for the next calibration try
                        // since a valid solution has been found
                        if (delayGroupCnt>=3)
                        {
                            if (jj >= 0)
                            {
                                // This is any lane but reference calibration
                                caliben &= ~(iBit);
                                
                            }
                            else
                            {
                                // This is reference lane calibration
                                if (jj == -1)
                                {
                                    caliben &= ~(iBit);
                                    ucRefCalib = 1;
                                }
                            }
                        }
                    }

                    delayGroupCnt = 0;
                }
            }

            if(jj < -1)
            {
                unsigned char clock_delay;
				clock_delay = board->core->Register_mi125adcidelayvalue.Bit.clkIdelay_value;

				// We want the reference lane delay to be at 16
				// So we move the clock delay the compensate for the difference
				// between 16 and the eye middle value found
				clock_delay += 16 - board->ucDataIoDelTable[index];
				PDEBUG("Found the reference lane delay should be %d.\n",board->ucDataIoDelTable[index]);

				board->core->Register_mi125adcidelayvalue.Bit.clkIdelay_value = clock_delay;
				PDEBUG("The clock delay will be set to %d.\n",clock_delay);

				iCountStart++;
				goto begin;
            }
        }
        
#ifdef MI125_MODDEBUG
    printf("Calibration result\n");
    for(jj=0; jj<32; jj++)
    {
        printf("%2d: %2d, %2d    [%2d,%2d]\n",
            jj,
            board->ucDataIoDelTable[jj],
            board->ucDataIoGroupCount[jj],
            first_tap_offset + (int)board->lanedelays.laneoffset[jj] - uExcursionMax / 2,
            first_tap_offset + (int)board->lanedelays.laneoffset[jj] + uExcursionMax / 2);
    }
#endif

        // Reset calibration detection process
        board->core->Register_mi125ctrl.Bit.reset_calib_detection = 1;
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        board->core->Register_mi125ctrl.Bit.reset_calib_detection = 0;
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);

        // Wait for the calibration detection process to be done
        iteration_calib_detection_done = 0;
        while (board->core->Register_mi125status.Bit.calib_detection_done != 1)
        {
            // If it excess maximum iteration number, return an error
            if (iteration_calib_detection_done > MAX_ITERATION_CALIB_DETECTION_DONE)
                return(MI125_MI125TRAININGDONE);

            Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
            iteration_calib_detection_done++;
        }

        // Verify the current calibration
        // if there are errors, add lanes in error for the next calibration try
        caliben |= board->core->Register_mi125caliberror.Bit.calib_error;

        // Keep trace of the channels lanes that were calibrated
        board->channelslanecalib[board->calibndx] = caliben;

        if (caliben==0)
        	calibrated = 1;
        else
        	calibrated = 0;

        // Compute which lanes to recalibrate
        if (!calibrated)
        {
            PDEBUG("Trial #%02d, to recalibrate: 0x%08X.\n",board->calibndx, caliben);
        }

    // Check if the digital calibration went correctly
    }while(!calibrated && ( (++board->calibndx) < MI125_MAX_CALIB_ATTEMPT));

    if (!calibrated)
    {
        // Set the index inside allowed buffer
        board->calibndx--;
    }

    PDEBUG("Bit slip alignment start\n");
    PDEBUG("\t ucBitslipIndex is currently %d \n",board->ucBitslipIndex);
    // Bitslip alignment of the ISERDES
    for(board->ucBitslipRetry=(board->ucBitslipIndex - 4); board->ucBitslipRetry<(board->ucBitslipIndex + 4); board->ucBitslipRetry++)
    {
        int iFirstBitSlip = -1;
        int iBitslipInitCounter;
        PDEBUG("\n");
        
        // Reset the SERDES
        board->core->Register_mi125ctrl.Bit.IserdesRst = 1;
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        
        // Clear the SERDES reset
        board->core->Register_mi125ctrl.Bit.IserdesRst = 0;
        Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        
        bitslipEn = 0xffffffff;
        
        for(iBitslipInitCounter = board->ucBitslipRetry; iBitslipInitCounter>=0; iBitslipInitCounter--)
        {
            board->core->Register_mi125bitslip.Bit.bitslip = bitslipEn;
            Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
        }

        for(jj=0; jj<8; jj++)
        {
            PDEBUG("Bit slip try #%d, iteration #%d\n",board->ucBitslipRetry,jj);
            
            // Reset calibration detection process
            board->core->Register_mi125ctrl.Bit.reset_calib_detection = 1;
            Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
            board->core->Register_mi125ctrl.Bit.reset_calib_detection = 0;
            Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);

            // Wait for the calibration detection process to be done
            iteration_calib_detection_done = 0;
            while (board->core->Register_mi125status.Bit.calib_detection_done != 1)
            {
                // If it excess maximum iteration number, return an error
                if (iteration_calib_detection_done > MAX_ITERATION_CALIB_DETECTION_DONE)
                {
                    PDEBUG("Error: Bit slip alignment timeout\n");
                    return(MI125_MI125TRAININGDONE);
                }

                Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);
                iteration_calib_detection_done++;
            }
            
            // Detect which lane is not calibrated
            PDEBUG("Bit slip error : 0x%x\n",board->core->Register_mi125calibpatternerror.Bit.calib_pattern_error);
            
            if(bitslipEn != board->core->Register_mi125calibpatternerror.Bit.calib_pattern_error)
            {
                if(iFirstBitSlip == -1)
                {
                    iFirstBitSlip = board->ucBitslipRetry + jj;
                    PDEBUG("\t First bitslip : %d \n",iFirstBitSlip);
                }
                else if((iFirstBitSlip-(board->ucBitslipRetry+jj))>4)
                {
                    PDEBUG("\t Too far apart from first bitslip : %d, %d \n",iFirstBitSlip, jj);
                    // Too far apart
                    bitslipEn = 0xffffffff;
                    break;
                }
            }
            
            bitslipEn = board->core->Register_mi125calibpatternerror.Bit.calib_pattern_error;
            
            // If all lanes are calibrated, exit loop
            if(!bitslipEn)
            {
                break;
            }
                
            // Apply bitslip to each lane not calibrated
            board->core->Register_mi125bitslip.Bit.bitslip = bitslipEn;
        }

        // If all lanes are calibrated, exit loop
        if(!bitslipEn)
        {
            board->ucBitslipIndex = (iFirstBitSlip + (board->ucBitslipRetry + jj))/2;
            PDEBUG("\t Set ucBitslipIndex to %d \n",board->ucBitslipIndex);
            break;
        }
    }
    
    
#endif

    // Disable calibration detection process
    board->core->Register_mi125ctrl.Bit.reset_calib_detection = 1;

    // Set fpga to match adc saved randomize mode
    board->core->Register_mi125ctrl.Bit.DigOutRandEn = board->randommode;

    // 8. Disable ADC digital calib testmode.
    // Setup each adc
    for (jj = 0; jj <= board->groupch; jj++)
    {
        // Set adc in normal mode
        ret = MI125_ltm9012_setdigitalcalib(&board->adc, MI125_LTM9012_DCALIBOFF, MI125_ADCCALIB, MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // Set channel to valid if the calibration of its 2 lanes are valid
    for (jj = 0; jj<16; jj++)
    {
        if (!((bitslipEn >> (2*jj)) & 0x3))
        {
            board->core->Register_mi125ctrl.Bit.AdcValid |= (0x1 << jj);
        }
        else
        {
            board->core->Register_mi125ctrl.Bit.AdcValid &= ~(0x1 << jj);
        }
    }

    // Update ChannelConfig and AdcValid values
    board->core->Register_mi125ctrl.Bit.UpdateADCStatus = 1;

    // Check if the digital calibration went correctly
    if (!calibrated || bitslipEn)
    {
        FUNC_EXIT();

        // Problem with digital calibration of outputs
        return(MI125_MI125TRAININGDONE);
    }

    // Find the smallest eye positive and negative taps tolerance around calibrated middle value
    for(jj = 0; jj < 32; jj++)
    {
        // Calculates positive tap tolerance
        ucDist = board->ucDataIoGroupEnd[jj] - board->ucDataIoDelTable[jj];
        // Update the minimum that we got so far
        if (board->ucDataSmallestPosDcoTol > ucDist)
            board->ucDataSmallestPosDcoTol = ucDist;

        // Calculates negative tap tolerance
        ucDist = board->ucDataIoDelTable[jj] - (board->ucDataIoGroupEnd[jj] + 1 - board->ucDataIoGroupCount[jj]);
        // Update the minimum that we got so far
        if (board->ucDataSmallestNegDcoTol > ucDist)
            board->ucDataSmallestNegDcoTol = ucDist;
            
        // Update smallest calibrated value so far
        if (board->ucDataIoDelTable[jj] < board->ucDataSmallestDelay)
            board->ucDataSmallestDelay = board->ucDataIoDelTable[jj];

        // Update greatest calibrated value so far
        if (board->ucDataIoDelTable[jj] > board->ucDataGreatestDelay)
            board->ucDataGreatestDelay = board->ucDataIoDelTable[jj];
    }

    // Display the number of calibration retries
    PDEBUG("Calibration trials succeeded on #%d.\n", board->calibndx);

    // Keep trace if calib was made correctly or not
    board->calibdone = 1;

    FUNC_EXIT();

    return ret;
}


Result MI125_mi125_set_config(MI125_mi125 * board, MI125_mi125_config *config)
{
    Result ret = MI125_MI125OK;
    MI125_ltm9012_randmode irand;
    MI125_ltm9012_binmode ibin;
    MI125_ltm9012_lvds ilvds;
    MI125_ltm9012_channel chact[4];
    MI125_pcf8574_channel actchannel;
    int jj, maxadc;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Configure the number of active channels
    chact[0] = MI125_LTM9012_4CHANNEL;

    // Validates how many channels are active by group of 4
    switch(config->groupch)
    {
        case MI125_04CHANNELS:
            chact[1] = MI125_LTM9012_0CHANNEL;
            chact[2] = MI125_LTM9012_0CHANNEL;
            chact[3] = MI125_LTM9012_0CHANNEL;
            actchannel = MI125_PCF8574_04CHANNELS;
            board->channellanemask = 0x000000ff;
            break;

        case MI125_08CHANNELS:
            chact[1] = MI125_LTM9012_4CHANNEL;
            chact[2] = MI125_LTM9012_0CHANNEL;
            chact[3] = MI125_LTM9012_0CHANNEL;
            actchannel = MI125_PCF8574_08CHANNELS;
            board->channellanemask = 0x0000ffff;
            break;

        case MI125_12CHANNELS:
            chact[1] = MI125_LTM9012_4CHANNEL;
            chact[2] = MI125_LTM9012_4CHANNEL;
            chact[3] = MI125_LTM9012_0CHANNEL;
            actchannel = MI125_PCF8574_12CHANNELS;
            board->channellanemask = 0x00ffffff;
            break;

        case MI125_16CHANNELS:
            chact[1] = MI125_LTM9012_4CHANNEL;
            chact[2] = MI125_LTM9012_4CHANNEL;
            chact[3] = MI125_LTM9012_4CHANNEL;
            actchannel = MI125_PCF8574_16CHANNELS;
            board->channellanemask = 0xffffffff;
            break;

        default:
            // Invalid channel config
            FUNC_EXIT();
            return(MI125_MI125INVALIDCH);
    }

    // Check if we added more channels to the group than what we currently have?
    if (config->groupch > board->groupch)
        // This means we need more channels to be actived than before (add more)
        maxadc = config->groupch;
    else
        maxadc = board->groupch;

    // Configure each adc for the channels used
    for (jj = 0; jj <= maxadc; jj++)
    {
        // Configure how many channels are used for each adc
        ret = MI125_ltm9012_setchannelmode(&board->adc, chact[jj], MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // Apply external adc shutdown for the unused adc devices
    if (board->beta)
        ret = MI125_pca9535_setshutdown(&board->expander2, actchannel);
    else
        ret = MI125_pcf8574_setshutdown(&board->expander, actchannel);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Configure the hardware with the number of active group
    board->core->Register_mi125ctrl.Bit.ChannelConfig = config->groupch;
    board->core->Register_mi125ctrl.Bit.UpdateADCStatus = 1;

    // Valid lvds settings
    switch(config->lvds)
    {
        case MI125_TERMOFF1750UA:
        case MI125_TERMOFF2100UA:
        case MI125_TERMOFF2500UA:
        case MI125_TERMOFF3000UA:
        case MI125_TERMOFF3500UA:
        case MI125_TERMOFF4000UA:
        case MI125_TERMOFF4500UA:
        case MI125_TERMON1750UA:
        case MI125_TERMON2100UA:
        case MI125_TERMON2500UA:
            ilvds = config->lvds;
            break;
        default:
            // Invalid lvds configuration
            FUNC_EXIT();
            return(MI125_MI125INVALIDLVDS);
    }

    // Valid randomize feature
    switch(config->randmode)
    {
        case MI125_RANDOMIZEOFF:
            irand = config->randmode;

            // Set fpga to match adc randomize mode OFF
            board->core->Register_mi125ctrl.Bit.DigOutRandEn = 0;

            // Save fpga randomize mode
            board->randommode = 0;
            break;
        case MI125_RANDOMIZEON:
            irand = config->randmode;

            // Set fpga to match adc randomize mode ON
            board->core->Register_mi125ctrl.Bit.DigOutRandEn = 1;

            // Save fpga randomize mode
            board->randommode = 1;
            break;
        default:
            // Invalid output randomize mode
            FUNC_EXIT();
            return(MI125_MI125INVALIDRAN);
    }

    // Valid output binary mode feature
    switch(config->binmode)
    {
        case MI125_OFFSETBINARYFORMAT:
            ibin = config->binmode;

            // Set fpga to match adc data format to record correctly in memory
            board->core->Register_mi125ctrl.Bit.DataFormat = 0;
            break;
        case MI125_TWOCOMPLEMENTFORMAT:
            ibin = config->binmode;

            // Set fpga to match adc data format to record correctly in memory
            board->core->Register_mi125ctrl.Bit.DataFormat = 1;
            break;
        default:
            // Invalid output binary format mode
            FUNC_EXIT();
            return(MI125_MI125INVALIDBIN);
    }

    // Configure each adc for the channels used
    for (jj = 0; jj <= config->groupch; jj++)
    {
        // Configure how many channels are used for each adc
        ret = MI125_ltm9012_setconfig(&board->adc, irand, ibin, ilvds,  MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    // Check if we added more channels to the group, and so need to redo the digital calibration
    if (config->groupch > board->groupch)
    {
        // Save the number of channels in the group
        board->groupch = config->groupch;

        // If new channels are added, their adc must stabilize 1st by going out of sleep mode
        Carrier_usleep(board->hCarrier, MI125_ADCSLEEPOUT_DELAY);

        // Digital output calibration
        ret = MI125_mi125_digital_adccalibration(board);
    }
    else
        // Save the number of channels in the group
        board->groupch = config->groupch;

    FUNC_EXIT();

    return(ret);
}


Result MI125_mi125_set_clksrc(MI125_mi125 * board,MI125_mi125_clock_source clksrc)
{
    Result ret = MI125_MI125OK;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // For any board type we could have at bottom position, the clk source could not be bottom fmc
    // This is only possible for top adapter
    if ((board->fmcposition == MI125_BOTTOM && clksrc == MI125_CLKSRCBOTTOMFMC))
    {
        FUNC_EXIT();
        return(MI125_MI125INVALIDCLKBOT);
    }

    // For top board, the clk source could not be different than MI125_CLKSRCBOTTOMFMC
    // if the top board is not the clock master. When not a beta board, the top clk source
    // must be MI125_CLKSRCBOTTOMFMC
    if ((board->fmcposition != MI125_BOTTOM && clksrc != MI125_CLKSRCBOTTOMFMC) &&
        (board->core->Register_mi125status.Bit.MI125ClkMaster == 0 || board->beta == 0))
    {
        FUNC_EXIT();
        return(MI125_MI125INVALIDCLKTOP);
    }

    // Validates the clock src
    switch(clksrc)
    {
        case MI125_CLKSRC125MHZ:
        case MI125_CLKSRCEXT:
        case MI125_CLKSRCBOTTOMFMC:
        case MI125_CLKSRCFMCCARRIER:
            break;

        default:
            // Invalid clock source
            FUNC_EXIT();
            return(MI125_MI125INVALIDCLK);
    }

    // Change the clock source
    ret = _MI125_mi125_isetclksrc(board,clksrc);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset the clock mcm before the calibration
    ret = MI125_mi125_clockreset(board);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Digital output calibration must be done again because of clock change
    ret = MI125_mi125_digital_adccalibration(board);

    FUNC_EXIT();

    return(ret);
}


Result MI125_mi125_set_trigout(MI125_mi125 * board, MI125_mi125_trigout trigout)
{
    Result ret = MI125_MI125OK;
    MI125_pcf8574_trigout iout;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Validates trigger out
    switch(trigout)
    {
        case MI125_TRIGOUTON:
        case MI125_TRIGOUTOFF:
            iout = trigout;
            break;

        default:
            // Invalid trigger out configuration
            FUNC_EXIT();
            return(MI125_MI125INVALIDTRIG);
    }

    // Change the trig out configuration
    if (board->beta)
        // Trigout configuration is inverted as compared to original alpha board
        ret = MI125_pca9535_settrgout(&board->expander2, (iout ^ 1));
    else
        ret = MI125_pcf8574_settrgout(&board->expander, iout);

    FUNC_EXIT();

    return(ret);
}


Result MI125_mi125_set_testmode(MI125_mi125 * board, MI125_mi125_testmode test, unsigned short pattern)
{
    Result ret = MI125_MI125OK;
    int jj;
    MI125_ltm9012_dcalib calib;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Check if the last board calibration was successfull: this is critical for the test mode
    // MI125_TESTMODE2 to work correctly
    if (!board->calibdone && test == MI125_TESTMODE2)
    {
        FUNC_EXIT();
        return(MI125_MI125NOCALIB);
    }

    // Check for test mode parameter
    switch(test)
    {
        case MI125_TESTMODE1:
        case MI125_TESTMODE2:
            calib = MI125_LTM9012_DCALIBON;
            board->testmode = test;

            // Set fpga to match adc randomize mode OFF
            board->core->Register_mi125ctrl.Bit.DigOutRandEn = 0;
            break;
        case MI125_TESTMODEOFF:
            calib = MI125_LTM9012_DCALIBOFF;
            board->testmode = test;

            // Set fpga to match adc saved randomize mode
            board->core->Register_mi125ctrl.Bit.DigOutRandEn = board->randommode;
            break;

        default:
            // Invalid test mode setting
            FUNC_EXIT();
            return(MI125_MI125INVALIDTEST);
    }

    // Setup ADC correctly with test pattern and calib test mode
    // Setup each adc
    for (jj = 0; jj <= board->groupch; jj++)
    {
        // Set adc in calib mode
        ret = MI125_ltm9012_setdigitalcalib(&board->adc, calib, pattern, MI125_COM_ADC0 + jj);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}
    }

    FUNC_EXIT();

    return ret;
}


Result MI125_mi125_get_temperature(MI125_mi125 * board, MI125_mi125_tempmode mode, short* temp)
{
    Result ret = MI125_MI125OK;

    FUNC_ENTER();

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Read pcb temperature
    ret = MI125_lm75_readtemp10x(&board->monitor, temp);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // If temperature mode is rounding to Celsius
    if (mode == MI125_TEMP1C)
        *temp = *temp / 10;

    FUNC_EXIT();

    return(ret);
}


Result MI125_mi125_get_channelcalibstatus(MI125_mi125 * board, unsigned int* channellanecalib, unsigned int* channelcalibstatus)
{
#if MI125_MODDEBUG
    int indx;
#endif

    FUNC_ENTER();

    // Get the current lane calibration status for the active channels
    *channellanecalib = (board->channelslanecalib[board->calibndx] & board->channellanemask);

#if MI125_MODDEBUG
    for (indx = 0; indx <= board->calibndx; indx++)
            PDEBUG("Trial #%02d, calib status: 0x%08X.\n",indx, (board->channelslanecalib[indx] & board->channellanemask));
#endif

    // Keep trace if calib was made correctly or not
    *channelcalibstatus = board->calibdone;

    FUNC_EXIT();

    return(MI125_MI125OK);
}


Result MI125_mi125_get_versions(MI125_mi125 * board, unsigned int* core_version, unsigned int *drv_version)
{

    FUNC_ENTER();

    // Get the MI125 hardware core version
    *core_version = board->core->Register_mi125info.Bit.version;

    // Get the MI125 software driver version
    *drv_version = MI125_DVERSION;

    FUNC_EXIT();

    return(MI125_MI125OK);
}


Result MI125_mi125_checkadc(MI125_mi125 * board, unsigned char *written, unsigned char *verify, unsigned char *detected, MI125_mi125_adcid adcid)
{
    Result ret = MI125_MI125OK;
    MI125_COM_DEVICES adcdev;

    FUNC_ENTER();

    *detected = 0;

    // Check if test mode is active
    if (board->testmode)
    {
        FUNC_EXIT();
        return(MI125_MI125TESTMODEACT);
    }

    // Initialize the com channel and the bridge if not already done
    if (board->beta)
    {
        // Because it is possible a beta board could have the bridge in reset before
        // resetting the io expander, an additionnal delay before the bridge is ready
        // could be needed to make sure that the bridge is ready for operation.
        // Attempt to initialize the com channel and the bridge (is it ready for used?);
        ret = MI125_com_init(&board->com);
        if(FAILURE(ret))
        {
            // Wait the worst time bridge reset delay
            Carrier_usleep(board->hCarrier, MI125_BRIDGESTART_DELAY);

            // Retry to configure the bridge which now must be ready
            ret = MI125_com_init(&board->com);
        }
    }
    else
    {
        // Initialize the com channel and the bridge
        ret = MI125_com_init(&board->com);
    }
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Check for adc selected
    switch(adcid)
    {
        case MI125_ADC0:
        case MI125_ADC1:
        case MI125_ADC2:
        case MI125_ADC3:
            adcdev = adcid;
            break;

        default:
            // Invalid ADC id
            FUNC_EXIT();
            return(MI125_MI125INVALIDADC);
    }

    // Do the adc check test (to verify adc presence)
    ret = MI125_ltm9012_checkadc(&board->adc, written, verify, detected, adcdev);

    FUNC_EXIT();

    return(ret);
}


Result MI125_mi125_checkcore(MI125_mi125 * board, unsigned char *detected)
{
    Result ret = MI125_MI125OK;

    FUNC_ENTER();

#ifndef SIMUL
    // Check if the core is correct MI125 type
    *detected = (board->core->Register_mi125info.Bit.coreid == MI125_COREID_MI125);
#else
    *detected = 1;
#endif

    FUNC_EXIT();

    return(ret);
}


Result MI125_mi125_get_clkmaster(MI125_mi125 * board, unsigned int *master)
{
    Result ret = MI125_MI125OK;

    FUNC_ENTER();

#ifndef SIMUL
    // Check if this fpga core instance is the clock master or not in a multiple boards configuration
    *master = board->core->Register_mi125status.Bit.MI125ClkMaster;
#else
    *master = 1;
#endif

    FUNC_EXIT();

    return(ret);
}


Result MI125_mi125_clockreset(MI125_mi125 * board)
{
    Result ret = MI125_MI125OK;
    int val;

    FUNC_ENTER();

    // Keep trace of the number of calibration trials
    board->calibndx = 0;

    // Keep trace of the channels lane that were calibrated correctly the last time it was done
    board->channelslanecalib[board->calibndx] = 0;

    // Keep trace if calib was made correctly or not
    board->calibdone = 0;

#ifndef SIMUL
    // Reset the mi125 core clock MCM
    board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 1;

    // Wait small delay
    Carrier_usleep(board->hCarrier, MI125_ADCSMALL_DELAY);

    // Remove core MCM clock reset
    board->core->Register_mi125ctrl.Bit.ADCClockMMCMRst = 0;

    // Check AdcClockMmcmLock register is getting to 1
    Carrier_usleep(board->hCarrier, MI125_ADCMCMCLOCKRDY_DELAY);

    // Check if we got a timeout
    if ((val = board->core->Register_mi125status.Bit.AdcClockMmcmLock) == 0)
    {
        FUNC_EXIT();

        // Problem with signal not rising after reset
        return(MI125_MI125MMCMLOCK);
    }
#endif

    FUNC_EXIT();

    return ret;
}


Result MI125_mi125_scan_i2c(MI125_mi125 * board, unsigned char scanmax, MI125_mi125_i2cscan *scanraw,
                            MI125_mi125_i2caddr *scan_res, unsigned char *ndevices, unsigned char *detect_ok)
{
    Result ret = MI125_MI125OK;
    unsigned char devaddr;

    FUNC_ENTER();

    // Initialize to no detection
    memset(scanraw,0,sizeof(MI125_mi125_i2cscan));
    memset(scan_res,0,sizeof(MI125_mi125_i2caddr));
    *detect_ok = 0;
    *ndevices=0;

    // Scan every 7 bits i2c addresses on the bus
    for (devaddr = 0; devaddr < MI125_I2SCAN_MAX_DEV && SUCCESS(ret); devaddr++)
    {
        // Do the adc check test (to verify adc presence)
        ret = MI125_com_check_raw(&board->com, devaddr, &scanraw->devdetect[devaddr]);

        // Check if a device is detected at this address
        if (scanraw->devdetect[devaddr])
        {
            PDEBUG("Device 0x%02x : detected.\n",devaddr);

            // Max devices found counter
            (*ndevices)++;

            // Check if we have found the bridge
            if (devaddr == board->com.bridge_addr)
                scan_res->spibridge = 1;
            else
            {
                // Check if we have found the temp sensor
                if (devaddr == board->com.sensor_addr)
                    scan_res->sensor = 1;
                else
                {
                    // Check if we have found the eeprom
                    if (devaddr == board->com.eeprom_addr)
                        scan_res->eeprom = 1;
                    else
                    {
                        // Check if we have found the beta io expander
                        if (devaddr == board->com.expander2_addr)
                            scan_res->expander = 1;
                        else
                        {
                            // Check if we have found the alpha io expander
                            if (devaddr == board->com.expander_addr)
                                scan_res->expander = 1;
                        }
                    }
                }
            }
        }
        else
        {
            PDEBUG("Device 0x%02x : not detected.\n",devaddr);
        }
    }

    // Check if we have detected all of our devices and nothing else
    if (scan_res->expander == 1 && scan_res->eeprom == 1 && scan_res->sensor == 1 &&
        scan_res->spibridge == 1 && ((*ndevices) <= scanmax || scanmax == 0) && SUCCESS(ret))
        *detect_ok = 1;

    FUNC_EXIT();

    return(ret);
}


Result MI125_mi125_test_localoscsw(MI125_mi125 * board, unsigned char *result_ok)
{
    MI125_mi125_clock_source clksrc = MI125_CLKSRC125MHZ;
    Result ret = MI125_MI125OK;

    FUNC_ENTER();

    // Default for test failure
    *result_ok = 0;

    // Check if the board has been reset correctly at least once
    if (!board->resetdone)
    {
        FUNC_EXIT();
        return(MI125_MI125NORESET);
    }

    // Attempt to disable the local oscillator
    if (board->beta)
    {
        MI125_pca9535_localoscsw sw;

        // Change the clock source to onboard local clock
        ret = _MI125_mi125_isetclksrc(board, clksrc);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

        // Reset the clock mcm and check if the clock is available
        ret = MI125_mi125_clockreset(board);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

        sw = MI125_PCA9535_LOSWOFF;
        ret = MI125_pca9535_setlocaloscsw(&board->expander2, sw);
        if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

        // Reset the clock mcm and check if the clock is available
        ret = MI125_mi125_clockreset(board);
        if(FAILURE(ret))
        {
            // Clock osc sw is working correctly and can disable the clock
            *result_ok = 1;
        }
     }
     else
     {
            // Fake the clock osc sw is working correctly and can disable the clock
            // for alpha board because there are no sw in this board type
            *result_ok = 1;
     }

    // For top board, the clk source could not be different than MI125_CLKSRCBOTTOMFMC
    // if the top board is not the clock master.
    if ((board->fmcposition != MI125_BOTTOM) &&
        board->core->Register_mi125status.Bit.MI125ClkMaster == 0)
    {
        clksrc = MI125_CLKSRCBOTTOMFMC;
    }

    // Change the clock source
    ret = _MI125_mi125_isetclksrc(board, clksrc);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Reset the clock mcm and check if the clock is available
    ret = MI125_mi125_clockreset(board);
    if(FAILURE(ret)) {FUNC_EXIT(); return ret;}

    // Digital output calibration must be done again because of clock change
    ret = MI125_mi125_digital_adccalibration(board);

    FUNC_EXIT();

    return(ret);
}


