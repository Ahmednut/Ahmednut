#include <unistd.h>
#include <string.h>

#include "eapi_serialize.h"
#include "cce.h"
#include "cce_mi125.h"
#include "mi125_serialize.h"
#include "MI125_mi125.h"
#include "bas_debug.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

#ifdef DEBUG
// Uncomment following line for debug
#define MI125CCE_DEBUG 1
#endif

// Uncomment following line for simulation only
//#define SIMUL 1

//debug or no debug
#ifdef MI125CCE_DEBUG
#include <stdio.h>
#include <stdlib.h>
#define mdebug(f, ...) printf(f, __VA_ARGS__)
#else
#define mdebug(f, ...)
#endif

extern Carrier_handle_t hCarrier;
extern MI125_mi125 mi125_board[];

static FMCEEprom_module_types _modlst[]={FMCEEPROM_EEPROM_MOD_MI125, FMCEEPROM_EEPROM_MOD_MI125E, FMCEEPROM_EEPROM_MOD_MI125W, FMCEEPROM_EEPROM_MOD_MI125WE};

Result _Mi125_Presence(unsigned int pos)
{
    Result res;
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;
        
    res = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if (FAILURE(res)) return res;
    if (pos >= uiFmcMaxNum) return CARRIER_INVALID_MODULE_INDEX;
    
    iBoard = pos % CARRIER_FMC_MAXBOARD;
    iConnector = pos / CARRIER_FMC_MAXBOARD;

    res = Carrier_FmcValidatePresence(hCarrier,
        iConnector, iBoard,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if (FAILURE(res)) return res;

    if (mdetected == CARRIER_NOT_DETECTED) return MI125_MI125ABSENT;

    return MI125_MI125OK;
}

Result MI125_Presence_recv(connection_state * state)
{
    Result res = MI125_MI125OK;
    unsigned nb;

    // Get desired board instance
    nb = uint_recv(state);

    mdebug("MI125_Presence #%d\n",nb+1); 

    res = _Mi125_Presence(nb);
    if(FAILURE(res)) return res;
    
    return MI125_MI125OK;
}

Result MI125_powerup_recv(connection_state * state)
{
    Result res = MI125_MI125OK;
    unsigned nb;
    int iConnector, iBoard;

    // Get desired board instance
    nb = uint_recv(state);

	mdebug("MI125_powerup #%d\n",nb+1); 

    res = _Mi125_Presence(nb);
    if(FAILURE(res)) return res;
    
    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;
    
#ifndef SIMUL
    res = Carrier_FmcPowerUp(hCarrier,
        iConnector, iBoard,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        VADJ_2_503V);
#endif
    
    return res;
}

Result MI125_mi125_reset_recv(connection_state * state)
{
    Result res = MI125_MI125OK;
    unsigned nb;

    nb = uint_recv(state);

	mdebug("MI125_mi125_reset #%d\n",nb+1); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) return res;
    
    res = MI125_mi125_reset(&mi125_board[nb]);
#endif
    return res;
}

Result MI125_mi125_digital_adccalibration_recv(connection_state * state)
{
    Result res = MI125_MI125OK;
    unsigned nb;

    nb = uint_recv(state);

	mdebug("MI125_mi125_digital_adccalibration #%d\n",nb+1); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) return res;
    
	res = MI125_mi125_digital_adccalibration(&mi125_board[nb]);   
#endif
    return res;
}

Result MI125_mi125_set_config_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    MI125_mi125_config config;
    unsigned nb;

    nb = uint_recv(state);
    mi125_set_config_param_recv(state, &config);
	
	mdebug("MI125_mi125_set_config #%d groupch: %d\n",nb+1,config.groupch); 
	mdebug("MI125_mi125_set_config lvds: %d\n",config.lvds); 
	mdebug("MI125_mi125_set_config randmode: %d\n",config.randmode); 
	mdebug("MI125_mi125_set_config binmode: %d\n",config.binmode); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) return res;
    
    res = MI125_mi125_set_config(&mi125_board[nb], &config);
#endif
    return res;
}

