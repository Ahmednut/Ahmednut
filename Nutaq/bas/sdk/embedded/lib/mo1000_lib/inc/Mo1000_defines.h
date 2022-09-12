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
 * @file       Mo1000_defines.h
 * @brief      mo1000 module related structure and data definitions
 *
 * $Date: 2015/10/16 14:31:21 $
 * $Revision: 1.3 $
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

#ifndef MO1000_DEFINES_H__
#define MO1000_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "carrier_service_def.h"
#include "Expander_defines.h"
#include "Ad9148_defines.h"
#include "Lm75_defines.h"
#include "Cdce62005_defines.h"
#include "Mo1000_regs.h"
#include "Mo1000DevCom_defines.h"
#include "DevCom_defines.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define MO1000_NBMAX 2                   ///< Maximum MO1000 boards we can find on a perseus 
#define MO1000_MAX_CALIB_ATTEMPT 16      ///< How many time it will attempt to calib all channel lanes when failure (must be a multiple of 16)
#define MO1000_MAX_FRAME_CALIB_ATTEMPT 3 ///< How many time it will attempt to frame calib when failure 

#define MO1000_MAX_SYNC_CALIB_ATTEMPT 1  ///< How many time it will attempt to sync calib when failure 


#define MO1000_DAC_PER_BOARD 2           ///< Number of dacs per board

#define MO1000_DAC_FIFO_SIZE 8           ///< Size of dac fifos

#define MO1000_DELAY_TAPS 32             ///< Number of core possible delay taps for data and frame lines

#define MO1000_FRAME_MODES 4             ///< Number of frame mode possible


/** @anchor MO1000_board_i2c_addresses
 *  @name MO1000_board_i2c_addresses
 *    
 *  Board i2c devices addresses.
 * 
 *  Possible board i2c devices addresses according to GA address used for the board.   
 */
//@{ 
#define MO1000_I2CADR_MAIN_GA00 {0x54,0x2C,0x4C,0x24}  ///< Perseus default (GA0=0 GA1=0) main board (fmc bottom)
#define MO1000_I2CADR_SEC_GA00 {0x50,0x28,0x48,0x20}   ///< Perseus default (GA0=0 GA1=0) secondary board (fmc top)
#define MO1000_I2CADR_MAIN_GA01 {0x55,0x2D,0x4D,0x25}  ///< GA0=0 GA1=1 main board (fmc bottom)  
#define MO1000_I2CADR_SEC_GA01 {0x51,0x29,0x49,0x21}   ///< GA0=0 GA1=1 secondary board (fmc top)
#define MO1000_I2CADR_MAIN_GA10 {0x56,0x2E,0x4E,0x26}  ///< GA0=1 GA1=0 main board (fmc bottom)  
#define MO1000_I2CADR_SEC_GA10 {0x52,0x2A,0x4A,0x22}   ///< GA0=1 GA1=0 secondary board (fmc top)
#define MO1000_I2CADR_MAIN_GA11 {0x57,0x2F,0x4F,0x27}  ///< GA0=1 GA1=1 main board (fmc bottom)  
#define MO1000_I2CADR_SEC_GA11 {0x53,0x2B,0x4B,0x23}   ///< GA0=1 GA1=1 secondary board (fmc top)
//@}


// Do not change following value
#define MO1000_I2SCAN_MAX_DEV 128                      ///< Max i2c devices scanned


/** @name MO1000 fmc positions
 * 
 *  Possible mo1000 fmc index position definition (bottom/top).   
 */
typedef enum Mo1000_EnumFmcPosition {
    eMo1000PositBottom=0,            ///< Index of bottom FMC board
    eMo1000PositTop=1                ///< Index of top FMC board
}Mo1000_eFmcPosition;


/** @name MO1000 Core reference clock io delay
 * 
 *  Possible mo1000 core iodelay reference clock.   
 */
typedef enum Mo1000_EnumIoDelayRef {
    eMo1000Ref300Mhz=52,             ///< 300 MHz iodelay reference clock
    eMo1000Ref200Mhz=78              ///< 200 MHz iodelay reference clock (default on perseus 601x)
}Mo1000_eIoDelayRef;


/** @name MO1000 test mode configuration
 * 
 *  Configuration for DAC test modes.   
 */
typedef enum Mo1000_EnumTestMode {
    eMo1000TestModeoff=0,            ///< All tests mode inactive (default)
    eMo1000TestMode1=1,              ///< Production test mode active (user could get unpredictable results)
    eMo1000TestMode2=2               ///< User pattern test mode active (could be used to check all fpga data generated chain)
}Mo1000_eTestMode;


/** @name Mo1000 Master Clock mode
 * 
 *  Possible mo1000 master clock generation mode.   
 */
typedef enum Mo1000_EnumMasterClkMode {
    eMo1000MasterClkAuto=0,          ///< Master clock automatically generated (for a double mo1000 stack)
    eMo1000MasterClkManual=1         ///< Master clock manually specified (example for a stack with a mi125)
}Mo1000_eMasterClkMode;


