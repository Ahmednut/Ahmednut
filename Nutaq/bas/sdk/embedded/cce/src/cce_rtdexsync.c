#include "cce.h"
#include "eapi_serialize.h"
#include "bas_error.h"
#include "bas_debug.h"

#include "cce_rtdexsync.h"
#include "rtdexsync_def.h"

#include "carrier_service.h"
#include "carrier_service_def.h"

extern Carrier_handle_t hCarrier;

/****************************************************************************
 *                                  Macros                                  * 
 ****************************************************************************/

/**
 * Macros for manipulatind fields
 */
#define SET_FIELD(FIELD,VAL)    (((VAL) & FIELD##_MASK) << FIELD##_SHIFT) 
#define GET_FIELD(FIELD,VAL)    (((VAL) >> FIELD##_SHIFT) & FIELD##_MASK) 


/****************************************************************************
 *                             Public functions                             *
 ****************************************************************************/
void RTDExSync_WriteChReg( uint8_t u8Channel,
                           RTDExSync_ChReg_t reg, 
                           uint32_t val )
{
    Result res;
    sRTDExSync_Core_t * Core;
    unsigned int uiRTDExSyncBaseAddr;
    
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleRTDExSync, 0, &uiRTDExSyncBaseAddr);
    if(FAILURE(res)) return;
    
    Core = (sRTDExSync_Core_t *)uiRTDExSyncBaseAddr;
    
    Core->Ctrl.WrEn = 0;
    
    Carrier_usleep(hCarrier, 1);
    
    Core->Ctrl.ChIdx = u8Channel;
    Core->Ctrl.RegIdx = reg;
    Core->u32WrData = val;
    
    Carrier_usleep(hCarrier, 1);
    
    Core->Ctrl.WrEn = 1;
    
    Carrier_usleep(hCarrier, 1);
    
    Core->Ctrl.WrEn = 0;
    
    return;
}

void RTDExSync_ReadChReg( uint8_t u8Channel,
                          RTDExSync_ChReg_t reg, 
                          uint32_t * pu32Val )
{
    Result res;
    sRTDExSync_Core_t * Core;
    unsigned int uiRTDExSyncBaseAddr;
    
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleRTDExSync, 0, &uiRTDExSyncBaseAddr);
    if(FAILURE(res)) return;
    
    Core = (sRTDExSync_Core_t *)uiRTDExSyncBaseAddr;
    
    Core->Ctrl.WrEn = 0;
    
    Carrier_usleep(hCarrier, 1);
    
    Core->Ctrl.ChIdx = u8Channel;
    Core->Ctrl.RegIdx = reg;
    
    Carrier_usleep(hCarrier, 1);
    
    *pu32Val = Core->u32RdData;
    
    return;
}

/****************************************************************************
 *                             Public functions                             *
 ****************************************************************************/
 
Result RTDExSync_Presence_recv(connection_state * state)
{
    Result res = RTDEXSYNCOK;
    sRTDExSync_Core_t * Core;
    unsigned int uiRTDExSyncBaseAddr;
    
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleRTDExSync, 0, &uiRTDExSyncBaseAddr);
    if(FAILURE(res)) return res;
    
    Core = (sRTDExSync_Core_t *)uiRTDExSyncBaseAddr;
    
    // Validate the ID
	if ( Core->CoreVerId.coreid != RTDEXSYNC_COREID )
		return RTDEXSYNCCOREABSENT;
    
    return RTDEXSYNCOK;
}
 
Result RTDExSync_CoreReset_recv(connection_state * state)
{
    Result res;
    sRTDExSync_Core_t * Core;
    unsigned int uiRTDExSyncBaseAddr;
    
    res = Carrier_GetModuleBaseAddr(hCarrier, Carrier_eModuleRTDExSync, 0, &uiRTDExSyncBaseAddr);
    if(FAILURE(res)) return res;
    
    Core = (sRTDExSync_Core_t *)uiRTDExSyncBaseAddr;
    
    // Validate core presence
    res = RTDExSync_Presence_recv(state);
    if(FAILURE(res)) return res;
    
    // Set reset bit
    Core->Ctrl.CoreReset = 1;
    
    // Sleep to let time to latch the reset state
    Carrier_usleep(hCarrier, 1);
    
    // Clear reset bit
    Core->Ctrl.CoreReset = 0;
    
    return RTDEXSYNCOK;
}
 
