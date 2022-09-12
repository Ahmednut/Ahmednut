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
 * @file       MI125_mi125_defines.h
 * @brief      mi125 module related data and structure definitions.
 *
 * $Date: 2015/06/03 18:26:17 $
 * $Revision: 1.5 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MI125_MI125
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

#ifndef __MI125_MI125_DEFINES__
#define __MI125_MI125_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "carrier_service_def.h"
#include "MI125_pcf8574_defines.h"
#include "MI125_ltm9012_defines.h"
#include "MI125_lm75_defines.h"
#include "MI125_mi125_regs.h"
#include "MI125_pca9535_defines.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define MI125_MAX_CALIB_ATTEMPT 20 ///< How many time it will attempt to calib all channel lanes when failure 


/** @anchor MI125_board_i2c_addresses
 *  @name MI125_board_i2c_addresses
 *    
 *  Board i2c devices addresses.
 * 
 *  Possible board i2c devices addresses according to GA address used for the board.   
 */
//@{ 
#define MI125_I2CADR_MAIN_GA00 {0x54,0x2C,0x4C,0x24}  ///< Perseus default (GA00) main board (fmc bottom)
#define MI125_I2CADR_SEC_GA00 {0x50,0x28,0x48,0x20}   ///< Perseus default (GA00) secondary board (fmc top)
#define MI125_I2CADR_MAIN_GA01 {0x55,0x2D,0x4D,0x25}  ///< GA01 main board (fmc bottom)  
#define MI125_I2CADR_SEC_GA01 {0x51,0x29,0x49,0x21}   ///< GA01 secondary board (fmc top)
#define MI125_I2CADR_MAIN_GA10 {0x56,0x2E,0x4E,0x26}  ///< GA10 main board (fmc bottom)  
#define MI125_I2CADR_SEC_GA10 {0x52,0x2A,0x4A,0x22}   ///< GA10 secondary board (fmc top)
#define MI125_I2CADR_MAIN_GA11 {0x57,0x2F,0x4F,0x27}  ///< GA11 main board (fmc bottom)  
#define MI125_I2CADR_SEC_GA11 {0x53,0x2B,0x4B,0x23}   ///< GA11 secondary board (fmc top)
//@}

// Do not change following value
#define MI125_I2SCAN_MAX_DEV 128                      ///< Max i2c devices scanned


/** @name MI125 fmc positions
 * 
 *  Possible mi125 fmc index position definition (bottom/top).   
 */
typedef enum MI125_E_mi125_fmc_position {
    MI125_BOTTOM=0,                  ///< Index of bottom FMC board
    MI125_TOP=1                      ///< Index of top FMC board
}MI125_mi125_fmc_position;

/** @name MI125 clock source
 * 
 *  Possible clock source inputs.   
 */
typedef enum MI125_E_mi125_clock_source {
    MI125_CLKSRC125MHZ=0x0,     ///< Internal 125 MHz (default)
    MI125_CLKSRCEXT=0x1,        ///< External clock
    MI125_CLKSRCBOTTOMFMC=0x2,  ///< This setting must not be used for bottom FMC (main) card, clock top board from bottom board
    MI125_CLKSRCFMCCARRIER=0x4  ///< FMC carrier clock (future)
}MI125_mi125_clock_source;

/** @name MI125 Core reference clock io delay
 * 
 *  Possible mi125 core iodelay reference clock.   
 */
typedef enum MI125_E_mi125_IoDelayRef {
    MI125_REF300MHZ=52,         ///< 300 MHz iodelay reference clock
    MI125_REF200MHZ=78          ///< 200 MHz iodelay reference clock (default on perseus 601x)
}MI125_mi125_IoDelayRef;


/** @name MI125 trigger out configuration
 * 
 *  Trigger out configuration (enable/disable).   
 */
typedef enum MI125_E_mi125_trigout {
    MI125_TRIGOUTON=0x0,        ///< Trigger out is connected from fpga to outside
    MI125_TRIGOUTOFF=0x1        ///< Trigger out is not connected from fpga to outside (default)
}MI125_mi125_trigout;

