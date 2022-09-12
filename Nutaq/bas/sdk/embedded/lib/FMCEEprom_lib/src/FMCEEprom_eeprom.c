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
 * @file       FMCEEprom_eeprom.c
 * @brief      fmceeprom eeprom library functions.
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup FMCEEPROMLIB_EEPROM
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
#include <string.h>
#include <ctype.h>

//#include "eapi_serialize.h"
#define FMCEEPROM_EEPROM_SEARCHCONDITIONS_ALLOCVAR 1
//#include "cce.h"
#include "FMCEEprom_eeprom.h"
#include "bas_debug.h"
#include "carrier_service.h"
#include "EE24LC02B_drv.h"
#include "EEM24128_drv.h"
//#include "cce_plb.h"
#include "FMCEEprom_ifru.h"
#include "FMCEEprom_eeprom_cond.h"


#ifdef DEBUG
// Uncomment following line for debug
#define FMCEEPROM_DEBUG 1
#endif

//debug or no debug
#ifdef FMCEEPROM_DEBUG
#include <stdio.h>
#include <stdlib.h>
    #define FUNC_ENTER()   printf("FMCEEprom_eeprom: enter %s\n", __FUNCTION__)
    #define FUNC_EXIT()    printf("FMCEEprom_eeprom: exit  %s\n", __FUNCTION__)
    #define PDEBUG(f, ...) printf(f, __VA_ARGS__)
#else
    #define FUNC_ENTER()
    #define FUNC_EXIT()
    #define PDEBUG(f, ...)
#endif

// Base address for eeprom
#define EEPROM_BASE_ADDRESS 0x50

typedef int (*char_process_fct)(int);

static Result _FMCEEprom_eeprom_readall(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist, FMCEEPROM_EEprom_Decode *epdata);
static FMCEEprom_module_types _FMCEEprom_eeprom_checkcondition(char *matchstr, char *eepromstr,unsigned char maxeestrlength,FMCEEprom_match_mode matchmode,FMCEEprom_module_types moduleid,FMCEEprom_case_mode casemode);
static char *_FMCEEprom_eeprom_pstrnstr(char *s, char *find, size_t slen, char_process_fct proc);
static int _FMCEEprom_eeprom_pstrncmp(char *s1, char *s2, size_t n, char_process_fct proc);
static int _FMCEEprom_eeprom_noprocess(int char1);


// Allocates a variable to store the dual stack compatibility table list
static FMCEEprom_dualmodule_compatibility FMCEEprom_dualmodule_comptable[] = FMCEEPROM_EEPROM_MODULESTACK_LIST;

// Allocates a variable to store the fpga core id for each module type
static FMCEEprom_module_fpgacoreids FMCEEprom_fpgacoreidtable[FMCEEPROM_EEPROM_MOD_LAST] = FMCEEPROM_EEPROM_FPGA_COREID;

// Allocates a variable to store the fpga core name for each module type
static char * FMCEEprom_fpgacorenametable[FMCEEPROM_EEPROM_MOD_LAST] = FMCEEPROM_EEPROM_FPGA_CORENAME;

// Allocates a variable to store the module name for each module type
static char * FMCEEprom_modnametable[FMCEEPROM_EEPROM_MOD_LAST] = FMCEEPROM_EEPROM_MODULE_NAME;