/** @name MO1000 possible hardware revisions
 * 
 *  Possible mo1000 hardware revision (starting with alpha boards).
 */
typedef enum Mo1000_EnumHardwareRev {
    eMo1000RevB=0,                   ///< Alpha revision
    eMo1000RevC=1,                   ///< Beta revision
    eMo1000RevLast=eMo1000RevC       ///< Last known revision
}Mo1000_eHardwareRev;


/** @name MO1000 logical devices
 * 
 *  List of I2C logical devices.
 */
typedef enum Mo1000_EnumDevices {
    eMo1000DeviceDac1=0,             ///< Dac1 device
    eMo1000DeviceDac2=1,             ///< Dac2 device
    eMo1000DevicePll=2,              ///< Pll device
    eMo1000DevicePorts=5,            ///< IO ports device
    eMo1000DeviceCore=10             ///< FPGA Core device
}Mo1000_eDevices;


/** @name MO1000 clock source
 * 
 *  Possible clock source inputs.   
 */
typedef enum Mo1000_EnumClockSource {
    eMo1000ClkSrc125mhz=0x1,    ///< Internal 125 MHz (default)
    eMo1000ClkSrcExt=0x3,       ///< External clock
    eMo1000ClkSrcBottomfmc=0x2, ///< This setting must not be used for bottom FMC (main) card, clock top board from bottom board (mandatory for clock slave)
    eMo1000ClkSrcFmccarrier2=0x0,///< FMC carrier CLK2 clock
    eMo1000ClkSrcFmccarrier3=0x4///< FMC carrier CLK3 clock
}Mo1000_eClockSource;


/** @name MO1000 temperature reading mode
 * 
 *  Temperature reading mode (resolution configuration).   
 */
typedef enum Mo1000_EnumTempMode {
    eMo1000Temp1c,               ///< 1C resolution mode
    eMo1000Temp0dot1c            ///< 0.1C resolution mode (0.5C)
}Mo1000_eTempMode;


/** @name MO1000 DAC channels group id
 * 
 *  DAC channels group identification.   
 */
typedef enum Mo1000_EnumDacGroupId {
    eMo1000DacGroup14 = 0,       ///< 1st dac channels group (channel 1 to 4)
    eMo1000DacGroup58 = 1        ///< 2nd dac channels group (channel 5 to 8)
}Mo1000_eDacGroupId;


/** @name Mo1000 Data format
 * 
 *  Possible dac data format.   
 */
typedef enum Mo1000_EnumDataFormat {
    eMo1000Format2complement=0,          ///< 2 complement data format (default)
    eMo1000FormatBinary=1                ///< Unsigned binary data format
}Mo1000_eDataFormat;


/** @name Mo1000 Dac channel selection
 * 
 *  Possible dac to work with.   
 */
typedef enum Mo1000_EnumDacSelect {
    eMo1000SelectDac1=0,                 ///< Dac 1 selected (I)
    eMo1000SelectDac2=1,                 ///< Dac 2 selected (Q)
    eMo1000SelectDac3=2,                 ///< Dac 3 selected (I)
    eMo1000SelectDac4=3,                 ///< Dac 4 selected (Q)
    eMo1000SelectDac5=4,                 ///< Dac 5 selected (I)
    eMo1000SelectDac6=5,                 ///< Dac 6 selected (Q)
    eMo1000SelectDac7=6,                 ///< Dac 7 selected (I)
    eMo1000SelectDac8=7,                 ///< Dac 8 selected (Q)
}Mo1000_eDacSelect;             


/** @name Mo1000 Dac channels pair selection
 * 
 *  Possible IQ dac channels pair selection   
 */
typedef enum Mo1000_EnumSelectDacPair {
    eMo1000SelectIQDac12=0,              ///< Dac 1 & 2 selected (I+Q)
    eMo1000SelectIQDac34=1,              ///< Dac 3 & 4 selected (I+Q)
    eMo1000SelectIQDac56=2,              ///< Dac 5 & 6 selected (I+Q)
    eMo1000SelectIQDac78=3               ///< Dac 7 & 8 selected (I+Q)
}Mo1000_eSelectDacPair;


/** @name Mo1000 Fine modulation sideband
 * 
 *  Possible fine modulation nco sideband   
 */
typedef enum Mo1000_EnumNcoSideband {
    eMo1000NcoHighside=0,                ///< Nco high sideband
    eMo1000NcoLowside=1                  ///< Nco low sideband
}Mo1000_eNcoSideband;


/** @name Mo1000 Output control state
 * 
 *  Possible output control state.   
 */
typedef enum Mo1000_EnumOutputState {
    eMo1000OutputEnable=0,               ///< Dac output is enable
    eMo1000OutputDisable=1               ///< Dac output is disable
}Mo1000_eOutputState;


/** @name MO1000 active channels configuration
 * 
 *  Configuration for the number of DAC active channels (not in power down).   
 */
typedef enum Mo1000_EnumActChannel {
    eMo1000ActChannels02=0,     ///< 02 channels active
    eMo1000ActChannels04=1,     ///< 04 channels active
    eMo1000ActChannels06=2,     ///< 06 channels active
    eMo1000ActChannels08=3      ///< 08 channels active (all channels) (default)
}Mo1000_eActChannel;


