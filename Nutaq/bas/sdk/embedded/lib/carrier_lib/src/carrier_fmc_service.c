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
 * @file       carrier_fmc_service.c
 * @brief      FMC carrier generic library functions.
 *
 * $Date: 2015/12/01 18:54:44 $
 * $Revision: 1.5 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup CARRIERLIB
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
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "carrier_service.h"
#include "carrier_service_def.h"
#include "carrier_internal_def.h"
#include "carrier_fmc_service.h"
#include "carrier_fmc_service_def.h"

#include "FMCEEprom_eeprom.h"
#include "FMCEEprom_eeprom_cond.h"

#ifdef DEBUG
// Uncomment following line for debug
#define CARRIERLIB_DEBUG 1
#endif

//debug or no debug
#ifdef CARRIERLIB_DEBUG
    #define FUNC_ENTER()   {if (_printout) printf("carrier_service: enter %s\n", __FUNCTION__);}
    #define FUNC_EXIT()    {if (_printout) printf("carrier_service: exit  %s\n", __FUNCTION__);}
    #define PDEBUG(f, ...) {if (_printout) printf(f, __VA_ARGS__);}
    #define PDEBUG2(f, ...) {if (_printout) printf(f, __VA_ARGS__);}
#else
    #define FUNC_ENTER()
    #define FUNC_EXIT()
    #define PDEBUG(f, ...)
    #define PDEBUG2(f, ...) {if (_printout == CARRIER_PRINTOUT_SUMMARY) printf(f, __VA_ARGS__);}
#endif

CARRIER_printout _printout;

static Result _CARRIER_get_operatingmode(FMCEEprom_module_types boardtype, FMCEEprom_operation_mode *mode);
static char* _CARRIER_itoa(int value, char* result, int base, unsigned strlength);


