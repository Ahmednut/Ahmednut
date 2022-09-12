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
 * @file      LoadDataToPlayback.c
 * 
 * @brief     Functions that control the RAM transfer to carrier from a file
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bas_types.h"
#include "media_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "eapi_carrier.h"
#include "carrier_service_def.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_eth.h"
#include "rtdex_pci.h"
#include "recplay_types.h"
#include "recplay.h"
#include "mac_utils.h" 
#include "media_utils.h"
#include "linux_utils.h"
#include "verbose_utils.h"
#include "bas_error.h"
#include "bas_time.h"
#include "LoadDataToPlayback.h"

#define	FIFOALMOSTFULLTHRESHOLD 0.75
#define	FIFOREENABLEFLOWTHRESHOLD 0.5
// WARNING: to run this demo under LINUX, you may need root privilege

RTDExPerseusParamsPcie perseusParams;

Result LoadDataToPlayback(connection_state * state,
						 uint32_t u32Channel,
                         uint32_t u32FrameSize, // in bytes
						 uint32_t uStartAddr,   // in bytes
                         char *pcDataBuffer,
                         uint32_t uFileSize)    // in bytes
{
    adp_result_t res = 0;

	unsigned int uiRecPlayBaseAddr;
	adp_handle_recplay hRecPlay = NULL;
	unsigned int uiRTDExGigeBaseAddr;
    adp_handle_rtdex_conn hConnRecPlayTx = NULL;
	uint32_t u32StatsIsIncluded;
	uint32_t u32RTDExFifoDepth;
    
	eMediaType eMedia;
    unsigned char pHostMacAddr[6];
    unsigned char pDeviceMacAddr[6];   
    unsigned char uInitDone;
    unsigned int uPlaybackRtdexFrameSize;

    /*************************************************************************
        Opening interface with the Record Playback module
    *************************************************************************/
    // Detecting RTDEx media
	res = DetectMedia(state, &eMedia);
	if(FAILURE(res))
    {
        bas_printf(INFO, "\n");
        bas_printf(INFO, "Cannot determine RTDEx media type. Make sure an RTDEx core is used in your FPGA design.\n");
        goto end;
    }

    bas_printf(INFO,"The data will be transfered through %s\n",
        (eMedia == eMediaPcie) ? "RTDEx PCIe" : "RTDEx Gigabit Ethernet");

    res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRecPlay, 0, &uiRecPlayBaseAddr);
    if(FAILURE(res)) return res;

	// Record/Playback initialization
	res = RecPlay_Open( &hRecPlay, state, uiRecPlayBaseAddr );
    if(FAILURE(res))
    {
        bas_printf(INFO, "\n");
        bas_printf(INFO, "Cannot open Record Playback module. Make sure the Record Playback core is used in your FPGA design.\n");
        goto end;
    }
    
    /*************************************************************************
        Verify memory status and reset the core
    *************************************************************************/
    res = RecPlayGetMemoryInitDoneStatus( hRecPlay, &uInitDone );
    if(FAILURE(res)) goto end;

    if( uInitDone == 0 )
    {
        bas_printf(ERR, "The DDR3 sodimm initialization is not done.\n"
                "Check if the DDR3 chip is correctly connected.\n" );
        goto end;
    }

    bas_printf(INFO, "Resetting Record Playback module...\n" );
    res = RecPlayResetCore( hRecPlay );
    if(FAILURE(res)) goto end;
    
    /*************************************************************************
        Opening interface with the RTDEx module
    *************************************************************************/
    if(eMedia == eMediaEth)
    {
		res = Carrier_GetModuleBaseAddr_send( state, Carrier_eModuleRTDExGigE, 0, &uiRTDExGigeBaseAddr);
		if(FAILURE(res)) goto end;

        // reset RTDEx core
        res = RTDExResetCoreEth( state, uiRTDExGigeBaseAddr);
        if(FAILURE(res)) goto end;

		res = RTDExGetRxConfigExtentedEth( state, uiRTDExGigeBaseAddr, 
            &u32RTDExFifoDepth, &u32StatsIsIncluded );
        if(FAILURE(res)) goto end;

		res = RTDExEnableFlowControlEth( state, uiRTDExGigeBaseAddr, 
            (uint32_t)(FIFOALMOSTFULLTHRESHOLD * u32RTDExFifoDepth), 
            (uint32_t)(FIFOREENABLEFLOWTHRESHOLD * u32RTDExFifoDepth) );
        if(FAILURE(res)) goto end;
        
        res = GetMACAddress(state, (char *)pHostMacAddr, (char *)pDeviceMacAddr);
        if(FAILURE(res))
        {
            bas_printf(ERR, "Error getting MAC Address, Code: 0x%08X.\n\n", res );
            goto end;
        }
        //opening RTDEx channel for playback
        res = RTDExOpenHostAndFpgaEth(&hConnRecPlayTx,
                            state,
                            uiRTDExGigeBaseAddr,
                            u32Channel,
                            eToFpga, 
                            eSingle);
        if(FAILURE(res))
        {         
            bas_printf(ERR, "Error opening the RTDEx channel.\n" );
            goto end;
        }
    }
    else if(eMedia == eMediaPcie)
    {
        RTDExInitParamsPcie(&perseusParams, state);
        res = RTDExResetCorePcie(&perseusParams);
        if(FAILURE(res)) goto end;

        res = RTDExOpenHostAndFpgaPcie( &hConnRecPlayTx, &perseusParams, u32Channel, eToFpga, eSingle);
        if(FAILURE(res))
        {
            bas_printf(ERR, "Error opening the RTDEx channel.\n" );
            goto end;
        }
    }
    else
    {
        bas_printf(ERR, "Unsupported media.\n" );
        res = RTDEXMEDIANOTSUPPORTED;
        goto end;
    }

    //Send data to the memory via RTDEx         //Send data to the memory via RTDEx 
    uPlaybackRtdexFrameSize = ( uFileSize < u32FrameSize ) ? uFileSize : u32FrameSize ;

    //setting record and playback core to Host2Mem mode
	//printf("put = %u\n", uStartAddr);
    bas_printf(INFO, "Setting the Record Playback module in host to memory transfer mode...\n" );
    res = RecPlaySetModeRtdexHost2Mem( hRecPlay, uStartAddr, uFileSize );
    if( res != 0 ) goto end;

    //Send data to the memory via RTDEx 
    res = RTDExStart( hConnRecPlayTx, uPlaybackRtdexFrameSize, uFileSize);
    if( res != 0 )
    {
        bas_printf(ERR, "Error starting RTDEx transfer.\n");
        goto end;
    }

    //Send data to FPGA
    bas_printf(INFO, "Sending the data through RTDEx...\n" );
    res = RTDExSend(hConnRecPlayTx, (unsigned char *)pcDataBuffer, uFileSize);
    if (FAILURE(res))
    {
        res = RTDExGetLastError( hConnRecPlayTx );
        bas_printf(ERR, "Error with RTDEx transfer.\n");
        goto end;
    }
    else
    {
        if(res != uFileSize)
        {
            bas_printf(ERR, "Error sending RTDEx data.\n");
            goto end;
        }
    }
    
    res = RecPlayWaitTransferDone(hRecPlay,1000);
    if(FAILURE(res))
    {
        bas_printf(ERR, "1 second after the last RTDEx send, the host to memory transfer is still not over.\n");
        bas_printf(ERR, "This can be caused by data loss during the transfer.\n");
        goto end;
    }
    
    bas_printf(INFO, "Done.\n");
    res = OK;
    
