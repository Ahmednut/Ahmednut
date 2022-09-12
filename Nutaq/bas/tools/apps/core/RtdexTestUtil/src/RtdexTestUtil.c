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
 * @file      RtdexTestUtil.c
 * 
 * @brief     Function for initializing the timestamp module or using some of its features
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
#include "eapi_carrier.h"
#include "bas_types.h"
#include "bas_error.h"
#include "bas_time.h"
#include "media_types.h"
#include "connection_state.h"
#include "eapi.h"
#include "rtdex_types.h"
#include "rtdex_eth_types.h"
#include "rtdex_media.h"
#include "rtdex_access.h"
#include "rtdex.h"
#include "rtdex_eth.h"
#include "media_utils.h" 
#include "linux_utils.h"
#include "terminal_utils.h"
#include "RtdexTestUtil.h"


/************************************************************************************************************************
											   Rtdex Test Util
************************************************************************************************************************/
Result RtdexTestUtil(char *pcIpAddress, handle_rtdextestparams hParams, char acAction[])
{
	adp_result_t res;	
    connection_state state;
    unsigned int *puSendData = NULL; 
    unsigned int u32AllocatedSize;
    unsigned int i;
    unsigned int j;
    FILE *fp;
    int32_t i32FileSize;
    char acSeperatedFileNames[MAX_RTDEX][60];
    uint32_t u32ExpectedValue = 0;
    uint32_t u32PresentValue = 0;
    uint32_t u32DownlinkErrorCount = 0;
    uint32_t u32DownlinkSampleCount = 0;
    uint32_t u32UplinkErrorCount[MAX_RTDEX]= {0};
    unsigned int u32FileAnalyzed[MAX_RTDEX]= {0};
    uint32_t u32UplinkOverflowCount = 0;
    uint32_t u32DownlinkUnderflowCount = 0;
    const char cFilenameDelim = ',';
    uint32_t u32NumberOfFiles = 0;

    /*
     * Connect to the board
     */

    // Connect the Perseus AMC
    printf( "Connecting to the board..."); 
    fflush(stdin);
    res = connect_cce(pcIpAddress, &state);
    if ( FAILURE(res) )
    {
       fprintf( stderr, "The Perseus is not responding.\n" );
       fprintf( stderr, "Please ensure the Perseus IP address you specified (%s) is correct.\n", pcIpAddress );
       goto error;
    }
    printf( " Done\n" );

    printf("Action: %s\n\n", acAction);
    if(!strcmp(acAction, "init"))
    {
        //reset test
        res = RTDExResetTest(&state, hParams->u32RtdexTestBaseCustomRegister);
        if(FAILURE(res))
		{
			goto error;
		}
		res = RTDExSetTestInitVal(&state, hParams->u32RtdexTestBaseCustomRegister, 0);
        if(FAILURE(res))
        {
            goto error;
        }
		res = RTDExSetTestCntTxDiv(&state, hParams->u32RtdexTestBaseCustomRegister, hParams->u32TestRate);
        if(FAILURE(res))
        {
            goto error;
        }

        // set test rate 
	    res = RTDExSetTestCntRxDiv(&state, hParams->u32RtdexTestBaseCustomRegister, hParams->u32TestRate);
        if(FAILURE(res))
        {
        	goto error;
        }

        if(hParams->bAdcDacMode) // if test mode is ADC/DAC emulation
	    {
            res = RTDExEnableAdcDacMode( &state, hParams->u32RtdexTestBaseCustomRegister );
            if(FAILURE(res))
            {
            	goto error;
            }
	    }
	    else	// if test mode is normal mode
	    {
		    res = RTDExDisableAdcDacMode( &state, hParams->u32RtdexTestBaseCustomRegister );
            if(FAILURE(res))
            {
            	goto error;
            }
	    }
        // allocation of data buffers
        if(hParams->bSingleTransfer)
	    {
            //u32AllocatedSize = hParams->u32TransferSize + 10000;
            u32AllocatedSize = hParams->u32TransferSize;
	    }
	    else
	    {
		    u32AllocatedSize = MAX_TRANSFER_SIZE + 10000;
	    }

        if(hParams->u32Direction == DOWNLINK || hParams->u32Direction == BOTH)
        {
            u32NumberOfFiles = ParseFileNames(hParams->pcFileNames, acSeperatedFileNames, cFilenameDelim);

            for(i = 0; i < u32NumberOfFiles; i++)
            {
                if(!_stricmp(acSeperatedFileNames[i], "none"))
                    continue;

                fp = fopen( acSeperatedFileNames[i], "wb");
                if ( fp == NULL )
                {
                    perror( "fopen");
                    goto error;
                }
	
                // generate source data
                for(j = 0; j < u32AllocatedSize / 4; j++)
                {
	                if(fwrite(&j, sizeof(int), 1, fp) != 1)
                    {
                        printf("Error with fwrite while writing ramp file.");
                        goto error;
                    }
                }
                if(fclose(fp))
                { 
                    printf("Error closing generated ramp file.");
                    goto error;
                }
            }
        }
    }
    else if(!strcmp(acAction, "enable"))
    {
        if(hParams->u32Direction == DOWNLINK || hParams->u32Direction == BOTH)
        {
            res = RTDExEnableDownlinkTest( &state, hParams->u32RtdexTestBaseCustomRegister, 1);
            if(FAILURE(res))
            {
                goto error;
            }
        }
        else if(hParams->u32Direction == UPLINK || hParams->u32Direction == BOTH)
        {
            res = RTDExEnableUplinkTest( &state, hParams->u32RtdexTestBaseCustomRegister, 1);
            if(FAILURE(res))
            {
                goto error;
            }
        }

    }
    else if(!strcmp(acAction, "stop"))
    {

        if(hParams->u32Direction == DOWNLINK || hParams->u32Direction == BOTH)
        {
            res = RTDExEnableDownlinkTest( &state, hParams->u32RtdexTestBaseCustomRegister, 0);
            if(FAILURE(res))
            {
                goto error;
            }
        }
        if(hParams->u32Direction == UPLINK || hParams->u32Direction == BOTH)
        {
            res = RTDExEnableUplinkTest( &state, hParams->u32RtdexTestBaseCustomRegister, 0);
            if(FAILURE(res))
            {
                goto error;
            }
        }
        	// check data ramp:
        if(hParams->u32Direction == UPLINK || hParams->u32Direction == BOTH)
        {
            u32NumberOfFiles = ParseFileNames(hParams->pcFileNames, acSeperatedFileNames, cFilenameDelim);
            for( i = 0; i < u32NumberOfFiles; i++)
            {
                u32ExpectedValue = 0;
                if(!_stricmp(acSeperatedFileNames[i], "none"))
                    continue;
                else
                fp = fopen( acSeperatedFileNames[i], "rb");
                if(fp == NULL)
                {
                    printf("Error with fopen while analyzing received ramp file.");;
                    goto error;
                }
                if(fseek(fp, 0, SEEK_END))
                {
                    printf("Error analyzing received ramp file.");
                    goto error;
                }
                i32FileSize = ftell(fp);
                if(i32FileSize < 0)
                {
                    printf("Error analyzing received ramp file.");
                    goto error;
                }
                if(fseek(fp, 0, SEEK_SET))
                {
                    printf("Error analyzing received ramp file.");
                    goto error;
                }
	            for( j = 0; j < (uint32_t)i32FileSize /4; j++ ) 
	            {
                    if(fread(&u32PresentValue, sizeof u32PresentValue, 1, fp) != 1)
                    {
                        printf("Error analyzing received ramp file.");
                        goto error;
                    }
		            if( u32PresentValue != u32ExpectedValue) 
		            {
                        u32UplinkErrorCount[i]++;
			            u32ExpectedValue = u32PresentValue + 1;
		            }
                    else 
		            {
			            u32ExpectedValue++;
		            }
	            }
                u32FileAnalyzed[i] = 1;
                if(fclose(fp))
                {
                    printf("Error closing received ramp file.");
                    goto error;
                }
            }
        }
        for( i = 0; i < hParams->u32NumberOfChannelsToAnalyze; i++)
        {
	        if(hParams->u32Direction == UPLINK || hParams->u32Direction == BOTH)
	        {
                if(u32FileAnalyzed[i])
                {
			        printf( "  - Sample(s) in error (Uplink), Ch %d):%18u \n", i, u32UplinkErrorCount[i]);
                }

			    if(hParams->bAdcDacMode)
			    {
				    res = RTDExGetUplinkOverFlow( &state, hParams->u32RtdexTestBaseCustomRegister, i, &u32UplinkOverflowCount);
				    printf( "  - Uplink Overflow (4 Bytes sample) (Uplink, Ch %d):%7u \n\n", i, u32UplinkOverflowCount);
			    }
	        }
        }

        for( i = 0; i < hParams->u32NumberOfChannelsToAnalyze; i++)
        {
            if(hParams->u32Direction == DOWNLINK || hParams->u32Direction == BOTH)
            {
                res = RTDExGetDownlinkErrorCnt( &state, hParams->u32RtdexTestBaseCustomRegister, i, &u32DownlinkErrorCount);
                res = RTDExGetDownlinkReceivedSamples( &state, hParams->u32RtdexTestBaseCustomRegister, i, &u32DownlinkSampleCount);

                printf( "  - Sample(s) in error (Downlink, Ch %d):%25u \n", i, u32DownlinkErrorCount);
                printf( "  - Sample(s) received by RtdexTest Core (Downlink, Ch %d):%7u \n", i, u32DownlinkSampleCount);
                if(hParams->bAdcDacMode)
	            {
                    res = RTDExGetDownlinkUnderFlow( &state, hParams->u32RtdexTestBaseCustomRegister, i, &u32DownlinkUnderflowCount);
		            printf( "  - Downlink Underflow (4 Bytes sample) (Downlink, Ch %d):%7u \n\n", i, u32DownlinkUnderflowCount);
	            }
            }
        }
    }
    else
    {
        printf("Error with action parameter. Value must be in the range 0 - 2.\n");
        goto error;
    }


	/************************************************************************************************************************
												Closing connection to Perseus
	************************************************************************************************************************/

error:
    if ( FAILURE(res) )
    {
        fprintf( stderr, "%s\n\n\n", ErrorToString(res) );
    }
    disconnect_cce(&state);
    return res;
}

