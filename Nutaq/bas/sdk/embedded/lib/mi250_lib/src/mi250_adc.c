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
 * @file       mi250_adc.c
 * @brief      MI250 configuration functions API for the ADC
 *
 * Copyright (C) 2014, Nutaq, Canada. All rights reserved.
 *
 * @ingroup MI250
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
#include "mi250_user.h"
#include "mi250.h"
#include "bas_debug.h"
#include "ADS62P49_def.h"

Result MI250_ADC_init(MI250Board_t * board)
{
	MI250Adaptator_t mi250ad;
	char regAdd[18] = {0x20,0x3F,0x40,0x41,0x44,0x50,0x51,0x52,0x53,
					 0x55,0x57,0x62,0x63,0x66,0x68,0x6A,0x75,0x76};
	char regVal[18] = {0x00,0x20,0x08,0x80,0x00,0x40,0xAA,0xAA,0x40,
					 0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00};

	char readVal;
	int i;
	unsigned int errors;

	//ADC reset
	mi250ad.ADC_Control.ADC_SPI_Reset = 1;
	MI250_WriteControlRegister( board, MI250_ADC_CONTROL, mi250ad.ADC_Control.Register );
	// Delay for reset
	board->pfUsleep(20);
	mi250ad.ADC_Control.ADC_SPI_Reset = 0;
	MI250_WriteControlRegister( board, MI250_ADC_CONTROL, mi250ad.ADC_Control.Register );

	// Set ADC registers
	for (i = 0; i < 18; i++)
	{
		MI250_WriteSPIRegisters(board,MI250_ADC_12,regAdd[i], 1, &regVal[i]);
		MI250_WriteSPIRegisters(board,MI250_ADC_34,regAdd[i], 1, &regVal[i]);
		MI250_WriteSPIRegisters(board,MI250_ADC_56,regAdd[i], 1, &regVal[i]);
		MI250_WriteSPIRegisters(board,MI250_ADC_78,regAdd[i], 1, &regVal[i]);
	}

	// Check ADC registers
	errors = 0;
	for (i = 0; i < 18; i++)
	{
		MI250_ReadSPIRegisters(board,MI250_ADC_12,regAdd[i], 1, &readVal);
		errors += (readVal != regVal[i]);
		MI250_ReadSPIRegisters(board,MI250_ADC_34,regAdd[i], 1, &readVal);
		errors += (readVal != regVal[i]);
		MI250_ReadSPIRegisters(board,MI250_ADC_56,regAdd[i], 1, &readVal);
		errors += (readVal != regVal[i]);
		MI250_ReadSPIRegisters(board,MI250_ADC_78,regAdd[i], 1, &readVal);
		errors += (readVal != regVal[i]);
	}
	if(errors)
	{
		debug("MI250 ADC setup errors: %d\n",errors);
		return(errors);
	}
    
    return OK;
}

Result MI250_ADC_arm(MI250Board_t * board, MI250_ADC_ARMED_STATUS armed)
{
	MI250Adaptator_t mi250ad;

    MI250_ReadControlRegister(board, MI250_ADC_CONTROL, &mi250ad.ADC_Control.Register);
	if(armed == MI250_ARMED)
	{
		mi250ad.ADC_Control.ChArmed = 1;
		MI250_WriteControlRegister( board, MI250_ADC_CONTROL, mi250ad.ADC_Control.Register);
	}
	else
	{
		mi250ad.ADC_Control.ChArmed = 0;
		MI250_WriteControlRegister( board, MI250_ADC_CONTROL, mi250ad.ADC_Control.Register);
	}
    
    return OK;
}


Result MI250_ADC_SetDataFormat(MI250Board_t * board, MI250_DEVICES DeviceId,  MI250_ADC_DATAFORMAT format)
{
    ADS62P49_Register_Map_t reg;
    
	 //enable individual channel control and set dataformat
    MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_50, 1, &reg.Register_50.Byte);
    reg.Register_50.Bit.Enable_Ind_Chan_Ctrl = 1;
    reg.Register_50.Bit.Data_Format = format;
    MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_50, 1, &reg.Register_50.Byte);
	
	return OK;
}

Result MI250_ADC_SetSleepMode(MI250Board_t * board, MI250_DEVICES DeviceId,  MI250_ADC_SLEEP_MODE mode)
{
    ADS62P49_Register_Map_t reg;

   //set sleep mode
    MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_40, 1, &reg.Register_40.Byte);
    reg.Register_40.Bit.PowerDown_Mode = mode;
    MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_40, 1, &reg.Register_40.Byte);
    return OK;
}

Result MI250_ADC_SetOffsetCorrectionTime(MI250Board_t * board,  MI250_DEVICES DeviceId, MI250_CHANNEL_OCT octa, MI250_CHANNEL_OCT octb)
{
    ADS62P49_Register_Map_t reg;

    //offset correction time channel A
    MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_55, 1, &reg.Register_55.Byte);
    reg.Register_55.Bit.ChanA_Offset_Time = octa;
    MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_55, 1, &reg.Register_55.Byte);

    //offset correction time channel B
    MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_68, 1, &reg.Register_68.Byte);
    reg.Register_68.Bit.ChanB_Offset_Time = octb;
    MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_68, 1, &reg.Register_68.Byte);
    return OK;
}



