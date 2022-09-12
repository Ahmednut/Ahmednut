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
 * @file  wdxg.c    
 *
 * @brief  Function definition for wideband digitizer    
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>



#include "wdxg.h"
#include "wsa_api.h"



wdxg_handle* WDxG_Open(wdxg_handle* hWDxG)
{

   hWDxG = (wdxg_handle*)malloc(sizeof(wdxg_handle)); // Allocation of Wideband Digitizer object
   hWDxG->sClocking = (wdxg_clocking*)malloc(sizeof(wdxg_clocking)); // Allocation of Wideband Digitizer
   hWDxG->sSweep = (wdxg_sweep*)malloc(sizeof(wdxg_sweep));
   hWDxG->sGain  = (wdxg_gain*)malloc(sizeof(wdxg_gain));
   hWDxG->state  = (connection_state*)malloc(sizeof(connection_state)); 		
   hWDxG->dev  = ( wsa_device_t)malloc(sizeof( struct wsa_device));
   return  hWDxG;
}

Result WDxG_Init (wdxg_handle* hWDxG, wdxg_clocking *sClocking)
{
   Result res;
 
   ADAC250_CLOCK_SOURCES eAdac250ClkSrc;   
   res =ADAC250_Presence_send (hWDxG->state); //Verifies the ADAC250 FMC card and FPGA core presence
   if(FAILURE(res))
   {
      return res;
   }
   res =ADAC250_PowerUp_send (hWDxG->state); //Powers up the FMC site for ADAC250 operation
   if(FAILURE(res))
   {
      return res;
   }
   switch(hWDxG->sClocking->eClkSrc)  //Setup the chosen clock 
   {
      case WDXG_CLKSRC_ALLINTERNAL: eAdac250ClkSrc = ADAC250_CLOCKIN_10MHZREF;
	  break;
      case WDXG_CLKSRC_WSA5000: eAdac250ClkSrc = ADAC250_CLOCKIN_EXTERNALREF;
	  break;
   }
   res = ADAC250_PLL_Init_send(hWDxG->state, hWDxG->sClocking->u32SrcClkFreq,hWDxG->sClocking->u32SamplingClkFreq,hWDxG->sClocking->u32SamplingClkFreq,ADAC250_DAC_INTERPOLATION_NONE,eAdac250ClkSrc); //  Resets and initializes the ADAC250 PLL with chosen sampling and ref clock
   if(FAILURE(res))
   {
      return res;
   }
   res = ADAC250_MUX_ConfigClockOutput_send( hWDxG->state, ADAC250_CLOCKOUT_REFOUT, ADAC250_CLOCKIN_PLLCLKOUT2); //Sets the ADAC250 clock multiplexer
   if(FAILURE(res))
   {
      return res;
   }
   res = ADAC250_ADC_init_send (hWDxG->state); //Initializes the A/D converter at the default ADAC250 values
   if(FAILURE(res))
   {
      return res;
   }
   res = ADAC250_ADC_Calibrate_send (hWDxG->state); //Calibrates the ADC lvds data bus timings
   if(FAILURE(res))
   {
      return res;
   }
   res = wsa_set_iq_output_mode(hWDxG->dev, WSA_IQ_CONNECTOR_STRING); //Activate I and Q connector on the WSA5000
   if(FAILURE(res))
   {
      return res;
   }
   return  0;
}

Result WDxG_SetReceiverFrequency (wdxg_handle* hWDxG, unsigned long long u64Frequency)
{
   Result res;
   res = wsa_set_freq(hWDxG->dev, u64Frequency); //Sets the WSA to the desired center frequency
   if(FAILURE(res))
   {
      return res;
   }
   return 0;
}

Result WDxG_SetSweep (wdxg_handle* hWDxG, wdxg_sweep* sSweep )
{
   Result res;
   char buffer1[100];
   char buffer2[100];
  
   res = wsa_send_scpi(hWDxG->dev,"SYSTEM:SYNC:MASTER OFF");
   if(FAILURE(res))
   {
      return res;
   }

   sprintf(buffer1,"%llu",hWDxG->sSweep->u32Sweep_DwellTime);
   snprintf(buffer2,100,"SYSTEM:SYNC:WAIT %s",buffer1);


   res = wsa_send_scpi(hWDxG->dev,buffer2);
   if(FAILURE(res))
   {
      return res;
   }

   res = wsa_set_sweep_iteration(hWDxG->dev,0);//Set the sweep center frequency   
   if(FAILURE(res))
   {
      return res;
   }

   res = wsa_sweep_entry_new(hWDxG->dev);//Set the sweep center frequency   
   if(FAILURE(res))
    {
      return res;
    } 

   res = wsa_set_sweep_dwell(hWDxG->dev, 1, 0);
   if(FAILURE(res))
    {
      return res;
    }

   res = wsa_set_sweep_trigger_type(hWDxG->dev,WSA_PULSE_TRIGGER_TYPE);
   if(FAILURE(res))
   {
      return res;
   }

   res = wsa_set_sweep_attenuation(hWDxG->dev,0);//Set the sweep center frequency   
   if(FAILURE(res))
   {
      return res;
   }
   res = wsa_set_sweep_rfe_input_mode(hWDxG->dev,"ZIF");//Set the sweep center frequency   
   if(FAILURE(res))
   {
      return res;
   }
   res = wsa_set_sweep_freq(hWDxG->dev, sSweep->u64Sweep_StartFreq,sSweep->u64Sweep_StopFreq);//Set the sweep center frequency   
   if(FAILURE(res))
   {
      return res;
   }
   res = wsa_set_sweep_freq_step(hWDxG->dev, sSweep->u32Sweep_Step); //Set the sweep frequency step size
   if(FAILURE(res))
   {
      return res;
   } 

   res = wsa_sweep_entry_save(hWDxG->dev,0);//Set the sweep center frequency   
   if(FAILURE(res))
   {
      return res;
   }
   return  0;
}

