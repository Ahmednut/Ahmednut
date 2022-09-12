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
 * @file       Ad9148_defines.h
 * @brief      DAC related data and structure definitions
 *
 * $Date: 2015/10/16 14:31:21 $
 * $Revision: 1.2 $
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

#ifndef AD9148_DEFINES_H__
#define AD9148_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "Mo1000DevCom_defines.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define AD9148_INVALIDTEMP -128          ///< This would give -128C which is outside device min range of -55C

#define AD9148_CHANNELS_PER_DAC 4        ///< Number of channels per dac


/** @name Quadrature offset range
 * 
 *  Possible quadrature offset range.   
 */
#define AD9148_QUADPHASERANGEMIN -512    ///<  Min quadrature offset possible
#define AD9148_QUADPHASERANGEMAX  511    ///<  Max quadrature offset possible


/** @name Active dac pair state
 * 
 *  Possible dac pair active state.   
 */
typedef enum Ad9148_EnumActPairState {
    eAd9148DacPairEnable=0,              ///< IQ Dac pair is active
    eAd9148DacPairDisable=1              ///< IQ Dac pair is disable (deep power down state)
}Ad9148_eActPairState;


/** @name Dac core operation
 * 
 *  Dac basic core info   
 */
typedef struct Ad9148_StructCore {
    Mo1000DevCom_stComCore * m_pCom;           ///< Communication info
}Ad9148_stCore;


/** @name Synchronisation mode
 * 
 *  Possible ad9148 synchronisation modes.   
 */
typedef enum Ad9148_EnumSyncMode {
    eAd9148SyncFifoRate=0,               ///< Fifo rate sync mode
    eAd9148SyncDataRate=1                ///< Data rate sync mode
}Ad9148_eSyncMode;


/** @name Synchronisation mode control
 * 
 *  Possible ad9148 synchronisation control.   
 */
typedef enum Ad9148_EnumSyncControl {
    eAd9148SyncDisable=0,                ///< Synchronisation mode disable
    eAd9148SyncEnable=1                  ///< Synchronisation mode enable
}Ad9148_eSyncControl;


/** @name Fine modulation nco control
 * 
 *  Possible fine modulation nco control.   
 */
typedef enum Ad9148_EnumNcoControl {
    eAd9148FineModEnable=0,              ///< Fine modulation enable
    eAd9148FineModDisable=1              ///< Fine modulation disable
}Ad9148_eNcoControl;


/** @name Data format
 * 
 *  Possible data format.   
 */
typedef enum Ad9148_EnumDataFormat {
    eAd9148Format2Complement=0,          ///< 2 complement data format
    eAd9148FormatBinary=1                ///< Unsigned binary data format
}Ad9148_eDataFormat;



/** @name Synchronisation averaging
 * 
 *  Possible synchronisation averaging values.   
 */
typedef enum Ad9148_EnumSyncAveraging {
    eAd9148SyncAveraging1=0,             ///< Sync Avg = 1 (no averaging)
    eAd9148SyncAveraging2=1,             ///< Synchronisation mode enable
    eAd9148SyncAveraging4=2,             ///< Synchronisation mode enable
    eAd9148SyncAveraging8=3,             ///< Synchronisation mode enable
    eAd9148SyncAveraging16=4,            ///< Synchronisation mode enable
    eAd9148SyncAveraging32=5,            ///< Synchronisation mode enable
    eAd9148SyncAveraging64=6,            ///< Synchronisation mode enable
    eAd9148SyncAveraging128=7            ///< Synchronisation mode enable
}Ad9148_eSyncAveraging;


/** @name Synchronisation averaging
 * 
 *  Possible synchronisation averaging values.   
 */