/****************************************************************************
 * Function: FMCEEprom_eeprom_query
 ************************************************************************//**
 *
 * Search eeproms and build an inventory list of what eeproms are found
 * in the system. It will correctly find all available eeproms, but it
 * may only identify 2402/24128 eeprom types if there are written data
 * in these eeproms and the data should contain at least 2 differents bytes
 * value.   
 * @warning 
 * - This function must be the first one used before using other functions in this module.
 * - This function is not reentrant, and must not be used in mutiple thread at the same time.
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param [out] eepromlist
 *    Pointer to a FMCEEprom_eepromdef structure. The EEPROM inventory found
 *    will be set in this structure. The structure can be used
 *    to list the different eeproms discovered. 
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_query(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist)
{
    int result=FMCEEPROM_EEPROM_OK, result1, loop;
    unsigned char data[FMCEEPROM_EEPROM_SZ24128];
    unsigned char current=0, count, pattern1, pattern2;
    
    FUNC_ENTER();
    
    // Erase the eeprom discovery structure
    memset(eepromlist,0,sizeof(FMCEEprom_eepromdef));
    
    // The following detection logic take into account that they could be only 2 possibles eeprom types,
    // 2402 and 24128 (according to vita 57.1 spec). Not all manufacturer eeproms react the same manner.
    // Warning: because we don't know if a 24128 device is really present, using 24128 normal eeprom read routines
    // is prohibitted if it is possible a 2402 device is there, because it will have the side effect
    // of attempting to write for a 2402 devices (which in the long term will exceeed the max write attempt limit
    // of the 2402 eeprom and surely will corrupt it) because of the different internal devices address
    // pointer size between both devices.
     
    // Scan all possibles i2c eeprom address (this will detect available devices)
    for (count = EEPROM_BASE_ADDRESS; count < EEPROM_BASE_ADDRESS + FMCEEPROM_EEPROM_MAX_ADDR; count++)
    {
        result1 = Carrier_I2cReadNoWrData(hCarrier, I2cZone, count, data, 1);
        if (result1 == 1)
        {
            // Eeprom detected
            PDEBUG("Detected EEprom: 0x%02X, to be analyzed.\n", count);
            (*eepromlist).eeadr[current] = count;
            (*eepromlist).type[current] = FMCEEPROM_EEPROM_UNKNOWN;
            current++;
        }
    }
    
    // Check how many eeprom were found and correct to not take more than the max we can find in a system
    // Some 2402 devices will answer at more than 1 i2c address faking many devices on the bus...
    if (current > FMCEEPROM_EEPROM_MAX)
    {
        PDEBUG("More EEproms detected than possible, found:%d, max:%d.\n", current, FMCEEPROM_EEPROM_MAX);
        current = FMCEEPROM_EEPROM_MAX;
    }
    else
    {
        // Did we detect something
        if (current == 0)
            PDEBUG("No EEprom detected.%c\n", 32);
    }
    
        
    // 2nd algorithm, will try to identify any unknown devices detected above
    // We really don't know what eeprom type we have here for FMCEEPROM_EEPROM_UNKNOWN detected above.
    // We will rely on data pattern detection to identify the eeprom type. 2402 devices will wrap
    // around every 256 bytes in the data. But to detect this, we must have different data in the eeprom 
    // (not a blank one).
    // Scan and search FMCEEPROM_EEPROM_UNKNOWN eeprom detected above...
    for (count = 0; count < current; count++)
    {
        // Check if this detected eeprom was not identified?
        if ((*eepromlist).type[count] == FMCEEPROM_EEPROM_UNKNOWN)
        {
            // Attempt to read without knowing which eeprom type we have.
            // This is safe reading routine for both 2402 and 24128 eeprom types without any risk of eeprom corruption
            // Warning, this operation does not set the internal eeprom read address, so it will start
            // to read randomly from anywhere in eeprom memory, for many eeproms...
            // 1st attempt to load 4 X 2402 size to see if we can find four mirror patterns in the data
            result1 = Carrier_I2cReadNoWrData(hCarrier, I2cZone, (*eepromlist).eeadr[count], data, (4 * FMCEEPROM_EEPROM_SZ2402));
            
            // Did we read the correct number of bytes?
            if(result1 == (4 * FMCEEPROM_EEPROM_SZ2402))
            {
                // Check for non uniform content in data
                for (loop=1, pattern1 = data[0]; loop < (4 * FMCEEPROM_EEPROM_SZ2402); loop++)
                {
                    pattern2 = data[loop];
                    
                    if (pattern1 != pattern2)
                        break;
                }
                // Is the data read the same everywhere?
                if (pattern1 != pattern2)
                {
                    // Look if the 1st difference is after the 2402 size, indicating
                    // it could not be a 2402 device
                    if (loop >= FMCEEPROM_EEPROM_SZ2402)
                        // It should be a 24128
                        pattern1 = 1;
                    else
                    {
                        // Attempt to look if we have 4 data patterns mirror in data, this has a big
                        // probability to identify a 2402 eeprom (256 bytes size)
                        for (loop=0, pattern1 = 0; loop < (FMCEEPROM_EEPROM_SZ2402); loop++)
                        {
                            if (data[loop] != data[loop+FMCEEPROM_EEPROM_SZ2402] || data[loop] != data[loop+FMCEEPROM_EEPROM_SZ2402*2] ||
                                data[loop] != data[loop+FMCEEPROM_EEPROM_SZ2402*3])
                                {
                                    pattern1 = 1;
                                    break;
                                }
                        }
                    }
                    // Did we detect a 24128 device by looking at the data?
                    if (pattern1)
                    {
                        PDEBUG("EEprom: 0x%02X, found 24128 eeprom type.\n", count);
                        (*eepromlist).type[count] = FMCEEPROM_EEPROM_24128;
                    }
                    else
                    {
                        PDEBUG("EEprom: 0x%02X, found 2402 eeprom type.\n", count);
                        (*eepromlist).type[count] = FMCEEPROM_EEPROM_2402;
                    }
                }
                else
                {
                    // Continue to read until the size of a 24128 is read
                    result1 = Carrier_I2cReadNoWrData(hCarrier, I2cZone, (*eepromlist).eeadr[count], &data[(4 * FMCEEPROM_EEPROM_SZ2402)], sizeof(data) - (4 * FMCEEPROM_EEPROM_SZ2402));
                    
                    // Did we read the correct number of bytes?
                    if (result1 == (sizeof(data) - (4 * FMCEEPROM_EEPROM_SZ2402)))
                    {
                        // Check for non uniform content in data in the rest of the eeprom
                        for (loop=(4 * FMCEEPROM_EEPROM_SZ2402), pattern1 = data[0]; loop < sizeof(data); loop++)
                        {
                            pattern2 = data[loop];
                            
                            if (pattern1 != pattern2)
                                break;
                        }
                        // Is the data read the same everywhere?
                        if (pattern1 != pattern2)
                        {
                            PDEBUG("EEprom: 0x%02X, found 24128 eeprom type.\n", count);
                            (*eepromlist).type[count] = FMCEEPROM_EEPROM_24128;
                        }
                        else
                        {
                            // Uniform eeprom content (empty eeprom?), cannot identify eeprom type
                            PDEBUG("EEprom: 0x%02X, found unknown eeprom type.\n", (*eepromlist).eeadr[count]);
                        }
                    }
                    else
                    {
                        // Read error, cannot identify eeprom type
                        PDEBUG("EEprom: 0x%02X, read error, cannot identify eeprom type.\n", (*eepromlist).eeadr[count]);
                        PDEBUG("EEprom: 0x%02X, byte read expected: %ld, read: %d.\n", (*eepromlist).eeadr[count], (sizeof(data) - (4 * FMCEEPROM_EEPROM_SZ2402)), result1);
                    }
                }
            }
            else
            {
                // Read error, cannot identify eeprom type
                PDEBUG("EEprom: 0x%02X, read error, cannot identify eeprom type.\n", (*eepromlist).eeadr[count]);
                PDEBUG("EEprom: 0x%02X, byte read expected: %d, read: %d.\n", (*eepromlist).eeadr[count], (4 * FMCEEPROM_EEPROM_SZ2402), result1);
            }
        }
    }
    (*eepromlist).neeprom = current;

    FUNC_EXIT();
    
    return result;
}

/****************************************************************************
 * Function: FMCEEprom_eeprom_read
 ************************************************************************//**
 *
 * Read the complete eeprom data content for the specified eeprom index. The eeprom
 * index is one of the eeprom discovered by the function FMCEEprom_eeprom_query().
 * @warning 
 * FMCEEprom_eeprom_query() must be called successfully before using this functions.
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param [in] eepromlist
 *    Pointer to the FMCEEprom_eepromdef structure previously configured when
 *    FMCEEprom_eeprom_query function has been called.
 *
 * @param [in] neeprom
 *    Eeprom index from the FMCEEprom_eepromdef structure. It must be a valid index
 *    of an existing eeprom discovered.
 *
 * @param [in,out] epdata
 *    Eeprom data structure is given and will be filled after eeprom reading operation.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_read(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist, unsigned char neeprom, FMCEEprom_eepromdata *epdata)
{
    Result ret;
    int result;

    FUNC_ENTER();
    
    // Check if the eeprom index is valid and inside the eeprom discovered inventory
    if (neeprom >= (*eepromlist).neeprom || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret= FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if we have a 24128 eeprom
        if ((*eepromlist).type[neeprom] == FMCEEPROM_EEPROM_24128)
        {
            epdata->size = FMCEEPROM_EEPROM_SZ24128;
            result = EEM24128_readMemory(hCarrier, I2cZone, (*eepromlist).eeadr[neeprom], 0, epdata->data, (unsigned)epdata->size);
            if(result == epdata->size) {
                ret = FMCEEPROM_EEPROM_OK;
            }
            else
            {
                PDEBUG("EEprom: 0x%02X, error reading eeprom.\n", (*eepromlist).eeadr[neeprom]);
                ret = FMCEEPROM_EEPROM_ERR_READSIZE;
            }
        }
        else
        {
            // Check if we have a 2402 eeprom
            if ((*eepromlist).type[neeprom] == FMCEEPROM_EEPROM_2402)
            {
                epdata->size = FMCEEPROM_EEPROM_SZ2402;
                result = EE24LC02B_readMemory(hCarrier, I2cZone, (*eepromlist).eeadr[neeprom], 0, epdata->data, (unsigned)epdata->size);
                if(result == epdata->size) {
                    ret = FMCEEPROM_EEPROM_OK;
                }
                else
                {
                    PDEBUG("EEprom: 0x%02X, error reading eeprom.\n", (*eepromlist).eeadr[neeprom]);
                    ret = FMCEEPROM_EEPROM_ERR_READSIZE;
                }
            }
            else
            {
                if ((*eepromlist).type[neeprom] != FMCEEPROM_EEPROM_UNKNOWN)
                {
                    // Invalid eeprom type
                    PDEBUG("EEprom: 0x%02X, invalid eeprom type.\n", (*eepromlist).eeadr[neeprom]);
                    ret = FMCEEPROM_ERR_INVALID_EEPROMTYPE;
                }
                else
                    // The eeprom is unknown
                    ret = FMCEEPROM_ERR_UNKNOWN_EEPROMTYPE;
            }
        }
    }

    FUNC_EXIT();
    
    return ret;
}

/****************************************************************************
 * Function: FMCEEprom_eeprom_write
 ************************************************************************//**
 *
 * Write the complete eeprom data content for the specified eeprom index. The eeprom
 * index is one of the eeprom discovered by the function FMCEEprom_eeprom_query().
 * @warning 
 * FMCEEprom_eeprom_query() must be called successfully before using this functions.
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param [in] eepromlist
 *    Pointer to the FMCEEprom_eepromdef structure previously configured when
 *    FMCEEprom_eeprom_query function has been called.
 *
 * @param [in] neeprom
 *    Eeprom index from the FMCEEprom_eepromdef structure. It must be a valid index
 *    of an existing eeprom discovered.
 *
 * @param [in] epdata
 *    Eeprom data structure is given with correct eeprom data and eeprom size.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_write(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist, unsigned char neeprom, FMCEEprom_eepromdata *epdata)
{
    Result ret;
    int result;

    FUNC_ENTER();
    
    // Check if the eeprom index is valid and inside the eeprom discovered inventory
    if (neeprom >= (*eepromlist).neeprom || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret= FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if we have a 24128 eeprom
        if ((*eepromlist).type[neeprom] == FMCEEPROM_EEPROM_24128)
        {
            if (epdata->size != FMCEEPROM_EEPROM_SZ24128)
                ret = FMCEEPROM_EEPROM_ERR_WRITESIZE;
            else
            {
                result = EEM24128_writeMemory(hCarrier, I2cZone, (*eepromlist).eeadr[neeprom], 0, epdata->data, (unsigned)epdata->size);
                if(result == epdata->size) {
                    ret = FMCEEPROM_EEPROM_OK;
                }
                else
                {
                    PDEBUG("EEprom: 0x%02X, error writing eeprom.\n", (*eepromlist).eeadr[neeprom]);
                    ret = FMCEEPROM_EEPROM_ERR_READSIZE;
                }
            }
        }
        else
        {
            // Check if we have a 2402 eeprom
            if ((*eepromlist).type[neeprom] == FMCEEPROM_EEPROM_2402)
            {
                if (epdata->size != FMCEEPROM_EEPROM_SZ2402)
                    ret = FMCEEPROM_EEPROM_ERR_WRITESIZE;
                else
                {
                    result = EE24LC02B_writeMemory(hCarrier, I2cZone, (*eepromlist).eeadr[neeprom], 0, epdata->data, (unsigned)epdata->size);
                    if(result == epdata->size) {
                        ret = FMCEEPROM_EEPROM_OK;
                    }
                    else
                    {
                        PDEBUG("EEprom: 0x%02X, error writing eeprom.\n", (*eepromlist).eeadr[neeprom]);
                        ret = FMCEEPROM_EEPROM_ERR_READSIZE;
                    }
                }
            }
            else
            {
                if ((*eepromlist).type[neeprom] != FMCEEPROM_EEPROM_UNKNOWN)
                {
                    // Invalid eeprom type
                    PDEBUG("EEprom: 0x%02X, invalid eeprom type.\n", (*eepromlist).eeadr[neeprom]);
                    ret = FMCEEPROM_ERR_INVALID_EEPROMTYPE;
                }
                else
                    // The eeprom is unknown
                    ret = FMCEEPROM_ERR_UNKNOWN_EEPROMTYPE;
            }
        }
    }

    FUNC_EXIT();
    
    return ret;
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_readdecodeall
 ************************************************************************//**
 *
 * Read the complete eeprom data content and decode all existing eeproms.
 * The decoding process will identify the eeprom encoding type (old custom eeprom
 * format or new vita 57 standard format), will decode the content according to
 * encoding format rules. It will also shows how many eeprom were decoded correctly
 * for each format. Once the decoding is completed, eeprom assignation to module can be done
 * with module identification.    
 * @warning 
 * FMCEEprom_eeprom_query() must be called successfully before using this functions.
 *
 * @param [in] hCarrier
 *    Handle to a carrier object. It is the carrier that will handle the I2C
 *    communication.
 *
 * @param [in] I2cZone
 *    Integer that indicates in which I2C zone is the I2C device
 *    is located.
 *
 * @param [in] eepromlist
 *    Pointer to the FMCEEprom_eepromdef structure previously configured when
 *    FMCEEprom_eeprom_query function has been called.
 *
 * @param [in,out] epdata
 *    Eeprom decoding data structure is given and will be filled after eeprom reading operation
 *    with each eeproms content. It will also be filled with eeprom decoding information, ready
 *    to be associated to a module for identification. 
 *
 * @param [in] sa
 *    Slot identification. It can be used to identify which bus was used and it is displayed
 *    if canonical format is not used. Use a value of 0 for a single bus system.
 *
 * @param [in] printout
 *    This will control if console output messages are needed (when !=0) when decoding vita eeprom format.
 *    To get console output, the vita decoding library must be compile to support this.
 *
 * @param [in] fdebug
 *    This will control to get extra debug messages on the console for vita decoding, but only
 *    when printout specifies console output.
 *
 * @param [in] fcanonical
 *    This will control the console output format: when !=0, it will not display sa parameter
 *    information, neither eeprom address on the console, but only
 *    when printout specifies console output.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_readdecodeall(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist, FMCEEPROM_EEprom_Decode *epdata, uchar sa,
                                      int printout, char fdebug, char fcanonical)
{
    Result ret, ret1;
    unsigned char count;
    uchar *pfru = NULL;
    int sfru = 0;
    uchar fruid =0;
    uchar frutype = 0;
    char fgetintchk = 1;
    char forcecont = 0;
    FMCEEPROM_vita_requierements vitalevel=FMCEEPROM_VITA_MINIMUM, mvitalevel;
    FMCEEPROM_content *fmceeprom;
    
    FUNC_ENTER();
    
    // Read all eeproms content in inventory, this also erase epdata structure prior to reading
    ret =  _FMCEEprom_eeprom_readall(hCarrier, I2cZone, eepromlist, epdata);
    
    // Attempt to decode all eeproms that were read correctly
    for (count = 0; count < epdata->neepromrd && count < FMCEEPROM_EEPROM_MAX; count++)
    {
        // Use the eeprom 7 bits address as fru id
        fruid = (*eepromlist).eeadr[epdata->eescan_index[count]];
        sfru = epdata->eeprom[count].eebindata.size;
        pfru = epdata->eeprom[count].eebindata.data;
        mvitalevel = vitalevel;
        fmceeprom = &epdata ->vitadecode[epdata->neeprom_decoded];
        ret1 = show_fru(sa,fruid,frutype,pfru,sfru,fdebug,fgetintchk,fcanonical,forcecont,fmceeprom,&mvitalevel, printout);
        
        // Increment the number of eeprom decoded correctly in the inventory
        if (SUCCESS(ret1))
        {
            // Save the vita level detected
            PDEBUG("EEprom#%02d, vita format, level:%d.\n", count, mvitalevel);
            epdata ->dvitalevel[epdata->neeprom_decoded] = mvitalevel;
            epdata ->eedecode[epdata->neeprom_decoded] = FMCEEPROM_EEPROM_VITADECODE;
            epdata ->eescan_dindex[epdata->neeprom_decoded]=count;
            (epdata->neeprom_decoded)++;
        }
        else
        {
            PDEBUG("EEprom#%02d, vita decoding problem, error:%08X.\n", count, ret1);

            ret1 = FMCEEPROM_EEPROM_OK;
            
            // Detect if we have old custom eeprom format
            if (strncmp("Nutaq",epdata->eeprom[count].olddecode.vendorName,5) == 0 ||
                strncmp("Lyrtech RD",epdata->eeprom[count].olddecode.vendorName,10) == 0 ||
                strncmp("LYRTECH RD",epdata->eeprom[count].olddecode.vendorName,10) == 0 ||
                strncmp("NUTAQ",epdata->eeprom[count].olddecode.vendorName,5) == 0 ||
                strncmp("lyrtech rd",epdata->eeprom[count].olddecode.vendorName,10) == 0 ||
                strncmp("nutaq",epdata->eeprom[count].olddecode.vendorName,5) == 0)
            {
                // Shows that we have an old eeprom custom format
                PDEBUG("EEprom#%02d, old format.\n", count);
                epdata ->eedecode[epdata->neeprom_decoded] = FMCEEPROM_EEPROM_OLDDECODE;
                epdata ->eescan_dindex[epdata->neeprom_decoded]=count;
                (epdata->neeprom_decoded)++;
            }
            else
            {
                PDEBUG("EEprom#%02d, old format decoding problem.\n", count);
            }
        }
        if (SUCCESS(ret))
            ret = ret1;
    }

    FUNC_EXIT();
    
    return ret;
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_identifyall
 ************************************************************************//**
 *
 * Identify the modules types associated with all decoded eeprom. This routine
 * will parse the detection conditions list to attempt to identify which module
 * type is associated with the differents decoded eeprom.  
 * @warning 
 * FMCEEprom_eeprom_query() and FMCEEprom_eeprom_readdecodeall() must be called successfully
 * before using this functions.
 *
 * @param [in,out] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_readdecodeall()
 *    to fill that structure. It returns the module types identification for each eeprom according
 *    to the provided conditions for identification. 
 *
 * @param [in] modsearchcond
 *    List of modules identification conditions that will be scanned to identify the modules.
 *
 * @param [in] nconditions
 *    Number of conditions available in the List modsearchcond.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_identifyall(FMCEEPROM_EEprom_Decode *epdata, 
                                    FMCEEprom_moduleid_search *modsearchcond, unsigned int nconditions)
{
    unsigned int condition, identified, eeprom;
    Result ret=FMCEEPROM_EEPROM_OK;
    
    FUNC_ENTER();
    
    // Scan every decode eeprom for identification
    for (eeprom = 0; eeprom < epdata->neeprom_decoded && SUCCESS(ret) && eeprom < FMCEEPROM_EEPROM_MAX; eeprom++)
    {
        // Verify each condition to see if there is a match for the current eeprom
        for (condition = 0, identified = 0; condition < nconditions && !identified && SUCCESS(ret); condition++)
        {
            // Check if the condition should be verified for this eeprom decoding type?
            if (modsearchcond[condition].decodet == epdata->eedecode[eeprom])
            {
                // If a vita condition
                if (modsearchcond[condition].decodet == FMCEEPROM_EEPROM_VITADECODE)
                {
                    // Check if minimum vita decoded level is reached
                    if (modsearchcond[condition].vital > epdata->dvitalevel[eeprom])
                        // Skip to next condition
                        continue;
                        
                    // Check what is the field use for that condition
                    switch(modsearchcond[condition].fieldused)
                    {
                     case FMCEEPROM_BOARD_MANUFACTURER:
                     case FMCEEPROM_BOARD_PRODUCTNAME:
                     case FMCEEPROM_BOARD_SERIALNUM:
                     case FMCEEPROM_BOARD_PARTNUM:
                     case FMCEEPROM_BOARD_FRUFILE:
                     case FMCEEPROM_BOARD_CUSTOM1:
                     case FMCEEPROM_BOARD_CUSTOM2:
                     case FMCEEPROM_BOARD_CUSTOM3:
                     case FMCEEPROM_BOARD_CUSTOM4:
                     case FMCEEPROM_BOARD_CUSTOM5:
                         // Check if the field specified exist and it is a FMCEEPROM_ASCII field type
                         if (epdata->vitadecode[eeprom].board.board_typelen_fields[modsearchcond[condition].fieldused].presence == FMCEEPROM_PRE_PRESENT &&
                             epdata->vitadecode[eeprom].board.board_typelen_fields[modsearchcond[condition].fieldused].type == FMCEEPROM_ASCII)
                         {
                             epdata->modtype[epdata->nmodule_identified] = _FMCEEprom_eeprom_checkcondition(modsearchcond[condition].matchstr,
                                                                                        (char *)epdata->vitadecode[eeprom].board.board_typelen_fields[modsearchcond[condition].fieldused].data,
                                                                                        sizeof(epdata->vitadecode[eeprom].board.board_typelen_fields[modsearchcond[condition].fieldused].data),
                                                                                        modsearchcond[condition].matchmode,
                                                                                        modsearchcond[condition].moduleid,
                                                                                        modsearchcond[condition].casemode);
                             // Check if the module was identified correctly
                             if (epdata->modtype[epdata->nmodule_identified] != FMCEEPROM_EEPROM_MOD_UNKNOWN)
                             {
                                 // Save the decoded eeprom index of this identified module
                                 PDEBUG("EEprom#%d, identified type: %d.\n", eeprom, epdata->modtype[epdata->nmodule_identified]);
                                 epdata->eescan_identindex[epdata->nmodule_identified] = eeprom;
                                 (epdata->nmodule_identified)++;
                                 identified = 1;
                             }
                         }
                         break;

                     default:
                         // Invalid field used, returns an error
                         PDEBUG("EEprom#%d, invalid vita field used in condition: %d.\n", eeprom, condition);
                         ret=FMCEEPROM_ERR_INVALID_IDENTVITAFIELD;
                         break;
                    }
                }
                else
                {
                    // If an old eeprom format condition
                    if (modsearchcond[condition].decodet == FMCEEPROM_EEPROM_OLDDECODE)
                    {
                        // This is a condition check for an old eeprom encoding (only the partNumber field can be checked)
                        epdata->modtype[epdata->nmodule_identified] = _FMCEEprom_eeprom_checkcondition(modsearchcond[condition].matchstr,
                                                                                   epdata->eeprom[epdata->eescan_dindex[eeprom]].olddecode.partNumber,
                                                                                   sizeof(epdata->eeprom[epdata->eescan_dindex[eeprom]].olddecode.partNumber),
                                                                                   modsearchcond[condition].matchmode,
                                                                                   modsearchcond[condition].moduleid,
                                                                                   modsearchcond[condition].casemode);
                        // Check if the module was identified correctly
                        if (epdata->modtype[epdata->nmodule_identified] != FMCEEPROM_EEPROM_MOD_UNKNOWN)
                        {
                            // Save the decoded eeprom index of this identified module
                            PDEBUG("EEprom#%d, identified type: %d.\n", eeprom, epdata->modtype[epdata->nmodule_identified]);
                            epdata->eescan_identindex[epdata->nmodule_identified] = eeprom;
                            (epdata->nmodule_identified)++;
                            identified = 1;
                        }
                    }
                    else
                    {
                        // Invalid eeprom decoding parameter used in the condition
                        PDEBUG("EEprom#%d, invalid condition:%d format identifier: %d.\n", eeprom, condition, modsearchcond[condition].decodet);
                        ret=FMCEEPROM_ERR_INVALID_IDENTEEDECODE;
                    }
                }
            }
        }
    }

    FUNC_EXIT();
    
    return(ret);
}                                    


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_vadj
 ************************************************************************//**
 *
 * Get the vadj nominal voltage value for the specified eeprom index and connector.
 * This value could be used on a carrier to detect if it can provide compatible
 * vadj voltage to the related module.  
 * @warning 
 * FMCEEprom_eeprom_identifyall() must be called successfully (with identified eeproms)
 * before using this function.
 *
 * @param [in] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_identifyall().
 *    This structure contains all information related to the board.
 *
 * @param [in] conn
 *    This is the connector we want to know the vadj value. For single width module on a single
 *    width carrier, only FMCEEPROM_EEPROM_CONNP1 is possible. For a double width module,
 *    the voltage information is also available for FMCEEPROM_EEPROM_CONNP2
 *
 * @param [in] neeprom
 *    Eeprom index from the modtype structure (FMCEEprom_module_types). It must be a valid index
 *    of an existing eeprom identified with FMCEEprom_eeprom_identifyall().
 *
 * @param [out] vadj
 *    Returns the nominal value of vadj in 10mv.
 *
 * @param [out] vadjmin
 *    Returns the minimum value of vadj in 10mv.
 *
 * @param [out] vadjmax
 *    Returns the maximum value of vadj in 10mv.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall(), FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_vadj(FMCEEPROM_EEprom_Decode *epdata,  FMCEEprom_eeprom_modconn conn,
                                 unsigned char neeprom, unsigned int *vadj,  unsigned int *vadjmin,
                                 unsigned int *vadjmax)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    FMCEEPROM_mrecord_power recordid;    
    
    // By default, 0V
    *vadj = 0;
    *vadjmin = 0;
    *vadjmax = 0;        
        
    // Check for valid connector identification
    if (conn < FMCEEPROM_EEPROM_CONNP1 || conn >= FMCEEPROM_EEPROM_CONN_LAST)
        ret = FMCEEPROM_ERR_INVALID_CONNINFO;
    else
    {
        // Check for a valid index
        if (neeprom >= epdata->nmodule_identified || neeprom >= FMCEEPROM_EEPROM_MAX)
            ret = FMCEEPROM_ERR_INVALID_EEPROMNUM;
        else
        {
            // Check if the module was identified correctly
            if (epdata->modtype[neeprom] <= FMCEEPROM_EEPROM_MOD_UNKNOWN || epdata->modtype[neeprom] >= FMCEEPROM_EEPROM_MOD_LAST)
                ret = FMCEEPROM_ERR_UNKNOWN_MODULETYPE;
            else
            {
                // Check what kind of eeprom decoding it was
                if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_OLDDECODE)
                {
                    if (conn == FMCEEPROM_EEPROM_CONNP1)
                        // All old eeprom format modules are 2.5V
                    {
                        // Default to 2.5v 5% tolerance
                        *vadj = 250;
                        *vadjmin = 238;                        
                        *vadjmax = 262;                        
                    }
                }
                else
                {
                    // Get which multirecord needed for vadj according to what conector is used
                    recordid = FMCEEPROM_POWER_P1_VADJ_DCLD;
                    if (conn == FMCEEPROM_EEPROM_CONNP2)
                        recordid = FMCEEPROM_POWER_P2_VADJ_DCLD;
                        
                    // Vita decoded module has the vadj voltage level in a multirecord 
                    if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_VITADECODE)
                    {
                        // Check the validity of the vadj multirecord
                        if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].power[recordid].presence == FMCEEPROM_PRE_PRESENT &&
                            epdata->vitadecode[epdata->eescan_identindex[neeprom]].power[recordid].ptype == FMCEEPROM_POWER_DCLOAD)
                            {
                                *vadj = epdata->vitadecode[epdata->eescan_identindex[neeprom]].power[recordid].nominal_volt;
                                *vadjmin = epdata->vitadecode[epdata->eescan_identindex[neeprom]].power[recordid].min_volt;
                                *vadjmax = epdata->vitadecode[epdata->eescan_identindex[neeprom]].power[recordid].max_volt;
                            }
                        else
                        {
                            // Give an error only if a double width and conn p1, p2 info is not there when it must be, or simple width and p1 info is not there
                            if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].vitamain.modsize == FMCEEPROM_VITAMODSZ_DBLWIDTH)
                            {
                                // For vita double width, there must be vadj info on connector P1
                        	    if (conn == FMCEEPROM_EEPROM_CONNP1)
                                    ret = FMCEEPROM_ERR_INVALID_VADJINFO;
                                else
                                    // For conn P2, if fitted vadj info must be there
                                    if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].vitamain.p2connsize != FMCEEPROM_VITACONNT_NOTFITTED)
                            	        ret = FMCEEPROM_ERR_INVALID_VADJINFO;
                            }
                            else
                                // For vita simple width, there must be vadj info on connector P1
                        	    if (conn == FMCEEPROM_EEPROM_CONNP1)
                                    ret = FMCEEPROM_ERR_INVALID_VADJINFO;
                        }
                    }
                    else
                        // Invalid decoded format
                        ret = FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT;
                }
            }
        }    
    }
    
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_idstring
 ************************************************************************//**
 *
 * Get the module id string information, this is the module name in string
 * format that is human readable.
 *
 * @param [in] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_identifyall().
 *    This structure contains all information related to the board.
 *
 * @param [in] neeprom
 *    Eeprom index from the modtype structure (FMCEEprom_module_types). It must be a valid index
 *    of an existing eeprom identified with FMCEEprom_eeprom_identifyall().
 *
 * @param [out] stringname
 *    Returns a string pointer for the module id.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall(), FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_idstring(FMCEEPROM_EEprom_Decode *epdata, unsigned char neeprom,
                                     char **stringname)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    // Default empty string
    *stringname = "";
        
    // Check for a valid index
    if (neeprom >= epdata->nmodule_identified || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret = FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if the module was identified correctly
        if (epdata->modtype[neeprom] <= FMCEEPROM_EEPROM_MOD_UNKNOWN || epdata->modtype[neeprom] >= FMCEEPROM_EEPROM_MOD_LAST)
            ret = FMCEEPROM_ERR_UNKNOWN_MODULETYPE;
        else
        {
            // Check what kind of eeprom decoding it was
            if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_OLDDECODE)
            {
                *stringname = epdata->eeprom[epdata->eescan_dindex[epdata->eescan_identindex[neeprom]]].olddecode.generalDescription;
            }
            else
            {
                // Get vita board name
                if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_VITADECODE)
                {
                    // Check the validity of board section name
                    if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.presence == FMCEEPROM_PRE_PRESENT &&
                        epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_PRODUCTNAME].presence == FMCEEPROM_PRE_PRESENT &&
                        epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_PRODUCTNAME].type != FMCEEPROM_BINARY)
                        {
                            *stringname = (char *)(epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_PRODUCTNAME].data);
                        }
                    else
                        ret = FMCEEPROM_ERR_INVALID_MODID_INFO;                
                }
                else
                    // Invalid decoded format
                    ret = FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT;
            }
        }
    }    
    
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_partnum
 ************************************************************************//**
 *
 * Get the module part number string information, this is the module part
 * number string in format that is human readable.
 *
 * @param [in] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_identifyall().
 *    This structure contains all information related to the board.
 *
 * @param [in] neeprom
 *    Eeprom index from the modtype structure (FMCEEprom_module_types). It must be a valid index
 *    of an existing eeprom identified with FMCEEprom_eeprom_identifyall().
 *
 * @param [out] stringname
 *    Returns a string pointer for the part number.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall(), FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_partnum(FMCEEPROM_EEprom_Decode *epdata, unsigned char neeprom,
                                    char **stringname)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    // Default empty string
    *stringname = "";
        
    // Check for a valid index
    if (neeprom >= epdata->nmodule_identified || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret = FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if the module was identified correctly
        if (epdata->modtype[neeprom] <= FMCEEPROM_EEPROM_MOD_UNKNOWN || epdata->modtype[neeprom] >= FMCEEPROM_EEPROM_MOD_LAST)
            ret = FMCEEPROM_ERR_UNKNOWN_MODULETYPE;
        else
        {
            // Check what kind of eeprom decoding it was
            if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_OLDDECODE)
            {
                *stringname = epdata->eeprom[epdata->eescan_dindex[epdata->eescan_identindex[neeprom]]].olddecode.partNumber;
            }
            else
            {
                if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_VITADECODE)
                {
                    // Check the validity of board section name
                    if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.presence == FMCEEPROM_PRE_PRESENT &&
                        epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_PARTNUM].presence == FMCEEPROM_PRE_PRESENT &&
                        epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_PARTNUM].type != FMCEEPROM_BINARY)
                        {
                            *stringname = (char *)(epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_PARTNUM].data);
                        }
                    else
                        ret = FMCEEPROM_ERR_INVALID_PARTNUM_INFO;                
                }
                else
                    // Invalid decoded format
                    ret = FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT;
            }
        }
    }    
    
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_serialnum
 ************************************************************************//**
 *
 * Get the module serial number string information, this is the module serial
 * number string in format that is human readable.
 *
 * @param [in] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_identifyall().
 *    This structure contains all information related to the board.
 *
 * @param [in] neeprom
 *    Eeprom index from the modtype structure (FMCEEprom_module_types). It must be a valid index
 *    of an existing eeprom identified with FMCEEprom_eeprom_identifyall().
 *
 * @param [out] stringname
 *    Returns a string pointer for the serial number.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall(), FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_serialnum(FMCEEPROM_EEprom_Decode *epdata, unsigned char neeprom,
                                      char **stringname)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    // Default empty string
    *stringname = "";
        
    // Check for a valid index
    if (neeprom >= epdata->nmodule_identified || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret = FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if the module was identified correctly
        if (epdata->modtype[neeprom] <= FMCEEPROM_EEPROM_MOD_UNKNOWN || epdata->modtype[neeprom] >= FMCEEPROM_EEPROM_MOD_LAST)
            ret = FMCEEPROM_ERR_UNKNOWN_MODULETYPE;
        else
        {
            // Check what kind of eeprom decoding it was
            if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_OLDDECODE)
            {
                // No serial number in old decode format
                *stringname = "";
            }
            else
            {
                if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_VITADECODE)
                {
                    // Check the validity of board section name
                    if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.presence == FMCEEPROM_PRE_PRESENT &&
                        epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_SERIALNUM].presence == FMCEEPROM_PRE_PRESENT &&
                        epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_SERIALNUM].type != FMCEEPROM_BINARY)
                        {
                            *stringname = (char *)(epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_SERIALNUM].data);
                        }
                    else
                        ret = FMCEEPROM_ERR_INVALID_SERIALNUM_INFO;                
                }
                else
                    // Invalid decoded format
                    ret = FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT;
            }
        }
    }    
    
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_hardrev
 ************************************************************************//**
 *
 * Get the module hardware revision string information, this is the module hardware
 * revision string in format that is human readable.
 *
 * @param [in] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_identifyall().
 *    This structure contains all information related to the board.
 *
 * @param [in] neeprom
 *    Eeprom index from the modtype structure (FMCEEprom_module_types). It must be a valid index
 *    of an existing eeprom identified with FMCEEprom_eeprom_identifyall().
 *
 * @param [out] stringname
 *    Returns a string pointer for the hardware revision.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall(), FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_hardrev(FMCEEPROM_EEprom_Decode *epdata, unsigned char neeprom,
                                    char **stringname)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    // Default empty string
    *stringname = "";
        
    // Check for a valid index
    if (neeprom >= epdata->nmodule_identified || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret = FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if the module was identified correctly
        if (epdata->modtype[neeprom] <= FMCEEPROM_EEPROM_MOD_UNKNOWN || epdata->modtype[neeprom] >= FMCEEPROM_EEPROM_MOD_LAST)
            ret = FMCEEPROM_ERR_UNKNOWN_MODULETYPE;
        else
        {
            // Check what kind of eeprom decoding it was
            if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_OLDDECODE)
            {
                *stringname = epdata->eeprom[epdata->eescan_dindex[epdata->eescan_identindex[neeprom]]].olddecode.hardwareRevision;
            }
            else
            {
                if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_VITADECODE)
                {
                    // This information is only available if we have nutaq vita level
                    if (epdata->dvitalevel[epdata->eescan_identindex[neeprom]] >= FMCEEPROM_VITA_NUTAQMIN)
                    {
                        // Check the validity of board section name
                        if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.presence == FMCEEPROM_PRE_PRESENT &&
                            epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_CUSTOM1].presence == FMCEEPROM_PRE_PRESENT &&
                            epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_CUSTOM1].type != FMCEEPROM_BINARY)
                            {
                                *stringname = (char *)(epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_CUSTOM1].data);
                            }
                        else
                            ret = FMCEEPROM_ERR_INVALID_HARDREV_INFO;                
                    }
                    else
                        *stringname = "";
                }                        
                else
                    // Invalid decoded format
                    ret = FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT;
            }
        }
    }    
    
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_firmrev
 ************************************************************************//**
 *
 * Get the module firmware revision string information, this is the module firmware
 * revision string in format that is human readable.
 *
 * @param [in] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_identifyall().
 *    This structure contains all information related to the board.
 *
 * @param [in] neeprom
 *    Eeprom index from the modtype structure (FMCEEprom_module_types). It must be a valid index
 *    of an existing eeprom identified with FMCEEprom_eeprom_identifyall().
 *
 * @param [out] stringname
 *    Returns a string pointer for the hardware revision.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall(), FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_firmrev(FMCEEPROM_EEprom_Decode *epdata, unsigned char neeprom,
                                    char **stringname)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    // Default empty string
    *stringname = "";
        
    // Check for a valid index
    if (neeprom >= epdata->nmodule_identified || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret = FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if the module was identified correctly
        if (epdata->modtype[neeprom] <= FMCEEPROM_EEPROM_MOD_UNKNOWN || epdata->modtype[neeprom] >= FMCEEPROM_EEPROM_MOD_LAST)
            ret = FMCEEPROM_ERR_UNKNOWN_MODULETYPE;
        else
        {
            // Check what kind of eeprom decoding it was
            if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_OLDDECODE)
            {
                *stringname = epdata->eeprom[epdata->eescan_dindex[epdata->eescan_identindex[neeprom]]].olddecode.firmwareRevision;
            }
            else
            {
                if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_VITADECODE)
                {
                    // This information is only available if we have nutaq vita level
                    if (epdata->dvitalevel[epdata->eescan_identindex[neeprom]] >= FMCEEPROM_VITA_NUTAQMIN)
                    {
                        // Check the validity of board section name
                        if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.presence == FMCEEPROM_PRE_PRESENT &&
                            epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_CUSTOM2].presence == FMCEEPROM_PRE_PRESENT &&
                            epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_CUSTOM2].type != FMCEEPROM_BINARY)
                            {
                                *stringname = (char *)(epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_CUSTOM2].data);
                            }
                        else
                            ret = FMCEEPROM_ERR_INVALID_FIRMREV_INFO;                
                    }
                    else
                        *stringname = "";
                }                        
                else
                    // Invalid decoded format
                    ret = FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT;
            }
        }
    }    
    
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_manufacturer
 ************************************************************************//**
 *
 * Get the module manufacturer string information, this is the module manufacturer
 * string in format that is human readable.
 *
 * @param [in] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_identifyall().
 *    This structure contains all information related to the board.
 *
 * @param [in] neeprom
 *    Eeprom index from the modtype structure (FMCEEprom_module_types). It must be a valid index
 *    of an existing eeprom identified with FMCEEprom_eeprom_identifyall().
 *
 * @param [out] stringname
 *    Returns a string pointer for the manufacturer.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall(), FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_manufacturer(FMCEEPROM_EEprom_Decode *epdata, unsigned char neeprom,
                                         char **stringname)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    // Default empty string
    *stringname = "";
        
    // Check for a valid index
    if (neeprom >= epdata->nmodule_identified || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret = FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if the module was identified correctly
        if (epdata->modtype[neeprom] <= FMCEEPROM_EEPROM_MOD_UNKNOWN || epdata->modtype[neeprom] >= FMCEEPROM_EEPROM_MOD_LAST)
            ret = FMCEEPROM_ERR_UNKNOWN_MODULETYPE;
        else
        {
            // Check what kind of eeprom decoding it was
            if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_OLDDECODE)
            {
                *stringname = epdata->eeprom[epdata->eescan_dindex[epdata->eescan_identindex[neeprom]]].olddecode.vendorName;
            }
            else
            {
                if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_VITADECODE)
                {
                    // Check the validity of board section name
                    if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.presence == FMCEEPROM_PRE_PRESENT &&
                        epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_MANUFACTURER].presence == FMCEEPROM_PRE_PRESENT &&
                        epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_MANUFACTURER].type != FMCEEPROM_BINARY)
                        {
                            *stringname = (char *)(epdata->vitadecode[epdata->eescan_identindex[neeprom]].board.board_typelen_fields[FMCEEPROM_BOARD_MANUFACTURER].data);
                        }
                    else
                        ret = FMCEEPROM_ERR_INVALID_MANUF_INFO;                
                }
                else
                    // Invalid decoded format
                    ret = FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT;
            }
        }
    }    
    
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_internal
 ************************************************************************//**
 *
 * Get a pointer on an internal custom NUTAQ information structure for the module. This
 * section is available only for NUTAQ vita eeprom encoded modules.
 *
 * @param [in] epdata
 *    Eeprom decoding information as input is provided by using FMCEEprom_eeprom_identifyall().
 *    This structure contains all information related to the board.
 *
 * @param [in] neeprom
 *    Eeprom index from the modtype structure (FMCEEprom_module_types). It must be a valid index
 *    of an existing eeprom identified with FMCEEprom_eeprom_identifyall().
 *
 * @param [out] internal
 *    Returns a pointer to the Nutaq custom internal structure (NULL if the structure is not available)
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query(), FMCEEprom_eeprom_readdecodeall(), FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_internal(FMCEEPROM_EEprom_Decode *epdata, unsigned char neeprom,
                                     FMCEEPROM_IFields **internal)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    // Default no internal structure
    *internal = NULL;
        
    // Check for a valid index
    if (neeprom >= epdata->nmodule_identified || neeprom >= FMCEEPROM_EEPROM_MAX)
        ret = FMCEEPROM_ERR_INVALID_EEPROMNUM;
    else
    {
        // Check if the module was identified correctly
        if (epdata->modtype[neeprom] <= FMCEEPROM_EEPROM_MOD_UNKNOWN || epdata->modtype[neeprom] >= FMCEEPROM_EEPROM_MOD_LAST)
            ret = FMCEEPROM_ERR_UNKNOWN_MODULETYPE;
        else
        {
            // Check what kind of eeprom decoding it was
            if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_OLDDECODE)
            {
                // There is no internal section in module encoded with old format
                *internal = NULL;
            }
            else
            {
                if (epdata->eedecode[epdata->eescan_identindex[neeprom]] == FMCEEPROM_EEPROM_VITADECODE)
                {
                    // This information is only available if we have nutaq vita level
                    if (epdata->dvitalevel[epdata->eescan_identindex[neeprom]] >= FMCEEPROM_VITA_NUTAQMIN)
                    {
                        // Check the validity of internal section
                        if (epdata->vitadecode[epdata->eescan_identindex[neeprom]].internal.presence == FMCEEPROM_PRE_PRESENT &&
                            epdata->vitadecode[epdata->eescan_identindex[neeprom]].internal.internal_area_version== 1)
                        {
                            // Get a ptr on the internal section
                            *internal = (&epdata->vitadecode[epdata->eescan_identindex[neeprom]].internal.IFields);
                        }
                        else
                            ret = FMCEEPROM_ERR_INVALID_INTERNAL_INFO;                
                    }
                    else
                        *internal = NULL;
                }                        
                else
                    // Invalid decoded format
                    ret = FMCEEPROM_ERR_UNKNOWN_MODULEEEFORMAT;
            }
        }
    }    
    
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_complist_info
 ************************************************************************//**
 *
 * Get a pointer on the compatibility list info between bottom/top modules.
 * This gives also information of modules types that could go in top position. 
 *
 * @param [in,out] complist
 *    A pointer is given on a compatibility list pointer to be assigned to the compatibility
 *    list for modules pairs.
 *
 * @param [out] nitems
 *    This is the number of items pairs in the compatibility list.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_complist_info(FMCEEprom_dualmodule_compatibility **complist,
                                          unsigned int *nitems)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    *complist = &FMCEEprom_dualmodule_comptable[0];
    *nitems = sizeof(FMCEEprom_dualmodule_comptable) / sizeof(FMCEEprom_dualmodule_compatibility);
             
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_fpgacore_info
 ************************************************************************//**
 *
 * Get a pointer on the fpga core id list for all module types
 *
 * @param [in,out] corelist
 *    A pointer is given on a fpga core list pointer to be assigned to the fpga
 *    core list for each module type.
 *
 * @param [out] nitems
 *    This is the number of items pairs in the fpga core list.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_fpgacore_info(FMCEEprom_module_fpgacoreids **corelist,
                                          unsigned int *nitems)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    *corelist = &FMCEEprom_fpgacoreidtable[0];
    *nitems = sizeof(FMCEEprom_fpgacoreidtable) / sizeof(FMCEEprom_module_fpgacoreids);
             
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_fpgacorename_info
 ************************************************************************//**
 *
 * Get a pointer on the fpga core id list for all module types
 *
 * @param [in,out] corenamelist
 *    A pointer is given on a fpga corename list pointer to be assigned to the fpga
 *    corename list for each module type.
 *
 * @param [out] nitems
 *    This is the number of items pairs in the fpga core list.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_fpgacorename_info(char *** corenamelist,
                                              unsigned int *nitems)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    *corenamelist = (char **)FMCEEprom_fpgacorenametable;
    *nitems = sizeof(FMCEEprom_fpgacorenametable) / sizeof(char *);
             
    return(ret);
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_get_modulename_info
 ************************************************************************//**
 *
 * Get a pointer on the module name list for all module types
 *
 * @param [in,out] modnamelist
 *    A pointer is given on a module name list pointer to be assigned to the module
 *    name list for each module type.
 *
 * @param [out] nitems
 *    This is the number of items pairs in the module name list.
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *     
 * @see
 *
 ***************************************************************************/ 
