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
 * @file       bas_error.h
 * @brief      Error code generation and analysis.
 *
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
 
#ifndef __BAS_ERROR_H_
#define __BAS_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
*****************************************************************************
* @file bas_error.h
*****************************************************************************
*
* This module manages the codes returned by functions. It is
* used by many drivers and software modules to ensure that the
* codes returned are uniform between drivers and modules. When errors occur, this
* method allows users to determine what drivers or modules failed, and
* the cause of the problem(s). It becomes easier and faster to troubleshoot
* applications.
*
****************************************************************************/

/**
*****************************************************************************
*
* Return code management type definitions.
*
****************************************************************************/
typedef int Result;

/**
*****************************************************************************
*
* Generates a function return code.
*
* @param [in]  severity
*      Degree of severity.
*
* @param [in]  module
*      Current software module or driver.
*
* @param [in]  code
*      Code description number.
*
* @return
*      Function return code.
*
****************************************************************************/
#define MAKE_RESULT(severity, module, code) \
    ((Result)(((unsigned int)(severity) << 30) | ((unsigned int)(module) << 16) | ((unsigned int)(code))))

/**
*****************************************************************************
*
* Retrieves the severity from the return code.
*
* @param [in]  result
*      Function return code.
*
* @return
*      Degree of severity.
*
*****************************************************************************/
#define RESULT_SEVERITY(result) \
    (((unsigned int)(result) >> 30) & 0x3)

/**
*****************************************************************************
*
* Retrieves the module or driver affected by the return code.
*
* @param [in]  result
*      Function return code.
*
* @return
*      Affected software module or driver.
*
****************************************************************************/
#define RESULT_MODULE(result) \
  (((unsigned int)(result >> 16)) & 0x3FFF)

/**
*****************************************************************************
*
* Retrieves the code description number of the return code.
*
* @param [in]  result
*      Function return code.
*
* @return
*      Code description number.
*
****************************************************************************/
#define RESULT_CODE(result) \
    ((unsigned int)(result) & 0xFFFF)

/**
*****************************************************************************
*
* Verifies whether the return code signifies success.
*
* @param [in]  result
*      Function return code.
*
* @return
*      Success (1) or failure (0).
*
****************************************************************************/
#define SUCCESS(result) \
    ((result) >= 0)

/**
*****************************************************************************
*
* Verifies whether the return code signifies failure.
*
* @param [in]  result
*      Function return code.
*
* @return
*      Failure (1) or success (0).
*
****************************************************************************/
#define FAILURE(result) \
    ((result) < 0)

/**
*****************************************************************************
*
* Degrees of severity of a return code.
*
****************************************************************************/
enum BasError_eSeverity {
    eBasSeveritySuccess=0,             /**< MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_NULL, 0) = 0x00000000 */
    eBasSeverityInformational=1,       /**< MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_NULL, 0) = 0x40000000 (use to report calls to legacy functionalities) */
    eBasSeverityWarning=2,             /**< MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_NULL, 0) = 0x80000000 */
    eBasSeverityError=3                /**< MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_NULL, 0) = 0xC0000000 */
};

// For retrocompatibility
#define LYR_SEVERITY_SUCCESS        eBasSeveritySuccess
#define LYR_SEVERITY_INFORMATIONAL  eBasSeverityInformational
#define LYR_SEVERITY_WARNING        eBasSeverityWarning
#define LYR_SEVERITY_ERROR          eBasSeverityError

/**
*****************************************************************************
*
* List of the software modules and drivers using this return code management.
*
****************************************************************************/
enum Module {
    /* broadly applicable module (NULL) */
    MODULE_NULL=0,          /**< Default software module */
    MODULE_I2C=1,           /**< I2C software module */
    MODULE_I2C_EEPROM=2,    /**< I2C EEPROM software module */
    MODULE_AD9510=3,        /**< AD9511 software module */
    MODULE_AD9511=4,        /**< AD9510 software module */
    MODULE_AD9517=5,        /**< AD9517 software module */
    MODULE_FMCEEPROM=6,     /**< FMCEEPROM software module */
    MODULE_FMCEEPROM_EEPROM=7,/**< FMCEEPROM eeprom software module */
    MODULE_CARRIER=9,       /**< Carrier generic system operation related module */
    MODULE_FMCVITA=0xA,     /**< FMC vita system module */