Result Carrier_FmcDiscover(Carrier_handle_t hCarrier, CARRIER_printout printout)
{
    sCarrier_t * Carrier = (sCarrier_t *)hCarrier;
    Result result=CARRIER_OK, ret11;
    uchar sa=0;   // Eventually could be slot/bus info
    char fcanonical=0;
    char stringname[1000];
    unsigned stringsize;
    Carrier_eFmcBoardNum_t mboard;
    Carrier_eFmcConnectorNum_t mconnector;
    unsigned int I2cZone;

    // Change to 1 to enable extra message in the vita decoding
    // when the library is built with the DEBUG define.
    const char bEnableExtraVitaDebugMessages = 0;

    FUNC_ENTER();

    _printout = printout;

    stringsize = sizeof(stringname);

    if (Carrier == NULL)
    {
        result = CARRIER_NOT_OPEN;
        FUNC_EXIT();
        return result;
    }
    if (!Carrier->bInit)
    {
        result = CARRIER_NOT_OPEN;
        FUNC_EXIT();
        return result;
    }

    if (Carrier->pfCarrier_FmcAssignSlot == NULL)
    {
        result = CARRIER_INVALID_ASSGFCT;
        FUNC_EXIT();
        return result;
    }

    for (mconnector = 0; mconnector < Carrier->uFmcConnectorNumMax; mconnector++)
    {
        PDEBUG("Get I2c zone for the current fmc connector...%c\n",32);        
        ret11 = Carrier_FmcGetI2cZone(hCarrier, mconnector, &I2cZone);

        if (FAILURE(ret11))
        {
            PDEBUG("Carrier_FmcGetI2cZone: error code 0x%08X...\n",ret11);
            FUNC_EXIT();
            return(ret11);
        }

    	PDEBUG("Query board eeproms available...%c\n",32);
    	ret11 = FMCEEprom_eeprom_query(hCarrier, I2cZone, &Carrier->pEepromList[mconnector]);

        if (FAILURE(ret11))
        {
            PDEBUG("FMCEEprom_eeprom_query: error code 0x%08X...\n",ret11);
            FUNC_EXIT();
            return(ret11);
        }

    	PDEBUG("Read and decode all eeproms...%c\n",32);
    	ret11 = FMCEEprom_eeprom_readdecodeall(hCarrier, I2cZone, &Carrier->pEepromList[mconnector], &Carrier->pEepromData[mconnector], sa, printout > CARRIER_PRINTOUT_MINIMUM, bEnableExtraVitaDebugMessages, fcanonical);

        if (FAILURE(ret11))
        {
    	     PDEBUG("FMCEEprom_eeprom_readdecodeall: error code 0x%08X...\n",ret11);
             if (SUCCESS(result)) result=ret11;
        }

        PDEBUG("Identify all module types from read eeproms...%c\n",32);
    	ret11 = FMCEEprom_eeprom_identifyall(&Carrier->pEepromData[mconnector], FMCEEprom_modsearchcond, FMCEEprom_modsearchcond_size);

        if (FAILURE(ret11))
        {
    	     PDEBUG("FMCEEprom_eeprom_identifyall: error code 0x%08X...\n",ret11);
             if (SUCCESS(result)) result=ret11;
        }

		// Board eeprom and modules association
    	PDEBUG("\nAssigning identified modules to board/slot...%c\n",32);
		ret11 = Carrier->pfCarrier_FmcAssignSlot(hCarrier, mconnector);
        if (FAILURE(ret11))
        {
    	     PDEBUG("_FMC_carrier_assign_slot: error code 0x%08X...\n",ret11);
             if (SUCCESS(result)) result=ret11;
        }

        // Ok we went through carrier discover process
        Carrier->bDiscover = 1;

		// Display slot information
    	PDEBUG2("\nDisplay slot/board assigned information...%c\n",32);
        PDEBUG2("FMC connector #%d:\n", mconnector + 1);

        for (mboard = 0; mboard < Carrier->uFmcBoardNumMax; mboard++)
        {
            //mboard = 0;
            PDEBUG2("\nBoard #%d:\n", mboard + 1);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_STATE, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("%s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_BOARDTYPE, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Identified board type: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_FPGACORE, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Board FPGA core id: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_FPGACORENAME, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Board FPGA core name: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_CARRIERVADJ, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Board vAdj carrier setting: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_IDSTRING, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Module Identification: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_MANUF, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Module Manufacturer: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_PARTNUM, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Module Part Number: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_SERIALNUM, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Module Serial Number: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_HARDREV, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Module Hardware Revision: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_SOFTREV, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("Module Firmware Revision: %s\n",stringname);

            ret11 = Carrier_FmcGetInfo(hCarrier, mconnector, mboard, CARRIER_INFO_VADJVAL, stringname, stringsize);
            if (FAILURE(ret11))
            {
                if (SUCCESS(result)) result=ret11;
            }
            PDEBUG2("%s \n",stringname);
        }
    }

    FUNC_EXIT();
    return result;
}

Result Carrier_FmcGetInfo(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector,
    Carrier_eFmcBoardNum_t board, CARRIER_infotype type, char *stringname, unsigned stringsize)
{
    sCarrier_t * Carrier = (sCarrier_t *)hCarrier;
    Carrier_eFmcSlotNum_t slot = Carrier_eFmcSlot0;
    Result result=CARRIER_OK;
    char *mystring, *mystring2;
    unsigned int vadj,vadjmin,vadjmax;

    FUNC_ENTER();

    if (Carrier == NULL)
    {
        result = CARRIER_NOT_OPEN;
        FUNC_EXIT();
        return result;
    }

    // Check if string is valid
    if (stringname == NULL || stringsize == 0 ) {FUNC_EXIT();return(CARRIER_INVALID_STRING);}

    // By default empty string
    *stringname = 0;

    // Check if slot number is valid
    if (connector >= Carrier->uFmcConnectorNumMax || connector < 0) {FUNC_EXIT();return(CARRIER_INVALID_CONNECTOR);}

    // Check if board number is valid
    if (board >= Carrier->uFmcBoardNumMax || board < 0) {FUNC_EXIT();return(CARRIER_INVALID_BOARD);}

	// Check if carrier initialization was done
    if (!Carrier->bInit)
        result = CARRIER_NOT_OPEN;
    else
    {
        // Check if the discover process ran correctly
        if (!Carrier->bDiscover)
            result = CARRIER_NODISCOVER;
        else
        {
            // Check what kind of information is needed
            switch(type)
            {
                case CARRIER_INFO_STATE:
        			if (Carrier->pSlot[connector].modstate[slot][board] == CARRIER_MODULE_UNDETECTED)
        				{strncpy(stringname,"No module present",stringsize-1);stringname[stringsize-1]=0;}
        			else
        				if (Carrier->pSlot[connector].modstate[slot][board] == CARRIER_MODULE_UNIDENTIFIED)
        					{strncpy(stringname,"Unidentified module present",stringsize-1);stringname[stringsize-1]=0;}
        				else
        					if (Carrier->pSlot[connector].modstate[slot][board] == CARRIER_WRONGFPGA_CORE)
        						{strncpy(stringname,"Module present and does not match fpga core",stringsize-1);stringname[stringsize-1]=0;}
        					else
        						if (Carrier->pSlot[connector].modstate[slot][board] == CARRIER_UNMATCH_MODULES_SLOT)
        							{strncpy(stringname,"Module incompatible with other module in the slot",stringsize-1);stringname[stringsize-1]=0;}
        						else
        							if (Carrier->pSlot[connector].modstate[slot][board] == CARRIER_UNMATCH_VADJ)
        								{strncpy(stringname,"Incompatible vAdj requierement between modules",stringsize-1);stringname[stringsize-1]=0;}
        							else
        								if (Carrier->pSlot[connector].modstate[slot][board] == CARRIER_UNSUPPORTED_VADJ)
        								    {strncpy(stringname,"Module vAdj requierement incompatible with carrier",stringsize-1);stringname[stringsize-1]=0;}
                                        else
                                            {strncpy(stringname,"Valid module detected",stringsize-1);stringname[stringsize-1]=0;}
                    break;

                case CARRIER_INFO_BOARDTYPE:
                    strncpy(stringname,Carrier->ppcModuleNameList[Carrier->pSlot[connector].boardtype[slot][board]],stringsize-1);
                    stringname[stringsize-1]=0;
                    result = CARRIER_OK;
                    break;

                case CARRIER_INFO_FPGACORE:
                    _CARRIER_itoa(Carrier->pSlot[connector].fpgacoretype[slot][board], stringname, 16, stringsize);
                    break;

                case CARRIER_INFO_CARRIERVADJ:
                    _CARRIER_itoa(Carrier->pSlot[connector].vadjreq[slot][board], stringname, 10, stringsize);
                    break;

                case CARRIER_INFO_IDSTRING:
            		if (Carrier->pSlot[connector].modstate[slot][board] > CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].module_identindex[slot][board] != 255)
                    {
                        result = FMCEEprom_eeprom_get_idstring(&Carrier->pEepromData[connector], Carrier->pSlot[connector].module_identindex[slot][board],&mystring);
                        if (SUCCESS(result) || result == FMCEEPROM_ERR_INVALID_EEPROMNUM)
                        {
                            strncpy(stringname,mystring,stringsize-1);
                            stringname[stringsize-1]=0;
                            result = CARRIER_OK;
                        }
                    }
                    break;

                case CARRIER_INFO_PARTNUM:
            		if (Carrier->pSlot[connector].modstate[slot][board] > CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].module_identindex[slot][board] != 255)
                    {
                		result = FMCEEprom_eeprom_get_partnum(&Carrier->pEepromData[connector], Carrier->pSlot[connector].module_identindex[slot][board],&mystring);
                        if (SUCCESS(result) || result == FMCEEPROM_ERR_INVALID_EEPROMNUM)
                        {
                            strncpy(stringname,mystring,stringsize-1);
                            stringname[stringsize-1]=0;
                            result = CARRIER_OK;
                        }
                    }
                    break;

                case CARRIER_INFO_SERIALNUM:
            		if (Carrier->pSlot[connector].modstate[slot][board] > CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].module_identindex[slot][board] != 255)
                    {
                		result = FMCEEprom_eeprom_get_serialnum(&Carrier->pEepromData[connector], Carrier->pSlot[connector].module_identindex[slot][board],&mystring);
                        if (SUCCESS(result) || result == FMCEEPROM_ERR_INVALID_EEPROMNUM)
                        {
                            strncpy(stringname,mystring,stringsize-1);
                            stringname[stringsize-1]=0;
                            result = CARRIER_OK;
                        }
                    }
                    break;

                case CARRIER_INFO_HARDREV:
            		if (Carrier->pSlot[connector].modstate[slot][board] > CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].module_identindex[slot][board] != 255)
                    {
                		result = FMCEEprom_eeprom_get_hardrev(&Carrier->pEepromData[connector], Carrier->pSlot[connector].module_identindex[slot][board],&mystring);
                        if (SUCCESS(result) || result == FMCEEPROM_ERR_INVALID_EEPROMNUM)
                        {
                            strncpy(stringname,mystring,stringsize-1);
                            stringname[stringsize-1]=0;
                            result = CARRIER_OK;
                        }
                    }
                    break;

                case CARRIER_INFO_SOFTREV:
            		if (Carrier->pSlot[connector].modstate[slot][board] > CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].module_identindex[slot][board] != 255)
                    {
                		result = FMCEEprom_eeprom_get_firmrev(&Carrier->pEepromData[connector], Carrier->pSlot[connector].module_identindex[slot][board],&mystring);
                        if (SUCCESS(result) || result == FMCEEPROM_ERR_INVALID_EEPROMNUM)
                        {
                            strncpy(stringname,mystring,stringsize-1);
                            stringname[stringsize-1]=0;
                            result = CARRIER_OK;
                        }
                    }
                    break;

                case CARRIER_INFO_VADJVAL:
            		if (Carrier->pSlot[connector].modstate[slot][board] > CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].module_identindex[slot][board] != 255)
                    {
                		// !!! M.T. To be modified for possible double width board with P2 connector
                        result = FMCEEprom_eeprom_get_vadj(&Carrier->pEepromData[connector], FMCEEPROM_EEPROM_CONNP1, Carrier->pSlot[connector].module_identindex[slot][board],&vadj,&vadjmin,&vadjmax);
                        if (SUCCESS(result) || result == FMCEEPROM_ERR_INVALID_EEPROMNUM)
                        {
                            char temp[100], temp1[100],temp2[100];
                            char *mystring3,*mystring4,*mystring5;

                        	//*mystring2 = "Conn P1 Vadj:%d, VadjMin:%d, VadjMax:%d (in 10mV)";
                            mystring2 = "Conn P1 Vadj: ";
                            mystring3 = ", VadjMin: ";
                            mystring4 = ", VadjMax: ";
                            mystring5 = " (in 10mV)";
                            temp[0]=0;
                            temp1[0]=0;
                            temp2[0]=0;
    						_CARRIER_itoa(vadj, temp, 10, sizeof(temp));
    						_CARRIER_itoa(vadjmin, temp1, 10, sizeof(temp1));
    						_CARRIER_itoa(vadjmax, temp2, 10, sizeof(temp2));

    						if ((strlen(mystring2) + strlen(mystring3) + strlen(mystring4) + strlen(mystring5) + strlen(temp) + strlen(temp1) + strlen(temp2)) < stringsize)
    						{
    							strcpy(stringname, mystring2);
    							strcat(stringname, temp);
    							strcat(stringname, mystring3);
    							strcat(stringname, temp1);
    							strcat(stringname, mystring4);
    							strcat(stringname, temp2);
    							strcat(stringname, mystring5);
                            }
                            result = CARRIER_OK;
                        }
                    }
                    break;

                case CARRIER_INFO_MANUF:
            		if (Carrier->pSlot[connector].modstate[slot][board] > CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].module_identindex[slot][board] != 255)
                    {
                        result = FMCEEprom_eeprom_get_manufacturer(&Carrier->pEepromData[connector], Carrier->pSlot[connector].module_identindex[slot][board],&mystring);
                        if (SUCCESS(result) || result == FMCEEPROM_ERR_INVALID_EEPROMNUM)
                        {
                            strncpy(stringname,mystring,stringsize-1);
                            stringname[stringsize-1]=0;
                            result = CARRIER_OK;
                        }
                    }
                    break;

                case CARRIER_INFO_FPGACORENAME:
                    {
                        int count;

                        // Search for core position in the list
                        for (count = 0; count < Carrier->uCoreNum; count++)
                        {
                            if (Carrier->pSlot[connector].fpgacoretype[slot][board] == Carrier->pCoreList[count].fpgacore)
                                break;
                        }
                        // When core is not found, it would be unknown
                        if (count >= Carrier->uCoreNum || count >= Carrier->uCoreNameNum)
                            count = 0;

                        strncpy(stringname,Carrier->ppcCoreNameList[count],stringsize-1);
                        stringname[stringsize-1]=0;
                        result = CARRIER_OK;
                    }
                    break;

                default:
                    result = CARRIER_BADINFOTYPE;
            }
        }
    }

    FUNC_EXIT();

    return result;
}


