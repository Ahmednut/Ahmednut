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
 * @file       Mo1000DevCom.c
 * @brief      Communication wrapper for all devices control
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
#include "Mo1000DevCom.h"

#include "carrier_service.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                             Local constants                              *
 ****************************************************************************/

// I2C bridge commands
#define BRIDGE_CONFIG_CMD 0xF0
#define BRIDGE_IDLE_CMD   0xF2


/****************************************************************************
 *                           Forward Declarations                           *
 ****************************************************************************/


/****************************************************************************
 *                             Local variables                              *
 ****************************************************************************/
static const unsigned short s_usI2cBridgeSpiSpeed[] = {1843, 461, 115, 58 };
static unsigned char s_ucI2cBridgeInitVal;


/****************************************************************************
 *                       Private functions prototypes                       *
 ****************************************************************************/
static Result _DevCom_SetSpiSpeed(Mo1000DevCom_stComCore * pCore, unsigned char ucSpeed);
static Result _DevCom_WriteDac(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned char ucData, unsigned char ucSpiCs);
static Result _DevCom_WriteSensors(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned short usData);
static Result _DevCom_WriteBridge(Mo1000DevCom_stComCore * pCore, unsigned char ucAddr, unsigned char ucData, unsigned char ucSize);
static Result _DevCom_WriteExpander(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned char ucData);
static Result _DevCom_WritePll(Mo1000DevCom_stComCore * pCore, unsigned char ucAddr, unsigned uData);
static Result _DevCom_ReadDac(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned char * ucData, unsigned char ucSpiCs);
static Result _DevCom_ReadSensors(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, short * sData);
static Result _DevCom_ReadBridge(Mo1000DevCom_stComCore * pCore, unsigned char *ucData);
static Result _DevCom_ReadExpander(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned char * ucData);
static Result _DevCom_ReadPll(Mo1000DevCom_stComCore * pCore, unsigned char ucAddr, unsigned * uData);


/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : _DevCom_SetSpiSpeed
 ************************************************************************//**
 *
 *   Configure the I2C/SPI bridge for correct SPI devices operation.
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] ucSpeed
 *      speed of spi clock on the bridge
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_SetSpiSpeed(Mo1000DevCom_stComCore * pCore, unsigned char ucSpeed)
{
    // Speed is only 2 bits (4 values)
    ucSpeed &= 0x3;
    pCore->m_ucSpiClockRate = ucSpeed;
    s_ucI2cBridgeInitVal &= ~0x3;
    s_ucI2cBridgeInitVal |= ucSpeed;

    // Configure the spi bridge
    return _DevCom_WriteBridge(pCore, BRIDGE_CONFIG_CMD, s_ucI2cBridgeInitVal, 1);
}

 /****************************************************************************
 *   NAME : _DevCom_WriteDac
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to an DAC device.
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] usAddr
 *      internal device register address where to start writing
 *
 *   @param [in] ucData
 *      data to write
 *
 *   @param [in] ucSpiCs
 *      spi cs to use to select the correct adc (DEVCOM_SPI_CS_DAC1,DEVCOM_SPI_CS_DAC2)
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_WriteDac(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned char ucData, unsigned char ucSpiCs)
{
    unsigned char aucBuffer[3];
    int iSentByteCount;
    const int iTransferSize = 3;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //function id
    aucBuffer[0] = ucSpiCs;

    usAddr &= 0x7F;
    //SPI data
    //short instruction word
    aucBuffer[1] = usAddr;
    //data
    aucBuffer[2] = ucData;

    // Write the i2c message to the device
    iSentByteCount = Carrier_I2cWrite(pCore->hCarrier, pCore->zone, pCore->m_ucBridgeAddr, aucBuffer[0], aucBuffer+1, iTransferSize-1, options);

    // Check if all bytes where sent correctly
    if (iSentByteCount == (iTransferSize-1))
    {
        return eDevComOk;
    }else{
        return eDevComFailed01;
    }
}


 /****************************************************************************
 *   NAME : _DevCom_WriteSensors
 ************************************************************************//**
 *
 *   Write up to 1 X 16-bit transfer to a temperature sensor device
 *   (configuration reg needs needs both 8 bits char of the 16 bits been the same).
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] usAddr
 *      internal device register address where to start writing
 *
 *   @param [in] usData
 *      data to write
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_WriteSensors(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned short usData)
{
    unsigned char aucBuffer[3];
    int iSentByteCount;
    const int iTransferSize = 3;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    aucBuffer[0] = usAddr & 0xFF;
     //data
    aucBuffer[1] = usData >> 8;
    aucBuffer[2] = usData & 0xFF;

    // Write i2c message
    iSentByteCount = Carrier_I2cWrite(pCore->hCarrier, pCore->zone, pCore->m_ucSensorAddr, aucBuffer[0], aucBuffer+1, iTransferSize-1, options);

    // Check if transfer went correctly
    if (iSentByteCount == (iTransferSize-1))
    {
        return eDevComOk;
    }else{
        return eDevComFailed02;
    }
}


 /****************************************************************************
 *   NAME : _DevCom_WriteBridge
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to an I2C/SPI bridge device
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] ucAddr
 *      internal device register address where to start writing
 *
 *   @param [in] ucData
 *      data to write
 *
 *   @param [in] ucSize
 *      size of data to write (0 for nodata or 1 for one data)
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_WriteBridge(Mo1000DevCom_stComCore * pCore, unsigned char ucAddr, unsigned char ucData, unsigned char ucSize)
{
    unsigned char aucBuffer[2];
    int iSentByteCount;
    int iTransferSize = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    aucBuffer[0] = ucAddr;

    // If this command do not send any data
    if (ucSize == 0)
        iTransferSize--;
    else
        aucBuffer[1] = ucData;

    // Write the I2C message
    iSentByteCount = Carrier_I2cWrite(pCore->hCarrier, pCore->zone, pCore->m_ucBridgeAddr, aucBuffer[0], aucBuffer+1, iTransferSize-1, options);

    // Did the sending went correctly?
    if (iSentByteCount == (iTransferSize-1))
    {
        return eDevComOk;
    }else{
        return eDevComFailed03;
    }
}


 /****************************************************************************
 *   NAME : _DevCom_WriteExpander
 ************************************************************************//**
 *
 *   Write up to 1 X 8-bit transfer to an 16 bits (2 X 8 bits) io expander.
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] usAddr
 *      internal device register address where to start writing
 *
 *   @param [in] ucData
 *      data to write
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_WriteExpander(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned char ucData)
{
    unsigned char aucBuffer[2];
    int iSentByteCount;
    const int iTransferSize = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    aucBuffer[0] = usAddr & 0xff;
    aucBuffer[1] = ucData;

    // Write the I2C message
    iSentByteCount = Carrier_I2cWrite(pCore->hCarrier, pCore->zone, pCore->m_ucExpanderAddr, aucBuffer[0], aucBuffer+1, iTransferSize-1, options);

    // If data was sent correctly
    if (iSentByteCount == (iTransferSize-1))
    {
        return eDevComOk;
    }else{
        return eDevComFailed04;
    }
}


 /****************************************************************************
 *   NAME : _DevCom_WritePll
 ************************************************************************//**
 *
 *   Write up to 1 X 28-bit transfer to a PLL device.
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] ucAddr
 *      internal device register address where to start writing (4 bits)
 *
 *   @param [in] uData
 *      data to write (28 bits)
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_WritePll(Mo1000DevCom_stComCore * pCore, unsigned char ucAddr, unsigned uData)
{
    int iCount;
    unsigned char aucBuffer[5];
    unsigned uNewData;
    int iSentByteCount;
    const int iTransferSize = 5;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    // Keep only 28 bits of data with 4 bits address
    uData = ((uData & 0x0fffffff) << 4) | (ucAddr & 0xf);

    // Reverse the bit ordering because the bridge is setup to tx MSB 1st (and setup for LSB req i2c command)
    uNewData = uData;         // Get lsb bit from uData
    {
        iCount = sizeof(uData) * 8 - 1;

        uData >>= 1;
        while (uData)
        {
            uNewData <<= 1;
            uNewData |= (uData & 1);
            uData >>= 1;
            iCount--;
        }
        uNewData <<= iCount;
    }

    aucBuffer[0] = DEVCOM_SPI_CS_PLL;

    //SPI data (swap byte ordering to achieve LE for pll, because the bridge send only bytes in MSB and not words)
    //data1
    aucBuffer[4] = *((unsigned char *)(&uNewData));
    //data2
    aucBuffer[3] = *(((unsigned char *)(&uNewData)) + 1);
    //data3
    aucBuffer[2] = *(((unsigned char *)(&uNewData)) + 2);
    //data4
    aucBuffer[1] = *(((unsigned char *)(&uNewData)) + 3);

    // Write the i2c message to the device
    iSentByteCount = Carrier_I2cWrite(pCore->hCarrier, pCore->zone, pCore->m_ucBridgeAddr, aucBuffer[0], aucBuffer+1, iTransferSize-1, options);

    // Check if all bytes where sent correctly
    if (iSentByteCount == (iTransferSize-1))
    {
        return eDevComOk;
    }else{
        return eDevComFailed05;
    }
}


 /****************************************************************************
 *   NAME : _DevCom_ReadDac
 ************************************************************************//**
 *
 *   Read up to 1 X 8-bit transfer from a DAC device.
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] addr
 *      internal device register address where to start reading
 *
 *   @param [out] ucData
 *      data read
 *
 *   @param [in] ucSpiCs
 *      spi cs to use to select the correct adc (DEVCOM_SPI_CS_DAC1,DEVCOM_SPI_CS_DAC2)
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_ReadDac(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned char * ucData, unsigned char ucSpiCs)
{
    unsigned char aucBuffer[7];
    int iSentByteCount, receivedByteCount;
    const int iTransferSize = 3;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    //function id
    aucBuffer[0] = ucSpiCs;

    //SPI data
    //short instruction word
    aucBuffer[1] = (usAddr & 0x7F) | 0x80;
    //data
    aucBuffer[2] = 0xFF;

    // Write the i2c message to prepare the reading
    iSentByteCount = Carrier_I2cWrite(pCore->hCarrier, pCore->zone, pCore->m_ucBridgeAddr, aucBuffer[0], aucBuffer+1, iTransferSize-1, options);

    // This the read preparation was ok?
    if (iSentByteCount != (iTransferSize-1))
    {
        return eDevComFailed06;
    }

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cReadNoWrData(pCore->hCarrier, pCore->zone, pCore->m_ucBridgeAddr, aucBuffer, iTransferSize - 1);

    // Check if data buffer is not empty
    if (ucData)
        *ucData = aucBuffer[1];
    else
        return eDevComInvalidDataBuffer01;

    // Check if we read the correct number of byte
    if (receivedByteCount == (iTransferSize - 1) )
    {
        return eDevComOk;
    }else{
        return eDevComFailed07;
    }
}

 /****************************************************************************
 *   NAME : _DevCom_ReadSensors
 ************************************************************************//**
 *
 *   Read up to 1 X 16-bit transfer from a temperature sensor device
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] usAddr
 *      internal device register address where to start reading
 *
 *   @param [out] data
 *      data read
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_ReadSensors(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, short * data)
{
    unsigned char aucBuffer[5];
    int receivedByteCount;
    const int read_size = 2;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    usAddr &= 0xFF;

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cRead(pCore->hCarrier, pCore->zone, pCore->m_ucSensorAddr, usAddr, aucBuffer, read_size, options);

    // Check if data buffer is not empty
    if (data)
    {
        *data = aucBuffer[0] << 8 | aucBuffer[1];
        if (usAddr != 1)
        {
        	// Remove don't care bits
        	*data &= 0xFF80;
        }
    }
    else
        return eDevComInvalidDataBuffer02;

    // Check if we read the correct number of byte
    if (receivedByteCount == read_size )
    {
        return eDevComOk;
    }else{
        return eDevComFailed09;
    }
}


 /****************************************************************************
 *   NAME : _DevCom_ReadBridge
 ************************************************************************//**
 *
 *   Read up to 1 X 8-bit transfer from an I2C/SPI bridge device buffer.
 *   The bridge buffer must has been filled from a previous _DevCom_WriteBridge
 *   operation to read something meaningful here.
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [out] ucData
 *      data read
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_ReadBridge(Mo1000DevCom_stComCore * pCore, unsigned char *ucData)
{
    unsigned char aucBuffer[2];
    int iReceivedByteCount;
    const int iReadSize = 1;

    //read back the data from I2C bridge buffer
    iReceivedByteCount = Carrier_I2cReadNoWrData(pCore->hCarrier, pCore->zone, pCore->m_ucBridgeAddr, aucBuffer, iReadSize);

    // Check if data buffer is not empty
    if (ucData)
    {
        *ucData = aucBuffer[0];
    }
    else
        return eDevComInvalidDataBuffer05;

    // Check if we read the correct number of byte
    if (iReceivedByteCount == iReadSize )
    {
        return eDevComOk;
    }else{
        return eDevComFailed14;
    }
}


 /****************************************************************************
 *   NAME : _DevCom_ReadExpander
 ************************************************************************//**
 *
 *   Read up to 1 X 8-bit transfer from a 16 bits io expander device.
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] usAddr
 *      internal device register address where to start reading
 *
 *   @param [out] ucData
 *      data read
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_ReadExpander(Mo1000DevCom_stComCore * pCore, unsigned short usAddr, unsigned char * ucData)
{
    unsigned char aucBuffer[5];
    int receivedByteCount;
    const int read_size = 1;
    I2C_Options_t options;
    options.nbByteAddr = 1;
    
    usAddr &= 0xFF;

    //read back the data from I2C bridge
    receivedByteCount = Carrier_I2cRead(pCore->hCarrier, pCore->zone, pCore->m_ucExpanderAddr, usAddr, aucBuffer, read_size, options);

    // Check if data buffer is not empty
    if (ucData)
    {
        *ucData = aucBuffer[0];
    }
    else
        return eDevComInvalidDataBuffer03;

    // Check if we read the correct number of byte
    if (receivedByteCount == read_size )
    {
        return eDevComOk;
    }else{
        return eDevComFailed11;
    }
}


 /****************************************************************************
 *   NAME : _DevCom_ReadPll
 ************************************************************************//**
 *
 *   Read up to 1 X 28-bit transfer from a PLL device.
 *
 *   @param [in] pCore
 *      communication core specified at module opening
 *
 *   @param [in] ucAddr
 *      internal device register address where to start reading
 *
 *   @param [out] uData
 *      data read
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
static Result _DevCom_ReadPll(Mo1000DevCom_stComCore * pCore, unsigned char ucAddr, unsigned * uData)
{
    int iCount, iLoop;
    unsigned char aucBuffer[5];
    unsigned uNewData, uInitData;
    int iSentByteCount, iReceivedByteCount;
    const int iTransferSize = 5;
    I2C_Options_t options;
    options.nbByteAddr = 1;

    // Keep only 4 bits of address register merged with read command instruction
    uInitData = ((ucAddr & 0x0f) << 4) | (0x0e);

    // Reverse the bit ordering because the bridge is setup to tx MSB 1st (and setup for LSB req i2c command)
    uNewData = uInitData;         // Get lsb bit from uInitData
    {
        iCount = sizeof(uData) * 8 - 1;

        uInitData >>= 1;
        while (uInitData)
        {
            uNewData <<= 1;
            uNewData |= (uInitData & 1);
            uInitData >>= 1;
            iCount--;
        }
        uNewData <<= iCount;
    }

    //function id
    aucBuffer[0] = DEVCOM_SPI_CS_PLL;

    //SPI data (swap byte ordering to achieve LE for pll, because the bridge send only bytes in MSB and not words)
    //data1
    aucBuffer[4] = *((unsigned char *)(&uNewData));
    //data2
    aucBuffer[3] = *(((unsigned char *)(&uNewData)) + 1);
    //data3
    aucBuffer[2] = *(((unsigned char *)(&uNewData)) + 2);
    //data4
    aucBuffer[1] = *(((unsigned char *)(&uNewData)) + 3);

    // Sent the command 2 times, the 1st one is to setup the pll read command
    // and the second time is to read the data from the register read command done in 1st pass
    for (iLoop = 0; iLoop < 2; iLoop++)
    {
        // Write the i2c message to prepare the reading
        iSentByteCount = Carrier_I2cWrite(pCore->hCarrier, pCore->zone, pCore->m_ucBridgeAddr, aucBuffer[0], aucBuffer+1, iTransferSize-1, options);

        // If the read preparation was ok?
        if (iSentByteCount != (iTransferSize-1))
        {
            return eDevComFailed12;
        }
    }

    //read back the data from I2C bridge
    iReceivedByteCount = Carrier_I2cReadNoWrData(pCore->hCarrier, pCore->zone, pCore->m_ucBridgeAddr, aucBuffer, iTransferSize - 1);

    // Check if data buffer is not empty
    if (uData)
    {
        // Swap byte order because the bridge deal with bytes and not words, and we need LE
        //data1
        *((unsigned char *)(&uInitData)) = aucBuffer[3];
        //data2
        *(((unsigned char *)(&uInitData)) + 1) = aucBuffer[2];
        //data3
        *(((unsigned char *)(&uInitData)) + 2) = aucBuffer[1];
        //data4
        *(((unsigned char *)(&uInitData)) + 3) = aucBuffer[0];

        // Reverse the bit ordering because the bridge is setup to tx MSB 1st (and setup for LSB req i2c command)
        uNewData = uInitData;         // Get lsb bit from uInitData
        {
            iCount = sizeof(uData) * 8 - 1;

            uInitData >>= 1;
            while (uInitData)
            {
                uNewData <<= 1;
                uNewData |= (uInitData & 1);
                uInitData >>= 1;
                iCount--;
            }
            uNewData <<= iCount;
        }
        // Flush the 4 lsb which are the register command
        *uData = (uNewData >> 4);
    }
    else
        return eDevComInvalidDataBuffer04;

    // Check if we read the correct number of byte
    if (iReceivedByteCount == (iTransferSize - 1) )
    {
        return eDevComOk;
    }else{
        return eDevComFailed13;
    }
}

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


 /****************************************************************************
 *   NAME : Mo1000DevCom_Open
 ************************************************************************//**
 *
 *   Open the Mo1000DevCom module before use.
 *   This function must be called before using any other function of this module.
 *
 *   @param [out] pCore
 *      returns a communication core data structure
 *
 *   @param [in] uBaseAddr
 *      This is the fpga i2c instance base address. In the perseus module, the fpga is
 *      connected to the i2c devices chain of the fmc module.
 *
 *   @param [in] pRelease
 *      This is the fpga i2c bus control base address. In the perseus module, this
 *      control allows to grant the i2c bus control to a master or not before using.
 *
 *   @param [in] ucBridgeAddr
 *      7 bits I2C address of I2C/SPI bridge device
 *
 *   @param [in] ucSensorAddr
 *      7 bits I2C address of temperature sensor device
 *
 *   @param [in] ucExpanderAddr
 *      7 bits I2C address of 16 bits IO expander device (beta and later)
 *
 *   @param [in] ucEepromAddr
 *      7 bits I2C address of eeprom device
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_Open(Mo1000DevCom_stComCore * pCore, Carrier_handle_t hCarrier, unsigned int zone, unsigned char ucBridgeAddr,
                 unsigned char ucSensorAddr, unsigned char ucExpanderAddr, unsigned char ucEepromAddr)
{
    pCore->hCarrier = hCarrier;
    pCore->zone = zone;
    pCore->m_ucBridgeAddr = ucBridgeAddr;
    pCore->m_ucSensorAddr = ucSensorAddr;
    pCore->m_ucExpanderAddr = ucExpanderAddr;
    pCore->m_ucEepromAddr = ucEepromAddr;

    //SPI clock low when idle, data clocked in on leading edge, MSB transmitted 1st
    s_ucI2cBridgeInitVal = 0x0;

    return eDevComOk;
}


 /****************************************************************************
 *   NAME : Mo1000DevCom_Reset
 ************************************************************************//**
 *
 *   Initialize the Mo1000DevCom to a default non working condition.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_Reset(Mo1000DevCom_stComCore * pCore)
{
    Result result;

    // Configure the spi parameters of the bridge with speed 1843 Khz
    result = _DevCom_SetSpiSpeed(pCore, 0);

    // If we got no error?
    if (result == eDevComOk)
    {
        // Set the bridge in low power down mode
        result = _DevCom_WriteBridge(pCore, BRIDGE_IDLE_CMD, 0, 0);
    }

    return result;
}


 /****************************************************************************
 *   NAME : Mo1000DevCom_Init
 ************************************************************************//**
 *
 *   Initialize the Mo1000DevCom to a default working condition.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_Init(Mo1000DevCom_stComCore * pCore)
{
    // Configure the spi parameters of the bridge with speed 1843 Khz
    return _DevCom_SetSpiSpeed(pCore, 0);
}


 /****************************************************************************
 *   NAME : Mo1000DevCom_Close
 ************************************************************************//**
 *
 *   Close the Mo1000DevCom module when not needed anymore.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_Close(Mo1000DevCom_stComCore * pCore)
{
    return eDevComOk;
}


 /****************************************************************************
 *   NAME : Mo1000DevCom_WriteArray
 ************************************************************************//**
 *
 *   Write an array of data to a specified device. Data is MSB 1st with
 *   incrementing address.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] hCom
 *      pointer to the structure returns from Mo1000DevCom_Open function
 *
 *   @param [in] u32DeviceId
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
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_WriteArray(void * hCom, uint32_t u32DeviceId, uint16_t u16Addr, const void * pData, int32_t i32Size)
{
    Result Ret;
    int iCount;
    
    Mo1000DevCom_stComCore * pCore = (Mo1000DevCom_stComCore *)hCom;
    Mo1000DevCom_eDevices eDevice = (Mo1000DevCom_eDevices)u32DeviceId;

    // Which device we need to write by looping one item size at a time (vary according to device)
    switch(eDevice)
    {
        // Write to a DAC device
        case eMo1000DevComDeviceDac1:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_WriteDac(pCore, u16Addr+iCount, ((unsigned char*)pData)[iCount], DEVCOM_SPI_CS_DAC1);
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Write to a DAC device
        case eMo1000DevComDeviceDac2:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_WriteDac(pCore, u16Addr+iCount, ((unsigned char*)pData)[iCount], DEVCOM_SPI_CS_DAC2);
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Write to a PLL device
        case eMo1000DevComDevicePll:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_WritePll(pCore, u16Addr+iCount, ((unsigned*)pData)[iCount]);
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Write to a temperature sensor device
        case eMo1000DevComDeviceSensors:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_WriteSensors(pCore, u16Addr+iCount, ((unsigned short*)pData)[iCount]);
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Write to IO expander device
        case eMo1000DevComDevicePorts:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_WriteExpander(pCore, u16Addr+iCount, ((unsigned char*)pData)[iCount]);
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Write to an I2C/SPI bridge device
        case eMo1000DevComDeviceI2cBridge:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_WriteBridge(pCore, u16Addr+iCount, ((unsigned char*)pData)[iCount], 1);
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Write to an eeprom device
        default:
        case eMo1000DevComDeviceEeprom:
            return eDevComUnimplemented01;
    }

    // Release I2C bus ownership
    return eDevComOk;
}


 /****************************************************************************
 *   NAME : Mo1000DevCom_ReadArray
 ************************************************************************//**
 *
 *   Read an array of data from a specified device. Data is MSB 1st with
 *   incrementing address.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] hCom
 *      pointer to the structure returns from Mo1000DevCom_Open function
 *
 *   @param [in] u32DeviceId
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
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_ReadArray(void * hCom, uint32_t u32DeviceId, uint16_t u16Addr, void * pData, int32_t i32Size)
{
    Result Ret;
    int iCount;
    
    Mo1000DevCom_stComCore * pCore = (Mo1000DevCom_stComCore *)hCom;
    Mo1000DevCom_eDevices eDevice = (Mo1000DevCom_eDevices)u32DeviceId;

    // select which i2c device type
    switch(eDevice)
    {
        // DAC device type
        case eMo1000DevComDeviceDac1:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_ReadDac(pCore, u16Addr+iCount, &(((unsigned char*)pData)[iCount]), DEVCOM_SPI_CS_DAC1);
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // DAC device type
        case eMo1000DevComDeviceDac2:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_ReadDac(pCore, u16Addr+iCount, &(((unsigned char*)pData)[iCount]), DEVCOM_SPI_CS_DAC2);
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // PLL device type
        case eMo1000DevComDevicePll:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_ReadPll(pCore, u16Addr+iCount, &(((unsigned*)pData)[iCount]));
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Bridge device type
        case eMo1000DevComDeviceI2cBridge:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_ReadBridge(pCore, &(((unsigned char*)pData)[iCount]));
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Temperature sensor device type
        case eMo1000DevComDeviceSensors:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_ReadSensors(pCore, u16Addr+iCount, &(((short*)pData)[iCount]));
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // Read from io expander device
        case eMo1000DevComDevicePorts:
            for(iCount=0; iCount<i32Size; iCount++)
            {
                Ret = _DevCom_ReadExpander(pCore, u16Addr+iCount, &(((unsigned char*)pData)[iCount]));
                if(FAILURE(Ret)) {return Ret;}
            }
            break;

        // This device types are currently not supported
        default:
        case eMo1000DevComDeviceEeprom:
            return eDevComUnimplemented02;
    }

    // Release bus ownership
    return eDevComOk;
}


 /****************************************************************************
 *   NAME : Mo1000DevCom_Write
 ************************************************************************//**
 *
 *   Write 1 data item to a specified device (size of data item vary according
 *   to the device).
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @param [in] eDevice
 *      which logical device to write
 *
 *   @param [in] usAddr
 *      at which internal device address starting to write
 *
 *   @param [in] pData
 *      data buffer used for writing
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_Write(Mo1000DevCom_stComCore * pCore, Mo1000DevCom_eDevices eDevice, unsigned short usAddr, const void * pData)
{
    // Use the array write routine with an item size of 1
    return Mo1000DevCom_WriteArray(pCore, eDevice, usAddr, pData, 1);
}


 /****************************************************************************
 *   NAME : Mo1000DevCom_Read
 ************************************************************************//**
 *
 *   Read 1 data item from a specified device. Data item size vary according
 *   to the device.
 *   Mo1000DevCom_Open must be called before using this function.
 *
 *   @param [in] pCore
 *      structure returns from Mo1000DevCom_Open function
 *
 *   @param [in] eDevice
 *      which logical device to read
 *
 *   @param [in] usAddr
 *      at which internal device address starting to read
 *
 *   @param [out] pData
 *      data buffer used for reading
 *
 *   @return   Error code: eDevComOk if no problem is found, !eDevComOk if problem
 *             Use SUCCESS(ret) macro to detect if an error occured.
 *
 *   @ingroup MO1000DEVCOM
 *
 ***************************************************************************/
Result Mo1000DevCom_Read(Mo1000DevCom_stComCore * pCore, Mo1000DevCom_eDevices eDevice, unsigned short usAddr, void * pData)
{
    return Mo1000DevCom_ReadArray(pCore, eDevice, usAddr, pData, 1);
}

