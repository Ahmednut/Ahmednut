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
 * @file       Cdce62005.c
 * @brief      Enhanced cdce62005 module related functions
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
#include <stdio.h>
#include <stdlib.h>

#include "Cdce62005.h"

#include "carrier_service.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

// Pll synthetized frequency range
#define CDCE62005_SYNTHFREQMAX (1178000000)
#define CDCE62005_SYNTHFREQMIN (350000000)

// Pll synthetized frequency gap (the pll cannot synthetized between these frequencies)
#define CDCE62005_SYNTHFREQGAPMAX (875000000)
#define CDCE62005_SYNTHFREQGAPMIN (785333333)

// Pll vco frequency range
#define CDCE62005_VCOFREQMAX ((unsigned)(2356e06))
#define CDCE62005_VCOFREQMIN (1750000000)

// Auxiliary crystal input frequency range
#define CDCE62005_XTALFREQMAX (42000000)
#define CDCE62005_XTALFREQMIN (2000000)

// Reference input (primary/secondary) frequency range
#define CDCE62005_LVCMOSINPUTMAXFREQ (250000000)
#define CDCE62005_LVPECLINPUTMAXFREQ (1500000000)
#define CDCE62005_LVDSINPUTMAXFREQ (800000000)
// Minimum reference input frequency. While the device allows down to 40khz, the sync function (used here)
// and phase adjust cannot work correctly below 1MHz 
#define CDCE62005_INPUTMINFREQ 1000000

// Maximum pll input reference frequency (smartmux input)
#define CDCE62005_SMARTMUXFREQMAX (500000000)

// High vco frequency transition
#define CDCE62005_HIGHVCOFREQ (2040000000)

// Max possible output divider
#define CDCE62005_VCOMAXOUTPUTDIVIDER 80

// Pll calibration time is worst case =550 X tpfd (uPfdFreqMin is CDCE62005_PFDMINEXT) = 13.75msec
#define CDCE62005_PLLCALIB_DELAY (20000)            ///< Delay for pll calibration

// Pll sync time is related to ref frequency period added with possible 6usec
#define CDCE62005_PLLSYNC_DELAY (1000)             ///< Delay for pll synchronisation

// Number of pll verification loop (each loop gives around 6msec in release mode)
#define CDCE62005_PLLLOCK_WAITLOOP (500)

// Number of consecutive pll lock verification that the pll must be locked (each loop gives around 6msec in release mode)
#define CDCE62005_PLLLOCK_VALID (250)

#define CDCE62005_PLLLOCK_LOOP_DELAY (1000)        ///< Minimum delay to wait for each lock loop


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/

typedef enum Cdce62005_EnumOutBufferModeReg {
    eCdce62005OutModeRegLvCmos=0x00,                            ///< Output in lvds mode
    eCdce62005OutModeRegDisabled=0x1a,                          ///< Output disabled
    eCdce62005OutModeRegLvPecl=0x20,                            ///< Output in lvpecl mode
    eCdce62005OutModeRegLvds=0x3a,                              ///< Output in lvds mode
} Cdce62005_eOutBufferModeReg;

// Possible prescaler values and binary register value
const unsigned char Cdce62005_aucPrescalerValues[][2] = {{2,0x3}, {3,0x1}, {4,0x2}, {5,0x0}};

// Binary possible register values according to needed output mux select
const unsigned char Cdce62005_aucOutMuxReg[4] = {0 , 2 , 1, 3};

// Binary possible register values according to needed input pre-divider
const unsigned char Cdce62005_aucInputPredividerReg[4] = {0 , 2 , 1, 3};

// Possible reference divider combined with input divider
// { combined reference divider, input divider, reference divider }
const unsigned char Cdce62005_aucPossibleRefDiv[][3] = {{1,1,1}, {2,1,2}, {3,1,3}, {4,1,4}, {5,1,5}, {6,1,6}, {7,1,7}, {8,1,8}, {10,2,5}, {12,2,6}, {14,2,7}, {16,2,8}};

// Possible reference divider combined with input divider (when pri/sec input pre divider is forced to div by 2)
// { combined reference divider, input divider, reference divider }
const unsigned char Cdce62005_aucPossibleRefDivForceDiv2[][3] = {{2,2,1}, {4,2,2}, {6,2,3}, {8,2,4}, {10,2,5}, {12,2,6}, {14,2,7}, {16,2,8}};


const unsigned char Cdce62005_aucPossibleOutDiv[][2] = {{1,0x20}, {2,0x40}, {3,0x41}, {4,0x42}, {5,0x43}, {6,0x01}, {8,0x02}, {10,0x03}, {12,0x05}, {16,0x06}, {18,0x09}, {20,0x07}, {24,0x0a},  {30,0x0b}, {32,0x0e}, {36,0x15}, {40,0x0f}, {42,0x19}, {48,0x16}, {50,0x13}, {56,0x1a}, {60,0x17}, {64,0x1e}, {70,0x1b}, {80,0x1f}};

//feedback divider setting lookup - from datasheet
// { divider value, value to be written in register }
const unsigned short Cdce62005_ausPossibleFeedbackDiv0[][2] = {{8,0x0}, {12,0x1}, {16,0x2}, {20,0x3}, {24,0x5}, {32,0x6}, {36,0x9}, {40,0x7}, {48,0xa}, {56,0x18}, {60,0xb}, {64,0xe}, {72,0x15}, {80,0xf}, {84,0x19}, {96,0x16}, {100,0x13}, {108,0x49}, {112,0x1a}, {120,0x17}, {128,0x1e}, {140,0x1b}, {144,0x35}, {160,0x1f}, {168,0x39}, {180,0x4b}, {192,0x36}, {200,0x33}, {216,0x55}, {224,0x3a}, {240,0x37}, {252,0x59}, {256,0x3e}, {280,0x3b}, {288,0x56}, {300,0x53}, {320,0x3f}, {336,0x59}, {360,0x57}, {384,0x5e}, {392,0xd8}, {400,0x73}, {420,0x5b}, {432,0xb5}, {448,0x79}, {480,0x5f}, {500,0x93}, {504,0xb9}, {512,0x7e}, {560,0x7b}, {576,0xb6}, {588,0xd9}, {600,0x97}, {640,0x7f}, {672,0xba}, {700,0x9b}, {720,0xb7}, {768,0xbe}, {784,0xda}, {800,0x9f}, {840,0xbb}, {896,0xde}, {960,0xbf}, {980,0xdb}, {1024,0xfe}, {1120,0xdf}, {1280,0xff}};

//feedback bypass divider lookup - from datasheet
// { divider value, value to be written in register }
const unsigned char Cdce62005_aucPossibleFeedbackDiv1[][2] = {{1, 0x7}, {2, 0x0}, {5, 0x1}, {8, 0x2}, {10, 0x3}, {16, 0x4}, {20, 0x5}};

// combined uniques feedback divider (Cdce62005_ausPossibleFeedbackDiv0 * Cdce62005_aucPossibleFeedbackDiv1)
// with index of Cdce62005_ausPossibleFeedbackDiv0 and Cdce62005_ausPossibleFeedbackDiv1 elements
const unsigned short Cdce62005_ausPossibleFeedbackDiv[][3] = {{8,0,0},{12,1,0},{16,0,1},{20,3,0},{24,1,1},{32,5,0},{36,6,0},{40,3,1},{48,8,0},{56,9,0},{60,1,2},{64,11,0},{72,12,0},{80,2,2},{84,14,0},{96,1,3},{100,16,0},{108,17,0},{112,18,0},{120,4,2},{128,0,5},{140,21,0},{144,22,0},{160,23,0},{168,14,1},{180,25,0},{192,26,0},{200,27,0},{216,28,0},{224,29,0},{240,8,2},{252,31,0},{256,5,3},{280,21,1},{288,22,1},{300,10,2},{320,3,5},{336,37,0},{360,25,1},{384,26,1},{392,40,0},{400,13,2},{420,14,2},{432,28,1},{448,29,1},{480,45,0},{500,46,0},{504,47,0},{512,32,1},{540,17,2},{560,49,0},{576,50,0},{588,51,0},{600,10,4},{640,20,2},{672,14,3},{700,55,0},{720,38,1},{768,15,3},{784,40,1},{800,16,3},{840,42,1},{864,17,3},{896,61,0},{900,25,2},{960,8,6},{980,63,0},{1000,27,2},{1008,47,1},{1024,11,5},{1080,17,4},{1120,49,1},{1152,12,5},{1176,51,1},
                                                           {1200,19,4},{1260,31,2},{1280,11,6},{1344,14,5},{1400,55,1},{1440,34,2},{1500,35,2},{1536,15,5},{1568,58,1},{1600,27,3},{1680,14,6},{1728,17,5},{1792,29,3},{1800,25,4},{1920,19,5},{1960,63,1},{2000,41,2},{2016,31,3},{2048,20,5},{2100,42,2},{2160,28,4},{2240,21,5},{2304,34,3},{2400,45,2},{2500,46,2},{2520,31,4},{2560,48,2},{2688,24,5},{2800,49,2},{2880,38,3},{2940,51,2},{3000,35,4},{3072,26,5},{3136,40,3},{3200,23,6},{3360,42,3},{3456,43,3},{3500,55,2},{3584,44,3},{3600,56,2},{3840,26,6},{3920,58,2},{4000,59,2},{4032,31,5},{4096,32,5},{4200,60,2},{4320,28,6},{4480,29,6},{4608,50,3},{4704,51,3},{4800,52,3},{4900,63,2},{5000,46,4},{5040,31,6},{5120,32,6},{5376,37,5},{5600,49,4},{5760,38,5},{5880,51,4},{6000,35,6},{6144,57,3},{6272,58,3},{6400,59,3},{6720,42,5},{6912,43,5},{7000,55,4},
                                                           {7168,44,5},{7200,56,4},{7680,45,5},{7840,40,6},{8000,46,5},{8064,47,5},{8192,64,3},{8400,42,6},{8640,43,6},{8960,61,4},{9216,50,5},{9408,51,5},{9600,45,6},{9800,63,4},{10000,46,6},{10080,47,6},{10240,66,3},{10752,54,5},{11200,65,4},{11520,50,6},{11760,51,6},{12000,52,6},{12288,57,5},{12544,58,5},{12800,59,5},{13440,54,6},{14000,55,6},{14336,61,5},{14400,56,6},{15360,62,5},{15680,63,5},{16000,59,6},{16384,64,5},{16800,60,6},{17920,61,6},{19200,62,6},{19600,63,6},{20480,64,6},{22400,65,6},{25600,66,6}};


#define CDCE62005_DEFAULT_NOTHING {{{ \
    0x06840010, \
    0x06840010, \
    0x06840010, \
    0x06840010, \
    0x06840011, \
    0x078001A4, \
    0x0040F7FE, \
    0x0959BBDE, \
    0x0200092F, \
}}}


#define CDCE62005_DEFAULT_INIT {{{ \
    0x06840010, \
    0x06840010, \
    0x06840010, \
    0x06840010, \
    0x06840011, \
    0x078001A4, \
    0x0040F7FE, \
    0x0959BBDE, \
    0x020009AF, \
}}}

// Includes loop filter configurations
#include "Cdce62005Loop.c"


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/

