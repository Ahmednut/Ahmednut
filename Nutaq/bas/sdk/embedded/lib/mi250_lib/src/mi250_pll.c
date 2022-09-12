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
 * @file       mi250_pll.c
 * @brief      MI250 configuration functions API for the PLL
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
#include "mi250.h"
#include "mi250_pll.h"
#include "mi250_user.h"
#include "mi250_AD9510_def.h"
#include "ADS62P49_def.h"
#include <string.h> //memset
#include <stdio.h>

#ifdef DEBUG
#define DEBUG_CALIBRATION 1
#endif

// Debug related definitions
#ifdef DEBUG_CALIBRATION
    // These macros are used to get printout info while debugging
    #define FUNC_ENTER()      printf("MI125: enter %s\n", __FUNCTION__)
    #define FUNC_EXIT()       printf("MI125: exit  %s\n", __FUNCTION__)
    #define PDEBUG(...) printf(__VA_ARGS__)

#else

    #define FUNC_ENTER()
    #define FUNC_EXIT()
    #define PDEBUG(...)

#endif



// http://en.wikipedia.org/wiki/Greatest_common_divisor
static int gcd ( unsigned a, unsigned b )
{
    int c;
    
    while ( a != 0 )
    {
        c = a;
        a = b%a;
        b = c;
    }
    
    return b;
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
        case 1: {
            reg = 0;
            break;
        }
        default: {
            tmp = value >> 1; // Divide by 2
            tmp--;
            reg = tmp | tmp<<4;
            if(value & 0x1) {
                reg++;
            }
            break;
        }
    }

    return reg;
}

static Result MI250_CalculateVCOfrequency(unsigned AdcClk, unsigned VCOFreq[10], int * VCOFreq_size)
{
    int i,k;
    long long multiple, vco;
    int divider_adc;
    
    vco = AdcClk;
    multiple = AdcClk;
    k = 0;
    i = 2;

    while(vco <= MI250_MAXVCOFREQ)
    {
        if(vco >= MI250_MINVCOFREQ)
        {
            divider_adc = (int) vco / AdcClk;
            if((divider_adc >= 1) && (divider_adc <= 32)) 
            {
                VCOFreq[k] = (int) vco;
                k++;
            }
            
            if(k >= *VCOFreq_size)
                break;
        }
        
        vco = multiple * i;
        i++;
    }
    
    *VCOFreq_size = k;
    
    return(k > 0 ? OK : MI250_NO_VALID_DIVIDER);
}

static Result MI250_GetFactors(unsigned int refIn, unsigned int vco, MI250_PLL_PARAM * param)
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
        
        if ( (refIn % divi == 0) && (pfdFreq >= MI250_PFD_MIN_FREQ) && (pfdFreq <= MI250_PFD_MAX_FREQ)) 
            break;
            
        if ( refTemp < MI250_PFD_MIN_FREQ )
        {
            return MI250_PFDFREQ_OUT_OF_RANGE;
        }
    }

    R = refIn / pfdFreq;
    if (R<1)
    {
        return MI250_NO_VALID_R;
    }

    if (R>16383)
    {
        return MI250_REF_CLOCK_TO_HIGH;
    }

    if( vco < 600e6) //P = 2 DM CLK2: <600 MHz
    {
        i=0;
    } else if (vco < 1e9) { //P = 4 DM CLK2: <1000 MHz
        i=1;
    } else if (vco <= MI250_MAXVCOFREQ) { 
        i=2;
    }else {
        return MI250_VCO_OUT_OF_RANGE;
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

    return MI250_NO_VALID_PLL_SOLUTION;
}

Result MI250_PLL_CalculateParams(unsigned refClk, unsigned AdcClk, MI250_PLL_CFG_PARAM cfgParam, MI250_PLL_PARAM * param)
{
    Result ret;
    int i;
    unsigned VCOFreq[10];
    int VCOFreq_size = sizeof(VCOFreq)/sizeof(unsigned);

    ret = MI250_CalculateVCOfrequency(AdcClk, VCOFreq, &VCOFreq_size);
    if(FAILURE(ret)) 
        return ret;

    for (i = 0; i < VCOFreq_size; i++)
    {
        ret = MI250_GetFactors(refClk, VCOFreq[i], param);
        if(SUCCESS(ret))
        {
            param->divisorADCClk = param->VCOFreq / AdcClk;
            param->divisorRsvdClk = 0; 
            param->divisorClkOutClk = 0;
            
            if((cfgParam == MI250_PLL_DUTY_50) && (VCOFreq_size > 1)) {
                if((param->divisorADCClk % 2) == 0) {
                    return ret;
                }
            }
            else {
                return ret;
            }
            
        }
    }

    return MI250_NO_VALID_PLL_SOLUTION;
}


