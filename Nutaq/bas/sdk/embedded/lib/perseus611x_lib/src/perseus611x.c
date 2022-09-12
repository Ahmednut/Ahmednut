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
 * @file       perseus611x.c
 * @brief      Perseus611x carrier related functions definitions.
 *
 * $Date: 2015/11/18 18:30:15 $
 * $Revision: 1.7 $
 *
 * @ingroup PERSEUS611X
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * This file is part of Nutaq's BAS Software Suite.
 *
 * You may at your option receive a license to Nutaq's BAS Software Suite under
 * either the terms of the GNU General Public License (GPL) or the
 * Nutaq Professional License, as explained in the note below.
 *
 * Nutaq's BAS Software Suite may be used under the terms of the GNU General
 * Public License version 3 as published by the Free Software Foundation
 * and appearing in the file LICENSE.GPL included in the packaging of this file.
 *
 * Nutaq's BAS Software Suite is provided AS IS WITHOUT WARRANTY OF
 * ANY KIND; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * NOTE:
 *
 * Using Nutaq's BAS Software Suite under the GPL requires that your work based on
 * Nutaq's BAS Software Suite, if  distributed, must be licensed under the GPL.
 * If you wish to distribute a work based on Nutaq's BAS Software Suite but desire
 * to license it under your own terms, e.g. a closed source license, you may purchase
 * a Nutaq Professional License.
 *
 * The Professional License, gives you -- under certain conditions -- the right to
 * use any license you wish for your work based on Nutaq's BAS Software Suite.
 * For the full terms of the Professional License, please contact the Nutaq sales team.
 *
 ****************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "bas_error.h"
#include "FMCEEprom_eeprom.h"

#include "perseus611x.h"
#include "perseus611x_def.h"
#include "perseus611x_regs.h"
#include "perseus611x_i2c.h"
#include "perseus611x_clkctrl.h"

#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"
#include "carrier_internal_def.h"

#ifdef DEBUG
// Uncomment following line for debug
#define FMCEEPROM_CARRIER611X_DEBUG 1
#endif

//debug or no debug
#ifdef FMCEEPROM_CARRIER611X_DEBUG
#include <stdio.h>
#include <stdlib.h>
    #define FUNC_ENTER()   printf("perseus611x: enter %s\n", __FUNCTION__)
    #define FUNC_EXIT()    printf("perseus611x: exit  %s\n", __FUNCTION__)
    #define PDEBUG(f, ...) printf(f, __VA_ARGS__)
#else
    #define FUNC_ENTER()
    #define FUNC_EXIT()
    #define PDEBUG(f, ...)
#endif

// This is stabilization time when configuring vadj
#define VADJ_TOFF_TIME 10000         // Turn off stabilization time in usec
#define VADJ_TON_TIME  100000        // Turn on stabilization time in usec


#define PERSEUS611X_FMC_MAXCONNECTOR    2                     ///< Maximum number of fmc connector on the carrier

#define PERSEUS611X_FMC_MAXBOARD        2                     ///< Maximum number of board for each fmc slot on the carrier

typedef struct S_Register_Map
{
	union U_regs_coreinfo
	{
		struct
		{
	#ifdef _BIG_ENDIAN
			unsigned int coreid:16, version:16;
	#else
			unsigned int version:16, coreid:16;
	#endif
		}Bit;
		 unsigned int raw;
	}regs_coreinfo;
}Register_Map;

static volatile Register_Map *fpgacorecon0br0 = (Register_Map *)(PERSEUS611X_FMC_BASE_ADDR + 0 * PERSEUS611X_FMC_SIZE);
static volatile Register_Map *fpgacorecon0br1 = (Register_Map *)(PERSEUS611X_FMC_BASE_ADDR + 1 * PERSEUS611X_FMC_SIZE);
static volatile Register_Map *fpgacorecon1br0 = (Register_Map *)(PERSEUS611X_FMC_BASE_ADDR + 2 * PERSEUS611X_FMC_SIZE);
static volatile Register_Map *fpgacorecon1br1 = (Register_Map *)(PERSEUS611X_FMC_BASE_ADDR + 3 * PERSEUS611X_FMC_SIZE);

static Result _Perseus611x_GetBoardPresence(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board, int *presence);
static Result _Perseus611x_GetBoardFpgaCoreId(Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board,unsigned short *core);

// This hold the carrier possible vadj levels
static Carrier_eVadjValueLevels_t Perseus611X_VadjLevels[] = PERSEUS611X_VADJ_VOLTAGE_LEVELS_DEFINITION;
static Carrier_eVadjValue_t Perseus611X_VadjValue[] = PERSEUS611X_VADJ_VALUE_DEFINITION;
static unsigned char Perseus611X_VadjNum = sizeof(Perseus611X_VadjLevels) / sizeof(Carrier_eVadjValueLevels_t);


// Will save the current vadj setting
static Carrier_eVadjValue_t vadjsetting[PERSEUS611X_FMC_MAXCONNECTOR][PERSEUS611X_FMC_MAXBOARD];

// This will hold data lines carrier calibration. For the delta part, the delays are sorted
// LA00-LA33, HA00-HA23, HB00-HB21 (values with 0 are not connected).
// The delay here are estimated value for material DK 4.5 (180 ps/po)
static FMCEEPROM_HPC_DATA_Specs stSignal[PERSEUS611X_FMC_MAXCONNECTOR] =
{
    {
        1,
        {0,0,0},
        {
            578,578,578,578,578,578,578,578,578,578,
            578,578,578,578,578,578,578,578,578,578,
            578,578,578,578,578,578,578,577,578,578,
            578,578,578,578,578,578,578,578,578,578,
            578,578,577,578,578,578,578,577,577,578,
            578,578,578,578,578,578,578,578,578,578,
            578,578,578,579,578,578,577,578,578,578,
            578,579,578,578,578,578,578,578,577,578
        }
    },
    {
        1,
        {0,0,0},
        {
            1088,1088,1088,1088,1088,1088,1088,1088,1088,1087,
            1088,1088,1088,1088,1087,1088,1088,1088,1088,1088,
            1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,
            1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,
            1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,
            1088,1088,1088,1088,1088,1088,1088,1088,1088,1088,
            1088,1088,1088,1088,1088,1088,1088,1088,1088,1087,
            1088,1088,1087,1087,1088,1087,1088,1088,1088,1088
        }
    }
};