Result WDxG_StartSweep (wdxg_handle* hWDxG) 
{
   Result res;
   res = wsa_sweep_start(hWDxG->dev); //start sweep mode
   if(FAILURE(res))
   {
	 return res;
   }
   return  0;
}

Result WDxG_StopSweep(wdxg_handle* hWDxG)
{
   Result res;
   res = wsa_sweep_stop(hWDxG->dev);//stop sweep mode
   if(FAILURE(res))
   {
      return res;
   }

   return  0;
}
Result WDxG_SweepStatus (wdxg_handle* hWDxG, char* pu8Status)
{
  Result res;
  res = wsa_get_sweep_status(hWDxG->dev,pu8Status);
  if(FAILURE(res))
   {
	 return res;
   }
  return  0;
}

Result WDxG_SetGain(wdxg_handle* hWDxG, wdxg_gain* sGain)
{
   Result res;
   char buffer1[100];
   char buffer4[100];
   
   
if(strcmp("408",hWDxG->dev->descr.dev_model) == 0)
 { 
   
   res = wsa_set_attenuation(hWDxG->dev, sGain->u32AttenuationMode);//Sets the attenuator's mode of operation (OFF/ON)
   if(FAILURE(res))
   {
      return res;
   }	 
 }
 else
 {       
    sprintf(buffer4,"%u",sGain->u32AttenuationVar);
	snprintf(buffer1,100,":INP:ATT:VAR %s",buffer4);
	res=wsa_send_scpi(hWDxG->dev, buffer1);
	if(FAILURE(res))
      {
         return res;
      }
 }
 
   res = ADAC250_ADC_setGain_send(hWDxG->state, ADAC250_ADC_CHANNEL_A,sGain->u32DigitalGain);//Sets the ADC_A gain
   if(FAILURE(res))
   {
      return res;
   }
   res = ADAC250_ADC_setGain_send(hWDxG->state, ADAC250_ADC_CHANNEL_B,sGain->u32DigitalGain);//Sets the ADC_B gain
   if(FAILURE(res))
   {
      return res;
   }
  
  
   
   return  0;
}

Result WDxG_Close(wdxg_handle* hWDxG)
{
	Result res;
   
   	res = disconnect_cce(hWDxG->state);
	if(FAILURE(res))
	{
		
		return res;
	}
	
	 wsa_close(hWDxG->dev);
	
   return  0;
}

Result WDxG_Connect(wdxg_handle* hWDxG , connection_state state, const char * IpAddress, const char *  WSA_IpAddress)
{
   Result res;

//Concat string to send at the WSA to connect
   char buffer[100];
   char prefix[8]={"TCPIP::"};
   char suffix[13]={":37001,37000"};
   int iplenght=0;
   int i=0;
   char status[100];


   while(WSA_IpAddress[i] != '\0')
   {
      iplenght = iplenght + 1;
      i = i +1;
   }

   for(i=0;i<(sizeof(prefix)/sizeof(prefix[0]));i++)
   {
     buffer[i] = prefix[i];
   }

  
  for(i=0;i<iplenght;i++)
   {
     buffer[i + 7] = WSA_IpAddress[i];
   }

  for(i=0;i<(sizeof(suffix)/sizeof(suffix[0]));i++)
   {
     buffer[i + iplenght + 7 ] = suffix[i];
   }
//************************************************

   res = connect_cce(IpAddress, hWDxG->state);
   if(FAILURE(res))
   {
      return res;
   }
   res=wsa_open(hWDxG->dev,buffer);// connection to WSA5000
   if(FAILURE(res))
   {
     
      return res;
   }
   res = WDxG_SweepStatus (hWDxG,status);
   
   if(FAILURE(res))
   {
      return res;
   }
   
   if(strcmp(status,"RUNNING")==0)
   {
      res = wsa_send_scpi(hWDxG->dev,":SWEEP:ENTR:DELETE ALL");

   }
   res = wsa_send_scpi(hWDxG->dev,"*RST");
   return  0;
}




