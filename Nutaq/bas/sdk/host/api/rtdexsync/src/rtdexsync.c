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
 *          I N N O V A T I O N  T O D A Y  F O R  T O M O R R O W       **** 
 *                                                                        ***       
 * 
 ************************************************************************//**    
 *
 * @file    rtdexsync.c
 * @brief   Implement synchronisation over RTDEx. 
 *
 * $Date: 2015/08/26 19:07:32 $
 * $Revision: 1.2 $
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.              
 *
 * @ingroup RTDEXSYNC
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

/****************************************************************************
 *                                  Includes                                *
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Misc ADP definitions
#include "bas_types.h"
#include "bas_error.h"
#include "media_types.h"

// External API (CCE interface)        
#include "eapi.h"                       // Requires "Ws2_32.lib" on Windows
        
// Connection state (i.e. AMC card)
#include "mac_utils.h"                  // Ethernet utils (this one is not a library, 
                                        // "mac_utils.c" needs to be included in the project !!)
#include "media_utils.h"                

// CCE function
#include "cce.h"
#include "eapi_serialize.h"

// RAW RTDEx interface
#include "rtdex_types.h"
#include "rtdex_media.h"
#include "rtdex.h"
#include "rtdex_eth.h"              // Requires "Iphlpapi.lib(GetAdaptersInfo)" 
#include "rtdex_pci.h"              // and "Winmm.lib(timeGetTime)" on Windows

// RTDExSync module includes
#include "rtdexsync.h"
#include "rtdexsync_def.h"
#include "rtdexsync_internal.h"
#include "rtdexsync_vita49.h"


/****************************************************************************
 *                            Private constants                             * 
 ****************************************************************************/ 

// Define to activate debug traces
//#define RTDExSync_DBG   1

/**
 * RTDExSync Receive Timeout (in ms)
 */
#define RTDExSync_RX_TIMEOUT    10

/**
 * Inter-RTDEx frames delay.
 * Normally, the RTDExSYnc interface will be cadenced by the ADC, 
 * therefore no intentionnaly gap is required
 */
#define RTDEX_ETH_FRAME_GAP     0      

/**
 * Maximum RTDEx Ethernet frame size
 */
#define RTDEX_ETH_MAX_FRAME_SIZE 8980


// polynomial: 0x104C11DB7
static const uint32_t crc32_table[256] = {
	0x00000000U,0x04C11DB7U,0x09823B6EU,0x0D4326D9U, 0x130476DCU,0x17C56B6BU,0x1A864DB2U,0x1E475005U,
	0x2608EDB8U,0x22C9F00FU,0x2F8AD6D6U,0x2B4BCB61U, 0x350C9B64U,0x31CD86D3U,0x3C8EA00AU,0x384FBDBDU,
	0x4C11DB70U,0x48D0C6C7U,0x4593E01EU,0x4152FDA9U, 0x5F15ADACU,0x5BD4B01BU,0x569796C2U,0x52568B75U,
	0x6A1936C8U,0x6ED82B7FU,0x639B0DA6U,0x675A1011U, 0x791D4014U,0x7DDC5DA3U,0x709F7B7AU,0x745E66CDU,
	0x9823B6E0U,0x9CE2AB57U,0x91A18D8EU,0x95609039U, 0x8B27C03CU,0x8FE6DD8BU,0x82A5FB52U,0x8664E6E5U,
	0xBE2B5B58U,0xBAEA46EFU,0xB7A96036U,0xB3687D81U, 0xAD2F2D84U,0xA9EE3033U,0xA4AD16EAU,0xA06C0B5DU,
	0xD4326D90U,0xD0F37027U,0xDDB056FEU,0xD9714B49U, 0xC7361B4CU,0xC3F706FBU,0xCEB42022U,0xCA753D95U,
	0xF23A8028U,0xF6FB9D9FU,0xFBB8BB46U,0xFF79A6F1U, 0xE13EF6F4U,0xE5FFEB43U,0xE8BCCD9AU,0xEC7DD02DU,
	0x34867077U,0x30476DC0U,0x3D044B19U,0x39C556AEU, 0x278206ABU,0x23431B1CU,0x2E003DC5U,0x2AC12072U,
	0x128E9DCFU,0x164F8078U,0x1B0CA6A1U,0x1FCDBB16U, 0x018AEB13U,0x054BF6A4U,0x0808D07DU,0x0CC9CDCAU,
	0x7897AB07U,0x7C56B6B0U,0x71159069U,0x75D48DDEU, 0x6B93DDDBU,0x6F52C06CU,0x6211E6B5U,0x66D0FB02U,
	0x5E9F46BFU,0x5A5E5B08U,0x571D7DD1U,0x53DC6066U, 0x4D9B3063U,0x495A2DD4U,0x44190B0DU,0x40D816BAU,
	0xACA5C697U,0xA864DB20U,0xA527FDF9U,0xA1E6E04EU, 0xBFA1B04BU,0xBB60ADFCU,0xB6238B25U,0xB2E29692U,
	0x8AAD2B2FU,0x8E6C3698U,0x832F1041U,0x87EE0DF6U, 0x99A95DF3U,0x9D684044U,0x902B669DU,0x94EA7B2AU,
	0xE0B41DE7U,0xE4750050U,0xE9362689U,0xEDF73B3EU, 0xF3B06B3BU,0xF771768CU,0xFA325055U,0xFEF34DE2U,
	0xC6BCF05FU,0xC27DEDE8U,0xCF3ECB31U,0xCBFFD686U, 0xD5B88683U,0xD1799B34U,0xDC3ABDEDU,0xD8FBA05AU,
	0x690CE0EEU,0x6DCDFD59U,0x608EDB80U,0x644FC637U, 0x7A089632U,0x7EC98B85U,0x738AAD5CU,0x774BB0EBU,
	0x4F040D56U,0x4BC510E1U,0x46863638U,0x42472B8FU, 0x5C007B8AU,0x58C1663DU,0x558240E4U,0x51435D53U,
	0x251D3B9EU,0x21DC2629U,0x2C9F00F0U,0x285E1D47U, 0x36194D42U,0x32D850F5U,0x3F9B762CU,0x3B5A6B9BU,
	0x0315D626U,0x07D4CB91U,0x0A97ED48U,0x0E56F0FFU, 0x1011A0FAU,0x14D0BD4DU,0x19939B94U,0x1D528623U,
	0xF12F560EU,0xF5EE4BB9U,0xF8AD6D60U,0xFC6C70D7U, 0xE22B20D2U,0xE6EA3D65U,0xEBA91BBCU,0xEF68060BU,
	0xD727BBB6U,0xD3E6A601U,0xDEA580D8U,0xDA649D6FU, 0xC423CD6AU,0xC0E2D0DDU,0xCDA1F604U,0xC960EBB3U,
	0xBD3E8D7EU,0xB9FF90C9U,0xB4BCB610U,0xB07DABA7U, 0xAE3AFBA2U,0xAAFBE615U,0xA7B8C0CCU,0xA379DD7BU,
	0x9B3660C6U,0x9FF77D71U,0x92B45BA8U,0x9675461FU, 0x8832161AU,0x8CF30BADU,0x81B02D74U,0x857130C3U,
	0x5D8A9099U,0x594B8D2EU,0x5408ABF7U,0x50C9B640U, 0x4E8EE645U,0x4A4FFBF2U,0x470CDD2BU,0x43CDC09CU,
	0x7B827D21U,0x7F436096U,0x7200464FU,0x76C15BF8U, 0x68860BFDU,0x6C47164AU,0x61043093U,0x65C52D24U,
	0x119B4BE9U,0x155A565EU,0x18197087U,0x1CD86D30U, 0x029F3D35U,0x065E2082U,0x0B1D065BU,0x0FDC1BECU,
	0x3793A651U,0x3352BBE6U,0x3E119D3FU,0x3AD08088U, 0x2497D08DU,0x2056CD3AU,0x2D15EBE3U,0x29D4F654U,
	0xC5A92679U,0xC1683BCEU,0xCC2B1D17U,0xC8EA00A0U, 0xD6AD50A5U,0xD26C4D12U,0xDF2F6BCBU,0xDBEE767CU,
	0xE3A1CBC1U,0xE760D676U,0xEA23F0AFU,0xEEE2ED18U, 0xF0A5BD1DU,0xF464A0AAU,0xF9278673U,0xFDE69BC4U,
	0x89B8FD09U,0x8D79E0BEU,0x803AC667U,0x84FBDBD0U, 0x9ABC8BD5U,0x9E7D9662U,0x933EB0BBU,0x97FFAD0CU,
	0xAFB010B1U,0xAB710D06U,0xA6322BDFU,0xA2F33668U, 0xBCB4666DU,0xB8757BDAU,0xB5365D03U,0xB1F740B4U
};


/****************************************************************************
 *                                  Macros                                  * 
 ****************************************************************************/

/**
 * Macros for manipulatind fields
 */
#define SET_FIELD(FIELD,VAL)    (((VAL) & FIELD##_MASK) << FIELD##_SHIFT) 
#define GET_FIELD(FIELD,VAL)    (((VAL) >> FIELD##_SHIFT) & FIELD##_MASK)  


/****************************************************************************
 *                            Private variables                             * 
 ****************************************************************************/


/****************************************************************************
 *                       Private function prototypes                        *
 ****************************************************************************/
static adp_result_t RTDExSync_Presence_send( connection_state * state );
static adp_result_t RTDExSync_CoreReset_send( connection_state * state );
static adp_result_t RTDExSync_ChannelReset_send( connection_state * state, uint8_t u8Channel );
static adp_result_t RTDExSync_ChannelStart_send( connection_state * state, uint8_t u8Channel, uint32_t u32RxCtrl, uint32_t u32TxCtrl, uint32_t u32TxCntxCtrl );
static adp_result_t RTDExSync_ChannelStop_send( connection_state * state, uint8_t u8Channel );
static adp_result_t RTDExSync_ChannelInfo_send( connection_state * state, uint8_t u8Channel, int * pPresent, int * pDir );

static adp_result_t RTDExSync_SetupRTDEx( RTDExSync_Handle_t hRTDExSync );
static uint32_t RTDExSync_Crc32( uint32_t crc, 
                                 uint32_t *pu32Data, 
                                 int len);
static adp_result_t RTDExSync_EncodeVrtPacket( RTDExSync_VrtPacket_t *pVrtPkt, 
                                      uint32_t *pu32Buff,
                                      uint32_t  u32BufSize );
static adp_result_t RTDExSync_DecodeVrtPacket( uint32_t *pu32EncPkt,
                                               uint32_t u32MaxPktSize,
                                               RTDExSync_VrtPacket_t *pDecPacket );
static adp_result_t RTDExSync_Monitor( RTDExSync_Handle_t hRTDExSync, 
                                       uint32_t           u32EvtQueueSize,
                                       RTDExSync_Event_t  *pEvtQueue,
                                       uint32_t           *pu32EvtCount,
                                       uint32_t           u32TimeoutMs );
static adp_result_t  RTDExSync_GetLocalMacAddr( connection_state state,
                                                uint8_t pu8HostMacAddr[6] );

/****************************************************************************
 *                             Public functions                             *
 ****************************************************************************/

