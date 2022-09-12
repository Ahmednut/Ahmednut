/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "bas_types.h"
#include "cce.h"

#include "eapi_serialize.h"
#include "carrier_service_def.h"

/****************************************************************************
 *                            Private constants                             *
 ****************************************************************************/


/****************************************************************************
 *                       Private function prototypes                        *
 ****************************************************************************/

 
/****************************************************************************
 *                             Public functions                             *
 ****************************************************************************/
Result Carrier_Presence_send(connection_state * state)
{
    cmd_send(state, CARRIER_PRESENCE);	// send presence command
    return result_recv(state);
}

Result Carrier_Reboot_send(connection_state * state)
{
    cmd_send(state, CARRIER_REBOOT);	// send reboot command
    return result_recv(state);
}

Result Carrier_GetType_send(connection_state * state, Carrier_eType_t * pType)
{
    cmd_send(state, CARRIER_GETTYPE);
    
    *pType = (Carrier_eType_t)uint_recv(state);

    return result_recv(state);
}

Result Carrier_GetModuleBaseAddr_send(connection_state * state, Carrier_eModule_t module, unsigned int index, unsigned int * addr)
{
    cmd_send(state, CARRIER_GETMODULEBASEADDR);
    
    uint_send(state, (uint32_t)module);
    uint_send(state, index);
    
    *addr = uint_recv(state);

    return result_recv(state);
}

Result Carrier_LedSetValue_send(connection_state * state, unsigned int index, unsigned int value)
{
    cmd_send(state, CARRIER_LEDSETVALUE);
    
    uint_send(state, index);
    uint_send(state, value);

    return result_recv(state);
}

Result Carrier_LedGetValue_send(connection_state * state, unsigned int index, unsigned int * value)
{
    cmd_send(state, CARRIER_LEDGETVALUE);
    
    uint_send(state, index);
    
    *value = uint_recv(state);

    return result_recv(state);
}

Result Carrier_TClkSetState_send(connection_state * state, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t clkstate)
{
    cmd_send(state, CARRIER_TCLKSETSTATE);
    
    uint_send(state, (uint32_t)tclk);
    uint_send(state, (uint32_t)dir);
    uint_send(state, (uint32_t)clkstate);

    return result_recv(state);
}

Result Carrier_TClkGetState_send(connection_state * state, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t * clkstate)
{
    cmd_send(state, CARRIER_TCLKGETSTATE);
    
    uint_send(state, (uint32_t)tclk);
    uint_send(state, (uint32_t)dir);
    
    *clkstate = (Carrier_eClkState_t)uint_recv(state);

    return result_recv(state);
}

Result Carrier_TClkSetTxSource_send(connection_state * state, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t source)
{
    cmd_send(state, CARRIER_TCLKSETTXSOURCE);
    
    uint_send(state, (uint32_t)tclk);
    uint_send(state, (uint32_t)source);

    return result_recv(state);
}

Result Carrier_TClkGetTxSource_send(connection_state * state, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t * source)
{
    cmd_send(state, CARRIER_TCLKGETTXSOURCE);
    
    uint_send(state, (uint32_t)tclk);
    
    *source = (Carrier_eTClkSrc_t)uint_recv(state);

    return result_recv(state);
}

Result Carrier_FmcBidirClkSetConnection_send(connection_state * state, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t connection)
{
    cmd_send(state, CARRIER_FMCBIDIRCLKSETCONNECTION);
    
    uint_send(state, (uint32_t)clk);
    uint_send(state, (uint32_t)connection);

    return result_recv(state);
}

Result Carrier_FmcBidirClkGetConnection_send(connection_state * state, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t * connection)
{
    cmd_send(state, CARRIER_FMCBIDIRCLKGETCONNECTION);
    
    uint_send(state, (uint32_t)clk);
    
    *connection = (Carrier_eFmcBidirClkConnection_t)uint_recv(state);

    return result_recv(state);
}

Result Carrier_SetOscState_send(connection_state * state, Carrier_eOscillator_t osc, Carrier_eClkState_t clkstate)
{
    cmd_send(state, CARRIER_SETOSCSTATE);
    
    uint_send(state, (uint32_t)osc);
    uint_send(state, (uint32_t)clkstate);

    return result_recv(state);
}

Result Carrier_GetOscState_send(connection_state * state, Carrier_eOscillator_t osc, Carrier_eClkState_t * clkstate)
{
    cmd_send(state, CARRIER_GETOSCSTATE);
    
    uint_send(state, (uint32_t)osc);
    
    *clkstate = (Carrier_eClkState_t)uint_recv(state);

    return result_recv(state);
}

Result Carrier_FClkASetConnection_send(connection_state * state, Carrier_eFClkAConnection_t connection)
{
    cmd_send(state, CARRIER_FCLKASETCONNECTION);
    
    uint_send(state, (uint32_t)connection);

    return result_recv(state);
}

Result Carrier_FClkAGetConnection_send(connection_state * state, Carrier_eFClkAConnection_t * connection)
{
    cmd_send(state, CARRIER_FCLKAGETCONNECTION);
    
    *connection = (Carrier_eFClkAConnection_t)uint_recv(state);

    return result_recv(state);
}

Result Carrier_I2cRead_send(connection_state * state, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options)
{
	Result res;
    unsigned int uiReceivedSize;
    
    cmd_send(state, CARRIER_I2CREAD);
    
    uint_send(state, (uint32_t)I2cZone);
    uint_send(state, (uint32_t)i2cDeviceAddr);
    uint_send(state, (uint32_t)addr);
    uint_send(state, (uint32_t)options.nbByteAddr);
    uint_send(state, size);
    
    buf_recv_with_size(state, data, &uiReceivedSize);
    
    res = result_recv(state);
    if(FAILURE(res)) return res;

    return uiReceivedSize;
}

Result Carrier_I2cWrite_send(connection_state * state, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned short addr, unsigned char * data, unsigned int size, I2C_Options_t options)
{    
    cmd_send(state, CARRIER_I2CWRITE);
    
    uint_send(state, (uint32_t)I2cZone);
    uint_send(state, (uint32_t)i2cDeviceAddr);
    uint_send(state, (uint32_t)addr);
    uint_send(state, (uint32_t)options.nbByteAddr);
    
    buf_send(state, data, size);

    return result_recv(state);
}

Result Carrier_I2cReadNoWrData_send(connection_state * state, unsigned int I2cZone, unsigned char i2cDeviceAddr, unsigned char * data, unsigned int size)
{
	Result res;
    unsigned int uiReceivedSize;
    
    cmd_send(state, CARRIER_I2CREADNOWRDATA);
    
    uint_send(state, (uint32_t)I2cZone);
    uint_send(state, (uint32_t)i2cDeviceAddr);
    uint_send(state, size);
    
    buf_recv_with_size(state, data, &uiReceivedSize);
    
    res = result_recv(state);
    if(FAILURE(res)) return res;

    return uiReceivedSize;
}