/** @name MI125 temperature reading mode
 * 
 *  Temperature reading mode (resolution configuration).   
 */
typedef enum MI125_E_mi125_tempmode {
    MI125_TEMP1C,               ///< 1C resolution mode
    MI125_TEMP0DOT1C            ///< 0.1C resolution mode (0.5C)
}MI125_mi125_tempmode;

/** @name MI125 ADC id
 * 
 *  ADC identifications when using adc check.   
 */
typedef enum MI125_E_mi125_adcid {
    MI125_ADC0 = 0,             ///< 1st adc (channel 1 to 4)
    MI125_ADC1 = 1,             ///< 2nd adc (channel 5 to 8)
    MI125_ADC2 = 2,             ///< 3rd adc (channel 9 to 12)
    MI125_ADC3 = 3,             ///< 4th adc (channel 13 to 16)
} MI125_mi125_adcid;

/** @name MI125 active channel configuration
 * 
 *  Configuration for the number of ADC active channels (not in power down).   
 */
typedef enum MI125_E_mi125_channel_mode {
    MI125_04CHANNELS=0,         ///< 04 channels active
    MI125_08CHANNELS=1,         ///< 08 channels active
    MI125_12CHANNELS=2,         ///< 12 channels active
    MI125_16CHANNELS=3          ///< 16 channels active (all channels) (default)
}MI125_mi125_channel_mode;

/** @name MI125 ADC randomize output mode
 * 
 *  Configuration for ADC randomize output mode (to lower channels interactions).   
 */
typedef enum MI125_E_mi125_randmode {
    MI125_RANDOMIZEOFF=0,       ///< Randomize mode is not active (default)
    MI125_RANDOMIZEON=1,        ///< Randomize mode is active
}MI125_mi125_randmode;

/** @name MI125 ADC data binary output format
 * 
 *  Configuration for ADC data binary output format.   
 */
typedef enum MI125_E_mi125_binmode {
    MI125_OFFSETBINARYFORMAT=0, ///< Offset binary format
    MI125_TWOCOMPLEMENTFORMAT=1,///< 2 complement format (default)
}MI125_mi125_binmode;

/** @name MI125 ADC lvds output configuration
 * 
 *  Configuration for ADC lvds lanes driving parameters.   
 */
typedef enum MI125_E_mi125_lvds {
    MI125_TERMOFF1750UA,        ///< No internal termination, 1.75mA drive current
    MI125_TERMOFF2100UA,        ///< No internal termination, 2.1mA drive current
    MI125_TERMOFF2500UA,        ///< No internal termination, 2.5mA drive current
    MI125_TERMOFF3000UA,        ///< No internal termination, 3.0mA drive current
    MI125_TERMOFF3500UA,        ///< No internal termination, 3.5mA drive current
    MI125_TERMOFF4000UA,        ///< No internal termination, 4.0mA drive current
    MI125_TERMOFF4500UA,        ///< No internal termination, 4.5mA drive current
    MI125_TERMON1750UA,         ///< With internal termination, 1.75mA drive current (default)
    MI125_TERMON2100UA,         ///< With internal termination, 2.1mA drive current
    MI125_TERMON2500UA          ///< With internal termination, 2.5mA drive current
}MI125_mi125_lvds;

/** @name MI125 test mode configuration
 * 
 *  Configuration for ADC test modes.   
 */
typedef enum MI125_E_mi125_testmode {
    MI125_TESTMODEOFF=0,        ///< All tests mode inactive (default)
    MI125_TESTMODE1=1,          ///< Production test mode active (user could get unpredictable results)
    MI125_TESTMODE2=2           ///< User pattern test mode active (could be used to check all data capture chain)
} MI125_mi125_testmode;

/** @name MI125 channel configuration
 * 
 *  Configuration for different channel parameters.   
 */
