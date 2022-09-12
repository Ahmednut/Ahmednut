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
 *  @file       : adac250_pll.c
 *  @brief      : ADAC250 configuration functions API for the PLL
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
#include "adac250.h"
#include "adac250_user.h"
#include "adac250_pll.h"
#include "AD9511_def.h"
#include "bas_debug.h"
#include <string.h> //memset
#include <stdio.h>

// Point the the pfADAC250_usleep function pointer define in the adac250.c
extern int (*pfADAC250_usleep)(unsigned int usec);

// http://en.wikipedia.org/wiki/Greatest_common_divisor
static int gcd ( unsigned a, unsigned b )
{
  unsigned c;
  while ( a != 0 )
  {
     c = a;
     a = b%a;
     b = c;
  }
  return b;
}


// http://en.wikipedia.org/wiki/Least_common_multiple
static unsigned lcm(unsigned a, unsigned b)
{
    return (b / gcd(a, b) ) * a;
}


static Result ADAC250_CalculateVCOfrequency(unsigned AdcClk, unsigned DacClk, int interpolation, unsigned VCOFreq[10], int * VCOFreq_size)
{
    int i,k;
    unsigned multiple, vco;
    int divider_adc, divider_dac;
    vco = multiple = lcm(AdcClk, DacClk);

    if(multiple % AdcClk != 0 || multiple % DacClk != 0)
    {
        return ADAC250_OVERFLOW;
    }

    k=0;
    i=2;
    while(vco<=ADAC250_MAXVCOFREQ)
    {
        if(vco>=ADAC250_MINVCOFREQ)
        {
            divider_adc = (int) vco / AdcClk;
            divider_dac = (int) vco / DacClk;
            //Clock duty needs to be 50%
            if( (divider_adc == 1 || divider_adc % 2 == 0) && (divider_dac == 1 || divider_dac % 2 == 0) )
            {
                VCOFreq[k] = (int) vco;
                k++;
            }
            if(k>=*VCOFreq_size)
                break;
        }
        vco = multiple * i;
        i++;
    }
    *VCOFreq_size = k;
    return k>0 ? OK : ADAC250_NO_VALID_DIVIDER;
}


static Result ADAC250_GetFactors( unsigned int refIn, unsigned int vco, ADAC250_PLL_PARAM * param)
{
    unsigned psclr[] = {2,4,8,16,32};
    unsigned P, R, B, A;
    unsigned N, i, divi;
    unsigned pfdFreq, refTemp;
    
    divi = 0;
    while(1)
    {
        divi++;
        refTemp = refIn / divi;
        pfdFreq = gcd(refTemp, vco);
        if ( (refIn % divi == 0) && (pfdFreq > ADAC250_PFD_MIN_FREQ) && (pfdFreq < ADAC250_PFD_MAX_FREQ)) break;
        if ( refTemp < ADAC250_PFD_MIN_FREQ )
        {
            return ADAC250_PFDFREQ_OUT_OF_RANGE;
        }
    }

    R = refIn / pfdFreq;
    if (R<1)
    {
        return ADAC250_NO_VALID_R;
    }

    if (R>16383)
    {
        return ADAC250_REF_CLOCK_TO_HIGH;
    }

    if( vco < 600e6) //P = 2 DM CLK2: <600 MHz
    {
        i=0;
    } else if (vco < 1e9) { //P = 4 DM CLK2: <1000 MHz
        i=1;
    } else if (vco <= ADAC250_MAXVCOFREQ) { 
        i=2;
    }else {
        return ADAC250_VCO_OUT_OF_RANGE;
    }
    /* refIn / R = pfdFreq = vco / ( P*B + A) */ 
    N = vco / pfdFreq;
    for(; i<(sizeof(psclr)/sizeof(int)); i++ )
    {
        P = psclr[i];
        B = N/P;
        if ( (B >= 3) && (B <= 8191) ) //13 bit
        {
            A = N % P;
            if( !( A & ~0x3F ) ) //6 bit
            {
                param->Bfactor = B;
                param->Rfactor = R;
                param->Afactor = A;
                param->Pfactor = P;
                param->pfdFreq = pfdFreq;
                param->VCOFreq = vco;
                return OK;
            }
        }
    }

    return ADAC250_NO_VALID_PLL_SOLUTION;
}


