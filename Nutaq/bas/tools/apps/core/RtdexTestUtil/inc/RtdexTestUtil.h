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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file      RtdexTestUtil.h
 * 
 * @brief     Function for timestamp module initialization
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

#ifndef __RtdexTestUtil__
#define __RtdexTestUtil__

#include "connection_state.h"
#include "bas_types.h"
#include "bas_error.h"

#define MEGABYTE 1048576//1024*1024
#define MAX_TRANSFER_SIZE  512*MEGABYTE  //(512 MegaBytes)

#define MAX_RTDEX				50

#define RTDEX_CTRL  0                 
  #define RX_RESET_BM_n(n) (1<<(n))         // n = 0..1
  #define RX_RESET_BM_all (0xff)
  #define TX_RESET_BM_n(n) (1<<((n)+8))
  #define TX_RESET_BM_all (0xff<<8)
  #define RX_START_BM_n(n) (1<<((n)+16))    
  #define RX_START_BM_all (0xff<<16)
  #define TX_START_BM_n(n) (1<<((n)+24))    
  #define TX_START_BM_all (0xff<<24)
#define LOOPBACK_EN 1  
  #define DISABLE_FLOW_CTRL (1<<8)
  #define ENABLE_FLOW_CTRL ~(1<<8)
  #define LB_EN_BM_n(n) (1<<(n))            // n = 0..1
#define TX_DIV_CNT 2                        // n= 0..1
#define RX_EN_DIV_CNT 3
#define TX_INITVAL 4                      // n= 0..1 (rx and tx)
#define ERR_CNT_n(n) (5+4*n)                  // n= 0..1
  #define ERRCNT_VAL_BM 0xff
  #define ERRCNT_OVF_BM 0x100
#define TX_OVERFLOW_CNT(n) (6+4*n)		    // n= 0..1
#define RX_UNDERFLOW_CNT(n) (7+4*n)	        // n= 0..1
#define RX_RECEIVEDSAMPLES_CNT(n) (8+4*n)	// n= 0..1

#define UPLINK 1
#define DOWNLINK 2
#define BOTH 3

typedef struct
{
    int bAdcDacMode;          
    int bResetOnly;    
    int bSingleTransfer;

    uint32_t u32RtdexTestBaseCustomRegister;
    uint32_t u32TestRate;
    uint32_t u32TransferSize;

    char pcFileNames[500];
    uint32_t u32Direction;
    uint32_t u32NumberOfChannelsToAnalyze;
} sRtdexTestParams;

typedef sRtdexTestParams *handle_rtdextestparams;

Result RtdexTestUtil(char *pcIpAddress, handle_rtdextestparams hParams, char acAction[]);

/**
*****************************************************************************
* @file RtdexTestUtil.h
*****************************************************************************
* RTDEx test utility functions.
* @attention
*           Before using these functions, make sure that the lyt_rtdex_test 
*           core is instantiated in the FPGA.
*
****************************************************************************/
 
 
/*==========================================================================
==  PUBLIC ENUMERATIONS
==========================================================================*/
/**
*****************************************************************************
* Function: RTDExResetTest
*****************************************************************************
*
* Reset the test.
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExResetTest( connection_state *state, uint32_t u32BaseCustomRegister);

/**
*****************************************************************************
* Function: RTDExEnableUplinkTest
*****************************************************************************
*
* Enable the test.
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uEnable
*      Enable of uplink direction
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExEnableUplinkTest( connection_state *state, uint32_t u32BaseCustomRegister, unsigned char uEnable);

/**
*****************************************************************************
* Function: RTDExEnableDownlinkTest
*****************************************************************************
*
* Enable the test.
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uEnable
*      Enable of downlink direction
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExEnableDownlinkTest( connection_state *state, uint32_t u32BaseCustomRegister, unsigned char uEnable);

/**
*****************************************************************************
* Function: RTDExSetTestInitVal
*****************************************************************************
*
* Set the test initial values.
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uValue
*      Value to program in the test cnt div
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExSetTestInitVal( connection_state *state, uint32_t u32BaseCustomRegister, unsigned int uValue);


/**
*****************************************************************************
* Function: RTDExGetRxErrorCnt
*****************************************************************************
*
* Returns the error count of specific RTDEx Rx channels.
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uNumChannel
*      The RTDEx Rx channel number.
*
* @param [out]  pCnt
*      A pointer holding the error count for the RTDEx Rx channel uNumChannel.
*
* @return
*      Success or error code.
*
****************************************************************************/                                 
adp_result_t RTDExGetRxErrorCnt( connection_state *state,
                               uint32_t u32BaseCustomRegister,
                               unsigned int uNumChannel,
                               unsigned int* pCnt );
        