Result FMCEEprom_eeprom_get_modulename_info(char *** modnamelist,
                                            unsigned int *nitems)
{
    Result ret = FMCEEPROM_EEPROM_OK;
    
    *modnamelist = (char **)FMCEEprom_modnametable;
    *nitems = sizeof(FMCEEprom_modnametable) / sizeof(char *);
             
    return(ret);
}


/****************************************************************************
 * Function: _FMCEEprom_eeprom_readall
 ************************************************************************//**
 *
 * Read the complete eeprom data content for all existing eeproms in inventory.
 * @warning 
 * FMCEEprom_eeprom_query() must be called successfully before using this functions.
 *
 * @param [in] i2cBaseAddr
 *    I2C base memory address
 *
 * @param [in,out] epdata
 *    Eeprom decoding data structure is given and will be filled after eeprom reading operation
 *    with each eeproms content. It will fill also which eeprom index in the scanned inventory
 *    was read correctly. 
 *
 * @return
 *    Error code: #FMCEEPROM_EEPROM_OK if no problem is found, !=#FMCEEPROM_EEPROM_OK if problem.
 *    Use SUCCESS(ret) macro to detect if an error occured. 
 * 
 * @pre
 *    FMCEEprom_eeprom_query().
 *     
 * @see
 *
 ***************************************************************************/ 
