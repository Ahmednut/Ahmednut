// Includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <xiic_l.h>
#include <xio.h>

#include "bas_error.h"
#include "perseus601x_i2c.h"
#include "perseus601x_def.h"
#include "carrier_service.h"

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


typedef struct I2C_S_com_i2c_release {
// i2c bus ownership register definition
#ifdef _BIG_ENDIAN 
    volatile unsigned reserved:30, MmcI2cReleaseAck:1, MmcI2cReleaseReq:1;
#else
    volatile unsigned MmcI2cReleaseReq:1, MmcI2cReleaseAck:1, reserved:30;
#endif                
} I2C_com_i2c_release;


// This loop count is used for the i2c busy bit check
#define _I2C_BUSY_TOUT 20000000

// This loop count is used for the i2c to request the bus ownership
#define _I2C_BUSGET_TOUT 20000000

static Result _Perseus601X_RequestI2cBus(I2C_com_i2c_release *i2c_release);
static Result _Perseus601X_ReleaseI2cBus(I2C_com_i2c_release *i2c_release);


Result Perseus601X_I2cWrite(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options)
{
    Result Ret;
    unsigned char statusReg;
    unsigned sentByteCount;
    unsigned char * dataBuffer;
    int index;
    unsigned int i;
    //unsigned char i2cOptions;
    
    // Verify the I2C zone
    if (I2cZone != Perseus601x_eI2cZoneCarrier && I2cZone != Perseus601x_eI2cZoneFmc)
    {
        mdebug("Perseus601X_I2cRead does not support the I2C zone : %d\n",CARRIER_INVALID_I2C_ZONE);
        return CARRIER_INVALID_I2C_ZONE;
    }
    
    // Do nothing with the I2cZone value
    // since there is no I2C switch on the Perseus601X

    // Get i2c bus ownership
    Ret = _Perseus601X_RequestI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR); 
    if (FAILURE(Ret))
    {
        mdebug("_Perseus601X_RequestI2cBus error code 0x%X\n",Ret);
        return Ret;
    }

    /*!!!! Enable if needed later.
      if(options.stopCondition & I2C_OPTION_NO_STOP) { 
      i2cOptions = XIIC_REPEATED_START;
      }
      else {
      i2cOptions = XIIC_STOP;
      }*/
    
    dataBuffer = (unsigned char *)malloc(options.nbByteAddr + size);
    if(dataBuffer == NULL) {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        mdebug("I2C_writedata error code -2 %c\n",32);
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
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        mdebug("I2C_writedata error code -3 %c\n",32);
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
        statusReg = XIo_In8(PERSEUS601X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
            break;
        }
        i--;
    }while(i != 0);

    // If it failed then return
    if(i == 0)
    {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        free(dataBuffer);
        mdebug("I2C_writedata error code -4 %c\n",32);
        return CARRIER_I2C_TIMEOUT;
    }

    // Write data at the specified address to the device.
    sentByteCount = XIic_Send(PERSEUS601X_I2C_BASE_ADDR, i2cDeviceAddr, dataBuffer, (options.nbByteAddr + size), XIIC_STOP);

    // Check the status register for the busy bus bit,
    // to wait for the write to complete. 
    i = _I2C_BUSY_TOUT;
    do {
        statusReg = XIo_In8(PERSEUS601X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
            break;
        }
        i--;
    }while(i != 0);

    // If it failed then return
    if(i == 0)
    {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        free(dataBuffer);
        mdebug("I2C_writedata error code -5 %c\n",32);
        return CARRIER_I2C_TIMEOUT;
    }

    // Return the number of bytes written to the device.
    _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
    free(dataBuffer);
    return((int)(sentByteCount - options.nbByteAddr));
}

