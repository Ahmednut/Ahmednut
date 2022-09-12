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
 * @file      rtdex_axi.c
 * 
 * @brief     Functions definition for RTDEx access via AXI.
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>

#include "eapi.h"
#include "bas_types.h"
#include "media_types.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_axi.h"
#include "rtdex_access.h"
#include "rtdex_ioctl.h"

adp_result_t RTDExOpenAxi( adp_handle_rtdex_conn *hConn, int8_t *device_name)
{
	int dev;
	adp_handle_rtdex_conn hTempConn = NULL;
	adp_handle_rtdex_axi_conn hAxiRTDEx;

	hTempConn = (adp_handle_rtdex_conn)(malloc(sizeof(sMediaRTDExStruct)));
	hAxiRTDEx = (adp_handle_rtdex_axi_conn)(malloc(sizeof(sAxiRTDExStruct)));

	hTempConn->eMedia = eMediaAxi;
	hTempConn->hMediaRTDexConn = (adp_handle_rtdex_media_conn)hAxiRTDEx;
	dev = open(device_name, O_RDWR);
    if(dev < 0)
    {
        return RTDEXAXIOPEN;
    }	
	ioctl(dev, RTDEX_SET_READ_CAST_MODE, RTDEX_CAST_MODE_NONE);
    ioctl(dev, RTDEX_SET_WRITE_CAST_MODE, RTDEX_CAST_MODE_NONE);
    ioctl(dev, RTDEX_SET_BURST_LENGTH, 16);
    ioctl(dev, RTDEX_SET_BLOCK_SIZE, 4096);
    ioctl(dev, RTDEX_RESET_ALL);
	
	hAxiRTDEx->m_dev = dev;
	*hConn = hTempConn;
	return RTDEXOK;
}

adp_result_t RTDExCloseAxi( adp_handle_rtdex_conn hConnRtDex )
{
	adp_handle_rtdex_axi_conn pAxiConn = (adp_handle_rtdex_axi_conn)(hConnRtDex->hMediaRTDexConn);

	close(pAxiConn->m_dev);
	free( hConnRtDex->hMediaRTDexConn);
	free( hConnRtDex);
	return RTDEXOK;
}

adp_result_t RTDExStartAxi( adp_handle_rtdex_conn hConnRtDex, uint32_t u32FrameSize, uint32_t u32TransferSize)
{	
	return 0;
}

int RTDExSendAxi( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize)
{
	int ret;
	adp_handle_rtdex_axi_conn pAxiConn = (adp_handle_rtdex_axi_conn)(hConnRtDex->hMediaRTDexConn);

	ret = write(pAxiConn->m_dev, (unsigned char*)pDataBuffer, u32MaxSize);
    if(ret < 0)
    {
        return RTDEXAXISEND;
    }
	return ret;
}

int RTDExReceiveAxi( adp_handle_rtdex_conn hConnRtDex, unsigned char *pDataBuffer, unsigned int u32MaxSize, 
					  eRTDExTransferMode eMode, unsigned int uTimeOut )
{
	int ret;
	adp_handle_rtdex_axi_conn pAxiConn = (adp_handle_rtdex_axi_conn)(hConnRtDex->hMediaRTDexConn);

	ret = read(pAxiConn->m_dev, (unsigned char*)pDataBuffer, u32MaxSize);
    if(ret < 0)
    {
        return RTDEXAXIRECEIVE;
    }
	return ret;
}