typedef struct MI125_S_mi125_config {
    MI125_mi125_channel_mode groupch;   ///< Which channels are active
    MI125_mi125_lvds lvds;              ///< Adc lanes lvds parameters
    MI125_mi125_randmode randmode;      ///< ADC output is randomize or not
    MI125_mi125_binmode binmode;        ///< ADC data format used (2compl or binary offset)
} MI125_mi125_config;

/** @name MI125 lvds lanes offset calibration
 * 
 *  Configuration for different fpga tap delays needed for calibration according
 *  to board routing information (top board has calibration also related to bottom board used).   
 */
typedef struct MI125_S_mi125_laneoffset {
    char laneoffset[32];           ///< Lanes 00-07 calibration delays
} MI125_mi125_laneoffset;

/** @name MI125 I2C devices address structure
 * 
 *  Hold i2c devices address when opening mi125 instance.
 */
typedef struct MI125_S_mi125_i2caddr {
    unsigned char eeprom;               ///< Eeprom i2c device address
    unsigned char spibridge;            ///< I2c to spi bridge device address 
    unsigned char sensor;               ///< Temperature sensor i2c address 
    unsigned char expander;             ///< I/O expander device address 
} MI125_mi125_i2caddr;


/** @name MI125 I2C devices scan structure
 * 
 *  Hold i2c devices scan results.
 */
typedef struct MI125_S_mi125_i2cscan {
    unsigned char devdetect[MI125_I2SCAN_MAX_DEV];          ///< i2c detected devices from scan
} MI125_mi125_i2cscan;


/** @name MI125 Board instance structure
 * 
 *  Hold all information regarding mi125 board instance used.
 */
typedef struct MI125_S_mi125 {
    volatile MI125_mi125_Register_Map * core;               ///< FPGA mi125 core registers
    MI125_mi125_channel_mode groupch;                       ///< Which channels are active 
    MI125_com_core com;                                     ///< I2c driver information
    MI125_pcf8574_core expander;                            ///< I/O expander information (alpha)
    MI125_pca9535_core expander2;                           ///< I/O expander2 information (beta and later)
    MI125_ltm9012_core adc;                                 ///< ADC related driver information
    MI125_lm75_core monitor;                                ///< Temperature related driver information
    Carrier_handle_t hCarrier;                              ///< Carrier handle to access the carrier specific services
    Carrier_eFmcConnectorNum_t FmcConnector;                ///< FMC connector related to this MI125 instance
    int resetdone;                                          ///< Module reset correctly or not
    unsigned int channelslanecalib[MI125_MAX_CALIB_ATTEMPT];///< Holds calibration information each trial
    unsigned int calibndx;                                  ///< Index in the calibration vector (previous)
    int calibdone;                                          ///< Calibration was done correctly or not
    int testmode;                                           ///< Information when a module test mode is configured
    MI125_mi125_fmc_position fmcposition;                   ///< FMC position of the module
    int resetinprog;                                        ///< Means inside the reset process
    unsigned int channellanemask;                           ///< Lanes calibration mask according to active channels
    int randommode;                                         ///< Save the ADC current randome mode
    MI125_mi125_laneoffset lanedelays;                      ///< Holds current lane tap delays offset (used for calibration)
    int beta;                                               ///< Configures if the board is used as a beta or later board
    unsigned short *pausLaneDelays;                         ///< Pointer on lanes delay information for the board (from board lanes to fpga) 
    MI125_mi125_IoDelayRef Reference;                       ///< Core Io delay reference clock used
    int defaulttaps;                                        ///< when !=0 means using default tap delay calibration
    unsigned char ucDataRefLane;                            ///< Data lane used for reference in the data calibration process
    unsigned short usRefDelay;                              ///< Reference lane delay (in ps) of master
    unsigned short *pusBottomRefDelay;                      ///< Pointer on reference lane delay (in ps) of bottom board
    int iFirstTapOffsetFeed;                                ///< First delay tap offset in tap analysis
    unsigned int errorDelay[32];                            ///< Calibration last error delay information
    unsigned short usDcoDelay;                              ///< Dco lane delay (in ps)
    unsigned short *pusBottomDcoDelay;                      ///< Pointer on dco lane delay (in ps) of bottom board
    unsigned char ucDataRefIoDelay;                         ///< Reference data line core delay choosen value in taps
    unsigned char *pucBottomDataRefIoDelay;                 ///< Pointer on reference data line core delay choosen value in taps of bottom board 
    unsigned char ucDataIoDelTable[32];                     ///< Data lines core delay choosen value for each data lane
    unsigned char ucDataIoGroupCount[32];                   ///< Data lines stable eye pattern width count for each data lane
    unsigned char ucDataIoGroupEnd[32];                     ///< Data lines stable eye pattern group end position
    unsigned char ucDataSmallestPosDcoTol;                  ///< Data lane calibration smallest dco tap delay tolerance from positive side
    unsigned char ucDataSmallestNegDcoTol;                  ///< Data lane calibration smallest dco tap delay tolerance from negative side
    unsigned char ucDataSmallestDelay;                      ///< Data lane calibration smallest tap delay calibrated in all lanes
    unsigned char ucDataGreatestDelay;                      ///< Data lane calibration greatest tap delay calibrated in all lanes
    unsigned char ucBitslipRetry;                           ///< Number of retry to calibrate the data pattern with the bit slip
    unsigned char ucBitslipIndex;                           ///< The index of the bitslip used to calibrate the serdes
    unsigned char *pucBottomBitslipIndex;                   ///< Pointer on master bitslip index calibration result
} MI125_mi125;