/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/
static int _Cdce62005_Cmp(const void *ElemA, const void *ElemB);
static int _Cdce62005_CheckOutDividerExist(unsigned char ucDiv);
static unsigned _Cdce62005_DoGcd ( unsigned uA, unsigned uB );
static unsigned _Cdce62005_DoLcm(unsigned uA, unsigned uB);
static Result _Cdce62005_CalculateAuxFreq(Cdce62005_stParam * stParam);
static Result _Cdce62005_CalculatePhaseIndex(Cdce62005_stParam * stParam);
static Result _Cdce62005_CalculateSynthFreq(unsigned *puFreq, unsigned uFreqSize, unsigned * puSynthFreq, unsigned * puSynthFreqSize);
static Result _Cdce62005_CalculateFactors(Cdce62005_stParam * stParam, unsigned uSynth, unsigned int *puIndex, unsigned char *pucInputPreDiv);
static Result _Cdce62005_CalculatePrescaler(Cdce62005_stParam * stParam, unsigned uSynth);
static int _Cdce62005_CheckPresentInArray(unsigned uValue, unsigned * puArr, int iArraySize);
static Result _Cdce62005_DoDivider2Reg(unsigned char ucDiv, unsigned char * pucRegs);
static Result _Cdce62005_DoPrescaler2Reg(unsigned char ucPrescaler, unsigned char * pucRegs);


 /****************************************************************************
 *   NAME : _Cdce62005_Cmp
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
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static int _Cdce62005_Cmp(const void *ElemA, const void *ElemB)
{
    return ( (*(unsigned int*)ElemA) - (*(unsigned int*)ElemB));
}

 /****************************************************************************
 *   NAME : _Cdce62005_CheckOutDividerExist
 ************************************************************************//**
 *
 * Check if the specified output divider is possible in the device.
 *
 * @param [in] ucDiv
 *    divider to be checked if possible
 *
 * @return   !=0 if divider is possible.
 *           ==0 if divider is impossible. 
 * 
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static int _Cdce62005_CheckOutDividerExist(unsigned char ucDiv)
{
    int iI;
    for (iI=0; iI < sizeof(Cdce62005_aucPossibleOutDiv) /2; iI++)
    {
        if (Cdce62005_aucPossibleOutDiv[iI][0] == ucDiv)
        {
            return 1;
        }
    }
    return 0;
}

 /****************************************************************************
 *   NAME : _Cdce62005_DoGcd
 ************************************************************************//**
 *
 * Compute the Greatest Common Divider of 2 integers,
 * according to http://en.wikipedia.org/wiki/Greatest_common_divisor
 *
 * @param [in] uA
 *    first number to compute GCD
 *
 * @param [in] uB
 *    second number to compute GCD
 *
 * @return   computed GCD.
 * 
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static unsigned _Cdce62005_DoGcd ( unsigned uA, unsigned uB )
{
  unsigned uC;
  
  while ( uA != 0 )
  {
     uC = uA;
     uA = uB % uA;
     uB = uC;
  }
  return uB;
}

 /****************************************************************************
 *   NAME : _Cdce62005_DoLcm
 ************************************************************************//**
 *
 * Compute the Least Common Multiple of 2 integers,
 * according to http://en.wikipedia.org/wiki/Least_common_multiple
 *
 * @param [in] uA
 *    first number to compute LCM
 *
 * @param [in] uB
 *    second number to compute LCM
 *
 * @return   computed LCM.
 * 
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static unsigned _Cdce62005_DoLcm(unsigned uA, unsigned uB)
{
    return (uB / _Cdce62005_DoGcd(uA, uB) ) * uA;
}

 /****************************************************************************
 *   NAME : _Cdce62005_CalculateAuxFreq
 ************************************************************************//**
 *
 * Compute auxiliary output frequency.
 *
 * @warning
 * - aucOutDivider member must be set before calling this function 
 *  
 * @param [inout] stParam
 *    in: aucOutDivider, AuxParams members are used for aux freq calculation
 *    out: uAuxFreq member will return the computed auxiliary output frequency 
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static Result _Cdce62005_CalculateAuxFreq(Cdce62005_stParam * stParam)
{
    Result Ret = eCdce62005Ok;
    unsigned char ucOutSelect, ucOutSelectIndx;
    
    // Check if auxiliary output is disabled?
    if (stParam->AuxParams.AuxState == eCdce62005AuxDisable)
    {
        stParam->uAuxFreq = 0;
    }
    else
    {
        // Check if auxiliary output is enabled?
        if (stParam->AuxParams.AuxState == eCdce62005AuxEnable)
        {
            // Check auxiliary correct selection
            if (stParam->AuxParams.AuxSelect == eCdce62005AuxSelect2)
            {
                ucOutSelectIndx = 2;
            }
            else
            {
                if (stParam->AuxParams.AuxSelect == eCdce62005AuxSelect3)
                {
                    ucOutSelectIndx = 3;
                }
                else
                {
                    // Invalid aux output selection
                    Ret = eCdce62005BadAuxOutSelect;
                    return(Ret);
                }
            }
            ucOutSelect = stParam->aucOutDivider[ucOutSelectIndx];
            if (ucOutSelect == 1) ucOutSelect = 0;
            if (ucOutSelect < 6) ucOutSelect*=2;
            
            if (ucOutSelect != 0)
                stParam->uAuxFreq = stParam->OutParams[ucOutSelectIndx].uOutputFreq * (unsigned) stParam->aucOutDivider[ucOutSelectIndx] / ucOutSelect;
            else
                stParam->uAuxFreq = 0;
        }
        else
        {
            // Error invalid state
            Ret = eCdce62005BadAuxOutState;
        }
    }
    
    return(Ret);
}


 /****************************************************************************
 *   NAME : _Cdce62005_CalculatePhaseIndex
 ************************************************************************//**
 *
 * Compute outputs phase index according to specified phase requierements.
 *
 * @warning
 * - aucOutDivider member must be set before calling this function.
 * - usPhaseAdj member (Cdce62005_stOutputParams) specified the requiered
 *   phase in degree. This routine will attempt to match the desired phase
 *   best as possible for the choosen output dividers by rounding the phase
 *   to the nearest possible value.
 *  
 * @param [inout] stParam
 *    in: aucOutDivider, usPhaseAdj members are used for phase index calculation
 *    out: aucOutPhaseAdjNdx member will return the computed outputs pahse adjust
 *         index (realphase = aucOutPhaseAdjNdx[x]/aucOutDivider[x]*360 deg)
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static Result _Cdce62005_CalculatePhaseIndex(Cdce62005_stParam * stParam)
{
    Result Ret = eCdce62005Ok;
    unsigned uI, uTemp1, uTemp2, uTemp3;
    
    // For every possible outputs
    for(uI=0; uI<CDCE62005_NPLL; uI++)
    {
        // Is this output disabled or could not be phase adjust
        if (stParam->aucOutDivider[uI] <= 1)
        {
            // Output cannot be phase adjust, set the phase index to 0 (0 deg)
            stParam->aucOutPhaseAdjNdx[uI] = 0;
        }
        else
        {
            // Computation based on realphase = aucOutPhaseAdjNdx[uI]/aucOutDivider[uI]*360 (units degree)
            uTemp1 = (unsigned) (stParam->OutParams[uI].usPhaseAdj) * stParam->aucOutDivider[uI];
            
            // Integral divider part
            uTemp2 = uTemp1 / 360;
            
            // Compute divider rest
            uTemp3 = uTemp1 - uTemp2 * 360;
            
            // Do we need rounding the integral result?
            if (uTemp3 >= 180)
            {
                // Yes add 1
                uTemp2++;
            }
            // Save the computed phase index
            stParam->aucOutPhaseAdjNdx[uI] = uTemp2 % stParam->aucOutDivider[uI];
        }
    }

    return(Ret);
}


 /****************************************************************************
 *   NAME : _Cdce62005_CalculateSynthFreq
 ************************************************************************//**
 *
 * Compute a possible synthetized frequencies vector according to specified
 * frequencies vector.
 *
 * @param [in] puFreq
 *    vector of frequencies to compute a possible synthetized frequencies vector
 *
 * @param [in] uFreqSize
 *    size of puFreq vector in number of frequencies
 *
 * @param [out] puSynthFreq
 *    vector of possible synthetized frequencies
 *
 * @param [out] puSynthFreqSize
 *    size of vector of possible synthetized frequencies in number of frequencies
 *
 * @return   computed LCM.
 * 
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static Result _Cdce62005_CalculateSynthFreq(unsigned *puFreq, unsigned uFreqSize, unsigned * puSynthFreq, unsigned * puSynthFreqSize)
{
    unsigned uSynth, uMultiple = 0, uI, uK, uCount;
    unsigned uDiv1;
	Result Ret;

    // For every input frequencies specified
    for (uI = 0; uI < uFreqSize; uI++)
    {
        if ( uI == 0)
            uMultiple = puFreq[0];
        else
        {
            // Compute cumulative LCM of current provided frequency 
            uMultiple = _Cdce62005_DoLcm(puFreq[uI], uMultiple);
            
            // Check if current multiple is an exact multiple of the specified frequency
            // (check for overflow, could happen for large coprime numbers)
            if(uMultiple % puFreq[uI])
            {
                return eCdce62005Overflow01;
            }
        }
    }
    if (uFreqSize > 1)
    {
        // Check if multiple is an exact multiple of the specified 1st frequency
        // (check for overflow, could happen for large coprime numbers)
        if(uMultiple % puFreq[0])
        {
            return eCdce62005Overflow02;
        }
    }
    uSynth = uMultiple;

    uI = 2;
    uK = 0;
    
    // While still inside device synthetized frequency range
    while(uSynth <= CDCE62005_SYNTHFREQMAX)
    {
        // Reach minimum frequency device can synthetized so far?
        if(uSynth >= CDCE62005_SYNTHFREQMIN)
        {
            // Makes sure we are not inside the frequency gap were we cannot synthetized
            if (!(uSynth > CDCE62005_SYNTHFREQGAPMIN && uSynth < CDCE62005_SYNTHFREQGAPMAX))
            {
                // For every input frequencies specified
                for (uCount = 0; uCount < uFreqSize; uCount++)
                {
                    // Compute the requiered divider for this specified frequency
                    uDiv1 = uSynth / puFreq[uCount];
                    
                    // Check if the output divider exist
                    if (uDiv1 * puFreq[uCount] != uSynth || !_Cdce62005_CheckOutDividerExist(uDiv1))
                        // does not exist
                        break;
                }
                // Is all output divider exist for all specified frequencies 
                if (uCount >= uFreqSize)
                {
                    // Yes, then save this synthetized frequency as a valid one in the return vector
                    puSynthFreq[uK] = uSynth;
                    uK++;
                }
                
                // Still room in the output vector?
                if(uK >= *puSynthFreqSize)
                    break;
            }
        }
        // Try for a multiple of the current cumulative lcm (which would also be a possible valid multiple for outputs)
        uSynth = uMultiple * uI;
        uI++;
    }
    
    // Return found number of possible synthetized frequencies
    *puSynthFreqSize = uK;
        
    // Did we find some synth freq?
	if (uK > 0)
		Ret = eCdce62005Ok;
	else
		Ret = eCdce62005Divider01;
	return(Ret);
}

 /****************************************************************************
 *   NAME : _Cdce62005_CalculateFactors
 ************************************************************************//**
 *
 * Calculates factors for the synthetiser frequency. It will calculate
 * input divider, reference divider, pfd frequency and feedback divider.
 * By default, it will find the 1st possible pfd highest frequency (feedback divider)
 * from provided starting index in combined feedback divider buffer
 * (index of 0 will give the highest pfd frequency solution). This behavior
 * can also be changed by limiting the range of authorized pfd frequencies with the
 * parameters uPfdFreqMin, uPfdFreqMax.   
 *
 * @param [in] uSynth
 *    synthetized frequency to use
 *
 * @param [inout] puIndex
 *    starting index in feedback divider (use 0 to find the 1st highest pfd
 *    frequency solution possible). Once a solution is find (this function returns without an error), the next
 *    index value can be used (*puIndex+1) to find the 2nd highest pfd solution with successive 
 *    call to this function, etc. 
 *
 * @param [inout] stParam
 *    pll parameters structure (in: ref_freq, uPfdFreqMin, uPfdFreqMax)
 *                             (out: usInputDivider, uPfdFreq, usFeedbackDivider, usReferenceDivider,
 *                                   usCFeedbackDividerIndex, usCFeedbackDivider, ucBypassDivider) 
 *
 * @param [inout] pucInputPreDiv
 *    current input prediv value. At output, it will be adjusted to the calculated prediv value
 *    This value when 1 will attempt to calculate the prediv value if possible, when = 2
 *    it will try to force it to a value of 2. This is only used for prim/sec smartmux ref used.
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static Result _Cdce62005_CalculateFactors(Cdce62005_stParam * stParam, unsigned uSynth, unsigned int *puIndex, unsigned char *pucInputPreDiv)
{
    unsigned uTotalDivider, uPfdFreq, uInputDivider, uReferenceDivider, uTemp1, uTemp2, uSizeChar = sizeof(unsigned char);
    unsigned uCount, uSize = 0, ucInputCombinedDiv, uRest, uTemp3;
    unsigned char *paucPossibleRefDiv = 0, ucInputPreDiv, ucPInputPreDiv;
    unsigned long long ulCalc;

    // Check if the starting feedback index is valid
    if (*puIndex >= (sizeof(Cdce62005_ausPossibleFeedbackDiv) / sizeof(Cdce62005_ausPossibleFeedbackDiv[0])))
        return(eCdce62005BadFeedbackIndex);
        
    if (pucInputPreDiv != 0)
        if (*pucInputPreDiv < 1 || *pucInputPreDiv > 2)
            return(eCdce62005BadInputPreDivider);
    
    // If the reference divider is usabled (it is not for auxiliary smartmux ref)?
    if (stParam->ucUsableRefDivider)
    {
        ucInputPreDiv = *pucInputPreDiv;
        
        // Forces by user specification to use input prediv by 2?
        if (*pucInputPreDiv == 2)
        {
            // Use the ref divider/input prediv limited to prediv =2
            paucPossibleRefDiv = (unsigned char *)&Cdce62005_aucPossibleRefDivForceDiv2[0][0];
            uSize = sizeof(Cdce62005_aucPossibleRefDivForceDiv2)/sizeof(Cdce62005_aucPossibleRefDivForceDiv2[0]);
        }
        else
        {
            // Use all possible ref divider/input prediv
            paucPossibleRefDiv = (unsigned char *)&Cdce62005_aucPossibleRefDiv[0][0];
            uSize = sizeof(Cdce62005_aucPossibleRefDiv)/sizeof(Cdce62005_aucPossibleRefDiv[0]);
        }
    }
    else
        // For auxiliary ref, there is no prediv
        ucInputPreDiv = 1;

    // Backup initial prediv
    ucPInputPreDiv = ucInputPreDiv;
    
    // For each possible combined feedback divider
    // This loop will find the highest pfd frequency possible (recommended from design guidelines)
    // by scanning the feedback divider from smallest to highest value, and keep it
    // as a possible solution
    for (; *puIndex < sizeof(Cdce62005_ausPossibleFeedbackDiv) / sizeof(Cdce62005_ausPossibleFeedbackDiv[0]); (*puIndex)++)
    {
        // Compute real feedback divider according to current parts
        uTotalDivider = (unsigned)Cdce62005_ausPossibleFeedbackDiv[*puIndex][0];
        
        // Compute comparison pfd frequency
        uPfdFreq = uSynth / uTotalDivider;
        
        // Was it an exact divide
        if ((uPfdFreq * uTotalDivider) == uSynth)
            // Yes
            uRest = 0;
        else
            uRest = 1;
            
        // Check if pfd frequency is inside specified pfd frequency range
        // Max check take also care of possible rounding errors
        if( (uPfdFreq >= stParam->uPfdFreqMin) && ((uPfdFreq + uRest) <= stParam->uPfdFreqMax))
        {
            // Calculate input divider requiered to meet pfd freq from input reference frequency
            //uInputDivider = stParam->uRefFreq / ucPInputPreDiv / uPfdFreq;
            ulCalc = ((unsigned long long) (stParam->uRefFreq)) *  uTotalDivider;
            uTemp3 = uSynth * ucPInputPreDiv;
            uInputDivider = (unsigned) (ulCalc / uTemp3);
            
            // Did we get an exact input divider?
            if (((unsigned long long) (uInputDivider)) * uTemp3 == ulCalc)
            {
                // Check if reference divider is usable and could be used to achieve a possible solution
                if (stParam->ucUsableRefDivider)
                {
                    // Ok input divider is outside allowed range, check if we can get it by using the reference/input prediv divider
                    if (uInputDivider > 256)
                    {
                        // Check all possible reference divider / prediv divider value if we can find an exact multiple
                        for (uCount = 0; uCount < uSize; uCount++)
                        {
                            uReferenceDivider = paucPossibleRefDiv[uCount * uSizeChar * 3 + 2];
                            ucInputPreDiv = paucPossibleRefDiv[uCount * uSizeChar * 3 + 1];
                            ucInputCombinedDiv = paucPossibleRefDiv[uCount * uSizeChar * 3 + 0];  
                            
                            // Calculates exact integral part
                            uTemp3 = uInputDivider * ucPInputPreDiv;
                            uTemp2 = uTemp3 / ucInputCombinedDiv;
                            // Calculates if any rest in the divide
                            uTemp1 = uTemp3 - uTemp2 * ucInputCombinedDiv;
                            // Is this an exact divider?
                            if (!uTemp1)
                            {
                                // Yes it is an exact match, so look if we are now inside input divider range
                                if (uTemp2 <= 256)
                                {
                                    // Yes inside of input divider, keep it as a possible solution
                                    uInputDivider = uTemp2;
                                    break;
                                }
                            }
                        }
                        // Did we find a possible solution?
                        if (uCount >= uSize)
                        {
                            // No use 1 as default
                            uReferenceDivider = 1;
                            ucInputPreDiv = ucPInputPreDiv;
                        }
                    }
                    else
                    {
                        // No use 1 as default
                        uReferenceDivider = 1;
                        ucInputPreDiv = ucPInputPreDiv;
                    }
                }
                else
                {
                    // No use 1 as default
                    uReferenceDivider = 1;
                }
                
                // Check if input divider is possible
                if ( uInputDivider >= 1 && uInputDivider <= 256)
                {
                    // Check if pfd * input divider * reference divider * prediv gives an exact multiple of pfd frequency
                    if ((((unsigned long long) uInputDivider) * uSynth * uReferenceDivider * ucInputPreDiv) == (((unsigned long long)(stParam->uRefFreq)) * uTotalDivider))
                    {
                        // We can find a valid setup of input divider, reference  div, feedback divider and pfd frequency for
                        // the specified synth frequency and input ref frequency
                        stParam->usInputDivider = uInputDivider;
                        stParam->uPfdFreq = uPfdFreq;
                        stParam->usFeedbackDivider = Cdce62005_ausPossibleFeedbackDiv0[Cdce62005_ausPossibleFeedbackDiv[*puIndex][1]][0];
                        stParam->ucBypassDivider = Cdce62005_aucPossibleFeedbackDiv1[Cdce62005_ausPossibleFeedbackDiv[*puIndex][2]][0];
                        stParam->usCFeedbackDivider = uTotalDivider;
                        stParam->usCFeedbackDividerIndex = *puIndex;
                        stParam->usReferenceDivider = uReferenceDivider;
                        if (stParam->ucUsableRefDivider && pucInputPreDiv != 0)
                        {
                            *pucInputPreDiv = ucInputPreDiv;
                        }
                        return eCdce62005Ok;
                    }
                }
            }
        }
    }
    return eCdce62005PfdImpossible;
}

 /****************************************************************************
 *   NAME : _Cdce62005_CalculatePrescaler
 ************************************************************************//**
 *
 * Calculates prescaler value for the desired synthetiser frequency.
 *
 * @param [in] uSynth
 *    synthetized frequency to use
 *
 * @param [out] stParam
 *    pll parameters structure (out: prescale_divider, vco_freq) 
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static Result _Cdce62005_CalculatePrescaler(Cdce62005_stParam * stParam, unsigned uSynth)
{
    unsigned uVco;
    int iI;
    
    // For every possible prescaler
    for (iI = 0; iI < sizeof(Cdce62005_aucPrescalerValues)/sizeof(Cdce62005_aucPrescalerValues[0]); iI++)
    {
        // Compute expected vco freq for this prescaler
        uVco = uSynth * Cdce62005_aucPrescalerValues[iI][0];
        
        // If vco frequency inside allowed range?
        if( (uVco <= CDCE62005_VCOFREQMAX) && (uVco >= CDCE62005_VCOFREQMIN))
        {
            // Valid synthetiser/vco frequency for choosen prescaler
            stParam->ucPrescaleDivider = Cdce62005_aucPrescalerValues[iI][0];
            stParam->uVcoFreq = uVco;
            return eCdce62005Ok;
        }
    }   
    return eCdce62005NoPrescaler01;
}

 /****************************************************************************
 *   NAME : _Cdce62005_CheckPresentInArray
 ************************************************************************//**
 *
 * Check if a specified value exists in a provided array.
 *
 * @param [in] uValue
 *    value to check in the provided vector
 *
 * @param [in] puArr
 *    vector to search for the uValue 
 *
 * @param [in] iArraySize
 *    search vector size in elements 
 *
 * @return   !=0 if we find the value
 *           ==0 if we did not find it
 *   
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static int _Cdce62005_CheckPresentInArray(unsigned uValue, unsigned * puArr, int iArraySize)
{
    int iCount;
    
    for(iCount=0; iCount<iArraySize; iCount++)
    {
        if(puArr[iCount] == uValue)
        {
            return 1;
        }
    }
    return 0;
}


 /****************************************************************************
 *   NAME : _Cdce62005_DoDivider2Reg
 ************************************************************************//**
 *
 * Calculates and checks for a valid output register value for the specified
 * output divider.
 *
 * @param [in] ucDiv
 *    output divider to search for
 *
 * @param [out] pucRegs
 *    register value to use
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static Result _Cdce62005_DoDivider2Reg(unsigned char ucDiv, unsigned char * pucRegs)
{
    unsigned uI;
    
    // Search for all known output dividers
    for(uI = 0; uI < sizeof(Cdce62005_aucPossibleOutDiv) / 2; uI++)
    {
        // Did we find the specified divider
        if(Cdce62005_aucPossibleOutDiv[uI][0] == ucDiv)
        {
            // Yes returns the needed register value
            *pucRegs = Cdce62005_aucPossibleOutDiv[uI][1];
            return eCdce62005Ok;
        }
    }
    return eCdce62005Divider02;
}

 /****************************************************************************
 *   NAME : _Cdce62005_DoPrescaler2Reg
 ************************************************************************//**
 *
 * Calculates and checks for a valid register value for the specified prescaler.
 *
 * @param [in] ucDiv
 *    prescaler divider to search for
 *
 * @param [out] pucRegs
 *    register value to use
 *
 * @return   Error code: eCdce62005Ok if no problem is found, !eCdce62005Ok if problem
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 *   
 * @ingroup PCDCE62005
 *
 ***************************************************************************/ 