Result ADAC250_PLL_CalculateParams(unsigned refClk, unsigned AdcClk, unsigned DacDataClk, ADAC250_DAC_INTERPOLATION interpolation, ADAC250_PLL_PARAM * param)
{
    Result ret;
    unsigned DacClk;
    int interp, i;
    unsigned VCOFreq[10];
    int VCOFreq_size = sizeof(VCOFreq)/sizeof(unsigned);

    switch (interpolation)
    {
    case ADAC250_DAC_INTERPOLATION_2X:
        interp=2;
        break;
    case ADAC250_DAC_INTERPOLATION_4X:
        interp=4;
        break;
    default:
    case ADAC250_DAC_INTERPOLATION_NONE:
        interp=1;
        break;
    }
    DacClk = DacDataClk * interp;
    
    if(AdcClk < ADAC250_GENERATED_FREQ_MIN || DacClk < ADAC250_GENERATED_FREQ_MIN)
    {
        return ADAC250_FREQLOW;
    }

    ret = ADAC250_CalculateVCOfrequency(AdcClk, DacClk, interp, VCOFreq, &VCOFreq_size);
    if(FAILURE(ret)) return ret;

    for (i=0; i<VCOFreq_size; i++)
    {
        ret = ADAC250_GetFactors(refClk, VCOFreq[i], param);
        if(SUCCESS(ret))
        {
            param->divisorADCClk = param->VCOFreq / AdcClk;
            param->divisorDACInClk = param->VCOFreq / DacClk;
            param->divisorDACDataClk = param->VCOFreq / DacDataClk;
            param->divisorRsvdClk = 0; //TODO ??
            param->divisorClkOutClk = 0;
            return ret;
        }
    }
    return ADAC250_NO_VALID_PLL_SOLUTION;
}


//P value to value we need to set in the register
static char prescaler_2reg(char value)
{
    switch(value)
    {
    default:
    case 2:
        return 2;
    case 4:
        return 3;
    case 8:
        return 4;
    case 16:
        return 5;
    case 32:
        return 6;
    }
}


static unsigned char divider_2reg(unsigned char value)
{
    unsigned char tmp;
    unsigned char reg;
    switch(value)
    {
    case 0:
    case 1:
        return 0;
    default:
        tmp = value >> 1; // div 2
        tmp--;
        reg = tmp | tmp<<4;
        if(value & 0x1)
        {
            reg++;
        }
    }
    return reg;
}

#ifndef ONLY_CALCULATE
static Result ADAC250_PLLSoftReset(ADAC250Board_t * board)
{
    AD9511_Register_Map_t reg;
    ADAC250_ReadSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_00, 1, &reg.Register_00.Byte);
    reg.Register_00.Bit.Soft_Reset=1;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_00, 1, &reg.Register_00.Byte);
    reg.Register_00.Bit.Soft_Reset=0;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_00, 1, &reg.Register_00.Byte);
    return OK;
}


Result ADAC250_PLL_Init(ADAC250Board_t * board)
{
    ADAC250Adaptator_t fpgareg;
    // VCO:1000MHz   ADAC:250MHz
    unsigned int A = 0;
    unsigned int B = 1250/5;
    unsigned int R = 100/5;     // PFD = 500KHz best phase noise.
    unsigned int P = 4<<2; //8    3<<2 //4
    unsigned int DIV = 0x11;
    unsigned char B1 = (B&0x1F00)>>8;
    unsigned char B2 = (B&0xFF);
    unsigned int regAdd[] = {0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x3D,0x41,0x45,0x4A,0x4C,0x4E,0x50,0x52};
    char regVal[] =         {A   ,B1  ,B2  ,0x04,0x47,0x00,P   ,0x00,R   ,0x00,0x04,0x01,0x02,DIV ,DIV ,DIV ,DIV ,DIV};
    unsigned int i;

    //PLL out of reset
    ADAC250_ReadControlRegister(board, ADAC250_MISC_CONTROL, &fpgareg.Misc_Control.Register);
    fpgareg.Misc_Control.PLL_Function = 0;
    ADAC250_WriteControlRegister(board, ADAC250_MISC_CONTROL, fpgareg.Misc_Control.Register);
    pfADAC250_usleep(2);
    fpgareg.Misc_Control.PLL_Function = 1;
    ADAC250_WriteControlRegister(board, ADAC250_MISC_CONTROL, fpgareg.Misc_Control.Register);
    //ADAC250_PLLSoftReset(board);
    pfADAC250_usleep(20);

    for (i=0;i<sizeof(regVal);i++)
    {
        ADAC250_WriteSPIRegisters(board, ADAC250_PLL, regAdd[i], 1, &regVal[i]);
    }
    ADAC250_PLL_trigRegisterTransfert(board);
    return ADAC250_PLL_SyncOutput(board);
}




