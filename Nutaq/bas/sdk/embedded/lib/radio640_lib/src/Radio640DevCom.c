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
 * @file       Radio640DevCom.c
 * @brief      Communication wrapper for all Radio640 devices control
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

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "bas_types.h"
#include "bas_error.h"
#include "Radio640DevCom.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "Radio640Spi.h"

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

// Use to generate correct SPI CS for each device
#define RADIO640DEVCOM_SPI_ID_TRX       (0)
#define RADIO640DEVCOM_SPI_ID_CLK       (1)

#define RADIO640DEVCOM_SPI_ID_DACOSC    (0)

#define RADIO640DEVCOM_SPI_ID_RFATT     (0)

// AD9361 SPI parameters
#define TRX_BITMASK_ADDR    (0x3FF)
#define TRX_BITMASK_DATA    (0xFF)
#define TRX_WRITE_CMD       (0x1)
#define TRX_READ_CMD        (0x0)
#define TRX_SPI_CLK_POL     (0)

#define TRX_SPI_CLK_FREQ         50e6
#define TRX_SPI_CLK_DOWNSAMPLING ((uint32_t)(100e6/TRX_SPI_CLK_FREQ))

// HMC7043 SPI parameters
#define CLK_BITMASK_ADDR    (0x1FFF)
#define CLK_BITMASK_DATA    (0xFF)
#define CLK_WRITE_CMD       (0x0)
#define CLK_READ_CMD        (0x1)
#define CLK_SPI_CLK_POL     (1)

#define CLK_SPI_CLK_FREQ         10e6
#define CLK_SPI_CLK_DOWNSAMPLING ((uint32_t)(100e6/CLK_SPI_CLK_FREQ))

// PE43705 SPI parameters
#define RFATT_BITMASK_DATA  (0x7F)
#define RFATT_ADDR1_REVA    (0x00)
#define RFATT_ADDR2_REVA    (0x01)
#define RFATT_ADDR1_REVB    (0x00)
#define RFATT_ADDR2_REVB    (0x01)

/****************************************************************************
 *                           Private Structure                              *
 ****************************************************************************/

// Main structure for module operation
typedef struct Radio640DevCom_stComCore {
    Radio640Spi_hSpi hSpiTrxClk;        ///< SPI bus to communicate with AD9361 and HMC7043
    Radio640Spi_hSpi hSpiDacOsc;        ///< SPI bus to communicate with DAC for the oscillator
    Radio640Spi_hSpi hSpiRfAtt;         ///< SPI bus to communicate with RF attenuators

    Carrier_handle_t hCarrier;          ///< Carrier handle to access the carrier specific services
    unsigned int Zone;            ///< I2c zone to access the mo1000 devices
    Radio640DevCom_stI2cAddr I2cAddr;   ///< I2C addresses
    uint8_t u8IsRevA;

} Radio640DevCom_stComCore;


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/