typedef enum Ad9148_EnumFifoOffset {
    eAd9148FifoOffset0=0,                ///< Fifo phase offset 0
    eAd9148FifoOffset1=1,                ///< Fifo phase offset 1
    eAd9148FifoOffset2=2,                ///< Fifo phase offset 2
    eAd9148FifoOffset3=3,                ///< Fifo phase offset 3
    eAd9148FifoOffset4=4,                ///< Fifo phase offset 4
    eAd9148FifoOffset5=5,                ///< Fifo phase offset 5
    eAd9148FifoOffset6=6,                ///< Fifo phase offset 6
    eAd9148FifoOffset7=7,                ///< Fifo phase offset 7
}Ad9148_eFifoOffset;


/** @name Synchronisation edge
 * 
 *  Possible ad9148 synchronisation edge.   
 */
typedef enum Ad9148_EnumSyncEdge {
    eAd9148SyncFallingEdge=0,            ///< Synchronisation mode falling
    eAd9148SyncRisingEdge=1              ///< Synchronisation mode rising
}Ad9148_eSyncEdge;


/** @name Event state
 * 
 *  Possible irq event state.   
 */
typedef enum Ad9148_EnumEventState {
    eAd9148EventInactive=0,              ///< Inactive or disable event
    eAd9148EventActive=1                 ///< Active or enable event
}Ad9148_eEventState;


/** @name Output control state
 * 
 *  Possible output control state.   
 */
typedef enum Ad9148_EnumOutputState {
    eAd9148OutputEnable=0,               ///< Dac output is enable
    eAd9148OutputDisable=1               ///< Dac output is disable
}Ad9148_eOutputState;


/** @name Dac selection
 * 
 *  Possible dac to work with.   
 */
typedef enum Ad9148_EnumDacSelect {
    eAd9148SelectDac1I=0,                ///< Dac 1 selected (I)
    eAd9148SelectDac2Q=1,                ///< Dac 2 selected (Q)
    eAd9148SelectDac3I=2,                ///< Dac 3 selected (I)
    eAd9148SelectDac4Q=3                 ///< Dac 4 selected (Q)
}Ad9148_eDacSelect;


/** @name IQ Dac pair selection
 * 
 *  Possible IQ dac pair selection   
 */
typedef enum Ad9148_EnumSelectDacPair {
    eAd9148SelectIQDac12,                ///< Dac 1 & 2 selected (I+Q)
    eAd9148SelectIQDac34                 ///< Dac 3 & 4 selected (I+Q)
}Ad9148_eSelectDacPair;


/** @name Dac Sed data port selection
 * 
 *  Possible dac Sed data port selection   
 */
typedef enum Ad9148_EnumSelectSedPort {
    eAd9148SelectPortA=0,                ///< Sed data port A
    eAd9148SelectPortB=1                 ///< Sed data port B
}Ad9148_eSelectSedPort;


/** @name SED control state
 * 
 *  Possible SED control state.   
 */
typedef enum Ad9148_EnumSedControlState {
    eAd9148SedDisable=0,                 ///< Sed pattern mode disable
    eAd9148SedEnable=1,                  ///< Cumulative Sed pattern mode enable
    eAd9148SedAutoClearEnable=2          ///< Auto clear Sed pattern mode enable
}Ad9148_eSedControlState;


/** @name Inverse Sinc filter control state
 * 
 *  Possible inverse sinc filter control state.   
 */
typedef enum Ad9148_EnumISincControlState {
    eAd9148ISincEnable=0,                ///< Inverse sinc filter enable
    eAd9148ISincDisable=1                ///< Inverse sinc filter disable
}Ad9148_eISincControlState;


/** @name Fine modulation sideband
 * 
 *  Possible fine modulation nco sideband   
 */
typedef enum Ad9148_EnumNcoSideband {
    eAd9148NcoHighSide=0,                ///< Nco high sideband
    eAd9148NcoLowSide=1                  ///< Nco low sideband
}Ad9148_eNcoSideband;


/** @name Interpolation modes
 * 
 *  Possible interpolation modes.   
 */
