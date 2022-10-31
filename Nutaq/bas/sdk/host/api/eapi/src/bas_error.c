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
 * @file      bas_error.c
 *
 * @brief     Error code generation and analysis.
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bas_error.h"

#define MAX_CHARACTER 256

char t_severity[][MAX_CHARACTER] = {"Success code","Informational code","Warning code","Error code"};

char t_module[][MAX_CHARACTER] =
{
	/*0*/ "Generic",                /**< Default software module */
	/*1*/ "I2C",                    /**< I2C software module */
	/*2*/ "I2C_EEPROM : ",          /**< I2C EEPROM software module */
    /*3*/ "AD9510",                 /**< AD9511 software module */
    /*4*/ "AD9511",                 /**< AD9510 software module */
    /*5*/ "AD9517",                 /**< AD9517 software module */
    /*6*/ "FMCEEPROM",              /**< FMCEEPROM software module */
    /*7*/ "FMCEEPROM_EEPROM",       /**< FMCEEPROM eeprom software module */
    /*8*/ "Undefined",              /**< Undefined */
    /*9*/ "CARRIER",                /**< Carrier generic system operation */
    /*A*/ "FMCVITA",                /**< FMC vita system module */
    /*B*/ "Undefined",              /**< Undefined */
    /*C*/ "Undefined",              /**< Undefined */
    /*D*/ "Undefined",              /**< Undefined */
    /*E*/ "Undefined",              /**< Undefined */
    /*F*/ "Undefined",              /**< Undefined */

    /* Host */
    /*10*/ "SMQUADAPI",             /**< SignalMaster Quad host API software module */
    /*11*/ "RFFE",                  /**< RFFE's control API software module */
    /*12*/ "U2C",                   /**< USB to GPIO control API software module (refer to i2cbridge.h) */
    /*13*/ "ADACSYNC",              /**< ADACSync's control API software module */
    /*14*/ "INITLOADER",            /**< Control API software module of the ADACSync's initialization loader */
    /*15*/ "TIME",                  /**< Timer API */
    /*16*/ "LINKEDLIST ",           /**< Linked list API */
    /*17*/ "Undefined",              /**< Undefined */
    /*18*/ "Undefined",              /**< Undefined */
    /*19*/ "Undefined",              /**< Undefined */
    /*1A*/ "Undefined",              /**< Undefined */
    /*1B*/ "Undefined",              /**< Undefined */
    /*1C*/ "Undefined",              /**< Undefined */
    /*1D*/ "Undefined",              /**< Undefined */
    /*1E*/ "Undefined",              /**< Undefined */
    /*1F*/ "Undefined",              /**< Undefined */

    /* DSP internal modules */
    /*20*/ "DMA",                   /**< DSP DMA software module */
    /*21*/ "IRQ",                   /**< DSP IRQ software module */
    /*22*/ "SYS",                   /**< DSP general platform configuration software module */
    /*23*/ "Undefined",              /**< Undefined */
    /*24*/ "Undefined",              /**< Undefined */
    /*25*/ "Undefined",              /**< Undefined */
    /*26*/ "Undefined",              /**< Undefined */
    /*27*/ "Undefined",              /**< Undefined */
    /*28*/ "Undefined",              /**< Undefined */
    /*29*/ "Undefined",              /**< Undefined */
    /*2A*/ "Undefined",              /**< Undefined */
    /*2B*/ "Undefined",              /**< Undefined */
    /*2C*/ "Undefined",              /**< Undefined */
    /*2D*/ "Undefined",              /**< Undefined */
    /*2E*/ "Undefined",              /**< Undefined */
    /*2F*/ "Undefined",              /**< Undefined */

    /* Lyrtech drivers */
    /*30*/ "XEVT",                  /**< DSP external event software module */
    /*31*/ "FBUS",                  /**< DSP FastBus driver */
    /*32*/ "RAVE",                  /**< DSP RAVe driver */
    /*33*/ "CAMLNK",                 /**< DSP Camera Link driver */
    /*34*/ "DRC",                    /**< DSP DRC Virtex-4 driver */
    /*35*/ "TTRFT",                 /**< Twin Tunable RF Transceiver's control API */
    /*36*/ "TWRFT",                 /**< Twin Wimax RF Transceiver's control API */
    /*37*/ "Undefined",              /**< Undefined */
    /*38*/ "Undefined",              /**< Undefined */
    /*39*/ "Undefined",              /**< Undefined */
    /*3A*/ "Undefined",              /**< Undefined */
    /*3B*/ "Undefined",              /**< Undefined */
    /*3C*/ "Undefined",              /**< Undefined */
    /*3D*/ "Undefined",              /**< Undefined */
    /*3E*/ "Undefined",              /**< Undefined */
    /*3F*/ "Undefined",              /**< Undefined */

    /* Utility modules */
    /*40*/ "QUEUE",                 /**< DSP queue management software module */
    /*41*/ "PINGPONG",              /**< DSP ping-pong management software module */
    /*42*/ "RECPLAYTEST",           /**< Recplay test module */
    /*43*/ "CONFIGFILE",            /**< ConfigFile Parser module */
    /*44*/ "Undefined",              /**< Undefined */
    /*45*/ "Undefined",              /**< Undefined */
    /*46*/ "Undefined",              /**< Undefined */
    /*47*/ "Undefined",              /**< Undefined */
    /*48*/ "Undefined",              /**< Undefined */
    /*49*/ "Undefined",              /**< Undefined */
    /*4A*/ "Undefined",              /**< Undefined */
    /*4B*/ "Undefined",              /**< Undefined */
    /*4C*/ "Undefined",              /**< Undefined */
    /*4D*/ "Undefined",              /**< Undefined */
    /*4E*/ "Undefined",              /**< Undefined */
    /*4F*/ "Undefined",              /**< Undefined */

    /* Linux */
    /*50*/ "ADP_PCIE_DRV",
    /*51*/ "ADP_PCIE",
    /*52*/ "ADP_CCE",
    /*53*/ "ADP_EAPI",
    /*54*/ "OSCONFIG",
    /*55*/ "Undefined",              /**< Undefined */
    /*56*/ "Undefined",              /**< Undefined */
    /*57*/ "Undefined",              /**< Undefined */
    /*58*/ "Undefined",              /**< Undefined */
    /*59*/ "Undefined",              /**< Undefined */
    /*5A*/ "Undefined",              /**< Undefined */
    /*5B*/ "Undefined",              /**< Undefined */
    /*5C*/ "Undefined",              /**< Undefined */
    /*5D*/ "Undefined",              /**< Undefined */
    /*5E*/ "Undefined",              /**< Undefined */
    /*5F*/ "Undefined",              /**< Undefined */

    /* ADP */
    /*60*/ "ADAC250",
    /*61*/ "FMC",
    /*62*/ "CLOCK",
    /*63*/ "FMCSPI",
    /*64*/ "RADIO420",
    /*65*/ "LMS6002",
    /*66*/ "CDCE62005",
    /*67*/ "MI250",
    /*68*/ "ADC5000",
    /*69*/ "COMWRAPPER",
    /*6A*/ "Undefined",              /**< Undefined */
    /*6B*/ "RTDEX",
    /*6C*/ "RECPLAY",
    /*6D*/ "MI125",
    /*6E*/ "TIMER",
    /*6E*/ "FMCLVDS",
    /*70*/ "EV10AQ190",
    /*71*/ "LTM9012",
    /*72*/ "AURORA",
	/*73*/ "MI125COMWRAPPER",
	/*74*/ "PPSSYNC",
    /*75*/ "LVDS",
    /*76*/ "MO1000DEVCOM",
    /*77*/ "MO1000EXPANDER",
    /*78*/ "AD9148",
    /*79*/ "CDCE62005P",
    /*7A*/ "MO1000",
    /*7B*/ "Timestamp",
    /*7C*/ "FMCOMMS3",
    /*7D*/ "Radio640",
    /*7E*/ "Radio640DevCom",
    /*7F*/ "Radio640Spi",

    /* Sensors */
    /*80*/ "ADT7411",
    /*81*/ "TM75",
    /*82*/ "LM75",
    /*83*/ "LM75P",
    /*84*/ "INA226",
    /*85*/ "AD9361",
    /*86*/ "Undefined",              /**< Undefined */
    /*87*/ "Undefined",              /**< Undefined */
    /*88*/ "Undefined",              /**< Undefined */
    /*89*/ "Undefined",              /**< Undefined */
    /*8A*/ "Undefined",              /**< Undefined */
    /*8B*/ "Undefined",              /**< Undefined */
    /*8C*/ "Undefined",              /**< Undefined */
    /*8D*/ "Undefined",              /**< Undefined */
    /*8E*/ "Undefined",              /**< Undefined */
    /*8F*/ "Undefined",              /**< Undefined */

    /* IO expanders */
    /*90*/ "PCF8574",
	/*91*/ "PCA9535",
	/*92*/ "Undefined",              /**< Undefined */
    /*93*/ "Undefined",              /**< Undefined */
    /*94*/ "Undefined",              /**< Undefined */
    /*95*/ "Undefined",              /**< Undefined */
    /*96*/ "Undefined",              /**< Undefined */
    /*97*/ "Undefined",              /**< Undefined */
    /*98*/ "Undefined",              /**< Undefined */
    /*99*/ "Undefined",              /**< Undefined */
    /*9A*/ "Undefined",              /**< Undefined */
    /*9B*/ "Undefined",              /**< Undefined */
    /*9C*/ "Undefined",              /**< Undefined */
    /*9D*/ "Undefined",              /**< Undefined */
    /*9E*/ "Undefined",              /**< Undefined */
    /*9F*/ "Undefined",              /**< Undefined */

	/* GNURADIO */
	/*A0*/ "GNURADIO",
	/*A1*/ "Undefined",              /**< Undefined */
    /*A2*/ "Undefined",              /**< Undefined */
    /*A3*/ "Undefined",              /**< Undefined */
    /*A4*/ "Undefined",              /**< Undefined */
    /*A5*/ "Undefined",              /**< Undefined */
    /*A6*/ "Undefined",              /**< Undefined */
    /*A7*/ "Undefined",              /**< Undefined */
    /*A8*/ "Undefined",              /**< Undefined */
    /*A9*/ "Undefined",              /**< Undefined */

    /* RTDEx Sync */
    /*AA*/ "RTDExSync",
    /*AB*/ "RTDExTs",


};

char t_null[][MAX_CHARACTER] =
{
    "Generic success",                                                              // OK
    "The method failed to allocate necessary memory",                               // ERR_OUTOFMEMORY
    "The necessary data is not yet available",                                      // ERR_PENDING
    "The ongoing operation was aborted",                                            // ERR_ABORT
    "General access denied",                                                        // ERR_ACCESSDENIED
    "The method is not implemented",                                                // ERR_NOTIMPLEMENTED
    "The supplied argument is invalid",                                             // ERR_INVARG
    "The supplied pointer is invalid",                                              // ERR_INVPTR
    "The supplied handle is invalid",                                               // ERR_INVHANDLE
    "The time has elapsed",                                                         // ERR_TIMEOUT
    "Resource already allocated",                                                   // ERR_ALRDYALLOC
    "The module was not initialized. Initialize it before calling this function",   // ERR_NOTINIT
    "General task that must be performed only once",                                // ERR_ALRDYDONE
    "The targeted hardware module was not detected"                                 // ERR_NOHWMODULE
};

char t_i2c[][MAX_CHARACTER] =
{
	"Success.",                             //I2C_OK
	"Error.",                               //I2C_ERROR
	"This feature is not implemented.",     //I2C_UNIMPLEMENTED
	"Transfer size is too large.",          //I2C_TOOLARGE
	"Error during I2C transfer.",           //I2C_FAILED
	"Timeout."                              //I2C_TIMEOUT
};