/** @name Mo1000 isinc control state
 * 
 *  Possible isinc filter control state.   
 */
typedef enum Mo1000_EnumIsincState {
    eMo1000IsincEnable=0,            ///< Dac group isinc filter is enable
    eMo1000IsincDisable=1            ///< Dac group isinc filter is disable
}Mo1000_eIsincState;


/** @name Mo1000 fine modulation (NCO) control state
 * 
 *  Possible fine modulation control state.   
 */
typedef enum Mo1000_EnumFineModState {
    eMo1000FineModEnable=0,          ///< Dac group fine modulation (NCO) is enable
    eMo1000FineModDisable=1          ///< Dac group fine modulation (NCO) is disable
}Mo1000_eFineModState;


/** @name MO1000 board error and status information
 * 
 *  Get board status and error information.   
 */
typedef struct Mo1000_StructBoardStatus {
    union
    {
        struct
        {
#ifdef _BIG_ENDIAN 
            unsigned int 
            m_uHReserved01:1,
            m_uHReserved02:1,
            m_uDac1SyncLostErr:1,           ///< Dac 1 Synchronisation lost error when 1
            m_uHReserved03:1,
            m_uPllLock:1,                   ///< Clock PLL lock status (=1 when locked)
            m_uHReserved04:1,
            m_uDac1FifoWarn1Err:1,          ///< Dac 1 Fifo warning #1 error when 1
            m_uDac1FifoWarn2Err:1,          ///< Dac 1 Fifo warning #2 error when 1
            m_uReserved02:1,
            m_uReserved03:1,
            m_uReserved04:1,
            m_uHReserved05:1,
            m_uHReserved06:1,
            m_uDac1SedCompareErr:1,         ///< Dac 1 Test pattern compare error when 1
            m_uReserved05:1,
            m_uReserved06:1,
            m_uHReserved11:1,
            m_uHReserved12:1,
            m_uDac2SyncLostErr:1,           ///< Dac 2 Synchronisation lost error when 1
            m_uHReserved13:1,                                                           
            m_uReserved11:1, 
            m_uHReserved14:1,                                                           
            m_uDac2FifoWarn1Err:1,          ///< Dac 2 Fifo warning #1 error when 1     
            m_uDac2FifoWarn2Err:1,          ///< Dac 2 Fifo warning #2 error when 1     
            m_uReserved12:1,                                                            
            m_uReserved13:1,                                                            
            m_uReserved14:1,                                                            
            m_uHReserved15:1,                                                           
            m_uHReserved16:1,                                                           
            m_uDac2SedCompareErr:1,         ///< Dac 2 Test pattern compare error when 1
            m_uReserved15:1,
            m_uReserved16:1;
#else
            unsigned int 
            m_uReserved16:1,
            m_uReserved15:1,
            m_uDac2SedCompareErr:1,         ///< Dac 2 Test pattern compare error when 1
            m_uHReserved16:1,                                                           
            m_uHReserved15:1,                                                           
            m_uReserved14:1,                                                            
            m_uReserved13:1,                                                            
            m_uReserved12:1,                                                            
            m_uDac2FifoWarn2Err:1,          ///< Dac 2 Fifo warning #2 error when 1     
            m_uDac2FifoWarn1Err:1,          ///< Dac 2 Fifo warning #1 error when 1     
            m_uHReserved14:1,                                                           
            m_uReserved11:1, 
            m_uHReserved13:1,                                                           
            m_uDac2SyncLostErr:1,           ///< Dac 2 Synchronisation lost error when 1
            m_uHReserved12:1,
            m_uHReserved11:1,
            m_uReserved06:1,
            m_uReserved05:1,
            m_uDac1SedCompareErr:1,         ///< Dac 1 Test pattern compare error when 1
            m_uHReserved06:1,
            m_uHReserved05:1,
            m_uReserved04:1,
            m_uReserved03:1,
            m_uReserved02:1,
            m_uDac1FifoWarn2Err:1,          ///< Dac 1 Fifo warning #2 error when 1
            m_uDac1FifoWarn1Err:1,          ///< Dac 1 Fifo warning #1 error when 1
            m_uHReserved04:1,
            m_uPllLock:1,                   ///< Clock PLL lock status (=1 when locked)
            m_uHReserved03:1,
            m_uDac1SyncLostErr:1,           ///< Dac 1 Synchronisation lost error when 1
            m_uHReserved02:1,
            m_uHReserved01:1;
#endif
        }stBit;
        struct
        {
#ifdef _BIG_ENDIAN 
            unsigned int m_uEvent10:8, m_uEvent11:8, m_uEvent20:8, m_uEvent21:8;
#else
            unsigned int m_uEvent21:8, m_uEvent20:8, m_uEvent11:8, m_uEvent10:8;
#endif
        }stEventParts;
        unsigned int m_uRaw;
    };
}Mo1000_stBoardStatus;


/** @name MO1000 compare error information
 * 
 *  Test pattern compare error information.   
 */