Result Perseus601X_I2cRead(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options)
{
    Result Ret;
    unsigned receivedByteCount;
    unsigned char statusReg;
    unsigned int i;
    
    // Verify the I2C zone
    if (I2cZone != Perseus601x_eI2cZoneCarrier && I2cZone != Perseus601x_eI2cZoneFmc)
    {
        mdebug("Perseus601X_I2cRead does not support the I2C zone : %d\n",CARRIER_INVALID_I2C_ZONE);
        return CARRIER_INVALID_I2C_ZONE;
    }

    // Get i2c bus ownership
    Ret = _Perseus601X_RequestI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR); 
    if (FAILURE(Ret))
    {
        mdebug("_Perseus601X_RequestI2cBus error code 0x%X\n",Ret);
        return Ret;
    }
    
    // Do nothing with the I2cZone value
    // since there is no I2C switch on the Perseus601X

    /*!!!! Enable if needed later.
      if(pptions.stopCondition & I2C_OPTION_NO_STOP) { 
      i2cOptions = XIIC_REPEATED_START;
      }
      else {
      i2cOptions = XIIC_STOP;
      }*/

    if((options.nbByteAddr < 1) || (options.nbByteAddr > 2)) {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        mdebug("Perseus601X_I2cRead error code -2 %c\n",32);
        return CARRIER_INVALID_I2C_OPTION;
    }
    
    // Check the status register for the busy bus bit, 
    // to verify if a previous write to the device could be pending and it
    // will not ack until that write is complete.
    i = _I2C_BUSY_TOUT;
    do {
        statusReg = XIo_In8(PERSEUS601X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
            break;
        }
        i--;
    }while(i != 0);

    // If it failed then return
    if(i == 0)
    {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        mdebug("I2C_writedata error code -4 %c\n",32);
        return CARRIER_I2C_TIMEOUT;
    }

    // Set the address register to the specified address by writing
    // the address to the device.
    receivedByteCount = XIic_Send(PERSEUS601X_I2C_BASE_ADDR, i2cDeviceAddr, (unsigned char *)&addr, options.nbByteAddr, XIIC_STOP);
    
    if(receivedByteCount != options.nbByteAddr)
    {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        mdebug("Perseus601X_I2cRead error code -3 %c\n",32);
        return CARRIER_I2C_FAILED;
    }
    
    receivedByteCount = XIic_Recv(PERSEUS601X_I2C_BASE_ADDR, i2cDeviceAddr, data, size, XIIC_STOP);

    _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
     
    return((int)receivedByteCount);
    
}


Result Perseus601X_I2cReadNoWrData(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned char * data, unsigned int size)
{
    Result Ret;
    unsigned receivedByteCount=0;
    unsigned char statusReg;
    unsigned int i;
    
    // Verify the I2C zone
    if (I2cZone != Perseus601x_eI2cZoneCarrier && I2cZone != Perseus601x_eI2cZoneFmc)
    {
        mdebug("Perseus601X_I2cRead does not support the I2C zone : %d\n",CARRIER_INVALID_I2C_ZONE);
        return CARRIER_INVALID_I2C_ZONE;
    }

    // Get i2c bus ownership
    Ret = _Perseus601X_RequestI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR); 
    if (FAILURE(Ret))
    {
        mdebug("_Perseus601X_RequestI2cBus error code 0x%X\n",Ret);
        return Ret;
    }
    
    // Do nothing with the I2cZone value
    // since there is no I2C switch on the Perseus601X

    // Wait with max timeout
    i = _I2C_BUSY_TOUT;
    do {
        statusReg = XIo_In8(PERSEUS601X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        //printf("Fifo Empty : 0x%x \r\n",statusReg & XIIC_SR_RX_FIFO_EMPTY_MASK);
        if(!(statusReg & XIIC_SR_BUS_BUSY_MASK)) {
            break;
        }
        i--;
    }while(i != 0);

    // Did the timeout exausted?
    if (i == 0)
    {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        mdebug("I2C_readNoWrData error code -2 %c\n",32);
        return CARRIER_I2C_TIMEOUT;
    }
    
    Carrier_usleep(hCarrier, 1000);

    // Read the number of bytes at the specified address from the device.
    receivedByteCount = XIic_Recv(PERSEUS601X_I2C_BASE_ADDR, i2cDeviceAddr, data, size, XIIC_STOP);

    _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
    
    // Return the number of bytes read from the device.
    return((int)receivedByteCount);
}


