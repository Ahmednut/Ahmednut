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
 * @file       Ad9361_defines.h
 * @brief      Ad9361 module related structure and data definitions
 *
 * $Date: 2016/04/22 20:03:09 $
 * $Revision: 1.7 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup Ad9361
 *
 * This file is part of Nutaq’s ADP Software Suite.
 *
 * You may at your option receive a license to Nutaq’s ADP Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s ADP Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s ADP Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s ADP Software Suite under the GPL requires that your work based on
 * Nutaq’s ADP Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s ADP Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s ADP Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/

#ifndef AD9361_DEFINES_H__
#define AD9361_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_types.h"
#include "bas_error.h"


/******************************************************************************/
/*************************** Types Declarations *******************************/
/******************************************************************************/

typedef struct Ad9361_stRfRssi {
    uint32_t ant;        /* Antenna number for which RSSI is reported */
    uint32_t symbol;        /* Runtime RSSI */
    uint32_t preamble;        /* Initial RSSI */
    int32_t multiplier;    /* Multiplier to convert reported RSSI */
    uint8_t duration;        /* Duration to be considered for measuring */
} Ad9361_stRfRssi;

// index:   0:RX, 1:TX
typedef struct Ad9361_stCalibResult
{
    char cSucceeded;
    char cBitSlip[2];
    char cClkDelay[2];
    char cDataDelay[2];
    char cDataDelayWidth[2];
} Ad9361_stCalibResult;

typedef enum Ad9361_eEnsmMode {
	eAd9361EnsmModeTx,
	eAd9361EnsmModeRx,
	eAd9361EnsmModeAlert,
	eAd9361EnsmModeFdd,
	eAd9361EnsmModeWait,
	eAd9361EnsmModeSleep,
	eAd9361EnsmModePinCtrl,
	eAd9361EnsmModeFddIndep,
} Ad9361_eEnsmMode;

static const string_enum_element_t AD9361_ENSM_MODE_STRING_TABLE[] = {
    {"eAd9361EnsmModeTx",   eAd9361EnsmModeTx},
    {"eAd9361EnsmModeRx",       eAd9361EnsmModeRx},
    {"eAd9361EnsmModeAlert",          eAd9361EnsmModeAlert},
    {"eAd9361EnsmModeFdd",     eAd9361EnsmModeFdd},
    {"eAd9361EnsmModeWait",          eAd9361EnsmModeWait},
    {"eAd9361EnsmModeSleep",     eAd9361EnsmModeSleep},
    {"eAd9361EnsmModePinCtrl",         eAd9361EnsmModePinCtrl},
    {"eAd9361EnsmModeFddIndep",    eAd9361EnsmModeFddIndep}
};

typedef enum Ad9361_eEnsmState
{
    eAd9361EnsmStateSleepWait   = 0x0,
    eAd9361EnsmStateAlert       = 0x5,
    eAd9361EnsmStateTx          = 0x6,
    eAd9361EnsmStateTxFlush     = 0x7,
    eAd9361EnsmStateRx          = 0x8,
    eAd9361EnsmStateRxFlush     = 0x9,
    eAd9361EnsmStateFdd         = 0xA,
    eAd9361EnsmStateFddFlush    = 0xB,
    eAd9361EnsmStateInvalid     = 0xFF,
    eAd9361EnsmStateSleep       = 0x80
} Ad9361_eEnsmState;

static const string_enum_element_t AD9361_ENSM_STATE_STRING_TABLE[] = {
    {"eAd9361EnsmStateSleepWait",   eAd9361EnsmStateSleepWait},
    {"eAd9361EnsmStateAlert",       eAd9361EnsmStateAlert},
    {"eAd9361EnsmStateTx",          eAd9361EnsmStateTx},
    {"eAd9361EnsmStateTxFlush",     eAd9361EnsmStateTxFlush},
    {"eAd9361EnsmStateRx",          eAd9361EnsmStateRx},
    {"eAd9361EnsmStateRxFlush",     eAd9361EnsmStateRxFlush},
    {"eAd9361EnsmStateFdd",         eAd9361EnsmStateFdd},
    {"eAd9361EnsmStateFddFlush",    eAd9361EnsmStateFddFlush},
    {"eAd9361EnsmStateInvalid",     eAd9361EnsmStateInvalid},
    {"eAd9361EnsmStateSleep",       eAd9361EnsmStateSleep}
};