static Result _FMCEEprom_eeprom_readall(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist, FMCEEPROM_EEprom_Decode *epdata)
{
    Result ret = FMCEEPROM_EEPROM_OK, ret1;
    unsigned char count;
    unsigned count1, error=0;

    FUNC_ENTER();

    // Make sure the decoded structure is clean
    memset(epdata, 0 ,sizeof(FMCEEPROM_EEprom_Decode));
    
    // Read all eeprom contents in the scanned inventory
    for (count = 0; count < (*eepromlist).neeprom && count < FMCEEPROM_EEPROM_MAX; count++)
    {
        //printf("&epdata->eeprom[epdata->neepromrd].eebindata    %d \r\n",sizeof(epdata->eeprom[epdata->neepromrd].eebindata));
		ret1 = FMCEEprom_eeprom_read(hCarrier, I2cZone, eepromlist, count, &epdata->eeprom[epdata->neepromrd].eebindata);
        
        // Increment the number of eeprom read correctly in the inventory if we got no error
        // If the eeprom type is unknown (blank eeprom), do not consider this as an error
        if (SUCCESS(ret1) || ret1 == FMCEEPROM_ERR_UNKNOWN_EEPROMTYPE)
        {
            // Save which eeprom in the inventory that was read correctly
            epdata->eescan_index[epdata->neepromrd] = count;
            (epdata->neepromrd)++;
        }
    }
    
    // Check if all eeprom were read correctly
    if (epdata->neepromrd != (*eepromlist).neeprom)
        ret = FMCEEPROM_ERR_COULDNOTREADALLEEPROM;

    // More than 1 eeprom read?
    if (epdata->neepromrd > 1)
    {
        // Check if we have shadow eeprom (for 2402 types, there could be eight shadow devices for microchip)
        for (count = epdata->neepromrd - 1; count > 0 && count < FMCEEPROM_EEPROM_MAX && !error; count--)
        {
            // Check if both eeprom have 2402 size
            if (epdata->eeprom[count].eebindata.size == FMCEEPROM_EEPROM_SZ2402 &&
                epdata->eeprom[count - 1].eebindata.size == FMCEEPROM_EEPROM_SZ2402)
            {
                // Check if both are an exact copy of the other
                for (count1 = 0; count1 < FMCEEPROM_EEPROM_SZ2402; count1++)
                {
                    // same data?
                    if (epdata->eeprom[count].eebindata.data[count1] != epdata->eeprom[count-1].eebindata.data[count1])
                    {
                        error = 1;
                        break;
                    }
                }
                // Both copies are the same?
                if (count1 == FMCEEPROM_EEPROM_SZ2402)
                {
                    // Yes, remove the last eeprom from the list
                    PDEBUG("EEprom#%d, shadow of EEprom#%d, removing it.\n", count, count-1);
                    epdata->eeprom[count].eebindata.size = 0;
                    (epdata->neepromrd)--;
                }
            }
        }
    } 

    FUNC_EXIT();

    return ret;
}