typedef enum Ad9148_EnumInterpolationMode {
    eAd9148Inter1x=0,                    ///< Interpolation 1x
    eAd9148Inter2x=1,                    ///< Interpolation 2x no Fcenter offset
    eAd9148Inter2x_fsdiv4=2,             ///< Interpolation 2x with Fcenter offset Fs/4 (with premodulation)
    eAd9148Inter2x_fsdiv2=3,             ///< Interpolation 2x with Fcenter offset Fs/2
    eAd9148Inter2x_3fsdiv4=4,            ///< Interpolation 2x with Fcenter offset 3Fs/4 (with premodulation)
    eAd9148Inter4x=5,                    ///< Interpolation 4x no Fcenter offset
    eAd9148Inter4x_fsdiv8=6,             ///< Interpolation 4x with Fcenter offset Fs/8 (with premodulation)
    eAd9148Inter4x_fsdiv4=7,             ///< Interpolation 4x with Fcenter offset Fs/4
    eAd9148Inter4x_3fsdiv8=8,            ///< Interpolation 4x with Fcenter offset 3Fs/8 (with premodulation)
    eAd9148Inter4x_fsdiv2=9,             ///< Interpolation 4x with Fcenter offset Fs/2
    eAd9148Inter4x_5fsdiv8=10,           ///< Interpolation 4x with Fcenter offset 5Fs/8 (with premodulation)
    eAd9148Inter4x_3fsdiv4=11,           ///< Interpolation 4x with Fcenter offset 3Fs/4
    eAd9148Inter4x_7fsdiv8=12,           ///< Interpolation 4x with Fcenter offset 7Fs/8 (with premodulation)
    eAd9148Inter8x=13,                   ///< Interpolation 8x no Fcenter offset
    eAd9148Inter8x_fsdiv8=14,            ///< Interpolation 8x with Fcenter offset Fs/8
    eAd9148Inter8x_fsdiv4=15,            ///< Interpolation 8x with Fcenter offset Fs/4
    eAd9148Inter8x_3fsdiv8=16,           ///< Interpolation 8x with Fcenter offset 3Fs/8
    eAd9148Inter8x_fsdiv2=17,            ///< Interpolation 8x with Fcenter offset Fs/2
    eAd9148Inter8x_5fsdiv8=18,           ///< Interpolation 8x with Fcenter offset 5Fs/8
    eAd9148Inter8x_3fsdiv4=19,           ///< Interpolation 8x with Fcenter offset 3Fs/4
    eAd9148Inter8x_7fsdiv8=20            ///< Interpolation 8x with Fcenter offset 7Fs/8
}Ad9148_eInterpolationMode;


/** @name Ad9148 events
 * 
 *  Possible dac events (irq).   
 */