/****************************************************************************
 * Function: RTDExSync_Reset
 *****************************************************************************
 *
 * Globally reset the RTDExSync core
 *
 * @param [in]  state
 *      Pointer to the connection state with the .
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_ResetCore( connection_state * state )
{
    return RTDExSync_CoreReset_send( state );
}

/****************************************************************************
 * Function: RTDExSync_GetChInfo
 *****************************************************************************
 *
 * Get the info about a RTDExSync channel.
 *
 * @param [in]  state
 *      Pointer to the connection state with the CCE.
 *
 * @param [in]  u8Channel
 *      RTDExSync channel number
 *
 * @param [out]  pPresent
 *      Presence flag (0:not present, 1:present)
 *
 * @param [out]  pDir (0:uplink, 1:downlink ())
 *      Channel direction.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_GetChInfo( connection_state * state,
                                  uint8_t u8Channel,
                                  int *pPresent, 
                                  int *pDir )
{
    return RTDExSync_ChannelInfo_send(state, u8Channel, pPresent, pDir);
}

/****************************************************************************
 * Function: RTDExSync_Open
 *****************************************************************************
 *
 * Open RTDExSync instance
 *
 * @param [in]  state
 *      Connection state between the host and the CCE.
 *
 * @param [in]  u8ChIdx;
 *      Channel to be configured. 
 *
 * @param [in]  dir
 *      Direction of transfer is either 0:uplink (Radio -> Host) or 1:downlink 
 *      (Host -> Radio). Note that the direction is determined at build time in
 *      the FPGA of the radio.
 *
 * @param [in]  u32RTDExEthBaseAddr
 *      Base address of the RTDEx Ethernet core in the target FPGA. This value
 *      is don't care if the RTDEx core is mapped on the PCIe bus. It is usually 
 *      set to 'module_rtdex_eth_base' from the file 'memory_map.h'.
 *
 * @param [in]  u32SubframeSize
 *      RTDExSync subframe size in bytes. The maximum depends on the media capacity).
 *      Maximum is 8980 bytes on Ethernet using jumbo frames. On PCIe, it controls 
 *      the CPU interrupt rates.
 *
 * @param [in]  u32FrameSize
 *      RTDExSync frame size in bytes. It must be a multiple of the subframe size.
 *      Each RTDExSync data transfer will transfer this quantity of bytes between 
 *      the host and the radio even if the frame is not full. If more data than what 
 *      can be transfer in one frame is requested, it will be split in multiple frames.
 *
 * @param [out]  pu32MaxIQSamples
 *      Address of a variable that receives the maximum number of IQ samples
 *      that can be sent or received per RTDExSync frame.
 *
 * @param [out]  phRTDExSync
 *      Pointer to RTDExSync handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_Open( connection_state state,
                             uint8_t u8ChIdx, 
                             RTDExSync_Dir_t dir, 
                             uint32_t u32RTDExEthBaseAddr,
                             uint32_t u32SubframeSize,
                             uint32_t u32FrameSize,
                             uint32_t *pu32MaxIQSamples,
                             RTDExSync_Handle_t *phRTDExSync )
{
    Result res = 0;
    int chDir;
    int chPresent; 

    // Make sure the handle is NULL in case something goes wrong
    *phRTDExSync = NULL;
    *pu32MaxIQSamples = 0;

    // Validate the arguments
    if ( u32SubframeSize & 3 )
    {
        return RTDEXSYNCINVSUBFRSIZE;
    }
    if ( u32FrameSize % u32SubframeSize )
    {
        return RTDEXSYNCINVFRSIZE;
    }


    /*
     * Alloc and initialize the RTDEx instance structure
     */    
	*phRTDExSync = (RTDExSync_Handle_t)malloc( sizeof(RTDExSync_Obj_t) );
	if( *phRTDExSync == NULL ) return RTDEXSYNCOUTOFMEMORY;
    
    memset( *phRTDExSync, 0, sizeof(RTDExSync_Obj_t) );
    
    // Save the configuration
    (*phRTDExSync)->ch.u8Idx = u8ChIdx;
    (*phRTDExSync)->u32RTDExEthBaseAddr  = u32RTDExEthBaseAddr;


    // Set connection state
    (*phRTDExSync)->state = state;

    /*
     * Connect to the Perseus
     */
    //res = BusAccess_Ethernet_OpenWithConnection( &(*phRTDExSync)->hBusAccess, &state, 0 );  
    //if ( FAILURE(res) ) goto error;

    /*
     * Make sure the RTDExSync core is present and configured in the same direction 
     */
    res = RTDExSync_ChannelInfo_send( &(*phRTDExSync)->state, u8ChIdx, &chPresent, &chDir );
    if ( FAILURE(res) ) goto error;

    if ( !chPresent)
    {
        res = RTDEXSYNCCHANNELABSENT;
        goto error; 
    }

    if ( chDir != dir )
    {
        res = RTDEXSYNCINVDIR;
        goto error; 
    }
    (*phRTDExSync)->ch.dir = dir;


    /*
     *  Determine the maximum frame size in each direction
     */
    switch ( dir )
    {
        case RTDExSync_Dir_Uplink:      
        {
            // TX: context packet
            (*phRTDExSync)->tx.u22MaxFrameSize = (VITA49_VRL_SIZE + VITA49_UL_VRT_DATA_SIZE(0)) * sizeof(uint32_t);

			// PCIe must be multiple of 8 bytes
			if((*phRTDExSync)->tx.u22MaxFrameSize % 8)
				(*phRTDExSync)->tx.u22MaxFrameSize = (*phRTDExSync)->tx.u22MaxFrameSize + (8 - (*phRTDExSync)->tx.u22MaxFrameSize % 8);

            // Set the media frame size the same size than the TX VRL frame
            (*phRTDExSync)->tx.u32MediaFrameSize = (*phRTDExSync)->tx.u22MaxFrameSize;


            // RX: data packet 
            (*phRTDExSync)->rx.u22MaxFrameSize = u32FrameSize;
            if ( (*phRTDExSync)->rx.u22MaxFrameSize < (VITA49_VRL_SIZE + VITA49_VRT_CTX_SIZE + VITA49_VRT_ECTX_SIZE) * sizeof(uint32_t) )
            {
                // If the data packet size is smaller than a context packet and a econtext packet, use the latter size instead
                (*phRTDExSync)->rx.u22MaxFrameSize = (VITA49_VRL_SIZE + VITA49_VRT_CTX_SIZE + VITA49_VRT_ECTX_SIZE) * sizeof(uint32_t);
            }
			// PCIe must be multiple of 8 bytes
			if((*phRTDExSync)->rx.u22MaxFrameSize % 8)
				(*phRTDExSync)->rx.u22MaxFrameSize = (*phRTDExSync)->rx.u22MaxFrameSize + (8 - (*phRTDExSync)->rx.u22MaxFrameSize % 8);

            // Make sure it can fit in a VRT frame (limited to a 16-bit value in words)
            if ( (*phRTDExSync)->rx.u22MaxFrameSize > (0x0FFFFu * sizeof(uint32_t)) )
            {
		        res = RTDEXSYNCINVFRSIZE;
                goto error;
            }

            (*phRTDExSync)->rx.u32MediaFrameSize = u32SubframeSize;

            // Convert the media frame size and the max number of frames per transfer in number od IQ samples
            *pu32MaxIQSamples = ((*phRTDExSync)->rx.u22MaxFrameSize / (2*sizeof(uint16_t))) - VITA49_VRL_SIZE - VITA49_UL_VRT_DATA_SIZE(0);
            break;
        }
        case RTDExSync_Dir_Downlink:
        {
            // RX: context packet and extended context packet
            (*phRTDExSync)->rx.u22MaxFrameSize = (VITA49_VRL_SIZE + VITA49_VRT_CTX_SIZE + VITA49_VRT_ECTX_SIZE) * sizeof(uint32_t);

			// PCIe must be multiple of 8 bytes
			if((*phRTDExSync)->rx.u22MaxFrameSize % 8)
				(*phRTDExSync)->rx.u22MaxFrameSize = (*phRTDExSync)->rx.u22MaxFrameSize + (8 - (*phRTDExSync)->rx.u22MaxFrameSize % 8);

            // Set the media frame size the same size than the RX VRL frame
            (*phRTDExSync)->rx.u32MediaFrameSize = (*phRTDExSync)->rx.u22MaxFrameSize;


            // TX: data packet
            (*phRTDExSync)->tx.u22MaxFrameSize = u32FrameSize;

			// PCIe must be multiple of 8 bytes
			if((*phRTDExSync)->tx.u22MaxFrameSize % 8)
				(*phRTDExSync)->tx.u22MaxFrameSize = (*phRTDExSync)->tx.u22MaxFrameSize + (8 - (*phRTDExSync)->tx.u22MaxFrameSize % 8);

            // Make sure it can fit in a VRT frame (limited to a 16-bit value in words)
            if ( (*phRTDExSync)->tx.u22MaxFrameSize > (0x0FFFFu * sizeof(uint32_t)) )
            {
		        res = RTDEXSYNCINVFRSIZE;
                goto error;
            }

            (*phRTDExSync)->tx.u32MediaFrameSize = u32SubframeSize;

            // Convert the media frame size and the max number of frames per transfer in number od IQ samples
            *pu32MaxIQSamples = ((*phRTDExSync)->tx.u22MaxFrameSize / (2*sizeof(uint16_t))) - VITA49_VRL_SIZE - VITA49_DL_VRT_DATA_SIZE(0);
            break;
        }
        default: break; // Impossible !!           
    }
    (*phRTDExSync)->u32MaxIQSamples = *pu32MaxIQSamples;


    /*
     * Alloc the buffer for the transfers
     */
    (*phRTDExSync)->tx.pu32Buffer = (uint32_t *)malloc( (*phRTDExSync)->tx.u22MaxFrameSize );
    if ( (*phRTDExSync)->tx.pu32Buffer == NULL )
    {
        res = RTDEXSYNCOUTOFMEMORY;
        goto error;
    }
    (*phRTDExSync)->rx.pu32Buffer = (uint32_t *)malloc( (*phRTDExSync)->rx.u22MaxFrameSize );
    if ( (*phRTDExSync)->rx.pu32Buffer == NULL )
    {
        res = RTDEXSYNCOUTOFMEMORY;
        goto error;
    }


    /*
     * Setup the associated RTDEx channels
     */

    // Put the RTDExSync channel in reset
    res = RTDExSync_ChannelReset_send( &(*phRTDExSync)->state, 
                                       (*phRTDExSync)->ch.u8Idx );
	if( FAILURE(res) ) goto error;

    // And setup the RTDEx channels
    res = RTDExSync_SetupRTDEx( *phRTDExSync );
    if ( FAILURE(res) ) goto error; 
    /*
     * Init and activate the RTDExSync channels
     */
    res = RTDExSync_ChannelStart_send( &(*phRTDExSync)->state, 
                                       (*phRTDExSync)->ch.u8Idx,
                                       (*phRTDExSync)->tx.u22MaxFrameSize,
                                       (*phRTDExSync)->rx.u22MaxFrameSize,
                                       (*phRTDExSync)->rx.u32MediaFrameSize );
	if( FAILURE(res) ) goto error;

    /*
     * Channel is ready
     */
    (*phRTDExSync)->ch.localState = RTDExSync_ChState_Idle;
    return res;

error:
    /*
     * Close the interfaces
     */
    if ( (*phRTDExSync)->tx.hRTDEx != NULL )
    {
        RTDExClose( (*phRTDExSync)->tx.hRTDEx );
        (*phRTDExSync)->tx.hRTDEx = NULL;
    }
    if ( (*phRTDExSync)->rx.hRTDEx != NULL )
    {
        RTDExClose( (*phRTDExSync)->rx.hRTDEx );
        (*phRTDExSync)->rx.hRTDEx = NULL;
    }
    /*
    if ( (*phRTDExSync)->hBusAccess != NULL )
    {
        BusAccess_Ethernet_Close( (*phRTDExSync)->hBusAccess );
        (*phRTDExSync)->hBusAccess = NULL;
    }*/


    /*
     * Deallocate the resources
     */
    if ( (*phRTDExSync)->rx.pu32Buffer != NULL )
    {
        free( (*phRTDExSync)->rx.pu32Buffer );
	    (*phRTDExSync)->rx.pu32Buffer = NULL;
    }
    if ( (*phRTDExSync)->tx.pu32Buffer != NULL )
    {
        free( (*phRTDExSync)->tx.pu32Buffer );
	    (*phRTDExSync)->tx.pu32Buffer = NULL;
    }
    if ( *phRTDExSync != NULL )
    {
	    free( *phRTDExSync );
        *phRTDExSync = NULL;
    }
	return res;
}


/****************************************************************************
 * Function: RTDExSync_Close
 *****************************************************************************
 *
 * Close RTDExSync instance
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @return
 *      None.
 *
 ****************************************************************************/
void RTDExSync_Close( RTDExSync_Handle_t hRTDExSync )
{
    if ( hRTDExSync != NULL )
    {
        /*
         * Close the interfaces
         */
        if ( hRTDExSync->tx.hRTDEx != NULL )
        {
            RTDExClose( hRTDExSync->tx.hRTDEx );
            hRTDExSync->tx.hRTDEx = NULL;
        }
        if ( hRTDExSync->rx.hRTDEx != NULL )
        {
            RTDExClose( hRTDExSync->rx.hRTDEx );
            hRTDExSync->rx.hRTDEx = NULL;
        }
        /*
        if ( hRTDExSync->hBusAccess != NULL )
        {
            BusAccess_Ethernet_Close( hRTDExSync->hBusAccess );
            hRTDExSync->hBusAccess = NULL;
        }*/

        /*
         * Deallocate the resources
         */
        if ( hRTDExSync->rx.pu32Buffer != NULL )
        {
            free( hRTDExSync->rx.pu32Buffer );
	        hRTDExSync->rx.pu32Buffer = NULL;
        }
        if ( hRTDExSync->tx.pu32Buffer != NULL )
        {
            free( hRTDExSync->tx.pu32Buffer );
	        hRTDExSync->tx.pu32Buffer = NULL;
        }
	    free( hRTDExSync );
        hRTDExSync = NULL;
    }
}

/****************************************************************************
 * Function: RTDExSync_Start
 *****************************************************************************
 *
 * Program the start mode of a new stream of transmission. In downlink, the 
 * start command will be sent along the first dta packet (RTDExSync_Send).
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @param [in]  mode
 *      Start mode.
 *
 * @param [in]  trigSel
 *      Trigger selection (0, 1, 2, 3). Only used for OnTrigger mode.
 *
 * @param [in]  u64StartTime
 *      Absolute start time in PPS Sync tick count. Only used for AtTime mode.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_Start( RTDExSync_Handle_t hRTDExSync,
                              RTDExSync_Mode_t mode,
                              RTDExSync_TrigSel_t trigSel,
                              uint64_t u64StartTime )
{
    Result res = RTDEXSYNCOK;

    // Valide the arguments
    if ((uint32_t)mode >= (uint32_t)RTDExSync_TrigMode_Num)
    {
        // Invalid mode for start
        return RTDEXSYNCINVSTARTMODE;
    }
    if ( (uint32_t)trigSel >= (uint32_t)RTDExSync_TrigSel_Num )
    {
        // Invalid trigger selection
        return RTDEXSYNCINVTRIGSEL;
    }
    if ( (uint32_t)mode == (uint32_t)RTDExSync_TrigMode_AtTime &&
        u64StartTime <= 3)
    {
        // Invalid time
        // The time must be at least higher than 3
        // since there is a subtraction of 3 done in the FPGA
        return RTDEXSYNCINVTIME;
    }

    switch ( hRTDExSync->ch.localState )
    {
        case RTDExSync_ChState_Idle:
        case RTDExSync_ChState_Stopped:
        {
            hRTDExSync->start.mode        = mode;
            hRTDExSync->start.trigSel     = trigSel;
            hRTDExSync->start.u64TickTime = u64StartTime;
                
            hRTDExSync->ch.localState     = RTDExSync_ChState_Armed;

            // Reset the sample count
            hRTDExSync->tx.u32SampleCount = 0;
            hRTDExSync->rx.u32SampleCount = 0;

            // Increase the session ID at every start                    
            hRTDExSync->ch.u16SessionId += 1; 

            // Send the command now if we are receiving (uplink)
            if ( hRTDExSync->ch.dir == RTDExSync_Dir_Uplink )
            {
                res = RTDExSync_Send( hRTDExSync, NULL, 0, 0, NULL, NULL );
            }
            break;
        }
        default:
        {
		    res = RTDEXSYNCINVSTATE;
            break;
        }
    }
    return res;
}

/****************************************************************************
 * Function: RTDExSync_Stop
 *****************************************************************************
 *
 * Program the stop mode of a transmission stream. In downlink, the stop command 
 * will only be sent in the next data packet when the mode is OnTrigger or AtTime. 
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @param [in]  mode
 *      Stop mode.
 *
 * @param [in]  trigSel
 *      Trigger selection (0, 1, 2, 3). Only used for OnTrigger mode.
 *
 * @param [in]  u64StopTime
 *      Absolute time in clock cycle counts. Only used for AtTime mode.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_Stop( RTDExSync_Handle_t hRTDExSync,
                             RTDExSync_Mode_t mode,
                             RTDExSync_TrigSel_t trigSel,
                             uint64_t u64StopTime )
{
    Result res = RTDEXSYNCOK;

    // Validate the arguments
    if ( (uint32_t)mode >= (uint32_t)RTDExSync_TrigMode_Num ) 
    {
        // Invalid mode for stop
        return RTDEXSYNCINVSTOPMODE;
    }
    if ( (uint32_t)trigSel >= (uint32_t)RTDExSync_TrigSel_Num )
    {
        // Invalid trigger selection
        return RTDEXSYNCINVTRIGSEL;
    }
    
    if ( (uint32_t)mode == (uint32_t)RTDExSync_TrigMode_AtTime &&
        u64StopTime <= 3)
    {
        // Invalid time
        // The time must be at least higher than 3
        // since there is a subtraction of 3 done in the FPGA
        return RTDEXSYNCINVTIME;
    }

    switch ( hRTDExSync->ch.localState )
    {
        case RTDExSync_ChState_Armed:
        case RTDExSync_ChState_Idle:
        {
            // Not started yet
            return RTDEXSYNCINVSTATE;
        }
        case RTDExSync_ChState_Running:
        case RTDExSync_ChState_StopPending:
        {
            // Save the STOP configuration
            hRTDExSync->stop.mode        = mode;
            hRTDExSync->stop.trigSel     = trigSel;
            hRTDExSync->stop.u64TickTime = u64StopTime;

            // Change our current state to STOP pending
            hRTDExSync->ch.localState    = RTDExSync_ChState_StopPending;

            // In case of uplink or stop immediate in downlink, send an empty data packet now !!
            if ( (hRTDExSync->ch.dir == RTDExSync_Dir_Uplink) || (mode == RTDExSync_TrigMode_Immediately) )
            {
                res = RTDExSync_Send( hRTDExSync, NULL, 0, 0, NULL, NULL );
            }
            break;
        }
        case RTDExSync_ChState_Stopped:
        {
            // Already stopped 
            break;
        }
        default:
        {
		    res = RTDEXSYNCINVSTATE;
        }
    }
    return res;
}

/****************************************************************************
 * Function: RTDExSync_Send
 *****************************************************************************
 *
 * Perform a RTDExSync transfer form the host to the device.
 *
 * @param [in]  hRTDExSync
 *      A pRTDExSync handle.
 *
 * @param [in]  pIQSample
 *      A pointer to the buffer containing the IQ samples be written.
 *
 * @param [in]  u32NumSample
 *      The number of IQ samples to be written. An IQ sample is a pair of 16-bit value.
 *      If u32NumSample is bigger than max transfer size, it will be split in multiple 
 *      transfers. If u32NumSample is 0, a single transfer with no data will be performed.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its length must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @return
 *      Returns the number of samples written or an error code. The count could 
 *      be smaller than the data size if the stop condition has been is reached. 
 *
 ****************************************************************************/
