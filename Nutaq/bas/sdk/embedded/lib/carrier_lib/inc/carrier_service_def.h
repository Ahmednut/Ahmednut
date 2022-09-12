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
 * @file       carrier_service_def.h
 * @brief      carrier generic library structure and data definitions.
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.7 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup CARRIERLIB
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

#ifndef __CARRIER_SERVICE_DEF_H__
#define __CARRIER_SERVICE_DEF_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus


/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

#define CARRIER_REG_BASE_ADDR           0x70000000
#define CARRIER_REG_MAX_NUM             1
#define CARRIER_REG_SIZE                0X10000

 /** @name Carrier object
 *
 *  Holds a pointer to access the carrier
 */
typedef struct sCarrier * Carrier_handle_t;

 /** @name usleep function pointer
 *
 *  Define a function point 
 */
typedef int (*Carrier_pfUsleep)(unsigned int usec);

/** @name Carrier type
 *
 *  Hold information of all the different supported carriers
 */
typedef enum Carrier_eType {
    Carrier_ePerseus601X,   ///< Perseus601X carrier
    Carrier_ePerseus611X,   ///< Perseus611X carrier
    Carrier_eML605,         ///< ML605 carrier
    Carrier_eVC707,         ///< VC707 carrier
    Carrier_eZedboard,      ///< Zedboard carrier
    Carrier_eZC706,         ///< ZC706 carrier
    Carrier_eUnknown        ///< Unknown carrier
}Carrier_eType_t;

/** @name Carrier I2C Zone type
 *
 *  Hold information of all the different I2C zones a carrier could have
 */
// typedef enum Carrier_eI2cZone {
    // Carrier_eI2cZoneCarrier=0,  ///< I2C zone that contains all I2C devices on the carrier
    // Carrier_eI2cZoneFmc1=1,     ///< I2C zone that contains all I2C devices on the FMC card on connector FMC 1
    // Carrier_eI2cZoneFmc2=2,     ///< I2C zone that contains all I2C devices on the FMC card on connector FMC 2
    // Carrier_eI2cZoneRtm=3       ///< I2C zone that contains all I2C devices on the rear transition module (RTM)
// }Carrier_eI2cZone_t;

/** @name I2C Options structure
 *
 *  Structure declaration of the I2C options global to all carrier
 */
typedef struct I2C_Options {
    unsigned nbByteAddr;
    unsigned stopCondition;
} I2C_Options_t;

// Do not change following value
#define I2C_I2SCAN_MAX_DEV 128                              ///< Max i2c devices scanned

/** @name I2C devices scan structure
 *
 *  Hold i2c devices scan results.
 */
typedef struct I2C_StructI2cScan {
    unsigned char m_ucDevDetect[I2C_I2SCAN_MAX_DEV];        ///< i2c detected devices from scan
} I2C_stI2cScan;

/** @name Carrier Module list
 *
 *  Hold information of all the different modules supported
 */
typedef enum Carrier_eModule {
    Carrier_eModuleRTDExGigE,   ///< Real-time data exchange (RTDEx) over Gigabit Ethernet FPGA core
    Carrier_eModuleRecPlay,     ///< Record and playback FPGA core
    Carrier_eModuleI2C,         ///< Main I2C FPGA core
    Carrier_eModuleDdr3I2C,     ///< I2C FPGA core for SODIMM DDR3
    Carrier_eModuleFMC,         ///< FPGA core that instantiate the FMC module
    Carrier_eModuleCarrier,     ///< Carrier FPGA core
    Carrier_eModulePpsSync,     ///< PPS Sync FPGA core
    Carrier_eModuleLVDS,        ///< LVDS instance 0 FPGA core
    Carrier_eModuleAurora,      ///< Aurora instance 0 FPGA core
    Carrier_eModuleRTDExSync,   ///< RTDEx Sync FPGA core
    Carrier_eModuleTimestamp,   ///< Timestamp FPGA core
    Carrier_eModuleDPRAM,       ///< Address space for the dual-port RAM
    Carrier_eModuleSysMon       ///< System monitor core
}Carrier_eModule_t;