#ifndef ONLY_CALCULATE
static Result MI250_PLLSoftReset(MI250Board_t * board)
{
    MI250_AD9510_Register_Map_t reg;
    MI250_ReadSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_00, 1, &reg.Register_00.Byte);
    reg.Register_00.Bit.Soft_Reset=1;
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_00, 1, &reg.Register_00.Byte);
    reg.Register_00.Bit.Soft_Reset=0;
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_00, 1, &reg.Register_00.Byte);
    
    return OK;
}

Result MI250_PLL_Init(MI250Board_t * board)
{
    MI250Adaptator_t fpgareg;

    /*!!!!CL 
    The default loop filter is designed for a phase detector frequency of 10MHz, loop bandwidth
    of 10kHz, phase margin of 45 deg, and a charge pump of 4.8mA.
    1) The internal reference is enabled by driving REF_EN high. The internal reference
    should only be enabled in case internal clock is used and there is no external
    reference applied. FMC10x with LPC connector cannot access the REF_EN, but
    should use the status output of the AD9510 to enable the internal reference oscillator.
    2) The communication bus can only be used in unidirectional mode; thus using SDIO as
    data input and SDO as data output.
    3) It is recommended to disable the unused clock outputs 4 and 7.
    4) It is recommended to disable PLL functions and CLK2 input on the AD9510 when an
    external sampling clock is applied.
    5) Although the AD9510 provides separate dividers on each clock output, it is not
    recommended to use different divider settings.
    6) In case internal clock is used the PLL functions needs to be enabled. The
    recommended phase detector frequency is 10MHz. In case the internal reference is
    used the R divider should be set to 10. In case an external reference is used the R
    divider should be set to ROUND(Fref / 10). Refer also to Appendix A.
    7) Lower phase detector frequencies may be used to increase flexibility on the output
    frequency. The stability of the PLL is not guaranteed in all cases.
    */

    // VCO:1200MHz   MI:200MHz
    /*unsigned int A = 0;
    unsigned int B = 15;
    unsigned int R = 10;   // PFD = 10MHz.
    unsigned int P = 4<<2; //8    3<<2 //4
    unsigned int DIV = 0x22;
    unsigned char B1 = (B&0x1F00)>>8;
    unsigned char B2 = (B&0xFF);
    unsigned int regAdd[] = {0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x3C,0x40,0x41,0x42,0x43,0x45,0x48,0x4A,0x4C,0x4E,0x50,0x52,0x54,0x56};
    char regVal[] =         {A   ,B1  ,B2  ,0x04,0x77,0x70,P   ,0x00,R   ,0x00,0x04,0x03,0x02,0x02,0x03,0x02,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV };
    */
    
    // VCO:1200MHz   MI:100MHz
    unsigned int A = 0;
    unsigned int B = 15;
    unsigned int R = 10;   // PFD = 10MHz.
    unsigned int P = 4<<2; //8    3<<2 //4
    unsigned int DIV = 0x55;
    unsigned char B1 = (B&0x1F00)>>8;
    unsigned char B2 = (B&0xFF);
    unsigned int regAdd[] = {0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x3C,0x40,0x41,0x42,0x43,0x45,0x48,0x4A,0x4C,0x4E,0x50,0x52,0x54,0x56};
//  char regVal[] =         {A   ,B1  ,B2  ,0x04,0x77,0x20,P   ,0x00,R   ,0x00,0x08,0x03,0x02,0x02,0x03,0x02,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV };
    char regVal[] =         {A   ,B1  ,B2  ,0x04,0x47,0x70,P   ,0x00,R   ,0x00,0x08,0x03,0x02,0x02,0x03,0x02,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV };
    unsigned int i;

    //PLL out of reset
    MI250_ReadControlRegister(board, MI250_PLL_CONTROL, &fpgareg.Pll_Control.Register);
    fpgareg.Pll_Control.PLL_Function = 0;
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);
    board->pfUsleep(2);
    fpgareg.Pll_Control.PLL_Function = 1;
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);
    //MI250_PLLSoftReset(board);
    board->pfUsleep(20);

    MI250_ReadControlRegister(board, MI250_PLL_CONTROL, &fpgareg.Pll_Control.Register);
    fpgareg.Pll_Control.PLL_RefEn = 1; // Uses internal reference.
    fpgareg.Pll_Control.VCO_PwrEn = 1; // Uses VCO.
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);

    for(i = 0; i < sizeof(regVal); i++)
    {
        MI250_WriteSPIRegisters(board, MI250_PLL, regAdd[i], 1, &regVal[i]);
    }
    MI250_PLL_trigRegisterTransfert(board);
    
    return MI250_PLL_SyncOutput(board);
}


