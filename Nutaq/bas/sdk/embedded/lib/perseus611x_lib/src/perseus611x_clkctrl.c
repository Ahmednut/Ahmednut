
#include "bas_error.h"
#include "perseus611x_clkctrl.h"
#include "perseus611x_def.h"
#include "perseus611x_regs.h"
#include "carrier_service_def.h"
#include "carrier_internal_def.h"

Result Perseus611X_TClkSetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t state)
{
    switch(tclk)
    {
        case Carrier_eTClkC:
        case Carrier_eTClkA:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            // Nothing to do, this carrier cannot have the TClkA and TClkC in TX direction
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    switch(state)
                    {
                        case Carrier_eClkEnable:
                            // Nothing to do, this carrier always have the TClkA and TClkC in RX direction
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eTClkD:
        case Carrier_eTClkB:
            switch(dir)
            {
                case Carrier_eTClkRx:
                case Carrier_eTClkTx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                        case Carrier_eClkEnable:
                            // Nothing to do, this carrier have TClkB and TClkD directly connected to the FPGA
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

Result Perseus611X_TClkGetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t * state)
{    
    switch(tclk)
    {
        case Carrier_eTClkA:
        case Carrier_eTClkC:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    *state = Carrier_eClkDisable;
                    break;
                    
                case Carrier_eTClkRx:
                    *state = Carrier_eClkEnable;
                    break;
                    
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eTClkB:
        case Carrier_eTClkD:
            switch(dir)
            {
                case Carrier_eTClkTx:
                case Carrier_eTClkRx:
                    *state = Carrier_eClkUnknown;
                    break;
                    
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;

        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

Result Perseus611X_TClkSetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t source)
{    
    switch(tclk)
    {
        case Carrier_eTClkB:
        case Carrier_eTClkD:
            switch(source)
            {
                case Carrier_eTClkFromFpga:
                    // Nothing to do, FPGA pins directly connected to AMC pins
                    break;
                                   
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;

        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

Result Perseus611X_TClkGetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t * source)
{    
    switch(tclk)
    {
        case Carrier_eTClkB:
        case Carrier_eTClkD:
            *source = Carrier_eTClkFromFpga;
            break;
            
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}


Result Perseus611X_FmcBidirClkSetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t connection)
{    
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    
    switch(clk)
    {
        case Carrier_eFmc1BidirClk2:
            switch(connection)
            {
                case Carrier_eFmcBidirClkWithFPGA:
                    pCore->stClkCtrl.stBit.m_uCtrlAmcTclkaToFmc1Clk2En = 0;
                    break;
                case Carrier_eFmcBidirClkFromTClk:
                    pCore->stClkCtrl.stBit.m_uCtrlAmcTclkaToFmc1Clk2En = 1;
                    break;
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eFmc1BidirClk3:
            switch(connection)
            {
                case Carrier_eFmcBidirClkWithFPGA:
                    pCore->stClkCtrl.stBit.m_uCtrlAmcTclkcToFmc1Clk3En = 0;
                    break;
                case Carrier_eFmcBidirClkFromTClk:
                    pCore->stClkCtrl.stBit.m_uCtrlAmcTclkcToFmc1Clk3En = 1;
                    break;
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eFmc2BidirClk2:
            switch(connection)
            {
                case Carrier_eFmcBidirClkWithFPGA:
                    pCore->stClkCtrl.stBit.m_uCtrlAmcTclkaToFmc2Clk2En = 0;
                    break;
                case Carrier_eFmcBidirClkFromTClk:
                    pCore->stClkCtrl.stBit.m_uCtrlAmcTclkaToFmc2Clk2En = 1;
                    break;
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eFmc2BidirClk3:
            switch(connection)
            {
                case Carrier_eFmcBidirClkWithFPGA:
                    pCore->stClkCtrl.stBit.m_uCtrlAmcTclkcToFmc2Clk3En = 0;
                    break;
                case Carrier_eFmcBidirClkFromTClk:
                    pCore->stClkCtrl.stBit.m_uCtrlAmcTclkcToFmc2Clk3En = 1;
                    break;
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
   
    return CARRIER_OK;
}

Result Perseus611X_FmcBidirClkGetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t * connection)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    
    switch(clk)
    {
        case Carrier_eFmc1BidirClk2:
            if(pCore->stClkCtrl.stBit.m_uCtrlAmcTclkaToFmc1Clk2En == 0)
            {
                *connection = Carrier_eFmcBidirClkWithFPGA;
            }
            else if(pCore->stClkCtrl.stBit.m_uCtrlAmcTclkaToFmc1Clk2En == 1)
            {
                *connection = Carrier_eFmcBidirClkFromTClk;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eFmc1BidirClk3:
            if(pCore->stClkCtrl.stBit.m_uCtrlAmcTclkcToFmc1Clk3En == 0)
            {
                *connection = Carrier_eFmcBidirClkWithFPGA;
            }
            else if(pCore->stClkCtrl.stBit.m_uCtrlAmcTclkcToFmc1Clk3En == 1)
            {
                *connection = Carrier_eFmcBidirClkFromTClk;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eFmc2BidirClk2:
            if(pCore->stClkCtrl.stBit.m_uCtrlAmcTclkaToFmc2Clk2En == 0)
            {
                *connection = Carrier_eFmcBidirClkWithFPGA;
            }
            else if(pCore->stClkCtrl.stBit.m_uCtrlAmcTclkaToFmc2Clk2En == 1)
            {
                *connection = Carrier_eFmcBidirClkFromTClk;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eFmc2BidirClk3:
            if(pCore->stClkCtrl.stBit.m_uCtrlAmcTclkcToFmc2Clk3En == 0)
            {
                *connection = Carrier_eFmcBidirClkWithFPGA;
            }
            else if(pCore->stClkCtrl.stBit.m_uCtrlAmcTclkcToFmc2Clk3En == 1)
            {
                *connection = Carrier_eFmcBidirClkFromTClk;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
   
    return CARRIER_OK;
}

Result Perseus611X_SetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t state)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    
    switch(osc)
    {
        case Carrier_eOscMgtRefClk100M :
            switch(state)
            {
                case Carrier_eClkDisable:
                    pCore->stClkCtrl.stBit.m_uMgtRefClk100En = 0;
                    break;
                case Carrier_eClkEnable:
                    pCore->stClkCtrl.stBit.m_uMgtRefClk100En = 1;
                    break;
                
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eOscMgtRefClk125M :
            switch(state)
            {
                case Carrier_eClkDisable:
                    pCore->stClkCtrl.stBit.m_uMgtRefClk125En = 0;
                    break;
                case Carrier_eClkEnable:
                    pCore->stClkCtrl.stBit.m_uMgtRefClk125En = 1;
                    break;
                
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eOscMgtRefClk156M25 :
            switch(state)
            {
                case Carrier_eClkDisable:
                    pCore->stClkCtrl.stBit.m_uMgtRefClk156En = 0;
                    break;
                case Carrier_eClkEnable:
                    pCore->stClkCtrl.stBit.m_uMgtRefClk156En = 1;
                    break;
                
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    return CARRIER_OK;
}

Result Perseus611X_GetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t * state)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    
    switch(osc)
    {
        case Carrier_eOscMgtRefClk100M :
            if(pCore->stClkCtrl.stBit.m_uMgtRefClk100En == 0)
            {
                *state = Carrier_eClkDisable;
            }
            else if(pCore->stClkCtrl.stBit.m_uMgtRefClk100En == 1)
            {
                *state = Carrier_eClkEnable;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eOscMgtRefClk125M :
            if(pCore->stClkCtrl.stBit.m_uMgtRefClk125En == 0)
            {
                *state = Carrier_eClkDisable;
            }
            else if(pCore->stClkCtrl.stBit.m_uMgtRefClk125En == 1)
            {
                *state = Carrier_eClkEnable;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eOscMgtRefClk156M25 :
            if(pCore->stClkCtrl.stBit.m_uMgtRefClk156En == 0)
            {
                *state = Carrier_eClkDisable;
            }
            else if(pCore->stClkCtrl.stBit.m_uMgtRefClk156En == 1)
            {
                *state = Carrier_eClkEnable;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

Result Perseus611X_FClkASetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t connection)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    
    Result res;
    Carrier_eClkState_t OscState;
    
    res = Perseus611X_GetOscState(hCarrier, Carrier_eOscMgtRefClk100M, &OscState);
    if(FAILURE(res)) return res;
    
    switch(connection)
    {
        case Carrier_eFClkAToMgtRefClk:
            if(OscState == Carrier_eClkEnable) return CARRIER_INVALID_100MOSCSTATE;
            
            pCore->stClkCtrl.stBit.m_uCtrlFclkaHighz = 0;
            break;
            
        case Carrier_eFClkAFrom100MOsc:
            if(OscState == Carrier_eClkDisable) return CARRIER_INVALID_100MOSCSTATE;
            
            pCore->stClkCtrl.stBit.m_uCtrlFclkaHighz = 0;
            break;
            
        case Carrier_eFClkADisable:
            pCore->stClkCtrl.stBit.m_uCtrlFclkaHighz = 1;
            
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

Result Perseus611X_FClkAGetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t * connection)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    
    Result res;
    Carrier_eClkState_t OscState;
    
    res = Perseus611X_GetOscState(hCarrier, Carrier_eOscMgtRefClk100M, &OscState);
    if(FAILURE(res)) return res;
    
    if(OscState == Carrier_eClkDisable && pCore->stClkCtrl.stBit.m_uCtrlFclkaHighz == 0)
    {
        *connection = Carrier_eFClkAToMgtRefClk;
    }
    else if(OscState == Carrier_eClkEnable && pCore->stClkCtrl.stBit.m_uCtrlFclkaHighz == 0)
    {
        *connection = Carrier_eFClkAFrom100MOsc;
    }
    else if(pCore->stClkCtrl.stBit.m_uCtrlFclkaHighz == 1)
    {
        *connection = Carrier_eFClkADisable;
    }
    else
    {
        return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

