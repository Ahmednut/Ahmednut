
/****************************************************************************
 *
 *    ****                              *
 *   ******                            ***
 *   *******                           ****
 *   ********    ****  ****     **** *********    ******* ****    ***********
 *   *********   ****  ****     **** *********  **************  *************
 *   **** *****  ****  ****     ****   ****    *****    ****** *****     ****
 *   ****  ***** ****  ****     ****   ****   *****      ****  ****      ****
 *  ****    *********  ****     ****   ****   ****       ****  ****      ****
 *  ****     ********  ****    *****  ****    *****     *****  ****      ****
 *  ****      ******   ***** ******   *****    ****** *******  ****** *******
 *  ****        ****   ************    ******   *************   *************
 *  ****         ***     ****  ****     ****      *****  ****     *****  ****
 *                                                                       ****
 *          I N N O V A T I O N  T O D A Y  F O R  T O M M O R O W       ****
 *                                                                        ***
 *
 ************************************************************************//**
 *
 * @file       ppssync.c
 * @brief      Function definitions for PPS-SYNC
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * This file is part of Nutaq’s BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq’s BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq’s BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq’s BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq’s BAS Software Suite under the GPL requires that your work based on
 * Nutaq’s BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq’s BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq’s BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bas_error.h"
#include "bas_debug.h"
#include "ppssync_types.h"
#include "ppssync_defines.h"
#include "ppssync.h"
#include "FMCEEprom_eeprom_id.h"
//#include "adac250.h"
//#include "bas_spi.h"
//#include "LTC2641_drv.h"

#define MALLOC(A)  malloc(A)
#define FREE       free

int cte_vector[CTE_MAX_INTERVAL];
int	cte_last = 0;
int	tick_last = 0;
//ADAC250Board_t *ADAC250Board;

static Result PPSSync_ReadReg(PPSSync_t *PPSSync, PPSSYNC_REGISTER Register, int *RegisterValue)
{
    switch(Register)
    {
        case PPSSYNC_COREIDVERS:
            *RegisterValue = PPSSync->PPSSyncCore->Core_ID_Vers.Register;
            break;

        case PPSSYNC_CLKTARGET:
            *RegisterValue = PPSSync->PPSSyncCore->ClkTarget.Register;
            break;

        case PPSSYNC_CONTROL:
            *RegisterValue = PPSSync->PPSSyncCore->Control.Register;
            break;

        case PPSSYNC_TICKCNT:
            *RegisterValue = PPSSync->PPSSyncCore->Tick.Register;
            break;

        case PPSSYNC_CTE:
            *RegisterValue = PPSSync->PPSSyncCore->CTE.Register;
            break;

        case PPSSYNC_MOVSUMLEN:
        	*RegisterValue = PPSSync->PPSSyncCore->MOVSUMLEN.Register;
            break;

        case PPSSYNC_PPSFREQ:
        	*RegisterValue = PPSSync->PPSSyncCore->PpsFreq;
            break;

       default:
            return -1;
            break;
	}
	return 0;

}

static Result PPSSync_WriteReg(PPSSync_t *PPSSync, PPSSYNC_REGISTER Register, int RegisterValue)
{
	if (!RegisterValue)
        return -1;

	switch(Register)
    {
        case PPSSYNC_CLKTARGET:
            PPSSync->PPSSyncCore->ClkTarget.Register = RegisterValue;
            break;

        case PPSSYNC_CONTROL:
   			PPSSync->PPSSyncCore->Control.Register = RegisterValue;
            break;

        case PPSSYNC_MOVSUMLEN:
        	PPSSync->PPSSyncCore->MOVSUMLEN.Register = RegisterValue;
            break;

        case PPSSYNC_PPSFREQ:
        	PPSSync->PPSSyncCore->PpsFreq = RegisterValue;
            break;

       	default:
            return -1;
            break;
	}
	return 0;
}

static void PPSSync_ClearCTE(PPSSync_t *PPSSync)
{
	PPSSync_WriteReg(PPSSync, PPSSYNC_CONTROL, 0x3c);
	PPSSync_WriteReg(PPSSync, PPSSYNC_CONTROL, 0x18);
	PPSSync->fCTE = 0.0f;

}

static Result PPSSync_DriveDac(PPSSync_t *PPSSync, float * pfDacValue)
{
    Result res;

    debug("[PPS ] DriveDac %f\n",*pfDacValue);

    if(PPSSync->pfDriveDac == NULL) return PPSSYNCINVFUNCPTR;

	if(*pfDacValue > PPSSync->fDacMax)
	{
		*pfDacValue = PPSSync->fDacMax;
	}
	else if(*pfDacValue < PPSSync->fDacMin)
	{
		*pfDacValue = PPSSync->fDacMin;
	}

    res = PPSSync->pfDriveDac(PPSSync->hBoard, *pfDacValue);
    if(FAILURE(res)) return res;

	return PPSSYNCOK;
}

void PPSSync_Reset(PPSSync_t *PPSSync)
{
    // To stop PPS Sync thread if running
	PPSSync->Start = 0;

	//assert global reset
	PPSSync_WriteReg(PPSSync, PPSSYNC_CONTROL, 0x01);
	//deassert global reset and clear CTE
	PPSSync_WriteReg(PPSSync, PPSSYNC_CONTROL, 0x08);

}

Result PPSSync_PPSDetect(PPSSync_t *PPSSync)
{
	int tickcnt;
	int err = 0;

	err = PPSSync_ReadReg(PPSSync, PPSSYNC_TICKCNT, &tickcnt);
	debug("[PPS ] PPS pulse detecting... %d\n", tickcnt);
	if(err) return err;

	if(!tickcnt) return PPSSYNCPPSABSENT;

	return PPSSYNCOK;
}


Result PPSSync_SaveRefDacVal(float fStableDac)
{
	FILE *fp;
	fp = fopen("StableDAC.conf","w+");
	if(!fp)
	{
		printf("[ERR ] Failed to save stable DAC\n");
		return -1;
	}
	fprintf(fp, "stable_dac=%f",fStableDac);
	fclose(fp);
	return PPSSYNCOK;
}

Result PPSSync_ReadRefDacVal(float * pfStableDac)
{
	FILE *fp;
	char line[80];
    
    // Default DAC value
	*pfStableDac = 0.0f;
    
	fp = fopen("StableDAC.conf","rt");
	if(!fp)
	{
		printf("[ERR ] Failed to open stable DAC file\n");
		return -1;
	}
    
	if(!fgets(line, 80, fp))
	{
		printf("[ERR ] Failed to read stable DAC file\n");
        fclose(fp);
		return -2;
	}
    
	sscanf(line,"stable_dac=%f", pfStableDac);
	fclose(fp);

    return PPSSYNCOK;
	
}

Result PPSSync_ReadPIProfile(float *Kp, float *Ki)
{
	FILE *fp;
	char line[80];
    
    // Default values
	*Kp = 60000.0f;
	*Ki = 4000.0f;

	fp = fopen("StableProfile.conf","rt");
	if(!fp)
	{
		printf("[ERR ] Failed to open stable PI profile\n");
        return -1;
	}

	if(!fgets(line, 80, fp))
    {
		printf("[ERR ] Failed to read PI profile\n");
        fclose(fp);
        return -2;
    }
    sscanf(line,"kp=%f\n", Kp);
    
	if(!fgets(line, 80, fp))
    {
		printf("[ERR ] Failed to read PI profile\n");
        fclose(fp);
        return -3;
    }
    sscanf(line,"ki=%f\n", Ki);
    
	fclose(fp);

    return PPSSYNCOK;
}

Result PPSSync_SavePIProfile(float Kp, float Ki)
{
	FILE *fp;
	fp = fopen("StableProfile.conf","w+");
	if(!fp)
	{
		printf("[ERR ] Failed to save stable PI profile\n");
		return -1;
	}
	fprintf(fp, "kp=%2.3f\n", Kp);
	fprintf(fp, "ki=%2.3f\n", Ki);
	fclose(fp);

	return PPSSYNCOK;
}

Result PPSSync_Open(PPSSync_t **PPSSync, unsigned int PPSSyncBaseAdd,
    PPSSync_pfDriveDac_t pfDriveDac, void * hBoard)
{
	//allocate memory
    *PPSSync = (PPSSync_t *) MALLOC(sizeof(PPSSync_t));

    (*PPSSync)->BaseAdd = PPSSyncBaseAdd;

	(*PPSSync)->pfDriveDac = pfDriveDac;
	(*PPSSync)->hBoard = hBoard;

	(*PPSSync)->PPSSyncCore = (PPSSyncCore_t *) PPSSyncBaseAdd;
	(*PPSSync)->RefFreq = 0;
	(*PPSSync)->Lock = 0;
	(*PPSSync)->LockLost = 0;
	(*PPSSync)->fMaxPPB = 0.0f;
	(*PPSSync)->fMeasuredPPB = 0.0f;
	(*PPSSync)->fCumErr = 0;
	(*PPSSync)->NumGoodClk = 0;
	(*PPSSync)->fCTE = 0.0f;
	(*PPSSync)->Tick = 0;
	(*PPSSync)->BusErr = 0;
	(*PPSSync)->BusTol = 10000;
	(*PPSSync)->Start = 0;
	(*PPSSync)->CTEInterval = CTE_INTERVAL;

    (*PPSSync)->IsDacInit = 0;

	return PPSSYNCOK;

}

Result PPSSync_Close(PPSSync_t *PPSSync)
{
    FREE(PPSSync);
    return PPSSYNCOK;
}

Result PPSSync_Configure(PPSSync_t *PPSSync,
    float fMinDAC, float fMaxDAC, unsigned int uPpsFreq)
{
	if(fMinDAC > fMaxDAC)
		return PPSSYNCPPSBADDACLIMIT;
    
    if(uPpsFreq == 0)
        return PPSSYNCPPSBADFREQ;
    
	PPSSync->fDacMax = fMaxDAC;
	PPSSync->fDacMin = fMinDAC;
    
    PPSSync->PpsFreq = uPpsFreq;
	PPSSync_WriteReg(PPSSync, PPSSYNC_PPSFREQ, uPpsFreq);

	return PPSSYNCOK;
}

Result PPSSync_Presence(PPSSync_t *PPSSync, unsigned int *id, unsigned int *ver)
{
	int value;
	unsigned int core_id, core_ver;


	PPSSync_ReadReg(PPSSync, PPSSYNC_COREIDVERS, &value);
	core_id = (unsigned int)(value & 0xffff0000) >> 16;
	core_ver = (unsigned int)(value & 0xffff);
	*id = core_id;
	*ver = core_ver;

	if((core_id != PERSEUS_PPS_SYNC_COREID) || (core_ver < PERSEUS_PPS_SYNC_COREVER))
	{
		return PPSSYNCCOREABSENT;
	}

    return PPSSYNCOK;
}

Result PPSSync_GetInfo(PPSSync_t *PPSSync, PPSSync_Info_t *PPSSyncInfo)
{
	PPSSyncInfo->TargetFreq = PPSSync->RefFreq;
	PPSSyncInfo->TickPerPPS = PPSSync->Tick;
	PPSSyncInfo->fCTE = PPSSync->fCTE;
	PPSSyncInfo->fDACValue = PPSSync->fCurrDac;
	PPSSyncInfo->Lock = PPSSync->Lock;
	PPSSyncInfo->fPPB = fabsf(PPSSync->fMeasuredPPB*(float)1e9);
	return PPSSYNCOK;

}

Result PPSSync_DacInit(PPSSync_t *PPSSync, float fDacVal)
{
	Result ret = 0;

	PPSSync->fCurrDac = fDacVal;
	ret = PPSSync_DriveDac(PPSSync, &PPSSync->fCurrDac);
    if(FAILURE(ret)) return ret;

    // Set DAC Init Status to true
    PPSSync->IsDacInit = 1;

	return PPSSYNCOK;
}


Result PPSSync_Start(PPSSync_t *PPSSync, unsigned int TargetFreq, float Kp, float Ki, unsigned int IntegrationTime)
{
    if(PPSSync->IsDacInit == 0) return PPSSYNCNOTINIT;

	PPSSync->Lock = 0;
	PPSSync->FineTuning = 0;
	PPSSync->LockLost = 0;
	PPSSync->fMeasuredPPB = 0.0f;
	PPSSync->fCumErr = 0.0f;
	PPSSync->NumGoodClk = 0;
	PPSSync->Tick = 0;
	PPSSync->BusErr = 0;
	PPSSync->Start = 0;
	PPSSync->CTEInterval = IntegrationTime;
	PPSSync->RefFreq = 	TargetFreq;
	PPSSync->Kp	 = Kp;
	PPSSync->Ki = Ki;

	memset(cte_vector, 0, CTE_MAX_INTERVAL);

	//set initial value for DAC
	PPSSync_DriveDac(PPSSync, &PPSSync->fCurrDac);

	//Set init value for integration time
	PPSSync_WriteReg(PPSSync, PPSSYNC_MOVSUMLEN, IntegrationTime-1);


//	printf("[INFO] Freq=%u DAC =%u Kp=%3.3f Ki=%3.3f Interval=%u\n", PPSSync->RefFreq, (unsigned int)PPSSync->fCurrDac, PPSSync->Kp, PPSSync->Ki, PPSSync->CTEInterval);

	//set frequency register
	PPSSync_WriteReg(PPSSync, PPSSYNC_CLKTARGET, PPSSync->RefFreq);

	PPSSync_ClearCTE(PPSSync);

	//initialize PI controller
	PPSSync->Start = 1;
	PPSSync->fMaxPPB = 0.0f;
	PPSSync->fCTE = 0.0f;

	//initial cummulative error
	if(PPSSync->Ki)
	{
		PPSSync->fCumErr = PPSSync->fCurrDac/PPSSync->Ki;
	}

	return PPSSYNCOK;
}

Result PPSSync_Stop(PPSSync_t *PPSSync)
{
	PPSSync->Start = 0;
	return 0;
}

Result PPSSync_Tick(PPSSync_t *PPSSync)
{
	int tick;
    int iIntegratedCTE;

	//get tick count
	PPSSync_ReadReg(PPSSync, PPSSYNC_TICKCNT, &tick);

	PPSSync->Tick = (unsigned int)tick;
	PPSSync->TickErr= PPSSync->RefFreq - tick;

	//avoid FPGA sends wrong data and jitter error
	if(abs(PPSSync->TickErr) > PPSSync->BusTol)
	{
        //printf("[ERR ] Bus error cte=%d tick_err=%d\n", PPSSync->CTE, tick_err);
		//PPSSync->BusErr++;
		PPSSync_ClearCTE(PPSSync);
		return PPSSYNCBUSERROR;
    }

    if(PPSSync->FineTuning)
    {
        PPSSync_ReadReg(PPSSync, PPSSYNC_CTE, &iIntegratedCTE);

        // Calculate the averaged CTE
        PPSSync->fCTE = (float)iIntegratedCTE / (float)PPSSync->CTEInterval;
    }
    else
    {
        PPSSync->fCTE += (float)PPSSync->TickErr;
    }

    //compute PPB
    PPSSync->fMeasuredPPB = PPSSync->fCTE/(float)PPSSync->RefFreq;

    //PI controller
    PPSSync->fCurrDac = (PPSSync->Kp * PPSSync->fMeasuredPPB + PPSSync->Ki * PPSSync->fCumErr)
         * (PPSSync->fDacMax - PPSSync->fDacMin);

    //drives DAC
    PPSSync_DriveDac(PPSSync, &PPSSync->fCurrDac);

    if(PPSSync->FineTuning)
    {
        // Update cumulative error only in fine tuning mode.
        // Since it is not update in coarse tuning mode,
        // the PI controller will act like a P controller
        PPSSync->fCumErr += PPSSync->fMeasuredPPB;
    }

    //VCXO lock detection
    if(fabsf(PPSSync->fMeasuredPPB) <= TARGET_PPB
        || (PPSSync->FineTuning == 0 && fabsf((float)PPSSync->TickErr/(float)PPSSync->RefFreq) <= TARGET_PPB))
    {
        PPSSync->LockLost = 0;
        PPSSync->NumGoodClk++;

        if(PPSSync->FineTuning == 1 && PPSSync->NumGoodClk == MAX_GOOD_CLOCK)
        {
            PPSSync->Lock = 1;
        }

        if(PPSSync->FineTuning == 0 && PPSSync->NumGoodClk > MAX_GOOD_CLOCK/2)
        {
            // Switch to fine tuning mode
            PPSSync->FineTuning = 1;
            PPSSync->NumGoodClk = 0;
            PPSSync_ClearCTE(PPSSync);

            //update cumulative error for fine tuning
            PPSSync->fCumErr = PPSSync->fCurrDac / PPSSync->Ki / (PPSSync->fDacMax - PPSSync->fDacMin);
        }
    }
    else
    {
        //reset good clock counter
        PPSSync->NumGoodClk = 0;

        //track lock lost
        if(PPSSync->Lock) PPSSync->LockLost++;

        if(PPSSync->FineTuning == 1 && PPSSync->LockLost > MAX_BAD_CLOCK)
        {
            // Switch to coarse tuning mode
            PPSSync->Lock = 0;
            PPSSync->FineTuning = 0;
            PPSSync->fMaxPPB = 0.0f;
            PPSSync->fCumErr = PPSSync->fCurrDac / PPSSync->Ki / (PPSSync->fDacMax - PPSSync->fDacMin);
        }
    }

    //monitor maximum clock error
    if(PPSSync->Lock && fabsf(PPSSync->fMeasuredPPB) > PPSSync->fMaxPPB)
    {
        PPSSync->fMaxPPB = fabsf(PPSSync->fMeasuredPPB);
    }

	return PPSSYNCOK;

}


