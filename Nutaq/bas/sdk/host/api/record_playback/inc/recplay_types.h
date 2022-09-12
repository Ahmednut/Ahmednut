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
 * @file       adp_recplay_types.h
 * @brief      Record Playback type definitions
 *
 * $Date: 2015/08/10 17:10:40 $
 * $Revision: 1.1 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RECORDPLAYBACK
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

#ifndef ADP_RECPLAY_STRUCT_H
#define ADP_RECPLAY_STRUCT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bas_types.h"
#include "connection_state.h"

/** @anchor RECPLAY_trigdelay_range
 *  @name RECPLAY_trigdelay_range
 *    
 *  Trigger delay possible range.
 * 
 *  This gives the range of possible values [MIN,MAX] for trigger delay.   
 */
//@{ 
#define RECPLAY_TRIGGERADDRMASK_1G 0x7FFFFFF    ///< Trigger addr mask (for 1G mem)
#define RECPLAY_TRIGGERADDRMASK_4G 0x1FFFFFFF   ///< Trigger addr mask (for 4G mem)
//@}


/** @name Trigger choices
 * 
 *  List all trigger choices available for the Record/Playback module.
 */
typedef enum
{
    eRecPlayTrigExternal = 0, /** Use the trigger port in the FPGA recording module to send a trigger. */
    eRecPlayTrigSoft = 1,     /** The trigger is sent by a host function call to the FPGA recording module. */
    erecPlayTrigNone = 2      /** No trigger. Start immediately. */
}eRecPlayTrigSrc;

/** @name DDR3mMemory size
 * 
 *  List all ddr3 memory size supported by the Record/Playback module.
 */
typedef enum{
    RECPLAY_DDR3SIZE_UNDEFINED = 0, ///< DDR3 size is undefined
    RECPLAY_DDR3SIZE_1G  = 1,  ///< DDR3 size is 1G
    RECPLAY_DDR3SIZE_4G  = 4}RECPLAY_DDR3Size; ///< DDR3 size is 4G

/** @name Record/Playback object structure
 * 
 *  Holds Bus Access media and connection
 */
typedef struct
{
    connection_state * state;                                                  ///<   Handle to associated connection state object 
    uint64_t u64RecPlayFpgaBaseAdd;                                            ///<   Record/Playback module base address  
    unsigned int uRecordModeValue;                                             ///<   Record mode value
    unsigned int uPlayBackSingleModeValue;                                     ///<   Single playback mode value 
    unsigned int uPlayBackContinuousModeValue;                                 ///<   Continuous playback mode value 
    unsigned int uRtdexMem2HostModeValue;                                      ///<   Mem to host mode value 
    unsigned int uRtdexHost2MemModeValue;                                      ///<   Host to mem mode value 
    RECPLAY_DDR3Size DDR3Size;                                                 ///<   DDR3 Size

}sRecPlayStruct;

typedef sRecPlayStruct* adp_handle_recplay;

/** @name Record/Playback error codes
 * 
 *  List all runtime related error codes of a Record/Playback module.
 */
enum RECPLAY_error {
    RECPLAYOK = 0,  /**<No error*/
    RECPLAYINVHANDLE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 1),     ///< Record/Playback handle used is invalid
    RECPLAYMEMALLOC = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 2),      ///< Error allocating memory for Record/Playback object
    RECPLAYINVARGSIZE = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 3),    ///< Invalid size is used
    RECPLAYINVTRIGOFFSET = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 4), ///< Invalid trigger offset calculated
    RECPLAYINVDELAY = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 5),      ///< Invalid trigger delay configured
    RECPLAYWRONGCOREID = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 6),   ///< Wrong core ID read
    RECPLAYINVSTARTADDR = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 7),  ///< Invalid start address
	RECPLAYSTATUSTIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 8), ///< Getting status has timeout
	RECPLAYINVNUMPORT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 9),    ///< Invalid number of ports
	RECPLAYINVDATAPORT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 10),  ///< Invalid ports width
	RECPLAYINVDDR3 = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 11),      ///< Invalid DDR3 size
	RECPLAYTRANSFERTIMEOUT = MAKE_RESULT(LYR_SEVERITY_ERROR, MODULE_RECPLAY, 12) ///< The waiting process has time outed before the transfer was done
    };
    
    
// Core ID
#define RECPLAY_ACCESS_COREID         0xEA00               

// Core registers
#define RECPLAY_ACCESS_COREID_INDEX         0x0
#define RECPLAY_ACCESS_CONTROL_INDEX         0x4
#define RECPLAY_ACCESS_SETMODE_INDEX         0x8
#define RECPLAY_ACCESS_MODEVALUE_INDEX         0xC
#define RECPLAY_ACCESS_SETSTARTADDR_INDEX     0x10
#define RECPLAY_ACCESS_STARTADDR_INDEX         0x14
#define RECPLAY_ACCESS_DELAYVAL_INDEX         0x18
#define RECPLAY_ACCESS_SETTRANSFERSIZE_INDEX     0x1C
#define RECPLAY_ACCESS_TRANSFERSIZE_INDEX     0x20
#define RECPLAY_ACCESS_TRIGGERADDR_INDEX     0x24
#define RECPLAY_ACCESS_TRIGGERADDROFFSET_INDEX     0x28
#define RECPLAY_ACCESS_TRANSFEROVER_INDEX     0x2C
#define RECPLAY_ACCESS_MODERECORD_INDEX     0x30
#define RECPLAY_ACCESS_MODEPLAYBACKSINGLE_INDEX 0x34
#define RECPLAY_ACCESS_MODEPLAYBACKCONTINOUS_INDEX 0x38
#define RECPLAY_ACCESS_MODERTDEXMEM2HOST_INDEX     0x3C
#define RECPLAY_ACCESS_MODERTDEXHOST2MEM_INDEX     0x40
#define RECPLAY_ACCESS_PFULLASSERT_INDEX     0x4C
#define RECPLAY_ACCESS_PFULLNEGATE_INDEX     0x50
#define RECPLAY_ACCESS_PARITY_INDEX         0x54
#define RECPLAY_ACCESS_NBRECPORT_INDEX         0x58
#define RECPLAY_ACCESS_NBPLAYPORT_INDEX     0x5C
#define RECPLAY_ACCESS_RECPORTWIDTH_INDEX     0x60
#define RECPLAY_ACCESS_PLAYPORTWIDTH_INDEX     0x64
#define RECPLAY_ACCESS_RECTRIGMUX_INDEX     0x68
#define RECPLAY_ACCESS_RECSOFTTRIG_INDEX     0x6C
#define RECPLAY_ACCESS_PLAYTRIGMUX_INDEX     0x74
#define RECPLAY_ACCESS_PLAYSOFTTRIG_INDEX     0x78
#define RECPLAY_ACCESS_DDR3SIZE_INDEX         0x7C
#define RECPLAY_ACCESS_RECORDSTORAGECNT_INDEX     0x80
#define RECPLAY_ACCESS_RTDEXSTORAGECNT_INDEX     0x84
#define RECPLAY_ACCESS_PLAYBACKREADCNT_INDEX     0x88
#define RECPLAY_ACCESS_RTDEXREADCNT_INDEX         0x8C


#ifdef __cplusplus
}
#endif

#endif