Result MI250_PLL_Init_Extern_Ref(MI250Board_t * board)
{
    MI250Adaptator_t fpgareg;

    /*!!!!CL 
    The default loop filter is designed for a phase detector frequency of 10MHz, loop bandwidth
    of 10kHz, phase margin of 45 deg, and a charge pump of 4.8mA.
    1) The internal reference is enabled by driving REF_EN high. The internal reference
    should only be enabled in case internal clock is used and there is no external
    reference applied. FMC10x with LPC connector cannot access the REF_EN, but
    should use the status output of the AD9510 to enable the internal reference oscillator.
    2) The communication bus can only be used in unidirectional mode; thus using SDIO as
    data input and SDO as data output.
    3) It is recommended to disable the unused clock outputs 4 and 7.
    4) It is recommended to disable PLL functions and CLK2 input on the AD9510 when an
    external sampling clock is applied.
    5) Although the AD9510 provides separate dividers on each clock output, it is not
    recommended to use different divider settings.
    6) In case internal clock is used the PLL functions needs to be enabled. The
    recommended phase detector frequency is 10MHz. In case the internal reference is
    used the R divider should be set to 10. In case an external reference is used the R
    divider should be set to ROUND(Fref / 10). Refer also to Appendix A.
    7) Lower phase detector frequencies may be used to increase flexibility on the output
    frequency. The stability of the PLL is not guaranteed in all cases.
    */

    // VCO:1200MHz   MI:200MHz
    /*unsigned int A = 0;
    unsigned int B = 15;
    unsigned int R = 10;   // PFD = 10MHz.
    unsigned int P = 4<<2; //8    3<<2 //4
    unsigned int DIV = 0x22;
    unsigned char B1 = (B&0x1F00)>>8;
    unsigned char B2 = (B&0xFF);
    unsigned int regAdd[] = {0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x3C,0x40,0x41,0x42,0x43,0x45,0x48,0x4A,0x4C,0x4E,0x50,0x52,0x54,0x56};
    char regVal[] =         {A   ,B1  ,B2  ,0x04,0x77,0x70,P   ,0x00,R   ,0x00,0x04,0x03,0x02,0x02,0x03,0x02,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV };
    */
    
    // VCO:1200MHz   MI:100MHz
    unsigned int A = 0;
    unsigned int B = 15;
    unsigned int R = 10;   // PFD = 10MHz.
    unsigned int P = 4<<2; //8    3<<2 //4
    unsigned int DIV = 0x55;
    unsigned char B1 = (B&0x1F00)>>8;
    unsigned char B2 = (B&0xFF);
    unsigned int regAdd[] = {0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x3C,0x40,0x41,0x42,0x43,0x45,0x48,0x4A,0x4C,0x4E,0x50,0x52,0x54,0x56};
    char regVal[] =         {A   ,B1  ,B2  ,0x04,0x47,0x70,P   ,0x00,R   ,0x00,0x08,0x03,0x02,0x02,0x03,0x02,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV ,DIV };
    unsigned int i;

    //PLL out of reset
    MI250_ReadControlRegister(board, MI250_PLL_CONTROL, &fpgareg.Pll_Control.Register);
    fpgareg.Pll_Control.PLL_Function = 0;
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);
    board->pfUsleep(2);
    fpgareg.Pll_Control.PLL_Function = 1;
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);
    //MI250_PLLSoftReset(board);
    board->pfUsleep(20);

    MI250_ReadControlRegister(board, MI250_PLL_CONTROL, &fpgareg.Pll_Control.Register);
    fpgareg.Pll_Control.PLL_RefEn = 0; // Uses external reference
    fpgareg.Pll_Control.VCO_PwrEn = 1; // Uses VCO.
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);

    for(i = 0; i < sizeof(regVal); i++)
    {
        MI250_WriteSPIRegisters(board, MI250_PLL, regAdd[i], 1, &regVal[i]);
    }
    MI250_PLL_trigRegisterTransfert(board);
    
    return MI250_PLL_SyncOutput(board);
}