// enable RX or TX tester
adp_result_t RTDExEnableUplinkTest( connection_state *state, uint32_t u32BaseCustomRegister, unsigned char uEnable)
{
    adp_result_t res;
    unsigned int Value;
    unsigned int Mask = 0x00ffffff; 

    res = custom_register_read_send( state, u32BaseCustomRegister + RTDEX_CTRL, &Value);
    if(FAILURE(res))
        return res;

    Value &= Mask;
    Value |= uEnable ? TX_START_BM_all : 0;

    res = custom_register_write_send( state, u32BaseCustomRegister + RTDEX_CTRL, Value);

    return res;
}

adp_result_t RTDExEnableDownlinkTest( connection_state *state, uint32_t u32BaseCustomRegister, unsigned char uEnable)
{
    adp_result_t res;
    unsigned int Value;
    unsigned int Mask = 0xff00ffff; 

    res = custom_register_read_send( state, u32BaseCustomRegister + RTDEX_CTRL, &Value);
    if(FAILURE(res))
        return res;

    Value &= Mask;
    Value |= uEnable ? RX_START_BM_all : 0;

    res = custom_register_write_send( state, u32BaseCustomRegister + RTDEX_CTRL, Value);

    return res;
}

Result RTDExResetTest( connection_state *state, uint32_t u32BaseCustomRegister )
{
    adp_result_t res;

    res = RTDExEnableUplinkTest( state, u32BaseCustomRegister, 0);
    if(FAILURE(res))
       return res;

    res = RTDExEnableDownlinkTest( state, u32BaseCustomRegister, 0);
    if(FAILURE(res))
       return res;

    res = custom_register_write_send( state, u32BaseCustomRegister + RTDEX_CTRL, RX_RESET_BM_all | TX_RESET_BM_all);
    if(FAILURE(res))
		return res;

    // Not Reset, not start
    res = custom_register_write_send( state, u32BaseCustomRegister + RTDEX_CTRL, 0);
    if(FAILURE(res))
		return res;

    return RTDEXOK;
}