Result Carrier_FmcValidatePresence(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector,
    Carrier_eFmcBoardNum_t board,  FMCEEprom_module_types *modlst, unsigned nmodlst, CARRIER_detected *detected)
{
    sCarrier_t * Carrier = (sCarrier_t *)hCarrier;
    Carrier_eFmcSlotNum_t slot = Carrier_eFmcSlot0;
    Result result=CARRIER_OK, res1;
    FMCEEprom_operation_mode mode;
	int ncount;

    FUNC_ENTER();

    if (Carrier == NULL)
    {
        result = CARRIER_NOT_OPEN;
        FUNC_EXIT();
        return result;
    }

    // default is not detected
	*detected = CARRIER_NOT_DETECTED;

    // Check if slot number is valid
    if (connector >= Carrier->uFmcConnectorNumMax || connector < 0) {FUNC_EXIT();return(CARRIER_INVALID_CONNECTOR);}

    // Check if board number is valid
    if (board >= Carrier->uFmcBoardNumMax || board < 0) {FUNC_EXIT();return(CARRIER_INVALID_BOARD);}

    // Check if module list to check is valid
    if (modlst == NULL || nmodlst == 0) {FUNC_EXIT();return(CARRIER_INVALID_BOARDTYPELST);}

	// Check if carrier initialization was done
    if (!Carrier->bInit)
        result = CARRIER_NOT_OPEN;
    else
    {
        // Check if the discover process ran correctly
        if (!Carrier->bDiscover)
            result = CARRIER_NODISCOVER;
        else
        {
            // Check if the board in specified slot is valid (this also means matching fpga core)
    		if (Carrier->pSlot[connector].modstate[slot][board] == CARRIER_VALID)
    		{
    			// In safe mode or not, there must be and exact match for detection
    			// Scan all the possible module types that we want to search for a match
    			for (ncount =0; ncount < (int) nmodlst; ncount++)
    			{
    				// Do we match exactly the identified module type?
    				if (modlst[ncount] == Carrier->pSlot[connector].boardtype[slot][board])
                    {
    					// Yes module exactly detected
    					*detected = CARRIER_EXACT_DETECTED;
    					break;
                    }
    			}
    		}
            else
            {
                // The module is not identified correctly
                // Is a module detected in the slot
                if (Carrier->pSlot[connector].modstate[slot][board] != CARRIER_MODULE_UNDETECTED)
                {
                    // Yes there is a module
                    if (Carrier->pSlot[connector].modstate[slot][board] >= CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].modstate[slot][board] <= CARRIER_WRONGFPGA_CORE)
                    {
    					// Check if the detected module was with incorrect fpga core
                        if (Carrier->pSlot[connector].modstate[slot][board] == CARRIER_WRONGFPGA_CORE)
                        {
                			// Get operating mode of identified module
                            res1 = _CARRIER_get_operatingmode(Carrier->pSlot[connector].boardtype[slot][board], &mode);
                			if (FAILURE(res1))
                				result = res1;
                            else
                            {
                                // Check if the identified module operating in safe mode?
                                if (mode == FMCEEPROM_EEPROM_OPER_SAFE)
                                    // Yes could not detect the module
                                    {FUNC_EXIT();return(result);}
                            }
                        }

                        // It is an unidentified module, or an identified with an error condition
                        // Scan all the possible module types we want to detect, if the module types has
                        // compatibility mode, it would be set to detected if the correct FPGA core is detected
    					for (ncount =0; ncount < (int) nmodlst && SUCCESS(result); ncount++)
    					{
                			res1 = _CARRIER_get_operatingmode(modlst[ncount], &mode);
                			if (FAILURE(res1))
                				result = res1;
                            else
                            {
                                // Is this module type runs in compatibility mode
                                if (mode != FMCEEPROM_EEPROM_OPER_SAFE)
                                {
                                    // Check if the fpga core is matching
                    				if (Carrier->pCoreList[modlst[ncount]].fpgacore == Carrier->pSlot[connector].fpgacoretype[slot][board])
                                    {
            							// Yes module detected in compatibility mode (matching fpga core only)
            							*detected = CARRIER_COMP_DETECTED;
            							break;
                                    }
                                }
                            }
    					}
                    }
                }
            }
        }
    }

    FUNC_EXIT();

    return result;
}