Result MI250_PLL_Init_Extern(MI250Board_t * board)
{
    MI250Adaptator_t fpgareg;

    /*!!!!CL 
    The default loop filter is designed for a phase detector frequency of 10MHz, loop bandwidth
    of 10kHz, phase margin of 45 deg, and a charge pump of 4.8mA.
    1) The internal reference is enabled by driving REF_EN high. The internal reference
    should only be enabled in case internal clock is used and there is no external
    reference applied. FMC10x with LPC connector cannot access the REF_EN, but
    should use the status output of the AD9510 to enable the internal reference oscillator.
    2) The communication bus can only be used in unidirectional mode; thus using SDIO as
    data input and SDO as data output.
    3) It is recommended to disable the unused clock outputs 4 and 7.
    4) It is recommended to disable PLL functions and CLK2 input on the AD9510 when an
    external sampling clock is applied.
    5) Although the AD9510 provides separate dividers on each clock output, it is not
    recommended to use different divider settings.
    6) In case internal clock is used the PLL functions needs to be enabled. The
    recommended phase detector frequency is 10MHz. In case the internal reference is
    used the R divider should be set to 10. In case an external reference is used the R
    divider should be set to ROUND(Fref / 10). Refer also to Appendix A.
    7) Lower phase detector frequencies may be used to increase flexibility on the output
    frequency. The stability of the PLL is not guaranteed in all cases.
    */

    // CLK1 sources distribution
    // REFIN, CLK2 disabled
    // Dividers bypassed
    unsigned int regAdd[] = {0x0B,0x0D,0x3C,0x40,0x41,0x42,0x43,0x45,0x49,0x4B,0x4D,0x4F,0x51,0x53,0x55,0x57};
    char regVal[] =         {0x00,0x00,0x08,0x03,0x02,0x02,0x03,0x1D,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80};
    unsigned int i;

    //PLL out of reset
    MI250_ReadControlRegister(board, MI250_PLL_CONTROL, &fpgareg.Pll_Control.Register);
    fpgareg.Pll_Control.PLL_Function = 0;
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);
    board->pfUsleep(2);
    fpgareg.Pll_Control.PLL_Function = 1;
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);
    MI250_PLLSoftReset(board);
    board->pfUsleep(20);

    MI250_ReadControlRegister(board, MI250_PLL_CONTROL, &fpgareg.Pll_Control.Register);
    fpgareg.Pll_Control.PLL_RefEn = 0; // Uses external sampling clock
    fpgareg.Pll_Control.VCO_PwrEn = 0; // Does NOT use VCO.
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, fpgareg.Pll_Control.Register);

    for(i = 0; i < sizeof(regVal); i++)
    {
        MI250_WriteSPIRegisters(board, MI250_PLL, regAdd[i], 1, &regVal[i]);
    }
    MI250_PLL_trigRegisterTransfert(board);

    return(0);
}