adp_result_t RTDExSync_Send( RTDExSync_Handle_t hRTDExSync, 
                             void *             pIQSample,
                             uint32_t           u32NumSample,
                             uint32_t           u32EvtQueueSize,
                             RTDExSync_Event_t  *pEvtQueue,
                             uint32_t           *pu32EvtCount )
{
	Result res = RTDEXSYNCOK;
    uint32_t crc;
    uint32_t tsf;
    uint32_t cmd;      
    uint32_t idx;    
    uint32_t mode;
    uint32_t plSize;
    uint32_t trigSel;
    uint32_t vrtSize;
    uint64_t ftime = 0;
    uint32_t evtCount = 0;
    uint32_t u32Written = 0;
    RTDExSync_VrtPacket_t vrtPkt; 
    int once = (u32NumSample == 0);
    int emptyDataPkt = (u32NumSample == 0);
    uint8_t *pu8Data = (uint8_t *)pIQSample;

    /*
     * Validate the parameters
     */
    switch ( hRTDExSync->ch.dir )
    {
        case RTDExSync_Dir_Uplink:
        {
            // Only empty data packets are supported for uplink channels
            if ( !emptyDataPkt )
            {
		        res = RTDEXSYNCINVFRSIZE;
                goto done;
            }
            break;
        }
        case RTDExSync_Dir_Downlink:
        {
            // OK
            break;
        }
        default:
        {
            res = RTDEXSYNCINVDIR;
            goto done;
        }
    }
    if ( u32EvtQueueSize && ((pEvtQueue == NULL) || pu32EvtCount == NULL) )
    {
        res = RTDEXSYNCINVEVTQUEUE;
        goto done;
    }

    // Clear the event queue
    if ( pu32EvtCount != NULL ) *pu32EvtCount = 0;
    memset( pEvtQueue, 0, u32EvtQueueSize * sizeof(pEvtQueue[0]) );


    /*
     * Send the data in as many VRL frames/VRT packets as required to transfer all the data
     */
    while ( (u32Written < u32NumSample) || once )
    {
        idx = 0;      
        once = 0;

        // Make sure we are in the proper state 
        switch ( hRTDExSync->ch.localState )
        {
            case RTDExSync_ChState_Idle:
            {
                res = RTDEXSYNCINVSTATE;
                goto done;
            }
            case RTDExSync_ChState_Armed:
            {
                // Send a START command
                cmd = VITA49_VRT_USERCTRL_CMD_START;

                // and set the starting time if mode is AtTime
                if ( hRTDExSync->start.mode == RTDExSync_TrigMode_AtTime )
                {
                    // Specify the absolute starting time in TickCount
                    tsf   = VITA49_VRT_DATAPKT_HEADER_TSF_FREERUNNING;
                    ftime = hRTDExSync->start.u64TickTime;
                }
                else tsf  = VITA49_VRT_CTXPKT_HEADER_TSF_NONE;

                mode    = (uint32_t)hRTDExSync->start.mode;
                trigSel = (uint32_t)hRTDExSync->start.trigSel;
                break;
            }
            case RTDExSync_ChState_Running:
            {
                cmd     = VITA49_VRT_USERCTRL_CMD_KEEP;
                tsf     = VITA49_VRT_CTXPKT_HEADER_TSF_NONE;
                mode    = (uint32_t)hRTDExSync->start.mode;
                trigSel = VITA49_VRT_USERCTRL_TRIGSEL_NA;
                break;
            }
            case RTDExSync_ChState_StopPending:
            {
                // Send STOP command
                cmd = VITA49_VRT_USERCTRL_CMD_STOP;

                // and set the stopping time if mode is AtTime
                if ( hRTDExSync->stop.mode == RTDExSync_TrigMode_AtTime )
                {
                    // Specify the absolute starting time in TickCount
                    tsf   = VITA49_VRT_DATAPKT_HEADER_TSF_FREERUNNING;
                    ftime = hRTDExSync->stop.u64TickTime;
                }
                else if ( hRTDExSync->stop.mode == RTDExSync_TrigMode_AtTime )
                {
                    // Specify the absolute starting time in TickCount
                    tsf   = VITA49_VRT_DATAPKT_HEADER_TSF_SAMPCNT;
                    ftime = hRTDExSync->stop.u64TickTime;
                }
                else tsf  = VITA49_VRT_CTXPKT_HEADER_TSF_NONE;

                mode    = (uint32_t)hRTDExSync->stop.mode;
                trigSel = (uint32_t)hRTDExSync->stop.trigSel;
                break;
            }
            case RTDExSync_ChState_Stopped:
            {
                // Channel is stopped
                res = (adp_result_t)u32Written;
                goto done;
            }
            default:
            {
                res = RTDEXSYNCINVSTATE;
                goto done;
            }
        }	
        // Compute the payload size
        plSize  = ((u32NumSample - u32Written) >= hRTDExSync->u32MaxIQSamples) ? hRTDExSync->u32MaxIQSamples : (u32NumSample - u32Written);

        //
        // Build the VRT packet
        //

        // Prepare the content
        vrtPkt.pktType                         = VITA49_VRT_DATAPKT_HEADER_PKTTYPE_DLDATA;      // VRT packet type

        vrtPkt.u.dlDataPkt.pktCnt              = hRTDExSync->tx.u4VrtPacketCount;               // VRT packet count (modulo 16)
        vrtPkt.u.dlDataPkt.u32StreamID         = (hRTDExSync->ch.u16SessionId << 16)            // Stream identifier
                                                 | hRTDExSync->ch.u8Idx;                



        vrtPkt.u.dlDataPkt.timestamp.TSI       = VITA49_VRT_DATAPKT_HEADER_TSI_SAMPCNT;         // Integer-seconds timestamp mode
        vrtPkt.u.dlDataPkt.timestamp.u32IntTs  = hRTDExSync->tx.u32SampleCount;                 // Integer-seconds timestamp
        vrtPkt.u.dlDataPkt.timestamp.TSF       = tsf;                                           // Fractional-seconds timestamp mode
        vrtPkt.u.dlDataPkt.timestamp.u64FracTs = ftime;                                         // Fractional-seconds timestamp
                                 
        vrtPkt.u.dlDataPkt.pl.size             = plSize * 2 * sizeof(uint16_t);                 // Payload size (in bytes)
        vrtPkt.u.dlDataPkt.pl.pData            = &pu8Data[u32Written * 2 * sizeof(uint16_t)];   // Pointer to the payload data
                                   
        vrtPkt.u.dlDataPkt.opmode.command      = cmd;                                           // Command (0:none, 1:start, 2:stop)
        vrtPkt.u.dlDataPkt.opmode.syncMode     = mode;                                          // Synchronization mode (0:Immediate, 1:On trigger, 2:At time)
        vrtPkt.u.dlDataPkt.opmode.trigSel      = trigSel;                                       // Trigger selection (External trigger [0:3])

        // Encode the packet (Skip two words at the beginning for the VRL header)
        res = vrtSize = RTDExSync_EncodeVrtPacket( &vrtPkt, &hRTDExSync->tx.pu32Buffer[2],  hRTDExSync->tx.u22MaxFrameSize - 3*sizeof(uint32_t) );
        if ( FAILURE(res) ) break;

        //
        // Add the VRL container
        //

        // VRL Magic
        hRTDExSync->tx.pu32Buffer[0] = SET_FIELD( VITA49_VRL_FAW_VRLP, 'VRLP' );

	    // VRL frame count and frame size
        hRTDExSync->tx.pu32Buffer[1] = SET_FIELD( VITA49_VRL_HEADER_FRCOUNT, hRTDExSync->tx.u12VrlFrameCount )
                                     | SET_FIELD( VITA49_VRL_HEADER_FRSIZE,  vrtSize + 3 );

        // Optional VRL CRC
        #ifdef RTDEXSYNC_CRC32
            crc = RTDExSync_Crc32( 0xFFFFFFFF, (uint32_t *)hRTDExSync->tx.pu8Buffer, (vrtSize + 3) - 1 );
            hRTDExSync->tx.pu32Buffer[2 + vrtSize] = SET_FIELD( VITA49_VRL_TRAILER_CRC, crc );
        #else
            crc = 0; 
            hRTDExSync->tx.pu32Buffer[2 + vrtSize] = SET_FIELD( VITA49_VRL_TRAILER_VEND, 'VEND' );
        #endif

        //
        // Update some counter
        //
        hRTDExSync->tx.u12VrlFrameCount = (hRTDExSync->tx.u12VrlFrameCount + 1) & 0x0FFF;
        hRTDExSync->tx.u4VrtPacketCount = (hRTDExSync->tx.u4VrtPacketCount + 1) & 0x0F;
        hRTDExSync->tx.u32SampleCount  += plSize;
        u32Written                     += plSize;

        //
        // Invoke RTDExSend transfer
        //
        res = RTDExSend( hRTDExSync->tx.hRTDEx, (uint8_t *)hRTDExSync->tx.pu32Buffer, hRTDExSync->tx.u22MaxFrameSize);
        if ( res != hRTDExSync->tx.u22MaxFrameSize ) 
	    {
		    res = RTDExGetLastError( hRTDExSync->tx.hRTDEx );		    
            break;
        }

        //
        // Update the local state
        //
        switch ( hRTDExSync->ch.localState )
        {
            case RTDExSync_ChState_Armed:
            {
                // Change our state to RUNNING
                hRTDExSync->ch.localState = RTDExSync_ChState_Running;
                break;
            }
            default: break;
        }

        //
        // Look for a stop condition 
        //
#if 0   // Deactivated because it slows the TX
        if ( (hRTDExSync->ch.dir == RTDExSync_Dir_Downlink) 
          && (hRTDExSync->ch.localState == RTDExSync_ChState_StopPending) )
        {
            do {
                uint32_t numEvt = 0;
                res = RTDExSync_Monitor( hRTDExSync, u32EvtQueueSize-evtCount, &pEvtQueue[evtCount], &numEvt, 0 );
            
                // Increate the event counter 
                evtCount += numEvt;

                // Make sure we are still running
                if ( hRTDExSync->ch.localState == RTDExSync_ChState_Stopped )
                {
                    // Channel is stopped, return the number of byte written
                    res = (adp_result_t)u32Written;
                    goto done;
                }
    
                // TODO: Look for an error, report to the user, abort transmission ??

            } while ( SUCCESS(res) );
        }
#endif
	}

    //
    // Look for a problem
    //
    if ( (hRTDExSync->ch.dir == RTDExSync_Dir_Downlink) && (!emptyDataPkt || (emptyDataPkt && u32EvtQueueSize)) )
    {
        do {
            uint32_t numEvt = 0;
            res = RTDExSync_Monitor( hRTDExSync, u32EvtQueueSize-evtCount, &pEvtQueue[evtCount], &numEvt, 0 );
            
            // Increate the event counter 
            evtCount += numEvt;

            // Make sure we are still running
            /* JR : Only leave when no more received packet */ /*
            if ( hRTDExSync->ch.localState == RTDExSync_ChState_Stopped )
            {
                // Channel is stopped, return the number of byte written
                res = (adp_result_t)u32Written;
                goto done;
            }
            */
    
            // TODO: Look for an error, report to the user, abort transmission ??

        } while ( SUCCESS(res) );
    }

    // Returs the number of bytes written
    res = (adp_result_t)u32Written;    

done:
    // Return the number of event received 
    if ( pu32EvtCount ) *pu32EvtCount = evtCount;

    return res;
}

/****************************************************************************
 * Function: RTDExSync_Receive
 *****************************************************************************
 *
 * Perform a RTDExSync transfer form the device to the host.
 *
 * @param [in]  hRTDExSync
 *      A pRTDExSync handle.
 *
 * @param [out]  pIQSample
 *      A pointer to the buffer that receives the transfered IQ samples.
 *
 * @param [in]  u32NumSample
 *      The number of IQ samples to receive.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its lenght must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @param [in]  block
 *      Blocking mode flag. If '0', the function returns only the data available.
 *      If not '0', the routine wait for the requested amount of data before returning.
 *
 * @return
 *      Returns the number of samples received or an error code. The count could 
 *      be smaller than the data size if the stop condition has been is reached. 
 *
 ****************************************************************************/
adp_result_t RTDExSync_Receive( RTDExSync_Handle_t hRTDExSync, 
                                void *             pIQSample,
                                uint32_t           u32NumSample,
                                uint32_t           u32EvtQueueSize,
                                RTDExSync_Event_t  *pEvtQueue,
                                uint32_t           *pu32EvtCount,
                                int                block )
{
    uint32_t num;
    uint32_t u32Read = 0;
    uint32_t evtCount = 0;
	Result res = RTDEXSYNCOK;
    uint8_t *pu8Data = (uint8_t *)pIQSample;

    /*
     * Validate the parameters
     */
    if ( hRTDExSync->ch.dir != RTDExSync_Dir_Uplink )
    {
		res = RTDEXSYNCINVDIR;
        goto done;
    }
    if ( u32EvtQueueSize && ((pEvtQueue == NULL) || pu32EvtCount == NULL) )
    {
		res = RTDEXSYNCINVEVTQUEUE;
        goto done;
    }

    // Clear the event queue
    if ( pu32EvtCount != NULL ) *pu32EvtCount = 0;
    memset( pEvtQueue, 0, u32EvtQueueSize * sizeof(pEvtQueue[0]) );
    
    if ( hRTDExSync->ch.localState == RTDExSync_ChState_Idle )
    {
		res = RTDEXSYNCINVSTATE;
        goto done;
    }

    // Read 0, return 0 !!
    if ( u32NumSample == 0 )
    {
        return (adp_result_t)0;
    }

    /*
     * Receive the data in as many VRL frames/VRT packets as required to transfer all the data
     */
    while ( (u32Read < u32NumSample) || (u32Read == 0) )
    {
        // Look if there is data already available
        if ( (hRTDExSync->rx.vrtPkt.pktType == VITA49_VRT_DATAPKT_HEADER_PKTTYPE_ULDATA)
          && (hRTDExSync->rx.vrtPkt.u.ulDataPkt.pl.size > 0 ) )
        {
            // Detect gap
            if ( hRTDExSync->rx.u32SampleCount != hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs )
            {
                // Overrun event
                if ( hRTDExSync->evt.overrun == 0 )
                {        
                    hRTDExSync->evt.overrun = 1;

                    if ( evtCount < u32EvtQueueSize )
                    {
                        pEvtQueue[evtCount].id             = RTDExSync_EventId_FifoOverrun;
                        pEvtQueue[evtCount].gTiming        = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.general;
                        pEvtQueue[evtCount].u32SampleCount = hRTDExSync->rx.u32SampleCount;
                        pEvtQueue[evtCount].u64TickCount   = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs;    
                        pEvtQueue[evtCount].u32EventCount  = hRTDExSync->evt.u32EventCnt;
    
                        evtCount++;
                    }

                    // Increment the event counter
                    hRTDExSync->evt.u32EventCnt++;
                }             

                // Oops, we have missed some samples
                num = (hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs - hRTDExSync->rx.u32SampleCount) * 2*sizeof(uint16_t);

                // Limit the number of sample to copy to the requested size
                if ( num  > ((u32NumSample - u32Read) * 2*sizeof(uint16_t)) ) num = (u32NumSample - u32Read) * 2*sizeof(uint16_t);
                
                // Insert 0s
                memset( pu8Data, 0, num );
                
                // Update the sample count 
                //hRTDExSync->rx.u32SampleCount = hRTDExSync->rx.u32SampleCount + (uint32_t)((num / 2*sizeof(uint16_t)) + 1);
                
                pu8Data += num;
                u32Read += num / (2*sizeof(uint16_t));
                hRTDExSync->rx.u32SampleCount += num / (2*sizeof(uint16_t));
                num = 0;
            }
            else
            {
                // Fifo resume event
                if ( hRTDExSync->evt.overrun == 1 )
                {        
                    hRTDExSync->evt.overrun = 0;

                    if ( evtCount < u32EvtQueueSize )
                    {
                        pEvtQueue[evtCount].id             = RTDExSync_EventId_FifoResume;
                        pEvtQueue[evtCount].gTiming        = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.general;
                        pEvtQueue[evtCount].u32SampleCount = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs;
                        pEvtQueue[evtCount].u64TickCount   = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs;
                        pEvtQueue[evtCount].u32EventCount  = hRTDExSync->evt.u32EventCnt;
    
                        evtCount++;
                    }

                    // Increment the event counter
                    hRTDExSync->evt.u32EventCnt++;
                }    

                // Copy received samples
                num = hRTDExSync->rx.vrtPkt.u.ulDataPkt.pl.size;    // In bytes    

                // Limit the number of sample to copy to the requested size
                if ( num  > ((u32NumSample - u32Read) * 2*sizeof(uint16_t)) ) num = (u32NumSample - u32Read) * 2*sizeof(uint16_t);

                // Copy the samples to the user buffer
                memcpy( pu8Data, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pl.pu8Data, num );
            
                hRTDExSync->rx.vrtPkt.u.ulDataPkt.pl.size -= num;
                hRTDExSync->rx.vrtPkt.u.ulDataPkt.pl.pu8Data += num;

                pu8Data += num;
                u32Read += num / (2*sizeof(uint16_t));

                hRTDExSync->rx.u32SampleCount += num / (2*sizeof(uint16_t));
                hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs += num / (2u*sizeof(uint16_t));
            }
            if ( !block ) break;
        }
        else
        {
            uint32_t numEvt = 0;

            // Get a new message
            res = RTDExSync_Monitor( hRTDExSync, u32EvtQueueSize-evtCount, &pEvtQueue[evtCount], &numEvt, RTDExSync_RX_TIMEOUT );
            
            // Increate the event counter 
            evtCount += numEvt;

            // Abort read on error (TIMEOUT return code is a WARNING)    
            if ( FAILURE(res) ) break;

            if ( res == RTDEXSYNCTIMEOUT && !block ) break;

            // Make sure we are still running
            if ( hRTDExSync->ch.localState == RTDExSync_ChState_Stopped )
            {
                break;
            }


            // Look if we have to react to the received message
            // todo
        }
    }
    res = (adp_result_t)u32Read;

done:
    // Return the number of event received 
    if ( pu32EvtCount ) *pu32EvtCount = evtCount;

    return res;
}


