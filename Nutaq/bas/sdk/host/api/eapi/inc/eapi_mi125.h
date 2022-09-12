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
 * @file       eapi_mi125.h
 * @brief      mi125 eapi module related functions definitions
 *
 * $Date: 2015/09/09 19:27:47 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup MI125_MI125
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

#ifndef __EAPI_MI125__
#define __EAPI_MI125__

#include "eapi.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "MI125_mi125_defines.h"

/** @defgroup MI125_MI125 MI125 EAPI
 *  
 *  MI125 module EAPI allows to control a single 16 ADC channels MI125-16, or
 *  a dual MI125-32 & MI125-32E 32 ADC channels 125 MHz, fmc stack.
 *  The ADC channels have 14 bits resolution. The module can use an
 *  external/internal clock source, and external/internal (software)
 *  trigger source.    
 *               
 *  @file
 *     
 */


 /****************************************************************************
 * NAME : MI125_Presence_send
 ************************************************************************//**
 *
 * 	Minimally verify if a fmc board is detected and mi125 fpga core is detected.
 * 
 * @warning 
 * - This function does not verify if a real mi125 board is used!
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @return
 *    Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_Presence_send(connection_state * state, unsigned nb);


 /****************************************************************************
 * NAME : MI125_powerup_send
 ************************************************************************//**
 *
 * 	Power up MI125 before used.
 * 
 * @warning 
 * - Function MI125_Presence_send() should be called to minimally verify the board
 *   presence before powering it up.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @return
 *    Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_powerup_send(connection_state * state, unsigned nb);


 /****************************************************************************
 * NAME : MI125_mi125_reset_send
 ************************************************************************//**
 *
 * Reset the mi125 module instance to default operating condition:
 * all 16 channels active, internal 125 MHz clock source, trig out disable,
 * 2compl output mode, adc randomize output off, lvds with termination and 1.75mA drive current,
 * testmode disable. After a reset, the module is already configure correctly
 * for adc capture. 
 * 
 * @warning 
 * - This function must be called at least once to reset this device to default
 *   operating condition before using other module functions to change the default
 *   operating mode.
 * 
 * @attention  
 * The module which is the clock master in a multi modules configuration
 * must be reset and configured first before the non master module. Resetting, reconfiguring
 * the clock source or changing any external clock source frequency for the clock master will
 * requiere that the non master module is calibrated again (see MI125_mi125_get_clkmaster_send()).
 * At reset, the default clock source is internal 125MHz (#MI125_CLKSRC125MHZ) for bottom
 * adapter, and fmc bottom clock source for top adapter (#MI125_CLKSRCBOTTOMFMC).     
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @return
 *    Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_reset_send(connection_state * state, unsigned nb);


 /****************************************************************************
 * NAME : MI125_mi125_digital_adccalibration_send
 ************************************************************************//**
 *
 * Force a digital calibration of all adc with active channels.
 * This function must be used each time adc clock input configuration / frequency is changed.
 * It must be used also each time the board active channels configuration is changed (when new
 * channels are added: this is handled internally).
 *  
 * @warning
 * - Failure to do a needed calibration could give unpredictable adc data captured.
 * - The MI125_mi125_reset_send() function must be call at least once before using this function.
 *  
 * @attention
 * A change of clock for the clock master implies that a calibration must be done
 * also for the module which is not the clock master (see MI125_mi125_set_clksrc_send() and
 * MI125_mi125_get_clkmaster_send()). If the clock frequency is changed for an external source,
 * the function MI125_mi125_clockreset_send() on the clock master must be called before doing a
 * calibration with this function.       
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_digital_adccalibration_send(connection_state * state, unsigned nb);


 /****************************************************************************
 * NAME : MI125_mi125_set_config_send
 ************************************************************************//**
 *
 * Configure the board with user specified custom options.
 * This function is optionnal and can be used if the default from MI125_mi125_reset_send()
 * for board operation is not what is desired.
 * The options that can be configured are: the number of active channels
 * by group of 4 (to lower board consumption and temperature increase when there are
 * unused channels), the adc lvds configuration (termination, drive current),
 * adc randomize output mode (for possible performance improvement if outputs crosstalk),
 * adc data format used (binary offset or 2 complement). 
 * 
 * @warning  
 * - The module MI125_mi125_reset_send() function must be call at least once before using this function.
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [in] groupch
 *    This is the configuration for channel grouping of the desired options.
 *
 * @param [in] lvds
 *    This is the configuration for lvds bus driving desired options.
 *
 * @param [in] randmode
 *    This is the configuration for data ouput randomize mode desired options.
 *
 * @param [in] binmode
 *    This is the configuration for output data format binary mode desired options.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_set_config_send(connection_state * state, unsigned nb, MI125_mi125_channel_mode groupch, MI125_mi125_lvds lvds, MI125_mi125_randmode randmode, MI125_mi125_binmode binmode);


 /****************************************************************************
 * NAME : MI125_mi125_set_clksrc_send
 ************************************************************************//**
 *
 * Configure the clock source that the boards ADC will used for all channels.
 * This function is optionnal and can be used if the default from MI125_mi125_reset_send()
 * for board operation is not what is desired.
 * The options that can be configured are: internal 125MHz, external clock,
 * for the TOP position adapter, it could also be clocked from the bottom adapter
 * to allow synchronize clocking for an adapter pair.
 * 
 * @warning  
 * - The module MI125_mi125_reset_send() function must be call at least once before using this function.
 * 
 * @attention
 * If a module instance is not the clock master (see MI125_mi125_get_clkmaster_send()),
 * (example the top adapter), its clock selection could only be #MI125_CLKSRCBOTTOMFMC; in this
 * situation, the bottom module must be configured to select the correct clock source input.
 * A change of clock for the clock master implies that a calibration must be done also for the
 * module which is not the clock master.    
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [in] clksrc
 *    This is the desired clock source.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_set_clksrc_send(connection_state * state, unsigned nb, MI125_mi125_clock_source clksrc);


 /****************************************************************************
 * NAME : MI125_mi125_checkcore_send
 ************************************************************************//**
 *
 * Attempt to detect the MI125 fpga core, for diagnostic purpose or evaluation
 * if the correct fpga core was loaded.
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [out] detected
 *    Returns if the mi125 core was detected or not.
 *    Detected is true (!=0) when the MI125 fpga core is detected correctly. 
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_checkcore_send(connection_state * state, unsigned nb, unsigned char *detected);


 /****************************************************************************
 * NAME : MI125_mi125_set_testmode_send
 ************************************************************************//**
 *
 * This function could be used to configure special test mode (mostly use at production time
 * for test purpose).
 * Here are some details about possible test modes:
 *  
 * #MI125_TESTMODEOFF:
 * All test modes are disabled.
 *  
 * #MI125_TESTMODE2:     
 * When test mode is active, each adc channel would be captured with user specified data pattern.
 * This is usefull to verify the reliability of the complete capture chain.
 * Only the 14 LSB bits of specified pattern are used for the test (2 MSB bits are force to 0)
 * The module MI125_mi125_digital_adccalibration_send() must have been done successfully last time
 * before using this function (this requierement is met after a successfull MI125_mi125_reset_send()).
 *    
 * #MI125_TESTMODE1:
 * This mode is similar as #MI125_TESTMODE2 explained above, but is mainly reserved for
 * production purpose.
 * @attention
 * The adc data captured in this mode is not necessary the specified pattern
 * because it could be done uncalibrated (so this mode could not be used to verify the 
 * reliability of the complete captured chain).
 *
 * For normal board ADC operation, test mode must be disable after use.
 * 
 * @warning
 * - Doing MI125_mi125_digital_adccalibration_send(), MI125_mi125_set_clksrc_send(), or MI125_mi125_set_config_send()
 *   when adding more channels, will disable the test mode automatically and revert to normal mode.
 * - The module MI125_mi125_reset_send() function must be call at least once before using this function.
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [in] test
 *    This is to configure what test mode to use or no test mode (see #MI125_mi125_testmode).
 *
 * @param [in] pattern
 *    14 bits test pattern used (2 MSB bits are ignored and force to 0).
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_set_testmode_send(connection_state * state, unsigned nb, MI125_mi125_testmode test, unsigned short pattern);


 /****************************************************************************
 * NAME : MI125_mi125_get_temperature_send
 ************************************************************************//**
 *
 * Read board pcb temperature in specified mode (resolution of 1C or 0.1C).
 * 
 * @attention
 * Board temperature is for information only. Different locations
 * on the board and environment conditions could affect the difference of temperature
 * seen on the board (like ADCs temperature). User is responsible to keep the
 * board temperature inside the allowed operating condition.
 * 
 * @warning     
 * - The module MI125_mi125_reset_send() function must be call at least once before using this function.
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [in] mode
 *    This is to select the temperature reading mode (resolution of 1C or 0.1C)
 *    (see #MI125_mi125_tempmode).
 *
 * @param [out] temp
 *    Returns the read temperature in 2 compl format. If the choosen mode is 1C resolution,
 *    the value is directly the temp in C. If the mode is #MI125_TEMP0DOT1C, the temperature
 *    returned is X 10 to allow returning an extra 0.x digit in an integer value.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_get_temperature_send(connection_state * state, unsigned nb, MI125_mi125_tempmode mode, short *temp);


 /****************************************************************************
 * NAME : MI125_mi125_get_channelcalibstatus_send
 ************************************************************************//**
 *
 * Read ADC channels calibration information. This info was the last time the
 * board ADC were calibrated (with MI125_mi125_digital_adccalibration_send()), manually
 * by user, or internally by other module functions.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [out] channellanecalib
 *    When channelcalibstatus=0, there was an
 *    error and channellanecalib shows all digital lanes calibration status, starting
 *    LSB bits for 1st adc 1st channel (2 lvds lanes each channel). All bits at 1 shows
 *    channels lanes that calibrated correctly, others are in problem. ADC channels
 *    which are not configured (unused) will show there lanes set to 0 (uncalibrated).  
 *
 * @param [out] channelcalibstatus
 *    When !=0, this means the channels calibration was successfull. If 0, there was an
 *    error and channellanecalib shows all digital lanes calibration status, starting
 *    LSB bits for 1st adc 1st channel (2 lvds lanes each channel). All bits at 1 shows
 *    channels lanes that calibrated correctly, others are in problem. 
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_get_channelcalibstatus_send(connection_state * state, unsigned nb, unsigned int *channellanecalib, unsigned int *channelcalibstatus);


 /****************************************************************************
 * NAME : MI125_mi125_get_versions_send
 ************************************************************************//**
 *
 * Read mi125 fpga core module version and driver version information.
 * This info could be used for diagnostic purpose.
 * 
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [out] core_version
 *    Returns MI125 fpga core version.
 *
 * @param [out] drv_version
 *    Returns MI125 software driver version.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_get_versions_send(connection_state * state, unsigned nb, unsigned int *core_version, unsigned int *drv_version);


 /****************************************************************************
 * NAME : MI125_mi125_checkadc_send
 ************************************************************************//**
 *
 * Check ADC communication for diagnostic purpose.
 * For each board ADC that is verified, this function reports an error if there is a problem.
 * It will also returns the written test pattern that caused problem and what verified
 * value was read.   
 *
 * @warning  
 * - MI125_mi125_reset_send() function should be called before using this function to reset the board correctly.
 * - Even if the MI125_mi125_reset_send() function fails, an attempt to call MI125_mi125_checkadc_send() can be made later.
 * - Test mode must not be active before using this function (see MI125_mi125_set_testmode_send()). 
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [in] adcid
 *    Which ADC we want to verify (#MI125_ADC0, #MI125_ADC1, #MI125_ADC2 or #MI125_ADC3).
 *
 * @param [out] written
 *    Returns written test pattern
 *
 * @param [out] verify
 *    Returns verified test pattern (should be the same as written when no error).
 *
 * @param [out] detected
 *    return a value =0 if the tested adc was not detected correctly, !=0 if it was OK.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_checkadc_send(connection_state * state, unsigned nb, unsigned adcid, unsigned char *written, unsigned char *verify,  unsigned char *detected);


 /****************************************************************************
 * NAME : MI125_mi125_set_trigout_send
 ************************************************************************//**
 *
 * Configure the trigger out IO to be disconnected (default) or connected
 * from FPGA to outside.
 * The options that can be configured are: trigger out enable or disable.
 * 
 * @warning
 * - The module MI125_mi125_reset_send() function must be called at least once before using this function.
 * - Because the trigger out function is shared with the trigger in pin,
 *   the trig out functionnality must be disabled if the external trig pin is used for
 *   trigger input.
 *    
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [in] trigout
 *    This is the desired trigger out configuration (see #MI125_mi125_trigout).
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_set_trigout_send(connection_state * state, unsigned nb, MI125_mi125_trigout trigout);


 /****************************************************************************
 * NAME : MI125_mi125_get_clkmaster_send
 ************************************************************************//**
 *
 * Detect if this current module instance is a clock master for the FPGA (uses its own MMCM).
 * In a multiple modules configuration, the FPGA clock master is the one that
 * provides its own DCO clock for the ADC data captured process. Normally, only
 * the module instance which is defined as clock master can use the function related
 * to clock source selection (MI125_mi125_set_clksrc_send()) to select internal (#MI125_CLKSRC125MHZ),
 * external (#MI125_CLKSRCEXT) clock source, etc. If external clock source is selected for the
 * clock master module, it must be connected on the EC signal of this module connector.
 * The module which is not the FPGA clock master can only selects as an ADC clock source,
 * the other module (#MI125_CLKSRCBOTTOMFMC).       
 *
 * @warning  
 * - In a multiple modules configuration, if there is only a single module configured as the
 *   clock master, it must be configured first.  
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [out] master
 *    Returns if the mi125 core instance is the clock master or not.
 *    Master is true (!=0) when the MI125 fpga core instance is the clock master. 
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_get_clkmaster_send(connection_state * state, unsigned nb, unsigned int *master);


 /****************************************************************************
 * NAME : MI125_mi125_clockreset_send
 ************************************************************************//**
 *
 * Force a MCM clock reset. This function is related to the adc calibration and clock operation.
 * Each time the adc clock source is changed, this function must be used before doing and adc
 * calibration with MI125_mi125_digital_adccalibration_send(). While this is handled internally
 * automatically when the other module functions are used, there is a case when the clock
 * source input is configured to external clock source (or other fmc module clock source for a
 * module which is not a clock master), and it is not possible to detect when the user
 * is changing externally the clock frequency for example. For this situation, a clock reset
 * must be used before running a new adc calibration with MI125_mi125_digital_adccalibration_send().
 *
 * @warning  
 * - In a multiple modules configuration, if there is only a single module configured as the
 *   clock master, if this function is called on the clock master, the other module instance must
 *   be also recalibrated the same way as the master (see MI125_mi125_get_clkmaster_send()).  
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_clockreset_send(connection_state * state, unsigned nb);


 /****************************************************************************
 * NAME : MI125_mi125_scan_i2c_send
 ************************************************************************//**
 *
 * Scan the i2c bus to detect all module devices.
 * This function is used for production purpose to check if all devices for module
 * operation are detected correctly on the i2c bus. This function is not
 * used in the normal user module operation. 
 *
 * @warning  
 * - MI125_mi125_reset_send() function must be called before using this function to reset the board correctly.
 * - Even if the MI125_mi125_reset_send() function fails, an attempt to call MI125_mi125_scan_i2c_send() can be made later.
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [in] scanmax
 *    This is the maximum number of devices that must be detected to avoid a scan error.
 *    When 0 is specified, this parameter is ignored. When used on a perseus fmc carrier, with a
 *    single mi125 module, this parameter should be 5 devices. 
 *
 * @param [out] scanraw
 *    Returns the raw i2c scan results structure (each detected device address in the vector
 *    hold != 0 when a device is detected)
 *
 * @param [out] scan_res
 *    For each of the device types of the module, this will return != 0 if the device has been
 *    detected correctly. All device types must be detected for the module to work
 *    correctly.
 *
 * @param [out] ndevices
 *    Number of i2c devices detected on the bus.
 *
 * @param [out] detect_ok
 *    This is the global scan result. This will hold true (!=0) when all the device types have
 *    been detected and no more devices than scanmax are detected on the bus (scanmax is ignored
 *    when set to 0).
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_scan_i2c_send(connection_state * state, unsigned nb, unsigned char scanmax, MI125_mi125_i2cscan *scanraw,
                                 MI125_mi125_i2caddr *scan_res, unsigned char *ndevices, unsigned char *detect_ok);


 /****************************************************************************
 * NAME : MI125_mi125_test_localoscsw_send
 ************************************************************************//**
 *
 * Test the local onboard clock oscillator switch control.
 * This function is used for production purpose to check if the onboard clock
 * control is functionnal. This function is not
 * used in the normal user module operation. 
 *
 * @warning  
 * - MI125_mi125_reset_send() function must be called before using this function to reset the board correctly.
 * - This test will configure the clock source to the local oscillator and will not revert
 *   to the clock source used before doing this test (if this is the top adapter, it
 *   will revert to  MI125_CLKSRCBOTTOMFMC if the top adapter is not a clock master).
 *
 * @attention
 * If a module instance is not the clock master (see MI125_mi125_get_clkmaster_send()),
 * (example the top adapter), a change of clock for the clock master implies that a calibration
 * must be done also for the module which is not the clock master after running this function.    
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Board instance to use. Instance number starts at 1.
 *
 * @param [out] result_ok
 *    This is the global test result. This will hold true (!=0) when the test is successfull,
 *    otherwise there is a problem with the local oscillator sw when set to 0).
 *
 * @return   Error code: #MI125_MI125OK if no problem is found, !=#MI125_MI125OK if problem.
 *           Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 ***************************************************************************/ 
Result MI125_mi125_test_localoscsw_send(connection_state * state, unsigned nb, unsigned char *result_ok);


#ifdef __cplusplus
}
#endif



#endif