/** @name Carrier mnemory space list
 *
 *  Hold information of all the different mnemory spaces supported
 */
typedef enum Carrier_eMemSpace {
    Carrier_eMemSpaceUser,     ///< Address space for the user modules
    Carrier_eMemSpaceFMC       ///< Address space for FMC modules
}Carrier_eMemSpace_t;


/** @name FMC carrier operation state
 *
 *  Hold information of different modules operation state.
 */
typedef enum Carrier_eState {
    CARRIER_MODULE_UNDETECTED=0,                    ///< Module is not detected
    CARRIER_MODULE_UNIDENTIFIED=1,                  ///< Module is unidentified
    CARRIER_WRONGFPGA_CORE=2,                       ///< Wrong fpga core that does not match with the module
    CARRIER_UNMATCH_MODULES_SLOT=3,                 ///< Unmatch modules in a slot
    CARRIER_UNMATCH_VADJ=4,                         ///< Unmatch vadj requierement for modules in a slot
    CARRIER_UNSUPPORTED_VADJ=5,                     ///< Unsupported vadj requierement for modules in a slot for the carrier
    CARRIER_VALID=6,                                ///< Module is compatible and could be operated
    CARRIER_LAST=CARRIER_VALID+1                ///< Last carrier status
}Carrier_eState_t;

/** @name FMC carrier connector position information
 *
 *  Hold information of all different possible connectors of the carrier.
 */
typedef enum Carrier_eFmcConnectorNum {
    Carrier_eFmcConnector0=0,                              ///< Connector 0
    Carrier_eFmcConnector1=1,                              ///< Connector 1
    Carrier_eFmcConnectorLast=Carrier_eFmcConnector1+1     ///< Last connector from carrier
}Carrier_eFmcConnectorNum_t;

/** @name FMC carrier slot position information
 *
 *  Hold information of all different possible slots of the carrier.
 */
typedef enum Carrier_eFmcSlotNum {
    Carrier_eFmcSlot0=0,                                ///< Slot 0
    Carrier_eFmcSlotLast=Carrier_eFmcSlot0+1            ///< Last slot from carrier
}Carrier_eFmcSlotNum_t;

/** @name FMC carrier board position information
 *
 *  Hold information of all different possible board position in a slot of the carrier.
 */
typedef enum Carrier_eFmcBoardNum {
    Carrier_eFmcBoard0=0,                               ///< Board #0 (bottom)
    Carrier_eFmcBoard1=1,                               ///< Board #1 (top)
    Carrier_eFmcBoardLast=Carrier_eFmcBoard1+1          ///< Last board in slot from carrier
}Carrier_eFmcBoardNum_t;

#define CARRIER_FMC_MAXCONNECTOR    Carrier_eFmcConnectorLast   // Highest number of connector supported for all the carriers
#define CARRIER_FMC_MAXSLOT         Carrier_eFmcSlotLast        // Highest number of slot supported for all the carriers (always be 1)
#define CARRIER_FMC_MAXBOARD        Carrier_eFmcBoardLast       // Highest number of board supported by FMC connector for all the carriers

/** @name FMC carrier board Vadj value information
 *
 *  Hold information of all different possible Vadj voltages.
 */
typedef enum Carrier_eVadjValue
{
    VADJ_FIRST=0,
	VADJ_1_192V=0,          /**< 1.192 V */
	VADJ_1_490V=1,          /**< 1.490 V */
	VADJ_1_803V=2,          /**< 1.803 V */
	VADJ_2_503V=3,          /**< 2.503 V */
  VADJ_3_300V=4,          /**< 3.300 V */
	VADJ_DISABLE=5,         /**< VADJ control disable*/
    VADJ_UNKNOWN=6          /**< Unknown */
}Carrier_eVadjValue_t;