/****************************************************************************
 * Function: RTDExSync_IsStarted
 *****************************************************************************
 *
 * Check if the RTDExSync channel is started or not.
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its lenght must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @param [in]  u32TimeoutMs 
 *      Timeout in msec.
 *
 * @return
 *      Returns "1" if started, "0" if nor, or an error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_IsStarted( RTDExSync_Handle_t hRTDExSync, 
                                  uint32_t           u32EvtQueueSize,
                                  RTDExSync_Event_t  *pEvtQueue,
                                  uint32_t           *pu32EvtCount,
                                  uint32_t           u32TimeoutMs )
{
    uint32_t evtCount = 0;
    adp_result_t res = RTDEXSYNCOK;

    /*
     * Validate the parameters
     */
    if ( u32EvtQueueSize && ((pEvtQueue == NULL) || pu32EvtCount == NULL) )
    {
		res = RTDEXSYNCINVEVTQUEUE;
        goto done;
    }

    // Clear the event queue
    if ( pu32EvtCount != NULL ) *pu32EvtCount = 0;
    memset( pEvtQueue, 0, u32EvtQueueSize * sizeof(pEvtQueue[0]) );

    switch ( hRTDExSync->ch.localState )
    {
        case RTDExSync_ChState_Idle:        // Not started
        case RTDExSync_ChState_Armed:       // Armed but not started
        {
            // No the channel is not started
            res = (adp_result_t)0;
            break;
        }
        case RTDExSync_ChState_Running:     // Started and running, may not be started yet on the device
        case RTDExSync_ChState_StopPending:  // Started and stopping
        {
            switch ( hRTDExSync->ch.remoteState )
            {
                case VITA49_VRT_TRAILER_CHSTATIND_RUNNING:          // Channel is running
                case VITA49_VRT_TRAILER_CHSTATIND_STOPPEND:         // Channel stop is pending
                {
                    // Yes, the channel is started
                    res = (adp_result_t)1;
                    break;
                }
                case VITA49_VRT_TRAILER_CHSTATIND_IDLE:             // Channel is idle or stopped
                case VITA49_VRT_TRAILER_CHSTATIND_STARTPEND:        // Channel start is pending
                {
                    // Try to get the status from the radio if it is a downlink channel. 
                    // For uplink channel, only the receive routine can perform read operation.
                    if ( hRTDExSync->ch.dir == RTDExSync_Dir_Downlink )
                    {
                         uint32_t numEvt = 0;

                        // Get a new message
                        res = RTDExSync_Monitor( hRTDExSync, u32EvtQueueSize-evtCount, &pEvtQueue[evtCount], &numEvt, u32TimeoutMs );
            
                        // Increate the event counter 
                        evtCount += numEvt;

                        if ( SUCCESS(res) )
                        {
                            switch ( hRTDExSync->ch.remoteState )
                            {
                                case VITA49_VRT_TRAILER_CHSTATIND_IDLE:         // Channel is idle or stopped
                                case VITA49_VRT_TRAILER_CHSTATIND_STARTPEND:    // Channel start is pending
                                {
                                    // No the channel is not started
                                    res = (adp_result_t)0;
                                    break;
                                }
                                case VITA49_VRT_TRAILER_CHSTATIND_RUNNING:      // Channel is running
                                case VITA49_VRT_TRAILER_CHSTATIND_STOPPEND:     // Channel stop is pending
                                {
                                    // Yes, the channel is started
                                    res = (adp_result_t)1;
                                    break;
                                }
                                default:
                                {
		                            res = RTDEXSYNCINVSTATE;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            // No (as far as we know !!)
                            res = (adp_result_t)0;
                            break;
                        }
                    }
                    else
                    {
                        // No (as far as we know !!)
                        res = (adp_result_t)0;
                        break;
                    }
                    break;
                }
                default:
                {
		            res = RTDEXSYNCINVSTATE;
                    break;
                }
            }
            break;
        }
        case RTDExSync_ChState_Stopped:     // Started and stopped
        {
            // Yes, the channel is started (and already stopped)
            res = (adp_result_t)1;
            break;
        }
        default:
        {
		    res = RTDEXSYNCINVSTATE;
            break;
        }
    }

done:
    // Return the number of event received 
    if ( pu32EvtCount ) *pu32EvtCount = evtCount;

    return res;
}

/****************************************************************************
 * Function: RTDExSync_IsStopped
 *****************************************************************************
 *
 * Check if the RTDExSync channel is stopped or not.
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its lenght must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @param [in]  u32TimeoutMs 
 *      Timeout in msec.
 *
 * @return
 *      Returns "1" if stopped, "0" if nor, or an error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_IsStopped( RTDExSync_Handle_t hRTDExSync, 
                                  uint32_t           u32EvtQueueSize,
                                  RTDExSync_Event_t  *pEvtQueue,
                                  uint32_t           *pu32EvtCount,
                                  uint32_t           u32TimeoutMs )
{
    uint32_t evtCount = 0;
    adp_result_t res = RTDEXSYNCOK;

    // Clear the event queue
    if ( pu32EvtCount != NULL ) *pu32EvtCount = 0;
    memset( pEvtQueue, 0, u32EvtQueueSize * sizeof(pEvtQueue[0]) );

    /*
     * Validate the parameters
     */
    if ( u32EvtQueueSize && ((pEvtQueue == NULL) || pu32EvtCount == NULL) )
    {
		res = RTDEXSYNCINVEVTQUEUE;
        goto done;
    }

    switch ( hRTDExSync->ch.localState )
    {
        case RTDExSync_ChState_Idle:        // Not started
        case RTDExSync_ChState_Armed:       // Armed but not started
        case RTDExSync_ChState_Running:     // Started and running, may not be started yet on the device
        {
            // No the channel is not stopped
            res =  (adp_result_t)0;
            goto done;
        }
        case RTDExSync_ChState_StopPending:  // Started and stopping
        {
            // Try to get the status from the radio if it is a downlink channel. 
            // For uplink channel, only the receive routine can perform read operation.
            if ( hRTDExSync->ch.dir == RTDExSync_Dir_Downlink )
            {
                uint32_t numEvt = 0;

                // Get a new message
                res = RTDExSync_Monitor( hRTDExSync, u32EvtQueueSize-evtCount, &pEvtQueue[evtCount], &numEvt, u32TimeoutMs );
            
                // Increase the event counter 
                evtCount += numEvt;

                if ( SUCCESS(res) )
                {
                    switch ( hRTDExSync->ch.remoteState )
                    {
                        case VITA49_VRT_TRAILER_CHSTATIND_IDLE:         // Channel is idle or stopped
                        {
                            // Yes, the channel is stopped
                            res =  (adp_result_t)1;
                            goto done;
                        }
                        case VITA49_VRT_TRAILER_CHSTATIND_STARTPEND:    // Channel start is pending
                        case VITA49_VRT_TRAILER_CHSTATIND_RUNNING:      // Channel is running
                        case VITA49_VRT_TRAILER_CHSTATIND_STOPPEND:     // Channel stop is pending
                        {
                            // No the channel is not stopped
                            res =  (adp_result_t)0;
                            goto done;
                        }
                        default:
                        {
		                    res = RTDEXSYNCINVSTATE;
                            break;
                        }
                    }
                }
                else
                {
                    // No (as far as we know !!)
                    res =  (adp_result_t)0;
                    goto done;
                }
            }
            else
            {
                // No (as far as we know !!)
                res =  (adp_result_t)0;
                goto done;
            }
            break;
        }
        case RTDExSync_ChState_Stopped:     // Started and stopped
        {
            // Yes, the channel is stopped
            res =  (adp_result_t)1;
            goto done;
        }
        default:
        {
		    res = RTDEXSYNCINVSTATE;
            break;
        }
    }

done:
    // Return the number of event received 
    if ( pu32EvtCount ) *pu32EvtCount = evtCount;

    return res;
}

/****************************************************************************
 * Function: RTDExSync_Abort
 *****************************************************************************
 *
 * Abort the current RTDExSync state in order to be able to 
 * handle a new start command.
 *
 * This command will stop any current data transmission and reception since it
 * is not synchronous with the data stream.
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @return
 *      Success or error code.
 *
 ****************************************************************************/
adp_result_t RTDExSync_Abort( RTDExSync_Handle_t hRTDExSync )
{
    adp_result_t res = RTDEXSYNCOK;

    // Put the RTDExSync channel in reset
    res = RTDExSync_ChannelReset_send( &hRTDExSync->state, 
                                       hRTDExSync->ch.u8Idx );
	if( FAILURE(res) ) goto error;

    /*
     * Reinitialised RTDExSync object
     */
    // Reset the state
    hRTDExSync->ch.localState = RTDExSync_ChState_Idle;
    hRTDExSync->ch.remoteState = RTDExSync_ChState_Idle;

    // Reset TX counters
    hRTDExSync->tx.u12VrlFrameCount = 0;
    hRTDExSync->tx.u4VrtPacketCount = 0;
    hRTDExSync->tx.u32SampleCount = 0;

    // Reset RX received data and counters
    hRTDExSync->rx.vrtPkt.pktType = 0;
    hRTDExSync->rx.u16ReadIdx = 0;
    hRTDExSync->rx.u16ReadCnt = 0;
    hRTDExSync->rx.u12VrlFrameCount = 0;
    hRTDExSync->rx.u4VrtPacketCount = 0;
    hRTDExSync->rx.u32SampleCount = 0;

    /*
     * Reset RTDEx
     */
    if ( hRTDExSync->tx.hRTDEx != NULL )
    {
        RTDExClose( hRTDExSync->tx.hRTDEx );
        hRTDExSync->tx.hRTDEx = NULL;
    }
    if ( hRTDExSync->rx.hRTDEx != NULL )
    {
        RTDExClose( hRTDExSync->rx.hRTDEx );
        hRTDExSync->rx.hRTDEx = NULL;
    }

    res = RTDExSync_SetupRTDEx( hRTDExSync );
    if( FAILURE(res) ) goto error;

    /*
     * Reactivate the RTDExSync channels
     */
    res = RTDExSync_ChannelStart_send( &hRTDExSync->state, 
                                       hRTDExSync->ch.u8Idx,
                                       hRTDExSync->tx.u22MaxFrameSize,
                                       hRTDExSync->rx.u22MaxFrameSize,
                                       hRTDExSync->rx.u32MediaFrameSize );
	if( FAILURE(res) ) goto error;


    return res;

error:
    return res;
}

/****************************************************************************
 *                             Private routines                             * 
 ****************************************************************************/

/****************************************************************************
* Function: RTDExSync_Presence_send
*****************************************************************************
*
* External API function to verify if the RTDExSync core if present and 
* correctly connected to the platform registers.
*
* @param [in]  state
*      Pointer to the connection state with the CCE.
*
* @return
*      Success or error code.
*
****************************************************************************/
static adp_result_t RTDExSync_Presence_send( connection_state * state )
{
    cmd_send(state, RTDEXSYNC_PRESENCE);
    return result_recv(state);
}

/****************************************************************************
* Function: RTDExSync_CoreReset_send
*****************************************************************************
*
* External API function to reset the RTDExSync core.
*
* @param [in]  state
*      Pointer to the connection state with the CCE.
*
* @return
*      Success or error code.
*
****************************************************************************/
static adp_result_t RTDExSync_CoreReset_send( connection_state * state )
{
    cmd_send(state, RTDEXSYNC_CORERESET);
    return result_recv(state);
}

/****************************************************************************
* Function: RTDExSync_ChannelReset_send
*****************************************************************************
*
* External API function to reset the specified RTDExSync channel.
*
* @param [in]  state
*      Pointer to the connection state with the CCE.
*
* @param [in]  u8Channel
*      RTDEx Sync channel index
*
* @return
*      Success or error code.
*
****************************************************************************/
static adp_result_t RTDExSync_ChannelReset_send( connection_state * state, uint8_t u8Channel )
{
    cmd_send(state, RTDEXSYNC_CHANNELRESET);
    uchar_send(state, u8Channel);
    return result_recv(state);
}

/****************************************************************************
* Function: RTDExSync_ChannelStart_send
*****************************************************************************
*
* External API function to set the frame sizes and
* to enable the specified RTDExSync channel.
*
* @param [in]  state
*      Pointer to the connection state with the CCE.
*
* @param [in]  u8Channel
*      RTDEx Sync channel index
*
* @param [in]  u32RxCtrl
*      VRL frame size from the host to the FPGA
*
* @param [in]  u32TxCtrl
*      Data VRL frame size from the FPGA to the host
*
* @param [in]  u32TxCntxCtrl
*      Context and extended context VRL frame size from the FPGA to the host
*
* @return
*      Success or error code.
*
****************************************************************************/
static adp_result_t RTDExSync_ChannelStart_send( connection_state * state, uint8_t u8Channel, uint32_t u32RxCtrl, uint32_t u32TxCtrl, uint32_t u32TxCntxCtrl )
{
    cmd_send(state, RTDEXSYNC_CHANNELSTART);
    uchar_send(state, u8Channel);
    uint_send(state, u32RxCtrl);
    uint_send(state, u32TxCtrl);
    uint_send(state, u32TxCntxCtrl);
    return result_recv(state);
}

/****************************************************************************
* Function: RTDExSync_ChannelStop_send
*****************************************************************************
*
* External API function to disable the specified RTDExSync channel.
*
* @param [in]  state
*      Pointer to the connection state with the CCE.
*
* @param [in]  u8Channel
*      RTDEx Sync channel index
*
* @return
*      Success or error code.
*
****************************************************************************/
static adp_result_t RTDExSync_ChannelStop_send( connection_state * state, uint8_t u8Channel )
{
    cmd_send(state, RTDEXSYNC_CHANNELSTOP);
    uchar_send(state, u8Channel);
    return result_recv(state);
}