typedef struct Mo1000_StructCompareErrors {
    union
    {
        struct
        {
#ifdef _BIG_ENDIAN 
            unsigned int m_uDac1CompareErr:16, m_uDac2CompareErr:16;
#else
            unsigned int m_uDac2CompareErr:16, m_uDac1CompareErr:16;
#endif
        }stBit;
        unsigned int m_uRaw;
    };
}Mo1000_stCompareErrors;


/** @name MO1000 I2C devices address structure
 * 
 *  Hold i2c devices address when opening mo1000 instance.
 */
typedef struct Mo1000_StructI2cAddr {
    unsigned char m_ucEeprom;               ///< Eeprom i2c device address
    unsigned char m_ucSpiBridge;            ///< I2c to spi bridge device address 
    unsigned char m_ucSensor;               ///< Temperature sensor i2c address 
    unsigned char m_ucExpander;             ///< I/O expander device address 
} Mo1000_stI2cAddr;


/** @name MO1000 I2C devices scan structure
 * 
 *  Hold i2c devices scan results.
 */
typedef struct Mo1000_StructI2cScan {
    unsigned char m_ucDevDetect[MO1000_I2SCAN_MAX_DEV];     ///< i2c detected devices from scan
} Mo1000_stI2cScan;


/** @name MO1000 Dac configuration structure
 * 
 *  Hold dac configuration structure.
 */
typedef struct Mo1000_StructDacConfig {
    Ad9148_eActPairState ActPairState[AD9148_CHANNELS_PER_DAC/2];          ///< Active pair state
    unsigned char ucActPairStateUpdate[AD9148_CHANNELS_PER_DAC/2];         ///< Active pair state updated flag
    Ad9148_stISincCoefficients stISincReal[AD9148_CHANNELS_PER_DAC/2];     ///< isinc real (I) coefficients
    Ad9148_stISincCoefficients stISincImaginary[AD9148_CHANNELS_PER_DAC/2];///< isinc imaginary (Q) coefficients
    unsigned char ucISincUpdate[AD9148_CHANNELS_PER_DAC/2];                ///< isinc parameters updated flag
    Ad9148_eISincControlState ISincState;                                  ///< isinc enable/disable state
    unsigned char ucISincStateUpdate;                                      ///< isinc control state updated flag
    Ad9148_eInterpolationMode InterMode;                                   ///< interpolation mode
    unsigned char ucInterUpdate;                                           ///< interpolation updated flag
    unsigned uFtw;                                                         ///< Fine modulation frequency tuning mode
    unsigned short usNcoPhaseOffset;                                       ///< Fine modulation nco phase offset
    Ad9148_eNcoSideband Sideband;                                          ///< Fine modulation sideband used
    unsigned char ucFineModUpdate;                                         ///< Fine modulation options updated flag
    Ad9148_eNcoControl FineModState;                                       ///< Fine modulation enable/disable state
    unsigned char ucFineModStateUpdate;                                    ///< Fine modulation state updated flag
    short sRealPhase[AD9148_CHANNELS_PER_DAC/2];                           ///< Quadrature phase I correction
    short sImaginaryPhase[AD9148_CHANNELS_PER_DAC/2];                      ///< Quadrature phase Q correction
    unsigned char ucQuadPhaseUpdate[AD9148_CHANNELS_PER_DAC/2];            ///< Quadrature phase updated flag
    Ad9148_eDataFormat DataFormat;                                         ///< Data format
    unsigned char ucDataFormatUpdate;                                      ///< Data format updated flag
    short sDcOffset[AD9148_CHANNELS_PER_DAC];                              ///< Dc offset
    unsigned char ucDcOffsetUpdate[AD9148_CHANNELS_PER_DAC];               ///< Dc offset updated flag
    unsigned char ucGain[AD9148_CHANNELS_PER_DAC];                         ///< Amplitude gain
    unsigned char ucGainUpdate[AD9148_CHANNELS_PER_DAC];                   ///< Amplitude gain updated flag
} Mo1000_stDacConfig;


/** @name MO1000 Clock pll info structure
 * 
 *  Hold calculated/manually provide pll parameters information.
 */
