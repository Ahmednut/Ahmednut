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
 *  @file       : adac250_dac.c
 *  @brief      : ADAC250 configuration functions API for the DAC
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
#include "DAC5682_def.h"
#include <stdio.h>

// Point the the pfADAC250_usleep function pointer define in the adac250.c
extern int (*pfADAC250_usleep)(unsigned int usec);

Result ADAC250_DAC_Init(ADAC250Board_t * board, ADAC250_DAC_SLEEP sleep, ADAC250_DAC_INTERPOLATION interpolation, ADAC250_DAC_CMIX_MODE mixmode0, ADAC250_DAC_CMIX_MODE mixmode1)
{
    //default value
    char regAdd[] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x0A,0x08,0x08,0x03};
    char regVal[] = {0x00,0xC0,0x71,0x00,0x86,0x03,0xFF,0xC8,0x04,0x00,0x01};
    int i;
    unsigned temp;
    //DAC5682_Register_Map_t reg;

    //reset
    ADAC250_ReadControlRegister( board, ADAC250_DAC_CONTROL, &temp );
    temp |= 1;
    ADAC250_WriteControlRegister( board, ADAC250_DAC_CONTROL, temp );
    temp &= ~1;
    pfADAC250_usleep(1);
    ADAC250_WriteControlRegister( board, ADAC250_DAC_CONTROL, temp );
    pfADAC250_usleep(10);
    
    //poll for DAC ready
    /*timeout = 100000;
    do
    {
        ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_STATUS0, 1, &reg.Status0.Byte);
        timeout--;
    } while ( ((reg.Status0.Bit.version) != 0x3) && (timeout !=0) );*/

    /*ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_STATUS0, 1, &value);

    printf("status0: %x\n", value);*/

    /*if (!timeout)
    {
        return ADAC250DACTIMEOUT;
    }*/
    
    //power down
    regVal[5] |= sleep << 4;
    
    //interpolation
    switch(interpolation)
    {
    default:
    case ADAC250_DAC_INTERPOLATION_NONE:
        break;
    case ADAC250_DAC_INTERPOLATION_2X:
        //0x01
        regVal[0] |= 0x10;
        //0x02
        regVal[1] &= ~0x20;
        break;
    case ADAC250_DAC_INTERPOLATION_4X:
        //0x01
        regVal[0] |= 0x10;
        //0x02
        regVal[1] |= 0x20;
        break;
    }

    switch(mixmode0)
    {
    default:
    case ADAC250_DAC_CMIX_LP:
    case ADAC250_DAC_CMIX_HP:
    case ADAC250_DAC_CMIX_PosFDac:
    case ADAC250_DAC_CMIX_NegFDac:
        //0x02
        regVal[1] &= ~0x03;
        regVal[1] |= mixmode0 & 0x03;
        break;
    }

    switch(mixmode1)
    {
    default:
    case ADAC250_DAC_CMIX_LP:
    case ADAC250_DAC_CMIX_HP:
    case ADAC250_DAC_CMIX_PosFDac:
    case ADAC250_DAC_CMIX_NegFDac:
        //0x02
        regVal[1] &= ~0x0C;
        regVal[1] |= (mixmode1 << 2) & 0x0C;
        break;
    }
    for (i=0;i<sizeof(regAdd);i++)
    { 
        ADAC250_WriteSPIRegisters(board, ADAC250_DAC, regAdd[i], 1, &regVal[i]);
    }
    return OK;
}

Result ADAC250_DAC_SetGain(ADAC250Board_t * board, ADAC250_DAC_CHANNEL channel, int gain)
{
    DAC5682_Register_Map_t reg;
    gain &= 0xF;
    ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG7, 1, &reg.Config7.Byte);
    switch(channel)
    {
    default:
    case ADAC250_DAC_CHANNEL_A:
        reg.Config7.Bit.DACA_Gain = gain;
        break;
    case ADAC250_DAC_CHANNEL_B:
        reg.Config7.Bit.DACB_Gain = gain;
        break;
    }
    return ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG7, 1, &reg.Config7.Byte);
}