/****************************************************************************
* Function: RTDExSync_ChannelInfo_send
*****************************************************************************
*
* External API function to get the presence and direction of the 
* specified RTDExSync channel.
*
* @param [in]  state
*      Pointer to the connection state with the CCE.
*
* @param [in]  u8Channel
*      RTDEx Sync channel index
*
* @param [out]  pPresent
*      0: Channel absent,
*      1: Channel present
*
* @param [out]  pDir
*      0: Uplink (Radio -> Host),
*      1: Downlink (Host -> Radio)
*
* @return
*      Success or error code.
*
****************************************************************************/
static adp_result_t RTDExSync_ChannelInfo_send( connection_state * state, uint8_t u8Channel, int * pPresent, int * pDir )
{
    cmd_send(state, RTDEXSYNC_CHANNELINFO);
    uchar_send(state, u8Channel);

    *pPresent = uchar_recv(state);
    *pDir = uchar_recv(state);

    return result_recv(state);
}

/****************************************************************************
* Function: RTDExSync_SetupRTDEx
*****************************************************************************
*
* Open and setup the RTDEx channel associated with this RDTExSync channel.
*
* @param [in]  hRTDExSync
*      A RTDExSync handle.
*
* @return
*      Success or error code.
*
****************************************************************************/
static adp_result_t RTDExSync_SetupRTDEx( RTDExSync_Handle_t hRTDExSync )
{
	Result res = RTDEXSYNCOK;
    uint32_t u32FifoDepth;
    uint32_t u32StatsIsIncluded;
    unsigned int uRTDExCoreId, uRTDExCoreVer;
    RTDExPerseusParamsPcie pcieRTDExParams;

    /*
     * Detect and validate the RTDEx media to be used
     */    
    do 
    {
        // Try Ethernet
        res = RTDExGetCoreIdVerEth( &hRTDExSync->state, hRTDExSync->u32RTDExEthBaseAddr, &uRTDExCoreId, &uRTDExCoreVer );
        if ( SUCCESS(res) )
        {
            if( uRTDExCoreId == RTDEX_ETH_CORE_ID) 
            {
                hRTDExSync->eRTDExMedia = eMediaEth;
                break;
            }
        }
        // Try PCIe
        res = RTDExInitParamsPcie( &pcieRTDExParams, &hRTDExSync->state );    
        if ( SUCCESS(res) )
        {
            res = RTDExGetCoreIdVerPcie( &pcieRTDExParams, &uRTDExCoreId, &uRTDExCoreVer );
            if (SUCCESS(res) )
            {
                if( uRTDExCoreId == RTDEX_PCIE_CORE_ID ) 
                {
                    hRTDExSync->eRTDExMedia = eMediaPcie;
                    break;
                }
            }
        }
        
        // No supported media found
        return RTDEXMEDIANOTSUPPORTED;
    } while (0);    // Once !


    /*
     * Open the RAW RTDex channels
     */
    switch (hRTDExSync->eRTDExMedia )
    {
        case eMediaEth:
        {
            // Make sure the media frame size does not exceeed Jumbo frame capacity
            if ( hRTDExSync->tx.u32MediaFrameSize > RTDEX_ETH_MAX_FRAME_SIZE ||
                hRTDExSync->rx.u32MediaFrameSize > RTDEX_ETH_MAX_FRAME_SIZE)
            {
                return RTDEXSYNCINVSUBFRSIZE;
            }

            // Open TX channel ...
            res = RTDExOpenHostAndFpgaEthTo( &hRTDExSync->tx.hRTDEx, 
                                             &hRTDExSync->state, 
                                             hRTDExSync->u32RTDExEthBaseAddr, 
                                             hRTDExSync->ch.u8Idx, 
                                             eToFpga, 
                                             eContinuous, 
                                             RTDExSync_RX_TIMEOUT );
            if( FAILURE(res) ) goto error; 

            // Open RX channel ...
            res = RTDExOpenHostAndFpgaEthTo( &hRTDExSync->rx.hRTDEx, 
                                             &hRTDExSync->state, 
                                             hRTDExSync->u32RTDExEthBaseAddr, 
                                             hRTDExSync->ch.u8Idx, 
                                             eFromFpga, 
                                             eContinuous, 
                                             -1 );  // Return immediately
            if( FAILURE(res) ) goto error;

            break;
        }
        case eMediaPcie:
        {
            // Open TX channel ....
            res = RTDExOpenHostAndFpgaPcie( &hRTDExSync->tx.hRTDEx, 
                                            &pcieRTDExParams, 
                                            hRTDExSync->ch.u8Idx, 
                                            eToFpga, 
                                            eContinuous );
            if( FAILURE(res) ) goto error; 

            // Open RX channel ....
            res = RTDExOpenHostAndFpgaPcie( &hRTDExSync->rx.hRTDEx, 
                                            &pcieRTDExParams, 
                                            hRTDExSync->ch.u8Idx, 
                                            eFromFpga, 
                                            eContinuous);
            if( FAILURE(res) ) goto error; 

            break;
        }          
        default: break;     // Impossible
    }

    /*
     * Reset fifos
     */
    res = RTDExResetFifo( hRTDExSync->tx.hRTDEx );
    if( FAILURE(res) ) goto error; 

    res = RTDExResetFifo( hRTDExSync->rx.hRTDEx );
    if( FAILURE(res) ) goto error; 

	/*
     * Set flow control setup for FPGA receiver
     */
    if ( hRTDExSync->eRTDExMedia == eMediaEth )
    {
    	// Set frame gap for FPGA transmitter
	    res = RTDExSetTxFrameGapValEth( &hRTDExSync->state, 
                                        hRTDExSync->u32RTDExEthBaseAddr, 
                                        RTDEX_ETH_FRAME_GAP );
	    if( FAILURE(res) ) goto error; 

        res = RTDExGetRxConfigExtentedEth( &hRTDExSync->state, 
                                           hRTDExSync->u32RTDExEthBaseAddr, 
                                           &u32FifoDepth, 
                                           &u32StatsIsIncluded );
        if( FAILURE(res) ) goto error; 

        res = RTDExEnableFlowControlEth( &hRTDExSync->state, 
                                         hRTDExSync->u32RTDExEthBaseAddr, 
                                         (uint32_t)(0.75 * u32FifoDepth),       // 3/4
                                         (uint32_t)(0.5 * u32FifoDepth) );      // 1/2
        if( FAILURE(res) ) goto error; 
    }

    /*
     * Enable the RAW RTDEx channels
     */
    res = RTDExStart( hRTDExSync->rx.hRTDEx, hRTDExSync->rx.u32MediaFrameSize, 0 );
    if( FAILURE(res) ) goto error; 

    res = RTDExStart( hRTDExSync->tx.hRTDEx, hRTDExSync->tx.u32MediaFrameSize, 0 );
    if( FAILURE(res) ) goto error; 

    return res;
     
error:
    /*
     * Free allocated resources
     */
    if ( hRTDExSync->rx.hRTDEx != NULL )
    {
        RTDExClose( hRTDExSync->rx.hRTDEx );
        hRTDExSync->rx.hRTDEx = NULL;
    }
    if ( hRTDExSync->tx.hRTDEx != NULL )
    {
        RTDExClose( hRTDExSync->tx.hRTDEx );
        hRTDExSync->tx.hRTDEx = NULL;
    }    
    return res;
}

/****************************************************************************
 * Function: RTDExSync_Crc32
 *****************************************************************************
 *
 * Compute a CRC32.
 *
 * @param [in]  crc
 *      Initial CRC value.
 *
 * @param [in]  pu32Data
 *      Pointer to the 32-bit data buffer.
 *
 * @param [in]  len
 *     Buffer length in 32-bit words.
 *
 * @return
 *      crc value.
 *
 ****************************************************************************/
static uint32_t RTDExSync_Crc32( uint32_t crc, uint32_t *pu32Data, int len)
{	
	int i;

	while ( len-- > 0 )
    {
        uint32_t netData = htonl( *pu32Data++ );
	    for( i = 0; i < sizeof(uint32_t); i++ )
	    {
		    crc = crc32_table[(uint8_t)(netData >> i * 8) ^ (uint8_t)(crc >> 24)] ^ (crc << 8);
        }
	}
    return crc;
}


/****************************************************************************
 * Function: RTDExSync_EncodeVrtPacket
 *****************************************************************************
 *
 * Encode a VRT packet.
 *
 * @param [in]  pVrtPkt
 *      Pointer to a structure containing the content of the VRT packet to encode.
 *
 * @param [out]  pu32Buff
 *      Pointer to a buffer that receives the encoded packet.
 *
 * @param [in]  u32BufSize
 *      Size of the buffer (in bytes).
 *
 * @return
 *      Returns the VRT packet size in 32-bit words or an error code. 
 *
 ****************************************************************************/
static int RTDExSync_EncodeVrtPacket( RTDExSync_VrtPacket_t *pVrtPkt, 
                                      uint32_t *pu32Buff,
                                      uint32_t  u32BufSize )
{
    int idx = 0;
    int vrtSizeInWord = 0;

    switch ( pVrtPkt->pktType )
    {
        // Downlink data packet
        case VITA49_VRT_DATAPKT_HEADER_PKTTYPE_DLDATA:
        {
           // Compute VRT size
           vrtSizeInWord = 1                                                                                    // VRT header  (1 word) 
                         + 1                                                                                    // VRT stream ID  (1 word)
                         + 1                                                                                    // Integer-seconds timestamp  (1 word)
                         + ((pVrtPkt->u.dlDataPkt.timestamp.TSF != VITA49_VRT_CTXPKT_HEADER_TSF_NONE) ? 2 : 0)  // Fractional-seconds timestamp  (2 word)
                         + 1                                                                                    // User control field (1 word)
                         + 1                                                                                    // Reserved (1 word)
                         + (pVrtPkt->u.dlDataPkt.pl.size / sizeof(uint32_t));                                   // VRT Payload

            // Check buffer size
            if ( (vrtSizeInWord * sizeof(uint32_t)) > u32BufSize )
            {
                return RTDEXSYNCINVPKTSIZE;
            }

            // VRT header  (1 word)          
            pu32Buff[idx++] = SET_FIELD( VITA49_VRT_DATAPKT_HEADER_PKTTYPE, VITA49_VRT_DATAPKT_HEADER_PKTTYPE_DLDATA )
                            | SET_FIELD( VITA49_VRT_DATAPKT_HEADER_C,       VITA49_VRT_DATAPKT_HEADER_C_ABSENT )   
                            | SET_FIELD( VITA49_VRT_DATAPKT_HEADER_T,       VITA49_VRT_DATAPKT_HEADER_T_ABSENT )  
                            | SET_FIELD( VITA49_VRT_DATAPKT_HEADER_RR,      VITA49_VRT_DATAPKT_HEADER_RR_VAL )  
                            | SET_FIELD( VITA49_VRT_DATAPKT_HEADER_TSI,     pVrtPkt->u.dlDataPkt.timestamp.TSI )  
                            | SET_FIELD( VITA49_VRT_DATAPKT_HEADER_TSF,     pVrtPkt->u.dlDataPkt.timestamp.TSF )  
                            | SET_FIELD( VITA49_VRT_DATAPKT_HEADER_PKTCNT,  pVrtPkt->u.dlDataPkt.pktCnt )  
                            | SET_FIELD( VITA49_VRT_DATAPKT_HEADER_PKTSIZE, vrtSizeInWord  );  
 
            // VRT stream ID  (1 word)
            pu32Buff[idx++] = SET_FIELD( VITA49_VRT_STREAMID, pVrtPkt->u.dlDataPkt.u32StreamID );        

            // Integer-seconds timestamp  (1 word)
            pu32Buff[idx++] = pVrtPkt->u.dlDataPkt.timestamp.u32IntTs;    

            // Fractional-seconds timestamp  (2 word)
            if ( pVrtPkt->u.dlDataPkt.timestamp.TSF != VITA49_VRT_CTXPKT_HEADER_TSF_NONE )
            {
                pu32Buff[idx++] = (uint32_t)(pVrtPkt->u.dlDataPkt.timestamp.u64FracTs >> 32ul);    
                pu32Buff[idx++] = (uint32_t)(pVrtPkt->u.dlDataPkt.timestamp.u64FracTs & 0x0FFFFFFFF);    
            }

	        // User control field (1 word)
            pu32Buff[idx++] = SET_FIELD( VITA49_VRT_USERCTRL_CMD,      pVrtPkt->u.dlDataPkt.opmode.command) 
                            | SET_FIELD( VITA49_VRT_USERCTRL_SYNCMODE, pVrtPkt->u.dlDataPkt.opmode.syncMode )
                            | SET_FIELD( VITA49_VRT_USERCTRL_TRIGSEL,  pVrtPkt->u.dlDataPkt.opmode.trigSel );

            // Reserved (1 word)
            pu32Buff[idx++] = 0x00000000;

            // VRT Payload
            memcpy( &pu32Buff[idx], pVrtPkt->u.dlDataPkt.pl.pData, pVrtPkt->u.dlDataPkt.pl.size );
            idx += pVrtPkt->u.dlDataPkt.pl.size / sizeof(uint32_t);
            break;
        }
        
        default:
        {
            // Packet type unsupported in this direction
            return RTDEXSYNCINVPKTTYPE;
        }
    }
    return vrtSizeInWord;
}


/****************************************************************************
 * Function: RTDExSync_DecodeVrtPacket
 *****************************************************************************
 *
 * Decode a VRT packet.
 *
 * @param [in]  pu32EncPkt
 *      Received encoded packet.
 *
 * @param [in]  u32MaxPktSize 
 *      Maximum excoded packet size (in bytes).
 *
 * @param [out]  pDecPacket 
 *      Pointer to a structure that receives the packet content.
 *
 * @return
 *      Success or error code.
 *
 * @ note
 *      The payload data is not copied. It needs to be read from the encoded 
 *      message buffer.
 *
 ****************************************************************************/