Result Carrier_FmcPowerUp(Carrier_handle_t hCarrier, Carrier_eFmcConnectorNum_t connector,
    Carrier_eFmcBoardNum_t board, FMCEEprom_module_types *modlst, unsigned nmodlst, Carrier_eVadjValue_t Value)
{
    sCarrier_t * Carrier = (sCarrier_t *)hCarrier;
    Carrier_eFmcSlotNum_t slot = Carrier_eFmcSlot0;
    Result result=CARRIER_OK;
    CARRIER_detected detected, other;
    Carrier_eVadjValue_t mvalue = Value;
    Carrier_eFmcBoardNum_t mboard;
    FMCEEprom_module_types MyModList[]={FMCEEPROM_EEPROM_MOD_UNKNOWN};

    FUNC_ENTER();

    if (Carrier == NULL)
    {
        result = CARRIER_NOT_OPEN;
        FUNC_EXIT();
        return result;
    }

    if (Carrier->pfCarrier_FmcSetVadj == NULL)
    {
        FUNC_EXIT();
        return CARRIER_INVALID_VADJFCT;
    }

    // Validation for all possible boards in a slot
    for (mboard = 0; mboard < Carrier->uFmcBoardNumMax; mboard++)
    {
        // For the board we were asked to powerup, check the correct board presence
        if (mboard == board)
        {
            // Check for the module presence
            result = Carrier_FmcValidatePresence(hCarrier, connector, mboard, modlst, nmodlst, &detected);
            if (FAILURE(result)) break;
        }
        // For the other board, only check if the board could be detected correctly is one board is there
        else
        {
            // If something in the slot
            if (Carrier->pSlot[connector].modstate[slot][mboard] != CARRIER_MODULE_UNDETECTED)
            {
                // Check for the module presence
                MyModList[0] = Carrier->pSlot[connector].boardtype[slot][mboard];
                result = Carrier_FmcValidatePresence(hCarrier, connector, mboard, MyModList, 1, &other);
                if (FAILURE(result)) break;

                // Is there a problem with this board in this slot?
                if (other == CARRIER_NOT_DETECTED)
                {
                    // Neighbor board is in problem, we cannot set Vadj
                    result = CARRIER_VADJ_NEIGHBORBAD;
                    break;
                }
            }
        }
    }

    if (SUCCESS(result))
    {
        switch(detected)
        {
            default:
                result = CARRIER_VADJ_NOVALIDMODULE;
                break;

            case CARRIER_EXACT_DETECTED:
                // For exact detection, use the voltage read from identification
                mvalue = Carrier->pSlot[connector].vadjreq[slot][board];

            case CARRIER_COMP_DETECTED:
		        // In compatibility mode, use the voltage given.
                // Warning, it is not validated in this mode if other boards in the slot
                // need the same vAdj voltage. But for perseus601x carrier, it is not supposed to happen
                result = Carrier->pfCarrier_FmcSetVadj(hCarrier, connector, board, mvalue);
                break;
        }
    }

    FUNC_EXIT();

    return result;
}


