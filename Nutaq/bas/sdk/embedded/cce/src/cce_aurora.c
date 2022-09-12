#include "eapi_serialize.h"
#include "bas_types.h"
#include "cce.h"
#include "cce_aurora.h"
#include "aurora.h"
#include "aurora_defines.h"
#include "bas_debug.h"

#include "carrier_service.h"
#include "carrier_service_def.h"

extern Carrier_handle_t hCarrier;
extern aurora_core aurora[];  

Result _Aurora_Presence(unsigned int nb)
{
    Result res;
    unsigned int uiMaxNum;
        
    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleAurora, &uiMaxNum);
    if (FAILURE(res)) return res;
    if (nb >= uiMaxNum) return CARRIER_INVALID_MODULE_INDEX;

    return AURORAOK;
}

Result Aurora_GetVersion_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    unsigned int uiVersion = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uint_send(state, uiVersion);
        return res;
    }

    res = Aurora_GetVersion(&aurora[nb], &uiVersion);
    uint_send(state, uiVersion);

    return res;
}

Result Aurora_ResetCore_recv(connection_state * state)
{
    Result res;
    unsigned int nb;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        return res;
    }

    res = Aurora_ResetCore(&aurora[nb]);

    return res;
}

Result Aurora_GetChannelStatus_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    unsigned int status = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uint_send(state, status);
        return res;
    }

    res = Aurora_GetChannelStatus(&aurora[nb], &status);
    uint_send(state, status);

    return res;
}

Result Aurora_ResetRxFifo_recv(connection_state * state)
{
    Result res;
    unsigned int nb;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        return res;
    }

    res = Aurora_ResetRxFifo(&aurora[nb]);

    return res;
}

Result Aurora_ResetTxFifo_recv(connection_state * state)
{
    Result res;
    unsigned int nb;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        return res;
    }

    res = Aurora_ResetTxFifo(&aurora[nb]);

    return res;
}

Result Aurora_GetRxDataCount_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint64_t ui64Value = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uint64_send(state, ui64Value);
        return res;
    }

    res = Aurora_GetRxDataCount(&aurora[nb], &ui64Value);
    uint64_send(state, ui64Value);

    return res;
}

Result Aurora_GetTxDataCount_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint64_t ui64Value = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uint64_send(state, ui64Value);
        return res;
    }

    res = Aurora_GetTxDataCount(&aurora[nb], &ui64Value);
    uint64_send(state, ui64Value);

    return res;
}

Result Aurora_GetRxDataRate_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint32_t ui32Value = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uint_send(state, ui32Value);
        return res;
    }

    res = Aurora_GetRxDataRate(&aurora[nb], &ui32Value);
    uint_send(state, ui32Value);

    return res;
}

Result Aurora_GetTxDataRate_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint32_t ui32Value = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uint_send(state, ui32Value);
        return res;
    }

    res = Aurora_GetTxDataRate(&aurora[nb], &ui32Value);
    uint_send(state, ui32Value);

    return res;
}

Result Aurora_SetGtxTxParam_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint8_t TxDiffCtrl = 0;
    uint8_t TxPostEmphasis = 0;
    uint8_t TxPreEmphasis = 0;

    nb = uint_recv(state);
    TxDiffCtrl = uchar_recv(state);
    TxPostEmphasis = uchar_recv(state);
    TxPreEmphasis = uchar_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        return res;
    }

    res = Aurora_SetGtxTxParam(&aurora[nb], TxDiffCtrl, TxPostEmphasis, TxPreEmphasis);

    return res;
}

Result Aurora_GetGtxTxParam_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint8_t TxDiffCtrl = 0;
    uint8_t TxPostEmphasis = 0;
    uint8_t TxPreEmphasis = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uchar_send(state, TxDiffCtrl);
        uchar_send(state, TxPostEmphasis);
        uchar_send(state, TxPreEmphasis);
        return res;
    }

    res = Aurora_GetGtxTxParam(&aurora[nb], &TxDiffCtrl, &TxPostEmphasis, &TxPreEmphasis);
    
    uchar_send(state, TxDiffCtrl);
    uchar_send(state, TxPostEmphasis);
    uchar_send(state, TxPreEmphasis);

    return res;
}

Result Aurora_SetGtxRxParam_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint8_t RxEqMix = 0;
    uint8_t DfeTap1 = 0;

    nb = uint_recv(state);
    RxEqMix = uchar_recv(state);
    DfeTap1 = uchar_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        return res;
    }

    res = Aurora_SetGtxRxParam(&aurora[nb], RxEqMix, DfeTap1);

    return res;
}

Result Aurora_GetGtxRxParam_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint8_t RxEqMix = 0;
    uint8_t DfeTap1 = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uchar_send(state, RxEqMix);
        uchar_send(state, DfeTap1);
        return res;
    }

    res = Aurora_GetGtxRxParam(&aurora[nb], &RxEqMix, &DfeTap1);
    
    uchar_send(state, RxEqMix);
    uchar_send(state, DfeTap1);

    return res;
}

Result Aurora_GetDfeEyeDacMon_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    uint8_t ui8Lane0 = 0;
    uint8_t ui8Lane1 = 0;
    uint8_t ui8Lane2 = 0;
    uint8_t ui8Lane3 = 0;

    nb = uint_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uchar_send(state, ui8Lane0);
        uchar_send(state, ui8Lane1);
        uchar_send(state, ui8Lane2);
        uchar_send(state, ui8Lane3);
        return res;
    }

    res = Aurora_GetDfeEyeDacMon(&aurora[nb], &ui8Lane0, &ui8Lane1, &ui8Lane2, &ui8Lane3);
    
    uchar_send(state, ui8Lane0);
    uchar_send(state, ui8Lane1);
    uchar_send(state, ui8Lane2);
    uchar_send(state, ui8Lane3);

    return res;
}

Result Aurora_SetChannelEnableState_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    Aurora_eDirection Direction = eAuroraDirectionRx;
    uint8_t Status = 0;

    nb = uint_recv(state);
    Direction = (Aurora_eDirection)uchar_recv(state);
    Status = uchar_recv(state);
    
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        return res;
    }
    res = Aurora_SetChannelEnableState(&aurora[nb], Direction, Status);
    return res;
}

Result Aurora_GetChannelEnableState_recv(connection_state * state)
{
    Result res;
    unsigned int nb;
    Aurora_eDirection Direction = eAuroraDirectionRx;
    uint8_t Status = 0;

    nb = uint_recv(state);
    Direction = (Aurora_eDirection)uchar_recv(state);
        
    res = _Aurora_Presence(nb);
    if(FAILURE(res))
    {
        uchar_send(state, Status);
        return res;
    }

    res = Aurora_GetChannelEnableState(&aurora[nb], Direction, &Status);
    uchar_send(state, Status);

    return res;
}