Result ADAC250_PLL_SetConfig(ADAC250Board_t * board, ADAC250_PLL_PARAM param)
{
    AD9511_Register_Map_t reg;
    memset(&reg, 0, sizeof(reg));

    //set factors
    reg.Register_04.Bit.A_Counter = param.Afactor;
    reg.Register_05.Bit.B_Counter_MSB = param.Bfactor>>8; 
    reg.Register_06.Bit.B_Counter_LSB = param.Bfactor & 0xFF; 
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_04, 3, &reg.Register_04.Byte);
    ADAC250_ReadSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_0A, 1, &reg.Register_0A.Byte);
    reg.Register_0A.Bit.Prescaler_P = prescaler_2reg(param.Pfactor);
    reg.Register_0B.Bit.R_Divider_MSB = param.Rfactor>>8;
    reg.Register_0C.Bit.R_Divider_LSB = param.Rfactor & 0xFF;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_0A, 3, &reg.Register_0A.Byte);

    //set dividers
    /* ADC <-> OUT0 */
    reg.Register_4A.Byte = divider_2reg(param.divisorADCClk);
    if(param.divisorADCClk == 1) reg.Register_4B.Bit.Bypass = 1;
    /* DAC <-> OUT1 */
    reg.Register_4C.Byte = divider_2reg(param.divisorDACInClk);
    if(param.divisorDACInClk == 1) reg.Register_4D.Bit.Bypass = 1;

    /* DAC data <-> OUT2-3 */
    reg.Register_4E.Byte = divider_2reg(param.divisorDACDataClk);
    if(param.divisorDACDataClk == 1) reg.Register_4F.Bit.Bypass = 1;
    reg.Register_50.Byte = divider_2reg(param.divisorDACDataClk);
    if(param.divisorDACDataClk == 1) reg.Register_51.Bit.Bypass = 1;

    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_4A, 14, &reg.Register_4A.Byte);

    ADAC250_PLL_trigRegisterTransfert(board);
    ADAC250_PLL_SyncOutput(board);
    return OK;
}


Result ADAC250_PLL_trigRegisterTransfert(ADAC250Board_t * board)
{
    AD9511_Register_Map_t reg;
    reg.Register_5A.Byte = 0;
    reg.Register_5A.Bit.Update_Registers = 1;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_5A, 1, &reg.Register_5A.Byte);
    return OK;
}


Result ADAC250_PLL_SyncOutput(ADAC250Board_t * board)
{
    AD9511_Register_Map_t reg;
    
    ADAC250_ReadSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_09, 1, &reg.Register_09.Byte);
    reg.Register_09.Bit.Reset_All_Counter = 1;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_09, 1, &reg.Register_09.Byte);
    reg.Register_09.Bit.Reset_All_Counter = 0;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_09, 1, &reg.Register_09.Byte);

//soft SYNCB
    ADAC250_ReadSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_58, 1, &reg.Register_58.Byte);
    reg.Register_58.Bit.Sync_Reg = 1;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_58, 1, &reg.Register_58.Byte);
    reg.Register_58.Bit.Sync_Reg = 0;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_58, 1, &reg.Register_58.Byte);
    