char t_ad9517[][MAX_CHARACTER] =
{
	"Success.",                                                                                 //AD9517OK
	"Overflow during calculation.",                                                             //AD9517OVERFLOW
	"Input divider.",                                                                           //AD9517_INPUT_DIVIDER
	"Requested output frequency is too low.",                                                   //AD9517FREQLOW
	"Requested output frequency is too high.",                                                  //AD9517FREQHIGH
	"There is no possible configuration that will generate the requested parameters.",          //AD9517_NO_VALID_SOLUTION
	"Requested output frequency impossible because output divider would be too large.",         //AD9517_DIVIDER
	"PFP frequency is out of bound.",                                                           //AD9517_PFDFREQ_OUT_OF_RAN
	"Impossible R divider, reference frequency is too large or PFD frequency is too small.",    //AD9517_NO_VALID_R
	"Reference frequency is too large.",                                                        //AD9517_REF_CLOCK_TOO_HIGH
	"VCO range constraint is out of bound.",                                                    //AD9517_VCO_OUT_OF_RANGE
	"VCO divider is out of range."                                                              //AD9517_VCODIVIDER
};

char t_fmceeprom[][MAX_CHARACTER] =
{
	"Success.",                                                                         //FMCEEPROM_OK
	"Incorrect fru size, cannot decode.",                                               //FMCEEPROM_ERR_OTHER
	"Header chksum error.",                                                             //FMCEEPROM_ERR_HDR_CHK
	"Internal section checksum.",                                                       //FMCEEPROM_ERR_IA_CHK
	"Board chksum error.",                                                              //FMCEEPROM_ERR_BD_CHK
	"Product chksum error.",                                                            //FMCEEPROM_ERR_PR_CHK
	"Chassis chksum error.",                                                            //FMCEEPROM_ERR_CH_CHK
	"Multirecord Header chksum error.",                                                 //FMCEEPROM_ERR_MLHDR_CHK
	"Multirecord data chksum error.",                                                   //FMCEEPROM_ERR_MLDAT_CHK
	"Typelen error in a field.",                                                        //FMCEEPROM_ERR_TYPE
	"Overflow error.",                                                                  //FMCEEPROM_ERR_OVERFLOW
	"Language dependant field (ASCII) cannot have a single character.",                 //FMCEEPROM_ERR_TLEN_ASC
	"Cannot decode the specified type.",                                                //FMCEEPROM_ERR_DEC_TYPE
	"Invalid oem data len.",                                                            //FMCEEPROM_VITA_INVALIDOEM_DATALEN
	"Unknown subtype 0 version.",                                                       //FMCEEPROM_VITA_UNKNOWN_SUBT0_VER
	"Invalid oem ascii 6 bits fielUnknown subtype 0 version.",                          //FMCEEPROM_VITA_INVALID_OEM_ASCII6
	"Invalid oem i2c character.",                                                       //FMCEEPROM_VITA_INVALID_I2C_CHAR
	"Unknown subtype version.",                                                         //FMCEEPROM_VITA_UNKNOWN_SUBT1_VER
	"Unkwonw vita oem subtype.",                                                        //FMCEEPROM_VITA_UNKNOWN_SUBT
	"Invalid vita oem data module size.",                                               //FMCEEPROM_VITA_INVALIDMOD_SIZE
	"Invalid vita oem p1 connector module size.",                                       //FMCEEPROM_VITA_INVALIDP1CON_SIZE
	"Invalid vita oem p2 connector module size.",                                       //FMCEEPROM_VITA_INVALIDP2CON_SIZE
	"Invalid vita oem p1 bank A number of signals pairs.",                              //FMCEEPROM_VITA_INVALIDP1CON_ASIGN
	"Invalid vita oem p1 bank B number of signals pairs.",                              //FMCEEPROM_VITA_INVALIDP1CON_BSIGN
	"Invalid vita oem p2 bank A number of signals pairs.",                              //FMCEEPROM_VITA_INVALIDP2CON_ASIGN
	"Invalid vita oem p2 bank B number of signals pairs.",                              //FMCEEPROM_VITA_INVALIDP2CON_BSIGN
	"Invalid TCK clock value.",                                                         //FMCEEPROM_VITA_INVALIDTCK_VAL
	"Invalid vita oem p1 gbt signals.",                                                 //FMCEEPROM_VITA_INVALIDP1CON_GBT
	"Invalid vita oem p2 gbt signals.",                                                 //FMCEEPROM_VITA_INVALIDP2CON_GBT
	"Invalid vita reserved field 1.",                                                   //FMCEEPROM_VITA_RESERVED1
	"EEprom format is non standard fmc vita (spd decode).",                             //FMCEEPROM_SPD_DECODE
	"EEprom internal section size check failed.",                                       //FMCEEPROM_ERR_INTERNAL_SIZECHK
	"EEprom internal section size check type 2 failed.",                                //FMCEEPROM_ERR_INTERNAL_SIZECHK2
	"EEprom chassis section unknown chassis type.",                                     //FMCEEPROM_ERR_CHASSIS_TYPE
	"EEprom chassis typelen field error.",                                              //FMCEEPROM_ERR_CHASSIS_TYPELEN
	"EEprom chassis too many typelen fields.",                                          //FMCEEPROM_ERR_CHASSIS_MANYFILEDS
	"EEprom board typelen field error.",                                                //FMCEEPROM_ERR_BOARD_TYPELEN
	"EEprom board too many typelen fields.",                                            //FMCEEPROM_ERR_BOARD_MANYFILEDS
	"EEprom product typelen field error.",                                              //FMCEEPROM_ERR_PRODUCT_TYPELEN
	"EEprom product too many typelen fields.",                                          //FMCEEPROM_ERR_PRODUCT_MANYFILEDS
	"Invalid vita requierement level specified."                                        //FMCEEPROM_ERR_INVALID_VITA_REQ_LEVEL
};

char t_fmceeprom_eeprom[][MAX_CHARACTER] =
{
	"Success.",                                                                         //FMCEEPROM_EEPROM_OK
	"Invalid eeprom id given.",                                                         //FMCEEPROM_ERR_INVALID_EEPROMNUM
	"EEprom read size error.",                                                          //FMCEEPROM_EEPROM_ERR_READSIZE
	"EEprom write size error.",                                                         //FMCEEPROM_EEPROM_ERR_WRITESIZE
	"Invalid eeprom type.",                                                             //FMCEEPROM_ERR_INVALID_EEPROMTYPE
	"Invalid vita identification filed used in matching condition.",                    //FMCEEPROM_ERR_INVALID_IDENTVITAFIELD
	"Wrong eeprom decoding type specified in matching condition.",                      //FMCEEPROM_ERR_INVALID_IDENTEEDECODE
	"Error, could not read all available eeprom (some maybe unidentified).",            //FMCEEPROM_ERR_COULDNOTREADALLEEPROM
	"I2C peripheral busy timeout.",                                                     //FMCEEPROM_ERR_I2CWAIT_TIMEOUT
	"Unknown eeprom type error while reading.",                                         //FMCEEPROM_ERR_UNKNOWN_EEPROMTYPE
	"Unknown module type, not identified correctly.",                                   //FMCEEPROM_ERR_UNKNOWN_MODULETYPE
	"Unknown module eeprom decoded format.",                                            //FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT
	"Invalid vadj information discovered (or not present).",                            //FMCEEPROM_ERR_INVALID_VADJINFO
	"Invalid module connector info specified.",                                         //FMCEEPROM_ERR_INVALID_CONNINFO
	"Invalid module id string information.",                                            //FMCEEPROM_ERR_INVALID_MODID_INFO
	"Invalid part number information.",                                                 //FMCEEPROM_ERR_INVALID_PARTNUM_INFO
	"Invalid serial number information.",                                               //FMCEEPROM_ERR_INVALID_SERIALNUM_INFO
	"Invalid hardware revision information.",                                           //FMCEEPROM_ERR_INVALID_HARDREV_INFO
	"Invalid firmware revision information.",                                           //FMCEEPROM_ERR_INVALID_FIRMREV_INFO
	"Invalid internal custom section.",                                                 //FMCEEPROM_ERR_INVALID_INTERNAL_INFO
	"Invalid manufacturer information."                                                 //FMCEEPROM_ERR_INVALID_MANUF_INFO
};

char t_carrier[][MAX_CHARACTER] =
{
	"Success.",                                                                             //CARRIER_SERVICE_OK
	"Carrier instance was not opened correctly.",                                           //CARRIER_SERVICE_NOT_OPEN
	"Invalid carrier connector/board assignment function specified.",                       //CARRIER_SERVICE_INVALID_ASSGFCT
	"Invalid carrier connector/board Vadj function specified.",                             //CARRIER_SERVICE_INVALID_VADJFCT
	"Invalid timer function specified.",                                                    //CARRIER_SERVICE_INVALID_TIMERFCT
	"Board type does not exist in condition list.",                                         //CARRIER_SERVICE_BOARDTYPE_NOTEXIST
	"Carrier discover function was not ran correctly.",                                     //CARRIER_SERVICE_NODISCOVER
	"No valid module detected, cannot set Vadj.",                                           //CARRIER_SERVICE_VADJ_NOVALIDMODULE
	"Invalid slot specified for this carrier.",                                             //CARRIER_SERVICE_INVALIDSLOT
	"Invalid board specified for this carrier.",                                            //CARRIER_SERVICE_INVALIDBOARD
	"Invalid board type list.",                                                             //CARRIER_SERVICE_INVALIDBOARDTYPELST
	"Invalid string provided.",                                                             //CARRIER_SERVICE_INVALIDSTRING
	"Invalid board info type specified.",                                                   //CARRIER_SERVICE_BADINFOTYPE
    "Slot neighbor board problem, cannot set Vadj",                                         //CARRIER_SERVICE_VADJ_NEIGHBORBAD
    "Invalid data signals delay function specified",                                        //CARRIER_SERVICE_INVALID_SIGNALFCT
    "Invalid module Nutaq internal data structure version",                                 //CARRIER_SERVICE_INVALID_INTSTRUCT
    "Invalid bottom module Nutaq internal data structure version",                          //CARRIER_SERVICE_INVALID_BOTINTSTRUCT
    "Top connector pin is routed to invalid bottom connector pin of bottom board",          //CARRIER_SERVICE_INVALID_ROUTEDPIN
    "The carrier does not support the specified I2C zone",                                  //CARRIER_INVALID_I2C_ZONE
    "Invalid usleep function provided",                                                     //CARRIER_INVALID_USLEEPFCT
    "The function is not supported by the instantiated carrier",                            //CARRIER_INVALID_FCT
    "I2C bus request or release function has timeout",                                      //CARRIER_I2C_BUS_TIMEOUT
    "I2C transaction timeout",                                                              //CARRIER_I2C_TIMEOUT
    "Clock configuration not supported",                                                    //CARRIER_INVALID_CLOCKCONFIG
    "The specified configuration does not match with the current 100 MHz oscillator state", //CARRIER_INVALID_100MOSCSTATE
    "The module is not supported by the instantiated carrier",                              //CARRIER_INVALID_MODULE
    "This module index is not supported by the instantiated carrier",                       //CARRIER_INVALID_MODULE_INDEX
    "This index is not supported by the instantiated carrier",                              //CARRIER_INVALID_INDEX
    "Invalid Vadj setting for the instantiated carrier",                                    //CARRIER_INVALID_VADJSETTING
    "Failed to allocate the required memory when opening the carrier instance",             //CARRIER_ALLOC_FAILED
    "Impossible to set different Vadj values on the same FMC connector",                    //CARRIER_INVALID_VADJREQCONNECTOR
    "The pointer to the signal structure is not valid.",                                    //CARRIER_INVALID_SIGNALPTR
    "The argument pointer is not valid.",                                                   //CARRIER_INVALID_PTR
    "Invalid I2C option",                                                                   //CARRIER_INVALID_I2C_OPTION
    "I2C transaction failed",                                                               //CARRIER_I2C_FAILED
    "Invalid carrier",                                                                      //CARRIER_INVALID
    "The memory space is not supported by the instantiated carrier"                         //CARRIER_INVALID_MEM_SPACE
};