static adp_result_t RTDExSync_DecodeVrtPacket( uint32_t *pu32EncPkt,
                                               uint32_t u32MaxPktSize,
                                               RTDExSync_VrtPacket_t *pDecPacket )
{
    uint32_t idx;
    memset( pDecPacket, 0, sizeof(RTDExSync_VrtPacket_t) );

    // Get the packet type
    idx = 0;
    pDecPacket->pktType = GET_FIELD( VITA49_VRT_DATAPKT_HEADER_PKTTYPE, pu32EncPkt[idx] );
        
    // Extract the contect based on the packet type    
    switch ( pDecPacket->pktType )
    {
        // Uplink data packet
        case VITA49_VRT_DATAPKT_HEADER_PKTTYPE_ULDATA:
        {
            uint32_t c, t, vrtSize;

            // Header (1 Word, Mandatory)
            pDecPacket->u.ulDataPkt.pktCnt        = GET_FIELD( VITA49_VRT_DATAPKT_HEADER_PKTCNT, pu32EncPkt[idx] );  
            pDecPacket->u.ulDataPkt.timestamp.TSI = GET_FIELD( VITA49_VRT_DATAPKT_HEADER_TSI,    pu32EncPkt[idx] ); 
            pDecPacket->u.ulDataPkt.timestamp.TSF = GET_FIELD( VITA49_VRT_DATAPKT_HEADER_TSF,    pu32EncPkt[idx] );  

            c       = GET_FIELD( VITA49_VRT_DATAPKT_HEADER_C,       pu32EncPkt[idx] );   
            t       = GET_FIELD( VITA49_VRT_DATAPKT_HEADER_T,       pu32EncPkt[idx] );  
            vrtSize = GET_FIELD( VITA49_VRT_DATAPKT_HEADER_PKTSIZE, pu32EncPkt[idx++] );  

            if ( ((vrtSize * sizeof(uint32_t)) > u32MaxPktSize) || vrtSize < 1 )
            {
                #if RTDExSync_DBG 
                    printf( "RTDExSync DBG(ULDATA): Received a VRT packet with an invalid size (%d words)\n", vrtSize );
                #endif  // RTDExSync_DBG 

                pDecPacket->pktType = 0;        // Make sure the packet type is invalid
                return RTDEXSYNCINVPKTSIZE;
            }

            // Stream Identifier (1 Word, Optional)
            pDecPacket->u.ulDataPkt.u32StreamID = GET_FIELD( VITA49_VRT_STREAMID, pu32EncPkt[idx++] );  

            // Class Identifier (2 Words, Optional)
            if ( c ) idx += 2;

            // Integer-seconds Timestamp (1 Word, Optional)
            if ( pDecPacket->u.ulDataPkt.timestamp.TSI != VITA49_VRT_DATAPKT_HEADER_TSI_NONE )
            {
                pDecPacket->u.ulDataPkt.timestamp.u32IntTs = pu32EncPkt[idx++];  
            }

            // Fractional-seconds Timestamp (2 Words, Optional)
            if ( pDecPacket->u.ulDataPkt.timestamp.TSF != VITA49_VRT_DATAPKT_HEADER_TSF_NONE )
            {
                pDecPacket->u.ulDataPkt.timestamp.u64FracTs  = (uint64_t)pu32EncPkt[idx++] << 32u;  
                pDecPacket->u.ulDataPkt.timestamp.u64FracTs |= (uint64_t)pu32EncPkt[idx++] & 0x0FFFFFFFF;  
            }

            // Data Payload (Variable, Mandatory)
            pDecPacket->u.ulDataPkt.pl.pu8Data = (uint8_t *)&pu32EncPkt[idx];
            idx += (vrtSize - idx - (t?1:0) - 1);

            // The last word is the valid sample count
            pDecPacket->u.ulDataPkt.pl.size = pu32EncPkt[idx++] * 2*sizeof(uint16_t);
            
            // Trailer (1 Word, Optional)
            if ( t )
            {
                uint32_t enables = GET_FIELD( VITA49_VRT_TRAILER_ENABLES, pu32EncPkt[idx] );  

                ///< Calibrated time indicator
                pDecPacket->u.ulDataPkt.ind.calTimeIndEn = !!(enables & VITA49_VRT_TRAILER_ENABLES_CALTIMIND);
                pDecPacket->u.ulDataPkt.ind.calTimeInd   = GET_FIELD( VITA49_VRT_TRAILER_CALTIMIND, pu32EncPkt[idx] );  
                
                ///< Valid data indicator
                pDecPacket->u.ulDataPkt.ind.validDataIndEn = !!(enables & VITA49_VRT_TRAILER_ENABLES_VALIDDATAIND);
                pDecPacket->u.ulDataPkt.ind.validDataInd   = GET_FIELD( VITA49_VRT_TRAILER_VALIDDATAIND, pu32EncPkt[idx] );  

                ///< Reference lock indicator
                pDecPacket->u.ulDataPkt.ind.refLockIndEn = !!(enables & VITA49_VRT_TRAILER_ENABLES_REFLOCKIND);
                pDecPacket->u.ulDataPkt.ind.refLockInd   = GET_FIELD( VITA49_VRT_TRAILER_REFLOCKIND, pu32EncPkt[idx] );  

                ///< Sample loss indicator
                pDecPacket->u.ulDataPkt.ind.sampLossIndEn = !!(enables & VITA49_VRT_TRAILER_ENABLES_SAMPLOSSIND);
                pDecPacket->u.ulDataPkt.ind.sampLossInd   = GET_FIELD( VITA49_VRT_TRAILER_SAMPLOSSIND, pu32EncPkt[idx] );  

                ///< Channel state indicator
                pDecPacket->u.ulDataPkt.ind.chStateIndEn = !!(enables & VITA49_VRT_TRAILER_ENABLES_CHSTATIND);
                pDecPacket->u.ulDataPkt.ind.chStateInd   = GET_FIELD( VITA49_VRT_TRAILER_CHSTATIND, pu32EncPkt[idx] );      
            
                ///< Associate context packet count
                pDecPacket->u.ulDataPkt.aCtxPkt.present = GET_FIELD( VITA49_VRT_TRAILER_E, pu32EncPkt[idx] );     
                pDecPacket->u.ulDataPkt.aCtxPkt.count   = GET_FIELD( VITA49_VRT_TRAILER_ECTX_PKTCNT, pu32EncPkt[idx++] );     
            }
            return vrtSize;
        }

        // Context packet
        case VITA49_VRT_CTXPKT_HEADER_PKTTYPE_CTX:
        {
            uint32_t c, vrtSize, ciIdx;

            // Header (1 Word, Mandatory)
            pDecPacket->u.ctkPkt.pktCnt            = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_PKTCNT, pu32EncPkt[idx] );  
            pDecPacket->u.ctkPkt.timestamp.TSI     = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_TSI,    pu32EncPkt[idx] ); 
            pDecPacket->u.ctkPkt.timestamp.TSF     = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_TSF,    pu32EncPkt[idx] );  
            pDecPacket->u.ctkPkt.timestamp.general = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_TSM,    pu32EncPkt[idx] );  

            c       = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_C,       pu32EncPkt[idx] );   
            vrtSize = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_PKTSIZE, pu32EncPkt[idx++] );  

            if ( vrtSize < 1 )
            {
                #if RTDExSync_DBG 
                    printf( "RTDExSync DBG(CTX)   : Received a VRT packet with an invalid size (%d words)\n", vrtSize );
                #endif  // RTDExSync_DBG 

                pDecPacket->pktType = 0;        // Make sure the packet type is invalid
                return RTDEXSYNCINVPKTSIZE;
            }

            // Stream Identifier (1 Word, Optional)
            pDecPacket->u.ctkPkt.u32StreamID = GET_FIELD( VITA49_VRT_STREAMID, pu32EncPkt[idx++] );  

            // Class Identifier (2 Words, Optional)
            if ( c ) idx += 2;

            // Integer-seconds Timestamp (1 Word, Optional)
            if ( pDecPacket->u.ctkPkt.timestamp.TSI != VITA49_VRT_DATAPKT_HEADER_TSI_NONE )
            {
                pDecPacket->u.ctkPkt.timestamp.u32IntTs = pu32EncPkt[idx++];  
            }

            // Fractional-seconds Timestamp (2 Words, Optional)
            if ( pDecPacket->u.ctkPkt.timestamp.TSF != VITA49_VRT_DATAPKT_HEADER_TSF_NONE )
            {
                pDecPacket->u.ctkPkt.timestamp.u64FracTs  = (uint64_t)pu32EncPkt[idx++] << 32u;  
                pDecPacket->u.ctkPkt.timestamp.u64FracTs |= (uint64_t)pu32EncPkt[idx++] & 0x0FFFFFFFF;  
            }

            // Context Indicator Field (1 Word, Mandatory)
            ciIdx = idx;

            // Context field change indicator mask 
            GET_FIELD( VITA49_VRT_CTXIND_I, pu32EncPkt[idx++] );

            // Reference point (Unsupported, skip 1 Word)
            if ( GET_FIELD(VITA49_VRT_CTXIND_RP, pu32EncPkt[ciIdx]) ) idx += 1;     

            // RF Bandwidth (Unsupported, skip 2 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_RFBW, pu32EncPkt[ciIdx]) ) idx += 2;     

            // IF reference frequency (Unsupported, skip 2 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_IFREFFREQ, pu32EncPkt[ciIdx]) ) idx += 2;     

            // RF reference frequency (Unsupported, skip 2 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_RFREFFREQ, pu32EncPkt[ciIdx]) ) idx += 2;     

            // IF band offset (Unsupported, skip 2 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_IFBANDOFST, pu32EncPkt[ciIdx]) ) idx += 2;     

            // Reference level (Unsupported, skip 1 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_REFLEV, pu32EncPkt[ciIdx]) ) idx += 1;     

            // Gain (Unsupported, skip 1 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_GAIN, pu32EncPkt[ciIdx]) ) idx += 1;     

            // Over-rage count (Unsupported, skip 1 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_OVCNT, pu32EncPkt[ciIdx]) ) idx += 1;     

            // Sample rate (Unsupported, skip 2 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_SR, pu32EncPkt[ciIdx]) ) idx += 2;     

            // Timestamp adjustment (Unsupported, skip 2 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_TA, pu32EncPkt[ciIdx]) ) idx += 2;     

            // Timestamp calibration time (Unsupported, skip 1 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_TC, pu32EncPkt[ciIdx]) ) idx += 1;     

            //  Temperature (Unsupported, skip 1 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_TEMP, pu32EncPkt[ciIdx]) ) idx += 1;     

            //  Device identifier (Unsupported, skip 2 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_DEVID, pu32EncPkt[ciIdx]) ) idx += 2;     

            //  State and event indicators (1 Word, Optional)
            pDecPacket->u.ctkPkt.stevt.present = GET_FIELD( VITA49_VRT_CTXIND_STEVT, pu32EncPkt[ciIdx] );
            if ( pDecPacket->u.ctkPkt.stevt.present )
            {
                uint32_t enables = GET_FIELD( VITA49_VRT_STEVT_ENABLES, pu32EncPkt[idx] );  

                // Calibrated time indicator
                pDecPacket->u.ctkPkt.stevt.calTimeIndEn = !!(enables & VITA49_VRT_STEVT_ENABLES_CALTIMIND);
                pDecPacket->u.ctkPkt.stevt.calTimeInd   = GET_FIELD( VITA49_VRT_STEVT_CALTIMIND, pu32EncPkt[idx] );  
                
                // Valid data indicator
                pDecPacket->u.ctkPkt.stevt.validDataIndEn = !!(enables & VITA49_VRT_STEVT_ENABLES_VALIDDATAIND);
                pDecPacket->u.ctkPkt.stevt.validDataInd   = GET_FIELD( VITA49_VRT_STEVT_VALIDDATAIND, pu32EncPkt[idx] );  

                // Reference lock indicator
                pDecPacket->u.ctkPkt.stevt.refLockIndEn = !!(enables & VITA49_VRT_STEVT_ENABLES_REFLOCKIND);
                pDecPacket->u.ctkPkt.stevt.refLockInd   = GET_FIELD( VITA49_VRT_STEVT_REFLOCKIND, pu32EncPkt[idx] );  

                // Sample loss indicator
                pDecPacket->u.ctkPkt.stevt.sampLossIndEn = !!(enables & VITA49_VRT_STEVT_ENABLES_SAMPLOSSIND);
                pDecPacket->u.ctkPkt.stevt.sampLossInd   = GET_FIELD(  VITA49_VRT_STEVT_SAMPLOSSIND, pu32EncPkt[idx] );  

                // Channel state indicator
                pDecPacket->u.ctkPkt.stevt.chStateIndEn = !!(enables & VITA49_VRT_STEVT_ENABLES_CHSTATIND);
                pDecPacket->u.ctkPkt.stevt.chStateInd   = GET_FIELD(  VITA49_VRT_STEVT_CHSTATIND, pu32EncPkt[idx++] );      
            }

            // Data payload format (2 Word, Optional)
            pDecPacket->u.ctkPkt.plfmt.present = GET_FIELD( VITA49_VRT_CTXIND_DPLFMT, pu32EncPkt[ciIdx] );
            if (  pDecPacket->u.ctkPkt.plfmt.present )
            {
                // Packing method
                pDecPacket->u.ctkPkt.plfmt.packMethod = GET_FIELD( VITA49_VRT_PLFMT0_PACK, pu32EncPkt[idx] );                    

                // Data type
                pDecPacket->u.ctkPkt.plfmt.dataType = GET_FIELD( VITA49_VRT_PLFMT0_DTYP, pu32EncPkt[idx] );                        

                ///< Data item format
                pDecPacket->u.ctkPkt.plfmt.dataItemFmt = GET_FIELD( VITA49_VRT_PLFMT0_DMFT, pu32EncPkt[idx] );                 

                // Item packing field size 
                pDecPacket->u.ctkPkt.plfmt.itemPackSize = GET_FIELD( VITA49_VRT_PLFMT0_IPACKFSZ, pu32EncPkt[idx] );                    

                // Data item size
                pDecPacket->u.ctkPkt.plfmt.dataItemSize = GET_FIELD( VITA49_VRT_PLFMT0_DATAISZ, pu32EncPkt[idx] );   

                // Skip the second words
                idx += 2;
            }

            // Formatted GPS (Global Positioning System) Geolocation (Unsupported, skip 11 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_GPS, pu32EncPkt[ciIdx]) ) idx += 11;     

            // Formatted INS (Inertial Navigation System) Geolocation (Unsupported, skip 11 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_INS, pu32EncPkt[ciIdx]) ) idx += 11;     

            // ECEF (Earth-Centered, Earth-Fixed) Ephemeris (Unsupported, skip 13 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_ECEFE, pu32EncPkt[ciIdx]) ) idx += 13;     

            // Relative Ephemeris (Unsupported, skip 13 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_RELE, pu32EncPkt[ciIdx]) ) idx += 13;     

            // Ephemeris Reference Identifier (Unsupported, skip 1 Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_EREFID, pu32EncPkt[ciIdx]) ) idx += 1;     

            // GPS ASCII (Unsupported, skip Variable Words)
            if ( GET_FIELD(VITA49_VRT_CTXIND_GPSA, pu32EncPkt[ciIdx]) )
            {
                idx += 1;                   // Skip the first words
                idx += pu32EncPkt[idx++];   // Skip the required number of words 
            }

            // Context association lists 
            if ( GET_FIELD(VITA49_VRT_CTXIND_CA, pu32EncPkt[ciIdx]) )
            {
                // Get the list sizes
                uint32_t srcListSize = GET_FIELD( VITA49_VRT_CALHDR0_SRCLISTSZ, pu32EncPkt[idx] );
                uint32_t sysListSize = GET_FIELD( VITA49_VRT_CALHDR0_SYSLISTSZ, pu32EncPkt[idx++] );
                uint32_t vecListSize = GET_FIELD( VITA49_VRT_CALHDR1_VCLISTSZ,  pu32EncPkt[idx] );
                uint32_t a           = GET_FIELD( VITA49_VRT_CALHDR1_A,         pu32EncPkt[idx] );
                uint32_t achListSize = GET_FIELD( VITA49_VRT_CALHDR1_ACLISTSZ,  pu32EncPkt[idx++] );

                // Skip the source list
                idx += srcListSize;

                // Skip the system list
                idx += sysListSize;

                // Skip the vector-component list
                idx += vecListSize;

                // Skip the asynchronous-channel list
                idx += achListSize;

                // Skip the asynchronous-channel tag List
                if ( a )
                {
                    // When present, it has the same size as the asynchronous-channel list
                    idx += achListSize;
                }
            }
            return vrtSize;
        }

        // Extension context packet
        case VITA49_VRT_CTXPKT_HEADER_PKTTYPE_ECTX:
        {
            uint32_t c, vrtSize, err;

            // Header (1 Word, Mandatory)
            pDecPacket->u.eCtkPkt.pktCnt            = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_PKTCNT, pu32EncPkt[idx] );  
            pDecPacket->u.eCtkPkt.timestamp.TSI     = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_TSI,    pu32EncPkt[idx] ); 
            pDecPacket->u.eCtkPkt.timestamp.TSF     = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_TSF,    pu32EncPkt[idx] );  
            pDecPacket->u.eCtkPkt.timestamp.general = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_TSM,    pu32EncPkt[idx] );  

            c       = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_C,       pu32EncPkt[idx] );   
            vrtSize = GET_FIELD( VITA49_VRT_CTXPKT_HEADER_PKTSIZE, pu32EncPkt[idx++] );  

            if ( vrtSize < 1 )
            {
                #if RTDExSync_DBG 
                    printf( "RTDExSync DBG(CTX)  : Received a VRT packet with an invalid size (%d words)\n", vrtSize );
                #endif  // RTDExSync_DBG 

                pDecPacket->pktType = 0;        // Make sure the packet type is invalid
                return RTDEXSYNCINVPKTSIZE;
            }

            // Stream Identifier (1 Word, Optional)
            pDecPacket->u.eCtkPkt.u32StreamID = GET_FIELD( VITA49_VRT_STREAMID, pu32EncPkt[idx++] );  

            // Class Identifier (2 Words, Optional)
            if ( c ) idx += 2;

            // Integer-seconds Timestamp (1 Word, Optional)
            if ( pDecPacket->u.eCtkPkt.timestamp.TSI != VITA49_VRT_DATAPKT_HEADER_TSI_NONE )
            {
                pDecPacket->u.eCtkPkt.timestamp.u32IntTs = pu32EncPkt[idx++];  
            }

            // Fractional-seconds Timestamp (2 Words, Optional)
            if ( pDecPacket->u.eCtkPkt.timestamp.TSF != VITA49_VRT_DATAPKT_HEADER_TSF_NONE )
            {
                pDecPacket->u.eCtkPkt.timestamp.u64FracTs  = (uint64_t)pu32EncPkt[idx++] << 32u;  
                pDecPacket->u.eCtkPkt.timestamp.u64FracTs |= (uint64_t)pu32EncPkt[idx++] & 0x0FFFFFFFF;  
            }

            // Error Indicators 
            err = GET_FIELD( VITA49_VRT_ECTXPKT_ERR_ERRIND, pu32EncPkt[idx++] );  
            pDecPacket->u.eCtkPkt.err.invCommand   = !!(err & VITA49_VRT_ECTXPKT_ERR_ERRIND_INVCMD);
            pDecPacket->u.eCtkPkt.err.invTimestamp = !!(err & VITA49_VRT_ECTXPKT_ERR_ERRIND_INVTS);
            pDecPacket->u.eCtkPkt.err.vrtPktErr    = !!(err & VITA49_VRT_ECTXPKT_ERR_ERRIND_VRTPKTERR);
            pDecPacket->u.eCtkPkt.err.vrlFrameErr  = !!(err & VITA49_VRT_ECTXPKT_ERR_ERRIND_VRLFRERR);
            pDecPacket->u.eCtkPkt.err.vrlSyncLoss  = !!(err & VITA49_VRT_ECTXPKT_ERR_ERRIND_VRLSYNCLOST);

            /* Currently unused and always set to 0 */
            // RTDEx stats
            pDecPacket->u.eCtkPkt.RTDEx.u32LostFrameCnt = pu32EncPkt[idx++];    // RTDEx lost frame counter
            pDecPacket->u.eCtkPkt.RTDEx.u32DropFrameCnt = pu32EncPkt[idx++];    // RTDEx drop frame counter
            pDecPacket->u.eCtkPkt.RTDEx.u32RecvFrameCnt = pu32EncPkt[idx++];    // RTDEx receive frame counter
            pDecPacket->u.eCtkPkt.RTDEx.u32XmitFrameCnt = pu32EncPkt[idx++];    // RTDEx transmit frame counter

            // Integer-seconds Miss-Timed Timestamp (1 Word, Optional)
            pDecPacket->u.eCtkPkt.invTimestamp.u32IntTs = pu32EncPkt[idx++];  

            // Fractional-seconds Miss-Timed Timestamp (2 Words, Optional)
            pDecPacket->u.eCtkPkt.invTimestamp.u64FracTs  = (uint64_t)pu32EncPkt[idx++] << 32u;  
            pDecPacket->u.eCtkPkt.invTimestamp.u64FracTs |= (uint64_t)pu32EncPkt[idx++] & 0x0FFFFFFFF;  

            return vrtSize;
        }
        default:
        {
            // Invalid packet type
            #if RTDExSync_DBG 
                printf( "RTDExSync DBG: Received a VRT packet with an unsupported type (%d)\n", pDecPacket->pktType );
            #endif  // RTDExSync_DBG 
            return RTDEXSYNCINVPKTTYPE;
        }
    }
    return 0;
}