/** @name AD9361 Error codes
 *
 *  List all runtime related error codes of a AD9361 module.
 */
enum Ad9361_eError {
    eAd9361Ok                           = MAKE_RESULT(eBasSeveritySuccess,  MODULE_AD9361, 0),    ///< No error
    eAd9361NoCore                       = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 1),    ///< No matching FPGA core
    eAd9361IoError                      = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 2),    ///< I/O error
    eAd9361CannotReadGain               = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 3),    ///< Cannot read the gain
    eAd9361OutOfMem                     = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 4),    ///< Out of memory
    eAd9361BadAddr                      = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 5),    ///< Bad address
    eAd9361NoDev                        = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 6),    ///< Device not present
    eAd9361InvArg                       = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 7),    ///< Invalid argument
    eAd9361CalibTimeout                 = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 8),    ///< Calibration has timed out
    eAd9361SpiError                     = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 9),    ///< SPI error
    eAd9361MasterCalib                  = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 10),   ///< Calibration from the master module is not done
    eAd9361InvHandle                    = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 11),   ///< Invalid handle
    eAd9361ComError                     = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 12),   ///< Cannot communicate with a AD9361 chip
    eAd9361InvChipID                    = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 13),   ///< Invalid AD9361 chip ID
    eAd9361InvEnum                      = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 14),   ///< Invalid enumeration
    eAd9361ArgOutOfRange                = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 15),   ///< Argument value is out of range
    eAd9361InvChannel                   = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 16),   ///< Invalid channel number
    eAd9361ChannelNotEnable             = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 17),   ///< The channel is not enabled
    eAd9361InvFddEnsmState              = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 18),   ///< The ENSM state is not valid in FDD mode
    eAd9361InvTddEnsmState              = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 19),   ///< The ENSM state is not valid in TDD mode
    eAd9361CurrentEnsmStateMustBeAlert  = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 20),   ///< The current ENSM state must be ALERT before changing to the desired state
    eAd9361InvClkRate                   = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 21),   ///< At least one of the clock rates must equal the serial clock rate.
    eAd9361InvSamplingRate              = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 22),   ///< The desired sampling rate exceeds the allowed range
    eAd9361CannotFindPllSolution        = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 23),   ///< Cannot find a value solution for the PLL
    eAd9361FastLockProfileNotInit       = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 24),   ///< The fast lock profile has not been initialized
    eAd9361InvRefClkFreq                = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 25),   ///< Invalid reference clock frequency
    eAd9361CalibNotSupported            = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 26),   ///< The specified calibration is not supported
    eAd9361FirCoefLoadError             = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 27),   ///< The FIR coefficients were not loaded correctly
    eAd9361InvFirParam                  = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 28),   ///< Invalid FIR parameters
    eAd9361InvFirInterpDecim            = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 29),   ///< Invalid FIR interpolation or decimation value
    eAd9361InvFirTxNumCoef              = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 30),   ///< The maximal allowed number of coefficient is 64 in TX with an interpolation of 1
    eAd9361InvFirNumCoef                = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 31),   ///< The number of coefficient is higher than the maximal allowed for the current ADC or DAC clock rate
    eAd9361InvClockConfig               = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 32),   ///< Invalid clock configuration
    eAd9361InvDirection                 = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 33),   ///< Invalid direction
    eAd9361DigitalCalibNoSolution       = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 34),   ///< No solution could be found for the digital calibration
    eAd9361BbPllUnlocked                = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 35),   ///< BBPLL unlocked. Make sure the input reference clock and specified frequencies are valid.
    eAd9361TxRfPllUnlocked              = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 36),   ///< TXRFPLL unlocked. Make sure the input reference clock and specified frequencies are valid.
    eAd9361RxRfPllUnlocked              = MAKE_RESULT(eBasSeverityError,    MODULE_AD9361, 37),   ///< RXRFPLL unlocked. Make sure the input reference clock and specified frequencies are valid.
};

#ifdef __cplusplus
}
#endif  // extern "C"
#endif
