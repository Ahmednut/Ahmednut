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
 * @file       common_utils.c
 * @brief      Utility functions common to all carriers
 *
 * $Date: 2015/11/13 19:22:55 $
 * $Revision: 1.4 $
 *
 * (c) 2013, Nutaq Inc. All rights reserved.              
 *
 * @ingroup CCE
 *
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "bas_types.h"
#include "bas_error.h"
#include "bas_debug.h"
#include "eapi_serialize.h"

#include "cce.h"
#include "eapi.h"
#include "carrier_service.h"
#include "carrier_service_def.h"

extern Carrier_handle_t hCarrier;
extern int bInvalidCarrier;

Result Carrier_GetType_recv(connection_state * state)
{
	Result res = OK;
	Carrier_eType_t type;
	
	debug("Carrier_GetType_recv\r\n", 0);

    if(bInvalidCarrier == 1)
    {
        type = Carrier_eUnknown;
    }
    else
    {
        res = Carrier_GetType(hCarrier, &type);
    }
	
	debug("Type %d\r\n", type);
	uint_send(state, (uint32_t)type);
    
    
	return res;	
}

Result Carrier_GetModuleBaseAddr_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eModule_t module;
    unsigned int index;
    unsigned int addr = 0;
    
    module = (Carrier_eModule_t)uint_recv(state);
    index = uint_recv(state);
    
	debug("Carrier_GetModuleBaseAddr\r\n", 0);
	debug("module: %d\r\n", module);
	debug("index:  %d\r\n", index);
    
    res = Carrier_GetModuleBaseAddr(hCarrier, module, index, &addr);
    
    uint_send(state, addr);
    
	debug("addr:   %d\r\n", addr);
    
	return res;
}


Result Carrier_LedSetValue_recv(connection_state * state)
{
	Result res = OK;
    unsigned int index;
    unsigned int value;
    
    index = uint_recv(state);
    value = uint_recv(state);
    
	debug("Carrier_LedSetValue\r\n", 0);
	debug("index:  %d\r\n", index);
	debug("value:  %d\r\n", value);
    
    res = Carrier_LedSetValue(hCarrier, index, value);
    
	return res;
}

Result Carrier_LedGetValue_recv(connection_state * state)
{
	Result res = OK;
    unsigned int index;
    unsigned int value = 0;
    
    index = uint_recv(state);
    
	debug("Carrier_LedGetValue\r\n", 0);
	debug("index:  %d\r\n", index);
    
    res = Carrier_LedGetValue(hCarrier, index, &value);
    
    uint_send(state, value);
    
	debug("value:  %d\r\n", value);
    
	return res;
}


Result Carrier_TClkSetState_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eTClk_t tclk;
    Carrier_eTClkDir_t dir;
    Carrier_eClkState_t clkstate;
    
    tclk = (Carrier_eTClk_t)uint_recv(state);
    dir = (Carrier_eTClkDir_t)uint_recv(state);
    clkstate = (Carrier_eClkState_t)uint_recv(state);
    
	debug("Carrier_TClkSetState\r\n", 0);
	debug("tclk:  %d\r\n", tclk);
	debug("dir:   %d\r\n", dir);
	debug("clkstate: %d\r\n", clkstate);
    
    res = Carrier_TClkSetState(hCarrier, tclk, dir, clkstate);
    
	return res;
}

Result Carrier_TClkGetState_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eTClk_t tclk;
    Carrier_eTClkDir_t dir;
    Carrier_eClkState_t clkstate;
    
    tclk = (Carrier_eTClk_t)uint_recv(state);
    dir = (Carrier_eTClkDir_t)uint_recv(state);
    
	debug("Carrier_TClkGetState\r\n", 0);
	debug("tclk:  %d\r\n", tclk);
	debug("dir:   %d\r\n", dir);
    
    res = Carrier_TClkGetState(hCarrier, tclk, dir, &clkstate);
    
    uint_send(state, (uint32_t)clkstate);
    
	debug("clkstate: %d\r\n", clkstate);
    
	return res;
}