Result Perseus611X_Open(Carrier_handle_t * phCarrier, int (*pf_usleep)(unsigned int usec))
{
    Result result=CARRIER_OK;
    Carrier_handle_t hCarrier;

    FUNC_ENTER();

    hCarrier = malloc(sizeof(sCarrier_t));
    if(hCarrier == NULL) return CARRIER_ALLOC_FAILED;
    memset(hCarrier,0,sizeof(sCarrier_t));
    *phCarrier = hCarrier;

    hCarrier->Type = Carrier_ePerseus611X;
    hCarrier->uFmcConnectorNumMax = PERSEUS611X_FMC_MAXCONNECTOR;
    hCarrier->uFmcBoardNumMax = PERSEUS611X_FMC_MAXBOARD;

    // Point to the Perseus611X registers FPGA core base address.
    hCarrier->pCore = (void *)PERSEUS611X_REG_BASE_ADDR;

    hCarrier->pSlot = malloc(PERSEUS611X_FMC_MAXCONNECTOR * sizeof(Carrier_sSlotInfo_t));
    if(hCarrier->pSlot == NULL)
    {
        free(hCarrier);
        return CARRIER_ALLOC_FAILED;
    }
    memset(hCarrier->pSlot,0,PERSEUS611X_FMC_MAXCONNECTOR * sizeof(Carrier_sSlotInfo_t));

    hCarrier->pEepromData = malloc(PERSEUS611X_FMC_MAXCONNECTOR * sizeof(FMCEEPROM_EEprom_Decode));
    if(hCarrier->pEepromData == NULL)
    {
        free(hCarrier->pSlot);
        free(hCarrier);
        return CARRIER_ALLOC_FAILED;
    }
    memset(hCarrier->pEepromData,0,PERSEUS611X_FMC_MAXCONNECTOR * sizeof(FMCEEPROM_EEprom_Decode));

    hCarrier->pEepromList = malloc(PERSEUS611X_FMC_MAXCONNECTOR * sizeof(FMCEEprom_eepromdef));
    if(hCarrier->pEepromList == NULL)
    {
        free(hCarrier->pEepromData);
        free(hCarrier->pSlot);
        free(hCarrier);
        return CARRIER_ALLOC_FAILED;
    }
    memset(hCarrier->pEepromList,0,PERSEUS611X_FMC_MAXCONNECTOR * sizeof(FMCEEprom_eepromdef));

    hCarrier->pfCarrier_Close                       = Perseus611X_Close;

    hCarrier->pfCarrier_Reboot                      = Perseus611X_Reboot;
    hCarrier->pfCarrier_FmcSetVadj                  = Perseus611X_FmcSetVadj;
    hCarrier->pfCarrier_FmcAssignSlot               = Perseus611X_FmcAssignSlot;

    hCarrier->pfCarrier_FmcGetI2cZone               = Perseus611X_FmcGetI2cZone;
    hCarrier->pfCarrier_I2cRead                     = Perseus611X_I2cRead;
    hCarrier->pfCarrier_I2cWrite                    = Perseus611X_I2cWrite;
    hCarrier->pfCarrier_I2cReadNoWrData             = Perseus611X_I2cReadNoWrData;
    hCarrier->pfCarrier_I2cCheckRaw                 = Perseus611X_I2cCheckRaw;
    
    hCarrier->pfCarrier_GetSignalsInfo              = Perseus611X_GetSignalsInfo;

    hCarrier->pfCarrier_usleep                      = pf_usleep;
    hCarrier->pfCarrier_FmcGetGA                    = Perseus611X_FmcGetGA;
    hCarrier->pfCarrier_TClkSetState                = Perseus611X_TClkSetState;
    hCarrier->pfCarrier_TClkGetState                = Perseus611X_TClkGetState;
    hCarrier->pfCarrier_TClkSetTxSource             = Perseus611X_TClkSetTxSource;
    hCarrier->pfCarrier_TClkGetTxSource             = Perseus611X_TClkGetTxSource;
    hCarrier->pfCarrier_FmcBidirClkSetConnection    = Perseus611X_FmcBidirClkSetConnection;
    hCarrier->pfCarrier_FmcBidirClkGetConnection    = Perseus611X_FmcBidirClkGetConnection;
    hCarrier->pfCarrier_SetOscState                 = Perseus611X_SetOscState;
    hCarrier->pfCarrier_GetOscState                 = Perseus611X_GetOscState;
    hCarrier->pfCarrier_FClkASetConnection          = Perseus611X_FClkASetConnection;
    hCarrier->pfCarrier_FClkAGetConnection          = Perseus611X_FClkAGetConnection;
    hCarrier->pfCarrier_GetModuleBaseAddr           = Perseus611X_GetModuleBaseAddr;
    hCarrier->pfCarrier_GetModuleSize               = Perseus611X_GetModuleSize;
    hCarrier->pfCarrier_GetModuleMaxNumber          = Perseus611X_GetModuleMaxNumber;
    hCarrier->pfCarrier_GetMemSpaceBaseAddr         = Perseus611X_GetMemSpaceBaseAddr;
    hCarrier->pfCarrier_GetMemSpaceSize             = Perseus611X_GetMemSpaceSize;
    hCarrier->pfCarrier_CustomRegisterWrite         = Perseus611X_CustomRegisterWrite;
    hCarrier->pfCarrier_CustomRegisterRead          = Perseus611X_CustomRegisterRead;
    hCarrier->pfCarrier_LedSetValue                 = Perseus611X_LedSetValue;
    hCarrier->pfCarrier_LedGetValue                 = Perseus611X_LedGetValue;

    hCarrier->bInit = 1;

    // Get fpga core information associated with each module types
    FMCEEprom_eeprom_get_fpgacore_info(&(hCarrier->pCoreList), &(hCarrier->uCoreNum));
    // Get fpga core name information associated with each module types
    FMCEEprom_eeprom_get_fpgacorename_info(&(hCarrier->ppcCoreNameList), &(hCarrier->uCoreNameNum));
    // Get fpga core name information associated with each module types
    FMCEEprom_eeprom_get_modulename_info(&(hCarrier->ppcModuleNameList), &(hCarrier->uModuleNameNum));

    FUNC_EXIT();
    return result;
}