char t_recplaytest[][MAX_CHARACTER] =
{
	"Success.",                                                 //RECPLAYTESTOK
	"Record/Playback test trigger position is invalid."         //RECPLAYTESTINVTRIGGER
};

char t_adp_cce[][MAX_CHARACTER] =
{
	"Success.",                                                                 //CCEOK
	"TCP error.",                                                               //CCETCPERROR
	"Unimplemented.",                                                           //CCEUNIMPLEMENTED
	"IOCT error.",                                                              //CCEIOCTLERROR
	"Invalid value.",                                                           //CCEINVALIDVALUE
	"Memory failed.",                                                           //CCEMEMORY
	"Buffer is too big.",                                                       //CCEBUFFER2BIG
	"Invalid mod.",                                                             //CCEINVALIDMOD
	"Invalid reg.",                                                             //CCEINVALIDREG
    "Invalid index.",                                                           //CCEINVALIDINDEX
    "Invalid bitstream.",                                                       //CCEINVALIDBITSTREAM
    "Invalid flash partition table. Try updating the system DTB and JFFS2."     //CCEINVALIDPARTITIONTABLE
};

char t_adp_eapi[][MAX_CHARACTER] =
{
	"Success.",                                                                                         //EAPIOK
	"Cannot connect to CCE.",                                                                           //EAPICONNECT
	"Cannot create TCP socket.",                                                                        //EAPISOCKET
	"Cannot access file, file not found.",                                                              //EAPIFILE
	"mmap call failed.",                                                                                //EAPIMMAP
	"CCE connection terminated.",                                                                       //EAPIDISCONNECT
	"TCP stream to CCE is out of synch.",                                                               //EAPIOUTOFSYNC
	"Error while starting WSA.",                                                                        //EAPIWSA
	"Not enough available memory.",                                                                     //EAPINOMEM
	"The file is too large.",                                                                           //EAPIFILETOOLARGE
	"The cce version doesn't match the EAPI version.",                                                  //EAPICCEVERSIONMISMATCH
    "The cce version doesn't match the EAPI version but a limited access to the CCE is possible.",      //EAPICCELIMITEDACCESS
    "The carrier board has not been correctly detected but a limited access to the CCE is possible."    //EAPICARRIERLIMITEDACCESS
};

char t_osconfig[][MAX_CHARACTER] =
{
	"Succes.",              //OSCONFIG_SUCCESS
	"Bad parameter.",       //OSCONFIG_BADPARAMETER
	"Bad value.",           //OSCONFIG_BADVALUE
	"Bad file",             //OSCONFIG_BADFILE
	"Not loaded"            //OSCONFIG_NOTLOADED
};

char t_adac250[][MAX_CHARACTER] =
{
	"Success.",                                                                 //ADAC250_OK
	"Cannot calculate Fvco value.",                                             //ADAC250_NO_VALID_DIVIDER
	"No valid R divider value.",                                                //ADAC250_NO_VALID_R
	"Reference clock frequency is too high.",                                   //ADAC250_REF_CLOCK_TO_HIGH
	"Phase frequency detector out of range.",                                   //ADAC250_PFDFREQ_OUT_OF_RANGE
	"No valid PLL parameter found given current constraints.",                  //ADAC250_NO_VALID_PLL_SOLUTION
	"VCO is out of valid range.",                                               //ADAC250_VCO_OUT_OF_RANGE
	"Running PLL out of specified range; PFD is smaller than 100 kHz.",         //ADAC250_PFDFREQ_OUT_OF_SPEC
	"Invalid mux configuration.",                                               //ADAC250_MUXERROR
	"D/A converter not ready.",                                                 //ADAC250_DACTIMEOUT
	"Generated frequency is too low.",                                          //ADAC250_FREQLOW
	"Overflow during calculation.",                                             //ADAC250_OVERFLOW
	"PLL not locked.",                                                          //ADAC250_PLLNOTLOCKED
	"Trigger delay value is invalid (Valid values are 1 to 32).",               //ADAC250_INVALID_TRIGGER_DELAY
	"Invalid pointer.",                                                         //ADAC250_INVALID_POINTER
	"A function argument is out of its valid range."                            //ADAC250_ARG_OUT_OF_RANGE
};

char t_fmc[][MAX_CHARACTER] =
{
	"Success.",             //FMCOK
	"Fmc is absent.",       //FMCABSENT
	"Vadj error."           //FMCVADJERROR
};

char t_fmcspi[][MAX_CHARACTER] =
{
	"Success.",                                 //FMCSPIOK
	"time out while waiting for SPI core."      //FMCSPITIMEOUT
};

char t_radio420[][MAX_CHARACTER] =
{
	"Success.",                                                                 //FMCRADIOOK
	"Timed out while waiting for the SPI core.",                                //FMCRADIOIVAL
	"Command is unsupported by device.",                                        //FMCRADIONOTSUPPORTED
	"No FMC card is present.",                                                  //FMCRADIOABSENT
	"Timed out while reading error value function from calibration core.",      //FMCRADIOERFTIMEOUT
	"Back off compensation is out of range.",                                   //FMCRADIOBACKOFF
	"RX DC calibration failed.",                                                //FMCRADIODCCALFAILED
	"This board revision is not supported by the library.",                     //FMCRADIOREVISION
	"RX RF front-end DC compensation is at maximum.",                           //FMCRADIODCCALOVERANGE
	"The PLL did not lock and the timeout was reached.",                        //FMCRADIOPLLUNLOCKED
	"A function argument is out of its valid range.",                           //FMCRADIOARGOUTOFRANGE
	"Invalid pointer."                                                          //FMCRADIOINVPTR
};

char t_lms6002[][MAX_CHARACTER] =
{
	"Success.",                                                                 //LMS6002OK
	"Invalid parameters.",                                                      //LMS6002IVAL
	"Functionality is not implemented.",                                        //LMS6002UNIMPLEMENTED
	"Cannot tune to selected frequency.",                                       //LMS6002TUNINGERROR
	"PLL calibration failed.",                                                  //LMS6002GENERALERROR
	"Communication with LMS6002 has failed.",                                   //LMS6002ABSENT
	"DC offset calibration has failed.",                                        //LMS6002DCCALFAILED
	"RXVGA DC offset calibration has failed.",                                  //LMS6002RXVGA2CALIB
	"PLL is unlocked.",                                                         //LMS6002PLLUNLOCKED
	"Frequency is too high."                                                    //LMS6002MULTIPLIER
};

char t_cdce62005[][MAX_CHARACTER] =
{
	"Success.",                                                                                             //CDCE62005OK
	"Time out while waiting for SPI core.",                                                                 //CDCE62005IVAL
	"This functionality is not yet implemented.",                                                           //CDCE62005UNIMPLEMENTED
	"Requested frequencies cannot be generated because of divider limitation.",                             //CDCE62005DIVIDER
	"Requested frequencies cannot be generated.",                                                           //CDCE62005_NOSOLUTION
	"Requested frequencies cannot be generated because of phase detector limitation.",                      //CDCE62005_PFDIMPOSSIBLE
	"Requested frequencies cannot be generated because of prescaler limitation.",                           //CDCE62005_NOPRESCALER
	"Invalid prescaler.",                                                                                   //CDCE62005_INVALIDPRESCALER
	"Requested frequencies are invalid. Only 2 uniques frequencies can be requested at this time.",         //CDCE62005_INVALIDFREQ
	"VCO freq is too high.",                                                                                //CDCE62005_OUTPUTDIVIMPOSSIBLE
	"Overflow during calculation."                                                                          //CDCE62005_OVERFLOW
};

char t_mi250[][MAX_CHARACTER] =
{
	"Success.",                                                             //MI250OK
 	"Cannot calculate Fvco Value.",                                         //MI250_NO_VALID_DIVIDER
	"No valid R divider value.",                                            //MI250_NO_VALID_R
	"Reference clock frequency is too high.",                               //MI250_REF_CLOCK_TO_HIGH
	"Phase Frequency Detector frequency is out of range.",                  //MI250_PFDFREQ_OUT_OF_RANGE
	"No valid PLL parameter found given current constraints.",              //MI250_NO_VALID_PLL_SOLUTION
	"VCO is out of valid range.",                                           //MI250_VCO_OUT_OF_RANGE
	"Running PLL out of specification; PFD is smaller than 1 MHz.",         //MI250_PFDFREQ_OUT_OF_SPEC
	"invalid mux configuration.",                                           //MI250MUXERROR
	"Invalid clock source.",                                                //MI250INVALIDCLOCKSOURCE
	"ADC write errors.",                                                    //MI250ADCSETUPERRORS
	"FPGA core is absent.",                                                 //MI250COREABSENT
	"Failed to calibrate ADC data ports for given frequency.",              //MI250CALIBRATIONFAILURE
	"The pointer to the clock master calibration structure is not set.",    //MI250MASTERCALIBPTRINV
	"The clock master board need to be calibrated before the clock slave modules are calibrated." //MI250MASTERCALIBNOTDONE
};

char t_adc5000[][MAX_CHARACTER] =
{
	"Success.",                                                     //ADC5000OK
	"Digital calibration with the ADC5000 data stream failed.",     //ADC5000ALIGN
	"Invalid parameters.",                                          //ADC5000IVAL
	"PLL is not locked.",                                           //ADC5000PLLUNSTABLE
	"Core absent.",                                                 //ADC5000COREABSENT
	"FMC absent.",                                                  //ADC5000FMCABSENT
	"Invalid trigger delay value.",                                 //ADC5000_INVALID_TRIGGER_DELAY
	"Communication error with the PLL chip",                        //ADC5000_ERROR_COMM_PLL
	"Communication error with the ADC chip"                         //ADC5000_ERROR_COMM_ADC
};

char t_comwrapper[][MAX_CHARACTER] =
{
	"Success.",         //COMWRAPPEROK
	"Timeout.",         //COMWRAPPERTIMEOUT
	"Released."         //COMWRAPPERRELEASE
};