static Result _Cdce62005_DoPrescaler2Reg(unsigned char ucPrescaler, unsigned char * pucRegs)
{
    int iI;

    // Search for any possible prescaler value
    for (iI=0; iI < sizeof(Cdce62005_aucPrescalerValues) / sizeof(Cdce62005_aucPrescalerValues[0]); iI++)
    {
        // If this prescaler is the one we are searching
        if( Cdce62005_aucPrescalerValues[iI][0] == ucPrescaler )
        {
            // Yes, returns the needed register value
            *pucRegs = Cdce62005_aucPrescalerValues[iI][1];
            return eCdce62005Ok;
        }
    }
    return eCdce62005InvalidPrescaler;
}


#ifndef CDCE62005P_CALC_ONLY
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
 * @param [in] TimerWaitUsec
 *    Function to wait for some uSec delay (similar as linux usleep)..
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
Result Cdce62005_Open(Cdce62005_stCore * pCore, Mo1000DevCom_stComCore * pCom, Carrier_handle_t hCarrier)
{
    // Save the com module information
    pCore->m_pCom = pCom;

    // Save the timer function
    pCore->hCarrier = hCarrier;

    return eCdce62005Ok;
}


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
Result Cdce62005_Reset(Cdce62005_stCore * pCore)
{
    Result Ret;
    Cdce62005_eAddr CAddr;
    Cdce62005_stRegisterMap stReg = CDCE62005_DEFAULT_NOTHING; // default do nothing reset reg values

    // Write all registers.
    for (CAddr=eCdce62005Addr00; CAddr<=eCdce62005Addr08; CAddr++)
    {                                                                  
        // This will reset all register bits to default no operating condition
        Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePll, CAddr, &stReg.m_auRegisters[CAddr]);
        
        // Check if we got a problem
        if(FAILURE(Ret)) break;
    }
    
    return Ret;
}


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
Result Cdce62005_Init(Cdce62005_stCore * pCore)
{
    Result Ret;
    Cdce62005_eAddr CAddr;
    Cdce62005_stRegisterMap stReg = CDCE62005_DEFAULT_INIT; // default do nothing no sleep

    // Write all registers.
    for (CAddr=eCdce62005Addr00; CAddr<=eCdce62005Addr08; CAddr++)
    {                                                                  
        // This will reset all register bits to default operating condition
        Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePll, CAddr, &stReg.m_auRegisters[CAddr]);
        
        // Check if we got a problem
        if(FAILURE(Ret)) break;
    }
    
    return Ret;
}


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
Result Cdce62005_WriteReg(Cdce62005_stCore * pCore, Cdce62005_eAddr Addr, unsigned uVal)
{
    Result Ret = eCdce62005InvalidAddr01;

    // Check if address is valid
    if (Addr >= eCdce62005Addr00 && Addr <= eCdce62005Addr08)
    {
        // Write specified register.
        Ret = Mo1000DevCom_Write(pCore->m_pCom, eMo1000DevComDevicePll, Addr, &uVal);
    }
    
    return Ret;
}


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
Result Cdce62005_ReadReg(Cdce62005_stCore * pCore, Cdce62005_eAddr Addr, unsigned *puVal)
{
    Result Ret = eCdce62005InvalidAddr02;

    *puVal = 0;
    
    // Check if address is valid
    if (Addr >= eCdce62005Addr00 && Addr <= eCdce62005Addr08)
    {
        // Read specified register.
        Ret = Mo1000DevCom_Read(pCore->m_pCom, eMo1000DevComDevicePll, Addr, puVal);
    }
    
    return Ret;
}

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
Result Cdce62005_Close(Cdce62005_stCore * pCore)
{
    return eCdce62005Ok;
}