Result Perseus611X_FmcGetI2cZone(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, unsigned int * I2cZone)
{
    Result ret = CARRIER_OK;
    FUNC_ENTER();
    
    if (hCarrier == NULL)
    {
        ret = CARRIER_NOT_OPEN;
        return ret;
    }

    if(connector == Carrier_eFmcConnector0)
        *I2cZone = (unsigned int)Perseus611x_eI2cZoneFmc1;
    else if(connector == Carrier_eFmcConnector1)
        *I2cZone = (unsigned int)Perseus611x_eI2cZoneFmc2;
    else
        ret = CARRIER_INVALID_CONNECTOR;
        
    FUNC_EXIT();
    return ret;
}

Result Perseus611X_Close(Carrier_handle_t hCarrier)
{
    Result result=CARRIER_OK;

    FUNC_ENTER();

    if(hCarrier == NULL)
    {
        result = CARRIER_NOT_OPEN;
        FUNC_EXIT();
        return result;
    }

    if(hCarrier->pSlot)
        free(hCarrier->pSlot);

    if(hCarrier->pEepromData)
        free(hCarrier->pEepromData);

    if(hCarrier->pEepromList)
        free(hCarrier->pEepromList);

    if(hCarrier)
        free(hCarrier);

    FUNC_EXIT();
    return result;
}