char t_rtdex[][MAX_CHARACTER] =
{
	"Success.", //0                                                     //RTDEXOK
	"Invalid RTDEx Handle is used.",                                    //RTDEXINVHANDLE
	"Invalid RTDEx context.",                                           //RTDEXINVCONTEXT
	"Memory allocation failed for RTDEx object.",                       //RTDEXMEMALLOC
	"Channel number used is not available on FPGA.",                    //RTDEXINVCHANNELNUM
	"Invalid RTDEx mutex handle.",                                      //RTDEXINVMUTEXHANDLE
	"Mutex error - init.",                                              //RTDEXMUTEXINIT
	"Mutex error - lock.",                                              //RTDEXMUTEXLOCK
	"Mutex error - unlock.",                                            //RTDEXMUTEXUNLOCK
	"RTDEx initial condition.",                                         //RTDEXCONDINIT
	"RTDEx waiting condition.",//10                                     //RTDEXCONDWAIT
	"RTDEx thread array.",                                              //RTDEXPTHREADARRAY
	"Invalid frame size is used.",                                      //RTDEXINVFRAMESIZE
	"Transfer size is not a multiple of frame size.",                   //RTDEXTRANSFERSIZENOTFRAM
	"Buffer size is too small.",                                        //RTDEXBUFFERSIZETOOSMALL
	"Wait thread call.",                                                //RTDEXWAITTHREADCALL
	"RTDEx channel handle is being used in the wrong direction.",       //RTDEXWRONGDIRECTION
	"PCAP compile error.",                                              //RTDEXPCAPCOMPILE
	"PCAP receive filter error.",                                       //RTDEXPCAPSETFILTER
	"PCAP could not verify network device.",                            //RTDEXPCAPFINDALLDEVS
	"Could not verify network interface.",//20                          //RTDEXINTERFACEITERATOR
	"Could not verify network device.",                                 //RTDEXDEVICEITERATOR
	"Could not get network adaptors info.",                             //RTDEXADAPTORSINFO
	"Thread is dead.",                                                  //RTDEXTHREADISDEAD
	"Could match networkdevice to MAC address.",                        //RTDEXINVINTERFACEDMAHAND
	"Network interface used is not valid.",                             //RTDEXINVSOURCEINTERFACE
	"Packet send error.",                                               //RTDEXSENDPACKET
	"Media not supported.",                                             //RTDEXMEDIANOTSUPPORTED
	"Invalid data buffer.",                                             //RTDEXINVALIDBUFFER
	"Invalid buffer size.",                                             //RTDEXINVALIDBUFFERSIZE
	"Frame size is too big.",//30                                       //RTDEXFRAMESIZETOOBIG
	"PCAP send error.",                                                 //RTDEXPCAPSEND
	"Invalid RTDEX core ID/Ver.",                                       //RTDEXBADCOREIDORVER
	"requested ressource busy.",                                        //RTDEXRESSOURCEBUSY
	"invalid bus access handle.",                                       //RTDEXINVALBUSACCESS
	"transfert size too big.",                                          //RTDEXTRANSFERTSIZETOOBIG
	"too many perseus devices.",                                        //RTDEXTOOMANYDEVS
	"invalid ressource #.",                                             //RTDEXINVALRESSOURCE
	"invalid Host MAC addr.",                                           //RTDEXINVALHOSTMACADDR
	"target MAC address conflict.",                                     //RTDEXPEERMACADDRCONFLICT
	"PCIe perseus to cpu driver command error.",//40                    //RTDEXPCIEPERSEUS2CPUFAIL
	"PCIe internal error.",                                             //RTDEXPCIEINTERR
	"PCIe system error.",                                               //RTDEXPCIESYSERR
	"PCIe system error - open.",                                        //RTDEXPCIEOPEN
	"PCIe system error - read.",                                        //RTDEXPCIEREAD
	"PCIe system error - write.",                                       //RTDEXPCIEWRITE
	"PCIe system error - ioctl.",                                       //RTDEXPCIEIOCTL
	"Not defined.",                                                     // 0x2F
	"PCIe end of file reading or writing.",	                            //RTDEXPCIEEOF
	"PCIe timeout reading or writing.",                                 //RTDEXPCIETIMEOUT
	"Receive size to large.",//50                                       //RTDEXRECEIVESENDSIZEISTO
	"Not supported (Perseus to Perseus - old eth interface).",          //RTDEXOLDETHNOTSUPPORTED
	"Bad Test Logic Core-Id.",                                          //RTDEXBADTESTLOGICCOREID
	"TCP/IP - function WSAStartup.",                                    //RTDEXTCPIPWSASTART
	"TCP/IP - Cannot open socket.",                                     //RTDEXTCPIPSOCKET
	"TCP/IP - function setsockopt.",                                    //RTDEXTCPIPSETSOCK
	"TCP/IP - function gethostbyname.",                                 //RTDEXTCPIPSERVER
	"TCP/IP - Cannot connect to test server.",                          //RTDEXTCPIPCONNECT
	"TCP/IP - Error sending. Got disconnected?",                        //RTDEXTCPIPSEND
	"TCP/IP - Error receiving. Got disconnected?",                      //RTDEXTCPIPRECEIVE
	"AXI - cannot open RTDEx character device.",//60                    //RTDEXAXIOPEN
	"AXI - error writing to FIFO.",                                     //RTDEXAXISEND
	"AXI - error reading from FIFO.",                                   //RTDEXAXIRECEIVE

	// 0x80
	"PCIe kernel driver - FIFO not connected.",                         //rtdxpkdrv_FIFO_NOT_CONNECTED
	"PCIe kernel driver - Bad FIFO number.",                            //rtdxpkdrv_BAD_FIFO_NO
	"PCIe kernel driver - No free AXI2BAR.",                            //rtdxpkdrv_NO_FREE_AXI2BAR
	"PCIe kernel driver - Bad channel number.",                         //rtdxpkdrv_BAD_CHNL_NO
	"PCIe kernel driver - DMA allocation failed.",                      //rtdxpkdrv_DMA_ALLOC_FAIL
	"PCIe kernel driver - No free channel.",                            //rtdxpkdrv_NO_FREE_CHANNEL
	"PCIe kernel driver - FIFO is busy.",                               //rtdxpkdrv_FIFO_BUSY
	"PCIe kernel driver - Bad frame size.",                             //rtdxpkdrv_BAD_FRAME_SIZE
	"PCIe kernel driver - Bad transfer size.",                          //rtdxpkdrv_BAD_TRANSFER_SIZE
	"PCIe kernel driver - Internal error.",                             //rtdxpkdrv_INTERNAL_ERROR
	"PCIe kernel driver - Missing parameters.",                         //rtdxpkdrv_MISSING_PARAMETER
	"PCIe kernel driver - Bad address.",                                //rtdxpkdrv_BAD_ADDRESS
	"PCIe kernel driver - Unknown command.",                            //rtdxpkdrv_UNKNOW_COMMAND
	"PCIe kernel driver - Ouput buffer is too small.",                  //rtdxpkdrv_OUTPUT_BUFFER_TOO_SMALL
	"PCIe kernel driver - Bad channel number.",                         //rtdxpkdrv_BAD_CHANNEL_NO
	"PCIe kernel driver - Channel is not connected.",                   //rtdxpkdrv_CHANNEL_NOT_CONNECTED
	"PCIe kernel driver - Bad channel type.",                           //rtdxpkdrv_BAD_CHANNEL_TYPE
	"PCIe kernel driver - Bad destination device number.",              //rtdxpkdrv_BAD_DEST_DEV_NO
	"PCIe kernel driver - Bad destination FIFO number.",                //rtdxpkdrv_BAD_DEST_FIFO_NO
	"PCIe kernel driver - Bad source FIFO number.",                     //rtdxpkdrv_BAD_SRC_FIFO_NO
	"PCIe kernel driver - Is still running.",                           //rtdxpkdrv_STILL_RUNNING
	"PCIe kernel driver - Input buffer is too bug.",                    //rtdxpkdrv_INPUT_BUFFER_TOO_BIG
	"PCIe kernel driver - DM reset timeout.",                           //rtdxpkdrv_DMRESET_TIMEOUT
	"PCIe kernel driver - CDMA reset timeout.",                         //rtdxpkdrv_CDMARESET_TIMEOUT
	"PCIe kernel driver - No command."                                  //rtdxpkdrv_NO_COMMAND
};

char t_recplay[][MAX_CHARACTER] =
{
	"Success.",                                                     //RECPLAYOK
	"Record/Playback handle used is invalid.",                      //RECPLAYINVHANDLE
	"Error allocating memory for Record/Playback object.",          //RECPLAYMEMALLOC
	"Invalid size is used.",                                        //RECPLAYINVARGSIZE
	"Invalid trigger offset calculated.",                           //RECPLAYINVTRIGOFFSET
	"Invalid trigger delay configured.",                            //RECPLAYINVDELAY
	"Wrong core ID read.",                                          //RECPLAYWRONGCOREID
	"Invalid start address.",                                       //RECPLAYINVSTARTADDR
	"Getting status has timeout.",                                  //RECPLAYSTATUSTIMEOUT
	"Invalid number of ports.",                                     //RECPLAYINVNUMPORT
	"Invalid ports width.",                                         //RECPLAYINVDATAPORT
	"Invalid DDR3 size.",                                           //RECPLAYINVDDR3
    "The waiting process has time outed before the transfer was done." //RECPLAYTRANSFERTIMEOUT
};

char t_mi125[][MAX_CHARACTER] =
{
	"Success.",                                                             // MI125_MI125OK
	"FPGA MI125 core not detected correctly.",                              // MI125_MI125NOCORE
	"FPGA idelayrdy problem.",                                              // MI125_MI125IDELAYRDY
	"FPGA trainingdone problem.",                                           // MI125_MI125TRAININGDONE
	"mi125_reset function has never been done and bad configuration.",      // MI125_MI125NORESET
	"invalid clk fmc bottom source for main fmc board.",                    // MI125_MI125INVALIDCLKBOT
	"invalid clk source.",                                                  // MI125_MI125INVALIDCLK
	"invalid trig out value.",                                              // MI125_MI125INVALIDTRIG
	"invalid active channel group configuration.",                          // MI125_MI125INVALIDCH
	"invalid lvds output configuration.",                                   // MI125_MI125INVALIDLVDS
	"invalid output randomize mode.",                                       // MI125_MI125INVALIDRAN
	"invalid output binary mode configuration.",                            // MI125_MI125INVALIDBIN
	"invalid test mode provided.",                                          // MI125_MI125INVALIDTEST
	"last calibration was invalid.",                                        // MI125_MI125NOCALIB
	"invalid adc parameter used.",                                          // MI125_MI125INVALIDADC
	"test mode is active (preventing this function).",                      // MI125_MI125TESTMODEACT
	"invalid timer user function specified.",                               // MI125_MI125BADUSECFCT
	"No FMC card is present.",                                              // MI125_MI125ABSENT
	"No Adc clock mmcm lock.",                                              // MI125_MI125MMCMLOCK
	"Invalid clk source for top secondary fmc board.",                      // MI125_MI125INVALIDCLKTOP
    "Invalid bottom board data reference tap delay pointer",                // MI125_MI125BADBOTTAPREFPTR
    "Invalid bottom board data reference tap delay value",                  // MI125_MI125BADBOTTAPREFVALUE
    "Invalid bottom board reference delay pointer",                         // MI125_MI125BADBOTREFDELAYPTR
    "Invalid bottom board dco delay pointer"                                // MI125_MI125BADBOTDCODELAYPTR
};

char t_fmclvds[][MAX_CHARACTER] =
{
	"Success.",                                              //FMCLVDSOK
	"Invalid value.",                                        //FMCLVDSIVAL
	"MMC I2C bus release timeout.",                          //FMCLVDSMMCRELEASE
	"MMC I2C bus request timeout.",                          //FMCLVDSMMCREQUEST
	"FMC LVDS FPGA core or FMC LVDS card is not present."    //FMCLVDSABSENT
};

char t_mi125comwrapper[][MAX_CHARACTER] =
{
	"Success.",                                         //MI125_COMWRAPPER_OK
    "Com direction with this device is UNIMPLEMENTED.", //MI125_COMWRAPPER_UNIMPLEMENTED
    "Error during I2C transfer .",                      //MI125_COMWRAPPER_FAILED
    "I2C bus busy timeout.",                            //MI125_COMWRAPPER_TIMEOUT
    "I2C bus ownership request timeout .",              //MI125_COMWRAPPER_RQTIMEOUT
    "Invalid data buffer given.",                       //MI125_COMWRAPPER_INVALIDDATABUF
    "I2C bus busy before operation timeout.",           //MI125_COMWRAPPER_BTIMEOUT
    "I2C bus ownership release timeout."                //MI125_COMWRAPPER_RLTIMEOUT
};

char t_aurora[][MAX_CHARACTER] =
{
    "Success.",                              //AURORAOK
    "Aurora core is present.",               //AURORA_INVALID_CORE
    "Aurora core is not open.",              //AURORA_NOT_OPEN
    "Aurora core is already open.",          //AURORA_ALREADY_OPEN
    "Aurora channel is down.",               //AURORA_CHANNEL_DOWN
    "Aurora invalid argument."               //AURORA_INVALID_ARG
};

char t_ev10aq190[][MAX_CHARACTER] =
{
	"Success.",                             //EV10AQ190OK
	"Calibration is not finished.",         //EV10AQ190_CALIB_TIMEOUT
	"Invalid parameter."                    //EV10AQ190_IVAL
};

char t_ltm9012[][MAX_CHARACTER] =
{
	"Success.",                             //MI125_LTM9012OK
	"Invalid channel mode parameter.",      //MI125_LTM9012_IVAL
	"Invalid lvds parameter."               //MI125_LTM9012_ILVDSVAL
};

char t_adt7411[][MAX_CHARACTER] =
{
	"Success.",                             //ADT7411OK
	"This feature is not implemented.",     //ADT7411UNIMPLEMENTED
	"Invalid parameters."                   //ADT7411IVAL
};

