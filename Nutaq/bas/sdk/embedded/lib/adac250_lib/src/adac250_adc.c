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
 *  @file       : adac250_adc.c
 *  @brief      : ADAC250 configuration functions API for the ADC
 * 
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup ADAC250
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
#include "adac250_user.h"
#include "adac250.h"
#include "ADS62P49_def.h"

// Point the the pfADAC250_usleep function pointer define in the adac250.c
extern int (*pfADAC250_usleep)(unsigned int usec);

Result ADAC250_ADC_init(ADAC250Board_t * board)
{
    char regAdd[18] = {0x20,0x3F,0x40,0x41,0x44,0x50,0x51,0x52,0x53,
                    0x55,0x57,0x62,0x63,0x66,0x68,0x6A,0x75,0x76};
    char regVal[18] = {0x00,0x20,0x08,0x80,0x00,0x44,0xAA,0xAA,0x40,
                    0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00};
    int i;

    ADAC250_WriteControlRegister( board, ADAC250_ADC_CONTROL, ADC_CTRL_SPI_RESET );
    pfADAC250_usleep(1);
    ADAC250_WriteControlRegister( board, ADAC250_ADC_CONTROL, 0x00 );

    ADAC250_WriteControlRegister( board, ADAC250_ADC_CONTROL,
                                  ADC_CTRL_ADC_A_RUN | ADC_CTRL_ADC_B_RUN | ADC_CTRL_ADC_A_START |
                                  ADC_CTRL_ADC_B_START | ADC_CTRL_ADC_DATA_TYPE );

    for (i=0;i<18;i++)
    {
        ADAC250_WriteSPIRegisters(board, ADAC250_ADC, regAdd[i], 1, &regVal[i]);
    }

    pfADAC250_usleep(2);

    return OK;
}


Result ADAC250_ADC_SetDataFormat(ADAC250Board_t * board, ADAC250_ADC_DATAFORMAT format)
{
    ADS62P49_Register_Map_t reg;
    ADAC250Adaptator_t fpgaregs;

    //enable individual channel control and set dataformat
    ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_50, 1, &reg.Register_50.Byte);
    reg.Register_50.Bit.Enable_Ind_Chan_Ctrl = 1;
    reg.Register_50.Bit.Data_Format = format;
    ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_50, 1, &reg.Register_50.Byte);

    ADAC250_ReadControlRegister(board, ADAC250_ADC_CONTROL, &fpgaregs.ADC_Control.Register);
    switch (format)
    {
    case ADAC250_ADC_OFFSET:
        fpgaregs.ADC_Control.ADC_DataType = 0;
        break;
    default:
    case ADAC250_ADC_TWOCMP:
        fpgaregs.ADC_Control.ADC_DataType = 1;
        break;
    }
    ADAC250_WriteControlRegister(board, ADAC250_ADC_CONTROL, fpgaregs.ADC_Control.Register);
	return OK;
}


Result ADAC250_ADC_SetOffsetCorrectionTime(ADAC250Board_t * board, ADAC250_CHANNEL_OCT octa, ADAC250_CHANNEL_OCT octb)
{
    ADS62P49_Register_Map_t reg;

    //offset correction time channel A
    ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_55, 1, &reg.Register_55.Byte);
    reg.Register_55.Bit.ChanA_Offset_Time = octa;
    ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_55, 1, &reg.Register_55.Byte);

    //offset correction time channel B
    ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_68, 1, &reg.Register_68.Byte);
    reg.Register_68.Bit.ChanB_Offset_Time = octb;
    ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_68, 1, &reg.Register_68.Byte);
    return OK;
}


Result ADAC250_ADC_SetSleepMode(ADAC250Board_t * board,  ADAC250_ADC_SLEEP_MODE mode)
{
    ADS62P49_Register_Map_t reg;

    //set sleep mode
    ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_40, 1, &reg.Register_40.Byte);
    reg.Register_40.Bit.PowerDown_Mode = mode;
    ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_40, 1, &reg.Register_40.Byte);
    return OK;
}


Result ADAC250_ADC_setGain(ADAC250Board_t * board, ADAC250_ADC_CHANNEL channel, ADAC250_ADC_GAIN gain)
{
    ADS62P49_Register_Map_t reg;
    switch (channel)
    {
    default:
    case ADAC250_ADC_CHANNEL_A:
        ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_55, 1, &reg.Register_55.Byte);
        reg.Register_55.Bit.ChanA_Gain = gain;
        ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_55, 1, &reg.Register_55.Byte);
        break;
    case ADAC250_ADC_CHANNEL_B:
        ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_68, 1, &reg.Register_68.Byte);
        reg.Register_68.Bit.ChanB_Gain = gain;
        ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_68, 1, &reg.Register_68.Byte);
        break;
    }
    return OK;
}


Result ADAC250_ADC_setFineGain(ADAC250Board_t * board, ADAC250_ADC_CHANNEL channel, char gain)
{
    ADS62P49_Register_Map_t reg;
    switch (channel)
    {
    default:
    case ADAC250_ADC_CHANNEL_A:
        ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_57, 1, &reg.Register_57.Byte);
        reg.Register_57.Bit.ChanA_Gain_Fine = gain;
        ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_57, 1, &reg.Register_57.Byte);
        break;
    case ADAC250_ADC_CHANNEL_B:
        ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_6A, 1, &reg.Register_6A.Byte);
        reg.Register_6A.Bit.ChanB_Gain_Fine = gain;
        ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_6A, 1, &reg.Register_6A.Byte);
        break;
    }
    return OK;
}