    /* Host */
    MODULE_SMQUADAPI=0x10,  /**< SignalMaster Quad host API software module */
    MODULE_RFFE     =0x11,  /**< RFFE's control API software module */
    MODULE_U2C      =0x12,  /**< USB to GPIO control API software module (refer to i2cbridge.h) */
    MODULE_ADACSYNC =0x13,  /**< ADACSync's control API software module */
    MODULE_INITLOADER=0x14, /**< Control API software module of the ADACSync's initialization loader */
    MODULE_TIME     =0x15,  /**< Timer API */
    MODULE_LINKEDLIST=0x16, /**< Linked list API */

    /* DSP internal modules */
    MODULE_DMA=0x20,        /**< DSP DMA software module */
    MODULE_IRQ=0x21,        /**< DSP IRQ software module */
    MODULE_SYS=0x22,        /**< DSP general platform configuration software module */

    /* Lyrtech drivers */
    MODULE_XEVT=0x30,       /**< DSP external event software module */
    MODULE_FBUS=0x31,       /**< DSP FastBus driver */
    MODULE_RAVE=0x32,       /**< DSP RAVe driver */
    MODULE_CAMLNK=0x33,     /**< DSP Camera Link driver */
    MODULE_DRC=0x34,        /**< DSP DRC Virtex-4 driver */
    MODULE_TTRFT=0x35,      /**< Twin Tunable RF Transceiver's control API */
    MODULE_TWRFT=0x36,      /**< Twin Wimax RF Transceiver's control API */

    /* Utility modules */
    MODULE_QUEUE=0x40,      /**< DSP queue management software module */
    MODULE_PINGPONG=0x41,   /**< DSP ping-pong management software module */
    MODULE_RECPLAYTEST=0x42,/**< Recplay test module */
    MODULE_CONFIGFILE=0x43, /**< ConfigFile Parser module */

    /* Linux */
    MODULE_ADP_PCIE_DRV=0x50, /**< Linux ADP PCIe driver */
    MODULE_ADP_PCIE=0x51,     /**< Linux ADP PCIe module */
    MODULE_ADP_CCE=0x52,      /**< Linux ADP CCE module */
    MODULE_ADP_EAPI=0x53,     /**< Linux ADP EAPI module */
    MODULE_OSCONFIG=0x54,     /**< Linux Osconfig module */

    /* ADP */
    MODULE_ADAC250=0x60,     /**< Adac250 FMC module */
    MODULE_FMC=0x61,         /**< FMC module */
    MODULE_CLOCK=0x62,       /**< Clock module */
    MODULE_FMCSPI=0x63,      /**< FMC Spi module */
    MODULE_FMCRADIO=0x64,    /**< Radio420 FMC module */
    MODULE_LMS6002=0x65,     /**< LMS6002 module */
    MODULE_CDCE62005=0x66,   /**< CDCE62005 module */
    MODULE_MI250=0x67,       /**< MI250 FMC module */
    MODULE_ADC5000=0x68,     /**< ADC5000 FMC module */
    MODULE_COMWRAPPER=0x69,  /**< Com Wrapper module */
    MODULE_RTDEX=0x6B,       /**< RTDEx module */
    MODULE_RECPLAY=0x6C,     /**< Record/Playback module */
    MODULE_MI125=0x6D,       /**< MI125 FMC module */
    MODULE_TIMER=0x6E,       /**< Timer module */
    MODULE_FMCLVDS=0x6F,     /**< LVDS Xin Xout FMC module */
    MODULE_EV10AQ190=0x70,   /**< EV10AQ190 ADC module */
    MODULE_LTM9012=0x71,     /**< LTM9012 ADC module */
	MODULE_AURORA=0x72,      /**< Aurora module */
    MODULE_MI125COMWRAPPER=0x73,  /**< MI125 com wrapper module */
    MODULE_PPSSYNC=0x74,     /**< PPS Sync module */
    MODULE_LVDS=0x75,        /**< LVDS module */
    MODULE_MO1000DEVCOM=0x76,/**< MO1000 DevCom module */
    MODULE_MO1000EXPANDER=0x77,/**< MO1000 Expander module */
    MODULE_AD9148=0x78,      /**< AD9148 DAC module */
    MODULE_CDCE62005P=0x79,  /**< CDCE62005P module */
    MODULE_MO1000=0x7A,      /**< MO1000 FMC module */
    MODULE_TIMESTAMP=0x7B,   /**< PPS Sync module */
    MODULE_FMCOMMS3=0x7C,    /**< FMCOMMS3 module */
    MODULE_RADIO640=0x7D,    /**< Radio640 module */
    MODULE_RADIO640DEVCOM=0x7E, /**< Radio640 DevCom module */
    MODULE_RADIO640SPI=0x7F, /**< Radio640 DevCom module */

