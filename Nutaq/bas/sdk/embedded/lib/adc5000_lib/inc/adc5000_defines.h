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
 * @file       adc5000_defines.h
 * @brief      Adc5000 module related data and structure definitions.
 *
 * $Date: 2015/05/15 15:14:46 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADC5000
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

#ifndef __ADC5000_DEFINES__
#define __ADC5000_DEFINES__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "ad9517_defines.h"
#include "ev10aq190_defines.h"
#include "adt7411_defines.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/
#define ADC5000_VCO_FREQ_MAX ((unsigned)2503e6) ///< Maximum ADC5000 PLL Voltage-controlled oscillator frequency
#define ADC5000_VCO_FREQ_MIN ((unsigned)2497e6) ///< Minimum ADC5000 PLL Voltage-controlled oscillator frequency


/** @name ADC5000 clock source
 *
 *  Possible adc5000 fmc clock source definition.
 */
typedef enum E_adc5000_clock_source {
    ADC5000_CLOCK_EXTERNAL = 0,     ///< External clock
    ADC5000_CLOCK_EXTERNAL_REF = 3, ///< PLL VCO clock with an external reference
    ADC5000_CLOCK_INTERNAL_REF = 6, ///< PLL VCO clock with an internal reference
} adc5000_clock_source;

/** @name ADC5000 sync source
 *
 *  Possible adc5000 fmc sync source definition.
 */
typedef enum E_adc5000_sync_source {
    ADC5000_SYNC_EXT,     ///< External sync from the Trigger front panel connector
    ADC5000_SYNC_CARRIER, ///< Sync from the FPGA carrier board
    ADC5000_SYNC_PLL,     ///< Sync from the ADC5000 PLL
    ADC5000_SYNC_OFF,     ///< Sync off
} adc5000_sync_source;

/** @name ADC5000 sensor
 *
 *  Possible adc5000 fmc sensor definition.
 */
typedef enum E_adc5000_sensor {
    ADC5000_BOARD_TEMPERATURE,  ///< Board temperature sensor
    ADC5000_3V3,                ///< 3.3V voltage sensor
    ADC5000_ADC_TEMPERATURE,    ///< ADC temperature sensor
    ADC5000_VADJ,               ///< Adjustable voltage sensor
    ADC5000_2V5,                ///< 2.5V voltage sensor
    ADC5000_3V3_DIGITAL,        ///< 3.3V digital voltage sensor
    ADC5000_3V3_ANALOG,         ///< 3.3V analog voltage sensor
    ADC5000_VCP,                ///< Charge pump voltage sensor
    ADC5000_1V8,                ///< 1.8V voltage sensor
} adc5000_sensor;


/** @name ADC5000 fpga internal control register structure
 *
 *  Describe the structure of the ADC5000 fpga internal control register
 */
typedef struct S_control_reg {
#ifdef _BIG_ENDIAN
 unsigned reserved1:20, d_aligned:1, c_aligned:1, b_aligned:1, a_aligned:1, reserved2:1, rst_fifo:1, core_reset_pulse:1, sync_cmd:1, reserved3:1, io_reset:1, clk_reset:1, delay_reset:1;
#else
 unsigned delay_reset:1, clk_reset:1, io_reset:1, reserved3:1, sync_cmd:1, core_reset_pulse:1, rst_fifo:1, reserved2:1, a_aligned:1, b_aligned:1, c_aligned:1, d_aligned:1, reserved1:20;
#endif
} control_reg;


/** @name ADC5000 fpga internal calibration register structure
 *
 *  Describe the structure of the ADC5000 fpga internal calibration registers
 */
typedef struct S_calib_reg {
#ifdef _BIG_ENDIAN
   unsigned reserved1:7, bitslip:10, idelay_we:10, idelay:5;
#else
   unsigned idelay:5, idelay_we:10, bitslip:10, reserved1:7;
#endif
} calib_reg;