Result MI250_ADC_setGain(MI250Board_t * board, MI250_ADC_CHANNEL channel, MI250_ADC_GAIN gain, MI250_DEVICES DeviceId)
{
    ADS62P49_Register_Map_t reg;

    switch (channel)
    {
        default:
        case MI250_ADC_CHANNEL_1:
            MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_55, 1, &reg.Register_55.Byte);
            reg.Register_55.Bit.ChanA_Gain = gain;
            MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_55, 1, &reg.Register_55.Byte);
            break;
        case MI250_ADC_CHANNEL_2:
            MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_68, 1, &reg.Register_68.Byte);
            reg.Register_68.Bit.ChanB_Gain = gain;
            MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_68, 1, &reg.Register_68.Byte);
            break;
    }

    return OK;
}


Result MI250_ADC_setFineGain(MI250Board_t * board, MI250_ADC_CHANNEL channel, char gain, MI250_DEVICES DeviceId)
{
    ADS62P49_Register_Map_t reg;

    switch (channel)
    {
        default:
        case MI250_ADC_CHANNEL_1:
            MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_57, 1, &reg.Register_57.Byte);
            reg.Register_57.Bit.ChanA_Gain_Fine = gain;
            MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_57, 1, &reg.Register_57.Byte);
            break;
        case MI250_ADC_CHANNEL_2:
            MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_6A, 1, &reg.Register_6A.Byte);
            reg.Register_6A.Bit.ChanB_Gain_Fine = gain;
            MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_6A, 1, &reg.Register_6A.Byte);
            break;
    }

    return OK;
}


Result MI250_ADC_setPedestal(MI250Board_t * board, MI250_ADC_CHANNEL channel, char value, MI250_DEVICES DeviceId)
{
    ADS62P49_Register_Map_t reg;

    switch (channel)
    {
        default:
        case MI250_ADC_CHANNEL_1:
            MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_63, 1, &reg.Register_63.Byte);
            reg.Register_63.Bit.ChanA_Offset_Pedestal = value;
            MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_63, 1, &reg.Register_63.Byte);
            break;
        case MI250_ADC_CHANNEL_2:
            MI250_ReadSPIRegisters(board, DeviceId, ADS62P49_REGISTER_76, 1, &reg.Register_76.Byte);
            reg.Register_76.Bit.ChanB_Offset_Pedestal = value;
            MI250_WriteSPIRegisters(board, DeviceId, ADS62P49_REGISTER_76, 1, &reg.Register_76.Byte);
            break;
    }
    
    return OK;
}

#if 0
Result MI250_ADC_getStatus(MI250Board_t * board, MI250_ADC_CHANNEL channel, MI250_ADC_STATUSES * status, MI250_DEVICES DeviceId)
{
    MI250Adaptator_t reg_plb;
    int registerId;
    int registerValue;

    MI250_ReadControlRegister(board, MI250_ADC_CONTROL, &reg_plb.ADC_Control.Register);

  /*  switch(channel)
    {
        default:
        case ADC_CHANNEL_A:
            *status = reg_plb.ADC_Control.Register & ADC_CTRL_ADC_A_OVR_RANGE ? ADC_OVERRANGE : ADC_NORMAL_STATUS; 
            break;
        case ADC_CHANNEL_B:
            *status = reg_plb.ADC_Control.Register & ADC_CTRL_ADC_B_OVR_RANGE ? ADC_OVERRANGE : ADC_NORMAL_STATUS; 
            break;
    }*/

    #if 1
    switch(DeviceId) {
        case MI250_ADC_12: 
            registerId = MI250_ADC_12_CONTROL;
            break;
        case MI250_ADC_34: 
            registerId = MI250_ADC_34_CONTROL;
            break;
        case MI250_ADC_56: 
            registerId = MI250_ADC_56_CONTROL;
            break;
        case MI250_ADC_78: 
            registerId = MI250_ADC_78_CONTROL;
            break;
        default : 
            return(-1);
            break;
    }
    MI250_ReadControlRegister(board, registerId, &registerValue);
    switch(channel)
    {
        default:
        case MI250_ADC_CHANNEL_1:
            *status = registerValue & MI250_ADC_CTRL_ADC_1_OVR_RANGE ? MI250_ADC_OVERRANGE : MI250_ADC_NORMAL_STATUS; 
            break;
        case MI250_ADC_CHANNEL_2:
            *status = registerValue & MI250_ADC_CTRL_ADC_2_OVR_RANGE ? MI250_ADC_OVERRANGE : MI250_ADC_NORMAL_STATUS; 
            break;
    }
    #endif
    
    return OK;
}
#endif