Result ADAC250_DAC_SetOffset(ADAC250Board_t * board, ADAC250_DAC_CHANNEL channel, int offset)
{
    DAC5682_Register_Map_t reg;
    if(offset)
    {
        offset &= 0x1FFF;
        //enable offset
        ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG3, 1, &reg.Config3.Byte);
        reg.Config3.Bit.DAC_Offset_Enable = 1;
        ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG3, 1, &reg.Config3.Byte);

        // clear offset sync
        ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG12, 1, &reg.Config12.Byte);
        reg.Config12.Bit.Offset_Sync=0;
        ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG12, 1, &reg.Config12.Byte);

        switch(channel)
        {
        default:
        case ADAC250_DAC_CHANNEL_A:
            reg.Config12.Bit.OffsetA_MSB = offset >> 8;
            reg.Config13.Bit.OffsetA_LSB = offset & 0xFF;
            reg.Config12.Bit.Offset_Sync = 1;
            ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG12, 2, &reg.Config12.Byte);
            break;
        case ADAC250_DAC_CHANNEL_B:
            ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG14, 1, &reg.Config14.Byte);
            reg.Config14.Bit.OffsetB_MSB = offset >> 8;
            reg.Config15.Bit.OffsetB_LSB = offset & 0xFF;
            ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG14, 2, &reg.Config14.Byte);
            reg.Config12.Bit.Offset_Sync = 1;
            ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG12, 1, &reg.Config12.Byte);
            break;
        }
    }else{
        //disable offset
        ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG3, 1, &reg.Config3.Byte);
        reg.Config3.Bit.DAC_Offset_Enable = 0;
        ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG3, 1, &reg.Config3.Byte);
    }
    return OK;
}

Result ADAC250_DAC_Sync(ADAC250Board_t * board, ADAC250_DAC_SYNC sync)
{
    DAC5682_Register_Map_t reg;
    ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG3, 1, &reg.Config3.Byte);
    reg.Config3.Bit.SW_Sync = sync & 1;
    ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG3, 1, &reg.Config3.Byte);
    return OK;
}

Result ADAC250_DAC_SyncSource(ADAC250Board_t * board, ADAC250_DAC_SYNC_SOURCE syncsource)
{
    DAC5682_Register_Map_t reg;
    ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG3, 1, &reg.Config3.Byte);
    reg.Config3.Bit.SW_Sync_Select = syncsource & 1;
    ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG3, 1, &reg.Config3.Byte);
    return OK;
}

Result ADAC250_DAC_Calibrate(ADAC250Board_t * board)
{
  DAC5682_Register_Map_t reg;
  unsigned char dacMuxSelA, dacMuxSelB;
  
  int i;
  char validDelay[63] = {0};
  char Config6Byte;
  
  unsigned char delayGroupCnt = 0;
  unsigned char delayGroupMiddle = 0;
  unsigned char delayGroupMax = 0;
  
  // Put DAC outputs to sleep
  ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG6, 1, &reg.Config6.Byte);
  Config6Byte = reg.Config6.Byte;
  reg.Config6.Bit.Sleep_A = 1;
  reg.Config6.Bit.Sleep_B = 1;
  ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG6, 1, &reg.Config6.Byte);
    
  // Set DACs in test mode
  dacMuxSelA = board->Adaptor->DAC_Control.DAC_MuxSelectA;
  dacMuxSelB = board->Adaptor->DAC_Control.DAC_MuxSelectB;
  board->Adaptor->DAC_Control.DAC_MuxSelectA = 4;
  board->Adaptor->DAC_Control.DAC_MuxSelectB = 4;

  // Check validity of all delay values
  for(i=0; i<63; i++)
  {
    if(i<31)
    {
      board->Adaptor->DacDelay_Control.DacClkIdelayValue = 31-i;
      board->Adaptor->DacDelay_Control.DacIdelayValue = 0;
    }
    else
    {
      board->Adaptor->DacDelay_Control.DacClkIdelayValue = 0;
      board->Adaptor->DacDelay_Control.DacIdelayValue = i-31;
    }
    
    reg.Status4.Byte = 0;
    ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_STATUS4, 1, &reg.Status4.Byte);

    pfADAC250_usleep(10);
    ADAC250_ReadSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_STATUS4, 1, &reg.Status4.Byte);
    validDelay[i] = reg.Status4.Bit.Pattern_Error;
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
    board->Adaptor->DacDelay_Control.DacClkIdelayValue = 31-delayGroupMiddle;
    board->Adaptor->DacDelay_Control.DacIdelayValue = 0;
  }
  else
  {
    board->Adaptor->DacDelay_Control.DacClkIdelayValue = 0;
    board->Adaptor->DacDelay_Control.DacIdelayValue = delayGroupMiddle-31;
  }

  // Set DACs in normal mode
  board->Adaptor->DAC_Control.DAC_MuxSelectA = dacMuxSelA;
  board->Adaptor->DAC_Control.DAC_MuxSelectB = dacMuxSelB;
  
  // Enable DAC outputs
  reg.Config6.Byte = Config6Byte;
  ADAC250_WriteSPIRegisters(board, ADAC250_DAC, DAC5682_REGISTER_CONFIG6, 1, &reg.Config6.Byte);
  
  if(delayGroupMax==0)
  {
    return -1;
  }
  else
  {
    return OK;
  }
}