char t_lm75[][MAX_CHARACTER] =
{
	"Success."      // MI125_LM75OK
};

char t_pcf8574[][MAX_CHARACTER] =
{
	"Success."      // FMCLVDS_PCF8574OK
};

char t_pca9535[][MAX_CHARACTER] =
{
	"Success."     //MI125_PCA9535OK
};

char t_ppssync[][MAX_CHARACTER] =
{
	"Success.",                                         //PPSSYNCOK
	"FPGA core is absent ",                             //PPSSYNCCOREABSENT
	"DAC value is out of range.",                       //PPSSYNCDACOUTOFRANGE
    "No PPS present."  ,                                //PPSSYNCPPSADSENT
    "Bad input frequency.",                             //PPSSYNCPPSBADFREQ
    "Bad input DAC limit values.",                      //PPSSYNCPPSBADDACLIMIT
    "Failed to start PPS-SYNC." ,                       //PPSSYNCSTARTFAILED
    "Failed to access EEPROM." ,                        //PPSSYNCEEPROMFAILED
    "Failed to get PPS-SYNC info."  ,                   //PPSSYNCGETINFOFAILED
    "Bus error.",                                       //PPSSYNCBUSERROR
	"VCXO is not locked.",                              //PPSSYNCLOCKEDFAILED
	"PPSSync_DACInit function must be called first.",   //PPSSYNCNOTINIT
	"Invalid function pointer."                         //PPSSYNCINVFUNCPTR
};

char t_lvds[][MAX_CHARACTER] =
{
	"Success.",                                                     //LVDSOK
	"The supplied value is invalid",                                //LVDSIVAL
	"LVDS FPGA core is not present.",                               //LVDSABSENT
    "This function is not supported for the current LVDS mode."     //LVDSIMODE
};

char t_gnuradio[][MAX_CHARACTER] =
{
	"Success.",																	//GNURADIO_OK
	"Could not connect to carrier board.",										//GNURADIO_CONNECT
	"Could not get MAC address.",												//GNURADIO_MACADDR
	"Could not reset RTDEX FPGA core.",											//GNURADIO_RESETCORE
	"Error while enabling flow control.",										//GNURADIO_FLOWCONTROL
	"RX has failed.",															//GNURADIO_RXFAILED
	"TX has failed.",															//GNURADIO_TXFAILED
	"The revision of Radio420 is not supported by this API.",					//GNURADIO_UNSUPPORTEDREV
	"Reset dit not complete successfully.",										//GNURADIO_RESET
	"Server not found.",														//GNURADIO_SERVERNOTFOUND
	"One of the arguments has an invalid value.",								//GNURADIO_INVVALUE
	"The carrier is not initialized, so this function cannot be executed."		//GNURADIO_CARRIER_NOT_INIT
};

char t_mo1000[][MAX_CHARACTER] =
{
	"Success.",																	//eMo1000Ok
	"Invalid device specified for register access location 01.",                //eMo1000InvalidDevice01
	"Invalid device specified for register access location 02.",				//eMo1000InvalidDevice02
	"Invalid fpga core register address location 01.",							//eMo1000InvalidCoreReg01
	"Invalid fpga core register address location 02.",						    //eMo1000InvalidCoreReg02
	"Bad user provided clock function.",										//eMo1000BadUseCFct
	"No matching MO1000 fpga core.",					                        //eMo1000NoCore
	"Unknown provided hardware revision.",										//eMo1000UnknownRevision
	"No module reset was done before executing a function that requieres it at location 01.",//eMo1000NoReset01
	"No module initialize was done before executing a function that requieres it at location 01.",//eMo1000NoInit01
	"No module initialize was done before executing a function that requieres it at location 02.",//eMo1000NoInit02
	"Board version does not support clock master in top position.",          	//eMo1000InvalidClkTop
	"No FMC card is present.",                                              	//eMo1000Absent
	"No module initialize was done before executing a function that requieres it at location 03.",//eMo1000NoInit03
    "No module initialize was done before executing a function that requieres it at location 04", //eMo1000NoInit04
    "Cannot execute fifo read synchronisation correctly",                       //eMo1000NoReadSync
    "Fifo read synchronisation lost while waiting for stabilization",           //eMo1000ReadSyncLost
    "Fifo level stabilization error",                                           //eMo1000FifoTher
    "Fifo warnings error after fifo write synchronisation",                     //eMo1000FifoWar
    "No module initialize was done before executing a function that requieres it at location 05", //eMo1000NoInit05
    "Core idelay ready (delay_ctrl_rdy) problem",                               //eMo1000NoIDelayReady
    "Core mmcm lock problem",                                                   //eMo1000NoMmcmLock
    "Data lanes training problem",                                              //eMo1000DataTraining
    "No module initialize was done before executing a function that requieres it at location 06", //eMo1000NoInit06
    "Invalid clock source (reference) specified",                               //eMo1000InvalidClkSrc
    "Unsupported clock source on board revision",                               //eMo1000UnsupportedClkSrc
    "Invalid bottom source clock (reference) specified",                        //eMo1000InvalidBottomClk
    "Invalid top source clock(reference) specified",                            //eMo1000InvalidTopClk
    "Requiered dac clock frequency overrange (greater than max supported 300 MHz at 1x, 600MHz at 2x and 1GHz > 2x)", //eMo1000DacClkOver
    "Invalid interpolation rate specified",                                     //eMo1000InvalidInterRate
    "Invalid master clock mode specified",                                      //eMo1000BadMasterClkMode
    "Invalid master clock frequency specified (overrange)",                     //eMo1000BadMasterClkFreq
    "No core dac clock reference mmcm lock",                                    //eMo1000MmcmLock
    "Frame lane training problem",                                              //eMo1000FrameTraining
    "No module initialize was done before executing a function that requieres it at location 07", //eMo1000NoInit07
    "Invalid dac channel specified at location 01",                             //eMo1000InvalidChannel01
    "Invalid dac channel state specified at location 01",                       //eMo1000InvalidState01
    "No module initialize was done before executing a function that requieres it at location 08", //eMo1000NoInit08
    "Invalid interpolation mode specified",                                     //eMo1000InvalidInterMode
    "No module initialize was done before executing a function that requieres it at location 09", //eMo1000NoInit09
    "No module initialize was done before executing a function that requieres it at location 10", //eMo1000NoInit10
    "Invalid dac channel specified at location 02",                             //eMo1000InvalidChannel02
    "Invalid dac channel specified at location 03",                             //eMo1000InvalidChannel03
    "No module initialize was done before executing a function that requieres it at location 11",//eMo1000NoInit11
    "Invalid number of active channels specified",                              //eMo1000InvalidActChannel
    "Fifo thermometer value is wrong",                                          //eMo1000FifoTherWrong
    "Fifo thermometer value could not be optimally set",                        //eMo1000FifoTherNotSet
    "Fifo thermometer value is wrong at location 02",                           //eMo1000FifoTherWrong2
    "Fifo level stabilization error at location 02",                            //eMo1000FifoTher2
    "No module initialize was done before executing a function that requieres it at location 12",//eMo1000NoInit12
    "Invalid dac pair specified at location 01",                                //eMo1000InvalidDacPair01
    "No module initialize was done before executing a function that requieres it at location 13",//eMo1000NoInit13
    "Invalid dac channels group specified at location 01",                      //eMo1000InvalidDacGroup01
    "Invalid dac isinc control state specified at location 02",                 //eMo1000InvalidState02
    "No module initialize was done before executing a function that requieres it at location 14",//eMo1000NoInit14
    "Invalid dac channels group specified at location 02",                      //eMo1000InvalidDacGroup02
    "Invalid fine modulation side band specified",                              //eMo1000InvalidSideBand
    "No module initialize was done before executing a function that requieres it at location 15",// eMo1000NoInit15
    "Invalid dac channels group specified at location 03",                      //eMo1000InvalidDacGroup03
    "Invalid fine modulation (NCO) state specified",                            //eMo1000InvalidNCOState
    "No module initialize was done before executing a function that requieres it at location 16",//eMo1000NoInit16
    "Invalid dac channels group specified at location 04",                      //eMo1000InvalidDacGroup04
    "Invalid dac data format specified",                                        //eMo1000InvalidDataFormat
    "No module initialize was done before executing a function that requieres it at location 17",//eMo1000NoInit17
    "Invalid dac pair specified at location 02",                                //eMo1000InvalidDacPair02
    "No module reset was done before executing a function that requieres it at location 02",//eMo1000NoReset02
    "No module reset was done before executing a function that requieres it at location 03",//eMo1000NoReset03
    "No module reset was done before executing a function that requieres it at location 04",//eMo1000NoReset04
    "No module reset was done before executing a function that requieres it at location 05",//eMo1000NoReset05
    "No module reset was done before executing a function that requieres it at location 06",//eMo1000NoReset06
    "No module reset was done before executing a function that requieres it at location 07",//eMo1000NoReset07
    "No module reset was done before executing a function that requieres it at location 08",//eMo1000NoReset08
    "No module reset was done before executing a function that requieres it at location 09",//eMo1000NoReset09
    "No module reset was done before executing a function that requieres it at location 10",//eMo1000NoReset10
    "Core reference frequency measurement error (time out)",                     //eMo1000BadCoreFrequency
    "Dacs reference frequency measurement error (time out)",                     //eMo1000BadDacsFrequency
    "Invalid test mode specified",                                               //eMo1000InvalidTestMode
    "No module initialize was done before executing a function that requieres it at location 18",//eMo1000NoInit18
    "Attempting test mode 2 without any data calibration done",                  //eMo1000NoCalibDone
    "Invalid inverse sinc real C0 coefficient specified",                        //eMo1000BadIsincRealC0
    "Invalid inverse sinc real C1 coefficient specified",                        //eMo1000BadIsincRealC1
    "Invalid inverse sinc real C2 coefficient specified",                        //eMo1000BadIsincRealC2
    "Invalid inverse sinc real C3 coefficient specified",                        //eMo1000BadIsincRealC3
    "Invalid inverse sinc real C4 coefficient specified",                        //eMo1000BadIsincRealC4
    "Invalid inverse sinc imaginary C0 coefficient specified",                   //eMo1000BadIsincImgC0
    "Invalid inverse sinc imaginary C1 coefficient specified",                   //eMo1000BadIsincImgC1
    "Invalid inverse sinc imaginary C2 coefficient specified",                   //eMo1000BadIsincImgC2
    "Invalid inverse sinc imaginary C3 coefficient specified",                   //eMo1000BadIsincImgC3
    "Invalid inverse sinc imaginary C4 coefficient specified",                   //eMo1000BadIsincImgC4
    "No module initialize was done before executing a function that requieres it at location 19", //eMo1000NoInit19
    "No clock setup was provided correctly using Mo1000_SetClockConfig() at location 01", //eMo1000NoClockSetup01
    "No valid pll loop filter parameters were calculated or provided",           //eMo1000NoClockLoopFilter
    "No clock setup was provided correctly using Mo1000_SetClockConfig() at location 02",//eMo1000NoClockSetup02
    "No module initialize was done before executing a function that requieres it at location 20",//eMo1000NoInit20,
    "No module initialize was done before executing a function that requieres it at location 21",//eMo1000NoInit21
    "No clock setup was provided correctly using Mo1000_SetClockConfig() at location 03",//eMo1000NoClockSetup03
    "Requiered fpga data rate is underrange: (smaller than min supported of 50 MHz)",//eMo1000DataFreqUnder
    "Invalid bottom board data reference tap delay pointer",                     //eMo1000BadBotTapRefPtr
    "Invalid bottom board data reference tap delay value",                       //eMo1000BadBotTapRefValue
    "Invalid bottom board dci tap delay pointer",                                //eMo1000BadBotDciDelPtr
    "Invalid bottom board dci tap delay value",                                  //eMo1000BadBotDciDelValue
    "Invalid bottom board dci inversion state pointer",                          //eMo1000BadBotDciInvPtr
    "Invalid bottom board dci inversion state value",                            //eMo1000BadBotDciInvValue
    "Invalid bottom board reference delay pointer",                              //eMo1000BadBotRefDelayPtr
    "Invalid bottom board dci delay pointer"                                     //eMo1000BadBotDciDelayPtr
};