static Result _Radio640DevCom_WriteTrx(Radio640DevCom_stComCore * hCom, uint16_t u16Addr, uint8_t u8Data);
static Result _Radio640DevCom_ReadTrx(Radio640DevCom_hComCore hCom, uint16_t u16Addr, uint8_t * pu8Data);
static Result _Radio640DevCom_WriteClk(Radio640DevCom_hComCore hCom, uint16_t u16Addr, uint8_t u8Data);
static Result _Radio640DevCom_WriteDacOsc(Radio640DevCom_hComCore hCom, uint16_t u16Data);
static Result _Radio640DevCom_WriteRfAtt(Radio640DevCom_hComCore hCom, Radio640DevCom_eDevices Device, uint8_t u8Data);
static Result _Radio640DevCom_WriteTemp(Radio640DevCom_hComCore hCom, uint8_t u8Addr, uint16_t u16Data);
static Result _Radio640DevCom_ReadTemp(Radio640DevCom_hComCore hCom, uint8_t u8Addr, uint16_t * pu16Data);
static Result _Radio640DevCom_WritePower(Radio640DevCom_hComCore hCom, Radio640DevCom_eDevices Device, uint8_t u8Addr, uint16_t u16Data);
static Result _Radio640DevCom_ReadPower(Radio640DevCom_hComCore hCom, Radio640DevCom_eDevices Device, uint8_t u8Addr, uint16_t * pu16Data);


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/

 /****************************************************************************
 *   NAME : _Radio640DevCom_WriteTrx
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to AD9361 transceiver device.
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u16Addr
 *      internal device register address where to start writing
 *
 *   @param [in] u8Data
 *      data to write
 *
 *   @param [in] ucSpiCs
 *      spi cs to use to select the correct adc (DEVCOM_SPI_CS_DAC1,DEVCOM_SPI_CS_DAC2)
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_WriteTrx(Radio640DevCom_stComCore * hCom, uint16_t u16Addr, uint8_t u8Data)
{
    Result res;

    uint32_t u32SpiCmd;

    res = Radio640Spi_SetClkPol(hCom->hSpiTrxClk, TRX_SPI_CLK_POL);
    if(FAILURE(res)) return res;

    u32SpiCmd = 0;                                  // Clear
    u32SpiCmd |= (TRX_WRITE_CMD << 23);             // Write command
    u32SpiCmd |= (u16Addr & TRX_BITMASK_ADDR) << 8; // Address
    u32SpiCmd |= (u8Data & TRX_BITMASK_DATA) << 0;  // Data

    res = Radio640Spi_Transfer(hCom->hSpiTrxClk, RADIO640DEVCOM_SPI_ID_TRX, TRX_SPI_CLK_DOWNSAMPLING, u32SpiCmd, NULL);
    if(FAILURE(res)) return res;

    return eRadio640DevComOk;
}


 /****************************************************************************
 *   NAME : _Radio640DevCom_ReadTrx
 ************************************************************************//**
 *
 *   Read up to 1 X 8-bit transfer from AD9361 transceiver device.
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u16Addr
 *      internal device register address where to start reading
 *
 *   @param [out] pu8Data
 *      data read
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_ReadTrx(Radio640DevCom_hComCore hCom, uint16_t u16Addr, uint8_t * pu8Data)
{
    Result res;

    uint32_t u32SpiData;
    uint32_t u32SpiCmd;

    res = Radio640Spi_SetClkPol(hCom->hSpiTrxClk, TRX_SPI_CLK_POL);
    if(FAILURE(res)) return res;

    u32SpiCmd = 0;                                  // Clear
    u32SpiCmd |= (TRX_READ_CMD << 23);              // Read command
    u32SpiCmd |= (u16Addr & TRX_BITMASK_ADDR) << 8; // Address

    res = Radio640Spi_Transfer(hCom->hSpiTrxClk, RADIO640DEVCOM_SPI_ID_TRX, TRX_SPI_CLK_DOWNSAMPLING, u32SpiCmd, &u32SpiData);
    if(FAILURE(res)) return res;

    // Check if data buffer is not empty
    if (pu8Data)
        *pu8Data = (uint8_t)(u32SpiData & TRX_BITMASK_DATA);
    else
        return eRadio640DevComInvalidDataBuffer01;

    return eRadio640DevComOk;
}


 /****************************************************************************
 *   NAME : _Radio640DevCom_WriteClk
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to HMC7043 clock device.
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u16Addr
 *      internal device register address where to start writing
 *
 *   @param [in] u8Data
 *      data to write
 *
 *   @param [in] ucSpiCs
 *      spi cs to use to select the correct adc (DEVCOM_SPI_CS_DAC1,DEVCOM_SPI_CS_DAC2)
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_WriteClk(Radio640DevCom_hComCore hCom, uint16_t u16Addr, uint8_t u8Data)
{
    Result res;

    uint32_t u32SpiCmd;

    res = Radio640Spi_SetClkPol(hCom->hSpiTrxClk, CLK_SPI_CLK_POL);
    if(FAILURE(res)) return res;

    u32SpiCmd = 0;                                  // Clear
    u32SpiCmd |= (CLK_WRITE_CMD << 23);             // Write command
    u32SpiCmd |= (u16Addr & CLK_BITMASK_ADDR) << 8; // Address
    u32SpiCmd |= (u8Data & CLK_BITMASK_DATA) << 0;  // Data

    res = Radio640Spi_Transfer(hCom->hSpiTrxClk, RADIO640DEVCOM_SPI_ID_CLK, CLK_SPI_CLK_DOWNSAMPLING, u32SpiCmd, NULL);
    if(FAILURE(res)) return res;

    return eRadio640DevComOk;
}


 /****************************************************************************
 *   NAME : _Radio640DevCom_WriteDacOsc
 ************************************************************************//**
 *
 *   Write up to 1 X 16-bit transfer to LTC2641-16 clock device.
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u16Data
 *      data to write
 *
 *   @param [in] ucSpiCs
 *      spi cs to use to select the correct adc (DEVCOM_SPI_CS_DAC1,DEVCOM_SPI_CS_DAC2)
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_WriteDacOsc(Radio640DevCom_hComCore hCom, uint16_t u16Data)
{
    Result res;

    res = Radio640Spi_Transfer(hCom->hSpiDacOsc, RADIO640DEVCOM_SPI_ID_DACOSC, 0, u16Data, NULL);
    if(FAILURE(res)) return res;

    return eRadio640DevComOk;
}


 /****************************************************************************
 *   NAME : _Radio640DevCom_WriteRfAtt
 ************************************************************************//**
 *
 *   Write up to 1 X 16-bit transfer to the Rf attenuator.
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u16Data
 *      data to write
 *
 *   @param [in] ucSpiCs
 *      spi cs to use to select the correct adc (DEVCOM_SPI_CS_DAC1,DEVCOM_SPI_CS_DAC2)
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_WriteRfAtt(Radio640DevCom_hComCore hCom, Radio640DevCom_eDevices Device, uint8_t u8Data)
{
    Result res;

    uint16_t u16Cmd;
    
    uint8_t u8AddrRfAtt1;
    uint8_t u8AddrRfAtt2;

    u16Cmd = 0;
    u16Cmd |= (u8Data & RFATT_BITMASK_DATA) << 0;
    
    if(hCom->u8IsRevA)
    {
        u8AddrRfAtt1 = RFATT_ADDR1_REVA;
        u8AddrRfAtt2 = RFATT_ADDR2_REVA;
    }
    else
    {
        u8AddrRfAtt1 = RFATT_ADDR1_REVB;
        u8AddrRfAtt2 = RFATT_ADDR2_REVB;
    }

    if(Device == eRadio640DeviceRfAtt1)
        u16Cmd |= (u8AddrRfAtt1 << 8);
    else if(Device == eRadio640DeviceRfAtt2)
        u16Cmd |= (u8AddrRfAtt2 << 8);
    else
        return eRadio640DevComUnimplemented01;

    res = Radio640Spi_Transfer(hCom->hSpiRfAtt, RADIO640DEVCOM_SPI_ID_RFATT, 0, u16Cmd, NULL);
    if(FAILURE(res)) return res;

    return eRadio640DevComOk;
}


 /****************************************************************************
 *   NAME : _Radio640DevCom_WriteTemp
 ************************************************************************//**
 *
 *   Write up to 1 X 16-bit transfer to a temperature sensor device
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u8Addr
 *      internal device register address where to start writing
 *
 *   @param [in] u16Data
 *      data to write
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_WriteTemp(Radio640DevCom_hComCore hCom, uint8_t u8Addr, uint16_t u16Data)
{
    const int iTransferSize = 2;
    unsigned char aucBuffer[2];
    int iSentByteCount;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    aucBuffer[0] = u16Data >> 8;
    aucBuffer[1] = u16Data & 0xFF;

    // Write i2c message
    iSentByteCount = Carrier_I2cWrite(hCom->hCarrier, hCom->Zone, hCom->I2cAddr.u8Temp, u8Addr, aucBuffer, iTransferSize, options);

    // Check if transfer went correctly
    if (iSentByteCount != iTransferSize) return eRadio640DevComFailed01;

    return eRadio640DevComOk;
}

 /****************************************************************************
 *   NAME : _Radio640DevCom_ReadTemp
 ************************************************************************//**
 *
 *   Read up to 1 X 16-bit transfer from a temperature sensor device
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u8Addr
 *      Internal device register address where to start writing
 *
 *   @param [out] pu16Data
 *      Pointer that will hold the read data
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_ReadTemp(Radio640DevCom_hComCore hCom, uint8_t u8Addr, uint16_t * pu16Data)
{
    unsigned char aucBuffer[2];
    int receivedByteCount;
    const int iReadSize = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cRead(hCom->hCarrier, hCom->Zone, hCom->I2cAddr.u8Temp, u8Addr, aucBuffer, iReadSize, options);

    // Check if data buffer is not empty
    if (pu16Data)
    {
        *pu16Data = aucBuffer[0] << 8 | aucBuffer[1];
        if (u8Addr != 1)
        {
        	// Remove don't care bits
        	*pu16Data &= 0xFF80;
        }
    }
    else
        return eRadio640DevComInvalidDataBuffer02;

    // Check if we read the correct number of byte
    if (receivedByteCount != iReadSize ) return eRadio640DevComFailed03;

    return eRadio640DevComOk;
}


 /****************************************************************************
 *   NAME : _Radio640DevCom_WritePower
 ************************************************************************//**
 *
 *   Write up to 1 X 16-bit transfer to a power sensor device
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u8Addr
 *      internal device register address where to start writing
 *
 *   @param [in] u16Data
 *      data to write
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_WritePower(Radio640DevCom_hComCore hCom, Radio640DevCom_eDevices Device, uint8_t u8Addr, uint16_t u16Data)
{
    const int iTransferSize = 2;
    unsigned char aucBuffer[2];
    int iSentByteCount;
    I2C_Options_t options;
    options.nbByteAddr = 1;
    uint8_t u8I2cAddr;

    if(Device == eRadio640DevicePower12V)
        u8I2cAddr = hCom->I2cAddr.u8Power12V;
    else if(Device == eRadio640DevicePowerVadj)
        u8I2cAddr = hCom->I2cAddr.u8PowerVadj;
    else
        return eRadio640DevComUnimplemented02;

    aucBuffer[0] = u16Data >> 8;
    aucBuffer[1] = u16Data & 0xFF;

    // Write i2c message
    iSentByteCount = Carrier_I2cWrite(hCom->hCarrier, hCom->Zone, u8I2cAddr, u8Addr, aucBuffer, iTransferSize, options);

    // Check if transfer went correctly
    if (iSentByteCount != iTransferSize) return eRadio640DevComFailed02;

    return eRadio640DevComOk;
}

 /****************************************************************************
 *   NAME : _Radio640DevCom_ReadPower
 ************************************************************************//**
 *
 *   Read up to 1 X 16-bit transfer from a power sensor device
 *
 *   @param [in] hCom
 *      Handle returned by Radio640DevCom_Open function
 *
 *   @param [in] u8Addr
 *      Internal device register address where to start writing
 *
 *   @param [out] pu16Data
 *      Pointer that will hold the read data
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup DEVCOM
 *
 ***************************************************************************/