Result Perseus611X_FmcAssignSlot(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector)
{
    FMCEEPROM_EEprom_Decode * epdata;
    Carrier_sSlotInfo_t * slot;

    Result result=CARRIER_OK, result1;
    Carrier_eFmcBoardNum_t boardcount;
    int presence, searched;
    FMCEEprom_dualmodule_compatibility *complist;
    unsigned int nitems;
    unsigned char modused[FMCEEPROM_EEPROM_MAX];  ///< Hold identified modules index that are assigned or not
    unsigned int useditem = 0, loopcomp, boardbyslot, ncores;
    FMCEEprom_module_fpgacoreids *corelist;
    unsigned int vadjnom,vadjmin,vadjmax;
    int presencesim[Carrier_eFmcSlotLast][PERSEUS611X_FMC_MAXBOARD];
    Carrier_eVadjValue_t testvadjreq[PERSEUS611X_FMC_MAXBOARD];

    FUNC_ENTER();

    if (connector >= hCarrier->uFmcConnectorNumMax || connector < Carrier_eFmcConnector0 )
    {
        result = CARRIER_INVALID_CONNECTOR;
        FUNC_EXIT();
        return result;
    }

    epdata = &hCarrier->pEepromData[connector];
    slot = &hCarrier->pSlot[connector];

    // Erase the eeprom discovery structure
    memset(slot,0,sizeof(Carrier_sSlotInfo_t));

    // Make sure identified eeprom index struct point to nothing valid
    memset(slot->module_identindex,255,sizeof(slot->module_identindex));

    // Erase presence simulation vector
    memset(presencesim,0,sizeof(presencesim));

    // Erase the module used structure
    memset(modused,0,sizeof(modused));

    // Get module pairs compatibility list information
    FMCEEprom_eeprom_get_complist_info(&complist, &nitems);

    // Get fpga core information associated with each module types
    FMCEEprom_eeprom_get_fpgacore_info(&corelist, &ncores);

    // Care must be taken for fmcradio modules which does not use GA the same way on all
    // modules revision, also problems with third party modules that could used a variant of
    // 2402 eeproms that would not allow to react correctly to GA bits according to slot.
    // This algo won't use GA information for the slot because perseus611x does not need this...

    // Because fmc radio modules does not have a hardware top detection signal, simulates a module
    // presence in top position if the exact number of eeprom where read (even blanks and unidentified ones) to match number of boards in a slot
    // !!! M.T. WARNING: this patch while could work for this carrier type, may cause
    // problem with a multiple slots board carrier later, so take care if code reuse (and modify the code)
    if (epdata->neepromrd == PERSEUS611X_FMC_MAXBOARD)
        // If enough eeprom detected to fill the whole slot of boards, simulate presence on top board
        presencesim[Carrier_eFmcSlot0][Carrier_eFmcBoard1] = 1;

    // Analyze each board in the current slot
    for (boardcount = (PERSEUS611X_FMC_MAXBOARD-1), boardbyslot = 0; boardcount >= Carrier_eFmcBoard0 && boardcount < PERSEUS611X_FMC_MAXBOARD; boardcount--)
    {
        // Default vAdj req is unknown
        slot->vadjreq[connector][boardcount] = VADJ_UNKNOWN;

        // Get fpga core id for the board slot
        result1 = _Perseus611x_GetBoardFpgaCoreId(connector, boardcount, (unsigned short *)(&slot->fpgacoretype[Carrier_eFmcSlot0][boardcount]));
        if (SUCCESS(result))
            result = result1;

        // Check if there is a detected module in the board position
        result1 = _Perseus611x_GetBoardPresence(hCarrier, connector, boardcount, &presence);
        if (SUCCESS(result))
            result = result1;

        // If no module detected at this board and this slot position?
        if (!presence && !presencesim[Carrier_eFmcSlot0][boardcount])
            slot->modstate[Carrier_eFmcSlot0][boardcount] = CARRIER_MODULE_UNDETECTED;
        else
        {
            // By default set this module to not identified
            slot->modstate[Carrier_eFmcSlot0][boardcount] = CARRIER_MODULE_UNIDENTIFIED;

            // Are there still identified eeproms not assigned to board slot?
            if (useditem < epdata->nmodule_identified)
            {
                // For top board
                if (boardcount == Carrier_eFmcBoard1)
                {
                    // Check for all modules that can go in the top position

                    // Loop through identified modules eeproms
                    for (searched = 0; searched < epdata->nmodule_identified; searched++)
                    {
                        // If this module identified eeprom already used
                        if (!modused[searched])
                        {
                            // Loop through all possibles top modules from compatibility list
                            for (loopcomp = 0; loopcomp < nitems; loopcomp++)
                            {
                                // Check if the identified eeprom match one of top module in the list
                                if (epdata->modtype[searched] == complist[loopcomp].topmodule)
                                {
                                    // Yes found a top identified module
                                    slot->modstate[Carrier_eFmcSlot0][boardcount] = CARRIER_VALID;
                                    slot->boardtype[Carrier_eFmcSlot0][boardcount] = epdata->modtype[searched];
                                    slot->module_identindex[Carrier_eFmcSlot0][boardcount] = searched;  // Save identified eeprom index
                                    modused[searched] = 1;     // Mark this identified eeprom as used

                                    // !!! M.T. This vadj code does not deal with possible double width modules
                                    // which could have 2 connectors and different vAdj for each. Persesu611x
                                    // does have a single fmc slot and missing time to implement that feature
                                    // to support more carrier generic
                                    result1 = FMCEEprom_eeprom_get_vadj(epdata,  FMCEEPROM_EEPROM_CONNP1,
                                              searched, &vadjnom, &vadjmin,&vadjmax);
                                    if (FAILURE(result1)  && result1 != FMCEEPROM_ERR_UNKNOWN_MODULETYPE)
                                        if (SUCCESS(result))
                                            result = result1;
                                    // Check if the carrier can support the vadj module requiered value
                                    result1 = Perseus611X_FmcGetVadj(connector, boardcount, vadjnom, vadjmin,
                                                                        vadjmax, &slot->vadjreq[Carrier_eFmcSlot0][boardcount]);

                                    if (SUCCESS(result))
                                        result = result1;

                                    useditem++;                // One more identified eeprom used
                                    searched = epdata->nmodule_identified; // Break other loop also
                                    boardbyslot++;
                                    break;
                                }
                            }
                        }
                    }
                }
                else
                {
                    // For bottom board...
                    // Loop through identified modules eeproms still available...
                    for (searched = 0; searched < epdata->nmodule_identified && useditem < epdata->nmodule_identified; searched++)
                    {
                        // If this module identified eeprom already used
                        if (!modused[searched])
                        {
                            // Yes found a bottom identified module
                            slot->modstate[Carrier_eFmcSlot0][boardcount] = CARRIER_VALID;
                            slot->boardtype[Carrier_eFmcSlot0][boardcount] = epdata->modtype[searched];
                            slot->module_identindex[Carrier_eFmcSlot0][boardcount] = searched;  // Save identified eeprom index
                            modused[searched] = 1;     // Mark this identified eeprom as used

                            // !!! M.T. This vadj code does not deal with possible double width modules
                            // which could have 2 connectors and different vAdj for each. Persesu611x
                            // does have a single fmc slot and missing time to implement that feature
                            result1 = FMCEEprom_eeprom_get_vadj(epdata,  FMCEEPROM_EEPROM_CONNP1,
                                      searched, &vadjnom, &vadjmin,&vadjmax);
                            if (FAILURE(result1)  && result1 != FMCEEPROM_ERR_UNKNOWN_MODULETYPE)
                                if (SUCCESS(result))
                                    result = result1;
                            // Check if the carrier can support the vadj module requiered value
                            result1 = Perseus611X_FmcGetVadj(connector, boardcount, vadjnom, vadjmin,
                                                                vadjmax, &slot->vadjreq[Carrier_eFmcSlot0][boardcount]);

                            if (SUCCESS(result))
                                result = result1;

                            useditem++;                // One more identified eeprom used
                            boardbyslot++;
                            break;
                        }
                    }
                }
            }
        }
        // Check if the fpga core match with an identified module
        if(slot->modstate[Carrier_eFmcSlot0][boardcount] == CARRIER_VALID)
        {
            if (corelist[slot->boardtype[Carrier_eFmcSlot0][boardcount]].fpgacore != slot->fpgacoretype[Carrier_eFmcSlot0][boardcount])
            {
                // Fpga core does not match with board type, we have a problem
                slot->modstate[Carrier_eFmcSlot0][boardcount] = CARRIER_WRONGFPGA_CORE;
                boardbyslot--;
            }
            else
            {
                // Check if the vadj requierements are possible for current board on the carrier
                // This is a safety check, normally it should be possible if it was identified (this check is more
                // important with dealing with third party adapter)
                if (slot->vadjreq[Carrier_eFmcSlot0][boardcount] == VADJ_UNKNOWN)
                {
                    // Unsupported vadj requierement
                    slot->modstate[Carrier_eFmcSlot0][boardcount] = CARRIER_UNSUPPORTED_VADJ;
                    boardbyslot--;
                }
            }
        }
    }

    // Check if we could have an identified module pair which is not compatible
    if (boardbyslot == PERSEUS611X_FMC_MAXBOARD)
    {
        // Loop through all possibles modules pairs from compatibility list for a match
        for (loopcomp = 0; loopcomp < nitems; loopcomp++)
        {
            // Check if the identified eeprom match one of top module in the list
            if (slot->boardtype[Carrier_eFmcSlot0][Carrier_eFmcBoard0] == complist[loopcomp].bottommodule &&
                slot->boardtype[Carrier_eFmcSlot0][Carrier_eFmcBoard1] == complist[loopcomp].topmodule)
            {
                // Yes found a supported match pair
                break;
            }
        }
        // Did we find a compatible pair?
        if  (loopcomp >= nitems)
        {
            // We did not find a compatible pair, mark the modules as non compatible
            slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard0] = CARRIER_UNMATCH_MODULES_SLOT;
            slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard1] = CARRIER_UNMATCH_MODULES_SLOT;
        }

        // Verifies for incompatible or not supported vAdj voltage: this is a safety measure
        // in case there is a match in the compatibility table of 2 boards in the slot without the same
        // vadj, which is impossible on a perseus611x
        if (slot->vadjreq[Carrier_eFmcSlot0][Carrier_eFmcBoard0] != slot->vadjreq[Carrier_eFmcSlot0][Carrier_eFmcBoard1])
        {
            // Both voltage does not match, log the error condition
            slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard0] = CARRIER_UNMATCH_VADJ;
            slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard1] = CARRIER_UNMATCH_VADJ;
        }
    }
    else
    {
        // Case where 0 or 1 valid module was detected, if there is still a pair of modules
        if (slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard0] != CARRIER_MODULE_UNDETECTED &&
            slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard1] != CARRIER_MODULE_UNDETECTED)
        {
            testvadjreq[Carrier_eFmcBoard0] = slot->vadjreq[Carrier_eFmcSlot0][Carrier_eFmcBoard0];
            testvadjreq[Carrier_eFmcBoard1] = slot->vadjreq[Carrier_eFmcSlot0][Carrier_eFmcBoard1];

            if (slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard0] == CARRIER_MODULE_UNIDENTIFIED)
            {
                testvadjreq[Carrier_eFmcBoard0] = VADJ_2_503V;
            }

            if (slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard1] == CARRIER_MODULE_UNIDENTIFIED)
            {
                testvadjreq[Carrier_eFmcBoard1] = VADJ_2_503V;
            }

            // Verifies for incompatible or not supported vAdj voltage: this is a safety measure
            // in case there is a match in the compatibility table of 2 boards in the slot without the same
            // vadj, which is impossible on a perseus611x
            if (testvadjreq[Carrier_eFmcBoard0] != testvadjreq[Carrier_eFmcBoard1])
            {
                // Both voltage does not match, log the error condition
                slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard0] = CARRIER_UNMATCH_VADJ;
                slot->modstate[Carrier_eFmcSlot0][Carrier_eFmcBoard1] = CARRIER_UNMATCH_VADJ;
            }
        }
    }

    FUNC_EXIT();
    return result;
}