Result Carrier_GetSignalsInfo(Carrier_handle_t hCarrier,
    Carrier_eFmcConnectorNum_t connector,
    Carrier_eFmcBoardNum_t board,
    FMCEEPROM_HPC_DATA_Specs *pDelay)
{
    sCarrier_t * Carrier = (sCarrier_t *)hCarrier;
    Result result=CARRIER_OK;
    Carrier_eFmcSlotNum_t slot = Carrier_eFmcSlot0;
    FMCEEPROM_IFields *internalBottom = NULL, *internal = NULL;
    FMCEEPROM_HPC_DATA_Specs *pSignal = NULL;
    int iCount, iIndex;

    FUNC_ENTER();

    // By default a zero delay structure will be returned
    memset(pDelay,0,sizeof(FMCEEPROM_HPC_DATA_Specs));

    // Check if slot number is valid
    if (connector >= Carrier->uFmcConnectorNumMax || connector < 0) {FUNC_EXIT();return(CARRIER_INVALID_CONNECTOR);}

    // Check if board number is valid
    if (board >= Carrier->uFmcBoardNumMax || board < 0) {FUNC_EXIT();return(CARRIER_INVALID_BOARD);}
    
    	// Check if carrier initialization was done
    if (!Carrier->bInit) { FUNC_EXIT(); return(CARRIER_NOT_OPEN); }

    // Check if the discover process ran correctly
    if (!Carrier->bDiscover) { FUNC_EXIT(); return(CARRIER_NODISCOVER); }

    // Get carrier signal info for this slot
    if(Carrier->pfCarrier_GetSignalsInfo == NULL)
    {
        FUNC_EXIT(); 
        return CARRIER_INVALID_FCT;
    }
    
    result = Carrier->pfCarrier_GetSignalsInfo(hCarrier, connector, &pSignal);
    if (FAILURE(result)) { FUNC_EXIT(); return result; }

    // Copy carrier related delays for default calibration if boards do not have one
    memcpy(pDelay, pSignal ,sizeof(FMCEEPROM_HPC_DATA_Specs));

    // Check for an identified module for the current board
    if (!(Carrier->pSlot[connector].modstate[slot][board] > CARRIER_MODULE_UNIDENTIFIED &&
        Carrier->pSlot[connector].module_identindex[slot][board] != 255))
    {
        FUNC_EXIT();
        return result;
    }

    // Get the internal Nutaq structure of current board
    result = FMCEEprom_eeprom_get_internal(&Carrier->pEepromData[connector], Carrier->pSlot[connector].module_identindex[slot][board],&internal);
    if (FAILURE(result))
    {
        if (result == FMCEEPROM_ERR_INVALID_EEPROMNUM) { result = CARRIER_OK; }
        FUNC_EXIT();
        return result;
    }

    // Check if the internal structure was returned correctly?
    if (internal == 0) { FUNC_EXIT(); return result; }

    // Checks if internal structure appears ok?
    if (!(internal->iversion == 1 && internal->isize == sizeof(FMCEEPROM_IFields)))
    {
        FUNC_EXIT();
        // Unknown internal structure version
        return(CARRIER_INVALID_INTSTRUCT);
    }

    // Checks for the presence of the current board needed calibration
    if (!internal->iHpcInfo.presence) { FUNC_EXIT(); return result; }

    // Check if current board is bottom?
    if (board == Carrier_eFmcBoard0)
    {
        // Adds the current board calibration to the carrier delays for a bottom board
        for (iCount = FMCEEPROM_HPC_LA00; iCount < FMCEEPROM_HPC_LAST; iCount++)
        {
            // Negative delay is ignored
            if (internal->iHpcInfo.delta[iCount] != (unsigned short)(-1))
            {
                // Bottom board line delay is bottom board LA delay + carrier delay
                pDelay->delta[iCount] += internal->iHpcInfo.delta[iCount];
            }
        }
    }
    else // Current board is a top board
    {
        // Check for an identified module at bottom position
        if (!(Carrier->pSlot[connector].modstate[slot][Carrier_eFmcBoard0] > CARRIER_MODULE_UNIDENTIFIED && Carrier->pSlot[connector].module_identindex[slot][Carrier_eFmcBoard0] != 255))
        {
            FUNC_EXIT();
            return result;
        }

        // Get the internal Nutaq structure of bottom board
        result = FMCEEprom_eeprom_get_internal(&Carrier->pEepromData[connector], Carrier->pSlot[connector].module_identindex[slot][Carrier_eFmcBoard0],&internalBottom);
        if (FAILURE(result))
        {
            if (result == FMCEEPROM_ERR_INVALID_EEPROMNUM)
            {
                result = CARRIER_OK;
            }
            FUNC_EXIT();
            return result;
        }

        // Check if the internal structure was returned correctly?
        if (internalBottom == 0) { FUNC_EXIT(); return result; }


        // Checks if internal bottom structure appears ok?
        if (!(internalBottom->iversion == 1 && internalBottom->isize == sizeof(FMCEEPROM_IFields)))
        {
            FUNC_EXIT();
            return CARRIER_INVALID_BOTINTSTRUCT;
        }


        // Checks for the presence of the bottom board needed calibration
        if (!internalBottom->iLpcLa.presence)  { FUNC_EXIT(); return result; }

        // Adds the current board LA calibration to the carrier delays
        for (iCount = FMCEEPROM_HPC_LA00; iCount <= FMCEEPROM_HPC_LA33; iCount++)
        {
            // Get on which pin a top fmc connector pin is routed to the bottom fmc connector on bottom board
            iIndex = internalBottom->iLpcLa.pins[iCount];

            // Checks if pin number is valid
            if (iIndex < FMCEEPROM_HPC_LAST)
            {
                // Negative delay is ignored
                if (internal->iHpcInfo.delta[iCount] != (unsigned short)(-1) && internalBottom->iLpcLa.delta[iCount] != (unsigned short)(-1))
                {
                    // Top board line delay is top board LA delay +  bottom board top->bottom connector delay + carrier delay where it is routed
                    pDelay->delta[iCount] = internal->iHpcInfo.delta[iCount] + internalBottom->iLpcLa.delta[iCount] + pDelay->delta[iIndex];
                }
            }
            else
            {
                // Top connector is routed to an invalid pin
                result = CARRIER_INVALID_ROUTEDPIN;
            }
        }
    }

    FUNC_EXIT();
    return result;
}