//Hardware SYNCB
/*    reg.Register_58.Byte=0;
    reg.Register_58.Bit.Function_Pin = 1; //SYNCB
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_58, 1, &reg.Register_58.Byte);
    ADAC250_ReadControlRegister(board, ADAC250_MISC_CONTROL, &value);
    value |= MISC_CTRL_PLL_FUNCTION;
    ADAC250_WriteControlRegister(board, ADAC250_MISC_CONTROL, value);
    value &= ~MISC_CTRL_PLL_FUNCTION;
    ADAC250_WriteControlRegister(board, ADAC250_MISC_CONTROL, value);*/
    return OK;
}


Result ADAC250_PLL_getStatus(ADAC250Board_t * board, ADAC250_PLL_STATUS * status)
{
    unsigned int value;
    Result ret;
    ret = ADAC250_ReadControlRegister(board, ADAC250_MISC_CONTROL, &value);
    *status = value & MISC_CTRL_PLL_STATUS;
    return ret;
}


Result ADAC250_PLL_SetSleep(ADAC250Board_t * board, ADAC250_PLL_SLEEP s)
{
    return ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_45, 1, (char*)&s);
}

Result ADAC250_PLL_setRefClkTuning(ADAC250Board_t * board, float fValue)
{
    Result res;
    unsigned short usValue;
	char Value[2];
    
    if(board == NULL) return ADAC250_INVALID_POINTER;
    
    if(fValue < -1.0f || fValue > 1.0f) return ADAC250_ARG_OUT_OF_RANGE;
    
    usValue = (unsigned)((fValue + 1.5f)/ 2.5f * 65535.0f);

	//prepare buffer
	Value[0] = (usValue >> 8) & 0xff;	//MSB byte
	Value[1] = (usValue >> 0) & 0xff; 	//LSB byte
    
    res = ADAC250_WriteSPIRegisters(board, ADAC250_TCVCXO, 0, 2, Value);
    if(FAILURE(res)) return res;
    
    return ADAC250_OK;
}

Result ADAC250_PLL_Init_Extern(ADAC250Board_t * board, unsigned int clkin, unsigned int adcclk, unsigned int dacclk, ADAC250_DAC_INTERPOLATION interp)
{
    ADAC250Adaptator_t fpgareg;

    // CLK1 sources distribution
    // REFIN, CLK2 disabled
    // Dividers bypassed
	unsigned int adcdivider = 0;
	unsigned int dacdivider = 0;
	unsigned int dacinterpdivider= 0;
	AD9511_Register_Map_t reg;
    memset(&reg, 0, sizeof(reg));

    //PLL out of reset
    ADAC250_ReadControlRegister(board, ADAC250_MISC_CONTROL, &fpgareg.Misc_Control.Register);
    fpgareg.Misc_Control.PLL_Function = 0;
    ADAC250_WriteControlRegister(board, ADAC250_MISC_CONTROL, fpgareg.Misc_Control.Register);
    pfADAC250_usleep(2);
    fpgareg.Misc_Control.PLL_Function = 1;
    ADAC250_WriteControlRegister(board, ADAC250_MISC_CONTROL, fpgareg.Misc_Control.Register);
    ADAC250_PLLSoftReset(board);
    pfADAC250_usleep(20);	
	
	if(clkin%adcclk)
	{
		return ADAC250_NO_VALID_PLL_SOLUTION;
	}
	else
	{
		adcdivider = clkin / adcclk;
		debug("ADC divider: %d\n\r", adcdivider);
	}
	
	if(clkin%dacclk)
	{
		return ADAC250_NO_VALID_PLL_SOLUTION;
	}
	else
	{
		dacdivider = clkin / dacclk;
		debug("DAC divider: %d\n\r", dacdivider);
	}
	
	if(clkin < (dacclk  * interp))
	{
		return ADAC250_NO_VALID_PLL_SOLUTION;
	}
	else
	{
		dacinterpdivider = dacdivider / (interp);
		debug("DAC interpolated divider: %d\n\r", dacinterpdivider);
	}
	
    //set dividers
    /* ADC <-> OUT0 */
    reg.Register_4A.Byte = divider_2reg(adcdivider);
    if(adcdivider == 1) reg.Register_4B.Bit.Bypass = 1;
    /* DAC <-> OUT1 */
    reg.Register_4C.Byte = divider_2reg(dacinterpdivider);
    if(dacinterpdivider== 1) reg.Register_4D.Bit.Bypass = 1;

    /* DAC data <-> OUT2-3 */
    reg.Register_4E.Byte = divider_2reg(dacdivider);
    if(dacdivider == 1) reg.Register_4F.Bit.Bypass = 1;
    reg.Register_50.Byte = divider_2reg(dacdivider);
    if(dacdivider == 1) reg.Register_51.Bit.Bypass = 1;
	
	reg.Register_45.Bit.Select_CLK_In = 1;
	reg.Register_45.Bit.CLK1_PD = 0;
	reg.Register_45.Bit.CLK2_PD = 1;
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_45, 1, &reg.Register_45.Byte);
    ADAC250_WriteSPIRegisters(board, ADAC250_PLL, AD9511_REGISTER_4A, 14, &reg.Register_4A.Byte);

    ADAC250_PLL_trigRegisterTransfert(board);
    ADAC250_PLL_SyncOutput(board);
	
    return(0);
}


