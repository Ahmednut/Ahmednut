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
 * @file       FMCEEprom_eeprom.h
 * @brief      fmceeprom eeprom library functions definitions.
 *
 * $Date: 2015/11/13 19:22:40 $
 * $Revision: 1.4 $
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

#ifndef __FMCEEPROM_EEPROM_H__
#define __FMCEEPROM_EEPROM_H__

#include "bas_error.h"

#ifdef __cplusplus
    extern "C" {
#endif // __cplusplus 

/****************************************************************************
 *                                 Includes                                 *
 ****************************************************************************/

#include "FMCEEprom_eeprom_def.h"
#include "FMCEEprom_eeprom_cond.h"
#include "carrier_service_def.h"


/** @defgroup FMCEEPROMLIB_EEPROM FMCEEPROM eeprom library
 *  
 *  Allows to deal with multiples eeprom for fru inventory available in the system.
 *               
 *  @file
 *     
 */

/****************************************************************************
 *                             Public Functions                             *
 ****************************************************************************/


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
Result FMCEEprom_eeprom_query(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist);


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
Result FMCEEprom_eeprom_read(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist, unsigned char neeprom, FMCEEprom_eepromdata *epdata);


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
Result FMCEEprom_eeprom_write(Carrier_handle_t hCarrier, unsigned int I2cZone, FMCEEprom_eepromdef *eepromlist, unsigned char neeprom, FMCEEprom_eepromdata *epdata);


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
                                      int printout, char fdebug, char fcanonical);


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
                                    FMCEEprom_moduleid_search *modsearchcond, unsigned int nconditions);


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
                                 unsigned int *vadjmax);


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
                                     char **stringname);


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
                                    char **stringname);


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
                                      char **stringname);


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
                                    char **stringname);


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
                                    char **stringname);


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
                                         char **stringname);


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
                                     FMCEEPROM_IFields **internal);


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
                                          unsigned int *nitems);


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
                                          unsigned int *nitems);


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
                                              unsigned int *nitems);

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
                                            unsigned int *nitems);

#ifdef __cplusplus
} 
#endif  // extern "C"                        
#endif