/** @name MI125 Error codes
 * 
 *  List all runtime related error codes of a mi125 module.
 */
enum MI125_mi125_error {
    MI125_MI125OK     = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_MI125, 0),      ///< No error
    MI125_MI125NOCORE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 1),        ///< FPGA MI125 core not detected correctly 
    MI125_MI125IDELAYRDY  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 2),    ///< FPGA idelayrdy problem
    MI125_MI125TRAININGDONE  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 3), ///< FPGA trainingdone problem
    MI125_MI125NORESET  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 4),      ///< mi125_reset function has never been done and bad configuration is possible
    MI125_MI125INVALIDCLKBOT  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 5),///< invalid clk fmc bottom source for main fmc board
    MI125_MI125INVALIDCLK  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 6),   ///< invalid clk source
    MI125_MI125INVALIDTRIG = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 7),   ///< invalid trig out value
    MI125_MI125INVALIDCH = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 8),     ///< invalid active channel group configuration
    MI125_MI125INVALIDLVDS = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 9),   ///< invalid lvds output configuration
    MI125_MI125INVALIDRAN = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 10),   ///< invalid output randomize mode
    MI125_MI125INVALIDBIN = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 11),   ///< invalid output binary mode configuration
    MI125_MI125INVALIDTEST = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 12),  ///< invalid test mode provided
    MI125_MI125NOCALIB = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 13),      ///< last calibration was invalid
    MI125_MI125INVALIDADC = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 14),   ///< invalid adc parameter used
    MI125_MI125TESTMODEACT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 15),  ///< test mode is active (preventing this function)
    MI125_MI125BADUSECFCT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 16),   ///< invalid timer user function specified
    MI125_MI125ABSENT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 17),       ///< No FMC card is present
    MI125_MI125MMCMLOCK = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 18),     ///< No Adc clock mmcm lock 
    MI125_MI125INVALIDCLKTOP = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 19),///< invalid clk source for top secondary fmc board
    MI125_MI125BADBOTTAPREFPTR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 20),///< Invalid bottom board data reference tap delay pointer 
    MI125_MI125BADBOTTAPREFVALUE= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 21),  ///< Invalid bottom board data reference tap delay value  
    MI125_MI125BADBOTREFDELAYPTR= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 22),  ///< Invalid bottom board reference delay pointer
    MI125_MI125BADBOTDCODELAYPTR= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MI125, 23)   ///< Invalid bottom board dco delay pointer  
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif
