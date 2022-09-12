// Includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "xiic.h"
#include "xio.h"

#include "bas_error.h"
#include "perseus611x_i2c.h"
#include "perseus611x_def.h"
#include "perseus611x_regs.h"

#include "carrier_service.h"
#include "carrier_internal_def.h"


#ifdef DEBUG
// Uncomment following line for debug
#define I2CDRV_DEBUG 1
#endif

//debug or no debug
#ifdef I2CDRV_DEBUG
#define mdebug(f, ...) printf(f, __VA_ARGS__)
#else
#define mdebug(f, ...)
#endif


// This loop count is used for the i2c busy bit check
#define _I2C_BUSY_TOUT 20000000

// This loop count is used for the i2c to request the bus ownership
#define _I2C_BUSGET_TOUT 20000000

static Result _Perseus611X_RequestI2cBus(Carrier_handle_t hCarrier);
static Result _Perseus611X_ReleaseI2cBus(Carrier_handle_t hCarrier);

static Result _Perseus611X_SetI2cZone(Carrier_handle_t hCarrier, unsigned int I2cZone);


Result Perseus611X_I2cWrite(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options)
{
    Result Ret;
    unsigned char statusReg;
    unsigned sentByteCount;
    unsigned char * dataBuffer;
    int index;
    unsigned int i;
    
    Ret = _Perseus611X_SetI2cZone(hCarrier, I2cZone);
    if (FAILURE(Ret))
    {
        mdebug("_Perseus611X_SetI2cZone error code 0x%X\n",Ret);
        return Ret;
    }

    // Get i2c bus ownership
    Ret = _Perseus611X_RequestI2cBus(hCarrier);
    if (FAILURE(Ret))
    {
        mdebug("_Perseus611X_RequestI2cBus error code 0x%X\n",Ret);
        return Ret;
    }
    
    dataBuffer = (unsigned char *)malloc(options.nbByteAddr + size);
    if(dataBuffer == NULL) {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        return CARRIER_ALLOC_FAILED;
    }

    // Put the address in databuffer first.
    if(options.nbByteAddr == 2) {
        dataBuffer[0] = (unsigned char)(addr >> 8);
        dataBuffer[1] = (unsigned char)(addr);
    }
    else if(options.nbByteAddr == 1) {
        dataBuffer[0] = (unsigned char)(addr);
    }
    else {
        free(dataBuffer);
        _Perseus611X_ReleaseI2cBus(hCarrier);
        return CARRIER_INVALID_I2C_OPTION;
    }
	
    // Next, put the data in the databuffer following the address.
    for (index = 0; index < size; index++) {
        dataBuffer[options.nbByteAddr + index] = data[index];
    }

    // Check the status register for the busy bus bit, 
    // to verify if a previous write to the device could be pending and it
    // will not ack until that write is complete.
    i = _I2C_BUSY_TOUT;
    do {
        statusReg = XIo_In8(PERSEUS611X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
            break;
        }
        i--;
    }while(i != 0);

    // If it failed then return
    if(i == 0)
    {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        free(dataBuffer);
        return CARRIER_I2C_TIMEOUT;
    }

    // Write data at the specified address to the device.
    sentByteCount = XIic_Send(PERSEUS611X_I2C_BASE_ADDR, i2cDeviceAddr, dataBuffer, (options.nbByteAddr + size), XIIC_STOP);

    // Check the status register for the busy bus bit,
    // to wait for the write to complete. 
    i = _I2C_BUSY_TOUT;
    do {
        statusReg = XIo_In8(PERSEUS611X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
            break;
        }
        i--;
    }while(i != 0);

    // If it failed then return
    if(i == 0)
    {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        free(dataBuffer);
        return CARRIER_I2C_TIMEOUT;
    }

    // Return the number of bytes written to the device.
    _Perseus611X_ReleaseI2cBus(hCarrier);
    free(dataBuffer);
    return((int)(sentByteCount - options.nbByteAddr));
}