/****************************************************************************
 * Function: RTDExSync_Monitor
 *****************************************************************************
 *
 * Wait for a Vita-49 context packet and act accordingly.
 *
 * @param [in]  hRTDExSync
 *      A RTDExSync handle.
 *
 * @param [in]  u32EvtQueueSize
 *      Size of the event queue. Use 0 to deactivate the event monitoring.
 *
 * @param [out]  pEvtQueue
 *      Array of event structures to be filled by the routine. Its lenght must match
 *      the u32EvtQueueSize parameter.
 *
 * @param [out]  pu32EvtCount
 *      Address of a variable that receives the number of event written to the queue.
 *
 * @param [in]  u32TimeoutMs 
 *      Timeout in msec.
 *
 * @return
 *      Returns "1" if stopped, "0" if nor, or an error code.
 *
 ****************************************************************************/
static adp_result_t RTDExSync_Monitor( RTDExSync_Handle_t hRTDExSync, 
                                       uint32_t           u32EvtQueueSize,
                                       RTDExSync_Event_t  *pEvtQueue,
                                       uint32_t           *pu32EvtCount,
                                       uint32_t           u32TimeoutMs )
{
    Result res;
    uint32_t idx;
    uint32_t cnt;
    uint32_t vrlCnt;
    uint32_t vrlSize;
    uint32_t packetType;

    // JR : Quick fix to avoid being stuck in this loop if 1 RTDEx packet is missing
    uint32_t cnt_no_data = 0;
    uint32_t rtdex_size = 0;

    // Invalidate the last packet received
    hRTDExSync->rx.vrtPkt.pktType = 0;

    // Try to get a new VRL packet
    cnt = 0;

    while ( 1 )
    {

        // Read only the first RTDEx packet because the size is unknown:
        //  CNTX and ECNTX are only contain in 1 media packet;
        //  ULDATA can take more than 1 media packet, defined by u22MaxFrameSize.
        if(cnt == 0)
            rtdex_size = hRTDExSync->rx.u32MediaFrameSize;
        else
        {
            // Get the packet type : ULDATA, CTX, ECTX
            packetType = GET_FIELD( VITA49_VRT_DATAPKT_HEADER_PKTTYPE, hRTDExSync->rx.pu32Buffer[2] );

            // If CNTX or ECNTX, read the missing byte to get a full RTDEx media packet
            if(packetType == VITA49_VRT_CTXPKT_HEADER_PKTTYPE_CTX ||
                packetType == VITA49_VRT_CTXPKT_HEADER_PKTTYPE_ECTX)
                rtdex_size = hRTDExSync->rx.u32MediaFrameSize - cnt;

            // If ULDATA, read the missing byte to get a full VRL packet
            else // ULDATA
                rtdex_size = hRTDExSync->rx.u22MaxFrameSize - cnt;
        }

        // If no more byte to read
        if(rtdex_size == 0)
            break;

	    res = RTDExReceive( hRTDExSync->rx.hRTDEx, 
                    &((uint8_t *)hRTDExSync->rx.pu32Buffer)[cnt],
                    rtdex_size, u32TimeoutMs ? eRTDExWaitTimeout : eRTDExDontWait, 
                    u32TimeoutMs );
	    if ( FAILURE(res) ) 
        {
            return RTDExGetLastError( hRTDExSync->rx.hRTDEx );
        }

        cnt += res;

        if ( cnt == 0 )
        {
            // Return timeout if nothing was available
            return RTDEXSYNCTIMEOUT;
        }

        // JR : Quick fix to avoid being stuck in this loop if 1 RTDEx packet is missing
        if ( cnt != 0 && res == 0 )
        {
            if (u32TimeoutMs)
                cnt_no_data += u32TimeoutMs;
            else
                cnt_no_data += 1;

            // Wait 1000 ms before timeout
            if(cnt_no_data > 1000)
            {
                // Return timeout if nothing was available
                return RTDEXSYNCTIMEOUT;
            }
        }
        else
        {
            cnt_no_data = 0;
        }

        // JR : Quick fix to avoid being stuck in this loop if 1 RTDEx packet is missing
        if ( GET_FIELD( VITA49_VRL_FAW_VRLP, hRTDExSync->rx.pu32Buffer[0]) != 'VRLP' )
        {
            cnt = 0;
        }

    }

    // Clear the remote Indicators
    hRTDExSync->ind.calTimeChInd   = 0;
    hRTDExSync->ind.validDataChInd = 0;
    hRTDExSync->ind.refLockChInd   = 0;
    hRTDExSync->ind.sampLossChInd  = 0;
    hRTDExSync->ind.chStateChInd   = 0;

    /*
     * Parse the VRL frame
     */    	
    idx = 0;

    // Validate the VRL Magic ID
    if ( GET_FIELD( VITA49_VRL_FAW_VRLP, hRTDExSync->rx.pu32Buffer[idx]) != 'VRLP' )
    {
        return RTDEXSYNCPROTOERROR;
    }
    idx++;

    // Get the VRL frame size and validate it
    vrlSize = GET_FIELD( VITA49_VRL_HEADER_FRSIZE, hRTDExSync->rx.pu32Buffer[idx] );
    if ( (vrlSize < 3) || (vrlSize > (hRTDExSync->rx.u22MaxFrameSize/sizeof(uint32_t))) )
    {
        return RTDEXSYNCINVPKTSIZE;
    }

    // Validate the VRL CRC (optional)
    #ifdef RTDEXSYNC_CRC32
        if ( RTDExSync_Crc32( 0xFFFFFFFF, hRTDExSync->rx.pu8Buffer, vrlSize ) != 0x00000000 )
    #else
        if ( GET_FIELD( VITA49_VRL_TRAILER_VEND, hRTDExSync->rx.pu32Buffer[vrlSize-1]) != 'VEND' )
    #endif
    {
        // Bad CRC
        return RTDEXSYNCPROTOERROR;
    }

    // Validate the VRL frame count
    vrlCnt = GET_FIELD( VITA49_VRL_HEADER_FRCOUNT, hRTDExSync->rx.pu32Buffer[idx] );
    if ( vrlCnt != hRTDExSync->rx.u12VrlFrameCount )
    {
        // Oops, we miss some frames !!        
        hRTDExSync->rx.u32MissVrlFrameCount++;        
    }
    hRTDExSync->rx.u12VrlFrameCount = (vrlCnt + 1) & 0x0FFF;
    idx++;


    /*
     * Parse the VRT frame(s)
     */
    vrlSize -= 3;   // Remove the VRL header and CRC from the size
    while ( vrlSize > 0 )
    {
        res = RTDExSync_DecodeVrtPacket( &hRTDExSync->rx.pu32Buffer[idx], vrlSize * sizeof(uint32_t), &hRTDExSync->rx.vrtPkt );
        if ( FAILURE(res) ) return res;

        // Substract the size of the VRT packet from the remaining VRL frame size
        vrlSize -= res;
        idx     += res;

        switch ( hRTDExSync->rx.vrtPkt.pktType )
        {
            // UL Data Packet
            case VITA49_VRT_DATAPKT_HEADER_PKTTYPE_ULDATA:
            {
                // Validate the packet count
                if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt != hRTDExSync->rx.u4VrtPacketCount )
                {
                    // Oops, we have missed some packet !!
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(ULDATA): <%5d/%2d:%08X/%016llX> Received a VRT packet with a wrong packet count. RX %d, EXPECTED %d\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt, hRTDExSync->rx.u4VrtPacketCount );
                    #endif  // RTDExSync_DBG 
                }
                hRTDExSync->rx.u4VrtPacketCount = (hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt + 1) & 0x0F;

                // Validate the channel number
                if ( (hRTDExSync->rx.vrtPkt.u.ulDataPkt.u32StreamID & 0x0FF) != hRTDExSync->ch.u8Idx )
                {
                    return RTDEXSYNCPROTOERROR;
                }

                // Validate the session ID
                if ( (hRTDExSync->rx.vrtPkt.u.ulDataPkt.u32StreamID >> 16) != hRTDExSync->ch.u16SessionId )
                {
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(ULDATA): <%5d/%2d:%08X/%016llX> Received a VRT packet with a wrong session ID. RX %d, EXPECTED %d\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ulDataPkt.u32StreamID >> 16, hRTDExSync->ch.u16SessionId );
                    #endif  // RTDExSync_DBG 

                    // Silently discard, it is probably a remaining of a previous session !!
                    break;
                }

                // Parse the Sate and Event Indicators 

#if 0   // YGOD: We ignore them for now, reactive this section if it ever become useful

                // Calibrated time indicator
                //      When set to one, shall indicate that the Timestamps in the Context Packet Stream and in the 
                //      associated Data Packet Stream are calibrated to some external reference. When set to zero
                //      this Indicator shall indicate that the Timestamps are free-running and may be inaccurate.
                if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.calTimeIndEn )
                {
                    if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.calTimeInd != hRTDExSync->ind.calTimeInd )
                    {
                        hRTDExSync->ind.calTimeInd = hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.calTimeInd;
                        hRTDExSync->ind.calTimeChInd = 1; 

                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(ULDATA): <%5d/%2d:%08X/%016llX> Calibrated time indicator: %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.calTimeInd );
                        #endif  // RTDExSync_DBG 
                    }
                }

                // Valid data indicator
                //      When set to one, shall indicate that the Data in the associated Data packet is valid. When 
                //      set to zero it shall indicate that some condition exists that may invalidate the Data.
                if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.validDataIndEn )
                {
                    if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.validDataInd != hRTDExSync->ind.validDataInd )
                    {
                        hRTDExSync->ind.calTimeInd = hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.validDataInd;
                        hRTDExSync->ind.validDataChInd = 1; 

                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(ULDATA): <%5d/%2d:%08X/%016llX> Valid data indicator: %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.validDataInd );
                        #endif  // RTDExSync_DBG 
                    }
                }

                // Reference lock indicator
                //      When set to one, shall indicate that any phase-locked loops (PLL) affecting the Described 
                //      Signal are locked and stable. When set to zero it shall indicate that at least one PLL is not
                //      locked and stable.
                if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.refLockIndEn )
                {
                    if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.refLockInd != hRTDExSync->ind.refLockInd )
                    {
                        hRTDExSync->ind.refLockInd = hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.refLockInd;
                        hRTDExSync->ind.refLockChInd = 1; 

                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(ULDATA): <%5d/%2d:%08X/%016llX> Reference lock indicator: %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.refLockInd );
                        #endif  // RTDExSync_DBG 
                    }
                }

                // Sample loss indicator
                //      When the Timestamp Mode is set to fine resolution (the TSM bit is set to zero) , the Sample Loss
                //      Indicator, when set to one, shall indicate that the paired Data packet contains a Data Sample 
                //      discontinuity due to processing errors and/or buffer overflow at the time given in the Context 
                //      packet Timestamp. When the Timestamp Mode is seto to coarse resolution (the TSM bit is set to one), 
                //      the Sample Loss Indicator, when set to one, shall indicate that the paired Data packet with 
                //      corresponding Timestamp contains at least one Data Sample discontinuity due to processing errors 
                //      and/or buffer overflow.
                if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.sampLossIndEn )
                {
                    if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.sampLossInd != hRTDExSync->ind.sampLossInd )
                    {
                        hRTDExSync->ind.sampLossInd = hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.sampLossInd;
                        hRTDExSync->ind.sampLossChInd = 1; 

                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(ULDATA): <%5d/%2d:%08X/%016llX> Sample loss indicator: %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.sampLossInd );
                        #endif  // RTDExSync_DBG 
                    }
                }

                // Channel state indicator
                if ( hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.chStateIndEn )
                {
                    if ( hRTDExSync->ind.chStateInd != hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.chStateInd )
                    {
                        hRTDExSync->ind.chStateInd = hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.chStateInd;
                        hRTDExSync->ind.chStateChInd = 1; 

                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(ULDATA): <%5d/%2d:%08X/%016llX> Channel state indicator: %d -> %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ulDataPkt.timestamp.u64FracTs, hRTDExSync->ch.remoteState, hRTDExSync->rx.vrtPkt.u.ulDataPkt.ind.chStateInd );
                        #endif  // RTDExSync_DBG 
                    }
                }