static Result _Radio640DevCom_ReadPower(Radio640DevCom_hComCore hCom, Radio640DevCom_eDevices Device, uint8_t u8Addr, uint16_t * pu16Data)
{
    unsigned char aucBuffer[2];
    int receivedByteCount;
    const int iReadSize = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;
    uint8_t u8I2cAddr;

    if(Device == eRadio640DevicePower12V)
        u8I2cAddr = hCom->I2cAddr.u8Power12V;
    else if(Device == eRadio640DevicePowerVadj)
        u8I2cAddr = hCom->I2cAddr.u8PowerVadj;
    else
        return eRadio640DevComUnimplemented03;

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cRead(hCom->hCarrier, hCom->Zone, u8I2cAddr, u8Addr, aucBuffer, iReadSize, options);

    // Check if data buffer is not empty
    if (pu16Data)
    {
        *pu16Data = aucBuffer[0] << 8 | aucBuffer[1];
    }
    else
        return eRadio640DevComInvalidDataBuffer03;

    // Check if we read the correct number of byte
    if (receivedByteCount != iReadSize ) return eRadio640DevComFailed04;

    return eRadio640DevComOk;
}

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : Radio640DevCom_Open
 ************************************************************************//**
 *
 *   Open the Radio640DevCom module before use.
 *   This function must be called before using any other function of this module.
 *
 *   @param [out] hCom
 *      returns a handle to a communication core data structure
 *
 *   @param [in] u8IsRevA
 *      If the Radio640 hardware revision is A, this parameter is 1, else 0.
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
Result Radio640DevCom_Open(Radio640DevCom_hComCore * phCom,
    Carrier_handle_t hCarrier, unsigned int Zone,
    uint32_t u32TrxClkSpiRegAddr,
    uint32_t u32TrxClkSpiReadRegAddr,
    uint32_t u32TrxClkSpiClkDownsamplingRegAddr,
    uint32_t u32DacOscSpiRegAddr,
    uint32_t u32RfAttSpiRegAddr,
    Radio640DevCom_stI2cAddr I2cAddr,
    uint32_t u8IsRevA)
{
    Result res;

    Radio640DevCom_hComCore hCom;

    Carrier_pfUsleep pfUsleep;

    pfUsleep = Carrier_GetUsleepFunc(hCarrier);
    if(pfUsleep == NULL) return -1;

    // Allocate Radio640 ComCore structure
    hCom = (Radio640DevCom_hComCore)malloc(sizeof(Radio640DevCom_stComCore));
    if(hCom == NULL) return -1;
    memset(hCom,0,sizeof(Radio640DevCom_stComCore));
    *phCom = hCom;

    // SPI devices
    res = Radio640Spi_Open(&(hCom->hSpiTrxClk), u32TrxClkSpiRegAddr, u32TrxClkSpiReadRegAddr, pfUsleep, u32TrxClkSpiClkDownsamplingRegAddr);
    if(FAILURE(res)) goto error;

    res = Radio640Spi_Open(&(hCom->hSpiDacOsc), u32DacOscSpiRegAddr, 0, pfUsleep, 0);
    if(FAILURE(res)) goto error;

    res = Radio640Spi_Open(&(hCom->hSpiRfAtt), u32RfAttSpiRegAddr, 0, pfUsleep, 0);
    if(FAILURE(res)) goto error;

    // I2C devices
    hCom->hCarrier = hCarrier;
    hCom->Zone = Zone;
    hCom->I2cAddr = I2cAddr;
    hCom->u8IsRevA = u8IsRevA;

    return eRadio640DevComOk;

error:

    Radio640DevCom_Close(&hCom);

    return res;
}

 /****************************************************************************
 *   NAME : Radio640DevCom_Close
 ************************************************************************//**
 *
 *   Close the Radio640DevCom module when not needed anymore.
 *   Radio640DevCom_Open must be called before using this function.
 *
 *   @param [in,out] phCom
 *      Pointer to the Handle returned by Radio640DevCom_Open function
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
Result Radio640DevCom_Close(Radio640DevCom_hComCore * phCom)
{
    Radio640DevCom_hComCore hCom = *phCom;

    if(hCom)
    {
        Radio640Spi_Close(&(hCom->hSpiTrxClk));
        Radio640Spi_Close(&(hCom->hSpiDacOsc));
        Radio640Spi_Close(&(hCom->hSpiRfAtt));

        free(*phCom);
        *phCom = NULL;
    }

    return eRadio640DevComOk;
}


 /****************************************************************************
 *   NAME : Radio640DevCom_WriteArray
 ************************************************************************//**
 *
 *   Write an array of data to a specified device.
 *   Radio640DevCom_Open must be called before using this function.
 *
 *   @param [in] hCom
 *      structure returns from Radio640DevCom_Open function
 *
 *   @param [in] Device
 *      which logical device to write
 *
 *   @param [in] u16Addr
 *      at which internal device address starting to write
 *
 *   @param [in] pData
 *      data buffer used for writing
 *
 *   @param [in] i32Size
 *      number of bytes to write
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
Result Radio640DevCom_WriteArray(void * hCom, uint32_t u32DeviceId, uint16_t u16Addr, const void * pData, int32_t i32Size)
{
    Result res;
    int iCount;
    Radio640DevCom_eDevices Device = (Radio640DevCom_eDevices)u32DeviceId;
    Radio640DevCom_hComCore hComCore = (Radio640DevCom_hComCore)hCom;

    // Which device we need to write by looping one item size at a time (vary according to device)
    switch(Device)
    {
        case eRadio640DeviceTrx:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_WriteTrx(hComCore, u16Addr+iCount, ((uint8_t*)pData)[iCount]);
                if(FAILURE(res)) {return res;}
            }
            break;

        case eRadio640DeviceClk:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_WriteClk(hComCore, u16Addr+iCount, ((uint8_t*)pData)[iCount]);
                if(FAILURE(res)) {return res;}
            }
            break;

        case eRadio640DeviceDacOsc:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_WriteDacOsc(hComCore, ((uint16_t*)pData)[iCount]);
                if(FAILURE(res)) {return res;}
            }
            break;

        case eRadio640DeviceRfAtt1:
        case eRadio640DeviceRfAtt2:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_WriteRfAtt(hComCore, Device, ((uint8_t*)pData)[iCount]);
                if(FAILURE(res)) {return res;}
            }
            break;

        case eRadio640DeviceTemp:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_WriteTemp(hComCore, (uint8_t)(u16Addr+iCount), ((uint16_t*)pData)[iCount]);
                if(FAILURE(res)) {return res;}
            }
            break;

        case eRadio640DevicePower12V:
        case eRadio640DevicePowerVadj:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_WritePower(hComCore, Device, (uint8_t)(u16Addr+iCount), ((uint16_t*)pData)[iCount]);
                if(FAILURE(res)) {return res;}
            }
            break;

        // This device types are currently not supported
        default:
            return eRadio640DevComUnimplemented04;
    }

    return eRadio640DevComOk;
}



 /****************************************************************************
 *   NAME : Radio640DevCom_ReadArray
 ************************************************************************//**
 *
 *   Read an array of data from a specified device.
 *   Radio640DevCom_Open must be called before using this function.
 *
 *   @param [in] hCom
 *      structure returns from Radio640DevCom_Open function
 *
 *   @param [in] Device
 *      which logical device to read
 *
 *   @param [in] u16Addr
 *      at which internal device address starting to read
 *
 *   @param [out] pData
 *      data buffer used for reading
 *
 *   @param [in] i32Size
 *      number of bytes to read
 *
 *   @return   Error code: eRadio640DevComOk if no problem is found, !eRadio640DevComOk if problem
 *             Use FAILURE(res) macro to detect if an error occurred.
 *
 *   @ingroup RADIO640DEVCOM
 *
 ***************************************************************************/