/****************************************************************************
 *
 * FMC Vadj levels values in 10mV resolution.
 *
 ****************************************************************************/
// Do not change this list content and order (see related E_VADJ_VALUE and VADJ_VOLTAGE_LEVELS_DEFINITION)

/** @name FMC carrier board Vadj value level information
 *
 *  Hold information of all different possible Vadj voltage levels.
 */
typedef enum Carrier_eVadjValueLevels
{
	VADJ_1_192V_LEVEL=119,    /**< 1.192 V */
	VADJ_1_490V_LEVEL=149,    /**< 1.490 V */
	VADJ_1_803V_LEVEL=180,    /**< 1.803 V */
	VADJ_2_503V_LEVEL=250,    /**< 2.503 V */
  VADJ_3_300V_LEVEL=330,    /**< 3.300 V */
	VADJ_DISABLE_LEVEL=0,     /**< VADJ control disable*/
    VADJ_UNKNOWN_LEVEL=0      /**< Unknown */
}Carrier_eVadjValueLevels_t;

/** @name TClk information
 *
 *  Hold information of all different possible TClk.
 */
typedef enum Carrier_eTClk {
    Carrier_eTClkA=1,               ///< TClkA
    Carrier_eTClkB=2,               ///< TClkB
    Carrier_eTClkC=3,               ///< TClkC
    Carrier_eTClkD=4                ///< TClkD
}Carrier_eTClk_t;

/** @name TClk direction information
 *
 *  Hold direction information of all different possible TClk directions.
 */
typedef enum Carrier_eTClkDir {
    Carrier_eTClkTx=0,               ///< TClk transmission from the carrier to the AMC backplane
    Carrier_eTClkRx=1                ///< TClk reception from the AMC backplane to the carrier.
}Carrier_eTClkDir_t;

/** @name Clk state information
 *
 *  Hold state information of all different possible clock states.
 */
typedef enum Carrier_eClkState {
    Carrier_eClkDisable=0,         ///< TClk in the specified direction is disable
    Carrier_eClkEnable=1,          ///< TClk in the specified direction is enable
    Carrier_eClkUnknown=2          ///< TClk state is unknown.
}Carrier_eClkState_t;

/** @name Oscillator information
 *
 *  Hold information of all different oscillators.
 */
typedef enum Carrier_eOscillator {
    Carrier_eOscMgtRefClk100M=0,         ///< 100 MHz oscillator that is used as a MGT reference
    Carrier_eOscMgtRefClk125M=1,         ///< 125 MHz oscillator that is used as a MGT reference
    Carrier_eOscMgtRefClk156M25=2        ///< 156.25 MHz oscillator that is used as a MGT reference
}Carrier_eOscillator_t;

/** @name TClk source information
 *
 *  Hold information of all different possible sources that can drive the TClk.
 */
typedef enum Carrier_eTClkSrc {
    Carrier_eTClkFromFpga=0,        ///< TClk is driven by FPGA pins
    Carrier_eTClkFromFmc=1          ///< TClk is driven by signal from the FMC connector
}Carrier_eTClkSrc_t;

/** @name FMC Bidirectional clock information
 *
 *  Hold information of all different possible FMC bidirectional clocks.
 */
typedef enum Carrier_eFmcBidirClk {
    Carrier_eFmc1BidirClk2=0,        ///< FMC #1 Bidirectional clock 2
    Carrier_eFmc1BidirClk3=1,        ///< FMC #1 Bidirectional clock 3
    Carrier_eFmc2BidirClk2=2,        ///< FMC #2 Bidirectional clock 2
    Carrier_eFmc2BidirClk3=3         ///< FMC #2 Bidirectional clock 3
}Carrier_eFmcBidirClk_t;

/** @name FMC Bidirectional clock connection information
 *
 *  Hold information of all different possible FMC bidirectional clock connections.
 */
