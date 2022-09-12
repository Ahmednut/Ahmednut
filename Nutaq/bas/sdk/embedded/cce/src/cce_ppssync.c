#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>

#include "cce.h"
#include "eapi_serialize.h"

#include "cce_ppssync.h"
#include "ppssync_types.h"
#include "ppssync_defines.h"
#include "ppssync.h"

#include "cce_radio420.h"
#include "cce_adac250.h"

#include "FMCEEprom_eeprom_id.h"
#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

extern Carrier_handle_t hCarrier;
extern PPSSync_t * PPSSync[];
extern int ppssync_selected_inst;

static FMCEEprom_module_types _modlst[]={FMCEEPROM_EEPROM_MOD_ADAC250,FMCEEPROM_EEPROM_MOD_FMCRADIO, 
                                         FMCEEPROM_EEPROM_MOD_FMCRADIO,
                                         FMCEEPROM_EEPROM_MOD_FMCRADIOE, FMCEEPROM_EEPROM_MOD_FMCRADIOS,
                                         FMCEEPROM_EEPROM_MOD_FMCRADIOE1V8, FMCEEPROM_EEPROM_MOD_FMCRADIOS1V8,
                                         FMCEEPROM_EEPROM_MOD_RADIO640E, FMCEEPROM_EEPROM_MOD_RADIO640 };
                                         
/****************************************************************************
 *                             Private Functions                            *
 ****************************************************************************/
 
Result _PPSSync_Presence()
{
    Result res;
    CARRIER_detected mdetected = CARRIER_NOT_DETECTED;
	unsigned int ppssync_id;
	unsigned int ppssync_ver;

    res = Carrier_FmcValidatePresence(hCarrier,
        ppssync_selected_inst, Carrier_eFmcBoard0,
        _modlst, sizeof(_modlst)/sizeof(FMCEEprom_module_types),
        &mdetected);
    if (FAILURE(res)) return res;

    if (mdetected == CARRIER_NOT_DETECTED) return CCEINVALIDMOD;
    
    //check PPS-SYNC core ID & version
    res = PPSSync_Presence(PPSSync[ppssync_selected_inst], &ppssync_id, &ppssync_ver);
    if(FAILURE(res)) return res;

    return OK;
}
                                         
//Multi-thread
pthread_mutex_t pps_mutex = PTHREAD_MUTEX_INITIALIZER;

static void *PPSSync_Thread(void *state)
{
	PPSSync_t * PPSSyncThrd= (PPSSync_t*) state;

//	printf("[THRD] PPS-SYNC thread started %u\n",PPSSyncThrd->Start);

	pthread_mutex_lock(&pps_mutex);
	while((PPSSyncThrd->Start))
	{
		if(PPSSync_Tick(PPSSyncThrd))
		{
			PPSSyncThrd->BusErr++;
		}
//		printf("[PPS ] ERR=%4u INT_TIME=%3u TICK=%8u TERR=%3d CTE=%4d DAC=%5u GOOD=%10u PPB=%6d MAX_PPB=%6d LOST=%2u ELOCK=%1u LOCK=%1u\n", PPSSyncThrd->BusErr, PPSSyncThrd->CTEInterval, PPSSyncThrd->Tick, PPSSyncThrd->TickErr, PPSSyncThrd->CTE, (unsigned int)PPSSyncThrd->CurrDac, PPSSyncThrd->NumGoodClk, PPSSyncThrd->MeasuredPPB, PPSSyncThrd->MaxPPB, PPSSyncThrd->LockLost, PPSSyncThrd->CoarseLock, PPSSyncThrd->Lock);		
		sleep(CTE_INTERVAL);

	}	
	pthread_mutex_unlock(&pps_mutex);
	pthread_exit(NULL);
}


/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/

Result PPSSync_Presence_recv(connection_state * state)
{
    Result res = OK;
	unsigned int ppssync_id = 0;
	unsigned int ppssync_ver = 0;

    res = _PPSSync_Presence();
    if(FAILURE(res))
    {
        uint_send(state, ppssync_id);
        uint_send(state, ppssync_ver);	
        return res;
    }
    
    //check PPS-SYNC core ID & version
    res = PPSSync_Presence(PPSSync[ppssync_selected_inst], &ppssync_id, &ppssync_ver);	

	//send PPS-SYNC core ID  and version to MB
    uint_send(state, ppssync_id);
    uint_send(state, ppssync_ver);	
    return res;
}

Result PPSSync_DacInit_recv(connection_state * state)
{
	Result res = 0;
	float fDacVal;

	fDacVal = float_recv(state);

    res = _PPSSync_Presence();
    if(FAILURE(res)) return res;
    
    res = PPSSync_DacInit(PPSSync[ppssync_selected_inst], fDacVal);

	return res;
}

Result PPSSync_Configure_recv(connection_state * state)
{
	Result res = 0;
	float fMinDAC;
    float fMaxDAC;
    unsigned int uPpsFreq;

	fMinDAC = float_recv(state);
	fMaxDAC = float_recv(state);
	uPpsFreq = uint_recv(state);

    res = _PPSSync_Presence();
    if(FAILURE(res)) return res;
    
    res = PPSSync_Configure(PPSSync[ppssync_selected_inst], fMinDAC, fMaxDAC, uPpsFreq);
	return res;
}