/****************************************************************************
 * Function: _CARRIER_get_operatingmode
 ************************************************************************//**
 *
 * Get the operating mode of a specified module type.
 * @warning
 *
 * @param [in] boardtype
 *    The board type we want to get the operating mode.
 *
 * @param [out] mode
 *    Pointer on the mode value that will be returned for the specified module type.
 *
 * @return
 *    Error code: #CARRIER_OK if no problem is found, !=#CARRIER_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured.
 *
 * @see
 *
 ***************************************************************************/
static Result _CARRIER_get_operatingmode(FMCEEprom_module_types boardtype, FMCEEprom_operation_mode *mode)
{
	int ncount;
	Result res = CARRIER_BOARDTYPE_NOTEXIST;

	// Scan all the condition list to find the specified module type
	for (ncount =0; ncount < (int) FMCEEprom_modsearchcond_size; ncount++)
	{
		if (FMCEEprom_modsearchcond[ncount].moduleid == boardtype)
		{
			// Board type found, returns the operating mode
			*mode = FMCEEprom_modsearchcond[ncount].operation;
			res = CARRIER_OK;
			break;
		}
	}

	return(res);
}


/****************************************************************************
 * Function: _CARRIER_itoa
 ************************************************************************//**
 *
 * Convert an integer to a string in the specified base.
 * @warning
 *
 * @param [in] value
 *    Value to be converted.
 *
 * @param [in] result
 *    character buffer where to store the output result (must be longer than 22 chars).
 *
 * @param [in] base
 *    Base for the conversion (from 2 to 36).
 *
 * @param [in] strlength
 *    Len of the result string buffer to check for overflow.
 *
 * @return
 *    the string pointer of the conversion.
 *
 * @see
 *
 ***************************************************************************/
static char* _CARRIER_itoa(int value, char* result, int base, unsigned strlength)
{
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	// check that the base if valid
	if (result == NULL) {return result;}
	if (base < 2 || base > 36 || strlength < 22) { *result = '\0'; return result; }

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}