typedef struct Mo1000_StructPllParam {
    unsigned uPrimRefFreq;                          ///< Primary Pll reference frequency used for the input divider (smartmux2) for vco operation (when usReferenceDivider=1, if not it is smartmux1 frequency)
    unsigned uSecRefFreq;                           ///< Secondary reference frequency
    Cdce62005_eOutRefMode OutputRef0;               ///< Output 0 divider reference config
    Cdce62005_eOutRefMode OutputRef1;               ///< Output 1 divider reference config
    Cdce62005_eOutRefMode OutputRef2;               ///< Output 2 divider reference config
    Cdce62005_eOutRefMode OutputRef3;               ///< Output 3 divider reference config
    Cdce62005_eOutRefMode OutputRef4;               ///< Output 4 divider reference config
    unsigned uVcoFreq;                              ///< Calculated vco frequency
    unsigned char  ucPrimaryInputDivider;           ///< Calculated primary input pre divider (0 disable, 1: div by 1, 2: div by 2)
    unsigned short usReferenceDivider;              ///< Calculated reference divider (means divide by 1 to 8)
    unsigned short usInputDivider;                  ///< Calculated input divider
    unsigned short usFeedbackDivider;               ///< Calculated feedback divider
    unsigned char  ucBypassDivider;                 ///< Calculated bypass divider
    unsigned char ucPrescaleDivider;                ///< Calculated prescaler divider
    unsigned char aucOutDivider0;                   ///< Calculated output 0 divider
    unsigned char aucOutDivider1;                   ///< Calculated output 1 divider
    unsigned char aucOutDivider2;                   ///< Calculated output 2 divider
    unsigned char aucOutDivider3;                   ///< Calculated output 3 divider
    unsigned char aucOutDivider4;                   ///< Calculated output 4 divider
    Cdce62005_eR2Values R2Value;                    ///< Calculated loop filter R2 value
    Cdce62005_eR3Values R3Value;                    ///< Calculated loop filter R3 value
    Cdce62005_eC1Values C1Value;                    ///< Calculated loop filter C1 value
    Cdce62005_eC2Values C2Value;                    ///< Calculated loop filter C2 value
    Cdce62005_eC3Values C3Value;                    ///< Calculated loop filter C3 value
    Cdce62005_eChargePump ChargePump;               ///< Calculated loop filter charge pump current value
} Mo1000_stPllParam;


/** @name MO1000 Board instance structure
 * 
 *  Hold all information regarding mo1000 board instance used.
 */