typedef struct Ad9148_StructDacEvent {
    union
    {
        struct
        {
#ifdef _BIG_ENDIAN 
            unsigned short 
            m_uPllLockLost:1,           ///< Pll lock lost when 1
            m_uPllLock:1,               ///< Pll lock when 1
            m_uSyncLockLost:1,          ///< Synchronisation lock lost when 1
            m_uSyncLock:1,              ///< Synchronisation lock when 1
            m_uReserved01:1,
            m_uFifoSpiAligned:1,        ///< Fifo aligned from spi command 1
            m_uFifoWarn1:1,             ///< Fifo warning #1 when 1
            m_uFifoWarn2:1,             ///< Fifo warning #2 when 1
            m_uReserved02:1,
            m_uReserved03:1,
            m_uReserved04:1,
            m_uAedComparePass:1,        ///< Sed Aed compare pass when 1
            m_uAedCompareFail:1,        ///< Sed Aed compare fail when 1
            m_uSedCompareFail:1,        ///< Sed compare fail when 1
            m_uReserved05:1,
            m_uReserved06:1;
#else
            unsigned short 
            m_uReserved06:1,
            m_uReserved05:1,
            m_uSedCompareFail:1,        ///< Sed compare fail when 1
            m_uAedCompareFail:1,        ///< Sed Aed compare fail when 1
            m_uAedComparePass:1,        ///< Sed Aed compare pass when 1
            m_uReserved04:1,
            m_uReserved03:1,
            m_uReserved02:1,
            m_uFifoWarn2:1,             ///< Fifo warning #2 when 1
            m_uFifoWarn1:1,             ///< Fifo warning #1 when 1
            m_uFifoSpiAligned:1,        ///< Fifo aligned from spi command 1
            m_uReserved01:1,
            m_uSyncLock:1,              ///< Synchronisation lock when 1
            m_uSyncLockLost:1,          ///< Synchronisation lock lost when 1
            m_uPllLock:1,               ///< Pll lock when 1
            m_uPllLockLost:1;           ///< Pll lock lost when 1
#endif
        }stBit;
        struct
        {
#ifdef _BIG_ENDIAN 
            unsigned short m_uEvent0:8, m_uEvent1:8;
#else
            unsigned short m_uEvent1:8, m_uEvent0:8;
#endif
        }stEventParts;
        unsigned short m_uRaw;
    };
}Ad9148_stDacEvent;


/** @name Ad9148 inverse sinc filter coefficients
 * 
 *  Inverse sinc filter coefficients definitions.   
 */
typedef struct Ad9148_StructISincCoefficients {
    union
    {
        struct
        {
            int 
            m_iC0:3,                ///< C0 coefficient
            m_iC1:4,                ///< C1 coefficient
            m_iC2:5,                ///< C2 coefficient
            m_iC3:7,                ///< C3 coefficient
            m_iC4:10,               ///< C4 coefficient
            m_iReserved:3;          ///< Reserved
        }stBit;
        int m_iRaw;
    };
}Ad9148_stISincCoefficients;


// Internal register address
typedef enum Ad9148_EnumAddr {
    eAd9148Addr00=0,
    eAd9148Addr01=1,
    eAd9148Addr03=3,
    eAd9148Addr04=4,
    eAd9148Addr05=5,
    eAd9148Addr06=6,
    eAd9148Addr07=7,
    eAd9148Addr08=8,
    eAd9148Addr0a=0x0a,
    eAd9148Addr0c=0x0c,
    eAd9148Addr0d=0x0d,
    eAd9148Addr0e=0x0e,
    eAd9148Addr0f=0x0f,
    eAd9148Addr10=0x10,
    eAd9148Addr11=0x11,
    eAd9148Addr12=0x12,
    eAd9148Addr14=0x14,
    eAd9148Addr15=0x15,
    eAd9148Addr17=0x17,
    eAd9148Addr18=0x18,
    eAd9148Addr19=0x19,
    eAd9148Addr1a=0x1a,
    eAd9148Addr1c=0x1c,
    eAd9148Addr1d=0x1d,
    eAd9148Addr1e=0x1e,
    eAd9148Addr1f=0x1f,
    eAd9148Addr20=0x20,
    eAd9148Addr21=0x21,
    eAd9148Addr22=0x22,
    eAd9148Addr23=0x23,
    eAd9148Addr24=0x24,
    eAd9148Addr25=0x25,
    eAd9148Addr26=0x26,
    eAd9148Addr27=0x27,
    eAd9148Addr28=0x28,
    eAd9148Addr29=0x29,
    eAd9148Addr2a=0x2a,
    eAd9148Addr2b=0x2b,
    eAd9148Addr2c=0x2c,
    eAd9148Addr2d=0x2d,
    eAd9148Addr2e=0x2e,
    eAd9148Addr2f=0x2f,
    eAd9148Addr30=0x30,
    eAd9148Addr31=0x31,
    eAd9148Addr32=0x32,
    eAd9148Addr33=0x33,
    eAd9148Addr34=0x34,
    eAd9148Addr35=0x35,
    eAd9148Addr36=0x36,
    eAd9148Addr37=0x37,
    eAd9148Addr38=0x38,
    eAd9148Addr39=0x39,
    eAd9148Addr3a=0x3a,
    eAd9148Addr3b=0x3b,
    eAd9148Addr3c=0x3c,
    eAd9148Addr3d=0x3d,
    eAd9148Addr3e=0x3e,
    eAd9148Addr3f=0x3f,
    eAd9148Addr40=0x40,
    eAd9148Addr41=0x41,
    eAd9148Addr42=0x42,
    eAd9148Addr43=0x43,
    eAd9148Addr44=0x44,
    eAd9148Addr50=0x50,
    eAd9148Addr51=0x51,
    eAd9148Addr54=0x54,
    eAd9148Addr55=0x55,
    eAd9148Addr56=0x56,
    eAd9148Addr57=0x57,
    eAd9148Addr58=0x58,
    eAd9148Addr59=0x59,
    eAd9148Addr5a=0x5a,
    eAd9148Addr5c=0x5c,
    eAd9148Addr5d=0x5d,
    eAd9148Addr5e=0x5e,
    eAd9148Addr5f=0x5f,
    eAd9148Addr72=0x72
} Ad9148_eAddr;