Result Perseus611X_FmcSetVadj(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board, Carrier_eVadjValue_t Value)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;

    Result ret=CARRIER_OK;
    static unsigned vadjs = 0;
    Carrier_eFmcBoardNum_t other;
    Carrier_eVadjValue_t othersetting;
    unsigned char index, found;
    unsigned int VadjSelTmp, VadjEnTmp;

    FUNC_ENTER();

    // Check if it is the 1st time we run this routine
    if(!vadjs)
    {
        // Yes, initialize to vadj disable
        for (vadjs = 0; vadjs < hCarrier->uFmcConnectorNumMax; vadjs++)
            for (other = 0; other < hCarrier->uFmcBoardNumMax; other++)
                vadjsetting[vadjs][other] = VADJ_DISABLE;
        vadjs = 1;
    }

    // Verify that the Vadj value is valid for the Perseus611X
    found = 0;
    for(index=0; index<Perseus611X_VadjNum; index++)
    {
        if (Value == Perseus611X_VadjValue[index])
        {
            found = 1;
            break;
        }
    }

    if(found == 0)
    {
        ret = CARRIER_INVALID_VADJSETTING;
        FUNC_EXIT();
        return(ret);
    }

    // Check for valid connector (perseus611x has 2 FMC connector)
    if (connector >= hCarrier->uFmcConnectorNumMax || connector < Carrier_eFmcConnector0 )
    {
        ret = CARRIER_INVALID_CONNECTOR;
        FUNC_EXIT();
        return(ret);
    }

    // There is a single connector on the perseus 611x, so not necessary to check for any other fmc connector
    switch(board)
    {
        case Carrier_eFmcBoard0:
        case Carrier_eFmcBoard1:

            // Check other boards in the connector to see if they have already set for another vadj
            for (other = 0, othersetting = VADJ_DISABLE; other < PERSEUS611X_FMC_MAXBOARD; other++)
            {
                // Do not check or own board
                if (other != board)
                {
                    if (vadjsetting[connector][other] >= VADJ_FIRST && vadjsetting[connector][other] < VADJ_DISABLE)
                        othersetting = vadjsetting[connector][other];
                }
            }

            // Check if vadj is already configured for the connector and the same
            if (othersetting != Value)
            {
                if (othersetting != VADJ_DISABLE && Value != VADJ_DISABLE && Value != VADJ_UNKNOWN)
                {
                    // Impossible to set a different vadj in the same connector between different board on perseus611x
                    ret = CARRIER_INVALID_VADJREQCONNECTOR;
                }
                else
                {
                    // Configure the vAdj voltage
                    switch(Value)
                    {
                        case VADJ_1_192V:
                            VadjSelTmp = 0;
                            VadjEnTmp = 1;
                            vadjsetting[connector][board] = Value;
                            break;

                        case VADJ_1_490V:
                            VadjSelTmp = 1;
                            VadjEnTmp = 1;
                            vadjsetting[connector][board] = Value;
                            break;

                        case VADJ_1_803V:
                            VadjSelTmp = 2;
                            VadjEnTmp = 1;
                            vadjsetting[connector][board] = Value;
                            break;

                        case VADJ_2_503V:
                            VadjSelTmp = 3;
                            VadjEnTmp = 1;
                            vadjsetting[connector][board] = Value;
                            break;

                        default:
                            VadjSelTmp = 0;
                            VadjEnTmp = 0;
                            vadjsetting[connector][board] = VADJ_DISABLE;
                            break;
                    }


                    // Disable vadj before changing it
                    if(connector == Carrier_eFmcConnector0)
                    {
                        pCore->stVadjResetCtrl.stBit.m_uVadj1En = 0;
                    }
                    else if(connector == Carrier_eFmcConnector1)
                    {
                        pCore->stVadjResetCtrl.stBit.m_uVadj2En = 0;
                    }

                    // Wait small delay
                    ret = Carrier_usleep(hCarrier, VADJ_TOFF_TIME);
                    if(FAILURE(ret))
                    {
                        FUNC_EXIT();
                        return(ret);
                    }

                    //turn on the selected vadj
                    if(connector == Carrier_eFmcConnector0)
                    {
                        pCore->stVadjResetCtrl.stBit.m_uVadj1Sel = VadjSelTmp;
                        pCore->stVadjResetCtrl.stBit.m_uVadj1En = VadjEnTmp;
                    }
                    else if(connector == Carrier_eFmcConnector1)
                    {
                        pCore->stVadjResetCtrl.stBit.m_uVadj2Sel = VadjSelTmp;
                        pCore->stVadjResetCtrl.stBit.m_uVadj2En = VadjEnTmp;
                    }

                    // Wait small delay for voltage stabilization
                    ret = Carrier_usleep(hCarrier, VADJ_TON_TIME);
                    if(FAILURE(ret))
                    {
                        FUNC_EXIT();
                        return(ret);
                    }
                }
            }
            else
                // Connector already the same, do nothing else and no error
                vadjsetting[connector][board] = Value;
        break;

        default:
            // Invalid board specified
            ret = CARRIER_INVALID_BOARD;
    }

    FUNC_EXIT();
    return(ret);
}