Result MI250_PLL_SetConfig(MI250Board_t * board, MI250_PLL_PARAM param)
{
    MI250_AD9510_Register_Map_t reg;
    memset(&reg, 0, sizeof(reg));

    //set factors
    reg.Register_04.Bit.A_Counter = param.Afactor;
    reg.Register_05.Bit.B_Counter_MSB = param.Bfactor>>8; 
    reg.Register_06.Bit.B_Counter_LSB = param.Bfactor & 0xFF; 
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_04, 3, &reg.Register_04.Byte);

    MI250_ReadSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_0A, 1, &reg.Register_0A.Byte);
    reg.Register_0A.Bit.Prescaler_P = prescaler_2reg(param.Pfactor);
    reg.Register_0B.Bit.R_Divider_MSB = param.Rfactor>>8;
    reg.Register_0C.Bit.R_Divider_LSB = param.Rfactor & 0xFF;
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_0A, 3, &reg.Register_0A.Byte);

    //set dividers
    /* ADC <-> OUT0 */
    reg.Register_48.Byte = divider_2reg(param.divisorADCClk);
    if(param.divisorADCClk == 1) { 
        reg.Register_49.Bit.Bypass = 1;
    }
    /* ADC <-> OUT1 */
    reg.Register_4A.Byte = divider_2reg(param.divisorADCClk);
    if(param.divisorADCClk == 1) { 
        reg.Register_4B.Bit.Bypass = 1;
    }
    /* ADC <-> OUT2 */
    reg.Register_4C.Byte = divider_2reg(param.divisorADCClk);
    if(param.divisorADCClk == 1) { 
        reg.Register_4D.Bit.Bypass = 1;
    }
    /* ADC <-> OUT3 */
    reg.Register_4E.Byte = divider_2reg(param.divisorADCClk);
    if(param.divisorADCClk == 1) { 
        reg.Register_4F.Bit.Bypass = 1;
    }
    /* CLOKC OUT <-> OUT5 */
    reg.Register_52.Byte = divider_2reg(param.divisorADCClk);
    if(param.divisorADCClk == 1) { 
        reg.Register_53.Bit.Bypass = 1;
    }
    /* FMC CLOCK OUT <-> OUT6 */
    reg.Register_54.Byte = divider_2reg(param.divisorADCClk);
    if(param.divisorADCClk == 1) { 
        reg.Register_55.Bit.Bypass = 1;
    }
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_48, 16, &reg.Register_48.Byte);

    MI250_PLL_trigRegisterTransfert(board);
    MI250_PLL_SyncOutput(board);
    return OK;
}

Result _MI250_scan_taps(MI250Board_t * MI250Board, char validDelay[4][32])
{
	int i, j;
    
	// Check validity of all delay values
	for(i=0; i<32; i++)
	{
			MI250Board->Adaptor->ADC_12_Control.AdcIdelayValue = i;
			MI250Board->Adaptor->ADC_34_Control.AdcIdelayValue = i;
			MI250Board->Adaptor->ADC_56_Control.AdcIdelayValue = i;
			MI250Board->Adaptor->ADC_78_Control.AdcIdelayValue = i;

			MI250Board->pfUsleep(50);
			validDelay[0][i] = MI250Board->Adaptor->ADC_12_Control.AdcPatternError;
			validDelay[1][i] = MI250Board->Adaptor->ADC_34_Control.AdcPatternError;
			validDelay[2][i] = MI250Board->Adaptor->ADC_56_Control.AdcPatternError;
			validDelay[3][i] = MI250Board->Adaptor->ADC_78_Control.AdcPatternError;

	}
    
#ifdef DEBUG_CALIBRATION
    printf("Result of the error scan\n");
    for(j=0; j<4; j++)
    {
        printf("ADC #%d : ",j);
        
        for(i=0; i<32; i++)
        {
            printf("%d",validDelay[j][i]);
        }
        
        printf("\n");
    }
#endif
    
    return OK;
}

Result _MI250_find_center_tap(MI250Board_t * MI250Board, char validDelay[4][32], char delayGroupMiddle[4], char delayGroupMax[4], char startIndex, char stopIndex)
{
	int i,j;
	char delayGroupCnt[4];
	char calibcount = 0;
    
    for(j = 0; j < 4; j++)
    {
    	// Default values
    	delayGroupMiddle[j] = 16;
    	delayGroupMax[j] = 0;
        delayGroupCnt[j] = 0;
        
        for(i=startIndex; i<stopIndex; i++)
        {
			if(validDelay[j][i]==0) // no error
			{
				delayGroupCnt[j]++;
				if(delayGroupCnt[j] > delayGroupMax[j])
				{
					delayGroupMax[j] = delayGroupCnt[j];
					delayGroupMiddle[j] = i - (delayGroupCnt[j] / 2);
				}
			}
			else
			{
				delayGroupCnt[j] = 0;
			}
		}
	}
    
#ifdef DEBUG_CALIBRATION
    printf("Calibration result\n");
    for(j=0; j<4; j++)
    {
        printf("ADC #%d : %2d, %2d",j, delayGroupMiddle[j], delayGroupMax[j]);
        printf("    start : %2d, stop : %2d\n", startIndex, stopIndex);
    }
#endif

    calibcount = 0;
	if(delayGroupMax[0] > 2) calibcount++;
	if(delayGroupMax[1] > 2) calibcount++;
	if(delayGroupMax[2] > 2) calibcount++;
	if(delayGroupMax[3] > 2) calibcount++;

    if(calibcount == 4)
        return OK;
    else
        return MI250CALIBRATIONFAILURE;
}

