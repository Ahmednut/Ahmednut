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
 * @file       cce_fmcvita.c
 * @brief      fmc vita functions for cce
 *
 * $Date: 2016/07/25 17:29:46 $
 * $Revision: 1.6 $
 *
 * (c) 2013, Nutaq Inc. All rights reserved.
 *
 ***************************************************************************/
//#include <unistd.h>
#include <string.h>

#include "eapi_serialize.h"
#include "cce.h"
#include "cce_fmcvita.h"
#include "bas_debug.h"
#include "FMCEEprom_eeprom.h"
#include "carrier_service.h"
#include "carrier_internal_def.h"


#ifdef DEBUG
// Uncomment following line for debug
#define FMCVITA_DEBUG 1
#endif

//debug or no debug
#ifdef FMCVITA_DEBUG
#include <stdio.h>
#include <stdlib.h>
#define mdebug(f, ...) printf(f, __VA_ARGS__)
#else
#define mdebug(f, ...)
#endif

extern Carrier_handle_t hCarrier;

unsigned int FmcEeprom_SelectedInst = 0;

Result FMCEEprom_eeprom_read_recv(connection_state * state)
{
    Result ret, ret1;
    FMCEEprom_eepromdata epdata;
    unsigned int I2cZone;

    // Check if there is a single eeprom discovered to avoid overriding the wrong one
    if (hCarrier->pEepromList[FmcEeprom_SelectedInst].neeprom != 1)
    {
        if (hCarrier->pEepromList[FmcEeprom_SelectedInst].neeprom == 0)
            ret = FMCVITA_ERR_INVALID_NOEEPROM;
        else
            ret = FMCVITA_ERR_INVALID_EEPROMNUM;
    }
    else
    {
        unsigned char neeprom = 0;

       	// This to avoid sending garbage when not read correctly
        memset(epdata.data, 255, sizeof(epdata.data));
        epdata.size = FMCEEPROM_EEPROM_NOSIZE;

        ret = Carrier_FmcGetI2cZone(hCarrier, FmcEeprom_SelectedInst, &I2cZone);

        ret = FMCEEprom_eeprom_read(hCarrier, I2cZone, &hCarrier->pEepromList[FmcEeprom_SelectedInst], neeprom, &epdata);

        ret1 = buf_send(state, (char*)&epdata, sizeof(FMCEEprom_eepromdata));
        if (SUCCESS(ret))
            ret =ret1;
    }

    return ret;
}

Result FMCEEprom_eeprom_write_recv(connection_state * state)
{
    Result ret = FMCVITA_OK;
    unsigned char neeprom = 0;
    FMCEEprom_eepromdata epdata;
    unsigned int I2cZone;

    // Get desired eeprom data (it must be a full 28128 eepron type size)
    ret = buf_recv(state, (char*)&epdata, sizeof(FMCEEprom_eepromdata));
    if(FAILURE(ret)) return ret;

    // Check if there is a single eeprom discovered to avoid overriding the wrong one
    if (hCarrier->pEepromList[FmcEeprom_SelectedInst].neeprom != 1)
    {
        if (hCarrier->pEepromList[FmcEeprom_SelectedInst].neeprom == 0)
            ret = FMCVITA_ERR_INVALID_NOEEPROM;
        else
            ret = FMCVITA_ERR_INVALID_EEPROMNUM;
    }
    else
    {
        // Check if the eeprom is blank, if not it is protected and cannot be written
        if (hCarrier->pEepromList[FmcEeprom_SelectedInst].type[0] != FMCEEPROM_EEPROM_UNKNOWN)
            ret = FMCVITA_ERR_EEPROM_NOTBLANK;
        else
        {
            // Check if received buffer size is a complete 28128 eeprom size
            if (epdata.size != FMCEEPROM_EEPROM_SZ24128)
                ret = FMCVITA_ERR_INVALID_EEPROMSIZE;
            else
            {
                ret = Carrier_FmcGetI2cZone(hCarrier, FmcEeprom_SelectedInst, &I2cZone);
                if (FAILURE(ret)) return ret;

            	// Force a 24128 eeprom type (when unknown, assumed this is all NUTAQ eeprom module type)
                hCarrier->pEepromList[FmcEeprom_SelectedInst].type[0]=FMCEEPROM_EEPROM_24128;
                ret = FMCEEprom_eeprom_write(hCarrier, I2cZone, &hCarrier->pEepromList[FmcEeprom_SelectedInst], neeprom, &epdata);
            	hCarrier->pEepromList[FmcEeprom_SelectedInst].type[0]=FMCEEPROM_EEPROM_UNKNOWN; // Force a slot discover before getting regognized as valid
            }
        }
    }
    return ret;
}

