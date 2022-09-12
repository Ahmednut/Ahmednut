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
 * @file       Mo1000DevCom.h
 * @brief      communication wrapper data structure definitions for all devices control
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.3 $
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

#ifndef MO1000DEVCOM_DEFINES_H__
#define MO1000DEVCOM_DEFINES_H__

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include "bas_error.h"
#include "carrier_service_def.h"


/****************************************************************************
 *                             Global constants                             *
 ****************************************************************************/

// Use to generate correct SPI CS for each DAC
#define DEVCOM_SPI_CS_DAC1 (1<<0)
#define DEVCOM_SPI_CS_DAC2 (1<<1)
#define DEVCOM_SPI_CS_PLL  (1<<2)


typedef struct Mo1000DevCom_StructI2cRelease {
// i2c bus ownership register definition
#ifdef _BIG_ENDIAN 
    unsigned m_uReserved:30, m_uMmcI2cReleaseAck:1, m_uMmcI2cReleaseReq:1;
#else
    unsigned m_uMmcI2cReleaseReq:1, m_uMmcI2cReleaseAck:1, m_uReserved:30;
#endif                
} Mo1000DevCom_stI2cRelease;


// main structure for module operation
typedef struct Mo1000DevCom_StructComCore {
    Carrier_handle_t hCarrier;      ///< Carrier handle to access the carrier specific services
    unsigned int zone;        ///< I2c zone to access the mo1000 devices
    unsigned char m_ucSpiClockRate;
    unsigned char m_ucBridgeAddr;
    unsigned char m_ucSensorAddr;
    unsigned char m_ucExpanderAddr;
    unsigned char m_ucEepromAddr;
} Mo1000DevCom_stComCore;


// List of I2C logical devices
typedef enum Mo1000DevCom_EnumDevices {
    eMo1000DevComDeviceDac1 = 0,
    eMo1000DevComDeviceDac2 = 1,
    eMo1000DevComDevicePll = 2,
    eMo1000DevComDeviceI2cBridge = 3,
    eMo1000DevComDeviceSensors = 4,
    eMo1000DevComDevicePorts = 5,
    eMo1000DevComDeviceEeprom = 6
} Mo1000DevCom_eDevices;

// List of errors
enum Mo1000DevCom_eError {
    eDevComOk                  = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_MO1000DEVCOM, 0),
    eDevComUnimplemented01     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 1),  /**> Com direction with this device is UNIMPLEMENTED location 01 */ 
    eDevComFailed01            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 2),  /**> Error during I2C transfer location 01 */
    eDevComBusyTimeOut01       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 3),  /**> I2C bus busy timeout location 01 */
    eDevComRequestTimeOut      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 4),  /**> I2C bus ownership request timeout */
    eDevComInvalidDataBuffer01 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 5),  /**> Invalid data buffer given location 01 */
    eDevComBeforeTimeOut01     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 6),  /**> I2C bus busy before operation timeout location 01 */
    eDevComReleaseTimeOut      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 7),  /**> I2C bus ownership release timeout */ 
    eDevComUnimplemented02     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 8),  /**> Com direction with this device is UNIMPLEMENTED location 02 */ 
    eDevComFailed02            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 9),  /**> Error during I2C transfer location 02 */
    eDevComFailed03            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 10), /**> Error during I2C transfer location 03 */
    eDevComFailed04            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 11), /**> Error during I2C transfer location 04 */
    eDevComFailed05            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 12), /**> Error during I2C transfer location 05 */
    eDevComFailed06            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 13), /**> Error during I2C transfer location 06 */
    eDevComFailed07            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 14), /**> Error during I2C transfer location 07 */
    eDevComFailed08            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 15), /**> Error during I2C transfer location 08 */
    eDevComFailed09            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 16), /**> Error during I2C transfer location 09 */
    eDevComFailed10            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 17), /**> Error during I2C transfer location 10 */
    eDevComFailed11            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 18), /**> Error during I2C transfer location 11 */
    eDevComFailed12            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 19), /**> Error during I2C transfer location 12 */
    eDevComFailed13            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 20), /**> Error during I2C transfer location 13 */
    eDevComBusyTimeOut02       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 21), /**> I2C bus busy timeout location 02 */
    eDevComBusyTimeOut03       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 22), /**> I2C bus busy timeout location 03 */
    eDevComBusyTimeOut04       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 23), /**> I2C bus busy timeout location 04 */
    eDevComBusyTimeOut05       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 24), /**> I2C bus busy timeout location 05 */
    eDevComBusyTimeOut06       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 25), /**> I2C bus busy timeout location 06 */
    eDevComBusyTimeOut07       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 26), /**> I2C bus busy timeout location 07 */
    eDevComBusyTimeOut08       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 27), /**> I2C bus busy timeout location 08 */
    eDevComBusyTimeOut09       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 28), /**> I2C bus busy timeout location 09 */
    eDevComBusyTimeOut10       = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 29), /**> I2C bus busy timeout location 10 */
    eDevComInvalidDataBuffer02 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 30), /**> Invalid data buffer given location 02 */
    eDevComInvalidDataBuffer03 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 31), /**> Invalid data buffer given location 03 */
    eDevComInvalidDataBuffer04 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 32), /**> Invalid data buffer given location 04 */
    eDevComBeforeTimeOut02     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 33), /**> I2C bus busy before operation timeout location 02 */
    eDevComBeforeTimeOut03     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 34), /**> I2C bus busy before operation timeout location 03 */
    eDevComBeforeTimeOut04     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 35), /**> I2C bus busy before operation timeout location 04 */
    eDevComBeforeTimeOut05     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 36), /**> I2C bus busy before operation timeout location 05 */
    eDevComBeforeTimeOut06     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 37), /**> I2C bus busy before operation timeout location 06 */
    eDevComBeforeTimeOut07     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 38), /**> I2C bus busy before operation timeout location 07 */
    eDevComBeforeTimeOut08     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 39), /**> I2C bus busy before operation timeout location 08 */
    eDevComBeforeTimeOut09     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 40), /**> I2C bus busy before operation timeout location 09 */
    eDevComBeforeTimeOut10     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 41), /**> I2C bus busy before operation timeout location 10 */
    eDevComBeforeTimeOut11     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 42), /**> I2C bus busy before operation timeout location 11 */
    eDevComInvalidDataBuffer05 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 43), /**> Invalid data buffer given location 05 */
    eDevComFailed14            = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_MO1000DEVCOM, 44)  /**> Error during I2C transfer location 14 */
};

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