Result Carrier_TClkSetTxSource_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eTClk_t tclk;
    Carrier_eTClkSrc_t source;
    
    tclk = (Carrier_eTClk_t)uint_recv(state);
    source = (Carrier_eTClkSrc_t)uint_recv(state);
    
	debug("Carrier_TClkSetTxSource\r\n", 0);
	debug("tclk:   %d\r\n", tclk);
	debug("source: %d\r\n", source);
    
    res = Carrier_TClkSetTxSource(hCarrier, tclk, source);
    
	return res;
}

Result Carrier_TClkGetTxSource_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eTClk_t tclk;
    Carrier_eTClkSrc_t source;
    
    tclk = (Carrier_eTClk_t)uint_recv(state);
    
	debug("Carrier_TClkGetTxSource\r\n", 0);
	debug("tclk:   %d\r\n", tclk);
    
    res = Carrier_TClkGetTxSource(hCarrier, tclk, &source);
    
    uint_send(state, (uint32_t)source);
    
	debug("source: %d\r\n", source);
    
	return res;
}

Result Carrier_FmcBidirClkSetConnection_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eFmcBidirClk_t clk;
    Carrier_eFmcBidirClkConnection_t connection;
    
    clk = (Carrier_eFmcBidirClk_t)uint_recv(state);
    connection = (Carrier_eFmcBidirClkConnection_t)uint_recv(state);
    
	debug("Carrier_FmcBidirClkSetConnection\r\n", 0);
	debug("clk:         %d\r\n", clk);
	debug("connection: %d\r\n", connection);
    
    res = Carrier_FmcBidirClkSetConnection(hCarrier, clk, connection);
    
	return res;
}

Result Carrier_FmcBidirClkGetConnection_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eFmcBidirClk_t clk;
    Carrier_eFmcBidirClkConnection_t connection;
    
    clk = (Carrier_eFmcBidirClk_t)uint_recv(state);
    
	debug("Carrier_FmcBidirClkGetConnection\r\n", 0);
	debug("clk:         %d\r\n", clk);
    
    res = Carrier_FmcBidirClkGetConnection(hCarrier, clk, &connection);
    
    uint_send(state, (uint32_t)connection);
    
	debug("connection: %d\r\n", connection);
    
	return res;
}

Result Carrier_SetOscState_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eOscillator_t osc;
    Carrier_eClkState_t clkstate;
    
    osc = (Carrier_eOscillator_t)uint_recv(state);
    clkstate = (Carrier_eClkState_t)uint_recv(state);
    
	debug("Carrier_SetOscState\r\n", 0);
	debug("osc:   %d\r\n", osc);
	debug("clkstate: %d\r\n", clkstate);
    
    res = Carrier_SetOscState(hCarrier, osc, clkstate);
    
	return res;
}

Result Carrier_GetOscState_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eOscillator_t osc;
    Carrier_eClkState_t clkstate;
    
    osc = (Carrier_eOscillator_t)uint_recv(state);
    
	debug("Carrier_GetOscState\r\n", 0);
	debug("osc:   %d\r\n", osc);
    
    res = Carrier_GetOscState(hCarrier, osc, &clkstate);
    
    uint_send(state, (uint32_t)clkstate);
    
	debug("clkstate: %d\r\n", clkstate);
    
	return res;
}

Result Carrier_FClkASetConnection_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eFClkAConnection_t connection;
    
    connection = (Carrier_eFClkAConnection_t)uint_recv(state);
    
	debug("Carrier_FClkASetConnection\r\n", 0);
	debug("connection:   %d\r\n", connection);
    
    res = Carrier_FClkASetConnection(hCarrier, connection);
    
	return res;
}

Result Carrier_FClkAGetConnection_recv(connection_state * state)
{
	Result res = OK;
    Carrier_eFClkAConnection_t connection;
    
	debug("Carrier_FClkAGetConnection\r\n", 0);
    
    res = Carrier_FClkAGetConnection(hCarrier, &connection);
    
    uint_send(state, (uint32_t)connection);
    
	debug("connection:   %d\r\n", connection);
    
	return res;
}