char t_ad9148[][MAX_CHARACTER] =
{
	"Success.",																	//eAd9148Ok
	"Invalid register address specified location 01.",                          //eAd9148InvalidAddr01
	"Invalid register address specified location 02.", 				            //eAd9148InvalidAddr02
	"Invalid dac selection specified location 01.", 				            //eAd9148InvalidDacSelect01
	"Invalid IQ dac pair selection specified location 01.",			            //eAd9148InvalidDacPair01
	"Invalid IQ dac pair selection specified location 02.",			            //eAd9148InvalidDacPair02
	"Invalid IQ dac pair selection specified location 03.",			            //eAd9148InvalidDacPair03
    "Time out while waiting for ftw update acknowledge for NCO",                //eAd9148FtwUpdateTimeOut
    "Invalid IQ dac pair selection specified location 02",                      //eAd9148InvalidDacPair02
    "Invalid I (real) quadrature phase",                                        //eAd9148IQuadPhaseErr
    "Invalid Q (imaginary) quadrature phase",                                   //eAd9148QQuadPhaseErr
    "Invalid dac selection specified location 02",                              //eAd9148InvalidDacSelect02
    "Invalid dac selection specified location 03",                              //eAd9148InvalidDacSelect03
    "Invalid interpolation mode",                                               //eAd9148InvalidInterMode
    "Invalid sync mode specified",                                              //eAd9148InvalidSyncMode
    "Invalid sync control specified",                                           //eAd9148InvalidSyncCtrl
    "Invalid sync averaging specified",                                         //eAd9148InvalidSyncAvg
    "Invalid sync edge specified",                                              //eAd9148InvalidSyncEdge
    "Invalid fifo sync offset specified",                                       //eAd9148InvalidSyncOffset
    "Invalid output control specified",                                         //eAd9148InvalidOutCtrl
    "Invalid sed control specified",                                            //eAd9148InvalidSedCtrl
    "Invalid inverse sinc control specified",                                   //eAd9148InvalidISincCtrl
    "Invalid sideband specified",                                               //eAd9148InvalidSideband
    "Invalid nco control specified",                                            //eAd9148InvalidNcoCtrl
    "Invalid data format specified",                                            //eAd9148InvalidDataFormat
    "Invalid IQ Sed dac port selection specified location 02"                   //eAd9148InvalidDacPort02
};

char t_cdce62005p[][MAX_CHARACTER] =
{
	"Success.",																	//eCdce62005Ok
	"Invalid register address specified location 01.",                          //eCdce62005InvalidAddr01
	"Invalid register address specified location 02.",				            //eCdce62005InvalidAddr02
    "Invalid output divider register value at location 01.",                    //eCdce62005Ival01
    "Requested frequencies cannot be generated because of divider limitation at location 01", //eCdce62005Divider01
    "Output frequency overrange for specified output pin mode",                 //eCdce62005OutputFreqOver
    "Requested frequencies cannot be generated because of phase detector limitation", //eCdce62005PfdImpossible
    "Requested frequencies cannot be generated because of prescaler limitation at location 01", //eCdce62005NoPrescaler01
    "Invalid prescaler",                                                         //eCdce62005InvalidPrescaler
    "VCO freq is too high",                                                      //eCdce62005OutputDivImpossible
    "Overflow during calculation at location 01",                                //eCdce62005Overflow01
    "Invalid output divider register value at location 02.",                     //eCdce62005Ival02
    "Invalid output divider register value at location 03.",                     //eCdce62005Ival03
    "Invalid output divider register value at location 04.",                     //eCdce62005Ival04
    "Invalid output divider register value at location 05.",                     //eCdce62005Ival05
    "Requested frequencies cannot be generated because of divider limitation at location 02",//eCdce62005Divider02
    "Requested frequencies cannot be generated because of divider limitation at location 03",//eCdce62005Divider03
    "Requested frequencies cannot be generated because of prescaler limitation at location 02",//eCdce62005NoPrescaler02
    "Overflow during calculation at location 02",                                //eCdce62005Overflow02
    "Bad feedback divider buffer starting index",                                //eCdce62005BadFeedbackIndex
    "Bad timer function provided",                                               //eCdce62005BadTimerFct
    "Pll lock validation problem",                                               //eCdce62005PllLockError
    "Invalid pll input reference selection choosen",                             //eCdce62005BadPllRefSelect
    "Invalid auxiliary output control state specified",                          //eCdce62005BadAuxOutState
    "Invalid auxiliary output selection specified",                              //eCdce62005BadAuxOutSelect
    "Invalid input reference type specified",                                    //eCdce62005BadInputType
    "Invalid overrange input reference frequency according to input type specified",//eCdce62005BadInputTypeFreq
    "Invalid overrange pll input (smartmux) reference frequency over 500 MHz at location 01",//eCdce62005BadInputPllFreq01
    "Invalid overrange pll input (smartmux) reference frequency over 500 MHz at location 02",//eCdce62005BadInputPllFreq02
    "Output is in fanout mode, but no divider exist for the specified output frequency at location 01",//eCdce62005NoFanoutDivider01
    "Output is in fanout mode, but no divider exist for the specified output frequency at location 02",//eCdce62005NoFanoutDivider02
    "Output is in fanout mode, but no divider exist for the specified output frequency at location 03",//eCdce62005NoFanoutDivider03
    "Unsupported, output configured to use smartmux, but it's clock reference is not auxiliary clock",// eCdce62005SmartMuxRefNotSup
    "Bad pfd minimum frequency provided",                                        //eCdce62005BadPfdMinFrequency
    "Bad pfd maximum frequency provided",                                        //eCdce62005BadPfdMaxFrequency
    "Bad reference input pre divider used for smartmux",                         //eCdce62005BadInputPreDivider
    "Bad output pin mode type specified at location 01",                         //eCdce62005BadPinOutMode01
    "No synthetized solution can be find",                                       //eCdce62005NoSynthSolution
    "No loop filter solution can be find, a synthetized solution exists that requieres manual loop filter calculation",//eCdce62005NoFilterSolution
    "Bad output pin mode type specified at location 02",                         //eCdce62005BadPinOutMode02
    "Bad output pin mode type specified at location 03",                         //eCdce62005BadPinOutMode03
    "Bad output pin mode type specified at location 04",                         //eCdce62005BadPinOutMode04
    "Bad output pin mode type specified at location 05",                         //eCdce62005BadPinOutMode05
    "Bad output pin mode type specified at location 06",                         //eCdce62005BadPinOutMode06
    "Bad charge pump value specified",                                           //eCdce62005BadChargePump
    "Bad C1 value specified",                                                    //eCdce62005BadC1
    "Bad C2 value specified",                                                    //eCdce62005BadC2
    "Bad C3 value specified",                                                    //eCdce62005BadC3
    "Bad R2 value specified",                                                    //eCdce62005BadR2
    "Bad R3 value specified",                                                    //eCdce62005BadR3
    "Bad loop filter type",                                                      //eCdce62005BadLoopFilterType
    "Bad primary input pre divider",                                             //eCdce62005BadPriPreDiv
    "Bad secondary input pre divider",                                           //eCdce62005BadSecPreDiv
    "Bad reference divider",                                                     //eCdce62005BadRefDivider
    "Bad smartmux (pll) input reference",                                        //eCdce62005BadInputRefMode
    "Bad input pins type (must be lvcmos, lvpecl or lvds)",                      //eCdce62005BadInputPinsType
    "Bad primary input termination control",                                     //eCdce62005BadPriTermCtrl
    "Bad secondary input termination control",                                   //eCdce62005BadSecTermCtrl
    "Bad inputs lvds fail safe mode",                                            //eCdce62005BadLvdsFailSafeMode
    "Bad primary input Vbb mode",                                                //eCdce62005BadPriVbbMode
    "Bad secondary input Vbb mode",                                              //eCdce62005BadSecVbbMode
    "Bad input termination mode (AC/DC)",                                        //eCdce62005BadInputTermMode
    "Bad pll vco frequency range",                                               //eCdce62005BadVcoFrequency
    "Bad pll lock pin mode",                                                     //eCdce62005BadPllLockPinMode
    "Bad pll lock count mode",                                                   //eCdce62005BadPllLockCountMode
    "Bad pll lock witdh",                                                        //eCdce62005BadPllLockWidth
    "Bad input divider",                                                         //eCdce62005BadInputDivider
    "Bad auxiliary output selection",                                            // eCdce62005BadAuxOutputSelect
    "Bad auxiliary output state",                                                //eCdce62005BadAuxOutputState
    "Bad output sync mode",                                                      //eCdce62005BadSyncMode
    "Output #0 as a wrong output phase value",                                   //eCdce62005BadOutput0Phase
    "Output #1 as a wrong output phase value",                                   //eCdce62005BadOutput1Phase
    "Output #2 as a wrong output phase value",                                   //eCdce62005BadOutput2Phase
    "Output #3 as a wrong output phase value",                                   //eCdce62005BadOutput3Phase
    "Output #4 as a wrong output phase value",                                   //eCdce62005BadOutput4Phase
    "Output #5 as a wrong output phase value",                                   //eCdce62005BadOutput5Phase
    "Output #0 as a wrong clock reference",                                      //eCdce62005BadOutput0Ref
    "Output #1 as a wrong clock reference",                                      //eCdce62005BadOutput1Ref
    "Output #2 as a wrong clock reference",                                      //eCdce62005BadOutput2Ref
    "Output #3 as a wrong clock reference",                                      //eCdce62005BadOutput3Ref
    "Output #4 as a wrong clock reference",                                      //eCdce62005BadOutput4Ref
    "Output #5 as a wrong clock reference",                                      //eCdce62005BadOutput5Ref
    "Output #0 as a bad positive lvcmos mode",                                   //eCdce62005BadPLvcmosOut0Mode
    "Output #1 as a bad positive lvcmos mode",                                   //eCdce62005BadPLvcmosOut1Mode
    "Output #2 as a bad positive lvcmos mode",                                   //eCdce62005BadPLvcmosOut2Mode
    "Output #3 as a bad positive lvcmos mode",                                   //eCdce62005BadPLvcmosOut3Mode
    "Output #4 as a bad positive lvcmos mode",                                   //eCdce62005BadPLvcmosOut4Mode
    "Output #5 as a bad positive lvcmos mode",                                   //eCdce62005BadPLvcmosOut5Mode
    "Output #0 as a bad negative lvcmos mode",                                   //eCdce62005BadNLvcmosOut0Mode
    "Output #1 as a bad negative lvcmos mode",                                   //eCdce62005BadNLvcmosOut1Mode
    "Output #2 as a bad negative lvcmos mode",                                   //eCdce62005BadNLvcmosOut2Mode
    "Output #3 as a bad negative lvcmos mode",                                   //eCdce62005BadNLvcmosOut3Mode
    "Output #4 as a bad negative lvcmos mode",                                   //eCdce62005BadNLvcmosOut4Mode
    "Output #5 as a bad negative lvcmos mode",                                   //eCdce62005BadNLvcmosOut5Mode
    "Output #0 as a wrong output phase value",                                   //eCdce62005BadOutput0PhaseVal
    "Output #1 as a wrong output phase value",                                   //eCdce62005BadOutput1PhaseVal
    "Output #2 as a wrong output phase value",                                   //eCdce62005BadOutput2PhaseVal
    "Output #3 as a wrong output phase value",                                   //eCdce62005BadOutput3PhaseVal
    "Output #4 as a wrong output phase value",                                   //eCdce62005BadOutput4PhaseVal
    "Output #5 as a wrong output phase value"                                    //eCdce62005BadOutput5PhaseVal
};