Result RTDExSync_ChannelReset_recv(connection_state * state)
{
    Result res = RTDEXSYNCOK;
    uint32_t u32ChRegVal;
    uint8_t u8Channel;
    
    u8Channel = uchar_recv(state);
    
    // Validate core presence
    res = RTDExSync_Presence_recv(state);
    if(FAILURE(res)) return res;
    
    // Read the curretn value of the channel control register
    RTDExSync_ReadChReg( u8Channel, RTDExSync_ChReg_Ctrl, &u32ChRegVal );
    
    // Set the reset bit and clear the enable bit
    u32ChRegVal |= SET_FIELD(RTDExSync_CHREG_CTRL_RESET, 1);
    u32ChRegVal &= ~SET_FIELD(RTDExSync_CHREG_CTRL_ENABLE, 1);
    RTDExSync_WriteChReg( u8Channel, RTDExSync_ChReg_Ctrl, u32ChRegVal );
    
    // Sleep to let time to latch the reset state
    Carrier_usleep(hCarrier, 1);
    
    // Clear the reset bit
    u32ChRegVal &= ~SET_FIELD(RTDExSync_CHREG_CTRL_RESET, 1);
    RTDExSync_WriteChReg( u8Channel, RTDExSync_ChReg_Ctrl, u32ChRegVal );
    
    return res;
}
 
Result RTDExSync_ChannelStart_recv(connection_state * state)
{
    Result res = RTDEXSYNCOK;
    
    uint8_t u8Channel;
    uint32_t u32ChRegVal;
    uint32_t u32RxCtrl;
    uint32_t u32TxCtrl;
    uint32_t u32TxCntxCtrl;
    
    // Receive parameters from the host
    u8Channel = uchar_recv(state);
    u32RxCtrl = uint_recv(state);
    u32TxCtrl = uint_recv(state);
    u32TxCntxCtrl = uint_recv(state);
    
    // Validate core presence
    res = RTDExSync_Presence_recv(state);
    if(FAILURE(res)) return res;
    
    // Read the curretn value of the channel control register
    RTDExSync_ReadChReg( u8Channel, RTDExSync_ChReg_Ctrl, &u32ChRegVal );
    
    // Clear the enable bit
    u32ChRegVal &= ~SET_FIELD(RTDExSync_CHREG_CTRL_ENABLE, 1);
    RTDExSync_WriteChReg( u8Channel, RTDExSync_ChReg_Ctrl, u32ChRegVal );
    
    // Set the frame sizes
    RTDExSync_WriteChReg( u8Channel, RTDExSync_ChReg_RxCtrl,     u32RxCtrl );
    RTDExSync_WriteChReg( u8Channel, RTDExSync_ChReg_TxCtrl,     u32TxCtrl );
    RTDExSync_WriteChReg( u8Channel, RTDExSync_ChReg_TxCntxCtrl, u32TxCntxCtrl );
    
    // Sleep to let time for the frame sizes to be stable
    Carrier_usleep(hCarrier, 1);
    
    // Set the enable bit
    u32ChRegVal |= SET_FIELD(RTDExSync_CHREG_CTRL_ENABLE, 1);
    RTDExSync_WriteChReg( u8Channel, RTDExSync_ChReg_Ctrl, u32ChRegVal );
    
    return res;
}
 
Result RTDExSync_ChannelStop_recv(connection_state * state)
{
    Result res = RTDEXSYNCOK;
    
    uint8_t u8Channel;
    uint32_t u32ChRegVal;
    
    // Receive parameters from the host
    u8Channel = uchar_recv(state);
    
    // Validate core presence
    res = RTDExSync_Presence_recv(state);
    if(FAILURE(res)) return res;
    
    // Read the curretn value of the channel control register
    RTDExSync_ReadChReg( u8Channel, RTDExSync_ChReg_Ctrl, &u32ChRegVal );
    
    // Clear the enable bit
    u32ChRegVal &= ~SET_FIELD(RTDExSync_CHREG_CTRL_ENABLE, 1);
    RTDExSync_WriteChReg( u8Channel, RTDExSync_ChReg_Ctrl, u32ChRegVal );
    
    return res;
}
 
Result RTDExSync_ChannelInfo_recv(connection_state * state)
{
    Result res = RTDEXSYNCOK;
    
    uint8_t u8Channel;
    uint32_t u32ChRegVal;
    uint8_t u8Present = 0;
    uint8_t u8Dir = 0;
    
    // Receive parameters from the host
    u8Channel = uchar_recv(state);
    
    // Validate core presence
    res = RTDExSync_Presence_recv(state);
    if(SUCCESS(res)) 
    {
        // Read the curretn value of the channel control register
        RTDExSync_ReadChReg( u8Channel, RTDExSync_ChReg_Ctrl, &u32ChRegVal );
        
        // Get the bit
        u8Present = GET_FIELD(RTDExSync_CHREG_CTRL_PRESENT, u32ChRegVal);
        u8Dir     = GET_FIELD(RTDExSync_CHREG_CTRL_DIR, u32ChRegVal);
    }
    
    uchar_send(state, u8Present);
    uchar_send(state, u8Dir);
    
    return res;
}