typedef enum Carrier_eFmcBidirClkConnection {
    Carrier_eFmcBidirClkWithFPGA=0,        ///< Connect the FMC bidirectional clock with the FPGA pins
    Carrier_eFmcBidirClkFromTClk=1         ///< Transmit the TClk to the FMC bidirectional clock of the FMC connector
}Carrier_eFmcBidirClkConnection_t;

/** @name AMC FClkA clock connection information
 *
 *  Hold information of all different possible AMC FClkA connections.
 */
typedef enum Carrier_eFClkAConnection {
    Carrier_eFClkAToMgtRefClk=0,          ///< Use FClkA clock as input to drive the MGT reference clock
    Carrier_eFClkAFrom100MOsc=1,          ///< Use the internal 100MHz oscillator to drive the FClkA output clock
    Carrier_eFClkADisable=2               ///< Set the AMC FClkA in high impedance
}Carrier_eFClkAConnection_t;

/** @name Carrier detection type
 *
 *  Hold information of what kind of detection was achieved for a module
 */
typedef enum CARRIER_E_detected {
    CARRIER_NOT_DETECTED=0,                             ///< Not detected
    CARRIER_EXACT_DETECTED=1,                           ///< Exactly detected for a valid module
    CARRIER_COMP_DETECTED=2                             ///< Compatibility mode module not exactly detected, but with a matching fpga core
}CARRIER_detected;


/** @name Carrier information type
 *
 *  Hold information of what kind of info could be requested from carrier
 */
typedef enum CARRIER_E_infotype {
    CARRIER_INFO_STATE=0,                               ///< State of module in the board/slot
    CARRIER_INFO_BOARDTYPE=1,                           ///< Board type of module in the board/slot
    CARRIER_INFO_FPGACORE=2,                            ///< FPGA core of module in the board/slot
    CARRIER_INFO_CARRIERVADJ=3,                         ///< Carrier vadj setting of module in the board/slot
    CARRIER_INFO_IDSTRING=4,                            ///< Identification string of module in the board/slot
    CARRIER_INFO_PARTNUM=5,                             ///< Part number string of module in the board/slot
    CARRIER_INFO_SERIALNUM=6,                           ///< Serial number string of module in the board/slot
    CARRIER_INFO_HARDREV=7,                             ///< Hardware revision string of module in the board/slot
    CARRIER_INFO_SOFTREV=8,                             ///< Software revision string of module in the board/slot
    CARRIER_INFO_VADJVAL=9,                             ///< Vadj voltage info string of module in the board/slot
    CARRIER_INFO_MANUF=10,                              ///< Manufacturer string of module in the board/slot
    CARRIER_INFO_FPGACORENAME=11                        ///< FPGA core name of module in the board/slot
}CARRIER_infotype;


/** @name Carrier console printout mode
 *
 *  Hold information of what kind of console printout is desired
 */
typedef enum CARRIER_E_printout {
    CARRIER_PRINTOUT_NONE=0,                            ///< No console printout at all
    CARRIER_PRINTOUT_SUMMARY=1,                         ///< This is the same as CARRIER_PRINTOUT_MINIMUM in debug, but in release will only print discovered module info
    CARRIER_PRINTOUT_MINIMUM=2,                         ///< Minimum console printout (does not display vita decoding info)
    CARRIER_PRINTOUT_DEEP=3                             ///< Deep console printout (also display vita decoding info)
}CARRIER_printout;

/** @name Carrier Error codes
 *
 *  List all runtime related error codes of a carrier service library.
 */
