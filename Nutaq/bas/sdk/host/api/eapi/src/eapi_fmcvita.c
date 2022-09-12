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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       ****
 *                                                                        ***      
 *
 ************************************************************************//**
 *
 * @file      eapi_fmcvita.c
 * 
 * @brief     Function definitions for the eapi fmc vita control
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
#include "eapi_fmcvita.h"
#include "cce.h"
#include "eapi_serialize.h"

Result FMCEEprom_eeprom_read_send(connection_state * state, FMCEEprom_eepromdata *oepdata)
{
    Result ret; 
    
    cmd_send(state, FMCEEPROM_EEPROM_READ);
    ret = buf_recv(state, (char*)oepdata, sizeof(FMCEEprom_eepromdata));
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}

Result FMCEEprom_eeprom_write_send(connection_state * state, FMCEEprom_eepromdata *epdata)
{
    Result ret; 
    
    cmd_send(state, FMCEEPROM_EEPROM_WRITE);
    ret = buf_send(state, (char*)epdata, sizeof(FMCEEprom_eepromdata));
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}

Result FMCEEprom_eeprom_wipe_send(connection_state * state, unsigned short password)
{
    Result ret; 
    
    cmd_send(state, FMCEEPROM_EEPROM_WIPE);
    uint16_send(state, password);
    //if(FAILURE(ret)) return ret;
    ret = result_recv(state);
    return ret;
}

Result FMCEEprom_eeprom_get_idstring_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_IDSTRING);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_board_state_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_BOARD_STATE);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_board_type_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_BOARD_TYPE);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_core_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_CORE);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_carriervadj_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_CARRIERVADJ);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_partnum_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_PARTNUM);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_serialnum_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_SERIALNUM);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_hardrev_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_HARDREV);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_firmrev_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_FIRMREV);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_modvadj_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_MODVADJ);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}

Result FMCEEprom_eeprom_get_corename_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_CORENAME);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


Result FMCEEprom_eeprom_get_manuf_send(connection_state * state, unsigned nb, char *mystring, unsigned stringlen)
{
    Result ret; 
    
	cmd_send(state, FMCEEPROM_EEPROM_GET_MANUF);
	uint_send(state, nb-1);
    ret = buf_recv(state, mystring, stringlen);
    if(FAILURE(ret)) return ret;
	ret = result_recv(state);
    return ret;
}