Result Radio640DevCom_ReadArray(void * hCom, uint32_t u32DeviceId, uint16_t u16Addr, void * pData, int32_t i32Size)
{
    Result res;
    int iCount;
    Radio640DevCom_eDevices Device = (Radio640DevCom_eDevices)u32DeviceId;
    Radio640DevCom_hComCore hComCore = (Radio640DevCom_hComCore)hCom;

    // select which i2c device type
    switch(Device)
    {
        case eRadio640DeviceTrx:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_ReadTrx(hComCore, u16Addr+iCount, &(((unsigned char*)pData)[iCount]));
                if(FAILURE(res)) {return res;}
            }
            break;

        case eRadio640DeviceTemp:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_ReadTemp(hComCore, (uint8_t)(u16Addr+iCount), &(((uint16_t*)pData)[iCount]));
                if(FAILURE(res)) {return res;}
            }
            break;

        case eRadio640DevicePower12V:
        case eRadio640DevicePowerVadj:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                res = _Radio640DevCom_ReadPower(hComCore, Device, (uint8_t)(u16Addr+iCount), &(((uint16_t*)pData)[iCount]));
                if(FAILURE(res)) {return res;}
            }
            break;

        // This device types are currently not supported
        default:
            return eRadio640DevComUnimplemented02;
    }

    return eRadio640DevComOk;
}