    /* Sensors */
    MODULE_ADT7411=0x80,     /**< ADT7411 Sensor module */
    MODULE_TM75=0x81,        /**< TM75 Sensor module */
    MODULE_LM75=0x82,        /**< LM75 Sensor module */
    MODULE_LM75P=0x83,       /**< LM75 Sensor plus module */
    MODULE_INA226=0x84,      /**< INA226 Sensor module */
    MODULE_AD9361=0x85,      /**< AD9361 Sensor module */

    /* IO expanders */
    MODULE_PCF8574=0x90,     /**< PCF8574 IO expanders module */
    MODULE_PCA9535=0x91,     /**< PCA9535 IO expanders module */
    
    /*PPS SYNC*/    

	/* GNURADIO */
	MODULE_GNURADIO=0xA0,    /**< GNURADIO module */

    /* RTDEx Sync */
    MODULE_RTDEXSYNC=0xAA,   /**< RTDEx Sync module */
    MODULE_RTDEXTS=0xAB,     /**< RTDExTs module */
 
    MODULE_NUM               /**< Number of modules */
};

/**
*****************************************************************************
*
* This is an example of enumeration that specifies a module's specific error code.
*
* @attention
*      The only usable return code for this enumeration is OK. Do NOT
*      use any other. Other return codes are general, so they do not apply
*      to any module. It is important to use module-specific return codes
*      because they inform you of what modules failed when errors occur. Also,
*      specify a code description number to each specific problem. This allows
*      you to better understand the problem when it occurs.
*
****************************************************************************/
enum Error {
    OK                  = MAKE_RESULT(LYR_SEVERITY_SUCCESS,       MODULE_NULL, 0),          /**< The method succeeded */
    ERR_GENERIC         = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 0xFFFF),     /**< The method failed */
    ERR_CRITICAL        = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 0xDEAD),     /**< Unrecoverable failure */
    ERR_OUTOFMEMORY     = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 1),          /**< The method failed to allocate necessary memory */
    ERR_PENDING         = MAKE_RESULT(LYR_SEVERITY_INFORMATIONAL, MODULE_NULL, 2),          /**< The necessary data is not yet available */
    ERR_ABORT           = MAKE_RESULT(LYR_SEVERITY_WARNING,       MODULE_NULL, 3),          /**< The ongoing operation was aborted */
    ERR_ACCESSDENIED    = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 4),          /**< General access denied */
    ERR_NOTIMPLEMENTED  = MAKE_RESULT(LYR_SEVERITY_INFORMATIONAL, MODULE_NULL, 5),          /**< The method is not implemented */
    ERR_INVARG          = MAKE_RESULT(LYR_SEVERITY_WARNING,       MODULE_NULL, 6),          /**< The supplied argument is invalid */
    ERR_INVPTR          = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 7),          /**< The supplied pointer is invalid */
    ERR_INVHANDLE       = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 8),          /**< The supplied handle is invalid */
    ERR_TIMEOUT         = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 9),          /**< The time has elapsed */
    ERR_ALRDYALLOC      = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 10),         /**< Resource already allocated */
    ERR_NOTINIT         = MAKE_RESULT(LYR_SEVERITY_ERROR,         MODULE_NULL, 11),         /**< The module was not initialized. Initialize it before calling this function */
    ERR_ALRDYDONE       = MAKE_RESULT(LYR_SEVERITY_WARNING,       MODULE_NULL, 12),         /**< General task that must be performed only once */
    ERR_NOHWMODULE      = MAKE_RESULT(LYR_SEVERITY_INFORMATIONAL, MODULE_NULL, 13)          /**< The targeted hardware module was not detected */
};

/****************************************************************************
* Function: GetError
*************************************************************************//**
*
* Returns the code error in a string
*
* @param [in]  err
*      The code error in hexadecimal
*
* @return
*      Code error string
*
****************************************************************************/                      
char* ErrorToString(int err);

#ifdef __cplusplus
}
#endif

#endif /*__LYR_ERROR_H_*/