Result Perseus611X_GetSignalsInfo(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, FMCEEPROM_HPC_DATA_Specs **ppSignal)
{
    Result ret=CARRIER_OK;

    FUNC_ENTER();

    // Check if slot number is valid
    if (connector >= hCarrier->uFmcConnectorNumMax || connector < 0) {FUNC_EXIT();return(CARRIER_INVALID_CONNECTOR);}

    // Check if output struct pointer is valid
    if (ppSignal == 0)
    {
        ret = CARRIER_INVALID_SIGNALPTR;
    }
    else
    {
        // Returns a pointer on the internal signal structure
        *ppSignal = &stSignal[connector];
    }

    FUNC_EXIT();

    return(ret);
}


/****************************************************************************
 * Function: _Perseus611x_GetBoardPresence
 ************************************************************************//**
 *
 * This function assign to each connector of the perseus 611x carrier, the detected fmc
 * modules.
 *
 * @param [in] connector
 *    Connector number for verification (perseus 611x has only 1 connector)
 *
 * @param [in] board
 *    Which board in the connector to verify
 *
 * @param [out] presence
 *    Return != 0 if a board was detected at this position
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
static Result _Perseus611x_GetBoardPresence(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board, int *presence)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;
    Result ret=CARRIER_OK;

    // Default to not present
    *presence = 0;

    if (connector >= PERSEUS611X_FMC_MAXCONNECTOR || connector < Carrier_eFmcConnector0 )
       ret = CARRIER_INVALID_CONNECTOR;
    else
    {
        // There is a single connector on the perseus 611x, so not necessary to check for any other fmc connector
        switch(board)
        {
            case Carrier_eFmcBoard0:
                // A detected module return 0 on this carrier signal
                if(connector == Carrier_eFmcConnector0)
                {
                    *presence = pCore->stCtrlStat.stBit.m_uFmc1Absent ? 0 : 1;
                }
                else if(connector == Carrier_eFmcConnector1)
                {
                    *presence = pCore->stCtrlStat.stBit.m_uFmc2Absent ? 0 : 1;
                }
            break;

            case Carrier_eFmcBoard1:
                // A detected module return 0 on this carrier signal
                // Because some fpga cores do not report this bit correctly, always report top
                // board absent for this carrier. Rely instead on the number of eeprom to simulate
                // top board presence (fmc radio does not have this feature anyway)
                *presence = 0;
            break;

            default:
                // Invalid board specified
                ret = CARRIER_INVALID_BOARD;
        }
    }

    return(ret);
}


/****************************************************************************
 * Function: _Perseus611x_GetBoardFpgaCoreId
 ************************************************************************//**
 *
 * This function assign to each slot of the perseus 611x carrier, the detected fpga
 * core id.
 *
 * @param [in] connector
 *    Connector number for verification (perseus 611x has only 1 connector)
 *
 * @param [in] board
 *    Which board in the connector to verify
 *
 * @param [out] core
 *    Fpga core id for this board slot
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
static Result _Perseus611x_GetBoardFpgaCoreId(Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board,
    unsigned short *core)
{
    Result ret=CARRIER_OK;

    // Default to no core
    *core = 0;

    switch(connector)
    {
        case Carrier_eFmcConnector0:
            // There is a single connector on the perseus 611x, so not necessary to check for any other fmc connector
            switch(board)
            {
                case Carrier_eFmcBoard0:
                    // Read connector 0 board 0 fpga core id
                    *core = fpgacorecon0br0->regs_coreinfo.Bit.coreid;
                break;

                case Carrier_eFmcBoard1:
                    // Read connector 0 board 1 fpga core id
                    *core = fpgacorecon0br1->regs_coreinfo.Bit.coreid;
                break;

                default:
                    // Invalid board specified
                    ret = CARRIER_INVALID_BOARD;
            }
            break;

        case Carrier_eFmcConnector1:
            // There is a single connector on the perseus 611x, so not necessary to check for any other fmc connector
            switch(board)
            {
                case Carrier_eFmcBoard0:
                    // Read connector 0 board 0 fpga core id
                    *core = fpgacorecon1br0->regs_coreinfo.Bit.coreid;
                break;

                case Carrier_eFmcBoard1:
                    // Read connector 0 board 1 fpga core id
                    *core = fpgacorecon1br1->regs_coreinfo.Bit.coreid;
                break;

                default:
                    // Invalid board specified
                    ret = CARRIER_INVALID_BOARD;
            }
            break;

        default:
            // Invalid board specified
            ret = CARRIER_INVALID_CONNECTOR;
    }

    return(ret);
}


Result Perseus611X_FmcGetVadj(Carrier_eFmcConnectorNum_t connector, Carrier_eFmcBoardNum_t board,
    unsigned vadjnom, unsigned vadjmin, unsigned vadjmax, Carrier_eVadjValue_t *pVadjValue)
{
    Result ret=CARRIER_OK;
    int loopvadj;
    int delta, ldelta = 10000000, index=0;

    // Default to not compatible pVadjValue
    *pVadjValue = VADJ_UNKNOWN;

    // Check if vAdj requierements are valid
    if (vadjmin >  vadjnom || vadjmin > vadjmax || vadjmax < vadjnom)
       ret = CARRIER_INVALID_VADJSETTING;
    else
    {
        if (connector >= PERSEUS611X_FMC_MAXCONNECTOR || connector < Carrier_eFmcConnector0 )
           ret = CARRIER_INVALID_CONNECTOR;
        else
        {
            // There is a single connector on the perseus 611x, so not necessary to check for any other fmc connector
            switch(board)
            {
                case Carrier_eFmcBoard0:
                case Carrier_eFmcBoard1:
                    // All boards in the connector have the same vadj voltage

                    // Check which vadj carrier compatible value is closer to the vadj nominal requierements
                    for (loopvadj = VADJ_FIRST; loopvadj < VADJ_UNKNOWN && loopvadj < Perseus611X_VadjNum; loopvadj++)
                    {
                        delta = vadjnom-Perseus611X_VadjLevels[loopvadj];

                        // Keep the absolue value of the difference
                        if (delta < 0)
                            delta = 0 - delta;

                        // Smallest difference so far?
                        if (delta < ldelta)
                        {
                            // Yes save the value as closest one
                            ldelta = delta;
                            index = loopvadj;
                        }
                    }

                    // Check if the closest vadj carrier value is within vadj range requierements
                    if ((unsigned)Perseus611X_VadjLevels[index] >= vadjmin && (unsigned)Perseus611X_VadjLevels[index] <= vadjmax)
                    {
                        // The carrier vadj is withing vadj requierements range and closer to nominal value, save it
                        *pVadjValue = Perseus611X_VadjValue[index];
                    }
                break;

                default:
                    // Invalid board specified
                    ret = CARRIER_INVALID_BOARD;
            }
        }
    }

    return(ret);
}

Result Perseus611X_FmcGetGA(Carrier_eFmcConnectorNum_t connector, unsigned char * ga)
{
    if (connector >= PERSEUS611X_FMC_MAXCONNECTOR || connector < Carrier_eFmcConnector0 )
        return CARRIER_INVALID_CONNECTOR;

    if(ga == NULL)
        return CARRIER_INVALID_PTR;

    if(connector == Carrier_eFmcConnector0)
    {
        *ga = 0;
    }
    else if(connector == Carrier_eFmcConnector1)
    {
        *ga = 1;
    }

    return CARRIER_OK;
}


Result Perseus611X_GetModuleBaseAddr(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int index, unsigned int * addr)
{
    Result res;
    unsigned int maxindex;

    res = Perseus611X_GetModuleMaxNumber(hCarrier, module, &maxindex);
    if(FAILURE(res)) return res;

    if(index >= maxindex) return CARRIER_INVALID_MODULE_INDEX;

    switch(module)
    {
        case Carrier_eModuleRTDExGigE:
            *addr = PERSEUS611X_RTDEX_GIGE_BASE_ADDR + (index * PERSEUS611X_RTDEX_GIGE_SIZE);
            break;
        case Carrier_eModuleRecPlay:
            *addr = PERSEUS611X_RECPLAY_BASE_ADDR + (index * PERSEUS611X_RECPLAY_SIZE);
            break;
        case Carrier_eModuleI2C:
            *addr = PERSEUS611X_I2C_BASE_ADDR + (index * PERSEUS611X_I2C_SIZE);
            break;
        case Carrier_eModuleDdr3I2C:
            *addr = PERSEUS611X_DDR3_I2C_BASE_ADDR + (index * PERSEUS611X_DDR3_I2C_SIZE);
            break;
        case Carrier_eModuleFMC:
            *addr = PERSEUS611X_FMC_BASE_ADDR + (index * PERSEUS611X_FMC_SIZE);
            break;
        case Carrier_eModuleCarrier:
            *addr = PERSEUS611X_REG_BASE_ADDR + (index * PERSEUS611X_REG_SIZE);
            break;
        case Carrier_eModulePpsSync:
            *addr = PERSEUS611X_PPSSYNC_BASE_ADDR + (index * PERSEUS611X_PPSSYNC_SIZE);
            break;
        case Carrier_eModuleLVDS:
            *addr = PERSEUS611X_LVDS_BASE_ADDR + (index * PERSEUS611X_LVDS_SIZE);
            break;
        case Carrier_eModuleAurora:
            *addr = PERSEUS611X_AURORA_BASE_ADDR + (index * PERSEUS611X_AURORA_SIZE);
            break;
        case Carrier_eModuleRTDExSync:
            *addr = PERSEUS611X_RTDEXSYNC_BASE_ADDR + (index * PERSEUS611X_RTDEXSYNC_SIZE);
            break;
        case Carrier_eModuleTimestamp:
            *addr = PERSEUS611X_TIMESTAMP_BASE_ADDR + (index * PERSEUS611X_TIMESTAMP_SIZE);
            break;
        case Carrier_eModuleDPRAM:
            *addr = PERSEUS611X_DPRAM_BASE_ADDR + (index * PERSEUS611X_DPRAM_SIZE);
            break;
        case Carrier_eModuleSysMon:
            *addr = PERSEUS611X_SYSMON_BASE_ADDR + (index * PERSEUS611X_SYSMON_SIZE);
            break;
        default:
            return CARRIER_INVALID_MODULE;
    }

    return CARRIER_OK;
}

Result Perseus611X_GetModuleSize(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * size)
{
    switch(module)
    {
        case Carrier_eModuleRTDExGigE:
            *size = PERSEUS611X_RTDEX_GIGE_SIZE;
            break;
        case Carrier_eModuleRecPlay:
            *size = PERSEUS611X_RECPLAY_SIZE;
            break;
        case Carrier_eModuleI2C:
            *size = PERSEUS611X_I2C_SIZE;
            break;
        case Carrier_eModuleDdr3I2C:
            *size = PERSEUS611X_DDR3_I2C_SIZE;
            break;
        case Carrier_eModuleFMC:
            *size = PERSEUS611X_FMC_SIZE;
            break;
        case Carrier_eModuleCarrier:
            *size = PERSEUS611X_REG_SIZE;
            break;
        case Carrier_eModulePpsSync:
            *size = PERSEUS611X_PPSSYNC_SIZE;
            break;
        case Carrier_eModuleLVDS:
            *size = PERSEUS611X_LVDS_SIZE;
            break;
        case Carrier_eModuleAurora:
            *size = PERSEUS611X_AURORA_SIZE;
            break;
        case Carrier_eModuleRTDExSync:
            *size = PERSEUS611X_RTDEXSYNC_SIZE;
            break;
        case Carrier_eModuleTimestamp:
            *size = PERSEUS611X_TIMESTAMP_SIZE;
            break;
        case Carrier_eModuleDPRAM:
            *size = PERSEUS611X_DPRAM_SIZE;
            break;
        case Carrier_eModuleSysMon:
            *size = PERSEUS611X_SYSMON_SIZE;
            break;
        default:
            return CARRIER_INVALID_MODULE;
    }

    return CARRIER_OK;
}

Result Perseus611X_GetModuleMaxNumber(Carrier_handle_t hCarrier, Carrier_eModule_t module, unsigned int * num)
{
    switch(module)
    {
        case Carrier_eModuleRTDExGigE:
            *num = PERSEUS611X_RTDEX_GIGE_MAX_NUM;
            break;
        case Carrier_eModuleRecPlay:
            *num = PERSEUS611X_RECPLAY_MAX_NUM;
            break;
        case Carrier_eModuleI2C:
            *num = PERSEUS611X_I2C_MAX_NUM;
            break;
        case Carrier_eModuleDdr3I2C:
            *num = PERSEUS611X_DDR3_I2C_MAX_NUM;
            break;
        case Carrier_eModuleFMC:
            *num = PERSEUS611X_FMC_MAX_NUM;
            break;
        case Carrier_eModuleCarrier:
            *num = PERSEUS611X_REG_MAX_NUM;
            break;
        case Carrier_eModulePpsSync:
            *num = PERSEUS611X_PPSSYNC_MAX_NUM;
            break;
        case Carrier_eModuleLVDS:
            *num = PERSEUS611X_LVDS_MAX_NUM;
            break;
        case Carrier_eModuleAurora:
            *num = PERSEUS611X_AURORA_MAX_NUM;
            break;
        case Carrier_eModuleRTDExSync:
            *num = PERSEUS611X_RTDEXSYNC_MAX_NUM;
            break;
        case Carrier_eModuleTimestamp:
            *num = PERSEUS611X_TIMESTAMP_MAX_NUM;
            break;
        case Carrier_eModuleDPRAM:
            *num = PERSEUS611X_DPRAM_MAX_NUM;
            break;
        case Carrier_eModuleSysMon:
            *num = PERSEUS611X_SYSMON_MAX_NUM;
            break;
        default:
            return CARRIER_INVALID_MODULE;
    }

    return CARRIER_OK;
}

Result Perseus611X_GetMemSpaceBaseAddr(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * addr)
{
    switch(space)
    {
        case Carrier_eMemSpaceUser:
            *addr = PERSEUS611X_USER_SPACE_BASE_ADDR;
            break;
        case Carrier_eMemSpaceFMC:
            *addr = PERSEUS611X_FMC_SPACE_BASE_ADDR;
            break;
        default:
            return CARRIER_INVALID_MEM_SPACE;
    }

    return CARRIER_OK;
}

Result Perseus611X_GetMemSpaceSize(Carrier_handle_t hCarrier, Carrier_eMemSpace_t space, unsigned int * size)
{
    switch(space)
    {
        case Carrier_eMemSpaceUser:
            *size = PERSEUS611X_USER_SPACE_SIZE;
            break;
        case Carrier_eMemSpaceFMC:
            *size = PERSEUS611X_FMC_SPACE_SIZE;
            break;
        default:
            return CARRIER_INVALID_MEM_SPACE;
    }

    return CARRIER_OK;
}

Result Perseus611X_CustomRegisterWrite(Carrier_handle_t hCarrier, unsigned int index, unsigned int value)
{
    volatile unsigned int * pCR;

    // Verif index
    if(index >= PERSEUS611X_CUSTOMREG_MAX_NUM) return CARRIER_INVALID_INDEX;

    // Calculate CR address
    pCR = (unsigned int *)(PERSEUS611X_CUSTOMREG_BASE_ADDR + (index * PERSEUS611X_CUSTOMREG_SIZE));

    // Set the value
    *pCR = value;

    return CARRIER_OK;
}

Result Perseus611X_CustomRegisterRead(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value)
{
    volatile unsigned int * pCR;

    // Verif index
    if(index >= PERSEUS611X_CUSTOMREG_MAX_NUM) return CARRIER_INVALID_INDEX;

    // Calculate CR address
    pCR = (unsigned int *)(PERSEUS611X_CUSTOMREG_BASE_ADDR + (index * PERSEUS611X_CUSTOMREG_SIZE));

    // Set the value
    *value = *pCR;

    return CARRIER_OK;
}

Result Perseus611X_LedSetValue(Carrier_handle_t hCarrier, unsigned int index, unsigned int value)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;

    if(index >= PERSEUS611X_LED_MAX_NUM) return CARRIER_INVALID_INDEX;

    // only keep the first bit
    value &= 0x00000001;

    // value:   0:= turn off, 1:= turn on
    if(value == 0)
    {
        pCore->stLedCtrl.m_uRaw &= ~(1 << index);
    }
    else
    {
        // LED active high
        pCore->stLedCtrl.m_uRaw |= (1 << index);
    }

    return CARRIER_OK;
}

Result Perseus611X_LedGetValue(Carrier_handle_t hCarrier, unsigned int index, unsigned int * value)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;

    if(index >= PERSEUS611X_LED_MAX_NUM) return CARRIER_INVALID_INDEX;

    *value = (pCore->stLedCtrl.m_uRaw >> index) & 0x00000001;

    return CARRIER_OK;
}

Result Perseus611X_Reboot(Carrier_handle_t hCarrier)
{
    volatile Perseus611X_stRegisterMap * pCore = (Perseus611X_stRegisterMap *)hCarrier->pCore;

    pCore->stVadjResetCtrl.stBit.m_uFpgaResetN = 0;
    return CARRIER_OK;
}