adp_result_t RTDExEnableAdcDacMode( connection_state *state, uint32_t u32BaseCustomRegister )
{
    adp_result_t res;
    unsigned uVal;

    res = custom_register_read_send( state, u32BaseCustomRegister + LOOPBACK_EN, &uVal);
	uVal |= DISABLE_FLOW_CTRL;
	return custom_register_write_send( state, u32BaseCustomRegister + LOOPBACK_EN, uVal);
}

adp_result_t RTDExDisableAdcDacMode( connection_state *state, uint32_t u32BaseCustomRegister )
{
	adp_result_t res;
    unsigned uVal;

	res = custom_register_read_send( state, u32BaseCustomRegister + LOOPBACK_EN, &uVal);
	uVal &= ENABLE_FLOW_CTRL;
	return custom_register_write_send( state, u32BaseCustomRegister + LOOPBACK_EN, uVal );
}

adp_result_t RTDExSetTestInitVal( connection_state *state, uint32_t u32BaseCustomRegister, unsigned int uValue )
{
    unsigned int uId;
    adp_result_t res;

    uId = u32BaseCustomRegister + TX_INITVAL;
    res = custom_register_write_send( state, uId, uValue );

    return res;
}

adp_result_t RTDExSetTestCntTxDiv( connection_state *state, uint32_t u32BaseCustomRegister, unsigned int uValue)
{
    return custom_register_write_send( state, u32BaseCustomRegister + TX_DIV_CNT, uValue );
}