Result FMCEEprom_eeprom_wipe_recv(connection_state * state)
{
    Result ret = FMCVITA_OK;
    unsigned char neeprom = 0;
    FMCEEprom_eepromdata epdata;
    unsigned int I2cZone;
    uint16_t password;
    
    password = uint16_recv(state);
    if(password != 0xfee7)
    {
        return FMCVITA_ERR_INVALID_PASSWORD;
    }

    // Check if there is a single eeprom discovered to avoid overriding the wrong one
    if (hCarrier->pEepromList[FmcEeprom_SelectedInst].neeprom != 1)
    {
        if (hCarrier->pEepromList[FmcEeprom_SelectedInst].neeprom == 0)
            ret = FMCVITA_ERR_INVALID_NOEEPROM;
        else
            ret = FMCVITA_ERR_INVALID_EEPROMNUM;
    }
    else
    {
        // The type must not be 2402 which are not Nutaq eeproms and not supported here
        if (hCarrier->pEepromList[FmcEeprom_SelectedInst].type[0] != FMCEEPROM_EEPROM_2402)
		{
            epdata.size = FMCEEPROM_EEPROM_SZ24128;
            memset(epdata.data, 255, epdata.size);

            ret = Carrier_FmcGetI2cZone(hCarrier, FmcEeprom_SelectedInst, &I2cZone);
            if (FAILURE(ret)) return ret;

            // Force a 24128 eeprom type (when unknown, assumed this is all NUTAQ eeprom module type)
            hCarrier->pEepromList[FmcEeprom_SelectedInst].type[0]=FMCEEPROM_EEPROM_24128;
            ret = FMCEEprom_eeprom_write(hCarrier, I2cZone, &hCarrier->pEepromList[FmcEeprom_SelectedInst], neeprom, &epdata);
            hCarrier->pEepromList[FmcEeprom_SelectedInst].type[0]=FMCEEPROM_EEPROM_UNKNOWN; // Force a slot discover before getting regognized as valid
		}
        else
        {
			ret = FMCVITA_ERR_INVALID_NOEEPROM;
        }
    }
    return ret;
}


Result FMCEEprom_eeprom_get_idstring_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_IDSTRING, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_manuf_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_MANUF, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_board_state_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_STATE, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_board_type_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_BOARDTYPE, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_core_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_FPGACORE, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_carriervadj_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_CARRIERVADJ, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_partnum_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_PARTNUM, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_serialnum_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_SERIALNUM, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_hardrev_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_HARDREV, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_firmrev_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_SOFTREV, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_modvadj_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_VADJVAL, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

Result FMCEEprom_eeprom_get_corename_recv(connection_state * state)
{
    Result ret = 0, ret1;
    unsigned nb;
    char stringname[200];
    unsigned stringsize = sizeof(stringname);
    int iConnector, iBoard;
    unsigned int uiFmcMaxNum;

    stringname[0]=0;

    nb = uint_recv(state);

    ret = Carrier_GetModuleMaxNumber(hCarrier, Carrier_eModuleFMC, &uiFmcMaxNum);
    if(FAILURE(ret)) return ret;

    if( nb >= uiFmcMaxNum)
    {
        ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
        return CCEINVALIDVALUE;
    }

    iBoard = nb % CARRIER_FMC_MAXBOARD;
    iConnector = nb / CARRIER_FMC_MAXBOARD;

    ret = Carrier_FmcGetInfo(hCarrier,
        iConnector, iBoard,
        CARRIER_INFO_FPGACORENAME, stringname, stringsize);

    ret1 = buf_send(state, (char*)stringname, (strlen(stringname)+1));
    if (SUCCESS(ret))
        ret =ret1;

    return ret;
}