Result Perseus611X_I2cRead(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options)
{
    Result Ret;
    unsigned receivedByteCount;
    unsigned char statusReg;
    unsigned int i;
    
    Ret = _Perseus611X_SetI2cZone(hCarrier, I2cZone);
    if (FAILURE(Ret))
    {
        mdebug("_Perseus611X_SetI2cZone error code 0x%X\n",Ret);
        return Ret;
    }

    // Get i2c bus ownership
    Ret = _Perseus611X_RequestI2cBus(hCarrier);
    if (FAILURE(Ret))
    {
        mdebug("_Perseus611X_RequestI2cBus error code 0x%X\n",Ret);
        return Ret;
    }

    if((options.nbByteAddr < 1) || (options.nbByteAddr > 2)) {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        return CARRIER_INVALID_I2C_OPTION;
    }

    // Check the status register for the busy bus bit, 
    // to verify if a previous write to the device could be pending and it
    // will not ack until that write is complete.
    i = _I2C_BUSY_TOUT;
    do {
        statusReg = XIo_In8(PERSEUS611X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
            break;
        }
        i--;
    }while(i != 0);

    // If it failed then return
    if(i == 0)
    {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        mdebug("I2C_writedata error code -4 %c\n",32);
        return CARRIER_I2C_TIMEOUT;
    }
    
    // Set the address register to the specified address by writing
    // the address to the device.
    receivedByteCount = XIic_Send(PERSEUS611X_I2C_BASE_ADDR, i2cDeviceAddr, (unsigned char *)&addr, options.nbByteAddr, XIIC_STOP);
    
    if(receivedByteCount != options.nbByteAddr)
    {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        mdebug("Perseus601X_I2cRead error code -3 %c\n",32);
        return CARRIER_I2C_FAILED;
    }
    
    receivedByteCount = XIic_Recv(PERSEUS611X_I2C_BASE_ADDR, i2cDeviceAddr, data, size, XIIC_STOP);

    _Perseus611X_ReleaseI2cBus(hCarrier);
     
    return((int)receivedByteCount);
    
}


Result Perseus611X_I2cReadNoWrData(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned char * data, unsigned int size)
{
    Result Ret;
    unsigned receivedByteCount=0;
    unsigned char statusReg;
    unsigned int i;
    
    Ret = _Perseus611X_SetI2cZone(hCarrier, I2cZone);
    if (FAILURE(Ret))
    {
        mdebug("_Perseus611X_SetI2cZone error code 0x%X\n",Ret);
        return Ret;
    }

    // Get i2c bus ownership
    Ret = _Perseus611X_RequestI2cBus(hCarrier);
    if (FAILURE(Ret))
    {
        mdebug("_Perseus611X_RequestI2cBus error code 0x%X\n",Ret);
        return Ret;
    }

    // Wait with max timeout
    i = _I2C_BUSY_TOUT;
    do {
        statusReg = XIo_In8(PERSEUS611X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        //printf("Fifo Empty : 0x%x \r\n",statusReg & XIIC_SR_RX_FIFO_EMPTY_MASK);
        if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
            break;
        }
        i--;
    }while(i != 0);

    // Did the timeout exausted?
    if (i == 0)
    {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        return CARRIER_I2C_TIMEOUT;
    }
    
    Carrier_usleep(hCarrier, 1000);

    // Read the number of bytes at the specified address from the device.
    receivedByteCount = XIic_Recv(PERSEUS611X_I2C_BASE_ADDR, i2cDeviceAddr, data, size, XIIC_STOP);

    _Perseus611X_ReleaseI2cBus(hCarrier);
    
    // Return the number of bytes read from the device.
    return((int)receivedByteCount);
}