/****************************************************************************
 * Function: FMCEEprom_eeprom_checkcondition
 ************************************************************************//**
 *
 * Check if an identification condition is match for a module. It will search
 * the provided eeprom string if the match string is a substring of it. There
 * could be match all characters ('?') in the match string.  
 * @warning 
 * FMCEEprom_eeprom_identifyall() must be called before using this function.
 *
 * @param [in] matchstr
 *    String to be verified from the match condition (could have match all characters '?')
 *
 * @param [in] eepromstr
 *    String to be verified from the eeprom against the match string
 *
 * @param [in] maxeestrlength
 *    This is the maximum possible size of the eepromstr string in case it is
 *    wrongly null terminated. No more characters than this size is checked
 *
 * @param [in] matchmode
 *    This is the string search match mode in the search field eepromstr
 *
 * @param [in] moduleid
 *    This is the module identification code that will be returned in case of a match
 *
 * @param [in] casemode
 *    This is the case character match mode used (sensitive or not)
 *
 * @return
 *    moduleid if there is a macth or FMCEEPROM_EEPROM_MOD_UNKNOWN if no match is found
 * 
 * @pre
 *    FMCEEprom_eeprom_identifyall().
 *     
 * @see
 *
 ***************************************************************************/ 
static FMCEEprom_module_types _FMCEEprom_eeprom_checkcondition(char *matchstr, char *eepromstr,
                                                               unsigned char maxeestrlength,
                                                               FMCEEprom_match_mode matchmode,
                                                               FMCEEprom_module_types moduleid,
                                                               FMCEEprom_case_mode casemode)
{
    size_t eepromlen, matchlen;
    FMCEEprom_module_types result = FMCEEPROM_EEPROM_MOD_UNKNOWN;
    char *search;
    char_process_fct proc;
    
    // Adjust the character processing fct according to case match mode
    if (casemode == FMCEEPROM_EEPROM_CASE_INSENSITIVE)
        proc = &toupper;
    else
        proc = &_FMCEEprom_eeprom_noprocess;
        
    // Get the length of the eeprom string
    eepromlen = strlen(eepromstr);
    
    // Check if there is not a eeprom string overflow to limit the search to max size
    if (eepromlen > maxeestrlength)
        eepromlen = maxeestrlength;
        
    // Get the length of the match string
    matchlen = strlen(matchstr);
    
    // Check if there is not a match string overflow to limit the search to max size
    if (matchlen > maxeestrlength)
        matchlen = maxeestrlength;
        
    // Use the match mode according to what is specified
    if (matchmode == FMCEEPROM_EEPROM_MATCH_ALL)
    {
        // Do the search for the specified eeprom string and match substring condition
        // anywhere in the eeprom field
        search = _FMCEEprom_eeprom_pstrnstr(eepromstr, matchstr, eepromlen, proc);

        // Do we have a match (matchstr was found as a substring in eepromstr?)
        if (search != NULL)
            // Match found, the module is identified
            result = moduleid;
    }
    else
    {
        // Do the search for the specified eeprom string and match substring condition
        // at the beginning of the eeprom field only
        if (_FMCEEprom_eeprom_pstrncmp(eepromstr, matchstr, matchlen, proc) == 0)
            // Match found, the module is identified
            result = moduleid;
    }
    
    return(result);        
}                                                       


/****************************************************************************
 * Function: _FMCEEprom_eeprom_pstrnstr
 ************************************************************************//**
 *
 * Search if a string is a substring of another string. This is similar as the
 * strnstr standard library function, but the difference is that the second string (str) may content
 * match all characters '?' which is simply ignored in the compare. The search is limited
 * to the 1st characters slen provided in s string. 
 *
 * @param [in] in
 *    string which will be searched for the provided substring
 *
 * @param [in] str
 *    this is the substring that we try to find in the in string. It can include some special
 *    match all characters '?'.
 *
 * @param [in] slen
 *    this is the max length of s string to find
 *
 * @param [in] proc
 *    character processing function to be applied on each character verified (toupper, _FMCEEprom_eeprom_noprocess)
 *    It must be assigned to a valid processing function because the pointer is not verified. 
 *
 * @return
 *    NULL if str is not found inside in string, or a pointer inside in is returned to show
 *    the 1st occurence where the substring str was found.
 * 
 * @pre
 *     
 * @see
 *
 ***************************************************************************/ 
static char *_FMCEEprom_eeprom_pstrnstr(char *s, char *find, size_t slen, char_process_fct proc)
{
	char c, sc;
	size_t len;

	if ((c = proc(*find++)) != '\0') 
    {
		len = strlen(find);
		do
        {
			do
            {
				if ((sc = proc(*s++)) == '\0' || slen-- < 1)
					return (NULL);
			} while (sc != c && c!='?');
			if (len > slen)
				return (NULL);
		} while (_FMCEEprom_eeprom_pstrncmp(s, find, len, proc) != 0);
		s--;
	}
	return ((char *)s);
}