/** @name ADC5000 fpga internal trigger control register structure
 *
 *  Describe the structure of the ADC5000 fpga internal trigger control register
 *  Trigger delay can be set between 0 and 31. This delay the trigger by
 *  1 to 32 clock cycles
 */
typedef struct S_trig_control_reg {
#ifdef _BIG_ENDIAN
   unsigned reserved1:27, trigger_delay:5;
#else
   unsigned trigger_delay:5, reserved1:27;
#endif
} trig_control_reg;


/** @name ADC5000 fpga internal frequency counter register structure
 *
 *  Describe the structure of the ADC5000 fpga internal frequency counter register
 *  clk_sel : x"0" => 200 MHz reference clock
 *	        : x"1" => ADC Channel A clock
 *	        : x"2" => ADC Channel B clock
 *	        : x"3" => ADC Channel C clock
 *	        : x"4" => ADC Channel D clock
 *	        : x"6" => PLL clock
 */
typedef struct S_freq_counter_reg {
#ifdef _BIG_ENDIAN
    unsigned reserved:12, clk_sel:4, clock_count:16;
#else
    unsigned clock_count:16, clk_sel:4, reserved:12;
#endif
} freq_counter_reg;


/** @name ADC5000 core register map
 *
 *  Hold ADC5000 fpga core internal device registers definitions.
 */
typedef struct S_adc5000_core {
    unsigned coreid;
    control_reg control;/*SERDES PHY cmd & status register*/
    calib_reg calibA;  /*serdes and iodelay values for channel A*/
    calib_reg calibB;  /*serdes and iodelay values for channel B*/
    calib_reg calibC;  /*serdes and iodelay values for channel C*/
    calib_reg calibD;  /*serdes and iodelay values for channel D*/
    unsigned calibErrorChA; /*bit calibration error for channel A*/
    unsigned calibErrorChB; /*bit calibration error for channel B*/
    unsigned calibErrorChC; /*bit calibration error for channel C*/
    unsigned calibErrorChD; /*bit calibration error for channel D*/
    trig_control_reg trig_control; /*Trigger edge configuration & control*/
    freq_counter_reg freq_counter; /*Frequency counter clock select & status register*/
} adc5000_core;


/** @name ADC5000 Board instance structure
 *
 *  Hold all information regarding ADC5000 board instance used.
 */
typedef struct S_adc5000 {
    volatile adc5000_core * core;
    Carrier_handle_t hCarrier;                              ///< Carrier handle to access the carrier specific services
    Carrier_eFmcConnectorNum_t FmcConnector;                ///< FMC connector related to this instance
    com_core com;
    ad9517_core pll;
    ev10aq190_core adc;
    adt7411_core monitoring;
    unsigned char calibration_done;
} adc5000;


/** @name ADC5000 Error codes
 *
 *  List all runtime related error codes of a ADC5000 module.
 */
enum adc5000_error {
    ADC5000OK     = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_ADC5000, 0),                 ///<  No error
    ADC5000ALIGN  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADC5000, 1),                   ///<  Digital calibration with the ADC5000 data stream failed
    ADC5000IVAL   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADC5000, 2),                   ///<  Invalid parameters
    ADC5000PLLUNSTABLE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADC5000, 3),              ///<  PLL is not locked
    ADC5000COREABSENT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADC5000, 4),               ///<  Core absent
    ADC5000FMCABSENT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADC5000, 5),                ///<  FMC absent
    ADC5000_INVALID_TRIGGER_DELAY = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADC5000, 6),   ///<  Invalid trigger delay value
    ADC5000_ERROR_COMM_PLL = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADC5000, 7),          ///<  Communication error with the PLL chip
    ADC5000_ERROR_COMM_ADC = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADC5000, 8),          ///<  Communication error with the ADC chip
};

#ifdef __cplusplus
}
#endif  // extern "C"

#endif