#endif
                break;
            }
            
            // Context packet
            case VITA49_VRT_CTXPKT_HEADER_PKTTYPE_CTX:
            {    
                // Invalidate the packet since it is porcessed in this function
                hRTDExSync->rx.vrtPkt.pktType = 0;

                // Validate the packet count
                if ( hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt != hRTDExSync->rx.u4VrtPacketCount )
                {
                    // Oops, we have missed some packet !!
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)   : <%5d/%2d:%08X/%016llX> Received a VRT packet with a wrong packet count. RX %d, EXPECTED %d\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt, hRTDExSync->rx.u4VrtPacketCount );
                    #endif  // RTDExSync_DBG 
                }
                hRTDExSync->rx.u4VrtPacketCount = (hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt + 1) & 0x0F;
                
                // Validate the channel number
                if ( (hRTDExSync->rx.vrtPkt.u.ctkPkt.u32StreamID & 0x0FF) != hRTDExSync->ch.u8Idx )
                {
                    return RTDEXSYNCPROTOERROR;
                }

                // Validate the session ID
                if ( (hRTDExSync->rx.vrtPkt.u.ctkPkt.u32StreamID >> 16) != hRTDExSync->ch.u16SessionId )
                {
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)   : <%5d/%2d:%08X/%016llX> Received a VRT packet with a wrong session ID. RX %d, EXPECTED %d\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ctkPkt.u32StreamID >> 16, hRTDExSync->ch.u16SessionId );
                    #endif  // RTDExSync_DBG 

                    // Silently discard, it is probably a remaining of a previous session !!
                    break;
                }
                
                // Parse the Sate and Event Indicators 
                if ( hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.present )
                {
                    // Calibrated time indicator
                    //      When set to one, shall indicate that the Timestamps in the Context Packet Stream and in the 
                    //      associated Data Packet Stream are calibrated to some external reference. When set to zero
                    //      this Indicator shall indicate that the Timestamps are free-running and may be inaccurate.
                    if ( hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.calTimeIndEn )
                    {
                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(CTX)   : <%5d/%2d:%08X/%016llX> Calibrated time indicator: %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.calTimeInd );
                        #endif  // RTDExSync_DBG 
                    }

                    // Valid data indicator
                    //      When set to one, shall indicate that the Data in the associated Data packet is valid. When 
                    //      set to zero it shall indicate that some condition exists that may invalidate the Data.
                    if ( hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.validDataIndEn )
                    {
                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(CTX)   : <%5d/%2d:%08X/%016llX> Valid data indicator: %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.validDataInd );
                        #endif  // RTDExSync_DBG 
                    }

                    // Reference lock indicator
                    //      When set to one, shall indicate that any phase-locked loops (PLL) affecting the Described 
                    //      Signal are locked and stable. When set to zero it shall indicate that at least one PLL is not
                    //      locked and stable.
                    if ( hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.refLockIndEn )
                    {
                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(CTX)   : <%5d/%2d:%08X/%016llX> Reference lock indicator: %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.refLockInd );
                        #endif  // RTDExSync_DBG 
                    }

                    // Sample loss indicator
                    //      When the Timestamp Mode is set to fine resolution (the TSM bit is set to zero) , the Sample Loss
                    //      Indicator, when set to one, shall indicate that the paired Data packet contains a Data Sample 
                    //      discontinuity due to processing errors and/or buffer overflow at the time given in the Context 
                    //      packet Timestamp. When the Timestamp Mode is seto to coarse resolution (the TSM bit is set to one), 
                    //      the Sample Loss Indicator, when set to one, shall indicate that the paired Data packet with 
                    //      corresponding Timestamp contains at least one Data Sample discontinuity due to processing errors 
                    //      and/or buffer overflow.
                    if ( hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.sampLossIndEn )
                    {
                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(CTX)   : <%5d/%2d:%08X/%016llX> Sample loss indicator: %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.sampLossInd );
                        #endif  // RTDExSync_DBG 

                        if ( pu32EvtCount && (*pu32EvtCount  < u32EvtQueueSize) )
                        {                            
                            pEvtQueue->id             = hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.sampLossInd ? RTDExSync_EventId_FifoUnderrun : RTDExSync_EventId_FifoResume;
                            pEvtQueue->gTiming        = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.general;
                            pEvtQueue->u32SampleCount = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs;
                            pEvtQueue->u64TickCount   = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs;
                            pEvtQueue->u32EventCount  = hRTDExSync->evt.u32EventCnt;
                            pEvtQueue++;
                            *pu32EvtCount += 1;
                        }
                        // Increment the event counter
                        hRTDExSync->evt.u32EventCnt++;
                    }

                    // Channel state indicator
                    if ( hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.chStateIndEn )
                    {
                        #if RTDExSync_DBG 
                            printf( "RTDExSync DBG(CTX)   : <%5d/%2d:%08X/%016llX> Channel state indicator: %d -> %d\n", 
                                vrlCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs, 
                                hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs, hRTDExSync->ch.remoteState, hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.chStateInd );
                        #endif  // RTDExSync_DBG 

                        // Add the event to the queue
                        if ( hRTDExSync->ch.remoteState != hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.chStateInd )
                        {
                            if ( pu32EvtCount && (*pu32EvtCount  < u32EvtQueueSize) )
                            {
                                switch ( hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.chStateInd )
                                {
                                    case RTDExSync_ChState_Idle:
                                    {
                                        pEvtQueue->id             = RTDExSync_EventId_Stopped;
                                        pEvtQueue->gTiming        = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.general;
                                        pEvtQueue->u32SampleCount = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs;
                                        pEvtQueue->u64TickCount   = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs;
                                        pEvtQueue->u32EventCount  = hRTDExSync->evt.u32EventCnt;
                                        pEvtQueue++;
                                        *pu32EvtCount += 1;
                                        break;
                                    }
                                    case RTDExSync_ChState_Armed: 
                                    {
                                        pEvtQueue->id             = RTDExSync_EventId_StartPending;
                                        pEvtQueue->gTiming        = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.general;
                                        pEvtQueue->u32SampleCount = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs;
                                        pEvtQueue->u64TickCount   = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs;
                                        pEvtQueue->u32EventCount  = hRTDExSync->evt.u32EventCnt;
                                        pEvtQueue++;
                                        *pu32EvtCount += 1;
                                        break;
                                    }
                                    case RTDExSync_ChState_Running: 
                                    {
                                        pEvtQueue->id             = RTDExSync_EventId_Started;
                                        pEvtQueue->gTiming        = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.general;
                                        pEvtQueue->u32SampleCount = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs;
                                        pEvtQueue->u64TickCount   = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs;
                                        pEvtQueue->u32EventCount  = hRTDExSync->evt.u32EventCnt;
                                        pEvtQueue++;
                                        *pu32EvtCount += 1;
                                        break;
                                    }
                                    case RTDExSync_ChState_StopPending: 
                                    {
                                        pEvtQueue->id             = RTDExSync_EventId_StopPending;
                                        pEvtQueue->gTiming        = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.general;
                                        pEvtQueue->u32SampleCount = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs;
                                        pEvtQueue->u64TickCount   = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs;
                                        pEvtQueue->u32EventCount  = hRTDExSync->evt.u32EventCnt;
                                        pEvtQueue++;
                                        *pu32EvtCount += 1;
                                        break;
                                    }
                                    default: break;
                                }
                            }

                            // Increment the event counter
                            hRTDExSync->evt.u32EventCnt++;
                        }

                        switch ( hRTDExSync->ch.localState )
                        {
                            case RTDExSync_ChState_Idle:
                            {
                                // Save the new remote state
                                hRTDExSync->ch.remoteState = hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.chStateInd;
                                break;
                            }
                            case RTDExSync_ChState_Armed:
                            case RTDExSync_ChState_Running:
                            case RTDExSync_ChState_Stopped:
                            case RTDExSync_ChState_StopPending:
                            {
                                if ( hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.chStateInd == VITA49_VRT_STEVT_CHSTATIND_IDLE )
                                {
                                    // The core has stopped, chage our current state to stopped
                                    hRTDExSync->ch.localState = RTDExSync_ChState_Stopped;
                                }

                                // Save the new remote state
                                hRTDExSync->ch.remoteState = hRTDExSync->rx.vrtPkt.u.ctkPkt.stevt.chStateInd;
                                break;
                            }
                            default: break;
                        }
                    }                
                }
                break;
            }            

            // Extension context packet
            case VITA49_VRT_CTXPKT_HEADER_PKTTYPE_ECTX:
            {
                // Invalidate the packet since it is porcessed in this function
                hRTDExSync->rx.vrtPkt.pktType = 0;

                // Validate the packet count
                if ( hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt != hRTDExSync->rx.u4VrtPacketCount )
                {
                    // Oops, we have missed some packet !!
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)  : <%5d/%2d:%08X/%016llX> Received a VRT packet with a wrong packet count. RX %d, EXPECTED %d\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt, hRTDExSync->rx.u4VrtPacketCount );
                    #endif  // RTDExSync_DBG 
                }
                hRTDExSync->rx.u4VrtPacketCount = (hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt + 1) & 0x0F;
                
                // Validate the channel number
                if ( (hRTDExSync->rx.vrtPkt.u.eCtkPkt.u32StreamID & 0x0FF) != hRTDExSync->ch.u8Idx )
                {
                    return RTDEXSYNCPROTOERROR;
                }

                // Validate the session ID
                if ( (hRTDExSync->rx.vrtPkt.u.eCtkPkt.u32StreamID >> 16) != hRTDExSync->ch.u16SessionId )
                {
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)  : <%5d/%2d:%08X/%016llX> Received a VRT packet with a wrong session ID. RX %d, EXPECTED %d\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u64FracTs, hRTDExSync->rx.vrtPkt.u.eCtkPkt.u32StreamID >> 16, hRTDExSync->ch.u16SessionId );
                    #endif  // RTDExSync_DBG 

                    // Silently discard, it is probably a remaining of a previous session !!
                    break;
                }

                //
                // Error Indicators 
                //

                // VRL sync lost
                if ( hRTDExSync->rx.vrtPkt.u.eCtkPkt.err.vrlSyncLoss )
                {
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)  : <%5d/%2d:%08X/%016llX> Received a VRL sync lost error.\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u64FracTs );
                    #endif  // RTDExSync_DBG 
                }
            
                // VRL frame error 
                if ( hRTDExSync->rx.vrtPkt.u.eCtkPkt.err.vrlFrameErr )
                {
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)  : <%5d/%2d:%08X/%016llX> Received a VRL frame error.\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u64FracTs );
                    #endif  // RTDExSync_DBG 
                }

                // VRT packet error
                if ( hRTDExSync->rx.vrtPkt.u.eCtkPkt.err.vrtPktErr )
                {
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)  : <%5d/%2d:%08X/%016llX> Received a VRT packet error.\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u64FracTs );
                    #endif  // RTDExSync_DBG 
                }

                // Invalid timestamp
                if ( hRTDExSync->rx.vrtPkt.u.eCtkPkt.err.invTimestamp )
                {
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)  : <%5d/%2d:%08X/%016llX> Received an invalid timestamp error [%016llX].\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt, 
                            hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u32IntTs, hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u64FracTs,
                            hRTDExSync->rx.vrtPkt.u.eCtkPkt.invTimestamp.u64FracTs );
                    #endif  // RTDExSync_DBG 
                        
                    if ( pu32EvtCount && (*pu32EvtCount  < u32EvtQueueSize) )
                    {
                        pEvtQueue->id               = RTDExSync_EventId_MissTimeCommand;
                        pEvtQueue->gTiming          = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.general;
                        pEvtQueue->u32SampleCount   = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u32IntTs;
                        pEvtQueue->u64TickCount     = hRTDExSync->rx.vrtPkt.u.ctkPkt.timestamp.u64FracTs;
                        pEvtQueue->u64TickCommand   = hRTDExSync->rx.vrtPkt.u.eCtkPkt.invTimestamp.u64FracTs;
                        pEvtQueue->u32EventCount    = hRTDExSync->evt.u32EventCnt;
                        pEvtQueue++;
                        *pu32EvtCount += 1;
                    }

                    // Increment the event counter
                    hRTDExSync->evt.u32EventCnt++;
                }

                //  Invalid command
                if ( hRTDExSync->rx.vrtPkt.u.eCtkPkt.err.invCommand )
                {
                    #if RTDExSync_DBG 
                        printf( "RTDExSync DBG(CTX)  : <%5d/%2d:%08X/%016llX> Received an invalid command error.\n", 
                            vrlCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.pktCnt, hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u32IntTs, 
                            hRTDExSync->rx.vrtPkt.u.eCtkPkt.timestamp.u64FracTs );
                    #endif  // RTDExSync_DBG 
                }

                /* Currently unsued and always set to 0
                //
                // RTDEx stats
                //
                #if RTDExSync_DBG 
                    printf( "RTDExSync DBG(CTX)  : RTDEx Stats:\n");
                    printf( "       Lost frames: %6d\n", hRTDExSync->rx.vrtPkt.u.eCtkPkt.RTDEx.u32LostFrameCnt );
                    printf( "       Drop frames: %6d\n", hRTDExSync->rx.vrtPkt.u.eCtkPkt.RTDEx.u32DropFrameCnt );
                    printf( "       Recv frames: %6d\n", hRTDExSync->rx.vrtPkt.u.eCtkPkt.RTDEx.u32RecvFrameCnt );
                    printf( "       Xmit frames: %6d\n\n", hRTDExSync->rx.vrtPkt.u.eCtkPkt.RTDEx.u32XmitFrameCnt );
                #endif  // RTDExSync_DBG 
                */
            
                break;
            }
            default:
            {
                return RTDEXSYNCPROTOERROR;
            }
		}
    }
    return res;
}


/****************************************************************************
 * Function: RTDExSync_GetLocalMacAddr
 *****************************************************************************
 *
 * Retrieve the HOST adapter MAC address.
 *
 * @param [in]  state
 *      Handle to connection state.
 *
 * @param [out]  pu8HostMacAddr 
 *      Address of a buffer that receives the corresponding MAC address.
 *
 * @return
*      Success or error code.
 *
 * @note
 *      This comes from the ADP sdk utils bit it is not part of any library.
 *      We don't want to for the user to include a source file in his project
 *      therefore the code of the routine has been compies in this file !!
 *
 ****************************************************************************/
static adp_result_t  RTDExSync_GetLocalMacAddr( connection_state state,
                                                uint8_t pu8HostMacAddr[6] )
{
    int i = 0;
    socklen_t len;
    adp_result_t res;
    char szIfaceDesc[261];
    char szIfaceIpAddr[16];
    struct sockaddr_in sin;
  
    // Then, get the IP address used to connect to the Perseus
    len = sizeof(sin);
    if ( getsockname(state.stream, (struct sockaddr *)&sin, &len) < 0 )
    {
        return RTDEXSYNCHOSTIPERR;
    }
    if( sin.sin_family != AF_INET ) 
    {
        return RTDEXSYNCHOSTIPERR;
    }

    // Finally, try to match the IP with one of the adapter IP address 
    while ( 1 )
    {
        res = RTDExGetMacEth( i++, pu8HostMacAddr, szIfaceIpAddr, szIfaceDesc );
        if ( FAILURE(res) ) break;

        if ( !strcmp(szIfaceIpAddr, inet_ntoa(sin.sin_addr)) ) break;
    }

    return res;
}
