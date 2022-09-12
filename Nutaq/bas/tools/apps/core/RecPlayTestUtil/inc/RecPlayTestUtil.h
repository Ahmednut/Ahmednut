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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**  
 * 
 * @file    RecPlayTestUtil.h 
 * @brief   Functions handling the recording and playback test module in the FPGA.
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


// Important. Before using this function, make sure that the lyt_plb_recplay_test_cnt core
// is instantiated in the FPGA. 

 
 
#ifndef ADP_RECPLAY_TEST_CNT_H
#define ADP_RECPLAY_TEST_CNT_H

#ifdef __cplusplus
extern "C" {
#endif

#define RESET_INDEX  0 
#define RECPLAY_START_INDEX 1
#define SET_REC_TRIG_INDEX 2
#define SET_REC_PLAY_INDEX 3
#define TRIG_ADDRESS_INDEX 4
#define DIV_CNT_INDEX 5
#define MAX_DATA_INDEX 6
#define DATA_CNT_INDEX 7
#define ERROR_CNT_CH_INDEX 8

//Test mode/direction defines
#define PLAYBACK 1  //The RecPlay Test core receives and analyzes data. Data is generated on the host.
#define RECORD 2    //The RecPlay Test core generates and sends data. Data is analyzed on the host.

typedef struct
{
    uint32_t u32RecPlayTestBaseCustomRegister;
    char pcFileName[500];
    uint32_t u32Mode;
    uint32_t u32TestSize;
    uint32_t u32PlaybackTimeout;
    int bInitRecPlayTest;
    int bStartRecPlayTest;      
    int bStopRecPlayTest;
} sRecPlayTestParams;

typedef sRecPlayTestParams *handle_recplaytestparams;

Result RecPlayTestUtil(char *pcIpAddress, handle_recplaytestparams hParams, char acAction[]);

/**
*****************************************************************************
* @file RecPlayTestUtil.h
*****************************************************************************
* Functions to use the test counter with the record and playback interface
****************************************************************************/

/*==========================================================================
==  PUBLIC ENUMERATIONS
==========================================================================*/

/** @anchor RECPLAYTEST_trigposition_max
 *  @name RECPLAYTEST_trigposition_max
 *    
 *  Trigger position maximum value.
 * 
 *  This gives the maximum position of the trigger position in the test engine.   
 */
//@{ 
#define RECPLAYTEST_MAXTRIGPOSITION  0x3FFFFFFF    ///< Maximum trigger position value (28 bits)
//@}


/**
*****************************************************************************
* Function: RecPlayTestReset
*****************************************************************************
*
* Resets the test logic in the Record/Playback Test core.
*
*
* @param [in]  state 
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlayTestReset( connection_state * state,
                               uint32_t u32BaseCustomRegisterId );

                               
                               
/**
*****************************************************************************
* Function: RecPlayTestStart
*****************************************************************************
*
* Starts the test in the Record/Playback Test core.
*
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @return
*      Success or error code.
*
****************************************************************************/
adp_result_t RecPlayTestStart( connection_state * state,
                               uint32_t u32BaseCustomRegisterId);

                               
/**
*****************************************************************************
* Function: RecPlayTestStop
*****************************************************************************
*
* Stops the test in the Record/Playback Test core.
*
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @return
*      Success or error code.
*
****************************************************************************/                               
adp_result_t RecPlayTestStop(  connection_state * state,
                               uint32_t u32BaseCustomRegisterId );

                              
/**
*****************************************************************************
* Function: RecPlaySetRecTrig
*****************************************************************************
*
* Generates a trigger signal forwarded to a specific address in the
* recording module of the recording and playback core.
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @param [in]  uTrigAddr
*      Address, in samples, where the trigger occurs.
*
* @return
*      Success or error code.
*
****************************************************************************/                               
adp_result_t RecPlaySetRecTrig( connection_state * state,
                                uint32_t u32BaseCustomRegisterId,
                                unsigned int uTrigAddr );

                                
/**
*****************************************************************************
* Function: RecPlaySetPlayTrig
*****************************************************************************
*
* Generates a trigger signal forwarded to the
* playback module in the recording and playback core.
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @return
*      Success or error code.
*
****************************************************************************/                                        
adp_result_t RecPlaySetPlayTrig(  connection_state * state,
                                  uint32_t u32BaseCustomRegisterId );

/**
*****************************************************************************
* Function: RecPlayGetNbErrors
*****************************************************************************
*
* Returns the error count of a specific playback port. The function RecPlayGetTestDataCnt
* must be used with this one before knowing if there was errors.
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @param [in]  uNumPort
*      Playback port number.
*
* @param [out]  pNbErrors
*      Pointer of the error count for the uNumPort playback port.
*
* @return
*      Success or error code.
*
****************************************************************************/                                     
adp_result_t RecPlayGetNbErrors( connection_state * state,
                                 uint32_t u32BaseCustomRegisterId,
                                 unsigned int uNumPort,
                                 unsigned int* pNbErrors );

/**
*****************************************************************************
* Function: RecPlaySetTestCntDiv
*****************************************************************************
*
* Configures the division factor to test downsampling.
* Example: Configure to 10 to downsample 10 times. Configure to 0 for full rate.
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @param [in]  uValue
*      The divider value (ie. the downsampling factor).
*
* @return
*      Success or error code.
*
****************************************************************************/                                        
adp_result_t RecPlaySetTestCntDiv(  connection_state * state,
                               uint32_t u32BaseCustomRegisterId, 
                                 unsigned int uValue );

/**
*****************************************************************************
* Function: RecPlaySetTestMaxData
*****************************************************************************
*
* Configures the maximum amount of expected data. This value is used to
* test continuous playback by forcing the expected data ramp
* to return to zero after reaching a maximum value. When testing
* single-shot playback, this value can be configured at its default value of
* 0xFFFFFFFF.
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @param [in]  uValue
*      The maximum expected data value.
*
* @return
*      Success or error code.
*
****************************************************************************/                                        
adp_result_t RecPlaySetTestMaxData(  connection_state * state,
                               uint32_t u32BaseCustomRegisterId, 
                                 unsigned int uValue );


/**
*****************************************************************************
* Function: RecPlayGetTestDataCnt
*****************************************************************************
*
* Read the number of elements that were verified by the test engine.
* This function must be used with RecPlayGetNbErrors to see if something
* was verified before knowing if there was errors in it.
*
* @param [in]  state
*      Handle to the connection
*
* @param [in]  u64BaseAddress
*      Base address in the FPGA of the Record/Playback Test core.
*
* @param [out] uValue
*      The returned data count for the number of verified values.
*
* @return
*      Success or error code.
*
****************************************************************************/                                        
adp_result_t RecPlayGetTestDataCnt( connection_state * state, 
                                    uint32_t u32BaseCustomRegisterId , 
                                    unsigned int *uValue );

                                    
/** @name Record/Playback test module error codes
 * 
 *  List all runtime related error codes of a Record/Playback test module.
 */
enum RECPLAYTEST_error {
    RECPLAYTESTOK = 0,  /**<No error*/
	RECPLAYTESTINVTRIGGER = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAYTEST, 1),     ///< Record/Playback test trigger position is invalid
	};

#ifdef __cplusplus
}
#endif


#endif
