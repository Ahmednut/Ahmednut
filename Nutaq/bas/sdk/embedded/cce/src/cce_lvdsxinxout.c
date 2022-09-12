
#include "cce.h"
#include "eapi_serialize.h"
#include "bas_debug.h"

#include "cce_lvdsxinxout.h"
#include "fmclvds_pcf8574.h"
#include "fmclvds_pcf8574_defines.h"
#include "fmclvds_defines.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

extern Carrier_handle_t hCarrier;
extern unsigned selected_fmclvds;
extern fmclvds fmclvds_handle[];
extern FMCLVDS_pcf8574_core fmclvds_com_handle[];

static FMCEEprom_module_types _modlst[]={FMCEEPROM_EEPROM_MOD_LVDS, FMCEEPROM_EEPROM_MOD_LVDSE};

Result _FmcLvds_Presence()
{
    Result res;
    int iConnector, iBoard;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;
    
    iBoard = selected_fmclvds % CARRIER_FMC_MAXBOARD;
    iConnector = selected_fmclvds / CARRIER_FMC_MAXBOARD;

    res = Carrier_FmcValidatePresence(hCarrier,
        iConnector, iBoard,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if FAILURE(res) return res;

    if (mdetected == CARRIER_NOT_DETECTED) return CCEINVALIDMOD;

    return OK;
}

Result fmclvds_init_recv(connection_state * state)
{
	int res=OK;
    
    res = _FmcLvds_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = FMCLVDS_pcf8574_reset(&fmclvds_com_handle[selected_fmclvds]);
    
    return res;
}

Result fmclvds_powerup_recv(connection_state * state)
{
	int res=OK;
    int iConnector, iBoard;
    
    iBoard = selected_fmclvds % CARRIER_FMC_MAXBOARD;
    iConnector = selected_fmclvds / CARRIER_FMC_MAXBOARD;

    res = Carrier_FmcPowerUp(hCarrier,
        iConnector, iBoard,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        VADJ_2_503V);
    return res;
}

Result fmclvds_presence_recv(connection_state * state)
{
	return _FmcLvds_Presence();
}

Result fmclvds_select_send(connection_state * state)
{   
    int res;
    unsigned nb; 
    unsigned int uiFmcMaxNum;
    
    nb = uint_recv(state);
    
    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(res)) return res;

    debug("fmclvds_select_send: current selected lvds: %d\n", selected_fmclvds);
    if(nb >= uiFmcMaxNum)
    {
        res = FMCLVDSIVAL;
    }
	else
	{
		selected_fmclvds = nb;
        debug("fmclvds_select_send: new selected lvds: %d\n", selected_fmclvds);
        res = OK;
    }

    return res; 
}

Result fmclvds_setdir_recv(connection_state * state)
{
    int res;
    FMCLVDS_GROUPS group;
    FMCLVDS_DIR dir;
    debug("dir",0);
    group = uint_recv(state);
    debug(" group %d",group);
    dir = uint_recv(state);
    debug(" dir %d",dir );

    res = _FmcLvds_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = fmclvds_setdir(&fmclvds_handle[selected_fmclvds], group, dir);
    
    return res;
}

Result fmclvds_pwdn_recv(connection_state * state)
{
    int res;
    FMCLVDS_GROUPS group;
    FMCLVDS_pcf8574_pwdn power;

    group = uint_recv(state);
    power = uint_recv(state);

    res = _FmcLvds_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = FMCLVDS_pcf8574_powerdown(&fmclvds_com_handle[selected_fmclvds], group, power);

    return res;
}

Result fmclvds_preemp_recv(connection_state * state)
{
    int res;
    FMCLVDS_GROUPS group;
    FMCLVDS_pcf8574_preemp preemp;

    group = uint_recv(state);
    preemp = uint_recv(state);

    res = _FmcLvds_Presence();
    if(FAILURE(res))
    {
        return res;
    }
    
    res = FMCLVDS_pcf8574_preemphasis(&fmclvds_com_handle[selected_fmclvds], group, preemp);

    return res;
}