Result Perseus611X_I2cCheckRaw(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char ucDevAddr, unsigned char * pucDetected)
{
    Result Ret;
    int iCount;
    unsigned char aucBuffer[5];
    int iSentByteCount, iStatusReg;
    const int iTransferSize = 1;
    unsigned char ucAddr=0;

    *pucDetected = 0;
    
    Ret = _Perseus611X_SetI2cZone(hCarrier, I2cZone);
    if (FAILURE(Ret))
    {
        mdebug("_Perseus611X_SetI2cZone error code 0x%X\n",Ret);
        return Ret;
    }

    // Get i2c bus ownership
    Ret = _Perseus611X_RequestI2cBus(hCarrier);
    if (FAILURE(Ret))
    {
        mdebug("_Perseus611X_RequestI2cBus error code 0x%X\n",Ret);
        return Ret;
    }
    
    // Do nothing with the I2cZone value
    // since there is no I2C switch on the Perseus611X

    aucBuffer[0] = ucAddr & 0xFF;

    // Check the status register for the busy bus bit, 
    // to verify if a previous write to the device could be pending and it
    // will not ack until that write is complete.
    iCount = _I2C_BUSY_TOUT;
    do {
        iStatusReg = XIo_In8(PERSEUS611X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(iStatusReg & XIIC_SR_BUS_BUSY_MASK))
        {
            break;
        }
        iCount--;
    }while(iCount != 0);

    // If it failed then return
    if( iCount == 0 )
    {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        return CARRIER_I2C_TIMEOUT;
    }

    // Send the i2c message to prepare a fake reading
    iSentByteCount = XIic_Send(PERSEUS611X_I2C_BASE_ADDR, ucDevAddr, aucBuffer, iTransferSize, XIIC_STOP);

    // Wait with max timeout
    iCount = _I2C_BUSY_TOUT;
    do {
        iStatusReg = XIo_In8(PERSEUS611X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(iStatusReg & XIIC_SR_BUS_BUSY_MASK))
        {
            break;
        }
        iCount--;
    }while(iCount != 0);

    // If it failed then return
    if( iCount == 0 )
    {
        _Perseus611X_ReleaseI2cBus(hCarrier);
        return CARRIER_I2C_TIMEOUT;
    }

    // Did the preparation message was written correctly?
    if (iSentByteCount == iTransferSize)
        *pucDetected = 1;

    // Release bus ownership
    return _Perseus611X_ReleaseI2cBus(hCarrier);
}

 /****************************************************************************
 *   NAME : I2C_wait_for_i2c_bus_release
 ************************************************************************//**
 *
 *   Request for I2C bus mastering (ownership).
 *   If succesfull, master can continue with i2c transactions on the bus.
 *   I2C_giveback_i2c_bus must be called to release the bus when completed.  
 *
 *   @param [in] i2c_release
 *      i2c bus request control
 *
 *   @return   Error code: 0 if no problem is found, !0 if problem
 *   
 *   @ingroup I2C_DRV
 *
 ***************************************************************************/ 
static Result _Perseus611X_RequestI2cBus(Carrier_handle_t hCarrier)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    volatile int timeout;
    
    // Ask the i2c bus ownership
    pCore->stI2cCtrl.stBit.m_uI2cRequest = 1;

    // Wait to get the bus
    for(timeout=_I2C_BUSGET_TOUT; timeout!=0; timeout--)
    {
        // Check if the request was acknowledge
        if (pCore->stI2cCtrl.stBit.m_uI2cGranted == 1)
        {
            break;
        }
    }
    
    // Did we get a timeout
    if (!timeout)
    {
        // Problem, we didn' t get the bus ownership in time
        // Remove our bus request
        _Perseus611X_ReleaseI2cBus(hCarrier);
        return CARRIER_I2C_BUS_TIMEOUT;
    }

    return CARRIER_OK;
}


 /****************************************************************************
 *   NAME : I2C_giveback_i2c_bus
 ************************************************************************//**
 *
 *   Release the I2C bus ownership that was get with I2C_wait_for_i2c_bus_release.
 *   I2C_giveback_i2c_bus must be called to release the bus when completed with
 *   I2C_wait_for_i2c_bus_release.  
 *
 *   @param [in] i2c_release
 *      i2c bus request control
 *
 *   @return   Error code: 0 if no problem is found, !0 if problem
 *   
 *   @ingroup I2C_DRV
 *
 ***************************************************************************/ 
static Result _Perseus611X_ReleaseI2cBus(Carrier_handle_t hCarrier)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    volatile int timeout;

    // Release the i2c bus ownership
    pCore->stI2cCtrl.stBit.m_uI2cRequest = 0;

    // Wait for the bus to be released
    for(timeout=_I2C_BUSGET_TOUT; timeout!=0; timeout--)
    {
        // Check if the request was acknowledge
        if (pCore->stI2cCtrl.stBit.m_uI2cGranted == 0)
        {
            break;
        }
    }
    
    // Did we get a timeout
    if (!timeout)
    {
        // Problem, we didn' t get the bus released in time
        return CARRIER_I2C_BUS_TIMEOUT;
    }

    return CARRIER_OK;
}


static Result _Perseus611X_SetI2cZone(Carrier_handle_t hCarrier, unsigned int I2cZone)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    
    if(I2cZone == Perseus611x_eI2cZoneCarrier) pCore->stI2cCtrl.stBit.m_uI2cBusSel = 0;
    else if(I2cZone == Perseus611x_eI2cZoneFmc1) pCore->stI2cCtrl.stBit.m_uI2cBusSel = 1;
    else if(I2cZone == Perseus611x_eI2cZoneFmc2) pCore->stI2cCtrl.stBit.m_uI2cBusSel = 2;
    else if(I2cZone == Perseus611x_eI2cZoneRtm) pCore->stI2cCtrl.stBit.m_uI2cBusSel = 3;
    else return CARRIER_INVALID_I2C_ZONE;
    
    return CARRIER_OK;
}