Result MI125_mi125_set_clksrc_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    MI125_mi125_clock_source clksrc;
    unsigned nb;

    nb = uint_recv(state);
	clksrc = uint_recv(state); 

    mdebug("MI125_mi125_set_clksrc #%d clksrc: %d\n",nb+1, clksrc);
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) return res;
    
	res = MI125_mi125_set_clksrc(&mi125_board[nb], clksrc);
#endif
    return res;
}

Result MI125_mi125_checkcore_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    unsigned char detected=0;
    unsigned nb;

    nb = uint_recv(state);

	mdebug("MI125_mi125_checkcore #%d\n",nb+1); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        uchar_send(state, detected);
        return res; 
    }
	res = MI125_mi125_checkcore(&mi125_board[nb], &detected);
#else
    detected = 1;   
#endif
    uchar_send(state, detected);
    return res;
}

Result MI125_mi125_set_testmode_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    MI125_mi125_testmode test;
    unsigned short pattern;
    unsigned nb;

    nb = uint_recv(state);

	test = uint_recv(state);
    pattern = ushort_recv(state); 

	mdebug("MI125_mi125_set_testmode #%d test: %d\n",nb+1,test); 
	mdebug("MI125_mi125_set_testmode pattern: 0x%04X\n",pattern); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) return res;
    
    res = MI125_mi125_set_testmode(&mi125_board[nb],  test, pattern);
#endif
    return res;
}

Result MI125_mi125_get_temperature_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
	MI125_mi125_tempmode mode;
    short temp=5555;
    unsigned nb;

    nb = uint_recv(state);
	mode = uint_recv(state); 

	mdebug("MI125_mi125_get_temperature #%d mode: %d\n",nb+1,mode); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        ushort_send(state, temp);
        return res; 
    }
	res = MI125_mi125_get_temperature(&mi125_board[nb], mode, &temp);
#else
    if (mode==MI125_TEMP0DOT1C)
        temp = 440;
    else
        temp = 44;    
#endif
	mdebug("MI125_mi125_get_temperature temp: %d\n",temp); 
	ushort_send(state, temp);
    return res;
}

Result MI125_mi125_get_channelcalibstatus_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    unsigned int channellanecalib=0;
    unsigned int channelcalibstatus=0;
    unsigned nb;

    nb = uint_recv(state);

	mdebug("MI125_mi125_get_channelcalibstatus #%d\n",nb+1); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        uint_send(state, channellanecalib);
        uint_send(state, channelcalibstatus);
        return res; 
    } 
	res = MI125_mi125_get_channelcalibstatus(&mi125_board[nb], &channellanecalib, &channelcalibstatus);
#else
    channelcalibstatus = 1;
    channellanecalib = (unsigned int)(0xFFFFFFFF);
#endif
	mdebug("MI125_mi125_get_channelcalibstatus channellanecalib: 0x%08X\n",channellanecalib); 
	mdebug("MI125_mi125_get_channelcalibstatus channelcalibstatus: %d\n",channelcalibstatus); 
	uint_send(state, channellanecalib);
	uint_send(state, channelcalibstatus);
    return res;
}

Result MI125_mi125_get_versions_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    unsigned int core_version=0, drv_version=0; 
    unsigned nb;

    nb = uint_recv(state);

	mdebug("MI125_mi125_get_versions #%d\n",nb+1); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        uint_send(state, core_version);
        uint_send(state, drv_version);
        return res; 
    }   
	res = MI125_mi125_get_versions(&mi125_board[nb], &core_version, &drv_version);
#else
    core_version = 0xaaaa;
    drv_version = 0x5555;    
#endif
	mdebug("MI125_mi125_get_versions core_version: %d\n",core_version); 
	mdebug("MI125_mi125_get_versions drv_version: %d\n",drv_version); 
	uint_send(state, core_version);
	uint_send(state, drv_version);
    return res;
}

Result MI125_mi125_checkadc_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    MI125_mi125_adcid adcid;
    unsigned char written=0;
    unsigned char verify=0;
    unsigned char detected=0;
    unsigned nb;

    nb = uint_recv(state);
	adcid = uint_recv(state); 

	mdebug("MI125_mi125_checkadc #%d adcid: %d\n",nb+1,adcid); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        uchar_send(state, written);
        uchar_send(state, verify);
        uchar_send(state, detected);
        return res; 
    }  
	res = MI125_mi125_checkadc(&mi125_board[nb], &written, &verify, &detected, adcid);