// Module errors
enum Ad9148_eError {
    eAd9148Ok                  = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_AD9148, 0),
    eAd9148InvalidAddr01       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 1),   /**< Invalid register address specified location 01 */
    eAd9148InvalidAddr02       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 2),   /**< Invalid register address specified location 02 */
    eAd9148InvalidDacSelect01  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 3),   /**< Invalid dac selection specified location 01 */
    eAd9148InvalidDacPair01    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 4),   /**< Invalid IQ dac pair selection specified location 01 */
    eAd9148InvalidDacPort01    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 5),   /**< Invalid IQ Sed dac port selection specified location 01 */
    eAd9148InvalidDacPair03    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 6),   /**< Invalid IQ dac pair selection specified location 03 */
    eAd9148FtwUpdateTimeOut    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 7),   /**< Time out while waiting for ftw update acknowledge for NCO */
    eAd9148InvalidDacPair02    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 8),   /**< Invalid IQ dac pair selection specified location 02 */
    eAd9148IQuadPhaseErr       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 9),   /**< Invalid I (real) quadrature phase */
    eAd9148QQuadPhaseErr       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 10),  /**< Invalid Q (imaginary) quadrature phase */
    eAd9148InvalidDacSelect02  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 11),  /**< Invalid dac selection specified location 02 */
    eAd9148InvalidDacSelect03  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 12),  /**< Invalid dac selection specified location 03 */
    eAd9148InvalidInterMode    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 13),  /**< Invalid interpolation mode */
    eAd9148InvalidSyncMode     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 14),  /**< Invalid sync mode specified */
    eAd9148InvalidSyncCtrl     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 15),  /**< Invalid sync control specified */
    eAd9148InvalidSyncAvg      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 16),  /**< Invalid sync averaging specified */
    eAd9148InvalidSyncEdge     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 17),  /**< Invalid sync edge specified */
    eAd9148InvalidSyncOffset   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 18),  /**< Invalid fifo sync offset specified */
    eAd9148InvalidOutCtrl      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 19),  /**< Invalid output control specified */
    eAd9148InvalidSedCtrl      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 20),  /**< Invalid sed control specified */
    eAd9148InvalidISincCtrl    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 21),  /**< Invalid inverse sinc control specified */
    eAd9148InvalidSideband     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 22),  /**< Invalid sideband specified */
    eAd9148InvalidNcoCtrl      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 23),  /**< Invalid nco control specified */
    eAd9148InvalidDataFormat   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 24),  /**< Invalid data format specified */
    eAd9148InvalidDacPort02    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_AD9148, 25),  /**< Invalid IQ Sed dac port selection specified location 02 */
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