enum CARRIER_service_error {
    CARRIER_OK                          = MAKE_RESULT(LYR_SEVERITY_SUCCESS,    MODULE_CARRIER, 0),          ///< No error
    CARRIER_NOT_OPEN                    = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 1),          ///< Carrier instance was not opened correctly
    CARRIER_INVALID_ASSGFCT             = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 2),          ///< Invalid carrier slot/board assignment function specified
    CARRIER_INVALID_VADJFCT             = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 3),          ///< Invalid carrier slot/board vAdj function specified
    CARRIER_INVALID_TIMERFCT            = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 4),          ///< Invalid timer function specified
    CARRIER_BOARDTYPE_NOTEXIST          = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 5),          ///< Board type does not exist in condition list
    CARRIER_NODISCOVER                  = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 6),          ///< Carrier discover function was not ran correctly
    CARRIER_VADJ_NOVALIDMODULE          = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 7),          ///< No valid module detected, cannot set Vadj
    CARRIER_INVALID_CONNECTOR           = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 8),          ///< Invalid connector specified for this carrier
    CARRIER_INVALID_BOARD               = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 9),          ///< Invalid board specified for this carrier
    CARRIER_INVALID_BOARDTYPELST        = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 10),         ///< Invalid board type list
    CARRIER_INVALID_STRING              = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 11),         ///< Invalid string provided
    CARRIER_BADINFOTYPE                 = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 12),         ///< Invalid board info type specified
    CARRIER_VADJ_NEIGHBORBAD            = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 13),         ///< Slot neighbour board problem, cannot set Vadj
    CARRIER_INVALID_SIGNALFCT           = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 14),         ///< Invalid data signals delay function specified
    CARRIER_INVALID_INTSTRUCT           = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 15),         ///< Invalid module Nutaq internal data structure version
    CARRIER_INVALID_BOTINTSTRUCT        = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 16),         ///< Invalid bottom module Nutaq internal data structure version
    CARRIER_INVALID_ROUTEDPIN           = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 17),         ///< Top connector pin is routed to invalid bottom connector pin of bottom board
    CARRIER_INVALID_I2C_ZONE            = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 18),         ///< The carrier does not support the specified I2C zone
    CARRIER_INVALID_USLEEPFCT           = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 19),         ///< Invalid usleep function provided
    CARRIER_INVALID_FCT                 = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 20),         ///< The function is not supported by the instantiated carrier
    CARRIER_I2C_BUS_TIMEOUT             = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 21),         ///< I2C bus request or release function has timeout
    CARRIER_I2C_TIMEOUT                 = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 22),         ///< I2C transaction timeout
    CARRIER_INVALID_CLOCKCONFIG         = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 23),         ///< Clock configuration not supported
    CARRIER_INVALID_100MOSCSTATE        = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 24),         ///< The specified configuration does not match with the current 100 MHz oscillator state
    CARRIER_INVALID_MODULE              = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 25),         ///< The module is not supported by the instantiated carrier
    CARRIER_INVALID_MODULE_INDEX        = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 26),         ///< This module index is not supported by the instantiated carrier
    CARRIER_INVALID_INDEX               = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 27),         ///< This index is not supported by the instantiated carrier
    CARRIER_INVALID_VADJSETTING         = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 28),         ///< Invalid Vadj setting for the instantiated carrier
    CARRIER_ALLOC_FAILED                = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 29),         ///< Failed to allocate the required memory when opening the carrier instance
    CARRIER_INVALID_VADJREQCONNECTOR    = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 30),         ///< Impossible to set different Vadj values on the same FMC connector
    CARRIER_INVALID_SIGNALPTR           = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 31),         ///< The pointer to the signal structure is not valid.
    CARRIER_INVALID_PTR                 = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 32),         ///< The argument pointer is not valid.
    CARRIER_INVALID_I2C_OPTION          = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 33),         ///< Invalid I2C option
    CARRIER_I2C_FAILED                  = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 34),         ///< I2C transaction failed
    CARRIER_INVALID                     = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 35),         ///< Invalid carrier
    CARRIER_INVALID_MEM_SPACE           = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 36),         ///< The memory space is not supported by the instantiated carrier
    CARRIER_INVALID_I2C_ADDRESS         = MAKE_RESULT(LYR_SEVERITY_ERROR,      MODULE_CARRIER, 37),         ///< The carrier has no device with the specified address in the specified zone
};


#ifdef __cplusplus
}
#endif  // extern "C"
#endif