adp_result_t ADAC250_InitPLL(ADAC250Board_t * board, unsigned int refclk, unsigned int adcclk, unsigned int dacclk, ADAC250_DAC_INTERPOLATION interp, ADAC250_CLOCK_SOURCES clksrc)
{
    Result ret;
	ADAC250_PLL_PARAM param;
	if(adcclk < ADAC250_MIN_ADC_PLL_CLK_FREQ)
	{
		return (ADAC250_NO_VALID_PLL_SOLUTION);
	}
	if(dacclk < ADAC250_MIN_DAC_PLL_CLK_FREQ)
	{
		return (ADAC250_NO_VALID_PLL_SOLUTION);
	}
	switch(clksrc)
	{
		case ADAC250_CLOCKIN_10MHZREF:
		
			ret  = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_REFOUT, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_PLLCLK1, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_PLLREF, ADAC250_CLOCKIN_10MHZ);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_FMCCLK1, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			// If internal reference clock to be used
			ret = ADAC250_PLL_Init(board);
			if(ret != 0) 
			{
				//debug("Error: Initializing PLL\n\r");
				return ret;
			}
			ret = ADAC250_PLL_CalculateParams(refclk, adcclk, dacclk, interp, &param);
			if(ret != 0) 
			{
				//debug("Error: Calculating PLL \n\r");
				return ret;
			}
			ret = ADAC250_PLL_SetConfig(board, param);
			if(ret != 0) 
			{
				//bug("Error: Writing pll config \n\r");
				return ret;
			}
			break;
		case ADAC250_CLOCKIN_EXTERNALREF:
		
			ret  = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_REFOUT, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_PLLCLK1, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_PLLREF, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_FMCCLK1, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			// If internal reference clock to be used
			ret = ADAC250_PLL_Init(board);
			if(ret != 0) 
			{
				//debug("Error: Initializing PLL\n\r");
				return ret;
			}
			ret = ADAC250_PLL_CalculateParams(refclk, adcclk, dacclk, interp, &param);
			if(ret != 0) 
			{
				//debug("Error: Calculating PLL \n\r");
				return ret;
			}
			ret = ADAC250_PLL_SetConfig(board, param);
			if(ret != 0) 
			{
				//debug("Error: Writing pll config \n\r");
				return ret;
			}
			break;
		case ADAC250_CLOCKIN_EXTERNALCLOCK:
			ret  = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_REFOUT, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_PLLCLK1, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_PLLREF, ADAC250_CLOCKIN_REFIN);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			ret = ADAC250_MUX_ConfigClockOutput(board, ADAC250_CLOCKOUT_FMCCLK1, ADAC250_CLOCKIN_PLLCLKOUT2);
			if(ret != 0) 
			{
				//debug("Error: Setting clock mux\n\r");
				return ret;
			}
			// If internal reference clock to be used
			ret = ADAC250_PLL_Init_Extern(board, refclk, adcclk, dacclk, interp);
			if(ret != 0) 
			{
				//debug("Error: Initializing PLL\n\r");
				return ret;
			}
			break;
	}
	return OK;
}


#endif /*ONLY_CALCULATE*/