/**
*****************************************************************************
* Function: RTDExEnableAdcDacMode
*****************************************************************************
*
* Enables the ADC DAC emulation mode of the RTDEx Test module
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @return
*      Success or error code.
*
****************************************************************************/   
adp_result_t RTDExEnableAdcDacMode( connection_state *state, uint32_t u32BaseCustomRegister );

/**
*****************************************************************************
* Function: RTDExDisableAdcDacMode
*****************************************************************************
*
* Disables the ADC DAC emulation mode of the RTDEx Test module
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @return
*      Success or error code.
*
****************************************************************************/   
adp_result_t RTDExDisableAdcDacMode( connection_state *state, uint32_t u32BaseCustomRegister );

/**
*****************************************************************************
* Function: RTDExSetTestCntTxDiv
*****************************************************************************
*
* Set the test counter divider, in Tx.
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uValue
*      Value to program in the test cnt div
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExSetTestCntTxDiv( connection_state *state, uint32_t u32BaseCustomRegister, unsigned int uValue);
                                 
/**
*****************************************************************************
* Function: RTDExSetTestCntRxDiv
*****************************************************************************
*
* Set the test counter divider.
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uValue
*      Value to program in the test cnt div
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExSetTestCntRxDiv( connection_state *state, uint32_t u32BaseCustomRegister,unsigned int uValue);

/**
*****************************************************************************
* Function: RTDExGetUplinkOverFlow
*****************************************************************************
*
* Get the number of uplink overflow events
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uChannlNo
*      RTDEx Test core channel number
*
* @param [out]  pCnt
*      Pointer to the variable holding the number of overflow events
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExGetUplinkOverFlow( connection_state *state, uint32_t u32BaseCustomRegister,
                              unsigned int uChannlNo, unsigned int* pCnt );
                              
/**
*****************************************************************************
* Function: RTDExGetDownlinkUnderFlow
*****************************************************************************
*
* Get the number of downlink underflow events
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uChannlNo
*      RTDEx Test core channel number
*
* @param [out]  pCnt
*      Pointer to the variable holding the number of underflow events
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExGetDownlinkUnderFlow( connection_state *state, uint32_t u32BaseCustomRegister,
                              unsigned int uChannlNo, unsigned int* pCnt );
                              



/**
*****************************************************************************
* Function: RTDExGetDownlinkErrorCnt
*****************************************************************************
*
* Returns the error count of specific RTDEx Rx channels.
*
* @param [in]  hBusAccess
*      Handle to a bus media access (for control purpose).
*
* @param [in]  u32BaseCustomRegister
*      The id of the first custom register used to control the RTDEx test core
*
* @param [in]  uChannlNo
*      RTDEx Test core channel number
*
* @param [in]  pCnt
*      Pointer to the variable holding the number of downlink errors
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RTDExGetDownlinkErrorCnt( connection_state *state, 
                                      uint32_t u32BaseCustomRegister,
                                      unsigned int uChannlNo, 
                                      unsigned int* pCnt );

adp_result_t RTDExGetDownlinkReceivedSamples( connection_state *state, 
                                      uint32_t u32BaseCustomRegister,
                                      unsigned int uChannlNo, 
                                      unsigned int* pCnt );

adp_result_t ParseFileNames(char fileNames[], char seperatedFileNames[MAX_RTDEX][60], const char cFileNameDelim);
#endif