char t_mo1000devcom[][MAX_CHARACTER] =
{
	"Success.",																	//eDevComOk
	"Com direction with this device is UNIMPLEMENTED location 01.",             //eDevComUnimplemented01
	"Error during I2C transfer location 01.",                                   //eDevComFailed01
	"I2C bus busy timeout location 01.",                                        //eDevComBusyTimeOut01
	"I2C bus ownership request timeout.",                                       //eDevComRequestTimeOut
	"Invalid data buffer given location 01.",                                   //eDevComInvalidDataBuffer01
	"I2C bus busy before operation timeout location 01.",                       //eDevComBeforeTimeOut01
	"I2C bus ownership release timeout.",                                       //eDevComReleaseTimeOut
	"Com direction with this device is UNIMPLEMENTED location 02.",             //eDevComUnimplemented02
	"Error during I2C transfer location 02.",                                   //eDevComFailed02
	"Error during I2C transfer location 03.",                                   //eDevComFailed03
	"Error during I2C transfer location 04.",                                   //eDevComFailed04
	"Error during I2C transfer location 05.",                                   //eDevComFailed05
	"Error during I2C transfer location 06.",                                   //eDevComFailed06
	"Error during I2C transfer location 07.",                                   //eDevComFailed07
	"Error during I2C transfer location 08.",                                   //eDevComFailed08
	"Error during I2C transfer location 09.",                                   //eDevComFailed09
	"Error during I2C transfer location 10.",                                   //eDevComFailed10
	"Error during I2C transfer location 11.",                                   //eDevComFailed11
	"Error during I2C transfer location 12.",                                   //eDevComFailed12
	"Error during I2C transfer location 13.",                                   //eDevComFailed13
	"I2C bus busy timeout location 02.",                                        //eDevComBusyTimeOut02
	"I2C bus busy timeout location 03.",                                        //eDevComBusyTimeOut03
	"I2C bus busy timeout location 04.",                                        //eDevComBusyTimeOut04
	"I2C bus busy timeout location 05.",                                        //eDevComBusyTimeOut05
	"I2C bus busy timeout location 06.",                                        //eDevComBusyTimeOut06
	"I2C bus busy timeout location 07.",                                        //eDevComBusyTimeOut07
	"I2C bus busy timeout location 08.",                                        //eDevComBusyTimeOut08
	"I2C bus busy timeout location 09.",                                        //eDevComBusyTimeOut09
	"I2C bus busy timeout location 10.",                                        //eDevComBusyTimeOut10
	"Invalid data buffer given location 02.",                                   //eDevComInvalidDataBuffer02
	"Invalid data buffer given location 03.",                                   //eDevComInvalidDataBuffer03
	"Invalid data buffer given location 04.",                                   //eDevComInvalidDataBuffer04
	"I2C bus busy before operation timeout location 02.",                       //eDevComBeforeTimeOut02
	"I2C bus busy before operation timeout location 03.",                       //eDevComBeforeTimeOut03
	"I2C bus busy before operation timeout location 04.",                       //eDevComBeforeTimeOut04
	"I2C bus busy before operation timeout location 05.",                       //eDevComBeforeTimeOut05
	"I2C bus busy before operation timeout location 06.",                       //eDevComBeforeTimeOut06
	"I2C bus busy before operation timeout location 07.",                       //eDevComBeforeTimeOut07
	"I2C bus busy before operation timeout location 08.",                       //eDevComBeforeTimeOut08
	"I2C bus busy before operation timeout location 09.",                       //eDevComBeforeTimeOut09
	"I2C bus busy before operation timeout location 10.",                       //eDevComBeforeTimeOut10
	"I2C bus busy before operation timeout location 11.",                       //eDevComBeforeTimeOut11
	"Invalid data buffer given location 05.",                                   //eDevComInvalidDataBuffer05
	"Error during I2C transfer location 14."         				            //eDevComFailed14
};

char t_mo1000expander[][MAX_CHARACTER] =
{
	"Success.",																	//eExpanderOk
	"Invalid dac specified.",                                                   //eExpanderInvalidDac
	"Invalid clock source specified.",                                          //eExpanderInvalidClkSource
	"Invalid raw port specified at location 01.",                               //eExpanderInvalidPort01
	"Invalid raw port specified at location 02.",                               //eExpanderInvalidPort02
	"Invalid raw port specified at location 03."                                //eExpanderInvalidPort03
};

char t_lm75p[][MAX_CHARACTER] =
{
	"Success."                                                                  // eLm75Ok
};

char t_timestamp[][MAX_CHARACTER] =
{
	"Success.",		                                                // TIMESTAMPOK
	"Timestamp FPGA core is not present.",                          // TIMESTAMPABSENT
	"Timestamp out of memory.",                                     // TIMESTAMPOUTOFMEMORY
	"Timestamp invalid handle.",                                    // TIMESTAMPINVHANDLE
	"PPS signal is not present.",                                   // TIMESTAMPPPSABSENT
	"The platform time is not synchronized with the host time.",    // TIMESTAMPOUTOFSYNC
	"Invalid timestamp clock frequency.",                           // TIMESTAMPINVFREQ
	"Invalid array length for returning the result.",               // TIMESTAMPINVARRAYLENGTH
	"Invalid time."                                                 // TIMESTAMPINVTIME
};

char t_fmcomms3[][MAX_CHARACTER] =
{
	"No error.",                                             //eFmcomms3Ok
	"No matching FPGA core.",                                //eFmcomms3NoCore
	"I/O error.",                                            //eFmcomms3IoError
	"Cannot read the gain.",                                 //eFmcomms3GainError
	"Out of memory.",                                        //eFmcomms3OutOfMem
	"Bad address.",                                          //eFmcomms3BadAddr
	"Device not present.",                                   //eFmcomms3NoDev
	"Invalid argument.",                                     //eFmcomms3InvArg
	"Connection timed out.",                                 //eFmcomms3Timedout
	"SPI error.",                                            //eFmcomms3SpiError
	"Calibration from the master module is not done."        //eFmcomms3MasterCalib
};

char t_radio640[][MAX_CHARACTER] =
{
	"Success.",                                                 //eRadio640Ok
	"No matching FPGA core",                                    //eRadio640NoCore
	"I/O error",                                                //eRadio640IoError
	"Cannot read the gain",                                     //eRadio640GainError
	"Out of memory",                                            //eRadio640OutOfMem
	"Bad address",                                              //eRadio640BadAddr
	"Device not present",                                       //eRadio640NoDev
	"Invalid argument",                                         //eRadio640InvArg
	"Timeout",                                                  //eRadio640Timeout
	"SPI error",                                                //eRadio640SpiError
	"Calibration from the master module is not done",           //eRadio640MasterCalib
	"Invalid handle",                                           //eRadio640InvHandle
	"This function is only available in the embedded library",  //eRadio640EmbedFuncOnly
	"This function is only available in the host library",      //eRadio640HostFuncOnly
	"The FMC board does not match the FPGA core",               //eRadio640Absent
	"Invalid pointer",                                          //eRadio640InvPtr
	"Invalid device",                                           //eRadio640InvDevice
	"Argument value is out of range",                           //eRadio640ArgOutOfRange
	"Argument value must only be 0 or 1",                       //eRadio640ArgIsBoolean
	"Invalid enumeration value",                                //eRadio640InvEnumValue
	"No Sync was sent to the TRX since the last MCS step",      //eRadio640NoSyncSent
	"The RefOut clock cannot by outputted in eRadio640ClkSrcMaster configuration since the clock buffer is bypassed",           //eRadio640RefOutInvCfg
	"The clock source from master board cannot be selected if the Radio640 is directly connected to the carrier board.",        //eRadio640InvClkSrcMaster
	"The clock source from carrier board cannot be selected if the Radio640 is not directly connected to the carrier board.",   //eRadio640InvClkSrcCarrier
	"The ClkToExtendedFmc clock cannot be outputted since the board does not have an extended  FMC connector."                  //eRadio640ClkToExtendedFmcInvCfg
};

char t_radio640devcom[][MAX_CHARACTER] =
{
	"Success.",                                                         //eRadio640DevComOk
	"Com direction with this device is unimplemented location 01",      //eRadio640DevComUnimplemented01
	"Com direction with this device is unimplemented location 02",      //eRadio640DevComUnimplemented02
	"Com direction with this device is unimplemented location 03",      //eRadio640DevComUnimplemented03
	"Com direction with this device is unimplemented location 04",      //eRadio640DevComUnimplemented04
	"Error during I2C transfer location 01",                            //eRadio640DevComFailed01
	"Error during I2C transfer location 02",                            //eRadio640DevComFailed02
	"Error during I2C transfer location 03",                            //eRadio640DevComFailed03
	"Error during I2C transfer location 04",                            //eRadio640DevComFailed04
	"Invalid data buffer given location 01",                            //eRadio640DevComInvalidDataBuffer01
	"Invalid data buffer given location 02",                            //eRadio640DevComInvalidDataBuffer02
	"Invalid data buffer given location 03"                             //eRadio640DevComInvalidDataBuffer03
};

char t_radio640spi[][MAX_CHARACTER] =
{
	"Success",                              //eRadio640SpiOk                
	"SPI bus busy timeout",                 //eRadio640SpiBusyTimeOut       
	"Write operation is not supported",     //eRadio640SpiWriteUnimplemented
	"Read operation is not supported",      //eRadio640SpiReadUnimplemented 
	"Invalid ID",                           //eRadio640SpiInvId             
	"Cannot allocate memory"                //eRadio640SpiMemAlloc          
};

char t_ad9361[][MAX_CHARACTER] =
{
	"Success.",                                                                                                //eAd9361Ok
	"No matching FPGA core",                                                                                   //eAd9361NoCore
	"I/O error",                                                                                               //eAd9361IoError
	"Cannot read the gain",                                                                                    //eAd9361CannotReadGain
	"Out of memory",                                                                                           //eAd9361OutOfMem
	"Bad address",                                                                                             //eAd9361BadAddr
	"Device not present",                                                                                      //eAd9361NoDev
	"Invalid argument",                                                                                        //eAd9361InvArg
	"Calibration has timed out",                                                                               //eAd9361CalibTimeout
	"SPI error",                                                                                               //eAd9361SpiError
	"Calibration from the master module is not done",                                                          //eAd9361MasterCalib
	"Invalid handle",                                                                                          //eAd9361InvHandle
	"Cannot communicate with a AD9361 chip",                                                                   //eAd9361ComError
	"Invalid AD9361 chip ID",                                                                                  //eAd9361InvChipID
	"Invalid enumeration",                                                                                     //eAd9361InvEnum
	"Argument value is out of range",                                                                          //eAd9361ArgOutOfRange
	"Invalid channel number",                                                                                  //eAd9361InvChannel
	"The channel is not enabled",                                                                              //eAd9361ChannelNotEnable
	"The ENSM state is not valid in FDD mode",                                                                 //eAd9361InvFddEnsmState
	"The ENSM state is not valid in TDD mode",                                                                 //eAd9361InvTddEnsmState
	"The current ENSM state must be ALERT before changing to the desired state",                               //eAd9361CurrentEnsmStateMustBeAlert
	"At least one of the clock rates must equal the serial clock rate.",                                       //eAd9361InvClkRate
	"The desired sampling rate exceeds the allowed range",                                                     //eAd9361InvSamplingRate
	"Cannot find a value solution for the PLL",                                                                //eAd9361CannotFindPllSolution
	"The fast lock profile has not been initialized",                                                          //eAd9361FastLockProfileNotInit
	"Invalid reference clock frequency",                                                                       //eAd9361InvRefClkFreq
	"The specified calibration is not supported",                                                              //eAd9361CalibNotSupported
	"The FIR coefficients were not loaded correctly",                                                          //eAd9361FirCoefLoadError
	"Invalid FIR parameters",                                                                                  //eAd9361InvFirParam
	"Invalid FIR interpolation or decimation value",                                                           //eAd9361InvFirInterpDecim
	"The maximal allowed number of coefficient is 64 in TX with an interpolation of 1",                        //eAd9361InvFirTxNumCoef
	"The number of coefficient is higher than the maximal allowed for the current ADC or DAC clock rate",      //eAd9361InvFirNumCoef
	"Invalid clock configuration",                                                                             //eAd9361InvClockConfig
	"Invalid direction",                                                                                       //eAd9361InvDirection
	"No solution could be found for the digital calibration",                                                  //eAd9361DigitalCalibNoSolution
	"BBPLL unlocked. Make sure the input reference clock and specified frequencies are valid.",                //eAd9361BbPllUnlocked
	"TXRFPLL unlocked. Make sure the input reference clock and specified frequencies are valid.",              //eAd9361TxRfPllUnlocked
	"RXRFPLL unlocked. Make sure the input reference clock and specified frequencies are valid."               //eAd9361RxRfPllUnlocked
};