Result MI250_PLL_Calibrate(MI250Board_t * MI250Board, unsigned int acqfreq)
{
    const int MAX_ITERATION_TO_FIND_MIDDLE_TAP = 4;
    Result res;
	int i;
	char validDelay[4][32];

	char clockvalue = 0;
	char delayGroupMiddle[4]= {0,0,0,0};
	char delayGroupMax[4] = {0,0,0,0};
	char value = 4;
    int error[4];
    
    MI250Board->CalibResult.cSucceeded = 0;

	// Set ADCs in test mode
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_12, ADS62P49_REGISTER_62, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_12, ADS62P49_REGISTER_75, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_12, ADS62P49_REGISTER_44, 1, &clockvalue);

	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_34, ADS62P49_REGISTER_62, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_34, ADS62P49_REGISTER_75, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_34, ADS62P49_REGISTER_44, 1, &clockvalue);

	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_56, ADS62P49_REGISTER_62, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_56, ADS62P49_REGISTER_75, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_56, ADS62P49_REGISTER_44, 1, &clockvalue);

	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_78, ADS62P49_REGISTER_62, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_78, ADS62P49_REGISTER_75, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_78, ADS62P49_REGISTER_44, 1, &clockvalue);

    // Set the clk idelya value in the middle of the range to have
    // the biggest margin in both direction to adjust the lane delay
    // at the 16th tap.
    // This is done in case other MI250 boards are calibrated on the reference tap.
    // If the reference tap is at 16th tap, there is more room for the other to adjust themself.
    if(MI250Board->Adaptor->Core_Reset.ClockMaster == 1)
    {
        MI250Board->Adaptor->ADC_12_Control.AdcClkIdelayValue = 16;
        MI250Board->Adaptor->ADC_12_Control.AdcBitSlip = 0;
        MI250Board->Adaptor->ADC_34_Control.AdcBitSlip = 0;
        MI250Board->Adaptor->ADC_56_Control.AdcBitSlip = 0;
        MI250Board->Adaptor->ADC_78_Control.AdcBitSlip = 0;
    
        // Find bitslip closer to 16th tap
        for(i=0; i<MAX_ITERATION_TO_FIND_MIDDLE_TAP; i++)
        {        
            // Check validity of all delay values
            _MI250_scan_taps(MI250Board, validDelay);
            
            // Find center tap
            res = _MI250_find_center_tap(MI250Board, validDelay, delayGroupMiddle, delayGroupMax, 0, 31);
            if(FAILURE(res) || delayGroupMiddle[0] < 6 || delayGroupMiddle[0] > 26)
            {
                if(MI250Board->Adaptor->ADC_12_Control.AdcBitSlip == 0)
                {
                    // If not valid center tap try with a different bitslip setting
                    MI250Board->Adaptor->ADC_12_Control.AdcClkIdelayValue = 16;
                    MI250Board->Adaptor->ADC_12_Control.AdcBitSlip = 1;
                    MI250Board->Adaptor->ADC_34_Control.AdcBitSlip = 1;
                    MI250Board->Adaptor->ADC_56_Control.AdcBitSlip = 1;
                    MI250Board->Adaptor->ADC_78_Control.AdcBitSlip = 1;
                    PDEBUG("Set bitslip.\n");
                    i=0;
                    continue;
                }
                else
                {
                    // If both bitslip settings do not worked
                    // return an error.
                    return res;
                }
            }
            
            // Do not modify the clock delay at the last iteration.
            // The last itteration will be result used.
            if(i < (MAX_ITERATION_TO_FIND_MIDDLE_TAP-1))
            {
                char clock_delay;
                PDEBUG("Found the reference lane delay should be %d.\n",delayGroupMiddle[0]);
                clock_delay = MI250Board->Adaptor->ADC_12_Control.AdcClkIdelayValue + 16 - delayGroupMiddle[0];
                if (clock_delay < 0)  clock_delay = 0;
                if (clock_delay > 31) clock_delay = 31;
                MI250Board->Adaptor->ADC_12_Control.AdcClkIdelayValue = clock_delay;
                PDEBUG("The clock delay will be set to %d.\n",MI250Board->Adaptor->ADC_12_Control.AdcClkIdelayValue);
            }
        }
    }
    else
    {
        char cStartIndex, cStopIndex;
        char cIndexOffset;
        
        if(MI250Board->pMasterCalibResult == NULL) return MI250MASTERCALIBPTRINV;
        if(MI250Board->pMasterCalibResult->cSucceeded == 0) return MI250MASTERCALIBNOTDONE;
        
        MI250Board->Adaptor->ADC_12_Control.AdcBitSlip = MI250Board->pMasterCalibResult->cRefBitSlip;
        MI250Board->Adaptor->ADC_34_Control.AdcBitSlip = MI250Board->pMasterCalibResult->cRefBitSlip;
        MI250Board->Adaptor->ADC_56_Control.AdcBitSlip = MI250Board->pMasterCalibResult->cRefBitSlip;
        MI250Board->Adaptor->ADC_78_Control.AdcBitSlip = MI250Board->pMasterCalibResult->cRefBitSlip;
        
        if(MI250Board->pMasterCalibResult->usRefLaneDelay != 0 &&
            MI250Board->pLaneDelay != NULL)
        {
            // 73 ps per tap with a reference clock of 200 MHz
            cIndexOffset = (MI250Board->pLaneDelay->delta[FMCEEPROM_HPC_LA04] - MI250Board->pMasterCalibResult->usRefLaneDelay) / 73;
        }
        else
        {
            cIndexOffset = 0;
        }
        PDEBUG("The index offset for the PCB lane length is : %d.\n",cIndexOffset);
        
        cStartIndex = (MI250Board->pMasterCalibResult->cRefIdelayMiddle - cIndexOffset - MI250Board->pMasterCalibResult->cRefIdelayWidth/2 - 2);
        if(cStartIndex<0) cStartIndex = 0;
        
        cStopIndex = (MI250Board->pMasterCalibResult->cRefIdelayMiddle - cIndexOffset + MI250Board->pMasterCalibResult->cRefIdelayWidth/2 + 2);
        if(cStopIndex>31) cStopIndex = 31;
        
        // Check validity of all delay values
        _MI250_scan_taps(MI250Board, validDelay);
        
        // Find center tap
        res = _MI250_find_center_tap(MI250Board, validDelay, delayGroupMiddle, delayGroupMax, cStartIndex, cStopIndex);
        if(FAILURE(res)) return res;
    }

    
    // Detect if the result valibration is still good
    MI250Board->Adaptor->ADC_12_Control.AdcIdelayValue = delayGroupMiddle[0];
    MI250Board->Adaptor->ADC_34_Control.AdcIdelayValue = delayGroupMiddle[1];
    MI250Board->Adaptor->ADC_56_Control.AdcIdelayValue = delayGroupMiddle[2];
    MI250Board->Adaptor->ADC_78_Control.AdcIdelayValue = delayGroupMiddle[3];
    MI250Board->pfUsleep(50);
    error[0] = MI250Board->Adaptor->ADC_12_Control.AdcPatternError;
    error[1] = MI250Board->Adaptor->ADC_34_Control.AdcPatternError;
    error[2] = MI250Board->Adaptor->ADC_56_Control.AdcPatternError;
    error[3] = MI250Board->Adaptor->ADC_78_Control.AdcPatternError;
    
    if(!(error[0] || error[1] || error[2] || error[3]))
    {
        MI250Board->CalibResult.cSucceeded = 1;
        res = OK;
    }
    else res = MI250CALIBRATIONFAILURE;
    
    MI250Board->CalibResult.cRefIdelayMiddle = delayGroupMiddle[0];
    MI250Board->CalibResult.cRefIdelayWidth = delayGroupMax[0];
    MI250Board->CalibResult.cRefBitSlip = MI250Board->Adaptor->ADC_12_Control.AdcBitSlip;
    // ADC A bit 0 is FMC LA04 lane
    if(MI250Board->pLaneDelay != NULL)
    {
        MI250Board->CalibResult.usRefLaneDelay = MI250Board->pLaneDelay->delta[FMCEEPROM_HPC_LA04];
        PDEBUG("Set reference lane PCB lane length to : %d.\n",MI250Board->CalibResult.usRefLaneDelay);
    }
    else
    {
        MI250Board->CalibResult.usRefLaneDelay = 0;
    }

	// reset ADCs to normal setup
	value = 0;
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_12, ADS62P49_REGISTER_62, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_12, ADS62P49_REGISTER_75, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_34, ADS62P49_REGISTER_62, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_34, ADS62P49_REGISTER_75, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_56, ADS62P49_REGISTER_62, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_56, ADS62P49_REGISTER_75, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_78, ADS62P49_REGISTER_62, 1, &value);
	MI250_WriteSPIRegisters(MI250Board, MI250_ADC_78, ADS62P49_REGISTER_75, 1, &value);
    
	return res;

}
Result MI250_PLL_trigRegisterTransfert(MI250Board_t * board)
{
    MI250_AD9510_Register_Map_t reg;
    
    reg.Register_5A.Byte = 0;
    reg.Register_5A.Bit.Update_Registers = 1;
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_5A, 1, &reg.Register_5A.Byte);
    
    return OK;
}

