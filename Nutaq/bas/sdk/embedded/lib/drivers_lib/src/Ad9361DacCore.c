/***************************************************************************//**
 *   @file   Ad9361DacCore.c
 *   @brief  Implementation of DAC Core Driver.
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
#include "Ad9361_defines.h"
#include "Ad9361DacCore.h"
#include "Ad9361Util.h"

/******************************************************************************/
/************************ Variables Definitions *******************************/
/******************************************************************************/

/******************************************************************************/
/********************** Macros and Constants Definitions **********************/
/******************************************************************************/

/***************************************************************************//**
 * @brief Ad9361DacCore_Read
*******************************************************************************/
void Ad9361DacCore_Read(struct Ad9361_stPhy *phy, uint32_t regAddr, uint32_t *data)
{
	*data = *(unsigned int *)(phy->uiTxBaseAddr + regAddr);
}

/***************************************************************************//**
 * @brief Ad9361DacCore_Write
*******************************************************************************/
void Ad9361DacCore_Write(struct Ad9361_stPhy *phy, uint32_t regAddr, uint32_t data)
{
	*(unsigned int *)(phy->uiTxBaseAddr + regAddr) = data;
}

/***************************************************************************//**
 * @brief dac_stop
*******************************************************************************/
void dac_stop(struct Ad9361_stPhy *phy)
{
	if (PCORE_VERSION_MAJOR(phy->dac_st.pcore_version) < 8)
	{
		Ad9361DacCore_Write(phy, DAC_REG_CNTRL_1, 0);
	}
}

/***************************************************************************//**
 * @brief Ad9361DacCore_Sync
*******************************************************************************/
void Ad9361DacCore_Sync(struct Ad9361_stPhy *phy, uint8_t force_on)
{
	if (PCORE_VERSION_MAJOR(phy->dac_st.pcore_version) < 8)
	{
		Ad9361DacCore_Write(phy, DAC_REG_CNTRL_1, (phy->dac_st.enable || force_on) ? DAC_ENABLE : 0);
	}
	else
	{
		Ad9361DacCore_Write(phy, DAC_REG_CNTRL_1, DAC_SYNC);
	}
}

/***************************************************************************//**
 * @brief Ad9361DacCore_Init
*******************************************************************************/
void Ad9361DacCore_Init(struct Ad9361_stPhy *phy, uint8_t data_sel)
{
	uint32_t reg_ctrl_2;
	Ad9361DacCore_Write(phy, DAC_REG_RSTN, 0x0);
	Ad9361DacCore_Write(phy, DAC_REG_RSTN, DAC_RSTN | DAC_MMCM_RSTN);

	phy->dac_st.dac_clk = &phy->clks[TX_SAMPL_CLK]->rate;
	phy->dac_st.rx2tx2 = phy->pdata->rx2tx2;
	Ad9361DacCore_Read(phy, DAC_REG_CNTRL_2, &reg_ctrl_2);
	if(phy->dac_st.rx2tx2)
	{
		phy->dac_st.num_dac_channels = 8;
		Ad9361DacCore_Write(phy, DAC_REG_RATECNTRL, DAC_RATE(3));
		reg_ctrl_2 &= ~DAC_R1_MODE;
	}
	else
	{
		phy->dac_st.num_dac_channels = 4;
		Ad9361DacCore_Write(phy, DAC_REG_RATECNTRL, DAC_RATE(1));
		reg_ctrl_2 |= DAC_R1_MODE;
	}
	Ad9361DacCore_Write(phy, DAC_REG_CNTRL_2, reg_ctrl_2);

	Ad9361DacCore_Read(phy, DAC_REG_VERSION, &phy->dac_st.pcore_version);

	dac_stop(phy);
	switch (data_sel) {
	case DATA_SEL_USER:
		Ad9361DacCore_Write(phy, DAC_REG_CNTRL_2, 0);
		Ad9361DacCore_SetDataSel(phy, -1, DATA_SEL_USER);
		break;
	default:
		break;
	}
	phy->dac_st.enable = true;
	Ad9361DacCore_Sync(phy, 0);
}

/***************************************************************************//**
 * @brief Ad9361DacCore_SetDataSel
*******************************************************************************/
int32_t Ad9361DacCore_SetDataSel(struct Ad9361_stPhy *phy, int32_t chan, enum Ad9361DacCore_eDataSel sel)
{
	if (PCORE_VERSION_MAJOR(phy->dac_st.pcore_version) > 7) {
		if (chan < 0) { /* ALL */
			int i;
			for (i = 0; i < phy->dac_st.num_dac_channels; i++) {
				Ad9361DacCore_Write(phy, DAC_REG_CHAN_CNTRL_7(i), sel);
				phy->dac_st.cached_datasel[i] = sel;
			}
		} else {
			Ad9361DacCore_Write(phy, DAC_REG_CHAN_CNTRL_7(chan), sel);
			phy->dac_st.cached_datasel[chan] = sel;
		}
	} else {
		uint32_t reg;

		switch(sel) {
		case DATA_SEL_SED:
		case DATA_SEL_USER:
			Ad9361DacCore_Read(phy, DAC_REG_CNTRL_2, &reg);
			reg &= ~DAC_DATA_SEL(~0);
			reg |= DAC_DATA_SEL(sel);
			Ad9361DacCore_Write(phy, DAC_REG_CNTRL_2, reg);
			break;
		default:
			return eAd9361InvEnum;
		}
	}

	return 0;
}

/***************************************************************************//**
 * @brief Ad9361DacCore_GetDataSel
*******************************************************************************/
void Ad9361DacCore_GetDataSel(struct Ad9361_stPhy *phy, int32_t chan, enum Ad9361DacCore_eDataSel *sel)
{
	*sel = phy->dac_st.cached_datasel[chan];
}

/***************************************************************************//**
 * @brief Ad9361DacCore_ReadValue
*******************************************************************************/
unsigned int Ad9361DacCore_ReadValue(struct Ad9361_stPhy *phy, unsigned long reg)
{
	uint32_t val;

	Ad9361DacCore_Read(phy, reg, &val);

	return val;
}