Result Carrier_I2cRead_recv(connection_state * state)
{
	Result res = OK;
    
    unsigned int I2cZone;
    unsigned char i2cDeviceAddr;
    unsigned short addr;
    unsigned char * data;
    unsigned int size;
    I2C_Options_t options;
    
    I2cZone = uint_recv(state);
    i2cDeviceAddr = (unsigned char)uint_recv(state);
    addr = (unsigned short)uint_recv(state);
    options.nbByteAddr = uint_recv(state);

    //receive size of bitstream
    size = uint_recv(state);
    debug("size = %i \n", size);
    
    data = (unsigned char *)malloc(size);
    if(!data)
    {
        uint_send(state, 0); // Send data buffer size 0
        return CCEMEMORY;
    }
    
	debug("Carrier_I2cRead\r\n", 0);
	debug("I2cZone:         %d\r\n", I2cZone);
	debug("i2cDeviceAddr:   %d\r\n", i2cDeviceAddr);
	debug("addr:            %d\r\n", addr);
	debug("size:            %d\r\n", size);
	debug("nbByteAddr:      %d\r\n", options.nbByteAddr);
    
    res = Carrier_I2cRead(hCarrier, I2cZone, i2cDeviceAddr, addr, data, size, options);
    if(SUCCESS(res))
    {
        size = res;
        buf_send(state, (char *)data, size);
    }
    else
    {
        uint_send(state, 0); // Send data buffer size 0
    }
    
    free(data);
    
	return res;
}

Result Carrier_I2cWrite_recv(connection_state * state)
{
	Result res = OK;
    int transferred;
    int i;
    
    unsigned int I2cZone;
    unsigned char i2cDeviceAddr;
    unsigned short addr;
    unsigned char * data;
    unsigned int size;
    I2C_Options_t options;
    
    I2cZone = uint_recv(state);
    i2cDeviceAddr = (unsigned char)uint_recv(state);
    addr = (unsigned short)uint_recv(state);
    options.nbByteAddr = uint_recv(state);

    //receive size of bitstream
    size = uint_recv(state);
    debug("size = %i \n", size);
    
    data = (unsigned char *)malloc(size);
    if(!data)
    {
        return CCEMEMORY;
    }
    
    transferred = 0;
    while (transferred<size)
    {
        i = recv(state->stream, &data[transferred], size-transferred, 0);
        if(i<0) return EAPIDISCONNECT;
        transferred+=i;
    }
    
	debug("Carrier_I2cWrite\r\n", 0);
	debug("I2cZone:         %d\r\n", I2cZone);
	debug("i2cDeviceAddr:   %d\r\n", i2cDeviceAddr);
	debug("addr:            %d\r\n", addr);
	debug("size:            %d\r\n", size);
	debug("nbByteAddr:      %d\r\n", options.nbByteAddr);
    
    res = Carrier_I2cWrite(hCarrier, I2cZone, i2cDeviceAddr, addr, data, size, options);
    
    free(data);
    
	return res;
}

Result Carrier_I2cReadNoWrData_recv(connection_state * state)
{
	Result res = OK;
    
    unsigned int I2cZone;
    unsigned char i2cDeviceAddr;
    unsigned char * data;
    unsigned int size;
    
    I2cZone = uint_recv(state);
    i2cDeviceAddr = (unsigned char)uint_recv(state);

    //receive size of bitstream
    size = uint_recv(state);
    debug("size = %i \n", size);
    
    data = (unsigned char *)malloc(size);
    if(!data)
    {
        uint_send(state, 0); // Send data buffer size 0
        return CCEMEMORY;
    }
    
	debug("Carrier_I2cRead\r\n", 0);
	debug("I2cZone:         %d\r\n", I2cZone);
	debug("i2cDeviceAddr:   %d\r\n", i2cDeviceAddr);
	debug("size:            %d\r\n", size);
    
    res = Carrier_I2cReadNoWrData(hCarrier, I2cZone, i2cDeviceAddr, data, size);
    if(SUCCESS(res))
    {
        size = res;
        buf_send(state, (char *)data, size);
    }
    else
    {
        uint_send(state, 0); // Send data buffer size 0
    }
    
    free(data);
    
	return res;
}