typedef struct Mo1000_StructMo1000 {
    volatile Mo1000_stRegisterMap * pCore;                   ///< FPGA mo1000 core registers
    Mo1000DevCom_stComCore Com;                              ///< I2c driver information
    DevCom_hCom hCom;                                        ///< Generic driver information
    Expander_stCore Expander;                                ///< I/O expander information
    Ad9148_stCore Dac;                                       ///< DAC related driver information
    Lm75_stCore Monitor;                                     ///< Temperature related driver information
    Cdce62005_stCore Pll;                                    ///< Pll related driver information
    Mo1000_eActChannel GroupCh;                              ///< Active channels
    Mo1000_stDacConfig DacConfig[MO1000_DAC_PER_BOARD];      ///< Configuration of dacs
    Carrier_handle_t hCarrier;                               ///< Carrier handle to access the carrier specific services
    Carrier_eFmcConnectorNum_t FmcConnector;                 ///< FMC connector related to this MO1000 instance
    int iResetDone;                                          ///< Module reset correctly or not
    int iInitDone;                                           ///< Module initialized correctly or not
    int iSynthDone;                                          ///< Clocks calculation could be synthetized and was done
    int iLoopFilterDone;                                     ///< Clocks loop filter parameters are correctly provided
    unsigned short *pausLaneDelays;                          ///< Pointer on lanes delay information for the board (from board lanes to fpga) 
    unsigned int auChannelsLaneCalib[MO1000_MAX_CALIB_ATTEMPT];///< Holds data calibration information each trial
    unsigned int uCalibNdx;                                  ///< Index in the data calibration vector (previous)
    unsigned int auFrameLaneCalib[MO1000_MAX_FRAME_CALIB_ATTEMPT]; ///< Holds frame calibration information each trial
    unsigned int uCalibFrameNdx;                             ///< Number of the frame calibration (previous)
    unsigned int auSyncCalib[MO1000_MAX_SYNC_CALIB_ATTEMPT]; ///< Holds sync calibration errors each trial
    unsigned int uCalibSyncNdx;                              ///< Number of the sync calibration (previous)
    int iCalibDone;                                          ///< Calibration was done correctly or not
    int iDataCalibDone;                                      ///< Data calibration was done correctly or not
    int iFrameCalibDone;                                     ///< Frame calibration was done correctly or not
    int iSyncCalibDone;                                      ///< Synchronisation calibration was done correctly or not
    int iTestMode;                                           ///< Information when a module test mode is configured
    Mo1000_eFmcPosition FmcPosition;                         ///< FMC position of the module
    int iResetInProg;                                        ///< Means inside the reset process
    unsigned int uChannelLaneMask;                           ///< Lanes calibration mask according to active channels
    Mo1000_eHardwareRev HardwareRev;                         ///< Configures the current hardware board revision detected
    unsigned char ucDciIoDelay;                              ///< Global dci clock core delay choosen value in taps
    unsigned char *pucBottomDciIoDelay;                      ///< Pointer on Global dci clock core delay choosen value in taps of bottom board 
    char cTapsDelayOffset;                                   ///< Global taps delay offset applied to all dci, frame and data lanes taps delay
    char cLaneOffset[32];                                    ///< This is tap correction for each data lane of each dacs
    unsigned short usRefDelay;                               ///< Reference lane delay (in ps) of master
    unsigned short *pusBottomRefDelay;                       ///< Pointer on reference lane delay (in ps) of bottom board
    unsigned short usDciDelay;                               ///< Dci lane delay (in ps)
    unsigned short *pusBottomDciDelay;                       ///< Pointer on dci lane delay (in ps) of bottom board
    unsigned uDataTapTable0[MO1000_DELAY_TAPS];              ///< Data lines core delay tap capture table without swapping patterns
    unsigned uFrameTapTable[MO1000_FRAME_MODES][MO1000_DELAY_TAPS];///< Frame line core delay tap capture table for each frame mode
    unsigned char ucDataRefLane;                             ///< Data lane used for reference in the data calibration process
    unsigned char ucDataRefIoDelay;                          ///< Reference data line core delay choosen value in taps
    unsigned char *pucBottomDataRefIoDelay;                  ///< Pointer on reference data line core delay choosen value in taps of bottom board 
    unsigned char ucDciInverted;                             ///< Dci was inverted for calibration purpose when !=0
    unsigned char *pucBottomDciInverted;                     ///< Pointer on Dci was inverted for calibration purpose when !=0 of bottom board 
    unsigned char ucDataIoDelTable[32];                      ///< Data lines core delay choosen value for each data lane
    unsigned char ucDataIoGroupCount[32];                    ///< Data lines stable eye pattern width count for each data lane
    unsigned char ucDataIoGroupEnd[32];                      ///< Data lines stable eye pattern group end position
    unsigned char ucDataSmallestPosDciTol;                   ///< Data lane calibration smallest dci tap delay tolerance from positive side
    unsigned char ucDataSmallestNegDciTol;                   ///< Data lane calibration smallest dci tap delay tolerance from negative side
    unsigned char ucDataSmallestDelay;                       ///< Data lane calibration smallest tap delay calibrated in all lanes
    unsigned char ucDataGreatestDelay;                       ///< Data lane calibration greatest tap delay calibrated in all lanes
    unsigned char ucFrameIoDelay;                            ///< Frame core delay choosen value
    unsigned char ucFrameGroupCount;                         ///< Frame stable eye pattern width count
    unsigned char ucFrameGroupEnd;                           ///< Frame stable eye pattern group end position
    unsigned char ucFramePattern;                            ///< Frame core pattern mode choosen value
    unsigned char ucFrameSmallestPosDciTol;                  ///< Frame calibration smallest dci tap delay tolerance from positive side
    unsigned char ucFrameSmallestNegDciTol;                  ///< Frame calibration smallest dci tap delay tolerance from negative side
    unsigned short usFifoTherm[MO1000_DAC_PER_BOARD][MO1000_DAC_FIFO_SIZE];///< Dac fifo level statistics initial
    unsigned short usFifoThermV[MO1000_DAC_PER_BOARD][MO1000_DAC_FIFO_SIZE];///< Dac fifo level statistics after correction
    unsigned char ucFifoThermOff[MO1000_DAC_PER_BOARD];      ///< Fifo pointer correction offset used for statistics in usFifoTherm 
    unsigned char ucFifoThermVOff[MO1000_DAC_PER_BOARD];     ///< Fifo pointer correction offset used for statistics in usFifoThermV 
    Mo1000_eIoDelayRef Reference;                            ///< Core Io delay reference clock used
    unsigned uFreqSrcClk;                                    ///< Source clock (reference) frequency used
    int iFreqDacClock1;                                      ///< Dacs operating frequency used (a negative value means clock is disable)
    int iFreqMasterClock3;                                   ///< Master clock frequency used (a negative value means clock is disable)
    int iFreqMasterRef2;                                     ///< Master reference clock frequency used (a negative value means clock is disable)
    int iFreqCoreRef4;                                       ///< Core mo1000 reference frequency used (a negative value means clock is disable)
    int iFreqSyncClock0;                                     ///< Dacs synchronisation clock frequency used (a negative value means clock is disable)
    int iFreqCoreRef4Calculated;                             ///< Core mo1000 master clock reference frequency used
    int iFreqVirtualClock5;                                  ///< Virtual output frequency used (a negative value means clock is disable)
    Cdce62005_stParam stParam;                               ///< Pll complete configuration and calculations
} Mo1000_stMo1000;


/** @name MO1000 Error codes
 * 
 *  List all runtime related error codes of a mo1000 module.
 */
