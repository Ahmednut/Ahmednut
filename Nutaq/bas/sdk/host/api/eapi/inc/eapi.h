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
 * @file       eapi.h
 * @brief      Function definitions for the eapi core functionalities
 *
 * $Date: 2016/07/19 15:08:59 $
 * $Revision: 1.6 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup EAPI
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

#ifndef __ADP_EAPI__
#define __ADP_EAPI__

#include "bas_error.h"
#include "connection_state.h"
#include "carrier_service_def.h"

#ifdef __cplusplus
extern "C" {
#endif

#define EAPI_VERSION 722    ///< ADP version 7.2.2

#if EAPI_VERSION == 640
#define MIN_VERSION 20800   ///< Minimum CCE version 2.08.00
#define MAX_VERSION 20899   ///< Maximum CCE version 2.08.99
#endif

#if EAPI_VERSION == 650
#define MIN_VERSION 21000   ///< Minimum CCE version 2.10.00
#define MAX_VERSION 21299   ///< Maximum CCE version 2.12.99
#endif

#if EAPI_VERSION == 660
#define MIN_VERSION 21300   ///< Minimum CCE version 2.13.00
#define MAX_VERSION 21399   ///< Maximum CCE version 2.13.99
#endif

#if EAPI_VERSION == 661
#define MIN_VERSION 21400   ///< Minimum CCE version 2.14.00
#define MAX_VERSION 21499   ///< Maximum CCE version 2.14.99
#endif

#if EAPI_VERSION == 700
#define MIN_VERSION 30200   ///< Minimum CCE version 3.00.00
#define MAX_VERSION 30299   ///< Maximum CCE version 3.00.99
#endif

#if EAPI_VERSION == 720
#define MIN_VERSION 30300   ///< Minimum CCE version 3.00.00
#define MAX_VERSION 30399   ///< Maximum CCE version 3.00.99
#endif

#if EAPI_VERSION == 721
#define MIN_VERSION 30400   ///< Minimum CCE version 3.00.00
#define MAX_VERSION 30499   ///< Maximum CCE version 3.00.99
#endif

#if EAPI_VERSION == 722
#define MIN_VERSION 30500   ///< Minimum CCE version 3.00.00
#define MAX_VERSION 30599   ///< Maximum CCE version 3.00.99
#endif

#define MIN_VERSION_UPDATE_COMPATIBLE 21224

/**
*****************************************************************************
* Function: connect_cce
**************************************************************************//**
*
* Connects to a central communication engine (CCE) process.
*
* @param[in]  servip String containing a Perseus IP address.
*
* @param[in,out] state TCP connection state structure.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result connect_cce(const char * servip, connection_state * state);

/****************************************************************************
* Function: disconnect_cce
*************************************************************************//**
*
* Disconnects from a CCE process.
*
* @param[in,out] state TCP connection state structure.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result disconnect_cce(connection_state * state);

/*****************************************************************************
* Function: echo_send
*************************************************************************//**
*
* Requests an echo from the CCE. Acts as the ping command.
*
* @param[in,out] state TCP connection state structure.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result echo_send(connection_state * state);

/****************************************************************************
* Function: get_cce_version
*************************************************************************//**
*
* Get the CCE version
*
* @param[in,out] state TCP connection state structure.
*
* @param[out] version Returns the CCE version in a string.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result get_cce_version(connection_state * state, char * version);

/****************************************************************************
* Function: get_eapi_version
*************************************************************************//**
*
* Get the eapi version (ADP release)
*
* Return: the eapi release version in integer (EAPI_VERSION define)
*
****************************************************************************/
int get_eapi_version();


/****************************************************************************
* Function: fpgaflash_send
************************************************************************//**
*
* Programs FPGA bitstreams in the flash memory from the buffer.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] buf Buffer containing a bitstream.
*
* @param[in] size Buffer size.
*
* @param[in] index Index of the location of the bitstream in the flash.
*            1 and 2 are the supported values for the Perseus.
*
* @param[in] comment Comment to insert into the bitstream "custom" header in flash memory.
*            The comment will only used is the "custom" section of the bitstream header is empty.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result fpgaflash_send(connection_state * state, char * buf, unsigned size, unsigned index, char * comment);


/****************************************************************************
* Function: updateflash_send
************************************************************************//**
*
* Programs the flash memory from the buffer.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] buf Buffer containing a file.
*
* @param[in] size Buffer size.
*
* @param[in] partition Target partition depending of file to flash in memory
*            0 = Kernel
*            1 = CCE
*            2 = U-Boot
*            3 = JFFS2
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result updateflash_send(connection_state * state, char * buf, unsigned size, unsigned partition);

/*****************************************************************************
* Function: updateflash_fromfile_send
*************************************************************************//**
*
* Programs the flash memory from a file. The file name will 
* be used as the comment for the "custom" section of the header.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] name Name of the file containing the data.
*
* @param[in] partition Target partition depending of file to flash in memory
*            0 = Kernel
*            1 = CCE
*            2 = U-Boot
*            3 = JFFS2
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result updateflash_fromfile_send(connection_state * state, char * name, unsigned partition);

/****************************************************************************
* Function: dtbflash_send
************************************************************************//**
*
* Programs the flash memory from the buffer.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] buf Buffer containing a file.
*
* @param[in] size Buffer size.
*
* @param[in] code Special code for production
*
* @param[in] mac_address FPGA's MAC address
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result dtbflash_send(connection_state * state, char * buf, unsigned size, unsigned code, char * mac_address);

/*****************************************************************************
* Function: dtbflash_fromfile_send
*************************************************************************//**
*
* Programs the flash memory from a file. The file name will 
* be used as the comment for the "custom" section of the header.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] name Name of the file containing the data.
*
* @param[in] code Special code for production
*
* @param[in] mac_address FPGA's MAC address
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result dtbflash_fromfile_send(connection_state * state, char * name, unsigned code, char * mac_address);

/****************************************************************************
* Function: fpgaflash_send_with_ID
**************************************************************************//**
*
* Programs FPGA bitstreams in the flash memory from the buffer. 
* An FPGA ID and a memory bank is is required.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] buf Buffer containing a bitstream.
*
* @param[in] size Buffer size.
*
* @param[in] id  ID of FPGA to be flashed
*
* @param[in] bank Memory bank to be used
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result fpgaflash_send_with_ID(connection_state * state, char * buf, unsigned size, unsigned char id, unsigned char bank);

/**
*****************************************************************************
* Function: fpgaflash_fromfile_send
*************************************************************************//**
*
* Programs FPGA bitstream in the flash memory from a file. The file name will 
* be used as the comment for the "custom" section of the bitstream header.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] name Name of the file containing the bitstream data.
*
* @param[in] index Index of the location of the bitstream in the flash.
*            1 and 2 are the supported values for the Perseus.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result fpgaflash_fromfile_send(connection_state * state, char * name, unsigned index);


/****************************************************************************
* Function: fpgaflash_fromfile_send_with_ID
************************************************************************//**
*
* Programs FPGA bitstreams in the flash memory from a file.
* An FPGA ID and a memory bank is is required.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] name Name of the file containing the bitstream data.
*
* @param[in] id  ID of FPGA to be flashed
*
* @param[in] bank Memory bank to be used
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result fpgaflash_fromfile_send_with_ID(connection_state * state, char * name, unsigned char id, unsigned char bank);

/****************************************************************************
* Function: fpgaflash_send
************************************************************************//**
*
* Programs FPGA bitstream in the flash memory from a file.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] buf Buffer containing a bitstream.
*
* @param[in] size Buffer size.
*
* @param[in] index Index of the location of the bitstream in the flash.
*            1 and 2 are the supported values for the Perseus.
*
* @param[in] comment Comment to insert into the bitstream "custom" header in flash memory.
*            The comment will only used is the "custom" section of the bitstream header is empty.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result fpgaflash_fromfile_with_comment_send(connection_state * state, char * name, unsigned index, char * comment);

/****************************************************************************
* Function: fpgaflash_get_info_send
************************************************************************//**
*
* Retrieve, for the bitstream located in flash at the specified index, all its headers.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] index Index of the location of the bitstream in the flash.
*            1 and 2 are the supported values for the Perseus.
*
* @param[in,out] name Pointer to the pointer of a char array. The function will allocate
*            the memory space required to contain the name field of the bitstream header.
*            It will copy the name field in it to return it to the user. It is the user
*            responsibility to free the allocated memory. 
*
* @param[in,out] name_length Pointer the name length variable. The function will return
*            the length of the name char array created through this pointer.
*
* @param[in,out] type Pointer to the pointer of a char array. The function will allocate
*            the memory space required to contain the type field of the bitstream header.
*            It will copy the type field in it to return it to the user. It is the user
*            responsibility to free the allocated memory. 
*
* @param[in,out] type_length Pointer the type length variable. The function will return
*            the length of the type char array created through this pointer.
*
* @param[in,out] date Pointer to the pointer of a char array. The function will allocate
*            the memory space required to contain the date field of the bitstream header.
*            It will copy the date field in it to return it to the user. It is the user
*            responsibility to free the allocated memory. 
*
* @param[in,out] date_length Pointer the date length variable. The function will return
*            the length of the date char array created through this pointer.
*
* @param[in,out] hour Pointer to the pointer of a char array. The function will allocate
*            the memory space required to contain the hour field of the bitstream header.
*            It will copy the hour field in it to return it to the user. It is the user
*            responsibility to free the allocated memory. 
*
* @param[in,out] date_length Pointer the hour length variable. The function will return
*            the length of the hour char array created through this pointer.
*
* @param[in,out] custom Pointer to the pointer of a char array. The function will allocate
*            the memory space required to contain the custom field of the bitstream header.
*            It will copy the custom field in it to return it to the user. It is the user
*            responsibility to free the allocated memory. 
*
* @param[in,out] custom_length Pointer the custom length variable. The function will return
*            the length of the custom char array created through this pointer.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result fpgaflash_get_info_send(connection_state * state, unsigned int index,
    char ** name, unsigned int * name_length,
    char ** type, unsigned int * type_length,
    char ** date, unsigned int * date_length,
    char ** hour, unsigned int * hour_length,
    char ** custom, unsigned int * custom_length);

/****************************************************************************
* Function: fpgaflash_get_index_send
**************************************************************************//**
*
* Return the index of the bitstream loaded at the system boot-up by reading
* the first bitstream configuration in the flash memory.
*
* @param[in,out] state TCP connection state structure
*
* @param[in,out] index Pointer to return the bitstream index of the current
*                configuration. If the index value is 0 or the return Result value 
*                is an error, that mean that the index is undefined.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result fpgaflash_get_index_send(connection_state * state, unsigned int * index);

/****************************************************************************
* Function: fpgaflash_get_index_send
**************************************************************************//**
*
* Set the index of the bitstream to be loaded at the system boot-up by writing
* the first bitstream configuration in the flash memory. A valid bitstream must
* be present in the first bitstream partition of the flash.
*
* @param[in,out] state TCP connection state structure
*
* @param[in] index Index to be written in the bitstream configuration.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result fpgaflash_set_index_send(connection_state * state, unsigned int index);

/****************************************************************************
* Function: memory_read_send
**************************************************************************//**
*
* Reads 32 bits from the MicroBlaze AXI bus..
*
* @param[in,out] state TCP connection state structure
*
* @param[in] addr Physical address on the axi bus
*
* @param[out] data Data
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result memory_read_send(connection_state * state, unsigned addr, unsigned * data);

/****************************************************************************
* Function: custom_register_read_send
**************************************************************************//**
*
* Reads the value of the specified custom register.
*
* @param[in,out] state TCP connection state structure
*
* @param[in] id Channel ID
*
* @param[out] data Data
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result custom_register_read_send(connection_state * state, unsigned id, unsigned * data);

/****************************************************************************
* Function: memory_write_send
*************************************************************************//**
*
* Writes 32 bits to the MicroBlaze AXI bus.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] addr Physical address on the AXI bus..
*
* @param[in] data Data.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result memory_write_send(connection_state * state, unsigned addr, unsigned data);

/****************************************************************************
* Function: memory_write_with_mask_send
*************************************************************************//**
*
* Writes 32 bits masked value to the MicroBlaze AXI bus.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] addr Physical address on the AXI bus.
*
* @param[in] data Data.
*
* @param[in] mask Mask applied to the data. Only the bit set to '1' in the mask will change value.
*                 The bit set to '0' in the mask, will remain unchanged.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result memory_write_with_mask_send(connection_state * state, unsigned addr, unsigned data, unsigned mask);

/****************************************************************************
* Function: custom_register_write_send
*************************************************************************//**
*
* Write the value to the specified custom register.
*
* @param[in,out] state TCP connection state structure.
*
* @param[in] id Channel ID
*
* @param[in] data Data.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result custom_register_write_send(connection_state * state, unsigned id, unsigned data);

/****************************************************************************
* Function: set_turbo_mode
*************************************************************************//**
*
* Do not wait for result before returning. Commands to the CCE always return Success even when failures occur.
*
* @param[in,out] state TCP connection state structure.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result set_turbo_mode(connection_state * state);

/****************************************************************************
* Function: set_normal_mode
*************************************************************************//**
*
* Configures the connection in Normal mode. Commands return normally.
*
* @param[in,out] state TCP connection state structure.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result set_normal_mode(connection_state * state);

/****************************************************************************
* Function: get_mac_address
*************************************************************************//**
*
* Retieves the FPGA's MAC address
*
* @param[in] state TCP connection state structure.
*
* @param[out] macstring Retrieve MAC address.
*
* Error code: #EAPIOK if no problem is found, !=#EAPIOK if problem.
*
****************************************************************************/
Result get_mac_address(connection_state * state, char *macstring);

/****************************************************************************
* Function: get_ip_address
*************************************************************************//**
*
* Retrieves the FPGA's IP address
*
* Return: Pointer to the IP address string
*
****************************************************************************/
char * get_ip_address(void);

 /****************************************************************************
 * NAME : i2c_scan_devices_send
 ************************************************************************//**
 *
 * Scan the specified i2c bus to detect available devices.
 * This function is used for production purpose to check if all devices
 * are detected correctly on the i2c bus.
 *
 * @param [in] state
 *    This structure is the one returned with BusAccess_GetConnectionState().
 *
 * @param [in] nb
 *    Bus to scan (1 for default).
 *
 * @param [out] scanraw
 *    Returns the raw i2c scan results structure (each detected device address in the vector
 *    hold != 0 when a device is detected)
 *
 * @param [out] ndevices
 *    Number of i2c devices detected on the bus.
 *
 * @return   Error code: 0 if no problem is found, != 0 if problem.
 * 
 * @pre
 *    BusAccess_Ethernet_OpenWithoutConnection() (according to media used). 
 *    BusAccess_GetConnectionState().
 *     
 * Example: 
 * @code
 * @endcode  
 *
 * @see
 *
 ***************************************************************************/ 
Result i2c_scan_devices_send(connection_state * state, unsigned nb, I2C_stI2cScan *scanraw,
                             unsigned char *ndevices);


/** private */
//void disconnect_ipsocket(int stream);

/**
*****************************************************************************
*
* EAPI error code
*
****************************************************************************/
enum EAPI_error {
    EAPIOK       = MAKE_RESULT(LYR_SEVERITY_SUCCESS, MODULE_ADP_EAPI, 0), /**< No problem. */
    EAPICONNECT  = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 1),   /**< Cannot connect to CCE. */
    EAPISOCKET   = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 2),   /**< Cannot create TCP socket. */
    EAPIFILE     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 3),   /**< Cannot access file, file not found .*/
    EAPIMMAP     = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 4),   /**< mmap call failed. */
    EAPIDISCONNECT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 5), /**< CCE connection terminated. */
    EAPIOUTOFSYNC = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 6),  /**< TCP stream to CCE is out of synch. */
    EAPIWSA      = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 7),   /**< Error while starting WSA. */
    EAPINOMEM    = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 8),   /**< Not enough available memory */
	EAPIFILETOOLARGE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 9), /**< The file is too large */
	EAPICCEVERSIONMISMATCH = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 10), /**< The cce version doesn't match the EAPI version */
	EAPICCELIMITEDACCESS = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 11), /**< The cce version doesn't match the EAPI version but a limited access to the CCE is possible */
	EAPICARRIERLIMITEDACCESS = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_ADP_EAPI, 12), /**< The carrier board has not been correctly detected but a limited access to the CCE is possible */
};

#ifdef __cplusplus
}
#endif


#endif