/****************************************************************************
 * Function: _FMCEEprom_eeprom_pstrncmp
 ************************************************************************//**
 *
 * Compare 2 strings (maximum specified number of characters) to check if
 * there are the same. This is similar as the strncmp standard library
 * function, but the difference is that the second string (s2) may content
 * match all characters '?' which is simply ignored in the compare.
 *
 * @param [in] s1
 *    1st string to be verified
 *
 * @param [in] s2
 *    2nd string to be verified that can include some special match all characters '?'.
 *
 * @param [in] n
 *    Maximum number of characters to be verified between the string
 *
 * @param [in] proc
 *    character processing function to be applied on each character verified (toupper, _FMCEEprom_eeprom_noprocess)
 *    It must be assigned to a valid processing function because the pointer is not verified. 
 *
 * @return
 *    0 if strings are equal, -1 or +1 if s1 has a character smaller or greater than in s2
 * 
 * @pre
 *     
 * @see
 *
 ***************************************************************************/ 
static int _FMCEEprom_eeprom_pstrncmp(char *s1, char *s2, size_t n, char_process_fct proc)
{
    for ( ; n > 0; s1++, s2++, --n)
	if ((proc(*s1)) != (proc(*s2)) && *s2 !='?')
	    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
	else if (*s1 == '\0')
	    return 0;
    return 0;
}


/****************************************************************************
 * Function: _FMCEEprom_eeprom_noprocess
 ************************************************************************//**
 *
 * This is a dummy function not to process characters, use for exact case
 * match with _FMCEEprom_eeprom_pstrncmp and _FMCEEprom_eeprom_pstrnstr.
 *
 * @param [in] char1
 *    Character to process
 *
 * @return
 *    char1
 * 
 * @pre
 *     
 * @see
 *
 ***************************************************************************/ 
static int _FMCEEprom_eeprom_noprocess(int char1)
{
    return char1;
}