enum Mo1000_eError {
    eMo1000Ok               = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_MO1000, 0),      ///< No error
    eMo1000InvalidDevice01  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 1),        ///< Invalid device specified for register access location 01 
    eMo1000InvalidDevice02  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 2),        ///< Invalid device specified for register access location 02 
    eMo1000InvalidCoreReg01 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 3),        ///< Invalid fpga core register address location 01 
    eMo1000InvalidCoreReg02 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 4),        ///< Invalid fpga core register address location 02 
    eMo1000BadUseCFct       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 5),        ///< Bad user provided clock function 
    eMo1000NoCore           = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 6),        ///< No matching MO1000 fpga core 
    eMo1000UnknownRevision  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 7),        ///< Unknown provided hardware revision 
    eMo1000NoReset01        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 8),        ///< No module reset was done before executing a function that requieres it at location 01 
    eMo1000NoInit01         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 9),        ///< No module initialize was done before executing a function that requieres it at location 01 
    eMo1000NoInit02         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 10),       ///< No module initialize was done before executing a function that requieres it at location 02 
    eMo1000InvalidClkTop    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 11),       ///< Board version does not support clock master in top position 
    eMo1000Absent           = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 12),       ///< No FMC card is present
    eMo1000NoInit03         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 13),       ///< No module initialize was done before executing a function that requieres it at location 03 
    eMo1000NoInit04         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 14),       ///< No module initialize was done before executing a function that requieres it at location 04 
    eMo1000NoReadSync       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 15),       ///< Cannot execute fifo read synchronisation correctly 
    eMo1000ReadSyncLost     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 16),       ///< Fifo read synchronisation lost while waiting for stabilization  
    eMo1000FifoTher         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 17),       ///< Fifo level stabilization error  
    eMo1000FifoWar          = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 18),       ///< Fifo warnings error after fifo write synchronisation  
    eMo1000NoInit05         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 19),       ///< No module initialize was done before executing a function that requieres it at location 05 
    eMo1000NoIDelayReady    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 20),       ///< Core idelay ready (delay_ctrl_rdy) problem
    eMo1000NoMmcmLock       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 21),       ///< Core mmcm lock problem
    eMo1000DataTraining     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 22),       ///< Data lanes training problem
    eMo1000NoInit06         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 23),       ///< No module initialize was done before executing a function that requieres it at location 06 
    eMo1000InvalidClkSrc    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 24),       ///< Invalid clock source (reference) specified  
    eMo1000UnsupportedClkSrc= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 25),       ///< Unsupported clock source on board revision  
    eMo1000InvalidBottomClk = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 26),       ///< Invalid bottom source clock (reference) specified  
    eMo1000InvalidTopClk    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 27),       ///< Invalid top source clock(reference) specified  
    eMo1000DacClkOver       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 28),       ///< Requiered dac clock frequency overrange (greater than max supported 300 MHz at 1x, 600MHz at 2x and 1GHz > 2x)  
    eMo1000InvalidInterRate = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 29),       ///< Invalid interpolation rate specified  
    eMo1000BadMasterClkMode = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 30),       ///< Invalid master clock mode specified  
    eMo1000BadMasterClkFreq = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 31),       ///< Invalid master clock frequency specified (overrange)  
    eMo1000MmcmLock         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 32),       ///< No core dac clock reference mmcm lock
    eMo1000FrameTraining    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 33),       ///< Frame lane training problem  
    eMo1000NoInit07         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 34),       ///< No module initialize was done before executing a function that requieres it at location 07 
    eMo1000InvalidChannel01 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 35),       ///< Invalid dac channel specified at location 01 
    eMo1000InvalidState01   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 36),       ///< Invalid dac channel state specified at location 01 
    eMo1000NoInit08         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 37),       ///< No module initialize was done before executing a function that requieres it at location 08 
    eMo1000InvalidInterMode = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 38),       ///< Invalid interpolation mode specified  
    eMo1000NoInit09         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 39),       ///< No module initialize was done before executing a function that requieres it at location 09 
    eMo1000NoInit10         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 40),       ///< No module initialize was done before executing a function that requieres it at location 10 
    eMo1000InvalidChannel02 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 41),       ///< Invalid dac channel specified at location 02 
    eMo1000InvalidChannel03 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 42),       ///< Invalid dac channel specified at location 03 
    eMo1000NoInit11         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 43),       ///< No module initialize was done before executing a function that requieres it at location 11 
    eMo1000InvalidActChannel= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 44),       ///< Invalid number of active channels specified 
    eMo1000FifoTherWrong    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 45),       ///< Fifo thermometer value is wrong
    eMo1000FifoTherNotSet   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 46),       ///< Fifo thermometer value could not be optimally set  
    eMo1000FifoTherWrong2   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 47),       ///< Fifo thermometer value is wrong at location 02
    eMo1000FifoTher2        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 48),       ///< Fifo level stabilization error at location 02  
    eMo1000NoInit12         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 49),       ///< No module initialize was done before executing a function that requieres it at location 12 
    eMo1000InvalidDacPair01 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 50),       ///< Invalid dac pair specified at location 01 
    eMo1000NoInit13         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 51),       ///< No module initialize was done before executing a function that requieres it at location 13 
    eMo1000InvalidDacGroup01= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 52),       ///< Invalid dac channels group specified at location 01 
    eMo1000InvalidState02   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 53),       ///< Invalid dac isinc control state specified at location 02 
    eMo1000NoInit14         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 54),       ///< No module initialize was done before executing a function that requieres it at location 14 
    eMo1000InvalidDacGroup02= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 55),       ///< Invalid dac channels group specified at location 02 
    eMo1000InvalidSideBand  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 56),       ///< Invalid fine modulation side band specified 
    eMo1000NoInit15         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 57),       ///< No module initialize was done before executing a function that requieres it at location 15 
    eMo1000InvalidDacGroup03= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 58),       ///< Invalid dac channels group specified at location 03 
    eMo1000InvalidNCOState  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 59),       ///< Invalid fine modulation (NCO) state specified 
    eMo1000NoInit16         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 60),       ///< No module initialize was done before executing a function that requieres it at location 16 
    eMo1000InvalidDacGroup04= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 61),       ///< Invalid dac channels group specified at location 04 
    eMo1000InvalidDataFormat= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 62),       ///< Invalid dac data format specified 
    eMo1000NoInit17         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 63),       ///< No module initialize was done before executing a function that requieres it at location 17 
    eMo1000InvalidDacPair02 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 64),       ///< Invalid dac pair specified at location 02 
    eMo1000NoReset02        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 65),       ///< No module reset was done before executing a function that requieres it at location 02 
    eMo1000NoReset03        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 66),       ///< No module reset was done before executing a function that requieres it at location 03 
    eMo1000NoReset04        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 67),       ///< No module reset was done before executing a function that requieres it at location 04 
    eMo1000NoReset05        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 68),       ///< No module reset was done before executing a function that requieres it at location 05 
    eMo1000NoReset06        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 69),       ///< No module reset was done before executing a function that requieres it at location 06 
    eMo1000NoReset07        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 70),       ///< No module reset was done before executing a function that requieres it at location 07 
    eMo1000NoReset08        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 71),       ///< No module reset was done before executing a function that requieres it at location 08 
    eMo1000NoReset09        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 72),       ///< No module reset was done before executing a function that requieres it at location 09 
    eMo1000NoReset10        = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 73),       ///< No module reset was done before executing a function that requieres it at location 10 
    eMo1000BadCoreFrequency = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 74),       ///< Core reference frequency measurement error (time out) 
    eMo1000BadDacsFrequency = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 75),       ///< Dacs reference frequency measurement error (time out) 
    eMo1000InvalidTestMode  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 76),       ///< Invalid test mode specified 
    eMo1000NoInit18         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 77),       ///< No module initialize was done before executing a function that requieres it at location 18 
    eMo1000NoCalibDone      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 78),       ///< Attempting test mode 2 without any data calibration done 
    eMo1000BadIsincRealC0   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 79),       ///< Invalid inverse sinc real C0 coefficient specified 
    eMo1000BadIsincRealC1   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 80),       ///< Invalid inverse sinc real C1 coefficient specified 
    eMo1000BadIsincRealC2   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 81),       ///< Invalid inverse sinc real C2 coefficient specified 
    eMo1000BadIsincRealC3   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 82),       ///< Invalid inverse sinc real C3 coefficient specified 
    eMo1000BadIsincRealC4   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 83),       ///< Invalid inverse sinc real C4 coefficient specified 
    eMo1000BadIsincImgC0    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 84),       ///< Invalid inverse sinc imaginary C0 coefficient specified 
    eMo1000BadIsincImgC1    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 85),       ///< Invalid inverse sinc imaginary C1 coefficient specified 
    eMo1000BadIsincImgC2    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 86),       ///< Invalid inverse sinc imaginary C2 coefficient specified 
    eMo1000BadIsincImgC3    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 87),       ///< Invalid inverse sinc imaginary C3 coefficient specified 
    eMo1000BadIsincImgC4    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 88),       ///< Invalid inverse sinc imaginary C4 coefficient specified 
    eMo1000NoInit19         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 89),       ///< No module initialize was done before executing a function that requieres it at location 19 
    eMo1000NoClockSetup01   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 90),       ///< No clock setup was provided correctly using Mo1000_SetClockConfig() at location 01 
    eMo1000NoClockLoopFilter= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 91),       ///< No valid pll loop filter parameters were calculated or provided 
    eMo1000NoClockSetup02   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 92),       ///< No clock setup was provided correctly using Mo1000_SetClockConfig() at location 02 
    eMo1000NoInit20         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 93),       ///< No module initialize was done before executing a function that requieres it at location 20 
    eMo1000NoInit21         = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 94),       ///< No module initialize was done before executing a function that requieres it at location 21 
    eMo1000NoClockSetup03   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 95),       ///< No clock setup was provided correctly using Mo1000_SetClockConfig() at location 03 
    eMo1000DataFreqUnder    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 96),       ///< Requiered fpga data rate is underrange: (smaller than min supported of 50 MHz)  
    eMo1000BadBotTapRefPtr  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 97),       ///< Invalid bottom board data reference tap delay pointer  
    eMo1000BadBotTapRefValue= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 98),       ///< Invalid bottom board data reference tap delay value  
    eMo1000BadBotDciDelPtr  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 99),       ///< Invalid bottom board dci tap delay pointer  
    eMo1000BadBotDciDelValue= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 100),      ///< Invalid bottom board dci tap delay value  
    eMo1000BadBotDciInvPtr  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 101),      ///< Invalid bottom board dci inversion state pointer  
    eMo1000BadBotDciInvValue= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 102),      ///< Invalid bottom board dci inversion state value
    eMo1000BadBotRefDelayPtr= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 103),      ///< Invalid bottom board reference delay pointer
    eMo1000BadBotDciDelayPtr= MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000, 104)       ///< Invalid bottom board dci delay pointer  
};                                                                                                                           


#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif
