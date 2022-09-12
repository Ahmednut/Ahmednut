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
 * @file      
 * @brief      
 *
 * $Date: 
 * $Revision: 
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup EAPI
 *
 * 
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


#ifndef __EAPI_WDXG__
#define __EAPI_WDXG__

#include "connection_state.h"
#include "adac250_types.h"
#include "bas_types.h"



#include "wsa_api.h"


#include "eapi_adac250.h"
#include "cce.h"
#include "bas_types.h"
#include "bas_error.h"
#include "connection_state.h"




#define OFF 0
#define ON  1



//Clock Source enumeration
typedef enum wdxg_clksrc
{
   WDXG_CLKSRC_ALLINTERNAL=0, // Both ADAC250 and WSA5000 set to their internal clock
   WDXG_CLKSRC_WSA5000=1      // WSA5000 supplies 10MHz clock to ADAC250
} wdxg_clksrc;

typedef struct wdxg_clocking	
{
   wdxg_clksrc eClkSrc;         // clock source chosen
   uint32_t u32SrcClkFreq;      // clock source frequency value in Hz
   uint32_t u32SamplingClkFreq; // ADAC250 sampling clock frequency in Hz
} wdxg_clocking;

typedef struct wdxg_sweep
{
   uint64_t u64Sweep_StartFreq;    // Sweep start frequency in Hz
   uint64_t u64Sweep_StopFreq;     // Sweep stop frequency in Hz
   uint64_t u32Sweep_DwellTime;	   // Dwell Time at each step in microseconds
   uint64_t u32Sweep_Step; 	   // Sweep step size in Hz
   BOOL bSweep_Enable;		   // Sweep enable (1 for sweep)
}wdxg_sweep;

typedef struct wdxg_gain
{	
   uint32_t u32AttenuationMode;	// WSA5000 attenuation mode
   uint32_t u32AttenuationVar;	// WSA5000 attenuation variable 
   enum ADAC250_ADC_GAIN u32DigitalGain;// ADAC250 Digital Gain 
} wdxg_gain;

typedef struct wsa_device* wsa_device_t;

typedef struct wdxg_handle 
{	
   connection_state * state;	// Connection state to Perseus
   wsa_device_t dev;		// WSA5000 device object (holds connection)
   wdxg_clocking* sClocking;	// WDxG clocking structure as previously defined
   uint64_t u64ReceiverFreq;	// Receiver frequency in Hz
   wdxg_sweep* sSweep;		// WDxG sweep structure as previously defined
   wdxg_gain* sGain;            // WDxG gain structure as previously defined
   uint32_t u32DownSamplingFactor; // Downsampling factor
} wdxg_handle;

typedef wdxg_handle* handle;
//Enumeration des cas d'erreur
//typedef enum wdxg_error
//{
//WDXG_OK=MAKE_RESULT(LYR_SEVERITY_SUCCESS,MODULE_WDXG, 0),
//}wdxg_error;

#ifdef __cplusplus
extern "C" {
#endif

/**
*****************************************************************************
* Function: WDxG_Open
**************************************************************************//**
*
* Allocation of object memory.
*
* @param[in]  handle pointer to the main structure
*
* 
* @return
*   Always returns 0.
*
   ****************************************************************************/
    wdxg_handle* WDxG_Open (wdxg_handle* hWDxG);

/**
*****************************************************************************
* Function: WDxG_Init
**************************************************************************//**
* 
* Initialisation of the wideband digitizer
*
* @param[in]  handle pointer to the main structure
*
* @param[in]  Pointer to the clock structure 
*
*
* @return  Error code: 0 if no problem is found, != 0 if problem.
*
****************************************************************************/
Result WDxG_Init (wdxg_handle*  hWDxG, wdxg_clocking *sClocking);
/**
*****************************************************************************
* Function: WDxG_Close 
*
* Free the object memory from memory
*
* @param[in]  handle pointer to the main structure
*
* 
* @return
*   Always returns 0.
*
****************************************************************************/
Result WDxG_Close(wdxg_handle* hWDxG);
/**
*****************************************************************************
* Function: WDxG_SetReceiverFrequency
**************************************************************************//**
*
* Sets the WSA5000 to the desired center frequency
*
* @param[in]  handle pointer to the main structure
*
* @param[in]  Center frequency 
*
* @return  Error code: 0 if no problem is found, 
*
****************************************************************************/
Result WDxG_SetReceiverFrequency(wdxg_handle* hWDxG, unsigned long long u64Frequency);
/**
*****************************************************************************
* Function: WDxG_SetSweep
**************************************************************************//**
* Initialise all sweep parameter 
* 
*
* @param[in] handle pointer to the main structure
* @param[in] pointer to sweep structure 
* 
*
* @return  Error code: 0 if no problem is found, 
*
****************************************************************************/
Result WDxG_SetSweep (wdxg_handle*  hWDxG, wdxg_sweep *sSweep);
/**
*****************************************************************************
* Function:  WDxG_StartSweep
**************************************************************************//**
*
* Start the sweep
*
* @param[in] handle pointer to the main structure
*
* 
*
*  @return  Error code: 0 if no problem is found,
*
****************************************************************************/
Result WDxG_StartSweep (wdxg_handle*  hWDxG);
/**
*****************************************************************************
* Function: WDxG_StopSweep
**************************************************************************//**
*  Stop the sweep and free all sweep parameter in the WSA
* 
*
* @param[in] handle pointer to the main structure 
*
* 
*
* @return  Error code: 0 if no problem is found,
*
****************************************************************************/
Result WDxG_StopSweep(wdxg_handle* hWDxG);
/**
*****************************************************************************
* Function: WDxG_SweepStatus
**************************************************************************//**
*
* 
*
* @param[in]  handle pointer to the main structure
*
* @param[in,out] poiter to a tab containing RUNNING or STOPPED
*
* Error code: Error code: 0 if no problem is found,
*
****************************************************************************/
Result WDxG_SweepStatus (wdxg_handle*  hWDxG, char *pu8Status);
/**
*****************************************************************************
* Function: WDxG_SetGain
**************************************************************************//**
*
* 
*
* @param[in]  handle pointer to the main structure
* @param[in]  pointer to the gain structure
* 
*
* Error code: Error code: 0 if no problem is found
*
****************************************************************************/
Result WDxG_SetGain(wdxg_handle*  hWDxG, wdxg_gain *sGain);
/**
*****************************************************************************
* Function: WDxG_Connect
**************************************************************************//**
*
* 
*
* @param[in]  handle pointer to the main structure 
*
* @param[in]  Perseus state pointer 
* @param[in]  Perseus IP address
* @param[in]  WSA IP address
* Error code: Error code: 0 if no problem is found
*
****************************************************************************/
Result WDxG_Connect(wdxg_handle* hWDxG , connection_state state, const char * IpAddress,const char * WSA_IpAddress);
#ifdef __cplusplus
}
#endif

#endif