Result PPSSync_Start_recv(connection_state * state)
{
	Result res = 0;
	float kp, ki;
	unsigned int freq;
	unsigned int int_time;
	pthread_t thread_id;
	
	freq = uint_recv(state);
	int_time = uint_recv(state);
	kp = float_recv(state);
	ki = float_recv(state);

    res = _PPSSync_Presence();
    if(FAILURE(res)) return res;
    
    //reset core
    PPSSync_Reset(PPSSync[ppssync_selected_inst]);
    
    sleep(2);

    res = PPSSync_PPSDetect(PPSSync[ppssync_selected_inst]);
    if(FAILURE(res))
    {
        printf("[ERR ] Failed to detect PPS pulse : 0x%x\n", res);
        return res;
    }

    res = PPSSync_Start(PPSSync[ppssync_selected_inst], freq, kp, ki, int_time);	
    if(FAILURE(res))
    {
        res = PPSSYNCSTARTFAILED;
    }

//	printf("[CCE ] freq=%u kp=%3.3f ki=%3.3f start=%d\n",freq, kp, ki, PPSSync[ppssync_selected_inst]->Start);

    res = pthread_create(&thread_id, NULL, PPSSync_Thread, PPSSync[ppssync_selected_inst]);			
//	printf("[CCE ] PPS-SYNC thread created %d\n", res);

	return res;
}

Result PPSSync_Stop_recv(connection_state * state)
{
	int res = 0;

    res = _PPSSync_Presence();
    if(FAILURE(res)) return res;
    
    res = PPSSync_Stop(PPSSync[ppssync_selected_inst]);

	return res;
}

Result PPSSync_ReadRefDacVal_recv(connection_state * state)
{
	int res = 0;
	float fDacVal = 0.0f;

    res = _PPSSync_Presence();
    if(FAILURE(res))
    {
        float_send(state, fDacVal);
        return res;
    }
    
    res = PPSSync_ReadRefDacVal(&fDacVal);	
    
    PPSSync[ppssync_selected_inst]->fCurrDac = fDacVal;

	float_send(state, fDacVal);

	return res;
}

Result PPSSync_SaveRefDacVal_recv(connection_state * state)
{
	int res = 0;

    res = _PPSSync_Presence();
    if(FAILURE(res)) return res;
    
    //save DAC 
    res = PPSSync_SaveRefDacVal(PPSSync[ppssync_selected_inst]->fCurrDac);	

	return res;
}

Result PPSSync_ReadPIProfile_recv(connection_state * state)
{
	int res = 0;
	float kp = 0.0f;
	float ki = 0.0f;

    res = _PPSSync_Presence();
    if(FAILURE(res))
    {
        float_send(state, kp);
        float_send(state, ki);
        return res;
    }
    
    //read PI profile
    res = PPSSync_ReadPIProfile(&kp, &ki);
    if(FAILURE(res))
    {
        res = PPSSYNCEEPROMFAILED;
    }

    PPSSync[ppssync_selected_inst]->Kp = kp;
    PPSSync[ppssync_selected_inst]->Ki = ki;
	
    //send to host
	float_send(state, kp);
	float_send(state, ki);

	return res;
}

Result PPSSync_SavePIProfile_recv(connection_state * state)
{
	int res = 0;
    
    res = _PPSSync_Presence();
    if(FAILURE(res)) return res;
    
    //save PI profile
    res = PPSSync_SavePIProfile(PPSSync[ppssync_selected_inst]->Kp, PPSSync[ppssync_selected_inst]->Ki);

	return res;
}

Result PPSSync_GetInfo_recv(connection_state * state)
{
	int res = 0;
	PPSSync_Info_t PPSSyncInfo;

	// Prepare dummy values
    PPSSyncInfo.TargetFreq = 0;
	PPSSyncInfo.TickPerPPS = 0;
	PPSSyncInfo.fCTE = 0.0f;
	PPSSyncInfo.fDACValue = 0.0f;
	PPSSyncInfo.Lock = 0;
	PPSSyncInfo.fPPB = 0.0f;

    res = _PPSSync_Presence();
    if(FAILURE(res))
    {
        uint_send(state, PPSSyncInfo.TargetFreq);
        uint_send(state, PPSSyncInfo.TickPerPPS);
        uint_send(state, (unsigned int)PPSSyncInfo.fCTE);
        uint_send(state, PPSSyncInfo.fDACValue);
        uint_send(state, PPSSyncInfo.Lock);
        uint_send(state, PPSSyncInfo.fPPB);
        return res;
    }
    
    //get PPS-SYNC info
    res = PPSSync_GetInfo(PPSSync[ppssync_selected_inst], &PPSSyncInfo);

	//send to host
	uint_send(state, PPSSyncInfo.TargetFreq);
	uint_send(state, PPSSyncInfo.TickPerPPS);
	float_send(state, PPSSyncInfo.fCTE);
	float_send(state, PPSSyncInfo.fDACValue);
	uint_send(state, PPSSyncInfo.Lock);
	float_send(state, PPSSyncInfo.fPPB);

	return res;

}