Result MI250_PLL_SyncOutput(MI250Board_t * board)
{
    MI250_AD9510_Register_Map_t reg;
    
    MI250_ReadSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_09, 1, &reg.Register_09.Byte);
    reg.Register_09.Bit.Reset_All_Counter = 1;
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_09, 1, &reg.Register_09.Byte);
    MI250_PLL_trigRegisterTransfert(board);

    reg.Register_09.Bit.Reset_All_Counter = 0;
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_09, 1, &reg.Register_09.Byte);
    MI250_PLL_trigRegisterTransfert(board);

//soft SYNCB
    MI250_ReadSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_58, 1, &reg.Register_58.Byte);
    reg.Register_58.Bit.Sync_Reg = 1;
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_58, 1, &reg.Register_58.Byte);
    MI250_PLL_trigRegisterTransfert(board);

    reg.Register_58.Bit.Sync_Reg = 0;
    MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_58, 1, &reg.Register_58.Byte);
    MI250_PLL_trigRegisterTransfert(board);
    
//Hardware SYNCB
/*    reg.Register_58.Byte=0;
    reg.Register_58.Bit.Function_Pin = 1; //SYNCB
    MI250_WriteSPIRegisters(board, MI250_PLL, AD9510_REGISTER_58, 1, &reg.Register_58.Byte);
    MI250_ReadControlRegister(board, MI250_PLL_CONTROL, &value);
    value |= MISC_CTRL_PLL_FUNCTION;
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, value);
    value &= ~MISC_CTRL_PLL_FUNCTION;
    MI250_WriteControlRegister(board, MI250_PLL_CONTROL, value);*/
    
    return OK;
}

Result MI250_PLL_getStatus(MI250Board_t * board, MI250_PLL_STATUS * status)
{
    int value;
    Result ret;

    ret = MI250_ReadControlRegister(board, MI250_PLL_CONTROL, &value);
    *status = value & MI250_MISC_CTRL_PLL_STATUS;

    return ret;
}

Result MI250_PLL_SetSleep(MI250Board_t * board, MI250_PLL_SLEEP s)
{
    return MI250_WriteSPIRegisters(board, MI250_PLL, MI250_AD9510_REGISTER_45, 1, (char*)&s);
}

//!!!!CL To verif if needed...
Result MI250_PLL_setRefClkTuning(MI250Board_t * board, unsigned short value)
{
    //return MI250_WriteSPIRegisters(board, MI250_TCVCXO, 0, sizeof(value), (char*)&value);
    return  0;
}
#endif