Result ADAC250_ADC_setPedestal(ADAC250Board_t * board, ADAC250_ADC_CHANNEL channel, char value)
{
    ADS62P49_Register_Map_t reg;
    switch (channel)
    {
    default:
    case ADAC250_ADC_CHANNEL_A:
        ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_63, 1, &reg.Register_63.Byte);
        reg.Register_63.Byte = value & 0x3F;
        ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_63, 1, &reg.Register_63.Byte);
        break;
    case ADAC250_ADC_CHANNEL_B:
        ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_76, 1, &reg.Register_76.Byte);
        reg.Register_76.Bit.ChanB_Offset_Pedestal = value;
        ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_76, 1, &reg.Register_76.Byte);
        break;
    }
    return OK;
}


Result ADAC250_ADC_getStatus(ADAC250Board_t * board, ADAC250_ADC_CHANNEL channel, ADAC250_ADC_STATUSES * status)
{
    ADAC250Adaptator_t fpgaregs;
    ADAC250_ReadControlRegister(board, ADAC250_ADC_CONTROL, &fpgaregs.ADC_Control.Register);
    switch(channel)
    {
    default:
    case ADAC250_ADC_CHANNEL_A:
        *status = fpgaregs.ADC_Control.Register & ADC_CTRL_ADC_A_OVR_RANGE ? ADAC250_ADC_OVERRANGE : ADAC250_ADC_NORMAL_STATUS; 
        break;
    case ADAC250_ADC_CHANNEL_B:
        *status = fpgaregs.ADC_Control.Register & ADC_CTRL_ADC_B_OVR_RANGE ? ADAC250_ADC_OVERRANGE : ADAC250_ADC_NORMAL_STATUS; 
        break;
    }
    return OK;
}

Result ADAC250_ADC_Calibrate(ADAC250Board_t * board)
{
  ADS62P49_Register_Map_t reg;
  
  int i;
  char validDelay[63] = {0};
  
  unsigned char delayGroupCnt = 0;
  unsigned char delayGroupMiddle = 0;
  unsigned char delayGroupMax = 0;
    
  // Set ADCs in test mode
  ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_62, 1, &reg.Register_62.Byte);
  reg.Register_62.Bit.ChanA_Test_Pattern = 3;
  ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_62, 1, &reg.Register_62.Byte);
  
  ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_75, 1, &reg.Register_75.Byte);
  reg.Register_75.Bit.ChanB_Test_Pattern = 3;
  ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_75, 1, &reg.Register_75.Byte);
  
  ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_44, 1, &reg.Register_44.Byte);
  reg.Register_44.Bit.Clock_Edge_Control = 0x00;
  ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_44, 1, &reg.Register_44.Byte);

  // Check validity of all delay values
  for(i=0; i<63; i++)
  {
    if(i<31)
    {
      board->Adaptor->AdcDelay_Control.AdcClkIdelayValue = 31-i;
      board->Adaptor->AdcDelay_Control.AdcIdelayValue = 0;
    }
    else
    {
      board->Adaptor->AdcDelay_Control.AdcClkIdelayValue = 0;
      board->Adaptor->AdcDelay_Control.AdcIdelayValue = i-31;
    }

    pfADAC250_usleep(10);
    validDelay[i] = board->Adaptor->AdcDelay_Control.AdcPatternError;
  }

  // Find the longest valid delay group and its middle value
  for(i=0; i<63; i++)
  {
    if(validDelay[i]==0)
    {
      delayGroupCnt++;
      if(delayGroupCnt>delayGroupMax)
      {
        delayGroupMax = delayGroupCnt;
        delayGroupMiddle = i-delayGroupCnt/2;
      }
    }
    else
    {
      delayGroupCnt = 0;
    }
  }

  // Set the delay value
  if(delayGroupMiddle<31)
  {
    board->Adaptor->AdcDelay_Control.AdcClkIdelayValue = 31-delayGroupMiddle;
    board->Adaptor->AdcDelay_Control.AdcIdelayValue = 0;
  }
  else
  {
    board->Adaptor->AdcDelay_Control.AdcClkIdelayValue = 0;
    board->Adaptor->AdcDelay_Control.AdcIdelayValue = delayGroupMiddle-31;
  }

  // Set ADCs in normal mode
  ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_62, 1, &reg.Register_62.Byte);
  reg.Register_62.Bit.ChanA_Test_Pattern = 0;
  ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_62, 1, &reg.Register_62.Byte);
  
  ADAC250_ReadSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_75, 1, &reg.Register_75.Byte);
  reg.Register_75.Bit.ChanB_Test_Pattern = 0;
  ADAC250_WriteSPIRegisters(board, ADAC250_ADC, ADS62P49_REGISTER_75, 1, &reg.Register_75.Byte);
  
  if(delayGroupMax==0)
  {
    return -1;
  }
  else
  {
    return OK;
  }
}

Result ADAC250_ADC_SetAdjTriggerDelay(ADAC250Board_t * board, char delay)
{

    ADAC250Adaptator_t fpgaregs;
	if(delay > 32)
	{
		return ADAC250_INVALID_TRIGGER_DELAY;
	}
	if(delay < 1)
	{
		return ADAC250_INVALID_TRIGGER_DELAY;
	}
	
    ADAC250_ReadControlRegister(board, ADAC250_ADC_CONTROL, &fpgaregs.ADC_Control.Register);
    fpgaregs.ADC_Control.TriggerDelay= delay - 1;
    ADAC250_WriteControlRegister(board, ADAC250_ADC_CONTROL, fpgaregs.ADC_Control.Register);
	return OK;
}