end:
    if(hConnRecPlayTx) RTDExClose(hConnRecPlayTx);
    if(hRecPlay) RecPlay_Close(hRecPlay);
    fflush(stdout);
    return res;
}

Result LoadDataToPlaybackFromFile(connection_state * state,
						          uint32_t u32Channel,
                                  uint32_t u32FrameSize, // in bytes
						          uint32_t u32StartAddress, // in bytes
						          char *pcFileName) 
{
    adp_result_t res = 0;

    FILE * pfile;
    int64_t i64FileSize;
    char* pcPlayBackBuffer = NULL;
    unsigned int uByteRead;
    
    /*************************************************************************
        Opening file
    *************************************************************************/
    bas_printf(INFO, "Opening %s file...\n",pcFileName);
    pfile = fopen(pcFileName,"rb");
    if (pfile == NULL)
    {
		bas_printf(ERR,"Error opening file, make sure the following path is correct: %s\n", pcFileName);
		fflush(stdout);
        return EAPIFILE;
    }
    
    /*************************************************************************
        Get the file size
    *************************************************************************/
    if(_fseeki64( pfile, 0, SEEK_END ) !=0 )
	{
		fclose(pfile);
		return EAPIFILE;
	}
	i64FileSize = _ftelli64( pfile );
    _fseeki64( pfile, 0, SEEK_SET );
    
    /*************************************************************************
        Malloc Buffer
    *************************************************************************/
    pcPlayBackBuffer = (char *)malloc(i64FileSize);
    if (pcPlayBackBuffer == NULL)
    {
        bas_printf(ERR,"Cannot allocate the memory from storing the file content.\n");
		fclose(pfile);
        fflush(stdout);
		return ERR_OUTOFMEMORY;
    }
    
    /*************************************************************************
        Read the complete file into the allocated buffer
    *************************************************************************/
    bas_printf(INFO, "Reading the file content...\n");
    uByteRead = (unsigned int)fread(pcPlayBackBuffer, 1, i64FileSize, pfile);
    if(uByteRead != i64FileSize )
    {
        bas_printf(ERR,"Cannot read all the data from the file.\n");
        free(pcPlayBackBuffer);
        fclose(pfile);
		fflush(stdout);
        return EAPIFILE;
    }
    
    fclose(pfile);
    
    /*************************************************************************
        Send the file content into the playback memory
    *************************************************************************/
    res = LoadDataToPlayback(state, u32Channel, u32FrameSize, u32StartAddress, pcPlayBackBuffer, (uint32_t)i64FileSize);
    if(FAILURE(res))
    {
        free(pcPlayBackBuffer);
        return res;
    }
    
    free(pcPlayBackBuffer);
    fflush(stdout);
	return OK;
}
