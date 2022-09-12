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
 * @file       EEM24128_drv.c
 * @brief      EEPROM driver for EEM24128
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup DRIVERS
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

#include "EEM24128_drv.h"
#include "carrier_service.h"

// M24128 EEPROM support page size of 64 bytes.
#define EEM24128_PAGE_SIZE	64

// Indicates the number of bytes for the data address of the EEPROM.
#define EEPROM_NB_BYTE_ADDRESS  2

int EEM24128_writeMemory(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char eeadr, unsigned short addr, unsigned char * data, unsigned nbByte)
{
    int result;
    I2C_Options_t options;
    unsigned short startAddr;
    unsigned short stopAddr;
    unsigned nbByteAbleToSend;
    unsigned nbByteToSend;
    int nbTotalByteToSend;
    int nbByteSent;
    
    options.nbByteAddr = EEPROM_NB_BYTE_ADDRESS;
    nbByteSent = 0;
    
    // Write one page at a time.
    startAddr = addr;
    stopAddr = addr + nbByte - 1;
    nbTotalByteToSend = nbByte;
    
    do {
        result = startAddr % EEM24128_PAGE_SIZE;
        if(result == 0) {
            nbByteAbleToSend = EEM24128_PAGE_SIZE;
        }
        else {
            nbByteAbleToSend = EEM24128_PAGE_SIZE - result;
        }
        
        nbByteToSend = nbByteAbleToSend;
        if(nbByteAbleToSend > nbTotalByteToSend) {
            nbByteToSend = nbTotalByteToSend;
        }
        usleep(30000);
        result = Carrier_I2cWrite(hCarrier, I2cZone, eeadr, startAddr, data, nbByteToSend, options);
        if(result == -1) {
            return(result);
        }
        else {
            nbByteSent += result;
            startAddr += nbByteToSend;
            nbTotalByteToSend = nbTotalByteToSend - nbByteToSend;
            if(nbTotalByteToSend > 0) {
                data += nbByteToSend;
            }
        }
    }while(startAddr <= stopAddr);
    
    return(nbByteSent);                              
}

int EEM24128_readMemory(Carrier_handle_t hCarrier, unsigned int I2cZone, unsigned char eeadr, unsigned short addr, unsigned char * data, unsigned nbByte)
{
    int result;
    I2C_Options_t options;
    
    options.nbByteAddr = EEPROM_NB_BYTE_ADDRESS;
    result = Carrier_I2cRead(hCarrier, I2cZone, eeadr, addr, data, nbByte, options);

    return(result);
}