#else    
    written = 0x55;
    verify = 0x55;
    detected = 1;
#endif
	mdebug("MI125_mi125_checkadc written: %d\n",written); 
	mdebug("MI125_mi125_checkadc verify: %d\n",verify); 
	mdebug("MI125_mi125_checkadc detected: %d\n",detected); 
    uchar_send(state, written);
    uchar_send(state, verify);
    uchar_send(state, detected);
    return res;
}

Result MI125_mi125_set_trigout_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    MI125_mi125_trigout trigout;
    unsigned nb;

    nb = uint_recv(state);
	trigout = uint_recv(state); 

	mdebug("MI125_mi125_set_trigout #%d trigout: %d\n",nb+1,trigout); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        return res; 
    }  
	res = MI125_mi125_set_trigout(&mi125_board[nb], trigout);
#endif
    return res;
}


Result MI125_mi125_get_clkmaster_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    unsigned int master=0;
    unsigned nb;

    nb = uint_recv(state);

	mdebug("MI125_mi125_get_clkmaster #%d\n",nb+1); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        uint_send(state, master);
        return res; 
    }   
    res = MI125_mi125_get_clkmaster(&mi125_board[nb], &master);
#else
    master = 1;
#endif
	mdebug("MI125_mi125_get_clkmaster: 0x%01X\n",master); 
	uint_send(state, master);
    return res;
}


Result MI125_mi125_clockreset_recv(connection_state * state)
{
    Result res = MI125_MI125OK;
    unsigned nb;

    nb = uint_recv(state);

	mdebug("MI125_mi125_clockreset #%d\n",nb+1); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        return res; 
    }     
	res = MI125_mi125_clockreset(&mi125_board[nb]);
#endif
    return res;
}


Result MI125_mi125_scan_i2c_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    unsigned char scanmax;
    MI125_mi125_i2cscan scanraw;
    MI125_mi125_i2caddr scan_res;
    unsigned char ndevices=0;;
    unsigned char detect_ok=0;
    unsigned nb;

    memset(&scanraw,0,sizeof(MI125_mi125_i2cscan));
    memset(&scan_res,0,sizeof(MI125_mi125_i2caddr));
    nb = uint_recv(state);
	scanmax = uchar_recv(state); 

	mdebug("MI125_mi125_scan_i2c #%d scanmax: %d\n",nb+1,scanmax); 
#ifndef SIMUL 
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        uchar_send(state, ndevices);
        uchar_send(state, detect_ok);
        buf_send(state, (char*)&scanraw, sizeof(MI125_mi125_i2cscan));
        buf_send(state, (char*)&scan_res, sizeof(MI125_mi125_i2caddr));
        return res; 
    }    
    res = MI125_mi125_scan_i2c(&mi125_board[nb], scanmax, &scanraw,
                               &scan_res, &ndevices, &detect_ok);
#else    
    detect_ok = 1;
    ndevices = scanmax;
#endif
	mdebug("MI125_mi125_scan_i2c detect_ok: %d\n",detect_ok); 
	mdebug("MI125_mi125_scan_i2c ndevices: %d\n",ndevices); 
    uchar_send(state, ndevices);
    uchar_send(state, detect_ok);
    buf_send(state, (char*)&scanraw, sizeof(MI125_mi125_i2cscan));
    buf_send(state, (char*)&scan_res, sizeof(MI125_mi125_i2caddr));
    return res;
}


Result MI125_mi125_test_localoscsw_recv(connection_state * state)
{
    Result res = MI125_MI125OK; 
    unsigned char testok=0;
    unsigned nb;

    nb = uint_recv(state);

	mdebug("MI125_mi125_test_localoscsw #%d\n",nb+1); 
#ifndef SIMUL
    res = _Mi125_Presence(nb);
    if(FAILURE(res)) 
    {
        uchar_send(state, testok);
        return res; 
    }     
    res = MI125_mi125_test_localoscsw(&mi125_board[nb], &testok);   
#else
    testok = 1;
#endif
	mdebug("MI125_mi125_test_localoscsw: 0x%01X\n",testok); 
	uchar_send(state, testok);
    return res;
}
