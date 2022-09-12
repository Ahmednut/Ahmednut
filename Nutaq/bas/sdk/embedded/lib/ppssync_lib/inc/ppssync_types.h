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
 *  @file        ppssync_types.h
 *  @brief       Definitions common to Perseus PPS sync Driver
 *
 *  Author(s)   : Minh Quang Nguyen
 *
 * @ingroup PPSSYNC
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
#ifndef PPSSYNC_TYPES_H
#define PPSSYNC_TYPES_H

#include "bas_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef Result (*PPSSync_pfDriveDac_t)(void * hBoard, float fValue);

typedef enum E_PPSSYNC_REGISTER
{
    PPSSYNC_COREIDVERS = 0,
    PPSSYNC_CLKTARGET = 1,
    PPSSYNC_CONTROL = 2,
    PPSSYNC_TICKCNT = 3,
    PPSSYNC_CTE = 4,
    PPSSYNC_MOVSUMLEN = 5,
    PPSSYNC_PPSFREQ = 6,
}PPSSYNC_REGISTER;


typedef struct s_PPSSyncCore
{
	union
    {
        struct
        {
#ifdef _BIG_ENDIAN
            unsigned CoreID:16,
                     CoreVersion:16;
#else
			unsigned CoreVersion:16,
                     CoreID:16;
#endif
        };
        int Register;
    }Core_ID_Vers;

    union
	{
		struct
		{
#ifdef _BIG_ENDIAN
			unsigned Frequency;
#else
			unsigned Frequency;
#endif
		};
		int Register;
	}ClkTarget;

	union
    {
        struct
        {
#ifdef _BIG_ENDIAN
            unsigned CoreReset:1,
                     ResetCTEUponRead:1,
					 ResetCTE:1,
					 CoreEnable:1,
					 MovSumEnable:1,
					 MovSumReset:1,
 					 Unused: 26;
#else
			unsigned Unused: 26,
					 MovSumReset:1,
					 MovSumEnable:1,
					 CoreEnable:1,
 					 ResetCTE:1,
					 ResetCTEUponRead:1,
                     CoreReset:1;
#endif
        };
        int Register;
    }Control;

    union
	{
		struct
		{
#ifdef _BIG_ENDIAN
			unsigned TickPerPPS;
#else
			unsigned TickPerPPS;
#endif
		};
		int Register;
	}Tick;

    union
	{
		struct
		{
#ifdef _BIG_ENDIAN
			unsigned CumTickErr;
#else
			unsigned CumTickErr;
#endif
		};
		int Register;
	}CTE;

	union
	{
		struct
		{
#ifdef _BIG_ENDIAN
			unsigned ProgDelay;
#else
			unsigned ProgDelay;
#endif
		};
		int Register;
	}MOVSUMLEN;

    unsigned PpsFreq;

}PPSSyncCore_t;

typedef struct PPSSync_s
{
	unsigned int BaseAdd;
    PPSSync_pfDriveDac_t pfDriveDac;
    void * hBoard;
	volatile PPSSyncCore_t *PPSSyncCore;
	unsigned int RefFreq;
	unsigned int PpsFreq;
	float fDacMax;
	float fDacMin;
	float Kp;
	float Ki;
	float fCurrDac;
	float fCumErr;
	float fMaxPPB;
	float fMeasuredPPB;
	float fCTE;
	unsigned int TargetPpb;
	unsigned int Tick;
	unsigned int BusErr;
	unsigned int BusTol;
	unsigned int CTEInterval;
	int TickErr;
	unsigned int NumGoodClk;
	unsigned char Lock;
	unsigned char FineTuning;
	unsigned char LockLost;
	unsigned char IsDacInit;
	volatile int Start;
}PPSSync_t;

typedef struct PPSSync_Info_s
{
	unsigned int TargetFreq;
	unsigned int TickPerPPS;
	float fCTE;
	float fDACValue;
	unsigned int Lock;
	float fPPB;

}PPSSync_Info_t;


#ifdef __cplusplus
}
#endif
#endif //PPSSYNC_TYPES_H