adp_result_t RTDExSetTestCntRxDiv( connection_state *state, uint32_t u32BaseCustomRegister, unsigned int uValue)
{ 
	return custom_register_write_send( state, u32BaseCustomRegister + RX_EN_DIV_CNT, uValue );
}

adp_result_t RTDExGetUplinkOverFlow( connection_state *state, uint32_t u32BaseCustomRegister,
                              unsigned int uChannlNo, unsigned int* pCnt )
{
    uint32_t u32Address;

    if( uChannlNo >= 8 ) {
        return RTDEXINVCHANNELNUM;
    }

    u32Address = u32BaseCustomRegister + TX_OVERFLOW_CNT(uChannlNo);

    return custom_register_read_send( state, u32Address, pCnt);
}

adp_result_t RTDExGetDownlinkUnderFlow( connection_state *state, uint32_t u32BaseCustomRegister,
                              unsigned int uChannlNo, unsigned int* pCnt )
{
    uint32_t u32Address;

    if( uChannlNo >= 8 ) {
        return RTDEXINVCHANNELNUM;
    }
    u32Address = u32BaseCustomRegister + RX_UNDERFLOW_CNT(uChannlNo);

    return custom_register_read_send( state, u32Address, pCnt);
}

adp_result_t RTDExGetDownlinkErrorCnt( connection_state *state, uint32_t u32BaseCustomRegister,
                              unsigned int uChannlNo, unsigned int* pCnt )
{
    uint32_t u32Address;

    if( uChannlNo >= 8 ) {
        return RTDEXINVCHANNELNUM;
    }

    u32Address = u32BaseCustomRegister + ERR_CNT_n( uChannlNo);

    return custom_register_read_send( state, u32Address, pCnt);
}

adp_result_t RTDExGetDownlinkReceivedSamples( connection_state *state, uint32_t u32BaseCustomRegister,
                                             unsigned int uChannlNo, unsigned int *pCnt )
{
    uint32_t u32Address;

    if( uChannlNo >= 8 ) {
        return RTDEXINVCHANNELNUM;
    }

    u32Address = u32BaseCustomRegister + RX_RECEIVEDSAMPLES_CNT( uChannlNo);

    return custom_register_read_send( state, u32Address, pCnt);
}

adp_result_t ParseFileNames(char fileNames[], char seperatedFileNames[MAX_RTDEX][60], const char cFileNameDelim)
{
    char *pcToken;
    char acTempFileNames[500];
    uint32_t u32NumberOfFiles = -1;
    strcpy(acTempFileNames, fileNames);

    pcToken = strtok(acTempFileNames, &cFileNameDelim);
    if(pcToken == NULL)
    {
        printf("Error parsing file names.\n");
        return u32NumberOfFiles;
    }
    u32NumberOfFiles = 0;
    while (pcToken) 
    {
        if ( sscanf(pcToken, "%s", seperatedFileNames[u32NumberOfFiles] ) != 1 )
        {
            fprintf( stderr, "Error: Invalid channel index.\n" );
            return -1;
        }
        pcToken = strtok(0, &cFileNameDelim);
        u32NumberOfFiles++;           
    }
    return u32NumberOfFiles;
}