char t_ina226[][MAX_CHARACTER] =
{
	"Success.",                                                                     //eIna226Ok
	"Invalid mode",                                                                 //eIna226InvMode
	"The timeout value has been reached before the value were ready to be read",    //eIna226Timeout
	"Invalid argument value"                                                        //eIna226InvArg
};

char t_rtdexsync[][MAX_CHARACTER] =
{
	"Success.",		                            // RTDEXSYNCOK
	"RTDExSync generic error",                  // RTDEXSYNCFALIED
	"Core absent.",                             // RTDEXSYNCCOREABSENT
	"Failed to allocate memory.",               // RTDEXSYNCOUTOFMEMORY
	"Timed out.",                               // RTDEXSYNCTIMEOUT
	"Invalid subframe size.",                   // RTDEXSYNCINVSUBFRSIZE
	"Invalid frame size.",                      // RTDEXSYNCINVFRSIZE
	"Invalid channel direction.",               // RTDEXSYNCINVDIR
	"Invalid channel state.",                   // RTDEXSYNCINVSTATE
    "Invalid start mode",                       // RTDEXSYNCINVSTARTMODE
    "Invalid stop mode",                        // RTDEXSYNCINVSTOPMODE
    "Invalid time",                             // RTDEXSYNCINVTIME
    "Invalid trigger selection",                // RTDEXSYNCINVTRIGSEL
    "Invalid packet size",                      // RTDEXSYNCINVPKTSIZE
    "Invalid packet type",                      // RTDEXSYNCINVPKTTYPE
    "Invalid event queue",                      // RTDEXSYNCINVEVTQUEUE
    "Protocol error",                           // RTDEXSYNCPROTOERROR
    "Failed to retrieve HOST IP address",       // RTDEXSYNCHOSTIPERR
    "Specified RTDExSync channel is absent"     // RTDEXSYNCCHANNELABSENT
};

char t_rtdexts[][MAX_CHARACTER] =
{
	"Success.",		                            // RTDEXTSOK
	"Failed to allocate memory.",               // RTDEXTSOUTOFMEMORY
	"Timestamp invalid handle."                 // RTDEXTSINVHANDLE
};

char t_configfile[][MAX_CHARACTER] =
{
    "Sucess.",                                  // CONFIGFILEOK
    "Bad file extension.",                      // CONFIGGILEBADFILEEXTENTSION
    ".ini file not found.",                     // CONFIGFILEFILENOTFOUND
    "Parameter not found in .ini file.",        // CONFIGFILEPARAMNOTFOUND
    "Invalid ValueMaxSize.",                    // CONFIGFILEMAXSIZE
    "Invalid ValueType.",                       // CONFIGFILEVALUETYPE
    "No FMC to initiliaze."                     // CONFIGFILENOFMCTOINIT
};

char t_fmcvita[][MAX_CHARACTER] =
{
    "Sucess.",                                                    // FMCVITA_OK
    "Too many eeprom available or invalid 2402 eeprom types.",    // FMCVITA_ERR_INVALID_EEPROMNUM
    "Eeeprom is not blanked and cannot be overrided.",            // FMCVITA_ERR_EEPROM_NOTBLANK
    "Eeeprom size specified in buffer is invalid.",               // FMCVITA_ERR_INVALID_EEPROMSIZE
    "No eeprom detected.",                                        // FMCVITA_ERR_INVALID_NOEEPROM
    "Invalid password for a protected feature."                   // FMCVITA_ERR_INVALID_PASSWORD
};

char* ErrorToString(int err)
{
	unsigned int sever, mod ,code;
	char str_sever[MAX_CHARACTER], str_codestr[MAX_CHARACTER], str_mod[MAX_CHARACTER],  str_code[MAX_CHARACTER];
	static char str_final[MAX_CHARACTER];

	sprintf(str_codestr,"%X", err);

    // Catch generic error -1
    if(err == -1)
        err = ERR_GENERIC;

	sever = RESULT_SEVERITY(err);
	mod = RESULT_MODULE(err);
	code = RESULT_CODE(err);

	strcpy(str_sever, t_severity[sever]);
	if(mod > MODULE_NUM)
	{
		strcpy(str_mod, "Undefined module");
	}
	else
	{
		strcpy(str_mod, t_module[mod]);
	}
	switch (mod)
	{
	default:
		strcpy(str_code, "Code error not defined.");
		break;
    case MODULE_NULL:
        if(code == RESULT_CODE(ERR_GENERIC))
            strcpy(str_code, "Generic error");
        else if(code == RESULT_CODE(ERR_CRITICAL))
            strcpy(str_code, "Unrecoverable failure");
        else
            strcpy(str_code, t_null[code]);
        break;
	case MODULE_I2C: //
		strcpy(str_code, t_i2c[code]);
		break;
	case MODULE_AD9517: //
		strcpy(str_code, t_ad9517[code]);
		break;
	case MODULE_FMCEEPROM: //
		strcpy(str_code, t_fmceeprom[code]);
		break;
	case MODULE_FMCEEPROM_EEPROM: //
		strcpy(str_code, t_fmceeprom_eeprom[code]);
		break;
	case MODULE_CARRIER: //MODULE_CARRIER
		strcpy(str_code, t_carrier[code]);
		break;
	case MODULE_RECPLAYTEST:
		strcpy(str_code, t_recplaytest[code]);
		break;
	case MODULE_ADP_CCE: //MODULE_ADP_CCE
		strcpy(str_code, t_adp_cce[code]);
		break;
	case MODULE_ADP_EAPI: //MODULE_ADP_EAPI
		strcpy(str_code, t_adp_eapi[code]);
		break;
	case MODULE_OSCONFIG: //MODULE_OSCONFIG
		strcpy(str_code,t_osconfig[code]);
		break;
	case MODULE_ADAC250: //MODULE_ADAC250
		strcpy(str_code, t_adac250[code]);
		break;
	case MODULE_FMC: //MODULE_FMC
		strcpy(str_code, t_fmc[code]);
		break;
	case MODULE_FMCSPI: //MODULE_FMCSPI
		strcpy(str_code, t_fmcspi[code]);
		break;
	case MODULE_FMCRADIO: //MODULE_FMCRADIO
		strcpy(str_code, t_radio420[code]);
		break;
	case MODULE_LMS6002: //MODULE_LMS6002
		strcpy(str_code, t_lms6002[code]);
		break;
	case MODULE_CDCE62005: //MODULE_CDCE62005
		strcpy(str_code, t_cdce62005[code]);
		break;
	case MODULE_MI250: //MODULE_MI250
		strcpy(str_code, t_mi250[code]);
		break;
	case MODULE_ADC5000: //MODULE_ADC5000
		strcpy(str_code, t_adc5000[code]);
		break;
	case MODULE_COMWRAPPER: //MODULE_COMWRAPPER
		strcpy(str_code, t_comwrapper[code]);
		break;
	case MODULE_RTDEX: //MODULE_RTDEX
		if (code <= 0x3E)
		{
			strcpy(str_code, t_rtdex[code]);
		}
		else
		{
			strcpy(str_code, t_rtdex[code-0x41]); //to get PCIe kernel error
		}
		break;
	case MODULE_RECPLAY: //MODULE_RECPLAY
		strcpy(str_code, t_recplay[code]);
		break;
	case MODULE_MI125: //MODULE_MI125
		strcpy(str_code, t_mi125[code]);
		break;
	case MODULE_FMCLVDS: //MODULE_FMCLVDS
		strcpy(str_code, t_fmclvds[code]);
		break;
	case MODULE_EV10AQ190: //MODULE_EV10AQ190
		strcpy(str_code, t_ev10aq190[code]);
		break;
	case MODULE_LTM9012: //MODULE_LTM9012
		strcpy(str_code, t_ltm9012[code]);
		break;
	case MODULE_AURORA: //MODULE_AURORA
		strcpy(str_code, t_aurora[code]);
		break;
    case MODULE_MI125COMWRAPPER:
		strcpy(str_code, t_mi125comwrapper[code]);
		break;
	case MODULE_ADT7411: //MODULE_ADT7411
		strcpy(str_code, t_adt7411[code]);
		break;
	case MODULE_LM75: //MODULE_LM75
		strcpy(str_code, t_lm75[code]);
		break;
	case MODULE_PCF8574: //MODULE_PCF8574
		strcpy(str_code, t_pcf8574[code]);
		break;
	case MODULE_PCA9535: //MODULE_PCA9535
		strcpy(str_code, t_pca9535[code]);
		break;
	case MODULE_PPSSYNC: //MODULE PPSSYNC
		strcpy(str_code, t_ppssync[code]);
		break;
	case MODULE_LVDS: //MODULE LVDS
		strcpy(str_code, t_lvds[code]);
		break;
	case MODULE_MO1000DEVCOM: //MODULE MO1000 devcom
		strcpy(str_code, t_mo1000devcom[code]);
		break;
	case MODULE_MO1000EXPANDER: //MODULE MO1000 expander
		strcpy(str_code, t_mo1000expander[code]);
		break;
	case MODULE_AD9148: //MODULE ad9148
		strcpy(str_code, t_ad9148[code]);
		break;
	case MODULE_CDCE62005P: //MODULE CDCE62005P
		strcpy(str_code, t_cdce62005p[code]);
		break;
	case MODULE_MO1000: //MODULE MO1000
		strcpy(str_code, t_mo1000[code]);
		break;
	case MODULE_TIMESTAMP: //MODULE_TIMESTAMP
		strcpy(str_code, t_timestamp[code]);
		break;
	case MODULE_FMCOMMS3: //MODULE_FMCOMMS3
		strcpy(str_code, t_fmcomms3[code]);
		break;
	case MODULE_RADIO640: //MODULE_RADIO640
		strcpy(str_code, t_radio640[code]);
		break;
	case MODULE_RADIO640DEVCOM: //MODULE_RADIO640DEVCOM
		strcpy(str_code, t_radio640devcom[code]);
		break;
	case MODULE_RADIO640SPI: //MODULE_RADIO640SPI
		strcpy(str_code, t_radio640spi[code]);
		break;
	case MODULE_AD9361: //MODULE_AD9361
		strcpy(str_code, t_ad9361[code]);
		break;
	case MODULE_INA226: //MODULE_INA226
		strcpy(str_code, t_ina226[code]);
		break;
	case MODULE_LM75P: //MODULE LM75P
		strcpy(str_code, t_lm75p[code]);
		break;
	case MODULE_GNURADIO: //MODULE GNURADIO
		strcpy(str_code, t_gnuradio[code]);
		break;
	case MODULE_RTDEXSYNC: //MODULE_RTDEXSYNC
		strcpy(str_code, t_rtdexsync[code]);
		break;
	case MODULE_RTDEXTS: //MODULE_RTDEXTS
		strcpy(str_code, t_rtdexts[code]);
		break;
  case MODULE_CONFIGFILE:
    strcpy(str_code, t_configfile[code]);
    break;
  case MODULE_FMCVITA:
    strcpy(str_code, t_fmcvita[code]);
    break;   
	}

	sprintf(str_final,"%s 0x%s: %s module - %s",str_sever,str_codestr,str_mod,str_code);
	return str_final;
}