Result Perseus601X_I2cCheckRaw(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char ucDevAddr, unsigned char * pucDetected)
{
    Result Ret;
    int iCount;
    unsigned char aucBuffer[5];
    int iSentByteCount, iStatusReg;
    const int iTransferSize = 1;
    unsigned char ucAddr=0;

    *pucDetected = 0;
    
    // Verify the I2C zone
    if (I2cZone != Perseus601x_eI2cZoneCarrier && I2cZone != Perseus601x_eI2cZoneFmc)
    {
        mdebug("Perseus601X_I2cRead does not support the I2C zone : %d\n",I2cZone);
        return CARRIER_INVALID_I2C_ZONE;
    }

    // Get i2c bus ownership
    Ret = _Perseus601X_RequestI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR); 
    if (FAILURE(Ret))
    {
        mdebug("_Perseus601X_RequestI2cBus error code 0x%X\n",Ret);
        return Ret;
    }
    
    // Do nothing with the I2cZone value
    // since there is no I2C switch on the Perseus601X

    aucBuffer[0] = ucAddr & 0xFF;

    // Check the status register for the busy bus bit, 
    // to verify if a previous write to the device could be pending and it
    // will not ack until that write is complete.
    iCount = _I2C_BUSY_TOUT;
    do {
        iStatusReg = XIo_In8(PERSEUS601X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(iStatusReg & XIIC_SR_BUS_BUSY_MASK))
        {
            break;
        }
        iCount--;
    }while(iCount != 0);

    // If it failed then return
    if( iCount == 0 )
    {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        mdebug("I2C_CheckRaw error code -2 %c\n",32);
        return CARRIER_I2C_TIMEOUT;
    }

    // Send the i2c message to prepare a fake reading
    iSentByteCount = XIic_Send(PERSEUS601X_I2C_BASE_ADDR, ucDevAddr, aucBuffer, iTransferSize, XIIC_STOP);

    // Wait with max timeout
    iCount = _I2C_BUSY_TOUT;
    do {
        iStatusReg = XIo_In8(PERSEUS601X_I2C_BASE_ADDR + XIIC_SR_REG_OFFSET);
        if(!(iStatusReg & XIIC_SR_BUS_BUSY_MASK))
        {
            break;
        }
        iCount--;
    }while(iCount != 0);

    // If it failed then return
    if( iCount == 0 )
    {
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        mdebug("I2C_CheckRaw error code -3 %c\n",32);
        return CARRIER_I2C_TIMEOUT;
    }

    // Did the preparation message was written correctly?
    if (iSentByteCount == iTransferSize)
        *pucDetected = 1;

    // Release bus ownership
    return _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
}


 /****************************************************************************
 *   NAME : _Perseus601X_RequestI2cBus
 ************************************************************************//**
 *
 *   Request for I2C bus mastering (ownership).
 *   If succesfull, master can continue with i2c transactions on the bus.
 *   _Perseus601X_ReleaseI2cBus must be called to release the bus when completed.  
 *
 *   @param [in] i2c_release
 *      i2c bus request control
 *
 *   @return   Error code: 0 if no problem is found, !0 if problem
 *   
 *   @ingroup I2C_DRV
 *
 ***************************************************************************/ 
static Result _Perseus601X_RequestI2cBus(I2C_com_i2c_release *i2c_release)
{
    volatile int timeout;

    // Ask the i2c bus ownership
    i2c_release->MmcI2cReleaseReq = 1;

    // Wait to get the bus
    for(timeout=_I2C_BUSGET_TOUT; timeout!=0; timeout--)
    {
        // Check if the request was acknowledge
        if (i2c_release->MmcI2cReleaseAck == 1)
        {
            break;
        }
    }
    
    // Did we get a timeout
    if (!timeout)
    {
        // Problem, we didn' t get the bus ownership in time
        // Remove our bus request
        _Perseus601X_ReleaseI2cBus((I2C_com_i2c_release *)PERSEUS601X_MMC_CTRL_BASE_ADDR);
        return CARRIER_I2C_BUS_TIMEOUT;
    }

    return CARRIER_OK;
}


 /****************************************************************************
 *   NAME : _Perseus601X_ReleaseI2cBus
 ************************************************************************//**
 *
 *   Release the I2C bus ownership that was get with _Perseus601X_RequestI2cBus.
 *   _Perseus601X_ReleaseI2cBus must be called to release the bus when completed with
 *   _Perseus601X_RequestI2cBus.  
 *
 *   @param [in] i2c_release
 *      i2c bus request control
 *
 *   @return   Error code: 0 if no problem is found, !0 if problem
 *   
 *   @ingroup I2C_DRV
 *
 ***************************************************************************/ 
static Result _Perseus601X_ReleaseI2cBus(I2C_com_i2c_release *i2c_release)
{
    volatile int timeout;

    // Release the i2c bus ownership
    i2c_release->MmcI2cReleaseReq = 0;

    // Wait for the bus to be released
    for(timeout=_I2C_BUSGET_TOUT; timeout!=0; timeout--)
    {
        // Check if the request was acknowledge
        if (i2c_release->MmcI2cReleaseAck == 0)
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