#endif



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
Result Cdce62005_Calculate(Cdce62005_stParam * stParam)
{
    Result Ret;
    unsigned uI, uJ;
    int iCount;
    unsigned auSynthFreq[50], uTryIndex;
    unsigned auUniqueFreq[CDCE62005_NPLL];
    unsigned uUniqueFreqSize,uMaxInputFreq;
    unsigned uSynthFreqSize = sizeof(auSynthFreq) / sizeof(auSynthFreq[0]);
    unsigned uDividerTmp;
    Cdce62005_eOutRefMode OutRefMode;
    unsigned uRefFreqNoSynth,uMaxFreq, uFindOne, uTryEverything;
    unsigned short usIndexSol;
    unsigned char *pucInputPreDiv = 0, ucInputPreDivBk = 1;
    unsigned int *puFoundSynth = 0;
    unsigned uTryIndexStart, uTryIndexStop, uFirstFind, uTryIndexStartBk;
    int iSynthFreqStart, iSynthFreqStop, iSynthFreqStartBk = 0; 
    
    uUniqueFreqSize = 0;
    
    // Primary input termination control validation
    if (stParam->RefInput.PrimaryInput.InputTerm > eCdce62005InputTermDisabled || stParam->RefInput.PrimaryInput.InputTerm < eCdce62005InputTermEnabled)
        return(eCdce62005BadPriTermCtrl);

    // Secondary input termination control validation
    if (stParam->RefInput.SecondaryInput.InputTerm > eCdce62005InputTermDisabled || stParam->RefInput.SecondaryInput.InputTerm < eCdce62005InputTermEnabled)
        return(eCdce62005BadSecTermCtrl);

    // Lvds fail safe mode validation
    if (stParam->RefInput.LvdsFailSafe > eCdce62005LvdsInputFailEnable || stParam->RefInput.LvdsFailSafe < eCdce62005LvdsInputFailDisable)
        return(eCdce62005BadLvdsFailSafeMode);

    // Primary input Vbb mode validation
    if (stParam->RefInput.PrimaryInput.InputVbb > eCdce62005InputVbbInverted || stParam->RefInput.PrimaryInput.InputVbb < eCdce62005InputVbbNormal)
        return(eCdce62005BadPriVbbMode);

    // Secondary input Vbb mode validation
    if (stParam->RefInput.SecondaryInput.InputVbb > eCdce62005InputVbbInverted || stParam->RefInput.SecondaryInput.InputVbb < eCdce62005InputVbbNormal)
        return(eCdce62005BadSecVbbMode);

    // Input termination mode validation
    if (stParam->RefInput.InputsTermination > eCdce62005InputDcTermination || stParam->RefInput.InputsTermination < eCdce62005InputAcTermination)
        return(eCdce62005BadInputTermMode);

    // Output params validation
    for (uI = 0; uI <= CDCE62005_NPLL; uI++)
    {
        // Checks output phase
        if (stParam->OutParams[uI].usPhaseAdj >= 360)
            return(eCdce62005BadOutput0PhaseVal + uI);
            
        // Checks output clock reference
        if (stParam->OutParams[uI].OutputRef > eCdce62005OutRefModeSynth || stParam->OutParams[uI].OutputRef < eCdce62005OutRefModePrimary)
            return(eCdce62005BadOutput0Ref + uI);
            
        // Checks positive lvcmos pin mode
        if (stParam->OutParams[uI].PLvCmosMode > eCdce62005LvCmosOutPinLow || stParam->OutParams[uI].PLvCmosMode < eCdce62005LvCmosOutPinActive)
            return(eCdce62005BadPLvcmosOut0Mode + uI);

        // Checks negative lvcmos pin mode
        if (stParam->OutParams[uI].NLvCmosMode > eCdce62005LvCmosOutPinLow || stParam->OutParams[uI].NLvCmosMode < eCdce62005LvCmosOutPinActive)
            return(eCdce62005BadNLvcmosOut0Mode + uI);
    }

    // Output sync mode validation
    if (stParam->SyncMode > Cdce62005_eSyncNextOutput || stParam->SyncMode < Cdce62005_eSync6UsNextRefAndOutput)
        return(eCdce62005BadSyncMode);

    // Auxiliary output selection validation
    if (stParam->AuxParams.AuxSelect > eCdce62005AuxSelect3 || stParam->AuxParams.AuxSelect < eCdce62005AuxSelect2)
        return(eCdce62005BadAuxOutputSelect);

    // Auxiliary output state validation
    if (stParam->AuxParams.AuxState > eCdce62005AuxEnable || stParam->AuxParams.AuxState < eCdce62005AuxDisable)
        return(eCdce62005BadAuxOutputState);

    // Pll lock pin mode validation
    if (stParam->PllLockParams.PllLockMode > eCdce62005AnalogPllLock || stParam->PllLockParams.PllLockMode < eCdce62005DigitalPllLock)
        return(eCdce62005BadPllLockPinMode);

    // Pll lock count mode validation
    if (stParam->PllLockParams.PllLockCount > eCdce62005PllLockCount64Pfd || stParam->PllLockParams.PllLockCount < eCdce62005PllLockCount1Pfd)
        return(eCdce62005BadPllLockCountMode);

    // Pll lock width validation
    if (stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW1D5Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW3D4Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW5D4Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW5D8Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW7D7Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW9D7Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW15D0Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW15D1Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW17D0Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW19D0Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW19D3Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW28D6Ns)
        return(eCdce62005BadPllLockWidth);

    // Loop filter type validation
    if (stParam->LoopFilterType > eCdce62005LoopFilterExt || stParam->LoopFilterType < eCdce62005LoopFilterInt)
        return(eCdce62005BadLoopFilterType);

    // Default calculated values
    stParam->uPfdFreq = 0;
    stParam->uVcoFreq = 0;
    stParam->uSynthFreq = 0;
    stParam->uAuxFreq = 0;
    stParam->usFeedbackDivider = 8;
    stParam->usCFeedbackDivider = 8;
    stParam->ucBypassDivider = 1;
    stParam->usCFeedbackDividerIndex = 0;
    stParam->usReferenceDivider = 1;
    stParam->ucPrimaryInputDivider = 0;
    stParam->ucSecondaryInputDivider = 0;
    stParam->usInputDivider = 1;
    memset(&stParam->aucOutDivider[0],0,sizeof(stParam->aucOutDivider));
    memset(&stParam->aucOutPhaseAdjNdx[0],0,sizeof(stParam->aucOutPhaseAdjNdx));
    stParam->ucVcoBypassAll = 0;
    memset(&stParam->aucVcoBypass[0],0,sizeof(stParam->aucVcoBypass));
    stParam->uRefFreq = 0;
    stParam->ucUsableRefDivider = 0;
    stParam->R2Value = eCdce62005R2Val0D1KO;
    stParam->R3Value = eCdce62005R3Val5D0KO;
    stParam->C1Value = eCdce62005C1Val0D0PF;
    stParam->C2Value = eCdce62005C2Val0D0PF;
    stParam->C3Value = eCdce62005C3Val0D0PF;
    stParam->ChargePump = eCdce62005ChargePump50D0UA;
    stParam->sLoopFilterIndex = -1;
    stParam->ucPrescaleDivider = Cdce62005_aucPrescalerValues[0][0];
        
    // Pfd frequencies range validation
    if (stParam->uPfdFreqMin < CDCE62005_PFDMINEXT || stParam->uPfdFreqMin > stParam->uPfdFreqMax )
        return(eCdce62005BadPfdMinFrequency);
    if (stParam->uPfdFreqMax > CDCE62005_PFDMAX)
        return(eCdce62005BadPfdMaxFrequency);
        
    // Input type validation
    switch(stParam->RefInput.InputsType)
    {
        case eCdce62005InputLvCmos:
            uMaxInputFreq = CDCE62005_LVCMOSINPUTMAXFREQ;
            break;
            
        case eCdce62005InputLvPecl:
            uMaxInputFreq = CDCE62005_LVPECLINPUTMAXFREQ;
            break;
            
        case eCdce62005InputLvds:
            uMaxInputFreq = CDCE62005_LVDSINPUTMAXFREQ;
            break;
            
        default:
            return eCdce62005BadInputType;
    }
    
    // Input frequency validation according to specified input type
    if (stParam->RefInput.PrimaryInput.uInputFrequency > uMaxInputFreq ||
        (stParam->RefInput.PrimaryInput.uInputFrequency < CDCE62005_INPUTMINFREQ && stParam->RefInput.PrimaryInput.uInputFrequency !=0) ||
        stParam->RefInput.SecondaryInput.uInputFrequency > uMaxInputFreq ||
        (stParam->RefInput.SecondaryInput.uInputFrequency < CDCE62005_INPUTMINFREQ && stParam->RefInput.SecondaryInput.uInputFrequency !=0) ||
        (stParam->RefInput.uAuxInputFrequency < CDCE62005_XTALFREQMIN && stParam->RefInput.uAuxInputFrequency !=0) ||
        stParam->RefInput.uAuxInputFrequency > CDCE62005_XTALFREQMAX)
    {
        return eCdce62005BadInputTypeFreq;
    }

    // Check the reference frequency we want to use for the pll
    switch(stParam->SmartMuxRefSelect)
    {
        case eCdce62005InputRefModePrimary:
            stParam->uRefFreq = stParam->RefInput.PrimaryInput.uInputFrequency;
            stParam->ucUsableRefDivider = 1;
            OutRefMode = eCdce62005OutRefModePrimary;
            break;
             
        case eCdce62005InputRefModeSecondary:
            stParam->uRefFreq = stParam->RefInput.SecondaryInput.uInputFrequency;
            stParam->ucUsableRefDivider = 1;
            OutRefMode = eCdce62005OutRefModeSecondary;
            break;
             
        case eCdce62005InputRefModeAux:
            stParam->uRefFreq = stParam->RefInput.uAuxInputFrequency;
            OutRefMode = eCdce62005OutRefModeSmartmux;
            break;
            
        // Invalid pll reference selected
        default:
            return eCdce62005BadPllRefSelect;
    }
    
    // Check every output frequency is within valid range
    for(uI=0; uI<CDCE62005_NPLL; uI++)
    {
        // What output pin mode was choosen?
        switch(stParam->OutParams[uI].OutputMode)
        {
            case eCdce62005OutModeLvCmos:
                uMaxFreq = CDCE62005_LVCMOSINPUTMAXFREQ;
                break; 

            case eCdce62005OutModeLvPecl:
            case eCdce62005OutModeLvPeclHighSwing:
                uMaxFreq = CDCE62005_LVPECLINPUTMAXFREQ;
                break; 

            case eCdce62005OutModeLvds:
                uMaxFreq = CDCE62005_LVDSINPUTMAXFREQ;
                break; 

            case eCdce62005OutModeDisabled:
                uMaxFreq = 0;
                break;
                
            default: 
                return eCdce62005BadPinOutMode01;
        }
        
        // If specified output frequency is overrange for pin output mode?
        if (stParam->OutParams[uI].uOutputFreq > uMaxFreq)
            return eCdce62005OutputFreqOver;
    }

    // Check if it is possible to bypass VCO
    // For every possible outputs
    for(uI=0; uI<CDCE62005_NPLL; uI++)
    {
		// Is this output a requiered frequency
        if ( stParam->OutParams[uI].uOutputFreq == 0) continue;

        // Is this output attempt to use the pll as a reference?
		if (stParam->OutParams[uI].OutputRef == eCdce62005OutRefModeSynth)
		{
            // Can we bypass the vco and use a direct divider instead?
            // We will attempt to find a solution without tweaking reference divider and input prediv divider,
            // use main/secondary/aux ref to the output block directly (and not try to use smartmux
            // to output block when it is not configured for aux. ref)
            if ( stParam->uRefFreq % stParam->OutParams[uI].uOutputFreq == 0)
            {
                // Yes compute the requiered divider
                uDividerTmp = stParam->uRefFreq / stParam->OutParams[uI].uOutputFreq;
                
                // Check if the divider exist, exact and possible
                if((uDividerTmp * stParam->OutParams[uI].uOutputFreq) == stParam->uRefFreq && _Cdce62005_CheckOutDividerExist(uDividerTmp))
                {
                    // We bypass the vco here and uses a direct divider for this output
                    stParam->aucVcoBypass[uI] = 1;
                    stParam->aucOutDivider[uI] = uDividerTmp;
                    stParam->OutParams[uI].OutputRef = OutRefMode;
                }else{
                    // Check if this requiered output frequency is unique and not already exist
                    if(!_Cdce62005_CheckPresentInArray(stParam->OutParams[uI].uOutputFreq, auUniqueFreq, uUniqueFreqSize))
                    {
                        // No add it to the list for later calculation
                        auUniqueFreq[uUniqueFreqSize++] = stParam->OutParams[uI].uOutputFreq;
                    }
                }
            }else{
                // No, check if this requiered output frequency is unique and not already exist
                if(!_Cdce62005_CheckPresentInArray(stParam->OutParams[uI].uOutputFreq, auUniqueFreq, uUniqueFreqSize))
                {
                    auUniqueFreq[uUniqueFreqSize++] = stParam->OutParams[uI].uOutputFreq;
                }
            }
		}
        else
        {
            // Forced not to use the pll, so try to find a solution in fanout mode (dividing the reference only)
            // According to which output reference we want to use to the output block
            switch (stParam->OutParams[uI].OutputRef)
            {
                case eCdce62005OutRefModePrimary:
                    uRefFreqNoSynth = stParam->RefInput.PrimaryInput.uInputFrequency;
                    
                    // Yes compute the requiered divider
                    uDividerTmp = uRefFreqNoSynth / stParam->OutParams[uI].uOutputFreq;
                    
                    // Check if the divider exist and possible exact divider?
                    if((uDividerTmp * stParam->OutParams[uI].uOutputFreq) == uRefFreqNoSynth && _Cdce62005_CheckOutDividerExist(uDividerTmp))
                    {
                        // We bypass the vco here as desired and uses a direct divider for this output
                        stParam->aucVcoBypass[uI] = 1;
                        stParam->aucOutDivider[uI] = uDividerTmp;
                    }
                    else
                    {
                        // There is no solution here if we don't use the vco
                        return eCdce62005NoFanoutDivider01;
                    }
                    break;
                    
                case eCdce62005OutRefModeSecondary:
                    uRefFreqNoSynth = stParam->RefInput.SecondaryInput.uInputFrequency;
                    
                    // Yes compute the requiered divider
                    uDividerTmp = uRefFreqNoSynth / stParam->OutParams[uI].uOutputFreq;
                    
                    // Check if the divider exist and possible exact divider?
                    if((uDividerTmp * stParam->OutParams[uI].uOutputFreq) == uRefFreqNoSynth && _Cdce62005_CheckOutDividerExist(uDividerTmp))
                    {
                        // We bypass the vco here as desired and uses a direct divider for this output
                        stParam->aucVcoBypass[uI] = 1;
                        stParam->aucOutDivider[uI] = uDividerTmp;
                    }
                    else
                    {
                        // There is no solution here if we don't use the vco
                        return eCdce62005NoFanoutDivider02;
                    }
                    break;
                    
                case eCdce62005OutRefModeSmartmux:
                    // If the reference divider usable (means are smartmux ref from primary or secondary)?
                    if (stParam->ucUsableRefDivider)
                    {
                        // We currently do not support having output block reference set to smartmux
                        // when smartmux input ref is not the auxiliary clock. The reason is that the complication
                        // to correctly support this means we need to deal with the reference and input prediv
                        // which are a single resource for all outputs that could ref to smartmux, and this can
                        // cause problem to have different needs for these dividers for the different outputs and the vco.
                        // We reserved the use of these dividers for the vco use (pll only)
                        return(eCdce62005SmartMuxRefNotSup); 
                    }
                    else
                    {
                        // No compute the requiered divider (we are using the auxiliary input)
                        uDividerTmp = stParam->uRefFreq / stParam->OutParams[uI].uOutputFreq;
                        
                        // Check if the divider exist and possible exact divider?
                        if((uDividerTmp * stParam->OutParams[uI].uOutputFreq) == stParam->uRefFreq && _Cdce62005_CheckOutDividerExist(uDividerTmp))
                        {
                            // We bypass the vco here as desired and uses a direct divider for this output
                            stParam->aucVcoBypass[uI] = 1;
                            stParam->aucOutDivider[uI] = uDividerTmp;
                        }
                        else
                        {
                            // There is no solution here if we don't use the vco
                            return eCdce62005NoFanoutDivider03;
                        }
                    }
                    break;
                // Cannot get here because of validation made above
                default:
                    break;
            }
        }
    }

    // If we don't have any frequency that needs the pll, we completly bypass the vco for all outputs
    if(uUniqueFreqSize == 0)
    {
        stParam->ucVcoBypassAll = 1;
        
        // Calculates desired phase index
        Ret = _Cdce62005_CalculatePhaseIndex(stParam);
        if(FAILURE(Ret)) return Ret;
        // Calculates aux output frequency and returns result
        return(_Cdce62005_CalculateAuxFreq(stParam));
    }

    // we have to make sure that smartmux input frequency is in allowed range
    // Check the reference frequency we want to use for the pll
    switch(stParam->SmartMuxRefSelect)
    {
        case eCdce62005InputRefModePrimary:
            pucInputPreDiv = &stParam->ucPrimaryInputDivider;
            if (stParam->uRefFreq > CDCE62005_SMARTMUXFREQMAX)
                return(eCdce62005BadInputPllFreq01);
            else if (stParam->uRefFreq > (CDCE62005_SMARTMUXFREQMAX / 2) || stParam->RefInput.PrimaryInput.ucForceDiv2 != 0)
            {
                // Needing divide by 2
                stParam->ucPrimaryInputDivider = 2;
            }
            else
                stParam->ucPrimaryInputDivider = 1;
			ucInputPreDivBk = *pucInputPreDiv;
            break;
        case eCdce62005InputRefModeSecondary:
            pucInputPreDiv = &stParam->ucSecondaryInputDivider;
            if (stParam->uRefFreq > CDCE62005_SMARTMUXFREQMAX)
                return(eCdce62005BadInputPllFreq02);
            else if (stParam->uRefFreq > (CDCE62005_SMARTMUXFREQMAX / 2) || stParam->RefInput.SecondaryInput.ucForceDiv2 != 0)
            {
                // Needing divide by 2
                stParam->ucSecondaryInputDivider = 2;
            }
            else
                stParam->ucSecondaryInputDivider = 1;
			ucInputPreDivBk = *pucInputPreDiv;
            break;
        case eCdce62005InputRefModeAux:
            break;
        default:
            // Cannot get here because of previous validation of this parameter
            break;
    }

    // Calculate a vector of all possibles synth frequencies for the output needing pll
    Ret = _Cdce62005_CalculateSynthFreq(auUniqueFreq, uUniqueFreqSize, auSynthFreq, &uSynthFreqSize);
    if(FAILURE(Ret)) return Ret;

    // Did not find a solution yet
    uFindOne = 0;
    
    // Adjust the starting/stopping loops index
    iSynthFreqStart = uSynthFreqSize-1;
    iSynthFreqStop = 0;
    uTryIndexStart = 0;
    uTryIndexStop = (sizeof(Cdce62005_ausPossibleFeedbackDiv) / sizeof(Cdce62005_ausPossibleFeedbackDiv[0]));
    uFirstFind = 0;
    uTryEverything = 0;
    
    // Loop until we are finding a complete solution if exists
    while(!uFindOne)
    {
        // Scan all possible calculated synth frequencies to find a working solution
        // Starting with highest one to allow some potential room for output phase adjust
        for (iCount=iSynthFreqStart; iCount >= iSynthFreqStop && !uFindOne; iCount--)
        {
            // Makes sur we have tried each possible pfd frequency calculated with _Cdce62005_CalculateFactors
            // This is only necessary normally if there is an attempt to calculate loop filter parameters that cannot be solved...
            for (uTryIndex = uTryIndexStart; !uFindOne && uTryIndex < uTryIndexStop; uTryIndex++)
            {
                // Recover prediv backup
        		if (pucInputPreDiv)
        		    *pucInputPreDiv = ucInputPreDivBk;
        
                // Recover default values which may be changed by _Cdce62005_CalculateFactors
                stParam->uPfdFreq = 0;
                stParam->usFeedbackDivider = 8;
                stParam->usCFeedbackDivider = 8;
                stParam->ucBypassDivider = 1;
                stParam->usCFeedbackDividerIndex = 0;
                stParam->usReferenceDivider = 1;
                stParam->usInputDivider = 1;
        
        	    // Calculates input divider, reference divider, pfd frequency and feedback divider (and possibly also input prediv)
                Ret = _Cdce62005_CalculateFactors(stParam, auSynthFreq[iCount], &uTryIndex, pucInputPreDiv);
                
                // Still valid so far
                if(SUCCESS(Ret))
                {
                    // Yes, calculate prescaler
                    Ret = _Cdce62005_CalculatePrescaler(stParam, auSynthFreq[iCount]);
                    if(SUCCESS(Ret))
                    {
                        stParam->uSynthFreq = auSynthFreq[iCount];
                        
                        // For all outputs
                        for(uJ=0; uJ<CDCE62005_NPLL; uJ++)
                        {
                            // If this output bypass the pll
                            if(stParam->OutParams[uJ].uOutputFreq && !stParam->aucVcoBypass[uJ])
                            {
                                // No, compute the output divider for this output
                                uDividerTmp = auSynthFreq[iCount] / stParam->OutParams[uJ].uOutputFreq;
                                
                                // Check if this output divider is within range and exact as a safety measure
                                if(uDividerTmp > CDCE62005_VCOMAXOUTPUTDIVIDER || uDividerTmp * stParam->OutParams[uJ].uOutputFreq != auSynthFreq[iCount])
                                {
                                    // No, error
                                    Ret = eCdce62005OutputDivImpossible;
                                    break;
                                }else{
                                    // Save the needed divider value for this output
                                    stParam->aucOutDivider[uJ] = uDividerTmp;
                                }
                            }
                        }
                        if (SUCCESS(Ret))
                        {
                            // Calculates desired phase index
                            Ret = _Cdce62005_CalculatePhaseIndex(stParam);
                            if(SUCCESS(Ret))
                            {
                                // Calculates auxiliary output frequency
                                Ret = _Cdce62005_CalculateAuxFreq(stParam);
                                if(SUCCESS(Ret))
                                {
                                    // We found a Synthetiser solution here, but maybe without precalculated loop filter solution...
                                    // Ok backup were we found this synth solution only if this is the first one we found
                                    if (!uFirstFind)
                                    {
                                        iSynthFreqStartBk = iSynthFreqStart;
                                        uTryIndexStartBk = uTryIndexStart;
                                        uFirstFind = 1;
                                    }
                                    
                                    // Are we using internal loop filter type? If not, it will never be calculated
                                    // user would need to provide it's configuration
                                    if (stParam->LoopFilterType == eCdce62005LoopFilterInt)
                                    {
                                        // Looks if we have a loopfilter solution for this synthetized frequency
                                        // !!! M.T. Warning, this code assumes that there are no synth freq entries that are double in the table
                                        if ((puFoundSynth = (unsigned *)bsearch(&auSynthFreq[iCount], Cdce62005_auLoopFilterConfigSyntFrq,
                                             CDCE62005_MAXLOOPFILT, sizeof(Cdce62005_auLoopFilterConfigSyntFrq[0]),
                                             _Cdce62005_Cmp)) != NULL)
                                        {
                                            // Checks to see if the combine feedback divider is the same as our solution?
                                            // Get index of the matching synth freq
                                            usIndexSol =  (unsigned)(puFoundSynth - &Cdce62005_auLoopFilterConfigSyntFrq[0]);
                                            if (Cdce62005_ausLoopFilterConfigCFbDiv[usIndexSol] == stParam->usCFeedbackDivider)
                                            {
                                                // Yes we have a loopfilter solution for this
                                                stParam->ChargePump = Cdce62005_aucLoopFilterConfigParts[usIndexSol][0];
                                                stParam->C1Value = Cdce62005_aucLoopFilterConfigParts[usIndexSol][1];
                                                stParam->R2Value = Cdce62005_aucLoopFilterConfigParts[usIndexSol][2];
                                                stParam->C2Value = Cdce62005_aucLoopFilterConfigParts[usIndexSol][3];
                                                stParam->R3Value = Cdce62005_aucLoopFilterConfigParts[usIndexSol][4];
                                                stParam->C3Value = Cdce62005_aucLoopFilterConfigParts[usIndexSol][5];
                                                stParam->sLoopFilterIndex = usIndexSol;
        
                                                // We found a solution with matching loop filter
                                                uFindOne = 1;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        // We found a synthetized solution with externally calculated loop filter
                                        uFindOne = 1;

                                        // The loop filter is external and needs user calculation
                                        Ret = eCdce62005NoFilterSolution;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else
                        Ret = eCdce62005NoPrescaler02;
                }
                else
                    break;
            }
        }
        // Did we get a complete solution with loop filter?
        if (!uFindOne)
        {
            // At least did we find a synthetized solution?
            if (uFirstFind)
            {
                // Did we get at least our synth solution
                if (!uTryEverything)
                {
                    iSynthFreqStart = iSynthFreqStartBk;
                    iSynthFreqStop = iSynthFreqStartBk;
                    uTryIndexStart = uTryIndexStartBk;
                    uTryIndexStop = uTryIndexStartBk + 1;
                    uTryEverything = 1;
                }
                else
                {
                    // Did we get any error so far
                    if (SUCCESS(Ret))
                    {
                        Ret = eCdce62005NoFilterSolution;
                    }
                    break;
                }
            }
            else
            {
               // Did we get any error so far
               if (SUCCESS(Ret))
               {
                   Ret = eCdce62005NoSynthSolution;
               }
               break;
            }
        }
    }

    return Ret;
}

#ifndef CDCE62005P_CALC_ONLY

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
Result Cdce62005_SetParameters(Cdce62005_stCore * pCore, Cdce62005_stParam * stParam, unsigned uSync)
{
    Result Ret;
    unsigned char ucDivTmp;
    Cdce62005_stRegisterMap Regs;
    Cdce62005_eAddr Addr;

    // Charge pump validation
    if (stParam->ChargePump > eCdce62005ChargePump3D75MA || stParam->ChargePump < eCdce62005ChargePump50D0UA)
        return(eCdce62005BadChargePump);
        
    // C1 validation
    if (stParam->C1Value > eCdce62005C1Val77D0PF || stParam->C1Value < eCdce62005C1Val0D0PF)
        return(eCdce62005BadC1);
        
    // C2 validation
    if (stParam->C2Value > eCdce62005C2Val473D5PF || stParam->C2Value < eCdce62005C2Val0D0PF)
        return(eCdce62005BadC2);
        
    // C3 validation
    if (stParam->C3Value > eCdce62005C3Val112D5PF || stParam->C3Value < eCdce62005C3Val0D0PF)
        return(eCdce62005BadC3);
        
    // R2 validation
    if (stParam->R2Value > eCdce62005R2Val0D1KO || stParam->R2Value < eCdce62005R2Val127D6KO)
        return(eCdce62005BadR2);
        
    // R3 validation
    if (stParam->R3Value > eCdce62005R3Val5D0KO || stParam->R3Value < eCdce62005R3Val20D0KO)
        return(eCdce62005BadR3);
        
    // Loop filter type validation
    if (stParam->LoopFilterType > eCdce62005LoopFilterExt || stParam->LoopFilterType < eCdce62005LoopFilterInt)
        return(eCdce62005BadLoopFilterType);

    // Primary input pre divider validation
    if (stParam->ucPrimaryInputDivider > 2)
        return(eCdce62005BadPriPreDiv);

    // Secondary input pre divider validation
    if (stParam->ucSecondaryInputDivider > 2)
        return(eCdce62005BadSecPreDiv);

    // Reference divider validation
    if (stParam->usReferenceDivider > 8 || stParam->usReferenceDivider < 1)
        return(eCdce62005BadRefDivider);

    // Smartmux reference selection validation
    if (stParam->SmartMuxRefSelect != eCdce62005InputRefModePrimary &&
        stParam->SmartMuxRefSelect != eCdce62005InputRefModeSecondary &&
        stParam->SmartMuxRefSelect != eCdce62005InputRefModeAux)
        return(eCdce62005BadInputRefMode);

    // Input types validation
    if (stParam->RefInput.InputsType != eCdce62005InputLvCmos &&
        stParam->RefInput.InputsType != eCdce62005InputLvPecl &&
        stParam->RefInput.InputsType != eCdce62005InputLvds)
        return(eCdce62005BadInputPinsType);

    // Primary input termination control validation
    if (stParam->RefInput.PrimaryInput.InputTerm > eCdce62005InputTermDisabled || stParam->RefInput.PrimaryInput.InputTerm < eCdce62005InputTermEnabled)
        return(eCdce62005BadPriTermCtrl);

    // Secondary input termination control validation
    if (stParam->RefInput.SecondaryInput.InputTerm > eCdce62005InputTermDisabled || stParam->RefInput.SecondaryInput.InputTerm < eCdce62005InputTermEnabled)
        return(eCdce62005BadSecTermCtrl);

    // Lvds fail safe mode validation
    if (stParam->RefInput.LvdsFailSafe > eCdce62005LvdsInputFailEnable || stParam->RefInput.LvdsFailSafe < eCdce62005LvdsInputFailDisable)
        return(eCdce62005BadLvdsFailSafeMode);

    // Primary input Vbb mode validation
    if (stParam->RefInput.PrimaryInput.InputVbb > eCdce62005InputVbbInverted || stParam->RefInput.PrimaryInput.InputVbb < eCdce62005InputVbbNormal)
        return(eCdce62005BadPriVbbMode);

    // Secondary input Vbb mode validation
    if (stParam->RefInput.SecondaryInput.InputVbb > eCdce62005InputVbbInverted || stParam->RefInput.SecondaryInput.InputVbb < eCdce62005InputVbbNormal)
        return(eCdce62005BadSecVbbMode);

    // Input termination mode validation
    if (stParam->RefInput.InputsTermination > eCdce62005InputDcTermination || stParam->RefInput.InputsTermination < eCdce62005InputAcTermination)
        return(eCdce62005BadInputTermMode);

    // Vco frequency validation
    if ((stParam->uVcoFreq > CDCE62005_VCOFREQMAX || stParam->uVcoFreq < CDCE62005_VCOFREQMIN) && stParam->uVcoFreq != 0)
        return(eCdce62005BadVcoFrequency);

    // Pll lock pin mode validation
    if (stParam->PllLockParams.PllLockMode > eCdce62005AnalogPllLock || stParam->PllLockParams.PllLockMode < eCdce62005DigitalPllLock)
        return(eCdce62005BadPllLockPinMode);

    // Pll lock count mode validation
    if (stParam->PllLockParams.PllLockCount > eCdce62005PllLockCount64Pfd || stParam->PllLockParams.PllLockCount < eCdce62005PllLockCount1Pfd)
        return(eCdce62005BadPllLockCountMode);

    // Pll lock width validation
    if (stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW1D5Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW3D4Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW5D4Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW5D8Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW7D7Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW9D7Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW15D0Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW15D1Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW17D0Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW19D0Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW19D3Ns &&
        stParam->PllLockParams.PllLockWidth != eCdce62005PllLockW28D6Ns)
        return(eCdce62005BadPllLockWidth);

    // Input divider validation
    if (stParam->usInputDivider > 256 || stParam->usInputDivider < 1)
        return(eCdce62005BadInputDivider);

    // Auxiliary output selection validation
    if (stParam->AuxParams.AuxSelect > eCdce62005AuxSelect3 || stParam->AuxParams.AuxSelect < eCdce62005AuxSelect2)
        return(eCdce62005BadAuxOutputSelect);

    // Auxiliary output state validation
    if (stParam->AuxParams.AuxState > eCdce62005AuxEnable || stParam->AuxParams.AuxState < eCdce62005AuxDisable)
        return(eCdce62005BadAuxOutputState);

    // Output sync mode validation
    if (stParam->SyncMode > Cdce62005_eSyncNextOutput || stParam->SyncMode < Cdce62005_eSync6UsNextRefAndOutput)
        return(eCdce62005BadSyncMode);

    // Output params validation
    for (ucDivTmp = 0; ucDivTmp <= CDCE62005_NPLL; ucDivTmp++)
    {
        // Checks output phase index
        if (stParam->aucOutPhaseAdjNdx[ucDivTmp] >= stParam->aucOutDivider[ucDivTmp] && stParam->aucOutDivider[ucDivTmp] != 0)
            return(eCdce62005BadOutput0Phase + ucDivTmp);
            
        // Checks output clock reference
        if (stParam->OutParams[ucDivTmp].OutputRef > eCdce62005OutRefModeSynth || stParam->OutParams[ucDivTmp].OutputRef < eCdce62005OutRefModePrimary)
            return(eCdce62005BadOutput0Ref + ucDivTmp);
            
        // Checks positive lvcmos pin mode
        if (stParam->OutParams[ucDivTmp].PLvCmosMode > eCdce62005LvCmosOutPinLow || stParam->OutParams[ucDivTmp].PLvCmosMode < eCdce62005LvCmosOutPinActive)
            return(eCdce62005BadPLvcmosOut0Mode + ucDivTmp);

        // Checks negative lvcmos pin mode
        if (stParam->OutParams[ucDivTmp].NLvCmosMode > eCdce62005LvCmosOutPinLow || stParam->OutParams[ucDivTmp].NLvCmosMode < eCdce62005LvCmosOutPinActive)
            return(eCdce62005BadNLvcmosOut0Mode + ucDivTmp);
    }

    // Readback pll registers to get reserved bits correctly (assumed device is coming from reset
    // with a valid configuration (with all correct reserved bits values)
    for (Addr = eCdce62005Addr00; Addr <= eCdce62005Addr08; Addr++)
    {
        Ret = Cdce62005_ReadReg(pCore, Addr, &Regs.m_auRegisters[Addr]);
        if(FAILURE(Ret)) return Ret;
    }

    if (uSync)
    {
        //disable outputs
        Regs.stRegister08.stBit.m_uSync = 0;
        Ret = Cdce62005_WriteReg(pCore, 8, Regs.m_auRegisters[8]);
        if(FAILURE(Ret)) return Ret;
    }

    // Loop filter parameters
    Regs.stRegister06.stBit.m_uChargePumpCurrent = stParam->ChargePump;
    Regs.stRegister07.stBit.m_uLoopFilterC1 = stParam->C1Value;
    Regs.stRegister07.stBit.m_uLoopFilterR2 = stParam->R2Value;
    Regs.stRegister07.stBit.m_uLoopFilterC2 = stParam->C2Value;
    Regs.stRegister07.stBit.m_uLoopFilterR3 = stParam->R3Value;
    Regs.stRegister07.stBit.m_uLoopFilterC3 = stParam->C3Value;
    
    Regs.stRegister06.stBit.m_uExternalLoopFilter = stParam->LoopFilterType;  // Which loop filter mode (internal or external)
    
    Regs.stRegister06.stBit.m_uChargePumpPulseWidth = 0; // charge pump narrow pulse width

    //References pre divider
    Regs.stRegister00.stBit.m_uPrimaryPredivider = Cdce62005_aucInputPredividerReg[stParam->ucPrimaryInputDivider];
    Regs.stRegister01.stBit.m_uSecondaryPredivider = Cdce62005_aucInputPredividerReg[stParam->ucSecondaryInputDivider];

    // Set reference divider
    Regs.stRegister02.stBit.m_uReferenceDivider = (stParam->usReferenceDivider - 1) & 3;
    Regs.stRegister03.stBit.m_uReferenceDivider = ((stParam->usReferenceDivider - 1) & 4) >> 2;

    //smart mux and inputs related
    Regs.stRegister05.stBit.m_uSoftInputSel = 1; // Follows internal clock source selection (ignores REF_SEL pin)
    Regs.stRegister05.stBit.m_uClockSelect = stParam->SmartMuxRefSelect; // Configure pll reference source
    Regs.stRegister05.stBit.m_uBufSelect = stParam->RefInput.InputsType; // Choose which inputs types
    Regs.stRegister05.stBit.m_uPrimaryInputTerminationDis = stParam->RefInput.PrimaryInput.InputTerm; // Primary input termination state
    Regs.stRegister06.stBit.m_uSecondaryInputTerminationDis = stParam->RefInput.SecondaryInput.InputTerm; // Secondary input termination state
    Regs.stRegister05.stBit.m_uFailSafe = stParam->RefInput.LvdsFailSafe; // lvds inputs failsafe mode
    Regs.stRegister05.stBit.m_uSecondaryInputBiased = stParam->RefInput.SecondaryInput.InputVbb; // choose between inverted/normal input 
    Regs.stRegister05.stBit.m_uPrimaryInputBiased = stParam->RefInput.PrimaryInput.InputVbb; // choose between inverted/normal input
    Regs.stRegister05.stBit.m_uBufferHysteresis = 1; // Choose input buffers hysteresis enabled
    Regs.stRegister05.stBit.m_uDcTerminationEn = stParam->RefInput.InputsTermination; // choose between ac or dc termination 

    //prescale divider
    Ret = _Cdce62005_DoPrescaler2Reg(stParam->ucPrescaleDivider, &ucDivTmp);
    if(FAILURE(Ret)) return Ret;
    Regs.stRegister06.stBit.m_uPrescaler = ucDivTmp;

    //choose VCO
    if (stParam->uVcoFreq > CDCE62005_HIGHVCOFREQ)
    {
        Regs.stRegister06.stBit.m_uVcoRange = 1;
    }else{
        Regs.stRegister06.stBit.m_uVcoRange = 0;
    }

    // Configure pll lock parameters
    Regs.stRegister05.stBit.m_uDigitalLockEn = stParam->PllLockParams.PllLockMode;
    Regs.stRegister05.stBit.m_uLongLock = stParam->PllLockParams.PllLockCount;
    Regs.stRegister05.stBit.m_uLockWindows = stParam->PllLockParams.PllLockWidth;
    
    //input divider
    Regs.stRegister05.stBit.m_uInputDivider = stParam->usInputDivider - 1;

    // If at least one output is using the pll
    if( !(stParam->ucVcoBypassAll))
    {
        //feedback divider
        if (stParam->usCFeedbackDividerIndex < (sizeof(Cdce62005_ausPossibleFeedbackDiv)/sizeof(Cdce62005_ausPossibleFeedbackDiv[0])))
        {
            Regs.stRegister06.stBit.m_uFeedbackDivider = Cdce62005_ausPossibleFeedbackDiv0[Cdce62005_ausPossibleFeedbackDiv[stParam->usCFeedbackDividerIndex][1]][1];
            Regs.stRegister06.stBit.m_uDividerBypass = Cdce62005_aucPossibleFeedbackDiv1[Cdce62005_ausPossibleFeedbackDiv[stParam->usCFeedbackDividerIndex][2]][1];
        }else{
            return eCdce62005Divider03;
        }
    }
    else
    {
        // default unused values
        Regs.stRegister06.stBit.m_uFeedbackDivider = Cdce62005_ausPossibleFeedbackDiv0[0][1];
        Regs.stRegister06.stBit.m_uDividerBypass = Cdce62005_aucPossibleFeedbackDiv1[0][1];
    }

    // Auxiliary output configuration
    Regs.stRegister06.stBit.m_uAuxSel = stParam->AuxParams.AuxSelect;  // Auxiliary output that drives it
    Regs.stRegister06.stBit.m_uAuxEn = stParam->AuxParams.AuxState;    // Auxiliary output enable state
    
    // Synchronisation mode
    Regs.stRegister06.stBit.m_uSyncMode2 = stParam->SyncMode & 1;
    Regs.stRegister04.stBit.m_uSyncMode1 = (stParam->SyncMode & 2) >> 1;
    
    // Makes sure the device is not in sleep mode
    Regs.stRegister08.stBit.m_uSleep = 1;

    if (!uSync)
        Regs.stRegister08.stBit.m_uSync = 1;
    
    // Set smartmux delay mode (it is recommended to be 1 for ref inputs < 150 MHz, but no info above 150 MHz...)
    Regs.stRegister07.stBit.m_uSelDel1 = 1;
    Regs.stRegister07.stBit.m_uSelDel2 = 1;
    
    //Output 0
    Regs.stRegister00.stBit.m_uPhaseAdjust = stParam->aucOutPhaseAdjNdx[0];
    Regs.stRegister00.stBit.m_uOutputMux = Cdce62005_aucOutMuxReg[stParam->OutParams[0].OutputRef];
    if (stParam->aucOutDivider[0] != 0)
    {
        Ret = _Cdce62005_DoDivider2Reg(stParam->aucOutDivider[0], &ucDivTmp);
        if(FAILURE(Ret)) return eCdce62005Ival01;
        Regs.stRegister00.stBit.m_uDivider = ucDivTmp;
        Regs.stRegister00.stBit.m_uDividerEnable = 1;
    }else{
        Regs.stRegister00.stBit.m_uDividerEnable = 0;
    }
    
    Regs.stRegister00.stBit.m_uHiSwing = 0;
    switch(stParam->OutParams[0].OutputMode)
    {
        case eCdce62005OutModeLvCmos:
            Regs.stRegister00.stBit.m_uOutputMode = eCdce62005OutModeRegLvCmos | stParam->OutParams[0].PLvCmosMode | ((stParam->OutParams[0].NLvCmosMode) << 2);
            break;
            
        case eCdce62005OutModeLvds:
            Regs.stRegister00.stBit.m_uOutputMode = eCdce62005OutModeRegLvds;
            break;
            
        case eCdce62005OutModeLvPeclHighSwing:
            Regs.stRegister00.stBit.m_uHiSwing = 1;
        case eCdce62005OutModeLvPecl:
            Regs.stRegister00.stBit.m_uOutputMode = eCdce62005OutModeRegLvPecl;
            break;
            
        case eCdce62005OutModeDisabled:
            Regs.stRegister00.stBit.m_uOutputMode = eCdce62005OutModeRegDisabled;
            break; 
            
        default:
            return(eCdce62005BadPinOutMode02);
    }
    
    //Output 1
    Regs.stRegister01.stBit.m_uPhaseAdjust = stParam->aucOutPhaseAdjNdx[1];
    Regs.stRegister01.stBit.m_uOutputMux = Cdce62005_aucOutMuxReg[stParam->OutParams[1].OutputRef];
    if (stParam->aucOutDivider[1] != 0)
    {
        Ret = _Cdce62005_DoDivider2Reg(stParam->aucOutDivider[1], &ucDivTmp);
        if(FAILURE(Ret)) return eCdce62005Ival01;
        Regs.stRegister01.stBit.m_uDivider = ucDivTmp;
        Regs.stRegister01.stBit.m_uDividerEnable = 1;
    }else{
        Regs.stRegister01.stBit.m_uDividerEnable = 0;
    }
    
    Regs.stRegister01.stBit.m_uHiSwing = 0;
    switch(stParam->OutParams[1].OutputMode)
    {
        case eCdce62005OutModeLvCmos:
            Regs.stRegister01.stBit.m_uOutputMode = eCdce62005OutModeRegLvCmos | stParam->OutParams[1].PLvCmosMode | ((stParam->OutParams[1].NLvCmosMode) << 2);
            break;
            
        case eCdce62005OutModeLvds:
            Regs.stRegister01.stBit.m_uOutputMode = eCdce62005OutModeRegLvds;
            break;
            
        case eCdce62005OutModeLvPeclHighSwing:
            Regs.stRegister01.stBit.m_uHiSwing = 1;
        case eCdce62005OutModeLvPecl:
            Regs.stRegister01.stBit.m_uOutputMode = eCdce62005OutModeRegLvPecl;
            break;
            
        case eCdce62005OutModeDisabled:
            Regs.stRegister01.stBit.m_uOutputMode = eCdce62005OutModeRegDisabled;
            break; 
            
        default:
            return(eCdce62005BadPinOutMode03);
    }
    
    //Output 2
    Regs.stRegister02.stBit.m_uPhaseAdjust = stParam->aucOutPhaseAdjNdx[2];
    Regs.stRegister02.stBit.m_uOutputMux = Cdce62005_aucOutMuxReg[stParam->OutParams[2].OutputRef];
    if (stParam->aucOutDivider[2] != 0)
    {
        Ret = _Cdce62005_DoDivider2Reg(stParam->aucOutDivider[2], &ucDivTmp);
        if(FAILURE(Ret)) return eCdce62005Ival01;
        Regs.stRegister02.stBit.m_uDivider = ucDivTmp;
        Regs.stRegister02.stBit.m_uDividerEnable = 1;
    }else{
        Regs.stRegister02.stBit.m_uDividerEnable = 0;
    }
    
    Regs.stRegister02.stBit.m_uHiSwing = 0;
    switch(stParam->OutParams[2].OutputMode)
    {
        case eCdce62005OutModeLvCmos:
            Regs.stRegister02.stBit.m_uOutputMode = eCdce62005OutModeRegLvCmos | stParam->OutParams[2].PLvCmosMode | ((stParam->OutParams[2].NLvCmosMode) << 2);
            break;
            
        case eCdce62005OutModeLvds:
            Regs.stRegister02.stBit.m_uOutputMode = eCdce62005OutModeRegLvds;
            break;
            
        case eCdce62005OutModeLvPeclHighSwing:
            Regs.stRegister02.stBit.m_uHiSwing = 1;
        case eCdce62005OutModeLvPecl:
            Regs.stRegister02.stBit.m_uOutputMode = eCdce62005OutModeRegLvPecl;
            break;
            
        case eCdce62005OutModeDisabled:
            Regs.stRegister02.stBit.m_uOutputMode = eCdce62005OutModeRegDisabled;
            break; 
            
        default:
            return(eCdce62005BadPinOutMode04);
    }

    //Output 3
    Regs.stRegister03.stBit.m_uPhaseAdjust = stParam->aucOutPhaseAdjNdx[3];
    Regs.stRegister03.stBit.m_uOutputMux = Cdce62005_aucOutMuxReg[stParam->OutParams[3].OutputRef];
    if (stParam->aucOutDivider[3] != 0)
    {
        Ret = _Cdce62005_DoDivider2Reg(stParam->aucOutDivider[3], &ucDivTmp);
        if(FAILURE(Ret)) return eCdce62005Ival01;
        Regs.stRegister03.stBit.m_uDivider = ucDivTmp;
        Regs.stRegister03.stBit.m_uDividerEnable = 1;
    }else{
        Regs.stRegister03.stBit.m_uDividerEnable = 0;
    }
    
    Regs.stRegister03.stBit.m_uHiSwing = 0;
    switch(stParam->OutParams[3].OutputMode)
    {
        case eCdce62005OutModeLvCmos:
            Regs.stRegister03.stBit.m_uOutputMode = eCdce62005OutModeRegLvCmos | stParam->OutParams[3].PLvCmosMode | ((stParam->OutParams[3].NLvCmosMode) << 2);
            break;
            
        case eCdce62005OutModeLvds:
            Regs.stRegister03.stBit.m_uOutputMode = eCdce62005OutModeRegLvds;
            break;
            
        case eCdce62005OutModeLvPeclHighSwing:
            Regs.stRegister03.stBit.m_uHiSwing = 1;
        case eCdce62005OutModeLvPecl:
            Regs.stRegister03.stBit.m_uOutputMode = eCdce62005OutModeRegLvPecl;
            break;
            
        case eCdce62005OutModeDisabled:
            Regs.stRegister03.stBit.m_uOutputMode = eCdce62005OutModeRegDisabled;
            break; 
            
        default:
            return(eCdce62005BadPinOutMode05);
    }

    //Output 4
    Regs.stRegister04.stBit.m_uPhaseAdjust = stParam->aucOutPhaseAdjNdx[4];
    Regs.stRegister04.stBit.m_uOutputMux = Cdce62005_aucOutMuxReg[stParam->OutParams[4].OutputRef];
    if (stParam->aucOutDivider[4] != 0)
    {
        Ret = _Cdce62005_DoDivider2Reg(stParam->aucOutDivider[4], &ucDivTmp);
        if(FAILURE(Ret)) return eCdce62005Ival01;
        Regs.stRegister04.stBit.m_uDivider = ucDivTmp;
        Regs.stRegister04.stBit.m_uDividerEnable = 1;
    }else{
        Regs.stRegister04.stBit.m_uDividerEnable = 0;
    }
    
    Regs.stRegister04.stBit.m_uHiSwing = 0;
    switch(stParam->OutParams[4].OutputMode)
    {
        case eCdce62005OutModeLvCmos:
            Regs.stRegister04.stBit.m_uOutputMode = eCdce62005OutModeRegLvCmos | stParam->OutParams[4].PLvCmosMode | ((stParam->OutParams[4].NLvCmosMode) << 2);
            break;
            
        case eCdce62005OutModeLvds:
            Regs.stRegister04.stBit.m_uOutputMode = eCdce62005OutModeRegLvds;
            break;
            
        case eCdce62005OutModeLvPeclHighSwing:
            Regs.stRegister04.stBit.m_uHiSwing = 1;
        case eCdce62005OutModeLvPecl:
            Regs.stRegister04.stBit.m_uOutputMode = eCdce62005OutModeRegLvPecl;
            break;
            
        case eCdce62005OutModeDisabled:
            Regs.stRegister04.stBit.m_uOutputMode = eCdce62005OutModeRegDisabled;
            break; 
            
        default:
            return(eCdce62005BadPinOutMode06);
    }

    // Default calibration mode manual
    Regs.stRegister06.stBit.m_uPllCalibration = 1;
    Regs.stRegister06.stBit.m_uVcoCalibration = 0;

    // Writes back pll registers
    for (Addr = eCdce62005Addr00; Addr <= eCdce62005Addr08; Addr++)
    {
        Ret = Cdce62005_WriteReg(pCore, Addr, Regs.m_auRegisters[Addr]);
        if(FAILURE(Ret)) return Ret;
    }

    //calibrate VCO to new Freq manually (m_uVcoCalibration bit edge from 0 -> 1).
    Regs.stRegister06.stBit.m_uVcoCalibration = 1;
    Ret = Cdce62005_WriteReg(pCore, eCdce62005Addr06, Regs.m_auRegisters[eCdce62005Addr06]);
    if(FAILURE(Ret)) return Ret;

    //add fixed delay to let the PLL stabilize and calibration to complete.
    Carrier_usleep(pCore->hCarrier, CDCE62005_PLLCALIB_DELAY);
        
    // If at least one output is using the pll
    if( !(stParam->ucVcoBypassAll))
    {
        unsigned uLock;
        
        Ret = Cdce62005_WaitPllLock(pCore, &uLock);
        if(FAILURE(Ret)) return Ret;
        
        if (!uLock)
        {
            return eCdce62005PllLockError;
        }
    }

    return eCdce62005Ok;
}


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
Result Cdce62005_DoSync(Cdce62005_stCore * pCore)
{
    Result Ret;
    Cdce62005_stRegisterMap stRegs;
    Cdce62005_eAddr Addr = eCdce62005Addr08;
    
    Ret = Cdce62005_ReadReg(pCore, Addr, &stRegs.m_auRegisters[Addr]);
    if(FAILURE(Ret)) return Ret;

    stRegs.stRegister08.stBit.m_uSync = 0;
    Ret = Cdce62005_WriteReg(pCore, Addr, stRegs.m_auRegisters[Addr]);
    if(FAILURE(Ret)) return Ret;
    stRegs.stRegister08.stBit.m_uSync = 1;
    Ret = Cdce62005_WriteReg(pCore, Addr, stRegs.m_auRegisters[Addr]); 
    if(FAILURE(Ret)) return Ret;

    //add fixed delay to let the synchronization completing
    Carrier_usleep(pCore->hCarrier, CDCE62005_PLLSYNC_DELAY);

    return Ret;
}


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
Result Cdce62005_WaitPllLock(Cdce62005_stCore * pCore, unsigned *puLock)
{
    Result Ret;
    Cdce62005_stRegisterMap Regs;
    unsigned uPllLockCount, uPllLockWait;

    // By default, not locked
    *puLock = 0;
        
    // Yes, so we need the pll to lock
    // Sample and wait for pll to lock for the max allowed time
    uPllLockCount = 0;
    uPllLockWait = 0;
    do
    {
        Ret = Cdce62005_ReadReg(pCore, eCdce62005Addr08, &Regs.m_auRegisters[eCdce62005Addr08]);
        if(FAILURE(Ret)) return Ret;
        
        // Count for the number of pll lock count detected
        if (Regs.stRegister08.stBit.m_uPllLock == eCdce62005PllLocked)
            uPllLockCount++;
        else
            uPllLockCount = 0;
    
        //add fixed minimum delay for each loop
        Carrier_usleep(pCore->hCarrier, CDCE62005_PLLLOCK_LOOP_DELAY);
    }while(uPllLockWait++ < CDCE62005_PLLLOCK_WAITLOOP);
    
    // Did the pll was locked long enough to be satisfied
    if (uPllLockCount >= CDCE62005_PLLLOCK_VALID)
    {
        *puLock = 1;
    }
    
    return Ret;
}
#endif
