/***************************************************************************//**
 *   @file   Ad9361AdcCore.c
 *   @brief  Implementation of ADC Core Driver.
 *   @author DBogdan (dragos.bogdan@analog.com)
********************************************************************************
 * Copyright 2013(c) Analog Devices, Inc.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  - Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *  - The use of this software may or may not infringe the patent rights
 *    of one or more patent holders.  This license does not release you
 *    from the requirement that you obtain separate licenses from these
 *    patent holders to use this software.
 *  - Use of the software either in source or binary form, must be run
 *    on or directly connected to an Analog Devices Inc. component.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, NON-INFRINGEMENT,
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ANALOG DEVICES BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, INTELLECTUAL PROPERTY RIGHTS, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "Ad9361AdcCore.h"
#include "Ad9361Util.h"
#include "Ad9361_defines.h"

/******************************************************************************/
/************************ Variables Definitions *******************************/
/******************************************************************************/

/***************************************************************************//**
 * @brief Ad9361AdcCore_Read
*******************************************************************************/
void Ad9361AdcCore_Read(struct Ad9361_stPhy *phy, uint32_t regAddr, uint32_t *data)
{
	*data = *(unsigned int *)(phy->uiRxBaseAddr + regAddr);
}

/***************************************************************************//**
 * @brief Ad9361AdcCore_Write
*******************************************************************************/
void Ad9361AdcCore_Write(struct Ad9361_stPhy *phy, uint32_t regAddr, uint32_t data)
{
	*(unsigned int *)(phy->uiRxBaseAddr + regAddr) = data;
}

/***************************************************************************//**
 * @brief Ad9361AdcCore_ReadValue
*******************************************************************************/
unsigned int Ad9361AdcCore_ReadValue(struct Ad9361_stPhy *phy, unsigned long reg)
{
	uint32_t val;

	Ad9361AdcCore_Read(phy, reg, &val);

	return val;
}

/***************************************************************************//**
 * @brief Ad9361AdcCore_Init
*******************************************************************************/
void Ad9361AdcCore_Init(struct Ad9361_stPhy *phy)
{
	Ad9361AdcCore_Write(phy, ADC_REG_RSTN, 0);
	Ad9361AdcCore_Write(phy, ADC_REG_RSTN, ADC_RSTN);

	Ad9361AdcCore_Write(phy, ADC_REG_CHAN_CNTRL(0),
		ADC_IQCOR_ENB | ADC_FORMAT_SIGNEXT | ADC_FORMAT_ENABLE | ADC_ENABLE);
	Ad9361AdcCore_Write(phy, ADC_REG_CHAN_CNTRL(1),
		ADC_IQCOR_ENB | ADC_FORMAT_SIGNEXT | ADC_FORMAT_ENABLE | ADC_ENABLE);

	if(phy->pdata->rx2tx2)
	{
		Ad9361AdcCore_Write(phy, ADC_REG_CHAN_CNTRL(2),
			ADC_IQCOR_ENB | ADC_FORMAT_SIGNEXT | ADC_FORMAT_ENABLE | ADC_ENABLE);
		Ad9361AdcCore_Write(phy, ADC_REG_CHAN_CNTRL(3),
			ADC_IQCOR_ENB | ADC_FORMAT_SIGNEXT | ADC_FORMAT_ENABLE | ADC_ENABLE);
	}
}

/***************************************************************************//**
 * @brief Ad9361AdcCore_ePnSel
*******************************************************************************/
int Ad9361AdcCore_SelPn(struct Ad9361_stPhy *phy, int channel, enum Ad9361AdcCore_ePnSel sel)
{
	unsigned reg;

	uint32_t version = Ad9361AdcCore_ReadValue(phy, 0x4000);

	if (PCORE_VERSION_MAJOR(version) > 7) {
		reg = Ad9361AdcCore_ReadValue(phy, ADC_REG_CHAN_CNTRL_3(channel));
		reg &= ~ADC_ADC_PN_SEL(~0);
		reg |= ADC_ADC_PN_SEL(sel);
		Ad9361AdcCore_Write(phy, ADC_REG_CHAN_CNTRL_3(channel), reg);
	} else {
		reg = Ad9361AdcCore_ReadValue(phy, ADC_REG_CHAN_CNTRL(channel));

		if (sel == ADC_PN_CUSTOM) {
			reg |= ADC_PN_SEL;
		} else if (sel == ADC_PN9) {
			reg &= ~ADC_PN23_TYPE;
			reg &= ~ADC_PN_SEL;
		} else {
			reg |= ADC_PN23_TYPE;
			reg &= ~ADC_PN_SEL;
		}

		Ad9361AdcCore_Write(phy, ADC_REG_CHAN_CNTRL(channel), reg);
	}

	return 0;
}


