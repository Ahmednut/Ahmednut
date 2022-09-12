
#include "bas_error.h"
#include "perseus601x_clkctrl.h"
#include "perseus601x_def.h"
#include "perseus601x_regs.h"
#include "carrier_service_def.h"
#include "carrier_internal_def.h"

Result Perseus601X_TClkSetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t state)
{   
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;

    switch(tclk)
    {
        case Carrier_eTClkA:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            pCore->CLK_CTRL.CTRL_TCLKA_TX_EN = 0;
                            break;
                        case Carrier_eClkEnable:
                            pCore->CLK_CTRL.CTRL_TCLKA_TX_EN = 1;
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            pCore->CLK_CTRL.CTRL_TCLKA_RX_DIS = 1;
                            break;
                        case Carrier_eClkEnable:
                            pCore->CLK_CTRL.CTRL_TCLKA_RX_DIS = 0;
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
        case Carrier_eTClkB:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            pCore->CLK_CTRL.CTRL_TCLKB_TX_EN = 0;
                            break;
                        case Carrier_eClkEnable:
                            pCore->CLK_CTRL.CTRL_TCLKB_TX_EN = 1;
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            pCore->CLK_CTRL.CTRL_TCLKB_RX_DIS = 1;
                            break;
                        case Carrier_eClkEnable:
                            pCore->CLK_CTRL.CTRL_TCLKB_RX_DIS = 0;
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
        case Carrier_eTClkC:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            pCore->CLK_CTRL.CTRL_TCLKC_TX_EN = 0;
                            break;
                        case Carrier_eClkEnable:
                            pCore->CLK_CTRL.CTRL_TCLKC_TX_EN = 1;
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            pCore->CLK_CTRL.CTRL_TCLKC_RX_DIS = 1;
                            break;
                        case Carrier_eClkEnable:
                            pCore->CLK_CTRL.CTRL_TCLKC_RX_DIS = 0;
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
            switch(dir)
            {
                case Carrier_eTClkTx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            pCore->CLK_CTRL.CTRL_TCLKD_TX_EN = 0;
                            break;
                        case Carrier_eClkEnable:
                            pCore->CLK_CTRL.CTRL_TCLKD_TX_EN = 1;
                            break;
                        default:
                            return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    switch(state)
                    {
                        case Carrier_eClkDisable:
                            pCore->CLK_CTRL.CTRL_TCLKD_RX_DIS = 1;
                            break;
                        case Carrier_eClkEnable:
                            pCore->CLK_CTRL.CTRL_TCLKD_RX_DIS = 0;
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

Result Perseus601X_TClkGetState(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkDir_t dir, Carrier_eClkState_t * state)
{
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    switch(tclk)
    {
        case Carrier_eTClkA:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    if(pCore->CLK_CTRL.CTRL_TCLKA_TX_EN == 0)
                    {
                        *state = Carrier_eClkDisable;
                    }
                    else if(pCore->CLK_CTRL.CTRL_TCLKA_TX_EN == 1)
                    {
                        *state = Carrier_eClkEnable;
                    }
                    else
                    {
                        return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    if(pCore->CLK_CTRL.CTRL_TCLKA_RX_DIS == 1)
                    {
                        *state = Carrier_eClkDisable;
                    }
                    else if(pCore->CLK_CTRL.CTRL_TCLKA_RX_DIS == 0)
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
            break;
        case Carrier_eTClkB:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    if(pCore->CLK_CTRL.CTRL_TCLKB_TX_EN == 0)
                    {
                        *state = Carrier_eClkDisable;
                    }
                    else if(pCore->CLK_CTRL.CTRL_TCLKB_TX_EN == 1)
                    {
                        *state = Carrier_eClkEnable;
                    }
                    else
                    {
                        return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    if(pCore->CLK_CTRL.CTRL_TCLKB_RX_DIS == 1)
                    {
                        *state = Carrier_eClkDisable;
                    }
                    else if(pCore->CLK_CTRL.CTRL_TCLKB_RX_DIS == 0)
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
            break;
        case Carrier_eTClkC:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    if(pCore->CLK_CTRL.CTRL_TCLKC_TX_EN == 0)
                    {
                        *state = Carrier_eClkDisable;
                    }
                    else if(pCore->CLK_CTRL.CTRL_TCLKC_TX_EN == 1)
                    {
                        *state = Carrier_eClkEnable;
                    }
                    else
                    {
                        return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    if(pCore->CLK_CTRL.CTRL_TCLKC_RX_DIS == 1)
                    {
                        *state = Carrier_eClkDisable;
                    }
                    else if(pCore->CLK_CTRL.CTRL_TCLKC_RX_DIS == 0)
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
            break;
        case Carrier_eTClkD:
            switch(dir)
            {
                case Carrier_eTClkTx:
                    if(pCore->CLK_CTRL.CTRL_TCLKD_TX_EN == 0)
                    {
                        *state = Carrier_eClkDisable;
                    }
                    else if(pCore->CLK_CTRL.CTRL_TCLKD_TX_EN == 1)
                    {
                        *state = Carrier_eClkEnable;
                    }
                    else
                    {
                        return CARRIER_INVALID_CLOCKCONFIG;
                    }
                    break;
                    
                case Carrier_eTClkRx:
                    if(pCore->CLK_CTRL.CTRL_TCLKD_RX_DIS == 1)
                    {
                        *state = Carrier_eClkDisable;
                    }
                    else if(pCore->CLK_CTRL.CTRL_TCLKD_RX_DIS == 0)
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
            break;
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

Result Perseus601X_TClkSetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t source)
{
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    switch(tclk)
    {
        case Carrier_eTClkA:
        case Carrier_eTClkC:
            switch(source)
            {
                case Carrier_eTClkFromFpga:
                    // Nothing to do
                    break;
                    
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
        case Carrier_eTClkB:
            switch(source)
            {
                case Carrier_eTClkFromFpga:
                    pCore->CLK_CTRL.CTRL_FMC_CLK0_2_AMC_TCLKB_EN = 0;
                    break;
                    
                case Carrier_eTClkFromFmc:
                    pCore->CLK_CTRL.CTRL_FMC_CLK0_2_AMC_TCLKB_EN = 1;
                    break;
                    
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
        case Carrier_eTClkD:
            switch(source)
            {
                case Carrier_eTClkFromFpga:
                    pCore->CLK_CTRL.CTRL_FMC_CLK1_2_AMC_TCLKD_EN = 0;
                    break;
                    
                case Carrier_eTClkFromFmc:
                    pCore->CLK_CTRL.CTRL_FMC_CLK1_2_AMC_TCLKD_EN = 1;
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

Result Perseus601X_TClkGetTxSource(Carrier_handle_t hCarrier, Carrier_eTClk_t tclk, Carrier_eTClkSrc_t * source)
{
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    switch(tclk)
    {
        case Carrier_eTClkA:
        case Carrier_eTClkC:
            *source = Carrier_eTClkFromFpga;
            break;
        case Carrier_eTClkB:
            if(pCore->CLK_CTRL.CTRL_FMC_CLK0_2_AMC_TCLKB_EN == 0)
            {
                *source = Carrier_eTClkFromFpga;
            }
            else if(pCore->CLK_CTRL.CTRL_FMC_CLK0_2_AMC_TCLKB_EN == 1)
            {
                *source = Carrier_eTClkFromFmc;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
        case Carrier_eTClkD:
            if(pCore->CLK_CTRL.CTRL_FMC_CLK1_2_AMC_TCLKD_EN == 0)
            {
                *source = Carrier_eTClkFromFpga;
            }
            else if(pCore->CLK_CTRL.CTRL_FMC_CLK1_2_AMC_TCLKD_EN == 1)
            {
                *source = Carrier_eTClkFromFmc;
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


Result Perseus601X_FmcBidirClkSetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t connection)
{    
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    switch(clk)
    {
        case Carrier_eFmc1BidirClk2:
            switch(connection)
            {
                case Carrier_eFmcBidirClkWithFPGA:
                    pCore->CLK_CTRL.CTRL_AMC_TCLKA_2_FMC_CLK2_EN = 0;
                    break;
                case Carrier_eFmcBidirClkFromTClk:
                    pCore->CLK_CTRL.CTRL_AMC_TCLKA_2_FMC_CLK2_EN = 1;
                    break;
                default:
                    return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eFmc1BidirClk3:
            switch(connection)
            {
                case Carrier_eFmcBidirClkWithFPGA:
                    pCore->CLK_CTRL.CTRL_AMC_TCLKC_2_FMC_CLK3_EN = 0;
                    break;
                case Carrier_eFmcBidirClkFromTClk:
                    pCore->CLK_CTRL.CTRL_AMC_TCLKC_2_FMC_CLK3_EN = 1;
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

Result Perseus601X_FmcBidirClkGetConnection(Carrier_handle_t hCarrier, Carrier_eFmcBidirClk_t clk, Carrier_eFmcBidirClkConnection_t * connection)
{
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    switch(clk)
    {
        case Carrier_eFmc1BidirClk2:
            if(pCore->CLK_CTRL.CTRL_AMC_TCLKA_2_FMC_CLK2_EN == 0)
            {
                *connection = Carrier_eFmcBidirClkWithFPGA;
            }
            else if(pCore->CLK_CTRL.CTRL_AMC_TCLKA_2_FMC_CLK2_EN == 1)
            {
                *connection = Carrier_eFmcBidirClkFromTClk;
            }
            else
            {
                return CARRIER_INVALID_CLOCKCONFIG;
            }
            break;
            
        case Carrier_eFmc1BidirClk3:
            if(pCore->CLK_CTRL.CTRL_AMC_TCLKC_2_FMC_CLK3_EN == 0)
            {
                *connection = Carrier_eFmcBidirClkWithFPGA;
            }
            else if(pCore->CLK_CTRL.CTRL_AMC_TCLKC_2_FMC_CLK3_EN == 1)
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

Result Perseus601X_SetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t state)
{
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    switch(osc)
    {
        case Carrier_eOscMgtRefClk100M :
            switch(state)
            {
                case Carrier_eClkDisable:
                    pCore->CLK_CTRL.CTRL_100_MHZ_OUT_EN = 0;
                    break;
                case Carrier_eClkEnable:
                    pCore->CLK_CTRL.CTRL_100_MHZ_OUT_EN = 1;
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

Result Perseus601X_GetOscState(Carrier_handle_t hCarrier, Carrier_eOscillator_t osc, Carrier_eClkState_t * state)
{
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    switch(osc)
    {
        case Carrier_eOscMgtRefClk100M :
            if(pCore->CLK_CTRL.CTRL_100_MHZ_OUT_EN == 0)
            {
                *state = Carrier_eClkDisable;
            }
            else if(pCore->CLK_CTRL.CTRL_100_MHZ_OUT_EN == 1)
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

Result Perseus601X_FClkASetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t connection)
{
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    Result res;
    Carrier_eClkState_t OscState;
    
    res = Perseus601X_GetOscState(hCarrier, Carrier_eOscMgtRefClk100M, &OscState);
    if(FAILURE(res)) return res;
    
    switch(connection)
    {
        case Carrier_eFClkAToMgtRefClk:
            if(OscState == Carrier_eClkEnable) return CARRIER_INVALID_100MOSCSTATE;
            
            pCore->CLK_CTRL.CTRL_FCLKA_HIGHZ = 0;
            break;
            
        case Carrier_eFClkAFrom100MOsc:
            if(OscState == Carrier_eClkDisable) return CARRIER_INVALID_100MOSCSTATE;
            
            pCore->CLK_CTRL.CTRL_FCLKA_HIGHZ = 0;
            break;
            
        case Carrier_eFClkADisable:
            pCore->CLK_CTRL.CTRL_FCLKA_HIGHZ = 1;
            
        default:
            return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

Result Perseus601X_FClkAGetConnection(Carrier_handle_t hCarrier, Carrier_eFClkAConnection_t * connection)
{
    volatile sPerseus601X_t * pCore = (sPerseus601X_t *)hCarrier->pCore;
    
    Result res;
    Carrier_eClkState_t OscState;
    
    res = Perseus601X_GetOscState(hCarrier, Carrier_eOscMgtRefClk100M, &OscState);
    if(FAILURE(res)) return res;
    
    if(OscState == Carrier_eClkDisable && pCore->CLK_CTRL.CTRL_FCLKA_HIGHZ == 0)
    {
        *connection = Carrier_eFClkAToMgtRefClk;
    }
    else if(OscState == Carrier_eClkEnable && pCore->CLK_CTRL.CTRL_FCLKA_HIGHZ == 0)
    {
        *connection = Carrier_eFClkAFrom100MOsc;
    }
    else if(pCore->CLK_CTRL.CTRL_FCLKA_HIGHZ == 1)
    {
        *connection = Carrier_eFClkADisable;
    }
    else
    {
        return CARRIER_INVALID_CLOCKCONFIG;
    }
    
    return CARRIER_OK;
}

